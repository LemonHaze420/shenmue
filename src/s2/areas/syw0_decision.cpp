#include "s2/areas/area_syw0.hpp"
#include "s2/areas/syw0_tables.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/motion_work.hpp"
#include "s2/save.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
#include <bit>
namespace shenmue::s2::areas {
namespace {
void Syw0StopPracticeOwner(Syw0Work &work, std::size_t offset, bool recursive = false) {
    CompiledSequenceWork *owner = Syw0Value<CompiledSequenceWork *>(&work, offset);
    if (owner) {
        if (recursive)
            RetireTrainingSequence(owner);
        else
            Syw0KillSequence(owner);
        Syw0Value(&work, offset, static_cast<CompiledSequenceWork *>(nullptr));
    }
}
void Syw0StopPracticeTasks(Syw0Work &work) {
    Syw0StopPracticeOwner(work, 0x5c8, true);
    Syw0StopPracticeOwner(work, 0x5c0);
    Syw0StopPracticeOwner(work, 0x5d0);
    Syw0StopPracticeOwner(work, 0x5d8);
}
void Syw0StartPracticeTimers(CompiledSequenceWork &sequence, Syw0Work &work, std::int64_t practice) {
    Syw0Value(&work, 0x5d0, Syw0Child(sequence, TrainingTimerSequence, SHEN_NAME(TIMM), 4));
    CompiledSequenceWork *advice = Syw0Child(sequence, TrainingAdviceTimerSequence, SHEN_NAME(WADV), 5);
    Syw0Value(&work, 0x5d8, advice);
    Syw0Value(advice, 0x138, practice);
    Syw0Value(advice, 0x140, std::int64_t{0});
}
void Syw0SelectPracticeCamera(CompiledSequenceWork &sequence, Syw0Work &work, std::int64_t practice) {
    std::int32_t camera = 0;
    if (Syw0Value<std::int32_t>(&work, 0x5b0)) {
        if (practice >= 0 && practice <= 1)
            camera = 9010 + (Syw0Value<std::int32_t>(&work, 0x5f4) == 9010);
        else if ((practice >= 2 && practice <= 3) || practice == 9)
            camera = 9012 + (Syw0Value<std::int32_t>(&work, 0x5f4) == 9012);
        Syw0Value(&work, 0x5b0, std::int32_t{0});
    } else
        camera = Syw0Value<std::int32_t>(&work, 0x5f4);
    Syw0Value(&work, 0x5f4, camera);
    SetTrainingCameraSequence(&sequence, camera);
}
bool Syw0NeedsFacingReset(std::int32_t practice) {
    std::int32_t target = 0;
    if (practice >= 0 && practice <= 1)
        target = 49152;
    else if (practice == 2 || practice == 9)
        target = trainingPracticeTargetYaw;
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(SYW0_RYO, &rotation, EventTransformFlags::World);
    const std::uint32_t delta = static_cast<std::uint32_t>(target) - static_cast<std::uint32_t>(rotation.y);
    const std::uint16_t forward = static_cast<std::uint16_t>(delta), backward = static_cast<std::uint16_t>(0U - delta);
    const std::uint32_t difference = forward >= backward ? backward : 0U - (target > rotation.y ? delta : forward);
    const std::uint32_t magnitude = std::bit_cast<std::int32_t>(difference) < 0 ? 0U - difference : difference;
    return std::bit_cast<std::int32_t>(magnitude) > 10922;
}
void Syw0ShowPracticeLcd(Syw0Work &work, bool weight) {
    const std::uint8_t *bitmap = weight ? SYW0_WEIGHT_LCD : SYW0_FORCE_LCD;
    Syw0Value(&work, 0x638, bitmap);
    Syw0Value(&work, 0x630, std::int32_t{600});
    LcdmSelectMessage(1);
    LcdmInit(reinterpret_cast<std::uintptr_t>(bitmap));
}
CompiledSequenceWork *Syw0QueueFeedback(CompiledSequenceWork &sequence, std::int64_t practice) {
    if (practice == 0)
        return Syw0Child(sequence, Syw0WeightFeedbackSequence, SHEN_NAME(FS_1), 1);
    if (practice == 1)
        return Syw0Child(sequence, Syw0ForceFeedbackSequence, SHEN_NAME(FS_2), 1);
    if (practice == 2 || practice == 9)
        return Syw0Child(sequence, Syw0GateFeedbackSequence, SHEN_NAME(FS_3), 1);
    return nullptr;
}
void Syw0WaitReposition(CompiledSequenceWork &sequence, std::uint16_t next) {
    if (!Syw0Value<std::int32_t>(&sequence, 0x250)) {
        sequence.reserved04 = next;
        Syw0Advance(sequence, 133);
    } else
        Syw0Advance(sequence, next);
}
} // namespace
void Syw0PracticeDecisionSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Syw0Work &work = *static_cast<Syw0Work *>(sequence.event);
    const std::int64_t practice = Syw0Value<std::int64_t>(&sequence, 0x138), instruction = Syw0Value<std::int64_t>(&sequence, 0x140);
    const std::int64_t second = Syw0Value<std::int64_t>(&sequence, 0x148), success = Syw0Value<std::int64_t>(&sequence, 0x150);
    const std::int64_t retry = Syw0Value<std::int64_t>(&sequence, 0x160), restart = Syw0Value<std::int64_t>(&sequence, 0x168);
    const std::int64_t finish = Syw0Value<std::int64_t>(&sequence, 0x170);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            if (!Syw0Value<std::uint8_t>(&work, 0x651))
                Syw0Advance(sequence, 100);
            break;
        case 100:
            Syw0Value(&work, 0x5e8, static_cast<std::int32_t>(practice));
            for (std::size_t offset : {0x538U, 0x540U, 0x548U, 0x550U})
                Syw0Value(&work, offset, std::int32_t{0});
            Syw0Value(&work, 0x5b0, std::int32_t{1});
            Syw0Value(&work, 0x5b8, std::int32_t{1});
            for (std::size_t offset : {0x5c0U, 0x5c8U, 0x5d0U, 0x5d8U})
                Syw0Value(&work, offset, static_cast<CompiledSequenceWork *>(nullptr));
            Syw0ResetPracticeActors(static_cast<std::int32_t>(practice));
            SetMotionRegionFlag(scenFindMotionwork(SYW0_KNK), 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(SYW0_SYE), 8841, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 71, 0, MotionRegion::LowerBody);
            Syw0Value(&sequence, 0x250, std::int32_t{1});
            Syw0Advance(sequence, 101);
            break;
        case 101:
            Syw0SelectPracticeCamera(sequence, work, practice);
            Syw0Value(&work, 0x5fc, std::int32_t{0});
            Syw0StartPracticeTimers(sequence, work, practice);
            Syw0Value(&work, 0x600, std::int32_t{0});
            Syw0Value(&sequence, 0x248, std::int32_t{0});
            Syw0Advance(sequence, 102);
            break;
        case 102: {
            if (practice == 2 && Syw0Value<std::uint32_t>(&work, 0x548) >= 10) {
                Syw0Value(&sequence, 0x88, static_cast<std::int32_t>(finish));
                sequence.reserved1a0 = 1;
                Syw0Advance(sequence, 132);
                break;
            }
            Syw0Value(&sequence, 0x24c, std::int32_t{0});
            Syw0Value(&work, 0x604, std::int32_t{0});
            if (Syw0Value<std::int32_t>(&sequence, 0x250)) {
                Syw0SelectPracticeCamera(sequence, work, practice);
                Syw0ResetPracticeActors(static_cast<std::int32_t>(practice));
            }
            CompiledSequenceWork *child = Syw0Child(sequence, Syw0PracticeSequence, SHEN_NAME(WDTP), 0);
            Syw0Value(child, 0x138, instruction);
            Syw0Value(child, 0x140, second);
            Syw0Value(child, 0x148, practice);
            sequence.resultSlot = 0;
            Syw0Wait(sequence, 8, 103);
            break;
        }
        case 103:
            Syw0Value(&sequence, 0x250, std::int32_t(Syw0NeedsFacingReset(static_cast<std::int32_t>(practice))));
            if (Syw0Value<std::uint8_t>(&work, 0x79) == 2 && practice == 9 && Syw0Value<std::int32_t>(&work, 0x538))
                Syw0Value(&sequence, 0x250, std::int32_t{0});
            Syw0Value(&work, 0x5b0, std::int32_t{1});
            for (std::size_t i = 0; i < 9; ++i)
                Syw0Value(&work, 0x84 + i, Syw0Value<std::uint8_t>(&work, 0x78 + i));
            if (Syw0Value<std::uint8_t>(&work, 0x7a) != 20)
                Syw0Value(&work, 0x554, Syw0Value<std::int32_t>(&work, 0x554) + 1);
            Syw0Advance(sequence, Syw0Value<std::uint8_t>(&work, 0x79) == 2 ? 104 : 116);
            break;
        case 104:
            if (!Syw0Value<std::int32_t>(&sequence, 0x250)) {
                if (practice >= 0 && practice <= 1)
                    QueueTrainingReposition(&sequence, 2, SYW0_RYO, 0, &SYW0_PLAYER_WEIGHT_POSITION);
                else if (practice == 2) {
                    QueueTrainingReposition(&sequence, 2, SYW0_RYO, 0, &SYW0_PLAYER_FORCE_POSITION);
                    QueueTrainingReposition(&sequence, 6, SYW0_KNK, 1, &SYW0_OPPONENT_FORCE_POSITION);
                }
            }
            Syw0Value(&work, 0x548, Syw0Value<std::int32_t>(&work, 0x548) + 1);
            Syw0Value(&work, 0x54c, Syw0Value<std::int32_t>(&work, 0x54c) + 1);
            Syw0Wait(sequence, 2, 105, 1);
            break;
        case 105:
            if (Syw0Value<std::int32_t>(&work, 0x538)) {
                scenCvsCtrlFree(nullptr);
                Syw0StopPracticeTasks(work);
                Syw0Value(&work, 0x5ec, std::int32_t{Syw0Value<std::uint8_t>(&work, 0x86)});
                Syw0Value(&work, 0x5f0, std::int32_t{0});
                Syw0Value(&work, 0x66c, Syw0Value<std::int32_t>(&work, 0x66c) + 1);
                Syw0Advance(sequence,
                    (Syw0Value<std::int32_t>(&work, 0x53c) | Syw0Value<std::int32_t>(&work, 0x540) | Syw0Value<std::int32_t>(&work, 0x544)) ? 106 : 114);
            } else {
                if (Syw0Value<std::uint8_t>(&work, 0x7a) != 20)
                    Syw0Value(&work, 0x5b8, std::int32_t{1});
                Syw0WaitReposition(sequence, 130);
            }
            break;
        case 106:
            mtwkSetExtraFlag(scenFindMotionwork(SYW0_RYO), 6, 0);
            Syw0WaitReposition(sequence, 107);
            break;
        case 107:
            if (Syw0Value<std::int32_t>(&work, 0x540) && practice >= 0 && practice <= 2)
                Syw0ShowPracticeLcd(work, practice == 0);
            if (Syw0QueueFeedback(sequence, practice)) {
                sequence.resultSlot = 1;
                Syw0Wait(sequence, 8, practice == 0 ? 108 : practice == 1 ? 109 : 110);
            }
            break;
        case 108:
        case 109:
        case 110: {
            CompiledSequenceWork *child = Syw0Child(sequence, Syw0ExitPracticeSequence, SHEN_NAME(EALP), 1);
            Syw0Value(child, 0x138, std::int64_t{1});
            Syw0Value(child, 0x140, retry);
            Syw0Value(child, 0x148, restart);
            Syw0Value(child, 0x150, finish);
            Syw0Value(child, 0x158, practice);
            sequence.resultSlot = 1;
            Syw0Wait(sequence, 8, 112);
            break;
        }
        case 111:
        case 124:
        case 125:
            break;
        case 112:
            Syw0Value(&sequence, 0x88, Syw0Value<std::int32_t>(&sequence, 0xbc));
            sequence.reserved1a0 = 1;
            Syw0Advance(sequence, 113);
            break;
        case 113:
            Syw0Value(&sequence, 0x250, std::int32_t{1});
            Syw0Advance(sequence, 115);
            break;
        case 114: {
            CompiledSequenceWork *feedback = Syw0QueueFeedback(sequence, practice);
            if (feedback) {
                Syw0Value(&work, 0x5c8, feedback);
                Syw0Value(&sequence, 0x88, static_cast<std::int32_t>(retry));
                sequence.reserved1a0 = 1;
            }
            Syw0Advance(sequence, 115);
            break;
        }
        case 115: {
            const bool quiet = Syw0Value<std::uint8_t>(&work, 0x7a) == 20 && !Syw0Value<std::int32_t>(&work, 0x53c) && !Syw0Value<std::int32_t>(&work, 0x540) &&
                !Syw0Value<std::int32_t>(&work, 0x544);
            Syw0Value(&work, 0x5b8, std::int32_t(Syw0Value<std::int32_t>(&work, 0x538) ? 2 : quiet ? 0 : 1));
            for (std::size_t offset : {0x5fcU, 0x538U, 0x540U, 0x54cU})
                Syw0Value(&work, offset, std::int32_t{0});
            if (Syw0Value<std::int32_t>(&sequence, 0x88) == retry)
                Syw0WaitReposition(sequence, 131);
            else
                Syw0Advance(sequence, 132);
            break;
        }
        case 116:
            scenCvsCtrlFree(nullptr);
            Syw0StopPracticeTasks(work);
            mtwkSetExtraFlag(scenFindMotionwork(SYW0_RYO), 6, 0);
            Syw0Wait(sequence, 2, 117, 1);
            break;
        case 117:
            if (!Syw0Value<std::int32_t>(&work, 0x558 + 4 * practice)) {
                Syw0Value(&work, 0x580 + 4 * practice, std::int32_t{0});
                Syw0Value(&work, 0x558 + 4 * practice, std::int32_t{1});
            }
            Syw0Advance(sequence, 120);
            break;
        case 120:
            Syw0Value(&work, 0x5ec, std::int32_t{Syw0Value<std::uint8_t>(&work, 0x86)});
            if (practice >= 0 && practice <= 2)
                Syw0Advance(sequence, static_cast<std::uint16_t>(121 + practice));
            break;
        case 121:
        case 122:
        case 123: {
            const std::int32_t mode = sequence.mode;
            Syw0Value(&sequence, 0x88, static_cast<std::int32_t>(success));
            sequence.reserved1a0 = 1;
            if (mode == 122 && Syw0Value<std::int32_t>(&work, 0x640)) {
                RetireTrainingFight(&work, SYW0_RYO);
                check_dead_status(1);
            }
            Syw0ShowPracticeLcd(work, mode == 121);
            const HLib::TaskCallback callback = mode == 121 ? Syw0WeightSuccessSequence : mode == 122 ? Syw0ForceSuccessSequence : Syw0GateSuccessSequence;
            const shen_name token = mode == 121 ? SHEN_NAME(SS_1) : mode == 122 ? SHEN_NAME(SS_2) : SHEN_NAME(SS_3);
            CompiledSequenceWork *child = Syw0Child(sequence, callback, token, 1);
            Syw0Value(child, 0x138, std::int64_t{Syw0Value<std::int32_t>(&work, 0x5ec)});
            Syw0Value(child, 0x140, std::int64_t{Syw0Value<std::int32_t>(&work, 0x5f0)});
            Syw0Value(child, 0x148, success);
            Syw0Value(child, 0x150, finish);
            if (mode == 121)
                Syw0Value(child, 0x158, retry);
            sequence.resultSlot = 1;
            Syw0Wait(sequence, 8, 126);
            if (mode == 121)
                SetQteCameraAtTop(1);
            break;
        }
        case 126:
            Syw0Value(&work, 0x74, std::int8_t{5});
            Syw0Value(&sequence, 0x88, Syw0Value<std::int32_t>(&sequence, 0xbc));
            sequence.reserved1a0 = 1;
            Syw0Advance(sequence, 127);
            break;
        case 127:
            Syw0Advance(sequence, 128);
            SetQteCameraAtTop(0);
            break;
        case 128:
            if (!Syw0Value<std::int32_t>(&sequence, 0x24c)) {
                Syw0Value(&work, 0x5f8, std::int32_t{0});
                Syw0Advance(sequence, 132);
            } else {
                Syw0Value(&sequence, 0x250, std::int32_t{1});
                Syw0StartPracticeTimers(sequence, work, practice);
                Syw0Wait(sequence, 2, 129, 2);
            }
            break;
        case 129:
            Syw0Advance(sequence, 130);
            break;
        case 130:
            if (Syw0Value<std::uint32_t>(&work, 0x600) < 100)
                Syw0Value(&work, 0x600, Syw0Value<std::uint32_t>(&work, 0x600) + 1);
            Syw0Advance(sequence, 102);
            break;
        case 131:
            Syw0Advance(sequence, 101);
            break;
        case 132: {
            Syw0StopPracticeOwner(work, 0x5d0);
            Syw0StopPracticeOwner(work, 0x5d8);
            Syw0StopPracticeOwner(work, 0x5c8, true);
            Syw0StopPracticeOwner(work, 0x5c0);
            Syw0Complete(sequence, true);
            Vector3 position;
            EV_GetCharaPosition(SYW0_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            return;
        }
        case 133:
            if (Syw0Value<std::int32_t>(&work, 0x604))
                break;
            [[fallthrough]];
        case 134:
            if (!Syw0Value<std::int32_t>(&work, 0x608))
                Syw0Advance(sequence, sequence.reserved04);
            break;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
        Vector3 position;
        EV_GetCharaPosition(SYW0_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
