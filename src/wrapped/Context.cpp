#include "wrapped/Context.hpp"

#include "native/utils/GlobalThreadPool.hpp"
#include "wrapped/utils/NativeEvent.hpp"

using namespace nglpmt;
using namespace nglpmt::js;

Napi::Function Context::createJsConstructor(Napi::Env env) {
    // This method is used to hook the accessor and method callbacks
    return DefineClass(env, "Context", {
        InstanceMethod<&Context::release>("release", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::start>("start", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::onRun>("onRun", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
    });
}

Context::Context(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Context>(info){
    static const native::Context::Parameters default_params{.title = _name, .width = 640, .height = 480};

    _native = _getMap().getOrMake(info[0].As<Napi::String>(), &native::Context::make, default_params);
    _on_run = NativeEvent<std::weak_ptr<native::Context>, const std::chrono::milliseconds&>::make(
        _native->onRun,
        [](Napi::Env env, std::weak_ptr<native::Context> ctx, const std::chrono::milliseconds& ms){
            return std::vector<Napi::Value>({Napi::Number::New(env, ms.count())});
        }
    );
}

Context::~Context(){
}

Napi::Value Context::release(const Napi::CallbackInfo& info){
    std::cout << __FUNCTION__ << std::endl;
    _native.reset();
    _on_run.reset();
    std::cout << __FUNCTION__ << std::endl;
    return info.Env().Null();
}

Napi::Value Context::start(const Napi::CallbackInfo& info){
    auto deffered = Napi::Promise::Deferred::New(info.Env());

    auto future = native::GlobalThreadPool::get()->submit([native = _native, deffered](){
        native->run();
        deffered.Resolve(Napi::Boolean::New(deffered.Env(), true));
    });
    return deffered.Promise();
}

Napi::Value Context::onRun(const Napi::CallbackInfo& info){

    if (info.Length() == 1){
        _on_run->push(info[0].As<Napi::Function>());
    }
    // else if (info[1].IsNumber())
    // {
    //     auto repeats_left = new int(info[1].As<Napi::Number>().Int32Value());
    //     _native->onRun->push([action, repeats_left](const std::chrono::milliseconds& ms){
    //         unsigned int* count = new unsigned int(ms.count());
    //         action.NonBlockingCall(count, &Context::_jsOnRunActionCallback);
            
    //         --*repeats_left;
    //         if (*repeats_left == 0){
    //             action.Release();
    //             delete repeats_left;
    //         }
    //     }, [repeats_left](){
    //         return *repeats_left == 1 ? native::EventConditionResult::Once
    //                                   : native::EventConditionResult::Continue;
    //     });
    // }
    else if (info[1].IsFunction()){
        // NativeEvent::push_js_with_condition<std::shared_ptr<native::Context>, const std::chrono::milliseconds&>(
        //     *_native->onRun,
        //     info[0].As<Napi::Function>(),
        //     info[1].As<Napi::Function>(),
        //     [](Napi::Env env, std::shared_ptr<native::Context> ctx, const std::chrono::milliseconds& ms){
        //         return std::vector<Napi::Value>({Napi::Number::New(env, ms.count())});
        //     }
        // );

        // auto condition = Napi::ThreadSafeFunction::New(
        //     info.Env(), info[1].As<Napi::Function>(), "nglpmt::Context.onRun condition", 100, 1
        // );

        // _native->onRun->push([action](const std::chrono::milliseconds& ms){
        //     unsigned int* count = new unsigned int(ms.count());
        //     action.NonBlockingCall(count, &Context::_jsOnRunActionCallback);
        // }, [action, condition](const std::chrono::milliseconds& ms){
        //     std::cout << "Condition" << std::endl;
        //     auto data = new jsOnRunConditionCallbackData;
        //     data->ms = ms.count();
        //     data->result = false;
        //     auto future = data->barrier.get_future();

        //     condition.BlockingCall(data, &_jsOnRunConditionCallback);
        //     future.wait();

        //     std::cout << "Done " << data << std::endl;

        //     bool result = data->result;
        //     delete data;

        //     if (result){
        //         return native::EventConditionResult::Continue;
        //     } else {
        //         action.Release();
        //         condition.Release();
        //         return native::EventConditionResult::Free;
        //     }
        // });

    }

    return info.Env().Null();
}

namespace {

// struct OnKeyData {
//     native::Key key;
//     int scancode;
//     native::KeyAction action;
//     native::KeyMods mods;
// };

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

MapMT<std::u16string, native::Context>& Context::_getMap(){
    static MapMT<std::u16string, native::Context> map;
    return map;
}