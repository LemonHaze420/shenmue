#pragma once

#include "s2/camera_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

std::int32_t camCtrlGetState();
std::uint32_t camCtrlGetFlags();

} } // namespace shenmue::s2

#endif
