#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct KachDealerEntry {
    shen_name character;
    std::uint32_t playedFlag;
};
struct KachVoiceWaitWork {
    HLib::Task *parent;
    std::int32_t delay, index, reserved10, voiceFrames;
    std::uint32_t flags, reserved1c;
};
struct KachMotionState {
    Vector3 position, velocity;
    float remainingTime;
};
struct KachCone {
    float slope, height;
};
struct KachDisk {
    float radius, x, z;
};
struct KachRuntime {
    shen_name dealer;
    std::int32_t timer, reserved08, state, maximumBet, minimumBet, bet;
    std::uint32_t result;
    std::int32_t selection;
    std::uint32_t soundFlags, reserved28, reserved2c;
    Vector3 ballPosition, ballVelocity, bowlOrigin, handPosition, handTarget;
};
static_assert(sizeof(KachDealerEntry) == 8);
static_assert(sizeof(KachVoiceWaitWork) == 0x20 && offsetof(KachVoiceWaitWork, voiceFrames) == 0x14 && offsetof(KachVoiceWaitWork, flags) == 0x18);
static_assert(sizeof(KachMotionState) == 0x1c && offsetof(KachMotionState, remainingTime) == 0x18);
static_assert(sizeof(KachCone) == 8 && sizeof(KachDisk) == 12);
static_assert(sizeof(KachRuntime) == 0x6c && offsetof(KachRuntime, state) == 0x0c && offsetof(KachRuntime, result) == 0x1c);
static_assert(offsetof(KachRuntime, ballPosition) == 0x30 && offsetof(KachRuntime, handPosition) == 0x54 && offsetof(KachRuntime, handTarget) == 0x60);
void InitKach();

void QueueKach(std::uintptr_t boardIndex);
void KachInitializeCallback(void *);
void KachPostLoadCallback(void *);
void KachGreetingCallback(void *);
void KachSelectRuleCallback(void *);
void KachAcceptRulesCallback(void *);
void KachQuitCallback(void *);
void KachNoMoneyCallback(void *);
void KachExplainRulesCallback(void *);
void KachSleepCallback(void *);
void KachBetCallback(void *);
void KachSelectPointCallback(void *);
void KachFallMoneyCallback(void *);
void KachThrowCallback(void *);
void KachWinCallback(void *);
void KachLoseCallback(void *);
void KachExitCallback(void *);
void KachCleanupCallback(void *);
void KachRollCallback(void *);
void KachVoiceWaitCallback(void *);
void KachLocalDebugNullsub();
std::uint32_t UpdateKachBall(Vector3 *position, Vector3 *velocity, const Vector3 *origin, std::uint16_t angle);
extern const ScriptAreaDefinition kachScriptDefinition;
} // namespace shenmue::s2
