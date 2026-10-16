#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaAPA0Init();
void AreaAPA0Main(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaAPA0ScriptDefinition;
} // namespace shenmue::s2::areas
