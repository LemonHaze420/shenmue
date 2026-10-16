#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaKWW4Init();
void AreaKWW4Main(std::uint32_t startParameters);
void AreaKWW4Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKWW4GameplayDefinition;

} // namespace shenmue::s2::areas
