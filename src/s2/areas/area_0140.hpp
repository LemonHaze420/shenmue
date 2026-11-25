#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0140_AREA SHEN_NAME4('0', '1', '4', '0')
#define AREA_0140_AR02 SHEN_NAME4('A', 'R', '0', '2')
#define AREA_0140_A010 SHEN_NAME4('a', '0', '1', '0')
#define AREA_0140_A396 SHEN_NAME4('a', '3', '9', '6')
#define AREA_0140_A546 SHEN_NAME4('a', '5', '4', '6')
#define AREA_0140_A548 SHEN_NAME4('a', '5', '4', '8')
#define AREA_0140_JOP_ SHEN_NAME4('J', 'O', 'P', '_')
#define AREA_0140_SHK_ SHEN_NAME4('S', 'H', 'K', '_')
#define AREA_0140_CONVERSATION "0140"
extern shen_name AREA_0140_CHARACTERS[];
void E0140Main(void);
void E0140MainCallback(void *);
void QueueE0140Loop(AreaAuthorTaskParameter *);
void E0140LoopCallback(void *);
void Configure0140(void);
void CleanupE0140(AreaAuthorTaskParameter *);
void SetupE0140Characters(void);
void RestoreE0140Characters(void);
void Prepare0140Character(shen_name);
void Restore0140Character(shen_name);
void Hide0140Prop(shen_name);
void Show0140Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0140_AREA{SHEN_NAME(0140)};
inline constexpr shen_name AREA_0140_AR02{SHEN_NAME(AR02)};
inline constexpr shen_name AREA_0140_A010{SHEN_NAME(a010)};
inline constexpr shen_name AREA_0140_A396{SHEN_NAME(a396)};
inline constexpr shen_name AREA_0140_A546{SHEN_NAME(a546)};
inline constexpr shen_name AREA_0140_A548{SHEN_NAME(a548)};
inline constexpr shen_name AREA_0140_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0140_SHK_{SHEN_NAME(SHK_)};
inline constexpr shen_name AREA_0140_CHARACTERS[]{AREA_0140_JOP_, AREA_0140_SHK_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0140_MOTION[] = "0140A.MOT", AREA_0140_CONVERSATION[] = "0140";

extern const ScriptAreaDefinition area0140ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
