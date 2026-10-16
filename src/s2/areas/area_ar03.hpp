#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaAR03Init();
void AreaAR03Main(std::uint32_t startParameters);
void AreaAR03Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAR03GameplayDefinition;

} // namespace shenmue::s2::areas
