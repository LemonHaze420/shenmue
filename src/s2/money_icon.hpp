#pragma once

#include "shenmue.h"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name TASK_ICMO = SHEN_NAME(ICMo);
inline constexpr std::array<std::int32_t, 8> moneyIconIncrements{10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
inline constexpr std::array<std::int32_t, 2> MONEY_TEXT_OPTION_FLAGS{2, 8};
inline constexpr std::array<const char *, 2> MONEY_TEXT_OPTION_NAMES{"Cancel", "MAX"};
inline constexpr std::array<std::int32_t, 2> MONEY_TEXT_RENDER_FLAGS{2, 1};
inline constexpr std::array<std::int32_t, 2> MONEY_ANIMATION_MODES{3, 0};
inline constexpr std::array<float, 4> MONEY_DIRECTION_X{64.0f, -64.0f, 0.0f, 0.0f};
inline constexpr std::array<float, 4> MONEY_DIRECTION_Y{0.0f, 0.0f, 36.0f, -36.0f};
struct MoneyIconPosition {
    float x, y, velocity, reservedScale;
    float depth;
    std::uint16_t reserved14, selectedDirection;
};
static_assert(sizeof(MoneyIconPosition) == 0x18);
static_assert(offsetof(MoneyIconPosition, velocity) == 8);
static_assert(offsetof(MoneyIconPosition, depth) == 0x10);
static_assert(offsetof(MoneyIconPosition, selectedDirection) == 0x16);
inline constexpr MoneyIconPosition MONEY_POSITION_INITIAL{.x = -516.0f, .y = 340.0f, .velocity = 1.0f, .depth = 3.0f, .selectedDirection = 4};
inline constexpr MoneyIconPosition MONEY_SELECTOR_POSITION_INITIAL{.x = -176.0f, .y = 91.0f, .velocity = 1.0f, .selectedDirection = 4};
} // namespace shenmue::s2
