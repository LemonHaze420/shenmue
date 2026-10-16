#include "s1/modules/harrier_render.hpp"

#include "logging.hpp"

#include "d3t/image_texture_loader.hpp"
#include "s1/archive_data.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/fvf_layout.hpp"
#include "s1/input.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/modules/harrier.hpp"
#include "s1/modules/harrier_cpu.hpp"
#include "s1/mt5_render.hpp"
#include "s1/shader_initialization.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <vector>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#endif

namespace shenmue::s1 {

std::array<std::uint32_t, kHarrierSpriteTextureCount> g_harrierSpriteTextures{};
std::array<std::uint32_t, kHarrierTileTextureCount> g_harrierTileTextures{};
std::uint32_t g_harrierFillTexture = 0;
std::uint32_t g_harrierPatternTexture = 0;
std::uint32_t g_harrierDebugFontTexture = 0;

namespace {

constexpr std::size_t kHarrierGraphicsBytes = 0x240000;
constexpr std::size_t kHarrierBackgroundGraphicsBytes = 0x40000;
constexpr std::size_t kHarrierObjectGraphicsBytes = 0x200000;
constexpr std::size_t kHarrierGraphicTileCount = kHarrierBackgroundGraphicsBytes / 64;
constexpr std::size_t kHarrierTilePaletteCount = 128;
constexpr std::size_t kHarrierSpritePaletteCount = 64;
constexpr std::size_t kHarrierSpriteScratchPixels = 0x10000;
constexpr std::size_t kHarrierTilePixels = 32 * 32;
constexpr std::size_t kHarrierMapWords = 32 * 64;
constexpr std::size_t kHarrierVisibleLines = 224;
constexpr std::size_t kHarrierSpriteCount = 64;
constexpr std::size_t kHarrierSpriteCachePoolStart = 64;
constexpr std::size_t kHarrierDebugColumns = 53;
constexpr std::size_t kHarrierDebugRows = 40;
constexpr std::size_t kHarrierAveragePaletteRefsOffset = 0xb0070;
constexpr std::size_t kHarrierLinePatternOffset = 0xe0200;
constexpr std::size_t kHarrierLinePaletteOffset = 0xe0401;
constexpr std::size_t kHarrierLineFlagsOffset = 0xe0601;
constexpr std::size_t kHarrierLayerPageSelectOffset = 0x98e9c;
constexpr std::size_t kHarrierLayerScrollXOffset = 0x98ff8;
constexpr std::size_t kHarrierLayerScrollYOffset = 0x98f25;
constexpr std::uint32_t kTransparentKey = 0x00818181;
constexpr std::uint32_t kOpaqueBlack = 0xff000000;
constexpr std::uint32_t kHarrierTextureFormat = 0x26;
constexpr std::uint32_t kPauseOverlayColour = 0x68000000;

struct HarrierPositionColourVertex {
    float x;
    float y;
    float z;
    float w;
    std::uint32_t colour;
};

struct HarrierPositionColourTextureVertex {
    float x;
    float y;
    float z;
    float w;
    std::uint32_t colour;
    float u;
    float v;
};

struct HarrierPositionTextureVertex {
    float x;
    float y;
    float z;
    float w;
    float u;
    float v;
};

static_assert(sizeof(HarrierPositionColourVertex) == 0x14);
static_assert(sizeof(HarrierPositionColourTextureVertex) == 0x1c);
static_assert(sizeof(HarrierPositionTextureVertex) == 0x18);

std::array<std::uint8_t, kHarrierGraphicsBytes> g_harrierGraphics{};
std::array<bool, kHarrierGraphicTileCount> g_harrierGraphicTileOccupied{};
std::array<std::uint32_t, kHarrierPaletteColourCount> g_harrierPaletteLookup{};
std::array<std::array<std::uint32_t, 8>, kHarrierTilePaletteCount> g_harrierTilePalettes{};
std::array<std::array<std::uint32_t, 16>, kHarrierSpritePaletteCount> g_harrierSpritePalettes{};
std::array<bool, kHarrierTilePaletteCount> g_harrierTilePaletteDirty{};
std::array<bool, kHarrierSpritePaletteCount> g_harrierSpritePaletteDirty{};
std::array<HarrierSpriteCacheEntry, kHarrierSpriteTextureCount> g_harrierSpriteCache{};
std::array<std::array<std::uint32_t, kHarrierTilePixels>, kHarrierTileTextureCount> g_harrierTilePixels{};
std::array<std::uint16_t, kHarrierTileTextureCount> g_harrierTileOccupancy{};
std::array<std::uint32_t, kHarrierSpriteScratchPixels> g_harrierSpritePixels{};
std::array<bool, kHarrierTileTextureCount> g_harrierTileTextureDirty{};
std::array<std::array<std::uint16_t, kHarrierMapWords>, 3> g_harrierTileMaps{};
std::array<std::array<std::uint16_t, kHarrierMapWords>, 3> g_harrierPreviousTileMaps{};
std::array<std::array<char, kHarrierDebugColumns>, kHarrierDebugRows> g_harrierDebugText{};
std::array<std::uint32_t, 16> g_harrierDebugColours{0xff404040, 0xff0000ff, 0xffff0000, 0xffff00ff, 0xff00ff00, 0xff00ffff, 0xffffff00, 0xffffffff, 0xff008080,
    0xffa02000, 0xffff8000, 0xffff20a0, 0xff000000, 0xff505050, 0xffa0a0a0, 0xfff0f0f0};
std::uint32_t g_harrierAverageColour = kTransparentKey;
std::uint32_t g_harrierPreviousAverageColour = kTransparentKey;
std::uint32_t g_harrierSpriteCacheCursor = 0;
std::uint32_t g_harrierDebugCursor = 0;
std::uint32_t g_harrierPdShaderLoad = 0;
std::uint32_t g_harrierPdtShaderLoad = 0;
std::uint32_t g_harrierPtShaderLoad = 0;
bool g_harrierResourcesLoaded = false;

std::uint16_t ReadBigEndianWord(const std::uint8_t *bytes) {
    return static_cast<std::uint16_t>((static_cast<std::uint16_t>(bytes[0]) << 8U) | bytes[1]);
}

std::uint16_t HarrierMemoryWord(std::size_t offset) {
    return ReadBigEndianWord(g_harrierMemory + offset);
}

bool ReadHarrierAsset(const char *path, std::uint8_t *destination, std::size_t capacity) {
    const std::uint64_t fileSize = ArchiveData_GetFileSize(path);
    if (!fileSize || fileSize > capacity) {
        return false;
    }
    std::memset(destination, 0, capacity);
    if (!ArchiveData_Read(path, 0, destination, fileSize)) {
        return false;
    }
    return true;
}

std::uint32_t LoadHarrierImage(const char *path) {
    const std::uint64_t fileSize = ArchiveData_GetFileSize(path);
    if (!fileSize || fileSize > static_cast<std::uint64_t>((std::numeric_limits<std::size_t>::max)())) {
        return 0;
    }
    std::vector<std::byte> bytes(static_cast<std::size_t>(fileSize));
    if (!ArchiveData_Read(path, 0, bytes.data(), fileSize)) {
        return 0;
    }
    const std::uint32_t texture = d3t::CreateImageTextureFromMemory(bytes.data(), bytes.size(), true);
    return texture;
}

void BuildHarrierPaletteLookup() {
    for (std::uint32_t value = 0; value < kHarrierPaletteColourCount; ++value) {
        const std::uint32_t red5 = ((value >> 13U) & 1U) + ((value >> 3U) & 0x1eU);
        const std::uint32_t green5 = ((value >> 12U) & 1U) + 2U * (value & 0x0fU);
        const std::uint32_t blue5 = ((value >> 14U) & 1U) + ((value >> 7U) & 0x1eU);
        const std::uint32_t red = 255U * red5 / 31U;
        const std::uint32_t green = 255U * green5 / 31U;
        const std::uint32_t blue = 255U * blue5 / 31U;
        g_harrierPaletteLookup[value] = kOpaqueBlack | blue | (green << 8U) | (red << 16U);
    }
}

void BuildHarrierGraphicTileOccupancy() {
    for (std::size_t tile = 0; tile < g_harrierGraphicTileOccupied.size(); ++tile) {
        const std::uint8_t *const pixels = g_harrierGraphics.data() + tile * 64U;
        bool occupied = false;
        for (std::size_t pixel = 0; pixel < 64; ++pixel) {
            if (pixels[pixel]) {
                occupied = true;
                break;
            }
        }
        g_harrierGraphicTileOccupied[tile] = occupied;
    }
}

void InitializeSpriteTextureCache() {
    for (std::size_t index = 0; index < g_harrierSpriteTextures.size(); ++index) {
        g_harrierSpriteTextures[index] = 0;
        HarrierSpriteCacheEntry &entry = g_harrierSpriteCache[index];
        entry = {};
        entry.textureSlot = static_cast<std::int32_t>(index);
        entry.cachedPatternKey = -1;
        entry.cachedHeight = -1;
    }
    for (std::array<std::uint32_t, 16> &palette : g_harrierSpritePalettes)
        palette.fill(kTransparentKey);
    g_harrierAverageColour = kTransparentKey;
    g_harrierPreviousAverageColour = kTransparentKey;
    g_harrierSpriteCacheCursor = 0;
}

void InitializeTileTextureCache() {
    for (std::size_t index = 0; index < g_harrierTileTextures.size(); ++index) {
        g_harrierTileTextures[index] = 0;
        g_harrierTileOccupancy[index] = 0;
        g_harrierTileTextureDirty[index] = false;
        g_harrierTilePixels[index].fill(0);
    }
    for (std::array<std::uint16_t, kHarrierMapWords> &map : g_harrierTileMaps)
        map.fill(0);
    for (std::array<std::uint16_t, kHarrierMapWords> &map : g_harrierPreviousTileMaps)
        map.fill(0);
    for (std::array<std::uint32_t, 8> &palette : g_harrierTilePalettes)
        palette.fill(kTransparentKey);
}

void InitializeHarrierDebugText() {
    for (std::array<char, kHarrierDebugColumns> &row : g_harrierDebugText)
        row.fill(0);
    g_harrierDebugCursor = 0;
    if (!g_harrierDebugFontTexture)
        g_harrierDebugFontTexture = LoadHarrierImage("XHAR/datas/SYSASCII.BMP");
}

void SetHarrierRenderFlags() {
    SetRenderFlags(0x93, 0);
    SetRenderFlags(0x3b, 1);
    SetRenderFlags(0x3e, 770);
    SetRenderFlags(0x3f, 771);
}

void SetHarrierUnfoggedRenderFlags() {
    SetRenderFlags(0x93, 0);
    SetRenderFlags(0x3b, 0);
}

void BindHarrierShader(D3TDeviceContext *context, std::uint32_t shaderLoad, std::uint32_t layoutIndex) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const std::uint32_t vertexShader = d3t_GetLoadedShader(shaderLoad, 0);
    const std::uint32_t pixelShader = d3t_GetLoadedShader(shaderLoad, 1);
    if (state.vertexShader != vertexShader) {
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
        state.vertexShader = vertexShader;
    }
    if (state.pixelShader != pixelShader) {
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
        state.pixelShader = pixelShader;
    }
    const std::uint32_t inputLayout = g_fvfLayouts[layoutIndex];
    if (state.inputLayout != inputLayout) {
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
        state.inputLayout = inputLayout;
    }
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip)) {
        state.bufferStateFlags |= d3t::BufferTopology;
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip);
    }
}

