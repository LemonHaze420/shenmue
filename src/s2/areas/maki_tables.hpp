#pragma once
#include "s2/areas/area_maki.hpp"
#include "s2/event_characters.hpp"
#include "s2/search.hpp"

namespace shenmue::s2::areas {
struct MakiFirewoodPlacement {
    std::int32_t type;
    Vector3 position;
    Vector3i rotation;
    std::int32_t facing;
};
static_assert(sizeof(MakiFirewoodPlacement) == 0x20);
inline constexpr MakiFirewoodPlacement makiFirewoodPlacements[]{
    {.type = 0, .position = {.x = 161.1f, .y = 100, .z = 384.5f}, .rotation = {.x = 0, .y = 44964, .z = 0}, .facing = 0},
    {.type = 0, .position = {.x = 158.7f, .y = 97.9f, .z = 374.8f}, .rotation = {.x = 0, .y = 6826, .z = 0}, .facing = 1},
    {.type = 0, .position = {.x = 160.8f, .y = 97.9f, .z = 375}, .rotation = {.x = 0, .y = 0, .z = 0}, .facing = 0},
    {.type = 0, .position = {.x = 160.9f, .y = 97.9f, .z = 373.5f}, .rotation = {.x = 0, .y = 0, .z = 0}, .facing = 0},
    {.type = 0, .position = {.x = 165.3f, .y = 99.2f, .z = 385.3f}, .rotation = {.x = 0, .y = 17294, .z = 0}, .facing = 1},
    {.type = 0, .position = {.x = 167.8f, .y = 99.5f, .z = 382.9f}, .rotation = {.x = 0, .y = 1820, .z = 0}, .facing = 1},
    {.type = 0, .position = {.x = 162.8f, .y = 97.9f, .z = 374.7f}, .rotation = {.x = 0, .y = 24576, .z = 0}, .facing = 0},
    {.type = 0, .position = {.x = 170.6f, .y = 96.4f, .z = 377.2f}, .rotation = {.x = 0, .y = 0, .z = 0}, .facing = 0},
    {.type = 0, .position = {.x = 171.7f, .y = 96.4f, .z = 378.5f}, .rotation = {.x = 0, .y = 0, .z = 0}, .facing = 0},
    {.type = 0, .position = {.x = 172.4f, .y = 96.4f, .z = 380.8f}, .rotation = {.x = 0, .y = 30492, .z = 0}, .facing = 1},
    {.type = 0, .position = {.x = 176.9f, .y = 96.5f, .z = 370.3f}, .rotation = {.x = 0, .y = 1820, .z = 0}, .facing = 1},
    {.type = 1, .position = {.x = 161.2f, .y = 97.9f, .z = 377.2f}, .rotation = {.x = 0, .y = 25486, .z = 0}, .facing = 1},
    {.type = 1, .position = {.x = 168, .y = 99.3f, .z = 387.6f}, .rotation = {.x = 0, .y = 27306, .z = 0}, .facing = 1},
    {.type = 1, .position = {.x = 169.3f, .y = 96.4f, .z = 373.9f}, .rotation = {.x = 0, .y = 1820, .z = 0}, .facing = 1},
    {.type = 1, .position = {.x = 181.7f, .y = 96.4f, .z = 371.9f}, .rotation = {.x = 0, .y = 0, .z = 0}, .facing = 0},
    {.type = 1, .position = {.x = 172.3f, .y = 96.4f, .z = 377.3f}, .rotation = {.x = 0, .y = 0, .z = 0}, .facing = 0},
    {.type = 2, .position = {.x = 179.4f, .y = 97.84f, .z = 387.9f}, .rotation = {.x = 0, .y = 0, .z = 0}, .facing = 0},
    {.type = 2, .position = {.x = 177.2f, .y = 96.4f, .z = 380.7f}, .rotation = {.x = 0, .y = 0, .z = 0}, .facing = 0},
    {.type = 3, .position = {.x = 181.81f, .y = 96.4f, .z = 372.1f}, .rotation = {.x = 0, .y = 0, .z = 0}, .facing = 0}};
inline constexpr shen_name MAKI_CLING{SHEN_NAME(CLIN)}, MAKI_SMALL_LOG{SHEN_NAME(CMK0)}, MAKI_MIDDLE_LOG{SHEN_NAME(CMK1)}, MAKI_LARGE_LOG{SHEN_NAME(CMK2)};
inline constexpr shen_name makiReadCharacters[]{MAKI_SHENHUA, EVENT_CHARACTER_INVALID};
inline constexpr shen_name makiHeldLogs[]{SHEN_NAME(MAK0), SHEN_NAME(MAK1), SHEN_NAME(MAK2), SHEN_NAME(MAK3), SHEN_NAME(MAK4), SHEN_NAME(MAK5), SHEN_NAME(MAK6),
    SHEN_NAME(MAK7), SHEN_NAME(MAK8), SHEN_NAME(MAK9), SHEN_NAME(MAKA), SHEN_NAME(MAKB), SHEN_NAME(MAKC), SHEN_NAME(MAKD), SHEN_NAME(MAKE), SHEN_NAME(MAKF)};
inline constexpr Vector3 makiHeldLogPositions[]{{.x = .1f, .y = 0, .z = .02f}, {.x = .06f, .y = 0, .z = .02f}, {.x = .1f, .y = 0, .z = .05f},
    {.x = .07f, .y = 0, .z = .05f}, {.x = .13f, .y = 0, .z = .05f}, {.x = .04f, .y = 0, .z = .05f}, {.x = .1f, .y = 0, .z = .07f},
    {.x = .06f, .y = 0, .z = .08f}, {.x = .02f, .y = 0, .z = .08f}, {.x = .14f, .y = 0, .z = .08f}, {.x = .09f, .y = 0, .z = .1f},
    {.x = .05f, .y = 0, .z = .1f}, {.x = .11f, .y = 0, .z = .1f}, {.x = .02f, .y = 0, .z = .11f}, {.x = .1f, .y = 0, .z = .12f},
    {.x = .07f, .y = 0, .z = .12f}};
inline constexpr Vector3 makiHeldLogAngles[]{{.x = 62, .y = 90, .z = 120}, {.x = 67.5f, .y = 90, .z = 112.5f}, {.x = 76, .y = 90, .z = 121.3f},
    {.x = 61.3f, .y = 90, .z = 116.3f}, {.x = 71.3f, .y = 69.9f, .z = 126.3f}, {.x = 81.4f, .y = 74.9f, .z = 126.3f}, {.x = 61.4f, .y = 84.9f, .z = 106.3f},
    {.x = 61.3f, .y = 89.9f, .z = 111.3f}, {.x = 61.3f, .y = 99.9f, .z = 116.3f}, {.x = 66.3f, .y = 74.9f, .z = 126.3f}, {.x = 66.3f, .y = 74.9f, .z = 116.3f},
    {.x = 66.3f, .y = 87.9f, .z = 121.2f}, {.x = 315, .y = 75, .z = 0}, {.x = 339.9f, .y = 85, .z = 35}, {.x = 305, .y = 80, .z = 0},
    {.x = 310, .y = 90, .z = 0}};
inline std::uint32_t makiCarryHandCommand[]{0, 726, 63534, 0, 0, 182, 0, 62623, 7098, 0, 0, 61349, 0, 0, 60622, 0, 0, 62805, 0, 546, 1820, 0, 0, 62441, 0, 0,
    58437, 0, 0, 62441, 0, 546, 2001, 0, 0, 58801, 0, 0, 60440, 0, 0, 63352, 0, 546, 1818, 0, 0, 55344, 0, 0, 64626, 0, 63898, 62805, 0, 1456, 1091, 0};
inline std::uint32_t makiSingleLogHandCommand[]{0, 726, 63534, 0, 4731, 65353, 0, 63170, 3821, 0, 0, 56800, 0, 0, 60440, 0, 1091, 56982, 0, 546, 1820, 0, 0,
    58801, 0, 0, 54252, 0, 0, 59893, 0, 546, 1456, 0, 0, 56435, 0, 0, 56071, 0, 0, 59166, 0, 546, 908, 0, 0, 52978, 0, 0, 56800, 0, 0, 59348, 0, 546, 1091, 0};
inline std::uint32_t makiLighterHandCommand[]{0, 363, 58983, 0, 2001, 64444, 0, 62623, 4547, 0, 44, 54213, 0, 902, 56326, 0, 0, 60257, 0, 546, 956, 0, 0, 60106,
    0, 0, 53261, 0, 0, 59893, 0, 546, 909, 0, 0, 54434, 0, 0, 52614, 0, 0, 59166, 0, 546, 1455, 0, 0, 48792, 0, 0, 56800, 0, 0, 57892, 0, 546, 1091, 0};
} // namespace shenmue::s2::areas
