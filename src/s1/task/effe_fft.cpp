#include "s1/task/effe_internal.hpp"

namespace shenmue::s1::effe_detail {

float EffeFftRandomUnit(std::uint32_t seed) {
    return static_cast<float>((seed >> 16U) & 0x7fffU) * 0.000030517578125f;
}

// Two g_EnvPrngSeed draws per pair of mirrored bins, skipping the DC pair.
void GenerateRandomFFTPhases(float *phases) {
    std::uint32_t seed = g_EnvPrngSeed;
    for (std::size_t row = 0; row < 8; ++row) {
        for (std::size_t column = 0; column < 8; ++column) {
            if (!row && !column)
                continue;
            const std::uint32_t first = LcgStep(seed);
            seed = LcgStep(first);
            g_EnvPrngSeed = seed;
            phases[row * 16 + column] = EffeFftRandomUnit(first) * 6.2831855f - 3.1415927f;
            phases[row * 16 + 16 - column] = EffeFftRandomUnit(seed) * 6.2831855f - 3.1415927f;
        }
    }
}

void NormalizeAndScaleFFTInput(float *rates, float scale) {
    std::uint32_t seed = g_EnvPrngSeed;
    for (std::size_t row = 0; row < 8; ++row) {
        for (std::size_t column = 0; column < 8; ++column) {
            const std::uint32_t first = LcgStep(seed);
            seed = LcgStep(first);
            float &low = rates[row * 16 + column];
            float &high = rates[row * 16 + 16 - column];
            low = EffeFftRandomUnit(first) * scale;
            high = EffeFftRandomUnit(seed) * scale;
            if (low < kEffeFftEpsilon && low > -kEffeFftEpsilon)
                low = 0.0f;
            if (high < kEffeFftEpsilon && high > -kEffeFftEpsilon)
                high = 0.0f;
        }
        g_EnvPrngSeed = seed;
    }
}

// One bin: amplitude * (cos, sin) of the phase in fixed-16 angle units.
void EffeFftStoreBin(float *real, float *imaginary, std::size_t index, std::size_t mirror, float phase, float amplitude) {
    const auto angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(phase * 10430.378f));
    const std::uint32_t folded = angle <= 0x8000U ? angle : 0x10000U - angle;
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000U - folded)) * amplitude;
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle)) * amplitude;
    real[index] = cosine;
    imaginary[index] = sine;
    real[mirror] = cosine;
    imaginary[mirror] = -sine;
}

// The Hermitian 17x17 spectrum from the 8x8 falloff and the mirrored phases; the four corner DC terms are -50.
void BitReverseAndPreprocess(const float *phases, float *real, float *imaginary, const float *falloff) {
    for (std::size_t index = 0; index < 289; ++index) {
        real[index] = 0.0f;
        imaginary[index] = 0.0f;
    }
    real[0] = -50.0f;
    real[16] = -50.0f;
    real[272] = -50.0f;
    real[288] = -50.0f;
    for (std::size_t column = 1; column < 8; ++column) {
        EffeFftStoreBin(real, imaginary, column, 288 - column, phases[column], falloff[column]);
        EffeFftStoreBin(real, imaginary, 16 - column, 272 + column, phases[16 - column], falloff[column]);
    }
    for (std::size_t row = 1; row < 8; ++row) {
        EffeFftStoreBin(real, imaginary, row * 17, 288 - row * 17, phases[row * 16], falloff[row * 8]);
        EffeFftStoreBin(real, imaginary, row * 17 + 16, (16 - row) * 17, phases[row * 16 + 16], falloff[row * 8]);
    }
    for (std::size_t row = 1; row < 8; ++row) {
        for (std::size_t column = 1; column < 8; ++column) {
            const float amplitude = falloff[row * 8 + column];
            EffeFftStoreBin(real, imaginary, row * 17 + column, (16 - row) * 17 + 16 - column, phases[row * 16 + column], amplitude);
            EffeFftStoreBin(real, imaginary, row * 17 + 16 - column, (16 - row) * 17 + column, phases[row * 16 + 16 - column], amplitude);
        }
    }
}

