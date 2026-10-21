#include "common/matrix_stack.hpp"

#include "common/math.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace shenmue::matrix_stack {

Matrix4x4 workTransformMatrix{};
Matrix4x4 transformMatrix_prePass{{
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f},
}};
MatrixStackTable g_matrixStack{};
alignas(32) TemporaryMatrixStacks g_temporaryMatrixStacks{};
std::uint16_t g_CurrentMatrixStackIdx = 0;

void MatrixStack_InitStackTableEntries(MatrixStackEntry *entry, Matrix4x4 *memory, std::int32_t capacity) {
    const auto address = reinterpret_cast<std::uintptr_t>(memory);
    auto *const alignedMemory = reinterpret_cast<Matrix4x4 *>((address + 0x1f) & ~std::uintptr_t{0x1f});
    if (memory != alignedMemory) {
        --capacity;
    }
    entry->stack = alignedMemory;
    entry->base = alignedMemory;
    if (capacity > 0x7fff) {
        capacity = 0x7fff;
    }
    entry->info.index = 0;
    entry->info.capacity = static_cast<std::int16_t>(capacity);
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4 *stack = entry->stack ? entry->stack : g_matrixStack.top.stack;
    *stack = workTransformMatrix;
}

void MatrixStack_InitTransformMatrixStack(Matrix4x4 *memory, std::int32_t size) {
    std::memset(&g_matrixStack, 0, sizeof(g_matrixStack));
    std::memset(&g_temporaryMatrixStacks, 0, sizeof(g_temporaryMatrixStacks));
    workTransformMatrix = {};
    MatrixStack_InitStackTableEntries(&g_matrixStack.entries[1], g_temporaryMatrixStacks.depthPrepass, 3);
    MatrixStack_InitStackTableEntries(&g_matrixStack.entries[2], g_temporaryMatrixStacks.opaque, 3);
    MatrixStack_InitStackTableEntries(&g_matrixStack.entries[0], memory, size + 1);
    MatrixStack_InitStackTableEntries(&g_matrixStack.entries[3], g_temporaryMatrixStacks.transparent, 3);
    g_matrixStack.top = g_matrixStack.entries[0];
    g_CurrentMatrixStackIdx = static_cast<std::uint16_t>(MatrixStackType::DepthPrepass);
}

void SetActiveStack(MatrixStackType type) {
    *g_matrixStack.top.stack = workTransformMatrix;
    const std::uint16_t previous = g_CurrentMatrixStackIdx;
    g_CurrentMatrixStackIdx = static_cast<std::uint16_t>(type);
    g_matrixStack.entries[previous - 1] = g_matrixStack.top;
    g_matrixStack.top = g_matrixStack.entries[g_CurrentMatrixStackIdx - 1];
    workTransformMatrix = *g_matrixStack.top.stack;
}

void SetupStack() {
    *g_matrixStack.top.stack = workTransformMatrix;
    g_matrixStack.top.stack = g_matrixStack.top.base;
    g_matrixStack.top.info.index = 0;
    workTransformMatrix = *g_matrixStack.top.base;
}

namespace {

float decomposeMatrix(Matrix4x4 &matrix, std::array<std::uint8_t, 4> &rowIndices) {
    std::array<float, 4> inverseScales{};
    for (std::size_t row = 0; row < 4; ++row) {
        rowIndices[row] = static_cast<std::uint8_t>(row);
        const float *const values = &matrix.rows[row].x;
        float largest = 0.0f;
        for (std::size_t column = 0; column < 4; ++column) {
            largest = std::fmax(std::fabs(values[column]), largest);
        }
        if (largest == 0.0f) {
            return 0.0f;
        }
        inverseScales[row] = 1.0f / largest;
    }

    float determinant = 1.0f;
    for (std::size_t column = 0; column < 4; ++column) {
        float largest = -1.0f;
        std::size_t pivotIndex = column;
        for (std::size_t candidate = column; candidate < 4; ++candidate) {
            const std::size_t row = rowIndices[candidate];
            const float scaled = std::fabs((&matrix.rows[row].x)[column]) * inverseScales[row];
            if (scaled > largest) {
                largest = scaled;
                pivotIndex = candidate;
            }
        }
        const std::uint8_t pivotRow = rowIndices[pivotIndex];
        if (pivotIndex != column) {
            determinant = -determinant;
            rowIndices[pivotIndex] = rowIndices[column];
            rowIndices[column] = pivotRow;
        }

        float *const pivot = &matrix.rows[pivotRow].x;
        const float pivotValue = pivot[column];
        determinant *= pivotValue;
        if (pivotValue == 0.0f) {
            return 0.0f;
        }
        for (std::size_t remaining = column + 1; remaining < 4; ++remaining) {
            float *const row = &matrix.rows[rowIndices[remaining]].x;
            const float factor = row[column] / pivotValue;
            row[column] = factor;
            for (std::size_t next = column + 1; next < 4; ++next) {
                row[next] -= factor * pivot[next];
            }
        }
    }
    return determinant;
}

} // namespace

