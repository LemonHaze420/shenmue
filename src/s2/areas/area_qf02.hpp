#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQF02Init();
void AreaQF02Main(std::uint32_t startParameters);
void AreaQF02Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQF02GameplayDefinition;
} // namespace shenmue::s2::areas
