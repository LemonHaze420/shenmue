#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2 {

inline constexpr shen_name WNPC_AREA{SHEN_NAME(WNPC)}, WNPC_TASK{SHEN_NAME(wnpc)};

void WNPCInit();
void WNPCMain(std::uintptr_t argument);
void WNPCWaitNpc(void *taskData);
extern const ScriptAreaDefinition wnpcScriptDefinition;

} // namespace shenmue::s2
