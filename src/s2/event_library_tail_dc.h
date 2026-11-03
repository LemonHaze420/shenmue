#ifndef SHENMUE_S2_EVENT_LIBRARY_TAIL_DC_H
#define SHENMUE_S2_EVENT_LIBRARY_TAIL_DC_H
#include "s2/events/event_dc.h"
#include <stdarg.h>

extern int vsprintf(char *buffer, const char *format, va_list arguments);

typedef struct EventMessageWork {
    const char *text;
    int frames;
} EventMessageWork;
typedef struct EventSceneLoadWork {
    shen_name scene;
    int entry;
} EventSceneLoadWork;
void EventDebugPrintf(int column, int line, const char *format, ...);
void EventMessageCallback(void *data);
void EventMessageWaitCallback(void *data);
void QueueEventMessage(const char *text, int delay, int frames);
void EventSceneLoadCallback(void *data);
void QueueEventSceneLoad(shen_name scene, int entry, int delay);

#endif
