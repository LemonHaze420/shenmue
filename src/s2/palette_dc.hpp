#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/legacy_types.hpp"

#define PAL_LAYOUT_NAME_(line) PALLayoutCheck##line
#define PAL_LAYOUT_NAME(line) PAL_LAYOUT_NAME_(line)
#define PAL_LAYOUT(condition) typedef char PAL_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {

struct PaletteSlot {
    std::int16_t firstBlock;
    std::uint8_t mode;
    std::uint8_t blockCount;
    std::int16_t references;
    std::int16_t index;
};

struct PaletteData {
    std::int16_t index;
    std::int16_t mode;
    const void *colours;
    std::int32_t colourCount;
};

PAL_LAYOUT(sizeof(PaletteSlot) == 8);
PAL_LAYOUT(offsetof(PaletteSlot, references) == 4);
PAL_LAYOUT(sizeof(PaletteData) == 0x0c);
PAL_LAYOUT(offsetof(PaletteData, colours) == 4);

extern PaletteSlot *g_paletteSlots;
extern std::int16_t g_paletteSlotCount;
extern std::uint16_t g_paletteBlockMasks[4];
extern std::int32_t g_paletteDisabled;
extern std::int32_t g_selectedPalette;

void InitializePaletteSlots(PaletteSlot *slots, std::int32_t count);
std::int32_t FindFreePaletteBlocks(std::int32_t mode, std::int32_t colourCount);
void ReservePaletteBlocks(std::int32_t firstBlock, std::int32_t colourCount);
void ReleasePaletteSlot(std::int32_t index);
std::int32_t GetPaletteFirstBlock(std::int32_t index);
void UpdatePaletteColours(std::int32_t index, const void *colours);
std::int32_t LoadPaletteReference(std::int32_t index, const PaletteData &palette);
void ReleasePaletteReference(std::int32_t index);
void SetSelectedPalette(std::int32_t index);

}
}
#undef PAL_LAYOUT_NAME
#undef PAL_LAYOUT_NAME_
#undef PAL_DATA
#undef PAL_INIT
#undef PAL_LAYOUT

#endif
