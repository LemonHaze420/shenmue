#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaWESMInit();
void AreaWESMMain(std::uint32_t startParameters);
void AreaWESMCleanup(std::int32_t afternoon, std::uint32_t startParameters);
extern const GameplayAreaDefinition areaWESMGameplayDefinition;
} // namespace shenmue::s2::areas
