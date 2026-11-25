#pragma once

#include "s2/weather_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {
void SetWeatherInstruction(std::uint32_t flags);
} } // namespace shenmue::s2

#endif
