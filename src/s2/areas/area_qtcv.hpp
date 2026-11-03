#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/areas/area_loop_dc.h"

#define QTCV_AREA SHEN_NAME4('Q', 'T', 'C', 'V')
#define QTCV_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define QTCV_REN SHEN_NAME4('R', 'E', 'N', '_')
#define QTCV_STAGE SHEN_NAME4('S', 'T', 'R', 'B')
#define QTCV_SECOND_STAGE SHEN_NAME4('S', 'T', 'P', 'B')
#define QTCV_DUMMY_PLAYER SHEN_NAME4('D', 'U', 'M', 'R')
#define QTCV_DUMMY_REN SHEN_NAME4('D', 'U', 'M', 'L')
#define QTCV_AUTHOR_1 SHEN_NAME4('A', 'T', 'H', '1')
#define QTCV_AUTHOR_2 SHEN_NAME4('A', 'T', 'H', '2')
#define QTCV_AUTHOR_0 SHEN_NAME4('A', 'T', 'H', '0')
#define QTCV_DEBUG_ID 3
#define QTCV_GRIP_HAND_POSE 11
typedef struct QtcvConfiguration {
    int kind, tail;
} QtcvConfiguration;
typedef struct QtcvPlacement {
    Vector3 position;
    float yaw;
} QtcvPlacement;
typedef struct QtcvEventWork {
    EventReadCharactersWork *characters;
    HLibTask *debugTask;
    HLibTask *task;
    HLibTaskCallback authorContinuation;
    const QtcvConfiguration *configuration;
    const QtcvPlacement *placement;
    int dialogue;
    float playerHeight;
    int changedFlag330;
} QtcvEventWork;
extern int qtcvCarryState;
void QtcvMain(void *argument);
void QueueQtcvEvent(const QtcvConfiguration *configuration);
void QtcvDebugExitCallback(void *data);
void QtcvEndCallback(void *data);
void QtcvDebugDisplayCallback(void *data);
void SetQtcvEndContinuation(HLibTaskCallback *continuation);
void QtcvStartCallback(void *data);
void QtcvAlternateAuthorCallback(void *data);
void AuthoredEventContinuationCallback(void *data);
void QtcvAlternateFinishCallback(void *data);
void QtcvEndThunkCallback(void *data);
void InitializeQtcvResources(QtcvEventWork *work);
void CleanupQtcvResources(QtcvEventWork *work);
int PlayPlankCarryAuthoredTransition(int kind, int dialogue);
int PlayPlankCarryAuthoredTransitionTail(int kind, int dialogue);
int StartQtcvAlternateCamera(void);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
namespace shenmue::s2 {
inline constexpr shen_name QTCV_AREA{SHEN_NAME(QTCV)}, QTCV_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name QTCV_PLAYER{SHEN_NAME(RYO_)}, QTCV_REN{SHEN_NAME(REN_)};
inline constexpr shen_name QTCV_STAGE{SHEN_NAME(STRB)}, QTCV_SECOND_STAGE{SHEN_NAME(STPB)}, QTCV_INVALID{0xffffffffU};
inline constexpr shen_name QTCV_DUMMY_PLAYER{SHEN_NAME(DUMR)}, QTCV_DUMMY_REN{SHEN_NAME(DUML)};
inline constexpr shen_name QTCV_AUTHORS[]{SHEN_NAME(ATH1), SHEN_NAME(ATH2), SHEN_NAME(ATH0)};
inline constexpr std::int16_t QTCV_AUTHOR_FLAGS[]{2062, 2063, 2064};
inline constexpr shen_name QTCV_LOAD_CHARACTERS[]{QTCV_PLAYER, QTCV_REN, QTCV_INVALID};
struct QtcvConfiguration {
    std::int32_t kind, tail;
};
inline constexpr QtcvConfiguration QTCV_CONFIGURATIONS[]{{0, 0}, {1, 0}, {2, 0}};
struct QtcvPlacement {
    Vector3 position;
    float yaw;
};
inline constexpr QtcvPlacement QTCV_PLACEMENTS[]{{{221.8000030517578f, 110.80000305175781f, 162.39999389648438f}, 180},
    {{202, 110.80000305175781f, 148.39999389648438f}, 90}, {{245.3000030517578f, 110.80000305175781f, 148.6999969482422f}, 270}};
struct QtcvEventWork {
    EventReadCharactersWork *characters;
    std::uint64_t unused08;
    HLib::Task *task;
    HLib::TaskCallback authorContinuation;
    const QtcvConfiguration *configuration;
    const QtcvPlacement *placement;
    std::int32_t dialogue;
    float playerHeight;
    std::int32_t changedFlag330;
    std::int32_t remainingFrames;
    HLib::TaskCallback waitContinuation;
};
static_assert(sizeof(QtcvConfiguration) == 8 && sizeof(QtcvPlacement) == 0x10);
static_assert(sizeof(QtcvEventWork) == 0x48);
static_assert(offsetof(QtcvEventWork, authorContinuation) == 0x18 && offsetof(QtcvEventWork, configuration) == 0x20);
static_assert(offsetof(QtcvEventWork, dialogue) == 0x30 && offsetof(QtcvEventWork, waitContinuation) == 0x40);
void QtcvInit();
void QtcvMain(std::uintptr_t);
void QueueQtcvEvent(const QtcvConfiguration *);
void InitializeQtcvResources(QtcvEventWork *);
void QtcvStartCallback(void *);
void QtcvNormalWaitCallback(void *);
void QtcvAlternateAuthorCallback(void *);
void AuthoredEventContinuationAt18Callback(void *);
void QtcvAlternateFinishCallback(void *);
void QtcvEnemyWaitCallback(void *);
void QtcvEndThunkCallback(void *);
void QtcvEndCallback(void *);
void CleanupQtcvResources(QtcvEventWork *);
void QtcvDormantBoundary();
extern const ScriptAreaDefinition qtcvScriptDefinition;
} // namespace shenmue::s2
#endif
