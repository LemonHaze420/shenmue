#include "s2/map_event_character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/pad_control_dc.hpp"
#include "s2/player_state_dc.hpp"

namespace shenmue {
namespace s2 {

void SetCharacterLayer(Character *character, std::int32_t layer);

PadControlEntry *GetMapPadControlEntry();

// @verified - d
std::int32_t IsPlayerControlEnabled() {
    return (static_cast<std::int32_t>(g_playerRuntimePointer->flags) >> 1) & 1;
}

} // namespace s2
} // namespace shenmue

#endif
