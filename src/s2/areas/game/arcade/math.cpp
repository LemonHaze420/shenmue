#include "s2/areas/game/arcade/math.hpp"

#include <bit>
#include <cstdint>

namespace shenmue::s2 {
namespace {

void DivideArcadeMathDevice(ArcadeMathDevice &device) {
    const std::uint32_t dividendBits = (static_cast<std::uint32_t>(static_cast<std::uint16_t>(device.dividendHigh)) << 16U) | device.dividendLow;
    const std::int32_t dividend = std::bit_cast<std::int32_t>(dividendBits);
    const std::int32_t divisor = device.divisor;
    std::uint32_t quotient;
    std::int32_t remainder;
    std::uint16_t overflow = 0;
    std::uint16_t divideByZero = 0;
    if (divisor != 0) {
        remainder = dividend % divisor;
        quotient = static_cast<std::uint32_t>(dividend / divisor);
        if (quotient + 0x8000U > 0xffffU)
            overflow = 1;
    } else {
        divideByZero = 1;
        quotient = device.dividendLow;
        remainder = 0;
        overflow = dividendBits + 0x8000U > 0xffffU ? 1 : 0;
    }
    if (overflow != 0) {
        const std::uint32_t product = dividendBits * static_cast<std::uint32_t>(divisor);
        quotient = (product & 0x80000000U) != 0 ? 0xffff8000U : 0x7fffU;
        remainder = 0;
    }
    device.quotient = static_cast<std::uint16_t>(quotient);
    device.remainder = static_cast<std::uint16_t>(remainder);
    device.divisionStatus = static_cast<std::uint16_t>((divideByZero | (overflow * 2U)) << 14U);
}

void CompareArcadeMathDevice(ArcadeMathDevice &device) {
    const std::int16_t lower = device.bound0 <= device.bound1 ? device.bound0 : device.bound1;
    const std::int16_t upper = device.bound0 <= device.bound1 ? device.bound1 : device.bound0;
    std::uint16_t status = 0;
    std::int16_t clamped = device.compareValue;
    if (device.compareValue < lower) {
        status = 0x8000;
        clamped = lower;
    } else if (device.compareValue > upper) {
        status = 0x4000;
        clamped = upper;
    }
    device.compareStatus = status;
    device.clampedValue = clamped;
}

} // namespace

std::uint32_t ReadArcadeMathValue(const ArcadeMathDevice &device, std::uint32_t address) {
    switch (address & 0xc000U) {
    case 0:
        switch (address & 6U) {
        case 0:
            return static_cast<std::uint16_t>(device.multiplier);
        case 2:
            return static_cast<std::uint16_t>(device.multiplicand);
        case 4: {
            const std::int32_t product = static_cast<std::int32_t>(device.multiplier) * device.multiplicand;
            return static_cast<std::uint16_t>(static_cast<std::uint32_t>(product) >> 16U);
        }
        case 6:
            return static_cast<std::uint32_t>(static_cast<std::uint16_t>(device.multiplier)) * static_cast<std::uint16_t>(device.multiplicand);
        }
        break;
    case 0x4000:
        switch (address & 0xeU) {
        case 0:
            return static_cast<std::uint16_t>(device.dividendHigh);
        case 2:
            return device.dividendLow;
        case 4:
            return static_cast<std::uint16_t>(device.divisor);
        case 6:
        case 0xe:
            return 0xffff;
        case 8:
            return device.quotient;
        case 0xa:
            return device.remainder;
        case 0xc:
            return device.divisionStatus;
        }
        break;
    case 0x8000:
        switch (address & 0x1eU) {
        case 0:
            return static_cast<std::uint16_t>(device.bound0);
        case 2:
        case 0xa:
            return static_cast<std::uint16_t>(device.bound1);
        case 4:
        case 0xc:
            return static_cast<std::uint16_t>(device.compareValue);
        case 6:
            return device.compareStatus;
        case 8:
            return 0;
        case 0xe:
            return static_cast<std::uint16_t>(device.clampedValue);
        default:
            return 0xff;
        }
    }

    return 0;
}

void WriteArcadeMathWord(ArcadeMathDevice &device, std::uint32_t address, std::uint16_t value) {
    const std::int16_t signedValue = std::bit_cast<std::int16_t>(value);
    switch (address & 0xc000U) {
    case 0:
        if ((address & 2U) != 0)
            device.multiplicand = signedValue;
        else
            device.multiplier = signedValue;
        return;
    case 0x4000:
        switch (address & 6U) {
        case 0: {
            const bool signChanged = ((value ^ static_cast<std::uint16_t>(device.dividendHigh)) & 0x8000U) != 0;
            device.dividendHigh = signedValue;
            if (signChanged)
                device.quotient = static_cast<std::uint16_t>((0U - device.quotient) & 0xfffeU);
            return;
        }
        case 2:
            device.dividendLow = value;
            break;
        case 4: {
            const bool signChanged = ((value ^ static_cast<std::uint16_t>(device.divisor)) & 0x8000U) != 0;
            device.divisor = signedValue;
            if (signChanged)
                device.quotient = static_cast<std::uint16_t>((0U - device.quotient) & 0xfffeU);
            break;
        }
        case 6:
            return;
        }
        if ((address & 0x10U) != 0)
            DivideArcadeMathDevice(device);
        return;
    case 0x8000:
        switch (address & 0x1eU) {
        case 0:
            device.bound0 = signedValue;
            break;
        case 2:
            device.bound1 = signedValue;
            break;
        case 4:
        case 0xc:
            device.compareValue = signedValue;
            break;
        default:
            return;
        }
        CompareArcadeMathDevice(device);
        return;
    }

}

} // namespace shenmue::s2
