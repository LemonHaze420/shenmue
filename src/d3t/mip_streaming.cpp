#include "d3t/mip_streaming.hpp"

#include "d3t/archive_data.hpp"
#include "d3t/threads.hpp"
#include "d3t/content.hpp"
#include "d3t/asset_remap.hpp"
#include "d3t/buffer.hpp"
#include "d3t/texture.hpp"
#include "d3t/shader_load.hpp"
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cstdio>
#include <cstring>
#include <limits>
#include <mutex>
#include <shared_mutex>
#include <cstdlib>

#define STBI_NO_STDIO
#include <stb_image.h>

#if defined(_WIN32) && defined(SHENMUE_HAS_DIRECTXTEX)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <DirectXTex.h>
#include <d3d11.h>
#endif

namespace shenmue::d3t {

std::uint32_t g_pendingMipMapRequestCount = 0;
std::array<std::uint64_t, 9> g_mipMapFrameCounters{};
bool g_loadTexturesDirectly = false;

namespace {

std::mutex g_mipRequestMutex;
std::mutex g_mipProducerMutex;
MipMapRequest *g_mipMapRequests = nullptr;
std::uint32_t g_mipMapRequestCapacity = 0;

std::array<MipTextureMetadata, kMipTextureTableSize> g_mipTextureMetadata{};
std::array<std::uint32_t, kMipTextureTableSize> g_mipTextureHashes{};
std::array<std::uint16_t, kMipTextureTableSize> g_mipTextureLinks{};
std::uint32_t g_mipTextureCount = 0;
std::shared_mutex g_mipTextureMutex;

MipTextureMetadata *FindMipTextureMetadata(std::uint32_t hash) {
    std::uint32_t index = hash & kMipTextureTableMask;
    for (;;) {
        if (g_mipTextureHashes[index] == hash)
            return &g_mipTextureMetadata[index];
        if (g_mipTextureLinks[index] == 0)
            return nullptr;
        index = (index + g_mipTextureLinks[index]) & kMipTextureTableMask;
    }
}

AsyncMipCallbackData *AllocateAsyncMipCallback() {
    void *payload = nullptr;
    d3t_AllocateAsyncLoadRecord(&payload);
    return static_cast<AsyncMipCallbackData *>(payload);
}

void ClearMipTextureReservedWord(std::uint32_t hash) {
    std::unique_lock lock(g_mipTextureMutex);
    if (MipTextureMetadata *const metadata = FindMipTextureMetadata(hash))
        metadata->reserved08 &= 0xffffffff00000000ULL;
}

} // namespace

bool ReplaceWithDecodedTexture(std::uint32_t textureHandle, const void *loadedData, std::uint64_t loadedSize, bool linearColour) {
    if (!loadedData || loadedSize == 0 || loadedSize > static_cast<std::uint64_t>((std::numeric_limits<int>::max)())) {
        return false;
    }
    const auto *const bytes = static_cast<const std::uint8_t *>(loadedData);

#if defined(_WIN32) && defined(SHENMUE_HAS_DIRECTXTEX)
    if (loadedSize >= 3 && std::memcmp(bytes, "DDS", 3) == 0) {
        DirectX::TexMetadata metadata{};
        DirectX::ScratchImage image;
        const HRESULT loadResult = DirectX::LoadFromDDSMemory(bytes, static_cast<std::size_t>(loadedSize), DirectX::DDS_FLAGS_NONE, &metadata, image);
        if (FAILED(loadResult)) {
            return false;
        }
        if (!linearColour) {
            metadata.format = DirectX::MakeSRGB(metadata.format);
            if (!image.OverrideFormat(metadata.format)) {
                return false;
            }
        }

        auto *const device = static_cast<ID3D11Device *>(g_D3DDevice);
        ID3D11ShaderResourceView *shaderView = nullptr;
        const HRESULT viewResult =
            device ? DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metadata, &shaderView) : E_POINTER;
        if (!device || FAILED(viewResult)) {
            return false;
        }
        ID3D11Resource *resource = nullptr;
        ID3D11Texture2D *texture = nullptr;
        shaderView->GetResource(&resource);
        const HRESULT queryResult = resource ? resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&texture)) : E_NOINTERFACE;
        if (resource)
            resource->Release();
        if (FAILED(queryResult) || !texture) {
            shaderView->Release();
            return false;
        }
        D3D11_TEXTURE2D_DESC description{};
        texture->GetDesc(&description);
        const bool replaced = ReplaceTexture2DNativeResource(textureHandle, description.Width, description.Height, description.ArraySize, description.MipLevels,
            static_cast<std::uint32_t>(description.Format), texture, shaderView);
        texture->Release();
        shaderView->Release();
        return replaced;
    }
