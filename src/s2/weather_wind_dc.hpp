#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"
#include "common/types.hpp"

namespace shenmue {
namespace s2 {

#define WEATHER_WIND_STORAGE SHEN_NAME4('W', 'I', 'N', 'D')
#define WEATHER_WIND_PROFILE_STORAGE SHEN_NAME4('W', 'B', 'L', 'T')
#define WEATHER_WIND_MAPPING_STORAGE SHEN_NAME4('S', '2', 'A', 'T')
#define WEATHER_WIND_IMPULSE_STORAGE SHEN_NAME(1/FW)
#define WEATHER_NOISE_COEFFICIENT_STORAGE SHEN_NAME4('T', 'M', 'P', '0')
#define WEATHER_NOISE_HISTORY_STORAGE SHEN_NAME4('T', 'M', 'P', '1')
// clang-format off
static const float windDirectionWeights[144]= {
    0.3f, 6.0f, 6.0f, 4.0f, 4.0f, 2.0f, 2.0f, 2.0f, 2.0f,
    0.3f, 5.0f, 5.0f, 2.0f, 1.4f, 0.9f, 0.8f, 0.7f, 0.2f,
    0.3f, 1.0f, 1.0f, 1.0f, 0.5f, 0.2f, 0.0f, 0.0f, 0.0f,
    0.3f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.2f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.2f, 0.5f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.2f, 1.0f, 0.5f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.2f, 0.6f, 0.6f, 0.8f, 0.7f, 0.6f, 0.5f, 0.5f, 0.0f,
    0.2f, 0.8f, 1.2f, 2.5f, 2.1f, 1.7f, 1.2f, 0.6f, 0.0f,
    0.2f, 0.8f, 1.0f, 1.6f, 1.0f, 0.5f, 0.4f, 0.0f, 0.0f,
    0.2f, 1.0f, 0.5f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.2f, 0.7f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.2f, 0.7f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.2f, 0.8f, 0.7f, 0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.3f, 1.0f, 1.0f, 1.0f, 0.6f, 0.4f, 0.4f, 0.3f, 0.0f,
    0.4f, 3.5f, 2.8f, 2.5f, 2.2f, 1.2f, 1.2f, 1.2f, 1.2f,
};
// clang-format on
struct WeatherWind {
    std::int32_t outputPhase, targetPhase, requestedPhase, phaseStep, phaseFramesRemaining;
    float sampledWindSpeed, currentWindSpeed, requestedWindSpeed, windSpeedStep;
    std::int32_t windSpeedFramesRemaining;
    Vector3 windDirection;
    float windScaleDenominator;
    std::int32_t cooldownFramesRemaining;
};
typedef char Layout_weather_wind_dc_0[(sizeof(WeatherWind) == 0x3c) ? 1 : -1];
typedef char Layout_weather_wind_dc_1[(offsetof(WeatherWind, sampledWindSpeed) == 0x14) ? 1 : -1];
typedef char Layout_weather_wind_dc_2[(offsetof(WeatherWind, windDirection) == 0x28) ? 1 : -1];
typedef char Layout_weather_wind_dc_3[(offsetof(WeatherWind, cooldownFramesRemaining) == 0x38) ? 1 : -1];
extern WeatherWind *&weatherWind;
extern float *windProfileSamples, *windMappingTable, *windImpulseLut;
extern float totalWindProfileWeight;
extern std::int32_t windImpulseLutIndex;
float EvaluateWeibullDensity(float shape, float scale, float value);
void GenerateWeibullProfile(float *samples, std::uint32_t count, float shape, float scale);
void AllocateTMP0_TMP1(float *output, std::uint32_t count, std::uint32_t taps);
void BuildWindMappingTable(float *table);
float SampleWindImpulse();
float SampleWindFromProfile(float baseSpeed, const float *profile);
std::int32_t MapWindToImpulseIndex(float sampledSpeed, const float *table);
void InitWeibullResources();
void UpdateWindWithSpeed(WeatherWind *wind);
WeatherWind *GetWind();
WeatherWind *AllocateWIND();
void FreeWIND(WeatherWind *wind);
void ResetWind(WeatherWind *wind);
void SetWindPhaseTransition(WeatherWind *wind, std::int32_t phase, std::int32_t frames);
void SetWindSpeedTransition(WeatherWind *wind, float speed, std::int32_t frames);
void BeginWindPhaseTransition(WeatherWind *wind, std::int32_t phase);
void BeginWindSpeedTransition(WeatherWind *wind, float speed);
float GetEffectiveWindSpeed(const WeatherWind *wind);
std::int32_t GetCurrentWindPhaseAccumulator(const WeatherWind *wind);
void GetWindDirection(const WeatherWind *wind, Vector3 *direction);
std::int32_t GetWindTargetPhase(const WeatherWind *wind);
float GetCurrentWindSpeed(const WeatherWind *wind);
struct WindowSlot;
namespace HLib {
struct Task;
}
void SetWindowOrigin(WindowSlot *window, std::int16_t x, std::int16_t y);
std::uint32_t wndGetFlag(HLib::Task *task);
void SetWindScaleDenominator(WeatherWind *wind, float denominator);

} // namespace s2
} // namespace shenmue

#endif
