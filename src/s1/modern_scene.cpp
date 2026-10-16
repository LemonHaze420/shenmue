#include "s1/modern_scene.hpp"

#include "s1/d3t/model.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/scratch_memory.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/initialization.hpp"
#include "s1/modern_lighting.hpp"
#include "s1/render_passes.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/fvf_layout.hpp"
#include "s1/archive_data.hpp"

#include <algorithm>
#include <atomic>
#include <bit>
#include <cfloat>
#include <cmath>
#include <cstring>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#endif

namespace shenmue::s1 {

extern void *g_largeDataBuffer;
extern std::uint32_t g_largeDataBufferSize;
bool g_modernNodeCulling = true;
bool g_modernSubmeshCulling = true;
bool g_modernDebugBounds = false;
bool g_modernSkipShadowNearPlanes = true;
bool g_modernSortGeometry = true;
bool g_modernSortByMaterial = true;
bool g_modernAlternateShading = false;
ModernGeometrySamplerSettings g_modernGeometrySamplerSettings{.magnification = 1, .minification = 1, .mip = 1, .anisotropy = 0};
float g_modernMinimumProjectedSize = 0.0f;
float g_modernLodBias = 0.0f;
std::array<Vector4, 6> g_modernFrustumPlanes{};

namespace {

constexpr Matrix4x4 kIdentity{{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

struct SkinnedNode {
    std::uint32_t node;
    std::uint16_t skin;
    std::uint16_t reserved;
    D3TModelRecord *model;
};

struct ObjectConstants {
    Matrix4x4 world;
    Matrix4x4 previousWorld;
    std::uint32_t skinOffset;
    std::uint32_t instanceOffset;
    std::uint32_t moving;
    std::uint32_t receiveShadows;
    std::uint32_t receiveLighting;
};

struct SkinningConstants {
    Matrix4x4 world;
    Matrix4x4 previousWorld;
    Matrix4x4 scale;
    Vector4 rotation;
    Vector4 dualTranslation;
};

static_assert(sizeof(SkinnedNode) == 16);
static_assert(sizeof(ObjectConstants) == 148);
static_assert(sizeof(SkinningConstants) == 224);

} // namespace


void *AllocateGeometryFrameMemory(std::uint32_t bytes) {
    const std::uint32_t offset =
        (static_cast<std::uint32_t>(
             std::atomic_ref<std::int32_t>(g_dxPipelineLock).fetch_add(static_cast<std::int32_t>(bytes + 4U), std::memory_order_seq_cst)) +
            3U) &
        ~3U;
    return offset + bytes < g_largeDataBufferSize ? static_cast<std::byte *>(g_largeDataBuffer) + offset : nullptr;
}

namespace {

Matrix4x4 Compose(const Matrix4x4 &parent, const Matrix4x4 &child) {
    Matrix4x4 result;
    for (unsigned row = 0; row < 4; ++row) {
        for (unsigned column = 0; column < 4; ++column) {
            result.rows[row][column] = parent.rows[row].x * child.rows[0][column] + parent.rows[row].y * child.rows[1][column] +
                parent.rows[row].z * child.rows[2][column] + parent.rows[row].w * child.rows[3][column];
        }
    }
    return result;
}

void TransformBounds(Vector3 &minimum, Vector3 &maximum, const Matrix4x4 &transform) {
    const Vector3 low = minimum;
    const Vector3 high = maximum;
    for (unsigned axis = 0; axis < 3; ++axis) {
        const Vector4 &row = transform.rows[axis];
        minimum[axis] = std::fmin(row.x * low.x, row.x * high.x) + std::fmin(row.y * low.y, row.y * high.y) + std::fmin(row.z * low.z, row.z * high.z) + row.w;
        maximum[axis] = std::fmax(row.x * low.x, row.x * high.x) + std::fmax(row.y * low.y, row.y * high.y) + std::fmax(row.z * low.z, row.z * high.z) + row.w;
    }
}

float BoundsDistanceSquared(const Vector3 &minimum, const Vector3 &maximum, const Vector3 &position) {
    float distance = 0.0f;
    for (unsigned axis = 0; axis < 3; ++axis) {
        if (minimum[axis] > position[axis]) {
            const float delta = minimum[axis] - position[axis];
            distance += delta * delta;
        }
        if (position[axis] > maximum[axis]) {
            const float delta = position[axis] - maximum[axis];
            distance += delta * delta;
        }
    }
    return distance;
}

Vector4 RotationQuaternion(const Matrix4x4 &matrix) {
    const float x = matrix.rows[0].x;
    const float y = matrix.rows[1].y;
    const float z = matrix.rows[2].z;
    const float trace = x + y + z;
    Vector4 result;
    if (trace > 0.0f) {
        const float reciprocal = 1.0f / (std::sqrt(trace + 1.0f) * 2.0f);
        result = {(matrix.rows[2].y - matrix.rows[1].z) * reciprocal, (matrix.rows[0].z - matrix.rows[2].x) * reciprocal,
            (matrix.rows[1].x - matrix.rows[0].y) * reciprocal, 0.25f / reciprocal};
    } else if (x > y && x > z) {
        const float reciprocal = 1.0f / (std::sqrt(x + 1.0f - y - z) * 2.0f);
        result = {0.25f / reciprocal, (matrix.rows[1].x + matrix.rows[0].y) * reciprocal, (matrix.rows[2].x + matrix.rows[0].z) * reciprocal,
            (matrix.rows[2].y - matrix.rows[1].z) * reciprocal};
    } else if (y > z) {
        const float reciprocal = 1.0f / (std::sqrt(y + 1.0f - x - z) * 2.0f);
        result = {(matrix.rows[1].x + matrix.rows[0].y) * reciprocal, 0.25f / reciprocal, (matrix.rows[2].y + matrix.rows[1].z) * reciprocal,
            (matrix.rows[0].z - matrix.rows[2].x) * reciprocal};
    } else {
        const float reciprocal = 1.0f / (std::sqrt(z + 1.0f - x - y) * 2.0f);
        result = {(matrix.rows[2].x + matrix.rows[0].z) * reciprocal, (matrix.rows[2].y + matrix.rows[1].z) * reciprocal, 0.25f / reciprocal,
            (matrix.rows[1].x - matrix.rows[0].y) * reciprocal};
    }
    return result;
}

void PublishObjectConstants(RenderLayerGeometryContext &geometry, D3TDeviceContext *context, std::uint32_t node) {
    CachedSkinningBufferSlot &slot = *geometry.skinning;
    const std::uint16_t flags = geometry.nodeFlags[node];
    if ((flags & 2U) && slot.secondaryMatrices[node].rows[0].x == FLT_MAX)
        slot.secondaryMatrices[node] = geometry.transforms[node];
    ObjectConstants *const mapped = static_cast<ObjectConstants *>(GetWriteMappedBufferPointer(context, slot.constantBufferHandles[node], 1, 0));
    if (mapped) {
        *mapped = {.world = geometry.transforms[node],
            .previousWorld = slot.secondaryMatrices[node],
            .skinOffset = geometry.skinOffsets[node],
            .instanceOffset = 0,
            .moving = (flags & 2U) != 0,
            .receiveShadows = (flags & 0x10U) == 0,
            .receiveLighting = (flags & 0x20U) == 0};
    }
    DX11_CopySceneNodeToGPU(context, slot.constantBufferHandles[node]);
}

bool IncludesMaterial(std::uint32_t mask, std::uint32_t blendMode) {
    switch (blendMode) {
    case 0:
        return (mask & 1U) != 0;
    case 1:
    case 5:
        return (mask & 2U) != 0;
    case 2:
    case 3:
        return (mask & 8U) != 0;
    case 4:
        return (mask & 4U) != 0;
    default:
        return false;
    }
}

} // namespace


RenderLayerGeometryContext *BuildRenderLayerGeometryContext(std::uint32_t handle, D3TDeviceContext *context) {
    RenderLayerGeometryContext *const geometry = static_cast<RenderLayerGeometryContext *>(AllocateGeometryFrameMemory(sizeof(RenderLayerGeometryContext)));
    if (!geometry)
        return nullptr;
    std::memset(geometry, 0, sizeof(*geometry));
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(handle);
    if (!slot)
        return geometry;
    geometry->skinning = slot;
    geometry->nodeCount = slot->entryCount;
    geometry->transforms = static_cast<Matrix4x4 *>(AllocateGeometryFrameMemory(slot->entryCount * sizeof(Matrix4x4)));
    geometry->nodeFlags = static_cast<std::uint16_t *>(AllocateGeometryFrameMemory(slot->entryCount * sizeof(std::uint16_t)));
    geometry->skinOffsets = static_cast<std::uint32_t *>(AllocateGeometryFrameMemory(slot->entryCount * sizeof(std::uint32_t)));
    geometry->debugIndices = static_cast<std::uint32_t *>(AllocateGeometryFrameMemory(slot->entryCount * sizeof(std::uint32_t)));
    if (!geometry->transforms || !geometry->nodeFlags || !geometry->skinOffsets || !geometry->debugIndices)
        return nullptr;
    std::memset(geometry->skinOffsets, 0xff, slot->entryCount * sizeof(std::uint32_t));
    std::memset(geometry->debugIndices, 0, slot->entryCount * sizeof(std::uint32_t));
    void *const checkpoint = GetGlobalPoolCheckpoint();
    SkinnedNode *const skinned = static_cast<SkinnedNode *>(AllocateMemoryFromGlobalPool(slot->entryCount * sizeof(SkinnedNode)));
    std::uint32_t skinnedCount = 0;
    const D3TModelNode *const nodes = reinterpret_cast<const D3TModelNode *>(slot->entries);
    for (std::uint32_t index = 0; index < slot->entryCount; ++index) {
        const D3TModelNode &node = nodes[index];
        geometry->transforms[index] =
            node.parentDistance ? Compose(geometry->transforms[index - node.parentDistance], slot->matrices[index]) : slot->matrices[index];
        geometry->nodeFlags[index] = static_cast<std::uint16_t>(node.flags);
        if ((node.flags & 1U) || !node.drawGroup)
            continue;
        D3TModelRecord *model = d3t::ResolveModel(slot->renderHandles[index]);
        if (!model)
            continue;
        D3TModelDrawGroup *group = model->drawGroups + node.drawGroup - 1U;
        if (group->part) {
            const D3TModelPart &part = model->parts[group->part - 1U];
            const std::uint32_t *const handles = model->partHandles + 3U * (group->part - 1U);
            for (std::uint32_t partIndex = 0; partIndex < part.pathCount; ++partIndex) {
                D3TModelRecord *const candidate = d3t::ResolveModel(handles[partIndex]);
                if (candidate && candidate->drawGroups[0].submeshCount > group->submeshCount) {
                    model = candidate;
                    group = candidate->drawGroups;
                }
            }
        }
        if (!group->skin)
            PublishObjectConstants(*geometry, context, index);
        for (std::uint32_t submesh = 0; submesh < group->submeshCount; ++submesh) {
            geometry->drawCapacity += model->referenceCount + 1U;
            const D3TModelMaterial &material = model->materials[model->submeshes[group->firstSubmesh + submesh].material];
            if ((node.flags & 8U) == 0) {
                if ((material.materialType == 7 || material.materialType == 8) && !geometry->waterBounds) {
                    geometry->waterBounds = &model->bounds;
                    geometry->waterTransform = geometry->transforms + index;
                }
                if (material.materialType == 10)
                    geometry->hasSpecialGeometry = 1;
            }
        }
        if (static_cast<std::int16_t>(group->skin) > 0)
            skinned[skinnedCount++] = {.node = index, .skin = static_cast<std::uint16_t>(group->skin - 1U), .model = model};
    }
    slot->publishedBoneCount = 0;
    SkinningConstants *mapped = nullptr;
    bool mappedSkin = false;
    for (std::uint32_t index = 0; index < skinnedCount; ++index) {
        const SkinnedNode &item = skinned[index];
        std::uint32_t rangeIndex = 0;
        for (std::uint32_t range = 0; range < slot->skinRangeCount; ++range) {
            if (slot->skinRanges[range * 3U] == slot->parentHashes[item.node]) {
                rangeIndex = range;
                break;
            }
        }
        const std::uint32_t *const range = slot->skinRanges + rangeIndex * 3U;
        geometry->transforms[item.node] = kIdentity;
        geometry->skinOffsets[item.node] = slot->publishedBoneCount;
        slot->publishedBoneCount += item.model->skins[item.skin].boneCount;
        if (!mapped) {
            mapped = static_cast<SkinningConstants *>(GetWriteMappedBufferPointer(context, slot->skinningBuffer, 1, 0));
            mappedSkin = mapped != nullptr;
        }
        if (mapped) {
            for (std::uint32_t bone = 0; bone < range[2]; ++bone) {
                const std::uint32_t node = GetValueFromMurmurHash(&slot->valueTable, slot->boneIndices[range[1] + bone]);
                const Matrix4x4 &bindPose = item.model->bindPoses[item.model->skins[item.skin].firstBone + bone];
                SkinningConstants &output = mapped[bone];
                output.world = Compose(geometry->transforms[node], bindPose);
                output.previousWorld = Compose(slot->secondaryMatrices[node], bindPose);
                output.scale = kIdentity;
                Matrix4x4 rotation = output.world;
                float scales[3];
                for (unsigned axis = 0; axis < 3; ++axis) {
                    scales[axis] = std::sqrt(rotation.rows[0][axis] * rotation.rows[0][axis] + rotation.rows[1][axis] * rotation.rows[1][axis] +
                        rotation.rows[2][axis] * rotation.rows[2][axis] + rotation.rows[3][axis] * rotation.rows[3][axis]);
                    output.scale.rows[axis][axis] = scales[axis];
                }
                if (scales[0] > 0.0f && scales[1] > 0.0f && scales[2] > 0.0f) {
                    for (unsigned row = 0; row < 3; ++row)
                        for (unsigned column = 0; column < 3; ++column)
                            rotation.rows[row][column] *= 1.0f / scales[column];
                }
                Vector4 quaternion = RotationQuaternion(rotation);
                const float inverseLength =
                    1.0f / std::sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
                for (unsigned component = 0; component < 4; ++component)
                    quaternion[component] *= inverseLength;
                output.rotation = quaternion;
                const float x = rotation.rows[0].w;
                const float y = rotation.rows[1].w;
                const float z = rotation.rows[2].w;
                output.dualTranslation = {(quaternion.w * x + quaternion.z * y - quaternion.y * z) * 0.5f,
                    (quaternion.w * y - quaternion.z * x + quaternion.x * z) * 0.5f, (quaternion.y * x - quaternion.x * y + quaternion.w * z) * 0.5f,
                    -(quaternion.x * x + quaternion.y * y + quaternion.z * z) * 0.5f};
            }
            mapped += range[2];
        }
        PublishObjectConstants(*geometry, context, item.node);
    }
    if (mappedSkin)
        DX11_CopySceneNodeToGPU(context, slot->skinningBuffer);
    RestoreGlobalPoolCheckpoint(checkpoint);
    return geometry;
}

void CullRenderLayerNodesAndSubmeshes(RenderLayerVisibility &visibility, const ModernScenePass &pass) {
    RenderLayerGeometryContext &geometry = *visibility.geometry;
    if (!visibility.nodeMask) {
        const std::uint32_t nodeBytes = (geometry.nodeCount + 1U) * sizeof(std::uint32_t);
        const std::uint32_t submeshBytes = (geometry.drawCapacity + 1U) * sizeof(std::uint32_t);
        visibility.nodeMask = static_cast<std::uint32_t *>(AllocateGeometryFrameMemory(nodeBytes));
        visibility.submeshMask = static_cast<std::uint32_t *>(AllocateGeometryFrameMemory(submeshBytes));
        if (!visibility.nodeMask || !visibility.submeshMask)
            return;
        std::memset(visibility.nodeMask, 0xff, nodeBytes);
        std::memset(visibility.submeshMask, 0xff, submeshBytes);
    }
    if (!geometry.skinning)
        return;
    const CachedSkinningBufferSlot &slot = *geometry.skinning;
    const D3TModelNode *const nodes = reinterpret_cast<const D3TModelNode *>(slot.entries);
    const std::uint8_t skip = pass.type == 1 && g_modernSkipShadowNearPlanes;
    std::uint32_t firstSubmesh = 0;
    for (std::uint32_t node = 0; node < slot.entryCount; ++node) {
        if ((geometry.nodeFlags[node] & 1U) || !nodes[node].drawGroup)
            continue;
        const D3TModelRecord *const model = d3t::ResolveModel(slot.renderHandles[node]);
        if (!model)
            continue;
        const D3TModelDrawGroup &group = model->drawGroups[nodes[node].drawGroup - 1U];
        Vector3 minimum = group.boundsMinimum;
        Vector3 maximum = group.boundsMaximum;
        TransformBounds(minimum, maximum, geometry.transforms[node]);
        const std::uint32_t bit = 1U << (node & 31U);
        if (!IsAabbInsideSelectedFrustumPlanes(g_modernFrustumPlanes, minimum, maximum, skip)) {
            visibility.nodeMask[node >> 5U] &= ~bit;
        } else {
            visibility.nodeMask[node >> 5U] |= bit;
            for (std::uint32_t mesh = 0; mesh < group.submeshCount; ++mesh) {
                const D3TModelSubmesh &submesh = model->submeshes[group.firstSubmesh + mesh];
                minimum = submesh.boundsMinimum;
                maximum = submesh.boundsMaximum;
                TransformBounds(minimum, maximum, geometry.transforms[node]);
                const Vector3 extent{maximum.x - minimum.x, maximum.y - minimum.y, maximum.z - minimum.z};
                const float diameter = std::sqrt(extent.x * extent.x + extent.y * extent.y + extent.z * extent.z);
                const float distance = BoundsDistanceSquared(minimum, maximum, pass.cameraPosition);
                const float projected = (distance <= 0.0f ? 1000.0f : (diameter * 0.5f) / (std::tan(0.78539753f) * distance)) * visibility.viewportWidth;
                if (!group.skin) {
                    const std::uint32_t meshBit = 1U << ((firstSubmesh + mesh) & 31U);
                    std::uint32_t &word = visibility.submeshMask[(firstSubmesh + mesh) >> 5U];
                    if ((!g_modernSubmeshCulling || IsAabbInsideSelectedFrustumPlanes(g_modernFrustumPlanes, minimum, maximum, skip)) &&
                        g_modernMinimumProjectedSize <= projected)
                        word |= meshBit;
                    else
                        word &= ~meshBit;
                }
            }
        }
        firstSubmesh += group.submeshCount;
    }
}

namespace {

template <typename T> T ReadField(const void *source, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<const std::byte *>(source) + offset, sizeof(value));
    return value;
}

template <typename T> void WriteField(void *destination, std::size_t offset, const T &value) {
    std::memcpy(static_cast<std::byte *>(destination) + offset, &value, sizeof(value));
}

void BindConstantBuffer(D3TDeviceContext *context, unsigned stages, unsigned slot, std::uint32_t handle) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    for (unsigned stage = 0; stage < 3; ++stage) {
        const std::size_t offset = 0x8cU + stage * 56U + slot * 4U;
        if ((stages & (1U << stage)) && ReadField<std::uint32_t>(&state, offset) != handle) {
            state.constantBufferDirtyMasks[stage] |= static_cast<std::uint16_t>(1U << slot);
            WriteField(&state, offset, handle);
        }
    }
}

void BindGeometryBuffer(D3TDeviceContext *context, unsigned stage, unsigned slot, std::uint32_t handle) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const std::size_t offset = 0x734U + (stage * 8U + slot) * 4U;
    if (ReadField<std::uint32_t>(&state, offset) != handle) {
        state.shaderResourceDirtyMasks[stage] |= 1U << slot;
        WriteField(&state, offset, handle);
    }
}

void BindGeometryTexture(D3TDeviceContext *context, unsigned slot, std::uint32_t handle) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const std::size_t resource = 0x1b4U + slot * 4U;
    const std::size_t view = 0x334U + slot * 4U;
    if (ReadField<std::uint32_t>(&state, resource) != handle || ReadField<std::uint32_t>(&state, view) != 0) {
        state.shaderResourceDirtyMasks[1] |= 1U << slot;
        WriteField(&state, resource, handle);
        WriteField(&state, view, std::uint32_t{0});
    }
}

