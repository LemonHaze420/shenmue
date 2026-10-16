#include "s1/task/pack.hpp"
#include "s1/d3t/namespace.hpp"
#include "s1/boot.hpp"
#include "s1/scene_memory.hpp"

#include "s1/archive_handles.hpp"
#include "s1/chrd.hpp"
#include "s1/collision.hpp"
#include "s1/dynamic_map.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/scrl.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/motion_registry.hpp"
#include "s1/render_array.hpp"
#include "s1/scene.hpp"
#include "s1/texture_cache.hpp"
#include "s1/texture_loader.hpp"
#include "s1/timing.hpp"

#include <array>
#include <atomic>
#include <bit>
#include <cctype>
#include "s1/task/mdlm.hpp"
#include "s1/archive_data.hpp"
#include "s1/d3t/d3t_asset_remap.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/mip_streaming.hpp"
#include "s1/shader_initialization.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>

namespace shenmue::s1 {

PackAdfsWrapper *ReadADFS_Wrapper(char *directory, const char *filename, std::int32_t timeout);
void decompress(void *source, void *destination, std::int64_t reserved0, std::int64_t reserved1, std::uint32_t decompressedSize, std::int64_t reserved2,
    std::uint32_t compressedSize);
PackModel *PAKF_Mallocs(PakChunkHeader *chunk, std::int32_t *textureIds, void *keepRenderEnabled, char *entityName, char *formattedPath);
void Shenmue_FileRead_ALL(std::int64_t model, char *formattedPath, std::uint32_t readMode, std::int16_t flags);
void HMDL_NormalizeNodeCommandHeaders(void *model);
void *HMDL_AllocateAttachmentTables(void *model);
void LoadSCRL(void *data, char *chunkName);
char *LoadSCR(void *data, char *chunkName, std::int32_t slot);
void HMDL_UnlinkSceneEntriesRecursive(void *modelTree);
void FreePVR(void *model);
void FreeClip_PREP(std::int32_t mapIndex);
void PACK_ReleaseSlotCharacters(PackSlot *slot);
std::int64_t PACK_FreeOwner(PackOwner *owner);

namespace {

constexpr std::uint32_t kPakfMagic = SHEN_NAME(PAKF);
constexpr std::uint32_t kPaksMagic = SHEN_NAME(PAKS);
constexpr std::uint32_t kPackAllocationToken = 0x004b4150;
constexpr std::uint32_t kPaksAllocationToken = SHEN_NAME(PAKS);
constexpr std::uint32_t kChrtAllocationToken = SHEN_NAME(PAKC);
constexpr std::uint32_t kTextureAllocationToken = SHEN_NAME(PAKT);

struct HmdlTransform {
    std::byte reserved_00[8];
    Vector3 rotation;
    Vector3 scale;
    Vector3 position;
};

#pragma pack(push, 1)
struct HmdlNode {
    std::uint16_t nodeKind;
    std::uint16_t nodeFlags;
    std::int32_t sceneBindingOffset;
    Vector3i rotation;
    Vector3 scale;
    Vector3 position;
    std::int32_t childOffset;
    std::int32_t siblingOffset;
    std::int32_t parentOffset;
    std::int32_t transformOffset;
    std::int32_t attachmentOffset;
};

struct HmdlSceneBinding {
    std::uint32_t flags;
    std::uint32_t vertexDataOffset;
    std::uint32_t attachmentCount;
    std::uint32_t commandStreamOffset;
};
#pragma pack(pop)

static_assert(offsetof(HmdlTransform, rotation) == 8);
static_assert(offsetof(HmdlTransform, scale) == 0x14);
static_assert(offsetof(HmdlTransform, position) == 0x20);
static_assert(sizeof(HmdlNode) == 0x40);
static_assert(sizeof(HmdlSceneBinding) == 0x10);

char g_packExtensionFilename[0x80]{};
std::int32_t g_chunkScanIndex = 0;
std::uint32_t g_chunkScanTag = 0;
std::array<std::int32_t, 32> g_packResultCapacities{};
char g_chunkName[9]{};

void PACK_RegisterDynamicMap(std::int32_t mapIndex, PackModel *model);

HmdlNode *HMDL_NodeFromOffset(std::byte *modelBase, std::int32_t offset) {
    return offset ? reinterpret_cast<HmdlNode *>(modelBase + offset) : nullptr;
}

HmdlSceneBinding *HMDL_Binding(std::byte *modelBase, HmdlNode *node) {
    return node->sceneBindingOffset ? reinterpret_cast<HmdlSceneBinding *>(modelBase + node->sceneBindingOffset) : nullptr;
}

HmdlNode *HMDL_NodeFromOffset(std::int32_t offset) {
    return HMDL_NodeFromOffset(g_pCurrentTaskData, offset);
}

HmdlSceneBinding *HMDL_Binding(HmdlNode *node) {
    return HMDL_Binding(g_pCurrentTaskData, node);
}

void HMDL_NormalizeNodeCommandHeadersRecursive(std::byte *modelBase, HmdlNode *node) {
    do {
        if (HmdlSceneBinding *const binding = HMDL_Binding(modelBase, node)) {
            HMDL_NormalizeCommandHeader(binding, modelBase);
        }
        if (HmdlNode *const child = HMDL_NodeFromOffset(modelBase, node->childOffset)) {
            HMDL_NormalizeNodeCommandHeadersRecursive(modelBase, child);
        }
        node = HMDL_NodeFromOffset(modelBase, node->siblingOffset);
    } while (node);
}

std::size_t HMDL_CountAttachmentBytesRecursive(HmdlNode *node) {
    std::size_t size = 0;
    do {
        if (HmdlSceneBinding *const binding = HMDL_Binding(node)) {
            size += sizeof(std::uint32_t) * binding->attachmentCount;
        }
        if (HmdlNode *const child = HMDL_NodeFromOffset(node->childOffset)) {
            size += HMDL_CountAttachmentBytesRecursive(child);
        }
        node = HMDL_NodeFromOffset(node->siblingOffset);
    } while (node);
    return size;
}

std::byte *HMDL_AssignAttachmentTablesRecursive(HmdlNode *node, std::byte *output) {
    do {
        if (HmdlSceneBinding *const binding = HMDL_Binding(node)) {
            node->attachmentOffset = output ? static_cast<std::int32_t>(output - g_pCurrentTaskData) : 0;
            output += sizeof(std::uint32_t) * binding->attachmentCount;
        }
        if (HmdlNode *const child = HMDL_NodeFromOffset(node->childOffset)) {
            output = HMDL_AssignAttachmentTablesRecursive(child, output);
        }
        node = HMDL_NodeFromOffset(node->siblingOffset);
    } while (node);
    return output;
}

PakDirectoryEntry *PACK_GetDirectoryEntries(PakDirectory *directory) {
    return reinterpret_cast<PakDirectoryEntry *>(reinterpret_cast<std::byte *>(directory) + directory->entriesOffset);
}

void *PACK_GetDirectoryEntryData(PakDirectory *directory, const PakDirectoryEntry &entry) {
    return reinterpret_cast<std::byte *>(directory) + entry.dataOffset;
}

void PACK_CopyChunkName(const PakDirectoryEntry &entry) {
    std::strncpy(g_chunkName, entry.name, 8);
}

std::int32_t PACK_FindNextChunk(PakDirectory *directory, std::uint32_t tag) {
    std::int32_t index = 0;
    if (tag) {
        g_chunkScanTag = tag;
        g_chunkScanIndex = 0;
    } else {
        index = g_chunkScanIndex;
        tag = g_chunkScanTag;
    }
    if (index >= directory->entryCount) {
        return -1;
    }
    PakDirectoryEntry *const entries = PACK_GetDirectoryEntries(directory);
    while (entries[index].tag != tag && tag != 0x2a2a2a2a) {
        g_chunkScanIndex = ++index;
        if (index >= directory->entryCount) {
            return -1;
        }
    }
    g_chunkScanIndex = index + 1;
    return index;
}

std::int32_t PACK_FindFreeDynamicMapSlot() {
    for (std::int32_t index = 0; index < 32; ++index) {
        if (!g_dynamicMapSlots[index].mapAsset) {
            return index;
        }
    }
    return -1;
}

void PACK_InsertMapIndex(PackSlot *slot, std::int32_t mapIndex) {
    for (std::int32_t index = 0; index < 32; ++index) {
        if (slot->mapIndices[index] == -1) {
            slot->mapIndices[index] = mapIndex;
            return;
        }
    }
}

PackModel *PACK_ReadModel(PackTaskData *pack, PackSlot *slot, PakChunkHeader *chunk, bool keepRenderEnabled, char *formattedPath) {
    PackModel *const model =
        PAKF_Mallocs(chunk, slot->textureIds, reinterpret_cast<void *>(static_cast<std::intptr_t>(keepRenderEnabled ? 1 : 0)), slot->name, formattedPath);
    Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), formattedPath, pack->renderHandleOrReadMode, 1);
    if ((slot->flags & PackSlotSpecialAllocation) != 0)
        reinterpret_cast<std::uint32_t *>(model)[-7] |= 1;
    const std::int32_t mapIndex = PACK_FindFreeDynamicMapSlot();
    PACK_RegisterDynamicMap(mapIndex, model);
    g_dynamicMapSlots[mapIndex].sourceKind = 0;
    if (!keepRenderEnabled)
        g_dynamicMapSlots[mapIndex].renderEnabled = 0;
    PACK_InsertMapIndex(slot, mapIndex);
    return model;
}

