#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaISTBInit();
void AreaISTBMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaISTBScriptDefinition;
} // namespace shenmue::s2::areas
