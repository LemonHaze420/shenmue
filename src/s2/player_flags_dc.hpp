#pragma once

#include "s2/player_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {
std::uint32_t plymAddFlags(std::uint32_t flags);
std::uint32_t plymRemoveFlags(std::uint32_t flags);
} } // namespace shenmue::s2

#endif