void PACK_RegisterDynamicMap(std::int32_t mapIndex, PackModel *model) {
    DynamicMapEntry &entry = g_dynamicMapSlots[mapIndex];
    PackModel *mapAsset = entry.mapAsset;
    if (mapAsset && mapAsset->model) {
        return;
    }
    if (mapIndex < 32) {
        entry.mapAsset = model;
        mapAsset = model;
    }
    if (model->model) {
        BuildMapPackClipRuntimeTables(model->model, mapIndex);
        HMDL_NormalizeNodeCommandHeaders(model->model);
        if (!entry.prepClone) {
            entry.prepClone = HMDL_AllocateAttachmentTables(model->model);
        }
        auto *const transform = static_cast<HmdlTransform *>(model->model);
        transform->position[0] = 0.0f;
        transform->position[1] = 0.0f;
        transform->position[2] = 0.0f;
        transform->rotation[0] = 0.0f;
        transform->rotation[1] = 0.0f;
        transform->rotation[2] = 0.0f;
        transform->scale[0] = 1.0f;
        transform->scale[1] = 1.0f;
        transform->scale[2] = 1.0f;
    }
    entry.prepReady = 0;
}

bool PACK_IsRenderHandleValid(std::uint32_t handle) {
    return d3t_IsRenderHandlePending(handle);
}

bool PACK_IsOpenFileTableEntryActive(void *file) {
    if (!file)
        return false;
    const auto *const openFile = static_cast<const PackOpenFile *>(file);
    return Archive_HasPendingRead(openFile->fileId);
}

void PACK_YieldToInitCallback() {
    if (!g_packTask) {
        return;
    }
    if (!g_packTask->initCallbackFnPtr) {
        g_packTask->initCallbackFnPtr = g_packTask->mainTaskCallbackFnPtr;
        g_packTask->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
        g_packTask->mainExecutionIterations = 0;
    }
    g_packTask->mainExecutionIterations = 0;
}

std::int64_t PACK_GetSlotIndex(const PackSlot *slot) {
    for (std::int64_t index = 0; index < 32; ++index) {
        if (&g_packSlots[index] == slot) {
            return index;
        }
    }
    return -1;
}

void PACK_CloseSlotFile(std::uint64_t pakType, PackSlot *slot, bool freePackData) {
    if ((slot->flags & PackSlotAsyncCachedRead) != 0) {
        if (slot->asyncFile) {
            ReadCachedFile(slot->asyncFile);
            slot->asyncFile = nullptr;
        }
    } else if (slot->asyncFile) {
        auto *const file = static_cast<PackOpenFile *>(slot->asyncFile);
        ReadFile_0(pakType, &file->fileId);
        slot->asyncFile = nullptr;
    }

    if (freePackData) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(slot->pakData), 0);
        slot->pakData = nullptr;
    }
}

void PACK_FreeMapStorage(PackSlot *slot) {
    if (slot->pakfResults) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(slot->pakfResults), 0);
        slot->pakfResults = nullptr;
    }
    if (slot->mapIndices) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(slot->mapIndices), 0);
        slot->mapIndices = nullptr;
    }
    if (slot->motionHeaders) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(slot->motionHeaders), 0);
        slot->motionHeaders = nullptr;
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(slot->pakData), 0);
    slot->pakData = nullptr;
    slot->tag = 0;
}

} // namespace

void HMDL_NormalizeCommandHeader(void *rawBinding, std::byte *modelBase) {
    const HmdlSceneBinding *const binding = static_cast<const HmdlSceneBinding *>(rawBinding);
    if (!binding || !modelBase || !binding->commandStreamOffset)
        return;
    std::uint16_t *command = reinterpret_cast<std::uint16_t *>(modelBase + static_cast<std::int32_t>(binding->commandStreamOffset));
    std::int16_t opcode = static_cast<std::int16_t>(*command++);
    while (opcode >= 0) {
        const std::uint8_t kind = static_cast<std::uint8_t>(opcode);




        if (kind == 0) {
            // No payload.
        } else if (kind >= 2 && kind <= 7) {
            const std::uint16_t payloadBytes = *command++;
            std::uint32_t *const flags = reinterpret_cast<std::uint32_t *>(command);
            command = reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(command) + payloadBytes);
            if ((*flags & 0xc0) != 0xc0) {
                *flags = (*flags & 0xffffff3f) | 0x80;
            }
        } else if (kind >= 8 && kind <= 11) {
            ++command;
        } else if ((kind >= 12 && kind <= 21) || (kind >= 24 && kind <= 29)) {
            const std::uint16_t payloadBytes = *command++;
            command = reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(command) + payloadBytes);
        } else {


            return;
        }
        opcode = static_cast<std::int16_t>(*command++);
    }
}

bool PACK_IsValidSlot(PackSlot *slot) {
    const auto address = reinterpret_cast<std::uintptr_t>(slot);
    if (address < reinterpret_cast<std::uintptr_t>(&g_packSlots[0]) || address > reinterpret_cast<std::uintptr_t>(&g_packSlots[31])) {
        return false;
    }
    const std::int64_t slotIndex = PACK_GetSlotIndex(slot);
    if (slotIndex < 0) {
        return false;
    }
    const std::uint32_t expectedTag = ((static_cast<std::uint32_t>(slotIndex) + 97) << 24) | 0x4d5048;
    if (slot->tag != expectedTag) {
        return false;
    }
    PackTaskData *pack = g_packTaskData;
    if (!pack) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    return slot != pack->loadingSlot && slot != pack->pendingPakf && slot != pack->activeReader;
}



void LoadAlternateMapModels(const char *rootDirectory, const std::array<std::int32_t, 32> &mapIds) {
    for (std::int32_t mapIndex = 0; mapIndex < 32 && mapIds[mapIndex] != -1; ++mapIndex) {
        const std::int32_t mapId = mapIds[mapIndex];
        char filename[64];
        char assetPath[272];
        if (mapId) {
            std::snprintf(filename, sizeof(filename), "MAP%02d.MT5", mapId);
            std::snprintf(assetPath, 260, "/%s/map%02d.xxx_/map%02d.fbx", rootDirectory, mapId, mapId);
        } else {
            std::snprintf(filename, sizeof(filename), "MAP.MT5");
            std::snprintf(assetPath, 260, "/%s/map.xxx_/map.fbx", rootDirectory);
        }
        void *const file = ReadFile(rootDirectory, filename, 0x2050414d);
        PackModel *const model = MT5_InitRawModelAndTextures(file, filename, assetPath);
        if (!model) {
            if (file)
                HLib_FreeTaskData(static_cast<HLib::TaskData *>(file), 0);
            continue;
        }
        Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), assetPath, 0, 0);
        PACK_RegisterDynamicMap(mapIndex, model);
        g_dynamicMapSlots[mapIndex].sourceKind = 1;
    }
}



void LoadMapModels(std::int32_t totalMapPacks, const char *rootDirectory) {
    void *const file = ReadFile(rootDirectory, "MAP.MT5", 0x2050414d);
    if (!file)
        return;
    char assetPath[272];
    std::snprintf(assetPath, 260, "%s/map.xxx_/map.fbx", rootDirectory);
    PackModel *const model = MT5_InitRawModelAndTextures(file, "MAP.MT5", assetPath);
    if (!model) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(file), 0);
        return;
    }
    Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), assetPath, 0, 0);
    PACK_RegisterDynamicMap(0, model);
    g_dynamicMapSlots[0].sourceKind = 1;
    for (std::int32_t mapIndex = 1; mapIndex <= totalMapPacks; ++mapIndex) {
        char filename[64];
        std::snprintf(filename, sizeof(filename), "MAP%02d.MT5", mapIndex);
        void *const mapFile = ReadFile(rootDirectory, filename, 0x2050414d);
        std::snprintf(assetPath, 260, "/%s/map%02d.xxx_/map%02d.fbx", rootDirectory, mapIndex, mapIndex);
        PackModel *const mapModel = MT5_InitRawModelAndTextures(mapFile, filename, assetPath);
        if (!mapModel) {
            if (mapFile)
                HLib_FreeTaskData(static_cast<HLib::TaskData *>(mapFile), 0);
            continue;
        }
        Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(mapModel->model), assetPath, 0, 0);
        PACK_RegisterDynamicMap(mapIndex, mapModel);
        g_dynamicMapSlots[mapIndex].sourceKind = 1;
    }
}

void ReadMAPR(const char *rootDirectory, const char *filename, std::int32_t mapIndex) {
    const PackModel *const mapAsset = g_dynamicMapSlots[mapIndex].mapAsset;
    if (mapAsset && mapAsset->model)
        return;
    void *const file = ReadFile(rootDirectory, filename, SHEN_NAME(MAPR));
    PackModel *const model = MT5_InitRawModelAndTextures(file, filename, nullptr);
    if (!model) {
        if (file)
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(file), 0);
        return;
    }
    PACK_RegisterDynamicMap(mapIndex, model);
    g_dynamicMapSlots[mapIndex].sourceKind = 1;
}

void HMDL_NormalizeNodeCommandHeaders(void *model) {
    if (!model)
        return;
    std::byte *const modelBase = MT5_ModelBaseForNode(model);
    HMDL_NormalizeNodeCommandHeadersRecursive(modelBase, static_cast<HmdlNode *>(model));
}

