#pragma once

#include "shenmue.h"
#include "common/math.hpp"
#if SHENMUE_S2_GCC295
#define SHENMUE_WEATHER_ARRAY(type, name, count) type name[count]
#else
#include <array>
#include <cstddef>
#include <cstdint>
#define SHENMUE_WEATHER_ARRAY(type, name, count) std::array<type, count> name
#endif

namespace shenmue { namespace s2 {

struct WeatherClipEntry {
    std::uint32_t enabled;
    SHENMUE_WEATHER_ARRAY(Vector2, points, 4);
    float clipY0;
    float clipY1;
    Vector3 center;
    float scale;
};
struct WeatherClipSet {
    std::int32_t enabled;
    std::int32_t count;
    WeatherClipEntry *entries;
};
#if SHENMUE_S2_GCC295
typedef char sizeofWeatherClipEntryCheck[sizeof(WeatherClipEntry) == 60 ? 1 : -1];
typedef char offsetofWeatherClipEntrypointsCheck[offsetof(WeatherClipEntry, points) == 4 ? 1 : -1];
typedef char offsetofWeatherClipEntrycenterCheck[offsetof(WeatherClipEntry, center) == 44 ? 1 : -1];
typedef char offsetofWeatherClipEntryscaleCheck[offsetof(WeatherClipEntry, scale) == 56 ? 1 : -1];
#else
static_assert(sizeof(WeatherClipEntry) == 60);
static_assert(offsetof(WeatherClipEntry, points) == 4);
static_assert(offsetof(WeatherClipEntry, center) == 44);
static_assert(offsetof(WeatherClipEntry, scale) == 56);
#endif
#if SHENMUE_IS_DC
#if SHENMUE_S2_GCC295
typedef char sizeofWeatherClipSetCheck[sizeof(WeatherClipSet) == 12 ? 1 : -1];
#else
static_assert(sizeof(WeatherClipSet) == 12);
#endif
#else
#if SHENMUE_S2_GCC295
typedef char sizeofWeatherClipSetCheck[sizeof(WeatherClipSet) == 16 ? 1 : -1];
#else
static_assert(sizeof(WeatherClipSet) == 16);
#endif
#endif

struct WeatherClipQuadPlanes {
    float clipY1;
    float clipY0;
    SHENMUE_WEATHER_ARRAY(float, x, 4);
    SHENMUE_WEATHER_ARRAY(float, z, 4);
    SHENMUE_WEATHER_ARRAY(float, edgeZ, 4);
    SHENMUE_WEATHER_ARRAY(float, edgeX, 4);
};

#if SHENMUE_S2_GCC295
typedef char sizeofWeatherClipQuadPlanesCheck[sizeof(WeatherClipQuadPlanes) == 0x48 ? 1 : -1];
typedef char offsetofWeatherClipQuadPlanesxCheck[offsetof(WeatherClipQuadPlanes, x) == 0x08 ? 1 : -1];
typedef char offsetofWeatherClipQuadPlanesedgeZCheck[offsetof(WeatherClipQuadPlanes, edgeZ) == 0x28 ? 1 : -1];
#else
static_assert(sizeof(WeatherClipQuadPlanes) == 0x48);
static_assert(offsetof(WeatherClipQuadPlanes, x) == 0x08);
static_assert(offsetof(WeatherClipQuadPlanes, edgeZ) == 0x28);
#endif

#if !SHENMUE_IS_DC
namespace {
#endif

struct WeatherInteractionCamera {
    Vector3 position;
    Vector3 currentCameraPosition;
    Vector3 previousBackwardDirection;
    Vector3 nextCameraPosition;
    Vector3 currentCameraPosition2;
    Vector3 backwardDirection;
};

#if SHENMUE_S2_GCC295
typedef char sizeofWeatherInteractionCameraCheck[sizeof(WeatherInteractionCamera) == 0x48 ? 1 : -1];
typedef char offsetofWeatherInteractionCameranextCameraPositionCheck[offsetof(WeatherInteractionCamera, nextCameraPosition) == 0x24 ? 1 : -1];
typedef char offsetofWeatherInteractionCameracurrentCameraPosition2Check[offsetof(WeatherInteractionCamera, currentCameraPosition2) == 0x30 ? 1 : -1];
typedef char offsetofWeatherInteractionCamerabackwardDirectionCheck[offsetof(WeatherInteractionCamera, backwardDirection) == 0x3c ? 1 : -1];
#else
static_assert(sizeof(WeatherInteractionCamera) == 0x48);
static_assert(offsetof(WeatherInteractionCamera, nextCameraPosition) == 0x24);
static_assert(offsetof(WeatherInteractionCamera, currentCameraPosition2) == 0x30);
static_assert(offsetof(WeatherInteractionCamera, backwardDirection) == 0x3c);
#endif

#if !SHENMUE_IS_DC
} // namespace
#endif

} } // namespace shenmue::s2

#undef SHENMUE_WEATHER_ARRAY
