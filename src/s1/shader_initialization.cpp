#include "s1/shader_initialization.hpp"
#include "s1/d3t/namespace.hpp"

#include "s1/d3t/d3t_content.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/fvf_layout.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/nuklear_integration.hpp"
#include "s1/simple_shader.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#endif

namespace shenmue::s1 {

void SetupNuklear();
std::uint32_t g_activeRenderHandle = 0;

namespace {

template <typename T> T ReadRenderState(const std::byte *state, std::size_t offset) {
    T value{};
    std::memcpy(&value, state + offset, sizeof(value));
    return value;
}

template <typename T> void WriteRenderState(std::byte *state, std::size_t offset, T value) {
    std::memcpy(state + offset, &value, sizeof(value));
}

} // namespace

namespace d3t {

std::int64_t SetupForDepthBufferPass(D3TDeviceContext *renderer) {
    if (!renderer)
        return 20;
    std::byte *const state = renderer->renderState.data();

    std::uint32_t fullScreenWidth = 0;
    std::uint32_t fullScreenHeight = 0;
    if (GetTexture2DDimensions(g_fullScreenTexture2D, &fullScreenWidth, &fullScreenHeight)) {
        MapForSuperSampling(renderer, static_cast<float>(fullScreenWidth));
    }
    SetupPPFX(renderer);

    constexpr std::uint64_t sampler = 0x280U;
    if (ReadRenderState<std::uint64_t>(state, 0x534) != sampler) {
        auto dirty = ReadRenderState<std::uint16_t>(state, 0x966);
        dirty |= 1U;
        WriteRenderState(state, 0x966, dirty);
        WriteRenderState(state, 0x534, sampler);
        WriteRenderState(state, 0x53c, std::uint64_t{0});
    }
    if (ReadRenderState<std::uint32_t>(state, 0x954) != 1U) {
        state[0x975] |= std::byte{0x20};
        WriteRenderState(state, 0x954, std::uint32_t{1});
    }
    if (ReadRenderState<std::uint32_t>(state, 0x794) != 0xf0000000U) {
        state[0x975] |= std::byte{8};
    }
    WriteRenderState(state, 0x794, std::uint64_t{0xf0000000ULL});

    if (ReadRenderState<std::uint64_t>(state, 0x940) != 20U || ReadRenderState<std::uint32_t>(state, 0x950) != 0U) {
        state[0x975] |= std::byte{0x10};
        WriteRenderState(state, 0x940, std::uint64_t{20});
        WriteRenderState(state, 0x948, std::uint64_t{0});
        WriteRenderState(state, 0x950, std::uint32_t{0});
    }
    if (ReadRenderState<std::uint32_t>(state, 0x938) != 0U) {
        state[0x975] |= std::byte{0x80};
        WriteRenderState(state, 0x938, std::uint32_t{0});
    }
    if (ReadRenderState<std::uint32_t>(state, 0x93c) != 1U) {
        state[0x976] |= std::byte{1};
        WriteRenderState(state, 0x93c, std::uint32_t{1});
    }
    return 20;
}
} // namespace d3t


std::int64_t ClearEffectABuffer() {
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (!context)
        return 0;
    std::byte *const state = context->renderState.data();
    if (ReadRenderState<std::uint32_t>(state, 0) != g_abufferClearVertexShader) {
        state[0x975] |= std::byte{0x81};
        WriteRenderState(state, 0x970, ~std::uint32_t{0});
    }
    WriteRenderState(state, 0, g_abufferClearVertexShader);
    if (ReadRenderState<std::uint32_t>(state, 4) != g_abufferClearPixelShader) {
        state[0x975] |= std::byte{2};
    }
    WriteRenderState(state, 4, g_abufferClearPixelShader);
    d3t::SetupForDepthBufferPass(context);
#if defined(_WIN32)
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    auto *native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    if (native)
        native->Draw(4, 0);
#endif
    return 0;
}


std::int64_t ComposeEffectABuffer() {
    D3TDeviceContext *const context = DX11::g_ppD3DDeviceContext;
    if (!context)
        return 0;

    std::byte *const state = context->renderState.data();

    CopyOrResolveTexture(context, g_fullScreenTexture2D, g_abufferCompositionTexture2D);

    if (ReadRenderState<std::uint32_t>(state, 0) != g_abufferCompositionVertexShader) {
        state[0x975] |= std::byte{0x81};
        WriteRenderState(state, 0x970, ~std::uint32_t{0});
    }
    WriteRenderState(state, 0, g_abufferCompositionVertexShader);
    if (ReadRenderState<std::uint32_t>(state, 4) != g_abufferCompositionPixelShader) {
        state[0x975] |= std::byte{2};
    }
    WriteRenderState(state, 4, g_abufferCompositionPixelShader);

    constexpr std::size_t kPixelTexture8 = 0x1d4;
    constexpr std::size_t kPixelViewFlags8 = 0x354;
    if (ReadRenderState<std::uint32_t>(state, kPixelTexture8) != g_abufferCompositionTexture2D ||
        ReadRenderState<std::uint32_t>(state, kPixelViewFlags8) != 0U) {
        auto dirty = ReadRenderState<std::uint32_t>(state, 0x95c);
        dirty |= 1U << 8U;
        WriteRenderState(state, 0x95c, dirty);
        WriteRenderState(state, kPixelTexture8, g_abufferCompositionTexture2D);
        WriteRenderState(state, kPixelViewFlags8, std::uint32_t{0});
    }
    d3t::SetupForDepthBufferPass(context);
#if defined(_WIN32)
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
    auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
    if (native)
        native->Draw(4, 0);
#endif
    return 0;
}


std::int32_t g_bHasSetupShaders = 0;

std::int64_t LoadShaders() {
    g_activeRenderHandle = d3t_AllocateRenderHandle();

    CreateFVFLayouts();
    d3t::LoadOriginalStripShaders();
    d3t::LoadShaders();
    d3t::InitNuklearTexture2D();
    SDTextureOverride();

    while (d3t_IsRenderHandlePending(g_activeRenderHandle)) {
        ProcessRemappings();
    }
    d3t_CloseRenderHandle(g_activeRenderHandle);

    LoadD3TGenericShaders();
    Shenmue_InitShaders();
    SetupNuklear();
    LoadSimpleShader();

    AssignShaderReferences();
    AssignShaderReferences_0();
    g_bHasSetupShaders = 1;
    return ClearEffectABuffer();
}

} // namespace shenmue::s1
