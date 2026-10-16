#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue {

struct MipMapRequest {
    char path[260];
    std::uint32_t textureHash;
    std::uint16_t requestedMipMask;
    std::uint8_t initialMip;
    std::uint8_t maximumMip;
};
struct MipTextureMetadata {
    std::uint32_t source;
    std::uint32_t textureHandle;
    std::uint64_t reserved08;
    std::uint64_t loadedSize;
};
struct PvrMipDescription {
    std::byte reserved00[8];
    std::uint16_t width;
    std::uint16_t height;
    std::uint8_t mipCount;
    std::uint8_t format;
};
struct AsyncMipCallbackData {
    std::uint32_t textureHash;
    std::uint8_t firstMip;
    std::uint8_t endMip;
};
static_assert(sizeof(MipMapRequest) == 0x10c);
static_assert(offsetof(MipMapRequest, textureHash) == 0x104);
static_assert(offsetof(MipMapRequest, requestedMipMask) == 0x108);
static_assert(offsetof(MipMapRequest, initialMip) == 0x10a);
static_assert(offsetof(MipMapRequest, maximumMip) == 0x10b);
static_assert(sizeof(MipTextureMetadata) == 0x18);
static_assert(sizeof(PvrMipDescription) == 0x0e);
static_assert(offsetof(AsyncMipCallbackData, firstMip) == 4);
static_assert(offsetof(AsyncMipCallbackData, endMip) == 5);

namespace d3t {
extern std::uint32_t g_pendingMipMapRequestCount;
inline constexpr std::size_t kMipTextureTableSize = 0x4000;
inline constexpr std::uint32_t kMipTextureTableMask = 0x3fff;
extern std::array<std::uint64_t, 9> g_mipMapFrameCounters;
extern bool g_loadTexturesDirectly;

MipMapRequest *QueueMipMapRequest(const MipMapRequest &request);
bool InsertMipTextureCacheEntry(std::uint32_t hash, const MipTextureMetadata &metadata);
bool FindMipTexture(std::uint32_t hash, MipTextureMetadata *metadata);
bool RemoveMipTexture(std::uint32_t hash);
std::int64_t LoadTextureFromFilepath(const char *path, std::uint32_t source, std::uint32_t *textureHandle, std::uint32_t renderHandle);
bool ReplaceWithDecodedTexture(std::uint32_t textureHandle, const void *loadedData, std::uint64_t loadedSize, bool linearColour);
void ReleaseMipTextureCacheEntry(void *hash);
void CreateTexture2DAsync(void *callbackData, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *sourcePath);
void LoadMipMaps();
std::int64_t SelectMipsForPVR(const PvrMipDescription *description, std::int32_t textureMipMode);
std::int64_t SelectMipForPVR(const char *path, const PvrMipDescription *description, std::uint32_t source, std::uint32_t *textureHandle,
    std::uint32_t renderHandle, std::int32_t textureMipMode);
} // namespace d3t
} // namespace shenmue
