#include "s1/skinning_cache.hpp"

#include "s1/archive_data.hpp"
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/render_offsets.hpp"
#include "s1/d3t/scratch_memory.hpp"
#include "s1/d3t/d3t_threads.hpp"
#include "s1/matrix_stack.hpp"

#include <algorithm>
#include <atomic>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <thread>
#include <unordered_map>

namespace shenmue::s1 {

SkinningCacheStorage g_CachedSkinningBuffers{};
SkinningWorkCacheStorage g_skinningWorkCache{};
std::uint32_t g_skinningWorkCacheCounter = 0;
std::uint64_t g_mainSkinningRoot = 0;
std::uint64_t g_sdOverrideSkinningRoot = 0;
std::uint64_t g_skySkinningRoot = 0;
std::uint64_t g_skyProbeSkinningRoot = 0;

namespace {

std::atomic<std::int32_t> g_skinningReaders{0};
std::atomic<std::uint8_t> g_skinningWriter{0};






std::unordered_map<std::uint64_t, std::string> g_modelSkinningNodeNames;

template <typename T> T Read(const std::byte *data, std::size_t offset) {
    T value{};
    std::memcpy(&value, data + offset, sizeof(value));
    return value;
}

template <typename T> void Write(std::byte *data, std::size_t offset, const T &value) {
    std::memcpy(data + offset, &value, sizeof(value));
}

const std::byte *FindExpressionMetadata(const std::byte *renderPayload, const char *name) {
    if (!renderPayload || !name || *name == '\0')
        return nullptr;
    const auto *const entries = Read<const std::byte *>(renderPayload, 0x10);
    const std::uint32_t count = Read<std::uint32_t>(renderPayload, 0x18);
    if (!entries)
        return nullptr;
    for (std::uint32_t index = 0; index < count; ++index) {
        const std::byte *const entry = entries + static_cast<std::size_t>(index) * 0xd4;
        const auto *candidate = reinterpret_cast<const unsigned char *>(entry + 4);
        const auto *requested = reinterpret_cast<const unsigned char *>(name);
        while (true) {
            const int left = std::tolower(*candidate++);
            const int right = std::tolower(*requested++);
            if (left != right)
                break;
            if (left == 0)
                return entry;
        }
    }
    return nullptr;
}

void InitializeSkinningWorkEntry(std::byte *workCache) {
    constexpr std::array<std::size_t, 2> poolOffsets{0x40, 0x127a0};
    for (const std::size_t poolOffset : poolOffsets) {
        const std::int32_t unused = -1;
        const std::uint64_t empty = 0;
        Write(workCache, poolOffset - 0x0c, unused);
        Write(workCache, poolOffset - 0x08, empty);
        for (std::size_t track = 0; track < 350; ++track) {
            std::byte *const state = workCache + poolOffset + track * sizeof(SkinningTrackState);
            std::memset(state, 0, 0xa0);
            for (std::size_t frame = 0; frame < 4; ++frame) {
                Write(state, 0xa8 + frame * sizeof(std::int32_t), unused);
            }
        }
    }
}

void SetSkinningTargetMask(const SkinningTexSortRecord *record, std::byte *workCache) {
    const std::uint32_t targetMask = Read<std::uint32_t>(workCache, 0x24efc);
    if (!record || !record->boneTables || targetMask == 0)
        return;
    const auto *const renderPayload = static_cast<const std::byte *>(d3t::GetPointerFromOffset(record->renderHandle));
    if (!renderPayload)
        return;
    const auto *const groups = Read<const std::byte *>(renderPayload, 0x20);
    const std::uint32_t groupCount = Read<std::uint32_t>(renderPayload, 0x28);
    const auto *const metadata = Read<const std::byte *>(workCache, 0x00);
    if (!groups || !metadata)
        return;
    const std::uint16_t tableIndex = Read<std::uint16_t>(metadata, 0x44);
    if (tableIndex >= record->boneTableCount)
        return;
    const std::byte *const table = record->boneTables + static_cast<std::size_t>(tableIndex) * 0x2d0;
    const auto *const masks = Read<const std::byte *>(table, 0x2c0);
    const std::uint32_t maskCount = Read<std::uint32_t>(table, 0x2c8);
    if (!masks)
        return;

    std::memset(workCache + 0x24f20, 0, 0x2c);
    std::uint32_t remaining = targetMask;
    for (std::uint32_t bit = 0; bit < 32 && remaining != 0; ++bit) {
        const std::uint32_t value = 1U << bit;
        if ((remaining & value) == 0)
            continue;
        remaining &= ~value;
        const std::byte *const group = groups + static_cast<std::size_t>(bit) * 0x50;
        const std::ptrdiff_t groupIndex = (group - groups) / 0x50;
        if (bit >= groupCount || groupIndex < 0 || static_cast<std::uint32_t>(groupIndex) >= maskCount) {
            continue;
        }
        const std::byte *const source = masks + static_cast<std::size_t>(groupIndex) * 0x2c;
        for (std::size_t word = 0; word < 11; ++word) {
            const std::uint32_t combined = Read<std::uint32_t>(workCache, 0x24f20 + word * 4) | Read<std::uint32_t>(source, word * 4);
            Write(workCache, 0x24f20 + word * 4, combined);
        }
    }
}

std::uint32_t AllocateSkinningWorkHandle() {
    const std::uint32_t first = g_skinningWorkCacheCounter;
    std::uint32_t slot = first;
    while ((g_skinningWorkCache.generations[slot] >> 7U) >= 0x1000000U) {
        slot = (slot + 1U) & 0x7fU;
        if (slot == first)
            return 0;
    }
    const std::uint32_t next = (g_skinningWorkCache.generations[slot] >> 7U) + 1U;
    const std::uint32_t counter = (next & 0xff000000U) == 0x01000000U ? 1U : next;
    const std::uint32_t generation = (counter | 0xff000000U) << 7U;
    g_skinningWorkCache.generations[slot] = generation;
    g_skinningWorkCacheCounter = (slot + 1U) & 0x7fU;
    return generation | slot;
}

void LockSkinningWriter() {
    while (true) {
        while (g_skinningReaders.load(std::memory_order_seq_cst) != 0) {
            std::this_thread::yield();
        }
        std::uint8_t expected = 0;
        if (!g_skinningWriter.compare_exchange_strong(expected, 1, std::memory_order_seq_cst)) {
            continue;
        }
        if (g_skinningReaders.load(std::memory_order_seq_cst) == 0)
            return;
        g_skinningWriter.store(0, std::memory_order_seq_cst);
    }
}

void UnlockSkinningWriter() {
    g_skinningWriter.store(0, std::memory_order_seq_cst);
}

void LockSkinningReader() {
    g_skinningReaders.fetch_add(1, std::memory_order_seq_cst);
    while (g_skinningWriter.load(std::memory_order_seq_cst) != 0) {
        std::this_thread::yield();
    }
}

void UnlockSkinningReader() {
    g_skinningReaders.fetch_sub(1, std::memory_order_seq_cst);
}

float CatmullRom(float p0, float p1, float p2, float p3, float amount) {
    return p1 + 0.5F * amount * ((p2 - p0) + amount * ((2.0F * p0 - 5.0F * p1 + 4.0F * p2 - p3) + amount * (3.0F * (p1 - p2) + p3 - p0)));
}

float SmoothStep(float amount) {
    return (3.0F - 2.0F * amount) * amount * amount;
}

template <typename Vector> void InterpolateComponents(Vector &destination, const SkinningTrackState &state, std::size_t sourceOffset, float amount) {
    const auto component = [&](std::size_t keyframe, std::size_t index) {
        const auto *const source = reinterpret_cast<const float *>(reinterpret_cast<const std::byte *>(&state.keyframes[keyframe]) + sourceOffset);
        return source[index];
    };
    for (std::size_t index = 0; index < destination.size(); ++index) {
        destination[index] = CatmullRom(component(0, index), component(1, index), component(2, index), component(3, index), amount);
    }
}

bool IsTrackEnabled(const std::byte *workCache, std::uint32_t track) {
    if (Read<std::uint32_t>(workCache, 0x24efc) == 0)
        return true;
    const std::uint32_t word = Read<std::uint32_t>(workCache, 0x24f20 + (track >> 5U) * sizeof(std::uint32_t));
    return (word & (1U << (track & 31U))) != 0;
}

Matrix4x4 QuaternionMatrix(const float *quaternion) {
    const float x = quaternion[0];
    const float y = quaternion[1];
    const float z = quaternion[2];
    const float w = quaternion[3];
    return Matrix4x4{{
        {1.0F - 2.0F * (z * z + y * y), 2.0F * (x * y - w * z), 2.0F * (w * y + z * x), 0.0F},
        {2.0F * (w * z + x * y), 1.0F - 2.0F * (z * z + x * x), 2.0F * (z * y - w * x), 0.0F},
        {2.0F * (z * x - w * y), 2.0F * (z * y + w * x), 1.0F - 2.0F * (y * y + x * x), 0.0F},
        {0.0F, 0.0F, 0.0F, 1.0F},
    }};
}

Matrix4x4 MultiplyMatrices(const Matrix4x4 &left, const Matrix4x4 &right) {
    Matrix4x4 output{};
    const auto *const lhs = reinterpret_cast<const float *>(&left);
    const auto *const rhs = reinterpret_cast<const float *>(&right);
    auto *const result = reinterpret_cast<float *>(&output);
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            float value = 0.0F;
            for (std::size_t item = 0; item < 4; ++item) {
                value += lhs[row * 4 + item] * rhs[item * 4 + column];
            }
            result[row * 4 + column] = value;
        }
    }
    return output;
}

void AddScaledMatrix(Matrix4x4 &destination, const Matrix4x4 &source, float scale) {
    auto *const output = reinterpret_cast<float *>(&destination);
    const auto *const input = reinterpret_cast<const float *>(&source);
    for (std::size_t index = 0; index < 16; ++index) {
        output[index] += input[index] * scale;
    }
}

constexpr Matrix4x4 kIdentityMatrix{{
    {1.0F, 0.0F, 0.0F, 0.0F},
    {0.0F, 1.0F, 0.0F, 0.0F},
    {0.0F, 0.0F, 1.0F, 0.0F},
    {0.0F, 0.0F, 0.0F, 1.0F},
}};

std::uint32_t g_cachedSkinningBufferCursor = 0;

bool ResizeSkinningEntries(CachedSkinningBufferSlot &slot, std::uint32_t capacity) {
    if (capacity <= slot.entryCapacity)
        return true;
    auto *const entries = static_cast<std::byte *>(std::calloc(capacity, 20U));
    auto *const matrices = static_cast<Matrix4x4 *>(std::calloc(capacity, sizeof(Matrix4x4)));
    auto *const secondaryMatrices = static_cast<Matrix4x4 *>(std::calloc(capacity, sizeof(Matrix4x4)));
    auto *const constantBufferHandles = static_cast<std::uint32_t *>(std::calloc(capacity, sizeof(std::uint32_t)));
    auto *const renderHandles = static_cast<std::uint32_t *>(std::calloc(capacity, sizeof(std::uint32_t)));
    auto *const nodeHashes = static_cast<std::uint32_t *>(std::calloc(capacity, sizeof(std::uint32_t)));
    auto *const floatValues = static_cast<float *>(std::calloc(capacity, sizeof(float)));
    if (!entries || !matrices || !secondaryMatrices || !constantBufferHandles || !renderHandles || !nodeHashes || !floatValues) {
        std::free(entries);
        std::free(matrices);
        std::free(secondaryMatrices);
        std::free(constantBufferHandles);
        std::free(renderHandles);
        std::free(nodeHashes);
        std::free(floatValues);
        return false;
    }

    const std::size_t count = slot.entryCount;
    if (count) {
        std::memcpy(entries, slot.entries, count * 20U);
        std::memcpy(matrices, slot.matrices, count * sizeof(Matrix4x4));
        std::memcpy(secondaryMatrices, slot.secondaryMatrices, count * sizeof(Matrix4x4));
        std::memcpy(constantBufferHandles, slot.constantBufferHandles, count * sizeof(std::uint32_t));
        std::memcpy(renderHandles, slot.renderHandles, count * sizeof(std::uint32_t));
        std::memcpy(nodeHashes, slot.parentHashes, count * sizeof(std::uint32_t));
        std::memcpy(floatValues, slot.floatValues, count * sizeof(float));
    }
    for (std::uint32_t index = slot.entryCount; index < capacity; ++index) {
        matrices[index] = kIdentityMatrix;
        secondaryMatrices[index] = kIdentityMatrix;
        floatValues[index] = 1.0F;
    }
    std::free(slot.entries);
    std::free(slot.matrices);
    std::free(slot.secondaryMatrices);
    std::free(slot.constantBufferHandles);
    std::free(slot.renderHandles);
    std::free(slot.parentHashes);
    std::free(slot.floatValues);
    slot.entries = entries;
    slot.matrices = matrices;
    slot.secondaryMatrices = secondaryMatrices;
    slot.constantBufferHandles = constantBufferHandles;
    slot.renderHandles = renderHandles;
    slot.parentHashes = nodeHashes;
    slot.floatValues = floatValues;
    slot.entryCapacity = capacity;
    return true;
}

