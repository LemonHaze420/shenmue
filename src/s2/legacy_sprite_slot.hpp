#pragma once
#include "shenmue.h"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue { namespace s2 {
struct BorrowedLegacySpriteSlot {
    const std::uint16_t *rectangles;
    std::uint16_t slot;
    std::int16_t texture;
    float inverseWidth;
    float inverseHeight;
    std::uint32_t reserved14;
};
static_assert(sizeof(BorrowedLegacySpriteSlot) == (SHENMUE_IS_DC ? 0x10 : 0x18));
extern std::array<BorrowedLegacySpriteSlot, 8> borrowedSpriteSlots;
bool RegisterBorrowedLegacySpriteSlot(std::int16_t texture, std::int32_t width, std::int32_t height, std::uint16_t slot, const std::uint16_t *rectangles);
void ReleaseBorrowedLegacySpriteSlot(std::uint16_t slot);
}
}
