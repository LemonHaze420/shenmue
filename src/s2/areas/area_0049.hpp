#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#define AREA_0049_AREA SHEN_NAME4('0', '0', '4', '9')
#define AREA_0049_SYE_ SHEN_NAME4('S', 'Y', 'E', '_')
#define AREA_0049_A092 SHEN_NAME4('a', '0', '9', '2')
#define AREA_0049_WB00 SHEN_NAME4('W', 'B', '0', '0')
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0049_CONVERSATION "0049"
extern shen_name AREA_0049_CHARACTERS[];
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0049_AREA{SHEN_NAME(0049)};
inline constexpr shen_name AREA_0049_SYE_{SHEN_NAME(SYE_)};
inline constexpr shen_name AREA_0049_A092{SHEN_NAME(a092)};
inline constexpr shen_name AREA_0049_WB00{SHEN_NAME(WB00)};
inline constexpr shen_name AREA_0049_CHARACTERS[]{AREA_0049_SYE_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0049_MOTION[] = "0049A.MOT", AREA_0049_CONVERSATION[] = "0049";

extern const ScriptAreaDefinition area0049ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