template <typename Vertex>
void DrawHarrierQuad(const std::array<Vertex, 4> &vertices, std::uint32_t shaderLoad, std::uint32_t layoutIndex, std::uint32_t texture) {
#if defined(_WIN32)
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (!context || !context->deviceContext || !shaderLoad || !g_fvfLayouts[layoutIndex])
        return;
    BindHarrierShader(context, shaderLoad, layoutIndex);
    if (texture)
        BindLegacyPixelTexture(context, texture);
    void *const mapped = GetMappedConstantBuffer(context, static_cast<std::int64_t>(layoutIndex * 4U), sizeof(vertices));
    if (!mapped)
        return;
    std::memcpy(mapped, vertices.data(), sizeof(vertices));
    CommitLegacyScreenRenderState(context, false);
    if (layoutIndex != 30) {
        // SetEmuRenderFlags selects point/wrap PS s4, independently of the legacy material samplers s0..s3.
        d3t::D3TRenderState &state = d3t::GetRenderState(*context);
        std::uint64_t samplerFlags = 0;
        std::memcpy(&samplerFlags, context->renderState.data() + 0x574, sizeof(samplerFlags));
        if (samplerFlags != 0) {
            std::memset(context->renderState.data() + 0x574, 0, 16);
            state.samplerDirtyMasks[1] |= 1U << 4U;
        }
    }
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(4, 0);
#else
    static_cast<void>(vertices);
    static_cast<void>(shaderLoad);
    static_cast<void>(layoutIndex);
    static_cast<void>(texture);
#endif
}

