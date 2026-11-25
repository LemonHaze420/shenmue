#pragma once

#include "s2/character_control_constructor_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

static const float kCharacterControlGravity = -0.010896279f, kCharacterControlSnapTolerance = 0.03f;
static const float kCharacterControlCollisionBlend = 0.8f, kCharacterControlBicycleRadius = 0.9f;
static const float kCharacterControlAlternateProbeHeight = 1.5f;
void ccmUpdateRootGroundHeight(CharacterControlModel *control);

} } // namespace shenmue::s2

#endif
