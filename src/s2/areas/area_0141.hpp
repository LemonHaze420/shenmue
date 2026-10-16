#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0141_AREA{SHEN_NAME(0141)};
inline constexpr shen_name AREA_0141_A542{SHEN_NAME(a542)};
inline constexpr shen_name AREA_0141_A544{SHEN_NAME(a544)};
inline constexpr shen_name AREA_0141_A048{SHEN_NAME(a048)};
inline constexpr shen_name AREA_0141_A256{SHEN_NAME(a256)};
inline constexpr shen_name AREA_0141_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0141_WB00{SHEN_NAME(WB00)};
inline constexpr shen_name AREA_0141_SIZ_{SHEN_NAME(SIZ_)};
inline constexpr shen_name AREA_0141_CHARACTERS[]{AREA_0141_KUN_, AREA_0141_SIZ_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0141_MOTION[] = "0141A.MOT", AREA_0141_CONVERSATION[] = "0141";
extern std::int32_t e0141Choice;

extern const ScriptAreaDefinition area0141ScriptDefinition;

} // namespace shenmue::s2::areas
