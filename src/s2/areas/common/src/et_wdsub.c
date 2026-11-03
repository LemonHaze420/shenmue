#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/training_seqp.hpp"

#if defined(SHENMUE_TRAINING_FIGHT)

const char trainingBattleRestart[] = "BtReStart %d\n";
const char trainingBattleStart[] = "BtStart\n";

// @verified - d
void TrainingAttackSequence(void *data)
{
    Thr *thr = data;
    TrainingWork *work = ThrGetWork(thr);
    TrainingStats *state = &work->control;
    shen_name player = ThrGetUser(thr, 0), opponent = ThrGetUser(thr, 1);
    int flags = ThrGetUser(thr, 2), style = ThrGetUser(thr, 3);
    void *package = (void *)ThrGetUser(thr, 4);
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            ThrChangeSeq(thr, 100, 1);
            break;
        case 100:
            FTBL.EV_EnableCharacterDisplay(player);
            if (opponent == TRAINING_DEKU)
                FTBL.EV_DisableCharacterDisplay(TRAINING_DEKU);
            else
                FTBL.EV_EnableCharacterDisplay(opponent);
            if (work->control.fight.fightSuspended) {
                int motion;
                ResumeTrainingFight(work, player, opponent);
                if (state->restartMotion == 1)
                    motion = 409;
                else if (state->restartMotion == 2)
                    motion = 408;
                else
                    motion = 0;
                state->restartMotion = 0;
                FTBL.DebugLogToFileRemoved(trainingBattleRestart, FTBL.GetFightMotionRuntime(motion));
            } else {
                FTBL.shenStartFGST(10, 10, 0, 0);
                if (package) FTBL.vfmDispatchControl(2, 0, package, 1);
                if (opponent == TRAINING_LKN) {
                    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(player), 0, 0, 0, 0, 46);
                    FTBL.vfCopyPosFromEntity(0, 0, 109.98f, 204.42f, 2.5f);
                } else
                    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(player), 0, 0, 0, 0, 16);
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(player), 0, 0x08000000, flags);
                FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(opponent), 0, 1, 1, 100, style);
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(player), 0, 0x10000, 0);
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(opponent), 0, 0x10000, 0);
                if (opponent == TRAINING_LKN) FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(TRAINING_LKN), 0, 0x10000000, 0);
                FTBL.vfmQueueFightStart(2, 99, 1.0f, 9, -1);
                FTBL.EV_vfBattleUtil(0, 1, 1);
                FTBL.DebugLogToFileRemoved(trainingBattleStart);
            }
            ThrChangeSeq(thr, 101, 1);
            break;
        case 101:
            if (work->control.fight.fightSuspended) ThrChangeSeq(thr, 102, 1);
            break;
        case 102:
            ThrSignalParent(thr, 1);
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
// @verified - d
void TrainingStopFight(void)
{
    FTBL.check_dead_status(1);
}
// @verified - d
void TrainingSuspendFight(TrainingWork *work, shen_name character, ...)
{
    work->control.fight.fightSuspended = 1;
    FTBL.SetFightStateFlags();
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(character), 1);
}
// @verified - d
void ResumeTrainingFight(TrainingWork *work, shen_name character, ...)
{
    int zero = 0;
    TrainingFightState *fight = &work->control.fight;
    fight->fightSuspended = zero;
    FTBL.SetFightFrame(1);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(character), 2);
}
#else

const char trainingCameraLog[] = "cam: %d\n";
const char trainingYawLog[] = "ma:%f/n";
const char trainingSource[] = __FILE__;
const char trainingStepLog[] = "diff y ang : %f (%f %f)\n";
const char trainingStepBack[] = "B\n";
const char trainingStepLeftRightBack[] = "L_RB\n";
const char trainingStepRight[] = "R\n";
const char trainingStepRightFront[] = "RF\n";
const char trainingStepFront[] = "F\n";
const char trainingStepLeftRightFront[] = "L_RF\n";
const char trainingStepLeft[] = "L\n";
const char trainingStepLeftBack[] = "LB\n";
const char trainingRepositionLimit[] = "WZ ModoriMove Limit Over !!!!!!!!!!!\n";

