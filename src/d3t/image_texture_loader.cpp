#include "d3t/image_texture_loader.hpp"
#include "logging.hpp"

#include "d3t/content.hpp"
#include "d3t/buffer.hpp"
#include "d3t/texture.hpp"

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <limits>

#define STBI_NO_STDIO
#include <stb_image.h>

#if defined(_WIN32) && defined(SHENMUE_HAS_DIRECTXTEX)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <DirectXTex.h>
#include <d3d11.h>
#endif

namespace shenmue::d3t {
namespace {

struct ImageLoadRequest {
    std::uint16_t generation;
    bool linearColour;
    std::byte reserved03;
    std::uint32_t textureHandle;
};

std::array<ImageLoadRequest, 8192> g_imageLoadRequests{};
std::atomic<std::uint16_t> g_imageLoadRequestIndex{0};

} // namespace

bool ReplaceDecodedImage(std::uint32_t textureHandle, const void *loadedData, std::uint64_t loadedSize, bool linearColour) {
    if (!loadedData || loadedSize == 0 || loadedSize > static_cast<std::uint64_t>((std::numeric_limits<int>::max)())) {
        return false;
    }
    const auto *const bytes = static_cast<const std::uint8_t *>(loadedData);

#if defined(_WIN32) && defined(SHENMUE_HAS_DIRECTXTEX)
    if (loadedSize >= 3 && std::memcmp(bytes, "DDS", 3) == 0) {
        DirectX::TexMetadata metadata{};
        DirectX::ScratchImage image;
        if (FAILED(DirectX::LoadFromDDSMemory(bytes, static_cast<std::size_t>(loadedSize), DirectX::DDS_FLAGS_NONE, &metadata, image))) {
            return false;
        }
        if (!linearColour) {
            metadata.format = DirectX::MakeSRGB(metadata.format);
            if (!image.OverrideFormat(metadata.format))
                return false;
        }
        auto *const device = static_cast<ID3D11Device *>(g_D3DDevice);
        ID3D11ShaderResourceView *shaderView = nullptr;
        if (!device || FAILED(DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metadata, &shaderView))) {
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

void ImageLoaded(void *data, std::uint32_t, void *loadedData, std::uint64_t loadedSize, const char *sourcePath) {
    auto *const request = static_cast<ImageLoadRequest *>(data);
    if (!request || !request->textureHandle)
        return;
     ReplaceDecodedImage(request->textureHandle, loadedData, loadedSize, request->linearColour);
}

} // namespace

std::int64_t LoadImageTexture(const char *path, bool linearColour, std::uint32_t *textureHandle, std::uint32_t renderHandle) {
    if (!path || !textureHandle)
        return -4;
    const Texture2DDescription emptyDescription{};
    const std::uint32_t texture = CreateTexture2D(&emptyDescription);
    if (!texture)
        return 0;

    const std::uint16_t index = g_imageLoadRequestIndex.fetch_add(1, std::memory_order_seq_cst) & 0x1fffU;
    ImageLoadRequest &request = g_imageLoadRequests[index];
    ++request.generation;
    request.linearColour = linearColour;
    request.textureHandle = texture;
    *textureHandle = texture;

    const D3TShaderLoadTask task{
        path,
        renderHandle,
        0,
        {ImageLoaded, &request},
        nullptr,
        0,
        0,
        {},
    };
    return PushShaderLoadToThreadQueue(&task);
}

std::uint32_t CreateImageTextureFromMemory(const void *data, std::size_t size, bool linearColour) {
    const Texture2DDescription emptyDescription{};
    const std::uint32_t texture = CreateTexture2D(&emptyDescription);
    if (!texture)
        return 0;
    if (!ReplaceDecodedImage(texture, data, size, linearColour)) {
        InitTexture2DResource(texture);
        return 0;
    }
    return texture;
}

} // namespace shenmue::d3t
