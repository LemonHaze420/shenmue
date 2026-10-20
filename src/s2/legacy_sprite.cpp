#include "s2/legacy_sprite.hpp"

#include "s2/d3t_renderer.hpp"
#include "s2/title_warning.hpp"
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

namespace shenmue::s2 {
std::array<std::int32_t, 21> titleSpriteTextures{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
std::int32_t titleGTextTexture = -1, titleCTextTexture = -1, titleKeyImageTexture = -1, titleContinueTexture = -1, titleEsrbTexture = -1;
float legacySpriteSlotScaleX{}, legacySpriteSlotScaleY{}, legacySpriteSlotPulse{};
std::uint32_t legacySpriteSlotColour{}, legacySpriteSlotSecondaryColour{};
std::int32_t legacySpriteSlotWidthOffset{}, legacySpriteSlotHeightOffset{}, legacySpriteSlotRotation{};
const std::uint32_t *titleConfirmChoice{};
std::array<TempTextureEntry *, 39> g_tempTextures{};
namespace {
std::array<BorrowedLegacySpriteSlot, 8> borrowedSpriteSlots{};

TempTextureWork titleCommonTempWork{};
LegacySpritePacket titleSpritePacket{};
std::array<LegacyIconSprite, 4> itemWindowEdges;
LegacySpritePacket itemWindowTiles;
LegacySpritePacket itemWindowSeparators;
LegacyIconSprite itemWindowSelection;

struct SDTextureMapping {
    std::array<char, 260> source{};
    std::array<char, 260> destination{};
};
static_assert(sizeof(SDTextureMapping) == 520);
std::vector<SDTextureMapping> sdTextureMappings;
const char *spriteTexturePath{};
std::array<std::int32_t, 5> collectionTextures{-1, -1, -1, -1, -1};
std::array<shen_name, 2> collectionSpriteOwners{};
std::array<LegacyIconSprite, 8> quantityDigits{};
std::array<LegacyIconSprite, 2> quantityBrackets{};
bool spriteAreaConfigurationsSelected{};
std::array<std::int32_t, 4> spriteBufferCapacities{};
std::array<std::uint32_t, 5> spriteWork{};
std::uint32_t spriteBufferWork{};
std::uint32_t spriteWorkInitialized{};
std::uint32_t unusedReloadWord{};
std::uint64_t unusedReloadWide0{};
std::uint64_t unusedReloadWide1{};

void RetireImagePrefetch(void *, std::uint32_t handle, void *, std::uint64_t, const char *) {
    d3t::ProcessThreadStates(handle, 0);
}

void ReplaceSpriteImage(void *data, std::uint32_t handle, void *bytes, std::uint64_t size, const char *) {
    d3t::ReplaceDecodedImage(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(data)), bytes, size, false);
    d3t::ProcessThreadStates(handle, 0);
}

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
}
// @verified - p
std::int64_t ResetUnusedReloadState() {
    unusedReloadWide0 = 0;
    unusedReloadWord = 0;
    unusedReloadWide1 = 0;
    return 0;
}

// @verified - p
std::int64_t ReloadSpriteResources() {
    nullsub();
    DestroyEffectTextureReferences();
    InitializeSprites(1, 1);
    return ResetUnusedReloadState();
}

// @verified - p
void SetSpriteAreaConfigurations() {
    spriteAreaConfigurationsSelected = true;
}

// @verified - p
std::uint32_t InitializeSpriteBuffersNullsub(std::int32_t, std::int32_t, std::int32_t, std::int32_t) {
    return 0xffffffffU;
}

// @verified - p
std::uint32_t EnsureSpriteWorkInitialized() {
    if (!spriteWorkInitialized)
        spriteWorkInitialized = 1;
    return spriteWorkInitialized;
}

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
        LoadPak();
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

std::int32_t TruncateSpriteCoordinate(float value) noexcept {
    return value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : std::numeric_limits<std::int32_t>::min();
}

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

void PrepareDrawSimple(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode) {
    const std::array<std::uint8_t, 4> uv = GetCommonIconRect(sprite.icon);
    const CommonIconAtlas *const atlas = GetCommonIconAtlas(sprite.icon);

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

void PrepareDrawWithMode(const LegacyIconSprite &sprite, ScreenMode mode) {
    PrepareDrawSimple(sprite, 0xff000000U, mode);
}

// @verified - p
void PrepareDrawSimpleOpaque(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode) {
    PrepareDrawSimple(sprite, colour | 0xff000000U, mode);
}

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

void DrawLegacyItemWindow(const Vector2 &position, float depth, std::int32_t columns, std::int32_t rows, ScreenMode mode) {
    const float width = static_cast<float>(columns) * 40.0f, height = static_cast<float>(rows) * 40.0f;
    const float x = position.x - width * 0.5f, y = position.y - height * 0.5f;
    DrawLegacyWindowBackground(x, y, width, height, depth - 0.0099999998f, 0, mode);
    DrawLegacyWindowTiles(x + 20.0f, y + 20.0f, columns, rows, depth - 0.02f, 0, mode);
    DrawLegacyWindowEdges(x - 4.0f, y + 23.0f, rows - 1, columns, depth, 0, mode);
    DrawLegacyWindowHorizontal(x - 15.0f, y - 4.0f, columns, 1, 0, depth, 0, mode);
    DrawLegacyWindowHorizontal(x - 15.0f, (y + height) + 4.0f, columns, 1, 1, depth, 0, mode);
}

// @verified - p
void PrepareDrawIndexedTexture(const LegacyIconSprite &sprite, ScreenMode mode) {
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = 0xffffffffU, .secondaryColour = 0, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    packet.quad.flags |= sprite.flags;
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int16_t>(sprite.icon)), sprite.depth);
}

