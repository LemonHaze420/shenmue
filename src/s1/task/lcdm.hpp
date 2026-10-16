#pragma once

#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
struct ScenarioContext;

struct LcdmUtilityParameters {
    std::uint32_t mode;
    std::uint32_t dataOffset;
};

static_assert(sizeof(LcdmUtilityParameters) == 8);
static_assert(offsetof(LcdmUtilityParameters, dataOffset) == 4);

#pragma pack(push, 1)
struct LcdmTaskData {
    std::uint8_t field_000;
    std::byte reserved_001[7];
    const std::uint8_t *data;
    std::uint64_t field_010;
    std::uint32_t field_018;
    std::uint8_t pixels[192 * 8];
    std::uint16_t field_61c;
    std::uint8_t scoreBitmap[0xc2];
    void *archive;
    const std::uint8_t *packedEntries[16];
    const std::uint8_t *startText;
    const std::uint8_t *countText;
    const std::uint8_t *endText;
    std::uint8_t largeDigitsHigh[10][44];
    std::uint8_t largeDigitsLow[10][44];
    std::uint8_t smallDigitsHigh[10][20];
    std::uint8_t smallDigitsLow[10][20];
    std::uint8_t ordinalSuffixes[4][16];
    const std::uint8_t *buttonGlyphs[9];
    const std::uint8_t *glyphTable[8];
    std::uint8_t buttonLookup[8];
};
#pragma pack(pop)

static_assert(sizeof(LcdmTaskData) == 0xd50);
static_assert(offsetof(LcdmTaskData, pixels) == 0x1c);
static_assert(offsetof(LcdmTaskData, field_61c) == 0x61c);
static_assert(offsetof(LcdmTaskData, archive) == 0x6e0);
static_assert(offsetof(LcdmTaskData, packedEntries) == 0x6e8);
static_assert(offsetof(LcdmTaskData, countText) == 0x770);
static_assert(offsetof(LcdmTaskData, largeDigitsHigh) == 0x780);
static_assert(offsetof(LcdmTaskData, largeDigitsLow) == 0x938);
static_assert(offsetof(LcdmTaskData, smallDigitsHigh) == 0xaf0);
static_assert(offsetof(LcdmTaskData, smallDigitsLow) == 0xbb8);
static_assert(offsetof(LcdmTaskData, ordinalSuffixes) == 0xc80);
static_assert(offsetof(LcdmTaskData, buttonGlyphs) == 0xcc0);
static_assert(offsetof(LcdmTaskData, glyphTable) == 0xd08);
static_assert(offsetof(LcdmTaskData, buttonLookup) == 0xd48);

extern HLib::Task *g_lcdmTask;
extern LcdmTaskData *g_lcdmTaskData;
extern const std::uint8_t *const lcdmPackedBitmapTable[3];
extern std::uint8_t g_lcdmBlankBitmap[194];

void LCDM_APICode();
void lcdmInit(std::uintptr_t bitmap);
void lcdmSetValues(std::int32_t round, std::int32_t defeatedEnemies);
void lcdmDrawButtonGlyph(std::int32_t x, std::int32_t y, const std::uint8_t *glyph);
void lcdmRenderButtonGlyphs(const std::uint8_t *glyphList);
void lcdmRenderButtonCommands(const std::int32_t *commands);
std::int64_t LCDM_Init();
void LCDM_Shutdown();
void Script_LCDM_Utils(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
