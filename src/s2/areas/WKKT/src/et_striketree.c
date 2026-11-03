#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_WKKT.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void WkktMain(int argument)
{
    HLibTask *task;
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    wkktEntry = argument;
    task = FTBL.HLib_EnqueueTaskWithoutParameter(WkktMakeTask, 4, 11, TASK_EVNT);
    FTBL.HLib_SetupTimedTask(task, 3);
}
#else
const shen_name wkktCharacters[] = {WKKT_RYO, WKKT_KEM, WKKT_ABSOLUTE};
int wkktMaximumLevel = 40;
float wkktMotionInputMargin = 6.0f;
float wkktLeafCount = 250.0f;
int wkktLevelDivisor = 4;
int wkktPaused = 0;
int wkktIdle = 0;
int wkktDelayedCommand = 0;
int wkktDescending = 0;
int wkktAnimationFrames = 0;
float wkktStrengthPeak __attribute__((section(".bss")));
int wkktEntry __attribute__((section(".bss")));
float wkktStrength __attribute__((section(".bss")));
int wkktVoiceParity __attribute__((section(".bss")));
int wkktTaught[5] __attribute__((section(".bss")));
int wkktGrade __attribute__((section(".bss")));
int wkktPooledLeaves[WKKT_LEAF_SLOTS] __attribute__((section(".bss")));
int wkktLeafSlots[WKKT_LEAF_SLOTS] __attribute__((section(".bss")));

void WkktRootCallback(void *data)
{
    if (FTBL.shenIsGamePaused())
        wkktPaused = 1;
}

void WkktMakeTask(void *data)
{
    HLibTask *root, *child;
    WkktTrainingRoot *rootWork;
    WkktTrainingWork *work;
    int i;
    root = FTBL.HLib_EnqueueTaskWithParameter(WkktRootCallback, 2, 11, sizeof(WkktTrainingRoot), TASK_EVNT);
    if (wkktEntry == 1)
        child = FTBL.HLib_EnqueueTaskWithParameter(WkktFirst, 5, 11, sizeof(WkktTrainingWork), TASK_EVNT);
    else
        child = FTBL.HLib_EnqueueTaskWithParameter(WkktAuto1, 5, 11, sizeof(WkktTrainingWork), TASK_EVNT);
    rootWork = (WkktTrainingRoot *)FTBL.HLib_GetTaskParameter(root);
    work = (WkktTrainingWork *)FTBL.HLib_GetTaskParameter(child);
    rootWork->self = root;
    rootWork->child = child;
    work->parent = root;
    work->self = child;
    work->phase = 0;
    work->elapsed = 0;
    work->idleTicks = 0;
    work->tries = 0;
    work->errorCount = 0;
    work->level = 0;
    work->rank = 0;
    work->leafA = -1;
    work->leafB = -1;
    work->smoke = -1;
    wkktStrength = -1.0f;
    wkktPaused = 0;
    wkktDelayedCommand = 0;
    wkktVoiceParity = 0;
    for (i = 0; i < 4; i++)
        wkktTaught[i] = 0;
    wkktTaught[0] = 1;
    wkktTaught[4] = 1;
    FTBL.EV_EventSetUpHelpIcon_0(66, 0);
    WkktResetInputHistory(&work->history);
    work->readCharacters = Queue_EVT_AT_ReadChr(child, wkktCharacters);
    WkktPrepare(work);
    EventDebugRegister(WKKT_DEBUG_ID, WkktDebugExitCallback, work, 3000000);
    FTBL.HLib_DestroyCurrentTask();
}

void WkktPrepare(WkktTrainingWork *work)
{
    int slot;
    FTBL.SetScenarioFollowCharacter(0);
    for (slot = 0; slot < WKKT_LEAF_SLOTS; slot++) {
        Vector3 position = {39.34f, 105.5f, 241.1f};
        Vector3 volume = {0.7f, 0.35f, 0.7f};
        wkktPooledLeaves[slot] = FTBL.CreateFallingParticleEffect(&position, &volume, (int)wkktLeafCount, 1.0f, 1024, "misc", "REDLEAF.pvr", 102.0f);
        wkktLeafSlots[slot] = 0;
    }
    if (wkktEntry != 1)
        FTBL.EV_SetNewSceneIndex();
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"WKKT", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "WKKT", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "WKKT", 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0026A028");
    FTBL.EV_FreeAuthorPak(1, "0026");
    FTBL.LoadGauge();
    FTBL.SetFightGaugeVisible(0);
    FTBL.CreateEventCharacter(3, 0, WKKT_BASE);
    EVT_AT_SetPos(WKKT_BASE, WKKT_ABSOLUTE, 39.4f, 102.3f, 241.0f, 0);
    EVT_AT_SetAngY(WKKT_BASE, WKKT_ABSOLUTE, 0.0f, 0);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(WKKT_RYO));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(WKKT_RYO), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(WKKT_RYO), 1, 1, 0);
    Queue_EVT_AT_CtrlHandUp_1(WKKT_RYO, 0, (void *)areaLoopHandPoses[WKKT_STANCE_HAND_POSE], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(WKKT_RYO, 1, (void *)areaLoopHandPoses[WKKT_STANCE_HAND_POSE], 0, 0);
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(WKKT_RYO));
}

