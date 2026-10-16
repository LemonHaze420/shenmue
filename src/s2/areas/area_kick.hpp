#pragma once
#include "s2/area_event.hpp"

namespace shenmue::s2::areas {
void AreaKICKInit();
void AreaKICKMain(std::uintptr_t parameter);
void KICK_KickDoor_Callback(void *data);
extern const ScriptAreaDefinition areaKICKScriptDefinition;
} // namespace shenmue::s2::areas
