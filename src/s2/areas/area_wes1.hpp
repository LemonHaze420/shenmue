#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaWES1Init();
void AreaWES1Main(std::uint32_t startParameters);
void AreaWES1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaWES1GameplayDefinition;

} // namespace shenmue::s2::areas