void WkktStop(void *data)
{
    WkktTrainingWork *work = (WkktTrainingWork *)data;
    if (g_currentMap != WKKT_AREA) {
        FTBL.spmFadeOut(0);
        if (FTBL.EV_GetScreenFilmIsNotShowing())
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(WkktEnd);
        FTBL.HLib_SetupTaskDelay(30);
    } else {
        if (work->leafA != -1)
            FTBL.FreeParticleEffect(3, work->leafA);
        if (work->leafB != -1)
            FTBL.FreeParticleEffect(3, work->leafB);
        if (work->smoke != -1)
            FTBL.FreeParticleEffect(0, work->smoke);
        work->leafA = -1;
        work->leafB = -1;
        work->smoke = -1;
        FTBL.HLib_SetCurrentTaskTo(WkktAuto1);
    }
}

void WkktDebugExitCallback(void *data)
{
    FTBL.EV_stepSetFlags(32, 0);
    WkktEnd(data);
}

void WkktEnd(void *data)
{
    WkktTrainingWork *work = (WkktTrainingWork *)data;
    EventDateTime date;
    HLibTask *task;
    int i;
    for (i = 0; i < WKKT_LEAF_SLOTS; i++)
        FTBL.FreeParticleEffect(3, wkktPooledLeaves[i]);
    FTBL.EV_SetPlayerFlag(16, 0, work->level);
    FTBL.CloseMoveLearning();
    FTBL.SetScenarioCharacterKillMode(WKKT_BASE, 1);
    if (work->leafA != -1)
        FTBL.FreeParticleEffect(3, work->leafA);
    if (work->leafB != -1)
        FTBL.FreeParticleEffect(3, work->leafB);
    if (work->smoke != -1)
        FTBL.FreeParticleEffect(0, work->smoke);
    FTBL.EV_ControlPackageResources(0, 6, "WKKT", 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0026");
    FTBL.InitiateCleanupForSceneChara(WKKT_KEM);
    ET_AT_KillWork(work->readCharacters);
    FTBL.EV_HLKillOtherHierarchyTask(work->parent);
    FTBL.EV_HLKillOtherHierarchyTask(work->self);
    EVT_AT_SetMot(WKKT_RYO, 0xf003, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetPos(WKKT_RYO, WKKT_ABSOLUTE, 40.0f, 102.25f, 242.3f, 0);
    EVT_AT_SetAngY(WKKT_RYO, WKKT_ABSOLUTE, 247.5f, 0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.lcdmInit(0);
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    FTBL.scenFreeEvent("WKKT.EV2");
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EventDebugUnregister(WKKT_DEBUG_ID);
    FTBL.SetPlayerControlEnabled(0);
    if (FTBL.EV_GetShenmueFlag(926) && !FTBL.EV_GetShenmueFlag(32)) {
        FTBL.GetCurrentDateTime((DateTime *)&date);
        if (date.hour < 19) {
            date.hour = 19;
            date.minute = 0;
            FTBL.npcResetDateTimeIfNeeded();
            FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
            FTBL.npcAdvanceTime(date.hour, date.minute);
        }
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_NPC_NewLoadSw_0(1);
    } else {
        task = FTBL.HLib_EnqueueTaskWithoutParameter(WkktWaitNpc, 4, 11, WKKT_WAIT_NPC_TASK);
        FTBL.HLib_SetupTimedTask(task, 30);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.CheckWarmStartNpcRoute(WKKT_KEM);
        FTBL.ForceLoadNpc(WKKT_KEM);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    }
}

void WkktWaitNpc(void *data)
{
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.npcLockUnlock(WKKT_KEM);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.HLib_DestroyCurrentTask();
}

void WkktFirst(void *data)
{
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_LoadAudio("0026A028", 1);
    FTBL.EV_SetPlayerFlag(16, 0, 0);
    EVT_AT_SetPos(WKKT_RYO, WKKT_BASE, 1.756f, 0.0f, 0.1922f, 0);
    EVT_AT_SetAngY(WKKT_RYO, WKKT_BASE, 90.0f, 0);
    EVT_AT_SetMot(WKKT_RYO, WKKT_IDLE_MOTION, 0.0f, 1.0f, 0x60000000U, -1);
    FTBL.EV_DisableCharacterDisplay(WKKT_KEM);
    EVT_AT_SetCam(2000, WKKT_BASE, WKKT_BASE, 0, 0, 0);
    FTBL.HLib_SetCurrentTaskTo(WkktTraining);
}

void WkktAuto1(void *data)
{
    WkktTrainingWork *work = (WkktTrainingWork *)data;
    int entryPoint, i;
    EventDebugCheck(WKKT_DEBUG_ID, 4);
    entryPoint = g_entryPoint;
    switch (work->phase) {
    case 0:
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_LoadAudio("0026A028", 0);
        work->level = FTBL.EV_GetPlayerFlag(15, 0);
        if (work->level == wkktMaximumLevel)
            work->rank = 3;
        else if (work->level >= 28)
            work->rank = 2;
        else if (work->level >= 14)
            work->rank = 1;
        else
            work->rank = 0;
        for (i = 0; i <= work->level / 10; i++)
            wkktTaught[i] = 1;
        if (entryPoint == 98 && g_currentMap == WKKT_AREA) {
            work->level = wkktMaximumLevel;
            wkktLeafCount = 2500.0f;
        }
        FTBL.EV_DisableCharacterDisplay(WKKT_KEM);
        EVT_AT_SetPos(WKKT_RYO, WKKT_BASE, 1.756f, 0.0f, 0.1922f, 0);
        EVT_AT_SetAngY(WKKT_RYO, WKKT_BASE, 90.0f, 0);
        EVT_AT_SetMot(WKKT_RYO, 5191, 0.0f, 1.0f, 0x40000000U, -1);
        EVT_AT_SetMot(WKKT_RYO, 5190, 0.0f, 1.0f, 0x40000000U, 30);
        EVT_AT_SetVoice("0026A026", 0);
        EVT_AT_SetCam(1000, WKKT_BASE, WKKT_BASE, 0, 0, -1);
        QueueSkChoiceWait(work->self, 0xffff, 151, 153, 0xffff, 153, 32767, 90);
        work->phase++;
        break;
    case 1:
        if (FTBL.shenGetCtrlSpeakIconFlag() == 151) {
            FTBL.EV_LoadAudio("0026A028", 1);
            EVT_AT_SetVoice("0026A027", 0);
            EVT_AT_SetMot(WKKT_RYO, 5192, 0.0f, 1.0f, 0x60000000U, 35);
            EVT_AT_SetMot(WKKT_RYO, WKKT_IDLE_MOTION, 0.0f, 1.0f, 0x60000000U, 60);
            EVT_AT_SetCam(work->rank + 2000, WKKT_BASE, WKKT_BASE, 0, 0, 35);
            FTBL.HLib_SetCurrentTaskTo(WkktTraining);
            FTBL.HLib_SetupTaskDelay(60);
        } else {
            EVT_AT_SetCam(1200, WKKT_BASE, WKKT_BASE, 0, 0, 35);
            EVT_AT_SetVoice("0026A033", 0);
            EVT_AT_SetMot(WKKT_RYO, 5189, 0.0f, 1.0f, 0x40000000U, 0);
            FTBL.EV_stepSetFlags(32, 1);
            FTBL.HLib_SetCurrentTaskTo(WkktStop);
            FTBL.HLib_SetupTaskDelay(70);
        }
        work->phase = 0;
        break;
    }
}

void WkktAuto2(void *data)
{
    WkktTrainingWork *work = (WkktTrainingWork *)data;
    switch (work->phase) {
    case 0:
        wkktIdle = 0;
        WkktHideGauge();
        FTBL.CheckHelpIcon(0, 0, 0);
        EVT_AT_SetPos(WKKT_RYO, WKKT_BASE, 1.756f, 0.0f, 0.1922f, 0);
        EVT_AT_SetPos(WKKT_KEM, WKKT_ABSOLUTE, 42.5f, 102.3f, 242.0f, 0);
        EVT_AT_SetAngY(WKKT_RYO, WKKT_ABSOLUTE, 90.0f, -1);
        EVT_AT_SetMot(WKKT_RYO, 5191, 0.0f, 1.0f, 0x40000000U, -1);
        EVT_AT_SetMot(WKKT_RYO, 5190, 0.0f, 1.0f, 0x40000000U, 30);
        EVT_AT_SetCam(1100, WKKT_BASE, WKKT_BASE, 0, 0, -1);
        EVT_AT_SetVoice("0026A028", 0);
        QueueSkChoiceWait(work->self, 0xffff, 152, 154, 0xffff, 154, 32767, 60);
        work->phase++;
        work->elapsed = 0;
        work->idleTicks = 0;
        work->tries = 0;
        break;
    case 1:
        if (FTBL.shenGetCtrlSpeakIconFlag() == 152) {
            EVT_AT_SetVoice("0026A027", 0);
            EVT_AT_SetMot(WKKT_RYO, 5192, 0.0f, 1.0f, 0x60000000U, 35);
            EVT_AT_SetMot(WKKT_RYO, WKKT_IDLE_MOTION, 0.0f, 1.0f, 0x60000000U, 60);
            EVT_AT_SetCam(work->rank + 2000, WKKT_BASE, WKKT_BASE, 0, 0, 35);
            FTBL.HLib_SetCurrentTaskTo(WkktTraining);
            FTBL.HLib_SetupTaskDelay(60);
        } else {
            EVT_AT_SetCam(1200, WKKT_BASE, WKKT_BASE, 0, 0, 35);
            FTBL.EV_stepSetFlags(32, 1);
            EVT_AT_SetMot(WKKT_RYO, 5189, 0.0f, 1.0f, 0x40000000U, 0);
            FTBL.HLib_SetCurrentTaskTo(WkktStop);
            FTBL.HLib_SetupTaskDelay(70);
        }
        work->phase = 0;
        break;
    }
}

void WkktAuto3(void *data)
{
    WkktTrainingWork *work = (WkktTrainingWork *)data;
    FTBL.CheckHelpIcon(0, 0, 0);
    WkktHideGauge();
    EVT_AT_SetPos(WKKT_RYO, WKKT_BASE, 1.756f, 0.0f, 0.1922f, 0);
    EVT_AT_SetAngY(WKKT_RYO, WKKT_BASE, 90.0f, 0);
    EVT_AT_SetMot(WKKT_RYO, 5191, 0.0f, 1.0f, 0x40000000U, -1);
    EVT_AT_SetMot(WKKT_RYO, 5190, 0.0f, 1.0f, 0x40000000U, 30);
    EVT_AT_SetMot(WKKT_RYO, 5189, 0.0f, 1.0f, 0x40000000U, 60);
    EVT_AT_SetCam(1100, WKKT_BASE, WKKT_BASE, 0, 0, -1);
    EVT_AT_SetCam(1200, WKKT_BASE, WKKT_BASE, 0, 0, 60);
    EVT_AT_SetVoice("0026A045", 0);
    work->elapsed = 0;
    work->idleTicks = 0;
    work->tries = 0;
    FTBL.EV_stepSetFlags(32, 1);
    FTBL.HLib_SetCurrentTaskTo(WkktStop);
    FTBL.HLib_SetupTaskDelay(130);
}

void WkktTalk(void *data)
{
    WkktTrainingWork *work = (WkktTrainingWork *)data;
    int duration = 0;
    WkktHideGauge();
    EVT_AT_SetMot(WKKT_RYO, 5200, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(WKKT_RYO, 5199, 0.0f, 1.0f, 0, 60);
    EVT_AT_SetMot(WKKT_KEM, 5193, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetPos(WKKT_KEM, WKKT_BASE, 3.4f, 0.0f, -1.4f, 0);
    EVT_AT_SetAngY(WKKT_KEM, WKKT_BASE, 135.0f, 0);
    EVT_AT_CharaDisp(WKKT_KEM, 0, 0);
    switch (work->level / 10) {
    case 1:
        EVT_AT_SetCam(3400, WKKT_BASE, WKKT_BASE, 0, 0, 0);
        EVT_AT_SetCam(3401, WKKT_BASE, WKKT_BASE, 0, 0, 170);
        EVT_AT_SetVoice("0026B056", 0);
        EVT_AT_SetVoice("0026A035", 170);
        duration = 240;
        break;
    case 2:
        EVT_AT_SetCam(3410, WKKT_BASE, WKKT_BASE, 0, 0, 0);
        EVT_AT_SetCam(3411, WKKT_BASE, WKKT_BASE, 0, 0, 150);
        EVT_AT_SetVoice("0026B057", 0);
        EVT_AT_SetVoice("0026A038", 150);
        duration = 210;
        break;
    case 3:
        EVT_AT_SetCam(3420, WKKT_BASE, WKKT_BASE, 0, 0, 0);
        EVT_AT_SetCam(3421, WKKT_BASE, WKKT_BASE, 0, 0, 110);
        EVT_AT_SetVoice("0026B058", 0);
        EVT_AT_SetVoice("0026A040", 110);
        duration = 200;
        break;
    }
    EVT_AT_SetCam(work->rank + 2000, WKKT_BASE, WKKT_BASE, 0, 0, duration - 25);
    EVT_AT_CharaDisp(WKKT_KEM, duration - 25, 1);
    EVT_AT_SetPos(WKKT_RYO, WKKT_BASE, 1.896f, 0.0f, 0.168f, duration - 25);
    EVT_AT_SetMot(WKKT_RYO, 5192, 0.0f, 1.0f, 0x60000000U, duration - 25);
    EVT_AT_SetMot(WKKT_RYO, WKKT_IDLE_MOTION, 0.0f, 1.0f, 0x60000000U, duration);
    work->phase = 1;
    FTBL.HLib_SetCurrentTaskTo(WkktTraining);
    if (duration > 0)
        FTBL.HLib_SetupTaskDelay(duration);
}

void WkktTraining(void *data)
{
    WkktTrainingWork *work = (WkktTrainingWork *)data;
    WkktDrawLcdLevel(work);
    FTBL.GetBattleDataConfig(0, SHEN_NAME4('B', 'T', 'P', 'P'));
    FTBL.GetBattleDataConfig(0, SHEN_NAME4('B', 'T', 'K', 'K'));
    FTBL.GetBattleDataConfig(0, SHEN_NAME4('B', 'T', 'C', 'C'));
    if (FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(WKKT_RYO), 0) == WKKT_IDLE_MOTION)
        wkktIdle = 1;
    else
        wkktIdle = 0;
    switch (work->phase) {
    case 0:
        wkktPaused = 0;
        FTBL.CheckHelpIcon(1, 0, 0);
        EVT_AT_SetPos(WKKT_RYO, WKKT_BASE, 1.7f, 0.0f, 0.2f, 0);
        EVT_AT_SetAngY(WKKT_RYO, WKKT_BASE, 90.0f, 0);
        work->phase++;
        break;
    case 1:
        if (FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(WKKT_RYO), 0) != WKKT_IDLE_MOTION)
            WkktBufferDuringMotion(work);
        else
            WkktBufferIdle(work);
        break;
    case 2:
        if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WKKT_RYO), 65, 1, 0)
            || FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(WKKT_RYO), 0) == WKKT_IDLE_MOTION) {
            if (work->action)
                WkktPlayStrike(work->action);
            work->phase = 0;
            FTBL.HLib_SetCurrentTaskTo(WkktAction);
        }
        break;
    }
    switch (FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(WKKT_RYO), 0)) {
    case 371:
    case 5185:
    case 5188:
        WkktUpdateStrength(0);
        break;
    default:
        WkktUpdateStrength(1);
        break;
    }
    if (FTBL.GetCurrentControlInput(7) == 512 && FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(WKKT_RYO), 0) == WKKT_IDLE_MOTION) {
        work->phase = 0;
        FTBL.HLib_SetCurrentTaskTo(WkktAuto2);
    }
    if (FTBL.shenGetMinutesUntilNextFrameMinute() == 0) {
        work->phase = 0;
        FTBL.HLib_SetCurrentTaskTo(WkktAuto3);
    }
}

