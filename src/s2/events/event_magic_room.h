#ifndef SHENMUE_S2_EVENTS_EVENT_MAGIC_ROOM_H
#define SHENMUE_S2_EVENTS_EVENT_MAGIC_ROOM_H

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
typedef struct MagicRoomDescriptor {
    struct { float x, y; } quad[4], center, size;
    unsigned short yaw, reserved32;
    int type;
} MagicRoomDescriptor;
struct MagicRoomFloorGroup {
    int entryCount, reserved04;
    struct { float x, y; } heightRange;
    const MagicRoomDescriptor *entries;
};
#else
#include "s2/magic_room.hpp"
#endif

#endif
