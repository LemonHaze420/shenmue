#include "s1/dynamic_map.hpp"

#include "s1/hlib.hpp"
#include "s1/lighting.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/task/came.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/scen.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {

namespace {

constexpr DynamicMapEntry MakeEmptyDynamicMapEntry() {
    DynamicMapEntry entry{};
    entry.renderEnabled = 1;
    entry.staticSceneMode = 1;
    entry.occlusionEnabled = 1;
    entry.scale = 1.0f;
    return entry;
}

constexpr std::array<DynamicMapEntry, 32> MakeEmptyDynamicMapTable() {
    std::array<DynamicMapEntry, 32> table{};
    for (DynamicMapEntry &entry : table) {
        entry = MakeEmptyDynamicMapEntry();
    }
    return table;
}

constexpr DynamicMapStaticTransform MakeEmptyStaticTransform() {
    DynamicMapStaticTransform transform{};
    transform.enabled = 1;
    transform.sourceMapSlot = 0x7fff;
    transform.field_18 = 1;
    return transform;
}

constexpr std::array<DynamicMapStaticTransform, 32> MakeEmptyStaticTransformTable() {
    std::array<DynamicMapStaticTransform, 32> table{};
    for (DynamicMapStaticTransform &transform : table)
        transform = MakeEmptyStaticTransform();
    return table;
}

} // namespace

std::array<DynamicMapEntry, 32> g_dynamicMapSlots = MakeEmptyDynamicMapTable();
DynamicMapAuxiliary g_dynamicMapAuxiliary[32]{};
float g_mapBlockScale = 0.0f;
std::uint32_t g_mapAreaOverrideId = 0;
std::array<DynamicMapStaticTransform, 32> g_dynamicMapStaticTransforms = MakeEmptyStaticTransformTable();
DynamicMapClipRecord *g_clipRecords = nullptr;
std::int32_t g_clipRecordCount = 0;
DynamicMapLodSnapshot g_dynamicMapLodSnapshot{
    .renderEnabledMask = 0xffffffffU,
    .occlusionEnabledMask = 0xffffffffU,
    .clipCellsReadyMask = 0xffffffffU,
    .lodDistance0 = 140.0f,
    .lodDistance2 = 140.0f,
    .npcDisplayRadius = 48.0f,
};

