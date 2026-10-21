#pragma once

#include "s2/ui.hpp"
#include "s2/map_assets.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name COLLECTION_SPRITE_FILE{SHEN_NAME(ICCL)};
inline constexpr shen_name TITLE_COMMON_TEMP_TASK{SHEN_NAME(TEMP)};
inline constexpr shen_name TITLE_SPRITE_FILE{SHEN_NAME(TSSS)};
inline constexpr shen_name TITLE_GTEXT_FILE{SHEN_NAME(TSSR)};
inline constexpr shen_name TITLE_CTEXT_FILE{SHEN_NAME(TSSQ)};
inline constexpr shen_name TITLE_KEY_IMAGE_FILE{SHEN_NAME(TSSK)};
inline constexpr shen_name TITLE_CONTINUE_FILE{SHEN_NAME(TSSP)};
inline constexpr shen_name TITLE_ESRB_FILE{SHEN_NAME(E3BS)};
extern std::array<std::int32_t, 21> titleSpriteTextures;
inline constexpr std::array<std::array<std::uint16_t, 2>, 21> TITLE_SPRITE_DIMENSIONS{
    {{1024, 512}, {512, 32}, {256, 128}, {512, 512}, {256, 128}, {128, 128}, {256, 256}, {512, 512}, {512, 256}, {256, 256}, {512, 512}, {256, 256}, {64, 64},
        {512, 512}, {512, 512}, {512, 512}, {512, 128}, {64, 64}, {0, 0}, {0, 0}, {0, 0}}};
extern std::int32_t titleGTextTexture, titleCTextTexture, titleKeyImageTexture, titleContinueTexture, titleEsrbTexture;
void InitShotsImages();
struct TempTextureEntry {
    std::int32_t texture;
    std::uint16_t width, height;
};
static_assert(sizeof(TempTextureEntry) == 8);
static_assert(offsetof(TempTextureEntry, width) == 4);
static_assert(offsetof(TempTextureEntry, height) == 6);
struct TempTextureDefinition {
    const char *file;
    std::int32_t textureCount;
    std::int32_t reserved0c;
};
static_assert(sizeof(TempTextureDefinition) == 0x10);
inline constexpr std::array<TempTextureDefinition, 39> TEMP_TEXTURE_DEFINITIONS{{{"TCOMMON.GZ", 2, 0}, {"ABG01", 2, 0}, {"ABG02", 2, 0}, {"A01", 2, 0},
    {"A02", 2, 0}, {"A03", 2, 0}, {"A04", 2, 0}, {"A05", 2, 0}, {"A06", 2, 0}, {"A07", 2, 0}, {"A08", 2, 0}, {"A09", 2, 0}, {"A10", 2, 0}, {"A11", 2, 0},
    {"A12", 2, 0}, {"A26", 2, 0}, {"A27", 2, 0}, {"A28", 2, 0}, {"A30", 2, 0}, {"A31", 2, 0}, {"A32", 2, 0}, {"A33", 2, 0}, {"A34", 2, 0}, {"A35", 2, 0},
    {"A36", 2, 0}, {"A38", 2, 0}, {"A39", 2, 0}, {"A40", 2, 0}, {"A41", 2, 0}, {"A42", 2, 0}, {"A43", 2, 0}, {"A45", 2, 0}, {nullptr, 0, 0}, {nullptr, 0, 0},
    {nullptr, 0, 0}, {nullptr, 0, 0}, {nullptr, 0, 0}, {nullptr, 0, 0}, {nullptr, 0, 0}}};
enum TempTextureFlag : std::uint16_t { TTF_Loaded = 1, TTF_Region = 2 };
struct TempTextureWork {
    std::uint16_t flags;
    std::int16_t texture;
    std::uint16_t width, height;
    std::array<float, 4> uv;
};
static_assert(sizeof(TempTextureWork) == 0x18);
static_assert(offsetof(TempTextureWork, uv) == 8);
static_assert(offsetof(TempTextureWork, texture) == 2);
inline constexpr std::array<std::uint16_t, 8> TITLE_COMMON_TEMP_RECTANGLES{0, 0, 255, 128, 0, 128, 255, 128};
inline constexpr std::array<std::array<std::uint32_t, 4>, 2> TITLE_CONFIRM_CHOICES{{{37, 0, 36, 1}, {36, 0, 37, 1}}};
extern float legacySpriteSlotScaleX, legacySpriteSlotScaleY, legacySpriteSlotPulse;
extern std::uint32_t legacySpriteSlotColour, legacySpriteSlotSecondaryColour;
extern std::int32_t legacySpriteSlotWidthOffset, legacySpriteSlotHeightOffset, legacySpriteSlotRotation;
extern const std::uint32_t *titleConfirmChoice;
std::int64_t ResetLegacySpriteSlotDrawState();
void DrawTitleSelection(std::int32_t x, std::int32_t y, std::int32_t separation, std::int32_t choice);
std::int32_t InitializeTitleCommonTempSprites();
extern std::array<TempTextureEntry *, 39> g_tempTextures;
void FreeTempSprites(std::int32_t index);
std::int32_t AllocateTempSprites(std::int32_t index);
const TempTextureWork &GetTempSpriteTexture(std::int32_t index, std::int32_t texture);
bool BindTempSprite(std::int32_t index, std::int32_t texture, std::uint16_t slot, const std::uint16_t *rectangles);
void DrawTempSpriteSlot(std::uint32_t icon, std::uint32_t position, float depth, std::uint8_t flags);
void DrawTempSpriteDigit(std::uint32_t icon, std::uint32_t position, float depth, std::int32_t digit);

