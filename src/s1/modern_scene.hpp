#pragma once

#include "s1/d3t/d3t_buffer.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue {
struct D3TModelRecord;
}

namespace shenmue::s1 {

struct CachedSkinningBufferSlot;

struct ModernDrawCallbackContext {
    D3TDeviceContext *context;
    Matrix4x4 transform;
};

struct ModernScenePass {
    std::uint64_t reserved;
    std::uint32_t vertexShader;
    std::uint32_t pixelShader;
    std::uint32_t materialMask;
    std::uint32_t type;
    Vector3 cameraPosition;
    float clusterNear;
    float clusterFar;
    float lodBias;
    std::uint8_t disableVelocity;
    std::uint8_t excludeMoving;
    std::uint8_t fogDisabled;
    std::uint8_t deferred;
    std::uint8_t alternateShader;
    std::uint8_t alternateNodes;
    std::uint8_t reverseCulling;
    std::uint8_t disableAspectCulling;
    void (*drawCallback)(ModernDrawCallbackContext *);
};

struct RenderLayerGeometryContext {
    CachedSkinningBufferSlot *skinning;
    Matrix4x4 *transforms;
    std::uint16_t *nodeFlags;
    std::uint32_t *skinOffsets;
    std::uint32_t *debugIndices;
    std::array<std::array<std::uint32_t, 8>, 32> debugRecords;
    std::uint32_t nodeCount;
    std::uint32_t reservedNodeCount;
    std::uint32_t drawCapacity;
    std::uint32_t reservedDrawCapacity;
    Vector4 *waterBounds;
    Matrix4x4 *waterTransform;
    std::uint8_t hasSpecialGeometry;
    std::byte reservedSpecialGeometry[3];
    std::uint32_t submittedDraws;
    std::uint32_t culledDraws;
};

struct RenderLayerVisibility {
    RenderLayerGeometryContext *geometry;
    std::uint32_t *nodeMask;
    std::uint32_t *submeshMask;
    float viewportWidth;
    std::uint32_t reserved;
};

struct ModelGeometryDraw {
    Matrix4x4 transform;
    std::uint32_t objectBuffer;
    std::uint8_t debugOverride;
    std::byte reservedDebug[3];
    std::array<std::uint32_t, 8> debugValues;
    std::uint32_t debugIndex;
    std::uint32_t reserved;
    D3TModelRecord *model;
    std::uint16_t drawGroup;
    std::uint16_t submesh;
    std::uint16_t firstInstance;
    std::uint16_t instanceCount;
    std::uint32_t nodeIndex;
    float opacity;
};

static_assert(sizeof(ModernScenePass) == 64);
static_assert(offsetof(ModernScenePass, type) == 20);
static_assert(offsetof(ModernScenePass, disableVelocity) == 48);
static_assert(offsetof(ModernScenePass, drawCallback) == 56);
static_assert(sizeof(RenderLayerGeometryContext) == 1112);
static_assert(offsetof(RenderLayerGeometryContext, nodeCount) == 1064);
static_assert(offsetof(RenderLayerGeometryContext, drawCapacity) == 1072);
static_assert(offsetof(RenderLayerGeometryContext, waterBounds) == 1080);
static_assert(offsetof(RenderLayerGeometryContext, submittedDraws) == 1100);
static_assert(sizeof(RenderLayerVisibility) == 32);
static_assert(sizeof(ModelGeometryDraw) == 136);
static_assert(offsetof(ModelGeometryDraw, model) == 112);
static_assert(offsetof(ModelGeometryDraw, opacity) == 132);

struct ModernGeometrySamplerSettings {
    std::uint32_t magnification;
    std::uint32_t minification;
    std::uint32_t mip;
    std::int32_t anisotropy;
};

extern ModernGeometrySamplerSettings g_modernGeometrySamplerSettings;
extern bool g_modernAlternateShading;

extern bool g_modernNodeCulling;
extern bool g_modernSubmeshCulling;
extern bool g_modernDebugBounds;
extern bool g_modernSkipShadowNearPlanes;
extern bool g_modernSortGeometry;
extern bool g_modernSortByMaterial;
extern float g_modernMinimumProjectedSize;
extern float g_modernLodBias;
extern std::array<Vector4, 6> g_modernFrustumPlanes;

void *AllocateGeometryFrameMemory(std::uint32_t bytes);
RenderLayerGeometryContext *BuildRenderLayerGeometryContext(std::uint32_t handle, D3TDeviceContext *context);
void CullRenderLayerNodesAndSubmeshes(RenderLayerVisibility &visibility, const ModernScenePass &pass);
void SubmitRenderLayerGeometry(RenderLayerVisibility &visibility, D3TDeviceContext *context, const ModernScenePass &pass);
void CommitModelGeometryDraw(D3TDeviceContext *context, const ModelGeometryDraw &draw, const ModernScenePass &pass);
void DrawRenderLayerSubmeshBounds(std::uint32_t handle, D3TDeviceContext *context, const ModernScenePass &pass, std::uint32_t color);

} // namespace shenmue::s1
