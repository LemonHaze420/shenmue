#pragma once

#include "s2/texture_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#define TC_DATA const
#define TC_INIT =
#define TC_LAYOUT_NAME_(line) TCLayoutCheck##line
#define TC_LAYOUT_NAME(line) TC_LAYOUT_NAME_(line)
#define TC_LAYOUT(condition) typedef char TC_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {

struct TextureCacheSurface {
    KMSURFACEDESC surface;
    std::byte retainedState[8];
    std::int32_t creationStatus;
    std::uint32_t references;
    std::uint32_t tsp, tcw;
    std::int32_t index;
};
struct TextureIndexMap {
    std::uint32_t references;
    std::int32_t surface;
};
typedef const void *(*TextureSourceCallback)(const TextureImage *);
TC_LAYOUT(sizeof(KMSURFACEDESC) == 0x20);
TC_LAYOUT(sizeof(TextureCacheSurface) == 0x3c);
TC_LAYOUT(offsetof(TextureCacheSurface, creationStatus) == 0x28);
TC_LAYOUT(offsetof(TextureCacheSurface, references) == 0x2c);
TC_LAYOUT(offsetof(TextureCacheSurface, tsp) == 0x30);
TC_LAYOUT(offsetof(TextureCacheSurface, tcw) == 0x34);
TC_LAYOUT(offsetof(TextureCacheSurface, index) == 0x38);
TC_LAYOUT(sizeof(TextureIndexMap) == 8);
TC_LAYOUT(offsetof(TextureIndexMap, surface) == 4);
TC_DATA std::int32_t kTextureCacheCapacity = static_cast<std::int32_t>(dc::sdk::kTextureSlots);
TC_DATA std::size_t kTextureCacheBitmapWords = (kTextureCacheCapacity + 32) / 32;
TC_DATA std::size_t kNamedTextureCellsOffset = (kTextureCacheBitmapWords + (kTextureCacheCapacity + 1) * 15 + (kTextureCacheCapacity + 32) * 2) * 4;
TC_DATA std::size_t kTextureCacheStorageBytes = kNamedTextureCellsOffset + kTextureCacheCapacity * sizeof(NamedTextureCell);
TC_DATA std::uint32_t kDefaultTexturePixels[32] TC_INIT{UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX,
    UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX,
    UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX};
TC_DATA TextureImage kDefaultTextureImage TC_INIT {SHEN_FIELD(width) 8, SHEN_FIELD(height) 8, SHEN_FIELD(type) 0x101, SHEN_FIELD(pixels) kDefaultTexturePixels};
extern TextureCacheSurface g_defaultTextureSurface;
extern std::int32_t g_defaultTextureSurfaceInitialized;
extern TextureCacheSurface *g_textureCacheSurfaces;
extern TextureIndexMap *g_textureIndexMap;
extern std::uint32_t *g_textureCacheSlotMasks;
extern std::int32_t g_textureCacheSurfaceCount, g_textureCacheIndexCount, g_textureCacheMaskWords;
extern std::uint32_t g_textureCreationCount;
extern TextureSourceCallback g_textureSourceCallback;
extern std::uint8_t *g_lastLoadedTexturePixels;
void InitializeTextureSurfaceStorage(void *storage, std::int32_t count);
void InitializeTextureCache(void *storage, std::int32_t surfaceCount, std::int32_t indexCount);
void InitializeNamedTextureStorage(void *storage, std::int32_t count);
std::int32_t AllocateTextureCacheSurface();
void FreeTextureCacheSurface(std::int32_t index);
void SetTextureSourceCallback(TextureSourceCallback callback);
std::uint32_t CreateTextureCacheSurface(const TextureImage *image, std::int32_t index, const void *pixels);
void ReloadTextureCacheSurface(std::int32_t surfaceIndex, const void *pixels);
std::uint32_t GetTextureDimensionBits(std::int32_t size) asm("_shenmue_dc_texture_dimension_bits");
void BuildTextureSurfacePacketState(TextureCacheSurface *surface);
void UpdateTextureSurfacePacketState(std::int32_t index);
void BindTextureSurfacePacketState(std::uint32_t *material, std::int32_t index);
void ClearTextureSurfacePacketState(std::uint32_t *material);
void InitializeDefaultTextureSurface();
std::uint32_t ReleaseMT7MeshMaterials(void *mesh, const void *textures, std::int32_t remap);

}
}
#undef TC_LAYOUT_NAME
#undef TC_LAYOUT_NAME_
#undef TC_DATA
#undef TC_INIT
#undef TC_LAYOUT

#endif
