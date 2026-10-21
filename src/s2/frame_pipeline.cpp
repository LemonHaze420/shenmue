#include "s2/frame_pipeline.hpp"

#include "s2/sd_probe.hpp"
#include "s2/simple_shader.hpp"
#include "s2/renderer_initialization.hpp"
#include <limits>
#include <bit>
#if defined(_WIN32)
#include <d3d11.h>
#endif
#include "s2/input_bindings.hpp"
#include "d3t/ui_renderer.hpp"
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

std::uint8_t g_modelSceneSuppressed{};
float g_currentFractionOfDay{};
float g_sceneTimeOscillation{};
float g_sceneTimeDirection = 1.0f;
float g_timeSinceStart{};

// @verified - p
void UpdateFractionOfDay(float fractionOfDay, std::int64_t, float frameTime) {
    g_currentFractionOfDay = fractionOfDay;
    g_sceneTimeOscillation += g_sceneTimeDirection * frameTime;
    if (g_sceneTimeOscillation > 5000.0f)
        g_sceneTimeDirection = -1.0f;
    else if (g_sceneTimeOscillation < 0.0f)
        g_sceneTimeDirection = 1.0f;
}

// @verified - p
void IncrementTimeSinceStart(float frameTime) {
    if (g_timeSinceStart >= std::numeric_limits<float>::max() - frameTime)
        g_timeSinceStart = frameTime + 1.0f;
    else
        g_timeSinceStart += frameTime;
}

void SD_Override() {
#if defined(_WIN32)
    if (!g_sdOverrideModelCache || GetLegacyPerspectiveProjection() == 1 || g_modelSceneSuppressed)
        return;
    d3t::D3TDeviceContext *const context = d3t::DXGetPipelineContext(1);
    SetupPostFXRenderTargetsAndViewport(context);
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.topology != 0) {
        state.bufferStateFlags |= 1U;
        state.topology = 0;
    }
    RefreshModelCacheMaterials(g_sdOverrideModelCache, context, g_sdShaderFileLock);
    CameraConstantBlock camera;
    camera.projectedOrigin = {};
    CreateCameraMatrices(camera);
    Matrix4x4 viewProjection = camera.viewProjection;
    UpdateModelSceneLights(context, &viewProjection);
    SetDebugShaderParams(context);
    SetSceneSharedState(context);
    void *const mappedCamera = d3t::MapTempConstantBufferForWriteDiscard(context, 3, 0, sizeof(CameraConstantBlock));
    if (mappedCamera)
        std::memcpy(mappedCamera, &camera, sizeof(camera));
    FrameConstantBlock *const frame = static_cast<FrameConstantBlock *>(d3t::MapTempConstantBufferForWriteDiscard(context, 3, 3, sizeof(FrameConstantBlock)));
    if (frame)
        frame->time = g_currentFractionOfDay;
    UpdateModelSceneProbes(context);
    const ModelScenePass base{.materialMask = 7,
        .cameraPosition = {camera.projectedOrigin.x, camera.projectedOrigin.y, camera.projectedOrigin.z},
        .fogDisabled = 1,
        .disableAspectCulling = 1};
    ExtractAndNormalizeSixFrustumPlanes(g_modelFrustumPlanes, camera.viewProjection);
    ModelGeometryContext *const geometry = BuildModelGeometry(g_sdOverrideModelCache, context);
    ModelScenePass pass = base;
    ConfigureAndSubmitModelScenePass(context, pass, *geometry);
    pass = base;
    pass.materialMask = 8;
    ConfigureAndSubmitModelScenePass(context, pass, *geometry);
    d3t::DX11::WaitForCommandListFinish(context);
#endif
}

