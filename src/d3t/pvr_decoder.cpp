#include "d3t/pvr_decoder.hpp"
#include "logging.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace shenmue::d3t {
namespace {

#pragma pack(push, 1)
#pragma pack(pop)

struct ChannelDecodeConfig {
    std::int32_t mask;
    std::int32_t scale;
    std::int32_t bias;
    std::int32_t shift;
};

struct PixelDecodeConfig {
    ChannelDecodeConfig channels[4];
};

static_assert(sizeof(PvrHeader) == 0x10);
static_assert(sizeof(ChannelDecodeConfig) == 0x10);
static_assert(sizeof(PixelDecodeConfig) == 0x40);

constexpr PixelDecodeConfig kPixelDecodeConfigs[3]{
    {{{0x7c00, 527, 0x5c00, 16},    {0x03e0, 527, 0x02e0, 11},      {0x001f, 527, 0x0017, 6},   {0x8000, 511, 0, 16}}},
    {{{0xf800, 527, 0xb800, 17},    {0x07e0, 259, 0x0420, 11},      {0x001f, 527, 0x0017, 6},   {0, 0, 255, 0}}},
    {{{0x0f00, 1088, 0x2000, 14},   {0x00f0, 1088, 0x0200, 10},     {0x000f, 1088, 0x0020, 6},  {0xf000, 1088, 0x20000, 18}}},
};

std::uint8_t decodeChannel(std::uint16_t pixel, const ChannelDecodeConfig &config) {
    return static_cast<std::uint8_t>((config.bias + config.scale * (pixel & config.mask)) >> config.shift);
}

std::array<std::uint8_t, 4> decodePixel(std::uint16_t pixel, const PixelDecodeConfig &config) {
    return {decodeChannel(pixel, config.channels[0]), decodeChannel(pixel, config.channels[1]), decodeChannel(pixel, config.channels[2]),
        decodeChannel(pixel, config.channels[3])};
}

std::uint32_t twiddledIndex(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) {
    const std::uint32_t limit = std::min(width, height);
    std::uint32_t index = 0;
    std::uint32_t sourceBit = 1;
    std::uint32_t destinationBit = 1;
    std::uint32_t bitCount = 0;
    while (sourceBit < limit) {
        if ((y & sourceBit) != 0) {
            index |= destinationBit;
        }
        if ((x & sourceBit) != 0) {
            index |= destinationBit << 1;
        }
        sourceBit <<= 1;
        destinationBit <<= 2;
        ++bitCount;
    }

    const std::uint32_t major = width > height ? x : y;
    return index | ((major >> bitCount) << (2 * bitCount));
}

void storeDecodedPixel(
    std::byte *destination, std::uint32_t width, std::uint32_t height, std::uint32_t x, std::uint32_t y, const std::array<std::uint8_t, 4> &pixel) {
    const std::size_t offset = (static_cast<std::size_t>(height - 1 - y) * width + x) * 4;
    for (std::size_t channel = 0; channel < pixel.size(); ++channel) {
        destination[offset + channel] = static_cast<std::byte>(pixel[channel]);
    }
}

void decodeTwiddled16(std::byte *destination, const std::byte *source, std::uint32_t width, std::uint32_t height, std::uint32_t pixelFormat) {
    if (pixelFormat > 2) {
        Shenmue_LogPrintf(stderr, "ERROR in decode_rectangle -- unsupported mode %u\n", pixelFormat);
        return;
    }

    const PixelDecodeConfig &config = kPixelDecodeConfigs[pixelFormat];
    for (std::uint32_t y = 0; y < height; ++y) {
        for (std::uint32_t x = 0; x < width; ++x) {
            const std::size_t sourceOffset = 2ULL * twiddledIndex(x, y, width, height);
            const auto pixel = static_cast<std::uint16_t>(
                std::to_integer<std::uint8_t>(source[sourceOffset]) | (std::to_integer<std::uint8_t>(source[sourceOffset + 1]) << 8));
            storeDecodedPixel(destination, width, height, x, y, decodePixel(pixel, config));
        }
    }
}

void decodeLinear16(std::byte *destination, const std::byte *source, std::uint32_t width, std::uint32_t height, std::uint32_t pixelFormat) {
    if (pixelFormat > 2) {
        Shenmue_LogPrintf(stderr, "ERROR in decode_rectangle -- unsupported mode %u\n", pixelFormat);
        return;
    }

    const PixelDecodeConfig &config = kPixelDecodeConfigs[pixelFormat];
    for (std::uint32_t y = 0; y < height; ++y) {
        for (std::uint32_t x = 0; x < width; ++x) {
            const std::size_t sourceOffset = 2ULL * (static_cast<std::size_t>(y) * width + x);
            const auto pixel = static_cast<std::uint16_t>(
                std::to_integer<std::uint8_t>(source[sourceOffset]) | (std::to_integer<std::uint8_t>(source[sourceOffset + 1]) << 8));
            storeDecodedPixel(destination, width, height, x, y, decodePixel(pixel, config));
        }
    }
}

void decodePalettized8(std::byte *destination, const std::byte *source, std::uint32_t width, std::uint32_t height) {
    const std::uint32_t stride = (width + 3) & ~3U;
    for (std::uint32_t y = 0; y < height; ++y) {
        std::byte *const row = destination + static_cast<std::size_t>(height - 1 - y) * stride;
        for (std::uint32_t x = 0; x < width; ++x) {
            row[x] = source[twiddledIndex(x, y, width, height)];
        }
    }
}

void decodePalettized4(std::byte *destination, const std::byte *source, std::uint32_t width, std::uint32_t height) {
    const std::uint32_t stride = ((width / 2) + 3) & ~3U;
    const std::uint32_t packedHeight = height / 2;
    for (std::uint32_t y = 0; y < height; ++y) {
        std::byte *const row = destination + static_cast<std::size_t>(height - 1 - y) * stride;
        for (std::uint32_t x = 0; x < width; x += 2) {
            const std::uint8_t firstPacked = std::to_integer<std::uint8_t>(source[twiddledIndex(x, y / 2, width, packedHeight)]);
            const std::uint8_t secondPacked = std::to_integer<std::uint8_t>(source[twiddledIndex(x + 1, y / 2, width, packedHeight)]);
            const std::uint32_t sourceShift = (y & 1U) * 4;
            const std::uint8_t first = static_cast<std::uint8_t>((firstPacked >> sourceShift) & 0x0f);
            const std::uint8_t second = static_cast<std::uint8_t>((secondPacked >> sourceShift) & 0x0f);
            row[x / 2] = static_cast<std::byte>(first | (second << 4));
        }
    }
}

void decodePalOrTwiddled(std::byte *destination, const std::byte *source, std::uint32_t width, std::uint32_t height, std::uint32_t pixelFormat) {
    if (pixelFormat == 5) {
        decodePalettized4(destination, source, width, height);
    } else if (pixelFormat == 6) {
        decodePalettized8(destination, source, width, height);
    } else {
        decodeTwiddled16(destination, source, width, height, pixelFormat);
    }
}

template <std::size_t ColourCount> std::array<std::array<std::uint8_t, 4>, ColourCount> decodeCodebook(const std::byte *source, std::uint32_t pixelFormat) {
    std::array<std::array<std::uint8_t, 4>, ColourCount> codebook{};
    const PixelDecodeConfig &config = kPixelDecodeConfigs[pixelFormat];
    for (std::size_t index = 0; index < ColourCount; ++index) {
        const auto pixel =
            static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(source[index * 2]) | (std::to_integer<std::uint8_t>(source[index * 2 + 1]) << 8));
        codebook[index] = decodePixel(pixel, config);
    }
    return codebook;
}

