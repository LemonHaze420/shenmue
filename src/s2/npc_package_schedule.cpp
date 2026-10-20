#include "s2/npc_package.hpp"

#include "s2/archive_data.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/hand.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion_pose.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player.hpp"
#include "s2/system_modules.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "s2/npc_package_internal.hpp"

namespace shenmue::s2 {
using namespace npc_package_detail;

namespace npc_package_detail {

void ClearNpcDeferredBranches(NpcRouteLiveEntry &entry) {
    // npcClearDeferredBranchTriggers clears only each sentinel;
    // the other fields remain available until the slot is reused.
    entry.localBranchCount = 0;
    for (std::size_t index = 0; index < 4; ++index) {
        WriteNpcLiveField<std::uint32_t>(entry, npcLocalBranchesOffset + index * sizeof(NpcLocalBranchTrigger), 0xffffffffU);
    }
    for (auto &trigger : npcGlobalBranchTriggers) {
        if (!entry.globalBranchCount)
            break;
        if (trigger.packedCondition != 0xffffffffU && trigger.owner.name == entry.name.name) {
            trigger.packedCondition = 0xffffffffU;
            --entry.globalBranchCount;
        }
    }
}

// @verified - p
void InitializeNpcInterpreterState(NpcRouteLiveEntry &entry, std::int32_t slot) {
    const std::byte *const record = entry.recordBase;
    NpcRouteLiveEntry *const previous = slot < 0 ? entry.previous : slot ? &entry - 1 : nullptr;
    NpcRouteLiveEntry *const next = slot < 0 ? entry.next : static_cast<std::size_t>(slot + 1) == npcRouteEntries.size() ? nullptr : &entry + 1;
    std::memset(&entry, 0, sizeof(entry));
    if (slot >= 0)
        entry.poolIndex = static_cast<std::int16_t>(slot);
    entry.previous = previous;
    entry.next = next;
    entry.motionSpeed = 1.0f;
    entry.stateContext.flags = 0;
    entry.stateContext.stateTable =
        npcSchedule.bytes.empty() ? nullptr : reinterpret_cast<const NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
    entry.stateContext.owner = &entry;
    entry.streamCost = 2;
    if (slot < static_cast<std::int32_t>(npcAuthoredEntryCount)) {
        entry.recordBase = record;
        entry.name = shen_name{ReadU32(record)};
        switch (entry.name.name) {
        case CHARACTER_REN.name:
        case CHARACTER_SHENHUA.name:
        case CHARACTER_WON.name:
        case NPC_SAM.name:
        case NPC_LLY.name:
        case CHARACTER_SYE.name:
        case NPC_YUA.name:
            entry.flags |= 0x04000000U;
            break;
        default:
            break;
        }
        entry.humanPackageIndex = FindNpcHumanPackageIndex(entry.name);
        entry.currentAreaPoint = ReadU16(record + 0x0c);
        entry.currentCommand = record + 0x38;
    } else {
        entry.flags |= NpcRouteAbstractActive;
        entry.recordBase = npcScriptRecord.data();
        entry.name = NPC_SCRIPT_ROUTE_SLOT;
        entry.humanPackageIndex = static_cast<std::int16_t>(slot);
        entry.streamSortKey = 1000.0f;
        entry.liveState = 1;
    }
    entry.branchCommandStart = entry.currentCommand;
    for (std::size_t index = 0; index < 4; ++index)
        WriteNpcLiveField(entry, npcLocalBranchesOffset + index * sizeof(NpcLocalBranchTrigger), std::uint32_t{0xffffffffU});
    entry.loadHumanName = entry.name;
    entry.activeBranchIndex = -1;
    entry.loadHumanResult = 8;
    entry.routeMotionState = 23;
    entry.primarySlotIndex = -1;
    entry.controlRowIndex = entry.nextControlRowIndex = entry.ownerRowIndex = entry.nextOwnerRowIndex = -1;
    if (NpcRouteRuntimeRecord *const runtime = GetNpcRuntimeRecord(entry)) {
        runtime->character = nullptr;
        runtime->packageAlias = {};
        runtime->streamState = NpcHumanStreamState::Inactive;
    }
}

bool SetNpcAreaPoint(NpcRouteLiveEntry &entry, std::uint16_t point) {
    if (point >= npcSchedule.areaPointCount || !Contains(npcSchedule.bytes, npcSchedule.areaPointTableOffset, npcSchedule.areaPointCount * 0x44ULL)) {
        return false;
    }
    const std::size_t offset = npcSchedule.areaPointTableOffset + static_cast<std::size_t>(point) * 0x44 + 0x34;
    if (!Contains(npcSchedule.bytes, offset, sizeof(Vector3)))
        return false;

    Vector3 worldPosition;
    std::memcpy(&worldPosition, npcSchedule.bytes.data() + offset, sizeof(worldPosition));
    Vector3 localPosition = worldPosition;
    localPosition.x -= npcSchedule.areaOrigin.x;
    localPosition.z -= npcSchedule.areaOrigin.z;
    entry.currentAreaPoint = point;
    WriteNpcLiveField(entry, npcLocalPositionOffset, localPosition);
    WriteNpcLiveField(entry, npcWorldPositionOffset, worldPosition);
    if (entry.node) {
        shenApplyTranslationToAll(&entry.node->entity.node, localPosition);
    }
    return true;
}

void ApplyNpcCompletionFlag(const std::byte *command) {
    if (ReadU32(command + 12)) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(ReadU32(command + 4)), static_cast<std::int32_t>(ReadU32(command + 8)));
    } else {
        EV_stepSetFlags(static_cast<std::uint8_t>(ReadU32(command + 8)), static_cast<std::int32_t>(ReadU32(command + 4)));
    }
}

bool AdvanceNpcTimedSequenceSlot(NpcRouteLiveEntry &entry, std::int32_t currentTime) {
    if (currentTime < entry.nextTimedCommandTime)
        return false;
    const NpcRouteRuntimeRecord &runtime = *GetNpcRuntimeRecord(entry);
    if (!entry.nextTimedCommand)
        return false;
    const std::byte *command = entry.nextTimedCommand + 8;
    entry.currentCommand = command;
    while (NpcRecordContains(runtime, command, 4)) {
        const std::uint32_t opcode = ReadU32(command);
        if (opcode >= npcOpcodeSizeDwords.size())
            break;
        if (opcode == 1) {
            entry.nextTimedCommand = command;
            const std::uint32_t time = ReadU32(command + 4);
            entry.nextTimedCommandTime = 120 * (static_cast<std::uint8_t>(time) + 60 * static_cast<std::uint8_t>(time >> 8));
            entry.resumeCommand = nullptr;
            return true;
        }
        command += npcOpcodeSizeDwords[opcode] * 4;
    }
    entry.nextTimedCommand = nullptr;
    entry.resumeCommand = nullptr;
    return true;
}

void BeginNpcTimedWait(NpcRouteLiveEntry &entry, std::int32_t currentTime) {
    const std::uint32_t packed = ReadU32(entry.currentCommand + 4);
    entry.delayStartTime = currentTime;
    entry.delayDuration = 120 * (static_cast<std::uint8_t>(packed >> 8) + 60 * static_cast<std::uint8_t>(packed));
    entry.currentCommand += 8;
    if (npcIsTimeAdvancing == 1)
        entry.delayDuration = 0;
    else
        entry.scheduleProgress = 2;
}

bool ExecuteNpcAreaPrelude(NpcRouteLiveEntry &entry, std::int32_t currentTime) {
    switch (ReadU32(entry.currentCommand)) {
    case 1:
        if (!AdvanceNpcTimedSequenceSlot(entry, currentTime)) {
            if (entry.resumeCommand)
                entry.currentCommand = entry.resumeCommand;
            if (npcIsTimeAdvancing == 1)
                npcTimeAdvanceBreak = 1;
        }
        return false;
    case 2:
        BeginNpcTimedWait(entry, currentTime);
        return false;
    case 24:
        entry.eyeControlMode = static_cast<std::int8_t>(entry.currentCommand[4]);
        entry.currentCommand += 8;
        return true;
    case 45:
        entry.waitParameter = ReadU16(entry.currentCommand + 4);
        entry.opcodeByte = static_cast<std::int8_t>(entry.currentCommand[8]);
        entry.currentCommand += 12;
        return true;
    case 46:
        entry.currentCommand += 8;
        entry.waitParameter = 0;
        entry.opcodeByte = -1;
        return true;
    default:
        return false;
    }
}

std::int64_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry) {
    if (entry->routeMotionState == 23)
        return 0;
    RequestNpcAreaRouteCompletion(*entry);
    return 1;
}

bool BeginNpcAreaPointState(NpcRouteLiveEntry &entry, std::int32_t currentTime) {
    if (!TryEnterNpcAreaRoute(entry))
        return false;
    entry.stateCallback = NpcAreaCompletionCallback;
    entry.scheduleProgress = 3;
    entry.liveState = 8;
    if (GetNpcAreaPointRouteState(entry.currentAreaPoint)->flags & 8U)
        npcSlotFlags[entry.poolIndex] |= 4U;
    else
        npcSlotFlags[entry.poolIndex] &= ~4U;
    for (std::int32_t iteration = 0; iteration < 64; ++iteration) {
        if (!ExecuteNpcAreaPrelude(entry, currentTime))
            break;
    }
    return true;
}

std::int64_t TickNpcAreaRoute(NpcRouteLiveEntry *routeEntry, std::int32_t currentTime) {
    NpcRouteLiveEntry &entry = *routeEntry;
    if (!TickNpcRouteStateContext(&entry.stateContext))
        return false;
    if (entry.motionWork)
        SetMotionSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
    entry.currentAreaPoint = entry.pendingAreaPoint;
    NpcRouteRuntimeRecord *const runtime = GetNpcRuntimeRecord(entry);
    if (runtime && NpcRecordContains(*runtime, entry.currentCommand, 16) && ReadU32(entry.currentCommand) == 42) {
        ApplyNpcCompletionFlag(entry.currentCommand);
        entry.currentCommand += 16;
    }
    if (npcIsTimeAdvancing == 1 || !BeginNpcAreaPointState(entry, currentTime)) {
        entry.liveState = 0;
        entry.scheduleProgress = 1;
    }
    SetNpcRouteDirection(entry, entry.currentAreaPoint);
    return true;
}

void SetNpcRouteDirection(NpcRouteLiveEntry &entry, std::uint16_t point) {
    const std::uint32_t initial = ReadU32(entry.recordBase + 0x0c);
    const std::uint32_t endpoint = ReadU32(entry.recordBase + 0x10);
    const std::uint32_t signedPoint = static_cast<std::uint32_t>(static_cast<std::int16_t>(point));
    entry.routeDirectionMode = signedPoint == initial ? 0 : static_cast<std::int8_t>(signedPoint == endpoint ? 1 : 3);
}

void ResolveNpcAreaPointOwnerAnchor(NpcRouteLiveEntry &entry, std::uint16_t point) {
    const std::byte *const areaPoint = GetNpcAreaPoint(point);
    entry.flags &= ~0x800U;
    if (areaPoint && !ReadU32(areaPoint + 0x40) && ReadU32(areaPoint + 0x30)) {
        const std::int32_t door = static_cast<std::int32_t>(ReadU32(areaPoint + 0x30));
        Vector3 position;
        if (ComputeDoorIngress(door, &entry.localPosition, &position)) {
            ComputeDoorEgress(door, &entry.localPosition, &position);
            entry.localPosition = position;
            entry.worldPosition = NpcAreaPosition(position);
            entry.flags |= 0x800U;
            if (entry.node)
                SetCharacterType(entry.node, 3);
        }
    } else if (entry.node)
        SetCharacterType(entry.node, CharacterTypeDefault);
}

bool PrimeNpcInterpreter(NpcRouteLiveEntry &entry) {
    const std::uint16_t point = entry.currentAreaPoint;
    const std::byte *const areaPoint = GetNpcAreaPoint(point);
    if (!areaPoint)
        return false;
    std::memcpy(&entry.worldPosition, areaPoint + 0x34, sizeof(Vector3));
    entry.localPosition = entry.worldPosition;
    entry.localPosition.x -= npcSchedule.areaOrigin.x;
    entry.localPosition.z -= npcSchedule.areaOrigin.z;
    entry.scheduleProgress = 1;
    SetNpcAreaPointTangentYaw(entry, point);
    ResolveNpcAreaPointOwnerAnchor(entry, point);
    entry.routeDirectionMode = 0;
    if (!entry.currentCommand)
        return true;
    const NpcRouteRuntimeRecord &runtime = *GetNpcRuntimeRecord(entry);
    entry.nextTimedCommand = nullptr;
    for (const std::byte *command = entry.currentCommand; NpcRecordContains(runtime, command, 4);) {
        const std::int32_t opcode = static_cast<std::int32_t>(ReadU32(command));
        if (opcode < 0 || static_cast<std::size_t>(opcode) >= npcOpcodeSizeDwords.size())
            break;
        if (opcode == 1) {
            entry.nextTimedCommand = command;
            const std::uint32_t packedTime = ReadU32(command + 4);
            entry.nextTimedCommandTime = 120 * (static_cast<std::uint8_t>(packedTime) + 60 * static_cast<std::uint8_t>(packedTime >> 8));
            break;
        }
        command += static_cast<std::size_t>(npcOpcodeSizeDwords[opcode]) * 4;
    }
    return true;
}
void SetNpcHumanVisibilityChannels(NpcRouteLiveEntry &entry, bool enabled) {
    if (!entry.node || !(entry.flags & 0x400000U))
        return;
    for (ModelComponentHeader *component = entry.node->modelComponents; component; component = component->next) {
        switch (component->token.name) {
        case SHEN_NAME(IMGM):
        case SHEN_NAME(MHND):
        case SHEN_NAME(REFB):
        case SHEN_NAME(FRTK):
        case SHEN_NAME(CCM_):
            if (enabled)
                component->flags |= 1U;
            else
                component->flags &= ~1U;
            break;
        default:
            break;
        }
    }
}

std::int64_t NpcClipCompletionCallback(NpcRouteLiveEntry *entry) {
    if (ReadU32(entry->currentCommand) == 0xffffffffU)
        return -1;
    if (entry->flags & 7U) {
        ++npcActiveStreamCost;
        SetNpcHumanVisibilityChannels(*entry, true);
    }
    entry->flags &= ~0x40000U;
    entry->streamCost = 2;
    entry->liveState = 0;
    return 0;
}

