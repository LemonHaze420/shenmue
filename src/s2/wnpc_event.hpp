#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

#define WNPC_TASK SHEN_NAME4('w', 'n', 'p', 'c')
void WNPCMain(void);
void WNPCWaitNpc(void *taskData);
#else
#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2 {

inline constexpr shen_name WNPC_AREA{SHEN_NAME(WNPC)}, WNPC_TASK{SHEN_NAME(wnpc)};

void WNPCInit();
void WNPCMain(std::uintptr_t argument);
void WNPCWaitNpc(void *taskData);
extern const ScriptAreaDefinition wnpcScriptDefinition;

} // namespace shenmue::s2
#endif