enum class ScreenAlignment : std::int32_t {
    VerticalCenter = 1,
    HorizontalCenter = 2,
    BothCentered = 3,
    VerticalStretch = 4,
    HorizontalStretch = 5,
    FullStretch = 6,
};
enum class ScreenMode : std::int32_t { Centered = 1, Stretched = 2, Adjusted = 3, AdjustedWithOffset = 4 };
inline constexpr std::array<std::uint16_t, 4> itemWindowEdgeIcons{1025, 1026, 1027, 1028};
inline constexpr std::array<float, 2> itemWindowEdgeOffsets{11.0f, -11.0f};
inline constexpr MapRenderProfile initialSpriteAreaConfiguration{.areaPattern = 0,
    .field04 = 0x30000,
    .field08 = 0xe0000,
    .field0c = 0x30000,
    .field10 = 0x400000,
    .farDistance = 140.0f,
    .nearDistance = 140.0f,
    .npcRadius = 48.0f,
    .alphaOperation = 0,
    .textureDistance = 0.0f,
    .textureDistanceW = 0.0f,
    .textureDistanceZ = 0.0f};


void SetSpriteAreaConfigurations();
void InitializeSprites(std::int32_t useCurrentArea, std::int32_t initializeCommonTextures);
std::int64_t ReloadSpriteResources();
std::int64_t ResetUnusedReloadState();
std::uint32_t InitializeSpriteBuffersNullsub(std::int32_t mode, std::int32_t firstCapacity, std::int32_t secondCapacity, std::int32_t flags);
std::uint32_t EnsureSpriteWorkInitialized();

struct LegacyScreenQuad {
    std::uint32_t flags;
    std::array<float, 8> corners;
};
static_assert(sizeof(LegacyScreenQuad) == 0x24);
static_assert(offsetof(LegacyScreenQuad, corners) == 4);
struct LegacyColourQuad {
    LegacyScreenQuad quad;
    std::uint32_t colour;
    std::array<std::uint32_t, 3> alternateColours;
};
static_assert(sizeof(LegacyColourQuad) == 0x34);


struct LegacySpritePacket {
    LegacyScreenQuad quad;
    std::uint32_t colour;
    std::array<std::byte, 12> reservedColour;
    std::uint32_t secondaryColour;
    std::array<std::byte, 12> reservedUv;
    std::array<float, 4> uv;
    std::uint32_t reservedTail;
};
static_assert(sizeof(LegacySpritePacket) == 0x58);
static_assert(offsetof(LegacySpritePacket, colour) == 0x24);
static_assert(offsetof(LegacySpritePacket, secondaryColour) == 0x34);
static_assert(offsetof(LegacySpritePacket, uv) == 0x44);

struct LegacySolidRect {
    float x, y, scaleX, scaleY, depth;
    std::uint32_t colour;
    std::int16_t width, height;
};
static_assert(sizeof(LegacySolidRect) == 28);
static_assert(offsetof(LegacySolidRect, depth) == 16);
static_assert(offsetof(LegacySolidRect, colour) == 20);
static_assert(offsetof(LegacySolidRect, width) == 24);
bool DrawLegacySolidRect(const LegacySolidRect &rect);

