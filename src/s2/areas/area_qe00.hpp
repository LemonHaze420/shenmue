#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQE00Init();
void AreaQE00Main(std::uint32_t startParameters);
void AreaQE00Cleanup(std::int32_t night, std::uint32_t flags);
extern const GameplayAreaDefinition areaQE00GameplayDefinition;
} // namespace shenmue::s2::areas