void *HMDL_AllocateAttachmentTables(void *model) {
    auto *const node = static_cast<HmdlNode *>(model);
    const std::size_t size = HMDL_CountAttachmentBytesRecursive(node);
    constexpr std::int32_t kPrepareToken = SHEN_NAME(PRER);
    auto *const allocation = static_cast<std::byte *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(size), kPrepareToken));
    if (allocation) {
        HMDL_AssignAttachmentTablesRecursive(node, allocation);
    }
    return allocation;
}

void PACK_EnsureMapPreparationStorage(std::int32_t mapIndex) {
    if (mapIndex < 0 || mapIndex >= 32)
        return;
    DynamicMapEntry &entry = g_dynamicMapSlots[mapIndex];
    if (!entry.mapAsset || !entry.mapAsset->model || entry.prepClone)
        return;
    entry.prepClone = HMDL_AllocateAttachmentTables(entry.mapAsset->model);
}

void PACK_EnsureAllMapPreparationStorage() {
    for (std::int32_t mapIndex = 0; mapIndex < 32; ++mapIndex) {
        PACK_EnsureMapPreparationStorage(mapIndex);
    }
}

void HMDL_UnlinkSceneEntriesRecursive(void *modelTree) {
    auto *node = static_cast<HmdlNode *>(modelTree);
    do {
        std::byte *const modelBase = MT5_ModelBaseForNode(node);
        if (auto *const binding = static_cast<HmdlSceneBinding *>(MT5_SceneBindingForNode(node, nullptr))) {
            const std::int32_t sceneKey = static_cast<std::int32_t>((reinterpret_cast<std::uintptr_t>(binding) + 4) % 0xffffffffULL);
            MT5_RemoveSceneModel(sceneKey);
            EFFE_RemoveQueuedSceneObjectReferences(binding);
        }
        if (HmdlNode *const child = HMDL_NodeFromOffset(modelBase, node->childOffset)) {
            HMDL_UnlinkSceneEntriesRecursive(child);
        }
        node = HMDL_NodeFromOffset(modelBase, node->siblingOffset);
    } while (node);
}

void FreePVR(void *modelPointer) {
    auto *const model = static_cast<PackModel *>(modelPointer);
    if (!model)
        return;
    std::int32_t allocationToken = 0;
    if (model && (reinterpret_cast<std::uintptr_t>(model) & 0x1f) == 0) {
        auto *const header = reinterpret_cast<HLib::TaskData *>(model) - 1;
        if (header->status == 0x20455355) {
            allocationToken = header->taskToken;
        }
    }

    const std::uintptr_t hmdl = reinterpret_cast<std::uintptr_t>(model->model);
    ReleaseHmdlRuntimeObject(hmdl);
    ReleaseHmdlRuntimeObject(hmdl + 1);
    HMDL_UnlinkSceneEntriesRecursive(model->model);
    if (!allocationToken) {
        return;
    }
    MT5_UnregisterModelAllocation(model->pakf);
    MT5_UnregisterModelAllocation(model);

    auto *texture = reinterpret_cast<std::int32_t *>(model + 1);
    for (std::uint32_t index = 0; index < model->textureCount; ++index) {
        PVR_FreeAllocatedSurfaceByIndex(*texture++);
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(model->pakf), 0);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(model), 0);
}

void FreeClip_PREP(std::int32_t mapIndex) {
    if (mapIndex >= 32) {
        return;
    }
    DynamicMapEntry &entry = g_dynamicMapSlots[mapIndex];
    if (!entry.mapAsset) {
        return;
    }
    if (entry.sourceKind) {
        FreePVR(entry.mapAsset);
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(entry.clipRuntime), SHEN_NAME(CLIP));
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(entry.clipCellPointers), SHEN_NAME(ACPH));
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(entry.sceneObjectData), SHEN_NAME(ACPD));
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(entry.prepClone), SHEN_NAME(PRER));

    DynamicMapAuxiliary &auxiliary = g_dynamicMapAuxiliary[mapIndex];
    if (auxiliary.clipCells) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(auxiliary.clipCells), SHEN_NAME(DANG));
    }
    if (auxiliary.pathData) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(auxiliary.pathData), SHEN_NAME(DAPH));
    }

    entry.renderEnabled = 1;
    auxiliary.clipCells = nullptr;
    auxiliary.clipCellCount = 0;
    auxiliary.clipCellsReady = 0;
    auxiliary.pathData = nullptr;
    entry.clipRuntime = nullptr;
    entry.mapAsset = nullptr;
    entry.sceneObjectCount = 0;
    entry.clipCellPointers = nullptr;
    entry.sceneObjectData = nullptr;
    entry.position[0] = 0.0f;
    entry.position[1] = 0.0f;
    entry.position[2] = 0.0f;
    entry.field_44 = 0;
    entry.sourceKind = 0;
    entry.staticSceneMode = 1;
    entry.occlusionEnabled = 1;
    entry.clipMetrics[0] = 0;
    entry.clipMetrics[1] = 0;
    entry.clipMetrics[2] = 0;
    entry.clipMetrics[3] = 0;
    entry.clipMetrics[5] = 0;
    entry.prepClone = nullptr;
    entry.scale = 1.0f;
    entry.lightingOverride01 = 0.0f;
}

PackTaskData *g_packTaskData = nullptr;
HLib::Task *g_packTask = nullptr;
PackOwner g_packOwners[8]{};
PackSlot g_packSlots[32]{};

void *PACK_GetReadyCollisionData() {
    PackTaskData *pack = g_packTaskData;
    if (!pack && g_packTask)
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    return pack && PACK_IsValidSlot(pack->readyPack) ? pack->readyPack->collisionData : nullptr;
}

const std::byte *PACK_GetCurrentDYNAChunk() {
    PackTaskData *pack = g_packTaskData;
    if (!pack) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }

    PackSlot *const ready = pack->readyPack;
    if (ready && pack->usePackCache) {
        const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(ready);
        if (address >= reinterpret_cast<std::uintptr_t>(&g_packSlots[0]) && address <= reinterpret_cast<std::uintptr_t>(&g_packSlots[31])) {
            const std::int32_t slotIndex = static_cast<std::int32_t>(PACK_GetSlotIndex(ready));
            const std::uint32_t expectedTag = ((static_cast<std::uint32_t>(slotIndex) + 97U) << 24U) | 0x4d5048U;
            if (ready->tag == expectedTag && ready->pakData)
                return static_cast<const std::byte *>(ready->dynamicMapData);
        }
        return nullptr;
    }

    const std::byte *chunk = nullptr;
    if (g_mapControlTask && g_mapControlTask->task_data_ptr) {
        chunk = static_cast<const std::byte *>(reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr)->mapInfo);
    }
    constexpr std::uint32_t kDyna = SHEN_NAME(DYNA);
    while (*reinterpret_cast<const std::uint32_t *>(chunk) != kDyna) {
        const std::uint32_t offset = *reinterpret_cast<const std::uint32_t *>(chunk + 4);
        if (!offset)
            return nullptr;
        chunk += offset;
    }
    return chunk;
}
std::uint64_t g_currentPak[3]{};

std::int64_t PACK_FreeOwner(PackOwner *owner) {
    const auto address = reinterpret_cast<std::uintptr_t>(owner);
    if (address < reinterpret_cast<std::uintptr_t>(&g_packOwners[0]) || address > reinterpret_cast<std::uintptr_t>(&g_packOwners[7])) {
        return 0;
    }
    PackAdfsWrapper *const wrapper = owner->adfsWrapper;
    if (!wrapper)
        return 0;
    if (--owner->referenceCount <= 0) {
        HLib_FreeTaskData(wrapper->taskData, 0);
        wrapper->status = -1;
    }
    owner->adfsWrapper = nullptr;
    owner->directory[0] = 0;
    owner->filename[0] = 0;
    owner->referenceCount = 0;
    return 0;
}

PackTaskData *ParseCHID(void *data, std::int32_t allocationToken) {
    PackTaskData *pack = g_packTaskData;
    if (!pack) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    const std::uint32_t previousUsePackCache = pack->usePackCache;
    pack->usePackCache = 1;
    pack->chidParseGuard = 1;
    CHRD_Init(data, g_entryPoint, allocationToken);
    pack->chidParseGuard = 0;
    PackTaskData *result = g_packTaskData;
    if (!result) {
        result = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    result->usePackCache = previousUsePackCache;
    return result;
}

void MAPMLoad(PackSlot *slot) {
    PakFileHeader *const pak = slot->pakData;
    auto *const directory = reinterpret_cast<PakDirectory *>(reinterpret_cast<std::byte *>(pak) + pak->directoryOffset);

    if (slot->pakfResults) {
        for (std::int32_t index = static_cast<std::int32_t>(pak->count) - 1; index >= 0; --index) {
            const std::uintptr_t model = reinterpret_cast<std::uintptr_t>(slot->pakfResults[index]->model);
            ReleaseHmdlRuntimeObject(model);
            ReleaseHmdlRuntimeObject(model + 1);
        }
    }

    std::int32_t modelIndex = 0;
    std::int32_t motionIndex = 0;
    PakDirectoryEntry *const entries = PACK_GetDirectoryEntries(directory);
    for (std::int32_t index = 0; index < directory->entryCount; ++index) {
        PackModel *model = nullptr;
        switch (entries[index].tag) {
        case SHEN_NAME(MAPD):
        case SHEN_NAME(MAPM): {
            model = slot->pakfResults[modelIndex];
            std::int32_t mapIndex = 0;
            while (mapIndex < 32 && g_dynamicMapSlots[mapIndex].mapAsset != model) {
                ++mapIndex;
            }
            if (mapIndex == 32) {
                mapIndex = -1;
            }
            FreeClip_PREP(mapIndex);
            break;
        }
        case SHEN_NAME(COLI):
            COLS_Free();
            continue;
        case SHEN_NAME(CHRM):
            model = slot->pakfResults[modelIndex];
            HMDL_UnlinkSceneEntriesRecursive(model->model);
            break;
        case SHEN_NAME(MOTN):
            AllocateUNFO_2(slot->motionHeaders[motionIndex++]);
            continue;
        default:
            continue;
        }
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(model), 0);
        ++modelIndex;
    }
    if ((slot->flags & PackSlotHasChrt) != 0) {
        PACK_ReleaseSlotCharacters(slot);
    }
}

