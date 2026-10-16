#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaAK00Init();
void AreaAK00Main(std::uint32_t startParameters);
void AreaAK00Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAK00GameplayDefinition;

} // namespace shenmue::s2::areas
