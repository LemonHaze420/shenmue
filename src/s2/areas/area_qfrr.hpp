#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaQFRRInit();
void AreaQFRRMain(std::uint32_t startParameters);
void AreaQFRRCleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaQFRRMainLoop(void *taskData);
extern const GameplayAreaDefinition areaQFRRGameplayDefinition;

} // namespace shenmue::s2::areas
