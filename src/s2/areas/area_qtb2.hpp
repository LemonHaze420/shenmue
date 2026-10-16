#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQTB2Init();
void AreaQTB2Main(std::uint32_t);
void AreaQTB2Cleanup(std::int32_t afternoon, std::uint32_t);
extern const GameplayAreaDefinition areaQTB2GameplayDefinition;
} // namespace shenmue::s2::areas
