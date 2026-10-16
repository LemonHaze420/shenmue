#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

#pragma pack(push, 1)
struct PvrHeader {
    std::uint32_t magic;
    std::int32_t dataSize;
    std::uint8_t pixelFormat;
    std::uint8_t dataFormat;
    std::uint16_t reserved0a;
    std::uint16_t width;
    std::uint16_t height;
};
#pragma pack(pop)

static_assert(sizeof(PvrHeader) == 0x10);



void DecodePvr(void *destination, const void *pvrHeader);

} // namespace shenmue::d3t
