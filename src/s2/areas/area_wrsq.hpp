#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/sk_event_dc.h"
#include "s2/areas/area_loop_dc.h"

#define WRSQ_AREA SHEN_NAME4('W', 'R', 'S', 'Q')
#define WRSQ_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define WRSQ_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define WRSQ_III SHEN_NAME4('I', 'I', 'I', '_')
#define WRSQ_SEE SHEN_NAME4('S', 'E', 'E', '_')
#define WRSQ_SJN SHEN_NAME4('S', 'J', 'N', '_')
#define WRSQ_SAY SHEN_NAME4('S', 'A', 'Y', '_')
#define WRSQ_GNB SHEN_NAME4('G', 'N', 'B', '_')
#define WRSQ_MYU SHEN_NAME4('M', 'Y', 'U', '_')
#define WRSQ_A244 SHEN_NAME4('a', '2', '4', '4')
#define WRSQ_A245 SHEN_NAME4('a', '2', '4', '5')
#define WRSQ_AUTHOR_FIGHT SHEN_NAME4('F', 'I', 'S', 'T')
#define WRSQ_AUTHOR_LOSE SHEN_NAME4('L', 'O', 'S', 'E')
#define WRSQ_AUTHOR_WIN SHEN_NAME4('W', 'I', 'N', '_')
#define WRSQ_INVALID 0xffffffffU
#define WRSQ_DEBUG_ID 3
#define WRSQ_QTE_HAND_POSE 13
#define WRSQ_QTE_REACTION_HAND_POSE 15
#define WRSQ_MOTION_FLAGS 0x44000000U

typedef struct WrsqWork {
    HLibTask *self;
    unsigned short phase, step;
    EventReadCharactersWork *characters;
    unsigned char won;
} WrsqWork;

void WrsqMain(void);
void QueueWrsq(void *data);
int InitializeWrsq(WrsqWork *work);
void WrsqDebugExitCallback(void *data);
int FinishWrsq(WrsqWork *work);
void WrsqEndCallback(void *data);
void WrsqMainCallback(void *data);
int IntroWrsq(WrsqWork *work);
int LoseWrsq(WrsqWork *work);
void WrsqLoseAuthorCallback(void *data);
int WinWrsq(WrsqWork *work);
int AdvanceWrsqTime(WrsqWork *work);
int BattleWrsq(WrsqWork *work);
int WrsqSuccessA(int delay);
int WrsqSuccessB(int delay);
int WrsqSuccessC(int delay);
int WrsqFailureA(int delay);
int WrsqFailureB(int delay);
int WrsqFailureC(int delay);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include <cstddef>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
struct WrsqWork {
    HLib::Task *self;
    std::uint16_t phase, step;
    std::uint32_t reserved0c;
    EventReadCharactersWork *characters;
    std::uint8_t won;
    std::byte reserved19[7];
};
static_assert(sizeof(WrsqWork) == 0x20);
static_assert(offsetof(WrsqWork, characters) == 0x10 && offsetof(WrsqWork, won) == 0x18);
void WrsqInit();
void WrsqMain(std::uintptr_t);
void QueueWrsq(void *);
void WrsqMainCallback(void *);
void WrsqEndCallback(void *);
void WrsqLoseAuthorCallback(void *);
extern const ScriptAreaDefinition wrsqScriptDefinition;
} // namespace shenmue::s2
#endif
