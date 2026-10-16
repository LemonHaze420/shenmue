#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

struct CompressedContentHeader {
    std::uint8_t compressed;
    std::uint8_t mode;
    std::uint16_t checksum;
    std::uint32_t reserved;
    std::uint64_t decompressedSize;
};

static_assert(sizeof(CompressedContentHeader) == 0x10);

struct D3TCompressionRequest {
    const void *source;
    void *destination;
    std::uint64_t compressedSize;
    std::uint64_t field18;
    std::uint64_t field20;
    std::uint64_t size;
};

static_assert(sizeof(D3TCompressionRequest) == 0x30);

using D3TCompressionSizeFunction = std::uint64_t (*)(std::uint64_t size);
using D3TCompressionFunction = bool (*)(D3TCompressionRequest *request);

struct D3TCompressionMode {
    const char *name;
    D3TCompressionSizeFunction compressedBufferSize;
    D3TCompressionFunction compress;
    D3TCompressionSizeFunction decodeBufferSize;
    D3TCompressionFunction decompress;
    bool platformOnly;
    std::array<std::uint8_t, 7> padding;
};

static_assert(sizeof(D3TCompressionMode) == 0x30);

extern std::array<D3TCompressionMode, 10> g_d3tCompressionModes;
extern std::uint32_t g_oodleMode;
extern std::uint32_t g_shouldShutdownOodle;

void d3t_InitializeOodleZLib();
void *d3t_DecompressContent(const void *data, std::size_t *size, std::uint32_t *result);

} // namespace shenmue::d3t
