#include "d3t/object_store.hpp"

#include "d3t/archive_data.hpp"
#include "d3t/asset_remap.hpp"
#include "d3t/model_paths.hpp"
#include "d3t/model_cache.hpp"
#include "common/matrix_stack.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/content.hpp"
#include "d3t/shader_load.hpp"
#include "shenmue.h"
#if SHENMUE_IS_S2
#include "s2/renderer_initialization.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/sd_light.hpp"
#include "s2/matrix_stack.hpp"
#endif

#include <cJSON.h>

#include <array>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <limits>
#include <stdexcept>

    namespace shenmue::d3t {

std::int32_t g_hashModelRequireAssignedNodes = 1;
std::int32_t g_hashModelRestrictCharacterPaths = -1;
ModelCacheIndexTable g_hashModelCharacterPaths{};
#if SHENMUE_IS_S2
std::int32_t g_uniqueModelCount = -1;
std::int32_t g_culledModelCount = -1;

static constexpr std::uint8_t hashAfterModelTransform = 1;
#endif
namespace {

struct ObjectEntry;
struct ObjectSource {
    std::uintptr_t key{};
    std::uint32_t threadHandle{};
    std::uint32_t loadHandle{};
    std::uint32_t renderHandle{};
    std::uint32_t flags{};
    std::uint32_t resolveState{};
    std::uint32_t modelKind{};
    char path[260]{};
    std::byte reserved124[20]{};
    std::uint32_t pathHash{};
    std::uint32_t references{};
    std::uint8_t retiring{};
    std::byte reserved141[7]{};
    D3TRawArray<ObjectEntry *> placements{};
    void *callbackData{};
    void (*callback)(void *, ObjectSource *){};
};
static_assert(sizeof(ObjectSource) == 0x168);
static_assert(offsetof(ObjectSource, placements) == 0x148);
static_assert(offsetof(ObjectSource, pathHash) == 0x138);

enum class EntryKind : std::uint32_t { Pending, ModelRuntime, Legacy };
enum class UpdateState : std::uint32_t { Idle, Dirty, Retire };
struct FacialExpressionChannel {
    std::uint32_t expression;
    float weight, duration, elapsed;
    std::uint8_t pending;
    std::byte reserved11[3];
};
// lookYaw and lookPitch are the eye's eased look-at angles, which ApplyEyeLookTarget keeps in the same eye's lid channel.
struct EyeLidExpressionChannel {
    float weight, duration;
    std::uint8_t pending;
    std::byte reserved09[3];
    float lookYaw, lookPitch;
};
struct FacialExpressionState {
    FacialExpressionChannel upper, lower, mouth;
    EyeLidExpressionChannel left, right;
    std::uint8_t initialized;
    std::byte reserved65[3];
};
static_assert(sizeof(FacialExpressionState) == 0x68);
static_assert(sizeof(FacialExpressionChannel) == 0x14);
static_assert(offsetof(FacialExpressionChannel, pending) == 0x10);
static_assert(sizeof(EyeLidExpressionChannel) == 0x14);
static_assert(offsetof(FacialExpressionState, lower) == 0x14);
static_assert(offsetof(FacialExpressionState, mouth) == 0x28);
static_assert(offsetof(FacialExpressionState, left) == 0x3c);
static_assert(offsetof(FacialExpressionState, right) == 0x50);
static_assert(offsetof(FacialExpressionState, initialized) == 0x64);
// The HD head rig's cache nodes: each eye's upper lid, eye and lower lid, then the head and eyelash meshes.
struct EyeControlNodeSet {
    std::uint64_t upperLid, eye, lowerLid;
};
struct EyeControlRig {
    EyeControlNodeSet left, right;
    std::uint64_t head, eyelashes;
};
static_assert(sizeof(EyeControlRig) == 0x40);
struct ObjectEntry {
    const void *placement{};
    ObjectSource *source{};
    std::uint64_t expressionTarget{};
    EntryKind kind{};
    std::uint8_t flags{};
    std::byte reserved1d[3]{};
    FacialExpressionState expressions{};
    ModelRuntimeState modelRuntime{};
    EyeControlRig *eyeControlRig{};
    std::byte screenMapping[0x24]{};
    std::uint32_t handle{};
    UpdateState update{};
    std::uint32_t reserved85b4{};
};
static_assert(sizeof(ObjectEntry) == 0x85b8);
static_assert(offsetof(ObjectEntry, modelRuntime) == 0x88);
static_assert(offsetof(ObjectEntry, eyeControlRig) == 0x8580);
static_assert(offsetof(ObjectEntry, handle) == 0x85ac);
static_assert(offsetof(ObjectEntry, update) == 0x85b0);



template <class Value, std::size_t Capacity> struct FixedMap {
    std::array<Value, Capacity> values{};
    std::array<std::uint32_t, Capacity> keys{};
    std::array<std::uint16_t, Capacity> probes{};
    std::uint32_t count{};

    std::size_t find(std::uint32_t key) const {
        std::size_t index = key & (Capacity - 1);
        for (;;) {
            if (keys[index] == key)
                return index;
            if (!probes[index])
                return Capacity;
            index = (index + probes[index]) & (Capacity - 1);
        }
    }

    std::size_t insert(std::uint32_t key, const Value &value) {
        if (count >= Capacity)
            return Capacity;
        std::size_t index = key & (Capacity - 1);
        std::size_t hole = Capacity;
        for (;;) {
            if (keys[index] == key)
                return index;
            if (!keys[index]) {
                if (hole == Capacity)
                    hole = index;
                if (!probes[index]) {
                    ++count;
                    keys[index] = key;
                    values[index] = value;
                    return index;
                }
            }
            if (!probes[index])
                break;
            index = (index + probes[index]) & (Capacity - 1);
        }
        if (hole != Capacity) {
            ++count;
            keys[hole] = key;
            values[hole] = value;
            return hole;
        }
        std::size_t next = (index + 1) & (Capacity - 1);
        while (next != index && keys[next])
            next = (next + 1) & (Capacity - 1);
        if (next == index)
            return Capacity;
        ++count;
        keys[next] = key;
        values[next] = value;
        probes[index] = static_cast<std::uint16_t>((next - index) & (Capacity - 1));
        return next;
    }

    bool erase(std::uint32_t key) {
        std::size_t index = key & (Capacity - 1);
        std::size_t previous = index;
        while (keys[index] != key) {
            if (!probes[index])
                return false;
            previous = index;
            index = (index + probes[index]) & (Capacity - 1);
        }
        keys[index] = 0;
        values[index] = {};
        if (previous != index && !probes[index])
            probes[previous] = 0;
        --count;
        return true;
    }
};

FixedMap<ObjectSource, 1024> sources;
FixedMap<std::uintptr_t, 16384> sourceAliases;
FixedMap<std::uintptr_t, 16384> publicAliases;
FixedMap<std::uint32_t, 1024> placementIndex;
std::array<ObjectEntry, 1024> entries{};
std::array<std::uint32_t, 1024> slotFlags{};
std::uint32_t nextSlot{};
bool dirty{};

#if SHENMUE_IS_S2
std::uint32_t objectStoreResetWord{};
#endif
std::uint32_t FoldKey(std::uintptr_t key) {
    return static_cast<std::uint32_t>(key + key / 0xffffffffULL);
}

ObjectEntry *FindPlacement(const void *placement) {
    if (!placement)
        return nullptr;
    const std::size_t index = placementIndex.find(FoldKey(reinterpret_cast<std::uintptr_t>(placement)));
    if (index == placementIndex.keys.size())
        return nullptr;
    const std::uint32_t handle = placementIndex.values[index];
    return slotFlags[handle & 0x3ffU] == (handle & 0xfffffc00U) ? &entries[handle & 0x3ffU] : nullptr;
}
#if SHENMUE_IS_S2
void InitializeObjectStoreExpressions(ObjectEntry &entry);
void BuildModelExpressionRig(ObjectEntry &entry);
#endif

ObjectEntry *ResolveHashedObjectEntry(const void *placement) {
    std::uintptr_t key = reinterpret_cast<std::uintptr_t>(placement);
    if (!key)
        return nullptr;
    const std::size_t alias = publicAliases.find(FoldKey(key));
    if (alias != publicAliases.keys.size())
        key = publicAliases.values[alias];
    return FindPlacement(reinterpret_cast<const void *>(key));
}

// @verified - p
bool IsModelPathInHashSet(const char *path) {
    const char *const slash = std::strrchr(path, '/');
    if (!slash || !slash[1])
        return false;
    const char *const name = slash + 1;
    const std::size_t length = std::strlen(name) - 4;
    if (length >= 8)
        return false;
    char stem[8];
    for (std::size_t index = 0; index < length; ++index)
        stem[index] = static_cast<char>(std::tolower(static_cast<unsigned char>(name[index])));
    stem[length] = '\0';
    const std::uint32_t hash = d3t_HashData(stem, static_cast<std::uint32_t>(length));
    const ModelCacheIndexTable &table = g_hashModelCharacterPaths;
    if (!table.capacity)
        return false;
    std::uint32_t index = hash % table.capacity;
    for (;;) {
        if (table.hashes[index] == hash)
            return true;
        if (!table.links[index])
            return false;
        index = (index + table.links[index]) % table.capacity;
    }
}

// @verified - p
bool HashOrQueueModelInstance(const void *placement, const float *alpha, std::uint32_t projectionMode, const Vector3 *mazeJoins = nullptr) {
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    if (!entry)
        return projectionMode == 1;
    const ObjectSource &source = *entry->source;
    bool queue = true;
    bool legacy = true;
    if (source.flags & 0x10U) {
        queue = projectionMode != 1;
        legacy = false;
    }
    if (projectionMode == 4) {
        queue = false;
        legacy = false;
    } else if (projectionMode == 1) {
        legacy = false;
    }
    if (g_hashModelRequireAssignedNodes && !entry->modelRuntime.nodeCount && source.modelKind == 1)
        queue = false;
    if ((!g_hashModelRestrictCharacterPaths || source.modelKind != 1 || IsModelPathInHashSet(source.path)) && queue && entry->kind == EntryKind::ModelRuntime) {
        if (source.flags & 0x2002U) {
            *matrix_stack::g_matrixStack.top.stack = matrix_stack::workTransformMatrix;
            Matrix4x4 transform = matrix_stack::workTransformMatrix;
            for (std::uint32_t row = 0; row < 4; ++row) {
                for (std::uint32_t column = row + 1; column < 4; ++column) {
                    const float value = transform.rows[row][column];
                    transform.rows[row][column] = transform.rows[column][row];
                    transform.rows[column][row] = value;
                }
            }
            SetModelCacheTransform(entry->expressionTarget, transform);
            if (source.flags & 2U) {
                SetModelCacheAlpha(entry->expressionTarget, 1.0f);
                if (alpha)
                    SetModelCacheAlpha(entry->expressionTarget, *alpha);
                if (mazeJoins) {
                    const Matrix4x4 left{
                        {{1.0f, 0.0f, 0.0f, mazeJoins[0].x}, {0.0f, 1.0f, 0.0f, mazeJoins[0].y}, {0.0f, 0.0f, 1.0f, mazeJoins[0].z}, {0.0f, 0.0f, 0.0f, 1.0f}}};
                    const Matrix4x4 right{
                        {{1.0f, 0.0f, 0.0f, mazeJoins[1].x}, {0.0f, 1.0f, 0.0f, mazeJoins[1].y}, {0.0f, 0.0f, 1.0f, mazeJoins[1].z}, {0.0f, 0.0f, 0.0f, 1.0f}}};
                    SetModelCacheTransform(FindModelCacheNode(entry->expressionTarget, "Bone_02"), left);
                    SetModelCacheTransform(FindModelCacheNode(entry->expressionTarget, "Bone_01"), right);
                }
                UpdateModelCacheFlags(entry->expressionTarget, 1, ModelCacheFlagMode::Clear);
            }
        }
        UpdateModelCacheFlags(entry->expressionTarget, 8, ModelCacheFlagMode::Clear);
        if (projectionMode == 2)
            legacy = false;
    }
    return !legacy;
}
ObjectSource *FindSource(const void *source) {
    std::uintptr_t key = reinterpret_cast<std::uintptr_t>(source);
    if (!key)
        return nullptr;
    const std::size_t alias = sourceAliases.find(FoldKey(key));
    if (alias != sourceAliases.keys.size())
        key = sourceAliases.values[alias];
    const std::size_t index = sources.find(FoldKey(key));
    if (index != sources.keys.size())
        return &sources.values[index];
    const ObjectEntry *const placement = FindPlacement(reinterpret_cast<const void *>(key));
    return placement ? placement->source : nullptr;
}

} // namespace

bool AuthoredAssetExists(const char *path) {
    // 38FB50 checks the fully hashed/remapped asset first, then its authored
    // archive path. Reuse the established asset-remap and archive owners.
    char fullHash[260];
    char archivePath[520];
    char remapped[260];
    GenerateFullHashString(path, fullHash);
    std::snprintf(archivePath, sizeof(archivePath), "./Tex/Assets/%s", fullHash);
    if (g_d3tAssetRemapping.state.load(std::memory_order_seq_cst) == 10 && genRemapPath(0, archivePath, remapped, 0, true)) {
        if (ArchiveData_GetFileSize(remapped))
            return true;
    } else if (ArchiveData_GetFileSize(archivePath))
        return true;
    std::snprintf(archivePath, sizeof(archivePath), "./Tex/Assets/%s", path);
    return ArchiveData_GetFileSize(archivePath) != 0;
}

namespace {

#if SHENMUE_IS_S2
std::uint64_t *AppendPlacementNodeMatch(D3TRawArray<std::uint64_t> &matches) {
    if (matches.count >= matches.capacity) {
        const std::uint32_t capacity = 2U * matches.capacity >= 16U ? 2U * matches.capacity : 16U;
        if (capacity > matches.capacity) {
            void *const allocation = std::realloc(matches.data, sizeof(std::uint64_t) * capacity);
            if (!allocation)
                return nullptr;
            matches.data = static_cast<std::uint64_t *>(allocation);
            matches.capacity = capacity;
        }
    }
    return matches.data + matches.count++;
}

void CollectPlacementNodeMatch(void *data, std::uint64_t handle) {
    D3TRawArray<std::uint64_t> &matches = *static_cast<D3TRawArray<std::uint64_t> *>(data);
    *AppendPlacementNodeMatch(matches) = handle;
}

void VisitModelCacheNodeNames(std::uint64_t handle, const char *text, void *data, void (*callback)(void *, std::uint64_t)) {
    if (std::strstr(GetModelCacheNodeName(handle), text))
        callback(data, handle);
    if (static_cast<std::uint32_t>(handle) && static_cast<std::uint32_t>(handle >> 32U)) {
        const ModelCacheSlot &cache = *ResolveModelCache(static_cast<std::uint32_t>(handle));
        const std::uint32_t index = LookupModelCacheIndex(cache.indexTable, static_cast<std::uint32_t>(handle >> 32U));
        const std::uint32_t count = cache.nodes[index].childCount;
        for (std::uint32_t child = 0; child < count; ++child) {
            const std::uint64_t next = GetModelCacheChild(handle, child);
            if (next != handle)
                VisitModelCacheNodeNames(next, text, data, callback);
        }
    }
}

void MarkPlacementDayNight(std::uint64_t handle) {
    D3TRawArray<std::uint64_t> days{};
    if (std::strstr(GetModelCacheNodeName(handle), "_day"))
        *AppendPlacementNodeMatch(days) = handle;
    if (static_cast<std::uint32_t>(handle) && static_cast<std::uint32_t>(handle >> 32U)) {
        const ModelCacheSlot &cache = *ResolveModelCache(static_cast<std::uint32_t>(handle));
        const std::uint32_t index = LookupModelCacheIndex(cache.indexTable, static_cast<std::uint32_t>(handle >> 32U));
        const std::uint32_t count = cache.nodes[index].childCount;
        for (std::uint32_t child = 0; child < count; ++child) {
            const std::uint64_t next = GetModelCacheChild(handle, child);
            if (next != handle)
                VisitModelCacheNodeNames(next, "_day", &days, CollectPlacementNodeMatch);
        }
    }
    for (std::uint32_t index = 0; index < days.count; ++index)
        UpdateModelCacheFlags(days.data[index], MCF_Day, ModelCacheFlagMode::Enable);
    D3TRawArray<std::uint64_t> nights{};
    if (std::strstr(GetModelCacheNodeName(handle), "_night"))
        *AppendPlacementNodeMatch(nights) = handle;
    if (static_cast<std::uint32_t>(handle) && static_cast<std::uint32_t>(handle >> 32U)) {
        const ModelCacheSlot &cache = *ResolveModelCache(static_cast<std::uint32_t>(handle));
        const std::uint32_t index = LookupModelCacheIndex(cache.indexTable, static_cast<std::uint32_t>(handle >> 32U));
        const std::uint32_t count = cache.nodes[index].childCount;
        for (std::uint32_t child = 0; child < count; ++child) {
            const std::uint64_t next = GetModelCacheChild(handle, child);
            if (next != handle)
                VisitModelCacheNodeNames(next, "_night", &nights, CollectPlacementNodeMatch);
        }
    }
    for (std::uint32_t index = 0; index < nights.count; ++index)
        UpdateModelCacheFlags(nights.data[index], MCF_Night, ModelCacheFlagMode::Enable);
}

// @verified - p
void RegisterPlacementLight(ObjectEntry &entry, const D3TModelSprite &sprite, std::uint64_t node, const char *textureName) {
    entry.source->flags |= OSF_HasLightBindings;
    const Matrix4x4 world = ComputeModelCacheGlobalTransform(node);
    const std::uint32_t index = s2::g_sdLightCount;
    s2::SDLightRecord &light = s2::g_sdLights[index];
    light.colour = sprite.colour;
    light.type = sprite.type;
    const float reciprocalW = 1.0f / (((world.rows[3].y * 0.0f + world.rows[3].x * 0.0f) + world.rows[3].z * 0.0f) + world.rows[3].w);
    light.position.x = reciprocalW * (((world.rows[0].y * 0.0f + world.rows[0].x * 0.0f) + world.rows[0].z * 0.0f) + world.rows[0].w);
    light.position.y = reciprocalW * (((world.rows[1].y * 0.0f + world.rows[1].x * 0.0f) + world.rows[1].z * 0.0f) + world.rows[1].w);
    light.position.z = reciprocalW * (((world.rows[2].y * 0.0f + world.rows[2].x * 0.0f) + world.rows[2].z * 0.0f) + world.rows[2].w);
    light.direction = {world.rows[0].z, world.rows[1].z, world.rows[2].z};
    const ModelCacheSlot &cache = *ResolveModelCache(static_cast<std::uint32_t>(node));
    const Matrix4x4 local = cache.matrices[LookupModelCacheIndex(cache.indexTable, static_cast<std::uint32_t>(node >> 32U))];
    Vector4 angles;
    Matrix4x4::ExtractEulerAnglesFromMatrix(&local, &angles);
    light.localRotationDegrees = {angles.x * 180.0f / 3.1415901f, angles.y * 180.0f / 3.1415901f, angles.z * 180.0f / 3.1415901f};
    light.range = sprite.range;
    light.coneCosine = std::cos(sprite.coneAngle * 0.5f);
    light.textureHandle = sprite.textureHandle;
    light.textureName = textureName;
    light.shadowMode = sprite.shadowMode;
    light.shadowBias = sprite.shadowBias;
    light.shadowIndex = 0;
    light.parameter104 = sprite.parameter0c;
    light.parameter108 = sprite.parameter2c;
    light.parameterEnabled = sprite.parameter0c != 0.0f;
    light.parameter128 = sprite.parameter34;
    light.nodeCache = static_cast<std::uint32_t>(node);
    light.nodeHash = static_cast<std::uint32_t>(node >> 32U);
    const std::uint64_t owner = s2::ResolveSDLightOwner(node);
    light.ownerCache = static_cast<std::uint32_t>(owner);
    light.ownerNode = static_cast<std::uint32_t>(owner >> 32U);
    s2::g_sdLightCount = index + 1;
    light.state = 0;
    s2::AddSDLightOverride(owner);
}
std::uint32_t RegisterPlacementLights(ObjectEntry &entry) {
    const std::uint64_t root = entry.expressionTarget;
    ModelCacheSlot *const cache = ResolveModelCache(static_cast<std::uint32_t>(root));
    if (!cache)
        return 0;
    std::uint32_t index = LookupModelCacheIndex(cache->indexTable, static_cast<std::uint32_t>(root >> 32U));
    std::uint32_t count = 0;
    std::int32_t pending = 0;
    do {
        const D3TModelNode &node = cache->nodes[index];
        if (node.sprite) {
            const D3TModelSprite &sprite = cache->sprites[node.sprite - 1U];
            const char *textureName = nullptr;
            if (sprite.texture) {
                const D3TModelRecord &model = *ResolveModel(cache->modelHandles[index]);
                const std::uint32_t offset = model.textures[sprite.texture - 1U].pathOffset;
                if (offset < model.stringsSize)
                    textureName = model.strings + offset;
            }
            RegisterPlacementLight(entry, sprite, static_cast<std::uint64_t>(cache->nodeHashes[index]) << 32U | static_cast<std::uint32_t>(root), textureName);
            ++count;
        }
        ++index;
        pending += static_cast<std::int32_t>(node.childCount) - 1;
    } while (pending >= 0);
    return count;
}
#endif
void BindPlacement(ObjectEntry &entry, const void *placement) {

#if SHENMUE_IS_S2
    if (entry.placement && entry.kind == EntryKind::ModelRuntime)
        s2::RemoveSDLightOwner(entry.expressionTarget);
#endif
    entry.modelRuntime = {};
    if (entry.placement) {
        const std::uintptr_t oldPlacement = reinterpret_cast<std::uintptr_t>(entry.placement);
        for (std::size_t index = 0; index < publicAliases.keys.size(); ++index)
            if (publicAliases.keys[index] && publicAliases.values[index] == oldPlacement)
                publicAliases.erase(publicAliases.keys[index]);
        placementIndex.erase(FoldKey(oldPlacement));
    }
    entry.placement = placement;
    if (placement) {
        placementIndex.insert(FoldKey(reinterpret_cast<std::uintptr_t>(placement)), entry.handle);
#if SHENMUE_IS_S2
        if (entry.kind == EntryKind::ModelRuntime) {
            MarkPlacementDayNight(entry.expressionTarget);
            RegisterPlacementLights(entry);
            if (entry.source->modelKind == 1)
                InitializeObjectStoreExpressions(entry);
            UpdateModelCacheFlags(entry.expressionTarget, 1, ModelCacheFlagMode::Clear);
        }
#endif
    }
#if SHENMUE_IS_S2
    else if (

        entry.kind == EntryKind::ModelRuntime) {
        UpdateModelCacheFlags(entry.expressionTarget, 1, ModelCacheFlagMode::Enable);
    }
#endif
}

#if SHENMUE_IS_S2
bool ResolveSource(ObjectSource &source, bool checkShaders) {
    if (source.resolveState != 0)
        return source.resolveState == 1;
    if (d3t_IsRenderHandlePending(source.threadHandle))
        return false;
    const D3TModelLoad *const load = static_cast<const D3TModelLoad *>(d3t_GetAsyncLoadRecord(source.loadHandle));
    const std::uint32_t model = load && ResolveModel(load->modelHandle) ? load->modelHandle : 0;
    if (source.flags & OSF_CopyMaterials)
        CopyModelMaterials(source.renderHandle, model);
    else
        source.renderHandle = model;
    if (checkShaders && !SetupShaderReferences(source.renderHandle))
        return source.resolveState == 1;
    d3t_CloseRenderHandle(source.threadHandle);
    source.loadHandle = 0;
    if (!source.renderHandle) {
        source.resolveState = 2;
        if (source.callback)
            source.callback(source.callbackData, &source);
        return false;
    }
    source.resolveState = 1;
    if (source.callback)
        source.callback(source.callbackData, &source);
    const char *const settings = GetModelSettings(source.renderHandle);
    if (settings) {
        cJSON *const root = cJSON_Parse(settings);
        if (root) {
            const cJSON *const shadow = cJSON_GetObjectItem(root, "DedicatedShadow");

            if (shadow && shadow->valueint == 1)
                source.flags |= OSF_DedicatedShadow;
            cJSON_Delete(root);
        }
    }

    return source.resolveState == 1;
}
#endif
bool RetireSource(ObjectSource &source) {
    if (source.placements.count) {
        for (std::uint32_t placement = 0; placement < source.placements.count; ++placement)
            if (source.placements.data[placement])
                source.placements.data[placement]->update = UpdateState::Retire;
        return false;
    }
#if SHENMUE_IS_S2
    ReleaseModelHandle(source.renderHandle);
    for (std::size_t alias = 0; alias < publicAliases.keys.size(); ++alias)
        if (publicAliases.keys[alias] && publicAliases.values[alias] == source.key)
            publicAliases.erase(publicAliases.keys[alias]);
#endif
    for (std::size_t alias = 0; alias < sourceAliases.keys.size(); ++alias)
        if (sourceAliases.keys[alias] && sourceAliases.values[alias] == source.key)
            sourceAliases.erase(sourceAliases.keys[alias]);
    std::free(source.placements.data);
    sources.erase(FoldKey(source.key));
    return true;
}
void AppendPlacement(ObjectSource &source, ObjectEntry *entry) {
    auto &list = source.placements;
    if (list.count == list.capacity) {
        const std::uint32_t capacity = list.capacity ? list.capacity * 2 : 1;
        void *const allocation = std::realloc(list.data, capacity * sizeof(ObjectEntry *));
        if (!allocation)
            throw std::bad_alloc();
        list.data = static_cast<ObjectEntry **>(allocation);
        list.capacity = capacity;
    }
    list.data[list.count++] = entry;
}

void RetireEntry(ObjectEntry &entry) {
    ObjectSource *const source = entry.source;
    const bool keep = source && (source->placements.count <= 1 || (source->flags & 0x4000U)) && entry.update != UpdateState::Retire;
    if (source && !keep) {
        auto &list = source->placements;
        for (std::uint32_t index = 0; index < list.count; ++index) {
            if (list.data[index] == &entry)
                list.data[index] = list.data[--list.count];
        }
    }
    BindPlacement(entry, nullptr);
    if (entry.kind == EntryKind::ModelRuntime) {
        UpdateModelCacheFlags(entry.expressionTarget, 1, ModelCacheFlagMode::Enable);
        if (!keep) {
            std::free(entry.eyeControlRig);
            entry.eyeControlRig = nullptr;
            RetireModelCacheNodeExpressions(entry.expressionTarget);
            RemoveModelCacheSubtree(entry.expressionTarget);
            entry.expressionTarget = 0;
        }
    }
    if (!keep) {
        std::uint32_t &tag = slotFlags[entry.handle & 0x3ffU];
        if (tag == (entry.handle & 0xfffffc00U))
            tag &= 0x7fffffffU;
        entry = {};
    }
    entry.update = UpdateState::Idle;
}

} // namespace

#if SHENMUE_IS_S2
// @verified - p
bool IsObjectModelRuntime(const void *placement) {
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    return entry && entry->kind == EntryKind::ModelRuntime;
}

// @verified - p
std::uint32_t GetShouldHashFlag() {
    return hashAfterModelTransform;
}

// @verified - p
void IncrementHashedModelCount(std::int32_t models, std::int32_t culled) {
    g_uniqueModelCount = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(g_uniqueModelCount) + static_cast<std::uint32_t>(models));
    g_culledModelCount = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(g_culledModelCount) + static_cast<std::uint32_t>(culled));
}

