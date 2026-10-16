#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaKMZ1Init();
void AreaKMZ1Main(std::uint32_t startParameters);
void AreaKMZ1Cleanup(std::int32_t afternoon, std::uint32_t flags);
extern const GameplayAreaDefinition areaKMZ1GameplayDefinition;
} // namespace shenmue::s2::areas
