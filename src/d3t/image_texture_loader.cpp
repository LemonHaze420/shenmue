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
#if defined(SHENMUE_GFX_DXVK_NATIVE)
#include "common/types.hpp"
#include <algorithm>
#include <bit>
#include <vector>
#endif

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

#if defined(SHENMUE_GFX_DXVK_NATIVE)
std::uint32_t DdsLegacyFormat(const DdsPixelFormat &format) {
    if (format.flags & DDS_PIXEL_FOURCC) {
        switch (format.fourCC) {
        case SHEN_NAME(DXT1):
            return 71;
        case SHEN_NAME(DXT2):
        case SHEN_NAME(DXT3):
            return 74;
        case SHEN_NAME(DXT4):
        case SHEN_NAME(DXT5):
            return 77;
        case SHEN_NAME(ATI1):
        case SHEN_NAME(BC4U):
            return 80;
        case SHEN_NAME(ATI2):
        case SHEN_NAME(BC5U):
            return 83;
        case 36:
            return 11;
        case 110:
            return 13;
        case 111:
            return 54;
        case 112:
            return 34;
        case 113:
            return 10;
        case 114:
            return 41;
        case 115:
            return 16;
        case 116:
            return 2;
        default:
            return 0;
        }
    }
    if (format.flags & DDS_PIXEL_RGB) {
        if (format.bitsPerPixel == 32 && format.greenMask == 0xff00) {
            if (format.redMask == 0xff && format.blueMask == 0xff0000)
                return 28;
            if (format.redMask == 0xff0000 && format.blueMask == 0xff)
                return format.alphaMask ? 87 : 88;
        }
        if (format.bitsPerPixel == 16) {
            if (format.redMask == 0xf800 && format.greenMask == 0x7e0 && format.blueMask == 0x1f)
                return 85;
            if (format.redMask == 0x7c00 && format.greenMask == 0x3e0 && format.blueMask == 0x1f && format.alphaMask == 0x8000)
                return 86;
            if (format.redMask == 0xf00 && format.greenMask == 0xf0 && format.blueMask == 0xf && format.alphaMask == 0xf000)
                return 115;
        }
    }
    if (format.flags & DDS_PIXEL_LUMINANCE) {
        if (format.bitsPerPixel == 8 && format.redMask == 0xff)
            return 61;
        if (format.bitsPerPixel == 16 && format.redMask == 0xffff)
            return 56;
        if (format.bitsPerPixel == 16 && format.redMask == 0xff && format.alphaMask == 0xff00)
            return 49;
    }
    return 0;
}

std::uint32_t DdsBitsPerPixel(std::uint32_t format) {
    switch (format) {
    case 2:
        return 128;
    case 6:
        return 96;
    case 10:
    case 11:
    case 12:
    case 13:
    case 16:
    case 20:
        return 64;
    case 24:
    case 26:
    case 28:
    case 29:
    case 31:
    case 34:
    case 35:
    case 40:
    case 41:
    case 42:
    case 45:
    case 87:
    case 88:
    case 91:
    case 93:
        return 32;
    case 49:
    case 54:
    case 56:
    case 57:
    case 85:
    case 86:
    case 115:
        return 16;
    case 61:
    case 65:
        return 8;
    default:
        return 0;
    }
}

