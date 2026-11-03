#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/event_library_tail_dc.h"
#include "s2/sk_event_dc.h"
#include "s2/areas/area_loop_dc.h"

#define WKKT_AREA SHEN_NAME4('W', 'K', 'K', 'T')
#define WKKT_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define WKKT_KEM SHEN_NAME4('K', 'E', 'M', '_')
#define WKKT_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define WKKT_WAIT_NPC_TASK SHEN_NAME4('w', 'n', 'p', 'c')
#define WKKT_AUTHOR_SUCCESS SHEN_NAME4('B', 'U', 'T', 'K')
#define WKKT_ABSOLUTE 0xffffffffU
#define WKKT_DEBUG_ID 2
#define WKKT_STANCE_HAND_POSE 13
#define WKKT_STRIKE_HAND_POSE 40
#define WKKT_IDLE_MOTION 5187
#define WKKT_REQUIRED_INPUT 1028
#define WKKT_LEAF_SLOTS 4

typedef struct WkktTrainingRoot {
    HLibTask *self, *child;
} WkktTrainingRoot;
typedef struct WkktInputHistory {
    int pressed[30];
    int held[30];
    int cursor, previous, step, reserved, waiting;
} WkktInputHistory;
typedef struct WkktTrainingWork {
    HLibTask *self, *parent;
    EventReadCharactersWork *readCharacters;
    unsigned int elapsed, idleTicks, tries;
    int phase, level, rank, action, leafA, leafB, smoke;
    unsigned char errorCount;
    WkktInputHistory history;
} WkktTrainingWork;
typedef struct WkktFallingWork {
    HLibTask *self;
    int reserved04;
    unsigned char slot;
    int frame, level;
} WkktFallingWork;

extern float wkktStrengthPeak;
extern int wkktEntry;
void WkktMain(int argument);
void WkktRootCallback(void *data);
void WkktMakeTask(void *data);
void WkktPrepare(WkktTrainingWork *work);
void WkktStop(void *data);
void WkktDebugExitCallback(void *data);
void WkktEnd(void *data);
void WkktWaitNpc(void *data);
void WkktFirst(void *data);
void WkktAuto1(void *data);
void WkktAuto2(void *data);
void WkktAuto3(void *data);
void WkktTalk(void *data);
void WkktTraining(void *data);
void WkktUpdateStrength(int cycling);
void WkktHideGauge(void);
void WkktDrawLcdLevel(WkktTrainingWork *work);
void WkktBufferIdle(WkktTrainingWork *work);
void WkktBufferDuringMotion(WkktTrainingWork *work);
void WkktAction(void *data);
void WkktFeedback(WkktTrainingWork *work);
void WkktFeedbackVoice(WkktTrainingWork *work);
void WkktActionWait(void *data);
void WkktQueueFallLeaves(int level, int delay);
void WkktFallLeaves(void *data);
void WkktKillLeaves(void *data);
void WkktResetInputHistory(WkktInputHistory *history);
int WkktMatchInput(WkktInputHistory *history, const int *required, int count);
void WkktPlayStrike(int action);
int WkktRejectInput(WkktInputHistory *history);
void WkktSuccess(void *data);
void WkktAnimateSuccessEffects(WkktTrainingWork *work);
void WkktCreateLeafBurst(int *effect, int count, float rate);
void WkktCreateSuccessSmoke(WkktTrainingWork *work);
#else
#pragma once
#include "s2/area_event.hpp"
namespace shenmue::s2::areas {
extern const ScriptAreaDefinition areaWKKTScriptDefinition;
std::int32_t wkktSetGaugeVisible(std::int32_t visible);
}
#endif
