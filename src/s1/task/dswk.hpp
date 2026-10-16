#pragma once

#include "s1/task/char.hpp"
#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

enum DSWKDoorRequestState : std::int32_t {
    DSWK_REQ_FREE = -3,
    DSWK_REQ_NOT_READY = -2,
    DSWK_REQ_FAILED = -1,
    DSWK_REQ_COMPLETE = 0,
    DSWK_REQ_ACTIVE = 1,
    DSWK_REQ_PENDING = 2,
};

enum DSWKTaskState : std::int32_t {
    DSWK_TASK_IDLE = 0,
    DSWK_TASK_PRIME_CANDIDATES = 1,
    DSWK_TASK_ACTIVE_UPDATE = 2,
};

enum DoorCommand : std::int32_t {
    DOOR_CMD_NOP = 0,
    DOOR_CMD_PRIME = 1,
    DOOR_CMD_SET_AND_UPDATE = 2,
    DOOR_CMD_FIND_MAP_ENTRY = 3,
    DOOR_CMD_FIND_BEST_DOOR_CAND = 4,
    DOOR_CMD_FIND_CLOSEST_POINT = 5,
    DOOR_CMD_ALLOC_ENTRY_SLOT = 6,
    DOOR_CMD_FIND_CLOSEST_ELEMENT = 7,
    DOOR_CMD_FIND_CLOSEST_ELEMENT_ANGLE = 8,
    DOOR_CMD_TEST_LINE_INTERSECTION = 9,
    DOOR_CMD_TIMER_START = 10,
    DOOR_CMD_TIMER_GET_DELTA_SCALED = 11,
    DOOR_CMD_SET_IDLE = 12,
    DOOR_CMD_POLL_REQUESTS = 13,
    DOOR_CMD_CALC_VECTOR_FROM_ANGLE = 14,
};

enum DoorBehaviorId : std::int32_t {
    DSWK_DOOR_BEHAVIOR_NONE = 0,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_HANDLE_RIGHT = 1,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_HANDLE_LEFT = 2,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_HANDLE_RIGHT = 3,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_HANDLE_LEFT = 4,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_HANDLE_RIGHT = 5,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_HANDLE_LEFT = 6,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_D_RIGHT = 7,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_D_LEFT = 8,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_LEAF_A_ONLY = 9,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_LEAF_B_ONLY = 10,
    DSWK_DOOR_BEHAVIOR_SINGLE_AUTOMATIC = 11,
    DSWK_DOOR_BEHAVIOR_SINGLE_AUTOMATIC_ALT = 12,
    DSWK_DOOR_BEHAVIOR_SWING_PAIR_SELECTABLE_SINGLE_LEAF = 13,
    DSWK_DOOR_BEHAVIOR_SWING_PAIR_SINGLE_LEAF_ALT = 14,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_SELECTABLE = 15,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_SPECIAL = 16,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_AUTOMATIC = 17,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_B_LEAF_A_ONLY = 18,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_B_LEAF_B_ONLY = 19,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_B_SELECTABLE = 20,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_C_LEAF_A_ONLY = 21,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_C_LEAF_B_ONLY = 22,
    DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_C_SELECTABLE_SINGLE_LEAF = 23,
    DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_A_DIR_4000 = 24,
    DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_A_DIR_C000 = 25,
    DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_B_DIR_4000 = 26,
    DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_B_DIR_C000 = 27,
    DSWK_DOOR_BEHAVIOR_SWING_PAIR_BOTH_LEAVES_SIMULTANEOUS = 28,
    DSWK_DOOR_BEHAVIOR_SPECIAL_SUBTYPE_1D = 29,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_ALT_RIGHT = 30,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_ALT_LEFT = 31,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_ALT_RIGHT = 32,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_ALT_LEFT = 33,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_ALT_RIGHT = 34,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_ALT_LEFT = 35,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_AP_RIGHT = 36,
    DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_AP_LEFT = 37,
};

enum DoorEntryFlags : std::uint32_t {
    DSWK_4 = 0x4,
    DSWK_8 = 0x8,
    DSWK_FRONT = 0x10,
    DSWK_20 = 0x20,
    DSWK_100 = 0x100,
};

