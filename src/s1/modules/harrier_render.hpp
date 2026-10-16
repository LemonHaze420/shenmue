#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

inline constexpr std::size_t kHarrierPaletteColourCount = 0x8000;
inline constexpr std::size_t kHarrierSpriteTextureCount = 96;
inline constexpr std::size_t kHarrierTileTextureCount = 384;
inline constexpr std::size_t kHarrierTilePaletteOffset = 0xb0000;
inline constexpr std::size_t kHarrierSpritePaletteOffset = 0xb0800;
inline constexpr std::size_t kHarrierTilePaletteMapOffset = 0xb0f80;
inline constexpr std::size_t kHarrierForegroundMapOffset = 0x98030;
inline constexpr std::size_t kHarrierSpriteListOffset = 0xc0000;
inline constexpr std::size_t kHarrierLineControlOffset = 0xe0000;
inline constexpr std::size_t kHarrierTilePageOffset = 0x90000;

struct HarrierSpriteCacheEntry {
    std::int32_t active;
    float left;
    float right;
    float top;
    float bottom;
    std::int32_t patternKey;
    std::int16_t palette;
    std::int16_t width;
    std::int16_t height;
    std::int16_t reserved1e;
    std::int32_t textureSlot;
    std::int32_t cachedPatternKey;
    std::int16_t cachedPalette;
    std::int16_t cachedHeight;
    std::uint16_t claimed;
    std::uint16_t reserved2e;
    HarrierSpriteCacheEntry *cache;
};

static_assert(sizeof(HarrierSpriteCacheEntry) == 0x38);
static_assert(offsetof(HarrierSpriteCacheEntry, patternKey) == 0x14);
static_assert(offsetof(HarrierSpriteCacheEntry, palette) == 0x18);
static_assert(offsetof(HarrierSpriteCacheEntry, textureSlot) == 0x20);
static_assert(offsetof(HarrierSpriteCacheEntry, cache) == 0x30);

extern std::array<std::uint32_t, kHarrierSpriteTextureCount> g_harrierSpriteTextures;
extern std::array<std::uint32_t, kHarrierTileTextureCount> g_harrierTileTextures;
extern std::uint32_t g_harrierFillTexture;
extern std::uint32_t g_harrierPatternTexture;
extern std::uint32_t g_harrierDebugFontTexture;

void HarrierLoadAndInitResources();
void CreateAndDrawEmu();
void HarrierControllerRelated();
void Harrier_DebugPrint(const char *text);
void Harrier_ReleaseRenderResources();

} // namespace shenmue::s1