#endif

    int width = 0;
    int height = 0;
    int components = 0;
    stbi_uc *const pixels = stbi_load_from_memory(bytes, static_cast<int>(loadedSize), &width, &height, &components, 4);
    if (!pixels || width <= 0 || height <= 0) {
        stbi_image_free(pixels);
        return false;
    }
    const Texture2DDescription description{
        static_cast<std::uint32_t>(width),
        static_cast<std::uint32_t>(height),
        1,
        1,
        linearColour ? 2U : 3U,
        0,
        0,
        1,
        0,
        0,
        pixels,
    };
    const bool replaced = ReplaceTexture2DResource(textureHandle, &description);
    stbi_image_free(pixels);
    return replaced;
}

namespace {

void RecreateTextureFromHandle(std::uint32_t textureHandle) {
    constexpr std::array<std::uint32_t, 4> kFailurePixels{
        0xff0000ffU,
        0xff0000ffU,
        0xff0000ffU,
        0xff0000ffU,
    };
    const Texture2DDescription description{
        2,
        2,
        1,
        1,
        2,
        0,
        0,
        1,
        0,
        0,
        kFailurePixels.data(),
    };
    ReplaceTexture2DResource(textureHandle, &description);
}

void LoadTextureInternal(void *callbackData, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *) {
    const std::uint32_t hash = *static_cast<const std::uint32_t *>(callbackData);
    MipTextureMetadata metadata{};
    if (FindMipTexture(hash, &metadata)) {
        if (ReplaceWithDecodedTexture(metadata.textureHandle, loadedData, loadedSize, metadata.source != 0)) {
            ClearMipTextureReservedWord(hash);
            AssignShaderCallback(metadata.textureHandle, ReleaseMipTextureCacheEntry, reinterpret_cast<void *>(static_cast<std::uintptr_t>(hash)));
        } else {
            RecreateTextureFromHandle(metadata.textureHandle);
        }
    }
    ProcessThreadStates(contentHandle, 0);
}

std::int64_t LoadTextureDirect(const char *path, std::uint32_t source, std::uint32_t *textureHandle, std::uint32_t renderHandle) {
    std::lock_guard producerLock(g_mipProducerMutex);
    std::array<char, 272> cleanPath{};
    const std::size_t pathLength = std::strlen(path);
    if (pathLength >= cleanPath.size())
        return 0;
    std::memcpy(cleanPath.data(), path, pathLength + 1);

    std::array<char, 272> remappedPath{};
    std::array<char, 272> fullHashPath{};
    const char *hashPath = cleanPath.data();
    if (g_d3tAssetRemapping.state.load(std::memory_order_seq_cst) == 10) {
        if (FindHashedPathSuffix(cleanPath.data()) && genRemapPath(0, cleanPath.data(), remappedPath.data(), 0, false)) {
            hashPath = remappedPath.data();
        } else {
            GenerateFullHashString(cleanPath.data(), fullHashPath.data());
            if (genRemapPath(0, fullHashPath.data(), remappedPath.data(), 0, false)) {
                hashPath = remappedPath.data();
            }
        }
    }

    const std::uint32_t pathHash = d3t_HashData(hashPath, static_cast<std::uint32_t>(std::strlen(hashPath)));
    const std::uint32_t hash = 0x1003fU * pathHash + 0x2001fU * source;
    MipTextureMetadata existing{};
    if (FindMipTexture(hash, &existing)) {
        *textureHandle = existing.textureHandle;
        return IncrementTextureById(*textureHandle);
    }

    AsyncMipCallbackData *const callback = AllocateAsyncMipCallback();
    if (!callback)
        return 0;
    callback->textureHash = hash;
    callback->firstMip = 0;
    callback->endMip = 0;
    const Texture2DDescription empty{};
    const std::uint32_t texture = CreateTexture2D(&empty);
    if (!texture)
        return 0;
    *textureHandle = texture;
    if (!InsertMipTextureCacheEntry(hash, {source, texture, 0, 0})) {
        InitTexture2DResource(texture);
        *textureHandle = 0;
        return 0;
    }

    const D3TShaderLoadTask task{
        cleanPath.data(),
        renderHandle,
        0,
        {LoadTextureInternal, &callback->textureHash},
        nullptr,
        0,
        0,
        {},
    };
    const std::int64_t contentHandle = PushShaderLoadToThreadQueue(&task);
    const std::uint32_t index = static_cast<std::uint32_t>(contentHandle) >> 16;
    if (index < g_d3tContentSlots.size() && g_d3tContentSlots[index].generation == static_cast<std::uint16_t>(contentHandle))
        return g_d3tContentSlots[index].reserved_08;
    return contentHandle;
}

} // namespace

