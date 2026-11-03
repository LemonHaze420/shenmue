#ifndef SHENMUE_S2_EVENTS_EVENT_DC_H
#define SHENMUE_S2_EVENTS_EVENT_DC_H

#define SHENMUE_EVENT_BOUNDARY() do { } while (0)
#define SHENMUE_EVENT_OPAQUE(value) __asm__ ("" : "+r" (value))
#define SHENMUE_EVENT_CLOBBER(reg) __asm__ ("" : : : reg)
#define SHENMUE_EVENT_USE(value) __asm__ ("" : : "X" (value))

typedef unsigned int shen_name;
#define SHEN_NAME4(a, b, c, d) ((shen_name)(a) | (shen_name)(b) << 8 | (shen_name)(c) << 16 | (shen_name)(d) << 24)
#define TASK_EVNT SHEN_NAME4('E', 'V', 'N', 'T')
#define LIGHT_DEFAULT_DEFINITION SHEN_NAME4('0', '0', '0', '0')

typedef struct Vector3 {
    float x, y, z;
} Vector3;
typedef struct Vector3i {
    int x, y, z;
} Vector3i;
typedef struct MotionWork MotionWork;
typedef struct DateTime DateTime;
typedef struct HLibTask HLibTask;
typedef void (*HLibTaskCallback)(void *data);

typedef struct EventFunctionTable {
#define EVENT_FUNCTION(ret, name, params) ret (*name) params;
#include "s2/events/event_ftbl.inc"
#undef EVENT_FUNCTION
} EventFunctionTable;

extern EventFunctionTable FTBL;
extern unsigned int g_frameCounter;
extern int g_actNumber;
extern shen_name g_currentMap;
extern int g_entryPoint;
void *memcpy(void *destination, const void *source, unsigned int size);
char *strcpy(char *destination, const char *source);
char *strcat(char *destination, const char *source);

#endif
