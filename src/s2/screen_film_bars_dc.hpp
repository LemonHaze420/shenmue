#pragma once

#include "s2/screen_film_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

const float CINEMA_BAR_HEIGHT = 64.0f;
void filmSetCinemaBarVisibility(HLib::Task *task, FilmControlFlag mode, std::int32_t frames);
float GetCinemaBarHeight(HLib::Task *task);

} } // namespace shenmue::s2

#endif
