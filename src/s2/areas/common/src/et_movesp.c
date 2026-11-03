#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/common/et_movesp.h"

static int eventMoveActive[EVENT_MOVE_SLOT_COUNT];
static HLibTask *eventMoveTasks[EVENT_MOVE_SLOT_COUNT];

void EVT_MV_ResetMoveMain(void)
{
    int slot;
    for (slot = 0; slot < EVENT_MOVE_SLOT_COUNT; slot++)
        eventMoveActive[slot] = 0;
}

void EVT_MV_CancelAllMoveMain(void)
{
    int slot;
    for (slot = 0; slot < EVENT_MOVE_SLOT_COUNT; slot++)
        EVT_MV_CancelMoveMain(slot);
}

int EVT_MV_MoveMain_Enqueue(HLibTask *parent, shen_name character, const EventMoveWaypoint *waypoints, const unsigned int *motions, float speed,
    int flags, int delay)
{
    HLibTask *task;
    EventMoveMain *work;
    int slot;
    for (slot = 0; slot < EVENT_MOVE_SLOT_COUNT; slot++)
        if (!eventMoveActive[slot])
            break;
    if (slot == EVENT_MOVE_SLOT_COUNT)
        FTBL.EventAssertNullsub(__FILE__, 103);
    task = FTBL.HLib_EnqueueTaskWithParameter(EVT_MV_MoveMain_Callback, 6, 11, sizeof(EventMoveMain), TASK_EVNT);
    work = (EventMoveMain *)FTBL.HLib_GetTaskParameter(task);
    if (work->flags & EVENT_MOVE_KEEP_MOTION) {
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
    } else if (delay + 5 > 0)
        FTBL.HLib_SetupTimedTask(task, delay + 5);
    eventMoveActive[slot] = 1;
    eventMoveTasks[slot] = task;
    work->parent = parent;
    work->character = character;
    work->waypoints = waypoints;
    work->motions = motions;
    work->speed = speed;
    work->index = -1;
    work->active = 0;
    work->slot = slot;
    work->flags = flags;
    EVT_MV_InitializeMoveMain(work, delay);
    if (work->parent)
        FTBL.HLib_SetupTimedTask(work->parent, 0);
    return work->slot;
}

void EVT_MV_InitializeMoveMain(EventMoveMain *work, int delay)
{
    Vector3 position;
    Vector3i rotation;
    unsigned int motion;
    float frame = 0.0f;
    unsigned int blend = 0;
    if (!(work->flags & EVENT_MOVE_KEEP_MOTION)) {
        if (work->flags < 0)
            motion = work->motions && work->motions[4] ? work->motions[4] : EVENT_MOVE_DEFAULT_ALTERNATE_MOTION;
        else
            motion = work->motions && work->motions[1] ? work->motions[1] : EVENT_MOVE_DEFAULT_MOTION;
        if (!(work->flags & EVENT_MOVE_BLEND_MOTION)) {
            if (motion == FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(work->character), 0)) {
                frame = FTBL.GetMotionFrame(FTBL.scenFindMotionwork(work->character), 0);
                blend = 0x40000000U;
            }
        } else
            blend = 0x40000000U;
        EVT_AT_SetMot(work->character, motion, frame, work->speed, blend, delay);
    }
    if (work->flags & EVENT_MOVE_FACE_FIRST_WAYPOINT) {
        float dx, dz;
        rotation.x = 0;
        rotation.y = 0;
        rotation.z = 0;
        FTBL.GetCharaPosition(work->character, -1, &position, 0x40000000);
        dx = work->waypoints->position.x - position.x;
        dz = work->waypoints->position.z - position.z;
        rotation.y = FTBL.InverseFixedAtan2(dx, dz);
        FTBL.EV_AT_SetAngY_0(work->character, 0x78000000, &rotation, 0);
    }
}

void EVT_MV_MoveMain_Callback(void *data)
{
    EventMoveMain *work = (EventMoveMain *)data;
    const EventMoveWaypoint *next;
    switch (work->active) {
    case 0:
        work->index++;
        work->active++;
        EVT_MV_StartMoveMainSegment(work);
        EVT_MV_ConfigureMoveMainMotions(work);
        break;
    case 1:
        if (!FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(work->character), 58, 1)) {
            next = &work->waypoints[work->index + 1];
            if (next->position.y > 0.0f)
                work->active = 0;
            else {
                if (work->parent)
                    FTBL.HLib_ResolveDeferredCallback(work->parent);
                FTBL.SetMotionTimeRelated(FTBL.scenFindMotionwork(work->character), 0, 0, 0);
                eventMoveActive[work->slot] = 0;
                FTBL.HLib_DestroyCurrentTask();
            }
        }
        break;
    }
}