bool BeginNpcMotionClip(NpcRouteLiveEntry &entry) {
    if (entry.stateCallback)
        return false;
    entry.stateCallback = NpcClipCompletionCallback;
    entry.motionSpeed = 1.0f;
    entry.clipOverrideIndex = static_cast<std::int8_t>(entry.currentCommand[4]);
    entry.currentCommand += 8;
    if (entry.flags & 7U) {
        if (entry.streamCost != 1 && (entry.flags & 0x1aU) == 2) {
            entry.field335 = 0;
            entry.flags = (entry.flags & 0xddfffff9U) | 0x22000004U;
        }
        npcActiveStreamCost += 1 - entry.streamCost;
        SetNpcHumanVisibilityChannels(entry, false);
    }
    entry.flags |= npcMotionStateActive;
    entry.streamCost = 1;
    entry.routeDirectionMode = 3;
    ResolveNpcAreaPointOwnerAnchor(entry, entry.currentAreaPoint);
    if (entry.branchCommandStart && ReadU32(entry.branchCommandStart) == 9) {
        entry.flags |= 0x40000U;
        entry.animationFrame += static_cast<float>(npcFrameCount);
    } else {
        entry.animationFrame = static_cast<float>(static_cast<std::int32_t>(IncrementSharedPrngFloat() * static_cast<float>(npcFrameCount)));
    }
    const std::uint32_t frameCount = npcMotionClips[entry.clipOverrideIndex].frameCount;
    if (frameCount)
        entry.animationFrame = static_cast<float>(static_cast<std::int32_t>(entry.animationFrame) % static_cast<std::int32_t>(frameCount));
    entry.liveState = 7;
    return true;
}

bool ResumeNpcTimedMotion(NpcRouteLiveEntry &entry) {
    if (!entry.timedMotionHandle && !AllocateNpcTimedMotion(entry))
        return false;
    mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
    const MotnSequence &sequence = *FindLoadedMotionSequence(entry.currentMotionId);
    const std::int32_t frame = npcFrameCount % (sequence.frameCount - 1);
    if (frame) {
        SetMotionRegionFlag(entry.motionWork, 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(entry.motionWork, 14, 0);
        SetMotionRegionEvalTime(entry.motionWork, static_cast<float>(frame), MotionRegion::LowerBody);
    }
    auto &slot = *static_cast<NpcTimedMotionSlot *>(entry.timedMotionHandle);
    if (!slot.motion)
        return true;
    slot.character = FindCharacterByScenarioName(slot.name);
    if (!slot.character)
        return false;
    slot.character->renderFlags |= CRF_DisplayEnabled;
    Vector3 position = slot.offset;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::SetViewMatrixFromEuler(ReadNpcEulerYaw(entry.euler), 0, 0);
    Matrix4x4::TransformPoint(position, position);
    Matrix4x4::Pop(1);
    position.x += entry.worldPosition.x;
    position.y += entry.worldPosition.y;
    position.z += entry.worldPosition.z;

    position = NpcRouteAreaToScene(entry.worldPosition);
    shenApplyTranslationToAll(&slot.character->entity.node, position);
    Vector3i rotation;
    std::memcpy(&rotation, &entry.euler, sizeof(rotation));
    SetCharacterWorldRotation(slot.character, rotation);
    InitializeTimedNodeMotion(slot.animation, static_cast<MT7Model *>(slot.character->model), slot.motion);
    slot.animation.frame = static_cast<float>(frame);
    TickTimedNodeMotion(slot.animation);
    slot.animation.frame = static_cast<float>(frame);
    return true;
}

std::int64_t NpcTimedMotionCompletionCallback(NpcRouteLiveEntry *entry) {
    switch (ReadU32(entry->currentCommand)) {
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 18:
    case 19:
    case 32:
    case 40:
        ReleaseNpcTimedMotion(*entry);
        entry->liveState = 0;
        return 0;
    default:
        return -1;
    }
}

void BeginNpcTimedMotion(NpcRouteLiveEntry &entry) {
    if (entry.stateCallback)
        return;
    entry.stateCallback = NpcTimedMotionCompletionCallback;
    const std::byte *const command = entry.currentCommand;
    if (!AllocateNpcTimedMotion(entry)) {
        entry.currentCommand += 36;
        return;
    }
    entry.currentMotionId = ReadU16(command + 4);
    entry.currentAreaPoint = ReadU16(command + 8);
    RestoreNpcAreaPointSeedPosition(entry);
    entry.euler.y = ReadF32(command + 12);
    auto &slot = *static_cast<NpcTimedMotionSlot *>(entry.timedMotionHandle);
    slot.name = shen_name{ReadU32(command + 16)};
    slot.motion = ReadU16(command + 20);
    slot.offset = {ReadF32(command + 24), ReadF32(command + 28), ReadF32(command + 32)};
    entry.flags |= 0x400U;
    entry.currentCommand += 36;
    entry.motionSpeed = 1.0f;
    if (entry.node) {
        mtwkSetRequestedRootMode(entry.motionWork, 0);
        ResumeNpcTimedMotion(entry);
        if (entry.flags & 0xa000U)
            entry.localPosition.y = entry.forcedY;
        else
            FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        Vector3i rotation;
        std::memcpy(&rotation, &entry.euler, sizeof(rotation));
        SetCharacterWorldRotation(entry.node, rotation);
        entry.streamRadius = 0.30000001f;
    }
    entry.liveState = 3;
    entry.flags |= npcMotionStateActive;
    ResolveNpcAreaPointOwnerAnchor(entry, entry.currentAreaPoint);
    SetNpcRouteDirection(entry, entry.currentAreaPoint);
}

NpcConditionResult EvaluateNpcCondition(NpcRouteLiveEntry &entry, std::uint32_t packedCondition, std::uint32_t argument, std::int32_t currentTime) {
    const std::uint16_t flag = static_cast<std::uint16_t>(packedCondition);
    switch (packedCondition >> 16) {
    case 0:
        return static_cast<std::uint32_t>(EV_GetShenmueFlag(flag)) == argument ? NpcConditionResult::True : NpcConditionResult::False;
    case 1:
        return static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(flag))) == argument ?
            NpcConditionResult::True :
            NpcConditionResult::False;
    case 2: {
        const Vector3 position = ReadNpcLiveField<Vector3>(entry, npcLocalPositionOffset);
        const float deltaX = npcPlayerPosition.x - position.x;
        const float deltaZ = npcPlayerPosition.z - position.z;
        float threshold{};
        std::memcpy(&threshold, &argument, sizeof(threshold));
        if (deltaX * deltaX + deltaZ * deltaZ <= threshold) {
            npcCondition2OutsideFrames = 0;
        } else {
            const std::uint32_t prior = npcCondition2OutsideFrames++;
            if (prior > 0x1c2U)
                return NpcConditionResult::True;
        }
        if (npcCondition2StartSeconds + 3600U >= npcCurrentSeconds)
            return NpcConditionResult::False;
        npcCondition2OutsideFrames = 451;
        return NpcConditionResult::True;
    }
    case 3:
        return static_cast<std::int32_t>(argument) >= currentTime ? NpcConditionResult::True : NpcConditionResult::False;
    case 4: {
        float selector{};
        std::memcpy(&selector, &argument, sizeof(selector));
        if (selector == 0.0f) {
            return !(npcCrowdDensityScale >= 0.1f) ? NpcConditionResult::True : NpcConditionResult::False;
        }
        return argument == 1U && !(npcCrowdDensityScale < 0.1f) ? NpcConditionResult::True : NpcConditionResult::False;
    }
    case 5:
        return static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Story, static_cast<std::int16_t>(flag))) == argument ? NpcConditionResult::True :
                                                                                                                                      NpcConditionResult::False;
    default:
        return NpcConditionResult::False;
    }
}

bool SwitchNpcBranch(NpcRouteLiveEntry &entry, std::uint32_t branch, bool clearPending) {
    NpcRouteRuntimeRecord *const runtime = GetNpcRuntimeRecord(entry);
    if (!runtime || branch >= runtime->branchCount) {
        return false;
    }

    const std::byte *link = entry.recordBase + 0x34;
    for (std::uint32_t index = 0; index < branch; ++index) {
        if (!NpcRecordContains(*runtime, link, 4))
            return false;
        const std::uint32_t relative = ReadU32(link);
        if (!relative || relative > static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(runtime->end) - reinterpret_cast<std::uintptr_t>(link))) {
            return false;
        }
        link += relative;
    }
    if (!NpcRecordContains(*runtime, link, 8))
        return false;
    const std::byte *const command = link + 4;
    WriteNpcLiveField(entry, npcCurrentCommandOffset, command);
    const std::byte *const noResume = nullptr;
    WriteNpcLiveField(entry, npcResumeCommandOffset, noResume);
    const std::int8_t active = static_cast<std::int8_t>(branch);
    WriteNpcLiveField(entry, npcActiveBranchOffset, active);
    entry.nextTimedCommand = nullptr;
    for (const std::byte *cursor = command; NpcRecordContains(*runtime, cursor, 4);) {
        const std::uint32_t opcode = ReadU32(cursor);
        if (opcode >= npcOpcodeSizeDwords.size())
            break;
        if (opcode == 1) {
            if (!NpcRecordContains(*runtime, cursor, 8))
                break;
            entry.nextTimedCommand = cursor;
            const std::uint32_t packedTime = ReadU32(cursor + 4);
            entry.nextTimedCommandTime = 120 * (static_cast<std::uint8_t>(packedTime) + 60 * static_cast<std::uint8_t>(packedTime >> 8));
            break;
        }
        cursor += static_cast<std::size_t>(npcOpcodeSizeDwords[opcode]) * 4;
    }
    if (clearPending)
        ClearNpcDeferredBranches(entry);
    return true;
}

void QueueNpcGlobalBranch(NpcRouteLiveEntry &entry, const std::byte *command) {
    const std::uint32_t packed = ReadU32(command + 4) | (ReadU32(command + 16) << 16);
    NpcGlobalBranchTrigger *freeTrigger = nullptr;
    for (auto &trigger : npcGlobalBranchTriggers) {
        if (trigger.owner.name == entry.name.name && trigger.packedCondition == packed)
            return;
        if (trigger.packedCondition == 0xffffffffU && !freeTrigger)
            freeTrigger = &trigger;
    }
    if (!freeTrigger)
        return;
    freeTrigger->commandOffset = static_cast<std::uint16_t>(command - entry.recordBase - 0x38);
    freeTrigger->owner = entry.name;
    freeTrigger->packedCondition = packed;
    freeTrigger->conditionArgument = ReadU32(command + 8);
    freeTrigger->targetBranch = static_cast<std::uint8_t>(command[12]);
    ++entry.globalBranchCount;
    if (ReadU32(command + 16) != 2 || npcScheduleSeedNames[1].name != entry.name.name || (entry.flags & 0x10000000U))
        return;
    const auto seedByte = [](std::int16_t index) { return static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, index)); };
    if (seedByte(34)) {
        npcCondition2StartSeconds = (seedByte(48) << 24) + (seedByte(49) << 16) + (seedByte(32) << 8) + seedByte(33);
    } else {
        DateTime seedTime = npcCurrentDateTime;
        seedTime.second = 30;
        npcCondition2StartSeconds = static_cast<std::uint32_t>(ComputeSecondsSince1980(seedTime));
        npcCondition2OutsideFrames = 0;
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 48, npcCondition2StartSeconds >> 24);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 49, (npcCondition2StartSeconds >> 16) & 0xffU);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 32, (npcCondition2StartSeconds >> 8) & 0xffU);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 33, npcCondition2StartSeconds & 0xffU);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 34, 1);
    }
}

void UpdateNpcScheduleSeedSlot(shen_name character, shen_name previous, std::size_t slot) {
    if (!character.name && npcScheduleSeedNames[slot].name != previous.name)
        return;
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character.name ? character : previous);
    if (!entry)
        return;
    if (character.name)
        entry->flags |= 0x10000U;
    else
        entry->flags &= ~0x10000U;
    npcScheduleSeedNames[slot] = character;
}

void QueueNpcLocalBranch(NpcRouteLiveEntry &entry, const std::byte *command) {
    if (entry.localBranchCount == 4)
        return;
    const std::uint32_t packed = ReadU32(command + 4) | (ReadU32(command + 16) << 16);
    std::size_t empty = 4;
    for (std::size_t index = 0; index < 4; ++index) {
        const std::size_t offset = npcLocalBranchesOffset + index * sizeof(NpcLocalBranchTrigger);
        const NpcLocalBranchTrigger trigger = ReadNpcLiveField<NpcLocalBranchTrigger>(entry, offset);
        if (trigger.packedCondition == packed)
            return;
        if (empty == 4 && trigger.packedCondition == 0xffffffffU) {
            empty = index;
        }
    }
    if (empty == 4)
        return;

    NpcLocalBranchTrigger trigger = ReadNpcLiveField<NpcLocalBranchTrigger>(entry, npcLocalBranchesOffset + empty * sizeof(NpcLocalBranchTrigger));
    trigger.packedCondition = packed;
    trigger.conditionArgument = ReadU32(command + 8);
    trigger.targetBranch = static_cast<std::uint8_t>(ReadU32(command + 12));
    trigger.commandOffset = static_cast<std::uint16_t>(command - entry.recordBase - 0x38);
    WriteNpcLiveField(entry, npcLocalBranchesOffset + empty * sizeof(NpcLocalBranchTrigger), trigger);
    ++entry.localBranchCount;
}

void EvaluateNpcDeferredBranches(NpcRouteLiveEntry &entry, std::int32_t currentTime) {
    std::int32_t remaining = static_cast<std::int8_t>(entry.localBranchCount);
    for (std::size_t index = 0; index < 4 && remaining; ++index) {
        const NpcLocalBranchTrigger trigger = ReadNpcLiveField<NpcLocalBranchTrigger>(entry, npcLocalBranchesOffset + index * sizeof(NpcLocalBranchTrigger));
        if (trigger.packedCondition == 0xffffffffU)
            continue;
        --remaining;
        if (EvaluateNpcCondition(entry, trigger.packedCondition, trigger.conditionArgument, currentTime) != NpcConditionResult::True ||
            !SwitchNpcBranch(entry, trigger.targetBranch, false)) {
            continue;
        }
        if ((trigger.packedCondition >> 16) <= 1)
            ResetNpcAreaRouteState(entry);
        ClearNpcDeferredBranches(entry);
        entry.scheduleProgress = 1;
        return;
    }
    remaining = static_cast<std::int8_t>(entry.globalBranchCount);
    for (const auto &trigger : npcGlobalBranchTriggers) {
        if (!remaining)
            break;
        if (trigger.packedCondition == 0xffffffffU || trigger.owner.name != entry.name.name)
            continue;
        --remaining;
        if (EvaluateNpcCondition(entry, trigger.packedCondition, trigger.conditionArgument, currentTime) != NpcConditionResult::True ||
            !SwitchNpcBranch(entry, trigger.targetBranch, false)) {
            continue;
        }
        if ((trigger.packedCondition >> 16) <= 1)
            ResetNpcAreaRouteState(entry);
        ClearNpcDeferredBranches(entry);
        entry.scheduleProgress = 1;
        return;
    }
}

