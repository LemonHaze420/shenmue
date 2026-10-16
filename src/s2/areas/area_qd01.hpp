#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQD01Init();
void AreaQD01Main(std::uint32_t startParameters);
void AreaQD01Cleanup(std::int32_t afternoon, std::uint32_t flags);
extern const GameplayAreaDefinition areaQD01GameplayDefinition;
} // namespace shenmue::s2::areas
