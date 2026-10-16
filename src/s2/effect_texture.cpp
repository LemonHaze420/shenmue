#include "s2/effect_texture.hpp"

#include "d3t/image_texture_loader.hpp"
#include "d3t/texture.hpp"
#include "s2/pak.hpp"
#include <cstring>

namespace shenmue::s2 {
std::uint32_t g_legacyEffectTextureCount{};
namespace {
std::array<LegacyEffectTexture, 2> effectTextureReferences{};
std::array<LegacyEffectTexture *, 2> activeEffectTextureReferences{};
std::array<std::uint32_t, 4> effectTextureReferenceDimensions{};
}


// @verified - p
LegacyEffectTexture *CreateEffectTextureReference(
    std::uint32_t width, std::uint32_t height, std::uint16_t format, std::span<const std::byte> image, std::int32_t slot) {
    const std::size_t index = slot != 0;
    LegacyEffectTexture *const texture = &effectTextureReferences[index];
    activeEffectTextureReferences[index] = texture;
    if (InitializeLegacyEffectTexture(*texture, width, height, format) != 0 || UploadLegacyEffectPvr(*texture, image) != 0)
        return nullptr;
    effectTextureReferenceDimensions[index * 2] = width;
    effectTextureReferenceDimensions[index * 2 + 1] = height;
    return texture;
}

// @verified - p
LegacyEffectTexture *GetEffectTextureReference(std::int32_t slot) {
    return &effectTextureReferences[slot != 0];
}

// @verified - p
const std::uint32_t *GetEffectTextureReferenceDimensions(std::int32_t slot) {
    return effectTextureReferenceDimensions.data() + (slot != 0 ? 2 : 0);
}

// @verified - p
void InitializeEffectTextureReferences() {
    effectTextureReferences = {};
    activeEffectTextureReferences = {};
    effectTextureReferenceDimensions = {};
}

// @verified - p
void DestroyEffectTextureReferences() {
    for (LegacyEffectTexture *texture : activeEffectTextureReferences) {
        if (texture && texture->handle) {
            d3t::InitTexture2DResource(static_cast<std::uint32_t>(texture->handle));
            texture->handle = 0;
            --g_legacyEffectTextureCount;
        }
    }
    InitializeEffectTextureReferences();
}

std::int32_t InitializeLegacyEffectTexture(LegacyEffectTexture &texture, std::uint32_t width, std::uint32_t height, std::uint16_t format) {
    const std::uint32_t family = format & 0xff00U;
    const std::uint32_t pixel = format & 0xffU;
    std::uint32_t gpuFormat = 4;
    if (family == 0x0100 || family == 0x0300 || family == 0x0900 || family == 0x0d00 || family == 0x8100 || family == 0x8300 || family == 0x8500 ||
        family == 0x8600) {
        if (pixel == 1 || pixel == 2 || pixel == 4 || pixel == 0x89)
            gpuFormat = 2;
    } else if (family == 0x8000 || family == 0x8700) {
        switch (pixel) {
        case 0:
        case 0x86:
            gpuFormat = 33;
            break;
        case 1:
        case 0x87:
            gpuFormat = 31;
            break;
        case 2:
        case 0x85:
            gpuFormat = 35;
            break;
        case 0x80:
            gpuFormat = 23;
            break;
        case 0x81:
            gpuFormat = 24;
            break;
        case 0x82:
            gpuFormat = 25;
            break;
        case 0x83:
        case 0x88:
        case 0x89:
            gpuFormat = 2;
            break;
        case 0x84:
            gpuFormat = 4;
            break;
        default:
            return 4;
        }
    } else
        return 4;
    const d3t::Texture2DDescription description{.width = width,
        .height = height,
        .arraySize = 1,
        .mipLevels = 1,
        .format = gpuFormat,
        .bindFlags = 0,
        .flags = 0,
        .sampleCount = 1,
        .sampleQuality = 0,
        .reserved_24 = 0,
        .initialData = nullptr};
    texture.handle = d3t::CreateTexture2D(&description);
    texture.planes = 1;
    texture.formatFlags = 0;
    for (const std::array<std::uint32_t, 2> &entry : kLegacyEffectFormatFlags) {
        if (entry[0] == family) {
            texture.formatFlags = entry[1];
            break;
        }
    }
    if (((family - 0x0500U) & 0xfffffeffU) == 0)
        texture.pixelFlags = 0x28000000U;
    else if (((family - 0x0700U) & 0xfffffeffU) == 0)
        texture.pixelFlags = 0x30000000U;
    else {
        switch (pixel) {
        case 0:
        case 5:
            texture.pixelFlags = 0;
            break;
        case 1:
            texture.pixelFlags = 0x08000000U;
            break;
        case 2:
            texture.pixelFlags = 0x10000000U;
            break;
        case 3:
        case 6:
            texture.pixelFlags = 0x18000000U;
            break;
        case 4:
            texture.pixelFlags = 0x20000000U;
            break;
        default:
            if (pixel >= 0x80 && pixel <= 0x89)
                texture.pixelFlags = 0x80000000U + (pixel - 0x80U) * 0x08000000U;
            break;
        }
    }
    texture.kind = ((family - 0x1000U) & 0xfffffeffU) == 0 ? 3U : 2U;
    texture.width = width;
    texture.height = height;
    ++g_legacyEffectTextureCount;
    return 0;
}

std::int32_t UploadLegacyEffectPvr(LegacyEffectTexture &texture, std::span<const std::byte> image) {
    if (texture.formatFlags & 0x80U) {
        std::uint32_t magic{};
        std::memcpy(&magic, image.data(), sizeof(magic));
        const std::size_t header = magic == SHEN_NAME(GBIX) ? 28U : 16U;
        const std::byte *payload = image.data() + header;
        std::size_t size = image.size() - header;
        if (*payload != std::byte{'D'}) {
            payload += 4;
            size -= 4;
        }
        return d3t::ReplaceDecodedImage(static_cast<std::uint32_t>(texture.handle), payload, size, true) ? 0 : 4;
    }
    if ((texture.formatFlags & 0x300U) == 0) {
        const std::uint32_t handle = UploadTexn(image, 0);
        d3t::InitTexture2DResource(static_cast<std::uint32_t>(texture.handle));
        texture.handle = handle;
    }
    return 0;
}

} // namespace shenmue::s2
