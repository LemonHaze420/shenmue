#pragma once

#include "shenmue.h"
#include "common/types.hpp"
#if SHENMUE_S2_GCC295
#define SHENMUE_MONEY_DATA static const
#define SHENMUE_MONEY_ARRAY(type, name, count) type name[count] =
#else
#include <array>
#include <cstddef>
#include <cstdint>
#define SHENMUE_MONEY_DATA inline constexpr
#define SHENMUE_MONEY_ARRAY(type, name, count) std::array<type, count> name
#endif

namespace shenmue {
namespace s2 {
#if SHENMUE_S2_GCC295
using ::shenmue::shen_name;
#define TASK_ICMO shen_name(SHEN_NAME4('I', 'C', 'M', 'o'))
#else
inline constexpr shen_name TASK_ICMO = SHEN_NAME(ICMo);
#endif
SHENMUE_MONEY_DATA SHENMUE_MONEY_ARRAY(std::int32_t, moneyIconIncrements, 8) { 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1 };
SHENMUE_MONEY_DATA SHENMUE_MONEY_ARRAY(std::int32_t, MONEY_TEXT_OPTION_FLAGS, 2) { 2, 8 };
#if SHENMUE_S2_GCC295
static const char *const MONEY_TEXT_OPTION_NAMES[2] = {"Cancel", "MAX"};
#else
inline constexpr std::array<const char *, 2> MONEY_TEXT_OPTION_NAMES{"Cancel", "MAX"};
#endif
SHENMUE_MONEY_DATA SHENMUE_MONEY_ARRAY(std::int32_t, MONEY_TEXT_RENDER_FLAGS, 2) { 2, 1 };
SHENMUE_MONEY_DATA SHENMUE_MONEY_ARRAY(std::int32_t, MONEY_ANIMATION_MODES, 2) { 3, 0 };
#if SHENMUE_S2_GCC295
SHENMUE_MONEY_DATA SHENMUE_MONEY_ARRAY(std::int32_t, MONEY_DIRECTION_X, 4) { 64, -64, 0, 0 };
SHENMUE_MONEY_DATA SHENMUE_MONEY_ARRAY(std::int32_t, MONEY_DIRECTION_Y, 4) { 0, 0, 36, -36 };
#else
SHENMUE_MONEY_DATA SHENMUE_MONEY_ARRAY(float, MONEY_DIRECTION_X, 4) { 64.0f, -64.0f, 0.0f, 0.0f };
SHENMUE_MONEY_DATA SHENMUE_MONEY_ARRAY(float, MONEY_DIRECTION_Y, 4) { 0.0f, 0.0f, 36.0f, -36.0f };
#endif
struct MoneyIconPosition {
    float x, y, velocity, reservedScale;
    float depth;
    std::uint16_t reserved14, selectedDirection;
};
#if SHENMUE_S2_GCC295
typedef char MoneyIconPositionSizeCheck[sizeof(MoneyIconPosition) == 0x18 ? 1 : -1];
typedef char MoneyIconVelocityOffsetCheck[offsetof(MoneyIconPosition, velocity) == 8 ? 1 : -1];
typedef char MoneyIconDepthOffsetCheck[offsetof(MoneyIconPosition, depth) == 0x10 ? 1 : -1];
typedef char MoneyIconDirectionOffsetCheck[offsetof(MoneyIconPosition, selectedDirection) == 0x16 ? 1 : -1];
#else
static_assert(sizeof(MoneyIconPosition) == 0x18);
static_assert(offsetof(MoneyIconPosition, velocity) == 8);
static_assert(offsetof(MoneyIconPosition, depth) == 0x10);
static_assert(offsetof(MoneyIconPosition, selectedDirection) == 0x16);
#endif
#if SHENMUE_S2_GCC295
static const MoneyIconPosition MONEY_POSITION_INITIAL = {SHEN_FIELD(x) - 516.0f, SHEN_FIELD(y) 340.0f, SHEN_FIELD(velocity) 1.0f,
    SHEN_FIELD(reservedScale) 0.0f, SHEN_FIELD(depth) 3.0f, SHEN_FIELD(reserved14) 0, SHEN_FIELD(selectedDirection) 4};
static const MoneyIconPosition MONEY_SELECTOR_POSITION_INITIAL = {SHEN_FIELD(x) - 176.0f, SHEN_FIELD(y) 91.0f, SHEN_FIELD(velocity) 1.0f,
    SHEN_FIELD(reservedScale) 0.0f, SHEN_FIELD(depth) 0.0f, SHEN_FIELD(reserved14) 0, SHEN_FIELD(selectedDirection) 4};
#else
inline constexpr MoneyIconPosition MONEY_POSITION_INITIAL{.x = -516.0f, .y = 340.0f, .velocity = 1.0f, .depth = 3.0f, .selectedDirection = 4};
inline constexpr MoneyIconPosition MONEY_SELECTOR_POSITION_INITIAL{.x = -176.0f, .y = 91.0f, .velocity = 1.0f, .selectedDirection = 4};
#endif
} // namespace s2
} // namespace shenmue
#undef SHENMUE_MONEY_DATA
#undef SHENMUE_MONEY_ARRAY
