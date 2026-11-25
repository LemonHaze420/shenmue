#include "s2/event_character_control_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_control_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/map_event_task_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include <cstring>
#include "s2/player_state_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_rig.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/hand_motion.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include <cmath>
#endif
#define SHENMUE_MOTION_NULL 0
#define SHENMUE_MOTION_CONST_CAST(type, value) static_cast<type *>(value)
#include "shenmue.h"
#include "s2/character_pose_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include <cstdio>
#define SHENMUE_MOTION_DATA(name) name
#define SHENMUE_MOTION_NAME(value) shenmue::shen_name(value)
#include "s2/character_yaw_dc.hpp"
#include "s2/math.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "s2/motion_task_dc.hpp"
#include "s2/motion_snapshot_capture_dc.hpp"
#include "s2/motion_snapshot_control_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/pak_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/world_render.hpp"
#endif
#include "s2/player_model_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_light_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/map_models_dc.hpp"
namespace shenmue {
namespace dc {
namespace sdk {
void SetMirroredModelDrawMode(std::int32_t enabled);
}
} // namespace dc
} // namespace shenmue
#include "s2/event_character_state_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/character_transparency_dc.hpp"
#include "s2/pause.hpp"
#endif
#include "s2/model_image_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/prop_dc.hpp"

namespace shenmue {
namespace s2 {

void MapEventTaskEntry_Callback(void *data);
void VisitMapEventCharacter(Character *character, HLib::Task *, MapEventTaskParameters *parameters);
bool ExecMapEventSceneChange(MapEventTaskParameters *parameters, std::int32_t region);
void Exec_MapEventSceneChange_Callback(void *);
bool CheckMapEventActionInput();
void SetMapEventRegionEnabled(MapEventComponentState *state, std::uint32_t region, std::int32_t enabled);
std::int32_t IsMapEventRegionEnabled(const MapEventComponentState *state, std::uint32_t region);
void MEVW_Callback(Character *character, void *data);

bool HasCharacterMotionWork(const Character *character);
std::uint32_t GetCharacterMotionCompatibilityResult(const Character *character);
std::int32_t IsPlayerControlEnabled();
shen_name g_newAreaId;
std::int32_t g_newEntryId;
std::uint32_t g_newScenarioFlags;
void SetCharacterLayer(Character *character, std::int32_t layer);
PadControlEntry *GetMapPadControlEntry();

// @verified - d
std::uint32_t GetCurrentControlInput(std::int32_t selector) {
    std::uint32_t value = 0;
    const LegacyInputSlot &input = *ctrlGetPadInputBuffer0(GetMapPadControlEntry());
    switch (selector) {
    case INPUT_HELD:
        value = input.held;
        break;
    case INPUT_PRESSED:
        value = input.pressedPriority;
        break;
    case INPUT_REPEAT:
        value = input.repeatedPresses;
        break;
    case INPUT_RIGHT_TRIGGER:
        value = input.triggers[0];
        break;
    case INPUT_LEFT_TRIGGER:
        value = input.triggers[1];
        break;
    case INPUT_RIGHT_X:
        value = input.leftStickX;
        break;
    case INPUT_RIGHT_Y:
        value = input.leftStickY;
        break;
    case INPUT_NEW_PRESSES:
        value = input.rawPressed;
        break;
    case INPUT_LEFT_X:
        value = input.rightStickX;
        break;
    case INPUT_LEFT_Y:
        value = input.rightStickY;
        break;
    }
    return value;
}

bool EV_SetCharaControllerFlags(shen_name name, std::uint32_t selector, std::int32_t operation) {
    std::uint32_t mask = 0;
    bool result = false;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (!character)
        return false;
    CharacterControlModel *const controller = static_cast<CharacterControlModel *>(charFindComp(character, CHARACTER_CONTROL));
    if (!controller)
        return false;
    switch (selector) {
    case 0:
        mask = CCMF_UpdateHeight;
        break;
    case 1:
        mask = CCMF_ApplyGravity;
        break;
    case 2:
        mask = CCMF_EvaluateMotion;
        break;
    case 3:
        mask = CCMF_VoxelSweep;
        break;
    }
    switch (operation) {
    case 0:
        controller->flags &= ~mask;
        break;
    case 1:
        controller->flags |= mask;
        break;
    case -1:
        result = (controller->flags & mask) != 0;
        break;
    }
    return result;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float ccmGetGroundHeight(Character *character) {
    CharacterControlModel *const ccm = static_cast<CharacterControlModel *>(charFindComp(character, CHARACTER_CONTROL));
    return ccm ? ccm->groundHeight : 0.0f;
}
#endif

// @verified - d
void SetCharacterMapRegionEnabled(Character *character, std::uint32_t region, std::int32_t enabled) {
    MapEventComponentState *const state = static_cast<MapEventComponentState *>(charFindComp(character, CHARACTER_MAP_EVENT));
    SetMapEventRegionEnabled(state, region, enabled);
}

// @verified - d
std::int32_t IsCharacterMapRegionEnabled(Character *character, std::uint32_t region) {
    const MapEventComponentState *const state = reinterpret_cast<const MapEventComponentState *>(charFindComp(character, CHARACTER_MAP_EVENT));
    return IsMapEventRegionEnabled(state, region);
}

HLib::Task *Queue_Callback_MapEventTaskEntry(std::uint8_t nextFunctionIndex) {
    const std::int8_t nextIndex = static_cast<std::int8_t>(nextFunctionIndex);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MapEventTaskEntry_Callback, nextIndex, 2, sizeof(MapEventTaskParameters), TASK_MEVE);
    static_cast<MapEventTaskParameters *>(HLib::GetTaskParameter(task))->callbacks = kEmptyMapEventCallbacks;
    return task;
}

// @verified - d
void MapEventTaskEntry_Callback(void *data) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    HLib::Task *const scenarioTask = control->scenarioTask;
    MapCharacterIterator iterator;
    BeginMapCharacterIteration(&iterator, 0);
    Character *character;
    while ((character = iterator.character) != 0) {
        VisitMapEventCharacter(character, scenarioTask, static_cast<MapEventTaskParameters *>(data));
        NextMapCharacter(&iterator);
    }
}

// @verified - d
void VisitMapEventCharacter(Character *character, HLib::Task *, MapEventTaskParameters *parameters) {
    if (IsModelComponentEnabled(character, CHARACTER_MAP_EVENT) == 0)
        return;
    MapEventComponentState *const state = static_cast<MapEventComponentState *>(charFindComp(character, CHARACTER_MAP_EVENT));
    if ((state->flags & 1U) && state->mode != 4)
        return;
    if (state->currentRegion == 255) {
        if (ryoNotBusy()) {
            SetCharacterLayer(character, static_cast<std::int32_t>(state->mode));
            state->flags |= 1U;
        }
        return;
    }
    if (!IsMapEventRegionEnabled(state, state->currentRegion))
        return;
    switch (state->mode) {
    case 1:
        if (CheckMapEventActionInput())
            ExecMapEventSceneChange(parameters, static_cast<std::int32_t>(state->currentRegion));
        return;
    case 3:
        if (!CheckMapEventActionInput())
            return;

    case 2:
        SetMapEventRegionEnabled(state, state->currentRegion, 0);

    case 0:
    case 4:
        if (ExecMapEventSceneChange(parameters, static_cast<std::int32_t>(state->currentRegion)))
            state->flags |= 1U;
        return;
    default:
        return;
    }
}

// @verified - d
bool ExecMapEventSceneChange(MapEventTaskParameters *parameters, std::int32_t region) {
    const MapEventCallbackEntry *entry = parameters->callbacks;
    if (entry != kEmptyMapEventCallbacks) {
        for (; entry->eventId != -1; ++entry) {
            if (entry->eventId != region)
                continue;
            if (!ryoNotBusy())
                return false;
            entry->callback(entry->argument);
            return true;
        }
    }
    for (const AreaTableEntry *area = g_areaTableEntries; area->area != UINT32_MAX; ++area) {
        if (area->area != g_currentMap)
            continue;
        for (const MapEventTransition *transition = area->transitions; transition->eventId != -1; ++transition) {
            if (transition->eventId != region)
                continue;
            if (!ryoNotBusy())
                return false;
            if (!RetainQronMapEventDestination(transition->areaId, region))
                return true;
            SetPlayerControlEnabled(0);
            shenToggleAreaFlags(2);
            const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            HLib::Task *const film = control->filmTask;
            ScreenFilmColourRequest request;
            request.durationFrames = 20;
            request.start[0] = 0;
            request.start[1] = 0;
            request.start[2] = 0;
            request.start[3] = 0;
            request.target[0] = 255;
            request.target[1] = 0;
            request.target[2] = 0;
            request.target[3] = 0;
            WaitForNextTask(film, request, 0);
            HLib::EnqueueTaskWithoutParameter(Exec_MapEventSceneChange_Callback, 3, 2, static_cast<std::int32_t>(TASK_EVFO));
            g_newAreaId = transition->areaId;
            g_newEntryId = transition->entryId;
            g_newScenarioFlags = transition->scenarioFlags;
            spmFadeOut(0);
            return true;
        }
    }
    return true;
}

void Exec_MapEventSceneChange_Callback(void *) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (DoesScreenFilmColourExist(control->filmTask))
        return;
    SystemModule module = SystemModule_Game;
    if (static_cast<std::int32_t>(g_newAreaId) < 0)
        module = SystemModule_Credits;
    if (g_newScenarioFlags) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        scenSetTaskFlags(static_cast<std::int8_t>(g_newScenarioFlags));
#else
        scenSetTaskFlags(static_cast<std::uint8_t>(g_newScenarioFlags));
#endif
    }
    shenQueueChangeModuleAndScene(module, g_actNumber, g_newAreaId, g_newEntryId);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    HLib::DestroyCurrentEventTask();
