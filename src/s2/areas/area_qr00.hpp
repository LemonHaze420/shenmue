#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQR00Init();
void AreaQR00Main(std::uint32_t startParameters);
void AreaQR00Cleanup(std::int32_t night, std::uint32_t flags);
extern const GameplayAreaDefinition areaQR00GameplayDefinition;
} // namespace shenmue::s2::areas
