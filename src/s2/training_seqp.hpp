#pragma once
#include "s2/seqp_event.hpp"
#include "common/math.hpp"
namespace shenmue::s2 {
extern std::int32_t trainingGateTargetYaw;
extern std::int32_t trainingPracticeTargetYaw;
extern Vector3i trainingSparRotation;
inline constexpr shen_name TRAINING_LOOK_CAMERA{SHEN_NAME(LLCP)}, TRAINING_TURN_STEP{SHEN_NAME(TUSU)};
void TrainingTurnSequence(void *);
void QueueTrainingCameraLookAt(CompiledSequenceWork *, std::uint16_t camera, shen_name character);
void SetTrainingCharacterYaw(shen_name character, std::int32_t yaw);
void TrainingAttackSequence(void *);
void RetireTrainingFight(void *event, shen_name character);
void TrainingTimerSequence(void *);
void TrainingAdviceTimerSequence(void *);
bool TrainingNeedsFacingReset(std::int32_t practice);
void SetTrainingCameraSequence(CompiledSequenceWork *sequence, std::int32_t camera);
void QueueTrainingReposition(CompiledSequenceWork *sequence, std::int32_t slot, shen_name character, std::int32_t index, const Vector3 *target);
void ClearTrainingSequenceChildren(CompiledSequenceWork *);
void RetireTrainingSequence(CompiledSequenceWork *);
void ResetTrainingAuthorCharacter(shen_name character);
} // namespace shenmue::s2