void DrawPositionColourQuad(float left, float top, float right, float bottom, std::uint32_t colour, std::uint32_t texture = 0) {
    const std::array<HarrierPositionColourVertex, 4> vertices{
        {{left, top, 0.5f, 1.0f, colour}, {left, bottom, 0.5f, 1.0f, colour}, {right, top, 0.5f, 1.0f, colour}, {right, bottom, 0.5f, 1.0f, colour}}};
    DrawHarrierQuad(vertices, g_harrierPdShaderLoad, 30, texture);
}

void DrawPositionColourTextureQuad(
    float left, float top, float right, float bottom, std::uint32_t colour, float u0, float v0, float u1, float v1, std::uint32_t texture) {
    const std::array<HarrierPositionColourTextureVertex, 4> vertices{{{left, top, 0.5f, 1.0f, colour, u0, v0}, {left, bottom, 0.5f, 1.0f, colour, u0, v1},
        {right, top, 0.5f, 1.0f, colour, u1, v0}, {right, bottom, 0.5f, 1.0f, colour, u1, v1}}};
    DrawHarrierQuad(vertices, g_harrierPdtShaderLoad, 31, texture);
}

void DrawPositionTextureQuad(float left, float top, float right, float bottom, std::uint32_t texture, float topV, float bottomV) {
    const std::array<HarrierPositionTextureVertex, 4> vertices{{{left, top, 0.5f, 1.0f, 0.0f, topV}, {right, top, 0.5f, 1.0f, 1.0f, topV},
        {left, bottom, 0.5f, 1.0f, 0.0f, bottomV}, {right, bottom, 0.5f, 1.0f, 1.0f, bottomV}}};
    DrawHarrierQuad(vertices, g_harrierPtShaderLoad, 32, texture);
}

void UpdateHarrierPalettes() {
    for (std::size_t paletteIndex = 0; paletteIndex < kHarrierTilePaletteCount; ++paletteIndex) {
        std::array<std::uint32_t, 8> &palette = g_harrierTilePalettes[paletteIndex];
        bool dirty = false;
        if (palette[0] != kTransparentKey) {
            palette[0] = kTransparentKey;
            dirty = true;
        }
        const std::size_t source = kHarrierTilePaletteOffset + paletteIndex * 16U;
        for (std::size_t colour = 1; colour < palette.size(); ++colour) {
            const std::uint32_t converted = g_harrierPaletteLookup[HarrierMemoryWord(source + colour * 2U) & 0x7fffU];
            if (palette[colour] != converted) {
                palette[colour] = converted;
                dirty = true;
            }
        }
        g_harrierTilePaletteDirty[paletteIndex] = dirty;
    }

    for (std::size_t paletteIndex = 0; paletteIndex < kHarrierSpritePaletteCount; ++paletteIndex) {
        std::array<std::uint32_t, 16> &palette = g_harrierSpritePalettes[paletteIndex];
        bool dirty = false;
        if (palette[0] != kTransparentKey) {
            palette[0] = kTransparentKey;
            dirty = true;
        }
        const std::size_t source = kHarrierSpritePaletteOffset + paletteIndex * 32U;
        for (std::size_t colour = 1; colour < palette.size(); ++colour) {
            const std::uint32_t converted = g_harrierPaletteLookup[HarrierMemoryWord(source + colour * 2U) & 0x7fffU];
            if (palette[colour] != converted) {
                palette[colour] = converted;
                dirty = true;
            }
        }
        g_harrierSpritePaletteDirty[paletteIndex] = dirty;
    }

    g_harrierPreviousAverageColour = g_harrierAverageColour;
    std::uint32_t red = 0;
    std::uint32_t green = 0;
    std::uint32_t blue = 0;
    for (std::size_t index = 0; index < 4; ++index) {
        const std::uint32_t colour = g_harrierPaletteLookup[HarrierMemoryWord(kHarrierAveragePaletteRefsOffset + index * 2U) & 0x7fffU];
        red += colour & 0xffU;
        green += (colour >> 8U) & 0xffU;
        blue += (colour >> 16U) & 0xffU;
    }
    red = (red >> 2U) * 75U / 100U;
    green = (green >> 2U) * 75U / 100U;
    blue = (blue >> 2U) * 75U / 100U;
    g_harrierAverageColour = kOpaqueBlack | red | (green << 8U) | (blue << 16U);
}

