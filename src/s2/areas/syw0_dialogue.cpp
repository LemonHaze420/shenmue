#include "s2/areas/area_syw0.hpp"
#include "s2/areas/syw0_tables.hpp"
#include "s2/advice.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/screen_film.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
void Syw0TurnToTeacher(CompiledSequenceWork &sequence) {
    CompiledSequenceWork *child = Syw0Child(sequence, TrainingTurnSequence, SHEN_NAME(TURN), 1);
    Syw0Value(child, 0x138, std::int64_t{SYW0_RYO.name});
    Syw0Value(child, 0x140, std::int64_t{SYW0_SYE.name});
    Syw0Value(child, 0x148, std::int64_t{15});
    Syw0Value(child, 0x150, std::int64_t{10});
    sequence.resultSlot = 1;
}
void Syw0FeedbackParent(void *data, std::int32_t kind) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0Advance(sequence, 100);
            break;
        case 100:
            Syw0Advance(sequence, 105);
            break;
        case 105: {
            const shen_name token = kind == 0 ? SHEN_NAME(FS1S) : kind == 1 ? SHEN_NAME(FS2S) : SHEN_NAME(FS3S);
            Syw0Child(sequence, kind == 0 ? Syw0SelectWeightFeedback(sequence) : Syw0SelectForceFeedback(sequence), token, 1);
            sequence.resultSlot = 1;
            Syw0Wait(sequence, 8, 106);
            break;
        }
        case 106:
            Syw0Value(sequence.event, 0x5c8, static_cast<CompiledSequenceWork *>(nullptr));
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0SuccessResult(CompiledSequenceWork &sequence) {
    Syw0Value(&sequence, 0x88, Syw0Value<std::int32_t>(&sequence, 0x148));
    sequence.reserved1a0 = 1;
}
} // namespace
void Syw0WeightFeedbackSequence(void *data) {
    Syw0FeedbackParent(data, 0);
}
void Syw0ForceFeedbackSequence(void *data) {
    Syw0FeedbackParent(data, 1);
}
void Syw0GateFeedbackSequence(void *data) {
    Syw0FeedbackParent(data, 2);
}
void Syw0ExitPracticeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Syw0Work &work = *static_cast<Syw0Work *>(sequence.event);
    const std::int32_t retry = Syw0Value<std::int32_t>(&sequence, 0x140), restart = Syw0Value<std::int32_t>(&sequence, 0x148);
    const std::int64_t practice = Syw0Value<std::int64_t>(&sequence, 0x158);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0Advance(sequence, 100);
            break;
        case 100:
            if (Syw0Value<std::int32_t>(&work, 0x544)) {
                Syw0Value(&work, 0x544, std::int32_t{0});
                Syw0Value(&sequence, 0x88, retry);
                sequence.reserved1a0 = 1;
                Syw0Advance(sequence, 132);
            } else if (!Syw0Value<std::int32_t>(&work, 0x540)) {
                Syw0Value(&sequence, 0x88, retry);
                sequence.reserved1a0 = 1;
                Syw0Advance(sequence, 132);
            } else
                Syw0Advance(sequence, 105);
            break;
        case 105:
            Syw0Value(&work, 0x5ac, std::int32_t{1});
            Syw0Advance(sequence, 112);
            break;
        case 106:
            EV_PlayDialogue("0064B050");
            Syw0Wait(sequence, 4, 107, 60);
            break;
        case 107:
            EV_PlayDialogue("0064B051");
            Syw0Wait(sequence, 4, 108, 60);
            break;
        case 108:
            Syw0Advance(sequence, 118);
            break;
        case 109:
            EV_PlayDialogue("0064A015");
            Syw0Wait(sequence, 4, 110, 60);
            break;
        case 110:
            EV_PlayDialogue("0064B054");
            Syw0Wait(sequence, 4, 111, 60);
            break;
        case 111:
            Syw0Advance(sequence, 130);
            break;
        case 112: {
            Syw0TurnToTeacher(sequence);
            Vector3 target;
            EV_GetCharaPosition(SYW0_RYO, 5, &target, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_SYE), &target, 0);
            if (practice >= 0 && practice <= 1) {
                EV_AT_SetPos(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, SYW0_TEACHER_WEIGHT_POSITION);
                SetTrainingCharacterYaw(SYW0_SYE, 32768);
            } else if (practice == 2 || practice == 9) {
                EV_AT_SetPos(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, SYW0_TEACHER_FORCE_POSITION);
                SetTrainingCharacterYaw(SYW0_SYE, 49152);
            }
            mtwkRequestMotion(scenFindMotionwork(SYW0_SYE), 8841, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 71, 0, MotionRegion::LowerBody);
            sequence.resultSlot = 1;
            Syw0Wait(sequence, 8, 113);
            break;
        }
        case 113:
            if (practice >= 0 && practice <= 1)
                QueueTrainingCameraLookAt(&sequence, 9040, SYW0_BASE);
            else if (practice == 2 || practice == 9)
                QueueTrainingCameraLookAt(&sequence, 9041, SYW0_BASE);
            Syw0Advance(sequence, 106);
            break;
        case 114:
            Syw0RestoreTrainingCharacters();
            Syw0Advance(sequence, 131);
            break;
        case 118: {
            std::int16_t choices[]{-1, 62, 60, -1};
            SetupControlSpeakAndUI(choices, 450, 0, 2);
            Syw0Wait(sequence, 6, 119);
            break;
        }
        case 119:
            Syw0KillSequence(sequence.children[3]);
            if (practice >= 0 && practice <= 1)
                QueueFightAdviceControl(17);
            else if (practice == 2 || practice == 9)
                QueueFightAdviceControl(18);
            Syw0Advance(sequence, sequence.choice == 62 ? 131 : 125);
            break;
        case 120:
            Syw0PrepareAuthorCharacters();
            Syw0QueueAuthor(SYW0_AUTHORS[3]);
            Syw0Wait(sequence, 13, 114, 5, 0);
            break;
        case 125:
            Syw0Advance(sequence, 109);
            break;
        case 130:
            Syw0Value(&work, 0x540, std::int32_t{0});
            Syw0Value(&sequence, 0x88, retry);
            sequence.reserved1a0 = 1;
            Syw0Advance(sequence, 132);
            break;
        case 131:
            Syw0Value(&sequence, 0x88, restart);
            sequence.reserved1a0 = 1;
            Syw0Value(&work, 0x540, std::int32_t{0});
            Syw0Advance(sequence, 132);
            break;
        case 132:
            Syw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0WeightSuccessSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Syw0Work &work = *static_cast<Syw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0Advance(sequence, 100);
            break;
        case 100:
            Syw0Advance(sequence, 103);
            break;
        case 101:
        case 102:
            return;
        case 103: {
            EV_SetCinemaBarVisibility(FilmControlFlag::Show, 5);
            work.camera = 9045;
            EVS_LoopCamera(9045, SYW0_BASE, SYW0_BASE);
            EV_AT_SetPos(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, SYW0_TEACHER_WEIGHT_POSITION);
            const Vector3i rotation{.x = 0, .y = 32768, .z = 0};
            EV_AT_SetAngY(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            Vector3 target;
            EV_GetCharaPosition(SYW0_RYO, 5, &target, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_SYE), &target, 0);
            mtwkRequestMotion(scenFindMotionwork(SYW0_SYE), 8841, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 71, 0, MotionRegion::LowerBody);
            Syw0Advance(sequence, 104);
            break;
        }
        case 104:
            Syw0TurnToTeacher(sequence);
            Syw0Wait(sequence, 8, 105);
            break;
        case 105:
            EV_PlayDialogue("0064B035");
            Syw0Wait(sequence, 4, 106, 60);
            break;
        case 106:
            EV_PlayDialogue("0064B036");
            Syw0Wait(sequence, 4, 107, 60);
            work.camera = 9046;
            EVS_LoopCamera(9046, SYW0_BASE, SYW0_BASE);
            break;
        case 107:
            EV_PlayDialogue("0064B037");
            Syw0Wait(sequence, 4, 108, 60);
            break;
        case 108:
            Syw0Advance(sequence, 146);
            break;
        case 146:
            EV_SetCinemaBarVisibility(FilmControlFlag::Hide, 5);
            Syw0RestoreTrainingCharacters();
            Syw0Wait(sequence, 2, 154, 2);
            break;
        case 154:
            Syw0SuccessResult(sequence);
            Syw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0ForceSuccessSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0Wait(sequence, 2, 100, 3);
            break;
        case 100:
            Syw0Advance(sequence, 103);
            break;
        case 101:
        case 102:
            return;
        case 103:
            Syw0PrepareAuthorCharacters();
            Syw0QueueAuthor(SYW0_AUTHORS[4]);
            Syw0Wait(sequence, 13, 134, 5, 0);
            break;
        case 134:
            Syw0SuccessResult(sequence);
            Syw0RestoreTrainingCharacters();
            EV_EnableCharacterDisplay(SYW0_KNK);
            Syw0Wait(sequence, 2, 142, 2);
            break;
        case 142:
            Syw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0GateSuccessSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0Advance(sequence, 100);
            break;
        case 100:
            Syw0Advance(sequence, 101);
            break;
        case 101:
            QueueExtraTelop("", 99);
            Syw0PrepareAuthorCharacters();
            EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
            Syw0QueueAuthor(SYW0_AUTHORS[5]);
            Syw0Child(sequence, Syw0AwardSequence, SHEN_NAME(TELP), 0);
            sequence.resultSlot = 0;
            Syw0Wait(sequence, 13, 102, 5, 0);
            break;
        case 102:
            SetNewEwzoTypeForMoveLearned(1, 0, 0);
            Syw0SuccessResult(sequence);
            EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            Syw0RestoreTrainingCharacters();
            Syw0Wait(sequence, 2, 103, 2);
            break;
        case 103:
            Syw0SuccessResult(sequence);
            if (sequence.parent) {
                Syw0Value(sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
                Syw0Value(sequence.parent, 0xb8 + 4 * sequence.childSlot, Syw0Value<std::int32_t>(&sequence, 0x88));
            }
            for (CompiledSequenceWork *child : sequence.children)
                if (child) {
                    ClearTrainingSequenceChildren(child);
                    Syw0KillSequence(child);
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
} // namespace shenmue::s2::areas
