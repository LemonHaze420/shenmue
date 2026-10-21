#include "s2/frame_pipeline.hpp"

#include "common/matrix_stack.hpp"
#include "d3t/model_cache.hpp"
#include "d3t/buffer.hpp"
#include "d3t/texture.hpp"

namespace shenmue::s2 {
Matrix4x4 g_previousCameraProjection = d3t::kModelCacheIdentity;
Matrix4x4 g_postEffectProjection = d3t::kModelCacheIdentity;

// @verified - p
void SetSceneSharedState(d3t::D3TDeviceContext *context) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.bindings.textureHandles[1][27] != d3t::g_defaultWhiteTexture || state.bindings.textureFlags[1][27] != 0) {
        state.shaderResourceDirtyMasks[1] |= 1U << 27U;
        state.bindings.textureHandles[1][27] = d3t::g_defaultWhiteTexture;
        state.bindings.textureFlags[1][27] = 0;
    }
    for (std::uint32_t index = 0; index < 2; ++index) {
        const std::uint32_t slot = 12U + index;
        const std::uint32_t flags = index ? 4757U : 4736U;
        d3t::D3TSamplerStateKey &sampler = state.bindings.samplers[1][slot];
        if ((sampler.flags[0] | (static_cast<std::uint64_t>(sampler.flags[1]) << 32U)) != flags) {
            state.samplerDirtyMasks[1] |= static_cast<std::uint16_t>(1U << slot);
            sampler = {.flags = {flags, 0}, .reserved = {}};
        }
    }
}

// @verified - p
void CreateCameraMatrices(CameraConstantBlock &block) {
    using namespace matrix_stack;
    SetActiveStack(MatrixStackType::PostRender);
    *g_matrixStack.top.stack = workTransformMatrix;
    for (std::size_t row = 0; row < 4; ++row)
        for (std::size_t column = 0; column < 4; ++column)
            block.view.rows[row][column] = g_matrixStack.top.stack->rows[column][row];
    SetActiveStack(MatrixStackType::Transparent);
    *g_matrixStack.top.stack = workTransformMatrix;
    for (std::size_t row = 0; row < 4; ++row)
        for (std::size_t column = 0; column < 4; ++column)
            block.projection.rows[row][column] = g_matrixStack.top.stack->rows[column][row];
    Matrix4x4 matrix;
    Matrix4x4::invertMatrix(&block.projection, &matrix);
    block.inverseProjection = matrix;
    for (std::size_t row = 0; row < 4; ++row)
        for (std::size_t column = 0; column < 4; ++column)
            matrix.rows[row][column] =
                ((block.projection.rows[row].x * block.view.rows[0][column] + block.projection.rows[row].y * block.view.rows[1][column]) +
                    block.projection.rows[row].z * block.view.rows[2][column]) +
                block.projection.rows[row].w * block.view.rows[3][column];
    block.viewProjection = matrix;
    block.inverseView = *createTransformedMatrix(&block.view, &matrix);
    const Matrix4x4 &inverse = block.inverseView;
    const float w = ((inverse.rows[3].y * 0.0f + inverse.rows[3].x * 0.0f) + inverse.rows[3].z * 0.0f) + inverse.rows[3].w;
    const float reciprocal = 1.0f / w;
    block.projectedOrigin = {.x = reciprocal * (((inverse.rows[0].y * 0.0f + inverse.rows[0].x * 0.0f) + inverse.rows[0].z * 0.0f) + inverse.rows[0].w),
        .y = reciprocal * (((inverse.rows[1].y * 0.0f + inverse.rows[1].x * 0.0f) + inverse.rows[1].z * 0.0f) + inverse.rows[1].w),
        .z = reciprocal * (((inverse.rows[2].y * 0.0f + inverse.rows[2].x * 0.0f) + inverse.rows[2].z * 0.0f) + inverse.rows[2].w),
        .w = reciprocal * w};
    for (std::size_t row = 0; row < 4; ++row)
        for (std::size_t column = 0; column < 4; ++column) {
            block.previousProjection.rows[row][column] = g_previousCameraProjection.rows[column][row];
            block.postEffectProjection.rows[row][column] = g_postEffectProjection.rows[column][row];
        }
    SetActiveStack(MatrixStackType::DepthPrepass);
}
} // namespace shenmue::s2
