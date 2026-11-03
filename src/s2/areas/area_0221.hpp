#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"

#define AREA_0221_A209 SHEN_NAME4('a', '2', '0', '9')
#define AREA_0221_A326 SHEN_NAME4('a', '3', '2', '6')
#define AREA_0221_A207 SHEN_NAME4('a', '2', '0', '7')
#define AREA_0221_A208 SHEN_NAME4('a', '2', '0', '8')
#define AREA_0221_WON_ SHEN_NAME4('W', 'O', 'N', '_')
#define AREA_0221_CLJ_ SHEN_NAME4('C', 'L', 'J', '_')
#define AREA_0221_REN SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_0221_QR00 SHEN_NAME4('Q', 'R', '0', '0')
extern const shen_name area0221Characters[];
void E0221Main(void);
void E0221MainCallback(void *taskData);
void QueueE0221Loop(AreaAuthorTaskParameter *parent);
void E0221LoopCallback(void *taskData);
void ConfigureE0221Environment(void);
void CleanupE0221(AreaAuthorTaskParameter *parent);
void SetupE0221Characters(void);
void RestoreE0221Characters(void);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0221_AREA{SHEN_NAME(0221)};
inline constexpr shen_name AREA_0221_A209{SHEN_NAME(a209)};
inline constexpr shen_name AREA_0221_A326{SHEN_NAME(a326)};
inline constexpr shen_name AREA_0221_A207{SHEN_NAME(a207)};
inline constexpr shen_name AREA_0221_A208{SHEN_NAME(a208)};
inline constexpr shen_name AREA_0221_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0221_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0221_CHARACTERS[]{AREA_0221_WON_, AREA_AUTHOR_REN, AREA_0221_CLJ_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0221_MOTION[] = "0221A.MOT", AREA_0221_CONVERSATION[] = "0221";
inline constexpr const char *AREA_0221_DEBUG_NAMES[]{"0221A000.c\n", "0221A010.c\n"};

inline constexpr shen_name AREA_0221_REN{SHEN_NAME(REN_)}, AREA_0221_QR00{SHEN_NAME(QR00)};
extern const ScriptAreaDefinition area0221ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
