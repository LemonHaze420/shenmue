#include "s2/areas/bkw0_sequence.hpp"
#include "s2/areas/bkw0_tables.hpp"
#include "s2/areas/area_arbt.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pause.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
inline constexpr std::uint32_t BKW0_FIGHT_PACKAGE[]{SHEN_NAME(BKW0), 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
void Bkw0PracticeNullsub() {
}
} // namespace
void Bkw0InitializeTrainingSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    const Vector4 reflector{1, .5f, .5f, .5f};
    EV_DisableCharacterDisplay(BKW0_RYO);
    EV_EnableCharacterDisplay(BKW0_LKN);
    EV_DisableCharacterDisplay(BKW0_DEKU);
    EV_AT_SetPos(BKW0_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, bkw0BasePosition);
    EV_AT_SetAngY(BKW0_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, bkw0BaseRotation);
    EV_SetCharacterAlphaEnabled(BKW0_RYO, false);
    EV_SetCharacterAlphaEnabled(BKW0_LKN, false);
    for (shen_name character : {BKW0_RYO, BKW0_LKN, BKW0_DEKU, BKW0_BKN}) {
        EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
    }
    EV_SetCharaControllerFlags(BKW0_RYO, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(BKW0_LKN, 2, NpcCharacterControllerState::Disabled);
    for (shen_name character : {BKW0_RYO, BKW0_LKN, BKW0_BKN})
        SetScenarioCharacterDrawControl(character, 1, 1);
    EV_mtwkSetEnabled(scenFindMotionwork(BKW0_RYO), 1);
    EV_mtwkSetEnabled(scenFindMotionwork(BKW0_LKN), 0);
    EV_mtwkSetEnabled(scenFindMotionwork(BKW0_BKN), 1);
    for (shen_name character : {BKW0_RYO, BKW0_LKN, BKW0_BKN})
        SetMotionSpeed(scenFindMotionwork(character), 1, MotionRegion::LowerBody);
    for (shen_name character : {BKW0_RYO, BKW0_LKN, BKW0_BKN})
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 1);
    for (shen_name character : {BKW0_RYO, BKW0_LKN, BKW0_BKN})
        SetScenarioReflectorBoardValues(character, &reflector);
    for (shen_name character : {BKW0_RYO, BKW0_LKN, BKW0_BKN})
        EV_SetRefBoardParamsFor(character, 1);
    const Vector3i ryoRotation{0, 32768, 0};
    EV_AT_SetPos(BKW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_RYO_INITIAL_POSITION);
    EV_AT_SetAngY(BKW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, ryoRotation);
    SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(BKW0_LKN, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_LKN_POSITION);
    const Vector3i lknRotation{0, arbtCameraRotation.yaw, 0};
    EV_AT_SetAngY(BKW0_LKN, EventTransformFlags::World | EventTransformFlags::AllAxes, lknRotation);
    SetMotionRegionFlag(scenFindMotionwork(BKW0_LKN), 71, 0, MotionRegion::LowerBody);
    GetNpcBatchCountOverride(30);
    Bkw0PlaceCrowd();
    Bkw0TrainCrowd(0, 1);
    Bkw0Complete(sequence);
}
void Bkw0ResetActors(std::int32_t practice) {
    if (!(practice & ~9) && practice != 8) {
        EV_AT_SetPos(BKW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_RYO_POSITION);
        const Vector3i rotation{0, 32768, 0};
        EV_AT_SetAngY(BKW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 71, 0, MotionRegion::LowerBody);
    }
    if (GetMotionAnimationId(scenFindMotionwork(BKW0_RYO), MotionRegion::LowerBody) != 306) {
        SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 71, 0, MotionRegion::LowerBody);
        UpdateMotionWorkWithPosition(scenFindMotionwork(BKW0_RYO), nullptr, 0);
        mtwkRequestMotion(scenFindMotionwork(BKW0_RYO), 306, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 71, 0, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 69, 0, MotionRegion::LowerBody);
    }
    EV_mtwkSetEnabled(scenFindMotionwork(BKW0_RYO), 1);
    SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 69, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(BKW0_LKN, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_LKN_POSITION);
    Bkw0ResetPracticeOpponentAngle();
    SetMotionRegionFlag(scenFindMotionwork(BKW0_LKN), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(BKW0_LKN), 3686, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(BKW0_LKN), 69, 0, MotionRegion::LowerBody);
}
void Bkw0PracticeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    const std::int32_t instruction = Bkw0SequenceValue<std::int32_t>(sequence, 0x138), second = Bkw0SequenceValue<std::int32_t>(sequence, 0x140);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0: {
            work.practiceReady = 0;
            UpdateMotionWorkWithPosition(scenFindMotionwork(BKW0_RYO), nullptr, 0);
            for (shen_name character : {BKW0_RYO, work.opponent}) {
                EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
            }
            EV_mtwkSetEnabled(scenFindMotionwork(BKW0_RYO), 1);
            EV_mtwkSetEnabled(scenFindMotionwork(BKW0_LKN), 1);
            EV_mtwkSetEnabled(scenFindMotionwork(work.opponent), 1);
            SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 71, 0, MotionRegion::LowerBody);
            EV_AT_SetPos(BKW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_RYO_POSITION);
            const Vector3i rotation{0, 32768, 0};
            EV_AT_SetAngY(BKW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 71, 0, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 69, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(BKW0_RYO), 306, MotionRegion::LowerBody);
            EV_AT_SetPos(BKW0_LKN, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_LKN_POSITION);
            Bkw0ResetPracticeOpponentAngle();
            SetMotionRegionFlag(scenFindMotionwork(BKW0_LKN), 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(BKW0_LKN), 3686, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(BKW0_LKN), 69, 2, MotionRegion::LowerBody);
            if (GetMotionAnimationId(scenFindMotionwork(BKW0_BKN), MotionRegion::LowerBody) == 7942)
                mtwkRequestMotion(scenFindMotionwork(BKW0_BKN), 7939, MotionRegion::LowerBody);
            FinishLoading();
            CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
            Bkw0Advance(sequence, 100);
            break;
        }
        case 100:
            Bkw0Wait(sequence, 2, 101, 1);
            break;
        case 101: {
            EV_AT_SetPos(work.opponent, EventTransformFlags::AllAxes, BKW0_LKN_POSITION);
            vfSetMoveInstructionConfiguration(BKW0_RYO, BKW0_LKN, static_cast<std::int16_t>(instruction), static_cast<std::int16_t>(second), work.fightResults);
            Vector3 position;
            EV_GetCharaPosition(BKW0_RYO, 5, &position, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(BKW0_BKN), &position, 0);
            CompiledSequenceWork *look = Bkw0Child(sequence, Bkw0LookSequence, BKW0_LOOK, 1);
            Bkw0SequenceValue(*look, 0x138, BKW0_BKN);
            Bkw0SequenceValue(*look, 0x140, BKW0_RYO);
            CompiledSequenceWork *attack = Bkw0Child(sequence, TrainingAttackSequence, BKW0_ATTACK, 0);
            Bkw0SequenceValue(*attack, 0x138, BKW0_RYO);
            Bkw0SequenceValue(*attack, 0x140, BKW0_LKN);
            Bkw0SequenceValue(*attack, 0x148, std::int32_t{0});
            Bkw0SequenceValue(*attack, 0x150, std::int32_t{32});
            Bkw0SequenceValue(*attack, 0x158, static_cast<const void *>(BKW0_FIGHT_PACKAGE));
            Bkw0Wait(sequence, 2, 102, 10);
            break;
        }
        case 102:
            if (work.fightResults[0] != std::byte{2})
                return;
            work.fightSuspended = 1;
            SuspendFight();
            mtwkSetRequestedRootMode(scenFindMotionwork(BKW0_RYO), 1);
            Bkw0Advance(sequence, 104);
            break;
        case 103:
            if (IsFightActive())
                Bkw0Advance(sequence, 104);
            else {
                work.fightSuspended = 1;
                SuspendFight();
                mtwkSetRequestedRootMode(scenFindMotionwork(BKW0_RYO), 1);
            }
            break;
        case 104:
            sequence.waitChildIndex = 0;
            Bkw0Wait(sequence, 10, 105);
            break;
        case 105:
            Bkw0PracticeNullsub();
            CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
            for (shen_name character : {BKW0_RYO, work.opponent}) {
                EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
            }
            Bkw0KillSequence(sequence.children[1]);
            Bkw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
