#include "common/matrix_stack.hpp"
#if SHENMUE_IS_DC
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_S2_GCC295
#define SHENMUE_MATRIX_NULL 0
#define SHENMUE_MATRIX_FMA(a, b, c) ((a) * (b) + (c))
#endif

#if SHENMUE_IS_DC && !SHENMUE_S2_GCC295
#define SHENMUE_MATRIX_NULL nullptr
#define SHENMUE_MATRIX_FMA __builtin_fmaf
#endif

#if !SHENMUE_IS_DC
#include "common/math.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#endif

namespace shenmue {

namespace matrix_stack {

#if SHENMUE_IS_DC && SHENMUE_S2_GCC295
Matrix4x4 g_worldToScreenMatrix = {};
Matrix4x4 g_savedWorkMatrix = {};
#endif

#if SHENMUE_IS_DC && !SHENMUE_S2_GCC295
Matrix4x4 g_worldToScreenMatrix{};
Matrix4x4 g_savedWorkMatrix{};
#endif

#if SHENMUE_IS_DC
float g_projectionViewportWidth = 640.0f;
float g_projectionViewportHeight = 480.0f;
float g_projectionViewportOriginY = 0.0f;
float g_projectionViewportOriginX = 0.0f;

// @verified - d
float InnerProduct4(const Vector4 &first, const Vector4 &second) {
    const float *source = first.data();
    const float *other = second.data();
    float result;
    asm volatile("fmov @%1+,fr0\n\tfmov @%1+,fr1\n\tfmov @%1+,fr2\n\tfmov @%1+,fr3\n\t"
                 "fmov @%2+,fr4\n\tfmov @%2+,fr5\n\tfmov @%2+,fr6\n\tfmov @%2+,fr7\n\tfipr fv0,fv4\n\tfmov fr7,%0"
        : "=f"(result), "+r"(source), "+r"(other)
        :
        : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7", "memory");
    return result;
}


// @verified - d
void RotateXSinCos(float sine, float cosine) {
    asm volatile("fmov %0,fr2\n\tfmov %1,fr1\n\tfmov %1,fr6\n\tfmov %0,fr5\n\tfneg fr5\n\t"
                 "fldi0 fr0\n\tfldi0 fr3\n\tftrv xmtrx,fv0\n\tfldi0 fr4\n\tfldi0 fr7\n\tftrv xmtrx,fv4\n\t"
                 "fschg\n\tfmov fr0,fr5\n\tfmov fr2,fr7\n\tfmov fr4,fr9\n\tfmov fr6,fr11\n\tfschg"
        :
        : "f"(sine), "f"(cosine)
        : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7");
}


// @verified - d
void RotateYSinCos(float sine, float cosine) {
    asm volatile("fmov %0,fr2\n\tfmov %1,fr0\n\tfmov %1,fr6\n\tfmov %0,fr4\n\tfneg fr2\n\t"
                 "fldi0 fr1\n\tfldi0 fr3\n\tftrv xmtrx,fv0\n\tfldi0 fr5\n\tfldi0 fr7\n\tftrv xmtrx,fv4\n\t"
                 "fschg\n\tfmov fr0,fr1\n\tfmov fr2,fr3\n\tfmov fr4,fr9\n\tfmov fr6,fr11\n\tfschg"
        :
        : "f"(sine), "f"(cosine)
        : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7");
}


// @verified - d
void RotateZSinCos(float sine, float cosine) {
    asm volatile("fmov %1,fr0\n\tfmov %0,fr1\n\tfmov %0,fr4\n\tfneg fr4\n\tfmov %1,fr5\n\t"
                 "fldi0 fr2\n\tfldi0 fr3\n\tftrv xmtrx,fv0\n\tfldi0 fr6\n\tfldi0 fr7\n\tftrv xmtrx,fv4\n\t"
                 "fschg\n\tfmov fr0,fr1\n\tfmov fr2,fr3\n\tfmov fr4,fr5\n\tfmov fr6,fr7\n\tfschg"
        :
        : "f"(sine), "f"(cosine)
        : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7");
}


// @verified - d
void CalcCameraTransform(const Vector4 &eye, const Vector4 &target, std::int16_t zAngle) {
    if (static_cast<std::uint16_t>(zAngle))
        dc::sdk::njRotateZ(SHENMUE_MATRIX_NULL, static_cast<std::uint16_t>(zAngle));

#if SHENMUE_IS_DC && SHENMUE_S2_GCC295
    const Vector3 direction = {target.x - eye.x, target.y - eye.y, target.z - eye.z};
#endif

#if SHENMUE_IS_DC && !SHENMUE_S2_GCC295
    const Vector3 direction{target.x - eye.x, target.y - eye.y, target.z - eye.z};
#endif
    float horizontalSquared = direction.z * direction.z;
    horizontalSquared = SHENMUE_MATRIX_FMA(direction.x, direction.x, horizontalSquared);
    const float inverseHorizontal = dc::sdk::InverseSqrt(horizontalSquared);
    float fullSquared = horizontalSquared;
    fullSquared = SHENMUE_MATRIX_FMA(direction.y, direction.y, fullSquared);
    const float inverseFull = dc::sdk::InverseSqrt(fullSquared);
    RotateXSinCos(-(direction.y * inverseFull), inverseFull / inverseHorizontal);
    RotateYSinCos(direction.x * inverseHorizontal, -(direction.z * inverseHorizontal));
    dc::sdk::njTranslate(SHENMUE_MATRIX_NULL, -eye.x, -eye.y, -eye.z);
}


// @verified - d
void TransformPosition(const Vector3 *input, Vector4 *output) {
    const float *source = input->data();
    float *destination = output->data();
    asm volatile("fmov @%0+,fr4\n\tfmov @%0+,fr5\n\tfmov @%0+,fr6\n\tfldi1 fr7\n\tftrv xmtrx,fv4\n\t"
                 "add #16,%1\n\tfmov fr7,@-%1\n\tfmov fr6,@-%1\n\tfmov fr5,@-%1\n\tfmov fr4,@-%1"
        : "+r"(source), "+r"(destination)
        :
        : "fr4", "fr5", "fr6", "fr7", "memory");
}
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2))
// @verified - d
void TransformVector4(const Vector4 *input, Vector4 *output) {
    const float *source = input->data();
    float *destination = output->data();
    asm volatile("fmov @%0+,fr4\n\tfmov @%0+,fr5\n\tfmov @%0+,fr6\n\tfmov @%0+,fr7\n\tftrv xmtrx,fv4\n\t"
                 "add #16,%1\n\tfmov fr7,@-%1\n\tfmov fr6,@-%1\n\tfmov fr5,@-%1\n\tfmov fr4,@-%1"
        : "+r"(source), "+r"(destination)
        :
        : "fr4", "fr5", "fr6", "fr7", "memory");
}


void CopyMatrix(Matrix4x4 *destination, const Matrix4x4 *source) {
    if (!destination) {
        if (source)
            LoadMatrix(source);
    } else if (!source) {
        GetMatrix(destination);
    } else {
        float *output = reinterpret_cast<float *>(destination);
        const float *input = reinterpret_cast<const float *>(source);
        for (int i = 0; i < 16; i++)
            *output++ = *input++;
    }
}
#endif

#if SHENMUE_IS_DC
// @verified - d
Matrix4x4 *GetMatrix(Matrix4x4 *memory) {
    if (!memory)
        memory = g_matrixStack.top.stack;
    dc::sdk::njGetMatrix(memory);
    return memory;
}


// @verified - d
void GetPreviousMatrix(Matrix4x4 *memory) {
    dc::sdk::njSetMatrix(memory, g_matrixStack.top.stack - 1);
}


// @verified - d
void SaveWorkMatrix() {
    GetMatrix(&g_savedWorkMatrix);
}


// @verified - d
void RestoreWorkMatrix() {
    LoadMatrix(&g_savedWorkMatrix);
}


// @verified - d
void GetTranslation(Vector3 *position) {
    dc::sdk::njGetTranslation(SHENMUE_MATRIX_NULL, position);
}


// @verified - d
void LoadMatrix(const Matrix4x4 *memory) {
    if (!memory)
        memory = g_matrixStack.top.stack;
    dc::sdk::njSetMatrix(SHENMUE_MATRIX_NULL, memory);
}


void LoadMatrixPairs(const Matrix4x4 *memory) {
    if (!memory)
        memory = g_matrixStack.top.stack;
    dc::sdk::njSetMatrix(SHENMUE_MATRIX_NULL, memory);
}


// @verified - d
bool Matrix4x4_Push(const Matrix4x4 *transform) {
    GetMatrix(SHENMUE_MATRIX_NULL);
    const bool available = g_matrixStack.top.info.capacity > g_matrixStack.top.info.index;
    ++g_matrixStack.top.info.index;
    ++g_matrixStack.top.stack;
    if (transform)
        LoadMatrix(transform);
    return available;
}
#endif

#if !SHENMUE_IS_DC
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
#endif

#if SHENMUE_IS_DC
// @verified - d
Matrix4x4 *MatrixStack_InitStackTableEntries(MatrixStackEntry *entry, Matrix4x4 *memory, std::int32_t capacity)
#endif

#if !SHENMUE_IS_DC
void MatrixStack_InitStackTableEntries(MatrixStackEntry *entry, Matrix4x4 *memory, std::int32_t capacity)
#endif
{

#if SHENMUE_IS_DC
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(memory);
#endif

#if SHENMUE_IS_DC && SHENMUE_S2_GCC295
    Matrix4x4 *const aligned = reinterpret_cast<Matrix4x4 *>((address + 31U) & ~std::uintptr_t(31));
#endif

#if SHENMUE_IS_DC && !SHENMUE_S2_GCC295
    Matrix4x4 *const aligned = reinterpret_cast<Matrix4x4 *>((address + 31U) & ~std::uintptr_t{31});
#endif

#if SHENMUE_IS_DC
    if (memory != aligned)
        capacity = static_cast<std::int32_t>(static_cast<std::uint32_t>(capacity) - 1U);
    if (capacity > 0x7fff)
#endif

#if !SHENMUE_IS_DC
    const auto address = reinterpret_cast<std::uintptr_t>(memory);
    auto *const alignedMemory = reinterpret_cast<Matrix4x4 *>((address + 0x1f) & ~std::uintptr_t{0x1f});
    if (memory != alignedMemory) {
        --capacity;
    }
    entry->stack = alignedMemory;
    entry->base = alignedMemory;
    if (capacity > 0x7fff) {
#endif
        capacity = 0x7fff;

#if !SHENMUE_IS_DC
    }
    entry->info.index = 0;
#endif
    entry->info.capacity = static_cast<std::int16_t>(capacity);

#if SHENMUE_IS_DC
    entry->info.index = 0;
    entry->stack = aligned;
    entry->base = aligned;
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2))
    dc::sdk::InitializeMatrixConstantsNullsub();
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S1) || (SHENMUE_IS_DEMO) || (!SHENMUE_IS_S2 && !SHENMUE_IS_S1 && !SHENMUE_IS_DEMO))
    dc::sdk::InitializeTransformConstants();
#endif

#if SHENMUE_IS_DC
    GetMatrix(entry->stack);
    return entry->base;
#endif

#if !SHENMUE_IS_DC
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4 *stack = entry->stack ? entry->stack : g_matrixStack.top.stack;
    *stack = workTransformMatrix;
#endif
}

