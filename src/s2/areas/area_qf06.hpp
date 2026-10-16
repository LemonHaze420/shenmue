#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include <array>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name QF06_DOOR{SHEN_NAME(R140)};
inline constexpr std::array<Vector3, 4> QF06_FIRE_POSITIONS{{
    {.x = 239.5f, .y = 122.4f, .z = 139.0f},
    {.x = 223.3f, .y = 122.4f, .z = 176.8f},
    {.x = 190.0f, .y = 122.4f, .z = 157.0f},
    {.x = 240.0f, .y = 122.6f, .z = 120.0f},
}};
inline constexpr std::array<std::uint16_t, 16> QF06_FIRE_PALETTE{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
extern std::uint32_t areaQF06Fire;
void AreaQF06Init();
void AreaQF06Main(std::uint32_t startParameters);
void CreateQF06Fire();
void AreaQF01Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQF06GameplayDefinition;

} // namespace shenmue::s2::areas
