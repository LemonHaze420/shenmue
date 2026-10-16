#include "common/math.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <limits>

namespace shenmue {

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4 &left, const Matrix4x4 &right) {
    Matrix4x4 output{};
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            for (int inner = 0; inner < 4; ++inner) {
                output.rows[row][column] += left.rows[row][inner] * right.rows[inner][column];
            }
        }
    }
    return output;
}

void Matrix4x4::preTranslate(Matrix4x4 &m, float x, float y, float z) {
    for (int c = 0; c < 4; ++c) {
        m.rows[3][c] += m.rows[0][c] * x + m.rows[1][c] * y + m.rows[2][c] * z;
    }
}

std::uint16_t atan2_fixed16(float y, float x) {
    const auto fixed = [](float ratio) { return static_cast<std::int32_t>(std::atan(ratio) * 65536.0f / 6.2831855f + 0.5f); };
    if (x >= y) {
        if (x >= -y)
            return x == 0.0f ? 0 : static_cast<std::uint16_t>(fixed(y / x));
        return y == 0.0f ? 0xc000 : static_cast<std::uint16_t>(-0x4000 - fixed(x / y));
    }
    if (x >= -y)
        return y == 0.0f ? 0x4000 : static_cast<std::uint16_t>(0x4000 - fixed(x / y));
    return x == 0.0f ? 0x8000 : static_cast<std::uint16_t>(fixed(y / x) - 0x8000);
}

std::int16_t asin_fixed16(float value) {
    if (std::fabs(value) <= 1.0f) {
        const float norm = std::sqrt(1.0f - value * value);
        if (norm != 0.0f)
            return static_cast<std::int16_t>(static_cast<std::int32_t>(std::atan(value / norm) * 65536.0f / 6.2831855f + 0.5f));
    }
    return value > 0.0f ? 0x4000 : static_cast<std::int16_t>(0xc000);
}



// @verified - p
template <typename T> float Vector3T<T>::NormalizeInPlace(Vector3T<T> &value) requires std::is_same_v<T, float> {
    const float magnitude = std::sqrt((value.x * value.x + value.y * value.y) + value.z * value.z);
    if (magnitude > 0.0f) {
        value.x = value.x * (1.0f / magnitude);
        value.y = value.y * (1.0f / magnitude);
        value.z = value.z * (1.0f / magnitude);
    }
    return magnitude;
}

// @verified - p
template <typename T> float Vector3T<T>::CalcMagnitude(const Vector3T<T> &vector) requires std::is_same_v<T, float> {
    return std::sqrt((vector.x * vector.x + vector.y * vector.y) + vector.z * vector.z);
}

template struct Vector3T<float>;


float ceil_float(float value) {
    const float negated = -value;
    const float magnitude = std::fabs(negated);
    if (magnitude >= 8388608.0f)
        return value;
    const std::int32_t whole = std::isnan(magnitude) ? std::numeric_limits<std::int32_t>::min() : static_cast<std::int32_t>(magnitude);
    if (!(0.0f > negated))
        return -static_cast<float>(whole);
    if (magnitude - static_cast<float>(whole) == 0.0f)
        return value;
    return static_cast<float>(whole) + 1.0f;
}


std::int32_t TruncateToInt32(float value) {
    return value > -2147483648.5f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : std::numeric_limits<std::int32_t>::min();
}

namespace {

constexpr float kLn2 = 0.6931471824645996f;

} // namespace


float fast_logf_approx(float value) {
    float result = 0.0f;
    if (0.0f > value || value == 0.0f)
        return result;
    while (value >= 2.0f) {
        value *= 0.5f;
        result += kLn2;
    }
    while (1.0f > value) {
        value += value;
        result += -kLn2;
    }
    const float t = (value - 1.0f) / (value + 1.0f);
    const float t2 = t * t;
    float term = t + t;
    float previous;
    std::int32_t divisor = 1;
    do {
        const float step = term / static_cast<float>(divisor);
        divisor += 2;
        previous = result;
        term *= t2;
        result += step;
    } while (result != previous);
    return result;
}


float fast_ldexpf(float value, std::int32_t exponent) {
    if (value == 0.0f)
        return 0.0f;
    std::uint32_t bits = std::bit_cast<std::uint32_t>(value);
    const std::int32_t biased = static_cast<std::int32_t>((bits >> 23U) & 0xffU) + exponent;
    if (biased < 1) {
        const std::uint32_t shift = static_cast<std::uint8_t>(1 - biased) & 31U;
        bits = (bits & 0x80000000U) | (((bits & 0x7fffffU) | 0x800000U) >> shift);
    } else if (biased > 254) {
        bits = (bits & 0x80000000U) | 0x7f800000U;
    } else {
        bits = (bits & 0x807fffffU) | (static_cast<std::uint32_t>(biased) << 23U);
    }
    return std::bit_cast<float>(bits);
}