#else
    HLib::DestroyCurrentTask();
#endif
}

bool CheckMapEventActionInput() {
    if (!IsPlayerControlEnabled())
        return false;
    if (ctrlGetPadInputBuffer0(GetMapPadControlEntry())->pressedPriority & kMapEventActionPressMask)
        return true;
    return false;
}

// @verified - d
void SetMapEventRegionEnabled(MapEventComponentState *state, std::uint32_t region, std::int32_t enabled) {
    const std::uint8_t mask = static_cast<std::uint8_t>(1U << (region & 7U));
    region >>= 3U;
    if (enabled)
        state->enabledRegions[region] |= mask;
    else
        state->enabledRegions[region] &= static_cast<std::uint8_t>(~mask);
}

std::int32_t IsMapEventRegionEnabled(const MapEventComponentState *state, std::uint32_t region) {
    const std::uint8_t mask = static_cast<std::uint8_t>(1U << (region & 7U));
    const std::uint32_t byteIndex = region >> 3U;
    if (state->enabledRegions[byteIndex] & mask)
        return 1;
    return 0;
}

// @verified - d
void AllocateMapEventComponent(Character *character) {
    MapEventComponentState *const state =
        static_cast<MapEventComponentState *>(AllocateModelComponent(character, CHARACTER_MAP_EVENT, sizeof(MapEventComponentState), MEVW_Callback));
    state->flags |= 3U;
    state->currentRegion = 0;
    state->mode = 0;
    std::memset(state->enabledRegions, 0xff, sizeof(state->enabledRegions));
    SetMapEventRegionEnabled(state, 0, 0);
}

