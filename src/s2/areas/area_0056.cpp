#include "s2/areas/area_0056.hpp"
#include "s2/pak.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_queued.hpp"
#include "s2/camera.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
namespace {
void E0056MainCallback(void *data);
void E0056LoopCallback(void *data);
// @verified - p
void E0056Init() {
}
// @verified - p
void E0056Main(std::uintptr_t argument) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0056MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0056TaskParameter *const parent = static_cast<E0056TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = g_currentMap.name == AREA_0056_AREA.name ? (g_entryPoint > 2 ? 0 : g_entryPoint) : *reinterpret_cast<const std::uint32_t *>(argument);
    parent->state = 1;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 1);
    EV_SetNewSceneIndex();
    if (g_currentMap.name == AREA_0056_AREA.name) {
        DateTime time;
        GetCurrentDateTime(time);
        if (time.hour < 19) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
        } else {
            EV_lightLoad(AREA_0056_LIGHT);
            PushGlobalLightingProgram();
        }
    }
    EV_LoadCutsceneFromCurrentScene("0056A000");
    EV_FreeAuthorPak(1, "0056");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0056_CHARACTERS, parent->self);
}
// @verified - p
void SetupE0056Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0056_A358, false);
    EV_SetCharacterAlphaEnabled(AREA_0056_A118, false);
    for (const shen_name *character = AREA_0056_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}
// @verified - p
void RestoreE0056Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0056_A358, true);
    EV_SetCharacterAlphaEnabled(AREA_0056_A118, true);
    for (const shen_name *character = AREA_0056_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}
// @verified - p
void CleanupE0056(E0056TaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0056_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    EV_NPC_NewLoadSw(1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 0);
    EV_FreeAuthorPak(0, "0056");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0056A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0056_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0056_AREA, g_entryPoint);
    else if (parent->selection == 1 || parent->selection == 2)
        EV_scenQueueLoad(g_actNumber, AREA_0056_ARKQ, 0);
    else if (parent->selection == 0) {
        LoadCutsceneAndSoundProgramForArea();
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, Vector3{289.78f, 95.3f, 590.3f}, 0);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, Vector3i{0, 364, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
    }
}
// @verified - p
void QueueE0056Loop(E0056TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0056LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0056LoopParameter *const child = static_cast<E0056LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->frame = 0;
    EV_LoadAudio("0056A000", parent->selection != 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    SetupE0056Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0056_AUTHORS[parent->selection].name));
    parent->state = 2;
}
// @verified - p
void E0056MainCallback(void *data) {
    E0056TaskParameter *const parent = static_cast<E0056TaskParameter *>(data);
    if (parent->state == 1)
        QueueE0056Loop(parent);
    else if (parent->state == 3)
        CleanupE0056(parent);
}
// @verified - p
void E0056LoopCallback(void *data) {
    E0056LoopParameter *const child = static_cast<E0056LoopParameter *>(data);
    E0056TaskParameter *const parent = child->parent;
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (parent->selection != 0) {
        RestoreE0056Characters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        parent->state = 3;
    } else if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= 500) {
        EV_stepSetFlags(1, 120);
        parent->selection = 1;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0056_AUTHORS[1].name));
        EV_LoadAudio("0056A000", 1);
        return;
    } else {
        EV_stepSetFlags(0, 120);
        parent->state = 3;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTimedTask(parent->self, 30);
    }
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0056ScriptDefinition{.areaId = AREA_0056_AREA, .field04 = 0, .init = E0056Init, .main = E0056Main};
} // namespace shenmue::s2::areas