void LoadFile(PackSlot *slot) {
    if (!PACK_IsValidSlot(slot)) {
        return;
    }
    PackTaskData *pack = g_packTaskData;
    if (!pack && g_packTask) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    std::int32_t textureIndex = slot->textureIndex;
    std::int32_t *textureId = slot->textureIds;
    while (textureIndex-- > 0) {
        PVR_FreeAllocatedSurfaceByIndex(*textureId++);
    }
    if (slot->textureIds) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(slot->textureIds), 0);
        slot->textureIds = nullptr;
    }
    MAPMLoad(slot);
    PACK_FreeMapStorage(slot);
    if (pack && slot == pack->readyPack) {
        pack->readyPack = nullptr;
    }
}

namespace d3t {
std::int32_t GetFormattedAssetPath(char *output, std::uint64_t, char *rootDirectory, PackSlot *slot, char *chunkName) {
    char root[64];
    const char *const sourceRoot = rootDirectory[0] == '/' ? rootDirectory + 1 : rootDirectory;
    std::strcpy(root, sourceRoot);
    if (!root[0])
        std::strcpy(root, "Scene/01");

    char packName[64];
    std::strcpy(packName, slot ? slot->name : "");

    char *const afs = std::strstr(packName, "afs");
    if (afs) {
        if (std::strncmp(packName, "HUMANS.afs", 10) == 0 && std::strlen(root) > 8 && root[8] == '/') {
            std::strcpy(root + 9, "stream");
        }
        char number[4]{};
        number[0] = afs[4];
        number[1] = afs[5];
        number[2] = afs[6];
        const std::int32_t archiveIndex = std::atoi(number);
        std::sprintf(afs + 3, "/afs%05u_", static_cast<std::uint32_t>(archiveIndex));
    }
    return std::sprintf(output, "/%s/%s/%s.XXX_/%s.fbx", root, packName, chunkName, chunkName);
}
} // namespace d3t

namespace {


float PackBudgetTime(float elapsed) {
    return static_cast<float>(static_cast<std::uint32_t>(static_cast<std::int64_t>(elapsed)));
}


bool PackCallbackBudgetExceeded(float start, std::int32_t budget) {
    if (g_packBudgetIgnored)
        return false;
    const float current = PackBudgetTime(g_LastFrameTime - QueryFrameTime());
    return static_cast<std::int32_t>(static_cast<double>(start - current) / 12.5) > budget && budget != -1;
}

} // namespace

bool g_packBudgetIgnored = false;

std::int32_t AllocatePAKT(PackTaskData *pack, PackSlot *slot) {
    const std::int32_t textureCount = static_cast<std::int32_t>(slot->pakData->count);
    if (!textureCount) {
        return 1;
    }

    std::int32_t textureIndex = 0;
    PakChunkHeader *texture = nullptr;
    if (slot->textureIds) {
        textureIndex = slot->textureIndex;
        texture = slot->textureIterator;
    } else {
        slot->textureIds = static_cast<std::int32_t *>(HLib_AllocMemBlock(static_cast<std::uint64_t>(4 * (textureCount + 1)), kTextureAllocationToken));
        if (!slot->textureIds) {
            return 1;
        }
        texture =
            reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(slot->pakData) + 0x10 + reinterpret_cast<std::uintptr_t>(slot->textureIterator));
        if (texture->tag == SHEN_NAME(DUMY)) {
            texture = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(texture) + texture->size);
        }
    }

    const float startTime = static_cast<float>(static_cast<std::uint32_t>(FrameTimeUtil(TimeMode::ReturnNewTime)));
    std::int32_t textureBank = g_texnTextureCacheBank[0];
    if ((slot->flags & PackSlotSpecialAllocation) != 0) {
        textureBank = 2;
    }
    g_texnTextureCacheBank[0] = textureBank;

    while (textureIndex < textureCount) {
        if (PackCallbackBudgetExceeded(startTime, pack->callbackBudget)) {
            break;
        }
        slot->textureIds[textureIndex++] = LoadTEXN(texture);
        texture = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(texture) + texture->size);
    }

    textureBank = g_texnTextureCacheBank[0];
    if ((slot->flags & PackSlotSpecialAllocation) != 0) {
        textureBank = 0;
    }
    slot->textureIndex = static_cast<std::int16_t>(textureIndex);
    g_texnTextureCacheBank[0] = textureBank;
    slot->textureIterator = texture;
    if (textureIndex >= textureCount) {
        slot->textureIds[textureCount] = -1;
        return 1;
    }
    return 0;
}

