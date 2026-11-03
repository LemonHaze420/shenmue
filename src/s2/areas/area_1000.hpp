#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_library_tail_dc.h"

void StartMotionViewer(void);
void UpdateMotionViewer(void *taskData);
#else
#endif