bool ReplaceDdsImage(std::uint32_t textureHandle, const std::uint8_t *bytes, std::size_t size, bool linearColour) {
    if (size < 4 + sizeof(DdsHeader) || std::memcmp(bytes, "DDS ", 4) != 0)
        return false;
    DdsHeader header;
    std::memcpy(&header, bytes + 4, sizeof(header));
    if (header.size != sizeof(DdsHeader) || header.pixelFormat.size != sizeof(DdsPixelFormat) || !header.width || !header.height || header.depth > 1 ||
        (header.caps2 & DDS_VOLUME))
        return false;
    const std::uint32_t mipLevels = std::max(1U, header.mipLevels);
    if (mipLevels > static_cast<std::uint32_t>(std::bit_width(std::max(header.width, header.height))))
        return false;
    std::size_t offset = 4 + sizeof(DdsHeader);
    std::uint32_t nativeFormat, arraySize = 1, flags = 0;
    const bool extended = (header.pixelFormat.flags & DDS_PIXEL_FOURCC) && header.pixelFormat.fourCC == SHEN_NAME(DX10);
    if (extended) {
        if (size - offset < sizeof(DdsHeaderDx10))
            return false;
        DdsHeaderDx10 extension;
        std::memcpy(&extension, bytes + offset, sizeof(extension));
        offset += sizeof(extension);
        if (extension.dimension != DDS_DX10_TEXTURE_2D || !extension.arraySize || extension.arraySize > 2048)
            return false;
        arraySize = extension.arraySize;
        nativeFormat = extension.format;
        if (extension.flags & DDS_DX10_TEXTURE_CUBE) {
            if (arraySize > 2048 / 6)
                return false;
            arraySize *= 6;
            flags = 2;
        }
    } else {
        nativeFormat = DdsLegacyFormat(header.pixelFormat);
        if (header.caps2 & DDS_CUBE) {
            if ((header.caps2 & DDS_CUBE_FACES) != DDS_CUBE_FACES)
                return false;
            arraySize = 6;
            flags = 2;
        }
    }
    if (!linearColour) {
        switch (nativeFormat) {
        case 28:
            nativeFormat = 29;
            break;
        case 71:
            nativeFormat = 72;
            break;
        case 74:
            nativeFormat = 75;
            break;
        case 77:
            nativeFormat = 78;
            break;
        case 87:
            nativeFormat = 91;
            break;
        case 88:
            nativeFormat = 93;
            break;
        default:
            break;
        }
    }
    std::uint32_t format = 0;
    for (std::uint32_t candidate = 1; candidate < 52; ++candidate) {
        if (GetD3TNativeTextureFormat(candidate) == nativeFormat) {
            format = candidate;
            break;
        }
    }
    if (!format || !nativeFormat) {
        std::fprintf(stderr, "DDS loader does not support DXGI format %u (FourCC %08x)\n", nativeFormat, header.pixelFormat.fourCC);
        return false;
    }
    std::uint32_t blockBytes = 0;
    if (nativeFormat == 71 || nativeFormat == 72 || nativeFormat == 80)
        blockBytes = 8;
    else if (nativeFormat == 74 || nativeFormat == 75 || nativeFormat == 77 || nativeFormat == 78 || nativeFormat == 83)
        blockBytes = 16;
    const std::uint32_t bits = DdsBitsPerPixel(nativeFormat);
    if (!blockBytes && !bits)
        return false;
    std::vector<DdsSubresource> subresources;
    subresources.reserve(static_cast<std::size_t>(arraySize) * mipLevels);
    for (std::uint32_t layer = 0; layer < arraySize; ++layer) {
        std::uint32_t width = header.width, height = header.height;
        for (std::uint32_t mip = 0; mip < mipLevels; ++mip) {
            const std::uint64_t rowPitch =
                blockBytes ? ((static_cast<std::uint64_t>(width) + 3) / 4) * blockBytes : (static_cast<std::uint64_t>(width) * bits + 7) / 8;
            const std::uint64_t rows = blockBytes ? (static_cast<std::uint64_t>(height) + 3) / 4 : height;
            const std::uint64_t bytesCount = rowPitch * rows;
            if (rowPitch > UINT32_MAX || bytesCount > UINT32_MAX || offset > size || bytesCount > size - offset)
                return false;
            subresources.push_back({.offset = offset, .size = static_cast<std::uint32_t>(bytesCount), .rowPitch = static_cast<std::uint32_t>(rowPitch)});
            offset += static_cast<std::size_t>(bytesCount);
            width = std::max(1U, width >> 1U);
            height = std::max(1U, height >> 1U);
        }
    }
    const Texture2DDescription description{.width = header.width,
        .height = header.height,
        .arraySize = arraySize,
        .mipLevels = mipLevels,
        .format = format,
        .bindFlags = 0,
        .flags = flags,
        .sampleCount = 1,
        .sampleQuality = 0,
        .reserved_24 = 0,
        .initialData = nullptr};
    if (!ReplaceTexture2DResource(textureHandle, &description))
        return false;
    const bool opaqueAlpha = !extended && (header.pixelFormat.flags & DDS_PIXEL_RGB) && header.pixelFormat.bitsPerPixel == 32 && !header.pixelFormat.alphaMask;
    std::vector<std::uint8_t> opaquePixels;
    for (std::uint32_t layer = 0; layer < arraySize; ++layer) {
        for (std::uint32_t mip = 0; mip < mipLevels; ++mip) {
            const DdsSubresource &subresource = subresources[layer * mipLevels + mip];
            const void *pixels = bytes + subresource.offset;
            if (opaqueAlpha) {
                opaquePixels.resize(subresource.size);
                std::memcpy(opaquePixels.data(), pixels, subresource.size);
                for (std::size_t index = 3; index < opaquePixels.size(); index += 4)
                    opaquePixels[index] = 255;
                pixels = opaquePixels.data();
            }
            if (!UploadTexture2DSubresource(textureHandle, mip, layer, pixels, subresource.size, subresource.rowPitch))
                return false;
        }
    }
    return true;
}
#endif

} // namespace

bool ReplaceDecodedImage(std::uint32_t textureHandle, const void *loadedData, std::uint64_t loadedSize, bool linearColour) {
    if (!loadedData || loadedSize == 0 || loadedSize > static_cast<std::uint64_t>((std::numeric_limits<int>::max)())) {
        return false;
    }
    const auto *const bytes = static_cast<const std::uint8_t *>(loadedData);

#if defined(SHENMUE_GFX_DXVK_NATIVE)
    if (loadedSize >= 4 && std::memcmp(bytes, "DDS ", 4) == 0)
        return ReplaceDdsImage(textureHandle, bytes, static_cast<std::size_t>(loadedSize), linearColour);
#endif

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
