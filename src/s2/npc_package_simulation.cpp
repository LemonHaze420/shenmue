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
                entry.node = FindCharacterByScenarioName(entry.name);
                if (entry.node) {
                    entry.motionWork = FindMotionWork(entry.node);
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
        else if (SetMotionRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody))
            RequestMotionRegionDetach(entry.motionWork, MotionRegion::LowerBody);
    } else if (entry.liveState == 11 && entry.node && SetMotionRegionFlag(entry.motionWork, 65, 1, MotionRegion::LowerBody)) {
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
    GetCurrentDateTime(npcCurrentDateTime);
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
        if (MotionWork *const work = FindMotionWork(g_playerRuntime.character))
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
    for (NpcRouteLiveEntry *entry = npcCrowdHead; entry; entry = entry->next) {
        if (entry->flags & 4U)
            UpdateNpcStreamFade(*entry);
        if ((entry->flags & 0x80000002U) == 0x80000002U && CanRevealNpcStream(*entry)) {
            entry->node->renderFlags |= 0x3fU;
            if ((entry->flags & 0x8000000U) || entry->streamCost != 1) {
                if (CharacterControlModel *const control = FindCharacterControlModel(entry->node))
                    control->flags |= CharacterControlVoxelSweep;
            }
            entry->flags &= ~0x80000000U;
            modelSetCharaAlpha(entry->node, 0.0f);
            npcSetAlphaOnOff(entry->node, true);
        }
        if (entry->node)
            SetMotionEnvironmentEnabled(entry->motionWork, (entry->flags & 8U) || environmentCount++ < 5);
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

std::int64_t npcGetCurrentTimeSimState() {
    return static_cast<std::uint32_t>(npcCurrentTimeSimState);
}

std::int64_t ResetCurrentTimeSimState() {
    npcCurrentTimeSimState = 0;
    return 0;
}

std::int64_t npcSetCurrentTimeSimStateEnabled() {
    npcCurrentTimeSimState = 1;
    return 1;
}

std::int64_t npcResetDateTimeIfNeeded() {
    const std::int64_t result = npcGetCurrentTimeSimState();
    if (result == 1)
        return result;
    GetCurrentDateTime(npcPausedDateTime);
    return npcSetCurrentTimeSimStateEnabled();
}

std::int64_t npcAdvanceTime(std::int32_t hour, std::int32_t minute) {
    const std::int64_t result = npcGetCurrentTimeSimState();
    if (!result)
        return result;
    if (hour < 0 || minute < 0) {
        DateTime now;
        GetCurrentDateTime(now);
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
} // namespace shenmue::s2
