#include "s2/areas/area_329e.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/pak.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
void Initialize329E() {
}

void Start329E(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update329E, 4, 0x0b, sizeof(Area329EParameter), AREA_329E_TASK);
    Area329EParameter *const parent = static_cast<Area329EParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = g_currentMap.name == AREA_329E_RETURN_MAP.name ? 1 : 0;
    const Vector3 center{123.38f, 92.8f, 184.34f};
    npcSetAreaName(AREA_329E_NPC_AREA, &center, 4.0f, 1);
    SetPlayerControlEnabled(0);
    if (g_currentMap.name == AREA_329E.name) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0329E000");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_329E_CHARACTERS, parent->self);
}

void Update329E(void *parameter) {
    Area329EParameter *const parent = static_cast<Area329EParameter *>(parameter);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update329EAuthorTask, 4, 0x0b, sizeof(Area329EAuthorParameter), AREA_329E_TASK);
        Area329EAuthorParameter *const child = static_cast<Area329EAuthorParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        child->frame = 0;
        EV_NPC_NewLoadSw(1);
        Prepare329ECharacters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_329E_AUTHOR.name));
        parent->state = 2;
    } else if (parent->state == 3) {
        parent->state = 4;
    } else if (parent->state == 4) {
        Cleanup329E(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64 || input == 128)
            parent->selection = 0;
        if (parent->selection == 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen("0329E000.c\n");
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

void Cleanup329E(Area329EParameter *parent) {
    SetPlayerControlEnabled(1);
    EV_npcLockUnlock(AREA_329E_GAI);
    npcSetAreaName(AREA_329E_NPC_AREA, nullptr, 0.0f, 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0329E"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_329E_RETURN_MAP.name) {
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
    } else {
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}

std::int64_t Update329EAuthor(Area329EAuthorParameter *child) {
    using enum CameraState;
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (result == 0) {
        if (g_currentMap.name == AREA_329E_RETURN_MAP.name) {
            child->parent->state = 3;
            EventSetPosition position{0, AREA_329E_RYO, shen_name{0xffffffffU}, {122.01f, 92.8f, 184.34f}, 0.0f};
            EVT_AT_SetPosRot(&position);
            EventSetRawAngle angle{0, AREA_329E_RYO, shen_name{0xffffffffU}, {0, 49978, 0}, 0.0f};
            EVT_AT_SetAng_Callback(&angle);
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(Init);
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            InitiateCleanupForSceneChara(AREA_329E_GAI);
            HLib::SetupTimedTask(child->parent->self, 30);
        } else {
            child->parent->state = 0;
            SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        }
        Restore329ECharacters();
        return HLib::DestroyCurrentTask();
    }
    return result;
}
void Update329EAuthorTask(void *parameter) {
    Update329EAuthor(static_cast<Area329EAuthorParameter *>(parameter));
}

void Prepare329ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_329E_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_329E_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_329E_GAI), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_329E_GAI, false);
    EV_SetCharacterAlphaEnabled(AREA_329E_PROP0, false);
    EV_SetCharacterAlphaEnabled(AREA_329E_PROP1, false);
}
void Restore329ECharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_329E_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_329E_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_329E_GAI), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_329E_GAI, false);
    EV_SetCharacterAlphaEnabled(AREA_329E_PROP0, true);
    EV_SetCharacterAlphaEnabled(AREA_329E_PROP1, true);
}
const ScriptAreaDefinition area329EScriptDefinition{.areaId = AREA_329E, .field04 = 0, .init = Initialize329E, .main = Start329E};
} // namespace shenmue::s2::areas
