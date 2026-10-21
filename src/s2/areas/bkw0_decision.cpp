#include "s2/areas/bkw0_sequence.hpp"
#include "s2/areas/bkw0_tables.hpp"
#include "s2/advice.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/training_seqp.hpp"
namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name BKW0_TIMER{SHEN_NAME(TIMM)}, BKW0_ADVICE{SHEN_NAME(WADV)}, BKW0_FEEDBACK{SHEN_NAME(FS_1)};
inline constexpr shen_name BKW0_EXIT{SHEN_NAME(EALP)}, BKW0_FIRST_THROW{SHEN_NAME(SS_1)}, BKW0_FINAL_THROW{SHEN_NAME(SS_2)};
void Bkw0StopPracticeTasks(Bkw0Work &work) {
    if (work.dialogueSequence) {
        RetireTrainingSequence(work.dialogueSequence);
        work.dialogueSequence = nullptr;
    }
    if (work.cameraSequence) {
        Bkw0KillSequence(work.cameraSequence);
        work.cameraSequence = nullptr;
    }
    if (work.timerSequence) {
        Bkw0KillSequence(work.timerSequence);
        work.timerSequence = nullptr;
    }
    if (work.adviceSequence) {
        Bkw0KillSequence(work.adviceSequence);
        work.adviceSequence = nullptr;
    }
}
void Bkw0StartPracticeTimers(CompiledSequenceWork &sequence, Bkw0Work &work, std::int32_t practice) {
    work.timerSequence = Bkw0Child(sequence, TrainingTimerSequence, BKW0_TIMER, 4);
    work.adviceSequence = Bkw0Child(sequence, TrainingAdviceTimerSequence, BKW0_ADVICE, 5);
    Bkw0SequenceValue(*work.adviceSequence, 0x138, practice);
    Bkw0SequenceValue(*work.adviceSequence, 0x140, std::int32_t{1});
}
void Bkw0SelectPracticeCamera(CompiledSequenceWork &sequence, Bkw0Work &work, std::int32_t practice) {
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
} // namespace
void Bkw0PracticeDecisionSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    const std::int32_t practice = Bkw0SequenceValue<std::int32_t>(sequence, 0x138), instruction = Bkw0SequenceValue<std::int32_t>(sequence, 0x140);
    const std::int32_t secondInstruction = Bkw0SequenceValue<std::int32_t>(sequence, 0x148), success = Bkw0SequenceValue<std::int32_t>(sequence, 0x150);
    const std::int32_t retry = Bkw0SequenceValue<std::int32_t>(sequence, 0x160), restart = Bkw0SequenceValue<std::int32_t>(sequence, 0x168);
    const std::int32_t finish = Bkw0SequenceValue<std::int32_t>(sequence, 0x170);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            if (!work.practiceBlocked)
                Bkw0Advance(sequence, 100);
            break;
        case 100:
            work.practice = practice;
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
            Bkw0ResetActors(practice);
            EV_AT_SetPos(BKW0_LKN, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_LKN_POSITION);
            Bkw0ResetPracticeOpponentAngle();
            SetMotionRegionFlag(scenFindMotionwork(BKW0_LKN), 71, 0, MotionRegion::LowerBody);
            Bkw0SequenceValue(sequence, 0x250, std::int32_t{1});
            Bkw0Advance(sequence, 101);
            break;
        case 101:
            Bkw0SelectPracticeCamera(sequence, work, practice);
            work.adviceElapsed = 0;
            Bkw0StartPracticeTimers(sequence, work, practice);

            Bkw0SequenceValue(sequence, 0x248, std::int32_t{0});
            Bkw0Advance(sequence, 102);
            break;
        case 102: {
            Bkw0SequenceValue(sequence, 0x24c, std::int32_t{0});
            if (Bkw0SequenceValue<std::int32_t>(sequence, 0x250)) {
                Bkw0SelectPracticeCamera(sequence, work, practice);
                Bkw0ResetActors(practice);
            }
            CompiledSequenceWork *child = Bkw0Child(sequence, Bkw0PracticeSequence, BKW0_PRACTICE_FIGHT, 0);
            Bkw0SequenceValue(*child, 0x138, instruction);
            Bkw0SequenceValue(*child, 0x140, secondInstruction);
            sequence.resultSlot = 0;
            Bkw0Wait(sequence, 8, 103);
            break;
        }
        case 103: {
            bool reset = TrainingNeedsFacingReset(practice);
            if ((std::to_integer<std::uint8_t>(work.fightResults[7]) & 1) != 0)
                reset = true;
            if (work.fightResults[1] == std::byte{2} && practice == 9 && work.advicePending)
                reset = false;
            Bkw0SequenceValue(sequence, 0x250, std::int32_t(reset));
            work.restartMotion =
                work.fightResults[2] != std::byte{20} || work.timedAdvice || work.practiceWarning || work.practiceExpired || work.advicePending;
            work.changeCamera = 1;
            for (std::int32_t i = 0; i < 9; ++i)
                work.fightResults[12 + i] = work.fightResults[i];
            if (work.fightResults[2] != std::byte{20})
                ++work.attemptedMoves;
            Bkw0Advance(sequence, work.fightResults[1] == std::byte{2} ? 104 : 116);
            break;
        }
        case 104:
            if (!Bkw0SequenceValue<std::int32_t>(sequence, 0x250)) {
                QueueTrainingReposition(&sequence, 2, BKW0_RYO, 0, &BKW0_RYO_POSITION);
                QueueTrainingReposition(&sequence, 6, BKW0_LKN, 1, &BKW0_LKN_POSITION);
            }
            if ((std::to_integer<std::uint8_t>(work.fightResults[7]) & 1) != 0)
                ++work.repeatedThrows;
            if (!work.previousFailure)
                ++work.consecutiveThrows;
            ++work.repetitions;
            ++work.consecutiveRepetitions;
            Bkw0Wait(sequence, 2, 105, 1);
            break;
        case 105:
            if (work.advicePending || work.finalPractice || work.repeatedThrows >= 10) {
                scenCvsCtrlFree(nullptr);
                Bkw0StopPracticeTasks(work);
                work.result = std::to_integer<std::uint8_t>(work.fightResults[14]);
                work.resultIndex = 0;
                if ((work.repeatedThrows == 5 || work.elapsed > 900) && !bkw0AdviceIssued) {
                    QueueFightAdviceControl(15);
                    bkw0AdviceIssued = 1;
                }
                if (work.finalPractice) {
                    if ((std::to_integer<std::uint8_t>(work.fightResults[7]) & 1) != 0) {
                        EV_debug_log("BKW0\\eBKW0_et_bukan2.c", 1473, "WC:%d\n", work.elapsed);
                        if (work.elapsed > 1800) {
                            work.finalPractice = 2;
                            Bkw0Advance(sequence, 106);
                        } else
                            Bkw0Advance(sequence, 131);
                    } else {
                        work.finalPractice = 2;
                        Bkw0Advance(sequence, 106);
                    }
                } else
                    Bkw0Advance(sequence, (work.timedAdvice | work.practiceWarning | work.practiceExpired | (work.repeatedThrows >= 10)) ? 106 : 114);
            } else if (!Bkw0SequenceValue<std::int32_t>(sequence, 0x250)) {
                sequence.reserved04 = 130;
                Bkw0Advance(sequence, 133);
            } else
                Bkw0Advance(sequence, 130);
            break;
        case 106:
            mtwkSetExtraFlag(scenFindMotionwork(BKW0_RYO), 6, 0);
            if (!Bkw0SequenceValue<std::int32_t>(sequence, 0x250)) {
                sequence.reserved04 = 107;
                Bkw0Advance(sequence, 133);
            } else
                Bkw0Advance(sequence, 107);
            break;
        case 107:
            if (work.practiceExpired && work.attemptedMoves >= 9 && !practice)
                Bkw0ShowPracticeLcd(work);
            else if ((practice & ~9) != 0 || practice == 8)
                break;
            Bkw0Child(sequence, Bkw0PracticeFeedbackSequence, BKW0_FEEDBACK, 1);
            sequence.resultSlot = 1;
            Bkw0Wait(sequence, 8, 108);
            break;
        case 108:
            if (work.finalPractice == 2 || work.finalPractice == 1) {
                Bkw0SequenceValue(sequence, 0x88, work.finalPractice == 2 ? finish : success);
                sequence.reserved1a0 = 1;
                Bkw0Advance(sequence, 113);
                break;
            }
            [[fallthrough]];
        case 109: {
            CompiledSequenceWork *child = Bkw0Child(sequence, Bkw0PracticeExitSequence, BKW0_EXIT, 1);
            Bkw0SequenceValue(*child, 0x138, std::int32_t{1});
            Bkw0SequenceValue(*child, 0x140, retry);
            Bkw0SequenceValue(*child, 0x148, restart);
            Bkw0SequenceValue(*child, 0x150, finish);
            Bkw0SequenceValue(*child, 0x158, practice);
            sequence.resultSlot = 1;
            Bkw0Wait(sequence, 8, 112);
            break;
        }
        case 111:
        case 123:
        case 124:
        case 125:
            break;
        case 112:
            Bkw0SequenceValue(sequence, 0x88, Bkw0SequenceValue<std::int32_t>(sequence, 0xbc));
            sequence.reserved1a0 = 1;
            Bkw0Advance(sequence, 113);
            break;
        case 113:
            Bkw0SequenceValue(sequence, 0x250, std::int32_t{1});
            Bkw0Advance(sequence, 115);
            break;
        case 114:
            if (!(practice & ~9) && practice != 8) {
                work.dialogueSequence = Bkw0Child(sequence, Bkw0PracticeFeedbackSequence, BKW0_FEEDBACK, 1);
                Bkw0SequenceValue(sequence, 0x88, retry);
                sequence.reserved1a0 = 1;
            }
            Bkw0Advance(sequence, 115);
            break;
        case 115:
            work.adviceElapsed = 0;
            work.advicePending = 0;
            work.timedAdvice = 0;
            work.practiceWarning = 0;
            work.practiceExpired = 0;
            work.consecutiveRepetitions = 0;
            work.previousRepetitions = 0;
            if (Bkw0SequenceValue<std::int32_t>(sequence, 0x88) != retry)
                Bkw0Advance(sequence, 132);
            else if (!Bkw0SequenceValue<std::int32_t>(sequence, 0x250)) {
                sequence.reserved04 = 131;
                Bkw0Advance(sequence, 133);
            } else
                Bkw0Advance(sequence, 131);
            break;
        case 116:
            scenCvsCtrlFree(nullptr);
            Bkw0StopPracticeTasks(work);
            mtwkSetExtraFlag(scenFindMotionwork(BKW0_RYO), 6, 0);
            Bkw0Wait(sequence, 2, 117, 1);
            break;
        case 117:
            if (!work.firstFailures[practice]) {
                work.repeatedFailures[practice] = 0;
                work.firstFailures[practice] = 1;
            }
            Bkw0Advance(sequence, 120);
            break;
        case 120:
            work.result = std::to_integer<std::uint8_t>(work.fightResults[14]);
            work.resultIndex = 0;
            if (!practice)
                Bkw0Advance(sequence, 121);
            else if (practice == 1)
                Bkw0Advance(sequence, 122);
            break;
        case 121:
        case 122: {
            const bool finalThrow = sequence.mode == 122;
            Bkw0SequenceValue(sequence, 0x88, success);
            sequence.reserved1a0 = 1;
            Bkw0ShowPracticeLcd(work);
            CompiledSequenceWork *child =
                Bkw0Child(sequence, finalThrow ? Bkw0FinalThrowSequence : Bkw0ThrowOutcomeSequence, finalThrow ? BKW0_FINAL_THROW : BKW0_FIRST_THROW, 1);
            Bkw0SequenceValue(*child, 0x138, work.result);
            Bkw0SequenceValue(*child, 0x140, work.resultIndex);
            Bkw0SequenceValue(*child, 0x148, success);
            Bkw0SequenceValue(*child, 0x150, finish);
            if (!finalThrow)
                Bkw0SequenceValue(*child, 0x158, retry);
            sequence.resultSlot = 1;
            Bkw0Wait(sequence, 8, 126);
            break;
        }
        case 126:
            Bkw0SequenceValue(sequence, 0x88, Bkw0SequenceValue<std::int32_t>(sequence, 0xbc));
            sequence.reserved1a0 = 1;
            Bkw0Advance(sequence, 127);
            break;
        case 127:
            Bkw0Advance(sequence, 128);
            break;
        case 128:
            if (Bkw0SequenceValue<std::int32_t>(sequence, 0x24c)) {
                Bkw0SequenceValue(sequence, 0x250, std::int32_t{1});
                Bkw0StartPracticeTimers(sequence, work, practice);
                Bkw0Wait(sequence, 2, 129, 2);
            } else {
                work.elapsed = 0;
                Bkw0Advance(sequence, 132);
            }
            break;
        case 129:
            Bkw0Advance(sequence, 130);
            break;
        case 130:
            figpAssignModeFor(BKW0_RYO, 0);
            figpAssignModeFor(BKW0_LKN, 0);
            work.previousFailure = work.fightResults[1] != std::byte{2};
            if (work.attempts < 100)
                ++work.attempts;
            Bkw0Advance(sequence, 102);
            break;
        case 131:
            Bkw0Advance(sequence, 101);
            break;
        case 132: {
            figpAssignModeFor(BKW0_RYO, 0);
            figpAssignModeFor(BKW0_LKN, 0);
            if (work.timerSequence) {
                Bkw0KillSequence(work.timerSequence);
                work.timerSequence = nullptr;
            }
            if (work.adviceSequence) {
                Bkw0KillSequence(work.adviceSequence);
                work.adviceSequence = nullptr;
            }
            if (work.dialogueSequence) {
                RetireTrainingSequence(work.dialogueSequence);
                work.dialogueSequence = nullptr;
            }
            if (work.cameraSequence) {
                Bkw0KillSequence(work.cameraSequence);
                work.cameraSequence = nullptr;
            }
            if (sequence.parent) {
                Bkw0SequenceValue(*sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
                Bkw0SequenceValue(*sequence.parent, 0xb8 + 4 * sequence.childSlot, Bkw0SequenceValue<std::int32_t>(sequence, 0x88));
            }
            for (CompiledSequenceWork *child : sequence.children)
                if (child) {
                    ClearTrainingSequenceChildren(child);
                    Bkw0KillSequence(child);
                }
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            Vector3 position;
            EV_GetCharaPosition(BKW0_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            return;
        }
        case 133:
            if (!work.repositionFrames[0])
                Bkw0Advance(sequence, sequence.reserved04);
            break;
        case 134:
            if (!work.repositionFrames[1])
                Bkw0Advance(sequence, sequence.reserved04);
            break;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
        Vector3 position;
        EV_GetCharaPosition(BKW0_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
