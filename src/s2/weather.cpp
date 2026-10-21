#include "s2/weather.hpp"
#include "s2/math.hpp"
#include "s2/particle_effects.hpp"

#include "s2/date_time.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/map_info.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/world_render.hpp"
#include "s2/scene_data.hpp"
#include "common/math.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace shenmue::s2 {
float debugSampledWindSpeedOverride = 0.0f;
WeatherFixcNode *g_secondaryWeatherWindList{};
std::int32_t g_planetPreviousRotationDay = 20, g_planetCurrentRotationDay = 41, g_planetRotationCycleDays = 62;
namespace {

constexpr std::size_t weatherRowSize = 0x60;
constexpr std::array<std::uint8_t, 32> weatherMonthDays1{
    0x09, 0x07, 0x07, 0x07, 0x08, 0x08, 0x07, 0x05, 0xb8, 0x0a, 0x09, 0xf9, 0xf9, 0x15, 0xf4, 0xa7,
    0xbc, 0x06, 0x08, 0x18, 0x15, 0x07, 0xab, 0xac, 0x0a, 0x07, 0x54, 0x53, 0xb7, 0x16, 0x19, 0x00};
constexpr std::array<std::uint8_t, 32> weatherMonthDays2{
    0x08, 0xf7, 0xe7, 0x09, 0x04, 0x54, 0x04, 0x06, 0x04, 0x75, 0xbc, 0xad, 0x35, 0xa9, 0x06, 0x08,
    0x15, 0x08, 0x09, 0x07, 0x16, 0x07, 0xf5, 0xaa, 0xbc, 0xb9, 0xb8, 0x06, 0x00, 0x00, 0x00, 0x00};
constexpr std::array<std::uint8_t, 32> weatherMonthDays3{
    0xf9, 0x07, 0xa4, 0xba, 0xb7, 0xbb, 0x09, 0x08, 0x0b, 0x0e, 0x08, 0x45, 0xf5, 0xa9, 0x08, 0x06,
    0x06, 0x0a, 0x34, 0x08, 0x46, 0x04, 0x05, 0xac, 0xad, 0x08, 0x87, 0x56, 0x07, 0x9a, 0x0b, 0x00};
constexpr std::array<WeatherMonth, 12> weatherMonths{{
    {.days = &weatherMonthDays1, .dayCount = 31},
    {.days = &weatherMonthDays2, .dayCount = 28},
    {.days = &weatherMonthDays3, .dayCount = 31},
    {.days = &weatherMonthDays1, .dayCount = 31},
    {.days = &weatherMonthDays3, .dayCount = 31},
    {.days = &weatherMonthDays1, .dayCount = 31},
    {.days = &weatherMonthDays3, .dayCount = 31},
    {.days = &weatherMonthDays1, .dayCount = 31},
    {.days = &weatherMonthDays3, .dayCount = 31},
    {.days = &weatherMonthDays1, .dayCount = 31},
    {.days = &weatherMonthDays3, .dayCount = 31},
    {.days = &weatherMonthDays1, .dayCount = 31},
}};
constexpr std::array<WeatherYear, 3> weatherYears{{
    {.months = weatherMonths.data(), .monthCount = 12},
    {.months = weatherMonths.data(), .monthCount = 12},
    {.months = weatherMonths.data(), .monthCount = 12},
}};


constexpr std::array<float, 144> windDirectionWeights{
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
constexpr std::array<std::uint32_t, 2> auxiliaryTimeline{0x00010800U, 0x00fcfdfbU};
std::array<WeatherTable, 4> weatherTables{};
std::array<WeatherTable, 4> defaultWeatherTables{};
std::array<WeatherTable, 4> sceneWeatherTables{{{.rows = nullptr, .count = 11, .padding0c = 0}, {.rows = nullptr, .count = 10, .padding0c = 0},
    {.rows = nullptr, .count = 10, .padding0c = 0}, {.rows = nullptr, .count = 10, .padding0c = 0}}};
void *sceneWeatherData = nullptr;
std::int32_t weatherTableMode = 0;

alignas(16) std::array<Vector4, kInterpolatedWeatherVectorCount> interpolatedWeatherValues{};
WeatherData *weatherData = nullptr;
WeatherAir *weatherAir = nullptr;
WeatherFloat *cloud = nullptr;
WeatherFloat *auxiliary = nullptr;
WeatherFloat *rain = nullptr;
WeatherFloat *rippleCount = nullptr;
HLib::Task *weatherExecutionTask = nullptr;
std::int32_t weatherUpdateTime = 0;
WeatherWind *weatherWind = nullptr;
std::int32_t weatherUpdateEnabled = 0;
std::int32_t weatherUpdateWind = 0;
std::int32_t weatherUpdateFog = 0;
std::int32_t weatherUpdateClouds = 0;
std::int32_t weatherSkyTransitionFrames = 0;
float *windProfileSamples = nullptr;
float *windMappingTable = nullptr;
float *windImpulseLut = nullptr;
std::int32_t windImpulseLutIndex = 0;
float totalWindProfileWeight = 0.0f;
std::int32_t debugCustomWindPhaseAccum = 0;
std::uint32_t weatherInstruction = 0;
std::int32_t weatherShouldUpdateDay = 0;
std::uint32_t pendingOverrides = 0;
std::uint32_t releaseOverrides = 0;
std::uint32_t activeOverrides = 0;
std::int32_t overrideGate = 1;
std::array<float, 3> overrideValues{};
std::array<WeatherOverride, 2> overrides{};
std::array<std::uint64_t, 2> reservedWeatherOverrideState{};
std::int32_t weatherPublishedYear = 0;
std::int32_t weatherPublishedMonth = 0;
std::int32_t weatherPublishedDay = 0;
std::int32_t weatherPublishedHour = 0;
std::int32_t weatherPublishedMinute = 0;
float minimumSunElevation{};

float FlushSmall(float value) {
    return std::fabs(value) < std::numeric_limits<float>::epsilon() ? 0.0f : value;
}
float SinSubNinetyDegrees(float angle) {
    return std::sin(1.5707964f - std::fabs(angle));
}

Vector3 BuildWeatherSunDirection(const WeatherAir &air) {
    constexpr float nativePi = 3.1400001f;
    constexpr std::array<std::int32_t, 12> accumulatedDays{
        31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    const std::size_t month = static_cast<std::size_t>(std::clamp(air.month, 1, 12) - 1);
    const float sunAngle =
        (((static_cast<float>(air.minute) / 60.0f + static_cast<float>(air.hour) + static_cast<float>(air.second) / 3600.0f) - 12.0f) * -15.0f * nativePi) /
        180.0f;
    const float seasonalAngle = (static_cast<float>(accumulatedDays[5] - accumulatedDays[month] - air.day + 21) / 365.0f * 360.0f / 180.0f) * nativePi;
    const float elevationAngle = 90.0f - (22.18f - SinSubNinetyDegrees(seasonalAngle) * 23.5f);
    const float elevation = std::fmax(minimumSunElevation, std::fabs(SinSubNinetyDegrees(sunAngle) * elevationAngle * 1.2f));



    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY(std::sin(sunAngle), std::cos(sunAngle));
    const float elevationRadians = -(elevation * nativePi / 180.0f);
    Matrix4x4::RotateAroundX(std::sin(elevationRadians), std::cos(elevationRadians));
    Vector3 direction{-workTransformMatrix.rows[2].x, -workTransformMatrix.rows[2].y, -workTransformMatrix.rows[2].z};
    Matrix4x4::Pop(1);
    direction.x = FlushSmall(direction.x);
    direction.y = FlushSmall(direction.y);
    direction.z = FlushSmall(direction.z);

    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (control && control->sunLocation != 0) {
        const auto angle = static_cast<std::uint16_t>(-static_cast<std::int16_t>(control->sunLocation));
        const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
        const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle)));
        const float x = direction.x;
        const float z = direction.z;
        direction.x = FlushSmall(FlushSmall(cosine * x) - FlushSmall(sine * z));
        direction.z = FlushSmall(FlushSmall(cosine * z) + FlushSmall(sine * x));
    }
    return direction;
}