void SwapSpriteCacheMetadata(HarrierSpriteCacheEntry &left, HarrierSpriteCacheEntry &right) {
    const std::int32_t textureSlot = left.textureSlot;
    const std::int32_t patternKey = left.cachedPatternKey;
    const std::int16_t palette = left.cachedPalette;
    const std::int16_t height = left.cachedHeight;
    left.textureSlot = right.textureSlot;
    left.cachedPatternKey = right.cachedPatternKey;
    left.cachedPalette = right.cachedPalette;
    left.cachedHeight = right.cachedHeight;
    right.textureSlot = textureSlot;
    right.cachedPatternKey = patternKey;
    right.cachedPalette = palette;
    right.cachedHeight = height;
}

HarrierSpriteCacheEntry *FindSpriteCacheEntry(std::size_t spriteIndex, std::int32_t patternKey, std::int16_t palette) {
    const std::size_t first = spriteIndex > 32 ? spriteIndex - 32 : 0;
    for (std::size_t index = spriteIndex + 1; index > first; --index) {
        HarrierSpriteCacheEntry &candidate = g_harrierSpriteCache[index - 1];
        if (candidate.cachedPatternKey == patternKey && candidate.cachedPalette == palette)
            return &candidate;
    }
    const std::size_t last = spriteIndex + 32 < kHarrierSpriteCount ? spriteIndex + 32 : kHarrierSpriteCount;
    for (std::size_t index = spriteIndex + 1; index < last; ++index) {
        HarrierSpriteCacheEntry &candidate = g_harrierSpriteCache[index];
        if (candidate.cachedPatternKey == patternKey && candidate.cachedPalette == palette)
            return &candidate;
    }
    for (std::size_t index = kHarrierSpriteCachePoolStart; index < g_harrierSpriteCache.size(); ++index) {
        HarrierSpriteCacheEntry &candidate = g_harrierSpriteCache[index];
        if (candidate.cachedPatternKey == patternKey && candidate.cachedPalette == palette)
            return &candidate;
    }
    return nullptr;
}

void UploadSpriteTexture(HarrierSpriteCacheEntry &entry) {
    if (entry.textureSlot < 0 || static_cast<std::size_t>(entry.textureSlot) >= g_harrierSpriteTextures.size() || entry.width <= 0 || entry.height <= 0)
        return;
    const std::size_t pixelCount = static_cast<std::size_t>(entry.width) * static_cast<std::size_t>(entry.height);
    const std::size_t sourceOffset = kHarrierBackgroundGraphicsBytes + static_cast<std::size_t>(entry.patternKey) * 8U;
    if (sourceOffset > g_harrierGraphics.size() || pixelCount > g_harrierGraphics.size() - sourceOffset || pixelCount > g_harrierSpritePixels.size())
        return;
    std::uint32_t *const pixels = g_harrierSpritePixels.data();
    const std::array<std::uint32_t, 16> &sourcePalette = g_harrierSpritePalettes[static_cast<std::uint16_t>(entry.palette) & 0x3fU];
    std::array<std::uint32_t, 16> palette = sourcePalette;

    if ((entry.palette & 0x80) == 0)
        palette[10] = g_harrierAverageColour;
    const std::uint8_t *const source = g_harrierGraphics.data() + sourceOffset;
    for (std::size_t index = 0; index < pixelCount; ++index)
        pixels[index] = palette[(source[index] >> 2U) & 0x0fU];
    std::uint32_t &texture = g_harrierSpriteTextures[static_cast<std::size_t>(entry.textureSlot)];
    InitTexture2DResource(texture);
    const Texture2DDescription description{
        static_cast<std::uint32_t>(entry.width), static_cast<std::uint32_t>(entry.height), 1, 1, kHarrierTextureFormat, 0, 0, 1, 0, 0, pixels};
    texture = CreateTexture2D(&description);
}

void AssignSpriteCache(std::size_t spriteIndex, std::int32_t patternKey, std::int16_t palette, std::int16_t width, std::int16_t height) {
    HarrierSpriteCacheEntry &entry = g_harrierSpriteCache[spriteIndex];
    HarrierSpriteCacheEntry *cached = nullptr;
    if ((static_cast<std::uint16_t>(palette) & 0x80U) != 0 || g_harrierAverageColour == g_harrierPreviousAverageColour)
        cached = FindSpriteCacheEntry(spriteIndex, patternKey, palette);
    if (cached) {
        if (!cached->claimed) {
            if (cached != &entry)
                SwapSpriteCacheMetadata(entry, *cached);
            cached = &entry;
            entry.claimed = 1;
        }
        entry.cache = cached;
        return;
    }

    g_harrierSpriteCacheCursor = (g_harrierSpriteCacheCursor + 1U) & 31U;
    cached = &g_harrierSpriteCache[kHarrierSpriteCachePoolStart + g_harrierSpriteCacheCursor];
    if (cached != &entry)
        SwapSpriteCacheMetadata(entry, *cached);
    entry.patternKey = patternKey;
    entry.palette = palette;
    entry.width = width;
    entry.height = height;
    entry.cachedPatternKey = patternKey;
    entry.cachedPalette = palette;
    entry.cachedHeight = height;
    entry.cache = &entry;
    entry.claimed = 1;
    UploadSpriteTexture(entry);
}