void DrawSceneTexture(d3t::D3TDeviceContext *context, std::uint32_t source, std::uint32_t destination) {
#if defined(_WIN32)
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    state.bufferStateFlags |= 2U;
    state.outputTargets.fill(std::byte{});
    std::memcpy(state.outputTargets.data(), &destination, 4);
    Vector2i size;
    if (!d3t::QueryTextureSize(destination, &size))
        return;
    const std::array<float, 6> viewport{
        0.0f, 0.0f, static_cast<float>(static_cast<std::uint32_t>(size.x)), static_cast<float>(static_cast<std::uint32_t>(size.y)), 0.0f, 1.0f};
    if (std::memcmp(state.viewport.data(), viewport.data(), sizeof(viewport)) != 0) {
        state.viewport = viewport;
        state.viewportDirtyMask |= 1U;
    }
    if (state.vertexShader != texturedQuadVertexShader) {
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = 0xffffffffU;
    }
    state.vertexShader = texturedQuadVertexShader;
    if (state.pixelShader != texturedQuadPixelShader)
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    state.pixelShader = texturedQuadPixelShader;
    if (state.inputLayout != g_fvfLayouts[34]) {
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
        state.inputLayout = g_fvfLayouts[34];
    }
    const std::uint64_t blend = 0xf0000000U;
    std::uint32_t previousBlend;
    std::memcpy(&previousBlend, state.blendState.data(), 4);
    if (previousBlend != static_cast<std::uint32_t>(blend))
        state.pipelineStateFlags |= d3t::PipelineBlend;
    std::memcpy(state.blendState.data(), &blend, 8);
    std::uint64_t previousDepth;
    std::memcpy(&previousDepth, state.depthStencilState.data(), 8);
    if (previousDepth || state.stencilReference) {
        state.pipelineStateFlags |= d3t::PipelineDepthStencil;
        state.depthStencilState.fill(std::byte{});
        state.stencilReference = 0;
    }
    if (state.rasterizerState != 1) {
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
        state.rasterizerState = 1;
    }
    d3t::D3TSamplerStateKey &sampler = state.bindings.samplers[1][0];
    if ((sampler.flags[0] | (static_cast<std::uint64_t>(sampler.flags[1]) << 32U)) != 5) {
        state.samplerDirtyMasks[1] |= 1U;
        sampler = {.flags = {5, 0}, .reserved = {}};
    }
    const S2UserConfiguration280 &configuration = GetRuntimeUserConfiguration();
    const float scale = configuration.enableSuperSampling ? 1.0f : 0.4444444477558136f;
    const float u = (((1.0f / (configuration.superSampling + 1.0f)) * 0.0f) + 1.0f) * scale;
    const std::array<Vector4, 4> vertices{{{.x = -1.0f, .y = -1.0f, .z = 0.0f, .w = scale}, {.x = 1.0f, .y = -1.0f, .z = u, .w = scale},
        {.x = -1.0f, .y = 1.0f, .z = 0.0f, .w = 0.0f}, {.x = 1.0f, .y = 1.0f, .z = u, .w = 0.0f}}};
    void *const mapped = d3t::GetMappedConstantBuffer(context, 0, sizeof(vertices));
    if (mapped)
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
    if (state.topology != 1) {
        state.bufferStateFlags |= 1U;
        state.topology = 1;
    }
    if (state.bindings.textureHandles[1][8] != source || state.bindings.textureFlags[1][8] != 0) {
        state.shaderResourceDirtyMasks[1] |= 0x100U;
        state.bindings.textureHandles[1][8] = source;
        state.bindings.textureFlags[1][8] = 0;
    }
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(4, 0);
    if (state.topology != 0) {
        state.bufferStateFlags |= 1U;
        state.topology = 0;
    }
#endif
}

void DrawSDTransparent() {
#if defined(_WIN32)
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context)
        return;
    const S2UserConfiguration280 &configuration = GetRuntimeUserConfiguration();
    if (configuration.superSampling + 1.0f > 1.0f) {
        const std::uint32_t destination = configuration.enableSuperSampling ? d3t::g_resolvedSceneTexture : d3t::g_legacySceneTexture;
        DrawSceneTexture(context, d3t::g_sceneTexture, destination);
    }
#endif
}

std::uint32_t g_postEffectSceneTexture{};

void SetBufferAndRectRenderFlags(d3t::D3TDeviceContext *context) {
#if defined(_WIN32)
    if (!context)
        return;
    const std::uint32_t texture = d3t::CreateNewTexture2D();
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    state.bufferStateFlags |= 2U;
    state.outputTargets.fill(std::byte{});
    std::memcpy(state.outputTargets.data(), &texture, 4);
    const float width = static_cast<float>(std::bit_cast<std::int32_t>(d3t::g_originalScreenWidth));
    const float height = static_cast<float>(std::bit_cast<std::int32_t>(d3t::g_originalScreenHeight));
    const std::array<float, 6> viewport{0.0f, 0.0f, width, height, 0.0f, 1.0f};
    if (std::memcmp(state.viewport.data(), viewport.data(), sizeof(viewport)) != 0) {
        state.viewport = viewport;
        state.viewportDirtyMask |= 1U;
    }
    const std::array<std::int32_t, 4> scissor{0, 0, TruncateToInt32(width), TruncateToInt32(height)};
    if (std::memcmp(state.scissorRect.data(), scissor.data(), sizeof(scissor)) != 0) {
        state.scissorRect = scissor;
        state.bufferStateFlags |= 4U;
    }
#endif
}

