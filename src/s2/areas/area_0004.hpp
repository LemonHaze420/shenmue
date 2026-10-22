#pragma once

#include "s2/area_event.hpp"

#include <array>
#include <cstdint>
namespace shenmue::s2::areas {

inline constexpr std::array<std::uint16_t, 5> OPENING_TEXTURE_PHASE_OFFSETS{0, 10, 7, 12, 5};
inline constexpr std::array<const char *, 11> OPENING_AUTHOR_DEBUG_NAMES{"0004A001.c\n", "0004A089.c\n", "0004A090.c\n", "0004A091.c\n", "0004A092.c\n",
    "0004A093.c\n", "0004A094.c\n", "0004A095.c\n", "0004A096.c\n", "0004A097.c\n", "0004A098.c\n"};
extern const ScriptAreaDefinition area0004ScriptDefinition;

} // namespace shenmue::s2::areas
