#pragma once

#include "shenmue.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#define SHENMUE_MATH_FUNCTION constexpr
#define SHENMUE_MATH_CONSTANT constexpr
#define SHENMUE_MATH_NOEXCEPT noexcept
#define SHENMUE_MATH_FLOAT_TYPE requires std::is_same_v<T, float>
#define SHENMUE_MATH_NULL nullptr

#define SHEN_PI 3.14159265358979323846
#define DEG_TO_ANG(x) (uint16_t)((x * 8192) / 45)

#if defined(_DREAMCAST)
template <typename T> using Ptr = std::uint32_t;
#else
template <typename T> using Ptr = T *;
#endif

namespace shenmue {

template <typename T> struct Vector2T {
    T x;
    T y;

    SHENMUE_MATH_FUNCTION T &operator[](std::size_t index) SHENMUE_MATH_NOEXCEPT {
        return index == 0 ? x : y;
    }

    SHENMUE_MATH_FUNCTION const T &operator[](std::size_t index) const SHENMUE_MATH_NOEXCEPT {
        return index == 0 ? x : y;
    }

    SHENMUE_MATH_FUNCTION T *data() SHENMUE_MATH_NOEXCEPT {
        return &x;
    }
    SHENMUE_MATH_FUNCTION const T *data() const SHENMUE_MATH_NOEXCEPT {
        return &x;
    }
    SHENMUE_MATH_FUNCTION T *begin() SHENMUE_MATH_NOEXCEPT {
        return data();
    }
    SHENMUE_MATH_FUNCTION const T *begin() const SHENMUE_MATH_NOEXCEPT {
        return data();
    }
    SHENMUE_MATH_FUNCTION T *end() SHENMUE_MATH_NOEXCEPT {
        return data() + 2;
    }
    SHENMUE_MATH_FUNCTION const T *end() const SHENMUE_MATH_NOEXCEPT {
        return data() + 2;
    }
    static SHENMUE_MATH_FUNCTION std::size_t size() SHENMUE_MATH_NOEXCEPT {
        return 2;
    }
};

template <typename T> struct Vector3T {
    T x;
    T y;
    T z;

    SHENMUE_MATH_FUNCTION T &operator[](std::size_t index) SHENMUE_MATH_NOEXCEPT {
        return index == 0 ? x : index == 1 ? y : z;
    }

    SHENMUE_MATH_FUNCTION const T &operator[](std::size_t index) const SHENMUE_MATH_NOEXCEPT {
        return index == 0 ? x : index == 1 ? y : z;
    }

    SHENMUE_MATH_FUNCTION T *data() SHENMUE_MATH_NOEXCEPT {
        return &x;
    }
    SHENMUE_MATH_FUNCTION const T *data() const SHENMUE_MATH_NOEXCEPT {
        return &x;
    }
    SHENMUE_MATH_FUNCTION T *begin() SHENMUE_MATH_NOEXCEPT {
        return data();
    }
    SHENMUE_MATH_FUNCTION const T *begin() const SHENMUE_MATH_NOEXCEPT {
        return data();
    }
    SHENMUE_MATH_FUNCTION T *end() SHENMUE_MATH_NOEXCEPT {
        return data() + 3;
    }
    SHENMUE_MATH_FUNCTION const T *end() const SHENMUE_MATH_NOEXCEPT {
        return data() + 3;
    }
    static SHENMUE_MATH_FUNCTION std::size_t size() SHENMUE_MATH_NOEXCEPT {
        return 3;
    }

    static SHENMUE_MATH_FUNCTION Vector3T Add(Vector3T first, Vector3T second) SHENMUE_MATH_FLOAT_TYPE {
        Vector3T result = {first.x + second.x, first.y + second.y, first.z + second.z};
        return result;
    }
    static SHENMUE_MATH_FUNCTION Vector3T Subtract(Vector3T first, Vector3T second) SHENMUE_MATH_FLOAT_TYPE {
        Vector3T result = {first.x - second.x, first.y - second.y, first.z - second.z};
        return result;
    }
    static SHENMUE_MATH_FUNCTION Vector3T Scale(Vector3T value, float scale) SHENMUE_MATH_FLOAT_TYPE {
        Vector3T result = {value.x * scale, value.y * scale, value.z * scale};
        return result;
    }
    static SHENMUE_MATH_FUNCTION float Dot(Vector3T first, Vector3T second) SHENMUE_MATH_FLOAT_TYPE {
        return (first.x * second.x + first.y * second.y) + first.z * second.z;
    }
    static SHENMUE_MATH_FUNCTION Vector3T Cross(Vector3T first, Vector3T second) SHENMUE_MATH_FLOAT_TYPE {
        Vector3T result = {first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x};
        return result;
    }
    static float NormalizeInPlace(Vector3T &value) SHENMUE_MATH_FLOAT_TYPE;
    static float CalcMagnitude(const Vector3T &vector) SHENMUE_MATH_FLOAT_TYPE;
};

typedef Vector2T<float> Vector2;
typedef Vector2T<std::int32_t> Vector2i;
typedef Vector2T<std::int16_t> Vector2s;
typedef Vector3T<float> Vector3;



typedef Vector3T<std::int32_t> Vector3i;
typedef Vector3T<std::int16_t> Vector3s;

struct Matrix4x4;

struct Vector4 {
    static Vector4 *ProjectPoint(const Matrix4x4 *transform, Vector4 *output, const Vector4 *input);
    float x;
    float y;
    float z;
    float w;

