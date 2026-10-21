#include "s2/areas/area_0175.hpp"
#include "s2/character.hpp"
#include "s2/pak.hpp"

#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/event_queued.hpp"
#include "s2/camera.hpp"
#include "s2/sound_program.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0175MainCallback(void *taskData);
void E0175LoopCallback(void *taskData);

// @verified - p
void E0175Init() {
}

// @verified - p
void E0175Main(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0175MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0175TaskParameter *const parent = static_cast<E0175TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = g_currentMap.name == AREA_0175_AREA.name ? (g_entryPoint > 1 ? 0 : g_entryPoint) : *reinterpret_cast<const std::int16_t *>(argument);
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, AREA_0175_CONVERSATION);
    EV_LoadCutsceneFromCurrentScene("0175A000");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0175_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0175Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0175_JOP_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0175_JOP_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0175_WON_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0175_WON_, false);
    EV_SetCharacterAlphaEnabled(AREA_0175_A396, false);
    EV_SetCharacterAlphaEnabled(AREA_0175_A417, false);
}

// @verified - p
void RestoreE0175Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0175_JOP_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0175_JOP_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0175_WON_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0175_WON_, true);
    EV_SetCharacterAlphaEnabled(AREA_0175_A396, true);
    EV_SetCharacterAlphaEnabled(AREA_0175_A417, true);
}

// @verified - p
void CleanupE0175(E0175TaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0175_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0175_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0175A"), 0);
    if (GetSoundProgramPhaseFlag(1))
        ResetSoundProgramPhaseFlags(1);
    else
        LoadCutsceneAndSoundProgramForArea();
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0175_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0175_AREA, g_entryPoint);
    else if (g_currentMap.name == AREA_0175_AB00.name)
        EV_scenQueueLoad(g_actNumber, AREA_0175_AR02, 26);
    else {
        EV_NPC_NewLoadSw(1);
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, Vector3{280.0f, 100.0f, 248.0f}, 0);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, Vector3i{0, 8192, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}

// @verified - p
void QueueE0175Loop(E0175TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0175LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    E0175LoopParameter *const child = static_cast<E0175LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetupE0175Characters();
    EV_spmBeginSampleExecute();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection == 0 ? AREA_0175_AUTHOR_A.name : AREA_0175_AUTHOR_B.name));
    parent->state = 2;
}

// @verified - p
void E0175MainCallback(void *taskData) {
    E0175TaskParameter *const parent = static_cast<E0175TaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0175Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0175(parent);
    }
}

// @verified - p
void E0175LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0175Characters();
    static_cast<E0175LoopParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0175ScriptDefinition{.areaId = AREA_0175_AREA, .field04 = 0, .init = E0175Init, .main = E0175Main};

} // namespace shenmue::s2::areas
