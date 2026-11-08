#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_queued_dc.h"

#define WBSR_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define WBSR_SAI SHEN_NAME4('S', 'A', 'I', '_')
#define WBSR_KNK SHEN_NAME4('K', 'N', 'K', '_')
#define WBSR_SIZ SHEN_NAME4('S', 'I', 'Z', '_')
#define WBSR_A048 SHEN_NAME4('a', '0', '4', '8')
#define WBSR_DOOR_RIGHT SHEN_NAME4('d', 'r', '2', 'R')
#define WBSR_DOOR_LEFT SHEN_NAME4('d', 'r', '2', 'L')
#define WBSR_TASK SHEN_NAME4('s', 'u', 's', 'u')
#define WBSR_LOCAL_XYZ 0x38000000U

typedef struct WbsrState {
    int phase, reserved04, reserved08, counter, frames;
    char reserved14[0x38];
    HLibTask *task;
    int reserved50[2];
    int timer;
} WbsrState;
typedef struct WbsrWork {
    void (*choiceCallback)(int choice);
    void (*nextCallback)(void);
    int reserved08;
    unsigned int authorFlags;
    shen_name sceneArea;
    int sceneEntry;
    int reserved18;
    const char *package;
    shen_name *characterList;
    int reserved24;
    int choice0, choice1, defaultChoice;
} WbsrWork;

int wbsrEntry;
WbsrState wbsrState;
WbsrWork wbsrWork;
extern int wbsrBaseEntry;
extern const char wbsrAuthorPackage[];

void WbsrMain(int *argument);
void WbsrCreateSmoke(void);
void WbsrInitializeCallback(void *data);
void WbsrPackageReady(void);
void WbsrOpeningAuthorDone(void);
void WbsrChoose(int choice);
void WbsrSelect1Callback(void *data);
void WbsrSelect2Callback(void *data);
void WbsrStartTomorrow(void);
void WbsrExit2Callback(void *data);
void WbsrExit3Callback(void *data);
void WbsrExit4Callback(void *data);
void WbsrQueueScenarioA(void);
void WbsrScenarioBCallback(void *data);
void WbsrScenarioACallback(void *data);
void WbsrResetCounters(int phase);
void WbsrReleaseResources(void);
void WbsrAuthorWaitCallback(void *data);
void WbsrStartAuthor(int author, void (*callback)(void), unsigned int flags);
void WbsrChoiceWaitCallback(void *data);
void WbsrStartChoice(int choice0, int choice1, int timeout, int defaultChoice, void (*callback)(int choice));
void WbsrPackageWaitCallback(void *data);
void WbsrLoadPackage(const char *package, shen_name *characters, void (*callback)(void));
void WbsrSceneChangeCallback(void *data);
void WbsrQueueSceneChange(shen_name area, int entry, unsigned int flags);
void WbsrFreePackage(void);
void WbsrPlaceCharacter(shen_name name, Vector3 *position, int angle);
void WbsrPlaceCharacterAt(shen_name name, float x, float y, float z, int angle);

#if defined(SHENMUE_EVENT_ENTRY)
unsigned int wbsrSmokeEffects[16] __attribute__((section(".bss")));
Vector3 wbsrSmokePositions[2] = {{330.64f, 128.05f, 268.0f}, {341.21f, 128.05f, 268.0f}};
 
unsigned int wbsrScriptVisit[] = {0, 4, 0, 0,
    0x80010005, WBSR_SAI,
    0x80010015, WBSR_KNK,
    0x80010015, WBSR_SIZ,
    0x80020027, (unsigned int)"0031E000", 0,
    0x80000004,
    0x80010001, 0,
    0x8001000d, 1,
    0x8002001c, WBSR_A048, WBSR_SAI,
    0x80010009, -30,
    0x8006001a, 97, 84, 1, 2, 97, 150,
    0x40000001,
    0x80010001, 1,
    0x80010009, -40,
    0x8001001d, WBSR_A048,
    0x80020007, 70, 1,
    0x8001000e, 2,
    0x40000002,
    0x80010001, 2,
    0x80010009, 0,
    0x8001001d, WBSR_A048,
    0x80050011, WBSR_A048, 0, 0, 0, 0,
    0x80050011, WBSR_SAI, 0, 0, 0, 0,
    0x80020007, 70, 0,
    0x8001000e, 1};
unsigned int wbsrScriptExhausted[] = {0, 4, 0, 0,
    0x80000021,
    0x80050011, WBSR_SAI, 33231, 12698, 26710, 34861,
    0x80050011, WBSR_RYO, 33598, 12697, 26915, 40049,
    0x80010022, WBSR_SAI,
    0x8002001b, WBSR_RYO, 5134,
    0x8001000d, 1,
    0x80020016, WBSR_RYO, 1006,
    0x8001000d, 100,
    0x8002001b, WBSR_SAI, 5137,
    0x8001000d, 110,
    0x80020016, WBSR_RYO, 1007,
    0x8001000d, 240,
    0x8001000e, 1};
int wbsrScriptChoices[2][3] = {{97, 84, 150}, {97, 84, 150}};

