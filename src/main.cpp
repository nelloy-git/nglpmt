#ifdef WIN32
// #include <vld.h>
#endif

#include "napi.h"

#include "wrapped/Context.hpp"
#include "wrapped/utils/Event.hpp"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports.Set("Context", nglpmt::js::Context::createJsConstructor(env));
    exports.Set("Event", nglpmt::js::Event::createJsConstructor(env));

    return exports;
}

NODE_API_MODULE(nglpmt, InitAll)