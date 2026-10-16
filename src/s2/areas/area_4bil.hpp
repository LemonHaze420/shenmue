#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void Initialize4Bil();
void Start4Bil(std::uintptr_t argument);
extern const ScriptAreaDefinition area4BilScriptDefinition;
} // namespace shenmue::s2::areas