#if SHENMUE_IS_DC
// @verified - d
Matrix4x4 *MatrixStack_InitTransformMatrixStack(Matrix4x4 *memory, std::int32_t size)
#endif

#if !SHENMUE_IS_DC
void MatrixStack_InitTransformMatrixStack(Matrix4x4 *memory, std::int32_t size)
#endif
{

#if SHENMUE_IS_DC
    MatrixStack_InitStackTableEntries(&g_matrixStack.entries[1], g_temporaryMatrixStacks.temporary1, 3);
    MatrixStack_InitStackTableEntries(&g_matrixStack.entries[2], g_temporaryMatrixStacks.temporary2, 3);
    Matrix4x4 *const base =
        MatrixStack_InitStackTableEntries(&g_matrixStack.entries[0], memory, static_cast<std::int32_t>(static_cast<std::uint32_t>(size) + 1U));
    g_CurrentMatrixStackIdx = 1;
#endif

#if !SHENMUE_IS_DC
    std::memset(&g_matrixStack, 0, sizeof(g_matrixStack));
    std::memset(&g_temporaryMatrixStacks, 0, sizeof(g_temporaryMatrixStacks));
    workTransformMatrix = {};
    MatrixStack_InitStackTableEntries(&g_matrixStack.entries[1], g_temporaryMatrixStacks.depthPrepass, 3);
    MatrixStack_InitStackTableEntries(&g_matrixStack.entries[2], g_temporaryMatrixStacks.opaque, 3);
    MatrixStack_InitStackTableEntries(&g_matrixStack.entries[0], memory, size + 1);
    MatrixStack_InitStackTableEntries(&g_matrixStack.entries[3], g_temporaryMatrixStacks.transparent, 3);
#endif
    g_matrixStack.top = g_matrixStack.entries[0];

#if SHENMUE_IS_DC
    return base;
#endif

#if !SHENMUE_IS_DC
    g_CurrentMatrixStackIdx = static_cast<std::uint16_t>(MatrixStackType::DepthPrepass);
#endif
}