template <typename Codebook>
void storeVqEntry(
    std::byte *destination, std::uint32_t width, std::uint32_t height, std::uint32_t x, std::uint32_t y, const Codebook &codebook, std::uint32_t entry) {
    const std::size_t base = static_cast<std::size_t>(entry) * 4;
    storeDecodedPixel(destination, width, height, x, y, codebook[base]);
    storeDecodedPixel(destination, width, height, x, y + 1, codebook[base + 1]);
    storeDecodedPixel(destination, width, height, x + 1, y, codebook[base + 2]);
    storeDecodedPixel(destination, width, height, x + 1, y + 1, codebook[base + 3]);
}

void decodeVq1024(
    std::byte *destination, const std::byte *indices, const std::byte *codebookSource, std::uint32_t width, std::uint32_t height, std::uint32_t pixelFormat) {
    const auto codebook = decodeCodebook<1024>(codebookSource, pixelFormat);
    const std::uint32_t compressedWidth = width / 2;
    const std::uint32_t compressedHeight = height / 2;
    for (std::uint32_t y = 0; y < height; y += 2) {
        for (std::uint32_t x = 0; x < width; x += 2) {
            const std::uint32_t index = twiddledIndex(x / 2, y / 2, compressedWidth, compressedHeight);
            const std::uint32_t entry = std::to_integer<std::uint8_t>(indices[index]);
            storeVqEntry(destination, width, height, x, y, codebook, entry);
        }
    }
}

