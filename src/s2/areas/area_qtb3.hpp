#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQTB3Init();
void AreaQTB3Main(std::uint32_t startParameters);
void AreaQTB3Cleanup(std::int32_t night, std::uint32_t flags);
extern const GameplayAreaDefinition areaQTB3GameplayDefinition;
} // namespace shenmue::s2::areas
