#include "s1/modern_lighting.hpp"

#include "d3t/content.hpp"
#include "d3t/render_handles.hpp"
#include "s1/boot.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/lighting.hpp"
#include "s1/render_array.hpp"
#include "s1/simple_shader.hpp"
#include "s1/skinning_cache.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <shared_mutex>

namespace shenmue::s1 {

std::array<ModernSceneLight, 128> g_modernSceneLights{};
std::array<ModernGpuLight, 16> g_modernUploadedLights{};
ModernLightConstants g_modernLightConstants{};
std::uint32_t g_modernUploadedLightCount = 0;
Vector3 g_modernAmbientColour{};
SceneLightDataHeader g_sceneLightDataHeader{};
std::uint32_t g_modernSceneId = 0;
shen_name g_modernSceneName{};
std::int32_t g_modernDebugRenderMode = 0;
std::int32_t g_modernAlternateNodes = 0;
SceneLightMetadataTable g_sceneLightMetadata{};
extern std::int32_t (*g_getAspectRatioModeFunction)();

namespace {

std::shared_mutex g_sceneLightMutex;
bool g_singleLightOverrideArmed = false;
ModernGpuLight g_singleLightOverride{};
std::array<std::uint32_t, 16> g_directionalLightIndices{};

bool InsertSceneLightMetadata(SceneLightMetadataTable &table, std::uint64_t value);

bool ResizeSceneLightMetadata(SceneLightMetadataTable &table, std::uint32_t capacity) {
    SceneLightMetadataTable replacement{.values = static_cast<std::uint64_t *>(std::calloc(capacity, sizeof(std::uint64_t))),
        .hashes = static_cast<std::uint32_t *>(std::calloc(capacity, sizeof(std::uint32_t))),
        .links = static_cast<std::uint16_t *>(std::calloc(capacity, sizeof(std::uint16_t))),
        .capacity = capacity,
        .count = 0};
    if (!replacement.values || !replacement.hashes || !replacement.links) {
        std::free(replacement.values);
        std::free(replacement.hashes);
        std::free(replacement.links);
        return false;
    }
    for (std::uint32_t index = 0; index < table.capacity; ++index)
        if (table.hashes[index])
            InsertSceneLightMetadata(replacement, table.values[index]);
    std::free(table.values);
    std::free(table.hashes);
    std::free(table.links);
    table = replacement;
    return true;
}

bool InsertSceneLightMetadata(SceneLightMetadataTable &table, std::uint64_t value) {
    if (!table.capacity || table.count >= table.capacity - (table.capacity >> 2U))
        if (!ResizeSceneLightMetadata(table, table.capacity >= 64 ? table.capacity + (table.capacity >> 2U) : 64))
            return false;
    const std::uint32_t hash = static_cast<std::uint32_t>(value >> 32U);
    std::uint32_t slot = hash % table.capacity;
    std::uint32_t empty = UINT32_MAX;
    for (;;) {
        if (table.hashes[slot] == hash)
            return true;
        if (!table.hashes[slot]) {
            if (empty == UINT32_MAX)
                empty = slot;
            if (!table.links[slot]) {
                table.values[slot] = value;
                table.hashes[slot] = hash;
                ++table.count;
                return true;
            }
        }
        if (!table.links[slot])
            break;
        slot = (slot + table.links[slot]) % table.capacity;
    }
    if (empty == UINT32_MAX) {
        empty = (slot + 1U) % table.capacity;
        while (table.hashes[empty]) {
            if (empty == slot)
                return false;
            empty = (empty + 1U) % table.capacity;
        }
        const std::uint32_t distance = empty >= slot ? empty - slot : empty + table.capacity - slot;
        table.links[slot] = static_cast<std::uint16_t>(distance);
    }
    table.values[empty] = value;
    table.hashes[empty] = hash;
    ++table.count;
    return true;
}

void BindModernBuffer(D3TDeviceContext *context, std::uint32_t stages, std::uint32_t slot, std::uint32_t handle) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    for (std::uint32_t stage = 0; stage < 3; ++stage) {
        if (!(stages & (1U << stage)))
            continue;
        std::uint32_t &previous = *reinterpret_cast<std::uint32_t *>(context->renderState.data() + 0x734 + (stage * 8 + slot) * 4);
        if (previous != handle) {
            previous = handle;
            state.shaderResourceDirtyMasks[stage] |= 1U << slot;
        }
    }
}

void BindModernTexture(D3TDeviceContext *context, std::uint32_t slot, std::uint32_t handle) {
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    std::uint32_t &previous = *reinterpret_cast<std::uint32_t *>(context->renderState.data() + 0x1b4 + slot * 4);
    std::uint32_t &flags = *reinterpret_cast<std::uint32_t *>(context->renderState.data() + 0x334 + slot * 4);
    if (previous != handle || flags) {
        previous = handle;
        flags = 0;
        state.shaderResourceDirtyMasks[1] |= 1U << slot;
    }
}

void PrepareTiledLightLists(D3TDeviceContext *context) {
    std::array<std::uint32_t, 50> points{};
    std::array<std::uint32_t, 50> spots{};
    std::uint16_t pointCount = 0;
    std::uint16_t spotCount = 0;

    for (std::uint32_t index = 0; index < g_modernUploadedLightCount; ++index) {
        const ModernGpuLight &light = g_modernUploadedLights[index];
        const float magnitude = (light.colour.x * light.colour.x + light.colour.y * light.colour.y) + light.colour.z * light.colour.z;
        if (!(magnitude >= 0.0000099999997f))
            continue;
        if (light.type == 0)
            points[pointCount++] = index;
        else if (light.type == 2)
            spots[spotCount++] = index;
    }
    ModernTileOffset *const offsets =
        static_cast<ModernTileOffset *>(d3t::GetWriteMappedBufferPointer(context, static_cast<std::int32_t>(g_simpleVertexBufferHandles[2]), 1, 0));
    if (offsets) {
        std::uint32_t cursor = 0;
        for (std::uint32_t tile = 0; tile < 8000; ++tile) {
            offsets[tile] = {.indexOffset = cursor, .spotCount = spotCount, .pointCount = pointCount, .probeCount = 0};
            cursor += pointCount + spotCount;
        }
    }
    d3t::DX11_CopySceneNodeToGPU(context, static_cast<std::int32_t>(g_simpleVertexBufferHandles[2]));
    std::uint32_t *const indices =
        static_cast<std::uint32_t *>(d3t::GetWriteMappedBufferPointer(context, static_cast<std::int32_t>(g_simpleIndexBufferHandles[2]), 1, 0));
    if (indices) {
        std::uint32_t cursor = 0;
        for (std::uint32_t tile = 0; tile < 8000; ++tile) {
            for (std::uint32_t index = 0; index < pointCount; ++index)
                indices[cursor++] = points[index];
            for (std::uint32_t index = 0; index < spotCount; ++index)
                indices[cursor++] = spots[index];
        }
    }
    d3t::DX11_CopySceneNodeToGPU(context, static_cast<std::int32_t>(g_simpleIndexBufferHandles[2]));
}


void UpdateModernShadowPassBindings(D3TDeviceContext *context) {
    BindModernBuffer(context, 2, 5, g_simpleVertexBufferHandles[2]);
    BindModernBuffer(context, 2, 6, g_simpleIndexBufferHandles[2]);
    BindModernBuffer(context, 3, 0, g_simpleShaderTemporaryBuffer);
#if defined(_WIN32)
    ModernLightConstants *const constants =
        static_cast<ModernLightConstants *>(MapTempConstantBufferForWriteDiscard(context, 3, 6, sizeof(ModernLightConstants)));
    if (constants) {
        *constants = g_modernLightConstants;
        constants->noShadows = 1;
    }
#endif
    std::uint32_t texture = d3t::g_defaultWhiteTexture;
    for (std::uint32_t index = 0; index < g_simpleShaderMode; ++index) {
        std::uint32_t candidate = 0;
        std::memcpy(&candidate, g_simpleShaderParameterBlocks[index].data.data() + 60, sizeof(candidate));
        if (candidate) {
            texture = candidate;
            break;
        }
    }
    BindModernTexture(context, 31, texture);
}

} // namespace


