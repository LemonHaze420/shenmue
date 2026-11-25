#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void WaveKRH1(void *taskData);
#else
#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaKRH1Init();
void AreaKRH1Main(std::uint32_t startParameters);
void AreaKRH1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKRH1GameplayDefinition;

} // namespace shenmue::s2::areas
#endif