void ResetHashedModelCounts() {
    g_uniqueModelCount = 0;
    g_culledModelCount = 0;
}
#endif

ModelRuntimeState *GetModelRuntimeState(const void *placement) {
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    if (!entry || entry->kind != EntryKind::ModelRuntime)
        return nullptr;
    ModelRuntimeState &state = entry->modelRuntime;
    if (!state.cacheHandle) {
        state.cacheHandle = static_cast<std::uint32_t>(entry->expressionTarget);
        state.rootHash = static_cast<std::uint32_t>(entry->expressionTarget >> 32);
        const char *const name = std::strrchr(entry->source->path, '/');
        std::strcpy(state.modelName, name ? name + 1 : entry->source->path);
        if (char *const dot = std::strchr(state.modelName, '.'))
            *dot = '\0';
    }
    return &state;
}

// @verified - p
std::uint64_t GetObjectModelPathHash(const void *placement) {
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    return entry ? entry->source->pathHash : 0;
}

const void *GetObjectModelTraversalSource(const void *placement) {
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    return entry && (entry->flags & 0x10U) && entry->source ? reinterpret_cast<const void *>(entry->source->key) : placement;
}
#if SHENMUE_IS_S2
namespace {
constexpr const char *faceElements[]{"UpperFace", "LowerFace", "FullFace", "LeftEyeLid", "RightEyeLid", "Mouth"};
constexpr const char *faceMouthNames[]{"MouthN", "MouthA", "MouthI", "MouthU", "MouthE", "MouthO", "MouthS"};
constexpr const char *faceEmotionNames[]{"NORMAL", "ANGRY", "DIFFICULTY", "SMILE", "SURPRISE", "SAD", "SUFFER", "WORRY", "WICKED", "DRUNKEN", "PITIABLE",
    "PAINFUL", "DIFFSMILE", "SCHEME", "BITE", "SMILE2", "PROVO1R", "PROVO1L", "PROVO2", "SMILE_KUN", "PAIN_CHO", "SMILE_SIN2", "ANGRY_SIN"};

void FormatObjectExpressionName(char (&name)[128], std::uint32_t emotion, std::uint32_t mouth) {
    std::snprintf(name, sizeof(name), "SM2Exp_%s-%s", faceEmotionNames[emotion < 23 ? emotion : 0], faceMouthNames[mouth < 7 ? mouth : 0]);
}
bool AdmitExpressionFrames(float &frames) {
    if (frames >= 1000.0f) {
        if (frames < 2000.0f)
            return false;
        frames -= 2000.0f;
    }
    return true;
}

bool StageFacialExpression(FacialExpressionChannel &channel, std::uint32_t expression, float frames, float weight) {
    if (channel.expression == expression && !(std::fabs(channel.weight - weight) > 0.01f))
        return false;
    channel.expression = expression;
    channel.weight = weight;
    channel.elapsed = 0.0f;
    channel.duration = frames * 0.033333335f;
    channel.pending = 1;
    return true;
}


void InitializeObjectStoreExpressions(ObjectEntry &entry) {
    if (entry.expressions.initialized)
        return;
    entry.expressions.initialized = 1;
    SetObjectFacialExpression(entry.placement, 0, FacialExpressionLayer::Upper, 0.0f, 1.0f);
    SetObjectFacialExpression(entry.placement, 0, FacialExpressionLayer::Lower, 0.0f, 1.0f);
    SetObjectMouthExpression(entry.placement, 0, 0.0f, 1.0f);
}

constexpr const char *eyeControlNodeNames[]{
    "L_Eye_Hi_Blink_Bn", "L_Eye_Bn", "L_Eye_Lo_Blink_Bn", "R_Eye_Hi_Blink_Bn", "R_Eye_Bn", "R_Eye_Lo_Blink_Bn", "head_geo", "eyelashes_geo"};
constexpr const char *eyeControlMarkerNames[]{"Node_LeftEyeLidUpper", "Node_LeftEye", "Node_LeftEyeLidLower", "Node_RightEyeLidUpper", "Node_RightEye",
    "Node_RightEyeLidLower", "Mesh_Head", "Mesh_Eyelashes"};

std::int32_t eyeControlAngleMode{};




void BuildModelExpressionRig(ObjectEntry &entry) {
    entry.eyeControlRig = static_cast<EyeControlRig *>(std::malloc(sizeof(EyeControlRig)));
    EyeControlRig &rig = *entry.eyeControlRig;
    std::uint64_t *const nodes[]{
        &rig.left.upperLid, &rig.left.eye, &rig.left.lowerLid, &rig.right.upperLid, &rig.right.eye, &rig.right.lowerLid, &rig.head, &rig.eyelashes};
    for (std::size_t index = 0; index < 8; ++index)
        *nodes[index] = FindModelCacheNode(entry.expressionTarget, eyeControlNodeNames[index]);
    const char *const settings = GetModelSettings(entry.source->renderHandle);
    cJSON *const root = settings ? cJSON_Parse(settings) : nullptr;
    if (root) {
        if (cJSON *const markers = cJSON_GetObjectItem(root, "ModelMarkers")) {
            for (std::size_t index = 0; index < 8; ++index) {
                *nodes[index] = FindModelCacheNode(entry.expressionTarget, eyeControlNodeNames[index]);
                const cJSON *const marker = cJSON_GetObjectItem(markers, eyeControlMarkerNames[index]);
                if (!marker)
                    continue;
                if (const std::uint64_t node = FindModelCacheNode(entry.expressionTarget, marker->valuestring))
                    *nodes[index] = node;
            }
        }
        cJSON_Delete(root);
    }
    for (const std::uint64_t node : {rig.eyelashes, rig.head})
        if (static_cast<std::uint32_t>(node) && node >> 32)
            UpdateModelCacheFlags(node, 0x20, ModelCacheFlagMode::Enable);
}



void TurnEyeControlNode(std::uint64_t node, std::uint16_t zAngle, const std::uint16_t *eyeAngles) {
    Matrix4x4 transform = GetModelCacheTransform(node);
    const Vector3 position{transform.rows[0].w, transform.rows[1].w, transform.rows[2].w};
    s2::Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundZ_Wrapper(zAngle);
    if (eyeAngles) {
        Matrix4x4::RotateAroundY_Wrapper(eyeAngles[0]);
        Matrix4x4::RotateAroundX_Wrapper(eyeAngles[1]);
    }
    *matrix_stack::g_matrixStack.top.stack = matrix_stack::workTransformMatrix;
    for (std::size_t row = 0; row < 4; ++row)
        for (std::size_t column = 0; column < 4; ++column)
            transform.rows[row][column] = matrix_stack::workTransformMatrix.rows[column][row];
    Matrix4x4::Pop(1);
    transform.rows[0].w = position.x;
    transform.rows[1].w = position.y;
    transform.rows[2].w = position.z;
    SetModelCacheTransform(node, transform);
}



void ApplyEyeControlAngles(const void *placement, std::uint32_t eye, std::uint16_t yAngle, std::int32_t zAngle, std::int16_t xAngle) {
    if (eye >= 2)
        return;
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    if (!entry || !entry->eyeControlRig)
        return;
    const EyeControlNodeSet &nodes = eye ? entry->eyeControlRig->right : entry->eyeControlRig->left;
    if (static_cast<std::uint32_t>(nodes.eye) && nodes.eye >> 32) {
        const std::uint16_t eyeAngles[]{yAngle, static_cast<std::uint16_t>(-xAngle)};
        TurnEyeControlNode(nodes.eye, static_cast<std::uint16_t>(-zAngle), eyeAngles);
    }
    const std::uint16_t lidAngle = static_cast<std::uint16_t>(-static_cast<std::int32_t>(static_cast<float>(zAngle) * 0.75f));
    if (static_cast<std::uint32_t>(nodes.upperLid) && nodes.upperLid >> 32)
        TurnEyeControlNode(nodes.upperLid, lidAngle, nullptr);
    if (static_cast<std::uint32_t>(nodes.lowerLid) && nodes.lowerLid >> 32)
        TurnEyeControlNode(nodes.lowerLid, lidAngle, nullptr);
}

void SubmitPendingEyeLidExpression(ObjectEntry &entry, const std::uint32_t *masks, bool left) {
    auto &eye = left ? entry.expressions.left : entry.expressions.right;
    if (!eye.pending)
        return;
    char name[128];
    if (eye.weight <= 0)
        FormatObjectExpressionName(name, entry.expressions.upper.expression, entry.expressions.mouth.expression);
    else
        std::strcpy(name, left ? "SM2Exp_LeftBlink" : "SM2Exp_RightBlink");
    QueueModelExpression(entry.expressionTarget, {.name = name, .elementMask = masks[left ? 3 : 4], .weight = eye.weight, .transitionDuration = eye.duration});
    eye.pending = 0;
}

void UpdateModelExpressionState(ObjectEntry &entry) {
    auto &state = entry.expressions;
    const float elapsed = s2::GetFrameTime();
    state.upper.elapsed += elapsed;
    state.lower.elapsed += elapsed;
    state.mouth.elapsed += elapsed;
    std::uint32_t masks[6]{};
    for (std::size_t element = 0; element < 6; ++element)
        masks[element] = FindExpressionElementMask(entry.expressionTarget, faceElements[element]);
    char name[128];
    if (state.upper.pending) {
        FormatObjectExpressionName(name, state.upper.expression, state.mouth.expression);
        ModelExpressionSubmission submission{.name = name, .elementMask = masks[0], .weight = state.upper.weight, .transitionDuration = state.upper.duration};
        if (!(state.left.weight > 0.00000011920929f)) {
            submission.elementMask |= masks[3];
            state.left.pending = 0;
        }
        if (state.right.weight <= 0.00000011920929f) {
            submission.elementMask |= masks[4];
            state.right.pending = 0;
        }
        if (state.upper.expression == state.lower.expression && state.upper.weight == state.lower.weight) {
            if (state.lower.pending) {
                if (submission.transitionDuration < state.lower.duration)
                    submission.transitionDuration = state.lower.duration;
                state.lower.pending = 0;
                submission.elementMask |= masks[1];
            }
            if (state.mouth.pending && state.lower.weight == state.mouth.weight) {
                if (submission.transitionDuration < state.mouth.duration)
                    submission.transitionDuration = state.mouth.duration;
                state.mouth.pending = 0;
                submission.elementMask |= masks[5];
            }
        }
        QueueModelExpression(entry.expressionTarget, submission);
        state.upper.pending = 0;
    }
    if (state.lower.pending) {
        FormatObjectExpressionName(name, state.lower.expression, state.mouth.expression);
        ModelExpressionSubmission submission{.name = name, .elementMask = masks[1], .weight = state.lower.weight, .transitionDuration = state.lower.duration};
        if (state.mouth.pending && state.lower.weight == state.mouth.weight) {
            if (submission.transitionDuration < state.mouth.duration)
                submission.transitionDuration = state.mouth.duration;
            state.mouth.pending = 0;
            submission.elementMask |= masks[5];
        }
        QueueModelExpression(entry.expressionTarget, submission);
        state.lower.pending = 0;
    }
    if (state.mouth.pending) {
        FormatObjectExpressionName(name, state.lower.expression, state.mouth.expression);
        QueueModelExpression(
            entry.expressionTarget, {.name = name, .elementMask = masks[5], .weight = state.mouth.weight, .transitionDuration = state.mouth.duration});
        state.mouth.pending = 0;
    }
    SubmitPendingEyeLidExpression(entry, masks, true);
    SubmitPendingEyeLidExpression(entry, masks, false);
}
} // namespace

