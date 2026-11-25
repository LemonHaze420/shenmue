#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include <array>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr const char *QTB4_AUDIO_BANK = "QTB4_000";
inline constexpr std::array<Vector3, 4> QTB4_FIRE_POSITIONS{{
#define QTB4_FIRE_POSITION(index, posX, posY, posZ) {.x = posX, .y = posY, .z = posZ},
#include "qtb4_fire_positions.def"
#undef QTB4_FIRE_POSITION
}};
extern std::uint32_t areaQTB4Fire;
void AreaQTB4Init();
void AreaQTB4Main(std::uint32_t startParameters);
void AreaQTB4Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
void CreateQTB4Fire();
extern const GameplayAreaDefinition areaQTB4GameplayDefinition;

} // namespace shenmue::s2::areas
