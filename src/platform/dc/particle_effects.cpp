#include "platform/dc/particle_effects.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#include <cmath>

namespace shenmue {
namespace dc {
namespace sdk {

#if !SHENMUE_IS_S2
TerrainEffect g_terrainEffects[2]{};
TerrainChannel g_terrainChannels[4]{};
Vector2 g_fftForwardTwiddles[32]{};
Vector2 g_fftBackwardTwiddles[32]{};

#endif

// @verified - d
void FftInverse16(Vector2 *values) {
    FftReverse16(values);
    FftRadix4(values, 4, g_fftBackwardTwiddles);
    FftInverseHalves(values, 4);
    FftInverseScale(values, 4, &kFftInverse16Scale);
}

namespace {

// @verified - d
std::int32_t NextFftReversedIndex(std::int32_t index, std::int32_t step) {
    while (step <= index) {
        index -= step;
        step >>= 1;
    }
    index += step;
    return index;
}

// @verified - d
void InitializeFftPhases(float *phases) {
    float (*const grid)[16] = reinterpret_cast<float (*)[16]>(phases);
    for (std::int32_t row = 0; row < 8; ++row) {
        for (std::int32_t column = 0; column < 8; ++column) {
            if (!row && !column)
                continue;
#if SHENMUE_IS_S2
            grid[row][column] = RandomFloat() * kFftPhaseTurn - kFftPhasePi;
            grid[row][16 - column] = RandomFloat() * kFftPhaseTurn - kFftPhasePi;
#else
            grid[row][column] = RandomFloat() * kFftPhaseTurn;
            grid[row][16 - column] = RandomFloat() * kFftPhaseTurn;
#endif
        }
    }
}

#if SHENMUE_IS_S2
// @verified - d
void BuildFftSpectrum(float *phases, float (*real)[17], float (*imaginary)[17], float speed, float *amplitudes) {
    float (*const weights)[8] = reinterpret_cast<float (*)[8]>(amplitudes);
    register float sine asm("fr4");
    register float cosine asm("fr5");
    std::int32_t row, column;
    for (row = 0; row < 17; ++row) {
        for (column = 0; column < 17; ++column) {
            real[row][column] = 0.0f;
            imaginary[row][column] = 0.0f;
        }
    }
    real[0][0] = -50.0f;
    imaginary[0][0] = 0.0f;
    real[0][16] = -50.0f;
    imaginary[0][16] = 0.0f;
    real[16][0] = -50.0f;
    imaginary[16][0] = 0.0f;
    real[16][16] = -50.0f;
    imaginary[16][16] = 0.0f;
    for (column = 1; column < 8; ++column) {
        const float first = phases[column];
        const float second = phases[16 - column];
        const float amplitude = weights[0][column];
        asm("fsca fpul,dr4" : "=f"(sine), "=f"(cosine) : "y"(static_cast<std::int32_t>((first * 65536.0f) / kFftPhaseTurn)));
        float realValue = cosine * amplitude;
        float imaginaryValue = sine * amplitude;
        real[0][column] = realValue;
        imaginary[0][column] = imaginaryValue;
        real[16][16 - column] = realValue;
        imaginary[16][16 - column] = -imaginaryValue;
        asm("fsca fpul,dr4" : "=f"(sine), "=f"(cosine) : "y"(static_cast<std::int32_t>((second * 65536.0f) / kFftPhaseTurn)));
        realValue = cosine * amplitude;
        imaginaryValue = sine * amplitude;
        real[0][16 - column] = realValue;
        imaginary[0][16 - column] = imaginaryValue;
        real[16][column] = realValue;
        imaginary[16][column] = -imaginaryValue;
    }
    for (row = 1; row < 8; ++row) {
        const float first = reinterpret_cast<float (*)[16]>(phases)[row][0];
        const float second = reinterpret_cast<float (*)[16]>(phases)[row][16];
        const float amplitude = weights[row][0];
        asm("fsca fpul,dr4" : "=f"(sine), "=f"(cosine) : "y"(static_cast<std::int32_t>((first * 65536.0f) / kFftPhaseTurn)));
        float realValue = cosine * amplitude;
        float imaginaryValue = sine * amplitude;
        real[row][0] = realValue;
        imaginary[row][0] = imaginaryValue;
        real[16 - row][16] = realValue;
        imaginary[16 - row][16] = -imaginaryValue;
        asm("fsca fpul,dr4" : "=f"(sine), "=f"(cosine) : "y"(static_cast<std::int32_t>((second * 65536.0f) / kFftPhaseTurn)));
        realValue = cosine * amplitude;
        imaginaryValue = sine * amplitude;
        real[row][16] = realValue;
        imaginary[row][16] = imaginaryValue;
        real[16 - row][0] = realValue;
        imaginary[16 - row][0] = -imaginaryValue;
    }
    for (row = 1; row < 8; ++row) {
        for (column = 1; column < 8; ++column) {
            const float first = reinterpret_cast<float (*)[16]>(phases)[row][column];
            const float second = reinterpret_cast<float (*)[16]>(phases)[row][16 - column];
            const float amplitude = weights[row][column];
            asm("fsca fpul,dr4" : "=f"(sine), "=f"(cosine) : "y"(static_cast<std::int32_t>((first * 65536.0f) / kFftPhaseTurn)));
            float realValue = cosine * amplitude;
            float imaginaryValue = sine * amplitude;
            real[row][column] = realValue;
            imaginary[row][column] = imaginaryValue;
            real[16 - row][16 - column] = realValue;
            imaginary[16 - row][16 - column] = -imaginaryValue;
            asm("fsca fpul,dr4" : "=f"(sine), "=f"(cosine) : "y"(static_cast<std::int32_t>((second * 65536.0f) / kFftPhaseTurn)));
            realValue = cosine * amplitude;
            imaginaryValue = sine * amplitude;
            real[row][16 - column] = realValue;
            imaginary[row][16 - column] = imaginaryValue;
            real[16 - row][column] = realValue;
            imaginary[16 - row][column] = -imaginaryValue;
        }
    }
}
#else
// @verified - d
std::int32_t FftPhaseAngle(float phase) {
    return static_cast<std::int32_t>((phase * 65536.0f) / kFftPhaseTurn);
}

// @verified - d
void BuildFftSpectrum(FftEffect &effect) {
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
    for (std::int32_t row = 0; row < 17; ++row) {
        for (std::int32_t column = 0; column < 17; ++column) {
            effect.real[row][column] = 0.0f;
            effect.imaginary[row][column] = 0.0f;
        }
    }
#endif
    effect.real[0][0] = -50.0f;
    effect.real[0][16] = -50.0f;
    effect.real[16][0] = -50.0f;
    effect.real[16][16] = -50.0f;
    effect.imaginary[0][0] = 0.0f;
    effect.imaginary[0][16] = 0.0f;
    effect.imaginary[16][0] = 0.0f;
    effect.imaginary[16][16] = 0.0f;
    for (std::int32_t column = 1; column < 8; ++column) {
        Vector2 values;
        const float amplitude = effect.amplitudes[column];
        CalculateFixedSinCos(FftPhaseAngle(effect.phases[column]), &values);
        float real = values.y * amplitude;
        float imaginary = values.x * amplitude;
        effect.real[0][column] = real;
        effect.imaginary[0][column] = imaginary;
        effect.real[16][16 - column] = real;
        effect.imaginary[16][16 - column] = -imaginary;
        CalculateFixedSinCos(FftPhaseAngle(effect.phases[16 - column]), &values);
        real = values.y * amplitude;
        imaginary = values.x * amplitude;
        effect.real[0][16 - column] = real;
        effect.imaginary[0][16 - column] = imaginary;
        effect.real[16][column] = real;
        effect.imaginary[16][column] = -imaginary;
    }
    for (std::int32_t row = 1; row < 8; ++row) {
        Vector2 values;
        const float amplitude = effect.amplitudes[row * 8];
        CalculateFixedSinCos(FftPhaseAngle(effect.phases[row * 16]), &values);
        float real = values.y * amplitude;
        float imaginary = values.x * amplitude;
        effect.real[row][0] = real;
        effect.imaginary[row][0] = imaginary;
        effect.real[16 - row][16] = real;
        effect.imaginary[16 - row][16] = -imaginary;
        CalculateFixedSinCos(FftPhaseAngle(effect.phases[row * 16 + 16]), &values);
        real = values.y * amplitude;
        imaginary = values.x * amplitude;
        effect.real[row][16] = real;
        effect.imaginary[row][16] = imaginary;
        effect.real[16 - row][0] = real;
        effect.imaginary[16 - row][0] = -imaginary;
    }
    for (std::int32_t row = 1; row < 8; ++row) {
        for (std::int32_t column = 1; column < 8; ++column) {
            Vector2 values;
            const float amplitude = effect.amplitudes[row * 8 + column];
            CalculateFixedSinCos(FftPhaseAngle(effect.phases[row * 16 + column]), &values);
            float real = values.y * amplitude;
            float imaginary = values.x * amplitude;
            effect.real[row][column] = real;
            effect.imaginary[row][column] = imaginary;
            effect.real[16 - row][16 - column] = real;
            effect.imaginary[16 - row][16 - column] = -imaginary;
            CalculateFixedSinCos(FftPhaseAngle(effect.phases[row * 16 + 16 - column]), &values);
            real = values.y * amplitude;
            imaginary = values.x * amplitude;
            effect.real[row][16 - column] = real;
            effect.imaginary[row][16 - column] = imaginary;
            effect.real[16 - row][column] = real;
            effect.imaginary[16 - row][column] = -imaginary;
        }
    }
}
#endif

#if SHENMUE_IS_S2
// @verified - d
void UpdateFftSpectrum(float (*phases)[16], float (*phaseSteps)[16], float (*real)[17], float (*imaginary)[17], float speed, float *amplitudes) {
    std::int32_t line, element;
    for (line = 0; line < 8; ++line) {
        for (element = 0; element < 8; ++element) {
            switch (kFftPhaseModes[line][element]) {
            case 0:
                phases[line][element] = 0.0f;
                phases[line][16 - element] = 0.0f;
                break;
            case 1:
                phases[line][element] = kFftPhasePi;
                phases[line][16 - element] = kFftPhasePi;
                break;
            default:
                phases[line][element] += phaseSteps[line][element];
                phases[line][16 - element] += phaseSteps[line][16 - element];
                break;
            }
            if (phases[line][element] > kFftPhasePi)
                phases[line][element] = phases[line][element] - kFftPhaseTurn;
            else if (phases[line][element] < -kFftPhasePi)
                phases[line][element] = phases[line][element] + kFftPhaseTurn;
            if (phases[line][16 - element] > kFftPhasePi)
                phases[line][16 - element] = phases[line][16 - element] - kFftPhaseTurn;
            else if (phases[line][16 - element] < -kFftPhasePi)
                phases[line][16 - element] = phases[line][16 - element] + kFftPhaseTurn;
        }
    }
    BuildFftSpectrum(*phases, real, imaginary, speed, amplitudes);
    Vector2 values[16];
    for (line = 0; line < 16; ++line) {
        for (element = 0; element < 16; ++element) {
            values[element].x = real[line][element];
            values[element].y = imaginary[line][element];
        }
        FftInverse16(values);
        for (element = 0; element < 16; ++element) {
            real[line][element] = values[element].x;
            imaginary[line][element] = values[element].y;
        }
    }
    for (line = 0; line < 16; ++line) {
        for (element = 0; element < 16; ++element) {
            values[element].x = real[element][line];
            values[element].y = imaginary[element][line];
        }
        FftInverse16(values);
        for (element = 0; element < 16; ++element) {
            real[element][line] = values[element].x;
            imaginary[element][line] = values[element].y;
        }
    }
}
#else
// @verified - d
void UpdateFftSpectrum(FftEffect &effect) {
    for (std::int32_t row = 0; row < 8; ++row) {
        for (std::int32_t column = 0; column < 8; ++column) {
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
            const std::int32_t mode = kFftPhaseModes[row][column];
            if (!mode) {
#else
            if (!row && !column) {
#endif
                effect.phases[row * 16 + column] = 0.0f;
                effect.phases[row * 16 + 16 - column] = 0.0f;
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
            } else if (mode == 1) {
#else
            } else if (!row || !column) {
#endif
                effect.phases[row * 16 + column] = kFftPhasePi;
                effect.phases[row * 16 + 16 - column] = kFftPhasePi;
            } else {
                effect.phases[row * 16 + column] += effect.phaseSteps[row * 16 + column];
                effect.phases[row * 16 + 16 - column] += effect.phaseSteps[row * 16 + 16 - column];
            }
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
            float &first = effect.phases[row * 16 + column];
            float &second = effect.phases[row * 16 + 16 - column];
            if (first > kFftPhasePi)
                first -= kFftPhaseTurn;
            else if (first < -kFftPhasePi)
                first += kFftPhaseTurn;
            if (second > kFftPhasePi)
                second -= kFftPhaseTurn;
            else if (second < -kFftPhasePi)
                second += kFftPhaseTurn;
#endif
        }
    }
    BuildFftSpectrum(effect);
    Vector2 values[16];
    for (std::int32_t row = 0; row < 16; ++row) {
        for (std::int32_t column = 0; column < 16; ++column) {
            values[column].x = effect.real[row][column];
            values[column].y = effect.imaginary[row][column];
        }
        FftInverse16(values);
        for (std::int32_t column = 0; column < 16; ++column) {
            effect.real[row][column] = values[column].x;
            effect.imaginary[row][column] = values[column].y;
        }
    }
    for (std::int32_t column = 0; column < 16; ++column) {
        for (std::int32_t row = 0; row < 16; ++row) {
            values[row].x = effect.real[row][column];
            values[row].y = effect.imaginary[row][column];
        }
        FftInverse16(values);
        for (std::int32_t row = 0; row < 16; ++row) {
            effect.real[row][column] = values[row].x;
            effect.imaginary[row][column] = values[row].y;
        }
    }
}
#endif

// @verified - d
void TwiddleFftPixels(std::uint16_t *pixels) {
    std::uint16_t twiddled[256];
    const std::uint16_t *line = pixels;
    std::uint32_t rowBits = 0;
    std::int32_t index;
    for (index = 0; index < 16; ++index) {
        std::uint32_t columnBits = 0;
        const std::uint16_t *source = line;
        for (std::int32_t column = 0; column < 16; ++column) {
            twiddled[columnBits | rowBits] = *source++;
            columnBits = (columnBits + 0x55555556U) & 0xaaaaaaaaU;
        }
        line += 16;
        rowBits = (rowBits + 0xaaaaaaabU) & 0x55555555U;
    }
    for (index = 0; index < 256; ++index)
        *pixels++ = twiddled[index];
}

} // namespace

#if SHENMUE_IS_S2
static inline void InitializeFftTwiddleTable(const std::uint32_t &size, const std::int32_t &halfSize, const float &angleIncrement) {
    const std::uint32_t count = size;
    const std::int32_t step = halfSize;
    const float angleStep = angleIncrement;
    std::int32_t reversed = -step;
    for (std::uint32_t index = 0; index < count; ++index) {
        reversed = NextFftReversedIndex(reversed, step);
        const float cosine = CosRadians(angleStep * static_cast<float>(index));
        const float sine = SinRadians(angleStep * static_cast<float>(index));
        g_fftForwardTwiddles[reversed].x = cosine;
        g_fftForwardTwiddles[reversed].y = sine;
        g_fftBackwardTwiddles[reversed].x = cosine;
        g_fftBackwardTwiddles[reversed].y = -sine;
    }
}

// @verified - d
void InitializeFftTwiddlesSmall() {
    InitializeFftTwiddleTable(4, 2, 0.785398185253143310546875f);
}

// @verified - d
void InitializeFftTwiddles() {
    InitializeFftTwiddleTable(8, 4, 0.39269909262657166f);
}

// @verified - d
void InitializeFftTwiddles32() {
    InitializeFftTwiddleTable(16, 8, 0.19634954631328583f);
}

// @verified - d
void FftReverse8(Vector2 *values) {
    float firstX = values[1].x, firstY = values[1].y, secondX = values[4].x, secondY = values[4].y;
    values[4].x = firstX;
    values[4].y = firstY;
    values[1].x = secondX;
    values[1].y = secondY;
    firstX = values[3].x;
    firstY = values[3].y;
    secondX = values[6].x;
    secondY = values[6].y;
    values[6].x = firstX;
    values[6].y = firstY;
    values[3].x = secondX;
    values[3].y = secondY;
}
#else
void InitializeFftTwiddles() {
    std::int32_t reversed = -4;
    for (std::int32_t index = 0; index < 8; ++index) {
        reversed = NextFftReversedIndex(reversed, 4);
        const float angle = static_cast<float>(index) * kFftTwiddleStep;
        const float cosine = std::cos(angle);
        const float sine = std::sin(angle);
        g_fftForwardTwiddles[reversed].x = cosine;
        g_fftForwardTwiddles[reversed].y = sine;
        g_fftBackwardTwiddles[reversed].x = cosine;
        g_fftBackwardTwiddles[reversed].y = -sine;
    }
}
#endif

#if SHENMUE_IS_S1 || SHENMUE_IS_S2
// @verified - d
void InitializeFftEffect(FftEffect *effect) {
    effect->flags &= 0x7fffffffU;
    effect->speed = 3.0f;
    effect->intensity = 1.0f;
    effect->phaseStepScale = 0.5f;
    InitializeFftPhases(effect->phases);
    InitializeFftPhaseSteps(effect->phaseSteps, effect->phaseStepScale);
#if SHENMUE_IS_S2
    BuildFftSpectrum(effect->phases, effect->real, effect->imaginary, effect->speed, effect->amplitudes);
#else
    BuildFftSpectrum(*effect);
#endif
    InitializeFftTwiddles();
}
#else
// @verified - d
void InitializeFftEffects() {
    for (std::int32_t index = 0; index < 2; ++index) {
        FftEffect &effect = g_fftEffects[index];
        effect.flags &= 0x7fffffffU;
        effect.speed = 3.0f;
        effect.intensity = 1.0f;
        effect.phaseStepScale = 0.5f;
        InitializeFftPhases(effect.phases);
        InitializeFftPhaseSteps(effect.phaseSteps, effect.phaseStepScale);
        BuildFftSpectrum(effect);
    }
    InitializeFftTwiddles();
}
#endif

#if !SHENMUE_IS_S2
// @verified - d
void InitializeTerrainEffects() {
    InitializeFftTwiddles();
    for (std::int32_t index = 0; index < 2; ++index) {
        TerrainEffect &effect = g_terrainEffects[index];
        effect.flags = 0;
        effect.colour = 0xffffffffU;
        for (std::int32_t row = 0; row < 8; ++row) {
            for (std::int32_t column = 0; column < 8; ++column) {
                effect.phases[row][column].first = RandomFloat() * kFftPhaseTurn;
                effect.phases[row][column].second = RandomFloat() * kFftPhaseTurn;
            }
        }
    }
    for (std::int32_t index = 0; index < 4; ++index)
        g_terrainChannels[index].surface.pSurface = nullptr;
}

#endif

// @verified - d
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateFftEffect(FftEffectHandle handle) {
    FftEffect &effect =
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
        *handle;
#else
        g_fftEffects[handle];
#endif
    if (!(effect.flags & 0x80000000U))
        return;
    std::uint16_t *const base = effect.pixels;
    std::uint16_t *pixels = base;
    const std::uint16_t colour = effect.colour & 0xfffU;
    const std::int32_t maximumColour = colour | -4096;
#if SHENMUE_IS_S2
    UpdateFftSpectrum(reinterpret_cast<float (*)[16]>(effect.phases), reinterpret_cast<float (*)[16]>(effect.phaseSteps), effect.real, effect.imaginary,
        effect.speed, effect.amplitudes);
#else
    UpdateFftSpectrum(effect);
#endif
    for (std::int32_t row = 0; row < 16; ++row) {
        for (std::int32_t column = 0; column < 16; ++column) {
            const float intensity = effect.real[row][column] * effect.intensity;
            if (!(intensity > 0.0f)) {
                *pixels = 0;
            } else if (!(intensity < 15.0f)) {
                *pixels = maximumColour;
            } else {
                *pixels = static_cast<std::uint16_t>((static_cast<std::int32_t>(intensity) << 12) | colour);
            }
            ++pixels;
        }
    }
    TwiddleFftPixels(base);
}
#else
void UpdateFftEffect(FftEffectHandle handle) {
    FftEffect &effect =
#if SHENMUE_IS_S1 || SHENMUE_IS_S2
        *handle;
#else
        g_fftEffects[handle];
#endif
    if (!(effect.flags & 0x80000000U))
        return;
    std::uint16_t *pixels = effect.pixels;
    const std::uint16_t colour = effect.colour & 0xfffU;
#if SHENMUE_IS_S2
    UpdateFftSpectrum(reinterpret_cast<float (*)[16]>(effect.phases), reinterpret_cast<float (*)[16]>(effect.phaseSteps), effect.real, effect.imaginary,
        effect.speed, effect.amplitudes);
#else
    UpdateFftSpectrum(effect);
#endif
    for (std::int32_t row = 0; row < 16; ++row) {
        for (std::int32_t column = 0; column < 16; ++column) {
            const float intensity = effect.real[row][column] * effect.intensity;
            if (intensity > 0.0f) {
                *pixels = intensity < 15.0f ? static_cast<std::uint16_t>(((static_cast<std::int32_t>(intensity) & 15) << 12) | colour) : colour | 0xf000U;
            } else {
                *pixels = 0;
            }
            ++pixels;
        }
    }
    TwiddleFftPixels(effect.pixels);
}
#endif


} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif
