#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_REN SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0080_AREA SHEN_NAME4('0', '0', '8', '0')
#define AREA_0080_AA48 SHEN_NAME4('a', 'a', '4', '8')
#define AREA_0080_AKY0 SHEN_NAME4('A', 'K', 'Y', '0')
#define AREA_0080_0000 SHEN_NAME4('0', '0', '0', '0')
#define AREA_0080_CONVERSATION "0080"
typedef struct E0080LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
} E0080LoopParameter;
extern shen_name AREA_0080_CHARACTERS[];
void E0080Main(void);
void E0080MainCallback(void *);
void QueueE0080Loop(AreaImmediateAuthorTaskParameter *);
void E0080LoopCallback(void *);
void ConfigureE0080Environment(void);
void CleanupE0080(AreaImmediateAuthorTaskParameter *);
void Setup0080Characters(void);
void Restore0080Characters(void);
void Prepare0080Character(shen_name);
void Restore0080Character(shen_name);
void Hide0080Prop(shen_name);
void Show0080Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0080_AREA{SHEN_NAME(0080)};
inline constexpr shen_name AREA_0080_AA48{SHEN_NAME(aa48)};
inline constexpr shen_name AREA_0080_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0080_AKY0{SHEN_NAME(AKY0)};
inline constexpr shen_name AREA_0080_CHARACTERS[]{AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0080_MOTION[] = "0080A.MOT", AREA_0080_CONVERSATION[] = "0080";

extern const ScriptAreaDefinition area0080ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