void CalcCameraTransform(const Vector4 &eye, const Vector4 &target, std::int16_t zAngle) {
    if (zAngle != 0) {
        const std::uint16_t angle = static_cast<std::uint16_t>(zAngle);
        std::int16_t cosineAngleValue = static_cast<std::int16_t>(-zAngle);
        if (angle <= 0x8000U)
            cosineAngleValue = zAngle;
        Matrix4x4::RotateAroundZ(sin_fixed16(zAngle), sin_fixed16(static_cast<std::int16_t>(0x4000 - cosineAngleValue)));
    }

    const float dx = target.x - eye.x;
    const float dy = target.y - eye.y;
    const float dz = target.z - eye.z;
    const float horizontalSquared = dz * dz + dx * dx;
    const float inverseHorizontal = horizontalSquared == 0.0f ? 3.402823466e38f : 1.0f / std::sqrt(horizontalSquared);
    const float lengthSquared = dy * dy + horizontalSquared;
    const float inverseLength = lengthSquared == 0.0f ? 3.402823466e38f : 1.0f / std::sqrt(lengthSquared);

    Matrix4x4::RotateAroundX(-inverseLength * dy, inverseLength / inverseHorizontal);
    Matrix4x4::RotateAroundY(inverseHorizontal * dx, -inverseHorizontal * dz);
    Matrix4x4::preTranslate(-eye.x, -eye.y, -eye.z);
}

bool Matrix4x4_Push(const Matrix4x4 *transform) {
    *g_matrixStack.top.stack = workTransformMatrix;
    if (transform)
        workTransformMatrix = *transform;
    ++g_matrixStack.top.info.index;
    if (g_matrixStack.top.info.index >= g_matrixStack.top.info.capacity)
        return false;
    ++g_matrixStack.top.stack;
    return true;
}

Vector3 *ComputeEulerAnglesFromVector(const Matrix4x4 *matrix, Vector3 *output) {
    Vector4 norms;
    Matrix4x4::ComputeColumnNorms(matrix, &norms);
    const float yz = matrix->rows[2].y / norms.y;
    const float zz = matrix->rows[2].z / norms.z;
    output->x = std::atan2(yz, zz);
    output->y = std::atan2(-(matrix->rows[2].x / norms.x), std::sqrt(zz * zz + yz * yz));
    output->z = std::atan2(matrix->rows[1].x / norms.x, matrix->rows[0].x / norms.x);
    return output;
}

Vector4 *PerformPerspectiveProjectionTransform(const Matrix4x4 *matrix, Vector4 *output, const Vector3 *input) {
    for (std::size_t row = 0; row < 4; ++row) {
        const Vector4 &r = matrix->rows[row];
        (*output)[row] = r.x * input->x + r.y * input->y + r.z * input->z + r.w;
    }
    const float reciprocalW = 1.0f / output->w;
    output->x *= reciprocalW;
    output->y *= reciprocalW;
    output->z *= reciprocalW;
    output->w *= reciprocalW;
    return output;
}

