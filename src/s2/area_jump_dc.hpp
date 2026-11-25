#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/date_time.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue {
namespace s2 {

union AreaJumpDestination {
    std::uint32_t areaId;
    std::int8_t candidateIndices[4];
    std::uint32_t specialMode;
};
struct AreaJumpDestinationEntry {
    std::int32_t speakIconFlag;
    AreaJumpDestination destination;
    std::int32_t destinationEntryId;
};
struct AreaJumpScheduleEntry {
    std::int16_t destinationIndices[4];
    std::int32_t requiredShenmueFlag;
};
struct CtrlAreaJumpEntry {
    shen_name sourceAreaId;
    float firstX, firstZ, secondX, secondZ;
    float endFirstX, endFirstZ, endSecondX, endSecondZ;
    float targetY;
    shen_name requiredAuthorId;
    const AreaJumpScheduleEntry *scheduleData;
};
struct SavedAreaJumpLocation {
    Vector3 position;
    std::int32_t yaw;
    std::uint8_t layer;
    std::byte retained11[0x13];
    DateTime capturedTime;
    std::byte retained2e[2];
};
struct AreaJumpPersistentState {
    SavedAreaJumpLocation location;
    std::byte playerPlacement[0x20];
    shen_name sceneAreaId;
    std::int16_t sceneEntryId;
    std::byte retained56[2];
    shen_name authorId;
};
typedef char Layout_area_jump_dc_0[(sizeof(AreaJumpDestination) == 4) ? 1 : -1];
typedef char Layout_area_jump_dc_1[(sizeof(AreaJumpDestinationEntry) == 12) ? 1 : -1];
typedef char Layout_area_jump_dc_2[(sizeof(AreaJumpScheduleEntry) == 12) ? 1 : -1];
typedef char Layout_area_jump_dc_3[(sizeof(CtrlAreaJumpEntry) == 0x30) ? 1 : -1];
typedef char Layout_area_jump_dc_4[(offsetof(CtrlAreaJumpEntry, targetY) == 0x24) ? 1 : -1];
typedef char Layout_area_jump_dc_5[(offsetof(CtrlAreaJumpEntry, requiredAuthorId) == 0x28) ? 1 : -1];
typedef char Layout_area_jump_dc_6[(offsetof(CtrlAreaJumpEntry, scheduleData) == 0x2c) ? 1 : -1];
typedef char Layout_area_jump_dc_7[(sizeof(SavedAreaJumpLocation) == 0x30) ? 1 : -1];
typedef char Layout_area_jump_dc_8[(offsetof(SavedAreaJumpLocation, capturedTime) == 0x24) ? 1 : -1];
typedef char Layout_area_jump_dc_9[(sizeof(AreaJumpPersistentState) == 0x5c) ? 1 : -1];
typedef char Layout_area_jump_dc_10[(offsetof(AreaJumpPersistentState, playerPlacement) == 0x30) ? 1 : -1];
typedef char Layout_area_jump_dc_11[(offsetof(AreaJumpPersistentState, sceneEntryId) == 0x54) ? 1 : -1];
typedef char Layout_area_jump_dc_12[(offsetof(AreaJumpPersistentState, authorId) == 0x58) ? 1 : -1];
#define TASK_AREA_JUMP SHEN_NAME4('A', 'S', 'J', 'P')
#define AREA_JUMP_AUTHOR SHEN_NAME4('A', 'K', 'B', '0')
extern std::int32_t g_areaJumpState, g_areaJumpSelectedDestination;
extern const CtrlAreaJumpEntry *g_activeAreaJumpEntry;
extern const AreaJumpScheduleEntry *g_currentAreaJumpSchedule, *g_initialAreaJumpSchedule;
extern const std::int8_t *g_alternateAreaJumpSchedule;
extern Vector3 g_areaJumpCurrentPosition, g_areaJumpPreviousPosition;
extern SavedAreaJumpLocation g_areaJumpPersistentLocation;
extern std::byte g_areaJumpPersistentPlacement[0x20];
extern shen_name g_areaJumpPersistentSceneAreaId, g_areaJumpPersistentAuthorId;
extern std::int16_t g_areaJumpPersistentSceneEntryId;
extern std::uint16_t g_areaJumpWarpEnabled;
extern SavedAreaJumpLocation g_savedAreaJumpLocation;
extern std::byte g_areaJumpPlacement[0x20];
extern std::uint8_t g_savedAreaJumpLocationRequested, g_savedAreaJumpLocationActive;
extern std::int32_t g_canDoAreaJump;

void initCtrlAreaJump();
void CtrlAreaJump_Callback(void *data);
std::int32_t IsAreaJumpEnabled();
std::int32_t GetAreaJumpSpeakIconFlag(std::int32_t index);
std::int32_t CheckLineSegmentXZIntersection(float firstX, float firstZ, float secondX, float secondZ, float targetY);
bool FindMapEventEntryLocation(shen_name areaId, std::int32_t entryId, Vector3 *position);
void AdvanceTimeForDistance(const Vector3 *position);
void CaptureCurrentAreaJumpState();
void CaptureCurrentPlayerState(SavedAreaJumpLocation *location, std::byte *placement);
void RestoreSavedAreaJumpLocation(const SavedAreaJumpLocation *location, const std::byte *placement);
void ActivateSavedAreaJumpLocation();
void ResetGlobalStateFlags();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetUnusedCtrlFlag(std::uint8_t value);
#endif
void SetUnusedFlag();
std::uint32_t HasSavedAreaJumpLocation();
bool GetSavedAreaJumpLocation(Vector3 &position, std::int32_t &yaw);
std::int32_t EV_CanDoAreaJump();
void EV_SetCanDoAreaJumpTo(std::int32_t enabled);
std::uint8_t GetAreaJumpPlacementByte(std::int32_t offset);
void SetAreaJumpPlacementByte(std::int32_t offset, std::uint8_t value);
float GetAreaJumpPlacementFloat(std::int32_t offset);
void SetAreaJumpPlacementFloat(std::int32_t offset, float value);
void SetTemporaryAreaJumpAuthor(shen_name author);
void EV_ApplyTranslationClockVisibility(std::uint32_t startParameters);

} // namespace s2
} // namespace shenmue

#endif
