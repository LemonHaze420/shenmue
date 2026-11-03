#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/step_event.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void Queue_EVT_AT_NPCClear(void *argument)
{
    EventDateTime date;
    FTBL.SetLoadingBusy();
    FTBL.GetCurrentDateTime((DateTime *)&date);
    date.hour = 17;
    date.minute = 30;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
    if (g_currentMap == STEP_AREA)
        FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.SetPlayerControlEnabled(0);
    QueueClearNpcs(Queue_eSTEP_EVT_KaidanQteMain, 0);
}
#else
shen_name stepCharacterNames[] = {STEP_RYO, STEP_CHD, STEP_ISN, STEP_KRD, STEP_KR4, STEP_KF4, EVENT_CHARACTER_INVALID};
shen_name stepAuthorCharacterNames[] = {SHEN_NAME4('R', 'E', 'N', '_'), SHEN_NAME4('S', 'R', 'Y', '_'), SHEN_NAME4('J', 'O', 'P', '_'),
    SHEN_NAME4('S', 'G', 'T', '_'), STEP_TGY, SHEN_NAME4('W', 'O', 'N', '_'), SHEN_NAME4('E', 'S', 'Y', '_'), SHEN_NAME4('B', 'F', '8', '_'),
    SHEN_NAME4('B', 'F', '7', '_'), SHEN_NAME4('B', 'F', '1', '_'), SHEN_NAME4('B', 'F', '9', '_'), SHEN_NAME4('K', 'F', '5', '_'), EVENT_CHARACTER_INVALID};
static int secondOpponentThrown;
static int fourthCounterCombo;

void Queue_eSTEP_EVT_KaidanQteMain(void *data)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(eSTEP_EVT_KaidanQteMain_Callback, 4, 11, sizeof(StepEventWork), TASK_EVNT);
    StepEventWork *work = (StepEventWork *)FTBL.HLib_GetTaskParameter(task);
    *work = (StepEventWork){0};
    work->task = task;
    work->phase = 0;
    work->state = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, stepCharacterNames);
    EventDebugRegister(STEP_DEBUG_ID, eSTEP_EVT_EndKaidanQte_Callback, work, 3000000);
}

int STEP_SetupKaidanQte(StepEventWork *work)
{
    int index;
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"STEP", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "STEP", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "STEP", 0, 0);
    FTBL.CreateEventCharacter(3, 0, STEP_BASE);
    EVT_AT_SetPos(STEP_BASE, EVENT_CHARACTER_INVALID, 264.7f, 241.3f, 137.51999f, 0);
    EVT_AT_SetAngY(STEP_BASE, EVENT_CHARACTER_INVALID, 0.0f, 0);
    EVT_AT_SetPos(STEP_RYO, STEP_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(STEP_RYO, STEP_BASE, 0.0f, 0);
    FTBL.SetIsSkySetup(0);
    ResetSkQteRetryCount();
    FTBL.SetLoadingBusy();
    FTBL.SetPlayerGameFlags(2, 0);
    FTBL.EV_FreeAuthorPak(1, "S2BV");
    FTBL.EV_FreeAuthorPak(1, "0274");
    work->phase = 1;
    EventDebugCheck(STEP_DEBUG_ID, 4);
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(STEP_RYO));
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STEP_RYO), 1);
    for (index = 0; stepCharacterNames[index] != EVENT_CHARACTER_INVALID; index++) {
        FTBL.SetScenarioCharacterType(stepCharacterNames[index], 4);
        EVT_AT_SetFaceLooks(stepCharacterNames[index], 1, 0, 1.0f, 0);
    }
    return 0;
}

int STEP_FinishKaidanAuthor(StepEventWork *work)
{
    if (g_currentMap == STEP_AREA && (unsigned int)g_entryPoint > 90) {
        work->phase = 1;
        work->state = 0;
    } else
        FTBL.HLib_SetCurrentTaskTo(eSTEP_EVT_EndKaidanQte_Callback);
    FTBL.SetScenarioCharacterType(STEP_TGY, 7);
    FTBL.SetMapModelEnabled(1, 4);
    FTBL.spmFadeOut(0);
    FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    return 30;
}