void InsertMurmurValue(MurmurValueTable &table, std::uint32_t key, std::uint32_t value) {
    const std::uint32_t hash = d3t_HashData(&key, sizeof(key));
    std::uint32_t index = hash % table.capacity;
    while (table.hashes[index] != 0) {
        if (table.hashes[index] == hash) {
            table.values[index] = value;
            return;
        }
        if (table.probes[index] == 0) {
            std::uint32_t next = (index + 1U) % table.capacity;
            std::uint32_t distance = 1;
            while (table.hashes[next] != 0) {
                next = (next + 1U) % table.capacity;
                if (++distance >= table.capacity)
                    return;
            }
            table.probes[index] = static_cast<std::uint16_t>(distance);
            index = next;
            break;
        }
        index = (index + table.probes[index]) % table.capacity;
    }
    table.hashes[index] = hash;
    table.values[index] = value;
}

bool RebuildMurmurTable(CachedSkinningBufferSlot &slot) {
    std::uint32_t capacity = 16;
    while (capacity < slot.entryCount * 2U)
        capacity *= 2U;
    auto *const values = static_cast<std::uint32_t *>(std::calloc(capacity, sizeof(std::uint32_t)));
    auto *const hashes = static_cast<std::uint32_t *>(std::calloc(capacity, sizeof(std::uint32_t)));
    auto *const probes = static_cast<std::uint16_t *>(std::calloc(capacity, sizeof(std::uint16_t)));
    if (!values || !hashes || !probes) {
        std::free(values);
        std::free(hashes);
        std::free(probes);
        return false;
    }
    std::free(slot.valueTable.values);
    std::free(slot.valueTable.hashes);
    std::free(slot.valueTable.probes);
    slot.valueTable = {values, hashes, probes, capacity};
    for (std::uint32_t index = 0; index < slot.entryCount; ++index)
        InsertMurmurValue(slot.valueTable, slot.parentHashes[index], index);
    return true;
}

void ReindexMurmurTable(CachedSkinningBufferSlot &slot) {
    if (!slot.valueTable.capacity || !slot.valueTable.values || !slot.valueTable.hashes || !slot.valueTable.probes) {
        return;
    }
    std::memset(slot.valueTable.values, 0, static_cast<std::size_t>(slot.valueTable.capacity) * sizeof(*slot.valueTable.values));
    std::memset(slot.valueTable.hashes, 0, static_cast<std::size_t>(slot.valueTable.capacity) * sizeof(*slot.valueTable.hashes));
    std::memset(slot.valueTable.probes, 0, static_cast<std::size_t>(slot.valueTable.capacity) * sizeof(*slot.valueTable.probes));
    for (std::uint32_t index = 0; index < slot.entryCount; ++index)
        InsertMurmurValue(slot.valueTable, slot.parentHashes[index], index);
}

void UpdateAncestorSubtreeSizes(CachedSkinningBufferSlot &slot, std::uint32_t nodeIndex, std::int32_t delta) {
    std::uint32_t index = nodeIndex;
    std::uint32_t remaining = slot.entryCount;
    while (index && remaining--) {
        const std::byte *const node = slot.entries + static_cast<std::size_t>(index) * 20U;
        const std::uint16_t parentDistance = Read<std::uint16_t>(node, 0x04);
        if (!parentDistance || parentDistance > index)
            break;
        index -= parentDistance;
        std::byte *const parent = slot.entries + static_cast<std::size_t>(index) * 20U;
        const std::int32_t subtreeSize = Read<std::uint16_t>(parent, 0x06) + delta;
        Write(parent, 0x06, static_cast<std::uint16_t>(std::clamp(subtreeSize, 1, static_cast<std::int32_t>(UINT16_MAX))));
    }
}

std::uint32_t AppendSkinningName(CachedSkinningBufferSlot &slot, const char *name) {
    if (!slot.names || !name)
        return UINT32_MAX;
    const std::size_t length = std::strlen(name) + 1U;
    for (std::uint32_t offset = 0; offset < slot.namesSize;) {
        const char *const candidate = slot.names + offset;
        const std::size_t candidateLength = std::strlen(candidate) + 1U;
        if (candidateLength == length && std::memcmp(candidate, name, candidateLength) == 0) {
            return offset;
        }
        offset += static_cast<std::uint32_t>(candidateLength);
    }
    if (length > slot.namesCapacity - slot.namesSize)
        return UINT32_MAX;
    const std::uint32_t offset = slot.namesSize;
    std::memcpy(slot.names + offset, name, length);
    slot.namesSize += static_cast<std::uint32_t>(length);
    return offset;
}

std::uint64_t AppendSkinningNode(CachedSkinningBufferSlot &slot, std::uint32_t cacheHandle, std::uint32_t parentIndex, const char *nodeName,
    std::uint32_t renderHandle, bool modelOwnedName) {
    if (slot.entryCount == slot.entryCapacity && !ResizeSkinningEntries(slot, slot.entryCapacity ? slot.entryCapacity + 1U : 10U)) {
        return 0;
    }
    const std::uint32_t index = slot.entryCount;
    if (parentIndex > index || index - parentIndex > UINT16_MAX)
        return 0;
    const std::uint32_t nameOffset = modelOwnedName ? 0U : AppendSkinningName(slot, nodeName);
    if (nameOffset == UINT32_MAX)
        return 0;
    std::byte *const entry = slot.entries + static_cast<std::size_t>(index) * 20U;
    Write(entry, 0x00, nameOffset);
    const std::uint16_t parentDistance = static_cast<std::uint16_t>(index - parentIndex);
    const std::uint16_t subtreeSize = 1;
    const std::uint32_t zero = 0;
    Write(entry, 0x04, parentDistance);
    Write(entry, 0x06, subtreeSize);
    Write(entry, 0x08, zero);
    Write(entry, 0x10, zero);
    slot.matrices[index] = kIdentityMatrix;
    slot.secondaryMatrices[index] = kIdentityMatrix;
    slot.constantBufferHandles[index] = 0;
    slot.renderHandles[index] = renderHandle;
    slot.floatValues[index] = 1.0F;
    std::uint32_t nodeHash = slot.nextNodeHash++;
    if (!nodeHash)
        nodeHash = slot.nextNodeHash++;
    slot.parentHashes[index] = nodeHash;
    ++slot.entryCount;
    if (!RebuildMurmurTable(slot)) {
        --slot.entryCount;
        if (!modelOwnedName)
            slot.namesSize = nameOffset;
        return 0;
    }
    UpdateAncestorSubtreeSizes(slot, index, 1);
    const std::uint64_t handle = (static_cast<std::uint64_t>(nodeHash) << 32U) | cacheHandle;
    if (modelOwnedName)
        g_modelSkinningNodeNames[handle] = nodeName;
    return handle;
}


void FreeSkinningSlot(CachedSkinningBufferSlot &slot) {
    std::free(slot.entries);
    std::free(slot.matrices);
    std::free(slot.secondaryMatrices);
    if (slot.constantBufferHandles) {
        for (std::uint32_t index = 0; index < slot.entryCount; ++index)
            ValidateAndCommitBuffer(static_cast<std::int32_t>(slot.constantBufferHandles[index]));
        std::free(slot.constantBufferHandles);
    }
    std::free(slot.renderHandles);
    std::free(slot.parentHashes);
    std::free(slot.floatValues);
    std::free(slot.skinRanges);
    slot.skinRanges = nullptr;
    slot.skinRangeCount = 0;
    slot.skinRangeCapacity = 0;
    std::free(slot.boneIndices);
    slot.boneIndices = nullptr;
    slot.boneIndexCount = 0;
    slot.boneIndexCapacity = 0;
    std::free(slot.sprites);
    slot.sprites = nullptr;
    slot.spriteCount = 0;
    slot.spriteCapacity = 0;
    std::free(slot.valueTable.values);
    std::free(slot.valueTable.hashes);
    std::free(slot.valueTable.probes);
    std::free(slot.names);



    auto *const batch = reinterpret_cast<SkinningTexSortBatch *>(reinterpret_cast<std::byte *>(&slot) + 0xb8);
    LockSkinningWriter();
    for (std::uint32_t index = 0; index < batch->recordCount; ++index) {
        auto *const record = reinterpret_cast<SkinningTexSortRecord *>(batch->records + static_cast<std::size_t>(index) * sizeof(SkinningTexSortRecord));
        std::free(record->boneTables);
        record->boneTables = nullptr;
        record->boneTableCount = 0;
    }
    std::free(batch->records);
    batch->records = nullptr;
    batch->recordCount = 0;
    UnlockSkinningWriter();
    ValidateAndCommitBuffer(static_cast<std::int32_t>(slot.skinningBuffer));
}

} // namespace

std::uint64_t AllocateCachedSkinningBuffer(const char *name) {
    const std::uint32_t first = g_cachedSkinningBufferCursor;
    std::uint32_t index = first;
    while ((g_CachedSkinningBuffers.generations[index] >> 7U) >= 0x01000000U) {
        index = (index + 1U) & 0x7fU;
        if (index == first)
            return 0;
    }
    const std::uint32_t next = (g_CachedSkinningBuffers.generations[index] >> 7U) + 1U;
    const std::uint32_t counter = (next & 0xff000000U) == 0x01000000U ? 1U : next;
    const std::uint32_t generation = (counter | 0xff000000U) << 7U;
    const std::uint32_t cacheHandle = generation | index;
    g_CachedSkinningBuffers.generations[index] = generation;
    g_cachedSkinningBufferCursor = (index + 1U) & 0x7fU;

    auto &storage = g_CachedSkinningBuffers.entries[index];
    std::memset(storage.data(), 0, storage.size());
    auto &slot = *reinterpret_cast<CachedSkinningBufferSlot *>(storage.data());
    slot.nextNodeHash = 1;
    slot.namesCapacity = 4000;
    slot.names = static_cast<char *>(std::malloc(slot.namesCapacity));
    if (!slot.names || !ResizeSkinningEntries(slot, 10U)) {
        FreeSkinningSlot(slot);
        std::memset(storage.data(), 0, storage.size());
        g_CachedSkinningBuffers.generations[index] &= 0x7fffffffU;
        return 0;
    }
    if (name) {
        std::strncpy(reinterpret_cast<char *>(storage.data() + 0xc8), name, 0x3f);
    }
    const std::uint64_t root = AppendSkinningNode(slot, cacheHandle, 0, "SceneRoot", 0, false);
    if (!root) {
        FreeSkinningSlot(slot);
        std::memset(storage.data(), 0, storage.size());
        g_CachedSkinningBuffers.generations[index] &= 0x7fffffffU;
    }
    if (root) {
        const BufferDescription description{.flags = 3, .size = 1835008, .count = 224, .flags3 = 2, .buffer = nullptr, .name = "Skinning Buffer"};
        slot.skinningBuffer = static_cast<std::uint32_t>(CreateBuffer(&description));
    }
    return root;
}

