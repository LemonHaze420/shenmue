#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaAKTMInit();
void AreaAKTMMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaAKTMScriptDefinition;
} // namespace shenmue::s2::areas
