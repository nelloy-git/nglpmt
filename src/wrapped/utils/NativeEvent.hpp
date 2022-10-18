#pragma once

#include "napi.h"
#include "wrapped/utils/MapMT.hpp"
#include "native/utils/Event.hpp"

namespace nglpmt::js {

template<typename ... Args>
class EventWrapped {
    enum class ActionType {
        once,
        repeat,
        cond
    };
    
    using Native = native::Event<Args...>;
    using ID = Native::ID;
    using PackedArgs = std::tuple<Args...>;
    using Converter = std::function<std::vector<Napi::Value>(Napi::Env, const PackedArgs&)>;

    struct TsfnData {
        TsfnData(const ActionType& type,
                 const std::shared_ptr<Converter>& converter,
                 const std::shared_ptr<PackedArgs>& args) :
            converter(converter),
            args(args){
            if (type == ActionType::cond){
                promise = std::promise<bool>();
            }
        }

        std::shared_ptr<Converter> converter;
        std::shared_ptr<PackedArgs> args;
        std::optional<std::promise<bool>> promise;
    };

    static void _jsTsfnCallback(Napi::Env env,
                                Napi::Function jsCallback,
                                std::nullptr_t* context,
                                TsfnData* data);

    using Tsfn = Napi::TypedThreadSafeFunction<std::nullptr_t, TsfnData, &_jsTsfnCallback>;
    using IdMap = std::unordered_map<ID, std::pair<ActionType, Tsfn>>;

public:
    EventWrapped(const std::shared_ptr<native::Event<Args...>> event,
                 const Converter& converter);
    ~EventWrapped();

    Napi::Value addAction(const Napi::CallbackInfo& info);
    Napi::Value removeAction(const Napi::CallbackInfo& info);
    Napi::Value clear(const Napi::CallbackInfo& info);

protected:

private:
    const std::shared_ptr<std::mutex> _lock;
    const std::shared_ptr<IdMap> _active;
    const std::shared_ptr<Native> _native;
    const ID _native_id;

    static ID _getUniqId();
    static ActionType _getActionType(Napi::Env env, const std::string& s_type);

    static bool _runJsActions(std::shared_ptr<Converter> converter,
                              std::shared_ptr<std::mutex> lock,
                              std::shared_ptr<IdMap> actions,
                              std::shared_ptr<PackedArgs> packed_args);
};





template<typename ... Args>
inline EventWrapped<Args...>::EventWrapped(const std::shared_ptr<Native> event,
                                           const Converter& converter) :
    _lock(new std::mutex),
    _active(new IdMap),
    _native(event),
    _native_id(event->addActionQueued([converter = std::make_shared<Converter>(converter) , lock = _lock, active = _active](Args... args){
        auto packed_args = std::make_shared<PackedArgs>(args...);
        _runJsActions(converter, lock, active, packed_args);
        return true;
    }).first){
    
}

template<typename ... Args>
inline EventWrapped<Args...>::~EventWrapped(){
    _native->delActionNow(_native_id);
}

template<typename ... Args>
inline Napi::Value EventWrapped<Args...>::addAction(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    ActionType type = _getActionType(env, info[0].As<Napi::String>());
    Napi::Function func = info[1].As<Napi::Function>();

    auto tsfn = Tsfn::New(env, func, __FUNCTION__, 0, 1);
    auto id = _getUniqId();

    std::lock_guard lg(*_lock);
    _active->emplace(id, std::pair{type, tsfn});
    return Napi::BigInt::New(env, id);
}

template<typename ... Args>
inline Napi::Value EventWrapped<Args...>::removeAction(const Napi::CallbackInfo& info){
    auto env = info.Env();
    bool lossless;
    ID id = info[0].As<Napi::BigInt>().Uint64Value(&lossless);
    if (!lossless){
        Napi::Error::New(env, "is not BigInt").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    
    std::lock_guard lg(*_lock);
    auto iter = _active->find(id);
    if (iter == _active->end()){
        return Napi::Boolean::New(env, false);
    }
    iter->second.second.Release();
    _active->erase(iter);
    return Napi::Boolean::New(env, true);
}

template<typename ... Args>
inline Napi::Value EventWrapped<Args...>::clear(const Napi::CallbackInfo& info){
    std::lock_guard lg(*_lock);
    for (auto& action : *_active){
        action.second.second.Release();
    }
    _active->clear();
    return info.Env().Null();
}

template<typename ... Args>
inline void EventWrapped<Args...>::_jsTsfnCallback(Napi::Env env,
                                                   Napi::Function jsCallback,
                                                   std::nullptr_t* context,
                                                   TsfnData* data){
    auto result = jsCallback.Call((*data->converter)(env, *data->args));
    if (data->promise.has_value()){
        data->promise.value().set_value(result.IsBoolean() ? result.As<Napi::Boolean>() : false);
    }
    delete data;
}

template<typename ... Args>
inline EventWrapped<Args...>::ID EventWrapped<Args...>::_getUniqId(){
    static std::atomic<ID> id(0);
    return id.fetch_add(1);
}

template<typename ... Args>
inline EventWrapped<Args...>::ActionType EventWrapped<Args...>::_getActionType(Napi::Env env, const std::string& s_type){
    if (s_type == "once"){
        return ActionType::once;
    } else if (s_type == "repeat"){
        return ActionType::repeat;
    } else if (s_type == "cond"){
        return ActionType::cond;
    } else {
        throw Napi::Error::New(env, "Wrong push type. Got \"" + s_type + "\". Available: [\"once\", \"repeat\", \"cond\"]");
    }
}

template<typename ... Args>
inline bool EventWrapped<Args...>::_runJsActions(std::shared_ptr<Converter> converter,
                                                 std::shared_ptr<std::mutex> lock,
                                                 std::shared_ptr<IdMap> actions,
                                                 std::shared_ptr<PackedArgs> packed_args){
    std::lock_guard lg(*lock);
    IdMap new_actions;
    std::vector<std::pair<ID, std::future<bool>>> waiting;
    for (auto& action : *actions){
        const ID& id = action.first;
        const ActionType& type = action.second.first;
        Tsfn& tsfn = action.second.second;

        TsfnData* data = new TsfnData(type, converter, packed_args);
        switch (type){
        case ActionType::once:
            break;
        case ActionType::repeat:
            new_actions[id] = action.second;
            break;
        case ActionType::cond:
            waiting.emplace_back(id, data->promise.value().get_future());
            break;
        }

        tsfn.NonBlockingCall(data);
    }

    for (auto& action : waiting){
        auto repeat = action.second.get();
        if (repeat){
            new_actions[action.first] = actions->at(action.first);
        }
    }
    *actions = new_actions;

    return true;
}

} // namespace nglpmt::js