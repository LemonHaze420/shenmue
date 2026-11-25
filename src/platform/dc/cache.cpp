#include "shenmue.h"

#if SHENMUE_IS_DC

#include <cstdint>

extern "C" void syCacheInit_P2_(std::uint32_t control);

extern "C" void shenmue_dc_configure_cpu_cache(std::uint32_t control) {
    typedef void (*CacheInitializer)(std::uint32_t);
    const CacheInitializer initialize = reinterpret_cast<CacheInitializer>(reinterpret_cast<std::uintptr_t>(&syCacheInit_P2_) | 0xa0000000U);
    initialize(control);
}

#endif
