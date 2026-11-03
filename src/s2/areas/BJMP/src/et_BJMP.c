#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_bjmp.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
static const shen_name names[] = {BJMP_TGY, BJMP_REN, BJMP_INVALID};

void AreaBJMPMain(void)
{
    HLibTask *task;
    JumpWork *work;
    FTBL.DebugLogToFileRemoved("_-_-_-_-_-_-_-_-BillJump-_-_-_-_-_-_-_-_\n");
    FTBL.SetPlayerControlEnabled(0);
    task = FTBL.HLib_EnqueueTaskWithParameter(Auth1_Callback, 5, 11, sizeof(JumpWork), TASK_EVNT);
    work = (JumpWork *)FTBL.HLib_GetTaskParameter(task);
    work->self = task;
    work->retries = 0;
    FTBL.CreateEventCharacter(3, 0, BJMP_DUMMY);
    EVT_AT_SetPos(BJMP_DUMMY, BJMP_INVALID, 215.3f, 111.0f, 197.0f, 0);
    EVT_AT_SetAngY(BJMP_DUMMY, BJMP_INVALID, 270.0f, 0);
    Queue_EVT_AT_ReadChr(work->self, names);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)bjmpPackage, 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    BJMP_SET_GRAVITY(BJMP_RYO, 0);
    EventDebugRegister(BJMP_DEBUG_ID, DebugExit_Callback, work, 2000000000);
    FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.qteBattleUtil(2, 0, 0, 0, 0);
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
    FTBL.EV_ControlPackageResources(2, 4, bjmpPackage, 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene(bjmpCutscene);
    FTBL.EV_FreeAuthorPak(1, bjmpAuthorPak);
}
#else
const char bjmpPackage[] = "QJMP";
const char bjmpCutscene[] = "0218A000";
const char bjmpAuthorPak[] = "0218";

void Auth1_Callback(void *data)
{
    JumpWork *work = (JumpWork *)data;
    EventDateTime date;
    work->phase = 0;
    FTBL.EV_cuffsAttach(BJMP_RYO, 1, BJMP_REN, 0, 0.3f);
    FTBL.EV_EnableCharacterDisplay(BJMP_TGY);
    FTBL.EV_LoadAudio(bjmpCutscene, 0);
    EVT_AT_SetSample(30, 0, 0);
    FTBL.GetCurrentDateTime((DateTime *)&date);
    date.hour = 8;
    date.minute = 30;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.EV_lightLoad(BJMP_LIGHT);
    FTBL.AssignLightProgram(0);
    FTBL.AssignLightParams(1, 1);
    FTBL.AssignLightParams(1, 2);
    FTBL.AssignLightParams(1, 3);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 2);
    FTBL.HLib_SetCurrentTaskTo(Main_Callback);
    QueueSkAuthorStart(work->self, BJMP_AUTHOR_START, 0, 0, 0);
}

void Main_Callback(void *data)
{
    JumpWork *work = (JumpWork *)data;
    int delay = 0;
    float board[4];
    switch (work->phase) {
    case 0:
        board[0] = 1.0f;
        board[1] = 1.0f;
        board[2] = 1.0f;
        board[3] = 1.0f;
        FTBL.SetScenarioReflectorBoardValues(BJMP_RYO, board);
        FTBL.SetRefBoardParamsForCharaById(BJMP_RYO, 1);
        FTBL.SetRefBoardParamsForCharaById(BJMP_RYO, 1);
        FTBL.SetScenarioReflectorBoardValues(BJMP_REN, board);
        FTBL.SetRefBoardParamsForCharaById(BJMP_REN, 1);
        FTBL.SetRefBoardParamsForCharaById(BJMP_REN, 1);
        FTBL.SetScenarioReflectorBoardValues(BJMP_TGY, board);
        FTBL.SetRefBoardParamsForCharaById(BJMP_TGY, 1);
        FTBL.SetRefBoardParamsForCharaById(BJMP_TGY, 1);
        EventDebugCheck(BJMP_DEBUG_ID, 4);
        EVT_AT_SetPos(BJMP_RYO, BJMP_DUMMY, -0.4f, 0.0f, -5.4f, 0);
        EVT_AT_SetAngY(BJMP_RYO, BJMP_DUMMY, 180.0f, 0);
        EVT_AT_SetPos(BJMP_REN, BJMP_DUMMY, 0.4f, 0.0f, -4.9f, 0);
        EVT_AT_SetAngY(BJMP_REN, BJMP_DUMMY, 180.0f, 0);
        EVT_AT_SetPos(BJMP_TGY, BJMP_DUMMY, 0.1f, -0.2f, -8.9f, 0);
        EVT_AT_SetAngY(BJMP_TGY, BJMP_DUMMY, 180.0f, 0);
        FTBL.qteBattleUtil(8, 2, 6, 16, 0);
        FTBL.qteBattleUtil(9, 1, 1, 256, 0);
        FTBL.qteBattleUtil(10, 0, 0, 0, 0);
        FTBL.qteBattleUtil(1, 0, 0, 0, 0);
        BeginRun(work);
        work->phase++;
        delay = 28;
        break;
    case 1:
        EVT_AT_SetMot(BJMP_TGY, 0xf002, 0.0f, 1.0f, 0, 40);
        FTBL.DebugLogToFileRemoved(">>> cnt = %d, frame = %d\n", work->retries, work->retries * 4 + 16);
        FTBL.qteBattleUtil(8, 2, work->retries + 6, work->retries * 4 + 16, 0);
        FTBL.qteBattleUtil(3, 0, 0, 0, 0);
        work->phase++;
        break;
    case 2:
        if (FTBL.qteBattleUtil(4, 0, 0, 0, 0)) {
            if (FTBL.qteBattleUtil(6, 0, 0, 0, 0) == 3) {
                delay = JumpSuccess();
                work->phase++;
            } else
                Retry(work);
        }
        break;
    case 3:
        delay = BeginLanding();
        work->phase++;
        break;
    case 4:
        delay = EndLanding();
        work->phase++;
        break;
    case 5:
        FTBL.HLib_SetCurrentTaskTo(Exit_Callback);
        break;
    default:
        FTBL.DebugLogToFileRemoved(">>> stepcnt = %d\n", work->phase);
        FTBL.EventAssertNullsub(__FILE__, 201);
        break;
    }
    if (delay)
        FTBL.HLib_SetupTaskDelay(delay);
}