// @verified - p
std::int64_t NpcForcedHeightCompletionCallback(NpcRouteLiveEntry *entry) {
    switch (ReadU32(entry->currentCommand)) {
    case 3:
    case 4:
    case 7:
    case 18:
    case 32:
        if (entry->node) {
            if (CharacterControlModel *const control = FindCharacterControlModel(entry->node)) {
                std::uint32_t flags = control->flags;
                if (npcSchedule.floor == -1000 || !(entry->packedMoveFlags & 0xff00U))
                    flags |= CharacterControlApplyGravity;
                control->flags = flags | CharacterControlEvaluateMotion;
            }
        }
        entry->flags &= ~0x8000U;
        return 0;
    case 8:
    case 19:
        return 0;
    default:
        return -1;
    }
}

// @verified - p
std::int32_t BranchNpcOnLoadedArea(NpcRouteLiveEntry &entry, std::uint32_t opcode) {
    const std::byte *const command = entry.currentCommand;
    const bool matches = opcode == 39 ? ReadU32(command + 4) == npcLoadedArea.name : ReadU16(command + 4) == static_cast<std::uint16_t>(npcLoadedArea.name);
    if (!matches || !SwitchNpcBranch(entry, ReadU32(command + 8), true))
        entry.currentCommand += 4 * npcOpcodeSizeDwords[opcode];
    return 1;
}

// @verified - p
std::int32_t BranchNpcOnCalendarOrPeerState(NpcRouteLiveEntry &entry, std::uint32_t opcode) {
    const std::byte *const command = entry.currentCommand;
    std::uint32_t branch = 0;
    bool matches = false;
    switch (opcode) {
    case 13:
        matches = npcCurrentDateTime.week != 0 && npcCurrentDateTime.week != 6;
        branch = ReadU32(command + 4);
        break;
    case 14:
        matches = npcCurrentDateTime.week == 0 || npcCurrentDateTime.week == 6;
        branch = ReadU32(command + 4);
        break;
    case 15:
        if (!npcRegistryTask.current)
            break;
        for (const NpcRouteLiveEntry &other : npcRouteEntries) {
            if (other.name.name != ReadU32(command + 4))
                continue;
            matches = static_cast<std::uint8_t>(other.recordBase[6]) == npcCurrentDateTime.month &&
                static_cast<std::uint8_t>(other.recordBase[7]) == npcCurrentDateTime.day;
            branch = ReadU32(command + 8);
            break;
        }
        break;
    case 57:
        matches = static_cast<std::uint8_t>(command[4]) == npcCurrentDateTime.month && static_cast<std::uint8_t>(command[5]) == npcCurrentDateTime.day;
        branch = ReadU32(command + 8);
        break;
    default:
        break;
    }
    if (!matches || !SwitchNpcBranch(entry, branch, true))
        entry.currentCommand += 4 * npcOpcodeSizeDwords[opcode];
    return 1;
}

std::int32_t ExecuteNpcCommand(NpcRouteLiveEntry &entry, std::int32_t currentTime) {
    NpcRouteRuntimeRecord *const runtime = GetNpcRuntimeRecord(entry);
    if (!runtime || !NpcRecordContains(*runtime, entry.currentCommand, 4))
        return 0;
    if (npcIsTimeAdvancing == 1) {
        if (entry.nextTimedCommand && npcLatestTime >= entry.nextTimedCommandTime && (entry.flags & npcMotionStateActive)) {
            entry.currentCommand = entry.nextTimedCommand;
            entry.scheduleProgress = 1;
            entry.delayDuration = 0;
        }
        if (entry.branchCommandStart && ReadU32(entry.branchCommandStart) == 6)
            return 2;
        const std::uint32_t pending = ReadU32(entry.currentCommand);
        if (pending == 1) {
            if (!AdvanceNpcTimedSequenceSlot(entry, npcLatestTime))
                return 2;
        } else if (pending == 6 ||
            (pending == 47 &&
                120 * (static_cast<std::uint8_t>(entry.currentCommand[4]) + 60 * static_cast<std::uint8_t>(entry.currentCommand[5])) > npcLatestTime))
            return 2;
    }
    const std::byte *const command = entry.currentCommand;
    if (!NpcRecordContains(*runtime, command, 4))
        return 0;
    const std::uint32_t opcode = ReadU32(command);
    if (opcode == 0xffffffffU) {
        entry.scheduleProgress = 4;
        return 2;
    }
    if (opcode >= npcOpcodeSizeDwords.size()) {
        entry.liveState = 10;
        entry.routeDirectionMode = -1;
        entry.scheduleProgress = 4;
        entry.branchCommandStart = command;
        return false;
    }
    const std::size_t commandSize = static_cast<std::size_t>(npcOpcodeSizeDwords[opcode]) * 4;
    if (!NpcRecordContains(*runtime, command, commandSize))
        return false;
    const std::byte *const next = command + commandSize;
    bool keepExecuting = false;

    switch (opcode) {
    case 0:
    case 34:
    case 35:
        break;
    case 16:
    case 25:
    case 29:
        entry.currentCommand = next;
        break;
    case 23:
        if (entry.node || npcIsTimeAdvancing == 1)
            entry.currentCommand = next;
        break;
    case 41:
        entry.currentAreaPoint = ReadU16(command + 4);
        entry.currentCommand = next;
        keepExecuting = true;
        break;
    case 1:
        if (!AdvanceNpcTimedSequenceSlot(entry, currentTime) && entry.resumeCommand)
            entry.currentCommand = entry.resumeCommand;
        break;
    case 17: {
        if (entry.node && npcIsTimeAdvancing != 1)
            break;
        const shen_name alias{ReadU32(command + 4)};
        WriteNpcLiveField(entry, npcLoadHumanNameOffset, alias);
        runtime->packageAlias = alias;
        runtime->streamState = NpcHumanStreamState::Inactive;
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    }
    case 47:
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 2:
        BeginNpcTimedWait(entry, currentTime);
        break;
    case 3:
    case 32: {
        const std::uint16_t point = ReadU16(command + 4);
        entry.flags &= ~0x0000a000U;
        entry.currentAreaPoint = point;
        RestoreNpcAreaPointSeedPosition(entry);
        SetNpcAreaPointTangentYaw(entry, point);
        entry.flags |= npcMotionStateActive;
        entry.liveState = 0;
        if (opcode != 3 ||
            (npcIsTimeAdvancing && entry.nextTimedCommand && npcLatestTime >= entry.nextTimedCommandTime && (entry.flags & npcMotionStateActive)) ||
            !BeginNpcAreaPointState(entry, currentTime))
            ResolveNpcAreaPointOwnerAnchor(entry, point);
        else
            entry.commandFlags |= 0x100U;
        if (entry.node) {
            CopyMotionTransition(entry.motionWork, nullptr, 0, 1, 0);
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            SetCharacterWorldRotation(entry.node, rotation);
            SetMotionRegionFlag(entry.motionWork, 71, 0, MotionRegion::LowerBody);
            mtwkSetExtraFlag(entry.motionWork, 14, 0);
            mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
        }
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        SetNpcRouteDirection(entry, point);
        break;
    }
    case 18:
    case 4: {
        if (npcIsTimeAdvancing == 1) {
            if (entry.stateCallback)
                break;
            entry.flags &= ~0x8000U;
            entry.stateCallback = NpcRouteScheduleCallback;
            entry.pendingAreaPoint = entry.currentAreaPoint = ReadU16(command + 4);
            entry.currentCommand = next;
            if (ReadU32(entry.currentCommand) == 42) {
                ApplyNpcCompletionFlag(entry.currentCommand);
                entry.currentCommand += 16;
            }
            SetNpcAreaPoint(entry, entry.currentAreaPoint);
            const std::uint32_t following = ReadU32(entry.currentCommand);
            if (((following == 4 || following == 18) && IncrementSharedPrngFloat() * 5.0f >= 2.0f) || !BeginNpcAreaPointState(entry, npcLatestTime))
                ResolveNpcAreaPointOwnerAnchor(entry, entry.currentAreaPoint);
            break;
        }
        NpcRouteRuntimeRecord *const routeRuntime = GetNpcRuntimeRecord(entry);
        if (entry.stateCallback || !routeRuntime || !BeginNpcAreaRoute(entry, ReadU16(command + 4), ReadF32(command + 8))) {
            break;
        }
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        if (opcode == 18)
            SetNpcRouteMotionState(entry.stateContext, 1);
        if (NpcRecordContains(*routeRuntime, next, 12) && ReadU32(next) == 55) {
            if (NpcRouteStateContext *const leader = FindNpcRouteStateContext(shen_name{ReadU32(next + 4)})) {
                entry.stateContext.linkedRoute = {leader->owner, ReadF32(next + 8)};
                entry.stateContext.routeFlags |= 0x1000000U;
            }
            entry.currentCommand += 12;
        }
        if (!npcColdResetMode && IsNpcGloballyBlocked(entry))
            SetNpcStreamLifecycleState(entry, 1);
        break;
    }
    case 8: {
        if (entry.stateCallback)
            break;
        entry.stateCallback = NpcForcedHeightCompletionCallback;
        entry.currentMotionId = ReadU16(command + 4);
        entry.currentAreaPoint = ReadU16(command + 8);
        entry.flags |= 0x8000U;
        entry.forcedY = ReadF32(command + 12);
        RestoreNpcAreaPointSeedPosition(entry);
        entry.euler.y = ReadF32(command + 16);
        entry.motionSpeed = ReadF32(command + 20);
        entry.currentCommand = next;
        if (entry.node &&
            (entry.currentMotionId != GetMotionAnimationId(entry.motionWork, MotionRegion::LowerBody) ||
                !motnIsCurrentAnimOpenEnded(entry.motionWork, MotionRegion::LowerBody))) {
            mtwkSetRequestedRootMode(entry.motionWork, 1);
            SetMotionFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags &= ~(CharacterControlApplyGravity | CharacterControlEvaluateMotion);
            SetCharacterType(entry.node, 3);
            if (entry.flags & 0xa000U)
                entry.localPosition.y = entry.forcedY;
            else
                FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            SetCharacterWorldRotation(entry.node, rotation);
            mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
            SetMotionSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
            entry.streamRadius = 0.30000001f;
        }
        entry.flags |= npcMotionStateActive;
        entry.liveState = 4;
        ResolveNpcAreaPointOwnerAnchor(entry, entry.currentAreaPoint);
        SetNpcRouteDirection(entry, entry.currentAreaPoint);
        break;
    }
    case 42:
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 56:
        entry.eyeControlProfile = static_cast<std::int8_t>(command[4]);
        entry.routeEyeMode = static_cast<std::int16_t>((entry.routeEyeMode & ~0xff) | static_cast<std::uint8_t>(command[8]));
        HandleNpcEyeControl(entry);
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        break;
    case 5:
        entry.flags |= npcMotionStateActive;
        WriteNpcLiveField(entry, npcResumeCommandOffset, next);
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 6: {
        const std::byte *const resume = ReadNpcLiveField<const std::byte *>(entry, npcResumeCommandOffset);
        WriteNpcLiveField(entry, npcCurrentCommandOffset, resume ? resume : next);
        break;
    }
    case 7: {
        entry.currentMotionId = ReadU16(command + 4);
        entry.currentAreaPoint = ReadU16(command + 8);
        RestoreNpcAreaPointSeedPosition(entry);
        entry.euler.y = ReadF32(command + 12);
        entry.motionSpeed = ReadF32(command + 16);
        entry.currentCommand = next;
        if (entry.node &&
            (entry.currentMotionId != GetMotionAnimationId(entry.motionWork, MotionRegion::LowerBody) ||
                !motnIsCurrentAnimOpenEnded(entry.motionWork, MotionRegion::LowerBody))) {
            mtwkSetRequestedRootMode(entry.motionWork, 1);
            SetMotionFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
            if (entry.flags & 0xa000U)
                entry.localPosition.y = entry.forcedY;
            else
                FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            SetCharacterWorldRotation(entry.node, rotation);
            mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
            SetMotionSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
            entry.streamRadius = 0.30000001f;
        }
        entry.flags |= npcMotionStateActive;
        entry.liveState = 3;
        ResolveNpcAreaPointOwnerAnchor(entry, entry.currentAreaPoint);
        SetNpcRouteDirection(entry, entry.currentAreaPoint);
        break;
    }
    case 9: {
        entry.currentMotionId = ReadU16(command + 4);
        entry.currentAreaPoint = ReadU16(command + 8);
        RestoreNpcAreaPointSeedPosition(entry);
        entry.euler.y = ReadF32(command + 12);
        entry.currentCommand = next;
        entry.motionSpeed = 1.0f;
        if (entry.node) {
            ResumeNpcScheduleMotion(entry);
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags &= ~(CharacterControlApplyGravity | CharacterControlEvaluateMotion);
        }
        entry.liveState = 5;
        entry.flags |= npcMotionStateActive;
        ResolveNpcAreaPointOwnerAnchor(entry, entry.currentAreaPoint);
        SetNpcRouteDirection(entry, entry.currentAreaPoint);
        break;
    }
    case 10: {
        entry.currentAreaPoint = ReadU16(command + 4);
        RestoreNpcAreaPointSeedPosition(entry);
        entry.euler.y = ReadF32(command + 8);
        entry.currentCommand = next;
        entry.motionSpeed = 1.0f;
        if (entry.node &&
            (entry.currentMotionId != GetMotionAnimationId(entry.motionWork, MotionRegion::LowerBody) ||
                !motnIsCurrentAnimOpenEnded(entry.motionWork, MotionRegion::LowerBody))) {
            mtwkSetRequestedRootMode(entry.motionWork, 1);
            SetMotionFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
            if (entry.flags & 0xa000U)
                entry.localPosition.y = entry.forcedY;
            else
                FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            SetCharacterWorldRotation(entry.node, rotation);
            mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
            SetMotionSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
            entry.streamRadius = 0.30000001f;
        }
        entry.flags |= npcMotionStateActive;
        entry.liveState = 6;
        entry.routeDirectionMode = 1;
        ResolveNpcAreaPointOwnerAnchor(entry, entry.currentAreaPoint);
        break;
    }
    case 11: {
        const std::uint32_t packed = ReadU32(command + 4) | (ReadU32(command + 16) << 16);
        const NpcConditionResult condition = EvaluateNpcCondition(entry, packed, ReadU32(command + 8), currentTime);
        if (condition == NpcConditionResult::True && ReadU32(command + 16) <= 1) {
            ResetNpcAreaRouteState(entry);
        }
        if (condition != NpcConditionResult::True || !SwitchNpcBranch(entry, ReadU32(command + 12), true)) {
            WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        }
        keepExecuting = true;
        break;
    }
    case 12:
    case 39:
        keepExecuting = BranchNpcOnLoadedArea(entry, opcode) != 0;
        break;
    case 13:
    case 14:
    case 15:
    case 57:
        keepExecuting = BranchNpcOnCalendarOrPeerState(entry, opcode) != 0;
        break;
    case 19:
        BeginNpcMotionClip(entry);
        break;
    case 40:
        BeginNpcTimedMotion(entry);
        break;
    case 20:
        entry.flags |= 0x20U;
        entry.currentCommand = next;
        SetNpcStreamLifecycleState(entry, 0);
        keepExecuting = true;
        break;
    case 22:
        entry.flags |= NpcRouteStreamForceHidden;
        entry.currentCommand = next;
        if (entry.node) {
            EV_SetCharDisplay(entry.node, CVM_All, 0);
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags &= ~CharacterControlVoxelSweep;
        }
        keepExecuting = true;
        break;
    case 21:
        entry.flags &= ~0x20U;
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 24:
        entry.eyeControlMode = static_cast<std::int8_t>(command[4]);
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 27: {
        const bool forced = ReadU32(command + 4) != 0;
        if (forced) {
            entry.flags |= npcForcedYPosition;
        } else {
            entry.flags &= ~npcForcedYPosition;
        }
        const float forcedY = forced ? static_cast<float>(static_cast<std::int32_t>(ReadU32(command + 8))) : 0.0f;
        WriteNpcLiveField(entry, npcForcedYPositionOffset, forcedY);
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    }
    case 28:
        entry.streamDistanceBias = ReadF32(command + 4);
        entry.currentCommand = command + 8;
        keepExecuting = true;
        break;
    case 30:
        QueueNpcGlobalBranch(entry, command);
        entry.currentCommand = next;
        keepExecuting = true;
        break;
    case 31: {
        const std::uint32_t packed = ReadU32(command + 4) | (ReadU32(command + 8) << 16);
        for (auto &trigger : npcGlobalBranchTriggers) {
            if (trigger.owner.name == entry.name.name && trigger.packedCondition == packed) {
                trigger.packedCondition = 0xffffffffU;
                --entry.globalBranchCount;
                break;
            }
        }
        entry.currentCommand = next;
        keepExecuting = true;
        break;
    }
    case 33:
        entry.packedMoveFlags = (entry.packedMoveFlags & 0x100ffU) | (ReadU32(command + 4) << 8);
        entry.stateContext.moveFlags = entry.packedMoveFlags;
        entry.currentCommand = next;
        keepExecuting = true;
        break;
    case 36:
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 37: {
        const float x = npcPlayerPosition.x - entry.localPosition.x;
        const float z = npcPlayerPosition.z - entry.localPosition.z;
        if (x * x + z * z <= ReadF32(command + 4) || !SwitchNpcBranch(entry, ReadU32(command + 8), true))
            entry.currentCommand = next;
        keepExecuting = true;
        break;
    }
    case 48: {
        const std::uint32_t selector = ReadU32(command + 4);
        if ((selector ? selector != 1 || npcEndOfDayWeatherForecast < 0.15f : npcEndOfDayWeatherForecast >= 0.15f) ||
            !SwitchNpcBranch(entry, ReadU32(command + 8), true))
            entry.currentCommand = next;
        keepExecuting = true;
        break;
    }
    case 38:
        if (ReadU32(command + 12)) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(ReadU32(command + 4)), static_cast<std::int32_t>(ReadU32(command + 8)));
        } else {
            EV_stepSetFlags(static_cast<std::uint8_t>(ReadU32(command + 8)), static_cast<std::int32_t>(ReadU32(command + 4)));
        }
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 43:
        QueueNpcLocalBranch(entry, command);
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 44: {
        const std::uint32_t packed = ReadU32(command + 4) | (ReadU32(command + 8) << 16);
        for (std::size_t index = 0; index < 4; ++index) {
            const std::size_t offset = npcLocalBranchesOffset + index * sizeof(NpcLocalBranchTrigger);
            if (ReadNpcLiveField<std::uint32_t>(entry, offset) == packed) {
                WriteNpcLiveField(entry, offset, std::uint32_t{0xffffffffU});
                --entry.localBranchCount;
                break;
            }
        }
        entry.currentCommand = next;
        keepExecuting = true;
        break;
    }
    case 45:
        SetNpcMotionAttachment(entry, ReadU16(command + 4), static_cast<std::uint8_t>(command[8]) | 0x20U, 1);
        entry.currentCommand = next;
        keepExecuting = true;
        break;
    case 46:
        ClearNpcMotionAttachment(entry, 1);
        entry.currentCommand = next;
        keepExecuting = true;
        break;
    case 49:
        if (ReadU32(command + 4)) {
            entry.flags &= ~0x00080000U;
        } else {
            entry.flags |= 0x00080000U;
        }
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 50:
        if (ReadU32(command + 4)) {
            entry.flags |= 0x00400000U;
        } else {
            entry.flags &= ~0x00400000U;
        }
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 51: {
        std::uint32_t controlFlags = ReadNpcLiveField<std::uint32_t>(entry, npcControlFlagsOffset);
        if (ReadU32(command + 4)) {
            entry.motionAttachments[0].flags &= ~0x40U;
            controlFlags &= ~0x10U;
        } else {
            controlFlags |= 0x10U;
        }
        WriteNpcLiveField(entry, npcControlFlagsOffset, controlFlags);
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    }
    case 52: {
        const std::uint32_t mode = ReadU32(command + 4);
        if (mode == 0 || mode == 1) {
            if (mode)
                entry.flags &= ~0x08000000U;
            else
                entry.flags |= 0x08000000U;
            if (entry.node) {
                const std::uint32_t flags = GetCharacterControlModelFlags(entry.node);
                SetCharacterControlModelFlags(entry.node, (flags & ~0x804U) | (mode ? 4U : 0x800U));
            }
        }
        entry.currentCommand = next;
        keepExecuting = true;
        break;
    }
    case 53:
        if (ReadU32(command + 4))
            entry.packedMoveFlags |= 0x10000U;
        else
            entry.packedMoveFlags &= ~0x10000U;
        entry.currentCommand = next;
        keepExecuting = true;
        break;
    case 54:
        if (ReadU32(command + 4))
            UpdateNpcScheduleSeedSlot(entry.name, shen_name{}, 1);
        else
            UpdateNpcScheduleSeedSlot(shen_name{}, entry.name, 1);
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 55:
        WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
        keepExecuting = true;
        break;
    case 26:
    case 58:
        if ((entry.node && SetMotionRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody)) || (!entry.node && opcode == 58) || npcIsTimeAdvancing == 1) {
            WriteNpcLiveField(entry, npcCurrentCommandOffset, next);
            entry.liveState = 0;
            entry.scheduleProgress = 1;
            keepExecuting = true;
        }
        break;
    default:
        break;
    }

    WriteNpcLiveField(entry, npcBranchCommandOffset, command);
    return keepExecuting;
}

