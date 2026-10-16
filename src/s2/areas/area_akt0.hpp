#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaAKT0Init();
void AreaAKT0Main(std::uint32_t startParameters);
void AreaAKT0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaAKTSharedCleanup(std::int32_t afternoon);
extern const GameplayAreaDefinition areaAKT0GameplayDefinition;

} // namespace shenmue::s2::areas
