#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_qfeb.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
const char qfebAuthorScene[] = "0302";
const shen_name qfebCharacters[] = {QFEB_RYO, QFEB_REN, QFEB_RKN, QFEB_KHO, QFEB_SOY, QFEB_KIC, QFEB_ANC, QFEB_MRC, EVENT_CHARACTER_INVALID};
const char qfebPackage[] = "QFEB";

void QfebMain(unsigned char entry)
{
    HLibTask *task;
    QfebEventWork *work;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    task = FTBL.HLib_EnqueueTaskWithParameter(QfebMainCallback, 5, 11, sizeof(QfebEventWork), TASK_EVNT);
    work = (QfebEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->phase = 0;
    work->reserved34 = 0;
    work->npcBatchOverride = FTBL.GetNpcBatchCountOverride(46);
    work->reserved34 = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, qfebCharacters);
    work->entry = entry;
    work->delay = 0;
    work->battleUpdates = 0;
    work->battleFrames = 0;
    work->battleFade = 0;
    work->exitFade = 0;
    work->idleFrames = 0;
    work->phaseFrames = 0;
    work->voiceFrames = 0;
    work->previousVoice = 0;
    switch (work->entry) {
    case 1:
        work->phase = 0;
        break;
    case 2:
    case 3:
    default:
        work->phase = 0;
        break;
    }
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)qfebPackage, 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, qfebPackage, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, qfebPackage, 0, 0);
    FTBL.EV_SetNewSceneIndex();
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(1);
    {
        float green = 40.0f, blue = 1.0f;
        do {
            FTBL.SetFogRed(9.1f);
            FTBL.SetFogGreen(green);
            FTBL.SetFogBlue(blue);
            FTBL.SetFogDistance(0x70000);
            FTBL.ReinitFogTasks(1);
        } while (0);
    }
    FTBL.camCtrlSetNpcRadius(40.0f);
    FTBL.CreateEventCharacter(3, 0, QFEB_BASE);
    EVT_AT_SetPos(QFEB_BASE, -1, qfebBasePositions[work->entry - 1].x, 128.8f, qfebBasePositions[work->entry - 1].z, 0);
    EVT_AT_SetAngY(QFEB_BASE, -1, qfebBasePositions[work->entry - 1].yaw, 0);
    FTBL.EV_SetCanDoAreaJumpTo(0);
    EventDebugRegister(QFEB_DEBUG_ID, QfebDebugExitCallback, work, 36000);
}
#else
Vector3i qfebAuthorReturnRotation = {0, 0, 0};
Vector3i qfebAuthorDefaultRotation = {0, 32768, 0};
int qfebReturnArguments[3][3] = {{2, 1, 1}, {0, 1, 1}, {1, 1, 1}};
QfebPlace qfebBasePositions[3] = {{241.68f, 147.0f, 70.28f}, {222.74f, 175.82f, 70.28f}, {188.3f, 150.53f, 70.28f}};
QfebPlace qfebChoreographyPositions[3][2] = {{{245.19f, 148.36f, 64.79f}, {245.97f, 147.44f, 77.6f}},
    {{224.54f, 176.62f, 55.21f}, {224.84f, 175.86f, 55.2f}}, {{187.83f, 150.43f, 245.54f}, {187.36f, 149.34f, 235.17f}}};
QfebNpcPlace qfebChoreographyNpcs[3][2] = {{{239.8f, 145.1f, 79.45f, QFEB_KHO}, {239.69f, 146.05f, 29.45f, QFEB_RKN}},
    {{222.0f, 174.71f, 70.1f, QFEB_ANC}, {221.52f, 175.57f, 45.0f, QFEB_KIC}},
    {{190.18f, 150.67f, 275.67f, QFEB_ANC}, {189.3f, 151.47f, 170.46f, QFEB_MRC}}};
QfebPlace qfebExitPositions[4] = {{257.73f, 168.0f, 90.0f}, {223.14f, 179.4f, 0.0f}, {188.85f, 146.85f, 180.0f}, {257.17f, 166.63f, 90.0f}};
QfebRenExit qfebRenExitPositions[4] = {{{257.04f, 125.2f, 166.31f}, 16384}, {{225.22f, 125.2f, 178.83f}, 0}, {{189.69f, 125.2f, 147.3f}, 32768},
    {{257.89f, 132.4f, 168.54f}, 12178}};
QfebAuthorReturnPose qfebAuthorReturnPoses[3] = {{{248.04f, 128.8f, 154.24f}, {0, 0, 0}}, {{224.81f, 127.0f, 185.59f}, {0, 0, 0}},
    {{187.28f, 127.0f, 140.07f}, {0, 32768, 0}}};
Vector3 qfebProximityEntry1 = {245.65f, 128.8f, 147.48f};
Vector3 qfebProximityEntry2 = {224.13f, 128.8f, 175.0f};
Vector3 qfebProximityEntry3 = {187.32f, 128.8f, 151.08f};

void QueueQfebAuthor(shen_name author)
{
    HLibTask *task;
    QfebAuthorWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(QfebAuthorCallback, 4, 11, sizeof(QfebAuthorWork), TASK_EVNT);
    work = (QfebAuthorWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->state = 1;
    work->author = author;
    PrepareQfebAuthor();
}

void QfebAuthorCallback(void *data)
{
    QfebAuthorWork *work = (QfebAuthorWork *)data;
    switch (work->state) {
    case 0:
        break;
    case 1:
        StartQfebAuthorLoop(work);
        break;
    case 3:
        FinishQfebAuthor(work);
        break;
    }
}

void StartQfebAuthorLoop(QfebAuthorWork *work)
{
    HLibTask *task;
    QfebAuthorLoopWork *loop;
    task = FTBL.HLib_EnqueueTaskWithParameter(QfebAuthorLoopCallback, 4, 11, sizeof(QfebAuthorLoopWork), TASK_EVNT);
    loop = (QfebAuthorLoopWork *)FTBL.HLib_GetTaskParameter(task);
    loop->parent = work;
    loop->frame = 0;
    HideQfebAuthorCharacters();
    FTBL.EV_authSceneCtrl(work->author);
    work->state = 2;
}

void QfebAuthorLoopCallback(void *data)
{
    QfebAuthorLoopWork *loop = (QfebAuthorLoopWork *)data;
    loop->frame = FTBL.EV_authSceneCtrl(-6);
    if (loop->frame == 0)
        FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_authSceneCtrl(-5);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.LoadCutsceneAndSoundProgramForArea();
        ShowQfebAuthorCharacters();
        FTBL.SetPlayerGameFlags(22, 0);
        loop->parent->state = 3;
        FTBL.HLib_DestroyCurrentEventTask();
    }
}

void PrepareQfebAuthor(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, qfebAuthorScene);
}

