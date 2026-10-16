#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
void AreaWSY0Init();
void AreaWSY0Main(std::uint32_t startParameters);
void AreaWSY0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaWSY0GameplayDefinition;
} // namespace shenmue::s2::areas