// @verified - p
void shenTransformScreenCoors(const LegacyIconSprite &sprite, std::uint32_t colour, ScreenMode mode) {
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = colour, .secondaryColour = 0, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    packet.quad.flags |= sprite.flags | 0x400U;
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int16_t>(sprite.icon)), sprite.depth);
}

// @verified - p
void PrepareDrawIndexedTextureWithSecondary(const LegacyIconSprite &sprite, std::uint32_t secondaryColour, ScreenMode mode) {
    LegacySpritePacket packet{
        .quad = {.flags = 0}, .colour = 0xffffffffU, .secondaryColour = secondaryColour, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    packet.quad.flags |= sprite.flags | 0x400U;
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int16_t>(sprite.icon)), sprite.depth);
}

void PrepareDrawTextureWithMode(const LegacyIconSprite &sprite, ScreenMode mode) {
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = 0xffffffffU, .secondaryColour = 0, .uv = {0.0f, 0.0f, 1.0f, 1.0f}, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    packet.quad.flags |= sprite.flags | 0x400U;
    LegacySpriteNullsub(packet, sprite.icon, sprite.depth);
}

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

// @verified - p
bool DrawLegacySolidRect(const LegacySolidRect &rect) {
    LegacyColourQuad packet{.quad = {.flags = 0}, .colour = rect.colour};
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(rect.x), TruncateSpriteCoordinate(rect.y), rect.width,
        rect.height, ScreenMode::Centered);
    packet.quad.flags |= 1024;
    return shenSubmitQuad(packet, rect.depth);
}
// @verified - p
bool DrawLegacyColourRect(const LegacyColourRect &rect) {
    LegacyColourQuad packet{.quad = {.flags = 0}, .colour = rect.colour};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(rect.alignment), TruncateSpriteCoordinate(rect.x), TruncateSpriteCoordinate(rect.y),
        rect.width, rect.height, rect.scaleX, rect.scaleY, rect.rotation, ScreenMode::Centered);
    packet.quad.flags |= 0x400U;
    return shenSubmitQuad(packet, rect.depth);
}
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

void DrawLegacyWindowBackground(float x, float y, float width, float height, float depth, std::int32_t flags, ScreenMode mode) {
    LegacyColourQuad packet{.quad = {.flags = 8}, .colour = 0x05000020U, .alternateColours = {0x05000020U, 0xbe000000U, 0xbe000000U}};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(0), TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(y),
        TruncateSpriteCoordinate(width), TruncateSpriteCoordinate(height), 1, 1, 0, mode);

    static_cast<void>(depth);
    static_cast<void>(flags);
}

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

bool vfSubmitQuad(std::uint32_t colour, float depth) {
    LegacyColourQuad packet{.quad = {.flags = 0}, .colour = colour};
    TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, 320, 240, 640, 480, 1.0f, 1.0f, 0, ScreenMode::Adjusted);
    packet.quad.flags |= 0x400U;
    return shenSubmitQuad(packet, depth);
}

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


