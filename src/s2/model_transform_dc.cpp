#include "s2/model_transform_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"

namespace shenmue {
namespace s2 {

__attribute__((aligned(32))) Matrix4x4 g_modelNodeTransformBase;

// @verified - d
void CaptureModelNodeTransformBase() {
    matrix_stack::GetMatrix(&g_modelNodeTransformBase);
}

// @verified - d
void RestoreModelNodeTransformBase() {
    matrix_stack::LoadMatrix(&g_modelNodeTransformBase);
}

} // namespace s2
} // namespace shenmue

#endif
