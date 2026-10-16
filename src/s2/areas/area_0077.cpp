#include "s2/areas/area_0077.hpp"
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
ConversationBuffer *e0077Voice;

void E0077MainCallback(void *taskData);
void E0077LoopCallback(void *taskData);

// @verified - p
void E0077Init() {
    e0077Voice = nullptr;
}

// @verified - p
void E0077Main(std::uintptr_t) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0077MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    if (g_currentMap.name == AREA_0077_WN00.name) {
        DateTime time;
        GetCurrentDateTime(time);
        parent->selection = time.hour < 19;
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        parent->selection = g_entryPoint != 0;
    }
    EV_stepSetFlags(0, 0x6f);
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0077A000");
    EV_FreeAuthorPak(1, "0077");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0077_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0077Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0077_JOP_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0077_JOP_, false);
    EV_SetCharacterAlphaEnabled(AREA_0077_A396, false);
    EV_SetCharacterAlphaEnabled(AREA_0077_A456, false);
}

// @verified - p
void RestoreE0077Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0077_JOP_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0077_JOP_, true);
    EV_SetCharacterAlphaEnabled(AREA_0077_A396, true);
    EV_SetCharacterAlphaEnabled(AREA_0077_A456, true);
}

// @verified - p
void CleanupE0077(AreaAuthorTaskParameter *parent) {
    SetNpcColdResetMode(1);
    for (const shen_name *character = AREA_0077_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0077_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0077A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0077_WN00.name) {
        DateTime time;
        GetCurrentDateTime(time);
        EV_SetModelDataDefaultVertexShader();
        if (time.hour < 19) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
        } else {
            EV_lightLoad(AREA_0077_LIGHT);
            PushGlobalLightingProgram();
        }
        LoadCutsceneAndSoundProgramForArea();
        EVT_AT_SetPos(AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, Vector3{246.4f, 104.5f, 195.2f}, 0);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, Vector3i{0, 15564, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0077Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0077LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    e0077Voice = shenPlayAudioWithFlags("0077C001", 1);
    EV_NPC_NewLoadSw(1);
    EV_spmBeginSampleExecute();
    SetupE0077Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0077_AUTHORS[parent->selection].name));
    parent->state = 2;
}

// @verified - p
void E0077MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0077Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0077(parent);
    }
}

// @verified - p
void E0077LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    if (GetAuthorFlags())
        ExecuteAicaCommand(0x1100a0, 4, 0);
    hsFadeVoice(e0077Voice, 30);
    HLib::SetupTimedTask(static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->self, 30);
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0077Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0077ScriptDefinition{.areaId = AREA_0077_AREA, .field04 = 0, .init = E0077Init, .main = E0077Main};

} // namespace shenmue::s2::areas