namespace {

constexpr std::array<std::int32_t, 12> kNodeClipCategories{
    0,
    0,
    255,
    0,
    4,
    0,
    8,
    0,
    12,
    16,
    0,
    0,
};

#pragma pack(push, 1)
struct ClipNode {
    std::uint16_t nodeKind;
    std::uint16_t nodeFlags;
    std::int32_t sceneBindingOffset;
    Vector3i rotation;
    Vector3 scale;
    Vector3 position;
    std::int32_t childOffset;
    std::int32_t siblingOffset;
    std::int32_t parentOffset;
    std::int32_t temporaryOffset;
    std::int32_t transformOffset;
};

struct ClipSceneBinding {
    std::byte reserved_00[0x10];
    float sphere[4];
};
#pragma pack(pop)

using ClipRecord = DynamicMapClipRecord;
using ClipCellRun = DynamicMapClipCellRun;

struct DynamicClipCell {
    Vector3 position;
    float radius;
};

static_assert(sizeof(ClipNode) == 0x40);
static_assert(offsetof(ClipNode, childOffset) == 0x2c);
static_assert(offsetof(ClipNode, siblingOffset) == 0x30);
static_assert(sizeof(ClipSceneBinding) == 0x20);
static_assert(sizeof(ClipRecord) == 0x28);
static_assert(offsetof(ClipRecord, node) == 0x18);
static_assert(sizeof(ClipCellRun) == 0x0c);
static_assert(sizeof(DynamicClipCell) == 0x10);

std::int32_t g_dynamicClipRecordCount = 0;
std::int32_t g_category4Count = 0;
std::int32_t g_category8Count = 0;
std::int32_t g_category12Count = 0;
std::int32_t g_category16Count = 0;
std::int16_t *g_staticClipCells = nullptr;
std::int16_t *g_dynamicClipCells = nullptr;
float g_activeBlockScale = 0.0f;
std::byte *g_activeModelBase = nullptr;

ClipNode *NodeFromOffset(std::int32_t offset) {
    return offset ? reinterpret_cast<ClipNode *>(g_activeModelBase + offset) : nullptr;
}

ClipSceneBinding *BindingFor(ClipNode *node) {
    return static_cast<ClipSceneBinding *>(MT5_SceneBindingForNode(node, nullptr));
}

void CountClipRecordsRecursive(ClipNode *node, std::int32_t &count) {
    do {
        if (BindingFor(node)) {
            ++count;
        }
        if (ClipNode *const child = NodeFromOffset(node->childOffset)) {
            CountClipRecordsRecursive(child, count);
        }
        node = NodeFromOffset(node->siblingOffset);
    } while (node);
}

std::int32_t InsertUniqueCell(std::int16_t *cells, std::int16_t cellX, std::int16_t cellZ) {
    std::int16_t *entry = cells;
    for (;;) {
        if (entry[0] == 0x5fff) {
            entry[0] = cellX;
            entry[1] = cellZ;
            entry[2] = 0x5fff;
            entry[3] = 0x5fff;
            return 1;
        }
        if (entry[0] == cellX && entry[1] == cellZ) {
            return 0;
        }
        entry += 2;
    }
}

void PushCurrentTransform() {
    *g_matrixStack.top.stack = workTransformMatrix;
    ++g_matrixStack.top.info.index;
    if (g_matrixStack.top.info.index < g_matrixStack.top.info.capacity) {
        ++g_matrixStack.top.stack;
    }
}

void PopOneTransform() {
    std::int16_t popCount = 1;
    if (g_matrixStack.top.info.index < 1) {
        popCount = g_matrixStack.top.info.index;
    }
    std::int16_t pointerCount = popCount;
    if (g_matrixStack.top.info.index > g_matrixStack.top.info.capacity) {
        pointerCount = static_cast<std::int16_t>(popCount + g_matrixStack.top.info.capacity - g_matrixStack.top.info.index);
    }
    g_matrixStack.top.info.index = static_cast<std::int16_t>(g_matrixStack.top.info.index - popCount);
    g_matrixStack.top.stack -= pointerCount;
    workTransformMatrix = *g_matrixStack.top.stack;
}

std::int16_t ClipCellCoordinate(float value) {
    if (value < 0.0f) {
        value -= 1.0f;
    }
    return static_cast<std::int16_t>(static_cast<std::int32_t>(value));
}

void BuildClipRecordsRecursive(ClipNode *node) {
    std::int32_t recordIndex = g_clipRecordCount;
    do {
        if (ClipSceneBinding *const binding = BindingFor(node)) {
            ClipRecord &record = g_clipRecords[recordIndex];
            record.position[0] = binding->sphere[0];
            record.position[1] = binding->sphere[1];
            record.position[2] = binding->sphere[2];
            record.radius = binding->sphere[3];

            const float maxScale = std::max(node->scale[0], std::max(node->scale[1], node->scale[2]));
            record.node = node;
            record.maxScale = maxScale;
            record.radius *= maxScale;

            if (node->nodeFlags >= kNodeClipCategories.size()) {
                node->nodeFlags = 0;
            }
            std::int32_t category = kNodeClipCategories[node->nodeFlags];
            if (category != 255 && category > 16) {
                category = 0;
            }
            record.category = category;

            const float cellXValue = (node->position[0] + record.position[0]) * g_activeBlockScale;
            const float cellZValue = (node->position[2] + record.position[2]) * g_activeBlockScale;
            std::int16_t cellX = ClipCellCoordinate(cellXValue);
            std::int16_t cellZ = ClipCellCoordinate(cellZValue);

            switch (category) {
            case 4:
                ++g_category4Count;
                break;
            case 8:
                ++g_category8Count;
                break;
            case 12:
                ++g_category12Count;
                break;
            case 16:
                ++g_category16Count;
                cellX = 0x4fff;
                cellZ = 0x4fff;
                break;
            default:
                break;
            }
            record.cellX = cellX;
            record.cellZ = cellZ;

            if (category == 255) {
                PushCurrentTransform();
                Matrix4x4::preTranslate(node->position[0], node->position[1], node->position[2]);
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation[2]));
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation[1]));
                Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation[0]));
                Matrix4x4::Scale(node->scale[0], node->scale[1], node->scale[2]);
                const Matrix4x4 transformed = workTransformMatrix;
                PopOneTransform();
                record.position[0] = transformed.rows[3].x;
                record.position[1] = transformed.rows[3].y;
                record.position[2] = transformed.rows[3].z;
                InsertUniqueCell(g_dynamicClipCells, cellX, cellZ);
                ++g_dynamicClipRecordCount;
            } else {
                InsertUniqueCell(g_staticClipCells, cellX, cellZ);
            }

            g_clipRecordCount = ++recordIndex;
        }

        if (ClipNode *const child = NodeFromOffset(node->childOffset)) {
            BuildClipRecordsRecursive(child);
            recordIndex = g_clipRecordCount;
        }
        node = NodeFromOffset(node->siblingOffset);
    } while (node);
}

