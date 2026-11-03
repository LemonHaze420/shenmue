#ifndef SHENMUE_S2_AREAS_COMMON_ET_ABORT_H
#define SHENMUE_S2_AREAS_COMMON_ET_ABORT_H

#include "s2/events/event_dc.h"

#define TASK_ABRT SHEN_NAME4('a', 'b', 'r', 't')
#define EVENT_ABORT_SLOT_COUNT 4
#define EVENT_ABORT_ACTIVE 2
#define EVENT_ABORT_INPUT 704

typedef struct EventSlotState {
    unsigned int flags;
    unsigned int reserved04[3];
} EventSlotState;
extern EventSlotState g_eventSlotStates[EVENT_ABORT_SLOT_COUNT];

typedef struct EventAbortWork {
    HLibTask *task;
    const char *slotName;
    int slot;
    HLibTaskCallback callback;
    void *data;
    int frames;
    unsigned int *flags;
} EventAbortWork;

void QueueEventAbort(int slot, HLibTaskCallback callback, void *data, int frames);
void CancelEventAbort(int slot);
void EventAbortCallback(void *data);
void SetEventAbortCallback(int slot, HLibTaskCallback callback, void *data);
unsigned int SetEventAbortFlags(int slot, unsigned int mask);
unsigned int ClearEventAbortFlags(int slot, unsigned int mask);
unsigned int GetEventAbortFlags(int slot);
int SetEventAbortFrames(int slot, int frames);
int GetEventAbortFrames(int slot);

#endif