    SHENMUE_MATH_FUNCTION float &operator[](std::size_t index) SHENMUE_MATH_NOEXCEPT {
        return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
    }

    SHENMUE_MATH_FUNCTION const float &operator[](std::size_t index) const SHENMUE_MATH_NOEXCEPT {
        return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
    }

    SHENMUE_MATH_FUNCTION float *data() SHENMUE_MATH_NOEXCEPT {
        return &x;
    }
    SHENMUE_MATH_FUNCTION const float *data() const SHENMUE_MATH_NOEXCEPT {
        return &x;
    }
    SHENMUE_MATH_FUNCTION float *begin() SHENMUE_MATH_NOEXCEPT {
        return data();
    }
    SHENMUE_MATH_FUNCTION const float *begin() const SHENMUE_MATH_NOEXCEPT {
        return data();
    }
    SHENMUE_MATH_FUNCTION float *end() SHENMUE_MATH_NOEXCEPT {
        return data() + 4;
    }
    SHENMUE_MATH_FUNCTION const float *end() const SHENMUE_MATH_NOEXCEPT {
        return data() + 4;
    }
    static SHENMUE_MATH_FUNCTION std::size_t size() SHENMUE_MATH_NOEXCEPT {
        return 4;
    }
};

struct Matrix4x4 {
    Vector4 rows[4];

    static void preTranslate(float dx, float dy, float dz);
    static Matrix4x4 Multiply(const Matrix4x4 &left, const Matrix4x4 &right);
    static void preTranslate(Matrix4x4 &matrix, float x, float y, float z);
    static void RotateAroundX(float sine, float cosine);
    static void RotateAroundY(float sine, float cosine);
    static void RotateAroundZ(float sine, float cosine);
    static void RotateAroundX_Wrapper(std::uint16_t angle);
    static void RotateAroundY_Wrapper(std::uint16_t angle);
    static void RotateAroundZ_Wrapper(std::uint16_t angle);
    static void SetViewMatrixFromEuler(std::int32_t rotationY, std::int32_t rotationX, std::int32_t rotationZ);
    static void RotateZY(const Vector3 &direction);
    static void Scale(float x, float y, float z);
    static void NormalizeAndSetTransformMatrix();
    static Matrix4x4 *GetTransformMatrix(Matrix4x4 *output = SHENMUE_MATH_NULL);
    static void SetActiveTransform(const Matrix4x4 *transform = SHENMUE_MATH_NULL);
    static void SetPrePassToTempTransform();
    static void Pop(std::int32_t count = 1);
    static void TransformPoint(const Vector3 &input, Vector3 &output);
    static void Transform(const Vector3 &input, Vector3 &output);
    static void InvertRigidTransform();
    static bool SetActiveTransformToPrePass();
    static void MultiplyWithPrePassMatrix(const Matrix4x4 *lhs);
    static void Multiply(const Matrix4x4 *lhs);
    static void ComputeColumnNorms(const Matrix4x4 *matrix, Vector4 *output);
    static Vector4 *ExtractEulerAnglesFromMatrix(const Matrix4x4 *matrix, Vector4 *output);
    static bool invertMatrix(const Matrix4x4 *source, Matrix4x4 *output);
    static Matrix4x4 *MakeFromQuat(Matrix4x4 *output, const Vector4 *first, const Vector4 *second);
    static void InvertInPlace(Matrix4x4 *matrix);
    static void AffineProjectionSwap();
    static void RotateMatrixCounterClockwise90();
};

// The size of a 16-bit angle, taken as signed (0x8000 counts as 0x8000).
SHENMUE_MATH_FUNCTION std::int32_t fixed16_angle_magnitude(std::int32_t angle) SHENMUE_MATH_NOEXCEPT {
    const std::int16_t value = static_cast<std::int16_t>(angle);
    return value < 0 ? -static_cast<std::int32_t>(value) : value;
}

SHENMUE_MATH_FUNCTION std::uint16_t fixed16_cosine_angle(std::uint16_t angle) SHENMUE_MATH_NOEXCEPT {
    const std::uint16_t adjusted = angle <= 0x8000u ? angle : static_cast<std::uint16_t>(-angle);
    return static_cast<std::uint16_t>(0x4000u - adjusted);
}

// The original code's linear congruential step (the ANSI C rand() constants), which its owners inline over their own seeds.
SHENMUE_MATH_FUNCTION std::uint32_t LcgStep(std::uint32_t seed) SHENMUE_MATH_NOEXCEPT {
    return seed * 1103515245U + 12345U;
}

// The original code's fixed-point sine: the half angle's tangent from a continued fraction, then the double-angle formula.
SHENMUE_MATH_FUNCTION float sin_fixed16(std::int16_t angle) SHENMUE_MATH_NOEXCEPT {
    SHENMUE_MATH_CONSTANT float twoPi = 6.2831855f;
    SHENMUE_MATH_CONSTANT float fixed16Scale = 0.000015258789f;
    SHENMUE_MATH_CONSTANT float half = 0.5f;
    SHENMUE_MATH_CONSTANT float halfPi = 1.5707964f;

    const std::uint16_t unsignedAngle = static_cast<std::uint16_t>(angle);
    float reduced = (static_cast<float>(unsignedAngle) * twoPi) * fixed16Scale * half;
    const auto quadrant = static_cast<std::int32_t>(static_cast<double>(reduced / halfPi) + 0.5);
    reduced -= static_cast<float>(quadrant) * halfPi;

    const float square = reduced * reduced;
    float fraction = 0.0f;
    for (std::int32_t divisor = 11; divisor >= 3; divisor -= 2) {
        fraction = square / (static_cast<float>(divisor) - fraction);
    }
    const float tangent = reduced / (1.0f - fraction);
    float result = (tangent + tangent) / ((tangent * tangent) + 1.0f);
    if ((quadrant & 1) != 0) {
        result = -result;
    }
    return result;
}

// The fixed-point cosine the original code builds from sin_fixed16 and the folded quarter-turn angle.
SHENMUE_MATH_FUNCTION float cos_fixed16(std::uint16_t angle) SHENMUE_MATH_NOEXCEPT {
    return sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(angle)));
}

