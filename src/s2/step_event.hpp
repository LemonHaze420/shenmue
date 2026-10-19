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
