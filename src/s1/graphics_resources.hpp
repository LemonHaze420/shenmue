#pragma once

#include "s1/d3t/namespace.hpp"
#include "d3t/buffer.hpp"
#include <d3t/texture.hpp>

#include <cstdint>

#include "s1/matrix_stack.hpp"

namespace shenmue::s1 {

using D3TDeviceContext = d3t::D3TDeviceContext;

struct TextureResourceDescriptor {
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t format;
    std::uint32_t reserved;
    std::uint64_t texture;
};

static_assert(sizeof(TextureResourceDescriptor) == 0x18);

extern TextureResourceDescriptor g_generalTextureResources[3];
extern std::uint32_t g_fullScreenTexture2D;
extern std::uint32_t g_abufferCompositionTexture2D;
extern std::uint32_t g_originalStripsTexture2D;
extern std::uint32_t g_auxiliaryTexture2D;
extern std::uint32_t g_superSamplingTexture2D;
extern std::uint32_t g_legacyFullScreenTexture2D;
extern std::uint32_t g_fullScreenUintArrayTexture2D;
extern std::uint32_t g_fullScreenFloatArrayTexture2D;
extern std::uint32_t g_fullScreenUintTexture2D;
extern std::uint32_t g_instanceBuffer;
extern std::uint32_t g_billboardBuffer;
extern std::uint32_t g_perObjectBuffer;
extern std::uint32_t g_dedicatedShadowBuffer;

struct ScreenWindow {
    std::uint32_t texture2D[2];
    std::uint32_t resolutionOffset[2];
    std::uint32_t resolution[2];
    std::uint32_t globalResolution[2];
};

static_assert(sizeof(ScreenWindow) == 0x20);

extern ScreenWindow g_screenWindow;
extern float g_superSampling;
extern float g_viewportInterpolationProgress;


extern float g_depthPassProgress;
extern float g_renderPassProgress;
extern std::uint8_t g_bufferPassNotBusy;
extern float g_viewportScaleX;
extern float g_viewportScaleY;
extern std::uint8_t g_superSamplingTextureActive;

extern std::uint8_t g_hasSetupBuffersForTextures;

std::int64_t RGBA8_CreateTexture2D();
void SetupGlobalRenderBuffers();
void SetupImmutableDXTex2D();
void InitGeneralTextureResources();
std::int64_t ReleaseRGBA8TextureResources();
std::int64_t ScaleViewport(float width, float height);
namespace d3t {
std::int64_t GetScaledViewportDimensions(float *width, float *height);
} // namespace d3t
void GetAspectRatio_0(D3TDeviceContext *context);
void MapForSuperSampling(D3TDeviceContext *context, float scale);
void SetupPPFX(D3TDeviceContext *context);
void SetupCharacterAlphaDepthTarget(D3TDeviceContext *context);
void SetupCharacterShadowTarget(D3TDeviceContext *context);
std::int64_t SetScreenDimensions(std::uint32_t width, std::uint32_t height);

} // namespace shenmue::s1