// @verified - d
void SetActiveStack(MatrixStackType type) {

#if SHENMUE_IS_DC
    GetMatrix(SHENMUE_MATRIX_NULL);
    const std::uintptr_t table = reinterpret_cast<std::uintptr_t>(&g_matrixStack);
    const std::int32_t previous = static_cast<std::int8_t>(g_CurrentMatrixStackIdx * sizeof(MatrixStackEntry));
    *reinterpret_cast<MatrixStackEntry *>(table + previous) = g_matrixStack.top;
    g_CurrentMatrixStackIdx = static_cast<std::uint8_t>(type);
    const std::int32_t selected = static_cast<std::int8_t>(g_CurrentMatrixStackIdx * sizeof(MatrixStackEntry));
    g_matrixStack.top = *reinterpret_cast<const MatrixStackEntry *>(table + selected);
    LoadMatrix(g_matrixStack.top.stack);
#endif

#if !SHENMUE_IS_DC
    *g_matrixStack.top.stack = workTransformMatrix;
    const std::uint16_t previous = g_CurrentMatrixStackIdx;
    g_CurrentMatrixStackIdx = static_cast<std::uint16_t>(type);
    g_matrixStack.entries[previous - 1] = g_matrixStack.top;
    g_matrixStack.top = g_matrixStack.entries[g_CurrentMatrixStackIdx - 1];
    workTransformMatrix = *g_matrixStack.top.stack;
#endif
}


