#include "s2/model_geometry.hpp"

#include "common/matrix_stack.hpp"
#include "d3t/file_state.hpp"
#include "d3t/material.hpp"
#include "s2/renderer_scratch.hpp"
#include <atomic>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstring>
#if defined(_WIN32)
#include <malloc.h>
#endif

namespace shenmue::s2 {

void *g_modelGeometryFrameMemory{};
std::uint32_t g_modelGeometryFrameSize{};
std::int32_t g_modelGeometryFrameOffset{};
std::uint32_t g_modelGeometryFileLock{0xffffU};
std::uint32_t g_instanceBuffer{};
std::uint32_t g_billboardBuffer{};
std::uint32_t g_defaultPerObjectBuffer{};
std::uint32_t g_dedicatedShadowBuffer{};

// @verified - p
void SetupGlobalRenderBuffers() {
    d3t::BufferDescription description{.flags = 3, .size = 0x28000, .count = 40, .flags3 = 2, .buffer = nullptr, .name = "Instance Buffer"};
    g_instanceBuffer = d3t::CreateBuffer(&description);
    description.flags = 2;
    description.size = 0x80000;
    description.count = 0;
    description.name = "Billboard Buffer";
    g_billboardBuffer = d3t::CreateBuffer(&description);
    const ModelObjectConstants defaults{.world = d3t::kModelCacheIdentity,
        .previousWorld = d3t::kModelCacheIdentity,
        .boneOffset = UINT32_MAX,
        .instanceOffset = 0,
        .writeVelocity = 0,
        .lightIndexOffset = 1,
        .useDualQuaternion = 0};
    std::byte initialData[160];
    std::memcpy(initialData, &defaults, sizeof(defaults));
    description.flags = 0;
    description.size = sizeof(defaults);
    description.flags3 = 0;
    description.buffer = initialData;
    description.name = "Default per object Buffer";
    g_defaultPerObjectBuffer = d3t::CreateBuffer(&description);
    std::byte shadow[48];
    std::memset(shadow, 0, 40);
    description.size = 40;
    description.buffer = shadow;
    description.name = "Default dedicated shadow Buffer";
    g_dedicatedShadowBuffer = d3t::CreateBuffer(&description);
}

void ReleaseModelGeometryResources() {
    if (g_modelGeometryFileLock != 0xffffU)
        d3t::ReleaseFileLockHandle(g_modelGeometryFileLock);
    g_modelGeometryFileLock = 0xffffU;
    d3t::ValidateAndCommitBuffer(g_instanceBuffer);
    d3t::ValidateAndCommitBuffer(g_billboardBuffer);
    d3t::ValidateAndCommitBuffer(g_defaultPerObjectBuffer);
    d3t::ValidateAndCommitBuffer(g_dedicatedShadowBuffer);
#if defined(_WIN32)
    _aligned_free(g_modelGeometryFrameMemory);
#else
    std::free(g_modelGeometryFrameMemory);
#endif
    g_modelGeometryFrameMemory = nullptr;
    g_modelGeometryFrameSize = 0;
}

void *AllocateModelGeometryFrameMemory(std::uint32_t bytes) {
    const std::uint32_t offset =
        (static_cast<std::uint32_t>(
             std::atomic_ref<std::int32_t>(g_modelGeometryFrameOffset).fetch_add(static_cast<std::int32_t>(bytes + 4U), std::memory_order_seq_cst)) +
            3U) &
        ~3U;
    return offset + bytes < g_modelGeometryFrameSize ? static_cast<std::byte *>(g_modelGeometryFrameMemory) + offset : nullptr;
}

// @verified - p
void RefreshModelCacheMaterials(std::uint32_t handle, d3t::D3TDeviceContext *context, std::uint32_t fileLock) {
    d3t::ModelCacheSlot *const cache = d3t::ResolveModelCache(handle);
    if (!cache)
        return;
    for (std::uint32_t node = 0; node < cache->nodeCount; ++node) {
        if ((cache->nodes[node].flags & 1U) || !cache->nodes[node].drawGroup)
            continue;
        D3TModelRecord *const model = d3t::ResolveModel(cache->modelHandles[node]);
        if (!model)
            continue;
        for (std::uint32_t material = 0; material < model->materialCount; ++material) {
            D3TModelMaterialRuntime &runtime = model->materialRuntime[material];
            if (!(runtime.flags & 2U))
                continue;
            const D3TModelMaterial &definition = model->materials[material];
            d3t::LockFileHandle(fileLock);
            d3t::GetOrCreateSceneEntity(definition, model->materialParameters + definition.firstParameter, runtime, context);
            d3t::UnlockFileHandle(fileLock);
            runtime.flags ^= 2U;
        }
    }
}

namespace {
Matrix4x4 ComposeModelTransform(const Matrix4x4 &parent, const Matrix4x4 &child) {
    Matrix4x4 result;
    for (unsigned row = 0; row < 4; ++row)
        for (unsigned column = 0; column < 4; ++column)
            result.rows[row][column] =
                ((parent.rows[row].x * child.rows[0][column] + parent.rows[row].y * child.rows[1][column]) + parent.rows[row].z * child.rows[2][column]) +
                parent.rows[row].w * child.rows[3][column];
    return result;
}

void PublishModelObjectConstants(ModelGeometryContext &geometry, d3t::D3TDeviceContext *context, std::uint32_t node) {
    d3t::ModelCacheSlot &cache = *geometry.cache;
    const std::uint16_t flags = geometry.nodeFlags[node];
    if ((flags & 2U) && cache.worldMatrices[node].rows[0].x == FLT_MAX)
        cache.worldMatrices[node] = geometry.transforms[node];
    ModelObjectConstants *const mapped = static_cast<ModelObjectConstants *>(d3t::GetWriteMappedBufferPointer(context, cache.constantBuffers[node], 1, 0));
    if (mapped)
        *mapped = {.world = geometry.transforms[node],
            .previousWorld = cache.worldMatrices[node],
            .boneOffset = geometry.skinOffsets[node],
            .instanceOffset = 0,
            .writeVelocity = (flags & 2U) != 0,
            .lightIndexOffset = (flags & 0x10U) == 0,
            .useDualQuaternion = (flags & 0x20U) == 0};
    d3t::DX11_CopySceneNodeToGPU(context, cache.constantBuffers[node]);
}

// @verified - p
void BuildModelDualQuaternion(ModelSkinningConstants &output, const Vector4 &quaternion, const Vector4 &translation) {
    const float inverse =
        1.0f / std::sqrt(((quaternion.x * quaternion.x + quaternion.y * quaternion.y) + quaternion.z * quaternion.z) + quaternion.w * quaternion.w);
    output.rotation = {quaternion.x * inverse, quaternion.y * inverse, quaternion.z * inverse, quaternion.w * inverse};
    const Vector4 q = output.rotation;
    output.dualTranslation = {(((q.w * translation.x + q.z * translation.y) - q.y * translation.z) + q.x * 0.0f) * 0.5f,
        (((q.w * translation.y - q.z * translation.x) + q.x * translation.z) + q.y * 0.0f) * 0.5f,
        (((q.y * translation.x - q.x * translation.y) + q.w * translation.z) + q.z * 0.0f) * 0.5f,
        (q.w * 0.0f - ((q.x * translation.x + q.y * translation.y) + q.z * translation.z)) * 0.5f};
}
} // namespace

ModelGeometryContext *BuildModelGeometry(std::uint32_t handle, d3t::D3TDeviceContext *context) {
    ModelGeometryContext *const geometry = static_cast<ModelGeometryContext *>(AllocateModelGeometryFrameMemory(sizeof(ModelGeometryContext)));
    std::memset(geometry, 0, sizeof(*geometry));
    d3t::ModelCacheSlot *const cache = d3t::ResolveModelCache(handle);
    if (!cache)
        return geometry;
    geometry->cache = cache;
    geometry->transforms = static_cast<Matrix4x4 *>(AllocateModelGeometryFrameMemory(cache->nodeCount * 64U));
    geometry->nodeFlags = static_cast<std::uint16_t *>(AllocateModelGeometryFrameMemory(cache->nodeCount * 2U));
    geometry->skinOffsets = static_cast<std::uint32_t *>(AllocateModelGeometryFrameMemory(cache->nodeCount * 4U));
    std::memset(geometry->skinOffsets, 0xff, static_cast<std::size_t>(cache->nodeCount) * 4U);
    geometry->debugIndices = static_cast<std::uint32_t *>(AllocateModelGeometryFrameMemory(cache->nodeCount * 4U));
    std::memset(geometry->debugIndices, 0, static_cast<std::size_t>(cache->nodeCount) * 4U);
    geometry->nodeCount = cache->nodeCount;
    ModelSkinningNode *const skinned =
        static_cast<ModelSkinningNode *>(GetRendererScratchBlock(static_cast<std::size_t>(cache->nodeCount) * sizeof(ModelSkinningNode)));
    std::uint32_t skinCount = 0;
    for (std::uint32_t node = 0; node < cache->nodeCount; ++node) {
        const D3TModelNode &entry = cache->nodes[node];
        geometry->transforms[node] =
            entry.parentDistance ? ComposeModelTransform(geometry->transforms[node - entry.parentDistance], cache->matrices[node]) : cache->matrices[node];
        geometry->nodeFlags[node] = static_cast<std::uint16_t>(entry.flags);
        if ((geometry->nodeFlags[node] & 1U) || !entry.drawGroup)
            continue;
        D3TModelRecord *model = d3t::ResolveModel(cache->modelHandles[node]);
        if (!model)
            continue;
        D3TModelDrawGroup *group = model->drawGroups + entry.drawGroup - 1U;
        if (group->part) {
            const D3TModelPart &part = model->parts[group->part - 1U];
            const std::uint32_t *const candidates = model->partHandles + 3U * (group->part - 1U);
            for (std::uint32_t candidate = 0; candidate < part.pathCount; ++candidate) {
                D3TModelRecord *const replacement = d3t::ResolveModel(candidates[candidate]);
                if (replacement && replacement->drawGroups[0].submeshCount > group->submeshCount) {
                    model = replacement;
                    group = replacement->drawGroups;
                }
            }
        }
        if (!group->skin)
            PublishModelObjectConstants(*geometry, context, node);
        for (std::uint32_t submesh = 0; submesh < group->submeshCount; ++submesh) {
            geometry->drawCapacity += model->referenceCount + 1U;
            const D3TModelMaterial &material = model->materials[model->submeshes[group->firstSubmesh + submesh].material];
            if ((geometry->nodeFlags[node] & 8U) == 0) {
                if ((material.materialType == 7 || material.materialType == 8) && !geometry->waterBounds) {
                    geometry->waterBounds = &model->bounds;
                    geometry->waterTransform = geometry->transforms + node;
                }
                if (material.materialType == 10)
                    geometry->hasSpecialGeometry = 1;
            }
        }
        if (static_cast<std::int16_t>(group->skin) > 0) {
            ModelSkinningNode &skinnedNode = skinned[skinCount++];
            skinnedNode.node = node;
            skinnedNode.skin = static_cast<std::uint16_t>(group->skin - 1U);
            skinnedNode.model = model;
        }
    }
    cache->publishedBoneCount = 0;
    ModelSkinningConstants *mapped = nullptr;
    bool hasSkinMapping = false;
    for (std::uint32_t index = 0; index < skinCount; ++index) {
        const ModelSkinningNode &skin = skinned[index];
        std::uint32_t rangeIndex = 0;
        for (std::uint32_t range = 0; range < cache->skinRangeCount; ++range)
            if (cache->skinRanges[range].nodeHash == cache->nodeHashes[skin.node]) {
                rangeIndex = range;
                break;
            }
        const d3t::ModelCacheSkinRange &range = cache->skinRanges[rangeIndex];
        geometry->transforms[skin.node] = d3t::kModelCacheIdentity;
        geometry->skinOffsets[skin.node] = cache->publishedBoneCount;
        const D3TModelSkin &definition = skin.model->skins[skin.skin];
        cache->publishedBoneCount += definition.boneCount;
        if (!mapped) {
            mapped = static_cast<ModelSkinningConstants *>(d3t::GetWriteMappedBufferPointer(context, cache->skinningBuffer, 1, 0));
            hasSkinMapping = mapped != nullptr;
        }
        if (mapped) {
            for (std::uint32_t bone = 0; bone < range.boneCount; ++bone) {
                const std::uint32_t node = d3t::LookupModelCacheIndex(cache->indexTable, cache->boneIndices[range.firstBone + bone]);
                const Matrix4x4 &bindPose = skin.model->bindPoses[definition.firstBone + bone];
                Matrix4x4 rotation = ComposeModelTransform(geometry->transforms[node], bindPose);
                ModelSkinningConstants &output = mapped[bone];
                Vector4 scales;
                Matrix4x4::ComputeColumnNorms(&rotation, &scales);
                const float x = scales.x, y = scales.y, z = scales.z;
                output.scale = d3t::kModelCacheIdentity;
                output.scale.rows[0].x = x;
                output.scale.rows[1].y = y;
                output.scale.rows[2].z = z;
                if (x > 0.0f && y > 0.0f && z > 0.0f)
                    for (unsigned row = 0; row < 3; ++row) {
                        rotation.rows[row].x *= 1.0f / x;
                        rotation.rows[row].y *= 1.0f / y;
                        rotation.rows[row].z *= 1.0f / z;
                    }
                Vector4 quaternion;
                matrix_stack::RotationMatrixToQuaternion(&rotation, &quaternion);
                const Vector4 translation{rotation.rows[0].w, rotation.rows[1].w, rotation.rows[2].w, 0};
                BuildModelDualQuaternion(output, quaternion, translation);
                output.world = ComposeModelTransform(geometry->transforms[node], bindPose);
                output.previousWorld = ComposeModelTransform(cache->worldMatrices[node], bindPose);
            }
            mapped += range.boneCount;
        }
        PublishModelObjectConstants(*geometry, context, skin.node);
    }
    if (hasSkinMapping)
        d3t::DX11_CopySceneNodeToGPU(context, cache->skinningBuffer);
    if (skinned)
        g_rendererScratch.current = reinterpret_cast<std::byte *>(skinned);
    return geometry;
}

} // namespace shenmue::s2
