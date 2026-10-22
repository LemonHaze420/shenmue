#include "s2/sd_light.hpp"

#include "s2/simple_shader.hpp"
#include "d3t/light_tiles.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/texture.hpp"

#include "d3t/content.hpp"
#include "d3t/model_cache.hpp"
#include "d3t/render_handles.hpp"
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>

    namespace shenmue::s2 {

SDLightDataHeader g_sdLightDataHeader{};
SDLightMetadataTable g_sdLightMetadata{};
SDLightOverrides g_sdLightOverrides{};
std::int32_t g_sdLightSceneId{};
shen_name g_sdLightArea{};
std::uint32_t g_sdLightCount{};

std::array<SDLightRecord, 512> g_sdLights{};
std::array<std::uint8_t, 32> g_sdShadowSlots{};
std::array<std::uint8_t, 32> g_sdShadowDirty{};
SDShadowMapping g_sdShadowMapping{};

std::int32_t g_sdDedicatedShadowSize = 1024;
void InitializeSDLightNodes() {
    g_sdShadowSlots.fill(0);
    g_sdShadowDirty.fill(0);
    SDLightRecord &sun = g_sdLights[0];
    sun.colour = {1.0f, 1.0f, 1.0f};
    sun.position = {};
    sun.direction = {-g_sdSunDirection.x, -g_sdSunDirection.y, -g_sdSunDirection.z};
    SDLightRecord &fill = g_sdLights[1];
    fill.colour = sun.colour;
    fill.position = sun.position;
    fill.direction = g_sdSunDirection;
    sun.type = fill.type = SDLT_Directional;
    sun.shadowMode = 0;
    fill.shadowMode = 1;
    sun.shadowBias = fill.shadowBias = 0.002f;
    sun.parameter104 = 0.25f;
    fill.parameter104 = 3.5f;
    sun.parameter108 = fill.parameter108 = 0.0f;
    sun.parameter128 = fill.parameter128 = 0;
    g_sdLightCount = 2;
    for (SDLightRecord &light : g_sdLights)
        light.parameterEnabled = 1;
}

// @verified - p
void BindModelSceneLights(d3t::D3TDeviceContext * context) {
    d3t::BindLightTileBuffers(context);
    d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    for (std::uint32_t stage = 0; stage < 2; ++stage)
        if (state.bindings.bufferHandles[stage][0] != g_sdLightBuffer) {
            state.shaderResourceDirtyMasks[stage] |= 1U;
            state.bindings.bufferHandles[stage][0] = g_sdLightBuffer;
        }
#if defined(_WIN32)
    SDLightConstants *const constants = static_cast<SDLightConstants *>(d3t::MapTempConstantBufferForWriteDiscard(context, 3, 6, 2164));
    if (constants) {
        std::memcpy(constants, &g_sdLightConstants, 2164);
        constants->noShadows = 1;
    }
#endif
    std::uint32_t texture = d3t::g_defaultWhiteTexture;
    for (std::uint32_t index = 0; index < g_sdLightCount; ++index)
        if (g_sdLights[index].textureHandle) {
            texture = g_sdLights[index].textureHandle;
            break;
        }
    if (state.bindings.textureHandles[1][31] != texture || state.bindings.textureFlags[1][31] != 0) {
        state.shaderResourceDirtyMasks[1] |= 0x80000000U;
        state.bindings.textureHandles[1][31] = texture;
        state.bindings.textureFlags[1][31] = 0;
    }
}
std::uint32_t AllocateSDShadowSlots(std::uint32_t count) {
    std::uint32_t freeCount = 0;
    for (std::uint32_t index = 0; index < 32; ++index) {
        if (g_sdShadowSlots[index])
            freeCount = 0;
        else if (++freeCount == count) {
            const std::uint32_t first = index - count + 1;
            for (std::uint32_t slot = first; slot < first + count; ++slot) {
                g_sdShadowSlots[slot] = 1;
                g_sdShadowDirty[slot] = 1;
            }
            return first;
        }
    }
    return 32;
}

namespace {
// @verified - p
const int g_sdShadowMappingCleanup = std::atexit(ReleaseSDShadowMapping);

// @verified - p
SDShadowMappingNode *AppendSDShadowMappingNode(SDShadowMapping &mapping) {
    if (mapping.nodeCount >= mapping.nodeCapacity) {
        const std::uint32_t capacity = 2U * mapping.nodeCapacity >= 16U ? 2U * mapping.nodeCapacity : 16U;
        if (capacity > mapping.nodeCapacity) {
            void *const allocation = std::realloc(mapping.nodes, sizeof(SDShadowMappingNode) * capacity);
            if (!allocation)
                return nullptr;
            mapping.nodes = static_cast<SDShadowMappingNode *>(allocation);
            mapping.nodeCapacity = capacity;
        }
    }
    return mapping.nodes + mapping.nodeCount++;
}

// @verified - p
SDShadowMappingRequest *AppendSDShadowMappingRequest(SDShadowMapping &mapping) {
    if (mapping.requestCount >= mapping.requestCapacity) {
        const std::uint32_t capacity = 2U * mapping.requestCapacity >= 16U ? 2U * mapping.requestCapacity : 16U;
        if (capacity > mapping.requestCapacity) {
            void *const allocation = std::realloc(mapping.requests, sizeof(SDShadowMappingRequest) * capacity);
            if (!allocation)
                return nullptr;
            mapping.requests = static_cast<SDShadowMappingRequest *>(allocation);
            mapping.requestCapacity = capacity;
        }
    }
    return mapping.requests + mapping.requestCount++;
}
// @verified - p
SDShadowMappingNode *InsertSDShadowMappingNode(SDShadowMapping &mapping, const SDShadowMappingRequest &request, SDShadowMappingNode *node) {
    for (;;) {
        while (node->children[0] || node->children[1]) {
            SDShadowMappingNode *const result = InsertSDShadowMappingNode(mapping, request, node->children[0]);
            if (result)
                return result;
            node = node->children[1];
        }
        if (node->output || node->width < request.width || node->height < request.height)
            return nullptr;
        if (node->width == request.width && node->height == request.height)
            return node;
        const std::int32_t x = node->x, y = node->y, width = node->width, height = node->height;
        SDShadowMappingNode **const second = &node->children[1];
        SDShadowMappingNode *const first = AppendSDShadowMappingNode(mapping);
        node->children[0] = first;
        node = first;
        SDShadowMappingNode *const other = AppendSDShadowMappingNode(mapping);
        *second = other;
        *node = {};
        *other = {};
        if (width - request.width <= height - request.height) {
            node->x = x;
            node->y = y;
            node->width = width;
            node->height = request.height;
            other->x = x;
            other->y = y + request.height;
            other->width = width;
            other->height = height - request.height;
        } else {
            node->x = x;
            node->y = y;
            node->width = request.width;
            node->height = height;
            other->x = x + request.width;
            other->y = y;
            other->width = width - request.width;
            other->height = height;
        }
    }
}

// @verified - p
void ComputeSDShadowAxisRatios(const SDShadowMappingNode &node, Vector2 &output) {
    output.x = static_cast<float>(node.x / node.width);
    output.y = static_cast<float>(node.y / node.height);
}

bool PackSDShadowMappingRequests() {
    SDShadowMapping &mapping = g_sdShadowMapping;
    for (std::uint32_t index = 1; index < mapping.requestCount; ++index) {
        const SDShadowMappingRequest request = mapping.requests[index];
        std::uint32_t destination = index;
        while (destination && request.width > mapping.requests[destination - 1].width) {
            mapping.requests[destination] = mapping.requests[destination - 1];
            --destination;
        }
        mapping.requests[destination] = request;
    }
    bool result = true;
    SDShadowMappingNode *const root = mapping.nodes;
    for (std::uint32_t index = 0; index < mapping.requestCount; ++index) {
        SDShadowMappingRequest &request = mapping.requests[index];
        if (request.processed)
            continue;
        request.processed = 1;
        SDShadowMappingNode *const node = InsertSDShadowMappingNode(mapping, request, root);
        if (node) {
            node->output = request.output;
            node->slot = request.slot;
            ComputeSDShadowAxisRatios(*node, *request.output);
        } else {
            const SDShadowMappingNode fallback{.children = {}, .x = -1, .y = -1, .width = -1, .height = -1, .slot = 0, .reserved24 = 0, .output = nullptr};
            ComputeSDShadowAxisRatios(fallback, *request.output);
            result = false;
        }
    }
    return result;
}

// @verified - p
void InsetSDShadowRectangle(const Vector2 &padding, const Vector2 &size, Vector2 &offset, Vector2 &minimum, Vector2 &maximum) {
    offset.x = size.x * offset.x;
    offset.y = size.y * offset.y;
    minimum = {padding.x + offset.x, padding.y + offset.y};
    maximum = {(offset.x + size.x) - padding.x, (offset.y + size.y) - padding.y};
}

// @verified - p
void ClearSDShadowMappingNode(SDShadowMapping &mapping, SDShadowMappingNode &node, std::uint32_t slot) {
    if (node.children[0])
        ClearSDShadowMappingNode(mapping, *node.children[0], slot);
    if (node.children[1])
        ClearSDShadowMappingNode(mapping, *node.children[1], slot);
    if (node.slot == slot)
        for (std::uint32_t index = 0; index < mapping.nodeCount; ++index)
            if (mapping.nodes[index].slot == slot)
                node.output = nullptr;
}
} // namespace

// @verified - p
void ReleaseSDShadowMapping() {
    std::free(g_sdShadowMapping.nodes);
    g_sdShadowMapping.nodes = nullptr;
    g_sdShadowMapping.nodeCount = g_sdShadowMapping.nodeCapacity = 0;
    std::free(g_sdShadowMapping.requests);
    g_sdShadowMapping.requests = nullptr;
    g_sdShadowMapping.requestCount = g_sdShadowMapping.requestCapacity = 0;
}

// @verified - p
bool AllocateSDShadowBinding(SDShadowBinding &binding) {
    const std::int32_t pixels = g_sdDedicatedShadowSize;
    const float size = static_cast<float>(static_cast<std::uint32_t>(pixels)) / g_sdShadowAtlasSize.x;
    const std::uint32_t slot = AllocateSDShadowSlots(1);
    if (slot == 32)
        return false;
    binding.size = {size, size};
    binding.slot = slot;
    SDShadowMappingRequest *const request = AppendSDShadowMappingRequest(g_sdShadowMapping);
    if (request) {
        request->processed = 0;
        request->slot = slot;
        request->x = request->y = 0;
        request->width = request->height = pixels;
        request->output = &binding.offset;
    }
    if (!PackSDShadowMappingRequests())
        return false;
    g_sdShadowDirty[slot] = 1;
    const Vector2 padding{(1.0f / g_sdShadowAtlasSize.x) * 2.0f, (1.0f / g_sdShadowAtlasSize.y) * 2.0f};
    InsetSDShadowRectangle(padding, binding.size, binding.offset, binding.minimum, binding.maximum);
    return true;
} // namespace

// @verified - p
void RemoveSDShadowMappingRequest(std::uint32_t slot) {
    SDShadowMapping &mapping = g_sdShadowMapping;
    for (std::uint32_t index = 0; index < mapping.requestCount; ++index) {
        if (mapping.requests[index].slot != slot)
            continue;
        SDShadowMappingNode &root = *mapping.nodes;
        if (root.children[0])
            ClearSDShadowMappingNode(mapping, *root.children[0], slot);
        if (root.children[1])
            ClearSDShadowMappingNode(mapping, *root.children[1], slot);
        if (root.slot == slot) {
            for (std::uint32_t node = 0; node < mapping.nodeCount; ++node)
                if (mapping.nodes[node].slot == slot)
                    root.output = nullptr;
        }
        std::memmove(mapping.requests + index, mapping.requests + mapping.requestCount - 1, sizeof(SDShadowMappingRequest));
        --mapping.requestCount;
        return;
    }
}

// @verified - p
void ReleaseSDLightShadowSlots(SDLightRecord &light) {
    std::uint32_t count;
    switch (light.type) {
    case SDLT_Directional:
        count = 4;
        break;
    case SDLT_Spot:
        count = light.textureHandle ? 0 : 1;
        break;
    case SDLT_Point:
        count = 6;
        break;
    default:
        return;
    }
    const std::uint32_t end = light.shadowIndex + count;
    for (std::uint32_t index = light.shadowIndex; index < end; ++index) {
        if (index >= 32)
            std::abort();
        g_sdShadowSlots[index] = 0;
    }
    if (light.type == SDLT_Spot)
        RemoveSDShadowMappingRequest(light.shadowIndex);
    else
        for (std::uint32_t index = 0; index < count; ++index)
            RemoveSDShadowMappingRequest(light.shadowIndex + index);
}

// @verified - p
void RemoveSDLightBinding(std::uint32_t index) {
    if (index < 2 || index >= g_sdLightCount)
        return;
    SDLightRecord &light = g_sdLights[index];
    if (light.shadowMode)
        ReleaseSDLightShadowSlots(light);
    d3t::RemoveModelCacheSubtree(static_cast<std::uint64_t>(light.nodeHash) << 32U | light.nodeCache);
    light.ownerCache = light.ownerNode = light.nodeCache = light.nodeHash = 0;
    if (index < g_sdLightCount - 1)
        std::memmove(&light, &g_sdLights[index + 1], sizeof(SDLightRecord) * (g_sdLightCount - index - 1));
    --g_sdLightCount;
}

// @verified - p
void RemoveSDLightOwner(std::uint64_t owner) {
    std::uint32_t index = 0;
    while (index < g_sdLightCount) {
        const SDLightRecord &light = g_sdLights[index];
        if (light.ownerCache == static_cast<std::uint32_t>(owner) && light.ownerNode == static_cast<std::uint32_t>(owner >> 32U))
            RemoveSDLightBinding(index);
        else
            ++index;
    }
}
namespace {
std::atomic<std::int32_t> sdLightReaderCount{};
std::atomic<std::uint8_t> sdLightWriteLock{};

std::uint64_t *AppendSDLightOverride() {
    SDLightOverrides &overrides = g_sdLightOverrides;
    if (overrides.count >= overrides.capacity) {
        const std::uint32_t capacity = 2U * overrides.capacity >= 16U ? 2U * overrides.capacity : 16U;
        if (capacity > overrides.capacity) {
            void *const allocation = std::realloc(overrides.values, sizeof(std::uint64_t) * capacity);
            if (!allocation)
                return nullptr;
            overrides.values = static_cast<std::uint64_t *>(allocation);
            overrides.capacity = capacity;
        }
    }
    ++overrides.count;
    return overrides.values + overrides.count - 1;
}
void AcquireSDLightWrite() {
    for (;;) {
        while (sdLightReaderCount.load() != 0) {
        }
        std::uint8_t expected = 0;
        if (!sdLightWriteLock.compare_exchange_strong(expected, 1))
            continue;
        if (sdLightReaderCount.load() == 0)
            return;
        sdLightWriteLock.store(0);
    }
}

bool InsertSDLightMetadata(SDLightMetadataTable &table, std::uint64_t value, std::uint32_t hash);

// @verified - p
bool ResizeSDLightMetadata(SDLightMetadataTable &table, std::uint32_t capacity) {
    SDLightMetadataTable replacement{.values = static_cast<std::uint64_t *>(std::malloc(8ULL * capacity)),
        .hashes = static_cast<std::uint32_t *>(std::malloc(4ULL * capacity)),
        .links = static_cast<std::uint16_t *>(std::malloc(2ULL * capacity)),
        .capacity = capacity,
        .count = 0};
    if (!replacement.values || !replacement.hashes) {
        std::free(replacement.values);
        std::free(replacement.hashes);
        std::free(replacement.links);
        return false;
    }
    std::memset(replacement.values, 0, 8ULL * capacity);
    std::memset(replacement.hashes, 0, 4ULL * capacity);
    std::memset(replacement.links, 0, 2ULL * capacity);
    if (table.values && table.hashes) {
        for (std::uint32_t index = 0; index < table.capacity; ++index)
            if (table.hashes[index])
                InsertSDLightMetadata(replacement, table.values[index], table.hashes[index]);
        std::free(table.values);
        std::free(table.hashes);
        std::free(table.links);
    }
    table.values = replacement.values;
    table.hashes = replacement.hashes;
    table.links = replacement.links;
    table.capacity = replacement.capacity;
    return true;
}

// @verified - p
bool InsertSDLightMetadata(SDLightMetadataTable &table, std::uint64_t value, std::uint32_t hash) {
    if (!table.capacity || table.count >= table.capacity - (table.capacity >> 2U))
        if (!ResizeSDLightMetadata(table, table.capacity >= 64 ? table.capacity + (table.capacity >> 2U) : 64))
            return false;
    std::uint32_t slot = hash % table.capacity;
    std::uint32_t empty = UINT32_MAX;
    for (;;) {
        if (table.hashes[slot] == hash)
            return true;
        if (!table.hashes[slot]) {
            if (empty == UINT32_MAX)
                empty = slot;
            if (!table.links[slot]) {
                ++table.count;
                table.hashes[slot] = hash;
                table.values[slot] = value;
                return true;
            }
        }
        if (!table.links[slot])
            break;
        slot = (slot + table.links[slot]) % table.capacity;
    }
    if (empty == UINT32_MAX) {
        empty = (slot + 1U) % table.capacity;
        while (empty != slot && table.hashes[empty])
            empty = (empty + 1U) % table.capacity;
        if (empty == slot)
            return false;
        table.links[slot] = static_cast<std::uint16_t>(empty >= slot ? empty - slot : empty + table.capacity - slot);
    }
    ++table.count;
    table.hashes[empty] = hash;
    table.values[empty] = value;
    return true;
}
} // namespace

void InitializeSDLightMetadata() {
    AcquireSDLightWrite();
    if (!g_sdLightMetadata.capacity && !g_sdLightMetadata.count)
        ResizeSDLightMetadata(g_sdLightMetadata, 512);
    sdLightWriteLock.store(0);
}

void ReleaseSDLightMetadata() {
    AcquireSDLightWrite();
    std::free(g_sdLightMetadata.values);
    std::free(g_sdLightMetadata.hashes);
    std::free(g_sdLightMetadata.links);
    g_sdLightMetadata = {};
    sdLightWriteLock.store(0);
}

// @verified - p
void CompleteSDLightDataLoad(void *, std::uint32_t contentHandle, void *data, std::uint64_t, const char *) {
    AcquireSDLightWrite();
    if (g_sdLightMetadata.count) {
        for (std::uint32_t index = 0; index < g_sdLightMetadata.capacity; ++index) {
            g_sdLightMetadata.values[index] = 0;
            g_sdLightMetadata.hashes[index] = 0;
            g_sdLightMetadata.links[index] = 0;
        }
        g_sdLightMetadata.count = 0;
    }
    if (data) {
        std::memcpy(&g_sdLightDataHeader, data, sizeof(g_sdLightDataHeader));
        if (g_sdLightDataHeader.version == 1) {
            const std::byte *const records = static_cast<const std::byte *>(data) + g_sdLightDataHeader.recordsOffset;
            for (std::uint32_t index = 0; index < g_sdLightDataHeader.count; ++index) {
                std::uint64_t value;
                std::memcpy(&value, records + index * g_sdLightDataHeader.stride, sizeof(value));
                const std::uint32_t hash = static_cast<std::uint32_t>(value >> 32U);
                if (hash)
                    InsertSDLightMetadata(g_sdLightMetadata, value, hash);
            }
        }
    }
    d3t::ProcessThreadStates(contentHandle, 1);
    sdLightWriteLock.store(0);
}

// @verified - p
std::int64_t LoadSDLightMetadata(std::int32_t sceneId, shen_name area) {
    AcquireSDLightWrite();
    g_sdLightSceneId = sceneId;
    g_sdLightArea = area;
    sdLightWriteLock.store(0);
    const std::uint32_t renderHandle = d3t::d3t_AllocateRenderHandle();
    sdLightReaderCount.fetch_add(1);
    while (sdLightWriteLock.load() != 0) {
    }
    const char *const name = g_sdLightArea.characters;
    char areaName[16];
    std::snprintf(areaName, sizeof(areaName), "%c%c%c%c", name[0], name[1], name[2], name[3]);
    char path[272];
    std::snprintf(path, sizeof(path), "/SceneMetaData/%s/SDLightMetaData_%s.sdlightdata", areaName, areaName);
    sdLightReaderCount.fetch_sub(1);
    const d3t::D3TShaderLoadTask task{.path = path,
        .renderHandle = renderHandle,
        .reserved_0c = 0,
        .thread = {.callback = CompleteSDLightDataLoad, .data = nullptr},
        .destination = nullptr,
        .destinationCapacity = 0,
        .option = 0,
        .reserved_31 = {}};
    return d3t::PushShaderLoadToThreadQueue(&task);
}

// @verified - p
std::uint64_t ResolveSDLightOwner(std::uint64_t handle) {
    std::uint64_t previous = 0;
    std::uint32_t attempts = 0;
    for (;;) {
        const std::uint64_t parent = d3t::GetModelCacheParent(handle);
        if (parent == previous || !static_cast<std::uint32_t>(parent) || !static_cast<std::uint32_t>(parent >> 32U))
            break;
        previous = parent;
        if (++attempts >= 1024)
            return parent;
    }
    return previous;
}

// @verified - p
void AddSDLightOverride(std::uint64_t owner) {
    for (std::uint32_t index = 0; index < g_sdLightOverrides.count; ++index)
        if (g_sdLightOverrides.values[index] == owner)
            return;
    *AppendSDLightOverride() = owner;
}
// @verified - p
void InitializeSDLightFlags() {
    g_sdLightOverrides.count = 0;
    g_sdLightCount = 2;
}

} // namespace shenmue::s2
