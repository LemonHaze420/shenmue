#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#if !SHENMUE_IS_DC
#include <array>
#endif
#include <cstdint>

namespace shenmue { namespace s2 {

struct WeatherColour {
#if SHENMUE_IS_DC
    float current[4];
#else
    std::array<float, 4> current;
#endif
#if SHENMUE_IS_DC
    float target[4];
#else
    std::array<float, 4> target;
#endif
#if SHENMUE_IS_DC
    float step[4];
#else
    std::array<float, 4> step;
#endif
    std::int32_t frame;
    std::int32_t frameCount;
};
typedef char LrgbLayout319[(sizeof(WeatherColour) == 0x38) ? 1 : -1];

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define WEATHER_COLOUR_STORAGE SHEN_NAME(LRGB)
WeatherColour *AllocateLRGB();
void FreeLRGB(WeatherColour *value);
std::int32_t AdvanceWeatherColour(WeatherColour *value);
void lrgbSetImmediate(WeatherColour *value, const Vector4 *target);
void BeginWeatherColourTransition(WeatherColour *value, const Vector4 *target, std::int32_t frames);
Vector4 *GetLRGBCurrent(WeatherColour *value, Vector4 *output);
#else
std::int32_t lrgbSetImmediate(WeatherColour *value, const Vector4 *target);
#endif

} } // namespace shenmue::s2
