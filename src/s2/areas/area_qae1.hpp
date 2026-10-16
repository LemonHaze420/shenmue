#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaQAE1Init();
void AreaQAE1Main(std::uint32_t startParameters);
void AreaQAE1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaQAE1MainLoop(void *taskData);
extern const GameplayAreaDefinition areaQAE1GameplayDefinition;

} // namespace shenmue::s2::areas