// @verified - d
void SetupStack() {

#if SHENMUE_IS_DC
    GetMatrix(SHENMUE_MATRIX_NULL);
#endif

#if !SHENMUE_IS_DC
    *g_matrixStack.top.stack = workTransformMatrix;
#endif
    g_matrixStack.top.stack = g_matrixStack.top.base;
    g_matrixStack.top.info.index = 0;

#if SHENMUE_IS_DC
    LoadMatrix(SHENMUE_MATRIX_NULL);
#endif

#if !SHENMUE_IS_DC
    workTransformMatrix = *g_matrixStack.top.base;
#endif
}

#if SHENMUE_IS_DC
// @verified - d
void TranslateVector(const Vector3 *translation) {
    Matrix4x4::preTranslate(translation->x, translation->y, translation->z);
}


// @verified - d
void TranslateInverseVector(const Vector3 *translation) {
    dc::sdk::njTranslate(SHENMUE_MATRIX_NULL, -translation->x, -translation->y, -translation->z);
}


// @verified - d
void UnitCurrentMatrix() {
    dc::sdk::njUnitMatrix(SHENMUE_MATRIX_NULL);
}


// @verified - d
bool PushUnitMatrix() {
    const bool available = Matrix4x4_Push(SHENMUE_MATRIX_NULL);
    UnitCurrentMatrix();
    return available;
}


// @verified - d
void MultiplyPreviousMatrix() {
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
}

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2))
// @verified - d
void UpdateWorldToScreenMatrix(float, float)
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S1) || (SHENMUE_IS_DEMO) || (!SHENMUE_IS_S2 && !SHENMUE_IS_S1 && !SHENMUE_IS_DEMO))
// @verified - d
void UpdateWorldToScreenMatrix()
#endif
{
    Matrix4x4_Push(g_matrixStack.entries[2].stack);
    MultiplyPreviousMatrix();
    GetMatrix(&g_worldToScreenMatrix);
    Matrix4x4::Pop(1);
}


// @verified - d
void ProjectWorldPointToScreen(const Vector3 *input, Vector2 *output) {
    Matrix4x4_Push(&g_worldToScreenMatrix);
    Vector4 projected;
    TransformPosition(input, &projected);
    const float inverse = 1.0f / projected.w;
    projected.x *= inverse;
    projected.y *= inverse;
    const float x = (projected.x + 1.0f) * g_projectionViewportWidth;
    const float half = 0.5f;
    float value = g_projectionViewportOriginX;
    value = SHENMUE_MATRIX_FMA(half, x, value);
    output->x = value;
    const float y = (-projected.y + 1.0f) * g_projectionViewportHeight;
    value = g_projectionViewportOriginY;
    value = SHENMUE_MATRIX_FMA(half, y, value);
    output->y = value;
    Matrix4x4::Pop(1);
}
#endif

namespace {

#if !SHENMUE_IS_DC
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
#endif

} // namespace

#if !SHENMUE_IS_DC
 // namespace

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
#endif

namespace {

#if !SHENMUE_IS_DC
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

// Native Vector3_NormalizeVector3D: a zero-length vector is returned unchanged.
Vector3 normalized(const Vector3 &value) {
    const float length = std::sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
    if (length <= 0.0f)
        return value;
    const float inverseLength = 1.0f / length;
    return {value.x * inverseLength, value.y * inverseLength, value.z * inverseLength};
}
#endif

} // namespace

#if !SHENMUE_IS_DC
 // namespace