void WkktUpdateStrength(int cycling)
{
    float peaks[4] = {75.6f, 84.0f, 92.4f, 100.0f};
    if (cycling) {
        if (wkktStrength < 0.0f) {
            FTBL.SetFightGaugeVisible(1);
            wkktDescending = 0;
            wkktStrengthPeak = peaks[FTBL.shenGetRandomInt32(4)];
            if (g_entryPoint == 97 && g_currentMap == WKKT_AREA)
                wkktStrengthPeak = peaks[3];
            wkktStrength = 0.0f;
            wkktAnimationFrames = 0;
        } else if (!wkktDelayedCommand) {
            if (!wkktDescending) {
                wkktStrength += 5.0f;
                if (wkktStrength >= wkktStrengthPeak) {
                    wkktStrength = wkktStrengthPeak;
                    wkktDescending = 1;
                }
            } else {
                wkktStrength -= 5.0f;
                if (wkktStrength <= 0.0f) {
                    wkktStrength = 0.0f;
                    wkktDescending = 0;
                    wkktStrengthPeak = peaks[FTBL.shenGetRandomInt32(4)];
                    if (g_entryPoint == 97 && g_currentMap == WKKT_AREA)
                        wkktStrengthPeak = peaks[3];
                }
            }
        } else
            wkktDelayedCommand = 0;
    } else {
        switch (FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(WKKT_RYO), 0)) {
        case 5188:
            if (wkktAnimationFrames++ > 45)
                wkktStrength -= 2.0f;
            break;
        case 371:
        case 5185:
            wkktStrength -= 10.0f;
            break;
        }
    }
    FTBL.UpdateSingleFightGauge(wkktStrength);
}

