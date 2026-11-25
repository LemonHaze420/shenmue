#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void AKTUnusedCleanup(int afternoon);
void AKT2PostUpdateCallback(void *data);
int IsOutsideQTB(void);
#else

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaAKT2Init();
void AreaAKT2Main(std::uint32_t startParameters);
void AreaAKT2Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAKT2GameplayDefinition;

} // namespace shenmue::s2::areas

#endif
