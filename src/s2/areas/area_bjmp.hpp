#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaBJMPInit();
void AreaBJMPMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaBJMPScriptDefinition;
} // namespace shenmue::s2::areas
