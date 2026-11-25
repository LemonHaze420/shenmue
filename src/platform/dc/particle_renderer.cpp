#include "platform/dc/particle_renderer.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

#include "platform/dc/matrix.hpp"
#include "platform/dc/ninja.hpp"

namespace shenmue { namespace dc { namespace sdk {

Matrix4x4 g_particleRenderMatrix __attribute__((aligned(32))) = {};
Vector4 g_particleProjectionScales __attribute__((aligned(32))) = {};
Matrix4x4 g_particleCallbackMatrix __attribute__((aligned(32))) = {};
Matrix4x4 g_particleFullMatrix[2] __attribute__((aligned(32))) = {};
Matrix4x4 g_particleFullSecondMatrix[2] __attribute__((aligned(32))) = {};
Matrix4x4 g_particleXyMatrix __attribute__((aligned(32))) = {};
Matrix4x4 g_particleXyzMatrix __attribute__((aligned(32))) = {};
Matrix4x4 g_particleCompactMatrix[2] __attribute__((aligned(32))) = {};
Matrix4x4 g_particleCompactSecondMatrix[2] __attribute__((aligned(32))) = {};
Matrix4x4 g_particleCompactXyMatrix __attribute__((aligned(32))) = {};
Matrix4x4 g_particleCompactXyzMatrix __attribute__((aligned(32))) = {};

// @verified - d
void PrepareParticleTransform() {
    matrix_stack::Matrix4x4_Push(&kParticleScreenMatrix);
    Matrix4x4::Multiply(matrix_stack::g_matrixStack.entries[2].stack);
    njGetMatrix(&g_particleRenderMatrix);
    g_particleProjectionScales.x = g_particleRenderMatrix.rows[0].x;
    g_particleProjectionScales.y = g_particleRenderMatrix.rows[0].y;
    g_particleProjectionScales.z = g_particleRenderMatrix.rows[1].x;
    g_particleProjectionScales.w = g_particleRenderMatrix.rows[1].y;
    matrix_stack::MultiplyPreviousMatrix();
}

// Native discard changes the common stack cursor while retaining the live hardware matrix.
// @verified - d
void DiscardParticleMatrix(std::int32_t count) {
    if (count <= 0)
        count = 1;
    std::int32_t index = matrix_stack::g_matrixStack.top.info.index;
    if (index < count)
        index = 0;
    else
        index -= count;
    matrix_stack::g_matrixStack.top.info.index = static_cast<std::int16_t>(index);
    if (index < matrix_stack::g_matrixStack.top.info.capacity)
        matrix_stack::g_matrixStack.top.stack = matrix_stack::g_matrixStack.top.base + index;
}

} } } // namespace shenmue::dc::sdk

#endif