void ReleaseCachedSkinningBuffer(std::uint64_t handle) {
    const std::uint32_t cacheHandle = static_cast<std::uint32_t>(handle);
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(cacheHandle);
    if (!slot)
        return;
    const std::uint32_t index = cacheHandle & 0x7fU;
    for (auto name = g_modelSkinningNodeNames.begin(); name != g_modelSkinningNodeNames.end();) {
        if (static_cast<std::uint32_t>(name->first) == cacheHandle)
            name = g_modelSkinningNodeNames.erase(name);
        else
            ++name;
    }
    FreeSkinningSlot(*slot);
    std::memset(g_CachedSkinningBuffers.entries[index].data(), 0, g_CachedSkinningBuffers.entries[index].size());
    g_CachedSkinningBuffers.generations[index] &= 0x7fffffffU;
    if (static_cast<std::uint32_t>(g_mainSkinningRoot) == cacheHandle) {
        g_mainSkinningRoot = 0;
    }
    if (static_cast<std::uint32_t>(g_sdOverrideSkinningRoot) == cacheHandle) {
        g_sdOverrideSkinningRoot = 0;
    }
    if (static_cast<std::uint32_t>(g_skySkinningRoot) == cacheHandle) {
        g_skySkinningRoot = 0;
    }
    if (static_cast<std::uint32_t>(g_skyProbeSkinningRoot) == cacheHandle) {
        g_skyProbeSkinningRoot = 0;
    }
}



template <typename Element> Element *AppendSkinningTable(Element *&table, std::uint32_t &count, std::uint32_t &capacity, std::uint32_t added) {
    std::uint32_t required = count + added;
    if (required > capacity) {
        const std::uint32_t grown = capacity * 2U >= 16U ? capacity * 2U : 16U;
        if (grown >= required)
            required = grown;
        if (required > capacity) {
            Element *const resized = static_cast<Element *>(std::realloc(table, static_cast<std::size_t>(required) * sizeof(Element)));
            if (!resized)
                return nullptr;
            table = resized;
            capacity = required;
        }
    }
    count += added;
    return table + (count - added);
}

struct SkinningSprite {
    std::byte bytes[sizeof(D3TModelSprite)];
};

struct SkinningProbe {
    std::byte bytes[sizeof(D3TModelProbe)];
};

struct SkinningRange {
    std::uint32_t nodeHash;
    std::uint32_t firstBone;
    std::uint32_t boneCount;
};





std::uint64_t AppendModelToCachedSkinningBuffer(std::uint32_t cacheHandle, std::uint32_t modelHandle) {
    if (!d3t::SetupShaderReferences(modelHandle))
        return 0;
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(cacheHandle);
    D3TModelRecord *const model = d3t::ResolveModel(modelHandle);
    if (!slot || !model)
        return 0;
    const std::uint32_t count = model->nodeCount;
    if (!count)
        return 0;
    if (slot->entryCapacity - slot->entryCount < count && !ResizeSkinningEntries(*slot, slot->entryCount + count))
        return 0;
    const std::uint32_t first = slot->entryCount;
    std::memcpy(slot->entries + static_cast<std::size_t>(first) * 20U, model->nodes, static_cast<std::size_t>(count) * sizeof(D3TModelNode));
    std::memcpy(slot->matrices + first, model->nodeTransforms, static_cast<std::size_t>(count) * sizeof(Matrix4x4));
    const std::uint32_t firstHash = slot->nextNodeHash;
    std::uint32_t nodeHash = firstHash;
    for (std::uint32_t index = first; index < first + count; ++index) {
        slot->secondaryMatrices[index].rows[0].x = 3.40282347e+38f;
        slot->renderHandles[index] = modelHandle;
        slot->parentHashes[index] = nodeHash++;
        slot->floatValues[index] = 1.0f;
        slot->constantBufferHandles[index] = 0;
    }
    slot->nextNodeHash = nodeHash;
    Write(slot->entries + static_cast<std::size_t>(first) * 20U, 0x04, static_cast<std::uint16_t>(first));
    Write(slot->entries, 0x08, static_cast<std::uint16_t>(Read<std::uint16_t>(slot->entries, 0x08) + 1U));
    slot->entryCount += count;
    Write(slot->entries, 0x06, static_cast<std::uint16_t>(Read<std::uint16_t>(slot->entries, 0x06) + count));
    RebuildMurmurTable(*slot);

    const std::uint32_t spriteBase = slot->spriteCount;
    SkinningSprite *sprites = reinterpret_cast<SkinningSprite *>(slot->sprites);
    SkinningSprite *const spriteTail = AppendSkinningTable(sprites, slot->spriteCount, slot->spriteCapacity, model->spriteCount);
    slot->sprites = reinterpret_cast<std::byte *>(sprites);
    if (spriteTail)
        std::memcpy(spriteTail, model->sprites, static_cast<std::size_t>(model->spriteCount) * sizeof(D3TModelSprite));
    const std::uint32_t probeBase = slot->probeCount;
    SkinningProbe *probes = reinterpret_cast<SkinningProbe *>(slot->probes);
    SkinningProbe *const probeTail = AppendSkinningTable(probes, slot->probeCount, slot->probeCapacity, model->probeCount);
    slot->probes = reinterpret_cast<std::byte *>(probes);
    if (probeTail)
        std::memcpy(probeTail, model->probes, static_cast<std::size_t>(model->probeCount) * sizeof(D3TModelProbe));

    for (std::uint32_t node = 0; node < count; ++node) {
        const std::uint32_t index = first + node;
        std::byte *const entry = slot->entries + static_cast<std::size_t>(index) * 20U;
        const D3TModelNode &source = model->nodes[node];
        if (source.sprite)
            Write(entry, 0x0c, static_cast<std::uint16_t>(Read<std::uint16_t>(entry, 0x0c) + spriteBase));
        if (source.probe)
            Write(entry, 0x0e, static_cast<std::uint16_t>(Read<std::uint16_t>(entry, 0x0e) + probeBase));
        if (!source.drawGroup)
            continue;
        const BufferDescription constants{.flags = 0, .size = 160, .count = 0, .flags3 = 2, .buffer = nullptr, .name = ""};
        slot->constantBufferHandles[index] = static_cast<std::uint32_t>(CreateBuffer(&constants));
        const D3TModelDrawGroup &drawGroup = model->drawGroups[source.drawGroup - 1U];
        if (!drawGroup.skin)
            continue;
        const D3TModelSkin &skin = model->skins[drawGroup.skin - 1U];
        SkinningRange *ranges = reinterpret_cast<SkinningRange *>(slot->skinRanges);
        SkinningRange *const range = AppendSkinningTable(ranges, slot->skinRangeCount, slot->skinRangeCapacity, 1U);
        slot->skinRanges = reinterpret_cast<std::uint32_t *>(ranges);
        if (range)
            *range = {.nodeHash = slot->parentHashes[index], .firstBone = slot->boneIndexCount, .boneCount = skin.boneCount};
        std::uint32_t *const bones = AppendSkinningTable(slot->boneIndices, slot->boneIndexCount, slot->boneIndexCapacity, skin.boneCount);
        for (std::uint32_t bone = 0; bones && bone < skin.boneCount; ++bone)
            bones[bone] = firstHash + model->boneNodes[skin.firstBone + bone];
    }
    return (static_cast<std::uint64_t>(firstHash) << 32U) | cacheHandle;
}


const char *GetSkinningNodeName(std::uint64_t handle) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot)
        return "INVALID NODE";
    const std::uint32_t index = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    const std::uint32_t nameOffset = Read<std::uint32_t>(slot->entries + static_cast<std::size_t>(index) * 20U, 0x00);
    if (!slot->renderHandles[index])
        return nameOffset < slot->namesSize ? slot->names + nameOffset : nullptr;
    const D3TModelRecord *const model = d3t::ResolveModel(slot->renderHandles[index]);
    return model && nameOffset < model->stringsSize ? model->strings + nameOffset : nullptr;
}

std::uint32_t GetSkinningNodeChildCount(std::uint64_t handle) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot || !static_cast<std::uint32_t>(handle >> 32U))
        return 0;
    const std::uint32_t index = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    return Read<std::uint16_t>(slot->entries + static_cast<std::size_t>(index) * 20U, 0x08);
}


std::uint64_t GetSkinningNodeChild(std::uint64_t handle, std::uint32_t child) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot)
        return 0;
    const std::uint32_t index = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    if (child >= Read<std::uint16_t>(slot->entries + static_cast<std::size_t>(index) * 20U, 0x08))
        return 0;
    std::uint32_t node = index + 1U;
    for (std::uint32_t skipped = 0; skipped < child; ++skipped)
        node += Read<std::uint16_t>(slot->entries + static_cast<std::size_t>(node) * 20U, 0x06);
    return (static_cast<std::uint64_t>(slot->parentHashes[node]) << 32U) | static_cast<std::uint32_t>(handle);
}

std::uint64_t AllocateSkinningNode(std::uint64_t parentHandle, const char *nodeName, std::uint32_t renderHandle, bool modelOwnedName) {
    const std::uint32_t cacheHandle = static_cast<std::uint32_t>(parentHandle);
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(cacheHandle);
    if (!slot || !nodeName || !slot->entryCount)
        return 0;
    std::uint32_t parentIndex = 0;
    const std::uint32_t parentHash = static_cast<std::uint32_t>(parentHandle >> 32U);
    if (parentHash) {
        parentIndex = GetValueFromMurmurHash(&slot->valueTable, parentHash);
        if (parentIndex >= slot->entryCount || slot->parentHashes[parentIndex] != parentHash) {
            return 0;
        }
    }
    return AppendSkinningNode(*slot, cacheHandle, parentIndex, nodeName, renderHandle, modelOwnedName);
}

