#pragma once

#include "s2/area_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr std::int32_t QAXX_FOG_EFFECT_MASK = 0x780;
inline constexpr std::int32_t QAXX_LIGHTING_UPDATE_MASK = 0x0f;
inline constexpr std::int32_t QAXX_SCENE_LIGHTING_MASK = 0x0c;
inline constexpr std::int32_t QAXX_WEATHER_CLOUD = 0, QAXX_WEATHER_RAIN = 1;

void AreaQAXXInit();
void AreaQAXXMainLoop(void *taskData);
void AreaQAXXMain(std::uint32_t startParameters);
void AreaQAXXCleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQAXXGameplayDefinition;

} // namespace shenmue::s2::areas
