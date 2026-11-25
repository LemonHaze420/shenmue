#include "s2/texture_cache_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/mt7.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

TextureCacheSurface *g_textureCacheSurfaces = 0;

TextureIndexMap *g_textureIndexMap = 0;

std::uint32_t *g_textureCacheSlotMasks = 0;

std::int32_t g_textureCacheSurfaceCount = 0, g_textureCacheIndexCount = 0, g_textureCacheMaskWords = 0;

std::uint32_t g_textureCreationCount = 0;

TextureSourceCallback g_textureSourceCallback = 0;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t *g_lastLoadedTexturePixels = 0;
#endif

TextureCacheSurface g_defaultTextureSurface;

std::int32_t g_defaultTextureSurfaceInitialized = 0;

// @verified - d
void InitializeTextureSurfaceStorage(void *storage, std::int32_t count) {
    g_textureCreationCount = 0;
    dc::sdk::g_modelTextureIdBase = 0;
    g_textureCacheSurfaceCount = count;
    g_textureCacheSurfaces = static_cast<TextureCacheSurface *>(storage);
    for (std::int32_t index = 0; index < count; ++index) {
        TextureCacheSurface &surface = g_textureCacheSurfaces[index];
        surface.creationStatus = -1;
        surface.references = 0;
        surface.index = -1;
    }
}

// @verified - d
void InitializeTextureCache(void *storage, std::int32_t surfaceCount, std::int32_t indexCount) {
    InitializeTextureSurfaceStorage(storage, surfaceCount + 1);
    std::byte *const bytes = static_cast<std::byte *>(storage);
    g_textureIndexMap = reinterpret_cast<TextureIndexMap *>(bytes + (surfaceCount + 1) * sizeof(TextureCacheSurface) + 8);
    g_textureCacheSlotMasks = reinterpret_cast<std::uint32_t *>(g_textureIndexMap + indexCount + 1);
    std::int32_t rounded = indexCount + 32;
    if (rounded < 0)
        rounded = indexCount + 63;
    g_textureCacheMaskWords = rounded >> 5;
    g_textureCacheIndexCount = indexCount;
    for (std::int32_t index = -1; index < g_textureCacheIndexCount; ++index)
        g_textureIndexMap[index] = (__typeof__(g_textureIndexMap[index])){SHEN_FIELD(references) 0, SHEN_FIELD(surface) - 1};
    for (std::int32_t index = 0; index < g_textureCacheMaskWords; ++index)
        g_textureCacheSlotMasks[index] = 0;
    dc::sdk::g_modelTextureIdBase = 0;
}

// @verified - d
std::int32_t AllocateTextureCacheSurface() {
    for (std::int32_t word = 0; word < g_textureCacheMaskWords; ++word) {
        const std::uint32_t mask = g_textureCacheSlotMasks[word];
        if (mask == UINT32_MAX)
            continue;
        std::uint32_t bit = 1;
        std::int32_t offset = 0;
        while (mask & bit) {
            bit += bit;
            ++offset;
        }
        g_textureCacheSlotMasks[word] |= bit;
        return word * 32 + offset;
    }
    return -1;
}

// @verified - d
void FreeTextureCacheSurface(std::int32_t index) {
    if (index >= 0)
        g_textureCacheSlotMasks[index >> 5] &= ~(1U << (static_cast<std::uint32_t>(index) & 31U));
}

// @verified - d
void SetTextureSourceCallback(TextureSourceCallback callback) {
    g_textureSourceCallback = callback;
}

std::uint32_t CreateTextureCacheSurface(const TextureImage *image, std::int32_t index, const void *pixels) {
    TextureCacheSurface &surface = g_textureCacheSurfaces[index];
    ++g_textureCreationCount;

    surface.creationStatus = kmCreateTextureSurface(&surface.surface, image->width, image->height, static_cast<KMTEXTURETYPE>(image->type));
    if (surface.creationStatus)
        return 1;
    if (g_textureSourceCallback) {
        const void *const replacement = g_textureSourceCallback(image);
        if (replacement)
            pixels = replacement;
    }

    kmLoadTexture(&surface.surface, static_cast<Uint32 *>(const_cast<void *>(pixels)));
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    g_lastLoadedTexturePixels = reinterpret_cast<std::uint8_t *>(reinterpret_cast<std::uintptr_t>(surface.surface.pSurface) + 0x04000000U);
#endif
    return 0;
}

