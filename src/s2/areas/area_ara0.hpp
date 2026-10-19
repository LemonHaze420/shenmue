#pragma once

#include "s2/area_event.hpp"
#include "s2/npc_package.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name ARA0_DEFAULT_LIGHT{SHEN_NAME(0000)};
inline constexpr shen_name ARA0_DARTS_ENTITY{SHEN_NAME(DART)};
inline constexpr shen_name ARA0_EVENT_TASK{SHEN_NAME(EVNT)};

inline constexpr ConditionalNpcLoadEntry areaARA0DartsNpcLoads[]{
    {.character = SHEN_NAME(JON_), .position = {10000, 0, 0}, .motion = 0xf002, .storyFlag = -1, .condition = AreConditionalNpcLoadsPending},
    {.character = SHEN_NAME(FCJ_), .position = {10000, 0, 0}, .motion = 0xf002, .storyFlag = -1, .condition = AreConditionalNpcLoadsPending},
    {.character = shen_name{0xffffffffU}}};

void AreaARA0Init();
void AreaARA0Main(std::uint32_t startParameters);
void AreaARA0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaARA0GameplayDefinition;

} // namespace shenmue::s2::areas
