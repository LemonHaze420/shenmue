#pragma once

#include "s2/character_control_update_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

static const float kCharacterGroundProbeHeight = 1.5f, kCharacterGroundRaisedThreshold = 0.1f;
static const float kCharacterGroundDeltaLimit = 0.015f, kCharacterGroundBlendDistance = 0.3f;
static const float kCharacterGroundLiftScale = 0.75f, kCharacterGroundFullLiftThreshold = 0.7f, kCharacterGroundDescentBlend = 0.05f;

} } // namespace shenmue::s2

#endif
