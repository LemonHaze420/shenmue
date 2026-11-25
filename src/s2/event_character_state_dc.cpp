#include "s2/event_character_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_alpha_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include <float.h>
#include <cmath>
#include <cstring>
#define SHENMUE_HAND_NULL 0
#define SHENMUE_HAND_INITIALIZE =
#define SHENMUE_HAND_MAX_FLOAT FLT_MAX
#define SHENMUE_HAND_FMA(a, b, c) ((a) * (b) + (c))
#include "s2/model_image_component_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "platform/dc/matrix.hpp"
#include <cstdio>
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/model_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/player_model_dc.hpp"
#endif
#include "shenmue.h"
#include "s2/character_transparency_dc.hpp"
#include "s2/math.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/event_character_control_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_rig.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/hand_motion.hpp"
#endif
#define SHENMUE_MOTION_NULL 0
#define SHENMUE_MOTION_CONST_CAST(type, value) static_cast<type *>(value)
#define SHENMUE_MOTION_DATA(name) name
#define SHENMUE_MOTION_NAME(value) shenmue::shen_name(value)
#include "s2/character_yaw_dc.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "s2/motion_task_dc.hpp"
#include "s2/motion_snapshot_capture_dc.hpp"
#include "s2/motion_snapshot_control_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/world_render.hpp"
#include "s2/field_collision_dc.hpp"
#endif
#include "s2/character_model_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/object_light_dc.hpp"
#include "s2/model_render_dc.hpp"
namespace shenmue {
namespace dc {
namespace sdk {
void SetMirroredModelDrawMode(std::int32_t enabled);
}
} // namespace dc
} // namespace shenmue
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/pause.hpp"
#endif
#include "s2/pad_control_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "s2/map_event_task_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/hand_draw_dc.hpp"
#include "s2/string_utility.hpp"

namespace shenmue {
namespace s2 {

CharacterHandComponent *AllocateHNDL(Character *character);
CharacterHandComponent *AllocateHNDR(Character *character);

bool HasCharacterMotionWork(const Character *character);
CharacterHandComponent *AllocateHNDR(Character *character);
CharacterHandComponent *AllocateHNDL(Character *character);
CharacterHandComponent *AllocateHNDR(Character *character);
CharacterHandComponent *AllocateHNDL(Character *character);
CharacterHandComponent *AllocateHNDR(Character *character);
CharacterHandComponent *AllocateHNDL(Character *character);
CharacterHandComponent *AllocateHNDR(Character *character);
CharacterHandComponent *AllocateHNDL(Character *character);
void GetHandPrimaryNodeRotation(const CharacterHandComponent &hand, Vector3i &rotation);
CharacterHandComponent *AllocateHNDL(Character *character);
CharacterHandComponent *AllocateHNDR(Character *character);
std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour);
void SetupModelDataDefaultVS(MT7Node *root);

namespace {

std::int32_t generatedEventCharacterCount = 0;

} // namespace

void ResetImageModule(Character *character);
void GetHandPrimaryNodeRotation(const CharacterHandComponent &hand, Vector3i &rotation);
CharacterHandComponent *AllocateHNDL(Character *character);
CharacterHandComponent *AllocateHNDR(Character *character);
void SetHandPrimaryRotation(CharacterHandComponent *hand, std::int32_t x, std::int32_t y, std::int32_t z);
void *charFindComp(const Character *character, shen_name token);
void (*charaAssignCoreCallback(Character *character, void (*callback)(Character *)))(Character *);
void *charFindComp(const Character *character, shen_name token);

// @verified - d
void SetMapModelSlotEnabled(std::int32_t enabled, std::int32_t index) {
    if (index < 32)
        g_mapModelSlots[index].enabled = static_cast<std::uint32_t>(enabled);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
CharacterHandComponent *GetCharacterHand(Character *character, std::int32_t side) {
    return !side ? AllocateHNDR(character) : AllocateHNDL(character);
}
#endif

// @verified - d
shen_name CreateEventCharacter(std::int32_t type, const std::byte *data, shen_name name) {
    char text[8];
    Character *const character = AddCharacterHandler(type);
    if (!character)
        return shen_name();
    if (!name) {
        do {
            std::sprintf(text, " %03d", generatedEventCharacterCount);
            if (++generatedEventCharacterCount > 999)
                generatedEventCharacterCount = 0;
        } while (scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, *reinterpret_cast<shen_name *>(text)));
    } else {
        *reinterpret_cast<shen_name *>(text) = name;
    }
    const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    CreateScenarioCharacterId(control->scenarioTask, *reinterpret_cast<shen_name *>(text), character);
    SetupImageModuleHandlers(character);
    if (data) {
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        SetupImageModule(character, control->modelManagerTask, reinterpret_cast<const char *>(data));
        if (character->model)
            SetupModelForImageModule(character->model);
    }
    return *reinterpret_cast<shen_name *>(text);
}

// @verified - d
void EV_SetupImageModule(shen_name name, const char *imageName) {
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (!imageName) {
        ResetImageModule(character);
    } else {
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        SetupImageModuleVariants(character, control->modelManagerTask, imageName, 0);
        if (character->model)
            SetupModelForImageModule(character->model);
    }
}

// @verified - d
bool IsScenarioCharacterDisplayEnabled(shen_name name) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return IsOfThisVisibilityType(scenFindChara(control->scenarioTask, name), CVM_DisplayEnabled);
}

