#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0068_AREA{SHEN_NAME(0068)};
inline constexpr shen_name AREA_0068_AA48{SHEN_NAME(aa48)};
inline constexpr shen_name AREA_0068_AA49{SHEN_NAME(aa49)};
inline constexpr shen_name AREA_0068_LKN_{SHEN_NAME(LKN_)};
inline constexpr shen_name AREA_0068_BKN_{SHEN_NAME(BKN_)};
inline constexpr shen_name AREA_0068_MYS_{SHEN_NAME(MYS_)};
inline constexpr shen_name AREA_0068_S01_{SHEN_NAME(S01_)};
inline constexpr shen_name AREA_0068_S02_{SHEN_NAME(S02_)};
inline constexpr shen_name AREA_0068_CHARACTERS[]{AREA_0068_LKN_, AREA_0068_BKN_, AREA_0068_MYS_, AREA_0068_S01_, AREA_0068_S02_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0068_MOTION[] = "0068A.MOT", AREA_0068_CONVERSATION[] = "0068";

inline constexpr char AREA_0068_AUDIO[] = "0068A000";
extern const ScriptAreaDefinition area0068ScriptDefinition;

} // namespace shenmue::s2::areas
