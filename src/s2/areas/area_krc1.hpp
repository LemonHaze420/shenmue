#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
#else

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaKRC1Init();
void AreaKRC1Main(std::uint32_t startParameters);
void AreaKRC1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKRC1GameplayDefinition;

} // namespace shenmue::s2::areas

#endif
