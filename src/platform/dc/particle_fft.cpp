#include "platform/dc/particle_effects.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#include "common/types.hpp"
#include <cmath>

namespace shenmue {
namespace dc {
namespace sdk {

#if SHENMUE_IS_S2
namespace {
std::uint64_t ShiftFftSignificand(std::uint64_t value, std::int32_t shift) {
    if (shift <= 0)
        return value;
    if (shift >= 64)
        return value != 0;
    return (value >> shift) | ((value << (64 - shift)) != 0);
}
} // namespace

// Keep the exact product and aligned addend until the final binary32 rounding.
float FftMultiplyAdd(float first, float second, float addend) {
    const std::uint32_t firstBits = floatAsU32(first), secondBits = floatAsU32(second), addendBits = floatAsU32(addend);
    const std::uint32_t firstMagnitude = firstBits & 0x7fffffffU, secondMagnitude = secondBits & 0x7fffffffU;
    const std::uint32_t addendMagnitude = addendBits & 0x7fffffffU;
    std::uint32_t sign = (firstBits ^ secondBits) & 0x80000000U;
    if (firstMagnitude > 0x7f800000U || secondMagnitude > 0x7f800000U || addendMagnitude > 0x7f800000U)
        return first * second + addend;
    if (firstMagnitude == 0x7f800000U || secondMagnitude == 0x7f800000U) {
        if (!firstMagnitude || !secondMagnitude || (addendMagnitude == 0x7f800000U && sign != (addendBits & 0x80000000U)))
            return u32AsFloat(0x7fc00000U);
        return u32AsFloat(sign | 0x7f800000U);
    }
    if (addendMagnitude == 0x7f800000U)
        return addend;
    if (!firstMagnitude || !secondMagnitude)
        return u32AsFloat(sign) + addend;
    std::uint64_t product = static_cast<std::uint64_t>((firstMagnitude & 0x7fffffU) | (firstMagnitude >= 0x800000U ? 0x800000U : 0)) *
        ((secondMagnitude & 0x7fffffU) | (secondMagnitude >= 0x800000U ? 0x800000U : 0));
    std::int32_t exponent = static_cast<std::int32_t>(firstMagnitude >> 23) + static_cast<std::int32_t>(secondMagnitude >> 23) - 300;
    if (firstMagnitude < 0x800000U)
        ++exponent;
    if (secondMagnitude < 0x800000U)
        ++exponent;
    while (!(product & (static_cast<std::uint64_t>(1) << 61))) {
        product <<= 1;
        --exponent;
    }
    if (addendMagnitude) {
        std::uint64_t addendSignificand = (addendMagnitude & 0x7fffffU) | (addendMagnitude >= 0x800000U ? 0x800000U : 0);
        std::int32_t addendExponent = static_cast<std::int32_t>(addendMagnitude >> 23) - 150;
        if (addendMagnitude < 0x800000U)
            ++addendExponent;
        while (!(addendSignificand & (static_cast<std::uint64_t>(1) << 61))) {
            addendSignificand <<= 1;
            --addendExponent;
        }
        if (exponent < addendExponent) {
            product = ShiftFftSignificand(product, addendExponent - exponent);
            exponent = addendExponent;
        } else {
            addendSignificand = ShiftFftSignificand(addendSignificand, exponent - addendExponent);
        }
        if (sign == (addendBits & 0x80000000U)) {
            product += addendSignificand;
        } else if (product >= addendSignificand) {
            product -= addendSignificand;
        } else {
            product = addendSignificand - product;
            sign = addendBits & 0x80000000U;
        }
    }
    if (!product)
        return 0.0f;
    while (!(product & (static_cast<std::uint64_t>(3) << 61))) {
        product <<= 1;
        --exponent;
    }
    if (product & (static_cast<std::uint64_t>(1) << 62)) {
        product = ShiftFftSignificand(product, 1);
        ++exponent;
    }
    std::int32_t encodedExponent = exponent + 188;
    if (encodedExponent <= 0) {
        product = ShiftFftSignificand(product, 1 - encodedExponent);
        encodedExponent = 1;
    }
    const std::uint64_t mask = (static_cast<std::uint64_t>(1) << 38) - 1;
    const std::uint64_t remainder = product & mask;
    std::uint32_t significand = static_cast<std::uint32_t>(product >> 38);
    const std::uint64_t halfway = static_cast<std::uint64_t>(1) << 37;
    if (remainder > halfway || (remainder == halfway && (significand & 1U)))
        ++significand;
    if (significand >= 0x1000000U) {
        significand >>= 1;
        ++encodedExponent;
    }
    if (encodedExponent >= 255)
        return u32AsFloat(sign | 0x7f800000U);
    if (significand < 0x800000U)
        encodedExponent = 0;
    return u32AsFloat(sign | (static_cast<std::uint32_t>(encodedExponent) << 23) | (significand & 0x7fffffU));
}
#endif

// @verified - d
void FftReverse16(Vector2 *values) {
    {
        const float firstX = values[1].x, firstY = values[1].y, secondX = values[8].x, secondY = values[8].y;
        values[8].x = firstX;
        values[8].y = firstY;
        values[1].x = secondX;
        values[1].y = secondY;
    }
    {
        const float firstX = values[2].x, firstY = values[2].y, secondX = values[4].x, secondY = values[4].y;
        values[4].x = firstX;
        values[4].y = firstY;
        values[2].x = secondX;
        values[2].y = secondY;
    }
    {
        const float firstX = values[3].x, firstY = values[3].y, secondX = values[12].x, secondY = values[12].y;
        values[12].x = firstX;
        values[12].y = firstY;
        values[3].x = secondX;
        values[3].y = secondY;
    }
    {
        const float firstX = values[5].x, firstY = values[5].y, secondX = values[10].x, secondY = values[10].y;
        values[10].x = firstX;
        values[10].y = firstY;
        values[5].x = secondX;
        values[5].y = secondY;
    }
    {
        const float firstX = values[7].x, firstY = values[7].y, secondX = values[14].x, secondY = values[14].y;
        values[14].x = firstX;
        values[14].y = firstY;
        values[7].x = secondX;
        values[7].y = secondY;
    }
    {
        const float firstX = values[11].x, firstY = values[11].y, secondX = values[13].x, secondY = values[13].y;
        values[13].x = firstX;
        values[13].y = firstY;
        values[11].x = secondX;
        values[11].y = secondY;
    }
}

// @verified - d
void FftInverseHalves(Vector2 *values, std::int32_t logSize) {
#if SHENMUE_S2_GCC295 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const std::int32_t size = static_cast<std::int16_t>(1 << logSize);
    const std::int16_t quarter = static_cast<std::int16_t>(size >> 2);
    Vector2 *upper = values + quarter;
    std::int16_t remaining = quarter;
    do {
        const float lowerX = values->x, lowerY = values->y;
        const float upperX = upper->x, upperY = upper->y;
        const float differenceX = lowerX - upperX;
        const float differenceY = lowerY - upperY;
        values->y = lowerY + upperY;
        values->x = lowerX + upperX;
        upper->y = differenceY;
        upper->x = differenceX;
        ++values;
        ++upper;
    } while (--remaining);
    values = upper;
    upper += quarter;
    remaining = quarter;
    do {
        const float lowerX = values->x, lowerY = values->y;
        const float upperX = upper->x, upperY = upper->y;
        const float differenceY = lowerX - upperX;
        const float differenceX = upperY - lowerY;
        values->y = lowerY + upperY;
        values->x = lowerX + upperX;
        upper->y = differenceY;
        upper->x = differenceX;
        ++values;
        ++upper;
    } while (--remaining);
#else
    const std::int16_t quarter = static_cast<std::int16_t>(static_cast<std::int16_t>(1 << logSize) >> 2);
    std::int16_t index = 0;
    do {
        const Vector2 first = values[index];
        const Vector2 second = values[quarter + index];
        values[index].y = first.y + second.y;
        values[index].x = first.x + second.x;
        values[quarter + index].y = first.y - second.y;
        values[quarter + index].x = first.x - second.x;
        ++index;
    } while (index != quarter);
    index = 0;
    do {
        const Vector2 first = values[quarter * 2 + index];
        const Vector2 second = values[quarter * 3 + index];
        values[quarter * 2 + index].y = first.y + second.y;
        values[quarter * 2 + index].x = first.x + second.x;
        values[quarter * 3 + index].y = first.x - second.x;
        values[quarter * 3 + index].x = second.y - first.y;
        ++index;
    } while (index != quarter);
#endif
}

#if SHENMUE_IS_S2
void FftForwardHalves(Vector2 *values, std::int32_t logSize) {
    const std::int32_t size = static_cast<std::int16_t>(1 << logSize);
    const std::int16_t quarter = static_cast<std::int16_t>(size >> 2);
    Vector2 *upper = values + quarter;
    std::int16_t remaining = quarter;
    do {
        const float lowerX = values->x, lowerY = values->y;
        const float upperX = upper->x, upperY = upper->y;
        const float differenceX = lowerX - upperX;
        const float differenceY = lowerY - upperY;
        values->y = lowerY + upperY;
        values->x = lowerX + upperX;
        upper->y = differenceY;
        upper->x = differenceX;
        ++values;
        ++upper;
    } while (--remaining);
    values = upper;
    upper += quarter;
    remaining = quarter;
    do {
        const float lowerX = values->x, lowerY = values->y;
        const float upperX = upper->x, upperY = upper->y;
        const float differenceY = upperX - lowerX;
        const float differenceX = lowerY - upperY;
        values->y = lowerY + upperY;
        values->x = lowerX + upperX;
        upper->y = differenceY;
        upper->x = differenceX;
        ++values;
        ++upper;
    } while (--remaining);
}

void FftForwardFinal(Vector2 *values, std::int32_t logSize) {
    const std::uint32_t size = 1U << logSize;
    Vector2 *upper = values + (size >> 1);
    std::uint32_t groups = size >> 2;
    do {
        float lowerX = values->x, lowerY = values->y;
        const float upperX = upper->x, upperY = upper->y;
        const float differenceX = lowerX - upperX;
        const float differenceY = lowerY - upperY;
        lowerX = lowerX + upperX;
        lowerY = lowerY + upperY;
        float nextLowerX = values[1].x, nextLowerY = values[1].y;
        const float nextUpperX = upper[1].x, nextUpperY = upper[1].y;
        upper->y = differenceY;
        upper->x = differenceX;
        values->y = lowerY;
        values->x = lowerX;
        const float nextDifferenceX = nextLowerX - nextUpperX;
        const float nextDifferenceY = nextLowerY - nextUpperY;
        nextLowerX = nextLowerX + nextUpperX;
        nextLowerY = nextLowerY + nextUpperY;
        upper[1].y = nextDifferenceY;
        upper[1].x = nextDifferenceX;
        values[1].y = nextLowerY;
        values[1].x = nextLowerX;
        values += 2;
        upper += 2;
    } while (--groups);
}
#endif

// @verified - d
void FftInverseScale(Vector2 *values, std::int32_t logSize, const float *scale) {
#if SHENMUE_S2_GCC295 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const float factor = *scale;
    const std::uint32_t size = 1U << logSize;
    Vector2 *upper = values + (size >> 1);
    std::uint32_t groups = size >> 2;
    do {
        float lowerX = values->x, lowerY = values->y;
        const float upperX = upper->x, upperY = upper->y;
        const float differenceX = (lowerX - upperX) * factor;
        const float differenceY = (lowerY - upperY) * factor;
        lowerX = (lowerX + upperX) * factor;
        lowerY = (lowerY + upperY) * factor;
        float nextLowerX = values[1].x, nextLowerY = values[1].y;
        const float nextUpperX = upper[1].x, nextUpperY = upper[1].y;
        upper->y = differenceY;
        upper->x = differenceX;
        values->y = lowerY;
        values->x = lowerX;
        const float nextDifferenceX = (nextLowerX - nextUpperX) * factor;
        const float nextDifferenceY = (nextLowerY - nextUpperY) * factor;
        nextLowerX = (nextLowerX + nextUpperX) * factor;
        nextLowerY = (nextLowerY + nextUpperY) * factor;
        upper[1].y = nextDifferenceY;
        upper[1].x = nextDifferenceX;
        values[1].y = nextLowerY;
        values[1].x = nextLowerX;
        values += 2;
        upper += 2;
    } while (--groups);
#else
    const std::uint32_t half = (1U << logSize) >> 1;
    const float factor = *scale;
    std::uint32_t group = 0;
    do {
        const std::uint32_t index = group * 2;
        const Vector2 first = values[index];
        const Vector2 second = values[half + index];
        const Vector2 third = values[index + 1];
        const Vector2 fourth = values[half + index + 1];
        values[half + index].y = (first.y - second.y) * factor;
        values[half + index].x = (first.x - second.x) * factor;
        values[index].y = (first.y + second.y) * factor;
        values[index].x = (first.x + second.x) * factor;
        values[half + index + 1].y = (third.y - fourth.y) * factor;
        values[half + index + 1].x = (third.x - fourth.x) * factor;
        values[index + 1].y = (third.y + fourth.y) * factor;
        values[index + 1].x = (third.x + fourth.x) * factor;
        ++group;
    } while (group != half / 2);
#endif
}

#if SHENMUE_IS_S2
static const float kFftInverse8Scale = 0.125f;
void FftReverse8(Vector2 *values);

void FftForward8(Vector2 *values) {
    FftReverse8(values);
    FftRadix4(values, 3, g_fftForwardTwiddles);
    FftForwardFinal(values, 3);
}

void FftInverse8(Vector2 *values) {
    FftReverse8(values);
    FftRadix4(values, 3, g_fftBackwardTwiddles);
    FftInverseScale(values, 3, &kFftInverse8Scale);
}

void FftForward16(Vector2 *values) {
    FftReverse16(values);
    FftRadix4(values, 4, g_fftForwardTwiddles);
    FftForwardHalves(values, 4);
    FftForwardFinal(values, 4);
}

void FftReverseBits(Vector2 *values, std::int32_t size) {
    const std::int32_t half = size >> 1;
    const std::uint32_t high = half, odd = half + 1, quarter = half >> 1;
    std::uint32_t reversed = 0;
    for (std::uint32_t index = 0; index < static_cast<std::uint32_t>(half - 1); index += 2) {
        float x = values[index + 1].x, y = values[index + 1].y;
        float otherX = values[reversed | high].x, otherY = values[reversed | high].y;
        values[reversed | high].x = x;
        values[reversed | high].y = y;
        values[index + 1].x = otherX;
        values[index + 1].y = otherY;
        if (index < reversed) {
            x = values[index].x;
            y = values[index].y;
            otherX = values[reversed].x;
            otherY = values[reversed].y;
            values[reversed].x = x;
            values[reversed].y = y;
            values[index].x = otherX;
            values[index].y = otherY;
            x = values[index | odd].x;
            y = values[index | odd].y;
            otherX = values[reversed | odd].x;
            otherY = values[reversed | odd].y;
            values[reversed | odd].x = x;
            values[reversed | odd].y = y;
            values[index | odd].x = otherX;
            values[index | odd].y = otherY;
        }
        std::uint32_t bit = quarter;
        while (bit <= reversed) {
            reversed -= bit;
            bit >>= 1;
        }
        reversed += bit;
    }
}
#endif

#if SHENMUE_S2_GCC295 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void FftRadix4(Vector2 *values, std::int32_t logSize, const Vector2 *twiddles) {
    const std::int32_t size = static_cast<std::int32_t>(1U << logSize);
    std::int32_t remaining = size >> 2;
    const Vector2 *pairTwiddle = twiddles;
    do {
        Vector2 *const first = values++;
        float firstX = first->x, firstY = first->y;
        Vector2 *const second = values++;
        const float secondX = second->x, secondY = second->y;
        const float firstReal = pairTwiddle->x, firstImaginary = pairTwiddle->y;
        ++pairTwiddle;
        const float firstDifferenceX = firstX - secondX;
        firstX += secondX;
        const float firstDifferenceY = firstY - secondY;
        firstY += secondY;
        float firstProductY = -firstImaginary * firstDifferenceX + firstDifferenceY * firstReal;
        float firstProductX = firstDifferenceX * firstReal + firstDifferenceY * firstImaginary;
        Vector2 *const third = values++;
        float thirdX = third->x, thirdY = third->y;
        Vector2 *const fourth = values++;
        const float fourthX = fourth->x, fourthY = fourth->y;
        const float secondReal = pairTwiddle->x, secondImaginary = pairTwiddle->y;
        ++pairTwiddle;
        const float secondDifferenceX = thirdX - fourthX;
        thirdX += fourthX;
        const float secondDifferenceY = thirdY - fourthY;
        thirdY += fourthY;
        const float secondProductY = -secondImaginary * secondDifferenceX + secondDifferenceY * secondReal;
        const float secondProductX = secondDifferenceX * secondReal + secondDifferenceY * secondImaginary;
        const float groupReal = twiddles->x, groupImaginary = twiddles->y;
        ++twiddles;
        const float sumDifferenceX = firstX - thirdX;
        firstX += thirdX;
        const float sumDifferenceY = firstY - thirdY;
        firstY += thirdY;
        const float sumProductY = -groupImaginary * sumDifferenceX + sumDifferenceY * groupReal;
        const float sumProductX = sumDifferenceX * groupReal + sumDifferenceY * groupImaginary;
        first->y = firstY;
        first->x = firstX;
        third->y = sumProductY;
        third->x = sumProductX;
        const float productDifferenceX = firstProductX - secondProductX;
        firstProductX += secondProductX;
        const float productDifferenceY = firstProductY - secondProductY;
        firstProductY += secondProductY;
        const float productY = -groupImaginary * productDifferenceX + productDifferenceY * groupReal;
        const float productX = productDifferenceX * groupReal + productDifferenceY * groupImaginary;
        second->y = firstProductY;
        second->x = firstProductX;
        fourth->y = productY;
        fourth->x = productX;
        remaining = static_cast<std::int32_t>(static_cast<std::uint32_t>(remaining) - 1U);
    } while (remaining);
}
#else
void FftRadix4(Vector2 *values, std::int32_t logSize, const Vector2 *twiddles) {
    const std::uint32_t size = logSize < 0 ? 0U : 1U << (static_cast<std::uint32_t>(logSize) & 31U);
    std::int32_t remaining = static_cast<std::int32_t>(size) >> 2;
    const Vector2 *pairTwiddle = twiddles;
    const Vector2 *groupTwiddle = twiddles;
    do {
        const Vector2 first = values[0];
        const Vector2 second = values[1];
        const Vector2 third = values[2];
        const Vector2 fourth = values[3];
        const float firstDifferenceX = first.x - second.x;
        const float firstDifferenceY = first.y - second.y;
        const float firstSumX = first.x + second.x;
        const float firstSumY = first.y + second.y;
#if SHENMUE_IS_S2
        const Vector2 firstProduct = {FftMultiplyAdd(firstDifferenceX, pairTwiddle[0].x, firstDifferenceY * pairTwiddle[0].y),
            FftMultiplyAdd(-pairTwiddle[0].y, firstDifferenceX, firstDifferenceY * pairTwiddle[0].x)};
        const float secondDifferenceX = third.x - fourth.x;
        const float secondDifferenceY = third.y - fourth.y;
        const float secondSumX = third.x + fourth.x;
        const float secondSumY = third.y + fourth.y;
        const Vector2 secondProduct = {FftMultiplyAdd(secondDifferenceX, pairTwiddle[1].x, secondDifferenceY * pairTwiddle[1].y),
            FftMultiplyAdd(-pairTwiddle[1].y, secondDifferenceX, secondDifferenceY * pairTwiddle[1].x)};
        const float sumDifferenceX = firstSumX - secondSumX;
        const float sumDifferenceY = firstSumY - secondSumY;
        values[0].y = firstSumY + secondSumY;
        values[0].x = firstSumX + secondSumX;
        values[2].y = FftMultiplyAdd(-groupTwiddle->y, sumDifferenceX, sumDifferenceY * groupTwiddle->x);
        values[2].x = FftMultiplyAdd(sumDifferenceX, groupTwiddle->x, sumDifferenceY * groupTwiddle->y);
        const float productDifferenceX = firstProduct.x - secondProduct.x;
        const float productDifferenceY = firstProduct.y - secondProduct.y;
        values[1].y = firstProduct.y + secondProduct.y;
        values[1].x = firstProduct.x + secondProduct.x;
        values[3].y = FftMultiplyAdd(-groupTwiddle->y, productDifferenceX, productDifferenceY * groupTwiddle->x);
        values[3].x = FftMultiplyAdd(productDifferenceX, groupTwiddle->x, productDifferenceY * groupTwiddle->y);
#else
        const float firstTwiddleX = firstDifferenceX * pairTwiddle[0].x;
        const float firstTwiddleY = firstDifferenceX * pairTwiddle[0].y;
        const Vector2 firstProduct{
            .x = std::fma(firstDifferenceY, pairTwiddle[0].y, firstTwiddleX), .y = std::fma(firstDifferenceY, pairTwiddle[0].x, -firstTwiddleY)};
        const float secondDifferenceX = third.x - fourth.x;
        const float secondDifferenceY = third.y - fourth.y;
        const float secondSumX = third.x + fourth.x;
        const float secondSumY = third.y + fourth.y;
        const float secondTwiddleX = secondDifferenceX * pairTwiddle[1].x;
        const float secondTwiddleY = secondDifferenceX * pairTwiddle[1].y;
        const Vector2 secondProduct{
            .x = std::fma(secondDifferenceY, pairTwiddle[1].y, secondTwiddleX), .y = std::fma(secondDifferenceY, pairTwiddle[1].x, -secondTwiddleY)};
        const float sumDifferenceX = firstSumX - secondSumX;
        const float sumDifferenceY = firstSumY - secondSumY;
        const float sumTwiddleX = groupTwiddle->x * sumDifferenceX;
        const float sumTwiddleY = groupTwiddle->y * sumDifferenceX;
        values[0].y = firstSumY + secondSumY;
        values[0].x = firstSumX + secondSumX;
        values[2].y = std::fma(groupTwiddle->x, sumDifferenceY, -sumTwiddleY);
        values[2].x = std::fma(groupTwiddle->y, sumDifferenceY, sumTwiddleX);
        const float productDifferenceX = firstProduct.x - secondProduct.x;
        const float productDifferenceY = firstProduct.y - secondProduct.y;
        const float productTwiddleX = groupTwiddle->x * productDifferenceX;
        const float productTwiddleY = groupTwiddle->y * productDifferenceX;
        values[1].y = firstProduct.y + secondProduct.y;
        values[1].x = firstProduct.x + secondProduct.x;
        values[3].y = std::fma(groupTwiddle->x, productDifferenceY, -productTwiddleY);
        values[3].x = std::fma(groupTwiddle->y, productDifferenceY, productTwiddleX);
#endif
        values += 4;
        pairTwiddle += 2;
        ++groupTwiddle;
        remaining = static_cast<std::int32_t>(static_cast<std::uint32_t>(remaining) - 1U);
    } while (remaining);
}
#endif

#if SHENMUE_IS_S2
namespace {
// @verified - d
void FftReverse32(Vector2 *values) {
    Vector2 *const upper = values + 16;
    {
        const float firstX = values[1].x, firstY = values[1].y, secondX = upper[0].x, secondY = upper[0].y;
        upper[0].x = firstX;
        upper[0].y = firstY;
        values[1].x = secondX;
        values[1].y = secondY;
    }
    {
        const float firstX = values[2].x, firstY = values[2].y, secondX = values[8].x, secondY = values[8].y;
        values[8].x = firstX;
        values[8].y = firstY;
        values[2].x = secondX;
        values[2].y = secondY;
    }
    {
        const float firstX = values[3].x, firstY = values[3].y, secondX = upper[8].x, secondY = upper[8].y;
        upper[8].x = firstX;
        upper[8].y = firstY;
        values[3].x = secondX;
        values[3].y = secondY;
    }
    {
        const float firstX = values[5].x, firstY = values[5].y, secondX = upper[4].x, secondY = upper[4].y;
        upper[4].x = firstX;
        upper[4].y = firstY;
        values[5].x = secondX;
        values[5].y = secondY;
    }
    {
        const float firstX = values[6].x, firstY = values[6].y, secondX = values[12].x, secondY = values[12].y;
        values[12].x = firstX;
        values[12].y = firstY;
        values[6].x = secondX;
        values[6].y = secondY;
    }
    {
        const float firstX = values[7].x, firstY = values[7].y, secondX = upper[12].x, secondY = upper[12].y;
        upper[12].x = firstX;
        upper[12].y = firstY;
        values[7].x = secondX;
        values[7].y = secondY;
    }
    {
        const float firstX = values[9].x, firstY = values[9].y, secondX = upper[2].x, secondY = upper[2].y;
        upper[2].x = firstX;
        upper[2].y = firstY;
        values[9].x = secondX;
        values[9].y = secondY;
    }
    {
        const float firstX = values[11].x, firstY = values[11].y, secondX = upper[10].x, secondY = upper[10].y;
        upper[10].x = firstX;
        upper[10].y = firstY;
        values[11].x = secondX;
        values[11].y = secondY;
    }
    {
        const float firstX = values[13].x, firstY = values[13].y, secondX = upper[6].x, secondY = upper[6].y;
        upper[6].x = firstX;
        upper[6].y = firstY;
        values[13].x = secondX;
        values[13].y = secondY;
    }
    {
        const float firstX = values[15].x, firstY = values[15].y, secondX = upper[14].x, secondY = upper[14].y;
        upper[14].x = firstX;
        upper[14].y = firstY;
        values[15].x = secondX;
        values[15].y = secondY;
    }
    {
        const float firstX = upper[3].x, firstY = upper[3].y, secondX = upper[9].x, secondY = upper[9].y;
        upper[9].x = firstX;
        upper[9].y = firstY;
        upper[3].x = secondX;
        upper[3].y = secondY;
    }
    {
        const float firstX = upper[7].x, firstY = upper[7].y, secondX = upper[13].x, secondY = upper[13].y;
        upper[13].x = firstX;
        upper[13].y = firstY;
        upper[7].x = secondX;
        upper[7].y = secondY;
    }
}

void FftInverseStages(Vector2 *values, std::int32_t logSize, const Vector2 *twiddles) {
#if SHENMUE_S2_GCC295 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::int32_t groups = (1 << logSize) >> 3, span = 4;
    for (std::int32_t stage = logSize - 4; stage; --stage) {
        Vector2 *first = values;
        const Vector2 *twiddle = twiddles;
        for (std::int32_t group = groups; group; --group, ++twiddle) {
            Vector2 *second = first + span;
            const float real = twiddle->x, imaginary = twiddle->y;
            const float negativeImaginary = 0.0f - imaginary;
            for (std::int32_t index = span; index; --index, ++first, ++second) {
                float firstX = first->x, firstY = first->y;
                const float secondX = second->x, secondY = second->y;
                const float differenceX = firstX - secondX, differenceY = firstY - secondY;
                firstY += secondY;
                firstX += secondX;
                first->y = firstY;
                first->x = firstX;
                second->y = negativeImaginary * differenceX + differenceY * real;
                second->x = real * differenceX + differenceY * imaginary;
            }
            first += span;
        }
        groups >>= 1;
        span += span;
    }
#else
    std::int32_t groups = (1 << logSize) >> 3, span = 4;
    for (std::int32_t stage = logSize - 4; stage; --stage) {
        Vector2 *first = values;
        const Vector2 *twiddle = twiddles;
        for (std::int32_t group = groups; group; --group, ++twiddle) {
            Vector2 *second = first + span;
            for (std::int32_t index = span; index; --index, ++first, ++second) {
                const float differenceX = first->x - second->x, differenceY = first->y - second->y;
                first->x += second->x;
                first->y += second->y;
                second->y = FftMultiplyAdd(-twiddle->y, differenceX, differenceY * twiddle->x);
                second->x = FftMultiplyAdd(twiddle->x, differenceX, differenceY * twiddle->y);
            }
            first += span;
        }
        groups >>= 1;
        span += span;
    }
#endif
}
} // namespace

void FftForward32(Vector2 *values) {
    FftReverse32(values);
    FftRadix4(values, 5, g_fftForwardTwiddles);
    FftInverseStages(values, 5, g_fftForwardTwiddles);
    FftForwardHalves(values, 5);
    FftForwardFinal(values, 5);
}

// @verified - d
void FftInverse32(Vector2 *values) {
    FftReverse32(values);
    FftRadix4(values, 5, g_fftBackwardTwiddles);
    FftInverseStages(values, 5, g_fftBackwardTwiddles);
    FftInverseHalves(values, 5);
    FftInverseScale(values, 5, &kFftInverse32Scale);
}

void FftForward(Vector2 *values, std::int32_t logSize) {
    FftReverseBits(values, 1 << logSize);
    FftRadix4(values, logSize, g_fftForwardTwiddles);
    FftInverseStages(values, logSize, g_fftForwardTwiddles);
    FftForwardHalves(values, logSize);
    FftForwardFinal(values, logSize);
}

void FftInverse(Vector2 *values, std::int32_t logSize) {
    const std::uint32_t size = 1U << logSize;
    const float scale = 1.0f / static_cast<float>(size);
    FftReverseBits(values, size);
    FftRadix4(values, logSize, g_fftBackwardTwiddles);
    FftInverseStages(values, logSize, g_fftBackwardTwiddles);
    FftInverseHalves(values, logSize);
    FftInverseScale(values, logSize, &scale);
}
#endif

} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif
