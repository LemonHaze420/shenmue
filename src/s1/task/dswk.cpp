#include "s1/task/dswk.hpp"

#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/save_format.hpp"
#include "s1/timing.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/srch.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <limits>

namespace shenmue::s1 {

std::int32_t g_dswk_tickTime = 0;
HLib::Task *g_doorSystemTask = nullptr;

namespace {

constexpr float kTwoPi = 6.28318530717958647692f;
constexpr float kFixedToRadians = kTwoPi / 65536.0f;
constexpr float kMetricNotVisible = 99999.898f;
constexpr float kMetricOccluded = 99999.0f;
constexpr float kCandidateMetricEmpty = 9999999.0f;

std::int32_t Atan2Fixed(float x, float z) {
    return static_cast<std::int16_t>(atan2_fixed16(x, z));
}

std::int32_t AbsFixedDelta(std::int32_t lhs, std::int32_t rhs) {
    const std::int32_t delta = static_cast<std::int16_t>(lhs - rhs);
    return delta < 0 ? -delta : delta;
}

float DistanceSquared(const Vector3 &lhs, const Vector3 &rhs) {
    const float dx = lhs.x - rhs.x;
    const float dy = lhs.y - rhs.y;
    const float dz = lhs.z - rhs.z;
    return dx * dx + dy * dy + dz * dz;
}

Vector3 CharacterPosition(const Character *character) {
    return character ? GetPositionForEntity(const_cast<CharacterNode &>(character->entity.node)) : Vector3{};
}

MapControlData *ControlData() {
    return g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}


Character *LookupCharacter(HLib::Task *scenarioTask, std::uint32_t actorId) {
    return actorId && scenarioTask ? CHID_Find(scenarioTask, actorId) : nullptr;
}

Character *LookupControlCharacter(std::uint32_t actorId) {
    const MapControlData *const control = ControlData();
    return LookupCharacter(control ? control->scenarioTask : nullptr, actorId);
}



const std::byte *ScenarioBase() {
    return g_pCurrentTaskData;
}

bool ScenarioRangeContains(std::int32_t offset, std::size_t) {
    return offset > 0 && ScenarioBase();
}

template <typename T> T *ScenarioPointer(std::int32_t offset) {
    return ScenarioRangeContains(offset, sizeof(T)) ? reinterpret_cast<T *>(const_cast<std::byte *>(ScenarioBase()) + offset) : nullptr;
}

template <typename T> const T *ScenarioInfoPointer(std::int32_t offset) {
    if (offset <= 0 || !g_pSCN_INFO_TaskData)
        return nullptr;
    return reinterpret_cast<const T *>(reinterpret_cast<const std::byte *>(g_pSCN_INFO_TaskData) + offset);
}

const DoorResourceDescriptor *ResourceFor(const DSWK_t &doorSupport, const MapObjectRecord &record) {

    return doorSupport.res_name_tbl ? doorSupport.res_name_tbl + record.resourceIndex : nullptr;
}

float BaseApproachSide(DoorBehaviorId behavior) {
    switch (behavior) {
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_HANDLE_RIGHT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_HANDLE_RIGHT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_HANDLE_RIGHT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_D_RIGHT:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_LEAF_A_ONLY:
    case DSWK_DOOR_BEHAVIOR_SINGLE_AUTOMATIC:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_B_LEAF_A_ONLY:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_C_LEAF_A_ONLY:
    case DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_A_DIR_C000:
    case DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_B_DIR_C000:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_ALT_RIGHT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_ALT_RIGHT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_ALT_RIGHT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_AP_RIGHT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_AP_LEFT:
        return 0.44999999f;

    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_HANDLE_LEFT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_HANDLE_LEFT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_HANDLE_LEFT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_D_LEFT:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_LEAF_B_ONLY:
    case DSWK_DOOR_BEHAVIOR_SINGLE_AUTOMATIC_ALT:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_B_LEAF_B_ONLY:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_C_LEAF_B_ONLY:
    case DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_A_DIR_4000:
    case DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_B_DIR_4000:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_A_ALT_LEFT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_B_ALT_LEFT:
    case DSWK_DOOR_BEHAVIOR_SINGLE_MANUAL_SUBTYPE_C_ALT_LEFT:
        return -0.44999999f;

    default:
        return 0.0f;
    }
}

bool UsesDynamicApproachSide(DoorBehaviorId behavior) {
    switch (behavior) {
    case DSWK_DOOR_BEHAVIOR_SWING_PAIR_SELECTABLE_SINGLE_LEAF:
    case DSWK_DOOR_BEHAVIOR_SWING_PAIR_SINGLE_LEAF_ALT:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_SELECTABLE:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_A_SPECIAL:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_AUTOMATIC:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_B_SELECTABLE:
    case DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_C_SELECTABLE_SINGLE_LEAF:
    case DSWK_DOOR_BEHAVIOR_SWING_PAIR_BOTH_LEAVES_SIMULTANEOUS:
    case DSWK_DOOR_BEHAVIOR_SPECIAL_SUBTYPE_1D:
        return true;
    default:
        return false;
    }
}

bool ComputeApproachPoint(const MapObjectRecord &record, const DoorResourceDescriptor *resource, Vector3 &output) {
    const DoorBehaviorId behavior = resource ? resource->behaviorId : DSWK_DOOR_BEHAVIOR_NONE;
    const bool dynamic = UsesDynamicApproachSide(behavior);
    const float side = dynamic ? 0.0f : BaseApproachSide(behavior);
    output = record.pos;
    if (!dynamic) {
        output.x += cos_fixed16(static_cast<std::uint16_t>(record.yawFixed16)) * side;
        output.z -= sin_fixed16(static_cast<std::int16_t>(record.yawFixed16)) * side;
    }
    return dynamic;
}

float ResolveApproachPointAndSide(const MapObjectRecord &record, const DoorResourceDescriptor *resource, const Vector3 &source, Vector3 &output) {

    const std::int32_t negativeYaw = static_cast<std::int16_t>(-static_cast<std::int16_t>(record.yawFixed16));

    if (ComputeApproachPoint(record, resource, output)) {
        const float sideTest = cos_fixed16(static_cast<std::uint16_t>(negativeYaw)) * (source.x - record.pos.x) +
            sin_fixed16(static_cast<std::int16_t>(negativeYaw)) * (source.z - record.pos.z);
        const float lateral = sideTest >= 0.0f ? 0.44999999f : -0.44999999f;
        output.x += cos_fixed16(static_cast<std::uint16_t>(record.yawFixed16)) * lateral;
        output.z -= sin_fixed16(static_cast<std::int16_t>(record.yawFixed16)) * lateral;
    }
    return cos_fixed16(static_cast<std::uint16_t>(negativeYaw)) * (source.z - record.pos.z) -
        sin_fixed16(static_cast<std::int16_t>(negativeYaw)) * (source.x - record.pos.x);
}

void ResetCandidateSlots(DSWK_t &doorSupport) {
    for (DSWKCandidateSlot &slot : doorSupport.slots) {
        slot.doorEntryIndex = -1;
        slot.retainedThisFrame = 0;
        slot.coarseMetric = kCandidateMetricEmpty;
        slot.selectionMetric = kCandidateMetricEmpty;
        slot.viewAngleDelta = 0;
    }
}

void ResetRequestSlots(DSWK_t &doorSupport) {
    for (DSWKRequestSlot &request : doorSupport.requestSlots) {
        request.requestedDoorEntryIndex = 0;
        request.requesterActorId = 0;
        request.candidateSlotIndex = -1;
        request.state = DSWK_REQ_FREE;
        request.requestedMotionVariantRow = 0;
        request.releaseDelayFrames = 5;
    }
}

void ResetEntriesAndPublishCtrlMasks(DSWK_t &doorSupport) {
    if (!doorSupport.doorEntries) {
        doorSupport.numEntries = 0;
        doorSupport.candidateCount = 0;
        return;
    }

    std::int32_t count = 0;
    while (doorSupport.doorEntries[count].slot != -1) {
        DSWKDoorEntry &entry = doorSupport.doorEntries[count];
        const std::uint32_t masks = entry.ctrl_mask_pair;
        entry.flags = static_cast<DoorEntryFlags>(0);
        entry.coarse_metric = 0.0f;
        entry.selection_metric = 0.0f;
        entry.view_ang_delta = 0;
        entry.group = 0;
        entry.global1 = std::numeric_limits<std::uint32_t>::max();
        entry.global2 = std::numeric_limits<std::uint32_t>::max();
        entry.global3 = std::numeric_limits<std::uint32_t>::max();
        entry.sndCommandId = std::numeric_limits<std::uint32_t>::max();
        entry.global5 = std::numeric_limits<std::uint32_t>::max();
        entry.global6 = std::numeric_limits<std::uint32_t>::max();


        for (const std::uint32_t index : {masks & 0xffffU, masks >> 16U}) {
            MapControlData *const control = ControlData();
            if (index < 0x100U && control) {
                auto *const cycleMask = reinterpret_cast<std::uint8_t *>(control->cycleMask);
                cycleMask[index >> 3U] = static_cast<std::uint8_t>(cycleMask[index >> 3U] | (1U << (index & 7U)));
            }
        }
        ++count;
    }
    doorSupport.numEntries = count;
    if (count < doorSupport.maxCandidates)
        doorSupport.maxCandidates = count;
    doorSupport.candidateCount = std::min<std::int32_t>(10, count);
}

void UpdateEntryMetrics(DSWK_t &doorSupport, std::int32_t mode) {
    if (!doorSupport.doorEntries || !doorSupport.door_id_lut || !doorSupport.map_object_records || doorSupport.numEntries <= 0) {
        return;
    }

    std::int32_t metric = 0;
    std::int32_t updateCount = 0;
    if (mode == 1) {
        metric = 0;
        updateCount = doorSupport.numEntries;
    } else if (mode != 0) {
        metric = mode;
        updateCount = mode;
    } else {
        metric = doorSupport.metric_cur;
        updateCount = doorSupport.maxCandidates;
    }

    const Character *actor = doorSupport.actor;
    const Vector3 actorPosition = CharacterPosition(actor);
    Vector3 cameraEye = g_CameraEyePos;
    cameraEye.y = actorPosition.y;

    const std::int32_t cameraYaw = Atan2Fixed(-(g_CameraTargetPos.x - g_CameraEyePos.x), -(g_CameraTargetPos.z - g_CameraEyePos.z));
    const std::int32_t actorYaw = actor ? charaGetWorldYaw(const_cast<Character *>(actor)) : 0;

    for (std::int32_t i = 0; i < updateCount; ++i) {
        std::int32_t entryIndex = 0;
        DSWKDoorEntry *entry = doorSupport.doorEntries;
        if (metric < doorSupport.numEntries) {
            entryIndex = metric;
            entry = doorSupport.doorEntries + metric;
        }
        const std::int32_t mapIndex = DSWK_FindMapObjectRecord(doorSupport, entryIndex, 2);
        if (mapIndex >= 0) {
            const MapObjectRecord &record = doorSupport.map_object_records[mapIndex];
            Vector3 approach{};
            ComputeApproachPoint(record, ResourceFor(doorSupport, record), approach);
            if (!shenIsLocalSphereInsideViewCullVolume(approach, 2.5f)) {
                entry->coarse_metric = kMetricNotVisible;
                entry->selection_metric = kMetricNotVisible;
            } else if (!shenIsLocalSphereVisibleInScreenDepthTable(approach, 2.5f)) {
                entry->coarse_metric = kMetricOccluded;
                entry->selection_metric = kMetricOccluded;
            } else {

                const std::int32_t viewDelta = AbsFixedDelta(cameraYaw, Atan2Fixed(-(approach.x - cameraEye.x), -(approach.z - cameraEye.z)));
                entry->view_ang_delta = static_cast<std::uint32_t>(viewDelta);

                const float degrees = static_cast<float>(viewDelta) / 182.04445f;
                const float correction = degrees >= 45.0f ? (degrees / 45.0f) * 20.0f : 1.0f;
                entry->coarse_metric = DistanceSquared(approach, cameraEye) * correction;

                Vector3 resolved{};
                const float approachDepth = ResolveApproachPointAndSide(record, ResourceFor(doorSupport, record), actorPosition, resolved);
                const float side = approachDepth >= 0.0f ? 0.40000001f : -0.40000001f;
                resolved.x += sin_fixed16(static_cast<std::int16_t>(record.yawFixed16)) * side;
                resolved.z += cos_fixed16(static_cast<std::uint16_t>(record.yawFixed16)) * side;

                const std::int32_t actorDelta = AbsFixedDelta(actorYaw, Atan2Fixed(-(resolved.x - actorPosition.x), -(resolved.z - actorPosition.z)));
                const float facing = cos_fixed16(static_cast<std::uint16_t>(actorDelta));
                const float facingScale = facing >= 0.0f ? facing : facing * -0.3f;
                const float divisor = facingScale * 0.55f + 0.45f;
                entry->selection_metric = DistanceSquared(resolved, actorPosition) / (divisor * divisor);
            }
        }
        metric = entryIndex + 1;
    }
    if (mode == 0)
        doorSupport.metric_cur = metric;
}

void RebuildBestCandidateSlots(DSWK_t &doorSupport) {
    if (!doorSupport.doorEntries || doorSupport.numEntries <= 0 || doorSupport.candidateCount <= 0) {
        return;
    }

    const std::int32_t count = std::min<std::int32_t>(doorSupport.candidateCount, 10);
    for (std::int32_t entryIndex = 0; entryIndex < doorSupport.numEntries; ++entryIndex) {

        const DSWKDoorEntry &entry = doorSupport.doorEntries[entryIndex];
        float coarse = entry.coarse_metric;
        float selection = entry.selection_metric;
        std::int32_t viewDelta = static_cast<std::int32_t>(entry.view_ang_delta);
        if (doorSupport.slots[0].coarseMetric <= coarse)
            continue;

        std::int32_t insertedEntry = entryIndex;
        DSWKCandidateSlot *slot = &doorSupport.slots[count - 1];
        std::int32_t scanned = 0;
        while (scanned < count) {
            const std::int32_t oldEntry = slot->doorEntryIndex;
            if (oldEntry == insertedEntry)
                break;

            if (slot->coarseMetric > coarse) {
                std::swap(slot->viewAngleDelta, viewDelta);
                std::swap(slot->coarseMetric, coarse);
                std::swap(slot->selectionMetric, selection);
                std::swap(slot->doorEntryIndex, insertedEntry);
                scanned = 0;
                slot = &doorSupport.slots[count - 1];
            }
            ++scanned;
            --slot;
        }
    }
}

Mt5RuntimeNode *FindModelNode(Mt5RuntimeNode *node, std::byte *base, std::uint16_t kind, std::int32_t depth = 0) {
    if (!node || !base || depth >= 64)
        return nullptr;

    std::int32_t guard = 0;
    while (node && guard++ < 4096) {
        if (node->nodeKind == kind)
            return node;

        if (node->childOffset) {
            if (Mt5RuntimeNode *found = FindModelNode(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset), base, kind, depth + 1)) {
                return found;
            }
        }
        node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset) : nullptr;
    }
    return nullptr;
}

Mt5RuntimeNode *FindModelNode(void *model, std::uint16_t kind) {
    auto *root = static_cast<Mt5RuntimeNode *>(model);
    return root ? FindModelNode(root, MT5_ModelBaseForNode(root), kind) : nullptr;
}

void SetNodePosition(void *model, std::uint16_t kind, const Vector3 &position) {
    if (Mt5RuntimeNode *node = FindModelNode(model, kind))
        node->position = position;
}

void ApplyDoorModelOffsets(Character &character, const DSWKDoorEntry &entry, const DoorResourceDescriptor &resource) {
    if ((resource.behaviorId == DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_B_SELECTABLE ||
            resource.behaviorId == DSWK_DOOR_BEHAVIOR_SLIDING_PAIR_SUBTYPE_C_SELECTABLE_SINGLE_LEAF)) {
        if ((entry.flags & DSWK_FRONT) != 0) {
            if (Mt5RuntimeNode *node = FindModelNode(character.model, 7))
                node->position.x += 0.75f;
        } else if ((entry.flags & DSWK_8) != 0) {
            if (Mt5RuntimeNode *node = FindModelNode(character.model, 12))
                node->position.x -= 0.75f;
        }
    }

    if ((resource.behaviorId == DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_A_DIR_4000 ||
            resource.behaviorId == DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_B_DIR_C000) &&
        (entry.flags & DSWK_FRONT) != 0) {
        character.entity.node.rotation.y -= 15958;
    } else if ((resource.behaviorId == DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_A_DIR_C000 ||
                   resource.behaviorId == DSWK_DOOR_BEHAVIOR_SWING_SINGLE_SUBTYPE_B_DIR_4000) &&
        (entry.flags & DSWK_FRONT) != 0) {
        character.entity.node.rotation.y += 15958;
    }

    const bool has7 = FindModelNode(character.model, 7) != nullptr;
    const bool has5 = FindModelNode(character.model, 5) != nullptr;
    const bool has10 = FindModelNode(character.model, 10) != nullptr;

    if ((has7 && has5 && has10) || (!has7 && has10)) {
        const float y = (entry.flags & DSWK_100) != 0 ? 0.0f : -1200.0f;
        SetNodePosition(character.model, 10, {0.0f, y, 0.0f});
        if (has7)
            SetNodePosition(character.model, 5, {0.0f, y, 0.0f});
    }
    if (FindModelNode(character.model, 8) && FindModelNode(character.model, 13)) {

        const bool flag20 = (entry.flags & DSWK_20) != 0;
        SetNodePosition(character.model, 13, {0.0f, flag20 ? 0.0f : -1200.0f, 0.0f});
        SetNodePosition(character.model, 8, {0.0f, flag20 ? -1200.0f : 0.0f, 0.0f});
    }
}

