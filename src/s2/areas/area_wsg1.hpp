#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaWSG1Init();
void AreaWSG1Main(std::uint32_t startParameters);
void AreaWSG1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaWSG1GameplayDefinition;

} // namespace shenmue::s2::areas
