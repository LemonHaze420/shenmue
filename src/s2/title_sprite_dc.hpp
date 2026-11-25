#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include "common/math.hpp"
#include "s2/legacy_sprite_types.hpp"

namespace shenmue {
namespace s2 {

struct TempTextureDefinition {
    const char *file;
    std::int32_t textureCount;
};
typedef char TitleSpriteLayoutCheck1[(sizeof(TempTextureDefinition) == 8) ? 1 : -1];
#define TITLE_COMMON_TEMP_TASK SHEN_NAME(TEMP)
#define TITLE_SPRITE_FILE SHEN_NAME(TSSS)
static const TempTextureDefinition tempTextureDefinitions[38] = {
    {"TCOMMON.GZ", 2},
    {"VM_T.GZ", 2},
    {"TFSEL.GZ", 2},
    {"ABG01", 2},
    {"ABG02", 2},
    {"A01", 2},
    {"A02", 2},
    {"A03", 2},
    {"A04", 2},
    {"A05", 2},
    {"A06", 2},
    {"A07", 2},
    {"A08", 2},
    {"A09", 2},
    {"A10", 2},
    {"A11", 2},
    {"A12", 2},
    {"A26", 2},
    {"A27", 2},
    {"A28", 2},
    {"A30", 2},
    {"A31", 2},
    {"A32", 2},
    {"A33", 2},
    {"A34", 2},
    {"A35", 2},
    {"A36", 2},
    {"A38", 2},
    {"A39", 2},
    {"A40", 2},
    {"A41", 2},
};
static const std::uint16_t titleCommonTempRectangles[8] = {0, 0, 255, 128, 0, 128, 255, 128};
static const std::uint16_t titleFileSelectRectangles[160] = {112, 176, 16, 32, 128, 176, 16, 32, 144, 176, 16, 32, 160, 176, 16, 32, 176, 176, 16, 32, 192, 176,
    16, 32, 208, 176, 16, 32, 224, 176, 16, 32, 240, 176, 16, 32, 256, 176, 16, 32, 272, 176, 16, 32, 288, 176, 16, 32, 304, 176, 16, 32, 112, 208, 50, 25, 162,
    208, 50, 25, 212, 208, 50, 25, 112, 233, 50, 25, 162, 233, 50, 25, 212, 233, 50, 25, 112, 258, 50, 25, 162, 258, 50, 25, 212, 258, 50, 25, 262, 208, 50, 25,
    262, 233, 50, 25, 262, 258, 50, 25, 0, 272, 82, 22, 0, 294, 82, 22, 0, 0, 320, 56, 0, 56, 320, 56, 0, 112, 146, 64, 146, 112, 146, 64, 0, 176, 112, 32, 0,
    208, 112, 32, 0, 240, 112, 32, 108, 310, 384, 92, 368, 32, 144, 278, 0, 316, 108, 28, 0, 344, 108, 28, 0, 372, 108, 28, 112, 283, 210, 27};
static const std::uint16_t titleLogoDimensions[5][2] = {{512, 128}, {256, 256}, {256, 128}, {128, 128}, {256, 256}};
extern std::int32_t titleSpriteTextures[15];
std::int32_t ReadTempTextureEntries(const void *source, TempTextureEntry *entries);
std::int32_t ReadTitleSpriteTextures(const void *source, std::uint32_t size, std::int32_t *textures);
void FreeTempSprites(std::int32_t index);
std::int32_t AllocateTempSprites(std::int32_t index);
bool BindTempSprite(std::int32_t index, std::int32_t texture, std::int32_t slot, const std::uint16_t *rectangles);
void InitializeTitleCommonTempSprites();
void InitShotsImages();
void LoadAreaSpriteImages();
void InitializeTitlePacket(LegacySpritePacket &packet);
void DrawTitleLogo(std::int32_t texture, float alpha);
void DrawTitlePressStart(LegacySpritePacket packet, std::int32_t frame);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct TitleTextureRectangle {
    std::int16_t left, top, right, bottom;
};
typedef char TitleSpriteLayoutCheck2[(sizeof(TitleTextureRectangle) == 8) ? 1 : -1];
static const TitleTextureRectangle titleOptionPanelRectangles[11] = {{0, 796, 625, 1000}, {0, 593, 625, 796}, {625, 796, 988, 1000}, {625, 593, 988, 790},
    {0, 417, 341, 593}, {343, 417, 683, 593}, {0, 242, 400, 417}, {683, 457, 722, 593}, {722, 457, 761, 593}, {761, 457, 800, 593}, {800, 457, 839, 593}};
static const TitleTextureRectangle titleOptionLabelRectangles[15] = {{0, 875, 410, 1000}, {410, 875, 1000, 1000}, {0, 750, 566, 875}, {566, 750, 1000, 875},
    {625, 625, 1000, 750}, {0, 625, 625, 750}, {0, 500, 625, 625}, {0, 375, 625, 500}, {0, 250, 625, 375}, {0, 125, 625, 250}, {0, 0, 625, 125},
    {625, 375, 1000, 500}, {625, 250, 1000, 375}, {625, 125, 1000, 250}, {625, 0, 1000, 125}};
struct TitleOptionLabel {
    std::int16_t rectangle, retained;
};
struct TitleOptionChoiceStyle {
    std::int16_t panel, arrow;
};
typedef char TitleSpriteLayoutCheck3[(sizeof(TitleOptionLabel) == 4) ? 1 : -1];
typedef char TitleSpriteLayoutCheck4[(sizeof(TitleOptionChoiceStyle) == 4) ? 1 : -1];
static const TitleOptionLabel titleOptionLabels[5] = {{0, 105}, {4, 96}, {1, 151}, {2, 145}, {3, 111}};
static const std::int8_t titleOptionLimits[5] = {4, 2, 2, 2, 2};
static const std::int16_t titleOptionValueLabels[3][2] = {{11, 12}, {6, 5}, {13, 14}};
static const std::int16_t titleOptionArrowPositions[2] = {402, 422};
static const TitleOptionChoiceStyle titleOptionChoiceStyles[4] = {{4, 7}, {5, 10}, {5, 9}, {4, 8}};
static const std::int8_t titleOptionDialogueLabels[4] = {7, 9, 8, 10};
static const std::int8_t titleOptionDialogueInfo[4][2] = {{7, -1}, {9, 10}, {8, -1}, {11, -1}};
static const std::int8_t titleOptionItemInfo[4][2] = {{7, -1}, {5, -1}, {6, -1}, {16, -1}};
extern const TitleOptionChoiceStyle *titleOptionChoiceCursor;
void DrawTitleOptionSprite(std::int32_t texture, std::int32_t rectangle, const Vector3 &position);
void DrawTitleOptionValue(const std::int16_t *labels, std::int32_t value, float y);
static const std::int16_t titleTextureDimensions[15][2] = {{1024, 512}, {512, 32}, {512, 128}, {256, 256}, {256, 128}, {128, 128}, {256, 256}, {512, 256},
    {512, 256}, {256, 256}, {512, 512}, {512, 512}, {512, 128}, {64, 64}, {512, 128}};
static const TitleTextureRectangle titleMenuHeadingRectangles[2] = {{0, 796, 625, 1000}, {0, 593, 625, 796}};
static const TitleTextureRectangle titleMenuOrbRectangles[2] = {{0, 0, 296, 593}, {296, 0, 593, 593}};
static const TitleTextureRectangle titleMenuLabelRectangles[3] = {{625, 843, 957, 1000}, {625, 687, 957, 843}, {625, 531, 957, 687}};
extern LegacySpritePacket titleSpritePacket;
void DrawScaledTitleSprite(const TitleTextureRectangle *rectangle, const Vector3 &position, std::int32_t texture, float scale);
void DrawTitleMenuBackground();
void DrawTitleControllerIcons();
void DrawTitleInfoLine(std::int32_t line, float y);
void DrawTitleInfoBar(std::int32_t line, std::int32_t secondaryLine);
void DrawTitleBackgroundAndButtons(std::uint32_t flags);
void DrawTitleMenuWheel();
#endif

} // namespace s2
} // namespace shenmue

#endif
