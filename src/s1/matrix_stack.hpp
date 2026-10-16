#pragma once

#include "common/matrix_stack.hpp"

namespace shenmue::s1 {

using matrix_stack::CalcCameraTransform;
using matrix_stack::Matrix4x4_Push;
using matrix_stack::MatrixStack_InitStackTableEntries;
using matrix_stack::MatrixStack_InitTransformMatrixStack;
using matrix_stack::MatrixStackEntry;
using matrix_stack::MatrixStackInfo;
using matrix_stack::MatrixStackTable;
using matrix_stack::MatrixStackType;
using matrix_stack::SetActiveStack;
using matrix_stack::TemporaryMatrixStacks;
using matrix_stack::g_CurrentMatrixStackIdx;
using matrix_stack::g_matrixStack;
using matrix_stack::g_temporaryMatrixStacks;
using matrix_stack::transformMatrix_prePass;
using matrix_stack::workTransformMatrix;

} // namespace shenmue::s1