void InterpretNpcSchedule(NpcRouteLiveEntry &entry, std::int32_t currentTime) {
    NpcRouteRuntimeRecord *const runtime = GetNpcRuntimeRecord(entry);
    if (!runtime)
        return;

    const std::byte *const timedCommand = ReadNpcLiveField<const std::byte *>(entry, npcNextTimedCommandOffset);
    if (timedCommand && (entry.flags & npcMotionStateActive) != 0 && currentTime >= ReadNpcLiveField<std::int32_t>(entry, npcNextTimedCommandTimeOffset)) {
        WriteNpcLiveField(entry, npcCurrentCommandOffset, timedCommand);
        WriteNpcLiveField<std::int32_t>(entry, npcDelayDurationOffset, 0);
        entry.scheduleProgress = 1;
    }
    EvaluateNpcDeferredBranches(entry, currentTime);
    if (entry.scheduleProgress == 4) {
        npcTimeAdvanceBreak = 1;
        return;
    }

    if (entry.scheduleProgress == 3)
        return;
    if (entry.scheduleProgress == 0) {
        if (!PrimeNpcInterpreter(entry))
            return;
    } else if (entry.scheduleProgress == 2) {
        const std::int32_t start = ReadNpcLiveField<std::int32_t>(entry, npcDelayStartOffset);
        const std::int32_t duration = ReadNpcLiveField<std::int32_t>(entry, npcDelayDurationOffset);
        if (currentTime - start < duration)
            return;
        const std::int32_t noDelay = 0;
        WriteNpcLiveField(entry, npcDelayDurationOffset, noDelay);
        entry.scheduleProgress = 1;
        return;
    } else if (entry.scheduleProgress != 1) {
        return;
    }

    for (std::size_t iteration = 0; iteration < 63; ++iteration) {
        if (entry.stateCallback) {
            const std::int64_t result = entry.stateCallback(&entry);
            if (!result)
                entry.stateCallback = nullptr;
            else if (result == 1)
                return;
        }
        std::int32_t result = ExecuteNpcCommand(entry, currentTime);
        if (result == 2) {
            npcTimeAdvanceBreak = 1;
            result = 0;
        }
        EvaluateNpcDeferredBranches(entry, currentTime);
        if (!result)
            return;
    }
}

void LoadNpcMotionBank(const char *directory, const char *filename) {
    const std::int32_t handle = LoadMOTNPCTable(directory, filename);
    if (handle >= 0)
        npcMotionBankHandles[npcMotionBankHandleCount++] = handle;
}

void LoadNpcMotionProps(const char *directory, const char *filename, shen_name allocationName = SHEN_NAME(NPCp)) {
    const std::int32_t handle = LoadMotionProps(directory, filename, allocationName);
    if (handle >= 0)
        npcMotionPropHandles[npcMotionPropHandleCount++] = handle;
}

bool LoadNpcProfileMotionBanks(std::span<const std::byte> profile, std::uint32_t listOffset) {
    if (!listOffset)
        return true;
    char directory[32];
    std::snprintf(directory, sizeof(directory), "scene/%02d/npc", g_actNumber);
    for (std::size_t offset = listOffset; Contains(profile, offset, 4); offset += 4) {
        const std::uint32_t filenameOffset = ReadU32(profile.data() + offset);
        if (!filenameOffset)
            return true;
        std::string filename;
        if (!ReadCString(profile, filenameOffset, filename))
            return false;
        if (filename.find(".MOT") != std::string::npos)
            LoadNpcMotionBank(directory, filename.c_str());
        else
            LoadNpcMotionProps(directory, filename.c_str());
    }
    return false;
}

void FreeNpcMotionBanks() {
    for (std::size_t index = 0; index < npcMotionBankHandleCount; ++index)
        DeleteMotion(npcMotionBankHandles[index]);
    npcMotionBankHandleCount = 0;
    for (std::size_t index = 0; index < npcMotionPropHandleCount; ++index)
        DeleteMotionProps(npcMotionPropHandles[index]);
    npcMotionPropHandleCount = 0;
    npcMotionBanksLoaded = false;
}

void FreeNpcMotionClips() {
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.motionWork)
            SetMotionDrawOverrideData(entry.motionWork, nullptr);
    }
    for (NpcMotionClip &clip : npcMotionClips) {
        HLib::FreeTaskData(clip.allocation);
        clip = {};
    }
    npcMotionClipsLoaded = false;
}

bool LoadNpcMotionClips(std::span<const std::byte> profile, std::uint32_t listOffset) {
    if (npcMotionClipsLoaded)
        return true;
    if (!listOffset) {
        npcMotionClipsLoaded = true;
        return true;
    }
    for (std::size_t index = 0; index <= npcMotionClips.size(); ++index) {
        const std::size_t offset = listOffset + index * 4;
        if (!Contains(profile, offset, 4))
            return false;
        const std::uint32_t filenameOffset = ReadU32(profile.data() + offset);
        if (!filenameOffset) {
            npcMotionClipsLoaded = true;
            return true;
        }
        if (index == npcMotionClips.size())
            return false;
        std::string filename;
        if (!ReadCString(profile, filenameOffset, filename))
            return false;
        if (filename == "none")
            continue;
        std::array<char, 128> path{};
        const int length = std::snprintf(path.data(), path.size(), "scene/%02d/npc/%s", g_actNumber, filename.c_str());
        if (length < 0 || static_cast<std::size_t>(length) >= path.size())
            return false;
        const std::uint64_t size = ArchiveData_GetFileSize(path.data());
        if (!size)
            continue;
        if (size < 0x40 || size > static_cast<std::uint64_t>((std::numeric_limits<std::int32_t>::max)()))
            return false;
        NpcMotionClip &clip = npcMotionClips[index];
        clip.allocation = static_cast<std::byte *>(HLib::AllocateStaticOrDynamicBlock(static_cast<std::uint32_t>(size), SHEN_NAME(NPCM)));
        if (!clip.allocation)
            continue;
        HLib::SetTaskDataMotionFlag(clip.allocation);
        if (!ArchiveData_Read(path.data(), 0, clip.allocation, size)) {
            HLib::FreeTaskData(clip.allocation);
            clip = {};
            continue;
        }
        clip.boneCount = ReadU32(clip.allocation);
        clip.frameCount = ReadU32(clip.allocation + 4);
        if (!clip.boneCount || !clip.frameCount || static_cast<std::uint64_t>(clip.boneCount) * clip.frameCount > (size - 0x40) / sizeof(Matrix4x4))
            return false;
        clip.frames = reinterpret_cast<const Matrix4x4 *>(clip.allocation + 0x40);
    }
    return false;
}

