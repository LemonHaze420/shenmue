#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/event_utility_dc.h"
#include "s2/sk_event_dc.h"
#include "s2/areas/area_loop_dc.h"

#define WTKQ_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define WTKQ_KUN SHEN_NAME4('K', 'U', 'N', '_')
#define WTKQ_III SHEN_NAME4('I', 'I', 'I', '_')
#define WTKQ_SEE SHEN_NAME4('S', 'E', 'E', '_')
#define WTKQ_SYG SHEN_NAME4('S', 'Y', 'G', '_')
#define WTKQ_TKM SHEN_NAME4('T', 'K', 'M', '_')
#define WTKQ_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define WTKQ_DUMMY SHEN_NAME4('D', 'U', 'M', '_')
#define WTKQ_OTAM SHEN_NAME4('O', 'T', 'A', 'M')
#define WTKQ_FLYP SHEN_NAME4('F', 'L', 'Y', 'P')
#define WTKQ_TABLE0 SHEN_NAME4('t', 'b', 'l', '0')
#define WTKQ_TABLE1 SHEN_NAME4('t', 'b', 'l', '1')
#define WTKQ_CHAIR0 SHEN_NAME4('i', 's', 'u', '0')
#define WTKQ_CHAIR1 SHEN_NAME4('i', 's', 'u', '1')
#define WTKQ_CHAIR2 SHEN_NAME4('i', 's', 'u', '2')
#define WTKQ_CHAIR3 SHEN_NAME4('i', 's', 'u', '3')
#define WTKQ_DOOR SHEN_NAME4('R', '7', '0', '9')
#define WTKQ_LIGHT SHEN_NAME4('l', '0', '0', '2')
#define WTKQ_END_LIGHT SHEN_NAME4('W', 'T', 'S', '3')
#define WTKQ_AUTHOR_START SHEN_NAME4('S', 'T', 'R', 'T')
#define WTKQ_AUTHOR_END SHEN_NAME4('E', 'N', 'D', '_')
#define WTKQ_DEBUG_ID 3
#define WTKQ_RYO_QTE_HAND_POSE 13
#define WTKQ_KUN_GRIP_HAND_POSE 11
#define WTKQ_KUN_FAILURE_HAND_POSE 18
typedef struct WtkqWork {
    HLibTask *task;
    short phase, step;
    EventReadCharactersWork *characters;
    int qte[4];
} WtkqWork;
void WtkqMain(void *argument);
void QueueWtkq(void *argument);
int WtkqInitialize(WtkqWork *work);
int WtkqFinish(WtkqWork *work);
void WtkqEndCallback(void *data);
void WtkqMainCallback(void *data);
int WtkqBattle(WtkqWork *work);
int WtkqRetry(WtkqWork *work);
int WtkqWin(WtkqWork *work);
int WtkqQteSuccess(void);
int WtkqQteFailure(void);
int WtkqStartAuthor(WtkqWork *work);
int WtkqEndAuthor(WtkqWork *work);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include <cstddef>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
struct WtkqWork {
    HLib::Task *self;
    std::uint16_t phase, step;
    std::uint32_t reserved0c;
    EventReadCharactersWork *characters;
    std::int32_t qteElapsed;
    std::uint32_t reserved1c;
    std::uint16_t qteStatus, reserved22;
    std::int32_t qteSucceeded;
    std::int32_t retried;
    std::uint32_t reserved2c;
};
static_assert(sizeof(WtkqWork) == 0x30);
static_assert(offsetof(WtkqWork, qteElapsed) == 0x18 && offsetof(WtkqWork, retried) == 0x28);
void WtkqInit();
void WtkqMain(std::uintptr_t);
void QueueWtkq(void *);
void WtkqMainCallback(void *);
void WtkqEndCallback(void *);
std::int32_t WtkqQteSuccess();
std::int32_t WtkqQteFailure();
extern const ScriptAreaDefinition wtkqScriptDefinition;
} // namespace shenmue::s2
#endif