void SetObjectFacialExpression(const void *placement, std::uint32_t expression, FacialExpressionLayer layer, float transitionFrames, float weight) {
    if (!AdmitExpressionFrames(transitionFrames))
        return;
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    if (!entry)
        return;
    if (layer == FacialExpressionLayer::Full) {

        SetObjectFacialExpression(placement, expression, FacialExpressionLayer::Upper, transitionFrames, weight);
        if (!AdmitExpressionFrames(transitionFrames))
            return;
        layer = FacialExpressionLayer::Lower;
    }
    auto &channel = layer == FacialExpressionLayer::Upper ? entry->expressions.upper : entry->expressions.lower;
    if (StageFacialExpression(channel, expression, transitionFrames, weight) && layer == FacialExpressionLayer::Lower)
        entry->expressions.mouth.pending = 1;
}

void SetObjectMouthExpression(const void *placement, std::uint32_t expression, float transitionFrames, float weight) {
    if (!AdmitExpressionFrames(transitionFrames))
        return;
    if (ObjectEntry *const entry = ResolveHashedObjectEntry(placement))
        StageFacialExpression(entry->expressions.mouth, expression, transitionFrames, weight);
}

void SetObjectEyeLidExpressionWeights(const void *placement, float leftWeight, float rightWeight, float transitionFrames) {
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    if (!entry)
        return;
    const auto stage = [transitionFrames](EyeLidExpressionChannel &channel, float weight) {
        weight = std::fmax(0.0f, std::fmin(1.0f, weight));
        if (weight < 0.1f)
            weight = 0.0f;
        if (std::fabs(weight - channel.weight) > std::numeric_limits<float>::epsilon()) {
            channel.weight = weight;
            channel.pending = 1;
            channel.duration = transitionFrames * 0.033333335f;
        }
    };
    stage(entry->expressions.left, leftWeight);
    stage(entry->expressions.right, rightWeight);
}