void WkktHideGauge(void)
{
    wkktStrength = -1.0f;
    FTBL.SetFightGaugeVisible(0);
}

void WkktDrawLcdLevel(WkktTrainingWork *work)
{

    unsigned char image[194] = {0x06, 0x20,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xf4, 0x5e, 0x80, 0x00, 0x00,
        0x10, 0x84, 0x50, 0x80, 0x00, 0x00, 0x10, 0x84, 0x50, 0x80, 0x00, 0x00, 0x10, 0xe4, 0x5e, 0x80, 0x00, 0x00,
        0x10, 0x84, 0x50, 0x80, 0x00, 0x00, 0x10, 0x82, 0x90, 0x80, 0x00, 0x00, 0x1e, 0xf1, 0x1e, 0xf0, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x10, 0x00, 0x00, 0x00, 0x00, 0x08,
        0x10, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x08,
        0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8};
    int i, j, k;
    i = 0;
    for (;;) {
        if (work->level <= 4) {
            for (j = work->level; j >= 0; j--)
                image[i * 6 + 86] |= 1 << (4 - j);
        } else {
            image[i * 6 + 86] = 31;
            for (k = 0; k < (work->level - 4) / 8; k++)
                image[k + 87 + i * 6] = 0xff;
            for (j = 0; j <= (work->level - 4) % 8; j++)
                image[k + 87 + i * 6] |= 1 << (8 - j);
        }
        if (++i > 3)
            break;
    }
    FTBL.lcdmInit(image);
}

