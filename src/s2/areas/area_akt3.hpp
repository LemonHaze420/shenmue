#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaAKT3Init();
void AreaAKT3Main(std::uint32_t startParameters);
void AreaAKT3Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAKT3GameplayDefinition;

} // namespace shenmue::s2::areas
