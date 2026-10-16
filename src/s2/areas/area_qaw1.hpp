#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaQAW1Init();
void AreaQAW1Main(std::uint32_t startParameters);
void AreaQAW1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaQAW1MainLoop(void *taskData);
extern const GameplayAreaDefinition areaQAW1GameplayDefinition;

} // namespace shenmue::s2::areas
