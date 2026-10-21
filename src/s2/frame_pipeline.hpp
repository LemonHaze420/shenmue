#pragma once

#include "common/math.hpp"
#include <cstddef>

namespace shenmue::d3t {
struct D3TDeviceContext;
}

namespace shenmue::s2 {
struct ModelGeometryContext;
struct ModelScenePass;
struct ModelDebugShaderConstants {
    std::int32_t renderMode;
    std::uint32_t visualizeCascade;
    std::uint32_t environmentProbesEnabled;
    std::uint32_t probeVisualizationIndex;
    std::int32_t specularBrdfMode;
    std::uint32_t probeVisualizationMode;
    std::uint32_t useDualQuaternion;
    std::uint32_t visualizeClusters;
    float clampDirectRoughness;
};
struct ModelDepthConstants {
    float nearFarProduct;
    float farMinusNear;
    float farPlane;
    float parameterScale;
    float parameterBias;
};
static_assert(sizeof(ModelDebugShaderConstants) == 36);
static_assert(offsetof(ModelDebugShaderConstants, useDualQuaternion) == 24);
static_assert(sizeof(ModelDepthConstants) == 20);
extern std::int32_t g_modelDebugRenderMode;
extern std::uint8_t g_modelVisualizeCascade;
extern std::int32_t g_modelSpecularBrdfMode;
extern std::uint8_t g_modelUseDualQuaternion;
extern std::uint8_t g_modelVisualizeClusters;
extern float g_modelClampDirectRoughness;
void SetDebugShaderParams(d3t::D3TDeviceContext *context);
void GetPostFXScaledRenderTargetSize(float *width, float *height);
void SetupPostFXRenderTargetsAndViewport(d3t::D3TDeviceContext *context);
void ConfigureAndSubmitModelScenePass(d3t::D3TDeviceContext *context, ModelScenePass &pass, ModelGeometryContext &geometry);

struct FrameConstantBlock {
    Vector4 fogRange;
    Vector4 fogColour;
    Vector2 backBufferSize;
    Vector2 windVelocity;
    Vector3 sunPosition;
    float storminess;
    float nightTimeyness;
    float time;
    float windTime;
    float sunsetSize;
    float wetness;
    float wetRoughnessDrop;
    float wetNormalDrop;
    float wetAlbedoDarken;
    Vector4 wetMaskUVScaleBias;
    Vector2 rainShadowMapScale;
    Vector2 rainShadowMapOffset;
    Vector2 rainShadowMapUVMin;
    Vector2 rainShadowMapUVMax;
    std::int32_t rainShadowViewProjectionIndex;
};
static_assert(sizeof(FrameConstantBlock) == 148);
static_assert(offsetof(FrameConstantBlock, time) == 68);
static_assert(offsetof(FrameConstantBlock, rainShadowViewProjectionIndex) == 144);
extern std::uint8_t g_modelSceneSuppressed;
extern float g_currentFractionOfDay;
extern float g_sceneTimeOscillation;
extern float g_sceneTimeDirection;
extern float g_timeSinceStart;
void UpdateFractionOfDay(float fractionOfDay, std::int64_t unused, float frameTime);
void IncrementTimeSinceStart(float frameTime);
void SD_Override();

void DrawSceneTexture(d3t::D3TDeviceContext *context, std::uint32_t source, std::uint32_t destination);
void DrawSDTransparent();
enum SimplePixelShaderFlag : std::uint8_t { SPSF_Gamma = 1, SPSF_PreserveAspect = 2 };
extern std::uint32_t g_postEffectSceneTexture;
void SetBufferAndRectRenderFlags(d3t::D3TDeviceContext *context);
d3t::D3TDeviceContext *RenderSimplePixelShaderPass(const std::uint32_t *texture, std::uint8_t flags);
void DoSimplePixelShaderPass();
extern float g_sceneDepthClearValue;
extern std::uint32_t g_sceneStencilClearValue;
void RenderPipeline(const char *name, std::int32_t keepTargets);
struct CameraConstantBlock {
    Matrix4x4 view;
    Matrix4x4 projection;
    Matrix4x4 inverseProjection;
    Matrix4x4 viewProjection;
    Matrix4x4 inverseView;
    Matrix4x4 previousProjection;
    Matrix4x4 postEffectProjection;
    Matrix4x4 reserved1c0;
    Vector4 projectedOrigin;
};
static_assert(sizeof(CameraConstantBlock) == 528);
static_assert(offsetof(CameraConstantBlock, inverseProjection) == 128);
static_assert(offsetof(CameraConstantBlock, inverseView) == 256);
static_assert(offsetof(CameraConstantBlock, previousProjection) == 320);
static_assert(offsetof(CameraConstantBlock, reserved1c0) == 448);
static_assert(offsetof(CameraConstantBlock, projectedOrigin) == 512);
extern Matrix4x4 g_previousCameraProjection;
extern Matrix4x4 g_postEffectProjection;
void CreateCameraMatrices(CameraConstantBlock &block);
void SetSceneSharedState(d3t::D3TDeviceContext *context);
} // namespace shenmue::s2
