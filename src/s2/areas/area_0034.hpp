#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_utility_dc.h"
#include "s2/event_queued_dc.h"

#define AREA_0034_AREA SHEN_NAME4('0', '0', '3', '4')
#define AREA_0034_WTA0 SHEN_NAME4('W', 'T', 'A', '0')
#define AREA_0034_WB00 SHEN_NAME4('W', 'B', '0', '0')
#define AREA_0034_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define AREA_0034_KUN SHEN_NAME4('K', 'U', 'N', '_')
#define AREA_0034_PROP(a, b) SHEN_NAME4('a', '0', a, b)

typedef struct E0034TaskParameter {
    HLibTask *self;
    short selection, state;
} E0034TaskParameter;
typedef struct E0034LoopParameter {
    E0034TaskParameter *parent;
    short currentFrame;
} E0034LoopParameter;

extern shen_name e0034Characters[], e0034Authors[];
void E0034Main(const short *selection);
void E0034MainCallback(void *taskData);
void QueueE0034Loop(E0034TaskParameter *parent);
void LoadE0034ScenePackages(int selection);
void E0034LoopCallback(void *taskData);
void ConfigureE0034Environment(void);
void CleanupE0034(E0034TaskParameter *parent);
void SetupE0034Characters(void);
void RestoreE0034Characters(void);
void PrepareE0034Character(shen_name name);
void RestoreE0034Character(shen_name name);
void HideE0034Prop(shen_name name);
void ShowE0034Prop(shen_name name);
#else
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0034_AREA{SHEN_NAME(0034)};
inline constexpr shen_name AREA_0034_KUN_{SHEN_NAME(KUN_)};
inline constexpr shen_name AREA_0034_A047{SHEN_NAME(a047)};
inline constexpr shen_name AREA_0034_A044{SHEN_NAME(a044)};
inline constexpr shen_name AREA_0034_A046{SHEN_NAME(a046)};
inline constexpr shen_name AREA_0034_A050{SHEN_NAME(a050)};
inline constexpr shen_name AREA_0034_A051{SHEN_NAME(a051)};
inline constexpr shen_name AREA_0034_A052{SHEN_NAME(a052)};
inline constexpr shen_name AREA_0034_A053{SHEN_NAME(a053)};
inline constexpr shen_name AREA_0034_AB27{SHEN_NAME(ab27)};
inline constexpr shen_name AREA_0034_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0034_WTA0{SHEN_NAME(WTA0)};
inline constexpr shen_name AREA_0034_WBBK{SHEN_NAME(WBBK)};
inline constexpr shen_name AREA_0034_TELS{SHEN_NAME(TELS)};
inline constexpr shen_name AREA_0034_CHARACTERS[]{AREA_0034_KUN_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0034_MOTION[] = "0034A.MOT", AREA_0034_CONVERSATION[] = "0034";

extern const ScriptAreaDefinition area0034ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
