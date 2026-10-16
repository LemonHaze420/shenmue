#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

void AreaKRM3Init();
void AreaKRM3Main(std::uint32_t startParameters);
void AreaKRM3Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKRM3GameplayDefinition;

} // namespace shenmue::s2::areas