void eSTEP_EVT_EndKaidanQte_Callback(void *data)
{
    StepEventWork *work = (StepEventWork *)data;
    FTBL.FinishLoading();
    FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(STEP_RYO));
    EVT_UTL_FreeNPCChara(stepAuthorCharacterNames);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.SetScenarioCharacterKillMode(STEP_BASE, 1);
    FTBL.EV_FreeAuthorPak(0, "S2BV");
    FTBL.EV_FreeAuthorPak(0, "0274");
    if (FTBL.EV_ControlPackageResources(0, 5, "STEPAUTH", 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, "STEPAUTH", 0, 0);
    FTBL.scenFreeEvent("STEP.EV3");
    EventDebugUnregister(STEP_DEBUG_ID);
    FTBL.scenSetTaskFlags(8);
    FTBL.Queue_ST_LoadTitleFadeIn(0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"TGLB", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.ExecuteCurrAreaMainFunc_Wrapper(2, 0);
}

void eSTEP_EVT_KaidanQteMain_Callback(void *data)
{
    StepEventWork *work = (StepEventWork *)data;
    int delay = 0;
    switch (work->phase) {
    case 0:
        delay = STEP_SetupKaidanQte(work);
        break;
    case 1:
        delay = STEP_RunKaidanQte(work);
        break;
    case 2:
        delay = STEP_RunKaidanAuthor(work);
        break;
    case 3:
        delay = STEP_FinishKaidanAuthor(work);
        break;
    }
    if (delay > 0)
        FTBL.HLib_SetupTaskDelay(delay);
}

int STEP_RunKaidanQte(StepEventWork *work)
{
    int delay = 0;
    int index;
    switch (work->state) {
    case 0:
        FTBL.EV_LoadAudio("0273A000", 0);
        FTBL.EV_spmBeginSampleExecute(0);
        EVT_AT_SetFaceLooks(STEP_RYO, 1, 0, 1.0f, 0);
        for (index = 0; stepCharacterNames[index] != EVENT_CHARACTER_INVALID; index++) {
            FTBL.EV_DisableCharacterDisplay(stepCharacterNames[index]);
            EVT_AT_SetFaceLooks(stepCharacterNames[index], 1, 0, 1.0f, 0);
        }
        FTBL.EV_EnableCharacterDisplay(STEP_RYO);
        delay = STEP_BeginFirstOpponent(work);
        work->state = 1;
        break;
    case 1:
        if (!work->qteSucceeded) {
            EVT_AT_SetVoice("S2BVA006", 20);
            QueueSkQteWait(&work->qteElapsed, 1024, 24, 16, STEP_FirstCounterSuccess, STEP_FirstCounterFailure, 20);
            work->state = 3;
        } else
            work->state = 4;
        break;
    case 3:
        if (work->qteSucceeded)
            work->state = 4;
        else
            work->state = 50;
        break;
    case 4:
        delay = STEP_BeginSecondOpponent(work);
        work->state++;
        break;
    case 5:
        if (work->qteSucceeded)
            work->state = 8;
        else {
            EVT_AT_SetCam(3305, STEP_BASE, STEP_BASE, 0, 0, 0);
            QueueSkQteWait(&work->qteElapsed, 2048, 24, 16, STEP_SecondCounterSuccess, STEP_SecondCounterFailure, 0);
            work->state = 7;
        }
        break;
    case 7:
        if (work->qteSucceeded)
            work->state = 8;
        else
            work->state = 50;
        break;
    case 8:
        delay = STEP_BeginThirdOpponent(work);
        work->state = 9;
        break;
    case 9:
        if (!work->qteSucceeded)
            work->state = 50;
        else
            work->state = 10;
        break;
    case 10:
        delay = STEP_BeginFourthOpponent(work);
        work->state = 11;
        break;
    case 11:
        if (work->qteSucceeded)
            QueueSkQteWait(&work->qteElapsed, 1, 24, 16, STEP_FourthComboSuccess, STEP_FourthComboFailure, 0);
        else
            QueueSkQteWait(&work->qteElapsed, 1, 24, 16, STEP_FourthCounterSuccess, STEP_FourthCounterFailure, 0);
        work->state++;
        break;
    case 12:
        EVT_AT_SetFaceLooks(STEP_RYO, 6, 8, 1.0f, 0);
        if (work->qteSucceeded) {
            delay = STEP_BeginFinalOpponent(work);
            work->state = 13;
        } else
            work->state = 50;
        break;
    case 13:
        if (work->qteSucceeded)
            work->state = 14;
        else
            work->state = 50;
        break;
    case 14:
        if (g_currentMap == STEP_AREA && (unsigned int)g_entryPoint > 90) {
            work->phase = 1;
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            delay = 30;
        } else {
            work->phase = 2;
            FTBL.spmFadeOut(0);
        }
        work->state = 0;
        break;
    case 50:
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        IncrementSkQteRetryCount();
        delay = 60;
        work->state = 0;
        break;
    }
    return delay;
}

int STEP_BeginFirstOpponent(StepEventWork *work)
{
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_SetPos(STEP_RYO, STEP_BASE, 0.2f, 0.0f, -1.0f, 0);
    EVT_AT_SetAngY(STEP_RYO, STEP_BASE, 0.0f, 0);
    STEP_PositionRunningOpponent(STEP_KR4, 0.0f, 0);
    STEP_PositionRunningOpponent(STEP_ISN, 0.0f, 120);
    QueueStepDescent(STEP_KR4, 2, 40);
    EVT_AT_SetMot(STEP_RYO, 7064, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetCam(3000, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetCam(3001, STEP_BASE, STEP_BASE, 0, 0, 40);
    QueueSkQteWait(&work->qteElapsed, 16, 24, 16, STEP_FirstDodgeSuccess, STEP_FirstDodgeFailure, 80);
    return 0;
}

int STEP_FirstDodgeSuccess(void)
{
    EVT_AT_SetCam(3100, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_DispBlur(STEP_RYO, 1, 10, 2, 0, 15, 0);
    EVT_AT_DispBlur(STEP_RYO, 6, 10, 2, 0, 10, 30);
    EVT_AT_SetPos(STEP_RYO, STEP_BASE, -0.1f, 0.0f, -1.6f, 0);
    EVT_AT_SetPos(STEP_KR4, STEP_RYO, 0.0f, 1.6f, -2.3f, 0);
    EVT_AT_SetAngY(STEP_KR4, STEP_RYO, 0.0f, 0);
    EVT_AT_SetMot(STEP_RYO, 7055, 0.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(STEP_KR4, 7095, 16.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetVoice("S2BVB001", 0);
    EVT_AT_SetVoice("S2BVB020", 30);
    return 30;
}

int STEP_FirstDodgeFailure(void)
{
    EVT_AT_SetCam(3101, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetPos(STEP_KR4, STEP_RYO, -0.2f, 1.8f, -2.5f, 0);
    EVT_AT_SetAngY(STEP_KR4, STEP_RYO, 0.0f, 0);
    EVT_AT_SetMot(STEP_RYO, 7060, 0.0f, 0.8f, 0x44000000U, 0);
    EVT_AT_SetMot(STEP_KR4, 7096, 31.0f, 0.8f, 0x44000000U, 0);
    EVT_AT_SetFaceLooks(STEP_RYO, 6, 8, 1.0f, 0);
    EVT_AT_SetVoice("S2BVA009", 0);
    return 45;
}

int STEP_FirstCounterSuccess(void)
{
    EVT_AT_SetCam(3102, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetPosAng(STEP_KR4, STEP_RYO, 0.2f, 0.3f, 0.0f, 0);
    EVT_AT_DispBlur(STEP_RYO, 5, 10, 2, 0, 30, 0);
    EVT_AT_SetFaceLooks(STEP_RYO, 1, 8, 1.0f, 10);
    EVT_AT_SetFaceLooks(STEP_KR4, 6, 8, 1.0f, 10);
    EVT_AT_SetMot(STEP_RYO, 7041, 0.0f, 1.0f, 0x4000000U, 0);
    EVT_AT_SetMot(STEP_KR4, 7092, 0.0f, 1.0f, 0x4000000U, 0);
    EVT_AT_SetVoice("S2BVA003", 10);
    return 15;
}

int STEP_FirstCounterFailure(void)
{
    EVT_AT_SetCam(3103, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetMot(STEP_RYO, 7044, 0.0f, 1.0f, 0x4000000U, 0);
    EVT_AT_SetMot(STEP_KR4, 7093, 0.0f, 1.0f, 0x4000000U, 0);
    EVT_AT_SetVoice("S2BVA012", 0);
    return 15;
}

int STEP_BeginSecondOpponent(StepEventWork *work)
{
    STEP_PositionRunningOpponent(STEP_KRD, 0.2f, 30);
    QueueStepDescent(STEP_ISN, 2, 0);
    EVT_AT_SetPos(STEP_RYO, STEP_BASE, 0.0f, 0.0f, -1.6f, 20);
    EVT_AT_SetPos(STEP_RYO, STEP_RYO, 0.0f, 0.456f, 0.0f, 41);
    EVT_AT_SetMot(STEP_RYO, 7051, 0.0f, 1.0f, 0, 20);
    EVT_AT_SetMot(STEP_RYO, 7050, 0.0f, 1.0f, 0x40000000U, 41);
    EVT_AT_SetCam(3300, STEP_BASE, STEP_BASE, 0, 0, 20);
    QueueSkQteWait(&work->qteElapsed, 2048, 24, 16, STEP_SecondDodgeSuccess, STEP_SecondDodgeFailure, 35);
    return 0;
}

int STEP_SecondDodgeSuccess(void)
{
    EVT_AT_SetCam(3301, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetPos(STEP_ISN, STEP_RYO, 0.0f, 1.6f, -2.1f, 0);
    EVT_AT_SetAngY(STEP_ISN, STEP_RYO, 0.0f, 0);
    EVT_AT_DispBlur(STEP_RYO, 1, 10, 2, 0, 15, 20);
    EVT_AT_SetMot(STEP_RYO, 7056, 0.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(STEP_ISN, 7101, 6.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetVoice("S2BVA001", 30);
    secondOpponentThrown = 1;
    return 5;
}

int STEP_SecondDodgeFailure(void)
{
    EVT_AT_SetCam(3302, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetFaceLooks(STEP_RYO, 6, 8, 1.0f, 15);
    EVT_AT_SetPos(STEP_ISN, STEP_RYO, 0.0f, 1.6f, -1.8f, 0);
    EVT_AT_SetAngY(STEP_ISN, STEP_RYO, 0.0f, 0);
    EVT_AT_SetMot(STEP_RYO, 7063, 0.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(STEP_ISN, 7103, 22.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetVoice("S2BVC009", 10);
    secondOpponentThrown = 0;
    return 20;
}

int STEP_SecondCounterSuccess(void)
{
    EVT_AT_SetCam(3303, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetCam(3306, STEP_BASE, STEP_BASE, 0, 0, 60);
    EVT_AT_SetFaceLooks(STEP_RYO, 1, 8, 1.0f, 0);
    EVT_AT_SetFaceLooks(STEP_KR4, 6, 8, 1.0f, 20);
    EVT_AT_DispBlur(STEP_RYO, 6, 10, 2, 0, 15, 0);
    EVT_AT_SetMot(STEP_RYO, 7042, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(STEP_ISN, 7097, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetVoice("S2BVA002", 30);
    return 70;
}

int STEP_SecondCounterFailure(void)
{
    EVT_AT_SetCam(3304, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetVoice("S2BVC008", 0);
    EVT_AT_SetMot(STEP_RYO, 7045, 30.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(STEP_ISN, 7099, 30.0f, 1.0f, 0x44000000U, 0);
    return 30;
}

int STEP_BeginThirdOpponent(StepEventWork *work)
{
    STEP_PositionRunningOpponent(STEP_KF4, 0.1f, 0);
    QueueStepDescent(STEP_KRD, 1, 0);
    QueueSkQteWait(&work->qteElapsed, 512, 24, 16, STEP_ThirdCounterSuccess, STEP_ThirdCounterFailure, 15);
    return 0;
}

int STEP_ThirdCounterSuccess(void)
{
    EVT_AT_SetCam(3400, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetPos(STEP_RYO, STEP_RYO, -0.4f, 0.45f, 0.0f, 0);
    EVT_AT_SetPos(STEP_KRD, STEP_RYO, -0.4f, 1.53f, -2.0f, 0);
    EVT_AT_SetAngY(STEP_KRD, STEP_RYO, 0.0f, 0);
    EVT_AT_SetMot(STEP_RYO, 7057, 0.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(STEP_KRD, 7083, 20.0f, 1.0f, 0x44000000U, 0);
    if (secondOpponentThrown) {
        EVT_AT_SetPos(STEP_ISN, STEP_ISN, 0.0f, -1.5f, 0.0f, 45);
        EVT_AT_SetMot(STEP_ISN, 7091, 40.0f, 1.0f, 0x40000000U, 45);
    } else {
        EVT_AT_SetPos(STEP_ISN, EVENT_CHARACTER_INVALID, 264.5f, 243.4f, 136.2f, 0);
        EVT_AT_SetAngY(STEP_ISN, EVENT_CHARACTER_INVALID, 0.0f, 0);
    }
    EVT_AT_SetVoice("S2BVA003", 10);
    EVT_AT_SetVoice("S2BVB023", 50);
    return 70;
}

int STEP_ThirdCounterFailure(void)
{
    EVT_AT_SetCam(3401, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetPos(STEP_RYO, STEP_RYO, 0.0f, 0.45f, 0.0f, 0);
    EVT_AT_SetPos(STEP_RYO, STEP_BASE, 0.27f, 0.9f, -2.6f, 0);
    EVT_AT_SetPos(STEP_KRD, STEP_RYO, -0.4f, 1.53f, -2.0f, 0);
    EVT_AT_SetAngY(STEP_KRD, STEP_RYO, 0.0f, 0);
    EVT_AT_SetMot(STEP_RYO, 7046, 0.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(STEP_KRD, 7080, 26.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetVoice("S2BVB007", 0);
    EVT_AT_SetVoice("S2BVA011", 30);
    return 40;
}

int STEP_BeginFourthOpponent(StepEventWork *work)
{
    STEP_PositionRunningOpponent(STEP_CHD, -0.1f, 0);
    EVT_AT_SetMot(STEP_KF4, 7104, 0.0f, 1.0f, 0x4000000U, 0);
    EVT_AT_SetPos(STEP_RYO, STEP_BASE, 0.26f, 0.9f, -2.9f, 0);
    EVT_AT_SetAngY(STEP_KF4, STEP_KF4, 90.0f, 0);
    EVT_AT_SetCam(3500, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetPos(STEP_KR4, STEP_BASE, -0.4f, 0.0f, -1.0f, 0);
    EVT_AT_SetAngY(STEP_KR4, STEP_BASE, 0.0f, 0);
    EVT_AT_SetPos(STEP_KRD, STEP_BASE, -0.1f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(STEP_KRD, STEP_BASE, 315.0f, 0);
    EVT_AT_SetPos(STEP_ISN, STEP_BASE, 0.4f, 0.0f, -1.3f, 0);
    EVT_AT_SetAngY(STEP_ISN, STEP_BASE, 45.0f, 0);
    EVT_AT_SetMot(STEP_KR4, 7088, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(STEP_KRD, 7066, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(STEP_ISN, 7088, 0.0f, 1.0f, 0x40000000U, 0);
    QueueSkQteWait(&work->qteElapsed, 1024, 24, 16, STEP_FourthDodgeSuccess, STEP_FourthDodgeFailure, 10);
    return 0;
}

int STEP_FourthDodgeSuccess(void)
{
    EVT_AT_DispBlur(STEP_RYO, 5, 10, 2, 0, 10, 0);
    EVT_AT_DispBlur(STEP_RYO, 4, 10, 2, 0, 10, 10);
    EVT_AT_SetMot(STEP_RYO, 7058, 0.0f, 1.0f, 0x4000000U, 0);
    EVT_AT_SetCam(3505, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetVoice("S2BVA003", 0);
    return 20;
}

int STEP_FourthDodgeFailure(void)
{
    EVT_AT_SetCam(3502, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetPos(STEP_KF4, STEP_RYO, -0.3f, 1.56f, -2.3f, 0);
    EVT_AT_SetAngY(STEP_KF4, STEP_RYO, 0.0f, 0);
    EVT_AT_SetMot(STEP_RYO, 7061, 0.0f, 1.0f, 0x4000000U, 0);
    EVT_AT_SetMot(STEP_KF4, 7102, 64.0f, 1.0f, 0x4000000U, 0);
    EVT_AT_SetVoice("S2BVC004", 0);
    EVT_AT_SetVoice("S2BVA010", 30);
    return 20;
}

int STEP_FourthComboSuccess(void)
{
    EVT_AT_DispBlur(STEP_RYO, 1, 10, 3, 0, 30, 0);
    EVT_AT_SetMot(STEP_RYO, 7053, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetVoice("S2BVA001", 0);
    fourthCounterCombo = 1;
    return 0;
}

int STEP_FourthComboFailure(void)
{
    EVT_AT_SetCam(3504, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetMot(STEP_RYO, 7054, 0.0f, 0.9f, 0, 0);
    EVT_AT_SetVoice("S2BVA007", 0);
    EVT_AT_CharaDisp(STEP_ISN, 0, 1);
    return 0;
}

int STEP_FourthCounterSuccess(void)
{
    EVT_AT_SetCam(3503, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetMot(STEP_RYO, 7043, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(STEP_KF4, 7098, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetVoice("S2BVA002", 0);
    fourthCounterCombo = 0;
    return 10;
}

int STEP_FourthCounterFailure(void)
{
    EVT_AT_SetPos(STEP_KF4, STEP_KF4, 0.0f, -0.2f, 0.0f, 0);
    EVT_AT_SetMot(STEP_RYO, 7047, 0.0f, 1.0f, 0, 10);
    EVT_AT_SetMot(STEP_KF4, 7100, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetVoice("S2BVC006", 0);
    return 15;
}

int STEP_BeginFinalOpponent(StepEventWork *work)
{
    float x, y, z;
    shen_name base, camera;
    if (fourthCounterCombo) {
        base = STEP_BASE;
        x = -0.05f;
        y = 1.15f;
        z = -3.1f;
    } else {
        base = STEP_BASE;
        x = 0.25f;
        y = 1.15f;
        z = -3.4f;
    }
    EVT_AT_SetPos(STEP_RYO, base, x, y, z, 0);
    camera = base;
    SHENMUE_EVENT_BOUNDARY();
    EVT_AT_SetCam(3600, camera, camera, 0, 0, 0);
    EVT_AT_SetCam(3601, camera, camera, 0, 0, 50);
    SHENMUE_EVENT_BOUNDARY();
    EVT_AT_SetVoice("S2BVA013", 50);
    EVT_AT_SetAngY(STEP_CHD, STEP_CHD, 270.0f, 0);
    EVT_AT_SetPos(STEP_CHD, STEP_CHD, 0.0f, -0.3f, 0.0f, 20);
    EVT_AT_SetPos(STEP_CHD, STEP_CHD, 0.0f, -0.45f, 0.0f, 36);
    EVT_AT_SetPos(STEP_CHD, STEP_RYO, -1.074f, 1.598f, -2.28f, 50);
    EVT_AT_SetAngY(STEP_CHD, STEP_RYO, 0.0f, 50);
    EVT_AT_SetMot(STEP_RYO, 7062, 0.0f, 1.0f, 0x44000000U, 50);
    EVT_AT_SetMot(STEP_CHD, 7068, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(STEP_CHD, 7067, 0.0f, 1.0f, 0x40000000U, 20);
    EVT_AT_SetMot(STEP_CHD, 7086, 38.0f, 1.0f, 0x44000000U, 50);
    EVT_AT_SetPos(STEP_KF4, STEP_BASE, 0.0f, 2.4f, -2.6f, 50);
    EVT_AT_SetAngY(STEP_KF4, STEP_BASE, 0.0f, 50);
    QueueSkQteWait(&work->qteElapsed, 1024, 24, 16, STEP_FinalDodgeSuccess, STEP_FinalDodgeFailure, 100);
    return 0;
}

int STEP_FinalDodgeSuccess(void)
{
    EVT_AT_SetFaceLooks(STEP_RYO, 1, 8, 1.0f, 0);
    EVT_AT_SetPos(STEP_RYO, STEP_RYO, -0.2f, 0.0f, 0.4f, 0);
    EVT_AT_SetPos(STEP_RYO, STEP_RYO, -0.3f, 0.0f, 0.0f, 80);
    EVT_AT_SetPos(STEP_CHD, STEP_RYO, -0.8f, 1.5f, -2.26f, 0);
    EVT_AT_SetAngY(STEP_CHD, STEP_RYO, 0.0f, 0);
    EVT_AT_SetMot(STEP_RYO, 7059, 170.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(STEP_RYO, 7107, 0.0f, 0.8f, 0, 80);
    EVT_AT_SetMot(STEP_CHD, 7084, 208.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_DispBlur(STEP_RYO, 4, 10, 1, 0, 10, 0);
    EVT_AT_DispBlur(STEP_RYO, 5, 10, 1, 0, 10, 20);
    EVT_AT_DispBlur(STEP_RYO, 6, 10, 1, 0, 15, 50);
    EVT_AT_SetCam(3603, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetCam(3605, STEP_BASE, STEP_BASE, 0, 0, 45);
    EVT_AT_SetCam(3604, STEP_BASE, STEP_BASE, 0, 0, 80);
    EVT_AT_SetVoice("S2BVA001", 0);
    EVT_AT_SetVoice("S2BVC021", 30);
    EVT_AT_SetVoice("S2BVA004", 50);
    return 100;
}

int STEP_FinalDodgeFailure(void)
{
    EVT_AT_SetPos(STEP_RYO, STEP_RYO, -0.3f, 0.0f, 0.0f, 0);
    EVT_AT_SetPos(STEP_CHD, STEP_RYO, -0.974f, 1.5f, -2.26f, 0);
    EVT_AT_SetAngY(STEP_CHD, STEP_RYO, 0.0f, 0);
    EVT_AT_SetMot(STEP_RYO, 7062, 160.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetMot(STEP_CHD, 7086, 198.0f, 1.0f, 0x44000000U, 0);
    EVT_AT_SetCam(3602, STEP_BASE, STEP_BASE, 0, 0, 0);
    EVT_AT_SetVoice("S2BVA010", 0);
    EVT_AT_SetVoice("S2BVA012", 50);
    return 60;
}

void STEP_PositionRunningOpponent(shen_name character, float offset, int delay)
{
    EVT_AT_CharaDisp(character, delay, 0);
    EVT_AT_SetPos(character, STEP_BASE, offset + -1.5f, 2.7f, -5.8f, delay);
    EVT_AT_SetAngY(character, STEP_BASE, 270.0f, delay);
    if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(character)) == STEP_ENEMY_RIG)
        EVT_AT_SetMot(character, 7069, 0.0f, 1.0f, 0x40000000U, delay);
    else
        EVT_AT_SetMot(character, 7105, 0.0f, 1.0f, 0x40000000U, delay);
}

void QueueStepDescent(shen_name character, short steps, int delay)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(eSTEP_EVT_Step, 4, 11, sizeof(StepDescentWork), TASK_EVNT);
    StepDescentWork *work = (StepDescentWork *)FTBL.HLib_GetTaskParameter(task);
    *work = (StepDescentWork){0};
    work->task = task;
    work->character = character;
    work->phase = 0;
    work->steps = steps;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
}

void eSTEP_EVT_Step(void *data)
{
    StepDescentWork *work = (StepDescentWork *)data;
    if (work->phase == 0) {
        FTBL.EV_EnableCharacterDisplay(work->character);
        if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->character)) == STEP_ENEMY_RIG) {
            EVT_AT_SetMot(work->character, 7068, 0.0f, 1.0f, 0, 0);
            EVT_AT_SetMot(work->character, 7067, 0.0f, 1.0f, 0x40000000U, 20);
        } else {
            EVT_AT_SetMot(work->character, 7090, 0.0f, 1.0f, 0, 0);
            EVT_AT_SetMot(work->character, 7089, 0.0f, 1.0f, 0x40000000U, 20);
        }
        EVT_AT_SetPos(work->character, work->character, 0.0f, -0.25f, 0.0f, 20);
        EVT_AT_SetAngY(work->character, work->character, 270.0f, 0);
        FTBL.HLib_SetupTaskDelay(25);
        work->phase++;
    } else if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(work->character), 65, 1, 0)) {
        if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->character)) == STEP_ENEMY_RIG)
            EVT_AT_SetMot(work->character, 7067, 0.0f, 1.0f, 0x40000000U, 0);
        else
            EVT_AT_SetMot(work->character, 7089, 0.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetPos(work->character, work->character, 0.0f, -0.45f, 0.0f, 0);
        if (++work->phase - 1 >= work->steps)
            FTBL.HLib_DestroyCurrentTask();
    }
}

int STEP_RunKaidanAuthor(StepEventWork *work)
{
    int delay = 0;
    switch (work->state) {
    case 0:
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        work->state++;
        delay = 30;
        break;
    case 1:
        ET_AT_KillWork(work->characters);
        FTBL.SetPlayerControlEnabled(0);
        FTBL.EV_ControlPackageResources(0, 6, "STEP", 0, 0);
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"STEPAUTH", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        Queue_EVT_UTL_LoadNPCChara_3(stepAuthorCharacterNames, work->task);
        FTBL.EV_LoadCutsceneFromCurrentScene("0274A000");
        FTBL.EV_LoadAudio("0274A000", 0);
        FTBL.EV_spmBeginSampleExecute(0);
        work->state++;
        break;
    case 2:
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        QueueSkAuthorStart(work->task, STEP_AUTHOR, STEP_AuthorFrameCallback, 0, 0);
        work->state = 0;
        work->phase = 3;
        break;
    }
    return delay;
}

void STEP_AuthorFrameCallback(void *data)
{
    short frame = FTBL.EV_authSceneCtrl(-6);
    switch (frame) {
    case 1630:
        FTBL.SetScenarioCharacterType(STEP_TGY, 7);
        break;
    case 1790:
        FTBL.SetScenarioCharacterType(STEP_TGY, 0);
        break;
    case 3511:
        FTBL.SetMapModelEnabled(0, 4);
        break;
    case 3622:
        FTBL.SetMapModelEnabled(1, 4);
        break;
    case 5145:
        FTBL.SetScenarioCharacterType(STEP_TGY, 7);
        break;
    }
}
#endif
#else
#include "s2/step_event.hpp"
#include "s2/author.hpp"
#include "s2/audio_data.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/sk_event.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2 {
std::int32_t scenarioQteRetryCount;
namespace {
std::int32_t secondOpponentThrown, fourthCounterCombo;
void StepRemovedLogger() {
}
void eSTEP_EVT_KaidanQteMain_Callback(void *data);
void Queue_eSTEP_EVT_KaidanQteMain(void *);
void eSTEP_EVT_Step(void *data);
void eSTEP_EVT_EndKaidanQte_Callback(void *data);
std::int32_t STEP_GetMotionRigId(shen_name character) {
    MotionWork *const motion = scenFindMotionwork(character);
    return motion ? static_cast<std::int32_t>(motion->rig.boneId) : -1;
}

void SetStepPosition(shen_name character, shen_name parent, const Vector3 &position) {
    EventSetPosition request{.cleanup = 0, .character = character, .parent = parent, .position = position, .delay = 0};
    EVT_AT_SetPosRot(&request);
}
void SetStepAngle(shen_name character, shen_name parent, float angle) {
    EventSetAngle request{.cleanup = 0, .character = character, .parent = parent, .degrees = angle, .delay = 0};
    EVT_AT_SetAngY_Callback(&request);
}
void SetStepMotion(shen_name character, std::uint32_t motion, float frame, float speed, std::uint32_t flags) {
    EventSetMotion request{.cleanup = 0, .character = character, .animation = motion, .frame = frame, .speed = speed, .flags = flags, .delay = 0};
    EVT_AT_SetMot_Callback(&request);
    GetMotionFrameCount(static_cast<std::uint16_t>(motion));
}
void SetStepCamera(std::int32_t camera) {
    EventSetCamera request{.cleanup = 0, .camera = camera, .eye = STEP_BASE, .target = STEP_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
void PlayStepVoice(const char *voice) {
    EventSetVoice request{.cleanup = 0, .mode = 1, .voice = {}, .delay = 0};
    for (std::size_t i = 0; (request.voice[i] = voice[i]) != 0; ++i) {
    }
    EVT_AT_SetVoice_Callback(&request);
}
void QueueStepFace(shen_name character, std::int32_t expression, std::int32_t frame, std::int32_t delay) {
    EVT_AT_SetFaceLooks(character, static_cast<std::uint32_t>(expression), frame, 1.0f, delay);
}
} // namespace

// @verified - p
void EVT_AT_ObjectDisp_1(shen_name character, std::int32_t delayFrames, std::int32_t mode) {
    if (delayFrames) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), STEP_EVENT);
        RegisterEventTask(task);
        StepObjectDisplayWork *const work = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
        work->cleanup = 1;
        work->character = character;
        work->mode = mode;
        work->delay = static_cast<float>(delayFrames);
    } else if (mode)
        EV_DisableCharacterDisplay(character);
    else
        EV_EnableCharacterDisplay(character);
}

void EVT_AT_ObjectDisp_Callback(void *data) {
    StepObjectDisplayWork *const work = static_cast<StepObjectDisplayWork *>(data);
    if (work->delay > 0) {
        work->delay -= HLib::GetScaledDeltaTick();
        return;
    }
    if (work->mode)
        EV_DisableCharacterDisplay(work->character);
    else
        EV_EnableCharacterDisplay(work->character);
    if (work->cleanup) {
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
    }
}

namespace {
void QueueStepDisplay(shen_name character, std::int32_t delay) {
    if (!delay) {
        EV_EnableCharacterDisplay(character);
        return;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), STEP_EVENT);
    RegisterEventTask(task);
    *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task)) = {.cleanup = 1, .character = character, .mode = 0, .delay = static_cast<float>(delay)};
}
} // namespace

// @verified - p
void EVT_SK_WaitQTE_Callback(void *data) {
    StepWaitQteWork *const work = static_cast<StepWaitQteWork *>(data);
    if (!work->phase) {
        SetShowQteFlagForButtonType(static_cast<std::int32_t>(work->button) < 0 ? 68 : static_cast<std::uint16_t>(work->button),
            static_cast<std::int16_t>(work->maximumFrames), static_cast<std::int32_t>(work->button) < 0 ? 4 : 2);
        ++work->phase;
    } else if (work->phase == 1) {
        ++*work->elapsed;
        if (GetShouldRenderIcons(RenderIconSlot::Qte))
            return;
        const std::uint16_t status = static_cast<std::uint16_t>(GetCurrentQteStatus());
        *work->status = status & 0xfff;
        if (static_cast<std::int32_t>(work->button) >= 0)
            *work->succeeded = static_cast<std::uint32_t>(status & 0xfff) == work->button;
        else {
            std::uint16_t mask = (work->button & 0xc0000000U) ? 64 : 68;
            if (work->button & 0xa0000000U)
                mask = 4;
            *work->succeeded = (status & mask) != 0;
        }
        const std::int32_t delay = work->referenceFrames - *work->elapsed;
        if (delay > 0)
            HLib::SetupTimedTask(work->task, delay);
        ++work->phase;
    } else if (work->phase == 2) {
        StepQteCallback const callback = *work->succeeded ? work->success : work->failure;
        const std::int32_t delay = callback ? callback() : 0;
        if (delay > 0)
            HLib::SetupTimedTask(work->parent, delay);
        else
            HLib::ResolveDeferredCallback(work->parent);
        HLib::DestroyTaskInternal(work->task);
    }
}

namespace {
void STEP_PositionRunningOpponent(shen_name character, float offset, std::int32_t delay) {
    QueueStepDisplay(character, delay);
    EVT_AT_SetPos(character, STEP_BASE, {.x = offset - 1.5f, .y = 2.7f, .z = -5.8f}, delay);
    EVT_AT_SetAngY(character, STEP_BASE, 270, delay);
    const std::uint32_t motion = STEP_GetMotionRigId(character) == 7 ? 7069 : 7105;
    EVT_AT_SetMot(character, motion, 0, 1, 0x40000000U, delay);
}
void QueueStepDescent(shen_name character, std::int16_t steps, std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(eSTEP_EVT_Step, 4, 0x0b, sizeof(StepDescentWork), STEP_EVENT);
    *static_cast<StepDescentWork *>(HLib::GetTaskParameter(task)) = {.task = task, .character = character, .phase = 0, .steps = steps};
    if (delay)
        HLib::SetupTimedTask(task, delay);
}
void eSTEP_EVT_Step(void *data) {
    StepDescentWork *const work = static_cast<StepDescentWork *>(data);
    if (!work->phase) {
        EV_EnableCharacterDisplay(work->character);
        const bool samRig = STEP_GetMotionRigId(work->character) == 7;
        SetStepMotion(work->character, samRig ? 7068 : 7090, 0, 1, 0);
        EVT_AT_SetMot(work->character, samRig ? 7067 : 7089, 0, 1, 0x40000000U, 20);
        EVT_AT_SetPos(work->character, work->character, {.x = 0, .y = -0.25f, .z = 0}, 20);
        SetStepAngle(work->character, work->character, 270);
        HLib::SetupTaskDelay(25);
        ++work->phase;
    } else if (mtwkSetRegionFlag(scenFindMotionwork(work->character), 65, 1, MotionRegion::LowerBody)) {
        const bool samRig = STEP_GetMotionRigId(work->character) == 7;
        EVT_AT_SetMot(work->character, samRig ? 7067 : 7089, 0, 1, 0x40000000U, 0);
        SetStepPosition(work->character, work->character, {.x = 0, .y = -0.45f, .z = 0});
        if (++work->phase > work->steps)
            HLib::DestroyCurrentTask();
    }
}

std::int32_t STEP_FirstDodgeSuccess() {
    SetStepCamera(3100);
    EVT_AT_DispBlur(STEP_RYO, 1, 10, 2, 15, 0);
    EVT_AT_DispBlur(STEP_RYO, 6, 10, 2, 10, 30);
    SetStepPosition(STEP_RYO, STEP_BASE, {.x = -0.1f, .y = 0, .z = -1.6f});
    SetStepPosition(STEP_KR4, STEP_RYO, {.x = 0, .y = 1.6f, .z = -2.3f});
    SetStepAngle(STEP_KR4, STEP_RYO, 0);
    SetStepMotion(STEP_RYO, 7055, 0, 1, 0x44000000U);
    SetStepMotion(STEP_KR4, 7095, 16, 1, 0x44000000U);
    PlayStepVoice("S2BVB001");
    EVT_AT_SetVoice("S2BVB020", 1, 30);
    return 30;
}
std::int32_t STEP_FirstDodgeFailure() {
    SetStepCamera(3101);
    SetStepPosition(STEP_KR4, STEP_RYO, {.x = -0.2f, .y = 1.8f, .z = -2.5f});
    SetStepAngle(STEP_KR4, STEP_RYO, 0);
    SetStepMotion(STEP_RYO, 7060, 0, 0.8f, 0x44000000U);
    SetStepMotion(STEP_KR4, 7096, 31, 0.8f, 0x44000000U);
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STEP_RYO), 6, 8, 1);
    PlayStepVoice("S2BVA009");
    return 45;
}
std::int32_t STEP_FirstCounterSuccess() {
    SetStepCamera(3102);
    EventSetPositionAngle request{
        .cleanup = 0, .character = STEP_KR4, .parent = STEP_RYO, .position = {.x = 0.2f, .y = 0, .z = 0.3f}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&request);
    EVT_AT_DispBlur(STEP_RYO, 5, 10, 2, 30, 0);
    QueueStepFace(STEP_RYO, 1, 8, 10);
    QueueStepFace(STEP_KR4, 6, 8, 10);
    SetStepMotion(STEP_RYO, 7041, 0, 1, 0x04000000U);
    SetStepMotion(STEP_KR4, 7092, 0, 1, 0x04000000U);
    EVT_AT_SetVoice("S2BVA003", 1, 10);
    return 15;
}
std::int32_t STEP_FirstCounterFailure() {
    camCtrlSetClipDistances(0.005f, 2200);
    SetStepCamera(3103);
    SetStepMotion(STEP_RYO, 7044, 0, 1, 0x04000000U);
    SetStepMotion(STEP_KR4, 7093, 0, 1, 0x04000000U);
    PlayStepVoice("S2BVA012");
    return 15;
}
std::int32_t STEP_BeginFirstOpponent(StepEventWork *work) {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetStepPosition(STEP_RYO, STEP_BASE, {.x = 0.2f, .y = 0, .z = -1});
    SetStepAngle(STEP_RYO, STEP_BASE, 0);
    STEP_PositionRunningOpponent(STEP_KR4, 0, 0);
    STEP_PositionRunningOpponent(STEP_ISN, 0, 120);
    QueueStepDescent(STEP_KR4, 2, 40);
    SetStepMotion(STEP_RYO, 7064, 0, 1, 0x40000000U);
    SetStepCamera(3000);
    EVT_AT_SetCam(3001, STEP_BASE, STEP_BASE, 0, 0, 40);
    QueueSkQteWait(&work->qteElapsed, 16, 24, 16, STEP_FirstDodgeSuccess, STEP_FirstDodgeFailure, 80);
    camCtrlResetClipDistances();
    return 0;
}
std::int32_t STEP_SecondDodgeSuccess() {
    SetStepCamera(3301);
    SetStepPosition(STEP_ISN, STEP_RYO, {.x = 0, .y = 1.6f, .z = -2.1f});
    SetStepAngle(STEP_ISN, STEP_RYO, 0);
    EVT_AT_DispBlur(STEP_RYO, 1, 10, 2, 15, 20);
    SetStepMotion(STEP_RYO, 7056, 0, 1, 0x44000000U);
    SetStepMotion(STEP_ISN, 7101, 6, 1, 0x44000000U);
    EVT_AT_SetVoice("S2BVA001", 1, 30);
    secondOpponentThrown = 1;
    return 5;
}
std::int32_t STEP_SecondDodgeFailure() {
    SetStepCamera(3302);
    QueueStepFace(STEP_RYO, 6, 8, 15);
    SetStepPosition(STEP_ISN, STEP_RYO, {.x = 0, .y = 1.6f, .z = -1.8f});
    SetStepAngle(STEP_ISN, STEP_RYO, 0);
    SetStepMotion(STEP_RYO, 7063, 0, 1, 0x44000000U);
    SetStepMotion(STEP_ISN, 7103, 22, 1, 0x44000000U);
    EVT_AT_SetVoice("S2BVC009", 1, 10);
    secondOpponentThrown = 0;
    return 20;
}
std::int32_t STEP_SecondCounterSuccess() {
    SetStepCamera(3303);
    EVT_AT_SetCam(3306, STEP_BASE, STEP_BASE, 0, 0, 60);
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STEP_RYO), 1, 8, 1);
    QueueStepFace(STEP_KR4, 6, 8, 20);
    EVT_AT_DispBlur(STEP_RYO, 6, 10, 2, 15, 0);
    SetStepMotion(STEP_RYO, 7042, 0, 1, 0x40000000U);
    SetStepMotion(STEP_ISN, 7097, 0, 1, 0x40000000U);
    EVT_AT_SetVoice("S2BVA002", 1, 30);
    return 70;
}
std::int32_t STEP_SecondCounterFailure() {
    SetStepCamera(3304);
    PlayStepVoice("S2BVC008");
    SetStepMotion(STEP_RYO, 7045, 30, 1, 0x44000000U);
    SetStepMotion(STEP_ISN, 7099, 30, 1, 0x44000000U);
    return 30;
}
std::int32_t STEP_BeginSecondOpponent(StepEventWork *work) {
    STEP_PositionRunningOpponent(STEP_KRD, 0.2f, 30);
    QueueStepDescent(STEP_ISN, 2, 0);
    EVT_AT_SetPos(STEP_RYO, STEP_BASE, {.x = 0, .y = 0, .z = -1.6f}, 20);
    EVT_AT_SetPos(STEP_RYO, STEP_RYO, {.x = 0, .y = 0.456f, .z = 0}, 41);
    EVT_AT_SetMot(STEP_RYO, 7051, 0, 1, 0, 20);
    EVT_AT_SetMot(STEP_RYO, 7050, 0, 1, 0x40000000U, 41);
    EVT_AT_SetCam(3300, STEP_BASE, STEP_BASE, 0, 0, 20);
    QueueSkQteWait(&work->qteElapsed, 2048, 24, 16, STEP_SecondDodgeSuccess, STEP_SecondDodgeFailure, 35);
    return 0;
}
std::int32_t STEP_ThirdCounterSuccess() {
    SetStepCamera(3400);
    SetStepPosition(STEP_RYO, STEP_RYO, {.x = -0.4f, .y = 0.45f, .z = 0});
    SetStepPosition(STEP_KRD, STEP_RYO, {.x = -0.4f, .y = 1.53f, .z = -2});
    SetStepAngle(STEP_KRD, STEP_RYO, 0);
    SetStepMotion(STEP_RYO, 7057, 0, 1, 0x44000000U);
    SetStepMotion(STEP_KRD, 7083, 20, 1, 0x44000000U);
    if (secondOpponentThrown) {
        EVT_AT_SetPos(STEP_ISN, STEP_ISN, {.x = 0, .y = -1.5f, .z = 0}, 45);
        EVT_AT_SetMot(STEP_ISN, 7091, 40, 1, 0x40000000U, 45);
    } else {
        SetStepPosition(STEP_ISN, STEP_INVALID, {.x = 264.5f, .y = 243.4f, .z = 136.2f});
        SetStepAngle(STEP_ISN, STEP_INVALID, 0);
    }
    EVT_AT_SetVoice("S2BVA003", 1, 10);
    EVT_AT_SetVoice("S2BVB023", 1, 50);
    return 70;
}
std::int32_t STEP_ThirdCounterFailure() {
    SetStepCamera(3401);
    SetStepPosition(STEP_RYO, STEP_RYO, {.x = 0, .y = 0.45f, .z = 0});
    SetStepPosition(STEP_RYO, STEP_BASE, {.x = 0.27f, .y = 0.9f, .z = -2.6f});
    SetStepPosition(STEP_KRD, STEP_RYO, {.x = -0.4f, .y = 1.53f, .z = -2});
    SetStepAngle(STEP_KRD, STEP_RYO, 0);
    SetStepMotion(STEP_RYO, 7046, 0, 1, 0x44000000U);
    SetStepMotion(STEP_KRD, 7080, 26, 1, 0x44000000U);
    PlayStepVoice("S2BVB007");
    EVT_AT_SetVoice("S2BVA011", 1, 30);
    return 40;
}
std::int32_t Queue_eSTEP_EVT_Step(StepEventWork *work) {
    STEP_PositionRunningOpponent(STEP_KF4, 0.1f, 0);
    QueueStepDescent(STEP_KRD, 1, 0);
    QueueSkQteWait(&work->qteElapsed, 512, 24, 16, STEP_ThirdCounterSuccess, STEP_ThirdCounterFailure, 15);
    return 0;
}
std::int32_t STEP_FourthDodgeSuccess() {
    EVT_AT_DispBlur(STEP_RYO, 5, 10, 2, 10, 0);
    EVT_AT_DispBlur(STEP_RYO, 4, 10, 2, 10, 10);
    SetStepMotion(STEP_RYO, 7058, 0, 1, 0x04000000U);
    SetStepCamera(3505);
    PlayStepVoice("S2BVA003");
    return 20;
}
std::int32_t STEP_FourthDodgeFailure() {
    SetStepCamera(3502);
    SetStepPosition(STEP_KF4, STEP_RYO, {.x = -0.3f, .y = 1.56f, .z = -2.3f});
    SetStepAngle(STEP_KF4, STEP_RYO, 0);
    SetStepMotion(STEP_RYO, 7061, 0, 1, 0x04000000U);
    SetStepMotion(STEP_KF4, 7102, 64, 1, 0x04000000U);
    PlayStepVoice("S2BVC004");
    EVT_AT_SetVoice("S2BVA010", 1, 30);
    return 20;
}
std::int32_t STEP_FourthComboSuccess() {
    EVT_AT_DispBlur(STEP_RYO, 1, 10, 3, 30, 0);
    SetStepMotion(STEP_RYO, 7053, 0, 1, 0);
    PlayStepVoice("S2BVA001");
    fourthCounterCombo = 1;
    return 0;
}
std::int32_t STEP_FourthComboFailure() {
    SetStepCamera(3504);
    SetStepMotion(STEP_RYO, 7054, 0, 0.9f, 0);
    PlayStepVoice("S2BVA007");
    EV_DisableCharacterDisplay(STEP_ISN);
    return 0;
}
std::int32_t STEP_FourthCounterSuccess() {
    SetStepCamera(3503);
    SetStepMotion(STEP_RYO, 7043, 0, 1, 0);
    SetStepMotion(STEP_KF4, 7098, 0, 1, 0);
    PlayStepVoice("S2BVA002");
    fourthCounterCombo = 0;
    return 10;
}
std::int32_t STEP_FourthCounterFailure() {
    SetStepPosition(STEP_KF4, STEP_KF4, {.x = 0, .y = -0.2f, .z = 0});
    EVT_AT_SetMot(STEP_RYO, 7049, 0, 1, 0, 10);
    SetStepMotion(STEP_KF4, 7100, 0, 1, 0);
    PlayStepVoice("S2BVC006");
    return 15;
}
std::int32_t STEP_BeginFourthOpponent(StepEventWork *work) {
    STEP_PositionRunningOpponent(STEP_CHD, -0.1f, 0);
    SetStepMotion(STEP_KF4, 7104, 0, 1, 0x04000000U);
    SetStepPosition(STEP_RYO, STEP_BASE, {.x = 0.26f, .y = 0.9f, .z = -2.9f});
    SetStepAngle(STEP_KF4, STEP_KF4, 90);
    SetStepCamera(3500);
    SetStepPosition(STEP_KR4, STEP_BASE, {.x = -0.4f, .y = 0, .z = -1});
    SetStepAngle(STEP_KR4, STEP_BASE, 0);
    SetStepPosition(STEP_KRD, STEP_BASE, {.x = -0.1f, .y = 0, .z = 0});
    SetStepAngle(STEP_KRD, STEP_BASE, 315);
    SetStepPosition(STEP_ISN, STEP_BASE, {.x = 0.4f, .y = 0, .z = -1.3f});
    SetStepAngle(STEP_ISN, STEP_BASE, 45);
    SetStepMotion(STEP_KR4, 7088, 0, 1, 0x40000000U);
    SetStepMotion(STEP_KRD, 7066, 0, 1, 0x40000000U);
    SetStepMotion(STEP_ISN, 7088, 0, 1, 0x40000000U);
    QueueSkQteWait(&work->qteElapsed, 1024, 24, 16, STEP_FourthDodgeSuccess, STEP_FourthDodgeFailure, 10);
    return 0;
}
void setRyoKaidanPos_Callback(void *) {
    EV_AT_SetPos(STEP_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, {.x = 264.9f, .y = 242.4f, .z = 134.1f});
    HLib::DestroyCurrentTask();
}
std::int32_t STEP_FinalDodgeSuccess() {
    SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STEP_RYO), 1, 8, 1);
    SetStepPosition(STEP_RYO, STEP_RYO, {.x = -0.2f, .y = 0, .z = 0.4f});
    SetStepPosition(STEP_CHD, STEP_RYO, {.x = -0.8f, .y = 1.5f, .z = -2.26f});
    SetStepAngle(STEP_CHD, STEP_RYO, 0);
    SetStepMotion(STEP_RYO, 7059, 170, 1, 0x44000000U);
    EVT_AT_SetMot(STEP_RYO, 7107, 0, 0.8f, 0, 80);
    GetMotionFrameCount(7107);
    SetStepMotion(STEP_CHD, 7084, 208, 1, 0x44000000U);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(setRyoKaidanPos_Callback, 6, 0x0b, 0x28, STEP_EVENT);
    HLib::SetupTimedTask(task, 80);
    EVT_AT_DispBlur(STEP_RYO, 4, 10, 1, 10, 0);
    EVT_AT_DispBlur(STEP_RYO, 5, 10, 1, 10, 20);
    EVT_AT_DispBlur(STEP_RYO, 6, 10, 1, 15, 50);
    SetStepCamera(3603);
    EVT_AT_SetCam(3605, STEP_BASE, STEP_BASE, 0, 0, 45);
    EVT_AT_SetCam(3604, STEP_BASE, STEP_BASE, 0, 0, 80);
    PlayStepVoice("S2BVA001");
    EVT_AT_SetVoice("S2BVC021", 1, 30);
    EVT_AT_SetVoice("S2BVA004", 1, 50);
    return 100;
}
std::int32_t STEP_FinalDodgeFailure() {
    SetStepPosition(STEP_RYO, STEP_RYO, {.x = -0.3f, .y = 0, .z = 0});
    SetStepPosition(STEP_CHD, STEP_RYO, {.x = -0.974f, .y = 1.5f, .z = -2.26f});
    SetStepAngle(STEP_CHD, STEP_RYO, 0);
    SetStepMotion(STEP_RYO, 7062, 160, 1, 0x44000000U);
    SetStepMotion(STEP_CHD, 7086, 198, 1, 0x44000000U);
    SetStepCamera(3602);
    PlayStepVoice("S2BVA010");
    EVT_AT_SetVoice("S2BVA012", 1, 50);
    return 60;
}
std::int32_t STEP_BeginFinalOpponent(StepEventWork *work) {
    SetStepPosition(STEP_RYO, STEP_BASE, fourthCounterCombo ? Vector3{.x = -0.05f, .y = 1.15f, .z = -3.1f} : Vector3{.x = 0.25f, .y = 1.15f, .z = -3.4f});
    SetStepCamera(3600);
    EVT_AT_SetCam(3601, STEP_BASE, STEP_BASE, 0, 0, 50);
    EVT_AT_SetVoice("S2BVA013", 1, 50);
    SetStepAngle(STEP_CHD, STEP_CHD, 270);
    EVT_AT_SetPos(STEP_CHD, STEP_CHD, {.x = 0, .y = -0.3f, .z = 0}, 20);
    EVT_AT_SetPos(STEP_CHD, STEP_CHD, {.x = 0, .y = -0.45f, .z = 0}, 36);
    EVT_AT_SetPos(STEP_CHD, STEP_RYO, {.x = -1.074f, .y = 1.598f, .z = -2.28f}, 50);
    EVT_AT_SetAngY(STEP_CHD, STEP_RYO, 0, 50);
    EVT_AT_SetMot(STEP_RYO, 7062, 0, 1, 0x44000000U, 50);
    SetStepMotion(STEP_CHD, 7068, 0, 1, 0);
    EVT_AT_SetMot(STEP_CHD, 7067, 0, 1, 0x40000000U, 20);
    GetMotionFrameCount(7067);
    EVT_AT_SetMot(STEP_CHD, 7086, 38, 1, 0x44000000U, 50);
    GetMotionFrameCount(7086);
    EVT_AT_SetPos(STEP_KF4, STEP_BASE, {.x = 0, .y = 2.4f, .z = -2.6f}, 50);
    EVT_AT_SetAngY(STEP_KF4, STEP_BASE, 0, 50);
    QueueSkQteWait(&work->qteElapsed, 1024, 24, 16, STEP_FinalDodgeSuccess, STEP_FinalDodgeFailure, 100);
    return 0;
}

std::int32_t STEP_SetupKaidanQte(StepEventWork *work) {
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("STEP"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("STEP"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("STEP"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, STEP_BASE);
    SetStepPosition(STEP_BASE, STEP_INVALID, {.x = 264.7f, .y = 241.3f, .z = 137.52f});
    SetStepAngle(STEP_BASE, STEP_INVALID, 0);
    SetStepPosition(STEP_RYO, STEP_BASE, {});
    SetStepAngle(STEP_RYO, STEP_BASE, 0);
    SetIsSkySetup(0);
    scenarioQteRetryCount = 0;
    StepRemovedLogger();
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, nullptr);
    EV_FreeAuthorPak(1, "S2BV");
    EV_FreeAuthorPak(1, "0274");
    work->state = 1;
    (void)FindMotionWork(ctrlGetScenarioChara(STEP_RYO));
    SetFaceFlagsForChara_0(ctrlGetScenarioChara(STEP_RYO), FacialNodeReplacement);
    for (std::size_t i = 0; i + 1 < stepCharacterNames.size(); ++i) {
        SetScenarioCharacterType(stepCharacterNames[i], CharacterTypeStencilShadow);
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(stepCharacterNames[i]), 1, 0, 1);
    }
    return 0;
}
std::int32_t STEP_RunKaidanQte(StepEventWork *work) {
    std::int32_t delay = 0;
    switch (work->phase) {
    case 0:
        EV_LoadAudio("0273A000", 0);
        EV_spmBeginSampleExecute();
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STEP_RYO), 1, 0, 1);
        for (std::size_t i = 0; i + 1 < stepCharacterNames.size(); ++i) {
            EV_DisableCharacterDisplay(stepCharacterNames[i]);
            SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(stepCharacterNames[i]), 1, 0, 1);
        }
        EV_EnableCharacterDisplay(STEP_RYO);
        delay = STEP_BeginFirstOpponent(work);
        work->phase = 1;
        break;
    case 1:
        if (work->qteSucceeded)
            work->phase = 4;
        else {
            EVT_AT_SetVoice("S2BVA006", 1, 20);
            QueueSkQteWait(&work->qteElapsed, 1024, 24, 16, STEP_FirstCounterSuccess, STEP_FirstCounterFailure, 20);
            work->phase = 3;
        }
        break;
    case 3:
        work->phase = work->qteSucceeded ? 4 : 50;
        break;
    case 4:
        delay = STEP_BeginSecondOpponent(work);
        ++work->phase;
        break;
    case 5:
        if (work->qteSucceeded)
            work->phase = 8;
        else {
            SetStepCamera(3305);
            QueueSkQteWait(&work->qteElapsed, 2048, 24, 16, STEP_SecondCounterSuccess, STEP_SecondCounterFailure, 0);
            work->phase = 7;
        }
        break;
    case 7:
        work->phase = work->qteSucceeded ? 8 : 50;
        break;
    case 8:
        delay = Queue_eSTEP_EVT_Step(work);
        work->phase = 9;
        break;
    case 9:
        work->phase = work->qteSucceeded ? 10 : 50;
        break;
    case 10:
        delay = STEP_BeginFourthOpponent(work);
        work->phase = 11;
        break;
    case 11:
        QueueSkQteWait(&work->qteElapsed, 1, 24, 16, work->qteSucceeded ? STEP_FourthComboSuccess : STEP_FourthCounterSuccess,
            work->qteSucceeded ? STEP_FourthComboFailure : STEP_FourthCounterFailure, 0);
        ++work->phase;
        break;
    case 12:
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STEP_RYO), 6, 8, 1);
        if (!work->qteSucceeded)
            work->phase = 50;
        else {
            delay = STEP_BeginFinalOpponent(work);
            work->phase = 13;
        }
        break;
    case 13:
        work->phase = work->qteSucceeded ? 14 : 50;
        break;
    case 14:
        if (g_currentMap.name == STEP_AREA.name && g_entryPoint > 90) {
            delay = 30;
            work->state = 1;
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        } else {
            work->state = 2;
            spmFadeOut();
        }
        work->phase = 0;
        break;
    case 50: {
        EventFilmControl fade{.cleanup = 0,
            .frames = 30,
            .startAlpha = 0,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 255,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 0};
        EVT_AT_MenCtrl_Callback(&fade);
        if (scenarioQteRetryCount < 2)
            ++scenarioQteRetryCount;
        delay = 60;
        work->phase = 0;
        break;
    }
    default:
        break;
    }
    return delay;
}
void STEP_AuthorFrameCallback(void *) {
    switch (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) {
    case 1630:
        SetScenarioCharacterType(STEP_TGY, CharacterTypeModelShadow);
        break;
    case 1790:
        SetScenarioCharacterType(STEP_TGY, CharacterTypeDefault);
        break;
    case 3511:
        EV_SetEntityVertexShaderType(4, 0);
        break;
    case 3622:
        EV_SetEntityVertexShaderType(4, 1);
        break;
    case 5145:
        SetScenarioCharacterType(STEP_TGY, CharacterTypeModelShadow);
        break;
    default:
        break;
    }
}
} // namespace

void EVT_SK_AuthStart_Callback(void *data) {
    StepAuthorStartWork *const work = static_cast<StepAuthorStartWork *>(data);
    if (!work->phase) {
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int32_t>(work->command)));
        ++work->phase;
        if (work->callback)
            work->callback(work->callbackData);
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        if (work->callback)
            work->callback(work->callbackData);
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            return;
    }
    HLib::ResolveDeferredCallback(work->parent);
    HLib::DestroyTaskInternal(work->task);
}

namespace {
std::int32_t STEP_RunKaidanAuthor(StepEventWork *work) {
    switch (work->phase) {
    case 0:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        ++work->phase;
        return 30;
    case 1:
        ResetAuthorAudioState();
        (void)IsAuthorMultichannelAudioEnabled();
        (void)IsAuthorMultichannelAudioEnabled();
        ET_AT_KillWork(work->characters);
        SetPlayerControlEnabled(0);
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("STEP"), 0);
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("STEPAUTH"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        Queue_EVT_UTL_LoadNPCChara_3(stepAuthorCharacterNames.data(), work->task);
        EV_LoadCutsceneFromCurrentScene("0274A000");
        if (!IsAuthorStreamingAudioLoaded())
            EV_LoadAudio("0274A000", 0);
        EV_spmBeginSampleExecute();
        ++work->phase;
        break;
    case 2: {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        (void)IsAuthorMultichannelAudioEnabled();
        QueueSkAuthorStart(work->task, static_cast<std::int32_t>(STEP_AUTHOR.name), STEP_AuthorFrameCallback, nullptr, 0);
        work->phase = 0;
        work->state = 3;
        break;
    }
    default:
        break;
    }
    return 0;
}
std::int32_t STEP_FinishKaidanAuthor(StepEventWork *work) {
    (void)IsAuthorMultichannelAudioEnabled();
    if (g_currentMap.name == STEP_AREA.name && g_entryPoint > 90) {
        work->phase = 0;
        work->state = 1;
    } else
        HLib::SetCurrentTaskTo(eSTEP_EVT_EndKaidanQte_Callback);
    SetScenarioCharacterType(STEP_TGY, CharacterTypeModelShadow);
    EV_SetEntityVertexShaderType(4, 1);
    spmFadeOut();
    SetScreenFilmFade(EV_authSceneCtrl(AuthSceneCtrl::GetCantSkipAuthor) ? 0 : 30, 0, 0, 0, 0, 255, 0, 0, 0);
    return 30;
}
void eSTEP_EVT_EndKaidanQte_Callback(void *data) {
    StepEventWork *const work = static_cast<StepEventWork *>(data);
    FinishLoading();
    (void)FindMotionWork(ctrlGetScenarioChara(STEP_RYO));
    for (std::size_t i = 0; i + 1 < stepAuthorCharacterNames.size(); ++i)
        if (DoesScenarioCharacterExist(stepAuthorCharacterNames[i])) {
            EV_npcLockUnlock(stepAuthorCharacterNames[i]);
            EV_npcFree(stepAuthorCharacterNames[i]);
        }
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    SetCharaKill(STEP_BASE, 1);
    EV_FreeAuthorPak(0, "S2BV");
    EV_FreeAuthorPak(0, "0274");
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("STEPAUTH"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("STEPAUTH"), 0);
    scenFreeEvent("STEP.EV3");
    scenSetTaskFlags(8);
    Queue_ST_LoadTitleFadeIn(shen_name{});
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("TGLB"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    ExecuteCurrAreaMainFunc_Wrapper(2, 0);
    camCtrlResetClipDistances();
}
void eSTEP_EVT_KaidanQteMain_Callback(void *data) {
    StepEventWork *const work = static_cast<StepEventWork *>(data);
    std::int32_t delay = 0;
    switch (work->state) {
    case 0:
        delay = STEP_SetupKaidanQte(work);
        break;
    case 1:
        delay = STEP_RunKaidanQte(work);
        break;
    case 2:
        delay = STEP_RunKaidanAuthor(work);
        break;
    case 3:
        delay = STEP_FinishKaidanAuthor(work);
        break;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void Queue_eSTEP_EVT_KaidanQteMain(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(eSTEP_EVT_KaidanQteMain_Callback, 4, 0x0b, sizeof(StepEventWork), STEP_EVENT);
    StepEventWork *const work = static_cast<StepEventWork *>(HLib::GetTaskParameter(task));
    *work = {};
    work->task = task;
    work->characters = Queue_EVT_AT_ReadChr(task, stepCharacterNames.data());
}
} // namespace

void EVT_AT_NPCClear_Callback(void *data) {
    StepNpcClearWork *const work = static_cast<StepNpcClearWork *>(data);
    if (work->delay > 0.0f) {
        work->delay -= HLib::GetScaledDeltaTick();
        return;
    }
    if (!work->state) {
        EV_NPC_NewLoadSw(0);
        work->state = static_cast<std::int32_t>(EV_ResetFreeTalkFlags());
    } else {
        work->callback(work->callbackData);
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
    }
}

namespace {
void STEP_init_func() {
    secondOpponentThrown = 0;
    fourthCounterCombo = 0;
}
void Queue_EVT_AT_NPCClear(std::uintptr_t) {
    StepRemovedLogger();
    DateTime date{};
    shenGetCurrentDateTime(date);
    date.hour = 17;
    date.minute = 30;
    SetCurrentDateTimeTo(date);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_UpdateWeather();
    if (g_currentMap.name == STEP_AREA.name)
        SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), STEP_EVENT);
    RegisterEventTask(task);
    StepNpcClearWork *const work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work->callback = Queue_eSTEP_EVT_KaidanQteMain;
    work->callbackData = nullptr;
    work->state = 0;
    work->reserved10 = 0;
    work->delay = 3;
}
} // namespace
const ScriptAreaDefinition stepScriptDefinition{.areaId = STEP_AREA, .field04 = 0, .init = STEP_init_func, .main = Queue_EVT_AT_NPCClear};
} // namespace shenmue::s2
#endif
