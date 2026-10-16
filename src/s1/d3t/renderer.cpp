#include "s1/d3t/renderer.hpp"

#include "s1/d3t/d3t_texture.hpp"

#include "s1/window.hpp"

#if defined(_WIN32)
#include <d3d11.h>
#include <dxgi.h>
#endif

namespace shenmue::s1 {

std::int64_t CopyOrResolveTexture(D3TDeviceContext *context, std::uint32_t sourceTexture, std::uint32_t destinationTexture) {
#if defined(_WIN32)
    if (!context || !context->deviceContext)
        return 0;

    IncrementTextureById(sourceTexture);
    IncrementTextureById(destinationTexture);

    D3TTextureBackendView source{};
    D3TTextureBackendView destination{};
    Texture2DDescription sourceDescription{};
    Texture2DDescription destinationDescription{};
    const bool resourcesAvailable = GetTexture2DBackendView(sourceTexture, &source) && GetTexture2DBackendView(destinationTexture, &destination) &&
        GetTexture2DDescription(sourceTexture, &sourceDescription) && GetTexture2DDescription(destinationTexture, &destinationDescription) && source.texture &&
        destination.texture;
    if (resourcesAvailable) {
        auto *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
        auto *const sourceResource = static_cast<ID3D11Texture2D *>(source.texture);
        auto *const destinationResource = static_cast<ID3D11Texture2D *>(destination.texture);
        if (sourceDescription.sampleCount == destinationDescription.sampleCount) {
            native->CopyResource(destinationResource, sourceResource);
        } else if (destinationDescription.sampleCount == 1) {
            native->ResolveSubresource(
                destinationResource, 0, sourceResource, 0, static_cast<DXGI_FORMAT>(GetD3TNativeTextureFormat(destinationDescription.format)));
        }
    }

    InitTexture2DResource(destinationTexture);
    InitTexture2DResource(sourceTexture);
#else
    static_cast<void>(context);
    static_cast<void>(sourceTexture);
    static_cast<void>(destinationTexture);
#endif
    return 0;
}

#if defined(_WIN32)
void DX11_D3D11CreateDeviceAndSwapChain(const DisplayModeInfo *config) {
    d3t::DX11_D3D11CreateDeviceAndSwapChain(config, g_windowHandle);
    g_swapChain = static_cast<IDXGISwapChain *>(d3t::GetNativeSwapChain());
}

void ShutdownDX11() {
    d3t::ShutdownDX11();
    g_swapChain = nullptr;
}
#endif

} // namespace shenmue::s1
