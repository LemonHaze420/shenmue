#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_istb.hpp"
#include "s2/event_queued_dc.h"
#include "s2/sk_event_dc.h"

static const shen_name names[] = {ISTB_SIN, 0xffffffffU};
static unsigned int splash;
static int currentSelection;
static unsigned short splashAge = 0;

void AreaISTBMain(void)
{
    HLibTask *task;
    StoneWork *work;
    FTBL.DebugLogToFileRemoved("_-_-_-_-_-_-_-_- Isitobi -_-_-_-_-_-_-_-_\n");
    FTBL.SetPlayerControlEnabled(0);
    task = FTBL.HLib_EnqueueTaskWithParameter(Init2_Callback, 5, 11, sizeof(StoneWork), TASK_EVNT);
    work = (StoneWork *)FTBL.HLib_GetTaskParameter(task);
    work->self = task;
    work->retries = 0;
    work->succeeded = 0;
    work->selection = 99;
    if (g_currentMap == ISTB_AREA_KRK2)
        work->selection = 0;
    if (g_currentMap == ISTB_AREA_KRK4)
        work->selection = 1;
    currentSelection = work->selection;
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    Queue_EVT_AT_ReadChr(work->self, names);
    FTBL.shenSetTimeSimulationTo(0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"ISTB", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.qteBattleUtil(2, 0, 0, 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0519A000.smp");
    FTBL.EV_FreeAuthorPak(1, "0519");
}

void Init2_Callback(void *data)
{
    StoneWork *work = (StoneWork *)data;
    float board[4];
    work->phase = 0;
    ISTB_SET_GRAVITY(ISTB_RYO, 0);
    ISTB_SET_GRAVITY(ISTB_SIN, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ISTB_RYO), 0);
    if (!work->selection) {
        FTBL.DebugLogToFileRemoved(">>> cnt = %d, frame = %d\n", work->retries, work->retries * 6 + 24);
        FTBL.qteBattleUtil(8, 8, 6, work->retries * 6 + 24, 0);
        FTBL.qteBattleUtil(9, 64, 4, 256, 0);
        FTBL.qteBattleUtil(10, 0, 0, 0, 0);
    } else {
        FTBL.DebugLogToFileRemoved(">>> cnt = %d, frame = %d\n", work->retries, work->retries * 8 + 36);
        FTBL.qteBattleUtil(8, 4, work->retries + 5, work->retries * 8 + 36, 0);
        FTBL.qteBattleUtil(9, 64, 4, 256, 4);
        FTBL.qteBattleUtil(10, 64, 0, 0, 0);
    }
    FTBL.qteBattleUtil(1, 0, 0, 0, 0);
    board[0] = 1.0f;
    board[1] = 1.0f;
    board[2] = 1.0f;
    board[3] = 1.0f;
    FTBL.SetScenarioReflectorBoardValues(ISTB_RYO, board);
    FTBL.SetRefBoardParamsForCharaById(ISTB_RYO, 1);
    FTBL.SetRefBoardParamsForCharaById(ISTB_RYO, 1);
    FTBL.SetScenarioReflectorBoardValues(ISTB_SIN, board);
    FTBL.SetRefBoardParamsForCharaById(ISTB_SIN, 1);
    FTBL.SetRefBoardParamsForCharaById(ISTB_SIN, 1);
    if (work->retries) {
        FTBL.ST_LoadTitleFade();
        work->phase = 1;
    }
    FTBL.HLib_SetCurrentTaskTo(Main_Callback);
}

void Main_Callback(void *data)
{
    StoneWork *work = (StoneWork *)data;
    int delay = 0;
    int count;
    switch (work->phase) {
    case 0:
        FTBL.HLib_SetCurrentTaskTo(Auth1_Callback);
        work->phase = 1;
        break;
    case 1:
        delay = BeginQte(work);
        work->phase = 2;
        break;
    case 2:
        work->qteAge = 0;
        work->retryAge = 0;
        FTBL.HLib_SetCurrentTaskTo(Qte_Callback);
        work->phase = 3;
        break;
    case 3:
        if (FTBL.qteBattleUtil(4, 0, 0, 0, 0)) {
            count = !work->selection ? 3 : 5;
            if (FTBL.qteBattleUtil(6, 0, 0, 0, 0) == count) {
                FTBL.DebugLogToFileRemoved("<<< Ishitobi Succeed!! >>>\n");
                work->succeeded = 1;
                work->phase = 4;
            } else
                FTBL.HLib_SetCurrentTaskTo(Auth2_Callback);
        }
        break;
    case 4:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            if (work->succeeded)
                work->phase = 5;
            else {
                FTBL.HLib_SetCurrentTaskTo(Init2_Callback);
                delay = 10;
            }
        }
        break;
    case 5:
        FTBL.EV_authSceneCtrl(-7);
        FTBL.EV_authSceneCtrl(-10);
        FTBL.HLib_SetCurrentTaskTo(Exit_Callback);
        break;
    default:
        FTBL.EventAssertNullsub(__FILE__, 203);
        break;
    }
    if (delay)
        FTBL.HLib_SetupTaskDelay(delay);
}

