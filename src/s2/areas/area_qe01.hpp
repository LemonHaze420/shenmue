#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr Vector4 qe01NightGlobalLighting{.x = 0.4196780025959015f, .y = 0.1799314320087433f, .z = 0.2399899959564209f, .w = 0.29980501532554626f};

void AreaQE01Init();
void AreaQE01Main(std::uint32_t startParameters);
void AreaQE01Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void AreaQE01MainLoop(void *taskData);
extern const GameplayAreaDefinition areaQE01GameplayDefinition;

} // namespace shenmue::s2::areas
