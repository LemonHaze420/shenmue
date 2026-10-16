#pragma once
#include "shenmue.h"
#include "common/types.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name TASK_CTRL_SPEAK = SHEN_NAME(ICSp);
inline constexpr shen_name CTRL_SPEAK_TEXTURE_TYPE = SHEN_NAME(ASAS);
inline constexpr shen_name BUTTON_CTRL_SPEAK_CANCEL = SHEN_NAME(BTMM);
inline constexpr std::array<float, 4> CTRL_SPEAK_ORIGIN_INITIAL{-176.0f, 91.0f, 1.0f, 0.0f};
inline constexpr std::array<std::array<std::int32_t, 2>, 3> CTRL_SPEAK_SPECIAL_LABELS{{{347, 39}, {348, 38}, {349, 40}}};
inline constexpr std::array<const char *, 2> CTRL_SPEAK_ALTERNATE_LABELS{"YES", "NO"};
} // namespace shenmue::s2
