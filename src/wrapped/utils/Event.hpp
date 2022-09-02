#pragma once

#include <variant>

#include "napi.h"
#include "native/utils/Event.hpp"

namespace nglpmt {

struct EventArg {
    EventArg(napi_valuetype type) :
        type(type){
    }
    EventArg(napi_valuetype type, const auto&& value) :
        type(type),
        value(value){
    }

    napi_valuetype type;
    std::variant<std::monostate, bool, double, std::u16string, std::shared_ptr<std::vector<std::pair<EventArg, EventArg>>>> value;
};

class Event : public Napi::ObjectWrap<Event> {
public:
    static void Init(Napi::Env env, Napi::Object exports){
        auto constructor_func = DefineClass(env, "Event", {
            InstanceMethod<&Event::emit>("emit", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
            InstanceMethod<&Event::push>("push", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        });

        exports.Set("Event", constructor_func);
    };

    Event(const Napi::CallbackInfo& info) :
        Napi::ObjectWrap<Event>(info),
        _native(native::Event<const std::vector<EventArg>&>::make()){
    };
    ~Event(){
    };

    Napi::Value push(const Napi::CallbackInfo& info){
        auto tsfn = Napi::ThreadSafeFunction::New(
            info.Env(), info[0].As<Napi::Function>(), "nglpmt::Context.onRun callback", 0, 1
        );

        _push_once(tsfn);
        return info.Env().Null();
    }

    Napi::Value emit(const Napi::CallbackInfo& info){
        for (int i = 0; i < info.Length(); ++i){
            if (!isValidEmitArg(info[i])){
                Napi::Error::New(info.Env(), "Argument " + std::to_string(i) + " has unavalable type.").ThrowAsJavaScriptException();
            }
        }

        std::vector<EventArg> args;
        for (int i = 0; i < info.Length(); ++i){
            args.push_back(toEventArg(info[i]));
        }

        auto deffered = Napi::Promise::Deferred::New(info.Env());
        auto release_callback = Napi::Function::New(info.Env(), [deffered](const Napi::CallbackInfo& info){deffered.Resolve(info.Env().Null());});
        auto tsfn = Napi::ThreadSafeFunction::New(info.Env(), release_callback, "nglpmt::Context.wait callback", 0, 1);

        std::thread th([native = _native, args, tsfn](){
            std::cout << args.size() << std::endl;
            native->emit(args);
            tsfn.NonBlockingCall();
            tsfn.Release();
        });
        th.detach();
        return deffered.Promise();
    }

private:
    std::shared_ptr<native::Event<const std::vector<EventArg>&>> _native;

    static void _jsCallback(Napi::Env env, Napi::Function jsCallback, const std::vector<EventArg>* args){
        std::cout << "JS: " << args->size() << std::endl;

        std::vector<napi_value> js_args;
        for (int i = 0; i < args->size(); ++i){
            js_args.push_back(fromEventArg(env, args->at(i)));
        }
        jsCallback.Call(js_args);
        delete args;
    };

    void _push_once(const Napi::ThreadSafeFunction& tsfn){
        _native->push([tsfn](const std::vector<EventArg>& args){
            auto copy = new std::vector<EventArg>(args);
            tsfn.NonBlockingCall(copy, &_jsCallback);
            tsfn.Release();
        });
    }

    static bool isValidEmitArg(const Napi::Value& arg){
        auto type = arg.Type();
        switch (type){
        case napi_undefined: return true;
        case napi_null:      return true;
        case napi_boolean:   return true;
        case napi_number:    return true;
        case napi_string:    return true;

        case napi_object: {
            auto obj = arg.As<Napi::Object>();
            for (const auto& field : obj){
                if (!isValidEmitArg(field.first) || !isValidEmitArg(field.second)){
                    return false;
                }
            }
            return true;
        }

        case napi_symbol:    return false;
        case napi_function:  return false;
        case napi_external:  return false;
        
        default: return false;
        }
    }

    static EventArg toEventArg(const Napi::Value& arg){
        auto type = arg.Type();

        switch (type){
        case napi_undefined: return EventArg(type);
        case napi_null:      return EventArg(type);
        case napi_boolean:   return EventArg(type, arg.As<Napi::Boolean>().Value());
        case napi_number:    return EventArg(type, arg.As<Napi::Number>().DoubleValue());
        case napi_string:    return EventArg(type, arg.As<Napi::String>().Utf16Value());

        case napi_object: {
            auto obj = std::make_shared<std::vector<std::pair<EventArg, EventArg>>>();
            for (const auto& field : arg.As<Napi::Object>()){
                obj->push_back({toEventArg(field.first), toEventArg(field.second)});
            }
            return EventArg(type, std::move(obj));
        }
        
        default: return EventArg(type);
        }

    }

    static Napi::Value fromEventArg(const Napi::Env& env, const EventArg& arg){
        switch (arg.type){
        case napi_undefined: return env.Undefined();
        case napi_null:      return env.Null();
        case napi_boolean:   return Napi::Boolean::New(env, std::get<bool>(arg.value));
        case napi_number:    return Napi::Number::New(env, std::get<double>(arg.value));
        case napi_string:    return Napi::String::New(env, std::get<std::u16string>(arg.value));

        case napi_object: {
            auto obj = std::get<std::shared_ptr<std::vector<std::pair<EventArg, EventArg>>>>(arg.value);
            auto res = Napi::Object::New(env);
            for (int i = 0; i < obj->size(); ++i){
                res.Set(fromEventArg(env, obj->at(i).first), fromEventArg(env, obj->at(i).second));
            }
            return res;
        }
        
        default: return env.Undefined();
        }

    }

    // void _pushOnce(Napi::ThreadSafeFunction action){
    //     _native->onRun->push([action](const std::chrono::milliseconds& ms){
    //         unsigned int* count = new unsigned int(ms.count());
    //         action.NonBlockingCall(count, &Context::_jsOnRunActionCallback);
    //         action.Release();
    //     });
    // }

    // static void _jsRunActionCallback(Napi::Env env, Napi::Function jsCallback, std::tuple<NapiArgs...>* args){
    //     jsCallback.Call(NapiArgs::Napi::New(env, args);
    // }

};

}