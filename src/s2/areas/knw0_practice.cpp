#include "s2/areas/area_knw0.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pause.hpp"
#include "s2/save.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
const std::uint8_t knw0MoveLcd[194]{0x06, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x03, 0x87, 0x0f, 0x03, 0xf0, 0x06, 0x02, 0x49, 0x09, 0x02, 0x10, 0x05, 0x02,
    0x49, 0x09, 0x04, 0x08, 0x04, 0x81, 0x32, 0x09, 0x04, 0xc8, 0x3c, 0x41, 0x32, 0x79, 0xe9, 0x24, 0x20, 0x20, 0x84, 0x40, 0x29, 0x24, 0x20, 0x10, 0x84, 0x40,
    0x29, 0xe4, 0x20, 0x20, 0x84, 0x40, 0x28, 0x04, 0x3c, 0x41, 0x32, 0x79, 0xe8, 0x04, 0x04, 0x81, 0x32, 0x09, 0x09, 0xe4, 0x05, 0x02, 0x49, 0x09, 0x09, 0x24,
    0x06, 0x02, 0x49, 0x09, 0x09, 0x24, 0x04, 0x03, 0x87, 0x0f, 0x0f, 0x3c};
void Knw0CaptureNoop() {
}
} // namespace
void Knw0ShowLcd(Knw0Work &work) {
    work.lcdBitmap = knw0MoveLcd;
    work.lcdFrames = 600;
    LcdmSelectMessage(1);
    LcdmInit(reinterpret_cast<std::uintptr_t>(work.lcdBitmap));
}
bool Knw0NeedsFacingReset(std::int32_t practice) {
    return TrainingNeedsFacingReset(practice);
}
void Knw0ResetActors(std::int32_t practice) {
    if (!(practice & ~9) && practice != 8) {
        EV_AT_SetPos(KNW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, KNW0_PLAYER_POSITION);
        SetTrainingCharacterYaw(KNW0_RYO, 32768);
        SetMotionRegionFlag(scenFindMotionwork(KNW0_RYO), 71, 0, MotionRegion::LowerBody);
    }
    if (GetMotionAnimationId(scenFindMotionwork(KNW0_RYO), MotionRegion::LowerBody) != 306) {
        SetMotionRegionFlag(scenFindMotionwork(KNW0_RYO), 71, 0, MotionRegion::LowerBody);
        UpdateMotionWorkWithPosition(scenFindMotionwork(KNW0_RYO), nullptr, 0);
        mtwkRequestMotion(scenFindMotionwork(KNW0_RYO), 306, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(KNW0_RYO), 71, 0, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(KNW0_RYO), 69, 0, MotionRegion::LowerBody);
    }
    EV_mtwkSetEnabled(scenFindMotionwork(KNW0_RYO), 1);
    SetMotionRegionFlag(scenFindMotionwork(KNW0_RYO), 69, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(KNW0_KNK, EventTransformFlags::World | EventTransformFlags::AllAxes, KNW0_TEACHER_POSITION);
    SetTrainingCharacterYaw(KNW0_KNK, 14563);
    SetMotionRegionFlag(scenFindMotionwork(KNW0_KNK), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(KNW0_KNK), 0x1902, MotionRegion::LowerBody);
}
void Knw0CapturePractice(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Knw0Work &work = *static_cast<Knw0Work *>(sequence.event);
    const std::int64_t instruction = Knw0Value<std::int64_t>(data, 0x138), second = Knw0Value<std::int64_t>(data, 0x140);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            work.practiceReady = 0;
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNW0_RYO), nullptr, 0);
            for (shen_name character : {KNW0_RYO, KNW0_DEKU}) {
                EV_SetCharaControllerFlags(character, NpcCharacterControllerFlags::ApplyGravity, NpcCharacterControllerState::Enabled);
                EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Enabled);
            }
            for (shen_name character : {KNW0_RYO, KNW0_KNK, KNW0_DEKU})
                EV_mtwkSetEnabled(scenFindMotionwork(character), 1);
            SetMotionRegionFlag(scenFindMotionwork(KNW0_RYO), 71, 0, MotionRegion::LowerBody);
            FinishLoading();
            CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
            Knw0Advance(sequence, 100);
            break;
        case 100:
            Knw0Wait(sequence, 2, 101, 1);
            return;
        case 101: {
            const Vector3 dummy{.x = 335.399993896484375f, .y = 127, .z = 270.100006103515625f};
            EV_AT_SetPos(KNW0_DEKU, EventTransformFlags::AllAxes, dummy);
            vfSetMoveInstructionConfiguration(
                KNW0_RYO, KNW0_KNK, static_cast<std::int16_t>(instruction), static_cast<std::int16_t>(second), reinterpret_cast<std::byte *>(work.results));
            Vector3 target;
            EV_GetCharaPosition(KNW0_RYO, 5, &target, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNW0_KNK), &target, 0);
            CompiledSequenceWork *child = Knw0Child(sequence, TrainingAttackSequence, SHEN_NAME(BATT), 0);
            Knw0Value(child, 0x138, KNW0_RYO);
            Knw0Value(child, 0x140, KNW0_DEKU);
            Knw0Value(child, 0x148, std::int32_t{0});
            Knw0Value(child, 0x150, std::int32_t{1});
            Knw0Value(child, 0x158, static_cast<const void *>("KNW0"));
            Knw0Wait(sequence, 2, 102, 10);
            break;
        }
        case 102:
            if (work.results[0].status != 2)
                return;
            work.fightSuspended = 1;
            SuspendFight();
            mtwkSetRequestedRootMode(scenFindMotionwork(KNW0_RYO), 1);
            Knw0Advance(sequence, 104);
            break;
        case 103:
            if (!IsFightActive())
                Knw0Advance(sequence, 104);
            else {
                work.fightSuspended = 1;
                SuspendFight();
                mtwkSetRequestedRootMode(scenFindMotionwork(KNW0_RYO), 1);
            }
            break;
        case 104:
            sequence.waitChildIndex = 0;
            Knw0Wait(sequence, 10, 105);
            return;
        case 105:
            Knw0CaptureNoop();
            CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
            for (shen_name character : {KNW0_RYO, KNW0_DEKU}) {
                EV_SetCharaControllerFlags(character, NpcCharacterControllerFlags::ApplyGravity, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
            }
            Knw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Knw0ExitPractice(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Knw0Work &work = *static_cast<Knw0Work *>(sequence.event);
    const std::int64_t retry = Knw0Value<std::int64_t>(data, 0x140), finish = Knw0Value<std::int64_t>(data, 0x150);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Knw0Advance(sequence, 100);
            break;
        case 100:
            mtwkRequestMotion(scenFindMotionwork(KNW0_KNK), 0x1902, MotionRegion::LowerBody);
            if (work.practiceExpired)
                Knw0Advance(sequence, 101);
            else if (work.practiceWarning)
                Knw0Advance(sequence, 105);
            else {
                sequence.reserved88 = static_cast<std::uint32_t>(retry);
                sequence.reserved1a0 = 1;
                Knw0Advance(sequence, 125);
            }
            break;
        case 101:
            work.conversationActive = 1;
            Knw0QueueAuthor(SHEN_NAME(AU_5));
            Knw0Wait(sequence, 13, 103, 5, 0);
            break;
        case 103:
            Knw0RestoreActors();
            Knw0QueueAuthor(SHEN_NAME(AU_6));
            Knw0Wait(sequence, 13, 104, 5, 0);
            break;
        case 104:
            Knw0RestoreActors();
            sequence.reserved88 = static_cast<std::uint32_t>(finish);
            sequence.reserved1a0 = 1;
            work.practiceExpired = 0;
            Knw0Advance(sequence, 125);
            break;
        case 125:
            Knw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Knw0Success(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    const std::int64_t success = Knw0Value<std::int64_t>(data, 0x150);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Knw0Advance(sequence, 100);
            break;
        case 100:
            Knw0Advance(sequence, 103);
            break;
        case 101:
        case 102:
            return;
        case 103:
            Knw0QueueAuthor(SHEN_NAME(AU_4));
            Knw0Wait(sequence, 13, 133, 5, 0);
            break;
        case 133:
            Knw0RestoreActors();
            sequence.reserved88 = static_cast<std::uint32_t>(success);
            sequence.reserved1a0 = 1;
            Knw0QueueAuthor(SHEN_NAME(AU_6));
            Knw0Wait(sequence, 13, 134, 5, 0);
            break;
        case 134:
            Knw0RestoreActors();
            Knw0Wait(sequence, 2, 142, 2);
            break;
        case 142:
            Knw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
