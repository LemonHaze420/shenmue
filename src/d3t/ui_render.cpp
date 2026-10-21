#include "d3t/ui_internal.hpp"

#if SHENMUE_IS_S2
#include "s2/frame_pipeline.hpp"
#endif

#if SHENMUE_IS_S1
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/d3t/d3t_threads.hpp"
#include "s1/d3t/model.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/render_passes.hpp"
#include "s1/shader_initialization.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/matrix_stack.hpp"
#endif

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <d3d11.h>
#endif

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif

namespace {



alignas(16) std::array<std::byte, 0x190000> g_nuklearVertexStaging{};
alignas(16) std::array<std::byte, 0x6e000> g_nuklearIndexStaging{};

template <typename T> T ReadState(const std::byte *state, std::size_t offset) {
    T value{};
    std::memcpy(&value, state + offset, sizeof(value));
    return value;
}

template <typename T> void WriteState(std::byte *state, std::size_t offset, const T &value) {
    std::memcpy(state + offset, &value, sizeof(value));
}

} // namespace

#if SHENMUE_IS_S2
// @verified - p
std::uint32_t InitNuklearScreenTexture() {
    d3t::InitTexture2DResource(g_nuklearTexture);
    const d3t::Texture2DDescription description{.width = g_uiScreenWidth,
        .height = 1080,
        .arraySize = 1,
        .mipLevels = 1,
        .format = 3,
        .bindFlags = 4,
        .flags = 0,
        .sampleCount = 1,
        .sampleQuality = 0,
        .reserved_24 = 0,
        .initialData = nullptr};
    g_nuklearTexture = d3t::CreateTexture2D(&description);
    return g_nuklearTexture;
}
#endif

#if SHENMUE_IS_S2
// @verified - p
void SetNuklearViewMatrix(const Matrix4x4 &matrix) {
    Matrix4x4 transposed;
    for (std::int32_t row = 0; row < 4; ++row)
        for (std::int32_t column = 0; column < 4; ++column)
            transposed.rows[row][column] = matrix.rows[column][row];
    g_bOrthographicRender = false;
    g_notebookPageMatrix = transposed;
}

// @verified - p
d3t::D3TDeviceContext *SetupNuklearBuffer() {
#if defined(_WIN32) || defined(SHENMUE_NULL_LIBRARY)
    if (!g_NuklearInitialized)
        return nullptr;
    d3t::D3TDeviceContext *const context = d3t::DXGetPipelineContext(1);
    SetBufferAndRectRenderFlags(context);
    return context;
#else
    return nullptr;
#endif
}

void CreateOrthographicViewMatrix(std::int32_t width, std::int32_t height, Matrix4x4 *matrix) {
    const float x = static_cast<float>(width);
    const float y = static_cast<float>(height);
    *matrix = {.rows = {{.x = 2.0f / x}, {.y = 2.0f / (0.0f - y)}, {.z = 0.5f}, {.x = (x + 0.0f) / (0.0f - x), .y = (y + 0.0f) / y, .z = 0.5f, .w = 1.0f}}};
}

std::intptr_t InvokeNuklearFrame(d3t::D3TDeviceContext *context) {
    g_nuklearRenderDeviceContext = context;
    return g_nuklearFrameCallback ? g_nuklearFrameCallback() : 0;
}

void FinishNuklearFrame() {
    if (!g_nuklearRenderDeviceContext)
        return;
#if defined(_WIN32)
    d3t::DX11::WaitForCommandListFinish(g_nuklearRenderDeviceContext);
#endif
    g_nuklearRenderDeviceContext = nullptr;
    g_Nuklear_num_indices = 0;
    g_Nuklear_num_verts = 0;
}

UIViewport AdjustScreenForAspectRatio(std::int32_t mode) {
    const std::int32_t bufferWidth = static_cast<std::int32_t>(g_uiScreenWidth);
    const std::int32_t originalWidth = static_cast<std::int32_t>(g_originalScreenWidth);
    const std::int32_t screenWidth = originalWidth > bufferWidth ? bufferWidth : originalWidth;
    float adjustment = 0.0f;
    if (mode == 1) {
        const float buffer = static_cast<float>(bufferWidth);
        const float screenHeight = static_cast<float>(static_cast<std::int32_t>(g_originalScreenHeight));
        const float scale = (1.0f / (1080.0f / buffer)) * (screenHeight / static_cast<float>(screenWidth));
        adjustment = static_cast<float>(static_cast<std::uint32_t>(static_cast<std::int64_t>(buffer / scale))) - 1439.964f;
    }
    return {.rect = {.x = adjustment * 0.5f, .y = 0.0f, .z = static_cast<float>(bufferWidth) - adjustment, .w = 1080.0f}, .depthRange = {.x = 0.0f, .y = 1.0f}};
}

