#include "common/math.hpp"

#if SHENMUE_S2_GCC295
#include <math.h>
#else
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <limits>
#endif

#if SHENMUE_IS_DC
#include "platform/dc/sdk.hpp"
#endif

namespace shenmue {

#if SHENMUE_S2_GCC295
namespace {
template <typename Target, typename Source> Target MathValueBits(Source value) {
    typedef char WidthCheck[sizeof(Target) == sizeof(Source) ? 1 : -1];
    union {
        Source source;
        Target target;
    } bits;
    bits.source = value;
    return bits.target;
}
} // namespace
#define SHENMUE_MATH_BITS MathValueBits
#define SHENMUE_MATH_FLOAT_TYPE
#define SHENMUE_MATH_NOEXCEPT
#define SHENMUE_MATH_FABS ::fabsf
#define SHENMUE_MATH_SQRT ::sqrtf
#define SHENMUE_MATH_ATAN ::atanf
#define SHENMUE_MATH_ISNAN ::isnanf
#define SHENMUE_MATH_INT_MIN (-2147483647 - 1)
#define SHENMUE_MATH_INT_MAX 2147483647
#else
#define SHENMUE_MATH_BITS std::bit_cast
#define SHENMUE_MATH_FLOAT_TYPE requires std::is_same_v<T, float>
#define SHENMUE_MATH_NOEXCEPT noexcept
#define SHENMUE_MATH_FABS std::fabs
#define SHENMUE_MATH_SQRT std::sqrt
#define SHENMUE_MATH_ATAN std::atan
#define SHENMUE_MATH_ISNAN std::isnan
#define SHENMUE_MATH_INT_MIN std::numeric_limits<std::int32_t>::min()
#define SHENMUE_MATH_INT_MAX std::numeric_limits<std::int32_t>::max()
#endif

#if SHENMUE_IS_DC
// @verified - d
void CalculateFixedSinCos(std::uint32_t angle, Vector2 *output) {
    output->y = dc::sdk::njCos(static_cast<std::int32_t>(angle));
    output->x = dc::sdk::njSin(static_cast<std::int32_t>(angle));
}

// @verified - d
float CalculateDistanceBetweenPoints(const Vector3 *first, const Vector3 *second) {
#if SHENMUE_S2_GCC295
    const float x = first->x - second->x;
    const float y = first->y - second->y;
    const float z = first->z - second->z;
    if (::fabsf(x) < 1.1920929e-07f && ::fabsf(y) < 1.1920929e-07f && ::fabsf(z) < 1.1920929e-07f)
        return 0.0f;
    return ::sqrtf(x * x + y * y + z * z);
#else
    const Vector3 delta{first->x - second->x, first->y - second->y, first->z - second->z};
    return dc::sdk::njScalor(&delta);
#endif
}
// @verified - d
float CalculatePlanarDistanceBetweenPoints(const Vector3 *first, const Vector3 *second) {
    const float x = first->x - second->x;
    const float z = first->z - second->z;
    float distance = x * x + z * z;
#if SHENMUE_S2_GCC295
    distance = ::sqrtf(distance);
#else
    distance = __builtin_sqrtf(distance);
#endif
    return distance;
}

// @verified - d
float CalculateDistanceSquaredBetweenPoints(const Vector3 *first, const Vector3 *second) {
#if SHENMUE_S2_GCC295
    const Vector3 delta = {first->x - second->x, first->y - second->y, first->z - second->z};
#else
    const Vector3 delta{first->x - second->x, first->y - second->y, first->z - second->z};
#endif
    return dc::sdk::njInnerProduct(&delta, &delta);
}

#endif

#if SHENMUE_IS_DC
// @verified - d
float CalculateFixedSine(std::uint32_t angle) {
    return dc::sdk::njSin(static_cast<std::int32_t>(angle));
}

// @verified - d
float CalculateFixedCosine(std::uint32_t angle) {
    return dc::sdk::njCos(static_cast<std::int32_t>(angle));
}

#endif


#if SHENMUE_IS_DC
// @verified - d
std::uint16_t atan_fixed16(float value) {
    const float reduced = value > 1.0f || value < -1.0f ? 1.0f / value : value;
    const float square = reduced * reduced;
    float fraction = 0.0f;
    for (std::int32_t index = 13; index >= 1; --index)
        fraction = (static_cast<float>(index * index) * square) / (static_cast<float>(index * 2 + 1) + fraction);
    const float radians = reduced / (1.0f + fraction);
    const float turn = SHENMUE_MATH_BITS<float>(0x40c90fdbU);
    const float scaled = ((radians + turn) * 65536.0f) / turn + 0.5f;
#if SHENMUE_IS_S2
    const std::uint16_t angle = static_cast<std::uint16_t>(TruncateToInt32(scaled));
#else
    const std::uint16_t angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(scaled));
#endif
    if (value > 1.0f)
        return static_cast<std::uint16_t>(0x4000U - angle);
    if (value < -1.0f)
        return static_cast<std::uint16_t>(0xc000U - angle);
    return angle;
}

