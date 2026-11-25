#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "s2/lflt.hpp"
#include "s2/weather_wind_dc.hpp"

namespace shenmue {
namespace s2 {

struct WeatherData;
struct WeatherAir;
struct WeatherFixcOwner;
struct WeatherFixcNode;
struct WeatherExecutionState {
    std::int32_t updateEnabled;
    std::int32_t updateTime;
    HLib::Task *task;
    WeatherData *data;
    std::int32_t year, month, day, hour, minute;
    WeatherAir *air;
    std::int32_t lightManagerEnabled;
    std::int32_t lightingEnabled;
    std::int32_t skyTransitionFrames;
    std::int32_t planetUpdateEnabled;
    WeatherFloat *cloud;
    WeatherFloat *auxiliary;
    WeatherWind *wind;
    WeatherFixcNode *secondaryWindList;
    WeatherFixcNode *weatherEffects;
    WeatherFloat *pressure;
    std::uint32_t rainColour;
    float rainPressureParameter0, rainPressureParameter1;
    Vector3 spawnDrift, driftVariance;
    std::byte retainedWeatherParameters[0x24];
    WeatherFloat *rippleCount;
    std::uint32_t rippleColour;
    float rippleDeltaTime;
    WeatherFixcOwner *activeFixc;
    std::int32_t fixcRegistered;
    std::uint32_t instructionFlags;
    Vector2 planetLayerOffset;
    std::int32_t tableMode;
};
typedef char Layout_weather_dc_0[(sizeof(WeatherExecutionState) == 0xbc) ? 1 : -1];
typedef char Layout_weather_dc_1[(offsetof(WeatherExecutionState, cloud) == 0x38) ? 1 : -1];
typedef char Layout_weather_dc_2[(offsetof(WeatherExecutionState, auxiliary) == 0x3c) ? 1 : -1];
typedef char Layout_weather_dc_3[(offsetof(WeatherExecutionState, wind) == 0x40) ? 1 : -1];
typedef char Layout_weather_dc_4[(offsetof(WeatherExecutionState, pressure) == 0x4c) ? 1 : -1];
typedef char Layout_weather_dc_5[(offsetof(WeatherExecutionState, rippleCount) == 0x98) ? 1 : -1];
typedef char Layout_weather_dc_6[(offsetof(WeatherExecutionState, activeFixc) == 0xa4) ? 1 : -1];
typedef char Layout_weather_dc_7[(offsetof(WeatherExecutionState, instructionFlags) == 0xac) ? 1 : -1];
typedef char Layout_weather_dc_8[(offsetof(WeatherExecutionState, tableMode) == 0xb8) ? 1 : -1];
extern float weatherRippleLifetimeStep, weatherRippleLifetimeScale;
extern Vector3 g_weatherClipCameraPosition;
#define WEATHER_EXECUTION_TASK SHEN_NAME4('W', 'E', 'X', 'E')
struct MapInfoChunk;
void QueueWeatherExecution(const MapInfoChunk *weatherChunk);
void ExecuteWeather_Callback(void *parameter);
extern WeatherExecutionState g_weatherExecution;

void SetWeatherSkyTransitionFrames(std::int32_t frames);
std::uint32_t GetCurrentWeatherInstruction();
void SetWeatherCloudImmediate(float intensity);
void BeginWeatherCloudTransition(float intensity, std::int32_t frames);
void BeginWeatherAuxiliaryTransition(float intensity, std::int32_t frames);
void SetWeatherAuxiliaryImmediate(float intensity);
float GetWeatherPressure();
float GetMaximumWeatherRainForecast(std::int32_t minutes);
float GetWeatherWindVector();
void SetWeatherUpdateEnabledTo(std::int32_t enabled);
std::int32_t GetWeatherUpdateEnabled();
void EnsureWeatherWind();
void SetWeatherPlanetUpdateEnabled(std::int32_t enabled);
std::int32_t GetWeatherPlanetUpdateEnabled();
void SetWeatherRainPressureImmediate(float pressure);
void UpdateWeatherForCurrentTimeImmediate();
void UpdateWeather();
void ProcessNewWeatherData();

} // namespace s2
} // namespace shenmue

#endif