void ExtractAndNormalizeSixFrustumPlanes(std::array<Vector4, 6> &planes, const Matrix4x4 &matrix) {
    const Vector4 x = matrix.rows[0];
    const Vector4 y = matrix.rows[1];
    const Vector4 z = matrix.rows[2];
    const Vector4 w = matrix.rows[3];
    planes = {{z, {w.x - z.x, w.y - z.y, w.z - z.z, w.w - z.w}, {x.x + w.x, x.y + w.y, x.z + w.z, x.w + w.w}, {w.x - x.x, w.y - x.y, w.z - x.z, w.w - x.w},
        {w.x - y.x, w.y - y.y, w.z - y.z, w.w - y.w}, {y.x + w.x, y.y + w.y, y.z + w.z, y.w + w.w}}};
    for (Vector4 &plane : planes) {
        const float length = std::sqrt(((plane.x * plane.x + plane.y * plane.y) + plane.z * plane.z) + plane.w * plane.w);
        const float inverse = 1.0f / length;
        plane = {plane.x * inverse, plane.y * inverse, plane.z * inverse, plane.w * inverse};
    }
}

void BindSceneFallbackTextureAndHighSamplers(D3TDeviceContext *context) {
    BindModernTexture(context, 27, d3t::g_defaultWhiteTexture);
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    constexpr std::uint64_t samplerFlags[2] = {7808, 7829};
    for (std::uint32_t index = 0; index < 2; ++index) {
        std::uint64_t *const descriptor = reinterpret_cast<std::uint64_t *>(context->renderState.data() + 0x534 + (12 + index) * 16);
        if (descriptor[0] != samplerFlags[index]) {
            descriptor[0] = samplerFlags[index];
            descriptor[1] = 0;
            state.samplerDirtyMasks[1] |= static_cast<std::uint16_t>(1U << (12 + index));
        }
    }
}



