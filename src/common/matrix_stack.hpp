#pragma once

#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::matrix_stack {

enum class MatrixStackType : std::uint8_t {
    PreRender = 0,
    DepthPrepass = 1,
    Opaque = 2,
    Transparent = 3,
    PostRender = 4,
};

struct MatrixStackInfo {
    std::int16_t index;
    std::int16_t capacity;
};

struct MatrixStackEntry {
    MatrixStackInfo info;
    std::int16_t reserved_04;
    std::int16_t reserved_06;
    Matrix4x4 *base;
    Matrix4x4 *stack;
};

struct MatrixStackTable {
    MatrixStackEntry top;
    MatrixStackEntry entries[4];
};

struct TemporaryMatrixStacks {
    Matrix4x4 depthPrepass[3];
    Matrix4x4 opaque[3];
    Matrix4x4 transparent[3];
};

static_assert(sizeof(MatrixStackEntry) == 0x18);
static_assert(sizeof(MatrixStackTable) == 0x78);
static_assert(sizeof(TemporaryMatrixStacks) == 0x240);

extern Matrix4x4 workTransformMatrix;
extern Matrix4x4 transformMatrix_prePass;
extern MatrixStackTable g_matrixStack;
extern TemporaryMatrixStacks g_temporaryMatrixStacks;
extern std::uint16_t g_CurrentMatrixStackIdx;

void MatrixStack_InitStackTableEntries(MatrixStackEntry *entry, Matrix4x4 *memory, std::int32_t capacity);
void MatrixStack_InitTransformMatrixStack(Matrix4x4 *memory, std::int32_t size);
void SetActiveStack(MatrixStackType type);
void SetupStack();
bool Matrix4x4_Push(const Matrix4x4 *transform = nullptr);
Vector3 *ComputeEulerAnglesFromVector(const Matrix4x4 *matrix, Vector3 *output);
Vector4 *PerformPerspectiveProjectionTransform(const Matrix4x4 *matrix, Vector4 *output, const Vector3 *input);
Matrix4x4 *createTransformedMatrix(const Matrix4x4 *source, Matrix4x4 *output);
Matrix4x4 *RotateMatrixByAngles(Matrix4x4 *output, float x, float y, float z);
Matrix4x4 *ComputeRotationMatrixFromVectorAndAngle(Matrix4x4 *output, const Vector3 *axis, float angle);
Matrix4x4 *ComputeRotationMatrixFromVectors(Matrix4x4 *output, const Vector3 *from, const Vector3 *to);
Matrix4x4 *ComputeLookAtViewMatrix(Matrix4x4 *output, const Vector3 *eye, const Vector3 *target, const Vector3 *up);
Vector4 *RotationMatrixToQuaternion(const Matrix4x4 *matrix, Vector4 *output);
void CalcCameraTransform(const Vector4 &eye, const Vector4 &target, std::int16_t zAngle);

} // namespace shenmue::matrix_stack
