#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaAOZRInit();
void AreaAOZRMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaAOZRScriptDefinition;
} // namespace shenmue::s2::areas
