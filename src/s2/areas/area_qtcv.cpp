#include "s2/areas/area_qtcv.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/carry_event.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <cstring>
namespace shenmue::s2 {
// @verified - p
void QtcvInit() {
}
// @verified - p
void QtcvDormantBoundary() {
}
// @verified - p
void QtcvMain(std::uintptr_t configuration) {
    QueueQtcvEvent(reinterpret_cast<const QtcvConfiguration *>(configuration));
}
// @verified - p
void QueueQtcvEvent(const QtcvConfiguration *configuration) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QtcvStartCallback, 4, 0x0b, sizeof(QtcvEventWork), QTCV_TASK);
    QtcvEventWork *const work = static_cast<QtcvEventWork *>(HLib::GetTaskParameter(task));
    work->unused08 = 0;
    work->task = task;
    work->configuration = configuration ? configuration : &QTCV_CONFIGURATIONS[g_entryPoint];
    InitializeQtcvResources(work);
}
// @verified - p
void InitializeQtcvResources(QtcvEventWork *work) {
    work->dialogue = -1;
    for (std::int32_t flag = 331; flag <= 333; ++flag)
        if (!GetShenmueFlag(flag))
            ++work->dialogue;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    Vector3 position;
    EV_GetCharaPosition(QTCV_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    work->playerHeight = position.y;
    work->changedFlag330 = 0;
    work->placement = &QTCV_PLACEMENTS[work->configuration->kind];
    work->characters = Queue_EVT_AT_ReadChr(work->task, QTCV_LOAD_CHARACTERS);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("301E"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QTCV"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QTCV"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QTCV"), 0);
    EV_FreeAuthorPak(1, "0301");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0301E000");
    SetRenderClockAndCompassTo(1);
    QtcvDormantBoundary();
    SetPluralControlFor(FindCharacterByScenarioName(QTCV_PLAYER), 1);
    SetPluralControlFor(FindCharacterByScenarioName(QTCV_REN), 1);
}
// @verified - p
void QtcvStartCallback(void *data) {
    QtcvEventWork &work = *static_cast<QtcvEventWork *>(data);
    work.authorContinuation = QtcvEndThunkCallback;
    EVT_AT_SetPos(QTCV_STAGE, QTCV_INVALID, {work.placement->position.x, work.playerHeight, work.placement->position.z}, 0);
    EVT_AT_SetAngY(QTCV_STAGE, QTCV_INVALID, work.placement->yaw, 0);
    EVT_AT_SetPos(QTCV_SECOND_STAGE, QTCV_INVALID, {}, 0);
    EVT_AT_SetAngY(QTCV_SECOND_STAGE, QTCV_INVALID, 0, 0);
    SetLowQualityHands(FindCharacterByScenarioName(QTCV_PLAYER));
    SetLowQualityHands(FindCharacterByScenarioName(QTCV_REN));
    SetHighModelXYZ(FindCharacterByScenarioName(QTCV_PLAYER), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(QTCV_PLAYER), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QTCV_PLAYER), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QTCV_PLAYER), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    SetHighModelXYZ(FindCharacterByScenarioName(QTCV_REN), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(QTCV_REN), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QTCV_REN), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QTCV_REN), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    AT_SetLocalBaseChara(QTCV_DUMMY_PLAYER, QTCV_PLAYER);
    AT_SetLocalBaseChara(QTCV_DUMMY_REN, QTCV_REN);
    EVT_AT_SetPos(QTCV_DUMMY_PLAYER, QTCV_PLAYER, {}, 0);
    EVT_AT_SetAngY(QTCV_DUMMY_PLAYER, QTCV_PLAYER, 0, 0);
    EVT_AT_SetPos(QTCV_DUMMY_REN, QTCV_REN, {}, 0);
    EVT_AT_SetAngY(QTCV_DUMMY_REN, QTCV_REN, 0, 0);
    EVT_AT_SetPos(QTCV_PLAYER, QTCV_STAGE, {}, 0);
    EVT_AT_SetPos(QTCV_REN, QTCV_STAGE, {}, 0);
    EV_NPC_NewLoadSw(1);
    const std::int32_t frames =
        static_cast<std::int32_t>(work.configuration->tail ? PlayPlankCarryAuthoredTransitionTail(work.configuration->kind, work.dialogue) :
                                                             PlayPlankCarryAuthoredTransition(work.configuration->kind, work.dialogue));
    if (GetShenmueFlag(330)) {
        work.changedFlag330 = 1;
        EV_stepSetFlags(1, 330);
        work.waitContinuation = QtcvAlternateAuthorCallback;
    } else
        work.waitContinuation = QtcvEndThunkCallback;
    EV_LoadAudio("0301E000", 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_MenCtrl(EventFilmControl{0, 30, 0, 0, 0, 0, 255, 0, 0, 0, static_cast<float>(frames - 30)});
    HLib::SetCurrentTaskTo(QtcvNormalWaitCallback);
    work.remainingFrames = frames;
}
// @verified - p
void QtcvNormalWaitCallback(void *data) {
    QtcvEventWork &work = *static_cast<QtcvEventWork *>(data);
    if (authGetFinished() || (work.configuration->tail && HasAuthorCancelBeenPressed()) || (IsAuthorStateFlagSet(11) && HasAuthorCancelBeenPressed())) {
        EV_CleanupMapControlTask();
        work.remainingFrames = 0;
    }
    if (work.remainingFrames > 0)
        --work.remainingFrames;
    else
        HLib::SetCurrentTaskTo(work.waitContinuation);
}
// @verified - p
void QtcvAlternateAuthorCallback(void *data) {
    QtcvEventWork &work = *static_cast<QtcvEventWork *>(data);
    if (IsAuthorStateFlagSet(11))
        EV_SetAuthorActiveTo(1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, QTCV_AUTHOR_FLAGS[work.configuration->kind], 1);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QTCV_AUTHORS[work.configuration->kind].name));
    work.authorContinuation = QtcvAlternateFinishCallback;
    HLib::SetCurrentTaskTo(AuthoredEventContinuationAt18Callback);
}
// @verified - p
void AuthoredEventContinuationAt18Callback(void *data) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {

        HLib::TaskCallback continuation;
        std::memcpy(&continuation, static_cast<const std::byte *>(data) + 0x18, sizeof(continuation));
        HLib::SetCurrentTaskTo(continuation);
    }
}
// @verified - p
void QtcvAlternateFinishCallback(void *data) {
    QtcvEventWork &work = *static_cast<QtcvEventWork *>(data);
    EVT_AT_SetCam(7016, QTCV_SECOND_STAGE, QTCV_SECOND_STAGE, 0, 0, 0);
    camCtrlDisableFilmBars();
    Loop3DSoundEffect(0x0da8);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1320, 1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_MenCtrl(EventFilmControl{0, 60, 0, 0, 0, 0, 255, 0, 0, 0, 140});
    work.remainingFrames = 200;
    HLib::SetCurrentTaskTo(QtcvEnemyWaitCallback);
}
// @verified - p
void QtcvEnemyWaitCallback(void *data) {
    QtcvEventWork &work = *static_cast<QtcvEventWork *>(data);
    if (authGetFinished() || (IsAuthorStateFlagSet(11) && HasAuthorCancelBeenPressed())) {
        EV_CleanupMapControlTask();
        work.remainingFrames = 0;
    }
    const std::int32_t previous = work.remainingFrames;
    work.remainingFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(previous) - 1U);
    if (previous <= 0) {
        SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(QtcvEndThunkCallback);
    }
}
// @verified - p
void QtcvEndThunkCallback(void *data) {
    QtcvEndCallback(data);
}
// @verified - p
void QtcvEndCallback(void *data) {
    QtcvEventWork &work = *static_cast<QtcvEventWork *>(data);
    const std::int32_t changedFlag330 = work.changedFlag330;
    CleanupQtcvResources(&work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1320, 0);
    if (changedFlag330)
        EV_stepSetFlags(0, 330);
    if (g_currentMap.name == QTCV_AREA.name) {
        scenSetTaskFlags(6);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    } else
        RepositionPlayerForQronFloor(7);
}
// @verified - p
void CleanupQtcvResources(QtcvEventWork *work) {
    EVT_AT_SetPosAng(QTCV_PLAYER, QTCV_STAGE, 0, 2, 180, 0);
    SetPluralControlFor(FindCharacterByScenarioName(QTCV_PLAYER), 0);
    SetPluralControlFor(FindCharacterByScenarioName(QTCV_REN), 0);
    hndlSetFlags(FindCharacterByScenarioName(QTCV_REN));
    hndlSetFlags(FindCharacterByScenarioName(QTCV_PLAYER));
    FinishLoading();
    SetRenderClockAndCompassTo(0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0301");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QTCV"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("301E"), 0);
    ET_AT_KillWork(work->characters);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_NPC_NewLoadSw(1);
}
const ScriptAreaDefinition qtcvScriptDefinition{QTCV_AREA, 0, QtcvInit, QtcvMain};
} // namespace shenmue::s2
