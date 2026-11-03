#include "s2/event_sleep_dc.h"
#include "s2/event_queued_dc.h"

#define SLEEP_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define SLEEP_BUTTON SHEN_NAME4('B', 'T', 'S', 'P')
#define SLEEP_IS_CHARACTER(name) (((name) & 0xff000000U) == 0x5f000000U)
#if SHENMUE_EVENT_SLEEP_REVISION >= 3
#define SLEEP_ACTOR_PENDING(i) (SLEEP_IS_CHARACTER(sleepEventData.actors[i]) && (sleepEventData.pendingResources & 2 << (i)))
#else
#define SLEEP_ACTOR_PENDING(i) (sleepEventData.pendingResources & 2 << (i))
#endif

const int sleepEventReserved = 0;
SleepEventData sleepEventData = {{0}};

void SleepCopyString(char *destination, const char *source)
{
    if (source)
        while (*source)
            *destination++ = *source++;
    *destination = 0;
}

void SleepCopyNames(shen_name *destination, const shen_name *source)
{
    if (source)
        while (*source)
            *destination++ = *source++;
    *destination = 0;
}

void AdvanceSleepToMorning(void)
{
    EventDateTime time;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    if (time.hour > 7)
        time.day++;
    time.hour = 8;
    time.minute = 0;
#if SHENMUE_EVENT_SLEEP_REVISION >= 2
    time.second = 0;
#endif
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    FTBL.EV_stepSetFlags(100, 0);
    FTBL.EV_stepSetFlags(303, 0);
    if (FTBL.EV_GetShenmueFlag(1072)) {
        time.hour = 7;
        time.minute = 45;
#if SHENMUE_EVENT_SLEEP_REVISION >= 2
        time.second = 0;
#endif
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    }
    FTBL.EV_setHoldStatesFlags(2);
}

void QueueSleepCallback(SleepContinuation accepted, SleepContinuation cancelled, int expectedSchedule, unsigned int flags)
{
    EventDateTime time;
    int icon;
    sleepEventData.continuation = accepted;
    sleepEventData.cancelled = cancelled;
    sleepEventData.flags = flags;
#if SHENMUE_EVENT_SLEEP_REVISION < 2
    FTBL.SetRenderClockAndCompassTo(1);
#endif
    FTBL.GetCurrentDateTime((DateTime *)&time);
    if (FTBL.shenGetMinutesUntilNextFrameMinute()) {
        if (time.hour < 7 || time.hour > 19)
            icon = 1;
        else
            icon = 3;
    } else
        icon = 2;
    if (expectedSchedule && expectedSchedule != FTBL.shenGetSleepScheduleFlag(0))
        icon = 3;
#if SHENMUE_EVENT_SLEEP_REVISION >= 2
    sleepEventData.icon = icon;
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.HLib_SetCurrentTaskTo(SleepIconCallback);
}

void SleepIconCallback(void *taskData)
{
    if (FTBL.GetLoadingPhaseFlags() & 0x100)
        return;
    FTBL.EV_SetPlayerFlag(12, 1022, 1);
    FTBL.shenSetTimeSimulationTo(0);
    switch (sleepEventData.icon) {
#else
    FTBL.EV_SetPlayerFlag(12, 1022, 1);
    switch (icon) {
#endif
    case 2:
        FTBL.SetSleepActionIcon(15);
        break;
    case 1:
        FTBL.SetSleepActionIcon(271);
        break;
    default:
        FTBL.SetSleepActionIcon(266);
        break;
    }
#if SHENMUE_EVENT_SLEEP_REVISION < 2
    FTBL.shenSetTimeSimulationTo(0);
#endif
    FTBL.HLib_SetCurrentTaskTo(SleepSelectionCallback);
}

void FinishSleepSelection(void)
{
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.shenSetTimeSimulationTo(1);
    if (sleepEventData.flags & 2) {
        FTBL.spmFadeOut(0);
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetupTaskDelay(32);
    }
}

void SleepSelectionCallback(void *taskData)
{
    int result = FTBL.GetSleepActionIconResult();
    if (result == 2) {
        FTBL.EV_SetPlayerFlag(12, 1022, 0);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)sleepEventData.continuation);
        FinishSleepSelection();
    } else if (result == 1) {
        FTBL.EV_SetPlayerFlag(12, 1022, 0);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)sleepEventData.cancelled);
        FTBL.SetRenderClockAndCompassTo(0);
        FTBL.shenSetTimeSimulationTo(1);
    }
}

