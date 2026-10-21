#pragma once

#include <cstdint>

namespace shenmue::s2 {



void BeginGameArcadeFrame();
void BindGameArcadeTarget(std::uint32_t texture);
void DrawGameHangOnFrame(std::uint32_t texture);

} // namespace shenmue::s2
