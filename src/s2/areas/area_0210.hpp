#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0210_AREA{SHEN_NAME(0210)};
inline constexpr shen_name AREA_0210_A200{SHEN_NAME(a200)};
inline constexpr shen_name AREA_0210_A201{SHEN_NAME(a201)};
inline constexpr shen_name AREA_0210_CHR_{SHEN_NAME(CHR_)};
inline constexpr shen_name AREA_0210_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0210_CHR_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0210_MOTION[] = "0210A.MOT", AREA_0210_CONVERSATION[] = "0210";
inline constexpr const char *AREA_0210_DEBUG_NAMES[]{
    "0210A000.c\n", "0210A010.c\n", "0210A020.c\n", "0210A030.c\n", "0210A040.c\n", "0210A050.c\n", "0210A060.c\n", "0210A035.c\n"};

extern const ScriptAreaDefinition area0210ScriptDefinition;

} // namespace shenmue::s2::areas
