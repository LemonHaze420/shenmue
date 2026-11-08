#pragma once

#include "s2/area_event.hpp"
#include "s2/npc_package.hpp"

namespace shenmue::s2::areas {

inline constexpr ConditionalNpcLoadEntry areaAKS1DartsNpcLoads[]{
    {.character = SHEN_NAME(HCY_), .position = {10000, 0, 0}, .motion = 0xf002, .storyFlag = -1, .condition = AreConditionalNpcLoadsPending},
    {.character = shen_name{0xffffffffU}}};

void AreaAKS1Init();
void AreaAKS1Main(std::uint32_t startParameters);
void AreaAKS1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAKS1GameplayDefinition;

} // namespace shenmue::s2::areas