#endif

#if SHENMUE_IS_DC && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
float CalculatePlanarMagnitude(float x, float z) {
    float magnitude = x * x + z * z;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    magnitude = SHENMUE_MATH_SQRT(magnitude);
#else
    magnitude = __builtin_sqrtf(magnitude);
#endif
    return magnitude;
}
#endif


Matrix4x4 Matrix4x4::Multiply(const Matrix4x4 &left, const Matrix4x4 &right) {
#if SHENMUE_S2_GCC295
    Matrix4x4 output = {};
#else
    Matrix4x4 output{};
#endif
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

// @verified - p
Vector4 *Vector4::ProjectPoint(const Matrix4x4 *transform, Vector4 *output, const Vector4 *input) {
    const float x = input->x, y = input->y, z = input->z;
    const float depth = ((x * transform->rows[2].x + y * transform->rows[2].y) + z * transform->rows[2].z) + transform->rows[2].w;
    const float w = ((x * transform->rows[3].x + y * transform->rows[3].y) + z * transform->rows[3].z) + transform->rows[3].w;
    const float inverse = 1.0f / w;
    const float outY = inverse * (((x * transform->rows[1].x + y * transform->rows[1].y) + z * transform->rows[1].z) + transform->rows[1].w);
    output->x = inverse * (((y * transform->rows[0].y + x * transform->rows[0].x) + z * transform->rows[0].z) + transform->rows[0].w);
    output->y = outY;
    output->z = inverse * depth;
    output->w = inverse * w;
    return output;
}

float BoundsDistanceSquared(const Vector3 &minimum, const Vector3 &maximum, const Vector3 &position) {
    float distance = 0.0f;
    for (unsigned axis = 0; axis < 3; ++axis) {
        if (minimum[axis] > position[axis]) {
            const float delta = minimum[axis] - position[axis];
            distance += delta * delta;
        }
        if (position[axis] > maximum[axis]) {
            const float delta = position[axis] - maximum[axis];
            distance += delta * delta;
        }
    }
    return distance;
}

#if !SHENMUE_S2_GCC295
void TransformBounds(Vector3 &minimum, Vector3 &maximum, const Matrix4x4 &transform) {
    const Vector3 low = minimum;
    const Vector3 high = maximum;
    for (unsigned axis = 0; axis < 3; ++axis) {
        const Vector4 &row = transform.rows[axis];
        minimum[axis] = std::fmin(row.x * low.x, row.x * high.x) + std::fmin(row.y * low.y, row.y * high.y) + std::fmin(row.z * low.z, row.z * high.z) + row.w;
        maximum[axis] = std::fmax(row.x * low.x, row.x * high.x) + std::fmax(row.y * low.y, row.y * high.y) + std::fmax(row.z * low.z, row.z * high.z) + row.w;
    }
}

// @verified - p
bool IsAabbInsideSelectedFrustumPlanes(const std::array<Vector4, 6> &planes, const Vector3 &minimum, const Vector3 &maximum, std::uint8_t skipPlanePairs) {
    const std::array<Vector3, 8> corners{{minimum, {maximum.x, minimum.y, minimum.z}, {maximum.x, maximum.y, minimum.z}, {minimum.x, maximum.y, minimum.z},
        {minimum.x, minimum.y, maximum.z}, {maximum.x, minimum.y, maximum.z}, maximum, {minimum.x, maximum.y, maximum.z}}};
    for (std::uint32_t index = 2U * skipPlanePairs; index < 6; ++index) {
        const Vector4 &plane = planes[index];
        bool outside = true;
        for (const Vector3 &point : corners) {
            if (!((((plane.x * point.x + plane.y * point.y) + plane.z * point.z) + plane.w) < 0.0f)) {
                outside = false;
                break;
            }
        }
        if (outside)
            return false;
    }
    return true;
}

#endif

std::uint16_t atan2_fixed16(float y, float x) {
#if SHENMUE_IS_DC
    if (y > x) {
        if (-y > x)
            return x == 0.0f ? 0x8000U : static_cast<std::uint16_t>(atan_fixed16(y / x) + 0x8000U);
        return y == 0.0f ? 0x4000U : static_cast<std::uint16_t>(0x4000U - atan_fixed16(x / y));
    }
    if (-y > x)
        return y == 0.0f ? 0xc000U : static_cast<std::uint16_t>(0xc000U - atan_fixed16(x / y));
    return x == 0.0f ? 0U : atan_fixed16(y / x);
#else
    const auto fixed = [](float ratio) { return static_cast<std::int32_t>(SHENMUE_MATH_ATAN(ratio) * 65536.0f / 6.2831855f + 0.5f); };
    if (x >= y) {
        if (x >= -y)
            return x == 0.0f ? 0 : static_cast<std::uint16_t>(fixed(y / x));
        return y == 0.0f ? 0xc000 : static_cast<std::uint16_t>(-0x4000 - fixed(x / y));
    }
    if (x >= -y)
        return y == 0.0f ? 0x4000 : static_cast<std::uint16_t>(0x4000 - fixed(x / y));
    return x == 0.0f ? 0x8000 : static_cast<std::uint16_t>(fixed(y / x) - 0x8000);
#endif
}

float cot_fixed16(std::uint16_t angle) {
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle)));
    return cosine / sine;
}