void ApplyDoorActorRecord(DSWK_t &doorSupport, std::int32_t entryIndex, Character *character) {
    if (!character || entryIndex < 0 || entryIndex >= doorSupport.numEntries)
        return;

    const std::int32_t recordIndex = DSWK_FindMapObjectRecord(doorSupport, entryIndex, 2);
    if (recordIndex < 0) {
        character->motionFlags &= static_cast<std::uint8_t>(~4U);
        return;
    }

    const MapObjectRecord &record = doorSupport.map_object_records[recordIndex];
    character->entity.node.position = record.pos;
    character->entity.node.rotation = {0, static_cast<std::int16_t>(record.yawFixed16), 0};
    character->scale = record.scale;
    character->skipRootMotionDelta = 1;
    character->clipAngle = 2.5f;

    if (const DoorResourceDescriptor *resource = ResourceFor(doorSupport, record)) {
        DSWK_PublishDoorActor(*character, *resource);
        ApplyDoorModelOffsets(*character, doorSupport.doorEntries[entryIndex], *resource);
    }
    character->motionFlags |= 4U;
    character->flags.value &= ~CharacterFlagWithinVisibleDistance;
}

void RefreshVisibleActors(DSWK_t &doorSupport) {
    if (!doorSupport.candDoorEntryIndices || !doorSupport.doorTaskStateBySlot || doorSupport.candidateCount <= 0)
        return;

    const std::int32_t count = std::min<std::int32_t>(doorSupport.candidateCount, 10);
    for (std::int32_t i = 0; i < count; ++i) {
        doorSupport.slots[i].retainedThisFrame = 0;
        if (doorSupport.doorTaskStateBySlot[i] <= 2)
            doorSupport.doorTaskStateBySlot[i] = 0;
    }
    for (std::int32_t assigned = 0; assigned < count; ++assigned) {
        const std::int32_t entryIndex = doorSupport.candDoorEntryIndices[assigned];
        if (entryIndex == -1)
            continue;

        for (std::int32_t slot = 0; slot < count; ++slot) {
            if (doorSupport.slots[slot].doorEntryIndex != entryIndex)
                continue;

            if (doorSupport.doorTaskStateBySlot[assigned] <= 2)
                doorSupport.doorTaskStateBySlot[assigned] = 1;
            doorSupport.slots[slot].retainedThisFrame = 1;
            break;
        }
    }
    DSWKCandidateSlot *candidate = &doorSupport.slots[count - 1];
    for (std::int32_t processed = 0; processed < count; ++processed, --candidate) {

        if (candidate->retainedThisFrame == 1)
            continue;

        std::int32_t openSlot = 0;
        while (openSlot < count && doorSupport.doorTaskStateBySlot[openSlot] >= 1) {
            ++openSlot;
        }

        if (openSlot >= count)
            continue;

        const std::int32_t entryIndex = candidate->doorEntryIndex;
        if (entryIndex < 0 || entryIndex >= doorSupport.numEntries)
            continue;

        doorSupport.candDoorEntryIndices[openSlot] = entryIndex;
        Character *character = doorSupport.door_actors_by_slot ? LookupControlCharacter(doorSupport.door_actors_by_slot[openSlot].name) : nullptr;
        const std::int32_t recordIndex = DSWK_FindMapObjectRecord(doorSupport, entryIndex, 2);
        if (recordIndex < 0) {
            if (character)
                character->motionFlags &= static_cast<std::uint8_t>(~4U);
            doorSupport.doorTaskStateBySlot[openSlot] = 1;
            continue;
        }

        ApplyDoorActorRecord(doorSupport, entryIndex, character);
        if (doorSupport.owner_actor_by_slot)
            doorSupport.owner_actor_by_slot[openSlot].name = 0;
        doorSupport.doorTaskStateBySlot[openSlot] = 1;
    }
}

