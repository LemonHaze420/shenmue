#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaBOK2Init();
void AreaBOK2Main(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaBOK2ScriptDefinition;
} // namespace shenmue::s2::areas
