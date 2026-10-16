#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0232_AREA{SHEN_NAME(0232)};
inline constexpr shen_name AREA_0232_GAI2{SHEN_NAME(GAI2)};
inline constexpr shen_name AREA_0232_A333{SHEN_NAME(a333)};
inline constexpr shen_name AREA_0232_A339{SHEN_NAME(a339)};
inline constexpr shen_name AREA_0232_A340{SHEN_NAME(a340)};
inline constexpr shen_name AREA_0232_A558{SHEN_NAME(a558)};
inline constexpr shen_name AREA_0232_A210{SHEN_NAME(a210)};
inline constexpr shen_name AREA_0232_A211{SHEN_NAME(a211)};
inline constexpr shen_name AREA_0232_A409{SHEN_NAME(a409)};
inline constexpr shen_name AREA_0232_L701{SHEN_NAME(l701)};
inline constexpr shen_name AREA_0232_L304{SHEN_NAME(l304)};
inline constexpr shen_name AREA_0232_CHARACTERS[]{AREA_0232_GAI2, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0232_MOTION[] = "0232A.MOT", AREA_0232_CONVERSATION[] = "0232";
inline constexpr const char *AREA_0232_DEBUG_NAMES[]{"0232A030.c\n", "0232A070.c\n", "0232A000.c\n"};
extern const ScriptAreaDefinition area0232ScriptDefinition;

} // namespace shenmue::s2::areas
