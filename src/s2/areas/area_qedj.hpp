#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaQEDJInit();
void AreaQEDJMain(std::uint32_t startParameters);
void AreaQEDJCleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaQEDJMainLoop(void *taskData);
extern const GameplayAreaDefinition areaQEDJGameplayDefinition;

} // namespace shenmue::s2::areas
