#include "s2/areas/area_0184.hpp"
#include "s2/character.hpp"

#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
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

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0184MainCallback(void *taskData);
void E0184LoopCallback(void *taskData);

// @verified - p
void E0184Init() {
}

// @verified - p
void E0184Main(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0184MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0184TaskParameter *const parent = static_cast<E0184TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->reserved08 = 0;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0184A000");
    EV_FreeAuthorPak(1, "0184");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0184_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0184Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0184_A191, false);
    EV_SetCharacterAlphaEnabled(AREA_0184_A192, false);
    for (const shen_name *character = AREA_0184_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0184Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0184_A191, true);
    EV_SetCharacterAlphaEnabled(AREA_0184_A192, true);
    for (const shen_name *character = AREA_0184_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

void CleanupE0184(E0184TaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0184_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0184_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0184A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0184_AREA.name) {
        EV_scenQueueLoad(g_actNumber, AREA_0184_AREA, g_entryPoint);
    } else {
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {103.4000015258789f, 100.0f, 204.42999267578125f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 49279, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        LoadCutsceneAndSoundProgramForArea();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}

// @verified - p
void QueueE0184Loop(E0184TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0184LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0184FrameParameter *const child = static_cast<E0184FrameParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->state = 0;
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    SetupE0184Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0184_AREA.name));
    parent->state = 2;
}

// @verified - p
void E0184MainCallback(void *taskData) {
    E0184TaskParameter *const parent = static_cast<E0184TaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0184Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0184(parent);
    }
}

// @verified - p
void E0184LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0184Characters();
    static_cast<E0184FrameParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0184ScriptDefinition{.areaId = AREA_0184_AREA, .field04 = 0, .init = E0184Init, .main = E0184Main};

} // namespace shenmue::s2::areas