void BindGeometrySampler(D3TDeviceContext *context, unsigned slot, std::uint64_t packed) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const std::size_t offset = 0x534U + slot * 16U;
    if (ReadField<std::uint64_t>(&state, offset) != packed) {
        state.samplerDirtyMasks[1] |= static_cast<std::uint16_t>(1U << slot);
        WriteField(&state, offset, packed);
        WriteField(&state, offset + 8U, std::uint64_t{0});
    }
}

void SetSampleMask(D3TDeviceContext *context, std::uint32_t mask) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.blendSampleMask != mask) {
        state.pipelineStateFlags |= d3t::PipelineBlend;
        state.blendSampleMask = mask;
    }
}

void CommitGeometryState(D3TDeviceContext *context) {
#if defined(_WIN32)
    DX11_ResetMappedResources(context);
    DX11_UnmapAndSyncShaderBindings(context);
    DX11_SetupShadersForDXAndSetViewports(context);
#else
    static_cast<void>(context);
#endif
}

} // namespace


void CommitModelGeometryDraw(D3TDeviceContext *context, const ModelGeometryDraw &draw, const ModernScenePass &pass) {
    const D3TModelRecord &model = *draw.model;
    const D3TModelDrawGroup &group = model.drawGroups[draw.drawGroup];
    const D3TModelGeometryRuntime &geometry = model.geometry[group.geometryIndex];
    const D3TModelSubmesh &mesh = model.submeshes[group.firstSubmesh + draw.submesh];
    const D3TModelMaterial &material = model.materials[mesh.material];
    const D3TModelMaterialRuntime &runtime = model.materialRuntime[mesh.material];
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    std::uint32_t vertexShader = state.vertexShader;
    std::uint32_t pixelShader = state.pixelShader;
    bool noPixelShader = false;
    switch (pass.type) {
    case 0:
    case 6:
        if (pass.deferred && (material.blendType & ~5U) == 0) {
            if (g_modernAlternateShading) {
                vertexShader = runtime.shaders[15];
                pixelShader = runtime.shaders[pass.alternateShader ? 17 : 16];
                noPixelShader = runtime.shaders[16] == 0;
            } else {
                vertexShader = runtime.shaders[11];
                pixelShader = runtime.shaders[pass.alternateShader ? (pass.type == 6 ? 14 : 13) : 12];
                noPixelShader = runtime.shaders[pass.alternateShader ? 16 : 12] == 0;
            }
            break;
        }
        [[fallthrough]];
    case 5:
    case 7:
        if (g_modernAlternateShading) {
            vertexShader = runtime.shaders[8];
            pixelShader = runtime.shaders[pass.alternateShader ? 10 : 9];
            noPixelShader = runtime.shaders[9] == 0;
        } else {
            vertexShader = runtime.shaders[5];
            pixelShader = runtime.shaders[pass.alternateShader ? 7 : 6];
            noPixelShader = runtime.shaders[pass.alternateShader ? 9 : 6] == 0;
        }
        break;
    case 1:
        vertexShader = runtime.shaders[0];
        pixelShader = (pass.materialMask & 6U) ? runtime.shaders[1] : 0;
        noPixelShader = (pass.materialMask & 6U) == 0;
        break;
    case 2:
    case 3:
        vertexShader = runtime.shaders[2];
        pixelShader = (pass.materialMask & 6U) ? runtime.shaders[pass.type == 2 ? 3 : 4] : 0;
        noPixelShader = (pass.materialMask & 6U) == 0;
        break;
    case 4:
        vertexShader = pass.vertexShader;
        pixelShader = pass.pixelShader;
        noPixelShader = pixelShader == 0;
        break;
    default:
        break;
    }
    SetShaderState(context, vertexShader, pixelShader, state.inputLayout);
    if (draw.debugOverride) {
#if defined(_WIN32)
        std::byte *const debug = static_cast<std::byte *>(MapTempConstantBufferForWriteDiscard(context, 3, 10, 40));
        if (!debug)
            return;
        WriteField(debug, 0, static_cast<std::uint32_t>(draw.debugOverride));
        std::memcpy(debug + 4, draw.debugValues.data(), 32);
        WriteField(debug, 36, draw.debugIndex);
#endif
    } else {
        BindConstantBuffer(context, 3, 10, g_dedicatedShadowBuffer);
    }
    BindConstantBuffer(context, 3, 1, draw.objectBuffer);
    const std::uint32_t savedRasterizer = state.rasterizerState;
    std::uint32_t rasterizer = material.doubleSided ? savedRasterizer & ~0x18U : savedRasterizer;
    const std::int32_t signedBias = static_cast<std::int32_t>(material.depthBias);
    const std::uint32_t bias = signedBias < 0 ? (0U - static_cast<std::uint32_t>(signedBias)) | 0x80U : static_cast<std::uint32_t>(signedBias);
    rasterizer ^= (rasterizer ^ (bias * 32U)) & 0x1fe0U;
    if (state.rasterizerState != rasterizer) {
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
        state.rasterizerState = rasterizer;
    }
    if (draw.opacity < 1.0f && pass.type != 1)
        SetSampleMask(context, static_cast<std::uint32_t>(static_cast<std::int32_t>(draw.opacity * 255.0f)));
    for (std::uint32_t vertex = 0; vertex < geometry.vertexBufferCount; ++vertex) {
        if (state.vertexBufferHandles[vertex] != geometry.vertexBuffers[vertex]) {
            state.vertexBufferHandles[vertex] = geometry.vertexBuffers[vertex];
            state.vertexBufferDirtyMask |= 1U << vertex;
        }
    }
    if (!noPixelShader) {
        const std::uint8_t alphaToCoverage = material.blendType == 4 && !pass.deferred && (pass.type == 0 || pass.type == 5 || pass.type == 6);
        if (ReadField<std::uint8_t>(state.blendState.data(), 64) != alphaToCoverage) {
            state.pipelineStateFlags |= d3t::PipelineBlend;
            WriteField(state.blendState.data(), 64, alphaToCoverage);
        }
        BindConstantBuffer(context, 3, 5, runtime.constantBuffer);
        if (runtime.textures[0]) {
            for (unsigned texture = 0; texture < 14; ++texture)
                BindGeometryTexture(context, 8U + texture, runtime.textures[texture]);
        } else {
            BindGeometryTexture(context, 8, d3t::g_defaultQuarterTexture);
            BindGeometryTexture(context, 9, d3t::g_defaultNormalTexture);
            BindGeometryTexture(context, 10, d3t::g_defaultThreeQuarterTexture);
            BindGeometryTexture(context, 11, d3t::g_defaultBlackTexture);
        }
        std::uint64_t sampler = (g_modernGeometrySamplerSettings.mip & 3U) * 16U | (g_modernGeometrySamplerSettings.magnification & 3U) * 4U |
            (g_modernGeometrySamplerSettings.minification & 3U);
        if (g_modernGeometrySamplerSettings.magnification == 2) {
            const std::uint32_t anisotropy =
                g_modernGeometrySamplerSettings.anisotropy >= 1 ? static_cast<std::uint32_t>(g_modernGeometrySamplerSettings.anisotropy) : 1U;
            sampler ^= (sampler ^ ((anisotropy - 1U) << 14U)) & 0x3c000U;
        }
        if (material.anisotropyLevel > 0) {
            const unsigned shift = static_cast<unsigned>(material.anisotropyLevel - 1) & 31U;
            const std::int32_t requested = static_cast<std::int32_t>(1U << shift);
            const std::uint32_t anisotropy = requested <= 1 ? 1U : requested > 16 ? 16U : static_cast<std::uint32_t>(requested);
            sampler = 42U | ((anisotropy - 1U) << 14U);
        }
        sampler |= static_cast<std::uint64_t>(material.addressU & 3U) << 6U;
        sampler |= static_cast<std::uint64_t>(material.addressV & 3U) << 8U;
        BindGeometrySampler(context, 0, sampler | (static_cast<std::uint64_t>(std::bit_cast<std::uint32_t>(material.minimumLod)) << 18U));
        const float clampedLod = material.maximumLod <= material.minimumLod ? material.maximumLod : material.minimumLod;
        BindGeometrySampler(context, 1, sampler | (static_cast<std::uint64_t>(std::bit_cast<std::uint32_t>(clampedLod)) << 18U));
        BindGeometrySampler(context, 2, 21);
        BindGeometrySampler(context, 3, 21);
        BindGeometrySampler(context, 10, 640);
        BindGeometrySampler(context, 11, 661);
    }
    if (pass.drawCallback) {
        ModernDrawCallbackContext callback{.context = context, .transform = draw.transform};
        pass.drawCallback(&callback);
    }
    if (material.materialType == 4 || material.materialType == 5) {
        const std::uint32_t firstBillboard = group.firstBillboard + mesh.firstIndex / 6U;
        const Vector4 &column = draw.transform.rows[0];
        const float scale = std::sqrt(column.x * column.x + draw.transform.rows[1].x * draw.transform.rows[1].x +
            draw.transform.rows[2].x * draw.transform.rows[2].x + draw.transform.rows[3].x * draw.transform.rows[3].x);
#if defined(_WIN32)
        void *const billboard = MapTempConstantBufferForWriteDiscard(context, 1, 2, 8);
        if (billboard) {
            WriteField(billboard, 0, firstBillboard);
            WriteField(billboard, 4, scale);
        }
#else
        static_cast<void>(firstBillboard);
        static_cast<void>(scale);
#endif
        if (state.inputLayout) {
            state.inputLayout = 0;
            state.pipelineStateFlags |= d3t::PipelineInputLayout;
        }
        CommitGeometryState(context);
#if defined(_WIN32)
        ID3D11DeviceContext *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
        if (native) {
            if (material.materialType == 5)
                native->DrawIndexed(draw.instanceCount, draw.firstInstance, 0);
            else
                native->Draw(6U * (mesh.primitiveCount >> 1U), 0);
        }
#endif
    } else {
        if (state.indexBufferHandle != geometry.indexBuffer)
            state.pipelineStateFlags |= d3t::PipelineIndexBuffer;
        state.indexBufferHandle = geometry.indexBuffer;
        state.indexFormat = geometry.indexFormat;
        CommitGeometryState(context);
#if defined(_WIN32)
        ID3D11DeviceContext *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
        if (native) {
            if (draw.instanceCount)
                native->DrawIndexedInstanced(
                    3U * mesh.primitiveCount, draw.instanceCount, mesh.firstIndex + group.firstIndex, group.baseVertex, draw.firstInstance);
            else
                native->DrawIndexed(3U * mesh.primitiveCount, mesh.firstIndex + group.firstIndex, group.baseVertex);
        }
#endif
    }
    if (draw.opacity < 1.0f)
        SetSampleMask(context, 0xff);
    if (material.doubleSided && state.rasterizerState != savedRasterizer) {
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
        state.rasterizerState = savedRasterizer;
    }
}