#pragma pack(push, 1)
struct DoorResourceDescriptor {
    std::int32_t modelNameOff = 0;
    DoorBehaviorId behaviorId = DSWK_DOOR_BEHAVIOR_NONE;
    std::int32_t openSfxId = 0;
    std::int32_t closeSfxId = 0;
};

struct MapObjectRecord {
    std::int32_t kind = 0;
    std::int32_t resourceIndex = 0;
    Vector3 scale{};
    Vector3 pos{};
    std::int32_t yawFixed16 = 0;
};

struct DoorSlotState {
    std::int32_t mode = 0;
    std::int32_t id = 0;
    std::int32_t entry_idx = 0;
};

struct DoorMotionSelectorBlock {
    char motion_desc_index[10][4]{};
};

struct ScnDoorMotionDescriptor {
    std::int32_t motion_id = 0;
    float frame_open_sfx_begin = 0.0f;
    float frame_phase1_end = 0.0f;
    float frame_close_sfx_begin = 0.0f;
    float frame_motion_end = 0.0f;
    float approach_off_x = 0.0f;
    float approach_off_z = 0.0f;
    float npc_tail_end_frame = 0.0f;
    float npc_tail_delta_per_frame = 0.0f;
};

struct DoorSupportCommandParams {
    DoorCommand command = DOOR_CMD_NOP;
    std::int32_t int4 = 0;
    std::int32_t int8 = 0;
    std::int32_t intC = 0;
};
struct DoorActorOpacityParams {
    std::uint32_t actorKey = 0;
    float opacity = 0.0f;
};

struct DoorEntryConfigParams {
    std::int32_t doorEntryIndex = 0;
    std::uint32_t group = 0;
    std::uint32_t global1 = 0;
    std::uint32_t global2 = 0;
    std::uint32_t global3 = 0;
    std::uint32_t sndCommandId = 0;
    std::uint32_t global5 = 0;
    std::uint32_t global6 = 0;
};

struct DoorSystemInitParams {
    std::int32_t doorEntriesOff = 0;
    std::int32_t doorIdLookupChainsOff = 0;
    std::int32_t resourceNameTableOff = 0;
    std::int32_t mapObjectRecordsOff = 0;
    std::int32_t systemStateOff = 0;
    std::int32_t slotStatesOff = 0;
    std::int32_t candidateDoorEntryIndicesOff = 0;
    std::int32_t ownerActorArrayOff = 0;
    std::int32_t progressArrayOff = 0;
    std::int32_t actorLookupKey = 0;
    std::int32_t maxCandidates = 0;
    std::int32_t auxTable0Off = 0;
    std::int32_t auxTable1Off = 0;
    std::int32_t doorActorHandlesOff = 0;
};

struct DSWKDoorEntry {
    std::int32_t slot = 0;
    DoorEntryFlags flags{};
    std::uint32_t ctrl_mask_pair = 0;
    float coarse_metric = 0.0f;
    float selection_metric = 0.0f;
    std::uint32_t view_ang_delta = 0;
    std::uint32_t group = 0;
    std::uint32_t global1 = 0;
    std::uint32_t global2 = 0;
    std::uint32_t global3 = 0;
    std::uint32_t sndCommandId = 0;
    std::uint32_t global5 = 0;
    std::uint32_t global6 = 0;
};

struct DSWKCandidateSlot {
    std::int32_t doorEntryIndex = -1;
    std::int32_t retainedThisFrame = 0;
    float coarseMetric = 9999999.0f;
    float selectionMetric = 9999999.0f;
    std::int32_t viewAngleDelta = 0;
};

struct DSWKRequestSlot {
    std::int32_t requestedDoorEntryIndex = 0;
    std::int32_t requesterActorId = 0;
    std::int32_t candidateSlotIndex = -1;
    DSWKDoorRequestState state = DSWK_REQ_FREE;
    std::int32_t requestedMotionVariantRow = 0;
    std::int32_t releaseDelayFrames = 0;
};




