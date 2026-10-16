#include "s2/qt_nj_event.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/snapshot.hpp"
#include "s2/system_modules.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2 {
namespace {
Matrix4x4 qtnjAuthorPlacement;
void QtnjStartCallback(void *);
void QtnjStartAuthorCallback(void *);
void QtnjPlayAuthorCallback(void *);
void QtnjNextTrialCallback(void *);
void QtnjStartRunCallback(void *);
void QtnjWaitRunCallback(void *);
void QtnjWaitQteCallback(void *);
void QtnjCheckAuthorCallback(void *);
void QtnjCheckTrialCallback(void *);
void QtnjReadFailureCharactersCallback(void *);
void QtnjStartFailureAuthorCallback(void *);
void QtnjPlayOutcomeAuthorCallback(void *);
void QtnjFinishCallback(void *);
// @verified - p: distinct no-argument nullsub_0 reached by native QTNJ setup.
void QtnjSetupNullsub() {
}
// @verified - p: QtnjInit.
void QtnjInit() {
}
// @verified - p: QtnjMain.
void QtnjMain(std::uintptr_t selection) {
    QueueQtnjEvent(reinterpret_cast<const std::int32_t *>(selection));
}
// @verified - p: InitializeQtnjEvent.
void InitializeQtnjEvent(QtnjEventWork *work) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EVT_SetQteRetryFrames(36);
    EVT_SetQteFailureCount(0);
    GetCurrentDateTime(eventTemporaryDateTime);
    eventMovementStates.fill(0);
    Vector3 position{};
    EV_GetCharaPosition(QTNJ_RYO, 0, &position, EventTransformFlags::World);
    work->baseHeight = position.y;
    work->npcReader = nullptr;
    work->trialRecord = QTNJ_TRIALS + work->selection[0];
    work->admission = Queue_EVT_AT_ReadChr(work->task, QTNJ_ADMISSION_CHARACTERS);
    const char *directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("284E"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("QTNJ"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QTNJ"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QTNJ"), 0);
    EV_FreeAuthorPak(1, "0284");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0284A000");
    SetRenderClockAndCompassTo(1);
    QtnjSetupNullsub();
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
}
// @verified - p: QtnjStartCallback.
void QtnjStartCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    work->state = 0;
    work->trial = 0;
    work->cooldown = 0;
    SetLowQualityHands(FindCharacterByScenarioName(QTNJ_RYO));
    SetLowQualityHands(FindCharacterByScenarioName(QTNJ_REN));
    EV_mtwkSetEnabled(scenFindMotionwork(QTNJ_RYO), 1);
    mtwkSetExtraFlag(scenFindMotionwork(QTNJ_RYO), 0, 0);
    if (work->npcReader)
        for (std::int32_t &request : work->npcReader->requests)
            request = 3;
    else
        work->npcReader = QueueEventNpcRead();
    RequestEventNpcCharacters(work->npcReader, QTNJ_START_CHARACTERS);
    HLib::SetCurrentTaskTo(QtnjStartAuthorCallback);
}
// @verified - p: QtnjStartAuthorCallback.
void QtnjStartAuthorCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    if (work->npcReader->busy == 0) {
        EV_LoadAudio("0284A000", 0);
        EV_spmBeginSampleExecute();
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (work->restart != 0)
            EV_SetAuthorActiveTo(1);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QTNJ_AUTH_START.name));
        work->continuation = QtnjNextTrialCallback;
        HLib::SetCurrentTaskTo(QtnjPlayAuthorCallback);
    }
}
// @verified - p: QtnjPlayAuthorCallback.
void QtnjPlayAuthorCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0) {
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        HLib::SetCurrentTaskTo(work->continuation);
    }
}
// @verified - p: QtnjNextTrialCallback.
void QtnjNextTrialCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    const QtnjTrial &trial = *work->trialRecord;
    EventSetPosition position{
        0, QTNJ_BASE, QTNJ_INVALID, {QTNJ_WAYPOINTS[work->state].position.x, work->baseHeight, QTNJ_WAYPOINTS[work->state].position.z}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, QTNJ_BASE, QTNJ_INVALID, QTNJ_WAYPOINTS[work->state].yawDegrees, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position.character = QTNJ_OBSTACLE_BASE;
    position.position = {trial.position.x, work->baseHeight, trial.position.z};
    EVT_AT_SetPosRot(&position);
    angle.character = QTNJ_OBSTACLE_BASE;
    angle.degrees = trial.yawDegrees;
    EVT_AT_SetAngY_Callback(&angle);
    position.parent = QTNJ_BASE;
    position.position = {0, 0, 0};
    angle.parent = QTNJ_BASE;
    angle.degrees = 0;
    position.character = QTNJ_DUMMY_RIGHT;
    EVT_AT_SetPosRot(&position);
    angle.character = QTNJ_DUMMY_RIGHT;
    EVT_AT_SetAngY_Callback(&angle);
    position.character = QTNJ_REN;
    EVT_AT_SetPosRot(&position);
    angle.character = QTNJ_REN;
    EVT_AT_SetAngY_Callback(&angle);
    position.character = QTNJ_RYO;
    position.parent = QTNJ_REN;
    position.position.z = 1.5f;
    EVT_AT_SetPosRot(&position);
    angle.character = QTNJ_RYO;
    angle.parent = QTNJ_REN;
    EVT_AT_SetAngY_Callback(&angle);
    position.character = QTNJ_DUMMY_LEFT;
    position.parent = QTNJ_BASE;
    position.position = {0, 0, 0};
    EVT_AT_SetPosRot(&position);
    angle.character = QTNJ_DUMMY_LEFT;
    angle.parent = QTNJ_BASE;
    EVT_AT_SetAngY_Callback(&angle);
    EventSetMotion motion{0, QTNJ_REN, 0xf002, 1.0f, 1.0f, 0x40000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(0xf002);
    motion.character = QTNJ_RYO;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(0xf002);
    SetHighModelXYZ(FindCharacterByScenarioName(QTNJ_RYO), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(QTNJ_RYO), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QTNJ_RYO), 0, reinterpret_cast<std::byte *>(commonAuthoredHandPose), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QTNJ_RYO), 1, reinterpret_cast<std::byte *>(commonAuthoredHandPose), 0);
    SetHighModelXYZ(FindCharacterByScenarioName(QTNJ_REN), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(QTNJ_REN), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QTNJ_REN), 0, reinterpret_cast<std::byte *>(commonAuthoredHandPose), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QTNJ_REN), 1, reinterpret_cast<std::byte *>(commonAuthoredHandPose), 0);
    work->chase.source = QTNJ_REN;
    work->chase.target = QTNJ_RYO;
    work->chase.delay = 1;
    work->chase.positionGain = {0.5f, 1.0f, 0.5f};
    work->chase.rotationGain = {1.0f, 0.3f, 1.0f};
    work->chase.positionGainAmplitude = 0.05f;
    work->chase.rotationGainAmplitude = 0.03f;
    work->chase.positionGainFrequency = 1.0f;
    work->chase.rotationGainFrequency = 2.0f;
    work->chase.positionDeadzone = {0.6f, 0, 0.5f};
    work->chase.rotationDeadzone = {0, 0, 0};
    work->chase.positionDeadzoneAmplitude = 0.1f;
    work->chase.rotationDeadzoneAmplitude = 0;
    work->chase.positionDeadzoneFrequency = 1.0f;
    work->chase.rotationDeadzoneFrequency = 1.0f;
    work->chase.ticks = 0;
    work->chase.readIndex = 0;
    work->chase.writeIndex = 0;
    EV_GetCharaPosition(QTNJ_REN, 0, &work->chase.positions[0], EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(QTNJ_REN, &work->chase.rotations[0], EventTransformFlags::World);
    work->obstacle.obstacle = trial.obstacle;
    work->obstacle.trial = work->trial;
    work->obstacle.npcs = trial.characters;
    work->obstacle.waitFrames = 1;
    work->cooldown = 0;
    InitializeQtnjObstacle(trial.obstacle);
    for (std::int32_t &request : work->npcReader->requests)
        request = 3;
    RequestEventNpcCharacters(work->npcReader, trial.characters);
    ++work->state;
    HLib::SetCurrentTaskTo(QtnjStartRunCallback);
}
// @verified - p: QtnjStartRunCallback.
void QtnjStartRunCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    if (work->npcReader->busy != 0)
        return;
    const EventFilmControl film{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 10};
    EVT_AT_MenCtrl(film);
    work->movementSlot = static_cast<std::int32_t>(QueueEventMovement(nullptr, QTNJ_REN, QTNJ_WAYPOINTS, &QTNJ_MOTIONS, 0.9f, 0xc8480000U));
    if (eventMovementStates[work->movementSlot] != 0) {
        EventMovementWork *const movement = static_cast<EventMovementWork *>(HLib::GetTaskParameter(eventMovementTasks[work->movementSlot]));
        movement->index = work->state;
        StartEventMovementSegment(movement);
        ConfigureEventMovementMotions(movement);
        movement->active = 1;
    }
    EventSetMotion motion{0, QTNJ_RYO, 0x2171, 1.0f, 0.9f, 0x50000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(0x2171);
    switch (work->state) {
    case 1:
        EVT_AT_SetCam(7410, QTNJ_BASE, QTNJ_BASE, 0, 0, 10);
        EVT_AT_SetCam(7411, QTNJ_BASE, QTNJ_BASE, 0, 0, 55);
        break;
    case 3: EVT_AT_SetVoice("0284C005",1,10); EVT_AT_SetCam(7412,QTNJ_BASE,QTNJ_BASE,0,0,10); break; // @text EN: "Uh oh!" | JA: "ヤベっ！"
    case 6:
        EVT_AT_SetCam(7413, QTNJ_BASE, QTNJ_BASE, 0, 0, 10);
        EVT_AT_SetCam(7414, QTNJ_BASE, QTNJ_BASE, 0, 0, 55);
        break;
    case 9: EVT_AT_SetVoice("0284C004",1,45); EVT_AT_SetCam(7401,QTNJ_RYO,QTNJ_RYO,0,0,10); break; // @text EN: "That's it!" | JA: "それだ！"
    case 12:
        EVT_AT_SetCam(7415, QTNJ_BASE, QTNJ_BASE, 0, 0, 10);
        break;
    case 14: EVT_AT_SetVoice("0284C003",1,20); EVT_AT_SetVoice("0284C002",1,100); // @text 0284C003 EN: "Oh man!" | JA: "くっそーっ！" ; 0284C002 EN: "That's it!" | JA: "そいつだ！"
        EVT_AT_SetCam(7416, QTNJ_BASE, QTNJ_BASE, 0, 0, 10);
        EVT_AT_SetCam(7417, QTNJ_BASE, QTNJ_BASE, 0, 0, 70);
        break;
    case 17:
        EVT_AT_SetCam(7418, QTNJ_BASE, QTNJ_BASE, 0, 0, 10);
        EVT_AT_SetCam(7419, QTNJ_BASE, QTNJ_BASE, 0, 0, 60);
        break;
    case 20: EVT_AT_SetVoice("0284C008",1,40); EVT_AT_SetCam(7420,QTNJ_BASE,QTNJ_BASE,0,0,10); break; // @text EN: "There!" | JA: "そこだ！"
    default:
        EVT_AT_SetCam(7401, QTNJ_RYO, QTNJ_RYO, 0, 0, 10);
        break;
    }
    HLib::SetCurrentTaskTo(QtnjWaitRunCallback);
}
// @verified - p: QtnjWaitRunCallback.
void QtnjWaitRunCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    const QtnjTrial &trial = *work->trialRecord;
    work->state = eventMovementStates[work->movementSlot] != 0 ?
        static_cast<EventMovementWork *>(HLib::GetTaskParameter(eventMovementTasks[work->movementSlot]))->index :
        -1;
    if (work->state == 23) {
        HLib::SetCurrentTaskTo(QtnjCheckTrialCallback);
        return;
    }
    UpdateEventChase(&work->chase);
    Vector3 position{};
    EV_GetCharaPosition(QTNJ_RYO, 0, &position, EventTransformFlags::World);
    if (ComputeAngle(position, 3.0f, 0, 65353, trial.position)) {
        qteBattleUtil(BattleQteMode::SetFirstCommands, trial.buttons[0], trial.buttons[1], trial.buttons[2], trial.buttons[3]);
        qteBattleUtil(BattleQteMode::SetLastCommands);
        EVT_AT_QTE(0, trial.maximumFrames, 4, 0);
        HLib::SetCurrentTaskTo(QtnjWaitQteCallback);
        return;
    }
    if (work->cooldown == 0) {
        if (work->state != trial.approachState)
            return;
        CancelEventMovement(work->movementSlot);
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(trial.author.name));
        work->state = trial.escapeState;
        work->continuation = QtnjStartRunCallback;
        HLib::SetCurrentTaskTo(QtnjCheckAuthorCallback);
    } else if (work->cooldown != 1)
        --work->cooldown;
}
// @verified - p: QtnjWaitQteCallback.
void QtnjWaitQteCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    if (qteBattleUtil(BattleQteMode::GetResult) != 0) {
        if (qteBattleUtil(BattleQteMode::GetCompletedCount) == work->trialRecord->requiredCommands) {
            work->success = 1;
            work->state = work->trialRecord->successState;
            CancelEventMovement(work->movementSlot);
            PlayQtnjObstacle(&work->obstacle);
            const EventFilmControl film{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, static_cast<float>(work->obstacle.waitFrames - 30)};
            EVT_AT_MenCtrl(film);
            HLib::SetCurrentTaskTo(QtnjCheckTrialCallback);
            HLib::SetupTaskDelay(work->obstacle.waitFrames);
        } else {
            work->success = 0;
            UpdateEventChase(&work->chase);
            for (std::int32_t &request : work->npcReader->requests)
                request = 3;
            HLib::SetCurrentTaskTo(QtnjReadFailureCharactersCallback);
        }
    } else
        UpdateEventChase(&work->chase);
}
// @verified - p: QtnjCheckAuthorCallback.
void QtnjCheckAuthorCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EventSetPosition position{
        0, QTNJ_BASE, QTNJ_INVALID, {QTNJ_WAYPOINTS[work->state].position.x, work->baseHeight, QTNJ_WAYPOINTS[work->state].position.z}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, QTNJ_BASE, QTNJ_INVALID, QTNJ_WAYPOINTS[work->state].yawDegrees, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position.character = QTNJ_REN;
    position.parent = QTNJ_BASE;
    position.position = {0, 0, 0};
    EVT_AT_SetPosRot(&position);
    angle.character = QTNJ_REN;
    angle.parent = QTNJ_BASE;
    angle.degrees = 0;
    EVT_AT_SetAngY_Callback(&angle);
    position.character = QTNJ_RYO;
    position.parent = QTNJ_REN;
    position.position.z = 1.5f;
    EVT_AT_SetPosRot(&position);
    angle.character = QTNJ_RYO;
    angle.parent = QTNJ_REN;
    EVT_AT_SetAngY_Callback(&angle);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, 0x10, QTNJ_TASK);
    RegisterEventTask(task);
    StepObjectDisplayWork *display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
    display->cleanup = 1;
    display->character = QTNJ_RYO;
    display->mode = 0;
    display->delay = 10;
    task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, 0x10, QTNJ_TASK);
    RegisterEventTask(task);
    display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
    display->cleanup = 1;
    display->character = QTNJ_REN;
    display->mode = 0;
    display->delay = 10;
    HLib::SetCurrentTaskTo(work->continuation);
}
// @verified - p: QtnjCheckTrialCallback.
void QtnjCheckTrialCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    if (++work->trial < 4) {
        ++work->trialRecord;
        HLib::SetCurrentTaskTo(QtnjNextTrialCallback);
    } else if (work->success != 0) {
        EV_mtwkSetEnabled(scenFindMotionwork(QTNJ_RYO), 1);
        mtwkSetExtraFlag(scenFindMotionwork(QTNJ_RYO), 0, 0);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QTNJ_AUTH_SUCCESS.name));
        work->continuation = QtnjFinishCallback;
        HLib::SetCurrentTaskTo(QtnjPlayOutcomeAuthorCallback);
    } else {
        work->trialRecord = QTNJ_TRIALS + work->selection[0];
        const std::uint32_t failures = EVT_GetQteFailureCount();
        if (failures < 2)
            EVT_SetQteFailureCount(failures + 1);
        SetCurrentDateTimeTo(eventTemporaryDateTime);
        shenSetTimeSimulationTo(1);
        work->restart = 1;
        HLib::SetCurrentTaskTo(QtnjStartCallback);
    }
}
// @verified - p: QtnjReadFailureCharactersCallback.
void QtnjReadFailureCharactersCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    for (std::int32_t state : work->npcReader->states)
        if (state != 0) {
            UpdateEventChase(&work->chase);
            return;
        }
    RequestEventNpcCharacters(work->npcReader, work->trial == 3 ? QTNJ_LAST_FAILURE_CHARACTERS : QTNJ_FAILURE_CHARACTERS);
    EventFilmControl film{0, 60, 0, 0, 0, 0, 255, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&film);
    HLib::SetCurrentTaskTo(QtnjStartFailureAuthorCallback);
}
// @verified - p: QtnjStartFailureAuthorCallback.
void QtnjStartFailureAuthorCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    if (work->npcReader->busy != 0 || filmCheckFadeTrackColour()) {
        UpdateEventChase(&work->chase);
        return;
    }
    CancelEventMovement(work->movementSlot);
    EV_mtwkSetEnabled(scenFindMotionwork(QTNJ_RYO), 1);
    mtwkSetExtraFlag(scenFindMotionwork(QTNJ_RYO), 0, 0);
    shen_name author = QTNJ_AUTH_LAST_FAILURE;
    if (work->trial != 3) {
        Matrix4x4_Push();
        workTransformMatrix = transformMatrix_prePass;
        qtnjAuthorPlacement = transformMatrix_prePass;
        workTransformMatrix.rows[3].x = QTNJ_FAILURE_OFFSETS[work->trial].x;
        workTransformMatrix.rows[3].y = QTNJ_FAILURE_OFFSETS[work->trial].y;
        workTransformMatrix.rows[3].z = QTNJ_FAILURE_OFFSETS[work->trial].z;
        qtnjAuthorPlacement.rows[3] = workTransformMatrix.rows[3];
        Matrix4x4::Pop(0);
        Author_SetPlacementMatrix(&qtnjAuthorPlacement);
        author = QTNJ_AUTH_FAILURE;
    }
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(author.name));
    work->trial = 4;
    work->continuation = QtnjCheckTrialCallback;
    if (work->restart != 0)
        EV_SetAuthorActiveTo(1);
    HLib::SetCurrentTaskTo(QtnjPlayOutcomeAuthorCallback);
}
// @verified - p: QtnjPlayOutcomeAuthorCallback.
void QtnjPlayOutcomeAuthorCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0) {
        for (std::int32_t &request : work->npcReader->requests)
            request = 3;
        spmFadeOut();
        HLib::SetCurrentTaskTo(work->continuation);
        HLib::SetupTaskDelay(60);
    }
    if (GetCurrentAuthorId().name == QTNJ_AUTH_FAILURE.name) {
        const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
        if (frame == 1)
            snapSetDesc(1);
        else if (frame == 101)
            snapSetDesc(0);
    }
}
// @verified - p: QtnjFinishCallback.
void QtnjFinishCallback(void *parameter) {
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(parameter);
    CleanupQtnjEvent(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    if (g_currentMap.name == QTNJ_AREA.name) {
        scenSetTaskFlags(6);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}
} // namespace
// @verified - p: QueueQtnjEvent.
void QueueQtnjEvent(const std::int32_t *selection) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QtnjStartCallback, 4, 0x0b, 0x3c8, QTNJ_TASK);
    QtnjEventWork *const work = static_cast<QtnjEventWork *>(HLib::GetTaskParameter(task));
    work->selection = selection ? selection : QTNJ_DEFAULT_SELECTION;
    work->task = task;
    work->restart = 0;
    InitializeQtnjEvent(work);
}
// @verified - p: CleanupQtnjEvent.
void CleanupQtnjEvent(QtnjEventWork *work) {
    EventSetPositionAngle position{0, QTNJ_RYO, QTNJ_INVALID, {251.5f, 0, 140.4f}, -90, 0};
    EVT_AT_SetPosAng_Callback(&position);
    hndlSetFlags(FindCharacterByScenarioName(QTNJ_REN));
    hndlSetFlags(FindCharacterByScenarioName(QTNJ_RYO));
    qteBattleUtil(BattleQteMode::RetireAndFree);
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    FinishLoading();
    SetRenderClockAndCompassTo(0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0284");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QTNJ"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("284E"), 0);
    ET_AT_KillWork(work->admission);
    for (std::int32_t slot = 0; slot < 20; ++slot)
        CancelEventMovement(slot);
    EventFilmControl film{0, 30, 255, 0, 0, 0, 0, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&film);
    EV_NPC_NewLoadSw(1);
}
const ScriptAreaDefinition qtnjScriptDefinition{QTNJ_AREA, 0, QtnjInit, QtnjMain};
} // namespace shenmue::s2
