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
void ConfigureAndSubmitModelScenePass(d3t::D3TDeviceContext *context, ModelScenePass &pass, ModelGeometryContext &geometry);

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
