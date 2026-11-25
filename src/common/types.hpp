#pragma once

#include "shenmue.h"

#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#else
#include <cstdint>
#endif

namespace shenmue {

#if SHENMUE_IS_DC
inline float u32AsFloat(std::uint32_t bits) {
    union {
        std::uint32_t bits;
        float value;
    } word;
    word.bits = bits;
    return word.value;
}
inline std::uint32_t floatAsU32(float value) {
    union {
        std::uint32_t bits;
        float value;
    } word;
    word.value = value;
    return word.bits;
}
#endif

#if SHENMUE_S2_GCC295
typedef std::uint32_t shen_name;
#else
union shen_name {
    std::uint32_t name;
    char characters[4];

    constexpr shen_name() noexcept : name(0) {
    }
    constexpr shen_name(std::uint32_t value) noexcept : name(value) {
    }

    constexpr explicit operator std::uint32_t() const noexcept {
        return name;
    }
};
#endif

#if SHENMUE_S2_GCC295
#define SHENMUE_NAME_CONSTEXPR inline
#define SHENMUE_NAME_NOEXCEPT
#else
#define SHENMUE_NAME_CONSTEXPR constexpr
#define SHENMUE_NAME_NOEXCEPT noexcept
#endif

#if !SHENMUE_S2_GCC295
SHENMUE_NAME_CONSTEXPR bool operator==(shen_name left, shen_name right) SHENMUE_NAME_NOEXCEPT {
    return left.name == right.name;
}
#endif

SHENMUE_NAME_CONSTEXPR std::uint32_t make_shen_name(char a, char b, char c, char d) SHENMUE_NAME_NOEXCEPT {
    return static_cast<std::uint8_t>(a) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(b)) << 8U) |
        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(c)) << 16U) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(d)) << 24U);
}

SHENMUE_NAME_CONSTEXPR std::uint32_t make_shen_name(const char (&text)[5]) SHENMUE_NAME_NOEXCEPT {
    return make_shen_name(text[0], text[1], text[2], text[3]);
}

SHENMUE_NAME_CONSTEXPR std::uint32_t make_shen_name(const char (&text)[4]) SHENMUE_NAME_NOEXCEPT {
    return make_shen_name(text[0], text[1], text[2], '\0');
}

SHENMUE_NAME_CONSTEXPR shen_name make_name(char a, char b, char c, char d) SHENMUE_NAME_NOEXCEPT {
#if SHENMUE_S2_GCC295
    return make_shen_name(a, b, c, d);
#else
    return shen_name(make_shen_name(a, b, c, d));
#endif
}

#if SHENMUE_S2_GCC295
inline shen_name HLibUsedToken() {
    return shen_name(make_shen_name('U', 'S', 'E', ' '));
}
inline shen_name HLibFreeToken() {
    return shen_name(make_shen_name('f', 'R', 'E', 'e'));
}
inline shen_name HLibFontToken() {
    return shen_name(make_shen_name('F', 'O', 'N', 'T'));
}
#define HLIB_USED_TOKEN HLibUsedToken()
#define HLIB_FREE_TOKEN HLibFreeToken()
#define HLIB_FONT_TOKEN HLibFontToken()
typedef char ShenNameSizeCheck[sizeof(shen_name) == 4 ? 1 : -1];
#else
inline constexpr shen_name HLIB_USED_TOKEN{make_shen_name("USE ")};
inline constexpr shen_name HLIB_FREE_TOKEN{make_shen_name("fREe")};
inline constexpr shen_name HLIB_FONT_TOKEN{make_shen_name("FONT")};

static_assert(sizeof(shen_name) == 4);
#endif

#undef SHENMUE_NAME_CONSTEXPR
#undef SHENMUE_NAME_NOEXCEPT

} // namespace shenmue

// A fixed FourCC spelled as its token: `inline constexpr shen_name TASK_FMOT{SHEN_NAME(FMOT)};`.
#if SHENMUE_S2_GCC295
#define SHEN_NAME(x) SHEN_NAME4(#x[0], #x[1], #x[2], #x[3])
#else
#define SHEN_NAME(x) ::shenmue::make_shen_name(#x)
#endif

#define SHEN_NAME4(a, b, c, d)                                                                                                                                 \
    (static_cast<std::uint32_t>(static_cast<std::uint8_t>(a)) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(b)) << 8U) |                             \
        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(c)) << 16U) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(d)) << 24U))
