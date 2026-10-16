#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr std::uint32_t ARC0_NIGHT_FLAG = 0xffU;
inline constexpr std::int32_t ARC0_FOG_WEATHER_MASK = 0x780, ARC0_GLOBAL_LIGHTING_MASK = 0x0c;

void AreaARC0Init();
void AreaARC0Main(std::uint32_t startParameters);
void AreaARC0Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void ARC0PostUpdateCallback(void *taskData);
extern const GameplayAreaDefinition areaARC0GameplayDefinition;

} // namespace shenmue::s2::areas
