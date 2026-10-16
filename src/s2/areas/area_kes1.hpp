#pragma once

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include <array>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr Vector3 KES1_FIRE_POSITION{66.2300034f, 101.5f, 139.139999f};
inline constexpr Vector3 KES1_SECOND_FIRE_POSITION{53.9799995f, 96.1999969f, 117.300003f};
inline constexpr std::array<std::uint16_t, 16> KES1_FIRE_PALETTE{
    785,
    785,
    1041,
    1297,
    1554,
    1810,
    2610,
    2866,
    2882,
    3152,
    3440,
    3696,
    3712,
    4000,
    4035,
    4054,
};

void AreaKES1Init();
void AreaKES1Main(std::uint32_t startParameters);
void AreaKES1Cleanup(std::int32_t afternoon, std::uint32_t areaIndex);
extern const GameplayAreaDefinition areaKES1GameplayDefinition;

} // namespace shenmue::s2::areas
