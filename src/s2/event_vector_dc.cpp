#include "s2/event_vector_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include <math.h>
#include "s2/scalar_math_dc.hpp"
#include <float.h>
#include "common/native_sqrt.hpp"
#define SHENMUE_SCALAR_NOEXCEPT
#define SHENMUE_SCALAR_ABS ::fabsf
#define SHENMUE_SCALAR_SIGNED_ANGLE(value) static_cast<std::int16_t>(value)

namespace shenmue {
namespace s2 {

// @verified - d
float GetVector3DotProduct(const Vector3 &first, const Vector3 &second) {
    const float *left = first.data();
    const float *right = second.data();
    float product;
    asm volatile("fmov @%1+,fr0\n\tfmov @%1+,fr1\n\tfmov @%1+,fr2\n\tfldi0 fr3\n\t"
                 "fmov @%2+,fr4\n\tfmov @%2+,fr5\n\tfmov @%2+,fr6\n\tfldi0 fr7\n\tfipr fv0,fv4\n\tfmov fr7,%0"
        : "=f"(product), "+r"(left), "+r"(right)
        :
        : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7", "memory");
    return product;
}

// @verified - d
void GetVector3CrossProduct(const Vector3 &first, const Vector3 &second, Vector3 &output) {
    const Vector3 left = first, right = second;
    float z = -(left.y * right.x), y = -(left.x * right.z), x = -(left.z * right.y);
    asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+f"(z) : "f"(left.x), "f"(right.y) : "fr0");
    asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+f"(y) : "f"(left.z), "f"(right.x) : "fr0");
    asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+f"(x) : "f"(left.y), "f"(right.z) : "fr0");
    output.x = x;
    output.y = y;
    output.z = z;
}

// @verified - d
bool TestCharaPositionWithinRange(shen_name name, const Vector3 *first, const Vector3 *second, shen_name relativeTo) {
    float swap;
    Vector3 position;
    Vector3 minimum;
    Vector3 maximum;
    Vector3 offset = {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.0f};
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (relativeTo) {
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        GetCharacterLocalPosition(scenFindChara(control->scenarioTask, relativeTo), &offset);
    }
    const float offsetX = offset.x;
    const float offsetZ = offset.z;
    minimum.x = first->x + offsetX;
    minimum.z = first->z + offsetZ;
    maximum.x = offsetX + second->x;
    maximum.z = offsetZ + second->z;
    if (minimum.x > maximum.x) {
        swap = minimum.x;
        minimum.x = maximum.x;
        maximum.x = swap;
    }
    if (minimum.z > maximum.z) {
        swap = minimum.z;
        minimum.z = maximum.z;
        maximum.z = swap;
    }
    GetCharacterLocalPosition(character, &position);
    if (position.x < minimum.x || position.x > maximum.x || position.z < minimum.z || position.z > maximum.z)
        return false;
    return true;
}

// @verified - d
float GetRandomNumberInRange(float bound) SHENMUE_SCALAR_NOEXCEPT {
    return dc::sdk::RandomFloat() * bound;
}

// @verified - d
std::int32_t shenGetRandomInt32(std::int32_t bound) SHENMUE_SCALAR_NOEXCEPT {
    return static_cast<std::int32_t>(dc::sdk::RandomFloat() * static_cast<float>(bound));
}

// @verified - d
float InterpolateTo(float target, float value, float weight) {
    return (target - value) * weight + value;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float InterpolateFloat(float target, float value, float weight) {
    return InterpolateTo(target, value, weight);
}

std::int32_t InterpolateInteger(std::int32_t from, std::int32_t to, float weight) {
    if (::fabsf(weight) < FLT_EPSILON)
        return from;
    const float difference = static_cast<float>(to - from);
    if (::fabsf(difference) < FLT_EPSILON)
        return from;
    return from + static_cast<std::int32_t>(weight * difference);
}
#endif

// @verified - d
std::int32_t Vector3_ToFixed16EulerAngles(const Vector3 &direction, Vector3s &angles) {
    std::int16_t *angle = &angles.x;
    if (SHENMUE_SCALAR_ABS(direction.x) < FLT_EPSILON && SHENMUE_SCALAR_ABS(direction.y) < FLT_EPSILON && SHENMUE_SCALAR_ABS(direction.z) < FLT_EPSILON)
        return -1;
    const float x = direction.x;
    float planar = direction.z * direction.z;
    asm volatile("fmov %1,fr0\n\tfmac fr0,%1,%0\n\tfsqrt %0" : "+&f"(planar) : "f"(x) : "fr0");
    *angle++ = SHENMUE_SCALAR_SIGNED_ANGLE(dc::sdk::ArcTan2Fixed(direction.y, planar));
    *angle = SHENMUE_SCALAR_SIGNED_ANGLE(static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(direction.x, direction.z)) - 0x8000);
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t Vector3_GetFixedYaw(const Vector3 *direction) {
    return static_cast<std::int16_t>(InverseFixedAtan2(direction->x, direction->z));
}
#endif

// @verified - d
void scenApplyTransRot(const Vector3 *position, const Vector3i *rotation, Vector3 *point) {
    const Vector3 input = *point;
    matrix_stack::PushUnitMatrix();
    if (position)
        Matrix4x4::preTranslate(position->x, position->y, position->z);
    if (rotation->z)
        Matrix4x4::RotateAroundZ_Wrapper(rotation->z);
    if (rotation->y)
        Matrix4x4::RotateAroundY_Wrapper(rotation->y);
    if (rotation->x)
        Matrix4x4::RotateAroundX_Wrapper(rotation->x);
    Matrix4x4::TransformPoint(input, *point);
    PopCurrentMatrix(1);
}

// @verified - d
void TransformScenarioDirection(const Vector3 *position, const Vector3i *rotation, Vector3 *direction) {
    const Vector3 input = *direction;
    matrix_stack::PushUnitMatrix();
    if (position)
        Matrix4x4::preTranslate(position->x, position->y, position->z);
    if (rotation->z)
        Matrix4x4::RotateAroundZ_Wrapper(rotation->z);
    if (rotation->y)
        Matrix4x4::RotateAroundY_Wrapper(rotation->y);
    if (rotation->x)
        Matrix4x4::RotateAroundX_Wrapper(rotation->x);
    Matrix4x4::Transform(input, *direction);
    PopCurrentMatrix(1);
}

// @verified - d
std::int32_t ComputeNormalizedAngleRatio(float ratio) {
    if (::fabsf(ratio) > 1.0f) {
        if (ratio > 0.0f)
            return 0x4000;
        return -0x4000;
    }
    float adjacent = 1.0f - ratio * ratio;
    asm volatile("fsqrt %0" : "+f"(adjacent));
    if (adjacent != 0.0f)
        return static_cast<std::int32_t>(dc::sdk::ArcTanFixed(ratio / adjacent));
    if (ratio > 0.0f)
        return 0x4000;
    return 0xc000;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ComputeInverseAngleRatio(float ratio);

std::int32_t Vector3_ComputePlanarAnglesBetween(const Vector3 &first, const Vector3 &second, Vector3s &angles) {
    std::int16_t *angle = &angles.x;
    const float firstXX = first.x * first.x;
    const float firstYY = first.y * first.y;
    const float firstZZ = first.z * first.z;
    const float secondXX = second.x * second.x;
    const float secondYY = second.y * second.y;
    const float secondZZ = second.z * second.z;
    float firstLength = ::sqrtf(firstXX + firstYY);
    float secondLength = ::sqrtf(secondXX + secondYY);
    *angle++ = static_cast<std::int16_t>(ComputeInverseAngleRatio((first.x * second.x + first.y * second.y) / (firstLength * secondLength)));
    firstLength = ::sqrtf(firstXX + firstZZ);
    secondLength = ::sqrtf(secondXX + secondZZ);
    *angle = static_cast<std::int16_t>(ComputeInverseAngleRatio((first.x * second.x + first.z * second.z) / (firstLength * secondLength)));
    return 0;
}

// @verified - d
std::int32_t Vector3_ComputeAngleBetween(const Vector3 &first, const Vector3 &second) {
    const float dot = first.x * second.x + first.y * second.y + first.z * second.z;
    const float firstLength = NativeSqrt(first.x * first.x + first.y * first.y + first.z * first.z);
    const float secondLength = NativeSqrt(second.x * second.x + second.y * second.y + second.z * second.z);
    const float length = firstLength * secondLength;
    if (::fabsf(length) < 1.1920928955078125e-7f)
        return 0;
    return static_cast<std::int16_t>(ComputeInverseAngleRatio(dot / length));
}

std::int32_t Vector3_ToFixed16EulerAnglesBetween(const Vector3 &from, const Vector3 &to, Vector3s &angles) {
    Vector3 delta;
    delta.x = to.x - from.x;
    delta.y = to.y - from.y;
    delta.z = to.z - from.z;
    return Vector3_ToFixed16EulerAngles(delta, angles);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - d
void Matrix4x4_BuildRotationMatrix(const Vector3 &first, const Vector3 &second) {
    Vector3 axis;
    GetVector3CrossProduct(first, second, axis);
    const float sine = Vector3::NormalizeInPlace(axis);
    const float cosine = GetVector3DotProduct(first, second);
    if (sine != 0.0f) {
        Matrix4x4_BuildAndApplyRotationMatrix(axis, sine, cosine);
    } else if (cosine < 0.0f) {
        axis = (__typeof__(axis)){SHEN_FIELD(x) first.y, SHEN_FIELD(y) first.z, SHEN_FIELD(z) first.x};
        Matrix4x4_BuildAndApplyRotationMatrix(axis, 0.0f, -1.0f);
    }
}
#endif

// @verified - d
void ComputeVectorAlignmentRotation(const Vector3 &first, const Vector3 &second, Vector3i *rotation) {
    matrix_stack::PushUnitMatrix();
    Matrix4x4_BuildRotationMatrix(first, second);
    Matrix4x4 transform;
    matrix_stack::GetMatrix(&transform);
    ExtractMatrixEulerAnglesFixed(&transform, rotation);
    PopCurrentMatrix(1);
}

// @verified - d
bool ComputeAngle(const Vector3 &origin, float radius, std::int32_t yaw, std::int32_t halfAngle, const Vector3 &point) {
    Vector3 delta;
    delta.x = point.x - origin.x;
    delta.y = point.y - origin.y;
    delta.z = point.z - origin.z;
    if (Vector3::CalcMagnitude(delta) > radius)
        return false;
    std::int32_t difference = (yaw - InverseFixedAtan2(delta.x, delta.z)) & 0xffff;
    if (difference > 0x7fff)
        difference = (~difference + 1) & 0xffff;
    if (difference >= halfAngle)
        return false;
    return true;
}

} // namespace s2
} // namespace shenmue

extern "C" float __attribute__((renesas)) shenmue_s2_event_GetRandomNumberInRange(float bound) {
    return shenmue::s2::GetRandomNumberInRange(bound);
}

#undef SHENMUE_SCALAR_NOEXCEPT
#undef SHENMUE_SCALAR_ABS
#undef SHENMUE_SCALAR_SIGNED_ANGLE

extern "C" {

bool __attribute__((renesas)) shenmue_s2_event_ComputeAngle(
    const shenmue::Vector3 *origin, float radius, std::int16_t yaw, std::int32_t halfAngle, const shenmue::Vector3 *point) {
    return shenmue::s2::ComputeAngle(*origin, radius, yaw, halfAngle, *point);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_ComputeNormalizedAngleRatio(float ratio) {
    return shenmue::s2::ComputeNormalizedAngleRatio(ratio);
}
}

#endif
