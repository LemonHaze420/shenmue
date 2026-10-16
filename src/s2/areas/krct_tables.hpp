#pragma once
#include <array>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr std::array<std::uint16_t, 16> krctFirePalette{785, 785, 1041, 1297, 1554, 1810, 2610, 2866, 2882, 3152, 3440, 3696, 3712, 4000, 4035, 4054};
inline constexpr std::array<std::int32_t, 60> krctRestHandPose{0, 725, 63534, 0, 64628, 63534, 0, 62622, 3821, 0, 182, 61896, 0, 0, 58800, 0, 0, 62805, 0, 546,
    1820, 0, 0, 60986, 0, 0, 61531, 0, 0, 61714, 0, 546, 1456, 0, 0, 58983, 0, 0, 62440, 0, 0, 63714, 0, 546, 908, 0, 0, 58074, 0, 0, 64262, 0, 0, 62440, 0,
    546, 1091, 0, 0, 0};
} // namespace shenmue::s2::areas