// Native Multiply(out, Rx, Ry) then Multiply(out, that, Rz); its Multiply(out, a, b) is b * a, so the result is Rz * Ry * Rx.
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

// @verified - p
Matrix4x4 *ComputeLookAtViewMatrix(Matrix4x4 *output, const Vector3 *eye, const Vector3 *target, const Vector3 *up) {
    Vector3 forward{target->x - eye->x, target->y - eye->y, target->z - eye->z};
    const float length = std::sqrt((forward.y * forward.y + forward.x * forward.x) + forward.z * forward.z);
    if (length > 0.0f) {
        forward.x *= 1.0f / length;
        forward.y *= 1.0f / length;
        forward.z *= 1.0f / length;
    }
    Vector3 side{up->y * forward.z - up->z * forward.y, up->z * forward.x - up->x * forward.z, up->x * forward.y - up->y * forward.x};
    const float sideLength = std::sqrt((side.y * side.y + side.x * side.x) + side.z * side.z);
    if (sideLength > 0.0f) {
        side.x *= 1.0f / sideLength;
        side.y *= 1.0f / sideLength;
        side.z *= 1.0f / sideLength;
    }
    const Vector3 newUp{side.z * forward.y - side.y * forward.z, side.x * forward.z - side.z * forward.x, side.y * forward.x - side.x * forward.y};
    output->rows[0] = {side.x, side.y, side.z, -((eye->y * side.y + eye->x * side.x) + eye->z * side.z)};
    output->rows[1] = {newUp.x, newUp.y, newUp.z, -((eye->y * newUp.y + eye->x * newUp.x) + eye->z * newUp.z)};
    output->rows[2] = {forward.x, forward.y, forward.z, -((eye->y * forward.y + eye->x * forward.x) + eye->z * forward.z)};
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
#endif

} // namespace matrix_stack

#if !SHENMUE_IS_DC
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
    // Native constructs the inverse of the normalized affine transform in
    // the current matrix.  Translation is projected onto the original basis
    // before that basis is transposed.
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
#endif

#if !SHENMUE_IS_DC || (SHENMUE_IS_DC && ((SHENMUE_IS_S2 && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))))
// @verified - d
Matrix4x4 *Matrix4x4::GetTransformMatrix(Matrix4x4 *output) {

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2 && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))))
    return matrix_stack::GetMatrix(output);
#endif

#if !SHENMUE_IS_DC
    if (!output)
        output = g_matrixStack.top.stack;
    *output = workTransformMatrix;
    return output;
#endif
}
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2 && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))))
// @verified - d
void Matrix4x4::AffineProjectionSwap() {
    Matrix4x4 matrix;
    matrix_stack::GetMatrix(&matrix);
    for (std::int32_t row = 0; row < 4; ++row) {
        for (std::int32_t column = row + 1; column < 4; ++column) {
            float *const first = &matrix.rows[row].x + column;
            float *const second = &matrix.rows[column].x + row;
            const float value = *first;
            *first = *second;
            *second = value;
        }
    }
    matrix_stack::LoadMatrix(&matrix);
}
#endif

#if !SHENMUE_IS_DC || (SHENMUE_IS_DC && ((SHENMUE_IS_S2 && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))))
// @verified - d
void Matrix4x4::SetActiveTransform(const Matrix4x4 *transform) {

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2 && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))))
    matrix_stack::LoadMatrix(transform);
#endif

#if !SHENMUE_IS_DC
    if (!transform)
        transform = g_matrixStack.top.stack;
    workTransformMatrix = *transform;
#endif
}


// @verified - d
void Matrix4x4::SetPrePassToTempTransform() {

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2 && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))))
    matrix_stack::UnitCurrentMatrix();
#endif

#if !SHENMUE_IS_DC
    workTransformMatrix = transformMatrix_prePass;
#endif
}
#endif

