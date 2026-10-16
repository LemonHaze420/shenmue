#pragma once

#include "s2/hlib.hpp"
#include "s2/legacy_sprite.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TASK_FQSP{SHEN_NAME(FQSP)};
inline constexpr shen_name TASK_FGSP{SHEN_NAME(FGSP)};
struct BattleQteSpriteDimensions {
    std::int32_t width, height;
    float scale, depth;
};
inline constexpr std::array<BattleQteSpriteDimensions, 13> battleQteSpriteDimensions{{
    {.width = 64, .height = 64, .scale = 1.0f, .depth = 3.0f},
    {.width = 64, .height = 64, .scale = 1.0f, .depth = 3.0f},
    {.width = 64, .height = 64, .scale = 1.0f, .depth = 3.0f},
    {.width = 64, .height = 64, .scale = 1.0f, .depth = 3.0f},
    {.width = 64, .height = 64, .scale = 1.0f, .depth = 3.0f},
    {.width = 64, .height = 64, .scale = 1.0f, .depth = 4.0f},
    {.width = 128, .height = 128, .scale = 1.0f, .depth = 1.0f},
    {.width = 16, .height = 16, .scale = 1.0f, .depth = 2.5f},
    {.width = 16, .height = 16, .scale = 1.0f, .depth = 2.0f},
    {.width = 64, .height = 128, .scale = 1.0f, .depth = 1.0f},
    {.width = 16, .height = 16, .scale = 1.0f, .depth = 2.0f},
    {.width = 16, .height = 16, .scale = 1.0f, .depth = 2.5f},
    {.width = 64, .height = 64, .scale = 1.0f, .depth = 2.0f},
}};
inline constexpr std::array<std::uint32_t, 5> battleQteSpriteColours{0x4040ffU, 0x10c010U, 0, 0xf01010U, 0xc0c0c0U};

std::int64_t LoadFBQTE();
void FreeFBQTE();
std::int64_t fbLoadGauge();
void FreeBattleGaugeSprites();
struct FightLoadSpriteRecord {
    std::int32_t textureIndex, width, height;
};
extern std::array<FightLoadSpriteRecord, 6> g_fightLoadSprites;
extern std::array<std::int32_t, 24> g_fightLoadDigits;
std::int64_t SetupLoadSprite();
void FreeLoadSprites();
void DrawFightLoadDigit(std::int32_t index);
void ControlFightLoadSprites(std::int32_t mode, std::int32_t index);
void DrawBattleQteSprite(
    std::uint8_t icon, std::int32_t x, std::int32_t y, float scale, float alpha, std::int32_t rotation, std::int32_t flags, float depth, ScreenMode mode);
void EV_FBQTE_Util(std::int32_t mode, std::int16_t icon, float x, float y, std::int32_t scaleBits, float alpha, std::int32_t rotation, std::int32_t flags);

} // namespace shenmue::s2
