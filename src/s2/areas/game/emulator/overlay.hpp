#pragma once

#include "s2/areas/game/arcade.hpp"

namespace shenmue::s2 {

void InitializeEmulatorFont();
void ReleaseEmulatorFont();
void RenderArcadeControllerOverlay(GameArcadeRuntime &runtime);

} // namespace shenmue::s2
