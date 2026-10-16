#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name GAIS_TASK{SHEN_NAME(gais)}, GAIS_STOPPER{SHEN_NAME(GAI_)}, GAIS_PLAYER{SHEN_NAME(RYO_)};
void AreaGAISInit();
void AreaGAISMain(std::uintptr_t argument);
void GAISStopperCallback(void *parameter);
extern const ScriptAreaDefinition areaGAISScriptDefinition;

} // namespace shenmue::s2::areas