void FinishQfebAuthor(QfebAuthorWork *work)
{
    FTBL.HLib_DestroyTask(work->task);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.camCtrlUtil(15, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    if (work->author == QFEB_AUTHOR_1)
        FTBL.EV_AT_SetAngY_0(QFEB_RYO, 0x78000000, &qfebAuthorReturnRotation, 0);
    else
        FTBL.EV_AT_SetAngY_0(QFEB_RYO, 0x78000000, &qfebAuthorDefaultRotation, 0);
}

void HideQfebAuthorCharacters(void)
{
    HideAuthorCharacter(QFEB_RYO);
    HideAuthorCharacter(QFEB_REN);
}

void ShowQfebAuthorCharacters(void)
{
    ShowAuthorCharacter(QFEB_RYO);
    ShowAuthorCharacter(QFEB_REN);
}

#include "s2/events/event_author_dc.h"

void QfebMainCallback(void *data)
{
    QfebEventWork *work = (QfebEventWork *)data;
    Vector3 ryo;
    signed char entry = work->entry - 1;
    FTBL.GetCharaPosition(QFEB_RYO, -1, &ryo, 0);
    switch (work->phase) {
    case 0:
        FTBL.EV_ResetFreeTalkFlags();
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.EV_LoadCutsceneFromCurrentScene("0302E000");
        FTBL.EV_LoadAudio("0302E000", 0);
        FTBL.SetMapControlStatusFlag(0, 0);
        FTBL.EV_DisableCharacterDisplay(QFEB_RKN);
        FTBL.EV_ToggleCharacterFlag(QFEB_RKN);
        FTBL.EV_DisableCharacterDisplay(QFEB_KHO);
        FTBL.EV_ToggleCharacterFlag(QFEB_KHO);
        FTBL.EV_DisableCharacterDisplay(QFEB_SOY);
        FTBL.EV_ToggleCharacterFlag(QFEB_SOY);
        FTBL.EV_DisableCharacterDisplay(QFEB_KIC);
        FTBL.EV_ToggleCharacterFlag(QFEB_KIC);
        FTBL.EV_DisableCharacterDisplay(QFEB_ANC);
        FTBL.EV_ToggleCharacterFlag(QFEB_ANC);
        FTBL.EV_DisableCharacterDisplay(QFEB_MRC);
        FTBL.EV_ToggleCharacterFlag(QFEB_MRC);
        FTBL.SetPlayerGameFlags(22, 1);
        switch (work->entry) {
        case 1:
            if (!FTBL.EV_GetPlayerFlag(11, 1990))
                FTBL.EV_SetPlayerFlag(12, 1990, 1);
            QueueQfebAuthor(QFEB_AUTHOR_0);
            break;
        case 2:
            if (!FTBL.EV_GetPlayerFlag(11, 1991))
                FTBL.EV_SetPlayerFlag(12, 1991, 1);
            QueueQfebAuthor(QFEB_AUTHOR_2);
            break;
        case 3:
            if (!FTBL.EV_GetPlayerFlag(11, 1990))
                FTBL.EV_SetPlayerFlag(12, 1992, 1);
            QueueQfebAuthor(QFEB_AUTHOR_1);
            break;
        }
        work->phase = 1;
        break;
    case 1:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            FTBL.EV_AT_SetPos(QFEB_REN, 0x78000000, &qfebAuthorReturnPoses[work->entry - 1].position);
            FTBL.EV_AT_SetAngY_0(QFEB_REN, 0x78000000, &qfebAuthorReturnPoses[work->entry - 1].rotation, 0);
            FTBL.SetRenderClockAndCompassTo(0);
            FTBL.ExecuteCleanupFuncForAreaWithCachedScene();
            work->phaseFrames = 0;
            FTBL.EV_SetCharaControllerFlags(QFEB_RYO, 2, 1);
            FTBL.EV_SetCharaControllerFlags(QFEB_RYO, 3, 1);
            SHENMUE_EVENT_BOUNDARY();
            FTBL.EV_SetCharaControllerFlags(QFEB_RYO, 1, 1);
            FTBL.EV_EnableCharacterDisplay(QFEB_RKN);
            FTBL.EV_EnsureCharacterDraw(QFEB_RKN);
            FTBL.EV_EnableCharacterDisplay(QFEB_KHO);
            FTBL.EV_EnsureCharacterDraw(QFEB_KHO);
            FTBL.EV_EnableCharacterDisplay(QFEB_SOY);
            FTBL.EV_EnsureCharacterDraw(QFEB_SOY);
            FTBL.EV_EnableCharacterDisplay(QFEB_KIC);
            FTBL.EV_EnsureCharacterDraw(QFEB_KIC);
            FTBL.EV_EnableCharacterDisplay(QFEB_ANC);
            FTBL.EV_EnsureCharacterDraw(QFEB_ANC);
            FTBL.EV_EnableCharacterDisplay(QFEB_MRC);
            FTBL.EV_EnsureCharacterDraw(QFEB_MRC);
            work->phase = 2;
        }
        break;
    case 2:
        if (FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(QFEB_RYO), 0) == 0xf009 ||
            FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(QFEB_RYO), 0) == 0xf008) {
            FTBL.SetPlayerControlEnabled(0);
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.shenToggleAreaFlags(2);
            work->phase = 11;
        } else if (FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(QFEB_RYO), 0) == 0xf02b ||
            FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(QFEB_RYO), 0) == 0xf02a) {
            FTBL.SetPlayerControlEnabled(0);
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.shenToggleAreaFlags(2);
            work->phase = 12;
        } else if (FTBL.ryoNotBusy() && ++work->idleFrames > 300 && FTBL.GetQronNextStream() == 8 && (unsigned char)(work->entry - 2) <= 1) {
            FTBL.SetPlayerControlEnabled(0);
            EVT_AT_SetMot(QFEB_RYO, 0xf003, 0.0f, 1.0f, 0x40000000, 0);
            FTBL.EV_SetNewSceneIndex();
            FTBL.spmFadeOut(0);
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.SetRenderClockAndCompassTo(1);
            FTBL.HLib_SetupTaskDelay(30);
            work->phase = 3;
        } else if (FTBL.GetSquaredDistanceXZ(&qfebProximityEntry3, &ryo) < 4.0f || FTBL.GetSquaredDistanceXZ(&qfebProximityEntry2, &ryo) < 4.0f ||
            FTBL.GetSquaredDistanceXZ(&qfebProximityEntry1, &ryo) < 4.0f) {
            FTBL.SetPlayerControlEnabled(0);
            EVT_AT_SetMot(QFEB_RYO, 0xf003, 0.0f, 1.0f, 0x40000000, 0);
            FTBL.EV_SetNewSceneIndex();
            FTBL.spmFadeOut(0);
            FTBL.SetRenderClockAndCompassTo(1);
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            work->phase = 3;
        }
        break;
    case 3:
        if (!FTBL.filmCheckFadeTrackColour()) {
            if (!FTBL.DoesScenarioCharacterExist(QFEB_REN)) {
                FTBL.DebugLogToFileRemoved("ID_REN Not Found!!! NPC_Load\n");
                FTBL.ForceLoadNpc(QFEB_REN);
            }
            if (FTBL.ForceLoadNpc(QFEB_REN)) {
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(QFEB_RYO), 3);
                FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(QFEB_RYO), 7, 0);
                EVT_AT_SetMot(QFEB_RYO, 0xf003, 0.0f, 1.0f, 0x80000000, 0);
                EVT_AT_SetPos(QFEB_RYO, -1, qfebChoreographyPositions[entry][0].x, 128.8f, qfebChoreographyPositions[entry][0].z, 0);
                EVT_AT_SetAngY(QFEB_RYO, -1, qfebChoreographyPositions[entry][0].yaw, 0);
                FTBL.EV_SetCharacterAlphaEnabled(QFEB_REN, 0);
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(QFEB_REN), 3);
                FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(QFEB_REN), 7, 0);
                EVT_AT_SetMot(QFEB_REN, 0xf003, 0.0f, 1.0f, 0x80000000, 0);
                EVT_AT_SetPos(QFEB_REN, -1, qfebChoreographyPositions[entry][1].x, 128.8f, qfebChoreographyPositions[entry][1].z, 0);
                EVT_AT_SetAngY(QFEB_REN, -1, qfebChoreographyPositions[entry][1].yaw, 0);
                FTBL.EV_LoadCutsceneFromCurrentScene("0302E000");
                FTBL.EV_LoadAudio("0302E000", 0);
                FTBL.EV_spmBeginSampleExecute(0);
                FTBL.adfsSetCameraIdxForChara(QFEB_RKN, QFEB_CAMERA_RKN);
                FTBL.adfsSetCameraIdxForChara(QFEB_ANC, QFEB_CAMERA_ANC);
                FTBL.adfsSetCameraIdxForChara(QFEB_MRC, QFEB_CAMERA_MRC);
                work->phase = 4;
                FTBL.HLib_SetupTaskDelay(15);
            }
        }
        break;
    case 4:
        if (work->phaseFrames == 15)
            PrepareQfebBattleChoreography(work);
        if (++work->phaseFrames > 445)
            work->phase = 5;
        else if (work->phaseFrames == 415)
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    case 5:
        PrepareQfebBattleStances(work);
        break;
    case 6:
        FTBL.HLib_SetupTaskDelay(work->delay);
        work->phase = 7;
        break;
    case 7:
        work->battleFade = 0;
        work->exitFade = 0;
        work->battleFrames = 360;
        StartQfebBattle(work);
        break;
    case 8:
        UpdateQfebBattleTaunts(work);
        if (work->battleUpdates++ > 180) {
            if (!work->battleFade) {
                FTBL.vfmDispatchControl(10, 0, 0, 1);
                FTBL.SetScreenFilmFade(360, 0, 0, 0, 0, 185, 35, 20, 0);
                work->battleFade = 1;
            }
            if (--work->battleFrames == 0) {
                work->battleFrames = 210;
                work->phase = 9;
            }
        }
        break;
    case 9:
        if (!work->exitFade) {
            FTBL.SetScreenFilmFade(210, 185, 35, 20, 0, 255, 0, 0, 0);
            work->exitFade = 1;
        }
        if (--work->battleFrames == 0) {
            FTBL.check_dead_status(1);
            FTBL.EV_NPC_NewLoadSw_0(0);
            FTBL.EV_ResetFreeTalkFlags();
            FTBL.EV_FreeAuthorPak(0, "0302");
            work->phase = 10;
        }
        break;
    case 10:
        if (!FTBL.filmCheckFadeTrackColour()) {
            work->battleFrames = 60;
            work->phaseFrames = 0;
            FTBL.EV_FreeAuthorPak(1, "S2BV");
            work->phase = 13;
        }
        break;
    case 11:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.shenToggleAreaFlags(1);
            FTBL.qronRepositionPlayerForFloor(7);
            work->battleFrames = 60;
            FTBL.HLib_SetupTaskDelay(30);
            work->phase = 16;
        }
        break;
    case 12:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.shenToggleAreaFlags(1);
            FTBL.qronRepositionPlayerForFloor(9);
            work->battleFrames = 60;
            FTBL.HLib_SetupTaskDelay(30);
            work->phase = 16;
        }
        break;
    case 13:
        if (!work->phaseFrames)
            QueueQfebAftermath();
        if (++work->phaseFrames > 320)
            work->phase = 14;
        else if (work->phaseFrames == 290)
            FTBL.spmFadeOut(0);
        break;
    case 14:
        FTBL.qronRepositionPlayerForFloor(7);
        FTBL.Queue_ST_LoadTitleFadeIn(QFEB_TITLE);
        work->phase = 15;
        break;
    case 15:
        if (--work->battleFrames == 0) {
            FTBL.ST_LoadTitleFade();
            work->battleFrames = 60;
            work->phase = 18;
            work->phaseFrames = 0;
        }
        break;
    case 16: {
        int index;
        FTBL.Queue_ST_LoadTitleFadeIn(QFEB_EXIT_TITLE);
        index = entry;
        if (FTBL.GetQronNextStream() == 9)
            index = 3;
        EVT_AT_SetMot(QFEB_RYO, 0xf002, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_RYO, -1, qfebExitPositions[index].x, 125.2f, qfebExitPositions[index].z, 0);
        EVT_AT_SetAngY(QFEB_RYO, -1, qfebExitPositions[index].yaw, 0);
        FTBL.npcStartPlayEntityAtPos(QFEB_REN, 0, &qfebRenExitPositions[index].position, qfebRenExitPositions[index].yaw);
        work->phase = 17;
        break;
    }
    case 17:
        if (--work->battleFrames == 0) {
            FTBL.ST_LoadTitleFade();
            work->battleFrames = 60;
            work->phase = 19;
        }
        break;
    case 18:
        if (--work->battleFrames == 0)
            FinishQfebDefeat(work);
        break;
    case 19:
        if (--work->battleFrames == 0)
            FinishQfebEscape(work);
        break;
    }
}

