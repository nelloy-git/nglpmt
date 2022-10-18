#pragma once

#include "napi.h"
#include "native/utils/Event.hpp"
#include "native/utils/SharedObject.hpp"

namespace nglpmt {

template<typename ... Args>
using NativeEventConverter = std::function<std::vector<Napi::Value>(Napi::Env, Args...)>;

template<typename ... Args>
class NativeEventContext {
public:
    NativeEventContext(const NativeEventConverter<Args...> converter) :
        _converter(converter){
    }

    std::future<bool> getFuture(){
        return _promise.get_future();
    }

    bool hasResult(){
        return _has_result;
    }

    void setResult(bool result){
        _promise.set_value(result);
        _has_result = true;
    }

    std::vector<Napi::Value> convert(Napi::Env env, const std::tuple<Args...>& args){
        auto convert = [this, env](Args... args){
            return _converter(env, args...);
        };
        return std::apply(convert, args);
    }

private:
    bool _has_result = false;
    std::promise<bool> _promise;
    NativeEventConverter<Args...> _converter;
};

template<typename ... Args>
class NativeEvent : public native::SharedObject<NativeEvent<Args...>> {
    static void _jsNativeCallback(Napi::Env env, Napi::Function jsCallback, NativeEventContext<Args...>* context, std::tuple<Args...>* data){
        auto result = jsCallback.Call(context->convert(env, *data));
        // context->setResult(result.As<Napi::Boolean>());
        delete data;
    };
    using TSFN = Napi::TypedThreadSafeFunction<NativeEventContext<Args...>, std::tuple<Args...>, &NativeEvent::_jsNativeCallback>;

public:
    static std::shared_ptr<NativeEvent> make(const std::shared_ptr<native::Event<Args...>> event,
                                             const NativeEventConverter<Args...>& converter){
        return std::shared_ptr<NativeEvent>(new NativeEvent(event, converter));
    }

    ~NativeEvent(){
        for (auto& tsfn : _in_use){
            tsfn.Abort();
        }
    }

    void push(Napi::Function js_action){
        auto action_tsfn = TSFN::New(
            js_action.Env(), js_action, __FUNCTION__, 0, 1, new NativeEventContext<Args...>(_converter), &NativeEvent::_jsNativeFinalizer
        );
        _in_use.push_back(action_tsfn);

        // _native->push(
        //     [action_tsfn](Args... args){
        //         auto data = new std::tuple<Args...>(args...);
        //         std::future<bool> future = action_tsfn.GetContext()->getFuture();
        //         auto status = action_tsfn.NonBlockingCall(data);

        //         if (status != napi_ok){
        //             return false;
        //         }

        //         std::cout << "Wait" << std::endl;
        //         future.wait();
        //         auto repeat = future.get();
        //         std::cout << "Result: " << repeat << std::endl;
        //         if (!repeat){
        //             action_tsfn.Release();   
        //         }
        //         return repeat;
        //     }
        // );
    }

protected:
    NativeEvent(const std::shared_ptr<native::Event<Args...>> event,
                const NativeEventConverter<Args...>& converter) :
        _native(event),
        _converter(converter){
    }

private:
    std::shared_ptr<native::Event<Args...>> _native;
    NativeEventConverter<Args...> _converter;
    std::deque<TSFN> _in_use;
    
    static void _jsNativeFinalizer(Napi::Env env, void* finalizeData, NativeEventContext<Args...>* context){
        if (!context->hasResult()){
            context->setResult(false);
        }
        delete context;
        // delete finalizeData;
    }
};

} // namespace nglpmt