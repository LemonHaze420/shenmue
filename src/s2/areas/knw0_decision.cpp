#include "s2/areas/area_knw0.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/training_seqp.hpp"
namespace shenmue::s2::areas {
bool Knw0NeedsFacingReset(std::int32_t practice);
}
namespace shenmue::s2::areas {
namespace {
void Knw0StopPracticeTasks(Knw0Work &work) {
    if (work.dialogueSequence) {
        RetireTrainingSequence(work.dialogueSequence);
        work.dialogueSequence = nullptr;
    }
    if (work.cameraSequence) {
        Knw0KillSequence(work.cameraSequence);
        work.cameraSequence = nullptr;
    }
    if (work.timerSequence) {
        Knw0KillSequence(work.timerSequence);
        work.timerSequence = nullptr;
    }
    if (work.adviceSequence) {
        Knw0KillSequence(work.adviceSequence);
        work.adviceSequence = nullptr;
    }
}
void Knw0StartPracticeTimers(CompiledSequenceWork &sequence, Knw0Work &work, std::int64_t practice) {
    work.timerSequence = Knw0Child(sequence, TrainingTimerSequence, SHEN_NAME(TIMM), 4);
    work.adviceSequence = Knw0Child(sequence, TrainingAdviceTimerSequence, SHEN_NAME(WADV), 5);
    Knw0Value(work.adviceSequence, 0x138, static_cast<std::int32_t>(practice));
    Knw0Value(work.adviceSequence, 0x140, std::int32_t{1});
}
void Knw0SelectPracticeCamera(CompiledSequenceWork &sequence, Knw0Work &work, std::int64_t practice) {
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
void Knw0WaitReposition(CompiledSequenceWork &sequence, std::uint16_t next) {
    sequence.reserved04 = next;
    Knw0Advance(sequence, 133);
}
} // namespace
void Knw0Decision(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Knw0Work &work = *static_cast<Knw0Work *>(sequence.event);
    const std::int64_t practice = Knw0Value<std::int64_t>(data, 0x138), instruction = Knw0Value<std::int64_t>(data, 0x140);
    const std::int64_t second = Knw0Value<std::int64_t>(data, 0x148), success = Knw0Value<std::int64_t>(data, 0x150);
    const std::int64_t retry = Knw0Value<std::int64_t>(data, 0x160), restart = Knw0Value<std::int64_t>(data, 0x168),
                       finish = Knw0Value<std::int64_t>(data, 0x170);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            if (!work.turning)
                Knw0Advance(sequence, 100);
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
            Knw0ResetActors(static_cast<std::int32_t>(practice));
            EV_AT_SetPos(KNW0_KNK, EventTransformFlags::World | EventTransformFlags::AllAxes, KNW0_TEACHER_POSITION);
            EV_AT_SetAngY(KNW0_KNK, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 14563, .z = 0});
            SetMotionRegionFlag(scenFindMotionwork(KNW0_KNK), 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(KNW0_KNK), 0x1902, MotionRegion::LowerBody);
            Knw0Value(data, 0x250, std::int32_t{1});
            Knw0Advance(sequence, 101);
            break;
        case 101:
            Knw0SelectPracticeCamera(sequence, work, practice);
            work.adviceElapsed = 0;
            Knw0StartPracticeTimers(sequence, work, practice);
            work.attempts = 0;
            Knw0Value(data, 0x248, std::int32_t{0});
            Knw0Advance(sequence, 102);
            break;
        case 102: {
            Knw0Value(data, 0x24c, std::int32_t{0});
            if (Knw0Value<std::int32_t>(data, 0x250)) {
                Knw0SelectPracticeCamera(sequence, work, practice);
                Knw0ResetActors(static_cast<std::int32_t>(practice));
            }
            CompiledSequenceWork *child = Knw0Child(sequence, Knw0CapturePractice, SHEN_NAME(WDTP), 0);
            Knw0Value(child, 0x138, static_cast<std::int32_t>(instruction));
            Knw0Value(child, 0x140, static_cast<std::int32_t>(second));
            sequence.resultSlot = 0;
            Knw0Wait(sequence, 8, 103);
            sequence.children[0] = nullptr;
            break;
        }
        case 103:
            Knw0Value(data, 0x250, std::int32_t(Knw0NeedsFacingReset(static_cast<std::int32_t>(practice))));
            if (work.results[0].phase == 2 && practice == 9 && work.advicePending)
                Knw0Value(data, 0x250, std::int32_t{0});
            work.restartMotion = work.results[0].error != 20 || work.timedAdvice || work.practiceWarning || work.practiceExpired || work.advicePending;
            work.changeCamera = 1;
            Knw0Value(&work, 0x84, Knw0Value<std::uint64_t>(&work, 0x78));
            Knw0Value(&work, 0x8c, Knw0Value<std::uint8_t>(&work, 0x80));
            if (work.results[0].error != 20)
                ++work.attemptedMoves;
            Knw0Advance(sequence, work.results[0].phase == 2 ? 104 : 116);
            break;
        case 104:
            if (!Knw0Value<std::int32_t>(data, 0x250))
                QueueTrainingReposition(&sequence, 2, KNW0_RYO, 0, &KNW0_PLAYER_POSITION);
            ++work.repetitions;
            ++work.consecutiveRepetitions;
            Knw0Wait(sequence, 2, 105, 1);
            break;
        case 105:
            if (work.advicePending) {
                Knw0StopPracticeTasks(work);
                work.result = work.results[1].error;
                work.resultIndex = 0;
                Knw0Advance(sequence, (work.timedAdvice | work.practiceWarning | work.practiceExpired) ? 106 : 114);
            } else if (!Knw0Value<std::int32_t>(data, 0x250))
                Knw0WaitReposition(sequence, 130);
            else
                Knw0Advance(sequence, 130);
            break;
        case 106:
            mtwkSetExtraFlag(scenFindMotionwork(KNW0_RYO), 6, 0);
            if (!Knw0Value<std::int32_t>(data, 0x250))
                Knw0WaitReposition(sequence, 107);
            else
                Knw0Advance(sequence, 107);
            break;
        case 107:
            if (work.practiceExpired && work.attemptedMoves >= 9) {
                if (practice)
                    break;
                Knw0ShowLcd(work);
            } else if (practice)
                break;
            Knw0Child(sequence, Knw0Feedback, SHEN_NAME(FS_1), 1);
            sequence.resultSlot = 1;
            Knw0Wait(sequence, 8, 108);
            break;
        case 108: {
            CompiledSequenceWork *child = Knw0Child(sequence, Knw0ExitPractice, SHEN_NAME(EALP), 1);
            Knw0Value(child, 0x138, std::int32_t{1});
            Knw0Value(child, 0x140, static_cast<std::int32_t>(retry));
            Knw0Value(child, 0x148, static_cast<std::int32_t>(restart));
            Knw0Value(child, 0x150, static_cast<std::int32_t>(finish));
            Knw0Value(child, 0x158, static_cast<std::int32_t>(practice));
            sequence.resultSlot = 1;
            Knw0Wait(sequence, 8, 112);
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
            Knw0Value(data, 0x88, Knw0Value<std::int32_t>(data, 0xbc));
            sequence.reserved1a0 = 1;
            Knw0Advance(sequence, 113);
            break;
        case 113:
            Knw0Value(data, 0x250, std::int32_t{1});
            Knw0Advance(sequence, 115);
            break;
        case 114:
            if (!practice) {
                work.dialogueSequence = Knw0Child(sequence, Knw0Feedback, SHEN_NAME(FS_1), 1);
                Knw0Value(data, 0x88, static_cast<std::int32_t>(retry));
                sequence.reserved1a0 = 1;
            }
            Knw0Advance(sequence, 115);
            break;
        case 115:
            work.adviceElapsed = 0;
            work.advicePending = 0;
            work.practiceWarning = 0;
            work.consecutiveRepetitions = 0;
            if (Knw0Value<std::int32_t>(data, 0x88) != retry)
                Knw0Advance(sequence, 132);
            else if (!Knw0Value<std::int32_t>(data, 0x250))
                Knw0WaitReposition(sequence, 131);
            else
                Knw0Advance(sequence, 131);
            break;
        case 116:
            Knw0StopPracticeTasks(work);
            mtwkSetExtraFlag(scenFindMotionwork(KNW0_RYO), 6, 0);
            Knw0Wait(sequence, 2, 117, 1);
            break;
        case 117:
            if (!work.firstFailures[practice]) {
                work.repeatedFailures[practice] = 0;
                work.firstFailures[practice] = 1;
            }
            Knw0Advance(sequence, 120);
            break;
        case 120:
            work.result = work.results[1].error;
            if (!practice)
                Knw0Advance(sequence, 121);
            break;
        case 121: {
            Knw0Value(data, 0x88, static_cast<std::int32_t>(success));
            sequence.reserved1a0 = 1;
            Knw0ShowLcd(work);
            CompiledSequenceWork *child = Knw0Child(sequence, Knw0Success, SHEN_NAME(SS_1), 1);
            Knw0Value(child, 0x138, work.result);
            Knw0Value(child, 0x140, work.resultIndex);
            Knw0Value(child, 0x148, static_cast<std::int32_t>(success));
            Knw0Value(child, 0x150, static_cast<std::int32_t>(finish));
            sequence.resultSlot = 1;
            Knw0Wait(sequence, 8, 126);
            break;
        }
        case 126:
            work.learnedMove = 2;
            Knw0Value(data, 0x88, Knw0Value<std::int32_t>(data, 0xbc));
            sequence.reserved1a0 = 1;
            Knw0Advance(sequence, 127);
            break;
        case 127:
            Knw0Advance(sequence, 128);
            break;
        case 128:
            if (Knw0Value<std::int32_t>(data, 0x24c)) {
                Knw0Value(data, 0x250, std::int32_t{1});
                Knw0StartPracticeTimers(sequence, work, practice);
                Knw0Wait(sequence, 2, 129, 2);
            } else {
                work.elapsed = 0;
                Knw0Advance(sequence, 132);
            }
            break;
        case 129:
            Knw0Advance(sequence, 130);
            break;
        case 130:
            if (work.attempts < 100)
                ++work.attempts;
            Knw0Advance(sequence, 102);
            break;
        case 131:
            Knw0Advance(sequence, 101);
            break;
        case 132:
            if (work.timerSequence) {
                Knw0KillSequence(work.timerSequence);
                work.timerSequence = nullptr;
            }
            if (work.adviceSequence) {
                Knw0KillSequence(work.adviceSequence);
                work.adviceSequence = nullptr;
            }
            if (work.dialogueSequence) {
                RetireTrainingSequence(work.dialogueSequence);
                work.dialogueSequence = nullptr;
            }
            if (work.cameraSequence) {
                Knw0KillSequence(work.cameraSequence);
                work.cameraSequence = nullptr;
            }
            Knw0Complete(sequence, true);
            break;
        case 133:
            if (!work.repositionFrames[0])
                Knw0Advance(sequence, sequence.reserved04);
            break;
        case 134:
            if (!work.repositionFrames[1])
                Knw0Advance(sequence, sequence.reserved04);
            break;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
        Vector3 position;
        EV_GetCharaPosition(KNW0_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
