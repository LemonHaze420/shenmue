#pragma once

#include "s2/screen_film_bars_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

void EV_SetCinemaBarVisibility(FilmControlFlag mode, std::int32_t frames);

} } // namespace shenmue::s2

#endif
