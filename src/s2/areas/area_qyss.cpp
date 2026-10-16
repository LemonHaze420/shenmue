#include "s2/areas/area_qyss.hpp"
#include "s2/scene_data.hpp"
#include "s2/map_control.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/elevator.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/motion.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {
std::int32_t qyssEntryMode{};
// @verified - p
void QyssInit() {
    qyssEntryMode = 0;
}
// @verified - p
void QyssDormantNullsub() {
}
void QyssMain(std::uintptr_t argument) {
    qyssEntryMode = static_cast<std::int32_t>(argument);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    QyssDormantNullsub();
    SetPlayerControlEnabled(0);
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), QYSS_EVENT_TASK);
    (void)RegisterEventTask(task);
    StepNpcClearWork *const work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = QueueQyssStalkCallback;
    work->callbackData = nullptr;
    work->reserved10 = 0;
    work->state = 0;
    work->delay = 3.0F;
}
// @verified - p
void QueueQyssStalkCallback(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QyssStalkCallback, 5, 0x0b, sizeof(QyssStalkWork), QYSS_EVENT_TASK);
    QyssStalkWork *const work = static_cast<QyssStalkWork *>(HLib::GetTaskParameter(task));
    work->authorState = 0;
    work->state = 0;
    work->reserved0c = 0;
    work->characters = nullptr;
    work->self = task;
    work->authorState = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, QYSS_CHARACTERS);
}
// @verified - p
void QyssStalkCallback(void *parameter) {
    QyssStalkWork *const work = static_cast<QyssStalkWork *>(parameter);
    std::int32_t frames;
    switch (work->authorState) {
    case 0:
        frames = InitializeQyssStalk(work);
        break;
    case 1:
        frames = ChooseQyssWaiting(work);
        break;
    case 2:
        frames = FadeQyssWaiting(work);
        break;
    case 3:
        frames = PlayQyssElevatorAuthor(work);
        break;
    case 4:
        spmFadeOut();
        HLib::SetCurrentTaskTo(QyssEndStalkCallback);
        HLib::SetupTaskDelay(30);
        return;
    default:
        return;
    }
    if (frames > 0)
        HLib::SetupTaskDelay(frames);
}
// @verified - p
std::int32_t InitializeQyssStalk(QyssStalkWork *work) {
    (void)SetDoorSubjectEnabled(QYSS_ELEVATOR, 0);
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QYSS"));
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QYSS"), 0);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QYSS"), 0);
    (void)EV_LoadCutsceneFromCurrentScene(qyssEntryMode == 2 ? "0235A012" : "0235A000");
    (void)EV_LoadAudio("023AE000", 0);
    (void)EV_spmBeginSampleExecute();
    (void)EV_FreeAuthorPak(1, "0235");
    (void)CreateEventCharacter(CharacterCreationHighModel, nullptr, QYSS_BASE);
    (void)SetLowQualityHands(FindCharacterByScenarioName(QYSS_REN));
    (void)SetLowQualityHands(FindCharacterByScenarioName(QYSS_YUAN));
    if (!qyssEntryMode)
        work->authorState = 3;
    else if (qyssEntryMode == 1) {
        work->authorState = 3;
        work->state = 1;
    } else if (qyssEntryMode == 2) {
        work->authorState = 3;
        work->state = 2;
    }
    return 0;
}
// @verified - p
std::int32_t ChooseQyssWaiting(QyssStalkWork *work) {
    if (work->state) {
        if (work->state == 1) {
            work->authorState = shenGetCtrlSpeakIconFlag() == 149 ? 2 : 4;
            work->state = 0;
        }
        return 0;
    }
    EV_DisableCharacterDisplay(QYSS_YUAN);
    EventSetMotion ryoMotion{0, QYSS_PLAYER, 0xf002, 0, 1.0F, 0x40000000U, 0};
    EVT_AT_SetMot_Callback(&ryoMotion);
    (void)GetMotionFrameCount(0xf002);
    EventSetMotion renMotion{0, QYSS_REN, 0xf002, 0, 1.0F, 0x40000000U, 0};
    EVT_AT_SetMot_Callback(&renMotion);
    (void)GetMotionFrameCount(0xf002);
    EventSetPosition ryoPosition{0, QYSS_PLAYER, QYSS_INVALID, QYSS_PLAYER_WAIT_POSITION, 0};
    EVT_AT_SetPosRot(&ryoPosition);
    EventSetAngle ryoAngle{0, QYSS_PLAYER, QYSS_INVALID, 45.0F, 0};
    EVT_AT_SetAngY_Callback(&ryoAngle);
    EventSetPosition renPosition{0, QYSS_REN, QYSS_INVALID, QYSS_REN_WAIT_POSITION, 0};
    EVT_AT_SetPosRot(&renPosition);
    EventSetAngle renAngle{0, QYSS_REN, QYSS_INVALID, 225.0F, 0};
    EVT_AT_SetAngY_Callback(&renAngle);
    EVT_AT_SetCam(1000, QYSS_REN, QYSS_REN, 0, 0, 1);
    EVT_AT_SetCam(2000, QYSS_PLAYER, QYSS_PLAYER, 0, 0, 60);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), QYSS_EVENT_TASK);
    SkEventMessageWork *const message = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(task));
    message->message = "ここでユアンを待ってみるか？";
    message->frames = 60;
    QueueSkChoiceWait(work->self, 0, 149, 155, -1, 155, 0, 60);
    ++work->state;
    return 0;
}
// @verified - p
std::int32_t FadeQyssWaiting(QyssStalkWork *work) {
    if (!work->state) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        ++work->state;
        return 30;
    }
    if (work->state == 1) {
        (void)npcResetDateTimeIfNeeded();
        DateTime date;
        GetCurrentDateTime(date);
        date.hour = 19;
        date.minute = 0;
        SetCurrentDateTimeTo(date);
        (void)npcAdvanceTime(date.hour, date.minute);
        work->authorState = 3;
        work->state = 0;
        return 90;
    }
    return 0;
}
// @verified - p
std::int32_t PlayQyssElevatorAuthor(QyssStalkWork *work) {
    if (!work->state) {
        if (!EV_GetScreenFilmIsNotShowing())
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_EnableCharacterDisplay(QYSS_YUAN);
        QueueSkAuthorStart(work->self, static_cast<std::int32_t>(QYSS_AUTHOR_START.name), nullptr, nullptr, 0);
        ++work->state;
    } else if (work->state == 1) {
        if (!EV_GetScreenFilmIsNotShowing())
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        if (IsAuthorStateFlagSet(0))
            EV_SetAuthorActiveTo(1);
        QueueSkAuthorStart(work->self, static_cast<std::int32_t>(QYSS_AUTHOR_ELV1.name), QyssElevatorAuthorFrameCallback, nullptr, 0);
        EV_stepSetFlags(0, 276);
        work->authorState = 4;
        work->state = 0;
    } else if (work->state == 2) {
        if (!EV_GetScreenFilmIsNotShowing())
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 6);
        if (IsAuthorStateFlagSet(0))
            EV_SetAuthorActiveTo(1);
        QueueSkAuthorStart(work->self, static_cast<std::int32_t>(QYSS_AUTHOR_ELV2.name), nullptr, nullptr, 0);
        (void)SetAuthorStateFlag(0, 1);
        work->authorState = 4;
        work->state = 0;
    }
    return 0;
}
// @verified - p
void QyssElevatorAuthorFrameCallback(void *) {
    switch (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) {
    case 0:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 11);
        break;
    case 1:
        (void)SetElevatorIndicatorFlag(QYSS_ELEVATOR, 1, 1);
        (void)SetElevatorIndicatorFlag(QYSS_ELEVATOR, 3, 1);
        break;
    case 140:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 10);
        break;
    case 170:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 9);
        break;
    case 200:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 8);
        break;
    case 230:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 7);
        break;
    case 260:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 6);
        (void)SetElevatorIndicatorFlag(QYSS_ELEVATOR, 1, 0);
        (void)SetElevatorIndicatorFlag(QYSS_ELEVATOR, 3, 0);
        break;
    case 690:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 6);
        (void)SetElevatorIndicatorFlag(QYSS_ELEVATOR, 1, 1);
        (void)SetElevatorIndicatorFlag(QYSS_ELEVATOR, 2, 1);
        break;
    case 830:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 7);
        break;
    case 860:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 8);
        break;
    case 890:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 9);
        break;
    case 920:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 10);
        break;
    case 950:
        (void)SetCurrentElevatorFloor(QYSS_ELEVATOR, 11);
        break;
    case 980:
        (void)SetElevatorIndicatorFlag(QYSS_ELEVATOR, 1, 0);
        (void)SetElevatorIndicatorFlag(QYSS_ELEVATOR, 2, 0);
        break;
    case 1100:
        snapSetDesc(1);
        break;
    case 1340:
        snapSetDesc(0);
        break;
    default:
        break;
    }
}
void QyssEndStalkCallback(void *parameter) {
    QyssStalkWork *const work = static_cast<QyssStalkWork *>(parameter);
    if (qyssEntryMode == 2) {
        EventSetPosition yuanPosition{0, QYSS_YUAN, QYSS_INVALID, QYSS_YUAN_END_POSITION, 0};
        EVT_AT_SetPosRot(&yuanPosition);
        EventSetAngle yuanAngle{0, QYSS_YUAN, QYSS_INVALID, 270.0F, 0};
        EVT_AT_SetAngY_Callback(&yuanAngle);
        EventSetMotion yuanMotion{0, QYSS_YUAN, 0xf002, 0, 1.0F, 2, 0};
        EVT_AT_SetMot_Callback(&yuanMotion);
        (void)GetMotionFrameCount(0xf002);
    }
    (void)hndlSetFlags(FindCharacterByScenarioName(QYSS_YUAN));
    (void)hndlSetFlags(FindCharacterByScenarioName(QYSS_REN));
    (void)SetDoorSubjectEnabled(QYSS_ELEVATOR, 1);
    ET_AT_KillWork(work->characters);
    EV_CleanupMapControlTask();
    HLKillOtherHierarchyTask(work->self);
    SetScenarioCharacterKillMode(QYSS_BASE, 1);
    (void)EV_FreeAuthorPak(0, "0235");
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QYSS"), 0);
    scenFreeEvent("QYSS.EV3");
    (void)EV_NPC_NewLoadSw(1);
    (void)npcSetCycleTimeLimitExpiry(QYSS_CYCLE, 17);
    FinishLoading();
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2068, 0);
    SetScenarioFollowCharacter(shen_name{});
    if (qyssEntryMode == 2) {
        EV_stepSetFlags(0, 0x46c);
        (void)ExecuteCleanupFuncForAreaWithCachedScene();
        (void)EV_spmBeginSampleExecute();
        EventSetPosition ryoPosition{0, QYSS_PLAYER, QYSS_INVALID, QYSS_PLAYER_END_POSITION, 0};
        EVT_AT_SetPosRot(&ryoPosition);
        EventSetAngle ryoAngle{0, QYSS_PLAYER, QYSS_INVALID, 292.5F, 0};
        EVT_AT_SetAngY_Callback(&ryoAngle);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else {
        SetPlayerControlEnabled(0);
        EV_scenQueueLoad(g_actNumber, QYSS_QAE6, 0);
    }
}
const ScriptAreaDefinition qyssScriptDefinition{.areaId = AREA_QYSS, .field04 = 0, .init = QyssInit, .main = QyssMain};
} // namespace shenmue::s2
