#include "s2/modifier_volume_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "platform/dc/mt7.hpp"
#include "platform/dc/ninja.hpp"
#include "platform/dc/sdk.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
void ApplyShadowVolumeProjection(const Matrix4x4 *projection, const Matrix4x4 *viewport) {
    dc::sdk::MultiplyMT7WorkMatrix(projection);
    dc::sdk::MultiplyMT7WorkMatrix(viewport);
}

// @verified - d
void PrepareShadowVolumeProjection() {
    ApplyShadowVolumeProjection(matrix_stack::g_matrixStack.entries[2].stack, &dc::sdk::g_mt7ViewportTransform);
}

} // namespace s2
} // namespace shenmue

#endif
