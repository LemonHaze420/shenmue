#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_REN SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_0222_AREA SHEN_NAME4('0', '2', '2', '2')
#define AREA_0222_YAN_ SHEN_NAME4('Y', 'A', 'N', '_')
#define AREA_0222_AA22 SHEN_NAME4('a', 'a', '2', '2')
#define AREA_0222_A052 SHEN_NAME4('a', '0', '5', '2')
#define AREA_0222_A053 SHEN_NAME4('a', '0', '5', '3')
#define AREA_0222_A212 SHEN_NAME4('a', '2', '1', '2')
#define AREA_0222_A095 SHEN_NAME4('a', '0', '9', '5')
#define AREA_0222_A316 SHEN_NAME4('a', '3', '1', '6')
#define AREA_0222_QR00 SHEN_NAME4('Q', 'B', '0', '0')
#define AREA_0222_QAAA SHEN_NAME4('Q', 'B', 'A', 'A')
#define AREA_0222_YAN1 SHEN_NAME4('Y', 'A', 'N', '1')
#define AREA_0222_YAN2 SHEN_NAME4('Y', 'A', 'N', '2')
#define AREA_0222_YAN3 SHEN_NAME4('Y', 'A', 'N', '3')
#define AREA_0222_CONVERSATION "0222"
extern shen_name AREA_0222_CHARACTERS[];
void E0222Main(short *);
void E0222MainCallback(void *);
void E0222LoopCallback(void *);
void QueueE0222Author(AreaImmediateAuthorTaskParameter *);
void ConfigureE0222Environment(AreaImmediateAuthorTaskParameter *);
void CleanupE0222(AreaImmediateAuthorTaskParameter *);
void SetupE0222Characters(short);
void RestoreE0222Characters(short);
void PrepareE0222Character(shen_name);
void RestoreE0222Character(shen_name);
void HideE0222Prop(shen_name);
void ShowE0222Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0222_AREA{SHEN_NAME(0222)};
inline constexpr shen_name AREA_0222_YAN_{SHEN_NAME(YAN_)};
inline constexpr shen_name AREA_0222_AA22{SHEN_NAME(aa22)};
inline constexpr shen_name AREA_0222_A052{SHEN_NAME(a052)};
inline constexpr shen_name AREA_0222_A053{SHEN_NAME(a053)};
inline constexpr shen_name AREA_0222_A212{SHEN_NAME(a212)};
inline constexpr shen_name AREA_0222_A095{SHEN_NAME(a095)};
inline constexpr shen_name AREA_0222_A316{SHEN_NAME(a316)};
inline constexpr shen_name AREA_0222_QR00{SHEN_NAME(QR00)};
inline constexpr shen_name AREA_0222_QAAA{SHEN_NAME(QAAA)};
inline constexpr shen_name AREA_0222_YAN1{SHEN_NAME(YAN1)};
inline constexpr shen_name AREA_0222_YAN2{SHEN_NAME(YAN2)};
inline constexpr shen_name AREA_0222_YAN3{SHEN_NAME(YAN3)};
inline constexpr shen_name AREA_0222_CHARACTERS[]{AREA_0222_YAN_, AREA_AUTHOR_REN, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0222_MOTION[] = "0222A.MOT", AREA_0222_CONVERSATION[] = "0222";
inline constexpr shen_name AREA_0222_AUTHORS[]{AREA_0222_YAN1, AREA_0222_YAN2, AREA_0222_YAN3};
extern const ScriptAreaDefinition area0222ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