bool SelectorMatches(std::uint32_t selector, shen_name area) {
    for (std::uint32_t shift = 0; shift < 32; shift += 8) {
        const unsigned char pattern = static_cast<unsigned char>((selector >> shift) & 0xffU);
        const unsigned char value = static_cast<unsigned char>((area.name >> shift) & 0xffU);
        if (pattern != static_cast<unsigned char>('*') && pattern != value)
            return false;
    }
    return true;
}

bool ReadSelectorList(std::span<const std::byte> bytes, std::uint32_t offset, shen_name area, bool &matched) {
    matched = false;
    if (!offset)
        return false;
    for (std::size_t cursor = offset; Contains(bytes, cursor, 4); cursor += 4) {
        const std::uint32_t selector = ReadU32(bytes.data() + cursor);
        if (!selector)
            return true;
        matched |= SelectorMatches(selector, area);
    }
    return false;
}

bool SelectNpcScheduleFilename(std::span<const std::byte> bytes, shen_name area, std::int32_t act, std::string &filename, std::uint32_t &clipListOffset,
    std::uint32_t &motionListOffset, std::uint8_t &motionMode, const char *requestedFilename = nullptr) {
    npcUsePositionCullCallback = false;
    npcPoolTarget = 34;
    npcPoolMinimum = -1;
    if (!Contains(bytes, 0, 4))
        return false;
    const std::uint32_t version = ReadU32(bytes.data());
    if (version != 6 && version != 7)
        return true;

    for (std::size_t offset = 4; Contains(bytes, offset, 0x1c); offset += 0x1c) {
        const std::uint32_t selectorOffset = ReadU32(bytes.data() + offset);
        if (!selectorOffset)
            return true;
        bool matches = false;
        std::string rowFilename;
        if (!ReadSelectorList(bytes, selectorOffset, area, matches) || !ReadCString(bytes, ReadU32(bytes.data() + offset + 4), rowFilename)) {
            return false;
        }
        const auto requiredFlag = static_cast<std::int16_t>(ReadU16(bytes.data() + offset + 0x0e));
        const std::uint8_t actMask = static_cast<std::uint8_t>(bytes[offset + 0x19]);
        if (filename.empty() && matches && (!actMask || (act >= 0 && act < 8 && (actMask & static_cast<std::uint8_t>(1U << act)) != 0)) &&
            (requestedFilename ? rowFilename == requestedFilename : (requiredFlag < 0 || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, requiredFlag)))) {
            filename = std::move(rowFilename);
            clipListOffset = ReadU32(bytes.data() + offset + 0x10);
            motionListOffset = ReadU32(bytes.data() + offset + 8);
            motionMode = static_cast<std::uint8_t>(bytes[offset + 0x18]);
            npcUsePositionCullCallback = (ReadU32(bytes.data() + offset + 0x18) & 0x10000U) != 0;
            const std::int32_t target = static_cast<std::int8_t>(bytes[offset + 0x0c]);
            npcPoolTarget = std::min(36, target + 1 + target / 6);
            npcPoolMinimum = version == 7 ? static_cast<std::int8_t>(bytes[offset + 0x0d]) : -1;
            return true;
        }
    }
    return false;
}

bool ParseNpcSchedule(std::span<std::byte> bytes, std::span<const shen_name> humans, NpcScheduleStorage &parsed) {
    const std::span<const std::byte> view{bytes};
    if (!Contains(view, 0, 0x70) || ReadU32(view.data()) != 4)
        return false;

    const std::uint32_t recordCount = ReadU32(view.data() + 8);
    const std::uint32_t recordTableOffset = ReadU32(view.data() + 0x0c);
    const std::uint32_t areaPointCount = ReadU32(view.data() + 0x10);
    const std::uint32_t areaPointTableOffset = ReadU32(view.data() + 0x14);
    const std::uint32_t routeNodeCount = ReadU32(view.data() + 0x18);
    const std::uint32_t routeNodeTableOffset = ReadU32(view.data() + 0x1c);
    const std::uint32_t routeResetOffset = ReadU32(view.data() + 0x68);
    const std::uint32_t entryScalarOffset = ReadU32(view.data() + 0x6c);
    const std::uint64_t recordTableSize = static_cast<std::uint64_t>(recordCount) * 4;
    const std::uint64_t areaPointTableSize = static_cast<std::uint64_t>(areaPointCount) * 0x44;
    const std::uint64_t routeNodeTableSize = static_cast<std::uint64_t>(routeNodeCount) * 0x4c;
    if (recordTableOffset < 0x70 || !Contains(view, recordTableOffset, recordTableSize) || !Contains(view, areaPointTableOffset, areaPointTableSize) ||
        !Contains(view, routeNodeTableOffset, routeNodeTableSize) || static_cast<std::uint64_t>(recordTableOffset) + recordTableSize > areaPointTableOffset ||
        static_cast<std::uint64_t>(areaPointTableOffset) + areaPointTableSize > routeNodeTableOffset ||
        (routeResetOffset && !Contains(view, routeResetOffset, 4)) || (entryScalarOffset && !Contains(view, entryScalarOffset, 4))) {
        return false;
    }
    if (areaPointCount > static_cast<std::uint32_t>((std::numeric_limits<std::uint16_t>::max)())) {
        return false;
    }

    for (std::uint32_t index = 0; index < areaPointCount; ++index) {
        const std::size_t offset = areaPointTableOffset + static_cast<std::size_t>(index) * 0x44;
        const auto routeNode = static_cast<std::int16_t>(ReadU16(view.data() + offset + 4));
        const std::uint32_t routeData = ReadU32(view.data() + offset + 0x40);
        if (routeNode < -1 || (routeNode >= 0 && static_cast<std::uint32_t>(routeNode) >= routeNodeCount) ||
            (routeData &&
                (static_cast<std::uint64_t>(areaPointTableOffset) + routeData < static_cast<std::uint64_t>(areaPointTableOffset) + areaPointTableSize ||
                    static_cast<std::uint64_t>(areaPointTableOffset) + routeData + 0x28 > routeNodeTableOffset))) {
            return false;
        }
    }
    for (std::uint32_t index = 0; index < routeNodeCount; ++index) {
        const std::size_t offset = routeNodeTableOffset + static_cast<std::size_t>(index) * 0x4c;
        const std::uint32_t pointOffset = ReadU32(view.data() + offset);
        const std::uint32_t pointCount = ReadU32(view.data() + offset + 4);
        if (pointCount && !Contains(view, static_cast<std::uint64_t>(routeNodeTableOffset) + pointOffset, static_cast<std::uint64_t>(pointCount) * 12)) {
            return false;
        }
        const std::uint32_t lengthsOffset = ReadU32(view.data() + offset + 8);
        const std::uint32_t boundsOffset = ReadU32(view.data() + offset + 0x3c);
        const std::uint32_t auxiliaryOffset = ReadU32(view.data() + offset + 0x40);
        if (pointCount < 4 ||
            !Contains(view, static_cast<std::uint64_t>(routeNodeTableOffset) + lengthsOffset, static_cast<std::uint64_t>(pointCount - 2) * sizeof(float)) ||
            !Contains(view, static_cast<std::uint64_t>(routeNodeTableOffset) + boundsOffset, static_cast<std::uint64_t>(pointCount - 3) * 2 * sizeof(float)) ||
            !Contains(view, static_cast<std::uint64_t>(routeNodeTableOffset) + auxiliaryOffset, pointCount - 3)) {
            return false;
        }
    }

    parsed.records.reserve(recordCount);
    for (std::uint32_t index = 0; index < recordCount; ++index) {
        const std::uint32_t relative = ReadU32(view.data() + recordTableOffset + index * 4);
        const std::uint64_t offset = static_cast<std::uint64_t>(recordTableOffset) + relative;
        const std::uint64_t end = index + 1 < recordCount ?
            static_cast<std::uint64_t>(recordTableOffset) + ReadU32(view.data() + recordTableOffset + (index + 1) * 4) :
            areaPointTableOffset;
        if (offset < static_cast<std::uint64_t>(recordTableOffset) + recordTableSize || end <= offset || end - offset < 0x38 ||
            !Contains(view, offset, end - offset)) {
            return false;
        }
        const std::uint32_t initialPoint = ReadU32(view.data() + offset + 0x0c);
        const std::uint32_t endpoint = ReadU32(view.data() + offset + 0x10);
        if ((initialPoint != 0xffffffffU && initialPoint >= areaPointCount) || (endpoint != 0xffffffffU && endpoint >= areaPointCount)) {
            return false;
        }

        std::uint16_t recordBranchCount = 0;
        std::uint64_t branchOffset = offset + 0x34;
        for (;;) {
            if (!Contains(view, branchOffset, 4) || branchOffset + 4 > end) {
                return false;
            }
            const std::uint32_t nextRelative = ReadU32(view.data() + branchOffset);
            const std::uint64_t branchEnd = nextRelative ? branchOffset + nextRelative : end;
            if (branchEnd <= branchOffset + 4 || branchEnd > end || ((branchEnd - offset) & 3U) != 0) {
                return false;
            }
            bool terminated = false;
            for (std::uint64_t cursor = branchOffset + 4; cursor < branchEnd;) {
                const std::int32_t opcode = static_cast<std::int32_t>(ReadU32(view.data() + cursor));
                if (opcode == -1) {
                    if (cursor + 4 != branchEnd)
                        return false;
                    terminated = true;
                    break;
                }
                if (opcode < 0 || static_cast<std::size_t>(opcode) >= npcOpcodeSizeDwords.size()) {
                    return false;
                }
                const std::uint64_t commandSize = static_cast<std::uint64_t>(npcOpcodeSizeDwords[static_cast<std::size_t>(opcode)]) * 4;
                if (cursor + commandSize > branchEnd)
                    return false;
                cursor += commandSize;
                ++parsed.commandCount;
            }
            if (!terminated)
                return false;
            ++parsed.branchCount;
            if (recordBranchCount == (std::numeric_limits<std::uint16_t>::max)()) {
                return false;
            }
            ++recordBranchCount;
            if (!nextRelative)
                break;
            branchOffset = branchEnd;
        }

        ParsedNpcRecord record{};
        record.name.name = ReadU32(view.data() + offset);
        record.offset = static_cast<std::uint32_t>(offset);
        record.endOffset = static_cast<std::uint32_t>(end);
        record.branchCount = recordBranchCount;
        for (std::size_t human = 0; human < humans.size(); ++human) {
            if (humans[human].name == record.name.name) {
                if (human > static_cast<std::size_t>((std::numeric_limits<std::int16_t>::max)())) {
                    return false;
                }
                record.humanPackageIndex = static_cast<std::int16_t>(human);
                break;
            }
        }
        parsed.records.push_back(record);
    }

    parsed.areaPointCount = areaPointCount;
    parsed.routeNodeCount = routeNodeCount;
    parsed.areaPointTableOffset = areaPointTableOffset;
    parsed.routeNodeTableOffset = routeNodeTableOffset;
    parsed.routeResetTableOffset = routeResetOffset;
    parsed.entryPointScalarTableOffset = entryScalarOffset;
    parsed.routeFlags = ReadU32(view.data() + 4);
    parsed.routeVisited.resize(routeNodeCount);
    parsed.humanPackages.assign(humans.begin(), humans.end());
    parsed.bytes = bytes;
    return true;
}

bool IsNpcRouteArea(shen_name area) {
    if (area.name == SHEN_NAME(0015) || area.name == SHEN_NAME(0138)) {
        return true;
    }
    const auto first = static_cast<unsigned char>(area.name & 0xffU);
    if (first < static_cast<unsigned char>('A') || first > static_cast<unsigned char>('W')) {
        return false;
    }
    return (0x410401U & (1U << (first - static_cast<unsigned char>('A')))) != 0;
}

// Map teardown: the act tables (NPC.BIN, the route-entry pool, its slot flags, the human index and STRL) stay allocated.
void ReleaseNpcMapResources() {
    FreeNpcMotionClips();
    FreeNpcRouteLiveEntries();
    ReleaseNativeNpcHumanPackages();
    FreeNpcMotionBanks();
    ResetNpcAreaRouteResources();
    npcScheduleFilename = {};
}



bool npcInitDataInternal() {
    // InitNpcPool: act-specific NPCC capacity includes six
    // script-owned entries. The final four allocation bytes are not an NPC.
    static constexpr std::array<std::size_t, 5> counts{473, 473, 473, 320, 51};
    if (g_loadedActNumber < 0 || static_cast<std::size_t>(g_loadedActNumber) >= counts.size())
        return false;
    std::array<char, 64> masterPath{};
    std::snprintf(masterPath.data(), masterPath.size(), "scene/%02d/npc/%s", g_loadedActNumber, g_loadedActNumber == 3 ? "NPC_YUAN.BIN" : "NPC.BIN");
    const std::uint64_t size = ArchiveData_GetFileSize(masterPath.data());
    if (!size || size > (std::numeric_limits<std::size_t>::max)())
        return false;
    auto *const binary = static_cast<std::byte *>(HLib::AllocateStaticOrDynamicBlock(size, SHEN_NAME(NPCB)));
    if (!binary)
        return false;
    HLib::SetTaskDataMotionFlag(binary);
    npcBinaryStorage = {binary, static_cast<std::size_t>(size)};
    const std::size_t count = counts[g_loadedActNumber] + npcScriptEntryCount;
    auto *const entries = static_cast<NpcRouteLiveEntry *>(HLib::AllocateStaticOrDynamicBlock(sizeof(NpcRouteLiveEntry) * count + 4, SHEN_NAME(NPCC)));
    if (!entries)
        return false;
    HLib::SetTaskDataMotionFlag(entries);
    npcEntryStorage = {entries, count};
    auto *const slots = static_cast<std::uint8_t *>(HLib::AllocateStaticOrDynamicBlock(count, SHEN_NAME(NPCS)));
    if (!slots)
        return false;
    HLib::SetTaskDataMotionFlag(slots);
    npcSlotFlags = {slots, count};
    return InitializeNpcAreaRouteStore();
}

void RefreshNpcScriptRouteMotion(NpcRouteStateContext &context) {
    NpcRouteLiveEntry &entry = *context.owner;
    if (!entry.node) {
        context.routeFlags |= 0x1000U;
        return;
    }
    if (static_cast<std::uint32_t>(GetCharacterBoneId(entry.node) - 3) <= 2U && (context.flags & 1U) &&
        !(static_cast<std::uint8_t>(entry.recordBase[0x0b]) & 0x20U) && !(context.flags & 0x10U)) {
        context.flags = (context.flags & ~0x50U) | 0x10U;
        InitializeNpcLocomotionProfile(context);
        RequestNpcRouteMotion(context, false);
    }
    context.routeFlags &= ~0x1000U;
}

