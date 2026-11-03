
#include "s2/sk_event_dc.h"

void SkVoiceListInit(SkVoiceList *list, const char *prefix)
{
    list->index = 0;
    list->count = 0;
    list->prefix = prefix;
}

void SkVoiceListAdd(SkVoiceList *list, const char *voice, int delay)
{
    if (list->count < SK_VOICE_LIST_CAPACITY) {
        list->voices[list->count] = voice;
        list->delays[list->count] = delay;
        list->count++;
    }
}

void QueueSkVoiceList(SkVoiceList *list)
{
    HLibTask *task;
    SkVoiceList *work;
    int i;
    task = FTBL.HLib_EnqueueTaskWithParameter(SkVoiceList_Callback, 6, 11, sizeof(SkVoiceList), SK_EVENT_TASK);
    work = (SkVoiceList *)FTBL.HLib_GetTaskParameter(task);
    work->prefix = list->prefix;
    work->count = list->count;
    work->parent = FTBL.HLib_GetCurrentTask();
    work->index = 0;
    work->state = 0;
    list->count = 0;
    for (i = 0; i < SK_VOICE_LIST_CAPACITY; i++) {
        work->voices[i] = list->voices[i];
        work->delays[i] = list->delays[i];
    }
    FTBL.HLib_SetupTimedTask(work->parent, 0);
}

void SkVoiceList_Callback(void *data)
{
    SkVoiceList *work = (SkVoiceList *)data;
    char voice[12];
    switch (work->state) {
    case 0:
        if (work->index == work->count) {
            work->count = 0;
            FTBL.HLib_ResolveDeferredCallback(work->parent);
            FTBL.HLib_DestroyCurrentTask();
        } else {
            strcpy(voice, work->prefix);
            strcat(voice, work->voices[work->index]);
            work->conversation = FTBL.EV_PlayDialogue(voice);
            work->state++;
        }
        break;
    case 1:
        if (!FTBL.cvsHasValidConverseData(work->conversation)) {
            work->state = 0;
            if (work->delays[work->index] > 0)
                FTBL.HLib_SetupTaskDelay(work->delays[work->index]);
            work->index++;
        }
        break;
    }
}

void QueueSkSimpleQteWait(int *result, unsigned int button, int referenceFrames, int maximumFrames, HLibTaskCallback success, HLibTaskCallback failure,
    int delay)
{
    HLibTask *task;
    SkSimpleQteWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(SkSimpleQteWait_Callback, 6, 11, sizeof(SkSimpleQteWork), SK_EVENT_TASK);
    work = (SkSimpleQteWork *)FTBL.HLib_GetTaskParameter(task);
    result[0] = 0;
    work->task = task;
    work->success = success;
    work->failure = failure;
    work->button = button;
    work->maximumFrames = maximumFrames;
    work->referenceFrames = referenceFrames;
    work->elapsed = result;
    work->status = (unsigned short *)(result + 2);
    work->phase = 0;
    work->parent = FTBL.HLib_GetCurrentTask();
    FTBL.HLib_SetupTimedTask(work->parent, 0);
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(work->task, delay);
}

void SkSimpleQteWait_Callback(void *data)
{
    SkSimpleQteWork *work = (SkSimpleQteWork *)data;
    int remaining;
    unsigned short status;
    if (!work->phase) {
        FTBL.SetShowQteFlagForButtonType(work->button, work->maximumFrames, 2);
        work->phase++;
    } else {
        (*work->elapsed)++;
        if (!FTBL.GetShouldRenderIcons(3)) {
            *work->status = status = FTBL.GetCurrentQteStatus();
            if ((status & 0xfff) == work->button) {
                if (work->success)
                    FTBL.HLib_AssignCallbackForTask(work->parent, work->success);
            } else if (work->failure)
                FTBL.HLib_AssignCallbackForTask(work->parent, work->failure);
            remaining = work->referenceFrames - *work->elapsed;
            if (remaining > 0)
                FTBL.HLib_SetupTimedTask(work->parent, remaining);
            else
                FTBL.HLib_ResolveDeferredCallback(work->parent);
            FTBL.HLib_DestroyTaskInternal(work->task);
        }
    }
}

