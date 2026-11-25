#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {

// @verified - d
void Matrix4x4::MultiplyWithPrePassMatrix(const Matrix4x4 *matrix) {
    Matrix4x4 product;
    for (std::uint32_t row = 0; row < 4; ++row)
        matrix_stack::TransformVector4(&matrix->rows[row], &product.rows[row]);
    matrix_stack::LoadMatrix(&product);
}

} // namespace shenmue

namespace shenmue {
namespace s2 {

// @verified - d
void SetCurrentMatrixTranslation(const Vector3 *translation) {
    Matrix4x4 current;
    matrix_stack::GetMatrix(&current);
    current.rows[3].x = translation->x;
    current.rows[3].y = translation->y;
    current.rows[3].z = translation->z;
    matrix_stack::LoadMatrix(&current);
}

// @verified - d
void AddCurrentMatrix(const Matrix4x4 *matrix) {
    Matrix4x4 current;
    matrix_stack::GetMatrix(&current);
    for (std::uint32_t row = 0; row < 4; ++row) {
        for (std::uint32_t column = 0; column < 4; ++column)
            current.rows[row][column] += matrix->rows[row][column];
    }
    matrix_stack::LoadMatrix(&current);
}

// @verified - d
void SubtractCurrentMatrix(const Matrix4x4 *matrix) {
    Matrix4x4 current;
    matrix_stack::GetMatrix(&current);
    for (std::uint32_t row = 0; row < 4; ++row) {
        for (std::uint32_t column = 0; column < 4; ++column)
            current.rows[row][column] -= matrix->rows[row][column];
    }
    matrix_stack::LoadMatrix(&current);
}

// @verified - d
void MultiplyCurrentMatrixFromRight(const Matrix4x4 *matrix) {
    const Matrix4x4 right = *matrix;
    Matrix4x4 current;
    matrix_stack::GetMatrix(&current);
    matrix_stack::LoadMatrix(&right);
    Matrix4x4 product;
    for (std::uint32_t row = 0; row < 4; ++row)
        matrix_stack::TransformVector4(&current.rows[row], &product.rows[row]);
    matrix_stack::LoadMatrix(&product);
}

// @verified - d
float FixedAngleSin(std::uint16_t angle) {
    float reduced = static_cast<float>(angle) * 6.283185482025146484375f;
    reduced *= 0.0000152587890625f;
    reduced *= 0.5f;
    const std::int32_t quadrant = static_cast<std::int32_t>(reduced / 1.57079637050628662109375f + 0.5f);
    reduced -= static_cast<float>(quadrant) * 1.57079637050628662109375f;
    const float squared = reduced * reduced;
    float fraction = 0.0f;
    for (std::int32_t divisor = 11; divisor > 2; divisor -= 2)
        fraction = squared / (static_cast<float>(divisor) - fraction);
    const float tangent = reduced / (1.0f - fraction);
    const float doubled = tangent + tangent;
    float denominator = 1.0f;
    asm volatile("fmov %1,fr0\n\tfmac fr0,%1,%0" : "+f"(denominator) : "f"(tangent) : "fr0");
    const float result = doubled / denominator;
    return (quadrant & 1) ? -result : result;
}

// @verified - d
float FixedAngleCos(std::uint16_t angle) {
    const std::uint32_t folded = angle <= 0x8000U ? angle : 0x10000U - angle;
    return FixedAngleSin(static_cast<std::uint16_t>(0x4000U - folded));
}

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void Matrix4x4_BuildAndApplyRotationMatrix(const Vector3 &axis, float sine, float cosine) {
    const float x = axis.x, y = axis.y, z = axis.z;
    const float xy = (x * y) * (1.0f - cosine), yz = (y * z) * (1.0f - cosine), zx = (z * x) * (1.0f - cosine);
    const float xs = x * sine, ys = y * sine, zs = z * sine;
    float xx = x * x, yy = y * y, zz = z * z;
    const float inverseX = 1.0f - xx, inverseY = 1.0f - yy, inverseZ = 1.0f - zz;
    asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+f"(xx) : "f"(inverseX), "f"(cosine) : "fr0");
    asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+f"(yy) : "f"(inverseY), "f"(cosine) : "fr0");
    asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+f"(zz) : "f"(inverseZ), "f"(cosine) : "fr0");
    const Matrix4x4 rotation = {{
        {SHEN_FIELD(x) xx, SHEN_FIELD(y) xy + zs, SHEN_FIELD(z) zx - ys, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) xy - zs, SHEN_FIELD(y) yy, SHEN_FIELD(z) yz + xs, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) zx + ys, SHEN_FIELD(y) yz - xs, SHEN_FIELD(z) zz, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 1.0f},
    }};
    Matrix4x4::MultiplyWithPrePassMatrix(&rotation);
}
#endif

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void Matrix4x4_ApplyAngleRotationToVector(const Vector3 &axis, std::uint16_t angle) {
    Vector3 normalized = axis;
    Vector3::NormalizeInPlace(normalized);
    const float sine = FixedAngleSin(angle);
    const float cosine = FixedAngleCos(angle);
    Matrix4x4_BuildAndApplyRotationMatrix(normalized, sine, cosine);
}
#endif

// @verified - d
bool PopCurrentMatrix(std::int32_t count) {
    if (count <= 0)
        count = 1;
    const bool available = matrix_stack::g_matrixStack.top.info.index >= count;
    Matrix4x4::Pop(count);
    return available;
}

} } // namespace shenmue::s2

extern "C" {

void __attribute__((renesas)) shenmue_s2_event_Matrix4x4_ScaleXYZ(float x, float y, float z) {
    shenmue::Matrix4x4::Scale(x, y, z);
}
}

#endif