void DrawHarrierSprites() {
    for (HarrierSpriteCacheEntry &entry : g_harrierSpriteCache) {
        entry.claimed = 0;
        entry.cache = nullptr;
        if (g_harrierSpritePaletteDirty[static_cast<std::uint16_t>(entry.cachedPalette) & 0x3fU])
            entry.cachedPatternKey = -1;
    }

    std::size_t spriteCount = kHarrierSpriteCount;
    for (std::size_t index = 0; index < kHarrierSpriteCount; ++index) {
        const std::uint8_t *const sprite = g_harrierMemory + kHarrierSpriteListOffset + index * 16U;
        if (sprite[0] == 0xffU) {
            spriteCount = index;
            break;
        }
        HarrierSpriteCacheEntry &entry = g_harrierSpriteCache[index];
        entry.active = 0;
        if (!sprite[0])
            continue;
        const std::uint16_t positionAndSize = ReadBigEndianWord(sprite + 2);
        std::int32_t size = 8 * (sprite[5] & 0x1fU);
        if (!size)
            size = 256;
        std::int32_t left = static_cast<std::int32_t>(positionAndSize & 0x1ffU) - 192;
        const std::int32_t pattern = 8 * (positionAndSize & 0x7000U) + (ReadBigEndianWord(sprite + 6) & 0x7fffU);
        float scaleX = static_cast<float>(128 - (sprite[8] & 0x7fU)) * 0.0078125f;
        const float scaleY = static_cast<float>(128 - (sprite[9] & 0x7fU)) * 0.0078125f;
        std::int32_t patternWidthKey = size >> 3;
        if (static_cast<std::int8_t>(sprite[6]) < 0) {
            patternWidthKey = 1;
            left += static_cast<std::int32_t>(static_cast<float>(size) * scaleX);
            scaleX = -scaleX;
        }
        const std::int32_t height = scaleY != 0.0f ? static_cast<std::int32_t>(static_cast<float>(sprite[0] - sprite[1]) / scaleY) : 0;
        if (height < 1)
            continue;
        entry.active = 1;
        entry.left = static_cast<float>(2 * left);
        entry.right = entry.left + 2.0f * static_cast<float>(size) * scaleX;
        entry.top = static_cast<float>(2 * sprite[0] + 16);
        entry.bottom = static_cast<float>(2 * sprite[1] + 16);
        AssignSpriteCache(
            index, pattern + patternWidthKey, static_cast<std::int16_t>(sprite[4]), static_cast<std::int16_t>(size), static_cast<std::int16_t>(height));
    }

    SetHarrierRenderFlags();
    for (std::size_t index = 0; index < spriteCount; ++index) {
        HarrierSpriteCacheEntry &entry = g_harrierSpriteCache[index];
        if (!entry.active || !entry.cache || entry.cache->textureSlot < 0)
            continue;
        const std::size_t textureSlot = static_cast<std::size_t>(entry.cache->textureSlot);
        if (textureSlot < g_harrierSpriteTextures.size() && g_harrierSpriteTextures[textureSlot])
            DrawPositionTextureQuad(entry.left, entry.top, entry.right, entry.bottom, g_harrierSpriteTextures[textureSlot], 0.0f, 0.9990234375f);
    }
}

void UpdateTile(std::int32_t column, std::int32_t row, std::size_t layerBase, std::uint16_t tile, std::size_t paletteByteOffset) {
    const std::int32_t tileColumn = column & 3;
    const std::int32_t tileRow = row & 3;
    const std::size_t textureIndex = layerBase + static_cast<std::size_t>(row / 4) * 16U + static_cast<std::size_t>(column / 4);
    if (textureIndex >= g_harrierTileTextures.size())
        return;
    g_harrierTileTextureDirty[textureIndex] = true;
    const std::uint16_t occupiedBit = static_cast<std::uint16_t>(1U << (tileColumn + 4 * tileRow));
    const std::size_t destinationX = static_cast<std::size_t>(tileColumn) * 8U;
    const std::size_t destinationY = static_cast<std::size_t>(tileRow) * 8U;
    const bool occupied = tile < g_harrierGraphicTileOccupied.size() && g_harrierGraphicTileOccupied[tile];
    if (occupied) {
        g_harrierTileOccupancy[textureIndex] |= occupiedBit;
        const std::uint8_t *const source = g_harrierGraphics.data() + static_cast<std::size_t>(tile) * 64U;
        const std::size_t paletteIndex = (paletteByteOffset / 8U) & 0x7fU;
        const std::array<std::uint32_t, 8> &palette = g_harrierTilePalettes[paletteIndex];
        for (std::size_t y = 0; y < 8; ++y) {
            for (std::size_t x = 0; x < 8; ++x) {
                const std::size_t colourIndex = (source[y * 8U + x] >> 2U) & 7U;
                g_harrierTilePixels[textureIndex][(destinationY + y) * 32U + destinationX + x] = palette[colourIndex];
            }
        }
    } else {
        g_harrierTileOccupancy[textureIndex] &= static_cast<std::uint16_t>(~occupiedBit);
        for (std::size_t y = 0; y < 8; ++y) {
            for (std::size_t x = 0; x < 8; ++x)
                g_harrierTilePixels[textureIndex][(destinationY + y) * 32U + destinationX + x] = kTransparentKey;
        }
    }
}

void UploadTileTextures(std::size_t layerBase) {
    for (std::size_t index = layerBase; index < layerBase + 128U; ++index) {
        if (!g_harrierTileTextureDirty[index])
            continue;
        g_harrierTileTextureDirty[index] = false;
        InitTexture2DResource(g_harrierTileTextures[index]);
        const Texture2DDescription description{32, 32, 1, 1, kHarrierTextureFormat, 0, 0, 1, 0, 0, g_harrierTilePixels[index].data()};
        g_harrierTileTextures[index] = CreateTexture2D(&description);
    }
}

void UpdateTileLayer(std::size_t layer) {
    const std::size_t layerBase = layer * 128U;
    std::array<std::uint16_t, kHarrierMapWords> &map = g_harrierTileMaps[layer];
    std::array<std::uint16_t, kHarrierMapWords> &previous = g_harrierPreviousTileMaps[layer];
    if (layer == 0) {
        for (std::int32_t row = 0; row < 28; ++row) {
            for (std::int32_t column = 0; column < 40; ++column) {
                map[static_cast<std::size_t>(row) * 64U + column] =
                    HarrierMemoryWord(kHarrierForegroundMapOffset + static_cast<std::size_t>(row) * 128U + static_cast<std::size_t>(column) * 2U);
            }
        }
    }
    const std::int32_t rows = layer == 0 ? 28 : 32;
    const std::int32_t columns = layer == 0 ? 40 : 64;
    for (std::int32_t row = 0; row < rows; ++row) {
        for (std::int32_t column = 0; column < columns; ++column) {
            const std::size_t mapIndex = static_cast<std::size_t>(row) * 64U + column;
            const std::uint16_t value = map[mapIndex];
            const std::size_t paletteIndex = layer == 0 ? (value >> 8U) & 0x0fU : (value >> 5U) & 0x7fU;
            if (previous[mapIndex] == value && !g_harrierTilePaletteDirty[paletteIndex])
                continue;
            previous[mapIndex] = value;
            const std::uint16_t tile = layer == 0 ? static_cast<std::uint8_t>(value) : value & 0x0fffU;
            const std::size_t paletteByteOffset = layer == 0 ? (value >> 5U) & 0x78U : (value >> 2U) & 0x3f8U;
            UpdateTile(column, row, layerBase, tile, paletteByteOffset);
        }
    }
    UploadTileTextures(layerBase);
}

