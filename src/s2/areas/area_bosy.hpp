#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaBOSYInit();
void AreaBOSYMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaBOSYScriptDefinition;
} // namespace shenmue::s2::areas