std::int32_t CountCellPairs(const std::int16_t *cells) {
    std::int32_t count = 0;
    while (cells[0] != 0x5fff) {
        ++count;
        cells += 2;
    }
    return count;
}

void FillCellRuns(ClipCellRun *runs, const std::int16_t *cells, std::int32_t count) {
    for (std::int32_t index = 0; index < count; ++index) {
        runs[index].cellX = cells[0];
        runs[index].cellZ = cells[1];
        runs[index].count = 0;
        cells += 2;
    }
}

void FillFirstIndices(ClipCellRun *runs, std::int32_t count) {
    if (count <= 0) {
        return;
    }
    runs[0].firstIndex = 0;
    std::int32_t firstIndex = 0;
    for (std::int32_t index = 1; index < count; ++index) {
        firstIndex += runs[index - 1].count;
        runs[index].firstIndex = firstIndex;
    }
}

void FinalizeMapPackClipCellTables(std::int32_t mapIndex) {
    DynamicMapEntry &map = g_dynamicMapSlots[mapIndex];
    const std::int32_t staticCellCount = CountCellPairs(g_staticClipCells);
    if (mapIndex < 32) {
        map.sceneObjectCount = staticCellCount;
    }

    auto *const staticRuns = static_cast<ClipCellRun *>(HLib_AllocHeapMemBlock(sizeof(ClipCellRun) * (staticCellCount + 1), SHEN_NAME(ACPH)));
    if (mapIndex < 32) {
        map.clipCellPointers = staticRuns;
    }

    const std::int32_t staticRecordCount = g_clipRecordCount - g_dynamicClipRecordCount;
    auto **const staticRecords = static_cast<ClipRecord **>(HLib_AllocHeapMemBlock(sizeof(ClipRecord *) * (staticRecordCount + 1), SHEN_NAME(ACPD)));
    if (mapIndex < 32) {
        map.sceneObjectData = staticRecords;
    }

    if (staticCellCount > 1 && g_staticClipCells[0] != 0x4fff) {
        for (std::int32_t index = 1; index < staticCellCount; ++index) {
            std::int16_t *const cell = g_staticClipCells + 2 * index;
            if (cell[0] == 0x4fff) {
                cell[0] = g_staticClipCells[0];
                cell[1] = g_staticClipCells[1];
                g_staticClipCells[0] = 0x4fff;
                g_staticClipCells[1] = 0x4fff;
                break;
            }
        }
    }

    if (staticCellCount > 0) {
        FillCellRuns(staticRuns, g_staticClipCells, staticCellCount);
        ClipRecord **output = staticRecords;
        for (std::int32_t cellIndex = 0; cellIndex < staticCellCount; ++cellIndex) {
            ClipCellRun &run = staticRuns[cellIndex];
            for (std::int32_t recordIndex = 0; recordIndex < g_clipRecordCount; ++recordIndex) {
                ClipRecord &record = g_clipRecords[recordIndex];
                if (record.category != 255 && record.cellX == run.cellX && record.cellZ == run.cellZ) {
                    ++run.count;
                    *output++ = &record;
                }
            }
        }
        FillFirstIndices(staticRuns, staticCellCount);
    }
    staticRuns[0].firstIndex = 0;

    if (!g_dynamicClipRecordCount) {
        return;
    }

    DynamicMapAuxiliary &auxiliary = g_dynamicMapAuxiliary[mapIndex];
    const std::int32_t dynamicCellCount = CountCellPairs(g_dynamicClipCells);
    auto *const dynamicRuns = static_cast<ClipCellRun *>(HLib_AllocHeapMemBlock(sizeof(ClipCellRun) * dynamicCellCount, SHEN_NAME(DAPH)));

    auxiliary.pathData = dynamicRuns;
    auxiliary.pathCellCount = dynamicCellCount;
    FillCellRuns(dynamicRuns, g_dynamicClipCells, dynamicCellCount);

    auto *output = static_cast<DynamicClipCell *>(auxiliary.clipCells);
    for (std::int32_t cellIndex = 0; cellIndex < dynamicCellCount; ++cellIndex) {
        ClipCellRun &run = dynamicRuns[cellIndex];
        for (std::int32_t recordIndex = 0; recordIndex < g_clipRecordCount; ++recordIndex) {

            const ClipRecord &record = g_clipRecords[recordIndex];
            if (record.category == 255 && record.cellX == run.cellX && record.cellZ == run.cellZ) {
                ++run.count;
                output->position[0] = record.position[0];
                output->position[1] = record.position[1];
                output->position[2] = record.position[2];
                output->radius = record.radius * 0.69f;
                ++output;
            }
        }
    }
    FillFirstIndices(dynamicRuns, dynamicCellCount);
}

} // namespace

