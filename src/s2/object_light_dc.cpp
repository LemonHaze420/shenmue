#include "s2/object_light_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/character_alpha_dc.hpp"
#include "s2/tail_lamp_dc.hpp"
#include "shenmue.h"
#include "s2/system_modules_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/scenario_talk_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/search_state_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/lighting_dc.hpp"
#include <cmath>
#include "s2/telephone_cord_dc.hpp"
#include "common/matrix_stack.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/math.hpp"
#include <math.h>
#include <string.h>
namespace shenmue {
namespace dc {
namespace sdk {
void DrawSingleLightMT7Mesh(void *mesh);
}
} // namespace dc
} // namespace shenmue
#include "s2/cold_archive_dc.hpp"
#include "s2/directory_cache.hpp"
#include "s2/event_overlay.hpp"
#include "s2/frame_dc.hpp"
#include "s2/initialization_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/arcade_rankings_dc.hpp"
#include "s2/texture_dc.hpp"
#include "platform/dc/weather_texture.hpp"
#include "s2/scene_data.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include "s2/stream_audio_dc.hpp"
#include <cstdio>
#include "s2/character_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/map_models_dc.hpp"
void SetMirroredModelDrawMode(std::int32_t enabled);
#include "s2/event_character_state_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/prop_dc.hpp"

