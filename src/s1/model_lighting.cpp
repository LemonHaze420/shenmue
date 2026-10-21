#include "s1/model_lighting.hpp"
#include "s1/render_array.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/d3t/model.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>

namespace shenmue::s1 {
std::array<ModelLightRecord, 16> g_modelLights{};
std::uint32_t g_modelLightCount = 0;
ModelLightOverrides g_modelLightOverrides{};
std::array<std::uint8_t, 32> g_modelShadowSlots{};
std::array<std::uint8_t, 32> g_modelShadowDirty{};
ModelShadowMapping g_modelShadowMapping{};
std::int32_t g_modelReflectionSize = 1024;
Vector2 g_modelReflectionAtlasSize{};

// @verified - p
std::uint32_t AllocateModelShadowSlots(std::uint32_t count) {
    std::uint32_t freeCount = 0;
    for (std::uint32_t index = 0; index < 32; ++index) {
        if (g_modelShadowSlots[index])
            freeCount = 0;
        else if (++freeCount == count) {
            const std::uint32_t first = index - count + 1;
            std::memset(g_modelShadowSlots.data() + first, 1, count);
            std::memset(g_modelShadowDirty.data() + first, 1, count);
            return first;
        }
    }
    return 32;
}

namespace {
const int g_modelShadowMappingCleanup = std::atexit(ReleaseModelShadowMapping);

// @verified - p
void ClearModelShadowMappingNode(ModelShadowMapping &mapping, ModelShadowMappingNode &node, std::uint32_t slot) {
    if (node.children[0])
        ClearModelShadowMappingNode(mapping, *node.children[0], slot);
    if (node.children[1])
        ClearModelShadowMappingNode(mapping, *node.children[1], slot);
    if (node.slot == slot)
        for (std::uint32_t index = 0; index < mapping.nodeCount; ++index)
            if (mapping.nodes[index].slot == slot)
                node.output = nullptr;
}

ModelShadowMappingNode *AppendModelShadowMappingNode(ModelShadowMapping &mapping) {
    if (mapping.nodeCount >= mapping.nodeCapacity) {
        const std::uint32_t capacity = 2U * mapping.nodeCapacity >= 16U ? 2U * mapping.nodeCapacity : 16U;
        if (capacity > mapping.nodeCapacity) {
            void *const allocation = std::realloc(mapping.nodes, sizeof(ModelShadowMappingNode) * capacity);
            if (!allocation)
                return nullptr;
            mapping.nodes = static_cast<ModelShadowMappingNode *>(allocation);
            mapping.nodeCapacity = capacity;
        }
    }
    return mapping.nodes + mapping.nodeCount++;
}

ModelShadowMappingRequest *AppendModelShadowMappingRequest(ModelShadowMapping &mapping) {
    if (mapping.requestCount >= mapping.requestCapacity) {
        const std::uint32_t capacity = 2U * mapping.requestCapacity >= 16U ? 2U * mapping.requestCapacity : 16U;
        if (capacity > mapping.requestCapacity) {
            void *const allocation = std::realloc(mapping.requests, sizeof(ModelShadowMappingRequest) * capacity);
            if (!allocation)
                return nullptr;
            mapping.requests = static_cast<ModelShadowMappingRequest *>(allocation);
            mapping.requestCapacity = capacity;
        }
    }
    return mapping.requests + mapping.requestCount++;
}

// @verified - p
ModelShadowMappingNode *InsertModelShadowMappingNode(ModelShadowMapping &mapping, const ModelShadowMappingRequest &request, ModelShadowMappingNode *node) {
    for (;;) {
        while (node->children[0] || node->children[1]) {
            ModelShadowMappingNode *const result = InsertModelShadowMappingNode(mapping, request, node->children[0]);
            if (result)
                return result;
            node = node->children[1];
        }
        if (node->output || node->width < request.width || node->height < request.height)
            return nullptr;
        if (node->width == request.width && node->height == request.height)
            return node;
        const std::int32_t x = node->x, y = node->y, width = node->width, height = node->height;
        ModelShadowMappingNode **const second = &node->children[1];
        ModelShadowMappingNode *const first = AppendModelShadowMappingNode(mapping);
        node->children[0] = first;
        node = first;
        ModelShadowMappingNode *const other = AppendModelShadowMappingNode(mapping);
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
void ComputeModelShadowAxisRatios(const ModelShadowMappingNode &node, Vector2 &output) {
    output.x = static_cast<float>(node.x / node.width);
    output.y = static_cast<float>(node.y / node.height);
}

// @verified - p
bool PackModelShadowMappingRequests() {
    ModelShadowMapping &mapping = g_modelShadowMapping;
    for (std::uint32_t index = 1; index < mapping.requestCount; ++index) {
        const ModelShadowMappingRequest request = mapping.requests[index];
        std::uint32_t destination = index;
        while (destination && request.width > mapping.requests[destination - 1].width) {
            mapping.requests[destination] = mapping.requests[destination - 1];
            --destination;
        }
        mapping.requests[destination] = request;
    }
    bool result = true;
    ModelShadowMappingNode *const root = mapping.nodes;
    for (std::uint32_t index = 0; index < mapping.requestCount; ++index) {
        ModelShadowMappingRequest &request = mapping.requests[index];
        if (request.processed)
            continue;
        request.processed = 1;
        ModelShadowMappingNode *const node = InsertModelShadowMappingNode(mapping, request, root);
        if (node) {
            node->output = request.output;
            node->slot = request.slot;
            ComputeModelShadowAxisRatios(*node, *request.output);
        } else {
            const ModelShadowMappingNode fallback{.children = {}, .x = -1, .y = -1, .width = -1, .height = -1, .slot = 0, .reserved24 = 0, .output = nullptr};
            ComputeModelShadowAxisRatios(fallback, *request.output);
            result = false;
        }
    }
    return result;
}

// @verified - p
void InsetModelShadowRectangle(const Vector2 &padding, const Vector2 &size, Vector2 &offset, Vector2 &minimum, Vector2 &maximum) {
    offset.x = size.x * offset.x;
    offset.y = size.y * offset.y;
    minimum = {padding.x + offset.x, padding.y + offset.y};
    maximum = {(offset.x + size.x) - padding.x, (offset.y + size.y) - padding.y};
}
} // namespace

// @verified - p
void ReleaseModelShadowMapping() {
    std::free(g_modelShadowMapping.nodes);
    g_modelShadowMapping.nodes = nullptr;
    g_modelShadowMapping.nodeCount = g_modelShadowMapping.nodeCapacity = 0;
    std::free(g_modelShadowMapping.requests);
    g_modelShadowMapping.requests = nullptr;
    g_modelShadowMapping.requestCount = g_modelShadowMapping.requestCapacity = 0;
}

// @verified - p
void RemoveModelShadowMappingRequest(std::uint32_t slot) {
    ModelShadowMapping &mapping = g_modelShadowMapping;
    for (std::uint32_t index = 0; index < mapping.requestCount; ++index) {
        if (mapping.requests[index].slot != slot)
            continue;
        ModelShadowMappingNode &root = *mapping.nodes;
        if (root.children[0])
            ClearModelShadowMappingNode(mapping, *root.children[0], slot);
        if (root.children[1])
            ClearModelShadowMappingNode(mapping, *root.children[1], slot);
        if (root.slot == slot)
            for (std::uint32_t node = 0; node < mapping.nodeCount; ++node)
                if (mapping.nodes[node].slot == slot)
                    root.output = nullptr;
        std::memmove(mapping.requests + index, mapping.requests + mapping.requestCount - 1, sizeof(ModelShadowMappingRequest));
        --mapping.requestCount;
        return;
    }
}

// @verified - p
void ReleaseModelLightShadowSlots(ModelLightRecord &light) {
    std::uint32_t count;
    switch (light.type) {
    case 0:
        count = 6;
        break;
    case 1:
        count = 4;
        break;
    case 2:
        count = light.textureHandle ? 0 : 1;
        break;
    default:
        return;
    }
    for (std::uint32_t index = light.shadowIndex; index < light.shadowIndex + count; ++index) {
        if (index >= 32)
            std::abort();
        g_modelShadowSlots[index] = 0;
    }
    if (light.type == 2)
        RemoveModelShadowMappingRequest(light.shadowIndex);
    else
        for (std::uint32_t index = 0; index < count; ++index)
            RemoveModelShadowMappingRequest(light.shadowIndex + index);
}

// @verified - p
void RemoveHmdlLightBinding(std::uint32_t index) {
    if (index < 2 || index >= g_modelLightCount)
        return;
    ModelLightRecord &light = g_modelLights[index];
    if (light.shadowMode)
        ReleaseModelLightShadowSlots(light);
    RemoveSkinningNode(static_cast<std::uint64_t>(light.nodeHash) << 32U | light.nodeCache);
    light.ownerCache = light.ownerNode = light.nodeCache = light.nodeHash = 0;
    if (index < g_modelLightCount - 1)
        std::memmove(&light, &g_modelLights[index + 1], sizeof(ModelLightRecord) * (g_modelLightCount - index - 1));
    --g_modelLightCount;
}

// @verified - p
void RemoveHmdlLightOwner(RenderArrayEntry &entry) {
    std::uint32_t index = 0;
    while (index < g_modelLightCount) {
        const ModelLightRecord &light = g_modelLights[index];
        const std::uint64_t owner = entry.handle;
        if (light.ownerCache == static_cast<std::uint32_t>(owner) && light.ownerNode == static_cast<std::uint32_t>(owner >> 32U))
            RemoveHmdlLightBinding(index);
        else
            ++index;
    }
}

// @verified - p
bool AllocateHmdlReflectionSlot(RenderArrayEntry &entry) {
    const std::int32_t pixels = g_modelReflectionSize;
    const float size = static_cast<float>(static_cast<std::uint32_t>(pixels)) / g_modelReflectionAtlasSize.x;
    const std::uint32_t slot = AllocateModelShadowSlots(1);
    if (slot == 32)
        return false;
    entry.reflectionSize = {size, size};
    entry.reflectionSlot = slot;
    ModelShadowMappingRequest *const request = AppendModelShadowMappingRequest(g_modelShadowMapping);
    if (request) {
        request->processed = 0;
        request->slot = slot;
        request->x = request->y = 0;
        request->width = request->height = pixels;
        request->output = &entry.reflectionOffset;
    }
    if (!PackModelShadowMappingRequests())
        return false;
    g_modelShadowDirty[slot] = 1;
    const Vector2 padding{(1.0f / g_modelReflectionAtlasSize.x) * 2.0f, (1.0f / g_modelReflectionAtlasSize.y) * 2.0f};
    InsetModelShadowRectangle(padding, entry.reflectionSize, entry.reflectionOffset, entry.reflectionMinimum, entry.reflectionMaximum);
    return true;
}

// @verified - p
std::uint64_t ResolveHmdlLightOwner(std::uint64_t handle) {
    std::uint64_t previous = 0;
    std::uint32_t attempts = 0;
    for (;;) {
        const std::uint64_t parent = GetHashForSkinningBuffer(handle);
        if (parent == previous || !static_cast<std::uint32_t>(parent) || !static_cast<std::uint32_t>(parent >> 32U))
            break;
        previous = parent;
        if (++attempts >= 1024)
            return parent;
    }
    return previous;
}

// @verified - p
void AddHmdlLightOverride(std::uint64_t owner) {
    ModelLightOverrides &overrides = g_modelLightOverrides;
    for (std::uint32_t index = 0; index < overrides.count; ++index)
        if (overrides.values[index] == owner)
            return;
    std::uint64_t *entry = nullptr;
    if (overrides.count >= overrides.capacity) {
        const std::uint32_t capacity = 2U * overrides.capacity >= 16U ? 2U * overrides.capacity : 16U;
        if (capacity > overrides.capacity) {
            void *const allocation = std::realloc(overrides.values, sizeof(std::uint64_t) * capacity);
            if (!allocation) {
                *entry = owner;
                return;
            }
            overrides.values = static_cast<std::uint64_t *>(allocation);
            overrides.capacity = capacity;
        }
    }
    entry = overrides.values + overrides.count++;
    *entry = owner;
}

// @verified - p
void RegisterHmdlWeatherSprite(RenderArrayEntry &entry, const ModelWeatherSpriteBinding &binding) {
    const D3TModelSprite &sprite = *binding.sprite;
    const std::uint64_t node = binding.node;
    entry.owner->flags |= 0x200U;
    const Matrix4x4 world = ComputeGlobalSkinningTransform(node);
    const std::uint32_t index = g_modelLightCount;
    ModelLightRecord &light = g_modelLights[index];
    light.colour = sprite.colour;
    light.type = sprite.type;
    const float reciprocalW = 1.0f / (((world.rows[3].y * 0.0f + world.rows[3].x * 0.0f) + world.rows[3].z * 0.0f) + world.rows[3].w);
    light.position.x = reciprocalW * (((world.rows[0].y * 0.0f + world.rows[0].x * 0.0f) + world.rows[0].z * 0.0f) + world.rows[0].w);
    light.position.y = reciprocalW * (((world.rows[1].y * 0.0f + world.rows[1].x * 0.0f) + world.rows[1].z * 0.0f) + world.rows[1].w);
    light.position.z = reciprocalW * (((world.rows[2].y * 0.0f + world.rows[2].x * 0.0f) + world.rows[2].z * 0.0f) + world.rows[2].w);
    light.direction = {world.rows[0].z, world.rows[1].z, world.rows[2].z};
    Matrix4x4 local;
    LookupSkinningTransform(node, local);
    Vector4 angles;
    Matrix4x4::ExtractEulerAnglesFromMatrix(&local, &angles);
    light.localRotationDegrees = {angles.x * 180.0f / 3.1415901f, angles.y * 180.0f / 3.1415901f, angles.z * 180.0f / 3.1415901f};
    light.range = sprite.range;
    light.coneCosine = std::cos(sprite.coneAngle * 0.5f);
    light.textureHandle = sprite.textureHandle;
    light.textureName = binding.textureName;
    light.shadowMode = sprite.shadowMode;
    light.shadowBias = sprite.shadowBias;
    light.shadowIndex = 0;
    light.parameter104 = sprite.parameter0c;
    light.parameter108 = sprite.parameter2c;
    light.parameterEnabled = sprite.parameter0c != 0.0f;
    light.parameter128 = sprite.parameter34;
    light.state = sprite.parameter38;
    light.nodeCache = static_cast<std::uint32_t>(node);
    light.nodeHash = static_cast<std::uint32_t>(node >> 32U);
    const std::uint64_t owner = ResolveHmdlLightOwner(node);
    light.ownerCache = static_cast<std::uint32_t>(owner);
    light.ownerNode = static_cast<std::uint32_t>(owner >> 32U);
    g_modelLightCount = index + 1;
    AddHmdlLightOverride(owner);
}

// @verified - p
std::uint32_t AssignHmdlWeatherSprites(std::uint64_t handle, RenderArrayEntry &entry) {
    const CachedSkinningBufferSlot *const cache = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!cache)
        return 0;
    std::uint32_t index = GetValueFromMurmurHash(&cache->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    std::uint32_t count = 0;
    std::int32_t pending = 0;
    do {
        const D3TModelNode &node = reinterpret_cast<const D3TModelNode *>(cache->entries)[index];
        if (node.sprite) {
            const D3TModelSprite &sprite = reinterpret_cast<const D3TModelSprite *>(cache->sprites)[node.sprite - 1U];
            const char *textureName = nullptr;
            if (sprite.texture) {
                const D3TModelRecord &model = *d3t::ResolveModel(cache->renderHandles[index]);
                const std::uint32_t offset = model.textures[sprite.texture - 1U].pathOffset;
                if (offset < model.stringsSize)
                    textureName = model.strings + offset;
            }
            const ModelWeatherSpriteBinding binding{.sprite = &sprite,
                .node = static_cast<std::uint64_t>(cache->parentHashes[index]) << 32U | static_cast<std::uint32_t>(handle),
                .textureName = textureName};
            RegisterHmdlWeatherSprite(entry, binding);
            ++count;
        }
        ++index;
        pending += node.childCount - 1;
    } while (pending >= 0);
    return count;
}

} // namespace shenmue::s1
