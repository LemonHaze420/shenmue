#ifndef SHENMUE_S2_AREAS_NUMERIC_EVENT_DC_H
#define SHENMUE_S2_AREAS_NUMERIC_EVENT_DC_H
#include "s2/event_utility_dc.h"

typedef struct AreaAuthorTaskParameter {
    HLibTask *self;
    void *motion;
    short selection, state;
} AreaAuthorTaskParameter;
typedef struct AreaAuthorLoopTaskParameter {
    AreaAuthorTaskParameter *parent;
} AreaAuthorLoopTaskParameter;
typedef struct AreaAuthorFrameTaskParameter {
    AreaAuthorTaskParameter *parent;
    unsigned short currentFrame;
} AreaAuthorFrameTaskParameter;
typedef struct AreaImmediateAuthorTaskParameter {
    HLibTask *self;
    short selection, state;
} AreaImmediateAuthorTaskParameter;
typedef struct AreaImmediateAuthorFrameParameter {
    AreaImmediateAuthorTaskParameter *parent;
    short state;
} AreaImmediateAuthorFrameParameter;
#endif