void Qte_Callback(void *data)
{
    StoneWork *work = (StoneWork *)data;
    if (!work->qteAge) {
        FTBL.EV_authSceneCtrl(-8);
        FTBL.EV_authSceneCtrl(-11);
        if (!work->selection)
            FTBL.EV_authSceneCtrl(ISTB_AUTHOR_QTE);
        else
            FTBL.EV_authSceneCtrl(ISTB_AUTHOR_QTE_KRK4);
    }
    if (work->qteAge == 10) {
        FTBL.qteBattleUtil(3, 0, 0, 0, 0);
        work->phase = 3;
        FTBL.HLib_SetCurrentTaskTo(Main_Callback);
    }
    work->qteAge++;
}

void Auth1_Callback(void *data)
{
    StoneWork *work = (StoneWork *)data;
    FTBL.EV_authSceneCtrl(-10);
    FTBL.HLib_SetCurrentTaskTo(Main_Callback);
    if (!work->selection)
        QueueSkAuthorStart(work->self, ISTB_AUTHOR_START, 0, 0, 0);
    else
        QueueSkAuthorStart(work->self, ISTB_AUTHOR_START_KRK4, 0, 0, 0);
    FTBL.SetScreenFilmFade(50, 255, 0, 0, 0, 0, 0, 0, 0);
}