struct DSWK_t {
    DSWKDoorEntry *doorEntries = nullptr;
    std::int32_t *door_id_lut = nullptr;
    DoorResourceDescriptor *res_name_tbl = nullptr;
    MapObjectRecord *map_object_records = nullptr;
    shen_name *door_actors_by_slot = nullptr;
    DoorSlotState *interactionState = nullptr;
    std::int32_t *doorTaskStateBySlot = nullptr;
    std::int32_t *candDoorEntryIndices = nullptr;
    shen_name *owner_actor_by_slot = nullptr;
    std::int32_t *motion_variant_row_by_slot = nullptr;
    DoorMotionSelectorBlock *mot_selectors = nullptr;
    ScnDoorMotionDescriptor *mot_descs = nullptr;
    Character *actor = nullptr;
    std::int32_t lookup_key = 0;
    DSWKTaskState state = DSWK_TASK_IDLE;
    std::int32_t metric_cur = 0;
    std::int32_t maxCandidates = 10;
    std::int32_t numEntries = 0;
    std::int32_t candidateCount = 0;
    DSWKCandidateSlot slots[10]{};
    DSWKRequestSlot requestSlots[16]{};
};
#pragma pack(pop)

static_assert(sizeof(DoorResourceDescriptor) == 0x10);
static_assert(sizeof(DoorSupportCommandParams) == 0x10);
static_assert(sizeof(DoorActorOpacityParams) == 0x8);
static_assert(sizeof(DoorEntryConfigParams) == 0x20);
static_assert(sizeof(DoorSystemInitParams) == 0x38);
static_assert(sizeof(MapObjectRecord) == 0x24);
static_assert(sizeof(DSWKDoorEntry) == 0x34);
static_assert(sizeof(DSWKCandidateSlot) == 0x14);
static_assert(sizeof(DSWKRequestSlot) == 0x18);
static_assert(offsetof(DSWK_t, state) == 0x6c);
static_assert(offsetof(DSWK_t, slots) == 0x80);
static_assert(offsetof(DSWK_t, requestSlots) == 0x148);
static_assert(sizeof(DSWK_t) == 0x2c8);


extern std::int32_t g_dswk_tickTime;
extern HLib::Task *g_doorSystemTask;

void InitDoorSystemTask();
void DoorSystemTask_Main(HLib::TaskData *taskData);
std::byte *DSWK_CurrentTaskDataAt(std::int32_t offset, std::size_t minSize);
void DSWK_BindTables(DSWK_t &doorSupport, const DoorSystemInitParams &params, HLib::Task *scenarioTask);
std::int32_t DSWK_FindMapObjectRecord(const DSWK_t &doorSupport, std::int32_t doorEntryIndex, std::int32_t kind);
void DSWK_Process(DSWK_t &doorSupport);
std::int32_t DSWK_ExecuteCommand(DSWK_t &doorSupport, const DoorSupportCommandParams &params);
std::int32_t DSWK_FindNearestDoorEntry(const DSWK_t &doorSupport, const Vector3 &position);
std::int32_t DSWK_ComputeApproachPose(const DSWK_t &doorSupport, std::int32_t doorEntryIndex, Vector3 &position, std::int32_t &yaw);
bool DSWK_DoorRequestTablesReady(const DSWK_t &doorSupport);
bool DSWK_TryComputeNearestDoorApproachPose(const DSWK_t &doorSupport, Vector3 &position, std::int32_t &yaw);
std::int32_t DSWK_AllocateDoorRequest(DSWK_t &doorSupport, std::uint32_t actorId, const Vector3 &position, std::int32_t motionId);
DSWKDoorRequestState DSWK_PollDoorRequest(DSWK_t &doorSupport, std::int32_t requestSlot);
bool DSWK_TestDoorLineIntersection(const Vector3 &start, const Vector3 &end, float radius);
bool DSWK_PublishDoorActor(Character &character, const DoorResourceDescriptor &resource);

struct ScenarioContext;
void DSWK_BindTablesAndInitState(ScenarioContext *context, void *params);
void DSWK_DoorUtils(ScenarioContext *context, void *params);
void DSWK_RebindDoorSystemTables(ScenarioContext *context, void *params);
void DSWK_AllocDoorMapcRenderState(ScenarioContext *context, void *params);
void DSWK_SetActorOpacityAndDrawCallback(ScenarioContext *context, void *params);
void DSWK_AssignDoorEntryConfig(ScenarioContext *context, void *params);
void DSWK_GetSettingsFlags(ScenarioContext *context, void *params);

} // namespace shenmue::s1
