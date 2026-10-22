#pragma once

#include "d3t/buffer.hpp"

#include <cstdint>

#include <cstddef>
#include <vector>

#if defined(_WIN32)
struct ID3D11DeviceContext;
#endif

namespace shenmue::d3t {

struct DisplayModeInfo {
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t refreshRateNumerator;
    std::uint32_t refreshRateDenominator;
    char description[16];
};

static_assert(sizeof(DisplayModeInfo) == 0x20);

extern std::uint32_t g_originalScreenHeight;
extern std::uint32_t g_originalScreenWidth;
extern std::uint32_t g_defaultTextureFormat;
std::uint64_t GetNumRenderedFrames() noexcept;

#if defined(_WIN32)
extern ID3D11DeviceContext *g_immediateContext;

struct FramePixelCapture {
    std::uint32_t width{};
    std::uint32_t height{};
    std::uint32_t rowPitch{};
    std::uint32_t format{};
    std::vector<std::byte> pixels;
};

bool BeginOcclusionQuery(std::uint32_t index);
bool EndOcclusionQuery(std::uint32_t index);
std::uint64_t ReadOcclusionQuerySamples(std::uint32_t index);
void CreateUserDefinedAnnotations();
void CreateConstantBuffers();
void CreateVertexBuffers();
void CreateNewSRVTexture();
void DX11_D3D11CreateDeviceAndSwapChain(const DisplayModeInfo *config, void *windowHandle);
void *GetNativeSwapChain();
void ShutdownDX11();
D3TDeviceContext *DXGetPipelineContext(std::int32_t mode);
namespace DX11 {
void WaitForCommandListFinish(D3TDeviceContext *context);
}
std::int64_t DX11_ResetMappedResources(D3TDeviceContext *context);
void DX11_UnmapAndSyncShaderBindings(D3TDeviceContext *context);
void UnmapAndClearViews(D3TDeviceContext *context, std::uint32_t texture2D, std::int64_t unused, const float *clearValue);
void *MapTempConstantBufferForWriteDiscard(D3TDeviceContext *context, std::int32_t stageMask, std::uint32_t registerIndex, std::int32_t bufferSize);
void *GetMappedConstantBuffer(D3TDeviceContext *context, std::int64_t unused, std::uint32_t size);
std::int64_t DX11_SetupShadersForDXAndSetViewports(D3TDeviceContext *context);
void SetupPipelineSampleMask(D3TDeviceContext *context, std::uint8_t opacity);
std::uint32_t CreateNewTexture2D();
void DirectXUpdateWindowDimensions(std::uint32_t width, std::uint32_t height);
void PresentFrame(std::int32_t renderMode);
void DumpFramePng(const char *filename);
bool WriteCapturedFrameJpeg(const char *filename, const FramePixelCapture &capture);
#endif

} // namespace shenmue::d3t
