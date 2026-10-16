#pragma once

#include "s1/task/effe.hpp"

#include <array>
#include <cstring>

namespace shenmue::s1 {

struct EffeTextureResource {
    std::byte reserved_00[0x20];
    std::uint32_t textureHandle;
    std::byte reserved_24[0x0c];
    std::uint64_t residentHandle;
};

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

inline constexpr std::uint32_t kActiveTextureFlag = 0x80000000U;

#pragma pack(push, 1)

struct EffePvrHeader {
    std::uint32_t magic;
    std::int32_t dataSize;
    std::uint8_t pixelFormat;
    std::uint8_t dataFormat;
    std::uint16_t reserved_0a;
    std::uint16_t width;
    std::uint16_t height;
};

#pragma pack(pop)

static_assert(sizeof(EffeTextureResource) == 0x38);

static_assert(offsetof(EffeTextureResource, textureHandle) == 0x20);

inline constexpr float kEffeFftEpsilon = 1.1920929e-07f;

// RunFFTStagesAndReorder's per-bin phase controls: 0 holds the bin at zero, 1 at pi, 2 advances it by its rate.
inline constexpr std::array<std::int32_t, 64> kEffeFftPhaseControl{0, 1, 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

} // namespace shenmue::s1::effe_detail
