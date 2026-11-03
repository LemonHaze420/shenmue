#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_queued_dc.h"
#include "s2/areas/area_gambling.hpp"

#define AREA_QTB1 SHEN_NAME4('Q', 'T', 'B', '1')
#define AREA_QTB3 SHEN_NAME4('Q', 'T', 'B', '3')
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void AreaTBAKCleanupNullsub(int afternoon);
void TBAKMainLoopNullsub(void *taskData);
int IsOutsideQTB(void);
#else
#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaTBAKInit();
void AreaTBAKMain(std::uint32_t);
void AreaTBAKCleanup(std::int32_t afternoon, std::uint32_t);
extern const GameplayAreaDefinition areaTBAKGameplayDefinition;
} // namespace shenmue::s2::areas
#endif
