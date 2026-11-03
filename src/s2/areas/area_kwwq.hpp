#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void AreaKWWQCleanupNullsub(int afternoon);
void KWWQMainLoopCallback(void *taskData);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaKWWQInit();
void AreaKWWQMain(std::uint32_t startParameters);
void AreaKWWQCleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKWWQGameplayDefinition;

} // namespace shenmue::s2::areas
#endif