Matrix4x4 *createTransformedMatrix(const Matrix4x4 *source, Matrix4x4 *output) {
    const float tx = source->rows[0].w;
    const float ty = source->rows[1].w;
    const float tz = source->rows[2].w;
    output->rows[0] = {source->rows[0].x, source->rows[1].x, source->rows[2].x, -(source->rows[1].x * ty + source->rows[0].x * tx + source->rows[2].x * tz)};
    output->rows[1] = {source->rows[0].y, source->rows[1].y, source->rows[2].y, -(source->rows[1].y * ty + source->rows[0].y * tx + source->rows[2].y * tz)};
    output->rows[2] = {source->rows[0].z, source->rows[1].z, source->rows[2].z, -(source->rows[1].z * ty + source->rows[0].z * tx + source->rows[2].z * tz)};
    output->rows[3] = {0.0f, 0.0f, 0.0f, 1.0f};
    return output;
}

namespace {

Matrix4x4 multiplyMatrices(const Matrix4x4 &left, const Matrix4x4 &right) {
    Matrix4x4 output{};
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            output.rows[row][column] = left.rows[row].x * right.rows[0][column] + left.rows[row].y * right.rows[1][column] +
                left.rows[row].z * right.rows[2][column] + left.rows[row].w * right.rows[3][column];
        }
    }
    return output;
}


Vector3 normalized(const Vector3 &value) {
    const float length = std::sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
    if (length <= 0.0f)
        return value;
    const float inverseLength = 1.0f / length;
    return {value.x * inverseLength, value.y * inverseLength, value.z * inverseLength};
}

} // namespace


