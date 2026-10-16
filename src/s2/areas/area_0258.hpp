#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0258_AREA{SHEN_NAME(0258)};
inline constexpr shen_name AREA_0258_A428{SHEN_NAME(a428)};
inline constexpr shen_name AREA_0258_A118{SHEN_NAME(a118)};
inline constexpr shen_name AREA_0258_CHN_{SHEN_NAME(CHN_)};
inline constexpr shen_name AREA_0258_CYC_{SHEN_NAME(CYC_)};
inline constexpr shen_name AREA_0258_EEN_{SHEN_NAME(EEN_)};
inline constexpr shen_name AREA_0258_SFA_{SHEN_NAME(SFA_)};
inline constexpr shen_name AREA_0258_SF6_{SHEN_NAME(SF6_)};
inline constexpr shen_name AREA_0258_CHARACTERS[]{
    AREA_0258_CHN_, AREA_0258_CYC_, AREA_AUTHOR_REN, AREA_0258_EEN_, AREA_0258_SFA_, AREA_0258_SF6_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0258_MOTION[] = "0258A.MOT", AREA_0258_CONVERSATION[] = "0258";
inline constexpr const char *AREA_0258_DEBUG_NAMES[]{"0258A000.c\n", "0258A005.c\n", "0258A020.c\n"};

extern const ScriptAreaDefinition area0258ScriptDefinition;

} // namespace shenmue::s2::areas
