#include "s2/event_debug_dc.h"

#if defined(SHENMUE_EVENT_DEBUG_GAMEJAM)
typedef struct EventDebugWork {
    HLibTask *task;
    const char *area;
    int id;
    HLibTaskCallback callback;
    void *argument;
    int frames;
    unsigned int *flags;
} EventDebugWork;

extern unsigned int g_eventTaskFlags1[4];
extern unsigned int g_eventTaskFlags2[4];
extern unsigned int g_eventTaskFlags3[4];
static EventDebugWork *eventDebugWork[4];
static const char eventDebugSeparator[] = "-----------------------------------------------------\n\n";

void EventDebugRegister(int id, HLibTaskCallback callback, void *data, int frames)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventDebugUpdate, 4, 11, sizeof(EventDebugWork), SHEN_NAME4('a', 'b', 'r', 't'));
    EventDebugWork *work = (EventDebugWork *)FTBL.HLib_GetTaskParameter(task);
    eventDebugWork[id] = work;
    work->area = FTBL.GetAreaSlotName(id);
    work->task = task;
    work->id = id;
    work->callback = callback;
    work->argument = data;
    work->frames = frames;
    switch (id) {
    case 1: work->flags = g_eventTaskFlags1; break;
    case 2: work->flags = g_eventTaskFlags2; break;
    case 3: work->flags = g_eventTaskFlags3; break;
    }
    if (*work->flags & 2) {
        FTBL.DebugLogToFileRemoved(eventDebugSeparator);
        FTBL.DebugLogToFileRemoved("               Error : (Event Leak)                  \n\n");
        FTBL.DebugLogToFileRemoved(eventDebugSeparator);
    }
    *work->flags = 2;
}

void EventDebugUnregister(int id)
{
    EventDebugWork *work = eventDebugWork[id];
    if (work) {
        FTBL.ctrlStopSubtitles();
        *work->flags = 0;
        FTBL.HLib_DestroyTaskInternal(work->task);
        eventDebugWork[id] = 0;
    }
}

void EventDebugUpdate(void *data)
{
    EventDebugWork *work = (EventDebugWork *)data;
    if (!FTBL.IsFightActive()) {
        int aborted = FTBL.EVT_GetControllerInput(0, 0) == 0x2c0;
        aborted = aborted || (*work->flags & 1);
        aborted = aborted && (*work->flags & 4);
        if (--work->frames <= 0 || aborted) {
            if (aborted) {
                FTBL.DebugLogToFileRemoved(eventDebugSeparator);
                FTBL.DebugLogToFileRemoved("                     Event Abort                     \n\n");
                FTBL.DebugLogToFileRemoved(eventDebugSeparator);
            } else {
                FTBL.DebugLogToFileRemoved(eventDebugSeparator);
                FTBL.DebugLogToFileRemoved("                   Event Time Limit                  \n\n");
                FTBL.DebugLogToFileRemoved(eventDebugSeparator);
            }
            EventDebugGetState(work->id, 4);
            if (FTBL.IsFightActive())
                FTBL.check_dead_status(1);
            if (FTBL.EV_authSceneCtrl(-1))
                FTBL.EV_authSceneCtrl(-4);
            work->callback(work->argument);
        }
    }
}

void EventDebugSetCallback(int id, HLibTaskCallback callback, void *data)
{
    EventDebugWork *work = eventDebugWork[id];
    if (work) {
        work->callback = callback;
        work->argument = data;
    }
}

unsigned int EventDebugCheck(int id, unsigned int flags)
{
    EventDebugWork *work = eventDebugWork[id];
    unsigned int previous = 0;
    if (work) {
        previous = *work->flags;
        *work->flags = flags | previous;
    }
    return previous;
}

unsigned int EventDebugGetState(int id, unsigned int flags)
{
    EventDebugWork *work = eventDebugWork[id];
    unsigned int previous = 0;
    if (work) {
        previous = *work->flags;
        *work->flags = ~flags & previous;
    }
    return previous;
}

unsigned int EventDebugGetFlags(int id)
{
    EventDebugWork *work = eventDebugWork[id];
    unsigned int flags;
    if (work == 0)
        flags = 0;
    else
        flags = *work->flags;
    return flags;
}

int EventDebugSetFrames(int id, int frames)
{
    EventDebugWork *work = eventDebugWork[id];
    int previous = 0;
    if (work) {
        previous = work->frames;
        work->frames = frames;
    }
    return previous;
}

int EventDebugGetFrames(int id)
{
    EventDebugWork *work = eventDebugWork[id];
    if (work)
        return work->frames;
    return 0;
}
#else
void EventDebugRegister(int id, HLibTaskCallback callback, void *data, int frames)
{
}

void EventDebugUnregister(int id)
{
}

void EventDebugUpdate(void)
{
}

int EventDebugCheck(int id, int item)
{
    return 0;
}

int EventDebugGetState(int id, int item)
{
    return 0;
}

int EventDebugIsActive(void)
{
    return 0;
}

int EventDebugIsPaused(void)
{
    return 0;
}

int EventDebugIsSkipping(void)
{
    return 0;
}
#endif
