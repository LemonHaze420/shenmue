#include "s2/areas/game/emulator/draw.hpp"

#include "s2/legacy_draw.hpp"
#include "s2/renderer_initialization.hpp"
#include "s1/archive_data.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/texture.hpp"

#include <array>
#include <cstdlib>
#include <cstring>
#if defined(_WIN32)
#include <d3d11.h>
#endif

namespace shenmue::s2 {

// @verified - p
void SetEmulatorDefaultRenderFlags() {
    d3t::D3TDeviceContext &context = *d3t::DX11::g_ppD3DDeviceContext;
    const std::array<std::byte, 16> sampler{};
    if (std::memcmp(context.renderState.data() + 0x574, sampler.data(), sampler.size()) != 0) {
        std::memcpy(context.renderState.data() + 0x574, sampler.data(), sampler.size());
        d3t::GetRenderState(context).samplerDirtyMasks[1] |= 0x10;
    }
    SetAlphaFlags(0, 5, 1);
    SetAlphaFlags(0, 3, 1);
    SetAlphaFlags(0, 4, 1);
    SetEngineFlag(7, 0);
    SetEngineFlag(147, 0);
    SetEngineFlag(59, 1);
    SetEngineFlag(62, 770);
    SetEngineFlag(63, 771);
}

void BindEmulatorTexture(std::uint32_t texture) {
    d3t::D3TDeviceContext &context = *d3t::DX11::g_ppD3DDeviceContext;
    std::uint32_t previous, flags;
    std::memcpy(&previous, context.renderState.data() + 0x1b4, sizeof(previous));
    std::memcpy(&flags, context.renderState.data() + 0x334, sizeof(flags));
    if (previous != texture || flags != 0) {
        d3t::GetRenderState(context).shaderResourceDirtyMasks[1] |= 1;
        std::memcpy(context.renderState.data() + 0x1b4, &texture, sizeof(texture));
        constexpr std::uint32_t zero = 0;
        std::memcpy(context.renderState.data() + 0x334, &zero, sizeof(zero));
    }
}

// @verified - p
void DrawEmulatorVertices(const void *data, std::uint32_t stride, std::uint32_t count, std::uint32_t layout, std::uint32_t pixelShader) {
    d3t::D3TDeviceContext &context = *d3t::DX11::g_ppD3DDeviceContext;
    d3t::D3TRenderState &state = d3t::GetRenderState(context);
    const std::uint32_t vertex = layout == 30 ? g_emulatorPdVertexShader : layout == 31 ? g_emulatorPdtVertexShader : g_emulatorPtVertexShader;
    const std::uint32_t pixel = pixelShader == 14 ? g_emulatorPdPixelShader : pixelShader == 15 ? g_emulatorPdtPixelShader : g_emulatorPtPixelShader;
    if (state.vertexShader != vertex) {
        state.vertexShader = vertex;
        state.pipelineStateFlags |= d3t::PipelineVertexShader | d3t::PipelineInputLayout;
        state.vertexBufferDirtyMask = ~std::uint32_t{0};
    }
    if (state.pixelShader != pixel) {
        state.pixelShader = pixel;
        state.pipelineStateFlags |= d3t::PipelinePixelShader;
    }
    if (state.inputLayout != g_fvfLayouts[layout]) {
        state.inputLayout = g_fvfLayouts[layout];
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
    if (state.topology != static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip)) {
        state.topology = static_cast<std::uint32_t>(d3t::TopologyType::TriangleStrip);
        state.bufferStateFlags |= d3t::BufferTopology;
    }
    const std::uint32_t size = count * stride;
    if (void *const mapped = d3t::GetMappedConstantBuffer(&context, 0, size))
        std::memcpy(mapped, data, size);
    UpdateShaderParams();
#if defined(_WIN32)
    d3t::DX11_ResetMappedResources(&context);
    d3t::DX11_UnmapAndSyncShaderBindings(&context);
    d3t::DX11_SetupShadersForDXAndSetViewports(&context);
    static_cast<ID3D11DeviceContext *>(context.deviceContext)->Draw(count, 0);
#endif
}

void LoadEmulatorFile(const char *path, void *destination, std::int32_t limit) {
    if (limit <= 0 || limit > 0x4000000)
        limit = 0x4000000;
    const std::uint64_t size = s1::ArchiveData_GetFileSize(path);
    const std::uint64_t copied = size < static_cast<std::uint64_t>(limit) ? size : static_cast<std::uint64_t>(limit);
    s1::ArchiveData_Read(path, 0, destination, copied);
}

std::uint32_t LoadEmulatorImage(const char *path) {
    const std::uint64_t size = s1::ArchiveData_GetFileSize(path);
    void *const data = std::malloc(static_cast<std::size_t>(size));
    std::uint32_t texture = 0;
    if (s1::ArchiveData_Read(path, 0, data, size))
        texture = d3t::CreateImageTextureFromMemory(data, static_cast<std::size_t>(size), true);
    std::free(data);
    return texture;
}

} // namespace shenmue::s2
