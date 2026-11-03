#ifndef SHENMUE_S2_AREAS_COMMON_ET_NPC_LOAD_H
#define SHENMUE_S2_AREAS_COMMON_ET_NPC_LOAD_H

#include "s2/events/event_dc.h"

typedef struct ConditionalNpcLoadEntry {
    shen_name character;
    Vector3 position;
    int yaw;
    unsigned int motion;
    int storyFlag;
    unsigned int (*condition)(void);
} ConditionalNpcLoadEntry;
void QueueConditionalNpcLoads(ConditionalNpcLoadEntry *entries);
unsigned int AreConditionalNpcLoadsPending(void);

#endif
