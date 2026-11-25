#include "platform/dc/particle_effects.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#include <cmath>
#include "common/native_sqrt.hpp"

namespace shenmue {
namespace dc {
namespace sdk {

// @verified - d
void SetFftEffectActive(FftEffectHandle handle, std::int32_t active) {
    FftEffect &effect =
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
        *handle;
#else
        g_fftEffects[handle];
#endif
    std::uint32_t &flags = effect.flags;
    if (active)
        flags |= 0x80000000U;
    else
        flags &= ~0x80000000U;
}

#if !SHENMUE_IS_S2 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeFftPhaseSteps(float *steps, float scale) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const float kFftPhasePi = 3.1415927410125732f;
#endif
    for (std::int32_t row = 0; row < 8; ++row) {
        for (std::int32_t column = 0; column < 8; ++column) {
            steps[row * 16 + column] = scale * RandomFloat();
            steps[row * 16 + 16 - column] = scale * RandomFloat();
#if SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            float &first = steps[row * 16 + column];
            float &second = steps[row * 16 + 16 - column];
            if (first < kFftDenormalEpsilon && first > -kFftDenormalEpsilon)
                first = 0.0f;
            if (second < kFftDenormalEpsilon && second > -kFftDenormalEpsilon)
                second = 0.0f;
            if (first > kFftPhasePi || first < -kFftPhasePi)
                asm volatile("trapa #0x42");
            if (second > kFftPhasePi || second < -kFftPhasePi)
                asm volatile("trapa #0x42");
#endif
        }
    }
}

// @verified - d
float *BuildFftFalloff(float exponent, float *table) {
    float *const falloff = table;
    std::int32_t row, column;
#if SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    falloff[0] = kFftDenormalEpsilon;
#endif
    for (row = 1; row < 8; ++row) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        float value = 100.0f / fast_powf_approx(static_cast<float>(row), exponent * 0.5f);
#else
        float value = 100.0f / std::pow(static_cast<float>(row), exponent * 0.5f);
#endif
#if SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        if (value < kFftDenormalEpsilon && value > -kFftDenormalEpsilon)
            value = kFftDenormalEpsilon;
#endif
        falloff[row * 8] = value;
    }
    for (column = 1; column < 8; ++column) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        float value = 100.0f / fast_powf_approx(static_cast<float>(column), exponent * 0.5f);
#else
        float value = 100.0f / std::pow(static_cast<float>(column), exponent * 0.5f);
#endif
#if SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        if (value < kFftDenormalEpsilon && value > -kFftDenormalEpsilon)
            value = kFftDenormalEpsilon;
#endif
        falloff[column] = value;
    }
    for (row = 1; row < 8; ++row) {
        for (column = 1; column < 8; ++column) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            float value = 25.0f / fast_powf_approx(NativeSqrt(static_cast<float>(row * row + column * column)), exponent * 0.5f);
#else
            float value = 25.0f / std::pow(NativeSqrt(static_cast<float>(row * row + column * column)), exponent * 0.5f);
#endif
#if SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            if (value < kFftDenormalEpsilon && value > -kFftDenormalEpsilon)
                value = kFftDenormalEpsilon;
#endif
            (falloff + row * 8)[column] = value;
        }
    }
    return table;
}

// @verified - d
float *ScaleFftFalloff(float scale, float *table) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float *entry = table;
    for (std::int32_t index = 0; index < 64; ++index, ++entry) {
        float value = *entry * scale;
        if (value < kFftDenormalEpsilon && value > -kFftDenormalEpsilon)
            value = kFftDenormalEpsilon;
        *entry = value;
    }
    return table;
#else
    for (std::int32_t index = 0; index < 64; ++index) {
        table[index] *= scale;
#if SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        if (table[index] < kFftDenormalEpsilon && table[index] > -kFftDenormalEpsilon)
            table[index] = kFftDenormalEpsilon;
#endif
    }
    return table;
#endif
}

// @verified - d
void SetFftEffectFalloff(FftEffectHandle handle, const float *table) {
    FftEffect &effect =
#if SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        *handle;
#else
        g_fftEffects[handle];
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float *amplitude = effect.amplitudes;
    for (std::int32_t row = 0; row < 8; ++row) {
        for (std::int32_t column = 0; column < 8; ++column)
            *amplitude++ = *table++;
    }
#else
    for (std::int32_t entry = 0; entry < 64; ++entry)
        effect.amplitudes[entry] = table[entry];
#endif
}

// @verified - d
void SetFftEffectPhaseScale(FftEffectHandle handle, float scale) {
    FftEffect &effect =
#if SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        *handle;
#else
        g_fftEffects[handle];
#endif
    effect.phaseStepScale = scale;
    InitializeFftPhaseSteps(effect.phaseSteps, scale);
}

#if !SHENMUE_IS_S2 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetFftEffectPixels(FftEffectHandle handle, std::uint16_t *pixels) {
    FftEffect &effect =
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
        *handle;
#else
        g_fftEffects[handle];
#endif
    effect.pixels = pixels;
}

// @verified - d
void SetFftEffectColour(FftEffectHandle handle, std::int16_t colour) {
    FftEffect &effect =
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
        *handle;
#else
        g_fftEffects[handle];
#endif
    effect.colour = colour;
}

#endif

#endif

} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif
