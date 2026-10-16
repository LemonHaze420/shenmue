#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaARM0Init();
void AreaARM0Main(std::uint32_t startParameters);
void AreaARM0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaARM0GameplayDefinition;

} // namespace shenmue::s2::areas
