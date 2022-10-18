#ifdef WIN32
// #include <vld.h>
#endif

#include "napi.h"

#include "wrapped/Context.hpp"

#include "wrapped/gl/Buffer.hpp"

#include "wrapped/utils/Event.hpp"

using namespace nglpmt::js;

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports.Set("Context", Context::getJsConstructor(env));
    exports.Set("Event", Event::getJsConstructor(env));
    exports.Set("GlBuffer", GlBuffer::getJsConstructor(env));

    return exports;
}

NODE_API_MODULE(nglpmt, InitAll)