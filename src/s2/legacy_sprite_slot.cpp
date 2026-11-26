#include "s2/legacy_sprite_slot.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SPRITE_NULL 0
namespace shenmue {
namespace s2 {
BorrowedLegacySpriteSlot borrowedSpriteSlots[8];
#else
#include <array>
#define SPRITE_NULL nullptr
namespace shenmue {
namespace s2 {
std::array<BorrowedLegacySpriteSlot, 8> borrowedSpriteSlots{};
#endif
// Native sprRegisterSlot borrows the caller's surface and UV table.
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t FindBorrowedSpriteSlot(std::int32_t slot);

bool RegisterBorrowedLegacySpriteSlot(std::int32_t texture, std::int32_t width, std::int32_t height, std::int32_t slot, const std::uint16_t *rectangles) {
    std::int32_t index = FindBorrowedSpriteSlot(slot);
    if (index < 0) {
        for (std::int32_t candidate = 0; candidate < 8; ++candidate) {
            if (!borrowedSpriteSlots[candidate].rectangles) {
                index = candidate;
                break;
            }
        }
    }
    if (index >= 0) {
        borrowedSpriteSlots[index].rectangles = rectangles;
        borrowedSpriteSlots[index].slot = slot;
        borrowedSpriteSlots[index].texture = texture;
        borrowedSpriteSlots[index].inverseWidth = 1.0f / static_cast<float>(width);
        borrowedSpriteSlots[index].inverseHeight = 1.0f / static_cast<float>(height);
        return true;
    }
    return false;
}
#else
// @verified - p
#if SHENMUE_S2_GCC295
bool RegisterBorrowedLegacySpriteSlot(std::int32_t texture, std::int32_t width, std::int32_t height, std::int32_t slot, const std::uint16_t *rectangles) {
#else
bool RegisterBorrowedLegacySpriteSlot(std::int16_t texture, std::int32_t width, std::int32_t height, std::uint16_t slot, const std::uint16_t *rectangles) {
#endif
    BorrowedLegacySpriteSlot *entry = 0;
    for (std::int32_t index = 0; index < 8; ++index) {
        BorrowedLegacySpriteSlot &candidate = borrowedSpriteSlots[index];
        if (candidate.rectangles && candidate.slot == slot) {
            entry = &candidate;
            break;
        }
    }
    if (!entry) {
        for (std::int32_t index = 0; index < 8; ++index) {
            BorrowedLegacySpriteSlot &candidate = borrowedSpriteSlots[index];
            if (!candidate.rectangles) {
                entry = &candidate;
                break;
            }
        }
    }
    if (!entry)
        return false;
    entry->rectangles = rectangles;
    entry->slot = slot;
    entry->texture = texture;
    entry->inverseWidth = 1.0f / static_cast<float>(width);
    entry->inverseHeight = 1.0f / static_cast<float>(height);
    return true;
}
#endif

// @verified - pd
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ReleaseBorrowedLegacySpriteSlot(std::int32_t slot)
#else
void ReleaseBorrowedLegacySpriteSlot(std::uint16_t slot)
#endif
{
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    BorrowedLegacySpriteSlot *entry;
    for (std::int32_t index = 0; index < 8; ++index) {
        entry = &borrowedSpriteSlots[index];
        if (entry->rectangles && entry->slot == slot) {
            entry->rectangles = SPRITE_NULL;
            entry->slot = 0;
            entry->texture = -1;
            break;
        }
    }
#else
    for (std::int32_t index = 0; index < 8; ++index) {
        BorrowedLegacySpriteSlot &entry = borrowedSpriteSlots[index];
        if (!entry.rectangles || entry.slot != slot)
            continue;
        entry.rectangles = 0;
        entry.slot = 0;
        entry.texture = -1;
        return;
    }
#endif
}
}
}

#undef SPRITE_NULL
