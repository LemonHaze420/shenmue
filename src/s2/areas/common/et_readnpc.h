#ifndef SHENMUE_S2_AREAS_COMMON_ET_READNPC_H
#define SHENMUE_S2_AREAS_COMMON_ET_READNPC_H

#include "s2/events/event_dc.h"

#define EVENT_NPC_READ_COUNT 32
#define EVENT_NPC_READ_INVALID 0xffffffffU

typedef struct EventNpcReadWork {
    shen_name characters[EVENT_NPC_READ_COUNT];
    unsigned int states[EVENT_NPC_READ_COUNT];
    unsigned int requests[EVENT_NPC_READ_COUNT];
    int busy;
    shen_name loadingCharacter;
} EventNpcReadWork;

EventNpcReadWork *QueueEventNpcRead(void);
void EventNpcReadCallback(void *data);
int IsEventNpcReadBusy(EventNpcReadWork *work);
void RequestEventNpcCharacters(EventNpcReadWork *work, const shen_name *characters);
int AreEventNpcReadsFree(EventNpcReadWork *work);
void ReleaseEventNpcRequests(EventNpcReadWork *work);
void RetireEventNpcRequests(EventNpcReadWork *work);

#endif
