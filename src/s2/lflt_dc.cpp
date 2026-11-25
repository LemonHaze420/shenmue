#include "s2/lflt.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cmath>
#include <cstring>

namespace shenmue {
namespace s2 {

// Packed-colour transition with interleaved channels.
struct WeatherPackedChannel {
    float current;
    float target;
    float step;
};

struct WeatherPackedColour {
    WeatherPackedChannel channels[4];
    std::int32_t frame;
    std::int32_t frameCount;
};
typedef char WeatherPackedColourLayout[(sizeof(WeatherPackedColour) == 0x38) ? 1 : -1];

static const float kWeatherEpsilon = 1.1920928955078125e-07f;

void ResetWeatherPackedColour(WeatherPackedColour *colour) {
    colour->channels[0].step = colour->channels[1].step = colour->channels[2].step = colour->channels[3].step = 0.0f;
    colour->frame = colour->frameCount = 0;
}

void BeginWeatherPackedColourTransition(WeatherPackedColour *colour, std::uint32_t argb, std::int32_t frames) {
    if (frames < 1) {
        ResetWeatherPackedColour(colour);
        return;
    }
    colour->channels[0].target = static_cast<float>(static_cast<std::int32_t>(argb >> 24));
    colour->channels[1].target = static_cast<float>((static_cast<std::int32_t>(argb) >> 16) & 0xff);
    colour->channels[2].target = static_cast<float>((static_cast<std::int32_t>(argb) >> 8) & 0xff);
    colour->channels[3].target = static_cast<float>(static_cast<std::int32_t>(argb & 0xff));
    const float count = static_cast<float>(frames);
    float difference;
    difference = colour->channels[0].target - colour->channels[0].current;
    if (std::fabs(difference) < kWeatherEpsilon)
        colour->channels[0].step = 0.0f;
    else {
        difference /= count;
        if (std::fabs(difference) < kWeatherEpsilon)
            colour->channels[0].step = 0.0f;
        else
            colour->channels[0].step = difference;
    }
    difference = colour->channels[1].target - colour->channels[1].current;
    if (std::fabs(difference) < kWeatherEpsilon)
        colour->channels[1].step = 0.0f;
    else {
        difference /= count;
        if (std::fabs(difference) < kWeatherEpsilon)
            colour->channels[1].step = 0.0f;
        else
            colour->channels[1].step = difference;
    }
    difference = colour->channels[2].target - colour->channels[2].current;
    if (std::fabs(difference) < kWeatherEpsilon)
        colour->channels[2].step = 0.0f;
    else {
        difference /= count;
        if (std::fabs(difference) < kWeatherEpsilon)
            colour->channels[2].step = 0.0f;
        else
            colour->channels[2].step = difference;
    }
    difference = colour->channels[1].target - colour->channels[1].current;
    if (std::fabs(difference) < kWeatherEpsilon)
        colour->channels[1].step = 0.0f;
    else {
        difference /= count;
        if (std::fabs(difference) < kWeatherEpsilon)
            colour->channels[1].step = 0.0f;
        else
            colour->channels[1].step = difference;
    }
    difference = colour->channels[3].target - colour->channels[3].current;
    if (std::fabs(difference) < kWeatherEpsilon)
        colour->channels[3].step = 0.0f;
    else {
        difference /= count;
        if (std::fabs(difference) < kWeatherEpsilon)
            colour->channels[3].step = 0.0f;
        else
            colour->channels[3].step = difference;
    }
    colour->frame = 0;
    colour->frameCount = frames;
}

std::uint32_t GetWeatherPackedColourCurrent(WeatherPackedColour *colour) {
    if (std::fabs(colour->channels[0].current) < kWeatherEpsilon)
        colour->channels[0].current = 0.0f;
    const std::int32_t alpha = static_cast<std::int32_t>(colour->channels[0].current);
    if (std::fabs(colour->channels[1].current) < kWeatherEpsilon)
        colour->channels[1].current = 0.0f;
    const std::int32_t red = static_cast<std::int32_t>(colour->channels[1].current);
    if (std::fabs(colour->channels[2].current) < kWeatherEpsilon)
        colour->channels[2].current = 0.0f;
    const std::int32_t green = static_cast<std::int32_t>(colour->channels[2].current);
    if (std::fabs(colour->channels[3].current) < kWeatherEpsilon)
        colour->channels[3].current = 0.0f;
    const std::int32_t blue = static_cast<std::int32_t>(colour->channels[3].current);
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

std::uint32_t GetWeatherPackedColourTarget(WeatherPackedColour *colour) {
    if (std::fabs(colour->channels[0].target) < kWeatherEpsilon)
        colour->channels[0].target = 0.0f;
    const std::int32_t alpha = static_cast<std::int32_t>(colour->channels[0].target);
    if (std::fabs(colour->channels[1].target) < kWeatherEpsilon)
        colour->channels[1].target = 0.0f;
    const std::int32_t red = static_cast<std::int32_t>(colour->channels[1].target);
    if (std::fabs(colour->channels[2].target) < kWeatherEpsilon)
        colour->channels[2].target = 0.0f;
    const std::int32_t green = static_cast<std::int32_t>(colour->channels[2].target);
    if (std::fabs(colour->channels[3].target) < kWeatherEpsilon)
        colour->channels[3].target = 0.0f;
    const std::int32_t blue = static_cast<std::int32_t>(colour->channels[3].target);
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

std::int32_t GetWeatherPackedColourFramesRemaining(const WeatherPackedColour *colour) {
    return colour->frameCount - colour->frame;
}

bool IsWeatherPackedColourInterpolating(const WeatherPackedColour *colour) {
    if (colour->frame == colour->frameCount)
        return 0;
    return 1;
}

void ZeroWordRange(std::uint32_t *word, std::uint32_t *last);

// @verified - d
WeatherFloat *AllocateLFLT() {
    WeatherFloat *const value = static_cast<WeatherFloat *>(HLib::AllocateTaskData(sizeof(WeatherFloat), WEATHER_FLOAT_STORAGE));
    ZeroWordRange(reinterpret_cast<std::uint32_t *>(value), reinterpret_cast<std::uint32_t *>(value) + 4);
    return value;
}

// @verified - d
void FreeLFLT(WeatherFloat *value) {
    HLib::FreeTaskData(value);
}

// @verified - d
void lfltSetImmediate(WeatherFloat *value, float target) {
    value->target = target;
    value->current = target;
    value->step = 0.0f;
    value->frameCount = 0;
    value->frame = 0;
}

// @verified - d
void BeginWeatherFloatTransition(WeatherFloat *value, float target, std::int32_t frames) {
    if (frames < 1) {
        lfltSetImmediate(value, target);
        return;
    }
    if (std::fabs(target) < 1.1920928955078125e-07f)
        target = 0.0f;
    value->target = target;
    const float difference = target - value->current;
    if (std::fabs(difference) < 1.1920928955078125e-07f)
        value->step = 0.0f;
    else {
        const float step = difference / static_cast<float>(frames);
        if (std::fabs(step) < 1.1920928955078125e-07f)
            value->step = 0.0f;
        else
            value->step = step;
    }
    value->frame = 0;
    value->frameCount = frames;
}

// @verified - d
std::int32_t AdvanceWeatherFloat(WeatherFloat *value) {
    if (value->frame == value->frameCount)
        return 0;
    value->frame = static_cast<std::int32_t>(static_cast<std::uint32_t>(value->frame) + 1U);
    if (value->frame == value->frameCount) {
        value->current = value->target;
        return 2;
    }
    value->current += value->step;
    if (std::fabs(value->current) < 1.1920928955078125e-07f)
        value->current = 0.0f;
    return 1;
}

// @verified - d
float GetLFLTCurrent(WeatherFloat *value) {
    if (std::fabs(value->current) < 1.1920928955078125e-07f)
        value->current = 0.0f;
    return value->current;
}

// @verified - d
float GetLFLTTarget(WeatherFloat *value) {
    if (std::fabs(value->target) < 1.1920928955078125e-07f)
        value->target = 0.0f;
    return value->target;
}

// @verified - d
std::int32_t lfltGetFramesRemaining(const WeatherFloat *value) {
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(value->frameCount) - static_cast<std::uint32_t>(value->frame));
}

// @verified - d
bool lfltIsInterpolating(const WeatherFloat *value) {
    if (value->frame == value->frameCount)
        return 0;
    return 1;
}

std::int32_t InterpolateWeatherInteger(std::int32_t first, std::int32_t second, float blend) {
    float difference = static_cast<float>(second - first);
    if (std::fabs(difference) < kWeatherEpsilon) {
        difference = 0.0f;
    } else {
        difference *= blend;
        if (std::fabs(difference) < kWeatherEpsilon)
            difference = 0.0f;
    }
    return static_cast<std::int32_t>(difference) + first;
}

Vector3 *InterpolateWeatherVector3(const Vector3 *first, const Vector3 *second, Vector3 *output, float blend) {
    float difference;
    difference = second->x - first->x;
    if (std::fabs(difference) < kWeatherEpsilon)
        difference = 0.0f;
    else {
        difference *= blend;
        if (std::fabs(difference) < kWeatherEpsilon)
            difference = 0.0f;
    }
    output->x = first->x + difference;
    difference = second->y - first->y;
    if (std::fabs(difference) < kWeatherEpsilon)
        difference = 0.0f;
    else {
        difference *= blend;
        if (std::fabs(difference) < kWeatherEpsilon)
            difference = 0.0f;
    }
    output->y = first->y + difference;
    difference = second->z - first->z;
    if (std::fabs(difference) < kWeatherEpsilon)
        difference = 0.0f;
    else {
        difference *= blend;
        if (std::fabs(difference) < kWeatherEpsilon)
            difference = 0.0f;
    }
    output->z = first->z + difference;
    return output;
}

Vector4 *InterpolateWeatherVector4(const Vector4 *first, const Vector4 *second, Vector4 *output, float blend) {
    float difference;
    difference = second->x - first->x;
    if (std::fabs(difference) < kWeatherEpsilon || std::fabs(blend) < kWeatherEpsilon)
        difference = 0.0f;
    else {
        difference *= blend;
        if (std::fabs(difference) < kWeatherEpsilon)
            difference = 0.0f;
    }
    output->x = first->x + difference;
    difference = second->y - first->y;
    if (std::fabs(difference) < kWeatherEpsilon || std::fabs(blend) < kWeatherEpsilon)
        difference = 0.0f;
    else {
        difference *= blend;
        if (std::fabs(difference) < kWeatherEpsilon)
            difference = 0.0f;
    }
    output->y = first->y + difference;
    difference = second->z - first->z;
    if (std::fabs(difference) < kWeatherEpsilon || std::fabs(blend) < kWeatherEpsilon)
        difference = 0.0f;
    else {
        difference *= blend;
        if (std::fabs(difference) < kWeatherEpsilon)
            difference = 0.0f;
    }
    output->z = first->z + difference;
    difference = second->w - first->w;
    if (std::fabs(difference) < kWeatherEpsilon || std::fabs(blend) < kWeatherEpsilon)
        difference = 0.0f;
    else {
        difference *= blend;
        if (std::fabs(difference) < kWeatherEpsilon)
            difference = 0.0f;
    }
    output->w = first->w + difference;
    return output;
}

} // namespace s2
} // namespace shenmue

#endif