bool RemoveSkinningNode(std::uint64_t handle) {
    const std::uint32_t cacheHandle = static_cast<std::uint32_t>(handle);
    const std::uint32_t nodeHash = static_cast<std::uint32_t>(handle >> 32U);
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(cacheHandle);
    if (!slot || !nodeHash || !slot->entryCount)
        return false;
    const std::uint32_t index = GetValueFromMurmurHash(&slot->valueTable, nodeHash);
    if (!index || index >= slot->entryCount || slot->parentHashes[index] != nodeHash) {
        return false;
    }

    const std::uint16_t subtreeSize = Read<std::uint16_t>(slot->entries + static_cast<std::size_t>(index) * 20U, 0x06);
    if (!subtreeSize || subtreeSize > slot->entryCount - index) {
        return false;
    }


    for (std::uint32_t removed = index; removed < index + subtreeSize; ++removed) {
        std::byte *const entry = slot->entries + static_cast<std::size_t>(removed) * 20U;
        if (const std::uint16_t sprite = Read<std::uint16_t>(entry, 0x0c)) {
            for (std::uint32_t node = 0; node < slot->entryCount; ++node) {
                std::byte *const other = slot->entries + static_cast<std::size_t>(node) * 20U;
                if (Read<std::uint16_t>(other, 0x0c) > sprite - 1U)
                    Write(other, 0x0c, static_cast<std::uint16_t>(Read<std::uint16_t>(other, 0x0c) - 1U));
            }
            for (std::uint32_t record = sprite - 1U; record + 1U < slot->spriteCount; ++record)
                std::memmove(slot->sprites + record * sizeof(D3TModelSprite), slot->sprites + (record + 1U) * sizeof(D3TModelSprite), sizeof(D3TModelSprite));
            --slot->spriteCount;
        }
        if (const std::uint16_t probe = Read<std::uint16_t>(entry, 0x0e)) {
            for (std::uint32_t node = 0; node < slot->entryCount; ++node) {
                std::byte *const other = slot->entries + static_cast<std::size_t>(node) * 20U;
                if (Read<std::uint16_t>(other, 0x0e) > probe - 1U)
                    Write(other, 0x0e, static_cast<std::uint16_t>(Read<std::uint16_t>(other, 0x0e) - 1U));
            }
            for (std::uint32_t record = probe - 1U; record + 1U < slot->probeCount; ++record)
                std::memmove(slot->probes + record * sizeof(D3TModelProbe), slot->probes + (record + 1U) * sizeof(D3TModelProbe), sizeof(D3TModelProbe));
            --slot->probeCount;
        }
        if (slot->constantBufferHandles[removed]) {
            ValidateAndCommitBuffer(static_cast<std::int32_t>(slot->constantBufferHandles[removed]));
            slot->constantBufferHandles[removed] = 0;
        }
    }
    if (const std::uint16_t parentDistance = Read<std::uint16_t>(slot->entries + static_cast<std::size_t>(index) * 20U, 0x04)) {
        std::byte *const parent = slot->entries + static_cast<std::size_t>(index - parentDistance) * 20U;
        Write(parent, 0x08, static_cast<std::uint16_t>(Read<std::uint16_t>(parent, 0x08) - 1U));
    }
    UpdateAncestorSubtreeSizes(*slot, index, -static_cast<std::int32_t>(subtreeSize));
    for (std::uint32_t removed = index; removed < index + subtreeSize; ++removed)
        g_modelSkinningNodeNames.erase((static_cast<std::uint64_t>(slot->parentHashes[removed]) << 32U) | cacheHandle);

    const std::uint32_t oldCount = slot->entryCount;
    const std::uint32_t newCount = oldCount - subtreeSize;
    const std::uint32_t movedCount = oldCount - index - subtreeSize;
    if (movedCount) {
        std::memmove(slot->entries + static_cast<std::size_t>(index) * 20U, slot->entries + static_cast<std::size_t>(index + subtreeSize) * 20U,
            static_cast<std::size_t>(movedCount) * 20U);
        std::memmove(slot->matrices + index, slot->matrices + index + subtreeSize, static_cast<std::size_t>(movedCount) * sizeof(*slot->matrices));
        std::memmove(slot->secondaryMatrices + index, slot->secondaryMatrices + index + subtreeSize,
            static_cast<std::size_t>(movedCount) * sizeof(*slot->secondaryMatrices));
        std::memmove(slot->constantBufferHandles + index, slot->constantBufferHandles + index + subtreeSize,
            static_cast<std::size_t>(movedCount) * sizeof(*slot->constantBufferHandles));
        std::memmove(
            slot->renderHandles + index, slot->renderHandles + index + subtreeSize, static_cast<std::size_t>(movedCount) * sizeof(*slot->renderHandles));
        std::memmove(slot->parentHashes + index, slot->parentHashes + index + subtreeSize, static_cast<std::size_t>(movedCount) * sizeof(*slot->parentHashes));
        std::memmove(slot->floatValues + index, slot->floatValues + index + subtreeSize, static_cast<std::size_t>(movedCount) * sizeof(*slot->floatValues));
    }

    for (std::uint32_t moved = index; moved < newCount; ++moved) {
        std::byte *const entry = slot->entries + static_cast<std::size_t>(moved) * 20U;
        const std::uint16_t parentDistance = Read<std::uint16_t>(entry, 0x04);
        const std::uint32_t oldIndex = moved + subtreeSize;
        if (parentDistance && parentDistance <= oldIndex && oldIndex - parentDistance < index) {
            Write(entry, 0x04, static_cast<std::uint16_t>(parentDistance - subtreeSize));
        }
    }
    std::memset(slot->entries + static_cast<std::size_t>(newCount) * 20U, 0, static_cast<std::size_t>(subtreeSize) * 20U);
    std::fill_n(slot->matrices + newCount, subtreeSize, kIdentityMatrix);
    std::fill_n(slot->secondaryMatrices + newCount, subtreeSize, kIdentityMatrix);
    std::fill_n(slot->constantBufferHandles + newCount, subtreeSize, 0U);
    std::fill_n(slot->renderHandles + newCount, subtreeSize, 0U);
    std::fill_n(slot->parentHashes + newCount, subtreeSize, 0U);
    std::fill_n(slot->floatValues + newCount, subtreeSize, 1.0F);
    slot->entryCount = newCount;
    ReindexMurmurTable(*slot);
    return true;
}

void InitializeSkinningWorkCache() {
    for (auto &entry : g_skinningWorkCache.entries) {
        std::memset(entry.data(), 0, entry.size());
        InitializeSkinningWorkEntry(entry.data());
    }
    g_skinningWorkCache.generations.fill(0);
    g_skinningWorkCacheCounter = 0;
}

CachedSkinningBufferSlot *ResolveCachedSkinningBuffer(std::uint32_t handle) {
    const std::uint32_t index = handle & 0x7fU;
    if (g_CachedSkinningBuffers.generations[index] != (handle & 0xffffff80U)) {
        return nullptr;
    }
    return reinterpret_cast<CachedSkinningBufferSlot *>(g_CachedSkinningBuffers.entries[index].data());
}

std::byte *ResolveSkinningWorkCache(std::uint32_t handle) {
    const std::uint32_t index = handle & 0x7fU;
    if (g_skinningWorkCache.generations[index] != (handle & 0xffffff80U)) {
        return nullptr;
    }
    return g_skinningWorkCache.entries[index].data();
}

std::uint64_t AssignNode(std::uint64_t skinningHash, const char *nodeName) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(skinningHash));
    if (!slot || !slot->entries || !slot->renderHandles || !slot->parentHashes || !nodeName || !slot->entryCount) {
        return 0;
    }

    std::uint32_t parentIndex = 0;
    const std::uint32_t parentHash = static_cast<std::uint32_t>(skinningHash >> 32U);
    if (parentHash) {
        parentIndex = GetValueFromMurmurHash(&slot->valueTable, parentHash);
        if (parentIndex >= slot->entryCount || slot->parentHashes[parentIndex] != parentHash) {
            return 0;
        }
    }

    const auto isWithinParent = [&](std::uint32_t index) {
        std::uint32_t guard = slot->entryCount;
        while (index > parentIndex && guard--) {
            const std::uint16_t distance = Read<std::uint16_t>(slot->entries + static_cast<std::size_t>(index) * 20U, 0x04);
            if (!distance || distance > index)
                return false;
            index -= distance;
        }
        return index == parentIndex;
    };
    const auto nodeNameAt = [&](std::uint32_t index) -> const char * {
        const std::uint64_t nodeHandle = (static_cast<std::uint64_t>(slot->parentHashes[index]) << 32U) | static_cast<std::uint32_t>(skinningHash);
        const auto modelName = g_modelSkinningNodeNames.find(nodeHandle);
        if (modelName != g_modelSkinningNodeNames.end())
            return modelName->second.c_str();
        const std::byte *const node = slot->entries + static_cast<std::size_t>(index) * 20U;
        const std::uint32_t nameOffset = Read<std::uint32_t>(node, 0x00);
        if (const std::byte *const renderEntry = d3t::GetRenderOffsetEntry(slot->renderHandles[index])) {
            const auto *const names = Read<const char *>(renderEntry, 0x10);
            const std::uint32_t namesSize = Read<std::uint32_t>(renderEntry, 0x18);
            if (names && nameOffset < namesSize)
                return names + nameOffset;
        }
        if (slot->names && nameOffset < slot->namesSize)
            return slot->names + nameOffset;
        return nullptr;
    };

    for (std::uint32_t index = parentIndex; index < slot->entryCount; ++index) {
        if (!isWithinParent(index))
            continue;
        const char *const candidate = nodeNameAt(index);
        if (candidate && std::strcmp(candidate, nodeName) == 0) {
            return (static_cast<std::uint64_t>(slot->parentHashes[index]) << 32U) | static_cast<std::uint32_t>(skinningHash);
        }
    }
    return 0;
}

bool UpdateSkinningTrackHistory(SkinningTrackState &state, std::int32_t requestedFrame, std::int32_t keyframeFrame, const SkinningKeyframe &keyframe) {
    if (state.frames[1] >= 0 && state.frames[1] <= requestedFrame && state.frames[2] > requestedFrame) {
        return true;
    }

    if (state.frames[3] == -1) {
        for (std::size_t index = 0; index < 4; ++index) {
            state.keyframes[index] = keyframe;
            state.frames[index] = keyframeFrame;
        }
    } else {
        std::memmove(&state.keyframes[0], &state.keyframes[1], 3 * sizeof(SkinningKeyframe));
        std::memmove(&state.frames[0], &state.frames[1], 3 * sizeof(std::int32_t));
        state.keyframes[3] = keyframe;
        state.frames[3] = keyframeFrame;
    }
    return requestedFrame < keyframeFrame;
}

void ClampSkinningTrackHistory(const std::byte *animation, SkinningTrackState &state, std::int32_t requestedFrame) {
    int shifts = 1 + (state.frames[3] <= requestedFrame ? 1 : 0);
    if (state.frames[2] > requestedFrame) {
        shifts = state.frames[3] <= requestedFrame ? 1 : 0;
    }
    while (shifts-- > 0) {
        std::memmove(&state.keyframes[0], &state.keyframes[1], 3 * sizeof(SkinningKeyframe));
        std::memmove(&state.frames[0], &state.frames[1], 3 * sizeof(std::int32_t));
    }

    const std::int32_t firstFrame = Read<std::uint16_t>(animation, 0x46);
    const std::int32_t lastFrame = Read<std::uint16_t>(animation, 0x48);
    for (std::int32_t &frame : state.frames) {
        frame = std::clamp(frame, firstFrame, lastFrame);
    }
}

void InterpolateSkinningTrack(SkinningTrackState &state, float frame) {
    const float first = static_cast<float>(state.frames[1]);
    const float span = static_cast<float>(state.frames[2]) - first;
    const float amount = std::fabs(span) <= 1.1920929e-7F ? 0.0F : std::clamp((frame - first) / span, 0.0F, 1.0F);

    InterpolateComponents(state.translation, state, offsetof(SkinningKeyframe, translation), amount);
    InterpolateComponents(state.rotation, state, offsetof(SkinningKeyframe, rotation), amount);
    const float length = std::sqrt(state.rotation[0] * state.rotation[0] + state.rotation[1] * state.rotation[1] + state.rotation[2] * state.rotation[2] +
        state.rotation[3] * state.rotation[3]);
    if (length >= 1.1920929e-7F) {
        const float reciprocal = 1.0F / length;
        for (float &component : state.rotation)
            component *= reciprocal;
    }
    InterpolateComponents(state.scale, state, offsetof(SkinningKeyframe, scale), amount);
}