Matrix4x4 *RotateMatrixByAngles(Matrix4x4 *output, float x, float y, float z) {
    const float sx = std::sin(x), cx = std::cos(x);
    const float sy = std::sin(y), cy = std::cos(y);
    const float sz = std::sin(z), cz = std::cos(z);
    const Matrix4x4 rx{{{1, 0, 0, 0}, {0, cx, -sx, 0}, {0, sx, cx, 0}, {0, 0, 0, 1}}};
    const Matrix4x4 ry{{{cy, 0, sy, 0}, {0, 1, 0, 0}, {-sy, 0, cy, 0}, {0, 0, 0, 1}}};
    const Matrix4x4 rz{{{cz, -sz, 0, 0}, {sz, cz, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
    *output = multiplyMatrices(rz, multiplyMatrices(ry, rx));
    return output;
}

Matrix4x4 *ComputeRotationMatrixFromVectorAndAngle(Matrix4x4 *output, const Vector3 *axis, float angle) {
    const Vector3 n = normalized(*axis);
    const float sine = std::sin(angle);
    const float cosine = std::cos(angle);
    const float oneMinusCosine = 1.0f - cosine;
    output->rows[0] = {n.x * n.x * oneMinusCosine + cosine, n.x * n.y * oneMinusCosine - n.z * sine, n.x * n.z * oneMinusCosine + n.y * sine, 0.0f};
    output->rows[1] = {n.x * n.y * oneMinusCosine + n.z * sine, n.y * n.y * oneMinusCosine + cosine, n.y * n.z * oneMinusCosine - n.x * sine, 0.0f};
    output->rows[2] = {n.x * n.z * oneMinusCosine - n.y * sine, n.y * n.z * oneMinusCosine + n.x * sine, n.z * n.z * oneMinusCosine + cosine, 0.0f};
    output->rows[3] = {0.0f, 0.0f, 0.0f, 1.0f};
    return output;
}

Matrix4x4 *ComputeRotationMatrixFromVectors(Matrix4x4 *output, const Vector3 *from, const Vector3 *to) {
    const Vector3 a = normalized(*from);
    const Vector3 b = normalized(*to);
    const float dot = a.x * b.x + a.y * b.y + a.z * b.z;
    if (dot > 0.99999988f) {
        *output = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
        return output;
    }
    const Vector3 axis = dot < -0.99999988f ? Vector3{a.y, a.x, a.z} : Vector3{a.y * b.z - a.z * b.y, b.x * a.z - a.x * b.z, a.x * b.y - b.x * a.y};
    return ComputeRotationMatrixFromVectorAndAngle(output, &axis, std::acos(dot));
}

Matrix4x4 *ComputeLookAtViewMatrix(Matrix4x4 *output, const Vector3 *eye, const Vector3 *target, const Vector3 *up) {
    Vector3 forward{target->x - eye->x, target->y - eye->y, target->z - eye->z};
    float length = std::sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
    if (length > 0.0f) {
        forward = {forward.x / length, forward.y / length, forward.z / length};
    }
    Vector3 side{up->y * forward.z - up->z * forward.y, up->z * forward.x - up->x * forward.z, up->x * forward.y - up->y * forward.x};
    length = std::sqrt(side.x * side.x + side.y * side.y + side.z * side.z);
    if (length > 0.0f) {
        side = {side.x / length, side.y / length, side.z / length};
    }
    const Vector3 newUp{forward.y * side.z - forward.z * side.y, forward.z * side.x - forward.x * side.z, forward.x * side.y - forward.y * side.x};
    output->rows[0] = {side.x, side.y, side.z, -(side.x * eye->x + side.y * eye->y + side.z * eye->z)};
    output->rows[1] = {newUp.x, newUp.y, newUp.z, -(newUp.x * eye->x + newUp.y * eye->y + newUp.z * eye->z)};
    output->rows[2] = {forward.x, forward.y, forward.z, -(forward.x * eye->x + forward.y * eye->y + forward.z * eye->z)};
    output->rows[3] = {0.0f, 0.0f, 0.0f, 1.0f};
    return output;
}

// @verified - p
Vector4 *RotationMatrixToQuaternion(const Matrix4x4 *matrix, Vector4 *output) {
    const float trace = matrix->rows[0].x + matrix->rows[1].y + matrix->rows[2].z;
    if (trace > 0.0f) {
        const float reciprocal = 1.0f / (std::sqrt(trace + 1.0f) * 2.0f);
        output->w = 0.25f / reciprocal;
        output->x = (matrix->rows[2].y - matrix->rows[1].z) * reciprocal;
        output->y = (matrix->rows[0].z - matrix->rows[2].x) * reciprocal;
        output->z = (matrix->rows[1].x - matrix->rows[0].y) * reciprocal;
    } else if (matrix->rows[0].x > matrix->rows[1].y && matrix->rows[0].x > matrix->rows[2].z) {
        const float reciprocal = 1.0f / (std::sqrt(matrix->rows[0].x + 1.0f - matrix->rows[1].y - matrix->rows[2].z) * 2.0f);
        output->w = (matrix->rows[2].y - matrix->rows[1].z) * reciprocal;
        output->x = 0.25f / reciprocal;
        output->y = (matrix->rows[1].x + matrix->rows[0].y) * reciprocal;
        output->z = (matrix->rows[2].x + matrix->rows[0].z) * reciprocal;
    } else if (matrix->rows[1].y > matrix->rows[2].z) {
        const float reciprocal = 1.0f / (std::sqrt(matrix->rows[1].y + 1.0f - matrix->rows[0].x - matrix->rows[2].z) * 2.0f);
        output->w = (matrix->rows[0].z - matrix->rows[2].x) * reciprocal;
        output->x = (matrix->rows[1].x + matrix->rows[0].y) * reciprocal;
        output->y = 0.25f / reciprocal;
        output->z = (matrix->rows[2].y + matrix->rows[1].z) * reciprocal;
    } else {
        const float reciprocal = 1.0f / (std::sqrt(matrix->rows[2].z + 1.0f - matrix->rows[0].x - matrix->rows[1].y) * 2.0f);
        output->w = (matrix->rows[1].x - matrix->rows[0].y) * reciprocal;
        output->x = (matrix->rows[2].x + matrix->rows[0].z) * reciprocal;
        output->y = (matrix->rows[2].y + matrix->rows[1].z) * reciprocal;
        output->z = 0.25f / reciprocal;
    }
    return output;
}

} // namespace shenmue::matrix_stack

namespace shenmue {

using matrix_stack::workTransformMatrix;
using matrix_stack::transformMatrix_prePass;
using matrix_stack::g_matrixStack;
using matrix_stack::decomposeMatrix;

void Matrix4x4::preTranslate(float dx, float dy, float dz) {
    workTransformMatrix.rows[3].y += workTransformMatrix.rows[0].y * dx + workTransformMatrix.rows[1].y * dy + workTransformMatrix.rows[2].y * dz;
    workTransformMatrix.rows[3].x += workTransformMatrix.rows[1].x * dy + workTransformMatrix.rows[0].x * dx + workTransformMatrix.rows[2].x * dz;
    workTransformMatrix.rows[3].z += workTransformMatrix.rows[0].z * dx + workTransformMatrix.rows[1].z * dy + workTransformMatrix.rows[2].z * dz;
    workTransformMatrix.rows[3].w += workTransformMatrix.rows[1].w * dy + workTransformMatrix.rows[0].w * dx + workTransformMatrix.rows[2].w * dz;
}

void Matrix4x4::RotateAroundX(float sine, float cosine) {
    Vector4 row1{};
    Vector4 row2{};
    float *const out1 = &row1.x;
    float *const out2 = &row2.x;
    const float *const old1 = &workTransformMatrix.rows[1].x;
    const float *const old2 = &workTransformMatrix.rows[2].x;
    for (std::size_t component = 0; component < 4; ++component) {
        out1[component] = old2[component] * sine + old1[component] * cosine;
        out2[component] = old2[component] * cosine - old1[component] * sine;
    }
    workTransformMatrix.rows[1] = row1;
    workTransformMatrix.rows[2] = row2;
}

void Matrix4x4::RotateAroundY(float sine, float cosine) {
    Vector4 row0{};
    Vector4 row2{};
    float *const out0 = &row0.x;
    float *const out2 = &row2.x;
    const float *const old0 = &workTransformMatrix.rows[0].x;
    const float *const old2 = &workTransformMatrix.rows[2].x;
    for (std::size_t component = 0; component < 4; ++component) {
        out0[component] = old0[component] * cosine - old2[component] * sine;
        out2[component] = old0[component] * sine + old2[component] * cosine;
    }
    workTransformMatrix.rows[0] = row0;
    workTransformMatrix.rows[2] = row2;
}

void Matrix4x4::RotateAroundZ(float sine, float cosine) {
    Vector4 row0{};
    Vector4 row1{};
    float *const out0 = &row0.x;
    float *const out1 = &row1.x;
    const float *const old0 = &workTransformMatrix.rows[0].x;
    const float *const old1 = &workTransformMatrix.rows[1].x;
    for (std::size_t component = 0; component < 4; ++component) {
        out0[component] = old1[component] * sine + old0[component] * cosine;
        out1[component] = old1[component] * cosine - old0[component] * sine;
    }
    workTransformMatrix.rows[0] = row0;
    workTransformMatrix.rows[1] = row1;
}

void Matrix4x4::RotateAroundX_Wrapper(std::uint16_t angle) {
    Matrix4x4::RotateAroundX(sin_fixed16(static_cast<std::int16_t>(angle)), cos_fixed16(angle));
}

void Matrix4x4::RotateAroundY_Wrapper(std::uint16_t angle) {
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(angle)), cos_fixed16(angle));
}

void Matrix4x4::RotateAroundZ_Wrapper(std::uint16_t angle) {
    Matrix4x4::RotateAroundZ(sin_fixed16(static_cast<std::int16_t>(angle)), cos_fixed16(angle));
}

void Matrix4x4::SetViewMatrixFromEuler(std::int32_t rotationY, std::int32_t rotationX, std::int32_t rotationZ) {
    const auto y = static_cast<std::int16_t>(rotationY);
    const auto x = static_cast<std::int16_t>(rotationX);
    const auto z = static_cast<std::int16_t>(rotationZ);
    if (x == 0 && y == 0 && z == 0)
        return;

    const float sy = sin_fixed16(y);
    const float cy = cos_fixed16(static_cast<std::uint16_t>(y));
    if (x == 0 && z == 0) {
        const Matrix4x4 source = workTransformMatrix;
        for (std::size_t column = 0; column < 4; ++column) {
            const float *const row0 = &source.rows[0].x;
            const float *const row2 = &source.rows[2].x;
            (&workTransformMatrix.rows[0].x)[column] = row0[column] * cy - row2[column] * sy;
            (&workTransformMatrix.rows[2].x)[column] = row0[column] * sy + row2[column] * cy;
        }
        return;
    }

    const float sx = sin_fixed16(x);
    const float cx = cos_fixed16(static_cast<std::uint16_t>(x));
    const float sz = sin_fixed16(z);
    const float cz = cos_fixed16(static_cast<std::uint16_t>(z));
    const Matrix4x4 source = workTransformMatrix;
    const std::array<std::array<float, 4>, 4> rotation{{
        {{cz * cy, cy * sz, -sy, 0.0f}},
        {{sy * sx * cz - cx * sz, sy * sx * sz + cz * cx, cy * sx, 0.0f}},
        {{cx * sy * cz + sz * sx, cx * sy * sz - cz * sx, cy * cx, 0.0f}},
        {{0.0f, 0.0f, 0.0f, 1.0f}},
    }};
    Matrix4x4 output{};
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            (&output.rows[row].x)[column] = rotation[row][0] * (&source.rows[0].x)[column] + rotation[row][1] * (&source.rows[1].x)[column] +
                rotation[row][2] * (&source.rows[2].x)[column] + rotation[row][3] * (&source.rows[3].x)[column];
        }
    }
    workTransformMatrix = output;
}

