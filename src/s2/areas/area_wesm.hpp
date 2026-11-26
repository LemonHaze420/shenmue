#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

#define WESM_BREATHER SHEN_NAME4('M', 'A', 'K', '_')
#define WESM_PROP SHEN_NAME4('M', 'A', 'K', 'I')
#define WESM_BELL SHEN_NAME4('B', 'E', 'E', 'L')
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void WESMSpringLoop(void *taskData);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void WESMUpdateFloor(void);
#endif
void LoadWESMNpcs(void);
void InitializeWESMFireBreather(void);
void WESMFireBreatherLoop(void *taskData);
#else
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaWESMInit();
void AreaWESMMain(std::uint32_t startParameters);
void AreaWESMCleanup(std::int32_t afternoon, std::uint32_t startParameters);
extern const GameplayAreaDefinition areaWESMGameplayDefinition;
} // namespace shenmue::s2::areas
#endif
