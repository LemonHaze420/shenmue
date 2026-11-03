#include "s2/areas/common/et_abort.h"

static EventAbortWork *eventAbortWorks[EVENT_ABORT_SLOT_COUNT];

void QueueEventAbort(int slot, HLibTaskCallback callback, void *data, int frames)
{
    HLibTask *task;
    EventAbortWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(EventAbortCallback, 4, 11, sizeof(EventAbortWork), TASK_ABRT);
    work = (EventAbortWork *)FTBL.HLib_GetTaskParameter(task);
    eventAbortWorks[slot] = work;
    work->slotName = FTBL.GetAreaSlotName(slot);
    work->task = task;
    work->slot = slot;
    work->callback = callback;
    work->data = data;
    work->frames = frames;
    switch (slot) {
    case 1:
        work->flags = &g_eventSlotStates[1].flags;
        break;
    case 2:
        work->flags = &g_eventSlotStates[2].flags;
        break;
    case 3:
        work->flags = &g_eventSlotStates[3].flags;
        break;
    }
    if (*work->flags & EVENT_ABORT_ACTIVE) {
        FTBL.DebugLogToFileRemoved("-----------------------------------------------------\n\n");
        FTBL.DebugLogToFileRemoved("               Error : (Event Leak)                  \n\n");
        FTBL.DebugLogToFileRemoved("-----------------------------------------------------\n\n");
    }
    *work->flags = EVENT_ABORT_ACTIVE;
}

void CancelEventAbort(int slot)
{
    EventAbortWork *work = eventAbortWorks[slot];
    if (work) {
        FTBL.ctrlStopSubtitles();
        *work->flags = 0;
        FTBL.HLib_DestroyTaskInternal(work->task);
        eventAbortWorks[slot] = 0;
    }
}

void EventAbortCallback(void *data)
{
    EventAbortWork *work = (EventAbortWork *)data;
    int abort;
    if (FTBL.IsFightActive())
        return;
    abort = FTBL.EVT_GetControllerInput(0, 0) == EVENT_ABORT_INPUT;
    abort = abort || (*work->flags & 1);
    abort = abort && (*work->flags & 4);
    if (--work->frames > 0 && !abort)
        return;
    if (abort) {
        FTBL.DebugLogToFileRemoved("-----------------------------------------------------\n\n");
        FTBL.DebugLogToFileRemoved("                     Event Abort                     \n\n");
        FTBL.DebugLogToFileRemoved("-----------------------------------------------------\n\n");
    } else {
        FTBL.DebugLogToFileRemoved("-----------------------------------------------------\n\n");
        FTBL.DebugLogToFileRemoved("                   Event Time Limit                  \n\n");
        FTBL.DebugLogToFileRemoved("-----------------------------------------------------\n\n");
    }
    ClearEventAbortFlags(work->slot, 4);
    if (FTBL.IsFightActive())
        FTBL.check_dead_status(1);
    if (FTBL.EV_authSceneCtrl(-1))
        FTBL.EV_authSceneCtrl(-4);
    work->callback(work->data);
}

void SetEventAbortCallback(int slot, HLibTaskCallback callback, void *data)
{
    EventAbortWork *work = eventAbortWorks[slot];
    if (work) {
        work->callback = callback;
        work->data = data;
    }
}

unsigned int SetEventAbortFlags(int slot, unsigned int mask)
{
    EventAbortWork *work = eventAbortWorks[slot];
    unsigned int flags = 0;
    if (work) {
        flags = *work->flags;
        *work->flags = mask | flags;
    }
    return flags;
}

unsigned int ClearEventAbortFlags(int slot, unsigned int mask)
{
    EventAbortWork *work = eventAbortWorks[slot];
    unsigned int flags = 0;
    if (work) {
        flags = *work->flags;
        *work->flags = ~mask & flags;
    }
    return flags;
}

unsigned int GetEventAbortFlags(int slot)
{
    EventAbortWork *work = eventAbortWorks[slot];
    if (work)
        return *work->flags;
    return 0;
}

int SetEventAbortFrames(int slot, int frames)
{
    EventAbortWork *work = eventAbortWorks[slot];
    int previous = 0;
    if (work) {
        previous = work->frames;
        work->frames = frames;
    }
    return previous;
}

int GetEventAbortFrames(int slot)
{
    EventAbortWork *work = eventAbortWorks[slot];
    if (work)
        return work->frames;
    return 0;
}
