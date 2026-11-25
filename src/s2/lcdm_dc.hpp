#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue {
namespace s2 {

struct LcdmTaskData {
    std::int8_t vibrationState;
    std::byte retained01[3];
    const std::uint8_t *vibrationCursor;
    std::int16_t vibrationFrames;
    std::uint8_t vibrationPacket[4];
    std::byte retained0e[2];
    std::int32_t displayDirty;
    std::uint8_t pixels[48 * 32];
    std::uint8_t width, height;
    std::byte retained616[0xc2];
    std::byte *resource;
    const std::uint8_t *images[19];
    std::uint8_t largeDigitsHigh[10][44], largeDigitsLow[10][44];
    std::uint8_t smallDigitsHigh[10][20], smallDigitsLow[10][20];
    const std::uint8_t *moveGlyphs[9], *orderedMoveGlyphs[8];
    std::uint8_t moveGlyphIndices[8];
};
typedef char Layout_lcdm_dc_0[(sizeof(LcdmTaskData) == 0xc74) ? 1 : -1];
typedef char Layout_lcdm_dc_1[(offsetof(LcdmTaskData, vibrationCursor) == 4) ? 1 : -1];
typedef char Layout_lcdm_dc_2[(offsetof(LcdmTaskData, vibrationFrames) == 8) ? 1 : -1];
typedef char Layout_lcdm_dc_3[(offsetof(LcdmTaskData, vibrationPacket) == 0xa) ? 1 : -1];
typedef char Layout_lcdm_dc_4[(offsetof(LcdmTaskData, displayDirty) == 0x10) ? 1 : -1];
typedef char Layout_lcdm_dc_5[(offsetof(LcdmTaskData, pixels) == 0x14) ? 1 : -1];
typedef char Layout_lcdm_dc_6[(offsetof(LcdmTaskData, width) == 0x614) ? 1 : -1];
typedef char Layout_lcdm_dc_7[(offsetof(LcdmTaskData, resource) == 0x6d8) ? 1 : -1];
typedef char Layout_lcdm_dc_8[(offsetof(LcdmTaskData, images) == 0x6dc) ? 1 : -1];
typedef char Layout_lcdm_dc_9[(offsetof(LcdmTaskData, largeDigitsHigh) == 0x728) ? 1 : -1];
typedef char Layout_lcdm_dc_10[(offsetof(LcdmTaskData, largeDigitsLow) == 0x8e0) ? 1 : -1];
typedef char Layout_lcdm_dc_11[(offsetof(LcdmTaskData, smallDigitsHigh) == 0xa98) ? 1 : -1];
typedef char Layout_lcdm_dc_12[(offsetof(LcdmTaskData, smallDigitsLow) == 0xb60) ? 1 : -1];
typedef char Layout_lcdm_dc_13[(offsetof(LcdmTaskData, moveGlyphs) == 0xc28) ? 1 : -1];
typedef char Layout_lcdm_dc_14[(offsetof(LcdmTaskData, orderedMoveGlyphs) == 0xc4c) ? 1 : -1];
typedef char Layout_lcdm_dc_15[(offsetof(LcdmTaskData, moveGlyphIndices) == 0xc6c) ? 1 : -1];

struct LcdmImageDirectory {
    std::uint32_t retained00, imageCount, tableOffset;
};
struct LcdmImageEntry {
    std::uint32_t name[2], retained08, dataOffset;
};
typedef char Layout_lcdm_dc_16[(sizeof(LcdmImageDirectory) == 12) ? 1 : -1];
typedef char Layout_lcdm_dc_17[(sizeof(LcdmImageEntry) == 16) ? 1 : -1];

static const char LCDM_IMAGE_NAMES[19][9] = {"SHENMUE ", "PK      ", "LOAD_JP ", "LOAD_US ", "SAVE_JP ", "SAVE_US ", "OK_JP   ", "OK_US   ", "NG_JP   ",
    "NG_US   ", "RSM_JP  ", "RSM_US  ", "RSM_OK  ", "RSM_NG  ", "", "", "BT_START", "BT_COUNT", "BT_END  "};
static const char LCDM_MOVE_NAMES[9][9] = {"WAZA_KB ", "WAZA_KF ", "WAZA_BA ", "WAZA_BB ", "WAZA_BX ", "WAZA_BY ", "WAZA_BL ", "WAZA_BR ", "WAZA_BP "};
static const std::uint8_t LCDM_MOVE_ORDER[8] = {4, 2, 1, 0, 6, 3, 5, 8};
static const std::uint8_t LCDM_VIBRATION_STOP[4] = {0xff, 0, 0, 0};
static const std::uint8_t LCDM_VIBRATION_PULSE[12] = {0xf0, 0xe0, 0x20, 0, 0, 0x20, 0xf0, 0xe0, 0x20, 0xff, 0, 0};
static const std::uint8_t LCDM_VIBRATION_RAMP[28] = {
    0xf0, 0xe0, 8, 0xf2, 0xe4, 8, 0xf4, 0xe8, 8, 0xf6, 0xec, 8, 0xf8, 0xf0, 8, 0xfa, 0xf4, 8, 0xfc, 0xf8, 8, 0xfe, 0xfc, 8, 0xff, 0, 0, 0};
extern const std::uint8_t *LCDM_VIBRATION_PATTERNS[3];
static const std::int32_t LCDM_GLYPH_START_X[5] = {0, 19, 13, 8, 2};
static const std::int32_t LCDM_GLYPH_ROW_Y[3] = {0, 9, 1};

extern HLib::Task *g_lcdmTask;
extern LcdmTaskData *g_lcdmTaskData;
extern std::uint32_t g_lcdPeripheralPort;
extern char g_lcdmSmallDigitName[9], g_lcdmLargeDigitName[9];

const std::uint8_t *FindLcdmImageData(const void *resource, const char *name);
void InitializeLcdmTaskData();
void FreeLcdmTaskData();
std::uint32_t GetLcdPeripheralPort();
std::int32_t IsDiscTrayOpen();
void LcdmCallback(void *data);
std::int32_t initlcdTask();
void ShutdownLcdmTask();
void LcdmInit(std::uintptr_t image);
void LcdmSelectMessage(std::uintptr_t message);
void lcdmDrawButtonGlyph(std::int32_t x, std::int32_t y, const std::uint8_t *glyph);
void lcdmRenderButtonGlyphs(const std::uint8_t *glyphs);
void lcdmRenderButtonCommands(const std::int32_t *commands);
void LcdmComposeBattleValues(std::int32_t smallValue, std::int32_t largeValue);
void LcdmDisplayBattleStart();
void LcdmDisplayBattleEnd();

} // namespace s2
} // namespace shenmue

#endif
