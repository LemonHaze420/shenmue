#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
void AreaQSFCInit();
void AreaQSFCMain(std::uint32_t);
void AreaQSFCCleanup(std::int32_t night, std::uint32_t);
extern const GameplayAreaDefinition areaQSFCGameplayDefinition;
} // namespace shenmue::s2::areas
