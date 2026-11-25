#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0077_AREA SHEN_NAME4('0', '0', '7', '7')
#define AREA_0077_JOP_ SHEN_NAME4('J', 'O', 'P', '_')
#define AREA_0077_A396 SHEN_NAME4('a', '3', '9', '6')
#define AREA_0077_A456 SHEN_NAME4('a', '4', '5', '6')
#define AREA_0077_WN00 SHEN_NAME4('W', 'N', '0', '0')
#define AREA_0077_LIGHT SHEN_NAME4('0', '0', '0', '0')
#define AREA_0077_AUTHOR0 SHEN_NAME4('0', '7', '7', 'A')
#define AREA_0077_AUTHOR1 SHEN_NAME4('0', '7', '7', 'B')
#define AREA_0077_CONVERSATION "0077"
extern shen_name AREA_0077_CHARACTERS[];
void E0077Main(void);
void E0077MainCallback(void *);
void E0077LoopCallback(void *);
void QueueE0077Loop(AreaAuthorTaskParameter *);
void ConfigureE0077Environment(void);
void CleanupE0077(AreaAuthorTaskParameter *);
void SetupE0077Characters(void);
void RestoreE0077Characters(void);
void PrepareE0077Character(shen_name);
void RestoreE0077Character(shen_name);
void HideE0077Prop(shen_name);
void ShowE0077Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0077_AREA{SHEN_NAME(0077)};
inline constexpr shen_name AREA_0077_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0077_A396{SHEN_NAME(a396)};
inline constexpr shen_name AREA_0077_A456{SHEN_NAME(a456)};
inline constexpr shen_name AREA_0077_CHARACTERS[]{AREA_0077_JOP_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0077_MOTION[] = "0077A.MOT", AREA_0077_CONVERSATION[] = "0077";
inline constexpr const char *AREA_0077_DEBUG_NAMES[]{"0077A000.c\n", "0077B000.c\n"};

inline constexpr shen_name AREA_0077_WN00{SHEN_NAME(WN00)}, AREA_0077_LIGHT{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0077_AUTHORS[]{SHEN_NAME(077A), SHEN_NAME(077B)};
extern const ScriptAreaDefinition area0077ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
