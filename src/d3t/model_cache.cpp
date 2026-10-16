#include "d3t/model_cache.hpp"
#include "d3t/archive_data.hpp"
#include "d3t/buffer.hpp"
#include "s2/renderer_scratch.hpp"
#include "d3t/threads.hpp"
#include <thread>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <mutex>

namespace shenmue::d3t {
ModelCacheTable g_modelCache{};
std::uint32_t g_modelExpressionCacheSlotTags[128]{};
ModelExpressionRuntime g_modelExpressionRuntime[128]{};
std::uint32_t g_nextModelExpressionSlot{};
namespace {
std::mutex g_modelCacheExpressionMutex;

bool InsertCacheIndex(ModelCacheIndexTable &table, std::uint32_t hash, std::uint32_t value);
bool GrowCacheIndex(ModelCacheIndexTable &table, std::uint32_t capacity) {
    ModelCacheIndexTable grown{.values = static_cast<std::uint32_t *>(std::malloc(4ULL * capacity)),
        .hashes = static_cast<std::uint32_t *>(std::malloc(4ULL * capacity)),
        .links = static_cast<std::uint16_t *>(std::malloc(2ULL * capacity)),
        .capacity = capacity,
        .count = 0};
    if (!grown.values || !grown.hashes) {
        std::free(grown.values);
        std::free(grown.hashes);
        std::free(grown.links);
        return false;
    }
    std::memset(grown.values, 0, 4ULL * capacity);
    std::memset(grown.hashes, 0, 4ULL * capacity);
    std::memset(grown.links, 0, 2ULL * capacity);
    if (table.values && table.hashes) {
        for (std::uint32_t index = 0; index < table.capacity; ++index)
            if (table.hashes[index])
                InsertCacheIndex(grown, table.hashes[index], table.values[index]);
        std::free(table.values);
        std::free(table.hashes);
        std::free(table.links);
    }
    table.values = grown.values;
    table.hashes = grown.hashes;
    table.links = grown.links;
    table.capacity = grown.capacity;
    return true;
}

bool InsertCacheIndex(ModelCacheIndexTable &table, std::uint32_t hash, std::uint32_t value) {
    if ((!table.capacity || table.count >= table.capacity - (table.capacity >> 2)) &&
        !GrowCacheIndex(table, table.capacity >= 64 ? table.capacity + (table.capacity >> 2) : 64))
        return false;
    std::uint32_t index = hash % table.capacity;
    std::uint32_t hole = UINT32_MAX;
    for (;;) {
        if (table.hashes[index] == hash)
            return true;
        if (table.hashes[index] == 0) {
            if (hole == UINT32_MAX)
                hole = index;
            if (!table.links[index]) {
                ++table.count;
                table.hashes[index] = hash;
                table.values[index] = value;
                return true;
            }
        }
        if (!table.links[index])
            break;
        index = (index + table.links[index]) % table.capacity;
    }
    if (hole != UINT32_MAX) {
        ++table.count;
        table.hashes[hole] = hash;
        table.values[hole] = value;
        return true;
    }
    std::uint32_t freeIndex = (index + 1) % table.capacity;
    while (freeIndex != index && table.hashes[freeIndex])
        freeIndex = (freeIndex + 1) % table.capacity;
    if (freeIndex == index)
        return false;
    ++table.count;
    table.hashes[freeIndex] = hash;
    table.values[freeIndex] = value;
    table.links[index] = static_cast<std::uint16_t>(freeIndex >= index ? freeIndex - index : freeIndex + table.capacity - index);
    return true;
}

bool EraseCacheIndex(ModelCacheIndexTable &table, std::uint32_t hash) {
    if (!table.capacity)
        return false;
    std::uint32_t index = hash % table.capacity;
    if (table.hashes[index] == hash) {
        table.hashes[index] = table.values[index] = 0;
        --table.count;
        return true;
    }
    std::uint32_t previous = index;
    while (table.links[index]) {
        index = (index + table.links[index]) % table.capacity;
        if (table.hashes[index] == hash) {
            table.hashes[index] = table.values[index] = 0;
            if (!table.links[index])
                table.links[previous] = 0;
            --table.count;
            return true;
        }
        previous = index;
    }
    return false;
}

std::uint32_t *GetOrInsertCacheIndex(ModelCacheIndexTable &table, std::uint32_t hash) {
    if ((!table.capacity || table.count >= table.capacity - (table.capacity >> 2)) &&
        !GrowCacheIndex(table, table.capacity >= 64 ? table.capacity + (table.capacity >> 2) : 64))
        return table.values;
    std::uint32_t index = hash % table.capacity;
    std::uint32_t hole = UINT32_MAX;
    for (;;) {
        if (!table.hashes[index]) {
            if (hole == UINT32_MAX)
                hole = index;
            if (!table.links[index]) {
                table.hashes[index] = hash;
                ++table.count;
                return table.values + index;
            }
        }
        if (table.hashes[index] == hash)
            return table.values + index;
        if (!table.links[index])
            break;
        index = (index + table.links[index]) % table.capacity;
    }
    if (hole != UINT32_MAX) {
        table.hashes[hole] = hash;
        ++table.count;
        return table.values + hole;
    }
    std::uint32_t next = (index + 1) % table.capacity;
    while (next != index && table.hashes[next])
        next = (next + 1) % table.capacity;
    if (next == index)
        return table.values + index;
    table.hashes[next] = hash;
    ++table.count;
    table.links[index] = static_cast<std::uint16_t>(next >= index ? next - index : next + table.capacity - index);
    return table.values + next;
}

void RemoveModelCacheSubtreeByIndex(ModelCacheSlot &slot, std::uint32_t first) {
    const std::uint32_t removed = slot.nodes[first].subtreeSize;
    const std::uint32_t end = first + removed;
    for (std::uint32_t node = first; node < end; ++node) {
        D3TModelNode &entry = slot.nodes[node];
        if (entry.sprite) {
            std::uint32_t sprite = entry.sprite - 1U;
            for (std::uint32_t index = 0; index < slot.nodeCount; ++index)
                if (slot.nodes[index].sprite > sprite)
                    --slot.nodes[index].sprite;
            for (; sprite < slot.spriteCount - 1; ++sprite)
                std::memmove(slot.sprites + sprite, slot.sprites + sprite + 1, sizeof(D3TModelSprite));
            --slot.spriteCount;
        }
        if (entry.probe) {
            std::uint32_t probe = entry.probe - 1U;
            for (std::uint32_t index = 0; index < slot.nodeCount; ++index)
                if (slot.nodes[index].probe > probe)
                    --slot.nodes[index].probe;
            for (; probe < slot.probeCount - 1; ++probe)
                std::memmove(slot.probes + probe, slot.probes + probe + 1, sizeof(D3TModelProbe));
            --slot.probeCount;
        }
        if (slot.constantBuffers[node]) {
            ValidateAndCommitBuffer(static_cast<std::int32_t>(slot.constantBuffers[node]));
            slot.constantBuffers[node] = 0;
        }
        EraseCacheIndex(slot.indexTable, d3t_HashData(slot.nodeHashes + node, 4));
    }
    if (slot.nodes[first].parentDistance) {
        std::uint32_t parent = first - slot.nodes[first].parentDistance;
        --slot.nodes[parent].childCount;
        for (;;) {
            slot.nodes[parent].subtreeSize = static_cast<std::uint16_t>(slot.nodes[parent].subtreeSize - removed);
            if (!slot.nodes[parent].parentDistance)
                break;
            parent -= slot.nodes[parent].parentDistance;
        }
    }
    for (std::uint32_t sibling = end; sibling < slot.nodeCount; sibling += slot.nodes[sibling].subtreeSize)
        slot.nodes[sibling].parentDistance = static_cast<std::uint16_t>(slot.nodes[sibling].parentDistance - removed);
    if (first < slot.nodeCount - removed) {
        const std::uint32_t remaining = slot.nodeCount - first - removed;
        std::memmove(slot.nodeHashes + first, slot.nodeHashes + end, 4ULL * remaining);
        std::memmove(slot.nodes + first, slot.nodes + end, sizeof(D3TModelNode) * remaining);
        std::memmove(slot.matrices + first, slot.matrices + end, sizeof(Matrix4x4) * remaining);
        std::memmove(slot.worldMatrices + first, slot.worldMatrices + end, sizeof(Matrix4x4) * remaining);
        std::memmove(slot.modelHandles + first, slot.modelHandles + end, 4ULL * remaining);
        std::memmove(slot.constantBuffers + first, slot.constantBuffers + end, 4ULL * remaining);
        std::memmove(slot.alpha + first, slot.alpha + end, 4ULL * remaining);
    }
    slot.nodeCount -= removed;
    for (std::uint32_t node = first; node < slot.nodeCount; ++node) {
        const std::uint32_t hash = d3t_HashData(slot.nodeHashes + node, 4);
        *GetOrInsertCacheIndex(slot.indexTable, hash) = node;
    }
}

void DestroyModelCacheExpressionRecord(ModelCacheExpressionRecord &record) {
    for (std::uint32_t index = 0; index < record.boneTableCount; ++index) {
        ModelCacheExpressionBoneTable &table = record.boneTables[index];
        std::free(table.affectedBones);
        table.affectedBones = nullptr;
        table.affectedBoneCount = table.affectedBoneCapacity = 0;
    }
    std::free(record.boneTables);
    record.boneTables = nullptr;
    record.boneTableCount = record.boneTableCapacity = 0;
    for (std::uint32_t index = 0; index < record.expressionSlotCount; ++index) {
        const std::uint32_t handle = record.expressionSlots[index];
        std::uint32_t &generation = g_modelExpressionCacheSlotTags[handle & 0x7f];
        if (generation == (handle & 0xffffff80))
            generation &= 0x7fffffff;
    }
}

bool ResizeModelCache(ModelCacheSlot &slot, std::uint32_t capacity) {
    D3TModelNode *const nodes = static_cast<D3TModelNode *>(std::realloc(slot.nodes, sizeof(D3TModelNode) * capacity));
    Matrix4x4 *const matrices = static_cast<Matrix4x4 *>(std::realloc(slot.matrices, sizeof(Matrix4x4) * capacity));
    Matrix4x4 *const worldMatrices = static_cast<Matrix4x4 *>(std::realloc(slot.worldMatrices, sizeof(Matrix4x4) * capacity));
    std::uint32_t *const constants = static_cast<std::uint32_t *>(std::realloc(slot.constantBuffers, 4ULL * capacity));
    std::uint32_t *const models = static_cast<std::uint32_t *>(std::realloc(slot.modelHandles, 4ULL * capacity));
    std::uint32_t *const hashes = static_cast<std::uint32_t *>(std::realloc(slot.nodeHashes, 4ULL * capacity));
    float *const alpha = static_cast<float *>(std::realloc(slot.alpha, 4ULL * capacity));
    if (!nodes || !matrices || !worldMatrices || !constants || !models || !hashes || !alpha)
        return false;
    std::memset(constants + slot.nodeCount, 0, 4ULL * (capacity - slot.nodeCount));
    for (std::uint32_t index = slot.nodeCount; index < capacity; ++index)
        alpha[index] = 1.0f;
    slot.nodes = nodes;
    slot.matrices = matrices;
    slot.worldMatrices = worldMatrices;
    slot.constantBuffers = constants;
    slot.modelHandles = models;
    slot.nodeHashes = hashes;
    slot.alpha = alpha;
    slot.nodeCapacity = capacity;
    return true;
}

std::uint32_t AppendCacheName(ModelCacheSlot &slot, const char *name) {
    const std::uint32_t size = static_cast<std::uint32_t>(std::strlen(name) + 1);
    for (std::uint32_t offset = 0; offset < slot.namesSize;) {
        const std::uint32_t existing = static_cast<std::uint32_t>(std::strlen(slot.names + offset) + 1);
        if (existing == size && std::memcmp(slot.names + offset, name, size) == 0)
            return offset;
        offset += existing;
    }
    if (slot.namesSize + size > slot.namesCapacity)
        return UINT32_MAX;
    const std::uint32_t offset = slot.namesSize;
    slot.namesSize += size;
    std::memcpy(slot.names + offset, name, size);
    return offset;
}

std::uint64_t AppendCacheRoot(std::uint32_t handle) {
    ModelCacheSlot &slot = *ResolveModelCache(handle);
    const std::uint32_t index = slot.nodeCount;
    if (slot.nodeCapacity <= index && !ResizeModelCache(slot, index + 10))
        return 0;
    ++slot.nodeCount;
    slot.nodes[index] = {};
    slot.nodes[index].nameOffset = AppendCacheName(slot, "SceneRoot");
    slot.nodes[index].subtreeSize = 1;
    slot.matrices[index] = kModelCacheIdentity;
    slot.modelHandles[index] = 0;
    const std::uint32_t nodeHash = slot.nextNodeHash;
    slot.nodeHashes[index] = nodeHash;
    InsertCacheIndex(slot.indexTable, d3t_HashData(&nodeHash, 4), index);
    slot.alpha[index] = 1.0f;
    ++slot.nextNodeHash;
    return static_cast<std::uint64_t>(nodeHash) << 32 | handle;
}

template <typename T> T *AppendCacheTable(T *&data, std::uint32_t &count, std::uint32_t &capacity, std::uint32_t added) {
    std::uint32_t required = count + added;
    if (required > capacity) {
        const std::uint32_t doubled = capacity * 2;
        const std::uint32_t grown = doubled < 16 ? 16 : doubled;
        if (required < grown)
            required = grown;
        T *const resized = static_cast<T *>(std::realloc(data, sizeof(T) * required));
        if (!resized)
            return nullptr;
        data = resized;
        capacity = required;
    }
    count += added;
    return data ? data + count - added : nullptr;
}

void DestroyModelCacheExpressionTarget(ModelCacheExpressionTarget &target) {
    const std::lock_guard lock(g_modelCacheExpressionMutex);
    for (std::uint32_t index = 0; index < target.count; ++index) {
        ModelCacheExpressionRecord &record = target.records[index];
        std::free(record.boneTables);
        record.boneTables = nullptr;
        record.boneTableCount = record.boneTableCapacity = 0;
    }
    std::free(target.records);
    target.records = nullptr;
    target.count = target.capacity = 0;
}

void FreeModelCacheSlot(ModelCacheSlot &slot) {
    std::free(slot.nodes);
    std::free(slot.matrices);
    std::free(slot.worldMatrices);
    if (slot.constantBuffers) {
        for (std::uint32_t index = 0; index < slot.nodeCount; ++index)
            ValidateAndCommitBuffer(static_cast<std::int32_t>(slot.constantBuffers[index]));
        std::free(slot.constantBuffers);
    }
    std::free(slot.modelHandles);
    std::free(slot.nodeHashes);
    std::free(slot.alpha);
    std::free(slot.skinRanges);
    slot.skinRanges = nullptr;
    slot.skinRangeCount = slot.skinRangeCapacity = 0;
    std::free(slot.boneIndices);
    slot.boneIndices = nullptr;
    slot.boneIndexCount = slot.boneIndexCapacity = 0;
    std::free(slot.sprites);
    slot.sprites = nullptr;
    slot.spriteCount = slot.spriteCapacity = 0;
    std::free(slot.names);
    slot.namesSize = slot.namesCapacity = 0;
    std::free(slot.indexTable.values);
    std::free(slot.indexTable.hashes);
    std::free(slot.indexTable.links);
    slot.indexTable = {};
    DestroyModelCacheExpressionTarget(slot.expressions);
    ValidateAndCommitBuffer(static_cast<std::int32_t>(slot.skinningBuffer));
}
} // namespace

ModelCacheSlot *ResolveModelCache(std::uint32_t handle) {
    const std::uint32_t index = handle & 0x7f;
    return g_modelCache.generations[index] == (handle & 0xffffff80) ? &g_modelCache.slots[index] : nullptr;
}

std::uint32_t AllocateModelCache(const char *name) {
    const std::uint32_t first = g_modelCache.cursor;
    std::uint32_t index = first;
    while ((g_modelCache.generations[index] >> 7) >= 0x1000000) {
        index = (index + 1) & 0x7f;
        if (index == first)
            return 0;
    }
    const std::uint32_t next = (g_modelCache.generations[index] >> 7) + 1;
    const std::uint32_t counter = (next & 0xff000000) == 0x1000000 ? 1 : next;
    const std::uint32_t generation = (counter | 0xff000000) << 7;
    const std::uint32_t handle = index | generation;
    g_modelCache.generations[index] = generation;
    g_modelCache.cursor = (index + 1) & 0x7f;
    ModelCacheSlot &slot = g_modelCache.slots[index];
    std::memset(&slot, 0, sizeof(slot));
    slot.nextNodeHash = 1;
    if (!ResizeModelCache(slot, 10))
        return 0;
    slot.names = static_cast<char *>(std::malloc(4000));
    slot.namesSize = 0;
    slot.namesCapacity = 4000;
    AppendCacheRoot(handle);
    std::memcpy(slot.expressions.name, name, std::strlen(name) + 1);
    const BufferDescription description{.flags = 3, .size = 1835008, .count = 224, .flags3 = 2, .buffer = nullptr, .name = "Skinning Buffer"};
    slot.skinningBuffer = static_cast<std::uint32_t>(CreateBuffer(&description));
    return handle;
}

void ReleaseModelCache(std::uint32_t handle) {
    ModelCacheSlot *const slot = ResolveModelCache(handle);
    if (!slot)
        return;
    FreeModelCacheSlot(*slot);
    const std::uint32_t index = handle & 0x7f;
    if (g_modelCache.generations[index] == (handle & 0xffffff80))
        g_modelCache.generations[index] &= 0x7fffffff;
}

std::uint32_t LookupModelCacheIndex(const ModelCacheIndexTable &table, std::uint32_t nodeHash) {
    const std::uint32_t hash = d3t_HashData(&nodeHash, 4);
    if (!table.capacity)
        return 0;
    std::uint32_t index = hash % table.capacity;
    for (;;) {
        if (table.hashes[index] == hash)
            return table.values[index];
        if (!table.links[index])
            return 0;
        index = (index + table.links[index]) % table.capacity;
    }
}

std::uint64_t AppendModelToCache(std::uint32_t cacheHandle, std::uint32_t modelHandle) {
    if (!SetupShaderReferences(modelHandle))
        return 0;
    ModelCacheSlot *const slot = ResolveModelCache(cacheHandle);
    D3TModelRecord *const model = ResolveModel(modelHandle);
    if (!slot || !model || !model->nodeCount)
        return 0;
    const std::uint32_t count = model->nodeCount;
    if (slot->nodeCapacity - slot->nodeCount < count && !ResizeModelCache(*slot, slot->nodeCount + count))
        return 0;
    const std::uint32_t first = slot->nodeCount;
    std::memcpy(slot->nodes + first, model->nodes, sizeof(D3TModelNode) * count);
    std::memcpy(slot->matrices + first, model->nodeTransforms, sizeof(Matrix4x4) * count);
    const std::uint32_t firstHash = slot->nextNodeHash;
    std::uint32_t hash = firstHash;
    for (std::uint32_t index = first; index < first + count; ++index) {
        slot->worldMatrices[index].rows[0].x = 3.40282347e+38f;
        slot->modelHandles[index] = modelHandle;
        slot->nodeHashes[index] = hash;
        InsertCacheIndex(slot->indexTable, d3t_HashData(&hash, 4), index);
        ++hash;
        slot->alpha[index] = 1.0f;
        slot->constantBuffers[index] = 0;
    }
    slot->nextNodeHash = hash;
    slot->nodes[first].parentDistance = static_cast<std::uint16_t>(first);
    ++slot->nodes[0].childCount;
    slot->nodeCount += count;
    slot->nodes[0].subtreeSize = static_cast<std::uint16_t>(slot->nodes[0].subtreeSize + count);
    const std::uint32_t spriteBase = slot->spriteCount;
    D3TModelSprite *const sprites = AppendCacheTable(slot->sprites, slot->spriteCount, slot->spriteCapacity, model->spriteCount);
    if (model->spriteCount)
        std::memcpy(sprites, model->sprites, sizeof(D3TModelSprite) * model->spriteCount);
    const std::uint32_t probeBase = slot->probeCount;
    D3TModelProbe *const probes = AppendCacheTable(slot->probes, slot->probeCount, slot->probeCapacity, model->probeCount);
    if (model->probeCount)
        std::memcpy(probes, model->probes, sizeof(D3TModelProbe) * model->probeCount);
    for (std::uint32_t node = 0; node < count; ++node) {
        const std::uint32_t index = first + node;
        const D3TModelNode &source = model->nodes[node];
        D3TModelNode &entry = slot->nodes[index];
        if (source.sprite)
            entry.sprite = static_cast<std::uint16_t>(entry.sprite + spriteBase);
        if (source.probe)
            entry.probe = static_cast<std::uint16_t>(entry.probe + probeBase);
        if (!source.drawGroup)
            continue;
        const BufferDescription description{.flags = 0, .size = 148, .count = 0, .flags3 = 2, .buffer = nullptr, .name = ""};
        slot->constantBuffers[index] = static_cast<std::uint32_t>(CreateBuffer(&description));
        const D3TModelDrawGroup &group = model->drawGroups[source.drawGroup - 1];
        if (!group.skin)
            continue;
        const D3TModelSkin &skin = model->skins[group.skin - 1];
        ModelCacheSkinRange *const range = AppendCacheTable(slot->skinRanges, slot->skinRangeCount, slot->skinRangeCapacity, 1);
        *range = {.nodeHash = slot->nodeHashes[index], .firstBone = slot->boneIndexCount, .boneCount = skin.boneCount};
        std::uint32_t *const bones = AppendCacheTable(slot->boneIndices, slot->boneIndexCount, slot->boneIndexCapacity, skin.boneCount);
        for (std::uint32_t bone = 0; bone < skin.boneCount; ++bone)
            bones[bone] = firstHash + model->boneNodes[skin.firstBone + bone];
    }
    return static_cast<std::uint64_t>(firstHash) << 32 | cacheHandle;
}

void RemoveModelCacheSubtree(std::uint64_t handle) {
    ModelCacheSlot *const slot = ResolveModelCache(static_cast<std::uint32_t>(handle));
    if (!slot)
        return;
    RemoveModelCacheSubtreeByIndex(*slot, LookupModelCacheIndex(slot->indexTable, static_cast<std::uint32_t>(handle >> 32)));
}

void RetireModelCacheNodeExpressions(std::uint64_t handle) {
    ModelCacheSlot *const slot = ResolveModelCache(static_cast<std::uint32_t>(handle));
    if (!slot)
        return;
    const std::lock_guard lock(g_modelCacheExpressionMutex);
    ModelCacheExpressionTarget &target = slot->expressions;
    for (std::uint32_t index = 0; index < target.count; ++index) {
        if (target.records[index].nodeHash != static_cast<std::uint32_t>(handle >> 32))
            continue;
        DestroyModelCacheExpressionRecord(target.records[index]);
        std::memmove(target.records + index, target.records + target.count - 1, sizeof(ModelCacheExpressionRecord));
        --target.count;
    }
}

std::uint64_t GetModelCacheParent(std::uint64_t handle) {
    ModelCacheSlot *const slot = ResolveModelCache(static_cast<std::uint32_t>(handle));
    if (!slot)
        return 0;
    const std::uint32_t index = LookupModelCacheIndex(slot->indexTable, static_cast<std::uint32_t>(handle >> 32));
    const std::uint16_t distance = slot->nodes[index].parentDistance;
    return distance ? static_cast<std::uint64_t>(slot->nodeHashes[index - distance]) << 32 | static_cast<std::uint32_t>(handle) : 0;
}

std::uint64_t GetModelCacheChild(std::uint64_t handle, std::uint32_t child) {
    ModelCacheSlot &slot = *ResolveModelCache(static_cast<std::uint32_t>(handle));
    const std::uint32_t index = LookupModelCacheIndex(slot.indexTable, static_cast<std::uint32_t>(handle >> 32));
    if (child >= slot.nodes[index].childCount)
        return 0;
    std::uint32_t next = index + 1;
    for (std::uint32_t sibling = 0; sibling < child; ++sibling)
        next += slot.nodes[next].subtreeSize;
    return static_cast<std::uint64_t>(slot.nodeHashes[next]) << 32 | static_cast<std::uint32_t>(handle);
}

void SetModelCacheTransform(std::uint64_t handle, const Matrix4x4 &transform) {
    ModelCacheSlot *const slot = ResolveModelCache(static_cast<std::uint32_t>(handle));
    if (!slot)
        return;
    const std::uint32_t index = LookupModelCacheIndex(slot->indexTable, static_cast<std::uint32_t>(handle >> 32));
    slot->matrices[index] = transform;
}

Matrix4x4 GetModelCacheTransform(std::uint64_t handle) {
    ModelCacheSlot &slot = *ResolveModelCache(static_cast<std::uint32_t>(handle));
    return slot.matrices[LookupModelCacheIndex(slot.indexTable, static_cast<std::uint32_t>(handle >> 32))];
}

namespace {
Matrix4x4 ComputeModelCacheGlobalTransformByIndex(const ModelCacheSlot &slot, std::uint32_t index) {
    Matrix4x4 transform = kModelCacheIdentity;
    std::uint32_t depth = 0;
    for (std::uint32_t parent = index; parent; ++depth)
        parent -= slot.nodes[parent].parentDistance;
    std::uint32_t *const ancestors = static_cast<std::uint32_t *>(s2::GetRendererScratchBlock(4ULL * depth));
    std::uint32_t count = 0;
    for (std::uint32_t parent = index; parent;) {
        parent -= slot.nodes[parent].parentDistance;
        ancestors[count++] = parent;
    }
    for (;;) {
        const std::uint32_t node = count ? ancestors[--count] : index;
        const Matrix4x4 &local = slot.matrices[node];
        Matrix4x4 product{};

        for (std::uint32_t row = 0; row < 4; ++row) {
            for (std::uint32_t column = 0; column < 4; ++column)
                product.rows[row][column] = ((transform.rows[row].x * local.rows[0][column] + transform.rows[row].y * local.rows[1][column]) +
                                                transform.rows[row].z * local.rows[2][column]) +
                    transform.rows[row].w * local.rows[3][column];
        }
        transform = product;
        if (node == index)
            break;
    }
    if (ancestors)
        s2::g_rendererScratch.current = reinterpret_cast<std::byte *>(ancestors);
    return transform;
}
} // namespace

Matrix4x4 ComputeModelCacheGlobalTransform(std::uint64_t handle) {
    ModelCacheSlot *const slot = ResolveModelCache(static_cast<std::uint32_t>(handle));
    if (!slot)
        return kModelCacheIdentity;
    const std::uint32_t index = LookupModelCacheIndex(slot->indexTable, static_cast<std::uint32_t>(handle >> 32));
    return ComputeModelCacheGlobalTransformByIndex(*slot, index);
}

void SetModelCacheAlpha(std::uint64_t handle, float alpha) {
    if (!static_cast<std::uint32_t>(handle) || !(handle >> 32))
        return;
    ModelCacheSlot *const slot = ResolveModelCache(static_cast<std::uint32_t>(handle));
    if (!slot)
        return;
    const std::uint32_t index = LookupModelCacheIndex(slot->indexTable, static_cast<std::uint32_t>(handle >> 32));
    const std::uint16_t count = slot->nodes[index].subtreeSize;
    for (std::uint32_t node = 0; node < count; ++node)
        slot->alpha[index + node] = alpha;
}

void UpdateModelCacheFlags(std::uint64_t handle, std::uint16_t flags, ModelCacheFlagMode mode) {
    ModelCacheSlot *const slot = ResolveModelCache(static_cast<std::uint32_t>(handle));
    if (!slot)
        return;
    const std::uint32_t index = LookupModelCacheIndex(slot->indexTable, static_cast<std::uint32_t>(handle >> 32));
    const std::uint16_t count = slot->nodes[index].subtreeSize;
    for (std::uint32_t node = 0; node < count; ++node) {
        std::uint32_t &value = slot->nodes[index + node].flags;
        switch (mode) {
        case ModelCacheFlagMode::Enable:
            value |= flags;
            break;
        case ModelCacheFlagMode::Clear:
            value &= ~static_cast<std::uint32_t>(flags);
            break;
        case ModelCacheFlagMode::Toggle:
            value ^= flags;
            break;
        case ModelCacheFlagMode::Set:
            value = (value & 0xffff0000U) | flags;
            break;
        default:
            break;
        }
    }
}

const char *GetModelCacheNodeName(std::uint64_t handle) {
    ModelCacheSlot *const slot = ResolveModelCache(static_cast<std::uint32_t>(handle));
    if (!slot)
        return "INVALID NODE";
    const std::uint32_t index = LookupModelCacheIndex(slot->indexTable, static_cast<std::uint32_t>(handle >> 32));
    const std::uint32_t offset = slot->nodes[index].nameOffset;
    const std::uint32_t modelHandle = slot->modelHandles[index];
    if (!modelHandle)
        return offset < slot->namesSize ? slot->names + offset : nullptr;
    const D3TModelRecord &model = *ResolveModel(modelHandle);
    return offset < model.stringsSize ? model.strings + offset : nullptr;
}

std::uint64_t FindModelCacheNode(std::uint64_t handle, const char *name) {
    ModelCacheSlot *const slot = ResolveModelCache(static_cast<std::uint32_t>(handle));
    if (!slot)
        return 0;
    std::uint32_t index = LookupModelCacheIndex(slot->indexTable, static_cast<std::uint32_t>(handle >> 32));
    std::int32_t remaining = 0;
    do {
        const D3TModelNode &node = slot->nodes[index];
        const D3TModelRecord *const model = ResolveModel(slot->modelHandles[index]);
        const char *text = model && node.nameOffset < model->stringsSize ? model->strings + node.nameOffset : nullptr;
        if (!text && node.nameOffset < slot->namesSize)
            text = slot->names + node.nameOffset;
        if (text && std::strcmp(text, name) == 0)
            return static_cast<std::uint64_t>(slot->nodeHashes[index]) << 32 | static_cast<std::uint32_t>(handle);
        ++index;
        remaining += static_cast<std::int32_t>(node.childCount) - 1;
    } while (remaining >= 0);
    return 0;
}

std::uint64_t FindModelCacheNodeContaining(std::uint64_t handle, const char *text) {
    if (std::strstr(GetModelCacheNodeName(handle), text))
        return handle;
    if (!static_cast<std::uint32_t>(handle) || !(handle >> 32))
        return 0;
    ModelCacheSlot &slot = *ResolveModelCache(static_cast<std::uint32_t>(handle));
    const std::uint32_t index = LookupModelCacheIndex(slot.indexTable, static_cast<std::uint32_t>(handle >> 32));
    const std::uint32_t count = slot.nodes[index].childCount;
    for (std::uint32_t child = 0; child < count; ++child) {
        const std::uint64_t found = FindModelCacheNodeContaining(GetModelCacheChild(handle, child), text);
        if (found)
            return found;
    }
    return 0;
}
namespace {
struct ExpressionNodeMask {
    std::uint32_t words[11];
};

ModelCacheExpressionRecord *GetModelCacheExpressionRecord(std::uint64_t target) {
    ModelCacheSlot *const cache = ResolveModelCache(static_cast<std::uint32_t>(target));
    if (!cache)
        return nullptr;
    const std::lock_guard lock(g_modelCacheExpressionMutex);
    for (std::uint32_t index = 0; index < cache->expressions.count; ++index)
        if (cache->expressions.records[index].nodeHash == static_cast<std::uint32_t>(target >> 32))
            return cache->expressions.records + index;
    const auto node = LookupModelCacheIndex(cache->indexTable, static_cast<std::uint32_t>(target >> 32));
    const auto modelHandle = cache->modelHandles[node];
    D3TModelRecord *const model = ResolveModel(modelHandle);
    if (!model || !model->animations.animationCount)
        return nullptr;
    const auto &animations = model->animations;
    for (std::uint32_t index = 0; index < animations.animationCount; ++index)
        if (animations.animations[index].channelCount > 350)
            return nullptr;
    auto &records = cache->expressions;
    if (!records.capacity) {
        auto *const grown = static_cast<ModelCacheExpressionRecord *>(std::realloc(records.records, 4 * sizeof(ModelCacheExpressionRecord)));
        if (grown) {
            records.records = grown;
            records.capacity = 4;
        }
    }
    ModelCacheExpressionRecord *const record = AppendCacheTable(records.records, records.count, records.capacity, 1);
    if (!record)
        return nullptr;
    *record = {};
    record->modelHandle = modelHandle;
    record->nodeHash = static_cast<std::uint32_t>(target >> 32);
    auto **const mapped = static_cast<std::uint16_t **>(s2::GetRendererScratchBlock(8ULL * animations.boneMapCount));
    for (std::uint32_t map = 0; map < animations.boneMapCount; ++map) {
        const D3TModelBoneMap &source = animations.boneMaps[map];
        mapped[map] = static_cast<std::uint16_t *>(s2::GetRendererScratchBlock(2ULL * source.entryCount));
        for (std::uint32_t entry = 0; entry < source.entryCount; ++entry) {
            const auto handle = FindModelCacheNode(target, reinterpret_cast<const char *>(source.entries + 64ULL * entry));
            if (static_cast<std::uint32_t>(handle) && static_cast<std::uint32_t>(handle >> 32))
                mapped[map][entry] = static_cast<std::uint16_t>((handle >> 32) - (target >> 32));
        }
    }
    auto *const tables = static_cast<ModelCacheExpressionBoneTable *>(std::realloc(record->boneTables, 720ULL * animations.animationCount));
    if (tables) {
        record->boneTables = tables;
        record->boneTableCapacity = animations.animationCount;
    }
    for (std::uint32_t animation = 0; animation < record->boneTableCapacity; ++animation) {
        ModelCacheExpressionBoneTable &table = record->boneTables[record->boneTableCount++];
        table = {};
        const auto &source = animations.animations[animation];
        auto *const channels = reinterpret_cast<std::uint16_t *>(table.reserved00);
        for (std::uint32_t channel = 0; channel < source.channelCount; ++channel)
            channels[channel] = static_cast<std::uint16_t>(source.channels[channel]);
        auto *masks = static_cast<ExpressionNodeMask *>(std::realloc(table.affectedBones, 44ULL * animations.boneMapCount));
        if (masks) {
            table.affectedBones = masks;
            table.affectedBoneCapacity = animations.boneMapCount;
        }
        for (std::uint32_t map = 0; map < animations.boneMapCount; ++map) {
            auto *const mask = AppendCacheTable(masks, table.affectedBoneCount, table.affectedBoneCapacity, 1);
            table.affectedBones = masks;
            if (!mask)
                continue;
            std::memset(mask, 0, 44);
            for (std::uint32_t channel = 0; channel < source.channelCount; ++channel)
                for (std::uint32_t entry = 0; entry < animations.boneMaps[map].entryCount; ++entry)
                    if (channels[channel] == mapped[map][entry]) {
                        mask->words[channel >> 5] |= 1U << (channel & 31U);
                        break;
                    }
        }
    }
    if (mapped)
        s2::g_rendererScratch.current = reinterpret_cast<std::byte *>(mapped);
    return record;
}

const ModelExpressionDefinition *FindExpressionDefinition(const D3TModelAnimationSet &set, const char *name) {
    if (!name || !*name)
        return nullptr;
    for (std::uint32_t index = 0; index < set.expressionCount; ++index) {
        const auto *const definition = reinterpret_cast<const ModelExpressionDefinition *>(set.expressions + 212ULL * index);
        const char *left = definition->name;
        const char *right = name;
        while (std::tolower(static_cast<unsigned char>(*left)) == std::tolower(static_cast<unsigned char>(*right))) {
            if (!*left)
                return definition;
            ++left;
            ++right;
        }
    }
    return nullptr;
}

void BuildExpressionAffectedNodeMasks(const ModelCacheExpressionRecord &record, ModelExpressionRuntime &expression) {
    if (!expression.submission.elementMask)
        return;
    D3TModelRecord *const model = ResolveModel(record.modelHandle);
    if (!model)
        return;
    const auto &table = record.boneTables[expression.definition->animationIndex];
    std::memset(expression.affectedNodeBits, 0, sizeof(expression.affectedNodeBits));
    for (std::uint32_t element = 0; element < 32; ++element) {
        if (!(expression.submission.elementMask & (1U << element)))
            continue;
        if (element >= model->animations.boneMapCount || element >= table.affectedBoneCount)
            continue;
        const auto *const mask = static_cast<const std::uint32_t *>(table.affectedBones) + 11ULL * element;
        for (std::size_t word = 0; word < 11; ++word)
            expression.affectedNodeBits[word] |= mask[word];
    }
}

void InitializeQueuedExpressionRuntime(ModelExpressionRuntime &expression) {
    for (auto &bank : expression.banks) {
        bank.selectedIndex = -1;
        bank.keyOffset = 0;
        for (auto &node : bank.nodes) {
            std::memset(node.keys, 0, sizeof(node.keys));
            for (std::int32_t &frame : node.keyFrames)
                frame = -1;
        }
    }
}
} // namespace

ModelExpressionRuntime *CheckModelExpressionHandle(std::uint32_t handle) {
    return g_modelExpressionCacheSlotTags[handle & 0x7fU] == (handle & 0xffffff80U) ? g_modelExpressionRuntime + (handle & 0x7fU) : nullptr;
}

std::uint32_t FindExpressionElementMask(std::uint64_t target, const char *name) {
    ModelCacheExpressionRecord *const record = GetModelCacheExpressionRecord(target);
    if (!record)
        return 0;
    D3TModelRecord *const model = ResolveModel(record->modelHandle);
    if (!model || !name || !*name || model->animations.boneMapCount > 32)
        return 0;
    for (std::uint32_t element = 0; element < model->animations.boneMapCount; ++element)
        if (!std::strcmp(reinterpret_cast<const char *>(model->animations.boneMaps[element].reserved_00), name))
            return 1U << element;
    return 0;
}

bool QueueModelExpression(std::uint64_t target, const ModelExpressionSubmission &submission) {
    ModelCacheExpressionRecord *record = GetModelCacheExpressionRecord(target);
    if (!record || record->expressionSlotCount >= 32)
        return false;
    D3TModelRecord *const model = ResolveModel(record->modelHandle);
    if (!model)
        return false;
    const auto *const definition = FindExpressionDefinition(model->animations, submission.name);
    if (!definition)
        return false;
    if (!(submission.flags & 4U)) {
        for (std::uint32_t index = 0; index < record->expressionSlotCount; ++index) {
            auto *const existing = CheckModelExpressionHandle(record->expressionSlots[index]);
            if (!existing || existing->definition != definition || existing->state != 1 ||
                (submission.elementMask & existing->submission.elementMask) != submission.elementMask)
                continue;
            if (definition->type == 1) {
                existing->blendInElapsed = 0;
                existing->submission.weight = submission.weight;
            }
            return false;
        }
    }
    const ModelExpressionDefinition *linkedA = nullptr;
    const ModelExpressionDefinition *linkedB = nullptr;
    if (definition->type == 1) {
        linkedA = FindExpressionDefinition(model->animations, definition->linkedA);
        linkedB = FindExpressionDefinition(model->animations, definition->linkedB);
        if (!linkedA || !linkedB)
            return false;
    }
    std::uint32_t index = g_nextModelExpressionSlot;
    while ((g_modelExpressionCacheSlotTags[index] >> 7) >= 0x1000000U) {
        index = (index + 1U) & 0x7fU;
        if (index == g_nextModelExpressionSlot)
            return false;
    }
    std::uint32_t generation = (g_modelExpressionCacheSlotTags[index] >> 7) + 1U;
    if ((generation & 0xff000000U) == 0x1000000U)
        generation = 1;
    const auto tag = (generation | 0xff000000U) << 7;
    g_modelExpressionCacheSlotTags[index] = tag;
    const std::uint32_t handle = tag | index;
    g_nextModelExpressionSlot = (index + 1U) & 0x7fU;
    if (!handle)
        return false;
    if (!(submission.flags & 8U)) {
        for (std::uint32_t old = 0; old < record->expressionSlotCount; ++old) {
            auto *const existing = CheckModelExpressionHandle(record->expressionSlots[old]);
            if (!existing || existing->state == 2 || !(existing->submission.elementMask & submission.elementMask))
                continue;
            const auto remainingMask = existing->submission.elementMask & ~submission.elementMask;
            if (remainingMask) {
                existing->submission.elementMask &= submission.elementMask;
                BuildExpressionAffectedNodeMasks(*record, *existing);
                auto split = existing->submission;
                split.elementMask = remainingMask;
                split.transitionDuration = 0;
                if (QueueModelExpression(target, split)) {
                    record = GetModelCacheExpressionRecord(target);
                    if (auto *const added = CheckModelExpressionHandle(record->expressionSlots[record->expressionSlotCount - 1])) {
                        added->state = existing->state;
                        added->currentWeight = existing->currentWeight;
                        added->blendInElapsed = existing->blendInElapsed;
                        added->blendOutElapsed = existing->blendOutElapsed;
                    }
                }
            }
            const float oldDuration =
                existing->submission.blendOutDuration > 0 ? existing->submission.blendOutDuration : existing->definition->blendOutDuration;
            const float newDuration = submission.transitionDuration > 0 ? submission.transitionDuration : definition->transitionDuration;
            existing->state = 2;
            existing->submission.blendOutDuration = std::fmax(newDuration, oldDuration);
        }
    }
    if (auto *const expression = CheckModelExpressionHandle(handle)) {
        record->expressionSlots[record->expressionSlotCount++] = handle;
        std::memset(expression, 0, sizeof(*expression));
        expression->definition = definition;
        expression->linkedA = linkedA;
        expression->linkedB = linkedB;
        expression->state = 1;
        expression->submission = submission;
        expression->submission.weight = std::fmax(0.0f, std::fmin(1.0f, submission.weight));
        expression->currentWeight = expression->submission.weight;
        BuildExpressionAffectedNodeMasks(*record, *expression);
        InitializeQueuedExpressionRuntime(*expression);
    }
    return true;
}

#if SHENMUE_IS_S2
// @verified - p
bool InterpolateExpressionNodeChannel(ModelExpressionNodeRuntime &node, std::int32_t frame, std::int32_t keyFrame, const void *key) {
    if (node.keyFrames[1] >= 0 && node.keyFrames[1] <= frame && node.keyFrames[2] > frame)
        return true;
    if (node.keyFrames[3] == -1) {
        for (std::uint32_t index = 0; index < 4; ++index) {
            std::memcpy(node.keys + index, key, sizeof(ModelExpressionTransform));
            node.keyFrames[index] = keyFrame;
        }
    } else {
        std::memmove(node.keys, node.keys + 1, 3 * sizeof(ModelExpressionTransform));
        std::memmove(node.keyFrames, node.keyFrames + 1, 3 * sizeof(std::int32_t));
        node.keyFrames[3] = keyFrame;
        std::memcpy(node.keys + 3, key, sizeof(ModelExpressionTransform));
    }
    return frame < keyFrame;
}

// @verified - p
void EvaluateExpressionChannel(const ModelExpressionDefinition &definition, ModelExpressionNodeRuntime &node, std::int32_t frame) {
    std::int32_t shifts = (node.keyFrames[3] <= frame) + 1;
    if (node.keyFrames[2] > frame)
        shifts = node.keyFrames[3] <= frame;
    while (shifts-- > 0) {
        std::memmove(node.keys, node.keys + 1, 3 * sizeof(ModelExpressionTransform));
        std::memmove(node.keyFrames, node.keyFrames + 1, 3 * sizeof(std::int32_t));
    }
    for (std::int32_t &keyFrame : node.keyFrames) {
        if (keyFrame < definition.firstFrame)
            keyFrame = definition.firstFrame;
        else if (keyFrame > definition.lastFrame)
            keyFrame = definition.lastFrame;
    }
}

// @verified - p
Vector3 EvaluateCatmullRomVector3(const Vector3 (&keys)[4], float fraction) {
    Vector3 result;
    for (std::size_t component = 0; component < 3; ++component) {
        const float a = keys[0][component], b = keys[1][component], c = keys[2][component], d = keys[3][component];
        result[component] = ((((a + a - b * 5.0f + c * 4.0f - d) + ((b - c) * 3.0f + d - a) * fraction) * fraction + (c - a)) * (fraction * 0.5f)) + b;
    }
    return result;
}

// @verified - p
Vector4 ApplyExpressionNodeResult(const Vector4 (&keys)[4], float fraction) {
    Vector4 result;
    for (std::size_t component = 0; component < 4; ++component) {
        const float a = keys[0][component], b = keys[1][component], c = keys[2][component], d = keys[3][component];
        result[component] = ((((a + a - b * 5.0f + c * 4.0f - d) + ((b - c) * 3.0f + d - a) * fraction) * fraction + (c - a)) * (fraction * 0.5f)) + b;
    }
    const float length = std::sqrt(((result.y * result.y + result.x * result.x) + result.z * result.z) + result.w * result.w);
    if (length >= 0.00000011920929f)
        for (float &component : result)
            component = (1.0f / length) * component;
    return result;
}

// @verified - p
void InterpolateExpressionRotation(ModelExpressionNodeRuntime &node, float fraction) {
    Vector3 translations[4], scales[4];
    Vector4 rotations[4];
    for (std::size_t index = 0; index < 4; ++index) {
        translations[index] = node.keys[index].translation;
        rotations[index] = node.keys[index].rotation;
        scales[index] = node.keys[index].scale;
    }
    node.sampled.translation = EvaluateCatmullRomVector3(translations, fraction);
    node.sampled.rotation = ApplyExpressionNodeResult(rotations, fraction);
    node.sampled.scale = EvaluateCatmullRomVector3(scales, fraction);
}

// @verified - p
void SampleExpressionAnimationNode(ModelExpressionNodeRuntime &node, float frame) {
    const float first = static_cast<float>(node.keyFrames[1]);
    const float distance = static_cast<float>(node.keyFrames[2]) - first;
    const float fraction = std::fabs(distance) <= 0.00000011920929f ? 0.0f : std::fmax(0.0f, std::fmin(1.0f, (frame - first) / distance));
    InterpolateExpressionRotation(node, fraction);
}

// @verified - p
std::int32_t DecodeExpressionAnimationNode(void *data) {
    if (!data)
        return 0;
    ModelExpressionDecodeTask &task = *static_cast<ModelExpressionDecodeTask *>(data);
    D3TModelRecord *const model = ResolveModel(task.record->modelHandle);
    if (!model)
        return 0;
    ModelExpressionRuntime &expression = *task.expression;
    const ModelExpressionDefinition &definition = *task.definition;
    ModelExpressionBank &bank = *task.bank;
    const D3TModelAnimation &animation = model->animations.animations[definition.animationIndex];
    float speed = static_cast<float>(definition.framesPerSecond);
    const float duration = static_cast<float>(static_cast<int>(definition.lastFrame) - definition.firstFrame);
    if (expression.submission.playbackDuration > 0.0f)
        speed = duration / expression.submission.playbackDuration;
    const float time = speed * task.elapsed;
    const std::uint32_t flags = expression.submission.flags;
    const float offset = (flags & 1U) || ((definition.playbackFlags & 1U) && !(flags & 2U)) ? std::fmod(time, duration) : std::fmin(duration, time);
    const std::int32_t frame = static_cast<std::uint16_t>(definition.firstFrame + static_cast<std::int32_t>(offset));
    if (frame != bank.selectedIndex) {
        std::uint64_t cursor = frame > bank.selectedIndex ? bank.keyOffset : 0;
        bool ready = false;
        while (cursor < animation.keyBytes && !ready) {
            std::int32_t keyFrame;
            std::uint32_t channelCount;
            std::memcpy(&keyFrame, animation.keys + cursor, 4);
            std::memcpy(&channelCount, animation.keys + cursor + 4, 4);
            cursor += 8;
            ready = true;
            for (std::uint32_t index = 0; index < channelCount; ++index) {
                std::uint32_t channel;
                std::memcpy(&channel, animation.keys + cursor, 4);
                cursor += 4;
                ready &= InterpolateExpressionNodeChannel(bank.nodes[channel], frame, keyFrame, animation.keys + cursor);
                const std::uint8_t components = static_cast<std::uint8_t>(animation.channels[channel] >> 16);
                cursor += ((components & 1U) ? 12 : 0) + ((components & 2U) ? 12 : 0) + ((components & 4U) ? 16 : 0);
            }
        }
        bank.keyOffset = cursor;
        bank.selectedIndex = frame;
        for (std::uint32_t channel = 0; channel < animation.channelCount; ++channel) {
            if (expression.submission.elementMask && !(expression.affectedNodeBits[channel >> 5] & (1U << (channel & 31U))))
                continue;
            EvaluateExpressionChannel(definition, bank.nodes[channel], frame);
        }
    }
    for (std::uint32_t channel = 0; channel < animation.channelCount; ++channel) {
        if (expression.submission.elementMask && !(expression.affectedNodeBits[channel >> 5] & (1U << (channel & 31U))))
            continue;
        SampleExpressionAnimationNode(bank.nodes[channel], static_cast<float>(definition.firstFrame) + offset);
    }
    return 0;
}

namespace {
void QueueExpressionDecode(ModelExpressionDecodeTask &task, std::uint32_t *&tasks) {
    if (tasks) {
        const D3TTaskDescriptor descriptor{.data = &task, .callback = DecodeExpressionAnimationNode};
        *tasks++ = d3t_QueueTask(&descriptor, sizeof(task));
    } else
        DecodeExpressionAnimationNode(&task);
}
void WaitExpressionTasks(const std::uint32_t *tasks, std::uint32_t count) {
    if (!tasks)
        return;
    for (;;) {
        bool active = false;
        for (std::uint32_t index = 0; index < count; ++index)
            if (d3t_IsTaskActive(tasks[index])) {
                active = true;
                break;
            }
        if (!active)
            return;
        if (!RunTask(0, tasks, count))
            std::this_thread::yield();
    }
}
}

// @verified - p
void UpdateModelExpressionSlot(ModelCacheExpressionRecord &record, ModelExpressionRuntime &expression, float elapsed, std::uint32_t *&tasks) {
    if (!ResolveModel(record.modelHandle))
        return;
    expression.blendInElapsed += elapsed;
    expression.banks[0].weight = 1.0f;
    const ModelExpressionDefinition &definition = *expression.definition;
    float duration = definition.transitionDuration;
    if (expression.submission.transitionDuration > 0.0f)
        duration = expression.submission.transitionDuration;
    if (duration > 0.0f && duration > expression.blendInElapsed) {
        const float fraction = std::fmax(0.0f, std::fmin(1.0f, expression.blendInElapsed / duration));
        expression.banks[0].weight = (3.0f - (fraction + fraction)) * (fraction * fraction);
    }
    if (expression.state == 2) {
        expression.blendOutElapsed += elapsed;
        duration = definition.blendOutDuration;
        if (expression.submission.blendOutDuration > 0.0f)
            duration = expression.submission.blendOutDuration;
        if (expression.blendOutElapsed >= duration) {
            expression.blendOutElapsed = duration;
            expression.state = 0;
        }
        if (duration <= 0.0f)
            expression.banks[0].weight = 0.0f;
        else {
            const float fraction = std::fmax(0.0f, std::fmin(1.0f, (expression.blendOutElapsed / duration - 1.0f) * -1.0f));
            expression.banks[0].weight *= (3.0f - (fraction + fraction)) * (fraction * fraction);
        }
        if (expression.banks[0].weight < 0.00000011920929f) {
            expression.blendOutElapsed = duration;
            expression.state = 0;
        }
    }
    if (!expression.state)
        return;
    ModelExpressionDecodeTask task{
        .record = &record, .expression = &expression, .definition = expression.definition, .bank = expression.banks, .elapsed = expression.blendInElapsed};
    if (definition.type == 1) {
        const float fraction = std::fmax(0.0f, std::fmin(1.0f, expression.blendInElapsed * 5.0f));
        expression.currentWeight = fraction * expression.submission.weight + (1.0f - fraction) * expression.currentWeight;
        const float weight = expression.submission.weightCallback ? expression.submission.weightCallback(&expression) : expression.currentWeight;
        if (weight <= 0.00000011920929f)
            task.definition = expression.linkedA;
        else if (weight >= 0.99999988f)
            task.definition = expression.linkedB;
        else {
            task.definition = expression.linkedA;
            QueueExpressionDecode(task, tasks);
            task.definition = expression.linkedB;
            task.bank = expression.banks + 1;
        }
    }
    QueueExpressionDecode(task, tasks);
}

namespace {

// @verified - p
Matrix4x4 MakeModelExpressionRotationMatrix(const Vector4 &q) {
    Matrix4x4 matrix = kModelCacheIdentity;
    const float zy = q.z * q.y, wy = q.w * q.y, yy = q.y * q.y;
    const float wx = q.w * q.x, yx = q.y * q.x, wz = q.w * q.z;
    matrix.rows[0].x = 1.0f - ((q.z * q.z + yy) + (q.z * q.z + yy));
    matrix.rows[0].y = (yx - wz) + (yx - wz);
    matrix.rows[1].x = (wz + yx) + (wz + yx);
    matrix.rows[0].z = (wy + q.z * q.x) + (wy + q.z * q.x);
    matrix.rows[2].x = (q.z * q.x - wy) + (q.z * q.x - wy);
    matrix.rows[2].z = 1.0f - ((yy + q.x * q.x) + (yy + q.x * q.x));
    matrix.rows[2].y = (zy + wx) + (zy + wx);
    matrix.rows[1].y = 1.0f - ((q.z * q.z + q.x * q.x) + (q.z * q.z + q.x * q.x));
    matrix.rows[1].z = (zy - wx) + (zy - wx);
    return matrix;
}


// @verified - p
Matrix4x4 MultiplyModelExpressionMatrices(const Matrix4x4 &first, const Matrix4x4 &second) {
    Matrix4x4 product;
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column)
            product.rows[row][column] =
                ((second.rows[row].x * first.rows[0][column] + second.rows[row].y * first.rows[1][column]) + second.rows[row].z * first.rows[2][column]) +
                second.rows[row].w * first.rows[3][column];
    }
    return product;
}
}

