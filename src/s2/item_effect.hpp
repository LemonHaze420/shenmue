#pragma once

#include "shenmue.h"
#include "common/types.hpp"
#include <cstddef>
#include <cstdint>
#define ITEM_DATA inline constexpr
#define ITEM_LAYOUT(condition) static_assert(condition)
namespace shenmue::s2 {
inline constexpr shen_name TASK_IEET = SHEN_NAME(IEET);
inline constexpr char ITEM_EFFECT_MISSING_NAME[] = "\xe3\x82\xa8\xe3\x83\xa9\xe3\x83\xbc";
struct ItemEffectPosition {
    float x, y, velocity, reservedScale;
    float depth;
    std::uint16_t reserved14, selectedDirection;
};
ITEM_LAYOUT(sizeof(ItemEffectPosition) == 0x18);
ITEM_LAYOUT(offsetof(ItemEffectPosition, velocity) == 8);
ITEM_LAYOUT(offsetof(ItemEffectPosition, depth) == 0x10);
ITEM_LAYOUT(offsetof(ItemEffectPosition, selectedDirection) == 0x16);
inline constexpr ItemEffectPosition ITEM_EFFECT_POSITION_INITIAL{.x = -672.0f, .y = 55.0f, .velocity = 1.0f, .depth = 3.0f, .selectedDirection = 4};
} // namespace shenmue::s2
#undef ITEM_DATA
#undef ITEM_LAYOUT
