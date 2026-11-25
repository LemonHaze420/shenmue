#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/legacy_sprite_types.hpp"
#include "common/types.hpp"
#include "common/math.hpp"
#define SPRITE_DATA const
#define SPRITE_INIT =
namespace shenmue {
namespace s2 {
SPRITE_DATA std::uint8_t kCommonIconRectangles[0x254] SPRITE_INIT{0, 6, 8, 8, 0, 4, 2, 6, 6, 5, 8, 6, 2, 4, 4, 6, 4, 4, 6, 6, 0, 0, 2, 2, 2, 3, 3, 4, 3, 3, 4,
    4, 4, 3, 5, 4, 5, 3, 6, 4, 6, 3, 7, 4, 7, 3, 8, 4, 2, 2, 3, 3, 3, 2, 4, 3, 4, 2, 5, 3, 5, 2, 6, 3, 6, 2, 7, 3, 7, 2, 8, 3, 2, 1, 3, 2, 3, 1, 4, 2, 4, 1, 5,
    2, 5, 1, 6, 2, 6, 1, 7, 2, 7, 1, 8, 2, 2, 0, 3, 1, 3, 0, 4, 1, 4, 0, 5, 1, 5, 0, 6, 1, 6, 0, 7, 1, 6, 4, 7, 5, 7, 4, 8, 5, 0, 4, 4, 8, 4, 4, 5, 8, 5, 4, 6,
    8, 6, 4, 7, 8, 7, 4, 8, 8, 0, 3, 2, 4, 0, 1, 2, 3, 5, 1, 8, 2, 2, 3, 5, 4, 2, 2, 5, 3, 2, 1, 5, 2, 2, 0, 5, 1, 5, 3, 8, 4, 5, 2, 8, 3, 5, 0, 6, 1, 6, 0, 7,
    1, 0, 4, 2, 6, 0, 2, 2, 4, 2, 4, 4, 6, 2, 2, 4, 4, 4, 4, 6, 6, 4, 2, 6, 4, 6, 4, 8, 6, 6, 2, 8, 4, 2, 6, 6, 8, 0, 7, 2, 8, 0, 6, 2, 7, 0, 0, 2, 2, 2, 0, 4,
    2, 4, 1, 5, 2, 4, 0, 5, 1, 5, 1, 6, 2, 5, 0, 6, 1, 6, 1, 7, 2, 0, 7, 1, 8, 1, 7, 2, 8, 2, 7, 3, 8, 3, 7, 4, 8, 4, 7, 5, 8, 5, 7, 6, 8, 6, 7, 7, 8, 7, 7, 8,
    8, 0, 6, 1, 7, 1, 6, 2, 7, 2, 6, 3, 7, 3, 6, 4, 7, 4, 6, 5, 7, 5, 6, 6, 7, 6, 6, 7, 7, 7, 6, 8, 7, 0, 5, 1, 6, 1, 5, 2, 6, 2, 5, 3, 6, 3, 5, 4, 6, 4, 5, 5,
    6, 5, 5, 6, 6, 6, 5, 7, 6, 7, 5, 8, 6, 0, 4, 1, 5, 1, 4, 2, 5, 2, 4, 3, 5, 3, 4, 4, 5, 4, 4, 5, 5, 5, 4, 6, 5, 6, 4, 7, 5, 7, 4, 8, 5, 0, 3, 1, 4, 1, 3, 2,
    4, 2, 3, 3, 4, 3, 3, 4, 4, 4, 3, 5, 4, 5, 3, 6, 4, 6, 3, 7, 4, 7, 3, 8, 4, 0, 2, 1, 3, 1, 2, 2, 3, 2, 2, 3, 3, 3, 2, 4, 3, 4, 2, 5, 3, 5, 2, 6, 3, 6, 2, 7,
    3, 7, 2, 8, 3, 0, 1, 1, 2, 1, 1, 2, 2, 2, 1, 3, 2, 3, 1, 4, 2, 4, 1, 5, 2, 5, 1, 6, 2, 6, 1, 7, 2, 7, 1, 8, 2, 0, 0, 1, 1, 1, 0, 2, 1, 2, 0, 3, 1, 3, 0, 4,
    1, 4, 0, 5, 1, 5, 0, 6, 1, 6, 0, 7, 1, 7, 0, 8, 1, 0, 6, 2, 8, 2, 6, 4, 8, 4, 6, 6, 8, 6, 6, 8, 8, 0, 4, 2, 6, 2, 4, 4, 6, 4, 4, 6, 6, 6, 4, 8, 6, 0, 2, 2,
    4, 2, 2, 4, 4, 4, 2, 6, 4, 6, 2, 8, 4, 0, 0, 2, 2, 2, 0, 4, 2, 4, 0, 6, 2, 6, 0, 8, 2, 0, 4, 4, 8, 4, 4, 8, 8, 0, 0, 4, 4, 4, 0, 8, 4};
SPRITE_DATA std::uint16_t kCommonIconRectangleOffsets[14] SPRITE_INIT{
    0x104, 0x204, 0x204, 0x204, 0, 0x7c, 0xbc, 0x244, 0x244, 0x104, 0x104, 0x104, 0x104, 0x204};
struct PakGlobalData;
extern std::int32_t g_commonIconTextures[14];
extern std::int32_t g_commonIconTextureCount;
void LoadCommonIconSprites(PakGlobalData *package);
void LoadLoadScreenSpriteTexture(PakGlobalData *package);
const std::int8_t *FindCommonIconRectangle(std::int16_t icon);
std::int32_t GetCommonIconTextureIndex(std::uint32_t icon);
void PrepareDrawSimple(const LegacyIconSprite &sprite, std::uint32_t colour);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
const std::int8_t *GetCommonIconRect(std::int32_t icon);
void InitializeLegacyIconSprite(LegacyIconSprite &sprite);
extern LegacyIconSprite itemWindowEdges[4], itemWindowSelection, quantityDigits[8], quantityBrackets[2];
extern LegacySpritePacket itemWindowTiles, itemWindowSeparators;
void DrawLegacyWindowHorizontal(float x, float y, std::int32_t columns, std::int32_t edgeType, std::int32_t flipped, float depth, std::int32_t flags);
void DrawLegacyWindowTiles(float x, float y, std::int32_t columns, std::int32_t rows, float depth, std::int32_t flags);
void DrawLegacyWindowBackground(float x, float y, float width, float height, float depth, std::int32_t flags);
bool LoadLegacyCollectionSprites(shen_name owner);
void ReleaseLegacyCollectionSprites(shen_name owner);
void DrawLegacyWindowEdges(float x, float y, std::int32_t rows, std::int32_t columns, float depth, std::int32_t flags);
void DrawLegacyItemWindow(const Vector2 &position, float depth, std::int32_t columns, std::int32_t rows);
void DrawLegacySelectionGlow(const Vector2 &position, std::uint32_t &colour, bool &brightening, float depth);
void shenSubmitQuadsUI(float x, float y, std::uint32_t fill, std::uint32_t border, std::int32_t width, std::int32_t height, float depth);
void DrawMoneyDigits(float x, float y, float scale, float depth, std::int32_t amount, std::uint32_t colour);
void DrawLegacyItemQuantity(float x, float y, std::int32_t quantity, float depth);
void PrepareDrawSimpleOpaque(const LegacyIconSprite &sprite, std::uint32_t colour);
void DrawMainIconOverlay(std::uint32_t colour, float depth);
void DrawLegacyColourRect(const LegacyColourRect &rect);
void PrepareDrawIndexedTextureWithSecondary(const LegacyIconSprite &sprite, std::uint32_t secondaryColour);
void shenTransformScreenCoors(const LegacyIconSprite &sprite, std::uint32_t colour);
#endif
void PrepareDrawWithMode(const LegacyIconSprite &sprite);
void PrepareDrawTextureWithMode(const LegacyIconSprite &sprite);
extern TempTextureEntry *g_tempTextures[38];
extern TempTextureWork titleCommonTempWork;
extern float legacySpriteSlotScaleX, legacySpriteSlotScaleY, legacySpriteSlotPulse;
extern std::int32_t legacySpriteSlotWidthOffset, legacySpriteSlotHeightOffset, legacySpriteSlotRotation;
extern std::uint32_t legacySpriteSlotColour, legacySpriteSlotSecondaryColour;
void SetLegacySpriteSlotScale(float x, float y);
void SetLegacySpriteSlotSizeOffsets(std::int32_t width, std::int32_t height);
void SetLegacySpriteSlotRotation(std::int32_t angle);
void SetLegacySpriteSlotColour(std::uint32_t colour);
void SetLegacySpriteSlotSecondaryColour(std::uint32_t colour);
void ResetLegacySpriteSlotDrawState();
const TempTextureWork &GetBorrowedSpriteTexture(std::int32_t index, std::int32_t texture);
const TempTextureWork &GetTempSpriteTexture(std::int32_t index, std::int32_t texture);
void ResetTempTextureReferences();
void AdjustScreenAspectRatio(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height);
void TransformScreenSpace(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
    float scaleX, float scaleY, std::int32_t rotation);
float ConvertLegacyScreenDepth(float depth);
void DrawLegacyColourPacket(const LegacySpritePacket &packet, float depth);
void SetScreenQuadBounds(LegacyScreenQuad &quad, std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom);
void DrawLegacySpritePacket(const LegacySpritePacket &packet, std::uint32_t texture, float depth);
void DrawTempSpriteSlot(std::uint32_t icon, std::uint32_t position, float depth, std::uint32_t flags);
} // namespace s2
} // namespace shenmue
#undef SPRITE_DATA
#undef SPRITE_INIT
#endif