void SubmitRenderLayerGeometry(RenderLayerVisibility &visibility, D3TDeviceContext *context, const ModernScenePass &pass) {
    RenderLayerGeometryContext &geometry = *visibility.geometry;
    geometry.submittedDraws = 0;
    geometry.culledDraws = 0;
    if (!geometry.skinning || !geometry.drawCapacity)
        return;
    CachedSkinningBufferSlot &slot = *geometry.skinning;
#if defined(_WIN32)
    struct PassConstants {
        std::uint32_t writeVelocity, fogDisabled;
        float clusterNear, clusterFar;
    };
    PassConstants *const constants = static_cast<PassConstants *>(MapTempConstantBufferForWriteDiscard(context, 3, 7, 16));
    if (constants)
        *constants = {pass.type != 6 && pass.disableVelocity != 1, pass.fogDisabled, pass.clusterNear, pass.clusterFar};
#endif
    void *const checkpoint = GetGlobalPoolCheckpoint();
    std::uint32_t *const order = static_cast<std::uint32_t *>(AllocateMemoryFromGlobalPool(4U * geometry.drawCapacity));
    ModelGeometryDraw *const draws = static_cast<ModelGeometryDraw *>(AllocateMemoryFromGlobalPool(sizeof(ModelGeometryDraw) * geometry.drawCapacity));
    float *const distances = static_cast<float *>(AllocateMemoryFromGlobalPool(4U * geometry.drawCapacity));
    std::int32_t *const materials = static_cast<std::int32_t *>(AllocateMemoryFromGlobalPool(4U * geometry.drawCapacity));
    const bool sort = (pass.materialMask & 8U) || (g_modernSortGeometry && (pass.materialMask & 7U));
    const D3TModelNode *const nodes = reinterpret_cast<const D3TModelNode *>(slot.entries);
    std::uint32_t count = 0;
    std::uint32_t firstSubmesh = 0;
    std::uint32_t billboardIndexCount = 0;
    for (std::uint32_t node = 0; node < slot.entryCount; ++node) {
        const std::uint16_t flags = geometry.nodeFlags[node];
        std::uint32_t sourceSubmeshCount = 0;
        D3TModelRecord *model = nullptr;
        if (!(flags & 1U) && nodes[node].drawGroup) {
            model = d3t::ResolveModel(slot.renderHandles[node]);
            if (model)
                sourceSubmeshCount = model->drawGroups[nodes[node].drawGroup - 1U].submeshCount;
        }
        const bool excluded = ((flags & 2U) && pass.excludeMoving) || ((flags & 0x80U) && !pass.alternateNodes) || ((flags & 0x40U) && pass.alternateNodes) ||
            ((flags & 8U) && pass.type < 4 && pass.type != 1);
        if (model && !excluded) {
            std::uint16_t groupIndex = nodes[node].drawGroup - 1U;
            D3TModelDrawGroup *group = model->drawGroups + groupIndex;
            if (group->part) {
                const D3TModelPart &part = model->parts[group->part - 1U];
                const std::uint32_t *const handles = model->partHandles + 3U * (group->part - 1U);
                const Matrix4x4 &transform = geometry.transforms[node];
                const float x = transform.rows[0].w - pass.cameraPosition.x;
                const float y = transform.rows[1].w - pass.cameraPosition.y;
                const float z = transform.rows[2].w - pass.cameraPosition.z;
                const float distance = std::sqrt(x * x + y * y + z * z) + g_modernLodBias + pass.lodBias;
                for (std::uint32_t partIndex = 0; partIndex < part.pathCount; ++partIndex) {
                    if (ReadField<float>(&part, 12U + partIndex * 4U) >= distance)
                        break;
                    D3TModelRecord *const candidate = d3t::ResolveModel(handles[partIndex]);
                    if (candidate) {
                        model = candidate;
                        group = model->drawGroups;
                        groupIndex = 0;
                    }
                }
            }
            for (std::uint32_t meshIndex = 0; meshIndex < group->submeshCount; ++meshIndex) {
                const D3TModelSubmesh &mesh = model->submeshes[group->firstSubmesh + meshIndex];
                const D3TModelMaterial &material = model->materials[mesh.material];
                if (pass.type == 1 && (!material.castsShadows || material.unlit))
                    continue;
                if (pass.type == 6 && (material.materialType == 7 || material.materialType == 8 || (flags & 0x100U)))
                    continue;
                if (material.materialType == 10 ? pass.type < 4 && pass.type != 1 : pass.type == 7)
                    continue;
                if (!IncludesMaterial(pass.materialMask, material.blendType))
                    continue;
                const bool bypassCulling = (flags & 2U) || group->skin;
                if (!bypassCulling &&
                    ((visibility.nodeMask && !(visibility.nodeMask[node >> 5U] & (1U << (node & 31U)))) ||
                        (g_modernSubmeshCulling && visibility.submeshMask &&
                            !(visibility.submeshMask[(firstSubmesh + meshIndex) >> 5U] & (1U << ((firstSubmesh + meshIndex) & 31U)))))) {
                    ++geometry.culledDraws;
                    continue;
                }
                order[count] = count;
                ModelGeometryDraw &draw = draws[count];
                draw = {.transform = geometry.transforms[node],
                    .objectBuffer = slot.constantBufferHandles[node],
                    .model = model,
                    .drawGroup = groupIndex,
                    .submesh = static_cast<std::uint16_t>(meshIndex),
                    .nodeIndex = node,
                    .opacity = slot.floatValues[node]};
                if (geometry.debugIndices[node]) {
                    draw.debugOverride = 1;
                    draw.debugValues = geometry.debugRecords[geometry.debugIndices[node]];
                    draw.debugIndex = geometry.debugIndices[node];
                }
                materials[count] = static_cast<std::int32_t>(mesh.material);
                if (sort) {
                    Vector3 minimum = mesh.boundsMinimum;
                    Vector3 maximum = mesh.boundsMaximum;
                    TransformBounds(minimum, maximum, draw.transform);
                    distances[count] = BoundsDistanceSquared(minimum, maximum, pass.cameraPosition);
                }
                ++count;
                if (material.materialType == 5) {
                    const std::uint32_t billboardCount = mesh.primitiveCount >> 1U;
                    const std::uint32_t firstBillboard = group->firstBillboard + mesh.firstIndex / 6U;
                    void *const billboardCheckpoint = GetGlobalPoolCheckpoint();
                    std::uint32_t *const indices = static_cast<std::uint32_t *>(AllocateMemoryFromGlobalPool(24U * billboardCount));
                    std::uint32_t indexCount = 0;
                    for (std::uint32_t reference = 0; reference < model->referenceCount; ++reference) {
                        D3TModelRecord *const referenced = d3t::ResolveModel(model->referenceHandles[reference]);
                        if (!referenced)
                            continue;
                        const float threshold = ReadField<float>(model->references + reference, 12);
                        const float fade = ReadField<float>(model->references + reference, 16);
                        for (std::uint32_t billboard = firstBillboard; billboard < firstBillboard + billboardCount; ++billboard) {
                            const std::byte *const instance = static_cast<const std::byte *>(model->instanceMirror) + 52U * billboard;
                            const std::byte *const bound = model->instanceBounds + 16U * billboard;
                            if (ReadField<std::uint32_t>(bound, 12) != reference)
                                continue;
                            const Vector3 position = ReadField<Vector3>(instance, 0);
                            Vector3 world;
                            for (unsigned axis = 0; axis < 3; ++axis) {
                                const Vector4 &row = draw.transform.rows[axis];
                                world[axis] = row.x * position.x + row.y * position.y + row.z * position.z + row.w;
                            }
                            const float scale =
                                (2.0f * ReadField<float>(instance, 16)) / (referenced->submeshes[0].boundsMaximum.y - referenced->submeshes[0].boundsMinimum.y);
                            const float x = world.x - pass.cameraPosition.x;
                            const float y = world.y - pass.cameraPosition.y;
                            const float z = world.z - pass.cameraPosition.z;
                            const float distance = std::sqrt(x * x + y * y + z * z) / scale;
                            const float fadeValue = fade > 0.0f ? std::fmax(0.0f, std::fmin(1.0f, (distance - (threshold - fade)) / fade)) : 0.0f;
                            if (threshold <= distance || fadeValue > 0.0f) {
                                for (unsigned vertex = 0; vertex < 6; ++vertex)
                                    indices[indexCount++] = 6U * billboard + vertex;
                            }
                        }
                    }
                    if (indexCount) {
                        void *const mapped = GetWriteMappedBufferPointer(context, g_billboardBuffer, 1, 24U * firstBillboard);
                        if (mapped)
                            std::memcpy(mapped, indices, 4U * indexCount);
                        DX11_CopySceneNodeToGPU(context, g_billboardBuffer);
                    }
                    draw.firstInstance = static_cast<std::uint16_t>(billboardIndexCount);
                    draw.instanceCount = static_cast<std::uint16_t>(indexCount);
                    billboardIndexCount += indexCount;
                    RestoreGlobalPoolCheckpoint(billboardCheckpoint);
                }
            }
        }
        firstSubmesh += sourceSubmeshCount;
    }

    if (sort) {
        if (pass.materialMask & 8U)
            std::sort(order, order + count, [distances](std::uint32_t left, std::uint32_t right) { return distances[left] > distances[right]; });
        else if (pass.type != 2 && pass.type != 3 && g_modernSortByMaterial)
            std::sort(order, order + count, [materials](std::uint32_t left, std::uint32_t right) { return materials[left] < materials[right]; });
        else
            std::sort(order, order + count, [distances](std::uint32_t left, std::uint32_t right) { return distances[left] < distances[right]; });
    }
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    for (std::uint32_t index = 0; index < count; ++index) {
        const ModelGeometryDraw &draw = draws[order[index]];
        const D3TModelDrawGroup &group = draw.model->drawGroups[draw.drawGroup];
        const D3TModelSubmesh &mesh = draw.model->submeshes[group.firstSubmesh + draw.submesh];
        const std::uint32_t inputLayout = draw.model->geometry[group.geometryIndex].inputLayout;
        if (state.inputLayout != inputLayout) {
            state.inputLayout = inputLayout;
            state.pipelineStateFlags |= d3t::PipelineInputLayout;
        }
        BindGeometryBuffer(context, 0, 1, slot.skinningBuffer);
        BindGeometryBuffer(context, 0, 2, draw.model->instanceBuffer);
        BindGeometryBuffer(context, 0, 3, g_instanceBuffer);
        if (draw.model->materials[mesh.material].materialType == 5) {
            if (state.indexBufferHandle != g_billboardBuffer)
                state.pipelineStateFlags |= d3t::PipelineIndexBuffer;
            state.indexBufferHandle = g_billboardBuffer;
            state.indexFormat = 16;
        }
        CommitModelGeometryDraw(context, draw, pass);
        BindGeometryBuffer(context, 0, 1, 0);
        BindGeometryBuffer(context, 0, 2, 0);
        BindGeometryBuffer(context, 0, 3, 0);
    }
    geometry.submittedDraws = count;
    RestoreGlobalPoolCheckpoint(checkpoint);
}


