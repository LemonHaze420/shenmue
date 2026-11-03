#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_arkq.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void RunArkqEvent(void)
{
    HLibTask *task;
    ArkqWork *work;
    FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    task = FTBL.HLib_EnqueueTaskWithParameter(ArkqMainCallback, 5, 13, sizeof(ArkqWork), ARKQ_TASK);
    work = (ArkqWork *)FTBL.HLib_GetTaskParameter(task);
    FTBL.HLib_SetupTimedTask(task, 5);
    EventDebugRegister(ARKQ_DEBUG_ID, ArkqDebugExitCallback, work, 300000);
    FTBL.SetLoadingBusy();
    InitializeArkqWork(work, task);
}
#else
static const shen_name arkqCharacters[] = {ARKQ_RYO, ARKQ_REN, ARKQ_SAM, ARKQ_LLY, ARKQ_CLJ, ARKQ_INVALID};
static const int arkqOpenHandPose[57] = {0, 726, 63534, 0, 364, 64990, 0, 62806, 4368, 0, 65172, 46972, 0, 0, 57710, 0, 1638, 55526, 0, 546, 1456, 0, 0,
    52614, 0, 0, 54798, 0, 1092, 56254, 0, 546, 909, 0, 0, 56982, 0, 0, 56800, 0, 364, 58620, 0, 546, 909, 0, 0, 53524, 0, 0, 58620, 0, 546, 59348, 0, 546,
    1092};
static const int arkqKnifeHandPose[57] = {0, 726, 63534, 0, 364, 64990, 0, 62806, 4368, 0, 65172, 46972, 0, 0, 57710, 0, 1638, 55526, 0, 546, 1456, 0, 0,
    52614, 0, 0, 54798, 0, 1092, 56254, 0, 546, 909, 0, 0, 56982, 0, 0, 56800, 0, 364, 58620, 0, 546, 909, 0, 0, 53524, 0, 0, 58620, 0, 546, 59348, 0, 546,
    1092};
static int arkqKnifeTurnFrame = 0;
static int arkqKnifeLightFrame = 0;
static int arkqAuthorStopDelay = -1;
static unsigned int arkqGlints[ARKQ_GLINT_COUNT];
static int arkqReserved[4];

void InitializeArkqWork(ArkqWork *work, HLibTask *task)
{
    int i;
    FTBL.SetPlayerControlEnabled(0);
    work->task = task;
    work->state = 0;
    work->inputMasks[0] = 0x100;
    work->inputMasks[1] = 0x10;
    work->inputMasks[2] = 0x200;
    work->scratch[0] = 0;
    work->scratch[1] = 0;
    work->commandPhase = 0;
    FTBL.CheckIsArgEquZero(1, 0);
    for (i = 0; i < ARKQ_GLINT_COUNT; i++)
        arkqGlints[i] = 0xffffffffU;
}

int ReadArkqCharacters(ArkqWork *work)
{
    work->characters = Queue_EVT_AT_ReadChr(work->task, arkqCharacters);
    work->state = 1;
    return 0;
}

int SetupArkqResources(ArkqWork *work)
{
    EventDateTime time;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    if (time.hour > 18 || (time.hour == 6 && time.minute > 29)) {
        FTBL.SetIsSkySetup(0);
        FTBL.SetObjectFlagByIndex(1, 0);
    }
    FTBL.qteBattleUtil(8, 0, 6, 40, 0);
    FTBL.qteBattleUtil(9, 64, 4, 256, 0);
    FTBL.qteBattleUtil(10, 0, 0, 0, 0);
    FTBL.qteBattleUtil(1, 0, 0, 0, 0);
    FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.qteBattleUtil(2, 0, 0, 0, 0);
    LoadArkqPackages(work);
    SetupArkqCharacters();
    FTBL.SetWeatherUpdateEnabledTo(0);
    FTBL.EV_SetWeatherInstruction(15, 0);
    FTBL.EV_SetWeatherInstruction(1920, 0);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ARKQ_RYO));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ARKQ_REN));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ARKQ_SAM));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ARKQ_LLY));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ARKQ_CLJ));
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(ARKQ_RYO));
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(ARKQ_REN));
    FTBL.LoadBlurModelForCharacter_0(ARKQ_REN, "REN_B.MT7", 1.0f);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(ARKQ_LIGHT);
    FTBL.PushGlobalLightingProgram();
    work->state = 2;
    EventDebugCheck(ARKQ_DEBUG_ID, 4);
    return 0;
}

void SetupArkqCharacters(void)
{
    FTBL.CreateEventCharacter(3, 0, ARKQ_BASE);
    EVT_AT_SetPos(ARKQ_BASE, ARKQ_INVALID, 329.5f, 95.3f, 597.5f, 0);
    EVT_AT_SetAngY(ARKQ_BASE, ARKQ_INVALID, 90.0f, 0);
    EVT_AT_SetPos(ARKQ_DUMMY, ARKQ_BASE, 6.0f, 0.0f, -7.9f, 0);
    EVT_AT_SetAngY(ARKQ_DUMMY, ARKQ_BASE, 180.0f, 0);
    EVT_AT_SetPos(ARKQ_RYO, ARKQ_BASE, 10.83f, 0.0f, -2.3943f, 0);
    EVT_AT_SetAngY(ARKQ_RYO, ARKQ_BASE, 450.0f, 0);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_BASE, 4.0601997f, 0.0f, -10.9572f, 0);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_BASE, 224.0167f, 0);
    EVT_AT_SetPos(ARKQ_CLJ, ARKQ_BASE, 2.459f, 0.0f, -10.296f, 0);
    EVT_AT_SetAngY(ARKQ_CLJ, ARKQ_BASE, 229.032f, 0);
    EVT_AT_SetPos(ARKQ_SAM, ARKQ_BASE, 6.8f, 0.0f, -10.3f, 0);
    EVT_AT_SetAngY(ARKQ_SAM, ARKQ_BASE, 180.0f, 0);
    EVT_AT_SetPos(ARKQ_LLY, ARKQ_BASE, 7.8f, 0.0f, -10.9f, 0);
    EVT_AT_SetAngY(ARKQ_LLY, ARKQ_BASE, 180.0f, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(ARKQ_SAM), 66, 0, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(ARKQ_LLY), 66, 0, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(ARKQ_CLJ), 66, 0, 0);
}

