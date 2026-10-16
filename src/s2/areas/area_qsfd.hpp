#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
void AreaQSFDInit();
void AreaQSFDMain(std::uint32_t);
void AreaQSFDCleanup(std::int32_t night, std::uint32_t);
extern const GameplayAreaDefinition areaQSFDGameplayDefinition;
} // namespace shenmue::s2::areas
