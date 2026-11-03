#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_queued_dc.h"
#include "s2/event_library_tail_dc.h"

#define ANATOBI_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define ANATOBI_HOLE SHEN_NAME4('H', 'O', 'L', 'E')
#define ANATOBI_BASE SHEN_NAME4('b', 'a', 's', 'e')
#define ANATOBI_PACKAGE "JUMP"
#define ANATOBI_POSITION_FLAGS 0x38000000
#define ANATOBI_TARGET_FLAGS 0x40000000
#define ANATOBI_DEBUG_LINES 10
#define ANATOBI_DEBUG_LINE_LENGTH 64
#define ANATOBI_ANGLE(units) ((unsigned short)((float)(units) / 182.044449f * 8192.0f / 45.0f))

enum AnaTobiPointKind {
    ANATOBI_POINT_HOLE = 1,
    ANATOBI_POINT_MOVE = 2,
    ANATOBI_POINT_MOVE_DIRECT = 3,
    ANATOBI_POINT_CURVE = 4,
    ANATOBI_POINT_PUT = 5,
    ANATOBI_POINT_ENEMY = 6,
    ANATOBI_POINT_END = 9999
};

typedef struct AnaTobiPoint {
    short kind;
    short reserved02;
    Vector3 position;
    unsigned int yaw;
    const char *name;
} AnaTobiPoint;
typedef struct AnaTobiEnemy {
    int character;
    Vector3 position;
    int yaw;
    int messageFrame, endFrame, camera, message;
    int reserved24;
} AnaTobiEnemy;
typedef struct AnaTobiEnemyWork {
    int frames;
    int character;
} AnaTobiEnemyWork;
typedef struct AnaTobiWork {
    char reserved00;
    char reserved01;
    char reserved02[2];
    int frames;
    int reserved08[2];
    int phase;
    int reserved14;
    int argument;
    int reserved1c;
    int timer;
    int reserved24[2];
    HLibTask *debugTask;
    const AnaTobiPoint *point;
    int reserved34[15];
} AnaTobiWork;
typedef struct AnaTobiLoader {
    int reserved00;
    void (*callback)(void);
    int reserved08[4];
    const char *package;
    shen_name *names;
    int reserved20[5];
} AnaTobiLoader;

int sprintf(char *buffer, const char *format, ...);

extern Vector3 anaTobiStartPosition;
extern float anaTobiStartYaw;
extern Vector3 anaTobiNoScale;
extern int anaTobiReserved;
extern shen_name anaTobiEnemyNames[];
extern shen_name anaTobiGuardNames[];
extern AnaTobiEnemy anaTobiEnemies[];
extern AnaTobiPoint anaTobiPoints[];
extern int anaTobiReservedBss[2];
extern int anaTobiUnusedCommon[3];
extern int anaTobiCourseIndex;
extern char anaTobiTextBuffer[68];
extern AnaTobiWork anaTobiWork;
extern AnaTobiLoader anaTobiLoader;
extern char anaTobiLogText[ANATOBI_DEBUG_LINES][ANATOBI_DEBUG_LINE_LENGTH];

void StartAnaTobi(int argument);
void QueueAnaTobiMain(void);
void AnaTobiLoad(void *data);
void AnaTobiNextPoint(void *data);
void AnaTobiHoleInit(void *data);
void AnaTobiHoleJump(void *data);
void AnaTobiMoveInit(void *data);
void AnaTobiMoveNext(void *data);
void AnaTobiMoveDirectInit(void *data);
void AnaTobiMoveDirectWait(void *data);
void AnaTobiCurveInit(void *data);
void AnaTobiCurveWait(void *data);
void AnaTobiEnemyInit(void *data);
void AnaTobiEnemyRun(void *data);
void AnaTobiPutInit(void *data);
void AnaTobiShowMessage(int message);
void AnaTobiExit(void *data);
void AnaTobiPlaceBase(void);
void AnaTobiSetTaskDelayed(HLibTaskCallback callback, int delay);
void AnaTobiSetTask(HLibTaskCallback callback);
void AnaTobiDebugDraw(void *data);
void AnaTobiDebugPrint(const char *text);
void AnaTobiDebugInit(void);
void AnaTobiNpcLoadWait(void *data);
void AnaTobiLoadNpcs(const char *package, shen_name *names, void (*callback)(void));
void AnaTobiFreeNpcs(void);
#else
#endif