std::int16_t asin_fixed16(float value) {
    if (SHENMUE_MATH_FABS(value) <= 1.0f) {
        const float norm = SHENMUE_MATH_SQRT(1.0f - value * value);
        if (norm != 0.0f)
            return static_cast<std::int16_t>(static_cast<std::int32_t>(SHENMUE_MATH_ATAN(value / norm) * 65536.0f / 6.2831855f + 0.5f));
    }
    return value > 0.0f ? 0x4000 : static_cast<std::int16_t>(0xc000);
}

// Native Vector3_NormalizeInPlace: scale the vector to unit length unless its length is zero (or NaN). The length stays in the return
// register, which callers such as Vector3_SubtractInto pass on.
// @verified - p
template <typename T> float Vector3T<T>::NormalizeInPlace(Vector3T<T> &value) SHENMUE_MATH_FLOAT_TYPE {
#if SHENMUE_IS_DC
    const float magnitude = dc::sdk::njScalor(&value);
    dc::sdk::njUnitVector(&value);
    return magnitude;
#else
    const float magnitude = SHENMUE_MATH_SQRT((value.x * value.x + value.y * value.y) + value.z * value.z);
    if (magnitude > 0.0f) {
        value.x = value.x * (1.0f / magnitude);
        value.y = value.y * (1.0f / magnitude);
        value.z = value.z * (1.0f / magnitude);
    }
    return magnitude;
#endif
}

// @verified - p
template <typename T> float Vector3T<T>::CalcMagnitude(const Vector3T<T> &vector) SHENMUE_MATH_FLOAT_TYPE {
#if SHENMUE_IS_DC
    return dc::sdk::njScalor(&vector);
#else
    return SHENMUE_MATH_SQRT((vector.x * vector.x + vector.y * vector.y) + vector.z * vector.z);
#endif
}

template struct Vector3T<float>;

// Native returns -floor(-value): magnitudes of at least 2^23 pass through, and a NaN yields 2^31 (the truncated INT_MIN negated).
float ceil_float(float value) {
    const float negated = -value;
    const float magnitude = SHENMUE_MATH_FABS(negated);
    if (magnitude >= 8388608.0f)
        return value;
    const std::int32_t whole = SHENMUE_MATH_ISNAN(magnitude) ? SHENMUE_MATH_INT_MIN : static_cast<std::int32_t>(magnitude);
    if (!(0.0f > negated))
        return -static_cast<float>(whole);
    if (magnitude - static_cast<float>(whole) == 0.0f)
        return value;
    return static_cast<float>(whole) + 1.0f;
}

// DC preserves FTRC hardware behavior; other branches preserve cvttss2si's INT_MIN convention.
std::int32_t TruncateToInt32(float value) {
#if SHENMUE_IS_DC
    if (value >= 2147483648.0f)
        return SHENMUE_MATH_INT_MAX;
    if (!(value > -2147483648.0f))
        return SHENMUE_MATH_INT_MIN;
    return static_cast<std::int32_t>(value);
#else
    return value > -2147483648.5f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : SHENMUE_MATH_INT_MIN;
#endif
}

namespace {

#if SHENMUE_S2_GCC295
const float kLn2 = 0.6931471824645996f;
#else
constexpr float kLn2 = 0.6931471824645996f;
#endif

} // namespace

// Native reduces into [1, 2) by powers of two, then sums the atanh series 2(t + t^3/3 + ...) until the float sum stops changing.
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

