#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0035_AREA{SHEN_NAME(0035)};
inline constexpr shen_name AREA_0035_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0035_AB04{SHEN_NAME(ab04)};
inline constexpr shen_name AREA_0035_A058{SHEN_NAME(a058)};
inline constexpr shen_name AREA_0035_A404{SHEN_NAME(a404)};
inline constexpr shen_name AREA_0035_A405{SHEN_NAME(a405)};
inline constexpr shen_name AREA_0035_A403{SHEN_NAME(a403)};
inline constexpr shen_name AREA_0035_A406{SHEN_NAME(a406)};
inline constexpr shen_name AREA_0035_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0035_KNK_{SHEN_NAME(KNK_)};
inline constexpr shen_name AREA_0035_CHARACTERS[]{AREA_0035_KUN_, AREA_0035_SYE_, AREA_0035_KNK_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0035_MOTION[] = "0035A.MOT", AREA_0035_CONVERSATION[] = "0035";
inline constexpr const char *AREA_0035_DEBUG_NAMES[]{"0035A000.c\n", "0035A060.c\n", "0035A070.c\n", "0035A080.c\n", "0035A050.c\n"};

extern const ScriptAreaDefinition area0035ScriptDefinition;

} // namespace shenmue::s2::areas
