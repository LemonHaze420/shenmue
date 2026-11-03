#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"

#define AREA_0240_A207 SHEN_NAME4('a', '2', '0', '7')
#define AREA_0240_A209 SHEN_NAME4('a', '2', '0', '9')
#define AREA_0240_A014 SHEN_NAME4('a', '0', '1', '4')
#define AREA_0240_A326 SHEN_NAME4('a', '3', '2', '6')
#define AREA_0240_A045 SHEN_NAME4('a', '0', '4', '5')
#define AREA_0240_A208 SHEN_NAME4('a', '2', '0', '8')
#define AREA_0240_A270 SHEN_NAME4('a', '2', '7', '0')
#define AREA_0240_A806 SHEN_NAME4('a', '8', '0', '6')
#define AREA_0240_A341 SHEN_NAME4('a', '3', '4', '1')
#define AREA_0240_REN_ SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_0240_CHO_ SHEN_NAME4('C', 'H', 'O', '_')
#define AREA_0240_CLJ_ SHEN_NAME4('C', 'L', 'J', '_')
#define AREA_0240_JOP_ SHEN_NAME4('J', 'O', 'P', '_')
#define AREA_0240_LIGHT SHEN_NAME4('l', '0', '0', '1')
#define AREA_0240_QR00 SHEN_NAME4('Q', 'R', '0', '0')
extern const shen_name area0240Characters[];
void E0240Main(void);
void E0240MainCallback(void *taskData);
void QueueE0240Loop(AreaAuthorTaskParameter *parent);
void E0240LoopCallback(void *taskData);
void ConfigureE0240Environment(void);
void CleanupE0240(AreaAuthorTaskParameter *parent);
void SetupE0240Characters(void);
void RestoreE0240Characters(void);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0240_AREA{SHEN_NAME(0240)};
inline constexpr shen_name AREA_0240_A207{SHEN_NAME(a207)};
inline constexpr shen_name AREA_0240_A209{SHEN_NAME(a209)};
inline constexpr shen_name AREA_0240_A014{SHEN_NAME(a014)};
inline constexpr shen_name AREA_0240_A326{SHEN_NAME(a326)};
inline constexpr shen_name AREA_0240_A045{SHEN_NAME(a045)};
inline constexpr shen_name AREA_0240_A208{SHEN_NAME(a208)};
inline constexpr shen_name AREA_0240_A270{SHEN_NAME(a270)};
inline constexpr shen_name AREA_0240_A806{SHEN_NAME(a806)};
inline constexpr shen_name AREA_0240_A341{SHEN_NAME(a341)};
inline constexpr shen_name AREA_0240_CHO_{SHEN_NAME(CHO_)};
inline constexpr shen_name AREA_0240_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0240_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0240_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0240_CHO_, AREA_0240_CLJ_, AREA_0240_JOP_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0240_MOTION[] = "0240A.MOT", AREA_0240_CONVERSATION[] = "0240";
inline constexpr const char *AREA_0240_DEBUG_NAMES[]{"0240A000.c\n"};

inline constexpr shen_name AREA_0240_LIGHT{SHEN_NAME(l001)}, AREA_0240_QR00{SHEN_NAME(QR00)};
extern const ScriptAreaDefinition area0240ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