void WbsrMain(int *argument)
{
    HLibTask *task;
    FTBL.EV_SetCanDoAreaJumpTo(0);
    FTBL.EV_SetPlayerFlag(12, 1006, 1);
    wbsrEntry = *argument;
    FTBL.DebugLogToFileRemoved("=== SusuHarai Keidai ===[%d]\n", *argument);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    FTBL.FinishLoading();
    task = FTBL.HLib_EnqueueTaskWithoutParameter(WbsrInitializeCallback, 4, 11, WBSR_TASK);
    wbsrState.task = task;
    FTBL.EV_FreeAuthorPak(1, wbsrAuthorPackage);
    FTBL.HLib_SetupTimedTask(task, 2);
}

const char wbsrAuthorPackage[] = "0031";

void WbsrCreateSmoke(void)
{
    int i;
    for (i = 0; i < 2; i++) {
        Vector3 position = {333.15079f, 127.98090f, 247.705f};
        Vector3 volume = {0.1f, 0.0f, 0.1f};
        Vector3 variance = {0.0f, 0.0f, 0.0f};
        Vector3 velocity = {0.0f, 0.016f, 0.0f};
        Vector3 reserved[4];
        position.x = wbsrSmokePositions[i].x;
        position.y = wbsrSmokePositions[i].y;
        position.z = wbsrSmokePositions[i].z;
        wbsrSmokeEffects[i] = FTBL.CreateSmokeEffect(&position, &volume, 70, 0.08f, 0, 0);
        FTBL.SetSmokeEffectFadeFactor(wbsrSmokeEffects[i], 21);
        FTBL.SetSmokeEffectFalloff(wbsrSmokeEffects[i], 110);
        FTBL.SetParticleEffectColour(0, wbsrSmokeEffects[i], 0, 144, 144, 144);
        FTBL.SetParticleEffectInitialScale(0, wbsrSmokeEffects[i], 0.329f);
        FTBL.SetParticleEffectSize(0, wbsrSmokeEffects[i], 1.56f, 1.41f);
        FTBL.SetParticleEffectBirthRate(0, wbsrSmokeEffects[i], 0.0078f);
        FTBL.SetParticleEffectVelocity(0, wbsrSmokeEffects[i], &velocity, &variance);
        FTBL.ResumeParticleEffect(0, wbsrSmokeEffects[i]);
    }
}
#else
int wbsrBaseEntry = 2;
shen_name wbsrCharacters[] = {WBSR_SAI, WBSR_KNK, WBSR_SIZ, 0};

void WbsrInitializeCallback(void *data)
{
    FTBL.SetScreenFilmFade(1, 255, 0, 0, 0, 255, 0, 0, 0);
    FTBL.DebugLogToFileRemoved("Pack,NPC Load\n");
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.CheckWarmStartNpcRoute(WBSR_SAI);
    FTBL.DebugLogToFileRemoved("Lock NPC SAI_\n");
    FTBL.EV_ResetFreeTalkFlags();
    WbsrLoadPackage("SUS1", wbsrCharacters, WbsrPackageReady);
    FTBL.HLib_DestroyCurrentTask();
}

void WbsrPackageReady(void)
{
    FTBL.EV_ControlPackageResources(2, 2, "SUS1", 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "SUS1", 0, 0);
    FTBL.EV_SetCharacterAlphaEnabled(WBSR_SAI, 0);
    FTBL.EV_SetCharaControllerFlags(WBSR_SAI, 3, 0);
    if (wbsrEntry == 1) {
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        wbsrState.timer = 450;
        WbsrPlaceCharacterAt(WBSR_RYO, 336.45001f, 126.97f, 263.59f, 0x8000);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.HLib_EnqueueTaskWithoutParameter(WbsrScenarioBCallback, 4, 11, WBSR_TASK);
        WbsrResetCounters(100);
    } else if (wbsrEntry == 2) {
        FTBL.EV_stepSetFlags(892, 1);
        FTBL.ExecuteCurrAreaMainFunc_Wrapper(1, &wbsrBaseEntry);
        FTBL.HLib_EnqueueTaskWithoutParameter(WbsrScenarioBCallback, 4, 11, WBSR_TASK);
        WbsrResetCounters(200);
    } else {
        FTBL.EV_DisableCharacterDisplay(WBSR_SIZ);
        FTBL.EV_LoadCutsceneFromCurrentScene("0031E000");
        FTBL.SetScenarioCharacterFlag40(WBSR_DOOR_RIGHT, 0);
        FTBL.SetScenarioCharacterFlag40(WBSR_DOOR_LEFT, 0);
        WbsrStartAuthor(SHEN_NAME4('A', 'U', 'T', '0'), WbsrOpeningAuthorDone, 1);
    }
}

void WbsrOpeningAuthorDone(void)
{
    FTBL.SetScenarioCharacterFlag40(WBSR_DOOR_RIGHT, 1);
    FTBL.SetScenarioCharacterFlag40(WBSR_DOOR_LEFT, 1);
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.shenSetTimeSimulationTo(0);
    WbsrStartChoice(333, 334, 1800, 0, WbsrChoose);
}