// @verified - d
void MEVW_Callback(Character *character, void *data) {
    MapEventComponentState *const state = static_cast<MapEventComponentState *>(data);
    if (HasCharacterMotionWork(character)) {
        switch (GetCharacterMotionCompatibilityResult(character)) {
        case 2:
        case 3:
            if (!IsOfThisVisibilityType(character, CVM_MotionworkDisplayCheck))
                return;
            break;
        }
    }
    Vector3 position;
    GetCharacterWorldPosition(character, &position);
    std::uint32_t region = flddTestMapEventIntersection(&position);
    const std::uint32_t mode = region >> 16;
    region = static_cast<std::uint16_t>(region);
    if (region == state->currentRegion)
        return;
    if (((state->flags >> 1) & 1U) || region == 255)
        state->flags &= ~1U;
    else
        state->flags |= 1U;
    state->currentRegion = region;
    state->mode = mode;
}

// @verified - d
void ParseMapEv(Character *character, std::int32_t enabled) {
    MapEventComponentState *const state = static_cast<MapEventComponentState *>(charFindComp(character, CHARACTER_MAP_EVENT));
    std::uint32_t flags, bit;
    if (enabled) {
        flags = state->flags;
        bit = 2;
    } else {
        flags = state->flags & ~2U;
        bit = 1;
    }
    state->flags = flags | bit;
}

