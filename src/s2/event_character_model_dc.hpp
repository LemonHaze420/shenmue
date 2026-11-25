#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

void ResetImageModule(Character *character);
void ReplaceScenarioImageModule(shen_name name, const char *imageName, std::int32_t count);
void EV_SetupImageModule(shen_name name, const char *imageName);
bool IsFacialReplacementSelected(const Character *character);
void SelectCharacterFacialNode(Character *character, std::int32_t replacement);
void SetScenarioCharacterType(shen_name name, std::int32_t type);

} } // namespace shenmue::s2

#endif
