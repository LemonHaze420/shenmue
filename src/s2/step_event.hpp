#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_debug_dc.h"
#include "s2/event_utility_dc.h"
#include "s2/sk_event_dc.h"

#define STEP_AREA SHEN_NAME4('S', 'T', 'E', 'P')
#define STEP_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define STEP_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define STEP_CHD SHEN_NAME4('C', 'H', 'D', '_')
#define STEP_ISN SHEN_NAME4('I', 'S', 'N', '_')
#define STEP_KRD SHEN_NAME4('K', 'R', 'D', '_')
#define STEP_KR4 SHEN_NAME4('K', 'R', '4', '_')
#define STEP_KF4 SHEN_NAME4('K', 'F', '4', '_')
#define STEP_TGY SHEN_NAME4('T', 'G', 'Y', '_')
#define STEP_AUTHOR SHEN_NAME4('A', 'U', 'T', 'H')
#define STEP_DEBUG_ID 3
#define STEP_ENEMY_RIG 7
typedef struct StepEventWork {
    HLibTask *task;
    short state;
    short phase;
    EventReadCharactersWork *characters;
    int qteElapsed;
    int reserved10;
    unsigned short qteStatus, reserved16;
    int qteSucceeded;
} StepEventWork;
typedef struct StepDescentWork {
    HLibTask *task;
    shen_name character;
    short phase;
    short steps;
} StepDescentWork;
extern shen_name stepCharacterNames[];
extern shen_name stepAuthorCharacterNames[];
void Queue_EVT_AT_NPCClear(void *argument);
void Queue_eSTEP_EVT_KaidanQteMain(void *data);
int STEP_SetupKaidanQte(StepEventWork *work);
int STEP_FinishKaidanAuthor(StepEventWork *work);
void eSTEP_EVT_EndKaidanQte_Callback(void *data);
void eSTEP_EVT_KaidanQteMain_Callback(void *data);
int STEP_RunKaidanQte(StepEventWork *work);
int STEP_BeginFirstOpponent(StepEventWork *work);
int STEP_FirstDodgeSuccess(void);
int STEP_FirstDodgeFailure(void);
int STEP_FirstCounterSuccess(void);
int STEP_FirstCounterFailure(void);
int STEP_BeginSecondOpponent(StepEventWork *work);
int STEP_SecondDodgeSuccess(void);
int STEP_SecondDodgeFailure(void);
int STEP_SecondCounterSuccess(void);
int STEP_SecondCounterFailure(void);
int STEP_BeginThirdOpponent(StepEventWork *work);
int STEP_ThirdCounterSuccess(void);
int STEP_ThirdCounterFailure(void);
int STEP_BeginFourthOpponent(StepEventWork *work);
int STEP_FourthDodgeSuccess(void);
int STEP_FourthDodgeFailure(void);
int STEP_FourthComboSuccess(void);
int STEP_FourthComboFailure(void);
int STEP_FourthCounterSuccess(void);
int STEP_FourthCounterFailure(void);
int STEP_BeginFinalOpponent(StepEventWork *work);
int STEP_FinalDodgeSuccess(void);
int STEP_FinalDodgeFailure(void);
void STEP_PositionRunningOpponent(shen_name character, float offset, int delay);
void QueueStepDescent(shen_name character, short steps, int delay);
void eSTEP_EVT_Step(void *data);
int STEP_RunKaidanAuthor(StepEventWork *work);
void STEP_AuthorFrameCallback(void *data);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name STEP_AREA{SHEN_NAME(STEP)}, STEP_RYO{SHEN_NAME(RYO_)}, STEP_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name STEP_CHD{SHEN_NAME(CHD_)}, STEP_ISN{SHEN_NAME(ISN_)}, STEP_KRD{SHEN_NAME(KRD_)};
inline constexpr shen_name STEP_KR4{SHEN_NAME(KR4_)}, STEP_KF4{SHEN_NAME(KF4_)}, STEP_TGY{SHEN_NAME(TGY_)};
inline constexpr shen_name STEP_INVALID{0xffffffffU}, STEP_EVENT{SHEN_NAME(EVNT)};
inline constexpr shen_name STEP_AUTHOR{SHEN_NAME(AUTH)};
inline constexpr std::array<shen_name, 7> stepCharacterNames{STEP_RYO, STEP_CHD, STEP_ISN, STEP_KRD, STEP_KR4, STEP_KF4, STEP_INVALID};
inline constexpr std::array<shen_name, 13> stepAuthorCharacterNames{SHEN_NAME(REN_), SHEN_NAME(SRY_), SHEN_NAME(JOP_), SHEN_NAME(SGT_), STEP_TGY,
    SHEN_NAME(WON_), SHEN_NAME(ESY_), SHEN_NAME(BF8_), SHEN_NAME(BF7_), SHEN_NAME(BF1_), SHEN_NAME(BF9_), SHEN_NAME(KF5_), STEP_INVALID};

