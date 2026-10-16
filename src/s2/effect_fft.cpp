#include "s2/effect_fft.hpp"
#include "s2/particle_effects.hpp"

#include "s2/motion_work.hpp"
#include "common/math.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
namespace {

constexpr float kPi = 3.1415927f;
constexpr float kTau = 6.2831855f;
constexpr float kMinimumFloat = 0.00000011920929f;
constexpr float kFixedAngleScale = 10430.378f;
constexpr std::size_t kSmokeOutputOffset = 0x80;

constexpr std::array<std::uint16_t, 16> kDefaultFirePalette{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};

constexpr std::array<std::uint8_t, 64> kSmokePhaseControl{
    0, 1, 1, 1, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2};

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
void PrepareSpectrum(const float *phases, const float *falloff, float *real, float *imaginary) noexcept {
    std::fill_n(real, Padded * Padded, 0.0f);
    std::fill_n(imaginary, Padded * Padded, 0.0f);

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

    real[0] = 0.0f;
    imaginary[0] = 0.0f;
    real[N] = 0.0f;
    imaginary[N] = 0.0f;
    real[N * Padded] = 0.0f;
    imaginary[N * Padded] = 0.0f;
    real[N * Padded + N] = 0.0f;
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

} // namespace

void InitializeSmokeFft(SmokeFftData &data) noexcept {
    data.flags &= ~activeFlag;
    data.phaseParameter = 3.0f;
    data.outputScale = 1.0f;
    data.phaseRateScale = 0.5f;
    GenerateNative32PointTwiddles();
    GenerateSmokePhases(data);
    GenerateSmokePhaseRates(data);
    PrepareSpectrum<16, 8, 17>(data.phases.data(), data.falloff.data(), data.real.data(), data.imaginary.data());
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
    PrepareSpectrum<16, 8, 17>(data.phases.data(), data.falloff.data(), data.real.data(), data.imaginary.data());
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
    PrepareSpectrum<32, 16, 33>(data.phases.data(), data.falloff.data(), data.real.data(), data.imaginary.data());
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
    PrepareSpectrum<32, 16, 33>(data.phases.data(), data.falloff.data(), data.real.data(), data.imaginary.data());
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

} // namespace shenmue::s2