// Native's exponent field arithmetic: underflow shifts the explicit mantissa right, overflow saturates to infinity, and zero returns +0.
float fast_ldexpf(float value, std::int32_t exponent) {
    if (value == 0.0f)
        return 0.0f;
    std::uint32_t bits = SHENMUE_MATH_BITS<std::uint32_t>(value);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    const std::int32_t biased = SHENMUE_MATH_BITS<std::int32_t>(((bits >> 23U) & 0xffU) + static_cast<std::uint32_t>(exponent));
#else
    const std::int32_t biased = static_cast<std::int32_t>((bits >> 23U) & 0xffU) + exponent;
#endif
    if (biased < 1) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        const std::uint32_t shift = static_cast<std::uint32_t>(biased) - 1U;
        const std::uint32_t mantissa = (bits & 0x7fffffU) | 0x800000U;
        std::uint32_t shifted;
        if (SHENMUE_MATH_BITS<std::int32_t>(shift) >= 0) {
            shifted = mantissa << (shift & 31U);
        } else {
            const std::uint32_t right = (0U - shift) & 31U;
            shifted = right ? mantissa >> right : 0U;
        }
        bits = (bits & 0x80000000U) | shifted;
#else
        const std::uint32_t shift = static_cast<std::uint8_t>(1 - biased) & 31U;
        bits = (bits & 0x80000000U) | (((bits & 0x7fffffU) | 0x800000U) >> shift);
#endif
    } else if (biased > 254) {
        bits = (bits & 0x80000000U) | 0x7f800000U;
    } else {
        bits = (bits & 0x807fffffU) | (static_cast<std::uint32_t>(biased) << 23U);
    }
    return SHENMUE_MATH_BITS<float>(bits);
}

// Native splits off a multiple of ln 2 and evaluates the remainder with the continued fraction 2 + r^2/(6 + r^2/(10 + r^2/14)).
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

// Native squares for whole exponents, returns 0 for other negative bases and otherwise uses exp(log(base) * exponent).
float fast_powf_approx(float base, float exponent) {
    std::int32_t whole = TruncateToInt32(exponent);
    if (base == 0.0f)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        return exponent > 0.0f ? 0.0f : 1.0f;
#else
        return exponent <= 0.0f ? 1.0f : 0.0f;
#endif
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

std::uint32_t *RadixSortIndicesBy4ByteKey(const float *keys, std::uint32_t count, std::uint32_t *&indices, std::uint32_t *&scratch) SHENMUE_MATH_NOEXCEPT {
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

#if SHENMUE_S2_GCC295
    std::uint32_t counts[4][256] = {};
#else
    std::array<std::array<std::uint32_t, 256>, 4> counts{};
#endif
    for (std::uint32_t index = 0; index < count; ++index) {
        const std::uint32_t key = SHENMUE_MATH_BITS<std::uint32_t>(keys[index]);
        for (unsigned byte = 0; byte < 4; ++byte) {
            ++counts[byte][(key >> (byte * 8U)) & 0xffU];
        }
    }

#if SHENMUE_S2_GCC295
    std::uint32_t offsets[256] = {};
#else
    std::array<std::uint32_t, 256> offsets{};
#endif
    for (unsigned byte = 0; byte < 4; ++byte) {
        const unsigned shift = byte * 8U;
        const std::uint32_t first = (SHENMUE_MATH_BITS<std::uint32_t>(keys[0]) >> shift) & 0xffU;
        if (counts[byte][first] == count) {
            if (byte == 3 && first >= 128) {
                for (std::uint32_t index = 0; index < count; ++index) {
                    scratch[index] = indices[count - index - 1];
                }
#if SHENMUE_S2_GCC295
                std::uint32_t *const previous = indices;
                indices = scratch;
                scratch = previous;
#else
                std::swap(indices, scratch);
#endif
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
            const std::uint32_t bucket = (SHENMUE_MATH_BITS<std::uint32_t>(keys[source]) >> shift) & 0xffU;
            const std::uint32_t destination = byte == 3 && bucket >= 128 ? --offsets[bucket] : offsets[bucket]++;
            scratch[destination] = source;
        }
#if SHENMUE_S2_GCC295
        std::uint32_t *const previous = indices;
        indices = scratch;
        scratch = previous;
#else
        std::swap(indices, scratch);
#endif
    }
    return indices;
}

#undef SHENMUE_MATH_BITS
#undef SHENMUE_MATH_FLOAT_TYPE
#undef SHENMUE_MATH_NOEXCEPT
#undef SHENMUE_MATH_FABS
#undef SHENMUE_MATH_SQRT
#undef SHENMUE_MATH_ATAN
#undef SHENMUE_MATH_ISNAN
#undef SHENMUE_MATH_INT_MIN
#undef SHENMUE_MATH_INT_MAX

} // namespace shenmue
