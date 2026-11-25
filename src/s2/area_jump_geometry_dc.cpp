#include "s2/area_jump_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_event_transition_dc.hpp"
#include <cstring>
#include "s2/player_config_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/area_jump_data_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#define INV_NOEXCEPT
#define INV_NULL 0
#define INV_INIT =
#define INV_ZERO
#include "platform/dc/sdk.hpp"
#include "s2/camera_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/game_save_area.hpp"
#include "s2/input_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/texture_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/story_state.hpp"
#include <cstdarg>
#include <cstdio>
#endif
#include "s2/door_control_dc.hpp"

namespace shenmue {
namespace s2 {

shen_name drGetCurrentAuthorId();

void RestoreSavedAreaJumpLocation(const SavedAreaJumpLocation *location, const std::byte *placement);
void ActivateSavedAreaJumpLocation();
void SetTemporaryAreaJumpAuthor(shen_name author);
std::int32_t EV_CanDoAreaJump();
std::int32_t IsAreaJumpEnabled();
shen_name drGetCurrentAuthorId();
SavedAreaJumpLocation g_areaJumpPersistentLocation;
std::byte g_areaJumpPersistentPlacement[0x20];
shen_name g_areaJumpPersistentSceneAreaId;
std::int16_t g_areaJumpPersistentSceneEntryId;
shen_name g_areaJumpPersistentAuthorId;
std::uint16_t g_areaJumpWarpEnabled = 0;

std::int32_t EV_CanDoAreaJump();
std::int32_t g_areaJumpState, g_areaJumpSelectedDestination = 0;
const CtrlAreaJumpEntry *g_activeAreaJumpEntry = 0;
const AreaJumpScheduleEntry *g_currentAreaJumpSchedule = 0, *g_initialAreaJumpSchedule = 0;
const std::int8_t *g_alternateAreaJumpSchedule = 0;
Vector3 g_areaJumpCurrentPosition, g_areaJumpPreviousPosition;
std::int32_t SetupControlSpeakAndUI_Internal(std::int16_t *choices, std::int32_t timeout, std::uint16_t defaultChoice, std::uint16_t mode, std::int32_t style);
static const AreaJumpScheduleEntry pendingAreaJumpSchedule[] = {{{7, 23, 25, 20}, 0}};

// @verified - d
void CaptureCurrentAreaJumpState() {
    if (EV_CanDoAreaJump()) {
        CaptureCurrentPlayerState(&g_areaJumpPersistentLocation, g_areaJumpPersistentPlacement);
        g_areaJumpPersistentSceneAreaId = g_currentMap;
        g_areaJumpPersistentSceneEntryId = static_cast<std::int16_t>(g_entryPoint);
        g_areaJumpWarpEnabled = 1;
        g_areaJumpPersistentAuthorId = drGetCurrentAuthorId();
    } else {
        g_areaJumpWarpEnabled = 0;
    }
}

// @verified - d
std::int32_t GetAreaJumpSpeakIconFlag(std::int32_t index) {
    const AreaJumpDestinationEntry &destination = areaJumpDestinations[index];
    if (destination.destinationEntryId >= 0 && destination.destination.specialMode >= 1U && destination.destination.specialMode <= 2U && !g_areaJumpWarpEnabled)
        return -1;
    return destination.speakIconFlag;
}

// @verified - d
bool FindMapEventEntryLocation(shen_name areaId, std::int32_t entryId, Vector3 *position) {
    for (const AreaTableEntry *area = g_areaTableEntries; area->area != UINT32_MAX; ++area) {
        if (area->area != areaId)
            continue;
        for (const MapEventTransition *transition = area->transitions; transition->eventId != -1; ++transition) {
            if (transition->eventId == entryId) {
                *position = transition->position;
                return true;
            }
        }
    }
    return false;
}

// @verified - d
void AdvanceTimeForDistance(const Vector3 *position) {
    const float x = g_areaJumpCurrentPosition.x - position->x;
    const float z = g_areaJumpCurrentPosition.z - position->z;
    float distance = ((x) * (x) + (z * z));
    asm("fsqrt %0" : "+f"(distance));
    const float minutes = distance * 0.22222222f;
    std::int32_t integer;
    asm("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(integer) : "f"(minutes) : "fpul");
    shenAdjustCurrentTimeByMinutes(static_cast<std::int16_t>(integer));
}

// @verified - d
std::int32_t CheckLineSegmentXZIntersection(float firstX, float firstZ, float secondX, float secondZ, float targetY) {
    const float height = targetY - g_areaJumpCurrentPosition.y;
    const float segmentX = secondX - firstX;
    const float segmentZ = secondZ - firstZ;
    const float movementX = g_areaJumpPreviousPosition.x - g_areaJumpCurrentPosition.x;
    const float movementZ = g_areaJumpPreviousPosition.z - g_areaJumpCurrentPosition.z;
    const float offsetX = firstX - g_areaJumpCurrentPosition.x;
    const float offsetZ = firstZ - g_areaJumpCurrentPosition.z;
    float fraction = segmentX * movementZ - segmentZ * movementX;
    if (0.0f > height) {
        if (1.0f > -height)
            goto checkDivisor;
    } else if (1.0f > height) {
        goto checkDivisor;
    }
    goto invalid;
checkDivisor:
    if (!(fraction > 0.00001f) && !(-0.00001f > fraction))
        goto invalid;
    fraction = (movementX * offsetZ - movementZ * offsetX) / fraction;
    if (!(1.00001f > fraction) || !(fraction > -0.00001f))
        goto done;
    if (movementX > 0.00001f || -0.00001f > movementX)
        fraction = ((fraction) * (segmentX) + (offsetX)) / movementX;
    else if (movementZ > 0.00001f || -0.00001f > movementZ)
        fraction = ((fraction) * (segmentZ) + (offsetZ)) / movementZ;
    else
        goto invalid;
    goto done;
invalid:
    fraction = -1.0f;
done:
    std::int32_t result = 0;
    if (1.00001f > fraction && fraction > -0.00001f)
        result = 1;
    return result;
}

void CtrlAreaJump_Callback(void *) {
    std::uint32_t icon;
    std::int16_t choices[4];
    Vector3 position;
    g_areaJumpPreviousPosition = g_areaJumpCurrentPosition;
    plymUpdatePlayerSearchLocation(&g_areaJumpCurrentPosition);
    switch (g_areaJumpState) {
    case 0:
        g_activeAreaJumpEntry = areaJumps;
        while (g_activeAreaJumpEntry->sourceAreaId && g_activeAreaJumpEntry->sourceAreaId != g_currentMap)
            ++g_activeAreaJumpEntry;
        if (g_activeAreaJumpEntry->sourceAreaId)
            g_areaJumpState = 1;
        else
            g_areaJumpState = 9;
        if (g_currentMap == AREA_QR00 && g_entryPoint != 99)
            g_areaJumpState = 9;
        break;
    case 1: {
        if (!IsAreaJumpEnabled() || !plymIsSearchable() || plymIsSearchBlocked())
            break;
        const CtrlAreaJumpEntry *const entry = g_activeAreaJumpEntry;
        if (entry->requiredAuthorId) {
            if (entry->requiredAuthorId == drGetCurrentAuthorId())
                g_areaJumpState = 2;
        } else if (CheckLineSegmentXZIntersection(g_activeAreaJumpEntry->firstX, g_activeAreaJumpEntry->firstZ, g_activeAreaJumpEntry->secondX,
                       g_activeAreaJumpEntry->secondZ, g_activeAreaJumpEntry->targetY)) {
            g_areaJumpState = 2;
        }
        break;
    }
    case 2: {
        if (!IsAreaJumpEnabled()) {
            g_areaJumpState = 1;
            break;
        }
        if (!plymIsSearchable() || plymIsSearchBlocked())
            break;
        const CtrlAreaJumpEntry *const entry = g_activeAreaJumpEntry;
        if (entry->requiredAuthorId) {
            if (entry->requiredAuthorId != drGetCurrentAuthorId())
                g_areaJumpState = 3;
        } else if (CheckLineSegmentXZIntersection(g_activeAreaJumpEntry->endFirstX, g_activeAreaJumpEntry->endFirstZ, g_activeAreaJumpEntry->endSecondX,
                       g_activeAreaJumpEntry->endSecondZ, g_activeAreaJumpEntry->targetY)) {
            g_areaJumpState = 3;
        }
        break;
    }
    case 3:
        g_initialAreaJumpSchedule = g_activeAreaJumpEntry->scheduleData;

    case 4:
        g_currentAreaJumpSchedule = g_initialAreaJumpSchedule;
        while (g_currentAreaJumpSchedule->requiredShenmueFlag > 0 && !EV_GetShenmueFlag(g_currentAreaJumpSchedule->requiredShenmueFlag))
            ++g_currentAreaJumpSchedule;
        if (g_currentAreaJumpSchedule->requiredShenmueFlag < 0) {
            g_areaJumpState = 8;
            break;
        }
        choices[0] = static_cast<std::int16_t>(GetAreaJumpSpeakIconFlag(g_currentAreaJumpSchedule->destinationIndices[0]));
        choices[1] = static_cast<std::int16_t>(GetAreaJumpSpeakIconFlag(g_currentAreaJumpSchedule->destinationIndices[1]));
        choices[2] = static_cast<std::int16_t>(GetAreaJumpSpeakIconFlag(g_currentAreaJumpSchedule->destinationIndices[2]));
        choices[3] = static_cast<std::int16_t>(GetAreaJumpSpeakIconFlag(g_currentAreaJumpSchedule->destinationIndices[3]));
        if (choices[0] < 0 && choices[1] < 0 && choices[2] < 0 && choices[3] < 0) {
            g_areaJumpState = 8;
            break;
        }
        if (SetupControlSpeakAndUI_Internal(choices, 10800, 0xffff, 2, 4))
            g_areaJumpState = 5;
        else
            g_areaJumpState = 8;
        break;
    case 5: {
        if (!IsCtrlSpeakIconInactive())
            break;
        icon = shenGetCtrlSpeakIconFlag();
        g_areaJumpSelectedDestination = -1;
        if (icon == static_cast<std::uint32_t>(areaJumpDestinations[g_currentAreaJumpSchedule->destinationIndices[0]].speakIconFlag))
            g_areaJumpSelectedDestination = g_currentAreaJumpSchedule->destinationIndices[0];
        else if (icon == static_cast<std::uint32_t>(areaJumpDestinations[g_currentAreaJumpSchedule->destinationIndices[1]].speakIconFlag))
            g_areaJumpSelectedDestination = g_currentAreaJumpSchedule->destinationIndices[1];
        else if (icon == static_cast<std::uint32_t>(areaJumpDestinations[g_currentAreaJumpSchedule->destinationIndices[2]].speakIconFlag))
            g_areaJumpSelectedDestination = g_currentAreaJumpSchedule->destinationIndices[2];
        else if (icon == static_cast<std::uint32_t>(areaJumpDestinations[g_currentAreaJumpSchedule->destinationIndices[3]].speakIconFlag))
            g_areaJumpSelectedDestination = g_currentAreaJumpSchedule->destinationIndices[3];
        g_areaJumpState = g_areaJumpSelectedDestination > 0 ? 7 : 8;
        break;
    }
    case 6: {
        if (!IsCtrlSpeakIconInactive())
            break;
        icon = shenGetCtrlSpeakIconFlag();
        g_areaJumpSelectedDestination = -1;
        if (icon == static_cast<std::uint32_t>(areaJumpDestinations[g_alternateAreaJumpSchedule[0]].speakIconFlag))
            g_areaJumpSelectedDestination = g_alternateAreaJumpSchedule[0];
        else if (icon == static_cast<std::uint32_t>(areaJumpDestinations[g_alternateAreaJumpSchedule[1]].speakIconFlag))
            g_areaJumpSelectedDestination = g_alternateAreaJumpSchedule[1];
        else if (icon == static_cast<std::uint32_t>(areaJumpDestinations[g_alternateAreaJumpSchedule[2]].speakIconFlag))
            g_areaJumpSelectedDestination = g_alternateAreaJumpSchedule[2];
        else if (icon == static_cast<std::uint32_t>(areaJumpDestinations[g_alternateAreaJumpSchedule[3]].speakIconFlag))
            g_areaJumpSelectedDestination = g_alternateAreaJumpSchedule[3];
        g_areaJumpState = g_areaJumpSelectedDestination > 0 ? 7 : 4;
        break;
    }
    case 7:
        if (areaJumpDestinations[g_areaJumpSelectedDestination].destinationEntryId < 0) {
            g_alternateAreaJumpSchedule = areaJumpDestinations[g_areaJumpSelectedDestination].destination.candidateIndices;
            choices[0] = static_cast<std::int16_t>(GetAreaJumpSpeakIconFlag(g_alternateAreaJumpSchedule[0]));
            choices[1] = static_cast<std::int16_t>(GetAreaJumpSpeakIconFlag(g_alternateAreaJumpSchedule[1]));
            choices[2] = static_cast<std::int16_t>(GetAreaJumpSpeakIconFlag(g_alternateAreaJumpSchedule[2]));
            choices[3] = static_cast<std::int16_t>(GetAreaJumpSpeakIconFlag(g_alternateAreaJumpSchedule[3]));
            if (choices[0] < 0 && choices[1] < 0 && choices[2] < 0 && choices[3] < 0) {
                g_areaJumpState = 4;
                break;
            }
            if (SetupControlSpeakAndUI_Internal(choices, 10800, 0xffff, 2, 4))
                g_areaJumpState = 6;
            else
                g_areaJumpState = 4;
            break;
        }
        switch (areaJumpDestinations[g_areaJumpSelectedDestination].destination.specialMode) {
        case 0:
        case 1:
            g_areaJumpState = 8;
            return;
        case 2:
            scenSetTaskFlags(0);
            AdvanceTimeForDistance(&g_areaJumpPersistentLocation.position);
            shenGetCurrentDateTime(g_areaJumpPersistentLocation.capturedTime);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            SetDoorQueuedAuthor(g_areaJumpPersistentAuthorId);
#else
            SetTemporaryAreaJumpAuthor(g_areaJumpPersistentAuthorId);
#endif
            RestoreSavedAreaJumpLocation(&g_areaJumpPersistentLocation, g_areaJumpPersistentPlacement);
            ActivateSavedAreaJumpLocation();
            shenQueueChangeModuleAndScene(SystemModule_Game, g_actNumber, g_areaJumpPersistentSceneAreaId, g_areaJumpPersistentSceneEntryId);
            g_areaJumpState = 9;
            if (g_areaJumpWarpEnabled)
                g_areaJumpWarpEnabled = 0;
            break;
        default:
            scenSetTaskFlags(0);
            if (FindMapEventEntryLocation(areaJumpDestinations[g_areaJumpSelectedDestination].destination.areaId,
                    areaJumpDestinations[g_areaJumpSelectedDestination].destinationEntryId, &position))
                AdvanceTimeForDistance(&position);
            shenQueueChangeModuleAndScene(SystemModule_Game, g_actNumber, areaJumpDestinations[g_areaJumpSelectedDestination].destination.areaId,
                areaJumpDestinations[g_areaJumpSelectedDestination].destinationEntryId);
            g_areaJumpState = 9;
            if (g_areaJumpWarpEnabled)
                g_areaJumpWarpEnabled = 0;
            break;
        }
        break;
    case 8:
        if (!g_activeAreaJumpEntry->sourceAreaId || g_currentMap == AREA_QR00)
            g_areaJumpState = 9;
        else
            g_areaJumpState = 1;
        break;
    }
}

// @verified - d
void initCtrlAreaJump() {
    HLib::EnqueueTaskWithoutParameter(CtrlAreaJump_Callback, 8, 2, TASK_AREA_JUMP);
    g_areaJumpState = 0;
    g_areaJumpCurrentPosition.x = g_areaJumpCurrentPosition.y = g_areaJumpCurrentPosition.z = 0.0f;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void ResetAreaJumpWarp() {
    g_areaJumpWarpEnabled = 0;
}
#endif

std::int32_t IsAreaJumpStateFive() {
    return g_areaJumpState == 5;
}

void CancelPendingAreaJump() {
    if (g_areaJumpState == 9 || g_areaJumpState == 1 || g_areaJumpState == 2) {
        g_initialAreaJumpSchedule = pendingAreaJumpSchedule;
        g_areaJumpState = 4;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void QueueAreaJumpToLocation(shen_name areaId, std::int32_t entryId, const Vector3 *position, std::int32_t yaw) {
    SavedAreaJumpLocation location;
    location.position = *position;
    location.yaw = yaw;
    shenGetCurrentDateTime(location.capturedTime);
    ActivateSavedAreaJumpLocation();
    RestoreSavedAreaJumpLocation(&location, g_areaJumpPersistentPlacement);
    shenQueueChangeModuleAndScene(SystemModule_Game, g_actNumber, areaId, entryId);
}
#endif

} // namespace s2
} // namespace shenmue

#endif
