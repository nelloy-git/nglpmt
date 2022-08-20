#include "wrapped/Context.hpp"

using namespace nglpmt;

std::mutex Context::_static_lock;
std::unordered_map<std::string, std::weak_ptr<native::Context>> Context::_reserved;

void Context::Init(Napi::Env env, Napi::Object exports) {
    // This method is used to hook the accessor and method callbacks
    Napi::Function constructor_func = DefineClass(env, "Context", {
        InstanceMethod<&Context::release>("release", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::start>("start", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::wait>("wait", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Context::onRun>("onRun", static_cast<napi_property_attributes>(napi_writable | napi_configurable))
    });

    exports.Set("Context", constructor_func);

    // Napi::FunctionReference* constructor = new Napi::FunctionReference();
    // *constructor = Napi::Persistent(constructor_func);

    // env.SetInstanceData<Napi::FunctionReference>(constructor);
}

Context::Context(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Context>(info){
    std::lock_guard lg(_static_lock);

    _name = info[0].As<Napi::String>();
    auto iter = _reserved.find(_name);

    std::shared_ptr<native::Context> native_ptr;
    if (iter == _reserved.end()){
        native_ptr = native::Context::make({.title = _name, .width = 640, .height = 480});
        _reserved[_name] = native_ptr;
    } else {
        native_ptr = iter->second.lock();
        if (!native_ptr){
            native_ptr = native::Context::make({.title = _name, .width = 640, .height = 480});
        }
        _reserved[_name] = native_ptr;
    }

    _native = native_ptr;
}

Context::~Context(){
}

Napi::Value Context::release(const Napi::CallbackInfo& info){
    _native.reset();
    return info.Env().Null();
}

Napi::Value Context::start(const Napi::CallbackInfo& info){
    auto deffered = Napi::Promise::Deferred::New(info.Env());
    auto release_callback = Napi::Function::New(info.Env(), [deffered](const Napi::CallbackInfo& info){deffered.Resolve(info.Env().Null());});
    auto tsfn = Napi::ThreadSafeFunction::New(info.Env(), release_callback, "nglpmt::Context.start callback", 0, 1);

    std::thread th([native = _native, tsfn](){
        native->start();
        tsfn.NonBlockingCall();
        tsfn.Release();
    });
    th.detach();
    return deffered.Promise();
    // return Napi::Boolean::New(info.Env(), _native->start());
}

Napi::Value Context::wait(const Napi::CallbackInfo& info){
    auto deffered = Napi::Promise::Deferred::New(info.Env());
    auto release_callback = Napi::Function::New(info.Env(), [deffered](const Napi::CallbackInfo& info){deffered.Resolve(info.Env().Null());});
    auto tsfn = Napi::ThreadSafeFunction::New(info.Env(), release_callback, "nglpmt::Context.wait callback", 0, 1);

    std::thread th([native = _native, tsfn](){
        native->wait();
        tsfn.NonBlockingCall();
        tsfn.Release();
    });
    th.detach();
    return deffered.Promise();
}

void Context::_jsOnRunActionCallback(Napi::Env env, Napi::Function jsCallback, unsigned int* ms){
    jsCallback.Call( {Napi::Number::New( env, double(*ms) )} );
    delete ms;
};

struct jsOnRunConditionCallbackData {
    std::promise<void> barrier;
    unsigned int ms;
    bool result;
};

void _jsOnRunConditionCallback(Napi::Env env, Napi::Function jsCallback, jsOnRunConditionCallbackData* data){
    std::cout << "Ptr: " << data << std::endl;
    auto js_result = jsCallback.Call( {Napi::Number::New( env, data->ms )} );
    data->result = js_result.As<Napi::Boolean>();
    data->barrier.set_value();
}

Napi::Value Context::onRun(const Napi::CallbackInfo& info){
    auto action = Napi::ThreadSafeFunction::New(
        info.Env(), info[0].As<Napi::Function>(), "nglpmt::Context.onRun callback", 0, 1
    );

    if (info.Length() == 1)
    {
        _native->onRun->push([action](const std::chrono::milliseconds& ms){
            unsigned int* count = new unsigned int(ms.count());
            action.NonBlockingCall(count, &Context::_jsOnRunActionCallback);
            action.Release();
        });
    }
    else if (info[1].IsNumber())
    {
        auto repeats_left = new int(info[1].As<Napi::Number>().Int32Value());
        _native->onRun->push([action, repeats_left](const std::chrono::milliseconds& ms){
            unsigned int* count = new unsigned int(ms.count());
            action.NonBlockingCall(count, &Context::_jsOnRunActionCallback);
            
            --*repeats_left;
            if (*repeats_left == 0){
                action.Release();
                delete repeats_left;
            }
        }, [repeats_left](){
            return *repeats_left == 1 ? native::EventConditionResult::Once
                                      : native::EventConditionResult::Continue;
        });
    }
    else if (info[1].IsFunction()){
        auto condition = Napi::ThreadSafeFunction::New(
            info.Env(), info[1].As<Napi::Function>(), "nglpmt::Context.onRun condition", 100, 1
        );

        _native->onRun->push([action](const std::chrono::milliseconds& ms){
            unsigned int* count = new unsigned int(ms.count());
            action.NonBlockingCall(count, &Context::_jsOnRunActionCallback);
        }, [action, condition](const std::chrono::milliseconds& ms){
            std::cout << "Condition" << std::endl;
            auto data = new jsOnRunConditionCallbackData;
            data->ms = ms.count();
            data->result = false;
            auto future = data->barrier.get_future();

            auto block_result = condition.BlockingCall(data, &_jsOnRunConditionCallback);
            future.wait();

            std::cout << "Done " << data << std::endl;

            bool result = data->result;
            delete data;

            if (result){
                return native::EventConditionResult::Continue;
            } else {
                action.Release();
                condition.Release();
                return native::EventConditionResult::Free;
            }
        });

    }

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