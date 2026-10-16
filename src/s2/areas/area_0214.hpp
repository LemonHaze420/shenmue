#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0214_AREA{SHEN_NAME(0214)};
inline constexpr shen_name AREA_0214_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0214_L101{SHEN_NAME(l101)};
inline constexpr shen_name AREA_0214_A329{SHEN_NAME(a329)};
inline constexpr shen_name AREA_0214_A530{SHEN_NAME(a530)};
inline constexpr shen_name AREA_0214_CHARACTERS[]{AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0214_MOTION[] = "0214A.MOT", AREA_0214_CONVERSATION[] = "0214";
inline constexpr const char *AREA_0214_DEBUG_NAMES[]{"0214A000.c.c\n", "0214A010.c.c\n"};

void SetupE0214Characters();
void RestoreE0214Characters();
void E0214LoopCallback(void *taskData);

extern const ScriptAreaDefinition area0214ScriptDefinition;

} // namespace shenmue::s2::areas
