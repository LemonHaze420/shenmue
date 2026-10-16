#pragma once

#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include <cstddef>

namespace shenmue::s2::areas {
inline constexpr shen_name Q7QB_AREA{SHEN_NAME(Q7QB)}, Q7QB_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name Q7QB_RYO{SHEN_NAME(RYO_)}, Q7QB_REN{SHEN_NAME(REN_)}, Q7QB_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name Q7QB_DUMMY{SHEN_NAME(DUMR)}, Q7QB_CAMERA_DUMMY{SHEN_NAME(DUME)}, Q7QB_INVALID{0xffffffffU};
inline constexpr shen_name Q7QB_KK9{SHEN_NAME(KK9_)}, Q7QB_KK7{SHEN_NAME(KK7_)}, Q7QB_KK8{SHEN_NAME(KK8_)};
inline constexpr shen_name Q7QB_KKA{SHEN_NAME(KKA_)}, Q7QB_KN1{SHEN_NAME(KN1_)}, Q7QB_KN2{SHEN_NAME(KN2_)};
inline constexpr shen_name Q7QB_RIK{SHEN_NAME(RIK_)}, Q7QB_SBK{SHEN_NAME(SBK_)};
inline constexpr shen_name Q7QB_CHARACTERS[]{Q7QB_RYO, Q7QB_REN, Q7QB_KK9, Q7QB_KK8, Q7QB_KN1, Q7QB_RIK, Q7QB_SBK, Q7QB_KKA, Q7QB_KN2, Q7QB_KK7, Q7QB_INVALID};
inline constexpr shen_name Q7QB_PLURAL_CHARACTERS[]{Q7QB_KK9, Q7QB_KK7, Q7QB_KK8, Q7QB_KKA, Q7QB_KN1, Q7QB_KN2};
inline constexpr shen_name Q7QB_HIDDEN_CHARACTERS[]{Q7QB_KK9, Q7QB_KK7, Q7QB_KK8, Q7QB_KKA, Q7QB_KN1, Q7QB_KN2, Q7QB_RIK, Q7QB_SBK};
inline constexpr shen_name Q7QB_STANDING_CHARACTERS[]{Q7QB_RYO, Q7QB_KK9, Q7QB_KK7, Q7QB_KK8, Q7QB_KKA, Q7QB_KN1, Q7QB_KN2, Q7QB_RIK, Q7QB_SBK};
inline constexpr Vector3 Q7QB_RYO_POSITION{.x = -1, .y = 0, .z = -.5f}, Q7QB_REN_POSITION{.x = -10000, .y = 0, .z = 0};
inline constexpr Vector3 Q7QB_CAMERA_POSITION{.x = -2.55f, .y = 0, .z = 5.2f}, Q7QB_HIDDEN_POSITION{.x = 100, .y = 0, .z = 0};

struct Q7QbResultParameters {
    shen_name player, firstOpponent, secondOpponent;
    std::uint32_t motionFlags;
};
static_assert(sizeof(Q7QbResultParameters) == 0x10);

struct Q7QbWork {
    EventReadCharactersWork *characters;
    HLib::Task *task;
    std::int32_t succeeded;
    std::uint32_t reserved14;
    HLib::TaskCallback continuation;
    Q7QbResultParameters resultParameters;
    std::int32_t phase;
    std::uint32_t reserved34;
    float baseHeight;
    std::uint32_t reserved3c;
};
static_assert(sizeof(Q7QbWork) == 0x40 && offsetof(Q7QbWork, continuation) == 0x18);
static_assert(offsetof(Q7QbWork, resultParameters) == 0x20 && offsetof(Q7QbWork, phase) == 0x30 && offsetof(Q7QbWork, baseHeight) == 0x38);

void Q7QbFirstAttackBegin(void *, std::uint32_t);
void Q7QbFirstAttackSuccess(void *, std::uint32_t);
void Q7QbFirstAttackFailure(void *, std::uint32_t);
void Q7QbSecondAttackBegin(void *, std::uint32_t);
void Q7QbSecondAttackSuccess(void *, std::uint32_t);
void Q7QbSecondAttackFailure(void *, std::uint32_t);
void Q7QbThirdAttackBegin(void *, std::uint32_t);
void Q7QbThirdAttackSuccess(void *, std::uint32_t);
void Q7QbThirdAttackFailure(void *, std::uint32_t);
void Q7QbFourthAttackBegin(void *, std::uint32_t);
void Q7QbFourthAttackSuccess(void *, std::uint32_t);
void Q7QbFourthAttackFailure(void *, std::uint32_t);
void Q7QbFifthAttackBegin(void *, std::uint32_t);
void Q7QbFifthAttackSuccess(void *, std::uint32_t);
void Q7QbFifthAttackFailure(void *, std::uint32_t);
void Q7QbSixthAttackBegin(void *, std::uint32_t);
void Q7QbSixthAttackSuccess(void *, std::uint32_t);
void Q7QbSixthAttackFailure(void *, std::uint32_t);
void Q7QbFinalAttackBegin(void *, std::uint32_t);
void Q7QbFinalAttackSuccess(void *, std::uint32_t);
void Q7QbFinalAttackFailure(void *, std::uint32_t);
void Q7QbFinalCounterSuccess(void *, std::uint32_t);
void Q7QbFinalCounterFailure(void *, std::uint32_t);
void Q7QbBattleSuccess(void *, std::uint32_t);
inline constexpr EventQteData Q7QB_QTE_DATA[]{
    {.requiredQteStatus = 0x400,
        .retryReferenceFrames = 23,
        .maxTime = 16,
        .callbackTimeA = 2,
        .callbackTimeB = 0,
        .callbackTimeC = 15,
        .authoredWord = 0,
        .beginSuccess = nullptr,
        .beginFailure = Q7QbFirstAttackBegin,
        .resultSuccess = Q7QbFirstAttackSuccess,
        .resultFailure = Q7QbFirstAttackFailure},
    {.requiredQteStatus = 0x100,
        .retryReferenceFrames = 23,
        .maxTime = 16,
        .callbackTimeA = 2,
        .callbackTimeB = 0,
        .callbackTimeC = 8,
        .authoredWord = 0,
        .beginSuccess = nullptr,
        .beginFailure = Q7QbSecondAttackBegin,
        .resultSuccess = Q7QbSecondAttackSuccess,
        .resultFailure = Q7QbSecondAttackFailure},
    {.requiredQteStatus = 0x200,
        .retryReferenceFrames = 23,
        .maxTime = 16,
        .callbackTimeA = 3,
        .callbackTimeB = 12,
        .callbackTimeC = 0,
        .authoredWord = 0,
        .beginSuccess = nullptr,
        .beginFailure = Q7QbThirdAttackBegin,
        .resultSuccess = Q7QbThirdAttackSuccess,
        .resultFailure = Q7QbThirdAttackFailure},
    {.requiredQteStatus = 0x100,
        .retryReferenceFrames = 23,
        .maxTime = 16,
        .callbackTimeA = 4,
        .callbackTimeB = 0,
        .callbackTimeC = 8,
        .authoredWord = 0,
        .beginSuccess = nullptr,
        .beginFailure = Q7QbFourthAttackBegin,
        .resultSuccess = Q7QbFourthAttackSuccess,
        .resultFailure = Q7QbFourthAttackFailure},
    {.requiredQteStatus = 0x800,
        .retryReferenceFrames = 23,
        .maxTime = 16,
        .callbackTimeA = 0,
        .callbackTimeB = 0,
        .callbackTimeC = 5,
        .authoredWord = 0,
        .beginSuccess = nullptr,
        .beginFailure = Q7QbFifthAttackBegin,
        .resultSuccess = Q7QbFifthAttackSuccess,
        .resultFailure = Q7QbFifthAttackFailure},
    {.requiredQteStatus = 0x100,
        .retryReferenceFrames = 23,
        .maxTime = 16,
        .callbackTimeA = 4,
        .callbackTimeB = 0,
        .callbackTimeC = 6,
        .authoredWord = 0,
        .beginSuccess = nullptr,
        .beginFailure = Q7QbSixthAttackBegin,
        .resultSuccess = Q7QbSixthAttackSuccess,
        .resultFailure = Q7QbSixthAttackFailure},
    {.requiredQteStatus = 0x800,
        .retryReferenceFrames = 23,
        .maxTime = 16,
        .callbackTimeA = 0,
        .callbackTimeB = 0,
        .callbackTimeC = 10,
        .authoredWord = 0,
        .beginSuccess = nullptr,
        .beginFailure = Q7QbFinalAttackBegin,
        .resultSuccess = Q7QbFinalAttackSuccess,
        .resultFailure = Q7QbFinalAttackFailure},
    {.requiredQteStatus = 0x100,
        .retryReferenceFrames = 14,
        .maxTime = 14,
        .callbackTimeA = 0,
        .callbackTimeB = 0,
        .callbackTimeC = 0,
        .authoredWord = 0,
        .beginSuccess = nullptr,
        .beginFailure = nullptr,
        .resultSuccess = Q7QbFinalCounterSuccess,
        .resultFailure = Q7QbFinalCounterFailure},
};
std::int32_t PlayStandingQteRetryVoices();
void InitializeQ7QbEvent();
void RunQ7QbEvent(std::uintptr_t argument);
extern const ScriptAreaDefinition q7qbScriptDefinition;
} // namespace shenmue::s2::areas
