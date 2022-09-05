#include "wrapped/utils/Event.hpp"

#include <variant>

#include "native/utils/GlobalThreadPool.hpp"

using namespace nglpmt;

struct Event::Parameter {
    Parameter(const Napi::Value& value) :
        _type(value.Type()){
        switch (_type){
        case napi_undefined: break;
        case napi_null:      break;
        case napi_boolean:   _value = value.As<Napi::Boolean>().Value(); break;
        case napi_number:    _value = value.As<Napi::Number>().DoubleValue(); break;
        case napi_string:    _value = value.As<Napi::String>().Utf16Value(); break;
        case napi_object: {
            _value.emplace<std::vector<std::pair<Parameter, Parameter>>>();
            auto& list = std::get<std::vector<std::pair<Parameter, Parameter>>>(_value); 
            for (const auto& field : value.As<Napi::Object>()){
                list.push_back({
                    Parameter(field.first),
                    Parameter(field.second)
                });

                if (list.back().first.getError()){
                    _err = std::string(field.first.ToString())
                            + "->"
                            + list.back().first.getError().value();
                    break;
                }
                if (list.back().second.getError()){
                    _err = std::string(Napi::Value(field.second).ToString())
                            + "->"
                            + list.back().second.getError().value();
                    break;
                }
            }
        }; break;

        default: _err = "invalid type";
    }
    }

    std::optional<std::string>& getError(){
        return _err;
    }

    Napi::Value getValue(const Napi::Env& env){
        switch (_type){
        case napi_undefined: return env.Undefined();
        case napi_null:      return env.Null();
        case napi_boolean:   return Napi::Boolean::New(env, std::get<bool>(_value));
        case napi_number:    return Napi::Number::New(env, std::get<double>(_value));
        case napi_string:    return Napi::String::New(env, std::get<std::u16string>(_value));

        case napi_object: {
            auto obj = Napi::Object::New(env);
            auto& list = std::get<std::vector<std::pair<Parameter, Parameter>>>(_value);
            for (auto& pair : list){
                obj.Set(pair.first.getValue(env), pair.second.getValue(env));
            }
            return obj;
        }
        
        default: return env.Undefined();
        }
    }

private:
    napi_valuetype _type;
    std::optional<std::string> _err;
    std::variant<
        std::monostate,
        bool,
        double,
        std::u16string,
        std::vector<std::pair<Parameter, Parameter>>
    > _value;
};

void Event::Init(Napi::Env env, Napi::Object exports){
    auto constructor_func = DefineClass(env, "Event", {
        InstanceMethod<&Event::emit>("emit", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Event::push>("push", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    });

    exports.Set("Event", constructor_func);
};

Event::Event(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Event>(info){
    std::u16string name = info[0].As<Napi::String>();
    _native = _getMap().getOrMake(name, &NativeEvent::make, native::GlobalThreadPool::get(), native::GlobalThreadPool::get());
}

Event::~Event(){
}

Napi::Value Event::push(const Napi::CallbackInfo& info){
    auto tsfn = Napi::ThreadSafeFunction::New(
        info.Env(), info[0].As<Napi::Function>(), "nglpmt::Context.onRun callback", 0, 1
    );

    _push_once(tsfn);
    return info.Env().Null();
}

Napi::Value Event::emit(const Napi::CallbackInfo& info){
    auto params = std::make_shared<std::vector<Parameter>>();
    for (int i = 0; i < info.Length(); ++i){
        auto& param = params->emplace_back(info[i]);
        if (param.getError()){
            Napi::Error::New(info.Env(), param.getError().value()).ThrowAsJavaScriptException();
        }
    }

    auto deffered = Napi::Promise::Deferred::New(info.Env());
    auto release_callback = Napi::Function::New(info.Env(), [deffered](const Napi::CallbackInfo& info){deffered.Resolve(info.Env().Null());});
    auto tsfn = Napi::ThreadSafeFunction::New(info.Env(), release_callback, "nglpmt::Context.wait callback", 0, 1);

    native::GlobalThreadPool::get()->submit([native = _native, params, tsfn](){
        native->emit(params);
        tsfn.NonBlockingCall();
        tsfn.Release();
    });
    
    return deffered.Promise();
}

MapMT<std::u16string, Event::NativeEvent>& Event::_getMap(){
    static MapMT<std::u16string, Event::NativeEvent> map;
    return map;
}

void Event::_jsCallback(Napi::Env env,
                        Napi::Function jsCallback,
                        std::shared_ptr<std::vector<Parameter>>* ptr){
    std::vector<Napi::Value> js_args;
    for (auto& param : *ptr->get()){
        js_args.push_back(param.getValue(env));
    }
    jsCallback.Call(js_args);
    delete ptr;
};

void Event::_push_once(const Napi::ThreadSafeFunction& tsfn){
    _native->push([tsfn](const std::shared_ptr<std::vector<Parameter>> params){
        auto copy = new std::shared_ptr<std::vector<Parameter>>(params);
        tsfn.NonBlockingCall(copy, &_jsCallback);
        tsfn.Release();
    });
}