void UpdateModelExpressions(const void *placement) {
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    if (entry && entry->expressions.initialized)
        UpdateModelExpressionState(*entry);
}





void ApplyEyeLookTarget(const void *placement, std::uint32_t eye, std::uint16_t yAngle, std::int32_t zAngle, std::int16_t xAngle, const Vector3 *target) {
    if (eye >= 2 || !target)
        return;
    if (eyeControlAngleMode) {
        ApplyEyeControlAngles(placement, eye, yAngle, zAngle, xAngle);
        return;
    }
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    if (!entry || !entry->eyeControlRig)
        return;
    const EyeControlNodeSet &nodes = eye ? entry->eyeControlRig->right : entry->eyeControlRig->left;
    EyeLidExpressionChannel &channel = eye ? entry->expressions.right : entry->expressions.left;
    const s2::EyeRigParameters &parameters = s2::g_eyeRigParameters;
    float yaw = 0.0f;
    float pitch = 0.0f;
    if (static_cast<std::uint32_t>(nodes.eye) && nodes.eye >> 32) {
        Matrix4x4 transform = GetModelCacheTransform(nodes.eye);
        const Vector3 position{transform.rows[0].w, transform.rows[1].w, transform.rows[2].w};
        if (std::fabs(target->x) > 0.0f && std::fabs(target->y) > 0.0f && std::fabs(target->z) > 0.0f) {
            const Matrix4x4 global = ComputeModelCacheGlobalTransform(nodes.eye);
            Matrix4x4 inverse{};
            matrix_stack::createTransformedMatrix(&global, &inverse);
            Vector4 direction{};
            for (std::size_t row = 0; row < 3; ++row)
                direction[row] = ((inverse.rows[row].x * target->x + inverse.rows[row].y * target->y) + inverse.rows[row].z * target->z) + inverse.rows[row].w;
            const float scale =
                1.0f / std::sqrt(((direction.x * direction.x + direction.y * direction.y) + direction.z * direction.z) + direction.w * direction.w);
            direction = {direction.x * scale, direction.y * scale, direction.z * scale, direction.w * scale};
            yaw = std::atan2(direction.z, direction.y);
            yaw = parameters.yawLimit < yaw ? parameters.yawLimit : yaw;
            yaw = -parameters.yawLimit > yaw ? -parameters.yawLimit : yaw;
            pitch = -std::asin(direction.x);
            pitch = parameters.pitchLimit < pitch ? parameters.pitchLimit : pitch;
            pitch = -parameters.pitchLimit > pitch ? -parameters.pitchLimit : pitch;
        }
        const float frameTime = s2::GetFrameTime();
        yaw = (yaw - channel.lookYaw) * s2::g_eyeControlSmoothingRate * frameTime + channel.lookYaw;
        channel.lookYaw = yaw;
        pitch = (pitch - channel.lookPitch) * s2::g_eyeControlSmoothingRate * frameTime + channel.lookPitch;
        channel.lookPitch = pitch;
        matrix_stack::RotateMatrixByAngles(&transform, yaw, 0.0f, pitch);
        transform.rows[0].w = position.x;
        transform.rows[1].w = position.y;
        transform.rows[2].w = position.z;
        SetModelCacheTransform(nodes.eye, transform);
    }
    const float lidPitch = parameters.lidPitchScale * pitch;
    const float lidYaw = parameters.lidYawScale * yaw;
    if (!static_cast<std::uint32_t>(nodes.upperLid) || !(nodes.upperLid >> 32) || !static_cast<std::uint32_t>(nodes.lowerLid) || !(nodes.lowerLid >> 32))
        return;
    Matrix4x4 lid = GetModelCacheTransform(nodes.upperLid);
    const Vector3 position{lid.rows[0].w, lid.rows[1].w, lid.rows[2].w};
    matrix_stack::RotateMatrixByAngles(&lid, lidYaw, 0.0f, lidPitch);
    lid.rows[0].w = position.x;
    lid.rows[1].w = position.y;
    lid.rows[2].w = position.z;
    SetModelCacheTransform(nodes.upperLid, lid);
    SetModelCacheTransform(nodes.lowerLid, lid);
}
#endif



