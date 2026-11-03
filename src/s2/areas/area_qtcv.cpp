#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_qtcv.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void QtcvMain(void *argument)
{
    QueueQtcvEvent((const QtcvConfiguration *)argument);
}
#elif defined(SHENMUE_EVENT_QTCV_CARRY)
static const short qtcvCarryCameraDelays[3][5] = {{0, 170, 417, 417, 417}, {0, 60, 180, 180, 180}, {0, 200, 340, 340, 340}};
static const unsigned short qtcvCarryCameras[3][5] = {{7010, 7012, 7015, 7015, 7015}, {7000, 7001, 7002, 7002, 7002}, {7005, 7006, 7007, 7007, 7007}};
static const Vector3 qtcvCarryRenOffsets[3] = {{-0.67113f, 0.0f, 1.84862f}, {0.54767f, 0.0f, 1.26767f}, {-0.31364f, 0.0f, 0.7248f}};
static const unsigned short qtcvCarryMotions[3][2] = {{0x21e2, 0x21df}, {0x21e3, 0x21e0}, {0x21e1, 0x21de}};
static const short qtcvCarryVoiceDelays[3][3][5] = {{{27, 55, 189, 223, 342}, {27, 55, 132, 173, 314}, {87, 104, 298, 350, 498}},
    {{27, 55, 164, 220, 302}, {27, 56, 129, 178, 319}, {87, 104, 282, 350, 450}},
    {{63, 98, 164, 220, 310}, {49, 80, 129, 190, 299}, {95, 146, 294, 350, 485}}};
static const char *qtcvCarryVoices[3][5] = {{"0301A005", "0301B004", "0301B005", "0301A004", "0301B006"},
    {"0301A005", "0301B007", "0301B008", "0301A006", "0301B009"}, {"0301A007", "0301B010", "0301B011", "0301A008", "0301B012"}};
static const float qtcvCarryTailFrames[3] = {120.0f, 100.0f, 200.0f};
static const short qtcvCarryTailCameraDelays[3][3] = {{0, 85, 297}, {0, 150, 150}, {0, 200, 200}};
static const unsigned short qtcvCarryTailCameras[3][3] = {{7011, 7012, 7015}, {7003, 7002, 7002}, {7008, 7009, 7009}};
static const Vector3 qtcvCarryTailRenOffsets[3] = {{-0.77113f, 0.0f, 1.84862f}, {0.54767f, 0.0f, 1.26767f}, {-0.31364f, 0.0f, 0.7248f}};
static const unsigned short qtcvCarryTailMotions[3][2] = {{0x21e2, 0x21df}, {0x21e3, 0x21e0}, {0x21e1, 0x21de}};
static const int qtcvCarryTailVoiceDelays[3][3] = {{43, 103, 194}, {22, 86, 194}, {67, 150, 263}};
static const char *qtcvCarryTailVoices[3][3] = {{"0301B014", "0301A009", "0301B015"}, {"0301B014", "0301A009", "0301B015"},
    {"0301B014", "0301A009", "0301B015"}};