std::int32_t FindBestCandidateSlot(const DSWK_t &doorSupport) {
    if (!doorSupport.candDoorEntryIndices || doorSupport.candidateCount <= 0)
        return -1;

    float best = kMetricOccluded;
    std::int32_t bestSlot = -1;
    for (std::int32_t i = 0; i < doorSupport.candidateCount; ++i) {
        const std::int32_t entry = doorSupport.candDoorEntryIndices[i];
        if (entry < 0 || entry >= doorSupport.numEntries)
            continue;

        if (doorSupport.doorEntries[entry].selection_metric < best) {
            best = doorSupport.doorEntries[entry].selection_metric;
            bestSlot = i;
        }
    }

    if (bestSlot < 0)
        return -1;

    (void)DSWK_FindMapObjectRecord(doorSupport, doorSupport.candDoorEntryIndices[bestSlot], 2);
    if (SRCH_ExecuteCommand(SearchCommand::ReturnFocused) != 0U || SCEN_BlocksDoorCandidate()) {
        return -1;
    }
    return best <= 1.0f ? bestSlot : -1;
}

std::int32_t FindNearestCandidateSlot(const DSWK_t &doorSupport, const Vector3 &position) {
    if (!doorSupport.candDoorEntryIndices || doorSupport.candidateCount <= 0)
        return -1;

    float best = kMetricOccluded;
    std::int32_t nearest = -1;
    for (std::int32_t i = 0; i < doorSupport.candidateCount; ++i) {
        const std::int32_t entry = doorSupport.candDoorEntryIndices[i];
        const std::int32_t mapIndex = DSWK_FindMapObjectRecord(doorSupport, entry, 2);
        if (mapIndex < 0)
            continue;

        const float distance = DistanceSquared(position, doorSupport.map_object_records[mapIndex].pos);
        if (distance < best) {
            best = distance;
            nearest = i;
        }
    }
    return nearest;
}

