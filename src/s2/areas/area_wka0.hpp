#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaWKA0Init();
void AreaWKA0Main(std::uint32_t startParameters);
void AreaWKA0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaWKA0GameplayDefinition;

} // namespace shenmue::s2::areas