std::int32_t TexSort2(void *data) {
    if (!data)
        return 0;
    const auto *const task = static_cast<const std::byte *>(data);
    const auto *const renderHandle = Read<const std::uint32_t *>(task, 0x00);
    auto *const workCache = Read<std::byte *>(task, 0x08);
    const auto *const animation = Read<const std::byte *>(task, 0x10);
    auto *const trackState = Read<std::byte *>(task, 0x18);
    const float timeScale = Read<float>(task, 0x20);
    if (!renderHandle || !workCache || !animation || !trackState)
        return 0;

    const auto *const renderPayload = static_cast<const std::byte *>(d3t::GetPointerFromOffset(*renderHandle));
    if (!renderPayload)
        return 0;
    const auto *const trackTable = Read<const std::byte *>(renderPayload, 0x00);
    if (!trackTable)
        return 0;

    const std::uint16_t animationTrack = Read<std::uint16_t>(animation, 0x44);
    const std::byte *const streamDescriptor = trackTable + static_cast<std::size_t>(animationTrack) * 0x70;
    const std::uint16_t firstFrame = Read<std::uint16_t>(animation, 0x46);
    const std::uint16_t lastFrame = Read<std::uint16_t>(animation, 0x48);
    float secondsPerCycle = static_cast<float>(Read<std::uint8_t>(animation, 0xca));
    const float workFrameRate = Read<float>(workCache, 0x24f18);
    if (workFrameRate > 0.0F) {
        secondsPerCycle = static_cast<float>(lastFrame - firstFrame) / workFrameRate;
    }

    const float scaledTime = timeScale * secondsPerCycle;
    const float frameSpan = static_cast<float>(lastFrame - firstFrame);
    const std::uint8_t workFlags = Read<std::uint8_t>(workCache, 0x24ef8);
    const std::uint8_t animationFlags = Read<std::uint8_t>(animation, 0xcb);
    const float relativeFrame =
        ((workFlags & 1U) != 0 || ((animationFlags & 1U) != 0 && (workFlags & 2U) == 0)) ? std::fmod(scaledTime, frameSpan) : std::min(frameSpan, scaledTime);
    const std::int32_t requestedFrame = static_cast<std::uint16_t>(firstFrame + static_cast<std::int32_t>(relativeFrame));
    const std::int32_t previousFrame = Read<std::int32_t>(trackState, 0x04);

    const std::uint32_t streamSize = Read<std::uint32_t>(streamDescriptor, 0x50);
    const std::byte *const stream = Read<const std::byte *>(streamDescriptor, 0x60);
    if (requestedFrame != previousFrame && stream) {
        std::uint64_t streamOffset = requestedFrame > previousFrame ? Read<std::uint64_t>(trackState, 0x08) : 0;
        const std::byte *cursor = stream + streamOffset;
        bool allTracksReady = false;
        while (streamOffset < streamSize && !allTracksReady) {
            const std::int32_t keyframeFrame = Read<std::int32_t>(cursor, 0x00);
            const std::uint32_t keyframeCount = Read<std::uint32_t>(cursor, 0x04);
            cursor += 8;
            streamOffset += 8;
            allTracksReady = true;
            for (std::uint32_t item = 0; item < keyframeCount; ++item) {
                const std::uint32_t trackIndex = Read<std::uint32_t>(cursor, 0x00);
                const std::byte *const keyframeBytes = cursor + 4;
                SkinningKeyframe keyframe{};
                std::memcpy(&keyframe, keyframeBytes, sizeof(keyframe));
                auto *const state =
                    reinterpret_cast<SkinningTrackState *>(trackState + 0x10 + static_cast<std::size_t>(trackIndex) * sizeof(SkinningTrackState));
                allTracksReady &= UpdateSkinningTrackHistory(*state, requestedFrame, keyframeFrame, keyframe);

                const auto *const componentFlags = Read<const std::uint8_t *>(streamDescriptor, 0x48);
                const std::uint8_t flags = componentFlags ? componentFlags[trackIndex * 4 + 2] : 0;
                std::size_t payloadSize = 0;
                if ((flags & 1U) != 0)
                    payloadSize += 12;
                if ((flags & 2U) != 0)
                    payloadSize += 12;
                if ((flags & 4U) != 0)
                    payloadSize += 16;
                cursor += 4 + payloadSize;
                streamOffset += 4 + payloadSize;
            }
        }

        std::memcpy(trackState + 0x08, &streamOffset, sizeof(streamOffset));
        std::memcpy(trackState + 0x04, &requestedFrame, sizeof(requestedFrame));
        const std::uint16_t trackCount = Read<std::uint16_t>(streamDescriptor, 0x40);
        for (std::uint32_t track = 0; track < trackCount; ++track) {
            if (Read<std::uint32_t>(workCache, 0x24efc) != 0) {
                const std::uint32_t word = Read<std::uint32_t>(workCache, 0x24f20 + (track >> 5U) * 4U);
                if ((word & (1U << (track & 31U))) == 0)
                    continue;
            }
            auto *const state = reinterpret_cast<SkinningTrackState *>(trackState + 0x10 + static_cast<std::size_t>(track) * sizeof(SkinningTrackState));
            ClampSkinningTrackHistory(animation, *state, requestedFrame);
        }
    }

    const std::uint16_t trackCount = Read<std::uint16_t>(streamDescriptor, 0x40);
    for (std::uint32_t track = 0; track < trackCount; ++track) {
        if (Read<std::uint32_t>(workCache, 0x24efc) != 0) {
            const std::uint32_t word = Read<std::uint32_t>(workCache, 0x24f20 + (track >> 5U) * 4U);
            if ((word & (1U << (track & 31U))) == 0)
                continue;
        }
        auto *const state = reinterpret_cast<SkinningTrackState *>(trackState + 0x10 + static_cast<std::size_t>(track) * sizeof(SkinningTrackState));
        InterpolateSkinningTrack(*state, static_cast<float>(firstFrame) + relativeFrame);
    }
    return 0;
}

void DoTexSort2(std::uint32_t *renderRecord, std::byte *workCache, float frameTime, std::uint32_t **taskCursor) {
    if (!renderRecord || !workCache || !taskCursor || !d3t::GetPointerFromOffset(*renderRecord)) {
        return;
    }

    const auto *const timing = Read<const std::byte *>(workCache, 0x00);
    if (!timing)
        return;
    float elapsed = Read<float>(workCache, 0x1c) + frameTime;
    std::memcpy(workCache + 0x1c, &elapsed, sizeof(elapsed));
    float weight = 1.0F;
    float introDuration = Read<float>(timing, 0xcc);
    const float introOverride = Read<float>(workCache, 0x24f10);
    if (introOverride > 0.0F)
        introDuration = introOverride;
    if (introDuration > 0.0F && elapsed < introDuration) {
        weight = SmoothStep(std::clamp(elapsed / introDuration, 0.0F, 1.0F));
    }

    std::int32_t mode = Read<std::int32_t>(workCache, 0x24);
    if (mode == 2) {
        float fadeElapsed = Read<float>(workCache, 0x20) + frameTime;
        float fadeDuration = Read<float>(timing, 0xd0);
        const float fadeOverride = Read<float>(workCache, 0x24f14);
        if (fadeOverride > 0.0F)
            fadeDuration = fadeOverride;
        if (fadeElapsed >= fadeDuration) {
            fadeElapsed = fadeDuration;
            mode = 0;
        }
        if (fadeDuration <= 0.0F) {
            weight = 0.0F;
        } else {
            const float amount = std::clamp(1.0F - fadeElapsed / fadeDuration, 0.0F, 1.0F);
            weight *= SmoothStep(amount);
        }
        if (weight < 1.1920929e-7F) {
            fadeElapsed = fadeDuration;
            mode = 0;
        }
        std::memcpy(workCache + 0x20, &fadeElapsed, sizeof(fadeElapsed));
        std::memcpy(workCache + 0x24, &mode, sizeof(mode));
    }
    std::memcpy(workCache + 0x30, &weight, sizeof(weight));
    if (mode == 0)
        return;

    const auto queueTrack = [&](const std::byte *animation, std::byte *state) {
        struct TaskData {
            std::uint32_t *renderRecord;
            std::byte *workCache;
            const std::byte *animation;
            std::byte *state;
            float time;
            std::uint32_t reserved;
        } taskData{renderRecord, workCache, animation, state, elapsed, 0};
        static_assert(sizeof(TaskData) == 0x28);
        if (*taskCursor) {
            const D3TTaskDescriptor descriptor{&taskData, TexSort2, nullptr, 0};
            *(*taskCursor)++ = d3t_QueueTask(&descriptor, sizeof(taskData));
        } else {
            TexSort2(&taskData);
        }
    };

    const auto *primaryAnimation = Read<const std::byte *>(workCache, 0x08);
    const auto *secondaryAnimation = Read<const std::byte *>(workCache, 0x10);
    const std::int32_t renderType = Read<std::int32_t>(timing, 0x00);
    if (renderType != 1) {
        queueTrack(timing, workCache + 0x30);
        return;
    }

    const float transition = std::clamp(elapsed * 5.0F, 0.0F, 1.0F);
    const float priorBlend = Read<float>(workCache, 0x18);
    const float targetBlend = Read<float>(workCache, 0x24f00);
    float blend = transition * targetBlend + (1.0F - transition) * priorBlend;
    std::memcpy(workCache + 0x18, &blend, sizeof(blend));
    using BlendCallback = float (*)(std::byte *);
    const auto callback = Read<BlendCallback>(workCache, 0x24f08);
    if (callback)
        blend = callback(workCache);

    if (blend <= 1.1920929e-7F) {
        queueTrack(primaryAnimation, workCache + 0x30);
    } else if (blend < 0.99999988F) {
        queueTrack(primaryAnimation, workCache + 0x30);
        queueTrack(secondaryAnimation, workCache + 0x12790);
    } else {
        queueTrack(secondaryAnimation, workCache + 0x12790);
    }
}

void ProcessSkinningCacheTexSort(std::uint32_t handle, void *data, float frameTime) {
    auto *const batch = static_cast<SkinningTexSortBatch *>(data);
    if (!batch || !batch->records || batch->recordCount == 0)
        return;

    LockSkinningReader();
    const std::uint32_t taskCapacity = batch->recordCount * 64U;
    auto *const taskHandles = static_cast<std::uint32_t *>(AllocateMemoryFromGlobalPool(static_cast<std::size_t>(taskCapacity) * sizeof(std::uint32_t)));
    if (!taskHandles) {
        UnlockSkinningReader();
        return;
    }
    std::memset(taskHandles, 0, static_cast<std::size_t>(taskCapacity) * sizeof(std::uint32_t));
    std::uint32_t *taskCursor = taskHandles;
    for (std::uint32_t recordIndex = 0; recordIndex < batch->recordCount; ++recordIndex) {
        auto *const record = reinterpret_cast<SkinningTexSortRecord *>(batch->records + static_cast<std::size_t>(recordIndex) * sizeof(SkinningTexSortRecord));
        for (std::uint32_t cacheIndex = 0; cacheIndex < record->cacheHandleCount; ++cacheIndex) {
            if (std::byte *const workCache = ResolveSkinningWorkCache(record->cacheHandles[cacheIndex])) {
                DoTexSort2(reinterpret_cast<std::uint32_t *>(record), workCache, frameTime, &taskCursor);
            }
        }
    }

    const auto waitForTasks = [&](std::uint32_t count) {
        while (true) {
            bool active = false;
            for (std::uint32_t index = 0; index < count; ++index) {
                if (d3t_IsTaskActive(taskHandles[index])) {
                    active = true;
                    break;
                }
            }
            if (!active)
                break;
            if (!RunTask(0, taskHandles, count)) {
                std::this_thread::yield();
            }
        }
    };

    waitForTasks(static_cast<std::uint32_t>(taskCursor - taskHandles));
    std::fill_n(taskHandles, batch->recordCount, 0U);
    taskCursor = taskHandles;

    struct TexSortTaskData {
        std::uint32_t skinningHandle;
        std::uint32_t reserved;
        SkinningTexSortRecord *record;
        float frameTime;
        std::uint32_t reserved_14;
    };
    static_assert(sizeof(TexSortTaskData) == 0x18);
    for (std::uint32_t recordIndex = 0; recordIndex < batch->recordCount; ++recordIndex) {
        auto *const record = reinterpret_cast<SkinningTexSortRecord *>(batch->records + static_cast<std::size_t>(recordIndex) * sizeof(SkinningTexSortRecord));
        const TexSortTaskData taskData{handle, 0, record, frameTime, 0};
        const D3TTaskDescriptor descriptor{&taskData, TexSort, nullptr, 0};
        *taskCursor++ = d3t_QueueTask(&descriptor, sizeof(taskData));
    }
    waitForTasks(batch->recordCount);
    RestoreGlobalPoolCheckpoint(taskHandles);
    UnlockSkinningReader();
}

