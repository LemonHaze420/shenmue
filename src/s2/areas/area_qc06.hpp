#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaQC06Init();
void AreaQC06Main(std::uint32_t startParameters);
void AreaQC06Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaQC06MainLoop(void *taskData);
extern const GameplayAreaDefinition areaQC06GameplayDefinition;

} // namespace shenmue::s2::areas
