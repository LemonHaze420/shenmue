#pragma once

#include "common/math.hpp"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <vector>

namespace shenmue::d3t {

inline constexpr Vector4 kDefaultWhiteColour{1.0f, 1.0f, 1.0f, 1.0f};
inline constexpr Vector4 kDefaultBlackColour{};
inline constexpr Vector4 kDefaultQuarterColour{0.25f, 0.25f, 0.25f, 0.25f};
inline constexpr Vector4 kDefaultThreeQuarterColour{0.75f, 0.75f, 0.75f, 0.75f};
inline constexpr Vector4 kDefaultNormalColour{0.5f, 0.5f, 1.0f, 1.0f};

extern std::uint32_t g_defaultWhiteTexture, g_defaultBlackTexture, g_defaultQuarterTexture, g_defaultThreeQuarterTexture, g_defaultNormalTexture;
std::uint32_t RGBA8_Create2x2Texture2D(const Vector4 *colour);
void CreateTemp2x2Textures();
extern std::int32_t g_TextureRefCount;

struct Texture2DDescription {
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t arraySize;
    std::uint32_t mipLevels;
    std::uint32_t format;
    std::uint32_t bindFlags;
    std::uint32_t flags;
    std::uint32_t sampleCount;
    std::uint32_t sampleQuality;
    std::uint32_t reserved_24;
    const void *initialData;
};

static_assert(sizeof(Texture2DDescription) == 0x30);

struct TextureUploadDescription {
    std::int16_t width;
    std::int16_t height;
    std::int32_t format;
    std::int32_t arenaOffset;
};

struct SurfaceMapping {
    std::int16_t referenceCount;
    std::uint16_t textureSlot;
};

struct DecodedTextureFormat {
    std::int32_t componentCount;
    std::int32_t enabled;
    // init_format_desc's control word for the format (0xA8000000 for low byte 0x85); a surface record's second half holds
    // CreateDirectXTexture2D's result here.
    std::uint32_t formatWord;
    std::uint32_t width;
    std::uint32_t height;
    float reserved_14;
    std::uint32_t extraParam;
    float reserved_1c;
    std::uint64_t texture;
};

struct SurfaceRecord {
    DecodedTextureFormat primary;
    DecodedTextureFormat secondary;
};

struct TextureResource {
    std::atomic<std::int32_t> referenceCount;
    std::int32_t width;
    std::int32_t height;
    std::int32_t arraySize;
    std::int32_t mipLevels;
    std::uint32_t format;
    std::uint32_t bindFlags;
    std::uint32_t flags;
    std::uint32_t sampleCount;
    std::uint32_t sampleQuality;
    void *texture;
    void *shaderResourceView;
    void *secondaryShaderResourceView;
    void *stagingTexture;
    void *view;
    void (*releaseCallback)(void *);
    void *releaseCallbackArgument;
    std::uint64_t reserved;
};

struct QueuedTextureCopy {
    void *sourceTexture;
    std::uint32_t destinationHandle;
    std::uint32_t firstMip;
    std::uint32_t endMip;
    std::uint32_t reserved;
};



extern std::atomic_flag g_textureCreationLock;
extern std::vector<QueuedTextureCopy> g_queuedTextureCopies;
extern std::mutex g_generateMipsMutex;
extern std::vector<std::uint32_t> g_generateMipsTextures;

// Holds the texture creation lock for its lifetime.
class TextureCreationLock {
  public:
    TextureCreationLock();
    ~TextureCreationLock();
};

static_assert(sizeof(TextureUploadDescription) == 0x0c);
static_assert(sizeof(SurfaceMapping) == 4);
static_assert(sizeof(DecodedTextureFormat) == 0x28);
static_assert(sizeof(SurfaceRecord) == 0x50);
static_assert(sizeof(TextureResource) == 0x68);
static_assert(sizeof(QueuedTextureCopy) == 0x18);

std::uint32_t CreateTexture2D(const Texture2DDescription *description);
std::int64_t InitTexture2DResource(std::uint32_t textureHandle);
std::int64_t IncrementTextureById(std::uint32_t textureHandle);
std::int64_t AssignShaderCallback(std::uint32_t textureHandle, void (*callback)(void *), void *callbackArgument);
bool GetTexture2DDimensions(std::uint32_t textureHandle, std::uint32_t *width, std::uint32_t *height);
bool GetTexture2DDescription(std::uint32_t textureHandle, Texture2DDescription *description);
bool ReplaceTexture2DResource(std::uint32_t textureHandle, const Texture2DDescription *description);
struct D3TTextureBackendView {
    void *texture;
    void *shaderResourceView;
    void *secondaryShaderResourceView;
    void *stagingTexture;
    void *view;
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t arraySize;
    std::uint32_t mipLevels;
    std::uint32_t format;
    std::uint32_t bindFlags;
    std::uint32_t flags;
};

bool GetTexture2DBackendView(std::uint32_t textureHandle, D3TTextureBackendView *view);
std::uint32_t GetD3TNativeTextureFormat(std::uint32_t format);
std::int64_t GetStride(std::int32_t width, std::int32_t height, std::uint32_t format);
inline constexpr std::uint32_t kInvalidLegacyTextureFormat = UINT32_MAX;
// CreateDirectXTexture2D's PVR format table: the high byte selects the family, the low byte the D3T format.
std::uint32_t DecodeS1LegacyTextureFormat(std::uint16_t format);
// init_format_desc's family table: the PVR format's high byte selects the source flags CreatePVRForDirectX tests.
inline constexpr std::uint32_t kPvrFamilySourceFlags[][2]{{0x100, 0x4}, {0x200, 0x5}, {0x300, 0x8}, {0x400, 0x9}, {0x500, 0x24}, {0x600, 0x25}, {0x700, 0x24},
    {0x800, 0x25}, {0x900, 0x0}, {0xa00, 0x1}, {0xb00, 0x10}, {0xc00, 0x11}, {0xd00, 0x4}, {0xe00, 0x0}, {0xf00, 0x1}, {0x1000, 0x8}, {0x1100, 0x9},
    {0x8000, 0x80}, {0x8700, 0x81}, {0x8100, 0x100}, {0x8200, 0x101}, {0x8300, 0x200}, {0x8400, 0x201}};
void init_format_desc(DecodedTextureFormat &out, std::uint16_t formatType);
std::uint32_t CreateDirectXTexture2D(DecodedTextureFormat &out, std::uint32_t width, std::uint32_t height, std::uint16_t formatType, const void *pixels);
#if defined(_WIN32)
bool ReplaceTexture2DNativeResource(std::uint32_t textureHandle, std::uint32_t width, std::uint32_t height, std::uint32_t arraySize, std::uint32_t mipLevels,
    std::uint32_t nativeFormat, void *texture, void *shaderResourceView);
std::uint32_t AdoptBackBufferTexture2D(std::uint32_t currentHandle, std::uint32_t width, std::uint32_t height, std::uint32_t format, void *texture,
    void *renderTargetView, void *shaderResourceView);
void ReleaseBackBufferTexture2D(std::uint32_t textureHandle);

namespace Texture2D {
void CopyResource(std::uint32_t textureHandle);
void *MapGpu(std::uint32_t textureHandle, std::uint32_t *rowPitch);
void UnmapGpu(std::uint32_t textureHandle);
} // namespace Texture2D
#endif
void CreateMipmappedScaledTexture(
    std::uint32_t textureHandle, std::uint32_t firstMip, std::uint32_t mipLevels, const void *pixels, std::uint32_t pixelDataSize);
void GenerateMipsAndUpdateResourcesForDirectX();

void InitializePvrTextureSlotStorage();
void InitPVRTaskData(std::int32_t surfaceIndex);
std::uint32_t GetPVRSurfaceTextureHandle(std::int32_t surfaceIndex);
const SurfaceRecord *GetPVRSurfaceRecord(std::int32_t surfaceIndex);


std::uint32_t ExchangePVRSurfaceTexture(std::int32_t surfaceIndex, std::uint32_t texture);
std::int32_t d3t_UploadTexture2DToGPU(std::int32_t surfaceIndex, const void *textureDescription, const void *pixels);
bool CreatePVRTextureBufferForSurface(std::int32_t surfaceIndex, std::uint32_t width, std::uint32_t height, std::uint16_t format, const void *pixels = nullptr);

} // namespace shenmue::d3t
