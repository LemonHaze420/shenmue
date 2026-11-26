#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_magic_room.h"
#define QF06_DOOR SHEN_NAME4('R', '1', '4', '0')
#else
#include "s2/area_event.hpp"
#include "common/math.hpp"
#include <array>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name QF06_DOOR{SHEN_NAME(R140)};
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
MagicRoomDescriptor QF06_MAGIC_ROOMS[4] = {
    {{{215.10000610351562f, 126.19999694824219f}, {220.5f, 126.19999694824219f}, {220.5f, 117.0f}, {215.10000610351562f, 117.0f}}, {217.8000030517578f, 120.5999984741211f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{220.5f, 126.19999694824219f}, {225.89999389648438f, 126.19999694824219f}, {225.89999389648438f, 117.0f}, {220.5f, 117.0f}}, {223.1999969482422f, 120.5999984741211f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{231.3000030517578f, 126.19999694824219f}, {236.6999969482422f, 126.19999694824219f}, {236.6999969482422f, 117.0f}, {231.3000030517578f, 117.0f}}, {234.0f, 120.5999984741211f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{225.89999389648438f, 126.19999694824219f}, {231.3000030517578f, 126.19999694824219f}, {231.3000030517578f, 117.0f}, {225.89999389648438f, 117.0f}}, {228.60000610351562f, 120.5999984741211f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
};
MagicRoomFloorGroup QF06_MAGIC_FLOORS[4] = {
    {3, 2, {107.19999694824219f, 110.5f}, QF06_MAGIC_ROOMS},
    {4, 5, {110.80000305175781f, 114.0999984741211f}, QF06_MAGIC_ROOMS},
    {4, 9, {114.4000015258789f, 117.69999694824219f}, QF06_MAGIC_ROOMS},
    {0, 0, {0.0f, 0.0f}, 0},
};
Vector3 QF06_FIRE_POSITIONS[4] = {
#else
inline constexpr std::array<Vector3, 4> QF06_FIRE_POSITIONS{{
#endif
    {239.5f, 122.4f, 139.0f},
    {223.3f, 122.4f, 176.8f},
    {190.0f, 122.4f, 157.0f},
#if defined(SHENMUE_EVENT_OVERLAY)
    {240.0f, 122.4f, 120.0f},
#else
    {240.0f, 122.6f, 120.0f},
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
};
unsigned short QF06_FIRE_PALETTE[16] = {
#else
}};
inline constexpr std::array<std::uint16_t, 16> QF06_FIRE_PALETTE{
#endif
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
#if !defined(SHENMUE_EVENT_OVERLAY)
extern std::uint32_t areaQF06Fire;
void AreaQF06Init();
void AreaQF06Main(std::uint32_t startParameters);
void CreateQF06Fire();
void AreaQF01Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaQF06GameplayDefinition;

} // namespace shenmue::s2::areas

#endif