// @verified - d
void IncrementTrainingFrames(TrainingWork *work)
{
    work->frames++;
}
// @verified - d
float TrainingSine(int angle)
{
    return FTBL.CalculateFixedSine(angle);
}
// @verified - d
float TrainingCosine(int angle)
{
    return FTBL.CalculateFixedCosine(angle);
}
// @verified - d
float TrainingSquareRoot(float value)
{
    return __builtin_sqrtf(value);
}
// @verified - d
float TrainingRandom(float bound)
{
    return FTBL.GetRandomNumberInRange(bound);
}
// @verified - d
float TrainingPlanarDistance(const Vector3 *a, const Vector3 *b)
{
    return FTBL.CalculatePlanarDistanceBetweenPoints(a, b);
}
// @verified - d
void TrainingSetCamera(TrainingWork *work, unsigned short camera)
{
    FTBL.DebugLogToFileRemoved(trainingCameraLog, work->camera);
    if (camera < 9000) camera = 9000;
    work->camera = camera;
    FTBL.EVS_LoopCamera(camera, 0, 0);
}
// @verified - d
void TrainingCameraSequence(void *data)
{
    Thr *thr = data;
    TrainingWork *work = ThrGetWork(thr);
    unsigned short camera = ThrGetUser(thr, 0);
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            work->camera = camera;
            {
                int selected = camera;
                FTBL.EVS_LoopCamera((unsigned short)selected, 0, 0);
            }
            ThrWaitFrames(thr, 100, 1, 2);
            break;
        case 100:
            ThrWaitEventCamera(thr, 101, 1);
            break;
        case 101:
            FTBL.EVS_LoopCamera((int)(unsigned short)work->camera, 0, 0);
            ThrWaitFrames(thr, 100, 1, 2);
            break;
        case 102:
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
// @verified - d
void TrainingQueueCamera(Thr *thr, int slot, unsigned short camera)
{
    TrainingWork *work = ThrGetWork(thr);
    Thr *child = ThrMakeChild(thr, TrainingCameraSequence, TRAINING_CAMERA, work, slot);
    ThrSetUserName(child, 0, camera);
}
// @verified - d
void TrainingKillCamera(Thr *thr, int slot)
{
    if (ThrGetChildTask(thr, slot)) ThrKillChild(thr, TRAINING_CAMERA, slot);
}
// @verified - d
void TrainingSetLookCamera(TrainingWork *work, unsigned short camera, shen_name character)
{
    FTBL.DebugLogToFileRemoved(trainingCameraLog, work->camera);
    work->camera = camera;
    FTBL.EVS_LoopCamera(camera, character, character);
}
// @verified - d
void TrainingLookCameraSequence(void *data)
{
    Thr *thr = data;
    TrainingWork *work = ThrGetWork(thr);
    unsigned short camera = ThrGetUser(thr, 0);
    shen_name character = ThrGetUser(thr, 1);
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            work->camera = camera;
            {
                int selected = camera;
                FTBL.EVS_LoopCamera((unsigned short)selected, character, character);
            }
            ThrWaitFrames(thr, 100, 1, 2);
            break;
        case 100:
            ThrWaitEventCamera(thr, 101, 1);
            break;
        case 101:
            FTBL.EVS_LoopCamera((int)(unsigned short)work->camera, character, character);
            ThrWaitFrames(thr, 100, 1, 2);
            break;
        case 102:
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
// @verified - d
void TrainingQueueLookCamera(Thr *thr, int slot, unsigned short camera, shen_name character)
{
    TrainingWork *work = ThrGetWork(thr);
    Thr *child;
    if (ThrGetChildTask(thr, slot)) ThrKillChild(thr, TRAINING_LOOK_CAMERA, slot);
    child = ThrMakeChild(thr, TrainingLookCameraSequence, TRAINING_LOOK_CAMERA, work, slot);
    ThrSetUserName(child, 0, camera);
    ThrSetUser(child, 1, character);
}
// @verified - d
void TrainingKillLookCamera(Thr *thr, int slot)
{
    if (ThrGetChildTask(thr, slot)) ThrKillChild(thr, TRAINING_LOOK_CAMERA, slot);
}
// @verified - d
void SetTrainingCharacterYaw(shen_name name, int yaw)
{
    Vector3i rotation;
    rotation.x = 0;
    rotation.y = yaw;
    rotation.z = 0;
    FTBL.EV_AT_SetAngY_0(name, 0x78000000, &rotation, 0);
}
// @verified - d
void TrainingSubtractCharacterYaw(shen_name name, int yaw)
{
    Vector3i rotation;
    FTBL.GetScenarioCharacterEulerAngles(name, &rotation, 0x02000000);
    FTBL.DebugLogToFileRemoved(trainingYawLog, rotation.y / 182.04445f);
    rotation.y = (rotation.y - yaw) & 0xffff;
    SetTrainingCharacterYaw(name, rotation.y);
}
// @verified - d
void TrainingFollowSequence(void *data)
{
    Thr *thr = data;
    TrainingWork *work = ThrGetWork(thr);
    shen_name target = ThrGetUser(thr, 0);
    int frames = ThrGetUser(thr, 1);
    int *count = ThrGetBuffer1Address(thr, 0);
    Vector3 position;
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            work->following = 1;
            *count = 0;
            ThrChangeSeq(thr, 100, 1);
            break;
        case 100:
            if (work->following) {
                FTBL.GetCharaPosition(target, 5, &position, 0x40000000);
                FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(TRAINING_RYO), &position, 0);
                if (frames != -1 && ++*count > frames) ThrKillOwn(thr);
            } else
                ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
// @verified - d
void TrainingTurnStepSequence(void *data)
{
    Thr *thr = data;
    shen_name name = ThrGetUser(thr, 0), target = ThrGetUser(thr, 1);
    unsigned int frame = ThrGetUser(thr, 2);
    int delay = ThrGetUser(thr, 3);
    Vector3 position;
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            ThrChangeSeq(thr, 100, 1);
            break;
        case 100:
            ThrWaitFrames(thr, 101, 1, delay);
            break;
        case 101:
            FTBL.GetCharaPosition(target, 5, &position, 0x40000000);
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(name), &position, 0);
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(name), 0);
            FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(name), frame, 0);
            TrainingSubtractCharacterYaw(name, 0);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(name), 0xf003, 0);
            ThrWaitFrames(thr, 102, 1, frame);
            break;
        case 102:
            FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(name), 8, 0);
            ThrSignalParent(thr, 1);
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
// @verified - d
void TrainingTurnSequence(void *data)
{
    Thr *thr = data;
    TrainingWork *work = ThrGetWork(thr);
    int name = ThrGetUser(thr, 0), target = ThrGetUser(thr, 1), frame = ThrGetUser(thr, 2), delay = ThrGetUser(thr, 3);
    Thr *child;
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            ThrChangeSeq(thr, 100, 1);
            break;
        case 100:
            ThrSignalParent(thr, 1);
            work->turning = 1;
            child = ThrMakeChild(thr, TrainingTurnStepSequence, TRAINING_TURN_STEP, work, 0);
            ThrSetUser(child, 0, name);
            ThrSetUser(child, 1, target);
            ThrSetUserName(child, 2, frame);
            ThrSetUserName(child, 3, delay);
            ThrSetChildDone(thr, 0, 0);
            ThrWaitChildResult(thr, 101, 1, 0);
            break;
        case 101:
            work->turning = 0;
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
// @verified - d
void TrainingMatchPositionSequence(void *data)
{
    Thr *thr = data;
    TrainingWork *work = ThrGetWork(thr);
    shen_name name = ThrGetUser(thr, 0), target = ThrGetUser(thr, 1);
    int delay = ThrGetUser(thr, 2);
    Vector3 position;
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            ThrChangeSeq(thr, 100, 1);
            break;
        case 100:
            work->turning = 1;
            FTBL.GetCharaPosition(target, -1, &position, 0x40000000);
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(name), &position, 0);
            ThrWaitFrames(thr, 101, 1, delay);
            break;
        case 101:
            FTBL.GetCharaPosition(target, 5, &position, 0x40000000);
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(name), &position, 0);
            work->turning = 0;
            ThrSignalParent(thr, 1);
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
// @verified - d
void TrainingClearFailures(TrainingWork *work)
{
    TrainingStats *state = &work->control;
    int i;
    work->turning = 0;
    i = 0;
    do {
        state->firstFailures[i] = 0;
        state->repeatedFailures[i] = 0;
        i++;
    } while (i < 10);
}
// @verified - d
int TrainingIsTurning(TrainingWork *work)
{
    return (unsigned char)work->turning;
}
// @verified - d
void TrainingLcdSequence(void *data)
{
    Thr *thr = data;
    TrainingWork *work = ThrGetWork(thr);
    TrainingLcdValues *lcd = &work->control.fight.lcd;
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            lcd->frames = 1;
            lcd->bitmap = 0;
            ThrChangeSeq(thr, 100, 1);
            break;
        case 100:
            if (lcd->frames && !--lcd->frames) {
                lcd->bitmap = 0;
                FTBL.lcdmInit(0);
            }
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
// @verified - d
void TrainingSetLcd(int *frames, const void *bitmap)
{
    frames[1] = (int)bitmap;
    if (!bitmap) {
        frames[0] = 0;
        FTBL.SetCombatEffectOrder(0);
    } else {
        frames[0] = 600;
        FTBL.SetCombatEffectOrder(1);
    }
    FTBL.lcdmInit((void *)frames[1]);
}
// @verified - d
void TrainingSetLcdOrder(int *frames, const void *bitmap, int order)
{
    frames[1] = (int)bitmap;
    if (!bitmap) {
        frames[0] = 0;
        FTBL.SetCombatEffectOrder(0);
    } else {
        if (!order) order = 1;
        frames[0] = 600;
        if (order)
            FTBL.SetCombatEffectOrder(1);
        else
            FTBL.SetCombatEffectOrder(0);
    }
    FTBL.lcdmInit((void *)frames[1]);
}
// @verified - d
int TrainingPlanarAngle(float x1, float z1, float x2, float z2)
{
    x1 = x2 - x1;
    z1 = z2 - z1;
    if (!((*(unsigned int *)&x1 >= 0x00800000U && *(unsigned int *)&x1 <= 0x7f7fffffU) ||
          (*(unsigned int *)&x1 >= 0x80800000U && *(unsigned int *)&x1 <= 0xff7fffffU) || *(unsigned int *)&x1 == 0 || *(unsigned int *)&x1 == 0x80000000U))
        FTBL.EventAssertNullsub(trainingSource, 685);
    if (!((*(unsigned int *)&z1 >= 0x00800000U && *(unsigned int *)&z1 <= 0x7f7fffffU) ||
          (*(unsigned int *)&z1 >= 0x80800000U && *(unsigned int *)&z1 <= 0xff7fffffU) || *(unsigned int *)&z1 == 0 || *(unsigned int *)&z1 == 0x80000000U))
        FTBL.EventAssertNullsub(trainingSource, 686);
    return FTBL.InverseFixedAtan2(x1, z1);
}
// @verified - d
int TrainingFacingDifference(int wanted, int current)
{
    int reverse = current - wanted;
    int delta = wanted - current;
    if ((delta & 0xffff) < (reverse & 0xffff)) {
        if (wanted > current)
            delta = -delta;
        else
            delta = -(delta & 0xffff);
    } else {
        if (wanted > current)
            delta = reverse & 0xffff;
        else
            delta = reverse & 0xffff;
    }
    return delta;
}
// @verified - d
void TrainingTurnWithOffset(shen_name name, const Vector3 *offset, int yaw)
{
    Vector3 transformed, position;
    Vector3i rotation;
    FTBL.GetCharaPosition(name, -1, &position, 0);
    FTBL.GetScenarioCharacterEulerAngles(name, &rotation, 0);
    rotation.y = (rotation.y + yaw) & 0xffff;
    SetTrainingCharacterYaw(name, rotation.y);
    transformed.x = TrainingSine(rotation.y) * offset->z + offset->x * TrainingCosine(rotation.y);
    transformed.z = offset->z * TrainingCosine(rotation.y) - offset->x * TrainingSine(rotation.y);
    transformed.x += position.x;
    transformed.y = position.y;
    transformed.z += position.z;
    FTBL.EV_AT_SetPos(name, 0x38000000, &position);
}
// @verified - d
int SelectTrainingStepMotion(shen_name name, const Vector3 *target)
{
    Vector3 position;
    Vector3i rotation, targetRotation;
    int diff, mirrored;
    int motion;
    FTBL.GetCharaPosition(name, -1, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(name, &rotation, 0x40000000);
    targetRotation.y = TrainingPlanarAngle(position.x, position.z, target->x, target->z);
    diff = TrainingFacingDifference(targetRotation.y, rotation.y);
    FTBL.DebugLogToFileRemoved(trainingStepLog, diff / 182.04445f, (float)rotation.y / 182.04445f, (float)targetRotation.y / 182.04445f);
    mirrored = FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 70, 1, 0);
    if (mirrored) {
        if (diff / 182.04445f < -157.5f || diff / 182.04445f > 157.5f) {
            motion = 5005;
            FTBL.DebugLogToFileRemoved(trainingStepBack);
        } else if (diff / 182.04445f < -112.5f) {
            motion = 5009;
            FTBL.DebugLogToFileRemoved(trainingStepLeftRightBack);
        } else if (diff / 182.04445f < -67.5f) {
            motion = 5011;
            FTBL.DebugLogToFileRemoved(trainingStepRight);
        } else if (diff / 182.04445f < -22.5f) {
            motion = 5012;
            FTBL.DebugLogToFileRemoved(trainingStepRightFront);
        } else if (diff / 182.04445f < 22.5f) {
            motion = 5006;
            FTBL.DebugLogToFileRemoved(trainingStepFront);
        } else if (diff / 182.04445f < 67.5f) {
            motion = 5010;
            FTBL.DebugLogToFileRemoved(trainingStepLeftRightFront);
        } else if (diff / 182.04445f < 112.5f) {
            motion = 5007;
            FTBL.DebugLogToFileRemoved(trainingStepLeft);
        } else {
            motion = 5008;
            FTBL.DebugLogToFileRemoved(trainingStepLeftBack);
        }
    } else {
        if (diff / 182.04445f < -157.5f || diff / 182.04445f > 157.5f) {
            motion = 5005;
            FTBL.DebugLogToFileRemoved(trainingStepBack);
        } else if (diff / 182.04445f < -112.5f) {
            motion = 5008;
            FTBL.DebugLogToFileRemoved(trainingStepLeftBack);
        } else if (diff / 182.04445f < -67.5f) {
            motion = 5007;
            FTBL.DebugLogToFileRemoved(trainingStepLeft);
        } else if (diff / 182.04445f < -22.5f) {
            motion = 5010;
            FTBL.DebugLogToFileRemoved(trainingStepLeftRightFront);
        } else if (diff / 182.04445f < 22.5f) {
            motion = 5006;
            FTBL.DebugLogToFileRemoved(trainingStepFront);
        } else if (diff / 182.04445f < 67.5f) {
            motion = 5012;
            FTBL.DebugLogToFileRemoved(trainingStepRightFront);
        } else if (diff / 182.04445f < 112.5f) {
            motion = 5011;
            FTBL.DebugLogToFileRemoved(trainingStepRight);
        } else {
            motion = 5009;
            FTBL.DebugLogToFileRemoved(trainingStepLeftRightBack);
        }
    }
    return motion;
}
// @verified - d
void TrainingRepositionSequence(void *data)
{
    Thr *thr = data;
    shen_name name = ThrGetUser(thr, 0);
    int index = ThrGetUser(thr, 1);
    int motion = ThrGetUser(thr, 2);
    const Vector3 *target = (void *)ThrGetUser(thr, 3);
    int *counters = (void *)ThrGetUser(thr, 4);
    int *repetitions = ThrGetBuffer1Address(thr, 0);
    float *distance = (void *)ThrGetBuffer1Address(thr, 1);
    Vector3 position;
    do {
        ThrResetLoopCount(thr);
    } while (0);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            *repetitions = 0;
            *distance = 9999.0f;
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 2);
            motion = SelectTrainingStepMotion(name, target);
            ThrSetUserName(thr, 2, motion);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(name), motion, 0);
            if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 70, 1, 0)) FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 69, 0, 0);
            TrainingSetMotionFlag(name);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(name), 1.0f, 0);
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(name), 1);
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 66, 0, 0);
            ThrChangeSeq(thr, 100, 1);
            break;
        case 100:
            if (counters[index]) {
                float next;
                FTBL.GetCharaPosition(name, -1, &position, 0x40000000);
                next = TrainingPlanarDistance(&position, target);
                if (next > *distance || next < 0.05f) {
                    counters[index] = 0;
                    ThrChangeSeq(thr, 101, 1);
                    break;
                }
                {
                    *distance = next;
                    if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 65, 1, 0)) {
                        motion = SelectTrainingStepMotion(name, target);
                        ThrSetUserName(thr, 2, motion);
                        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(name), motion, 0);
                        if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 70, 1, 0))
                            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 69, 0, 0);
                        if (++*repetitions > 7) {
                            FTBL.DebugLogToFileRemoved(trainingRepositionLimit);
                            goto finish;
                        }
                    }
                    break;
                }
            finish:
                counters[index] = 0;
            }
            ThrChangeSeq(thr, 101, 1);
            break;
        case 101:
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(name), 306, 0);
            if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 70, 1, 0)) FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 69, 0, 0);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(name), 1.0f, 0);
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(name), 1);
            ThrKillOwn(thr);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}