std::int32_t PKF_Reader(PackTaskData *pack, PackSlot *slot) {
    PakFileHeader *const header = slot->pakData;
    std::int32_t chunkIndex = slot->chunkIndex;
    auto *const directory = reinterpret_cast<PakDirectory *>(reinterpret_cast<std::byte *>(header) + header->directoryOffset);
    const std::int32_t entryCount = directory->entryCount;
    if (!entryCount) {
        return 1;
    }




    PakDirectoryEntry *const entries = PACK_GetDirectoryEntries(directory);
    const std::size_t slotIndex = static_cast<std::size_t>(slot - g_packSlots);
    const std::int32_t resultCapacity = slot->pakfResults ? g_packResultCapacities[slotIndex] : [entries, entryCount, header, slotIndex] {
        std::int32_t modelChunkCount = 0;
        for (std::int32_t index = 0; index < entryCount; ++index) {
            const std::uint32_t tag = entries[index].tag;
            if (tag == SHEN_NAME(MAPD) || tag == SHEN_NAME(MAPM) || tag == SHEN_NAME(CHRM))
                ++modelChunkCount;
        }
        const std::int32_t capacity = std::max<std::int32_t>(header->count, modelChunkCount);
        g_packResultCapacities[slotIndex] = capacity;
        return capacity;
    }();
    const auto storeResult = [slot, resultCapacity](std::int32_t index, PackModel *model) {
        if (slot->pakfResults && index < resultCapacity)
            slot->pakfResults[index] = model;
    };

    std::int32_t motionIndex = 0;
    std::int32_t resultIndex = 0;
    if (header->magic == kPakfMagic) {
        resultIndex = entryCount;
        motionIndex = entryCount;
    } else if (slot->pakfResults) {
        resultIndex = slot->pakfResultIndex;
        motionIndex = slot->motionIndex;
    } else {
        const std::uint64_t resultBytes = 8ULL * static_cast<std::uint64_t>(resultCapacity);
        if ((slot->flags & PackSlotSpecialAllocation) != 0) {
            slot->pakfResults = static_cast<PackModel **>(HLib_AllocMemBlock(resultBytes, SHEN_NAME(PAKM)));
        } else {
            slot->pakfResults = static_cast<PackModel **>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(resultBytes), SHEN_NAME(PAKM)));
        }
        if (!slot->pakfResults) {
            return 1;
        }

        std::int32_t mapCount = 0;
        for (std::int32_t found = PACK_FindNextChunk(directory, SHEN_NAME(MAPM)); found >= 0; found = PACK_FindNextChunk(directory, 0)) {
            ++mapCount;
        }
        for (std::int32_t found = PACK_FindNextChunk(directory, SHEN_NAME(MAPD)); found >= 0; found = PACK_FindNextChunk(directory, 0)) {
            ++mapCount;
        }
        if (mapCount) {
            slot->mapIndices = static_cast<std::int32_t *>(HLib_AllocHeapMemBlock(4LL * (mapCount + 1), SHEN_NAME(PAKM)));
            if (!slot->mapIndices) {
                return 1;
            }
            for (std::int32_t index = 0; index <= mapCount; ++index) {
                slot->mapIndices[index] = -1;
            }
        } else {
            slot->mapIndices = nullptr;
        }

        std::int32_t motionCount = 0;
        for (std::int32_t found = PACK_FindNextChunk(directory, SHEN_NAME(MOTN)); found >= 0; found = PACK_FindNextChunk(directory, 0)) {
            ++motionCount;
        }
        if (motionCount) {
            slot->motionHeaders = static_cast<std::int32_t *>(HLib_AllocHeapMemBlock(4LL * motionCount, SHEN_NAME(PAKM)));
            if (!slot->motionHeaders) {
                return 1;
            }
        } else {
            slot->motionHeaders = nullptr;
        }
        slot->motionIndex = 0;
    }

    const float frameDeltaBase = PackBudgetTime(g_LastFrameTime - QueryFrameTime());
    char formattedPath[272];

    while (chunkIndex < entryCount) {
        if (PackCallbackBudgetExceeded(frameDeltaBase, pack->callbackBudget)) {
            break;
        }

        PakDirectoryEntry &entry = entries[chunkIndex];
        void *const entryData = PACK_GetDirectoryEntryData(directory, entry);
        switch (entry.tag) {
        case SHEN_NAME(DYNM):
            if (!slot->dynamicMapData) {
                slot->dynamicMapData = entryData;
            }
            break;
        case SHEN_NAME(SCR0):
            PACK_CopyChunkName(entry);
            LoadSCR(entryData, g_chunkName, 0);
            break;
        case SHEN_NAME(SCR1):
            PACK_CopyChunkName(entry);
            LoadSCR(entryData, g_chunkName, 1);
            break;
        case SHEN_NAME(MAPD): {
            PACK_CopyChunkName(entry);
            d3t::GetFormattedAssetPath(formattedPath, 0, pack->rootDirectory, slot, g_chunkName);
            PackModel *const model = PACK_ReadModel(pack, slot, static_cast<PakChunkHeader *>(entryData), false, formattedPath);
            storeResult(resultIndex++, model);
            break;
        }
        case SHEN_NAME(COLI):
            if (!slot->collisionData) {
                COLS_Free();
                COLS_Process(entryData);
                slot->collisionData = entryData;
            }
            break;
        case SHEN_NAME(SCRL):
            PACK_CopyChunkName(entry);
            LoadSCRL(entryData, g_chunkName);
            break;
        case SHEN_NAME(MAPM): {
            PACK_CopyChunkName(entry);
            d3t::GetFormattedAssetPath(formattedPath, 0, pack->rootDirectory, slot, g_chunkName);
            PackModel *const model = PAKF_Mallocs(static_cast<PakChunkHeader *>(entryData), slot->textureIds, nullptr, slot->name, formattedPath);
            Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), formattedPath, pack->renderHandleOrReadMode, 1);
            if ((slot->flags & PackSlotSpecialAllocation) != 0) {
                reinterpret_cast<std::uint32_t *>(model)[-7] |= 1;
            }
            const std::int32_t mapIndex = PACK_FindFreeDynamicMapSlot();
            PACK_RegisterDynamicMap(mapIndex, model);
            g_dynamicMapSlots[mapIndex].sourceKind = 0;
            PACK_InsertMapIndex(slot, mapIndex);
            storeResult(resultIndex++, model);
            break;
        }
        case SHEN_NAME(CHRM): {
            PACK_CopyChunkName(entry);
            d3t::GetFormattedAssetPath(formattedPath, 0, pack->rootDirectory, slot, g_chunkName);
            PACK_CopyChunkName(entry);
            PackModel *const model = PAKF_Mallocs(static_cast<PakChunkHeader *>(entryData), slot->textureIds, nullptr, g_chunkName, formattedPath);
            Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(model->model), formattedPath, pack->renderHandleOrReadMode, 0x4022);
            if ((slot->flags & PackSlotSpecialAllocation) != 0) {
                reinterpret_cast<std::uint32_t *>(model)[-7] |= 1;
            }
            auto *const modelHeader = static_cast<std::int16_t *>(model->model);
            auto *const modelField14 = reinterpret_cast<std::int32_t *>(reinterpret_cast<std::byte *>(model) + 0x14);
            *modelField14 = modelHeader && *modelHeader > 28672 ? *modelHeader - 28673 : 0;
            storeResult(resultIndex++, model);
            break;
        }
        case SHEN_NAME(MOTN):
            slot->motionHeaders[motionIndex++] = MOTN_ReadMOTNHeader(entryData);
            break;
        case SHEN_NAME(CHRT):
            if ((slot->flags & PackSlotHasChrt) == 0) {
                pack->chrtData = entryData;
                pack->chrtSize = entry.auxiliary;
                slot->flags |= PackSlotHasChrt;
            }
            break;
        default:
            break;
        }
        ++chunkIndex;
    }

    if (header->magic == kPaksMagic) {
        slot->pakfResultIndex = static_cast<std::int16_t>(resultIndex);
        slot->motionIndex = static_cast<std::int16_t>(motionIndex);
    }
    if (chunkIndex >= entryCount) {
        if (header->magic == kPakfMagic) {
            slot->chunkIndex = 0;
        }
        return 1;
    }
    slot->chunkIndex = static_cast<std::int16_t>(chunkIndex);
    return 0;
}

char *CombineFileExtensionAndFilename(char *filename, const char *extension) {
    char *output = g_packExtensionFilename;
    while ((*output++ = *filename++) != '\0') {
    }
    --output;
    *output++ = '.';
    while ((*output++ = *extension++) != '\0') {
    }
    return g_packExtensionFilename;
}

PackSlot *PACK_FindModelOwner(PackModel *model) {
    PackTaskData *pack = g_packTaskData;
    if (!pack) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    for (PackSlot &slot : g_packSlots) {
        if (!slot.tag || &slot == pack->loadingSlot || !slot.pakfResults)
            continue;
        for (std::int32_t index = 0; index < slot.pakfResultIndex; ++index) {
            if (slot.pakfResults[index] == model)
                return &slot;
        }
    }
    return nullptr;
}

namespace {

std::int32_t PACK_FindDirectoryEntryIndex(PakDirectory *directory, const char *filename) {
    if (directory->entryCount <= 0)
        return -1;

    const std::size_t length = std::strlen(filename);
    if (length > 15)
        return -1;
    char wanted[16]{};
    std::strcpy(wanted, filename);
    bool reachedExtension = false;
    for (std::size_t index = 0; index < 15; ++index) {
        if (!wanted[index] || wanted[index] == '.')
            reachedExtension = true;
        if (reachedExtension) {
            wanted[index] = '\0';
        } else {
            wanted[index] = static_cast<char>(std::toupper(static_cast<unsigned char>(wanted[index])));
        }
    }

    const PakDirectoryEntry *const entries = PACK_GetDirectoryEntries(directory);
    std::int32_t first = 0;
    std::int32_t last = directory->entryCount - 1;
    while (first < last) {
        const std::int32_t middle = (first + last) / 2;
        if (std::memcmp(entries[middle].name, wanted, 8) < 0)
            first = middle + 1;
        else
            last = middle;
    }
    return std::memcmp(entries[first].name, wanted, 8) == 0 ? first : -1;
}

} // namespace

PackModel *PACK_FindCharacterModel(PackSlot *slot, const char *filename) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(slot);
    if (address < reinterpret_cast<std::uintptr_t>(&g_packSlots[0]) || address > reinterpret_cast<std::uintptr_t>(&g_packSlots[31])) {
        return nullptr;
    }
    const std::int32_t slotIndex = static_cast<std::int32_t>(PACK_GetSlotIndex(slot));
    const std::uint32_t expectedTag = ((static_cast<std::uint32_t>(slotIndex) + 97U) << 24U) | 0x4d5048U;
    if (slot->tag != expectedTag || !slot->pakData)
        return nullptr;

    if (!PACK_IsValidSlot(slot)) {
        PackTaskData *pack = g_packTaskData;
        if (!pack) {
            pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
        }
        if (pack->activeReader != slot)
            return nullptr;
    }

    auto *const directory = reinterpret_cast<PakDirectory *>(reinterpret_cast<std::byte *>(slot->pakData) + slot->pakData->directoryOffset);
    const std::int32_t entryIndex = PACK_FindDirectoryEntryIndex(directory, filename);
    if (entryIndex == -1)
        return nullptr;
    PakDirectoryEntry *const entries = PACK_GetDirectoryEntries(directory);
    const PakDirectoryEntry &entry = entries[entryIndex];
    if (entry.tag != SHEN_NAME(CHRM) && entry.tag != SHEN_NAME(MAPM) && entry.tag != SHEN_NAME(MAPD)) {
        return nullptr;
    }
    auto *const chunk = reinterpret_cast<PakChunkHeader *>(reinterpret_cast<std::byte *>(directory) + static_cast<std::int32_t>(entry.dataOffset));
    if (!slot->pakfResults)
        return nullptr;
    for (std::int32_t index = 0; index < slot->pakfResultIndex; ++index) {
        if (slot->pakfResults[index]->pakf == chunk)
            return slot->pakfResults[index];
    }
    return nullptr;
}

void *PACK_FindFileData(PackSlot *slot, const char *filename, std::uint32_t tag) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(slot);
    if (address < reinterpret_cast<std::uintptr_t>(&g_packSlots[0]) || address > reinterpret_cast<std::uintptr_t>(&g_packSlots[31])) {
        return nullptr;
    }
    const std::int32_t slotIndex = static_cast<std::int32_t>(PACK_GetSlotIndex(slot));
    const std::uint32_t expectedTag = ((static_cast<std::uint32_t>(slotIndex) + 97U) << 24U) | 0x4d5048U;
    if (slot->tag != expectedTag || !slot->pakData)
        return nullptr;

    auto *const directory = reinterpret_cast<PakDirectory *>(reinterpret_cast<std::byte *>(slot->pakData) + slot->pakData->directoryOffset);
    const std::int32_t entryIndex = PACK_FindDirectoryEntryIndex(directory, filename);
    if (entryIndex == -1)
        return nullptr;
    PakDirectoryEntry *const entries = PACK_GetDirectoryEntries(directory);
    const PakDirectoryEntry &entry = entries[entryIndex];
    if (entry.tag != tag)
        return nullptr;
    return reinterpret_cast<std::byte *>(directory) + static_cast<std::int32_t>(entry.dataOffset);
}
void *LoadFileIntoHeapMem(PackSlot *slot, const char *filename, std::uint32_t tag) {
    return PACK_FindFileData(slot, filename, tag);
}

