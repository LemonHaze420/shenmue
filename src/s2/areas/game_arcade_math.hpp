#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

#pragma pack(push, 2)
struct ArcadeMathDevice {
    std::int16_t multiplier;
    std::int16_t multiplicand;
    std::int16_t dividendHigh;
    std::uint16_t dividendLow;
    std::int16_t divisor;
    std::uint16_t quotient;
    std::uint16_t remainder;
    std::uint16_t divisionStatus;
    std::int16_t bound0;
    std::int16_t bound1;
    std::int16_t compareValue;
    std::uint16_t compareStatus;
    std::int16_t clampedValue;
};
#pragma pack(pop)
static_assert(sizeof(ArcadeMathDevice) == 0x1a);
static_assert(offsetof(ArcadeMathDevice, dividendHigh) == 4);
static_assert(offsetof(ArcadeMathDevice, quotient) == 0x0a);
static_assert(offsetof(ArcadeMathDevice, divisionStatus) == 0x0e);
static_assert(offsetof(ArcadeMathDevice, bound0) == 0x10);
static_assert(offsetof(ArcadeMathDevice, clampedValue) == 0x18);

std::uint32_t ReadArcadeMathValue(const ArcadeMathDevice &device, std::uint32_t address);
void WriteArcadeMathWord(ArcadeMathDevice &device, std::uint32_t address, std::uint16_t value);

} // namespace shenmue::s2