int PlayPlankCarryAuthoredTransition(int kind, int dialogue)
{
    int frames;
    int index;
    qtcvCarryState = 0;
    EVT_AT_SetPosAng(QTCV_PLAYER, QTCV_STAGE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetPosAng(QTCV_REN, QTCV_PLAYER, qtcvCarryRenOffsets[kind].x, qtcvCarryRenOffsets[kind].z, 0.0f, 0);
    frames = EVT_AT_SetMot(QTCV_PLAYER, qtcvCarryMotions[kind][0], 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(QTCV_REN, qtcvCarryMotions[kind][1], 0.0f, 1.0f, 0x40000000U, 0);
    for (index = 0; index < 5; index++)
        EVT_AT_SetCam(qtcvCarryCameras[kind][index], QTCV_STAGE, QTCV_STAGE, 0, 0, qtcvCarryCameraDelays[kind][index]);
    for (index = 0; index < 5; index++)
        EVT_AT_SetVoice(qtcvCarryVoices[dialogue][index], qtcvCarryVoiceDelays[dialogue][kind][index]);
    return frames;
}

int PlayPlankCarryAuthoredTransitionTail(int kind, int dialogue)
{
    int frames;
    int index;
    qtcvCarryState = 0;
    EVT_AT_SetPosAng(QTCV_PLAYER, QTCV_STAGE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetPosAng(QTCV_REN, QTCV_PLAYER, qtcvCarryTailRenOffsets[kind].x, qtcvCarryTailRenOffsets[kind].z, 0.0f, 0);
    frames = EVT_AT_SetMot(QTCV_PLAYER, qtcvCarryTailMotions[kind][0], qtcvCarryTailFrames[kind], 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(QTCV_REN, qtcvCarryTailMotions[kind][1], qtcvCarryTailFrames[kind], 1.0f, 0x44000000U, 0);
    for (index = 0; index < 3; index++)
        EVT_AT_SetCam(qtcvCarryTailCameras[kind][index], QTCV_STAGE, QTCV_STAGE, 0, 0, qtcvCarryTailCameraDelays[kind][index]);
    for (index = 0; index < 3; index++)
        EVT_AT_SetVoice(qtcvCarryTailVoices[dialogue][index], qtcvCarryTailVoiceDelays[kind][index]);
    return frames;
}

int StartQtcvAlternateCamera(void)
{
    EVT_AT_SetCam(7016, QTCV_SECOND_STAGE, QTCV_SECOND_STAGE, 0, 0, 0);
    EVT_AT_SetSound(0x0da8, 0);
    return 200;
}
#else
static const QtcvPlacement qtcvPlacements[] = {{{221.8f, 110.8f, 162.4f}, 180.0f}, {{202.0f, 110.8f, 148.4f}, 90.0f}, {{245.3f, 110.8f, 148.7f}, 270.0f}};
static const QtcvConfiguration qtcvConfigurations[] = {{0, 0}, {1, 0}, {2, 0}};
static shen_name qtcvLoadCharacters[] = {QTCV_PLAYER, QTCV_REN, EVENT_CHARACTER_INVALID};
int qtcvCarryState = 0;
static int qtcvAuthorFlags[] = {2062, 2063, 2064};
static shen_name qtcvAuthors[] = {QTCV_AUTHOR_1, QTCV_AUTHOR_2, QTCV_AUTHOR_0};

void QueueQtcvEvent(const QtcvConfiguration *configuration)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(QtcvStartCallback, 4, 11, sizeof(QtcvEventWork), TASK_EVNT);
    QtcvEventWork *work = (QtcvEventWork *)FTBL.HLib_GetTaskParameter(task);
    HLibTask *debugTask = FTBL.HLib_EnqueueTaskWithParameter(QtcvDebugDisplayCallback, 3, 11, sizeof(QtcvEventWork *), TASK_EVNT);
    *(QtcvEventWork **)FTBL.HLib_GetTaskParameter(debugTask) = work;
    work->debugTask = debugTask;
    work->task = task;
    if (configuration)
        work->configuration = configuration;
    else
        work->configuration = &qtcvConfigurations[g_entryPoint];
    InitializeQtcvResources(work);
    EventDebugRegister(QTCV_DEBUG_ID, QtcvDebugExitCallback, work, 2000000000);
}

void QtcvDebugExitCallback(void *data)
{
    QtcvEndCallback(data);
}

void QtcvEndCallback(void *data)
{
    QtcvEventWork *work = (QtcvEventWork *)data;
    int changedFlag330 = work->changedFlag330;
    CleanupQtcvResources(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->debugTask);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    EventDebugUnregister(QTCV_DEBUG_ID);
    FTBL.EV_SetPlayerFlag(12, 1320, 0);
    if (changedFlag330)
        FTBL.EV_stepSetFlags(330, 0);
    if (g_currentMap == QTCV_AREA) {
        FTBL.scenSetTaskFlags(6);
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    } else
        FTBL.qronRepositionPlayerForFloor(7);
}

void QtcvDebugDisplayCallback(void *data)
{
    FTBL.SetDebugScreenTextPosition(2, 23);
}

void SetQtcvEndContinuation(HLibTaskCallback *continuation)
{
    *continuation = QtcvEndThunkCallback;
}

void QtcvStartCallback(void *data)
{
    QtcvEventWork *work = (QtcvEventWork *)data;
    HLibTaskCallback *continuation = &work->authorContinuation;
    const QtcvPlacement *placement = work->placement;
    int frames;
    EventDebugCheck(QTCV_DEBUG_ID, 4);
    SetQtcvEndContinuation(continuation);
    EVT_AT_SetPos(QTCV_STAGE, EVENT_CHARACTER_INVALID, placement->position.x, work->playerHeight, placement->position.z, 0);
    EVT_AT_SetAngY(QTCV_STAGE, EVENT_CHARACTER_INVALID, placement->yaw, 0);
    EVT_AT_SetPos(QTCV_SECOND_STAGE, EVENT_CHARACTER_INVALID, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QTCV_SECOND_STAGE, EVENT_CHARACTER_INVALID, 0.0f, 0);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(QTCV_PLAYER));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(QTCV_REN));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QTCV_PLAYER), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QTCV_PLAYER), 1, 1, 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QTCV_PLAYER), 0, (void *)areaLoopHandPoses[QTCV_GRIP_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QTCV_PLAYER), 1, (void *)areaLoopHandPoses[QTCV_GRIP_HAND_POSE], 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QTCV_REN), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QTCV_REN), 1, 1, 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QTCV_REN), 0, (void *)areaLoopHandPoses[QTCV_GRIP_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(QTCV_REN), 1, (void *)areaLoopHandPoses[QTCV_GRIP_HAND_POSE], 0);
    FTBL.AT_SetLocalBaseChara(QTCV_DUMMY_PLAYER, QTCV_PLAYER);
    FTBL.AT_SetLocalBaseChara(QTCV_DUMMY_REN, QTCV_REN);
    EVT_AT_SetPos(QTCV_DUMMY_PLAYER, QTCV_PLAYER, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QTCV_DUMMY_PLAYER, QTCV_PLAYER, 0.0f, 0);
    EVT_AT_SetPos(QTCV_DUMMY_REN, QTCV_REN, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(QTCV_DUMMY_REN, QTCV_REN, 0.0f, 0);
    EVT_AT_SetPos(QTCV_PLAYER, QTCV_STAGE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetPos(QTCV_REN, QTCV_STAGE, 0.0f, 0.0f, 0.0f, 0);
    FTBL.EV_NPC_NewLoadSw_0(1);
    if (work->configuration->tail)
        frames = PlayPlankCarryAuthoredTransitionTail(work->configuration->kind, work->dialogue);
    else
        frames = PlayPlankCarryAuthoredTransition(work->configuration->kind, work->dialogue);
    if (FTBL.EV_GetShenmueFlag(330)) {
        work->changedFlag330 = 1;
        FTBL.EV_stepSetFlags(330, 1);
        FTBL.HLib_SetCurrentTaskTo(QtcvAlternateAuthorCallback);
    } else
        FTBL.HLib_SetCurrentTaskTo(QtcvEndThunkCallback);
    FTBL.EV_LoadAudio("0301E000", 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, frames - 30);
    FTBL.HLib_SetupTaskDelay(frames);
}

void QtcvAlternateAuthorCallback(void *data)
{
    QtcvEventWork *work = (QtcvEventWork *)data;
    FTBL.EV_SetPlayerFlag(12, qtcvAuthorFlags[work->configuration->kind], 1);
    FTBL.EV_authSceneCtrl(qtcvAuthors[work->configuration->kind]);
    work->authorContinuation = QtcvAlternateFinishCallback;
    FTBL.HLib_SetCurrentTaskTo(AuthoredEventContinuationCallback);
}

void AuthoredEventContinuationCallback(void *data)
{
    if (!FTBL.EV_authSceneCtrl(-1))
        FTBL.HLib_SetCurrentTaskTo(((QtcvEventWork *)data)->authorContinuation);
}

void QtcvAlternateFinishCallback(void *data)
{
    int frames = StartQtcvAlternateCamera();
    FTBL.EV_SetPlayerFlag(12, 1320, 1);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_MenCtrl(60, 0, 0, 0, 0, 255, 0, 0, 0, frames - 60);
    FTBL.HLib_SetCurrentTaskTo(QtcvEndThunkCallback);
    FTBL.HLib_SetupTaskDelay(frames);
}

void QtcvEndThunkCallback(void *data)
{
    QtcvEndCallback(data);
}

void InitializeQtcvResources(QtcvEventWork *work)
{
    int flags[3] = {331, 332, 333};
    Vector3 position;
    int index;
    for (index = 0, work->dialogue = -1; index <= 2; index++)
        if (!FTBL.EV_GetShenmueFlag(flags[index]))
            work->dialogue++;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    FTBL.GetCharaPosition(QTCV_PLAYER, 0xffffffffU, &position, 0x40000000U);
    work->playerHeight = position.y;
    work->changedFlag330 = 0;
    work->placement = &qtcvPlacements[work->configuration->kind];
    work->characters = Queue_EVT_AT_ReadChr(work->task, qtcvLoadCharacters);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"301E", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"QTCV", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "QTCV", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "QTCV", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0301");
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0301E000");
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QTCV_PLAYER), 1);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QTCV_REN), 1);
}

