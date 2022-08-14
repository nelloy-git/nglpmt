#ifdef WIN32
#include <vld.h>
#endif

#include "napi.h"

#include "wrapped/Context.hpp"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    nglpmt::Context::Init(env, exports);

    return exports;
}

NODE_API_MODULE(nglpmt, InitAll)