namespace shenmue {
namespace s2 {

void DrawKnifeCharacter(Character *character);
std::uint32_t objectDrawLightEnabled;
std::int32_t objectDrawLightKind;
Vector4 objectDrawLightColour;
Vector3 objectDrawLightDirection, objectDrawLightPosition = {};
float objectDrawLightRange, objectDrawLightSceneIntensity, objectDrawLightFalloff = 0.0f;

// @verified - d
void UpdateCharacterAlpha(Character *character, void *payload) {
    CharacterAlphaState *const state = static_cast<CharacterAlphaState *>(payload);
    Vector3 world, position;
    if (HasCharacterMotionWork(character)) {
        if (camCtrlGetNpcRadius() < character->drawDistance) {
            float alpha = character->alpha - characterAlphaFadeStep;
            if (alpha < 0.0f)
                alpha = 0.0f;
            character->alpha = alpha;
            state->holdFrames = -1;
            return;
        }
        const float *src = &character->viewPosition.x;
        float *dst = &position.x;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst = *src;
    } else {
        GetCharacterWorldPosition(character, &world);
        Matrix4x4::TransformPoint(world, position);
    }
    float forwardRange = 0.75f;
    const float sideRange = forwardRange;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const shen_name name = scenGetCharaName(control->scenarioTask, character);
    if (name == srchCheckFlags(6))
        forwardRange = -1.0f;
    if (IsScenarioCharacterForcedVisible(name))
        forwardRange = -1.0f;
    if (position.z < forwardRange && position.z > -forwardRange && position.x > -sideRange && position.x < sideRange) {
        float alpha = position.z / forwardRange * 0.75f;
        alpha = (-alpha - 0.25f) * 2.0f;
        if (state->holdFrames > 0) {
            if (alpha < 0.9f)
                --state->holdFrames;
        } else if (state->holdFrames < 0) {
            if (alpha > character->alpha)
                alpha = character->alpha + characterAlphaFadeStep;
            else
                state->holdFrames = characterAlphaHoldFrames;
        } else {
            alpha = character->alpha - characterAlphaFadeStep;
        }
        if (alpha > 1.0f)
            alpha = 1.0f;
        if (alpha < 0.0f)
            alpha = 0.0f;
        character->alpha = alpha;
        return;
    }
    float alpha = character->alpha + characterAlphaFadeStep;
    if (alpha > 1.0f)
        alpha = 1.0f;
    if (forwardRange < 0.0f)
        alpha = 1.0f;
    character->alpha = alpha;
    state->holdFrames = -1;
}

// @verified - d
void TailLampTaskCallback(void *payload) {
    TailLampDrawWork *const draw = static_cast<TailLampDrawWork *>(payload);
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::LoadMatrix(&draw->transform);
    charaPrepareNodesForDraw(draw->root, draw->alpha);
    PopCurrentMatrix(1);
    draw->alpha += draw->fadeDelta;
    if (!(draw->alpha > 0.0f))
        HLib::DestroyCurrentTask();
}

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM || SHENMUE_TARGET_S2_DC_JP_1001
// @verified - d
void DrawKnifeModel(const MT7Node *node) {
    const std::int32_t previousLight = GetCurrentActiveLight();
    if (previousLight != 2)
        SetCurrentActiveLight(2);
    const std::int32_t light = FindUnusedSelectedLight();
    matrix_stack::Matrix4x4_Push(0);
    dc::sdk::PushGraphicsWorkspace();
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    if (node->mesh)
        dc::sdk::DrawSingleLightMT7Mesh(node->mesh);
    const MT7Node *child = node->child;
    if (child) {
        node = child;
        Matrix4x4 projection;
        if (light != 17) {
            ResetSelectedLightEntry(light);
            SetLightKind(light, objectDrawLightKind);
            shenSetLightColour(light, objectDrawLightColour);
            SetSelectedLightSceneIntensity(light, objectDrawLightSceneIntensity);
            SetSelectedLightPosition(light, objectDrawLightPosition);
            SetSelectedLightDirection(light, objectDrawLightDirection);
            SetSelectedLightRange(light, objectDrawLightRange);
            SetSelectedLightFalloff(light, objectDrawLightFalloff);
            SetSelectedLightEnabled(light, objectDrawLightEnabled);
            camCtrlGetProjectionMatrix(&projection);
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrix(&projection);
            PublishGlobalLighting();
            PopCurrentMatrix(1);
        }
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->mesh)
            dc::sdk::DrawSingleLightMT7Mesh(node->mesh);
        if (light != 17) {
            ResetSelectedLightEntry(light);
            matrix_stack::LoadMatrix(&projection);
            PublishGlobalLighting();
        }
    }
    if (previousLight != 2)
        SetCurrentActiveLight(previousLight);
    PopCurrentMatrix(1);
    dc::sdk::PopGraphicsWorkspace();
}
#endif

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM || SHENMUE_TARGET_S2_DC_JP_1001
// @verified - d
void AllocateKnifeComponent(Character *character) {
    charaAssignCoreCallback(character, DrawKnifeCharacter);
}
#endif

// @verified - d
void SetObjectDrawLightEnabled(std::uint32_t enabled) {
    objectDrawLightEnabled = enabled;
}

// @verified - d
void SetObjectDrawLightKind(std::int32_t kind) {
    objectDrawLightKind = kind;
}

// @verified - d
void SetObjectDrawLightColour(const Vector4 *colour) {
    objectDrawLightColour.x = colour->x;
    objectDrawLightColour.y = colour->y;
    objectDrawLightColour.z = colour->z;
    objectDrawLightColour.w = colour->w;
}

// @verified - d
void SetObjectDrawLightSceneIntensity(float intensity) {
    objectDrawLightSceneIntensity = intensity;
}

// @verified - d
void SetObjectDrawLightDirection(const Vector3 *direction) {
    objectDrawLightDirection.x = direction->x;
    objectDrawLightDirection.y = direction->y;
    objectDrawLightDirection.z = direction->z;
}

// @verified - d
void SetObjectDrawLightPosition(const Vector3 *position) {
    objectDrawLightPosition.x = position->x;
    objectDrawLightPosition.y = position->y;
    objectDrawLightPosition.z = position->z;
}

// @verified - d
void SetObjectDrawLightRange(float range) {
    objectDrawLightRange = range;
}

// @verified - d
void SetObjectDrawLightFalloff(float falloff) {
    objectDrawLightFalloff = falloff;
}

// @verified - d
void ResetObjectDrawLight() {
    Vector4 colour;
    colour.x = 1.0f;
    colour.y = 1.0f;
    colour.z = 1.0f;
    colour.w = 1.0f;
    SetObjectDrawLightEnabled(0);
    SetObjectDrawLightKind(3);
    SetObjectDrawLightSceneIntensity(1.0f);
    SetObjectDrawLightColour(&colour);
    SetObjectDrawLightRange(2.0f);
    SetObjectDrawLightFalloff(0.25f);
}

// @verified - d
void InitializeTelephoneCordGeometry(TelephoneCordGeometry *geometry, void *mesh, float length, std::int32_t count) {
    memset(geometry, 0, sizeof(*geometry));
    geometry->segmentCount = count;
    geometry->mesh = mesh;
    geometry->segmentLength = length;
    geometry->force.y = 100.0f;
}

// @verified - d
void AttachCharacterAlphaComponent(Character *character) {
    character->alpha = 0.0f;
    CharacterAlphaState *const state =
        static_cast<CharacterAlphaState *>(AllocateModelComponent(character, CHARACTER_ALPHA_MANAGER, sizeof(CharacterAlphaState), UpdateCharacterAlpha));
    state->holdFrames = -1;
}

// @verified - d
std::int32_t cpamSetFlag(Character *character, std::int32_t enabled) {
    if (character) {
        const std::int32_t previous = IsModelComponentEnabled(character, CHARACTER_ALPHA_MANAGER);
        SetFlagForToken(character, CHARACTER_ALPHA_MANAGER, enabled);
        if (!enabled)
            character->alpha = 1.0f;
        return previous;
    }
    return 0;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool shenCheckCharaHasAlphaManager(const Character *character) {
    return character ? IsModelComponentEnabled(character, CHARACTER_ALPHA_MANAGER) : false;
}
#endif

// @verified - d
void UpdateTailLamp(Character *character, void *payload) {
    TailLampWork *const lamp = static_cast<TailLampWork *>(payload);
    if (!character)
        return;
    if (lamp->active) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(TailLampTaskCallback, 13, 0, sizeof(TailLampDrawWork), MODEL_COMPONENT_LAMP);
        if (!task)
            return;
        TailLampDrawWork *const draw = static_cast<TailLampDrawWork *>(HLib::GetTaskParameter(task));
        draw->alpha = ((static_cast<float>(lamp->lifetime)) * (0.001953125f) + (0.25f));
        draw->fadeDelta = -0.037500001f;
        draw->root = lamp->model->root;
        CharacterNode *const node = GetCharacterNode(character);
        matrix_stack::PushUnitMatrix();
        if (node->parent)
            ApplyRecursiveCharacterNodeTransforms(node->parent);
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        matrix_stack::GetMatrix(&draw->transform);
        if (lamp->lightSlot != tailLampNoLightSlot) {
            Matrix4x4::preTranslate(0.0f, 0.5f, 1.2f);
            Vector3 position;
            matrix_stack::GetTranslation(&position);
            SetSelectedLightPosition(lamp->lightSlot, position);
            EnableSelectedLight(lamp->lightSlot);
        }
        matrix_stack::GetPreviousMatrix(0);
        Matrix4x4::MultiplyWithPrePassMatrix(&draw->transform);
        matrix_stack::GetMatrix(&draw->transform);
        PopCurrentMatrix(1);
    } else if (lamp->lightSlot != tailLampNoLightSlot) {
        DisableSelectedLight(lamp->lightSlot);
    }
}

// @verified - d
void CleanupTailLamp(Character *character, void *payload) {
    if (!character)
        return;
    TailLampWork *const lamp = static_cast<TailLampWork *>(payload);
    DisableSelectedLight(lamp->lightSlot);
    ReleaseMT7Model(lamp->model);
}

// @verified - d
void AttachTailLampComponent(Character *character) {
    TailLampWork *const lamp = static_cast<TailLampWork *>(AllocateModelComponent(character, MODEL_COMPONENT_LAMP, sizeof(TailLampWork), UpdateTailLamp));
    lamp->model = 0;
    lamp->lightSlot = 0;
    lamp->active = 0;
    lamp->lifetime = 0;
    lamp->model = InitializeMT7Model(shenLoadFile("model/object", "TALS500G.MT5", MODEL_COMPONENT_LAMP));
    lamp->lifetime = 128;
    if (GetCurrentActiveLight() == 1) {
        lamp->lightSlot = tailLampNoLightSlot;
    } else {
        Vector4 colour;
        colour.x = 1.0f;
        colour.y = 1.0f;
        colour.z = 0.0f;
        colour.w = 0.0f;
        lamp->lightSlot = tailLampLightSlot;
        ResetSelectedLightEntry(lamp->lightSlot);
        SetLightKind(lamp->lightSlot, 3);
        shenSetLightColour(lamp->lightSlot, colour);
        SetSelectedLightRange(lamp->lightSlot, 1.4f);
        DisableSelectedLight(lamp->lightSlot);
    }
    SetModelComponentCleanup(character, MODEL_COMPONENT_LAMP, CleanupTailLamp);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetTailLampActive(Character *character, std::uint32_t active) {
    TailLampWork *const lamp = static_cast<TailLampWork *>(charFindComp(character, MODEL_COMPONENT_LAMP));
    if (lamp)
        lamp->active = active;
}

void SetTailLampLifetime(Character *character, std::uint32_t lifetime) {
    TailLampWork *const lamp = static_cast<TailLampWork *>(charFindComp(character, MODEL_COMPONENT_LAMP));
    if (lamp && lifetime <= 256)
        lamp->lifetime = lifetime;
}
#endif

// @verified - d
void SetupRefboard() {
    ResetObjectDrawLight();
}

// @verified - d
void ResetColdRuntimeState() {
    ResetNewGameConfiguration();
    ResetNativePlayerStats();
    InitItems();
    texSetMotionFlag(0);
    MSTS_Reader(0, 0, 0);
    InitializeWeatherTextureSurfaces();
    InitializeNewGameCalendar();
    ResetArcadeRankings();
}

// @verified - d
void mstsInit() {
    ResetNativePlayerStats();
    texSetMotionFlag(0);
    MSTS_Reader(0, 0, 0);
}

} // namespace s2
} // namespace shenmue

#endif