// @verified - d
void ReloadTextureCacheSurface(std::int32_t surfaceIndex, const void *pixels) {
    kmLoadTexture(&g_textureCacheSurfaces[surfaceIndex].surface, static_cast<Uint32 *>(const_cast<void *>(pixels)));
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    g_lastLoadedTexturePixels =
        reinterpret_cast<std::uint8_t *>(reinterpret_cast<std::uintptr_t>(g_textureCacheSurfaces[surfaceIndex].surface.pSurface) + 0x04000000U);
#endif
}

// @verified - d
std::uint32_t GetTextureDimensionBits(std::int32_t size) {
    switch (size) {
    case 8:
        return 0;
    case 16:
        return 1;
    case 32:
        return 2;
    case 64:
        return 3;
    case 128:
        return 4;
    case 256:
        return 5;
    case 512:
        return 6;
    case 1024:
        return 7;
    }
}

// @verified - d
void BuildTextureSurfacePacketState(TextureCacheSurface *surface) {
    const std::uint32_t width = GetTextureDimensionBits(surface->surface.Width);
    const std::uint32_t height = GetTextureDimensionBits(surface->surface.Height);
    surface->tsp = width * 8U | height;
    const std::uint32_t flags = surface->surface.fSurfaceFlags;
    surface->tcw = (flags << 31U) | ((flags & 8U) << 27U) | (surface->surface.PixelFormat & 0x38000000U) | ((~flags & 4U) << 24U) | ((flags & 0x10U) << 21U) |
        (reinterpret_cast<std::uintptr_t>(surface->surface.pSurface) >> 3U);
}

// @verified - d
void UpdateTextureSurfacePacketState(std::int32_t index) {
    TextureCacheSurface &surface = g_textureCacheSurfaces[index];
    if (!surface.creationStatus) {
        BuildTextureSurfacePacketState(&surface);
    } else {
        surface.tsp = g_defaultTextureSurface.tsp;
        surface.tcw = g_defaultTextureSurface.tcw;
    }
}

// @verified - d
void BindTextureSurfacePacketState(std::uint32_t *material, std::int32_t index) {
    const TextureCacheSurface &surface = g_textureCacheSurfaces[index];
    material[2] = (material[2] & 0xffffffc0U) | surface.tsp;
    material[3] = surface.tcw;
}

// @verified - d
void ClearTextureSurfacePacketState(std::uint32_t *material) {
    material[2] = (material[2] & 0xffffffc0U) | g_defaultTextureSurface.tsp;
    material[3] = (material[3] & 0xf8000000U) | g_defaultTextureSurface.tcw;
}

// @verified - d
void InitializeDefaultTextureSurface() {
    kmCreateTextureSurface(
        &g_defaultTextureSurface.surface, kDefaultTextureImage.width, kDefaultTextureImage.height, static_cast<KMTEXTURETYPE>(kDefaultTextureImage.type));
    kmLoadTexture(&g_defaultTextureSurface.surface, static_cast<Uint32 *>(const_cast<void *>(kDefaultTextureImage.pixels)));
    BuildTextureSurfacePacketState(&g_defaultTextureSurface);
}

// @verified - d
std::int32_t ReleaseTextureIndex(std::int32_t index) {
    const std::int32_t resolved = static_cast<std::int32_t>(static_cast<std::uint32_t>(index) + dc::sdk::g_modelTextureIdBase);
    if (g_textureIndexMap[resolved].references) {
        const std::int32_t slot = g_textureIndexMap[resolved].surface;
        if (!g_textureCacheSurfaces[slot].creationStatus && kmFreeTexture(&g_textureCacheSurfaces[slot].surface))
            return -2;
        g_textureCacheSurfaces[slot].references = 0;
        g_textureCacheSurfaces[slot].index = -1;
        FreeTextureCacheSurface(slot);
        g_textureIndexMap[resolved].surface = -1;
        g_textureIndexMap[resolved].references = 0;
    }
    return 0;
}

// @verified - d
std::int32_t ReleaseNamedTextureByName(const std::uint32_t *name) {
    NamedTextureCell *previous;
    NamedTextureCell *cell;
    const std::int32_t index = FindNamedTexture(name[0], name[1], &previous, &cell);
    if (index < 0)
        return -1;
    --cell->references;
    if (static_cast<std::int16_t>(cell->references) > 0)
        return 0;
    const std::int32_t result = ReleaseTextureIndex(index);
    ReleaseNamedTextureCell(previous, cell);
    return result;
}

} // namespace s2
} // namespace shenmue

#endif
