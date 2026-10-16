#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaWR00Init();
void AreaWR00Main(std::uint32_t startParameters);
void AreaWR00Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaWR00GameplayDefinition;
} // namespace shenmue::s2::areas
