#include "s2/weather_data_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/scene_data.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include <cmath>
#include "s2/weather_dc.hpp"
#include "s2/cold_archive_dc.hpp"
#include "s2/map_info.hpp"
#include "s2/weather_particles_dc.hpp"
#include "s2/weather_instruction_dc.hpp"
#include "shenmue.h"
#include "s2/weather_wind_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include <cstring>
#include "s2/date_time.hpp"
#include "s2/weather_air_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/weather.hpp"
#include "s2/lens_flare_dc.hpp"
#include "s2/window.hpp"
#include "s2/scenario_subtitles_dc.hpp"
#include "s2/font.hpp"
#include "s2/generated_surface_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/camera_dc.hpp"
#include "s2/air_dc.hpp"
#include "s2/scnw_dc.hpp"
#include "platform/dc/weather_texture.hpp"
#include "platform/dc/ninja.hpp"
#include "s2/character_dc.hpp"
#include "platform/dc/matrix.hpp"

namespace shenmue {
namespace s2 {

void LoadWeatherDefinitionFile();
void ReadWeatherTimeline(WeatherTimeline *timeline, std::uint32_t time);

const std::uint32_t auxiliaryTimeline[2] = {0x00010800U, 0x00fcfdfbU};
const WeatherYear weatherYears[3] = {{SHEN_FIELD(months) weatherMonths, SHEN_FIELD(monthCount) 12},
    {SHEN_FIELD(months) weatherMonths, SHEN_FIELD(monthCount) 12}, {SHEN_FIELD(months) weatherMonths, SHEN_FIELD(monthCount) 12}};
std::int32_t g_weatherShouldUpdateDay = 0;
WeatherChannelOverrideState g_weatherChannelOverrides;
float g_weatherOverrideReserved;
WeatherDefinitionFile g_weatherDefinitionFile;

// @verified - d
void ResetWeatherChannelOverrides() {
    g_weatherChannelOverrides.flags = 0;
    g_weatherChannelOverrides.enabled = 1;
    g_weatherChannelOverrides.values[0] = 0.0f;
    g_weatherChannelOverrides.values[1] = 0.0f;
    g_weatherChannelOverrides.values[2] = 0.0f;
}

// @verified - d
void SetWeatherOverrideEnabled(std::int32_t enabled) {
    g_weatherChannelOverrides.enabled = enabled;
}

// @verified - d
void BindWeatherDefinitionChunks(std::byte *data) {
    if (data) {
        g_weatherDefinitionFile.data = data;
        FindNativeChunk("WDAT", reinterpret_cast<MapInfoChunk *>(data));
        MapInfoChunk *chunk = FindNativeChunk("CLUD", reinterpret_cast<MapInfoChunk *>(data));
        std::int32_t size = static_cast<std::int32_t>(chunk->nextByteOffset);
        g_weatherDefinitionFile.cloudCount = size / 4 - 2;
        ++chunk;
        g_weatherDefinitionFile.cloudNodes = reinterpret_cast<std::uint32_t *>(chunk);
        chunk = FindNativeChunk("RAIN", reinterpret_cast<MapInfoChunk *>(data));
        size = static_cast<std::int32_t>(chunk->nextByteOffset);
        g_weatherDefinitionFile.rainCount = size / 4 - 2;
        ++chunk;
        g_weatherDefinitionFile.rainNodes = reinterpret_cast<std::uint32_t *>(chunk);
        chunk = FindNativeChunk("SNOW", reinterpret_cast<MapInfoChunk *>(data));
        size = static_cast<std::int32_t>(chunk->nextByteOffset);
        g_weatherDefinitionFile.snowCount = size / 4 - 2;
        ++chunk;
        g_weatherDefinitionFile.snowNodes = reinterpret_cast<std::uint32_t *>(chunk);
        chunk = FindNativeChunk("FCAS", reinterpret_cast<MapInfoChunk *>(data));
        size = static_cast<std::int32_t>(chunk->nextByteOffset);
        g_weatherDefinitionFile.forecastCount = size / 4 - 2;
        ++chunk;
        g_weatherDefinitionFile.forecastNodes = reinterpret_cast<std::uint32_t *>(chunk);
    }
}

// @verified - d
bool InitializeWeatherParameters() {
    g_weatherDefinitionFile.original = 0;
    g_weatherDefinitionFile.data = 0;
    g_weatherDefinitionFile.cloudNodes = 0;
    g_weatherDefinitionFile.rainNodes = 0;
    g_weatherDefinitionFile.snowNodes = 0;
    g_weatherDefinitionFile.cloudCount = 0;
    g_weatherDefinitionFile.rainCount = 0;
    g_weatherDefinitionFile.snowCount = 0;
    g_weatherDefinitionFile.forecastNodes = 0;
    g_weatherDefinitionFile.forecastCount = 0;
    LoadWeatherDefinitionFile();
    return g_weatherDefinitionFile.data != 0;
}

// @verified - d
void LoadWeatherDefinitionFile() {
    std::byte *const data = static_cast<std::byte *>(CopyColdArchiveSection(2, false, WEATHER_DATA_STORAGE));
    g_weatherDefinitionFile.size = static_cast<std::uint32_t>(HLib::GetTaskDataSize(data));
    BindWeatherDefinitionChunks(data);
}

// @verified - d
void SelectWeatherDefinitionFile(std::uint32_t original) {
    if (original == g_weatherDefinitionFile.original)
        return;
    if (!original) {
        shenLoadFile("sprite", "WEATHER.BIN", g_weatherDefinitionFile.data, g_weatherDefinitionFile.size);
    } else {
        WeatherDefinitionFile *file = &g_weatherDefinitionFile;
        shenOpenFile("sprite", "WEAT_ORG.BIN");
        const std::uint32_t size = file->size;
        shenLoadFile("sprite", "WEAT_ORG.BIN", file->data, size);
    }
    BindWeatherDefinitionChunks(g_weatherDefinitionFile.data);
    g_weatherDefinitionFile.original = original;
}

void ResetWeatherDefinitions() {
    g_weatherDefinitionFile.original = 0;
    g_weatherDefinitionFile.data = 0;
    g_weatherDefinitionFile.cloudNodes = 0;
    g_weatherDefinitionFile.rainNodes = 0;
    g_weatherDefinitionFile.snowNodes = 0;
    g_weatherDefinitionFile.cloudCount = 0;
    g_weatherDefinitionFile.rainCount = 0;
    g_weatherDefinitionFile.snowCount = 0;
    g_weatherDefinitionFile.forecastNodes = 0;
    g_weatherDefinitionFile.forecastCount = 0;
    LoadWeatherDefinitionFile();
}

WeatherData *SetupWeatherData(std::int32_t baseYear) {
    WeatherData *const data = static_cast<WeatherData *>(HLib::AllocateTaskData(sizeof(WeatherData), shen_name(SHEN_NAME4('W', 'D', 'T', 'A'))));
    data->baseYear = baseYear;
    data->year = 0;
    data->day = 1;
    data->month = 1;
    WeatherTimeline *timeline = static_cast<WeatherTimeline *>(HLib::AllocateTaskData(sizeof(WeatherTimeline), shen_name(SHEN_NAME4('W', 'I', 'T', '1'))));
    timeline->nodes = timeline->current = g_weatherDefinitionFile.cloudNodes;
    timeline->nodeCount = g_weatherDefinitionFile.cloudCount;
    timeline->blend = timeline->intensity = 0.0f;
    data->timelines[0] = timeline;
    timeline = static_cast<WeatherTimeline *>(HLib::AllocateTaskData(sizeof(WeatherTimeline), shen_name(SHEN_NAME4('W', 'I', 'T', '2'))));
    timeline->nodes = timeline->current = g_weatherDefinitionFile.rainNodes;
    timeline->nodeCount = g_weatherDefinitionFile.rainCount;
    timeline->blend = timeline->intensity = 0.0f;
    data->timelines[1] = timeline;
    timeline = static_cast<WeatherTimeline *>(HLib::AllocateTaskData(sizeof(WeatherTimeline), shen_name(SHEN_NAME4('W', 'I', 'T', '3'))));
    timeline->nodes = timeline->current = g_weatherDefinitionFile.snowNodes;
    timeline->nodeCount = g_weatherDefinitionFile.snowCount;
    timeline->blend = timeline->intensity = 0.0f;
    data->timelines[2] = timeline;
    timeline = static_cast<WeatherTimeline *>(HLib::AllocateTaskData(sizeof(WeatherTimeline), shen_name(SHEN_NAME4('W', 'I', 'T', '4'))));
    timeline->nodes = timeline->current = auxiliaryTimeline;
    timeline->nodeCount = 2;
    timeline->blend = timeline->intensity = 0.0f;
    data->timelines[3] = timeline;
    data->years = weatherYears;
    data->yearCount = 3;
    data->flags = 0;
    data->unknownValue = 0.0f;
    data->forecastNodes = g_weatherDefinitionFile.forecastNodes;
    data->forecastNodeCount = g_weatherDefinitionFile.forecastCount;
    return data;
}

// @verified - d
void FreeWeatherData(WeatherData *data) {
    HLib::FreeTaskData(data->timelines[0]);
    HLib::FreeTaskData(data->timelines[1]);
    HLib::FreeTaskData(data->timelines[2]);
    HLib::FreeTaskData(data->timelines[3]);
    HLib::FreeTaskData(data);
}

// @verified - d
void SetWeatherShouldUpdateDay(std::int32_t enabled) {
    g_weatherShouldUpdateDay = enabled;
}

// @verified - d
void NormalizeWeatherDate(std::int32_t *year, std::int32_t *month, std::int32_t *day) {
    *year = 86;
    *month = 12;
    if (g_weatherShouldUpdateDay) {
        *day = (*day - 1) % 10 + 1;
        if (g_weatherShouldUpdateDay > 0) {
            *year = 101;
            *month = 12;
            *day = 31;
        } else {
            *year = 99;
        }
    }
}

// @verified - d
void UpdateWeatherTimelines(WeatherData *data, std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute) {
    NormalizeWeatherDate(&year, &month, &day);
    year = static_cast<std::int32_t>(static_cast<std::uint32_t>(year) - static_cast<std::uint32_t>(data->baseYear));
    if (year < 0) {
        year = 0;
        month = -1;
    } else if (year > 15) {
        year = 15;
        month = 13;
    }
    if (month < 1) {
        month = 1;
        day = -1;
    } else if (month > 12) {
        month = 12;
        day = 32;
    }
    if (day < 1) {
        day = 1;
        hour = -1;
    } else if (day > 31) {
        day = 31;
        hour = 24;
    }
    if (hour < 0) {
        hour = 0;
        minute = -1;
    } else if (hour > 23) {
        hour = 23;
        minute = 60;
    }
    if (minute < 0)
        minute = 0;
    else if (minute > 59)
        minute = 59;
    const std::uint32_t time = (static_cast<std::uint32_t>(year) << 20 | static_cast<std::uint32_t>(month) << 16 | static_cast<std::uint32_t>(day) << 11 |
                                   static_cast<std::uint32_t>(hour) << 6 | static_cast<std::uint32_t>(minute)) &
        0xffffffU;
    data->year = year;
    data->month = month;
    data->day = day;
    ReadWeatherTimeline(data->timelines[0], time);
    ReadWeatherTimeline(data->timelines[1], time);
    ReadWeatherTimeline(data->timelines[2], time);
    ReadWeatherTimeline(data->timelines[3], time);
}

// @verified - d
void ReadWeatherTimeline(WeatherTimeline *timeline, std::uint32_t time) {
    const std::uint32_t *current = timeline->current;
    const std::uint32_t *next = current + 1;
    std::uint32_t start, end;
    for (;;) {
        start = *current & 0xffffffU;
        end = *next & 0xffffffU;
        if (!(end > time)) {
            ++current;
            ++next;
        } else if (!(time >= start)) {
            --current;
            --next;
        } else {
            break;
        }
    }
    timeline->current = current;
    timeline->blend = static_cast<float>(time - start) / static_cast<float>(end - start);
    timeline->intensity = InterpolateWeatherValue(static_cast<float>(*current >> 24) / 255.0f, static_cast<float>(*next >> 24) / 255.0f, timeline->blend);
}

// @verified - d
float GetWeatherCloudIntensity(const WeatherData *data) {
    if ((g_weatherChannelOverrides.flags & 1U) && g_weatherChannelOverrides.enabled)
        return g_weatherChannelOverrides.values[0];
    float value = data->timelines[0]->intensity;
    if (std::fabs(value) < 1.1920928955078125e-07f)
        value = 0.0f;
    return value;
}

// @verified - d
float GetWeatherRainIntensity(const WeatherData *data) {
    if ((g_weatherChannelOverrides.flags & 2U) && g_weatherChannelOverrides.enabled)
        return g_weatherChannelOverrides.values[1];
    float value = data->timelines[1]->intensity;
    if (std::fabs(value) < 1.1920928955078125e-07f)
        value = 0.0f;
    return value;
}

float GetWeatherSnowIntensity(const WeatherData *) {
    return 0.0f;
}

// @verified - d
float GetWeatherAuxiliaryIntensity(const WeatherData *data) {
    float value = data->timelines[3]->intensity;
    if (std::fabs(value) < 1.1920928955078125e-07f)
        value = 0.0f;
    return value;
}

// @verified - d
std::int32_t GetWeatherPhaseForCurrentDay(const WeatherData *data) {
    std::int32_t yearIndex = data->year;
    if (yearIndex < 0)
        yearIndex = 0;
    else if (yearIndex > 2)
        yearIndex = 2;
    const WeatherYear &year = data->years[yearIndex];
    std::int32_t month = data->month - 1;
    if (month < 0)
        month = 0;
    else if (static_cast<std::uint32_t>(month) >= static_cast<std::uint32_t>(year.monthCount))
        month = year.monthCount - 1;
    const WeatherMonth &entry = year.months[month];
    std::int32_t day = data->day - 1;
    if (day < 0)
        day = 0;
    else if (static_cast<std::uint32_t>(day) >= static_cast<std::uint32_t>(entry.dayCount))
        day = entry.dayCount - 1;
    return static_cast<std::int32_t>((entry.days[day] & 0xf0U) << 8);
}

// @verified - d
float GetWeatherIntensityForCurrentDay(const WeatherData *data) {
    std::int32_t yearIndex = data->year;
    if (yearIndex < 0)
        yearIndex = 0;
    else if (yearIndex > 2)
        yearIndex = 2;
    const WeatherYear &year = data->years[yearIndex];
    std::int32_t month = data->month - 1;
    if (month < 0)
        month = 0;
    else if (static_cast<std::uint32_t>(month) >= static_cast<std::uint32_t>(year.monthCount))
        month = year.monthCount - 1;
    const WeatherMonth &entry = year.months[month];
    std::int32_t day = data->day - 1;
    if (day < 0)
        day = 0;
    else if (static_cast<std::uint32_t>(day) >= static_cast<std::uint32_t>(entry.dayCount))
        day = entry.dayCount - 1;
    return static_cast<float>(entry.days[day] & 0xfU) * 0.125f;
}

// @verified - d
void SetWeatherChannelOverride(float intensity, std::int32_t channel, std::int32_t enabled) {
    if (intensity > 1.0f)
        intensity = 1.0f;
    if (intensity < 0.0f)
        intensity = 0.0f;
    switch (channel) {
    case 0:
        if (enabled) {
            g_weatherChannelOverrides.flags |= 1U;
            g_weatherChannelOverrides.values[0] = intensity;
        } else {
            g_weatherChannelOverrides.flags &= ~1U;
            g_weatherChannelOverrides.values[0] = 0.0f;
        }
        break;
    case 1:
        if (enabled) {
            g_weatherChannelOverrides.flags |= 2U;
            g_weatherChannelOverrides.values[1] = intensity;
        } else {
            g_weatherChannelOverrides.flags &= ~2U;
            g_weatherChannelOverrides.values[1] = 0.0f;
        }
        break;
    case 2:
        if (enabled) {
            g_weatherChannelOverrides.flags |= 4U;
            g_weatherChannelOverrides.values[2] = intensity;
        } else {
            g_weatherChannelOverrides.flags &= ~4U;
            g_weatherChannelOverrides.values[2] = 0.0f;
        }
        break;
    }
}

std::int32_t FindWeatherForecast(std::int32_t year, std::int32_t month, std::int32_t day) {
    const std::uint32_t *nodes = g_weatherDefinitionFile.forecastNodes;
    const std::int32_t count = g_weatherDefinitionFile.forecastCount;
    year -= 86;
    if (year < 0)
        return 0;
    year = year << 26 | month << 21 | day << 16;
    for (std::int32_t index = 0; index < count; ++index, ++nodes) {
        const std::uint32_t node = *nodes;
        if (static_cast<std::uint32_t>(year) == (node & 0xbfff0000U))
            return static_cast<std::uint8_t>(node);
    }
    return 0;
}

// @verified - d
std::int32_t FindCurrentWeatherForecastSecondary() {
    const std::uint32_t *nodes = g_weatherDefinitionFile.forecastNodes;
    const std::int32_t count = g_weatherDefinitionFile.forecastCount;
    DateTime now;
    shenGetCurrentDateTime(now);
    now.year -= 86;
    if (now.year < 0)
        return 0;
    const std::uint32_t key = now.year << 26 | now.month << 21 | now.day << 16;
    for (std::int32_t index = 0; index < count; ++index, ++nodes) {
        const std::int32_t node = static_cast<std::int32_t>(*nodes);
        if (key == (static_cast<std::uint32_t>(node) & 0xbfff0000U))
            return node >> 8 & 0xff;
    }
    return 0;
}

} // namespace s2
} // namespace shenmue

#endif