void decodeVq64(
    std::byte *destination, const std::byte *indices, const std::byte *codebookSource, std::uint32_t width, std::uint32_t height, std::uint32_t pixelFormat) {
    const auto codebook = decodeCodebook<64>(codebookSource, pixelFormat);
    const std::uint32_t compressedWidth = width / 2;
    const std::uint32_t compressedHeight = height / 4;
    for (std::uint32_t y = 0; y < height; y += 4) {
        for (std::uint32_t x = 0; x < width; x += 2) {
            const std::uint32_t index = twiddledIndex(x / 2, y / 4, compressedWidth, compressedHeight);
            const std::uint8_t packed = std::to_integer<std::uint8_t>(indices[index]);
            storeVqEntry(destination, width, height, x, y, codebook, packed & 0x0fU);
            storeVqEntry(destination, width, height, x, y + 2, codebook, packed >> 4);
        }
    }
}

std::uint32_t squareMipTexelCount(std::uint32_t width) {
    std::uint32_t count = 1;
    while (width != 0) {
        width >>= 1;
        count += width * width;
    }
    return count;
}

std::uint32_t vqMipIndexOffset(std::uint32_t width) {
    std::uint32_t count = 1;
    width >>= 1;
    while (width != 0) {
        width >>= 1;
        count += width * width;
    }
    return count;
}

} // namespace

void DecodePvr(void *destination, const void *pvrHeader) {
    const auto *const header = static_cast<const PvrHeader *>(pvrHeader);
    auto *const output = static_cast<std::byte *>(destination);
    const std::byte *const data = static_cast<const std::byte *>(pvrHeader) + sizeof(PvrHeader);
    const std::uint32_t width = header->width;
    const std::uint32_t height = header->height;
    const std::uint32_t pixelFormat = header->pixelFormat;

    switch (header->dataFormat) {
    case 1:
    case 5:
    case 7:
        decodePalOrTwiddled(output, data, width, height, pixelFormat);
        break;
    case 2:
        decodePalOrTwiddled(output, data + 2 * squareMipTexelCount(width), width, height, pixelFormat);
        break;
    case 3:
        decodeVq1024(output, data + 0x800, data, width, height, pixelFormat);
        break;
    case 4:
        decodeVq1024(output, data + 0x800 + vqMipIndexOffset(width), data, width, height, pixelFormat);
        break;
    case 6:
        decodePalOrTwiddled(output, data + squareMipTexelCount(width) / 2, width, height, pixelFormat);
        break;
    case 8:
        decodePalOrTwiddled(output, data + squareMipTexelCount(width), width, height, pixelFormat);
        break;
    case 9:
        decodeLinear16(output, data, width, height, pixelFormat);
        break;
    case 13:
        decodeTwiddled16(output, data, width, height, pixelFormat);
        break;
    case 16:
        decodeVq64(output, data + 0x80, data, width, height, pixelFormat);
        break;
    case 17:
        decodeVq64(output, data + 0x80 + (vqMipIndexOffset(width) / 2), data, width, height, pixelFormat);
        break;
    default:
        break;
    }
}

} // namespace shenmue::d3t
