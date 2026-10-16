#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQC01Init();
void AreaQC01Main(std::uint32_t startParameters);
void AreaQC01Cleanup(std::int32_t afternoon, std::uint32_t flags);
extern const GameplayAreaDefinition areaQC01GameplayDefinition;
} // namespace shenmue::s2::areas