std::int32_t FindCandidateSlotForEntry(const DSWK_t &doorSupport, std::int32_t entry) {
    if (!doorSupport.candDoorEntryIndices || entry < 0)
        return -1;

    const std::int32_t count = std::min<std::int32_t>(doorSupport.candidateCount, 10);
    for (std::int32_t i = 0; i < count; ++i) {
        if (doorSupport.candDoorEntryIndices[i] == entry)
            return i;
    }
    return -1;
}

std::int32_t ComputeCandidateApproachPose(const DSWK_t &doorSupport, std::int32_t candidateSlot, Vector3 &position, std::int32_t &yaw) {
    if (!doorSupport.candDoorEntryIndices || candidateSlot < 0 || candidateSlot >= doorSupport.candidateCount) {
        position = {};
        yaw = 0;
        return 0;
    }

    const std::int32_t entry = doorSupport.candDoorEntryIndices[candidateSlot];
    const std::int32_t mapIndex = DSWK_FindMapObjectRecord(doorSupport, entry, 2);
    if (mapIndex < 0) {
        position = {};
        yaw = 0;
        return 0;
    }

    const MapObjectRecord &record = doorSupport.map_object_records[mapIndex];
    Vector3 output{};
    const float side = ResolveApproachPointAndSide(record, ResourceFor(doorSupport, record), position, output);
    yaw = side >= 0.0f ? record.yawFixed16 : record.yawFixed16 + 0x8000;
    position = output;
    return side >= 0.0f ? -1 : 1;
}

std::int32_t MotionVariantRow(std::int32_t motionId) {
    switch (motionId) {
    case 3:
        return 1;
    case 4:
        return 2;
    case 7:
        return 3;
    case 8:
        return 4;
    case 10:
        return 5;
    case 19:
        return 6;
    case 15:
        return 7;
    case 12:
        return 8;
    default:
        return 9;
    }
}

std::int32_t AllocateRequest(DSWK_t &doorSupport, std::int32_t entry, std::uint32_t actorId, std::int32_t variantRow) {

    for (std::size_t i = 0; i < std::size(doorSupport.requestSlots); ++i) {
        DSWKRequestSlot &request = doorSupport.requestSlots[i];
        if (request.state != DSWK_REQ_FREE)
            continue;

        request.requestedDoorEntryIndex = entry;
        request.requesterActorId = static_cast<std::int32_t>(actorId);
        request.state = DSWK_REQ_PENDING;
        request.requestedMotionVariantRow = variantRow;
        request.releaseDelayFrames = 5;
        return static_cast<std::int32_t>(i);
    }
    return -1;
}

void ProcessRequestSlots(DSWK_t &doorSupport) {
    for (DSWKRequestSlot &request : doorSupport.requestSlots) {
        if (request.state < DSWK_REQ_NOT_READY)
            continue;

        if (request.state > DSWK_REQ_COMPLETE) {
            if (request.state == DSWK_REQ_ACTIVE) {
                const std::uint32_t owner = doorSupport.owner_actor_by_slot[request.candidateSlotIndex].name;
                if (owner == 0U) {
                    request.state = DSWK_REQ_COMPLETE;
                } else if (owner != static_cast<std::uint32_t>(request.requesterActorId)) {
                    request.state = DSWK_REQ_FAILED;
                }
                continue;
            }

            std::int32_t candidate = -1;
            bool found = false;
            if (request.requestedDoorEntryIndex >= 0) {
                for (std::int32_t i = 0; i < doorSupport.candidateCount; ++i) {
                    candidate = i;
                    if (doorSupport.candDoorEntryIndices[i] == request.requestedDoorEntryIndex) {
                        found = true;
                        break;
                    }
                }
            }
            request.candidateSlotIndex = candidate;
            if (!found) {
                request.state = DSWK_REQ_FAILED;
                continue;
            }

            if (doorSupport.doorTaskStateBySlot[candidate] != 1) {
                request.state = DSWK_REQ_NOT_READY;
                continue;
            }

            if ((doorSupport.doorEntries[request.requestedDoorEntryIndex].flags & DSWK_4) != 0) {
                request.state = DSWK_REQ_COMPLETE;
                continue;
            }

            doorSupport.doorTaskStateBySlot[candidate] = 5;
            doorSupport.owner_actor_by_slot[candidate].name = static_cast<std::uint32_t>(request.requesterActorId);
            doorSupport.motion_variant_row_by_slot[candidate] = request.requestedMotionVariantRow;
            request.state = DSWK_REQ_ACTIVE;
            continue;
        }

        if (request.releaseDelayFrames <= 0) {
            request.state = DSWK_REQ_FREE;
            request.releaseDelayFrames = 5;
        } else if (!LookupControlCharacter(static_cast<std::uint32_t>(request.requesterActorId))) {
            request.releaseDelayFrames = 0;
        }
    }
}

DSWK_t *DoorTaskData() {
    return g_doorSystemTask && g_doorSystemTask->task_data_ptr ? reinterpret_cast<DSWK_t *>(g_doorSystemTask->task_data_ptr) : nullptr;
}

void SetScenarioResult(ScenarioContext *context, std::uint32_t result) {
    if (context && context->stack)
        context->stack->accumulator = result;
}

} // namespace

std::byte *DSWK_CurrentTaskDataAt(std::int32_t offset, std::size_t minSize) {
    if (offset < 0 || !g_doorSystemTask || !g_doorSystemTask->task_data_ptr)
        return nullptr;
    const std::size_t raw = static_cast<std::size_t>(offset);
    if (raw > sizeof(DSWK_t) || minSize > sizeof(DSWK_t) - raw)
        return nullptr;
    return reinterpret_cast<std::byte *>(g_doorSystemTask->task_data_ptr) + raw;
}