std::uint32_t IsCharacterMapEvSuspended(Character *character) {
    const MapEventComponentState *const state = reinterpret_cast<const MapEventComponentState *>(charFindComp(character, CHARACTER_MAP_EVENT));
    return (state->flags >> 1) & 1U;
}

// @verified - d
std::uint32_t GetCharacterMapRegion(Character *character) {
    const MapEventComponentState *const state = reinterpret_cast<const MapEventComponentState *>(charFindComp(character, CHARACTER_MAP_EVENT));
    return state->currentRegion;
}

// @verified - d
void SetCharacterMapEventRegionEnabled(shen_name name, std::uint32_t region, std::uint32_t enabled) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (character)
        SetCharacterMapRegionEnabled(character, region, static_cast<std::int32_t>(enabled & 1U));
}

// @verified - d
std::int32_t IsCharacterMapEventRegionEnabled(shen_name name, std::uint32_t region) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (!character)
        return 0;
    return IsCharacterMapRegionEnabled(character, region);
}

std::uint32_t GetCharacterMapEventRegion(shen_name name) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (!character)
        return 0;
    return GetCharacterMapRegion(character);
}

// @verified - d
void SetMapEventCallbacks(const MapEventCallbackEntry *callbacks) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    MapEventTaskParameters *const parameters = static_cast<MapEventTaskParameters *>(HLib::GetTaskParameter(control->mapEventTask));
    parameters->callbacks = callbacks;
}

// @verified - d
float SetScenarioCharacterRadius(shen_name name, float radius) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    const float previous = GetCharacterDisplayRadius(character);
    if (!(previous < 0.0f))
        SetCharacterRadius(character, radius);
    return previous;
}

// @verified - d
std::uint32_t SetScenarioCharacterFlag40(shen_name name, std::int32_t enabled) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    const std::uint32_t previous = charaCheckFlags(character);
    if (enabled >= 0)
        charaToggleFlag(character, enabled);
    return previous;
}

} // namespace s2
} // namespace shenmue

extern "C" {

std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetCurrentControlInput(std::int32_t selector) {
    return shenmue::s2::GetCurrentControlInput(selector);
}

bool __attribute__((renesas)) shenmue_s2_event_EV_SetCharaControllerFlags(std::uint32_t name, std::uint32_t selector, std::int32_t operation) {
    return shenmue::s2::EV_SetCharaControllerFlags(shenmue::shen_name(name), selector, operation);
}

void __attribute__((renesas)) shenmue_s2_event_SetCharacterMapEventRegionEnabled(std::uint32_t name, std::uint32_t region, std::uint32_t enabled) {
    shenmue::s2::SetCharacterMapEventRegionEnabled(shenmue::shen_name(name), region, enabled);
}

bool __attribute__((renesas)) shenmue_s2_event_IsCharacterMapEventRegionEnabled(std::uint32_t name, std::uint32_t region) {
    return shenmue::s2::IsCharacterMapEventRegionEnabled(shenmue::shen_name(name), region);
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_GetCharacterMapEventRegion(std::uint32_t name) {
    return shenmue::s2::GetCharacterMapEventRegion(shenmue::shen_name(name));
}

void __attribute__((renesas)) shenmue_s2_event_SetMapEventCallbacks(const shenmue::s2::MapEventCallbackEntry *callbacks) {
    shenmue::s2::SetMapEventCallbacks(callbacks);
}

float __attribute__((renesas)) shenmue_s2_event_SetScenarioCharacterRadius(std::uint32_t name, float radius) {
    return shenmue::s2::SetScenarioCharacterRadius(shenmue::shen_name(name), radius);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_GetNpcBatchCountOverride() {
    return shenmue::s2::GetNpcBatchCountOverride();
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_EV_SetMapLightingPlacementEnabled(std::int32_t index, std::int32_t enabled) {
    return shenmue::s2::EV_SetMapLightingPlacementEnabled(index, enabled);
}
}

#endif
