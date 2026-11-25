#include "s2/weather_wind_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/sdk.hpp"
#include "s2/hlib.hpp"
#include <cmath>

namespace shenmue {
namespace s2 {

float EvaluateWeibullDensity(float shape, float scale, float value);

float SampleWindImpulse();

float *windProfileSamples = 0;

float *windMappingTable = 0;

float *windImpulseLut = 0;

float totalWindProfileWeight = 0.0f;

std::int32_t windImpulseLutIndex = 0;

void ZeroWordRange(std::uint32_t *word, std::uint32_t *last) {
    do
        *word++ = 0;
    while (reinterpret_cast<std::int32_t>(word) <= reinterpret_cast<std::int32_t>(last));
}

// @verified - d
void GenerateWeibullProfile(float *samples, std::uint32_t count, float shape, float scale) {
    float total = static_cast<float>(count);
    for (float sample = 0.0f; total > sample; sample += 1.0f) {
        float value = 0.0f;
        if (!(std::fabs(scale) < 1.1920928955078125e-07f)) {
            value = sample / total;
            if (std::fabs(value) < 1.1920928955078125e-07f) {
                value = 0.0f;
            } else {
                value *= scale;
                if (std::fabs(value) < 1.1920928955078125e-07f)
                    value = 0.0f;
            }
        }
        *samples++ = EvaluateWeibullDensity(shape, 1.0f, value);
    }
}

// @verified - d
float EvaluateWeibullDensity(float shape, float scale, float value) {
    if (std::fabs(shape) < 1.1920928955078125e-07f || std::fabs(scale) < 1.1920928955078125e-07f || std::fabs(value) < 1.1920928955078125e-07f)
        return 0.0f;
    float scaled = value / scale;
    if (std::fabs(scaled) < 1.1920928955078125e-07f)
        return 0.0f;
    float term = shape - 1.0f;
    if (std::fabs(term) < 1.1920928955078125e-07f)
        return 0.0f;
    const float power = fast_powf_approx(scaled, term);
    if (std::fabs(power) < 1.1920928955078125e-07f)
        return 0.0f;
    term = shape / scale;
    if (std::fabs(term) < 1.1920928955078125e-07f)
        return 0.0f;
    term *= power;
    if (std::fabs(term) < 1.1920928955078125e-07f)
        return 0.0f;
    scaled *= -power;
    if (std::fabs(scaled) < 1.1920928955078125e-07f)
        return 0.0f;
    scaled = fast_expf_approx(scaled);
    if (std::fabs(scaled) < 1.1920928955078125e-07f)
        return 0.0f;
    return term * scaled;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AllocateTMP0_TMP1(float *output, std::uint32_t count, std::uint32_t taps) {
    float *const coefficients = static_cast<float *>(HLib::AllocateTaskData(4U * taps, WEATHER_NOISE_COEFFICIENT_STORAGE));
    float *const history = static_cast<float *>(HLib::AllocateTaskData(4U * taps, WEATHER_NOISE_HISTORY_STORAGE));
    std::uint32_t i, j;
    coefficients[0] = 1.0f;
    float sum = 1.0f;
    float value = sum;
    for (i = 1; i < taps; ++i) {
        coefficients[i] = coefficients[i - 1] * ((value + value) - 1.0f) / (value + value);
        sum += coefficients[i];
        value += 1.0f;
    }
    for (i = 0; i < taps; ++i)
        history[i] = dc::sdk::RandomFloat();
    for (i = 0; i < count; ++i) {
        value = 0.0f;
        for (j = 0; j < taps; ++j)
            value += coefficients[j] * history[j];
        *output++ = value / sum;
        for (j = 0; j < taps - 1U; ++j)
            history[j] = history[j + 1];
        history[j] = dc::sdk::RandomFloat();
    }
    HLib::FreeTaskData(history);
    HLib::FreeTaskData(coefficients);
}
#else
// @verified - d
void AllocateTMP0_TMP1(float *output, std::uint32_t count, std::uint32_t taps) {
    float *const coefficients = static_cast<float *>(HLib::AllocateTaskData(4U * taps, WEATHER_NOISE_COEFFICIENT_STORAGE));
    float *const history = static_cast<float *>(HLib::AllocateTaskData(4U * taps, WEATHER_NOISE_HISTORY_STORAGE));
    coefficients[0] = 1.0f;
    float sum = 1.0f;
    float position = 1.0f;
    for (std::uint32_t index = 1; index < taps; ++index) {
        const float twice = position + position;
        coefficients[index] = ((twice - 1.0f) * coefficients[index - 1]) / twice;
        sum += coefficients[index];
        position += 1.0f;
    }
    for (std::uint32_t index = 0; index < taps; ++index)
        history[index] = dc::sdk::RandomFloat();
    for (std::uint32_t sample = 0; sample < count; ++sample) {
        float value = 0.0f;
        for (std::uint32_t index = 0; index < taps; ++index)
            asm volatile("fmov %2,fr0\n\tfmac fr0,%1,%0" : "+&f"(value) : "f"(history[index]), "f"(coefficients[index]) : "fr0");
        *output++ = value / sum;
        std::uint32_t index = 0;
        while (index < taps - 1U) {
            history[index] = history[index + 1];
            ++index;
        }
        history[index] = dc::sdk::RandomFloat();
    }
    HLib::FreeTaskData(history);
    HLib::FreeTaskData(coefficients);
}
#endif

// @verified - d
float SampleWindFromProfile(float baseSpeed, const float *profile) {
    if (!(baseSpeed > 0.0f))
        return 0.0f;
    float impulse = SampleWindImpulse() - 0.35f;
    if (impulse < 0.0f)
        impulse = 0.0f;
    impulse += impulse;
    if (impulse > 1.0f)
        impulse = 1.0f;
    const float range = totalWindProfileWeight + 1.0f;
    const float threshold = impulse * range;
    std::int32_t band = 0;
    float cumulative = 0.0f + profile[0];
    while (!(cumulative > threshold)) {
        if (++band > 47)
            break;
        cumulative += profile[band];
    }
    if (band < 48)
        return static_cast<float>(band - 1) * baseSpeed * 0.1f;
    const float overflow = (((threshold - totalWindProfileWeight) / (range - totalWindProfileWeight)) * 80.0f) * baseSpeed * 0.1f;
    return 0.6666667f > overflow ? overflow : 0.6666667f;
}

} // namespace s2
} // namespace shenmue

#endif
