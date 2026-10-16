#pragma once
#include "shenmue.h"
#include <array>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name TASK_SLEEP_ACTION = SHEN_NAME(ICSV);
inline constexpr std::array<std::int16_t, 4> SLEEP_ARROW_X{0, -61, 0, 62}, SLEEP_ARROW_Y{-49, 0, 50, 0};
inline constexpr std::array<std::int16_t, 4> SLEEP_TEXT_X{0, -55, 0, 56}, SLEEP_TEXT_Y{-44, 0, 43, 0};
inline constexpr std::array<float, 4> SLEEP_ARROW_DEPTH{1.0f, 2.0f, 1.0f, 3.0f}, SLEEP_TEXT_DEPTH{4.0f, 4.0f, 5.0f, 5.0f};
inline constexpr std::array<float, 4> SLEEP_POSITION_INITIAL{-176.0f, 118.0f, 1.0f, 0.0f};
inline constexpr std::array<std::uint8_t, 4> SLEEP_TEXT_FLAG_BITS{0, 1, 4, 3};
inline constexpr std::array<const char *, 4> SLEEP_TEXT_NAMES{"Sleep", "Save", "Cancel", "Options"};
inline constexpr std::array<std::int8_t, 12> ACTION_SELECTOR_DIRECTION_TO_SLOT{0, 4, 3, 4, 2, 4, 1, 4, 4, 0, 2, 0};
inline constexpr std::array<float, 4> ACTION_SELECTOR_CHOICE_Y{-35.0f, 0.0f, 36.0f, 0.0f};
inline constexpr std::array<float, 4> ACTION_SELECTOR_RADIAL_X{0.0f, -1.0f, 0.0f, 1.0f}, ACTION_SELECTOR_RADIAL_Y{-1.0f, 0.0f, 1.0f, 0.0f};
inline constexpr std::array<std::uint16_t, 6> ACTION_SELECTOR_MARKERS{45, 42, 55, 56, 62, 0};
inline constexpr std::array<std::uint16_t, 4> ACTION_SELECTOR_FRAME_IDS{1284, 1283, 1282, 1281};
inline constexpr std::array<std::int16_t, 4> SLEEP_LABEL_WIDTHS{128, 128, 128, 128};
}