#if SHENMUE_IS_DC
void Matrix4x4::RotateAroundY(float sine, float cosine) {
    matrix_stack::RotateYSinCos(sine, cosine);
}

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2))
// @verified - d
void Matrix4x4::RotateAroundY_Wrapper(std::int32_t angle)
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S1) || (SHENMUE_IS_DEMO) || (!SHENMUE_IS_S2 && !SHENMUE_IS_S1 && !SHENMUE_IS_DEMO))
// @verified - d
void Matrix4x4::RotateAroundY_Wrapper(std::uint16_t angle)
#endif
{
    Vector2 values;
    CalculateFixedSinCos(angle, &values);
    matrix_stack::RotateYSinCos(values.x, values.y);
}

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2))
// @verified - d
void Matrix4x4::RotateAroundX_Wrapper(std::int32_t angle)
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S1) || (SHENMUE_IS_DEMO) || (!SHENMUE_IS_S2 && !SHENMUE_IS_S1 && !SHENMUE_IS_DEMO))
// @verified - d
void Matrix4x4::RotateAroundX_Wrapper(std::uint16_t angle)
#endif
{
    Vector2 values;
    CalculateFixedSinCos(angle, &values);
    matrix_stack::RotateXSinCos(values.x, values.y);
}

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2))
// @verified - d
void Matrix4x4::RotateAroundZ_Wrapper(std::int32_t angle)
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S1) || (SHENMUE_IS_DEMO) || (!SHENMUE_IS_S2 && !SHENMUE_IS_S1 && !SHENMUE_IS_DEMO))
// @verified - d
void Matrix4x4::RotateAroundZ_Wrapper(std::uint16_t angle)
#endif
{
    Vector2 values;
    CalculateFixedSinCos(angle, &values);
    matrix_stack::RotateZSinCos(values.x, values.y);
}


// @verified - d
void Matrix4x4::Multiply(const Matrix4x4 *matrix) {
    dc::sdk::njMultiMatrix(SHENMUE_MATRIX_NULL, matrix);
}


// @verified - d
void Matrix4x4::preTranslate(float x, float y, float z) {
    dc::sdk::njTranslate(SHENMUE_MATRIX_NULL, x, y, z);
}