std::int64_t LoadTextureFromFilepath(const char *path, std::uint32_t source, std::uint32_t *textureHandle, std::uint32_t renderHandle) {
    if (!path || !textureHandle) {
        return -4;
    }
    return LoadTextureDirect(path, source, textureHandle, renderHandle);
}

MipMapRequest *QueueMipMapRequest(const MipMapRequest &request) {
    std::lock_guard lock(g_mipRequestMutex);
    if (g_pendingMipMapRequestCount >= g_mipMapRequestCapacity) {
        const std::uint32_t capacity = std::max<std::uint32_t>(16, 2 * g_mipMapRequestCapacity);
        MipMapRequest *const data = static_cast<MipMapRequest *>(std::realloc(g_mipMapRequests, sizeof(MipMapRequest) * capacity));
        if (!data)
            return nullptr;
        g_mipMapRequests = data;
        g_mipMapRequestCapacity = capacity;
    }
    MipMapRequest *const queued = g_mipMapRequests + g_pendingMipMapRequestCount++;
    *queued = request;
    return queued;
}

bool InsertMipTextureCacheEntry(std::uint32_t hash, const MipTextureMetadata &metadata) {
    std::unique_lock lock(g_mipTextureMutex);
    if (g_mipTextureCount >= kMipTextureTableSize)
        return false;
    std::uint32_t index = hash & kMipTextureTableMask;
    std::uint32_t firstFree = UINT32_MAX;
    for (;;) {
        if (g_mipTextureHashes[index] == hash)
            return true;
        if (g_mipTextureHashes[index] == 0) {
            if (firstFree == UINT32_MAX)
                firstFree = index;
            if (g_mipTextureLinks[index] == 0) {
                g_mipTextureMetadata[index] = metadata;
                g_mipTextureHashes[index] = hash;
                ++g_mipTextureCount;
                return true;
            }
        }
        if (g_mipTextureLinks[index] == 0)
            break;
        index = (index + g_mipTextureLinks[index]) & kMipTextureTableMask;
    }
    if (firstFree != UINT32_MAX) {
        g_mipTextureMetadata[firstFree] = metadata;
        g_mipTextureHashes[firstFree] = hash;
        ++g_mipTextureCount;
        return true;
    }
    std::uint32_t freeIndex = (index + 1) & kMipTextureTableMask;
    while (freeIndex != index && g_mipTextureHashes[freeIndex] != 0)
        freeIndex = (freeIndex + 1) & kMipTextureTableMask;
    if (freeIndex == index)
        return false;
    g_mipTextureMetadata[freeIndex] = metadata;
    g_mipTextureHashes[freeIndex] = hash;
    g_mipTextureLinks[index] = static_cast<std::uint16_t>((freeIndex - index) & kMipTextureTableMask);
    ++g_mipTextureCount;
    return true;
}

bool FindMipTexture(std::uint32_t hash, MipTextureMetadata *metadata) {
    std::shared_lock lock(g_mipTextureMutex);
    MipTextureMetadata *const entry = FindMipTextureMetadata(hash);
    if (!entry)
        return false;
    if (metadata)
        *metadata = *entry;
    return true;
}

bool RemoveMipTexture(std::uint32_t hash) {
    std::unique_lock lock(g_mipTextureMutex);
    std::uint32_t index = hash & kMipTextureTableMask;
    std::uint32_t previous = index;
    if (g_mipTextureHashes[index] != hash) {
        for (;;) {
            const std::uint16_t distance = g_mipTextureLinks[index];
            if (distance == 0)
                return false;
            previous = index;
            index = (index + distance) & kMipTextureTableMask;
            if (g_mipTextureHashes[index] == hash)
                break;
        }
    }

    g_mipTextureHashes[index] = 0;
    g_mipTextureMetadata[index] = {};
    if (index != previous && g_mipTextureLinks[index] == 0)
        g_mipTextureLinks[previous] = 0;
    --g_mipTextureCount;
    return true;
}

void ReleaseMipTextureCacheEntry(void *hash) {
    RemoveMipTexture(static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(hash)));
}

