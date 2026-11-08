#pragma once

#include "common/math.hpp"
#include <cmath>
#include <cstdint>

namespace shenmue::s2 {

std::uint16_t FloatToShort(float value);
std::int32_t ComputeTriangleNormal(const Vector3 &first, const Vector3 &second, const Vector3 &third, Vector3 &normal);
void NormalizeVector3(Vector3 *vector);
extern Matrix4x4 g_modelInverseTransform;
void InvertModelWorkTransform();

// Matrix4x4::BuildAndApplyRotationMatrix and ApplyAngleRotationToVector: rotate the
// work matrix about a normalized axis through the retained tmpRotationXform.
extern Matrix4x4 g_motionAxisRotationMatrix;
void Matrix4x4_BuildAndApplyRotationMatrix(const Vector3 &axis, float sine, float cosine);
void Matrix4x4_ApplyAngleRotationToVector(const Vector3 &axis, std::uint16_t angle);
// Matrix4x4::BuildRotationMatrix: turns `from` onto `to` about their normalized cross product.
void Matrix4x4_BuildRotationMatrix(const Vector3 &from, const Vector3 &to);


std::int32_t ComputeNormalizedAngleRatio(float ratio);
std::int32_t ComputeInverseAngleRatio(float ratio);
std::int32_t Vector3_ComputeAngleBetween(const Vector3 &first, const Vector3 &second);

float FixedAngleSin(std::uint16_t angle);
float FixedAngleCos(std::uint16_t angle);

float GetPlanarDistance(const Vector3 *first, const Vector3 *second);
float GetEuclideanDistance(const Vector3 *first, const Vector3 *second);
float GetSquaredDistanceXZ(const Vector3 *first, const Vector3 *second);
float GetSquaredEuclideanDistance(const Vector3 *first, const Vector3 *second);
std::int16_t InverseFixedAtan2(float first, float direction);


std::int32_t Vector3_ToFixed16EulerAngles(const Vector3 &direction, Vector3s &angles);
void ExtractMatrixEulerAnglesFixed(const Matrix4x4 *transform, Vector3i *rotation);

// atanf2_i16 uses quadrant-specific quotients and adds 0.5 before truncation.
// std::atan2 followed by a fixed-angle conversion does not preserve that rule.
// @verified - p
inline std::uint16_t FixedAtan2(float first, float direction) {
    constexpr float fixedTurn = 65536.0f;
    constexpr float twoPi = 6.2831855f;
    const auto roundedRatio = [=](float numerator, float denominator) {
        return static_cast<std::int32_t>(std::atan(numerator / denominator) * fixedTurn / twoPi + 0.5f);
    };

    if (direction < first) {
        if (direction < -first) {
            if (direction == 0.0f)
                return 0x8000U;
            return static_cast<std::uint16_t>(roundedRatio(first, direction) + 0x8000);
        }
        if (first == 0.0f)
            return 0x4000U;
        return static_cast<std::uint16_t>(0x4000 - roundedRatio(direction, first));
    }
    if (direction < -first) {
        if (first == 0.0f)
            return 0xc000U;
        return static_cast<std::uint16_t>(-0x4000 - roundedRatio(direction, first));
    }
    if (direction == 0.0f)
        return 0;
    return static_cast<std::uint16_t>(roundedRatio(first, direction));
}

// abs16: magnitude of a wrapped 16-bit angle difference.
inline std::uint16_t Abs16(std::int32_t value) {
    const auto angle = static_cast<std::int16_t>(value);
    return static_cast<std::uint16_t>(angle < 0 ? -angle : angle);
}

inline std::int16_t AtanToFixedAngle(float ratio) {
    return static_cast<std::int16_t>(static_cast<std::int32_t>(std::atan(ratio) * 65536.0f / 6.2831855f + 0.5f));
}

} // namespace shenmue::s2
