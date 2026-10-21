#include "s2/areas/game_arcade_render.hpp"

#include "s2/d3t_renderer.hpp"
#include "s2/input_bindings.hpp"
#include "s2/renderer_initialization.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_renderer.hpp"
#include "common/math.hpp"

#include <array>
#include <cstring>
#if defined(_WIN32)
#include <d3d11.h>
#endif

namespace shenmue::s2 {
namespace {

void SetTopology(d3t::D3TRenderState &state, d3t::TopologyType topology) {
    const std::uint32_t value = static_cast<std::uint32_t>(topology);
    if (state.topology != value) {
        state.bufferStateFlags |= d3t::BufferTopology;
        state.topology = value;
    }
}



void BindQuadState(d3t::D3TDeviceContext &context, bool gamma, bool point) {
    d3t::D3TRenderState &state = d3t::GetRenderState(context);
    const std::uint32_t vertexShader = gamma ? texturedQuadGammaVertexShader : texturedQuadVertexShader;
    const std::uint32_t pixelShader = gamma ? texturedQuadGammaPixelShader : texturedQuadPixelShader;
    if (state.vertexShader != vertexShader) {
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
        state.vertexShader = vertexShader;
    }
    if (state.pixelShader != pixelShader) {
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
        state.pixelShader = pixelShader;
    }
    const std::uint32_t layout = g_fvfLayouts[gamma ? 35 : 34];
    if (state.inputLayout != layout) {
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
        state.inputLayout = layout;
    }
    constexpr std::uint64_t blend = 0xf0000000ULL;
    std::uint64_t previousBlend;
    std::memcpy(&previousBlend, state.blendState.data(), sizeof(previousBlend));
    if (previousBlend != blend) {
        state.pipelineStateFlags |= d3t::PipelineBlend;
        std::memcpy(state.blendState.data(), &blend, sizeof(blend));
    }
    constexpr std::array<std::byte, 0x10> depth{};
    if (state.depthStencilState != depth || state.stencilReference != 0) {
        state.pipelineStateFlags |= d3t::PipelineDepthStencil;
        state.depthStencilState = depth;
        state.stencilReference = 0;
    }
    if (state.rasterizerState != 1) {
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
        state.rasterizerState = 1;
    }
    const std::array<std::uint64_t, 2> sampler{point ? 0ULL : 5ULL, 0};
    if (std::memcmp(context.renderState.data() + 0x534, sampler.data(), sizeof(sampler)) != 0) {
        state.samplerDirtyMasks[1] |= 1;
        std::memcpy(context.renderState.data() + 0x534, sampler.data(), sizeof(sampler));
    }
}

void BindTexture(d3t::D3TDeviceContext &context, std::uint32_t texture) {
    d3t::D3TRenderState &state = d3t::GetRenderState(context);
    std::uint32_t previousTexture, previousFlags;
    std::memcpy(&previousTexture, context.renderState.data() + 0x1d4, sizeof(previousTexture));
    std::memcpy(&previousFlags, context.renderState.data() + 0x354, sizeof(previousFlags));
    if (previousTexture != texture || previousFlags != 0) {
        state.shaderResourceDirtyMasks[1] |= 0x100;
        std::memcpy(context.renderState.data() + 0x1d4, &texture, sizeof(texture));
        constexpr std::uint32_t flags = 0;
        std::memcpy(context.renderState.data() + 0x354, &flags, sizeof(flags));
    }
}

void DrawQuad(d3t::D3TDeviceContext &context, std::uint32_t texture, const std::array<Vector4, 4> &vertices) {
#if defined(_WIN32)


    if (void *const mapped = d3t::GetMappedConstantBuffer(&context, 0, sizeof(vertices)))
        std::memcpy(mapped, vertices.data(), sizeof(vertices));
    d3t::D3TRenderState &state = d3t::GetRenderState(context);
    SetTopology(state, d3t::TopologyType::TriangleStrip);
    BindTexture(context, texture);
    d3t::DX11_ResetMappedResources(&context);
    d3t::DX11_UnmapAndSyncShaderBindings(&context);
    d3t::DX11_SetupShadersForDXAndSetViewports(&context);
    static_cast<ID3D11DeviceContext *>(context.deviceContext)->Draw(4, 0);
    SetTopology(state, d3t::TopologyType::TriangleList);
#else
    static_cast<void>(context);
    static_cast<void>(texture);
    static_cast<void>(vertices);
#endif
}

} // namespace

void BeginGameArcadeFrame() {



    BeginD3TRenderPass(true);
    d3t::D3TRenderState &state = d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext);
    SetTopology(state, d3t::TopologyType::TriangleList);
}

void BindGameArcadeTarget(std::uint32_t texture) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*d3t::DX11::g_ppD3DDeviceContext);
    state.outputTargets.fill(std::byte{});
    std::memcpy(state.outputTargets.data(), &texture, sizeof(texture));
    state.bufferStateFlags |= d3t::BufferOutputTargets;
    const std::array<float, 6> viewport{0, 0, 640, 480, 0, 1};
    if (state.viewport != viewport) {
        state.viewport = viewport;
        state.viewportDirtyMask |= 1;
    }
}

void DrawGameHangOnFrame(std::uint32_t texture) {

    d3t::BindSceneFrameRenderTarget();
    d3t::D3TDeviceContext &context = *d3t::DX11::g_ppD3DDeviceContext;
    BindQuadState(context, false, true);
    const std::int32_t width = static_cast<std::int32_t>(d3t::g_originalScreenWidth);
    const std::int32_t height = static_cast<std::int32_t>(d3t::g_originalScreenHeight);
    const std::int32_t scale = height / 224 >= width / 320 ? width / 320 : height / 224;
    const float y = static_cast<float>(scale * 224) / static_cast<float>(height);
    const float resolutionScale = GetRuntimeUserConfiguration().resolutionScale * .60000002f + .40000001f;
    const float scaledWidth =
        GetAspectRatioModeShouldScale() != 0 ? resolutionScale * static_cast<float>(height) * 1.33333f : resolutionScale * static_cast<float>(width);
    const float aspectRatio = static_cast<float>(static_cast<std::int64_t>(scaledWidth)) / static_cast<float>(height);
    const float x = y / (aspectRatio / 1.4285715f);
    const std::array<Vector4, 4> vertices{{
        {.x = -x, .y = -y, .z = .1875f, .w = .9375f},
        {.x = x, .y = -y, .z = .8125f, .w = .9375f},
        {.x = -x, .y = y, .z = .1875f, .w = .0625f},
        {.x = x, .y = y, .z = .8125f, .w = .0625f},
    }};
    DrawQuad(context, texture, vertices);
}

} // namespace shenmue::s2