bool MoveForNpcInData(NpcRouteLiveEntry &entry, std::uint32_t area, bool apply) {
    if (!entry.scheduleProgress)
        return false;
    const std::uint8_t state = entry.liveState == 2 ? entry.previousLiveState : entry.liveState;
    if (state == 1) {
        if (entry.stateContext.phase != 7 || (entry.stateContext.flags & 0x1000000U))
            return false;
    } else if (state == 4 || state == 7 || state == 8) {
        return false;
    }
    if ((entry.flags & 0x800U) || !npcSceneCheckEnabled)
        return false;
    if (!apply && !area)
        return true;
    std::uint16_t target = 0xffff;
    for (std::uint32_t index = 0; index < npcSchedule.areaPointCount; ++index) {
        const std::byte *const point = GetNpcAreaPoint(static_cast<std::uint16_t>(index));
        if (ReadU32(point) == area) {
            target = static_cast<std::uint16_t>(index);
            break;
        }
    }
    if (target == 0xffff)
        return false;
    if (!apply)
        npcCachedAreaRouteContext = entry.stateContext;
    NpcRouteStateContext &context = apply ? entry.stateContext : npcCachedAreaRouteContext;
    context.pendingAreaPoint = target;
    context.scriptOwner = entry.name;
    bool restoreRunning = true;
    if (context.flags & 1U) {
        restoreRunning = (context.flags & 0x4000U) != 0;
        if (context.flags & 2U)
            UpdateNpcRouteWindow(context, context.parameter);
        context.avoidanceResult = 0;
        context.avoidanceAuxiliary = -1;
        context.routeFlags = 0;
        context.previousSegment = -10;
        context.flags &= 0x20002U;
        context.doorWaitFrames = -1;
        context.phase = 0;
        context.otherName = {};
        context.movementProgress = 0.0f;
        context.field0f4 = 0;
        context.avoidanceWaitFrames = 0;
        context.other = nullptr;
        context.field0a1 = 0;
        context.avoidancePhase = 0;
        context.stuckCounter = 0;
    } else {
        context.currentAreaPoint = entry.currentAreaPoint;
        entry.pendingAreaPoint = entry.currentAreaPoint;
        const std::uint32_t yaw = static_cast<std::uint32_t>(ReadNpcEulerYaw(entry.euler));
        context.arrivalYaw = static_cast<std::uint16_t>(yaw);
        context.field10a = static_cast<std::uint16_t>(yaw >> 16);
    }
    context.flags |= 1U;
    context.routeFlags |= 7U;
    npcActiveRouteContext = &context;
    context.motionScale = 1.0f;
    InitializeNpcLocomotionProfile(context);
    SetNpcLocomotionScale(context, 1.0f);
    context.flags |= 0x100U;
    context.phase = 0;
    context.field036 = 1;
    while (context.phase <= 2) {
        switch (context.phase) {
        case 0:
            EnterNpcRoute(context);
            break;
        case 1:
            BuildNpcRoute(context);
            break;
        case 2:
            BindNpcRouteWindow(context);
            break;
        }
        if (!(context.flags & 2U))
            continue;
        if (restoreRunning)
            context.flags |= 0x4000U;
        context.routeFlags |= 0x100000U;
        RefreshNpcScriptRouteMotion(context);
        if (apply) {
            entry.field32f = entry.scheduleProgress;
            if (entry.liveState == 2) {
                entry.field331 = entry.previousLiveState;
                entry.previousLiveState = 1;
            } else {
                entry.field331 = entry.liveState;
                entry.liveState = 1;
            }
            entry.scheduleProgress = 3;
        } else {
            npcCachedAreaRouteCharacter = entry.name;
            npcCachedAreaRouteId = area;
        }
        entry.stateCallback = NpcRouteScheduleCallback;
        context.floor = static_cast<std::int8_t>(context.moveFlags >> 8);
        context.moveFlags = (entry.packedMoveFlags & 0x100ffU) | 0x100U;
        return true;
    }
    if ((context.flags & 2U) && apply && entry.liveState == 1) {
        InitializeNpcRouteRequest(context, entry.currentAreaPoint, entry.pendingAreaPoint);
        SetNpcLocomotionScale(context, entry.motionSpeed);
    }
    context.flags &= ~0x100U;
    return false;
}
} // namespace npc_package_detail

bool RequestNpcAreaTransfer(shen_name character, std::uint32_t area) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry)
        return false;
    if (character.name == npcCachedAreaRouteCharacter.name && area == npcCachedAreaRouteId) {
        entry->stateContext = npcCachedAreaRouteContext;
        entry->stateContext.motionWork = entry->motionWork;
        entry->field32f = entry->scheduleProgress;
        const std::uint8_t state = entry->liveState == 2 ? entry->previousLiveState : entry->liveState;
        if (entry->liveState == 2)
            entry->previousLiveState = 1;
        else
            entry->liveState = 1;
        entry->field331 = state;
        entry->scheduleProgress = 3;
    } else if (!MoveForNpcInData(*entry, area, true)) {
        return false;
    }
    if (npcScriptRouteOwner && npcScriptRouteOwner != entry)
        ReleaseNpcScriptRouteOwner();
    npcScriptRouteOwner = entry;
    UpdateNpcScheduleSeedSlot(entry->name, {}, 0);
    entry->flags |= 0x200U;
    if (entry->node) {
        std::uint32_t flags = GetCharacterControlModelFlags(entry->node);
        flags = (entry->flags & 0x8000000U) ? flags | 0x800U : flags & ~0x800U;
        if (npcSchedule.floor == -1000 || !(entry->packedMoveFlags & 0xff00U))
            flags |= 2U;
        SetCharacterControlModelFlags(entry->node, flags | 4U);
    }
    return true;
}
namespace npc_package_detail {

void ApplyNpcScriptStateSeedPayload(NpcRouteLiveEntry &entry, const NpcSavedRouteState &saved) {
    const float savedYaw = entry.euler.y;
    const Vector3 savedPosition{saved.localX, 0.0f, saved.localZ};
    NpcRouteStateContext &context = entry.stateContext;
    entry.worldPosition = savedPosition;
    entry.localPosition = NpcRouteAreaToScene(savedPosition);
    context.localPosition = savedPosition;
    context.scenePosition = entry.localPosition;
    context.moveFlags = entry.packedMoveFlags;
    if (saved.flags & 0x100U) {
        if (saved.flags & 0x4000U) {
            SetNpcYawBits(entry, static_cast<std::int16_t>(saved.arrivalYaw));
            entry.scheduleProgress = saved.scheduleProgress;
            entry.liveState = saved.liveState;
        } else {
            context.phase = 7;
            context.flags |= 1U;
            context.pendingAreaPoint = saved.pendingAreaPoint;
            if (saved.flags & 4U) {
                context.activeNode = static_cast<std::int16_t>(saved.currentAreaPoint);
                context.flags |= 2U;
            } else {
                context.currentAreaPoint = saved.currentAreaPoint;
            }
        }
        const std::uint32_t area = ReadU32(GetNpcAreaPoint(saved.pendingAreaPoint));
        if (!RequestNpcAreaTransfer(entry.name, area))
            return;
        if (!(saved.flags & 8U)) {
            context.routeFlags &= ~1U;
            TickNpcPausedRoute(*npcActiveRouteContext);
        }
    } else if (saved.flags & 0x8200U) {
        InitializeNpcRouteRequest(context, saved.currentAreaPoint, saved.pendingAreaPoint);
        SetNpcLocomotionScale(context, entry.motionSpeed);
        context.flags |= 0x200U;
        if (saved.flags & 0x4000U) {
            context.routeFlags |= 0x20000U;
            context.flags |= 0x4000U;
        }
        entry.field338 = saved.routeMode;
        if (saved.flags & 0x8000U) {
            if (static_cast<std::int8_t>(saved.routeMode) >= 0)
                entry.routeDirectionMode = 11;
            else
                entry.eyeControlMode = 11;
            context.routeFlags |= 1U;
            context.flags |= 0x8000U;
            npcScriptRouteOwner = &entry;
            context.waitStartTime = 120 * (npcCurrentDateTime.minute + 60 * npcCurrentDateTime.hour);
        } else {
            entry.flags |= 0x10U;
            InitializeNpcSpecialRouteDirection(entry);
            entry.flags &= ~0x10U;
        }
    } else {
        if (saved.flags & 4U) {
            context.flags |= 2U;
            context.activeNode = static_cast<std::int16_t>(saved.currentAreaPoint);
        }
        InitializeNpcRouteRequest(context, saved.currentAreaPoint, saved.pendingAreaPoint);
        SetNpcLocomotionScale(context, entry.motionSpeed);
    }
    if (saved.flags & 0x10U)
        SetNpcRouteMotionState(context, 1);
    if (!(saved.routeFlags & 2U))
        context.routeFlags &= ~2U;
    if (!(saved.routeFlags & 4U))
        context.routeFlags &= ~4U;
    if (saved.phase == 7) {
        context.routeFlags |= 0x400000U;
        TickNpcRouteStateContext(&context);
        while (context.phase != 7 && (context.flags & 1U) && context.phase != 4)
            TickNpcRouteStateContext(&context);
        std::int32_t parameter;
        if (saved.flags & 4U) {
            NpcRouteStateContext copy = context;
            parameter = FindNearestNpcParameter(copy, savedPosition);
        } else {
            parameter = static_cast<std::int16_t>(saved.parameter);
        }
        TickNpcRouteStateContext(&context);
        while (context.parameter < parameter && (context.flags & 1U) && context.phase != 4)
            TickNpcRouteStateContext(&context);
        context.routeFlags &= ~0x400000U;
    } else if (saved.phase == 8) {
        context.moveState = 0;
        context.phase = 8;
    }
    entry.worldPosition = savedPosition;
    entry.localPosition = NpcRouteAreaToScene(savedPosition);
    context.localPosition = savedPosition;
    context.scenePosition = entry.localPosition;
    context.savedEuler.y = savedYaw;
    entry.euler.y = savedYaw;
}

void RestoreNpcAreaPointSeedPosition(NpcRouteLiveEntry &entry) {
    const std::byte *const point = GetNpcAreaPoint(entry.currentAreaPoint);
    std::memcpy(&entry.localPosition, point + 0x34, sizeof(Vector3));
    entry.localPosition.x -= npcSchedule.areaOrigin.x;
    entry.localPosition.z -= npcSchedule.areaOrigin.z;
    const std::uint32_t routeFlags = ReadU32(GetNpcRouteNode(ReadU16(point + 4)) + 0x48);
    entry.packedMoveFlags = (entry.packedMoveFlags & 0x1ff00U) | static_cast<std::uint8_t>(routeFlags);
    if (entry.flags & 0xa000U)
        entry.localPosition.y = entry.forcedY;
    entry.worldPosition = NpcAreaPosition(entry.localPosition);
    if (routeFlags & 0x400U)
        entry.flags |= 0x100000U;
    else
        entry.flags &= ~0x100000U;
}

shen_name ApplyNpcScheduleSeed(const NpcScheduleSeed &seed, std::int32_t currentTime) {
    if (seed.kind == NpcScheduleSeedKind::Empty)
        return {};
    NpcRouteLiveEntry *entry = nullptr;
    for (NpcRouteLiveEntry &candidate : npcRouteEntries) {
        if (candidate.name.name == seed.name.name) {
            entry = &candidate;
            break;
        }
    }
    if (!entry)
        return {};
    InitializeNpcInterpreterState(*entry, entry->poolIndex);
    const std::byte *timed = entry->currentCommand;
    while (ReadU32(timed) < npcOpcodeSizeDwords.size()) {
        const std::uint32_t opcode = ReadU32(timed);
        if (opcode == 1)
            break;
        timed += static_cast<std::size_t>(npcOpcodeSizeDwords[opcode]) * 4;
    }
    if (ReadU32(timed) != 1)
        timed = nullptr;
    entry->nextTimedCommand = timed;
    if (timed) {
        const std::uint32_t packedTime = ReadU32(timed + 4);
        entry->nextTimedCommandTime = 120 * (static_cast<std::uint8_t>(packedTime) + 60 * static_cast<std::uint8_t>(packedTime >> 8));
        while (entry->nextTimedCommand && currentTime >= entry->nextTimedCommandTime)
            AdvanceNpcTimedSequenceSlot(*entry, currentTime);
    }
    const std::byte *const savedCommand = entry->recordBase + 0x38 + seed.currentCommandOffset;
    const std::uint32_t opcode = ReadU32(savedCommand);
    if (opcode && static_cast<std::int32_t>(opcode) != static_cast<std::int8_t>(seed.currentOpcode))
        return {};
    entry->flags |= 0x10000000U;
    const std::int32_t previousTimeAdvancing = npcIsTimeAdvancing;
    npcIsTimeAdvancing = 0;
    for (const std::uint16_t offset : seed.localTriggerOffsets) {
        if (offset == 0xffff)
            break;
        entry->currentCommand = entry->recordBase + 0x38 + offset;
        if (ReadU32(entry->currentCommand) != 43) {
            InitializeNpcInterpreterState(*entry, entry->poolIndex);
            return {};
        }
        ExecuteNpcCommand(*entry, currentTime);
    }
    for (const std::uint16_t offset : seed.globalTriggerOffsets) {
        if (offset == 0xffff)
            break;
        entry->currentCommand = entry->recordBase + 0x38 + offset;
        if (ReadU32(entry->currentCommand) != 30) {
            InitializeNpcInterpreterState(*entry, entry->poolIndex);
            return {};
        }
        ExecuteNpcCommand(*entry, currentTime);
    }
    npcIsTimeAdvancing = previousTimeAdvancing;
    entry->packedMoveFlags =
        static_cast<std::uint32_t>(static_cast<std::uint8_t>(seed.packedMoveFlagsLow) | (static_cast<std::int8_t>(seed.packedMoveFlagsHigh) * 256));
    if (seed.packedMoveFlagsLow & 0x100U)
        entry->packedMoveFlags |= 0x10000U;
    entry->currentCommand = entry->branchCommandStart = savedCommand;
    entry->activeBranchIndex = static_cast<std::int8_t>(seed.activeBranchIndex);
    if (seed.resumeCommandOffset)
        entry->resumeCommand = entry->recordBase + 0x38 + seed.resumeCommandOffset;
    entry->currentAreaPoint = seed.currentAreaPoint;
    entry->pendingAreaPoint = seed.pendingAreaPoint;
    PrimeNpcInterpreter(*entry);
    entry->flags = seed.flags | 0x10000000U;
    entry->controlFlags = seed.controlFlagsAndForcedY & 0xffffU;
    if (seed.controlBit4)
        entry->controlFlags |= 0x10U;
    if (seed.attachmentMotion) {
        entry->motionAttachments[1].motion = seed.attachmentMotion;
        entry->motionAttachments[1].flags = (entry->motionAttachments[1].flags & 0xffc1U) | ((seed.attachmentFlags | 0x20U) & 0x3eU);
    }
    entry->forcedY = static_cast<float>(seed.controlFlagsAndForcedY >> 16) / 100.0f;
    if (seed.routeDirectionOrEyeMode >= 0)
        entry->routeDirectionMode = seed.routeDirectionOrEyeMode;
    else
        entry->eyeControlMode = static_cast<std::int8_t>(-seed.routeDirectionOrEyeMode);
    entry->scheduleProgress = seed.scheduleProgress;
    entry->liveState = seed.liveState;
    SetNpcYawBits(*entry, static_cast<std::int16_t>(seed.yaw));
    switch (seed.kind) {
    case NpcScheduleSeedKind::DelayWait:
        entry->currentMotionId = static_cast<std::uint16_t>(seed.state.flags);
        entry->delayStartTime = static_cast<std::int32_t>(seed.state.routeFlags);
        std::memcpy(&entry->delayDuration, &seed.state.routeMode, sizeof(entry->delayDuration));
        break;
    case NpcScheduleSeedKind::ScriptState:
        if (!entry->stateCallback)
            entry->stateCallback = NpcRouteScheduleCallback;
        ApplyNpcScriptStateSeedPayload(*entry, seed.state);
        break;
    case NpcScheduleSeedKind::AreaPointState:
        entry->stateCallback = NpcAreaCompletionCallback;
        RestoreNpcAreaPointSeedPosition(*entry);
        entry->flags |= 0x20000U;
        BeginNpcAreaPointState(*entry, currentTime);
        break;
    default:
        break;
    }
    return entry->name;
}

void ApplyPendingNpcScheduleSeeds(std::int32_t currentTime) {
    if (!hasPendingNpcScheduleSeeds)
        return;
    for (std::size_t index = 0; index < npcScheduleSeedNames.size(); ++index) {
        NpcScheduleSeed seed;
        std::memcpy(&seed, pendingNpcScheduleSeeds + index * sizeof(seed), sizeof(seed));
        npcScheduleSeedNames[index] = ApplyNpcScheduleSeed(seed, currentTime);
        if (index == 1 && npcScheduleSeedNames[index].name) {
            if (NpcRouteLiveEntry *const entry = FindActiveNpcEntry(npcScheduleSeedNames[index]))
                entry->streamDistanceBias = 50.0f;
        }
    }
    hasPendingNpcScheduleSeeds = false;
}

bool NpcRouteConditionsChanged(NpcRouteLiveEntry &entry) {
    const std::byte *link = entry.recordBase + 0x34;
    if (!ReadU32(link))
        return false;
    const std::byte *command = link + 4;
    std::int32_t selectedBranch = -1;
    std::uint32_t jumps = 0;
    for (;;) {
        const std::int32_t opcode = static_cast<std::int32_t>(ReadU32(command));
        if (opcode < 0 || opcode >= 59 || opcode == 6)
            return selectedBranch != -1 || entry.activeBranchIndex != -1;
        std::int32_t branch = -1;
        switch (opcode) {
        case 11:
        case 30:
        case 43:
            if (ReadU32(command + 16) != 2 &&
                EvaluateNpcCondition(entry, ReadU32(command + 4) | (ReadU32(command + 16) << 16), ReadU32(command + 8),
                    120 * (npcCurrentDateTime.minute + 60 * npcCurrentDateTime.hour)) == NpcConditionResult::True)
                branch = static_cast<std::int32_t>(ReadU32(command + 12));
            break;
        case 12:
            if (static_cast<std::uint16_t>(npcLoadedArea.name) == ReadU16(command + 4))
                branch = static_cast<std::int32_t>(ReadU32(command + 8));
            break;
        case 13:
            if (npcCurrentDateTime.week != 0 && npcCurrentDateTime.week != 6)
                branch = static_cast<std::int32_t>(ReadU32(command + 4));
            break;
        case 14:
            if (npcCurrentDateTime.week == 0 || npcCurrentDateTime.week == 6)
                branch = static_cast<std::int32_t>(ReadU32(command + 4));
            break;
        case 15:
            for (const NpcRouteLiveEntry &other : npcRouteEntries) {
                if (other.name.name == ReadU32(command + 4)) {
                    if (static_cast<std::uint8_t>(other.recordBase[6]) == npcCurrentDateTime.month &&
                        static_cast<std::uint8_t>(other.recordBase[7]) == npcCurrentDateTime.day)
                        branch = static_cast<std::int32_t>(ReadU32(command + 8));
                    break;
                }
            }
            break;
        case 39:
            if (ReadU32(command + 4) == npcLoadedArea.name)
                branch = static_cast<std::int32_t>(ReadU32(command + 8));
            break;
        case 48: {
            const std::uint32_t selector = ReadU32(command + 4);
            if ((selector == 0 && !(npcEndOfDayWeatherForecast >= 0.15f)) || (selector == 1 && !(npcEndOfDayWeatherForecast < 0.15f)))
                branch = static_cast<std::int32_t>(ReadU32(command + 8));
            break;
        }
        case 57:
            if (static_cast<std::uint8_t>(ReadU32(command + 4)) == npcCurrentDateTime.month &&
                static_cast<std::uint8_t>(ReadU32(command + 4) >> 8) == npcCurrentDateTime.day)
                branch = static_cast<std::int32_t>(ReadU32(command + 8));
            break;
        default:
            break;
        }
        if (branch == -1) {
            command += 4 * npcOpcodeSizeDwords[opcode];
            continue;
        }
        if (branch == entry.activeBranchIndex)
            return false;
        if (++jumps > 256)
            return true;
        link = entry.recordBase + 0x34;
        for (std::int32_t index = 0; index < branch; ++index) {
            const std::int32_t offset = static_cast<std::int32_t>(ReadU32(link));
            if (!offset)
                return false;
            link += offset;
        }
        command = link + 4;
        selectedBranch = branch;
    }
}

void RebaseNpcRouteContextAfterReload(NpcRouteStateContext &context, std::uint8_t slotFlags, float distance) {
    if (!(context.flags & 1U))
        return;
    if (context.routeFlags & 0x8000U) {
        const shen_name owner = context.scriptOwner;
        ReleaseNpcRouteControl(&context, {}, 0);
        for (std::size_t index = npcAuthoredEntryCount; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != owner.name)
                continue;
            entry.name = SHEN_NAME(MOVE);
            entry.node = nullptr;
            entry.motionWork = nullptr;
            break;
        }
        return;
    }
    context.scenePosition = NpcRouteAreaToScene(context.localPosition);
    if (context.phase == 8 && !context.otherName.name && ReadU32(GetNpcAreaPoint(context.pendingAreaPoint) + 0x30))
        context.moveState = 0;
    if (context.phase != 7 || !(slotFlags & 2U))
        return;
    const std::int32_t steps = static_cast<std::int32_t>(distance / context.motionStep);
    const bool restoreAvoidance = (context.flags & 0x40000U) != 0;
    if (restoreAvoidance) {
        context.flags &= ~0x140000U;
        if (!(context.routeFlags & 0x2000000U)) {
            if (!(context.flags & 0x100000U)) {
                context.flags &= ~0x1000U;
                context.routeFlags &= ~0x800U;
            }
            RequestNpcRouteMotion(context, true);
        }
    }
    for (std::int32_t step = 0; step < steps; ++step)
        if (TickNpcRouteStateContext(&context))
            break;
    if (restoreAvoidance && (context.flags & 1U) && (context.routeFlags & 0x100000U)) {
        context.routeFlags |= 0x800U;
        context.flags |= 0x40000U;
    }
}