float fast_expf_approx(float value) {
    const std::int32_t exponent = TruncateToInt32(value / kLn2 + (value >= 0.0f ? 0.5f : -0.5f));
    const float reduced = value - static_cast<float>(exponent) * kLn2;
    const float squared = reduced * reduced;
    float fraction = squared / 14.0f;
    for (std::int32_t term = 10; term >= 6; term -= 4)
        fraction = squared / (static_cast<float>(term) + fraction);
    fraction += 2.0f;
    return fast_ldexpf((fraction + reduced) / (fraction - reduced), exponent);
}


float fast_powf_approx(float base, float exponent) {
    std::int32_t whole = TruncateToInt32(exponent);
    if (base == 0.0f)
        return exponent <= 0.0f ? 1.0f : 0.0f;
    if (exponent == static_cast<float>(whole)) {
        float result = 1.0f;
        if (whole < 0) {
            whole = static_cast<std::int32_t>(0U - static_cast<std::uint32_t>(whole));
            base = 1.0f / base;
        }
        for (;;) {
            if ((whole & 1) != 0)
                result *= base;
            whole /= 2;
            if (!whole)
                return result;
            base *= base;
        }
    }
    if (0.0f > base)
        return 0.0f;
    return fast_expf_approx(fast_logf_approx(base) * exponent);
}

std::uint32_t *RadixSortIndicesBy4ByteKey(const float *keys, std::uint32_t count, std::uint32_t *&indices, std::uint32_t *&scratch) noexcept {
    if (!keys || !indices || !scratch || count == 0)
        return indices;

    bool alreadySorted = true;
    for (std::uint32_t index = 0; index < count; ++index) {
        indices[index] = index;
        if (index != 0 && keys[index - 1] > keys[index]) {
            alreadySorted = false;
        }
    }
    if (alreadySorted)
        return indices;

    std::array<std::array<std::uint32_t, 256>, 4> counts{};
    for (std::uint32_t index = 0; index < count; ++index) {
        const std::uint32_t key = std::bit_cast<std::uint32_t>(keys[index]);
        for (unsigned byte = 0; byte < 4; ++byte) {
            ++counts[byte][(key >> (byte * 8U)) & 0xffU];
        }
    }

    std::array<std::uint32_t, 256> offsets{};
    for (unsigned byte = 0; byte < 4; ++byte) {
        const unsigned shift = byte * 8U;
        const std::uint32_t first = (std::bit_cast<std::uint32_t>(keys[0]) >> shift) & 0xffU;
        if (counts[byte][first] == count) {
            if (byte == 3 && first >= 128) {
                for (std::uint32_t index = 0; index < count; ++index) {
                    scratch[index] = indices[count - index - 1];
                }
                std::swap(indices, scratch);
            }
            continue;
        }

        if (byte != 3) {
            offsets[0] = 0;
            for (unsigned bucket = 1; bucket < 256; ++bucket) {
                offsets[bucket] = offsets[bucket - 1] + counts[byte][bucket - 1];
            }
        } else {
            offsets[0] = 0;
            for (unsigned bucket = 128; bucket < 256; ++bucket) {
                offsets[0] += counts[3][bucket];
            }
            for (unsigned bucket = 1; bucket < 128; ++bucket) {
                offsets[bucket] = offsets[bucket - 1] + counts[3][bucket - 1];
            }
            offsets[255] = counts[3][255];
            for (int bucket = 254; bucket >= 128; --bucket) {
                offsets[static_cast<std::size_t>(bucket)] = offsets[static_cast<std::size_t>(bucket + 1)] + counts[3][static_cast<std::size_t>(bucket)];
            }
        }

        for (std::uint32_t index = 0; index < count; ++index) {
            const std::uint32_t source = indices[index];
            const std::uint32_t bucket = (std::bit_cast<std::uint32_t>(keys[source]) >> shift) & 0xffU;
            const std::uint32_t destination = byte == 3 && bucket >= 128 ? --offsets[bucket] : offsets[bucket]++;
            scratch[destination] = source;
        }
        std::swap(indices, scratch);
    }
    return indices;
}

} // namespace shenmue
