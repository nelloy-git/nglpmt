#pragma once

#include "napi.h"
#include "wrapped/utils/MapMT.hpp"
#include "native/utils/Event.hpp"

namespace nglpmt::NativeEvent {

template<typename ... Args>
struct Data {
    std::tuple<Args...> native_data;
    const std::function<std::vector<Napi::Value>(Napi::Env, Args...)> convert;

    void convert_data(Napi::Env env){
        auto f = std::bind(convert, env);
        return std::apply(f, native_data);
    }
};

template<typename ... Args>
static void jsNativeCallback(Napi::Env env, Napi::Function jsCallback, Data<Args...>* data){
    jsCallback.Call(data->convert_data());
    delete data;
};

template<typename ... Args>
static void runOnce(native::Event<Args...>& event,
                    Napi::Function js_action,
                    const std::function<std::vector<Napi::Value>(Napi::Env, Args...)>& convert){
    auto action_tsfn = Napi::ThreadSafeFunction::New(
        js_action.Env(), js_action, "nglpmt::Context.onRun callback", 0, 1
    );

    event.push([action_tsfn, convert](Args... args){
        auto p_data = new Data<Args...>({
            .native_data = std::make_tuple(args...);
            .convert = convert;
        });
        action_tsfn.NonBlockingCall(p_data, &jsNativeCallback<Args...>);
        action_tsfn.Release();
    });
}

}