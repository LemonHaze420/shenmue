#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0218_AREA{SHEN_NAME(0218)};
inline constexpr shen_name AREA_0218_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0218_A274{SHEN_NAME(a274)};
inline constexpr shen_name AREA_0218_A275{SHEN_NAME(a275)};
inline constexpr shen_name AREA_0218_TGY_{SHEN_NAME(TGY_)};
inline constexpr shen_name AREA_0218_CHARACTERS[]{AREA_0218_TGY_, AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0218_MOTION[] = "0218A.MOT", AREA_0218_CONVERSATION[] = "0218";
inline constexpr const char *AREA_0218_DEBUG_NAMES[]{"0218A000.c.c\n", "0218A030.c.c\n"};

extern const ScriptAreaDefinition area0218ScriptDefinition;

} // namespace shenmue::s2::areas
