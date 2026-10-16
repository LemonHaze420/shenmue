#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaQAW6Init();
void AreaQAW6Main(std::uint32_t startParameters);
void AreaQAW6Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaQAW6MainLoop(void *taskData);
extern const GameplayAreaDefinition areaQAW6GameplayDefinition;

} // namespace shenmue::s2::areas
