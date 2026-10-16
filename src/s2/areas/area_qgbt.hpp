#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
void AreaQGBTInit();
void AreaQGBTMain(std::uint32_t);
void AreaQGBTCleanup(std::int32_t night, std::uint32_t);
extern const GameplayAreaDefinition areaQGBTGameplayDefinition;
} // namespace shenmue::s2::areas