// pack_find_file_data_and_extract_filename: the byte size a 'BIN ' directory entry records in its auxiliary word.
std::uint32_t PACK_FindBinFileSize(PackSlot *slot, const char *filename) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(slot);
    if (address < reinterpret_cast<std::uintptr_t>(&g_packSlots[0]) || address > reinterpret_cast<std::uintptr_t>(&g_packSlots[31]))
        return 0;
    const std::int32_t slotIndex = static_cast<std::int32_t>(PACK_GetSlotIndex(slot));
    if (slot->tag != (((static_cast<std::uint32_t>(slotIndex) + 97U) << 24U) | 0x4d5048U) || !slot->pakData)
        return 0;
    auto *const directory = reinterpret_cast<PakDirectory *>(reinterpret_cast<std::byte *>(slot->pakData) + slot->pakData->directoryOffset);
    const std::int32_t entryIndex = PACK_FindDirectoryEntryIndex(directory, filename);
    if (entryIndex < 0)
        return 0;
    const PakDirectoryEntry &entry = PACK_GetDirectoryEntries(directory)[entryIndex];
    return entry.tag == 0x204e4942U ? entry.auxiliary : 0;
}

void ReadInnerFileFromPak(PackTaskData *pack, PackSlot *slot, std::int32_t innerIndex) {
    slot->flags |= PackSlotAsyncCachedRead;
    const std::int32_t fileId = slot->ownerOrHumans->adfsWrapper->status;
    LargeFileHandle *const file = LargeFile_OpenTocEntryHandle(fileId, innerIndex);
    slot->asyncFile = file;

    const std::int64_t blockCount = LargeFile_GetSizeInBlocks(file);
    const std::uint32_t packSize = static_cast<std::uint32_t>(blockCount << 11);

    if (!slot->pakData) {
        const std::int64_t slotIndex = PACK_GetSlotIndex(slot);
        slot->pakData =
            static_cast<PakFileHeader *>(HLib_AllocHeapMemBlock(packSize, ((static_cast<std::int32_t>(slotIndex) + 97) << 24) | kPackAllocationToken));
    }
    if (!slot->pakData) {
        pack->loadingSlot = nullptr;
        PACK_CloseSlotFile(0, slot, false);
        return;
    }

    (void)GetFileFromCachedBuffer(file, static_cast<std::uint64_t>(blockCount), slot->pakData, 0);
    slot->pakSize = packSize;
    std::strcpy(reinterpret_cast<char *>(g_currentPak), slot->name);
}

void PAK_Wrapper(PackTaskData *pack, PackSlot *slot, char *rootDirectory, char *filename) {
    pack->adfsRetryCount = 0;
    slot->flags &= ~PackSlotAsyncCachedRead;

    FileState *const file = Archive_GetNextUnusedFileState();
    slot->asyncFile = file;
    if (!PrepareDirectoryPathForFile(file, rootDirectory, filename)) {
        pack->loadingSlot = nullptr;
        return;
    }

    std::uint32_t fileSize = 0;
    if (Archive_DoesHandleExist(file->handle)) {
        fileSize = static_cast<std::uint32_t>(OpenFileInternal(file->path));
    }
    slot->pakSize = fileSize;

    const std::int64_t slotIndex = PACK_GetSlotIndex(slot);
    const std::int32_t token = ((static_cast<std::int32_t>(slotIndex) + 97) << 24) | static_cast<std::int32_t>(kPackAllocationToken);
    void *packData = nullptr;
    if ((slot->flags & PackSlotPaksPhase) == 0 || (pack->flags & PackDataLoadPksDirect) != 0) {
        packData = HLib_AllocMemBlock((fileSize + 31) & 0xffffffe0u, token);
    } else if ((slot->flags & PackSlotGzip) != 0) {
        packData = HLib_AllocMemBlock(fileSize, token);
    } else {
        packData = HLib_AllocHeapMemBlock(fileSize, token);
    }

    slot->pakData = static_cast<PakFileHeader *>(packData);
    if (!packData) {
        pack->loadingSlot = nullptr;
        PACK_CloseSlotFile(0, slot, false);
        return;
    }

    OpenFile(file, packData, static_cast<std::int32_t>(fileSize));
    std::strcpy(reinterpret_cast<char *>(g_currentPak), slot->name);
}

void PACK_Callback(HLib::TaskData *taskData) {
    auto *pack = reinterpret_cast<PackTaskData *>(taskData);
    if (!pack) {
        pack = g_packTaskData;
        if (!pack) {
            return;
        }
    }

    if ((!pack->loadingSlot && !pack->pendingPakf && !pack->activeReader) || PACK_IsRenderHandleValid(pack->renderHandleOrReadMode)) {
        PACK_YieldToInitCallback();
        return;
    }

    PackSlot *loading = pack->loadingSlot;
    if (loading) {
        if ((loading->flags & PackSlotAsyncCachedRead) != 0) {
            const std::int32_t fileState = CheckFileInfo(loading->asyncFile);
            if (fileState == 2) {
                goto process_readers;
            }
            if (fileState != 3) {
                PackSlot *const slot = pack->loadingSlot;
                PackOwner *const owner = slot->ownerOrHumans;
                if (slot->asyncFile) {
                    PACK_CloseSlotFile(0, slot, true);
                }

                if (owner) {
                    if (owner->adfsWrapper) {
                        HLib_FreeTaskData(owner->adfsWrapper->taskData, 0);
                        owner->adfsWrapper->status = -1;
                        owner->adfsWrapper = nullptr;
                    }

                    if (owner->directory[0] && owner->filename[0] && std::strlen(owner->filename) <= sizeof(owner->filename) - 1 &&
                        pack->adfsRetryCount++ < 120) {
                        owner->adfsWrapper = ReadADFS_Wrapper(owner->directory, owner->filename, 1000);
                        if (owner->adfsWrapper) {
                            const std::int32_t innerIndex = (slot->flags & PackSlotPaksPhase) ? slot->pksInnerIndex : slot->pkfInnerIndex;
                            (void)ReadInnerFileFromPak(pack, slot, innerIndex);
                        } else {
                            pack->loadingSlot = nullptr;
                        }
                    } else {
                        pack->loadingSlot = nullptr;
                        PACK_FreeOwner(owner);
                        PACK_FreeMapStorage(slot);
                    }
                } else {
                    pack->loadingSlot = nullptr;
                    PACK_FreeMapStorage(slot);
                }
                goto process_readers;
            }
        } else if (PACK_IsOpenFileTableEntryActive(loading->asyncFile)) {
            goto process_readers;
        }

        PackSlot *const slot = pack->loadingSlot;
        pack->loadingSlot = nullptr;

        const std::uint32_t originalFlags = slot->flags;
        if ((originalFlags & PackSlotPaksPhase) == 0 || (pack->flags & PackDataLoadPksDirect) != 0) {
            const std::uint32_t expectedMagic = (pack->flags & PackDataLoadPksDirect) ? kPaksMagic : kPakfMagic;
            if (slot->pakData->magic != expectedMagic && *reinterpret_cast<std::uint16_t *>(slot->pakData) == 0x8b1f) {
                slot->flags = originalFlags | PackSlotGzip;
            }
        }

        if ((slot->flags & PackSlotGzip) != 0) {
            auto *const bytes = reinterpret_cast<std::byte *>(slot->pakData);
            const std::uint32_t decompressedSize = *reinterpret_cast<std::uint32_t *>(bytes + slot->pakSize - 4);
            const std::int64_t slotIndex = PACK_GetSlotIndex(slot);
            void *const decompressed = HLib_AllocHeapMemBlock(decompressedSize, ((static_cast<std::int32_t>(slotIndex) + 97) << 24) | kPackAllocationToken);
            void *const work3000 = HLib_AllocMemBlock(0x3000, 0);
            void *const work1000 = HLib_AllocMemBlock(0x1000, 0);
            if (!decompressed || !work3000 || !work1000) {
                LoadFile(slot);
                return;
            }
            decompress(slot->pakData, decompressed, 0, 0, decompressedSize, 0, slot->pakSize);
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(work1000), 0);
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(work3000), 0);
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(slot->pakData), 0);
            slot->pakData = static_cast<PakFileHeader *>(decompressed);
        }

        if (slot->pakData->magic == kPakfMagic) {
            pack->pendingPakf = slot;
        } else if (slot->pakData->magic == kPaksMagic) {
            pack->activeReader = slot;
        }
    }