void Matrix4x4::RotateZY(const Vector3 &direction) {
    const float xy = std::hypot(direction.x, direction.y);
    const float xyz = std::hypot(xy, direction.z);
    if (xy < 0x1p-23f || xyz < 0x1p-23f)
        return;
    Matrix4x4::RotateAroundZ(direction.y / xy, direction.x / xy);
    Matrix4x4::RotateAroundY(-(direction.z / xyz), xy / xyz);
}

void Matrix4x4::Scale(float x, float y, float z) {
    workTransformMatrix.rows[0].x *= x;
    workTransformMatrix.rows[0].y *= x;
    workTransformMatrix.rows[0].z *= x;
    workTransformMatrix.rows[0].w *= x;
    workTransformMatrix.rows[1].x *= y;
    workTransformMatrix.rows[1].y *= y;
    workTransformMatrix.rows[1].z *= y;
    workTransformMatrix.rows[1].w *= y;
    workTransformMatrix.rows[2].x *= z;
    workTransformMatrix.rows[2].y *= z;
    workTransformMatrix.rows[2].z *= z;
    workTransformMatrix.rows[2].w *= z;
}

void Matrix4x4::NormalizeAndSetTransformMatrix() {



    Matrix4x4 normalized = workTransformMatrix;
    for (std::size_t row = 0; row < 3; ++row) {
        Vector4 &axis = normalized.rows[row];
        const float length = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
        if (length != 0.0f) {
            axis.x /= length;
            axis.y /= length;
            axis.z /= length;
        }
        axis.w = 0.0f;
    }

    const Vector4 translation = normalized.rows[3];
    const Vector3 inverseTranslation{
        -(normalized.rows[0].x * translation.x + normalized.rows[0].y * translation.y + normalized.rows[0].z * translation.z),
        -(normalized.rows[1].x * translation.x + normalized.rows[1].y * translation.y + normalized.rows[1].z * translation.z),
        -(normalized.rows[2].x * translation.x + normalized.rows[2].y * translation.y + normalized.rows[2].z * translation.z),
    };
    workTransformMatrix = normalized;
    Matrix4x4::RotateMatrixCounterClockwise90();
    workTransformMatrix.rows[3] = {inverseTranslation.x, inverseTranslation.y, inverseTranslation.z, 1.0f};
}