void RebuildNpcRuntimeAfterAreaReload() {
    if (!npcFrameCount)
        return;
    npcAuthoredEntryCount = npcSchedule.records.size();
    npcRouteEntries = npcEntryStorage.first(npcAuthoredEntryCount + npcScriptEntryCount);
    npcReloadPassStarted = false;
    npcIsTimeAdvancing = 0;
    npcColdResetMode = 1;
    npcWaitFrames = 0;
    npcWaitSawLoadFailure = false;
    npcPlayerPresent = false;
    npcActiveStreamCost = 0;
    npcCrowdPhase = 0;
    npcFrameCount = 0;
    npcStreamThrottle = 0.0f;
    npcTimedMotionCount = 0;
    for (NpcTimedMotionSlot &slot : npcTimedMotionSlots)
        slot.occupied = 0;
    npcCrowdHead = npcCrowdBookmark = npcRouteEntries.data();
    float distance = 0.0f;
    const std::byte *group = npcSchedule.entryPointScalarTableOffset ? npcSchedule.bytes.data() + npcSchedule.entryPointScalarTableOffset : nullptr;
    bool foundScalar = false;
    while (group && ReadU32(group)) {
        const std::int32_t count = static_cast<std::int32_t>(ReadU32(group + 4));
        if (ReadU32(group) == npcLoadedArea.name) {
            for (std::int32_t index = 0; index < count; ++index) {
                const std::byte *scalar = group + 8 + index * 8;
                if (static_cast<std::int32_t>(ReadU32(scalar)) == g_entryPoint) {
                    distance = ReadF32(scalar + 4);
                    foundScalar = true;
                    break;
                }
            }
            if (foundScalar)
                break;
        }
        group += 8 * (count + 1);
    }
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        const bool changed = NpcRouteConditionsChanged(entry);
        const std::uint8_t slotFlags = npcSlotFlags[index];
        npcSlotFlags[index] = (entry.flags >> 23) & 8U;
        if (index < npcAuthoredEntryCount && changed && !(entry.stateContext.flags & 0x300U)) {
            if (entry.stateContext.otherName.name)
                ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.stateContext.scriptOwner);
            entry.node = nullptr;
            ResetNpcAreaRouteState(entry);
            ClearNpcDeferredBranches(entry);
            const auto name = entry.name;
            const auto record = entry.recordBase;
            const auto human = entry.humanPackageIndex;
            entry = {};
            entry.name = name;
            entry.recordBase = record;
            entry.humanPackageIndex = human;
            InitializeNpcInterpreterState(entry, static_cast<std::int32_t>(index));
            entry.poolIndex = static_cast<std::int16_t>(index);
            entry.stateContext.owner = &entry;
            entry.stateContext.flags = 0;
            entry.stateContext.stateTable = reinterpret_cast<const NpcRouteNode *>(npcSchedule.bytes.data() + npcSchedule.routeNodeTableOffset);
        } else {
            entry.flags &= 0x5dffffe8U;
            entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
            entry.node = nullptr;
            entry.motionWork = nullptr;
            entry.stateContext.motionWork = nullptr;
            entry.timedMotionHandle = nullptr;
            entry.streamRouteChangeKind = 0;
            entry.loadHumanResult = 8;
            if (entry.flags & 0xa000U)
                entry.localPosition.y = entry.forcedY;
            else
                FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
            entry.worldPosition = NpcAreaPosition(entry.localPosition);
            RebaseNpcRouteContextAfterReload(entry.stateContext, slotFlags, distance);
            if (entry.liveState == 8) {
                if (GetNpcAreaPointRouteState(entry.currentAreaPoint)->flags & 8U)
                    npcSlotFlags[index] |= 4U;
            } else {
                if (!(entry.flags & 0x800U))
                    ResolveNpcAreaPointOwnerAnchor(entry, entry.currentAreaPoint);
                npcSlotFlags[index] &= ~4U;
            }
        }
        entry.previous = index ? &npcRouteEntries[index - 1] : nullptr;
        entry.next = index + 1 < npcRouteEntries.size() ? &npcRouteEntries[index + 1] : nullptr;
        YieldNpcLoadingPresentation();
    }
}

void WarmBootstrapNpcRoutes() {
    npcPositionQueryMode = 0;
    npcWaitFrames = 0;
    npcActiveStreamCost = 0;
    npcFrameCount = 0;
    npcCrowdPhase = 0;
    npcTimedMotionCount = 0;
    for (NpcTimedMotionSlot &slot : npcTimedMotionSlots)
        slot.occupied = 0;
    npcStreamThrottle = 0.0f;
    npcCrowdHead = npcCrowdBookmark = npcRouteEntries.data();
    npcPresentedTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    UpdateNpcStreamThrottle(true);
    for (NpcGlobalBranchTrigger &trigger : npcGlobalBranchTriggers)
        trigger.packedCondition = 0xffffffffU;
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        const std::uint32_t retained = entry.flags & (NpcRouteAbstractActive | NpcRouteStreamPinned);
        if (retained & NpcRouteAbstractActive)
            npcSetLock(entry.name, 1);
        entry.node = nullptr;
        ResetNpcAreaRouteState(entry);
        InitializeNpcInterpreterState(entry, static_cast<std::int32_t>(index));
        npcSlotFlags[index] = (entry.flags >> 23) & 8U;
        YieldNpcLoadingPresentation();
        NpcRouteRuntimeRecord &runtime = npcRouteRuntimeRecords[index];
        runtime.character = nullptr;
        runtime.packageAlias = {};
        runtime.streamState = NpcHumanStreamState::Inactive;
        if (retained & NpcRouteAbstractActive) {
            WarmStartNpcRoute(entry.name);
            entry.node = FindCharacterByScenarioName(entry.name);
            if (entry.node) {
                npcActiveStreamCost += entry.streamCost;
                entry.loadHumanResult = 1;
                InitializeStreamedNpcRoute(entry, true);
                runtime.character = entry.node;
                runtime.streamState = NpcHumanStreamState::Live;
            }
        }
        entry.flags |= retained;
    }
    if (!npcRouteEntries.empty()) {
        npcRouteEntries.front().previous = nullptr;
        npcRouteEntries.back().next = nullptr;
    }
    BuildRouteTimeWindow(53880, 54000, 1);
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        npcRouteEntries[index].flags &= ~0x10000000U;
        std::byte *const flags = const_cast<std::byte *>(npcRouteEntries[index].recordBase) + 0x0b;
        *flags &= std::byte{0xfe};
    }
}

