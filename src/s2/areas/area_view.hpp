#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

void StartView(void);
void UpdateView(void *taskData);
#else
#endif
