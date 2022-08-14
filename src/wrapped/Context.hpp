#pragma once

#include <unordered_map>
#include <memory>

#include "napi.h"
#include "native/Context.hpp"

namespace nglpmt {

class Context : public Napi::ObjectWrap<Context> {
public:
    static void Init(Napi::Env& env, Napi::Object& exports);

    Context(const Napi::CallbackInfo& info);
    ~Context();

    Napi::Value start(const Napi::CallbackInfo& info);
    Napi::Value wait(const Napi::CallbackInfo& info);
    Napi::Value getThreadId(const Napi::CallbackInfo& info);

    Napi::Value onRun(const Napi::CallbackInfo& info);
    Napi::Value onKey(const Napi::CallbackInfo& info);
    

private:
    static std::unordered_map<std::string, std::weak_ptr<native::Context>> _created;
    std::shared_ptr<native::Context> _native;
};

} // namespace nglpmt