// @verified - p
void ApplyModelExpressionsToMatrices(std::uint32_t cacheHandle, ModelCacheExpressionRecord &record) {
    D3TModelRecord *const model = ResolveModel(record.modelHandle);
    if (!model)
        return;
    ModelExpressionMatrixContribution contributions[350]{};
    for (std::uint32_t slot = 0; slot < record.expressionSlotCount; ++slot) {
        ModelExpressionRuntime *const expression = CheckModelExpressionHandle(record.expressionSlots[slot]);
        if (!expression || !expression->definition || !(expression->banks[0].weight > 0.0f))
            continue;
        const std::uint32_t animationIndex = expression->definition->animationIndex;
        const std::uint32_t count = model->animations.animations[animationIndex].channelCount;
        const std::uint16_t *const nodes = reinterpret_cast<const std::uint16_t *>(record.boneTables[animationIndex].reserved00);
        const float weight = expression->submission.weightCallback ? expression->submission.weightCallback(expression) : expression->currentWeight;
        if (weight > 0.00000011920929f && weight < 0.99999988f) {
            for (std::uint32_t channel = 0; channel < count; ++channel) {
                if (expression->submission.elementMask && !(expression->affectedNodeBits[channel >> 5] & (1U << (channel & 31U))))
                    continue;
                ModelExpressionTransform &first = expression->banks[0].nodes[channel].sampled;
                const ModelExpressionTransform &second = expression->banks[1].nodes[channel].sampled;
                for (std::size_t component = 0; component < 3; ++component)
                    first.translation[component] += (second.translation[component] - first.translation[component]) * weight;
                for (std::size_t component = 0; component < 4; ++component)
                    first.rotation[component] += (second.rotation[component] - first.rotation[component]) * weight;
                for (std::size_t component = 0; component < 3; ++component)
                    first.scale[component] += (second.scale[component] - first.scale[component]) * weight;
            }
        }
        for (std::uint32_t channel = 0; channel < count; ++channel) {
            if (expression->submission.elementMask && !(expression->affectedNodeBits[channel >> 5] & (1U << (channel & 31U))))
                continue;
            std::uint32_t index = 0;
            while (index < 350 && contributions[index].node != nodes[channel])
                ++index;
            if (index == 350) {
                index = 0;
                while (index < 350 && contributions[index].node)
                    ++index;
                if (index == 350)
                    continue;
                contributions[index].node = nodes[channel];
            }
            ++contributions[index].count;
            contributions[index].weight += expression->banks[0].weight;
        }
    }
    for (std::uint32_t slot = 0; slot < record.expressionSlotCount; ++slot) {
        ModelExpressionRuntime *const expression = CheckModelExpressionHandle(record.expressionSlots[slot]);
        if (!expression || !expression->definition || !(expression->banks[0].weight > 0.0f))
            continue;
        const std::uint32_t animationIndex = expression->definition->animationIndex;
        const std::uint32_t count = model->animations.animations[animationIndex].channelCount;
        const std::uint16_t *const nodes = reinterpret_cast<const std::uint16_t *>(record.boneTables[animationIndex].reserved00);
        for (std::uint32_t channel = 0; channel < count; ++channel) {
            if (expression->submission.elementMask && !(expression->affectedNodeBits[channel >> 5] & (1U << (channel & 31U))))
                continue;
            std::uint32_t index = 0;
            while (index < 350 && contributions[index].node != nodes[channel])
                ++index;
            if (index == 350)
                continue;
            const ModelExpressionTransform &sampled = expression->banks[0].nodes[channel].sampled;
            Matrix4x4 scale{};
            scale.rows[0].x = sampled.scale.x;
            scale.rows[1].y = sampled.scale.y;
            scale.rows[2].z = sampled.scale.z;
            scale.rows[3].w = 1.0f;
            const Matrix4x4 rotation = MakeModelExpressionRotationMatrix(sampled.rotation);
            Matrix4x4 matrix = MultiplyModelExpressionMatrices(scale, rotation);
            matrix.rows[0].w = sampled.translation.x;
            matrix.rows[1].w = sampled.translation.y;
            matrix.rows[2].w = sampled.translation.z;
            const float weight = expression->banks[0].weight / contributions[index].weight;
            for (std::size_t row = 0; row < 4; ++row)
                for (std::size_t component = 0; component < 4; ++component)
                    contributions[index].matrix.rows[row][component] += matrix.rows[row][component] * weight;
        }
    }
    for (const ModelExpressionMatrixContribution &contribution : contributions) {
        if (!contribution.node || !contribution.count)
            continue;
        const std::uint64_t target = (static_cast<std::uint64_t>(record.nodeHash + contribution.node) << 32) | cacheHandle;
        Matrix4x4 matrix = GetModelCacheTransform(target);
        for (std::size_t row = 0; row < 4; ++row)
            for (std::size_t component = 0; component < 4; ++component)
                matrix.rows[row][component] += (contribution.matrix.rows[row][component] - matrix.rows[row][component]) * 0.75f;
        SetModelCacheTransform(target, matrix);
    }
}

