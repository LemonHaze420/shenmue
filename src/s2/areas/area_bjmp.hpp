#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/sk_event_dc.h"
#include "s2/areas/area_loop_dc.h"

#define BJMP_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define BJMP_REN SHEN_NAME4('R', 'E', 'N', '_')
#define BJMP_TGY SHEN_NAME4('T', 'G', 'Y', '_')
#define BJMP_DUMMY SHEN_NAME4('D', 'U', 'M', '_')
#define BJMP_LIGHT SHEN_NAME4('B', 'J', 'M', 'P')
#define BJMP_RUN_LIGHT SHEN_NAME4('0', '0', '0', '0')
#define BJMP_AUTHOR_START SHEN_NAME4('Q', 'J', 'P', '0')
#define BJMP_AUTHOR_RETRY SHEN_NAME4('Q', 'J', 'P', '1')
#define BJMP_EXIT_AREA SHEN_NAME4('0', '2', '2', '1')
#define BJMP_INVALID 0xffffffffU
#define BJMP_DEBUG_ID 3
#define BJMP_TGY_HAND_POSE 18
#define BJMP_SET_GRAVITY(name, enabled) do { FTBL.EV_SetCharaControllerFlags(name, 2, enabled); FTBL.EV_SetCharaControllerFlags(name, 3, enabled); } while (0)

typedef struct JumpWork {
    HLibTask *self;
    int reserved04;
    unsigned short phase, retries;
    int reserved0c[4];
} JumpWork;

extern const char bjmpPackage[], bjmpCutscene[], bjmpAuthorPak[];
void AreaBJMPMain(void);
void Auth1_Callback(void *data);
void Main_Callback(void *data);
void BeginRun(JumpWork *work);
int JumpSuccess(void);
int BeginLanding(void);
int EndLanding(void);
void Retry(JumpWork *work);
void FallingPosition_Callback(void *data);
void DebugExit_Callback(void *data);
void Exit_Callback(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
void AreaBJMPInit();
void AreaBJMPMain(std::uintptr_t parameter);
extern const ScriptAreaDefinition areaBJMPScriptDefinition;
} // namespace shenmue::s2::areas
#endif