void PrepareQfebBattleStances(QfebEventWork *work)
{
    switch (work->entry) {
    case 1:
        EVT_AT_SetMot(QFEB_RYO, 306, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_RYO, -1, 232.37f, 128.8f, 142.94f, 0);
        EVT_AT_SetAngY(QFEB_RYO, -1, 355.79f, 0);
        EVT_AT_SetMot(QFEB_RKN, 3632, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_RKN, -1, 234.89f, 128.8f, 138.92f, 0);
        EVT_AT_SetAngY(QFEB_RKN, -1, 185.79f, 0);
        EVT_AT_SetMot(QFEB_KHO, 3636, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_KHO, -1, 229.64f, 128.8f, 147.21f, 0);
        EVT_AT_SetAngY(QFEB_KHO, -1, 330.74f, 0);
        EVT_AT_SetMot(QFEB_SOY, 3640, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_SOY, -1, 237.86f, 128.8f, 142.98f, 0);
        EVT_AT_SetAngY(QFEB_SOY, -1, 90.8f, 0);
        EVT_AT_SetMot(QFEB_KIC, 3644, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_KIC, -1, 226.94f, 128.8f, 142.86f, 0);
        EVT_AT_SetAngY(QFEB_KIC, -1, 227.34f, 0);
        EVT_AT_SetMot(QFEB_ANC, 3648, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_ANC, -1, 235.08f, 128.8f, 147.39f, 0);
        EVT_AT_SetAngY(QFEB_ANC, -1, 30.73f, 0);
        EVT_AT_SetMot(QFEB_MRC, 3632, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_MRC, -1, 229.96f, 128.8f, 138.52f, 0);
        EVT_AT_SetAngY(QFEB_MRC, -1, 175.79f, 0);
        {
            Vector3 eye = {235.75f, 130.18f, 144.09f};
            Vector3 target = {232.37f, 129.74f, 142.97f};
            FTBL.camCtrlSetState(9);
            FTBL.camCtrlSetNextPosition(&eye);
            FTBL.camCtrlSetTargetPosition(&target);
        }
        break;
    case 2:
        EVT_AT_SetMot(QFEB_RYO, 306, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_RYO, -1, 214.79f, 128.8f, 174.59f, 0);
        EVT_AT_SetAngY(QFEB_RYO, -1, 360.0f, 0);
        EVT_AT_SetMot(QFEB_RKN, 3632, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_RKN, -1, 216.13f, 128.8f, 172.6f, 0);
        EVT_AT_SetAngY(QFEB_RKN, -1, 149.96f, 0);
        EVT_AT_SetMot(QFEB_KHO, 3636, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_KHO, -1, 218.13f, 128.8f, 173.88f, 0);
        EVT_AT_SetAngY(QFEB_KHO, -1, 84.97f, 0);
        EVT_AT_SetMot(QFEB_SOY, 3640, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_SOY, -1, 212.92f, 128.8f, 172.9f, 0);
        EVT_AT_SetAngY(QFEB_SOY, -1, 209.94f, 0);
        EVT_AT_SetMot(QFEB_KIC, 3644, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_KIC, -1, 216.46f, 128.8f, 177.59f, 0);
        EVT_AT_SetAngY(QFEB_KIC, -1, 21.58f, 0);
        EVT_AT_SetMot(QFEB_ANC, 3648, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_ANC, -1, 212.14f, 128.8f, 175.77f, 0);
        EVT_AT_SetAngY(QFEB_ANC, -1, 285.01f, 0);
        EVT_AT_SetMot(QFEB_MRC, 3632, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_MRC, -1, 214.62f, 128.8f, 171.67f, 0);
        EVT_AT_SetAngY(QFEB_MRC, -1, 169.95f, 0);
        {
            Vector3 eye = {219.7723f, 130.6452f, 172.8159f};
            Vector3 target = {215.5634f, 130.0f, 173.7166f};
            FTBL.camCtrlSetState(9);
            FTBL.camCtrlSetNextPosition(&eye);
            FTBL.camCtrlSetTargetPosition(&target);
        }
        break;
    case 3:
        EVT_AT_SetMot(QFEB_RYO, 306, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_RYO, -1, 194.96f, 128.8f, 150.7f, 0);
        EVT_AT_SetAngY(QFEB_RYO, -1, 265.68f, 0);
        EVT_AT_SetMot(QFEB_RKN, 3632, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_RKN, -1, 196.21f, 128.8f, 147.84f, 0);
        EVT_AT_SetAngY(QFEB_RKN, -1, 164.95f, 0);
        EVT_AT_SetMot(QFEB_KHO, 3636, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_KHO, -1, 193.72f, 128.8f, 153.48f, 0);
        EVT_AT_SetAngY(QFEB_KHO, -1, 353.59f, 0);
        EVT_AT_SetMot(QFEB_SOY, 3640, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_SOY, -1, 198.14f, 128.8f, 151.24f, 0);
        EVT_AT_SetAngY(QFEB_SOY, -1, 86.86f, 0);
        EVT_AT_SetMot(QFEB_KIC, 3644, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_KIC, -1, 191.6f, 128.8f, 150.71f, 0);
        EVT_AT_SetAngY(QFEB_KIC, -1, 277.66f, 0);
        EVT_AT_SetMot(QFEB_ANC, 3648, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_ANC, -1, 196.77f, 128.8f, 153.56f, 0);
        EVT_AT_SetAngY(QFEB_ANC, -1, 24.99f, 0);
        EVT_AT_SetMot(QFEB_MRC, 3632, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(QFEB_MRC, -1, 193.2f, 128.8f, 147.91f, 0);
        EVT_AT_SetAngY(QFEB_MRC, -1, 194.95f, 0);
        {
            Vector3 eye = {195.0555f, 130.95961f, 156.4722f};
            Vector3 target = {195.6803f, 130.0f, 150.9868f};
            FTBL.camCtrlSetState(9);
            FTBL.camCtrlSetNextPosition(&eye);
            FTBL.camCtrlSetTargetPosition(&target);
        }
        break;
    }
    work->phase = 7;
}

void StartQfebBattle(QfebEventWork *work)
{
    unsigned char index = 0;
    FTBL.EV_DisableCharacterDisplay(QFEB_REN);
    FTBL.EV_ToggleCharacterFlag(QFEB_REN);
    FTBL.shenStartFGST(9, 20, 0, 6);
    FTBL.EV_vfBattleUtil(3, 32, 1);
    AddQfebGenerators(work);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QFEB_RYO), 0, 0, 0, 0, 0);
    FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_RYO), 0, 0x20000, 31);
    switch (work->entry) {
    case 1:
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QFEB_RKN), 0, 1, 1, 150, 2);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_RKN), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_RKN), 0, 0x200000, 2048);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QFEB_KHO), 0, 1, 1, 150, 3);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_KHO), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_KHO), 0, 0x200000, 2048);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QFEB_SOY), 0, 1, 1, 150, 4);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_SOY), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_SOY), 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 3, 3636, 234.89f, 128.8f, 138.92f, 185);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 4, 3640, 229.64f, 128.8f, 147.21f, 330);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 5, 3644, 237.86f, 128.8f, 142.98f, 90);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 6, 3648, 226.94f, 128.8f, 142.86f, 227);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 2, 3632, 235.08f, 128.8f, 147.39f, 30);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 3, 3636, 229.96f, 128.8f, 138.52f, 175);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 4, 3640, 234.89f, 128.8f, 138.92f, 185);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 5, 3644, 229.64f, 128.8f, 147.21f, 330);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 6, 3648, 237.86f, 128.8f, 142.98f, 90);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 2, 3632, 226.94f, 128.8f, 142.86f, 227);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 3, 3636, 229.96f, 128.8f, 138.52f, 175);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 4, 3640, 234.89f, 128.8f, 138.92f, 185);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 5, 3644, 229.64f, 128.8f, 147.21f, 330);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 6, 3648, 237.86f, 128.8f, 142.98f, 90);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.vfCopyPosFromEntity(0, 0, 232.37f, 142.94f, 6.0f);
        break;
    case 2:
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QFEB_ANC), 0, 1, 1, 150, 2);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_ANC), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_ANC), 0, 0x200000, 2048);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QFEB_KIC), 0, 1, 1, 150, 3);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_KIC), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_KIC), 0, 0x200000, 2048);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QFEB_KHO), 0, 1, 1, 150, 4);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_KHO), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_KHO), 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 4, 3640, 216.18f, 128.8f, 172.6f, 149);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 5, 3644, 218.13f, 128.8f, 173.88f, 84);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 6, 3648, 212.92f, 128.8f, 177.9f, 209);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 2, 3632, 216.46f, 128.8f, 177.59f, 21);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 3, 3636, 212.14f, 128.8f, 175.77f, 285);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 4, 3640, 214.62f, 128.8f, 171.67f, 169);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 5, 3644, 216.18f, 128.8f, 172.6f, 149);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 6, 3648, 218.13f, 128.8f, 177.88f, 84);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 2, 3632, 212.92f, 128.8f, 177.9f, 209);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 3, 3636, 212.14f, 128.8f, 175.77f, 285);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 4, 3640, 214.62f, 128.8f, 171.67f, 169);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 5, 3644, 216.18f, 128.8f, 172.6f, 149);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 6, 3648, 218.13f, 128.8f, 173.88f, 84);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.vfCopyPosFromEntity(0, 0, 214.8f, 174.6f, 4.0f);
        break;
    case 3:
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QFEB_MRC), 0, 1, 1, 150, 2);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_MRC), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_MRC), 0, 0x200000, 2048);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QFEB_ANC), 0, 1, 1, 150, 3);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_ANC), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_ANC), 0, 0x200000, 2048);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QFEB_SOY), 0, 1, 1, 150, 4);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_SOY), 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(QFEB_SOY), 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 3, 3636, 196.21f, 128.8f, 147.84f, 62);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 4, 3640, 193.72f, 128.8f, 153.48f, 190);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 5, 3644, 198.14f, 128.8f, 151.24f, 201);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 6, 3648, 191.6f, 128.8f, 150.71f, 147);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 2, 3632, 196.77f, 128.8f, 153.56f, 171);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 3, 3636, 193.2f, 128.8f, 147.91f, 170);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 4, 3640, 196.21f, 128.8f, 147.84f, 204);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 5, 3644, 193.72f, 128.8f, 153.48f, 165);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 6, 3648, 198.14f, 128.8f, 151.24f, 217);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 2, 3632, 191.6f, 128.8f, 150.71f, 82);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 3, 3636, 196.77f, 128.8f, 153.56f, 62);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 4, 3640, 193.2f, 128.8f, 147.91f, 88);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 5, 3644, 196.21f, 128.8f, 147.91f, 99);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.StartFightDefeat(150, 6, 3648, 193.72f, 128.8f, 153.56f, 181);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x400000, 128);
        FTBL.j_vfUpdateFlagsFor((void *)QFEB_DEAD, 0, 0x200000, 2048);
        FTBL.vfCopyPosFromEntity(0, 0, 194.96f, 150.7f, 4.0f);
        break;
    }
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_vfBattleUtil(3, 2048, 1);
    for (index = 0; qfebCharacters[index] != EVENT_CHARACTER_INVALID; index++) {
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(qfebCharacters[index]), 1);
        FTBL.SetScenarioCharacterType(qfebCharacters[index], 0);
    }
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QFEB_RYO), 1);
    FTBL.SetScenarioCharacterType(QFEB_RYO, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QFEB_RYO), 0, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(QFEB_RYO), 0, 1, 0);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(QFEB_RYO), 0);
    switch (work->entry) {
    case 1:
        FTBL.npcFree(QFEB_KIC);
        FTBL.npcFree(QFEB_ANC);
        FTBL.npcFree(QFEB_MRC);
        break;
    case 2:
        FTBL.npcFree(QFEB_SOY);
        FTBL.npcFree(QFEB_RKN);
        FTBL.npcFree(QFEB_MRC);
        break;
    case 3:
        FTBL.npcFree(QFEB_KIC);
        FTBL.npcFree(QFEB_RKN);
        FTBL.npcFree(QFEB_KHO);
        break;
    }
    FTBL.vfmQueueFightStart(6, 2, 1.0f, 1, -1);
    FTBL.EV_vfBattleUtil(3, 64, 1);
    FTBL.vfmDispatchControl(8, 0, (const void *)10, 1);
    work->phase = 8;
}