std::uint64_t ResolveModelRuntimeNode(const void *placement, const char *name, std::int16_t kind) {
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    if (!entry || entry->kind != EntryKind::ModelRuntime)
        return 0;
    const auto unique = [&](std::uint64_t handle) {
        const auto hash = static_cast<std::uint32_t>(handle >> 32);
        if (!static_cast<std::uint32_t>(handle) || !hash)
            return false;
        for (std::uint32_t index = 0; index < entry->modelRuntime.recordCount; ++index)
            if (entry->modelRuntime.nodes[index].nodeHash == hash)
                return false;
        return true;
    };
    const auto candidate = FindModelCacheNode(entry->expressionTarget, name);
    if (unique(candidate))
        return candidate;
    constexpr const char *aliases[][2]{{"Node_tgy_m_023_KP00120", "Node_tgy_m_024_KP00120"}, {"Node_tgy_m_024_KP00120", "Node_tgy_m_025_KP00120"},
        {"Node_tgy_m_025_KP00120", "Node_tgy_m_026_KP00120"}, {"Node_tgy_m_026_KP00120", "Node_tgy_m_027_KP00120"},
        {"Node_tgy_m_027_KP00120", "Node_tgy_m_028_KP00120"}, {"Node_tgy_m_028_KP00120", "Node_tgy_m_029_KP00120"},
        {"Node_neko200t_000_KP00000", "Node_neko200t_000_KP00001"}, {"Node_neko2bkt_000_KP00000", "Node_neko2bkt_000_KP00001"},
        {"Node_neko2wht_000_KP00000", "Node_neko2wht_000_KP00001"}};
    for (const auto &alias : aliases) {
        const char *left = name;
        const char *right = alias[0];
        while (*left && std::tolower(static_cast<unsigned char>(*left)) == std::tolower(static_cast<unsigned char>(*right))) {
            ++left;
            ++right;
        }
        if (*left || *right)
            continue;
        const auto mapped = FindModelCacheNode(entry->expressionTarget, alias[1]);
        if (unique(mapped))
            return mapped;
    }
    if (entry->source && entry->source->modelKind == 1 && kind != -1) {
        char suffix[32];
        std::snprintf(suffix, sizeof(suffix), "_K%c%05u", kind < 0 ? 'M' : 'P', static_cast<unsigned int>(std::abs(static_cast<int>(kind))));
        const auto mapped = FindModelCacheNodeContaining(entry->expressionTarget, suffix);
        if (unique(mapped))
            return mapped;
    }
    return 0;
}

