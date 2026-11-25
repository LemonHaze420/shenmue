#include "platform/dc/runtime_arithmetic.hpp"

#if SHENMUE_IS_DC

#include <cerrno>

namespace shenmue { namespace dc {

namespace {

std::int32_t CalculateSignedArithmetic(std::int32_t numerator, std::int32_t denominator, bool modulo) {
    if (!denominator) {
        errno = 0x44e;
        return 0;
    }
    const std::uint32_t dividend = numerator < 0 ? 0U - static_cast<std::uint32_t>(numerator) : static_cast<std::uint32_t>(numerator);
    const std::uint32_t divisor = denominator < 0 ? 0U - static_cast<std::uint32_t>(denominator) : static_cast<std::uint32_t>(denominator);
    std::uint32_t quotient = 0;
    std::uint32_t remainder = 0;
    for (std::int32_t bit = 31; bit >= 0; --bit) {
        const bool carry = remainder >> 31;
        remainder = (remainder << 1) | ((dividend >> bit) & 1U);
        if (carry || remainder >= divisor) {
            remainder -= divisor;
            quotient |= 1U << bit;
        }
    }
    if (modulo)
        return static_cast<std::int32_t>(numerator < 0 ? 0U - remainder : remainder);
    return static_cast<std::int32_t>((numerator < 0) != (denominator < 0) ? 0U - quotient : quotient);
}

} // namespace

std::int32_t SignedDivide(std::int32_t numerator, std::int32_t denominator) {
    return CalculateSignedArithmetic(numerator, denominator, false);
}

std::int32_t SignedModulo(std::int32_t numerator, std::int32_t denominator) {
    return CalculateSignedArithmetic(numerator, denominator, true);
}

} } // namespace shenmue::dc

#endif
