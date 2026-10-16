#pragma once
#include "common/types.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name REN_QTE_PLAYER{SHEN_NAME(RYO_)}, REN_QTE_REN{SHEN_NAME(REN_)}, REN_QTE_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name REN_QTE_SECOND_BASE{SHEN_NAME(BAS2)}, REN_QTE_DOOR{SHEN_NAME(DOR1)}, REN_QTE_BROKEN_DOOR{SHEN_NAME(DOR2)};
inline constexpr shen_name REN_QTE_KD7{SHEN_NAME(KD7_)}, REN_QTE_KK1{SHEN_NAME(KK1_)}, REN_QTE_KT1{SHEN_NAME(KT1_)}, REN_QTE_KU7{SHEN_NAME(KU7_)};
inline constexpr shen_name REN_QTE_INVALID{0xffffffffU}, REN_QTE_EVENT_TASK{SHEN_NAME(EVNT)}, REN_QTE_SOUND_FADE_TASK{SHEN_NAME(sndf)};
inline constexpr shen_name REN_QTE_DOOR_TASK{SHEN_NAME(door)}, REN_QTE_DOOR_DYNAMICS{SHEN_NAME(DOOR)};
inline constexpr shen_name REN_QTE_INIT{SHEN_NAME(INIT)}, REN_QTE_START{SHEN_NAME(STRT)}, REN_QTE_INPUT{SHEN_NAME(QTE_)};
inline constexpr shen_name REN_QTE_SUCCESS{SHEN_NAME(SUCC)}, REN_QTE_FAILURE{SHEN_NAME(FAIL)}, REN_QTE_FINISH{SHEN_NAME(FINI)},
    REN_QTE_CAPTURE{SHEN_NAME(CAPT)};
struct RenQteCamera {
    std::int32_t camera;
    shen_name attachment;
    std::int32_t delay;
};
struct RenQteVoice {
    const char *voice;
    std::int32_t delay;
    std::uint32_t reserved0c;
};
struct RenQteEntry {
    std::int32_t type;
    float startX, startZ;
    std::int32_t startYaw;
    float endX, endZ;
    std::int32_t endYaw;
    std::uint32_t command;
    float approachDistance;
    std::uint32_t reserved24;
    const RenQteCamera *initialCameras;
    const RenQteCamera *successCameras;
    const RenQteCamera *failureCameras;
    const RenQteVoice *initialVoices;
    const RenQteVoice *successVoices;
    const RenQteVoice *failureVoices;
    std::int32_t successFrames, failureFrames;
    float cameraMotionScale, cameraYOffset;
};
struct RenQteWork {
    shen_name base, state;
    std::int32_t retryCount, lives, doorAttempts, timer;
    std::uint64_t hidePursuers;
    float triggerDistance;
    std::uint32_t reserved24;
    RenQteEntry *entry;
};
struct RenQteCrushDoorWork {
    std::int32_t wait, frame, lastFrame, restoreIntactDoor;
};
static_assert(sizeof(RenQteCamera) == 12 && sizeof(RenQteVoice) == 16);
static_assert(sizeof(RenQteEntry) == 104 && offsetof(RenQteEntry, initialCameras) == 0x28);
static_assert(offsetof(RenQteEntry, initialVoices) == 0x40 && offsetof(RenQteEntry, successFrames) == 0x58);
static_assert(offsetof(RenQteEntry, cameraMotionScale) == 0x60 && offsetof(RenQteEntry, cameraYOffset) == 0x64);
static_assert(sizeof(RenQteWork) == 0x30 && offsetof(RenQteWork, state) == 4 && offsetof(RenQteWork, timer) == 0x14);
static_assert(offsetof(RenQteWork, hidePursuers) == 0x18 && offsetof(RenQteWork, triggerDistance) == 0x20);
static_assert(offsetof(RenQteWork, entry) == 0x28 && sizeof(RenQteCrushDoorWork) == 0x10);
extern std::int32_t renQteRetryDifficulty;
bool UpdateRenQteEvent(RenQteWork *work);
void PlayRenQteCameras(const RenQteCamera *cameras);
void PlayRenQteVoices(const RenQteVoice *voices);
void RenQteFadeOutSoundCallback(void *);
void InitializeRenQteEntry(RenQteWork *);
void BeginRenQteDoorApproach(RenQteWork *);
void BeginRenQteExitApproach(RenQteWork *);
void BeginRenQteRightExitApproach(RenQteWork *);
void BeginRenQteRunApproach(RenQteWork *);
void WaitRenQteApproach(RenQteWork *);
void FinishRenQteExit(RenQteWork *);
void CaptureRenQtePursuers(RenQteWork *);
void FinishRenQteRightExit(RenQteWork *);
void PollRenQteInput(RenQteWork *);
void RenQteUnusedResult(RenQteWork *);
void TurnRenQteLeftSuccess(RenQteWork *);
void TurnRenQteLeftFailure(RenQteWork *);
void TurnRenQteRightSuccess(RenQteWork *);
void TurnRenQteRightFailure(RenQteWork *);
void ContinueRenQteRunSuccess(RenQteWork *);
void ContinueRenQteRunFailure(RenQteWork *);
void JumpRenQtePitSuccess(RenQteWork *);
void JumpRenQtePitFailure(RenQteWork *);
void DodgeRenQtePoleSuccess(RenQteWork *);
void DodgeRenQtePoleFailure(RenQteWork *);
void SwitchRenQteLeaderSuccess(RenQteWork *);
void SwitchRenQteLeaderFailure(RenQteWork *);
void BreakRenQteDoorSuccess(RenQteWork *);
void BreakRenQteDoorFailure(RenQteWork *);
void RenQteCrushDoorCallback(void *);
using RenQteResultCallback = void (*)(RenQteWork *);
inline constexpr RenQteResultCallback renQteSuccessCallbacks[]{RenQteUnusedResult, TurnRenQteLeftSuccess, TurnRenQteRightSuccess, ContinueRenQteRunSuccess,
    JumpRenQtePitSuccess, DodgeRenQtePoleSuccess, SwitchRenQteLeaderSuccess, BreakRenQteDoorSuccess};
inline constexpr RenQteResultCallback renQteFailureCallbacks[]{RenQteUnusedResult, TurnRenQteLeftFailure, TurnRenQteRightFailure, ContinueRenQteRunFailure,
    JumpRenQtePitFailure, DodgeRenQtePoleFailure, SwitchRenQteLeaderFailure, BreakRenQteDoorFailure};
} // namespace shenmue::s2