// @verified - p
bool RegisterObjectAlias(const void *publicKey, const void *placement) {
    return publicAliases.insert(FoldKey(reinterpret_cast<std::uintptr_t>(publicKey)), reinterpret_cast<std::uintptr_t>(placement)) != publicAliases.keys.size();
}

bool RegisterObjectSource(const void *source, const char *modelPath, std::uint16_t flags) {
    if (ObjectSource *const existing = FindSource(source)) {
        if (!existing->retiring)
            return false;
        existing->retiring = 0;
        if (!(flags & 0x20U))
            RegisterObjectPlacement(source, source, 0);
        else {
            bool hasSpare = false;
            for (std::uint32_t index = 0; index < existing->placements.count; ++index)
                if (!existing->placements.data[index]->placement)
                    hasSpare = true;
            if (!hasSpare)
                RegisterObjectPlacement(source, nullptr, 0);
        }
        return true;
    }
    char path[512]{'/'};
    std::uint32_t modelKind = 4;
    if (flags & 0x80U) {
        std::strcat(path, modelPath);
    } else {
        char remapped[272]{};
        modelKind = static_cast<std::uint32_t>(RemapModelPath(modelPath, remapped));
        std::strcpy(path, remapped);
    }
    FilepathCleaner(path);
    if (flags & 0x10U) {
        std::size_t end = std::strlen(path);
        const char *name = path;
        if (end > 1) {
            while (true) {
                if (path[end - 1] == '/' || path[end - 1] == '\\') {
                    name = path + end;
                    break;
                }
                if (end-- == 1 || end == 1)
                    break;
            }
        }
        char stem[64]{};
        std::sprintf(stem, "%s", name);
        if (char *const dot = std::strchr(stem, '.'))
            *dot = '\0';
        char overridePath[272]{};
        std::sprintf(overridePath, "/modelOverride/%s/%s.fbx", stem, stem);
        FilepathCleaner(overridePath);
        if (!AuthoredAssetExists(overridePath))
            return false;
#if SHENMUE_IS_S2
        std::strcpy(path, overridePath);
#else
        throw std::runtime_error("Native HD object realization is unavailable");
#endif
    }
    const std::uint32_t hash = d3t_HashData(path, static_cast<std::uint32_t>(std::strlen(path)));
    if (!(flags & 0x800U)) {
        for (std::size_t index = 0; index < sources.keys.size(); ++index) {
            ObjectSource &candidate = sources.values[index];
            if (!sources.keys[index] || candidate.pathHash != hash)
                continue;
            ++candidate.references;
            candidate.retiring = 0;
            sourceAliases.insert(FoldKey(reinterpret_cast<std::uintptr_t>(source)), candidate.key);
            RegisterObjectPlacement(source, (flags & 0x30U) == 0x20U ? nullptr : source, 0);
            return true;
        }
    }
    ObjectSource record;
    record.key = reinterpret_cast<std::uintptr_t>(source);
    record.flags = flags;
    record.resolveState = 2;
    record.modelKind = modelKind;
    record.pathHash = hash;
    record.references = 1;
    if (std::strlen(path) >= sizeof(record.path))
        throw std::length_error("Native object model path exceeds 259 bytes");
    std::strcpy(record.path, path);
    if (sources.insert(FoldKey(record.key), record) == sources.keys.size())
        return false;
#if SHENMUE_IS_S2
    if (flags & 0x10U) {
        ObjectSource *const admitted = FindSource(source);
        admitted->threadHandle = d3t_AllocateRenderHandle();
        admitted->loadHandle = DispatchModelLoad(admitted->path, admitted->threadHandle);
        admitted->resolveState = 0;
    }
#endif
    dirty = true;
    RegisterObjectPlacement(source, (flags & 0x30U) == 0x20U ? nullptr : source, 0);
    return true;
}