void WkktBufferIdle(WkktTrainingWork *work)
{
    int required = WKKT_REQUIRED_INPUT;
    work->action = WkktMatchInput(&work->history, &required, 0);
    if (work->action >= 0 && work->action <= 2) {
        if (work->action == 0)
            work->errorCount = 0;
        else {
            WkktPlayStrike(work->action);
            if (++work->errorCount > 10) {
                EVT_AT_SetVoice("0026A043", 0);
                work->errorCount = 0;
            }
        }
        work->phase = 0;
        FTBL.HLib_SetCurrentTaskTo(WkktAction);
    }
    work->elapsed++;
    work->idleTicks++;
    if (work->elapsed > 5400) {
        work->phase = 0;
        WkktHideGauge();
        FTBL.HLib_SetCurrentTaskTo(WkktAuto2);
    }
    if (FTBL.GetCurrentControlInput(0))
        work->idleTicks = 0;
}

void WkktBufferDuringMotion(WkktTrainingWork *work)
{
    int required = WKKT_REQUIRED_INPUT;
    float end, frame;
    if (FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(WKKT_RYO), 0) != WKKT_IDLE_MOTION) {
        end = (float)FTBL.GetMotionFrameCount(FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(WKKT_RYO), 0));
        frame = FTBL.GetMotionFrame(FTBL.scenFindMotionwork(WKKT_RYO), 0);
        if (frame >= end - wkktMotionInputMargin) {
            work->action = WkktMatchInput(&work->history, &required, 0);
            if (work->action >= 0 && work->action <= 2) {
                if (work->action != 0) {
                    if (++work->errorCount > 10) {
                        EVT_AT_SetVoice("0026A043", 0);
                        work->errorCount = 0;
                    }
                }
                FTBL.DebugLogToFileRemoved("ADVANCE!!! : %f\n", end - frame);
                work->phase++;
            }
        }
        if (end - frame < 0.02f && frame - end < 0.02f)
            EVT_AT_SetMot(WKKT_RYO, WKKT_IDLE_MOTION, 0.0f, 1.0f, 0x60000000U, 0);
    }
    if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WKKT_RYO), 65, 1, 0) && work->action != 0 && work->action != 2 && work->action != 1)
        EVT_AT_SetMot(WKKT_RYO, WKKT_IDLE_MOTION, 0.0f, 1.0f, 0x60000000U, -1);
}