void CopyTilePage(std::int32_t x, std::int32_t y, std::size_t pageOffset, std::array<std::uint16_t, kHarrierMapWords> &destination) {
    std::int32_t rows = 32;
    std::int32_t columns = 64;
    std::int32_t sourceRow = 0;
    std::int32_t sourceColumn = 0;
    if (y < -8) {
        sourceRow = ((-9 - y) >> 3) + 1;
        rows -= sourceRow;
        y += 8 * sourceRow;
    }
    if (x < -8) {
        sourceColumn = ((-9 - x) >> 3) + 1;
        columns -= sourceColumn;
        x += 8 * sourceColumn;
    }
    const std::int32_t clippedColumns = (328 - x) / 8 < columns ? (328 - x) / 8 : columns;
    const std::int32_t clippedRows = (232 - y) / 8 < rows ? (232 - y) / 8 : rows;
    if (clippedColumns <= 0 || clippedRows <= 0)
        return;
    for (std::int32_t row = 0; row < clippedRows; ++row) {
        const std::size_t sourceBase =
            kHarrierTilePageOffset + pageOffset + static_cast<std::size_t>(sourceRow + row) * 128U + static_cast<std::size_t>(sourceColumn) * 2U;
        const std::size_t destinationBase = static_cast<std::size_t>(sourceRow + row) * 64U + static_cast<std::size_t>(sourceColumn);
        for (std::int32_t column = 0; column < clippedColumns; ++column)
            destination[destinationBase + static_cast<std::size_t>(column)] = HarrierMemoryWord(sourceBase + static_cast<std::size_t>(column) * 2U);
    }
}

void AssembleScrollingLayer(std::size_t layer, std::int32_t scrollX, std::int32_t scrollY, std::uint16_t pageSelection) {
    std::array<std::uint16_t, kHarrierMapWords> &destination = g_harrierTileMaps[layer];
    CopyTilePage(scrollX, scrollY, 4096U * (pageSelection & 0x0fU), destination);
    CopyTilePage(scrollX + 512, scrollY, 4096U * ((pageSelection >> 4U) & 0x0fU), destination);
    CopyTilePage(scrollX, scrollY + 256, 4096U * ((pageSelection >> 8U) & 0x0fU), destination);
    CopyTilePage(scrollX + 512, scrollY + 256, 4096U * ((pageSelection >> 12U) & 0x0fU), destination);
}

void DrawTileLayer(std::size_t layer, std::int32_t x, std::int32_t y) {
    const std::size_t layerBase = layer * 128U;
    std::int32_t screenY = 2 * y + 16;
    std::size_t textureIndex = layerBase;
    for (std::int32_t textureRow = 0; textureRow < 8; ++textureRow) {
        std::int32_t screenX = 2 * x;
        for (std::int32_t textureColumn = 0; textureColumn < 16; ++textureColumn, ++textureIndex) {
            if (screenX >= -63 && screenX <= 639 && screenY >= -47 && screenY <= 463 && g_harrierTileOccupancy[textureIndex] &&
                g_harrierTileTextures[textureIndex]) {
                DrawPositionTextureQuad(static_cast<float>(screenX), static_cast<float>(screenY), static_cast<float>(screenX + 64),
                    static_cast<float>(screenY + 64), g_harrierTileTextures[textureIndex], 0.9990000128746033f, 0.0f);
            }
            screenX += 64;
        }
        screenY += 64;
    }
}

void DrawHarrierFillLayer(std::uint16_t priority) {
    SetHarrierUnfoggedRenderFlags();
    float top = 16.0f;
    for (std::size_t line = 0; line < kHarrierVisibleLines; ++line) {
        const std::uint16_t control = HarrierMemoryWord(kHarrierLineControlOffset + line * 2U);
        if ((control & 0x600U) == priority) {
            const std::size_t colourMapIndex = static_cast<std::size_t>(static_cast<std::uint8_t>(control)) * 2U;
            const std::uint8_t paletteIndex = g_harrierMemory[kHarrierLinePaletteOffset + colourMapIndex] & 0x3fU;
            const std::uint32_t colour = g_harrierPaletteLookup[HarrierMemoryWord(kHarrierTilePaletteMapOffset + paletteIndex * 2U) & 0x7fffU];
            DrawPositionColourQuad(0.0f, top, 640.0f, top + 2.0f, colour, g_harrierFillTexture);
        }
        top += 2.0f;
    }
}

