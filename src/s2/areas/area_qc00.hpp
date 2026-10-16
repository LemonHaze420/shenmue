#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaQC00Init();
void AreaQC00Main(std::uint32_t startParameters);
void AreaQC00Cleanup(std::int32_t night, std::uint32_t flags);
extern const GameplayAreaDefinition areaQC00GameplayDefinition;
} // namespace shenmue::s2::areas
