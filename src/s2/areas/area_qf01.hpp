#pragma once
#include "s2/events/event_magic_room.h"
#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
static MagicRoomDescriptor QF01_MAGIC_ROOMS[] = {
#else
inline constexpr MagicRoomDescriptor QF01_MAGIC_ROOMS[]{
#endif
    {{{215.100006f, 126.199997f}, {220.5f, 126.199997f}, {220.5f, 117.0f}, {215.100006f, 117.0f}},
        {217.800003f, 120.599998f},
        {5.4000001f, 7.19999981f},
        0,
        0,
        0},
    {{{220.5f, 126.199997f}, {225.899994f, 126.199997f}, {225.899994f, 117.0f}, {220.5f, 117.0f}},
        {223.199997f, 120.599998f},
        {5.4000001f, 7.19999981f},
        0,
        0,
        0},
};
#if defined(SHENMUE_EVENT_OVERLAY)
static MagicRoomFloorGroup QF01_MAGIC_ROOM_FLOORS[] = {
#else
inline constexpr MagicRoomFloorGroup QF01_MAGIC_ROOM_FLOORS[]{
#endif
    {2, 0, {100.0f, 103.5f}, QF01_MAGIC_ROOMS},
    {0, 0, {0, 0}, 0},
};

#if !defined(SHENMUE_EVENT_OVERLAY)
void AreaQF01Init();
void AreaQF01Main(std::uint32_t startParameters);
extern const GameplayAreaDefinition areaQF01GameplayDefinition;

} // namespace shenmue::s2::areas
#endif
