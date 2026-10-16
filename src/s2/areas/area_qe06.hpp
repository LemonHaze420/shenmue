#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

void AreaQE06Init();
void AreaQE06Main(std::uint32_t startParameters);
void AreaQE06MainLoop(void *taskData);
extern const GameplayAreaDefinition areaQE06GameplayDefinition;

} // namespace shenmue::s2::areas
