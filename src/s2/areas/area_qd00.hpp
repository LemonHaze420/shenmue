#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQD00Init();
void AreaQD00Main(std::uint32_t);
void AreaQD00Cleanup(std::int32_t, std::uint32_t);
extern const GameplayAreaDefinition areaQD00GameplayDefinition;
} // namespace shenmue::s2::areas
