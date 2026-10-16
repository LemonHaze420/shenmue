#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQLBTInit();
void AreaQLBTMain(std::uint32_t startParameters);
void AreaQLBTCleanup(std::int32_t afternoon, std::uint32_t flags);
extern const GameplayAreaDefinition areaQLBTGameplayDefinition;
} // namespace shenmue::s2::areas
