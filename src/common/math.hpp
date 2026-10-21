#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "shenmue.h"

#define SHEN_PI 3.14159265358979323846
#define DEG_TO_ANG(x) (uint16_t)((x * 8192) / 45)

#ifdef _DREAMCAST
template <typename T> using Ptr = std::uint32_t;
#else
template <typename T> using Ptr = T *;
#endif

namespace shenmue {

template <typename T> struct Vector2T {
    T x;
    T y;

    constexpr T &operator[](std::size_t index) noexcept {
        return index == 0 ? x : y;
    }

    constexpr const T &operator[](std::size_t index) const noexcept {
        return index == 0 ? x : y;
    }

    constexpr T *data() noexcept {
        return &x;
    }
    constexpr const T *data() const noexcept {
        return &x;
    }
    constexpr T *begin() noexcept {
        return data();
    }
    constexpr const T *begin() const noexcept {
        return data();
    }
    constexpr T *end() noexcept {
        return data() + 2;
    }
    constexpr const T *end() const noexcept {
        return data() + 2;
    }
    static constexpr std::size_t size() noexcept {
        return 2;
    }
};

template <typename T> struct Vector3T {
    T x;
    T y;
    T z;

    constexpr T &operator[](std::size_t index) noexcept {
        return index == 0 ? x : index == 1 ? y : z;
    }

    constexpr const T &operator[](std::size_t index) const noexcept {
        return index == 0 ? x : index == 1 ? y : z;
    }

    constexpr T *data() noexcept {
        return &x;
    }
    constexpr const T *data() const noexcept {
        return &x;
    }
    constexpr T *begin() noexcept {
        return data();
    }
    constexpr const T *begin() const noexcept {
        return data();
    }
    constexpr T *end() noexcept {
        return data() + 3;
    }
    constexpr const T *end() const noexcept {
        return data() + 3;
    }
    static constexpr std::size_t size() noexcept {
        return 3;
    }

    static constexpr Vector3T Add(Vector3T first, Vector3T second) requires std::is_same_v<T, float> {
        return {.x = first.x + second.x, .y = first.y + second.y, .z = first.z + second.z};
    }
    static constexpr Vector3T Subtract(Vector3T first, Vector3T second) requires std::is_same_v<T, float> {
        return {.x = first.x - second.x, .y = first.y - second.y, .z = first.z - second.z};
    }
    static constexpr Vector3T Scale(Vector3T value, float scale) requires std::is_same_v<T, float> {
        return {.x = value.x * scale, .y = value.y * scale, .z = value.z * scale};
    }
    static constexpr float Dot(Vector3T first, Vector3T second) requires std::is_same_v<T, float> {
        return (first.x * second.x + first.y * second.y) + first.z * second.z;
    }
    static constexpr Vector3T Cross(Vector3T first, Vector3T second) requires std::is_same_v<T, float> {
        return {.x = first.y * second.z - first.z * second.y, .y = first.z * second.x - first.x * second.z, .z = first.x * second.y - first.y * second.x};
    }
    static float NormalizeInPlace(Vector3T &value) requires std::is_same_v<T, float>;
    static float CalcMagnitude(const Vector3T &vector) requires std::is_same_v<T, float>;
};

using Vector2 = Vector2T<float>;
using Vector2i = Vector2T<std::int32_t>;
using Vector2s = Vector2T<std::int16_t>;
using Vector3 = Vector3T<float>;
using Vector3i = Vector3T<std::int32_t>;
using Vector3s = Vector3T<std::int16_t>;

struct Matrix4x4;

struct Vector4 {
    static Vector4 *ProjectPoint(const Matrix4x4 *transform, Vector4 *output, const Vector4 *input);
    float x;
    float y;
    float z;
    float w;

    constexpr float &operator[](std::size_t index) noexcept {
        return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
    }

    constexpr const float &operator[](std::size_t index) const noexcept {
        return index == 0 ? x : index == 1 ? y : index == 2 ? z : w;
    }

    constexpr float *data() noexcept {
        return &x;
    }
    constexpr const float *data() const noexcept {
        return &x;
    }
    constexpr float *begin() noexcept {
        return data();
    }
    constexpr const float *begin() const noexcept {
        return data();
    }
    constexpr float *end() noexcept {
        return data() + 4;
    }
    constexpr const float *end() const noexcept {
        return data() + 4;
    }
    static constexpr std::size_t size() noexcept {
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
    static Matrix4x4 *GetTransformMatrix(Matrix4x4 *output = nullptr);
    static void SetActiveTransform(const Matrix4x4 *transform = nullptr);
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
constexpr std::int32_t fixed16_angle_magnitude(std::int32_t angle) noexcept {
    const auto value = static_cast<std::int16_t>(angle);
    return value < 0 ? -static_cast<std::int32_t>(value) : value;
}

constexpr std::uint16_t fixed16_cosine_angle(std::uint16_t angle) noexcept {
    const std::uint16_t adjusted = angle <= 0x8000u ? angle : static_cast<std::uint16_t>(-angle);
    return static_cast<std::uint16_t>(0x4000u - adjusted);
}

// The original code's linear congruential step (the ANSI C rand() constants), which its owners inline over their own seeds.
constexpr std::uint32_t LcgStep(std::uint32_t seed) noexcept {
    return seed * 1103515245U + 12345U;
}

// The original code's fixed-point sine: the half angle's tangent from a continued fraction, then the double-angle formula.
constexpr float sin_fixed16(std::int16_t angle) noexcept {
    constexpr float twoPi = 6.2831855f;
    constexpr float fixed16Scale = 0.000015258789f;
    constexpr float half = 0.5f;
    constexpr float halfPi = 1.5707964f;

    const auto unsignedAngle = static_cast<std::uint16_t>(angle);
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
constexpr float cos_fixed16(std::uint16_t angle) noexcept {
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
std::uint32_t *RadixSortIndicesBy4ByteKey(const float *keys, std::uint32_t count, std::uint32_t *&indices, std::uint32_t *&scratch) noexcept;

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

} // namespace shenmue
