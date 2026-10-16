#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaWB01Init();
void AreaWB01Main(std::uint32_t startParameters);
void AreaWB01Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaWB01GameplayDefinition;
} // namespace shenmue::s2::areas