void PrepareTiledSceneLightingAndShadowBuffers(D3TDeviceContext *context, const Matrix4x4 &view) {
    static_cast<void>(view);
    g_modernUploadedLights = {};
    g_modernUploadedLightCount = 0;
    if (g_singleLightOverrideArmed) {
        g_modernUploadedLights[0] = g_singleLightOverride;
        g_modernUploadedLightCount = 1;
        g_singleLightOverrideArmed = false;
    } else {
        for (const ModernSceneLight &source : g_modernSceneLights) {
            if (g_modernUploadedLightCount >= 16)
                break;
            if (!source.enabled)
                continue;
            ModernGpuLight &light = g_modernUploadedLights[g_modernUploadedLightCount++];
            light.colour = source.colour;
            light.type = source.type;
            light.position = source.position;
            light.direction = source.direction;
            light.range = source.type ? source.range : 5.0f * source.range;
            light.cone = source.cone;
        }
    }
    PrepareTiledLightLists(context);
    std::uint32_t directionalCount = 0;
    for (std::uint32_t index = 0; index < g_modernUploadedLightCount; ++index)
        if (g_modernUploadedLights[index].type == 1)
            g_directionalLightIndices[directionalCount++] = index;
    for (std::uint32_t index = 0; index < 4; ++index)
        g_modernLightConstants.directionalIndices[index] = g_directionalLightIndices[index];
    g_modernLightConstants.directionalCount = directionalCount;
    for (Vector4 &ambient : g_modernLightConstants.ambient)
        ambient = {g_modernAmbientColour.x, g_modernAmbientColour.y, g_modernAmbientColour.z, 1.0f};
    void *const mapped = d3t::GetWriteMappedBufferPointer(context, static_cast<std::int32_t>(g_simpleShaderTemporaryBuffer), 1, 0);
    if (mapped)
        std::memcpy(mapped, g_modernUploadedLights.data(), sizeof(g_modernUploadedLights));
    d3t::DX11_CopySceneNodeToGPU(context, static_cast<std::int32_t>(g_simpleShaderTemporaryBuffer));
    UpdateModernShadowPassBindings(context);
}



