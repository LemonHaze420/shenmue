#include "shenmue.h"
#include "s2/effect_fft.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/event_matrix_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/particle_effects.hpp"

#include "s2/motion_work.hpp"
#endif

#include "common/math.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "platform/dc/particle_effects.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/modifier_volume_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <algorithm>
#include <array>
#endif

#include <cmath>

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include <cstring>
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <cstddef>
#include <cstdint>
#endif

namespace shenmue {

namespace s2 {

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void BuildFireSpectrum(const float *phases, float (*real)[33], float (*imaginary)[33], float phaseParameter, const float *falloff);
void UpdateFireSpectrum(float *phases, const float *phaseRates, float (*real)[33], float (*imaginary)[33], float phaseParameter, const float *falloff);
void InitializeFirePhases(float *phases);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
constexpr float kPi = 3.1415927f;
constexpr float kTau = 6.2831855f;
constexpr float kMinimumFloat = 0.00000011920929f;
constexpr float kFixedAngleScale = 10430.378f;
constexpr std::size_t kSmokeOutputOffset = 0x80;

constexpr std::array<std::uint16_t, 16> kDefaultFirePalette{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};

constexpr std::array<std::uint8_t, 64> kSmokePhaseControl{0, 1, 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

struct Complex {
    float real{};
    float imaginary{};
};
std::array<Complex, 16> fftTwiddles{};

void GenerateNative32PointTwiddles() noexcept {
    for (std::uint32_t index = 0; index < 16; ++index) {
        const std::uint32_t reversed = ((index & 1U) << 3U) | ((index & 2U) << 1U) | ((index & 4U) >> 1U) | ((index & 8U) >> 3U);
        const float angle = static_cast<float>(index) * 0.19634955f;
        fftTwiddles[reversed] = {std::sin(1.5707964f - std::fabs(angle)), -std::sin(angle)};
    }
}

float NativeLog(float value) noexcept {
    float result = 0.0f;
    if (value <= 0.0f)
        return result;
    while (value >= 2.0f) {
        value *= 0.5f;
        result += 0.69314718f;
    }
    while (value < 1.0f) {
        value += value;
        result -= 0.69314718f;
    }
    const float z = (value - 1.0f) / (value + 1.0f);
    float term = z + z;
    const float square = z * z;
    for (std::int32_t denominator = 1;; denominator += 2) {
        const float previous = result;
        const float current = term;
        term *= square;
        result += current / static_cast<float>(denominator);
        if (result == previous)
            return result;
    }
}

float NativePower(float value, float exponent) noexcept {
    const float logarithm = NativeLog(value) * exponent;
    const std::int32_t power = static_cast<std::int32_t>(logarithm / 0.69314718f + (logarithm < 0.0f ? -0.5f : 0.5f));
    const float remainder = logarithm - static_cast<float>(power) * 0.69314718f;
    const float square = remainder * remainder;
    float fraction = square / 14.0f;
    for (std::int32_t denominator = 10; denominator >= 6; denominator -= 4)
        fraction = square / (static_cast<float>(denominator) + fraction);
    return std::ldexp(((fraction + 2.0f) + remainder) / ((fraction + 2.0f) - remainder), power);
}

float NativeSine(float phase) noexcept {
    const auto angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(phase * kFixedAngleScale));
    return sin_fixed16(static_cast<std::int16_t>(angle));
}

float NativeCosine(float phase) noexcept {
    const auto angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(phase * kFixedAngleScale));
    const auto cosineAngle = static_cast<std::uint16_t>(0x4000 - (angle > 0x8000U ? 0x10000U - angle : angle));
    return sin_fixed16(static_cast<std::int16_t>(cosineAngle));
}

Complex Add(Complex first, Complex second) noexcept {
    return {first.real + second.real, first.imaginary + second.imaginary};
}

Complex Subtract(Complex first, Complex second) noexcept {
    return {first.real - second.real, first.imaginary - second.imaginary};
}

Complex Rotate(Complex value, Complex twiddle) noexcept {
    return {value.real * twiddle.real + twiddle.imaginary * value.imaginary, -twiddle.imaginary * value.real + twiddle.real * value.imaginary};
}

void TransformSmokeLine(std::array<Complex, 16> &line) noexcept {
    for (std::uint32_t index = 0; index < 16; ++index) {
        const std::uint32_t reversed = ((index & 1U) << 3U) | ((index & 2U) << 1U) | ((index & 4U) >> 1U) | ((index & 8U) >> 3U);
        if (index < reversed)
            std::swap(line[index], line[reversed]);
    }

    for (std::size_t group = 0; group < 4; ++group) {
        const std::size_t index = group * 4;
        const Complex first = Add(line[index], line[index + 1]);
        const Complex second = Add(line[index + 2], line[index + 3]);
        const Complex third = Rotate(Subtract(line[index], line[index + 1]), fftTwiddles[group * 2]);
        const Complex fourth = Rotate(Subtract(line[index + 2], line[index + 3]), fftTwiddles[group * 2 + 1]);
        line[index] = Add(first, second);
        line[index + 2] = Rotate(Subtract(first, second), fftTwiddles[group]);
        line[index + 1] = Add(third, fourth);
        line[index + 3] = Rotate(Subtract(third, fourth), fftTwiddles[group]);
    }
    for (std::size_t index = 0; index < 4; ++index) {
        const Complex difference = Subtract(line[index], line[index + 4]);
        line[index] = Add(line[index], line[index + 4]);
        line[index + 4] = difference;
        const Complex rotated = Subtract(line[index + 8], line[index + 12]);
        line[index + 8] = Add(line[index + 8], line[index + 12]);
        line[index + 12] = {-rotated.imaginary, rotated.real};
    }
    for (std::size_t index = 0; index < 8; ++index) {
        const Complex sum = Add(line[index], line[index + 8]);
        const Complex difference = Subtract(line[index], line[index + 8]);
        line[index] = {sum.real * 0.0625f, sum.imaginary * 0.0625f};
        line[index + 8] = {difference.real * 0.0625f, difference.imaginary * 0.0625f};
    }
}

void TransformFireLine(std::array<Complex, 32> &line) noexcept {
    for (std::uint32_t index = 0; index < 32; ++index) {
        const std::uint32_t reversed = ((index & 1U) << 4U) | ((index & 2U) << 2U) | (index & 4U) | ((index & 8U) >> 2U) | ((index & 16U) >> 4U);
        if (index < reversed)
            std::swap(line[index], line[reversed]);
    }

    for (std::size_t group = 0; group < 8; ++group) {
        const std::size_t index = group * 4;
        const Complex first = Add(line[index], line[index + 1]);
        const Complex second = Add(line[index + 2], line[index + 3]);
        const Complex third = Rotate(Subtract(line[index], line[index + 1]), fftTwiddles[group * 2]);
        const Complex fourth = Rotate(Subtract(line[index + 2], line[index + 3]), fftTwiddles[group * 2 + 1]);
        line[index] = Add(first, second);
        line[index + 2] = Rotate(Subtract(first, second), fftTwiddles[group]);
        line[index + 1] = Add(third, fourth);
        line[index + 3] = Rotate(Subtract(third, fourth), fftTwiddles[group]);
    }

    for (std::size_t group = 0; group < 4; ++group) {
        const Complex twiddle = fftTwiddles[group];
        for (std::size_t index = 0; index < 4; ++index) {
            const std::size_t firstIndex = group * 8 + index;
            const std::size_t secondIndex = firstIndex + 4;
            const Complex first = line[firstIndex];
            const Complex second = line[secondIndex];
            const float differenceReal = first.real - second.real;
            const float differenceImaginary = first.imaginary - second.imaginary;
            line[firstIndex] = Add(first, second);
            line[secondIndex] = {-twiddle.imaginary * differenceReal + twiddle.real * differenceImaginary,
                twiddle.real * differenceReal + twiddle.imaginary * differenceImaginary};
        }
    }

    for (std::size_t index = 0; index < 8; ++index) {
        const Complex first = line[index];
        const Complex second = line[index + 8];
        line[index] = Add(first, second);
        line[index + 8] = Subtract(first, second);
    }
    for (std::size_t index = 16; index < 24; ++index) {
        const Complex first = line[index];
        const Complex second = line[index + 8];
        line[index] = Add(first, second);
        line[index + 8] = {second.imaginary - first.imaginary, first.real - second.real};
    }

    for (std::size_t index = 0; index < 16; ++index) {
        const Complex first = line[index];
        const Complex second = line[index + 16];
        const Complex sum = Add(first, second);
        const Complex difference = Subtract(first, second);
        line[index] = {sum.real * 0.03125f, sum.imaginary * 0.03125f};
        line[index + 16] = {difference.real * 0.03125f, difference.imaginary * 0.03125f};
    }
}

template <std::size_t N, std::size_t Half, std::size_t Padded>
void PrepareSpectrum(const float *phases, const float *falloff, float *real, float *imaginary, float dc) noexcept {
    if constexpr (N == 16) {
        std::fill_n(real, Padded * Padded, 0.0f);
        std::fill_n(imaginary, Padded * Padded, 0.0f);
    }

    const auto store = [real, imaginary](std::size_t y, std::size_t x, float amplitude, float phase, bool conjugate) {
        const std::size_t destination = y * Padded + x;
        real[destination] = NativeCosine(phase) * amplitude;
        const float sine = NativeSine(phase) * amplitude;
        imaginary[destination] = conjugate ? -sine : sine;
    };

    for (std::size_t y = 0; y < Half; ++y) {
        for (std::size_t x = 0; x < Half; ++x) {
            const float amplitude = falloff[y * Half + x];
            const float firstPhase = phases[y * N + x];
            const float secondPhase = phases[y * N + (N - x)];
            store(y, x, amplitude, firstPhase, false);
            store(N - y, N - x, amplitude, firstPhase, true);
            store(y, N - x, amplitude, secondPhase, false);
            store(N - y, x, amplitude, secondPhase, true);
        }
    }

    real[0] = dc;
    imaginary[0] = 0.0f;
    real[N] = dc;
    imaginary[N] = 0.0f;
    real[N * Padded] = dc;
    imaginary[N * Padded] = 0.0f;
    real[N * Padded + N] = dc;
    imaginary[N * Padded + N] = 0.0f;
}

template <std::size_t N, std::size_t Padded> void RunTwoDimensionalFft(float *real, float *imaginary) noexcept {
    std::array<Complex, N> values{};
    for (std::size_t row = 0; row < N; ++row) {
        for (std::size_t column = 0; column < N; ++column) {
            const std::size_t index = row * Padded + column;
            values[column] = {real[index], imaginary[index]};
        }
        if constexpr (N == 16)
            TransformSmokeLine(values);
        else
            TransformFireLine(values);
        for (std::size_t column = 0; column < N; ++column) {
            const std::size_t index = row * Padded + column;
            real[index] = values[column].real;
            imaginary[index] = values[column].imaginary;
        }
    }
    for (std::size_t column = 0; column < N; ++column) {
        for (std::size_t row = 0; row < N; ++row) {
            const std::size_t index = row * Padded + column;
            values[row] = {real[index], imaginary[index]};
        }
        if constexpr (N == 16)
            TransformSmokeLine(values);
        else
            TransformFireLine(values);
        for (std::size_t row = 0; row < N; ++row) {
            const std::size_t index = row * Padded + column;
            real[index] = values[row].real;
            imaginary[index] = values[row].imaginary;
        }
    }
}

void GenerateSmokePhases(SmokeFftData &data) noexcept {
    for (std::size_t y = 0; y < 8; ++y) {
        for (std::size_t x = 0; x < 8; ++x) {
            if (x == 0 && y == 0)
                continue;
            data.phases[y * 16 + x] = IncrementSharedPrngFloat() * kTau - kPi;
            data.phases[y * 16 + (16 - x)] = IncrementSharedPrngFloat() * kTau - kPi;
        }
    }
}

void GenerateSmokePhaseRates(SmokeFftData &data) noexcept {
    for (std::size_t y = 0; y < 8; ++y) {
        for (std::size_t x = 0; x < 8; ++x) {
            float first = IncrementSharedPrngFloat() * data.phaseRateScale;
            float second = IncrementSharedPrngFloat() * data.phaseRateScale;
            if (first < kMinimumFloat && first > -kMinimumFloat)
                first = 0.0f;
            if (second < kMinimumFloat && second > -kMinimumFloat)
                second = 0.0f;
            data.phaseRates[y * 16 + x] = first;
            data.phaseRates[y * 16 + (16 - x)] = second;
        }
    }
}

void GenerateFirePhases(FireFftData &data) noexcept {
    for (std::size_t y = 0; y < 16; ++y) {
        for (std::size_t x = 0; x < 16; ++x) {
            if (x == 0 && y == 0)
                continue;
            data.phases[y * 32 + x] = IncrementSharedPrngFloat() * kTau;
            data.phases[y * 32 + (32 - x)] = IncrementSharedPrngFloat() * kTau;
        }
    }
}

void GenerateFirePhaseRates(FireFftData &data) noexcept {
    for (std::size_t y = 0; y < 16; ++y) {
        for (std::size_t x = 0; x < 16; ++x) {
            const float distance = std::sqrt(static_cast<float>(y * y + x * x)) * data.falloffScalar;
            data.phaseRates[y * 32 + x] = distance;
            data.phaseRates[y * 32 + (32 - x)] = distance;
        }
    }
}

float WrapSmokePhase(float value) noexcept {
    if (value > kPi)
        return value - kTau;
    if (value < -kPi)
        return value + kTau;
    return value;
}

void AdvanceSmokePhases(SmokeFftData &data) noexcept {
    for (std::size_t y = 0; y < 8; ++y) {
        for (std::size_t x = 0; x < 8; ++x) {
            const std::size_t first = y * 16 + x;
            const std::size_t second = y * 16 + (16 - x);
            switch (kSmokePhaseControl[y * 8 + x]) {
            case 0:
                data.phases[first] = 0.0f;
                data.phases[second] = 0.0f;
                break;
            case 1:
                data.phases[first] = kPi;
                data.phases[second] = kPi;
                break;
            default:
                data.phases[first] = WrapSmokePhase(data.phases[first] + data.phaseRates[first]);
                data.phases[second] = WrapSmokePhase(data.phases[second] + data.phaseRates[second]);
                break;
            }
        }
    }
}

void AdvanceFirePhases(FireFftData &data) noexcept {
    for (std::size_t y = 0; y < 16; ++y) {
        for (std::size_t x = 0; x < 16; ++x) {
            const std::size_t first = y * 32 + x;
            const std::size_t second = y * 32 + (32 - x);
            const std::uint8_t control = (x >= 8 || y >= 8) ? 0U : ((x == 0 && y == 0) ? 0U : ((x == 0 || y == 0) ? 1U : 2U));
            if (control == 0) {
                data.phases[first] = 0.0f;
                data.phases[second] = 0.0f;
            } else if (control == 1) {
                data.phases[first] = kPi;
                data.phases[second] = kPi;
            } else {
                data.phases[first] += data.phaseRates[first];
                data.phases[second] += data.phaseRates[second];
            }
        }
    }
}

std::uint32_t ExpandArgb4444(std::uint16_t packed) noexcept {
    return static_cast<std::uint32_t>(packed & 0x000fU) * 17U | static_cast<std::uint32_t>((packed >> 4U) & 0x000fU) * 17U << 8U |
        static_cast<std::uint32_t>((packed >> 8U) & 0x000fU) * 17U << 16U | static_cast<std::uint32_t>((packed >> 12U) & 0x000fU) * 17U << 24U;
}
#endif

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
 // namespace

void SetFireFftPalette(std::int32_t index, const std::uint16_t *palette);
void GenerateFirePhaseRates(float *steps, float scalar);

FireFftPrefix *g_fireFftData[8] = {};

// @verified - d
void ResetFireFftChannels() {
    for (std::int32_t rangeIndex57 = 0; rangeIndex57 < 8; ++rangeIndex57) {
        FireFftPrefix *&effect = (g_fireFftData)[rangeIndex57];
        effect = 0;
    }
}


// @verified - d
void InitializeFireFft(std::int32_t index, FireFftPrefix *effect) {
    g_fireFftData[index] = effect;
    if (!effect)
        return;
    effect->flags &= 0x7fffffffU;
    effect->phaseParameter = 3.0f;
    effect->outputScale = 1.0f;
    effect->falloffScalar = -0.699999988079071f;
    effect->tick = 0;
    effect->width = 1.0f;
    effect->height = 1.0f;
    InitializeFirePhases(effect->phases);
    GenerateFirePhaseRates(effect->phaseRates, effect->falloffScalar);
    BuildFireSpectrum(effect->phases, effect->real, effect->imaginary, effect->phaseParameter, effect->falloff);
    dc::sdk::InitializeFftTwiddles32();
    SetFireFftPalette(index, kFireFftDefaultPalette);
}


// @verified - d
void SetFireFftActive(std::int32_t index, std::int32_t active) {
    std::uint32_t &flags = g_fireFftData[index]->flags;
    if (active)
        flags |= 0x80000000U;
    else
        flags &= 0x7fffffffU;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
 // namespace

void InitializeSmokeFft(SmokeFftData &data) noexcept {
    data.flags &= ~activeFlag;
    data.phaseParameter = 3.0f;
    data.outputScale = 1.0f;
    data.phaseRateScale = 0.5f;
    GenerateNative32PointTwiddles();
    GenerateSmokePhases(data);
    GenerateSmokePhaseRates(data);
    PrepareSpectrum<16, 8, 17>(data.phases.data(), data.falloff.data(), data.real.data(), data.imaginary.data(), -50.0f);
}

void SetSmokeFftOutput(SmokeFftData &data, std::byte *raw0x49fAllocation) noexcept {
    data.outputAllocation = raw0x49fAllocation;
}

void SetSmokeFftColour(SmokeFftData &data, std::uint16_t colour) noexcept {
    data.packedColour = colour;
}

void SetSmokeFftAmplitude(SmokeFftData &data, float amplitude) noexcept {
    data.phaseRateScale = amplitude;
    GenerateSmokePhaseRates(data);
}

void GenerateSmokeFalloff(float radius, std::span<float, 64> destination) noexcept {
    const float exponent = radius * 0.5f;
    destination[0] = kMinimumFloat;
    for (std::size_t index = 1; index < 8; ++index) {
        const float power = NativePower(static_cast<float>(index), exponent);
        float value = std::fabs(power) < kMinimumFloat ? kMinimumFloat : 100.0f / power;
        if (value < kMinimumFloat && value > -kMinimumFloat)
            value = kMinimumFloat;
        destination[index * 8] = value;
        destination[index] = value;
    }
    for (std::size_t y = 1; y < 8; ++y) {
        for (std::size_t x = 1; x < 8; ++x) {
            const float distance = std::sqrt(static_cast<float>(y * y + x * x));
            const float power = NativePower(distance, exponent);
            float value = std::fabs(power) < kMinimumFloat ? kMinimumFloat : 25.0f / power;
            if (value < kMinimumFloat && value > -kMinimumFloat)
                value = kMinimumFloat;
            destination[y * 8 + x] = value;
        }
    }
}

void ScaleSmokeFalloff(float gain, std::span<float, 64> falloff) noexcept {
    for (float &value : falloff) {
        value *= gain;
        if (value < kMinimumFloat && value > -kMinimumFloat)
            value = kMinimumFloat;
    }
}

void SetSmokeFftFalloff(SmokeFftData &data, std::span<const float, 64> falloff) noexcept {
    std::copy(falloff.begin(), falloff.end(), data.falloff.begin());
}

void UpdateSmokeFft(SmokeFftData &data) noexcept {
    if ((data.flags & activeFlag) == 0)
        return;
    AdvanceSmokePhases(data);
    PrepareSpectrum<16, 8, 17>(data.phases.data(), data.falloff.data(), data.real.data(), data.imaginary.data(), -50.0f);
    RunTwoDimensionalFft<16, 17>(data.real.data(), data.imaginary.data());

    auto *output = reinterpret_cast<std::uint32_t *>(data.outputAllocation + kSmokeOutputOffset);
    const std::uint16_t colour = data.packedColour & 0x0fffU;
    for (std::size_t y = 0; y < 16; ++y) {
        for (std::size_t x = 0; x < 16; ++x) {
            const float value = data.real[y * 17 + x] * data.outputScale;
            if (value <= 0.0f) {
                *output++ = 0;
                continue;
            }
            const std::uint16_t alpha = value < 15.0f ? static_cast<std::uint16_t>(value) : 15U;
            *output++ = ExpandArgb4444(static_cast<std::uint16_t>(colour | (alpha << 12U)));
        }
    }
}

void InitializeFireFft(FireFftData &data) noexcept {
    data.flags &= ~activeFlag;
    data.phaseParameter = 3.0f;
    data.outputScale = 1.0f;
    data.falloffScalar = -0.69999999f;
    data.tick = 0;
    data.width = 1.0f;
    data.height = 1.0f;
    GenerateNative32PointTwiddles();
    GenerateFirePhases(data);
    GenerateFirePhaseRates(data);
    PrepareSpectrum<32, 16, 33>(data.phases.data(), data.falloff.data(), data.real.data(), data.imaginary.data(), data.falloff[0]);
    data.palette = kDefaultFirePalette.data();
}

void SetFireFftOutput(FireFftData &data, std::uint16_t *alignedOutput) noexcept {
    data.output = alignedOutput;
}

void SetFireFftPalette(FireFftData &data, const std::uint16_t *palette) noexcept {
    data.palette = palette;
}

void SetFireFftSize(FireFftData &data, float width, float height) noexcept {
    data.width = width;
    data.height = height;
}

void SetFireFftScalar(FireFftData &data, float scalar) noexcept {
    data.falloffScalar = scalar;
    GenerateFirePhaseRates(data);
}

void GenerateFireFalloff(float radiusX, float radiusY, std::span<float, 256> destination) noexcept {
    const float exponent = radiusX * 0.5f;
    destination[0] = radiusY * -200.0f;
    for (std::size_t index = 1; index < 16; ++index) {
        const float power = NativePower(static_cast<float>(index), exponent);
        const float value = std::fabs(power) >= kMinimumFloat ? 400.0f / power : 0.0f;
        destination[index * 16] = value;
        destination[index] = value;
    }
    for (std::size_t y = 1; y < 16; ++y) {
        for (std::size_t x = 1; x < 16; ++x) {
            const float distance = std::sqrt(static_cast<float>(y * y + x * x));
            const float power = NativePower(distance, exponent);
            destination[y * 16 + x] = std::fabs(power) >= kMinimumFloat ? 100.0f / power : 0.0f;
        }
    }
}

void ScaleFireFalloff(float gain, std::span<float, 256> falloff) noexcept {
    const float first = falloff[0];
    for (float &value : falloff)
        value *= gain;
    falloff[0] = first;
}

void SetFireFftFalloff(FireFftData &data, std::span<const float, 256> falloff) noexcept {
    std::copy(falloff.begin(), falloff.end(), data.falloff.begin());
}

void UpdateFireFft(FireFftData &data, std::uint16_t flags) noexcept {
    if ((data.flags & activeFlag) == 0)
        return;
    ++data.tick;
    AdvanceFirePhases(data);
    PrepareSpectrum<32, 16, 33>(data.phases.data(), data.falloff.data(), data.real.data(), data.imaginary.data(), data.falloff[0]);
    RunTwoDimensionalFft<32, 33>(data.real.data(), data.imaginary.data());

    const bool duplicateLowerHalf = (flags & 2U) != 0;
    std::uint16_t *output = data.output;
    for (std::size_t y = 0; y < 32; ++y) {
        const std::size_t sourceY = duplicateLowerHalf ? (y >> 1U) + 16U : y;
        const float bias = static_cast<float>(32 - y) * 0.0625f;
        for (std::size_t x = 0; x < 32; ++x) {
            float value = data.real[sourceY * 33 + x] * data.outputScale + bias;
            value = std::clamp(value, 0.0f, 1.0f);
            const std::uint16_t paletteIndex = static_cast<std::uint16_t>(value * 15.0f);
            *output++ = static_cast<std::uint16_t>((paletteIndex << 12U) | data.palette[paletteIndex]);
        }
    }
}
#endif

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t FireTextureSize(std::uint32_t size) {
    switch (size) {
    case 8:
        return 0;
    case 16:
        return 1;
    case 32:
        return 2;
    case 64:
        return 3;
    case 128:
        return 4;
    case 256:
        return 5;
    case 512:
        return 6;
    case 1024:
        return 7;
    default:
        return size;
    }
}
#endif

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
 // namespace

// @verified - d
void SetFireFftOutput(std::int32_t index, std::uint16_t *pixels) {
    FireFftPrefix &effect = *g_fireFftData[index];
    std::uint32_t textures[2] = {0, 0};
    effect.pixels = pixels;
    FireFftSurface *entry = effect.surfaces;
    std::uint32_t size;
    std::uint32_t *texture = textures;
    for (std::int32_t surfaceIndex = 0; surfaceIndex < 2; ++surfaceIndex) {
        kmCreateTextureSurface(&entry->surface, 32, 32, static_cast<KMTEXTURETYPE>(0x102));
        size = FireTextureSize(entry->surface.Width) << 3;
        size |= FireTextureSize(entry->surface.Height);
        *texture = (*texture & 0x7fffffffU) | (entry->surface.fSurfaceFlags << 31);
        *texture = (*texture & 0xbfffffffU) | ((entry->surface.fSurfaceFlags & 8U) << 27);
        *texture = (*texture & 0xc7ffffffU) | (entry->surface.PixelFormat & 0x38000000U);
        *texture = (*texture & 0xfbffffffU) | ((~(entry->surface.fSurfaceFlags & 4U) & 4U) << 24);
        *texture = (*texture & 0xfdffffffU) | ((entry->surface.fSurfaceFlags & 0x10U) << 21);
        *texture = (*texture & 0xfeffffffU) | ((entry->surface.fSurfaceFlags & 2U) << 23);
        *texture = (*texture & 0xff800000U) | (reinterpret_cast<std::uintptr_t>(entry->surface.pSurface) >> 3);
        ++entry;
        ++texture;
    }
    std::uint32_t *context = effect.context;
    *context = 0x8200000eU;
    ++context;
    *context = 0xca800000U;
    ++context;
    *context = 0x941024c0U | size;
    ++context;
    *context = textures[0];
    ++context;
    *context = textures[1];
}


// @verified - d
void SetFireFftPalette(std::int32_t index, const std::uint16_t *palette) {
    g_fireFftData[index]->palette = palette;
}


// @verified - d
void SetFireFftSize(std::int32_t index, float width, float height) {
    FireFftPrefix *const effect = g_fireFftData[index];
    effect->width = width;
    effect->height = height;
}


// @verified - d
float *GenerateFireFalloff(float radiusX, float radiusY, float *falloff) {
    float (*grid)[16] = reinterpret_cast<float (*)[16]>(falloff);
    std::int32_t row, column;
    falloff[0] = radiusY * -200.0f;
    for (row = 1; row < 16; ++row)
        grid[row][0] = 400.0f / fast_powf_approx(static_cast<float>(row), radiusX * 0.5f);
    for (column = 1; column < 16; ++column)
        grid[0][column] = 400.0f / fast_powf_approx(static_cast<float>(column), radiusX * 0.5f);
    for (row = 1; row < 16; ++row) {
        for (column = 1; column < 16; ++column)
            grid[row][column] = 100.0f / fast_powf_approx(::sqrtf(static_cast<float>(row * row + column * column)), radiusX * 0.5f);
    }
    return falloff;
}


// @verified - d
float *ScaleFireFalloff(float gain, float *falloff) {
    float *value = falloff;
    const float first = *falloff;
    for (std::int32_t row = 0; row < 16; ++row)
        for (std::int32_t column = 0; column < 16; ++column) {
            *value *= gain;
            ++value;
        }
    *falloff = first;
    return falloff;
}


// @verified - d
void SetFireFftFalloff(std::int32_t index, const float *falloff) {
    float *destination = g_fireFftData[index]->falloff;
    for (std::int32_t row = 0; row < 16; ++row)
        for (std::int32_t column = 0; column < 16; ++column) {
            *destination = *falloff++;
            ++destination;
        }
}


// @verified - d
void SetFireFftScalar(std::int32_t index, float scalar) {
    FireFftPrefix *effect;
    effect = g_fireFftData[index];
    effect->falloffScalar = scalar;
    GenerateFirePhaseRates(effect->phaseRates, scalar);
}
#endif

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void TwiddleFirePixels(std::uint16_t *pixels) {
    std::uint32_t columnStep = 0x55555556U, rowStep = 0xaaaaaaabU, columnMask = 0xaaaaaaaaU, rowMask = 0x55555555U;
    std::uint16_t twiddled[1024];
    const std::uint16_t *rowPixels = pixels;
    std::uint32_t rowBits = 0;
    std::int32_t index;
    for (index = 0; index < 32; ++index) {
        std::uint32_t columnBits = 0;
        const std::uint16_t *source = rowPixels;
        for (std::int32_t column = 0; column < 32; ++column) {
            twiddled[columnBits | rowBits] = *source++;
            columnBits = (columnBits + columnStep) & columnMask;
        }
        rowPixels += 32;
        rowBits = (rowBits + rowStep) & rowMask;
    }
    for (index = 0; index < 1024; ++index) {
        *pixels = twiddled[index];
        ++pixels;
    }
}


// @verified - d
void ResetFireBillboardAxes() {
    Matrix4x4 matrix;
    matrix_stack::GetMatrix(&matrix);
    matrix.rows[0].x = 1.0f;
    matrix.rows[0].y = 0.0f;
    matrix.rows[0].z = 0.0f;
    matrix.rows[1].x = 0.0f;
    matrix.rows[1].y = 1.0f;
    matrix.rows[1].z = 0.0f;
    matrix.rows[2].x = 0.0f;
    matrix.rows[2].y = 0.0f;
    matrix.rows[2].z = 1.0f;
    matrix_stack::LoadMatrix(&matrix);
}
#endif

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
 // namespace

// @verified - d
void ProjectFireFftPoint(const Vector3 &point, Vector3 &output) {
    Vector4 position;
    matrix_stack::TransformPosition(&point, &position);
    const float inverse = 1.0f / position.w;
    output.x = position.x * inverse;
    output.y = position.y * inverse;
    output.z = inverse;
}


// @verified - d
void UpdateFireFft(std::int32_t index, std::int32_t flags) {
    FireFftPrefix *const effect = g_fireFftData[index];
    if (!effect || !(effect->flags & 0x80000000U))
        return;
    std::uint16_t *const pixels = effect->pixels;
    std::uint16_t *cursor = pixels;
    ++effect->tick;
    UpdateFireSpectrum(effect->phases, effect->phaseRates, effect->real, effect->imaginary, effect->phaseParameter, effect->falloff);
    for (std::int32_t row = 0; row < 32; ++row) {
        for (std::int32_t column = 0; column < 32; ++column) {
            const std::int32_t sourceRow = flags & 2 ? row / 2 + 16 : row;
            float intensity = effect->real[sourceRow][column] * effect->outputScale + static_cast<float>(32 - row) * 0.03125f * 2.0f;
            if (intensity > 1.0f)
                intensity = 1.0f;
            if (intensity < 0.0f)
                intensity = 0.0f;
            const std::int32_t colour = static_cast<std::int32_t>(15.0f * intensity);
            *cursor++ = static_cast<std::uint16_t>((static_cast<std::uint32_t>(colour) << 12) | effect->palette[colour]);
        }
    }
    TwiddleFirePixels(pixels);
    kmLoadTexture(&effect->surfaces[~effect->tick & 1U].surface, reinterpret_cast<std::uint32_t *>(effect->pixels), 0, 0);
}


// @verified - d
void DrawFireFft(std::int32_t index, float alpha) {
    FireFftPrefix *const effect = g_fireFftData[index];
    if (!effect || !(effect->flags & 0x80000000U))
        return;
    const Vector3 points[4] = {{-0.5f, 0.0f, 0.0f}, {-0.5f, 1.0f, 0.0f}, {0.5f, 0.0f, 0.0f}, {0.5f, 1.0f, 0.0f}};
    if (alpha < 0.0f)
        alpha = 0.0f;
    const float scaledAlpha = alpha * 255.0f;
    std::uint32_t opacity = scaledAlpha < 2147483648.0f ? static_cast<std::uint32_t>(TruncateToInt32(scaledAlpha)) :
                                                          static_cast<std::uint32_t>(TruncateToInt32(scaledAlpha - 2147483648.0f)) + 0x80000000U;
    if (opacity > 255U)
        opacity = 255U;
    const std::uint32_t colour = (opacity << 24) | 0xffffffU;
    std::uint32_t *const buffers = dc::sdk::g_sdkVertexBufferPointers;
    if (effect->tick & 1U) {
        std::memcpy(reinterpret_cast<void *>(buffers[2]), effect->context, 16);
        buffers[2] += 32;
    } else {
        std::memcpy(reinterpret_cast<void *>(buffers[2]), effect->context, 12);
        buffers[2] += 12;
        std::memcpy(reinterpret_cast<void *>(buffers[2]), effect->context + 4, 4);
        buffers[2] += 20;
    }
    matrix_stack::Matrix4x4_Push(0);
    ResetFireBillboardAxes();
    Matrix4x4::Scale(effect->width, effect->height, 1.0f);
    PrepareShadowVolumeProjection();
    std::uint32_t *cursor = reinterpret_cast<std::uint32_t *>(buffers[2]);
    for (std::int32_t index = 0; index < 4; ++index) {
        Vector3 point;
        ProjectFireFftPoint(points[index], point);
        cursor[0] = index == 3 ? 0xf0000000U : 0xe0000000U;
        std::memcpy(cursor + 1, &point, 12);
        const float u = index >= 2 ? 1.0f : 0.0f;
        const float v = index & 1 ? 0.949999988079071f : 0.0f;
        std::memcpy(cursor + 4, &u, 4);
        std::memcpy(cursor + 5, &v, 4);
        cursor[6] = colour;
        cursor[7] = 0;
        cursor += 8;
    }
    buffers[2] = reinterpret_cast<std::uintptr_t>(cursor);
    PopCurrentMatrix(1);
}
#endif

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void UpdateFireSpectrum(float *phases, const float *phaseRates, float (*real)[33], float (*imaginary)[33], float phaseParameter, const float *falloff) {
    for (std::int32_t row = 0; row < 16; ++row) {
        for (std::int32_t column = 0; column < 16; ++column) {
            float &first = phases[row * 32 + column], &second = phases[row * 32 + 32 - column];
            if (row >= 8 || column >= 8 || (!row && !column)) {
                first = 0.0f;
                second = 0.0f;
            } else if (!row || !column) {
                first = dc::sdk::kFftPhasePi;
                second = dc::sdk::kFftPhasePi;
            } else {
                first += phaseRates[row * 32 + column];
                second += phaseRates[row * 32 + 32 - column];
            }
        }
    }
    BuildFireSpectrum(phases, real, imaginary, phaseParameter, falloff);
    Vector2 values[32];
    for (std::int32_t row = 0; row < 32; ++row) {
        for (std::int32_t column = 0; column < 32; ++column) {
            values[column].x = real[row][column];
            values[column].y = imaginary[row][column];
        }
        dc::sdk::FftInverse32(values);
        for (std::int32_t column = 0; column < 32; ++column) {
            real[row][column] = values[column].x;
            imaginary[row][column] = values[column].y;
        }
    }
    for (std::int32_t column = 0; column < 32; ++column) {
        for (std::int32_t row = 0; row < 32; ++row) {
            values[row].x = real[row][column];
            values[row].y = imaginary[row][column];
        }
        dc::sdk::FftInverse32(values);
        for (std::int32_t row = 0; row < 32; ++row) {
            real[row][column] = values[row].x;
            imaginary[row][column] = values[row].y;
        }
    }
}


// @verified - d
void InitializeFirePhases(float *phases) {
    float (*grid)[32] = reinterpret_cast<float (*)[32]>(phases);
    for (std::int32_t row = 0; row < 16; ++row) {
        for (std::int32_t column = 0; column < 16; ++column) {
            if (!row && !column)
                continue;
            grid[row][column] = dc::sdk::RandomFloat() * dc::sdk::kFftPhaseTurn;
            grid[row][32 - column] = dc::sdk::RandomFloat() * dc::sdk::kFftPhaseTurn;
        }
    }
}
#endif

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
 // namespace

// @verified - d
void GenerateFirePhaseRates(float *steps, float scalar) {
    float (*grid)[32] = reinterpret_cast<float (*)[32]>(steps);
    for (std::int32_t row = 0; row < 16; ++row) {
        for (std::int32_t column = 0; column < 16; ++column) {
            float distance = ::sqrtf(static_cast<float>(row * row + column * column));
            grid[row][column] = distance * scalar;
            distance = ::sqrtf(static_cast<float>(row * row + column * column));
            grid[row][32 - column] = distance * scalar;
        }
    }
}
#endif

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void BuildFireSpectrum(const float *phases, float (*real)[33], float (*imaginary)[33], float phaseParameter, const float *falloff) {
    real[0][0] = falloff[0];
    imaginary[0][0] = 0.0f;
    real[0][32] = falloff[0];
    imaginary[0][32] = 0.0f;
    real[32][0] = falloff[0];
    imaginary[32][0] = 0.0f;
    real[32][32] = falloff[0];
    imaginary[32][32] = 0.0f;
    for (std::int32_t column = 1; column < 16; ++column) {
        Vector2 values;
        const float amplitude = falloff[column];
        CalculateFixedSinCos(TruncateToInt32((phases[column] * 65536.0f) / dc::sdk::kFftPhaseTurn), &values);
        float realPart = values.y * amplitude, imaginaryPart = values.x * amplitude;
        real[0][column] = realPart;
        imaginary[0][column] = imaginaryPart;
        real[32][32 - column] = realPart;
        imaginary[32][32 - column] = -imaginaryPart;
        CalculateFixedSinCos(TruncateToInt32((phases[32 - column] * 65536.0f) / dc::sdk::kFftPhaseTurn), &values);
        realPart = values.y * amplitude;
        imaginaryPart = values.x * amplitude;
        real[0][32 - column] = realPart;
        imaginary[0][32 - column] = imaginaryPart;
        real[32][column] = realPart;
        imaginary[32][column] = -imaginaryPart;
    }
    for (std::int32_t row = 1; row < 16; ++row) {
        Vector2 values;
        const float amplitude = falloff[row * 16];
        CalculateFixedSinCos(TruncateToInt32((phases[row * 32] * 65536.0f) / dc::sdk::kFftPhaseTurn), &values);
        float realPart = values.y * amplitude, imaginaryPart = values.x * amplitude;
        real[row][0] = realPart;
        imaginary[row][0] = imaginaryPart;
        real[32 - row][32] = realPart;
        imaginary[32 - row][32] = -imaginaryPart;
        CalculateFixedSinCos(TruncateToInt32((phases[row * 32 + 32] * 65536.0f) / dc::sdk::kFftPhaseTurn), &values);
        realPart = values.y * amplitude;
        imaginaryPart = values.x * amplitude;
        real[row][32] = realPart;
        imaginary[row][32] = imaginaryPart;
        real[32 - row][0] = realPart;
        imaginary[32 - row][0] = -imaginaryPart;
    }
    for (std::int32_t row = 1; row < 16; ++row) {
        for (std::int32_t column = 1; column < 16; ++column) {
            Vector2 values;
            const float amplitude = falloff[row * 16 + column];
            CalculateFixedSinCos(TruncateToInt32((phases[row * 32 + column] * 65536.0f) / dc::sdk::kFftPhaseTurn), &values);
            float realPart = values.y * amplitude, imaginaryPart = values.x * amplitude;
            real[row][column] = realPart;
            imaginary[row][column] = imaginaryPart;
            real[32 - row][32 - column] = realPart;
            imaginary[32 - row][32 - column] = -imaginaryPart;
            CalculateFixedSinCos(TruncateToInt32((phases[row * 32 + 32 - column] * 65536.0f) / dc::sdk::kFftPhaseTurn), &values);
            realPart = values.y * amplitude;
            imaginaryPart = values.x * amplitude;
            real[row][32 - column] = realPart;
            imaginary[row][32 - column] = imaginaryPart;
            real[32 - row][column] = realPart;
            imaginary[32 - row][column] = -imaginaryPart;
        }
    }
}
#endif

} // namespace

} // namespace s2

} // namespace shenmue
