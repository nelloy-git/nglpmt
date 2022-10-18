#include "wrapped/utils/Event.hpp"

#include <optional>
#include <variant>

#include "native/utils/GlobalThreadPool.hpp"

using namespace nglpmt;
using namespace nglpmt::js;

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

Napi::Function Event::getJsConstructor(Napi::Env env){
    return DefineClass(env, "Event", {
        InstanceMethod<&Event::addActionQueued>("addActionQueued", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Event::addActionNow>("addActionNow", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Event::delAction>("delAction", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Event::emitQueued>("emitQueued", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&Event::emitNow>("emitNow", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    });
};

Event::Event(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<Event>(info),
    _destroying(new std::atomic<bool>(false)),
    _native(_getMap().getOrMake(info[0].As<Napi::String>(), &Native::make,
                                native::GlobalThreadPool::get())),
    _id2tsfn(new std::unordered_map<ID, Tsfn>){
}

Event::~Event(){
    *_destroying = true;
    for (auto& data : *_id2tsfn){
        static_cast<void>(_native->delActionNow(data.first));
    }
}

Napi::Value Event::addActionQueued(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    std::string s_type = info[0].As<Napi::String>();
    Napi::Function func = info[1].As<Napi::Function>();

    ActionType type = _getActionType(env, s_type);
    auto tsfn = Tsfn::New(env, func, __FUNCTION__, 0, 1);
    auto id = _native->addActionQueued(
        [type, tsfn, destroying = _destroying](const std::shared_ptr<std::vector<Parameter>> params){
            if (*destroying){return false;}
            return _nativeTsfnCallback(type, tsfn, params);
        }
    );
    _id2tsfn->insert_or_assign(id.first, tsfn);

    return Napi::BigInt::New(env, id.first);
}

Napi::Value Event::addActionNow(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    std::string s_type = info[0].As<Napi::String>();
    Napi::Function func = info[1].As<Napi::Function>();

    ActionType type = _getActionType(env, s_type);
    auto tsfn = Tsfn::New(env, func, __FUNCTION__, 0, 1);
    auto id = _native->addActionNow(
        [type, tsfn, destroying = _destroying](const std::shared_ptr<std::vector<Parameter>> params){
            if (*destroying){return false;}
            return _nativeTsfnCallback(type, tsfn, params);
        }
    );
    _id2tsfn->insert_or_assign(id.first, tsfn);

    return Napi::BigInt::New(env, id.first);
}

Napi::Value Event::clean(const Napi::CallbackInfo& info){
    for (auto& data : *_id2tsfn){
        static_cast<void>(_native->delActionNow(data.first));
    }
}

Napi::Value Event::delAction(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    bool lossless;
    ID id = info[0].As<Napi::BigInt>().Uint64Value(&lossless);

    if (!lossless){
        Napi::Error::New(env, "is not BigInt").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    auto iter = _id2tsfn->find(id);
    if (iter != _id2tsfn->end()){
        auto future = _native->delActionNow(id);
        try {
            future.get();
        } catch (const std::exception& e){
            std::cout << e.what() << std::endl;
        }
        

        iter->second.Release();
        _id2tsfn->erase(iter);
        return Napi::Boolean::New(env, true);
    }
    return Napi::Boolean::New(env, false);
}

Napi::Value Event::emitQueued(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    auto params = _convertArgs(info);
    auto promise = _addJsPromise(env);
    _native->emitQueued(params);
    return promise;
}

Napi::Value Event::emitNow(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    auto params = _convertArgs(info);
    auto promise = _addJsPromise(env);
    _native->emitNow(params);
    return promise;
}

MapMT<std::u16string, Event::Native>& Event::_getMap(){
    static MapMT<std::u16string, native::Event<const std::shared_ptr<std::vector<Parameter>>>> map;
    return map;
}

Event::ActionType Event::_getActionType(Napi::Env env, const std::string& s_type){
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

void Event::_jsTsfnCallback(Napi::Env env,
                            Napi::Function jsCallback,
                            std::nullptr_t* context,
                            TsfnData* data){
    std::vector<Napi::Value> js_args;
    for (auto& param : *data->params){
        js_args.push_back(param.getValue(env));
    }
    Napi::Value result = jsCallback.Call(js_args);
    data->promise.set_value(result.IsBoolean() ? result.As<Napi::Boolean>() : false);
    delete data;
}

bool Event::_nativeTsfnCallback(ActionType type,
                                Tsfn tsfn,
                                const std::shared_ptr<std::vector<Parameter>> params){
    auto data = new TsfnData{params};
    auto future = data->promise.get_future();
    auto status = tsfn.NonBlockingCall(data);

    if (status != napi_ok){
        std::cout << "ERROR: status != napi_ok " << __FUNCTION__ << std::endl;
        return false;
    }

    bool repeat;
    switch (type){
        case ActionType::once: 
            repeat = false;
            tsfn.Release();
            break;
        case ActionType::repeat:
            repeat = true;
            break;
        case ActionType::cond:
            repeat = future.get();
            if (!repeat){
                tsfn.Release();
            }
            break;
    }
    return repeat;
}

std::shared_ptr<std::vector<Event::Parameter>> Event::_convertArgs(const Napi::CallbackInfo& info){
    auto params = std::make_shared<std::vector<Parameter>>();
    for (size_t i = 0; i < info.Length(); ++i){
        auto& param = params->emplace_back(info[i]);
        if (param.getError()){
            Napi::Error::New(info.Env(), param.getError().value()).ThrowAsJavaScriptException();
        }
    }
    return params;
}

Napi::Promise Event::_addJsPromise(Napi::Env env){
    auto deffered = Napi::Promise::Deferred::New(env);
    auto resolve = Napi::Function::New(env, [deffered](const Napi::CallbackInfo& info){deffered.Resolve(deffered.Env().Null());});
    auto tsfn = Napi::TypedThreadSafeFunction<>::New(env, resolve, __FUNCTION__, 0, 1);

    _native->addActionQueued([tsfn](){
        tsfn.NonBlockingCall();
        tsfn.Release();
        return false;
    });
    return deffered.Promise();
}