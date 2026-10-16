#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaWET0Init();
void AreaWET0Main(std::uint32_t startParameters);
void AreaWET0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaWET0GameplayDefinition;

} // namespace shenmue::s2::areas
