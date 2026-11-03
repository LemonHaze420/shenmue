#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_knmn.hpp"
#define TRAINING_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define TRAINING_LKN SHEN_NAME4('L', 'K', 'N', '_')
#define TRAINING_DEKU SHEN_NAME4('D', 'E', 'K', 'U')
#define TRAINING_CAMERA SHEN_NAME4('L', '_', 'C', 'P')
#define TRAINING_LOOK_CAMERA SHEN_NAME4('L', 'L', 'C', 'P')
#define TRAINING_TURN_STEP SHEN_NAME4('T', 'U', 'S', 'U')
#define TRAINING_REPOSITION SHEN_NAME4('B', 'T', 'S', 'P')
typedef KnmnPracticeWork TrainingWork;
typedef KnmnPracticeControl TrainingStats;
typedef KnmnMoveResult TrainingResult;
extern const char trainingBattleRestart[], trainingBattleStart[], trainingCameraLog[], trainingYawLog[], trainingSource[], trainingStepLog[],
    trainingStepBack[], trainingStepLeftRightBack[], trainingStepRight[], trainingStepRightFront[], trainingStepFront[], trainingStepLeftRightFront[],
    trainingStepLeft[], trainingStepLeftBack[], trainingRepositionLimit[];
void TrainingAttackSequence(void *);
void TrainingStopFight(void);
void TrainingSuspendFight(TrainingWork *, shen_name, ...);
void ResumeTrainingFight(TrainingWork *, shen_name, ...);
void IncrementTrainingFrames(TrainingWork *);
float TrainingSine(int);
float TrainingCosine(int);
float TrainingSquareRoot(float);
float TrainingRandom(float);
float TrainingPlanarDistance(const Vector3 *, const Vector3 *);
void TrainingSetCamera(TrainingWork *, unsigned short);
void TrainingCameraSequence(void *);
void TrainingQueueCamera(Thr *, int, unsigned short);
void TrainingKillCamera(Thr *, int);
void TrainingSetLookCamera(TrainingWork *, unsigned short, shen_name);
void TrainingLookCameraSequence(void *);
void TrainingQueueLookCamera(Thr *, int, unsigned short, shen_name);
void TrainingKillLookCamera(Thr *, int);
void SetTrainingCharacterYaw(shen_name, int);
void TrainingSubtractCharacterYaw(shen_name, int);
void TrainingFollowSequence(void *);
void TrainingTurnStepSequence(void *);
void TrainingTurnSequence(void *);
void TrainingMatchPositionSequence(void *);
void TrainingClearFailures(TrainingWork *);
int TrainingIsTurning(TrainingWork *);
void TrainingLcdSequence(void *);
void TrainingSetLcd(int *, const void *);
void TrainingSetLcdOrder(int *, const void *, int);
int TrainingPlanarAngle(float, float, float, float);
int TrainingFacingDifference(int, int);
void TrainingTurnWithOffset(shen_name, const Vector3 *, int);
int SelectTrainingStepMotion(shen_name, const Vector3 *);
void TrainingRepositionSequence(void *);
void QueueTrainingReposition(Thr *, int, shen_name, int, const Vector3 *);
int TrainingIsRepositioning(TrainingWork *, int);
int TrainingResultNullsub(int, int);
int TrainingResultError(TrainingResult *, int);
void TrainingCopyResult(TrainingResult *, const TrainingResult *, int);
void TrainingTimerSequence(void *);
void TrainingAdviceTimerSequence(void *);
int TrainingKillOwnHandle(Thr **);
int TrainingKillChildHandle(Thr **, shen_name, int);
int TrainingKillTreeHandle(Thr **);
int TrainingKillOtherHandle(Thr **, shen_name);
int TrainingKillOtherTreeHandle(Thr **, shen_name);
void DisableTrainingAlpha(shen_name);
void TrainingEnableAlpha(shen_name);
void ClearTrainingMotionFlag(shen_name);
void TrainingSetMotionFlag(shen_name);
int TrainingTestMotionFlag(shen_name);
void TrainingSetMotion(shen_name, unsigned int, float, float, float, float);
void TrainingAssignFigure(shen_name, unsigned char);
void TrainingClearFigure(shen_name);
void TrainingSpeak(short, short, short, short, short);
void ResetTrainingAuthorCharacter(shen_name);
void RestoreTrainingCharacter(shen_name);

#else  
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
#endif