std::int32_t TexSort(void *data) {
    if (!data)
        return 0;
    const auto *const task = static_cast<const std::byte *>(data);
    const std::uint32_t skinningHandle = Read<std::uint32_t>(task, 0x00);
    auto *const record = Read<SkinningTexSortRecord *>(task, 0x08);
    if (!record || record->cacheHandleCount == 0)
        return 0;

    BuildSkinningMatrices(skinningHandle, record);
    std::uint32_t index = 0;
    while (index < record->cacheHandleCount) {
        const std::uint32_t cacheHandle = record->cacheHandles[index];
        std::byte *const workCache = ResolveSkinningWorkCache(cacheHandle);
        if (workCache && Read<const void *>(workCache, 0x48)) {
            ++index;
            continue;
        }
        if (workCache) {
            const std::uint32_t slot = cacheHandle & 0x7fU;
            if (g_skinningWorkCache.generations[slot] == (cacheHandle & 0xffffff80U)) {
                g_skinningWorkCache.generations[slot] &= 0x7fffffffU;
            }
        }
        const std::uint32_t remaining = record->cacheHandleCount - index - 1U;
        if (remaining != 0) {
            std::memmove(&record->cacheHandles[index], &record->cacheHandles[index + 1], remaining * sizeof(std::uint32_t));
        }
        --record->cacheHandleCount;
    }
    return 0;
}

void BuildSkinningMatrices(std::uint32_t skinningHandle, SkinningTexSortRecord *record) {
    if (!record || !record->boneTables)
        return;
    const auto *const renderPayload = static_cast<const std::byte *>(d3t::GetPointerFromOffset(record->renderHandle));
    if (!renderPayload)
        return;
    const auto *const renderTracks = Read<const std::byte *>(renderPayload, 0x00);
    if (!renderTracks)
        return;

    struct BoneAccumulator {
        std::uint16_t boneId;
        std::uint16_t contributionCount;
        float totalWeight;
        Matrix4x4 matrix;
    };
    static_assert(sizeof(BoneAccumulator) == 0x48);
    std::array<BoneAccumulator, 350> accumulators{};

    const auto findAccumulator = [&](std::uint16_t boneId) -> BoneAccumulator * {
        for (BoneAccumulator &accumulator : accumulators) {
            if (accumulator.boneId == boneId)
                return &accumulator;
        }
        for (BoneAccumulator &accumulator : accumulators) {
            if (accumulator.boneId == 0) {
                accumulator.boneId = boneId;
                return &accumulator;
            }
        }
        return nullptr;
    };

    for (std::uint32_t cacheIndex = 0; cacheIndex < record->cacheHandleCount; ++cacheIndex) {
        std::byte *const workCache = ResolveSkinningWorkCache(record->cacheHandles[cacheIndex]);
        if (!workCache || !Read<const void *>(workCache, 0x00))
            continue;
        const float fadeWeight = Read<float>(workCache, 0x30);
        if (fadeWeight <= 0.0F)
            continue;

        const auto *const timing = Read<const std::byte *>(workCache, 0x00);
        const std::uint16_t trackSet = Read<std::uint16_t>(timing, 0x44);
        const std::byte *const trackDescriptor = renderTracks + static_cast<std::size_t>(trackSet) * 0x70;
        const std::uint16_t trackCount = Read<std::uint16_t>(trackDescriptor, 0x40);
        const auto *const boneIds = reinterpret_cast<const std::int16_t *>(record->boneTables + static_cast<std::size_t>(trackSet) * 0x2d0);

        float blend = Read<float>(workCache, 0x18);
        using BlendCallback = float (*)(std::byte *);
        const auto callback = Read<BlendCallback>(workCache, 0x24f08);
        if (callback)
            blend = callback(workCache);
        if (blend > 1.1920929e-7F && blend < 0.99999988F) {
            for (std::uint32_t track = 0; track < trackCount; ++track) {
                if (!IsTrackEnabled(workCache, track))
                    continue;
                auto *const primary = reinterpret_cast<float *>(workCache + 0xf0 + static_cast<std::size_t>(track) * sizeof(SkinningTrackState));
                const auto *const secondary =
                    reinterpret_cast<const float *>(workCache + 0x12850 + static_cast<std::size_t>(track) * sizeof(SkinningTrackState));
                for (std::size_t component = 0; component < 10; ++component) {
                    primary[component] += (secondary[component] - primary[component]) * blend;
                }
            }
        }

        for (std::uint32_t track = 0; track < trackCount; ++track) {
            if (!IsTrackEnabled(workCache, track))
                continue;
            BoneAccumulator *const accumulator = findAccumulator(static_cast<std::uint16_t>(boneIds[track]));
            if (!accumulator)
                continue;
            ++accumulator->contributionCount;
            accumulator->totalWeight += fadeWeight;
        }
    }

    for (std::uint32_t cacheIndex = 0; cacheIndex < record->cacheHandleCount; ++cacheIndex) {
        std::byte *const workCache = ResolveSkinningWorkCache(record->cacheHandles[cacheIndex]);
        if (!workCache || !Read<const void *>(workCache, 0x00))
            continue;
        const float fadeWeight = Read<float>(workCache, 0x30);
        if (fadeWeight <= 0.0F)
            continue;

        const auto *const timing = Read<const std::byte *>(workCache, 0x00);
        const std::uint16_t trackSet = Read<std::uint16_t>(timing, 0x44);
        const std::byte *const trackDescriptor = renderTracks + static_cast<std::size_t>(trackSet) * 0x70;
        const std::uint16_t trackCount = Read<std::uint16_t>(trackDescriptor, 0x40);
        const auto *const boneIds = reinterpret_cast<const std::int16_t *>(record->boneTables + static_cast<std::size_t>(trackSet) * 0x2d0);

        for (std::uint32_t track = 0; track < trackCount; ++track) {
            if (!IsTrackEnabled(workCache, track))
                continue;
            BoneAccumulator *selected = nullptr;
            for (BoneAccumulator &candidate : accumulators) {
                if (candidate.boneId == static_cast<std::uint16_t>(boneIds[track])) {
                    selected = &candidate;
                    break;
                }
            }
            if (!selected || selected->totalWeight <= 0.0F)
                continue;

            const auto *const transform = reinterpret_cast<const float *>(workCache + 0xf0 + static_cast<std::size_t>(track) * sizeof(SkinningTrackState));
            Matrix4x4 scale{{
                {transform[7], 0.0F, 0.0F, 0.0F},
                {0.0F, transform[8], 0.0F, 0.0F},
                {0.0F, 0.0F, transform[9], 0.0F},
                {0.0F, 0.0F, 0.0F, 1.0F},
            }};
            const Matrix4x4 rotation = QuaternionMatrix(transform + 3);
            Matrix4x4 matrix = MultiplyMatrices(rotation, scale);
            matrix.rows[0].w = transform[0];
            matrix.rows[1].w = transform[1];
            matrix.rows[2].w = transform[2];
            AddScaledMatrix(selected->matrix, matrix, fadeWeight / selected->totalWeight);
        }
    }

    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(skinningHandle);
    if (!slot || !slot->matrices)
        return;
    for (const BoneAccumulator &accumulator : accumulators) {
        if (accumulator.boneId == 0 || accumulator.contributionCount == 0) {
            continue;
        }
        const std::uint32_t key = record->baseBoneHash + accumulator.boneId;
        const std::uint32_t matrixIndex = GetValueFromMurmurHash(&slot->valueTable, key);
        Matrix4x4 output = slot->matrices[matrixIndex];
        auto *const destination = reinterpret_cast<float *>(&output);
        const auto *const source = reinterpret_cast<const float *>(&accumulator.matrix);
        for (std::size_t component = 0; component < 16; ++component) {
            destination[component] += (source[component] - destination[component]) * 0.75F;
        }
        slot->matrices[matrixIndex] = output;
    }
}

void ReleaseSkinningTexSortRecord(SkinningTexSortRecord *record) {
    if (!record)
        return;
    auto *const boneTables = record->boneTables;
    for (std::uint32_t index = 0; index < record->boneTableCount; ++index) {
        std::byte *const table = boneTables + static_cast<std::size_t>(index) * 0x2d0;
        std::free(Read<void *>(table, 0x2c0));
        std::memset(table + 0x2c0, 0, 0x10);
    }
    std::free(record->boneTables);
    record->boneTables = nullptr;
    record->boneTableCount = 0;

    for (std::uint32_t index = 0; index < record->cacheHandleCount; ++index) {
        const std::uint32_t handle = record->cacheHandles[index];
        const std::uint32_t slot = handle & 0x7fU;
        if (g_skinningWorkCache.generations[slot] == (handle & 0xffffff80U)) {
            g_skinningWorkCache.generations[slot] &= 0x7fffffffU;
        }
    }
}

SkinningTexSortRecord *AppendSkinningTexSortRecord(SkinningTexSortBatch *batch) {
    if (!batch)
        return nullptr;
    if (batch->recordCount >= batch->capacity) {
        const std::uint32_t doubled = batch->capacity * 2U;
        const std::uint32_t newCapacity = std::max(16U, doubled);
        if (newCapacity <= batch->capacity) {
            return reinterpret_cast<SkinningTexSortRecord *>(batch->records + static_cast<std::size_t>(batch->recordCount++) * sizeof(SkinningTexSortRecord));
        }
        const std::size_t size = static_cast<std::size_t>(newCapacity) * sizeof(SkinningTexSortRecord);
        void *const allocation = std::realloc(batch->records, size);
        if (!allocation)
            return nullptr;
        batch->records = static_cast<std::byte *>(allocation);
        batch->capacity = newCapacity;
    }
    return reinterpret_cast<SkinningTexSortRecord *>(batch->records + static_cast<std::size_t>(batch->recordCount++) * sizeof(SkinningTexSortRecord));
}

