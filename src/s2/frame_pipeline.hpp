#pragma once

#include "common/math.hpp"
#include <cstddef>

namespace shenmue::d3t {
struct D3TDeviceContext;
}

namespace shenmue::s2 {
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
