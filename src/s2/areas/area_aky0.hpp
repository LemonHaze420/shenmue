#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaAKY0Init();
void AreaAKY0Main(std::uint32_t startParameters);
void AreaAKY0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAKY0GameplayDefinition;

} // namespace shenmue::s2::areas
