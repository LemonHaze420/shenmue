#include "s2/areas/area_knw0.hpp"
#include "s2/areas/knw0_authored.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/model_component.hpp"
#include "s2/input.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
extern std::int32_t knw0AdviceShown;
CompiledSequenceWork *Knw0Child(CompiledSequenceWork &parent, HLib::TaskCallback callback, shen_name token, std::int32_t slot) {
    CompiledSequenceWork *child = QueueCompiledSequence(callback, token, parent.event);
    if (!parent.children[slot])
        parent.children[slot] = child;
    child->parent = &parent;
    child->childSlot = slot;
    child->reference = parent.reference + 1;
    parent.timer = slot;
    Knw0Value(&parent, 0x90 + 4 * slot, std::int32_t{0});
    return child;
}
void Knw0Complete(CompiledSequenceWork &sequence, bool result) {
    if (sequence.parent) {
        Knw0Value(sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
        if (result)
            Knw0Value(sequence.parent, 0xb8 + 4 * sequence.childSlot, sequence.reserved88);
    }
    RetireCompiledSequenceLinks(&sequence);
    HLib::DestroyCurrentTask();
}
void Knw0KillSequence(CompiledSequenceWork *sequence) {
    if (sequence && sequence->task) {
        HLib::Task *task = sequence->task;
        RetireCompiledSequenceLinks(sequence);
        HLib::DestroyTaskInternal(task);
    }
}
void Knw0QueueAuthor(shen_name command) {
    Knw0PrepareAuthorCharacter(KNW0_RYO);
    Knw0PrepareAuthorCharacter(KNW0_KNK);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Knw0AuthorMainCallback, 4, 0x0b, 0x18, KNW0_TASK);
    Knw0AuthorWork &work = *static_cast<Knw0AuthorWork *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.state = 1;
    work.author = command;
    SetPlayerControlEnabled(0);
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetObjectFlagByIndex(4, 0);
}
void Knw0RestoreActors() {
    const Vector4 reflector{.x = 1, .y = .30000001192092896f, .z = .30000001192092896f, .w = .30000001192092896f};
    for (shen_name character : {KNW0_RYO, KNW0_KNK}) {
        EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(character, false);
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 1);
        SetScenarioReflectorBoardValues(character, &reflector);
        EV_SetRefBoardParamsFor(character, 1);
    }
}
void Knw0InitializeActors(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    const Vector4 reflector{.x = 1, .y = .5f, .z = .5f, .w = .5f};
    EV_DisableCharacterDisplay(KNW0_RYO);
    EV_EnableCharacterDisplay(KNW0_KNK);
    EV_DisableCharacterDisplay(KNW0_DEKU);
    EV_SetCharacterAlphaEnabled(KNW0_RYO, false);
    EV_SetCharacterAlphaEnabled(KNW0_KNK, false);
    for (shen_name character : {KNW0_RYO, KNW0_KNK}) {
        hmdlSetFlagsFor(FindCharacterByScenarioName(character), 0, 0, 16);
        hmdlSetFlagsFor(FindCharacterByScenarioName(character), 1, 0, 16);
    }
    for (shen_name character : {KNW0_RYO, KNW0_KNK, KNW0_DEKU}) {
        EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
    }
    for (shen_name character : {KNW0_RYO, KNW0_KNK})
        EV_SetCharaControllerFlags(character, 2, NpcCharacterControllerState::Disabled);
    SetScenarioCharacterDrawControl(KNW0_RYO, 1, 1);
    SetScenarioCharacterDrawControl(KNW0_KNK, 1, 1);
    EV_mtwkSetEnabled(scenFindMotionwork(KNW0_RYO), 1);
    EV_mtwkSetEnabled(scenFindMotionwork(KNW0_KNK), 0);
    SetMotionSpeed(scenFindMotionwork(KNW0_RYO), 1, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(KNW0_KNK), 1, MotionRegion::LowerBody);
    for (shen_name character : {KNW0_RYO, KNW0_KNK}) {
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 1);
        SetScenarioReflectorBoardValues(character, &reflector);
        EV_SetRefBoardParamsFor(character, 1);
    }
    EV_AT_SetPos(KNW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, KNW0_INITIAL_PLAYER);
    SetTrainingCharacterYaw(KNW0_RYO, 49152);
    SetMotionRegionFlag(scenFindMotionwork(KNW0_RYO), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(KNW0_KNK, EventTransformFlags::World | EventTransformFlags::AllAxes, KNW0_INITIAL_TEACHER);
    SetTrainingCharacterYaw(KNW0_KNK, 16384);
    SetMotionRegionFlag(scenFindMotionwork(KNW0_KNK), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(KNW0_KNK), 0x1902, MotionRegion::LowerBody);
    Knw0Complete(sequence);
}
void Knw0RootSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Knw0Work &work = *static_cast<Knw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            work.lcdSequence = nullptr;
            knw0AdviceShown = 0;
            Knw0Advance(sequence, 103);
            break;
        case 103:
            if (!work.lcdSequence)
                work.lcdSequence = Knw0Child(sequence, CompiledLcdCountdownCallback, SHEN_NAME(DLCD), 1);
            Knw0Child(sequence, Knw0StartPractice, SHEN_NAME(S000), 0);
            sequence.resultSlot = 0;
            Knw0Wait(sequence, 8, work.entryPoint == 1 ? 120 : 106);
            break;
        case 106:
            Knw0Child(sequence, Knw0Introduction, SHEN_NAME(S010), 0);
            sequence.resultSlot = 0;
            Knw0Wait(sequence, 8, 107);
            break;
        case 107:
            Knw0Advance(sequence, Knw0Value<std::int32_t>(data, 0xb8) == 62 ? 112 : 109);
            break;
        case 109:
            Knw0Child(sequence, Knw0FirstLesson, SHEN_NAME(S020), 0);
            sequence.resultSlot = 0;
            Knw0Wait(sequence, 8, 124);
            break;
        case 112:
            Knw0Child(sequence, Knw0SecondLesson, SHEN_NAME(S030), 0);
            sequence.resultSlot = 0;
            Knw0Wait(sequence, 8, 120);
            break;
        case 115:
            break;
        case 119:
            Knw0Advance(sequence, 120);
            break;
        case 120: {
            CompiledSequenceWork *child = Knw0Child(sequence, Knw0Decision, SHEN_NAME(WDMA), 0);
            const std::int32_t arguments[]{0, 2, -1, 120, 120, 120, 119, 124};
            for (std::size_t i = 0; i < 8; ++i)
                Knw0Value(child, 0x138 + 8 * i, arguments[i]);
            sequence.resultSlot = 0;
            sequence.mode = 9;
            sequence.continueAfterWait = 1;
            break;
        }
        case 124:
            Knw0Child(sequence, Knw0FinishPractice, SHEN_NAME(S100), 0);
            sequence.resultSlot = 0;
            Knw0Wait(sequence, 8, 126);
            break;
        case 126:
            Knw0Child(sequence, Knw0AwardMove, SHEN_NAME(S200), 0);
            sequence.resultSlot = 0;
            Knw0Wait(sequence, 8, 127);
            break;
        case 127:
            work.active = 0;
            if (work.lcdSequence) {
                Knw0KillSequence(sequence.children[1]);
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
void Knw0StartPractice(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Knw0Work &work = *static_cast<Knw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Knw0Advance(sequence, 100);
            break;
        case 100:
            Knw0Value(&work, 0x558, std::uint64_t{0});
            Knw0Value(&work, 0x560, std::uint32_t{0});
            Knw0Value(&work, 0x65c, std::uint8_t{0});
            work.learnedMove = 0;
            Knw0Value(&work, 0x5a8, std::uint64_t{0});
            work.elapsed = 0;
            work.combinedPracticeEntries = 0;
            work.demonstrationCamera = 0;
            work.turning = 0;
            for (std::size_t i = 0; i < 10; ++i) {
                work.firstFailures[i] = 0;
                work.repeatedFailures[i] = 0;
            }
            Knw0Child(sequence, Knw0InitializeActors, SHEN_NAME(IWDE), 0);
            sequence.resultSlot = 0;
            Knw0Wait(sequence, 8, 105);
            break;
        case 105:
            Knw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Knw0Introduction(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Knw0Work &work = *static_cast<Knw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Knw0Advance(sequence, 100);
            break;
        case 100:
            Knw0QueueAuthor(SHEN_NAME(AU_0));
            Knw0Wait(sequence, 13, 101, 5, 0);
            break;
        case 101:
            Knw0Wait(sequence, 2, 102, 2);
            return;
        case 102:
            Knw0Wait(sequence, 11, 103, 2);
            return;
        case 103:
            Knw0Wait(sequence, 2, 104, 60);
            return;
        case 104:
            Knw0RestoreActors();
            Knw0QueueAuthor(SHEN_NAME(AU_1));
            Knw0Wait(sequence, 13, 105, 5, 0);
            break;
        case 105:
            Knw0RestoreActors();
            work.camera = 9001;
            EVS_LoopCamera(9001, shen_name{}, shen_name{});
            EV_AT_SetPos(KNW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, KNW0_INITIAL_PLAYER);
            SetTrainingCharacterYaw(KNW0_RYO, 49152);
            EV_AT_SetPos(KNW0_KNK, EventTransformFlags::World | EventTransformFlags::AllAxes, KNW0_INITIAL_TEACHER);
            SetTrainingCharacterYaw(KNW0_KNK, 16384);
            SetMotionRegionFlag(scenFindMotionwork(KNW0_RYO), 71, 0, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(KNW0_KNK), 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(KNW0_KNK), 0x1903, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(KNW0_RYO), 0xf003, MotionRegion::LowerBody);
            Knw0Advance(sequence, 121);
            break;
        case 121: {
            QueueTrainingCameraLookAt(&sequence, 9001, SHEN_NAME(BOX_));
            std::int16_t choices[]{-1, 62, 60, -1};
            SetupControlSpeakAndUI(choices, 450, 0, 2);
            Knw0Wait(sequence, 6, 122);
            break;
        }
        case 122:
            Knw0KillSequence(sequence.children[3]);
            sequence.reserved88 = sequence.choice;
            sequence.reserved1a0 = 1;
            Knw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
namespace {
void Knw0Lesson(void *data, shen_name author, std::uint16_t finish, bool skipAward) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Knw0Work &work = *static_cast<Knw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Knw0Advance(sequence, 100);
            break;
        case 100:
            Knw0QueueAuthor(author);
            Knw0Wait(sequence, 13, 101, 5, 0);
            break;
        case 101:
            Knw0RestoreActors();
            Knw0Wait(sequence, 2, finish, 2);
            break;
        default:
            if (sequence.mode == finish) {
                if (skipAward)
                    work.learnedMove = -1;
                Knw0Complete(sequence);
                return;
            }
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace
void Knw0FirstLesson(void *data) {
    Knw0Lesson(data, SHEN_NAME(AU_2), 107, true);
}
void Knw0SecondLesson(void *data) {
    Knw0Lesson(data, SHEN_NAME(AU_3), 106, false);
}
void Knw0FinishPractice(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        if (!sequence.mode)
            Knw0Advance(sequence, 100);
        else if (sequence.mode == 100) {
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNW0_RYO), nullptr, 0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNW0_KNK), nullptr, 0);
            ProcessAicaCommand(0xaa0, 30);
            Knw0Complete(sequence);
            return;
        } else
            UpdateCompiledSequenceWait(&sequence);
    } while (sequence.continueLoop);
}
void Knw0AwardMove(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Knw0Work &work = *static_cast<Knw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Knw0Advance(sequence, 100);
            break;
        case 100:
            Knw0Wait(sequence, 11, 101, 2);
            break;
        case 101:
            if (work.learnedMove == -1)
                Knw0Advance(sequence, 103);
            else {
                if (work.learnedMove == 2 || work.learnedMove == 0) {
                    SetMoveProficiency(0, 1, 22, work.learnedMove == 2 ? 12 : 1);
                    SetNewEwzoTypeForMoveLearned(work.learnedMove == 2 ? 0 : 2, 1, 22);
                }
                work.resultFrames = 150;
                Knw0Advance(sequence, 102);
            }
            break;
        case 102:
            --work.resultFrames;
            if (GetCurrentControlInput(1) == 512 || !work.resultFrames) {
                SetNewEwzoTypeForMoveLearned(1, 0, 0);
                Knw0Advance(sequence, 103);
            }
            break;
        case 103:
            Knw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