bool RegisterObjectPlacement(const void *sourceKey, const void *placement, std::uint8_t flags) {
    ObjectSource *const source = FindSource(sourceKey);
    if (!source)
        return false;
    if (ObjectEntry *const existing = FindPlacement(placement)) {
        if (existing->update == UpdateState::Idle)
            return false;
        if (existing->source == source) {
            existing->update = UpdateState::Idle;
            return true;
        }
        RetireEntry(*existing);
    }
    if (source->retiring)
        return false;
    ObjectEntry *entry = nullptr;
    for (std::uint32_t index = 0; index < source->placements.count; ++index) {
        if (!source->placements.data[index]->placement) {
            entry = source->placements.data[index];
            break;
        }
    }
    if (!entry) {
        if (placementIndex.count >= 1024)
            return false;
        std::uint32_t slot = nextSlot;
        while ((slotFlags[slot] >> 10) >= 0x200000U) {
            slot = (slot + 1) & 0x3ffU;
            if (slot == nextSlot)
                return false;
        }
        std::uint32_t generation = (slotFlags[slot] >> 10) + 1;
        if ((generation & 0xffe00000U) == 0x200000U)
            generation = 1;
        const std::uint32_t tag = (generation | 0xffe00000U) << 10;
        slotFlags[slot] = tag;
        nextSlot = (slot + 1) & 0x3ffU;
        entry = &entries[slot];
        *entry = {};
        entry->handle = slot | tag;
        entry->source = source;
        AppendPlacement(*source, entry);
    }
    entry->flags = flags;
    BindPlacement(*entry, placement);
    dirty = true;
    return true;
}

void RetireObjectPlacement(const void *placement) {
    ObjectEntry *const entry = FindPlacement(placement);
    if (!entry)
        return;
    if (entry->update != UpdateState::Retire)
        entry->update = UpdateState::Dirty;
    dirty = true;
}

std::uint32_t GetObjectSourceRenderHandle(const void *sourceKey) {
    const ObjectSource *const source = FindSource(sourceKey);
    return source ? source->renderHandle : 0;
}

void ReleaseObjectSource(const void *sourceKey) {
    ObjectSource *const source = FindSource(sourceKey);
    if (!source)
        return;
    if (source->references)
        --source->references;
    if (source->references) {
        sourceAliases.erase(FoldKey(reinterpret_cast<std::uintptr_t>(sourceKey)));
        return;
    }
    source->retiring = 1;
    for (std::uint32_t index = 0; index < source->placements.count; ++index)
        source->placements.data[index]->update = UpdateState::Retire;
    dirty = true;
}

