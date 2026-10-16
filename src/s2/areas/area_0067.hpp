#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0067_AREA{SHEN_NAME(0067)};
inline constexpr shen_name AREA_0067_KEK_{SHEN_NAME(KEK_)};
inline constexpr shen_name AREA_0067_IKO_{SHEN_NAME(IKO_)};
inline constexpr shen_name AREA_0067_MOK_{SHEN_NAME(MOK_)};
inline constexpr shen_name AREA_0067_A073{SHEN_NAME(a073)};
inline constexpr shen_name AREA_0067_CHARACTERS[]{AREA_0067_KEK_, AREA_0067_IKO_, AREA_0067_MOK_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0067_MOTION[] = "0067A.MOT", AREA_0067_CONVERSATION[] = "0067";
inline constexpr const char *AREA_0067_DEBUG_NAMES[]{"0067A000.c\n", "0067A020.c\n", "0067A030.c\n", "0067A040.c\n"};

inline constexpr char AREA_0067_AUDIO[] = "0067A000";
extern const ScriptAreaDefinition area0067ScriptDefinition;

} // namespace shenmue::s2::areas
