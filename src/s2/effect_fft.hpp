#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

struct SmokeFftData {
    std::uint32_t flags{};
    std::uint32_t reserved04{};
    std::byte *outputAllocation{};
    std::uint16_t packedColour{};
    std::uint16_t reserved12{};
    float phaseParameter{3.0f};
    float outputScale{1.0f};
    float phaseRateScale{0.5f};
    std::array<float, 64> falloff{};
    std::array<std::byte, 0x40> reserved120{};
    std::array<float, 256> phases{};
    std::array<float, 256> phaseRates{};
    std::array<float, 289> real{};
    std::array<float, 289> imaginary{};
};

struct FireFftGraphicsRecord {
    std::array<std::byte, 0x30> storage{};
};

static_assert(sizeof(FireFftGraphicsRecord) == 0x30);

struct FireFftData {
    std::uint32_t flags{};
    float width{1.0f};
    float height{1.0f};
    std::uint32_t reserved0c{};
    std::uint16_t *output{};
    const std::uint16_t *palette{};
    FireFftGraphicsRecord graphicsRecord0{};
    FireFftGraphicsRecord graphicsRecord1{};
    std::array<std::byte, 0x14> reserved80{};
    std::uint32_t tick{};
    float phaseParameter{3.0f};
    float outputScale{1.0f};
    float falloffScalar{-0.69999999f};
    std::array<float, 256> falloff{};
    std::array<std::byte, 0x80> reserved4a4{};
    std::array<float, 1024> phases{};
    std::array<float, 1024> phaseRates{};
    std::array<float, 1089> real{};
    std::array<float, 1089> imaginary{};
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