void CreateTexture2DAsync(void *callbackData, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *) {
    const AsyncMipCallbackData *const request = static_cast<const AsyncMipCallbackData *>(callbackData);
    MipTextureMetadata texture{};
    if (FindMipTexture(request->textureHash, &texture)) {
        CreateMipmappedScaledTexture(texture.textureHandle, request->firstMip, request->endMip, loadedData, static_cast<std::uint32_t>(loadedSize));
    }
    ProcessThreadStates(contentHandle, 0);
}

void LoadMipMaps() {
    std::lock_guard lock(g_mipRequestMutex);
    std::uint32_t queued = 0;
    std::size_t index = 0;
    while (index < g_pendingMipMapRequestCount && queued < 100) {
        MipMapRequest &request = g_mipMapRequests[index];
        bool submitted = false;
        for (std::uint32_t mip = 0; mip <= request.maximumMip; ++mip) {
            const bool alreadyRequested = mip < 16 && ((request.requestedMipMask >> mip) & 1U) != 0;
            if (alreadyRequested)
                continue;

            AsyncMipCallbackData *const callback = AllocateAsyncMipCallback();
            if (!callback)
                continue;

            std::array<char, 272> path{};
            const int length = std::snprintf(path.data(), path.size(), "%s&mip=%u", request.path, mip);
            if (length < 0 || static_cast<std::size_t>(length) >= path.size()) {
                continue;
            }

            callback->textureHash = request.textureHash;
            callback->firstMip = static_cast<std::uint8_t>(request.initialMip - static_cast<std::uint8_t>(mip));
            callback->endMip = static_cast<std::uint8_t>(callback->firstMip + 1);
            const D3TShaderLoadTask task{
                path.data(),
                0,
                0,
                {CreateTexture2DAsync, &callback->textureHash},
                nullptr,
                0,
                0,
                {},
            };
            PushShaderLoadToThreadQueue(&task);
            if (mip < 16) {
                request.requestedMipMask = static_cast<std::uint16_t>(request.requestedMipMask | (1U << mip));
            }
            ++queued;
            ++index;
            submitted = true;
            break;
        }

        if (!submitted) {
            std::memmove(g_mipMapRequests + index, g_mipMapRequests + index + 1, sizeof(MipMapRequest) * (g_pendingMipMapRequestCount - index - 1));
            --g_pendingMipMapRequestCount;
        }
    }
}

std::int64_t SelectMipsForPVR(const PvrMipDescription *description, std::int32_t textureMipMode) {
    if (!description)
        return 0;

    constexpr std::uint32_t kMinimumMipCount = 10;
    std::uint32_t selectedMipCount = 16;
    if (textureMipMode == 0) {
        selectedMipCount = 9;
    } else if (textureMipMode == 1) {
        selectedMipCount = 11;
    }
    selectedMipCount = std::max(selectedMipCount, kMinimumMipCount);
    selectedMipCount = std::min(selectedMipCount, static_cast<std::uint32_t>(description->mipCount));

    std::uint32_t width = description->width;
    std::uint32_t height = description->height;
    for (std::uint32_t mip = selectedMipCount; mip < description->mipCount; ++mip) {
        width = std::max(width >> 1U, 1U);
        height = std::max(height >> 1U, 1U);
    }
    if (width == 0 || height == 0 || selectedMipCount == 0)
        return 0;

    const std::uint32_t dimensionMipCount = std::bit_width(std::max(width, height));
    selectedMipCount = std::min(selectedMipCount, dimensionMipCount);

    std::int64_t byteCount = 0;
    for (std::uint32_t mip = 0; mip < selectedMipCount; ++mip) {
        byteCount +=
            GetStride(static_cast<std::int32_t>(std::max(width >> mip, 1U)), static_cast<std::int32_t>(std::max(height >> mip, 1U)), description->format);
    }
    return byteCount;
}

