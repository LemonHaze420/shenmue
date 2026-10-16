#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQKBTInit();
void AreaQKBTMain(std::uint32_t startParameters);
void AreaQKBTCleanup(std::int32_t afternoon, std::uint32_t flags);
extern const GameplayAreaDefinition areaQKBTGameplayDefinition;
} // namespace shenmue::s2::areas