void WkktAction(void *data)
{
    WkktTrainingWork *work = (WkktTrainingWork *)data;
    int firstSounds[4] = {85, 46, 43, 44};
    int secondSounds[4] = {206, 449, 450, 947};
    int gains[4] = {0, 1, 2, 5};
    float shake[4] = {0.0f, 0.5f, 0.75f, 0.9f};
    work->phase = 1;
    switch (work->action) {
    case 0:
        if (wkktStrength >= 100.0f)
            wkktGrade = 3;
        else if (wkktStrength >= 75.0f)
            wkktGrade = 2;
        else if (wkktStrength >= 41.66f)
            wkktGrade = 1;
        else
            wkktGrade = 0;
        if (work->level >= wkktMaximumLevel && wkktGrade > 1 && !(g_entryPoint == 97 && g_currentMap == WKKT_AREA)) {
            WkktHideGauge();
            work->phase = 0;
            FTBL.HLib_SetCurrentTaskTo(WkktSuccess);
            break;
        }
        if (work->level == 0 && wkktGrade)
            FTBL.SetCombatEffectOrder(1);
        work->level += gains[wkktGrade];
        work->phase = 0;
        if (work->level > wkktMaximumLevel)
            work->level = wkktMaximumLevel;
        work->tries++;
        if (work->level == wkktMaximumLevel)
            work->rank = 3;
        else if (work->level >= 28)
            work->rank = 2;
        else if (work->level >= 14)
            work->rank = 1;
        else
            work->rank = 0;
        if (wkktGrade) {
            EVT_AT_SetCamRot(0.15f, 0.3f, shake[wkktGrade], 27);
            WkktQueueFallLeaves(work->level, 30);
        }
        EVT_AT_SetSound(firstSounds[wkktGrade], 20);
        EVT_AT_SetSound(948, 10);
        EVT_AT_SetSound(secondSounds[wkktGrade], 30);
        EVT_AT_SetMot(WKKT_RYO, 5188, 40.0f, 1.0f, 0x20000000U, 0);
        Queue_EVT_AT_CtrlHandUp_1(WKKT_RYO, 0, (void *)areaLoopHandPoses[WKKT_STRIKE_HAND_POSE], 8, 0);
        Queue_EVT_AT_CtrlHandUp_1(WKKT_RYO, 0, (void *)areaLoopHandPoses[WKKT_STANCE_HAND_POSE], 8, 60);
        WkktFeedback(work);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        FTBL.HLib_SetCurrentTaskTo(WkktTraining);
        break;
    case 5:
        FTBL.HLib_SetCurrentTaskTo(WkktTraining);
        break;
    }
}

void WkktFeedback(WkktTrainingWork *work)
{
    int offset = 0;
    work->phase = 0;
    WkktDrawLcdLevel(work);
    if (work->rank > 1)
        offset = 50;
    switch (wkktGrade) {
    case 0:
        EVT_AT_SetCam(offset + 3000, WKKT_BASE, WKKT_BASE, 0, 0, 10);
        break;
    case 1:
        EVT_AT_SetCam(3100 + FTBL.shenGetRandomInt32(4) + offset, WKKT_BASE, WKKT_BASE, 0, 0, 10);
        break;
    case 2:
        EVT_AT_SetCam(3200 + FTBL.shenGetRandomInt32(4) + offset, WKKT_BASE, WKKT_BASE, 0, 0, 10);
        break;
    case 3:
        EVT_AT_SetCam(offset + 3300, WKKT_BASE, WKKT_BASE, 0, 0, 10);
        EVT_AT_SetCam(offset + 3301, WKKT_BASE, WKKT_BASE, 0, 0, 25);
        break;
    }
    WkktFeedbackVoice(work);
    if (wkktTaught[work->level / 10] == 1)
        EVT_AT_SetCam(work->rank + 2000, WKKT_BASE, WKKT_BASE, 0, 0, 120);
    if (work->level >= wkktMaximumLevel && wkktGrade > 2)
        FTBL.HLib_SetCurrentTaskTo(WkktActionWait);
    if (work->level >= wkktMaximumLevel)
        FTBL.HLib_SetCurrentTaskTo(WkktTraining);
    else if (wkktTaught[work->level / 10] == 0)
        FTBL.HLib_SetCurrentTaskTo(WkktActionWait);
    else
        FTBL.HLib_SetCurrentTaskTo(WkktTraining);
}

void WkktFeedbackVoice(WkktTrainingWork *work)
{
    const char *voices[4][4][2] = {{{"0026A049", "0026A037"}, {"0026A036", "0026A050"}, {"0026A051", "0026A052"}, {"0026A053", "0026A053"}},
        {{"0026A054", "0026A054"}, {"0026A055", "0026A056"}, {"0026A041", "0026A057"}, {"0026A058", "0026A058"}},
        {{"0026A059", "0026A059"}, {"0026A055", "0026A061"}, {"0026A042", "0026A052"}, {"0026A063", "0026A063"}},
        {{"0026A064", "0026A064"}, {"0026A065", "0026A066"}, {"0026A036", "0026A041"}, {"0026A041", "0026A042"}}};
    int unused[4] = {6, 0, 0, 13};
    wkktVoiceParity = (wkktVoiceParity + 1) % 2;
    if ((wkktVoiceParity == 0 || wkktGrade == 0 || wkktGrade == 3) && wkktTaught[work->level / 10] == 1)
        EVT_AT_SetVoice(voices[work->rank][wkktGrade][FTBL.shenGetRandomInt32(2)], 60);
}