// @verified - d
void QueueTrainingReposition(Thr *thr, int slot, shen_name name, int index, const Vector3 *target)
{
    TrainingWork *work = ThrGetWork(thr);
    int *counters = work->control.repositionFrames;
    Thr *child;
    Vector3 position;
    counters[index] = 0;
    FTBL.GetCharaPosition(name, -1, &position, 0x40000000);
    position.y = target->y;
    FTBL.EV_AT_SetPos(name, 0x78000000, &position);
    FTBL.EV_SetCharaControllerFlags(name, 1, 0);
    counters[index] = 1000;
    child = ThrMakeChild(thr, TrainingRepositionSequence, TRAINING_REPOSITION, work, slot);
    ThrSetUser(child, 0, name);
    ThrSetUserName(child, 1, index);
    ThrSetUserName(child, 2, 0);
    ThrSetUserLong(child, 3, (long)target);
    ThrSetUserLong(child, 4, (long)counters);
}
// @verified - d
int TrainingIsRepositioning(TrainingWork *work, int index)
{
    int *counters = work->control.repositionFrames;
    return counters[index] != 0;
}
// @verified - d
int TrainingResultNullsub(int attempts, int error)
{
    return 0;
}
// @verified - d
int TrainingResultError(TrainingResult *result, int index)
{
    return result->error;
}
// @verified - d
void TrainingCopyResult(TrainingResult *destination, const TrainingResult *source, int index)
{
    int copied = 0;
    TrainingResult *row = &destination[copied];
    row->status = source->status;
    row->phase = source->phase;
    row->error = source->error;
    row->steps = source->steps;
    row->force = source->force;
    row->timing = source->timing;
    row->retained06 = source->retained06;
    row->flags = source->flags;
    row->detail = source->detail;
}
// @verified - d
void TrainingTimerSequence(void *data)
{
    TrainingWork *work = ThrGetWork(data);
    TrainingStats *state = &work->control;
    state->elapsed++;
    state->adviceElapsed++;
}
// @verified - d
void TrainingAdviceTimerSequence(void *data)
{
    Thr *thr = data;
    TrainingWork *work = ThrGetWork(thr);
    int controlOffset = (char *)&work->control - (char *)work;
    TrainingStats *state = (TrainingStats *)((char *)work + controlOffset);
    int practice = ThrGetUser(thr, 0), mode = ThrGetUser(thr, 1);
    if (practice == 9) {
        if (state->previousRepetitions != state->consecutiveRepetitions && state->consecutiveRepetitions % 3 == 0) *(int *)((char *)work + controlOffset) = 1;
        if (state->elapsed && state->elapsed % 450 == 0) {
            state->advicePending = 1;
            state->timedAdvice = 1;
        }
    } else
        switch (mode) {
        case 0:
            if ((state->adviceElapsed && state->adviceElapsed % 300 == 0) ||
                (state->previousRepetitions != state->consecutiveRepetitions && state->consecutiveRepetitions % 3 == 0))
                state->advicePending = 1;
            if (state->elapsed && state->elapsed % 2700 == 0) {
                state->advicePending = 1;
                state->practiceExpired = 1;
            } else if (state->elapsed && state->elapsed % 1800 == 0) {
                state->advicePending = 1;
                state->practiceWarning = 1;
            }
            break;
        case 1:
            if ((state->adviceElapsed && state->adviceElapsed % 300 == 0) ||
                (state->previousRepetitions != state->consecutiveRepetitions && state->consecutiveRepetitions % 3 == 0))
                state->advicePending = 1;
            if (state->elapsed && state->elapsed % 1800 == 0) {
                state->advicePending = 1;
                state->practiceExpired = 1;
            }
            break;
        case 2:
            if ((state->adviceElapsed && state->adviceElapsed % 300 == 0) ||
                (state->previousRepetitions != state->consecutiveRepetitions && state->consecutiveRepetitions % 3 == 0))
                state->advicePending = 1;
            break;
        }
    state->previousRepetitions = state->consecutiveRepetitions;
}
// @verified - d
int TrainingKillOwnHandle(Thr **handle)
{
    int changed = 0;
    if (*handle) {
        ThrKillOwn(*handle);
        *handle = 0;
        changed = 1;
    }
    return changed;
}
// @verified - d
int TrainingKillChildHandle(Thr **handle, shen_name id, int index)
{
    int changed = 0;
    if (*handle) {
        ThrKillChild(*handle, id, index);
        *handle = 0;
        changed = 1;
    }
    return changed;
}
// @verified - d
int TrainingKillTreeHandle(Thr **handle)
{
    int changed = 0;
    if (*handle) {
        ThrKillTree(*handle);
        *handle = 0;
        changed = 1;
    }
    return changed;
}
// @verified - d
int TrainingKillOtherHandle(Thr **handle, shen_name id)
{
    int changed = 0;
    if (*handle) {
        ThrKillOther(*handle, id);
        *handle = 0;
        changed = 1;
    }
    return changed;
}
// @verified - d
int TrainingKillOtherTreeHandle(Thr **handle, shen_name id)
{
    int changed = 0;
    if (*handle) {
        ThrKillOtherTree(*handle, id);
        *handle = 0;
        changed = 1;
    }
    return changed;
}