// @verified - d
void EV_EnableCharacterDisplay(shen_name name) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    EV_SetCharDisplay(scenFindChara(control->scenarioTask, name), CVM_DisplayEnabled, 1);
}

// @verified - d
void EV_DisableCharacterDisplay(shen_name name) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    EV_SetCharDisplay(scenFindChara(control->scenarioTask, name), CVM_DisplayEnabled, 0);
}

// @verified - d
void SetScenarioCharacterKillMode(shen_name name, std::int32_t mode) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    MarkCharacterForCleanup(scenFindChara(control->scenarioTask, name), mode);
}

// @verified - d
void EV_ToggleCharacterFlag(shen_name name) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (character) {
        HLib::Task *const task = GetCharacterHandlerTask(character);
        SetCharacterActive(character, 0);
        HLib::SetupTimedTask(task, 0);
    }
}

// @verified - d
void EV_EnsureCharacterDraw(shen_name name) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (character) {
        HLib::Task *const task = GetCharacterHandlerTask(character);
        SetCharacterActive(character, 1);
        HLib::ResolveDeferredCallback(task);
    }
}

// @verified - d
bool IsScenarioCharacterActive(shen_name name) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (!character)
        return false;
    GetCharacterHandlerTask(character);
    return IsCharacterActive(character);
}

// @verified - d
std::int32_t DoesScenarioCharacterExist(shen_name name) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return scenFindChara(control->scenarioTask, name) != 0;
}

// @verified - d
std::int32_t EV_SetCharacterAlphaEnabled(shen_name name, std::int32_t enabled) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return cpamSetFlag(scenFindChara(control->scenarioTask, name), enabled);
}

// @verified - d
void SetScenarioCharacterTransparency(shen_name name, float alpha) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (character)
        character->alpha = alpha;
}

// @verified - d
void mtwkSetMotionworkScale3(shen_name name, const Vector3 *scale) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (character && !HasCharacterMotionWork(character))
        SetCharacterScale(character, scale);
}

// @verified - d
void EV_SetCharacterImageSelection(shen_name name, std::int32_t selection) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (character)
        SetCharacterImageSelection(character, selection);
}

// @verified - d
std::int32_t GetCharacterFaceCacheIndex(const Character *character) {
    std::int32_t index = -1;
    if (!HasCharacterMotionWork(character))
        return -1;
    const char *const name = GetCharacterFaceNameNullsub(character);
    if (!name)
        return -1;
    index = static_cast<std::int8_t>(GetCharacterFaceIndexNullsub(name));
    if (index == 8)
        return -1;
    return index;
}

// @verified - d
void ReleaseCharacterFaceCache(const Character *character) {
    if (HasCharacterMotionWork(character)) {
        if (const char *const name = GetCharacterFaceNameNullsub(character)) {
            const std::int8_t index = static_cast<std::int8_t>(FindCachedFacialModelIndex(name));
            if (index != 8)
                ReleaseCachedFacialModel(name, static_cast<std::uint8_t>(index));
        }
    }
}

// @verified - d
void SetCharacterFaceParameters(const Character *character, std::int32_t mode, std::int32_t first, std::int32_t second) {
    if (HasCharacterMotionWork(character)) {
        if (const char *const name = GetCharacterFaceNameNullsub(character))
            SetCharacterFaceParametersNullsub(name, mode, first, second);
    }
}