void LoadArkqPackages(ArkqWork *work)
{
    FTBL.EV_ControlPackageResources(2, 2, "MPK00", 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0057A000");
    FTBL.EV_LoadAudio("0057A000", 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_FreeAuthorPak(1, "0057");
}

int FinishArkqResult(ArkqWork *work)
{
    int delay = 0;
    if ((unsigned int)g_entryPoint > 90) {
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work->commandPhase = 0;
        work->state = 2;
    } else {
        FTBL.spmFadeOut(0);
        if (work->state == 8) {
            FTBL.AddInventoryItem(6025, 1, 1);
            delay = 180;
        } else
            delay = 30;
        if (FTBL.EV_GetScreenFilmIsNotShowing())
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(ArkqCleanupCallback);
    }
    return delay;
}

void ArkqDebugExitCallback(void *data)
{
    ((ArkqWork *)data)->state = 9;
    ArkqCleanupCallback(data);
}

void ArkqCleanupCallback(void *data)
{
    ArkqWork *work = (ArkqWork *)data;
    unsigned short state;
    EventDateTime time;
    EventDebugUnregister(ARKQ_DEBUG_ID);
    FTBL.qteBattleUtil(5, 0, 0, 0, 0);
    state = work->state;
    FTBL.AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_INVALID);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(ARKQ_RYO));
    FTBL.EV_FreeAuthorPak(0, "0057");
    ET_AT_KillWork(work->characters);
    FTBL.FinishLoading();
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    if (state == 8) {
        FTBL.EV_stepSetFlags(123, 0);
        FTBL.EV_scenQueueLoad(g_actNumber, ARKQ_SUCCESS_MAP, 50);
    } else {
        FTBL.GetCurrentDateTime((DateTime *)&time);
        if (time.hour > 8)
            time.day++;
        time.hour = 1;
        time.minute = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        FTBL.EV_stepSetFlags(123, 1);
        FTBL.EV_scenQueueLoad(g_actNumber, ARKQ_FAILURE_MAP, 0);
    }
}

void ArkqMainCallback(void *data)
{
    ArkqWork *work = (ArkqWork *)data;
    int delay = 0;
    switch (work->state) {
    case 0:
        delay = ReadArkqCharacters(work);
        break;
    case 1:
        delay = SetupArkqResources(work);
        break;
    case 2:
        delay = BeginArkqIntro(work);
        break;
    case 3:
        delay = ArkqBeginKnifeDisplay(work);
        break;
    case 4:
        delay = WaitArkqQte(work);
        break;
    case 5:
        delay = PlayArkqResult(work);
        break;
    case 6:
        delay = BeginArkqSuccessAuthor(work);
        break;
    case 7:
        delay = BeginArkqFailureAuthor(work);
        break;
    case 8:
        delay = FinishArkqResult(work);
        break;
    case 9:
        delay = FinishArkqResult(work);
        break;
    }
    if (delay > 0)
        FTBL.HLib_SetupTaskDelay(delay);
}

int BeginArkqIntro(ArkqWork *work)
{
    EVT_AT_MenCtrl(60, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_INVALID);
    FTBL.EV_authSceneCtrl(-11);
    QueueSkAuthorStart(work->task, ARKQ_INTRO, ArkqIntroAuthorCallback, work, 0);
    work->state = 5;
    return 0;
}

int ArkqBeginKnifeDisplay(ArkqWork *work)
{
    Vector3 position = {0.09f, 0.05f, 0.0f};
    Vector3i rotation = {0x6000, 0xd000, 0xdddd};
    HLibTask *turn, *light;
    int i;
    for (i = 0; i < ARKQ_GLINT_COUNT; i++) {
        if (arkqGlints[i] != 0xffffffffU) {
            FTBL.FreeParticleEffect(0, arkqGlints[i]);
            arkqGlints[i] = 0xffffffffU;
            arkqGlints[i] = CreateArkqKnifeGlint();
            FTBL.ResumeParticleEffect(0, arkqGlints[i]);
        }
    }
    EVT_AT_SetCam(2002, ARKQ_BASE, ARKQ_BASE, 0, 0, 0);
    EVT_AT_SetCam(2001, ARKQ_BASE, ARKQ_BASE, 0, 0, 45);
    FTBL.AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_HAND_ANCHOR);
    FTBL.AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_HAND_ANCHOR);
    EVT_AT_SetPos(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, 0.0f, 0.0f, -0.07f, 0);
    EVT_AT_SetAng(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, 0.0f, 180.0f, 0.0f, 0);
    Queue_EVT_AT_Cling(ARKQ_HAND_ANCHOR, ARKQ_REN, 14, &position, &rotation, 0, 0);
    EVT_AT_SetMot(ARKQ_REN, 32519, 610.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(ARKQ_REN, 32519, 640.0f, 0.0f, ARKQ_MOTION_FLAGS, 45);
    EVT_AT_SetVoice("0057B003", 10);
    turn = FTBL.HLib_EnqueueTaskWithoutParameter(ArkqTurnKnifeCallback, 4, 11, ARKQ_TURN_TASK);
    light = FTBL.HLib_EnqueueTaskWithoutParameter(ArkqKnifeLightCallback, 4, 11, ARKQ_LIGHT_TASK);
    FTBL.HLib_SetupTimedTask(turn, 150);
    FTBL.HLib_SetupTimedTask(light, 155);
    work->state = 4;
    return 145;
}