// @verified - d
void DisableTrainingAlpha(shen_name name)
{
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}
// @verified - d
void TrainingEnableAlpha(shen_name name)
{
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}
// @verified - d
void ClearTrainingMotionFlag(shen_name name)
{
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 71, 0, 0);
}
// @verified - d
void TrainingSetMotionFlag(shen_name name)
{
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 71, 2, 0);
}
// @verified - d
int TrainingTestMotionFlag(shen_name name)
{
    if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 71, 1, 0))
        return 0;
    else
        return 1;
}
// @verified - d
void TrainingSetMotion(shen_name name, unsigned int motion, float loopStart, float startFrame, float endFrame, float speed)
{
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(name), motion, 0);
    FTBL.SetMotionLayerFrameRange(FTBL.scenFindMotionwork(name), loopStart, startFrame, endFrame, 0);
    FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(name), speed, 0);
}
// @verified - d
void TrainingAssignFigure(shen_name name, unsigned char mode)
{
    FTBL.figpAssignModeFor(name, mode);
}
// @verified - d
void TrainingClearFigure(shen_name name)
{
    FTBL.figpAssignModeFor(name, 0);
}
// @verified - d
void TrainingSpeak(short a, short b, short c, short d, short selection)
{
    short values[4];
    values[0] = a;
    values[1] = b;
    values[2] = c;
    values[3] = d;
    FTBL.SetupControlSpeakAndUI(values, selection, 0, 2);
}
// @verified - d
void ResetTrainingAuthorCharacter(shen_name name)
{
    FTBL.EV_EnableCharacterDisplay(name);
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(name), 0, 0);
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(name), 0, 3);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 0);
    FTBL.EV_SetCharaControllerFlags(name, 2, 0);
    FTBL.EV_SetCharaControllerFlags(name, 3, 0);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.EV_SetCharaControllerFlags(name, 1, 0);
    DisableTrainingAlpha(name);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(name), 0, 0);
    FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(name), 1);
    FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(name), 1.0f, 0);
}
// @verified - d
void RestoreTrainingCharacter(shen_name name)
{
    float reflector[4];
    reflector[0] = 1.0f;
    reflector[1] = 0.3f;
    reflector[2] = 0.3f;
    reflector[3] = 0.3f;
    FTBL.EV_SetCharaControllerFlags(name, 2, 0);
    FTBL.EV_SetCharaControllerFlags(name, 3, 0);
    DisableTrainingAlpha(name);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 1);
    FTBL.SetScenarioReflectorBoardValues(name, reflector);
    FTBL.SetRefBoardParamsForCharaById(name, 1);
}

#endif

#endif