float InterpolateValue(float current, float next, float blend) {
    const float difference = next - current;
    if (std::fabs(difference) < std::numeric_limits<float>::epsilon())
        return current + 0.0f;
    return current + FlushSmall(difference * blend);
}

std::uint32_t InterpolateColour(std::uint32_t current, std::uint32_t next, float blend) {
    std::uint32_t result = 0;
    for (std::uint32_t shift = 0; shift < 32; shift += 8) {
        const auto first = static_cast<std::int32_t>((current >> shift) & 0xffU);
        const auto second = static_cast<std::int32_t>((next >> shift) & 0xffU);

        const float difference = static_cast<float>(second - first);
        const float scaled = std::fabs(difference) < std::numeric_limits<float>::epsilon() ? 0.0f : FlushSmall(difference * blend);
        const std::int32_t step = TruncateToInt32(scaled);
        result |= (static_cast<std::uint32_t>(first) + static_cast<std::uint32_t>(step)) << shift;
    }
    return result;
}

std::uint32_t AddPackedColour(std::uint32_t first, std::uint32_t second) {
    std::uint32_t result = 0;
    for (std::uint32_t shift = 0; shift < 32; shift += 8) {
        const std::uint32_t sum = ((first >> shift) & 0xffU) + ((second >> shift) & 0xffU);
        result |= std::min(sum, 255U) << shift;
    }
    return result;
}

WeatherTableSample ReadWeatherSample(const std::byte *row) {
    WeatherTableSample sample{};
    std::memcpy(&sample, row, sizeof(sample));
    return sample;
}

void AccumulateWeatherSample(WeatherTableSample &output, const WeatherTableSample &input, float weight);
WeatherTableSample InterpolateWeatherSample(const WeatherTableSample &first, const WeatherTableSample &second, float blend, std::int32_t seconds) {
    WeatherTableSample result{};
    result.seconds = seconds;
    for (std::size_t index = 0; index < result.primaryValues.size(); ++index) {

        const bool skipBlend = index >= 3 && index <= 6 && std::fabs(blend) < std::numeric_limits<float>::epsilon();
        result.primaryValues[index] =
            skipBlend ? first.primaryValues[index] + 0.0f : InterpolateValue(first.primaryValues[index], second.primaryValues[index], blend);
    }
    for (std::size_t index = 0; index < result.primaryColours.size(); ++index) {
        result.primaryColours[index] = InterpolateColour(first.primaryColours[index], second.primaryColours[index], blend);
    }
    for (std::size_t index = 0; index < result.secondaryValues.size(); ++index) {
        result.secondaryValues[index] = InterpolateValue(first.secondaryValues[index], second.secondaryValues[index], blend);
    }
    for (std::size_t index = 0; index < result.environmentColours.size(); ++index) {
        result.environmentColours[index] = InterpolateColour(first.environmentColours[index], second.environmentColours[index], blend);
    }
    return result;
}

WeatherTableSample SampleWeatherTable(const WeatherTable &table, std::int32_t seconds) {
    if (table.count == 1) {
        WeatherTableSample result{};
        AccumulateWeatherSample(result, ReadWeatherSample(table.rows), 1.0f);
        result.seconds = 0;
        return result;
    }
    const std::byte *current = table.rows;
    const std::byte *next = current + weatherRowSize;
    std::int32_t startSeconds = 0;
    std::int32_t endSeconds = 0;
    std::int32_t index = 1;
    for (; index < table.count; ++index) {
        std::memcpy(&startSeconds, current, sizeof(startSeconds));
        std::memcpy(&endSeconds, next, sizeof(endSeconds));
        if (startSeconds <= seconds && seconds < endSeconds)
            break;
        current += weatherRowSize;
        next += weatherRowSize;
    }
    if (index == table.count) {

        next = table.rows;
        endSeconds = 86400;
    }
    const std::int32_t elapsed = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(seconds) - static_cast<std::uint32_t>(startSeconds));
    const std::int32_t duration = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(endSeconds) - static_cast<std::uint32_t>(startSeconds));
    const float blend = static_cast<float>(elapsed) / static_cast<float>(duration);
    return InterpolateWeatherSample(ReadWeatherSample(current), ReadWeatherSample(next), blend, seconds);
}

void AccumulateWeatherSample(WeatherTableSample &output, const WeatherTableSample &input, float weight) {
    const std::int32_t weightedSeconds = TruncateToInt32(FlushSmall(static_cast<float>(input.seconds) * weight));
    output.seconds = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(output.seconds) + static_cast<std::uint32_t>(weightedSeconds));
    for (std::size_t index = 0; index < output.primaryValues.size(); ++index) {
        output.primaryValues[index] = FlushSmall(output.primaryValues[index] + FlushSmall(input.primaryValues[index] * weight));
    }
    for (std::size_t index = 0; index < output.primaryColours.size(); ++index) {
        output.primaryColours[index] = AddPackedColour(output.primaryColours[index], InterpolateColour(0, input.primaryColours[index], weight));
    }
    for (std::size_t index = 0; index < output.secondaryValues.size(); ++index) {
        output.secondaryValues[index] = FlushSmall(output.secondaryValues[index] + FlushSmall(input.secondaryValues[index] * weight));
    }
    for (std::size_t index = 0; index < output.environmentColours.size(); ++index) {
        output.environmentColours[index] = AddPackedColour(output.environmentColours[index], InterpolateColour(0, input.environmentColours[index], weight));
    }
}

void SetWeatherColourImmediate(WeatherColour &colour, std::uint32_t packed) {
    for (std::uint32_t index = 0; index < 4; ++index) {
        const std::uint32_t shift = 24U - index * 8U;
        const float component = static_cast<float>((packed >> shift) & 0xffU);
        colour.current[index] = component;
        colour.target[index] = component;
        colour.step[index] = 0.0f;
    }
    colour.frame = colour.frameCount = 0;
}
// @verified - p
std::array<float, 4> *GetWeatherColourVector(WeatherColour *colour, std::array<float, 4> *output) {
    for (std::size_t index = 0; index < output->size(); ++index) {
        colour->current[index] = FlushSmall(colour->current[index]);
        (*output)[index] = colour->current[index];
    }
    return output;
}
std::uint32_t TruncateWeatherColourComponent(float value) {
    const std::int64_t integer =
        value >= -9223372036854775808.0f && value < 9223372036854775808.0f ? static_cast<std::int64_t>(value) : (std::numeric_limits<std::int64_t>::min)();
    return static_cast<std::uint32_t>(integer);
}

// @verified - p
std::uint32_t wairPackRainColorRGBA(WeatherAir *air) {
    std::array<float, 4> colour;
    GetWeatherColourVector(air->colour50, &colour);
    return (((TruncateWeatherColourComponent(colour[0]) << 8U) | TruncateWeatherColourComponent(colour[1])) << 16U) |
        (TruncateWeatherColourComponent(colour[2]) << 8U) | TruncateWeatherColourComponent(colour[3]);
}