d3t::D3TDeviceContext *RenderSimplePixelShaderPass(const std::uint32_t *texture, std::uint8_t flags) {
#if defined(_WIN32)
    SetBufferAndRectRenderFlags(d3t::DX11::g_ppD3DDeviceContext);
    d3t::D3TDeviceContext *const context = d3t::DX11::g_ppD3DDeviceContext;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const std::uint32_t vertexShader = flags & SPSF_Gamma ? texturedQuadGammaVertexShader : texturedQuadVertexShader;
    const std::uint32_t pixelShader = flags & SPSF_Gamma ? texturedQuadGammaPixelShader : texturedQuadPixelShader;
    const std::uint32_t layout = g_fvfLayouts[flags & SPSF_Gamma ? 35 : 34];
    if (state.vertexShader != vertexShader) {
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = 0xffffffffU;
    }
    state.vertexShader = vertexShader;
    if (state.pixelShader != pixelShader)
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    state.pixelShader = pixelShader;
    if (state.inputLayout != layout) {
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
        state.inputLayout = layout;
    }
    const std::uint64_t blend = 0xf0000000U;
    std::uint32_t previousBlend;
    std::memcpy(&previousBlend, state.blendState.data(), 4);
    if (previousBlend != static_cast<std::uint32_t>(blend))
        state.pipelineStateFlags |= d3t::PipelineBlend;
    std::memcpy(state.blendState.data(), &blend, 8);
    std::uint64_t previousDepth;
    std::memcpy(&previousDepth, state.depthStencilState.data(), 8);
    if (previousDepth || state.stencilReference) {
        state.pipelineStateFlags |= d3t::PipelineDepthStencil;
        state.depthStencilState.fill(std::byte{});
        state.stencilReference = 0;
    }
    if (state.rasterizerState != 1) {
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
        state.rasterizerState = 1;
    }
    d3t::D3TSamplerStateKey &sampler = state.bindings.samplers[1][0];
    if ((sampler.flags[0] | (static_cast<std::uint64_t>(sampler.flags[1]) << 32U)) != 5) {
        state.samplerDirtyMasks[1] |= 1U;
        sampler = {.flags = {5, 0}, .reserved = {}};
    }
    float scale = 1.0f;
    if (flags & SPSF_PreserveAspect) {
        Vector2i size;
        if (d3t::QueryTextureSize(*texture, &size))
            scale = (static_cast<float>(static_cast<std::uint32_t>(size.x)) / static_cast<float>(static_cast<std::uint32_t>(size.y))) /
                (static_cast<float>(std::bit_cast<std::int32_t>(d3t::g_originalScreenWidth)) /
                    static_cast<float>(std::bit_cast<std::int32_t>(d3t::g_originalScreenHeight)));
    }
    const std::array<Vector4, 4> vertices{{{.x = -scale, .y = -1.0f, .z = 0.0f, .w = 1.0f}, {.x = scale, .y = -1.0f, .z = 1.0f, .w = 1.0f},
        {.x = -scale, .y = 1.0f, .z = 0.0f, .w = 0.0f}, {.x = scale, .y = 1.0f, .z = 1.0f, .w = 0.0f}}};
    void *const mapped = d3t::GetMappedConstantBuffer(context, 0, sizeof(vertices));
    if (mapped)
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
    if (state.topology != 1) {
        state.bufferStateFlags |= 1U;
        state.topology = 1;
    }
    const std::uint32_t source = texture ? *texture : 0;
    if (state.bindings.textureHandles[1][8] != source || state.bindings.textureFlags[1][8] != 0) {
        state.shaderResourceDirtyMasks[1] |= 0x100U;
        state.bindings.textureHandles[1][8] = source;
        state.bindings.textureFlags[1][8] = 0;
    }
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(4, 0);
    d3t::D3TDeviceContext *const result = d3t::DX11::g_ppD3DDeviceContext;
    d3t::D3TRenderState &finalState = d3t::GetRenderState(*result);
    if (finalState.topology != 0) {
        finalState.bufferStateFlags |= 1U;
        finalState.topology = 0;
    }
    return result;
#else
    return nullptr;
#endif
}

void DoSimplePixelShaderPass() {
#if defined(_WIN32)
    if (!d3t::DX11::g_ppD3DDeviceContext)
        return;
    if (g_PPFX_Enabled) {
        RenderSimplePixelShaderPass(&g_postEffectSceneTexture, SPSF_Gamma);
    } else {
        const std::uint32_t *const texture = GetRuntimeUserConfiguration().enableSuperSampling ? &d3t::g_resolvedSceneTexture : &d3t::g_legacySceneTexture;
        RenderSimplePixelShaderPass(texture, SPSF_Gamma);
    }
#endif
}
void GetPostFXScaledRenderTargetSize(float *width, float *height) {
    Vector2i dimensions;
    if (!d3t::QueryTextureSize(d3t::g_sceneTexture, &dimensions))
        return;
    const S2UserConfiguration280 &configuration = GetRuntimeUserConfiguration();
    const float scale = configuration.enableSuperSampling ? 1.0f : 0.4444444477558136f;
    *width = (static_cast<float>(dimensions.x) * scale) * (((1.0f / (configuration.superSampling + 1.0f)) * 0.0f) + 1.0f);
    *height = static_cast<float>(dimensions.y) * (configuration.enableSuperSampling ? 1.0f : 0.4444444477558136f);
}

void SetupPostFXRenderTargetsAndViewport(d3t::D3TDeviceContext * context) {
    if (!context)
        return;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    state.bufferStateFlags |= 2U;
    state.outputTargets.fill(std::byte{});
    std::memcpy(state.outputTargets.data(), &d3t::g_sceneTexture, 4);
    std::memcpy(state.outputTargets.data() + 128, &d3t::g_sceneDepthTexture, 4);
    float width, height;
    GetPostFXScaledRenderTargetSize(&width, &height);
    const std::array<float, 6> viewport{0.0f, 0.0f, width, height, 0.0f, 1.0f};
    if (std::memcmp(state.viewport.data(), viewport.data(), sizeof(viewport)) != 0) {
        state.viewport = viewport;
        state.viewportDirtyMask |= 1U;
    }
}

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
