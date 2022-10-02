#pragma once

#include "napi.h"
#include "wrapped/utils/MapMT.hpp"
#include "native/utils/Event.hpp"

namespace nglpmt::js {

class Event : public Napi::ObjectWrap<Event> {

struct Parameter;
using NativeEvent = typename native::Event<const std::shared_ptr<std::vector<Parameter>>>;

public:
    static Napi::Function createJsConstructor(Napi::Env env);

    Event(const Napi::CallbackInfo& info);
    virtual ~Event();

    // info[0] - "once", "repeat", "cond"
    // info[1] - function
    // return BigInt action id for removal
    Napi::Value addActionQueued(const Napi::CallbackInfo& info);
    Napi::Value addActionNow(const Napi::CallbackInfo& info);

    Napi::Value delAction(const Napi::CallbackInfo& info);

    Napi::Value emitQueued(const Napi::CallbackInfo& info);
    Napi::Value emitNow(const Napi::CallbackInfo& info);

private:
    enum class ActionType {
        once,
        repeat,
        cond
    };

    using ID = NativeEvent::ID;
    static MapMT<std::u16string, NativeEvent>& _getMap();
    static ActionType _getActionType(Napi::Env env, const std::string& s_type);

    struct TsfnData {
        TsfnData(std::shared_ptr<std::vector<Parameter>> params) : params(params){};
        std::promise<bool> promise;
        std::shared_ptr<std::vector<Parameter>> params;
    };
    static void _jsTsfnCallback(Napi::Env env,
                                Napi::Function jsCallback,
                                std::nullptr_t* context,
                                TsfnData* data);
    using Tsfn = Napi::TypedThreadSafeFunction<std::nullptr_t, TsfnData, &Event::_jsTsfnCallback>;
    static bool _nativeTsfnCallback(ActionType type,
                                    Tsfn tsfn,
                                    const std::shared_ptr<std::vector<Parameter>> params);
    
    
    std::shared_ptr<std::atomic<bool>> _destroying;
    std::shared_ptr<NativeEvent> _native;
    std::shared_ptr<std::unordered_map<ID, Tsfn>> _id2tsfn;

    std::shared_ptr<std::vector<Parameter>> _convertArgs(const Napi::CallbackInfo& info);
    Napi::Promise _addJsPromise(Napi::Env env);
};

} // namespace nglpmt::js