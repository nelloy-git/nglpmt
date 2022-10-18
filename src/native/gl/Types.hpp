#pragma once

#include "KHR/khrplatform.h"

struct __GLsync;

namespace nglpmt::native {

using Enum = unsigned int;
using Bool = unsigned char;
using BitField = unsigned int;
using Void = void;
using Byte = khronos_int8_t;
using UByte = khronos_uint8_t;
using Short = khronos_int16_t;
using UShort = khronos_uint16_t;
using Int = int;
using Int64 = khronos_int64_t;
using UInt = unsigned int;
using UInt64 = khronos_uint64_t;
using Sizei = int;
using Float = khronos_float_t;
using Double = double;
using Char = char;

#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
using IntPtr = khronos_intptr_t;
#else
using IntPtr = khronos_intptr_t;
#endif

#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
using SizeiPtr = khronos_ssize_t;
#else
using SizeiPtr = khronos_ssize_t;
#endif

using GLint64 = khronos_int64_t;
using GLuint64 = khronos_uint64_t;

typedef struct __GLsync *GLsync;
using GLdebugProc = void (*)(Enum source, Enum type, UInt id, Enum severity, Sizei length, const Char *message, const void *userParam);

using ApiProc = void (*)(void);
using LoadFunc = ApiProc (*)(const char *name);

}