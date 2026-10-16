#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQ100Init();
void AreaQ100Main(std::uint32_t startParameters);
void AreaQ100Cleanup(std::int32_t afternoon, std::uint32_t flags);
extern const GameplayAreaDefinition areaQ100GameplayDefinition;
} // namespace shenmue::s2::areas
