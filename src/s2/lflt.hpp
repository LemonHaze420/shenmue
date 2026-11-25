#pragma once

#include "shenmue.h"
#include "common/types.hpp"
#include <cstdint>

namespace shenmue { namespace s2 {

struct WeatherFloat {
    float current;
    float target;
    float step;
    std::int32_t frame;
    std::int32_t frameCount;
};
typedef char WeatherFloatLayout[(sizeof(WeatherFloat) == 0x14) ? 1 : -1];

void BeginWeatherFloatTransition(WeatherFloat *value, float target, std::int32_t frames);
std::int32_t AdvanceWeatherFloat(WeatherFloat *value);
std::int32_t lfltGetFramesRemaining(const WeatherFloat *value);
bool lfltIsInterpolating(const WeatherFloat *value);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define WEATHER_FLOAT_STORAGE SHEN_NAME(LFLT)
WeatherFloat *AllocateLFLT();
void FreeLFLT(WeatherFloat *value);
void lfltSetImmediate(WeatherFloat *value, float target);
float GetLFLTCurrent(WeatherFloat *value);
float GetLFLTTarget(WeatherFloat *value);
#else
std::int32_t lfltSetImmediate(WeatherFloat *value, float target);
#endif

} } // namespace shenmue::s2