void DSWK_BindTables(DSWK_t &doorSupport, const DoorSystemInitParams &params, HLib::Task *scenarioTask) {
    doorSupport.doorEntries = ScenarioPointer<DSWKDoorEntry>(params.doorEntriesOff);
    doorSupport.door_id_lut = ScenarioPointer<std::int32_t>(params.doorIdLookupChainsOff);
    doorSupport.res_name_tbl = ScenarioPointer<DoorResourceDescriptor>(params.resourceNameTableOff);
    doorSupport.map_object_records = ScenarioPointer<MapObjectRecord>(params.mapObjectRecordsOff);
    doorSupport.interactionState = ScenarioPointer<DoorSlotState>(params.systemStateOff);
    doorSupport.doorTaskStateBySlot = ScenarioPointer<std::int32_t>(params.slotStatesOff);
    doorSupport.candDoorEntryIndices = ScenarioPointer<std::int32_t>(params.candidateDoorEntryIndicesOff);
    doorSupport.owner_actor_by_slot = ScenarioPointer<shen_name>(params.ownerActorArrayOff);
    doorSupport.motion_variant_row_by_slot = ScenarioPointer<std::int32_t>(params.progressArrayOff);
    doorSupport.mot_selectors = ScenarioPointer<DoorMotionSelectorBlock>(params.auxTable0Off);
    doorSupport.mot_descs = ScenarioPointer<ScnDoorMotionDescriptor>(params.auxTable1Off);
    doorSupport.door_actors_by_slot = ScenarioPointer<shen_name>(params.doorActorHandlesOff);
    doorSupport.actor = LookupCharacter(scenarioTask, static_cast<std::uint32_t>(params.actorLookupKey));
    doorSupport.lookup_key = params.actorLookupKey;
    doorSupport.state = DSWK_TASK_IDLE;
    doorSupport.metric_cur = 0;
    doorSupport.maxCandidates = params.maxCandidates;
    doorSupport.numEntries = 0;
    doorSupport.candidateCount = 0;
    ResetEntriesAndPublishCtrlMasks(doorSupport);
    ResetCandidateSlots(doorSupport);
    ResetRequestSlots(doorSupport);
}

std::int32_t DSWK_FindMapObjectRecord(const DSWK_t &doorSupport, std::int32_t doorEntryIndex, std::int32_t kind) {
    if (!doorSupport.doorEntries || !doorSupport.door_id_lut || !doorSupport.map_object_records || doorEntryIndex < 0 ||
        doorEntryIndex >= doorSupport.numEntries) {
        return -1;
    }
    const std::int32_t slot = doorSupport.doorEntries[doorEntryIndex].slot;
    if (slot < 0)
        return -1;
    const std::int32_t *lut = doorSupport.door_id_lut + slot;
    while (*lut != -1) {
        const std::int32_t mapIndex = *lut++;
        if (doorSupport.map_object_records[mapIndex].kind == kind)
            return mapIndex;
    }
    return -1;
}

std::int32_t DSWK_FindNearestDoorEntry(const DSWK_t &doorSupport, const Vector3 &position) {
    if (!doorSupport.doorEntries || !doorSupport.map_object_records)
        return -1;

    float best = kMetricOccluded;
    std::int32_t nearest = -1;
    for (std::int32_t entry = 0; entry < doorSupport.numEntries; ++entry) {
        const std::int32_t mapIndex = DSWK_FindMapObjectRecord(doorSupport, entry, 2);
        if (mapIndex < 0)
            continue;
        const float distance = DistanceSquared(position, doorSupport.map_object_records[mapIndex].pos);
        if (distance < best) {
            best = distance;
            nearest = entry;
        }
    }
    return nearest;
}

std::int32_t DSWK_ComputeApproachPose(const DSWK_t &doorSupport, std::int32_t doorEntryIndex, Vector3 &position, std::int32_t &yaw) {
    const std::int32_t mapIndex = DSWK_FindMapObjectRecord(doorSupport, doorEntryIndex, 2);
    if (mapIndex < 0)
        return 0;
    const MapObjectRecord &record = doorSupport.map_object_records[mapIndex];
    Vector3 output{};
    const float side = ResolveApproachPointAndSide(record, ResourceFor(doorSupport, record), position, output);
    yaw = side >= 0.0f ? record.yawFixed16 : record.yawFixed16 + 0x8000;
    position = output;
    return side >= 0.0f ? -1 : 1;
}

bool DSWK_DoorRequestTablesReady(const DSWK_t &doorSupport) {
    return doorSupport.doorEntries && doorSupport.door_id_lut && doorSupport.res_name_tbl && doorSupport.map_object_records &&
        doorSupport.doorTaskStateBySlot && doorSupport.candDoorEntryIndices && doorSupport.owner_actor_by_slot && doorSupport.motion_variant_row_by_slot &&
        doorSupport.numEntries > 0 && doorSupport.maxCandidates > 0;
}

bool DSWK_TryComputeNearestDoorApproachPose(const DSWK_t &doorSupport, Vector3 &position, std::int32_t &yaw) {
    if (!DSWK_DoorRequestTablesReady(doorSupport))
        return false;

    const std::int32_t nearest = DSWK_FindNearestDoorEntry(doorSupport, position);
    if (nearest < 0)
        return false;

    const std::int32_t candidate = FindCandidateSlotForEntry(doorSupport, nearest);
    return candidate >= 0 && ComputeCandidateApproachPose(doorSupport, candidate, position, yaw) != 0;
}

std::int32_t DSWK_AllocateDoorRequest(DSWK_t &doorSupport, std::uint32_t actorId, const Vector3 &position, std::int32_t motionId) {
    const std::int32_t entry = DSWK_FindNearestDoorEntry(doorSupport, position);
    return AllocateRequest(doorSupport, entry, actorId, MotionVariantRow(motionId));
}

DSWKDoorRequestState DSWK_PollDoorRequest(DSWK_t &doorSupport, std::int32_t requestSlot) {
    if (requestSlot < 0 || requestSlot >= static_cast<std::int32_t>(std::size(doorSupport.requestSlots))) {
        return DSWK_REQ_FAILED;
    }

    DSWKRequestSlot &request = doorSupport.requestSlots[requestSlot];
    const DSWKDoorRequestState state = request.state;
    request.releaseDelayFrames = 0;
    switch (state) {
    case DSWK_REQ_NOT_READY:
    case DSWK_REQ_FAILED:
    case DSWK_REQ_COMPLETE:
    case DSWK_REQ_ACTIVE:
    case DSWK_REQ_PENDING:
        return state;
    default:
        return DSWK_REQ_FAILED;
    }
}



