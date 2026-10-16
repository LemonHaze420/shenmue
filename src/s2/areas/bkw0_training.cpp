#include "s2/areas/bkw0_sequence.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
void Bkw0TrainingSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            work.lcdSequence = nullptr;
            Bkw0Advance(sequence, 103);
            break;
        case 103:
            if (!work.lcdSequence)
                work.lcdSequence = Bkw0Child(sequence, CompiledLcdCountdownCallback, BKW0_LCD, 1);
            Bkw0Child(sequence, Bkw0StartTrainingSequence, BKW0_START, 0);
            sequence.resultSlot = 0;
            Bkw0Wait(sequence, 8, work.selector == 1 ? 120 : 106);
            break;
        case 106:
            Bkw0Child(sequence, Bkw0IntroductionSequence, BKW0_INTRO, 0);
            sequence.resultSlot = 0;
            Bkw0Wait(sequence, 8, 119);
            break;
        case 119: {
            const Vector3 position{112.1f, 100.0f, 204.6f};
            const Vector3i rotation{0, 17294, 0};
            EV_AT_SetPos(BKW0_BKN, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(BKW0_BKN, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            mtwkRequestMotion(scenFindMotionwork(BKW0_BKN), 7939, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(BKW0_BKN), 71, 0, MotionRegion::LowerBody);
            work.differentThrows = 0;
            work.repeatedThrows = 0;
            work.consecutiveThrows = 0;
            work.previousFailure = -1;
            work.throwDirection = 0;
            for (std::uint8_t &direction : work.throwDirections)
                direction = 0;
            Bkw0Advance(sequence, 120);
            break;
        }
        case 120:
        case 121: {
            const bool second = sequence.mode == 121;
            CompiledSequenceWork *child = Bkw0Child(sequence, Bkw0PracticeDecisionSequence, BKW0_PRACTICE, 0);
            Bkw0SequenceValue(*child, 0x138, std::int32_t(second));
            Bkw0SequenceValue(*child, 0x140, std::int32_t(second ? 8 : 4));
            Bkw0SequenceValue(*child, 0x148, std::int32_t{-1});
            Bkw0SequenceValue(*child, 0x150, std::int32_t{121});
            Bkw0SequenceValue(*child, 0x158, std::int32_t(second ? 121 : 120));
            Bkw0SequenceValue(*child, 0x160, std::int32_t(second ? 121 : 120));
            Bkw0SequenceValue(*child, 0x168, std::int32_t{119});
            Bkw0SequenceValue(*child, 0x170, std::int32_t{124});
            sequence.mode = 9;
            sequence.nextMode = 0;
            sequence.resultSlot = 0;
            sequence.continueAfterWait = 1;
            break;
        }
        case 124:
            Bkw0Child(sequence, Bkw0FinishTrainingSequence, BKW0_FINISH, 0);
            sequence.resultSlot = 0;
            Bkw0Wait(sequence, 8, 126);
            break;
        case 126:
            Bkw0Child(sequence, Bkw0AwardMoveSequence, BKW0_AWARD, 0);
            sequence.resultSlot = 0;
            Bkw0Wait(sequence, 8, 127);
            break;
        case 127:
            work.active = 0;
            if (work.lcdSequence) {
                Bkw0KillSequence(sequence.children[1]);
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
void Bkw0StartTrainingSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            work.firstFailures[0] = 0;
            work.firstFailures[1] = 0;
            work.firstFailures[2] = 0;
            work.adviceFacingCycle = 0;
            work.learnedMove = 0;
            work.finalPractice = 0;
            work.reserved5a8 = 0;
            work.conversationActive = 0;
            work.elapsed = 0;
            work.reserved5b4 = 0;
            work.reserved5bc = 0;
            work.practiceBlocked = 0;
            bkw0AdviceIssued = 0;
            for (std::int32_t i = 0; i < 10; ++i) {
                work.firstFailures[i] = 0;
                work.repeatedFailures[i] = 0;
            }
            Bkw0Child(sequence, Bkw0InitializeTrainingSequence, BKW0_INITIALIZE, 0);
            sequence.resultSlot = 0;
            Bkw0Wait(sequence, 8, 105);
            break;
        case 105:
            Bkw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0IntroductionSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            ResetTrainingAuthorCharacter(BKW0_RYO);
            ResetTrainingAuthorCharacter(BKW0_LKN);
            ResetTrainingAuthorCharacter(BKW0_BKN);
            figpAssignModeFor(BKW0_RYO, 0);
            figpAssignModeFor(BKW0_LKN, 0);
            Bkw0QueueAuthor(BKW0_AUTHOR_INTRO);
            Bkw0Wait(sequence, 2, 101, 2);
            break;
        case 101:
            Bkw0Wait(sequence, 13, 103, 5, 0);
            break;
        case 103:
            Bkw0Wait(sequence, 11, 104, 2);
            break;
        case 104:
            Bkw0ResetReflectors();
            Bkw0Wait(sequence, 2, 123, 1);
            break;
        case 123:
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            Bkw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0FinishTrainingSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        if (!sequence.mode)
            Bkw0Advance(sequence, 100);
        else if (sequence.mode == 100) {
            UpdateMotionWorkWithPosition(scenFindMotionwork(BKW0_RYO), nullptr, 0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(BKW0_LKN), nullptr, 0);
            ProcessAicaCommand(0xaa0, 30);
            Bkw0Complete(sequence);
            return;
        } else
            UpdateCompiledSequenceWait(&sequence);
    } while (sequence.continueLoop);
}
void Bkw0AwardMoveSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            Bkw0Wait(sequence, 11, 101, 2);
            break;
        case 101:
            if (work.learnedMove == -1)
                Bkw0Advance(sequence, 103);
            else {
                work.resultFrames = 150;
                if (work.learnedMove == 4)
                    SetMoveProficiency(0, 3, 15, 12);
                else if (!work.learnedMove) {
                    SetMoveProficiency(0, 3, 15, 1);
                    SetNewEwzoTypeForMoveLearned(2, 3, 15);
                }
                Bkw0Advance(sequence, 102);
            }
            break;
        case 102:
            --work.resultFrames;
            if (GetCurrentControlInput(1) == 512 || !work.resultFrames) {
                SetNewEwzoTypeForMoveLearned(1, 0, 0);
                Bkw0Advance(sequence, 103);
            }
            break;
        case 103:
            Bkw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
