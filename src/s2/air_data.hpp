#pragma once

#include "common/math.hpp"
#include <cstddef>

namespace shenmue { namespace s2 {

#if SHENMUE_IS_DC
struct AirPlanetDrawRecord {
    Vector3 position;
    Vector3 orbitInput;
    Vector3i rotation;
    Vector3i intermediateRotation;
    std::int32_t tilt;
    Vector3i timeRotation;
    Vector3 scale;
    float intensity;
};
#else
struct AirPlanetDrawRecord {
    Vector3 position{};
    Vector3 orbitInput{-3200.0f, 0.0f, 0.0f};
    Vector3i rotation{};
    Vector3i intermediateRotation{};
    std::int32_t tilt{6144};
    Vector3i timeRotation{0, 0, 32768};
    Vector3 scale{20.0f, 20.0f, 20.0f};
    float intensity{0.40000001f};
};
#endif
typedef char AirPlanetLayout661[(sizeof(AirPlanetDrawRecord) == 0x50) ? 1 : -1];
typedef char AirPlanetLayout713[(offsetof(AirPlanetDrawRecord, orbitInput) == 0x0c) ? 1 : -1];
typedef char AirPlanetLayout779[(offsetof(AirPlanetDrawRecord, rotation) == 0x18) ? 1 : -1];
typedef char AirPlanetLayout843[(offsetof(AirPlanetDrawRecord, intermediateRotation) == 0x24) ? 1 : -1];
typedef char AirPlanetLayout919[(offsetof(AirPlanetDrawRecord, tilt) == 0x30) ? 1 : -1];
typedef char AirPlanetLayout979[(offsetof(AirPlanetDrawRecord, timeRotation) == 0x34) ? 1 : -1];
typedef char AirPlanetLayout1047[(offsetof(AirPlanetDrawRecord, scale) == 0x40) ? 1 : -1];
typedef char AirPlanetLayout1108[(offsetof(AirPlanetDrawRecord, intensity) == 0x4c) ? 1 : -1];

} } // namespace shenmue::s2
