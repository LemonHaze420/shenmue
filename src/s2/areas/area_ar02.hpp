#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaAR02Init();
void AreaAR02Main(std::uint32_t startParameters);
void AreaAR02Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaAR02GameplayDefinition;

} // namespace shenmue::s2::areas
