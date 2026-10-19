#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name FIND_TASK{SHEN_NAME(find)};
inline constexpr shen_name FIND_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name FIND_BASE{SHEN_NAME(BASE)};
inline constexpr char FIND_CHAWAN_SLOT[] = "CHAW.EV3";

void AreaFINDInit();
void FINDBeginSignSearchCallback(void *);
void FINDDescribeSignCallback(void *);
void FINDOfferSignCallback(void *);
void FINDWaitChoiceCallback(void *);
void FINDFinishChoiceCallback(void *);
void FINDEnterChawanCallback(void *);
void AreaFINDMain(std::uintptr_t placement);
extern const ScriptAreaDefinition areaFINDScriptDefinition;

} // namespace shenmue::s2::areas
