#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void KRM3NonSleepReadyCallback(void *data);
void KRM3StartCallback(void *data);
void KRM3WaitCallback(void *data);
void KRM3WaveCallback(void *data);
#else

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaKRM3Init();
void AreaKRM3Main(std::uint32_t startParameters);
void AreaKRM3Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKRM3GameplayDefinition;

} // namespace shenmue::s2::areas

#endif
