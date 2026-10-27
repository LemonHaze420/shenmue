#include "s2/areas/area_0140.hpp"
#include "s2/character.hpp"

#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/inventory.hpp"
#include "s2/npc.hpp"
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

void E0140MainCallback(void *taskData);
void E0140LoopCallback(void *taskData);

// @verified - p
void E0140Init() {
}

// @verified - p
void E0140Main(std::uintptr_t) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 1);
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{0});
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0140MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    shenSetClockAndCompassRenderTo(1);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, "0140");
    EV_LoadCutsceneFromCurrentScene("0140A000");
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0140_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0140Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0140_A010, false);
    EV_SetCharacterAlphaEnabled(AREA_0140_A396, false);
    EV_SetCharacterAlphaEnabled(AREA_0140_A546, false);
    EV_SetCharacterAlphaEnabled(AREA_0140_A548, false);
    for (const shen_name *character = AREA_0140_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0140Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0140_A010, true);
    EV_SetCharacterAlphaEnabled(AREA_0140_A396, true);
    EV_SetCharacterAlphaEnabled(AREA_0140_A546, true);
    EV_SetCharacterAlphaEnabled(AREA_0140_A548, true);
    for (const shen_name *character = AREA_0140_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

void CleanupE0140(AreaAuthorTaskParameter *parent) {
    using enum CameraState;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x8b4, 0);
    for (const shen_name *character = AREA_0140_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0140_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0140A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0140_AR02.name) {
        EV_NPC_NewLoadSw(1);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 0, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&angle);
        LoadCutsceneAndSoundProgramForArea();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        BeginSecondarySampleExecution(0);
        shenSetClockAndCompassRenderTo(0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0140Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0140LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0140Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0140_AREA.name));
    parent->state = 2;
}

// @verified - p
void E0140MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0140Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0140(parent);
    }
}

// @verified - p
void E0140LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    if (GetAuthorFlags() != 0)
        spmFadeOut();
    AddInventoryItem(0x1f4f, 1, 1);
    HLib::SetupTimedTask(static_cast<AreaAuthorFrameTaskParameter *>(taskData)->parent->self, 130);
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0140Characters();
    static_cast<AreaAuthorFrameTaskParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0140ScriptDefinition{.areaId = AREA_0140_AREA, .field04 = 0, .init = E0140Init, .main = E0140Main};

} // namespace shenmue::s2::areas
