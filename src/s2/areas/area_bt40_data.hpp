#pragma once
#include "common/types.hpp"
#include "common/math.hpp"
#include <array>
namespace shenmue::s2::bt40 {
inline constexpr shen_name ryo{SHEN_NAME(RYO_)}, joy{SHEN_NAME(JOP_)}, base{SHEN_NAME(BASE)};
inline constexpr shen_name characters[]{ryo, joy, SHEN_NAME(KRE_), SHEN_NAME(BF2_), SHEN_NAME(BF4_), SHEN_NAME(BF8_), SHEN_NAME(BFA_), shen_name{0xffffffffU}};
inline constexpr shen_name challengeCharacters[]{ryo, joy, shen_name{0xffffffffU}};
inline constexpr float difficultyScale[]{1, .95f, .9f, .85f, .8f, .75f, .7f, .65f, .6f, .55f, .5f, .45f, .4f, .35f, .3f, .25f};
inline constexpr int difficultyFlags[]{2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
inline constexpr std::array<Vector3, 7> battlePositions{{{257.29998779296875f, 244, 134.2899932861328f}, {250.52999877929688f, 244, 130.5f},
    {259.4599914550781f, 244, 135.05999755859375f}, {256.17999267578125f, 244, 133.22000122070312f}, {257.6199951171875f, 244, 136.0800018310547f},
    {258.1499938964844f, 244, 133.0800018310547f}, {256.05999755859375f, 244, 135.22999572753906f}}};
inline constexpr int battleYaw[]{0xc000, 0xb555, 0x31c7, 0xa38e, 0x1555, 0x6aaa, 0xd555};
inline constexpr shen_name walkAreas[]{SHEN_NAME(Q400), SHEN_NAME(Q401), SHEN_NAME(Q402), SHEN_NAME(Q403), SHEN_NAME(Q404)};
inline constexpr int walkYaw[]{36408, 54613, 22755, 12743, 2730};
inline constexpr Vector3 leavePosition{264, 245.66000366210938f, 136.69000244140625f};
} // namespace shenmue::s2::bt40
