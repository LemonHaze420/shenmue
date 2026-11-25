#pragma once
#include "shenmue.h"
#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#else
#include <array>
#include <cstddef>
#include <cstdint>
#endif

namespace shenmue {
namespace s2 {
struct BorrowedLegacySpriteSlot {
    const std::uint16_t *rectangles;
    std::uint16_t slot;
    std::int16_t texture;
    float inverseWidth;
    float inverseHeight;
#if !SHENMUE_IS_DC
    std::uint32_t reserved14;
#endif
};
#if SHENMUE_S2_GCC295
typedef char BorrowedLegacySpriteSlotSizeCheck[sizeof(BorrowedLegacySpriteSlot) == 0x10 ? 1 : -1];
extern BorrowedLegacySpriteSlot borrowedSpriteSlots[8];
#else
static_assert(sizeof(BorrowedLegacySpriteSlot) == (SHENMUE_IS_DC ? 0x10 : 0x18));
extern std::array<BorrowedLegacySpriteSlot, 8> borrowedSpriteSlots;
#endif
#if SHENMUE_S2_GCC295
bool RegisterBorrowedLegacySpriteSlot(std::int32_t texture, std::int32_t width, std::int32_t height, std::int32_t slot, const std::uint16_t *rectangles);
#else
bool RegisterBorrowedLegacySpriteSlot(std::int16_t texture, std::int32_t width, std::int32_t height, std::uint16_t slot, const std::uint16_t *rectangles);
#endif
void ReleaseBorrowedLegacySpriteSlot(std::uint16_t slot);
}
}
