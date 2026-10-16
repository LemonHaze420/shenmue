#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaKSH1Init();
void AreaKSH1Main(std::uint32_t startParameters);
void AreaKSH1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKSH1GameplayDefinition;

} // namespace shenmue::s2::areas