process_readers:
    PackSlot *const pending = pack->pendingPakf;
    if (pending && AllocatePAKT(pack, pending)) {
        pack->pendingPakf = nullptr;
        pack->activeReader = pending;
    }

    PackSlot *const active = pack->activeReader;
    if (!active || !PKF_Reader(pack, active)) {
        return;
    }

    const std::uint32_t pakType = active->pakData->magic;
    if (pakType != kPakfMagic) {
        if (pakType == kPaksMagic) {
            PACK_CloseSlotFile(pakType, active, false);
            if (pack->chrtData) {
                const std::int64_t slotIndex = PACK_GetSlotIndex(active);
                ParseCHID(pack->chrtData, ((static_cast<std::int32_t>(slotIndex) + 97) << 24) | kPackAllocationToken);
                HLib_FreeTaskData(static_cast<HLib::TaskData *>(pack->chrtData), 0);
                pack->chrtData = nullptr;
            }
            if ((active->flags & PackSlotSpecialAllocation) != 0) {
                auto *const packWords = reinterpret_cast<std::uint32_t *>(active->pakData);
                if (packWords) {
                    packWords[-7] |= 1;
                }
                if (active->textureIds) {
                    reinterpret_cast<std::uint32_t *>(active->textureIds)[-7] |= 1;
                }
            }
            pack->readyPack = active;
        }
        pack->activeReader = nullptr;
        return;
    }

    active->flags |= PackSlotPaksPhase;
    void *temporaryChrt = nullptr;
    std::uint32_t copiedChrtSize = 0;
    if (pack->chrtData) {
        copiedChrtSize = (pack->chrtSize + 31) & 0xffffffe0u;
        temporaryChrt = HLib_AllocMemBlock(copiedChrtSize, kPaksAllocationToken);
        if (!temporaryChrt) {
            pack->loadingSlot = nullptr;
            pack->activeReader = nullptr;
            return;
        }
        std::memcpy(temporaryChrt, pack->chrtData, copiedChrtSize);
    }

    PACK_CloseSlotFile(pakType, active, true);
    if (pack->chrtData) {
        pack->chrtData = HLib_AllocHeapMemBlock(copiedChrtSize, kChrtAllocationToken);
        if (!pack->chrtData) {
            pack->loadingSlot = nullptr;
            pack->activeReader = nullptr;
            return;
        }
        std::memcpy(pack->chrtData, temporaryChrt, copiedChrtSize);
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(temporaryChrt), 0);
    }

    if (active->textureIds && (active->flags & PackSlotSpecialAllocation) == 0) {
        std::int32_t textureBytes = 0;
        if ((reinterpret_cast<std::uintptr_t>(active->textureIds) & 0x1f) == 0) {
            textureBytes = active->textureIds[-14] - 64;
        }
        const std::size_t copySize = (static_cast<std::size_t>(textureBytes) + 31) & ~std::size_t{31};
        void *const copiedTextureIds = HLib_AllocHeapMemBlock(static_cast<std::int64_t>(copySize), kTextureAllocationToken);
        std::memcpy(copiedTextureIds, active->textureIds, copySize);
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(active->textureIds), 0);
        active->textureIds = static_cast<std::int32_t *>(copiedTextureIds);
    }

    if ((pack->flags & PackDataSkipPksFollowup) == 0) {
        if ((active->flags & PackSlotAsyncCachedRead) != 0) {
            ReadInnerFileFromPak(pack, active, active->pksInnerIndex);
        } else {
            char *const filename = CombineFileExtensionAndFilename(active->name, "PKS");
            PAK_Wrapper(pack, active, pack->rootDirectory, filename);
        }
        pack->loadingSlot = active;
    }
    pack->activeReader = nullptr;
}

void PACK_Init() {
    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(PACK_Callback, 3, 2, SHEN_NAME(PACK));
    if (task && !task->task_data_ptr) {
        HLib::TaskData *const allocated = static_cast<HLib::TaskData *>(HLib_AllocHeapMemBlock(sizeof(PackTaskData), SHEN_NAME(TASK)));
        if (allocated)
            HLib_EnableTokenCheck(allocated);
        task->task_data_ptr = allocated;
        if (allocated)
            goto task_ready;
    }
    if (task) {
        task->initCallbackFnPtr = nullptr;
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        task->autoInitTaskCallbackFnPtr = nullptr;
    }
    task = nullptr;

task_ready:
    g_packTask = task;
    auto *const data = reinterpret_cast<PackTaskData *>(task ? task->task_data_ptr : nullptr);
    data->tag = SHEN_NAME(PACK);
    data->chidParseGuard = 0;
    data->usePackCache = 0;
    data->rootDirectory[0] = '\0';
    data->chrtData = nullptr;
    data->chrtSize = 0;
    data->readyPack = nullptr;
    data->loadingSlot = nullptr;
    data->pendingPakf = nullptr;
    data->activeReader = nullptr;
    data->renderHandleOrReadMode = 0;
    data->flags = 0;
    data->callbackBudget = 3000;
    data->adfsRetryCount = 0;

    g_packTaskData = nullptr;
    std::memset(g_packSlots, 0, sizeof(g_packSlots));
    for (PackOwner &owner : g_packOwners) {
        owner.adfsWrapper = nullptr;
        owner.directory[0] = '\0';
        owner.filename[0] = '\0';
        owner.referenceCount = 0;
    }

    if (task) {
        if (!task->initCallbackFnPtr) {
            task->initCallbackFnPtr = task->mainTaskCallbackFnPtr;
            task->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
            task->mainExecutionIterations = 0;
        }
        task->mainExecutionIterations = 0;
    }
    std::memset(g_currentPak, 0, sizeof(g_currentPak));
}

namespace {

char PACK_FoldCase(char value) {
    return value >= 'a' && value <= 'z' ? static_cast<char>(value - ('a' - 'A')) : value;
}

bool PACK_NameEquals(const char *left, const char *right) {
    const std::size_t count = std::min(std::strlen(left), std::strlen(right));
    for (std::size_t index = 0; index < count; ++index) {
        if (PACK_FoldCase(left[index]) != PACK_FoldCase(right[index])) {
            return false;
        }
    }
    return true;
}

bool PACK_ExactNameEquals(const char *left, const char *right) {
    while (true) {
        const char foldedLeft = PACK_FoldCase(*left++);
        const char foldedRight = PACK_FoldCase(*right++);
        if (foldedLeft != foldedRight)
            return false;
        if (!foldedLeft)
            return true;
    }
}

bool PACK_IsBusy(const PackTaskData *pack) {
    return !pack || pack->loadingSlot || pack->pendingPakf || pack->activeReader;
}

bool PACK_HasNamedSlot(const char *name) {
    for (const PackSlot &slot : g_packSlots) {
        if (slot.tag && PACK_NameEquals(slot.name, name))
            return true;
    }
    return false;
}

PackSlot *PACK_AllocateSlot(const char *name) {
    for (std::int32_t index = 0; index < 32; ++index) {
        PackSlot *const slot = &g_packSlots[index];
        if (slot->tag)
            continue;
        std::memset(slot, 0, sizeof(*slot));
        slot->tag = ((static_cast<std::uint32_t>(index) + 97U) << 24U) | 0x004d5048U;
        std::strcpy(slot->name, name);
        slot->ownerOrHumans = nullptr;
        slot->pkfInnerIndex = -1;
        slot->pksInnerIndex = -1;
        return slot;
    }
    return nullptr;
}

bool PACK_SetRootDirectory(PackTaskData *pack, const char *rootDirectory) {
    if (!pack || !rootDirectory || std::strlen(rootDirectory) >= sizeof(pack->rootDirectory)) {
        return false;
    }
    std::strcpy(pack->rootDirectory, rootDirectory);
    std::size_t length = std::strlen(pack->rootDirectory);
    while (length && (pack->rootDirectory[length - 1] == '/' || pack->rootDirectory[length - 1] == '\\')) {
        pack->rootDirectory[--length] = '\0';
    }
    return true;
}

PackOwner *PACK_AcquireOwner(const char *directory, const char *filename) {
    PackOwner *freeOwner = nullptr;
    for (PackOwner &owner : g_packOwners) {
        if (owner.adfsWrapper && PACK_NameEquals(owner.directory, directory) && PACK_NameEquals(owner.filename, filename)) {
            ++owner.referenceCount;
            return &owner;
        }
        if (!freeOwner && !owner.adfsWrapper)
            freeOwner = &owner;
    }
    if (!freeOwner)
        return nullptr;

    PackAdfsWrapper *const wrapper = ReadADFS_Wrapper(const_cast<char *>(directory), filename, 1000);
    freeOwner->adfsWrapper = wrapper;
    if (!wrapper)
        return nullptr;
    std::strcpy(freeOwner->directory, directory);
    std::strcpy(freeOwner->filename, filename);
    freeOwner->referenceCount = 1;
    return freeOwner;
}

void PACK_ArmCallback(PackTaskData *pack) {
    g_packTaskData = pack;
    g_packTask->mainTaskCallbackFnPtr = PACK_Callback;
    g_packTask->initCallbackFnPtr = nullptr;
    g_packTask->mainExecutionIterations = 0;
}

} // namespace
PackSlot *PACK_FindSlotByName(const char *name) {
    for (PackSlot &slot : g_packSlots) {
        if (slot.tag && PACK_NameEquals(slot.name, name))
            return &slot;
    }
    return nullptr;
}



void PACK_SetReadyPack(PackSlot *slot) {
    const std::int64_t index = PACK_GetSlotIndex(slot);
    if (index < 0 || slot->tag != (((static_cast<std::uint32_t>(index) + 97U) << 24U) | 0x004d5048U) || !slot->pakData)
        return;
    PackTaskData *pack = g_packTaskData;
    if (!pack)
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    pack->readyPack = slot;
}

