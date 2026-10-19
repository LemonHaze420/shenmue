#pragma once

#include "common/types.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name AREA_ENTITY_BASE{SHEN_NAME(cwbs)};
inline constexpr shen_name AREA_ENTITY_PACKAGE{SHEN_NAME(CHAW)};
struct AreaEntityPlacement {
    shen_name area;
    Vector3 position;
    std::int32_t yaw;
    std::int32_t field14;
    std::int32_t field18;
    shen_name character;
};
static_assert(sizeof(AreaEntityPlacement) == 0x20);
static_assert(offsetof(AreaEntityPlacement, yaw) == 0x10);
static_assert(offsetof(AreaEntityPlacement, character) == 0x1c);
inline constexpr AreaEntityPlacement AREA_ENTITY_PLACEMENTS[]{
    {.area = SHEN_NAME(WS00), .position = {61.5999985f, 100.099998f, 181.550003f}, .yaw = 49152, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cw00)},
    {.area = SHEN_NAME(WS00), .position = {35.9500008f, 100.099998f, 176.600006f}, .yaw = 24576, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cw10)},
    {.area = SHEN_NAME(WN00), .position = {218.5f, 104.5f, 212.559998f}, .yaw = 57344, .field14 = 1, .field18 = 0, .character = SHEN_NAME(cw20)},
    {.area = SHEN_NAME(WN00), .position = {212.5f, 104.5f, 197.649994f}, .yaw = 40960, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cw30)},
    {.area = SHEN_NAME(WN00), .position = {264.899994f, 104.5f, 204.5f}, .yaw = 49152, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cw40)},
    {.area = SHEN_NAME(WN00), .position = {190.350006f, 104.599998f, 224.0f}, .yaw = 32768, .field14 = 0, .field18 = 2, .character = SHEN_NAME(cw50)},
    {.area = SHEN_NAME(WN00), .position = {204.300003f, 104.599998f, 207.800003f}, .yaw = 24576, .field14 = 0, .field18 = 2, .character = SHEN_NAME(cw60)},
    {.area = SHEN_NAME(WN00), .position = {235.550003f, 104.5f, 202.199997f}, .yaw = 32768, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cw70)},
    {.area = SHEN_NAME(WN00), .position = {220.320007f, 104.599998f, 206.960007f}, .yaw = 40960, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cw80)},
    {.area = SHEN_NAME(WT00), .position = {264.350006f, 115.300003f, 330.299988f}, .yaw = 16384, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cw90)},
    {.area = SHEN_NAME(WR00), .position = {178.600006f, 102.25f, 192.300003f}, .yaw = 60074, .field14 = 0, .field18 = 1, .character = SHEN_NAME(cwA0)},
    {.area = SHEN_NAME(AR02), .position = {257.420013f, 101.0f, 398.320007f}, .yaw = 64807, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cwB0)},
    {.area = SHEN_NAME(AR03), .position = {303.420013f, 100.150002f, 478.380005f}, .yaw = 0, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cwC0)},
    {.area = SHEN_NAME(AR02), .position = {244.800003f, 100.0f, 425.100006f}, .yaw = 32768, .field14 = 0, .field18 = 1, .character = SHEN_NAME(cwD0)},
    {.area = SHEN_NAME(AKS0), .position = {268.700012f, 100.0f, 260.700012f}, .yaw = 32768, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cwE0)},
    {.area = SHEN_NAME(AKS1), .position = {269.200012f, 106.074997f, 262.75f}, .yaw = 32768, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cwF0)},
    {.area = SHEN_NAME(Q300), .position = {264.899994f, 100.089996f, 195.800003f}, .yaw = 55887, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cwG0)},
    {.area = SHEN_NAME(Q300), .position = {249.970001f, 100.089996f, 197.149994f}, .yaw = 0, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cwH0)},
    {.area = SHEN_NAME(Q100), .position = {188.75f, 100.0f, 80.4000015f}, .yaw = 15109, .field14 = 0, .field18 = 1, .character = SHEN_NAME(cwI0)},
    {.area = SHEN_NAME(Q300), .position = {262.200012f, 100.0f, 161.75f}, .yaw = 58254, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cwJ0)},
    {.area = SHEN_NAME(Q100), .position = {176.199997f, 100.0f, 191.429993f}, .yaw = 57963, .field14 = 0, .field18 = 0, .character = SHEN_NAME(cwK0)},
    {.area = SHEN_NAME(WKA0), .position = {57.7000008f, 103.373001f, 312.0f}, .yaw = 24576, .field14 = 0, .field18 = 0, .character = SHEN_NAME(0000)},
};


struct AreaEntityPositionImage {
    Vector3 positions[3];
    shen_name terminator;
    std::uint32_t adjacentFlagIds[2];
};
static_assert(sizeof(AreaEntityPositionImage) == 0x30);
inline constexpr AreaEntityPositionImage AREA_ENTITY_POSITION_IMAGE{
    .positions = {{-0.15f, 0.75f, 0.3f}, {-0.15f, 0.75f, 0.4f}, {-0.15f, 0.75f, 0.5f}},
    .terminator = SHEN_NAME(END_),
    .adjacentFlagIds = {2225, 2226},
};
void areaClearAreaEntities();
void PlaceAreaEntityGroup(std::int32_t placementIndex);
void PlaceAreaEntity(std::int32_t placementIndex, std::int32_t characterIndex);

} // namespace shenmue::s2