void AddQfebGenerators(QfebEventWork *work)
{
    switch (work->entry) {
    case 1:
        FTBL.vfAddGeneratorPoint(234.89f, 128.8f, 138.92f);
        FTBL.vfAddGeneratorPoint(229.64f, 128.8f, 147.21f);
        FTBL.vfAddGeneratorPoint(237.86f, 128.8f, 142.98f);
        FTBL.vfAddGeneratorPoint(226.94f, 128.8f, 142.86f);
        FTBL.vfAddGeneratorPoint(235.08f, 128.8f, 147.39f);
        FTBL.vfAddGeneratorPoint(229.96f, 128.8f, 138.52f);
        break;
    case 2:
        FTBL.vfAddGeneratorPoint(216.13f, 128.8f, 172.6f);
        FTBL.vfAddGeneratorPoint(218.13f, 128.8f, 173.88f);
        FTBL.vfAddGeneratorPoint(212.92f, 128.8f, 172.9f);
        FTBL.vfAddGeneratorPoint(216.46f, 128.8f, 177.59f);
        FTBL.vfAddGeneratorPoint(212.14f, 128.8f, 175.77f);
        FTBL.vfAddGeneratorPoint(214.62f, 128.8f, 171.67f);
        break;
    case 3:
        FTBL.vfAddGeneratorPoint(196.21f, 128.8f, 147.84f);
        FTBL.vfAddGeneratorPoint(193.72f, 128.8f, 153.48f);
        FTBL.vfAddGeneratorPoint(198.14f, 128.8f, 151.24f);
        FTBL.vfAddGeneratorPoint(191.6f, 128.8f, 150.71f);
        FTBL.vfAddGeneratorPoint(196.77f, 128.8f, 153.56f);
        FTBL.vfAddGeneratorPoint(193.2f, 128.8f, 147.91f);
        break;
    }
}

