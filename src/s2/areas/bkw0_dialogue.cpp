#include "s2/areas/bkw0_sequence.hpp"
#include "s2/areas/bkw0_tables.hpp"
#include "s2/areas/area_arbt.hpp"
#include "s2/advice.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name BKW0_SELECTED_ADVICE{SHEN_NAME(FS1S)}, BKW0_DISMISSAL{SHEN_NAME(topa)};
std::int32_t bkw0MasterDialogueYaw;
void Bkw0PrepareAdviceFacing(CompiledSequenceWork &sequence) {
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    if (work.adviceFacingCycle == 0 || work.adviceFacingCycle == 1)
        UpdateMotionWorkWithPosition(scenFindMotionwork(BKW0_LKN), nullptr, 0);
    else if (work.adviceFacingCycle == 2 && SetMotionRegionFlag(scenFindMotionwork(BKW0_LKN), 65, 1, MotionRegion::LowerBody)) {
        Vector3 position;
        EV_GetCharaPosition(BKW0_RYO, 5, &position, EventTransformFlags::World);
        UpdateMotionWorkWithPosition(scenFindMotionwork(BKW0_LKN), &position, 0);
    }
    if (++work.adviceFacingCycle > 2)
        work.adviceFacingCycle = 0;
}
void Bkw0CompleteAdviceFacing(CompiledSequenceWork &sequence) {
    if (SetMotionRegionFlag(scenFindMotionwork(BKW0_LKN), 65, 1, MotionRegion::LowerBody)) {
        Vector3 position;
        EV_GetCharaPosition(BKW0_RYO, 5, &position, EventTransformFlags::World);
        UpdateMotionWorkWithPosition(scenFindMotionwork(BKW0_LKN), &position, 0);
    }
    Bkw0Complete(sequence);
}
void Bkw0AdviceLine(CompiledSequenceWork &sequence, const char *voice) {
    EV_PlayDialogue(voice);
    Bkw0Wait(sequence, 4, static_cast<std::uint16_t>(sequence.mode + 1), 60);
}
void Bkw0IdleAdviceSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(3));
            Bkw0AdviceLine(sequence, choice == 0 ? "0068C017" : choice == 1 ? "0068C018" : "0068C019");
            break;
        }
        case 101:
            Bkw0AdviceLine(sequence, "0068C012");
            break;
        case 102:
            Bkw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0HesitationAdviceSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0PrepareAdviceFacing(sequence);
            Bkw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(2));
            if (!choice)
                Bkw0AdviceLine(sequence, "0068C017");
            else if (choice == 1)
                Bkw0AdviceLine(sequence, "0068C018");
            break;
        }
        case 101:
            Bkw0CompleteAdviceFacing(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0DefaultAdviceSequence(void *data) {
    Bkw0HesitationAdviceSequence(data);
}
void Bkw0GripAdviceSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0PrepareAdviceFacing(sequence);
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            Bkw0AdviceLine(sequence, "0068C012");
            break;
        case 101:
            Bkw0CompleteAdviceFacing(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0ThrowAdviceSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0PrepareAdviceFacing(sequence);
            Bkw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(3));
            Bkw0AdviceLine(sequence, choice == 0 ? "0068B007" : choice == 1 ? "0068B008" : "0068B009");
            break;
        }
        case 101:
            Bkw0CompleteAdviceFacing(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0UnnecessaryMoveAdviceSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0PrepareAdviceFacing(sequence);
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            Bkw0AdviceLine(sequence, "0068C013");
            break;
        case 101:
            Bkw0CompleteAdviceFacing(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0ArmForceAdviceSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0PrepareAdviceFacing(sequence);
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            Bkw0AdviceLine(sequence, "0068C014");
            break;
        case 101:
            Bkw0CompleteAdviceFacing(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0LegForceAdviceSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0PrepareAdviceFacing(sequence);
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            Bkw0AdviceLine(sequence, "0068C015");
            break;
        case 101:
            Bkw0CompleteAdviceFacing(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0ExcessiveForceAdviceSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0PrepareAdviceFacing(sequence);
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            Bkw0AdviceLine(sequence, "0068B006");
            break;
        case 101:
            Bkw0CompleteAdviceFacing(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0GripRetryAdviceSequence(void *data) {
    Bkw0GripAdviceSequence(data);
}
struct Bkw0AdviceSelection {
    HLib::TaskCallback callback;
    std::int32_t delay;
};
Bkw0AdviceSelection Bkw0SelectAdvice(CompiledSequenceWork &sequence) {
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    switch (work.result) {
    case 16:
        return {.callback = Bkw0DefaultAdviceSequence, .delay = 0};
    case 17:
        return {.callback = Bkw0HesitationAdviceSequence, .delay = 0};
    case 18:
        return {.callback = Bkw0GripAdviceSequence, .delay = 0};
    case 20:
        return {.callback = Bkw0IdleAdviceSequence, .delay = 0};
    default:
        break;
    }
    const std::byte *result = work.fightResults + 12 * (work.resultIndex + 1);
    if (result[3] == std::byte{1})
        return {.callback = Bkw0GripRetryAdviceSequence, .delay = 0};
    if (result[3] == std::byte{4}) {
        switch (std::to_integer<std::uint8_t>(result[2])) {
        case 2:
        case 11:
            return {.callback = Bkw0UnnecessaryMoveAdviceSequence, .delay = 0};
        case 5:
            return {.callback = Bkw0GripRetryAdviceSequence, .delay = 0};
        case 7:
            return {.callback = (std::to_integer<std::uint8_t>(work.fightResults[7]) & 1) ? Bkw0ThrowAdviceSequence : Bkw0GripAdviceSequence, .delay = 0};
        case 8:
            return {.callback = Bkw0ArmForceAdviceSequence, .delay = 0};
        case 9:
            return {.callback = Bkw0LegForceAdviceSequence, .delay = 0};
        case 10:
            return {.callback = Bkw0ExcessiveForceAdviceSequence, .delay = 0};
        case 15:
            return {.callback = Bkw0IdleAdviceSequence, .delay = 0};
        default:
            break;
        }
    }
    return {.callback = Bkw0DefaultAdviceSequence, .delay = 0};
}
void Bkw0ArrangeDismissal(Bkw0Work &work) {
    work.conversationActive = 1;
    work.camera = 9040;
    EVS_LoopCamera(9040, BKW0_BASE, BKW0_BASE);
    EV_DisableCharacterDisplay(BKW0_LKN);
    EV_AT_SetPos(BKW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_RYO_POSITION);
    EV_AT_SetPos(BKW0_BKN, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_BKN_POSITION);
    const Vector3i ryoRotation{0, arbtDestinationRotation.yaw, 0}, bknRotation{0, bkw0MasterDialogueYaw, 0};
    EV_AT_SetAngY(BKW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, ryoRotation);
    EV_AT_SetAngY(BKW0_BKN, EventTransformFlags::World | EventTransformFlags::AllAxes, bknRotation);
    mtwkRequestMotion(scenFindMotionwork(BKW0_RYO), 7952, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(BKW0_BKN), 7953, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 71, 0, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(BKW0_BKN), 71, 0, MotionRegion::LowerBody);
    SetMotionLayerFrameRange(scenFindMotionwork(BKW0_RYO), -1, 280, -1, MotionRegion::LowerBody);
    SetMotionLayerFrameRange(scenFindMotionwork(BKW0_BKN), -1, 280, -1, MotionRegion::LowerBody);
}
} // namespace
void Bkw0PracticeFeedbackSequence(void *data) {
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
            if (work.finalPractice == 2)
                Bkw0Advance(sequence, 106);
            else if (work.finalPractice == 1) {
                mtwkRequestMotion(scenFindMotionwork(BKW0_BKN), 7943, MotionRegion::LowerBody);
                Bkw0Advance(sequence, 102);
            } else {
                mtwkRequestMotion(scenFindMotionwork(BKW0_BKN), 7941, MotionRegion::LowerBody);
                Bkw0Advance(sequence, 105);
            }
            break;
        case 101:
            Bkw0ResetReflectors();
            Bkw0Wait(sequence, 2, 112, 2);
            break;
        case 102:
            Bkw0AdviceLine(sequence, "0068C027");
            break;
        case 103:
            Bkw0AdviceLine(sequence, "0068A006");
            break;
        case 104:
            Bkw0Advance(sequence, 112);
            break;
        case 105: {
            const Bkw0AdviceSelection selected = Bkw0SelectAdvice(sequence);
            Bkw0Child(sequence, selected.callback, BKW0_SELECTED_ADVICE, 1);
            sequence.resultSlot = 1;
            Bkw0Wait(sequence, 8, 112);
            break;
        }
        case 106:
            Bkw0Child(sequence, Bkw0PracticeDismissalSequence, BKW0_DISMISSAL, 1);
            sequence.waitChildIndex = 1;
            Bkw0Wait(sequence, 10, 112);
            break;
        case 112:
            work.dialogueSequence = nullptr;
            Bkw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0PracticeDismissalSequence(void *data) {
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
            Bkw0ArrangeDismissal(work);
            Bkw0Advance(sequence, 101);
            break;
        case 101:
            Bkw0AdviceLine(sequence, "0068C033");
            break;
        case 102:
            Bkw0AdviceLine(sequence, "0068C034");
            break;
        case 103:
            Bkw0AdviceLine(sequence, "0068A013");
            break;
        case 104:
            Bkw0Advance(sequence, 105);
            break;
        case 105:
            Bkw0Wait(sequence, 2, 106, 90);
            break;
        case 106:
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            spmFadeOut();
            Bkw0Wait(sequence, 2, 107, 30);
            break;
        case 107:
            work.dialogueSequence = nullptr;
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0PracticeExitSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    const std::int32_t retry = Bkw0SequenceValue<std::int32_t>(sequence, 0x140), finish = Bkw0SequenceValue<std::int32_t>(sequence, 0x150);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            if (work.practiceExpired || work.repeatedThrows >= 10) {
                Bkw0SequenceValue(sequence, 0x88, finish);
                sequence.reserved1a0 = 1;
                Bkw0Advance(sequence, 101);
            } else if (work.practiceWarning)
                Bkw0Advance(sequence, 109);
            else {
                Bkw0SequenceValue(sequence, 0x88, retry);
                sequence.reserved1a0 = 1;
                Bkw0Advance(sequence, 129);
            }
            break;
        case 101:
            Bkw0ArrangeDismissal(work);
            Bkw0Advance(sequence, 105);
            break;
        case 103:
            Bkw0Wait(sequence, 2, 104, 90);
            break;
        case 104:
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            spmFadeOut();
            Bkw0Wait(sequence, 2, 129, 30);
            break;
        case 105:
            Bkw0AdviceLine(sequence, "0068C033");
            break;
        case 106:
            Bkw0AdviceLine(sequence, "0068C034");
            break;
        case 107:
            Bkw0AdviceLine(sequence, "0068A013");
            break;
        case 108:
            Bkw0Advance(sequence, 103);
            break;
        case 129:
            Bkw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Bkw0ThrowOutcomeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    const std::int32_t success = Bkw0SequenceValue<std::int32_t>(sequence, 0x148), finish = Bkw0SequenceValue<std::int32_t>(sequence, 0x150);
    const std::int32_t retry = Bkw0SequenceValue<std::int32_t>(sequence, 0x158);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            Bkw0Advance(sequence, 103);
            break;
        case 101:
        case 102:
            return;
        case 103: {
            const std::uint8_t flags = std::to_integer<std::uint8_t>(work.fightResults[8]);
            if (flags & 0x10)
                work.throwDirection = 0;
            else if (flags & 0x20)
                work.throwDirection = 1;
            else if (flags & 0x40)
                work.throwDirection = 2;
            else if (flags & 0x80)
                work.throwDirection = 3;
            const bool fresh = !work.throwDirections[work.throwDirection];
            if (fresh) {
                work.throwDirections[work.throwDirection] = 1;
                ++work.differentThrows;
            } else
                ++work.repeatedThrows;
            if ((work.repeatedThrows == 5 || (!fresh && work.elapsed > 900)) && !bkw0AdviceIssued) {
                QueueFightAdviceControl(15);
                bkw0AdviceIssued = 1;
            }
            if (work.repeatedThrows >= 10) {
                Bkw0SequenceValue(sequence, 0x88, finish);
                sequence.reserved1a0 = 1;
                Bkw0Child(sequence, Bkw0PracticeDismissalSequence, BKW0_DISMISSAL, 1);
                sequence.waitChildIndex = 1;
                Bkw0Wait(sequence, 10, 160);
            } else if (work.differentThrows < 3) {
                work.camera = 9060;
                EVS_LoopCamera(9060, BKW0_BASE, BKW0_BASE);
                mtwkRequestMotion(scenFindMotionwork(BKW0_BKN), fresh ? 7940 : 7942, MotionRegion::LowerBody);
                Bkw0SequenceValue(sequence, 0x88, retry);
                sequence.reserved1a0 = 1;
                Bkw0Advance(sequence, fresh ? 104 : 107);
            } else {
                work.differentThrows = 0;
                work.repeatedThrows = 0;
                work.consecutiveThrows = 0;
                work.previousFailure = -1;
                work.throwDirection = 0;
                for (std::uint8_t &direction : work.throwDirections)
                    direction = 0;
                work.finalPractice = 1;
                Bkw0SequenceValue(sequence, 0x88, success);
                sequence.reserved1a0 = 1;
                mtwkRequestMotion(scenFindMotionwork(BKW0_BKN), 7943, MotionRegion::LowerBody);
                Bkw0Advance(sequence, 111);
            }
            break;
        }
        case 104:
            Bkw0AdviceLine(sequence, "0068C024");
            break;
        case 105:
        case 109:
        case 112:
            Bkw0AdviceLine(sequence, "0068A006");
            break;
        case 106:
        case 110:
        case 113:
            Bkw0Advance(sequence, 144);
            break;
        case 107:
            Bkw0AdviceLine(sequence, "0068C025");
            break;
        case 108:
            Bkw0AdviceLine(sequence, "0068C026");
            break;
        case 111:
            Bkw0AdviceLine(sequence, "0068C027");
            break;
        case 144:
            Bkw0ResetReflectors();
            Bkw0Wait(sequence, 2, 160, 2);
            break;
        case 160:
            Bkw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
