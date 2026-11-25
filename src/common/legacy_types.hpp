#pragma once

#include "shenmue.h"

#if SHENMUE_S2_GCC295
#include <stddef.h>

namespace std {
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef int intptr_t;
typedef unsigned int uintptr_t;
typedef ::size_t size_t;
typedef ::ptrdiff_t ptrdiff_t;
typedef unsigned char byte;
typedef char FixedIntegerWidthCheck[sizeof(int8_t) == 1 && sizeof(int16_t) == 2 && sizeof(int32_t) == 4 && sizeof(int64_t) == 8 ? 1 : -1];
typedef char PointerIntegerWidthCheck[sizeof(intptr_t) == sizeof(void *) && sizeof(uintptr_t) == sizeof(void *) ? 1 : -1];
} // namespace std

#ifndef INT8_MIN
#define INT8_MIN (-127 - 1)
#endif
#ifndef INT8_MAX
#define INT8_MAX 127
#endif
#ifndef UINT8_MAX
#define UINT8_MAX 255
#endif
#ifndef INT16_MIN
#define INT16_MIN (-32767 - 1)
#endif
#ifndef INT16_MAX
#define INT16_MAX 32767
#endif
#ifndef UINT16_MAX
#define UINT16_MAX 65535
#endif
#ifndef INT32_MIN
#define INT32_MIN (-2147483647L - 1)
#endif
#ifndef INT32_MAX
#define INT32_MAX 2147483647L
#endif
#ifndef UINT32_MAX
#define UINT32_MAX 0xffffffffUL
#endif
#ifndef INT64_MIN
#define INT64_MIN (-9223372036854775807LL - 1)
#endif
#ifndef INT64_MAX
#define INT64_MAX 9223372036854775807LL
#endif
#ifndef UINT64_MAX
#define UINT64_MAX 0xffffffffffffffffULL
#endif
#ifndef INTPTR_MIN
#define INTPTR_MIN (-2147483647 - 1)
#endif
#ifndef INTPTR_MAX
#define INTPTR_MAX 2147483647
#endif
#ifndef UINTPTR_MAX
#define UINTPTR_MAX 0xffffffffU
#endif
#ifndef PTRDIFF_MIN
#define PTRDIFF_MIN (-2147483647 - 1)
#endif
#ifndef PTRDIFF_MAX
#define PTRDIFF_MAX 2147483647
#endif
#ifndef SIZE_MAX
#define SIZE_MAX 0xffffffffU
#endif
#endif
