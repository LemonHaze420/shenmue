#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

std::int32_t GetCharacterWorldYaw(const Character *character);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetCharacterLocalYaw(const Character *character);
#endif
void SetCharacterWorldYaw(Character *character, std::int32_t yaw);

} // namespace s2
} // namespace shenmue

#endif
