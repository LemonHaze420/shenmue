#include "s2/areas/area_0530.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include "s2/areas/numeric_event.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
#include "s2/world_render.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0530() {
}

// @verified - p
void Start0530(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0530, 4, 0x0b, sizeof(Area0530Parameter), AUTHOR_VIEWER_TASK);
    Area0530Parameter *const parent = static_cast<Area0530Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    if (g_currentMap.name == AREA_0530_RUNTIME_MAP.name) {
        parent->result = reinterpret_cast<std::int32_t *>(argument);
        parent->selection = std::bit_cast<std::uint32_t>(*parent->result);
    } else {
        parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0530_MOTION));
        parent->selection = 0;
        Queue_EVT_UTL_LoadNPCChara_3(AREA_0530_CHARACTERS, task);
    }
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(std::uintptr_t{1}));
    Setup0530Scene();
}

// @verified - p
void Update0530(void *taskData) {
    Area0530Parameter *const parent = static_cast<Area0530Parameter *>(taskData);
    if (parent->state == 1)
        Queue0530Author(parent);
    else if (parent->state == 4)
        Cleanup0530(parent);
}

// @verified - p
void Cleanup0530(Area0530Parameter *parent) {
    const Vector3 position{150.0f, 100.0f, 398.3999938964844f};
    const Vector3i rotation{0, 0x8000, 0};
    EV_FreeAuthorPak(0, AREA_0530_CONVERSATION);
    EV_CleanupMapControlTask();
    if (g_currentMap.name == AREA_0530_RUNTIME_MAP.name) {
        *parent->result = -1;
        mtwkRequestMotion(scenFindMotionwork(AREA_0530_RYO_), 0xf085, MotionRegion::LowerBody);
        EV_AT_SetPos(AREA_0530_RYO_, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(AREA_0530_RYO_, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0530_RYO_), 17, 0);
        SetMotionRegionFlag(scenFindMotionwork(AREA_0530_RYO_), 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0530_RYO_), 13, 0);
        HLib::DestroyTask(parent->self);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0530"), 0);
    } else {
        for (const shen_name *character = AREA_0530_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
            if (FindCharacterByScenarioName(*character)) {
                EV_npcLockUnlock(*character);
                EV_npcFree(*character);
            }
        }
        DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
        HLib::DestroyTask(parent->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

// @verified - p
std::int64_t Queue0530Author(Area0530Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0530AuthorTask, 4, 0x0b, sizeof(Area0530AuthorParameter), AUTHOR_VIEWER_TASK);
    Area0530AuthorParameter *const child = static_cast<Area0530AuthorParameter *>(HLib::GetTaskParameter(task));
    child->currentFrame = 0;
    child->parent = parent;
    EV_LoadAudio("0530A000", 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    ST_LoadTitleFade();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0530Author(Area0530AuthorParameter *child) {
    child->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->currentFrame == 0)
        Prepare0530Characters();
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0530Characters();
        child->parent->state = 4;
        if (g_currentMap.name != AREA_0530_RUNTIME_MAP.name)
            SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0530AuthorTask(void *taskData) {
    Update0530Author(static_cast<Area0530AuthorParameter *>(taskData));
}

// @verified - p
void Prepare0530Characters() {
    EV_EnableCharacterDisplay(AREA_0530_RYO_);
    EV_SetCharaControllerFlags(AREA_0530_RYO_, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0530_RYO_, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0530_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0530_RYO_, false);
    EV_EnableCharacterDisplay(AREA_0530_a199);
    EV_SetCharaControllerFlags(AREA_0530_a199, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0530_a199, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_0530_a199, false);
    EV_EnableCharacterDisplay(AREA_0530_a197);
    EV_SetCharaControllerFlags(AREA_0530_a197, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0530_a197, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_0530_a197, false);
    EV_EnableCharacterDisplay(AREA_0530_a271);
    EV_SetCharaControllerFlags(AREA_0530_a271, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0530_a271, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_0530_a271, false);
    EV_EnableCharacterDisplay(AREA_0530_a272);
    EV_SetCharaControllerFlags(AREA_0530_a272, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0530_a272, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_0530_a272, false);
    EV_EnableCharacterDisplay(AREA_0530_a273);
    EV_SetCharaControllerFlags(AREA_0530_a273, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0530_a273, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(AREA_0530_a273, false);
    for (const shen_name *character = AREA_0530_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        EV_SetCharaControllerFlags(*character, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(*character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0530Characters() {
    EnableCharDisplayAndSetMotionworkData(AREA_0530_RYO_);
    EV_EnableCharacterDisplay(AREA_0530_a199);
    EV_SetCharaControllerFlags(AREA_0530_a199, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0530_a199, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0530_a199, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_0530_a199, true);
    EV_EnableCharacterDisplay(AREA_0530_a197);
    EV_SetCharaControllerFlags(AREA_0530_a197, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0530_a197, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0530_a197, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_0530_a197, true);
    EV_EnableCharacterDisplay(AREA_0530_a271);
    EV_SetCharaControllerFlags(AREA_0530_a271, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0530_a271, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0530_a271, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_0530_a271, true);
    EV_EnableCharacterDisplay(AREA_0530_a272);
    EV_SetCharaControllerFlags(AREA_0530_a272, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0530_a272, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0530_a272, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_0530_a272, true);
    EV_EnableCharacterDisplay(AREA_0530_a273);
    EV_SetCharaControllerFlags(AREA_0530_a273, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0530_a273, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0530_a273, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_0530_a273, true);
    for (const shen_name *character = AREA_0530_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        EV_SetCharaControllerFlags(*character, UpdateHeight, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(*character, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(*character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void Setup0530Scene() {
    SetPlayerControlEnabled(0);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_SetWeatherInstruction(0x780, 0);
    if (g_currentMap.name == AREA_0530_RUNTIME_MAP.name) {
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("0530"), 0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("0530"), 0);
        EV_LoadCutsceneFromCurrentScene("0530A000");
    } else {
        HLib::EnqueueTaskWithoutParameter(Update0530Waves, 3, 2, static_cast<std::int32_t>(AUTHOR_VIEWER_TASK.name));
    }
    SetMapModelCommandRoutingEnabled(0, 4);
    EV_FreeAuthorPak(1, AREA_0530_CONVERSATION);
}

// @verified - p
void Update0530Waves(void *) {
    SetTextureAnimationUV(0.01f, 0.0f);
    ApplyTextureAnimationResource(6);
    SetTextureAnimationUV(0.0f, 0.015f);
    ApplyTextureAnimationResource(7);
}

const ScriptAreaDefinition area0530ScriptDefinition{.areaId = AREA_0530, .field04 = 0, .init = Initialize0530, .main = Start0530};

} // namespace shenmue::s2::areas
