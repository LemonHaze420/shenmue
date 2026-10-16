#pragma once

#include "common/matrix_stack.hpp"

namespace shenmue::s2 {

using matrix_stack::CalcCameraTransform;
void ApplyShearToPrePassMatrix(float sine, float cosine);
// S2 Matrix4x4_Push checks capacity before incrementing; S1's shared implementation
// deliberately increments first and therefore cannot implement this boundary.
inline bool Matrix4x4_Push(const Matrix4x4 *transform = nullptr) {
    auto &top = matrix_stack::g_matrixStack.top;
    *top.stack = matrix_stack::workTransformMatrix;
    if (transform)
        matrix_stack::workTransformMatrix = *transform;
    if (top.info.index >= top.info.capacity)
        return false;
    ++top.stack;
    ++top.info.index;
    return true;
}
// Matrix4x4::PushActiveTransform: the same capacity-checked push, then the prepass matrix.
inline void Matrix4x4_PushActiveTransform() {
    Matrix4x4_Push();
    matrix_stack::workTransformMatrix = matrix_stack::transformMatrix_prePass;
}
using matrix_stack::MatrixStack_InitStackTableEntries;
using matrix_stack::MatrixStack_InitTransformMatrixStack;
using matrix_stack::MatrixStackEntry;
using matrix_stack::MatrixStackInfo;
using matrix_stack::MatrixStackTable;
using matrix_stack::MatrixStackType;
using matrix_stack::SetActiveStack;
using matrix_stack::SetupStack;
using matrix_stack::TemporaryMatrixStacks;
using matrix_stack::g_CurrentMatrixStackIdx;
using matrix_stack::g_matrixStack;
using matrix_stack::g_temporaryMatrixStacks;
using matrix_stack::transformMatrix_prePass;
using matrix_stack::workTransformMatrix;

} // namespace shenmue::s2
