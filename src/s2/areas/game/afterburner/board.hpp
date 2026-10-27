#pragma once
#include "s2/areas/game/arcade.hpp"
#include <cstdint>
namespace shenmue::s2 {
extern std::int32_t g_afterBurnerInputMode;
void InitializeAfterBurnerBoard(GameArcadeRuntime &runtime, const std::uint8_t *rankings);
void UpdateAfterBurnerBoard(GameArcadeRuntime &runtime);
void CopyAfterBurnerRankings(std::uint8_t *rankings);
} // namespace shenmue::s2