void DrawHarrierPatternLayer(std::uint16_t priority) {
    SetHarrierRenderFlags();
    float top = 16.0f;
    for (std::size_t line = 0; line < kHarrierVisibleLines; ++line) {
        const std::uint16_t control = HarrierMemoryWord(kHarrierLineControlOffset + line * 2U);
        if ((control & 0x600U) == priority) {
            const std::size_t controlIndex = static_cast<std::size_t>(static_cast<std::uint8_t>(control)) * 2U;
            const std::int32_t horizontal = (static_cast<std::int32_t>(g_harrierMemory[kHarrierLinePatternOffset + controlIndex + 1U]) -
                                                (static_cast<std::int32_t>(g_harrierMemory[kHarrierLinePatternOffset + controlIndex]) << 8)) &
                0x1ff;
            const float left = static_cast<float>(2 * (-24 - horizontal));
            const float patternV = static_cast<float>(controlIndex) * (1.0f / 1024.0f);
            const bool alternate = (g_harrierMemory[kHarrierLineFlagsOffset + controlIndex] & 2U) != 0;
            const std::size_t firstColourOffset = kHarrierAveragePaletteRefsOffset + (alternate ? 2U : 0U);
            const std::size_t secondColourOffset = kHarrierAveragePaletteRefsOffset + (alternate ? 4U : 6U);
            const std::uint32_t firstColour = g_harrierPaletteLookup[HarrierMemoryWord(firstColourOffset) & 0x7fffU];
            const std::uint32_t secondColour = g_harrierPaletteLookup[HarrierMemoryWord(secondColourOffset) & 0x7fffU];
            DrawPositionColourTextureQuad(left, top, left + 1024.0f, top + 2.0f, firstColour, 0.0f, patternV, 1.0f, patternV, g_harrierPatternTexture);
            DrawPositionColourTextureQuad(
                left, top, left + 1024.0f, top + 2.0f, secondColour, 0.0f, patternV + 0.5f, 1.0f, patternV + 0.5f, g_harrierPatternTexture);
        }
        top += 2.0f;
    }
}

void DrawHarrierBorder() {
    SetHarrierUnfoggedRenderFlags();
    DrawPositionColourQuad(0.0f, 0.0f, 640.0f, 16.0f, kOpaqueBlack);
    DrawPositionColourQuad(0.0f, 464.0f, 640.0f, 480.0f, kOpaqueBlack);
}

void DrawDebugString(float x, float y, const char *text, std::uint32_t colourIndex) {
    if (!text || !g_harrierDebugFontTexture)
        return;
    SetHarrierRenderFlags();
    const std::uint32_t colour = g_harrierDebugColours[colourIndex & 0x0fU];
    while (*text) {
        const std::uint8_t character = static_cast<std::uint8_t>(*text++);
        const float u0 = static_cast<float>(character & 0x0fU) * 0.0625f;
        const float v0 = static_cast<float>(character >> 4U) * 0.0625f;
        DrawPositionColourTextureQuad(x, y, x + 16.0f, y + 16.0f, colour, u0, v0, u0 + 0.0625f, v0 + 0.0625f, g_harrierDebugFontTexture);
        x += 16.0f;
    }
}

void DrawRepeatedDebugGlyph(float x, float y, std::size_t count, std::uint32_t colour) {
    std::array<char, 54> glyphs{};
    if (count >= glyphs.size())
        count = glyphs.size() - 1;
    for (std::size_t index = 0; index < count; ++index)
        glyphs[index] = 1;
    DrawDebugString(x, y, glyphs.data(), colour);
}

void DrawHarrierDebugMenu() {
    const InputSlot &input = g_inputState.slots[0];
    if ((input.pressedPriority & 0x3000U) != 0) {
        g_harrierInputOrientation = (g_harrierInputOrientation - 1) & 1;
        const float magnitude = std::fabs(g_harrierInputScaleY);
        g_harrierInputScaleY = g_harrierInputOrientation ? -magnitude : magnitude;
    }
    for (std::size_t line = 0; line < 5; ++line)
        DrawRepeatedDebugGlyph(160.0f, 192.0f + static_cast<float>(line) * 16.0f, 21, 15);
    DrawDebugString(176.0f, 192.0f, "PAD DIRECTION", 15);
    if (g_harrierInputOrientation) {
        DrawDebugString(176.0f, 240.0f, ">", 15);
        DrawDebugString(192.0f, 224.0f, "TYPE1", 15);
        DrawDebugString(192.0f, 240.0f, "TYPE2", 2);
        DrawDebugString(288.0f, 224.0f, "    UP", 2);
        DrawDebugString(288.0f, 240.0f, "LEFT + RIGHT", 2);
        DrawDebugString(288.0f, 256.0f, "    DOWN", 2);
    } else {
        DrawDebugString(176.0f, 224.0f, ">", 15);
        DrawDebugString(192.0f, 224.0f, "TYPE1", 2);
        DrawDebugString(192.0f, 240.0f, "TYPE2", 15);
        DrawDebugString(288.0f, 224.0f, "    DOWN", 2);
        DrawDebugString(288.0f, 240.0f, "LEFT + RIGHT", 2);
        DrawDebugString(288.0f, 256.0f, "    UP", 2);
    }
}

void DrawPauseOverlay() {
    SetHarrierRenderFlags();
    DrawPositionColourQuad(0.0f, 0.0f, 640.0f, 480.0f, kPauseOverlayColour);
}

} // namespace

void HarrierLoadAndInitResources() {
    BuildHarrierPaletteLookup();
    ReadHarrierAsset("XHAR/datas/SHXBG.BIN", g_harrierGraphics.data(), kHarrierBackgroundGraphicsBytes);
    ReadHarrierAsset("XHAR/datas/SHXOBJ.BIN", g_harrierGraphics.data() + kHarrierBackgroundGraphicsBytes, kHarrierObjectGraphicsBytes);
    BuildHarrierGraphicTileOccupancy();
    InitializeSpriteTextureCache();
    InitializeTileTextureCache();
    g_harrierFillTexture = LoadHarrierImage("XHAR/datas/FILL.BMP");
    g_harrierPatternTexture = LoadHarrierImage("XHAR/datas/SHICPAT.BMP");
    InitializeHarrierDebugText();
    if (!g_harrierPdShaderLoad)
        g_harrierPdShaderLoad = LoadPrecompiledShader("/shaders/emu/emu_pd", g_activeRenderHandle);
    if (!g_harrierPdtShaderLoad)
        g_harrierPdtShaderLoad = LoadPrecompiledShader("/shaders/emu/emu_pdt", g_activeRenderHandle);
    if (!g_harrierPtShaderLoad)
        g_harrierPtShaderLoad = LoadPrecompiledShader("/shaders/emu/emu_pt", g_activeRenderHandle);
    g_harrierResourcesLoaded = true;
}

