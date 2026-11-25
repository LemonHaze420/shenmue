#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && (SHENMUE_IS_S1 || SHENMUE_IS_S2)

#include "platform/dc/ninja.hpp"
#include <cstdint>

namespace shenmue { namespace dc {

struct WeatherTextureSurfaces {
    NJS_TEXSURFACE surfaces[2];
    NJS_TEXSURFACE *active[2];
    std::uint32_t dimensions[2][2];
};
typedef char WeatherTextureLayout314[(sizeof(WeatherTextureSurfaces) == 0x68) ? 1 : -1];
typedef char WeatherTextureLayout369[(offsetof(WeatherTextureSurfaces, active) == 0x50) ? 1 : -1];
typedef char WeatherTextureLayout434[(offsetof(WeatherTextureSurfaces, dimensions) == 0x58) ? 1 : -1];

} } // namespace shenmue::dc

#if SHENMUE_IS_S2
namespace shenmue { namespace s2 {
#else
namespace shenmue { namespace s1 {
#endif

typedef dc::WeatherTextureSurfaces WeatherTextureSurfaces;
extern WeatherTextureSurfaces g_weatherTextureSurfaces;
NJS_TEXSURFACE *CreateWeatherTextureSurface(std::int32_t width, std::int32_t height, std::uint32_t format, const void *pixels, std::int32_t index);
void InitializeWeatherTextureSurfaces();
void ReleaseWeatherTextureSurfaces();

} } // namespace shenmue

#endif
