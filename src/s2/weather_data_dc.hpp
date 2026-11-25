#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#define SHENMUE_WEATHER_DATA static const
#define SHENMUE_WEATHER_ARRAY(type, name, count) type name[count] =
#define SHENMUE_WEATHER_BEGIN(type, name, count) static const type name[count] = {
#define SHENMUE_WEATHER_END }
#define SHENMUE_WEATHER_DATA_PTR(name) name
#define SHENMUE_WEATHER_CHECK(name, expression) typedef char name[(expression) ? 1 : -1]

namespace shenmue {
namespace s2 {

struct WeatherOverride {
    std::int32_t frames;
    float current, step, target;
};
SHENMUE_WEATHER_CHECK(sizeofWeatherOverrideCheck, sizeof(WeatherOverride) == 0x10);
struct WeatherOverrideState {
    std::uint32_t pendingFlags, releaseFlags;
    WeatherOverride overrides[3];
};
SHENMUE_WEATHER_CHECK(sizeofWeatherOverrideStateCheck, sizeof(WeatherOverrideState) == 0x38);
struct WeatherChannelOverrideState {
    std::uint32_t flags;
    std::int32_t enabled;
    float values[3];
};
SHENMUE_WEATHER_CHECK(sizeofWeatherChannelOverrideStateCheck, sizeof(WeatherChannelOverrideState) == 0x14);
struct WeatherDefinitionFile {
    std::uint32_t original, size;
    std::byte *data;
    std::uint32_t *cloudNodes, *rainNodes, *snowNodes;
    std::int32_t cloudCount, rainCount, snowCount;
    std::uint32_t *forecastNodes;
    std::int32_t forecastCount;
};
SHENMUE_WEATHER_CHECK(sizeofWeatherDefinitionFileCheck, sizeof(WeatherDefinitionFile) == 0x2c);
SHENMUE_WEATHER_CHECK(offsetofWeatherDefinitionFileforecastNodesCheck, offsetof(WeatherDefinitionFile, forecastNodes) == 0x24);
struct WeatherTable {
    const std::byte *rows;
    std::int32_t count;
};
SHENMUE_WEATHER_CHECK(sizeofWeatherTableCheck, sizeof(WeatherTable) == 8);

struct WeatherTimeline {
    const std::uint32_t *nodes;
    const std::uint32_t *current;
    std::int32_t nodeCount;
    float blend;
    float intensity;
};
struct WeatherMonth {
    const std::uint8_t *days;
    std::int32_t dayCount;
};
struct WeatherYear {
    const WeatherMonth *months;
    std::int32_t monthCount;
};
struct WeatherData {
    std::int32_t baseYear, year, month, day;
    WeatherTimeline *timelines[4];
    const WeatherYear *years;
    std::int32_t yearCount;
    std::uint32_t flags;
    float unknownValue;
    const std::uint32_t *forecastNodes;
    std::int32_t forecastNodeCount;
};
SHENMUE_WEATHER_CHECK(sizeofWeatherTimelineCheck, sizeof(WeatherTimeline) == 0x14);
SHENMUE_WEATHER_CHECK(offsetofWeatherTimelineintensityCheck, offsetof(WeatherTimeline, intensity) == 0x10);
SHENMUE_WEATHER_CHECK(sizeofWeatherMonthCheck, sizeof(WeatherMonth) == 8);
SHENMUE_WEATHER_CHECK(sizeofWeatherYearCheck, sizeof(WeatherYear) == 8);
SHENMUE_WEATHER_CHECK(sizeofWeatherDataCheck, sizeof(WeatherData) == 0x38);
SHENMUE_WEATHER_CHECK(offsetofWeatherDatatimelinesCheck, offsetof(WeatherData, timelines) == 0x10);
SHENMUE_WEATHER_CHECK(offsetofWeatherDatayearsCheck, offsetof(WeatherData, years) == 0x20);
SHENMUE_WEATHER_CHECK(offsetofWeatherDataforecastNodesCheck, offsetof(WeatherData, forecastNodes) == 0x30);

using ::shenmue::shen_name;
#define WEATHER_CONDITION_STORAGE shen_name(SHEN_NAME4('W', 'C', 'N', 'D'))
#define WEATHER_CALENDAR_STORAGE shen_name(SHEN_NAME4('W', 'D', 'A', 'T'))
#define WEATHER_SCENE_STORAGE shen_name(SHEN_NAME4('W', 'S', 'C', 'N'))
static const std::uint32_t WEATHER_TIMELINE_TASKS[4] = {
    SHEN_NAME4('W', 'I', 'T', '1'), SHEN_NAME4('W', 'I', 'T', '2'), SHEN_NAME4('W', 'I', 'T', '3'), SHEN_NAME4('W', 'I', 'T', '4')};
extern const std::uint32_t auxiliaryTimeline[2];
SHENMUE_WEATHER_DATA SHENMUE_WEATHER_ARRAY(std::uint8_t, weatherMonthDays1, 32) {
    0x09,
    0x07,
    0x07,
    0x07,
    0x08,
    0x08,
    0x07,
    0x05,
    0xb8,
    0x0a,
    0x09,
    0xf9,
    0xf9,
    0x15,
    0xf4,
    0xa7,
    0xbc,
    0x06,
    0x08,
    0x18,
    0x15,
    0x07,
    0xab,
    0xac,
    0x0a,
    0x07,
    0x54,
    0x53,
    0xb7,
    0x16,
    0x19,
    0x00
};
SHENMUE_WEATHER_DATA SHENMUE_WEATHER_ARRAY(std::uint8_t, weatherMonthDays2, 28) {
    0x08,
    0xf7,
    0xe7,
    0x09,
    0x04,
    0x54,
    0x04,
    0x06,
    0x04,
    0x75,
    0xbc,
    0xad,
    0x35,
    0xa9,
    0x06,
    0x08,
    0x15,
    0x08,
    0x09,
    0x07,
    0x16,
    0x07,
    0xf5,
    0xaa,
    0xbc,
    0xb9,
    0xb8,
    0x06
};
SHENMUE_WEATHER_DATA SHENMUE_WEATHER_ARRAY(std::uint8_t, weatherMonthDays3, 32) {
    0xf9,
    0x07,
    0xa4,
    0xba,
    0xb7,
    0xbb,
    0x09,
    0x08,
    0x0b,
    0x0e,
    0x08,
    0x45,
    0xf5,
    0xa9,
    0x08,
    0x06,
    0x06,
    0x0a,
    0x34,
    0x08,
    0x46,
    0x04,
    0x05,
    0xac,
    0xad,
    0x08,
    0x87,
    0x56,
    0x07,
    0x9a,
    0x0b,
    0x00
};
SHENMUE_WEATHER_BEGIN(WeatherMonth, weatherMonths, 12){SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays1), SHEN_FIELD(dayCount) 31},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays2), SHEN_FIELD(dayCount) 28},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays3), SHEN_FIELD(dayCount) 31},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays1), SHEN_FIELD(dayCount) 31},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays3), SHEN_FIELD(dayCount) 31},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays1), SHEN_FIELD(dayCount) 31},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays3), SHEN_FIELD(dayCount) 31},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays1), SHEN_FIELD(dayCount) 31},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays3), SHEN_FIELD(dayCount) 31},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays1), SHEN_FIELD(dayCount) 31},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays3), SHEN_FIELD(dayCount) 31},
    {SHEN_FIELD(days) SHENMUE_WEATHER_DATA_PTR(weatherMonthDays1), SHEN_FIELD(dayCount) 31}, SHENMUE_WEATHER_END;
