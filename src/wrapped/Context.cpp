#include "wrapped/Context.hpp"

#include "native/utils/GlobalThreadPool.hpp"
#include "wrapped/utils/NativeEvent.hpp"

using namespace nglpmt;
using namespace nglpmt::js;

Napi::Function Context::getJsConstructor(Napi::Env env) {
    // This method is used to hook the accessor and method callbacks
    return DefineClass(env, "Context", {
        InstanceMethod<&Context::release>("release", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::run>("run", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::onRun>("onRun", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),

        InstanceMethod<&Context::onKeyAdd>("onKeyAdd", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::onKeyRemove>("onKeyRemove", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::onKeyClear>("onKeyClear", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    });
}

namespace {
    static const native::Context::Parameters default_params{.title = "Context", .width = 640, .height = 480};
    static std::vector<Napi::Value> onKeyConvert(Napi::Env env,
                                                 const std::tuple<const std::shared_ptr<native::Context>, const int, const int, const int, const int> args){
        auto key = Napi::BigInt::New(env, static_cast<int64_t>(std::get<1>(args)));
        auto scancode = Napi::BigInt::New(env, static_cast<int64_t>(std::get<2>(args)));
        auto action = Napi::BigInt::New(env, static_cast<int64_t>(std::get<3>(args)));
        auto mods = Napi::BigInt::New(env, static_cast<int64_t>(std::get<4>(args)));
        return std::vector<Napi::Value>{key, scancode, action, mods};
    }
}

Context::Context(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Context>(info),
    _native(_getMap().getOrMake(info[0].As<Napi::String>(), &native::Context::make, default_params)),
    _on_key(_native->onKey, &onKeyConvert){
}

Context::~Context(){
}

std::shared_ptr<native::Context> Context::getNative(){
    return _native;
}

Napi::Value Context::release(const Napi::CallbackInfo& info){
    std::cout << __FUNCTION__ << std::endl;
    _native.reset();
    // _on_run.reset();
    std::cout << __FUNCTION__ << std::endl;
    return info.Env().Null();
}

Napi::Value Context::run(const Napi::CallbackInfo& info){
    auto deffered = Napi::Promise::Deferred::New(info.Env());
    auto resolve = Napi::Function::New(info.Env(), [deffered](const Napi::CallbackInfo& info){deffered.Resolve(deffered.Env().Null());});
    auto tsfn = Napi::TypedThreadSafeFunction<>::New(info.Env(), resolve, __FUNCTION__, 0, 1);

    _native->onFinish->addActionQueued([tsfn](){
        tsfn.NonBlockingCall();
        tsfn.Release();
        return false;
    });
    _native->run();

    return deffered.Promise();
}

Napi::Value Context::onRun(const Napi::CallbackInfo& info){
    // return _on_run.addAction(info);
    return info.Env().Null();
}

Napi::Value Context::onKeyAdd(const Napi::CallbackInfo& info){
    return _on_key.addAction(info);
}

Napi::Value Context::onKeyRemove(const Napi::CallbackInfo& info){
    return _on_key.removeAction(info);
}

Napi::Value Context::onKeyClear(const Napi::CallbackInfo& info){
    return _on_key.clear(info);
}

MapMT<std::u16string, native::Context>& Context::_getMap(){
    static MapMT<std::u16string, native::Context> map;
    return map;
}