void PrepareQfebBattleChoreography(QfebEventWork *work)
{
    signed char entry = work->entry - 1;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_SetFaceLooks(QFEB_RYO, 0, 5, 1.0f, 0);
    EVT_AT_SetFaceLooks(QFEB_REN, 0, 5, 1.0f, 0);
    EVT_AT_SetFaceLooks(QFEB_RYO, 1, 10, 1.0f, 190);
    EVT_AT_SetFaceLooks(QFEB_REN, 1, 10, 1.0f, 150);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(qfebChoreographyNpcs[entry][0].character), 3);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(qfebChoreographyNpcs[entry][0].character), 15, 0);
    EVT_AT_SetMot(qfebChoreographyNpcs[entry][0].character, 0x7f03, 0.0f, 1.0f, 0x80000000, 10);
    EVT_AT_SetPos(qfebChoreographyNpcs[entry][0].character, -1, qfebChoreographyNpcs[entry][0].x, 128.8f, qfebChoreographyNpcs[entry][0].z, 0);
    EVT_AT_SetAngY(qfebChoreographyNpcs[entry][0].character, -1, qfebChoreographyNpcs[entry][0].yaw, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(qfebChoreographyNpcs[entry][1].character), 3);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(qfebChoreographyNpcs[entry][1].character), 15, 0);
    EVT_AT_SetMot(qfebChoreographyNpcs[entry][1].character, 0x7f06, 0.0f, 1.0f, 0x80000000, 20);
    EVT_AT_SetPos(qfebChoreographyNpcs[entry][1].character, -1, qfebChoreographyNpcs[entry][1].x, 128.8f, qfebChoreographyNpcs[entry][1].z, 0);
    EVT_AT_SetAngY(qfebChoreographyNpcs[entry][1].character, -1, qfebChoreographyNpcs[entry][1].yaw, 0);
    EVT_AT_SetMot(QFEB_RYO, 0x7f05, 0.0f, 1.0f, 0x80000000, 190);
    EVT_AT_SetPos(QFEB_RYO, -1, qfebChoreographyPositions[entry][0].x, 128.8f, qfebChoreographyPositions[entry][0].z, 0);
    EVT_AT_SetAngY(QFEB_RYO, -1, qfebChoreographyPositions[entry][0].yaw, 0);
    EVT_AT_SetMot(QFEB_REN, 0x7f04, 0.0f, 1.0f, 0x80000000, 150);
    EVT_AT_SetPos(QFEB_REN, -1, qfebChoreographyPositions[entry][1].x, 128.8f, qfebChoreographyPositions[entry][1].z, 0);
    EVT_AT_SetAngY(QFEB_REN, -1, qfebChoreographyPositions[entry][1].yaw, 0);
    switch (entry) {
    case 0:
        EVT_AT_SetVoice("0302C001", 110);
        break;
    case 1:
        EVT_AT_SetVoice("0302D001", 110);
        break;
    case 2:
        EVT_AT_SetVoice("0302G001", 110);
        break;
    }
    EVT_AT_SetCam(work->entry + 1999, QFEB_BASE, QFEB_BASE, 0, 0, 0);
    EVT_AT_SetCam(work->entry + 2049, QFEB_BASE, QFEB_BASE, 0, 0, 170);
}

void UpdateQfebBattleTaunts(QfebEventWork *work)
{
    unsigned char index;
    work->voiceFrames++;
    if (FTBL.cvsHasValidConverseData(0) != 1 && work->battleUpdates > 29 && work->voiceFrames > 89) {
        index = FTBL.shenGetRandomInt32(4);
        if (index == work->previousVoice) {
            index++;
            if (index > 3)
                index = 0;
        }
        switch (index) {
        case 0:
            FTBL.EV_PlayDialogue("0302C002");
            work->voiceFrames = 0;
            work->previousVoice = 0;
            break;
        case 1:
            FTBL.EV_PlayDialogue("0302E001");
            work->voiceFrames = 0;
            work->previousVoice = 1;
            break;
        case 2:
            FTBL.EV_PlayDialogue("0302F001");
            work->voiceFrames = 0;
            work->previousVoice = 2;
            break;
        case 3:
            FTBL.EV_PlayDialogue("0302H001");
            work->voiceFrames = 0;
            work->previousVoice = 3;
            break;
        }
    }
}

void QueueQfebAftermath(void)
{
    EVT_AT_SetSound(16, 10);
    EVT_AT_SetVoice("S2BVF007", 11);
    EVT_AT_SetSound(38, 30);
    EVT_AT_SetVoice("S2BVA030", 31);
    EVT_AT_SetVoice("S2BVE017", 50);
    EVT_AT_SetVoice("S2BVG014", 89);
    EVT_AT_SetVoice("S2BVF012", 124);
    EVT_AT_SetVoice("S2BVF005", 171);
    EVT_AT_SetSound(16, 193);
    EVT_AT_SetVoice("S2BVA007", 194);
    EVT_AT_SetSound(14, 231);
    EVT_AT_SetSound(14, 236);
    EVT_AT_SetVoice("S2BVG019", 238);
    EVT_AT_SetSound(34, 246);
    EVT_AT_SetSound(16, 273);
    EVT_AT_SetVoice("S2BVA012", 274);
    EVT_AT_SetSound(38, 293);
}

void QfebDebugExitCallback(void *data)
{
    FinishQfebDefeat((QfebEventWork *)data);
}

void FinishQfebDefeat(QfebEventWork *work)
{
    int index;
    switch (work->entry) {
    case 1:
        FTBL.EV_SetPlayerFlag(12, 1990, 0);
        break;
    case 2:
        FTBL.EV_SetPlayerFlag(12, 1991, 0);
        break;
    case 3:
        FTBL.EV_SetPlayerFlag(12, 1992, 0);
        break;
    }
    FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(QFEB_RYO), 0);
    FTBL.EV_SetCharacterAlphaEnabled(QFEB_REN, 1);
    FTBL.EV_EnsureCharacterDraw(QFEB_REN);
    FTBL.EV_EnableCharacterDisplay(QFEB_REN);
    FTBL.SetScenarioCharacterKillMode(QFEB_BASE, 1);
    ET_AT_KillWork(work->characters);
    FTBL.GetNpcBatchCountOverride(work->npcBatchOverride);
    FTBL.EV_ControlPackageResources(0, 6, qfebPackage, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "S2BV");
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.camCtrlSetNpcRadius(25.0f);
    FTBL.SetAreaFogParams();
    FTBL.SetPlayerControlEnabled(0);
    EventDebugUnregister(QFEB_DEBUG_ID);
    FTBL.EV_stepSetFlags(337, 0);
    FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(QFEB_RYO), 0, 5, 1.0f);
    FTBL.scenFreeEvent("QFEB.EV2");
    FTBL.EV_SetCanDoAreaJumpTo(1);
    index = FTBL.EV_GetPlayerFlag(11, 2062) != 0;
    if (FTBL.EV_GetPlayerFlag(11, 2063))
        index = 2;
    if (FTBL.EV_GetPlayerFlag(11, 2064))
        index = 0;
    FTBL.LoadAreaFunc_Wrapper_0("QTCV.EV3", 0);
    FTBL.ExecuteCurrAreaMainFunc("QTCV.EV3", qfebReturnArguments[index]);
}

