
#include "s2/events/event_dc.h"
#include "s2/event_queued_dc.h"

typedef void (*AreaBaseCallback)(unsigned int *parameters);
typedef struct AreaBaseState {
    float initFlag;
    int nightAuthorState;
    int cleanupState;
} AreaBaseState;
#define AreaFlag_KeepPlayerControlLocked 1U
#define AreaFlag_DeferPreAreaMainLoop 2U
#define AreaFlag_SuppressAutoControlLock 4U
#define LoadingPhase_SkipFinishLoading 8U
#define TASK_BASE SHEN_NAME4('b', 'a', 's', 'e')
#define AREA_BASE_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define AREA_BASE_NIGHT SHEN_NAME4('Y', 'O', 'R', 'U')

#define AREA_BASE_REVISION_WRS2 10
#define AREA_BASE_REVISION_Q200 20
#define AREA_BASE_REVISION_TBAK 30
#ifndef SHENMUE_AREA_BASE_REVISION
#define SHENMUE_AREA_BASE_REVISION AREA_BASE_REVISION_TBAK
#endif

extern AreaBaseCallback areaBaseCallbacks[10];
extern const char areaBaseSourceFile[], areaBaseIllegalFunction[];
void EVT_AreaBaseInit(void);
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);

#if defined(SHENMUE_EVENT_ENTRY)
void EVT_PreAreaMainLoop(int *parameters)
{
    if (parameters == 0) {
        EVT_AreaBaseInit();
    } else {
        if (*parameters <= 10)
            areaBaseCallbacks[*parameters - 1]((unsigned int *)(parameters + 1));
        else
#if defined(SHENMUE_AREA_BASE_CLOCK_NIGHT)
            FTBL.EventAssertMessageNullsub(areaBaseSourceFile, 109, areaBaseIllegalFunction);
#else
            FTBL.EventAssertMessageNullsub(areaBaseSourceFile, 108, areaBaseIllegalFunction);
#endif
    }
}
#else
void EVT_PreAreaMainLoop_Callback(void *taskData);
void EVT_AreaMainLoop_Callback(void *taskData);
void EVT_NightAuthor_Callback(void *taskData);

static HLibTask *preAreaMainLoopTask;
static int savedStartParameters;
static AreaBaseState areaBaseState;

unsigned int areaFlags;
#if !defined(SHENMUE_AREA_BASE_CLOCK_NIGHT)
unsigned int areaHeldItemRestorePending;
#endif

void AreaBaseDummyCallback(unsigned int *parameters)
{
    FTBL.DebugLogToFileRemoved("EVENT areabase : call %d is dummy function\n", *parameters);
}

AreaBaseCallback areaBaseCallbacks[10] = {AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback,
    AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback, AreaBaseDummyCallback};

void AreaBaseSetCallback(int index, AreaBaseCallback callback)
{
    FTBL.DebugLogToFileRemoved(" = = = Call EZ Functionz on EV1 = = =\n");
    areaBaseCallbacks[index - 1] = callback;
}

const char areaBaseSourceFile[] = __FILE__;
const char areaBaseIllegalFunction[] = "EVENT areabase : Illigal function number\n";

