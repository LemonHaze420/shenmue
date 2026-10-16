#include "s2/areas/area_mfw0.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/hand.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pause.hpp"
#include "s2/sound_program.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
#include <cstdlib>
namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name MFW0_ATTACK{SHEN_NAME(BATT)};
void Mfw0NeutralHands() {
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MFW0_RYO), 0, reinterpret_cast<std::byte *>(neutralHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MFW0_RYO), 1, reinterpret_cast<std::byte *>(neutralHandPoseCommand), 0);
}
void Mfw0Place(shen_name character, const Vector3 &position, std::int32_t yaw) {
    EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    SetTrainingCharacterYaw(character, yaw);
}
void Mfw0RotateSampled(shen_name character, std::int32_t delta) {
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(character, &rotation, EventTransformFlags::MotionYaw);
    SetTrainingCharacterYaw(character, static_cast<std::uint16_t>(rotation.y + delta));
}
void Mfw0Demonstration(void *data, std::int32_t kind) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0NeutralHands();
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const std::uint16_t camera = kind == 0 ? 9005 : kind == 1 ? 9006 : static_cast<std::uint16_t>(Mfw0Value<std::int64_t>(data, 0x138));
            Mfw0Camera(sequence, camera);
            Mfw0Place(MFW0_RYO, MFW0_RYO_POSITION, 8192);
            if (kind != 2)
                Mfw0Place(MFW0_TKR, MFW0_TEACHER_POSITION, 41264);
            EV_DisableCharacterDisplay(MFW0_TKR);
            Mfw0Advance(sequence, 101);
            break;
        }
        case 101:
            figpSetFlagsBlendForBlur(MFW0_RYO, 8, 2);
            figpAssignModeFor(MFW0_RYO, 4);
            SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 71, 0, MotionRegion::LowerBody);
            EV_mtwkSetEnabled(scenFindMotionwork(MFW0_RYO), 1);
            mtwkRequestMotion(scenFindMotionwork(MFW0_RYO), kind == 1 ? 5022 : 5016, MotionRegion::LowerBody);
            SetMotionLayerFrameRange(scenFindMotionwork(MFW0_RYO), -1,
                kind == 1     ? 90 :
                    kind == 2 ? 26 :
                                10,
                kind == 1     ? 136 :
                    kind == 2 ? 38 :
                                40,
                MotionRegion::LowerBody);
            SetMotionSpeed(scenFindMotionwork(MFW0_RYO), kind == 1 ? 1 : .69999999f, MotionRegion::LowerBody);
            if (kind == 1)
                SetMotionSpeed(scenFindMotionwork(MFW0_RYO), 1, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 69, 0, MotionRegion::LowerBody);
            if (kind == 1)
                Mfw0Wait(sequence, 2, 103, 49);
            else {
                sequence.waitCharacter = MFW0_RYO;
                Mfw0Wait(sequence, 5, 102);
            }
            break;
        case 102:
            if (kind == 1) {
                UpdateCompiledSequenceWait(&sequence);
                break;
            }
            SetMotionSpeed(scenFindMotionwork(MFW0_RYO), 1, MotionRegion::LowerBody);
            figpAssignModeFor(MFW0_RYO, 0);
            Mfw0Complete(sequence);
            return;
        case 103:
            if (kind != 1) {
                UpdateCompiledSequenceWait(&sequence);
                break;
            }
            if (sequence.parent)
                Mfw0Value(sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
            Mfw0Wait(sequence, 2, 104, 1);
            break;
        case 104:
            if (kind != 1) {
                UpdateCompiledSequenceWait(&sequence);
                break;
            }
            ProcessAicaCommand(0x1200a0, 0);
            QueueExtraTelop("HHHHHHHHHHHH", 2);
            SetMotionSpeed(scenFindMotionwork(MFW0_RYO), 1, MotionRegion::LowerBody);
            figpAssignModeFor(MFW0_RYO, 0);
            sequence.previousMode = sequence.mode;
            sequence.mode = 105;
            sequence.parent = nullptr;
            return;
        case 105:
            if (kind != 1) {
                UpdateCompiledSequenceWait(&sequence);
                break;
            }
            EV_LoadAudio("0050A000", 0);
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace
void Mfw0InitializeActorsSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    EV_DisableCharacterDisplay(MFW0_RYO);
    EV_EnableCharacterDisplay(MFW0_TKR);
    EV_DisableCharacterDisplay(MFW0_DEKU);
    EV_SetCharacterAlphaEnabled(MFW0_RYO, false);
    EV_SetCharacterAlphaEnabled(MFW0_TKR, false);
    hmdlSetFlagsFor(FindCharacterByScenarioName(MFW0_RYO), 0, 0, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(MFW0_RYO), 1, 0, 16);
    SetHighModelXYZ(FindCharacterByScenarioName(MFW0_RYO), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MFW0_RYO), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MFW0_RYO), 1, reinterpret_cast<std::byte *>(neutralHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MFW0_RYO), 0, reinterpret_cast<std::byte *>(neutralHandPoseCommand), 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MFW0_TKR), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(MFW0_TKR), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MFW0_TKR), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MFW0_TKR), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    for (shen_name name : {MFW0_RYO, MFW0_TKR, MFW0_DEKU}) {
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
    }
    EV_SetCharaControllerFlags(MFW0_RYO, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(MFW0_TKR, 2, NpcCharacterControllerState::Disabled);
    SetScenarioCharacterDrawControl(MFW0_RYO, 1, 1);
    SetScenarioCharacterDrawControl(MFW0_TKR, 1, 1);
    EV_mtwkSetEnabled(scenFindMotionwork(MFW0_RYO), 1);
    EV_mtwkSetEnabled(scenFindMotionwork(MFW0_TKR), 0);
    SetMotionSpeed(scenFindMotionwork(MFW0_RYO), 1, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(MFW0_TKR), 1, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(MFW0_RYO), 1);
    mtwkSetRequestedRootMode(scenFindMotionwork(MFW0_TKR), 1);
    SetMotionFrameTime(scenFindMotionwork(MFW0_RYO), 8, MotionRegion::LowerBody);
    SetMotionFrameTime(scenFindMotionwork(MFW0_TKR), 8, MotionRegion::LowerBody);
    SetScenarioReflectorBoardValues(MFW0_RYO, &MFW0_REFLECTOR);
    SetScenarioReflectorBoardValues(MFW0_TKR, &MFW0_REFLECTOR);
    EV_SetRefBoardParamsFor(MFW0_RYO, 1);
    EV_SetRefBoardParamsFor(MFW0_TKR, 1);
    Mfw0Place(MFW0_RYO, MFW0_INITIAL_RYO_POSITION, 8192);
    SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 71, 0, MotionRegion::LowerBody);
    Mfw0Place(MFW0_TKR, MFW0_INITIAL_TEACHER_POSITION, 16384);
    SetMotionRegionFlag(scenFindMotionwork(MFW0_TKR), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(MFW0_TKR), 4999, MotionRegion::LowerBody);
    Mfw0Complete(sequence);
}
void Mfw0DemonstrationSequence(void *data) {
    Mfw0Demonstration(data, 0);
}
void Mfw0SlowDemonstrationSequence(void *data) {
    Mfw0Demonstration(data, 1);
}
void Mfw0RepeatedDemonstrationSequence(void *data) {
    Mfw0Demonstration(data, 2);
}
void Mfw0ResetPlayer(std::int32_t kind) {
    if (kind == 1) {
        Mfw0RotateSampled(MFW0_RYO, 0);
        mtwkRequestMotion(scenFindMotionwork(MFW0_RYO), 0xf003, MotionRegion::LowerBody);
    } else if (kind == 0) {
        Mfw0Place(MFW0_RYO, MFW0_RYO_POSITION, 8192);
        Mfw0RotateSampled(MFW0_RYO, 0);
        SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 71, 0, MotionRegion::LowerBody);
        mtwkRequestMotion(scenFindMotionwork(MFW0_RYO), 0xf003, MotionRegion::LowerBody);
        UpdateMotionWorkWithPosition(scenFindMotionwork(MFW0_RYO), nullptr, 3);
    }
}
void Mfw0ResetTeacher() {
    Mfw0Place(MFW0_TKR, MFW0_TEACHER_POSITION, 41264);
    Mfw0RotateSampled(MFW0_TKR, 10923);
    SetMotionRegionFlag(scenFindMotionwork(MFW0_TKR), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(MFW0_TKR), 5003, MotionRegion::LowerBody);
    UpdateMotionWorkWithPosition(scenFindMotionwork(MFW0_TKR), nullptr, 3);
}
void Mfw0ResetActors(std::int32_t practice) {
    if (!(practice & ~9) && practice != 8) {
        Mfw0Place(MFW0_RYO, MFW0_RYO_POSITION, 8192);
        SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 71, 0, MotionRegion::LowerBody);
    }
    if (GetMotionAnimationId(scenFindMotionwork(MFW0_RYO), MotionRegion::LowerBody) != 306) {
        SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 71, 0, MotionRegion::LowerBody);
        UpdateMotionWorkWithPosition(scenFindMotionwork(MFW0_RYO), nullptr, 0);
        mtwkRequestMotion(scenFindMotionwork(MFW0_RYO), 306, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 71, 0, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 69, 0, MotionRegion::LowerBody);
    }
    EV_mtwkSetEnabled(scenFindMotionwork(MFW0_RYO), 1);
    SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 69, 0, MotionRegion::LowerBody);
    Mfw0Place(MFW0_TKR, MFW0_TEACHER_POSITION, 41264);
    Mfw0RotateSampled(MFW0_TKR, 10923);
    SetMotionRegionFlag(scenFindMotionwork(MFW0_TKR), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(MFW0_TKR), 5003, MotionRegion::LowerBody);
}
bool Mfw0NeedsFacingReset(std::int32_t practice) {
    const std::int32_t wanted = !(practice & ~9) && practice != 8 ? 8192 : 0;
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(MFW0_RYO, &rotation, EventTransformFlags::World);
    const std::uint16_t forward = static_cast<std::uint16_t>(wanted - rotation.y), reverse = static_cast<std::uint16_t>(rotation.y - wanted);
    const std::int32_t difference = forward >= reverse ? reverse : wanted > rotation.y ? rotation.y - wanted : -static_cast<std::int32_t>(forward);
    return std::abs(difference) > 10922;
}
void Mfw0PracticeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const std::int64_t instruction = Mfw0Value<std::int64_t>(data, 0x138), second = Mfw0Value<std::int64_t>(data, 0x140);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            work.practiceReady = 0;
            UpdateMotionWorkWithPosition(scenFindMotionwork(MFW0_RYO), nullptr, 0);
            for (shen_name name : {MFW0_RYO, MFW0_DEKU}) {
                EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Enabled);
                EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Enabled);
            }
            EV_mtwkSetEnabled(scenFindMotionwork(MFW0_RYO), 1);
            EV_mtwkSetEnabled(scenFindMotionwork(MFW0_TKR), 1);
            EV_mtwkSetEnabled(scenFindMotionwork(MFW0_DEKU), 1);
            SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 71, 0, MotionRegion::LowerBody);
            FinishLoading();
            CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Wait(sequence, 2, 101, 1);
            break;
        case 101: {
            EV_AT_SetPos(MFW0_DEKU, EventTransformFlags::AllAxes, MFW0_TEACHER_POSITION);
            vfSetMoveInstructionConfiguration(
                MFW0_RYO, MFW0_TKR, static_cast<std::int16_t>(instruction), static_cast<std::int16_t>(second), reinterpret_cast<std::byte *>(&work.results[0]));
            Vector3 position;
            EV_GetCharaPosition(MFW0_RYO, 5, &position, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(MFW0_TKR), &position, 0);
            CompiledSequenceWork *attack = Mfw0Child(sequence, TrainingAttackSequence, MFW0_ATTACK, 0);
            Mfw0Value(attack, 0x138, std::int32_t{static_cast<std::int32_t>(MFW0_RYO.name)});
            Mfw0Value(attack, 0x140, std::int32_t{static_cast<std::int32_t>(MFW0_DEKU.name)});
            Mfw0Value(attack, 0x148, std::int32_t{0});
            Mfw0Value(attack, 0x150, std::int32_t{1});
            Mfw0Value(attack, 0x158, static_cast<const void *>("MFW0"));
            Mfw0Wait(sequence, 2, 102, 10);
            break;
        }
        case 102:
            if (work.results[0].status != 2)
                return;
            work.fightSuspended = 1;
            SuspendFight();
            mtwkSetRequestedRootMode(scenFindMotionwork(MFW0_RYO), 1);
            Mfw0Advance(sequence, 104);
            break;
        case 103:
            if (!IsFightActive()) {
                Mfw0Advance(sequence, 104);
                break;
            }
            work.fightSuspended = 1;
            SuspendFight();
            mtwkSetRequestedRootMode(scenFindMotionwork(MFW0_RYO), 1);
            break;
        case 104:
            sequence.waitChildIndex = 0;
            Mfw0Wait(sequence, 10, 105);
            break;
        case 105:
            Mfw0Nullsub();
            CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
            for (shen_name name : {MFW0_RYO, MFW0_DEKU}) {
                EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
            }
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
