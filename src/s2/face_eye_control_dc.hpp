#pragma once

#include "s2/face_controller_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

static const float FACIAL_EYE_OUTER_SCALE = 1.8f, FACIAL_EYE_INNER_SCALE = 0.2f;
void AT_SetEyeCtrl(Character *character, float transitionFrames, float leftWeight, float rightWeight);

} } // namespace shenmue::s2

#endif
