#pragma once

#include <cmath>

namespace shenmue {

// GameJam takes square roots with the SH-4 fsqrt instruction written as inline assembly.
inline float NativeSqrt(float value) {
#if defined(__sh__)
    __asm__("fsqrt %0" : "=f"(value) : "0"(value));
    return value;
#else
    return std::sqrt(value);
#endif
}

} // namespace shenmue
