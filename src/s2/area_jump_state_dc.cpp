#include "s2/area_jump_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/door_state_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include <cstring>
#include "s2/character_yaw_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/map_event_character_dc.hpp"

namespace shenmue {
namespace s2 {

std::uint32_t HasSavedAreaJumpLocation();

SavedAreaJumpLocation g_savedAreaJumpLocation;
std::byte g_areaJumpPlacement[0x20];
std::uint8_t g_savedAreaJumpLocationRequested, g_savedAreaJumpLocationActive = 0;
std::int32_t g_canDoAreaJump;

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint8_t unusedCtrlFlag;
#endif

} // namespace

// @verified - d
void CaptureCurrentPlayerState(SavedAreaJumpLocation *location, std::byte *placement) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const Character *const character = control->ryo;
    if (!character) {
        std::memset(location, 0, 0x30);
        return;
    }
    GetCharacterWorldPosition(character, &location->position);
    location->yaw = GetCharacterWorldYaw(character);
    location->layer = static_cast<std::uint8_t>(GetCharacterLayer(character));
    std::memcpy(placement, g_areaJumpPlacement, 0x20);
}

// @verified - d
bool GetSavedAreaJumpLocation(Vector3 &position, std::int32_t &yaw) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    position = g_savedAreaJumpLocation.position;
    yaw = g_savedAreaJumpLocation.yaw;
    if (HasSavedAreaJumpLocation())
        return true;
    return false;
#else
    position = __typeof__(position)();
    yaw = 0;
    return HasSavedAreaJumpLocation() != 0;
#endif
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void EV_ApplyTranslationClockVisibility(std::uint32_t) {
    SavedAreaJumpLocation *location = &g_savedAreaJumpLocation;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = control->ryo;
    if (!character)
        return;
    SetCharacterWorldPosition(character, &location->position);
    SetCharacterWorldYaw(character, location->yaw);
    SetCharacterLayer(character, location->layer);
    const std::int32_t state = camCtrlGetState();
    camCtrlSetState(9);
    camCtrlSetState(state);
    camCtrlSetFovTransitionTo(1);
    g_savedAreaJumpLocationActive = 0;
}
#else
// @verified - d
void EV_ApplyTranslationClockVisibility(std::uint32_t) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = control->ryo;
    if (!character)
        return;
    SetCharacterWorldPosition(character, &g_savedAreaJumpLocation.position);
    SetCharacterWorldYaw(character, static_cast<std::int16_t>(g_savedAreaJumpLocation.yaw));
    SetCharacterLayer(character, g_savedAreaJumpLocation.layer);
    const std::int32_t state = camCtrlGetState();
    camCtrlSetState(9);
    camCtrlSetState(state);
    camCtrlSetFovTransitionTo(1);
    g_savedAreaJumpLocationActive = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetUnusedCtrlFlag(std::uint8_t value) {
    unusedCtrlFlag = value;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetUnusedFlag() {
    if (unusedCtrlFlag)
        unusedCtrlFlag = 0;
}
#endif

// @verified - d
void RestoreSavedAreaJumpLocation(const SavedAreaJumpLocation *location, const std::byte *placement) {
    std::memcpy(&g_savedAreaJumpLocation, location, 0x30);
    std::memcpy(g_areaJumpPlacement, placement, 0x20);
}

// @verified - d
void ActivateSavedAreaJumpLocation() {
    g_savedAreaJumpLocationRequested = 1;
    g_savedAreaJumpLocationActive = 1;
}

// @verified - d
void ResetGlobalStateFlags() {
    g_savedAreaJumpLocationRequested = 0;
    g_savedAreaJumpLocationActive = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ConsumeSavedAreaJumpRequest() {
    if (g_savedAreaJumpLocationRequested) {
        --g_savedAreaJumpLocationRequested;
        return 1;
    }
    return 0;
}
#endif

// @verified - d
std::uint32_t HasSavedAreaJumpLocation() {
    if (g_savedAreaJumpLocationActive)
        return UINT32_MAX;
    return 0;
}

// @verified - d
void SetAreaJumpPlacementByte(std::int32_t offset, std::uint8_t value) {
    g_areaJumpPlacement[offset] = static_cast<std::byte>(value);
}

// @verified - d
std::uint8_t GetAreaJumpPlacementByte(std::int32_t offset) {
    return static_cast<std::uint8_t>(g_areaJumpPlacement[offset]);
}

// @verified - d
float GetAreaJumpPlacementFloat(std::int32_t offset) {
    float value;
    std::memcpy(&value, g_areaJumpPlacement + offset, 4);
    return value;
}

// @verified - d
void SetAreaJumpPlacementFloat(std::int32_t offset, float value) {
    std::memcpy(g_areaJumpPlacement + offset, &value, 4);
}

// @verified - d
void SetTemporaryAreaJumpAuthor(shen_name author) {
    g_doorRuntimeState.temporaryAuthor = author;
}

// @verified - d
std::int32_t EV_CanDoAreaJump() {
    return g_canDoAreaJump;
}

// @verified - d
void EV_SetCanDoAreaJumpTo(std::int32_t enabled) {
    g_canDoAreaJump = enabled;
}

} // namespace s2
} // namespace shenmue

#endif