void BuildMapPackClipRuntimeTables(void *model, std::int32_t mapIndex) {
    if (!model || mapIndex < 0 || mapIndex >= 32)
        return;

    auto *const root = static_cast<ClipNode *>(model);
    g_activeModelBase = MT5_ModelBaseForNode(root);
    std::int32_t recordCapacity = 0;
    CountClipRecordsRecursive(root, recordCapacity);

    g_clipRecords = static_cast<ClipRecord *>(HLib_AllocHeapMemBlock(sizeof(ClipRecord) * (recordCapacity + 1), SHEN_NAME(CLIP)));
    if (mapIndex < 32) {
        g_dynamicMapSlots[mapIndex].clipRuntime = g_clipRecords;
    }

    g_clipRecordCount = 0;
    g_dynamicClipRecordCount = 0;
    g_category4Count = 0;
    g_category8Count = 0;
    g_category12Count = 0;
    g_category16Count = 0;

    DynamicMapAuxiliary &auxiliary = g_dynamicMapAuxiliary[mapIndex];
    auxiliary.clipCells = nullptr;
    auxiliary.clipCellsReady = 0;

    g_staticClipCells = static_cast<std::int16_t *>(HLib_AllocMemBlock(0x800, SHEN_NAME(CTMP)));
    g_dynamicClipCells = static_cast<std::int16_t *>(HLib_AllocMemBlock(0x800, SHEN_NAME(DTMP)));
    g_staticClipCells[0] = 0x5fff;
    g_staticClipCells[1] = 0x5fff;
    g_dynamicClipCells[0] = 0x5fff;
    g_dynamicClipCells[1] = 0x5fff;
    g_activeBlockScale = g_mapBlockScale;

    Matrix4x4::SetActiveTransformToPrePass();
    BuildClipRecordsRecursive(root);
    PopOneTransform();

    if (g_dynamicClipRecordCount) {
        auxiliary.clipCells = HLib_AllocHeapMemBlock(sizeof(DynamicClipCell) * g_dynamicClipRecordCount, SHEN_NAME(DANG));
        auxiliary.clipCellCount = g_dynamicClipRecordCount;
        auxiliary.clipCellsReady = auxiliary.clipCells != nullptr ? 1 : 0;
    }

    FinalizeMapPackClipCellTables(mapIndex);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(g_staticClipCells), SHEN_NAME(CTMP));
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(g_dynamicClipCells), SHEN_NAME(DTMP));

    DynamicMapEntry &map = g_dynamicMapSlots[mapIndex];
    map.clipMetrics[0] = g_clipRecordCount - g_dynamicClipRecordCount;
    map.clipMetrics[1] = g_category16Count;
    map.clipMetrics[2] = g_category8Count;
    map.clipMetrics[3] = g_dynamicClipRecordCount;
    map.clipMetrics[4] = g_category12Count;
    map.clipMetrics[5] = g_category4Count;
}
void DYNM_ClearMapPreparation(std::int32_t mapIndex, bool requireAllocatedStorage) {
    if (mapIndex < 0 || mapIndex >= static_cast<std::int32_t>(g_dynamicMapSlots.size())) {
        return;
    }
    DynamicMapEntry &entry = g_dynamicMapSlots[static_cast<std::size_t>(mapIndex)];
    if (!entry.mapAsset || !entry.mapAsset->model || (requireAllocatedStorage && !entry.prepClone)) {
        return;
    }
    HMDL_ClearPreparationFlagRecursive(entry.mapAsset->model);
    entry.prepReady = 0;
}

