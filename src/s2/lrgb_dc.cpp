#include "s2/lrgb.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include <cmath>

namespace shenmue {
namespace s2 {

void ZeroWordRange(std::uint32_t *word, std::uint32_t *last);

// @verified - d
WeatherColour *AllocateLRGB() {
    WeatherColour *const value = static_cast<WeatherColour *>(HLib::AllocateTaskData(sizeof(WeatherColour), WEATHER_COLOUR_STORAGE));
    ZeroWordRange(reinterpret_cast<std::uint32_t *>(value), reinterpret_cast<std::uint32_t *>(&value->frameCount));
    return value;
}

// @verified - d
void FreeLRGB(WeatherColour *value) {
    HLib::FreeTaskData(value);
}

// @verified - d
std::int32_t AdvanceWeatherColour(WeatherColour *value) {
    if (value->frame == value->frameCount)
        return 0;
    value->frame = static_cast<std::int32_t>(static_cast<std::uint32_t>(value->frame) + 1U);
    float *const current = value->current;
    if (value->frame == value->frameCount) {
        current[0] = value->target[0];
        current[1] = value->target[1];
        current[2] = value->target[2];
        current[3] = value->target[3];
        return 2;
    }
    float *const step = value->step;
    current[0] += step[0];
    if (std::fabs(current[0]) < 1.1920928955078125e-07f)
        current[0] = 0.0f;
    current[1] += step[1];
    if (std::fabs(current[1]) < 1.1920928955078125e-07f)
        current[1] = 0.0f;
    current[2] += step[2];
    if (std::fabs(current[2]) < 1.1920928955078125e-07f)
        current[2] = 0.0f;
    current[3] += step[3];
    if (std::fabs(current[3]) < 1.1920928955078125e-07f)
        current[3] = 0.0f;
    return 1;
}

// @verified - d
void lrgbSetImmediate(WeatherColour *value, const Vector4 *target) {
    float *const destination = value->target;
    float *const step = value->step;
    value->current[0] = destination[0] = target->x;
    value->current[1] = destination[1] = target->y;
    value->current[2] = destination[2] = target->z;
    value->current[3] = destination[3] = target->w;
    step[0] = step[1] = step[2] = step[3] = 0.0f;
    value->frame = value->frameCount = 0;
}

// @verified - d
void BeginWeatherColourTransition(WeatherColour *value, const Vector4 *target, std::int32_t frames) {
    if (frames < 1) {
        lrgbSetImmediate(value, target);
        return;
    }
    float *const current = value->current;
    float *const destination = value->target;
    float *const step = value->step;
    float channel;
    channel = target->x;
    if (std::fabs(channel) < 1.1920928955078125e-07f)
        channel = 0.0f;
    destination[0] = channel;
    channel = target->y;
    if (std::fabs(channel) < 1.1920928955078125e-07f)
        channel = 0.0f;
    destination[1] = channel;
    channel = target->z;
    if (std::fabs(channel) < 1.1920928955078125e-07f)
        channel = 0.0f;
    destination[2] = channel;
    channel = target->w;
    if (std::fabs(channel) < 1.1920928955078125e-07f)
        channel = 0.0f;
    destination[3] = channel;
    const float duration = static_cast<float>(frames);
    float difference;
    difference = destination[0] - current[0];
    if (std::fabs(difference) < 1.1920928955078125e-07f) {
        step[0] = 0.0f;
    } else {
        difference /= duration;
        if (std::fabs(difference) < 1.1920928955078125e-07f)
            step[0] = 0.0f;
        else
            step[0] = difference;
    }
    difference = destination[1] - current[1];
    if (std::fabs(difference) < 1.1920928955078125e-07f) {
        step[1] = 0.0f;
    } else {
        difference /= duration;
        if (std::fabs(difference) < 1.1920928955078125e-07f)
            step[1] = 0.0f;
        else
            step[1] = difference;
    }
    difference = destination[2] - current[2];
    if (std::fabs(difference) < 1.1920928955078125e-07f) {
        step[2] = 0.0f;
    } else {
        difference /= duration;
        if (std::fabs(difference) < 1.1920928955078125e-07f)
            step[2] = 0.0f;
        else
            step[2] = difference;
    }
    difference = destination[3] - current[3];
    if (std::fabs(difference) < 1.1920928955078125e-07f) {
        step[3] = 0.0f;
    } else {
        difference /= duration;
        if (std::fabs(difference) < 1.1920928955078125e-07f)
            step[3] = 0.0f;
        else
            step[3] = difference;
    }
    value->frame = 0;
    value->frameCount = frames;
}

// @verified - d
Vector4 *GetLRGBCurrent(WeatherColour *value, Vector4 *output) {
    if (std::fabs(value->current[0]) < 1.1920928955078125e-07f)
        value->current[0] = 0.0f;
    output->x = value->current[0];
    if (std::fabs(value->current[1]) < 1.1920928955078125e-07f)
        value->current[1] = 0.0f;
    output->y = value->current[1];
    if (std::fabs(value->current[2]) < 1.1920928955078125e-07f)
        value->current[2] = 0.0f;
    output->z = value->current[2];
    if (std::fabs(value->current[3]) < 1.1920928955078125e-07f)
        value->current[3] = 0.0f;
    output->w = value->current[3];
    return output;
}

Vector4 *GetLRGBTarget(WeatherColour *value, Vector4 *output) {
    float *const target = value->target;
    if (std::fabs(target[0]) < 1.1920928955078125e-07f)
        target[0] = 0.0f;
    output->x = target[0];
    if (std::fabs(target[1]) < 1.1920928955078125e-07f)
        target[1] = 0.0f;
    output->y = target[1];
    if (std::fabs(target[2]) < 1.1920928955078125e-07f)
        target[2] = 0.0f;
    output->z = target[2];
    if (std::fabs(target[3]) < 1.1920928955078125e-07f)
        target[3] = 0.0f;
    output->w = target[3];
    return output;
}

std::int32_t GetLRGBRemainingFrames(WeatherColour *value) {
    return value->frameCount - value->frame;
}

std::int32_t IsLRGBTransitioning(WeatherColour *value) {
    if (value->frame == value->frameCount)
        return 0;
    return 1;
}

// LCH4: current, target and step are interleaved at a 12-byte stride.
struct LCH4Channel {
    float current;
    float target;
    float step;
};
struct LCH4Colour {
    LCH4Channel channels[4];
    std::int32_t frame;
    std::int32_t frameCount;
};
typedef char Lch4Layout[(sizeof(LCH4Colour) == 0x38) ? 1 : -1];

LCH4Colour *AllocateLCH4() {
    LCH4Colour *const value = static_cast<LCH4Colour *>(HLib::AllocateTaskData(sizeof(LCH4Colour), SHEN_NAME(LCH4)));
    ZeroWordRange(reinterpret_cast<std::uint32_t *>(value), reinterpret_cast<std::uint32_t *>(&value->frameCount));
    return value;
}

void FreeLCH4(LCH4Colour *value) {
    HLib::FreeTaskData(value);
}

std::int32_t AdvanceLCH4(LCH4Colour *value) {
    if (value->frame == value->frameCount)
        return 0;
    value->frame = static_cast<std::int32_t>(static_cast<std::uint32_t>(value->frame) + 1U);
    if (value->frame == value->frameCount) {
        value->channels[0].current = value->channels[0].target;
        value->channels[1].current = value->channels[1].target;
        value->channels[2].current = value->channels[2].target;
        value->channels[3].current = value->channels[3].target;
        return 2;
    }
    value->channels[0].current += value->channels[0].step;
    if (std::fabs(value->channels[0].current) < 1.1920928955078125e-07f)
        value->channels[0].current = 0.0f;
    value->channels[1].current += value->channels[1].step;
    if (std::fabs(value->channels[1].current) < 1.1920928955078125e-07f)
        value->channels[1].current = 0.0f;
    value->channels[2].current += value->channels[2].step;
    if (std::fabs(value->channels[2].current) < 1.1920928955078125e-07f)
        value->channels[2].current = 0.0f;
    value->channels[3].current += value->channels[3].step;
    if (std::fabs(value->channels[3].current) < 1.1920928955078125e-07f)
        value->channels[3].current = 0.0f;
    return 1;
}

} // namespace s2
} // namespace shenmue

#endif
