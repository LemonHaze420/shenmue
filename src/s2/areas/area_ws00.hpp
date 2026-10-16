#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaWS00Init();
void AreaWS00Main(std::uint32_t startParameters);
void AreaWS00Cleanup(std::int32_t night, std::uint32_t flags);
extern const GameplayAreaDefinition areaWS00GameplayDefinition;
} // namespace shenmue::s2::areas
