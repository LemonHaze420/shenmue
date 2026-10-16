#pragma once

#include "s2/areas/game_arcade.hpp"

namespace shenmue::s2 {

void InitializeOutRunSound();
void UpdateOutRunSound(const GameArcadeRuntime &runtime);
void ReleaseOutRunSound();

} // namespace shenmue::s2
