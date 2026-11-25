#pragma once

#include "s2/character_yaw_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

void SetCharacterWorldPosition(Character *character, const Vector3 *position);

} } // namespace shenmue::s2

#endif