extern const WeatherYear weatherYears[3];

extern WeatherOverrideState g_weatherOverrideState;
extern float g_weatherReservedOverrides[2];
extern std::int32_t g_weatherShouldUpdateDay;
extern WeatherTable g_weatherTables[4], g_sceneWeatherTables[4];
struct WeatherTableSample;
extern WeatherTableSample *g_sceneWeatherData;
void ReadWeatherConditionData();
void SetWeatherTableMode(std::int32_t mode);
void ResetWeatherOverrides();
void ResetWeatherOverrideState();
void ResetReservedWeatherOverrides();
extern WeatherChannelOverrideState g_weatherChannelOverrides;
extern WeatherDefinitionFile g_weatherDefinitionFile;
extern float g_weatherOverrideReserved;
void SetWeatherChannelOverride(float intensity, std::int32_t channel, std::int32_t enabled);
void AdvanceWeatherOverride(std::int32_t channel);
void SetWeatherOverrideEnabled(std::int32_t enabled);

WeatherData *SetupWeatherData(std::int32_t baseYear);
void FreeWeatherData(WeatherData *data);
void SetWeatherShouldUpdateDay(std::int32_t enabled);
void WSCN_SetWeatherShouldUpdateDay(std::int32_t enabled);
void NormalizeWeatherDate(std::int32_t *year, std::int32_t *month, std::int32_t *day);
void ReadWeatherTimeline(WeatherTimeline *timeline, std::uint32_t time);
void UpdateWeatherTimelines(WeatherData *data, std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute);
float GetWeatherCloudIntensity(const WeatherData *data);
float GetWeatherRainIntensity(const WeatherData *data);
float GetWeatherAuxiliaryIntensity(const WeatherData *data);
std::int32_t GetWeatherPhaseForCurrentDay(const WeatherData *data);
float GetWeatherIntensityForCurrentDay(const WeatherData *data);
void ResetWeatherChannelOverrides();

} // namespace s2
} // namespace shenmue

#undef SHENMUE_WEATHER_DATA
#undef SHENMUE_WEATHER_ARRAY
#undef SHENMUE_WEATHER_BEGIN
#undef SHENMUE_WEATHER_END
#undef SHENMUE_WEATHER_DATA_PTR
#undef SHENMUE_WEATHER_CHECK

#endif