// @verified - p
bool RegisterBorrowedLegacySpriteSlot(std::int16_t texture, std::int32_t width, std::int32_t height, std::uint16_t slot, const std::uint16_t *rectangles) {
    BorrowedLegacySpriteSlot *entry = nullptr;
    for (BorrowedLegacySpriteSlot &candidate : borrowedSpriteSlots) {
        if (candidate.rectangles && candidate.slot == slot) {
            entry = &candidate;
            break;
        }
    }
    if (!entry) {
        for (BorrowedLegacySpriteSlot &candidate : borrowedSpriteSlots) {
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

// @verified - p
void ReleaseBorrowedLegacySpriteSlot(std::uint16_t slot) {
    for (BorrowedLegacySpriteSlot &entry : borrowedSpriteSlots) {
        if (!entry.rectangles || entry.slot != slot)
            continue;
        entry.rectangles = nullptr;
        entry.slot = 0;
        entry.texture = -1;
        return;
    }
}

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

// @verified - p
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

// @verified - p
void DrawTitleSelection(std::int32_t x, std::int32_t y, std::int32_t separation, std::int32_t choice) {
    titleConfirmChoice = TITLE_CONFIRM_CHOICES[choice].data();
    DrawTempSpriteSlot(
        (titleConfirmChoice[0] << 16U) | titleConfirmChoice[1], (static_cast<std::uint32_t>(x - separation) << 16U) | static_cast<std::uint16_t>(y), 13.0f, 16);
    titleConfirmChoice += 2;
    DrawTempSpriteSlot(
        (titleConfirmChoice[0] << 16U) | titleConfirmChoice[1], (static_cast<std::uint32_t>(x + separation) << 16U) | static_cast<std::uint16_t>(y), 13.0f, 16);
}

void SetSpriteTexturePath(const char *path) noexcept {
    spriteTexturePath = path;
}

void ApplySpriteTextureOverride(const void *texn, std::uint32_t textureHandle) {
    if (!spriteTexturePath || !*spriteTexturePath)
        return;
    char name[9]{};
    std::memcpy(name, static_cast<const std::byte *>(texn) + 8, 8);
    char source[260]{};
    std::snprintf(source, sizeof(source), "%s%s.png", spriteTexturePath, name);
    FilepathCleaner(source);
    for (const SDTextureMapping &mapping : sdTextureMappings) {
        if (std::strcmp(mapping.source.data(), source) != 0)
            continue;
        char destination[272]{};
        std::snprintf(destination, sizeof(destination), "%s", mapping.destination.data());
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
        return;
    }
}

std::int64_t QueueSDTextureOverride(const char *path, std::uint32_t renderHandle) {
    const d3t::D3TShaderLoadTask request{.path = path, .renderHandle = renderHandle, .thread = {SDTextureOverride, nullptr}};
    return d3t::PushShaderLoadToThreadQueue(&request);
}

void ReleaseSDTextureOverrides() {
    std::vector<SDTextureMapping>{}.swap(sdTextureMappings);
    spriteTexturePath = nullptr;
}

void ResetLegacyCollectionSpriteOwners() noexcept {
    collectionSpriteOwners = {};
}

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

void FreeTempSprites(std::int32_t index) {
    TempTextureEntry *&entries = g_tempTextures[index];
    if (!entries)
        return;
    for (std::int64_t texture = TEMP_TEXTURE_DEFINITIONS[index].textureCount - 1; texture >= 0; --texture)
        ReleaseNamedTexture(entries[texture].texture);
    HLib::FreeTaskData(entries);
    entries = nullptr;
}

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

// @verified - p
bool BindTempSprite(std::int32_t index, std::int32_t texture, std::uint16_t slot, const std::uint16_t *rectangles) {
    if (!g_tempTextures[index])
        return false;
    const TempTextureWork &work = GetTempSpriteTexture(index, texture);
    return RegisterBorrowedLegacySpriteSlot(work.texture, work.width, work.height, slot, rectangles);
}
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

// @verified - p
std::int32_t InitializeTitleCommonTempSprites() {
    if (AllocateTempSprites(0) != 1)
        return 0;
    BindTempSprite(0, 0, 36, TITLE_COMMON_TEMP_RECTANGLES.data());

    return BindTempSprite(0, 1, 37, TITLE_COMMON_TEMP_RECTANGLES.data()) ? 1 : 0;
}

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

} // namespace shenmue::s2
