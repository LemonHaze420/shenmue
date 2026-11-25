#include "s2/palette_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/ninja.hpp"

namespace shenmue {
namespace s2 {

PaletteSlot *g_paletteSlots;
std::int16_t g_paletteSlotCount;
std::uint16_t g_paletteBlockMasks[4];
std::int32_t g_paletteDisabled = 0;
std::int32_t g_selectedPalette = 0;

// @verified - d
void InitializePaletteSlots(PaletteSlot *slots, std::int32_t count) {
    g_paletteSlots = slots;
    g_paletteSlotCount = static_cast<std::int16_t>(count);
    for (; count != 0; --count, ++slots) {
        slots->index = -1;
        slots->firstBlock = -1;
        slots->mode = 0xff;
        slots->references = 0;
        slots->blockCount = 0;
    }
    for (std::size_t rangeIndex162 = 0; rangeIndex162 < sizeof(g_paletteBlockMasks) / sizeof((g_paletteBlockMasks)[0]); ++rangeIndex162) {
        std::uint16_t &mask = (g_paletteBlockMasks)[rangeIndex162];
        mask = 0;
    }
}

// @verified - d
std::int32_t FindFreePaletteBlocks(std::int32_t mode, std::int32_t colourCount) {
    const std::int32_t blocks = static_cast<std::int32_t>(static_cast<std::uint32_t>(colourCount) + 15U) >> 4;
    std::int32_t available = 0;
    std::int32_t first = -1;
    if (mode == 16) {
        for (std::int32_t word = 0; word < 4; ++word) {
            std::uint32_t bit = 1;
            for (std::int32_t position = 0; position < 16; ++position, bit <<= 1U) {
                if ((g_paletteBlockMasks[word] & bit) == 0) {
                    if (available == 0)
                        first = word * 16 + position;
                    ++available;
                    if (available >= blocks)
                        break;
                } else {
                    available = 0;
                }
            }
        }
    } else {
        const std::uint32_t count = static_cast<std::uint32_t>(blocks);
        const std::uint32_t shifted = blocks < 0 ? ((count & 31U) == 0 ? 0U : 1U >> ((~count & 31U) + 1U)) : 1U << (count & 31U);
        const std::uint32_t mask = shifted - 1U;
        for (std::int32_t word = 0; word < 4; ++word) {
            if ((static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(g_paletteBlockMasks[word]))) & mask) == 0) {
                first = word * 16;
                available = blocks;
                break;
            }
        }
    }
    return available < blocks ? -1 : first;
}

// @verified - d
void ReservePaletteBlocks(std::int32_t firstBlock, std::int32_t colourCount) {
    std::int32_t word = firstBlock >> 4;
    std::uint32_t bit = 1U << (static_cast<std::uint32_t>(firstBlock) & 15U);
    std::int32_t blocks = static_cast<std::int32_t>(static_cast<std::uint32_t>(colourCount) + 15U) >> 4;
    while (blocks > 0) {
        --blocks;
        g_paletteBlockMasks[word] |= static_cast<std::uint16_t>(bit);
        bit <<= 1U;
        if (bit > 0xffffU) {
            ++word;
            bit = 1;
        }
    }
}

// @verified - d
void ReleasePaletteSlot(std::int32_t index) {
    PaletteSlot &slot = g_paletteSlots[index];
    if (slot.references >= 2) {
        --slot.references;
        return;
    }
    const std::int32_t firstBlock = slot.firstBlock;
    std::uint32_t blocks = slot.blockCount;
    slot.index = -1;
    slot.firstBlock = -1;
    slot.mode = 0xff;
    slot.references = 0;
    std::uint32_t bit = 1U << (static_cast<std::uint32_t>(firstBlock) & 15U);
    std::int32_t word = firstBlock >> 4;
    while (blocks != 0) {
        --blocks;
        g_paletteBlockMasks[word] &= static_cast<std::uint16_t>(~bit);
        bit <<= 1U;
        if (bit == 0x10000U) {
            bit = 1;
            ++word;
        }
    }
}

// @verified - d
std::int32_t GetPaletteFirstBlock(std::int32_t index) {
    return g_paletteSlots[index].firstBlock;
}

void UpdatePaletteColours(std::int32_t index, const void *colours) {
    const PaletteSlot &slot = g_paletteSlots[index];
    if (slot.references != 0)
        njSetPaletteData(slot.firstBlock * 16, slot.blockCount * 16, const_cast<void *>(colours));
}

std::int32_t LoadPaletteReference(std::int32_t index, const PaletteData &palette) {
    if (g_paletteDisabled != 0)
        return 0;
    if (index >= g_paletteSlotCount)
        return 1;
    PaletteSlot &slot = g_paletteSlots[index];
    if (slot.references != 0) {
        slot.references = static_cast<std::int16_t>(static_cast<std::uint16_t>(slot.references) + 1U);
        return 0;
    }
    const std::int32_t firstBlock = FindFreePaletteBlocks(palette.mode, palette.colourCount);
    if (firstBlock < 0)
        return 1;
    slot.mode = static_cast<std::uint8_t>(palette.mode);
    slot.blockCount = static_cast<std::uint8_t>((static_cast<std::uint32_t>(palette.colourCount) + 15U) >> 4U);
    slot.references = 1;
    slot.index = static_cast<std::int16_t>(index);
    slot.firstBlock = static_cast<std::int16_t>(firstBlock);
    ReservePaletteBlocks(firstBlock, palette.colourCount);
    njSetPaletteData(firstBlock * 16, palette.colourCount, const_cast<void *>(palette.colours));
    return 0;
}

// @verified - d
void ReleasePaletteReference(std::int32_t index) {
    if (g_paletteDisabled == 0 && index < g_paletteSlotCount)
        ReleasePaletteSlot(index);
}

// @verified - d
void SetSelectedPalette(std::int32_t index) {
    g_selectedPalette = index;
}

} // namespace s2
} // namespace shenmue

#endif