static_assert(sin_fixed16(0) == 0.0f && cos_fixed16(0) > 0.99999f);

void TransformBounds(Vector3 &minimum, Vector3 &maximum, const Matrix4x4 &transform);
float BoundsDistanceSquared(const Vector3 &minimum, const Vector3 &maximum, const Vector3 &position);
bool IsAabbInsideSelectedFrustumPlanes(const std::array<Vector4, 6> &planes, const Vector3 &minimum, const Vector3 &maximum, std::uint8_t skipPlanePairs);

std::uint16_t atan2_fixed16(float y, float x);
std::int16_t asin_fixed16(float value);
float cot_fixed16(std::uint16_t angle);
float ceil_float(float value);
std::int32_t TruncateToInt32(float value);
float fast_logf_approx(float value);
float fast_ldexpf(float value, std::int32_t exponent);
float fast_expf_approx(float value);
float fast_powf_approx(float base, float exponent);
std::uint32_t *RadixSortIndicesBy4ByteKey(const float *keys, std::uint32_t count, std::uint32_t *&indices, std::uint32_t *&scratch) SHENMUE_MATH_NOEXCEPT;

static_assert(sizeof(Vector2) == 0x8);
static_assert(sizeof(Vector3) == 0xc);
static_assert(sizeof(Vector3i) == 0xc);
static_assert(sizeof(Vector3s) == 0x6);
static_assert(sizeof(Vector4) == 0x10);
static_assert(sizeof(Matrix4x4) == 0x40);
static_assert(std::is_standard_layout_v<Vector2>);
static_assert(std::is_standard_layout_v<Vector3>);
static_assert(std::is_standard_layout_v<Vector4>);
static_assert(std::is_standard_layout_v<Matrix4x4>);
static_assert(std::is_trivially_copyable_v<Vector2>);
static_assert(std::is_trivially_copyable_v<Vector3>);
static_assert(std::is_trivially_copyable_v<Vector4>);
static_assert(std::is_trivially_copyable_v<Matrix4x4>);


#undef SHENMUE_MATH_FUNCTION
#undef SHENMUE_MATH_CONSTANT
#undef SHENMUE_MATH_NOEXCEPT
#undef SHENMUE_MATH_FLOAT_TYPE
#undef SHENMUE_MATH_NULL

} // namespace shenmue
