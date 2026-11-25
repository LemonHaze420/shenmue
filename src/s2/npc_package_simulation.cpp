#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/npc_data_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/qron_dc.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include <cmath>
#include <cfloat>
#include <cstring>
#else
#include "s2/npc_package.hpp"

#include "s2/archive_data.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/fight_stage.hpp"
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
std::int64_t npcGetCurrentTimeSimState();
std::int64_t ResetCurrentTimeSimState();
std::int64_t npcSetCurrentTimeSimStateEnabled();
std::int64_t npcAdvanceTime(std::int32_t hour, std::int32_t minute);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace npc_package_detail {

// The loading presentation yield native inlines in its long NPC rebuild loops: while the loading screen draws, a loop that
// has run past 15000 x 12.5 timer units since the last present draws the loading modes, flushes and profiles a frame.
void YieldNpcLoadingPresentation() {
    npcSimDeltaTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    if (!IsLoadingDrawActive())
        return;
    const std::uint32_t elapsed = npcPresentedTime - npcSimDeltaTime;
    if (static_cast<double>(static_cast<float>(static_cast<std::int64_t>(elapsed))) / 12.5 <= 15000.0)
        return;
    DrawLoadingModes();
    FlushQueuedStripType3Batches();
    (void)ProfileFramePerformance();
    npcPresentedTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
}

void SelectNpcStreamEntries() {
    for (NpcRouteLiveEntry *entry = npcCrowdBookmark; entry; entry = entry->next) {
        if (!(entry->flags & 0x200U) && (entry->flags & 0x1aU) == 2U)
            RequestNpcStreamOut(*entry, 0);
    }
    NpcRouteLiveEntry *replacement = nullptr;
    NpcRouteLiveEntry *retire = nullptr;
    for (NpcRouteLiveEntry *entry = npcCrowdHead; entry && entry != npcCrowdBookmark; entry = entry->next) {
        const std::uint32_t flags = entry->flags;
        if (flags & 0x200U)
            continue;
        if (replacement) {
            if (flags & 2U)
                retire = entry;
        } else if (!(flags & 2U) && entry->streamSortKey < 20.0f && !(flags & 0x2000000U) && ((flags & 0x20000000U) || entry->loadHumanResult < 0))
            replacement = entry;
        if ((flags & 0x3aU) == 0x22U)
            RequestNpcStreamOut(*entry, 0);
    }
    if (replacement && retire) {
        if (replacement->flags & 0x20000000U)
            replacement->flags &= ~0x20000000U;
        else
            RequestNpcStreamOut(*retire, 1);
    }
}

void TickNpcRouteEntry(NpcRouteLiveEntry &entry, std::int32_t currentTime) {
    if (entry.flags & 8U) {
        if (entry.stateContext.routeFlags & 0x10U) {
            if (npcSlotFlags[entry.poolIndex])
                entry.stateContext.routeFlags &= ~0x10000U;
            else
                entry.stateContext.routeFlags |= 0x10000U;
            TickNpcRouteStateContext(&entry.stateContext);
            if (!(entry.stateContext.routeFlags & 0x20U))
                entry.controlFlags &= ~2U;
            if (static_cast<std::size_t>(entry.poolIndex) >= npcAuthoredEntryCount) {
                entry.node = ctrlGetScenarioChara(entry.name);
                if (entry.node) {
                    entry.motionWork =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(entry.node);
                    npcSlotFlags[entry.poolIndex] |= 1U;
                } else
                    npcSlotFlags[entry.poolIndex] &= ~1U;
            }
        }
        if ((entry.flags & 0x80U) && entry.liveState == 2)
            TickNpcSpecialMotion(entry);
        UpdateNpcMotionAttachments(entry);
        return;
    }
    if (npcCurrentTimeSimState)
        return;
    if (!(entry.flags & 0x10U))
        InterpretNpcSchedule(entry, currentTime);
    if (entry.liveState == 1) {
        if (npcSlotFlags[entry.poolIndex])
            entry.stateContext.routeFlags &= ~0x10000U;
        else
            entry.stateContext.routeFlags |= 0x10000U;
        TickNpcAreaRoute(&entry, currentTime);
    } else if (entry.liveState == 2)
        TickNpcSpecialMotion(entry);
    else if (entry.liveState == 3 && (entry.flags & 0x400U)) {
        auto *const slot = static_cast<NpcTimedMotionSlot *>(entry.timedMotionHandle);
        if (slot && slot->character && TickTimedNodeMotion(slot->animation))
            InitializeTimedNodeMotion(slot->animation, static_cast<MT7Model *>(slot->character->model), slot->motion);
    } else if (entry.liveState == 7)
        TickNpcMotionClip(entry);
    else if (entry.liveState == 8) {
        TickNpcAreaRouteState(entry);
        if (entry.routeMotionState == 23) {
            entry.scheduleProgress = 1;
            entry.liveState = 0;
            npcSlotFlags[entry.poolIndex] &= ~4U;
        }
    } else if (entry.liveState == 10 && entry.node) {
        if (currentTime % 60 == 0)
            mtwkRequestMotion(entry.motionWork, entry.recordBase[4] == std::byte{} ? 0x8032 : 0x8063, MotionRegion::RightArm);
        else if (mtwkSetRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody))
            mtwkRequestRegionDetach(entry.motionWork, MotionRegion::LowerBody);
    } else if (entry.liveState == 11 && entry.node && mtwkSetRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody)) {
        entry.currentMotionId = SelectNpcIdleMotion(entry);
        mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
    }
    if ((entry.stateContext.routeFlags & 0x400U) && !(entry.flags & 7U)) {
        if (NpcRouteConditionsChanged(entry)) {
            if (entry.stateContext.otherName.name)
                ReleaseDoor(static_cast<std::int32_t>(entry.stateContext.otherName.name), entry.stateContext.scriptOwner);
            ResetNpcAreaRouteState(entry);
            InitializeNpcInterpreterState(entry, -1);
        }
        entry.stateContext.routeFlags &= ~0x400U;
    }
    UpdateNpcMotionAttachments(entry);
    UpdateNpcFrameCulling(entry);
}