float WrapEffeFftPhase(float phase) {
    if (phase > 3.1415927f)
        return static_cast<float>(static_cast<double>(phase) - 6.283185482025146);
    if (phase < -3.1415927f)
        return static_cast<float>(static_cast<double>(phase) + 6.283185482025146);
    return phase;
}

// Advance the phases, rebuild the spectrum, then run the 16-point transform over the rows and the columns.
void RunFFTStagesAndReorder(EffeFftData &fft) {
    for (std::size_t row = 0; row < 8; ++row) {
        for (std::size_t column = 0; column < 8; ++column) {
            float &low = fft.phases[row * 16 + column];
            float &high = fft.phases[row * 16 + 16 - column];
            const std::int32_t control = kEffeFftPhaseControl[row * 8 + column];
            if (control == 0) {
                low = 0.0f;
                high = 0.0f;
            } else if (control == 1) {
                low = 3.1415927f;
                high = 3.1415927f;
            } else {
                low = fft.phaseRates[row * 16 + column] + low;
                high = fft.phaseRates[row * 16 + 16 - column] + high;
            }
            low = WrapEffeFftPhase(low);
            high = WrapEffeFftPhase(high);
        }
    }
    BitReverseAndPreprocess(fft.phases.data(), fft.real.data(), fft.imaginary.data(), fft.falloff.data());
    std::array<WaveComplex, 16> values{};
    for (std::size_t row = 0; row < 16; ++row) {
        for (std::size_t column = 0; column < 16; ++column)
            values[column] = {fft.real[row * 17 + column], fft.imaginary[row * 17 + column]};
        Run16PointFFTButterflyPasses(values);
        for (std::size_t column = 0; column < 16; ++column) {
            fft.real[row * 17 + column] = values[column].real;
            fft.imaginary[row * 17 + column] = values[column].imaginary;
        }
    }
    for (std::size_t column = 0; column < 16; ++column) {
        for (std::size_t row = 0; row < 16; ++row)
            values[row] = {fft.real[row * 17 + column], fft.imaginary[row * 17 + column]};
        Run16PointFFTButterflyPasses(values);
        for (std::size_t row = 0; row < 16; ++row) {
            fft.real[row * 17 + column] = values[row].real;
            fft.imaginary[row * 17 + column] = values[row].imaginary;
        }
    }
}

// The 8x8 falloff: 100 / n^(base/2) along the first row and column, 25 / d^(base/2) inside; tiny values become epsilon.
float *PrecomputeLogTable(float base, float *table) {
    const float exponent = base * 0.5f;
    const auto guard = [](float numerator, float power) {
        if (static_cast<float>(std::fabs(static_cast<double>(power))) < kEffeFftEpsilon)
            return kEffeFftEpsilon;
        const float value = numerator / power;
        return value < kEffeFftEpsilon && value > -kEffeFftEpsilon ? kEffeFftEpsilon : value;
    };
    table[0] = kEffeFftEpsilon;
    for (std::int32_t row = 1; row < 8; ++row)
        table[row * 8] = guard(100.0f, fast_powf_approx(static_cast<float>(row), exponent));
    for (std::int32_t column = 1; column < 8; ++column)
        table[column] = guard(100.0f, fast_powf_approx(static_cast<float>(column), exponent));
    for (std::int32_t row = 1; row < 8; ++row) {
        for (std::int32_t column = 1; column < 8; ++column)
            table[row * 8 + column] = guard(25.0f, fast_powf_approx(std::sqrt(static_cast<float>(column * column + row * row)), exponent));
    }
    return table;
}