void BeginRun(JumpWork *work)
{
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(BJMP_RUN_LIGHT);
    FTBL.AssignLightProgram(0);
    FTBL.SetScenarioCharacterDrawControl(BJMP_RYO, 2, 0, 0.0f);
    FTBL.SetScenarioCharacterDrawControl(BJMP_REN, 2, 0, 0.0f);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(BJMP_RYO), 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(BJMP_REN), 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(BJMP_TGY), 0);
    EVT_AT_SetMot(BJMP_TGY, 9611, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(BJMP_RYO, 9577, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(BJMP_REN, 9546, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetCam(10, BJMP_DUMMY, BJMP_DUMMY, 0, 0, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
}

int JumpSuccess(void)
{
    FTBL.ReinitializeLights(1);
    FTBL.SetLightFlagForIndex(0, 2);
    FTBL.SetLightEntryIndex(0, 1);
    EVT_AT_SetVoice("0218A009", 20);
    EVT_AT_SetCam(110, BJMP_DUMMY, BJMP_DUMMY, 0, 0, 0);
    EVT_AT_SetMot(BJMP_RYO, 9575, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(BJMP_REN, 9543, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetPos(BJMP_TGY, BJMP_DUMMY, 0.0f, -100.0f, 0.0f, 0);
    EVT_AT_SetAngY(BJMP_TGY, BJMP_DUMMY, 180.0f, 0);
    EVT_AT_SetPos(BJMP_RYO, BJMP_DUMMY, -0.36f, -0.15f, -1.28f, 0);
    EVT_AT_SetAngY(BJMP_RYO, BJMP_DUMMY, 180.0f, 0);
    EVT_AT_SetPos(BJMP_REN, BJMP_DUMMY, 0.5f, -0.15f, -0.37f, 0);
    EVT_AT_SetAngY(BJMP_REN, BJMP_DUMMY, 180.0f, 0);
    FTBL.EV_DisableCharacterDisplay(BJMP_TGY);
    return 50;
}

int BeginLanding(void)
{
    EVT_AT_SetCam(100, BJMP_DUMMY, BJMP_DUMMY, 0, 0, 0);
    EVT_AT_SetMot(BJMP_RYO, 9575, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(BJMP_REN, 9543, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetPos(BJMP_RYO, BJMP_DUMMY, -0.36f, -0.15f, -1.28f, 0);
    EVT_AT_SetAngY(BJMP_RYO, BJMP_DUMMY, 180.0f, 0);
    EVT_AT_SetPos(BJMP_REN, BJMP_DUMMY, 0.5f, -0.15f, -0.37f, 0);
    EVT_AT_SetAngY(BJMP_REN, BJMP_DUMMY, 180.0f, 0);
    return 50;
}

int EndLanding(void)
{
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(BJMP_TGY));
    EVT_AT_SetFaceLooks(BJMP_TGY, 1, 10, 1.0f, 0);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(BJMP_TGY));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(BJMP_TGY), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(BJMP_TGY), 1, 1, 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(BJMP_TGY), 0, (void *)areaLoopHandPoses[BJMP_TGY_HAND_POSE], 0);
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(BJMP_TGY), 1, (void *)areaLoopHandPoses[BJMP_TGY_HAND_POSE], 0);
    FTBL.EV_EnableCharacterDisplay(BJMP_TGY);
    EVT_AT_SetCam(20, BJMP_DUMMY, BJMP_DUMMY, 0, 0, 0);
    EVT_AT_SetCam(25, BJMP_DUMMY, BJMP_DUMMY, 0, 0, 39);
    EVT_AT_SetCam(30, BJMP_DUMMY, BJMP_DUMMY, 0, 0, 80);
    EVT_AT_SetCam(35, BJMP_DUMMY, BJMP_DUMMY, 0, 0, 170);
    EVT_AT_SetCam(40, BJMP_DUMMY, BJMP_DUMMY, 0, 0, 230);
    EVT_AT_SetCam(50, BJMP_DUMMY, BJMP_DUMMY, 0, 0, 480);
    EVT_AT_SetSound(1405, 60);
    EVT_AT_MenCtrl(90, 0, 0, 0, 0, 255, 0, 0, 0, 510);
    EVT_AT_SetSample(90, 1, 510);
    EVT_AT_SetMot(BJMP_RYO, 9575, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(BJMP_REN, 9543, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(BJMP_RYO, 9577, 0.0f, 1.0f, 0, 280);
    EVT_AT_SetMot(BJMP_REN, 9546, 0.0f, 1.0f, 0, 280);
    EVT_AT_SetMot(BJMP_TGY, 9634, 180.0f, 1.0f, 0, 80);
    EVT_AT_SetPos(BJMP_RYO, BJMP_DUMMY, -0.36f, -0.15f, -1.28f, 0);
    EVT_AT_SetAngY(BJMP_RYO, BJMP_DUMMY, 180.0f, 0);
    EVT_AT_SetPos(BJMP_REN, BJMP_DUMMY, 0.5f, -0.15f, -0.37f, 0);
    EVT_AT_SetAngY(BJMP_REN, BJMP_DUMMY, 180.0f, 0);
    EVT_AT_SetPos(BJMP_TGY, BJMP_DUMMY, 0.0f, -0.2f, 0.8f, 80);
    EVT_AT_SetAngY(BJMP_TGY, BJMP_DUMMY, 180.0f, 80);
    EVT_AT_SetPos(BJMP_RYO, BJMP_DUMMY, 0.0f, -3.8f, 6.4f, 280);
    EVT_AT_SetAngY(BJMP_RYO, BJMP_DUMMY, 140.0f, 280);
    EVT_AT_SetPos(BJMP_REN, BJMP_DUMMY, 0.8f, -3.8f, 6.9f, 280);
    EVT_AT_SetAngY(BJMP_REN, BJMP_DUMMY, 140.0f, 280);
    EVT_AT_SetVoice("0218A013", 25);
    EVT_AT_SetVoice("0218B010", 120);
    EVT_AT_SetVoice("0218D005", 280);
    FTBL.HLib_EnqueueTaskWithoutParameter(FallingPosition_Callback, 4, 11, TASK_EVNT);
    return 600;
}

void Retry(JumpWork *work)
{
    if (work->retries <= 2)
        work->retries++;
    FTBL.DebugLogToFileRemoved("misscnt [%d]\n", work->retries);
    FTBL.EV_DisableCharacterDisplay(BJMP_TGY);
    FTBL.ReinitializeLights(1);
    FTBL.SetLightFlagForIndex(0, 2);
    FTBL.SetLightEntryIndex(0, 1);
    FTBL.HLib_SetCurrentTaskTo(Auth1_Callback);
    QueueSkAuthorStart(work->self, BJMP_AUTHOR_RETRY, 0, 0, 0);
}

void FallingPosition_Callback(void *data)
{
    Vector3 position;
    FTBL.AT_SetLocalBaseChara(BJMP_RYO, BJMP_DUMMY);
    FTBL.GetCharaPosition(BJMP_RYO, 0xffffffffU, &position, 0);
    FTBL.AT_SetLocalBaseChara(BJMP_RYO, BJMP_INVALID);
    EVT_AT_SetPos(BJMP_RYO, BJMP_RYO, 0.0f, -0.015f, -0.03f, 0);
    EVT_AT_SetPos(BJMP_REN, BJMP_REN, 0.0f, -0.015f, -0.03f, 0);
    if (position.y <= -1.08f)
        FTBL.HLib_DestroyCurrentTask();
}

void DebugExit_Callback(void *data)
{
    Exit_Callback(data);
}

void Exit_Callback(void *data)
{
    JumpWork *work = (JumpWork *)data;
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(BJMP_TGY));
    FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(BJMP_TGY));
    FTBL.qteBattleUtil(5, 0, 0, 0, 0);
    FTBL.FinishLoading();
    EventDebugUnregister(BJMP_DEBUG_ID);
    FTBL.EV_cuffsSetFlags();
    FTBL.EV_stepSetFlags(272, 0);
    FTBL.EV_stepSetFlags(317, 0);
    BJMP_SET_GRAVITY(BJMP_RYO, 1);
    FTBL.npcFree(BJMP_RYO);
    FTBL.npcLockUnlock(BJMP_RYO);
    FTBL.npcFree(BJMP_TGY);
    FTBL.npcLockUnlock(BJMP_TGY);
    FTBL.npcFree(BJMP_REN);
    FTBL.npcLockUnlock(BJMP_REN);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    FTBL.SetScenarioCharacterKillMode(BJMP_DUMMY, 1);
    FTBL.EV_ControlPackageResources(0, 6, bjmpPackage, 0, 0);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.EV_HLKillOtherHierarchyTask(work->self);
    FTBL.EV_scenQueueLoad(g_actNumber, BJMP_EXIT_AREA, 0);
}
#endif
#endif