void QueueRandomSleepAuthor(const shen_name *authors, int count, SleepContinuation continuation, unsigned int flags)
{
    int author;
    sleepEventData.continuation = continuation;
    sleepEventData.flags = flags;
    author = authors[FTBL.shenGetRandomInt32(count)];
    FTBL.DebugLogToFileRemoved("Author:%c%c%c%c\n", author, author >> 8, author >> 16, author >> 24);
    if (sleepEventData.flags & 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!(sleepEventData.flags & 64)) {
        sleepEventData.savedFilmBars = FTBL.EV_authSceneCtrl(-9);
        FTBL.EV_authSceneCtrl(-8);
    }
    if (sleepEventData.flags & 32)
        FTBL.EV_authSceneCtrl(-11);
    else if (sleepEventData.flags & 128)
        FTBL.EV_authSceneCtrl(-10);
    else
        FTBL.EV_authSceneCtrl(-16);
    FTBL.EV_authSceneCtrl(author);
    FTBL.HLib_SetCurrentTaskTo(SleepAuthorWaitCallback);
}

void SleepAuthorWaitCallback(void *taskData)
{
    int frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == FTBL.EV_authSceneCtrl(-5) - 31 && (sleepEventData.flags & 2))
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        int i;
        for (i = 0; sleepEventData.actors[i]; i++)
            HideSleepActor(sleepEventData.actors[i]);
        FTBL.EV_authSceneCtrl(-10);
#if SHENMUE_EVENT_SLEEP_REVISION >= 3
        if (sleepEventData.flags & 256)
            FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
#endif
        if (!(sleepEventData.flags & 64))
            FTBL.EV_authSceneCtrl(-7);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)sleepEventData.continuation);
    }
}

void SelectBadEndingState(SleepContinuation continuation)
{
    sleepEventData.continuation = continuation;
    sleepEventData.subtitleFrames = 200;
    AdvanceSleepToMorning();
    switch (GetBadEndingState()) {
    case 1:
        FTBL.HLib_SetCurrentTaskTo(SleepBadEndNoticeCallback);
        FTBL.HLib_SetupTaskDelay(60);
        break;
    case 2:
        FTBL.HLib_SetCurrentTaskTo(SleepBadEndCallback);
        break;
    default:
        FTBL.HLib_SetCurrentTaskTo(SleepSubtitleStartCallback);
        FTBL.HLib_SetupTaskDelay(60);
        break;
    }
}

void SleepBadEndCallback(void *taskData)
{
    FTBL.scenSetTaskFlags(8);
    FTBL.EV_scenQueueLoad(g_actNumber, SHEN_NAME4('0', '5', '4', '9'), 1);
}

void SleepBadEndNoticeCallback(void *taskData)
{
    FTBL.EV_FreeAuthorPak(1, "0549");
    SleepCopyString(sleepEventData.package + 16, FTBL.GetCurrentBgm());
    FTBL.EV_LoadCutsceneFromCurrentScene("0549A000");
    FTBL.EV_authSceneCtrl(-10);
    FTBL.EV_authSceneCtrl(SHEN_NAME4('N', 'B', 'A', 'D'));
    FTBL.HLib_SetCurrentTaskTo(SleepNoticeWaitCallback);
}

void SleepNoticeWaitCallback(void *taskData)
{
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.EV_LoadCutsceneFromCurrentScene(sleepEventData.package + 16);
        FTBL.EV_FreeAuthorPak(0, "0549");
        FTBL.HLib_SetCurrentTaskTo(SleepSubtitleStartCallback);
    }
}