struct LegacyColourRect {
    float x, y, scaleX, scaleY, depth;
    std::uint32_t colour;
    std::int16_t width, height;
    std::int32_t rotation;
    std::uint8_t alignment;
    std::array<std::byte, 3> reserved21;
};
static_assert(sizeof(LegacyColourRect) == 0x24);
static_assert(offsetof(LegacyColourRect, colour) == 0x14);
static_assert(offsetof(LegacyColourRect, rotation) == 0x1c);
static_assert(offsetof(LegacyColourRect, alignment) == 0x20);
bool DrawLegacyColourRect(const LegacyColourRect &rect);
inline constexpr std::array<std::int32_t, 3> TITLE_HINT_MASKS{0, 4, 8};
void DrawTitleHints(std::int32_t flags);
void DrawTitleFrame(std::int32_t flags);
void DrawTitleCaptionLine(std::int32_t y, std::int32_t item);
void DrawTitleCaption(std::int32_t item, std::int32_t secondary);
void DrawTitleColourSpriteRect(const std::uint16_t *uv, const Vector3 &position, std::int32_t texture, float scale, std::uint32_t colour);
void DrawSizedTitleSpriteRect(const std::uint16_t *uv, const Vector3 &position, std::int32_t texture, std::int16_t width, std::int16_t height);
void DrawTitleBackground();
void DrawTitleSpriteRect(const std::uint16_t *uv, const Vector3 &position, std::int32_t texture, float scale);
std::int32_t TruncateSpriteCoordinate(float value) noexcept;
void AdjustScreenAspectRatio(
    LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, ScreenMode mode);
void TransformScreenSpace(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
    float scaleX, float scaleY, std::int32_t rotation, ScreenMode mode);
void PrepareDrawSimple(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode);
void PrepareDrawWithMode(const LegacyIconSprite &sprite, ScreenMode mode);

void PrepareDrawSimpleOpaque(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode);
void PrepareDrawTextureWithMode(const LegacyIconSprite &sprite, ScreenMode mode);


void PrepareDrawIndexedTexture(const LegacyIconSprite &sprite, ScreenMode mode);
void shenTransformScreenCoors(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode);
void PrepareDrawIndexedTextureWithSecondary(const LegacyIconSprite &sprite, std::uint32_t secondaryColour, ScreenMode mode);
void InitializeLegacyItemWindow();
void DrawLegacySelectionGlow(float x, float y, float depth, std::uint32_t &colour, bool &brightening, ScreenMode mode);
void DrawLegacyItemWindow(const Vector2 &position, float depth, std::int32_t columns, std::int32_t rows, ScreenMode mode);
void DrawLegacyWindowBackground(float x, float y, float width, float height, float depth, std::int32_t flags, ScreenMode mode);
void DrawLegacyWindowEdges(float x, float y, std::int32_t rows, std::int32_t columns, float depth, std::int32_t flags, ScreenMode mode);
void DrawLegacyWindowHorizontal(
    float x, float y, std::int32_t columns, std::int32_t edgeType, std::int32_t flipped, float depth, std::int32_t flags, ScreenMode mode);
void DrawLegacyWindowTiles(float x, float y, std::int32_t columns, std::int32_t rows, float depth, std::int32_t flags, ScreenMode mode);
bool shenSubmitQuad(const LegacyColourQuad &packet, float depth);
bool shenSubmitQuadsUI(float x, float y, std::uint32_t fill, std::uint32_t border, std::int32_t width, std::int32_t height, float depth);
void LegacySpriteNullsub(const LegacySpritePacket &packet, std::uint32_t texture, float depth);
struct BorrowedLegacySpriteSlot {
    const std::uint16_t *rectangles;
    std::uint16_t slot;
    std::int16_t texture;
    float inverseWidth;
    float inverseHeight;
    std::uint32_t reserved14;
};
static_assert(sizeof(BorrowedLegacySpriteSlot) == 0x18);
bool RegisterBorrowedLegacySpriteSlot(std::int16_t texture, std::int32_t width, std::int32_t height, std::uint16_t slot, const std::uint16_t *rectangles);
void ReleaseBorrowedLegacySpriteSlot(std::uint16_t slot);
void DrawBorrowedLegacySprite(std::uint32_t icon, std::int32_t x, std::int32_t y, float scaleX, float scaleY, std::int32_t rotation, std::uint32_t colour);
bool vfSubmitQuad(std::uint32_t colour, float depth);
void DrawLegacyIconSprite(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode);
void SetSpriteTexturePath(const char *path) noexcept;
void ApplySpriteTextureOverride(const void *texn, std::uint32_t textureHandle);
std::int64_t QueueSDTextureOverride(const char *path, std::uint32_t renderHandle);
void ReleaseSDTextureOverrides();
void ResetLegacyCollectionSpriteOwners() noexcept;
std::int64_t LoadLegacyCollectionSprites(shen_name owner);
void ReleaseLegacyCollectionSprites(shen_name owner);
void InitializeLegacyQuantitySprites();
void DrawLegacyItemQuantity(float x, float y, std::int32_t quantity, float depth);

} // namespace shenmue::s2
