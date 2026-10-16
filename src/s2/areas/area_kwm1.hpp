#pragma once

#include "s2/area_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name TASK_KWM1{SHEN_NAME(kwm1)};
inline constexpr shen_name TASK_KWM1_WAVE{SHEN_NAME(wave)};
void AreaKWM1Init();
void AreaKWM1Main(std::uint32_t startParameters);
void AreaKWM1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKWM1GameplayDefinition;

} // namespace shenmue::s2::areas
