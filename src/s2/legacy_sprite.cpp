#include "shenmue.h"
#include "s2/legacy_sprite_types.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/legacy_sprite_dc.hpp"
#include "s2/legacy_sprite_slot.hpp"
#include "common/math.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/money_icon.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/pause.hpp"
#include "s2/texture_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/save.hpp"
#include "s2/debug_screen_dc.hpp"
#endif
#include "platform/dc/screen.hpp"
#include <string.h>
#define SPRITE_NULL 0
#define SPRITE_ZERO
#define SPRITE_INIT =
#define SPRITE_ARRAY(type, name, count) type name[count]
#define SPRITE_DATA(name) (name)
#define SPRITE_MEMCPY ::memcpy
#define SPRITE_ALIGN(name) name
#define SPRITE_MODE(name) name
#include "auto_test.hpp"
#include "s2/title_sprite_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/hlib.hpp"
#include "s2/title_dc.hpp"
#include "s2/title_save_dc.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>
#include "s2/backup_hotplug_dc.hpp"
#include "s2/backup_dc.hpp"
#include "s2/backup_metadata_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/save_load_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/sound_dc.hpp"
extern "C" {
std::int32_t buGetFileSize(std::int32_t port, const char *filename);
std::int32_t buLoadFile(std::int32_t port, const char *filename, void *data, std::int32_t blocks);
}
#else
#include "s2/legacy_sprite.hpp"
#include "s2/legacy_sprite_slot.hpp"

#include "s2/d3t_renderer.hpp"
#include "s2/title_warning.hpp"
#include "s2/save.hpp"
#include "d3t/texture.hpp"
#include <bit>
#include "s2/pak.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/navi.hpp"
#include "s2/archive_data.hpp"
#include "s2/scene_data.hpp"
#include "s2/pause.hpp"
#include "s2/flag.hpp"
#include "s2/map_control.hpp"
#include "s2/particle_effects.hpp"
#include "s2/effect_texture.hpp"
#include "s2/weather.hpp"
#include "d3t/content.hpp"
#include "d3t/image_texture_loader.hpp"
#include "cJSON.h"
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
#include <limits>
#include <string>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::array<std::int32_t, 21> titleSpriteTextures{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

std::int32_t titleGTextTexture = -1, titleCTextTexture = -1, titleKeyImageTexture = -1, titleContinueTexture = -1, titleEsrbTexture = -1;

float legacySpriteSlotScaleX{}, legacySpriteSlotScaleY{}, legacySpriteSlotPulse{};

std::uint32_t legacySpriteSlotColour{}, legacySpriteSlotSecondaryColour{};

std::int32_t legacySpriteSlotWidthOffset{}, legacySpriteSlotHeightOffset{}, legacySpriteSlotRotation{};

const std::uint32_t *titleConfirmChoice{};

std::array<TempTextureEntry *, 39> g_tempTextures;