void SetNuklearAspectRatio(std::int32_t mode) {
    const UIViewport viewport = AdjustScreenForAspectRatio(mode);
    g_currentUIViewportRect = viewport.rect;
    g_currentUIViewportDepth = viewport.depthRange;
}

void Nuklear_BindRenderTarget() {
    d3t::D3TDeviceContext *const context = g_nuklearRenderDeviceContext;
    if (!context || !g_NuklearInitialized)
        return;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const bool redirected = g_nuklearTargetRedirectPending;
    const std::uint32_t texture = redirected ? *g_nuklearTargetRedirect : g_nuklearTexture;
    g_nuklearTargetRedirectPending = false;
    state.outputTargets.fill(std::byte{});
    std::memcpy(state.outputTargets.data(), &texture, sizeof(texture));
    state.bufferStateFlags |= d3t::BufferOutputTargets;
#if defined(_WIN32)
    if (!redirected) {
        constexpr std::array<float, 4> transparent{};
        d3t::UnmapAndClearViews(context, texture, 0, transparent.data());
    }
#endif
}

void NuklearUpdateForAspectRatio(std::int32_t mode) {
    d3t::D3TDeviceContext *const context = g_nuklearRenderDeviceContext;
    if (!context || !g_NuklearInitialized)
        return;
    const UIViewport viewport = AdjustScreenForAspectRatio(mode);
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (std::memcmp(state.viewport.data(), &viewport, sizeof(viewport)) != 0) {
        state.viewportDirtyMask |= 1U;
        std::memcpy(state.viewport.data(), &viewport, sizeof(viewport));
    }
    Nuklear_RenderPass(&viewport);
}

void NuklearCompositeNullsub(d3t::D3TDeviceContext *, ...) {
}

