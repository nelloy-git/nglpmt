#pragma once

#include "napi.h"

#include "native/gl/Buffer.hpp"

namespace nglpmt::js {
    
class GlBuffer : public Napi::ObjectWrap<GlBuffer> {
public:
    static Napi::Function getJsConstructor(Napi::Env env);

    GlBuffer(const Napi::CallbackInfo& info);
    ~GlBuffer();

    Napi::Value data(const Napi::CallbackInfo& info);
    Napi::Value storage(const Napi::CallbackInfo& info);
    Napi::Value getUsage(const Napi::CallbackInfo& info);

private:
    std::shared_ptr<native::GlBuffer> _native;
};

} // namespace nglpmt::js