// @verified - p
std::uint32_t wairPackRippleColorRGBA(WeatherAir *air) {
    std::array<float, 4> colour;
    GetWeatherColourVector(air->colour60, &colour);
    return (((TruncateWeatherColourComponent(colour[0]) << 8U) | TruncateWeatherColourComponent(colour[1])) << 16U) |
        (TruncateWeatherColourComponent(colour[2]) << 8U) | TruncateWeatherColourComponent(colour[3]);
}

void UpdateFog(WeatherAir &air) {
    const float cloudWeight = std::fmin(1.0f, air.cloud * 4.0f);
    const float rainWeight = std::fmin(1.0f, air.rain * 3.0f);
    const float snowWeight = std::fmin(1.0f, air.snow * 3.0f);
    const float clearWeight = 1.0f - std::fmax(snowWeight, std::fmax(rainWeight, cloudWeight));
    const float total = ((clearWeight + cloudWeight) + rainWeight) + snowWeight;
    const std::array<float, 4> weights{
        FlushSmall(clearWeight / total), FlushSmall(cloudWeight / total), FlushSmall(rainWeight / total), FlushSmall(snowWeight / total)};
    WeatherTableSample sample{};
    for (std::int32_t index = 0; index < air.tableCount; ++index) {
        const float weight = weights[static_cast<std::size_t>(index)];
        if (weight > 0.0f) {
            AccumulateWeatherSample(sample, SampleWeatherTable(air.tables[index], air.seconds), weight);
        }
    }



    std::memcpy(interpolatedWeatherValues.data(), &sample, sizeof(sample));
    const Vector4 lightColour{sample.primaryValues[3], sample.primaryValues[4], sample.primaryValues[5], sample.primaryValues[6]};
    if (weatherInstruction & 0x1U) {
        const Vector3 sunDirection = BuildWeatherSunDirection(air);
        ApplyWeatherLightState(air.lightIndex, nullptr, nullptr, &sunDirection);
    }
    if (weatherInstruction & 0x2U)
        ApplyWeatherLightState(air.lightIndex, &lightColour, nullptr, nullptr);
    SetWeatherLighting(sample.primaryValues[7]);
    if (weatherInstruction & 0x4U) {
        ApplyWeatherGlobalLighting(sample.primaryValues[7]);
    }
    SetWeatherSceneLighting(sample.primaryValues[8]);
    if (weatherInstruction & 0x8U) {
        ApplyWeatherLightState(air.lightIndex, nullptr, &sample.primaryValues[8], nullptr);
    }
    if (weatherInstruction & 0x10U)
        SetWeatherColourImmediate(*air.colour50, sample.primaryColours[0]);
    if (weatherInstruction & 0x20U)
        SetWeatherColourImmediate(*air.colour58, sample.primaryColours[1]);
    if (weatherInstruction & 0x40U)
        SetWeatherColourImmediate(*air.colour60, sample.primaryColours[2]);

    if (weatherInstruction & 0x80U)
        SetFogRed(sample.secondaryValues[0]);
    if (weatherInstruction & 0x100U)
        SetFogGreen(sample.secondaryValues[1]);
    if (weatherInstruction & 0x200U)
        SetFogBlue(sample.secondaryValues[2]);
    if (weatherInstruction & 0x400U)
        SetFogDistance(sample.environmentColours[0] & 0xffffffU);
    if (weatherInstruction & 0x780U)
        ReinitFogTasks(1);
    if (weatherInstruction & 0x1800U) {
        const std::uint32_t firstColour = sample.environmentColours[1] | 0xff000000U;
        const std::uint32_t secondColour = sample.environmentColours[2] | 0xff000000U;
        EV_SetPlanetLayerColours_Impl(firstColour, secondColour, 0);
        EV_SetPlanetLayerColours_Impl(firstColour, secondColour, 1);
        SetCloudAndSkyColour(sample.environmentColours[3] | 0xff000000U, sample.environmentColours[4] | 0xff000000U);
    }
}

void ReadTimeline(WeatherTimeline &timeline, std::uint32_t time) {
    const std::uint32_t *current = timeline.current;
    const std::uint32_t *next = current + 1;
    while ((*next & 0xffffffU) <= time) {
        ++current;
        ++next;
    }
    while (time < (*current & 0xffffffU)) {
        --current;
        --next;
    }
    timeline.current = current;
    const std::uint32_t start = *current & 0xffffffU;
    timeline.blend = static_cast<float>(time - start) / static_cast<float>((*next & 0xffffffU) - start);
    timeline.intensity = InterpolateValue(static_cast<float>(*current >> 24U) / 255.0f, static_cast<float>(*next >> 24U) / 255.0f, timeline.blend);
}



void UpdateWeatherTimelines(std::int32_t day, std::int32_t hour, std::int32_t minute) {
    std::int32_t year = 86;
    if (weatherShouldUpdateDay != 0) {
        day = (day - 1) % 10 + 1;
        if (weatherShouldUpdateDay <= 0) {
            year = 99;
        } else {
            year = 101;
            day = 31;
        }
    }
    year -= weatherData->baseYear;
    std::int32_t month = 12;
    if (year < 0) {
        year = 0;
        month = 1;
        day = 1;
        hour = minute = 0;
    } else if (year > 15) {
        year = 15;
        day = 31;
        hour = 23;
        minute = 59;
    } else if (day < 1) {
        day = 1;
        hour = minute = 0;
    } else if (day > 31) {
        day = 31;
        hour = 23;
        minute = 59;
    } else if (hour < 0) {
        hour = minute = 0;
    } else if (hour > 23) {
        hour = 23;
        minute = 59;
    } else {
        minute = std::clamp(minute, 0, 59);
    }
    weatherData->year = year;
    weatherData->month = month;
    weatherData->day = day;
    const std::uint32_t time =
        static_cast<std::uint32_t>(((minute | ((month | (16 * (year & 0xf))) << 16)) & 0x00ffffff) | (((hour & 0x3ffff) | (32 * (day & 0x1fff))) << 6));
    for (WeatherTimeline *timeline : weatherData->timelines)
        ReadTimeline(*timeline, time);
}


std::int32_t GetWeatherPhaseForCurrentDay(const WeatherData &data) {
    const WeatherYear &year = data.years[data.year < 0 ? 0 : data.year >= 3 ? 2 : data.year];
    std::int32_t month = data.month - 1;
    if (month < 0)
        month = 0;
    else if (static_cast<std::uint32_t>(month) >= static_cast<std::uint32_t>(year.monthCount))
        month = static_cast<std::int32_t>(year.monthCount) - 1;
    const WeatherMonth &entry = year.months[month];
    std::int32_t day = data.day - 1;
    if (day < 0)
        return ((*entry.days)[0] >> 4) << 12;
    if (static_cast<std::uint32_t>(day) >= static_cast<std::uint32_t>(entry.dayCount))
        day = static_cast<std::int32_t>(entry.dayCount) - 1;
    return ((*entry.days)[day] >> 4) << 12;
}


float GetWeatherIntensityForCurrentDay(const WeatherData &data) {
    const WeatherYear &year = data.years[data.year < 0 ? 0 : data.year >= 3 ? 2 : data.year];
    std::int32_t month = data.month - 1;
    if (month < 0)
        month = 0;
    else if (static_cast<std::uint32_t>(month) >= static_cast<std::uint32_t>(year.monthCount))
        month = static_cast<std::int32_t>(year.monthCount) - 1;
    const WeatherMonth &entry = year.months[month];
    std::int32_t day = data.day - 1;
    if (day < 0)
        day = 0;
    else if (static_cast<std::uint32_t>(day) >= static_cast<std::uint32_t>(entry.dayCount))
        day = static_cast<std::int32_t>(entry.dayCount) - 1;
    return static_cast<float>((*entry.days)[day] & 0xfU) * 0.125f;
}

