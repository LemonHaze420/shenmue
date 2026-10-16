#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
void AreaQTB1Init();
void AreaQTB1Main(std::uint32_t);
void AreaQTB1Cleanup(std::int32_t night, std::uint32_t);
extern const GameplayAreaDefinition areaQTB1GameplayDefinition;
} // namespace shenmue::s2::areas
