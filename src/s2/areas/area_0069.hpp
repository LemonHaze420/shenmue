#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0069_AREA{SHEN_NAME(0069)};
inline constexpr shen_name AREA_0069_KNK_{SHEN_NAME(KNK_)};
inline constexpr shen_name AREA_0069_A250{SHEN_NAME(a250)};
inline constexpr shen_name AREA_0069_A266{SHEN_NAME(a266)};
inline constexpr shen_name AREA_0069_A251{SHEN_NAME(a251)};
inline constexpr shen_name AREA_0069_A264{SHEN_NAME(a264)};
inline constexpr shen_name AREA_0069_A252{SHEN_NAME(a252)};
inline constexpr shen_name AREA_0069_A265{SHEN_NAME(a265)};
inline constexpr shen_name AREA_0069_A253{SHEN_NAME(a253)};
inline constexpr shen_name AREA_0069_A263{SHEN_NAME(a263)};
inline constexpr shen_name AREA_0069_A254{SHEN_NAME(a254)};
inline constexpr shen_name AREA_0069_A262{SHEN_NAME(a262)};
inline constexpr shen_name AREA_0069_A255{SHEN_NAME(a255)};
inline constexpr shen_name AREA_0069_A261{SHEN_NAME(a261)};
inline constexpr shen_name AREA_0069_CHARACTERS[]{AREA_0069_KNK_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0069_MOTION[] = "0069A.MOT", AREA_0069_CONVERSATION[] = "0069";
inline constexpr const char *AREA_0069_DEBUG_NAMES[]{
    "0050A000.c\n", "0050A020.c\n", "0050A030.c\n", "0050A040.c\n", "0050A050.c\n", "0050A060.c\n", "0050A070.c\n"};

extern const ScriptAreaDefinition area0069ScriptDefinition;

} // namespace shenmue::s2::areas