void CreateAndDrawEmu() {
    if (!g_harrierResourcesLoaded)
        return;
    UpdateHarrierPalettes();
    DrawHarrierFillLayer(0x200);
    DrawHarrierPatternLayer(0x000);

    const std::int32_t layer1Y = -static_cast<std::int32_t>(g_harrierMemory[kHarrierLayerScrollYOffset + 2U]);
    const std::uint16_t layer1Pages = HarrierMemoryWord(kHarrierLayerPageSelectOffset);
    const std::int32_t layer1X = ((static_cast<std::int32_t>(g_harrierMemory[kHarrierLayerScrollXOffset + 3U]) -
                                      (static_cast<std::int32_t>(g_harrierMemory[kHarrierLayerScrollXOffset + 2U]) << 8)) &
                                     0x1ff) -
        712;
    AssembleScrollingLayer(1, layer1X, layer1Y, layer1Pages);
    UpdateTileLayer(1);
    SetHarrierRenderFlags();
    DrawTileLayer(1, layer1X, layer1Y);
    DrawTileLayer(1, layer1X + 512, layer1Y);
    DrawTileLayer(1, layer1X, layer1Y + 256);
    DrawTileLayer(1, layer1X + 512, layer1Y + 256);

    const std::int32_t layer2Y = -static_cast<std::int32_t>(g_harrierMemory[kHarrierLayerScrollYOffset]);
    const std::uint16_t layer2Pages = HarrierMemoryWord(kHarrierLayerPageSelectOffset + 2U);
    const std::int32_t layer2X = ((static_cast<std::int32_t>(g_harrierMemory[kHarrierLayerScrollXOffset + 1U]) -
                                      (static_cast<std::int32_t>(g_harrierMemory[kHarrierLayerScrollXOffset]) << 8)) &
                                     0x1ff) -
        712;
    AssembleScrollingLayer(2, layer2X, layer2Y, layer2Pages);
    UpdateTileLayer(2);
    SetHarrierRenderFlags();
    DrawTileLayer(2, layer2X, layer2Y);
    DrawTileLayer(2, layer2X + 512, layer2Y);
    DrawTileLayer(2, layer2X, layer2Y + 256);
    DrawTileLayer(2, layer2X + 512, layer2Y + 256);

    DrawHarrierFillLayer(0x600);
    DrawHarrierPatternLayer(0x400);
    DrawHarrierSprites();
    UpdateTileLayer(0);
    SetHarrierRenderFlags();
    DrawTileLayer(0, 0, 0);
    DrawHarrierBorder();
}

void HarrierControllerRelated() {
    const InputSlot &input = g_inputState.slots[0];
    if (g_harrierPaused) {
        DrawPauseOverlay();
        if (g_harrierDebugEnabled) {
            if (g_harrierDebugEnabled == 1) {
                DrawHarrierDebugMenu();
                if (emu68_insn_defs[0].a == 1 && (input.pressedPriority & 0x400U) != 0)
                    g_harrierDebugEnabled = 0;
            }
        } else {
            ++g_harrierPauseFrames;
            if (g_harrierPauseFrames > 24)
                g_harrierPauseFrames = 0;
            if (emu68_insn_defs[0].a == 1 && (input.pressedPriority & 0x400U) != 0)
                g_harrierDebugEnabled = 1;
        }
    }
    if (g_harrierControllerDisconnected) {
        DrawRepeatedDebugGlyph(48.0f, 96.0f, 34, 15);
        DrawRepeatedDebugGlyph(48.0f, 112.0f, 34, 15);
        DrawDebugString(64.0f, 96.0f, "Please reconnect the controller", 15);
        DrawDebugString(64.0f, 112.0f, "  and press START to continue. ", 15);
        --g_harrierHelpChoice;
        if (g_harrierHelpChoice < 0)
            g_harrierHelpChoice = 120;
    }
}

void Harrier_DebugPrint(const char *text) {
    if (!text)
        return;
    g_harrierDebugText[kHarrierDebugRows - 1][g_harrierDebugCursor] = ' ';
    while (*text) {
        const std::uint8_t character = static_cast<std::uint8_t>(*text++);
        if (character >= 32U && character <= 127U) {
            g_harrierDebugText[kHarrierDebugRows - 1][g_harrierDebugCursor++] = static_cast<char>(character);
            if (g_harrierDebugCursor >= kHarrierDebugColumns) {
                g_harrierDebugCursor = 0;
                for (std::size_t row = 20; row < kHarrierDebugRows - 1; ++row)
                    g_harrierDebugText[row] = g_harrierDebugText[row + 1];
                g_harrierDebugText[kHarrierDebugRows - 1].fill(0);
            }
        }
        if (character == 8U && g_harrierDebugCursor > 0)
            --g_harrierDebugCursor;
        if (character == 10U) {
            g_harrierDebugCursor = 0;
            for (std::size_t row = 20; row < kHarrierDebugRows - 1; ++row)
                g_harrierDebugText[row] = g_harrierDebugText[row + 1];
            g_harrierDebugText[kHarrierDebugRows - 1].fill(0);
        } else if (character == 13U) {
            g_harrierDebugCursor = 0;
        }
    }
    g_harrierDebugText[kHarrierDebugRows - 1][g_harrierDebugCursor] = '_';
}

void Harrier_ReleaseRenderResources() {
    for (std::uint32_t &texture : g_harrierSpriteTextures) {
        InitTexture2DResource(texture);
        texture = 0;
    }
    for (std::uint32_t &texture : g_harrierTileTextures) {
        InitTexture2DResource(texture);
        texture = 0;
    }
    InitTexture2DResource(g_harrierFillTexture);
    InitTexture2DResource(g_harrierPatternTexture);
    g_harrierFillTexture = 0;
    g_harrierPatternTexture = 0;
    g_harrierResourcesLoaded = false;
}

} // namespace shenmue::s1