WeatherWind *AllocateWIND() {
    auto *const wind = static_cast<WeatherWind *>(HLib::AllocateTaskData(sizeof(WeatherWind), SHEN_NAME(WIND)));
    if (!wind)
        return nullptr;
    std::memset(wind, 0, sizeof(*wind));
    wind->windScaleDenominator = 55.651402f;
    return wind;
}

// @verified - p
void BeginWindPhaseTransition(WeatherWind &wind, std::int32_t phase) {
    wind.cooldownFramesRemaining = 0;
    wind.requestedPhase = phase;
    wind.phaseFramesRemaining = 10;
    wind.phaseStep = static_cast<std::int32_t>(static_cast<std::uint32_t>(phase) - static_cast<std::uint32_t>(wind.targetPhase)) / 10;
}

// @verified - p
void BeginWindSpeedTransition(WeatherWind &wind, float speed) {
    wind.cooldownFramesRemaining = 0;
    wind.requestedWindSpeed = speed;
    wind.windSpeedFramesRemaining = 1;
    wind.windSpeedStep = speed - wind.currentWindSpeed;
}


void GenerateWeibullProfile(float *samples, std::int32_t count, float shape, float scale) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    const float total = static_cast<float>(count);
    for (float x = 0.0f; total > x; x += 1.0f, ++samples) {
        float u = 0.0f;
        if (!(epsilon > std::fabs(scale)) && !(epsilon > std::fabs(x / total))) {
            u = x / total * scale;
            if (epsilon > std::fabs(u))
                u = 0.0f;
        }
        float result = 0.0f;
        if (!(epsilon > std::fabs(shape)) && !(epsilon > std::fabs(u)) && !(epsilon > std::fabs(shape - 1.0f))) {
            const float power = fast_powf_approx(u, shape - 1.0f);
            const float weighted = power * shape;
            const float exponent = -(power * u);
            if (!(epsilon > std::fabs(power)) && !(epsilon > std::fabs(weighted)) && !(epsilon > std::fabs(exponent))) {
                const float decay = fast_expf_approx(exponent);
                if (!(epsilon > std::fabs(decay)))
                    result = weighted * decay;
            }
        }
        *samples = result;
    }
}



void AllocateTMP0_TMP1(float *output, std::uint32_t count, std::uint32_t taps) {
    auto *const coefficients = static_cast<float *>(HLib::AllocateTaskData(4ULL * taps, SHEN_NAME(TMP0)));
    auto *const history = static_cast<float *>(HLib::AllocateTaskData(4ULL * taps, SHEN_NAME(TMP1)));
    coefficients[0] = 1.0f;
    float sum = 1.0f;
    float n = 1.0f;
    for (std::uint32_t index = 1; index < taps; ++index) {
        const float twice = n + n;
        n += 1.0f;
        coefficients[index] = (twice - 1.0f) * coefficients[index - 1] / twice;
        sum += coefficients[index];
    }
    for (std::uint32_t index = 0; index < taps; ++index)
        history[index] = IncrementSharedPrngFloat();
    for (std::uint32_t sample = 0; sample < count; ++sample) {
        float value = 0.0f;
        for (std::uint32_t index = 0; index < taps; ++index)
            value += history[index] * coefficients[index];
        *output++ = value / sum;
        for (std::uint32_t index = 0; index + 1 < taps; ++index)
            history[index] = history[index + 1];
        history[taps - 1] = IncrementSharedPrngFloat();
    }
    HLib::FreeTaskData(history);
    HLib::FreeTaskData(coefficients);
}



void InitWeibullResources() {
    windProfileSamples = static_cast<float *>(HLib::AllocateTaskData(0xc0, SHEN_NAME(WBLT)));
    GenerateWeibullProfile(windProfileSamples, 48, 1.78f, 4.8f);
    float total = 0.0f;
    for (std::size_t index = 0; index < 48; ++index)
        total += windProfileSamples[index];
    totalWindProfileWeight = total;
    windMappingTable = static_cast<float *>(HLib::AllocateTaskData(0x240, SHEN_NAME(S2AT)));
    for (std::size_t column = 0; column < 9; ++column) {
        float cumulative = 0.0f;
        for (std::size_t row = 0; row < 16; ++row) {
            cumulative += windDirectionWeights[row * 9 + column];
            windMappingTable[row * 9 + column] = cumulative;
        }
    }
    windImpulseLut = static_cast<float *>(HLib::AllocateTaskData(0x400, shen_name{make_shen_name('1', '/', 'F', 'W')}));
    AllocateTMP0_TMP1(windImpulseLut, 256, 13);
    windImpulseLut[0] = 0.0f;
    windImpulseLutIndex = 0;
}


float SampleWindFromProfile(float baseSpeed, const float *profile) {
    if (baseSpeed <= 0.0f)
        return 0.0f;
    const float impulse = windImpulseLut[windImpulseLutIndex] - 0.35f;
    windImpulseLutIndex = windImpulseLutIndex + 1 >= 256 ? 0 : windImpulseLutIndex + 1;
    const float range = totalWindProfileWeight + 1.0f;
    const float positive = 0.0f > impulse ? 0.0f : impulse;
    const float doubled = positive + positive;
    const float threshold = (1.0f < doubled ? 1.0f : doubled) * range;
    float cumulative = 0.0f;
    std::int32_t band = 0;
    while (band < 48) {
        cumulative += profile[band];
        if (cumulative > threshold)
            break;
        ++band;
    }
    if (band < 48)
        return static_cast<float>(band - 1) * baseSpeed * 0.1f;
    const float overflow = (threshold - totalWindProfileWeight) / (range - totalWindProfileWeight) * 80.0f * baseSpeed * 0.1f;
    return 0.6666667f < overflow ? 0.6666667f : overflow;
}


std::int32_t MapWindToImpulseIndex(float sampledSpeed, const float *table) {
    std::int32_t bin = static_cast<std::int32_t>(sampledSpeed * 30.0f);
    if (bin > 8)
        bin = 8;
    const float target = IncrementSharedPrngFloat() * table[135 + bin];
    std::int32_t row = 0;
    while (row < 16 && !(table[row * 9 + bin] > target))
        ++row;
    return (row << 12) - 0x1000;
}


void WindDebugTrace(std::int32_t, float, float, float, float, float) {
}