void DYNM_ClearAllMapPreparation(bool requireAllocatedStorage) {
    for (std::int32_t mapIndex = 0; mapIndex < static_cast<std::int32_t>(g_dynamicMapSlots.size()); ++mapIndex) {
        DYNM_ClearMapPreparation(mapIndex, requireAllocatedStorage);
    }
}



void DYNM_PrepareMapLighting(std::int32_t mapIndex) {
    if (static_cast<std::uint32_t>(mapIndex) > 31U)
        return;
    std::array<EffectLightRenderSlot, 128> lights{};
    const std::int32_t count = BuildActiveSpotLightRenderParams(lights.data());
    if (!count)
        return;
    TransformPrelightLights(lights.data(), count);
    DynamicMapEntry &entry = g_dynamicMapSlots[static_cast<std::size_t>(mapIndex)];
    if (!entry.mapAsset || !entry.mapAsset->model)
        return;
    PACK_EnsureMapPreparationStorage(mapIndex);
    const DynamicMapStaticTransform &transform = g_dynamicMapStaticTransforms[static_cast<std::size_t>(mapIndex)];
    Matrix4x4_Push();
    Matrix4x4::preTranslate(transform.position.x, transform.position.y, transform.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(transform.yaw));
    HMDL_PrelightSpotLights(entry.mapAsset->model, lights.data(), count);
    Matrix4x4::Pop(1);
    entry.prepReady = 1;
}


void DYNM_PrepareAllMapLighting() {
    std::array<EffectLightRenderSlot, 128> lights{};
    const std::int32_t count = BuildActiveSpotLightRenderParams(lights.data());
    if (!count)
        return;
    TransformPrelightLights(lights.data(), count);
    for (std::int32_t mapIndex = 0; mapIndex < 32; ++mapIndex) {
        DynamicMapEntry &entry = g_dynamicMapSlots[static_cast<std::size_t>(mapIndex)];
        if (!entry.mapAsset || !entry.mapAsset->model)
            continue;
        PACK_EnsureMapPreparationStorage(mapIndex);
        const DynamicMapStaticTransform &transform = g_dynamicMapStaticTransforms[static_cast<std::size_t>(mapIndex)];
        Matrix4x4_Push();
        Matrix4x4::preTranslate(transform.position.x, transform.position.y, transform.position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(transform.yaw));
        HMDL_PrelightSpotLights(entry.mapAsset->model, lights.data(), count);
        Matrix4x4::Pop(1);
        entry.prepReady = 1;
    }
}

void Script_SetStaticSceneObjectTransformSource(ScenarioContext *context, void *arguments) {
    const auto *const params = static_cast<const std::int32_t *>(arguments);
    DynamicMapStaticTransform &transform = g_dynamicMapStaticTransforms.data()[params[0]];
    context->stack->accumulator = transform.sourceMapSlot != 0 ? UINT32_MAX : 0;

    std::int32_t source = params[1];
    if (source <= -1 || !g_dynamicMapSlots.data()[source].mapAsset || !g_dynamicMapSlots.data()[source].mapAsset->model) {
        source = 0x7fff;
    }
    transform.sourceMapSlot = source;
}