bool DSWK_TestDoorLineIntersection(const Vector3 &start, const Vector3 &end, float) {
    MapControlData *const control = ControlData();
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (!ryo)
        return false;
    Matrix4x4_TranslateAndRotate(*reinterpret_cast<const CharacterIdTransform *>(&ryo->entity.node));
    Vector3 localStart = start;
    Vector3 localEnd = end;
    if (ryo->entity.node.child) {
        localStart = WorldToEntityLocalPoint(*ryo->entity.node.child, localStart);
        localEnd = WorldToEntityLocalPoint(*ryo->entity.node.child, localEnd);
    }
    QueryShape query{};
    query.mode = CalculateLineIntersection_0;
    query.packed[0] = {localStart.x, localStart.z, localEnd.x};
    query.packed[1].x = localEnd.z;
    return ExecuteMathFnWrapper(&query, reinterpret_cast<const std::uint8_t *>(control->cycleMask)) != 0;
}

bool DSWK_PublishDoorActor(Character &character, const DoorResourceDescriptor &resource) {
    const char *const name = ScenarioInfoPointer<char>(resource.modelNameOff);
    if (!name || !*name) {
        IMGM_UnloadCharacterModel(&character);
        return false;
    }
    return MDLM_ActivateCharacterImage(&character, name, 0);
}

std::int32_t DSWK_ExecuteCommand(DSWK_t &doorSupport, const DoorSupportCommandParams &params) {
    const auto scenarioVector = [](std::int32_t offset) -> Vector3 * { return ScenarioPointer<Vector3>(offset); };
    const auto scenarioInt = [](std::int32_t offset) -> std::int32_t * { return ScenarioPointer<std::int32_t>(offset); };
    switch (params.command) {
    case DOOR_CMD_NOP:
        return 0;
    case DOOR_CMD_PRIME:
        doorSupport.state = DSWK_TASK_PRIME_CANDIDATES;
        return 0;
    case DOOR_CMD_SET_AND_UPDATE:
        doorSupport.state = DSWK_TASK_ACTIVE_UPDATE;
        return 0;
    case DOOR_CMD_FIND_MAP_ENTRY:
        return DSWK_FindMapObjectRecord(doorSupport, params.int4, params.int8);
    case DOOR_CMD_FIND_BEST_DOOR_CAND:
        return FindBestCandidateSlot(doorSupport);
    case DOOR_CMD_FIND_CLOSEST_POINT: {
        Vector3 *position = scenarioVector(params.int4);
        return position ? FindNearestCandidateSlot(doorSupport, *position) : -1;
    }
    case DOOR_CMD_ALLOC_ENTRY_SLOT:
        return AllocateRequest(doorSupport, params.int8, static_cast<std::uint32_t>(params.int4), 0);
    case DOOR_CMD_FIND_CLOSEST_ELEMENT: {
        Vector3 *position = scenarioVector(params.int8);
        return position ? DSWK_AllocateDoorRequest(doorSupport, static_cast<std::uint32_t>(params.int4), *position, params.intC) : -1;
    }
    case DOOR_CMD_FIND_CLOSEST_ELEMENT_ANGLE: {
        Vector3 *position = scenarioVector(params.int4);
        std::int32_t *yaw = scenarioInt(params.int8);
        if (!position || !yaw)
            return 0;
        const std::int32_t nearest = DSWK_FindNearestDoorEntry(doorSupport, *position);
        const std::int32_t candidate = FindCandidateSlotForEntry(doorSupport, nearest);
        return candidate >= 0 && ComputeCandidateApproachPose(doorSupport, candidate, *position, *yaw) != 0;
    }
    case DOOR_CMD_TEST_LINE_INTERSECTION: {
        const Vector3 *end = scenarioVector(params.int4);
        const Vector3 *start = scenarioVector(params.int8);
        return start && end && DSWK_TestDoorLineIntersection(*start, *end, 0.0f);
    }
    case DOOR_CMD_TIMER_START:
        g_dswk_tickTime = static_cast<std::int32_t>(FrameTimeUtil(TimeMode::ReturnNewTime));
        return 0;
    case DOOR_CMD_TIMER_GET_DELTA_SCALED: {
        g_dswk_tickTime -= static_cast<std::int32_t>(FrameTimeUtil(TimeMode::ReturnNewTime));
        const float delta = static_cast<float>(g_dswk_tickTime) / 12.5f;
        return static_cast<std::int32_t>(std::bit_cast<std::uint32_t>(delta));
    }
    case DOOR_CMD_SET_IDLE:
        doorSupport.state = DSWK_TASK_IDLE;
        return 0;
    case DOOR_CMD_POLL_REQUESTS:
        return DSWK_PollDoorRequest(doorSupport, params.int4);
    case DOOR_CMD_CALC_VECTOR_FROM_ANGLE: {
        Vector3 *position = scenarioVector(params.int8);
        std::int32_t *yaw = scenarioInt(params.intC);
        if (!position || !yaw)
            return 0;
        const std::int32_t result = ComputeCandidateApproachPose(doorSupport, params.int4, *position, *yaw);
        if (result) {
            position->x += sin_fixed16(static_cast<std::int16_t>(*yaw)) * 0.4f;
            position->z += cos_fixed16(static_cast<std::uint16_t>(*yaw)) * 0.4f;
        }
        return result;
    }
    default:
        return 0;
    }
}

void DSWK_Process(DSWK_t &doorSupport) {
    if (doorSupport.state == DSWK_TASK_PRIME_CANDIDATES) {
        UpdateEntryMetrics(doorSupport, 1);
        RebuildBestCandidateSlots(doorSupport);
        doorSupport.state = DSWK_TASK_ACTIVE_UPDATE;
    } else if (doorSupport.state == DSWK_TASK_ACTIVE_UPDATE) {
        UpdateEntryMetrics(doorSupport, g_nAuthorState != 0 ? 1 : 0);
        ResetCandidateSlots(doorSupport);
        RebuildBestCandidateSlots(doorSupport);
        RefreshVisibleActors(doorSupport);
        ProcessRequestSlots(doorSupport);
    }
}