void UpdateWindWithSpeed(WeatherWind &wind) {
    if (wind.requestedPhase != wind.targetPhase) {
        if (wind.phaseFramesRemaining > 0) {
            --wind.phaseFramesRemaining;
            wind.targetPhase = static_cast<std::int32_t>(static_cast<std::uint32_t>(wind.requestedPhase) -
                static_cast<std::uint32_t>(wind.phaseStep) * static_cast<std::uint32_t>(wind.phaseFramesRemaining));
        } else {
            wind.targetPhase = wind.requestedPhase;
        }
    }
    float speed = wind.requestedWindSpeed;
    if (speed != wind.currentWindSpeed) {
        if (wind.windSpeedFramesRemaining > 0) {
            --wind.windSpeedFramesRemaining;
            speed -= static_cast<float>(wind.windSpeedFramesRemaining) * wind.windSpeedStep;
        }
        wind.currentWindSpeed = speed;
    }
    if (wind.cooldownFramesRemaining > 0) {
        --wind.cooldownFramesRemaining;
    } else if (wind.holdPreviousOutput) {
        wind.outputPhase = wind.targetPhase;
        wind.sampledWindSpeed = wind.currentWindSpeed;
    } else {
        const float sampled = SampleWindFromProfile(wind.currentWindSpeed, windProfileSamples);
        const std::int32_t impulse = MapWindToImpulseIndex(sampled, windMappingTable);
        const float hold = windImpulseLut[windImpulseLutIndex] * 1600.0f;
        windImpulseLutIndex = windImpulseLutIndex + 1 >= 256 ? 0 : windImpulseLutIndex + 1;
        const std::int32_t magnitude = impulse >= 0x8000 ? impulse ^ 0xffff : impulse;
        wind.sampledWindSpeed = sampled;
        wind.cooldownFramesRemaining = static_cast<std::int32_t>(hold) - magnitude / 1024;
        wind.outputPhase = static_cast<std::uint16_t>(static_cast<std::uint32_t>(wind.targetPhase) + static_cast<std::uint32_t>(impulse));
    }
    const float scale = wind.sampledWindSpeed / wind.windScaleDenominator;
    const std::uint16_t phase = static_cast<std::uint16_t>(wind.outputPhase);
    wind.windDirection.x = -(sin_fixed16(static_cast<std::int16_t>(phase)) * scale) * 0.5f;
    wind.windDirection.z = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(phase))) * scale * 0.5f;
    WindDebugTrace(wind.cooldownFramesRemaining, wind.sampledWindSpeed, static_cast<float>(wind.outputPhase) / 182.04445f, wind.currentWindSpeed,
        static_cast<float>(wind.targetPhase) / 182.04445f, wind.windScaleDenominator);
}

float ReadWeatherChannel(std::size_t channel) {
    if (channel < overrideValues.size() && overrideGate && (activeOverrides & (1U << channel)))
        return overrideValues[channel];
    return FlushSmall(weatherData->timelines[channel]->intensity);
}

WeatherFloat *AllocateWeatherFloat() {
    auto *const value = static_cast<WeatherFloat *>(HLib::AllocateTaskData(sizeof(WeatherFloat), SHEN_NAME(LFLT)));
    std::memset(value, 0, sizeof(*value));
    return value;
}

WeatherColour *AllocateWeatherColour() {
    auto *const value = static_cast<WeatherColour *>(HLib::AllocateTaskData(sizeof(WeatherColour), SHEN_NAME(LRGB)));
    std::memset(value, 0, sizeof(*value));
    return value;
}

void BeginTransition(WeatherFloat &value, float target, std::int32_t frames) {
    if (frames < 1) {
        (void)lfltSetImmediate(&value, target);
        return;
    }
    value.target = FlushSmall(target);
    value.step = FlushSmall(FlushSmall(value.target - value.current) / static_cast<float>(frames));
    value.frame = 0;
    value.frameCount = frames;
}

void CopyWeatherTime(WeatherAir &air, const DateTime &time) {
    air.month = time.month;
    air.day = time.day;
    air.hour = time.hour;
    air.minute = time.minute;
    air.second = time.second;
    air.seconds = time.second + 60 * (time.minute + 60 * time.hour);
}

// @verified - p
WeatherAir *AllocateWeatherAir(std::uint32_t lightIndex, const WeatherTable *tables, std::int32_t tableCount) {
    WeatherAir *const air = static_cast<WeatherAir *>(HLib::AllocateTaskData(sizeof(WeatherAir), TASK_WAIR));
    air->lightIndex = lightIndex;
    air->tables = tables;
    air->tableCount = tableCount;
    air->instruction = 0x3fffU;
    DateTime time;
    GetCurrentDateTime(time);
    CopyWeatherTime(*air, time);
    air->cloud = air->rain = air->snow = air->auxiliary = 0.0f;
    air->field40 = air->field44 = 0;
    air->mode = -1;
    air->colour50 = AllocateWeatherColour();
    air->colour58 = AllocateWeatherColour();
    air->colour60 = AllocateWeatherColour();
    air->float68 = AllocateWeatherFloat();
    air->float70 = AllocateWeatherFloat();
    air->colour78 = AllocateWeatherColour();
    air->colour80 = AllocateWeatherColour();
    air->colour88 = AllocateWeatherColour();
    return air;
}

// @verified - p
void FreeWeatherAir(WeatherAir *air) {
    HLib::FreeTaskData(air->colour50);
    HLib::FreeTaskData(air->colour58);
    HLib::FreeTaskData(air->colour60);
    HLib::FreeTaskData(air->float68);
    HLib::FreeTaskData(air->float70);
    HLib::FreeTaskData(air->colour78);
    HLib::FreeTaskData(air->colour80);
    HLib::FreeTaskData(air->colour88);
    HLib::FreeTaskData(air);
}

void PublishWeatherAir(bool initial = false) {
    weatherAir->cloud = cloud->current = FlushSmall(cloud->current);
    weatherAir->rain = GetWeatherPressure();
    weatherAir->auxiliary = auxiliary->current = FlushSmall(auxiliary->current);
    if (weatherUpdateFog == 1)
        UpdateFog(*weatherAir);
    if (weatherUpdateClouds == 1) {
        if (initial)
            UpdateClouds(weatherAir);
        else
            UpdateWeatherClouds(weatherAir, weatherSkyTransitionFrames);
    }
    if (weatherUpdateWind == 1) {
        SetWeatherParticleRainColour(wairPackRainColorRGBA(weatherAir));
        SetWeatherParticleRippleColour(wairPackRippleColorRGBA(weatherAir));
    }
}

void ProcessNewWeatherData() {
    if (!weatherAir)
        return;
    (void)AdvanceWeatherFloat(cloud);
    (void)AdvanceWeatherFloat(auxiliary);
    DateTime time;
    GetCurrentDateTime(time);
    CopyWeatherTime(*weatherAir, time);
    PublishWeatherAir();
}

void UpdateSunOrWindRotationVector() {
    const float speed = std::fmax(GetWeatherPressure() * 0.00099999993f, GetWeatherWindVector() * 0.00050000002f) + 0.00050000002f;
    DateTime time{};
    GetCurrentDateTime(time);
    const float timeOfDay = ((static_cast<float>(time.minute) / 60.0f) * 10000.0f + static_cast<float>(time.hour) * 10000.0f) +
        ((static_cast<float>(time.second) / 60.0f) * 100.0f) * 1.666666f;
    const std::uint16_t angle = static_cast<std::uint16_t>(static_cast<std::int32_t>((timeOfDay * 0.0000083333334f) * 65536.0f));
    const float x = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle))) * speed;
    const float y = sin_fixed16(static_cast<std::int16_t>(angle)) * speed;
    shenUpdatePlanetByLayer(x, 0, 0);
    shenUpdatePlanetByLayer(y, 1, 0);
    shenUpdatePlanetByLayer(x, 0, 1);
    shenUpdatePlanetByLayer(y, 1, 1);
}

void AdvanceOverride(std::size_t channel) {
    WeatherOverride &value = overrides[channel];
    const std::uint32_t mask = 1U << channel;
    value.frames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(value.frames) - 1U);
    if (value.frames > 0) {
        value.current += value.step;
    } else {
        value.current = value.target;
        value.frames = 0;
        if (releaseOverrides & mask) {
            pendingOverrides &= ~mask;
            releaseOverrides &= ~mask;
            activeOverrides &= ~mask;
            overrideValues[channel] = 0.0f;
            weatherSkyTransitionFrames = 500;
            return;
        }
    }
    activeOverrides |= mask;
    overrideValues[channel] = std::fmax(0.0f, std::fmin(1.0f, value.current));
}

