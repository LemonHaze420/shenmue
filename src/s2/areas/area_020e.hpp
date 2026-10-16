#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_020E_AREA{SHEN_NAME(020E)};
inline constexpr shen_name AREA_020E_ANA_{SHEN_NAME(ANA_)};
inline constexpr shen_name AREA_020E_CHARACTERS[]{AREA_020E_ANA_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_020E_MOTION[] = "0020E.MOT", AREA_020E_CONVERSATION[] = "0020";
inline constexpr const char *AREA_020E_DEBUG_NAMES[]{
    "0020E000.c\n", "0020E001.c\n", "0020E002.c\n", "0020E003.c\n", "0020E004.c\n", "0020E005.c\n", "0020E006.c\n", "0020E008.c\n", "0020E009.c\n"};
extern const ScriptAreaDefinition area020EScriptDefinition;

} // namespace shenmue::s2::areas
