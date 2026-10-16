#pragma once

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

constexpr bool operator==(shen_name left, shen_name right) noexcept {
    return left.name == right.name;
}

constexpr std::uint32_t make_shen_name(char a, char b, char c, char d) noexcept {
    return static_cast<std::uint8_t>(a) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(b)) << 8U) |
        (static_cast<std::uint32_t>(static_cast<std::uint8_t>(c)) << 16U) | (static_cast<std::uint32_t>(static_cast<std::uint8_t>(d)) << 24U);
}

constexpr std::uint32_t make_shen_name(const char (&text)[5]) noexcept {
    return make_shen_name(text[0], text[1], text[2], text[3]);
}

constexpr shen_name make_name(char a, char b, char c, char d) noexcept {
    return shen_name{make_shen_name(a, b, c, d)};
}

inline constexpr shen_name HLIB_USED_TOKEN{make_shen_name("USE ")};
inline constexpr shen_name HLIB_FREE_TOKEN{make_shen_name("fREe")};
inline constexpr shen_name HLIB_FONT_TOKEN{make_shen_name("FONT")};

static_assert(sizeof(shen_name) == 4);

} // namespace shenmue

// A fixed FourCC spelled as its token: `inline constexpr shen_name TASK_FMOT{SHEN_NAME(FMOT)};`.
#define SHEN_NAME(x) ::shenmue::make_shen_name(#x)