void UpdateNpcStreamThrottle(bool forceForecast) {
    shenGetCurrentDateTime(npcCurrentDateTime);
    npcCurrentSeconds = static_cast<std::uint32_t>(ComputeSecondsSince1980(npcCurrentDateTime));
    if (forceForecast || npcFrameCount % 7200 == 0)
        npcEndOfDayWeatherForecast = UpdateWeatherAndTime(1439 - 60 * npcCurrentDateTime.hour - npcCurrentDateTime.minute);
    npcCrowdDensityScale = GetWeatherPressure();
    if (npcFrameCount % 120 == 0 && GetNpcAreaRouteStoreCount())
        npcHourWeatherForecast = UpdateWeatherAndTime(60);
}

// Slot updates are five rotating slices, followed by forward/select/reload list passes.
void TickNpcRouteLiveEntries() {
    PrepareNpcAreaRoutes();
    npcPlayerPresent = g_playerRuntime.character != nullptr;
    if (npcPlayerPresent) {
        const Vector3 previous = npcPlayerPosition;
        plymUpdatePlayerSearchLocation(&npcPlayerPosition);
        const float x = npcPlayerPosition.x - previous.x;
        const float y = npcPlayerPosition.y - previous.y;
        const float z = npcPlayerPosition.z - previous.z;
        npcPlayerDisplacement = std::sqrt(x * x + y * y + z * z);
        if (std::abs(npcPlayerDisplacement) < 0.00000011920929f)
            npcPlayerDisplacement = 0.0099999998f;
        if (MotionWork *const work =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(g_playerRuntime.character))
            npcPlayerCenter = work->runtime.clusterCenter;
        npcPlayerRadius = 0.34999999f;
        npcSchedule.floor = static_cast<std::int32_t>(GetQronNextStream());
        const float displacement = npcPlayerDisplacement >= 0.039999999f ? std::fmin(0.17f, npcPlayerDisplacement) : 0.0f;
        npcStreamThrottle = std::fmin(npcStreamThrottle + displacement, displacement * 100.0f);
        npcStreamThrottleWithBias = npcStreamThrottle + 4.5f;
    } else
        npcStreamThrottleWithBias = 4.5f;
    ++npcFrameCount;
    npcRouteBuildCount = 0;
    UpdateNpcStreamThrottle(false);
    const DateTime &dateTime = npcCurrentDateTime;
    const std::int32_t currentTime =
        2 * (static_cast<std::int32_t>(dateTime.second) + 60 * (static_cast<std::int32_t>(dateTime.minute) + 60 * static_cast<std::int32_t>(dateTime.hour)));
    Vector3 cameraPosition{};
    camCtrlGetNextCamPos(&cameraPosition);
    cameraPosition.y = 0.0f;
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        std::uint8_t &slots = npcSlotFlags[index];
        if (npcCrowdPhase == index * 5 / npcRouteEntries.size()) {
            TickNpcRouteEntry(entry, currentTime);
            RankNpcStreamEntry(entry, slots, cameraPosition);
        } else if (slots)
            TickNpcRouteEntry(entry, currentTime);
    }
    if (npcCrowdPhase == 5 || (npcCrowdPhase == 7 && npcNewLoadState)) {
        const bool reload = npcCrowdPhase == 7;
        std::int32_t skipped = 0;
        std::int32_t cost = 0;
        const std::int32_t target = AdjustedNpcStreamTarget();
        NpcRouteLiveEntry *entry = npcCrowdHead;
        if (reload)
            npcReloadPassStarted = true;
        while (entry && cost < target) {
            const std::int32_t candidate = MeasureNpcStreamCost(*entry, reload);
            if (!candidate && ++skipped == 20)
                break;
            entry = entry->next;
            cost += candidate;
        }
        if (!reload)
            npcCrowdBookmark = entry;
        if (reload)
            npcWaitBypassActive = false;
    } else if (npcCrowdPhase == 6)
        SelectNpcStreamEntries();
    npcCrowdPhase = static_cast<std::uint8_t>((npcCrowdPhase + 1) % 8);
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (!entry.node)
            continue;
        float radius;
        if (entry.stateContext.flags & 1U) {
            SetCharacterCollisionCapsuleMargin(entry.node, 0.34999999f);
            entry.streamCenter = entry.localPosition;
            radius = 0.34999999f;
        } else {
            entry.streamCenter = entry.motionWork->runtime.clusterCenter;
            radius = entry.motionWork->runtime.clusterRadius + 0.050000001f;
        }
        if (radius > entry.streamRadius)
            entry.streamRadius = radius;
    }
    std::int32_t environmentCount = 0;
    const VFMA_t *const game = vfmaGetTaskParam();
    for (NpcRouteLiveEntry *entry = npcCrowdHead; entry; entry = entry->next) {
        if (entry->flags & 4U)
            UpdateNpcStreamFade(*entry);
        if ((entry->flags & 0x80000002U) == 0x80000002U && CanRevealNpcStream(*entry)) {
            entry->node->renderFlags |= CRF_All;
            if ((entry->flags & 0x8000000U) || entry->streamCost != 1) {
                if (CharacterControlModel *const control = FindCharacterControlModel(entry->node))
                    control->flags |= CCMF_VoxelSweep;
            }
            entry->flags &= ~0x80000000U;
            modelSetCharaAlpha(entry->node, 0.0f);
            npcSetAlphaOnOff(entry->node, true);
        }
        if (entry->node)
            mtwkSetEnvironmentEnabled(
                entry->motionWork, (entry->flags & 8U) || (game && static_cast<std::uint32_t>(GetBtlStatus(entry->node)) <= 1U) || environmentCount++ < 5);
    }
    NpcRouteLiveEntry *entry = npcCrowdHead;
    while (entry) {
        if (!(entry->flags & 1U))
            entry = entry->next;
        else {
            RequestNpcStreamLoad(*entry);
            entry = (entry->flags & 1U) ? entry->next : npcCrowdHead;
        }
    }
}
} // namespace npc_package_detail

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

