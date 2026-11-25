#include "s2/player_held_air_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

// @verified - d
std::int32_t GetPlayerHeldAirTransitionState() {
    const std::int8_t *const model = reinterpret_cast<const std::int8_t *>(g_playerRuntimePointer->model);
    if (model && model[kPlayerHeldStateModeOffset] == 5 && model[kPlayerHeldStatePhaseOffset] == 5)
        return model[kPlayerHeldAirTransitionStateOffset];
    return 0;
}

} } // namespace shenmue::s2

#endif