void EVT_MV_StartMoveMainSegment(EventMoveMain *work)
{
    const EventMoveWaypoint *waypoint = &work->waypoints[work->index];
    int mirror;
    if (work->flags & EVENT_MOVE_MIRROR_1)
        mirror = 1;
    else
        mirror = (work->flags & EVENT_MOVE_MIRROR_2) ? 2 : 0;
    if (work->flags & EVENT_MOVE_TRANSITION_5)
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(work->character), &waypoint->position, (unsigned short)(waypoint->yaw * 8192.0f / 45.0f), 5, mirror);
    else if (work->flags & EVENT_MOVE_TRANSITION_6)
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(work->character), &waypoint->position, (unsigned short)(waypoint->yaw * 8192.0f / 45.0f), 6, mirror);
    else if (work->flags & EVENT_MOVE_TRANSITION_7)
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(work->character), &waypoint->position, (unsigned short)(waypoint->yaw * 8192.0f / 45.0f), 7, mirror);
    else
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(work->character), &waypoint->position, (unsigned short)(waypoint->yaw * 8192.0f / 45.0f), 0, mirror);
    if (work->flags < 0)
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(work->character), 0, 0, 8, 0);
    else
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(work->character), 0, 0, 14, 0);
    if (work->flags & EVENT_MOVE_TRANSITION_11)
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(work->character), 0, 0, 11, 0);
    FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(work->character), work->speed, 0);
}

void EVT_MV_ConfigureMoveMainMotions(EventMoveMain *work)
{
    if (work->motions) {
        if (work->motions[0])
            FTBL.SetMotionTimeRelated(FTBL.scenFindMotionwork(work->character), 1, 0, (unsigned short)work->motions[0]);
        if (work->motions[1])
            FTBL.SetMotionTimeRelated(FTBL.scenFindMotionwork(work->character), 1, 1, (unsigned short)work->motions[1]);
        if (work->motions[4])
            FTBL.SetMotionTimeRelated(FTBL.scenFindMotionwork(work->character), 1, 4, (unsigned short)work->motions[4]);
        if (work->motions[5])
            FTBL.SetMotionTimeRelated(FTBL.scenFindMotionwork(work->character), 1, 5, (unsigned short)work->motions[5]);
    }
}

void Queue_EVT_MV_CancelMoveMain(int slot, int delay)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_MV_CancelMoveMain_Callback, 6, 11, sizeof(int), TASK_EVNT);
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    *(int *)FTBL.HLib_GetTaskParameter(task) = slot;
}

void EVT_MV_CancelMoveMain_Callback(void *data)
{
    if (*(int *)data == EVENT_MOVE_CANCEL_ALL)
        EVT_MV_CancelAllMoveMain();
    else
        EVT_MV_CancelMoveMain(*(int *)data);
}

int EVT_MV_IsMoveMainActive(int slot)
{
    if (eventMoveActive[slot])
        return 1;
    return 0;
}

void EVT_MV_SetMoveMainWaypoints(int slot, const EventMoveWaypoint *waypoints)
{
    EventMoveMain *work;
    if (eventMoveActive[slot]) {
        work = (EventMoveMain *)FTBL.HLib_GetTaskParameter(eventMoveTasks[slot]);
        work->waypoints = waypoints;
        work->index = 0;
        EVT_MV_StartMoveMainSegment(work);
        EVT_MV_ConfigureMoveMainMotions(work);
        work->active = 1;
    }
}

void EVT_MV_SetMoveMainIndex(int slot, int index)
{
    EventMoveMain *work;
    if (eventMoveActive[slot]) {
        work = (EventMoveMain *)FTBL.HLib_GetTaskParameter(eventMoveTasks[slot]);
        work->index = index;
        EVT_MV_StartMoveMainSegment(work);
        EVT_MV_ConfigureMoveMainMotions(work);
        work->active = 1;
    }
}

int EVT_MV_GetMoveMainIndex(int slot)
{
    if (eventMoveActive[slot])
        return ((EventMoveMain *)FTBL.HLib_GetTaskParameter(eventMoveTasks[slot]))->index;
    return -1;
}

void EVT_MV_SetMoveMainSpeed(int slot, float speed)
{
    EventMoveMain *work;
    if (eventMoveActive[slot]) {
        work = (EventMoveMain *)FTBL.HLib_GetTaskParameter(eventMoveTasks[slot]);
        work->speed = speed;
        EVT_MV_StartMoveMainSegment(work);
        EVT_MV_ConfigureMoveMainMotions(work);
        work->active = 1;
    }
}

void EVT_MV_CancelMoveMain(int slot)
{
    EventMoveMain *work;
    if (eventMoveActive[slot]) {
        work = (EventMoveMain *)FTBL.HLib_GetTaskParameter(eventMoveTasks[slot]);
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(work->character), 0, 0, 1, 0);
        FTBL.SetMotionTimeRelated(FTBL.scenFindMotionwork(work->character), 0, 0, 0);
        if (work->parent)
            FTBL.HLib_ResolveDeferredCallback(work->parent);
        eventMoveActive[slot] = 0;
        FTBL.HLib_DestroyTaskInternal(eventMoveTasks[slot]);
    }
}
#endif
