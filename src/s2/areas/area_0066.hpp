#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0066_AREA SHEN_NAME4('0', '0', '6', '6')
#define AREA_0066_KUN_ SHEN_NAME4('K', 'U', 'N', '_')
#define AREA_0066_A047 SHEN_NAME4('a', '0', '4', '7')
#define AREA_0066_A890 SHEN_NAME4('a', '8', '9', '0')
#define AREA_0066_A540 SHEN_NAME4('a', '5', '4', '0')
#define AREA_0066_A044 SHEN_NAME4('a', '0', '4', '4')
#define AREA_0066_A045 SHEN_NAME4('a', '0', '4', '5')
#define AREA_0066_A046 SHEN_NAME4('a', '0', '4', '6')
#define AREA_0066_L001 SHEN_NAME4('l', '0', '0', '1')
#define AREA_0066_WTA0 SHEN_NAME4('W', 'T', 'A', '0')
#define AREA_0066_0001 SHEN_NAME4('0', '0', '0', '1')
#define AREA_0066_WT00 SHEN_NAME4('W', 'T', '0', '0')
#define AREA_0066_CONVERSATION "0066"
typedef struct E0066LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
} E0066LoopParameter;
extern shen_name AREA_0066_CHARACTERS[];
void E0066Main(void);
void E0066MainCallback(void *);
void QueueE0066Loop(AreaImmediateAuthorTaskParameter *);
void E0066LoopCallback(void *);
void ConfigureE0066Environment(void);
void CleanupE0066(AreaImmediateAuthorTaskParameter *);
void SetupE0066Characters(void);
void RestoreE0066Characters(void);
void Prepare0066Character(shen_name);
void Restore0066Character(shen_name);
void Hide0066Prop(shen_name);
void Show0066Prop(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0066_AREA{SHEN_NAME(0066)};
inline constexpr shen_name AREA_0066_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0066_A047{SHEN_NAME(a047)};
inline constexpr shen_name AREA_0066_A890{SHEN_NAME(a890)};
inline constexpr shen_name AREA_0066_A540{SHEN_NAME(a540)};
inline constexpr shen_name AREA_0066_A044{SHEN_NAME(a044)};
inline constexpr shen_name AREA_0066_A045{SHEN_NAME(a045)};
inline constexpr shen_name AREA_0066_A046{SHEN_NAME(a046)};
inline constexpr shen_name AREA_0066_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0066_WTA0{SHEN_NAME(WTA0)};
inline constexpr shen_name AREA_0066_0001{SHEN_NAME(0001)};
inline constexpr shen_name AREA_0066_WT00{SHEN_NAME(WT00)};
inline constexpr shen_name AREA_0066_CHARACTERS[]{AREA_0066_KUN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0066_MOTION[] = "0066A.MOT", AREA_0066_CONVERSATION[] = "0066";

extern const ScriptAreaDefinition area0066ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