void CopyNuklearTexToBackBuffer(d3t::D3TDeviceContext *context, const std::uint32_t *texture) {
#if defined(_WIN32)
    NuklearCompositeNullsub(context, "NK GUI Texture to backbuffer");
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    std::byte *const bytes = context->renderState.data();
    const std::uint32_t backBuffer = d3t::CreateNewTexture2D();
    state.outputTargets.fill(std::byte{});
    WriteState(state.outputTargets.data(), 0, backBuffer);
    state.bufferStateFlags |= d3t::BufferOutputTargets;
    const std::int32_t width = static_cast<std::int32_t>(g_originalScreenWidth);
    const std::int32_t height = static_cast<std::int32_t>(g_originalScreenHeight);
    const std::array<float, 6> viewport{0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f};
    if (std::memcmp(state.viewport.data(), viewport.data(), sizeof(viewport)) != 0) {
        state.viewportDirtyMask |= 1U;
        state.viewport = viewport;
    }
    const std::array<std::int32_t, 4> scissor{0, 0, width, height};
    if (state.scissorRect != scissor) {
        state.bufferStateFlags |= 4U;
        state.scissorRect = scissor;
    }
    if (state.vertexShader != texturedQuadVertexShader) {
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = 0xffffffffU;
        state.vertexShader = texturedQuadVertexShader;
    }
    if (state.pixelShader != texturedQuadPixelShader) {
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
        state.pixelShader = texturedQuadPixelShader;
    }
    if (state.inputLayout != g_fvfLayouts[34]) {
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
        state.inputLayout = g_fvfLayouts[34];
    }
    if (ReadState<std::uint32_t>(state.blendState.data(), 0) != static_cast<std::uint32_t>(kNuklearCompositeBlend))
        state.pipelineStateFlags |= d3t::PipelineBlend;
    WriteState(state.blendState.data(), 0, kNuklearCompositeBlend);
    if (ReadState<std::uint64_t>(state.depthStencilState.data(), 0) != 0U || state.stencilReference != 0U) {
        state.pipelineStateFlags |= d3t::PipelineDepthStencil;
        state.depthStencilState = {};
        state.stencilReference = 0;
    }
    if (state.rasterizerState != 1U) {
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
        state.rasterizerState = 1;
    }
    constexpr std::size_t samplerOffset = 0x534;
    if (ReadState<std::uint64_t>(bytes, samplerOffset) != 5U) {
        state.samplerDirtyMasks[1] |= 1U;
        WriteState(bytes, samplerOffset, std::array<std::uint64_t, 2>{5U, 0U});
    }
    const std::int32_t screenWidth = width > static_cast<std::int32_t>(g_uiScreenWidth) ? static_cast<std::int32_t>(g_uiScreenWidth) : width;
    const float widthRatio = static_cast<float>(width) / static_cast<float>(screenWidth);
    const float quadWidth = 2.0f / widthRatio;
    const float left = (2.0f - quadWidth) * 0.5f - 1.0f;
    const float inverseAspect = 1.0f / (1080.0f / static_cast<float>(g_uiScreenWidth));
    const float bottom = 1.0f / ((static_cast<float>(height) / static_cast<float>(screenWidth)) * inverseAspect);
    const std::array<Vector4, 4> vertices{{{.x = left, .y = -1.0f, .z = 0.0f, .w = 1.0f}, {.x = quadWidth + left, .y = -1.0f, .z = bottom, .w = 1.0f},
        {.x = left, .y = 1.0f, .z = 0.0f, .w = 0.0f}, {.x = quadWidth + left, .y = 1.0f, .z = bottom, .w = 0.0f}}};
    if (void *const mapped = d3t::GetMappedConstantBuffer(context, 0, sizeof(vertices)))
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip)) {
        state.bufferStateFlags |= d3t::BufferTopology;
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip);
    }
    constexpr std::size_t textureOffset = 0x1d4;
    constexpr std::size_t flagsOffset = 0x354;
    if (ReadState<std::uint32_t>(bytes, textureOffset) != *texture || ReadState<std::uint32_t>(bytes, flagsOffset) != 0U) {
        state.shaderResourceDirtyMasks[1] |= 0x100U;
        WriteState(bytes, textureOffset, *texture);
        WriteState(bytes, flagsOffset, std::uint32_t{0});
    }
    d3t::DX11_ResetMappedResources(context);
    d3t::DX11_UnmapAndSyncShaderBindings(context);
    d3t::DX11_SetupShadersForDXAndSetViewports(context);
    static_cast<ID3D11DeviceContext *>(context->deviceContext)->Draw(4, 0);
    state.bufferStateFlags |= d3t::BufferTopology;
    state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleList);
    NuklearCompositeNullsub(context);
#endif
}

void CopyNuklearTexToBackBuffer() {
    d3t::D3TDeviceContext *const context = g_nuklearRenderDeviceContext;
    if (!context)
        return;
    CopyNuklearTexToBackBuffer(context, &g_nuklearTexture);
    NuklearCompositeNullsub(context);
}
#endif


std::array<nk_panel, 2> g_aspectBarPanels{};





void DrawAspectBars(float alpha, const UIViewport &viewport, std::int32_t reveal) {
    const std::int32_t bufferWidth = static_cast<std::int32_t>(g_uiScreenWidth);
    const std::int32_t originalWidth = static_cast<std::int32_t>(d3t::g_originalScreenWidth);
    const std::int32_t screenWidth = originalWidth > bufferWidth ? bufferWidth : originalWidth;
    const float buffer = static_cast<float>(bufferWidth);
    const float screenHeight = static_cast<float>(static_cast<std::int32_t>(d3t::g_originalScreenHeight));
    const float scale = (1.0f / (1080.0f / buffer)) * (screenHeight / static_cast<float>(screenWidth));
#if SHENMUE_IS_S1
    const float frameWidth = static_cast<float>(static_cast<std::int32_t>(buffer / scale));
#else
    const float frameWidth = static_cast<float>(static_cast<std::uint32_t>(static_cast<std::int64_t>(buffer / scale)));
#endif
    nk_context &context = GNuklearWindow;
    nk_clear(&context);
    const std::int32_t opacity = static_cast<std::int32_t>(std::fmin(alpha * 8.0f, 1.0f) * 255.0f);
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, opacity < 0 ? 0 : opacity < 255 ? opacity : 255));
    if (reveal)
        context.style.window.fixed_background.data.color = nk_rgba(255, 255, 255, 50);
    const auto bar = [&context](nk_panel &panel, const char *name, struct nk_rect rect) {
        nk_begin_titled(&context, &panel, name, name, rect, NK_WINDOW_NO_SCROLLBAR);
        if (context.current) {
            if (!(context.current->flags & NK_WINDOW_HIDDEN))
                nk_end(&context);
            context.current = nullptr;
        }
    };
    const float margin = viewport.rect.x;
    bar(g_aspectBarPanels[0], "Aspect Bar Left", nk_rect(-10.0f, -10.0f, alpha * margin + 10.0f, 1090.0f));
    bar(g_aspectBarPanels[1], "Aspect Bar Right", nk_rect(frameWidth - margin * alpha - 1.0f, -10.0f, margin + 10.0f, 1090.0f));
    if (reveal) {
        context.style.window.fixed_background.data.color = nk_rgba(0, 0, 0, 225);
        bar(g_aspectBarPanels[0], "Aspect Bar Left - Reveal", nk_rect(alpha * margin - 5.0f, -10.0f, 5.0f, 1090.0f));
        bar(g_aspectBarPanels[1], "Aspect Bar Right - Reveal", nk_rect(frameWidth - alpha * margin - 1.0f, -10.0f, 5.0f, 1090.0f));
    }
    const UIViewport whole{.rect = {.x = 0.0f, .y = 0.0f, .z = buffer, .w = 1080.0f}, .depthRange = {.x = 0.0f, .y = 1.0f}};
