#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_queued_dc.h"
#define AREA_AUTHOR_TASK TASK_EVNT
#define AREA_AUTHOR_RYO RYO_CHARACTER
#define AREA_AUTHOR_REN SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_AUTHOR_INVALID INVALID_CHARACTER
#define AREA_0265_AREA SHEN_NAME4('0', '2', '6', '5')
#define AREA_0265_CHN_ SHEN_NAME4('C', 'H', 'N', '_')
#define AREA_0265_L001 SHEN_NAME4('l', '0', '0', '1')
#define AREA_0265_0000 SHEN_NAME4('0', '0', '0', '0')
#define AREA_0265_CONVERSATION "0265"
typedef struct E0265LoopParameter {
    AreaImmediateAuthorTaskParameter *parent;
} E0265LoopParameter;
extern shen_name AREA_0265_CHARACTERS[];
void E0265Main(void);
void E0265MainCallback(void *);
void QueueE0265Loop(AreaImmediateAuthorTaskParameter *);
void E0265LoopCallback(void *);
void ConfigureE0265Environment(void);
void CleanupE0265(AreaImmediateAuthorTaskParameter *);
void SetupE0265Characters(void);
void RestoreE0265Characters(void);
void Prepare0265Character(shen_name);
void Restore0265Character(shen_name);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0265_AREA{SHEN_NAME(0265)};
inline constexpr shen_name AREA_0265_CHN_{SHEN_NAME(CHN_)};
inline constexpr shen_name AREA_0265_L001{SHEN_NAME(l001)};
inline constexpr shen_name AREA_0265_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0265_CHARACTERS[]{AREA_AUTHOR_REN, AREA_0265_CHN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0265_MOTION[] = "0265A.MOT", AREA_0265_CONVERSATION[] = "0265";

extern const ScriptAreaDefinition area0265ScriptDefinition;

} // namespace shenmue::s2::areas

#endif
