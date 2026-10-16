#pragma once

#include "s1/task/effe.hpp"

#include <array>
#include <cstring>

namespace shenmue::s1::effe_detail {

struct VFFightGroundEffectRecord {
    std::int16_t taskSlot = -1;
    std::uint16_t framesRemaining = 0;
    float probability = 0.0f;
};

static_assert(sizeof(VFFightGroundEffectRecord) == 8);

struct VFFightGroundEffectConfig {
    float velocityRange;
    float unused;
    float probability;
    std::uint8_t particleCount;
    std::uint8_t variant;
};

inline constexpr std::array<VFFightGroundEffectConfig, 6> kVFFightGroundEffectConfigs{{
    {0.01f, 0.02f, 0.2f, 10, 75},
    {0.01f, 0.02f, 0.2f, 10, 75},
    {0.01f, 0.02f, 0.2f, 10, 75},
    {0.005f, 0.18f, 0.3f, 25, 40},
    {0.02f, 0.08f, 0.2f, 20, 40},
    {0.014f, 0.014f, 0.2f, 8, 80},
}};

inline constexpr std::array<Vector3, 10> kVFFightGroundEffectColours{{
    {0.0f, 0.0f, 0.0f},
    {3.0f, 3.0f, 3.0f},
    {5.0f, 5.0f, 5.0f},
    {8.0f, 8.0f, 7.0f},
    {6.0f, 6.0f, 6.0f},
    {5.0f, 4.0f, 3.0f},
    {5.0f, 4.0f, 2.0f},
    {10.0f, 10.0f, 9.0f},
    {8.0f, 8.0f, 7.0f},
    {6.0f, 6.0f, 5.0f},
}};

inline constexpr std::array<std::uint8_t, 12> kVFFightGroundEffectColourByTimeAndArea{
    9,
    8,
    9,
    7,
    7,
    7,
    7,
    7,
    7,
    8,
    7,
    8,
};

} // namespace shenmue::s1::effe_detail
