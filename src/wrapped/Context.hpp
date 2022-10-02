#pragma once

#include <unordered_map>
#include <memory>

#include "napi.h"

#include "native/Context.hpp"
#include "wrapped/utils/MapMT.hpp"
#include "wrapped/utils/NativeEvent.hpp"

namespace nglpmt::js {

class Context : public Napi::ObjectWrap<Context> {
public:
    static Napi::Function createJsConstructor(Napi::Env env);

    Context(const Napi::CallbackInfo& info);
    ~Context();

    Napi::Value release(const Napi::CallbackInfo& info);
    Napi::Value start(const Napi::CallbackInfo& info);

    Napi::Value onRun(const Napi::CallbackInfo& info);
    Napi::Value onKey(const Napi::CallbackInfo& info);
    

private:
    std::string _name;
    std::shared_ptr<native::Context> _native;
    std::shared_ptr<NativeEvent<std::weak_ptr<native::Context>, const std::chrono::milliseconds&>> _on_run;
    
    static MapMT<std::u16string, native::Context>& _getMap(); 

};

} // namespace nglpmt