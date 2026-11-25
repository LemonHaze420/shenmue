#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void AKTUnusedCleanup(int afternoon);
void AKT3PostUpdateCallback(void *data);
int IsOutsideQTB(void);
#else

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaAKT3Init();
void AreaAKT3Main(std::uint32_t startParameters);
void AreaAKT3Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAKT3GameplayDefinition;

} // namespace shenmue::s2::areas

#endif
