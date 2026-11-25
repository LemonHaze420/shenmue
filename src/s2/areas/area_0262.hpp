#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0262_AREA SHEN_NAME4('0', '2', '6', '2')
#define AREA_0262_A276 SHEN_NAME4('a', '2', '7', '6')
#define AREA_0262_AA80 SHEN_NAME4('a', 'a', '8', '0')
#define AREA_0262_AA77 SHEN_NAME4('a', 'a', '7', '7')
#define AREA_0262_L001 SHEN_NAME4('l', '0', '0', '1')
#define AREA_0262_0000 SHEN_NAME4('0', '0', '0', '0')
#define AREA_0262_262A SHEN_NAME4('2', '6', '2', 'A')
#define AREA_0262_262B SHEN_NAME4('2', '6', '2', 'B')
#define AREA_0262_WON_ SHEN_NAME4('W', 'O', 'N', '_')
#define AREA_AUTHOR_REN SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_0262_CONVERSATION "0262"
extern shen_name AREA_0262_CHARACTERS[];
void E0262Main(short *);
void E0262MainCallback(void *);
void E0262LoopCallback(void *);
void QueueE0262Author(AreaImmediateAuthorTaskParameter *);
void ConfigureE0262Environment(void);
void CleanupE0262(AreaImmediateAuthorTaskParameter *);
void SetupE0262Characters(void);
void RestoreE0262Characters(void);
void PrepareE0262Character(shen_name);
void RestoreE0262Character(shen_name);
void HideE0262Prop(shen_name);
void ShowE0262Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0262_AREA{SHEN_NAME(0262)};
inline constexpr shen_name AREA_0262_A276{SHEN_NAME(a276)};
inline constexpr shen_name AREA_0262_AA80{SHEN_NAME(aa80)};
inline constexpr shen_name AREA_0262_AA77{SHEN_NAME(aa77)};
inline constexpr shen_name AREA_0262_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0262_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0262_262A{SHEN_NAME(262A)};
inline constexpr shen_name AREA_0262_262B{SHEN_NAME(262B)};
inline constexpr shen_name AREA_0262_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0262_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0262_WON_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0262_MOTION[] = "0262A.MOT", AREA_0262_CONVERSATION[] = "0262";
inline constexpr shen_name AREA_0262_AUTHORS[]{AREA_0262_262A, AREA_0262_262B};
extern const ScriptAreaDefinition area0262ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
