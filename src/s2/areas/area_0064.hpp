#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0064_AREA{SHEN_NAME(0064)};
inline constexpr shen_name AREA_0064_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0064_KNK_{SHEN_NAME(KNK_)};
inline constexpr shen_name AREA_0064_KSM_{SHEN_NAME(KSM_)};
inline constexpr shen_name AREA_0064_KSY_{SHEN_NAME(KSY_)};
inline constexpr shen_name AREA_0064_CHARACTERS[]{AREA_0064_SYE_, AREA_0064_KNK_, AREA_0064_KSM_, AREA_0064_KSY_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0064_MOTION[] = "0064A.MOT", AREA_0064_CONVERSATION[] = "0064";
inline constexpr const char *AREA_0064_DEBUG_NAMES[]{
    "0064A000.c\n", "0064A020.c\n", "0064A040.c\n", "0064A060.c\n", "0064A070.c\n", "0064A080.c\n", "0064A010.c\n"};
extern const ScriptAreaDefinition area0064ScriptDefinition;

} // namespace shenmue::s2::areas