void ConfigureAndSubmitModernScenePass(D3TDeviceContext *context, ModernScenePass &pass, RenderLayerGeometryContext &geometry) {
    if (g_perspectiveProjection == 4)
        return;
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    std::uint32_t rasterizer = (pass.reverseCulling == 0) | (2U * ((g_modernDebugRenderMode == 6) | (pass.type == 1 ? 4100U : 4104U)));
    if (g_getAspectRatioModeFunction && g_getAspectRatioModeFunction() && !pass.disableAspectCulling && !(pass.type & ~2U))
        rasterizer |= 4U;
    if (state.rasterizerState != rasterizer) {
        state.rasterizerState = rasterizer;
        state.pipelineStateFlags |= d3t::PipelineRasterizer;
    }
    const bool transparent = (pass.materialMask & 8U) != 0;
    const std::uint64_t depthFlags = !pass.deferred || transparent ? (transparent ? 0x15U : 0x17U) : 13U;
    std::uint64_t previousDepth = 0;
    std::memcpy(&previousDepth, state.depthStencilState.data(), sizeof(previousDepth));
    if (previousDepth != depthFlags || state.stencilReference) {
        std::memcpy(state.depthStencilState.data(), &depthFlags, sizeof(depthFlags));
        std::memset(state.depthStencilState.data() + 8, 0, 8);
        state.stencilReference = 0;
        state.pipelineStateFlags |= d3t::PipelineDepthStencil;
    }
    const std::uint64_t blend = transparent ? 0xf0000285U : 0xf0000000U;
    std::uint32_t previousBlend = 0;
    std::memcpy(&previousBlend, state.blendState.data(), sizeof(previousBlend));
    if (previousBlend != static_cast<std::uint32_t>(blend))
        state.pipelineStateFlags |= d3t::PipelineBlend;
    std::memcpy(state.blendState.data(), &blend, sizeof(blend));
#if defined(_WIN32)
    float *const depth = static_cast<float *>(MapTempConstantBufferForWriteDiscard(context, 2, 12, 20));
    if (depth) {
        depth[0] = 450.00003f;
        depth[1] = 2999.8501f;
        depth[2] = 3000.0f;
        depth[3] = 1.0f;
        depth[4] = 5.0f;
    }
#endif
    RenderLayerVisibility &visibility = *static_cast<RenderLayerVisibility *>(AllocateGeometryFrameMemory(sizeof(RenderLayerVisibility)));
    visibility = {.geometry = &geometry, .nodeMask = nullptr, .submeshMask = nullptr, .viewportWidth = state.viewport[2], .reserved = 0};
    if (g_modernNodeCulling)
        CullRenderLayerNodesAndSubmeshes(visibility, pass);
    if (!pass.type || pass.type == 6 || pass.type == 7)
        pass.alternateShader = 1;
    if (!IsPostProcessPass())
        pass.disableVelocity = 1;
    pass.clusterNear = 0.15000001f;
    pass.clusterFar = 3000.0f;
    pass.alternateNodes = g_modernAlternateNodes == 1;
    SubmitRenderLayerGeometry(visibility, context, pass);
    if (g_modernDebugBounds) {
        const std::uint32_t color = (pass.materialMask & 2U) ? 0xff0000ffU :
            (pass.materialMask & 4U)                         ? 0xffff0000U :
            (pass.materialMask & 8U)                         ? 0xff00ff00U :
                                                               0xffffffffU;
        const std::uint32_t vertex = d3t_GetLoadedShader(g_simpleShaderLoad, 0);
        const std::uint32_t pixel = d3t_GetLoadedShader(g_simpleShaderLoad, 1);
        if (state.vertexShader != vertex) {
            state.pipelineStateFlags |= d3t::PipelineInputLayout | d3t::PipelineVertexShader;
            state.vertexBufferDirtyMask = UINT32_MAX;
        }
        state.vertexShader = vertex;
        if (state.pixelShader != pixel)
            state.pipelineStateFlags |= d3t::PipelinePixelShader;
        state.pixelShader = pixel;
        DrawRenderLayerSubmeshBounds(static_cast<std::uint32_t>(g_mainSkinningRoot), context, pass, color);
    }
    visibility = {};
}


