#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

typedef struct EmptyAreaWork {
    int state;
    HLibTask *task;
} EmptyAreaWork;

void UpdateEmptyArea(void *taskData);
void ShowEmptyAreaMessage(const char *text);
void ClearEmptyAreaMessage(void *taskData);

void StartEmptyArea(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(UpdateEmptyArea, 4, 11, sizeof(EmptyAreaWork), TASK_EVNT);
    EmptyAreaWork *work = (EmptyAreaWork *)FTBL.HLib_GetTaskParameter(task);
    work->state = 0;
    work->task = task;
    FTBL.SetPlayerControlEnabled(1);
    FTBL.FinishLoading();
}

void UpdateEmptyArea(void *taskData)
{
    if ((FTBL.EVT_GetControllerInput(0, 0) >> 10) & 1) {
        FTBL.HLib_DestroyCurrentTask();
    } else {
        ShowEmptyAreaMessage("やめちゃいやーん いや いやーん");
        FTBL.HLib_SetupTaskDelay(100);
    }
}

void ShowEmptyAreaMessage(const char *text)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithoutParameter(ClearEmptyAreaMessage, 4, 11, TASK_EVNT);
    FTBL.dbgPrintEventMessage(text);
    FTBL.HLib_SetupTimedTask(task, 40);
}

void ClearEmptyAreaMessage(void *taskData)
{
    FTBL.ctrlStopSubtitles();
    FTBL.HLib_DestroyCurrentTask();
}
#endif
