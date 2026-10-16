#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0146_AREA{SHEN_NAME(0146)};
inline constexpr shen_name AREA_0146_L002{SHEN_NAME(l002)};
inline constexpr shen_name AREA_0146_A308{SHEN_NAME(a308)};
inline constexpr shen_name AREA_0146_A309{SHEN_NAME(a309)};
inline constexpr shen_name AREA_0146_A311{SHEN_NAME(a311)};
inline constexpr shen_name AREA_0146_A312{SHEN_NAME(a312)};
inline constexpr shen_name AREA_0146_A302{SHEN_NAME(a302)};
inline constexpr shen_name AREA_0146_A303{SHEN_NAME(a303)};
inline constexpr shen_name AREA_0146_A295{SHEN_NAME(a295)};
inline constexpr shen_name AREA_0146_A296{SHEN_NAME(a296)};
inline constexpr shen_name AREA_0146_A305{SHEN_NAME(a305)};
inline constexpr shen_name AREA_0146_A304{SHEN_NAME(a304)};
inline constexpr shen_name AREA_0146_A301{SHEN_NAME(a301)};
inline constexpr shen_name AREA_0146_A310{SHEN_NAME(a310)};
inline constexpr shen_name AREA_0146_A306{SHEN_NAME(a306)};
inline constexpr shen_name AREA_0146_A307{SHEN_NAME(a307)};
inline constexpr shen_name AREA_0146_A331{SHEN_NAME(a331)};
inline constexpr shen_name AREA_0146_A332{SHEN_NAME(a332)};
inline constexpr shen_name AREA_0146_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0146_SYG_{SHEN_NAME(SYG_)};
inline constexpr shen_name AREA_0146_TKM_{SHEN_NAME(TKM_)};
inline constexpr shen_name AREA_0146_III_{SHEN_NAME(III_)};
inline constexpr shen_name AREA_0146_SEE_{SHEN_NAME(SEE_)};
inline constexpr shen_name AREA_0146_CHARACTERS[]{AREA_0146_KUN_, AREA_0146_SYG_, AREA_0146_TKM_, AREA_0146_III_, AREA_0146_SEE_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0146_MOTION[] = "0146A.MOT", AREA_0146_CONVERSATION[] = "0146";
inline constexpr const char *AREA_0146_DEBUG_NAMES[]{"0146A000.c\n", "0146A020.c\n"};

inline constexpr char AREA_0146_MEDIA_ZERO[] = "0146A000", AREA_0146_MEDIA_OTHER[] = "0146A020";
extern const ScriptAreaDefinition area0146ScriptDefinition;

} // namespace shenmue::s2::areas