void ArkqTurnKnifeCallback(void *data)
{
    EVT_AT_SetAng(ARKQ_KNIFE, ARKQ_KNIFE, -36.0f, 0.0f, 0.0f, 0);
    if (++arkqKnifeTurnFrame > 4) {
        arkqKnifeTurnFrame = 0;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void ArkqKnifeLightCallback(void *data)
{
    Vector3 position;
    Vector3 point = {0, 0, 0};
    Vector3 volume = {0, 0, 0};
    Vector3i rotation;
    FTBL.GetCharaPosition(ARKQ_KNIFE, 0xffffffffU, &position, ARKQ_WORLD);
    FTBL.GetScenarioCharacterEulerAngles(ARKQ_KNIFE, &rotation, ARKQ_WORLD);
    point.z = -0.05f - (float)arkqKnifeLightFrame * 0.01f;
    FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &point);
    FTBL.SetParticleEffectPosition(0, arkqGlints[arkqKnifeLightFrame / 3], &point, &volume);
    EVT_RepeatCall(ArkqSuspendGlintCallback, &arkqGlints[arkqKnifeLightFrame / 3], 6, 1);
    if (++arkqKnifeLightFrame > 9) {
        arkqKnifeLightFrame = 0;
        FTBL.HLib_DestroyCurrentTask();
    }
}

int WaitArkqQte(ArkqWork *work)
{
    if (work->commandPhase == 0) {
        FTBL.qteBattleUtil(3, 0, 0, 0, 0);
        work->commandPhase++;
    } else if (FTBL.qteBattleUtil(4, 0, 0, 0, 0)) {
        work->commandPhase = 0;
        work->state = 5;
    }
    return 0;
}

int PlayArkqResult(ArkqWork *work)
{
    int completed;
    int delay = 0;
    EVT_AT_CharaDisp(ARKQ_KNIFE_IMAGE, 0, 1);
    FTBL.EV_authSceneCtrl(-10);
    EVT_RepeatCall(ArkqFreeGlintsCallback, 0, 20, 1);
    completed = FTBL.qteBattleUtil(6, 0, 0, 0, 0);
    if (completed > 2)
        delay = ArkqAllDodges(delay);
    else {
        if (completed > 0)
            delay = ArkqFirstDodge(delay);
        if (completed > 1)
            delay = ArkqSecondDodge(delay);
        if (completed == 0)
            delay = ArkqFirstFailure(delay);
        if (completed == 1)
            delay = ArkqSecondFailure(delay);
        if (completed == 2)
            delay = ArkqThirdFailure(delay);
    }
    if (completed == 3)
        work->state = 6;
    else
        work->state = 7;
    EVT_AT_DispBlur(ARKQ_KNIFE, 1, 30, 1, 0, delay, 0);
    EVT_AT_CharaDisp(ARKQ_MONEY, 0, 1);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARKQ_RYO), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARKQ_RYO), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARKQ_REN), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARKQ_REN), 1, 1, 0);
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_RYO, 0, (void *)arkqOpenHandPose, 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_RYO, 1, (void *)arkqOpenHandPose, 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_REN, 0, (void *)arkqKnifeHandPose, 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_REN, 1, (void *)arkqOpenHandPose, 0, 0);
    EVT_AT_SetFaceLooks(ARKQ_RYO, 1, 16, 1.0f, 0);
    EVT_AT_SetFaceLooks(ARKQ_REN, 1, 16, 1.0f, 0);
    EVT_AT_CharaDraw(ARKQ_SAM, 0, 1);
    EVT_AT_CharaDraw(ARKQ_LLY, 0, 1);
    EVT_AT_CharaDraw(ARKQ_CLJ, 0, 1);
    EVT_AT_CharaDraw(ARKQ_SAM, delay + 1, 0);
    EVT_AT_CharaDraw(ARKQ_LLY, delay + 1, 0);
    EVT_AT_CharaDraw(ARKQ_CLJ, delay + 1, 0);
    return delay;
}

int ArkqFirstDodge(int delay)
{
    Vector3 grip = {0.09f, 0.05f, 0.0f};
    Vector3i gripRotation = {0x6000, 0xd000, 0xdddd};
    Vector3 parry = {0.11f, 0.03f, 0.03f};
    Vector3i parryRotation = {0x6000, 0xd000, 0xdddd};
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_REN, 1, (void *)arkqKnifeHandPose, 0, delay + 20);
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_REN, 0, (void *)arkqOpenHandPose, 0, delay + 20);
    Queue_EVT_AT_Cling(ARKQ_HAND_ANCHOR, ARKQ_REN, 14, &grip, &gripRotation, delay, 0);
    Queue_EVT_AT_Cling(ARKQ_HAND_ANCHOR, ARKQ_REN, 21, &parry, &parryRotation, delay + 20, 0);
    FTBL.AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_HAND_ANCHOR);
    EVT_AT_SetPos(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, 0.0f, 0.0f, -0.07f, delay);
    EVT_AT_SetAng(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, 180.0f, 180.0f, 0.0f, delay);
    EVT_AT_SetPos(ARKQ_RYO, ARKQ_BASE, 6.5f, 0.0f, -8.2f, 0);
    EVT_AT_SetAngY(ARKQ_RYO, ARKQ_BASE, 30.0f, 0);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, -0.36f, 0.0f, -1.9f, 0);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0.0f, 0);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, -0.0f, 0.0f, -1.2f, 0);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0.0f, 0);
    EVT_AT_SetMot(ARKQ_RYO, 5906, 30.0f, 1.0f, ARKQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(ARKQ_REN, 5900, 30.0f, 1.0f, ARKQ_MOTION_FLAGS, delay);
    EVT_AT_SetVoice("0057A020", delay + 15);
    EVT_AT_SetCam(3000, ARKQ_BASE, ARKQ_BASE, 0, 0, delay);
    EVT_AT_DispBlur(ARKQ_REN, 4, 15, 1, 0, 25, 0);
    return delay + 25;
}

int ArkqFirstFailure(int delay)
{
    Vector3 grip = {0.09f, 0.005f, 0.0f};
    Vector3i gripRotation = {0x6000, 0xd000, 0xdddd};
    FTBL.AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_HAND_ANCHOR);
    EVT_AT_SetPos(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, 0.0f, 0.0f, -0.07f, 0);
    EVT_AT_SetAng(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, 180.0f, 180.0f, 0.0f, 0);
    Queue_EVT_AT_Cling(ARKQ_HAND_ANCHOR, ARKQ_REN, 14, &grip, &gripRotation, 0, 0);
    EVT_AT_SetPos(ARKQ_RYO, ARKQ_BASE, 6.4682f, 0.0f, -8.2266f, delay);
    EVT_AT_SetAngY(ARKQ_RYO, ARKQ_BASE, 389.94629f, delay);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, 0.0f, 0.0f, -1.1f, delay);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0.0f, delay);
    EVT_AT_SetMot(ARKQ_RYO, 5904, 0.0f, 1.0f, ARKQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(ARKQ_REN, 5897, 0.0f, 1.0f, ARKQ_MOTION_FLAGS, delay);
    EVT_AT_SetCam(3013, ARKQ_BASE, ARKQ_BASE, 0, 0, delay);
    EVT_AT_SetCam(3014, ARKQ_BASE, ARKQ_BASE, 0, 0, delay + 30);
    EVT_AT_SetCam(3015, ARKQ_BASE, ARKQ_BASE, 0, 0, delay + 45);
    EVT_AT_DispBlur(ARKQ_REN, 4, 15, 1, 0, 20, delay + 20);
    EVT_AT_DispBlur(ARKQ_REN, 7, 15, 1, 0, 30, delay + 45);
    EVT_AT_SetVoice("0057A023", delay + 30);
    EVT_AT_SetVoice("0057B009", delay + 45);
    EVT_AT_SetFaceLooks(ARKQ_RYO, 6, 15, 1.0f, delay + 30);
    return delay + 90;
}

int ArkqSecondDodge(int delay)
{
    Vector3 parry = {0.11f, 0.03f, 0.03f};
    Vector3i parryRotation = {0x6000, 0xd000, 0xdddd};
    EVT_AT_SetCam(3002, ARKQ_BASE, ARKQ_BASE, 0, 0, delay);
    Queue_EVT_AT_Cling(ARKQ_HAND_ANCHOR, ARKQ_REN, 21, &parry, &parryRotation, delay + 20, 0);
    EVT_AT_DispBlur(ARKQ_REN, 4, 15, 1, 0, 25, delay);
    EVT_AT_SetVoice("0057A021", delay + 20);
    return delay + 25;
}

