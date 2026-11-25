#pragma once

#include "s2/camera_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {
void camCtrlSetNextRoll(std::int16_t roll);
} } // namespace shenmue::s2

#endif
