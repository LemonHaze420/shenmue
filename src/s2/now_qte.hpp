#pragma once
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name TASK_NOW_QTE = SHEN_NAME(BTTN);
inline constexpr shen_name NOW_QTE_TEXTURE_TYPE = SHEN_NAME(CMRA);
inline constexpr shen_name NOW_QTE_TEXTURE_NAME = SHEN_NAME(TXBS);
inline constexpr std::size_t NOW_QTE_PTX_STORAGE_SIZE = 0x8030;
struct HelpDirectionBlink {
    std::uint8_t mode;
    std::array<std::byte, 3> reserved01;
    std::int32_t onFrames, offFrames, remaining, visible;
};
static_assert(sizeof(HelpDirectionBlink) == 0x14);
static_assert(offsetof(HelpDirectionBlink, onFrames) == 4);
static_assert(offsetof(HelpDirectionBlink, offFrames) == 8);
static_assert(offsetof(HelpDirectionBlink, remaining) == 0xc);
static_assert(offsetof(HelpDirectionBlink, visible) == 0x10);
inline constexpr std::array<std::int32_t, 16> HELP_DIRECTION_ON_FRAMES{6, 0, 0, 0, 5, 5, 0, 0, 5, 8, 5, 0, 5, 5, 5, 5};
inline constexpr std::array<std::int32_t, 16> HELP_DIRECTION_PHASES{6, 0, 0, 0, 5, -5, 0, 0, 5, 8, -5, 0, -5, 5, -3, 3};
inline constexpr std::array<std::size_t, 4> HELP_DIRECTION_MISSING_ORDER{1, 3, 0, 2};
inline constexpr std::array<std::size_t, 4> HELP_DIRECTION_RING_ORDER{0, 3, 1, 2};
inline constexpr std::array<std::int16_t, 4> HELP_CARDINAL_ROTATIONS{0, 180, 270, 90};
inline constexpr std::array<std::uint32_t, 4> HELP_BUTTON_MASKS{4, 2, 0x10, 8};
inline constexpr std::array<std::uint32_t, 4> HELP_SELECTED_BUTTON_MASKS{0x400, 0x200, 4, 2};
inline constexpr std::array<std::int16_t, 28> HELP_HINT_ICONS{
    43, 42, 56, 45, 45, 51, 44, 52, 46, 57, 55, 58, 48, 49, 47, 50, 54, 58, 53, 61, 59, 60, 45, 783, 782, 62, 63, 38};
inline constexpr std::array<std::int32_t, 20> HELP_HINT_BLINK_SLOTS{-1, 1, 0, 3, 2, -1, -1, -1, -1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
} // namespace shenmue::s2