int ArkqSecondFailure(int delay)
{
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, 0.0f, 0.0f, -1.3f, delay);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0.0f, delay);
    EVT_AT_SetMot(ARKQ_RYO, 5912, 0.0f, 1.0f, ARKQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(ARKQ_REN, 5898, 0.0f, 1.0f, ARKQ_MOTION_FLAGS, delay);
    EVT_AT_SetCam(3011, ARKQ_BASE, ARKQ_BASE, 0, 0, delay);
    EVT_AT_SetCam(3012, ARKQ_BASE, ARKQ_BASE, 0, 0, delay + 20);
    EVT_AT_DispBlur(ARKQ_REN, 4, 15, 1, 0, 15, delay + 5);
    EVT_AT_DispBlur(ARKQ_REN, 7, 15, 1, 0, 30, delay + 20);
    EVT_AT_SetVoice("0057A024", delay + 10);
    EVT_AT_SetVoice("0057B009", delay + 35);
    EVT_AT_SetFaceLooks(ARKQ_RYO, 6, 15, 1.0f, delay + 20);
    return delay + 60;
}

int ArkqThirdFailure(int delay)
{
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, -0.5f, 0.0f, -1.1f, delay);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0.0f, delay);
    EVT_AT_SetMot(ARKQ_RYO, 5905, 0.0f, 1.0f, ARKQ_MOTION_FLAGS, delay);
    EVT_AT_SetMot(ARKQ_REN, 5899, 0.0f, 1.0f, ARKQ_MOTION_FLAGS, delay);
    EVT_AT_SetCam(3008, ARKQ_BASE, ARKQ_BASE, 0, 0, delay);
    EVT_AT_SetCam(3009, ARKQ_BASE, ARKQ_BASE, 0, 0, delay + 25);
    EVT_AT_DispBlur(ARKQ_REN, 4, 15, 1, 0, 65, delay);
    EVT_AT_SetVoice("0057A025", delay + 10);
    EVT_AT_SetVoice("0057B009", delay + 35);
    EVT_AT_SetFaceLooks(ARKQ_RYO, 6, 15, 1.0f, delay + 25);
    return delay + 65;
}

int ArkqAllDodges(int delay)
{
    Vector3 grip = {0.09f, 0.005f, 0.0f};
    Vector3i gripRotation = {0x6000, 0xd000, 0xdddd};
    Vector3 parry = {0.11f, 0.03f, 0.03f};
    Vector3i parryRotation = {0x6000, 0xd000, 0xdddd};
    FTBL.AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_HAND_ANCHOR);
    EVT_AT_SetFaceLooks(ARKQ_RYO, 0, 0, 1.0f, 0);
    EVT_AT_SetFaceLooks(ARKQ_REN, 0, 0, 1.0f, 0);
    EVT_AT_SetFaceLooks(ARKQ_RYO, 1, 5, 1.0f, 30);
    EVT_AT_SetFaceLooks(ARKQ_REN, 1, 5, 1.0f, 55);
    EVT_AT_SetPos(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, 0.0f, 0.0f, -0.07f, 0);
    EVT_AT_SetAng(ARKQ_KNIFE, ARKQ_HAND_ANCHOR, 180.0f, 180.0f, 0.0f, 0);
    Queue_EVT_AT_Cling(ARKQ_HAND_ANCHOR, ARKQ_REN, 14, &grip, &gripRotation, 0, 0);
    Queue_EVT_AT_Cling(ARKQ_HAND_ANCHOR, ARKQ_REN, 21, &parry, &parryRotation, 45, 0);
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_REN, 1, (void *)arkqKnifeHandPose, 0, 45);
    Queue_EVT_AT_CtrlHandUp_1(ARKQ_REN, 0, (void *)arkqOpenHandPose, 0, 45);
    EVT_AT_CharaDisp(ARKQ_KNIFE, 80, 1);
    EVT_AT_CharaDisp(ARKQ_HAND_ANCHOR, 80, 1);
    EVT_AT_SetPos(ARKQ_RYO, ARKQ_BASE, 6.5f, 0.0f, -8.2f, 0);
    EVT_AT_SetAngY(ARKQ_RYO, ARKQ_BASE, 30.0f, 0);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_RYO, 0.1f, 0.0f, -1.15f, 0);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_RYO, 0.0f, 0);
    EVT_AT_SetMot(ARKQ_RYO, 5906, 30.0f, 1.0f, ARKQ_MOTION_FLAGS, 0);
    EVT_AT_SetMot(ARKQ_REN, 5900, 30.0f, 1.0f, ARKQ_MOTION_FLAGS, 0);
    EVT_AT_SetMot(ARKQ_RYO, 5906, 55.0f, 0.0f, ARKQ_MOTION_FLAGS, 25);
    EVT_AT_SetMot(ARKQ_REN, 5900, 55.0f, 0.0f, ARKQ_MOTION_FLAGS, 25);
    EVT_AT_SetMot(ARKQ_RYO, 5906, 55.0f, 1.0f, ARKQ_MOTION_FLAGS, 35);
    EVT_AT_SetMot(ARKQ_REN, 5900, 55.0f, 1.0f, ARKQ_MOTION_FLAGS, 35);
    EVT_AT_SetMot(ARKQ_RYO, 5906, 80.0f, 0.0f, ARKQ_MOTION_FLAGS, 60);
    EVT_AT_SetMot(ARKQ_REN, 5900, 80.0f, 0.0f, ARKQ_MOTION_FLAGS, 60);
    EVT_AT_SetMot(ARKQ_RYO, 5906, 80.0f, 1.0f, ARKQ_MOTION_FLAGS, 70);
    EVT_AT_SetMot(ARKQ_REN, 5900, 80.0f, 1.0f, ARKQ_MOTION_FLAGS, 70);
    EVT_AT_SetVoice("0057A020", 15);
    EVT_AT_SetVoice("0057A021", 45);
    EVT_AT_SetVoice("0057A022", 70);
    EVT_AT_DispBlur(ARKQ_REN, 4, 7, 1, 0, 20, 10);
    EVT_AT_DispBlur(ARKQ_REN, 4, 7, 1, 0, 15, 35);
    EVT_AT_DispBlur(ARKQ_REN, 5, 7, 1, 0, 15, 60);
    EVT_AT_DispBlur(ARKQ_RYO, 1, 10, 1, 0, 15, 30);
    EVT_AT_DispBlur(ARKQ_RYO, 1, 10, 1, 0, 25, 70);
    EVT_AT_SetCam(3000, ARKQ_BASE, ARKQ_BASE, 0, 0, 0);
    EVT_AT_SetCam(3001, ARKQ_BASE, ARKQ_BASE, 0, 0, 25);
    EVT_AT_SetCam(3002, ARKQ_BASE, ARKQ_BASE, 0, 0, 35);
    EVT_AT_SetCam(3003, ARKQ_BASE, ARKQ_BASE, 0, 0, 50);
    EVT_AT_SetCam(3004, ARKQ_BASE, ARKQ_BASE, 0, 0, 60);
    EVT_AT_SetCam(3005, ARKQ_BASE, ARKQ_BASE, 0, 0, 80);
    EVT_AT_SetCam(3006, ARKQ_BASE, ARKQ_BASE, 0, 0, 130);
    EVT_AT_SetVoice("0057B005", 130);
    return 230;
}

