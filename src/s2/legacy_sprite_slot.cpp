#include "s2/legacy_sprite_slot.hpp"

namespace shenmue { namespace s2 {
std::array<BorrowedLegacySpriteSlot, 8> borrowedSpriteSlots{};

// @verified - pd
bool RegisterBorrowedLegacySpriteSlot(std::int16_t texture, std::int32_t width, std::int32_t height, std::uint16_t slot, const std::uint16_t *rectangles) {
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

// @verified - pd
void ReleaseBorrowedLegacySpriteSlot(std::uint16_t slot) {
    for (std::int32_t index = 0; index < 8; ++index) {
        BorrowedLegacySpriteSlot &entry = borrowedSpriteSlots[index];
        if (!entry.rectangles || entry.slot != slot)
            continue;
        entry.rectangles = 0;
        entry.slot = 0;
        entry.texture = -1;
        return;
    }
}

}
}
