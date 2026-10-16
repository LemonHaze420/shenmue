#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaARZ0Init();
void AreaARZ0Main(std::uint32_t startParameters);
void AreaARZ0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaARZ0GameplayDefinition;

} // namespace shenmue::s2::areas
