#include "s2/areas/knmn_practice.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/training_seqp.hpp"
namespace shenmue::s2::areas {
namespace {
void KnmnStopPracticeTasks(KnmnPracticeWork &work) {
    if (work.dialogueSequence) {
        RetireTrainingSequence(work.dialogueSequence);
        work.dialogueSequence = nullptr;
    }
    if (work.cameraSequence) {
        KnmnKillSequence(work.cameraSequence);
        work.cameraSequence = nullptr;
    }
    if (work.timerSequence) {
        KnmnKillSequence(work.timerSequence);
        work.timerSequence = nullptr;
    }
    if (work.adviceSequence) {
        KnmnKillSequence(work.adviceSequence);
        work.adviceSequence = nullptr;
    }
}
void KnmnStartPracticeTimers(CompiledSequenceWork &sequence, KnmnPracticeWork &work, std::int64_t practice) {
    work.timerSequence = KnmnChild(sequence, TrainingTimerSequence, SHEN_NAME(TIMM), 4);
    work.adviceSequence = KnmnChild(sequence, TrainingAdviceTimerSequence, SHEN_NAME(WADV), 5);
    KnmnValue(work.adviceSequence, 0x138, static_cast<std::int32_t>(practice));
    KnmnValue(work.adviceSequence, 0x140, std::int32_t{2});
}
void KnmnSelectPracticeCamera(CompiledSequenceWork &sequence, KnmnPracticeWork &work, std::int64_t practice) {
    std::int32_t camera = 0;
    if (work.changeCamera) {
        if (practice <= 3 || practice == 9)
            camera = 9010 + (work.practiceCamera == 9010);
        work.changeCamera = 0;
    } else
        camera = work.practiceCamera;
    work.practiceCamera = camera;
    SetTrainingCameraSequence(&sequence, camera);
}
void KnmnWaitReposition(CompiledSequenceWork &sequence, std::uint16_t next) {
    sequence.reserved04 = next;
    KnmnAdvance(sequence, 133);
}
} // namespace
void KnmnPracticeDecisionSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    const std::int64_t practice = KnmnValue<std::int64_t>(data, 0x138), instruction = KnmnValue<std::int64_t>(data, 0x140);
    const std::int64_t second = KnmnValue<std::int64_t>(data, 0x148), success = KnmnValue<std::int64_t>(data, 0x150);
    const std::int64_t retry = KnmnValue<std::int64_t>(data, 0x160), restart = KnmnValue<std::int64_t>(data, 0x168),
                       finish = KnmnValue<std::int64_t>(data, 0x170);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            if (!work.turning)
                KnmnAdvance(sequence, 100);
            break;
        case 100:
            work.practice = static_cast<std::int32_t>(practice);
            work.advicePending = 0;
            work.timedAdvice = 0;
            work.practiceWarning = 0;
            work.practiceExpired = 0;
            work.repetitions = 0;
            work.consecutiveRepetitions = 0;
            work.previousRepetitions = 0;
            work.attemptedMoves = 0;
            work.changeCamera = 1;
            work.restartMotion = 1;
            work.cameraSequence = nullptr;
            work.dialogueSequence = nullptr;
            work.timerSequence = nullptr;
            work.adviceSequence = nullptr;
            KnmnResetActors(static_cast<std::int32_t>(practice));
            EV_AT_SetPos(KNMN_KEM, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_TEACHER_PRACTICE_POSITION);
            EV_AT_SetAngY(KNMN_KEM, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 60984, .z = 0});
            SetMotionRegionFlag(scenFindMotionwork(KNMN_KEM), 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), 5193, MotionRegion::LowerBody);
            KnmnValue(data, 0x250, std::int32_t{1});
            KnmnAdvance(sequence, 101);
            break;
        case 101:
            KnmnSelectPracticeCamera(sequence, work, practice);
            work.adviceElapsed = 0;
            KnmnStartPracticeTimers(sequence, work, practice);
            work.attempts = 0;
            KnmnValue(data, 0x248, std::int32_t{0});
            KnmnAdvance(sequence, 102);
            break;
        case 102: {
            KnmnValue(data, 0x24c, std::int32_t{0});
            if (work.elapsed >= 10800) {
                KnmnValue(data, 0x88, static_cast<std::int32_t>(finish));
                sequence.reserved1a0 = 1;
                KnmnAdvance(sequence, 132);
                break;
            }
            if (KnmnValue<std::int32_t>(data, 0x250)) {
                KnmnSelectPracticeCamera(sequence, work, practice);
                KnmnResetActors(static_cast<std::int32_t>(practice));
            }
            CompiledSequenceWork *child = KnmnChild(sequence, KnmnPracticeSequence, SHEN_NAME(WDTP), 0);
            KnmnValue(child, 0x138, static_cast<std::int32_t>(instruction));
            KnmnValue(child, 0x140, static_cast<std::int32_t>(second));
            sequence.resultSlot = 0;
            KnmnWait(sequence, 8, 103);
            sequence.children[0] = nullptr;
            break;
        }
        case 103:
            KnmnValue(data, 0x250, std::int32_t(KnmnNeedsFacingReset(static_cast<std::int32_t>(practice))));
            if (work.results[0].phase == 2 && practice == 9 && work.advicePending)
                KnmnValue(data, 0x250, std::int32_t{0});
            work.restartMotion = work.results[0].error != 20 || work.timedAdvice || work.practiceWarning || work.practiceExpired || work.advicePending;
            work.changeCamera = 1;
            KnmnValue(&work, 0x84, KnmnValue<std::uint64_t>(&work, 0x78));
            KnmnValue(&work, 0x8c, KnmnValue<std::uint8_t>(&work, 0x80));
            if (work.results[0].error != 20)
                ++work.attemptedMoves;
            KnmnAdvance(sequence, work.results[0].phase == 2 ? 104 : 116);
            break;
        case 104:
            if (!KnmnValue<std::int32_t>(data, 0x250))
                QueueTrainingReposition(&sequence, 2, KNMN_RYO, 0, &KNMN_PLAYER_PRACTICE_POSITION);
            ++work.repetitions;
            ++work.consecutiveRepetitions;
            KnmnWait(sequence, 2, 105, 1);
            break;
        case 105:
            if (work.advicePending) {
                KnmnStopPracticeTasks(work);
                work.result = work.results[1].error;
                work.resultIndex = 0;
                KnmnAdvance(sequence, (work.timedAdvice | work.practiceWarning | work.practiceExpired) ? 106 : 114);
            } else if (!KnmnValue<std::int32_t>(data, 0x250))
                KnmnWaitReposition(sequence, 130);
            else
                KnmnAdvance(sequence, 130);
            break;
        case 106:
            mtwkSetExtraFlag(scenFindMotionwork(KNMN_RYO), 6, 0);
            if (!KnmnValue<std::int32_t>(data, 0x250))
                KnmnWaitReposition(sequence, 107);
            else
                KnmnAdvance(sequence, 107);
            break;
        case 107:
            if (work.practiceExpired && work.attemptedMoves >= 9) {
                if (practice)
                    break;
                KnmnShowPracticeLcd(work);
            } else if (practice)
                break;
            KnmnChild(sequence, KnmnFeedbackSequence, SHEN_NAME(FS_1), 1);
            sequence.resultSlot = 1;
            KnmnWait(sequence, 8, 108);
            break;
        case 108: {
            CompiledSequenceWork *child = KnmnChild(sequence, KnmnExitPracticeSequence, SHEN_NAME(EALP), 1);
            KnmnValue(child, 0x138, std::int32_t{1});
            KnmnValue(child, 0x140, static_cast<std::int32_t>(retry));
            KnmnValue(child, 0x148, static_cast<std::int32_t>(restart));
            KnmnValue(child, 0x150, static_cast<std::int32_t>(finish));
            KnmnValue(child, 0x158, static_cast<std::int32_t>(practice));
            sequence.resultSlot = 1;
            KnmnWait(sequence, 8, 112);
            break;
        }
        case 109:
        case 111:
        case 122:
        case 123:
        case 124:
        case 125:
            break;
        case 112:
            KnmnValue(data, 0x88, KnmnValue<std::int32_t>(data, 0xbc));
            sequence.reserved1a0 = 1;
            KnmnAdvance(sequence, 113);
            break;
        case 113:
            KnmnValue(data, 0x250, std::int32_t{1});
            KnmnAdvance(sequence, 115);
            break;
        case 114:
            if (!practice) {
                work.dialogueSequence = KnmnChild(sequence, KnmnFeedbackSequence, SHEN_NAME(FS_1), 1);
                KnmnValue(data, 0x88, static_cast<std::int32_t>(retry));
                sequence.reserved1a0 = 1;
            }
            KnmnAdvance(sequence, 115);
            break;
        case 115:
            work.adviceElapsed = 0;
            work.advicePending = 0;
            work.practiceWarning = 0;
            work.consecutiveRepetitions = 0;
            if (KnmnValue<std::int32_t>(data, 0x88) != retry)
                KnmnAdvance(sequence, 132);
            else if (!KnmnValue<std::int32_t>(data, 0x250))
                KnmnWaitReposition(sequence, 131);
            else
                KnmnAdvance(sequence, 131);
            break;
        case 116:
            KnmnStopPracticeTasks(work);
            mtwkSetExtraFlag(scenFindMotionwork(KNMN_RYO), 6, 0);
            KnmnWait(sequence, 2, 117, 1);
            break;
        case 117:
            if (!work.firstFailures[practice]) {
                work.repeatedFailures[practice] = 0;
                work.firstFailures[practice] = 1;
            }
            KnmnAdvance(sequence, 120);
            break;
        case 120:
            work.result = work.results[1].error;
            if (!practice)
                KnmnAdvance(sequence, 121);
            break;
        case 121: {
            KnmnValue(data, 0x88, static_cast<std::int32_t>(success));
            sequence.reserved1a0 = 1;
            KnmnShowPracticeLcd(work);
            CompiledSequenceWork *child = KnmnChild(sequence, KnmnSuccessSequence, SHEN_NAME(SS_1), 1);
            KnmnValue(child, 0x138, work.result);
            KnmnValue(child, 0x140, work.resultIndex);
            KnmnValue(child, 0x148, static_cast<std::int32_t>(success));
            KnmnValue(child, 0x150, static_cast<std::int32_t>(finish));
            sequence.resultSlot = 1;
            KnmnWait(sequence, 8, 126);
            break;
        }
        case 126:
            work.learnedMove = 6;
            KnmnValue(data, 0x88, KnmnValue<std::int32_t>(data, 0xbc));
            sequence.reserved1a0 = 1;
            KnmnAdvance(sequence, 127);
            break;
        case 127:
            KnmnAdvance(sequence, 128);
            break;
        case 128:
            if (KnmnValue<std::int32_t>(data, 0x24c)) {
                KnmnValue(data, 0x250, std::int32_t{1});
                KnmnStartPracticeTimers(sequence, work, practice);
                KnmnWait(sequence, 2, 129, 2);
            } else {
                work.elapsed = 0;
                KnmnAdvance(sequence, 132);
            }
            break;
        case 129:
            KnmnAdvance(sequence, 130);
            break;
        case 130:
            if (work.attempts < 100)
                ++work.attempts;
            KnmnAdvance(sequence, 102);
            break;
        case 131:
            KnmnAdvance(sequence, 101);
            break;
        case 132:
            if (work.timerSequence) {
                KnmnKillSequence(work.timerSequence);
                work.timerSequence = nullptr;
            }
            if (work.adviceSequence) {
                KnmnKillSequence(work.adviceSequence);
                work.adviceSequence = nullptr;
            }
            if (work.dialogueSequence) {
                RetireTrainingSequence(work.dialogueSequence);
                work.dialogueSequence = nullptr;
            }
            if (work.cameraSequence) {
                KnmnKillSequence(work.cameraSequence);
                work.cameraSequence = nullptr;
            }
            KnmnComplete(sequence, true);
            break;
        case 133:
            if (!work.repositionFrames[0])
                KnmnAdvance(sequence, sequence.reserved04);
            break;
        case 134:
            if (!work.repositionFrames[1])
                KnmnAdvance(sequence, sequence.reserved04);
            break;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
        Vector3 position;
        EV_GetCharaPosition(KNMN_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
