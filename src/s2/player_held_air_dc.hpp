#pragma once

#include "s2/player_control_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

static const std::size_t kPlayerHeldStatePhaseOffset = 0x389, kPlayerHeldAirTransitionStateOffset = 0x3d0;
std::int32_t GetPlayerHeldAirTransitionState();

} } // namespace shenmue::s2

#endif