extern DateTime npcCurrentDateTime;

extern std::uint32_t npcCurrentSeconds, npcRouteBuildCount;

extern std::int32_t npcCrowdPhase, npcReloadPassStarted;

extern float npcStreamThrottle, npcStreamThrottleWithBias, npcHalfHourWeatherForecast;

extern Vector3 npcPlayerPosition;

void InterpretNpcScheduleEntry(NpcRouteLiveEntry *entry, std::int32_t currentTime);

void mtwkSetEnvironmentEnabled(MotionWork *work, std::uint32_t enabled);

void RankNpcStreamEntry(NpcRouteLiveEntry *entry);

extern std::int32_t npcScheduleSimulationActive, npcSimulationSinglePass, npcSimulationTime, npcSimulationStop;

std::int32_t npcCurrentTimeSimState = 0;

void ProcessNpcDeferredBranches(NpcRouteLiveEntry *entry, std::int32_t currentTime);

void PrimeNpcInterpreter(NpcRouteLiveEntry *entry);

std::int32_t PollNpcStateCallback(NpcRouteLiveEntry *entry);

std::int32_t ExecuteNpcScheduleOpcode(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t TickNpcRouteStateContext(NpcRouteStateContextPrefix *context);

std::int32_t TickNpcSpecialMotion(NpcRouteLiveEntry *entry);

void UpdateNpcMotionAttachments(NpcRouteLiveEntry *entry);

void BeginNpcAreaRoute(NpcRouteLiveEntry *entry, std::int32_t currentTime);

std::int32_t TickNpcAreaRouteState(NpcRouteLiveEntry *entry);

void UpdateNpcRouteQueryState(NpcRouteLiveEntry *entry, std::int32_t moving);

Character *ctrlGetRootNode();

bool CheckNpcSpecialMotionTarget(shen_name name, Character *character);

std::int32_t StopNpcFollowingMotion(shen_name name);

std::int32_t NpcRouteConditionsChanged(NpcRouteLiveEntry *entry);

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t npcGetCurrentTimeSimState();
std::int32_t npcSetCurrentTimeSimStateEnabled();
std::int32_t ResetCurrentTimeSimState();
void npcAdvanceTime(std::int32_t hour, std::int32_t minute);
void TickNpcTimedMotion(NpcRouteLiveEntry *entry);
std::int32_t UpdateNpcViewVisibility(NpcRouteLiveEntry *entry);
void InterpretNpcScheduleEntry(NpcRouteLiveEntry *entry, std::int32_t currentTime);
void SetNpcRouteScenePosition(NpcRouteStateContextPrefix *context, shen_name name, const Vector3 *position);
void RankNpcStreamEntry(NpcRouteLiveEntry *entry);
void TickNpcRouteSlices(std::int32_t currentTime);
#else
namespace npc_package_detail {

void YieldNpcLoadingPresentation();
void SelectNpcStreamEntries();
void TickNpcRouteEntry(NpcRouteLiveEntry &entry, std::int32_t currentTime);
void UpdateNpcStreamThrottle(bool forceForecast);
void TickNpcRouteLiveEntries();

} // namespace npc_package_detail

std::int64_t npcGetCurrentTimeSimState();
std::int64_t npcSetCurrentTimeSimStateEnabled();
std::int64_t npcResetDateTimeIfNeeded();
std::int64_t ResetCurrentTimeSimState();
std::int64_t npcAdvanceTime(std::int32_t hour, std::int32_t minute);
void BuildRouteTimeWindow(std::int32_t startTick, std::int32_t endTick, std::int32_t skipAuthoredSeed);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t npcGetCurrentTimeSimState() {
    return npcCurrentTimeSimState;
}

#else

std::int64_t npcGetCurrentTimeSimState() {
    return static_cast<std::uint32_t>(npcCurrentTimeSimState);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t npcSetCurrentTimeSimStateEnabled() {
    npcCurrentTimeSimState = 1;
    return 1;
}

#else

std::int64_t npcSetCurrentTimeSimStateEnabled() {
    npcCurrentTimeSimState = 1;
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int64_t npcResetDateTimeIfNeeded() {
    const std::int64_t result = npcGetCurrentTimeSimState();
    if (result == 1)
        return result;
    shenGetCurrentDateTime(npcPausedDateTime);
    return npcSetCurrentTimeSimStateEnabled();
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ResetCurrentTimeSimState() {
    npcCurrentTimeSimState = 0;
    return 0;
}

#else

std::int64_t ResetCurrentTimeSimState() {
    npcCurrentTimeSimState = 0;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void TickNpcTimedMotion(NpcRouteLiveEntry *entry) {
    if (entry->timedMotionHandle && entry->timedMotionHandle->character && TickTimedNodeMotion(&entry->timedMotionHandle->animation) == 1) {
        NpcTimedMotionSlotPrefix *const slot = entry->timedMotionHandle;
        InitializeTimedNodeMotion(
            &slot->animation, slot->character->model, reinterpret_cast<const TimedNodeMotionHeader *>(MOTN_GetDataForAnimId(slot->motion)));
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void npcAdvanceTime(std::int32_t hour, std::int32_t minute) {
    const std::uint32_t end = (static_cast<std::uint32_t>(hour) * 3600U + static_cast<std::uint32_t>(minute) * 60U) * 2U;
    SimulateNpcScheduleTime(0, static_cast<std::int32_t>(end), 0);
    ResetCurrentTimeSimState();
}

#else

std::int64_t npcAdvanceTime(std::int32_t hour, std::int32_t minute) {
    const std::int64_t result = npcGetCurrentTimeSimState();
    if (!result)
        return result;
    if (hour < 0 || minute < 0) {
        DateTime now;
        shenGetCurrentDateTime(now);
        hour = now.hour;
        minute = now.minute;
    }
    const std::int32_t endTick = 120 * (minute + 60 * hour);
    const std::int32_t startTick = 120 * (npcPausedDateTime.minute + 60 * npcPausedDateTime.hour);
    ActivateNpcAreaRoutes(false);
    npcIsTimeAdvancing = 1;
    npcLatestTime = endTick;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.flags & 0x10000000U)
            continue;
        npcTimeAdvanceBreak = 0;
        for (std::int32_t time = startTick; time < endTick; ++time) {
            npcRouteBuildCount = 0;
            TickNpcRouteEntry(entry, time);
            if (entry.routeMotionState != 23 && ((entry.commandFlags & 0x80U) || entry.field2e8 || entry.routeMotionState == 4))
                break;
            if (npcTimeAdvanceBreak == 1)
                break;
        }
    }
    npcIsTimeAdvancing = 0;
    npcTimeAdvanceBreak = 0;
    ActivateNpcAreaRoutes(true);
    return ResetCurrentTimeSimState();
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void BuildRouteTimeWindow(std::int32_t startTick, std::int32_t endTick, std::int32_t skipAuthoredSeed) {
    ActivateNpcAreaRoutes(false);
    npcIsTimeAdvancing = 1;
    npcLatestTime = endTick;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if ((skipAuthoredSeed && (static_cast<std::uint8_t>(entry.recordBase[0x0b]) & 1U)) || (entry.flags & 0x10000000U))
            continue;
        npcTimeAdvanceBreak = 0;
        for (std::int32_t time = startTick; time < endTick; ++time) {
            npcRouteBuildCount = 0;
            TickNpcRouteEntry(entry, time);
            if ((entry.routeMotionState != 23 && ((entry.commandFlags & 0x80U) || entry.field2e8 || entry.routeMotionState == 4)) || npcTimeAdvanceBreak == 1)
                break;
            if (skipAuthoredSeed && time % 300 == 0)
                YieldNpcLoadingPresentation();
        }
    }
    npcIsTimeAdvancing = 0;
    npcTimeAdvanceBreak = 0;
    ActivateNpcAreaRoutes(true);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void TickNpcRouteSlices(std::int32_t currentTime) {
    NpcRouteLiveEntry *entry = reinterpret_cast<NpcRouteLiveEntry *>(npcEntryStorage);
    for (std::int32_t index = 0; index < npcRouteEntryCount; ++index, ++entry) {
        if (entry->flags & 0x100000U)
            continue;
        const std::int32_t slice = index * 5 / npcRouteEntryCount;
        if (entry->flags & 0x4000000U)
            npcSlotFlags[index] |= 2U;
        if (npcSlotFlags[index] || npcCrowdPhase == slice)
            InterpretNpcScheduleEntry(entry, currentTime);
        if (npcCrowdPhase == slice)
            RankNpcStreamEntry(entry);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void InterpretNpcScheduleEntry(NpcRouteLiveEntry *entry, std::int32_t currentTime) {
    Vector3 probe;
    Vector3 point;
    Vector3 position;
    Vector3i rotation;
    if (entry->flags & 8U) {
        if ((&entry->stateContext)->routeFlags & 0x10U) {
            MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            entry->node = scenFindChara(control->scenarioTask, entry->name);
            if (entry->node) {
                entry->motionWork = charFindMotionwork(entry->node);
                npcSlotFlags[entry->poolIndex] |= 1U;
            } else
                npcSlotFlags[entry->poolIndex] &= ~1U;
            if (!npcSlotFlags[entry->poolIndex])
                (&entry->stateContext)->routeFlags |= 0x10000U;
            else
                (&entry->stateContext)->routeFlags &= ~0x10000U;
            TickNpcRouteStateContext(&entry->stateContext);
        }
        if ((entry->flags & 0x80U) && static_cast<std::int8_t>(entry->liveState) == 2)
            TickNpcSpecialMotion(entry);
        UpdateNpcMotionAttachments(entry);
        return;
    }
    if (npcGetCurrentTimeSimState())
        return;
    entry->previousPosition = entry->localPosition;
    if (!(entry->flags & 0x10U)) {
        if (entry->nextTimedCommand && currentTime >= entry->nextTimedCommandTime && (entry->flags & 0x20000U)) {
            entry->currentCommand = entry->nextTimedCommand;
            entry->scheduleProgress = 1;
            entry->waitDuration = 0;
        }
        ProcessNpcDeferredBranches(entry, currentTime);
        switch (static_cast<std::int8_t>(entry->scheduleProgress)) {
        case 0:
            PrimeNpcInterpreter(entry);
        case 1:
            if (PollNpcStateCallback(entry) != 1) {
                std::int32_t result;
                do {
                    result = ExecuteNpcScheduleOpcode(entry, currentTime);
                    if (result == 2) {
                        result = 0;
                        npcSimulationStop = 1;
                    }
                    ProcessNpcDeferredBranches(entry, currentTime);
                } while (result);
            }
            break;
        case 2:
            if (currentTime - entry->waitStartTime >= entry->waitDuration) {
                entry->waitDuration = 0;
                entry->scheduleProgress = 1;
            }
            break;
        case 3:
        case 4:
            break;
        }
    }
    switch (static_cast<std::int8_t>(entry->liveState)) {
    case 0:
        break;
    case 13:
        TickNpcSpecialMotion(entry);
        if (!CheckNpcSpecialMotionTarget(entry->name, ctrlGetRootNode()) || ((&entry->stateContext)->flags & 1U) == 0) {
            StopNpcFollowingMotion(entry->name);
            break;
        }
    case 1:
        if (!npcSlotFlags[entry->poolIndex])
            (&entry->stateContext)->routeFlags |= 0x10000U;
        else
            (&entry->stateContext)->routeFlags &= ~0x10000U;
        if (!TickNpcRouteStateContext(&entry->stateContext))
            break;
        if (entry->node)
            mtwkSetSpeed(entry->motionWork, entry->motionSpeed, LowerBody);
        entry->currentAreaPoint = entry->pendingAreaPoint;
        if (*reinterpret_cast<const std::uint32_t *>(entry->currentCommand) == 0x2aU) {
            if (reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[3])
                EV_SetPlayerFlag(PlayerFlagWriteMode_Internal, reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1],
                    reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2]);
            else
                stepUpdateFlagUtil(
                    reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[1], reinterpret_cast<const std::uint32_t *>(entry->currentCommand)[2]);
            entry->currentCommand += 16;
        }
        if (GetNpcAreaPointRouteState(entry->currentAreaPoint))
            BeginNpcAreaRoute(entry, currentTime);
        else {
            entry->scheduleProgress = 1;
            entry->liveState = 0;
        }
        UpdateNpcRouteQueryState(entry, 0);
        break;
    case 2:
        TickNpcSpecialMotion(entry);
        break;
    case 3:
        if (entry->flags & 0x400U)
            TickNpcTimedMotion(entry);
        break;
    case 7:
        probe = entry->localPosition;
        probe.y += 1.0f;
        if (!camCtrlIsInsideDepthScaledRect(probe, 1.0f)) {
            probe = entry->localPosition;
            Vector3i &savedRotation = reinterpret_cast<Vector3i &>(point);
            savedRotation.x = entry->rotation.x;
            savedRotation.y = entry->rotation.y;
            savedRotation.z = entry->rotation.z;
            entry->localPosition = entry->alternatePosition;
            entry->rotation.x = entry->alternateRotation.x;
            entry->rotation.y = entry->alternateRotation.y;
            entry->rotation.z = entry->alternateRotation.z;
            entry->alternatePosition = probe;
            entry->alternateRotation.x = savedRotation.x;
            entry->alternateRotation.y = savedRotation.y;
            entry->alternateRotation.z = savedRotation.z;
            if (entry->node) {
                SetCharacterWorldPosition(entry->node, &entry->localPosition);
                charaSetWorldRot(entry->node, entry->rotation);
            }
        }
        break;
    case 11:
        entry->liveState = 12;
        break;
    case 12: {
        if (!entry->node || !npcMotionClipFrames[static_cast<std::int8_t>(entry->retained324)] || mtwkSetRegionFlag(entry->motionWork, 0x44, 1, LowerBody))
            break;
        SampleNativeMotionRoot(
            npcMotionClipFallbackAnimations[static_cast<std::int8_t>(entry->retained324)], charGetMotionRigType(entry->node), entry->animationFrame, &point);
        GetCharacterWorldPosition(entry->node, &position);
        GetCharacterWorldRotation(entry->node, &rotation);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::SetViewMatrixFromEuler(rotation.x, rotation.y, rotation.z);
        Matrix4x4::TransformPoint(point, point);
        PopCurrentMatrix(1);
        position.x -= point.x;
        position.z -= point.z;
        SetCharacterWorldPosition(entry->node, &position);
        entry->liveState = 8;
    }
    case 8:
        if (!npcMotionClipFrames[static_cast<std::int8_t>(entry->retained324)])
            break;
        entry->animationFrame = ++entry->animationFrame >= static_cast<std::int32_t>(npcMotionClipFrameCounts[static_cast<std::int8_t>(entry->retained324)]) ?
            0 :
            entry->animationFrame;
        if (entry->node) {
            point = entry->localPosition;
            EV_SetCharDisplay(entry->node, CVM_MotionworkDisplayCheck, camCtrlIsInsideDepthScaledRect(point, GetCharacterDisplayRadius(entry->node)));
            Matrix4x4::TransformPoint(point, point);
            do {
                float *view = &entry->node->viewPosition.x;
                const float *source = &point.x;
                *view++ = *source++;
                *view++ = *source++;
                *view = *source;
            } while (0);
            const float distance = Vector3::CalcMagnitude(point);
            if (distance < 1.0e-5f)
                entry->node->drawDistance = 0.0f;
            else
                entry->node->drawDistance = distance;
            mtwkSetDrawOverrideData(entry->motionWork,
                npcMotionClipFrames[static_cast<std::int8_t>(entry->retained324)] +
                    npcMotionClipBoneCounts[static_cast<std::int8_t>(entry->retained324)] * entry->animationFrame);
        }
        break;
    case 9:
        TickNpcAreaRouteState(entry);
        if (entry->routeMotionState == 23) {
            entry->scheduleProgress = 1;
            entry->liveState = 0;
            npcSlotFlags[entry->poolIndex] &= ~4U;
        }
        break;
    case 15:
        if (entry->node && mtwkSetRegionFlag(entry->motionWork, 0x41, 1, LowerBody))
            mtwkRequestMotion(entry->motionWork,
                entry->currentMotionId = SelectNpcProfileMotion(entry->node, static_cast<std::int8_t>(entry->scriptRecord[5]),
                    static_cast<std::int8_t>(entry->scriptRecord[4]), static_cast<std::int8_t>(entry->scriptRecord[8])),
                LowerBody);
        break;
    case 14:
        if (entry->node) {
            if (currentTime == (currentTime / 60) * 60)
                mtwkRequestMotion(entry->motionWork, entry->scriptRecord[4] ? 0x807b : 0x8039, RightArm);
            else if (mtwkSetRegionFlag(entry->motionWork, 0x41, 1, LowerBody))
                mtwkRequestRegionDetach(entry->motionWork, LowerBody);
        }
        break;
    }
    if (entry->attachedCharacter && entry->node) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        Character *const target = scenFindChara(control->scenarioTask, entry->attachedCharacter);
        if (target) {
            GetCharacterWorldPosition(target, &position);
            GetCharacterWorldRotation(target, &rotation);
            matrix_stack::PushUnitMatrix();
            Matrix4x4::preTranslate(position.x, position.y, position.z);
            Matrix4x4::SetViewMatrixFromEuler(rotation.x, rotation.y, rotation.z);
            Matrix4x4::preTranslate(entry->attachmentOffset.x, entry->attachmentOffset.y, entry->attachmentOffset.z);
            matrix_stack::GetTranslation(&position);
            PopCurrentMatrix(1);
            SetCharacterWorldPosition(entry->node, &position);
            rotation.y += entry->attachmentYaw;
            entry->localPosition = position;
            charaSetWorldRot(entry->node, rotation);
        }
    }
    if (((&entry->stateContext)->routeFlags & 0x400U) && !(entry->flags & 7U)) {
        if (NpcRouteConditionsChanged(entry) == 1) {
            if (entry->stateContext.otherName)
                ReleaseDoor(entry->stateContext.otherName, entry->stateContext.scriptOwner);
            ClearNpcAreaRouteForEntry(entry);
            InitializeNpcInterpreterState(entry, -1);
        }
        (&entry->stateContext)->routeFlags &= ~0x400U;
    }
    UpdateNpcMotionAttachments(entry);
    UpdateNpcViewVisibility(entry);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetNpcRouteScenePosition(NpcRouteStateContextPrefix *context, shen_name name, const Vector3 *position) {
    if (!context) {
        NpcRouteLiveEntry *const entry = FindNpcRouteEntry(name);
        if (!entry)
            return;
        context = &entry->stateContext;
    }
    context->scenePosition = *position;
    context->localPosition = context->scenePosition;
    context->localPosition.x += npcRouteOrigin.x;
    context->localPosition.z += npcRouteOrigin.z;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RankNpcStreamEntry(NpcRouteLiveEntry *entry) {
    *reinterpret_cast<float *>(entry->retained364) = entry->streamSortKey;
    Vector3 point;
    if (std::fabs(entry->localPosition.y) < FLT_EPSILON) {
        camCtrlGetNextCamPos(&point);
        am2Vector3SubtractInPlace(&point, &entry->localPosition);
        point.y = 0.0f;
    } else
        Matrix4x4::TransformPoint(entry->localPosition, point);
    point.x += point.x;
    point.z += 4.5f;
    entry->streamSortKey = Vector3::CalcMagnitude(point);
    point.z += npcStreamThrottleWithBias;
    entry->streamSortKey += Vector3::CalcMagnitude(point);
    if (entry->streamSortKey < 50.0f)
        npcSlotFlags[entry->poolIndex] |= 2U;
    else
        npcSlotFlags[entry->poolIndex] &= ~2U;
    if (*reinterpret_cast<float *>(entry->retained36c) > FLT_EPSILON) {
        entry->streamSortKey -= *reinterpret_cast<float *>(entry->retained36c);
        if (entry->streamSortKey < 0.0f)
            entry->streamSortKey = 0.0f;
    } else if (*reinterpret_cast<float *>(entry->retained36c) < -FLT_EPSILON)
        entry->streamSortKey -= *reinterpret_cast<float *>(entry->retained36c);
    if (entry->flags & 0x10000U) {
        entry->streamSortKey -= 2.0f;
        if (entry->streamSortKey < 0.0f)
            entry->streamSortKey = 0.0f;
    }
    if (entry->flags & 0x200U)
        entry->streamSortKey = 0.0f;
    else
        entry->streamSortKey += 0.0099999998f;
    NpcRouteLiveEntry *insertion = entry;
    if (!entry->previous) {
        while (insertion->next && insertion->next->streamSortKey < entry->streamSortKey)
            insertion = insertion->next;
    } else if (entry->streamSortKey < entry->previous->streamSortKey) {
        while (insertion->previous) {
            insertion = insertion->previous;
            if (!(insertion->streamSortKey > entry->streamSortKey))
                break;
        }
    } else if (!entry->next) {
        while (insertion->previous) {
            insertion = insertion->previous;
            if (!(insertion->streamSortKey > entry->streamSortKey))
                break;
        }
    } else if (entry->next->streamSortKey < entry->streamSortKey) {
        while (insertion->next && insertion->next->streamSortKey < entry->streamSortKey)
            insertion = insertion->next;
    }
    if (insertion == entry || insertion->next == entry)
        return;
    if (entry->previous)
        entry->previous->next = entry->next;
    else
        npcCrowdHead = entry->next;
    if (entry->next)
        entry->next->previous = entry->previous;
    entry->next = insertion->next;
    entry->previous = insertion;
    if (entry->next)
        entry->next->previous = entry;
    if (entry->previous)
        entry->previous->next = entry;
}

#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" void __attribute__((renesas)) shenmue_s2_event_npcAdvanceTime(std::int32_t hour, std::int32_t minute) {
    shenmue::s2::npcAdvanceTime(hour, minute);
}
#else
#endif
