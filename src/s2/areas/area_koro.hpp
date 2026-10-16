#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaKOROInit();
void AreaKOROMain(std::uintptr_t parameter);
std::int32_t KORO_PlayDialogue(std::int32_t selection);
void KORO_SetLightState(std::int32_t animate);
extern const ScriptAreaDefinition areaKOROScriptDefinition;
} // namespace shenmue::s2::areas