Matrix4x4 *Matrix4x4::GetTransformMatrix(Matrix4x4 *output) {
    if (!output)
        output = g_matrixStack.top.stack;
    *output = workTransformMatrix;
    return output;
}

void Matrix4x4::SetActiveTransform(const Matrix4x4 *transform) {
    if (!transform)
        transform = g_matrixStack.top.stack;
    workTransformMatrix = *transform;
}

void Matrix4x4::SetPrePassToTempTransform() {
    workTransformMatrix = transformMatrix_prePass;
}

void Matrix4x4::Pop(std::int32_t count) {
    if (count <= 0)
        count = 1;
    if (count > g_matrixStack.top.info.index) {
        count = g_matrixStack.top.info.index;
    }
    std::int32_t stackCount = count;
    if (g_matrixStack.top.info.index > g_matrixStack.top.info.capacity) {
        stackCount += g_matrixStack.top.info.capacity - g_matrixStack.top.info.index;
    }
    g_matrixStack.top.info.index = static_cast<std::int16_t>(g_matrixStack.top.info.index - count);
    g_matrixStack.top.stack -= stackCount;
    workTransformMatrix = *g_matrixStack.top.stack;
}

void Matrix4x4::TransformPoint(const Vector3 &input, Vector3 &output) {
    const Matrix4x4 &m = workTransformMatrix;
    const Vector3 value = input;
    output.x = ((m.rows[1].x * value.y + m.rows[0].x * value.x) + m.rows[2].x * value.z) + m.rows[3].x;
    output.z = ((m.rows[1].z * value.y + m.rows[0].z * value.x) + m.rows[2].z * value.z) + m.rows[3].z;
    output.y = ((m.rows[1].y * value.y + m.rows[0].y * value.x) + m.rows[2].y * value.z) + m.rows[3].y;
}

