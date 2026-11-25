#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void AreaKRK6Nullsub(void);
void KRK6MainLoopCallback(void *data);
void KRK6WaveCallback(void *data);
#else

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaKRK6Init();
void AreaKRK6Main(std::uint32_t startParameters);
void AreaKRK6Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKRK6GameplayDefinition;

} // namespace shenmue::s2::areas

#endif
