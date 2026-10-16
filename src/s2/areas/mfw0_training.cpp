#include "s2/areas/area_mfw0.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
CompiledSequenceWork *Mfw0Child(CompiledSequenceWork &sequence, HLib::TaskCallback callback, shen_name token, std::int32_t slot) {
    CompiledSequenceWork *child = QueueCompiledSequence(callback, token, sequence.event);
    if (!sequence.children[slot])
        sequence.children[slot] = child;
    child->parent = &sequence;
    child->childSlot = slot;
    child->reference = sequence.reference + 1;
    sequence.timer = slot;
    Mfw0Value(&sequence, 0x90 + 4 * slot, std::int32_t{0});
    return child;
}
void Mfw0Complete(CompiledSequenceWork &sequence, bool result) {
    if (sequence.parent) {
        Mfw0Value(sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
        if (result)
            Mfw0Value(sequence.parent, 0xb8 + 4 * sequence.childSlot, Mfw0Value<std::int32_t>(&sequence, 0x88));
    }
    RetireCompiledSequenceLinks(&sequence);
    HLib::DestroyCurrentTask();
}
void Mfw0KillSequence(CompiledSequenceWork *sequence) {
    if (sequence && sequence->task) {
        HLib::Task *task = sequence->task;
        RetireCompiledSequenceLinks(sequence);
        HLib::DestroyTaskInternal(task);
    }
}
void Mfw0Dialogue(CompiledSequenceWork &sequence, const char *line) {
    EV_PlayDialogue(line);
    Mfw0Wait(sequence, 4, static_cast<std::uint16_t>(sequence.mode + 1), 60);
}
void Mfw0Camera(CompiledSequenceWork &sequence, std::uint16_t camera, shen_name target) {
    static_cast<Mfw0Work *>(sequence.event)->camera = camera;
    EVS_LoopCamera(camera, target, target);
}
void Mfw0TurnToTeacher(CompiledSequenceWork &sequence) {
    constexpr shen_name turn{SHEN_NAME(TURN)};
    CompiledSequenceWork *child = Mfw0Child(sequence, TrainingTurnSequence, turn, 1);
    Mfw0Value(child, 0x138, MFW0_RYO);
    Mfw0Value(child, 0x140, MFW0_TKR);
    Mfw0Value(child, 0x148, std::int32_t{15});
    Mfw0Value(child, 0x150, std::int32_t{10});
}
namespace {
inline constexpr shen_name MFW0_LCD{SHEN_NAME(DLCD)}, MFW0_START{SHEN_NAME(S000)}, MFW0_INTRO{SHEN_NAME(S010)};
inline constexpr shen_name MFW0_DECLINE{SHEN_NAME(S020)}, MFW0_FIRST{SHEN_NAME(S021)}, MFW0_REPEAT{SHEN_NAME(S030)};
inline constexpr shen_name MFW0_DECISION{SHEN_NAME(WDMA)}, MFW0_FINISH{SHEN_NAME(S100)}, MFW0_AWARD{SHEN_NAME(S200)};
inline constexpr shen_name MFW0_INITIALIZE{SHEN_NAME(IWDE)};
void Mfw0QueueTrainingChild(CompiledSequenceWork &sequence, HLib::TaskCallback callback, shen_name name, std::uint16_t next) {
    Mfw0Child(sequence, callback, name, 0);
    sequence.resultSlot = 0;
    Mfw0Wait(sequence, 8, next);
}
void Mfw0Lesson(void *data, shen_name author, std::uint16_t terminal, bool declined) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            ResetTrainingAuthorCharacter(MFW0_RYO);
            ResetTrainingAuthorCharacter(MFW0_TKR);
            Mfw0QueueAuthor(author);
            Mfw0Wait(sequence, 13, 101, 5, 0);
            break;
        case 101:
            Mfw0ResetReflectors();
            Mfw0Wait(sequence, 2, terminal, 2);
            break;
        default:
            if (sequence.mode == terminal) {
                if (declined)
                    static_cast<Mfw0Work *>(sequence.event)->learnedMove = -1;
                Mfw0Complete(sequence);
                return;
            }
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace
void Mfw0TrainingSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            work.lcdSequence = nullptr;
            Mfw0Advance(sequence, 103);
            break;
        case 103:
            if (!work.lcdSequence)
                work.lcdSequence = Mfw0Child(sequence, CompiledLcdCountdownCallback, MFW0_LCD, 1);
            Mfw0QueueTrainingChild(sequence, Mfw0StartTrainingSequence, MFW0_START, work.selector == 1 ? 120 : 106);
            break;
        case 106:
            Mfw0QueueTrainingChild(sequence, Mfw0IntroductionSequence, MFW0_INTRO, 107);
            break;
        case 107:
            Mfw0Advance(sequence, Mfw0Value<std::int32_t>(data, 0xb8) == 62 ? 112 : 109);
            break;
        case 109:
            Mfw0QueueTrainingChild(sequence, Mfw0FirstLessonSequence, MFW0_DECLINE, 124);
            break;
        case 112:
            Mfw0QueueTrainingChild(sequence, Mfw0SecondLessonSequence, MFW0_FIRST, 120);
            break;
        case 115:
            return;
        case 119:
            Mfw0QueueTrainingChild(sequence, Mfw0FinalLessonSequence, MFW0_REPEAT, 120);
            break;
        case 120:
        case 122:
        case 123: {
            if (sequence.mode == 123)
                ++work.combinedPracticeEntries;
            EV_SetRefBoardParamsFor(MFW0_RYO, 1);
            SetScenarioReflectorBoardValues(MFW0_RYO, &MFW0_PRACTICE_REFLECTOR);
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0PracticeDecisionSequence, MFW0_DECISION, 0);
            Mfw0Value(child, 0x138, std::int32_t{sequence.mode == 120 ? 0 : sequence.mode == 122 ? 1 : 9});
            Mfw0Value(child, 0x140, std::int32_t{sequence.mode == 120 ? 9 : 10});
            Mfw0Value(child, 0x148, std::int32_t{sequence.mode == 120 ? 10 : -1});
            Mfw0Value(child, 0x150, std::int32_t{sequence.mode == 120 ? 122 : 123});
            Mfw0Value(child, 0x158, std::int32_t{123});
            Mfw0Value(child, 0x160, std::int32_t{sequence.mode});
            Mfw0Value(child, 0x168, std::int32_t{119});
            Mfw0Value(child, 0x170, std::int32_t{124});
            sequence.resultSlot = 0;
            sequence.mode = 9;
            sequence.continueAfterWait = 1;
            break;
        }
        case 124:
            Mfw0QueueTrainingChild(sequence, Mfw0FinishTrainingSequence, MFW0_FINISH, 126);
            break;
        case 126:
            Mfw0QueueTrainingChild(sequence, Mfw0AwardMoveSequence, MFW0_AWARD, 127);
            break;
        case 127:
            work.active = 0;
            if (work.lcdSequence) {
                Mfw0KillSequence(sequence.children[1]);
                work.lcdSequence = nullptr;
                LcdmInit(0);
                CheckHelpIcon(2048, 2048, CheckHelpIconOp::Remove);
            }
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0StartTrainingSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Value(&work, 0x558, std::uint64_t{0});
            work.firstFailures[2] = 0;
            Mfw0Value(&work, 0x652, std::uint32_t{0});
            Mfw0Value(&work, 0x656, std::uint16_t{0});
            work.veryLateHint = 0;
            work.learnedMove = 0;
            Mfw0Value(&work, 0x5a8, std::uint64_t{0});
            work.elapsed = 0;
            work.combinedPracticeEntries = 0;
            work.demonstrationCamera = 0;
            work.turning = 0;
            for (std::int32_t i = 0; i < 10; ++i) {
                work.firstFailures[i] = 0;
                work.repeatedFailures[i] = 0;
            }
            Mfw0QueueTrainingChild(sequence, Mfw0InitializeActorsSequence, MFW0_INITIALIZE, 105);
            break;
        case 101:
            Mfw0Wait(sequence, 11, 102, 2);
            break;
        case 102:
            Mfw0Wait(sequence, 3, 103);
            break;
        case 103:
            Mfw0Camera(sequence, 9000, shen_name{});
            Mfw0Wait(sequence, 3, 104);
            break;
        case 104:
            Mfw0Wait(sequence, 2, 105, 20);
            break;
        case 105:
            Mfw0Camera(sequence, 9000, shen_name{});
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0IntroductionSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            ResetTrainingAuthorCharacter(MFW0_RYO);
            ResetTrainingAuthorCharacter(MFW0_TKR);
            Mfw0QueueAuthor(SHEN_NAME(AU_0));
            Mfw0Wait(sequence, 13, 101, 5, 0);
            break;
        case 101:
            Mfw0ResetReflectors();
            Mfw0Camera(sequence, 9001, shen_name{});
            EV_AT_SetPos(MFW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, MFW0_RYO_POSITION);
            SetTrainingCharacterYaw(MFW0_RYO, 8192);
            EV_AT_SetPos(MFW0_TKR, EventTransformFlags::World | EventTransformFlags::AllAxes, MFW0_TEACHER_POSITION);
            SetTrainingCharacterYaw(MFW0_TKR, 41264);
            SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 71, 0, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(MFW0_TKR), 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(MFW0_TKR), 4994, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(MFW0_RYO), 0xf003, MotionRegion::LowerBody);
            Mfw0Advance(sequence, 119);
            break;
        case 119: {
            QueueTrainingCameraLookAt(&sequence, 9070, MFW0_RYO);
            std::int16_t choices[]{-1, 62, 60, -1};
            SetupControlSpeakAndUI(choices, 450, 0, 2);
            Mfw0Wait(sequence, 6, 120);
            break;
        }
        case 120:
            Mfw0KillSequence(sequence.children[1]);
            Mfw0KillSequence(sequence.children[3]);
            Mfw0Value(data, 0x88, sequence.choice);
            sequence.reserved1a0 = 1;
            Mfw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0FirstLessonSequence(void *data) {
    Mfw0Lesson(data, SHEN_NAME(AU_6), 107, true);
}
void Mfw0SecondLessonSequence(void *data) {
    Mfw0Lesson(data, SHEN_NAME(AU_1), 106, false);
}
void Mfw0FinalLessonSequence(void *data) {
    Mfw0Lesson(data, SHEN_NAME(AU_B), 106, false);
}
void Mfw0FinishTrainingSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            UpdateMotionWorkWithPosition(scenFindMotionwork(MFW0_RYO), nullptr, 0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(MFW0_TKR), nullptr, 0);
            ProcessAicaCommand(0xaa0, 30);
            spmFadeOut();
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0AwardMoveSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Wait(sequence, 11, 101, 2);
            break;
        case 101:
            if (work.learnedMove == -1) {
                Mfw0Advance(sequence, 103);
                break;
            }
            if (work.learnedMove == 1 || !work.learnedMove) {
                SetMoveProficiency(0, 1, 20, work.learnedMove ? 12 : 1);
                SetNewEwzoTypeForMoveLearned(work.learnedMove ? 0 : 2, 1, 20);
            }
            work.resultFrames = 150;
            Mfw0Advance(sequence, 102);
            break;
        case 102:
            work.resultFrames = static_cast<std::int16_t>(static_cast<std::uint16_t>(work.resultFrames) - 1);
            if (static_cast<std::int16_t>(GetCurrentControlInput(1)) == 512 || !work.resultFrames) {
                SetNewEwzoTypeForMoveLearned(1, 0, 0);
                Mfw0Advance(sequence, 103);
            }
            break;
        case 103:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
