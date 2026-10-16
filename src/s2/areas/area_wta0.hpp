#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
void AreaWTA0Init();
void AreaWTA0Main(std::uint32_t startParameters);
void AreaWTA0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaWTA0GameplayDefinition;
} // namespace shenmue::s2::areas