void Auth2_Callback(void *data)
{
    StoneWork *work = (StoneWork *)data;
    if (!work->retryAge) {
        FTBL.EV_authSceneCtrl(-7);
        FTBL.EV_authSceneCtrl(-11);
        if (work->retries <= 5)
            work->retries++;
    }
    if (!work->selection && work->retryAge == 30) {
        work->phase = 4;
        EVT_RepeatCall(RetryTitle_Callback, 0, 370, 1);
        EVT_RepeatCall(SpawnSplash_Callback, 0, 40, 1);
        QueueSkAuthorStart(work->self, ISTB_AUTHOR_RETRY, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(Main_Callback);
    }
    if (work->selection && work->retryAge == 85) {
        work->phase = 4;
        EVT_RepeatCall(RetryTitle_Callback, 0, 370, 1);
        EVT_RepeatCall(SpawnSplash_Callback, 0, 40, 1);
        QueueSkAuthorStart(work->self, ISTB_AUTHOR_RETRY_KRK4, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(Main_Callback);
    }
    work->retryAge++;
}

void RetryTitle_Callback(void *data)
{
    FTBL.Queue_ST_LoadTitleFadeIn(ISTB_RETRY_TITLE);
}

int BeginQte(StoneWork *work)
{
    int frames;
    FTBL.EV_authSceneCtrl(-8);
    FTBL.EV_authSceneCtrl(-11);
    if (!work->selection) {
        FTBL.EV_authSceneCtrl(ISTB_AUTHOR_THROW);
        frames = FTBL.EV_authSceneCtrl(-5) - 2;
        if (work->retries) {
            FTBL.SetScreenFilmFade(50, 255, 0, 0, 0, 0, 0, 0, 0);
            FTBL.EV_LoadCutsceneFromCurrentScene("0519A000.smp");
            FTBL.EV_LoadAudio("0519A000", 1);
            EVT_AT_SetSample(30, 0, 0);
        }
    } else {
        FTBL.EV_authSceneCtrl(ISTB_AUTHOR_THROW_KRK4);
        frames = FTBL.EV_authSceneCtrl(-5) - 2;
        if (work->retries) {
            FTBL.SetScreenFilmFade(50, 255, 0, 0, 0, 0, 0, 0, 0);
            FTBL.EV_LoadCutsceneFromCurrentScene("0519A000.smp");
            FTBL.EV_LoadAudio("0519A000", 4);
            EVT_AT_SetSample(30, 0, 0);
        } else {
            EVT_AT_MenCtrl(1, 255, 0, 0, 0, 0, 0, 0, 0, 1);
            FTBL.DebugLogToFileRemoved("FadeIn\n");
        }
    }
    return frames;
}

void SpawnSplash_Callback(void *data)
{
    Vector3 volume = {0.2f, 0.0f, 0.2f};
    Vector3 velocity = {0.0074509755f, 0.059999995f, 0.016078435f};
    Vector3 variance = {0.015294112f, 0.041176476f, 0.039607845f};
    Vector3 position = {33.34f, 100.1f, 39.830002f};
    if (currentSelection) {
        position.x = 47.73f;
        position.y = 100.6f;
        position.z = 61.53f;
    }
    FTBL.HLib_EnqueueTaskWithoutParameter(Splash_Callback, 4, 11, TASK_EVNT);
    splash = FTBL.CreateSplashEffect(&position, &volume, 3000, 0.02f, 0, 99.8f, 0, 0);
    FTBL.SetParticleEffectColour(1, splash, 246, 175, 175, 255);
    FTBL.SetParticleEffectPosition(1, splash, &position, &volume);
    FTBL.SetParticleEffectVelocity(1, splash, &velocity, &variance);
    FTBL.SetParticleEffectBirthRate(1, splash, 0.1f);
    FTBL.SetParticleEffectSize(1, splash, 0.012549019418656826f, 0.012549019418656826f);
    FTBL.ResumeParticleEffect(1, splash);
}

void Splash_Callback(void *data)
{
    switch (splashAge) {
    case 20:
        FTBL.SetParticleEffectBirthRate(1, splash, 0.0f);
        splashAge++;
        break;
    case 45:
        splashAge = 0;
        FTBL.SuspendParticleEffect(1, splash);
        FTBL.FreeParticleEffect(1, splash);
        FTBL.HLib_DestroyCurrentTask();
        break;
    default:
        splashAge++;
        break;
    }
}

void Exit_Callback(void *data)
{
    StoneWork *work = (StoneWork *)data;
    EventDateTime time;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    time.minute += 30;
    if (time.minute >= 60) {
        time.minute -= 60;
        time.hour++;
    }
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    FTBL.shenSetTimeSimulationTo(1);
    FTBL.qteBattleUtil(5, 0, 0, 0, 0);
    ISTB_SET_GRAVITY(ISTB_RYO, 1);
    ISTB_SET_GRAVITY(ISTB_SIN, 1);
    FTBL.npcFree(ISTB_SIN);
    FTBL.npcLockUnlock(ISTB_SIN);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EVS_LoopCamera(0, 0, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.EV_ControlPackageResources(0, 6, "ISTB", 0, 0);
    FTBL.EV_HLKillOtherHierarchyTask(work->self);
    if (!work->selection)
        FTBL.EV_scenQueueLoad(g_actNumber, ISTB_EXIT_KRK2, 0);
    else
        FTBL.EV_scenQueueLoad(g_actNumber, ISTB_EXIT_KRK4, 0);
}
#endif