void EVT_AreaBaseInit(void)
{
    EventDateTime dateTime;
    char *state;
    int i;
    savedStartParameters = FTBL.HasSavedAreaJumpLocation();
    FTBL.DebugLogToFileRemoved("===START INIT AREA===\n");
    FTBL.EventSystemNullsub();
    areaFlags = 0;
#if !defined(SHENMUE_AREA_BASE_CLOCK_NIGHT)
    areaHeldItemRestorePending = FTBL.EV_GetPlayerFlag(11, 966);
#endif
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    FTBL.SetWeatherUpdateEnabledTo(1);
    FTBL.RefreshWeatherForCurrentTime(0);
    FTBL.UpdateWeather();
    FTBL.BeginPrimarySampleExecution(0);
    FTBL.BeginSecondarySampleExecution(0);
#if !defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) || SHENMUE_AREA_BASE_REVISION >= AREA_BASE_REVISION_Q200
    FTBL.EV_SetPlayerFlag(12, 1006, 0);
#endif
    preAreaMainLoopTask = FTBL.HLib_EnqueueTaskWithoutParameter(EVT_PreAreaMainLoop_Callback, 3, 11, TASK_BASE);
    state = (char *)&areaBaseState;
    for (i = 0; i < 12; i++)
        state[i] = 0;
    areaBaseState.initFlag = 1.0f;
    if ((unsigned char)(dateTime.hour - 5) > 13)
        areaBaseState.cleanupState = 0;
    else
        areaBaseState.cleanupState = 1;
    AreaMain(savedStartParameters);
    FTBL.DebugLogToFileRemoved("RESUME START:");
    if (savedStartParameters) {
        FTBL.EV_ApplyTranslationClockVisibility();
        FTBL.DebugLogToFileRemoved("ON\n");
    } else {
        FTBL.DebugLogToFileRemoved("OFF\n");
    }
    if ((FTBL.shenUpdateFlagEvaluation(1) & 1) && !(areaFlags & AreaFlag_SuppressAutoControlLock))
        areaFlags |= AreaFlag_KeepPlayerControlLocked;
    if (!(areaFlags & AreaFlag_DeferPreAreaMainLoop))
        FTBL.QueueImmediateNextFadeTask(preAreaMainLoopTask);
    FTBL.SetAreaCleanupCallback(AreaCleanup);
    FTBL.DebugLogToFileRemoved("===END INIT AREA===\n");
    FTBL.EventSystemNullsub();
}

void EVT_PreAreaMainLoop_Callback(void *taskData)
{
    if ((areaFlags & AreaFlag_KeepPlayerControlLocked) == 0)
        FTBL.SetPlayerControlEnabled(1);
    FTBL.EV_SetCanDoAreaJumpTo(1);
#if defined(SHENMUE_AREA_BASE_CLOCK_NIGHT)
    FTBL.FinishLoading();
#else
    if (!(FTBL.GetLoadingPhaseFlags() & LoadingPhase_SkipFinishLoading))
        FTBL.FinishLoading();
    if (areaHeldItemRestorePending) {
        FTBL.EV_setHoldStatesFlags(0);
        areaHeldItemRestorePending = 1;
    }
#endif
    FTBL.HLib_SetCurrentTaskTo(EVT_AreaMainLoop_Callback);
}

void EVT_AreaMainLoop_Callback(void *taskData)
{
#if defined(SHENMUE_AREA_BASE_CLOCK_NIGHT)
    EventDateTime dateTime;
#endif
    FTBL.shenUpdateFlagEvaluation(0);
#if defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) && SHENMUE_AREA_BASE_REVISION < AREA_BASE_REVISION_Q200
    if (FTBL.ryoNotBusy()) {
#else
    if (FTBL.ryoNotBusy() && !FTBL.EV_GetPlayerFlag(11, 1006)) {
#endif
#if defined(SHENMUE_AREA_BASE_CLOCK_NIGHT)
        FTBL.GetCurrentDateTime((DateTime *)&dateTime);
        if ((unsigned char)(dateTime.hour - 5) > 13) {
#else
        if (FTBL.EV_GetShenmueFlag(255)) {
#endif
            if (areaBaseState.cleanupState == 1) {
                int author = FTBL.drGetCurrentAuthorId();
                if (!author) {
                    if (FTBL.FindAuthor(AREA_BASE_NIGHT)) {
                        FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) && SHENMUE_AREA_BASE_REVISION < AREA_BASE_REVISION_TBAK
                        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(AREA_BASE_RYO), 0xf003, 0);
#else
                        FTBL.shenToggleAreaFlags(1);
#endif
                        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
#if !defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) || SHENMUE_AREA_BASE_REVISION >= AREA_BASE_REVISION_Q200
                        FTBL.spmFadeOut(0);
#endif
                        FTBL.HLib_SetCurrentTaskTo(EVT_NightAuthor_Callback);
                        areaBaseState.nightAuthorState = 0;
                    } else {
                        AreaCleanup(1);
                    }
#if defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) && SHENMUE_AREA_BASE_REVISION < AREA_BASE_REVISION_Q200
                    areaBaseState.cleanupState = 0;
                }
#else
                }
                areaBaseState.cleanupState = 0;
#endif
            }
        } else if (areaBaseState.cleanupState == 0 && !FTBL.drGetCurrentAuthorId()) {
            AreaCleanup(0);
            areaBaseState.cleanupState = 1;
        }
    }