void WkktActionWait(void *data)
{
    WkktTrainingWork *work = (WkktTrainingWork *)data;
    if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WKKT_RYO), 65, 1, 0)) {
        if (!wkktTaught[work->level / 10] && !work->action) {
            wkktTaught[work->level / 10] = 1;
            FTBL.HLib_SetCurrentTaskTo(WkktTalk);
        } else
            FTBL.HLib_SetCurrentTaskTo(WkktTraining);
    }
}

void WkktQueueFallLeaves(int level, int delay)
{
    HLibTask *task;
    WkktFallingWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(WkktFallLeaves, 6, 11, sizeof(WkktFallingWork), TASK_EVNT);
    work = (WkktFallingWork *)FTBL.HLib_GetTaskParameter(task);
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    work->level = level;
    work->self = task;
}

void WkktFallLeaves(void *data)
{
    WkktFallingWork *work = (WkktFallingWork *)data;
    Vector3 position = {39.34f, 105.5f, 241.1f};
    Vector3 volume = {0.7f, 0.35f, 0.7f};
    Vector3 velocity = {0.0f, -0.03f, 0.0f};
    Vector3 variance = {0.012f, -0.005f, 0.012f};
    int sounds[11] = {451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 460};
    float rates[11] = {0.008f, 0.02f, 0.03f, 0.045f, 0.08f, 0.15f, 0.2f, 0.35f, 0.5f, 0.7f, 1.0f};
    float gradeRates[4] = {0.0f, 0.2f, 0.5f, 1.0f};
    float rate;
    int slot;
    rate = gradeRates[wkktGrade] * rates[work->level / wkktLevelDivisor];
    EVT_AT_SetSound(sounds[work->level / wkktLevelDivisor], 0);
    for (slot = 0; slot < WKKT_LEAF_SLOTS && wkktLeafSlots[slot]; slot++)
        ;
    if (slot == WKKT_LEAF_SLOTS)
        FTBL.EventAssertNullsub(__FILE__, 1171);
    work->slot = slot;
    wkktLeafSlots[work->slot] = 1;
    FTBL.SetParticleEffectPosition(3, wkktPooledLeaves[work->slot], &position, &volume);
    FTBL.SetParticleEffectVelocity(3, wkktPooledLeaves[work->slot], &velocity, &variance);
    FTBL.SetParticleEffectSize(3, wkktPooledLeaves[work->slot], 0.03f, 0.09f);
    FTBL.SetParticleEffectBirthRate(3, wkktPooledLeaves[work->slot], rate);
    FTBL.ResumeParticleEffect(3, wkktPooledLeaves[work->slot]);
    work->frame = 0;
    FTBL.DebugLogToFileRemoved("Skill   = %d\n", work->level);
    FTBL.DebugLogToFileRemoved("Num     = %d\n", work->slot);
    FTBL.DebugLogToFileRemoved("Req     = %f\n", gradeRates[wkktGrade]);
    FTBL.DebugLogToFileRemoved("L_Rate  = %f\n", rates[work->level / wkktLevelDivisor]);
    FTBL.DebugLogToFileRemoved("Rate    = %f\n", rate);
    FTBL.HLib_SetCurrentTaskTo(WkktKillLeaves);
    FTBL.HLib_SetupTaskDelay(30);
}

void WkktKillLeaves(void *data)
{
    WkktFallingWork *work = (WkktFallingWork *)data;
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 volume = {0.0f, 0.0f, 0.0f};
    wkktLeafSlots[work->slot] = 0;
    FTBL.SetParticleEffectPosition(3, wkktPooledLeaves[work->slot], &position, &volume);
    FTBL.HLib_DestroyTaskInternal(work->self);
}

void WkktResetInputHistory(WkktInputHistory *history)
{
    history->cursor = 0;
    history->previous = 0;
    history->pressed[0] = 0;
    history->held[0] = 0;
    history->step = 0;
    history->waiting = 0;
}

int WkktMatchInput(WkktInputHistory *history, const int *required, int count)
{
    int previous;
    history->pressed[history->cursor] = FTBL.GetCurrentControlInput(7);
    history->held[history->cursor] = FTBL.GetCurrentControlInput(0);
    if (history->cursor == 0 && (history->held[history->cursor] & history->held[history->previous]))
        return 6;
    if (!history->held[history->cursor]) {
        if (!history->waiting) {
            history->waiting = 1;
            history->cursor++;
        }
        return 6;
    }
    if ((history->pressed[history->cursor] & required[history->step]) == required[history->step]) {
        WkktResetInputHistory(history);
        return 0;
    }
    history->waiting = 0;
    if (history->held[history->cursor] & required[history->step]) {
        previous = history->cursor - 1;
        if (previous < 0)
            previous = history->previous;
        wkktDelayedCommand = 1;
        if ((history->held[history->cursor] & history->held[previous]) == required[history->step]) {
            FTBL.DebugLogToFileRemoved("Delayed Command !!\n");
            WkktResetInputHistory(history);
            return 0;
        }
        if ((history->held[history->cursor] & required[history->step]) == (history->held[previous] & required[history->step]))
            history->previous = 0;
        else {
            if (++history->cursor > 30)
                history->cursor = 0;
            return 6;
        }
    }
    return WkktRejectInput(history);
}

