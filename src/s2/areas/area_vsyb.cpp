#include "s2/areas/area_vsyb.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/osage.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2 {
// @verified - p
void VsybInit() {
}
// @verified - p
void VsybMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(VsybMainCallback, 5, 0x0b, sizeof(VsybEventWork), VSYB_EVENT_TASK);
    VsybEventWork *const work = static_cast<VsybEventWork *>(HLib::GetTaskParameter(task));
    HLib::SetupTimedTask(task, 5);
    SetRenderClockAndCompassTo(1);
    work->self = task;
    work->state = 0;
    work->elapsedFrames = 0;
    work->durationFrames = 6;
    SetWeatherUpdateEnabledTo(0);
    EV_setHoldStatesFlags(0);
}
// @verified - p
void VsybMainCallback(void *parameter) {
    VsybEventWork *const work = static_cast<VsybEventWork *>(parameter);
    if (++work->elapsedFrames < work->durationFrames)
        return;
    switch (work->state) {
    case 0:
        work->characters = Queue_EVT_AT_ReadChr(work->self, VSYB_CHARACTERS);
        work->state = 1;
        break;
    case 1:
        InitializeVsybResources(work);
        work->state = 2;
        break;
    case 2:
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        QueueSkAuthorStart(work->self, static_cast<std::int32_t>(VSYB_OPENING_AUTHOR.name), nullptr, nullptr, 0);
        SetCharacterOsageMode(FindCharacterByScenarioName(VSYB_REN), 4);
        work->elapsedFrames = 0;
        work->durationFrames = 0;
        work->state = 4;
        break;
    case 4:
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        PrepareVsybBattle();
        HLib::SetCurrentTaskTo(VsybBattleCallback);
        work->durationFrames = 80;
        work->elapsedFrames = 0;
        break;
    case 7:
        flddSetPointFlagOnGrid(VSYB_BATTLE_POINT, 0);
        EV_stepSetFlags(0, 0x7d);
        QueueSkAuthorStart(work->self, static_cast<std::int32_t>(VSYB_WIN_AUTHOR.name), nullptr, nullptr, 0);
        work->elapsedFrames = 0;
        work->durationFrames = 0;
        work->state = 11;
        break;
    case 9: {
        flddSetPointFlagOnGrid(VSYB_BATTLE_POINT, 0);
        EV_stepSetFlags(1, 0x7d);
        DateTime date;
        GetCurrentDateTime(date);
        date.hour += 2;
        SetCurrentDateTimeTo(date);
        EV_DisableCharacterDisplay(VSYB_YUAN);
        EV_DisableCharacterDisplay(VSYB_FIRST_OPPONENT);
        EV_DisableCharacterDisplay(VSYB_SECOND_OPPONENT);
        EV_DisableCharacterDisplay(VSYB_THIRD_OPPONENT);
        EV_DisableCharacterDisplay(VSYB_COH);
        QueueSkAuthorStart(work->self, static_cast<std::int32_t>(VSYB_LOSE_AUTHOR.name), nullptr, nullptr, 0);
        work->elapsedFrames = 0;
        work->state = 11;
        work->durationFrames = 30;
        snapAssignFrmCntFlags(1, 690);
        break;
    }
    case 11:
        work->durationFrames = 30;
        work->elapsedFrames = 0;
        work->state = 12;
        break;
    case 12:
        CleanupVsybEvent(work);
        break;
    default:
        break;
    }
}
// @verified - p
void InitializeVsybResources(VsybEventWork *work) {
    EV_DisableCharacterDisplay(VSYB_REN);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("VSYB"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("VSYB"), 0);
    EV_LoadCutsceneFromCurrentScene("0058A000.smp");
    EV_LoadAudio("0058A000.sbt", 0);
    EV_spmBeginSampleExecute();
    EV_SetWeatherInstruction(12, 0);
    flddSetPointFlagOnGrid(VSYB_BATTLE_POINT, 0);
    work->motion = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    EV_FreeAuthorPak(1, "0058");
    shenStartFGST(10, 10, 0, 0);
    shenInitFightData(FindCharacterByScenarioName(VSYB_PLAYER), nullptr, 0, 0, 0, 0);
    shenInitFightData(FindCharacterByScenarioName(VSYB_FIRST_OPPONENT), nullptr, 1, 1, 300, 2);
    shenInitFightData(FindCharacterByScenarioName(VSYB_SECOND_OPPONENT), nullptr, 1, 1, 300, 2);
}
// @verified - p
void PrepareVsybBattle() {
    flddSetPointFlagOnGrid(VSYB_BATTLE_POINT, 1);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(VSYB_LIGHT);
    PushGlobalLightingProgram();
    EV_EnableCharacterDisplay(VSYB_FIRST_OPPONENT);
    EV_EnableCharacterDisplay(VSYB_SECOND_OPPONENT);
    EVT_AT_SetPos(VSYB_PLAYER, VSYB_INVALID, {292.28F, 93.699997F, 277.54999F}, 1);
    EVT_AT_SetAngY(VSYB_PLAYER, VSYB_INVALID, 287.6055908203125F, 1);
    EVT_AT_SetPos(VSYB_FIRST_OPPONENT, VSYB_INVALID, {294.54999F, 93.699997F, 276.82999F}, 1);
    EVT_AT_SetAngY(VSYB_FIRST_OPPONENT, VSYB_INVALID, 107.6055908203125F, 1);
    EVT_AT_SetPos(VSYB_SECOND_OPPONENT, VSYB_INVALID, {294.26001F, 93.699997F, 278.34F}, 1);
    EVT_AT_SetAngY(VSYB_SECOND_OPPONENT, VSYB_INVALID, 68.2965087890625F, 1);
    EVT_AT_SetPos(VSYB_YUAN, VSYB_INVALID, {297.1000061035156F, 93.69999694824219F, 279.6000061035156F}, 0);
    EVT_AT_SetAngY(VSYB_YUAN, VSYB_INVALID, 67.5F, 0);
    EVT_AT_SetMot(VSYB_YUAN, 11265, 0, 1, 0x40000000U, 0);
    vfCopyPosFromEntity(0, nullptr, 293.5f, 273.0f, 6.0f);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800U}, 1);
    EV_DisableCharacterDisplay(VSYB_REN);
    ToggleScenarioCharacterFlag(VSYB_REN);
    EV_DisableCharacterDisplay(VSYB_WONG);
    ToggleScenarioCharacterFlag(VSYB_WONG);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(VSYB_PLAYER), FacialNodeOriginal);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(VSYB_YUAN), FacialNodeOriginal);
    SetScenarioCharacterType(VSYB_PLAYER, CharacterTypeDefault);
    SetScenarioCharacterType(VSYB_FIRST_OPPONENT, CharacterTypeDefault);
    SetScenarioCharacterType(VSYB_SECOND_OPPONENT, CharacterTypeDefault);
    SetScenarioCharacterType(VSYB_YUAN, CharacterTypeDefault);
    hndlSetFlags(FindCharacterByScenarioName(VSYB_PLAYER));
    hndlSetFlags(FindCharacterByScenarioName(VSYB_REN));
    hndlSetFlags(FindCharacterByScenarioName(VSYB_YUAN));
    hndlSetFlags(FindCharacterByScenarioName(VSYB_SECOND_OPPONENT));
    hndlSetFlags(FindCharacterByScenarioName(VSYB_COH));
    vfmQueueFightStart(3, 99, 1, 0, -1);
    vfmDispatchControl(FightControlOp::CameraParameter, shen_name{0}, 13, 1);
    EV_LoadAudio("0058A000.sbt", 1);
}
// @verified - p
void VsybBattleCallback(void *parameter) {
    VsybEventWork *const work = static_cast<VsybEventWork *>(parameter);
    if (IsFightActive() == 0) {
        EnsureScenarioCharacterDraw(VSYB_REN);
        EV_EnableCharacterDisplay(VSYB_WONG);
        EnsureScenarioCharacterDraw(VSYB_WONG);
        SetLowQualityHands(FindCharacterByScenarioName(VSYB_PLAYER));
        SetLowQualityHands(FindCharacterByScenarioName(VSYB_REN));
        SetLowQualityHands(FindCharacterByScenarioName(VSYB_YUAN));
        SetLowQualityHands(FindCharacterByScenarioName(VSYB_SECOND_OPPONENT));
        SetLowQualityHands(FindCharacterByScenarioName(VSYB_COH));
        work->state = GetBtlStatus(FindCharacterByScenarioName(VSYB_PLAYER)) == 3 ? 7 : 9;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(VsybMainCallback);
    }
    work->elapsedFrames = 0;
}
void CleanupVsybEvent(VsybEventWork *work) {
    spmFadeOut();
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("VSYB"), 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(VSYB_LIGHT);
    CheckIsArgEquZero(1);
    PushGlobalLightingProgram();
    DeleteMotion(work->motion);
    EV_FreeAuthorPak(0, "0058");
    ET_AT_KillWork(work->characters);
    EV_setHoldStatesFlags(1);
    EV_scenQueueLoad(g_actNumber, VSYB_RETURN_AREA, 60);
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
}
const ScriptAreaDefinition vsybScriptDefinition{VSYB_AREA, 0, VsybInit, VsybMain};
} // namespace shenmue::s2
