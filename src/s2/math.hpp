#pragma once

#include "common/math.hpp"
#if SHENMUE_S2_GCC295
#include <math.h>
#define SHENMUE_S2_ATAN ::atanf
#else
#include <cmath>
#include <cstdint>
#define SHENMUE_S2_ATAN std::atan
#endif

namespace shenmue {
namespace s2 {

std::uint16_t FloatToShort(float value);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InverseTransformPointByMatrix(const Matrix4x4 *transform, Vector3 *position);
void TransformCharacterPoseByMatrix(const Matrix4x4 *transform, Vector3 *position, Vector3i *rotation);
#endif
std::int32_t ComputeTriangleNormal(const Vector3 &first, const Vector3 &second, const Vector3 &third, Vector3 &normal);
void NormalizeVector3(Vector3 *vector);
void NormalizeVector4(Vector4 *vector);
extern Matrix4x4 g_modelInverseTransform;
void InvertModelWorkTransform();

// Matrix4x4::BuildAndApplyRotationMatrix and ApplyAngleRotationToVector: rotate the
// work matrix about a normalized axis through the retained tmpRotationXform.
extern Matrix4x4 g_motionAxisRotationMatrix;
void Matrix4x4_BuildAndApplyRotationMatrix(const Vector3 &axis, float sine, float cosine);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void Matrix4x4_ApplyNormalizedAngleRotation(const Vector3 &axis, std::int32_t angle);
void Matrix4x4_ApplyAngleRotationToVector(const Vector3 &axis, std::int32_t angle) asm(
    "__ZN7shenmue2s236Matrix4x4_ApplyAngleRotationToVectorERKNS_8Vector3TIfEEt");
#else
void Matrix4x4_ApplyAngleRotationToVector(const Vector3 &axis, std::uint16_t angle);
#endif
// Matrix4x4::BuildRotationMatrix: turns `from` onto `to` about their normalized cross product.
void Matrix4x4_BuildRotationMatrix(const Vector3 &from, const Vector3 &to);
// Native fixed-angle arcsine and arccosine of a ratio, and Vector3::ComputeAngleBetween: the 0..0x8000 angle between two
// vectors, 0 when either is degenerate.
std::int32_t ComputeNormalizedAngleRatio(float ratio);
std::int32_t ComputeInverseAngleRatio(float ratio);
std::int32_t Vector3_ComputeAngleBetween(const Vector3 &first, const Vector3 &second);
// Native sin_fp16 and cos_fp16 over the 65536-step fixed angle.
float FixedAngleSin(std::uint16_t angle);
float FixedAngleCos(std::uint16_t angle);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
float FixedAngleTangent(std::int32_t angle);
#endif

float GetPlanarDistance(const Vector3 *first, const Vector3 *second);
float GetEuclideanDistance(const Vector3 *first, const Vector3 *second);
float GetSquaredDistanceXZ(const Vector3 *first, const Vector3 *second);
float GetSquaredEuclideanDistance(const Vector3 *first, const Vector3 *second);
#if SHENMUE_S2_GCC295
std::int32_t InverseFixedAtan2(float first, float direction);
#else
std::int16_t InverseFixedAtan2(float first, float direction);
#endif
// Native Vector3::ToFixed16EulerAngles: X is the pitch over the planar length, Y the yaw turned half a revolution;
// Z is left untouched. A direction whose three components are all below FLT_EPSILON returns -1 without writing.
std::int32_t Vector3_ToFixed16EulerAngles(const Vector3 &direction, Vector3s &angles);
void ExtractMatrixEulerAnglesFixed(const Matrix4x4 *transform, Vector3i *rotation);

#if SHENMUE_S2_GCC295
inline std::int32_t RoundFixedAngleRatio(float numerator, float denominator) {
    return static_cast<std::int32_t>(::atanf(numerator / denominator) * 65536.0f / 6.2831855f + 0.5f);
}
#define SHENMUE_S2_ROUNDED_RATIO RoundFixedAngleRatio
#else
#define SHENMUE_S2_ROUNDED_RATIO roundedRatio
#endif

// atanf2_i16 uses quadrant-specific quotients and adds 0.5 before truncation.
// std::atan2 followed by a fixed-angle conversion does not preserve that rule.
// @verified - p
inline std::uint16_t FixedAtan2(float first, float direction) {
#if !SHENMUE_S2_GCC295
    constexpr float fixedTurn = 65536.0f;
    constexpr float twoPi = 6.2831855f;
    const auto roundedRatio = [=](float numerator, float denominator) {
        return static_cast<std::int32_t>(std::atan(numerator / denominator) * fixedTurn / twoPi + 0.5f);
    };
#endif

    if (direction < first) {
        if (direction < -first) {
            if (direction == 0.0f)
                return 0x8000U;
            return static_cast<std::uint16_t>(SHENMUE_S2_ROUNDED_RATIO(first, direction) + 0x8000);
        }
        if (first == 0.0f)
            return 0x4000U;
        return static_cast<std::uint16_t>(0x4000 - SHENMUE_S2_ROUNDED_RATIO(direction, first));
    }
    if (direction < -first) {
        if (first == 0.0f)
            return 0xc000U;
        return static_cast<std::uint16_t>(-0x4000 - SHENMUE_S2_ROUNDED_RATIO(direction, first));
    }
    if (direction == 0.0f)
        return 0;
    return static_cast<std::uint16_t>(SHENMUE_S2_ROUNDED_RATIO(first, direction));
}

// abs16: magnitude of a wrapped 16-bit angle difference.
inline std::uint16_t Abs16(std::int32_t value) {
#if SHENMUE_S2_GCC295
    const std::int16_t angle = static_cast<std::int16_t>(value);
#else
    const auto angle = static_cast<std::int16_t>(value);
#endif
    return static_cast<std::uint16_t>(angle < 0 ? -angle : angle);
}

inline std::int16_t AtanToFixedAngle(float ratio) {
    return static_cast<std::int16_t>(static_cast<std::int32_t>(SHENMUE_S2_ATAN(ratio) * 65536.0f / 6.2831855f + 0.5f));
}

} // namespace s2
} // namespace shenmue

#undef SHENMUE_S2_ATAN
#undef SHENMUE_S2_ROUNDED_RATIO