void WkktPlayStrike(int action)
{
    switch (action) {
    case 1:
        EVT_AT_SetMot(WKKT_RYO, 5185, 0.0f, 1.0f, 0x60000000U, 0);
        EVT_AT_SetSound(136, 0);
        break;
    case 2:
        EVT_AT_SetMot(WKKT_RYO, 371, 0.0f, 1.0f, 0x60000000U, 0);
        EVT_AT_SetSound(136, 0);
        break;
    case 3:
    case 4:
        break;
    }
}

int WkktRejectInput(WkktInputHistory *history)
{
    int cursor = history->cursor;
    history->cursor = 0;
    if (history->held[cursor] & 4)
        return 1;
    if (history->held[cursor] & 0x400)
        return 2;
    if (history->held[cursor] & 2)
        return 3;
    return (history->held[cursor] & 0x200) ? 4 : 5;
}

void WkktSuccess(void *data)
{
    WkktTrainingWork *work = (WkktTrainingWork *)data;
    switch (work->phase) {
    case 0:
        FTBL.EV_EnableCharacterDisplay(WKKT_KEM);
        FTBL.CheckHelpIcon(0, 0, 0);
        FTBL.EV_authSceneCtrl(WKKT_AUTHOR_SUCCESS);
        WkktCreateSuccessSmoke(work);
        WkktCreateLeafBurst(&work->leafA, 1000, 0.02f);
        WkktCreateLeafBurst(&work->leafB, 2500, 0.005f);
        work->phase++;
        break;
    case 1:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            work->phase = 0;
            FTBL.QueueExtraTelop("功", 1);
            FTBL.InitiateCleanupForSceneChara(WKKT_KEM);
            FTBL.EV_stepSetFlags(32, 0);
            FTBL.HLib_SetCurrentTaskTo(WkktStop);
        } else
            WkktAnimateSuccessEffects(work);
        break;
    }
}

void WkktAnimateSuccessEffects(WkktTrainingWork *work)
{
    switch (FTBL.EV_authSceneCtrl(-6)) {
    case 40:
        FTBL.SuspendParticleEffect(3, work->leafA);
        break;
    case 90:
        FTBL.ResumeParticleEffect(0, work->smoke);
        break;
    case 119:
        FTBL.SetParticleEffectBirthRate(0, work->smoke, 0.0f);
        break;
    case 120:
        FTBL.SuspendParticleEffect(0, work->smoke);
        break;
    case 145:
        FTBL.ResumeParticleEffect(3, work->leafA);
        break;
    case 440:
        FTBL.SuspendParticleEffect(3, work->leafA);
        break;
    case 1330:
        FTBL.SetParticleEffectSize(3, work->leafB, 0.04f, 0.12f);
        FTBL.ResumeParticleEffect(3, work->leafB);
        break;
    }
}

void WkktCreateLeafBurst(int *effect, int count, float rate)
{
    Vector3 position = {39.34f, 105.5f, 241.1f};
    Vector3 volume = {1.0f, 0.3f, 1.5f};
    Vector3 velocity = {0.0001f, -0.04f, 0.0001f};
    Vector3 variance = {0.012f, -0.01f, 0.012f};
    *effect = FTBL.CreateFallingParticleEffect(&position, &volume, count, rate, 1024, "misc", "REDLEAF.pvr", -1.0f);
    FTBL.SetParticleEffectVelocity(3, *effect, &velocity, &variance);
    FTBL.SetParticleEffectSize(3, *effect, 0.03f, 0.09f);
}

void WkktCreateSuccessSmoke(WkktTrainingWork *work)
{
    Vector3 position = {40.69f, 102.2f, 241.27f};
    Vector3 volume = {0.0f, 0.0f, 0.0f};
    Vector3 velocity, variance;
    velocity.x = 0.0f;
    velocity.y = 0.01f;
    velocity.z = 0.0f;
    variance.x = 0.0f;
    variance.y = 0.0f;
    variance.z = 0.0f;
    work->smoke = FTBL.CreateSmokeEffect(&position, &volume, 100, 0.001f, 0, 3);
    FTBL.SetSmokeEffectFadeFactor(work->smoke, 10);
    FTBL.SetSmokeEffectFalloff(work->smoke, 255);
    FTBL.SetParticleEffectColour(0, work->smoke, 0, 142, 125, 110);
    FTBL.SetParticleEffectInitialScale(0, work->smoke, 0.435294122f);
    FTBL.SetParticleEffectSize(0, work->smoke, 3.13725495f, 3.17647052f);
    FTBL.SetParticleEffectBirthRate(0, work->smoke, 0.0f);
    FTBL.SetParticleEffectVelocity(0, work->smoke, &velocity, &variance);
    FTBL.SetSmokeEffectSpread(work->smoke, 55.8639984f, 1.05882359f);
    FTBL.SetSmokeEffectRadialMotion(work->smoke, 0.772549033f, 0.466666669f);
    FTBL.SetParticleEffectBirthRate(0, work->smoke, 0.1f);
}

#include "s2/event_debug_dc.c"
#endif
#endif
