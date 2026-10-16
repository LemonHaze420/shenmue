#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaBTELInit();
void AreaBTELMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaBTELScriptDefinition;
} // namespace shenmue::s2::areas
