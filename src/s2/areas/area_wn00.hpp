#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaWN00Init();
void AreaWN00Main(std::uint32_t startParameters);
void AreaWN00Cleanup(std::int32_t night, std::uint32_t flags);
extern const GameplayAreaDefinition areaWN00GameplayDefinition;
} // namespace shenmue::s2::areas
