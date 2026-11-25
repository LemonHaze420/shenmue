#pragma once

#include "shenmue.h"
#if !SHENMUE_IS_DC
#include <array>
#endif
#include <cstddef>
#include <cstdint>

namespace shenmue {
namespace s2 {

#pragma pack(push, 4)
#if SHENMUE_IS_DC
struct WeatherSamplePrimary {
    float values[9];
    std::uint32_t colours[3];
};
struct WeatherSampleEnvironment {
    float values[3];
    std::uint32_t colours[5];
};
struct WeatherTableSample {
    std::int32_t seconds;
    WeatherSamplePrimary primary;
    WeatherSampleEnvironment environment;
    std::byte reserved[12];
};
#else
struct WeatherTableSample {
    std::int32_t seconds;
    std::array<float, 9> primaryValues;
    std::array<std::uint32_t, 3> primaryColours;
    std::array<float, 3> secondaryValues;
    std::array<std::uint32_t, 5> environmentColours;
    std::array<std::byte, 12> reserved;
};
#endif
#pragma pack(pop)

typedef char WeatherTableLayout794[(sizeof(WeatherTableSample) == 0x60) ? 1 : -1];
#if SHENMUE_IS_DC
typedef char WeatherTableLayoutPrimarySize[(sizeof(WeatherSamplePrimary) == 0x30) ? 1 : -1];
typedef char WeatherTableLayoutEnvironmentSize[(sizeof(WeatherSampleEnvironment) == 0x20) ? 1 : -1];
typedef char WeatherTableLayoutPrimary[(offsetof(WeatherTableSample, primary) == 0x04) ? 1 : -1];
typedef char WeatherTableLayoutEnvironment[(offsetof(WeatherTableSample, environment) == 0x34) ? 1 : -1];
typedef char WeatherTableLayout1125[(offsetof(WeatherTableSample, reserved) == 0x54) ? 1 : -1];
#else
typedef char WeatherTableLayout845[(offsetof(WeatherTableSample, primaryValues) == 0x04) ? 1 : -1];
typedef char WeatherTableLayout913[(offsetof(WeatherTableSample, primaryColours) == 0x28) ? 1 : -1];
typedef char WeatherTableLayout982[(offsetof(WeatherTableSample, secondaryValues) == 0x34) ? 1 : -1];
typedef char WeatherTableLayout1052[(offsetof(WeatherTableSample, environmentColours) == 0x40) ? 1 : -1];
typedef char WeatherTableLayout1125[(offsetof(WeatherTableSample, reserved) == 0x54) ? 1 : -1];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct WeatherTable;
float InterpolateWeatherValue(float first, float second, float blend);
std::uint32_t InterpolateWeatherColour(std::int32_t first, std::int32_t second, float blend);
std::uint32_t AddWeatherColours(std::uint32_t first, std::uint32_t second);
void ClearWeatherTableSample(WeatherTableSample *sample);
void AccumulateWeatherSample(WeatherTableSample *output, const WeatherTableSample *sample, float weight);
void InterpolateWeatherSample(const WeatherTableSample *first, const WeatherTableSample *second, float blend, WeatherTableSample *output);
void SampleWeatherTable(const WeatherTable *table, std::int32_t seconds, WeatherTableSample *output);
void EvaluateWeatherTable(
    const WeatherTable *tables, std::int32_t count, std::int32_t seconds, float cloud, float rain, float snow, WeatherTableSample *output);
#endif

} // namespace s2
} // namespace shenmue