void QueueSkChoiceWait(HLibTask *parent, int first, int second, int third, int fourth, int defaultChoice, int timeout, int delay)
{
    HLibTask *task;
    SkChoiceWaitWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(SkChoiceWaitCallback, 6, 11, sizeof(SkChoiceWaitWork), SK_EVENT_TASK);
    work = (SkChoiceWaitWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->parent = parent;
    work->timeout = timeout;
    work->phase = 0;
    work->choices[0] = first;
    work->choices[3] = fourth;
    work->choices[1] = second;
    work->choices[2] = third;
    work->defaultChoice = defaultChoice;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(work->task, delay);
    FTBL.HLib_SetupTimedTask(work->parent, 0);
}

void SkChoiceWaitCallback(void *data)
{
    SkChoiceWaitWork *work = (SkChoiceWaitWork *)data;
    if (!work->phase) {
        if (work->defaultChoice != -1)
            FTBL.SetupControlSpeakAndUI(work->choices, work->timeout, work->defaultChoice, 2);
        else
            FTBL.SetupControlSpeakAndUI(work->choices, work->timeout, 0xffff, 0);
        work->phase++;
    } else if (!FTBL.GetShouldRenderIcons(2)) {
        FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyTaskInternal(work->task);
    }
}

void QueueSkMoneyIconWait(HLibTask *parent, short maximum, short step, int delay)
{
    HLibTask *task;
    SkMoneyIconWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(SkMoneyIcon_Callback, 6, 11, sizeof(SkMoneyIconWork), SK_EVENT_TASK);
    work = (SkMoneyIconWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->parent = parent;
    work->maximum = maximum;
    work->step = step;
    work->phase = 0;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(work->task, delay);
    FTBL.HLib_SetupTimedTask(work->parent, 0);
}

void SkMoneyIcon_Callback(void *data)
{
    SkMoneyIconWork *work = (SkMoneyIconWork *)data;
    if (!work->phase) {
        FTBL.SetMoneyRelatedUIElementAlternative(work->maximum, 32767, work->step);
        work->phase++;
    } else if (!FTBL.GetShouldRenderIcons(4)) {
        FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyTaskInternal(work->task);
    }
}

void QueueSkAddMoney(HLibTask *parent, int amount, int delay)
{
    HLibTask *task;
    SkAddMoneyWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(SkAddMoney_Callback, 6, 11, sizeof(SkAddMoneyWork), SK_EVENT_TASK);
    work = (SkAddMoneyWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->parent = parent;
    work->amount = amount;
    work->phase = 0;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(work->task, delay);
    FTBL.HLib_SetupTimedTask(work->parent, 0);
}

void SkAddMoney_Callback(void *data)
{
    SkAddMoneyWork *work = (SkAddMoneyWork *)data;
    if (!work->phase) {
        FTBL.AddThisToCashOrSegaCoins(work->amount, 2);
        work->phase++;
    } else if (!FTBL.GetShouldRenderIcons(4)) {
        FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyTaskInternal(work->task);
    }
}

void QueueSkAuthorStart(HLibTask *parent, int command, HLibTaskCallback callback, void *callbackData, int delay)
{
    HLibTask *task;
    SkAuthorStartWork *work;
#if SHENMUE_EVENT_SK_REVISION < SK_EVENT_REVISION_ISTB
    task = FTBL.HLib_EnqueueTaskWithParameter(EVT_SK_AuthStart_Callback, 4, 11, sizeof(SkAuthorStartWork), SK_EVENT_TASK);
    work = (SkAuthorStartWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->parent = parent;
    work->command = command;
    work->callback = callback;
    work->callbackData = callbackData;
    work->phase = 0;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(work->task, delay);
#else
    task = FTBL.HLib_EnqueueTaskWithParameter(EVT_SK_AuthStart_Callback, 3, 11, sizeof(SkAuthorStartWork), SK_EVENT_TASK);
    work = (SkAuthorStartWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->parent = parent;
    work->command = command;
    work->callback = callback;
    work->callbackData = callbackData;
    if (delay > 0) {
        work->phase = 0;
        FTBL.HLib_SetupTimedTask(work->task, delay);
    } else {
        work->phase = 1;
        FTBL.EV_authSceneCtrl(work->command);
        if (work->callback)
            work->callback(work->callbackData);
    }
#endif
    FTBL.HLib_SetupTimedTask(work->parent, 0);
}

void EVT_SK_AuthStart_Callback(void *data)
{
    SkAuthorStartWork *work = (SkAuthorStartWork *)data;
#if SHENMUE_EVENT_SK_REVISION < SK_EVENT_REVISION_ISTB
    if (!work->phase) {
        FTBL.EV_authSceneCtrl(work->command);
        work->phase++;
        if (work->callback)
            work->callback(work->callbackData);
    } else if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyTaskInternal(work->task);
    } else if (work->callback)
        work->callback(work->callbackData);
#else
    if (!work->phase) {
        FTBL.EV_authSceneCtrl(work->command);
        work->phase++;
        if (work->callback)
            work->callback(work->callbackData);
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyTaskInternal(work->task);
    } else {
        if (work->callback)
            work->callback(work->callbackData);
        if (!FTBL.EV_authSceneCtrl(-1)) {
            FTBL.HLib_ResolveDeferredCallback(work->parent);
            FTBL.HLib_DestroyTaskInternal(work->task);
        }
    }
#endif
}

void QueueSkNpcWait(HLibTask *parent)
{
    HLibTask *task;
    SkNpcWaitWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(SkNpcWait_Callback, 6, 11, sizeof(SkNpcWaitWork), SK_EVENT_TASK);
    work = (SkNpcWaitWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->parent = parent;
#if SHENMUE_EVENT_SK_REVISION >= SK_EVENT_REVISION_ISTB
    FTBL.DebugLogToFileRemoved("WaitNpc");
#endif
    FTBL.HLib_SetupTimedTask(work->parent, 0);
}

void SkNpcWait_Callback(void *data)
{
    SkNpcWaitWork *work = (SkNpcWaitWork *)data;
#if SHENMUE_EVENT_SK_REVISION >= SK_EVENT_REVISION_ISTB
    FTBL.DebugLogToFileRemoved(".");
#endif
    if (!FTBL.NpcWait()) {
#if SHENMUE_EVENT_SK_REVISION >= SK_EVENT_REVISION_ISTB
        FTBL.DebugLogToFileRemoved("\n");
#endif
        FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyTaskInternal(work->task);
    }
}

#if SHENMUE_EVENT_SK_REVISION >= SK_EVENT_REVISION_ISTB
int scenarioQteRetryCount = 0;

void ResetSkQteRetryCount(void)
{
    scenarioQteRetryCount = 0;
}

int GetSkQteRetryCount(void)
{
    return scenarioQteRetryCount;
}

void IncrementSkQteRetryCount(void)
{
    if (scenarioQteRetryCount < 2)
        scenarioQteRetryCount++;
}
#endif

void QueueSkQteWait(int *result, unsigned int button, int referenceFrames, int baseMaximumFrames, SkQteCallback success, SkQteCallback failure, int delay)
{
    HLibTask *task;
    SkWaitQteWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(EVT_SK_WaitQTE_Callback, 6, 11, sizeof(SkWaitQteWork), SK_EVENT_TASK);
    work = (SkWaitQteWork *)FTBL.HLib_GetTaskParameter(task);
    result[0] = 0;
    result[3] = 0;
    work->task = task;
    work->success = success;
    work->failure = failure;
    work->button = button;
#if SHENMUE_EVENT_SK_REVISION < SK_EVENT_REVISION_ISTB
    work->maximumFrames = baseMaximumFrames;
#else
    work->maximumFrames = baseMaximumFrames + scenarioQteRetryCount * 4;
#endif
    work->referenceFrames = referenceFrames;
    work->elapsed = result;
    work->status = (unsigned short *)(result + 2);
    work->succeeded = result + 3;
    work->phase = 0;
#if SHENMUE_EVENT_SK_REVISION >= SK_EVENT_REVISION_ISTB
    if (delay > 4) {
        delay -= 4;
        work->referenceFrames += 4;
    }
    if (work->maximumFrames > work->referenceFrames - 4)
        work->maximumFrames = work->referenceFrames - 4;
#endif
    work->parent = FTBL.HLib_GetCurrentTask();
    FTBL.HLib_SetupTimedTask(work->parent, 0);
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(work->task, delay);
}

void EVT_SK_WaitQTE_Callback(void *data)
{
    SkWaitQteWork *work = (SkWaitQteWork *)data;
    int delay = 0;
    unsigned int status;
    int mask;
    SkQteCallback callback;
    switch (work->phase) {
    case 0:
        if ((int)work->button < 0)
            FTBL.SetShowQteFlagForButtonType(68, work->maximumFrames, 4);
        else
            FTBL.SetShowQteFlagForButtonType(work->button, work->maximumFrames, 2);
        work->phase++;
        break;
    case 1:
        (*work->elapsed)++;
        if (FTBL.GetShouldRenderIcons(3))
            break;
        status = FTBL.GetCurrentQteStatus();
        *work->status = status & 0xfff;
        if ((int)work->button < 0) {
            if (work->button & 0xc0000000U)
                mask = 64;
            else
                mask = 68;
            if (work->button & 0xa0000000U)
                mask = 4;
            if ((unsigned short)(status & 0xfff) & mask)
                *work->succeeded = 1;
            else
                *work->succeeded = 0;
        } else if ((status & 0xfff) == work->button)
            *work->succeeded = 1;
        else
            *work->succeeded = 0;
        delay = work->referenceFrames - *work->elapsed;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(work->task, delay);
        work->phase++;
        break;
    case 2:
        callback = *work->succeeded ? work->success : work->failure;
        if (callback)
            delay = callback();
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(work->parent, delay);
        else
            FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyTaskInternal(work->task);
        break;
    }
}

static char skSavedBgm[16];
static char skSavedSbt[16];
static int skSavedSoundScene;

void SaveSkSound(void)
{
    skSavedSoundScene = FTBL.GetSelectedSoundSceneIndex();
    strcpy(skSavedSbt, FTBL.shenGetCurrentSbt());
    strcpy(skSavedBgm, FTBL.GetCurrentBgm());
}

void RestoreSkSound(void)
{
    FTBL.EV_LoadCutsceneFromCurrentScene(skSavedBgm);
    FTBL.EV_LoadAudio(skSavedSbt, skSavedSoundScene);
}
