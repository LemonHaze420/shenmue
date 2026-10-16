#pragma once

#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct PackModel;
struct ScenarioContext;

struct DynamicMapClipRecord {
    Vector3 position;
    float radius;
    std::int16_t cellX;
    std::int16_t cellZ;
    std::int32_t field_14;
    void *node;
    std::int32_t category;
    float maxScale;
};

struct DynamicMapClipCellRun {
    std::int16_t cellX;
    std::int16_t cellZ;
    std::int32_t count;
    std::int32_t firstIndex;
};

struct DynamicMapEntry {
    std::int32_t renderEnabled;
    std::int32_t field_04;
    void *clipRuntime;
    std::int32_t sourceKind;
    std::int32_t field_14;
    PackModel *mapAsset;
    std::int32_t sceneObjectCount;
    std::int32_t field_24;
    void *clipCellPointers;
    void *sceneObjectData;
    Vector3 position;
    std::int32_t field_44;
    std::uint32_t staticSceneMode;
    std::uint32_t debugDrawFlag;
    std::uint32_t occlusionEnabled;
    std::int32_t clipMetrics[6];
    void *prepClone;
    std::int32_t prepReady;
    float scale;
    float lightingOverride01;
    std::int32_t sceneSlotValue34;
};

struct DynamicMapAuxiliary {
    void *clipCells;
    std::int32_t clipCellCount;
    std::int32_t clipCellsReady;
    std::int32_t pathCellCount;
    std::int32_t field_14;
    void *pathData;
};

#pragma pack(push, 1)
struct DynamicMapStaticTransform {
    std::uint32_t enabled;
    std::int32_t sourceMapSlot;
    Vector3 position;
    std::int32_t yaw;
    std::int32_t field_18;
};
#pragma pack(pop)

static_assert(sizeof(DynamicMapEntry) == 0x88);
static_assert(sizeof(DynamicMapClipRecord) == 0x28);
static_assert(offsetof(DynamicMapClipRecord, node) == 0x18);
static_assert(sizeof(DynamicMapClipCellRun) == 0x0c);
static_assert(offsetof(DynamicMapEntry, mapAsset) == 0x18);
static_assert(offsetof(DynamicMapEntry, staticSceneMode) == 0x48);
static_assert(offsetof(DynamicMapEntry, debugDrawFlag) == 0x4c);
static_assert(offsetof(DynamicMapEntry, occlusionEnabled) == 0x50);
static_assert(offsetof(DynamicMapEntry, clipMetrics) == 0x54);
static_assert(offsetof(DynamicMapEntry, prepClone) == 0x70);
static_assert(offsetof(DynamicMapEntry, prepReady) == 0x78);
static_assert(offsetof(DynamicMapEntry, lightingOverride01) == 0x80);
static_assert(offsetof(DynamicMapEntry, sceneSlotValue34) == 0x84);



struct DynamicMapLodSnapshot {
    std::uint32_t renderEnabledMask;
    std::uint32_t occlusionEnabledMask;
    std::uint32_t clipCellsReadyMask;
    float lodDistance0;
    float lodDistance2;
    float npcDisplayRadius;
};

static_assert(sizeof(DynamicMapAuxiliary) == 0x20);
static_assert(sizeof(DynamicMapStaticTransform) == 0x1c);
static_assert(offsetof(DynamicMapAuxiliary, pathData) == 0x18);
static_assert(sizeof(DynamicMapLodSnapshot) == 0x18);

extern std::array<DynamicMapEntry, 32> g_dynamicMapSlots;
extern DynamicMapAuxiliary g_dynamicMapAuxiliary[32];
extern float g_mapBlockScale;

extern std::uint32_t g_mapAreaOverrideId;
extern std::array<DynamicMapStaticTransform, 32> g_dynamicMapStaticTransforms;



extern DynamicMapClipRecord *g_clipRecords;
extern std::int32_t g_clipRecordCount;
extern DynamicMapLodSnapshot g_dynamicMapLodSnapshot;



void DYNM_TransferRenderEnabledMask(bool apply);
void DYNM_TransferClipCellsReadyMask(bool apply);
void DYNM_TransferOcclusionEnabledMask(bool apply);
void DYNM_ExecuteLodSnapshotCommand(std::int32_t command);

void BuildMapPackClipRuntimeTables(void *model, std::int32_t mapIndex);
void DYNM_ClearMapPreparation(std::int32_t mapIndex, bool requireAllocatedStorage);
void DYNM_ClearAllMapPreparation(bool requireAllocatedStorage);
void DYNM_PrepareMapLighting(std::int32_t mapIndex);
void DYNM_PrepareAllMapLighting();
void Script_SetStaticSceneObjectTransformSource(ScenarioContext *context, void *arguments);
void Script_InsertObjectIntoXFormStack(ScenarioContext *context, void *arguments);
void Script_SetStaticSceneObjectTransform(ScenarioContext *context, void *arguments);

} // namespace shenmue::s1
