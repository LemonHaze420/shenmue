#pragma once

#include "d3t/dx11_renderer.hpp"
#include "s1/d3t/settings.hpp"

namespace shenmue::s1 {

using D3TDeviceContext = d3t::D3TDeviceContext;
namespace DX11 = d3t::DX11;

inline std::uint32_t &g_originalScreenHeight = d3t::g_originalScreenHeight;
inline std::uint32_t &g_originalScreenWidth = d3t::g_originalScreenWidth;
inline std::uint32_t &g_defaultTextureFormat = d3t::g_defaultTextureFormat;

std::int64_t CopyOrResolveTexture(D3TDeviceContext *context, std::uint32_t sourceTexture, std::uint32_t destinationTexture);

#if defined(_WIN32)
using d3t::CreateConstantBuffers;
using d3t::CreateNewSRVTexture;
using d3t::CreateNewTexture2D;
using d3t::CreateUserDefinedAnnotations;
using d3t::CreateVertexBuffers;
using d3t::DX11_ResetMappedResources;
using d3t::DX11_SetupShadersForDXAndSetViewports;
using d3t::DX11_UnmapAndSyncShaderBindings;
using d3t::DX11::WaitForCommandListFinish;
using d3t::GetMappedConstantBuffer;
using d3t::MapTempConstantBufferForWriteDiscard;
using d3t::UnmapAndClearViews;

void DX11_D3D11CreateDeviceAndSwapChain(const DisplayModeInfo *config);
void ShutdownDX11();
inline D3TDeviceContext *d3t_DXGetPipelineContext(std::int32_t mode) {
    return d3t::DXGetPipelineContext(mode);
}
inline void d3t_PresentFrame(std::int32_t renderMode) {
    d3t::PresentFrame(renderMode);
}
inline void d3t_DumpFramePng(const char *filename) {
    d3t::DumpFramePng(filename);
}
#endif

} // namespace shenmue::s1