int BeginArkqSuccessAuthor(ArkqWork *work)
{
    EVT_AT_CharaDisp(ARKQ_KNIFE, 0, 1);
    EVT_AT_CharaDisp(ARKQ_HAND_ANCHOR, 0, 0);
    Queue_EVT_AT_Cling(ARKQ_HAND_ANCHOR, 0, 0, 0, 0, 0, 1);
    FTBL.AT_SetLocalBaseChara(ARKQ_KNIFE, 0);
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(ARKQ_SUCCESS_LIGHT);
    FTBL.PushGlobalLightingProgram();
    QueueSkAuthorStart(work->task, ARKQ_SUCCESS_AUTHOR, ArkqSuccessAuthorNullsub, 0, 0);
    work->state = 8;
    return 0;
}

void ArkqSuccessAuthorNullsub(void *data)
{
}

int BeginArkqFailureAuthor(ArkqWork *work)
{
    Queue_EVT_AT_Cling(ARKQ_HAND_ANCHOR, 0, 0, 0, 0, 0, 1);
    FTBL.AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_INVALID);
    EVT_AT_CharaDisp(ARKQ_KNIFE, 0, 1);
    QueueSkAuthorStart(work->task, ARKQ_FAILURE_AUTHOR, 0, 0, 0);
    work->state = 9;
    return 0;
}

unsigned int CreateArkqKnifeGlint(void)
{
    Vector3 position = {0, 0, 0};
    Vector3 volume = {0, 0, 0};
    Vector3 velocity;
    Vector3 variance;
    unsigned int effect;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    velocity.z = 0.0f;
    variance.x = 0.0f;
    variance.y = 0.0f;
    variance.z = 0.0f;
    effect = FTBL.CreateSmokeEffect(&position, &volume, 100, 0.1f, 0, 0);
    FTBL.SetSmokeEffectFadeFactor(effect, 255);
    FTBL.SetSmokeEffectFadeFactor(effect, 112);
    FTBL.SetSmokeEffectFalloff(effect, 255);
    FTBL.SetParticleEffectSize(0, effect, 0.003f, 0.003f);
    FTBL.SetParticleEffectColour(0, effect, 0, 255, 255, 255);
    FTBL.SetParticleEffectBirthRate(0, effect, 0.050196078f);
    FTBL.SetParticleEffectInitialScale(0, effect, 0.08039216f);
    FTBL.SetParticleEffectSize(0, effect, 0.039215688f, 10.0f);
    FTBL.SetParticleEffectVelocity(0, effect, &velocity, &variance);
    return effect;
}

void ArkqIntroAuthorCallback(void *data)
{
    Vector3 position = {0, 0, 0};
    Vector3 point = {0.0f, 0.0f, -0.07f};
    Vector3 volume = {0, 0, 0};
    Vector3i rotation;
    int frame, i, stopDelay;
    int glintStart, glintEnd, remainingFrames;
    float glintScale;
    frame = FTBL.EV_authSceneCtrl(-6);
    switch (frame) {
    case 1:
        arkqAuthorStopDelay = -1;
        for (i = 0; i < ARKQ_GLINT_COUNT; i++) {
            if (arkqGlints[i] != 0xffffffffU) {
                FTBL.FreeParticleEffect(0, arkqGlints[i]);
                arkqGlints[i] = 0xffffffffU;
            }
            arkqGlints[i] = CreateArkqKnifeGlint();
        }
        EVT_AT_CharaDisp(ARKQ_MONEY, 0, 0);
        break;
    case 1320:
        FTBL.AddThisToCashOrSegaCoins(-500, 2);
        break;
    case 1460:
        break;
    case 1498:
        FTBL.qteBattleUtil(3, 0, 0, 0, 0);
        EVT_AT_CharaDisp(ARKQ_MONEY, 0, 1);
        break;
    case 1500:
        for (i = 0; i < ARKQ_GLINT_COUNT; i++)
            FTBL.ResumeParticleEffect(0, arkqGlints[i]);
        break;
    case 1505:
        EVT_AT_CharaDisp(ARKQ_KNIFE_IMAGE, 0, 0);
        EVT_AT_CharaDisp(ARKQ_KNIFE, 0, 0);
        FTBL.AssignIMGMParams(FTBL.ctrlGetCharaByName(ARKQ_KNIFE_IMAGE), 1, 1, 0.5f, 0);
        break;
    }
    FTBL.GetCharaPosition(ARKQ_LIGHT_ANCHOR, 0xffffffffU, &position, 0);
    FTBL.SetLightRuntimePosition(39, &position);
    glintStart = 1500;
    glintEnd = glintStart + 20;
    if (frame > glintStart && frame < glintEnd && (frame & 1) == 0) {
        FTBL.GetCharaPosition(ARKQ_HAND_ANCHOR, 0xffffffffU, &position, ARKQ_WORLD);
        FTBL.GetScenarioCharacterEulerAngles(ARKQ_HAND_ANCHOR, &rotation, ARKQ_WORLD);
        point.z = (remainingFrames = glintEnd - frame, glintScale = 0.006f, glintScale * remainingFrames + -0.18f);
        FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &point);
        FTBL.SetParticleEffectPosition(0, arkqGlints[remainingFrames / 3], &point, &volume);
        EVT_RepeatCall(ArkqSuspendGlintCallback, &arkqGlints[remainingFrames / 3], 4, 1);
    }
    if (FTBL.qteBattleUtil(4, 0, 0, 0, 0))
        arkqAuthorStopDelay = 15;
    stopDelay = arkqAuthorStopDelay;
    if (stopDelay != -1) {
        arkqAuthorStopDelay = stopDelay - 1;
        if (stopDelay == 0)
            FTBL.EV_authSceneCtrl(-4);
    }
}