std::int64_t PACK_MarkFree() {
    PackTaskData *pack = g_packTaskData;
    if (!pack) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    PackSlot *const loading = pack->loadingSlot;
    if (!loading)
        return 0;

    PackSlot *const active = pack->activeReader;
    if (active && active->pakData->magic == kPaksMagic)
        return 0;

    if ((loading->flags & PackSlotAsyncCachedRead) != 0) {
        if (loading->asyncFile) {
            ReadCachedFile(loading->asyncFile);
            loading->asyncFile = nullptr;
        }
    } else if (loading->asyncFile) {
        auto *const file = static_cast<PackOpenFile *>(loading->asyncFile);
        ReadFile_0(reinterpret_cast<std::uint64_t>(active ? active->pakData : nullptr), &file->fileId);
        loading->asyncFile = nullptr;
    }
    PACK_FreeMapStorage(loading);
    pack->loadingSlot = nullptr;
    return 1;
}

void PACK_LoadFile(PackSlot *slot) {
    LoadFile(slot);
}

PackOwner *PACK_ReadAFS(const char *rootDirectory, const char *filename) {
    return PACK_AcquireOwner(rootDirectory, filename);
}

PackSlot *PACK_FindAfsEntrySlot(PackOwner *owner, std::int32_t pkfEntryIndex, std::int32_t pksEntryIndex) {
    const std::uintptr_t ownerAddress = reinterpret_cast<std::uintptr_t>(owner);
    if (ownerAddress < reinterpret_cast<std::uintptr_t>(&g_packOwners[0]) || ownerAddress > reinterpret_cast<std::uintptr_t>(&g_packOwners[7]))
        return nullptr;
    char slotName[24];
    std::sprintf(slotName, "%s#%03d#%03d", owner->filename, pkfEntryIndex, pksEntryIndex);
    for (PackSlot &slot : g_packSlots) {
        if (slot.tag && PACK_NameEquals(slot.name, slotName))
            return &slot;
    }
    return nullptr;
}

PackSlot *PACK_LoadHumanByIndex(PackOwner *owner, std::uint32_t pkfEntryIndex, std::uint32_t pksEntryIndex) {
    const std::uintptr_t ownerAddress = reinterpret_cast<std::uintptr_t>(owner);
    if (ownerAddress < reinterpret_cast<std::uintptr_t>(&g_packOwners[0]) || ownerAddress > reinterpret_cast<std::uintptr_t>(&g_packOwners[7]) ||
        !owner->adfsWrapper) {
        return nullptr;
    }
    PackTaskData *pack = g_packTaskData;
    if (!pack) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    if (pack->loadingSlot || pack->pendingPakf || pack->activeReader) {
        return nullptr;
    }



    char slotName[24];
    std::sprintf(slotName, "%s#%03d#%03d", owner->filename, static_cast<std::int32_t>(pkfEntryIndex), static_cast<std::int32_t>(pksEntryIndex));
    for (PackSlot &existing : g_packSlots) {
        if (existing.tag && PACK_ExactNameEquals(existing.name, slotName)) {
            return &existing;
        }
    }
    PackSlot *const slot = PACK_AllocateSlot(slotName);
    if (!slot)
        return nullptr;
    slot->ownerOrHumans = owner;
    slot->pkfInnerIndex = static_cast<std::int16_t>(pkfEntryIndex);
    slot->pksInnerIndex = static_cast<std::int16_t>(pksEntryIndex);
    pack->loadingSlot = slot;
    ReadInnerFileFromPak(pack, slot, static_cast<std::int32_t>(pkfEntryIndex));
    SetMainCallbackToInitForCleanup(g_packTask);
    return slot;
}

std::int64_t PACK_FreeOwnerADFSData(void *owner) {
    return PACK_FreeOwner(static_cast<PackOwner *>(owner));
}

PackSlot *PACK_QueueFile(const char *rootDirectory, const char *packageName) {
    PackTaskData *pack = g_packTaskData;
    if (!pack) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    if (pack->loadingSlot || pack->pendingPakf || pack->activeReader)
        return nullptr;

    for (PackSlot &existing : g_packSlots) {
        if (!existing.tag)
            continue;
        const char *wanted = packageName;
        const char *available = existing.name;
        while (true) {
            const char left = PACK_FoldCase(*wanted);
            const char right = PACK_FoldCase(*available);
            if (left != right)
                break;
            if (!left)
                return &existing;
            ++wanted;
            ++available;
        }
    }

    PackSlot *const slot = PACK_AllocateSlot(packageName);
    if (!slot)
        return nullptr;
    slot->pkfInnerIndex = 0;
    slot->pksInnerIndex = 0;
    pack->loadingSlot = slot;
    std::strcpy(pack->rootDirectory, rootDirectory);
    const std::size_t rootLength = std::strlen(pack->rootDirectory);
    if (rootLength > 1 && pack->rootDirectory[rootLength - 1] == '/')
        pack->rootDirectory[rootLength - 1] = '\0';

    const bool loadPksDirect = (pack->flags & PackDataLoadPksDirect) != 0;
    if (loadPksDirect)
        slot->flags |= PackSlotPaksPhase;
    char *const filename = CombineFileExtensionAndFilename(slot->name, loadPksDirect ? "PKS" : "PKF");
    PAK_Wrapper(pack, slot, pack->rootDirectory, filename);
    SetMainCallbackToInitForCleanup(g_packTask);
    return slot;
}

PackSlot *PACK_QueueAFSPair(const char *rootDirectory, const char *afsFilename, std::int32_t pkfEntryIndex, std::int32_t pksEntryIndex, const char *) {
    PackTaskData *pack = g_packTaskData;
    if (!pack) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    if (pack->loadingSlot || pack->pendingPakf || pack->activeReader)
        return nullptr;

    char slotName[24];
    std::sprintf(slotName, "%s#%03d#%03d", afsFilename, pkfEntryIndex, pksEntryIndex);
    for (PackSlot &existing : g_packSlots) {
        if (!existing.tag)
            continue;
        const char *wanted = slotName;
        const char *available = existing.name;
        while (true) {
            const char left = PACK_FoldCase(*wanted);
            const char right = PACK_FoldCase(*available);
            if (left != right)
                break;
            if (!left)
                return &existing;
            ++wanted;
            ++available;
        }
    }

    PackOwner *const owner = PACK_AcquireOwner(rootDirectory, afsFilename);
    if (!owner)
        return nullptr;
    PackSlot *const slot = PACK_AllocateSlot(slotName);
    if (!slot)
        return nullptr;
    slot->ownerOrHumans = owner;
    slot->pkfInnerIndex = static_cast<std::int16_t>(pkfEntryIndex);
    slot->pksInnerIndex = static_cast<std::int16_t>(pksEntryIndex);
    pack->loadingSlot = slot;
    ReadInnerFileFromPak(pack, slot, pkfEntryIndex);
    SetMainCallbackToInitForCleanup(g_packTask);
    return slot;
}
void PACK_Drain() {
    PackTaskData *pack = g_packTaskData;
    if (!pack) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }

    while (pack->loadingSlot || pack->pendingPakf || pack->activeReader || PACK_IsRenderHandleValid(pack->renderHandleOrReadMode)) {
        PACK_Callback(reinterpret_cast<HLib::TaskData *>(pack));
        RenderScreen();
        d3t_CloseRenderHandle(pack->renderHandleOrReadMode);
    }
}



void PACK_Shutdown() {
    if (!g_packTask)
        return;
    PACK_Drain();
    for (PackSlot &slot : g_packSlots) {
        if (slot.tag && (slot.flags & PackSlotSpecialAllocation) == 0)
            LoadFile(&slot);
    }
    for (PackOwner &owner : g_packOwners)
        PACK_FreeOwner(&owner);
    HLib_DestroyTask(g_packTask);
    g_packTask = nullptr;
    if (g_packTaskData)
        g_TotalSceneFileReadBytes = reinterpret_cast<std::byte *>(g_packTaskData) - static_cast<std::byte *>(g_pSTCH) - 4;
}

std::int64_t PACK_HasValidPack() {
    PackTaskData *pack = g_packTaskData;
    if (!pack) {
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    }
    return pack->loadingSlot || pack->pendingPakf || pack->activeReader || (g_scrollCurrentInfo && g_bShouldFreeActiveScroll) ? 1 : 0;
}

} // namespace shenmue::s1

#include <cstdint>

namespace shenmue::s1 {
void PACK_ReleaseSlotCharacters(PackSlot *slot) {
    PackTaskData *pack = g_packTaskData;
    if (!pack && g_packTask)
        pack = reinterpret_cast<PackTaskData *>(HLib_GetTaskDataForTask(g_packTask));
    std::int32_t slotIndex = 0;
    while (&g_packSlots[slotIndex] != slot) {
        if (++slotIndex >= 32) {
            slotIndex = 0xff;
            break;
        }
    }
    PackSlot *const previousReader = pack->activeReader;
    pack->activeReader = slot;
    pack->chidParseGuard = 1;
    CEWP_RetireGroup(((slotIndex + 97) << 24) | static_cast<std::int32_t>(kPackAllocationToken), true);
    MapControlInternal();
    if (slot)
        MDLM_UnloadPackImages(slot);
    pack->activeReader = previousReader;
    pack->chidParseGuard = 0;
}

} // namespace shenmue::s1
