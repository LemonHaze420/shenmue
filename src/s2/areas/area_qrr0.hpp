#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
void AreaQRR0Init();
void AreaQRR0Main(std::uint32_t startParameters);
void AreaQRR0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQRR0GameplayDefinition;
} // namespace shenmue::s2::areas
