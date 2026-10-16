#pragma once

#include "s2/area_event.hpp"
#include <cstdint>

namespace shenmue::s2::areas {

void AreaKMZ3Init();
void AreaKMZ3Main(std::uint32_t startParameters);
void AreaKMZ3Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKMZ3GameplayDefinition;

} // namespace shenmue::s2::areas
