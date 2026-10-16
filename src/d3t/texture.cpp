#include "d3t/texture.hpp"

#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <vector>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#include <dxgiformat.h>
#endif

namespace shenmue::d3t {
std::uint32_t g_defaultWhiteTexture = 0;
std::uint32_t g_defaultBlackTexture = 0;
std::uint32_t g_defaultQuarterTexture = 0;
std::uint32_t g_defaultThreeQuarterTexture = 0;
std::uint32_t g_defaultNormalTexture = 0;
std::uint32_t RGBA8_Create2x2Texture2D(const Vector4 *colour) {
    const std::uint8_t red = static_cast<std::uint8_t>(static_cast<std::int32_t>(colour->x * 255.0f));
    const std::uint8_t green = static_cast<std::uint8_t>(static_cast<std::int32_t>(colour->y * 255.0f));
    const std::uint8_t blue = static_cast<std::uint8_t>(static_cast<std::int32_t>(colour->z * 255.0f));
    const std::uint8_t alpha = static_cast<std::uint8_t>(static_cast<std::int32_t>(colour->w * 255.0f));
    std::array<std::uint8_t, 16> pixels{};
    for (std::size_t offset = 0; offset < pixels.size(); offset += 4) {
        pixels[offset] = red;
        pixels[offset + 1] = green;
        pixels[offset + 2] = blue;
        pixels[offset + 3] = alpha;
    }
    const Texture2DDescription description{.width = 2,
        .height = 2,
        .arraySize = 1,
        .mipLevels = 1,
        .format = 2,
        .bindFlags = 0,
        .flags = 0,
        .sampleCount = 1,
        .sampleQuality = 0,
        .reserved_24 = 0,
        .initialData = pixels.data()};
    return CreateTexture2D(&description);
}

void CreateTemp2x2Textures() {
    g_defaultWhiteTexture = RGBA8_Create2x2Texture2D(&kDefaultWhiteColour);
    g_defaultBlackTexture = RGBA8_Create2x2Texture2D(&kDefaultBlackColour);
    g_defaultQuarterTexture = RGBA8_Create2x2Texture2D(&kDefaultQuarterColour);
    g_defaultThreeQuarterTexture = RGBA8_Create2x2Texture2D(&kDefaultThreeQuarterColour);
    g_defaultNormalTexture = RGBA8_Create2x2Texture2D(&kDefaultNormalColour);
}



std::int32_t g_TextureRefCount = 0;

namespace {

constexpr std::size_t kSurfaceCount = 640;
constexpr std::size_t kTextureCount = 0x4000;
constexpr std::uint32_t kTextureIndexMask = 0x3fffU;
constexpr std::uint32_t kTextureTagMask = 0xffffc000U;
constexpr std::uint32_t kTextureOccupied = 0x80000000U;

std::array<SurfaceMapping, kSurfaceCount> g_surfaceMappings{};
std::array<std::uint16_t, kSurfaceCount> g_freeTextureSlots{};
std::array<SurfaceRecord, kSurfaceCount> g_surfaceRecords{};
std::uint32_t g_freeTextureHead = 0;
std::uint32_t g_freeTextureTail = kSurfaceCount - 1;
std::uint32_t g_allocatedTextureSlotCount = 0;
std::int32_t g_surfaceBase = 0;

std::array<TextureResource, kTextureCount> g_textureResources{};
std::array<std::uint32_t, kTextureCount> g_textureTags{};
std::uint32_t g_nextTextureIndex = 0;
std::atomic_flag g_textureLock = ATOMIC_FLAG_INIT;

class TextureLock {
  public:
    TextureLock() {
        while (g_textureLock.test_and_set(std::memory_order_acquire)) {
        }
    }

