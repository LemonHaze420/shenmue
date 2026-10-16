#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQ300Init();
void AreaQ300Main(std::uint32_t startParameters);
void AreaQ300Cleanup(std::int32_t night, std::uint32_t flags);
extern const GameplayAreaDefinition areaQ300GameplayDefinition;
} // namespace shenmue::s2::areas
