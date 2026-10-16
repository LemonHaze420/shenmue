#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name TES2_TASK{SHEN_NAME(EVNT)};
void Tes2Init();
void Tes2Main(std::uintptr_t argument);
void Tes2MainLoopCallback(void *taskData);
extern const ScriptAreaDefinition tes2ScriptDefinition;

} // namespace shenmue::s2
