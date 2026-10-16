#include "s2/areas/area_0036.hpp"
#include "s2/pak.hpp"
#include "s2/game_runtime.hpp"
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
#include "s2/load_title.hpp"
namespace shenmue::s2::areas {
namespace {
float e0036TearAlpha;
void E0036WaitAuthSkipCallback(void *data);
void FinishE0036Auth(void *data);
void E0036MainCallback(void *data);
void E0036LoopCallback(void *data);
// @verified - p
void E0036Init() {
    e0036TearAlpha = 0.0f;
}
// @verified - p
void E0036Main(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0036MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0036TaskParameter *const parent = static_cast<E0036TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->authorCommand = g_entryPoint;
    parent->state = 1;
    if (g_currentMap.name == AREA_0036_WTA0.name || g_entryPoint == 0)
        parent->authorCommand = AREA_0036_AREA.name;
    else {
        parent->authorCommand = AREA_0036_AUTHOR_0034.name;
        parent->state = 4;
    }
    EV_stepSetFlags(0, 44);
    EV_stepSetFlags(0, 45);
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    EV_FreeAuthorPak(1, "0034");
    EV_FreeAuthorPak(1, "0036");
    EV_LoadCutsceneFromCurrentScene("0036A000");
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0036_LIGHT_001);
    if (g_currentMap.name == AREA_0036_WTA0.name)
        EV_DisableCharacterDisplay(AREA_0036_LIGHT_0001);
    SetNewWindPhaseAndSpeed(0, 0.0f);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0036_CHARACTERS, parent->self);
}
void SetupE0036Characters(std::uint32_t authorCommand) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    const Vector3 zero{0, 0, 0};
    if (authorCommand == AREA_0036_AREA.name) {
        EV_SetCharacterAlphaEnabled(AREA_0036_A047, false);
        EV_SetCharacterAlphaEnabled(AREA_0036_A044, false);
        EV_SetCharacterAlphaEnabled(AREA_0036_A104, false);
        EV_SetCharacterAlphaEnabled(AREA_0036_A051, false);
        EV_SetCharacterAlphaEnabled(AREA_0036_AB08, false);

        mtwkSetExtraFlag(scenFindMotionwork(AREA_0036_SYP_), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0036_SYP_, false);
        EV_AT_SetPos(AREA_0036_KUN_, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    } else {
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0036_KUN_), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0036_KUN_, false);
        EV_SetCharacterAlphaEnabled(AREA_0036_A046, false);
        EV_DisableCharacterDisplay(AREA_0036_SYP_);
    }
}
// @verified - p
void RestoreE0036Characters(std::uint32_t authorCommand) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    if (authorCommand == AREA_0036_AREA.name) {
        EV_SetCharacterAlphaEnabled(AREA_0036_A047, true);
        EV_SetCharacterAlphaEnabled(AREA_0036_A044, true);
        EV_SetCharacterAlphaEnabled(AREA_0036_A104, true);
        EV_SetCharacterAlphaEnabled(AREA_0036_A051, true);
        EV_SetCharacterAlphaEnabled(AREA_0036_AB08, true);

        mtwkSetExtraFlag(scenFindMotionwork(AREA_0036_SYP_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0036_SYP_, true);
    } else {
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0036_KUN_), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0036_KUN_, true);
        EV_SetCharacterAlphaEnabled(AREA_0036_A046, true);
        EV_EnableCharacterDisplay(AREA_0036_SYP_);
    }
}
// @verified - p
void CleanupE0036(E0036TaskParameter *parent) {
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 8;
    time.minute = 55;
    SetCurrentDateTimeTo(time);
    for (const shen_name *character = AREA_0036_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0036A"), 0);
    EV_FreeAuthorPak(0, "0036");
    EV_FreeAuthorPak(0, "0034");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0036_WTA0.name)
        EV_scenQueueLoad(g_actNumber, AREA_0036_WBBK, 1);
    else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void QueueE0036Loop(E0036TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0036LoopCallback, 4, 0x0b, 0x8, AREA_AUTHOR_TASK);
    E0036LoopParameter *const child = static_cast<E0036LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0036Characters(parent->authorCommand);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->authorCommand));
    parent->state = 2;
}
// @verified - p
void E0036MainCallback(void *data) {
    E0036TaskParameter *const parent = static_cast<E0036TaskParameter *>(data);
    if (parent->state == 1)
        QueueE0036Loop(parent);
    else if (parent->state == 3)
        CleanupE0036(parent);
    else if (parent->state == 4) {
        SyncWindWithWDATWeatherState();
        shenUpdateCurrentDateTime();
        Queue_ST_LoadTitleFadeIn(AREA_0036_NEXT_DAY);
        HLib::SetupTimedTask(parent->self, 60);
        EV_SetModelDataDefaultVertexShader();
        EV_SetWeatherInstruction(12, 0);
        EV_lightLoad(AREA_0036_LIGHT_0000);
        PushGlobalLightingProgram();
        EV_LoadCutsceneFromCurrentScene("0034A000");
        parent->state = 5;
    } else if (parent->state == 5) {
        ST_LoadTitleFade();
        HLib::SetupTimedTask(parent->self, 60);
        parent->state = 1;
    }
}
// @verified - p
void E0036LoopCallback(void *data) {
    E0036LoopParameter *const child = static_cast<E0036LoopParameter *>(data);
    E0036TaskParameter *const parent = child->parent;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (parent->authorCommand == AREA_0036_AUTHOR_0034.name) {
        if (frame == 0 || frame == 161)
            SetChildObjectFlagByIndex(FindCharacterByScenarioName(AREA_AUTHOR_RYO), frame == 0 ? 2 : 0);
    } else if (parent->authorCommand == AREA_0036_AREA.name && frame == 776) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, 0x30, AREA_AUTHOR_TASK);
        RegisterEventTask(task);
        EventPlayDynamics *const dynamics = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
        dynamics->resource = "TEAR";
        dynamics->parts = AREA_0036_TEAR_PARTS;
        dynamics->character = AREA_0036_TEAR_CHARACTER;
        dynamics->endFrame = 96.0f;
        dynamics->trackCount = 8;
        dynamics->frameStep = 2.0f;
        dynamics->currentFrame = 4.0f;
        dynamics->delay = 0.0f;
    }
    if (parent->authorCommand == AREA_0036_AREA.name) {
        bool update = true;
        if (frame == 776)
            e0036TearAlpha = 1.0f;
        else if (static_cast<std::uint32_t>(frame - 777) <= 27)
            e0036TearAlpha += -0.034482758f;
        else if (frame == 805)
            e0036TearAlpha = 0.0f;
        else
            update = false;
        if (update)
            SetScenarioCharacterTransparency(AREA_0036_TEAR_MODEL, e0036TearAlpha);
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (GetAuthorFlags()) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(E0036WaitAuthSkipCallback);
        HLib::SetupTaskDelay(30);
    } else
        FinishE0036Auth(data);
}
// @verified - p
void FinishE0036Auth(void *data) {
    E0036TaskParameter *const parent = static_cast<E0036LoopParameter *>(data)->parent;
    RestoreE0036Characters(parent->authorCommand);
    if (g_currentMap.name == AREA_0036_WTA0.name && parent->authorCommand == AREA_0036_AREA.name) {
        parent->authorCommand = AREA_0036_AUTHOR_0034.name;
        parent->state = 4;
    } else {
        if (g_currentMap.name == AREA_0036_WTA0.name)
            SetChildObjectFlagByIndex(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 0);
        parent->state = 3;
    }
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
// @verified - p
void E0036WaitAuthSkipCallback(void *data) {
    FinishE0036Auth(data);
}
} // namespace
const ScriptAreaDefinition area0036ScriptDefinition{.areaId = AREA_0036_AREA, .field04 = 0, .init = E0036Init, .main = E0036Main};
} // namespace shenmue::s2::areas