void BeginWeatherRainTransition(float target, std::int32_t frames) {
    BeginRainPressureTransitionForWeatherEffects(target, frames);
    BeginTransition(*rain, target, frames);
    ComputeCloudVisibility(target, frames);
}

void UpdateWeatherForCurrentTimeImmediate() {
    weatherUpdateTime = 1;
    DateTime time;
    GetCurrentDateTime(time);
    UpdateWeatherTimelines(time.day, time.hour, time.minute);
    (void)lfltSetImmediate(cloud, ReadWeatherChannel(0));
    (void)lfltSetImmediate(auxiliary, ReadWeatherChannel(3));
    SetWeatherRainPressureImmediate(ReadWeatherChannel(1));
    if (weatherWind) {
        BeginWindPhaseTransition(*weatherWind, GetWeatherPhaseForCurrentDay(*weatherData));
        BeginWindSpeedTransition(*weatherWind, GetWeatherIntensityForCurrentDay(*weatherData) / 30.0f);
    }
    weatherPublishedYear = time.year;
    weatherPublishedMonth = time.month;
    weatherPublishedDay = time.day;
    weatherPublishedHour = time.hour;
    weatherPublishedMinute = time.minute;
}

// @verified - p
void ExecuteWeather_Callback(void *) {
    if (weatherUpdateEnabled == 0)
        return;
    weatherRippleLifetimeScale = 1.0f;
    weatherRippleLifetimeStep = 0.050000001f;
    if (weatherUpdateTime == 1) {
        DateTime time;
        GetCurrentDateTime(time);
        if (weatherPublishedYear != time.year || weatherPublishedMonth != time.month || weatherPublishedDay != time.day) {
            if (weatherWind) {
                UpdateWeatherTimelines(time.day, time.hour, time.minute);
                BeginWindPhaseTransition(*weatherWind, GetWeatherPhaseForCurrentDay(*weatherData));
                BeginWindSpeedTransition(*weatherWind, GetWeatherIntensityForCurrentDay(*weatherData) / 30.0f);
            }
            weatherPublishedYear = time.year;
            weatherPublishedMonth = time.month;
            weatherPublishedDay = time.day;
        }
        if (pendingOverrides & 1U) {
            AdvanceOverride(0);
            BeginTransition(*cloud, overrides[0].current, 0);
        }
        if (pendingOverrides & 10U) {
            if ((pendingOverrides & 8U) == 0)
                AdvanceOverride(1);
            BeginWeatherRainTransition(overrides[1].current, 0);
        }
        if (weatherPublishedHour != time.hour || weatherPublishedMinute != time.minute) {
            DateTime forecast = time;
            const DateTime oneMinute{.minute = 1};
            IncrementDateTimeInPlace(forecast, oneMinute);
            UpdateWeatherTimelines(forecast.day, forecast.hour, forecast.minute);
            if (!(pendingOverrides & 1U))
                BeginTransition(*cloud, ReadWeatherChannel(0), 80);
            if (!(pendingOverrides & 10U)) {
                BeginRainPressureTransitionForWeatherEffects(ReadWeatherChannel(1), 80);
                BeginTransition(*rain, ReadWeatherChannel(1), 80);
                ComputeCloudVisibility(ReadWeatherChannel(1), 80);
            }
            BeginTransition(*auxiliary, ReadWeatherChannel(3), 80);
            weatherPublishedHour = forecast.hour;
            weatherPublishedMinute = forecast.minute;
        }
        if (weatherInstruction & 0x2000U)
            UpdateSunOrWindRotationVector();
        ApplyPlanetRotForCurrentDateTime();
    }
    UpdateWeatherParticleGroundCameraPosition();
    if (weatherWind)
        UpdateWindWithSpeed(*weatherWind);
    for (WeatherFixcNode *node = g_secondaryWeatherWindList; node; node = node->next)
        UpdateWindWithSpeed(*static_cast<WeatherWind *>(node->payload.pointer));
    ProcessNewWeatherData();
    PerformWeatherRenderPass();
    UpdateWeatherClipCameraPosition();
    Weather_UpdateWCCPClipSet0();
    Weather_UpdateWCCPClipSet1();
    UpdateWeatherInteractionCamera();
    UpdateWeatherParticleGroups();
    SetWeatherParticleDrawingEnabled(IsWorldDrawingEnabled() ? 1 : 0);
}

} // namespace

// @verified - p
void ApplyPlanetRotForCurrentDateTime() {
    DateTime date;
    GetCurrentDateTime(date);
    std::int32_t day = static_cast<std::int8_t>(date.day);
    if (date.month & 1)
        day += 31;
    std::int32_t numerator = date.hour >= 12 ? day : day - 1;
    if (numerator < 0)
        numerator = 0;
    std::int32_t denominator = g_planetPreviousRotationDay;
    if (numerator > g_planetCurrentRotationDay) {
        numerator -= g_planetCurrentRotationDay;
        denominator = g_planetRotationCycleDays - g_planetCurrentRotationDay;
    } else if (numerator > g_planetPreviousRotationDay) {
        numerator -= g_planetPreviousRotationDay;
        denominator = g_planetCurrentRotationDay - g_planetPreviousRotationDay;
    }
    const float phase = (static_cast<float>(numerator) / static_cast<float>(denominator)) * 65536.0f;
    const Vector3i rotation{0, FloatToShort(phase), 0};
    SetPlanetRotByIndex(rotation, 0);
}

void BeginWeatherFloatTransition(WeatherFloat *value, float target, std::int32_t frames) {
    BeginTransition(*value, target, frames);
}

// @verified - p
std::int32_t AdvanceWeatherFloat(WeatherFloat *value) {
    if (value->frame == value->frameCount)
        return 0;
    value->frame = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(value->frame) + 1U);
    if (value->frame == value->frameCount) {
        value->current = value->target;
        return 2;
    }
    value->current = FlushSmall(value->step + value->current);
    return 1;
}

WeatherFloat *GetWeatherPressureLflt() {
    return rain;
}

WeatherFloat *GetWeatherRippleLflt() {
    return rippleCount;
}

// @verified - p
std::int32_t lfltGetFramesRemaining(const WeatherFloat *value) {
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(value->frameCount) - static_cast<std::uint32_t>(value->frame));
}

// @verified - p
bool lfltIsInterpolating(const WeatherFloat *value) {
    return value->frame != value->frameCount;
}

// @verified - p
std::int32_t lfltSetImmediate(WeatherFloat *value, float target) {
    value->current = value->target = target;
    value->step = 0.0f;
    value->frame = value->frameCount = 0;
    return 0;
}

// @verified - p
std::int32_t lrgbSetImmediate(WeatherColour *value, const Vector4 *target) {
    value->current = value->target = {target->x, target->y, target->z, target->w};
    value->step = {};
    value->frame = value->frameCount = 0;
    return 0;
}

// @verified - p
std::int32_t shenClearData(void *start, const void *end) {
    std::byte *cursor = static_cast<std::byte *>(start);
    do {
        std::memset(cursor, 0, 8);
        cursor += 8;
    } while (reinterpret_cast<std::uintptr_t>(cursor) <= reinterpret_cast<std::uintptr_t>(end));
    return 0;
}