namespace {


TempTextureWork titleCommonTempWork{};

LegacySpritePacket titleSpritePacket{};

std::array<LegacyIconSprite, 4> itemWindowEdges;

LegacySpritePacket itemWindowTiles;

LegacySpritePacket itemWindowSeparators;

LegacyIconSprite itemWindowSelection;

std::vector<SDTextureMapping> sdTextureMappings;

const char *spriteTexturePath{};

std::array<std::int32_t, 5> collectionTextures{-1, -1, -1, -1, -1};

std::array<shen_name, 2> collectionSpriteOwners{};

std::array<LegacyIconSprite, 8> quantityDigits;

std::array<LegacyIconSprite, 2> quantityBrackets;

bool spriteAreaConfigurationsSelected{};

std::array<std::int32_t, 4> spriteBufferCapacities{};

std::array<std::uint32_t, 5> spriteWork{};

std::uint32_t spriteBufferWork{};

std::uint32_t spriteWorkInitialized{};

std::uint32_t unusedReloadWord{};

std::uint64_t unusedReloadWide0{};

std::uint64_t unusedReloadWide1{};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void InitializeSprites(std::int32_t useCurrentArea, std::int32_t initializeCommonTextures);
void AdjustScreenAspectRatio(
    LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, ScreenMode mode);
void TransformScreenSpace(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
    float scaleX, float scaleY, std::int32_t rotation, ScreenMode mode);
void PrepareDrawSimple(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode);
void PrepareDrawWithMode(const LegacyIconSprite &sprite, ScreenMode mode);
void PrepareDrawSimpleOpaque(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode);
void DrawLegacySelectionGlow(float x, float y, float depth, std::uint32_t &colour, bool &brightening, ScreenMode mode);
void DrawLegacyItemWindow(const Vector2 &position, float depth, std::int32_t columns, std::int32_t rows, ScreenMode mode);
void shenTransformScreenCoors(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode);
void PrepareDrawIndexedTextureWithSecondary(const LegacyIconSprite &sprite, std::uint32_t secondaryColour, ScreenMode mode);
void PrepareDrawTextureWithMode(const LegacyIconSprite &sprite, ScreenMode mode);
bool shenSubmitQuad(const LegacyColourQuad &packet, float depth);
void DrawLegacyWindowBackground(float x, float y, float width, float height, float depth, std::int32_t flags, ScreenMode mode);
void DrawLegacyWindowEdges(float x, float y, std::int32_t rows, std::int32_t columns, float depth, std::int32_t flags, ScreenMode mode);
void DrawLegacyWindowHorizontal(
    float x, float y, std::int32_t columns, std::int32_t edgeType, std::int32_t flipped, float depth, std::int32_t flags, ScreenMode mode);
void DrawLegacyWindowTiles(float x, float y, std::int32_t columns, std::int32_t rows, float depth, std::int32_t flags, ScreenMode mode);
bool vfSubmitQuad(std::uint32_t colour, float depth);
std::int64_t ResetLegacySpriteSlotDrawState();
void DrawTempSpriteSlot(std::uint32_t icon, std::uint32_t position, float depth, std::uint8_t flags);
const SDTextureMapping *GetSDTextureMappings(std::int32_t *count);
void ReplaceSDTextureImage(const char *mappedDestination, std::uint32_t textureHandle);
std::int64_t LoadLegacyCollectionSprites(shen_name owner);
void ReleaseLegacyCollectionSprites(shen_name owner);
const TempTextureWork &GetTempSpriteTexture(std::int32_t index, std::int32_t texture);
bool BindTempSprite(std::int32_t index, std::int32_t texture, std::uint16_t slot, const std::uint16_t *rectangles);
std::int32_t AllocateTempSprites(std::int32_t index);
void DrawLegacyItemQuantity(float x, float y, std::int32_t quantity, float depth);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void RetireImagePrefetch(void *, std::uint32_t handle, void *, std::uint64_t, const char *) {
    d3t::ProcessThreadStates(handle, 0);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ReplaceSpriteImage(void *data, std::uint32_t handle, void *bytes, std::uint64_t size, const char *) {
    d3t::ReplaceDecodedImage(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(data)), bytes, size, true);
    d3t::ProcessThreadStates(handle, 0);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SDTextureOverride(void *, std::uint32_t handle, void *bytes, std::uint64_t size, const char *) {
    if (bytes && size) {
        const std::string terminatedText(static_cast<const char *>(bytes), static_cast<std::size_t>(size));
        cJSON *const root = cJSON_Parse(terminatedText.c_str());
        if (root) {
            cJSON *const mappings = cJSON_GetObjectItem(root, "Mappings");
            const int count = cJSON_GetArraySize(mappings);
            sdTextureMappings.clear();
            sdTextureMappings.resize(static_cast<std::size_t>(count));
            for (int i = 0; i < count; ++i) {
                cJSON *const row = cJSON_GetArrayItem(mappings, i);
                cJSON *const source = cJSON_GetObjectItem(row, "source");
                cJSON *const destination = cJSON_GetObjectItem(row, "destination");
                if (!source || source->type != cJSON_String || !destination || destination->type != cJSON_String)
                    continue;
                SDTextureMapping &mapping = sdTextureMappings[static_cast<std::size_t>(i)];
                std::snprintf(mapping.source.data(), mapping.source.size(), "%s", source->valuestring);
                std::snprintf(mapping.destination.data(), mapping.destination.size(), "%s", destination->valuestring);
                FilepathCleaner(mapping.source.data());
                FilepathCleaner(mapping.destination.data());
                const std::size_t length = std::strlen(mapping.destination.data());
                if (length > 4 && std::strcmp(mapping.destination.data() + length - 4, ".png") == 0)
                    std::snprintf(mapping.destination.data() + length, mapping.destination.size() - length, "?usage=0");
                const d3t::D3TShaderLoadTask request{.path = mapping.destination.data(), .renderHandle = 0, .thread = {RetireImagePrefetch, nullptr}};
                d3t::PushShaderLoadToThreadQueue(&request);
            }
            cJSON_Delete(root);
        }
    }
    d3t::ProcessThreadStates(handle, 0);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

float SinScreenAngle(std::uint16_t angle) {
    const float halfAngle = ((static_cast<float>(angle) * 6.2831855f) * 0.000015258789f) * 0.5f;
    const std::int32_t quadrant = static_cast<std::int32_t>(halfAngle / 1.5707964f + 0.5f);
    const float reduced = halfAngle - static_cast<float>(quadrant) * 1.5707964f;
    float fraction = 0.0f;
    for (std::int32_t divisor = 11; divisor >= 3; divisor -= 2)
        fraction = (reduced * reduced) / (static_cast<float>(divisor) - fraction);
    const float tangent = reduced / (1.0f - fraction);
    const float result = (tangent + tangent) / (tangent * tangent + 1.0f);
    return (quadrant & 1) != 0 ? -result : result;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int64_t ResetUnusedReloadState() {
    unusedReloadWide0 = 0;
    unusedReloadWord = 0;
    unusedReloadWide1 = 0;
    return 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int64_t ReloadSpriteResources() {
    nullsub();
    DestroyEffectTextureReferences();
    InitializeSprites(1, 1);
    return ResetUnusedReloadState();
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SetSpriteAreaConfigurations() {
    spriteAreaConfigurationsSelected = true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint32_t InitializeSpriteBuffersNullsub(std::int32_t, std::int32_t, std::int32_t, std::int32_t) {
    return 0xffffffffU;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint32_t EnsureSpriteWorkInitialized() {
    if (!spriteWorkInitialized)
        spriteWorkInitialized = 1;
    return spriteWorkInitialized;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void InitializeSprites(std::int32_t useCurrentArea, std::int32_t initializeCommonTextures) {
    const MapRenderProfile &profile =
        spriteAreaConfigurationsSelected ? MatchMapRenderProfile(useCurrentArea ? g_currentMap : shen_name{}) : initialSpriteAreaConfiguration;
    nullsub(0x8cec0000U);
    spriteBufferCapacities[3] = static_cast<std::int32_t>(profile.field10);
    nullsub(static_cast<std::uint32_t>(spriteBufferCapacities[3] / 4));
    spriteBufferCapacities[2] = static_cast<std::int32_t>(profile.field0c);
    nullsub(static_cast<std::uint32_t>(spriteBufferCapacities[2] / 4));
    spriteBufferCapacities[0] = static_cast<std::int32_t>(profile.field04);
    spriteBufferCapacities[1] = static_cast<std::int32_t>(profile.field08);
    InitializeSpriteBuffersNullsub(0, spriteBufferCapacities[0] / 4, spriteBufferCapacities[1] / 4, 0);
    spriteWork[3] = spriteWork[4] = 0;
    spriteWork[1] = spriteWork[2] = 0;
    spriteWork[0] = 0;
    spriteBufferWork = 0;
    EnsureSpriteWorkInitialized();
    if (initializeCommonTextures) {
        PakLoadTaskEntry_Enqueue(3, 0);
        ReadPakFileFromDirectory("/MISC", "TEXTURES", 4);
        pakLoadFlush();
        PakGlobalData *const package = GetPakLoadTaskDataPointer();
        if (package) {
            constexpr shen_name binExtension{make_shen_name('B', 'I', 'N', ' ')};
            const std::span<const std::byte> ripple = FileLoaderView(package, "w_hamon.pvr", binExtension);
            if (!ripple.empty())
                ReadWeatherRipplePvrTexture(ripple);
            SetNamedTextureBank(2);
            LoadCommonIconSprites(package);
            LoadLoadScreenSpriteTexture(package);
            SetNamedTextureBank(0);
        }
        FreePakLoadTask();
    }
    nullsub();
    nullsub(16);
    SetLegacySavedAlphaOperation(profile.alphaOperation);
    SetTexUVW(0, profile.textureDistance);
    SetTexUVW(1, profile.textureDistanceW);
    SetWeatherMinimumSunElevation(profile.textureDistanceZ);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t TruncateSpriteCoordinate(float value) noexcept {
    return value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : std::numeric_limits<std::int32_t>::min();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef std::int32_t (*ItemWindowShape)(std::int32_t cell, std::int32_t columns);

void ReleaseNamedTexture_Wrapper(std::int32_t index);

void ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
#define TITLE_SAVE_AICA_COMMAND ExecuteAicaCommand
#else
#define TITLE_SAVE_AICA_COMMAND shenExecuteAICACommand
#endif

void LoadTitleSoundProgram_Wrapper(std::int32_t sceneIndex);

void ImportTitleSaveSlot(const void *bank, std::int32_t slot);

template <typename T, typename U> T SpriteBits(const U &value);

std::int32_t GetRoundedItemWindowShape(std::int32_t cell, std::int32_t columns);

std::int32_t GetStraightItemWindowShape(std::int32_t cell, std::int32_t columns);

void DrawLegacyColourPacket(const LegacySpritePacket &packet, float depth);

void DrawLegacySpritePacket(const LegacySpritePacket &packet, std::uint32_t texture, float depth);

void SetLegacySpriteSlotScale(float x, float y);

void SetLegacySpriteSlotSizeOffsets(std::int32_t width, std::int32_t height);

void SetLegacySpriteSlotRotation(std::int32_t angle);

void SetLegacySpriteSlotColour(std::uint32_t colour);

void SetLegacySpriteSlotSecondaryColour(std::uint32_t colour);

std::int32_t GetCommonIconTextureIndex(std::uint32_t icon);

std::uint16_t GetCommonIconTextureHandle(std::uint16_t icon);

std::int32_t FindLegacyCollectionSpriteSlot(std::int32_t owner);

std::int32_t HasLegacyCollectionSpriteSlot();

void PrepareDrawSimple(const LegacyIconSprite &sprite, std::uint32_t colour);

void PrepareDrawWithMode(const LegacyIconSprite &sprite);

void AdjustScreenAspectRatio(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height);

void TransformScreenSpace(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
    float scaleX, float scaleY, std::int32_t rotation);

float ConvertLegacyScreenDepth(float depth);

std::uint32_t GetLegacySpriteSlotPulse();

std::int32_t AllocateTempSprites(std::int32_t index);

bool BindTempSprite(std::int32_t index, std::int32_t texture, std::int32_t slot, const std::uint16_t *rectangles);

void PrepareDrawSimpleOpaque(const LegacyIconSprite &sprite, std::uint32_t colour);

void PrepareDrawSimpleOpaque(const LegacyIconSprite &sprite, std::uint32_t colour);

#define SPRITE_BITS(type, value) SpriteBits<type>(value)

std::int32_t g_commonIconTextures[14] SPRITE_ZERO;

SPRITE_ARRAY(TempTextureEntry *, g_tempTextures, 38) SPRITE_ZERO;

TempTextureWork titleCommonTempWork SPRITE_ZERO;

float legacySpriteSlotScaleX = 1.0f, legacySpriteSlotScaleY = 1.0f, legacySpriteSlotPulse = 0.0f;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float legacySpriteSlotPulseStep = 0.0f;
#endif

std::int32_t legacySpriteSlotWidthOffset = 0, legacySpriteSlotHeightOffset = 0, legacySpriteSlotRotation = 0;

std::uint32_t legacySpriteSlotColour = UINT32_MAX, legacySpriteSlotSecondaryColour = 0;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
LegacyIconSprite itemWindowEdges[4] SPRITE_ZERO;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
LegacySpritePacket itemWindowTiles SPRITE_ZERO;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
LegacySpritePacket itemWindowSeparators SPRITE_ZERO;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
LegacyIconSprite itemWindowSelection SPRITE_ZERO;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
LegacyIconSprite quantityDigits[8] SPRITE_ZERO;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
LegacyIconSprite quantityBrackets[2] SPRITE_ZERO;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
const float itemWindowEdgeOffsets[2] = {11.0f, -11.0f};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern const std::uint16_t itemQuantityDigitIcons[10] = {1030, 1031, 1032, 1033, 1034, 1035, 1036, 1037, 1038, 1039};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
const ItemWindowShape itemWindowShapes[2] = {GetStraightItemWindowShape, GetRoundedItemWindowShape};
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

template <typename T, typename U> T SpriteBits(const U &value) {
    T result;
    ::memcpy(&result, &value, sizeof(result));
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static const std::uint8_t *const kCommonIconBankRectangles[14] = {kCommonIconRectangles + 0x104, kCommonIconRectangles + 0x204, kCommonIconRectangles + 0x204,
    kCommonIconRectangles + 0x204, kCommonIconRectangles, kCommonIconRectangles + 0x7c, kCommonIconRectangles + 0xbc, kCommonIconRectangles + 0x244,
    kCommonIconRectangles + 0x244, kCommonIconRectangles + 0x104, kCommonIconRectangles + 0x104, kCommonIconRectangles + 0x104, kCommonIconRectangles + 0x104,
    kCommonIconRectangles + 0x204};

const std::int8_t *GetCommonIconRect(std::int32_t icon) {
    const std::int32_t bank = (icon >> 8) & 255;
    const std::int32_t index = icon & 255;
    const std::uint8_t *rect;
    if (bank > 13)
        rect = kCommonIconBankRectangles[0];
    else
        rect = kCommonIconBankRectangles[bank] + index * 4;
    return reinterpret_cast<const std::int8_t *>(rect);
}

void CommonIconRectNullsub() {
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetLegacyIconSpritePosition(LegacyIconSprite &sprite, float x, float y) {
    sprite.x = x;
    sprite.y = y;
}

void SetLegacyIconSpriteScaleAndSize(LegacyIconSprite &sprite, float scaleX, float scaleY, float width, float height) {
    sprite.scaleX = scaleX;
    sprite.scaleY = scaleY;
    sprite.width = static_cast<std::int16_t>(width);
    sprite.height = static_cast<std::int16_t>(height);
}

void SetLegacyIconSpriteState(LegacyIconSprite &sprite, std::uint32_t rotation, std::uint8_t alignment, float depth, std::uint16_t flags) {
    sprite.rotation = rotation;
    sprite.alignment = alignment;
    sprite.depth = depth;
    sprite.flags = flags;
}

void SetLegacyIconSpriteIcon(LegacyIconSprite &sprite, std::uint16_t icon) {
    sprite.icon = icon;
}

// @verified - d
void InitializeLegacyIconSprite(LegacyIconSprite &sprite) {
    sprite.width = 32;
    sprite.height = 32;
    sprite.scaleX = 1.0f;
    sprite.scaleY = 1.0f;
    sprite.depth = 1.0f;
    sprite.alignment = 5;
    sprite.rotation = 0;
    sprite.flags = 0;
    sprite.x = 0.0f;
    sprite.y = 0.0f;
    sprite.icon = 0;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t GetRoundedItemWindowShape(std::int32_t cell, std::int32_t columns) {
    if (!cell)
        return 0;
    std::int32_t shape = 2;
    if (cell != columns)
        shape = 1;
    return shape;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t GetStraightItemWindowShape(std::int32_t cell, std::int32_t columns) {
    return !cell || cell == columns ? 3 : 1;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawLegacyWindowHorizontal(float x, float y, std::int32_t columns, std::int32_t edgeType, std::int32_t flipped, float depth, std::int32_t flags) {
    float cursor = x + 3.0f;
    const ItemWindowShape shapeForCell = itemWindowShapes[edgeType];
    const float verticalOffset = itemWindowEdgeOffsets[flipped];
    const float edgeDepth = depth + ((flags & 0x1000) ? 0.001f : 1.0f);
    for (std::int32_t index = 0; index <= columns; ++index) {
        const std::int32_t cell = edgeType && flipped ? columns - index : index;
        const std::int32_t shape = shapeForCell(cell, columns);
        LegacyIconSprite sprite = itemWindowEdges[shape];
        sprite.depth = depth;
        const float halfWidth = static_cast<float>(sprite.width) * 0.5f;
        cursor += halfWidth;
        sprite.y = y;
        sprite.x = cursor;
        sprite.flags = static_cast<std::uint16_t>(flags);
        if (edgeType) {
            if (flipped)
                sprite.flags = static_cast<std::uint16_t>(flags | 0x300);
            if (shape == 0 || shape == 2)
                sprite.y += verticalOffset;
        } else if (index == 0) {
            sprite.flags |= 0x300U;
        }
        if (index == 0 || index == columns) {
            sprite.depth = edgeDepth;
            sprite.x = index == 0 ? cursor + 3.0f : cursor - 3.0f;
        } else {
            sprite.x = cursor;
        }
        PrepareDrawWithMode(sprite);
        cursor += halfWidth;
    }
}
#endif

#else

void DrawLegacyWindowHorizontal(
    float x, float y, std::int32_t columns, std::int32_t edgeType, std::int32_t flipped, float depth, std::int32_t flags, ScreenMode mode) {
    float cursor = x + 3.0f;
    const float edgeDepth = depth + ((flags & 0x1000) ? 0.001f : 1.0f);
    for (std::int32_t i = 0; i <= columns; ++i) {
        const std::int32_t cell = edgeType && flipped ? columns - i : i;
        const std::size_t shape = edgeType ? (cell == 0 ? 0U : cell == columns ? 2U : 1U) : (cell == 0 || cell == columns ? 3U : 1U);
        LegacyIconSprite sprite = itemWindowEdges[shape];
        sprite.depth = depth;
        sprite.flags = static_cast<std::uint16_t>(flags);
        sprite.y = y;
        const float halfWidth = static_cast<float>(sprite.width) * 0.5f;
        cursor += halfWidth;
        if (edgeType) {
            if (flipped)
                sprite.flags |= 0x300U;
            if ((shape & ~2U) == 0)
                sprite.y += itemWindowEdgeOffsets[static_cast<std::size_t>(flipped)];
        } else if (i == 0) {
            sprite.flags |= 0x300U;
        }
        if (i == 0 || i == columns) {
            sprite.depth = edgeDepth;
            sprite.x = cursor + (i ? -3.0f : 3.0f);
        } else {
            sprite.x = cursor;
        }
        PrepareDrawWithMode(sprite, mode);
        cursor += halfWidth;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawLegacyWindowTiles(float x, float y, std::int32_t columns, std::int32_t rows, float depth, std::int32_t flags) {
    std::int32_t column;
    float left = x;
    const std::int32_t texture = GetCommonIconTextureHandle(1547);
    itemWindowTiles.quad.flags = static_cast<std::uint32_t>(flags) | 0x400U;
    for (column = columns; column != 0; --column) {
        float row = y;
        for (std::int32_t remaining = rows; remaining != 0; --remaining) {
            AdjustScreenAspectRatio(
                itemWindowTiles.quad, static_cast<ScreenAlignment>(5), static_cast<std::int32_t>(left), static_cast<std::int32_t>(row), 40, 40);
            DrawLegacySpritePacket(itemWindowTiles, static_cast<std::uint32_t>(texture), depth);
            row += 40.0f;
        }
        left += 40.0f;
    }
}
#endif

#else

void DrawLegacyWindowTiles(float x, float y, std::int32_t columns, std::int32_t rows, float depth, std::int32_t flags, ScreenMode mode) {
    const CommonIconAtlas *const atlas = GetCommonIconAtlas(1547);
    const std::uint32_t texture = atlas ? atlas->textureHandle : 0;
    itemWindowTiles.quad.flags = static_cast<std::uint32_t>(flags) | 0x400U;
    for (; columns != 0; --columns, x += 40.0f) {
        float row = y;
        for (std::int32_t i = rows; i != 0; --i, row += 40.0f) {
            TransformScreenSpace(
                itemWindowTiles.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(row), 40, 40, 1, 1, 0, mode);
            LegacySpriteNullsub(itemWindowTiles, texture, depth);
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
// @verified - d
void DrawLegacyWindowBackground(float x, float y, float width, float height, float depth, std::int32_t flags) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 8U;
    packet.colour = 0x05000020U;
    std::uint32_t *const words = reinterpret_cast<std::uint32_t *>(&packet);
    const std::uint32_t alternateColourWord = offsetof(LegacySpritePacket, reservedColour) / sizeof(std::uint32_t);
    words[alternateColourWord] = 0x05000020U;
    words[alternateColourWord + 1] = 0xbf000000U;
    words[alternateColourWord + 2] = 0xbf000000U;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), static_cast<std::int32_t>(x), static_cast<std::int32_t>(y),
        static_cast<std::int32_t>(width), static_cast<std::int32_t>(height));
    packet.quad.flags |= static_cast<std::uint32_t>(flags) | 0x400U;
    DrawLegacyColourPacket(packet, depth);
}
#endif

#else

void DrawLegacyWindowBackground(float x, float y, float width, float height, float depth, std::int32_t flags, ScreenMode mode) {
    LegacyColourQuad packet{.quad = {.flags = 8}, .colour = 0x05000020U, .alternateColours = {0x05000020U, 0xbe000000U, 0xbe000000U}};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(0), TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(y),
        TruncateSpriteCoordinate(width), TruncateSpriteCoordinate(height), 1, 1, 0, mode);
    // The PC background leaf constructs this packet but submits nothing.
    static_cast<void>(depth);
    static_cast<void>(flags);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool LoadLegacyCollectionSprites(shen_name owner) {
    bool result = false;
    std::int32_t slot = FindLegacyCollectionSpriteSlot(owner);
    if (slot < 0) {
        slot = FindLegacyCollectionSpriteSlot(0);
        if (slot >= 0)
            collectionSpriteOwners[slot] = owner;
    }
    if (g_commonIconTextures[9] >= 0)
        return true;
    void *const texture = shenLoadFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "COM_CL.SPR", SHEN_NAME(ICCL));
    if (texture) {
        const std::uint32_t bytes = shenOpenFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "COM_CL.SPR");
        if (bytes) {
            ReadTitleSpriteTextures(texture, bytes, &g_commonIconTextures[9]);
            result = true;
        }
        HLib::FreeTaskData(texture);
    }
    return result;
}
#endif
#endif

#else

std::int64_t LoadLegacyCollectionSprites(shen_name owner) {
    bool registered{};
    for (const shen_name token : collectionSpriteOwners)
        registered |= token.name == owner.name;
    if (!registered)
        for (shen_name &token : collectionSpriteOwners)
            if (!token.name) {
                token = owner;
                break;
            }
    if (collectionTextures[0] >= 0)
        return 1;
    void *allocation{};
    std::uint32_t size{};
    allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "COM_CL.SPR", COLLECTION_SPRITE_FILE, &size);
    if (!allocation)
        return 0;
    const auto *cursor = static_cast<const std::byte *>(allocation);
    const auto *const end = cursor + size;
    std::size_t index{};
    while (cursor < end && index < collectionTextures.size() && end - cursor >= 8) {
        std::uint32_t tag{}, chunkSize{};
        std::memcpy(&tag, cursor, 4);
        std::memcpy(&chunkSize, cursor + 4, 4);
        if (!tag || chunkSize < 8 || chunkSize > static_cast<std::size_t>(end - cursor))
            break;
        collectionTextures[index++] = static_cast<std::int32_t>(ReadTexnTexture(cursor));
        cursor += chunkSize;
    }
    HLib::FreeTaskData(allocation);
    return size ? 1 : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ReleaseLegacyCollectionSprites(shen_name owner) {
    if (owner) {
        const std::int32_t slot = FindLegacyCollectionSpriteSlot(owner);
        if (slot >= 0) {
            collectionSpriteOwners[slot] = 0;
            if (!HasLegacyCollectionSpriteSlot())
                for (std::int32_t index = 9; index < 14; ++index) {
                    if (g_commonIconTextures[index] != -1)
                        ReleaseNamedTexture_Wrapper(g_commonIconTextures[index]);
                    g_commonIconTextures[index] = -1;
                }
        }
    }
}
#endif
#endif

#else

void ReleaseLegacyCollectionSprites(shen_name owner) {
    if (!owner.name)
        return;
    bool registered{};
    for (shen_name &token : collectionSpriteOwners)
        if (token.name == owner.name) {
            token = shen_name{};
            registered = true;
            break;
        }
    if (!registered)
        return;
    for (const shen_name token : collectionSpriteOwners)
        if (token.name)
            return;
    for (std::int32_t &texture : collectionTextures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void FreeTempSprites(std::int32_t index) {
    TempTextureEntry *&entries = g_tempTextures[index];
    if (!entries)
        return;
    for (std::int64_t texture = TEMP_TEXTURE_DEFINITIONS[index].textureCount - 1; texture >= 0; --texture)
        ReleaseNamedTexture(entries[texture].texture);
    HLib::FreeTaskData(entries);
    entries = nullptr;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawLegacyWindowEdges(float x, float y, std::int32_t rows, std::int32_t columns, float depth, std::int32_t flags) {
    std::int32_t remaining;
    const float right = static_cast<float>(columns * 40 + 8);
    float row = 20.0f;
    const std::uint32_t texture = GetCommonIconTextureHandle(1026);
    itemWindowSeparators.quad.flags = static_cast<std::uint32_t>(flags) | 0x400U;
    for (remaining = rows; remaining != 0; --remaining) {
        TransformScreenSpace(
            itemWindowSeparators.quad, HorizontalStretch, static_cast<std::int32_t>(x), static_cast<std::int32_t>(y + row), 40, 16, 1, 1, 0x4000);
        DrawLegacySpritePacket(itemWindowSeparators, texture, depth);
        itemWindowSeparators.quad.flags |= static_cast<std::uint32_t>(flags) | 0x300U;
        TransformScreenSpace(
            itemWindowSeparators.quad, HorizontalStretch, static_cast<std::int32_t>(right + x), static_cast<std::int32_t>(y + row), 40, 16, 1, 1, 0x4000);
        DrawLegacySpritePacket(itemWindowSeparators, texture, depth);
        row += 40.0f;
    }
}
#endif
#endif

#else

void DrawLegacyWindowEdges(float x, float y, std::int32_t rows, std::int32_t columns, float depth, std::int32_t flags, ScreenMode mode) {
    const CommonIconAtlas *const atlas = GetCommonIconAtlas(1026);
    const std::uint32_t texture = atlas ? atlas->textureHandle : 0;
    itemWindowSeparators.quad.flags = static_cast<std::uint32_t>(flags) | 0x400U;
    const float right = static_cast<float>(columns) * 40.0f + 8.0f;
    for (float row = 20.0f; rows != 0; --rows, row += 40.0f) {
        TransformScreenSpace(itemWindowSeparators.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(row + y), 40,
            16, 1, 1, 0x4000, mode);
        LegacySpriteNullsub(itemWindowSeparators, texture, depth);
        itemWindowSeparators.quad.flags |= static_cast<std::uint32_t>(flags) | 0x300U;
        TransformScreenSpace(itemWindowSeparators.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(right + x),
            TruncateSpriteCoordinate(row + y), 40, 16, 1, 1, 0x4000, mode);
        LegacySpriteNullsub(itemWindowSeparators, texture, depth);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawLegacyItemWindow(const Vector2 &position, float depth, std::int32_t columns, std::int32_t rows) {
    const float columnCount = static_cast<float>(columns), rowCount = static_cast<float>(rows);
    const float width = columnCount * 40.0f, height = rowCount * 40.0f;
    const float x = position.x - width * 0.5f, y = position.y - height * 0.5f;
    DrawLegacyWindowBackground(x, y, width, height, depth + 0.0001f, 0x1000);
    DrawLegacyWindowTiles(x + 20.0f, y + 20.0f, columns, rows, depth + 0.0002f, 0x1000);
    DrawLegacyWindowEdges(x - 4.0f, y + 23.0f, rows - 1, columns, depth, 0x1000);
    DrawLegacyWindowHorizontal(x - 15.0f, y - 4.0f, columns, 1, 0, depth, 0x1000);
    DrawLegacyWindowHorizontal(x - 15.0f, (y + height) + 4.0f, columns, 1, 1, depth, 0x1000);
}
#endif
#endif

#else

void DrawLegacyItemWindow(const Vector2 &position, float depth, std::int32_t columns, std::int32_t rows, ScreenMode mode) {
    const float width = static_cast<float>(columns) * 40.0f, height = static_cast<float>(rows) * 40.0f;
    const float x = position.x - width * 0.5f, y = position.y - height * 0.5f;
    DrawLegacyWindowBackground(x, y, width, height, depth - 0.0099999998f, 0, mode);
    DrawLegacyWindowTiles(x + 20.0f, y + 20.0f, columns, rows, depth - 0.02f, 0, mode);
    DrawLegacyWindowEdges(x - 4.0f, y + 23.0f, rows - 1, columns, depth, 0, mode);
    DrawLegacyWindowHorizontal(x - 15.0f, y - 4.0f, columns, 1, 0, depth, 0, mode);
    DrawLegacyWindowHorizontal(x - 15.0f, (y + height) + 4.0f, columns, 1, 1, depth, 0, mode);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void PrepareDrawIndexedTexture(const LegacyIconSprite &sprite, ScreenMode mode) {
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = 0xffffffffU, .secondaryColour = 0, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    packet.quad.flags |= sprite.flags;
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int16_t>(sprite.icon)), sprite.depth);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateLegacySelectionGlow(std::uint32_t &colour, bool &brightening) {
    if (brightening) {
        colour += 0x05050505U;
        if (colour > 0x6f6f6f6eU) {
            brightening = false;
            colour = 0x6f6f6f6fU;
        }
    } else {
        colour += 0xfafafafbU;
        if (colour < 0x20202021U) {
            brightening = true;
            colour = 0x20202020U;
        }
    }
}
#endif
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawLegacySelectionGlow(const Vector2 &position, std::uint32_t &colour, bool &brightening, float depth) {
    itemWindowSelection.x = position.x;
    itemWindowSelection.y = position.y;
    itemWindowSelection.depth = depth;
    PrepareDrawSimpleOpaque(itemWindowSelection, colour & 0xffffffU);
    if (!shenIsGamePaused())
        UpdateLegacySelectionGlow(colour, brightening);
}
#endif
#endif

#else

void DrawLegacySelectionGlow(float x, float y, float depth, std::uint32_t &colour, bool &brightening, ScreenMode mode) {
    itemWindowSelection.x = x;
    itemWindowSelection.y = y;
    itemWindowSelection.depth = depth;
    PrepareDrawSimpleOpaque(itemWindowSelection, colour & 0xffffffU, mode);
    if (shenIsGamePaused())
        return;
    if (brightening) {
        colour += 0x05050505U;
        if (colour >= 0x6f6f6f6fU) {
            brightening = false;
            colour = 0x6f6f6f6fU;
        }
    } else {
        colour -= 0x05050505U;
        if (colour <= 0x20202020U) {
            brightening = true;
            colour = 0x20202020U;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawMoneyDigits(float x, float y, float scale, float depth, std::int32_t amount, std::uint32_t colour) {
    bool significant = false;
    const std::int16_t advance = static_cast<std::int16_t>(16.0f * scale);
    float digitX = x - static_cast<float>(advance * 7);
    LegacyIconSprite *sprite = quantityDigits;
    for (std::int32_t remaining = 8; remaining != 0; --remaining, ++sprite) {
        sprite->x = digitX;
        sprite->y = y;
        sprite->depth = depth;
        sprite->scaleX = scale;
        sprite->scaleY = scale;
        const std::int32_t increment = moneyIconIncrements[8 - remaining];
        const std::int32_t digit = amount / increment;
        amount -= digit * increment;
        if (digit != 0)
            significant = true;
        sprite->icon = itemQuantityDigitIcons[digit];
        digitX += static_cast<float>(advance);
        if (significant || remaining == 1)
            PrepareDrawSimple(*sprite, colour);
    }
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawLegacyItemQuantity(float x, float y, std::int32_t quantity, float depth) {
    float left = (x - 128.0f) - 16.0f;
    const std::int32_t *increment = moneyIconIncrements;
    LegacyIconSprite *bracket = quantityBrackets;
    if (quantity > 0) {
        std::int32_t remaining = 8;
        if (quantity < *increment++) {
            do {
                left += 16.0f;
            } while (--remaining && quantity < *increment++);
        }
    } else {
        left += 112.0f;
    }
    bracket[0].x = x;
    bracket[0].y = y;
    bracket[1].x = left;
    bracket[1].y = y;
    DrawMoneyDigits(x - 16.0f, y, 1.0f, depth, quantity, 0xff303030U);
    std::int32_t count = 2;
    do {
        bracket->depth = depth;
        PrepareDrawSimple(*bracket++, 0xff303030U);
    } while (--count);
}
#endif

#else

void DrawLegacyItemQuantity(float x, float y, std::int32_t quantity, float depth) {
    float left = (x - 128.0f) - 16.0f;
    if (quantity <= 0)
        left += 112.0f;
    else
        for (const auto increment : moneyIconIncrements) {
            if (quantity >= increment)
                break;
            left += 16.0f;
        }
    quantityBrackets[0].x = x;
    quantityBrackets[0].y = y;
    quantityBrackets[1].x = left;
    quantityBrackets[1].y = y;
    float digitX = (x - 16.0f) - 112.0f;
    bool significant{};
    for (std::size_t i = 0; i < quantityDigits.size(); ++i) {
        LegacyIconSprite &sprite = quantityDigits[i];
        const auto digit = quantity / moneyIconIncrements[i];
        quantity %= moneyIconIncrements[i];
        sprite.x = digitX;
        sprite.y = y;
        sprite.depth = depth;
        sprite.scaleX = sprite.scaleY = 1;
        sprite.icon = static_cast<std::uint16_t>(1030 + digit);
        digitX += 16;
        significant |= digit != 0;
        if (significant || i == quantityDigits.size() - 1)
            PrepareDrawSimple(sprite, 0xff303030U, ScreenMode::Centered);
    }
    for (LegacyIconSprite &sprite : quantityBrackets) {
        sprite.depth = depth;
        PrepareDrawSimple(sprite, 0xff303030U, ScreenMode::Centered);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetLegacySpriteSlotDrawState() {
    SetLegacySpriteSlotScale(1.0f, 1.0f);
    SetLegacySpriteSlotSizeOffsets(0, 0);
    SetLegacySpriteSlotRotation(0);
    SetLegacySpriteSlotColour(UINT32_MAX);
    SetLegacySpriteSlotSecondaryColour(0);
}

#else

// @verified - p
std::int64_t ResetLegacySpriteSlotDrawState() {
    legacySpriteSlotScaleX = 1.0f;
    legacySpriteSlotWidthOffset = 0;
    legacySpriteSlotHeightOffset = 0;
    legacySpriteSlotRotation = 0;
    legacySpriteSlotSecondaryColour = 0;
    legacySpriteSlotScaleY = 1.0f;
    legacySpriteSlotColour = 0xffffffffU;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetTempTextureReferences() {
    for (std::int32_t index = 0; index < 38; ++index)
        g_tempTextures[index] = SPRITE_NULL;
    for (std::int32_t index = 0; index < 8; ++index)
        borrowedSpriteSlots[index].rectangles = SPRITE_NULL;
}

void ReleaseTempTextures() {
    for (std::int32_t index = 0; index < 38; ++index) {
        TempTextureEntry *&entries = g_tempTextures[index];
        if (entries) {
            HLib::FreeTaskData(entries);
            entries = SPRITE_NULL;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t FindBorrowedSpriteSlot(std::int32_t slot);

const TempTextureWork &GetBorrowedSpriteTexture(std::int32_t index, std::int32_t texture) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    TempTextureWork *work = &titleCommonTempWork;
    const BorrowedLegacySpriteSlot *slot = 0;
    const std::int32_t found = FindBorrowedSpriteSlot(index);
    if (found >= 0)
        slot = &borrowedSpriteSlots[found];
    if (!slot) {
        work->width = 0;
        work->height = 0;
        work->texture = -1;
        work->flags = 0;
        return *work;
    }
    const std::int32_t offset = static_cast<std::int32_t>(static_cast<std::uint32_t>(texture) * 4U);
    const std::uint16_t *const rectangles = slot->rectangles;
    if (slot->texture >= 0) {
        const std::uint16_t *const rect = rectangles + offset;
        work->width = rect[2];
        work->height = rect[3];
        work->texture = slot->texture;
        work->uv[0] = rectangles[offset] * slot->inverseWidth;
        work->uv[1] = 1.0f - ((rect[1] + rect[3]) * slot->inverseHeight);
        work->uv[2] = (rectangles[offset] + rect[2]) * slot->inverseWidth;
        work->uv[3] = 1.0f - (rect[1] * slot->inverseHeight);
        work->flags = TTF_Loaded | TTF_Region;
    } else {
        work->width = 0;
        work->height = 0;
        work->texture = -1;
        work->flags = 0;
    }
    return *work;
#else
    const std::int32_t found = FindBorrowedSpriteSlot(index);
    const BorrowedLegacySpriteSlot *slot = found >= 0 ? &borrowedSpriteSlots[found] : SPRITE_NULL;
    if (!slot || slot->texture < 0) {
        titleCommonTempWork.width = 0;
        titleCommonTempWork.height = 0;
        titleCommonTempWork.texture = -1;
        titleCommonTempWork.flags = 0;
        return titleCommonTempWork;
    }
    const std::uint16_t *const rect = slot->rectangles + texture * 4;
    titleCommonTempWork.width = rect[2];
    titleCommonTempWork.height = rect[3];
    titleCommonTempWork.texture = slot->texture;
    titleCommonTempWork.uv[0] = rect[0] * slot->inverseWidth;
    titleCommonTempWork.uv[1] = 1.0f - ((rect[1] + rect[3]) * slot->inverseHeight);
    titleCommonTempWork.uv[2] = (rect[0] + rect[2]) * slot->inverseWidth;
    titleCommonTempWork.uv[3] = 1.0f - (rect[1] * slot->inverseHeight);
    titleCommonTempWork.flags = TTF_Loaded | TTF_Region;
    return titleCommonTempWork;
#endif
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
const TempTextureWork &GetTempSpriteTexture(std::int32_t index, std::int32_t texture) {
    TempTextureWork *work = &titleCommonTempWork;
    if (index > 30) {
        work = const_cast<TempTextureWork *>(&GetBorrowedSpriteTexture(index, texture));
        if (work->flags & TTF_Loaded)
            return *work;
    }
    TempTextureEntry *entries = g_tempTextures[index];
    if (entries) {
        work->width = entries[texture].width;
        work->height = entries[texture].height;
        work->texture = static_cast<std::int16_t>(entries[texture].texture);
        work->flags = TTF_Loaded;
    } else {
        work->width = 0;
        work->height = 0;
        work->texture = -1;
        work->flags = 0;
    }
    return *work;
}

// @verified - d
std::int32_t FindBorrowedSpriteSlot(std::int32_t slot) {
    std::int32_t found = -1;
    for (std::int32_t index = 0; index < 8; ++index) {
        if (borrowedSpriteSlots[index].rectangles && static_cast<std::int16_t>(borrowedSpriteSlots[index].slot) == slot) {
            found = index;
            break;
        }
    }
    return found;
}

#else

// @verified - p
const TempTextureWork &GetTempSpriteTexture(std::int32_t index, std::int32_t texture) {
    if (index >= 32) {
        const BorrowedLegacySpriteSlot *slot = nullptr;
        for (const BorrowedLegacySpriteSlot &entry : borrowedSpriteSlots) {
            if (entry.rectangles && entry.slot == index) {
                slot = &entry;
                break;
            }
        }
        if (slot && slot->texture >= 0) {
            const std::uint16_t *const rect = slot->rectangles + 4 * texture;
            titleCommonTempWork.width = rect[2];
            titleCommonTempWork.height = rect[3];
            titleCommonTempWork.texture = slot->texture;
            titleCommonTempWork.uv = {rect[0] * slot->inverseWidth, 1.0f - ((rect[1] + rect[3]) * slot->inverseHeight),
                (rect[0] + rect[2]) * slot->inverseWidth, 1.0f - (rect[1] * slot->inverseHeight)};
            titleCommonTempWork.flags = TTF_Loaded | TTF_Region;
            return titleCommonTempWork;
        }
        titleCommonTempWork.width = titleCommonTempWork.height = 0;
        titleCommonTempWork.texture = -1;
        titleCommonTempWork.flags = 0;
    }
    if (g_tempTextures[index]) {
        const TempTextureEntry &entry = g_tempTextures[index][texture];
        titleCommonTempWork.width = entry.width;
        titleCommonTempWork.height = entry.height;
        titleCommonTempWork.texture = static_cast<std::int16_t>(entry.texture);
        titleCommonTempWork.flags = TTF_Loaded;
    } else {
        titleCommonTempWork.width = titleCommonTempWork.height = 0;
        titleCommonTempWork.texture = -1;
        titleCommonTempWork.flags = 0;
    }
    return titleCommonTempWork;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
bool BindTempSprite(std::int32_t index, std::int32_t texture, std::uint16_t slot, const std::uint16_t *rectangles) {
    if (!g_tempTextures[index])
        return false;
    const TempTextureWork &work = GetTempSpriteTexture(index, texture);
    return RegisterBorrowedLegacySpriteSlot(work.texture, work.width, work.height, slot, rectangles);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t ReadTempTextureEntries(const void *source, TempTextureEntry *entries) {
    const std::byte *bytes = static_cast<const std::byte *>(source);
    std::uint32_t signature;
    std::memcpy(&signature, bytes, 4);
    std::int32_t count = 0;
    while (signature == SHEN_NAME(TEXN)) {
        entries[count].texture = static_cast<std::int32_t>(ReadTexnTexture(bytes));
        std::uint32_t length;
        std::memcpy(&entries[count].width, bytes + 40, 2);
        std::memcpy(&entries[count].height, bytes + 42, 2);
        std::memcpy(&length, bytes + 4, 4);
        ++count;
        bytes += length;
        std::memcpy(&signature, bytes, 4);
    }
    return count;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int32_t AllocateTempSprites(std::int32_t index) {
    if (g_tempTextures[index])
        FreeTempSprites(index);
    TempTextureEntry *&entries = g_tempTextures[index];
    entries = static_cast<TempTextureEntry *>(HLib::AllocateTaskData(8LL * TEMP_TEXTURE_DEFINITIONS[index].textureCount, TITLE_COMMON_TEMP_TASK));
    if (!entries)
        return 0;
    char file[32]{};
    const char *const source = TEMP_TEXTURE_DEFINITIONS[index].file;
    if (std::strstr(source, ".GZ"))
        std::strcpy(file, source);
    else
        std::snprintf(file, sizeof(file), "VM_%s.GZ", source);
    void *allocation = nullptr;
    std::size_t size = 0;
    LoadLegacySprite("sprite", file, TITLE_COMMON_TEMP_TASK, allocation, size);
    if (ReadTempTextureEntries(allocation, entries) <= 0) {
        HLib::FreeTaskData(entries);
        entries = nullptr;
        return 0;
    }
    HLib::FreeTaskData(allocation);
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int32_t InitializeTitleCommonTempSprites() {
    if (AllocateTempSprites(0) != 1)
        return 0;
    BindTempSprite(0, 0, 36, TITLE_COMMON_TEMP_RECTANGLES.data());
    return BindTempSprite(0, 1, 37, TITLE_COMMON_TEMP_RECTANGLES.data()) ? 1 : 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Native InitWarnLogos: the title module drops the warning logo, the shots images and the scene effect textures. Its 25-slot
// TITLE.GZ reset runs through the four image handles that follow the 21 sprite slots.
void InitWarnLogos() {
    SetTitleWarningLogo(0, nullptr);
    FreeTitleWarningResources();
    for (const std::int32_t texture : {titleGTextTexture, titleCTextTexture, titleKeyImageTexture, titleContinueTexture})
        if (static_cast<std::uint32_t>(texture + 1) > 1U)
            d3t::InitTexture2DResource(static_cast<std::uint32_t>(texture));
    InitSceneEffects();
    titleEsrbTexture = -1;
    titleSpriteTextures.fill(-1);
    titleGTextTexture = titleCTextTexture = titleKeyImageTexture = titleContinueTexture = -1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void InitShotsImages() {
    InitializeTitleCommonTempSprites();
    void *allocation = nullptr;
    std::size_t size = 0;
    LoadLegacySprite(Filepath_Gen(FilepathGenMode::SpriteDirectory), "TITLE.GZ", TITLE_SPRITE_FILE, allocation, size);
    if (allocation) {
        if (size) {
            const std::byte *bytes = static_cast<const std::byte *>(allocation);
            const std::byte *const end = bytes + size;
            std::int32_t *textures = titleSpriteTextures.data();
            while (bytes < end) {
                std::uint32_t signature, length;
                std::memcpy(&signature, bytes, 4);
                if (!signature)
                    break;
                *textures++ = static_cast<std::int32_t>(ReadTexnTexture(bytes));
                std::memcpy(&length, bytes + 4, 4);
                bytes += length;
            }
        }
        HLib::FreeTaskData(allocation);
    }
    std::uint32_t fileSize = 0;
    allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "GText.png", TITLE_GTEXT_FILE, &fileSize);
    if (allocation) {
        if (fileSize)
            titleGTextTexture = static_cast<std::int32_t>(d3t::CreateImageTextureFromMemory(allocation, fileSize, false));
        HLib::FreeTaskData(allocation);
    }
    allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "CText.png", TITLE_CTEXT_FILE, &fileSize);
    if (allocation) {
        if (fileSize)
            titleCTextTexture = static_cast<std::int32_t>(d3t::CreateImageTextureFromMemory(allocation, fileSize, false));
        HLib::FreeTaskData(allocation);
    }
    allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "KEYIMG.png", TITLE_KEY_IMAGE_FILE, &fileSize);
    if (allocation) {
        if (fileSize)
            titleKeyImageTexture = static_cast<std::int32_t>(d3t::CreateImageTextureFromMemory(allocation, fileSize, false));
        HLib::FreeTaskData(allocation);
    }
    allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "CONT.png", TITLE_CONTINUE_FILE, &fileSize);
    if (allocation) {
        if (fileSize)
            titleContinueTexture = static_cast<std::int32_t>(d3t::CreateImageTextureFromMemory(allocation, fileSize, false));
        HLib::FreeTaskData(allocation);
    }
    allocation = LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "E3_ESRB.SPR", TITLE_ESRB_FILE, &fileSize);
    if (allocation) {
        if (fileSize) {
            const std::byte *bytes = static_cast<const std::byte *>(allocation);
            std::int32_t *texture = &titleEsrbTexture;
            std::uint32_t signature, length;
            std::memcpy(&signature, bytes, 4);
            while (signature == SHEN_NAME(TEXN)) {
                *texture++ = static_cast<std::int32_t>(ReadTexnTexture(bytes));
                std::memcpy(&length, bytes + 4, 4);
                bytes += length;
                std::memcpy(&signature, bytes, 4);
            }
        }
        HLib::FreeTaskData(allocation);
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void InitializeLegacyQuantitySprites() {
    for (LegacyIconSprite &sprite : quantityDigits)
        sprite = {.x = -516, .y = 350, .scaleX = 1, .scaleY = 1, .depth = 3, .icon = 1030, .width = 16, .height = 16, .alignment = 5};
    for (std::size_t i = 0; i < quantityBrackets.size(); ++i)
        quantityBrackets[i] = {.x = -516,
            .y = 340,
            .scaleX = 1,
            .scaleY = 1.5f,
            .depth = 3,
            .icon = static_cast<std::uint16_t>(i ? 1053 : 1054),
            .width = 16,
            .height = 16,
            .alignment = 5};
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void DrawLegacyColourPacket(const LegacySpritePacket &packet, float depth) {
    if (!(packet.quad.flags & 0x1000U))
        depth = ConvertLegacyScreenDepth(depth);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    depth = 1.0f / depth;
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    NJS_POLYGON_VTX vertices[4];
    vertices[0].z = vertices[1].z = vertices[2].z = vertices[3].z = depth;
    vertices[0].x = packet.quad.corners[0];
    vertices[0].y = packet.quad.corners[1];
    vertices[1].x = packet.quad.corners[2];
    vertices[1].y = packet.quad.corners[3];
    vertices[2].x = packet.quad.corners[4];
    vertices[2].y = packet.quad.corners[5];
    vertices[3].x = packet.quad.corners[6];
    vertices[3].y = packet.quad.corners[7];
    if (packet.quad.flags & 8U) {
        vertices[0].col = packet.colour;
        SPRITE_MEMCPY(&vertices[1].col, SPRITE_DATA(packet.reservedColour), sizeof(std::uint32_t));
        SPRITE_MEMCPY(&vertices[2].col, SPRITE_DATA(packet.reservedColour) + 4, sizeof(std::uint32_t));
        SPRITE_MEMCPY(&vertices[3].col, SPRITE_DATA(packet.reservedColour) + 8, sizeof(std::uint32_t));
    } else {
        const std::uint32_t colour = packet.colour;
        vertices[0].col = colour;
        vertices[1].col = colour;
        vertices[2].col = colour;
        vertices[3].col = colour;
    }
#else
    std::uint32_t colours[4] SPRITE_INIT{packet.colour, packet.colour, packet.colour, packet.colour};
    if (packet.quad.flags & 8U)
        SPRITE_MEMCPY(colours + 1, SPRITE_DATA(packet.reservedColour), 12);
    NJS_POLYGON_VTX vertices[4] SPRITE_INIT{
        {SHEN_FIELD(x) packet.quad.corners[0], SHEN_FIELD(y) packet.quad.corners[1], SHEN_FIELD(z) depth, SHEN_FIELD(col) colours[0]},
        {SHEN_FIELD(x) packet.quad.corners[2], SHEN_FIELD(y) packet.quad.corners[3], SHEN_FIELD(z) depth, SHEN_FIELD(col) colours[1]},
        {SHEN_FIELD(x) packet.quad.corners[4], SHEN_FIELD(y) packet.quad.corners[5], SHEN_FIELD(z) depth, SHEN_FIELD(col) colours[2]},
        {SHEN_FIELD(x) packet.quad.corners[6], SHEN_FIELD(y) packet.quad.corners[7], SHEN_FIELD(z) depth, SHEN_FIELD(col) colours[3]},
    };
#endif
    njFogDisable();
    njDrawPolygon(vertices, 4, (packet.quad.flags & 0x400U) != 0);
    njFogEnable();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void SubmitLegacyTexturePacket(const LegacySpritePacket &packet, std::uint32_t texture, float depth) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float vertices[28];
    vertices[2] = vertices[9] = vertices[16] = vertices[23] = depth;
    vertices[0] = packet.quad.corners[0];
    vertices[1] = packet.quad.corners[1];
    vertices[7] = packet.quad.corners[2];
    vertices[8] = packet.quad.corners[3];
    vertices[14] = packet.quad.corners[4];
    vertices[15] = packet.quad.corners[5];
    vertices[21] = packet.quad.corners[6];
    vertices[22] = packet.quad.corners[7];
    float u0, u1, v0, v1;
    if (packet.quad.flags & 0x100U) {
        u0 = packet.uv[2];
        u1 = packet.uv[0];
    } else {
        u0 = packet.uv[0];
        u1 = packet.uv[2];
    }
    if (packet.quad.flags & 0x200U) {
        v0 = packet.uv[1];
        v1 = packet.uv[3];
    } else {
        v0 = packet.uv[3];
        v1 = packet.uv[1];
    }
    vertices[3] = u0;
    vertices[4] = v0;
    vertices[10] = u1;
    vertices[11] = v0;
    vertices[17] = u0;
    vertices[18] = v1;
    vertices[24] = u1;
    vertices[25] = v1;
    if (packet.quad.flags & 8U) {
        vertices[5] = SPRITE_BITS(float, packet.colour);
        SPRITE_MEMCPY(&vertices[12], SPRITE_DATA(packet.reservedColour), sizeof(float));
        SPRITE_MEMCPY(&vertices[19], SPRITE_DATA(packet.reservedColour) + 4, sizeof(float));
        SPRITE_MEMCPY(&vertices[26], SPRITE_DATA(packet.reservedColour) + 8, sizeof(float));
    } else {
        const float colour = SPRITE_BITS(float, packet.colour);
        vertices[5] = colour;
        vertices[12] = colour;
        vertices[19] = colour;
        vertices[26] = colour;
    }
    if (packet.quad.flags & 0x20U) {
        vertices[6] = SPRITE_BITS(float, packet.secondaryColour);
        SPRITE_MEMCPY(&vertices[13], SPRITE_DATA(packet.reservedUv), sizeof(float));
        SPRITE_MEMCPY(&vertices[20], SPRITE_DATA(packet.reservedUv) + 4, sizeof(float));
        SPRITE_MEMCPY(&vertices[27], SPRITE_DATA(packet.reservedUv) + 8, sizeof(float));
    } else {
        const float colour = SPRITE_BITS(float, packet.secondaryColour);
        vertices[6] = colour;
        vertices[13] = colour;
        vertices[20] = colour;
        vertices[27] = colour;
    }
#else
    const float u0 = packet.uv[(packet.quad.flags & 0x100U) ? 2 : 0];
    const float u1 = packet.uv[(packet.quad.flags & 0x100U) ? 0 : 2];
    const float v0 = packet.uv[(packet.quad.flags & 0x200U) ? 1 : 3];
    const float v1 = packet.uv[(packet.quad.flags & 0x200U) ? 3 : 1];
    std::uint32_t colours[4] SPRITE_INIT{packet.colour, packet.colour, packet.colour, packet.colour};
    std::uint32_t offsets[4] SPRITE_INIT{packet.secondaryColour, packet.secondaryColour, packet.secondaryColour, packet.secondaryColour};
    if (packet.quad.flags & 8U)
        SPRITE_MEMCPY(colours + 1, SPRITE_DATA(packet.reservedColour), 12);
    if (packet.quad.flags & 0x20U)
        SPRITE_MEMCPY(offsets + 1, SPRITE_DATA(packet.reservedUv), 12);
    const SPRITE_ARRAY(float, vertices, 28)
        SPRITE_INIT{packet.quad.corners[0], packet.quad.corners[1], depth, u0, v0, SPRITE_BITS(float, colours[0]), SPRITE_BITS(float, offsets[0]),
            packet.quad.corners[2], packet.quad.corners[3], depth, u1, v0, SPRITE_BITS(float, colours[1]), SPRITE_BITS(float, offsets[1]),
            packet.quad.corners[4], packet.quad.corners[5], depth, u0, v1, SPRITE_BITS(float, colours[2]), SPRITE_BITS(float, offsets[2]),
            packet.quad.corners[6], packet.quad.corners[7], depth, u1, v1, SPRITE_BITS(float, colours[3]), SPRITE_BITS(float, offsets[3])};
#endif
    dc::sdk::SubmitScreenTextureStrip(
        SPRITE_DATA(vertices), static_cast<std::int32_t>(texture), packet.quad.flags, static_cast<std::int32_t>(packet.reservedTail));
}

void DrawLegacySpritePacket(const LegacySpritePacket &packet, std::uint32_t texture, float depth) {
    if (!(packet.quad.flags & 0x1000U))
        depth = ConvertLegacyScreenDepth(depth);
    SubmitLegacyTexturePacket(packet, texture, depth);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetLegacySpriteSlotScale(float x, float y) {
    legacySpriteSlotScaleX = x;
    legacySpriteSlotScaleY = y;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetLegacySpriteSlotSizeOffsets(std::int32_t width, std::int32_t height) {
    legacySpriteSlotWidthOffset = width;
    legacySpriteSlotHeightOffset = height;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetLegacySpriteSlotRotation(std::int32_t angle) {
    legacySpriteSlotRotation = angle;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetLegacySpriteSlotColour(std::uint32_t colour) {
    legacySpriteSlotColour = colour;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetLegacySpriteSlotSecondaryColour(std::uint32_t colour) {
    legacySpriteSlotSecondaryColour = colour;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void DrawTempSpriteSlot(std::uint32_t icon, std::uint32_t position, float depth, std::uint32_t flags) {
    LegacySpritePacket packet;
    const TempTextureWork &work = GetTempSpriteTexture(static_cast<std::int16_t>(icon >> 16), static_cast<std::uint16_t>(icon));
    if ((work.flags & TTF_Loaded) == 0)
        return;
    INIT_LEGACY_SPRITE_PACKET(packet);
    if (flags & 0x10U) {
        const std::uint32_t pulse = GetLegacySpriteSlotPulse();
        if (flags & 0x20U) {
            packet.colour = 0xffcc808dU;
            packet.secondaryColour = ((pulse << 16) | (pulse << 8) | pulse) - 0x780000U;
        } else {
            packet.secondaryColour = ((pulse << 16) | (pulse << 8) | pulse) - 0x1000000U;
        }
    }
    if (legacySpriteSlotColour != UINT32_MAX)
        packet.colour = legacySpriteSlotColour;
    if (legacySpriteSlotSecondaryColour)
        packet.secondaryColour = legacySpriteSlotSecondaryColour;
    if (work.flags & TTF_Region) {
        packet.uv[0] = work.uv[0];
        packet.uv[1] = work.uv[1];
        packet.uv[2] = work.uv[2];
        packet.uv[3] = work.uv[3];
    }
    packet.quad.flags |= 0x400U;
    if (legacySpriteSlotScaleX == 1.0f && legacySpriteSlotScaleY == 1.0f && !legacySpriteSlotRotation)
        AdjustScreenAspectRatio(packet.quad, SPRITE_ALIGN(HorizontalStretch), static_cast<std::int16_t>(position >> 16), static_cast<std::uint16_t>(position),
            work.width + legacySpriteSlotWidthOffset, work.height + legacySpriteSlotHeightOffset);
    else
        TransformScreenSpace(packet.quad, SPRITE_ALIGN(HorizontalStretch), static_cast<std::int16_t>(position >> 16), static_cast<std::uint16_t>(position),
            work.width + legacySpriteSlotWidthOffset, work.height + legacySpriteSlotHeightOffset, legacySpriteSlotScaleX, legacySpriteSlotScaleY,
            legacySpriteSlotRotation);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(work.texture), depth);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static std::int32_t legacyClearColourEditComponent = 0;
static std::int32_t legacyClearColourEditEntry = 0;
#endif
static std::uint8_t legacyClearColourBytes[8] = {0xff, 0x50, 0x50, 0x50, 0xff, 0, 0, 0};

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void EditLegacyClearColours(std::int32_t column, std::int32_t line, std::int32_t port) {
    const std::uint32_t held = g_inputBuffer[port].held;
    const std::uint32_t pressed = g_inputBuffer[port].pressedPriority;
    if (pressed & 0x1000U) {
        legacyClearColourEditEntry = 1 - legacyClearColourEditEntry;
    } else if (pressed & 0x2000U) {
        legacyClearColourEditEntry = 1 - legacyClearColourEditEntry;
    } else if (pressed & 0x4000U) {
        if (--legacyClearColourEditComponent < 0)
            legacyClearColourEditComponent = 3;
    } else if (pressed & 0x8000U) {
        if (++legacyClearColourEditComponent > 3)
            legacyClearColourEditComponent = 0;
    }
    if ((pressed >> 10) & 1 || (held & 0x440U) == 0x440U) {
        const std::int32_t value = legacyClearColourBytes[legacyClearColourEditComponent + legacyClearColourEditEntry * 4] + 1;
        legacyClearColourBytes[legacyClearColourEditComponent + legacyClearColourEditEntry * 4] = value > 0xff ? 0xff : value;
    } else if ((pressed >> 9) & 1 || (held & 0x240U) == 0x240U) {
        const std::int32_t value = legacyClearColourBytes[legacyClearColourEditComponent + legacyClearColourEditEntry * 4] - 1;
        legacyClearColourBytes[legacyClearColourEditComponent + legacyClearColourEditEntry * 4] = value < 0 ? 0 : value;
    } else if ((held & 0x480U) == 0x480U) {
        legacyClearColourBytes[legacyClearColourEditComponent + legacyClearColourEditEntry * 4] = 0xff;
    } else if ((held & 0x280U) == 0x280U) {
        legacyClearColourBytes[legacyClearColourEditComponent + legacyClearColourEditEntry * 4] = 0;
    }
    SetDebugScreenTextPosition(column, line);
    shenmue_s2_event_DebugLogToScreen(
        "base: %03d %03d %03d %03d", legacyClearColourBytes[0], legacyClearColourBytes[1], legacyClearColourBytes[2], legacyClearColourBytes[3]);
    SetDebugScreenTextPosition(column, line + 1);
    shenmue_s2_event_DebugLogToScreen("ofst:     %03d %03d %03d", legacyClearColourBytes[5], legacyClearColourBytes[6], legacyClearColourBytes[7]);
    SetDebugScreenTextPosition(column + 5 + legacyClearColourEditComponent * 4, line + legacyClearColourEditEntry);
    shenmue_s2_event_DebugLogToScreen(">");
}
#endif

std::uint32_t GetGreyClearColour() {
    return static_cast<std::uint32_t>(legacyClearColourBytes[0]) << 24 | static_cast<std::uint32_t>(legacyClearColourBytes[1]) << 16 |
        static_cast<std::uint32_t>(legacyClearColourBytes[2]) << 8 | legacyClearColourBytes[3];
}

std::uint32_t GetBlackClearColour() {
    return 0xff000000U | static_cast<std::uint32_t>(legacyClearColourBytes[5]) << 16 | static_cast<std::uint32_t>(legacyClearColourBytes[6]) << 8 |
        legacyClearColourBytes[7];
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawTempSpriteClearColourSlot(std::uint32_t icon, std::uint32_t position, float depth, std::int32_t mode) {
    LegacySpritePacket packet;
    const TempTextureWork &work = GetTempSpriteTexture(static_cast<std::int16_t>(icon >> 16), static_cast<std::uint16_t>(icon));
    if ((work.flags & TTF_Loaded) == 0)
        return;
    if (!mode && (g_inputBuffer[1].held & 0x800U))
        DrawTempSpriteClearColourSlot(icon, position, depth, 1);
    INIT_LEGACY_SPRITE_PACKET(packet);
    if (mode == 1) {
        packet.colour = 0xffffffffU;
    } else {
        packet.colour = GetGreyClearColour();
        packet.secondaryColour = GetBlackClearColour();
    }
    if (work.flags & TTF_Region) {
        packet.uv[0] = work.uv[0];
        packet.uv[1] = work.uv[1];
        packet.uv[2] = work.uv[2];
        packet.uv[3] = work.uv[3];
    }
    packet.quad.flags |= 0x400U;
    if (legacySpriteSlotScaleX == 1.0f && legacySpriteSlotScaleY == 1.0f)
        AdjustScreenAspectRatio(packet.quad, SPRITE_ALIGN(HorizontalStretch), static_cast<std::int16_t>(position >> 16), static_cast<std::uint16_t>(position),
            work.width + legacySpriteSlotWidthOffset, work.height + legacySpriteSlotHeightOffset);
    else
        TransformScreenSpace(packet.quad, SPRITE_ALIGN(HorizontalStretch), static_cast<std::int16_t>(position >> 16), static_cast<std::uint16_t>(position),
            work.width + legacySpriteSlotWidthOffset, work.height + legacySpriteSlotHeightOffset, legacySpriteSlotScaleX, legacySpriteSlotScaleY, 0);
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(work.texture), depth);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ResetLegacySpriteSlotPulse() {
    legacySpriteSlotPulse = 0.0f;
    legacySpriteSlotPulseStep = 1.4f;
}

void UpdateLegacySpriteSlotPulse() {
    legacySpriteSlotPulse += legacySpriteSlotPulseStep;
    if (legacySpriteSlotPulse > 22.0f) {
        legacySpriteSlotPulse = 22.0f;
        legacySpriteSlotPulseStep = -1.4f;
    } else if (legacySpriteSlotPulse < 0.0f) {
        legacySpriteSlotPulse = 0.0f;
        legacySpriteSlotPulseStep = 1.4f;
    }
}
#endif

std::uint32_t GetLegacySpriteSlotPulse() {
    return static_cast<std::uint32_t>(legacySpriteSlotPulse);
}

#else

void DrawTempSpriteSlot(std::uint32_t icon, std::uint32_t position, float depth, std::uint8_t flags) {
    const TempTextureWork &work = GetTempSpriteTexture(std::bit_cast<std::int32_t>(icon) >> 16, static_cast<std::uint16_t>(icon));
    if (!(work.flags & TTF_Loaded))
        return;
    const std::int32_t x = std::bit_cast<std::int32_t>(position) >> 16;
    const std::uint16_t y = static_cast<std::uint16_t>(position);
    std::uint32_t colour = 0xffffffffU, secondary = 0;
    if (flags & 0x10U) {
        const std::uint32_t pulse = static_cast<std::uint32_t>(static_cast<std::int64_t>(legacySpriteSlotPulse));
        const std::uint32_t rgb = pulse | ((pulse | (pulse << 8U)) << 8U);
        if (flags & 0x20U) {
            colour = 0xffcc808dU;
            secondary = rgb - 0x780000U;
        } else
            secondary = rgb - 0x1000000U;
    }
    if (legacySpriteSlotColour != 0xffffffffU)
        colour = legacySpriteSlotColour;
    if (legacySpriteSlotSecondaryColour)
        secondary = legacySpriteSlotSecondaryColour;
    LegacySpritePacket packet{.quad = {.flags = 1024}, .colour = colour, .secondaryColour = secondary, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
    if (work.flags & TTF_Region)
        packet.uv = work.uv;
    if (legacySpriteSlotScaleX == 1.0f && legacySpriteSlotScaleY == 1.0f && !legacySpriteSlotRotation)
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, x, y, legacySpriteSlotWidthOffset + work.width,
            legacySpriteSlotHeightOffset + work.height, ScreenMode::Centered);
    else
        TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, x, y, legacySpriteSlotWidthOffset + work.width,
            legacySpriteSlotHeightOffset + work.height, legacySpriteSlotScaleX, legacySpriteSlotScaleY, legacySpriteSlotRotation, ScreenMode::Centered);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(work.texture), depth);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTempSpriteDigit(std::uint32_t icon, std::uint32_t position, float depth, std::int32_t digit) {
    const TempTextureWork &work = GetTempSpriteTexture(std::bit_cast<std::int32_t>(icon) >> 16, static_cast<std::uint16_t>(icon));
    if (!(work.flags & TTF_Loaded))
        return;
    LegacySpritePacket packet{.quad = {.flags = 1024}, .colour = 0xffffffffU, .secondaryColour = 0, .uv = TITLE_WARNING_DIGIT_UV[digit]};
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), std::bit_cast<std::int32_t>(position) >> 16, static_cast<std::uint16_t>(position), 18,
        24, ScreenMode::Centered);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(work.texture), depth);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleSelection(std::int32_t x, std::int32_t y, std::int32_t separation, std::int32_t choice) {
    titleConfirmChoice = TITLE_CONFIRM_CHOICES[choice].data();
    DrawTempSpriteSlot(
        (titleConfirmChoice[0] << 16U) | titleConfirmChoice[1], (static_cast<std::uint32_t>(x - separation) << 16U) | static_cast<std::uint16_t>(y), 13.0f, 16);
    titleConfirmChoice += 2;
    DrawTempSpriteSlot(
        (titleConfirmChoice[0] << 16U) | titleConfirmChoice[1], (static_cast<std::uint32_t>(x + separation) << 16U) | static_cast<std::uint16_t>(y), 13.0f, 16);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetSpriteTexturePath(const char *path) noexcept {
    spriteTexturePath = path;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ApplySpriteTextureOverride(const void *texn, std::uint32_t textureHandle) {
    if (!spriteTexturePath || !*spriteTexturePath)
        return;
    char name[9]{};
    std::memcpy(name, static_cast<const std::byte *>(texn) + 8, 8);
    char source[260]{};
    std::snprintf(source, sizeof(source), "%s%s.png", spriteTexturePath, name);
    FilepathCleaner(source);
    std::int32_t count = 0;
    const SDTextureMapping *const mappings = GetSDTextureMappings(&count);
    for (std::int32_t index = 0; index < count; ++index) {
        if (std::strcmp(mappings[index].source.data(), source) != 0)
            continue;
        ReplaceSDTextureImage(mappings[index].destination.data(), textureHandle);
        return;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Native SD-override list accessor shared by the TEXN and MT7 readers.
const SDTextureMapping *GetSDTextureMappings(std::int32_t *count) {
    *count = static_cast<std::int32_t>(sdTextureMappings.size());
    return sdTextureMappings.data();
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// The language-localized override replaces the texture's image.
void ReplaceSDTextureImage(const char *mappedDestination, std::uint32_t textureHandle) {
    char destination[272]{};
    std::snprintf(destination, sizeof(destination), "%s", mappedDestination);
    constexpr std::array<const char *, 7> suffix{"", "_jp", "_fr", "_de", "_kr", "_tc", "_sc"};
    const std::int32_t language = ResolveSceneTextLanguage();
    const char *const extension = std::strrchr(destination, '.');
    if (extension && language > 0 && language < static_cast<std::int32_t>(suffix.size())) {
        char localized[272]{};
        const auto stemLength = static_cast<int>(extension - destination);
        std::snprintf(localized, sizeof(localized), "%.*s%s%s", stemLength, destination, suffix[static_cast<std::size_t>(language)], extension);
        if (ArchiveData_GetFileSize(localized))
            std::snprintf(destination, sizeof(destination), "%s", localized);
    }
    const d3t::D3TShaderLoadTask request{
        .path = destination, .renderHandle = 0, .thread = {ReplaceSpriteImage, reinterpret_cast<void *>(static_cast<std::uintptr_t>(textureHandle))}};
    d3t::PushShaderLoadToThreadQueue(&request);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int64_t QueueSDTextureOverride(const char *path, std::uint32_t renderHandle) {
    const d3t::D3TShaderLoadTask request{.path = path, .renderHandle = renderHandle, .thread = {SDTextureOverride, nullptr}};
    return d3t::PushShaderLoadToThreadQueue(&request);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ReleaseSDTextureOverrides() {
    std::vector<SDTextureMapping>{}.swap(sdTextureMappings);
    spriteTexturePath = nullptr;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ResetLegacyCollectionSpriteOwners() noexcept {
    collectionSpriteOwners = {};
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
const std::int8_t *FindCommonIconRectangle(std::int16_t icon) {
    const std::uint32_t name = static_cast<std::uint32_t>(static_cast<std::int32_t>(icon));
    const std::uint32_t bank = (name >> 8) & 255U;
    const std::uint32_t offset = bank < 14 ? kCommonIconRectangleOffsets[bank] + (name & 255U) * 4U : 0x104U;
    return reinterpret_cast<const std::int8_t *>(kCommonIconRectangles + offset);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t GetCommonIconTextureIndex(std::uint32_t icon) {
    const std::int32_t bank = (icon >> 8) & 255U;
    std::int32_t texture;
    if (bank > 13)
        texture = g_commonIconTextures[0];
    else
        texture = g_commonIconTextures[bank];
    return texture;
}

std::uint16_t GetCommonIconTextureHandle(std::uint16_t icon) {
    return static_cast<std::uint16_t>(GetCommonIconTextureIndex(icon));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void PrepareDrawSimple(const LegacyIconSprite &sprite, std::uint32_t colour) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const std::int8_t *const rectangle = GetCommonIconRect(sprite.icon);
    if (!rectangle || GetCommonIconTextureIndex(static_cast<std::uint16_t>(sprite.icon)) < 0)
        return;
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.secondaryColour = colour & 0x00ffffffU;
    packet.colour = (colour & 0xff000000U) | 0x00ffffffU;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), static_cast<std::int32_t>(sprite.x), static_cast<std::int32_t>(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, static_cast<std::int32_t>(sprite.rotation));
    packet.uv[0] = static_cast<float>(rectangle[0]) * 0.125f;
    packet.uv[1] = static_cast<float>(rectangle[1]) * 0.125f;
    packet.uv[2] = static_cast<float>(rectangle[2]) * 0.125f;
    packet.uv[3] = static_cast<float>(rectangle[3]) * 0.125f;
    packet.quad.flags |= static_cast<std::uint16_t>(sprite.flags | 0x400U);
    DrawLegacySpritePacket(packet, GetCommonIconTextureHandle(sprite.icon), sprite.depth);
#else
    const std::int8_t *const rectangle = FindCommonIconRectangle(static_cast<std::int16_t>(sprite.icon));
    if (!rectangle || GetCommonIconTextureIndex(sprite.icon) < 0)
        return;
    LegacySpritePacket packet;
    packet.quad.flags = 0;
    packet.colour = (colour & 0xff000000U) | 0x00ffffffU;
    packet.secondaryColour = colour & 0x00ffffffU;
    packet.uv[0] = packet.uv[1] = 0.0f;
    packet.uv[2] = packet.uv[3] = 1.0f;
    packet.reservedTail = 0;
    std::int32_t x;
    std::int32_t y;
    asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(x) : "f"(sprite.x) : "fpul");
    asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(y) : "f"(sprite.y) : "fpul");
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), x, y, sprite.width, sprite.height, sprite.scaleX, sprite.scaleY,
        SPRITE_BITS(std::int32_t, sprite.rotation));
    for (std::int32_t index = 0; index < 4; ++index)
        packet.uv[index] = static_cast<float>(rectangle[index]) * 0.125f;
    packet.quad.flags |= sprite.flags | 0x400U;
    DrawLegacySpritePacket(packet, static_cast<std::uint16_t>(GetCommonIconTextureIndex(sprite.icon)), sprite.depth);
#endif
}

#else

void PrepareDrawSimple(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode) {
    const std::array<std::uint8_t, 4> uv = GetCommonIconRect(sprite.icon);
    const CommonIconAtlas *const atlas = GetCommonIconAtlas(sprite.icon);
    // The repository atlas stores tagged D3T handles, not signed native surface indices.
    if (atlas->textureHandle == 0 || atlas->textureHandle == 0xffffffffU)
        return;
    LegacySpritePacket packet;
    packet.quad.flags = 0;
    packet.colour = colour | 0x00ffffffU;
    packet.secondaryColour = colour & 0x00ffffffU;
    packet.reservedTail = 0;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    for (std::size_t i = 0; i < uv.size(); ++i)
        packet.uv[i] = static_cast<float>(static_cast<std::int8_t>(uv[i])) * 0.125f;
    packet.quad.flags |= sprite.flags | 0x400U;
    LegacySpriteNullsub(packet, static_cast<std::uint16_t>(atlas->textureHandle), sprite.depth);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#else

bool vfSubmitQuad(std::uint32_t colour, float depth) {
    LegacyColourQuad packet{.quad = {.flags = 0}, .colour = colour};
    TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, 320, 240, 640, 480, 1.0f, 1.0f, 0, ScreenMode::Adjusted);
    packet.quad.flags |= 0x400U;
    return shenSubmitQuad(packet, depth);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void DrawLegacyIconSprite(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode) {
    const CommonIconAtlas *const atlas = GetCommonIconAtlas(sprite.icon);
    if (!atlas || !atlas->textureHandle)
        return;
    const auto uv = GetCommonIconRect(sprite.icon);
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = colour | 0x00ffffffU, .secondaryColour = colour & 0x00ffffffU};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    for (std::size_t i = 0; i < uv.size(); ++i)
        packet.uv[i] = static_cast<float>(static_cast<std::int8_t>(uv[i])) * 0.125f;
    packet.quad.flags |= sprite.flags | 0x400U;
    const float depth = (packet.quad.flags & 0x1000U) ? sprite.depth : sprite.depth < 0.0f ? std::fmax(1.0f, -sprite.depth) : 1.0f / (sprite.depth + 100.0f);
    DrawLegacySpritePacket(packet, atlas->textureHandle, depth);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void DrawBorrowedLegacySprite(std::uint32_t icon, std::int32_t x, std::int32_t y, float scaleX, float scaleY, std::int32_t rotation, std::uint32_t colour) {
    for (const BorrowedLegacySpriteSlot &entry : borrowedSpriteSlots) {
        if (!entry.rectangles || entry.slot != (icon >> 16U) || entry.texture < 0)
            continue;
        const std::uint16_t *const rectangle = entry.rectangles + static_cast<std::uint16_t>(icon) * 4;
        LegacySpritePacket packet{};
        packet.colour = colour;
        packet.uv = {rectangle[0] * entry.inverseWidth, 1.0f - ((rectangle[1] + rectangle[3]) * entry.inverseHeight),
            (rectangle[0] + rectangle[2]) * entry.inverseWidth, 1.0f - (rectangle[1] * entry.inverseHeight)};
        TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, x, y, rectangle[2], rectangle[3], scaleX, scaleY, rotation, ScreenMode::Centered);
        packet.quad.flags |= 0x400U;
        LegacySpriteNullsub(packet, static_cast<std::uint32_t>(entry.texture), 0.0f);
        return;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void PrepareDrawWithMode(const LegacyIconSprite &sprite) {
    PrepareDrawSimple(sprite, 0xff000000U);
}

#else

void PrepareDrawWithMode(const LegacyIconSprite &sprite, ScreenMode mode) {
    PrepareDrawSimple(sprite, 0xff000000U, mode);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void PrepareDrawSimpleOpaque(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode) {
    PrepareDrawSimple(sprite, colour | 0xff000000U, mode);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void InitializeLegacyItemWindow() {
    InitializeLegacyIconSprite(itemWindowSelection);
    itemWindowSelection.depth = 0.80000001f;
    itemWindowSelection.icon = 1029;
    itemWindowSelection.width = itemWindowSelection.height = 38;
    for (std::uint32_t index = 0; index < itemWindowEdges.size(); ++index) {
        InitializeLegacyIconSprite(itemWindowEdges[index]);
        itemWindowEdges[index].icon = itemWindowEdgeIcons[index];
        itemWindowEdges[index].depth = 0.5f;
        if (itemWindowEdgeIcons[index] == 1026) {
            itemWindowEdges[index].width = 40;
            itemWindowEdges[index].height = 16;
        }
    }
    const std::array<std::uint8_t, 4> tiles = GetCommonIconRect(1547), separators = GetCommonIconRect(1026);
    itemWindowTiles = {.quad = {.flags = 0x400}, .colour = 0xffffffffU, .secondaryColour = 0, .reservedTail = 0};
    itemWindowSeparators = {.quad = {.flags = 0}, .colour = 0xffffffffU, .secondaryColour = 0, .reservedTail = 0};
    for (std::size_t index = 0; index < 4; ++index) {
        itemWindowTiles.uv[index] = static_cast<float>(static_cast<std::int8_t>(tiles[index])) * 0.125f;
        itemWindowSeparators.uv[index] = static_cast<float>(static_cast<std::int8_t>(separators[index])) * 0.125f;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void PrepareDrawTextureWithMode(const LegacyIconSprite &sprite) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), static_cast<std::int32_t>(sprite.x), static_cast<std::int32_t>(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, static_cast<std::int32_t>(sprite.rotation));
#else
    std::int32_t x;
    std::int32_t y;
    asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(x) : "f"(sprite.x) : "fpul");
    asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(y) : "f"(sprite.y) : "fpul");
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), x, y, sprite.width, sprite.height, sprite.scaleX, sprite.scaleY,
        SPRITE_BITS(std::int32_t, sprite.rotation));
#endif
    packet.quad.flags |= sprite.flags | 0x400U;
    DrawLegacySpritePacket(packet, static_cast<std::int16_t>(sprite.icon), sprite.depth);
}

#else

void PrepareDrawTextureWithMode(const LegacyIconSprite &sprite, ScreenMode mode) {
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = 0xffffffffU, .secondaryColour = 0, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    packet.quad.flags |= sprite.flags | 0x400U;
    LegacySpriteNullsub(packet, sprite.icon, sprite.depth);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
bool shenSubmitQuad(const LegacyColourQuad &packet, float depth) {
    if (!(packet.quad.flags & 0x1000U))
        depth = depth >= 0.0f ? 1.0f / (depth + 100.0f) : std::fmax(1.0f, -depth);
    std::array<LegacyStripType0Vertex, 4> vertices;
    for (std::size_t index = 0; index < vertices.size(); ++index) {
        vertices[index] = {.position = {packet.quad.corners[index * 2], packet.quad.corners[index * 2 + 1], depth},
            .diffuse = index && (packet.quad.flags & 8U) ? packet.alternateColours[index - 1] : packet.colour};
    }
    SetLegacyFogMode(2);
    if (packet.quad.flags & 0x400U)
        SubmitStripType3Quad(vertices);
    else
        ApplyLegacyStripRenderMode();
    SetLegacyFogMode(0);
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
bool DrawLegacySolidRect(const LegacySolidRect &rect) {
    LegacyColourQuad packet{.quad = {.flags = 0}, .colour = rect.colour};
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(rect.x), TruncateSpriteCoordinate(rect.y), rect.width,
        rect.height, ScreenMode::Centered);
    packet.quad.flags |= 1024;
    return shenSubmitQuad(packet, rect.depth);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
bool DrawLegacyColourRect(const LegacyColourRect &rect) {
    LegacyColourQuad packet{.quad = {.flags = 0}, .colour = rect.colour};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(rect.alignment), TruncateSpriteCoordinate(rect.x), TruncateSpriteCoordinate(rect.y),
        rect.width, rect.height, rect.scaleX, rect.scaleY, rect.rotation, ScreenMode::Centered);
    packet.quad.flags |= 0x400U;
    return shenSubmitQuad(packet, rect.depth);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleBackground() {
    titleSpritePacket.colour = 0xffffffffU;
    titleSpritePacket.secondaryColour = 0;
    titleSpritePacket.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    titleSpritePacket.reservedTail = 0;
    titleSpritePacket.quad.flags = 1024;
    AdjustScreenAspectRatio(titleSpritePacket.quad, ScreenAlignment::HorizontalStretch, 320, 240, 640, 640, ScreenMode::Centered);
    LegacySpriteNullsub(titleSpritePacket, static_cast<std::uint32_t>(titleSpriteTextures[14]), -2.9f);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleHints(std::int32_t flags) {
    std::int32_t ordinal = 0;
    for (std::int32_t x = 485; x < 629; x += 48) {
        if (TITLE_HINT_MASKS[ordinal] == 0 || (TITLE_HINT_MASKS[ordinal] & flags) == 0) {
            titleSpritePacket.colour = 0xffffffffU;
            titleSpritePacket.secondaryColour = 0;
            titleSpritePacket.uv = {0.0f, 0.0f, 1.0f, 1.0f};
            titleSpritePacket.reservedTail = 0;
            titleSpritePacket.quad.flags = 1024;
            AdjustScreenAspectRatio(titleSpritePacket.quad, ScreenAlignment::HorizontalStretch, x, 54, 32, 32, ScreenMode::Centered);
            titleSpritePacket.uv[0] = static_cast<float>(ordinal & 1) * 0.5f;
            titleSpritePacket.uv[1] = static_cast<float>((ordinal >> 1) & 1) * 0.5f;
            titleSpritePacket.uv[2] = titleSpritePacket.uv[0] + 0.5f;
            titleSpritePacket.uv[3] = titleSpritePacket.uv[1] + 0.5f;
            LegacySpriteNullsub(titleSpritePacket, static_cast<std::uint32_t>(titleSpriteTextures[17]), -2.9f);
        }
        ++ordinal;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleFrame(std::int32_t flags) {
    if ((flags & 1) != 0)
        DrawTitleBackground();
    if ((flags & 2) != 0)
        DrawTitleHints(flags);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleCaptionLine(std::int32_t y, std::int32_t item) {
    titleSpritePacket.colour = 0xffffffffU;
    titleSpritePacket.secondaryColour = 0;
    titleSpritePacket.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    titleSpritePacket.reservedTail = 0;
    titleSpritePacket.quad.flags = 1024;
    const std::int32_t page = item / 17, row = item % 17;
    AdjustScreenAspectRatio(titleSpritePacket.quad, ScreenAlignment::HorizontalStretch, 320, y, 512, 30, ScreenMode::Centered);
    titleSpritePacket.uv[0] = 0.0f;
    titleSpritePacket.uv[2] = 1.0f;
    titleSpritePacket.uv[1] = (static_cast<float>(16 - row) * 30.0f) * 0.001953125f;
    titleSpritePacket.uv[3] = (static_cast<float>(17 - row) * 30.0f) * 0.001953125f;
    LegacySpriteNullsub(titleSpritePacket, static_cast<std::uint32_t>(titleSpriteTextures[page + 15]), -2.9f);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleCaption(std::int32_t item, std::int32_t secondary) {
    std::int32_t y = 415;
    if (secondary > -1) {
        DrawTitleCaptionLine(400, secondary);
        y = 430;
    }
    DrawTitleCaptionLine(y, item);
    titleSpritePacket.colour = 0xffffffffU;
    titleSpritePacket.secondaryColour = 0;
    titleSpritePacket.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    titleSpritePacket.reservedTail = 0;
    titleSpritePacket.quad.flags = 1024;
    AdjustScreenAspectRatio(titleSpritePacket.quad, ScreenAlignment::HorizontalStretch, 320, 415, 512, 128, ScreenMode::Centered);
    LegacySpriteNullsub(titleSpritePacket, static_cast<std::uint32_t>(titleSpriteTextures[16]), -2.9f);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleColourSpriteRect(const std::uint16_t *uv, const Vector3 &position, std::int32_t texture, float scale, std::uint32_t colour) {
    titleSpritePacket.secondaryColour = 0;
    titleSpritePacket.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    titleSpritePacket.reservedTail = 0;
    titleSpritePacket.quad.flags = 1024;
    titleSpritePacket.colour = colour;
    std::int32_t width = TITLE_SPRITE_DIMENSIONS[texture][0], height = TITLE_SPRITE_DIMENSIONS[texture][1];
    if (uv) {
        for (std::size_t index = 0; index < 4; ++index)
            titleSpritePacket.uv[index] = static_cast<float>(static_cast<std::int16_t>(uv[index])) * 0.001f;
        width = TruncateSpriteCoordinate(
            static_cast<float>(static_cast<std::int16_t>(uv[2]) - static_cast<std::int16_t>(uv[0])) * 0.001f * static_cast<float>(width));
        height = TruncateSpriteCoordinate(
            static_cast<float>(static_cast<std::int16_t>(uv[3]) - static_cast<std::int16_t>(uv[1])) * 0.001f * static_cast<float>(height));
    }
    AdjustScreenAspectRatio(titleSpritePacket.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(position.x),
        TruncateSpriteCoordinate(position.y), TruncateSpriteCoordinate(static_cast<float>(width) * scale),
        TruncateSpriteCoordinate(static_cast<float>(height) * scale), ScreenMode::Centered);
    LegacySpriteNullsub(titleSpritePacket, static_cast<std::uint32_t>(titleSpriteTextures[texture]), position.z);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawSizedTitleSpriteRect(const std::uint16_t *uv, const Vector3 &position, std::int32_t texture, std::int16_t width, std::int16_t height) {
    titleSpritePacket.colour = 0xffffffffU;
    titleSpritePacket.secondaryColour = 0;
    titleSpritePacket.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    titleSpritePacket.reservedTail = 0;
    titleSpritePacket.quad.flags = 1024;
    if (uv)
        for (std::size_t index = 0; index < 4; ++index)
            titleSpritePacket.uv[index] = static_cast<float>(static_cast<std::int16_t>(uv[index])) * 0.001f;
    AdjustScreenAspectRatio(titleSpritePacket.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(position.x),
        TruncateSpriteCoordinate(position.y), width, height, ScreenMode::Centered);
    LegacySpriteNullsub(titleSpritePacket, static_cast<std::uint32_t>(titleSpriteTextures[texture]), position.z);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleSpriteRect(const std::uint16_t *uv, const Vector3 &position, std::int32_t texture, float scale) {
    titleSpritePacket.colour = 0xffffffffU;
    titleSpritePacket.secondaryColour = 0;
    titleSpritePacket.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    titleSpritePacket.reservedTail = 0;
    titleSpritePacket.quad.flags = 1024;
    std::int32_t width = TITLE_SPRITE_DIMENSIONS[texture][0], height = TITLE_SPRITE_DIMENSIONS[texture][1];
    if (uv) {
        for (std::size_t index = 0; index < 4; ++index)
            titleSpritePacket.uv[index] = static_cast<float>(static_cast<std::int16_t>(uv[index])) * 0.001f;
        width = TruncateSpriteCoordinate(
            static_cast<float>(static_cast<std::int16_t>(uv[2]) - static_cast<std::int16_t>(uv[0])) * 0.001f * static_cast<float>(width));
        height = TruncateSpriteCoordinate(
            static_cast<float>(static_cast<std::int16_t>(uv[3]) - static_cast<std::int16_t>(uv[1])) * 0.001f * static_cast<float>(height));
    }
    AdjustScreenAspectRatio(titleSpritePacket.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(position.x),
        TruncateSpriteCoordinate(position.y), TruncateSpriteCoordinate(static_cast<float>(width) * scale),
        TruncateSpriteCoordinate(static_cast<float>(height) * scale), ScreenMode::Centered);
    LegacySpriteNullsub(titleSpritePacket, static_cast<std::uint32_t>(titleSpriteTextures[texture]), position.z);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool shenSubmitQuadsUI(float x, float y, std::uint32_t fill, std::uint32_t border, std::int32_t width, std::int32_t height, float depth) {
    LegacyColourQuad packet{.quad = {.flags = 0}, .colour = fill};
    const float innerWidth = static_cast<float>(width) - 4.0f, innerHeight = static_cast<float>(height) - 4.0f;
    const std::int32_t ix = TruncateSpriteCoordinate(x), iy = TruncateSpriteCoordinate(y);
    TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, ix, iy, TruncateSpriteCoordinate(innerWidth), TruncateSpriteCoordinate(innerHeight),
        1.0f, 1.0f, 0, ScreenMode::Centered);
    packet.quad.flags |= 0x400U;
    shenSubmitQuad(packet, depth);
    packet.colour = border;
    const float halfHeight = innerHeight * 0.5f, halfWidth = innerWidth * 0.5f;
    TransformScreenSpace(
        packet.quad, ScreenAlignment::HorizontalStretch, ix, TruncateSpriteCoordinate((y - halfHeight) - 1.0f), width, 2, 1.0f, 1.0f, 0, ScreenMode::Centered);
    shenSubmitQuad(packet, depth + 1.0f);
    TransformScreenSpace(
        packet.quad, ScreenAlignment::HorizontalStretch, ix, TruncateSpriteCoordinate((halfHeight + y) + 1.0f), width, 2, 1.0f, 1.0f, 0, ScreenMode::Centered);
    shenSubmitQuad(packet, depth + 1.0f);
    TransformScreenSpace(
        packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate((x - halfWidth) - 1.0f), iy, 2, height, 1.0f, 1.0f, 0, ScreenMode::Centered);
    shenSubmitQuad(packet, depth + 1.0f);
    TransformScreenSpace(
        packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate((halfWidth + x) + 1.0f), iy, 2, height, 1.0f, 1.0f, 0, ScreenMode::Centered);
    return shenSubmitQuad(packet, depth + 1.0f);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void PrepareDrawIndexedTextureWithSecondary(const LegacyIconSprite &sprite, std::uint32_t secondaryColour) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.secondaryColour = secondaryColour;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateToInt32(sprite.x), TruncateToInt32(sprite.y), sprite.width,
        sprite.height, sprite.scaleX, sprite.scaleY, SPRITE_BITS(std::int32_t, sprite.rotation));
    packet.quad.flags |= sprite.flags | 0x400U;
    DrawLegacySpritePacket(packet, static_cast<std::int16_t>(sprite.icon), sprite.depth);
}
#endif

#else

// @verified - p
void PrepareDrawIndexedTextureWithSecondary(const LegacyIconSprite &sprite, std::uint32_t secondaryColour, ScreenMode mode) {
    LegacySpritePacket packet{
        .quad = {.flags = 0}, .colour = 0xffffffffU, .secondaryColour = secondaryColour, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    packet.quad.flags |= sprite.flags | 0x400U;
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int16_t>(sprite.icon)), sprite.depth);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void shenTransformScreenCoors(const LegacyIconSprite &sprite, std::uint32_t colour) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.colour = colour;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateToInt32(sprite.x), TruncateToInt32(sprite.y), sprite.width,
        sprite.height, sprite.scaleX, sprite.scaleY, SPRITE_BITS(std::int32_t, sprite.rotation));
    packet.quad.flags |= sprite.flags | 0x400U;
    DrawLegacySpritePacket(packet, static_cast<std::int16_t>(sprite.icon), sprite.depth);
}
#endif

#else

// @verified - p
void shenTransformScreenCoors(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode) {
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = colour, .secondaryColour = 0, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    packet.quad.flags |= sprite.flags | 0x400U;
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int16_t>(sprite.icon)), sprite.depth);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AdjustScreenAspectRatio(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    switch (static_cast<std::int32_t>(alignment) / 4) {
    case 0:
        quad.corners[1] = quad.corners[3] = static_cast<float>(y);
        quad.corners[5] = quad.corners[7] = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(y) + height));
        break;
    case 1: {
        float *const topLeft = &quad.corners[1];
        float *const topRight = &quad.corners[3];
        const std::int32_t half = height / 2;
        *topLeft = *topRight = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(y) - half));
        topLeft[4] = topRight[4] = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(height) - half + y));
        break;
    }
    default:
        quad.corners[1] = quad.corners[3] = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(y) - height + 1U));
        quad.corners[5] = quad.corners[7] = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(y) + 1U));
        break;
    }
    switch (static_cast<std::int32_t>(alignment) & 3) {
    case 0:
        quad.corners[0] = quad.corners[4] = static_cast<float>(x);
        quad.corners[2] = quad.corners[6] = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(x) + width));
        break;
    case 1: {
        float *const topLeft = &quad.corners[0];
        float *const bottomLeft = &quad.corners[4];
        const std::int32_t half = width / 2;
        *topLeft = *bottomLeft = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(x) - half));
        topLeft[2] = bottomLeft[2] = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(width) - half + x));
        break;
    }
    default:
        quad.corners[0] = quad.corners[4] = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(x) - width + 1U));
        quad.corners[2] = quad.corners[6] = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(x) + 1U));
        break;
    }
#else
    const std::int32_t vertical = static_cast<std::int32_t>(alignment) / 4;
    std::int32_t top;
    std::int32_t bottom;
    if (!vertical) {
        top = y;
        bottom = static_cast<std::int32_t>(static_cast<std::uint32_t>(y) + static_cast<std::uint32_t>(height));
    } else if (vertical == 1) {
        const std::int32_t half = height / 2;
        top = static_cast<std::int32_t>(static_cast<std::uint32_t>(y) - static_cast<std::uint32_t>(half));
        bottom = static_cast<std::int32_t>((static_cast<std::uint32_t>(height) - static_cast<std::uint32_t>(half)) + static_cast<std::uint32_t>(y));
    } else {
        top = static_cast<std::int32_t>((static_cast<std::uint32_t>(y) - static_cast<std::uint32_t>(height)) + 1U);
        bottom = static_cast<std::int32_t>(static_cast<std::uint32_t>(y) + 1U);
    }
    quad.corners[1] = quad.corners[3] = static_cast<float>(top);
    quad.corners[5] = quad.corners[7] = static_cast<float>(bottom);
    const std::int32_t horizontal = static_cast<std::int32_t>(alignment) & 3;
    std::int32_t left;
    std::int32_t right;
    if (!horizontal) {
        left = x;
        right = static_cast<std::int32_t>(static_cast<std::uint32_t>(x) + static_cast<std::uint32_t>(width));
    } else if (horizontal == 1) {
        const std::int32_t half = width / 2;
        left = static_cast<std::int32_t>(static_cast<std::uint32_t>(x) - static_cast<std::uint32_t>(half));
        right = static_cast<std::int32_t>((static_cast<std::uint32_t>(width) - static_cast<std::uint32_t>(half)) + static_cast<std::uint32_t>(x));
    } else {
        left = static_cast<std::int32_t>((static_cast<std::uint32_t>(x) - static_cast<std::uint32_t>(width)) + 1U);
        right = static_cast<std::int32_t>(static_cast<std::uint32_t>(x) + 1U);
    }
    quad.corners[0] = quad.corners[4] = static_cast<float>(left);
    quad.corners[2] = quad.corners[6] = static_cast<float>(right);
#endif
}

#else

// @verified - p
void AdjustScreenAspectRatio(
    LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, ScreenMode mode) {
    const std::int32_t vertical = static_cast<std::int32_t>(alignment) / 4, horizontal = static_cast<std::int32_t>(alignment) & 3;
    std::uint32_t top = static_cast<std::uint32_t>(y), left = static_cast<std::uint32_t>(x);
    if (vertical)
        top -= vertical == 1 ? static_cast<std::uint32_t>(height / 2) : static_cast<std::uint32_t>(height) - 1U;
    if (horizontal)
        left -= horizontal == 1 ? static_cast<std::uint32_t>(width / 2) : static_cast<std::uint32_t>(width) - 1U;
    const float topY = static_cast<float>(std::bit_cast<std::int32_t>(top));
    const float bottomY = static_cast<float>(std::bit_cast<std::int32_t>(top + static_cast<std::uint32_t>(height)));
    const float leftX = static_cast<float>(std::bit_cast<std::int32_t>(left));
    const float rightX = static_cast<float>(std::bit_cast<std::int32_t>(left + static_cast<std::uint32_t>(width)));
    quad.corners = {leftX, topY, rightX, topY, leftX, bottomY, rightX, bottomY};
    if (ShouldScaleS2AspectRatio())
        return;
    const float adjustment = GetS2AspectRatioAdjustment();
    float offset = 0.0f;
    if (mode == ScreenMode::Centered || mode == ScreenMode::Adjusted || mode == ScreenMode::AdjustedWithOffset)
        offset = ((1.0f - adjustment) * 640.0f) * 0.5f;
    else if (mode == ScreenMode::Stretched)
        offset = (((1.0f - adjustment) * 640.0f) * 0.5f) * 2.0f;
    for (std::size_t index = 0; index < quad.corners.size(); index += 2)
        quad.corners[index] = offset + adjustment * quad.corners[index];
    if (mode == ScreenMode::Adjusted) {
        quad.corners[0] -= offset;
        quad.corners[4] -= offset;
        quad.corners[2] = (offset + 0.5f) + quad.corners[2];
        quad.corners[6] = (offset + 0.5f) + quad.corners[6];
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetScreenQuadBounds(LegacyScreenQuad &quad, std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom) {
    quad.corners[0] = static_cast<float>(left);
    quad.corners[1] = static_cast<float>(top);
    quad.corners[2] = static_cast<float>(right);
    quad.corners[3] = static_cast<float>(top);
    quad.corners[4] = static_cast<float>(left);
    quad.corners[5] = static_cast<float>(bottom);
    quad.corners[6] = static_cast<float>(right);
    quad.corners[7] = static_cast<float>(bottom);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void TransformScreenSpace(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
    float scaleX, float scaleY, std::int32_t rotation) {
    struct ScreenQuadPairs {
        std::uint32_t flags;
        float corners[4][2];
    };
    std::int32_t coordinates[4][2];
    float matrix[4];
    std::int32_t index;
    switch (static_cast<std::int32_t>(alignment) / 4) {
    case 0:
        coordinates[0][1] = coordinates[1][1] = 0;
        coordinates[2][1] = coordinates[3][1] = height;
        break;
    case 1:
        coordinates[2][1] = coordinates[3][1] = height / 2;
        coordinates[0][1] = coordinates[1][1] = coordinates[2][1] - height;
        break;
    default:
        coordinates[0][1] = coordinates[1][1] = 1 - height;
        coordinates[2][1] = coordinates[3][1] = 1;
        break;
    }
    switch (static_cast<std::int32_t>(alignment) & 3) {
    case 0:
        coordinates[0][0] = coordinates[2][0] = 0;
        coordinates[1][0] = coordinates[3][0] = width;
        break;
    case 1:
        coordinates[1][0] = coordinates[3][0] = width / 2;
        coordinates[0][0] = coordinates[2][0] = coordinates[1][0] - width;
        break;
    default:
        coordinates[0][0] = coordinates[2][0] = 1 - width;
        coordinates[1][0] = coordinates[3][0] = 1;
        break;
    }
    if (rotation) {
        matrix[0] = matrix[3] = dc::sdk::CosFixed(rotation);
        matrix[1] = dc::sdk::SinFixed(rotation);
        matrix[2] = -matrix[1];
        matrix[0] *= scaleX;
        matrix[1] *= scaleX;
        matrix[2] *= scaleY;
        matrix[3] *= scaleY;
        for (index = 0; index < 4; ++index) {
            reinterpret_cast<ScreenQuadPairs &>(quad).corners[index][0] =
                static_cast<float>(coordinates[index][0]) * matrix[0] + static_cast<float>(coordinates[index][1]) * matrix[2] + static_cast<float>(x);
            reinterpret_cast<ScreenQuadPairs &>(quad).corners[index][1] =
                static_cast<float>(coordinates[index][0]) * matrix[1] + static_cast<float>(coordinates[index][1]) * matrix[3] + static_cast<float>(y);
        }
    } else {
        for (index = 0; index < 4; ++index) {
            reinterpret_cast<ScreenQuadPairs &>(quad).corners[index][0] = static_cast<float>(coordinates[index][0]) * scaleX + static_cast<float>(x);
            reinterpret_cast<ScreenQuadPairs &>(quad).corners[index][1] = static_cast<float>(coordinates[index][1]) * scaleY + static_cast<float>(y);
        }
    }
}
#else
// @verified - d
void TransformScreenSpace(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
    float scaleX, float scaleY, std::int32_t rotation) {
    const std::int32_t vertical = static_cast<std::int32_t>(alignment) / 4;
    const std::int32_t bottom = !vertical ? height : vertical == 1 ? height / 2 : 1;
    const std::int32_t top = !vertical ? 0 : static_cast<std::int32_t>(static_cast<std::uint32_t>(bottom) - static_cast<std::uint32_t>(height));
    const std::int32_t horizontal = static_cast<std::int32_t>(alignment) & 3;
    const std::int32_t right = !horizontal ? width : horizontal == 1 ? width / 2 : 1;
    const std::int32_t left = !horizontal ? 0 : static_cast<std::int32_t>(static_cast<std::uint32_t>(right) - static_cast<std::uint32_t>(width));
    if (rotation) {
        const float cosine = cos_fixed16(static_cast<std::uint16_t>(rotation));
        const float sine = sin_fixed16(static_cast<std::int16_t>(rotation));
        const float xx = cosine * scaleX, xy = sine * scaleX, yx = -sine * scaleY, yy = cosine * scaleY;
        const std::int32_t coordinates[8] SPRITE_INIT{left, top, right, top, left, bottom, right, bottom};
        for (std::int32_t index = 0; index < 8; index += 2) {
            const float localX = static_cast<float>(coordinates[index]);
            const float localY = static_cast<float>(coordinates[index + 1]);
            float resultX = localY * yx;
            float resultY = localY * yy;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            resultX = localX * xx + resultX;
            resultY = localX * xy + resultY;
#else
            asm volatile("fmov %2,fr0\n\tfmac fr0,%3,%0\n\tfmac fr0,%4,%1" : "+f"(resultX), "+f"(resultY) : "f"(localX), "f"(xx), "f"(xy) : "fr0");
#endif
            quad.corners[index] = resultX + static_cast<float>(x);
            quad.corners[index + 1] = resultY + static_cast<float>(y);
        }
    } else {
        const std::int32_t coordinates[8] SPRITE_INIT{left, top, right, top, left, bottom, right, bottom};
        for (std::int32_t index = 0; index < 8; index += 2) {
            const float localX = static_cast<float>(coordinates[index]);
            const float localY = static_cast<float>(coordinates[index + 1]);
            float resultX = static_cast<float>(x);
            float resultY = static_cast<float>(y);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            resultX = localX * scaleX + resultX;
            resultY = localY * scaleY + resultY;
#else
            asm volatile("fmov %2,fr0\n\tfmac fr0,%3,%0\n\tfmov %4,fr0\n\tfmac fr0,%5,%1"
                : "+f"(resultX), "+f"(resultY)
                : "f"(localX), "f"(scaleX), "f"(localY), "f"(scaleY)
                : "fr0");
#endif
            quad.corners[index] = resultX;
            quad.corners[index + 1] = resultY;
        }
    }
}

#endif

void DrawLegacyTextureRectangle(std::uint32_t texture, std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom, float depth) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    SetScreenQuadBounds(packet.quad, left, top, right, bottom);
    DrawLegacySpritePacket(packet, texture, depth);
}

#else

// @verified - p
void TransformScreenSpace(LegacyScreenQuad &quad, ScreenAlignment alignment, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height,
    float scaleX, float scaleY, std::int32_t rotation, ScreenMode mode) {
    const std::int32_t vertical = static_cast<std::int32_t>(alignment) / 4;
    const std::int32_t bottom = vertical == 0 ? height : vertical == 1 ? height / 2 : 1;
    const std::int32_t top = vertical == 0 ? 0 : bottom - height;
    const std::int32_t horizontal = static_cast<std::int32_t>(alignment) & 3;
    const std::int32_t right = horizontal == 0 ? width : horizontal == 1 ? width / 2 : 1;
    const std::int32_t left = horizontal == 0 ? 0 : right - width;
    if (rotation != 0) {
        const std::uint16_t angle = static_cast<std::uint16_t>(rotation);
        const std::uint16_t folded = angle <= 32768U ? angle : static_cast<std::uint16_t>(-angle);
        const float cosine = SinScreenAngle(static_cast<std::uint16_t>(16384U - folded));
        const float sine = SinScreenAngle(angle);
        const float xx = cosine * scaleX, xy = sine * scaleX, yx = -sine * scaleY, yy = cosine * scaleY;
        quad.corners[0] = (static_cast<float>(left) * xx + static_cast<float>(top) * yx) + static_cast<float>(x);
        quad.corners[1] = (static_cast<float>(left) * xy + static_cast<float>(top) * yy) + static_cast<float>(y);
        quad.corners[2] = (static_cast<float>(right) * xx + static_cast<float>(top) * yx) + static_cast<float>(x);
        quad.corners[3] = (static_cast<float>(right) * xy + static_cast<float>(top) * yy) + static_cast<float>(y);
        quad.corners[4] = (static_cast<float>(left) * xx + static_cast<float>(bottom) * yx) + static_cast<float>(x);
        quad.corners[5] = (static_cast<float>(left) * xy + static_cast<float>(bottom) * yy) + static_cast<float>(y);
        quad.corners[6] = (static_cast<float>(right) * xx + static_cast<float>(bottom) * yx) + static_cast<float>(x);
        quad.corners[7] = (static_cast<float>(right) * xy + static_cast<float>(bottom) * yy) + static_cast<float>(y);
    } else {
        quad.corners = {static_cast<float>(left) * scaleX + static_cast<float>(x), static_cast<float>(top) * scaleY + static_cast<float>(y),
            static_cast<float>(right) * scaleX + static_cast<float>(x), static_cast<float>(top) * scaleY + static_cast<float>(y),
            static_cast<float>(left) * scaleX + static_cast<float>(x), static_cast<float>(bottom) * scaleY + static_cast<float>(y),
            static_cast<float>(right) * scaleX + static_cast<float>(x), static_cast<float>(bottom) * scaleY + static_cast<float>(y)};
    }
    if (ShouldScaleS2AspectRatio())
        return;
    const float adjustment = GetS2AspectRatioAdjustment();
    float offset = 0.0f;
    if (mode == ScreenMode::Centered || mode == ScreenMode::Adjusted || mode == ScreenMode::AdjustedWithOffset)
        offset = ((1.0f - adjustment) * 640.0f) * 0.5f;
    else if (mode == ScreenMode::Stretched)
        offset = (((1.0f - adjustment) * 640.0f) * 0.5f) * 2.0f;
    for (std::size_t i = 0; i < quad.corners.size(); i += 2)
        quad.corners[i] = offset + adjustment * quad.corners[i];
    if (mode == ScreenMode::Adjusted) {
        quad.corners[0] -= offset;
        quad.corners[4] -= offset;
        quad.corners[2] = (offset + 0.5f) + quad.corners[2];
        quad.corners[6] = (offset + 0.5f) + quad.corners[6];
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
float ConvertLegacyScreenDepth(float depth) {
    if (depth < 0.0f) {
        depth *= -1.0f;
        if (depth < 1.0f)
            depth = 1.0f;
        return depth;
    }
    return 1.0f / (depth + 100.0f);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void LoadAreaSpriteImages() {
    if (AllocateTempSprites(2) == 1)
        BindTempSprite(2, 0, 37, titleFileSelectRectangles);
}

void DrawFileSelectFrame() {
    DrawTempSpriteSlot(0x25001bU, (160U << 16U) | 59U, 1.2f, 0);
    DrawTempSpriteSlot(0x25001cU, (480U << 16U) | 59U, 1.2f, 0);
    SetLegacySpriteSlotSizeOffsets(4, 8);
    DrawTempSpriteSlot(0x250023U, (511U << 16U) | 227U, 1.2f, 0);
    SetLegacySpriteSlotSizeOffsets(0, 0);
}

std::int32_t TitleSaveLoadAction(std::uint32_t flags) {
    std::int32_t action;
    if ((flags & 1U) == 0)
        action = 12;
    else
        action = flags & 0x90U ? 15 : 13;
    return action;
}

std::int32_t TitleSaveWriteAction(std::uint32_t flags) {
    std::int32_t action;
    if ((flags & 1U) == 0)
        action = 17;
    else if (flags & 8U) {
        titleActiveSaveSelection->result = 0;
        action = 5;
    } else
        action = flags & 2U ? 18 : 19;
    return action;
}

std::int32_t TitleSaveOverwriteAction(std::uint32_t flags) {
    if ((flags & 1U) == 0)
        return 27;
    if (flags & 2U)
        return 28;
    if (flags & 0x10U) {
        titleActiveSaveSelection->result = 1;
        return 7;
    }
    return 28;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetTitleMenuAction();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
TitleSaveSelectionPrefix titleSaveSelection;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
TitleSaveSelectionPrefix *titleActiveSaveSelection = &titleSaveSelection;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
BackupHotplugData *titleSaveBackupData = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
BackupListenerNode *titleSaveBackupListener = 0;
TitleSaveStateRule titleSaveTransitionRules[20] = {{13, 13, -1, -1, 40}, {14, 14, 16, 16, 1}, {3, 4, 16, 16, 4}, {1, 3, 16, 16, 30}, {4, 4, 16, 16, 8},
    {20, 20, 22, 22, 5}, {15, 15, 22, 22, 11}, {15, 15, 22, 22, 7}, {21, 1, 22, 22, 9}, {15, 15, 22, 22, 10}, {1, 1, 22, 22, 8}, {15, 15, 23, 23, 8},
    {2, 5, 23, 23, 14}, {4, 4, 16, 16, 3}, {8, 8, 26, 26, 8}, {8, 8, 26, 26, 28}, {6, 6, 26, 26, 12}, {9, 7, 26, 26, 38}, {9, 7, 26, 26, 39},
    {26, 26, 26, 26, 32}};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t titleSaveTransitionTimer = 0, titleSaveState = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t titleSaveWarningParameters[7];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void TitleSaveSelectionResetNoOp() {
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ShowTitleLoadConfirmation() {
    std::uint8_t parameters[7];
    parameters[0] = 1;
    parameters[1] = titleSaveSelection.retained03;
    SetTitleWarningLogo(38, parameters);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetTitleSaveBackupPort() {
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t TitleFileSelectCanUseSave(const BackupSlotMetadata *metadata) {
    if (!metadata->saveKind)
        return false;
    if (GetTitleMenuAction() == 2)
        return reinterpret_cast<const std::uint8_t *>(metadata)[0x24] == 6;
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InitializeTitleFileSelectState() {
    titleSaveSelection.availableCount = 0;
    titleSaveSelection.selectedSlot = -1;
    titleSaveSelection.retained03 = 0;
    TitleSaveSelectionResetNoOp();
    titleSaveSelection.result = -1;
    BackupSlotMetadata *metadata;
    if (GetTitleMenuAction() == 2)
        metadata = titleSaveBackupData->imported;
    else
        metadata = titleSaveBackupData->saves;
    for (std::int16_t index = 0; index < 3; ++index, ++metadata) {
        titleSaveSelection.slots[index].flags = 0;
        titleSaveSelection.slots[index].metadata = metadata;
        if (metadata->saveKind) {
            titleSaveSelection.slots[index].flags |= 1U;
            if (reinterpret_cast<const std::uint8_t *>(metadata)[0x24] == 6)
                titleSaveSelection.slots[index].flags |= 2U;
            else if (reinterpret_cast<const std::uint8_t *>(metadata)[0x24] == 4)
                titleSaveSelection.slots[index].flags |= 4U;
            if (TitleFileSelectCanUseSave(metadata)) {
                if (!titleSaveSelection.availableCount)
                    titleSaveSelection.selectedSlot = static_cast<std::int8_t>(index);
                ++titleSaveSelection.availableCount;
            }
        }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void TitleSaveHotplugCallback(void *data) {
    titleSaveBackupData = static_cast<BackupHotplugData *>(data);
    const std::uint32_t flags = GetBackupInfo(GetTitleSaveBackupPort());
    InitializeTitleFileSelectState();
    if (titleSaveState > 11) {
        const TitleSaveStateRule &rule = titleSaveTransitionRules[titleSaveState - 12];
        if (flags & 1U) {
            if (rule.connectedState >= 0)
                titleSaveState = rule.connectedState;
        } else if (rule.disconnectedState >= 0)
            titleSaveState = rule.disconnectedState;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t SelectTitleSaveSlot(BackupSlotMetadata *metadata) {
    const std::uint32_t flags = GetBackupInfo(0);
    const std::uint16_t buttons = g_inputBuffer[0].pressedPriority;
    const std::int8_t previous = titleActiveSaveSelection->selectedSlot;
    std::int32_t state;
    if ((flags & 1U) == 0)
        state = 26;
    else if (buttons & 0x200U) {
        titleActiveSaveSelection->phase = 0;
        state = 8;
    } else {
        if (buttons & 0x2000U) {
            do {
                if (++titleActiveSaveSelection->selectedSlot > 2)
                    titleActiveSaveSelection->selectedSlot = 0;
            } while (!TitleFileSelectCanUseSave(metadata + titleActiveSaveSelection->selectedSlot));
        } else if (buttons & 0x1000U) {
            do {
                if (--titleActiveSaveSelection->selectedSlot < 0)
                    titleActiveSaveSelection->selectedSlot = 2;
            } while (!TitleFileSelectCanUseSave(metadata + titleActiveSaveSelection->selectedSlot));
        }
        if (buttons & 0x400U) {
            if (titleActiveSaveSelection->slots[titleActiveSaveSelection->selectedSlot].flags & 2U) {
                TITLE_SAVE_AICA_COMMAND(0x800ab, 0, 0);
                state = 30;
            } else {
                TITLE_SAVE_AICA_COMMAND(0x900ab, 0, 0);
                titleActiveSaveSelection->phase = 1;
                titleSaveTransitionTimer = 0;
                state = 29;
            }
        } else {
            if (previous != titleActiveSaveSelection->selectedSlot)
                TITLE_SAVE_AICA_COMMAND(0x200ab, 0, 0);
            state = titleSaveState;
        }
    }
    return state;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t SelectImportedTitleSaveSlot(BackupSlotMetadata *metadata) {
    const std::uint32_t flags = GetBackupInfo(0);
    std::uint16_t buttons = g_inputBuffer[0].pressedPriority;
    std::int32_t state;
    if ((flags & 1U) == 0) {
        titleActiveSaveSelection->result = -1;
        state = 23;
    } else {
        if (titleActiveSaveSelection->availableCount < 1)
            buttons &= 0xcfffU;
        if (g_inputBuffer[0].pressedPriority & 0x200U) {
            TITLE_SAVE_AICA_COMMAND(0x800ab, 0, 0);
            titleActiveSaveSelection->result = -1;
            state = 15;
        } else {
            const std::int32_t previous = titleActiveSaveSelection->selectedSlot;
            if (buttons & 0x2000U) {
                do {
                    if (++titleActiveSaveSelection->selectedSlot > 2)
                        titleActiveSaveSelection->selectedSlot = 0;
                } while (!TitleFileSelectCanUseSave(metadata + titleActiveSaveSelection->selectedSlot));
            } else if (buttons & 0x1000U) {
                do {
                    if (--titleActiveSaveSelection->selectedSlot < 0)
                        titleActiveSaveSelection->selectedSlot = 2;
                } while (!TitleFileSelectCanUseSave(metadata + titleActiveSaveSelection->selectedSlot));
            }
            if (buttons & 0x400U) {
                TITLE_SAVE_AICA_COMMAND(0x900ab, 0, 0);
                titleActiveSaveSelection->phase = 1;
                titleSaveTransitionTimer = 0;
                state = 24;
            } else {
                if ((std::int8_t)previous != titleActiveSaveSelection->selectedSlot)
                    TITLE_SAVE_AICA_COMMAND(0x200ab, 0, 0);
                state = titleSaveState;
            }
        }
    }
    return state;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void StartSelectedTitleSave() {
    if (titleSaveBackupData) {
        const BackupSlotMetadata &save = titleSaveBackupData->saves[titleActiveSaveSelection->selectedSlot];
        g_actNumber = save.sceneNumber;
        g_currentMap = save.mapName;
        g_entryPoint = save.entryPoint;
        g_gamePauseState = 0;
        LoadTitleSoundProgram_Wrapper(-1);
        ApplyTitleMenuOptions();
        ChangeSystemModule(SystemModule_Game, 0);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ImportTitleSaveData(std::int32_t inherited, const void *bank, std::int32_t slot) {
    if (!inherited)
        InitializeDefaultSaveData();
    ImportTitleSaveSlot(bank, slot);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t ReadTitleInheritedSave(void *image) {
    const BackupFileHeader *const header = reinterpret_cast<const BackupFileHeader *>(static_cast<std::byte *>(image) + 0x680);
    std::int32_t result = WaitForBackupOperationPort0();
    if (!result)
        result = LoadImportedSaveFile(image);
    if (!result)
        result = WaitForBackupOperationPort0();
    if (!result && !ValidateBackupSaveImage(image))
        result = -6;
    if (!result && !ValidateImportedSaveHeader(header))
        result = -6;
    SetScenarioSaveRestoreState(result == 0);
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t UpdateTitleSaveChoice(std::uint8_t *choice, std::uint16_t buttons) {
    const std::uint8_t previous = *choice;
    std::int32_t result = -1;
    if (buttons & 0x4000U)
        *choice = 1;
    if (buttons & 0x8000U)
        *choice = 0;
    if (buttons & 0x400U)
        result = *choice != 1;
    else if (buttons & 0x200U)
        result = 1;
    else if (previous != *choice)
        TITLE_SAVE_AICA_COMMAND(0x200ab, 0, 0);
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ControlTitleFileSelect(void *data) {
    TitleControlData *const control = static_cast<TitleControlData *>(data);
    const TitleSaveStateRule *rule = 0;
    const std::uint16_t buttons = g_inputBuffer[0].pressedPriority;
    const std::uint32_t flags = GetBackupInfo(0);
    if (titleSaveState > 11)
        rule = &titleSaveTransitionRules[titleSaveState - 12];
    switch (titleSaveState) {
    case 0:
        titleSaveState = TitleSaveLoadAction(flags);
        break;
    case 1:
        titleSaveState = TitleSaveWriteAction(flags);
        break;
    case 6:
        titleSaveState = TitleSaveOverwriteAction(flags);
        break;
    case 3:
        if (titleActiveSaveSelection->phase)
            ImportTitleSaveData(titleActiveSaveSelection->inherited, control->inheritedSaveBank, titleActiveSaveSelection->selectedSlot);
        else {
            ImportTitleSaveData(titleActiveSaveSelection->inherited, control->inheritedSaveBank, -1);
            TITLE_SAVE_AICA_COMMAND(0x900ab, 0, 0);
        }
        ExecuteTitleMenuCallback(control, 4);
        TITLE_SAVE_AICA_COMMAND(0x1200a0, 2, 0);
        break;
    case 2:
        if (flags & 1U) {
            if (!ReadTitleInheritedSave(control->inheritedSaveBank)) {
                titleSaveState = 3;
                titleActiveSaveSelection->inherited = 1;
            }
        } else
            titleSaveState = 25;
        break;
    case 4:
    case 8:
        HPLG_UnregisterCallback(titleSaveBackupListener);
        titleSaveBackupListener = 0;
        ExecuteTitleMenuCallback(control, 0);
        break;
    case 5:
        titleSaveState = SelectImportedTitleSaveSlot(titleSaveBackupData->imported);
        break;
    case 7:
        titleSaveState = SelectTitleSaveSlot(titleSaveBackupData->saves);
        break;
    case 9:
        if (LoadGameFromSaveSlot(titleActiveSaveSelection->selectedSlot, 1))
            titleSaveState = 10;
        else
            titleSaveState = 31;
        break;
    case 10:
        titleSaveState = 11;
        break;
    case 11:
        if (flags & 1U)
            StartSelectedTitleSave();
        else
            titleSaveState = 31;
        break;
    case 12:
    case 13:
    case 16:
    case 17:
    case 18:
    case 19:
    case 21:
    case 22:
    case 23:
    case 25:
    case 26:
    case 27:
    case 30:
    case 31:
        SetTitleWarningLogo(rule->warning, 0);
        if (buttons & 0x600U) {
            titleSaveState = rule->nextState;
            titleSaveWarningParameters[1] = 0;
            SetTitleWarningLogo(0, 0);
        }
        break;
    case 14:
    case 20:
    case 24:
    case 29:
        switch (UpdateTitleSaveChoice(titleSaveWarningParameters + 1, buttons)) {
        case 0:
            SetTitleWarningLogo(0, 0);
            titleSaveState = rule->nextState;
            titleSaveWarningParameters[1] = 0;
            TITLE_SAVE_AICA_COMMAND(0x900ab, 0, 0);
            break;
        case 1:
            SetTitleWarningLogo(0, 0);
            titleSaveState = rule->alternateState;
            titleSaveWarningParameters[1] = 0;
            TITLE_SAVE_AICA_COMMAND(0x800ab, 0, 0);
            break;
        default:
            SetTitleWarningLogo(rule->warning, titleSaveWarningParameters);
            break;
        }
        break;
    case 15:
        switch (UpdateTitleSaveChoice(titleSaveWarningParameters + 1, buttons)) {
        case 0:
            SetTitleWarningLogo(0, 0);
            titleSaveState = rule->nextState;
            titleSaveWarningParameters[1] = 0;
            TITLE_SAVE_AICA_COMMAND(0x900ab, 0, 0);
            break;
        case 1:
            SetTitleWarningLogo(0, 0);
            if ((buttons & 0x600U) == 0x200U)
                titleSaveState = 4;
            else
                titleSaveState = rule->alternateState;
            titleSaveWarningParameters[1] = 0;
            break;
        default:
            SetTitleWarningLogo(rule->warning, titleSaveWarningParameters);
            break;
        }
        break;
    }
}
#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#undef SPRITE_NULL
#undef SPRITE_ZERO
#undef SPRITE_INIT
#undef SPRITE_ARRAY
#undef SPRITE_DATA
#undef SPRITE_MEMCPY
#undef SPRITE_ALIGN
#undef SPRITE_MODE
#undef SPRITE_BITS
#undef TITLE_SAVE_AICA_COMMAND
#else
#endif
