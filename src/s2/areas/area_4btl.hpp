#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/event_library_tail_dc.h"
#include "s2/areas/area_loop_dc.h"

#define BTL4_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define BTL4_SAM SHEN_NAME4('S', 'A', 'M', '_')
#define BTL4_LLY SHEN_NAME4('L', 'L', 'Y', '_')
#define BTL4_REN SHEN_NAME4('R', 'E', 'N', '_')
#define BTL4_CLJ SHEN_NAME4('C', 'L', 'J', '_')
#define BTL4_DUMMY SHEN_NAME4('D', 'U', 'M', '_')
#define BTL4_CAMERA SHEN_NAME4('C', 'A', 'M', '_')
#define BTL4_KEIB SHEN_NAME4('K', 'E', 'I', 'B')
#define BTL4_NEXT_AREA SHEN_NAME4('A', 'R', '0', '3')
#define BTL4_GRID SHEN_NAME4('B', 'T', 'C', '1')
#define BTL4_PROP_GRID SHEN_NAME4('P', 'R', 'O', 'P')
#define BTL4_DEFAULT_GRID SHEN_NAME4('0', '0', '0', '0')
#define BTL4_TABLE_MOTION SHEN_NAME4('T', 'U', 'K', 'U')
#define BTL4_DEBUG_ID 3
#define BTL4_GRIP_HAND_POSE 11
#define BTL4_FLAG_ROUTE_A 886
#define BTL4_FLAG_ROUTE_B 888
#define BTL4_CHAIR_COUNT 9
typedef struct Btl4Work {
    HLibTask *task;
    shen_name partner;
    int phase, step;
    EventReadCharactersWork *characters;
} Btl4Work;
typedef struct Btl4Spot {
    float x, y, z, angle;
} Btl4Spot;
void Start4Btl(void *argument);
void Queue4Btl(void *argument);
int Btl4Initialize(Btl4Work *work);
void Btl4DebugExitCallback(void *data);
void Btl4Finish(Btl4Work *work);
void Btl4Callback(void *data);
int Btl4RunFight(Btl4Work *work);
int Btl4Intro(Btl4Work *work);
int Btl4SamIntroRepeat(Btl4Work *work);
int Btl4LlyIntro(Btl4Work *work);
int Btl4SamIntro(Btl4Work *work);
int Btl4LlyIntroRepeat(Btl4Work *work);
void Btl4SetupSamFight(shen_name partner);
void Btl4SetupLlyFight(shen_name partner);
int Btl4Won(Btl4Work *work);
int Btl4SamWon(Btl4Work *work);
int Btl4LlyWon(Btl4Work *work);
int Btl4Lost(Btl4Work *work);
int Btl4SamLost(Btl4Work *work);
int Btl4LlyLost(Btl4Work *work);
unsigned int Btl4FindAnchor(void);
void Btl4SelectAnchor(shen_name character);
void Btl4ApplyAnchor(shen_name character);
void Btl4CreateStageObjects(void);
void Btl4ChairState(int restore);
#else
#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
extern const ScriptAreaDefinition area4BtlScriptDefinition;
}
#endif