void SleepSubtitleStartCallback(void *taskData)
{
    shen_name title = 0;
    if (g_currentMap == SHEN_NAME4('A', 'R', '0', '1'))
        title = SHEN_NAME4('A', 'R', 'K', '0');
    if (g_currentMap == SHEN_NAME4('W', 'S', 'Y', '0'))
        title = SHEN_NAME4('W', 'S', 'Y', '1');
    if (g_currentMap == SHEN_NAME4('W', 'T', 'A', '0'))
        title = SHEN_NAME4('W', 'T', 'S', 'H');
    if (g_currentMap == SHEN_NAME4('A', 'B', '0', '0'))
        title = SHEN_NAME4('A', 'B', 'B', '0');
    if (title)
        FTBL.Queue_ST_LoadTitleFadeIn(title);
    FTBL.Queue_ST_LoadTitleFadeIn(0);
    FTBL.HLib_SetCurrentTaskTo(SleepSubtitleWaitCallback);
}

void SleepSubtitleWaitCallback(void *taskData)
{
    if (--sleepEventData.subtitleFrames == 31)
        FTBL.ST_LoadTitleFade();
    if (sleepEventData.subtitleFrames == 120)
        FTBL.EV_spmBeginSampleExecute(0);
    if (!sleepEventData.subtitleFrames)
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)sleepEventData.continuation);
}

void QueueSleepMemo(SleepContinuation continuation, unsigned int flags)
{
    sleepEventData.flags = flags;
    sleepEventData.continuation = continuation;
    if (flags & 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.RefreshPadTaskData(0);
    FTBL.HLib_SetCurrentTaskTo(SleepMemoWaitCallback);
}

void SleepMemoWaitCallback(void *taskData)
{
    if (!FTBL.IsItemSelectable())
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)sleepEventData.continuation);
}

void QueueSleepPackageActors(const char *package, const shen_name *actors, SleepContinuation continuation)
{
    int i;
    sleepEventData.continuation = continuation;
    sleepEventData.pendingResources = 0;
    SleepCopyString(sleepEventData.package, package);
    SleepCopyNames(sleepEventData.actors, actors);
    for (i = 0; sleepEventData.actors[i]; i++) {
#if SHENMUE_EVENT_SLEEP_REVISION >= 3
        if (!SLEEP_IS_CHARACTER(sleepEventData.actors[i]))
            continue;
#endif
        sleepEventData.pendingResources |= 2 << i;
        FTBL.CheckWarmStartNpcRoute(sleepEventData.actors[i]);
    }
    if (sleepEventData.package[0]) {
        sleepEventData.pendingResources |= 1;
        FTBL.HLib_SetCurrentTaskTo(SleepPackageRetryCallback);
    } else
        FTBL.HLib_SetCurrentTaskTo(SleepPackageWaitCallback);
}

void SleepPackageRetryCallback(void *taskData)
{
    if (FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)sleepEventData.package, 0)) {
        FTBL.DebugLogToFileRemoved("sl:PakLoad(%d)\n", g_frameCounter);
        FTBL.HLib_SetCurrentTaskTo(SleepPackageWaitCallback);
    } else
        FTBL.DebugLogToFileRemoved("sl:PakWait(%d)\n", g_frameCounter);
}

void SleepPackageWaitCallback(void *taskData)
{
    int i;
    if ((sleepEventData.pendingResources & 1) && !FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0))
        sleepEventData.pendingResources &= ~1;
    for (i = 0; sleepEventData.actors[i]; i++)
        if (SLEEP_ACTOR_PENDING(i) && FTBL.ForceLoadNpc(sleepEventData.actors[i]))
            sleepEventData.pendingResources &= ~(2 << i);
    if (!sleepEventData.pendingResources) {
        for (i = 0; sleepEventData.actors[i]; i++)
            HideSleepActor(sleepEventData.actors[i]);
        HideSleepActor(SLEEP_RYO);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)sleepEventData.continuation);
    }
}

