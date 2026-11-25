#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

#define CHARACTER_ALPHA_MANAGER SHEN_NAME4('C', 'P', 'A', 'M')
static const std::int32_t characterAlphaHoldFrames = 45;
static const float characterAlphaFadeStep = 0.05f;
struct CharacterAlphaState {
    std::int32_t holdFrames;
};
typedef char Layout_character_alpha_dc_0[(sizeof(CharacterAlphaState) == 4) ? 1 : -1];
void UpdateCharacterAlpha(Character *character, void *payload);
void AttachCharacterAlphaComponent(Character *character);
std::int32_t cpamSetFlag(Character *character, std::int32_t enabled);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool shenCheckCharaHasAlphaManager(const Character *character);
#endif

} } // namespace shenmue::s2

#endif