void ArmSingleLightOverrideForCachedMapModel(Mt5RuntimeNode *model) {
    if (!ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(model) + 1U))
        return;
    g_singleLightOverrideArmed = true;
    g_singleLightOverride = {};
    const EffectLightRenderSlot &source = g_effectLightRenderSlots[31];
    g_singleLightOverride.colour = {source.shaderConstants04[5], source.shaderConstants04[6], source.shaderConstants04[7]};
    g_singleLightOverride.direction = {source.shaderConstants5C[3], source.shaderConstants5C[4], source.shaderConstants5C[5]};
    g_singleLightOverride.type = 1;
}

void InitializeModernSceneLightMetadata() {
    const std::unique_lock<std::shared_mutex> lock(g_sceneLightMutex);
    if (!g_sceneLightMetadata.capacity)
        ResizeSceneLightMetadata(g_sceneLightMetadata, 16);
}

void ReleaseModernSceneLightMetadata() {
    const std::unique_lock<std::shared_mutex> lock(g_sceneLightMutex);
    std::free(g_sceneLightMetadata.values);
    std::free(g_sceneLightMetadata.hashes);
    std::free(g_sceneLightMetadata.links);
    g_sceneLightMetadata = {};
}

void CompleteSceneLightDataLoad(void *callbackData, std::uint32_t taskHandle, void *data, std::uint64_t size, const char *path) {
    static_cast<void>(callbackData);
    static_cast<void>(size);
    static_cast<void>(path);
    const std::unique_lock<std::shared_mutex> lock(g_sceneLightMutex);
    if (g_sceneLightMetadata.count) {
        std::memset(g_sceneLightMetadata.values, 0, g_sceneLightMetadata.capacity * sizeof(std::uint64_t));
        std::memset(g_sceneLightMetadata.hashes, 0, g_sceneLightMetadata.capacity * sizeof(std::uint32_t));
        std::memset(g_sceneLightMetadata.links, 0, g_sceneLightMetadata.capacity * sizeof(std::uint16_t));
        g_sceneLightMetadata.count = 0;
    }
    if (data) {
        std::memcpy(&g_sceneLightDataHeader, data, sizeof(g_sceneLightDataHeader));
        if (g_sceneLightDataHeader.version == 1) {
            const std::byte *const records = static_cast<const std::byte *>(data) + g_sceneLightDataHeader.recordsOffset;
            for (std::uint32_t index = 0; index < g_sceneLightDataHeader.count; ++index) {
                std::uint64_t value = 0;
                std::memcpy(&value, records + index * g_sceneLightDataHeader.stride, sizeof(value));
                if (value >> 32U)
                    InsertSceneLightMetadata(g_sceneLightMetadata, value);
            }
        }
    }
    d3t::ProcessThreadStates(taskHandle, 1);
}

std::int64_t SetNewSceneDetails(std::uint32_t sceneId, shen_name area) {
    {
        const std::unique_lock<std::shared_mutex> lock(g_sceneLightMutex);
        g_modernSceneId = sceneId;
        g_modernSceneName = area;
    }
    const std::uint32_t renderHandle = d3t::d3t_AllocateRenderHandle();
    char path[272];
    {
        const std::shared_lock<std::shared_mutex> lock(g_sceneLightMutex);
        const char *const name = g_modernSceneName.characters;
        std::snprintf(path, sizeof(path), "/SceneMetaData/%c%c%c%c/SDLightMetaData_%c%c%c%c.sdlightdata", name[0], name[1], name[2], name[3], name[0], name[1],
            name[2], name[3]);
    }
    const d3t::D3TShaderLoadTask task{.path = path,
        .renderHandle = renderHandle,
        .reserved_0c = 0,
        .thread = {.callback = CompleteSceneLightDataLoad, .data = nullptr},
        .destination = nullptr,
        .destinationCapacity = 0,
        .option = 0,
        .reserved_31 = {}};
    return d3t::PushShaderLoadToThreadQueue(&task);
}

} // namespace shenmue::s1
