#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

#if defined(SHENMUE_GFX_DXVK_NATIVE)
struct DdsPixelFormat {
    std::uint32_t size, flags, fourCC, bitsPerPixel;
    std::uint32_t redMask, greenMask, blueMask, alphaMask;
};
struct DdsHeader {
    std::uint32_t size, flags, height, width, pitchOrLinearSize, depth, mipLevels;
    std::uint32_t reserved[11];
    DdsPixelFormat pixelFormat;
    std::uint32_t caps, caps2, caps3, caps4, reserved2;
};
struct DdsHeaderDx10 {
    std::uint32_t format, dimension, flags, arraySize, alphaMode;
};
struct DdsSubresource {
    std::size_t offset;
    std::uint32_t size, rowPitch;
};
static_assert(sizeof(DdsPixelFormat) == 32);
static_assert(sizeof(DdsHeader) == 124);
static_assert(sizeof(DdsHeaderDx10) == 20);
inline constexpr std::uint32_t DDS_PIXEL_FOURCC = 4, DDS_PIXEL_RGB = 0x40, DDS_PIXEL_LUMINANCE = 0x20000;
inline constexpr std::uint32_t DDS_CUBE = 0x200, DDS_CUBE_FACES = 0xfc00, DDS_VOLUME = 0x200000;
inline constexpr std::uint32_t DDS_DX10_TEXTURE_2D = 3, DDS_DX10_TEXTURE_CUBE = 4;
#endif

std::int64_t LoadImageTexture(const char *path, bool linearColour, std::uint32_t *textureHandle, std::uint32_t renderHandle);
std::uint32_t CreateImageTextureFromMemory(const void *data, std::size_t size, bool linearColour);

bool ReplaceDecodedImage(std::uint32_t textureHandle, const void *loadedData, std::uint64_t loadedSize, bool linearColour);

} // namespace shenmue::d3t
