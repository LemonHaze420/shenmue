#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/lflt.hpp"
#include "s2/lrgb.hpp"
#include "s2/weather_data_dc.hpp"
#include "s2/weather_table.hpp"

namespace shenmue {
namespace s2 {

struct WeatherAir {
    std::uint32_t lightIndex;
    WeatherTable *tables;
    std::int32_t tableCount;
    std::uint32_t instruction;
    std::int32_t month, day, hour, minute, second, seconds;
    float cloud, rain, snow, auxiliary;
    std::int32_t cloudState, rainState, mode;
    WeatherColour *primaryColours[3];
    WeatherFloat *values[2];
    WeatherColour *secondaryColours[3];
};
typedef char Layout_weather_air_dc_0[(sizeof(WeatherAir) == 0x64) ? 1 : -1];
typedef char Layout_weather_air_dc_1[(offsetof(WeatherAir, seconds) == 0x24) ? 1 : -1];
typedef char Layout_weather_air_dc_2[(offsetof(WeatherAir, cloudState) == 0x38) ? 1 : -1];
typedef char Layout_weather_air_dc_3[(offsetof(WeatherAir, primaryColours) == 0x44) ? 1 : -1];
typedef char Layout_weather_air_dc_4[(offsetof(WeatherAir, values) == 0x50) ? 1 : -1];
typedef char Layout_weather_air_dc_5[(offsetof(WeatherAir, secondaryColours) == 0x58) ? 1 : -1];

#define WEATHER_AIR_STORAGE SHEN_NAME4('W', 'A', 'I', 'R')
static const std::int32_t weatherAirTextureIds[7] = {0, 5, 4, 3, 1, 2, 2};
static const float weatherAirTextureScales[7] = {1.0f, 1.0f, 1.0f, 1.0f, 0.4f, 1.0f, 1.0f};
static const std::int32_t weatherAccumulatedMonthDays[12] = {31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
extern WeatherTableSample g_interpolatedWeatherSample;

WeatherAir *AllocateWeatherAir(std::uint32_t lightIndex, WeatherTable *tables, std::int32_t tableCount);
void FreeWeatherAir(WeatherAir *air);
void SetWeatherAirTable(WeatherAir *air, std::int32_t index, const std::byte *rows, std::int32_t count);
const std::byte *GetWeatherAirTableData(const WeatherAir *air, std::int32_t index);
std::int32_t GetWeatherAirTableCount(const WeatherAir *air, std::int32_t index);
void EnableWeatherAirInstructions(WeatherAir *air, std::uint32_t flags);
void DisableWeatherAirInstructions(WeatherAir *air, std::uint32_t flags);
std::uint32_t GetWeatherAirInstructions(const WeatherAir *air);
void SetWeatherAirTime(WeatherAir *air, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute, std::int32_t second);
void SetWeatherAirCloud(WeatherAir *air, float cloud);
void SetWeatherAirRain(WeatherAir *air, float rain);
void SetWeatherAirSnow(WeatherAir *air, float snow);
void SetWeatherAirAuxiliary(WeatherAir *air, float auxiliary);
Vector4 *UnpackWeatherColour(std::uint32_t packed, Vector4 *colour);
std::uint32_t PackWeatherColour(const Vector4 *colour);
std::uint32_t wairPackRainColorRGBA(WeatherAir *air);
std::uint32_t wairPackSecondaryColorRGBA(WeatherAir *air);
std::uint32_t wairPackRippleColorRGBA(WeatherAir *air);
std::int32_t GetAirTextureForTime(std::int32_t hour, std::int32_t minute);
void UpdateClouds(WeatherAir *air);
void UpdateWeatherClouds(WeatherAir *air, std::int32_t frames);
void ClearWeatherSamplePrimary(WeatherSamplePrimary *block);
void AccumulateWeatherSamplePrimary(WeatherSamplePrimary *output, const WeatherSamplePrimary *sample, float weight);
void InterpolateWeatherSamplePrimary(const WeatherSamplePrimary *first, const WeatherSamplePrimary *second, float blend, WeatherSamplePrimary *output);
void ClearWeatherSampleEnvironment(WeatherSampleEnvironment *block);
void AccumulateWeatherSampleEnvironment(WeatherSampleEnvironment *output, const WeatherSampleEnvironment *sample, float weight);
void InterpolateWeatherSampleEnvironment(
    const WeatherSampleEnvironment *first, const WeatherSampleEnvironment *second, float blend, WeatherSampleEnvironment *output);
Vector3 *InterpolateWeatherVector3(const Vector3 *first, const Vector3 *second, Vector3 *output, float blend);
Vector4 *InterpolateWeatherVector4(const Vector4 *first, const Vector4 *second, Vector4 *output, float blend);
void UpdateFog(WeatherAir &air);
void BuildWeatherSunDirection(
    std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute, float latitude, std::int32_t second, Vector3 *direction);

} // namespace s2
} // namespace shenmue

#endif
