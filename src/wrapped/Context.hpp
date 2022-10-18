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
    static Napi::Function getJsConstructor(Napi::Env env);

    Context(const Napi::CallbackInfo& info);
    ~Context();

    std::shared_ptr<native::Context> getNative();

    Napi::Value release(const Napi::CallbackInfo& info);
    Napi::Value run(const Napi::CallbackInfo& info);

    Napi::Value onRun(const Napi::CallbackInfo& info);

    Napi::Value onKeyAdd(const Napi::CallbackInfo& info);
    Napi::Value onKeyRemove(const Napi::CallbackInfo& info);
    Napi::Value onKeyClear(const Napi::CallbackInfo& info);
    

private:
    std::string _name;
    std::shared_ptr<native::Context> _native;
    // EventWrapped<std::weak_ptr<native::Context>, const std::chrono::milliseconds&> _on_run;
    EventWrapped<const std::shared_ptr<native::Context>, const int, const int, const int, const int> _on_key;
    
    static MapMT<std::u16string, native::Context>& _getMap(); 

};

} // namespace nglpmt