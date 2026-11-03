#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/common/et_darts.h"
#include "s2/areas/common/et_npc_load.h"

#define WRS2_EVENT_TASK TASK_EVNT
#define WRS2_DARTS_ENTITY SHEN_NAME4('D', 'A', 'R', 'T')
void AreaMain(unsigned int startParameters);
void AreaCleanup(int afternoon);
void AreaWRS2CleanupNullsub(int afternoon);
void WRS2MainLoopNullsub(void *taskData);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/npc_package.hpp"
namespace shenmue::s2::areas {
inline constexpr ConditionalNpcLoadEntry areaWRS2DartsNpcLoads[]{
    {.character = SHEN_NAME(FCJ_), .position = {10000, 0, 0}, .motion = 0xf002, .storyFlag = -1, .condition = AreConditionalNpcLoadsPending},
    {.character = SHEN_NAME(JON_), .position = {10000, 0, 0}, .motion = 0xf002, .storyFlag = -1, .condition = AreConditionalNpcLoadsPending},
    {.character = shen_name{0xffffffffU}}};
void AreaWRS2Init();
void AreaWRS2Main(std::uint32_t startParameters);
void AreaWRS2Cleanup(std::int32_t afternoon, std::uint32_t startParameters);
extern const GameplayAreaDefinition areaWRS2GameplayDefinition;
} // namespace shenmue::s2::areas
#endif
