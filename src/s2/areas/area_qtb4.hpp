#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include <array>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr const char *QTB4_AUDIO_BANK = "QTB4_000";
inline constexpr std::array<Vector3, 4> QTB4_FIRE_POSITIONS{{
    {.x = 224.01f, .y = 100.7f, .z = 128.51f},
    {.x = 223.9f, .y = 100.7f, .z = 137.69f},
    {.x = 243.46f, .y = 100.7f, .z = 128.72f},
    {.x = 243.46f, .y = 100.7f, .z = 137.66f},
}};
extern std::uint32_t areaQTB4Fire;
void AreaQTB4Init();
void AreaQTB4Main(std::uint32_t startParameters);
void AreaQTB4Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void CreateQTB4Fire();
extern const GameplayAreaDefinition areaQTB4GameplayDefinition;

} // namespace shenmue::s2::areas