void CleanupQtcvResources(QtcvEventWork *work)
{
    EVT_AT_SetPosAng(QTCV_PLAYER, QTCV_STAGE, 0.0f, 2.0f, 180.0f, 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QTCV_PLAYER), 0);
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QTCV_REN), 0);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(QTCV_REN));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(QTCV_PLAYER));
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0301");
    FTBL.EV_ControlPackageResources(0, 6, "QTCV", 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "301E", 0, 0);
    ET_AT_KillWork(work->characters);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_NPC_NewLoadSw_0(1);
}
#endif
#else
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
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("301E"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QTCV"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QTCV"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QTCV"), 0);
    EV_FreeAuthorPak(1, "0301");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0301E000");
    shenSetClockAndCompassRenderTo(1);
    QtcvDormantBoundary();
    SetPluralControlFor(ctrlGetScenarioChara(QTCV_PLAYER), 1);
    SetPluralControlFor(ctrlGetScenarioChara(QTCV_REN), 1);
}
// @verified - p
void QtcvStartCallback(void *data) {
    QtcvEventWork &work = *static_cast<QtcvEventWork *>(data);
    work.authorContinuation = QtcvEndThunkCallback;
    EVT_AT_SetPos(QTCV_STAGE, QTCV_INVALID, {work.placement->position.x, work.playerHeight, work.placement->position.z}, 0);
    EVT_AT_SetAngY(QTCV_STAGE, QTCV_INVALID, work.placement->yaw, 0);
    EVT_AT_SetPos(QTCV_SECOND_STAGE, QTCV_INVALID, {}, 0);
    EVT_AT_SetAngY(QTCV_SECOND_STAGE, QTCV_INVALID, 0, 0);
    SetLowQualityHands(ctrlGetScenarioChara(QTCV_PLAYER));
    SetLowQualityHands(ctrlGetScenarioChara(QTCV_REN));
    SetHighModelXYZ(ctrlGetScenarioChara(QTCV_PLAYER), 1, 0, 0);
    SetHighModelXYZ(ctrlGetScenarioChara(QTCV_PLAYER), 1, 1, 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(QTCV_PLAYER), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(QTCV_PLAYER), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    SetHighModelXYZ(ctrlGetScenarioChara(QTCV_REN), 1, 0, 0);
    SetHighModelXYZ(ctrlGetScenarioChara(QTCV_REN), 1, 1, 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(QTCV_REN), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(QTCV_REN), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
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
    SetPluralControlFor(ctrlGetScenarioChara(QTCV_PLAYER), 0);
    SetPluralControlFor(ctrlGetScenarioChara(QTCV_REN), 0);
    hndlSetFlags(ctrlGetScenarioChara(QTCV_REN));
    hndlSetFlags(ctrlGetScenarioChara(QTCV_PLAYER));
    FinishLoading();
    shenSetClockAndCompassRenderTo(0);
    sbtLoadProgram();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0301");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QTCV"), 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("301E"), 0);
    ET_AT_KillWork(work->characters);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_NPC_NewLoadSw(1);
}
const ScriptAreaDefinition qtcvScriptDefinition{QTCV_AREA, 0, QtcvInit, QtcvMain};
} // namespace shenmue::s2
#endif
