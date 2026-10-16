#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
void AreaQJBTInit();
void AreaQJBTMain(std::uint32_t);
void AreaQJBTCleanup(std::int32_t, std::uint32_t);
extern const GameplayAreaDefinition areaQJBTGameplayDefinition;
} // namespace shenmue::s2::areas