std::int32_t SetWeatherLightManager(std::int32_t enabled) {
    const std::int32_t previous = g_weatherSunState.enabled;
    g_weatherSunState.enabled = enabled;
    return previous;
}

void SetWeatherMinimumSunElevation(float elevation) {

    if (elevation >= 35.0f && elevation <= 90.0f) {
        minimumSunElevation = elevation;
    }
}

std::uint64_t WSCN_LoadWeatherData(const char *directory, const char *filename) {
    sceneWeatherData = LoadLegacySceneFile(directory, filename, WEATHER_SCENE_DATA);
    sceneWeatherTables[0].rows = static_cast<const std::byte *>(sceneWeatherData);
    weatherTableMode = 2;
    const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(sceneWeatherData);
    for (std::size_t index = 1; index < sceneWeatherTables.size(); ++index) {
        const std::int64_t offset = static_cast<std::int64_t>(sceneWeatherTables[index - 1].count) * weatherRowSize;
        sceneWeatherTables[index].rows = reinterpret_cast<const std::byte *>(base + static_cast<std::uintptr_t>(offset));
    }
    for (std::size_t index = 0; index < weatherTables.size(); ++index) {
        weatherTables[index].rows = sceneWeatherTables[index].rows;
        weatherTables[index].count = sceneWeatherTables[index].count;
    }
    return 64;
}

// @verified - p
std::int64_t WSCN_DeleteWeatherData() {
    weatherTableMode = 1;
    std::int64_t result = 0;
    for (std::size_t index = 0; index < weatherTables.size(); ++index) {
        weatherTables[index].rows = defaultWeatherTables[index].rows;
        weatherTables[index].count = defaultWeatherTables[index].count;
        result = static_cast<std::uint32_t>(defaultWeatherTables[index].count);
    }
    if (sceneWeatherData) {
        result = HLib::FreeTaskData(sceneWeatherData);
        sceneWeatherData = nullptr;
    }
    return result;
}

// @verified - p
WeatherData *SetupWeatherData(std::int32_t baseYear) {
    WeatherData *const data = static_cast<WeatherData *>(HLib::AllocateTaskData(sizeof(WeatherData), TASK_WDAT));
    data->baseYear = baseYear;
    data->year = 0;
    data->month = data->day = 1;
    const std::array<const std::uint32_t *, 4> nodes{
        g_weatherCloudTimelineNodes, g_weatherRainTimelineNodes, g_weatherSnowTimelineNodes, auxiliaryTimeline.data()};
    const std::array<std::int32_t, 4> counts{g_weatherCloudTimelineNodeCount, g_weatherRainTimelineNodeCount, g_weatherSnowTimelineNodeCount, 2};
    for (std::size_t index = 0; index < nodes.size(); ++index) {
        WeatherTimeline *const timeline = static_cast<WeatherTimeline *>(HLib::AllocateTaskData(sizeof(WeatherTimeline), WEATHER_TIMELINE_TASKS[index]));
        timeline->nodes = timeline->current = nodes[index];
        timeline->nodeCount = counts[index];
        timeline->blend = timeline->intensity = 0.0f;
        data->timelines[index] = timeline;
    }
    weatherShouldUpdateDay = 0;
    data->years = weatherYears.data();
    data->timelineKindCount = 3;
    data->flags = 0;
    return data;
}

// @verified - p
void QueueWeatherExecution(const MapInfoChunk *weatherChunk) {
    weatherRippleLifetimeScale = 1.0f;
    weatherRippleLifetimeStep = 0.050000001f;
    defaultWeatherTables = {{{g_weatherConditions.data(), 11, 0}, {g_weatherConditions.data() + 1056, 10, 0}, {g_weatherConditions.data() + 2016, 10, 0},
        {g_weatherConditions.data() + 2976, 10, 0}}};
    weatherTableMode = 1;
    for (std::size_t index = 0; index < weatherTables.size(); ++index) {
        weatherTables[index].rows = defaultWeatherTables[index].rows;
        weatherTables[index].count = defaultWeatherTables[index].count;
    }
    AllocateICAR(2.5f, 4.0f, 4.0f, 0.0099999998f);
    sceneWeatherData = nullptr;
    pendingOverrides = releaseOverrides = activeOverrides = 0;
    overrideGate = 1;
    overrideValues = {};
    overrides = {};
    reservedWeatherOverrideState = {};
    weatherUpdateEnabled = 1;
    weatherUpdateWind = 1;
    weatherUpdateTime = 0;
    weatherExecutionTask = HLib::EnqueueTaskWithoutParameter(ExecuteWeather_Callback, 12, 8, static_cast<std::int32_t>(TASK_WEXE.name));
    weatherData = SetupWeatherData(86);
    weatherUpdateFog = 1;
    weatherUpdateClouds = 1;
    weatherSkyTransitionFrames = 500;
    weatherAir = nullptr;
    cloud = AllocateWeatherFloat();
    auxiliary = AllocateWeatherFloat();
    weatherWind = nullptr;
    g_secondaryWeatherWindList = nullptr;
    rain = AllocateWeatherFloat();
    rippleCount = AllocateWeatherFloat();
    weatherInstruction = 0x3fffU;
    weatherClipSets = {};
    InitWeibullResources();
    if (!weatherWind)
        weatherWind = AllocateWIND();
    weatherPublishedYear = weatherPublishedMonth = weatherPublishedDay = weatherPublishedHour = weatherPublishedMinute = 0;
    InitializeWeatherParticleResources();
    UpdateWeatherForCurrentTimeImmediate();
    UpdateWeather();
    if (weatherChunk) {
        const std::byte *const chunk = reinterpret_cast<const std::byte *>(weatherChunk);
        std::int32_t count0;
        std::int32_t count1;
        std::memcpy(&count0, chunk + 12, sizeof(count0));
        std::memcpy(&count1, chunk + 16, sizeof(count1));
        const std::byte *entries = chunk + 24;
        if (count0 != 0) {
            Weather_LoadWCCPEntries(entries, count0, 0);
            entries += static_cast<std::int64_t>(count0) * 56;
        }
        if (count1 != 0)
            Weather_LoadWCCPEntries(entries, count1, 1);
    }
    shenLoadSetLensflare();
}

void FreeWAIR() {
    if (!weatherAir)
        return;
    FreeWeatherAir(weatherAir);
    weatherAir = nullptr;
    weatherUpdateWind = 0;
}

void UpdateWeather() {
    if (weatherAir)
        return;
    weatherAir = AllocateWeatherAir(0, weatherTables.data(), 4);
    EnsureWeatherFixcRegistered();
    weatherUpdateWind = 1;
    DateTime time;
    GetCurrentDateTime(time);
    CopyWeatherTime(*weatherAir, time);
    UpdateWeatherTimelines(time.day, time.hour, time.minute);
    (void)lfltSetImmediate(cloud, ReadWeatherChannel(0));
    (void)lfltSetImmediate(auxiliary, ReadWeatherChannel(3));
    SetWeatherRainPressureImmediate(ReadWeatherChannel(1));
    PublishWeatherAir(true);
}

