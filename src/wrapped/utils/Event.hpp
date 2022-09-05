#pragma once

#include "napi.h"
#include "wrapped/utils/MapMT.hpp"
#include "native/utils/Event.hpp"

namespace nglpmt {

class Event : public Napi::ObjectWrap<Event> {

struct Parameter;

public:
    static void Init(Napi::Env env, Napi::Object exports);



    Event(const Napi::CallbackInfo& info);
    virtual ~Event();

    Napi::Value emit(const Napi::CallbackInfo& info);
    Napi::Value push(const Napi::CallbackInfo& info);

private:
    using NativeEvent = native::Event<const std::shared_ptr<std::vector<Parameter>>>;
    static MapMT<std::u16string, NativeEvent>& _getMap(); 
    static void _jsCallback(Napi::Env env,
                            Napi::Function jsCallback,
                            std::shared_ptr<std::vector<Parameter>>* ptr);


    std::shared_ptr<NativeEvent> _native;

    void _push_once(const Napi::ThreadSafeFunction& tsfn);
};

} // namespace nglpmt