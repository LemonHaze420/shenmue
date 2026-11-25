#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0173_AREA SHEN_NAME4('0', '1', '7', '3')
#define AREA_0173_ANP1 SHEN_NAME4('A', 'N', 'P', '1')
#define AREA_0173_WB00 SHEN_NAME4('W', 'B', '0', '0')
#define AREA_0173_SYP_ SHEN_NAME4('S', 'Y', 'P', '_')
#define AREA_0173_CONVERSATION "0173"
extern shen_name AREA_0173_CHARACTERS[];
void E0173Main(void);
void E0173MainCallback(void *);
void QueueE0173Loop(AreaImmediateAuthorTaskParameter *);
void E0173LoopCallback(void *);
void Configure0173(void);
void CleanupE0173(AreaImmediateAuthorTaskParameter *);
void Setup0173Characters(void);
void Restore0173Characters(void);
void Prepare0173Character(shen_name);
void Restore0173Character(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0173_AREA{SHEN_NAME(0173)};
inline constexpr shen_name AREA_0173_ANP1{SHEN_NAME(ANP1)};
inline constexpr shen_name AREA_0173_WB00{SHEN_NAME(WB00)};
inline constexpr shen_name AREA_0173_SYP_{SHEN_NAME(SYP_)};
inline constexpr shen_name AREA_0173_CHARACTERS[]{AREA_0173_SYP_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0173_MOTION[] = "0173A.MOT", AREA_0173_CONVERSATION[] = "0173";

extern const ScriptAreaDefinition area0173ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
