#include "s2/areas/area_0076.hpp"
#include "s2/pak.hpp"
#include "s2/areas/area_0047.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
namespace {
void E0076MainCallback(void *);
void E0076LoopCallback(void *);
// @verified - p
void SetupE0076Environment() {
    SetRenderClockAndCompassTo(1);
    EV_SetNewSceneIndex();
    if (g_currentMap.name != AREA_0076_AR03.name) {
        DateTime time;
        GetCurrentDateTime(time);
        if (time.hour < 19) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
            EV_SetObjectFlagByIndex(4, 0);
        } else {
            EV_lightLoad(AREA_0076_LIGHT);
            PushGlobalLightingProgram();
        }
    }
    EV_FreeAuthorPak(1, "0076");
    EV_LoadCutsceneFromCurrentScene("0076A000");
}
// @verified - p
void E0076Main(std::uintptr_t) {
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{0});
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0076MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    SetupE0076Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0076_CHARACTERS, parent->self);
}
// @verified - p
void SetupE0076Characters() {
    Vector3 hiddenPosition{0, -1000, 1000};
    EV_ForceLoadNpc(AREA_0076_PIN);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0076_JOP), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0076_JOP, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0076_GOI), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0076_GOI, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0076_HTK), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0076_HTK, false);
    EV_SetCharacterAlphaEnabled(AREA_0076_AA39, false);
    EV_SetCharacterAlphaEnabled(AREA_0076_A016, false);
    EV_SetCharacterAlphaEnabled(AREA_0076_AA44_FIRST, false);
    EV_SetCharacterAlphaEnabled(AREA_0076_AA44_SECOND, false);
    EV_SetCharacterAlphaEnabled(AREA_0076_A060, false);
    if (EV_GetShenmueFlag(0xff) == 0) {
        EV_scenApplyPosOrRotToPartOf(AREA_0076_AA39, 0xca, &hiddenPosition, nullptr, EventCharacterPartTransformMode::Absolute);
        EV_scenApplyPosOrRotToPartOf(AREA_0076_AA39, 0xcb, &hiddenPosition, nullptr, EventCharacterPartTransformMode::Absolute);
    }
}
// @verified - p
void RestoreE0076Characters() {
    EV_npcLockUnlock(AREA_0076_PIN);
    SetPluralControlFor(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 0);
    EV_EnsureCharacterDraw(AREA_AUTHOR_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    SetPluralControlFor(FindCharacterByScenarioName(AREA_0076_JOP), 0);
    EV_EnsureCharacterDraw(AREA_0076_JOP);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0076_JOP), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0076_JOP, true);
    SetPluralControlFor(FindCharacterByScenarioName(AREA_0076_GOI), 0);
    EV_EnsureCharacterDraw(AREA_0076_GOI);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0076_GOI), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0076_GOI, true);
    SetPluralControlFor(FindCharacterByScenarioName(AREA_0076_HTK), 0);
    EV_EnsureCharacterDraw(AREA_0076_HTK);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0076_HTK), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0076_HTK, true);
    EV_SetCharacterAlphaEnabled(AREA_0076_AA39, true);
    EV_SetCharacterAlphaEnabled(AREA_0076_A016, true);
    EV_SetCharacterAlphaEnabled(AREA_0076_AA44_FIRST, true);
    EV_SetCharacterAlphaEnabled(AREA_0076_AA44_SECOND, true);
    EV_SetCharacterAlphaEnabled(AREA_0076_A060, true);
}
// @verified - p
void CleanupE0076(AreaAuthorTaskParameter *parent) {
    using enum CameraState;
    for (const shen_name *character = AREA_0076_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0076");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0076A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == AREA_0076_AR03.name) {
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {300.4700012207031f, 100, 485.17999267578125f}, 0};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 16566, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        LoadCutsceneAndSoundProgramForArea();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetRenderClockAndCompassTo(0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void E0076MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0076LoopCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
        static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
        EV_spmBeginSampleExecute();
        SetupE0076Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0076_AUTHOR.name));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0076(parent);
}
// @verified - p
void E0076LoopCallback(void *taskData) {
    const std::int32_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    switch (frame) {
    case 1:
        camCtrlSetFarDistance(40);
        camCtrlSetNearDistance(40);
        SetPluralControlFor(FindCharacterByScenarioName(AREA_0076_JOP), 1);
        SetPluralControlFor(FindCharacterByScenarioName(AREA_0076_GOI), 1);
        SetPluralControlFor(FindCharacterByScenarioName(AREA_0076_HTK), 1);
        SetPluralControlFor(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 1);
        EV_ToggleCharacterFlag(AREA_0076_JOP);
        EV_DisableCharacterDisplay(AREA_0076_JOP);
        break;
    case 1744: {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, 48, AREA_AUTHOR_TASK);
        RegisterEventTask(task);
        EventPlayDynamics *const dynamics = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
        dynamics->resource = "beer";
        dynamics->parts = AREA_0047_DYNAMICS_PARTS;
        dynamics->character = AREA_0076_BEER_CHARACTER;
        dynamics->endFrame = 108.0f;
        dynamics->trackCount = 84;
        dynamics->frameStep = 2;
        dynamics->currentFrame = 2;
        dynamics->delay = 0;
        break;
    }
    case 2000:
        EV_EnsureCharacterDraw(AREA_0076_JOP);
        EV_EnableCharacterDisplay(AREA_0076_JOP);
        break;
    case 2380:
        EV_ToggleCharacterFlag(AREA_0076_HTK);
        EV_ToggleCharacterFlag(AREA_0076_GOI);
        EV_DisableCharacterDisplay(AREA_0076_HTK);
        EV_DisableCharacterDisplay(AREA_0076_GOI);
        break;
    case 2450:
        EV_ToggleCharacterFlag(AREA_0076_PIN);
        break;
    default:
        break;
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    EV_EnsureCharacterDraw(AREA_0076_PIN);
    RestoreE0076Characters();
    static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent->state = 3;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0076ScriptDefinition{.areaId = AREA_0076_AREA, .field04 = 0, .init = nullptr, .main = E0076Main};
} // namespace shenmue::s2::areas
