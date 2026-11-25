#pragma once

#include "s2/scenario_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

shen_name CreateEventCharacter(std::int32_t type, const std::byte *data, shen_name name);
void SetCharaTargetTransformOffset(shen_name name, const Vector3 *factor, std::int32_t rotation);
void SetMapControlStatusFlag(std::int32_t bit, std::int32_t enabled);
std::uint32_t SetScenarioCharacterFlag40(shen_name name, std::int32_t enabled);
void EV_ToggleCharacterFlag(shen_name name);
void mtwkSetMotionworkScale3(shen_name name, const Vector3 *scale);
void EV_SetCharacterImageSelection(shen_name name, std::int32_t selection);

} // namespace s2
} // namespace shenmue

#endif
