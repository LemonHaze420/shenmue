#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0028_AREA{SHEN_NAME(0028)};
inline constexpr shen_name AREA_0028_A040{SHEN_NAME(a040)};
inline constexpr shen_name AREA_0028_A041{SHEN_NAME(a041)};
inline constexpr shen_name AREA_0028_AA18{SHEN_NAME(aa18)};
inline constexpr shen_name AREA_0028_SOS_{SHEN_NAME(SOS_)};
inline constexpr shen_name AREA_0028_M01_{SHEN_NAME(M01_)};
inline constexpr shen_name AREA_0028_SHI_{SHEN_NAME(SHI_)};
inline constexpr shen_name AREA_0028_Y01_{SHEN_NAME(Y01_)};
inline constexpr shen_name AREA_0028_TAO_{SHEN_NAME(TAO_)};
inline constexpr shen_name AREA_0028_SYO_{SHEN_NAME(SYO_)};
inline constexpr shen_name AREA_0028_Y05_{SHEN_NAME(Y05_)};
inline constexpr shen_name AREA_0028_CHARACTERS[]{
    AREA_0028_SOS_, AREA_0028_M01_, AREA_0028_SHI_, AREA_0028_Y01_, AREA_0028_TAO_, AREA_0028_SYO_, AREA_0028_Y05_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0028_MOTION[] = "0028A.MOT", AREA_0028_CONVERSATION[] = "0028";
inline constexpr const char *AREA_0028_DEBUG_NAMES[]{"0028A000.c\n", "0028A015.c\n", "0028A001.c\n"};

extern const ScriptAreaDefinition area0028ScriptDefinition;

} // namespace shenmue::s2::areas
