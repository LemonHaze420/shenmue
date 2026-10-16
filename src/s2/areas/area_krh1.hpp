#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaKRH1Init();
void AreaKRH1Main(std::uint32_t startParameters);
void AreaKRH1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKRH1GameplayDefinition;

} // namespace shenmue::s2::areas