void DoorSystemTask_Main(HLib::TaskData *taskData) {
    if (!g_doorSystemTask || g_doorSystemTask->task_data_ptr != taskData || g_bDebugSkipCyclesForCharacters) {
        return;
    }
    DSWK_Process(*reinterpret_cast<DSWK_t *>(taskData));
}

void DSWK_BindTablesAndInitState(ScenarioContext *context, void *rawParams) {
    if (!g_doorSystemTask)
        return;
    SetMainCallbackToInitForCleanup(g_doorSystemTask);
    if (!rawParams)
        return;
    if (DSWK_t *data = DoorTaskData()) {
        DSWK_BindTables(*data, *static_cast<const DoorSystemInitParams *>(rawParams), context && context->task ? context->task->task : nullptr);
    }
}

void DSWK_DoorUtils(ScenarioContext *context, void *rawParams) {
    DSWK_t *const data = DoorTaskData();
    if (!data || !rawParams)
        return;

    const auto &params = *static_cast<const DoorSupportCommandParams *>(rawParams);
    const std::int32_t result = DSWK_ExecuteCommand(*data, params);
    switch (params.command) {
    case DOOR_CMD_FIND_MAP_ENTRY:
    case DOOR_CMD_FIND_BEST_DOOR_CAND:
    case DOOR_CMD_FIND_CLOSEST_POINT:
    case DOOR_CMD_ALLOC_ENTRY_SLOT:
    case DOOR_CMD_FIND_CLOSEST_ELEMENT:
    case DOOR_CMD_FIND_CLOSEST_ELEMENT_ANGLE:
    case DOOR_CMD_TEST_LINE_INTERSECTION:
    case DOOR_CMD_TIMER_GET_DELTA_SCALED:
    case DOOR_CMD_POLL_REQUESTS:
    case DOOR_CMD_CALC_VECTOR_FROM_ANGLE:
        SetScenarioResult(context, static_cast<std::uint32_t>(result));
        break;
    default:
        break;
    }
}

void DSWK_RebindDoorSystemTables(ScenarioContext *, void *rawParams) {
    const auto *params = static_cast<const std::int32_t *>(rawParams);
    DSWK_t *data = DoorTaskData();
    if (!params || !data)
        return;
    data->doorEntries = ScenarioPointer<DSWKDoorEntry>(params[0]);
    data->door_id_lut = ScenarioPointer<std::int32_t>(params[1]);
    data->res_name_tbl = ScenarioPointer<DoorResourceDescriptor>(params[2]);
    data->map_object_records = ScenarioPointer<MapObjectRecord>(params[3]);
    data->state = DSWK_TASK_IDLE;
    data->metric_cur = 0;
    data->maxCandidates = params[4];
    data->numEntries = 0;
    data->candidateCount = 0;
    ResetEntriesAndPublishCtrlMasks(*data);
    ResetCandidateSlots(*data);
}

void DSWK_AllocDoorMapcRenderState(ScenarioContext *context, void *rawParams) {
    const auto *params = static_cast<const std::uint32_t *>(rawParams);
    Character *character = params ? LookupCharacter(context && context->task ? context->task->task : nullptr, params[0]) : nullptr;
    if (!character)
        return;
    CPAM_Alloc(character);
}

void DSWK_SetActorOpacityAndDrawCallback(ScenarioContext *context, void *rawParams) {
    const auto *params = static_cast<const DoorActorOpacityParams *>(rawParams);
    Character *character = params ? LookupCharacter(context && context->task ? context->task->task : nullptr, params->actorKey) : nullptr;
    if (!character)
        return;
    character->renderOpacity = params->opacity;
    character->drawCallback = CharacterHandler_AlphaDraw;
}

void DSWK_AssignDoorEntryConfig(ScenarioContext *, void *rawParams) {
    const auto *params = static_cast<const DoorEntryConfigParams *>(rawParams);
    DSWK_t *data = DoorTaskData();
    if (!params || !data)
        return;
    DSWKDoorEntry &entry = data->doorEntries[params->doorEntryIndex];
    entry.group = params->group;
    entry.global1 = params->global1;
    entry.global2 = params->global2;
    entry.global3 = params->global3;
    entry.sndCommandId = params->sndCommandId;
    entry.global5 = params->global5;
    entry.global6 = params->global6;
}

void DSWK_GetSettingsFlags(ScenarioContext *context, void *) {
    SetScenarioResult(context, (S1SettingsFlags_Get() >> 3U) & 1U);
}

void InitDoorSystemTask() {
    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(DoorSystemTask_Main, 5, 11, SHEN_NAME(DSWK));
    if (task) {
        if (!task->task_data_ptr) {
            auto *data = static_cast<DSWK_t *>(HLib_AllocHeapMemBlock(sizeof(DSWK_t), SHEN_NAME(TASK)));
            if (data)
                HLib_EnableTokenCheck(data);
            task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
            if (!data) {
                task->initCallbackFnPtr = nullptr;
                task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
                task->autoInitTaskCallbackFnPtr = nullptr;
                task = nullptr;
            }
        } else {
            task->initCallbackFnPtr = nullptr;
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            task->autoInitTaskCallbackFnPtr = nullptr;
            task = nullptr;
        }
    }

    if (task && task->task_data_ptr)
        std::memset(task->task_data_ptr, 0, sizeof(DSWK_t));

    if (task && !task->initCallbackFnPtr) {
        task->initCallbackFnPtr = task->mainTaskCallbackFnPtr;
        task->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
        task->mainExecutionIterations = 0;
    }
    if (task)
        task->mainExecutionIterations = 0;
    g_doorSystemTask = task;
}

} // namespace shenmue::s1
