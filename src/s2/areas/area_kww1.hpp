#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaKWW1Init();
void AreaKWW1Main(std::uint32_t startParameters);
void AreaKWW1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKWW1GameplayDefinition;

} // namespace shenmue::s2::areas