// @verified - d
void SetCharacterFaceSecondaryParameters(const Character *character, std::uint8_t mode, std::int16_t first, std::uint8_t second) {
    if (HasCharacterMotionWork(character)) {
        if (const char *const name = GetCharacterFaceNameNullsub(character))
            SetCharacterFaceSecondaryParametersNullsub(name + 0x44, mode, first, second);
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyCharacterHandRotation(Character *character, std::int32_t side, std::uint32_t flags, const Vector3i *rotation) {
    CharacterHandComponent *const hand = GetCharacterHand(character, side);
    if (!hand)
        return;
    Vector3i current;
    mhndGetPrimaryRotation(hand, &current);
    if (flags & 1U)
        current.x = rotation->x;
    if (flags & 4U)
        current.y = rotation->y;
    if (flags & 0x10U)
        current.z = rotation->z;
    if (flags & 2U)
        current.x += rotation->x;
    if (flags & 8U)
        current.y += rotation->y;
    if (flags & 0x20U)
        current.z += rotation->z;
    mhndSetPrimaryRotation(hand, current.x, current.y, current.z);
}
#endif

// @verified - d
std::uint32_t ExchangeMapModelEnabled(std::int32_t index, std::uint32_t enabled) {
    const std::uint32_t previous = GetMapModelEnabled(index);
    SetMapModelEnabled(enabled, index);
    return previous;
}

// @verified - d
std::uint32_t EV_SetMapLightingPlacementEnabled(std::int32_t index, std::int32_t enabled) {
    const std::uint32_t previous = GetMapLightingPlacementEnabled(index);
    SetMapLightingPlacementEnabled(enabled, index);
    return previous;
}

// @verified - d
void ReplaceScenarioImageModule(shen_name name, const char *imageName, std::int32_t count) {
    const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (!imageName) {
        ResetImageModule(character);
        DisableHandPoseController(character);
    } else {
        GetCharacterImageSourceType(character);
        RestoreClothMeshBindings(character);
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        SetupImageModuleVariants(character, control->modelManagerTask, imageName, count);
        if (HasCharacterMotionWork(character)) {
            GetCharacterImageSourceType(character);
            FindCLMD(character);
            FindOSAG(character);
        }
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyCharacterGeometryLighting(Character *character, std::int32_t replaceDrawCallback) {
    Vector4 colour;
    if (GetGeometryLightColour(&colour))
        return;
    Vector3 position;
    GetCharacterWorldPosition(character, &position);
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&position);
    ApplyGeometryLightTree(character->model, g_lightRuntimeEntries, 256, &colour);
    PopCurrentMatrix(1);
    if (replaceDrawCallback)
        imgmAssignUpdateCallback(character, DrawPreparedVertexColours);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ResetCharacterGeometryLighting(Character *character) {
    SetupModelDataDefaultVS(character->model);
}
#endif

} // namespace s2
} // namespace shenmue

extern "C" {

bool __attribute__((renesas)) shenmue_s2_event_IsScenarioCharacterDisplayEnabled(std::uint32_t name) {
    return shenmue::s2::IsScenarioCharacterDisplayEnabled(shenmue::shen_name(name));
}

void __attribute__((renesas)) shenmue_s2_event_EV_EnableCharacterDisplay(std::uint32_t name) {
    shenmue::s2::EV_EnableCharacterDisplay(shenmue::shen_name(name));
}

void __attribute__((renesas)) shenmue_s2_event_EV_DisableCharacterDisplay(std::uint32_t name) {
    shenmue::s2::EV_DisableCharacterDisplay(shenmue::shen_name(name));
}

void __attribute__((renesas)) shenmue_s2_event_SetScenarioCharacterKillMode(std::uint32_t name, std::int32_t mode) {
    shenmue::s2::SetScenarioCharacterKillMode(shenmue::shen_name(name), mode);
}

void __attribute__((renesas)) shenmue_s2_event_EV_EnsureCharacterDraw(std::uint32_t name) {
    shenmue::s2::EV_EnsureCharacterDraw(shenmue::shen_name(name));
}

bool __attribute__((renesas)) shenmue_s2_event_GetScenarioCharacter(std::uint32_t name) {
    return shenmue::s2::IsScenarioCharacterActive(shenmue::shen_name(name));
}

bool __attribute__((renesas)) shenmue_s2_event_DoesScenarioCharacterExist(std::uint32_t name) {
    return shenmue::s2::DoesScenarioCharacterExist(shenmue::shen_name(name));
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_EV_SetCharacterAlphaEnabled(std::uint32_t name, std::int32_t enabled) {
    return shenmue::s2::EV_SetCharacterAlphaEnabled(shenmue::shen_name(name), enabled);
}

void __attribute__((renesas)) shenmue_s2_event_SetScenarioCharacterTransparency(std::uint32_t name, float alpha) {
    shenmue::s2::SetScenarioCharacterTransparency(shenmue::shen_name(name), alpha);
}
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" {
__attribute__((renesas)) shenmue::s2::Character *shenmue_s2_event_ctrlGetCharaByName(std::uint32_t name) {
    return shenmue::s2::ctrlGetCharaByName(shenmue::shen_name(name));
}
__attribute__((renesas)) bool shenmue_s2_event_CheckIsArgEquZero(std::int32_t value) {
    return shenmue::s2::CheckIsArgEquZero(value);
}
__attribute__((renesas)) void shenmue_s2_event_SetScenarioCharacterDrawControl(std::uint32_t name, std::int32_t mode, std::int32_t argument) {
    shenmue::s2::SetScenarioCharacterDrawControl(shenmue::shen_name(name), mode, argument);
}
__attribute__((renesas)) shenmue::s2::ReflectorBoardWork *shenmue_s2_event_SetScenarioReflectorBoardValues(std::uint32_t name, const shenmue::Vector4 *values) {
    return shenmue::s2::SetScenarioReflectorBoardValues(shenmue::shen_name(name), values);
}
}
#endif

#endif
