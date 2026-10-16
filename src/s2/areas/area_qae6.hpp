#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaQAE6Init();
void AreaQAE6Main(std::uint32_t startParameters);
void AreaQAE6Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaQAE6MainLoop(void *taskData);
extern const GameplayAreaDefinition areaQAE6GameplayDefinition;

} // namespace shenmue::s2::areas
