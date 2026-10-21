#include "s2/areas/area_0186.hpp"
#include "s2/character.hpp"

#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/pak.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
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

void E0186MainCallback(void *taskData);
void E0186LoopCallback(void *taskData);

// @verified - p
void E0186Init() {
}

// @verified - p
void ConfigureE0186Environment() {
    if (g_currentMap.name == AREA_0186_AREA.name) {
        DateTime time;
        GetCurrentDateTime(time);
        time.hour = 22;
        time.minute = 0;
        SetCurrentDateTimeTo(time);
    }
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0186");
    EV_LoadCutsceneFromCurrentScene("0186A000");
}

// @verified - p
void E0186Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0186MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = g_currentMap.name == AREA_0186_AREA.name && g_entryPoint != 0;
    parent->state = 1;
    ConfigureE0186Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0186_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0186Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0186_WON_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0186_WON_, false);
    EV_SetCharacterAlphaEnabled(AREA_0186_A126, false);
    EV_SetCharacterAlphaEnabled(AREA_0186_A142, false);
    EV_SetCharacterAlphaEnabled(AREA_0186_A506, false);
    EV_SetCharacterAlphaEnabled(AREA_0186_A504, false);
    EV_SetCharacterAlphaEnabled(AREA_0186_A648, false);
}

// @verified - p
void RestoreE0186Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0186_WON_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0186_WON_, true);
    EV_SetCharacterAlphaEnabled(AREA_0186_A126, true);
    EV_SetCharacterAlphaEnabled(AREA_0186_A142, true);
    EV_SetCharacterAlphaEnabled(AREA_0186_A506, true);
    EV_SetCharacterAlphaEnabled(AREA_0186_A504, true);
    EV_SetCharacterAlphaEnabled(AREA_0186_A648, true);
}

void CleanupE0186(AreaAuthorTaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0186_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0186A"), 0);
    EV_FreeAuthorPak(0, AREA_0186_CONVERSATION);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0186_AREA.name) {
        EV_scenQueueLoad(g_actNumber, AREA_0186_AREA, g_entryPoint);
    } else {
        EV_LoadCutsceneFromCurrentScene("AB00");
        EV_LoadAudio("AB00_000", 0);
        EV_NPC_NewLoadSw(1);
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {123.5999984741211f, 95.25f, 552.5999755859375f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 50917, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
    }
}

void QueueE0186Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0186LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    AreaAuthorLoopTaskParameter *const child = static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    AuthSceneCtrl author;
    if (parent->selection != 0) {
        EV_SetObjectFlagByIndex(11, 0);
        EV_SetObjectFlagByIndex(10, 1);
        DisableScenarioCharacterDisplay(AREA_0186_A126);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetMapModelDataDefaultVertexShader();
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        author = static_cast<AuthSceneCtrl>(AREA_0186_SECOND_AUTHOR.name);
    } else {
        SetupE0186Characters();
        EV_spmBeginSampleExecute();
        author = static_cast<AuthSceneCtrl>(AREA_0186_FIRST_AUTHOR.name);
    }
    EV_authSceneCtrl(author);
    EV_spmBeginSampleExecute();
    parent->state = 2;
}

// @verified - p
void E0186MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1)
        QueueE0186Loop(parent);
    else if (parent->state == 3)
        CleanupE0186(parent);
    else if (parent->state == 4) {
        parent->state = 1;
        if (EV_GetShenmueFlag(0x43f) != 0) {
            EV_stepSetFlags(0, 0x43f);
            EV_EventSetUpHelpIcon_0(0x20, 0);
            SetHelpIconState();
        }
    } else if (parent->state == 0) {
        parent->state = 4;
        HLib::SetupTimedTask(parent->self, 40);
        ST_LoadTitleFade();
    }
}

// @verified - p
void E0186LoopCallback(void *taskData) {
    if (GetNpcNewLoadState() == 1) {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
    }
    if (plymIsSearchBlocked())
        SetPlayerControlEnabled(0);
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent;
    if (parent->selection != 0) {
        RestoreE0186Characters();
        parent->state = 3;
    } else {
        parent->state = 0;
        ++parent->selection;
        shenUpdateCurrentDateTime();
        Queue_ST_LoadTitleFadeIn(AREA_0186_AB00);
        HLib::SetupTimedTask(parent->self, 130);
    }
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0186ScriptDefinition{.areaId = AREA_0186_AREA, .field04 = 0, .init = E0186Init, .main = E0186Main};

} // namespace shenmue::s2::areas
