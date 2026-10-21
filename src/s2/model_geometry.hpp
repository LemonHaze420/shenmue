#pragma once

#include "common/math.hpp"
#include "d3t/model_cache.hpp"
#include "d3t/buffer.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

struct ModelObjectConstants {
    Matrix4x4 world;
    Matrix4x4 previousWorld;
    std::uint32_t boneOffset;
    std::uint32_t instanceOffset;
    std::uint32_t writeVelocity;
    std::uint32_t lightIndexOffset;
    std::uint32_t useDualQuaternion;
};
struct ModelSkinningConstants {
    Matrix4x4 world;
    Matrix4x4 previousWorld;
    Matrix4x4 scale;
    Vector4 rotation;
    Vector4 dualTranslation;
};
struct ModelSkinningNode {
    std::uint32_t node;
    std::uint16_t skin;
    std::uint16_t reserved06;
    D3TModelRecord *model;
};
struct ModelGeometryContext {
    d3t::ModelCacheSlot *cache;
    Matrix4x4 *transforms;
    std::uint16_t *nodeFlags;
    std::uint32_t *skinOffsets;
    std::uint32_t *debugIndices;
    std::byte debugData[1024];
    std::uint32_t nodeCount;
    std::uint32_t reserved42c;
    std::uint32_t drawCapacity;
    std::uint32_t reserved434;
    Vector4 *waterBounds;
    Matrix4x4 *waterTransform;
    std::uint8_t hasSpecialGeometry;
    std::byte reserved449[3];
    std::uint32_t submittedDraws;
    std::uint32_t culledDraws;
};
static_assert(sizeof(ModelObjectConstants) == 148);
static_assert(offsetof(ModelObjectConstants, boneOffset) == 128);
static_assert(offsetof(ModelObjectConstants, writeVelocity) == 136);
static_assert(offsetof(ModelObjectConstants, lightIndexOffset) == 140);
static_assert(offsetof(ModelObjectConstants, useDualQuaternion) == 144);
static_assert(sizeof(ModelSkinningConstants) == 224);
static_assert(offsetof(ModelSkinningConstants, rotation) == 192);
static_assert(sizeof(ModelSkinningNode) == 16);
static_assert(sizeof(ModelGeometryContext) == 1112);
static_assert(offsetof(ModelGeometryContext, nodeCount) == 1064);
static_assert(offsetof(ModelGeometryContext, drawCapacity) == 1072);
static_assert(offsetof(ModelGeometryContext, waterBounds) == 1080);
static_assert(offsetof(ModelGeometryContext, hasSpecialGeometry) == 1096);
static_assert(offsetof(ModelGeometryContext, submittedDraws) == 1100);

inline constexpr std::uint32_t MODEL_GEOMETRY_FRAME_BYTES = 0x800000;
extern void *g_modelGeometryFrameMemory;
extern std::uint32_t g_modelGeometryFrameSize;
extern std::int32_t g_modelGeometryFrameOffset;
extern std::uint32_t g_modelGeometryFileLock;
extern std::uint32_t g_instanceBuffer;
extern std::uint32_t g_billboardBuffer;
extern std::uint32_t g_defaultPerObjectBuffer;
extern std::uint32_t g_dedicatedShadowBuffer;
void SetupGlobalRenderBuffers();
void ReleaseModelGeometryResources();
void *AllocateModelGeometryFrameMemory(std::uint32_t bytes);
void RefreshModelCacheMaterials(std::uint32_t handle, d3t::D3TDeviceContext *context, std::uint32_t fileLock);
ModelGeometryContext *BuildModelGeometry(std::uint32_t handle, d3t::D3TDeviceContext *context);

} // namespace shenmue::s2
