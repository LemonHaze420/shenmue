#pragma once

#include "s2/area_event.hpp"
#include <cstdint>

namespace shenmue::s2::areas {

void AreaKMZ2Init();
void AreaKMZ2Main(std::uint32_t startParameters);
void AreaKMZ2Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKMZ2GameplayDefinition;

} // namespace shenmue::s2::areas
