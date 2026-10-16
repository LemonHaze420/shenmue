#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaWK00Init();
void AreaWK00Main(std::uint32_t startParameters);
void AreaWK00Cleanup(std::int32_t night, std::uint32_t flags);
extern const GameplayAreaDefinition areaWK00GameplayDefinition;
} // namespace shenmue::s2::areas