void Matrix4x4::Transform(const Vector3 &input, Vector3 &output) {
    const Matrix4x4 &m = workTransformMatrix;
    const Vector3 value = input;
    output.x = (m.rows[1].x * value.y + m.rows[0].x * value.x) + m.rows[2].x * value.z;
    output.z = (m.rows[1].z * value.y + m.rows[0].z * value.x) + m.rows[2].z * value.z;
    output.y = (m.rows[1].y * value.y + m.rows[0].y * value.x) + m.rows[2].y * value.z;
}

void Matrix4x4::InvertRigidTransform() {
    const Matrix4x4 m = workTransformMatrix;
    const Vector4 &t = m.rows[3];
    workTransformMatrix.rows[0] = {m.rows[0].x, m.rows[1].x, m.rows[2].x, 0.0f};
    workTransformMatrix.rows[1] = {m.rows[0].y, m.rows[1].y, m.rows[2].y, 0.0f};
    workTransformMatrix.rows[2] = {m.rows[0].z, m.rows[1].z, m.rows[2].z, 0.0f};
    workTransformMatrix.rows[3] = {-((m.rows[0].y * t.y + m.rows[0].x * t.x) + m.rows[0].z * t.z),
        -((m.rows[1].y * t.y + m.rows[1].x * t.x) + m.rows[1].z * t.z), -((m.rows[2].y * t.y + m.rows[2].x * t.x) + m.rows[2].z * t.z), 1.0f};
}

bool Matrix4x4::SetActiveTransformToPrePass() {
    *g_matrixStack.top.stack = workTransformMatrix;
    ++g_matrixStack.top.info.index;
    const bool pushed = g_matrixStack.top.info.index < g_matrixStack.top.info.capacity;
    if (pushed)
        ++g_matrixStack.top.stack;
    workTransformMatrix = transformMatrix_prePass;
    return pushed;
}

void Matrix4x4::MultiplyWithPrePassMatrix(const Matrix4x4 *lhs) {
    Matrix4x4 product{};
    for (std::size_t row = 0; row < 4; ++row) {
        const float *const left = &lhs->rows[row].x;
        float *const output = &product.rows[row].x;
        for (std::size_t column = 0; column < 4; ++column) {
            output[column] = left[0] * (&workTransformMatrix.rows[0].x)[column] + left[1] * (&workTransformMatrix.rows[1].x)[column] +
                left[2] * (&workTransformMatrix.rows[2].x)[column] + left[3] * (&workTransformMatrix.rows[3].x)[column];
        }
    }
    workTransformMatrix = product;
}

void Matrix4x4::Multiply(const Matrix4x4 *lhs) {
    Matrix4x4::MultiplyWithPrePassMatrix(lhs);
}