// @verified - p
std::int16_t SetObjectSourceActiveFlag(const void *sourceKey, std::int32_t active) {
    ObjectSource *const source = FindSource(sourceKey);
    if (!source)
        return 0;
    if (active != 0) {
        source->flags = (source->flags & 0xfffffffeU) | 2U;
        return -2;
    }
    source->flags = (source->flags & 0xfffffffdU) | 1U;
    return std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(source->flags));
}

bool ProcessObjectStore(bool checkShaders, std::int32_t maximumOperations) {
    if (!dirty)
        return false;
    bool settled = true;
    std::int32_t operations = 0;
    for (std::size_t index = 0; index < entries.size(); ++index) {
        if (!(slotFlags[index] & 0x80000000U))
            continue;
        ObjectEntry &entry = entries[index];
        if (entry.kind == EntryKind::Pending) {
            if (maximumOperations == -1 || operations < maximumOperations) {
                if (!entry.source || entry.source->resolveState == 2)
                    entry.kind = EntryKind::Legacy;
                else if (entry.source->resolveState == 1) {
#if SHENMUE_IS_S2
                    const std::uint32_t cache = (entry.source->flags & 0x10U) ? s2::g_sdOverrideModelCache :
                        (entry.flags & 2U)                                    ? s2::g_skyModelCache :
                        (entry.flags & 4U)                                    ? s2::g_skyProbeModelCache :
                                                                                s2::g_mainModelCache;
                    entry.expressionTarget = AppendModelToCache(cache, entry.source->renderHandle);
                    if (entry.expressionTarget) {
                        entry.kind = EntryKind::ModelRuntime;
                        if (entry.source->flags & 2U)
                            UpdateModelCacheFlags(entry.expressionTarget, 2, ModelCacheFlagMode::Enable);
                        if (entry.source->modelKind == 1)
                            UpdateModelCacheFlags(entry.expressionTarget, 16, ModelCacheFlagMode::Enable);
                        if (entry.source->flags & 0x1000U)
                            UpdateModelCacheFlags(entry.expressionTarget, 256, ModelCacheFlagMode::Enable);
                        BuildModelExpressionRig(entry);

                        entry.modelRuntime.cacheHandle = static_cast<std::uint32_t>(entry.expressionTarget);
                        entry.modelRuntime.rootHash = static_cast<std::uint32_t>(entry.expressionTarget >> 32);
                        const char *const name = std::strrchr(entry.source->path, '/');
                        std::strcpy(entry.modelRuntime.modelName, name ? name + 1 : entry.source->path);
                        if (char *const dot = std::strchr(entry.modelRuntime.modelName, '.'))
                            *dot = '\0';
                        if (entry.placement) {
                            MarkPlacementDayNight(entry.expressionTarget);
                            RegisterPlacementLights(entry);
                        }
                        if (entry.placement && entry.source->modelKind == 1)
                            InitializeObjectStoreExpressions(entry);
                        UpdateModelCacheFlags(entry.expressionTarget, 1, entry.placement ? ModelCacheFlagMode::Clear : ModelCacheFlagMode::Enable);
                        ++operations;
                    } else {
                        entry.kind = EntryKind::Legacy;
                    }
#else
                    throw std::runtime_error("Native HD placement realization 35BFB0/3A7450 is unavailable");
#endif
                }
            }
            settled &= entry.kind != EntryKind::Pending;
        }
        if (entry.update != UpdateState::Idle) {
            if ((maximumOperations == -1 || operations < maximumOperations) && entry.kind != EntryKind::Pending) {
                RetireEntry(entry);
                ++operations;
            }
            settled &= entry.update == UpdateState::Idle;
        }
    }
    for (std::size_t index = 0; index < sources.keys.size(); ++index) {
#if SHENMUE_IS_S2
        if (maximumOperations != -1 && operations >= maximumOperations)
            break;
#endif
        if (!sources.keys[index])
            continue;
        ObjectSource &source = sources.values[index];
#if SHENMUE_IS_S2
        if (source.resolveState == 0) {
            if (maximumOperations == -1 || operations < maximumOperations) {
                ResolveSource(source, checkShaders);
                if (source.resolveState == 1)
                    ++operations;
                    }

            settled &= source.resolveState != 0;
        }
#endif
        if (!source.retiring)
            continue;
        if (maximumOperations != -1 && operations >= maximumOperations) {
            settled = false;
            continue;
        }

#if SHENMUE_IS_S2
        if (source.resolveState == 0) {
            settled = false;
            continue;
        }

#endif
        if (RetireSource(source))
            ++operations;
        settled &= source.retiring == 0;
    }
    dirty = !settled;
    return dirty;
}

void ResetObjectStoreForMap() {
    for (std::size_t index = 0; index < sources.keys.size(); ++index) {
        if (!sources.keys[index])
            continue;
        ObjectSource &source = sources.values[index];
        source.references = 1;
        if (!(source.flags & 0x40U)) {
            const void *const key = reinterpret_cast<const void *>(source.key);
            ReleaseObjectSource(key);
            ReleaseObjectSource(key);
        } else {
            for (std::uint32_t placement = 0; placement < source.placements.count; ++placement)
                if (source.placements.data[placement])
                    source.placements.data[placement]->update = UpdateState::Retire;
        }
    }
#if SHENMUE_IS_S2
    const std::uint32_t previousResetWord = objectStoreResetWord;
    objectStoreResetWord = 0;
#endif
    dirty = true;
    while (ProcessObjectStore(false, -1)) {
#if SHENMUE_IS_S2
        ProcessRemappings();
#endif
    }
#if SHENMUE_IS_S2
    objectStoreResetWord = previousResetWord;
#endif
    for (std::size_t index = 0; index < sources.keys.size(); ++index)
        if (sources.keys[index] && !(sources.values[index].flags & 0x40U))
            sources.erase(sources.keys[index]);
    for (std::size_t index = 0; index < publicAliases.keys.size(); ++index) {
        publicAliases.values[index] = 0;
        publicAliases.keys[index] = 0;
        publicAliases.probes[index] = 0;
    }
    publicAliases.count = 0;
    for (std::size_t index = 0; index < sourceAliases.keys.size(); ++index) {
        sourceAliases.values[index] = 0;
        sourceAliases.keys[index] = 0;
        sourceAliases.probes[index] = 0;
    }
    sourceAliases.count = 0;
}

// @verified - p
bool HashModelWithAlpha(const void *placement, float alpha, std::uint32_t projectionMode) {
    return HashOrQueueModelInstance(placement, &alpha, projectionMode);
}

// @verified - p
bool HashModel(const void *placement, std::uint32_t projectionMode) {
    return HashOrQueueModelInstance(placement, nullptr, projectionMode);
}

bool FindHashedObject(const void *placement, const Vector3 *joins, std::uint32_t projectionMode) {
    return HashOrQueueModelInstance(placement, nullptr, projectionMode, joins);
}

void SetHashedObjectTransform(const void *placement) {
    ObjectEntry *const entry = ResolveHashedObjectEntry(placement);
    if (!entry || entry->kind != EntryKind::ModelRuntime || (entry->source->flags & 0x2002U) == 0)
        return;
    *matrix_stack::g_matrixStack.top.stack = matrix_stack::workTransformMatrix;
    Matrix4x4 transform = matrix_stack::workTransformMatrix;
    for (std::uint32_t row = 0; row < 4; ++row) {
        for (std::uint32_t column = row + 1; column < 4; ++column) {
            const float value = transform.rows[row][column];
            transform.rows[row][column] = transform.rows[column][row];
            transform.rows[column][row] = value;
        }
    }
    SetModelCacheTransform(entry->expressionTarget, transform);
}

} // namespace shenmue::d3t