void FreeSleepPackageActors(void)
{
    int i;
    if (sleepEventData.package[0])
        FTBL.EV_ControlPackageResources(0, 6, sleepEventData.package, 0, 0);
    for (i = 0; sleepEventData.actors[i]; i++) {
#if SHENMUE_EVENT_SLEEP_REVISION >= 3
        if (!SLEEP_IS_CHARACTER(sleepEventData.actors[i]))
            continue;
#endif
        FTBL.npcFree(sleepEventData.actors[i]);
        FTBL.npcLockUnlock(sleepEventData.actors[i]);
    }
}

void DisableSleepControl(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_setHoldStatesFlags(0);
}

void EnableSleepControl(void)
{
    FTBL.SetPlayerControlEnabled(1);
    FTBL.EV_setHoldStatesFlags(1);
}

void QueueSleepSceneChange(shen_name scene, int entry, unsigned int flags)
{
    HLibTask *task;
    sleepEventData.flags = flags;
    sleepEventData.nextScene = scene;
    sleepEventData.nextEntry = entry;
    task = FTBL.HLib_EnqueueTaskWithoutParameter(SleepSceneChangeCallback, 4, 11, SLEEP_EVENT_TASK);
    if (sleepEventData.flags & 2) {
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetupTimedTask(task, 31);
    }
}

void SleepSceneChangeCallback(void *taskData)
{
    FTBL.EV_scenQueueLoad(g_actNumber, sleepEventData.nextScene, sleepEventData.nextEntry);
    FTBL.HLib_DestroyCurrentTask();
}

void HideSleepActor(shen_name actor)
{
    Vector3 position;
    position.x = -1000.0f;
    position.y = -1000.0f;
    position.z = -1000.0f;
#if SHENMUE_EVENT_SLEEP_REVISION >= 3
    if (SLEEP_IS_CHARACTER(actor)) {
        FTBL.EV_SetCharacterAlphaEnabled(actor, 0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(actor), 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(actor), 71, 0, 0);
    }
    FTBL.EV_SetCharaControllerFlags(actor, 1, 0);
    FTBL.EV_SetCharaControllerFlags(actor, 2, 0);
    FTBL.EV_SetCharaControllerFlags(actor, 3, 0);
#else
    FTBL.EV_SetCharaControllerFlags(actor, 1, 0);
    FTBL.EV_SetCharacterAlphaEnabled(actor, 0);
    FTBL.EV_SetCharaControllerFlags(actor, 2, 0);
    FTBL.EV_SetCharaControllerFlags(actor, 3, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(actor), 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(actor), 71, 0, 0);
#endif
    FTBL.EV_AT_SetPos(actor, 0x38000000, &position);
}

unsigned int shenTrySleepTrigger(SleepTriggerBox *box)
{
    unsigned int result = 0;
    EventDateTime date;
    Vector3 position;
    Vector3i rotation;
    FTBL.GetCurrentDateTime((DateTime *)&date);
    FTBL.GetCharaPosition(SLEEP_RYO, -1, &position, 0);
    FTBL.GetScenarioCharacterEulerAngles(SLEEP_RYO, &rotation, 0);
    if (box->minimum.y < position.y && position.y < box->maximum.y) {
        if (FTBL.ryoNotBusy() && (date.hour < 7 || date.hour > 19)) {
            float minimum[3] = {box->minimum.x - 0.5f, box->minimum.y - 0.5f, box->minimum.z - 0.5f};
            float maximum[3] = {box->maximum.x + 0.5f, box->maximum.y + 0.5f, box->maximum.z + 0.5f};
            if (!FTBL.TestCharaPositionWithinRange(SLEEP_RYO, (Vector3 *)minimum, (Vector3 *)maximum, 0))
                box->state &= ~1;
            if (FTBL.TestCharaPositionWithinRange(SLEEP_RYO, &box->minimum, &box->maximum, 0) && (box->state & 1) == 0) {
                box->state |= 1;
                result = 1;
                if (!FTBL.shenGetMinutesUntilNextFrameMinute())
                    result = 2;
            }
        }
        if (FTBL.TestCharaPositionWithinRange(SLEEP_RYO, &box->minimum, &box->maximum, 0) && FTBL.ryoNotBusy()) {
            FTBL.GetBattleDataConfig(0, SLEEP_BUTTON);
            if (FTBL.GetBattleDataConfig(2, SLEEP_BUTTON))
                result = 3;
        }
    }
    return result;
}