void ResolveNpcScenarioNodesAndWarmStart() {
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        entry.node = FindScenarioCharacter(control ? control->scenarioTask : nullptr, entry.name);
        if (!entry.node)
            continue;
        EV_CheckWarmStartNpcRoute(entry.name);
        npcActiveStreamCost += entry.streamCost;
        entry.loadHumanResult = 1;
        entry.flags = (entry.flags & ~3U) | NpcRouteStreamLive;
        entry.motionWork = FindMotionWork(entry.node);
        npcSlotFlags[entry.poolIndex] |= 1U;
        entry.stateContext.motionWork = entry.motionWork;
        entry.streamRadius = 0.30000001f;
        entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
        if (entry.eyeControlProfile)
            HandleNpcEyeControl(entry);
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node)) {
            if (!(entry.flags & 0x200U)) {
                if ((npcSchedule.floor != -1000 && (entry.packedMoveFlags & 0xff00U)) || (entry.flags & 0x2000U))
                    control->flags &= ~CharacterControlApplyGravity;
                if (entry.flags & 0x8000000U)
                    control->flags = (control->flags & ~0x804U) | 0x800U;
            }
        }
        if (entry.flags & 0x800U)
            SetCharacterType(entry.node, 3);
    }
}

bool LoadCurrentNpcSchedule(bool loadResources, const char *requestedFilename) {
    std::vector<std::byte> profileBytes;
    std::string filename;
    std::uint32_t clipListOffset{};
    std::uint32_t motionListOffset{};
    std::uint8_t motionMode{};
    if (!ReadArchiveFile("misc/npcarea.bin", profileBytes) ||
        !SelectNpcScheduleFilename(profileBytes, g_currentMap, g_actNumber, filename, clipListOffset, motionListOffset, motionMode, requestedFilename)) {
        return false;
    }
    if (filename.empty()) {
        static constexpr const char *defaults[]{"NPC.BIN", "NPC.BIN", "NPC.BIN", "NPC_Q100.BIN", "NPC_KRN1.BIN"};
        if (g_actNumber < 0 || g_actNumber >= 5)
            return false;
        filename = defaults[g_actNumber];
    }

    std::array<char, 96> schedulePath{};
    const int length = std::snprintf(schedulePath.data(), schedulePath.size(), "scene/%02d/npc/%s", g_actNumber, filename.c_str());
    if (length <= 0 || static_cast<std::size_t>(length) >= schedulePath.size()) {
        return false;
    }
    if (npcBinaryStorage.empty() || !npcHumanSlots)
        return false;
    const std::span<const shen_name> humans{reinterpret_cast<const shen_name *>(npcHumanIndex + 4), static_cast<std::size_t>(npcHumanCount)};
    const bool loadBanks = loadResources && !npcMotionBanksLoaded;
    if (loadBanks) {
        if (!motionMode)
            LoadNpcMotionBank("misc", "NPC.MOT");
        if (motionMode != 2)
            LoadNpcMotionBank("misc", "NPC_TBL.MOT");
        LoadNpcMotionProps("misc", "NPC.MPT", SHEN_NAME(NPCP));
    }
    if (loadResources && !LoadNpcMotionClips(profileBytes, clipListOffset)) {
        FreeNpcMotionClips();
        return false;
    }
    if (loadBanks) {
        if (!LoadNpcProfileMotionBanks(profileBytes, motionListOffset)) {
            FreeNpcMotionBanks();
            return false;
        }
        npcMotionBanksLoaded = true;
    }
    if (filename == npcScheduleFilename.data() && !npcSchedule.bytes.empty() && !npcRouteEntries.empty()) {
        RebuildNpcRouteCollision(true);
        RebuildNpcRuntimeAfterAreaReload();
        npcLoadedArea = g_currentMap;
        ResetNpcRoutes(reinterpret_cast<NpcRouteResetGroupHeader *>(npcSchedule.bytes.data() + npcSchedule.routeResetTableOffset));
        if (!loadResources)
            ResolveNpcScenarioNodesAndWarmStart();
        return true;
    }
    if (loadResources) {
        FreeNpcRouteLiveEntries();
    } else {
        // npcGetRoutePoint reinitializes NPCC, not SCNT/PAK. GetFreeTalkFlag
        // has released unpinned streams; pinned/abstract scenario owners
        // survive the filename change and are rebound by the warm-start pass.
        UnpublishNpcRouteLiveEntries();
        npcRouteEntries = {};
        npcRouteRuntimeRecords.clear();
        ResetNpcAreaRouteResources();
        npcSchedule = {};
        npcTimedMotionCount = 0;
        for (NpcTimedMotionSlot &slot : npcTimedMotionSlots)
            slot.occupied = 0;
    }
    const std::uint64_t size = ArchiveData_GetFileSize(schedulePath.data());
    if (!size || size > npcBinaryStorage.size())
        return false;
    const std::span<std::byte> scheduleBytes = npcBinaryStorage.first(static_cast<std::size_t>(size));
    NpcScheduleStorage parsed;
    if (!ArchiveData_Read(schedulePath.data(), 0, scheduleBytes.data(), scheduleBytes.size()) || !ParseNpcSchedule(scheduleBytes, humans, parsed)) {
        return false;
    }

    npcSchedule = std::move(parsed);
    std::snprintf(npcScheduleFilename.data(), npcScheduleFilename.size(), "%s", filename.c_str());
    if (!BindNpcAreaRouteResources({reinterpret_cast<NpcAreaPoint *>(npcSchedule.bytes.data() + npcSchedule.areaPointTableOffset), npcSchedule.areaPointCount},
            npcSchedule.areaOrigin))
        return false;
    RebuildNpcRouteCollision(false);
    std::vector<NpcRouteRecordSource> sources;
    sources.reserve(npcSchedule.records.size());
    for (const ParsedNpcRecord &record : npcSchedule.records) {
        sources.push_back({record.name, npcSchedule.bytes.data() + record.offset, record.humanPackageIndex});
    }
    // npcResetSpecialSceneState, called by the cold route bootstrap:
    // only these exact schedule filenames permit MoveForNpcInData checks.
    npcSceneCheckEnabled = filename == "NPC.BIN" || filename == "NPC_Q100.BIN";
    npcCachedAreaRouteCharacter = {};
    npcCachedAreaRouteId = 0;
    npcScriptRouteOwner = nullptr;
    npcFrameCount = 0;
    UpdateNpcStreamThrottle(true);
    if (CreateNpcRouteLiveEntries(sources).empty())
        return false;
    npcLoadedArea = g_currentMap;
    ResetNpcRoutes(reinterpret_cast<NpcRouteResetGroupHeader *>(npcSchedule.bytes.data() + npcSchedule.routeResetTableOffset));
    const DateTime &now = npcCurrentDateTime;
    ApplyPendingNpcScheduleSeeds(120 * (now.minute + 60 * now.hour));
    std::int32_t hour = now.hour;
    if (hour < 7 && now.minute < 30)
        hour += 24;
    BuildRouteTimeWindow(54000, 120 * (now.minute + 60 * hour), 1);
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        npcRouteEntries[index].flags &= ~0x10000000U;
        auto *const flags = const_cast<std::byte *>(npcRouteEntries[index].recordBase) + 0x0b;
        *flags &= std::byte{0xfe};
    }
    if (!loadResources)
        ResolveNpcScenarioNodesAndWarmStart();
    return true;
}

void CaptureNpcScriptStateSeedPayload(const NpcRouteLiveEntry &entry, NpcSavedRouteState &saved) {
    const NpcRouteStateContext &context = entry.stateContext;
    saved.flags = context.flags;
    saved.routeFlags = context.routeFlags;
    saved.routeMode = entry.field338;
    if (context.flags & 4U) {
        saved.currentAreaPoint =
            static_cast<std::uint16_t>(context.crossingPoint == 2 && (context.parameter & 0x3fU) > 32 ? context.nextNode : context.activeNode);
    } else {
        saved.currentAreaPoint = context.currentAreaPoint;
    }
    saved.phase = static_cast<std::int8_t>(context.phase);
    saved.pendingAreaPoint = context.pendingAreaPoint;
    saved.localX = context.localPosition.x;
    saved.localZ = context.localPosition.z;
    saved.arrivalYaw = context.arrivalYaw;
    saved.parameter = context.parameter;
    saved.scheduleProgress = entry.field32f;
    saved.liveState = entry.field331;
}
} // namespace npc_package_detail
void EV_LoadNPCRoute(const char *filename) {
    if (!npcRegistryTask.pending)
        return;
    EV_ResetFreeTalkFlags();
    npcSchedule.floor = static_cast<std::int32_t>(GetQronNextStream());
    LoadCurrentNpcSchedule(false, filename);
}

void ReleaseNpcScriptRouteOwner() {
    if (!npcRegistryTask.current || !npcScriptRouteOwner)
        return;
    NpcRouteLiveEntry &entry = *npcScriptRouteOwner;
    UpdateNpcScheduleSeedSlot({}, entry.name, 0);
    NpcRouteStateContext &context = entry.stateContext;
    const std::uint32_t flags = context.flags;
    if (flags & 0x100U) {
        InitializeNpcRouteRequest(context, context.pendingAreaPoint, entry.pendingAreaPoint);
        SetNpcLocomotionScale(context, entry.motionSpeed);
        context.flags |= 0x200U;
        if (flags & 0x4000U) {
            context.flags |= 0x4000U;
            context.routeFlags |= 0x20000U;
        }
        if (entry.field338 & 0x80U)
            entry.eyeControlMode = 12;
        else
            entry.routeDirectionMode = 12;
        context.moveFlags = (entry.packedMoveFlags & 0x100ffU) | (static_cast<std::uint32_t>(static_cast<std::int32_t>(context.floor)) << 8);
    } else if (flags & 0x8000U) {
        context.waitStartTime = 0;
    }
    entry.flags &= ~0x200U;
    npcScriptRouteOwner = nullptr;
}

void ResetNpcScheduleSeeds() {
    npcScheduleSeedNames = {};
    hasPendingNpcScheduleSeeds = false;
}

void SetPendingNpcScheduleSeedBuffer(std::span<const std::byte, NpcScheduleSeedBufferSize> source) {
    pendingNpcScheduleSeeds = source.data();
    hasPendingNpcScheduleSeeds = true;
}

void CaptureNpcScheduleSeeds(std::span<std::byte, NpcScheduleSeedBufferSize> destination) {
    std::array<shen_name, 2> names = npcScheduleSeedNames;
    if (names[0].name == names[1].name)
        names[1] = {};
    for (std::size_t index = 0; index < names.size(); ++index) {
        NpcScheduleSeed seed{};
        const NpcRouteLiveEntry *const entry = names[index].name ? FindActiveNpcEntry(names[index]) : nullptr;
        if (entry) {
            seed.name = entry->name;
            seed.routeDirectionOrEyeMode = entry->eyeControlMode ? static_cast<std::int8_t>(-entry->eyeControlMode) : entry->routeDirectionMode;
            seed.packedMoveFlagsHigh = static_cast<std::uint8_t>(entry->packedMoveFlags >> 8);
            seed.packedMoveFlagsLow = static_cast<std::uint8_t>(entry->packedMoveFlags);
            if (entry->packedMoveFlags & 0x10000U)
                seed.packedMoveFlagsLow |= 0x100U;
            seed.scheduleProgress = entry->scheduleProgress;
            seed.liveState = entry->liveState;
            seed.currentOpcode = static_cast<std::uint8_t>(*entry->currentCommand);
            seed.currentCommandOffset = static_cast<std::uint16_t>(entry->currentCommand - entry->recordBase - 0x38);
            seed.activeBranchIndex = static_cast<std::uint8_t>(entry->activeBranchIndex);
            if (entry->resumeCommand)
                seed.resumeCommandOffset = static_cast<std::uint16_t>(entry->resumeCommand - entry->recordBase - 0x38);
            seed.flags = entry->flags & 0x5dffffe8U;
            seed.currentAreaPoint = entry->currentAreaPoint;
            seed.pendingAreaPoint = entry->pendingAreaPoint;
            seed.yaw = static_cast<std::uint16_t>(ReadNpcEulerYaw(entry->euler));
            std::size_t localCount = 0;
            for (std::size_t triggerIndex = 0; triggerIndex < 4; ++triggerIndex) {
                const NpcLocalBranchTrigger trigger =
                    ReadNpcLiveField<NpcLocalBranchTrigger>(*entry, npcLocalBranchesOffset + triggerIndex * sizeof(NpcLocalBranchTrigger));
                if (trigger.packedCondition != 0xffffffffU)
                    seed.localTriggerOffsets[localCount++] = trigger.commandOffset;
            }
            if (localCount != 4)
                seed.localTriggerOffsets[localCount] = 0xffff;
            std::size_t globalCount = 0;
            for (const auto &trigger : npcGlobalBranchTriggers) {
                if (trigger.packedCondition != 0xffffffffU && trigger.owner.name == entry->name.name) {
                    seed.globalTriggerOffsets[globalCount++] = trigger.commandOffset;
                }
            }
            if (globalCount != 16)
                seed.globalTriggerOffsets[globalCount] = 0xffff;
            seed.controlFlagsAndForcedY =
                (entry->controlFlags & 0xffffU) | (static_cast<std::uint32_t>(static_cast<std::int32_t>(entry->forcedY * 100.0f)) << 16);
            seed.controlBit4 = (entry->controlFlags & 0x10U) != 0;
            seed.attachmentFlags = entry->motionAttachments[1].flags & 0x1eU;
            seed.attachmentMotion = entry->motionAttachments[1].motion;
            if (entry->liveState == 1) {
                seed.kind = NpcScheduleSeedKind::ScriptState;
                CaptureNpcScriptStateSeedPayload(*entry, seed.state);
            } else if (entry->liveState == 8) {
                seed.kind = NpcScheduleSeedKind::AreaPointState;
            } else {
                seed.kind = NpcScheduleSeedKind::DelayWait;
                seed.state.flags = entry->currentMotionId;
                seed.state.routeFlags = static_cast<std::uint32_t>(entry->delayStartTime);
                std::memcpy(&seed.state.routeMode, &entry->delayDuration, sizeof(entry->delayDuration));
            }
        }
        npcScheduleSeedNames[index] = seed.name;
        std::memcpy(destination.data() + index * sizeof(seed), &seed, sizeof(seed));
    }
}
} // namespace shenmue::s2
