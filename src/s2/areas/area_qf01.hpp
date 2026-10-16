#pragma once

#include "s2/area_event.hpp"
#include "s2/magic_room.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr MagicRoomDescriptor QF01_MAGIC_ROOMS[]{
    {.quad = {{215.100006f, 126.199997f}, {220.5f, 126.199997f}, {220.5f, 117.0f}, {215.100006f, 117.0f}},
        .center = {217.800003f, 120.599998f},
        .size = {5.4000001f, 7.19999981f},
        .yaw = 0,
        .reserved32 = 0,
        .type = 0},
    {.quad = {{220.5f, 126.199997f}, {225.899994f, 126.199997f}, {225.899994f, 117.0f}, {220.5f, 117.0f}},
        .center = {223.199997f, 120.599998f},
        .size = {5.4000001f, 7.19999981f},
        .yaw = 0,
        .reserved32 = 0,
        .type = 0},
};
inline constexpr MagicRoomFloorGroup QF01_MAGIC_ROOM_FLOORS[]{
    {.entryCount = 2, .reserved04 = 0, .heightRange = {100.0f, 103.5f}, .entries = QF01_MAGIC_ROOMS},
    {.entryCount = 0, .reserved04 = 0, .heightRange = {}, .entries = nullptr},
};

void AreaQF01Init();
void AreaQF01Main(std::uint32_t startParameters);
extern const GameplayAreaDefinition areaQF01GameplayDefinition;

} // namespace shenmue::s2::areas