// @verified - p
void Matrix4x4::ComputeColumnNorms(const Matrix4x4 *matrix, Vector4 *output) {

    const float y0 = matrix->rows[0].y, y2 = matrix->rows[2].y, y3 = matrix->rows[3].y, y1 = matrix->rows[1].y;
    const float z0 = matrix->rows[0].z, z2 = matrix->rows[2].z, z3 = matrix->rows[3].z, z1 = matrix->rows[1].z;
    output->x = std::sqrt(((matrix->rows[1].x * matrix->rows[1].x + matrix->rows[0].x * matrix->rows[0].x) + matrix->rows[2].x * matrix->rows[2].x) +
        matrix->rows[3].x * matrix->rows[3].x);
    output->y = std::sqrt(((y1 * y1 + y0 * y0) + y2 * y2) + y3 * y3);
    output->z = std::sqrt(((z1 * z1 + z0 * z0) + z2 * z2) + z3 * z3);

    output->w = 1.0f;
}

// @verified - p
Vector4 *Matrix4x4::ExtractEulerAnglesFromMatrix(const Matrix4x4 *matrix, Vector4 *output) {
    Vector4 norms;
    ComputeColumnNorms(matrix, &norms);
    const float y2 = matrix->rows[2].y / norms.y, z2 = matrix->rows[2].z / norms.z;
    const float x0 = matrix->rows[0].x / norms.x, x1 = matrix->rows[1].x / norms.x, x2 = matrix->rows[2].x / norms.x;
    output->x = std::atan2(y2, z2);
    output->y = std::atan2(-x2, std::sqrt(z2 * z2 + y2 * y2));
    output->z = std::atan2(x1, x0);
    return output;
}
bool Matrix4x4::invertMatrix(const Matrix4x4 *source, Matrix4x4 *output) {
    const float *m = &source->rows[0].x;
    float inverse[16];
    inverse[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    inverse[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    inverse[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    inverse[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
    inverse[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inverse[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inverse[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    inverse[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
    inverse[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
    inverse[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
    inverse[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
    inverse[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
    inverse[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
    inverse[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
    inverse[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
    inverse[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
    const float determinant = m[0] * inverse[0] + m[1] * inverse[4] + m[2] * inverse[8] + m[3] * inverse[12];
    if (determinant == 0.0f) {
        return false;
    }
    float *destination = &output->rows[0].x;
    const float scale = 1.0f / determinant;
    for (std::size_t index = 0; index < 16; ++index) {
        destination[index] = inverse[index] * scale;
    }
    return true;
}

Matrix4x4 *Matrix4x4::MakeFromQuat(Matrix4x4 *output, const Vector4 *first, const Vector4 *second) {
    const float inverseLength = 1.0f / std::sqrt(first->x * first->x + first->y * first->y + first->z * first->z + first->w * first->w);
    const Vector4 q{first->x * inverseLength, first->y * inverseLength, first->z * inverseLength, first->w * inverseLength};
    output->rows[0] = q;
    output->rows[1] = {0.5f * (q.w * second->x + q.z * second->y - q.y * second->z), 0.5f * (q.w * second->y - q.z * second->x + q.x * second->z),
        0.5f * (q.y * second->x - q.x * second->y + q.w * second->z), -0.5f * (q.x * second->x + q.y * second->y + q.z * second->z)};
    return output;
}

void Matrix4x4::InvertInPlace(Matrix4x4 *matrix) {
    Matrix4x4 *const destination = matrix ? matrix : g_matrixStack.top.stack;
    const Matrix4x4 *const source = matrix ? matrix : &workTransformMatrix;
    Matrix4x4 inverse;
    Matrix4x4::invertMatrix(source, &inverse);
    *destination = inverse;
    if (!matrix) {
        workTransformMatrix = inverse;
    }
}

void Matrix4x4::AffineProjectionSwap() {
    std::swap(workTransformMatrix.rows[0].y, workTransformMatrix.rows[1].x);
    std::swap(workTransformMatrix.rows[0].z, workTransformMatrix.rows[2].x);
    std::swap(workTransformMatrix.rows[0].w, workTransformMatrix.rows[3].x);
    std::swap(workTransformMatrix.rows[1].z, workTransformMatrix.rows[2].y);
    std::swap(workTransformMatrix.rows[1].w, workTransformMatrix.rows[3].y);
    std::swap(workTransformMatrix.rows[2].w, workTransformMatrix.rows[3].z);
}

void Matrix4x4::RotateMatrixCounterClockwise90() {
    Matrix4x4::AffineProjectionSwap();
}

} // namespace shenmue
