#pragma once

#include "shenmue.h"
#include "common/types.hpp"
#if SHENMUE_S2_GCC295
#define ITEM_DATA const
#define ITEM_LAYOUT_NAME_(line) ItemEffectLayoutCheck##line
#define ITEM_LAYOUT_NAME(line) ITEM_LAYOUT_NAME_(line)
#define ITEM_LAYOUT(condition) typedef char ITEM_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#else
#include <cstddef>
#include <cstdint>
#define ITEM_DATA inline constexpr
#define ITEM_LAYOUT(condition) static_assert(condition)
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_S2_GCC295
#define TASK_IEET shen_name(SHEN_NAME4('I', 'E', 'E', 'T'))
#else
inline constexpr shen_name TASK_IEET = SHEN_NAME(IEET);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
ITEM_DATA char ITEM_EFFECT_MISSING_NAME[] = "\xa5\xa8\xa5\xe9\xa1\xbc";
#else
inline constexpr char ITEM_EFFECT_MISSING_NAME[] = "\xe3\x82\xa8\xe3\x83\xa9\xe3\x83\xbc";
#endif
struct ItemEffectPosition {
    float x, y, velocity, reservedScale;
    float depth;
    std::uint16_t reserved14, selectedDirection;
};
ITEM_LAYOUT(sizeof(ItemEffectPosition) == 0x18);
ITEM_LAYOUT(offsetof(ItemEffectPosition, velocity) == 8);
ITEM_LAYOUT(offsetof(ItemEffectPosition, depth) == 0x10);
ITEM_LAYOUT(offsetof(ItemEffectPosition, selectedDirection) == 0x16);
#if SHENMUE_S2_GCC295
const ItemEffectPosition ITEM_EFFECT_POSITION_INITIAL = {SHEN_FIELD(x) - 672.0f, SHEN_FIELD(y) 55.0f, SHEN_FIELD(velocity) 1.0f, SHEN_FIELD(reservedScale) 0.0f,
    SHEN_FIELD(depth) 3.0f, SHEN_FIELD(reserved14) 0, SHEN_FIELD(selectedDirection) 4};
#else
inline constexpr ItemEffectPosition ITEM_EFFECT_POSITION_INITIAL{.x = -672.0f, .y = 55.0f, .velocity = 1.0f, .depth = 3.0f, .selectedDirection = 4};
#endif
} // namespace s2
} // namespace shenmue
#if SHENMUE_S2_GCC295
#undef ITEM_LAYOUT_NAME
#undef ITEM_LAYOUT_NAME_
#endif
#undef ITEM_DATA
#undef ITEM_LAYOUT
