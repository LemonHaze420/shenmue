#include "s2/frame_pipeline.hpp"

#include "s2/model_draw.hpp"
#include "s2/sd_light.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/legacy_draw.hpp"
#include "d3t/dx11_renderer.hpp"
#include <cstring>

#include "common/matrix_stack.hpp"
#include "d3t/model_cache.hpp"
#include "d3t/buffer.hpp"
#include "d3t/texture.hpp"

    namespace shenmue::s2 {
Matrix4x4 g_previousCameraProjection = d3t::kModelCacheIdentity;
Matrix4x4 g_postEffectProjection = d3t::kModelCacheIdentity;

// @verified - p
std::int32_t g_modelDebugRenderMode{};
std::uint8_t g_modelVisualizeCascade{};
std::int32_t g_modelSpecularBrdfMode{};
std::uint8_t g_modelUseDualQuaternion = 1;
std::uint8_t g_modelVisualizeClusters{};
float g_modelClampDirectRoughness = 0.4f;

// @verified - p
void SetDebugShaderParams(d3t::D3TDeviceContext * context) {
#if defined(_WIN32)
    ModelDebugShaderConstants *const constants = static_cast<ModelDebugShaderConstants *>(d3t::MapTempConstantBufferForWriteDiscard(context, 3, 9, 36));
    if (constants)
        *constants = {.renderMode = g_modelDebugRenderMode,
            .visualizeCascade = g_modelVisualizeCascade,
            .environmentProbesEnabled = 0,
            .probeVisualizationIndex = 0,
            .specularBrdfMode = g_modelSpecularBrdfMode,
            .probeVisualizationMode = 0,
            .useDualQuaternion = g_modelUseDualQuaternion,
            .visualizeClusters = g_modelVisualizeClusters,
            .clampDirectRoughness = g_modelClampDirectRoughness};
#endif
}

void ConfigureAndSubmitModelScenePass(d3t::D3TDeviceContext * context, ModelScenePass & pass, ModelGeometryContext & geometry) {
    if (GetLegacyPerspectiveProjection() == 4)
        return;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    std::uint32_t rasterizer = (pass.reverseCulling == 0) | (2U * ((g_modelDebugRenderMode == 6) | (pass.type == 1 ? 4100U : 4104U)));
    if (GetAspectRatioModeShouldScale() && !pass.disableAspectCulling && !(pass.type & ~2U))
        rasterizer |= 4U;
    if (state.rasterizerState != rasterizer) {
        state.rasterizerState = rasterizer;
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
    }
    const bool transparent = (pass.materialMask & 8U) != 0;
    const std::uint64_t depthFlags = !pass.deferred || transparent ? (transparent ? 0x15U : 0x17U) : 13U;
    std::uint64_t previousDepth;
    std::memcpy(&previousDepth, state.depthStencilState.data(), sizeof(previousDepth));
    if (previousDepth != depthFlags || state.stencilReference != 0) {
        std::memcpy(state.depthStencilState.data(), &depthFlags, sizeof(depthFlags));
        std::memset(state.depthStencilState.data() + 8, 0, 8);
        state.stencilReference = 0;
        state.pipelineStateFlags |= d3t::PipelineDepthStencil;
    }
    const std::uint64_t blend = transparent ? 0xf0000285U : 0xf0000000U;
    std::uint32_t previousBlend;
    std::memcpy(&previousBlend, state.blendState.data(), sizeof(previousBlend));
    if (previousBlend != static_cast<std::uint32_t>(blend))
        state.pipelineStateFlags |= d3t::PipelineBlend;
    std::memcpy(state.blendState.data(), &blend, sizeof(blend));
#if defined(_WIN32)
    ModelDepthConstants *const depth = static_cast<ModelDepthConstants *>(d3t::MapTempConstantBufferForWriteDiscard(context, 2, 12, 20));
    if (depth)
        *depth = {.nearFarProduct = 450.00003f, .farMinusNear = 2999.8501f, .farPlane = 3000.0f, .parameterScale = 1.0f, .parameterBias = 5.0f};
#endif
    ModelGeometryVisibility &visibility = *static_cast<ModelGeometryVisibility *>(AllocateModelGeometryFrameMemory(sizeof(ModelGeometryVisibility)));
    visibility = {.geometry = &geometry, .nodeMask = nullptr};
    const ModelScenePass cullPass = pass;
    CullModelGeometry(visibility, cullPass);
    if (pass.type == 0 || pass.type == 6 || pass.type == 7)
        pass.alternateShader = 1;
    if (!g_PPFX_Enabled)
        pass.disableVelocity = 1;
    pass.clusterNear = 0.15000000596046448f;
    pass.clusterFar = 3000.0f;
    pass.alternateNodes = g_sdNightTime == 1;
    const ModelScenePass drawPass = pass;
    SubmitModelGeometry(visibility, context, drawPass);
    visibility = {};
}
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
