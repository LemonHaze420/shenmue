#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaKRK6Init();
void AreaKRK6Main(std::uint32_t startParameters);
void AreaKRK6Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKRK6GameplayDefinition;

} // namespace shenmue::s2::areas