void WbsrChoose(int choice)
{
    switch (choice) {
    case 0:
        FTBL.DebugLogToFileRemoved("交渉決裂\n");
        FTBL.SetPlayerGameFlags(32, 0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.InitiateCleanupForSceneChara(WBSR_SAI);
        FTBL.DebugLogToFileRemoved("EV_FREETALK_RESET( ID_SAI )\n");
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.EV_stepSetFlags(70, 2);
        FTBL.ExecuteCurrAreaMainFunc_Wrapper(1, &wbsrBaseEntry);
        {
            Vector3 position = {336.013f, 126.97f, 269.26001f};
            Vector3i rotation = {0, 0x4000, 0};
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(WBSR_RYO), 0xf002, 0);
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(WBSR_RYO), 17, 0);
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WBSR_RYO), 71, 0, 0);
            FTBL.EV_AT_SetAngY_0(WBSR_RYO, WBSR_LOCAL_XYZ, &rotation, 0);
            FTBL.EV_AT_SetPos(WBSR_RYO, WBSR_LOCAL_XYZ, &position);
            FTBL.EV_DisableCharacterDisplay(WBSR_A048);
            FTBL.HLib_EnqueueTaskWithoutParameter(WbsrSelect1Callback, 4, 11, WBSR_TASK);
        }
        break;
    case 1:
        FTBL.DebugLogToFileRemoved("侵入処理\n");
        FTBL.EV_stepSetFlags(70, 0);
        FTBL.ExecuteCurrAreaMainFunc_Wrapper(1, &wbsrBaseEntry);
        FTBL.SetRenderClockAndCompassTo(0);
        FTBL.shenSetTimeSimulationTo(1);
        WbsrStartAuthor(SHEN_NAME4('A', 'U', 'T', '2'), WbsrQueueScenarioA, 0);
        break;
    }
}

void WbsrSelect1Callback(void *data)
{
    if (FTBL.EV_GetShenmueFlag(698)) {
        FTBL.SetPlayerControlEnabled(0);
        FTBL.SetPlayerGameFlags(30, 0);
        FTBL.SetRenderClockAndCompassTo(0);
        FTBL.shenSetTimeSimulationTo(1);
        FTBL.EV_EnableCharacterDisplay(WBSR_A048);
        WbsrStartAuthor(SHEN_NAME4('A', 'U', 'T', '2'), WbsrQueueScenarioA, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
    if (FTBL.EV_GetShenmueFlag(699)) {
        FTBL.SetPlayerControlEnabled(0);
        FTBL.SetPlayerGameFlags(30, 0);
        FTBL.EV_stepSetFlags(699, 0);
        FTBL.ExecuteCurrAreaMainFunc_Wrapper(1, &wbsrBaseEntry);
        FTBL.SetRenderClockAndCompassTo(0);
        FTBL.shenSetTimeSimulationTo(1);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(WBSR_RYO), 55, 0);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(WBSR_RYO), 17, 0);
        FTBL.HLib_SetCurrentTaskTo(WbsrSelect2Callback);
        FTBL.HLib_SetupTaskDelay(15);
    }
}

void WbsrSelect2Callback(void *data)
{
    FTBL.EV_EnableCharacterDisplay(WBSR_A048);
    WbsrStartAuthor(SHEN_NAME4('A', 'U', 'T', '1'), WbsrStartTomorrow, 0);
    FTBL.HLib_DestroyCurrentTask();
}

void WbsrStartTomorrow(void)
{
    FTBL.EV_stepSetFlags(70, 1);
    FTBL.ExecuteCurrAreaMainFunc_Wrapper(1, &wbsrBaseEntry);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    {
        Vector3 position = {337.67001f, 126.97f, 280.07001f};
        Vector3i rotation = {0, 10922, 0};
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(WBSR_RYO), 17, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WBSR_RYO), 71, 0, 0);
        FTBL.EV_AT_SetPos(WBSR_RYO, WBSR_LOCAL_XYZ, &position);
        FTBL.EV_AT_SetAngY_0(WBSR_RYO, WBSR_LOCAL_XYZ, &rotation, 0);
        FTBL.SetScreenFilmFade(60, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_FreeAuthorPak(1, "0401");
        FTBL.EV_PlayDialogue("0401A001");
        FTBL.SetPlayerControlEnabled(0);
        FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(WBSR_RYO), 0, 0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(WBSR_RYO), 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(WBSR_RYO), 0xf002, 0);
        FTBL.HLib_SetupTimedTask(FTBL.HLib_EnqueueTaskWithoutParameter(WbsrExit2Callback, 4, 11, WBSR_TASK), 30);
    }
}

void WbsrExit2Callback(void *data)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetPlayerGameFlags(32, 0);
    FTBL.EV_SetPlayerFlag(12, 1006, 0);
    FTBL.plymAdvanceTime(23, 0);
    FTBL.HLib_SetCurrentTaskTo(WbsrExit3Callback);
}

void WbsrExit3Callback(void *data)
{
    switch (FTBL.plymGetAreaExitState()) {
    case 0:
        FTBL.HLib_SetCurrentTaskTo(WbsrExit4Callback);
        break;
    case 1:
        FTBL.EV_PlayDialogue("0401A002");
        FTBL.HLib_SetCurrentTaskTo(WbsrExit4Callback);
        FTBL.HLib_SetupTaskDelay(40);
        break;
    }
}

void WbsrExit4Callback(void *data)
{
    FTBL.SetPlayerGameFlags(30, 0);
    FTBL.SetPlayerControlEnabled(1);
    WbsrReleaseResources();
    FTBL.EV_SetCanDoAreaJumpTo(1);
    FTBL.EV_SetPlayerFlag(12, 1006, 0);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.HLib_DestroyCurrentTask();
}

