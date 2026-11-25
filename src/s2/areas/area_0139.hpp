#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0139_AREA SHEN_NAME4('0', '1', '3', '9')
#define AREA_0139_AR03 SHEN_NAME4('A', 'R', '0', '3')
#define AREA_0139_SYN_ SHEN_NAME4('S', 'Y', 'N', '_')
#define AREA_0139_A450 SHEN_NAME4('a', '4', '5', '0')
#define AREA_0139_0000 SHEN_NAME4('0', '0', '0', '0')
#define AREA_0139_CONVERSATION "0139"
extern shen_name AREA_0139_CHARACTERS[];
void E0139Main(void);
void E0139MainCallback(void *);
void QueueE0139Loop(AreaImmediateAuthorTaskParameter *);
void E0139LoopCallback(void *);
void ConfigureE0139Environment(void);
void CleanupE0139(AreaImmediateAuthorTaskParameter *);
void Setup0139Characters(void);
void Restore0139Characters(void);
void Prepare0139Character(shen_name);
void Restore0139Character(shen_name);
void Hide0139Prop(shen_name);
void Show0139Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0139_AREA{SHEN_NAME(0139)};
inline constexpr shen_name AREA_0139_SYN_{SHEN_NAME(SYN_)};
inline constexpr shen_name AREA_0139_A450{SHEN_NAME(a450)};
inline constexpr shen_name AREA_0139_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0139_AR03{SHEN_NAME(AR03)};
inline constexpr shen_name AREA_0139_CHARACTERS[]{AREA_0139_SYN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0139_MOTION[] = "0139A.MOT", AREA_0139_CONVERSATION[] = "0139";

extern const ScriptAreaDefinition area0139ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
