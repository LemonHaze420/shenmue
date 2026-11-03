#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"

#define GAIS_TASK SHEN_NAME4('g', 'a', 'i', 's')
#define GAIS_STOPPER SHEN_NAME4('G', 'A', 'I', '_')
#define GAIS_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
void AreaGAISMain(void *argument);
void GAISStopperCallback(void *data);
void GAISWakeCallback(void *data);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name GAIS_TASK{SHEN_NAME(gais)}, GAIS_STOPPER{SHEN_NAME(GAI_)}, GAIS_PLAYER{SHEN_NAME(RYO_)};
void AreaGAISInit();
void AreaGAISMain(std::uintptr_t argument);
void GAISStopperCallback(void *parameter);
extern const ScriptAreaDefinition areaGAISScriptDefinition;

} // namespace shenmue::s2::areas
#endif
