#include "s2/event_library_tail_dc.h"

#if defined(SHENMUE_EVENT_LIBRARY_TAIL_LOG)
void EventDebugLog(const char *format, ...)
{
    char text[256];
    va_list arguments;
    va_start(arguments, format);
    vsprintf(text, format, arguments);
    FTBL.DebugLogToFileRemoved(text);
}

#endif

void EventDebugPrintf(int column, int line, const char *format, ...)
{
    char text[256];
    va_list arguments;
    va_start(arguments, format);
    vsprintf(text, format, arguments);
    FTBL.SetDebugScreenTextPosition(column, line);
    FTBL.DebugLogToScreen(text);
}

void EventMessageCallback(void *data)
{
    FTBL.dbgPrintEventMessage(((EventMessageWork *)data)->text);
    FTBL.HLib_SetCurrentTaskTo(EventMessageWaitCallback);
}

void EventMessageWaitCallback(void *data)
{
    EventMessageWork *work = (EventMessageWork *)data;
    if (--work->frames == 0) {
        FTBL.ctrlStopSubtitles();
        FTBL.HLib_DestroyCurrentTask();
    }
}

void QueueEventMessage(const char *text, int delay, int frames)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventMessageCallback, 6, 11, sizeof(EventMessageWork), TASK_EVNT);
    EventMessageWork *work;
    if (delay)
        FTBL.HLib_SetupTimedTask(task, delay);
    work = (EventMessageWork *)FTBL.HLib_GetTaskParameter(task);
    work->text = text;
    work->frames = frames;
}

void EventSceneLoadCallback(void *data)
{
    EventSceneLoadWork *work = (EventSceneLoadWork *)data;
    FTBL.scenSetTaskFlags(6);
    FTBL.EV_scenQueueLoad(g_actNumber, work->scene, work->entry);
}

void QueueEventSceneLoad(shen_name scene, int entry, int delay)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventSceneLoadCallback, 6, 11, sizeof(EventSceneLoadWork), TASK_EVNT);
    EventSceneLoadWork *work;
    if (delay)
        FTBL.HLib_SetupTimedTask(task, delay);
    work = (EventSceneLoadWork *)FTBL.HLib_GetTaskParameter(task);
    work->scene = scene;
    work->entry = entry;
}
