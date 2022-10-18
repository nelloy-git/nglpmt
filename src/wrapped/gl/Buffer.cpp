#include "wrapped/gl/Buffer.hpp"

#include <unordered_map>
#include "glad/gl.h"

#include "wrapped/Context.hpp"
#include "wrapped/utils/EnumMap.hpp"

#include "wrapped/gl/Val.hpp"

using namespace nglpmt;
using namespace nglpmt::js;

Napi::Function GlBuffer::getJsConstructor(Napi::Env env) {
    // This method is used to hook the accessor and method callbacks
    return DefineClass(env, "GlBuffer", {
        InstanceMethod<&GlBuffer::data>("data", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
        InstanceMethod<&GlBuffer::getUsage>("getUsage", static_cast<napi_property_attributes>(napi_writable | napi_configurable)),
    });
}

GlBuffer::GlBuffer(const Napi::CallbackInfo& info) :
    Napi::ObjectWrap<GlBuffer>(info),
    _native(native::GlBuffer::make(Context::Unwrap(info[0].As<Napi::Object>())->getNative())){
}

GlBuffer::~GlBuffer(){
}

Napi::Value GlBuffer::data(const Napi::CallbackInfo& info){
    static const EnumMap<native::Enum> name2usage {
        {"GL_STREAM_DRAW", GL_STREAM_DRAW},
        {"GL_STREAM_READ", GL_STREAM_READ},
        {"GL_STREAM_COPY", GL_STREAM_COPY},
        {"GL_STATIC_DRAW", GL_STATIC_DRAW},
        {"GL_STATIC_READ", GL_STATIC_READ},
        {"GL_STATIC_COPY", GL_STATIC_COPY},
        {"GL_DYNAMIC_DRAW", GL_DYNAMIC_DRAW},
        {"GL_DYNAMIC_READ", GL_DYNAMIC_READ},
        {"GL_DYNAMIC_COPY", GL_DYNAMIC_COPY},
    };

    auto js_usage = info[0];
    auto js_data = info[1].As<Napi::Float32Array>();

    native::Val<native::Enum> usage(0);
    if (js_usage.IsString()){
        auto found = name2usage.find(js_usage.As<Napi::String>().Utf8Value());
        if (!found){
            Napi::Error::New(info.Env(), "Wrong \"usage\". Available: " + name2usage.available()).ThrowAsJavaScriptException();
            return info.Env().Null();
        }
        usage = found.value();
    } else {
        try {
            usage = GlVal<native::Enum>::Unwrap(js_usage.As<Napi::Object>())->getNative();
        } catch (std::exception& e){
            Napi::Error::New(info.Env(), "Wrong \"usage\". Available: " + name2usage.available()).ThrowAsJavaScriptException();
            return info.Env().Null();
        }
    }


    auto length = js_data.ElementLength();
    auto data = std::make_shared<float[]>(length);
    memcpy(data.get(), js_data.Data(), sizeof(float) * length);

    const native::Val<const native::Enum> u(usage);
    _native->data(length, data, u);
    return info.Env().Null();
}

Napi::Value GlBuffer::storage(const Napi::CallbackInfo& info){
    // static const EnumMap<native::Enum> name2usage {
    //     {"GL_STREAM_DRAW", GL_STREAM_DRAW},
    //     {"GL_STREAM_READ", GL_STREAM_READ},
    //     {"GL_STREAM_COPY", GL_STREAM_COPY},
    //     {"GL_STATIC_DRAW", GL_STATIC_DRAW},
    //     {"GL_STATIC_READ", GL_STATIC_READ},
    //     {"GL_STATIC_COPY", GL_STATIC_COPY},
    //     {"GL_DYNAMIC_DRAW", GL_DYNAMIC_DRAW},
    //     {"GL_DYNAMIC_READ", GL_DYNAMIC_READ},
    //     {"GL_DYNAMIC_COPY", GL_DYNAMIC_COPY},
    // };

    // auto js_usage = info[0].As<Napi::String>().Utf8Value();
    // auto js_data = info[1].As<Napi::Float32Array>();

    // auto usage = name2usage.find(js_usage);
    // if (!usage){
    //     Napi::Error::New(info.Env(), "Wrong \"usage\". Available: " + name2usage.available()).ThrowAsJavaScriptException();
    //     return info.Env().Null();
    // }

    // auto length = js_data.ElementLength();
    // auto data = std::make_shared<float[]>(length);
    // memcpy(data.get(), js_data.Data(), sizeof(float) * length);

    // _native->data(length, data, usage.value());
    return info.Env().Null();
}

Napi::Value GlBuffer::getUsage(const Napi::CallbackInfo& info){
    native::Val<native::Enum> dst(0);
    _native->getUsage(dst);

    return GlVal<native::Enum>::jsCreate(info.Env(), dst);;
}