void QueueSleepPromptTask(SleepPromptCallback callback)
{
    EventDateTime time;
    sleepEventData.prompt = callback;
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.GetCurrentDateTime((DateTime *)&time);
    if (!FTBL.shenGetMinutesUntilNextFrameMinute())
        FTBL.SetSleepActionIcon(15);
    else if (time.hour < 7 || time.hour > 19)
        FTBL.SetSleepActionIcon(271);
    else
        FTBL.SetSleepActionIcon(266);
    FTBL.shenSetTimeSimulationTo(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(SleepPromptTaskCallback, 4, 11, SLEEP_EVENT_TASK);
}

void SleepPromptTaskCallback(void *taskData)
{
    int result = FTBL.GetSleepActionIconResult();
    if (result == 2) {
        FTBL.SetRenderClockAndCompassTo(0);
        FTBL.shenSetTimeSimulationTo(1);
        if (sleepEventData.prompt)
            sleepEventData.prompt(1);
        FTBL.HLib_DestroyCurrentTask();
    } else if (result == 1) {
        FTBL.SetRenderClockAndCompassTo(0);
        FTBL.shenSetTimeSimulationTo(1);
        if (sleepEventData.prompt)
            sleepEventData.prompt(0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void QueueRandomSleepAuthorTask(const shen_name *authors, int count, SleepContinuation continuation, unsigned int flags)
{
    int author;
    sleepEventData.continuation = continuation;
    sleepEventData.flags = flags;
    author = authors[FTBL.shenGetRandomInt32(count)];
    FTBL.DebugLogToFileRemoved("Author:%c%c%c%c\n", author, author >> 8, author >> 16, author >> 24);
    if (sleepEventData.flags & 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (sleepEventData.flags & 32)
        FTBL.EV_authSceneCtrl(-11);
    if (!(sleepEventData.flags & 64)) {
        sleepEventData.savedFilmBars = FTBL.EV_authSceneCtrl(-9);
        FTBL.EV_authSceneCtrl(-8);
    }
    FTBL.EV_authSceneCtrl(author);
    FTBL.HLib_EnqueueTaskWithoutParameter(SleepAuthorTaskCallback, 4, 11, SLEEP_EVENT_TASK);
}

void SleepAuthorTaskCallback(void *taskData)
{
    int frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == FTBL.EV_authSceneCtrl(-5) - 31 && (sleepEventData.flags & 2))
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        int i;
        for (i = 0; sleepEventData.actors[i]; i++)
            HideSleepActor(sleepEventData.actors[i]);
        if (sleepEventData.flags & 32)
            FTBL.EV_authSceneCtrl(-10);
        if (!(sleepEventData.flags & 64))
            FTBL.EV_authSceneCtrl(-7);
        if (sleepEventData.continuation)
            sleepEventData.continuation();
        FTBL.HLib_DestroyCurrentTask();
    }
}

void QueueSleepSubtitleTask(SleepContinuation continuation)
{
    sleepEventData.continuation = continuation;
    sleepEventData.subtitleFrames = 200;
    AdvanceSleepToMorning();
    FTBL.HLib_SetupTimedTask(FTBL.HLib_EnqueueTaskWithoutParameter(SleepSubtitleTaskStartCallback, 4, 11, SLEEP_EVENT_TASK), 60);
}

void SleepSubtitleTaskStartCallback(void *taskData)
{
    FTBL.Queue_ST_LoadTitleFadeIn(0);
    FTBL.HLib_SetCurrentTaskTo(SleepSubtitleTaskWaitCallback);
}

void SleepSubtitleTaskWaitCallback(void *taskData)
{
    if (--sleepEventData.subtitleFrames == 31)
        FTBL.ST_LoadTitleFade();
    if (!sleepEventData.subtitleFrames) {
        if (sleepEventData.continuation)
            sleepEventData.continuation();
        FTBL.HLib_DestroyCurrentTask();
    }
}

void QueueSleepMemoTask(SleepContinuation continuation, unsigned int flags)
{
    sleepEventData.flags = flags;
    sleepEventData.continuation = continuation;
    if (flags & 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.RefreshPadTaskData(0);
    FTBL.HLib_EnqueueTaskWithoutParameter(SleepMemoTaskCallback, 4, 11, SLEEP_EVENT_TASK);
}

void SleepMemoTaskCallback(void *taskData)
{
    if (!FTBL.IsItemSelectable()) {
        if (sleepEventData.continuation)
            sleepEventData.continuation();
        FTBL.HLib_DestroyCurrentTask();
    }
}

void QueueSleepPackageActorsTask(const char *package, const shen_name *actors, SleepContinuation continuation)
{
    int i;
    sleepEventData.continuation = continuation;
    sleepEventData.pendingResources = 0;
    SleepCopyString(sleepEventData.package, package);
    SleepCopyNames(sleepEventData.actors, actors);
    for (i = 0; sleepEventData.actors[i]; i++) {
#if SHENMUE_EVENT_SLEEP_REVISION >= 3
        if (!SLEEP_IS_CHARACTER(sleepEventData.actors[i]))
            continue;
#endif
        sleepEventData.pendingResources |= 2 << i;
        FTBL.CheckWarmStartNpcRoute(sleepEventData.actors[i]);
        FTBL.ForceLoadNpc(sleepEventData.actors[i]);
    }
    if (sleepEventData.package[0]) {
        sleepEventData.pendingResources |= 1;
        FTBL.HLib_EnqueueTaskWithoutParameter(SleepPackageTaskRetryCallback, 4, 11, SLEEP_EVENT_TASK);
    } else
        FTBL.HLib_EnqueueTaskWithoutParameter(SleepPackageTaskWaitCallback, 4, 11, SLEEP_EVENT_TASK);
}

void SleepPackageTaskRetryCallback(void *taskData)
{
    if (FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)sleepEventData.package, 0))
        FTBL.HLib_SetCurrentTaskTo(SleepPackageTaskWaitCallback);
    else
        FTBL.DebugLogToFileRemoved("sl:PakWait(%d)\n", g_frameCounter);
}

void SleepPackageTaskWaitCallback(void *taskData)
{
    int i;
    if ((sleepEventData.pendingResources & 1) && !FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0))
        sleepEventData.pendingResources &= ~1;
    for (i = 0; sleepEventData.actors[i]; i++)
        if (SLEEP_ACTOR_PENDING(i) && FTBL.ForceLoadNpc(sleepEventData.actors[i]))
            sleepEventData.pendingResources &= ~(2 << i);
    if (!sleepEventData.pendingResources) {
        for (i = 0; sleepEventData.actors[i]; i++)
            HideSleepActor(sleepEventData.actors[i]);
        HideSleepActor(SLEEP_RYO);
        if (sleepEventData.continuation)
            sleepEventData.continuation();
        FTBL.HLib_DestroyCurrentTask();
    }
}

int GetBadEndingState(void)
{
    EventDateTime date;
    FTBL.GetCurrentDateTime((DateTime *)&date);
    if (date.month >= 7 && FTBL.EV_GetShenmueFlag(929)) {
        FTBL.EV_stepSetFlags(929, 0);
        return 1;
    }
    if (date.month >= 8 && FTBL.EV_GetShenmueFlag(930))
        return 2;
    return 0;
}