void ArkqFreeGlintsCallback(void *data)
{
    int i;
    for (i = 0; i < ARKQ_GLINT_COUNT; i++) {
        if (arkqGlints[i] != 0xffffffffU) {
            FTBL.FreeParticleEffect(0, arkqGlints[i]);
            arkqGlints[i] = 0xffffffffU;
        }
    }
}

void ArkqSuspendGlintCallback(void *data)
{
    unsigned int effect = *(unsigned int *)data;
    if (effect != 0xffffffffU)
        FTBL.SuspendParticleEffect(0, effect);
}
#endif
#else
#include "s2/areas/area_arkq.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_call.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {
ArkqGlobals arkqGlobals;
namespace {
void ArkqSetupNullsub() {
}
void ArkqSuccessAuthorNullsub(void *) {
}

void SetupArkqCharacters() {
    CreateEventCharacter(CharacterCreationHighModel, nullptr, ARKQ_BASE);
    EVT_AT_SetPos(ARKQ_BASE, ARKQ_INVALID, {.x = 329.5f, .y = 95.3f, .z = 597.5f}, 0);
    EVT_AT_SetAngY(ARKQ_BASE, ARKQ_INVALID, 90, 0);
    EVT_AT_SetPos(ARKQ_DUMMY, ARKQ_BASE, {.x = 6, .y = 0, .z = -7.9f}, 0);
    EVT_AT_SetAngY(ARKQ_DUMMY, ARKQ_BASE, 180, 0);
    EVT_AT_SetPos(ARKQ_RYO, ARKQ_BASE, {.x = 10.83f, .y = 0, .z = -2.3943f}, 0);
    EVT_AT_SetAngY(ARKQ_RYO, ARKQ_BASE, 450, 0);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_BASE, {.x = 4.0601997f, .y = 0, .z = -10.9572f}, 0);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_BASE, 224.01669f, 0);
    EVT_AT_SetPos(ARKQ_CLJ, ARKQ_BASE, {.x = 2.459f, .y = 0, .z = -10.296f}, 0);
    EVT_AT_SetAngY(ARKQ_CLJ, ARKQ_BASE, 229.032f, 0);
    EVT_AT_SetPos(ARKQ_SAM, ARKQ_BASE, {.x = 6.8f, .y = 0, .z = -10.3f}, 0);
    EVT_AT_SetAngY(ARKQ_SAM, ARKQ_BASE, 180, 0);
    EVT_AT_SetPos(ARKQ_LLY, ARKQ_BASE, {.x = 7.8f, .y = 0, .z = -10.9f}, 0);
    EVT_AT_SetAngY(ARKQ_LLY, ARKQ_BASE, 180, 0);
    for (shen_name character : ARKQ_SPECTATORS)
        mtwkSetRegionFlag(scenFindMotionwork(character), 66, 0, MotionRegion::LowerBody);
}
std::int32_t SetupArkqResources(ArkqWork &work) {
    DateTime time;
    shenGetCurrentDateTime(time);
    if (time.hour >= 19 || (time.hour == 6 && time.minute >= 30)) {
        SetIsSkySetup(0);
        EV_SetObjectFlagByIndex(1, 0);
    }
    qteBattleUtil(BattleQteMode::SetWait, 0, 6, 40);
    qteBattleUtil(BattleQteMode::SetFirstCommands, 64, 4, 256);
    qteBattleUtil(BattleQteMode::SetLastCommands);
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("MPK00"), 0);
    EV_LoadCutsceneFromCurrentScene("0057A000");
    EV_LoadAudio("0057A000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0057");
    SetupArkqCharacters();
    SetWeatherUpdateEnabledTo(0);
    EV_SetWeatherInstruction(15, 0);
    EV_SetWeatherInstruction(0x780, 0);
    for (const shen_name *character = ARKQ_CHARACTERS; character->name != ARKQ_INVALID.name; ++character)
        SetLowQualityHands(ctrlGetCharaByName(*character));
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(ARKQ_RYO));
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(ARKQ_REN));
    LoadBlurModelForCharacter(ARKQ_REN, "REN_B.MT7", 1);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(ARKQ_LIGHT);
    PushGlobalLightingProgram();
    work.state = 2;
    return 0;
}
std::int32_t BeginArkqIntro(ArkqWork &work) {
    EventFilmControl film{.cleanup = 0,
        .frames = 60,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 0};
    EVT_AT_MenCtrl_Callback(&film);
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_INVALID);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_SK_AuthStart_Callback, 3, 0x0b, sizeof(StepAuthorStartWork), ARKQ_EVENT);
    StepAuthorStartWork *child = static_cast<StepAuthorStartWork *>(HLib::GetTaskParameter(task));
    child->task = task;
    child->parent = work.task;
    child->command = ARKQ_INTRO.name;
    child->callback = ArkqIntroAuthorCallback;
    child->phase = 1;
    child->callbackData = &work;
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(ARKQ_INTRO.name));
    child->callback(child->callbackData);
    HLib::SetupTimedTask(child->parent, 0);
    work.state = 5;
    snapAssignFrmCntFlags(1, 575);
    return 0;
}
std::int32_t BeginArkqSuccessAuthor(ArkqWork &work) {
    EV_DisableCharacterDisplay(ARKQ_KNIFE);
    EV_EnableCharacterDisplay(ARKQ_HAND_ANCHOR);
    EventCling detach;
    detach.cleanup = 0;
    detach.target = ARKQ_HAND_ANCHOR;
    detach.character = {};
    detach.mode = 1;
    detach.delay = 0;
    EVT_AT_Cling(&detach);
    AT_SetLocalBaseChara(ARKQ_KNIFE, shen_name{});
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(ARKQ_SUCCESS_LIGHT);
    PushGlobalLightingProgram();
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_SK_AuthStart_Callback, 3, 0x0b, sizeof(StepAuthorStartWork), ARKQ_EVENT);
    StepAuthorStartWork *child = static_cast<StepAuthorStartWork *>(HLib::GetTaskParameter(task));
    child->task = task;
    child->parent = work.task;
    child->command = ARKQ_SUCCESS_AUTHOR.name;
    child->callback = ArkqSuccessAuthorNullsub;
    child->phase = 1;
    child->callbackData = nullptr;
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(ARKQ_SUCCESS_AUTHOR.name));
    child->callback(child->callbackData);
    HLib::SetupTimedTask(child->parent, 0);
    work.state = 8;
    snapAssignFrmCntFlags(898, 20);
    return 0;
}
std::int32_t BeginArkqFailureAuthor(ArkqWork &work) {
    EventCling detach;
    detach.cleanup = 0;
    detach.target = ARKQ_HAND_ANCHOR;
    detach.character = {};
    detach.mode = 1;
    detach.delay = 0;
    EVT_AT_Cling(&detach);
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_INVALID);
    EV_DisableCharacterDisplay(ARKQ_KNIFE);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_SK_AuthStart_Callback, 3, 0x0b, sizeof(StepAuthorStartWork), ARKQ_EVENT);
    StepAuthorStartWork *child = static_cast<StepAuthorStartWork *>(HLib::GetTaskParameter(task));
    child->task = task;
    child->parent = work.task;
    child->command = ARKQ_FAILURE_AUTHOR.name;
    child->callback = nullptr;
    child->phase = 1;
    child->callbackData = nullptr;
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(ARKQ_FAILURE_AUTHOR.name));
    HLib::SetupTimedTask(child->parent, 0);
    work.state = 9;
    snapAssignFrmCntFlags(1, 21);
    return 0;
}
std::int32_t FinishArkqResult(ArkqWork &work) {
    if (g_entryPoint <= 90U) {
        spmFadeOut();
        const std::int32_t delay = work.state == 8 ? 180 : 30;
        if (work.state == 8)
            AddInventoryItem(0x1789, 1, 1);
        if (EV_GetScreenFilmIsNotShowing())
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(ArkqCleanupCallback);
        return delay;
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    work.commandPhase = 0;
    work.state = 2;
    return 0;
}
std::int32_t PlayArkqResult(ArkqWork &work) {
    EV_DisableCharacterDisplay(ARKQ_KNIFE_IMAGE);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    QueueEventCallFunction(ArkqFreeGlintsCallback, nullptr, 20);
    const std::int32_t completed = static_cast<std::int32_t>(qteBattleUtil(BattleQteMode::GetCompletedCount));
    std::int32_t delay = 0;
    if (completed > 2)
        delay = ArkqAllDodges();
    else {
        if (completed > 0)
            delay = ArkqFirstDodge();
        if (completed > 1)
            delay = ArkqSecondDodge(delay);
        if (completed == 0)
            delay = ArkqFirstFailure(delay);
        else if (completed == 1)
            delay = ArkqSecondFailure(delay);
        else if (completed == 2)
            delay = ArkqThirdFailure(delay);
    }
    work.state = completed == 3 ? 6 : 7;
    EVT_AT_DispBlur(ARKQ_KNIFE, 1, 30, 1, delay, 0);
    EV_DisableCharacterDisplay(ARKQ_MONEY);
    SetHighModelXYZ(ctrlGetCharaByName(ARKQ_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ARKQ_RYO), 1, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ARKQ_REN), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ARKQ_REN), 1, 1, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ARKQ_RYO), 0, reinterpret_cast<std::byte *>(arkqOpenHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ARKQ_RYO), 1, reinterpret_cast<std::byte *>(arkqOpenHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ARKQ_REN), 0, reinterpret_cast<std::byte *>(arkqKnifeHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ARKQ_REN), 1, reinterpret_cast<std::byte *>(arkqOpenHandPose), 0);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(ARKQ_RYO), 1, 16, 1);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(ARKQ_REN), 1, 16, 1);
    for (shen_name character : ARKQ_SPECTATORS) {
        EV_ToggleCharacterFlag(character);
        EV_DisableCharacterDisplay(character);
    }
    for (shen_name character : ARKQ_SPECTATORS) {
        if (delay == -1) {
            EV_EnsureCharacterDraw(character);
            EV_EnableCharacterDisplay(character);
        } else {
            HLib::Task *task = HLib::EnqueueTaskWithParameter(ArkqCharacterPauseCallback, 6, 0x0b, sizeof(ArkqCharacterPauseWork), ARKQ_EVENT);
            RegisterEventTask(task);
            *static_cast<ArkqCharacterPauseWork *>(HLib::GetTaskParameter(task)) = {
                .cleanup = 1, .character = character, .paused = 0, .delay = static_cast<float>(delay + 1)};
        }
    }
    return delay;
}
} // namespace