void WbsrQueueScenarioA(void)
{
    FTBL.EV_DisableCharacterDisplay(WBSR_SAI);
    FTBL.EV_stepSetFlags(70, 0);
    FTBL.ExecuteCurrAreaMainFunc_Wrapper(1, &wbsrBaseEntry);
    EVT_AT_SetAngY(WBSR_RYO, 0xffffffffU, 0.0f, 0);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(WBSR_RYO), 12, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(WBSR_RYO), 1);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(WBSR_RYO), 15, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(WBSR_RYO), 0xf002, 0);
    FTBL.HLib_EnqueueTaskWithoutParameter(WbsrScenarioACallback, 4, 11, WBSR_TASK);
    WbsrResetCounters(4);
}

void WbsrScenarioBCallback(void *data)
{
    ++wbsrState.frames;
    switch (wbsrState.phase) {
    case 100:
        if (wbsrState.timer-- < 0 && FTBL.ryoNotBusy()) {
            FTBL.SetPlayerControlEnabled(0);
            FTBL.DebugLogToFileRemoved("追い出し\n");
            FTBL.EV_stepSetFlags(892, 1);
            FTBL.ExecuteCurrAreaMainFunc_Wrapper(1, &wbsrBaseEntry);
            WbsrResetCounters(200);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(WBSR_RYO), 0.0f, 0);
            FTBL.SetPlayerGameFlags(31, 0);
            FTBL.SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.HLib_SetupTaskDelay(60);
        }
        break;
    case 200:
        FTBL.SetPlayerGameFlags(30, 0);
        FTBL.EV_EnableCharacterDisplay(WBSR_SAI);
        WbsrStartAuthor(SHEN_NAME4('A', 'U', 'T', 'B'), WbsrStartTomorrow, 1);
        WbsrResetCounters(999);
        break;
    case 999:
        break;
    }
}

void WbsrScenarioACallback(void *data)
{
    ++wbsrState.frames;
    switch (wbsrState.phase) {
    case 19: {
        Vector3 position = {336.51999f, 127.0f, 262.32999f};
        Vector3i rotation = {0, 0, 0};
        SHENMUE_EVENT_BOUNDARY();
        FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(WBSR_RYO), 12, 0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(WBSR_RYO), 1);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(WBSR_RYO), 15, 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(WBSR_RYO), 0xf005, 0);
        FTBL.EV_SetCharaControllerFlags(WBSR_RYO, 2, 0);
        FTBL.EV_SetCharaControllerFlags(WBSR_RYO, 3, 0);
        FTBL.EV_AT_SetPos(WBSR_RYO, WBSR_LOCAL_XYZ, &position);
        FTBL.EV_AT_SetAngY_0(WBSR_RYO, WBSR_LOCAL_XYZ, &rotation, 0);
        WbsrQueueSceneChange(SHEN_NAME4('W', 'B', '0', '1'), 1, 2);
        FTBL.HLib_DestroyCurrentTask();
        break;
    }
    case 4:
        switch (wbsrState.counter) {
        case 0:
            EVT_AT_SetAngY(WBSR_RYO, 0xffffffffU, 0.0f, 0);
            ++wbsrState.counter;
            break;
        case 1:
            FTBL.camCtrlSetFovTransitionTo(0);
            FTBL.camCtrlSetState(0);
            FTBL.SetPlayerControlEnabled(1);
            wbsrState.timer = 1800;
            ++wbsrState.counter;
            break;
        case 2: {
            Vector3 position, target;
            Vector3i rotation;
            if (wbsrState.timer-- < 0 && FTBL.ryoNotBusy()) {
                FTBL.SetPlayerControlEnabled(0);
                WbsrResetCounters(21);
            }
            FTBL.SetDebugScreenTextPosition(2, 8);
            target.x = 336.51999f;
            target.z = 262.84f;
            FTBL.GetCharaPosition(WBSR_RYO, 0xffffffffU, &position, 0);
            FTBL.GetScenarioCharacterEulerAngles(WBSR_RYO, &rotation, 0);
            target.y = position.y;
            break;
        }
        }
        break;
    case 21:
        switch (wbsrState.counter) {
        case 0:
            FTBL.SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.HLib_SetupTaskDelay(60);
            FTBL.SetPlayerGameFlags(31, 0);
            ++wbsrState.counter;
            break;
        case 1:
            FTBL.SetPlayerGameFlags(30, 0);
            FTBL.EV_EnableCharacterDisplay(WBSR_SAI);
            WbsrStartAuthor(SHEN_NAME4('A', 'U', 'T', 'B'), WbsrStartTomorrow, 1);
            FTBL.EV_stepSetFlags(699, 0);
            FTBL.ExecuteCurrAreaMainFunc_Wrapper(1, &wbsrBaseEntry);
            WbsrResetCounters(999);
            break;
        }
        break;
    }
}

void WbsrResetCounters(int phase)
{
    wbsrState.phase = phase;
    wbsrState.counter = 0;
    wbsrState.frames = 0;
}

void WbsrReleaseResources(void)
{
    FTBL.EV_FreeAuthorPak(0, wbsrAuthorPackage);
    WbsrFreePackage();
}

void WbsrAuthorWaitCallback(void *data)
{
    if (!FTBL.EV_authSceneCtrl(-1)) {
        wbsrWork.nextCallback();
        FTBL.HLib_DestroyCurrentTask();
    }
    if (FTBL.EV_authSceneCtrl(-6) == FTBL.EV_authSceneCtrl(-5) - 30 && (wbsrWork.authorFlags & 2))
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
}