void DrawRenderLayerSubmeshBounds(std::uint32_t handle, D3TDeviceContext *context, const ModernScenePass &pass, std::uint32_t color) {
#if defined(_WIN32)
    static std::uint32_t inputLayout = 0;
    if (!inputLayout) {
        D3TFVFDescription description{};
        description.elementCount = 2;
        description.elements[0] = {.semanticName = "POSITION", .format = 21, .semanticHash = d3t_HashData("POSITION", 8)};
        description.elements[1] = {.semanticName = "COLOR", .format = 2, .semanticHash = d3t_HashData("COLOR", 5)};
        inputLayout = CreateFVF(&description);
    }
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    if (state.inputLayout != inputLayout) {
        state.inputLayout = inputLayout;
        state.pipelineStateFlags |= d3t::PipelineInputLayout;
    }
    struct BoundsVertex {
        Vector3 position;
        std::uint32_t color;
    };
    BoundsVertex *const vertices = static_cast<BoundsVertex *>(GetMappedConstantBuffer(context, 0, 393216));
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(handle);
    if (!slot || !vertices)
        return;
    void *const checkpoint = GetGlobalPoolCheckpoint();
    std::uint16_t *const flags = static_cast<std::uint16_t *>(AllocateMemoryFromGlobalPool(2U * slot->entryCount));
    Matrix4x4 *const transforms = static_cast<Matrix4x4 *>(AllocateMemoryFromGlobalPool(sizeof(Matrix4x4) * slot->entryCount));
    if (!flags || !transforms) {
        RestoreGlobalPoolCheckpoint(checkpoint);
        return;
    }
    const D3TModelNode *const nodes = reinterpret_cast<const D3TModelNode *>(slot->entries);
    std::uint32_t vertexCount = 0;
    constexpr std::uint8_t corners[24]{0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7, 7, 6, 6, 4, 0, 4, 1, 5, 3, 7, 2, 6};
    for (std::uint32_t node = 0; node < slot->entryCount; ++node) {
        flags[node] = static_cast<std::uint16_t>(nodes[node].flags);
        transforms[node] = slot->matrices[node];
        if (nodes[node].parentDistance) {
            const std::uint32_t parent = node - nodes[node].parentDistance;
            transforms[node] = Compose(transforms[parent], transforms[node]);
            flags[node] |= flags[parent];
        }
        if ((flags[node] & 1U) || !nodes[node].drawGroup)
            continue;
        const D3TModelRecord *const model = d3t::ResolveModel(slot->renderHandles[node]);
        if (!model)
            continue;
        const D3TModelDrawGroup &group = model->drawGroups[nodes[node].drawGroup - 1U];
        for (std::uint32_t mesh = 0; mesh < group.submeshCount; ++mesh) {
            const D3TModelSubmesh &submesh = model->submeshes[group.firstSubmesh + mesh];
            if (vertexCount >= 24576 || !IncludesMaterial(pass.materialMask, model->materials[submesh.material].blendType))
                continue;
            Vector3 minimum = submesh.boundsMinimum;
            Vector3 maximum = submesh.boundsMaximum;
            TransformBounds(minimum, maximum, transforms[node]);
            for (const std::uint8_t corner : corners) {
                vertices[vertexCount++] = {
                    .position = {(corner & 1U) ? maximum.x : minimum.x, (corner & 2U) ? maximum.y : minimum.y, (corner & 4U) ? maximum.z : minimum.z},
                    .color = color};
            }
        }
    }
    RestoreGlobalPoolCheckpoint(checkpoint);
    if (vertexCount) {
        BindConstantBuffer(context, 1, 1, g_perObjectBuffer);
        SetTopology(context, static_cast<std::uint32_t>(d3t::TopologyType::LineList));
        CommitGeometryState(context);
        ID3D11DeviceContext *const native = static_cast<ID3D11DeviceContext *>(context->deviceContext);
        if (native)
            native->Draw(vertexCount, 0);
        SetTopology(context, static_cast<std::uint32_t>(d3t::TopologyType::TriangleList));
    }
#else
    static_cast<void>(handle);
    static_cast<void>(context);
    static_cast<void>(pass);
    static_cast<void>(color);
#endif
}

} // namespace shenmue::s1