struct StepEventWork {
    HLib::Task *task;
    std::int16_t phase;
    std::int16_t state;
    std::uint16_t reserved0c, reserved0e;
    EventReadCharactersWork *characters;
    std::int32_t qteElapsed;
    std::uint32_t reserved1c;
    std::uint16_t qteStatus, reserved22;
    std::int32_t qteSucceeded;
};
struct StepDescentWork {
    HLib::Task *task;
    shen_name character;
    std::int16_t phase;
    std::int16_t steps;
};
struct StepNpcClearWork {
    HLib::TaskCallback callback;
    void *callbackData;
    std::uint32_t reserved10;
    std::int32_t state;
    float delay;
    std::uint32_t reserved1c;
};
using StepQteCallback = std::int32_t (*)();
struct StepWaitQteWork {
    HLib::Task *task;
    HLib::Task *parent;
    StepQteCallback success;
    StepQteCallback failure;
    std::int32_t referenceFrames;
    std::uint32_t reserved24;
    std::int32_t *elapsed;
    std::int32_t maximumFrames;
    std::uint32_t button;
    std::uint16_t *status;
    std::uint16_t phase;
    std::byte reserved42[6];
    std::int32_t *succeeded;
};
struct StepAuthorStartWork {
    HLib::Task *task;
    HLib::Task *parent;
    std::uint32_t command;
    std::uint16_t phase, reserved16;
    HLib::TaskCallback callback;
    void *callbackData;
};
struct StepObjectDisplayWork {
    std::int32_t cleanup;
    shen_name character;
    std::int32_t mode;
    float delay;
};
static_assert(sizeof(StepEventWork) == 0x28 && offsetof(StepEventWork, characters) == 0x10);
static_assert(offsetof(StepEventWork, state) == 0x0a && offsetof(StepEventWork, qteSucceeded) == 0x24);
static_assert(sizeof(StepDescentWork) == 0x10 && offsetof(StepDescentWork, steps) == 0x0e);
static_assert(sizeof(StepNpcClearWork) == 0x20 && offsetof(StepNpcClearWork, delay) == 0x18);
static_assert(offsetof(StepNpcClearWork, reserved10) == 0x10 && offsetof(StepNpcClearWork, state) == 0x14);
static_assert(sizeof(StepWaitQteWork) == 0x50 && offsetof(StepWaitQteWork, phase) == 0x40);
static_assert(sizeof(StepAuthorStartWork) == 0x28 && offsetof(StepAuthorStartWork, callback) == 0x18);
static_assert(sizeof(StepObjectDisplayWork) == 0x10);
extern std::int32_t scenarioQteRetryCount;
void EVT_AT_NPCClear_Callback(void *taskData);
void EVT_AT_ObjectDisp_Callback(void *taskData);
void EVT_AT_ObjectDisp_1(shen_name character, std::int32_t delayFrames, std::int32_t mode);
void EVT_SK_AuthStart_Callback(void *taskData);
void EVT_SK_WaitQTE_Callback(void *taskData);
extern const ScriptAreaDefinition stepScriptDefinition;
} // namespace shenmue::s2
#endif