std::int64_t SelectMipForPVR(const char *path, const PvrMipDescription *description, std::uint32_t source, std::uint32_t *textureHandle,
    std::uint32_t renderHandle, std::int32_t textureMipMode) {
    if (!path || !description || !textureHandle)
        return 0;
    if (g_loadTexturesDirectly) {
        return LoadTextureDirect(path, source, textureHandle, renderHandle);
    }
    if (description->format == 0) {
        *textureHandle = d3t::g_defaultWhiteTexture;
        return IncrementTextureById(*textureHandle);
    }

    std::lock_guard producerLock(g_mipProducerMutex);
    std::array<char, 272> lowMipPath{};
    const int length = std::snprintf(lowMipPath.data(), lowMipPath.size(), "%s&mip=LOW", path);
    if (length < 0 || static_cast<std::size_t>(length) >= lowMipPath.size()) {
        return 0;
    }

    std::array<char, 272> remappedPath{};
    std::array<char, 272> fullHashPath{};
    const char *hashPath = lowMipPath.data();
    if (g_d3tAssetRemapping.state.load(std::memory_order_seq_cst) == 10) {
        if (FindHashedPathSuffix(lowMipPath.data()) && genRemapPath(0, lowMipPath.data(), remappedPath.data(), 0, false)) {
            hashPath = remappedPath.data();
        } else {
            GenerateFullHashString(lowMipPath.data(), fullHashPath.data());
            if (genRemapPath(0, fullHashPath.data(), remappedPath.data(), 0, false)) {
                hashPath = remappedPath.data();
            }
        }
    }

    const std::uint32_t pathHash = d3t_HashData(hashPath, static_cast<std::uint32_t>(std::strlen(hashPath)));
    const std::uint32_t textureHash = 0x1003fU * pathHash + 0x2001fU * source;
    MipTextureMetadata existing{};
    if (FindMipTexture(textureHash, &existing)) {
        *textureHandle = existing.textureHandle;
        return IncrementTextureById(*textureHandle);
    }

    std::uint32_t selectedMipCount = 16;
    if (textureMipMode == 0) {
        selectedMipCount = 9;
    } else if (textureMipMode == 1) {
        selectedMipCount = 11;
    }
    constexpr std::uint32_t kMinimumMipCount = 10;
    selectedMipCount = std::max(selectedMipCount, kMinimumMipCount);
    selectedMipCount = std::min(selectedMipCount, static_cast<std::uint32_t>(description->mipCount));
    AsyncMipCallbackData *const callback = AllocateAsyncMipCallback();
    if (!callback)
        return 0;
    callback->textureHash = textureHash;

    std::uint32_t width = description->width;
    std::uint32_t height = description->height;
    for (std::uint32_t mip = selectedMipCount; mip < description->mipCount; ++mip) {
        width = std::max(width >> 1U, 1U);
        height = std::max(height >> 1U, 1U);
    }
    const Texture2DDescription textureDescription{.width = width,
        .height = height,
        .arraySize = 1,
        .mipLevels = selectedMipCount,
        .format = description->format,
        .bindFlags = 0,
        .flags = 8,
        .sampleCount = 1,
        .sampleQuality = 0,
        .reserved_24 = 0,
        .initialData = nullptr};
    const std::uint32_t texture = CreateTexture2D(&textureDescription);
    if (texture == 0)
        return 0;
    *textureHandle = texture;

    const MipTextureMetadata metadata{
        .source = source, .textureHandle = texture, .reserved08 = 0, .loadedSize = static_cast<std::uint64_t>(SelectMipsForPVR(description, textureMipMode))};
    if (!InsertMipTextureCacheEntry(textureHash, metadata)) {
        InitTexture2DResource(texture);
        *textureHandle = 0;
        return 0;
    }
    AssignShaderCallback(texture, ReleaseMipTextureCacheEntry, reinterpret_cast<void *>(static_cast<std::uintptr_t>(textureHash)));

    callback->firstMip = static_cast<std::uint8_t>(selectedMipCount > kMinimumMipCount ? selectedMipCount - kMinimumMipCount : 0);
    callback->endMip = static_cast<std::uint8_t>(selectedMipCount);
    const D3TShaderLoadTask task{
        lowMipPath.data(),
        renderHandle,
        0,
        {CreateTexture2DAsync, &callback->textureHash},
        nullptr,
        0,
        0,
        {},
    };
    std::int64_t result = PushShaderLoadToThreadQueue(&task);
    const std::uint32_t index = static_cast<std::uint32_t>(result) >> 16;
    if (index < g_d3tContentSlots.size() && g_d3tContentSlots[index].generation == static_cast<std::uint16_t>(result))
        result = g_d3tContentSlots[index].reserved_08;

    if (selectedMipCount > kMinimumMipCount && textureMipMode != 0) {
        MipMapRequest request{};
        std::memcpy(request.path, path, std::strlen(path) + 1);
        request.textureHash = textureHash;
        request.requestedMipMask = static_cast<std::uint16_t>((1U << (kMinimumMipCount + 1U)) - 1U);
        request.initialMip = static_cast<std::uint8_t>(selectedMipCount);
        request.maximumMip = static_cast<std::uint8_t>(selectedMipCount);
        result = static_cast<std::int64_t>(reinterpret_cast<std::intptr_t>(QueueMipMapRequest(request)));
    }
    return result;
}

} // namespace shenmue::d3t