void WbsrStartAuthor(int author, void (*callback)(void), unsigned int flags)
{
    wbsrWork.nextCallback = callback;
    wbsrWork.authorFlags = flags;
    if (flags & 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_authSceneCtrl(-11);
    FTBL.EV_authSceneCtrl(author);
    FTBL.HLib_EnqueueTaskWithoutParameter(WbsrAuthorWaitCallback, 4, 11, WBSR_TASK);
}

void WbsrChoiceWaitCallback(void *data)
{
    int result = FTBL.shenGetCtrlSpeakIconFlag();
    if (!FTBL.GetShouldRenderIcons(2)) {
        if (result == wbsrWork.choice0) {
            FTBL.DebugLogToFileRemoved("left\n");
            wbsrWork.choiceCallback(0);
        } else if (result == wbsrWork.choice1) {
            FTBL.DebugLogToFileRemoved("right\n");
            wbsrWork.choiceCallback(1);
        } else {
            FTBL.DebugLogToFileRemoved("cancel\n");
            wbsrWork.choiceCallback(wbsrWork.defaultChoice);
        }
        FTBL.HLib_DestroyCurrentTask();
    }
}

void WbsrStartChoice(int choice0, int choice1, int timeout, int defaultChoice, void (*callback)(int choice))
{
    short choices[4];
    choices[0] = -1;
    choices[1] = choice0;
    choices[2] = choice1;
    choices[3] = -1;
    FTBL.SetupControlSpeakAndUI(choices, timeout, 0, 2);
    wbsrWork.choiceCallback = callback;
    wbsrWork.choice0 = choice0;
    wbsrWork.choice1 = choice1;
    wbsrWork.defaultChoice = defaultChoice;
    FTBL.HLib_EnqueueTaskWithoutParameter(WbsrChoiceWaitCallback, 4, 11, WBSR_TASK);
}

void WbsrPackageWaitCallback(void *data)
{
    int i;
    if (FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0))
        return;
    if (wbsrWork.characterList)
        for (i = 0; wbsrWork.characterList[i]; i++)
            if (!FTBL.ForceLoadNpc(wbsrWork.characterList[i]))
                return;
    wbsrWork.nextCallback();
    FTBL.HLib_DestroyCurrentTask();
}

void WbsrLoadPackage(const char *package, shen_name *characters, void (*callback)(void))
{
    int i;
    wbsrWork.nextCallback = callback;
    wbsrWork.characterList = characters;
    wbsrWork.package = package;
    if (package)
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)wbsrWork.package, 0);
    if (wbsrWork.characterList)
        for (i = 0; wbsrWork.characterList[i]; i++) {
            FTBL.CheckWarmStartNpcRoute(wbsrWork.characterList[i]);
            FTBL.ForceLoadNpc(wbsrWork.characterList[i]);
        }
    FTBL.HLib_EnqueueTaskWithoutParameter(WbsrPackageWaitCallback, 4, 11, WBSR_TASK);
}

void WbsrSceneChangeCallback(void *data)
{
    FTBL.EV_scenQueueLoad(g_actNumber, wbsrWork.sceneArea, wbsrWork.sceneEntry);
    FTBL.HLib_DestroyCurrentTask();
}

void WbsrQueueSceneChange(shen_name area, int entry, unsigned int flags)
{
    HLibTask *task;
    wbsrWork.authorFlags = flags;
    wbsrWork.sceneArea = area;
    wbsrWork.sceneEntry = entry;
    task = FTBL.HLib_EnqueueTaskWithoutParameter(WbsrSceneChangeCallback, 4, 11, WBSR_TASK);
    if (wbsrWork.authorFlags & 2) {
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetupTimedTask(task, 30);
    }
}

void WbsrFreePackage(void)
{
    int i;
    if (wbsrWork.package)
        FTBL.EV_ControlPackageResources(0, 6, wbsrWork.package, 0, 0);
    if (wbsrWork.characterList)
        for (i = 0; wbsrWork.characterList[i]; i++)
            FTBL.npcLockUnlock(wbsrWork.characterList[i]);
}

void WbsrPlaceCharacter(shen_name name, Vector3 *position, int angle)
{
    Vector3i rotation = {0, angle, 0};
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 71, 0, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 0);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(name), 0, 0);
    FTBL.EV_AT_SetPos(name, WBSR_LOCAL_XYZ, position);
    FTBL.EV_AT_SetAngY_0(name, WBSR_LOCAL_XYZ, &rotation, 0);
}

