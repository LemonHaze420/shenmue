#include "s2/areas/area_syw0.hpp"
#include "s2/areas/syw0_tables.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pause.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
#include "s2/flag.hpp"
namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name SYW0_ATTACK{SHEN_NAME(BATT)};
void Syw0PlaceActor(shen_name name, const Vector3 &position, std::int32_t yaw) {
    EV_AT_SetPos(name, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    const Vector3i rotation{.x = 0, .y = yaw, .z = 0};
    EV_AT_SetAngY(name, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
}
} // namespace
void Syw0ResetPracticeActors(std::int32_t practice) {
    if (practice >= 0 && (practice <= 1 || practice == 2 || practice == 9)) {
        Syw0PlaceActor(SYW0_RYO, practice <= 1 ? SYW0_PLAYER_WEIGHT_POSITION : SYW0_PLAYER_FORCE_POSITION, practice <= 1 ? 49152 : trainingPracticeTargetYaw);
        SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 71, 0, MotionRegion::LowerBody);
        const shen_name opponent = practice <= 1 ? SYW0_DEKU : SYW0_KNK;
        Syw0PlaceActor(opponent, practice <= 1 ? SYW0_DUMMY_WEIGHT_POSITION : SYW0_OPPONENT_FORCE_POSITION, practice <= 1 ? 16384 : 32768);
        SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    }
    if (GetMotionAnimationId(scenFindMotionwork(SYW0_RYO), MotionRegion::LowerBody) != 306) {
        SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 71, 0, MotionRegion::LowerBody);
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_RYO), nullptr, 0);
        mtwkRequestMotion(scenFindMotionwork(SYW0_RYO), 306, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 71, 0, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 69, 0, MotionRegion::LowerBody);
    }
    EV_mtwkSetEnabled(scenFindMotionwork(SYW0_RYO), 1);
    SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 69, 0, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(SYW0_KNK), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(SYW0_KNK), 3686, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(SYW0_KNK), 69, 0, MotionRegion::LowerBody);
}
void Syw0PracticeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    void *event = sequence.event;
    const std::int32_t instruction = Syw0Value<std::int32_t>(data, 0x138), second = Syw0Value<std::int32_t>(data, 0x140);
    const std::int32_t practice = Syw0Value<std::int32_t>(data, 0x148);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0: {
            Syw0Value(event, 0x650, std::uint8_t{0});
            UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_RYO), nullptr, 0);
            for (shen_name name : {SYW0_RYO, SYW0_KNK, SYW0_DEKU}) {
                EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
            }
            EV_mtwkSetEnabled(scenFindMotionwork(SYW0_RYO), 1);
            EV_mtwkSetEnabled(scenFindMotionwork(SYW0_KNK), 1);
            EV_mtwkSetEnabled(scenFindMotionwork(Syw0Value<shen_name>(event, 0x644)), 1);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 71, 0, MotionRegion::LowerBody);
            if (practice == 2) {
                Syw0PlaceActor(SYW0_RYO, SYW0_PLAYER_FORCE_POSITION, trainingPracticeTargetYaw);
                Syw0PlaceActor(SYW0_KNK, SYW0_OPPONENT_FORCE_POSITION, 32768);
                Syw0PlaceActor(SYW0_DEKU, SYW0_DUMMY_FORCE_POSITION, 32768);
            } else {
                Syw0PlaceActor(SYW0_RYO, SYW0_PLAYER_WEIGHT_POSITION, 49152);
                Syw0PlaceActor(SYW0_KNK, SYW0_OPPONENT_WEIGHT_POSITION, 16384);
                Syw0PlaceActor(SYW0_DEKU, SYW0_DUMMY_WEIGHT_POSITION, 16384);
            }
            SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 71, 0, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_KNK), 71, 0, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_DEKU), 71, 0, MotionRegion::LowerBody);
            if (static_cast<std::uint32_t>(practice) <= 1 || practice == 2) {
                SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 69, practice == 2 ? 2 : 0, MotionRegion::LowerBody);
                SetMotionRegionFlag(scenFindMotionwork(SYW0_KNK), 69, 0, MotionRegion::LowerBody);
            }
            mtwkRequestMotion(scenFindMotionwork(SYW0_RYO), 306, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(SYW0_KNK), 2325, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(SYW0_DEKU), 306, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_DEKU), 69, 2, MotionRegion::LowerBody);
            FinishLoading();
            CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
            Syw0Wait(sequence, 2, 100, 10);
            break;
        }
        case 100:
            Syw0Wait(sequence, 2, 101, 1);
            return;
        case 101: {
            vfSetMoveInstructionConfiguration(
                SYW0_RYO, SYW0_KNK, static_cast<std::int16_t>(instruction), static_cast<std::int16_t>(second), static_cast<std::byte *>(event) + 0x78);
            Vector3 target;
            EV_GetCharaPosition(SYW0_RYO, 5, &target, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_SYE), &target, 0);
            CompiledSequenceWork *child = Syw0Child(sequence, TrainingAttackSequence, SYW0_ATTACK, 0);
            Syw0Value(child, 0x138, SYW0_RYO);
            Syw0Value(child, 0x140, practice == 2 ? SYW0_KNK : SYW0_DEKU);
            Syw0Value(child, 0x148, std::int32_t(practice == 2 ? 1 : 0));
            Syw0Value(child, 0x150, std::int32_t(practice == 2 ? 33 : 1));
            Syw0Value(child, 0x158, static_cast<const void *>("SYW0"));
            Syw0Wait(sequence, 2, 102, 10);
            break;
        }
        case 102:
            if (Syw0Value<std::uint8_t>(event, 0x78) != 2)
                return;
            Syw0Value(event, 0x640, std::int32_t{1});
            SuspendFight();
            mtwkSetRequestedRootMode(scenFindMotionwork(SYW0_RYO), 1);
            Syw0Advance(sequence, 104);
            break;
        case 103:
            if (!IsFightActive()) {
                Syw0Advance(sequence, 104);
                break;
            }
            Syw0Value(event, 0x640, std::int32_t{1});
            SuspendFight();
            mtwkSetRequestedRootMode(scenFindMotionwork(SYW0_RYO), 1);
            break;
        case 104:
            SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 70, 1, MotionRegion::LowerBody);
            sequence.waitChildIndex = 0;
            Syw0Wait(sequence, 10, 105);
            break;
        case 105:
            nullsub();
            CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
            for (shen_name name : {SYW0_RYO, Syw0Value<shen_name>(event, 0x644)}) {
                EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
            }
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