SkinningTexSortRecord *BuildSkinningTexSortRecord(std::uint32_t renderHandle, SkinningTexSortBatch *batch, std::uint64_t skinningHash) {
    if (!batch)
        return nullptr;
    const auto *const renderPayload = static_cast<const std::byte *>(d3t::GetPointerFromOffset(renderHandle));
    if (!renderPayload)
        return nullptr;
    const auto *const trackDescriptors = Read<const std::byte *>(renderPayload, 0x00);
    const std::uint32_t trackDescriptorCount = Read<std::uint32_t>(renderPayload, 0x08);
    if (!trackDescriptors || trackDescriptorCount == 0)
        return nullptr;
    for (std::uint32_t index = 0; index < trackDescriptorCount; ++index) {
        if (Read<std::uint16_t>(trackDescriptors + static_cast<std::size_t>(index) * 0x70, 0x40) > 350) {
            return nullptr;
        }
    }

    LockSkinningWriter();
    if (batch->capacity == 0) {
        void *const allocation = std::realloc(batch->records, 4U * sizeof(SkinningTexSortRecord));
        if (allocation) {
            batch->records = static_cast<std::byte *>(allocation);
            batch->capacity = 4;
        }
    }
    SkinningTexSortRecord *const record = AppendSkinningTexSortRecord(batch);
    UnlockSkinningWriter();
    if (!record)
        return nullptr;

    std::memset(record, 0, sizeof(*record));
    record->renderHandle = renderHandle;
    record->baseBoneHash = static_cast<std::uint32_t>(skinningHash >> 32U);

    const std::uint32_t mappingGroupCount = Read<std::uint32_t>(renderPayload, 0x28);
    const auto *const mappingGroups = Read<const std::byte *>(renderPayload, 0x20);
    void *const scratchCheckpoint = GetGlobalPoolCheckpoint();
    auto **const resolvedBoneIds = static_cast<std::uint16_t **>(AllocateMemoryFromGlobalPool(static_cast<std::size_t>(mappingGroupCount) * sizeof(void *)));
    if (mappingGroupCount != 0 && (!mappingGroups || !resolvedBoneIds)) {
        RestoreGlobalPoolCheckpoint(scratchCheckpoint);
        return record;
    }

    for (std::uint32_t group = 0; group < mappingGroupCount; ++group) {
        const std::byte *const mapping = mappingGroups + static_cast<std::size_t>(group) * 0x50;
        const std::uint32_t nodeCount = Read<std::uint32_t>(mapping, 0x48);
        auto *const ids = static_cast<std::uint16_t *>(AllocateMemoryFromGlobalPool(static_cast<std::size_t>(nodeCount) * sizeof(std::uint16_t)));
        resolvedBoneIds[group] = ids;
        const auto *const nodeNames = Read<const std::byte *>(mapping, 0x40);
        for (std::uint32_t node = 0; node < nodeCount; ++node) {
            const std::uint64_t assigned = AssignNode(skinningHash, reinterpret_cast<const char *>(nodeNames + static_cast<std::size_t>(node) * 0x40));
            if (assigned != 0 && (assigned >> 32U) != 0) {
                ids[node] = static_cast<std::uint16_t>(static_cast<std::uint32_t>(assigned >> 32U) - static_cast<std::uint16_t>(record->baseBoneHash));
            }
        }
    }

    if (trackDescriptorCount > record->boneTableCapacity) {
        void *const allocation = std::realloc(record->boneTables, static_cast<std::size_t>(trackDescriptorCount) * 0x2d0);
        if (allocation) {
            record->boneTables = static_cast<std::byte *>(allocation);
            record->boneTableCapacity = trackDescriptorCount;
        }
    }

    for (std::uint32_t descriptorIndex = 0; descriptorIndex < trackDescriptorCount; ++descriptorIndex) {
        if (record->boneTableCount >= record->boneTableCapacity)
            break;
        std::byte *const table = record->boneTables + static_cast<std::size_t>(record->boneTableCount++) * 0x2d0;
        std::memset(table, 0, 0x2d0);
        const std::byte *const descriptor = trackDescriptors + static_cast<std::size_t>(descriptorIndex) * 0x70;
        const std::uint16_t boneCount = Read<std::uint16_t>(descriptor, 0x40);
        const auto *const sourceBoneIds = Read<const std::uint16_t *>(descriptor, 0x48);
        for (std::uint32_t bone = 0; bone < boneCount; ++bone) {
            std::memcpy(table + bone * sizeof(std::uint16_t), &sourceBoneIds[bone], sizeof(std::uint16_t));
        }

        void *masks = Read<void *>(table, 0x2c0);
        std::uint32_t maskCapacity = Read<std::uint32_t>(table, 0x2cc);
        if (mappingGroupCount > maskCapacity) {
            void *const allocation = std::realloc(masks, static_cast<std::size_t>(mappingGroupCount) * 0x2c);
            if (allocation) {
                masks = allocation;
                maskCapacity = mappingGroupCount;
                std::memcpy(table + 0x2c0, &masks, sizeof(masks));
                std::memcpy(table + 0x2cc, &maskCapacity, sizeof(maskCapacity));
            }
        }
        std::uint32_t maskCount = 0;
        for (std::uint32_t group = 0; group < mappingGroupCount; ++group) {
            if (maskCount >= maskCapacity) {
                const std::uint32_t newCapacity = std::max(16U, maskCapacity * 2U);
                if (newCapacity > maskCapacity) {
                    void *const allocation = std::realloc(masks, static_cast<std::size_t>(newCapacity) * 0x2c);
                    if (allocation) {
                        masks = allocation;
                        maskCapacity = newCapacity;
                        std::memcpy(table + 0x2c0, &masks, sizeof(masks));
                        std::memcpy(table + 0x2cc, &maskCapacity, sizeof(maskCapacity));
                    }
                }
            }
            if (maskCount >= maskCapacity)
                continue;
            auto *const mask = static_cast<std::byte *>(masks) + static_cast<std::size_t>(maskCount++) * 0x2c;
            std::memset(mask, 0, 0x2c);
            const std::byte *const mapping = mappingGroups + static_cast<std::size_t>(group) * 0x50;
            const std::uint32_t nodeCount = Read<std::uint32_t>(mapping, 0x48);
            for (std::uint32_t bone = 0; bone < boneCount; ++bone) {
                const std::uint16_t tableBone = Read<std::uint16_t>(table, bone * sizeof(std::uint16_t));
                for (std::uint32_t node = 0; node < nodeCount; ++node) {
                    if (tableBone != resolvedBoneIds[group][node])
                        continue;
                    std::uint32_t word = Read<std::uint32_t>(mask, (bone >> 5U) * sizeof(std::uint32_t));
                    word |= 1U << (bone & 31U);
                    std::memcpy(mask + (bone >> 5U) * sizeof(std::uint32_t), &word, sizeof(word));
                    break;
                }
            }
        }
        std::memcpy(table + 0x2c8, &maskCount, sizeof(maskCount));
    }

    RestoreGlobalPoolCheckpoint(scratchCheckpoint);
    return record;
}

SkinningTexSortRecord *LookupOrBuildSkinningTexSortRecord(std::uint64_t skinningHash) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(skinningHash));
    if (!slot)
        return nullptr;
    auto *const slotBytes = reinterpret_cast<std::byte *>(slot);
    auto *const batch = reinterpret_cast<SkinningTexSortBatch *>(slotBytes + 0xb8);

    LockSkinningReader();
    const std::uint32_t key = static_cast<std::uint32_t>(skinningHash >> 32U);
    for (std::uint32_t index = 0; index < batch->recordCount; ++index) {
        auto *const record = reinterpret_cast<SkinningTexSortRecord *>(batch->records + static_cast<std::size_t>(index) * sizeof(SkinningTexSortRecord));
        if (record->baseBoneHash == key) {
            UnlockSkinningReader();
            return record;
        }
    }
    UnlockSkinningReader();

    const std::uint32_t *const renderHandles = slot->renderHandles;
    if (!renderHandles)
        return nullptr;
    const std::uint32_t valueIndex = GetValueFromMurmurHash(&slot->valueTable, key);
    const std::uint32_t renderHandle = renderHandles[valueIndex];
    if (!d3t::GetRenderOffsetEntry(renderHandle))
        return nullptr;
    return BuildSkinningTexSortRecord(renderHandle, batch, skinningHash);
}



std::uint32_t FindFacialTargetMask(std::uint64_t skinningHash, const char *name) {
    SkinningTexSortRecord *const record = LookupOrBuildSkinningTexSortRecord(skinningHash);
    if (!record)
        return 0;
    const auto *const renderPayload = static_cast<const std::byte *>(d3t::GetPointerFromOffset(record->renderHandle));
    if (!renderPayload || !name || *name == '\0')
        return 0;
    const std::uint32_t count = Read<std::uint32_t>(renderPayload, 0x28);
    if (count > 32)
        return 0;
    const std::byte *const groups = Read<const std::byte *>(renderPayload, 0x20);
    for (std::uint32_t index = 0; index < count; ++index) {
        if (std::strcmp(reinterpret_cast<const char *>(groups + static_cast<std::size_t>(index) * 0x50), name) == 0)
            return 1U << index;
    }
    return 0;
}

bool StartFacialExpressionWork(std::uint64_t skinningHash, const FacialExpressionRequest *request) {
    if (!request)
        return false;
    SkinningTexSortRecord *const record = LookupOrBuildSkinningTexSortRecord(skinningHash);
    if (!record || record->cacheHandleCount >= 32)
        return false;
    const auto *const renderPayload = static_cast<const std::byte *>(d3t::GetPointerFromOffset(record->renderHandle));
    if (!renderPayload)
        return false;
    const std::byte *const metadata = FindExpressionMetadata(renderPayload, request->name);
    if (!metadata)
        return false;

    if ((request->flags & 4U) == 0) {
        for (std::uint32_t index = 0; index < record->cacheHandleCount; ++index) {
            std::byte *const work = ResolveSkinningWorkCache(record->cacheHandles[index]);
            if (!work || Read<const std::byte *>(work, 0x00) != metadata) {
                continue;
            }
            if ((Read<std::uint32_t>(work, 0x24efc) & request->targetMask) != request->targetMask || Read<std::uint32_t>(work, 0x24) != 1) {
                continue;
            }
            if (Read<std::uint32_t>(metadata, 0x00) == 1) {
                const std::uint32_t zero = 0;
                Write(work, 0x1c, zero);
                Write(work, 0x24f00, request->blend);
            }
            return false;
        }
    }

    const std::byte *secondaryA = nullptr;
    const std::byte *secondaryB = nullptr;
    if (Read<std::uint32_t>(metadata, 0x00) == 1) {
        secondaryA = FindExpressionMetadata(renderPayload, reinterpret_cast<const char *>(metadata + 0x4a));
        secondaryB = FindExpressionMetadata(renderPayload, reinterpret_cast<const char *>(metadata + 0x8a));
        if (!secondaryA || !secondaryB)
            return false;
    }

    const std::uint32_t newHandle = AllocateSkinningWorkHandle();
    if (newHandle == 0)
        return false;
    if ((request->flags & 8U) == 0) {
        const std::uint32_t originalCount = record->cacheHandleCount;
        for (std::uint32_t index = 0; index < originalCount; ++index) {
            std::byte *const existing = ResolveSkinningWorkCache(record->cacheHandles[index]);
            if (!existing)
                continue;
            const std::uint32_t oldMask = Read<std::uint32_t>(existing, 0x24efc);
            const std::uint32_t overlap = oldMask & request->targetMask;
            if (overlap == 0 || Read<std::uint32_t>(existing, 0x24) == 2) {
                continue;
            }
            const std::uint32_t leftover = oldMask & ~request->targetMask;
            if (leftover != 0) {
                Write(existing, 0x24efc, overlap);
                SetSkinningTargetMask(record, existing);
                FacialExpressionRequest split{};
                std::memcpy(&split, existing + 0x24ef0, sizeof(split));
                split.targetMask = leftover;
                split.fadeSeconds = 0.0F;
                if (StartFacialExpressionWork(skinningHash, &split) && record->cacheHandleCount != 0) {
                    std::byte *const created = ResolveSkinningWorkCache(record->cacheHandles[record->cacheHandleCount - 1]);
                    if (created) {
                        std::memcpy(created + 0x18, existing + 0x18, 0x0c);
                    }
                }
            }
            const std::byte *const oldMetadata = Read<const std::byte *>(existing, 0x00);
            float oldFade = oldMetadata ? Read<float>(oldMetadata, 0xd0) : 0.0F;
            const float oldOverride = Read<float>(existing, 0x24f14);
            if (oldOverride > 0.0F)
                oldFade = oldOverride;
            float newFade = Read<float>(metadata, 0xcc);
            if (request->fadeSeconds > 0.0F) {
                newFade = request->fadeSeconds;
            }
            const std::uint32_t fading = 2;
            Write(existing, 0x24, fading);
            Write(existing, 0x24f14, std::max(newFade, oldFade));
        }
    }

    std::byte *const work = ResolveSkinningWorkCache(newHandle);
    if (work) {
        record->cacheHandles[record->cacheHandleCount++] = newHandle;
        std::memset(work, 0, kSkinningWorkCacheEntrySize);
        Write(work, 0x00, metadata);
        Write(work, 0x08, secondaryA);
        Write(work, 0x10, secondaryB);
        const std::uint32_t active = 1;
        Write(work, 0x24, active);
        std::memcpy(work + 0x24ef0, request, sizeof(*request));
        const float blend = std::clamp(Read<float>(work, 0x24f00), 0.0F, 1.0F);
        Write(work, 0x24f00, blend);
        Write(work, 0x18, blend);
        SetSkinningTargetMask(record, work);
        InitializeSkinningWorkEntry(work);
    }
    return true;
}

