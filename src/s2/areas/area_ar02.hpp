#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_queued_dc.h"
#include "s2/areas/area_pigeons.hpp"
#include "s2/areas/area_entities.hpp"

#define AR02_FIRST_PIGEON SHEN_NAME4('h', 'a', 't', '0')
#define AR02_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define AR02_GATE1 SHEN_NAME4('g', 'a', 't', '1')
#define AR02_GATE2 SHEN_NAME4('g', 'a', 't', '2')
#define AR02_ARMW SHEN_NAME4('a', 'r', 'm', 'w')
#define AR02_BTC1 SHEN_NAME4('B', 'T', 'C', '1')
#define TASK_AR02_WAVE SHEN_NAME4('w', 'a', 'v', 'e')
#define TASK_AR02_SPRING SHEN_NAME4('f', 'n', 't', 'n')
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void LoadAR02Audio(int afternoon);
void LoadRobBagPackage(void);
void ApplyAR02Gate(void);
void PrepareAR02NpcState(void);
void AR02WaveCallback(void);
void AR02SpringCallback(void *taskData);
#else
#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaAR02Init();
void AreaAR02Main(std::uint32_t startParameters);
void AreaAR02Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAR02GameplayDefinition;

} // namespace shenmue::s2::areas
#endif
