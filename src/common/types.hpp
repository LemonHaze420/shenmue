#pragma once

#include "shenmue.h"

#include <cstdint>

namespace shenmue {


union shen_name {
    std::uint32_t name;
    char characters[4];

    constexpr shen_name() noexcept : name(0) {
    }
    constexpr shen_name(std::uint32_t value) noexcept : name(value) {
    }
};

#define SHENMUE_NAME_CONSTEXPR constexpr
#define SHENMUE_NAME_NOEXCEPT noexcept

SHENMUE_NAME_CONSTEXPR bool operator==(shen_name left, shen_name right) SHENMUE_NAME_NOEXCEPT {
    return left.name == right.name;
}


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
    return shen_name(make_shen_name(a, b, c, d));
}

inline constexpr shen_name HLIB_USED_TOKEN{make_shen_name("USE ")};
inline constexpr shen_name HLIB_FREE_TOKEN{make_shen_name("fREe")};
inline constexpr shen_name HLIB_FONT_TOKEN{make_shen_name("FONT")};

static_assert(sizeof(shen_name) == 4);

#undef SHENMUE_NAME_CONSTEXPR
#undef SHENMUE_NAME_NOEXCEPT

} // namespace shenmue

// A fixed FourCC spelled as its token: `inline constexpr shen_name TASK_FMOT{SHEN_NAME(FMOT)};`.
#define SHEN_NAME(x) ::shenmue::make_shen_name(#x)

#define SHEN_NAME4(a, b, c, d) (static_cast<std::uint32_t>(static_cast<std::uint8_t>(a)) | \
    (static_cast<std::uint32_t>(static_cast<std::uint8_t>(b)) << 8U) | \
    (static_cast<std::uint32_t>(static_cast<std::uint8_t>(c)) << 16U) | \
    (static_cast<std::uint32_t>(static_cast<std::uint8_t>(d)) << 24U))
