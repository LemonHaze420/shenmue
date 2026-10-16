#pragma once
#include "common/types.hpp"
#include "common/math.hpp"
#include <array>
namespace shenmue::s2::alley_battle {
inline constexpr std::array<shen_name, 16> characters{{shen_name{1599035730U}, shen_name{1598117448U}, shen_name{1598637908U}, shen_name{1598246483U},
    shen_name{1598898515U}, shen_name{1599687756U}, shen_name{1598704707U}, shen_name{1598443077U}, shen_name{1599756628U}, shen_name{1597059144U},
    shen_name{1597124680U}, shen_name{1597190216U}, shen_name{1597255752U}, shen_name{1597321288U}, shen_name{1597386824U}, shen_name{4294967295U}}};
inline constexpr std::array<shen_name, 9> authoredCharacters{{shen_name{1598117448U}, shen_name{1598637908U}, shen_name{1598246483U}, shen_name{1598898515U},
    shen_name{1599687756U}, shen_name{1598704707U}, shen_name{1598443077U}, shen_name{1599756628U}, shen_name{4294967295U}}};
struct Reinforcement {
    std::uint16_t hp;
    std::uint8_t style;
    std::uint16_t animation;
    Vector3 position;
    std::uint16_t yaw;
};
inline constexpr std::array<Reinforcement, 14> reinforcements{
    {{100, 3, 3636, {254.02f, 96.849998f, 490.84f}, 115}, {100, 4, 3640, {253.92f, 96.849998f, 494.84f}, 35},
        {100, 5, 3644, {251.32001f, 96.849998f, 496.23999f}, 0}, {100, 6, 3648, {248.12f, 96.849998f, 494.54001f}, 285},
        {100, 2, 3632, {247.92f, 96.849998f, 490.94f}, 215}, {100, 3, 3636, {249.02f, 96.849998f, 489.44f}, 240},
        {100, 4, 3640, {253.22f, 96.849998f, 489.54001f}, 125}, {100, 5, 3644, {254.82001f, 96.849998f, 492.73999f}, 80},
        {100, 6, 3648, {253.12f, 96.849998f, 496.44f}, 20}, {100, 2, 3632, {248.92f, 96.849998f, 496.23999f}, 340},
        {100, 3, 3636, {254.83f, 96.849998f, 489.54001f}, 125}, {100, 4, 3640, {253.02f, 96.849998f, 495.84f}, 20},
        {100, 5, 3644, {252.92f, 96.849998f, 489.54001f}, 170}, {100, 6, 3648, {253.72f, 96.849998f, 490.64001f}, 90}}};
} // namespace shenmue::s2::alley_battle