#if defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) && SHENMUE_AREA_BASE_REVISION < AREA_BASE_REVISION_TBAK
    if (FTBL.EV_GetPlayerFlag(11, 966) && FTBL.ryoNotBusy()) {
        FTBL.EV_SetPlayerFlag(12, 966, 0);
        FTBL.RefreshPadTaskData(0);
    }
#elif defined(SHENMUE_AREA_BASE_CLOCK_NIGHT)
    if (FTBL.EV_GetPlayerFlag(11, 966) && FTBL.shenCheckIsBusy()) {
        FTBL.EV_SetPlayerFlag(12, 966, 0);
        FTBL.RefreshPadTaskData(0);
    }
#else
    if (FTBL.EV_GetPlayerFlag(11, 966) && FTBL.IsItemSelectable())
        FTBL.EV_SetPlayerFlag(12, 966, 0);
    if (FTBL.EV_GetPlayerFlag(11, 966) && FTBL.shenCheckIsBusy())
        FTBL.RefreshPadTaskData(0);
    if (areaHeldItemRestorePending && !FTBL.IsItemSelectable() && !FTBL.EV_GetPlayerFlag(11, 966)) {
        FTBL.EV_setHoldStatesFlags(1);
        areaHeldItemRestorePending = 0;
    }
#endif
}

void EVT_NightAuthor_Callback(void *taskData)
{
    switch (areaBaseState.nightAuthorState) {
    case 0:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.EV_DisableCharacterDisplay(AREA_BASE_RYO);
            FTBL.EV_ToggleCharacterFlag(AREA_BASE_RYO);
            AreaCleanup(1);
            FTBL.CreateEventCharacter(3, 0, AREA_BASE_NIGHT);
#if !defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) || SHENMUE_AREA_BASE_REVISION >= AREA_BASE_REVISION_Q200
            FTBL.EV_authSceneCtrl(-16);
#endif
#if !defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) || SHENMUE_AREA_BASE_REVISION >= AREA_BASE_REVISION_TBAK
            FTBL.EV_authSceneCtrl(-8);
#endif
            FTBL.EV_authSceneCtrl(AREA_BASE_NIGHT);
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#if !defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) || SHENMUE_AREA_BASE_REVISION >= AREA_BASE_REVISION_Q200
            FTBL.EV_spmBeginSampleExecute(0);
#endif
            areaBaseState.nightAuthorState = 1;
        }
        break;
    case 1:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            FTBL.EV_EnableCharacterDisplay(AREA_BASE_RYO);
            FTBL.EV_EnsureCharacterDraw(AREA_BASE_RYO);
            FTBL.camCtrlSetState(0);
            FTBL.camCtrlSetFovTransitionTo(1);
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
#if defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) && SHENMUE_AREA_BASE_REVISION < AREA_BASE_REVISION_TBAK
            FTBL.SetPlayerControlEnabled(1);
            FTBL.SetScenarioCharacterKillMode(AREA_BASE_NIGHT, 1);
            FTBL.HLib_SetCurrentTaskTo(EVT_AreaMainLoop_Callback);
#else
            FTBL.EV_authSceneCtrl(-7);
            FTBL.SetScenarioCharacterKillMode(AREA_BASE_NIGHT, 1);
            areaBaseState.nightAuthorState = 2;
#endif
        }
        break;
#if !defined(SHENMUE_AREA_BASE_CLOCK_NIGHT) || SHENMUE_AREA_BASE_REVISION >= AREA_BASE_REVISION_TBAK
    case 2:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.SetPlayerControlEnabled(1);
            FTBL.HLib_SetCurrentTaskTo(EVT_AreaMainLoop_Callback);
        }
        break;
#endif
    }
}
#endif