std::uint32_t GetValueFromMurmurHash(const void *table, std::uint32_t key) {
    if (!table)
        return 0;
    const auto *const bytes = static_cast<const std::byte *>(table);
    const std::uint32_t capacity = Read<std::uint32_t>(bytes, 0x18);
    if (capacity == 0)
        return 0;

    const auto *const values = Read<const std::uint32_t *>(bytes, 0x00);
    const auto *const hashes = Read<const std::uint32_t *>(bytes, 0x08);
    const auto *const probes = Read<const std::uint16_t *>(bytes, 0x10);
    if (!values || !hashes || !probes)
        return 0;

    const std::uint32_t hash = d3t_HashData(&key, sizeof(key));
    std::uint32_t index = hash % capacity;
    while (hashes[index] != hash) {
        const std::uint16_t distance = probes[index];
        if (distance == 0)
            return 0;
        index = (index + distance) % capacity;
    }
    return values[index];
}

bool AppendHashForSkinningBuffer(std::uint64_t handle, const Matrix4x4 &transform) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot || !slot->matrices)
        return false;
    const std::uint32_t matrixIndex = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    slot->matrices[matrixIndex] = transform;
    return true;
}


bool LookupSkinningTransform(std::uint64_t handle, Matrix4x4 &transform) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot || !slot->matrices)
        return false;
    const std::uint32_t matrixIndex = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    transform = slot->matrices[matrixIndex];
    return true;
}



bool PublishOsageSkinningTransform(std::uint64_t handle, const Vector3 &position, const Vector3 *directionTarget) {
    Matrix4x4 transform{};
    if (!GetHashForSkinningBuffer(handle) || !LookupSkinningTransform(handle, transform)) {
        return false;
    }
    if (directionTarget) {
        const Vector3 up{0.0F, 1.0F, 0.0F};
        const Vector3 direction{directionTarget->x - position.x, directionTarget->y - position.y, directionTarget->z - position.z};
        matrix_stack::ComputeRotationMatrixFromVectors(&transform, &up, &direction);
    }
    transform.rows[0].w = position.x;
    transform.rows[1].w = position.y;
    transform.rows[2].w = position.z;
    return AppendHashForSkinningBuffer(handle, transform);
}

std::uint64_t GetHashForSkinningBuffer(std::uint64_t handle) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot || !slot->entries || !slot->parentHashes)
        return 0;
    const std::uint32_t valueIndex = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    const std::uint16_t parentDistance = Read<std::uint16_t>(slot->entries, static_cast<std::size_t>(valueIndex) * 20U + 4U);
    if (!parentDistance || valueIndex < parentDistance)
        return 0;
    const std::uint32_t parentHash = slot->parentHashes[valueIndex - parentDistance];
    return static_cast<std::uint32_t>(handle) | (static_cast<std::uint64_t>(parentHash) << 32U);
}




bool ConvertSkinningNodeToParentSpace(std::uint64_t handle, Matrix4x4 &published) {
    const std::uint64_t parent = GetHashForSkinningBuffer(handle);
    if (!parent || !(parent >> 32U))
        return false;
    const Matrix4x4 global = ComputeGlobalSkinningTransform(parent);
    Matrix4x4 inverse{};
    Matrix4x4::invertMatrix(&global, &inverse);
    LookupSkinningTransform(handle, published);
    AppendHashForSkinningBuffer(handle, MultiplyMatrices(inverse, published));
    return true;
}



Matrix4x4 ComputeGlobalSkinningTransform(std::uint64_t handle) {
    const Matrix4x4 identity{{
        {1.0F, 0.0F, 0.0F, 0.0F},
        {0.0F, 1.0F, 0.0F, 0.0F},
        {0.0F, 0.0F, 1.0F, 0.0F},
        {0.0F, 0.0F, 0.0F, 1.0F},
    }};
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot || !slot->entries || !slot->matrices)
        return identity;

    std::uint32_t index = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    Matrix4x4 transform = slot->matrices[index];
    std::uint32_t remaining = slot->entryCount;
    while (index && remaining--) {
        const std::uint16_t parentDistance = Read<std::uint16_t>(slot->entries, static_cast<std::size_t>(index) * 20U + 4U);
        if (!parentDistance || index < parentDistance)
            break;
        index -= parentDistance;
        transform = MultiplyMatrices(slot->matrices[index], transform);
    }
    return transform;
}



void HeadRig_Publish(const HeadRigState *head) {
    if (!head || std::bit_cast<float>(head->active) == 0.0f || std::bit_cast<float>(head->initialized) == 0.0f || !head->cacheHandle ||
        std::bit_cast<float>(head->targetHash) == 0.0f || !head->sourceHash) {
        return;
    }
    const std::uint64_t source = head->cacheHandle | (static_cast<std::uint64_t>(head->sourceHash) << 32U);
    const std::uint64_t target = head->cacheHandle | (static_cast<std::uint64_t>(head->targetHash) << 32U);
    const Matrix4x4 placed = MultiplyMatrices(ComputeGlobalSkinningTransform(source), head->localTransform);
    Matrix4x4 inverseParent{};
    const Matrix4x4 parentGlobal = ComputeGlobalSkinningTransform(GetHashForSkinningBuffer(target));
    Matrix4x4::invertMatrix(&parentGlobal, &inverseParent);
    AppendHashForSkinningBuffer(target, MultiplyMatrices(inverseParent, placed));
}




std::uint64_t SetSkinningNodeDrawFlag(std::uint64_t handle, std::int32_t operation) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot)
        return handle;
    std::byte *const array = slot->entries;
    const std::uint32_t value = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    std::byte *const flags = array + static_cast<std::size_t>(value) * 20U + 16U;
    std::uint32_t current = Read<std::uint32_t>(flags, 0);
    switch (operation) {
    case 0:
        current |= 1U;
        break;
    case 1:
        current &= ~1U;
        std::memcpy(flags, &current, sizeof(current));
        return 0xfffe;
    case 2:
        current ^= 1U;
        break;
    case 3:
        current = 1U;
        std::memcpy(flags, &current, sizeof(current));
        return 1;
    default:
        break;
    }
    std::memcpy(flags, &current, sizeof(current));
    return reinterpret_cast<std::uint64_t>(array);
}


std::uint32_t GetSkinningNodeDescendantCount(std::uint64_t handle) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot)
        return 0;
    std::uint32_t index = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    std::uint32_t count = 0;
    for (std::int32_t remaining = 0; remaining >= 0;) {
        const std::uint32_t children = Read<std::uint16_t>(slot->entries + static_cast<std::size_t>(index++) * 20U, 0x08);
        count += children;
        remaining += static_cast<std::int32_t>(children) - 1;
    }
    return count;
}


std::uint32_t GetSkinningNodeRenderHandle(std::uint64_t handle) {
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(static_cast<std::uint32_t>(handle));
    if (!slot)
        return 0;
    const std::uint32_t index = GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(handle >> 32U));
    return index < slot->entryCount ? slot->renderHandles[index] : 0U;
}


std::uint64_t FindModelNodeByName(std::uint32_t modelHandle, const char *name) {
    const D3TModelRecord *const model = d3t::ResolveModel(modelHandle);
    if (!model)
        return 0;
    for (std::uint32_t row = 0; row < model->nodeCount; ++row) {
        const std::uint32_t offset = model->nodes[row].nameOffset;
        if (offset < model->stringsSize && std::strcmp(name, model->strings + offset) == 0)
            return (static_cast<std::uint64_t>(row) << 32U) | modelHandle;
    }
    return 0;
}



std::uint64_t AllocateSkinningNodeFromRenderHandle(std::uint32_t cacheHandle, std::uint64_t modelNode, std::uint64_t parentHandle) {
    const std::uint32_t modelHandle = static_cast<std::uint32_t>(modelNode);
    const std::uint32_t row = static_cast<std::uint32_t>(modelNode >> 32U);
    if (!cacheHandle || !modelHandle || !row)
        return 0;
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(cacheHandle);
    const D3TModelRecord *const model = d3t::ResolveModel(modelHandle);
    if (!slot || !model || row >= model->nodeCount)
        return 0;
    const D3TModelNode &source = model->nodes[row];
    const char *const name = source.nameOffset < model->stringsSize ? model->strings + source.nameOffset : nullptr;
    const std::uint64_t node = AllocateSkinningNode(parentHandle, name, modelHandle, true);
    if (!static_cast<std::uint32_t>(node) || !(node >> 32U))
        return 0;
    std::byte *const entry = slot->entries + static_cast<std::size_t>(GetValueFromMurmurHash(&slot->valueTable, static_cast<std::uint32_t>(node >> 32U))) * 20U;
    Write(entry, 0x0c, source.sprite);
    Write(entry, 0x0a, source.drawGroup);
    Write(entry, 0x0e, source.probe);
    Write(entry, 0x10, static_cast<std::uint16_t>(source.flags));
    return node;
}



void HeadRig_Init(HeadRigState *head, std::uint32_t entryNodeHash) {
    if (!head || head->initialized)
        return;
    head->initialized = 1;
    const std::uint64_t cache = head->cacheHandle;
    if (!cache || !entryNodeHash)
        return;
    const std::uint64_t target = AssignNode(cache | (static_cast<std::uint64_t>(entryNodeHash) << 32U), "HeadRig:c_kao_bn");
    if (!target || !(target >> 32U))
        return;
    head->targetHash = static_cast<std::uint32_t>(target >> 32U);
    if (!head->sourceHash)
        return;
    const Matrix4x4 source = ComputeGlobalSkinningTransform(cache | (static_cast<std::uint64_t>(head->sourceHash) << 32U));
    Matrix4x4 inverse{};
    Matrix4x4::invertMatrix(&source, &inverse);
    head->localTransform = MultiplyMatrices(inverse, ComputeGlobalSkinningTransform(cache | (static_cast<std::uint64_t>(head->targetHash) << 32U)));
}

void ModifyArrayElementsByMurmurHash(std::uint64_t handle, std::uint16_t mask, std::int32_t operation) {
    const std::uint32_t cacheHandle = static_cast<std::uint32_t>(handle);
    CachedSkinningBufferSlot *const slot = ResolveCachedSkinningBuffer(cacheHandle);
    if (!slot)
        return;
    std::byte *const array = slot->entries;
    if (!array)
        return;
    const std::uint32_t key = static_cast<std::uint32_t>(handle >> 32U);
    const std::uint32_t value = GetValueFromMurmurHash(&slot->valueTable, key);
    std::byte *flags = array + static_cast<std::size_t>(value) * 20U + 16U;
    const std::uint16_t count = Read<std::uint16_t>(array, static_cast<std::size_t>(value) * 20U + 6U);
    for (std::uint16_t item = 0; item < count; ++item, flags += 20) {
        std::uint16_t current = Read<std::uint16_t>(flags, 0);
        switch (operation) {
        case 0:
            current = static_cast<std::uint16_t>(current | mask);
            break;
        case 1:
            current = static_cast<std::uint16_t>(current & ~mask);
            break;
        case 2:
            current = static_cast<std::uint16_t>(current ^ mask);
            break;
        case 3:
            current = mask;
            break;
        default:
            break;
        }
        std::memcpy(flags, &current, sizeof(current));
    }
}

void TexSortWrapper(float frameTime) {
    for (std::uint32_t index = 0; index < g_CachedSkinningBuffers.entries.size(); ++index) {
        const std::uint32_t generation = g_CachedSkinningBuffers.generations[index];
        if (generation < 0x80000000U)
            continue;

        const std::uint32_t handle = (generation & 0xffffff80U) | index;
        ProcessSkinningCacheTexSort(handle, g_CachedSkinningBuffers.entries[index].data() + 0xb8, frameTime);
    }
}

} // namespace shenmue::s1