#if SHENMUE_IS_S1
    d3t::D3TRenderState &state = d3t::GetRenderState(*g_RenderDeviceContext);
#else
    d3t::D3TRenderState &state = d3t::GetRenderState(*g_nuklearRenderDeviceContext);
#endif
    if (std::memcmp(state.viewport.data(), &whole, sizeof(whole)) != 0) {
        state.viewportDirtyMask |= 1U;
        std::memcpy(state.viewport.data(), &whole, sizeof(whole));
    }
    Nuklear_RenderPass(&whole);
}

#if SHENMUE_IS_S2
void NuklearDrawAspectBars(float alpha, std::int32_t mode, std::int32_t reveal) {
    if (!g_nuklearRenderDeviceContext || !g_NuklearInitialized)
        return;
    const UIViewport viewport = AdjustScreenForAspectRatio(mode);
    if (mode != 0)
        DrawAspectBars(alpha, viewport, reveal);
}
#endif








void Nuklear_RenderPass(const UIViewport *viewport) {
#if defined(_WIN32)
#if SHENMUE_IS_S1
    D3TDeviceContext *const context = g_RenderDeviceContext;
#else
    d3t::D3TDeviceContext *const context = g_nuklearRenderDeviceContext;
#endif
    const std::int32_t mapMode = g_Nuklear_num_verts != 0 ? 2 : 1;
    if (!g_nuklearPixelShader)
        return;
    nk_buffer vertices;
    nk_buffer elements;
    nk_buffer_init_fixed(&vertices, g_nuklearVertexStaging.data(), g_nuklearVertexStaging.size());
    nk_buffer_init_fixed(&elements, g_nuklearIndexStaging.data(), g_nuklearIndexStaging.size());
    const nk_convert_config config{.global_alpha = 1.0f,
        .line_AA = NK_ANTI_ALIASING_ON,
        .shape_AA = NK_ANTI_ALIASING_ON,
        .circle_segment_count = 22,
        .arc_segment_count = 22,
        .curve_segment_count = 22,
        .null = g_nuklearNullTexture,
        .vertex_layout = kNuklearVertexLayout.data(),
        .vertex_size = 20,
        .vertex_alignment = 4};
    nk_convert(&GNuklearWindow, &g_nuklearCommands, &vertices, &elements, &config);
    if (vertices.allocated) {
        void *const vertexData = d3t::GetWriteMappedBufferPointer(context, g_nuklearVertexBuffer, mapMode, 20 * g_Nuklear_num_verts);
        void *const indexData = d3t::GetWriteMappedBufferPointer(context, g_nuklearIndexBuffer, mapMode, 2 * g_Nuklear_num_indices);
        std::memcpy(vertexData, g_nuklearVertexStaging.data(), vertices.allocated);
        std::memcpy(indexData, g_nuklearIndexStaging.data(), elements.allocated);
        d3t::DX11_CopySceneNodeToGPU(context, g_nuklearIndexBuffer);
        d3t::DX11_CopySceneNodeToGPU(context, g_nuklearVertexBuffer);
    }
    if (g_bOrthographicRender) {
        if (Matrix4x4 *const matrix = static_cast<Matrix4x4 *>(d3t::MapTempConstantBufferForWriteDiscard(context, 1, 0, 64)))
            CreateOrthographicViewMatrix(static_cast<std::int32_t>(viewport->rect.z), static_cast<std::int32_t>(viewport->rect.w), matrix);
    } else if (Matrix4x4 *const matrix = static_cast<Matrix4x4 *>(d3t::MapTempConstantBufferForWriteDiscard(context, 1, 0, 64))) {
        *matrix = g_notebookPageMatrix;
    }
    if (float *const reciprocal = static_cast<float *>(d3t::MapTempConstantBufferForWriteDiscard(context, 2, 1, 8))) {
        if (!(g_nuklearArmedImageSize[0] > 0.0f) || !(g_nuklearArmedImageSize[1] > 0.0f)) {
            reciprocal[0] = 0.0f;
            reciprocal[1] = 0.0f;
        } else {
            reciprocal[0] = 1.0f / g_nuklearArmedImageSize[0];
            reciprocal[1] = 1.0f / g_nuklearArmedImageSize[1];
        }
    }

    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    std::byte *const bytes = context->renderState.data();
    constexpr std::uint64_t kNuklearBlend = 0xf0138293U;
    if (ReadState<std::uint32_t>(state.blendState.data(), 0) != static_cast<std::uint32_t>(kNuklearBlend))
        state.pipelineStateFlags |= d3t::PipelineBlend;
    WriteState(state.blendState.data(), 0, kNuklearBlend);
    if (ReadState<std::uint64_t>(state.depthStencilState.data(), 0) != 0U || state.stencilReference != 0U) {
        state.pipelineStateFlags |= d3t::PipelineDepthStencil;
        state.depthStencilState = {};
        state.stencilReference = 0;
    }
    constexpr std::size_t kPixelSamplerOffset = 0x534;
    if (ReadState<std::uint64_t>(bytes, kPixelSamplerOffset) != 0x295U) {
        state.samplerDirtyMasks[1] |= 1U;
        WriteState(bytes, kPixelSamplerOffset, std::array<std::uint64_t, 2>{0x295U, 0U});
    }
    if (state.inputLayout != g_nuklearInputLayout) {
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
        state.inputLayout = g_nuklearInputLayout;
    }
    if (state.indexBufferHandle != static_cast<std::uint32_t>(g_nuklearIndexBuffer))
        state.pipelineStateFlags |= d3t::PipelineIndexBuffer;
    state.indexBufferHandle = static_cast<std::uint32_t>(g_nuklearIndexBuffer);
    state.indexFormat = 11;
    if (context->resourceCache[0]) {
        static_cast<ID3D11DeviceContext *>(context->deviceContext)->Unmap(reinterpret_cast<ID3D11Resource *>(context->resourceCache[0]), 0);
        context->resourceCache[0] = 0;
    }
    if (state.vertexBufferHandles[0] != static_cast<std::uint32_t>(g_nuklearVertexBuffer))
        state.vertexBufferDirtyMask |= 1U;
    state.vertexBufferHandles[0] = static_cast<std::uint32_t>(g_nuklearVertexBuffer);
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleList)) {
        state.bufferStateFlags |= d3t::BufferTopology;
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleList);
    }
    if (state.vertexShader != g_nuklearVertexShader) {
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = 0xffffffffU;
    }
    state.vertexShader = g_nuklearVertexShader;
    if (state.pixelShader != g_nuklearPixelShader)
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    state.pixelShader = g_nuklearPixelShader;

    const UIViewport current = viewport ? *viewport :
                                          UIViewport{.rect = {.x = state.viewport[0], .y = state.viewport[1], .z = state.viewport[2], .w = state.viewport[3]},
                                              .depthRange = {.x = state.viewport[4], .y = state.viewport[5]}};
    g_currentUIViewportRect = current.rect;
    g_currentUIViewportDepth = current.depthRange;
    std::uint32_t startIndex = g_Nuklear_num_indices;
    for (const nk_draw_command *command = nk__draw_begin(&GNuklearWindow, &g_nuklearCommands); command;
         command = nk__draw_next(command, &g_nuklearCommands, &GNuklearWindow)) {
        if (!command->elem_count)
            continue;
        const std::uint32_t texture = static_cast<std::uint32_t>(command->texture.id);
#if SHENMUE_IS_S1
        const std::uint32_t blockTexture = command->block_handle.id != -1 ? static_cast<std::uint32_t>(command->block_handle.id) : d3t::g_defaultWhiteTexture;
#else
        const std::uint32_t blockTexture =
            command->block_handle.id != -1 ? static_cast<std::uint32_t>(command->block_handle.id) : static_cast<std::uint32_t>(g_uiDefaultImage.handle.id);
#endif
        const struct nk_rect &clip = command->clip_rect;
        if (clip.x == -8192.0f && clip.y == -8192.0f && clip.w == 16384.0f && clip.h == 16384.0f) {
            if (state.rasterizerState != 1U) {
                state.pipelineStateFlags |= d3t::PipelineRasterizer;
                state.rasterizerState = 1;
            }
            constexpr std::array<std::int32_t, 4> kFullScissor{0, 0, 0x4000, 0x4000};
            if (state.scissorRect != kFullScissor) {
                state.bufferStateFlags |= 4U;
                state.scissorRect = kFullScissor;
            }
        } else {
#if SHENMUE_IS_S1
            const std::array<std::int32_t, 4> scissor{static_cast<std::int32_t>(clip.x) + static_cast<std::int32_t>(current.rect.x),
                static_cast<std::int32_t>(clip.y), static_cast<std::int32_t>(clip.x + current.rect.x + clip.w), static_cast<std::int32_t>(clip.y + clip.h)};
#else
            const std::array<std::int32_t, 4> scissor{static_cast<std::int32_t>(clip.x + current.rect.x), static_cast<std::int32_t>(clip.y),
                static_cast<std::int32_t>(clip.x + current.rect.x + clip.w), static_cast<std::int32_t>(clip.y + clip.h)};
#endif
            if (state.rasterizerState != 5U) {
                state.pipelineStateFlags |= d3t::PipelineRasterizer;
                state.rasterizerState = 5;
            }
            if (state.scissorRect != scissor) {
                state.bufferStateFlags |= 4U;
                state.scissorRect = scissor;
            }
        }
        std::uint32_t width = 0;
        std::uint32_t height = 0;
#if SHENMUE_IS_S2
        if (GetTexture2DDimensions(texture, &width, &height)) {
#endif
            constexpr std::size_t kPixelTextureOffset = 0x1d4;
            constexpr std::size_t kPixelViewFlagsOffset = 0x354;
            const std::array<std::uint32_t, 2> textures{texture, blockTexture};
            for (std::size_t slot = 0; slot < textures.size(); ++slot) {
                const std::size_t offset = slot * sizeof(std::uint32_t);
                const std::size_t textureOffset = kPixelTextureOffset + offset;
                const std::size_t viewFlagsOffset = kPixelViewFlagsOffset + offset;
                if (ReadState<std::uint32_t>(bytes, textureOffset) != textures[slot] || ReadState<std::uint32_t>(bytes, viewFlagsOffset) != 0U) {
                    state.shaderResourceDirtyMasks[1] |= 0x100U << slot;
                    WriteState(bytes, textureOffset, textures[slot]);
                    WriteState(bytes, viewFlagsOffset, std::uint32_t{0});
                }
            }
#if SHENMUE_IS_S1
            if (GetTexture2DDimensions(texture, &width, &height)) {
#endif
                const std::uint32_t baseVertex = g_Nuklear_num_verts;
                d3t::DX11_ResetMappedResources(context);
                d3t::DX11_UnmapAndSyncShaderBindings(context);
                d3t::DX11_SetupShadersForDXAndSetViewports(context);
                static_cast<ID3D11DeviceContext *>(context->deviceContext)->DrawIndexed(command->elem_count, startIndex, static_cast<INT>(baseVertex));
            }
            startIndex += command->elem_count;
        }
        g_Nuklear_num_indices += static_cast<std::uint32_t>(elements.allocated >> 1);
        g_Nuklear_num_verts += static_cast<std::uint32_t>(vertices.allocated / 20);
        nk_clear(&GNuklearWindow);
        nk_clear(&GNuklearWindow);
        g_bOrthographicRender = true;
        if (state.rasterizerState != 17U) {
            state.pipelineStateFlags |= d3t::PipelineRasterizer;
            state.rasterizerState = 17;
        }
#else
    static_cast<void>(viewport);
#endif
    }

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
