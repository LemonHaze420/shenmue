#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name Q200_EVENT_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name Q200_FOUNTAIN_TASK{SHEN_NAME(fntn)};
void AreaQ200Init();
void AreaQ200Main(std::uint32_t startParameters);
void AreaQ200Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQ200GameplayDefinition;

} // namespace shenmue::s2::areas