float *ApplyLogClampVectorScaling(float scale, float *table) {
    for (std::size_t index = 0; index < 64; ++index) {
        const float value = table[index] * scale;
        table[index] = value > -kEffeFftEpsilon && value < kEffeFftEpsilon ? kEffeFftEpsilon : value;
    }
    return table;
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void InitializeFFTData(EffeFftData *fft) {
    fft->flags &= ~0x80000000U;
    fft->phaseParameter = 3.0f;
    fft->outputScale = 1.0f;
    fft->phaseRateScale = 0.5f;
    GenerateRandomFFTPhases(fft->phases.data());
    NormalizeAndScaleFFTInput(fft->phaseRates.data(), fft->phaseRateScale);
    BitReverseAndPreprocess(fft->phases.data(), fft->real.data(), fft->imaginary.data(), fft->falloff.data());
    Generate32PointTwiddles();
}

// Only while bit 31 is set: advance and transform, then quantize real * outputScale to the 4-bit alpha of packedColour's RGB444.
void ProcessAndQuantizeFFTOutput(EffeFftData *fft) {
    if ((fft->flags & 0x80000000U) == 0)
        return;
    const std::uint32_t colour = fft->packedColour & 0xfffU;
    const std::uint32_t saturated = colour | 0xf000U;
    RunFFTStagesAndReorder(*fft);
    std::uint32_t *output = fft->output;
    for (std::size_t row = 0; row < 16; ++row) {
        for (std::size_t column = 0; column < 16; ++column) {
            const float value = fft->real[row * 17 + column] * fft->outputScale;
            if (!(static_cast<double>(value) > 0.0)) {
                *output++ = 0;
                continue;
            }
            std::uint32_t texel = saturated;
            if (static_cast<double>(value) < 15.0)
                texel = static_cast<std::uint16_t>(static_cast<std::int32_t>(value) << 12) | colour;
            const std::uint32_t expanded = ((((texel >> 12) << 8) + ((texel >> 8) & 0xfU)) << 8) + ((texel >> 4) & 0xfU);
            *output++ = 17U * ((expanded << 8) + (texel & 0xfU));
        }
    }
}

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

// EnqueueSpecialEffect's group owner: shared by every special effect of the group, created with its FFT record, a 0xff-filled output block and
// the base-3.02 falloff scaled by magnitude * 100 / 255.
EffeFftOwner *EFFE_AcquireFftOwner(std::uint32_t taskFlags, float magnitude) {
    const std::size_t group = (taskFlags >> 4U) & 0xfU;
    EffeFftOwner *owner = g_effeFftOwners[group];
    if (!owner) {
        owner = static_cast<EffeFftOwner *>(HLib_AllocHeapMemBlock(sizeof(EffeFftOwner), SHEN_NAME(EFPT)));
        g_effeFftOwners[group] = owner;
        owner->fftIndex = EFFE_Related(1, -1, nullptr, nullptr, SHEN_NAME(EFPT));
        owner->colour = 0x00ffffffU;
        EffeFftData *const fft = g_effeFftData[static_cast<std::size_t>(owner->fftIndex)];
        fft->packedColour = owner->colour;
        owner->pixels = static_cast<std::byte *>(HLib_AllocHeapMemBlock(0x41f, SHEN_NAME(EFPT)));
        std::memset(owner->pixels, 0xff, 0x41f);
        fft->output = reinterpret_cast<std::uint32_t *>(owner->pixels);
        std::array<float, 64> table{};
        std::memcpy(fft->falloff.data(),
            ApplyLogClampVectorScaling(static_cast<float>(static_cast<double>(magnitude) * 100.0 / 255.0), PrecomputeLogTable(3.0196078f, table.data())),
            sizeof(fft->falloff));
        fft->phaseRateScale = 0.5f;
        NormalizeAndScaleFFTInput(fft->phaseRates.data(), 0.5f);
        owner->references = 0;
        owner->activeMask = 0;
    }
    ++owner->references;
    return owner;
}

// DebugEffect's owner release: the last special effect of the group frees the FFT record, the output block and the owner.
void EFFE_ReleaseFftOwner(EffeFftOwner *owner, std::uint32_t taskFlags) {
    if (!owner || --owner->references)
        return;
    const auto index = static_cast<std::size_t>(owner->fftIndex);
    EffeFftData *const fft = g_effeFftData[index];
    fft->flags &= ~0x80000000U;
    EFFE_ReleaseRegistryEntry(g_effeFftRegistry[index]);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(fft), 0);
    std::byte *const pixels = owner->pixels;
    g_effeFftOwners[(taskFlags >> 4U) & 0xfU] = nullptr;
    if (pixels)
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(pixels), 0);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(owner), 0);
}

} // namespace shenmue::s1::effe_detail
