#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/npc_data_dc.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/date_time.hpp"
#include "s2/math.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cstring>
#include <cmath>
#else
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
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

using namespace npc_package_detail;

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace npc_package_detail {

void ClearNpcDeferredBranches(NpcRouteLiveEntry &entry);
void InitializeNpcInterpreterState(NpcRouteLiveEntry &entry, std::int32_t slot);
bool AdvanceNpcTimedSequenceSlot(NpcRouteLiveEntry &entry, std::int32_t currentTime);
std::int64_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry);
bool BeginNpcAreaPointState(NpcRouteLiveEntry &entry, std::int32_t currentTime);
void ResolveNpcAreaPointOwnerAnchor(NpcRouteLiveEntry &entry, std::uint16_t point);
bool PrimeNpcInterpreter(NpcRouteLiveEntry &entry);
NpcConditionResult EvaluateNpcCondition(NpcRouteLiveEntry &entry, std::uint32_t packedCondition, std::uint32_t argument, std::int32_t currentTime);
void UpdateNpcScheduleSeedSlot(shen_name character, shen_name previous, std::size_t slot);
std::int32_t ExecuteNpcCommand(NpcRouteLiveEntry &entry, std::int32_t currentTime);
void LoadNpcMotionBank(const char *directory, const char *filename);
void LoadNpcMotionProps(const char *directory, const char *filename, shen_name allocationName = SHEN_NAME(NPCp));
bool LoadNpcProfileMotionBanks(std::span<const std::byte> profile, std::uint32_t listOffset);
void FreeNpcMotionBanks();
void FreeNpcMotionClips();
bool LoadNpcMotionClips(std::span<const std::byte> profile, std::uint32_t listOffset);
bool SelectNpcScheduleFilename(std::span<const std::byte> bytes, shen_name area, std::int32_t act, std::string &filename, std::uint32_t &clipListOffset,
    std::uint32_t &motionListOffset, std::uint8_t &motionMode, const char *requestedFilename = nullptr);
bool ParseNpcSchedule(std::span<std::byte> bytes, std::span<const shen_name> humans, NpcScheduleStorage &parsed);
bool MoveForNpcInData(NpcRouteLiveEntry &entry, std::uint32_t area, bool apply);
bool NpcRouteConditionsChanged(NpcRouteLiveEntry &entry);

} // namespace npc_package_detail

bool RequestNpcAreaTransfer(shen_name character, std::uint32_t area);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void PlaceNpcOutsideDoor(NpcRouteLiveEntry *entry);

void ResetNpcEntryToAreaPoint(NpcRouteLiveEntry *entry);

void BeginNpcAreaRoute(NpcRouteLiveEntry *entry, std::int32_t currentTime);

const Vector3 *GetNpcAreaPointPosition(std::int32_t point);

std::uint32_t GetNpcAreaPointMoveFlags(std::int32_t point);

extern std::int32_t npcPositionQueryMode;

NpcRouteSpline *GetNpcScriptRouteTable();

struct NpcGlobalBranchTrigger {
    shen_name owner;
    std::uint32_t packedCondition, target, parameter;
};

NpcGlobalBranchTrigger npcGlobalBranchTriggers[16];

DateTime npcCurrentDateTime = {};

std::uint32_t npcCurrentSeconds, npcPresentedTime, npcSimDeltaTime = 0;

std::uint32_t npcSpecialMotionEpoch, npcRouteBuildCount = 0;

std::int32_t npcCrowdPhase = 0, npcReloadPassStarted = 0, npcScheduleSimulationActive = 0;

std::int32_t npcSimulationSinglePass, npcSimulationTime, npcSimulationStop = 0;

float npcStreamThrottle, npcStreamThrottleWithBias;

float npcEndOfDayWeatherForecast = 0.0f, npcHalfHourWeatherForecast = 0.0f;

Vector3 npcPlayerPosition = {};

void ActivateNpcAreaRoutes(std::int32_t active);

std::int32_t ClearNpcAreaRouteForEntry(NpcRouteLiveEntry *entry);

void RestorePendingNpcScheduleSeeds(std::int32_t currentTime);

void InterpretNpcScheduleEntry(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t NpcRouteConditionsChanged(NpcRouteLiveEntry *entry);

void RebuildNpcRouteContextAfterReload(NpcRouteStateContextPrefix *context, std::int32_t slotFlags, float offset);

void PlaceNpcOutsideDoor(NpcRouteLiveEntry *entry);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool EvaluateNpcBranchCondition(NpcRouteLiveEntry *entry, std::uint32_t condition, std::uint32_t expected, std::int32_t currentTime) {
    bool result = false;
    switch (condition >> 16U) {
    case 0:
        if (static_cast<std::uint32_t>(GetShenmueFlag(condition & 0xffffU)) == expected)
            result = true;
        break;
    case 1:
        if (static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode_Internal, static_cast<std::int16_t>(condition & 0xffffU))) == expected)
            result = true;
        break;
    case 2: {
        const float x = npcPlayerPosition.x - entry->localPosition.x, z = npcPlayerPosition.z - entry->localPosition.z;
        float distance;
        std::memcpy(&distance, &expected, sizeof(distance));
        if (x * x + z * z > distance)
            result = true;
        else if (npcCurrentSeconds > npcSpecialMotionEpoch + 3600U) {
            if (npcCurrentSeconds <= npcSpecialMotionEpoch + 28800U)
                result = true;
            else {
                DateTime time;
                std::memcpy(&time, &npcCurrentDateTime, sizeof(time));
                time.second = 30;
                npcSpecialMotionEpoch = ComputeSecondsSince1980(time);
                SetScenarioByteStat(0x30, npcSpecialMotionEpoch >> 24U);
                SetScenarioByteStat(0x31, (npcSpecialMotionEpoch >> 16U) & 0xffU);
                SetScenarioByteStat(0x20, (npcSpecialMotionEpoch >> 8U) & 0xffU);
                SetScenarioByteStat(0x21, npcSpecialMotionEpoch & 0xffU);
            }
        }
        break;
    }
    case 3:
        if (currentTime <= static_cast<std::int32_t>(expected))
            result = true;
        break;
    case 4:
        switch (expected) {
        case 0:
            if (npcHalfHourWeatherForecast < 0.15f)
                result = true;
            break;
        case 1:
            if (!(npcHalfHourWeatherForecast < 0.15f))
                result = true;
            break;
        }
        break;
    case 5:
        if (static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode_Stat, static_cast<std::int16_t>(condition & 0xffffU))) == expected)
            result = true;
        break;
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static const std::int8_t npcOpcodeSizeDwords[56] = {1, 2, 2, 2, 3, 2, 2, 5, 6, 4, 3, 5, 3, 2, 2, 3, 7, 2, 3, 2, 2, 2, 2, 2, 2, 6, 2, 3, 2, 4, 5, 3, 2, 2, 5, 5,
    2, 3, 4, 3, 9, 2, 4, 5, 3, 3, 2, 2, 3, 2, 2, 2, 2, 2, 2, 3};

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t NpcRouteConditionsChanged(NpcRouteLiveEntry *entry) {
    if (!*reinterpret_cast<const std::uint32_t *>(entry->scriptRecord + 0x34))
        return 0;
    const std::uint32_t *command = reinterpret_cast<const std::uint32_t *>(entry->scriptRecord + 0x38);
    std::uint32_t selected = 0xffffffffU;
    for (; *command < 56U; command += npcOpcodeSizeDwords[*command]) {
        switch (*command) {
        case 0x0b:
        case 0x1e:
        case 0x2b:
            if (EvaluateNpcBranchCondition(
                    entry, (command[4] << 16U) | command[1], command[2], (npcCurrentDateTime.hour * 3600 + npcCurrentDateTime.minute * 60) * 2))
                selected = command[3];
            break;
        case 0x27:
            if (command[1] == npcLoadedArea)
                selected = command[2];
            break;
        case 0x0c:
            if ((command[1] & 0xffffU) == (npcLoadedArea & 0xffffU))
                selected = command[2];
            break;
        case 0x0d:
            if (static_cast<std::int8_t>(npcCurrentDateTime.week) != 0 && static_cast<std::int8_t>(npcCurrentDateTime.week) != 6)
                selected = command[1];
            break;
        case 0x0e:
            if (static_cast<std::int8_t>(npcCurrentDateTime.week) == 0 || static_cast<std::int8_t>(npcCurrentDateTime.week) == 6)
                selected = command[1];
            break;
        case 0x0f: {
            const NpcRouteLiveEntry *const other = FindNpcRouteEntry(shen_name(command[1]));
            if (other && static_cast<std::int8_t>(other->scriptRecord[6]) == npcCurrentDateTime.month &&
                static_cast<std::int8_t>(other->scriptRecord[7]) == npcCurrentDateTime.day)
                selected = command[2];
            break;
        }
        case 0x30:
            switch (command[1]) {
            case 0:
                if (npcEndOfDayWeatherForecast < 0.15f)
                    selected = command[2];
                break;
            case 1:
                if (!(npcEndOfDayWeatherForecast < 0.15f))
                    selected = command[2];
            }
        }
        if (selected != 0xffffffffU && selected != static_cast<std::uint32_t>(*reinterpret_cast<const std::int8_t *>(&entry->retained31c)))
            return 1;
    }
    if (selected == 0xffffffffU && *reinterpret_cast<const std::int8_t *>(&entry->retained31c) != -1)
        return 1;
    return 0;
}

