#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name QB00_RYO{SHEN_NAME(RYO_)};
inline constexpr std::uint32_t QB00_NIGHT_FLAG = 0xffU, QB00_FOG_COLOUR = 0xff000000U;
inline constexpr std::int32_t QB00_FOG_WEATHER_MASK = 0x780, QB00_GLOBAL_LIGHTING_MASK = 0x0c;

void AreaQB00Init();
void AreaQB00Main(std::uint32_t startParameters);
void AreaQB00Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaQB00MainLoop(void *taskData);
extern const GameplayAreaDefinition areaQB00GameplayDefinition;

} // namespace shenmue::s2::areas