void WbsrPlaceCharacterAt(shen_name name, float x, float y, float z, int angle)
{
    Vector3 position;
    position.x = x;
    position.y = y;
    position.z = z;
    WbsrPlaceCharacter(name, &position, angle);
}
#endif
#else
#include "s2/areas/area_wbsr.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2 {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, sai{SHEN_NAME(SAI_)}, siz{SHEN_NAME(SIZ_)}, a048{SHEN_NAME(a048)};
constexpr shen_name characters[]{sai, SHEN_NAME(KNK_), siz, shen_name{}};
constexpr std::int32_t taskName = 1970500979;
constexpr std::uint32_t localXYZ = 0x38000000U;
struct WbsrState {
    std::int32_t phase, reserved04, reserved08, counter, frames;
    std::byte reserved14[0x3c];
    HLib::Task *task;
    std::uint64_t reserved58;
    std::int32_t timer;
    std::uint32_t reserved64;
};
static_assert(sizeof(WbsrState) == 0x68 && offsetof(WbsrState, task) == 0x50);
WbsrState state{};
std::int32_t entry[4]{}, baseEntry = 2;
void (*nextCallback)() = nullptr;
void (*choiceCallback)(std::int32_t) = nullptr;
std::uint32_t authorFlags{};
shen_name sceneArea{};
std::int32_t sceneEntry{}, choice0{}, choice1{}, defaultChoice{};
const char *package{};
const shen_name *characterList{};
HLib::Task *Queue(HLib::TaskCallback callback) {
    return HLib::EnqueueTaskWithoutParameter(callback, 4, 0x0b, taskName);
}
MotionWork *RyoMotion() {
    return FindMotionWork(ctrlGetScenarioChara(ryo));
}
void ExecuteBase() {
    (void)ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&baseEntry));
}
void ClearGameFlag(std::int32_t flag) {
    (void)SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(flag), nullptr);
}
void Fade(std::int32_t frames, std::int32_t start, std::int32_t target) {
    SetScreenFilmFade(frames, start, 0, 0, 0, target, 0, 0, 0);
}
void SetAuthor(std::int32_t index) {
    if (IsAuthorStateFlagSet(index))
        EV_SetAuthorActiveTo(1);
}
void StartAuthor(shen_name author, void (*callback)(), std::uint32_t flags, bool allowSkip) {
    nextCallback = callback;
    authorFlags = flags;
    (void)EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(author.name));
    (void)Queue(WbsrAuthorWaitCallback);
    if (allowSkip)
        (void)EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
}
void ResetCounters(std::int32_t phase) {
    state.phase = phase;
    state.counter = state.frames = 0;
}
void QueueScenarioA();
void StartTomorrow();
void ExhaustedAuthorDone() {
    (void)SetAuthorStateFlag(31, 1);
    StartTomorrow();
}
void RefusedAuthorDone() {
    (void)SetAuthorStateFlag(32, 1);
    StartTomorrow();
}
void Choose(std::int32_t choice) {
    using enum CameraState;
    if (choice == 1) {
        EV_stepSetFlags(0, 0x46);
        ExecuteBase();
        shenSetClockAndCompassRenderTo(0);
        (void)shenSetTimeSimulationTo(1);
        SetAuthor(30);
        StartAuthor(SHEN_NAME(AUT2), QueueScenarioA, 0, true);
    } else if (!choice) {
        ClearGameFlag(32);
        SetPlayerControlEnabled(1);
        InitiateCleanupForSceneChara(sai);
        camCtrlSetFovTransitionTo(true);
        (void)camCtrlSetState(Init);
        EV_stepSetFlags(2, 0x46);
        ExecuteBase();
        mtwkRequestMotion(RyoMotion(), 0xf002, MotionRegion::LowerBody);
        mtwkSetExtraFlag(RyoMotion(), 17, 0);
        mtwkSetRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
        (void)EV_AT_SetAngY(ryo, localXYZ, {0, 0x4000, 0});
        EV_AT_SetPos(ryo, localXYZ, {336.013f, 126.97f, 269.26001f});
        EV_AT_SetPos(a048, localXYZ, {334.519989f, 128.177001f, 269.390015f});
        (void)Queue(WbsrSelect1Callback);
    }
}
void OpeningAuthorDone() {
    (void)SetAuthorStateFlag(29, 1);
    (void)SetScenarioCharacterFlag40(SHEN_NAME(dr2R), 1);
    (void)SetScenarioCharacterFlag40(SHEN_NAME(dr2L), 1);
    shenSetClockAndCompassRenderTo(1);
    (void)shenSetTimeSimulationTo(0);
    std::int16_t choices[]{-1, 333, 334, -1};
    (void)SetupControlSpeakAndUI(choices, 1800, 0, 2);
    choice0 = 333;
    choice1 = 334;
    defaultChoice = 0;
    choiceCallback = Choose;
    (void)Queue(WbsrChoiceWaitCallback);
}
void PositionRyo(const Vector3 &position) {
    mtwkSetRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(RyoMotion(), 0);
    mtwkSetFrameTime(RyoMotion(), 0, MotionRegion::LowerBody);
    EV_AT_SetPos(ryo, localXYZ, position);
    (void)EV_AT_SetAngY(ryo, localXYZ, {0, 0x8000, 0});
}
void PackageReady() {
    using enum CameraState;
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("SUS1"), 0);
    (void)EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SUS1"), 0);
    EV_SetCharacterAlphaEnabled(sai, false);
    EV_SetCharaControllerFlags(sai, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    if (entry[0] == 1) {
        camCtrlSetFovTransitionTo(true);
        (void)camCtrlSetState(Init);
        state.timer = 450;
        PositionRyo({336.45001f, 126.97f, 263.59f});
        SetPlayerControlEnabled(1);
        Fade(30, 255, 0);
        (void)Queue(WbsrScenarioBCallback);
        ResetCounters(100);
    } else if (entry[0] == 2) {
        EV_stepSetFlags(1, 0x37c);
        ExecuteBase();
        (void)Queue(WbsrScenarioBCallback);
        ResetCounters(200);
    } else {
        EV_DisableCharacterDisplay(siz);
        (void)EV_LoadCutsceneFromCurrentScene("0031E000");
        (void)SetScenarioCharacterFlag40(SHEN_NAME(dr2R), 0);
        (void)SetScenarioCharacterFlag40(SHEN_NAME(dr2L), 0);
        nextCallback = OpeningAuthorDone;
        authorFlags = 1;
        Fade(30, 255, 0);
        StartAuthor(SHEN_NAME(AUT0), OpeningAuthorDone, 1, false);
    }
}
void QueueScenarioA() {
    (void)SetAuthorStateFlag(30, 1);
    EV_DisableCharacterDisplay(sai);
    EV_stepSetFlags(0, 0x46);
    ExecuteBase();
    EVT_AT_SetAngY(ryo, shen_name{0xffffffffU}, 0, 0);
    mtwkSetFrameTime(RyoMotion(), 12, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(RyoMotion(), 1);
    mtwkSetExtraFlag(RyoMotion(), 15, 0);
    mtwkRequestMotion(RyoMotion(), 0xf002, MotionRegion::LowerBody);
    (void)Queue(WbsrScenarioACallback);
    ResetCounters(4);
}
void StartTomorrow() {
    using enum CameraState;
    EV_stepSetFlags(1, 0x46);
    ExecuteBase();
    camCtrlSetFovTransitionTo(true);
    (void)camCtrlSetState(Init);
    mtwkSetExtraFlag(RyoMotion(), 17, 0);
    mtwkSetRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(ryo, localXYZ, {337.67001f, 126.97f, 280.07001f});
    (void)EV_AT_SetAngY(ryo, localXYZ, {0, 10922, 0});
    Fade(60, 255, 0);
    (void)EV_FreeAuthorPak(1, "0401");
    (void)EV_PlayDialogue("0401A001");
    SetPlayerControlEnabled(0);
    mtwkSetFrameTime(RyoMotion(), 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(RyoMotion(), 0);
    mtwkRequestMotion(RyoMotion(), 0xf002, MotionRegion::LowerBody);
    HLib::SetupTimedTask(Queue(WbsrExit2Callback), 30);
}
} // namespace
// @verified - p
void WbsrInit() {
    state = {};
    baseEntry = 2;
    choiceCallback = nullptr;
    nextCallback = nullptr;
    authorFlags = 0;
    sceneArea = {};
    sceneEntry = 0;
    package = nullptr;
    characterList = nullptr;
    choice0 = choice1 = defaultChoice = 0;
    for (auto &value : entry)
        value = 0;
}
// @verified - p
void WbsrMain(std::uintptr_t argument) {
    EV_SetCanDoAreaJumpTo(0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 1);
    entry[0] = *reinterpret_cast<const std::int32_t *>(argument);
    SetPlayerControlEnabled(0);
    Fade(0, 255, 255);
    FinishLoading();
    state.task = Queue(WbsrInitializeCallback);
    (void)EV_FreeAuthorPak(1, "0031");
    HLib::SetupTimedTask(state.task, 2);
}
// @verified - p
void WbsrInitializeCallback(void *) {
    Fade(1, 255, 255);
    (void)EV_NPC_NewLoadSw(0);
    (void)EV_CheckWarmStartNpcRoute(sai);
    (void)EV_ResetFreeTalkFlags();
    nextCallback = PackageReady;
    package = "SUS1";
    characterList = characters;
    (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(package));
    for (const auto *p = characterList; p->name; ++p) {
        (void)EV_CheckWarmStartNpcRoute(*p);
        (void)EV_ForceLoadNpc(*p);
    }
    (void)Queue(WbsrPackageWaitCallback);
    HLib::DestroyCurrentTask();
}
// @verified - p
void WbsrPackageWaitCallback(void *) {
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0))
        return;
    if (characterList)
        for (const auto *p = characterList; p->name; ++p)
            if (!EV_ForceLoadNpc(*p))
                return;
    nextCallback();
    HLib::DestroyCurrentTask();
}
// @verified - p
void WbsrAuthorWaitCallback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        nextCallback();
        HLib::DestroyCurrentTask();
    }
    const auto frame = EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30;
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == frame && (authorFlags & 2))
        Fade(30, 0, 255);
}
// @verified - p
void WbsrChoiceWaitCallback(void *) {
    const auto result = shenGetCtrlSpeakIconFlag();
    if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
        choiceCallback(result == static_cast<std::uint32_t>(choice0) ? 0 : result == static_cast<std::uint32_t>(choice1) ? 1 : defaultChoice);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void WbsrScenarioBCallback(void *) {
    ++state.frames;
    if (state.phase == 100) {
        if (state.timer-- < 0 && ryoNotBusy()) {
            SetPlayerControlEnabled(0);
            EV_stepSetFlags(1, 0x37c);
            ExecuteBase();
            ResetCounters(200);
            mtwkSetSpeed(RyoMotion(), 0, MotionRegion::LowerBody);
            ClearGameFlag(31);
            Fade(60, 0, 255);
            HLib::SetupTaskDelay(60);
        }
    } else if (state.phase == 200) {
        ClearGameFlag(30);
        EV_EnableCharacterDisplay(sai);
        SetAuthor(31);
        nextCallback = ExhaustedAuthorDone;
        authorFlags = 1;
        Fade(30, 255, 0);
        StartAuthor(SHEN_NAME(AUTB), ExhaustedAuthorDone, 1, true);
        ResetCounters(999);
    }
}
// @verified - p
void WbsrScenarioACallback(void *) {
    using enum CameraState;
    ++state.frames;
    if (state.phase == 4) {
        switch (state.counter) {
        case 0:
            EVT_AT_SetAngY(ryo, shen_name{0xffffffffU}, 0, 0);
            ++state.counter;
            break;
        case 1:
            camCtrlSetFovTransitionTo(false);
            (void)camCtrlSetState(Init);
            SetPlayerControlEnabled(1);
            state.timer = 1800;
            ++state.counter;
            break;
        case 2: {
            if (state.timer-- < 0 && ryoNotBusy()) {
                SetPlayerControlEnabled(0);
                ResetCounters(21);
            }
            EV_set_debug_text_pos(2, 8);
            Vector3 position{};
            Vector3i rotation{};
            EV_GetCharaPosition(ryo, 0xffffffffU, &position, EventTransformFlags::Local);
            GetScenarioCharacterEulerAngles(ryo, &rotation, EventTransformFlags::Local);
            break;
        }
        default:
            break;
        }
    } else if (state.phase == 19) {
        mtwkSetFrameTime(RyoMotion(), 12, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(RyoMotion(), 1);
        mtwkSetExtraFlag(RyoMotion(), 15, 0);
        mtwkRequestMotion(RyoMotion(), 0xf005, MotionRegion::LowerBody);
        EV_SetCharaControllerFlags(ryo, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(ryo, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_AT_SetPos(ryo, localXYZ, {336.51999f, 127, 262.32999f});
        (void)EV_AT_SetAngY(ryo, localXYZ, {15, 15, 15});
        authorFlags = 2;
        sceneArea = SHEN_NAME(WB01);
        sceneEntry = 1;
        auto *task = Queue(WbsrSceneChangeCallback);
        if (authorFlags & 2) {
            Fade(30, 0, 255);
            HLib::SetupTimedTask(task, 30);
        }
        HLib::DestroyCurrentTask();
    } else if (state.phase == 21) {
        if (!state.counter) {
            Fade(60, 0, 255);
            HLib::SetupTaskDelay(60);
            ClearGameFlag(31);
            ++state.counter;
        } else if (state.counter == 1) {
            ClearGameFlag(30);
            EV_EnableCharacterDisplay(sai);
            SetAuthor(31);
            nextCallback = ExhaustedAuthorDone;
            authorFlags = 1;
            Fade(30, 255, 0);
            StartAuthor(SHEN_NAME(AUTB), ExhaustedAuthorDone, 1, true);
            EV_stepSetFlags(0, 0x2bb);
            ExecuteBase();
            ResetCounters(999);
        }
    }
}
// @verified - p
void WbsrSelect1Callback(void *) {
    if (EV_GetShenmueFlag(0x2ba)) {
        SetPlayerControlEnabled(0);
        ClearGameFlag(30);
        shenSetClockAndCompassRenderTo(0);
        (void)shenSetTimeSimulationTo(1);
        EV_EnableCharacterDisplay(a048);
        SetAuthor(30);
        StartAuthor(SHEN_NAME(AUT2), QueueScenarioA, 0, true);
        HLib::DestroyCurrentTask();
    }
    if (EV_GetShenmueFlag(0x2bb)) {
        SetPlayerControlEnabled(0);
        ClearGameFlag(30);
        EV_stepSetFlags(0, 0x2bb);
        ExecuteBase();
        shenSetClockAndCompassRenderTo(0);
        (void)shenSetTimeSimulationTo(1);
        mtwkSetExtraFlag(RyoMotion(), 55, 0);
        mtwkSetExtraFlag(RyoMotion(), 17, 0);
        HLib::SetCurrentTaskTo(WbsrSelect2Callback);
        HLib::SetupTaskDelay(15);
    }
}
// @verified - p
void WbsrSelect2Callback(void *) {
    EV_EnableCharacterDisplay(a048);
    SetAuthor(32);
    StartAuthor(SHEN_NAME(AUT1), RefusedAuthorDone, 0, true);
    HLib::DestroyCurrentTask();
}
// @verified - p
void WbsrExit2Callback(void *) {
    SetPlayerControlEnabled(0);
    ClearGameFlag(32);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
    plymAdvanceTime(23, 0);
    HLib::SetCurrentTaskTo(WbsrExit3Callback);
}
// @verified - p
void WbsrExit3Callback(void *) {
    const auto held = plymGetAreaExitState();
    if (!held)
        HLib::SetCurrentTaskTo(WbsrExit4Callback);
    else if (held == 1) {
        (void)EV_PlayDialogue("0401A002");
        HLib::SetCurrentTaskTo(WbsrExit4Callback);
        HLib::SetupTaskDelay(40);
    }
}
// @verified - p
void WbsrExit4Callback(void *) {
    ClearGameFlag(30);
    SetPlayerControlEnabled(1);
    (void)EV_FreeAuthorPak(0, "0031");
    if (package)
        (void)EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    if (characterList)
        for (const auto *p = characterList; p->name; ++p)
            (void)EV_npcLockUnlock(*p);
    EV_SetCanDoAreaJumpTo(1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
    (void)EV_NPC_NewLoadSw(1);
    HLib::DestroyCurrentTask();
}
// @verified - p
void WbsrSceneChangeCallback(void *) {
    if (!(authorFlags & 4))
        (void)scenSetTaskFlags(6);
    (void)EV_scenQueueLoad(g_actNumber, sceneArea, sceneEntry);
    HLib::DestroyCurrentTask();
}
const ScriptAreaDefinition wbsrScriptDefinition{.areaId = AREA_WBSR, .field04 = 0, .init = WbsrInit, .main = WbsrMain};
} // namespace shenmue::s2
#endif
