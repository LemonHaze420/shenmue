#include "shenmue.h"

#if SHENMUE_IS_DC

#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#include <errno.h>
#else
#include <cstdint>
#include <cstddef>
#include <cerrno>
#endif
#include <reent.h>

#if SHENMUE_S2_GCC295
typedef char NativeRuntimeTypeSizes[(sizeof(void *) == 4 && sizeof(float) == 4 && sizeof(double) == 4 && sizeof(long double) == 8) ? 1 : -1];
#else
static_assert(sizeof(void *) == 4 && sizeof(float) == 4 && sizeof(double) == 8);
#endif

namespace {
#if SHENMUE_S2_GCC295
typedef void (*Initializer)();
#else
using Initializer = void (*)();
#endif
extern "C" Initializer g_preinitBegin[] asm("__preinit_array_start");
extern "C" Initializer g_preinitEnd[] asm("__preinit_array_end");
extern "C" Initializer g_initBegin[] asm("__init_array_start");
extern "C" Initializer g_initEnd[] asm("__init_array_end");
extern "C" Initializer g_ctorsBegin[] asm("__dc_ctors_begin");
extern "C" Initializer g_ctorsEnd[] asm("__dc_ctors_end");
extern "C" std::byte g_crtHeapBegin[] asm("__dc_crt_heap_begin");
extern "C" std::byte g_crtHeapEnd[] asm("__dc_crt_heap_end");
extern "C" void syMallocInit(void *memory, std::uint32_t bytes);
extern "C" void *syMalloc(std::uint32_t bytes);
extern "C" void *syCalloc(std::uint32_t count, std::uint32_t bytes);
extern "C" void *syRealloc(void *pointer, std::uint32_t bytes);
extern "C" void syFree(void *pointer);
extern "C" void syHwInit();
#if SHENMUE_IS_S2
extern "C" void syIntEnableInt(std::uint32_t mask);
extern "C" int _isatty(int descriptor);
#endif
}

#if SHENMUE_IS_S2
extern "C" int isatty(int descriptor) {
    return _isatty(descriptor);
}
#endif

extern "C" void *_malloc_r(_reent *state, std::size_t bytes) {
    void *const pointer = syMalloc(bytes);
    if (!pointer)
        state->_errno = ENOMEM;
    return pointer;
}

extern "C" void *_calloc_r(_reent *state, std::size_t count, std::size_t bytes) {
    void *const pointer = syCalloc(count, bytes);
    if (!pointer)
        state->_errno = ENOMEM;
    return pointer;
}

extern "C" void *_realloc_r(_reent *state, void *pointer, std::size_t bytes) {
    void *const result = syRealloc(pointer, bytes);
    if (!result && bytes)
        state->_errno = ENOMEM;
    return result;
}

extern "C" void _free_r(_reent *, void *pointer) {
    syFree(pointer);
}

extern "C" void shenmue_dc_runtime_initialize() {
    syHwInit();
    syMallocInit(g_crtHeapBegin, static_cast<std::uint32_t>(g_crtHeapEnd - g_crtHeapBegin));
    for (Initializer *entry = g_preinitBegin; entry != g_preinitEnd; ++entry)
        (*entry)();
    for (Initializer *entry = g_initBegin; entry != g_initEnd; ++entry)
        (*entry)();
    for (Initializer *entry = g_ctorsEnd; entry != g_ctorsBegin;) {
        Initializer const initializer = *--entry;
        if (initializer && reinterpret_cast<std::uintptr_t>(initializer) !=
#if SHENMUE_S2_GCC295
                static_cast<std::uintptr_t>(-1))
#else
                UINT32_MAX)
#endif
            initializer();
    }
#if SHENMUE_IS_S2
    syIntEnableInt(0);
#endif
}

#endif