void Script_InsertObjectIntoXFormStack(ScenarioContext *context, void *arguments) {
    const auto *const params = static_cast<const std::int32_t *>(arguments);
    DynamicMapStaticTransform &transform = g_dynamicMapStaticTransforms.data()[params[0]];
    context->stack->accumulator = transform.enabled != 0 ? UINT32_MAX : 0;
    if (params[0] < 32)
        transform.enabled = params[1];
}

void Script_SetStaticSceneObjectTransform(ScenarioContext *, void *arguments) {
    const auto *const params = static_cast<const std::int32_t *>(arguments);
    const auto *const position = params[1] ? reinterpret_cast<const Vector3 *>(g_pCurrentTaskData + params[1]) : nullptr;
    DynamicMapStaticTransform *const transform = g_dynamicMapStaticTransforms.data() + params[0];
    if (params[0] < 32)
        transform->position = *position;
    transform->yaw = params[2];
}

void DYNM_TransferRenderEnabledMask(bool apply) {
    if (apply) {
        std::uint32_t mask = g_dynamicMapLodSnapshot.renderEnabledMask;
        for (DynamicMapEntry &map : g_dynamicMapSlots) {
            map.renderEnabled = static_cast<std::int32_t>(mask & 1U);
            mask >>= 1;
        }
        return;
    }
    std::uint32_t mask = 0;
    for (std::uint32_t index = 0; index < 32; ++index) {
        if (g_dynamicMapSlots[index].renderEnabled)
            mask |= 1U << index;
    }
    g_dynamicMapLodSnapshot.renderEnabledMask = mask;
}

void DYNM_TransferClipCellsReadyMask(bool apply) {
    if (apply) {
        std::uint32_t mask = g_dynamicMapLodSnapshot.clipCellsReadyMask;
        for (DynamicMapAuxiliary &map : g_dynamicMapAuxiliary) {
            map.clipCellsReady = map.clipCells ? static_cast<std::int32_t>(mask & 1U) : 0;
            mask >>= 1;
        }
        return;
    }
    std::uint32_t mask = 0;
    for (std::uint32_t index = 0; index < 32; ++index) {
        if (g_dynamicMapAuxiliary[index].clipCellsReady)
            mask |= 1U << index;
    }
    g_dynamicMapLodSnapshot.clipCellsReadyMask = mask;
}

void DYNM_TransferOcclusionEnabledMask(bool apply) {
    if (apply) {
        std::uint32_t mask = g_dynamicMapLodSnapshot.occlusionEnabledMask;
        for (DynamicMapEntry &map : g_dynamicMapSlots) {
            map.occlusionEnabled = mask & 1U;
            mask >>= 1;
        }
        return;
    }
    std::uint32_t mask = 0;
    for (std::uint32_t index = 0; index < 32; ++index) {
        if (g_dynamicMapSlots[index].occlusionEnabled)
            mask |= 1U << index;
    }
    g_dynamicMapLodSnapshot.occlusionEnabledMask = mask;
}

void DYNM_ExecuteLodSnapshotCommand(std::int32_t command) {
    DynamicMapLodSnapshot &snapshot = g_dynamicMapLodSnapshot;
    switch (command) {
    case 2:
        DYNM_TransferRenderEnabledMask(false);
        DYNM_TransferClipCellsReadyMask(false);
        DYNM_TransferOcclusionEnabledMask(false);
        break;
    case 3:
        DYNM_TransferRenderEnabledMask(true);
        DYNM_TransferClipCellsReadyMask(true);
        DYNM_TransferOcclusionEnabledMask(true);
        break;
    case 4:
        snapshot.lodDistance0 = LOD_Distance_0;
        snapshot.npcDisplayRadius = NPCDisplayRadius;
        snapshot.lodDistance2 = LOD_Distance_2;
        break;
    case 5:
        LOD_Distance_0 = snapshot.lodDistance0;
        NPCDisplayRadius = snapshot.npcDisplayRadius;
        LOD_Distance_2 = snapshot.lodDistance2;
        break;
    default:
        break;
    }
}

} // namespace shenmue::s1
