#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0222_AREA{SHEN_NAME(0222)};
inline constexpr shen_name AREA_0222_YAN_{SHEN_NAME(YAN_)};
inline constexpr shen_name AREA_0222_AA22{SHEN_NAME(aa22)};
inline constexpr shen_name AREA_0222_A052{SHEN_NAME(a052)};
inline constexpr shen_name AREA_0222_A053{SHEN_NAME(a053)};
inline constexpr shen_name AREA_0222_A212{SHEN_NAME(a212)};
inline constexpr shen_name AREA_0222_A095{SHEN_NAME(a095)};
inline constexpr shen_name AREA_0222_A316{SHEN_NAME(a316)};
inline constexpr shen_name AREA_0222_QR00{SHEN_NAME(QR00)};
inline constexpr shen_name AREA_0222_QAAA{SHEN_NAME(QAAA)};
inline constexpr shen_name AREA_0222_YAN1{SHEN_NAME(YAN1)};
inline constexpr shen_name AREA_0222_YAN2{SHEN_NAME(YAN2)};
inline constexpr shen_name AREA_0222_YAN3{SHEN_NAME(YAN3)};
inline constexpr shen_name AREA_0222_CHARACTERS[]{AREA_0222_YAN_, AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0222_MOTION[] = "0222A.MOT", AREA_0222_CONVERSATION[] = "0222";
inline constexpr shen_name AREA_0222_AUTHORS[]{AREA_0222_YAN1, AREA_0222_YAN2, AREA_0222_YAN3};
extern const ScriptAreaDefinition area0222ScriptDefinition;

} // namespace shenmue::s2::areas