void ArkqCharacterPauseCallback(void *data) {
    ArkqCharacterPauseWork &work = *static_cast<ArkqCharacterPauseWork *>(data);
    if (work.delay <= 0) {
        if (work.paused) {
            EV_ToggleCharacterFlag(work.character);
            EV_DisableCharacterDisplay(work.character);
        } else {
            EV_EnsureCharacterDraw(work.character);
            EV_EnableCharacterDisplay(work.character);
        }
        if (work.cleanup) {
            UnregisterEventTask(HLib::GetCurrentTask());
            HLib::DestroyCurrentTask();
        }
    } else
        work.delay -= HLib::GetScaledDeltaTick();
}
std::uint32_t CreateArkqKnifeGlint() {
    const Vector3 zero{};
    const std::uint32_t effect = static_cast<std::uint32_t>(CreateSmokeEffect(zero, zero, 100, .1f, shen_name{}, 0));
    SetSmokeEffectFadeFactor(effect, 255);
    SetSmokeEffectFadeFactor(effect, 112);
    SetSmokeEffectFalloff(effect, 255);
    SetParticleEffectSize(0, effect, .003f, .003f);
    SetParticleEffectColour(0, effect, 0, 255, 255, 255);
    SetParticleEffectBirthRate(0, effect, .050196077674627304f);
    SetParticleEffectInitialScale(0, effect, .08039215952157974f);
    SetParticleEffectSize(0, effect, .03921568766236305f, 10);
    SetParticleEffectVelocity(0, effect, zero, zero);
    return effect;
}
void ArkqSuspendGlintCallback(void *data) {
    const std::uint32_t effect = *static_cast<std::uint32_t *>(data);
    if (effect != 0xffffffffU)
        SuspendParticleEffect(0, effect);
}
void ArkqFreeGlintsCallback(void *) {
    for (std::uint32_t &effect : arkqGlobals.glints) {
        if (effect != 0xffffffffU) {
            FreeParticleEffect(0, effect);
            effect = 0xffffffffU;
        }
    }
}
void ArkqTurnKnifeCallback(void *) {
    EventSetRawAngle angle{.cleanup = 0, .character = ARKQ_KNIFE, .parent = ARKQ_KNIFE, .rotation = {.x = 58983, .y = 0, .z = 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    if (++arkqGlobals.knifeTurnFrame >= 5) {
        arkqGlobals.knifeTurnFrame = 0;
        HLib::DestroyCurrentTask();
    }
}
void ArkqKnifeLightCallback(void *) {
    Vector3 position;
    Vector3i rotation;
    const Vector3 volume{};
    Vector3 point{.x = 0, .y = 0, .z = 0};
    EV_GetCharaPosition(ARKQ_KNIFE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(ARKQ_KNIFE, &rotation, EventTransformFlags::World);
    point.z = -.05f - static_cast<float>(arkqGlobals.knifeLightFrame) * .01f;
    scenApplyTransRot(&position, &rotation, &point);
    std::uint32_t *effect = &arkqGlobals.glints[arkqGlobals.knifeLightFrame / 3];
    SetParticleEffectPosition(0, *effect, &point, &volume);
    QueueEventCallFunction(ArkqSuspendGlintCallback, effect, 6);
    if (++arkqGlobals.knifeLightFrame >= 10) {
        arkqGlobals.knifeLightFrame = 0;
        HLib::DestroyCurrentTask();
    }
}
void ArkqIntroAuthorCallback(void *) {
    Vector3 position{.x = 0, .y = 0, .z = 0}, point{.x = 0, .y = 0, .z = -.07f};
    const Vector3 volume{};
    Vector3i rotation;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    switch (frame) {
    case 1:
        arkqGlobals.authorStopDelay = -1;
        for (std::uint32_t &effect : arkqGlobals.glints) {
            if (effect != 0xffffffffU) {
                FreeParticleEffect(0, effect);
                effect = 0xffffffffU;
            }
            effect = CreateArkqKnifeGlint();
        }
        EV_EnableCharacterDisplay(ARKQ_MONEY);
        break;
    case 1320:
        AddThisToCashOrSegaCoins(-500, ECashOrCoins::Cash2);
        break;
    case 1498:
        qteBattleUtil(BattleQteMode::Start);
        EV_DisableCharacterDisplay(ARKQ_MONEY);
        break;
    case 1500:
        for (std::uint32_t effect : arkqGlobals.glints)
            ResumeParticleEffect(0, effect);
        break;
    case 1505:
        EV_EnableCharacterDisplay(ARKQ_KNIFE_IMAGE);
        EV_EnableCharacterDisplay(ARKQ_KNIFE);
        AssignImageModuleParameters(ctrlGetCharaByName(ARKQ_KNIFE_IMAGE), 1, 1, .5f, 0);
        break;
    default:
        break;
    }
    EV_GetCharaPosition(ARKQ_LIGHT_ANCHOR, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    SetLightRuntimePosition(39, position);
    if (static_cast<std::uint32_t>(frame) - 1501U <= 18U && (frame & 1) == 0) {
        EV_GetCharaPosition(ARKQ_HAND_ANCHOR, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(ARKQ_HAND_ANCHOR, &rotation, EventTransformFlags::World);
        point.z = static_cast<float>(1520 - frame) * .006f - .18f;
        scenApplyTransRot(&position, &rotation, &point);
        std::uint32_t *effect = &arkqGlobals.glints[(1520 - frame) / 3];
        SetParticleEffectPosition(0, *effect, &point, &volume);
        QueueEventRepeatedFunction(ArkqSuspendGlintCallback, effect, 4, 1);
    }
    const std::int32_t result = static_cast<std::int32_t>(qteBattleUtil(BattleQteMode::GetResult));
    std::int32_t remaining;
    if (result)
        remaining = 15;
    else {
        remaining = arkqGlobals.authorStopDelay;
        if (remaining == -1)
            return;
    }
    arkqGlobals.authorStopDelay = remaining - 1;
    if (!remaining)
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
}
void ArkqCleanupCallback(void *data) {
    ArkqWork &work = *static_cast<ArkqWork *>(data);
    qteBattleUtil(BattleQteMode::RetireAndFree);
    const std::int16_t state = work.state;
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_INVALID);
    hndlSetFlags(ctrlGetCharaByName(ARKQ_RYO));
    EV_FreeAuthorPak(0, "0057");
    ET_AT_KillWork(work.characters);
    FinishLoading();
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (state == 8) {
        EV_stepSetFlags(0, 0x7b);
        EV_scenQueueLoad(g_actNumber, ARKQ_SUCCESS_MAP, 50);
    } else {
        DateTime time;
        shenGetCurrentDateTime(time);
        if (time.hour >= 1)
            ++time.day;
        time.hour = 1;
        time.minute = 0;
        SetCurrentDateTimeTo(time);
        EV_stepSetFlags(1, 0x7b);
        EV_scenQueueLoad(g_actNumber, ARKQ_FAILURE_MAP, 0);
    }
}
void ArkqMainCallback(void *data) {
    ArkqWork &work = *static_cast<ArkqWork *>(data);
    std::int32_t delay;
    switch (work.state) {
    case 0:
        work.characters = Queue_EVT_AT_ReadChr(work.task, ARKQ_CHARACTERS);
        work.state = 1;
        return;
    case 1:
        delay = SetupArkqResources(work);
        break;
    case 2:
        delay = BeginArkqIntro(work);
        break;
    case 3:
        delay = ArkqBeginKnifeDisplay(work);
        break;
    case 4:
        if (work.commandPhase) {
            if (qteBattleUtil(BattleQteMode::GetResult)) {
                work.commandPhase = 0;
                work.state = 5;
            }
        } else {
            qteBattleUtil(BattleQteMode::Start);
            ++work.commandPhase;
        }
        return;
    case 5:
        delay = PlayArkqResult(work);
        break;
    case 6:
        delay = BeginArkqSuccessAuthor(work);
        break;
    case 7:
        delay = BeginArkqFailureAuthor(work);
        break;
    case 8:
    case 9:
        delay = FinishArkqResult(work);
        break;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void InitializeArkqEvent() {
    arkqGlobals.authorStopDelay = -1;
    for (std::uint32_t &effect : arkqGlobals.glints)
        effect = 0;
    for (std::uint32_t &reserved : arkqGlobals.reserved2c)
        reserved = 0;
    arkqGlobals.knifeTurnFrame = 0;
    arkqGlobals.knifeLightFrame = 0;
}
bool InitializeArkqWork(ArkqWork &work, HLib::Task *task) {
    SetPlayerControlEnabled(0);
    work.task = task;
    work.scratch[0] = 0;
    work.scratch[1] = 0;
    work.commandPhase = 0;
    work.inputMasks[2] = 0x200;
    work.state = 0;
    work.inputMasks[0] = 0x100;
    work.inputMasks[1] = 0x10;
    const bool result = CheckIsArgEquZero(1);
    for (std::uint32_t &effect : arkqGlobals.glints)
        effect = 0xffffffffU;
    return result;
}
void RunArkqEvent(std::uintptr_t) {
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(ArkqMainCallback, 5, 0x0d, sizeof(ArkqWork), ARKQ_TASK);
    ArkqWork *work = static_cast<ArkqWork *>(HLib::GetTaskParameter(task));
    HLib::SetupTimedTask(task, 5);
    ArkqSetupNullsub();
    InitializeArkqWork(*work, task);
}
const ScriptAreaDefinition arkqScriptDefinition{.areaId = ARKQ_AREA, .field04 = 0, .init = InitializeArkqEvent, .main = RunArkqEvent};
} // namespace shenmue::s2::areas
#endif