void FinishQfebEscape(QfebEventWork *work)
{
    EventDebugUnregister(QFEB_DEBUG_ID);
    FTBL.SetScenarioCharacterKillMode(QFEB_BASE, 1);
    switch (work->entry) {
    case 1:
        FTBL.EV_SetPlayerFlag(12, 1990, 0);
        break;
    case 2:
        FTBL.EV_SetPlayerFlag(12, 1991, 0);
        break;
    case 3:
        FTBL.EV_SetPlayerFlag(12, 1992, 0);
        break;
    }
    FTBL.EV_SetCharaControllerFlags(QFEB_RYO, 2, 1);
    FTBL.EV_SetCharaControllerFlags(QFEB_RYO, 3, 1);
    SHENMUE_EVENT_BOUNDARY();
    ET_AT_KillWork(work->characters);
    FTBL.GetNpcBatchCountOverride(work->npcBatchOverride);
    FTBL.EV_ControlPackageResources(0, 6, qfebPackage, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0302");
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.camCtrlSetNpcRadius(25.0f);
    FTBL.SetAreaFogParams();
    FTBL.qronRepositionPlayerForFloor(FTBL.GetQronNextStream());
    FTBL.EV_SetCanDoAreaJumpTo(1);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
#endif
#else
#include "s2/areas/area_qfeb.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/fog.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include <cstring>
namespace shenmue::s2 {
namespace {
void QfebMainCallback(void *);
void QfebAuthorCallback(void *);
void QfebAuthorLoopCallback(void *);
void QueueQfebAuthor(shen_name author) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QfebAuthorCallback, 4, 0x0b, sizeof(QfebAuthorWork), QFEB_TASK);
    QfebAuthorWork *const work = static_cast<QfebAuthorWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 1;
    work->author = author;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0302");
}
void StartQfebAuthorLoop(QfebAuthorWork *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QfebAuthorLoopCallback, 4, 0x0b, sizeof(QfebAuthorLoopWork), QFEB_TASK);
    QfebAuthorLoopWork *const work = static_cast<QfebAuthorLoopWork *>(HLib::GetTaskParameter(task));
    work->frame = 0;
    work->parent = parent;
    mtwkSetExtraFlag(scenFindMotionwork(QFEB_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(QFEB_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(QFEB_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(QFEB_REN, false);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->author.name));
    parent->state = 2;
}
void FinishQfebAuthor(QfebAuthorWork *work) {
    using enum CameraState;
    HLib::DestroyTask(work->task);
    SetPlayerControlEnabled(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    const Vector3i &rotation = work->author.name == QFEB_AUTHORS[2].name ? qfebAuthorReturnRotation : QFEB_AUTHOR_DEFAULT_ROTATION;
    EV_AT_SetAngY(QFEB_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
}
void QfebAuthorCallback(void *data) {
    QfebAuthorWork *const work = static_cast<QfebAuthorWork *>(data);
    if (work->state == 1)
        StartQfebAuthorLoop(work);
    else if (work->state == 3)
        FinishQfebAuthor(work);
}
void QfebAuthorLoopCallback(void *data) {
    QfebAuthorLoopWork *const work = static_cast<QfebAuthorLoopWork *>(data);
    work->frame = static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        sbtLoadProgram();
        mtwkSetExtraFlag(scenFindMotionwork(QFEB_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(QFEB_RYO, true);
        mtwkSetExtraFlag(scenFindMotionwork(QFEB_REN), 6, 0);
        EV_SetCharacterAlphaEnabled(QFEB_REN, true);
        SetCurrentGameFlag(22, 0);
        work->parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}
void PrepareQfebBattleChoreography(QfebEventWork *work) {
    const std::int8_t entry = static_cast<std::int8_t>(work->entry - 1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QFEB_RYO), FacialExpressionId::Normal, 5, 1);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QFEB_REN), FacialExpressionId::Normal, 5, 1);
    EVT_AT_SetFaceLooks(QFEB_RYO, FacialExpressionId::Angry, 10, 1, 190);
    EVT_AT_SetFaceLooks(QFEB_REN, FacialExpressionId::Angry, 10, 1, 150);
    for (std::int32_t index = 0; index < 2; ++index) {
        const shen_name name = QFEB_CHOREOGRAPHY_NPCS[entry][index];
        mtwkSetRequestedRootMode(scenFindMotionwork(name), 3);
        mtwkSetFrameTime(scenFindMotionwork(name), 15, MotionRegion::LowerBody);
        EVT_AT_SetMot(name, index == 0 ? 0x7f03U : 0x7f06U, 0, 1, 0x80000000U, index == 0 ? 10 : 20);
        EventSetPosition position{
            .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_CHOREOGRAPHY_NPC_POSITIONS[entry][index], .delay = 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{
            .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_CHOREOGRAPHY_NPC_YAWS[entry][index], .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
    }
    for (std::int32_t index = 0; index < 2; ++index) {
        const shen_name name = index == 0 ? QFEB_RYO : QFEB_REN;
        EVT_AT_SetMot(name, index == 0 ? 0x7f05U : 0x7f04U, 0, 1, 0x80000000U, index == 0 ? 190 : 150);
        EventSetPosition position{
            .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_CHOREOGRAPHY_POSITIONS[entry][index], .delay = 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{.cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_CHOREOGRAPHY_YAWS[entry][index], .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
    }
    if (entry == 0)
        EVT_AT_SetVoice("0302C001", 1, 110); // @text EN: "Hey!\nWho are you!?" | JA: "おい！　てめぇ何モンだ！"
    else if (entry == 1)
        EVT_AT_SetVoice("0302D001", 1, 110); // @text EN: "You must be the intruders!" | JA: "侵入者ってのは、てめぇらか！？"
    else if (entry == 2)
        EVT_AT_SetVoice("0302G001", 1, 110); // @text EN: "Where did you rats come from?!" | JA: "てめぇら、どこのネズミだ！"
    EventSetCamera camera{
        .cleanup = 0, .camera = work->entry + 1999, .eye = QFEB_BASE, .target = QFEB_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(work->entry + 2049, QFEB_BASE, QFEB_BASE, 0, 0, 170);
}
void PrepareQfebBattleStances(QfebEventWork *work) {
    using enum CameraState;
    if (work->entry >= 1 && work->entry <= 3) {
        const std::int32_t entry = work->entry - 1;
        for (std::int32_t index = 0; index < 7; ++index) {
            const shen_name name = QFEB_FIGHT_CHARACTERS[index];
            EventSetMotion motion{
                .cleanup = 0, .character = name, .animation = QFEB_STANCE_MOTIONS[index], .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
            EVT_AT_SetMot_Callback(&motion);
            GetMotionFrameCount(QFEB_STANCE_MOTIONS[index]);
            EventSetPosition position{
                .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_BATTLE_POSITIONS[entry][index], .delay = 0};
            EVT_AT_SetPosRot(&position);
            EventSetAngle angle{.cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_BATTLE_YAWS[entry][index], .delay = 0};
            EVT_AT_SetAngY_Callback(&angle);
        }
        camCtrlSetState(Event);
        camCtrlSetNextPosition(QFEB_CAMERA_EYES[entry]);
        camCtrlSetTargetPosition(QFEB_CAMERA_TARGETS[entry]);
    }
    work->phase = 7;
}
void AddQfebGenerators(QfebEventWork *work) {
    if (work->entry >= 1 && work->entry <= 3) {
        for (std::int32_t index = 1; index < 7; ++index) {
            const Vector3 &point = QFEB_BATTLE_POSITIONS[work->entry - 1][index];
            vfAddGeneratorPoint(point.x, point.y, point.z);
        }
    }
}
void StartQfebBattle(QfebEventWork *work) {
    EV_DisableCharacterDisplay(QFEB_REN);
    EV_ToggleCharacterFlag(QFEB_REN);
    shenStartFGST(9, 20, 0, 6);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x20}, 1);
    AddQfebGenerators(work);
    shenInitFightData(ctrlGetCharaByName(QFEB_RYO), nullptr, 0, 0, 0, 0);
    vfUpdateFlagsFor(ctrlGetCharaByName(QFEB_RYO), 0, 0x20000, 31);
    if (work->entry >= 1 && work->entry <= 3) {
        for (std::int32_t index = 0; index < 3; ++index) {
            const shen_name name = QFEB_LIVE_ENEMIES[work->entry - 1][index];
            shenInitFightData(ctrlGetCharaByName(name), nullptr, 1, 1, 150, static_cast<std::uint8_t>(index + 2));
            vfUpdateFlagsFor(ctrlGetCharaByName(name), 0, 0x400000, 128);
            vfUpdateFlagsFor(ctrlGetCharaByName(name), 0, 0x200000, 2048);
        }
    }
    Character *const dead = reinterpret_cast<Character *>(static_cast<std::uintptr_t>(QFEB_DEAD.name));
    switch (work->entry) {
    case 1:
        PublishFightRespawn(150, 3, 3636, 234.89f, 128.8f, 138.92f, 185);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 229.64f, 128.8f, 147.21001f, 330);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 237.86f, 128.8f, 142.98f, 90);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 226.94f, 128.8f, 142.86f, 227);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 235.08f, 128.8f, 147.39f, 30);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 229.96001f, 128.8f, 138.52f, 175);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 234.89f, 128.8f, 138.92f, 185);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 229.64f, 128.8f, 147.21001f, 330);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 237.86f, 128.8f, 142.98f, 90);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 226.94f, 128.8f, 142.86f, 227);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 229.96001f, 128.8f, 138.52f, 175);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 234.89f, 128.8f, 138.92f, 185);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 229.64f, 128.8f, 147.21001f, 330);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 237.86f, 128.8f, 142.98f, 90);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        vfCopyPosFromEntity(0, nullptr, 232.37f, 142.94f, 6.0f);
        break;
    case 2:
        PublishFightRespawn(150, 4, 3640, 216.17999f, 128.8f, 172.60001f, 149);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 218.13f, 128.8f, 173.88f, 84);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 212.92f, 128.8f, 177.89999f, 209);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 216.46001f, 128.8f, 177.59f, 21);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 212.14f, 128.8f, 175.77f, 285);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 214.62f, 128.8f, 171.67f, 169);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 216.17999f, 128.8f, 172.60001f, 149);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 218.13f, 128.8f, 177.88f, 84);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 212.92f, 128.8f, 177.89999f, 209);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 212.14f, 128.8f, 175.77f, 285);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 214.62f, 128.8f, 171.67f, 169);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 216.17999f, 128.8f, 172.60001f, 149);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 218.13f, 128.8f, 173.88f, 84);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        vfCopyPosFromEntity(0, nullptr, 214.8f, 174.60001f, 4.0f);
        break;
    case 3:
        PublishFightRespawn(150, 3, 3636, 196.21001f, 128.8f, 147.84f, 62);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 193.72f, 128.8f, 153.48f, 190);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 198.14f, 128.8f, 151.24001f, 201);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 191.60001f, 128.8f, 150.71001f, 147);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 196.77f, 128.8f, 153.56f, 171);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 193.2f, 128.8f, 147.91f, 170);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 196.21001f, 128.8f, 147.84f, 204);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 193.72f, 128.8f, 153.48f, 165);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 198.14f, 128.8f, 151.24001f, 217);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 2, 3632, 191.60001f, 128.8f, 150.71001f, 82);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 3, 3636, 196.77f, 128.8f, 153.56f, 62);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 4, 3640, 193.2f, 128.8f, 147.91f, 88);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 5, 3644, 196.21001f, 128.8f, 147.91f, 99);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        PublishFightRespawn(150, 6, 3648, 193.72f, 128.8f, 153.56f, 181);
        vfUpdateFlagsFor(dead, 0, 0x400000, 128);
        vfUpdateFlagsFor(dead, 0, 0x200000, 2048);
        vfCopyPosFromEntity(0, nullptr, 194.96001f, 150.7f, 4.0f);
        break;
    default:
        break;
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    for (const shen_name *name = QFEB_CHARACTERS; name->name != EVENT_CHARACTER_INVALID.name; ++name) {
        SetPluralControlFor(ctrlGetCharaByName(*name), 1);
        SetScenarioCharacterType(*name, CharacterTypeDefault);
    }
    SetPluralControlFor(ctrlGetCharaByName(QFEB_RYO), 1);
    SetScenarioCharacterType(QFEB_RYO, CharacterTypeDefault);
    SetHighModelXYZ(ctrlGetCharaByName(QFEB_RYO), 0, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(QFEB_RYO), 0, 1, 0);
    SetFaceFlagsForChara_0(ctrlGetCharaByName(QFEB_RYO), FacialNodeOriginal);
    if (work->entry >= 1 && work->entry <= 3)
        for (shen_name name : QFEB_RELEASE_ENEMIES[work->entry - 1])
            EV_npcFree(name);
    vfmQueueFightStart(6, 2, 1, 1, -1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x40}, 1);
    vfmDispatchControl(FightControlOp::CameraParameter, shen_name{}, 10, 1);
    work->phase = 8;
}
void UpdateQfebBattleTaunts(QfebEventWork *work) {
    ++work->voiceFrames;
    if (cvsHasValidConverseData(nullptr) != 1 && work->battleUpdates >= 30 && work->voiceFrames >= 90) {
        std::uint32_t index = static_cast<std::uint32_t>(shenGetRandomInt32(4));
        if (index == work->previousVoice)
            index = (index + 1) % 4;
        switch (index) {
        case 0:
            EV_PlayDialogue("0302C002");
            break;
        case 1:
            EV_PlayDialogue("0302E001");
            break;
        case 2:
            EV_PlayDialogue("0302F001");
            break;
        case 3:
            EV_PlayDialogue("0302H001");
            break;
        }
        const std::uint16_t voice = static_cast<std::uint16_t>(index << 8);
        std::memcpy(&work->voiceFrames, &voice, sizeof(voice));
    }
}
void QueueQfebAftermath() {
    EVT_AT_SetSound(16, 10);
    EVT_AT_SetVoice("S2BVF007", 1, 11);
    EVT_AT_SetSound(38, 30);
    EVT_AT_SetVoice("S2BVA030", 1, 31);
    EVT_AT_SetVoice("S2BVE017", 1, 50); // @text EN: "Damn kid!" | JA: "このガキぃっ！"
    EVT_AT_SetVoice("S2BVG014", 1, 89); // @text EN: "You cocky kid!" | JA: "ふざけやがって！"
    EVT_AT_SetVoice("S2BVF012", 1, 124); // @text EN: "You're not going any further!" | JA: "これ以上、行かせるか！"
    EVT_AT_SetVoice("S2BVF005", 1, 171); // @text EN: "Die!" | JA: "死ねっ！"
    EVT_AT_SetSound(16, 193);
    EVT_AT_SetVoice("S2BVA007", 1, 194);
    EVT_AT_SetSound(14, 231);
    EVT_AT_SetSound(14, 236);
    EVT_AT_SetVoice("S2BVG019", 1, 238); // @text EN: "Drop dead!" | JA: "くたばりやがれっ！"
    EVT_AT_SetSound(34, 246);
    EVT_AT_SetSound(16, 273);
    EVT_AT_SetVoice("S2BVA012", 1, 274);
    EVT_AT_SetSound(38, 293);
}
void FinishQfebDefeat(QfebEventWork *work) {
    if (work->entry >= 1 && work->entry <= 3)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(1989 + work->entry), 0);
    SetPluralControlFor(ctrlGetCharaByName(QFEB_RYO), 0);
    EV_SetCharacterAlphaEnabled(QFEB_REN, true);
    EV_EnsureCharacterDraw(QFEB_REN);
    EV_EnableCharacterDisplay(QFEB_REN);
    SetCharaKill(QFEB_BASE, 1);
    ET_AT_KillWork(work->characters);
    GetNpcBatchCountOverride(work->npcBatchOverride);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QFEB"), 0);
    EV_FreeAuthorPak(0, "S2BV");
    sbtLoadProgram();
    shenSetClockAndCompassRenderTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    camCtrlSetNpcRadius(25);
    SetAreaFogParams();
    SetPlayerControlEnabled(0);
    EV_stepSetFlags(0, 337);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(QFEB_RYO), FacialExpressionId::Normal, 5, 1);
    scenFreeEvent("QFEB.EV2");
    EV_SetCanDoAreaJumpTo(1);
    std::int32_t index = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2062) != 0;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2063))
        index = 2;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2064))
        index = 0;
    LoadAreaFunc_Wrapper_0("QTCV.EV3", 0);
    ExecuteCurrAreaMainFunc("QTCV.EV3", reinterpret_cast<std::uintptr_t>(QFEB_RETURN_ARGUMENTS[index]));
}
void FinishQfebEscape(QfebEventWork *work) {
    SetCharaKill(QFEB_BASE, 1);
    if (work->entry >= 1 && work->entry <= 3)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(1989 + work->entry), 0);
    EV_SetCharaControllerFlags(QFEB_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(QFEB_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    ET_AT_KillWork(work->characters);
    GetNpcBatchCountOverride(work->npcBatchOverride);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QFEB"), 0);
    EV_FreeAuthorPak(0, "0302");
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    camCtrlSetNpcRadius(25);
    SetAreaFogParams();
    RepositionPlayerForQronFloor(static_cast<std::int32_t>(GetQronNextStream()));
    EV_SetCanDoAreaJumpTo(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
void QfebMainCallback(void *data) {
    QfebEventWork *const work = static_cast<QfebEventWork *>(data);
    std::int8_t entry = static_cast<std::int8_t>(work->entry - 1);
    Vector3 ryo;
    EV_GetCharaPosition(QFEB_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::Local);
    switch (work->phase) {
    case 0:
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        EV_LoadCutsceneFromCurrentScene("0302A000");
        EV_LoadAudio("0302A000", 0);
        SetMapControlStatusFlag(0, 0);
        SetCurrentGameFlag(22, 1);
        if (work->entry >= 1 && work->entry <= 3) {
            const std::int16_t readFlag = work->entry == 2 ? 1991 : 1990;
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, readFlag))
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(1989 + work->entry), 1);
            for (std::int32_t index = 1; index < 7; ++index) {
                const shen_name name = QFEB_FIGHT_CHARACTERS[index];
                EventSetPosition position{
                    .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_BATTLE_POSITIONS[entry][index], .delay = 0};
                EVT_AT_SetPosRot(&position);
                EventSetAngle angle{.cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_BATTLE_YAWS[entry][index], .delay = 0};
                EVT_AT_SetAngY_Callback(&angle);
            }
            QueueQfebAuthor(QFEB_AUTHORS[entry]);
        }
        work->phase = 1;
        break;
    case 1:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            EV_AT_SetPos(QFEB_REN, EventTransformFlags::World | EventTransformFlags::AllAxes, QFEB_AUTHOR_RETURN_POSES[entry].position);
            EV_AT_SetAngY(QFEB_REN, EventTransformFlags::World | EventTransformFlags::AllAxes, QFEB_AUTHOR_RETURN_POSES[entry].rotation);
            shenSetClockAndCompassRenderTo(0);
            ExecuteCleanupFuncForAreaWithCachedScene();
            work->phaseFrames = 0;
            for (std::int32_t flag = 1; flag <= 3; ++flag)
                EV_SetCharaControllerFlags(QFEB_RYO, flag, NpcCharacterControllerState::Enabled);
            work->phase = 2;
        }
        break;
    case 2:
        if (mtwkGetAnimationId(scenFindMotionwork(QFEB_RYO), MotionRegion::LowerBody) == 0xf009 ||
            mtwkGetAnimationId(scenFindMotionwork(QFEB_RYO), MotionRegion::LowerBody) == 0xf008) {
            SetPlayerControlEnabled(0);
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            shenToggleAreaFlags(2);
            work->phase = 11;
        } else if (mtwkGetAnimationId(scenFindMotionwork(QFEB_RYO), MotionRegion::LowerBody) == 0xf02b ||
            mtwkGetAnimationId(scenFindMotionwork(QFEB_RYO), MotionRegion::LowerBody) == 0xf02a) {
            SetPlayerControlEnabled(0);
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            shenToggleAreaFlags(2);
            work->phase = 12;
        } else if (ryoNotBusy() && ++work->idleFrames > 300 && GetQronNextStream() == 8 && static_cast<std::uint32_t>(work->entry - 2) <= 1) {
            SetPlayerControlEnabled(0);
            EVT_AT_SetMot(QFEB_RYO, 0xf003, 0, 1, 0x40000000U, 0);
            EV_SetNewSceneIndex();
            spmFadeOut();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            shenSetClockAndCompassRenderTo(1);
            HLib::SetupTaskDelay(30);
            work->phase = 3;
        } else if (GetSquaredDistanceXZ(&QFEB_PROXIMITY_POINTS[0], &ryo) < 4 || GetSquaredDistanceXZ(&QFEB_PROXIMITY_POINTS[1], &ryo) < 4 ||
            GetSquaredDistanceXZ(&QFEB_PROXIMITY_POINTS[2], &ryo) < 4) {
            SetPlayerControlEnabled(0);
            EVT_AT_SetMot(QFEB_RYO, 0xf003, 0, 1, 0x40000000U, 0);
            EV_SetNewSceneIndex();
            spmFadeOut();
            shenSetClockAndCompassRenderTo(1);
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            work->phase = 3;
        }
        break;
    case 3:
        if (!filmCheckFadeTrackColour()) {
            if (!DoesScenarioCharacterExist(QFEB_REN))
                EV_ForceLoadNpc(QFEB_REN);
            if (EV_ForceLoadNpc(QFEB_REN)) {
                for (std::int32_t index = 0; index < 2; ++index) {
                    const shen_name name = index == 0 ? QFEB_RYO : QFEB_REN;
                    if (index)
                        EV_SetCharacterAlphaEnabled(name, false);
                    mtwkSetRequestedRootMode(scenFindMotionwork(name), 3);
                    mtwkSetFrameTime(scenFindMotionwork(name), 7, MotionRegion::LowerBody);
                    EVT_AT_SetMot(name, 0xf003, 0, 1, 0x80000000U, 0);
                    EventSetPosition position{
                        .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_CHOREOGRAPHY_POSITIONS[entry][index], .delay = 0};
                    EVT_AT_SetPosRot(&position);
                    EventSetAngle angle{
                        .cleanup = 0, .character = name, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_CHOREOGRAPHY_YAWS[entry][index], .delay = 0};
                    EVT_AT_SetAngY_Callback(&angle);
                }
                EV_LoadCutsceneFromCurrentScene("0302A000");
                EV_LoadAudio("0302A000", 0);
                EV_spmBeginSampleExecute();
                adfsSetCameraIdxForChara(QFEB_RKN, QFEB_CAMERA_RKN);
                adfsSetCameraIdxForChara(QFEB_ANC, QFEB_CAMERA_ANC);
                adfsSetCameraIdxForChara(QFEB_MRC, QFEB_CAMERA_MRC);
                work->phase = 4;
                HLib::SetupTaskDelay(15);
            }
        }
        break;
    case 4:
        if (work->phaseFrames == 15)
            PrepareQfebBattleChoreography(work);
        if (++work->phaseFrames > 445)
            work->phase = 5;
        else if (work->phaseFrames == 415)
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    case 5:
        PrepareQfebBattleStances(work);
        break;
    case 6:
        HLib::SetupTaskDelay(work->delay);
        work->phase = 7;
        break;
    case 7:
        work->battleFade = 0;
        work->exitFade = 0;
        work->battleFrames = 360;
        StartQfebBattle(work);
        break;
    case 8: {
        UpdateQfebBattleTaunts(work);
        const std::uint32_t updates = work->battleUpdates++;
        if (updates > 180) {
            if (!work->battleFade) {
                vfmDispatchControl(FightControlOp::SpriteDepth, shen_name{}, 0, 1);
                SetScreenFilmFade(360, 0, 0, 0, 0, 185, 35, 20, 0);
                work->battleFade = 1;
            }
            if (work->battleFrames-- == 1) {
                work->battleFrames = 210;
                work->phase = 9;
            }
        }
        break;
    }
    case 9:
        if (!work->exitFade) {
            SetScreenFilmFade(210, 185, 35, 20, 0, 255, 0, 0, 0);
            work->exitFade = 1;
        }
        if (work->battleFrames-- == 1) {
            check_dead_status(1);
            EV_NPC_NewLoadSw(0);
            EV_ResetFreeTalkFlags();
            EV_FreeAuthorPak(0, "0302");
            work->phase = 10;
        }
        break;
    case 10:
        if (!filmCheckFadeTrackColour()) {
            work->battleFrames = 60;
            work->phaseFrames = 0;
            EV_FreeAuthorPak(1, "S2BV");
            work->phase = 13;
        }
        break;
    case 11:
    case 12:
        if (!filmCheckFadeTrackColour()) {
            shenToggleAreaFlags(1);
            RepositionPlayerForQronFloor(work->phase == 11 ? 7 : 9);
            work->battleFrames = 60;
            HLib::SetupTaskDelay(30);
            work->phase = 16;
        }
        break;
    case 13:
        if (!work->phaseFrames)
            QueueQfebAftermath();
        if (++work->phaseFrames > 320)
            work->phase = 14;
        else if (work->phaseFrames == 290)
            spmFadeOut();
        break;
    case 14:
        RepositionPlayerForQronFloor(7);
        Queue_ST_LoadTitleFadeIn(QFEB_TITLE);
        work->phase = 15;
        break;
    case 15:
        if (work->battleFrames-- == 1) {
            SetAuthorStateFlag(11, 1);
            ST_LoadTitleFade();
            work->battleFrames = 60;
            work->phaseFrames = 0;
            work->phase = 18;
        }
        break;
    case 16: {
        Queue_ST_LoadTitleFadeIn(QFEB_EXIT_TITLE);
        if (GetQronNextStream() == 9)
            entry = 3;
        EVT_AT_SetMot(QFEB_RYO, 0xf002, 0, 1, 0x40000000U, 0);
        EventSetPosition position{.cleanup = 0, .character = QFEB_RYO, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_EXIT_POSITIONS[entry], .delay = 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{.cleanup = 0, .character = QFEB_RYO, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_EXIT_YAWS[entry], .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
        PlaceNpcAtAreaPoint(QFEB_REN, shen_name{}, &QFEB_REN_EXIT_POSITIONS[entry], QFEB_REN_EXIT_YAWS[entry]);
        work->phase = 17;
        break;
    }
    case 17:
        if (work->battleFrames-- == 1) {
            ST_LoadTitleFade();
            work->battleFrames = 60;
            work->phase = 19;
        }
        break;
    case 18:
        if (work->battleFrames-- == 1)
            FinishQfebDefeat(work);
        break;
    case 19:
        if (work->battleFrames-- == 1)
            FinishQfebEscape(work);
        break;
    default:
        break;
    }
}
} // namespace
void QfebInit() {
}
void QfebMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QfebMainCallback, 5, 0x0b, sizeof(QfebEventWork), QFEB_TASK);
    QfebEventWork *const work = static_cast<QfebEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->phase = 0;
    work->reserved38 = 0;
    work->npcBatchOverride = GetNpcBatchCountOverride(46);
    work->reserved38 = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, QFEB_CHARACTERS);
    work->entry = static_cast<std::uint8_t>(argument);
    work->delay = 0;
    work->battleUpdates = 0;
    work->battleFrames = 0;
    work->battleFade = 0;
    work->exitFade = 0;
    work->idleFrames = 0;
    work->phaseFrames = 0;
    work->voiceFrames = 0;
    work->previousVoice = 0;
    work->phase = 0;
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QFEB"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QFEB"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QFEB"), 0);
    EV_SetNewSceneIndex();
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
    SetFogRed(9.1f);
    SetFogGreen(40);
    SetFogBlue(1);
    SetFogDistance(0x70000);
    ReinitFogTasks(1);
    camCtrlSetNpcRadius(40);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, QFEB_BASE);
    EventSetPosition position{
        .cleanup = 0, .character = QFEB_BASE, .parent = EVENT_CHARACTER_INVALID, .position = QFEB_BASE_POSITIONS[work->entry - 1], .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = QFEB_BASE, .parent = EVENT_CHARACTER_INVALID, .degrees = QFEB_BASE_YAW, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    EV_SetCanDoAreaJumpTo(0);
}
const ScriptAreaDefinition qfebScriptDefinition{.areaId = AREA_QFEB, .field04 = 0, .init = QfebInit, .main = QfebMain};
} // namespace shenmue::s2
#endif
