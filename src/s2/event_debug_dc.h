#ifndef SHENMUE_S2_EVENT_DEBUG_DC_H
#define SHENMUE_S2_EVENT_DEBUG_DC_H
#include "s2/events/event_dc.h"

void EventDebugRegister(int id, HLibTaskCallback callback, void *data, int frames);
void EventDebugUnregister(int id);
void EventDebugUpdate(void);
int EventDebugCheck(int id, int item);
int EventDebugGetState(int id, int item);
int EventDebugIsActive(void);
int EventDebugIsPaused(void);
int EventDebugIsSkipping(void);

#endif
