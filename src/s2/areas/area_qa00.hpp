#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQA00Init();
void AreaQA00Main(std::uint32_t startParameters);
void AreaQA00Cleanup(std::int32_t night, std::uint32_t flags);
extern const GameplayAreaDefinition areaQA00GameplayDefinition;
} // namespace shenmue::s2::areas