#else

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
        const std::uint32_t retained = entry.flags & (NRSF_AbstractActive | NRSF_Pinned);
        if (retained & NRSF_AbstractActive)
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
        if (retained & NRSF_AbstractActive) {
            WarmStartNpcRoute(entry.name);
            entry.node = ctrlGetScenarioChara(entry.name);
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
        entry.node = scenFindChara(control ? control->scenarioTask : nullptr, entry.name);
        if (!entry.node)
            continue;
        EV_CheckWarmStartNpcRoute(entry.name);
        npcActiveStreamCost += entry.streamCost;
        entry.loadHumanResult = 1;
        entry.flags = (entry.flags & ~3U) | NRSF_Live;
        entry.motionWork =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(entry.node);
        npcSlotFlags[entry.poolIndex] |= 1U;
        entry.stateContext.motionWork = entry.motionWork;
        entry.streamRadius = 0.30000001f;
        entry.localPosition = NpcRouteAreaToScene(entry.worldPosition);
        if (entry.eyeControlProfile)
            HandleNpcEyeControl(entry);
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node)) {
            if (!(entry.flags & 0x200U)) {
                if ((npcSchedule.floor != -1000 && (entry.packedMoveFlags & 0xff00U)) || (entry.flags & 0x2000U))
                    control->flags &= ~CCMF_ApplyGravity;
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
    const auto seedByte = [](std::int16_t index) { return static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, index)); };
    npcCondition2StartSeconds = (seedByte(48) << 24) + (seedByte(49) << 16) + (seedByte(32) << 8) + seedByte(33);
    npcCondition2OutsideFrames = 0;
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void EV_LoadNPCRoute(const char *filename) {
    if (!npcRegistryTask.pending)
        return;
    EV_ResetFreeTalkFlags();
    npcSchedule.floor = static_cast<std::int32_t>(GetQronNextStream());
    LoadCurrentNpcSchedule(false, filename);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ResetNpcScheduleSeeds() {
    npcScheduleSeedNames = {};
    hasPendingNpcScheduleSeeds = false;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetPendingNpcScheduleSeedBuffer(std::span<const std::byte, NpcScheduleSeedBufferSize> source) {
    pendingNpcScheduleSeeds = source.data();
    hasPendingNpcScheduleSeeds = true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

const std::byte *FindNextNpcTimedCommand(const std::byte *cursor) {
    const std::uint32_t *command = reinterpret_cast<const std::uint32_t *>(cursor);
    const std::byte *result = 0;
    for (; *command < 56U; command += npcOpcodeSizeDwords[*command]) {
        if (*command == 1) {
            result = reinterpret_cast<const std::byte *>(command);
            break;
        }
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

inline std::int32_t NpcTimedCommandTime(const std::byte *command) {
    const std::uint32_t packed = *reinterpret_cast<const std::uint32_t *>(command + 4);
    const std::int32_t hour = (packed & 0xff00U) >> 8;
    return (hour * 3600 + (packed & 0xffU) * 60) * 2;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool SelectNpcScheduleBranch(NpcRouteLiveEntry *entry, std::int32_t index) {
    if (!*reinterpret_cast<const std::int32_t *>(entry->scriptRecord + 0x34))
        return false;
    const std::int32_t *record = reinterpret_cast<const std::int32_t *>(entry->scriptRecord + 0x34);
    for (std::int32_t current = 0; current < index; ++current) {
        const std::int32_t offset = *record;
        if (!offset)
            return false;
        record = reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::byte *>(record) + offset);
    }
    entry->currentCommand = reinterpret_cast<const std::byte *>(++record);
    *reinterpret_cast<std::int8_t *>(&entry->retained31c) = static_cast<std::int8_t>(index);
    entry->loopCommand = 0;
    entry->nextTimedCommand = FindNextNpcTimedCommand(entry->currentCommand);
    if (entry->nextTimedCommand)
        entry->nextTimedCommandTime = NpcTimedCommandTime(entry->nextTimedCommand);
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool AdvanceNpcTimedCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime) {
    if (currentTime >= entry->nextTimedCommandTime) {
        entry->currentCommand = entry->nextTimedCommand + 8;
        entry->nextTimedCommand = FindNextNpcTimedCommand(entry->currentCommand);
        if (entry->nextTimedCommand)
            entry->nextTimedCommandTime = NpcTimedCommandTime(entry->nextTimedCommand);
        entry->loopCommand = 0;
        return true;
    }
    return false;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void BeginNpcScheduleWait(NpcRouteLiveEntry *entry, std::int32_t currentTime) {
    entry->waitStartTime = currentTime;
    const std::uint32_t *command = reinterpret_cast<const std::uint32_t *>(entry->currentCommand);
    entry->waitDuration = ((command[1] & 0xffU) * 60 + ((command[1] & 0xff00U) >> 8U)) * 120;
    if (npcScheduleSimulationActive == 1)
        entry->waitDuration = 0;
    else
        entry->scheduleProgress = 2;
    entry->currentCommand += 8;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t PollNpcStateCallback(NpcRouteLiveEntry *entry) {
    if (!entry->stateCallback)
        return 0;
    const std::int32_t result = entry->stateCallback(entry);
    if (result == -1)
        return 0;
    if (result == 0)
        entry->stateCallback = 0;
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void UpdateNpcRouteQueryState(NpcRouteLiveEntry *entry, std::int32_t moving) {
    if (!moving) {
        if (entry->currentAreaPoint == reinterpret_cast<const std::int32_t *>(entry->scriptRecord)[3])
            entry->routeQueryState = 0;
        else if (entry->currentAreaPoint == reinterpret_cast<const std::int32_t *>(entry->scriptRecord)[4])
            entry->routeQueryState = 1;
        else
            entry->routeQueryState = 3;
    } else {
        if (entry->pendingAreaPoint == reinterpret_cast<const std::int32_t *>(entry->scriptRecord)[3])
            entry->routeQueryState = 4;
        else if (entry->pendingAreaPoint == reinterpret_cast<const std::int32_t *>(entry->scriptRecord)[4])
            entry->routeQueryState = 2;
        else
            entry->routeQueryState = 2;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ProcessNpcDeferredBranches(NpcRouteLiveEntry *entry, std::int32_t currentTime) {
    std::int32_t selected = 0;
    NpcLocalBranchTrigger *local = entry->localBranches;
    std::int32_t remaining = entry->localBranchCount;
    std::int32_t index = 0;
    if (remaining) {
        do {
            if (local->packedCondition != 0xffffffffU) {
                --remaining;
                selected = EvaluateNpcBranchCondition(entry, local->packedCondition, local->parameter, currentTime);
                if (selected == 1) {
                    if (SelectNpcScheduleBranch(entry, local->target)) {
                        if (((local->packedCondition & 0xffff0000U) >> 16U) < 2U)
                            ClearNpcAreaRouteForEntry(entry);
                        local->packedCondition = 0xffffffffU;
                        --entry->localBranchCount;
                        entry->scheduleProgress = 1;
                        goto localDone;
                    }
                    selected = 0;
                }
            }
            ++index;
            ++local;
            if (!remaining)
                goto localDone;
        } while (index < 4);
    }
localDone:
    if (!selected) {
        NpcGlobalBranchTrigger *global = npcGlobalBranchTriggers;
        remaining = entry->globalBranchCount;
        index = 0;
        if (remaining) {
            do {
                if (global->packedCondition != 0xffffffffU && global->owner == entry->name) {
                    --remaining;
                    selected = EvaluateNpcBranchCondition(entry, global->packedCondition, global->target, currentTime);
                    if (selected == 1 && SelectNpcScheduleBranch(entry, static_cast<std::int8_t>(global->parameter))) {
                        if ((global->packedCondition >> 16U) < 2U)
                            ClearNpcAreaRouteForEntry(entry);
                        global->packedCondition = 0xffffffffU;
                        --entry->globalBranchCount;
                        entry->scheduleProgress = 1;
                        return;
                    }
                }
                ++index;
                ++global;
                if (!remaining)
                    return;
            } while (index < 16);
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ComputeNpcAreaPointYaw(const NpcRouteSpline *routes, std::int32_t point) {
    const NpcAreaPoint &area = npcAreaPoints[point];
    const NpcRouteSpline &route = routes[area.routeIndex];
    Vector3 delta = *GetNpcAreaPointPosition(point);
    Vector3 target;
    InterpolateNpcSplineNormalized(route.points, route.pointCount, *reinterpret_cast<const std::int16_t *>(area.routeParameters), &target);
    delta.x = target.x - delta.x;
    delta.z = target.z - delta.z;
    return std::fabs(delta.x) < 0.001f && std::fabs(delta.z) < 0.001f ? 0 : static_cast<std::int16_t>(InverseFixedAtan2(delta.x, delta.z));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void PrimeNpcInterpreter(NpcRouteLiveEntry *entry) {
    entry->localPosition = *GetNpcAreaPointPosition(entry->currentAreaPoint);
    entry->worldPosition = entry->localPosition;
    entry->localPosition.x -= npcRouteOrigin.x;
    entry->localPosition.z -= npcRouteOrigin.z;
    entry->previousPosition = entry->localPosition;
    entry->scheduleProgress = 1;
    entry->rotation.y = ComputeNpcAreaPointYaw(GetNpcScriptRouteTable(), entry->currentAreaPoint);
    PlaceNpcOutsideDoor(entry);
    entry->routeQueryState = 0;
    entry->nextTimedCommand = FindNextNpcTimedCommand(entry->currentCommand);
    if (entry->nextTimedCommand)
        entry->nextTimedCommandTime = NpcTimedCommandTime(entry->nextTimedCommand);
}

#else

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
        entry.flags |= NRSF_AbstractActive;
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
        mtwkSetSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
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
        mtwkSetRegionFlag(entry.motionWork, 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(entry.motionWork, 14, 0);
        mtwkSetRegionEvalTime(entry.motionWork, static_cast<float>(frame), MotionRegion::LowerBody);
    }
    auto &slot = *static_cast<NpcTimedMotionSlot *>(entry.timedMotionHandle);
    if (!slot.motion)
        return true;
    slot.character = ctrlGetScenarioChara(slot.name);
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
    // The native owner overwrites the transformed offset after publishing its matrix scratch.
    position = NpcRouteAreaToScene(entry.worldPosition);
    shenApplyTranslationToAll(&slot.character->entity.node, position);
    Vector3i rotation;
    std::memcpy(&rotation, &entry.euler, sizeof(rotation));
    charaSetWorldRot(slot.character, rotation);
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
        charaSetWorldRot(entry.node, rotation);
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
        return static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, static_cast<std::int16_t>(flag))) == argument ? NpcConditionResult::True :
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
                    flags |= CCMF_ApplyGravity;
                control->flags = flags | CCMF_EvaluateMotion;
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
            mtwkCopyTransition(entry.motionWork, nullptr, 0, 1, 0);
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            charaSetWorldRot(entry.node, rotation);
            mtwkSetRegionFlag(entry.motionWork, 71, 0, MotionRegion::LowerBody);
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
            RestoreNpcAreaPointSeedPosition(entry);
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
            (entry.currentMotionId != mtwkGetAnimationId(entry.motionWork, MotionRegion::LowerBody) ||
                !mtwkIsCurrentAnimOpenEnded(entry.motionWork, MotionRegion::LowerBody))) {
            mtwkSetRequestedRootMode(entry.motionWork, 1);
            mtwkSetFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags &= ~(CCMF_ApplyGravity | CCMF_EvaluateMotion);
            SetCharacterType(entry.node, 3);
            if (entry.flags & 0xa000U)
                entry.localPosition.y = entry.forcedY;
            else
                FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            charaSetWorldRot(entry.node, rotation);
            mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
            mtwkSetSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
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
            (entry.currentMotionId != mtwkGetAnimationId(entry.motionWork, MotionRegion::LowerBody) ||
                !mtwkIsCurrentAnimOpenEnded(entry.motionWork, MotionRegion::LowerBody))) {
            mtwkSetRequestedRootMode(entry.motionWork, 1);
            mtwkSetFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
            if (entry.flags & 0xa000U)
                entry.localPosition.y = entry.forcedY;
            else
                FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            charaSetWorldRot(entry.node, rotation);
            mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
            mtwkSetSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
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
                control->flags &= ~(CCMF_ApplyGravity | CCMF_EvaluateMotion);
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
            (entry.currentMotionId != mtwkGetAnimationId(entry.motionWork, MotionRegion::LowerBody) ||
                !mtwkIsCurrentAnimOpenEnded(entry.motionWork, MotionRegion::LowerBody))) {
            mtwkSetRequestedRootMode(entry.motionWork, 1);
            mtwkSetFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
            if (entry.flags & 0xa000U)
                entry.localPosition.y = entry.forcedY;
            else
                FindNpcIntersectionInScene(entry.localPosition, entry.packedMoveFlags);
            shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            charaSetWorldRot(entry.node, rotation);
            mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
            mtwkSetSpeed(entry.motionWork, entry.motionSpeed, MotionRegion::LowerBody);
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
        entry.flags |= NRSF_ForceHidden;
        entry.currentCommand = next;
        if (entry.node) {
            EV_SetCharDisplay(entry.node, CVM_All, 0);
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags &= ~CCMF_VoxelSweep;
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
        if ((entry.node && mtwkSetRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody)) || (!entry.node && opcode == 58) || npcIsTimeAdvancing == 1) {
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

void LoadNpcMotionProps(const char *directory, const char *filename, shen_name allocationName) {
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
            mtwkSetDrawOverrideData(entry.motionWork, nullptr);
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
    std::uint32_t &motionListOffset, std::uint8_t &motionMode, const char *requestedFilename) {
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

// Native npcInitDataInternal: NPC.BIN's buffer, InitNpcPool's route-entry pool and slot flags, and AllocateSTRL, all from
// the heap tail and kept for the act.
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t StopNpcScriptRoute(NpcRouteStateContextPrefix *context, shen_name name, std::int32_t yaw);

std::int32_t StopNpcAreaRoute(NpcRouteLiveEntry *entry);

void ReleaseNpcTimedMotion(NpcRouteLiveEntry *entry);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool BindNpcStateCallback(NpcRouteLiveEntry *entry, std::int32_t (*callback)(NpcRouteLiveEntry *)) {
    if (entry->stateCallback)
        return false;
    entry->stateCallback = callback;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t NpcRouteCompletionCallback(NpcRouteLiveEntry *entry) {
    std::int32_t result;
    if (static_cast<volatile NpcRouteStateContextPrefix *>(&entry->stateContext)->flags & 1U) {
        result = 0;
        NpcRouteStateContextPrefix *context = &entry->stateContext;
        if (!(context->flags & 2U))
            context->flags &= ~1U;
        else {
            {
                NpcRouteStateContextPrefix *context = &entry->stateContext;
                if (context->flags & 0x8300U)
                    return 1;
            }
            if (entry->stateContext.otherName)
                return 1;
            switch (*reinterpret_cast<const std::uint32_t *>(entry->currentCommand)) {
            case 3:
            case 4:
            case 7:
            case 8:
            case 9:
            case 10:
            case 16:
            case 18:
            case 19:
            case 32:
            case 40: {
                NpcRouteStateContextPrefix *context = &entry->stateContext;
                if (context->flags & 0x400U) {
                    if (!(context->flags & 0x1000U)) {
                        context->routeFlags |= 0x800U;
                        result = 1;
                    }
                } else if (*reinterpret_cast<const std::uint32_t *>(entry->currentCommand) != 4 &&
                    *reinterpret_cast<const std::uint32_t *>(entry->currentCommand) != 18)
                    StopNpcScriptRoute(&entry->stateContext, shen_name(), entry->rotation.y);
            } break;
            default:
                result = -1;
            }
        }
    } else
        return 0;
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t NpcAreaCompletionCallback(NpcRouteLiveEntry *entry) {
    if (entry->routeMotionState != 23) {
        StopNpcAreaRoute(entry);
        return 1;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t NpcClipCompletionCallback(NpcRouteLiveEntry *entry) {
    entry->streamCost = 2;
    if (entry->node) {
        mtwkRequestMotion(entry->motionWork, npcMotionClipFallbackAnimations[static_cast<std::int8_t>(entry->retained324)], LowerBody);
        mtwkSetRegionFlag(entry->motionWork, 0x47, 0, LowerBody);
        mtwkSetExtraFlag(entry->motionWork, 0x0e, 0);
        mtwkSetLayerFrameRange(entry->motionWork, 0.0f, entry->animationFrame + 1,
            static_cast<std::int32_t>(npcMotionClipFrameCounts[static_cast<std::int8_t>(entry->retained324)]), LowerBody);
        mtwkSetSpeed(entry->motionWork, entry->motionSpeed, LowerBody);
    }
    entry->flags &= ~0x40000U;
    entry->liveState = 0;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t NpcFixedHeightNullsub(NpcRouteLiveEntry *) {
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t NpcForcedHeightNullsub(NpcRouteLiveEntry *) {
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t NpcTimedCompletionCallback(NpcRouteLiveEntry *entry) {
    const std::uint32_t command = *reinterpret_cast<const std::uint32_t *>(entry->currentCommand);
    std::int32_t result = 0;
    if (command == 2 || command == 26)
        result = -1;
    else {
        ReleaseNpcTimedMotion(entry);
        entry->liveState = 0;
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ResetNpcEntryToAreaPoint(NpcRouteLiveEntry *entry);

void PrimeNpcInterpreter(NpcRouteLiveEntry *entry);

void SetNpcRouteScenePosition(NpcRouteStateContextPrefix *context, shen_name name, const Vector3 *position);

void SetNpcRouteSceneRotation(NpcRouteStateContextPrefix *context, shen_name name, const Vector3i *rotation);

void SetNpcRouteMoveFlags(NpcRouteStateContextPrefix *context, shen_name name, std::uint32_t flags);

std::int32_t InitializeNpcAreaMovement(shen_name name, NpcRouteStateContextPrefix *context, std::int32_t current, std::int32_t next);

void SetNpcRouteFollowing(NpcRouteStateContextPrefix *context, shen_name name, shen_name other, float distance);

void BeginNpcAreaRoute(NpcRouteLiveEntry *entry, std::int32_t currentTime);

void ApplyNpcCommandMotion(NpcRouteLiveEntry *entry, std::int32_t opcode);

void ResumeNpcScheduleMotion(NpcRouteLiveEntry *entry);

std::int32_t QueueNpcMotionAttachment(NpcRouteLiveEntry *entry, std::uint16_t motion, std::int32_t flags, std::int32_t slot);

std::int32_t RegisterNpcLocalBranch(NpcRouteLiveEntry *entry);

std::int32_t CancelNpcLocalBranch(NpcRouteLiveEntry *entry);

std::int32_t RegisterNpcGlobalBranch(NpcRouteLiveEntry *entry);

std::int32_t CancelNpcGlobalBranch(NpcRouteLiveEntry *entry);

std::int32_t BranchNpcArea(NpcRouteLiveEntry *entry, std::int32_t lowWord);

std::int32_t SkipNpcUnusedOpcode(NpcRouteLiveEntry *entry);

std::int32_t BranchNpcUnconditionally(NpcRouteLiveEntry *entry);

std::int32_t BranchNpcIfWeekday(NpcRouteLiveEntry *entry);

std::int32_t BranchNpcIfWeekend(NpcRouteLiveEntry *entry);

std::int32_t BranchNpcOtherAvailable(NpcRouteLiveEntry *entry);

std::int32_t BranchNpcWeather(NpcRouteLiveEntry *entry);

std::int32_t BeginNpcClipMotion(NpcRouteLiveEntry *entry);

std::int32_t SetNpcAttachedCharacter(NpcRouteLiveEntry *entry);

std::int32_t SetNpcForcedHeight(NpcRouteLiveEntry *entry);

std::int32_t BranchNpcDistance(NpcRouteLiveEntry *entry);

std::int32_t WaitNpcMotionFinished(NpcRouteLiveEntry *entry);

void ClearNpcMotionAttachment(NpcRouteLiveEntry *entry, std::int32_t slot);

void SetNpcScheduleSeedName(shen_name oldName, shen_name newName, std::int32_t enabled);

std::int32_t AcquireNpcTimedMotionSlot(NpcRouteLiveEntry *entry);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static float NpcCommandFloat(const std::uint32_t *value) {
    float result;
    std::memcpy(&result, value, sizeof(result));
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SimulateNpcMoveCommand(NpcRouteLiveEntry *entry, std::int32_t words) {
    entry->currentAreaPoint = entry->pendingAreaPoint = reinterpret_cast<const std::int16_t *>(entry->currentCommand)[2];
    entry->currentCommand += words * 4;
    if (*reinterpret_cast<const std::uint32_t *>(entry->currentCommand) == 0x2aU) {
        const std::uint32_t *command = reinterpret_cast<const std::uint32_t *>(entry->currentCommand);
        if (command[3])
            EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, command[1], command[2]);
        else
            stepUpdateFlagUtil(
                reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1], reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2]);
        entry->currentCommand += 16;
    }
    ResetNpcEntryToAreaPoint(entry);
    if (GetNpcAreaPointRouteState(entry->currentAreaPoint))
        BeginNpcAreaRoute(entry, npcSimulationTime);
    else
        PlaceNpcOutsideDoor(entry);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int32_t ExecuteNpcScheduleOpcode(NpcRouteLiveEntry *entry, std::int32_t currentTime) {
    typedef const std::int32_t *Words;
    typedef const float *Reals;
    std::int32_t result = 0;
    std::int32_t branched = 0;
    if (npcScheduleSimulationActive == 1) {
        if (entry->nextTimedCommand && npcSimulationTime >= entry->nextTimedCommandTime && (entry->flags & 0x20000U)) {
            entry->currentCommand = entry->nextTimedCommand;
            entry->scheduleProgress = 1;
            entry->waitDuration = 0;
        }
        if (entry->branchCommandStart && *reinterpret_cast<Words>(entry->branchCommandStart) == 6)
            return 2;
        switch (*reinterpret_cast<Words>(entry->currentCommand)) {
        case 47:
            if (NpcTimedCommandTime(entry->currentCommand) > npcSimulationTime)
                return 2;
            break;
        case 6:
            return 2;
        case 1:
            if (!AdvanceNpcTimedCommand(entry, npcSimulationTime))
                return 2;
            npcSimulationSinglePass = 1;
            break;
        }
    }
    const std::int32_t *const original = reinterpret_cast<Words>(entry->currentCommand);
    switch (*original) {
    case 0:
        break;
    case 1:
        if (!AdvanceNpcTimedCommand(entry, currentTime) && entry->loopCommand)
            entry->currentCommand = entry->loopCommand;
        break;
    case 2:
        BeginNpcScheduleWait(entry, currentTime);
        break;
    case 3:
    case 32:
        entry->currentAreaPoint = static_cast<std::int16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        entry->flags &= ~0xa000U;
        ResetNpcEntryToAreaPoint(entry);
        entry->rotation.y = ComputeNpcAreaPointYaw(GetNpcScriptRouteTable(), entry->currentAreaPoint);
        entry->liveState = 0;
        if (*reinterpret_cast<Words>(entry->currentCommand) == 3 && GetNpcAreaPointRouteState(entry->currentAreaPoint)) {
            BeginNpcAreaRoute(entry, currentTime);
            entry->commandFlags |= 0x100U;
        } else
            PlaceNpcOutsideDoor(entry);
        if (entry->node)
            SetCharacterWorldPosition(entry->node, &entry->localPosition);
        entry->flags |= 0x20000U;
        UpdateNpcRouteQueryState(entry, 0);
        entry->currentCommand += 8;
        break;
    case 41:
        entry->currentAreaPoint = static_cast<std::int16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        entry->currentCommand += 8;
        result = 1;
        break;
    case 4:
    case 18:
        if (!BindNpcStateCallback(entry, NpcRouteCompletionCallback))
            break;
        entry->flags &= ~0x8000U;
        if (npcScheduleSimulationActive == 1 && npcSimulationSinglePass == 1) {
            SimulateNpcMoveCommand(entry, 3);
            break;
        }
        entry->pendingAreaPoint = static_cast<std::int16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        InitializeNpcAreaMovement(entry->name, &entry->stateContext, entry->currentAreaPoint, entry->pendingAreaPoint);
        if (entry->flags & 0x40U) {
            NpcRouteStateContextPrefix *context = &entry->stateContext;
            context->routeFlags &= ~6U;
        }
        SetNpcRouteScenePosition(&entry->stateContext, 0, &entry->localPosition);
        SetNpcRouteSceneRotation(&entry->stateContext, 0, &entry->rotation);
        SetNpcRouteMoveFlags(&entry->stateContext, 0, entry->packedMoveFlags);
        entry->motionSpeed = reinterpret_cast<Reals>(entry->currentCommand)[2];
        SetNpcLocomotionScale(&entry->stateContext, 0, entry->motionSpeed);
        if (*reinterpret_cast<Words>(entry->currentCommand) == 4)
            entry->currentCommand += 12;
        else {
            SetNpcRouteMotionState(&entry->stateContext, entry->name, 1);
            entry->currentCommand += 12;
        }
        if (*reinterpret_cast<Words>(entry->currentCommand) == 55) {
            SetNpcRouteFollowing(&entry->stateContext, 0, reinterpret_cast<Words>(entry->currentCommand)[1], reinterpret_cast<Reals>(entry->currentCommand)[2]);
            entry->currentCommand += 12;
        }
        entry->liveState = 1;
        entry->scheduleProgress = 3;
        UpdateNpcRouteQueryState(entry, 1);
        break;
    case 5:
        entry->currentCommand = entry->loopCommand = entry->currentCommand + 8;
        entry->flags |= 0x20000U;
        result = 1;
        break;
    case 6:
        if (entry->loopCommand)
            entry->currentCommand = entry->loopCommand;
        else
            entry->currentCommand += 8;
        break;
    case 7:
        entry->currentMotionId = static_cast<std::uint16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        entry->currentAreaPoint = static_cast<std::int16_t>(reinterpret_cast<Words>(entry->currentCommand)[2]);
        ResetNpcEntryToAreaPoint(entry);
        entry->rotation.y = reinterpret_cast<Words>(entry->currentCommand)[3];
        entry->motionSpeed = reinterpret_cast<Reals>(entry->currentCommand)[4];
        entry->currentCommand += 20;
        ApplyNpcCommandMotion(entry, 7);
        entry->liveState = 3;
        entry->flags |= 0x20000U;
        PlaceNpcOutsideDoor(entry);
        UpdateNpcRouteQueryState(entry, 0);
        break;
    case 16:
        entry->currentMotionId = static_cast<std::uint16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        entry->currentAreaPoint = static_cast<std::int16_t>(reinterpret_cast<Words>(entry->currentCommand)[2]);
        ResetNpcEntryToAreaPoint(entry);
        entry->rotation.y = reinterpret_cast<Words>(entry->currentCommand)[3];
        entry->alternatePosition = *GetNpcAreaPointPosition(reinterpret_cast<Words>(entry->currentCommand)[4]);
        entry->alternatePosition.x -= npcRouteOrigin.x;
        entry->alternatePosition.z -= npcRouteOrigin.z;
        if (entry->flags & 0xa000U)
            entry->alternatePosition.y = entry->forcedY;
        entry->alternateWorldPosition = entry->alternatePosition;
        entry->alternatePosition.x += npcRouteOrigin.x;
        entry->alternatePosition.z += npcRouteOrigin.z;
        entry->alternateRotation.x = entry->alternateRotation.z = 0;
        entry->alternateRotation.y = reinterpret_cast<Words>(entry->currentCommand)[5];
        entry->motionSpeed = reinterpret_cast<Reals>(entry->currentCommand)[6];
        entry->currentCommand += 28;
        ApplyNpcCommandMotion(entry, 16);
        entry->liveState = 7;
        entry->flags |= 0x20000U;
        PlaceNpcOutsideDoor(entry);
        UpdateNpcRouteQueryState(entry, 0);
        break;
    case 40:
        if (!BindNpcStateCallback(entry, NpcTimedCompletionCallback))
            break;
        if (!AcquireNpcTimedMotionSlot(entry)) {
            entry->currentCommand += 36;
            break;
        }
        entry->currentMotionId = static_cast<std::uint16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        entry->currentAreaPoint = static_cast<std::int16_t>(reinterpret_cast<Words>(entry->currentCommand)[2]);
        ResetNpcEntryToAreaPoint(entry);
        entry->rotation.y = reinterpret_cast<Words>(entry->currentCommand)[3];
        entry->timedMotionHandle->name = reinterpret_cast<Words>(entry->currentCommand)[4];
        entry->timedMotionHandle->motion = static_cast<std::uint16_t>(reinterpret_cast<Words>(entry->currentCommand)[5]);
        entry->timedMotionHandle->offset.x = reinterpret_cast<Reals>(entry->currentCommand)[6];
        entry->timedMotionHandle->offset.y = reinterpret_cast<Reals>(entry->currentCommand)[7];
        entry->timedMotionHandle->offset.z = reinterpret_cast<Reals>(entry->currentCommand)[8];
        entry->motionSpeed = 1.0f;
        entry->flags |= 0x400U;
        entry->currentCommand += 36;
        ApplyNpcCommandMotion(entry, 40);
        entry->liveState = 3;
        entry->flags |= 0x20000U;
        PlaceNpcOutsideDoor(entry);
        UpdateNpcRouteQueryState(entry, 0);
        break;
    case 8:
        if (!BindNpcStateCallback(entry, NpcFixedHeightNullsub))
            break;
        entry->currentMotionId = static_cast<std::uint16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        entry->currentAreaPoint = static_cast<std::int16_t>(reinterpret_cast<Words>(entry->currentCommand)[2]);
        entry->flags |= 0x8000U;
        entry->forcedY = reinterpret_cast<Reals>(entry->currentCommand)[3];
        ResetNpcEntryToAreaPoint(entry);
        entry->rotation.y = reinterpret_cast<Words>(entry->currentCommand)[4];
        entry->motionSpeed = reinterpret_cast<Reals>(entry->currentCommand)[5];
        entry->currentCommand += 24;
        ApplyNpcCommandMotion(entry, 8);
        entry->liveState = 4;
        entry->flags |= 0x20000U;
        PlaceNpcOutsideDoor(entry);
        UpdateNpcRouteQueryState(entry, 0);
        break;
    case 9:
        entry->currentMotionId = static_cast<std::uint16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        entry->currentAreaPoint = static_cast<std::int16_t>(reinterpret_cast<Words>(entry->currentCommand)[2]);
        ResetNpcEntryToAreaPoint(entry);
        entry->rotation.y = reinterpret_cast<Words>(entry->currentCommand)[3];
        entry->currentCommand += 16;
        entry->motionSpeed = 1.0f;
        if (entry->node) {
            ResumeNpcScheduleMotion(entry);
            Character *node = entry->node;
            AssignCharacterControlFlags(node, ccmGetFlags(node) & ~6U);
        }
        entry->liveState = 5;
        entry->flags |= 0x20000U;
        PlaceNpcOutsideDoor(entry);
        UpdateNpcRouteQueryState(entry, 0);
        break;
    case 10:
        entry->currentAreaPoint = static_cast<std::int16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        ResetNpcEntryToAreaPoint(entry);
        entry->rotation.y = reinterpret_cast<Words>(entry->currentCommand)[2];
        entry->currentCommand += 12;
        entry->motionSpeed = 1.0f;
        ApplyNpcCommandMotion(entry, 10);
        entry->liveState = 6;
        entry->routeQueryState = 1;
        entry->flags |= 0x20000U;
        PlaceNpcOutsideDoor(entry);
        break;
    case 11:
        if (EvaluateNpcBranchCondition(entry, (reinterpret_cast<Words>(entry->currentCommand)[4] << 16) | reinterpret_cast<Words>(entry->currentCommand)[1],
                reinterpret_cast<Words>(entry->currentCommand)[2], currentTime)) {
            branched = SelectNpcScheduleBranch(entry, reinterpret_cast<Words>(entry->currentCommand)[3]);
            if (static_cast<std::uint32_t>(reinterpret_cast<Words>(entry->currentCommand)[4]) < 2)
                ClearNpcAreaRouteForEntry(entry);
        }
        if (!branched)
            entry->currentCommand += 20;
        result = 1;
        break;
    case 30:
        result = RegisterNpcGlobalBranch(entry);
        break;
    case 31:
        result = CancelNpcGlobalBranch(entry);
        break;
    case 43:
        result = RegisterNpcLocalBranch(entry);
        break;
    case 44:
        result = CancelNpcLocalBranch(entry);
        break;
    case 39:
        result = BranchNpcArea(entry, 0);
        break;
    case 12:
        result = BranchNpcArea(entry, 1);
        break;
    case 13:
        result = BranchNpcIfWeekday(entry);
        break;
    case 14:
        result = BranchNpcIfWeekend(entry);
        break;
    case 15:
        result = BranchNpcOtherAvailable(entry);
        break;
    case 48:
        result = BranchNpcWeather(entry);
        break;
    case 17:
        if (!entry->node || npcScheduleSimulationActive == 1) {
            entry->loadHumanName = reinterpret_cast<Words>(entry->currentCommand)[1];
            entry->currentCommand += 8;
            result = 1;
        }
        break;
    case 19:
        result = BeginNpcClipMotion(entry);
        break;
    case 20:
        entry->currentCommand += 8;
        entry->flags |= 0x20U;
        BeginNpcRouteStreamOut(entry, 0);
        result = 1;
        break;
    case 21:
        entry->currentCommand += 8;
        entry->flags &= ~0x20U;
        result = 1;
        break;
    case 22:
        entry->currentCommand += 8;
        entry->flags |= 0x40U;
        if (entry->node) {
            cpamSetFlag(entry->node, 0);
            SetCharacterAlpha(entry->node, 0.0f);
            Character *node = entry->node;
            AssignCharacterControlFlags(node, ccmGetFlags(node) & ~8U);
        }
        result = 1;
        break;
    case 23:
        if (entry->node || npcScheduleSimulationActive == 1)
            entry->currentCommand += 8;
        break;
    case 36:
        entry->currentCommand += 8;
        result = 1;
        break;
    case 24:
        entry->pendingRouteQueryState = static_cast<std::int8_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        entry->currentCommand += 8;
        result = 1;
        break;
    case 25:
        result = SetNpcAttachedCharacter(entry);
        break;
    case 26:
        result = WaitNpcMotionFinished(entry);
        break;
    case 27:
        result = SetNpcForcedHeight(entry);
        break;
    case 28:
        *reinterpret_cast<float *>(entry->retained36c) = reinterpret_cast<Reals>(entry->currentCommand)[1];
        entry->currentCommand += 8;
        result = 1;
        break;
    case 29:
        entry->currentCommand += 16;
        break;
    case 37:
        result = BranchNpcDistance(entry);
        break;
    case 33:
        entry->packedMoveFlags = (reinterpret_cast<Words>(entry->currentCommand)[1] << 8) | (entry->packedMoveFlags & 0xffU);
        entry->currentCommand += 8;
        result = 1;
        break;
    case 34:
    case 35:
        if (!BindNpcStateCallback(entry, NpcRouteCompletionCallback))
            break;
        entry->flags &= ~0x8000U;
        if (npcScheduleSimulationActive == 1 && npcSimulationSinglePass == 1) {
            SimulateNpcMoveCommand(entry, 5);
            break;
        }
        entry->pendingAreaPoint = static_cast<std::int16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]);
        InitializeNpcAreaMovement(entry->name, &entry->stateContext, entry->currentAreaPoint, entry->pendingAreaPoint);
        if (entry->flags & 0x40U) {
            NpcRouteStateContextPrefix *context = &entry->stateContext;
            context->routeFlags &= ~6U;
        }
        SetNpcRouteScenePosition(&entry->stateContext, 0, &entry->localPosition);
        SetNpcRouteSceneRotation(&entry->stateContext, 0, &entry->rotation);
        SetNpcRouteMoveFlags(&entry->stateContext, 0, entry->packedMoveFlags);
        entry->motionSpeed = reinterpret_cast<Reals>(entry->currentCommand)[4];
        SetNpcLocomotionScale(&entry->stateContext, entry->name, entry->motionSpeed);
        entry->liveState = 1;
        if (*reinterpret_cast<Words>(entry->currentCommand) == 34)
            entry->currentCommand += 20;
        else {
            SetNpcRouteMotionState(&entry->stateContext, entry->name, 1);
            entry->currentCommand += 20;
        }
        entry->scheduleProgress = 3;
        UpdateNpcRouteQueryState(entry, 1);
        break;
    case 38:
        if (reinterpret_cast<Words>(entry->currentCommand)[3])
            EV_SetPlayerFlag(
                PlayerFlagWriteMode_Internal, reinterpret_cast<Words>(entry->currentCommand)[1], reinterpret_cast<Words>(entry->currentCommand)[2]);
        else
            stepUpdateFlagUtil(reinterpret_cast<Words>(entry->currentCommand)[1], reinterpret_cast<Words>(entry->currentCommand)[2]);
        entry->currentCommand += 16;
        result = 1;
        break;
    case 42:
        entry->currentCommand += 16;
        result = 1;
        break;
    case 47:
        entry->currentCommand += 8;
        if (npcScheduleSimulationActive == 1)
            npcSimulationSinglePass = 1;
        result = 1;
        break;
    case 49:
        if (!reinterpret_cast<Words>(entry->currentCommand)[1])
            entry->flags |= 0x80000U;
        else
            entry->flags &= ~0x80000U;
        entry->currentCommand += 8;
        result = 1;
        break;
    case 50:
        if (reinterpret_cast<Words>(entry->currentCommand)[1])
            entry->flags |= 0x400000U;
        else
            entry->flags &= ~0x400000U;
        entry->currentCommand += 8;
        result = 1;
        break;
    case 51:
        if (reinterpret_cast<Words>(entry->currentCommand)[1]) {
            entry->motionAttachments[0].flags &= ~0x40U;
            entry->controlFlags &= ~0x10U;
        } else
            entry->controlFlags |= 0x10U;
        entry->currentCommand += 8;
        result = 1;
        break;
    case 52:
        if (!reinterpret_cast<Words>(entry->currentCommand)[1]) {
            entry->flags |= 0x8000000U;
            Character *node = entry->node;
            if (node)
                AssignCharacterControlFlags(node, ccmGetFlags(node) & ~12U);
        } else {
            entry->flags &= ~0x8000000U;
            Character *node = entry->node;
            if (node)
                AssignCharacterControlFlags(node, ccmGetFlags(node) | 12U);
        }
        entry->currentCommand += 8;
        result = 1;
        break;
    case 53:
        if (reinterpret_cast<Words>(entry->currentCommand)[1])
            entry->packedMoveFlags |= 0x80000U;
        else
            entry->packedMoveFlags &= ~0x80000U;
        entry->currentCommand += 8;
        result = 1;
        break;
    case 54:
        if (reinterpret_cast<Words>(entry->currentCommand)[1])
            SetNpcScheduleSeedName(entry->name, 0, 1);
        else
            SetNpcScheduleSeedName(0, entry->name, 1);
        entry->currentCommand += 8;
        result = 1;
        break;
    case 45:
        QueueNpcMotionAttachment(
            entry, static_cast<std::uint16_t>(reinterpret_cast<Words>(entry->currentCommand)[1]), reinterpret_cast<Words>(entry->currentCommand)[2] | 0x20, 1);
        entry->currentCommand += 12;
        result = 1;
        break;
    case 46:
        ClearNpcMotionAttachment(entry, 1);
        entry->currentCommand += 8;
        result = 1;
        break;
    case 55:
        entry->currentCommand += 12;
        result = 1;
        break;
    case -1:
        entry->scheduleProgress = 4;
        result = 2;
        break;
    default:
        entry->scheduleProgress = 4;
        entry->liveState = 14;
        entry->routeQueryState = -1;
    }
    if (*original != -1)
        entry->branchCommandStart = reinterpret_cast<const std::byte *>(original);
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t RegisterNpcLocalBranch(NpcRouteLiveEntry *entry) {
    NpcLocalBranchTrigger *trigger = entry->localBranches;
    NpcLocalBranchTrigger *free = 0;
    std::int32_t index = 0;
    for (; index < 4; ++index, ++trigger) {
        if (trigger->packedCondition ==
            ((reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[4] << 16U) | reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1]))
            break;
        if (trigger->packedCondition == 0xffffffffU && !free)
            free = trigger;
    }
    if (index == 4 && free) {
        index = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(entry->currentCommand - 0x38));
        index -= reinterpret_cast<std::uintptr_t>(entry->scriptRecord);
        free->commandOffset = static_cast<std::uint16_t>(index);
        free->packedCondition =
            (reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[4] << 16U) | reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1];
        free->parameter = reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2];
        free->target = *reinterpret_cast<const std::int8_t *>(entry->currentCommand + 12);
        ++entry->localBranchCount;
    }
    entry->currentCommand += 20;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t CancelNpcLocalBranch(NpcRouteLiveEntry *entry) {
    NpcLocalBranchTrigger *branch = entry->localBranches;
    for (std::int32_t index = 0; index < 4; ++index, ++branch) {
        const std::uint32_t *const command = reinterpret_cast<const std::uint32_t *>(entry->currentCommand);
        if (branch->packedCondition == ((command[2] << 16U) | command[1])) {
            branch->packedCondition = 0xffffffffU;
            --entry->localBranchCount;
            break;
        }
    }
    entry->currentCommand += 12;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t RegisterNpcGlobalBranch(NpcRouteLiveEntry *entry) {
    NpcGlobalBranchTrigger *trigger = npcGlobalBranchTriggers;
    NpcGlobalBranchTrigger *free = 0;
    std::int32_t index = 0;
    for (; index < 16; ++index, ++trigger) {
        if (trigger->owner == entry->name &&
            trigger->packedCondition ==
                ((reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[4] << 16U) |
                    reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1]))
            break;
        if (trigger->packedCondition == 0xffffffffU && !free)
            free = trigger;
    }
    if (index == 16 && free) {
        index = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(entry->currentCommand - 0x38));
        index -= reinterpret_cast<std::uintptr_t>(entry->scriptRecord);
        *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(&free->parameter) + 2) = static_cast<std::uint16_t>(index);
        free->owner = entry->name;
        free->packedCondition =
            (reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[4] << 16U) | reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1];
        free->target = reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2];
        *reinterpret_cast<std::uint8_t *>(&free->parameter) = static_cast<std::uint8_t>(reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[3]);
        ++entry->globalBranchCount;
        if (reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[4] == 2 && npcScheduleSeedNames[1] == entry->name && !(entry->flags & 0x10000000U)) {
            if (!GetScenarioByteStat(0x22)) {
                DateTime time = npcCurrentDateTime;
                time.second = 30;
                const std::int32_t seconds = ComputeSecondsSince1980(time);
                npcSpecialMotionEpoch = seconds;
                SetScenarioByteStat(0x30, npcSpecialMotionEpoch >> 24U);
                SetScenarioByteStat(0x31, npcSpecialMotionEpoch >> 16U & 0xffU);
                SetScenarioByteStat(0x20, npcSpecialMotionEpoch >> 8U & 0xffU);
                SetScenarioByteStat(0x21, npcSpecialMotionEpoch & 0xffU);
            } else
                npcSpecialMotionEpoch =
                    (GetScenarioByteStat(0x30) << 24U) + (GetScenarioByteStat(0x31) << 16U) + (GetScenarioByteStat(0x20) << 8U) + GetScenarioByteStat(0x21);
        }
    }
    entry->currentCommand += 20;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t CancelNpcGlobalBranch(NpcRouteLiveEntry *entry) {
    NpcGlobalBranchTrigger *trigger = npcGlobalBranchTriggers;
    for (std::int32_t index = 0; index < 16; ++index, ++trigger) {
        if (trigger->owner == entry->name &&
            trigger->packedCondition ==
                ((reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2] << 16U) |
                    reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1])) {
            trigger->packedCondition = 0xffffffffU;
            --entry->globalBranchCount;
            break;
        }
    }
    entry->currentCommand += 12;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t BranchNpcArea(NpcRouteLiveEntry *entry, std::int32_t lowWord) {
    std::int32_t selected = 0;
    if (!lowWord) {
        if (reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1] == npcLoadedArea)
            selected = SelectNpcScheduleBranch(entry, reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2]);
    } else {
        if ((reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1] & 0xffffU) == (npcLoadedArea & 0xffffU))
            selected = SelectNpcScheduleBranch(entry, reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2]);
    }
    if (!selected)
        entry->currentCommand += 12;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t SkipNpcUnusedOpcode(NpcRouteLiveEntry *entry) {
    entry->currentCommand += 8;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t BranchNpcUnconditionally(NpcRouteLiveEntry *entry) {
    const std::uint32_t *const command = reinterpret_cast<const std::uint32_t *>(entry->currentCommand);
    if (!SelectNpcScheduleBranch(entry, command[1]))
        entry->currentCommand += 8;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t BranchNpcOtherAvailable(NpcRouteLiveEntry *entry) {
    std::int32_t selected = 0;
    const NpcRouteLiveEntry *const other = FindNpcRouteEntry(shen_name(reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1]));
    if (other) {
        if (static_cast<std::int8_t>(other->scriptRecord[6]) == npcCurrentDateTime.month &&
            static_cast<std::int8_t>(other->scriptRecord[7]) == npcCurrentDateTime.day)
            selected = SelectNpcScheduleBranch(entry, reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2]);
    }
    if (!selected)
        entry->currentCommand += 12;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t BranchNpcWeather(NpcRouteLiveEntry *entry) {
    std::int32_t selected = 0;
    switch (reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1]) {
    case 0:
        if (npcEndOfDayWeatherForecast < 0.15f)
            selected = 1;
        break;
    case 1:
        if (!(npcEndOfDayWeatherForecast < 0.15f))
            selected = 1;
        break;
    }
    if (selected)
        selected = SelectNpcScheduleBranch(entry, reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2]);
    if (!selected)
        entry->currentCommand += 12;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t SetNpcAttachedCharacter(NpcRouteLiveEntry *entry) {
    entry->attachedCharacter = reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1];
    entry->attachmentOffset.x = reinterpret_cast<const float *>(entry->currentCommand)[2];
    entry->attachmentOffset.y = reinterpret_cast<const float *>(entry->currentCommand)[3];
    entry->attachmentOffset.z = reinterpret_cast<const float *>(entry->currentCommand)[4];
    entry->attachmentYaw = reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[5];
    entry->currentCommand += 24;
    entry->flags |= 0x20000U;
    PlaceNpcOutsideDoor(entry);
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t SetNpcForcedHeight(NpcRouteLiveEntry *entry) {
    if (!BindNpcStateCallback(entry, NpcForcedHeightNullsub))
        return 0;
    if (reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1]) {
        entry->flags |= 0x2000U;
        entry->forcedY = reinterpret_cast<const float *>(entry->currentCommand)[2];
    } else {
        entry->flags &= ~0x2000U;
        entry->forcedY = 0.0f;
    }
    entry->currentCommand += 12;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t BranchNpcDistance(NpcRouteLiveEntry *entry) {
    std::uint32_t selected = 0;
    const std::uint32_t *const command = reinterpret_cast<const std::uint32_t *>(entry->currentCommand);
    const float *const range = reinterpret_cast<const float *>(command + 1);
    const float x = npcPlayerPosition.x - entry->localPosition.x, z = npcPlayerPosition.z - entry->localPosition.z;
    if (x * x + z * z > *range)
        selected = SelectNpcScheduleBranch(entry, command[2]);
    if (!selected)
        entry->currentCommand += 12;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t WaitNpcMotionFinished(NpcRouteLiveEntry *entry) {
    std::int32_t result = 0;
    if ((entry->node && mtwkSetRegionFlag(entry->motionWork, 0x41, 1, LowerBody)) || npcScheduleSimulationActive == 1) {
        entry->scheduleProgress = 1;
        entry->liveState = 0;
        entry->currentCommand += 8;
        result = 1;
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetNpcScheduleSeedName(shen_name value, shen_name expected, std::int32_t index) {
    if (value || npcScheduleSeedNames[index] == expected)
        npcScheduleSeedNames[index] = value;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t BeginNpcClipMotion(NpcRouteLiveEntry *entry) {
    if (!BindNpcStateCallback(entry, NpcClipCompletionCallback))
        return 0;
    entry->motionSpeed = 1.0f;
    entry->retained324 = entry->currentCommand[4];
    entry->currentCommand += 8;
    entry->streamCost = 1;
    entry->routeQueryState = 3;
    entry->flags |= 0x20000U;
    PlaceNpcOutsideDoor(entry);
    if (entry->branchCommandStart && *reinterpret_cast<const std::uint32_t *>(entry->branchCommandStart) == 9) {
        entry->flags |= 0x40000U;
        entry->animationFrame = static_cast<std::int16_t>(static_cast<std::int32_t>(entry->animationFrame + npcFrameCount) %
            static_cast<std::int32_t>(npcMotionClipFrameCounts[static_cast<std::int8_t>(entry->retained324)]));
    } else
        entry->animationFrame =
            static_cast<std::int16_t>(static_cast<std::int32_t>(dc::sdk::RandomFloat() * static_cast<float>(static_cast<std::int32_t>(npcFrameCount))) %
                static_cast<std::int32_t>(npcMotionClipFrameCounts[static_cast<std::int8_t>(entry->retained324)]));
    if (npcMotionClipFallbackAnimations[static_cast<std::int8_t>(entry->retained324)]) {
        entry->liveState = 11;
        if (entry->node) {
            mtwkRequestMotion(entry->motionWork, npcMotionClipFallbackAnimations[static_cast<std::int8_t>(entry->retained324)], LowerBody);
            mtwkSetExtraFlag(entry->motionWork, 14, 0);
            mtwkSetLayerFrameRange(entry->motionWork, 0.0f, entry->animationFrame,
                static_cast<float>(static_cast<std::int32_t>(npcMotionClipFrameCounts[static_cast<std::int8_t>(entry->retained324)])), LowerBody);
            mtwkSetSpeed(entry->motionWork, entry->motionSpeed, LowerBody);
        }
    } else
        entry->liveState = 8;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

struct NpcScheduleSeed {
    std::uint8_t state;
    std::int8_t queryState, moveFlags;
    std::uint8_t attachmentControl;
    shen_name name;
    std::uint32_t flags;
    std::uint16_t commandOffset, loopOffset;
    std::uint8_t scheduleProgress, liveState;
    std::int8_t branchIndex;
    std::uint8_t retained13;
    std::int16_t currentPoint, pendingPoint, yaw;
    std::uint16_t retained1a;
    std::int16_t attachmentMotion, attachmentFlags;
    std::uint32_t controlFlags;
    std::uint16_t localBranches[4], globalBranches[16];
    std::uint32_t stateData[7];
};

typedef char NpcScheduleSeedSize[(sizeof(NpcScheduleSeed) == 0x68) ? 1 : -1];

std::int32_t RestoreNpcRouteSeedData(NpcRouteLiveEntry *entry, const std::uint32_t *data);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static shen_name CaptureNpcScheduleSeed(shen_name name, NpcScheduleSeed *seed) {
    std::size_t size = sizeof(*seed);
    std::memset(seed, 0, size);
    if (!name) {
        seed->state = 0;
        return shen_name();
    }
    const NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
    if (!entry) {
        seed->state = 0;
        return shen_name();
    }
    if (entry->pendingRouteQueryState)
        seed->queryState = -entry->pendingRouteQueryState;
    else
        seed->queryState = entry->routeQueryState;
    seed->moveFlags = 0;
    seed->name = name;
    seed->scheduleProgress = entry->scheduleProgress;
    seed->liveState = entry->liveState;
    std::int32_t value = entry->currentCommand - (entry->scriptRecord + 0x38);
    seed->commandOffset = static_cast<std::uint16_t>(value);
    seed->branchIndex = static_cast<std::int8_t>(entry->retained31c);
    if (entry->loopCommand) {
        value = entry->loopCommand - (entry->scriptRecord + 0x38);
        seed->loopOffset = static_cast<std::uint16_t>(value);
    }
    seed->flags = entry->flags & 0xdffffff8U;
    seed->currentPoint = entry->currentAreaPoint;
    seed->pendingPoint = entry->pendingAreaPoint;
    seed->yaw = static_cast<std::int16_t>(entry->rotation.y);
    const NpcLocalBranchTrigger *local = entry->localBranches;
    value = 0;
    std::int32_t index;
    for (index = 0; index < 4; ++index, ++local)
        if (local->packedCondition != UINT32_MAX)
            seed->localBranches[value++] = local->commandOffset;
    if (value != 4)
        seed->localBranches[value] = 0xffffU;
    const NpcGlobalBranchTrigger *trigger = npcGlobalBranchTriggers;
    value = 0;
    for (index = 0; index < 16; ++index, ++trigger)
        if (trigger->packedCondition != UINT32_MAX && trigger->owner == entry->name)
            seed->globalBranches[value++] = reinterpret_cast<const std::uint16_t *>(&trigger->parameter)[1];
    if (value != 16)
        seed->globalBranches[value] = 0xffffU;
    seed->controlFlags = entry->controlFlags;
    if (entry->controlFlags & 0x10U)
        seed->attachmentControl = 1;
    else
        seed->attachmentControl = 0;
    seed->attachmentFlags = entry->motionAttachments[1].flags & 0x1eU;
    seed->attachmentMotion = entry->motionAttachments[1].motion;
    switch (static_cast<std::int8_t>(entry->liveState)) {
    default: {
        seed->state = 1;
        std::uint32_t *const data = seed->stateData;
        *reinterpret_cast<std::uint16_t *>(data) = entry->currentMotionId;
        data[1] = entry->waitStartTime;
        data[2] = entry->waitDuration;
        break;
    }
    case 1:
        seed->state = 2;
        CaptureNpcRouteSeedData(entry, seed->stateData);
        break;
    case 9:
        seed->state = 3;
        break;
    }
    return entry->name;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void CaptureNpcScheduleSeedPayload(void *payload) {
    shen_name names[2];
    NpcScheduleSeed *seed = static_cast<NpcScheduleSeed *>(payload);
    if (npcScheduleSeedNames[0] == npcScheduleSeedNames[1]) {
        names[0] = npcScheduleSeedNames[0];
        names[1] = 0;
    } else {
        names[0] = npcScheduleSeedNames[0];
        names[1] = npcScheduleSeedNames[1];
    }
    for (std::int32_t index = 0; index < 2; ++index, ++seed)
        npcScheduleSeedNames[index] = CaptureNpcScheduleSeed(names[index], seed);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

shen_name RestoreNpcScheduleSeed(const NpcScheduleSeed *seed, std::int32_t currentTime) {
    if (!seed->state)
        return shen_name();
    NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
    std::int32_t index = 0;
    for (; index < npcRouteEntryCount && seed->name != entry->name; ++index, ++entry) {
    }
    if (index == npcRouteEntryCount)
        return shen_name();
    InitializeNpcInterpreterState(entry, entry->poolIndex);
    entry->nextTimedCommand = FindNextNpcTimedCommand(entry->currentCommand);
    if (entry->nextTimedCommand) {
        entry->nextTimedCommandTime = NpcTimedCommandTime(entry->nextTimedCommand);
        while (entry->nextTimedCommand && AdvanceNpcTimedCommand(entry, currentTime)) {
        }
    }
    const std::int32_t saved = npcScheduleSimulationActive;
    entry->flags |= 0x10000000U;
    npcScheduleSimulationActive = 0;
    for (index = 0; index < 4 && seed->localBranches[index] != 0xffffU; ++index) {
        entry->currentCommand = entry->scriptRecord + 0x38 + static_cast<std::uint16_t>(seed->localBranches[index]);
        ExecuteNpcScheduleOpcode(entry, currentTime);
    }
    for (index = 0; index < 16 && seed->globalBranches[index] != 0xffffU; ++index) {
        entry->currentCommand = entry->scriptRecord + 0x38 + static_cast<std::uint16_t>(seed->globalBranches[index]);
        ExecuteNpcScheduleOpcode(entry, currentTime);
    }
    npcScheduleSimulationActive = saved;
    entry->packedMoveFlags = static_cast<std::int32_t>(seed->moveFlags);
    entry->currentCommand = entry->scriptRecord + 0x38 + seed->commandOffset;
    *reinterpret_cast<std::int8_t *>(&entry->retained31c) = seed->branchIndex;
    entry->branchCommandStart = entry->currentCommand;
    if (seed->loopOffset)
        entry->loopCommand = entry->scriptRecord + 0x38 + seed->loopOffset;
    entry->flags = seed->flags;
    entry->currentAreaPoint = seed->currentPoint;
    entry->pendingAreaPoint = seed->pendingPoint;
    PrimeNpcInterpreter(entry);
    entry->flags |= 0x10000000U;
    entry->controlFlags = seed->controlFlags;
    if (seed->attachmentControl)
        entry->controlFlags |= 0x10U;
    if (seed->attachmentMotion)
        QueueNpcMotionAttachment(entry, seed->attachmentMotion, seed->attachmentFlags | 0x20U, 1);
    if (seed->queryState < 0)
        entry->pendingRouteQueryState = -seed->queryState;
    else
        entry->routeQueryState = seed->queryState;
    entry->scheduleProgress = seed->scheduleProgress;
    entry->liveState = seed->liveState;
    entry->rotation.y = seed->yaw;
    switch (static_cast<std::int8_t>(seed->state)) {
    case 1: {
        const std::uint32_t *const data = seed->stateData;
        entry->currentMotionId = *reinterpret_cast<const std::uint16_t *>(data);
        entry->waitStartTime = data[1];
        entry->waitDuration = data[2];
        break;
    }
    case 2:
        RestoreNpcRouteSeedData(entry, seed->stateData);
        break;
    case 3:
        ResetNpcEntryToAreaPoint(entry);
        entry->flags |= 0x20000U;
        if (GetNpcAreaPointRouteState(entry->currentAreaPoint))
            BeginNpcAreaRoute(entry, currentTime);
    }
    return entry->name;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RestorePendingNpcScheduleSeeds(std::int32_t currentTime) {
    const NpcScheduleSeed *seed = reinterpret_cast<const NpcScheduleSeed *>(pendingNpcScheduleSeeds);
    if (hasPendingNpcScheduleSeeds) {
        for (std::int32_t index = 0; index < 2; ++index, ++seed)
            npcScheduleSeedNames[index] = RestoreNpcScheduleSeed(seed, currentTime);
        hasPendingNpcScheduleSeeds = 0;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

bool InitializeNpcAreaRoute(NpcRouteLiveEntry *entry);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t InterpretNpcAreaCommand(NpcRouteLiveEntry *entry, std::int32_t currentTime) {
    std::int32_t result = 0;
    switch (*reinterpret_cast<const std::uint32_t *>(entry->currentCommand)) {
    case 1:
        if (AdvanceNpcTimedCommand(entry, currentTime))
            break;
        if (entry->loopCommand)
            entry->currentCommand = entry->loopCommand;
        if (npcScheduleSimulationActive == 1)
            npcSimulationStop = 1;
        break;
    case 2:
        BeginNpcScheduleWait(entry, currentTime);
        break;
    case 36:
        entry->pendingRouteQueryState = static_cast<std::int8_t>(reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1]);
        entry->currentCommand += 8;
        result = 1;
        break;
    case 45:
        entry->waitParameter = static_cast<std::uint16_t>(reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1]);
        entry->opcodeByte = static_cast<std::uint8_t>(reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2]);
        entry->currentCommand += 12;
        result = 1;
        break;
    case 46:
        entry->waitParameter = 0;
        entry->opcodeByte = 0xff;
        entry->currentCommand += 8;
        result = 1;
        break;
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
inline void YieldNpcLoadingPresentation() {
    npcSimDeltaTime = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
    if (IsLoadingDrawActive() && static_cast<float>(npcPresentedTime - npcSimDeltaTime) / 12.5f > 15000.0f) {
        DrawLoadingModes();
        dc::sdk::SubmitFrame();
        npcPresentedTime = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetNpcRuntime() {
    npcRouteTableReady = 0;
    npcReloadPassStarted = 0;
    npcScheduleSimulationActive = 0;
    npcAuthoredEntryCount = npcScheduleRecordCount;
    npcRouteEntryCount = npcScheduleRecordCount + 6;
    npcActiveStreamCost = 0;
    npcFrameCount = 0;
    npcStreamThrottle = 0.0f;
    npcCrowdPhase = 0;
    npcCrowdHead = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
    npcCrowdBookmark = npcCrowdHead;
    npcPositionQueryMode = 0;
    npcTimedMotionCount = 0;
    for (std::int32_t index = 0; index < 2; ++index)
        npcTimedMotionSlots[index].occupied = 0;
    npcPresentedTime = *reinterpret_cast<volatile std::uint32_t *>(kFrameTimerCounterRegister);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetNpcDeferredBranches() {
    NpcGlobalBranchTrigger *trigger = npcGlobalBranchTriggers;
    for (std::int32_t index = 0; index < 16; ++index, ++trigger)
        trigger->packedCondition = 0xffffffffU;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ClearNpcDeferredBranches(NpcRouteLiveEntry *entry) {
    entry->localBranchCount = 0;
    std::int32_t index;
    for (index = 0; index < 4; ++index)
        entry->localBranches[index].packedCondition = 0xffffffffU;
    NpcGlobalBranchTrigger *trigger = npcGlobalBranchTriggers;
    for (index = 0; entry->globalBranchCount && index < 16; ++index, ++trigger) {
        if (trigger->packedCondition != 0xffffffffU && trigger->owner == entry->name) {
            trigger->packedCondition = 0xffffffffU;
            --entry->globalBranchCount;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void SimulateNpcScheduleTime(std::int32_t startTime, std::int32_t endTime, std::int32_t onlyUninitialized) {
    NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
    ActivateNpcAreaRoutes(0);
    npcScheduleSimulationActive = 1;
    npcSimulationTime = endTime;
    for (std::int32_t index = 0; index < npcAuthoredEntryCount; ++index, ++entry) {
        if ((entry->flags & 0x100000U) || (onlyUninitialized && (entry->scriptRecord[0x0b] & 1U)))
            continue;
        if (entry->flags & 0x10000000U) {
            entry->flags &= ~0x10000000U;
            continue;
        }
        npcSimulationSinglePass = 1;
        npcSimulationStop = 0;
        for (std::int32_t time = startTime; time < endTime; ++time) {
            npcRouteBuildCount = 0;
            InterpretNpcScheduleEntry(entry, time);
            if ((entry->commandFlags & 0x80U) || npcSimulationStop == 1)
                break;
            if (onlyUninitialized && time % 300 == 0)
                YieldNpcLoadingPresentation();
        }
    }
    npcScheduleSimulationActive = 0;
    npcSimulationSinglePass = 0;
    npcSimulationStop = 0;
    ActivateNpcAreaRoutes(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void InitializeNpcRouteEntries(std::byte **records, NpcRouteSpline *routes, std::int32_t reload) {
    std::int32_t index;
    ResetNpcRuntime();
    shenGetCurrentDateTime(npcCurrentDateTime);
    const std::int32_t seconds = ComputeSecondsSince1980(npcCurrentDateTime);
    npcCurrentSeconds = seconds;
    npcEndOfDayWeatherForecast = GetMaximumWeatherRainForecast((23 - npcCurrentDateTime.hour) * 60 + (59 - npcCurrentDateTime.minute));
    npcHalfHourWeatherForecast = GetMaximumWeatherRainForecast(30);
    ResetNpcDeferredBranches();
    if (!reload) {
        npcScheduleRecords = records;
        for (index = 0; index < npcAuthoredEntryCount; ++index)
            npcScheduleRecords[index] += reinterpret_cast<std::uintptr_t>(npcScheduleRecords);
        LoadNPCFiles(routes);
    }
    NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
    for (index = 0; index < npcRouteEntryCount; ++index, ++entry) {
        if (reload)
            ClearNpcAreaRouteForEntry(entry);
        InitializeNpcInterpreterState(entry, index);
        npcSlotFlags[index] = 0;
        if ((entry->scriptRecord[0x0b] & 8U) && npcEndOfDayWeatherForecast >= 0.15f) {
            entry->flags |= 0x100000U;
            *reinterpret_cast<float *>(reinterpret_cast<std::byte *>(entry) + 0x36c) = -10000.0f;
        }
        YieldNpcLoadingPresentation();
    }
    reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage)->previous = 0;
    (reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage) + npcRouteEntryCount - 1)->next = 0;
    if (!reload)
        RestorePendingNpcScheduleSeeds(2 * (npcCurrentDateTime.hour * 3600 + npcCurrentDateTime.minute * 60));
    SimulateNpcScheduleTime(0xe100, 2 * (npcCurrentDateTime.hour * 3600 + npcCurrentDateTime.minute * 60), 1);
    entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
    for (index = 0; index < npcAuthoredEntryCount; ++index, ++entry) {
        if ((entry->scriptRecord[0x0b] & 1U) == 0)
            entry->flags |= 0x10000U;
        else
            reinterpret_cast<std::int8_t *>(entry->scriptRecord)[0x0b] &= ~1;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RegisterAuthoredNpcEntries(std::byte **records, NpcRouteSpline *routes) {
    npcSpecialMotionEpoch = static_cast<std::uint32_t>(GetScenarioByteStat(0x30)) * 0x1000000U +
        static_cast<std::uint32_t>(GetScenarioByteStat(0x31)) * 0x10000U + static_cast<std::uint32_t>(GetScenarioByteStat(0x20)) * 0x100U +
        GetScenarioByteStat(0x21);
    InitializeNpcRouteEntries(records, routes, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void ReinitializeNpcRouteEntries() {
    InitializeNpcRouteEntries(0, 0, 1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RebuildNpcRuntimeAfterReload() {
    if (!npcFrameCount)
        return;
    ResetNpcRuntime();
    ActivateNpcAreaRoutes(1);
    float offset = 0.0f;
    const std::int32_t *table;
    const std::int32_t *pair;
    switch (GetNpcScheduleVersion()) {
    case 4: {
        std::int32_t selected = 0;
        table = reinterpret_cast<const std::int32_t *>(GetNpcScheduleEntryData());
        while (table[0]) {
            const std::int32_t count = table[1];
            if (static_cast<std::uint32_t>(table[0]) == npcLoadedArea) {
                pair = table + 2;
                for (std::int32_t index = 0; index < count; ++index, pair += 2) {
                    if (g_entryPoint == pair[0]) {
                        offset = *reinterpret_cast<const float *>(pair + 1);
                        selected = 1;
                        break;
                    }
                }
            }
            if (selected == 1)
                break;
            table += count * 2 + 2;
        }
        break;
    }
    case 3:
        pair = reinterpret_cast<const std::int32_t *>(GetNpcScheduleEntryData());
        while (pair[0] != -1) {
            if (g_entryPoint == pair[0]) {
                offset = *reinterpret_cast<const float *>(pair + 1);
                break;
            }
            pair += 2;
        }
        break;
    }
    NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
    for (std::int32_t index = 0; index < npcRouteEntryCount; ++index, ++entry) {
        const std::int32_t changed = NpcRouteConditionsChanged(entry);
        const std::int8_t oldFlags = static_cast<std::int8_t>(npcSlotFlags[index]);
        npcSlotFlags[index] = 0;
        const NpcRouteStateContextPrefix *context = &entry->stateContext;
        if (index < npcAuthoredEntryCount && changed == 1 && !(context->flags & 0x100U)) {
            if (entry->stateContext.otherName)
                ReleaseDoor(entry->stateContext.otherName, entry->stateContext.scriptOwner);
            ClearNpcAreaRouteForEntry(entry);
            ClearNpcDeferredBranches(entry);
            InitializeNpcInterpreterState(entry, index);
        } else {
            entry->node = 0;
            entry->motionWork = 0;
            entry->timedMotionHandle = 0;
            entry->flags &= 0xdfffffe8U;
            entry->streamRouteChangeKind = 0;
            entry->previous = entry - 1;
            entry->next = entry + 1;
            entry->loadHumanResult = 8;
            entry->localPosition = entry->worldPosition;
            entry->localPosition.x -= npcRouteOrigin.x;
            entry->localPosition.z -= npcRouteOrigin.z;
            if (entry->flags & 0xa000U)
                entry->localPosition.y = entry->forcedY;
            else
                FindNpcIntersectionInScene(entry->localPosition, entry->packedMoveFlags);
            entry->worldPosition = entry->localPosition;
            entry->worldPosition.x += npcRouteOrigin.x;
            entry->worldPosition.z += npcRouteOrigin.z;
            if (entry->alternatePosition.x != 0.0f || entry->alternatePosition.z != 0.0f) {
                entry->alternatePosition = entry->alternateWorldPosition;
                entry->alternatePosition.x -= npcRouteOrigin.x;
                entry->alternatePosition.z -= npcRouteOrigin.z;
                if (std::fabs(entry->alternatePosition.y) < 0.00000011920928955078125f) {
                    FindNpcIntersectionInScene(entry->alternatePosition, entry->packedMoveFlags);
                    if (entry->flags & 0xa000U)
                        entry->alternatePosition.y = entry->forcedY;
                    entry->alternateWorldPosition = entry->alternatePosition;
                    entry->alternateWorldPosition.x += npcRouteOrigin.x;
                    entry->alternateWorldPosition.z += npcRouteOrigin.z;
                }
            }
            RebuildNpcRouteContextAfterReload(&entry->stateContext, oldFlags, offset);
            if (static_cast<std::int8_t>(entry->liveState) == 9) {
                if (GetNpcAreaPointRouteState(entry->currentAreaPoint)->flags & 8U)
                    npcSlotFlags[entry->poolIndex] |= 4U;
            } else if (!(entry->flags & 0x800U))
                PlaceNpcOutsideDoor(entry);
            if (static_cast<std::int8_t>(entry->liveState) != 9)
                npcSlotFlags[entry->poolIndex] &= ~4U;
        }
        if ((entry->scriptRecord[0x0b] & 8U) && npcEndOfDayWeatherForecast >= 0.15f) {
            entry->flags |= 0x100000U;
            *reinterpret_cast<float *>(reinterpret_cast<std::byte *>(entry) + 0x36c) = -10000.0f;
        } else if (entry->flags & 0x100000U) {
            entry->flags &= ~0x100000U;
            *reinterpret_cast<float *>(reinterpret_cast<std::byte *>(entry) + 0x36c) = 0.0f;
        }
        YieldNpcLoadingPresentation();
    }
    reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage)->previous = 0;
    (reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage) + npcRouteEntryCount - 1)->next = 0;
}

#endif

} // namespace s2
} // namespace shenmue
