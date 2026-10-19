#include "s2/areas/area_mfw0.hpp"
#include "s2/areas/mfw0_tables.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/training_seqp.hpp"
#include "s2/save.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
void Mfw0StopOwner(CompiledSequenceWork *&owner, bool recursive = false) {
    if (!owner)
        return;
    if (recursive)
        RetireTrainingSequence(owner);
    else
        Mfw0KillSequence(owner);
    owner = nullptr;
}
void Mfw0StopPracticeOwners(Mfw0Work &work) {
    Mfw0StopOwner(work.dialogueSequence, true);
    Mfw0StopOwner(work.cameraSequence);
    Mfw0StopOwner(work.timerSequence);
    Mfw0StopOwner(work.adviceSequence);
}
std::int32_t Mfw0SelectPracticeCamera(Mfw0Work &work, std::int32_t practice) {
    std::int32_t camera = 0;
    if (work.changeCamera) {
        if (static_cast<std::uint32_t>(practice) <= 3 || practice == 9)
            camera = 9010 + (work.practiceCamera == 9010);
        work.changeCamera = 0;
    } else
        camera = work.practiceCamera;
    work.practiceCamera = camera;
    return camera;
}
void Mfw0StartPracticeTimers(CompiledSequenceWork &sequence, std::int32_t practice) {
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    work.timerSequence = Mfw0Child(sequence, TrainingTimerSequence, SHEN_NAME(TIMM), 4);
    work.adviceSequence = Mfw0Child(sequence, TrainingAdviceTimerSequence, SHEN_NAME(WADV), 5);
    Mfw0Value(work.adviceSequence, 0x138, practice);
    Mfw0Value(work.adviceSequence, 0x140, std::int32_t{0});
}
void Mfw0ShowMoveLcd(Mfw0Work &work, bool punch) {
    work.lcdBitmap = punch ? MFW0_PUNCH_LCD : MFW0_STEP_LCD;
    work.lcdFrames = 600;
    LcdmSelectMessage(1);
    LcdmInit(reinterpret_cast<std::uintptr_t>(work.lcdBitmap));
}
void Mfw0SetDecisionResult(CompiledSequenceWork &sequence, std::int32_t result) {
    sequence.reserved88 = static_cast<std::uint32_t>(result);
    sequence.reserved1a0 = 1;
}
void Mfw0WaitDecisionChild(CompiledSequenceWork &sequence, std::uint16_t next) {
    sequence.resultSlot = 1;
    Mfw0Wait(sequence, 8, next);
}
void Mfw0WaitReposition(CompiledSequenceWork &sequence, std::uint16_t next) {
    if (!Mfw0Value<std::int32_t>(&sequence, 0x250)) {
        sequence.reserved04 = next;
        Mfw0Advance(sequence, 133);
    } else
        Mfw0Advance(sequence, next);
}
} // namespace
void Mfw0PracticeDecisionSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const std::int32_t practice = static_cast<std::int32_t>(Mfw0Value<std::int64_t>(data, 0x138));
    const std::int64_t instruction = Mfw0Value<std::int64_t>(data, 0x140), second = Mfw0Value<std::int64_t>(data, 0x148);
    const std::int32_t firstSuccess = static_cast<std::int32_t>(Mfw0Value<std::int64_t>(data, 0x150));
    const std::int32_t secondSuccess = static_cast<std::int32_t>(Mfw0Value<std::int64_t>(data, 0x158));
    const std::int32_t continuation = static_cast<std::int32_t>(Mfw0Value<std::int64_t>(data, 0x160));
    const std::int32_t retry = static_cast<std::int32_t>(Mfw0Value<std::int64_t>(data, 0x168));
    const std::int32_t exit = static_cast<std::int32_t>(Mfw0Value<std::int64_t>(data, 0x170));
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            if (!work.turning)
                Mfw0Advance(sequence, 100);
            break;
        case 100: {
            work.practice = practice;
            Mfw0Value(&work, 0x538, std::uint64_t{0});
            Mfw0Value(&work, 0x540, std::uint64_t{0});
            Mfw0Value(&work, 0x548, std::uint64_t{0});
            Mfw0Value(&work, 0x550, std::uint64_t{0});
            work.changeCamera = 1;
            work.restartMotion = 1;
            work.cameraSequence = nullptr;
            work.dialogueSequence = nullptr;
            work.timerSequence = nullptr;
            work.adviceSequence = nullptr;
            Mfw0ResetActors(practice);
            EV_AT_SetPos(MFW0_TKR, EventTransformFlags::World | EventTransformFlags::AllAxes, MFW0_TEACHER_POSITION);
            SetTrainingCharacterYaw(MFW0_TKR, 41264);
            Vector3i rotation;
            GetScenarioCharacterEulerAngles(MFW0_TKR, &rotation, EventTransformFlags::MotionYaw);
            SetTrainingCharacterYaw(MFW0_TKR, static_cast<std::uint16_t>(rotation.y + 10923));
            SetMotionRegionFlag(scenFindMotionwork(MFW0_TKR), 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(MFW0_TKR), 5003, MotionRegion::LowerBody);
            Mfw0Value(data, 0x250, std::int32_t{1});
            Mfw0Advance(sequence, 101);
            break;
        }
        case 101:
            SetTrainingCameraSequence(&sequence, Mfw0SelectPracticeCamera(work, practice));
            work.adviceElapsed = 0;
            Mfw0StartPracticeTimers(sequence, practice);
            work.attempts = 0;
            Mfw0Value(data, 0x248, std::int32_t{0});
            Mfw0Advance(sequence, 102);
            break;
        case 102: {
            Mfw0Value(data, 0x24c, std::int32_t{0});
            if (Mfw0Value<std::int32_t>(data, 0x250)) {
                SetTrainingCameraSequence(&sequence, Mfw0SelectPracticeCamera(work, practice));
                Mfw0ResetActors(practice);
            }
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0PracticeSequence, SHEN_NAME(WDTP), 0);
            Mfw0Value(child, 0x138, static_cast<std::int32_t>(instruction));
            Mfw0Value(child, 0x140, static_cast<std::int32_t>(second));
            sequence.resultSlot = 0;
            Mfw0Wait(sequence, 8, 103);
            break;
        }
        case 103:
            Mfw0Value(data, 0x250, std::int32_t{Mfw0NeedsFacingReset(practice)});
            if (work.results[0].phase == 2 && practice == 9 && work.advicePending)
                Mfw0Value(data, 0x250, std::int32_t{0});
            work.changeCamera = 1;
            for (std::size_t i = 0; i < 8; ++i)
                Mfw0Value(&work, 0x84 + i, Mfw0Value<std::uint8_t>(&work, 0x78 + i));
            work.results[1].detail = work.results[0].detail;
            if (work.results[0].error != 20)
                ++work.attemptedMoves;
            Mfw0Advance(sequence, work.results[0].phase == 2 ? 104 : 116);
            break;
        case 104:
            if (!Mfw0Value<std::int32_t>(data, 0x250))
                QueueTrainingReposition(&sequence, 2, MFW0_RYO, 0, &MFW0_RYO_POSITION);
            ++work.repetitions;
            ++work.consecutiveRepetitions;
            Mfw0Wait(sequence, 2, 105, 1);
            break;
        case 105:
            if (work.advicePending) {
                scenCvsCtrlFree(nullptr);
                Mfw0StopPracticeOwners(work);
                work.result = work.results[1].error;
                work.resultIndex = 0;
                Mfw0FeedbackSelection selection;
                if (!practice)
                    selection = Mfw0SelectStepFeedback(sequence);
                else if (practice == 1 || practice == 9)
                    selection = Mfw0SelectPunchFeedback(sequence);
                Mfw0Advance(sequence, (selection.corrective | work.timedAdvice | work.practiceWarning | work.practiceExpired) ? 106 : 114);
            } else {
                if (work.results[0].error != 20)
                    work.restartMotion = 1;
                Mfw0WaitReposition(sequence, 130);
            }
            break;
        case 106:
            mtwkSetExtraFlag(scenFindMotionwork(MFW0_RYO), 6, 0);
            Mfw0WaitReposition(sequence, 107);
            break;
        case 107:
            if (work.practiceExpired && work.attemptedMoves >= 9 && (practice == 0 || practice == 1))
                Mfw0ShowMoveLcd(work, practice == 1);
            if (practice == 0 || practice == 1) {
                const std::uint16_t next = practice == 0 ? 108 : 109;
                if (work.practiceExpired | work.timedAdvice | work.practiceWarning)
                    Mfw0Advance(sequence, next);
                else {
                    Mfw0Child(
                        sequence, practice == 0 ? Mfw0StepFeedbackSequence : Mfw0PunchFeedbackSequence, practice == 0 ? SHEN_NAME(FS_1) : SHEN_NAME(FS_2), 1);
                    Mfw0WaitDecisionChild(sequence, next);
                }
            } else if (practice == 9) {
                CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0FinalFeedbackSequence, SHEN_NAME(FS_3), 1);
                Mfw0Value(child, 0x138, firstSuccess);
                Mfw0Value(child, 0x140, exit);
                Mfw0WaitDecisionChild(sequence, 111);
            }
            break;
        case 108:
        case 109: {
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0ExitPracticeSequence, SHEN_NAME(EALP), 1);
            Mfw0Value(child, 0x138, std::int32_t{1});
            Mfw0Value(child, 0x140, continuation);
            Mfw0Value(child, 0x148, retry);
            Mfw0Value(child, 0x150, exit);
            Mfw0Value(child, 0x158, practice);
            Mfw0WaitDecisionChild(sequence, 112);
            break;
        }
        case 111:
            Mfw0Advance(sequence, 112);
            break;
        case 112:
            Mfw0SetDecisionResult(sequence, Mfw0Value<std::int32_t>(data, 0xbc));
            Mfw0Advance(sequence, 113);
            break;
        case 113:
            Mfw0Value(data, 0x250, std::int32_t{1});
            Mfw0Advance(sequence, 115);
            break;
        case 114:
            if (practice == 0 || practice == 1 || practice == 9) {
                work.dialogueSequence = Mfw0Child(sequence, practice == 0 ? Mfw0StepFeedbackSequence : Mfw0PunchFeedbackSequence,
                    practice == 0     ? SHEN_NAME(FS_1) :
                        practice == 1 ? SHEN_NAME(FS_2) :
                                        SHEN_NAME(FS_3),
                    1);
                Mfw0SetDecisionResult(sequence, continuation);
            }
            Mfw0Advance(sequence, 115);
            break;
        case 115:
            Mfw0UpdateRestartMotion(sequence, practice);
            Mfw0Value(&work, 0x5fc, std::uint64_t{0});
            Mfw0Value(&work, 0x538, std::uint64_t{0});
            Mfw0Value(&work, 0x540, std::uint64_t{0});
            Mfw0Value(&work, 0x54c, std::uint64_t{0});
            if (static_cast<std::int32_t>(sequence.reserved88) != continuation)
                Mfw0Advance(sequence, 132);
            else
                Mfw0WaitReposition(sequence, 131);
            break;
        case 116:
            Mfw0StopPracticeOwners(work);
            mtwkSetExtraFlag(scenFindMotionwork(MFW0_RYO), 6, 0);
            Mfw0Wait(sequence, 2, 117, 1);
            break;
        case 117:
            if (!work.firstFailures[practice]) {
                work.repeatedFailures[practice] = 0;
                work.firstFailures[practice] = 1;
            }
            Mfw0Advance(sequence, work.results[0].error == 22 ? 118 : 120);
            break;
        case 118: {
            Mfw0ShowMoveLcd(work, true);
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0PracticeSuccessSequence, SHEN_NAME(SCMP), 1);
            Mfw0Value(child, 0x138, secondSuccess);
            Mfw0Value(child, 0x140, exit);
            Mfw0WaitDecisionChild(sequence, 119);
            break;
        }
        case 119:
            Mfw0SetDecisionResult(sequence, Mfw0Value<std::int32_t>(data, 0xbc));
            work.learnedMove = 1;
            work.restCount = 0;
            Mfw0Advance(sequence, 128);
            break;
        case 120:
            work.result = work.results[1].error;
            work.resultIndex = 0;
            if (practice == 0 || practice == 1 || practice == 9)
                Mfw0Advance(sequence, practice == 0 ? 121 : practice == 1 ? 122 : 124);
            break;
        case 121: {
            Mfw0SetDecisionResult(sequence, firstSuccess);
            Mfw0ShowMoveLcd(work, false);
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0StepSuccessSequence, SHEN_NAME(SS_1), 1);
            Mfw0Value(child, 0x138, work.result);
            Mfw0Value(child, 0x140, work.resultIndex);
            Mfw0WaitDecisionChild(sequence, 127);
            break;
        }
        case 122: {
            Mfw0ShowMoveLcd(work, true);
            work.learnedMove = 1;
            work.restCount = 0;
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0PunchSuccessSequence, SHEN_NAME(SS_2), 1);
            Mfw0Value(child, 0x138, work.result);
            Mfw0Value(child, 0x140, work.resultIndex);
            Mfw0Value(child, 0x148, firstSuccess);
            Mfw0Value(child, 0x150, exit);
            Mfw0WaitDecisionChild(sequence, 126);
            break;
        }
        case 123:
            break;
        case 124: {
            Mfw0ShowMoveLcd(work, true);
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0RepeatedDemonstrationSequence, SHEN_NAME(RP11), 1);
            switch (work.demonstrationCamera) {
            case 0:
                Mfw0Value(child, 0x138, std::int32_t{9007});
                ++work.demonstrationCamera;
                break;
            case 1:
                Mfw0Value(child, 0x138, std::int32_t{9008});
                ++work.demonstrationCamera;
                break;
            case 2:
                Mfw0Value(child, 0x138, std::int32_t{9009});
                work.demonstrationCamera = 0;
                break;
            default:
                break;
            }
            Mfw0WaitDecisionChild(sequence, 125);
            break;
        }
        case 125:
            EV_EnableCharacterDisplay(MFW0_RYO);
            EV_EnableCharacterDisplay(MFW0_TKR);
            Mfw0Value(data, 0x248, Mfw0Value<std::uint32_t>(data, 0x248) + 1);
            if (Mfw0Value<std::uint32_t>(data, 0x248) != 5 && !work.timedAdvice) {
                Mfw0SetDecisionResult(sequence, continuation);
                Mfw0Value(data, 0x24c, std::int32_t{1});
                Mfw0ResetActors(practice);
                std::int32_t camera = Mfw0SelectPracticeCamera(work, practice);
                if (static_cast<std::uint16_t>(camera) < 9000)
                    camera = 9000;
                work.camera = static_cast<std::uint16_t>(camera);
                EVS_LoopCamera(camera, shen_name{}, shen_name{});
                Mfw0Advance(sequence, 127);
            } else {
                CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0RepeatSuccessSequence, SHEN_NAME(SS_F), 1);
                Mfw0Value(child, 0x138, work.result);
                Mfw0Value(child, 0x140, work.resultIndex);
                Mfw0Value(child, 0x148, firstSuccess);
                Mfw0Value(child, 0x150, exit);
                Mfw0WaitDecisionChild(sequence, 126);
            }
            break;
        case 126:
            Mfw0SetDecisionResult(sequence, Mfw0Value<std::int32_t>(data, 0xbc));
            Mfw0Advance(sequence, 127);
            break;
        case 127:
            Mfw0Advance(sequence, 128);
            break;
        case 128:
            if (Mfw0Value<std::int32_t>(data, 0x24c)) {
                Mfw0Value(data, 0x250, std::int32_t{1});
                Mfw0StartPracticeTimers(sequence, practice);
                Mfw0Wait(sequence, 2, 129, 2);
            } else {
                work.elapsed = 0;
                Mfw0Advance(sequence, 132);
            }
            break;
        case 129:
            Mfw0Advance(sequence, 130);
            break;
        case 130:
            if (work.attempts < 100)
                ++work.attempts;
            Mfw0Advance(sequence, 102);
            break;
        case 131:
            Mfw0Advance(sequence, 101);
            break;
        case 132: {
            Mfw0StopOwner(work.timerSequence);
            Mfw0StopOwner(work.adviceSequence);
            Mfw0StopOwner(work.dialogueSequence, true);
            Mfw0StopOwner(work.cameraSequence);
            Mfw0Complete(sequence, true);
            Vector3 position;
            EV_GetCharaPosition(MFW0_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            return;
        }
        case 133:
            if (!work.repositionFrames[0])
                Mfw0Advance(sequence, sequence.reserved04);
            break;
        case 134:
            if (!work.repositionFrames[1])
                Mfw0Advance(sequence, sequence.reserved04);
            break;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
        Vector3 position;
        EV_GetCharaPosition(MFW0_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
