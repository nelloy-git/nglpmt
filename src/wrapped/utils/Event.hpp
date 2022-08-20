#pragma once

#include "napi.h"
#include "native/utils/Event.hpp"

namespace nglpmt {

template<typename ... Args>
class Event : public Napi::ObjectWrap<Event> {
public:
    static void Init(Napi::Env env, Napi::Object exports);

    Event(const Napi::CallbackInfo& info){};
    Event(const std::shared_ptr<native::Event<Args...>>);
    ~Event();

    void push(const Napi::CallbackInfo& info){

    }

private:
    std::shared_ptr<native::Event<Args...>> _native;

};

}