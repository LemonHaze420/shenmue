#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_011E_AREA{SHEN_NAME(011E)};
inline constexpr shen_name AREA_011E_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_011E_GET_{SHEN_NAME(GET_)};
inline constexpr shen_name AREA_011E_DEY_{SHEN_NAME(DEY_)};
inline constexpr shen_name AREA_011E_AGO_{SHEN_NAME(AGO_)};
inline constexpr shen_name AREA_011E_EEN_{SHEN_NAME(EEN_)};
inline constexpr shen_name AREA_011E_SEE_{SHEN_NAME(SEE_)};
inline constexpr shen_name AREA_011E_MEY_{SHEN_NAME(MEY_)};
inline constexpr shen_name AREA_011E_DYU_{SHEN_NAME(DYU_)};
inline constexpr shen_name AREA_011E_CHS_{SHEN_NAME(CHS_)};
inline constexpr shen_name AREA_011E_HLN_{SHEN_NAME(HLN_)};
inline constexpr shen_name AREA_011E_A317{SHEN_NAME(a317)};
inline constexpr shen_name AREA_011E_A318{SHEN_NAME(a318)};
inline constexpr shen_name AREA_011E_A319{SHEN_NAME(a319)};
inline constexpr shen_name AREA_011E_KBOU{SHEN_NAME(KBOU)};
inline constexpr shen_name AREA_011E_SALA{SHEN_NAME(SALA)};
inline constexpr shen_name AREA_011E_CHARACTERS[]{AREA_011E_WON_, AREA_011E_GET_, AREA_011E_DEY_, AREA_011E_AGO_, AREA_011E_EEN_, AREA_011E_SEE_,
    AREA_011E_MEY_, AREA_011E_DYU_, AREA_011E_CHS_, AREA_011E_HLN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_011E_MOTION[] = "0011E.MOT", AREA_011E_CONVERSATION[] = "0011";
inline constexpr const char *AREA_011E_DEBUG_NAMES[]{"0011E120.c\n", "0011E121.c\n", "0011E122.c\n", "0011E131.c\n", "0011E020.c\n", "0011E021.c\n",
    "0011E022.c\n", "0011E132.c\n", "0011E032.c\n", "0011E000.c\n", "0011E001.c\n", "0011E002.c\n", "0011E003.c\n", "0011E050.c\n", "0011E051.c\n",
    "0011E052.c\n", "0011E061.c\n", "0011E062.c\n", "0011E040.c\n", "0011E041.c\n", "0011E042.c\n", "0011E090.c\n", "0011E004.c\n", "0011E005.c\n",
    "0011E130.c\n"};

extern const ScriptAreaDefinition area011EScriptDefinition;

} // namespace shenmue::s2::areas