std::int64_t RefreshWeatherForCurrentTime(std::int32_t frames) {
    weatherUpdateTime = 1;
    DateTime time;
    GetCurrentDateTime(time);
    const std::int32_t totalMinutes = frames / 240;
    const std::int32_t hours = totalMinutes / 60;
    time.minute = static_cast<std::int8_t>(static_cast<std::int32_t>(time.minute) + totalMinutes % 60);
    time.hour = static_cast<std::int8_t>(static_cast<std::int32_t>(time.hour) + hours % 24);
    time.day = static_cast<std::int8_t>(static_cast<std::int32_t>(time.day) + hours / 24);
    UpdateWeatherTimelines(time.day, time.hour, time.minute);
    BeginTransition(*cloud, ReadWeatherChannel(0), frames);
    BeginTransition(*auxiliary, ReadWeatherChannel(3), frames);
    const float rainValue = ReadWeatherChannel(1);
    BeginRainPressureTransitionForWeatherEffects(rainValue, frames);
    BeginTransition(*rain, rainValue, frames);
    if (weatherWind) {
        BeginWindPhaseTransition(*weatherWind, GetWeatherPhaseForCurrentDay(*weatherData));
        BeginWindSpeedTransition(*weatherWind, GetWeatherIntensityForCurrentDay(*weatherData) / 30.0f);
    }
    weatherPublishedYear = time.year;
    weatherPublishedMonth = time.month;
    weatherPublishedDay = time.day;
    weatherPublishedHour = time.hour;
    weatherPublishedMinute = time.minute;
    return static_cast<std::uint32_t>(time.minute);
}

void UpdateWeatherForMinute(std::int32_t mode, std::int32_t enable, std::int32_t frames, float value) {
    DateTime time;
    GetCurrentDateTime(time);
    UpdateWeatherTimelines(time.day, time.hour, time.minute);
    if ((mode != 0 && mode != 1) || (mode == 1 && (pendingOverrides & 8U) != 0))
        return;
    const std::size_t channel = static_cast<std::size_t>(mode);
    const std::uint32_t mask = 1U << channel;
    pendingOverrides |= mask;
    WeatherOverride &state = overrides[channel];
    state.frames = frames;
    state.current = ReadWeatherChannel(channel);
    float target = value >= 0.0f ? std::fmin(1.0f, value) : 0.0f;
    if (enable) {
        releaseOverrides |= mask;
        overrideGate = false;
        UpdateWeatherTimelines(time.day, time.hour, time.minute + frames / 80 + 1);
        target = ReadWeatherChannel(channel);
        overrideGate = true;
    } else {
        releaseOverrides &= ~mask;
    }
    state.target = target;
    state.step = frames ? (target - state.current) / static_cast<float>(frames) : 0.0f;
    weatherSkyTransitionFrames = frames;
    AdvanceOverride(channel);
    if (mode == 0) {
        BeginTransition(*cloud, state.current, 0);
    } else {
        BeginWeatherRainTransition(state.current, 0);
    }
}

// @verified - p
std::int64_t ResetWeatherOverrides() {
    activeOverrides = 0;
    overrideGate = true;
    overrideValues = {};
    pendingOverrides = releaseOverrides = 0;
    overrides = {};
    reservedWeatherOverrideState = {};
    return 0;
}

void SetWeatherInstruction(std::uint32_t instruction, bool enabled) {
    if (enabled)
        weatherInstruction |= instruction;
    else
        weatherInstruction &= ~instruction;
}

std::uint32_t GetCurrentWeatherInstruction() {
    return weatherInstruction;
}

void SetWeatherShouldUpdateDay(std::int32_t flag) {
    weatherShouldUpdateDay = flag;
}

float GetWeatherPressure() {
    if (!rain)
        return 0.0f;
    rain->current = FlushSmall(rain->current);
    return rain->current;
}

// @verified - p
float UpdateWeatherAndTime(std::int32_t minutes) {
    DateTime time;
    GetCurrentDateTime(time);
    UpdateWeatherTimelines(time.day, time.hour, time.minute);
    float maximum = ReadWeatherChannel(1);
    const std::int32_t firstStep = 10 - time.minute % 10;
    DateTime increment{.minute = static_cast<std::int8_t>(firstStep)};
    IncrementDateTimeInPlace(time, increment);
    const std::int32_t remaining = minutes - firstStep;
    increment.minute = 10;
    if (remaining / 10 >= 0) {
        std::uint32_t count = static_cast<std::uint32_t>(remaining / 10 + 1);
        do {
            UpdateWeatherTimelines(time.day, time.hour, time.minute);
            maximum = std::fmax(ReadWeatherChannel(1), maximum);
            IncrementDateTimeInPlace(time, increment);
        } while (--count);
    }
    return maximum;
}

float GetWeatherWindVector() {
    if (!cloud)
        return 0.0f;
    cloud->current = FlushSmall(cloud->current);
    return cloud->current;
}

void SetWeatherUpdateEnabledTo(std::int32_t enabled) {
    weatherUpdateEnabled = enabled;
}

// @verified - p
std::uint64_t GetWeatherUpdateEnabled() {
    return static_cast<std::uint32_t>(weatherUpdateEnabled);
}

// @verified - p
std::int32_t GetCurrentWeather() {
    DateTime time;
    GetCurrentDateTime(time);
    UpdateWeatherTimelines(time.day, time.hour, time.minute);
    if (ReadWeatherChannel(1) > 0.0f)
        return 2;
    return ReadWeatherChannel(0) > 0.0f ? 1 : 0;
}

// @verified - p
void SetNewWindPhaseAndSpeed(std::int32_t phase, float speed) {
    WeatherWind *const wind = weatherWind;
    if (!wind)
        return;
    BeginWindPhaseTransition(*wind, phase);
    BeginWindSpeedTransition(*wind, speed);
    SetWindFreezeFlag(wind, 1);
}

// @verified - p
void SyncWindWithWDATWeatherState() {
    WeatherWind *const wind = weatherWind;
    if (!wind)
        return;
    DateTime time;
    GetCurrentDateTime(time);
    UpdateWeatherTimelines(time.day, time.hour, time.minute);
    BeginWindPhaseTransition(*wind, GetWeatherPhaseForCurrentDay(*weatherData));
    BeginWindSpeedTransition(*wind, GetWeatherIntensityForCurrentDay(*weatherData) / 30.0f);
    SetWindFreezeFlag(wind, 0);
}

WeatherWind *GetWind() {
    return weatherWind;
}

// @verified - p
float GetCurrentWindSpeed() {
    return weatherWind ? weatherWind->currentWindSpeed : 0.0f;
}

// @verified - p
void windGetDirectionVector(const WeatherWind *wind, Vector3 *out) {
    *out = wind->windDirection;
}

// @verified - p
void SetWindFreezeFlag(WeatherWind *wind, std::int32_t enabled) {
    wind->holdPreviousOutput = enabled;
}

float GetEffectiveWindSpeed(const WeatherWind *wind) {
    if (debugSampledWindSpeedOverride > 0.0f)
        return debugSampledWindSpeedOverride;
    return wind ? wind->sampledWindSpeed : 0.0f;
}

std::int32_t GetCurrentWindPhaseAccumulator(const WeatherWind *wind) {
    if (debugCustomWindPhaseAccum)
        return static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(debugCustomWindPhaseAccum) * 8192.0f / 45.0f));
    return wind ? wind->outputPhase : 0;
}

Vector4 *GetInterpolatedWeatherValues() {
    return interpolatedWeatherValues.data();
}

std::array<std::uint32_t, 2> GetWeatherWaveColours() {
    // The existing typed wave consumer reads the same borrowed publication, including consumer mutations until the next update.
    return {std::bit_cast<std::uint32_t>(interpolatedWeatherValues[4].z), std::bit_cast<std::uint32_t>(interpolatedWeatherValues[5].x)};
}

} // namespace shenmue::s2
