#include "s2/weather_air_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/date_time.hpp"
#include "s2/hlib.hpp"
#include <cmath>

namespace shenmue {
namespace s2 {

void LoadAirTextures(std::int32_t texture, float scale, std::int32_t slot);
std::int32_t IsAirTextureTransitionActive();
void CancelAirTextureTransition();
std::int32_t BeginAirTextureTransition(std::int32_t texture, std::int32_t frames, std::int32_t fadePrevious, float scale);

// @verified - d
WeatherAir *AllocateWeatherAir(std::uint32_t lightIndex, WeatherTable *tables, std::int32_t tableCount) {
    WeatherAir *const air = static_cast<WeatherAir *>(HLib::AllocateTaskData(sizeof(WeatherAir), WEATHER_AIR_STORAGE));
    air->lightIndex = lightIndex;
    air->tables = tables;
    air->tableCount = tableCount;
    air->instruction = 0x3fff;
    DateTime time;
    shenGetCurrentDateTime(time);
    air->month = time.month;
    air->day = time.day;
    const std::int32_t hour = time.hour;
    air->hour = hour;
    const std::int32_t minute = time.minute;
    air->minute = minute;
    const std::int32_t second = time.second;
    air->second = second;
    air->seconds = hour * 3600 + minute * 60 + second;
    air->cloud = air->rain = air->snow = air->auxiliary = 0.0f;
    air->rainState = 0;
    air->cloudState = 0;
    air->mode = -1;
    air->primaryColours[0] = AllocateLRGB();
    air->primaryColours[1] = AllocateLRGB();
    air->primaryColours[2] = AllocateLRGB();
    air->values[0] = AllocateLFLT();
    air->values[1] = AllocateLFLT();
    air->secondaryColours[0] = AllocateLRGB();
    air->secondaryColours[1] = AllocateLRGB();
    air->secondaryColours[2] = AllocateLRGB();
    return air;
}

// @verified - d
void FreeWeatherAir(WeatherAir *air) {
    FreeLRGB(air->primaryColours[0]);
    FreeLRGB(air->primaryColours[1]);
    FreeLRGB(air->primaryColours[2]);
    FreeLFLT(air->values[0]);
    FreeLFLT(air->values[1]);
    FreeLRGB(air->secondaryColours[0]);
    FreeLRGB(air->secondaryColours[1]);
    FreeLRGB(air->secondaryColours[2]);
    HLib::FreeTaskData(air);
}

// @verified - d
void SetWeatherAirTable(WeatherAir *air, std::int32_t index, const std::byte *rows, std::int32_t count) {
    WeatherTable *tables = air->tables;
    tables[index].rows = rows;
    tables[index].count = count;
}

// @verified - d
const std::byte *GetWeatherAirTableData(const WeatherAir *air, std::int32_t index) {
    return air->tables[index].rows;
}

// @verified - d
std::int32_t GetWeatherAirTableCount(const WeatherAir *air, std::int32_t index) {
    return air->tables[index].count;
}

// @verified - d
void EnableWeatherAirInstructions(WeatherAir *air, std::uint32_t flags) {
    if (air)
        air->instruction |= flags;
}

// @verified - d
void DisableWeatherAirInstructions(WeatherAir *air, std::uint32_t flags) {
    if (air)
        air->instruction &= ~flags;
}

// @verified - d
std::uint32_t GetWeatherAirInstructions(const WeatherAir *air) {
    if (!air)
        return 0;
    return air->instruction;
}

// @verified - d
void SetWeatherAirTime(WeatherAir *air, std::int32_t month, std::int32_t day, std::int32_t hour, std::int32_t minute, std::int32_t second) {
    air->month = month;
    air->day = day;
    air->hour = hour;
    air->minute = minute;
    air->second = second;
    air->seconds = static_cast<std::int32_t>(static_cast<std::uint32_t>(hour) * 3600U + static_cast<std::uint32_t>(minute) * 60U + second);
}

// @verified - d
void SetWeatherAirCloud(WeatherAir *air, float cloud) {
    air->cloud = cloud;
}

// @verified - d
void SetWeatherAirRain(WeatherAir *air, float rain) {
    air->rain = rain;
}

// @verified - d
void SetWeatherAirSnow(WeatherAir *air, float snow) {
    air->snow = snow;
}

// @verified - d
void SetWeatherAirAuxiliary(WeatherAir *air, float auxiliary) {
    air->auxiliary = auxiliary;
}

Vector4 *UnpackWeatherColour(std::uint32_t packed, Vector4 *colour) {
    colour->x = static_cast<float>(packed >> 24);
    colour->y = static_cast<float>((packed >> 16) & 0xffU);
    colour->z = static_cast<float>((packed >> 8) & 0xffU);
    colour->w = static_cast<float>(packed & 0xffU);
    return colour;
}

// @verified - d
std::uint32_t PackWeatherColour(const Vector4 *colour) {
    return static_cast<std::uint32_t>(static_cast<std::int32_t>(colour->x)) << 24 | static_cast<std::uint32_t>(static_cast<std::int32_t>(colour->y)) << 16 |
        static_cast<std::uint32_t>(static_cast<std::int32_t>(colour->z)) << 8 | static_cast<std::uint32_t>(static_cast<std::int32_t>(colour->w));
}

// @verified - d
std::uint32_t wairPackRainColorRGBA(WeatherAir *air) {
    Vector4 colour;
    return PackWeatherColour(GetLRGBCurrent(air->primaryColours[0], &colour));
}

// @verified - d
std::uint32_t wairPackSecondaryColorRGBA(WeatherAir *air) {
    Vector4 colour;
    return PackWeatherColour(GetLRGBCurrent(air->primaryColours[1], &colour));
}

// @verified - d
std::uint32_t wairPackRippleColorRGBA(WeatherAir *air) {
    Vector4 colour;
    return PackWeatherColour(GetLRGBCurrent(air->primaryColours[2], &colour));
}

// @verified - d
std::int32_t GetAirTextureForTime(std::int32_t hour, std::int32_t minute) {
    std::int32_t time = hour * 60;
    time += minute;
    if (time < 330)
        return 4;
    if (time <= 629)
        return 0;
    if (time <= 899)
        return 1;
    if (time <= 959)
        return 2;
    if (time <= 1079)
        return 3;
    return 4;
}

// @verified - d
void UpdateClouds(WeatherAir *air) {
    if (!(air->rain < 0.1f) || !(air->snow < 0.1f))
        air->rainState = 1;
    else
        air->rainState = 0;
    if (air->cloud > 0.25f)
        air->cloudState = 1;
    else
        air->cloudState = 0;
    std::int32_t mode = 5;
    if (air->rainState == 0) {
        mode = 6;
        if (air->cloudState == 0)
            mode = GetAirTextureForTime(air->hour, air->minute);
    }
    LoadAirTextures(weatherAirTextureIds[mode], weatherAirTextureScales[mode], 0);
    air->mode = mode;
}

// @verified - d
void UpdateWeatherClouds(WeatherAir *air, std::int32_t frames) {
    if (air->rainState == 0 && (!(air->rain < 0.1f) || !(air->snow < 0.1f)))
        air->rainState = 1;
    else if (air->rainState == 1 && air->rain < 0.05f && air->snow < 0.05f)
        air->rainState = 0;
    if (air->cloudState == 0 && !(air->cloud < 0.25f))
        air->cloudState = 1;
    else if (air->cloudState == 1 && air->cloud < 0.2f)
        air->cloudState = 0;
    std::int32_t mode = 5;
    if (air->rainState == 0) {
        mode = 6;
        if (air->cloudState == 0)
            mode = GetAirTextureForTime(air->hour, air->minute);
    }
    if (air->mode == mode)
        return;
    if (IsAirTextureTransitionActive())
        CancelAirTextureTransition();
    DateTime time;
    shenGetCurrentDateTime(time);
    if (frames == 500) {
        if (static_cast<std::uint8_t>(time.hour - 5) > 12 && air->mode < 5 && static_cast<std::uint32_t>(mode - 5) < 2U)
            frames = 800;
    } else if (static_cast<std::uint32_t>(mode - 5) > 1U && static_cast<std::uint32_t>(air->mode - 5) > 1U) {
        frames = 500;
    }
    const std::int32_t fadePrevious = weatherAirTextureIds[air->mode] == 2;
    if (BeginAirTextureTransition(weatherAirTextureIds[mode], frames, fadePrevious, weatherAirTextureScales[mode]))
        air->mode = mode;
}

namespace {

inline float WeightWeatherValue(float sample, float weight) {
    float value = weight * sample;
    if (std::fabs(value) < 1.1920928955078125e-07f)
        value = 0.0f;
    return value;
}

} // namespace

#define AccumulateWeatherFloat(output, sample, weight)                                                                                                         \
    {                                                                                                                                                          \
        (output) += WeightWeatherValue((sample), (weight));                                                                                                    \
        if (std::fabs(output) < 1.1920928955078125e-07f)                                                                                                       \
            (output) = 0.0f;                                                                                                                                   \
    }
#define AccumulateWeatherColour(output, sample, weight) ((output) = AddWeatherColours((output), InterpolateWeatherColour(0, (sample), (weight))))

inline void AccumulateWeatherVector3(Vector3 *output, const Vector3 *sample, float weight) {
    AccumulateWeatherFloat(output->x, sample->x, weight);
    AccumulateWeatherFloat(output->y, sample->y, weight);
    AccumulateWeatherFloat(output->z, sample->z, weight);
}

inline void AccumulateWeatherVector4(Vector4 *output, const Vector4 *sample, float weight) {
    AccumulateWeatherFloat(output->x, sample->x, weight);
    AccumulateWeatherFloat(output->y, sample->y, weight);
    AccumulateWeatherFloat(output->z, sample->z, weight);
    AccumulateWeatherFloat(output->w, sample->w, weight);
}

void ClearWeatherSamplePrimary(WeatherSamplePrimary *block) {
    block->values[0] = block->values[1] = block->values[2] = 0.0f;
    block->values[3] = block->values[4] = block->values[5] = block->values[6] = 0.0f;
    block->values[7] = block->values[8] = 0.0f;
    block->colours[0] = block->colours[1] = block->colours[2] = 0;
}

void AccumulateWeatherSamplePrimary(WeatherSamplePrimary *output, const WeatherSamplePrimary *sample, float weight) {
    Vector3 *outputVector = reinterpret_cast<Vector3 *>(output->values);
    const Vector3 *sampleVector = reinterpret_cast<const Vector3 *>(sample->values);
    AccumulateWeatherVector3(outputVector, sampleVector, weight);
    AccumulateWeatherVector4(reinterpret_cast<Vector4 *>(output->values + 3), reinterpret_cast<const Vector4 *>(sample->values + 3), weight);
    AccumulateWeatherFloat(output->values[7], sample->values[7], weight);
    AccumulateWeatherFloat(output->values[8], sample->values[8], weight);
    AccumulateWeatherColour(output->colours[0], sample->colours[0], weight);
    AccumulateWeatherColour(output->colours[1], sample->colours[1], weight);
    AccumulateWeatherColour(output->colours[2], sample->colours[2], weight);
}

void InterpolateWeatherSamplePrimary(const WeatherSamplePrimary *first, const WeatherSamplePrimary *second, float blend, WeatherSamplePrimary *output) {
    InterpolateWeatherVector3(reinterpret_cast<const Vector3 *>(first->values), reinterpret_cast<const Vector3 *>(second->values),
        reinterpret_cast<Vector3 *>(output->values), blend);
    InterpolateWeatherVector4(reinterpret_cast<const Vector4 *>(first->values + 3), reinterpret_cast<const Vector4 *>(second->values + 3),
        reinterpret_cast<Vector4 *>(output->values + 3), blend);
    output->values[7] = InterpolateWeatherValue(first->values[7], second->values[7], blend);
    output->values[8] = InterpolateWeatherValue(first->values[8], second->values[8], blend);
    output->colours[0] = InterpolateWeatherColour(first->colours[0], second->colours[0], blend);
    output->colours[1] = InterpolateWeatherColour(first->colours[1], second->colours[1], blend);
    output->colours[2] = InterpolateWeatherColour(first->colours[2], second->colours[2], blend);
}

void ClearWeatherSampleEnvironment(WeatherSampleEnvironment *block) {
    block->values[0] = block->values[1] = block->values[2] = 0.0f;
    block->colours[0] = block->colours[1] = block->colours[2] = block->colours[3] = block->colours[4] = 0;
}

void AccumulateWeatherSampleEnvironment(WeatherSampleEnvironment *output, const WeatherSampleEnvironment *sample, float weight) {
    AccumulateWeatherFloat(output->values[0], sample->values[0], weight);
    AccumulateWeatherFloat(output->values[1], sample->values[1], weight);
    AccumulateWeatherFloat(output->values[2], sample->values[2], weight);
    AccumulateWeatherColour(output->colours[0], sample->colours[0], weight);
    AccumulateWeatherColour(output->colours[1], sample->colours[1], weight);
    AccumulateWeatherColour(output->colours[2], sample->colours[2], weight);
    AccumulateWeatherColour(output->colours[3], sample->colours[3], weight);
    AccumulateWeatherColour(output->colours[4], sample->colours[4], weight);
}

void InterpolateWeatherSampleEnvironment(
    const WeatherSampleEnvironment *first, const WeatherSampleEnvironment *second, float blend, WeatherSampleEnvironment *output) {
    output->values[0] = InterpolateWeatherValue(first->values[0], second->values[0], blend);
    output->values[1] = InterpolateWeatherValue(first->values[1], second->values[1], blend);
    output->values[2] = InterpolateWeatherValue(first->values[2], second->values[2], blend);
    output->colours[0] = InterpolateWeatherColour(first->colours[0], second->colours[0], blend);
    output->colours[1] = InterpolateWeatherColour(first->colours[1], second->colours[1], blend);
    output->colours[2] = InterpolateWeatherColour(first->colours[2], second->colours[2], blend);
    output->colours[3] = InterpolateWeatherColour(first->colours[3], second->colours[3], blend);
    output->colours[4] = InterpolateWeatherColour(first->colours[4], second->colours[4], blend);
}

} // namespace s2
} // namespace shenmue

#endif
