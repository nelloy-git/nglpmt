#ifdef WIN32
// #include <vld.h>
#endif

#include "napi.h"

#include "wrapped/Context.hpp"
#include "wrapped/utils/Event.hpp"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    nglpmt::Context::Init(env, exports);
    nglpmt::Event::Init(env, exports);

    return exports;
}

NODE_API_MODULE(nglpmt, InitAll)