#pragma once

#include "napi.h"

#include "native/gl/Types.hpp"
#include "native/utils/Val.hpp"

namespace nglpmt::js {

template<typename T>
class GlVal : public Napi::ObjectWrap<GlVal<T>> {
public:
    static Napi::Value jsCreate(Napi::Env env, native::Val<native::Enum>& init);

    GlVal(const Napi::CallbackInfo& info) :
        Napi::ObjectWrap<GlVal<T>>(info),
        _native(*static_cast<native::Val<T>*>(info.Data())){
    };
    ~GlVal(){};

    Napi::Value value(const Napi::CallbackInfo& info);

    native::Val<T> getNative(){
        return _native;
    }

private:
    native::Val<T> _native;
};

template<>
Napi::Value GlVal<native::Enum>::jsCreate(Napi::Env env, native::Val<native::Enum>& init){
    return DefineClass(env, "GlValEnum", {
        InstanceMethod<&GlVal<native::Enum>::value>("value", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    }, &init).New({});
}

template<>
Napi::Value GlVal<native::Enum>::value(const Napi::CallbackInfo& info){
    return Napi::Number::New(info.Env(), *_native);
}

} // namespace nglpmt::js