#include "wrapped/Context.hpp"

using namespace nglpmt;

void Context::Init(Napi::Env& env, Napi::Object& exports) {
    // This method is used to hook the accessor and method callbacks
    Napi::Function func = DefineClass(env, "Context", {
        InstanceMethod<&Context::start>("start", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::wait>("wait", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::onRun>("onRun", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
    });

    exports.Set("Context", func);
}

Context::Context(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Context>(info){
    
    auto node_params = info[0].As<Napi::Object>();

    native::Context::Parameters params;
    params.title = node_params.Get("title").As<Napi::String>();
    params.width = node_params.Get("width").As<Napi::Number>();
    params.height = node_params.Get("height").As<Napi::Number>();

    _native = native::Context::make(params);
}

Context::~Context(){

}

Napi::Value Context::start(const Napi::CallbackInfo& info){
    return Napi::Boolean::New(info.Env(), _native->start());
}

Napi::Value Context::wait(const Napi::CallbackInfo& info){
    _native->wait();
    return info.Env().Null();
}

Napi::Value Context::onRun(const Napi::CallbackInfo& info){
    auto action = Napi::ThreadSafeFunction::New(
        info.Env(), info[0].As<Napi::Function>(), "nglpmt::Context.onRun callback", 0, 1
    );

    _native->onRun.push([action](native::Context* ctx, const std::chrono::microseconds& dt){
        auto callback = []( Napi::Env env, Napi::Function jsCallback, unsigned int* us) {
            jsCallback.Call( {Napi::Number::New( env, double(*us) )} );
            delete us;
        };

        unsigned int* us = new unsigned int(dt.count());
        action.NonBlockingCall(us, callback);
        action.Release();
    }, [](){return true;});

    return info.Env().Null();
}

namespace {

struct OnKeyData {
    native::Key key;
    int scancode;
    native::KeyAction action;
    native::KeyMods mods;
};

}

Napi::Value Context::onKey(const Napi::CallbackInfo& info){
    // static auto js_callback = []( Napi::Env env, Napi::Function jsCallback, OnKeyData* key_data) {
    //     // mods
    //     auto js_key_mods = Napi::Object::New(env);
    //     js_key_mods.Set("Shift", Napi::Boolean::New(env, key_data->mods.isMod(native::KeyMod::Shift)));
    //     js_key_mods.Set("Control", Napi::Boolean::New(env, key_data->mods.isMod(native::KeyMod::Control)));
    //     js_key_mods.Set("Alt", Napi::Boolean::New(env, key_data->mods.isMod(native::KeyMod::Alt)));
    //     js_key_mods.Set("Super", Napi::Boolean::New(env, key_data->mods.isMod(native::KeyMod::Super)));
    //     js_key_mods.Set("CapsLock", Napi::Boolean::New(env, key_data->mods.isMod(native::KeyMod::CapsLock)));
    //     js_key_mods.Set("NumLock", Napi::Boolean::New(env, key_data->mods.isMod(native::KeyMod::NumLock)));
    //     js_key_mods.Set("Unknown", Napi::Boolean::New(env, key_data->mods.isMod(native::KeyMod::Unknown)));

    //     auto js_key_data = Napi::Object::New(env);
    //     js_key_data.Set("key", Napi::String::New(env, native::keyToString(key_data->key)));
    //     js_key_data.Set("scancode", Napi::Number::New(env, key_data->scancode));
    //     js_key_data.Set("action", Napi::String::New(env, native::keyActionToString(key_data->action)));
    //     js_key_data.Set("mods", js_key_mods);

    //     jsCallback.Call( {js_key_data} );
    //     delete key_data;
    // };
    
    // auto action_napi = Napi::ThreadSafeFunction::New(
    //     info.Env(), info[0].As<Napi::Function>(), "nglpmt::Context.onRun callback", 0, 1
    // );

    // _native->onKey.push<[](){return true;}>([action_napi](native::Context* ctx, 
    //                                                       const native::Key& key,
    //                                                       const int& scancode,
    //                                                       const native::KeyAction& action,
    //                                                       const native::KeyMods& mods){
    //     auto* key_data = new OnKeyData{key, scancode, action, mods};
    //     action_napi.NonBlockingCall(key_data, js_callback);
    //     action_napi.Release();
    // });

    return info.Env().Null();
}

//     Napi::ObjectWrap<Example>(info) {
//     Napi::Env env = info.Env();

//     // Napi::Number value = info[0].As<Napi::Number>();
// }