#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name TASK_KRK4_WAVE{SHEN_NAME(wave)};

void AreaKRK4Init();
void AreaKRK4Main(std::uint32_t startParameters);
void AreaKRK4Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaKRK4WaveCallback(void *taskData);
extern const GameplayAreaDefinition areaKRK4GameplayDefinition;

} // namespace shenmue::s2::areas