// @verified - d
void Matrix4x4::Scale(float x, float y, float z) {
    dc::sdk::njScale(SHENMUE_MATRIX_NULL, x, y, z);
}

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2))
// @verified - d
void Matrix4x4::SetViewMatrixFromEuler(std::int32_t rotationX, std::int32_t rotationY, std::int32_t rotationZ)
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S1) || (SHENMUE_IS_DEMO) || (!SHENMUE_IS_S2 && !SHENMUE_IS_S1 && !SHENMUE_IS_DEMO))
// @verified - d
void Matrix4x4::SetViewMatrixFromEuler(std::int32_t rotationY, std::int32_t rotationX, std::int32_t rotationZ)
#endif
{
    const std::uint32_t x = static_cast<std::uint16_t>(rotationX);
    const std::uint32_t y = static_cast<std::uint16_t>(rotationY);
    const std::uint32_t z = static_cast<std::uint16_t>(rotationZ);
    if (!(x | z)) {
        if (y) {
            Vector2 values;
            CalculateFixedSinCos(y, &values);
            matrix_stack::RotateYSinCos(values.x, values.y);
        }
        return;
    }
    Vector2 xValues, yValues, zValues;
    CalculateFixedSinCos(x, &xValues);
    CalculateFixedSinCos(y, &yValues);
    CalculateFixedSinCos(z, &zValues);
    const float sx = xValues.x, cx = xValues.y, sy = yValues.x, cy = yValues.y, sz = zValues.x, cz = zValues.y;
    const float sxcz = sx * cz;
    const float sxsz = sx * sz;
    const float cxsz = cx * sz;
    const float cxcz = cx * cz;

#if SHENMUE_IS_DC && SHENMUE_S2_GCC295
    const Matrix4x4 rotation = {{{cy * cz, cy * sz, -sy, 0.0f}, {sxcz * sy - cxsz, sxsz * sy + cxcz, sx * cy, 0.0f},
        {cxcz * sy + sxsz, cxsz * sy - sxcz, cx * cy, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}};
#endif

#if SHENMUE_IS_DC && !SHENMUE_S2_GCC295
    const Matrix4x4 rotation{{Vector4{cy * cz, cy * sz, -sy, 0.0f}, Vector4{sxcz * sy - cxsz, sxsz * sy + cxcz, sx * cy, 0.0f},
        Vector4{cxcz * sy + sxsz, cxsz * sy - sxcz, cx * cy, 0.0f}, Vector4{0.0f, 0.0f, 0.0f, 1.0f}}};
#endif
    Matrix4x4::Multiply(&rotation);
}
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2))
// @verified - d
void Matrix4x4::NormalizeAndSetTransformMatrix() {
    Matrix4x4 matrix;
    matrix_stack::GetMatrix(&matrix);
    float projectedTranslation[3];
    for (std::uint32_t index = 0; index < 3; ++index) {
        const float *translation = matrix.rows[3].data();
        const float *axis = matrix.rows[index].data();
        float projected;
        asm volatile("fmov @%1+,fr4\n\tfmov @%1+,fr5\n\tfmov @%1+,fr6\n\tfmov @%1,fr7\n\t"
                     "fmov @%2+,fr8\n\tfmov @%2+,fr9\n\tfmov @%2+,fr10\n\tfmov @%2,fr11\n\tfipr fv4,fv8\n\tfmov fr11,%0"
            : "=&f"(projected), "+r"(translation), "+r"(axis)
            :
            : "fr4", "fr5", "fr6", "fr7", "fr8", "fr9", "fr10", "fr11", "memory");
        projectedTranslation[index] = -projected;
    }
    float inverseLengths[3];
    for (std::uint32_t index = 0; index < 3; ++index) {
        matrix.rows[index].w = 0.0f;
        const float *axis = matrix.rows[index].data();
        float inverse;
        asm volatile("fmov @%1+,fr4\n\tfmov @%1+,fr5\n\tfmov @%1+,fr6\n\tfmov @%1,fr7\n\tfipr fv4,fv4\n\tfsrra fr7\n\tfmov fr7,%0"
            : "=&f"(inverse), "+r"(axis)
            :
            : "fr4", "fr5", "fr6", "fr7", "memory");
        inverseLengths[index] = inverse;
    }

#if SHENMUE_IS_DC && SHENMUE_S2_GCC295
    const Matrix4x4 normalized = {{
        {matrix.rows[0].x * inverseLengths[0], matrix.rows[1].x * inverseLengths[1], matrix.rows[2].x * inverseLengths[2], 0.0f},
        {matrix.rows[0].y * inverseLengths[0], matrix.rows[1].y * inverseLengths[1], matrix.rows[2].y * inverseLengths[2], 0.0f},
        {matrix.rows[0].z * inverseLengths[0], matrix.rows[1].z * inverseLengths[1], matrix.rows[2].z * inverseLengths[2], 0.0f},
        {projectedTranslation[0] * inverseLengths[0], projectedTranslation[1] * inverseLengths[1], projectedTranslation[2] * inverseLengths[2],
            matrix.rows[3].w},
#endif

#if SHENMUE_IS_DC && ((SHENMUE_IS_S2 && (!SHENMUE_S2_GCC295)))
    const Matrix4x4 normalized{{
        {.x = matrix.rows[0].x * inverseLengths[0], .y = matrix.rows[1].x * inverseLengths[1], .z = matrix.rows[2].x * inverseLengths[2], .w = 0.0f},
        {.x = matrix.rows[0].y * inverseLengths[0], .y = matrix.rows[1].y * inverseLengths[1], .z = matrix.rows[2].y * inverseLengths[2], .w = 0.0f},
        {.x = matrix.rows[0].z * inverseLengths[0], .y = matrix.rows[1].z * inverseLengths[1], .z = matrix.rows[2].z * inverseLengths[2], .w = 0.0f},
        {.x = projectedTranslation[0] * inverseLengths[0],
            .y = projectedTranslation[1] * inverseLengths[1],
            .z = projectedTranslation[2] * inverseLengths[2],
            .w = matrix.rows[3].w},
#endif
    }};
    matrix_stack::LoadMatrix(&normalized);
}
#endif

#if !SHENMUE_IS_DC
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
#endif


// @verified - d
void Matrix4x4::TransformPoint(const Vector3 &input, Vector3 &output) {

#if SHENMUE_IS_DC
    dc::sdk::njCalcPoint(SHENMUE_MATRIX_NULL, const_cast<Vector3 *>(&input), &output);
#endif

#if !SHENMUE_IS_DC
    const Matrix4x4 &m = workTransformMatrix;
    const Vector3 value = input;
    output.x = ((m.rows[1].x * value.y + m.rows[0].x * value.x) + m.rows[2].x * value.z) + m.rows[3].x;
    output.z = ((m.rows[1].z * value.y + m.rows[0].z * value.x) + m.rows[2].z * value.z) + m.rows[3].z;
    output.y = ((m.rows[1].y * value.y + m.rows[0].y * value.x) + m.rows[2].y * value.z) + m.rows[3].y;
#endif
}


// @verified - d
void Matrix4x4::Transform(const Vector3 &input, Vector3 &output) {

#if SHENMUE_IS_DC
    dc::sdk::njCalcVector(SHENMUE_MATRIX_NULL, const_cast<Vector3 *>(&input), &output);
#endif

#if !SHENMUE_IS_DC
    const Matrix4x4 &m = workTransformMatrix;
    const Vector3 value = input;
    output.x = (m.rows[1].x * value.y + m.rows[0].x * value.x) + m.rows[2].x * value.z;
    output.z = (m.rows[1].z * value.y + m.rows[0].z * value.x) + m.rows[2].z * value.z;
    output.y = (m.rows[1].y * value.y + m.rows[0].y * value.x) + m.rows[2].y * value.z;
#endif
}


// @verified - d
void Matrix4x4::InvertRigidTransform() {

#if SHENMUE_IS_DC
    asm volatile("frchg\n\tfldi0 fr3\n\tfsub fr11,fr11\n\tfldi0 fr7\n\tfldi1 fr15\n\t"
                 "flds fr1,fpul\n\tfipr fv12,fv0\n\tfmov fr4,fr1\n\tfipr fv12,fv4\n\tfsts fpul,fr4\n\tfipr fv12,fv8\n\t"
                 "fldi0 fr12\n\tfsub fr14,fr14\n\tfsub fr3,fr12\n\tfldi0 fr13\n\tfsub fr7,fr13\n\tflds fr2,fpul\n\t"
                 "fsub fr11,fr14\n\tfmov fr8,fr2\n\tfsub fr3,fr3\n\tfsts fpul,fr8\n\tfsub fr7,fr7\n\tflds fr6,fpul\n\t"
                 "fsub fr11,fr11\n\tfmov fr9,fr6\n\tfsts fpul,fr9\n\tfrchg"
        :
        :
        : "fpul", "memory");
#endif

#if !SHENMUE_IS_DC
    const Matrix4x4 m = workTransformMatrix;
    const Vector4 &t = m.rows[3];
    workTransformMatrix.rows[0] = {m.rows[0].x, m.rows[1].x, m.rows[2].x, 0.0f};
    workTransformMatrix.rows[1] = {m.rows[0].y, m.rows[1].y, m.rows[2].y, 0.0f};
    workTransformMatrix.rows[2] = {m.rows[0].z, m.rows[1].z, m.rows[2].z, 0.0f};
    workTransformMatrix.rows[3] = {-((m.rows[0].y * t.y + m.rows[0].x * t.x) + m.rows[0].z * t.z),
        -((m.rows[1].y * t.y + m.rows[1].x * t.x) + m.rows[1].z * t.z), -((m.rows[2].y * t.y + m.rows[2].x * t.x) + m.rows[2].z * t.z), 1.0f};
#endif
}

#if SHENMUE_IS_DC
// @verified - d
void Matrix4x4::Pop(std::int32_t count) {
    if (count <= 0)
        count = 1;
    std::int32_t index = matrix_stack::g_matrixStack.top.info.index;
    if (index < count)
        index = 0;
    else
        index -= count;
    Matrix4x4 *const matrix = matrix_stack::g_matrixStack.top.base + index;
    matrix_stack::g_matrixStack.top.info.index = static_cast<std::int16_t>(index);
    if (index < matrix_stack::g_matrixStack.top.info.capacity)
        matrix_stack::g_matrixStack.top.stack = matrix;
    matrix_stack::LoadMatrix(matrix);
}
#endif

#if !SHENMUE_IS_DC
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

// Native Matrix4x4_InvertInPlace: the scaled, row-pivoted LU factorization is made in place, so a singular matrix keeps its partial
// factorization; only a non-zero determinant solves the identity's columns into the matrix (and the work matrix for the stack top).
void Matrix4x4::InvertInPlace(Matrix4x4 *matrix) {
    Matrix4x4 *const target = matrix ? matrix : g_matrixStack.top.stack;
    if (!matrix)
        *target = workTransformMatrix;
    std::array<std::uint8_t, 4> rowIndices{};
    if (decomposeMatrix(*target, rowIndices) == 0.0f)
        return;
    Matrix4x4 inverse;
    float *const solved = &inverse.rows[0].x;
    for (std::size_t column = 0; column < 4; ++column) {
        for (std::size_t row = 0; row < 4; ++row) {
            const float *const factors = &target->rows[rowIndices[row]].x;
            float value = rowIndices[row] == column ? 1.0f : 0.0f;
            for (std::size_t previous = 0; previous < row; ++previous)
                value -= factors[previous] * solved[previous * 4 + column];
            solved[row * 4 + column] = value;
        }
        for (std::size_t row = 4; row-- > 0;) {
            const float *const factors = &target->rows[rowIndices[row]].x;
            float value = solved[row * 4 + column];
            for (std::size_t next = row + 1; next < 4; ++next)
                value -= factors[next] * solved[next * 4 + column];
            solved[row * 4 + column] = value / factors[row];
        }
    }
    *target = inverse;
    if (!matrix)
        workTransformMatrix = *target;
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
#endif

} // namespace shenmue

#if SHENMUE_IS_DC
 // namespace shenmue

#undef SHENMUE_MATRIX_NULL
#undef SHENMUE_MATRIX_FMA
#endif
