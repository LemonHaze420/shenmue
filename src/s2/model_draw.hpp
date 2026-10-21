#pragma once

#include "s2/model_geometry.hpp"

namespace shenmue::s2 {
struct ModelGeometryDraw {
    Matrix4x4 transform;
    std::uint32_t objectBuffer;
    std::uint8_t debugOverride;
    std::byte reserved45[3];
    std::array<std::uint32_t, 8> debugValues;
    std::uint32_t debugIndex;
    std::uint32_t reserved6c;
    D3TModelRecord *model;
    std::uint16_t drawGroup;
    std::uint16_t submesh;
    std::uint16_t firstInstance;
    std::uint16_t instanceCount;
    std::uint32_t nodeIndex;
    float opacity;
};
struct ModelPassConstants {
    std::int32_t writeVelocity;
    std::int32_t fogDisabled;
    float clusterNear;
    float clusterFar;
};
struct ModelDebugConstants {
    std::array<std::uint32_t, 8> values;
    std::uint32_t enabled;
    std::uint32_t index;
};
struct ModelBillboardInstance {
    Vector3 position;
    std::uint32_t parameter0c;
    float halfHeight;
    std::byte reserved14[24];
    std::uint32_t parameter2c;
    std::uint32_t reserved30;
};
struct ModelBillboardBounds {
    Vector3 position;
    std::uint32_t reference;
};
struct ModelNearBillboard {
    Vector4 position;
    Vector3 bounds;
    float scale;
    float opacity;
    std::uint32_t parameter24;
};
struct ModelGeometrySamplerSettings {
    std::uint32_t magnification;
    std::uint32_t minification;
    std::uint32_t mip;
    std::int32_t anisotropy;
};
static_assert(sizeof(ModelGeometryDraw) == 136);
static_assert(offsetof(ModelGeometryDraw, debugValues) == 72);
static_assert(offsetof(ModelGeometryDraw, model) == 112);
static_assert(offsetof(ModelGeometryDraw, firstInstance) == 124);
static_assert(offsetof(ModelGeometryDraw, opacity) == 132);
static_assert(sizeof(ModelPassConstants) == 16);
static_assert(sizeof(ModelDebugConstants) == 40);
static_assert(offsetof(ModelDebugConstants, enabled) == 32);
static_assert(sizeof(ModelBillboardInstance) == 52);
static_assert(offsetof(ModelBillboardInstance, halfHeight) == 16);
static_assert(offsetof(ModelBillboardInstance, parameter2c) == 44);
static_assert(sizeof(ModelBillboardBounds) == 16);
static_assert(sizeof(ModelNearBillboard) == 40);
extern ModelGeometrySamplerSettings g_modelGeometrySamplerSettings;
extern bool g_modelAlternateShading;
extern bool g_sortModelGeometry;
extern float g_modelLodScale;
extern float g_modelLodBias;
void CommitModelGeometryDraw(d3t::D3TDeviceContext *context, const ModelGeometryDraw &draw, const ModelScenePass &pass);
void SubmitModelGeometry(ModelGeometryVisibility &visibility, d3t::D3TDeviceContext *context, const ModelScenePass &pass);
} // namespace shenmue::s2
