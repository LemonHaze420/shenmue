#include "s2/training_seqp.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include <cstring>
#include <cstdlib>
namespace shenmue::s2 {
std::int32_t trainingGateTargetYaw{};
std::int32_t trainingPracticeTargetYaw{};
Vector3i trainingSparRotation{};
namespace {
inline constexpr shen_name TRAINING_RYO{SHEN_NAME(RYO_)}, TRAINING_LKN{SHEN_NAME(LKN_)}, TRAINING_DEKU{SHEN_NAME(DEKU)};
inline constexpr shen_name TRAINING_CAMERA{SHEN_NAME(WDCA)}, TRAINING_REPOSITION{SHEN_NAME(BTSP)};
template <class T> T ReadTrainingValue(const void *data, std::size_t offset) {
    T result;
    std::memcpy(&result, static_cast<const std::byte *>(data) + offset, sizeof(result));
    return result;
}
template <class T> void WriteTrainingValue(void *data, std::size_t offset, T value) {
    std::memcpy(static_cast<std::byte *>(data) + offset, &value, sizeof(value));
}
void TrainingAdvance(CompiledSequenceWork &sequence, std::uint16_t mode) {
    sequence.previousMode = sequence.mode;
    sequence.mode = mode;
    sequence.continueAfterWait = 1;
    sequence.continueLoop = 1;
}
void TrainingComplete(CompiledSequenceWork &sequence) {
    if (sequence.parent)
        WriteTrainingValue(sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
    RetireCompiledSequenceLinks(&sequence);
    HLib::DestroyCurrentTask();
}
CompiledSequenceWork *TrainingChild(CompiledSequenceWork &sequence, HLib::TaskCallback callback, shen_name token, std::int32_t slot) {
    CompiledSequenceWork *child = QueueCompiledSequence(callback, token, sequence.event);
    if (!sequence.children[slot])
        sequence.children[slot] = child;
    child->parent = &sequence;
    child->childSlot = slot;
    child->reference = sequence.reference + 1;
    WriteTrainingValue(&sequence, 0x19c, slot);
    WriteTrainingValue(&sequence, 0x90 + 4 * slot, std::int32_t{0});
    return child;
}
std::int32_t TrainingFacingDifference(std::int32_t wanted, std::int32_t current) {
    const std::uint16_t forward = static_cast<std::uint16_t>(wanted - current), reverse = static_cast<std::uint16_t>(current - wanted);
    if (forward >= reverse)
        return reverse;
    return wanted > current ? current - wanted : -static_cast<std::int32_t>(forward);
}
void TrainingTurnStepSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    const shen_name character{static_cast<std::uint32_t>(ReadTrainingValue<std::uint64_t>(data, 0x138))};
    const shen_name target{static_cast<std::uint32_t>(ReadTrainingValue<std::uint64_t>(data, 0x140))};
    const std::uint32_t frameTime = static_cast<std::uint32_t>(ReadTrainingValue<std::uint64_t>(data, 0x148));
    const std::int32_t initialDelay = static_cast<std::int32_t>(ReadTrainingValue<std::uint64_t>(data, 0x150));
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            TrainingAdvance(sequence, 100);
            break;
        case 100:
            sequence.mode = 2;
            sequence.nextMode = 101;
            sequence.continueAfterWait = 1;
            sequence.delay = initialDelay;
            break;
        case 101: {
            Vector3 position;
            EV_GetCharaPosition(target, 5, &position, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(character), &position, 0);
            EV_mtwkSetEnabled(scenFindMotionwork(character), 0);
            SetMotionFrameTime(scenFindMotionwork(character), frameTime, MotionRegion::LowerBody);
            Vector3i sampled;
            GetScenarioCharacterEulerAngles(character, &sampled, EventTransformFlags::MotionYaw);
            const Vector3i rotation{0, static_cast<std::uint16_t>(sampled.y), 0};
            EV_AT_SetAngY(character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            mtwkRequestMotion(scenFindMotionwork(character), 0xf003, MotionRegion::LowerBody);
            sequence.mode = 2;
            sequence.nextMode = 102;
            sequence.continueAfterWait = 1;
            sequence.delay = static_cast<std::int32_t>(frameTime);
            break;
        }
        case 102:
            SetMotionFrameTime(scenFindMotionwork(character), 8, MotionRegion::LowerBody);
            TrainingComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void TrainingLookCameraSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    const std::uint16_t camera = static_cast<std::uint16_t>(ReadTrainingValue<std::uint64_t>(data, 0x138));
    const shen_name character{static_cast<std::uint32_t>(ReadTrainingValue<std::uint64_t>(data, 0x140))};
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            WriteTrainingValue(sequence.event, 0x1c, camera);
            EVS_LoopCamera(camera, character, character);
            sequence.mode = 2;
            sequence.nextMode = 100;
            sequence.continueAfterWait = 1;
            sequence.delay = 2;
            break;
        case 100:
            sequence.mode = 3;
            sequence.nextMode = 101;
            sequence.continueAfterWait = 1;
            break;
        case 101:
            EVS_LoopCamera(ReadTrainingValue<std::uint16_t>(sequence.event, 0x1c), character, character);
            sequence.mode = 2;
            sequence.nextMode = 100;
            sequence.continueAfterWait = 1;
            sequence.delay = 2;
            break;
        case 102:
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
std::uint16_t SelectTrainingStepMotion(shen_name character, const Vector3 *target) {
    Vector3 position;
    EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(character, &rotation, EventTransformFlags::World);
    const std::int16_t angle = InverseFixedAtan2(target->x - position.x, target->z - position.z);
    const float degrees = TrainingFacingDifference(angle, rotation.y) / 182.04445f;
    const bool mirrored = SetMotionRegionFlag(scenFindMotionwork(character), 70, 1, MotionRegion::LowerBody) != 0;
    if (degrees < -157.5f || degrees > 157.5f)
        return 5005;
    if (mirrored) {
        if (degrees < -112.5f)
            return 5009;
        if (degrees < -67.5f)
            return 5011;
        if (degrees < -22.5f)
            return 5012;
        if (degrees < 22.5f)
            return 5006;
        if (degrees < 67.5f)
            return 5010;
        return degrees < 112.5f ? 5007 : 5008;
    }
    if (degrees < -112.5f)
        return 5008;
    if (degrees < -67.5f)
        return 5007;
    if (degrees < -22.5f)
        return 5010;
    if (degrees < 22.5f)
        return 5006;
    if (degrees < 67.5f)
        return 5012;
    return degrees < 112.5f ? 5011 : 5009;
}
void TrainingRepositionSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    const shen_name character = ReadTrainingValue<shen_name>(data, 0x138);
    const std::int32_t index = ReadTrainingValue<std::int32_t>(data, 0x140);
    const Vector3 *target = ReadTrainingValue<const Vector3 *>(data, 0x150);
    std::int32_t *counters = ReadTrainingValue<std::int32_t *>(data, 0x158);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0: {
            WriteTrainingValue(data, 0x1d0, std::int32_t{0});
            WriteTrainingValue(data, 0x1d4, 9999.0f);
            mtwkSetRequestedRootMode(scenFindMotionwork(character), 2);
            const std::uint16_t motion = SelectTrainingStepMotion(character, target);
            WriteTrainingValue(data, 0x148, std::int32_t(motion));
            mtwkRequestMotion(scenFindMotionwork(character), motion, MotionRegion::LowerBody);
            if (SetMotionRegionFlag(scenFindMotionwork(character), 70, 1, MotionRegion::LowerBody))
                SetMotionRegionFlag(scenFindMotionwork(character), 69, 0, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(character), 71, 2, MotionRegion::LowerBody);
            SetMotionSpeed(scenFindMotionwork(character), 1, MotionRegion::LowerBody);
            EV_mtwkSetEnabled(scenFindMotionwork(character), 1);
            SetMotionRegionFlag(scenFindMotionwork(character), 66, 0, MotionRegion::LowerBody);
            TrainingAdvance(sequence, 100);
            break;
        }
        case 100: {
            if (!counters[index]) {
                TrainingAdvance(sequence, 101);
                break;
            }
            Vector3 position;
            EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            const float distance = GetSquaredEuclideanDistance(&position, target);
            if (distance <= ReadTrainingValue<float>(data, 0x1d4) && distance >= .05f) {
                WriteTrainingValue(data, 0x1d4, distance);
                if (!SetMotionRegionFlag(scenFindMotionwork(character), 65, 1, MotionRegion::LowerBody))
                    break;
                const std::uint16_t motion = SelectTrainingStepMotion(character, target);
                WriteTrainingValue(data, 0x148, std::int32_t(motion));
                mtwkRequestMotion(scenFindMotionwork(character), motion, MotionRegion::LowerBody);
                if (SetMotionRegionFlag(scenFindMotionwork(character), 70, 1, MotionRegion::LowerBody))
                    SetMotionRegionFlag(scenFindMotionwork(character), 69, 0, MotionRegion::LowerBody);
                const std::int32_t repetitions = ReadTrainingValue<std::int32_t>(data, 0x1d0) + 1;
                WriteTrainingValue(data, 0x1d0, repetitions);
                if (repetitions <= 7)
                    break;
            }
            counters[index] = 0;
            TrainingAdvance(sequence, 101);
            break;
        }
        case 101:
            mtwkRequestMotion(scenFindMotionwork(character), 306, MotionRegion::LowerBody);
            if (SetMotionRegionFlag(scenFindMotionwork(character), 70, 1, MotionRegion::LowerBody))
                SetMotionRegionFlag(scenFindMotionwork(character), 69, 0, MotionRegion::LowerBody);
            SetMotionSpeed(scenFindMotionwork(character), 1, MotionRegion::LowerBody);
            EV_mtwkSetEnabled(scenFindMotionwork(character), 1);
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void TrainingCameraSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    const std::uint32_t camera = ReadTrainingValue<std::uint32_t>(data, 0x138);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            sequence.mode = 2;
            sequence.nextMode = 100;
            sequence.delay = 2;
            sequence.continueAfterWait = 1;
            break;
        case 100:
            sequence.mode = 3;
            sequence.nextMode = 101;
            sequence.continueAfterWait = 1;
            break;
        case 101: {
            const std::uint16_t selected = static_cast<std::uint16_t>(camera < 9000 ? 9000 : camera);
            WriteTrainingValue(sequence.event, 0x1c, selected);
            EVS_LoopCamera(selected, shen_name{}, shen_name{});
            sequence.mode = 2;
            sequence.nextMode = 100;
            sequence.delay = 2;
            sequence.continueAfterWait = 1;
            break;
        }
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace
void TrainingAttackSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    void *event = sequence.event;
    const shen_name player = ReadTrainingValue<shen_name>(data, 0x138), opponent = ReadTrainingValue<shen_name>(data, 0x140);
    const std::int32_t flags = ReadTrainingValue<std::int32_t>(data, 0x148), style = ReadTrainingValue<std::int32_t>(data, 0x150);
    const void *package = ReadTrainingValue<const void *>(data, 0x158);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            TrainingAdvance(sequence, 100);
            break;
        case 100:
            EV_EnableCharacterDisplay(player);
            if (opponent.name == TRAINING_DEKU.name)
                EV_DisableCharacterDisplay(TRAINING_DEKU);
            else
                EV_EnableCharacterDisplay(opponent);
            if (ReadTrainingValue<std::int32_t>(event, 0x640)) {
                WriteTrainingValue(event, 0x640, std::int32_t{0});
                ResumeFight(1);
                mtwkSetRequestedRootMode(scenFindMotionwork(player), 2);
                const std::int32_t restart = ReadTrainingValue<std::int32_t>(event, 0x5b8);
                WriteTrainingValue(event, 0x5b8, std::int32_t{0});
                SetTrainingFightReaction(restart == 1 ? 409 : restart == 2 ? 408 : 0);
            } else {
                shenStartFGST(10, 10, 0, 0);
                if (package)
                    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>(package), 1);
                if (opponent.name == TRAINING_LKN.name) {
                    shenInitFightData(FindCharacterByScenarioName(player), nullptr, 0, 0, 0, 46);
                    vfCopyPosFromEntity(0, nullptr, 109.98f, 204.42f, 2.5f);
                } else
                    shenInitFightData(FindCharacterByScenarioName(player), nullptr, 0, 0, 0, 16);
                vfUpdateFlagsFor(FindCharacterByScenarioName(player), 0, 0x08000000U, flags);
                shenInitFightData(FindCharacterByScenarioName(opponent), nullptr, 1, 1, 100, static_cast<std::uint8_t>(style));
                vfUpdateFlagsFor(FindCharacterByScenarioName(player), 0, 0x10000, 0);
                vfUpdateFlagsFor(FindCharacterByScenarioName(opponent), 0, 0x10000, 0);
                if (opponent.name == TRAINING_LKN.name)
                    vfUpdateFlagsFor(FindCharacterByScenarioName(TRAINING_LKN), 0, 0x10000000, 0);
                vfmQueueFightStart(2, 99, 1, 9, -1);
                EV_vfBattleUtil(FightBattleUtilOp::CollisionFlags, shen_name{1}, 1);
            }
            TrainingAdvance(sequence, 101);
            break;
        case 101:
            if (ReadTrainingValue<std::int32_t>(event, 0x640))
                TrainingAdvance(sequence, 102);
            break;
        case 102:
            TrainingComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void RetireTrainingFight(void *event, shen_name character) {
    WriteTrainingValue(event, 0x640, std::int32_t{0});
    ResumeFight(1);
    mtwkSetRequestedRootMode(scenFindMotionwork(character), 2);
}
void ResetTrainingAuthorCharacter(shen_name character) {
    EV_EnableCharacterDisplay(character);
    UpdateMotionWorkWithPosition(scenFindMotionwork(character), nullptr, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(character), nullptr, 3);
    mtwkSetRequestedRootMode(scenFindMotionwork(character), 0);
    EV_SetCharaControllerFlags(character, NpcCharacterControllerFlags::ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, NpcCharacterControllerFlags::UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(character, false);
    SetMotionFrameTime(scenFindMotionwork(character), 0, MotionRegion::LowerBody);
    EV_mtwkSetEnabled(scenFindMotionwork(character), 1);
    SetMotionSpeed(scenFindMotionwork(character), 1, MotionRegion::LowerBody);
}
bool TrainingNeedsFacingReset(std::int32_t practice) {
    const std::int32_t wanted = !(practice & ~9) && practice != 8 ? 32768 : 0;
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(TRAINING_RYO, &rotation, EventTransformFlags::World);
    return std::abs(TrainingFacingDifference(wanted, rotation.y)) > 10922;
}
// @verified - p
void TrainingTurnSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    const std::uint64_t character = ReadTrainingValue<std::uint64_t>(data, 0x138), target = ReadTrainingValue<std::uint64_t>(data, 0x140);
    const std::uint64_t frameTime = ReadTrainingValue<std::uint64_t>(data, 0x148), initialDelay = ReadTrainingValue<std::uint64_t>(data, 0x150);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            TrainingAdvance(sequence, 100);
            break;
        case 100: {
            if (sequence.parent)
                WriteTrainingValue(sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
            WriteTrainingValue(sequence.event, 0x651, std::uint8_t{1});
            CompiledSequenceWork *child = TrainingChild(sequence, TrainingTurnStepSequence, TRAINING_TURN_STEP, 0);
            WriteTrainingValue(child, 0x138, static_cast<std::uint32_t>(character));
            WriteTrainingValue(child, 0x140, static_cast<std::uint32_t>(target));
            WriteTrainingValue(child, 0x148, static_cast<std::uint32_t>(frameTime));
            WriteTrainingValue(child, 0x150, static_cast<std::uint32_t>(initialDelay));
            sequence.mode = 8;
            sequence.nextMode = 101;
            sequence.resultSlot = 0;
            sequence.continueAfterWait = 1;
            break;
        }
        case 101:
            WriteTrainingValue(sequence.event, 0x651, std::uint8_t{0});
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
// @verified - p
void QueueTrainingCameraLookAt(CompiledSequenceWork *sequence, std::uint16_t camera, shen_name character) {
    CompiledSequenceWork *previous = sequence->children[3];
    if (previous && previous->task) {
        HLib::Task *task = previous->task;
        RetireCompiledSequenceLinks(previous);
        HLib::DestroyTaskInternal(task);
    }
    CompiledSequenceWork *child = TrainingChild(*sequence, TrainingLookCameraSequence, TRAINING_LOOK_CAMERA, 3);
    WriteTrainingValue(child, 0x138, std::uint32_t{camera});
    WriteTrainingValue(child, 0x140, character);
}
// @verified - p
void SetTrainingCharacterYaw(shen_name character, std::int32_t yaw) {
    const Vector3i rotation{0, yaw, 0};
    EV_AT_SetAngY(character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
}
void SetTrainingCameraSequence(CompiledSequenceWork *sequence, std::int32_t camera) {
    void *event = sequence->event;
    if (ReadTrainingValue<std::uint16_t>(event, 0x1c) != camera) {
        const std::uint16_t selected = static_cast<std::uint16_t>(static_cast<std::uint32_t>(camera) < 9000 ? 9000 : camera);
        WriteTrainingValue(event, 0x1c, selected);
        EVS_LoopCamera(selected, shen_name{}, shen_name{});
    }
    CompiledSequenceWork *previous = ReadTrainingValue<CompiledSequenceWork *>(event, 0x5c0);
    if (previous) {
        if (previous->task) {
            HLib::Task *task = previous->task;
            RetireCompiledSequenceLinks(previous);
            HLib::DestroyTask(task);
        }
        WriteTrainingValue(event, 0x5c0, static_cast<CompiledSequenceWork *>(nullptr));
    }
    CompiledSequenceWork *child = TrainingChild(*sequence, TrainingCameraSequence, TRAINING_CAMERA, 3);
    WriteTrainingValue(child, 0x138, camera);
    WriteTrainingValue(event, 0x5c0, child);
}
void QueueTrainingReposition(CompiledSequenceWork *sequence, std::int32_t slot, shen_name character, std::int32_t index, const Vector3 *target) {
    void *event = sequence->event;
    WriteTrainingValue(event, 0x604 + 4 * index, std::int32_t{0});
    Vector3 position;
    EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.y = target->y;
    EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_SetCharaControllerFlags(character, 2, NpcCharacterControllerState::Disabled);
    WriteTrainingValue(event, 0x604 + 4 * index, std::int32_t{1000});
    CompiledSequenceWork *child = TrainingChild(*sequence, TrainingRepositionSequence, TRAINING_REPOSITION, slot);
    WriteTrainingValue(child, 0x138, character);
    WriteTrainingValue(child, 0x140, index);
    WriteTrainingValue(child, 0x148, std::int32_t{0});
    WriteTrainingValue(child, 0x150, target);
    WriteTrainingValue(child, 0x158, static_cast<std::byte *>(event) + 0x604);
}
void TrainingTimerSequence(void *data) {
    void *event = static_cast<CompiledSequenceWork *>(data)->event;
    WriteTrainingValue(event, 0x5f8, ReadTrainingValue<std::uint32_t>(event, 0x5f8) + 1);
    WriteTrainingValue(event, 0x5fc, ReadTrainingValue<std::uint32_t>(event, 0x5fc) + 1);
}
void TrainingAdviceTimerSequence(void *data) {
    void *event = static_cast<CompiledSequenceWork *>(data)->event;
    const std::int32_t practice = ReadTrainingValue<std::int32_t>(data, 0x138), mode = ReadTrainingValue<std::int32_t>(data, 0x140);
    const std::uint32_t repetitions = ReadTrainingValue<std::uint32_t>(event, 0x54c), previous = ReadTrainingValue<std::uint32_t>(event, 0x550);
    const std::uint32_t elapsed = ReadTrainingValue<std::uint32_t>(event, 0x5f8), adviceElapsed = ReadTrainingValue<std::uint32_t>(event, 0x5fc);
    if (practice == 9) {
        if (previous != repetitions && repetitions % 3 == 0)
            WriteTrainingValue(event, 0x538, std::int32_t{1});
        if (elapsed && elapsed % 450 == 0) {
            WriteTrainingValue(event, 0x53c, std::int32_t{1});
            WriteTrainingValue(event, 0x538, std::int32_t{1});
        }
    } else if (mode == 2) {
        if ((adviceElapsed && adviceElapsed % 300 == 0) || (previous != repetitions && repetitions % 3 == 0))
            WriteTrainingValue(event, 0x538, std::int32_t{1});
    } else if (mode == 0 || mode == 1) {
        if ((adviceElapsed && adviceElapsed % 300 == 0) || (previous != repetitions && repetitions % 3 == 0))
            WriteTrainingValue(event, 0x538, std::int32_t{1});
        if (elapsed && elapsed % (mode ? 1800 : 2700) == 0) {
            WriteTrainingValue(event, 0x544, std::int32_t{1});
            WriteTrainingValue(event, 0x538, std::int32_t{1});
        } else if (!mode && elapsed && elapsed % 1800 == 0) {
            WriteTrainingValue(event, 0x540, std::int32_t{1});
            WriteTrainingValue(event, 0x538, std::int32_t{1});
        }
    }
    WriteTrainingValue(event, 0x550, repetitions);
}
void ClearTrainingSequenceChildren(CompiledSequenceWork *sequence) {
    for (std::int32_t i = 0; i < 10; ++i) {
        if (sequence->children[i]) {
            ClearTrainingSequenceChildren(sequence->children[i]);
            CompiledSequenceWork *child = sequence->children[i];
            if (child && child->task) {
                HLib::Task *task = child->task;
                RetireCompiledSequenceLinks(child);
                HLib::DestroyTask(task);
            }
        }
    }
}
void RetireTrainingSequence(CompiledSequenceWork *sequence) {
    ClearTrainingSequenceChildren(sequence);
    if (sequence && sequence->task) {
        HLib::Task *task = sequence->task;
        RetireCompiledSequenceLinks(sequence);
        HLib::DestroyTask(task);
    }
}
} // namespace shenmue::s2