// @verified - p
std::int32_t ModelExpressionNodeTaskCallback(void *data) {
    if (!data)
        return 0;
    ModelExpressionApplyTask &task = *static_cast<ModelExpressionApplyTask *>(data);
    ModelCacheExpressionRecord &record = *task.record;
    if (!record.expressionSlotCount)
        return 0;
    ApplyModelExpressionsToMatrices(task.cacheHandle, record);
    std::uint32_t index = 0;
    while (index < record.expressionSlotCount) {
        const std::uint32_t handle = record.expressionSlots[index];
        ModelExpressionRuntime *const expression = CheckModelExpressionHandle(handle);
        if (expression && expression->state) {
            ++index;
            continue;
        }
        if (expression)
            g_modelExpressionCacheSlotTags[handle & 0x7fU] &= 0x7fffffffU;
        if (index < static_cast<std::uint32_t>(record.expressionSlotCount - 1))
            std::memmove(record.expressionSlots + index, record.expressionSlots + index + 1, 4ULL * (record.expressionSlotCount - index - 1));
        --record.expressionSlotCount;
    }
    return 0;
}

// @verified - p
void UpdateModelExpressionAnimations(std::uint32_t cacheHandle, ModelCacheExpressionTarget &target, float elapsed) {
    if (!target.count)
        return;
    const std::lock_guard lock(g_modelCacheExpressionMutex);
    const std::uint32_t taskCapacity = target.count << 6;
    std::uint32_t *const tasks = static_cast<std::uint32_t *>(s2::GetRendererScratchBlock(4ULL * taskCapacity));
    std::uint32_t *nextTask = tasks;
    if (tasks)
        std::memset(tasks, 0, 4ULL * taskCapacity);
    for (std::uint32_t record = 0; record < target.count; ++record) {
        ModelCacheExpressionRecord &node = target.records[record];
        for (std::uint32_t slot = 0; slot < node.expressionSlotCount; ++slot) {
            ModelExpressionRuntime *const expression = CheckModelExpressionHandle(node.expressionSlots[slot]);
            if (expression)
                UpdateModelExpressionSlot(node, *expression, elapsed, nextTask);
        }
    }
    WaitExpressionTasks(tasks, taskCapacity);
    nextTask = tasks;
    if (tasks)
        std::memset(tasks, 0, 4ULL * target.count);
    for (std::uint32_t record = 0; record < target.count; ++record) {
        ModelExpressionApplyTask task{.cacheHandle = cacheHandle, .record = target.records + record, .elapsed = elapsed};
        if (nextTask) {
            const D3TTaskDescriptor descriptor{.data = &task, .callback = ModelExpressionNodeTaskCallback};
            *nextTask++ = d3t_QueueTask(&descriptor, sizeof(task));
        } else
            ModelExpressionNodeTaskCallback(&task);
    }
    WaitExpressionTasks(tasks, target.count);
    if (tasks)
        s2::g_rendererScratch.current = reinterpret_cast<std::byte *>(tasks);
}

// @verified - p
void TexSortWrapper(float elapsed) {
    for (std::uint32_t index = 0; index < 128; ++index) {
        const std::uint32_t tag = g_modelCache.generations[index];
        if (tag < 0x80000000U)
            continue;
        UpdateModelExpressionAnimations(tag | index, g_modelCache.slots[index].expressions, elapsed);
    }
}
#endif

} // namespace shenmue::d3t