    ~TextureLock() {
        g_textureLock.clear(std::memory_order_release);
    }
};

} // namespace

std::atomic_flag g_textureCreationLock = ATOMIC_FLAG_INIT;
std::vector<QueuedTextureCopy> g_queuedTextureCopies;
std::mutex g_generateMipsMutex;
std::vector<std::uint32_t> g_generateMipsTextures;

TextureCreationLock::TextureCreationLock() {
    while (g_textureCreationLock.test_and_set(std::memory_order_acquire)) {
    }
}

TextureCreationLock::~TextureCreationLock() {
    g_textureCreationLock.clear(std::memory_order_release);
}

namespace {

std::uint32_t TextureStride(std::int32_t width, std::int32_t height, std::uint32_t format) {
    switch (format) {
    case 2:
    case 3:
    case 0x25:
    case 0x26:
        return static_cast<std::uint32_t>(4 * width * height);
    case 9:
    case 0x2b:
        return static_cast<std::uint32_t>(width * height);
    case 0x17:
    case 0x1c:
        return static_cast<std::uint32_t>(8 * ((width + 3) >> 2) * ((height + 3) >> 2));
    case 0x18:
    case 0x19:
    case 0x1b:
    case 0x1d:
    case 0x1e:
        return static_cast<std::uint32_t>(16 * ((width + 3) >> 2) * ((height + 3) >> 2));
    case 0x23:
    case 0x24:
        return static_cast<std::uint32_t>(2 * width * height);
    default:
        return 0;
    }
}

} // namespace





std::uint32_t DecodeS1LegacyTextureFormat(std::uint16_t format) {
    const std::uint16_t type = format & 0xff00U;
    const std::uint8_t code = static_cast<std::uint8_t>(format);
    switch (type) {
    case 0x0100:
    case 0x0300:
    case 0x0900:
    case 0x8100:
    case 0x8300:
    case 0x8500:
    case 0x8600:
        switch (code) {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x04:
        case 0x83:
        case 0x89:
            return 2;
        default:
            return 4;
        }
    case 0x8000:
    case 0x8700:
        switch (code) {
        case 0x00:
        case 0x86:
            return 33;
        case 0x01:
        case 0x87:
            return 31;
        case 0x02:
        case 0x85:
            return 35;
        case 0x80:
            return 23;
        case 0x81:
            return 24;
        case 0x82:
            return 25;
        case 0x84:
            return 4;
        case 0x83:
        case 0x88:
        case 0x89:
            return 2;
        default:
            return kInvalidLegacyTextureFormat;
        }
    default:
        return kInvalidLegacyTextureFormat;
    }
}

namespace {

#if defined(_WIN32)
constexpr std::array<DXGI_FORMAT, 52> kSupportedTextureFormats{
    DXGI_FORMAT_UNKNOWN,
    static_cast<DXGI_FORMAT>(0x1f),
    static_cast<DXGI_FORMAT>(0x1c),
    static_cast<DXGI_FORMAT>(0x1d),
    static_cast<DXGI_FORMAT>(0x1c),
    static_cast<DXGI_FORMAT>(0x1d),
    static_cast<DXGI_FORMAT>(0x1a),
    static_cast<DXGI_FORMAT>(0x0c),
    static_cast<DXGI_FORMAT>(0x0a),
    static_cast<DXGI_FORMAT>(0x3d),
    static_cast<DXGI_FORMAT>(0x31),
    static_cast<DXGI_FORMAT>(0x39),
    static_cast<DXGI_FORMAT>(0x38),
    static_cast<DXGI_FORMAT>(0x36),
    static_cast<DXGI_FORMAT>(0x23),
    static_cast<DXGI_FORMAT>(0x22),
    static_cast<DXGI_FORMAT>(0x2a),
    static_cast<DXGI_FORMAT>(0x29),
    static_cast<DXGI_FORMAT>(0x28),
    static_cast<DXGI_FORMAT>(0x2d),
    static_cast<DXGI_FORMAT>(0x10),
    static_cast<DXGI_FORMAT>(0x06),
    static_cast<DXGI_FORMAT>(0x02),
    static_cast<DXGI_FORMAT>(0x47),
    static_cast<DXGI_FORMAT>(0x4a),
    static_cast<DXGI_FORMAT>(0x4d),
    static_cast<DXGI_FORMAT>(0x50),
    static_cast<DXGI_FORMAT>(0x53),
    static_cast<DXGI_FORMAT>(0x48),
    static_cast<DXGI_FORMAT>(0x4b),
    static_cast<DXGI_FORMAT>(0x4e),
    static_cast<DXGI_FORMAT>(0x55),
    static_cast<DXGI_FORMAT>(0x55),
    static_cast<DXGI_FORMAT>(0x56),
    static_cast<DXGI_FORMAT>(0x56),
    static_cast<DXGI_FORMAT>(0x73),
    static_cast<DXGI_FORMAT>(0x73),
    static_cast<DXGI_FORMAT>(0x57),
    static_cast<DXGI_FORMAT>(0x5b),
    static_cast<DXGI_FORMAT>(0x58),
    static_cast<DXGI_FORMAT>(0x5d),
    static_cast<DXGI_FORMAT>(0x14),
    static_cast<DXGI_FORMAT>(0x18),
    static_cast<DXGI_FORMAT>(0x41),
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    static_cast<DXGI_FORMAT>(0x01010101),
    static_cast<DXGI_FORMAT>(0x02020202),
    static_cast<DXGI_FORMAT>(0x03030303),
    static_cast<DXGI_FORMAT>(0x04040404),
    static_cast<DXGI_FORMAT>(0x05050505),
    DXGI_FORMAT_UNKNOWN,
};
#endif

TextureResource *GetTextureResource(std::uint32_t handle) {
    if (handle == 0) {
        return nullptr;
    }
    const std::size_t index = handle & kTextureIndexMask;
    if (g_textureTags[index] != (handle & kTextureTagMask)) {
        return nullptr;
    }
    return &g_textureResources[index];
}

void ReleaseTextureObjects(TextureResource &resource) {
#if defined(_WIN32)
    if (resource.texture) {
        static_cast<ID3D11Texture2D *>(resource.texture)->Release();
        resource.texture = nullptr;
    }
    if (resource.stagingTexture) {
        static_cast<ID3D11Texture2D *>(resource.stagingTexture)->Release();
        resource.stagingTexture = nullptr;
    }
    if (resource.shaderResourceView) {
        static_cast<ID3D11ShaderResourceView *>(resource.shaderResourceView)->Release();
        resource.shaderResourceView = nullptr;
    }
    if (resource.secondaryShaderResourceView) {
        static_cast<ID3D11ShaderResourceView *>(resource.secondaryShaderResourceView)->Release();
        resource.secondaryShaderResourceView = nullptr;
    }
    if ((resource.bindFlags & 8U) == 0 && (resource.bindFlags & 4U) != 0 && resource.arraySize > 1) {
        auto **const views = static_cast<ID3D11View **>(resource.view);
        if (views) {
            for (std::int32_t arrayIndex = 0; arrayIndex < resource.arraySize; ++arrayIndex) {
                for (std::int32_t mipIndex = 0; mipIndex < resource.mipLevels; ++mipIndex) {
                    ID3D11View *&view = views[arrayIndex * resource.mipLevels + mipIndex];
                    if (view) {
                        view->Release();
                        view = nullptr;
                    }
                }
            }
        }
    } else if (resource.view) {
        static_cast<ID3D11View *>(resource.view)->Release();
        resource.view = nullptr;
    }
#else
    resource.texture = nullptr;
    resource.stagingTexture = nullptr;
    resource.shaderResourceView = nullptr;
    resource.secondaryShaderResourceView = nullptr;
    resource.view = nullptr;
#endif
    if (resource.releaseCallback) {
        resource.releaseCallback(resource.releaseCallbackArgument);
    }
}

#if defined(_WIN32)
DXGI_FORMAT NativeTextureFormat(std::uint32_t format) {
    return format < kSupportedTextureFormats.size() ? kSupportedTextureFormats[format] : DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT NativeDepthResourceFormat(std::uint32_t format) {
    switch (format) {
    case 2:
        return DXGI_FORMAT_R8G8B8A8_TYPELESS;
    case 18:
        return DXGI_FORMAT_R32_TYPELESS;
    case 19:
        return DXGI_FORMAT_R24G8_TYPELESS;
    case 41:
        return DXGI_FORMAT_R32G8X24_TYPELESS;
    default:
        return DXGI_FORMAT_UNKNOWN;
    }
}

DXGI_FORMAT NativeDepthShaderFormat(std::uint32_t format) {
    switch (format) {
    case 18:
        return DXGI_FORMAT_R32_FLOAT;
    case 19:
        return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    case 41:
        return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
    default:
        return NativeTextureFormat(format);
    }
}

void CreateNativeTexture(TextureResource &resource, const Texture2DDescription &description) {
    auto *const device = static_cast<ID3D11Device *>(g_D3DDevice);
    if (!device) {
        return;
    }

    D3D11_TEXTURE2D_DESC native{};
    native.Width = description.width;
    native.Height = description.height;
    native.MipLevels = description.mipLevels;
    native.ArraySize = description.arraySize;
    native.Format = NativeTextureFormat(description.format);
    native.SampleDesc.Count = description.sampleCount;
    native.SampleDesc.Quality = description.sampleQuality;
    native.Usage = D3D11_USAGE_DEFAULT;
    native.BindFlags = (description.bindFlags & 0x20U) ? D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS : D3D11_BIND_SHADER_RESOURCE;
    if (description.bindFlags & 4U) {
        native.BindFlags |= D3D11_BIND_RENDER_TARGET;
    }
    if (description.bindFlags & 8U) {
        native.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
        native.Format = NativeDepthResourceFormat(description.format);
    }
    if (description.flags & 1U) {
        native.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
    }
    if (description.flags & 2U) {
        native.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
    }
    if (description.flags & 8U) {
        native.MiscFlags |= D3D11_RESOURCE_MISC_RESOURCE_CLAMP;
    }

    D3D11_SUBRESOURCE_DATA initialData{};
    initialData.pSysMem = description.initialData;
    initialData.SysMemPitch = TextureStride(static_cast<std::int32_t>(description.width), 1, description.format);
    const HRESULT textureResult =
        device->CreateTexture2D(&native, description.initialData ? &initialData : nullptr, reinterpret_cast<ID3D11Texture2D **>(&resource.texture));

    if (FAILED(textureResult) || !resource.texture) {
        return;
    }

    auto *const texture = static_cast<ID3D11Texture2D *>(resource.texture);
    if (description.bindFlags & 4U) {
        D3D11_RENDER_TARGET_VIEW_DESC view{};
        view.Format = NativeTextureFormat(description.format);
        if (description.arraySize <= 1 && description.mipLevels <= 1) {
            view.ViewDimension = description.sampleCount == 1 ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DMS;
            device->CreateRenderTargetView(texture, &view, reinterpret_cast<ID3D11RenderTargetView **>(&resource.view));
        } else {
            const std::size_t viewCount = static_cast<std::size_t>(description.arraySize) * description.mipLevels;
            auto **const views = static_cast<ID3D11RenderTargetView **>(std::calloc(viewCount, sizeof(ID3D11RenderTargetView*)));
            resource.view = views;
            view.ViewDimension = description.arraySize <= 1 ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            view.Texture2DArray.ArraySize = 1;
            for (std::uint32_t arrayIndex = 0; arrayIndex < description.arraySize; ++arrayIndex) {
                for (std::uint32_t mipIndex = 0; mipIndex < description.mipLevels; ++mipIndex) {
                    if (description.arraySize <= 1) {
                        view.Texture2D.MipSlice = mipIndex;
                    } else {
                        view.Texture2DArray.FirstArraySlice = arrayIndex;
                        view.Texture2DArray.MipSlice = mipIndex;
                    }
                    device->CreateRenderTargetView(texture, &view, &views[arrayIndex * description.mipLevels + mipIndex]);
                }
            }
        }
    }

    if (description.bindFlags & 0x20U) {
        D3D11_UNORDERED_ACCESS_VIEW_DESC view{};
        view.Format = native.Format;
        if (description.arraySize <= 1) {
            view.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
            view.Texture2D.MipSlice = 0;
        } else {
            view.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
            view.Texture2DArray.MipSlice = 0;
            view.Texture2DArray.FirstArraySlice = 0;
            view.Texture2DArray.ArraySize = description.arraySize;
        }
        device->CreateUnorderedAccessView(texture, &view, reinterpret_cast<ID3D11UnorderedAccessView **>(&resource.view));
    }

    if (description.bindFlags & 8U) {
        D3D11_DEPTH_STENCIL_VIEW_DESC view{};
        view.Format = NativeTextureFormat(description.format);
        view.ViewDimension = description.sampleCount == 1 ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DMS;
        device->CreateDepthStencilView(texture, &view, reinterpret_cast<ID3D11DepthStencilView **>(&resource.view));
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderView{};
    shaderView.Format = NativeDepthShaderFormat(description.format);
    if (description.flags & 2U) {
        shaderView.TextureCube.MostDetailedMip = 0;
        shaderView.TextureCube.MipLevels = UINT(-1);
        if (description.arraySize > 6) {
            shaderView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
            shaderView.TextureCubeArray.First2DArrayFace = 0;
            shaderView.TextureCubeArray.NumCubes = description.arraySize / 6;
            device->CreateShaderResourceView(texture, &shaderView, reinterpret_cast<ID3D11ShaderResourceView **>(&resource.shaderResourceView));
        } else {
            shaderView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
            device->CreateShaderResourceView(texture, &shaderView, reinterpret_cast<ID3D11ShaderResourceView **>(&resource.shaderResourceView));
            shaderView = {};
            shaderView.Format = NativeDepthShaderFormat(description.format);
            shaderView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
            shaderView.Texture2DArray.MostDetailedMip = 0;
            shaderView.Texture2DArray.MipLevels = description.mipLevels;
            shaderView.Texture2DArray.FirstArraySlice = 0;
            shaderView.Texture2DArray.ArraySize = description.arraySize;
            device->CreateShaderResourceView(texture, &shaderView, reinterpret_cast<ID3D11ShaderResourceView **>(&resource.secondaryShaderResourceView));
        }
    } else if (description.arraySize > 1) {
        shaderView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
        shaderView.Texture2DArray.MostDetailedMip = 0;
        shaderView.Texture2DArray.MipLevels = description.mipLevels;
        shaderView.Texture2DArray.FirstArraySlice = 0;
        shaderView.Texture2DArray.ArraySize = description.arraySize;
        device->CreateShaderResourceView(texture, &shaderView, reinterpret_cast<ID3D11ShaderResourceView **>(&resource.shaderResourceView));
    } else {
        shaderView.ViewDimension = description.sampleCount == 1 ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DMS;
        shaderView.Texture2D.MostDetailedMip = 0;
        shaderView.Texture2D.MipLevels = description.mipLevels;
        device->CreateShaderResourceView(texture, &shaderView, reinterpret_cast<ID3D11ShaderResourceView **>(&resource.shaderResourceView));
    }

    if (description.bindFlags & 0x10U) {
        native.Usage = D3D11_USAGE_STAGING;
        native.BindFlags = 0;
        native.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        device->CreateTexture2D(&native, nullptr, reinterpret_cast<ID3D11Texture2D **>(&resource.stagingTexture));
    }
}
#else
void CreateNativeTexture(TextureResource &, const Texture2DDescription &) {
}
#endif

std::uint32_t CreateTextureResource(const Texture2DDescription &description, bool createNative = true) {
    const TextureLock lock;
    const std::uint32_t first = g_nextTextureIndex;
    std::uint32_t index = first;
    while (g_textureTags[index] & kTextureOccupied) {
        index = (index + 1) & kTextureIndexMask;
        if (index == first) {
            return 0;
        }
    }

    std::uint32_t generation = (g_textureTags[index] >> 14) + 1;
    if ((generation & 0xfffe0000U) == 0x00020000U) {
        generation = 1;
    }
    const std::uint32_t tag = ((generation | 0xfffe0000U) << 14) & kTextureTagMask;
    const std::uint32_t handle = tag | index;
    g_textureTags[index] = tag;
    g_nextTextureIndex = (index + 1) & kTextureIndexMask;

    TextureResource &resource = g_textureResources[index];
    std::memset(reinterpret_cast<std::byte *>(&resource) + sizeof(resource.referenceCount), 0, sizeof(resource) - sizeof(resource.referenceCount));
    resource.referenceCount.store(1, std::memory_order_release);
    resource.width = static_cast<std::int32_t>(description.width);
    resource.height = static_cast<std::int32_t>(description.height);
    resource.arraySize = static_cast<std::int32_t>(description.arraySize);
    resource.mipLevels = static_cast<std::int32_t>(description.mipLevels);
    resource.format = description.format;
    resource.bindFlags = description.bindFlags;
    resource.flags = description.flags;
    resource.sampleCount = description.sampleCount;
    resource.sampleQuality = description.sampleQuality;
    if (createNative) {
        CreateNativeTexture(resource, description);
    }
    if (description.flags & 8U) {
        const TextureCreationLock queueLock;
        g_queuedTextureCopies.push_back(QueuedTextureCopy{
            nullptr,
            handle,
            description.mipLevels,
            description.mipLevels,
            0,
        });
    }
    return handle;
}

std::int32_t CreateDirectXTexture2DWrapper(std::uint32_t surface, std::uint32_t width, std::uint32_t height, std::uint16_t format, const void *pixels) {
    if (surface < kSurfaceCount && g_surfaceMappings[surface].referenceCount) {
        return 0;
    }
    SurfaceMapping &mapping = g_surfaceMappings[surface];
    if (mapping.textureSlot == 0xffffU) {
        if (g_freeTextureHead == g_freeTextureTail) {
            mapping.textureSlot = 0xffffU;
        } else {
            mapping.textureSlot = g_freeTextureSlots[g_freeTextureHead];
            g_freeTextureHead = (g_freeTextureHead + 1) % kSurfaceCount;
            ++g_allocatedTextureSlotCount;
        }
    }
    ++mapping.referenceCount;
    if (surface >= kSurfaceCount || !mapping.referenceCount || mapping.textureSlot == 0xffffU) {
        return 0;
    }

    SurfaceRecord &record = g_surfaceRecords[mapping.textureSlot];
    record.secondary.formatWord = CreateDirectXTexture2D(record.primary, width, height, format, pixels);
    if (record.secondary.formatWord)
        return 0;
    record.secondary.extraParam = static_cast<std::uint32_t>(record.primary.texture);
    return 1;
}

} // namespace

std::uint32_t CreateTexture2D(const Texture2DDescription *description) {
    return CreateTextureResource(*description);
}


void init_format_desc(DecodedTextureFormat &out, std::uint16_t formatType) {
    out.enabled = 1;
    out.extraParam = 0;
    const std::uint32_t family = formatType & 0xff00U;
    for (const std::uint32_t(&entry)[2] : kPvrFamilySourceFlags) {
        if (entry[0] == family) {
            out.extraParam = entry[1];
            break;
        }
    }
    if (((family - 0x500U) & 0xfffffeffU) == 0U) {
        out.formatWord = 0x28000000U;
    } else if (((family - 0x700U) & 0xfffffeffU) == 0U) {
        out.formatWord = 0x30000000U;
    } else {
        switch (formatType & 0xffU) {
        case 0x00:
        case 0x05:
            out.formatWord = 0;
            break;
        case 0x01:
            out.formatWord = 0x08000000U;
            break;
        case 0x02:
            out.formatWord = 0x10000000U;
            break;
        case 0x03:
        case 0x06:
            out.formatWord = 0x18000000U;
            break;
        case 0x04:
            out.formatWord = 0x20000000U;
            break;
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
            out.formatWord = 0x80000000U | ((formatType & 0x7fU) << 27U);
            break;
        default:
            break;
        }
    }
    out.componentCount = ((family - 0x1000U) & 0xfffffeffU) == 0U ? 3 : 2;
}



std::uint32_t CreateDirectXTexture2D(DecodedTextureFormat &out, std::uint32_t width, std::uint32_t height, std::uint16_t formatType, const void *pixels) {
    const std::uint32_t format = DecodeS1LegacyTextureFormat(formatType);
    if (format == kInvalidLegacyTextureFormat)
        return 4;
    const Texture2DDescription description{.width = width,
        .height = height,
        .arraySize = 1,
        .mipLevels = 1,
        .format = format,
        .bindFlags = 0,
        .flags = 0,
        .sampleCount = 1,
        .sampleQuality = 0,
        .reserved_24 = 0,
        .initialData = pixels};
    out.texture = CreateTexture2D(&description);
    init_format_desc(out, formatType);
    out.width = width;
    out.height = height;
    ++g_TextureRefCount;
    return 0;
}

std::int64_t InitTexture2DResource(std::uint32_t textureHandle) {
    TextureResource *resource = nullptr;
    bool release = false;
    {
        const TextureLock lock;
        resource = GetTextureResource(textureHandle);
        if (resource) {
            release = resource->referenceCount.fetch_sub(1, std::memory_order_acq_rel) == 1;
        }
    }

    if (release) {
        ReleaseTextureObjects(*resource);
        const TextureLock lock;
        const std::size_t index = textureHandle & kTextureIndexMask;
        if (g_textureTags[index] == (textureHandle & kTextureTagMask)) {
            g_textureTags[index] &= ~kTextureOccupied;
        }
    }
    return static_cast<std::int64_t>(textureHandle & kTextureTagMask);
}

std::int64_t IncrementTextureById(std::uint32_t textureHandle) {
    const TextureLock lock;
    TextureResource *const resource = GetTextureResource(textureHandle);
    if (resource) {
        resource->referenceCount.fetch_add(1, std::memory_order_seq_cst);
    }
    return 0;
}

std::int64_t AssignShaderCallback(std::uint32_t textureHandle, void (*callback)(void *), void *callbackArgument) {
    const TextureLock lock;
    TextureResource *const resource = GetTextureResource(textureHandle);
    if (resource) {
        resource->releaseCallback = callback;
        resource->releaseCallbackArgument = callbackArgument;
    }
    return 0;
}

bool GetTexture2DDimensions(std::uint32_t textureHandle, std::uint32_t *width, std::uint32_t *height) {
    const TextureLock lock;
    const TextureResource *const resource = GetTextureResource(textureHandle);
    if (!resource) {
        return false;
    }
    *width = static_cast<std::uint32_t>(resource->width);
    *height = static_cast<std::uint32_t>(resource->height);
    return true;
}

bool GetTexture2DDescription(std::uint32_t textureHandle, Texture2DDescription *description) {
    if (!description) {
        return false;
    }
    const TextureLock lock;
    const TextureResource *const resource = GetTextureResource(textureHandle);
    if (!resource) {
        return false;
    }
    *description = Texture2DDescription{
        static_cast<std::uint32_t>(resource->width),
        static_cast<std::uint32_t>(resource->height),
        static_cast<std::uint32_t>(resource->arraySize),
        static_cast<std::uint32_t>(resource->mipLevels),
        resource->format,
        resource->bindFlags,
        resource->flags,
        resource->sampleCount,
        resource->sampleQuality,
        0,
        nullptr,
    };
    return true;
}

bool ReplaceTexture2DResource(std::uint32_t textureHandle, const Texture2DDescription *description) {
    if (!description)
        return false;
    const TextureLock lock;
    TextureResource *const resource = GetTextureResource(textureHandle);
    if (!resource)
        return false;

    void (*const callback)(void *) = resource->releaseCallback;
    void *const callbackArgument = resource->releaseCallbackArgument;
    resource->releaseCallback = nullptr;
    resource->releaseCallbackArgument = nullptr;
    ReleaseTextureObjects(*resource);
    resource->width = static_cast<std::int32_t>(description->width);
    resource->height = static_cast<std::int32_t>(description->height);
    resource->arraySize = static_cast<std::int32_t>(description->arraySize);
    resource->mipLevels = static_cast<std::int32_t>(description->mipLevels);
    resource->format = description->format;
    resource->bindFlags = description->bindFlags;
    resource->flags = description->flags;
    resource->sampleCount = description->sampleCount;
    resource->sampleQuality = description->sampleQuality;
    resource->reserved = 0;
    CreateNativeTexture(*resource, *description);
    resource->releaseCallback = callback;
    resource->releaseCallbackArgument = callbackArgument;
    return true;
}

bool GetTexture2DBackendView(std::uint32_t textureHandle, D3TTextureBackendView *view) {
    if (!view) {
        return false;
    }
    TextureLock lock;
    const TextureResource *const resource = GetTextureResource(textureHandle);
    if (!resource) {
        *view = {};
        return false;
    }
    view->texture = resource->texture;
    view->shaderResourceView = resource->shaderResourceView;
    view->secondaryShaderResourceView = resource->secondaryShaderResourceView;
    view->stagingTexture = resource->stagingTexture;
    view->view = resource->view;
    view->width = static_cast<std::uint32_t>(resource->width);
    view->height = static_cast<std::uint32_t>(resource->height);
    view->arraySize = static_cast<std::uint32_t>(resource->arraySize);
    view->mipLevels = static_cast<std::uint32_t>(resource->mipLevels);
    view->format = resource->format;
    view->bindFlags = resource->bindFlags;
    view->flags = resource->flags;
    return true;
}

std::uint32_t GetD3TNativeTextureFormat(std::uint32_t format) {
#if defined(_WIN32)
    return static_cast<std::uint32_t>(NativeTextureFormat(format));
#else
    static_cast<void>(format);
    return 0;
#endif
}

std::int64_t GetStride(std::int32_t width, std::int32_t height, std::uint32_t format) {
    switch (format) {
    case 23:
    case 28:
        return static_cast<std::int64_t>(8) * ((width + 3) >> 2) * ((height + 3) >> 2);
    case 24:
    case 25:
    case 27:
    case 29:
    case 30:
        return static_cast<std::int64_t>(16) * ((width + 3) >> 2) * ((height + 3) >> 2);
    case 2:
    case 3:
    case 37:
    case 38:
        return static_cast<std::int64_t>(4) * width * height;
    case 9:
    case 43:
        return static_cast<std::int64_t>(width) * height;
    case 35:
    case 36:
        return static_cast<std::int64_t>(2) * width * height;
    default:
        return 0;
    }
}

#if defined(_WIN32)
bool ReplaceTexture2DNativeResource(std::uint32_t textureHandle, std::uint32_t width, std::uint32_t height, std::uint32_t arraySize, std::uint32_t mipLevels,
    std::uint32_t nativeFormat, void *texture, void *shaderResourceView) {
    const TextureLock lock;
    TextureResource *const resource = GetTextureResource(textureHandle);
    if (!resource)
        return false;

    void (*const callback)(void *) = resource->releaseCallback;
    void *const callbackArgument = resource->releaseCallbackArgument;
    resource->releaseCallback = nullptr;
    resource->releaseCallbackArgument = nullptr;
    ReleaseTextureObjects(*resource);
    resource->width = static_cast<std::int32_t>(width);
    resource->height = static_cast<std::int32_t>(height);
    resource->arraySize = static_cast<std::int32_t>(arraySize);
    resource->mipLevels = static_cast<std::int32_t>(mipLevels);
    resource->format = 0;
    for (std::uint32_t format = 0; format < kSupportedTextureFormats.size(); ++format) {
        if (kSupportedTextureFormats[format] == static_cast<DXGI_FORMAT>(nativeFormat)) {
            resource->format = format;
            break;
        }
    }
    resource->bindFlags = 0;
    resource->flags = 0;
    resource->sampleCount = 1;
    resource->sampleQuality = 0;
    resource->texture = texture;
    resource->shaderResourceView = shaderResourceView;
    if (texture)
        static_cast<ID3D11Texture2D *>(texture)->AddRef();
    if (shaderResourceView) {
        static_cast<ID3D11ShaderResourceView *>(shaderResourceView)->AddRef();
    }
    resource->releaseCallback = callback;
    resource->releaseCallbackArgument = callbackArgument;
    return true;
}

std::uint32_t AdoptBackBufferTexture2D(std::uint32_t currentHandle, std::uint32_t width, std::uint32_t height, std::uint32_t format, void *texture,
    void *renderTargetView, void *shaderResourceView) {
    bool needsHandle = false;
    {
        const TextureLock lock;
        needsHandle = GetTextureResource(currentHandle) == nullptr;
    }
    if (needsHandle) {
        const Texture2DDescription empty{};
        currentHandle = CreateTextureResource(empty, false);
    }
    const TextureLock lock;
    TextureResource *const resource = GetTextureResource(currentHandle);
    if (!resource) {
        return 0;
    }

    ReleaseTextureObjects(*resource);
    resource->width = static_cast<std::int32_t>(width);
    resource->height = static_cast<std::int32_t>(height);
    resource->arraySize = 0;
    resource->mipLevels = 0;
    resource->format = format;
    resource->bindFlags = 4;
    resource->flags = 0;
    resource->sampleCount = 1;
    resource->sampleQuality = 0;
    resource->texture = texture;
    resource->view = renderTargetView;
    resource->shaderResourceView = shaderResourceView;
    if (texture) {
        static_cast<ID3D11Texture2D *>(texture)->AddRef();
    }
    if (renderTargetView) {
        static_cast<ID3D11RenderTargetView *>(renderTargetView)->AddRef();
    }
    if (shaderResourceView) {
        static_cast<ID3D11ShaderResourceView *>(shaderResourceView)->AddRef();
    }
    return currentHandle;
}



void ReleaseBackBufferTexture2D(std::uint32_t textureHandle) {
    const TextureLock lock;
    if (TextureResource *const resource = GetTextureResource(textureHandle)) {
        ReleaseTextureObjects(*resource);
    }
}

namespace Texture2D {

// @verified - p
void CopyResource(std::uint32_t textureHandle) {
    const TextureLock lock;
    if (TextureResource *const resource = GetTextureResource(textureHandle))
        g_immediateContext->CopyResource(static_cast<ID3D11Texture2D *>(resource->stagingTexture), static_cast<ID3D11Texture2D *>(resource->texture));
}

// @verified - p
void *MapGpu(std::uint32_t textureHandle, std::uint32_t *rowPitch) {
    const TextureLock lock;
    void *data = nullptr;
    if (TextureResource *const resource = GetTextureResource(textureHandle)) {
        D3D11_MAPPED_SUBRESOURCE mapped{};
        if (SUCCEEDED(g_immediateContext->Map(static_cast<ID3D11Texture2D *>(resource->stagingTexture), 0, D3D11_MAP_READ, 0, &mapped))) {
            data = mapped.pData;
            *rowPitch = mapped.RowPitch;
        }
    }
    return data;
}

// @verified - p
void UnmapGpu(std::uint32_t textureHandle) {
    const TextureLock lock;
    if (TextureResource *const resource = GetTextureResource(textureHandle))
        g_immediateContext->Unmap(static_cast<ID3D11Texture2D *>(resource->stagingTexture), 0);
}

} // namespace Texture2D
#endif

void CreateMipmappedScaledTexture(
    std::uint32_t textureHandle, std::uint32_t firstMip, std::uint32_t mipLevels, const void *pixels, std::uint32_t pixelDataSize) {
    const TextureLock textureLock;
    TextureResource *const resource = GetTextureResource(textureHandle);
    if (!resource || (resource->flags & 8U) == 0) {
        return;
    }

    const std::uint32_t divisor = 1U << (firstMip & 31U);
    std::uint32_t width = static_cast<std::uint32_t>(resource->width) / divisor;
    std::uint32_t height = static_cast<std::uint32_t>(resource->height) / divisor;
    if (!width) {
        width = 1;
    }
    if (!height) {
        height = 1;
    }

#if defined(_WIN32)
    std::vector<D3D11_SUBRESOURCE_DATA> subresources(mipLevels - firstMip);
#endif
    const auto *cursor = static_cast<const std::byte *>(pixels);
    for (std::uint32_t mip = firstMip; mip < mipLevels; ++mip) {
        const std::uint32_t rowPitch = TextureStride(static_cast<std::int32_t>(width), 1, resource->format);
        const std::uint32_t mipSize = TextureStride(static_cast<std::int32_t>(width), static_cast<std::int32_t>(height), resource->format);
#if defined(_WIN32)
        D3D11_SUBRESOURCE_DATA &subresource = subresources[mip - firstMip];
        subresource.pSysMem = cursor;
        subresource.SysMemPitch = rowPitch;
        subresource.SysMemSlicePitch = 0;
#else
        (void)rowPitch;
#endif
        std::uint16_t residentMips = static_cast<std::uint16_t>(resource->reserved);
        if (mip < 16) {
            residentMips = static_cast<std::uint16_t>(residentMips | static_cast<std::uint16_t>(1U << mip));
        }
        resource->reserved = (resource->reserved & ~std::uint64_t{0xffffU}) | residentMips;
        width = width > 1 ? width >> 1 : 1;
        height = height > 1 ? height >> 1 : 1;
        cursor += mipSize;
    }

    if (static_cast<std::uint64_t>(cursor - static_cast<const std::byte *>(pixels)) > pixelDataSize) {
        return;
    }

    void *sourceTexture = nullptr;
#if defined(_WIN32)
    D3D11_TEXTURE2D_DESC description{};
    description.Width = static_cast<UINT>(std::max<std::int32_t>(1, resource->width / static_cast<std::int32_t>(divisor)));
    description.Height = static_cast<UINT>(std::max<std::int32_t>(1, resource->height / static_cast<std::int32_t>(divisor)));
    description.MipLevels = mipLevels - firstMip;
    description.ArraySize = 1;
    description.Format = NativeTextureFormat(resource->format);
    description.SampleDesc.Count = 1;
    description.Usage = D3D11_USAGE_STAGING;
    description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    auto *const device = static_cast<ID3D11Device *>(g_D3DDevice);
    if (!device || FAILED(device->CreateTexture2D(&description, subresources.data(), reinterpret_cast<ID3D11Texture2D **>(&sourceTexture)))) {
        return;
    }
#endif

    const TextureCreationLock queueLock;
    g_queuedTextureCopies.push_back(QueuedTextureCopy{
        sourceTexture,
        textureHandle,
        firstMip,
        mipLevels,
        0,
    });
}



void GenerateMipsAndUpdateResourcesForDirectX() {
    {
        const std::lock_guard<std::mutex> mipsLock(g_generateMipsMutex);
        const TextureLock textureLock;
#if defined(_WIN32)
        for (const std::uint32_t texture : g_generateMipsTextures) {
            TextureResource *const resource = GetTextureResource(texture);
            if (resource)
                g_immediateContext->GenerateMips(static_cast<ID3D11ShaderResourceView *>(resource->shaderResourceView));
        }
#endif
        g_generateMipsTextures.clear();
    }
    const TextureLock textureLock;
    const TextureCreationLock queueLock;
    for (QueuedTextureCopy &copy : g_queuedTextureCopies) {
        TextureResource *const resource = GetTextureResource(copy.destinationHandle);
        if (!resource) {
            continue;
        }
#if defined(_WIN32)
        ID3D11DeviceContext *const context = g_immediateContext;
        auto *const source = static_cast<ID3D11Texture2D *>(copy.sourceTexture);
        auto *const destination = static_cast<ID3D11Texture2D *>(resource->texture);
        for (std::uint32_t mip = copy.firstMip; mip < copy.endMip; ++mip) {
            context->CopySubresourceRegion(destination, mip, 0, 0, 0, source, mip - copy.firstMip, nullptr);
        }
        if (source) {
            source->Release();
            copy.sourceTexture = nullptr;
        }
        const std::uint16_t residentMips = static_cast<std::uint16_t>(resource->reserved);
        const std::uint32_t minimumLod =
            residentMips ? static_cast<std::uint32_t>(std::countr_zero(residentMips)) : static_cast<std::uint32_t>(resource->mipLevels);
        context->SetResourceMinLOD(destination, static_cast<float>(minimumLod));
#endif
    }
    g_queuedTextureCopies.clear();
}

void InitializePvrTextureSlotStorage() {
    for (std::size_t index = 0; index < kSurfaceCount; ++index) {
        g_surfaceMappings[index].referenceCount = 0;
        g_surfaceMappings[index].textureSlot = 0xffffU;
        g_freeTextureSlots[index] = static_cast<std::uint16_t>(index);
    }
    std::memset(g_surfaceRecords.data(), 0, sizeof(g_surfaceRecords));
    g_freeTextureHead = 0;
    g_freeTextureTail = kSurfaceCount - 1;
    g_allocatedTextureSlotCount = 0;
    g_surfaceBase = 0;
}

void InitPVRTaskData(std::int32_t surfaceIndex) {
    const std::int32_t request = g_surfaceBase + surfaceIndex;
    if (request < 0 || request >= static_cast<std::int32_t>(kSurfaceCount)) {
        return;
    }
    SurfaceMapping &mapping = g_surfaceMappings[request];
    if (!mapping.referenceCount) {
        return;
    }
    if (mapping.textureSlot != 0xffffU) {
        SurfaceRecord &record = g_surfaceRecords[mapping.textureSlot];
        if (record.primary.texture) {
            InitTexture2DResource(static_cast<std::uint32_t>(record.primary.texture));
            --g_TextureRefCount;
        }
        std::memset(&record, 0, sizeof(record));
    }
    while (mapping.referenceCount) {
        --mapping.referenceCount;
        if (!mapping.referenceCount) {
            g_freeTextureTail = (g_freeTextureTail + 1) % kSurfaceCount;
            g_freeTextureSlots[g_freeTextureTail] = mapping.textureSlot;
            mapping.textureSlot = 0xffffU;
        }
    }
    --g_allocatedTextureSlotCount;
}

std::uint32_t GetPVRSurfaceTextureHandle(std::int32_t surfaceIndex) {
    if (surfaceIndex < 0 || surfaceIndex >= static_cast<std::int32_t>(kSurfaceCount))
        return 0;
    const std::int32_t request = g_surfaceBase + surfaceIndex;
    if (request < 0 || request >= static_cast<std::int32_t>(kSurfaceCount))
        return 0;
    const SurfaceMapping &mapping = g_surfaceMappings[request];
    if (!mapping.referenceCount || mapping.textureSlot == 0xffffU)
        return 0;
    return static_cast<std::uint32_t>(g_surfaceRecords[mapping.textureSlot].primary.texture);
}

// The surface record the strip and model draws read inline: none when the surface is out of range, unreferenced or has no slot.
const SurfaceRecord *GetPVRSurfaceRecord(std::int32_t surfaceIndex) {
    const std::uint32_t request = static_cast<std::uint32_t>(g_surfaceBase + surfaceIndex);
    if (request >= kSurfaceCount)
        return nullptr;
    const SurfaceMapping &mapping = g_surfaceMappings[request];
    if (!mapping.referenceCount || mapping.textureSlot == 0xffffU)
        return nullptr;
    return &g_surfaceRecords[mapping.textureSlot];
}

std::uint32_t ExchangePVRSurfaceTexture(std::int32_t surfaceIndex, std::uint32_t texture) {
    const std::int32_t request = g_surfaceBase + surfaceIndex;
    if (surfaceIndex < 0 || request < 0 || request >= static_cast<std::int32_t>(kSurfaceCount))
        return 0;
    const SurfaceMapping &mapping = g_surfaceMappings[request];
    if (!mapping.referenceCount || mapping.textureSlot == 0xffffU)
        return 0;
    std::uint64_t &slot = g_surfaceRecords[mapping.textureSlot].primary.texture;
    const auto previous = static_cast<std::uint32_t>(slot);
    slot = texture;
    return previous;
}

std::int32_t d3t_UploadTexture2DToGPU(std::int32_t surfaceIndex, const void *textureDescription, const void *pixels) {
    const auto *const description = static_cast<const TextureUploadDescription *>(textureDescription);
    const std::int32_t request = g_surfaceBase + surfaceIndex;
    if (request < static_cast<std::int32_t>(kSurfaceCount) && g_surfaceMappings[request].referenceCount) {
        ++g_surfaceMappings[request].referenceCount;
        return surfaceIndex;
    }
    if (!description->arenaOffset) {
        return -1;
    }
    if (!CreateDirectXTexture2DWrapper(static_cast<std::uint32_t>(request), static_cast<std::uint16_t>(description->width),
            static_cast<std::uint16_t>(description->height), static_cast<std::uint16_t>(description->format), pixels)) {
        return -1;
    }
    return surfaceIndex;
}

bool CreatePVRTextureBufferForSurface(
    const std::int32_t surfaceIndex, const std::uint32_t width, const std::uint32_t height, const std::uint16_t format, const void *pixels) {
    const std::int32_t request = g_surfaceBase + surfaceIndex;
    if (request < 0 || request >= static_cast<std::int32_t>(kSurfaceCount))
        return false;
    return CreateDirectXTexture2DWrapper(static_cast<std::uint32_t>(request), width, height, format, pixels) != 0;
}

} // namespace shenmue::d3t
