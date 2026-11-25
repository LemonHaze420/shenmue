#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/math.hpp"

#include "s2/character_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#else
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"

#include <bit>
#if defined(__SSE__) || defined(_M_X64) || defined(_M_IX86)
#include <xmmintrin.h>
#endif
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

Matrix4x4 g_modelInverseTransform{};

Matrix4x4 g_motionAxisRotationMatrix{};

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t ComputeTriangleNormal(const Vector3 &first, const Vector3 &second, const Vector3 &third, Vector3 &normal);
void NormalizeVector3(Vector3 *vector);
void Matrix4x4_BuildAndApplyRotationMatrix(const Vector3 &axis, float sine, float cosine);
void Matrix4x4_BuildRotationMatrix(const Vector3 &from, const Vector3 &to);
void Matrix4x4_ApplyAngleRotationToVector(const Vector3 &axis, std::uint16_t angle);
std::int32_t ComputeInverseAngleRatio(float ratio);
float GetSquaredDistanceXZ(const Vector3 *first, const Vector3 *second);
float GetSquaredEuclideanDistance(const Vector3 *first, const Vector3 *second);
float GetPlanarDistance(const Vector3 *first, const Vector3 *second);
float GetEuclideanDistance(const Vector3 *first, const Vector3 *second);
#if SHENMUE_S2_GCC295
std::int32_t InverseFixedAtan2(float first, float direction);
#else
std::int16_t InverseFixedAtan2(float first, float direction);
#endif
void ExtractMatrixEulerAnglesFixed(const Matrix4x4 *transform, Vector3i *rotation);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint16_t FloatToShort(float value) {
#if defined(__SSE__) || defined(_M_X64) || defined(_M_IX86)
    return static_cast<std::uint16_t>(_mm_cvttss_si32(_mm_set_ss(value)));
#else
    const std::int32_t truncated = value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : INT32_MIN;
    return static_cast<std::uint16_t>(truncated);
#endif
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void NormalizeVector3(Vector3 *vector) {
    const float x = vector->x, y = vector->y, z = vector->z;
    const float length = __builtin_sqrtf((x * x + y * y) + z * z);
    if (length > 0.0f) {
        const float reciprocal = 1.0f / length;
        vector->x = x * reciprocal;
        vector->y = y * reciprocal;
        vector->z = z * reciprocal;
    }
}
#endif

#else

// @verified - p
void NormalizeVector3(Vector3 *vector) {
    const float x = vector->x, y = vector->y, z = vector->z;
    const float length = std::sqrt((x * x + y * y) + z * z);
    if (length > 0.0f) {
        const float reciprocal = 1.0f / length;
        vector->x = x * reciprocal;
        vector->y = y * reciprocal;
        vector->z = z * reciprocal;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void NormalizeVector4(Vector4 *vector) {
    const float x = vector->x, y = vector->y, z = vector->z, w = vector->w;
    const float length = std::sqrt(((x * x + y * y) + z * z) + w * w);
    *vector = {.x = x * (1.0f / length), .y = y * (1.0f / length), .z = z * (1.0f / length), .w = w * (1.0f / length)};
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void InvertModelWorkTransform() {
    g_modelInverseTransform = workTransformMatrix;
    for (std::size_t row = 0; row < 3; ++row) {
        Vector3 axis{g_modelInverseTransform.rows[row].x, g_modelInverseTransform.rows[row].y, g_modelInverseTransform.rows[row].z};
        NormalizeVector3(&axis);
        g_modelInverseTransform.rows[row].x = axis.x;
        g_modelInverseTransform.rows[row].y = axis.y;
        g_modelInverseTransform.rows[row].z = axis.z;
    }
    Matrix4x4 &matrix = g_modelInverseTransform;
    matrix.rows[0].w = matrix.rows[1].w = matrix.rows[2].w = 0.0f;
    const float x = -((matrix.rows[0].x * matrix.rows[3].x + matrix.rows[0].y * matrix.rows[3].y) + matrix.rows[0].z * matrix.rows[3].z);
    const float y = -((matrix.rows[3].x * matrix.rows[1].x + matrix.rows[3].y * matrix.rows[1].y) + matrix.rows[3].z * matrix.rows[1].z);
    const float z = -((matrix.rows[2].x * matrix.rows[3].x + matrix.rows[2].y * matrix.rows[3].y) + matrix.rows[2].z * matrix.rows[3].z);
    const float xy = matrix.rows[0].y, xz = matrix.rows[0].z, yz = matrix.rows[1].z;
    matrix.rows[0].y = matrix.rows[1].x;
    matrix.rows[0].z = matrix.rows[2].x;
    matrix.rows[1].x = xy;
    matrix.rows[1].z = matrix.rows[2].y;
    matrix.rows[2].x = xz;
    matrix.rows[2].y = yz;
    matrix.rows[3] = {x, y, z, 1.0f};
    workTransformMatrix = matrix;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t ComputeTriangleNormal(const Vector3 &first, const Vector3 &second, const Vector3 &third, Vector3 &normal) {
    const Vector3 incoming = {second.x - first.x, second.y - first.y, second.z - first.z};
    const Vector3 outgoing = {third.x - second.x, third.y - second.y, third.z - second.z};
    dc::sdk::njOuterProduct(&outgoing, &incoming, &normal);
    const float squaredLength = dc::sdk::njInnerProduct(&normal, &normal);
    if (squaredLength == 0.0f)
        return 0;
    const float reciprocal = dc::sdk::njInvertSqrt(squaredLength);
    normal.x *= reciprocal;
    normal.y *= reciprocal;
    normal.z *= reciprocal;
    return 1;
}
#endif

#else

// @verified - p
std::int32_t ComputeTriangleNormal(const Vector3 &first, const Vector3 &second, const Vector3 &third, Vector3 &normal) {
    const float firstY = second.y - first.y, firstZ = second.z - first.z, firstX = second.x - first.x;
    const float secondY = third.y - second.y, secondZ = third.z - second.z, secondX = third.x - second.x;
    const float x = secondY * firstZ - secondZ * firstY;
    const float y = secondZ * firstX - secondX * firstZ;
    const float z = secondX * firstY - secondY * firstX;
    normal = {x, y, z};
    const float square = (y * y + x * x) + z * z;
    if (square == 0.0f)
        return 0;
    const float reciprocal = 1.0f / std::sqrt(square);
    normal = {x * reciprocal, y * reciprocal, z * reciprocal};
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void Matrix4x4_ApplyNormalizedAngleRotation(const Vector3 &axis, std::int32_t angle) {
    const float sine = dc::sdk::njSin(angle);
    const float cosine = dc::sdk::njCos(angle);
    Matrix4x4_BuildAndApplyRotationMatrix(axis, sine, cosine);
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void Matrix4x4_ApplyAngleRotationToVector(const Vector3 &axis, std::int32_t angle) {
    Vector3 normalized = axis;
    dc::sdk::njUnitVector(&normalized);
    Matrix4x4_ApplyNormalizedAngleRotation(normalized, angle);
}
#endif

#else

// @verified - p
void Matrix4x4_ApplyAngleRotationToVector(const Vector3 &axis, std::uint16_t angle) {
    Vector3 normalized = axis;
    const float magnitude = std::sqrt((normalized.y * normalized.y + normalized.x * normalized.x) + normalized.z * normalized.z);
    if (magnitude > 0.0f) {
        normalized.x = normalized.x * (1.0f / magnitude);
        normalized.y = normalized.y * (1.0f / magnitude);
        normalized.z = normalized.z * (1.0f / magnitude);
    }
    const float cosine = cos_fixed16(angle);
    Matrix4x4_BuildAndApplyRotationMatrix(normalized, sin_fixed16(static_cast<std::int16_t>(angle)), cosine);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Native sin_fp16: tangent of the half angle by continued fraction, then the double-angle form.
// @verified - p
float FixedAngleSin(std::uint16_t angle) {
    const float halfAngle = ((static_cast<float>(angle) * 6.2831855f) * 0.000015258789f) * 0.5f;
    const std::int32_t quadrant = static_cast<std::int32_t>(halfAngle / 1.5707964f + 0.5f);
    const float reduced = halfAngle - static_cast<float>(quadrant) * 1.5707964f;
    float fraction = 0.0f;
    for (std::int32_t divisor = 11; divisor >= 3; divisor -= 2)
        fraction = (reduced * reduced) / (static_cast<float>(divisor) - fraction);
    const float tangent = reduced / (1.0f - fraction);
    const float result = (tangent + tangent) / (tangent * tangent + 1.0f);
    return (quadrant & 1) != 0 ? -result : result;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
float FixedAngleCos(std::uint16_t angle) {
    return FixedAngleSin(static_cast<std::uint16_t>(16384 - (angle <= 32768U ? angle : static_cast<std::uint16_t>(-angle))));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t ComputeNormalizedAngleRatio(float ratio) {
    if (std::fabs(ratio) > 1.0f)
        return ratio > 0.0f ? 0x4000 : -0x4000;
    const float adjacent = std::sqrt(1.0f - ratio * ratio);
    if (adjacent == 0.0f)
        return ratio > 0.0f ? 0x4000 : 0xc000;
    return static_cast<std::int32_t>(std::atan(ratio / adjacent) * 65536.0f / 6.2831855f + 0.5f);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void Matrix4x4_BuildAndApplyRotationMatrix(const Vector3 &axis, float sine, float cosine) {
    const float x = axis.x, y = axis.y, z = axis.z, complement = 1.0f - cosine;
    const float xy = (x * y) * complement, yz = (y * z) * complement, zx = (z * x) * complement;
    const float sx = x * sine, sy = y * sine, sz = z * sine;
    const float xx = x * x, yy = y * y, zz = z * z;
    const Matrix4x4 rotation = {
        SHEN_FIELD(rows){{SHEN_FIELD(x)((1.0f - xx) * (cosine) + (xx)), SHEN_FIELD(y) xy + sz, SHEN_FIELD(z) zx - sy, SHEN_FIELD(w) 0.0f},
            {SHEN_FIELD(x) xy - sz, SHEN_FIELD(y)((1.0f - yy) * (cosine) + (yy)), SHEN_FIELD(z) yz + sx, SHEN_FIELD(w) 0.0f},
            {SHEN_FIELD(x) zx + sy, SHEN_FIELD(y) yz - sx, SHEN_FIELD(z)((1.0f - zz) * (cosine) + (zz)), SHEN_FIELD(w) 0.0f},
            {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 1.0f}}};
    Matrix4x4::Multiply(&rotation);
}
#endif

#else

// @verified - p
void Matrix4x4_BuildAndApplyRotationMatrix(const Vector3 &axis, float sine, float cosine) {
    const float x = axis.x, y = axis.y, z = axis.z;
    Matrix4x4 &rotation = g_motionAxisRotationMatrix;
    rotation.rows[3].y = 0.0f;
    rotation.rows[3].z = 0.0f;
    rotation.rows[2].w = 0.0f;
    rotation.rows[3].x = 0.0f;
    rotation.rows[1].w = 0.0f;
    rotation.rows[0].w = 0.0f;
    rotation.rows[3].w = 1.0f;
    const float xz = (z * x) * (1.0f - cosine), xy = (y * x) * (1.0f - cosine), yz = (z * y) * (1.0f - cosine);
    rotation.rows[0].z = xz - y * sine;
    rotation.rows[2].x = y * sine + xz;
    rotation.rows[0].x = ((1.0f - x * x) * cosine) + x * x;
    rotation.rows[0].y = z * sine + xy;
    rotation.rows[1].x = xy - z * sine;
    rotation.rows[1].y = ((1.0f - y * y) * cosine) + y * y;
    rotation.rows[2].z = ((1.0f - z * z) * cosine) + z * z;
    rotation.rows[1].z = x * sine + yz;
    rotation.rows[2].y = yz - x * sine;
    Matrix4x4::MultiplyWithPrePassMatrix(&rotation);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void NormalizeVector3(Vector3 *vector);
std::int32_t ComputeTriangleNormal(const Vector3 &first, const Vector3 &second, const Vector3 &third, Vector3 &normal);
void Matrix4x4_ApplyNormalizedAngleRotation(const Vector3 &axis, std::int32_t angle);
void Matrix4x4_ApplyAngleRotationToVector(const Vector3 &axis, std::int32_t angle);
void Matrix4x4_BuildAndApplyRotationMatrix(const Vector3 &axis, float sine, float cosine);
void Matrix4x4_BuildRotationMatrix(const Vector3 &from, const Vector3 &to);
void InverseTransformPointByMatrix(const Matrix4x4 *transform, Vector3 *position);
float GetSquaredDistanceXZ(const Vector3 *first, const Vector3 *second);
float GetPlanarDistance(const Vector3 *first, const Vector3 *second);
std::int32_t ComputeInverseAngleRatio(float ratio);
float GetSquaredEuclideanDistance(const Vector3 *first, const Vector3 *second);
float GetEuclideanDistance(const Vector3 *first, const Vector3 *second);
std::int32_t InverseFixedAtan2(float first, float direction);
void TransformCharacterPoseByMatrix(const Matrix4x4 *transform, Vector3 *position, Vector3i *rotation);
#else
std::uint16_t FloatToShort(float value);
void NormalizeVector3(Vector3 *vector);
void NormalizeVector4(Vector4 *vector);
void InvertModelWorkTransform();
std::int32_t ComputeTriangleNormal(const Vector3 &first, const Vector3 &second, const Vector3 &third, Vector3 &normal);
void Matrix4x4_ApplyAngleRotationToVector(const Vector3 &axis, std::uint16_t angle);
float FixedAngleSin(std::uint16_t angle);
float FixedAngleCos(std::uint16_t angle);
std::int32_t ComputeNormalizedAngleRatio(float ratio);
void Matrix4x4_BuildAndApplyRotationMatrix(const Vector3 &axis, float sine, float cosine);
void Matrix4x4_BuildRotationMatrix(const Vector3 &from, const Vector3 &to);
float GetSquaredDistanceXZ(const Vector3 *first, const Vector3 *second);
float GetPlanarDistance(const Vector3 *first, const Vector3 *second);
std::int32_t ComputeInverseAngleRatio(float ratio);
std::int32_t Vector3_ComputeAngleBetween(const Vector3 &first, const Vector3 &second);
float GetSquaredEuclideanDistance(const Vector3 *first, const Vector3 *second);
float GetEuclideanDistance(const Vector3 *first, const Vector3 *second);
#if SHENMUE_S2_GCC295
std::int32_t InverseFixedAtan2(float first, float direction);
#else
std::int16_t InverseFixedAtan2(float first, float direction);
#endif
std::int32_t Vector3_ToFixed16EulerAngles(const Vector3 &direction, Vector3s &angles);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
float FixedAngleTangent(std::int32_t angle) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    Vector2 values;
    CalculateFixedSinCos(static_cast<std::uint32_t>(angle), &values);
    const float sine = values.x, cosine = values.y;
#else
    float sine, cosine;
    asm volatile("lds %2,FPUL\n\tfsca FPUL,dr4\n\tfmov fr4,%0\n\tfmov fr5,%1" : "=f"(sine), "=f"(cosine) : "r"(angle) : "fr4", "fr5", "fpul");
#endif
    return cosine == 0.0f ? 0.0f : sine / cosine;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
float GetEuclideanDistance(const Vector3 *first, const Vector3 *second) {
    return CalculateDistanceBetweenPoints(first, second);
}

#else

// @verified - p
float GetEuclideanDistance(const Vector3 *first, const Vector3 *second) {
    const float x = first->x - second->x;
    const float y = first->y - second->y;
    const float z = first->z - second->z;
    if (!(std::fabs(x) >= 1.1920929e-07f || std::fabs(y) >= 1.1920929e-07f || std::fabs(z) >= 1.1920929e-07f))
        return 0.0f;
    return std::sqrt((y * y + x * x) + z * z);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
float GetSquaredDistanceXZ(const Vector3 *first, const Vector3 *second) {
    const float x = first->x - second->x;
    const float z = first->z - second->z;
    return x * x + z * z;
}

#else

// @verified - p
float GetSquaredDistanceXZ(const Vector3 *first, const Vector3 *second) {
    const float x = first->x - second->x;
    const float z = first->z - second->z;
    return x * x + z * z;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
float GetSquaredEuclideanDistance(const Vector3 *first, const Vector3 *second) {
    return CalculateDistanceSquaredBetweenPoints(first, second);
}

#else

// @verified - p
float GetSquaredEuclideanDistance(const Vector3 *first, const Vector3 *second) {
    const float x = first->x - second->x;
    const float y = first->y - second->y;
    const float z = first->z - second->z;
    return (y * y + x * x) + z * z;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t InverseFixedAtan2(float first, float direction) {
    return static_cast<std::int16_t>(atan2_fixed16(-first, -direction));
}

#else

// @verified - p
#if SHENMUE_S2_GCC295
std::int32_t InverseFixedAtan2(float first, float direction) {
#else
std::int16_t InverseFixedAtan2(float first, float direction) {
#endif
    return std::bit_cast<std::int16_t>(FixedAtan2(-first, -direction));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int32_t Vector3_ToFixed16EulerAngles(const Vector3 &direction, Vector3s &angles) {
    constexpr float epsilon = 1.1920929e-07f;
    if (std::fabs(direction.x) < epsilon && std::fabs(direction.y) < epsilon && std::fabs(direction.z) < epsilon)
        return -1;
    angles.x = std::bit_cast<std::int16_t>(FixedAtan2(direction.y, std::sqrt(direction.x * direction.x + direction.z * direction.z)));
    angles.y = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(FixedAtan2(direction.x, direction.z) + 0x8000));
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ExtractMatrixEulerAnglesFixed(const Matrix4x4 *transform, Vector3i *rotation) {
    const float sine = -transform->rows[0].z;
    if (sine < 1.0f) {
        if (sine > -1.0f) {
            float cosine = 1.0f - sine * sine;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            cosine = __builtin_sqrtf(cosine);
#else
            asm volatile("fsqrt %0" : "+f"(cosine));
#endif
            rotation->x = dc::sdk::ArcTan2Fixed(transform->rows[1].z, transform->rows[2].z);
            rotation->y = dc::sdk::ArcTan2Fixed(sine, cosine);
            rotation->z = dc::sdk::ArcTan2Fixed(transform->rows[0].y, transform->rows[0].x);
        } else {
            rotation->x = dc::sdk::ArcTan2Fixed(-transform->rows[1].x, transform->rows[1].y);
            rotation->y = 0xc000;
            rotation->z = 0;
        }
    } else {
        rotation->x = dc::sdk::ArcTan2Fixed(transform->rows[1].x, transform->rows[1].y);
        rotation->y = 0x4000;
        rotation->z = 0;
    }
    if (rotation->x >= 0x8000 && rotation->z >= 0x8000) {
        rotation->x -= 0x8000;
        rotation->z -= 0x8000;
        rotation->y = 0x8000 - rotation->y;
    }
}

#else

// @verified - p
void ExtractMatrixEulerAnglesFixed(const Matrix4x4 *transform, Vector3i *rotation) {
    const float sine = -transform->rows[0].z;
    if (sine - 1.0f < -7.629510946571827e-06f) {
        if (sine + 1.0f > 7.629510946571827e-06f) {
            const float cosine = std::sqrt(1.0f - sine * sine);
            rotation->x = FixedAtan2(transform->rows[1].z, transform->rows[2].z);
            rotation->y = FixedAtan2(sine, cosine);
            rotation->z = FixedAtan2(transform->rows[0].y, transform->rows[0].x);
        } else {
            rotation->x = FixedAtan2(-transform->rows[1].x, transform->rows[1].y);
            rotation->y = 49152;
            rotation->z = 0;
        }
    } else {
        rotation->x = FixedAtan2(transform->rows[1].x, transform->rows[1].y);
        rotation->y = 16384;
        rotation->z = 0;
    }
    if (static_cast<std::uint32_t>(rotation->x) >= 32768U && static_cast<std::uint32_t>(rotation->z) >= 32768U) {
        rotation->x -= 32768;
        rotation->z -= 32768;
        rotation->y = 32768 - rotation->y;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InverseTransformPointByMatrix(const Matrix4x4 *transform, Vector3 *position) {
    const Matrix4x4 matrix = *transform;
    const Vector4 delta = {SHEN_FIELD(x) - matrix.rows[3].x + position->x, SHEN_FIELD(y) - matrix.rows[3].y + position->y,
        SHEN_FIELD(z) - matrix.rows[3].z + position->z, SHEN_FIELD(w) 0.0f};
    position->z = matrix_stack::InnerProduct4(delta, matrix.rows[2]);
    position->y = matrix_stack::InnerProduct4(delta, matrix.rows[1]);
    position->x = matrix_stack::InnerProduct4(delta, matrix.rows[0]);
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void TransformCharacterPoseByMatrix(const Matrix4x4 *transform, Vector3 *position, Vector3i *rotation) {
    Matrix4x4 saved;
    matrix_stack::GetMatrix(&saved);
    matrix_stack::LoadMatrix(transform);
    Vector4 transformed;
    matrix_stack::TransformPosition(position, &transformed);
    Matrix4x4 composed = *transform;
    composed.rows[3] = transformed;
    matrix_stack::LoadMatrix(&composed);
    Matrix4x4::SetViewMatrixFromEuler(rotation->x, rotation->y, rotation->z);
    matrix_stack::GetMatrix(&composed);
    matrix_stack::LoadMatrix(&saved);
    position->x = composed.rows[3].x;
    position->y = composed.rows[3].y;
    position->z = composed.rows[3].z;
    ExtractMatrixEulerAnglesFixed(&composed, rotation);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
float GetPlanarDistance(const Vector3 *first, const Vector3 *second) {
    const float x = first->x - second->x, z = first->z - second->z;
    if (__builtin_fabsf(x) < 0.00000011920929f && __builtin_fabsf(z) < 0.00000011920929f)
        return 0.0f;
    return __builtin_sqrtf(x * x + z * z);
}

#else

// @verified - p
float GetPlanarDistance(const Vector3 *first, const Vector3 *second) {
    const float x = first->x - second->x;
    const float z = first->z - second->z;
    if (!(std::fabs(x) >= 1.1920929e-07f || std::fabs(z) >= 1.1920929e-07f))
        return 0.0f;
    return std::sqrt(z * z + x * x);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ComputeInverseAngleRatio(float ratio) {
    if (__builtin_fabsf(ratio) > 1.0f)
        return ratio > 0.0f ? 0 : 0x8000;
    return (0x4000U - dc::sdk::ArcSinFixed(ratio)) & 0xffffU;
}

#else

std::int32_t ComputeInverseAngleRatio(float ratio) {
    if (std::fabs(ratio) <= 1.0f)
        return 16384 - ComputeNormalizedAngleRatio(ratio);
    return ratio > 0.0f ? 0 : 0x8000;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t Vector3_ComputeAngleBetween(const Vector3 &first, const Vector3 &second) {
    const float dot = (second.x * first.x + second.y * first.y) + second.z * first.z;
    const float lengths =
        std::sqrt((second.x * second.x + second.y * second.y) + second.z * second.z) * std::sqrt((first.x * first.x + first.y * first.y) + first.z * first.z);
    return std::fabs(lengths) >= 0.00000011920929f ? ComputeInverseAngleRatio(dot / lengths) : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void Matrix4x4_BuildRotationMatrix(const Vector3 &from, const Vector3 &to) {
    Vector3 axis;
    dc::sdk::njOuterProduct(&from, &to, &axis);
    const float sine = dc::sdk::njUnitVector(&axis);
    const float cosine = dc::sdk::njInnerProduct(&from, &to);
    if (sine == 0.0f) {
        if (!(cosine < 0.0f))
            return;
        axis.x = from.y;
        axis.y = from.z;
        axis.z = from.x;
        Matrix4x4_BuildAndApplyRotationMatrix(axis, 0.0f, -1.0f);
    } else {
        Matrix4x4_BuildAndApplyRotationMatrix(axis, sine, cosine);
    }
}
#endif

#else

// Parallel vectors leave the matrix alone; opposite ones turn half way about the axis cycled out of `from`.
// @verified - p
void Matrix4x4_BuildRotationMatrix(const Vector3 &from, const Vector3 &to) {
    Vector3 axis{to.z * from.y - to.y * from.z, to.x * from.z - from.x * to.z, from.x * to.y - to.x * from.y};
    float sine = std::sqrt((axis.y * axis.y + axis.x * axis.x) + axis.z * axis.z);
    if (sine > 0.0f)
        axis = {axis.x * (1.0f / sine), axis.y * (1.0f / sine), axis.z * (1.0f / sine)};
    float cosine = (from.x * to.x + to.y * from.y) + from.z * to.z;
    if (sine == 0.0f) {
        if (cosine >= 0.0f)
            return;
        cosine = -1.0f;
        axis = {from.y, from.z, from.x};
    }
    Matrix4x4_BuildAndApplyRotationMatrix(axis, sine, cosine);
}

#endif

} // namespace s2
} // namespace shenmue
