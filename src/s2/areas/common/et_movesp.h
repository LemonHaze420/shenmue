#ifndef SHENMUE_S2_AREAS_COMMON_ET_MOVESP_H
#define SHENMUE_S2_AREAS_COMMON_ET_MOVESP_H

#include "s2/event_queued_dc.h"

#define EVENT_MOVE_SLOT_COUNT 20
#define EVENT_MOVE_CANCEL_ALL (-1)
#define EVENT_MOVE_KEEP_MOTION 0x20000
#define EVENT_MOVE_FACE_FIRST_WAYPOINT 0x40000
#define EVENT_MOVE_BLEND_MOTION 0x80000
#define EVENT_MOVE_TRANSITION_7 0x100000
#define EVENT_MOVE_TRANSITION_6 0x200000
#define EVENT_MOVE_TRANSITION_5 0x400000
#define EVENT_MOVE_MIRROR_2 0x2000000
#define EVENT_MOVE_MIRROR_1 0x4000000
#define EVENT_MOVE_TRANSITION_11 0x40000000
#define EVENT_MOVE_DEFAULT_MOTION 0xf005
#define EVENT_MOVE_DEFAULT_ALTERNATE_MOTION 0xf006

typedef struct EventMoveWaypoint {
    Vector3 position;
    float yaw;
} EventMoveWaypoint;
typedef struct EventMoveMain {
    HLibTask *parent;
    shen_name character;
    const EventMoveWaypoint *waypoints;
    const unsigned int *motions;
    float speed;
    int index;
    int active;
    int slot;
    int flags;
} EventMoveMain;

void EVT_MV_ResetMoveMain(void);
void EVT_MV_CancelAllMoveMain(void);
int EVT_MV_MoveMain_Enqueue(HLibTask *parent, shen_name character, const EventMoveWaypoint *waypoints, const unsigned int *motions, float speed,
    int flags, int delay);
void EVT_MV_InitializeMoveMain(EventMoveMain *work, int delay);
void EVT_MV_MoveMain_Callback(void *data);
void EVT_MV_StartMoveMainSegment(EventMoveMain *work);
void EVT_MV_ConfigureMoveMainMotions(EventMoveMain *work);
void Queue_EVT_MV_CancelMoveMain(int slot, int delay);
void EVT_MV_CancelMoveMain_Callback(void *data);
int EVT_MV_IsMoveMainActive(int slot);
void EVT_MV_SetMoveMainWaypoints(int slot, const EventMoveWaypoint *waypoints);
void EVT_MV_SetMoveMainIndex(int slot, int index);
int EVT_MV_GetMoveMainIndex(int slot);
void EVT_MV_SetMoveMainSpeed(int slot, float speed);
void EVT_MV_CancelMoveMain(int slot);

#endif
