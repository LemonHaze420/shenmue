#pragma once

#include "common/math.hpp"

#if !SHENMUE_S2_GCC295
#include <cstddef>
#include <cstdint>
#endif

namespace shenmue {
namespace matrix_stack {

#if SHENMUE_S2_GCC295
typedef std::uint8_t MatrixStackType;
enum MatrixStackValue {
#else
enum class MatrixStackType : std::uint8_t {
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    Current = 0,
    Primary = 1,
    Temporary1 = 2,
    Temporary2 = 3,
#else
    PreRender = 0,
    DepthPrepass = 1,
    Opaque = 2,
    Transparent = 3,
    PostRender = 4,
#endif
};

struct MatrixStackInfo {
    std::int16_t index;
    std::int16_t capacity;
};

struct MatrixStackEntry {
    MatrixStackInfo info;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    std::int16_t reserved_04;
    std::int16_t reserved_06;
#endif
    Matrix4x4 *base;
    Matrix4x4 *stack;
};

struct MatrixStackTable {
    MatrixStackEntry top;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    MatrixStackEntry entries[3];
#else
    MatrixStackEntry entries[4];
#endif
};

struct TemporaryMatrixStacks {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    Matrix4x4 temporary1[3];
    Matrix4x4 temporary2[3];
#else
    Matrix4x4 depthPrepass[3];
    Matrix4x4 opaque[3];
    Matrix4x4 transparent[3];
#endif
};

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#if SHENMUE_S2_GCC295
typedef char MatrixStackTypeSizeCheck[sizeof(MatrixStackType) == 1 ? 1 : -1];
typedef char MatrixStackEntrySizeCheck[sizeof(MatrixStackEntry) == 0x0c ? 1 : -1];
typedef char MatrixStackBaseOffsetCheck[offsetof(MatrixStackEntry, base) == 4 ? 1 : -1];
typedef char MatrixStackCurrentOffsetCheck[offsetof(MatrixStackEntry, stack) == 8 ? 1 : -1];
typedef char MatrixStackTableSizeCheck[sizeof(MatrixStackTable) == 0x30 ? 1 : -1];
typedef char TemporaryMatrixStacksSizeCheck[sizeof(TemporaryMatrixStacks) == 0x180 ? 1 : -1];
#else
static_assert(sizeof(MatrixStackEntry) == 0x0c);
static_assert(offsetof(MatrixStackEntry, base) == 4);
static_assert(offsetof(MatrixStackEntry, stack) == 8);
static_assert(sizeof(MatrixStackTable) == 0x30);
static_assert(sizeof(TemporaryMatrixStacks) == 0x180);
#endif

extern MatrixStackTable g_matrixStack asm("_shenmue_dc_matrix_stack");
extern TemporaryMatrixStacks g_temporaryMatrixStacks asm("_shenmue_dc_temporary_matrix_stacks");
extern std::uint8_t g_CurrentMatrixStackIdx asm("_shenmue_dc_current_matrix_stack");
Matrix4x4 *MatrixStack_InitStackTableEntries(MatrixStackEntry *entry, Matrix4x4 *memory, std::int32_t capacity);
Matrix4x4 *MatrixStack_InitTransformMatrixStack(Matrix4x4 *memory, std::int32_t size);
#else
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
#endif
void SetActiveStack(MatrixStackType type);
void SetupStack();
bool Matrix4x4_Push(const Matrix4x4 *transform = 0);
Vector3 *ComputeEulerAnglesFromVector(const Matrix4x4 *matrix, Vector3 *output);
Vector4 *PerformPerspectiveProjectionTransform(const Matrix4x4 *matrix, Vector4 *output, const Vector3 *input);
Matrix4x4 *createTransformedMatrix(const Matrix4x4 *source, Matrix4x4 *output);
Matrix4x4 *RotateMatrixByAngles(Matrix4x4 *output, float x, float y, float z);
Matrix4x4 *ComputeRotationMatrixFromVectorAndAngle(Matrix4x4 *output, const Vector3 *axis, float angle);
Matrix4x4 *ComputeRotationMatrixFromVectors(Matrix4x4 *output, const Vector3 *from, const Vector3 *to);
Matrix4x4 *ComputeLookAtViewMatrix(Matrix4x4 *output, const Vector3 *eye, const Vector3 *target, const Vector3 *up);
Vector4 *RotationMatrixToQuaternion(const Matrix4x4 *matrix, Vector4 *output);
void CalcCameraTransform(const Vector4 &eye, const Vector4 &target, std::int16_t zAngle);

} } // namespace shenmue::matrix_stack
