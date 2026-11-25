#pragma once

#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include <cstddef>
#include <cstdint>
#include "platform/dc/ninja.hpp"
namespace shenmue {
namespace s2 {
struct FireFftSurface {
    KMSURFACEDESC surface;
    std::byte retained20[8];
};
struct FireFftPrefix {
    std::uint32_t flags;
    float width, height;
    std::uint16_t *pixels;
    const std::uint16_t *palette;
    FireFftSurface surfaces[2];
    std::uint32_t context[5];
    std::uint32_t tick;
    float phaseParameter, outputScale, falloffScalar;
    float falloff[256];
    std::byte retained488[0x80];
    float phases[1024], phaseRates[1024];
    float real[33][33], imaginary[33][33];
};
static const std::uint16_t kFireFftDefaultPalette[16] = {
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
typedef char FireFftSurfaceSizeCheck[(sizeof(FireFftSurface) == 0x28) ? 1 : -1];
typedef char FireFftSurfaceOffsetCheck[(offsetof(FireFftPrefix, surfaces) == 0x14) ? 1 : -1];
typedef char FireFftContextOffsetCheck[(offsetof(FireFftPrefix, context) == 0x64) ? 1 : -1];
typedef char Layout_effect_fft_0[(sizeof(FireFftPrefix) == 0x4710) ? 1 : -1];
typedef char Layout_effect_fft_1[(offsetof(FireFftPrefix, width) == 4) ? 1 : -1];
typedef char Layout_effect_fft_2[(offsetof(FireFftPrefix, height) == 8) ? 1 : -1];
typedef char Layout_effect_fft_3[(offsetof(FireFftPrefix, pixels) == 0x0c) ? 1 : -1];
typedef char Layout_effect_fft_4[(offsetof(FireFftPrefix, palette) == 0x10) ? 1 : -1];
typedef char Layout_effect_fft_5[(offsetof(FireFftPrefix, tick) == 0x78) ? 1 : -1];
typedef char Layout_effect_fft_6[(offsetof(FireFftPrefix, phaseParameter) == 0x7c) ? 1 : -1];
typedef char Layout_effect_fft_7[(offsetof(FireFftPrefix, outputScale) == 0x80) ? 1 : -1];
typedef char Layout_effect_fft_8[(offsetof(FireFftPrefix, falloffScalar) == 0x84) ? 1 : -1];
typedef char Layout_effect_fft_9[(offsetof(FireFftPrefix, falloff) == 0x88) ? 1 : -1];
typedef char Layout_effect_fft_10[(offsetof(FireFftPrefix, phases) == 0x508) ? 1 : -1];
typedef char Layout_effect_fft_11[(offsetof(FireFftPrefix, phaseRates) == 0x1508) ? 1 : -1];
typedef char Layout_effect_fft_12[(offsetof(FireFftPrefix, real) == 0x2508) ? 1 : -1];
typedef char Layout_effect_fft_13[(offsetof(FireFftPrefix, imaginary) == 0x360c) ? 1 : -1];
extern FireFftPrefix *g_fireFftData[8];
void InitializeFireFft(std::int32_t index, FireFftPrefix *effect);
void SetFireFftOutput(std::int32_t index, std::uint16_t *pixels);
void UpdateFireFft(std::int32_t index, std::int32_t flags);
void DrawFireFft(std::int32_t index, float alpha);
void ResetFireFftChannels();
void SetFireFftActive(std::int32_t index, std::int32_t active);
void SetFireFftSize(std::int32_t index, float width, float height);
void SetFireFftPalette(std::int32_t index, const std::uint16_t *palette);
float *GenerateFireFalloff(float radiusX, float radiusY, float *falloff);
float *ScaleFireFalloff(float gain, float *falloff);
void SetFireFftFalloff(std::int32_t index, const float *falloff);
void GenerateFirePhaseRates(float *steps, float scalar);
void SetFireFftScalar(std::int32_t index, float scalar);
float *GenerateDistanceFalloff(float exponent, float *output);
float *GenerateDistanceKernel(float exponent, float *output);
} // namespace s2
} // namespace shenmue
#endif
#else

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

struct SmokeFftData {
    std::uint32_t flags{}; // +0x000
    std::uint32_t reserved04{};
    std::byte *outputAllocation{}; // +0x008
    std::uint16_t packedColour{};  // +0x010
    std::uint16_t reserved12{};
    float phaseParameter{3.0f};      // +0x014
    float outputScale{1.0f};         // +0x018
    float phaseRateScale{0.5f};      // +0x01c
    std::array<float, 64> falloff{}; // +0x020
    std::array<std::byte, 0x40> reserved120{};
    std::array<float, 256> phases{};     // +0x160
    std::array<float, 256> phaseRates{}; // +0x560
    std::array<float, 289> real{};       // +0x960
    std::array<float, 289> imaginary{};  // +0xde4
};

struct FireFftGraphicsRecord {
    std::array<std::byte, 0x30> storage{};
};

static_assert(sizeof(FireFftGraphicsRecord) == 0x30);

struct FireFftData {
    std::uint32_t flags{}; // +0x000
    float width{1.0f};     // +0x004
    float height{1.0f};    // +0x008
    std::uint32_t reserved0c{};
    std::uint16_t *output{};                 // +0x010
    const std::uint16_t *palette{};          // +0x018
    FireFftGraphicsRecord graphicsRecord0{}; // +0x020
    FireFftGraphicsRecord graphicsRecord1{}; // +0x050
    std::array<std::byte, 0x14> reserved80{};
    std::uint32_t tick{};              // +0x094
    float phaseParameter{3.0f};        // +0x098
    float outputScale{1.0f};           // +0x09c
    float falloffScalar{-0.69999999f}; // +0x0a0
    std::array<float, 256> falloff{};  // +0x0a4
    std::array<std::byte, 0x80> reserved4a4{};
    std::array<float, 1024> phases{};     // +0x524
    std::array<float, 1024> phaseRates{}; // +0x1524
    std::array<float, 1089> real{};       // +0x2524
    std::array<float, 1089> imaginary{};  // +0x3628
    std::uint32_t reserved472c{};
};

static_assert(sizeof(SmokeFftData) == 0x1268);
static_assert(offsetof(SmokeFftData, outputAllocation) == 0x008);
static_assert(offsetof(SmokeFftData, packedColour) == 0x010);
static_assert(offsetof(SmokeFftData, phaseParameter) == 0x014);
static_assert(offsetof(SmokeFftData, outputScale) == 0x018);
static_assert(offsetof(SmokeFftData, phaseRateScale) == 0x01c);
static_assert(offsetof(SmokeFftData, falloff) == 0x020);
static_assert(offsetof(SmokeFftData, phases) == 0x160);
static_assert(offsetof(SmokeFftData, phaseRates) == 0x560);
static_assert(offsetof(SmokeFftData, real) == 0x960);
static_assert(offsetof(SmokeFftData, imaginary) == 0xde4);

static_assert(sizeof(FireFftData) == 0x4730);
static_assert(offsetof(FireFftData, output) == 0x010);
static_assert(offsetof(FireFftData, width) == 0x004);
static_assert(offsetof(FireFftData, height) == 0x008);
static_assert(offsetof(FireFftData, palette) == 0x018);
static_assert(offsetof(FireFftData, graphicsRecord0) == 0x020);
static_assert(offsetof(FireFftData, graphicsRecord1) == 0x050);
static_assert(offsetof(FireFftData, tick) == 0x094);
static_assert(offsetof(FireFftData, falloff) == 0x0a4);
static_assert(offsetof(FireFftData, phaseParameter) == 0x098);
static_assert(offsetof(FireFftData, outputScale) == 0x09c);
static_assert(offsetof(FireFftData, falloffScalar) == 0x0a0);
static_assert(offsetof(FireFftData, phases) == 0x524);
static_assert(offsetof(FireFftData, phaseRates) == 0x1524);
static_assert(offsetof(FireFftData, real) == 0x2524);
static_assert(offsetof(FireFftData, imaginary) == 0x3628);

void InitializeSmokeFft(SmokeFftData &data) noexcept;
void SetSmokeFftOutput(SmokeFftData &data, std::byte *raw0x49fAllocation) noexcept;
void SetSmokeFftColour(SmokeFftData &data, std::uint16_t colour) noexcept;
void SetSmokeFftAmplitude(SmokeFftData &data, float amplitude) noexcept;
void UpdateSmokeFft(SmokeFftData &data) noexcept;

void GenerateSmokeFalloff(float radius, std::span<float, 64> destination) noexcept;
void ScaleSmokeFalloff(float gain, std::span<float, 64> falloff) noexcept;
void SetSmokeFftFalloff(SmokeFftData &data, std::span<const float, 64> falloff) noexcept;

void InitializeFireFft(FireFftData &data) noexcept;
void SetFireFftOutput(FireFftData &data, std::uint16_t *alignedOutput) noexcept;
void SetFireFftPalette(FireFftData &data, const std::uint16_t *palette) noexcept;
void SetFireFftSize(FireFftData &data, float width, float height) noexcept;
void SetFireFftScalar(FireFftData &data, float scalar) noexcept;
void UpdateFireFft(FireFftData &data, std::uint16_t flags) noexcept;

void GenerateFireFalloff(float radiusX, float radiusY, std::span<float, 256> destination) noexcept;
void ScaleFireFalloff(float gain, std::span<float, 256> falloff) noexcept;
void SetFireFftFalloff(FireFftData &data, std::span<const float, 256> falloff) noexcept;

} // namespace shenmue::s2

#endif
