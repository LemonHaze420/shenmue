#pragma once
#include "s2/area_event.hpp"
#include "s2/npc_package.hpp"
namespace shenmue::s2::areas {
inline constexpr ConditionalNpcLoadEntry areaWEG0DartsNpcLoads[]{
    {.character = SHEN_NAME(XHO_), .position = {10000, 0, 0}, .motion = 0xf002, .storyFlag = -1, .condition = AreConditionalNpcLoadsPending},
    {.character = SHEN_NAME(HLN_), .position = {10000, 0, 0}, .motion = 0xf002, .storyFlag = -1, .condition = AreConditionalNpcLoadsPending},
    {.character = shen_name{0xffffffffU}}};
void AreaWEG0Init();
void AreaWEG0Main(std::uint32_t startParameters);
void AreaWEG0Cleanup(std::int32_t night, std::uint32_t flags);
extern const GameplayAreaDefinition areaWEG0GameplayDefinition;
} // namespace shenmue::s2::areas
