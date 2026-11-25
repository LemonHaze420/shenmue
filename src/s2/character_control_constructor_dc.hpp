#pragma once

#include "s2/character_control_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

static const std::uint32_t kCharacterControlInitialFlags = 0x40000000U, kCharacterControlPlayerFlags = 0x04400200U;
static const std::size_t kCharacterControlGroundHalfwordsOffset = 0x48, kCharacterControlDimensionsOffset = 0x4c;
static const float kCharacterControlDefaultDimension = 0.4f;
void ResetCharacterControlModel(CharacterControlModel *control);
void CCM_Callback(Character *character, void *data);
std::int32_t AllocateCCM(Character *character);
Character *ctrlGetRootNode();

} } // namespace shenmue::s2

#endif
