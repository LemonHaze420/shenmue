#include "s2/areas/knmn_practice.hpp"
#include "s2/advice.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/input.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
#include <cstdlib>
namespace shenmue::s2::areas {
const std::uint8_t knmnPracticeLcd[194]{6, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 14, 3, 192, 31, 128,
    4, 146, 2, 64, 16, 128, 4, 146, 2, 64, 32, 64, 2, 100, 2, 64, 38, 64, 2, 100, 30, 120, 73, 32, 1, 8, 16, 8, 73, 32, 1, 8, 16, 8, 79, 32, 1, 8, 16, 8, 64,
    32, 2, 100, 30, 120, 64, 32, 2, 100, 2, 64, 79, 32, 4, 146, 2, 64, 73, 32, 4, 146, 2, 64, 73, 32, 7, 14, 3, 192, 121, 224};
CompiledSequenceWork *KnmnChild(CompiledSequenceWork &work, HLib::TaskCallback callback, shen_name token, std::int32_t slot) {
    CompiledSequenceWork *child = QueueCompiledSequence(callback, token, work.event);
    if (!work.children[slot])
        work.children[slot] = child;
    child->parent = &work;
    child->childSlot = slot;
    child->reference = work.reference + 1;
    work.timer = slot;
    KnmnValue(&work, 0x90 + 4 * slot, std::int32_t{0});
    return child;
}
void KnmnComplete(CompiledSequenceWork &work, bool result) {
    if (work.parent) {
        KnmnValue(work.parent, 0x90 + 4 * work.childSlot, std::int32_t{1});
        if (result)
            KnmnValue(work.parent, 0xb8 + 4 * work.childSlot, KnmnValue<std::int32_t>(&work, 0x88));
    }
    RetireCompiledSequenceLinks(&work);
    HLib::DestroyCurrentTask();
}
void KnmnKillSequence(CompiledSequenceWork *work) {
    if (work && work->task) {
        HLib::Task *task = work->task;
        RetireCompiledSequenceLinks(work);
        HLib::DestroyTaskInternal(task);
    }
}
void KnmnShowPracticeLcd(KnmnPracticeWork &work) {
    work.lcdFrames = 600;
    work.lcdBitmap = knmnPracticeLcd;
    LcdmSelectMessage(1);
    LcdmInit(reinterpret_cast<std::uintptr_t>(work.lcdBitmap));
}
void KnmnRestorePracticeCharacters() {
    for (shen_name character : {KNMN_RYO, KNMN_KEM}) {
        EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(character, false);
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 1);
        SetScenarioReflectorBoardValues(character, &KNMN_PRACTICE_REFLECTOR);
        EV_SetRefBoardParamsFor(character, 1);
    }
}
void KnmnResetActors(std::int32_t practice) {
    if (!(practice & ~9) && practice != 8) {
        EV_AT_SetPos(KNMN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_PLAYER_PRACTICE_POSITION);
        EV_AT_SetAngY(KNMN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 16384, .z = 0});
        SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 71, 0, MotionRegion::LowerBody);
    }
    if (GetMotionAnimationId(scenFindMotionwork(KNMN_RYO), MotionRegion::LowerBody) != 306) {
        SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 71, 0, MotionRegion::LowerBody);
        UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 0);
        mtwkRequestMotion(scenFindMotionwork(KNMN_RYO), 306, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 71, 0, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 69, 0, MotionRegion::LowerBody);
    }
    EV_mtwkSetEnabled(scenFindMotionwork(KNMN_RYO), 1);
    SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 69, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(KNMN_KEM, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_TEACHER_PRACTICE_POSITION);
    EV_AT_SetAngY(KNMN_KEM, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 60984, .z = 0});
    SetMotionRegionFlag(scenFindMotionwork(KNMN_KEM), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), 5193, MotionRegion::LowerBody);
}
bool KnmnNeedsFacingReset(std::int32_t practice) {
    const std::int32_t wanted = !(practice & ~9) && practice != 8 ? 16384 : 0;
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(KNMN_RYO, &rotation, EventTransformFlags::World);
    const std::uint16_t forward = static_cast<std::uint16_t>(wanted - static_cast<std::uint16_t>(rotation.y));
    const std::uint16_t reverse = static_cast<std::uint16_t>(static_cast<std::uint16_t>(rotation.y) - wanted);
    const std::int32_t difference = forward >= reverse ? reverse : wanted > rotation.y ? rotation.y - wanted : -static_cast<std::int32_t>(forward);
    return std::abs(difference) > 10922;
}
void KnmnPracticeMainCallback(void *data) {
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(data);
    switch (work.state) {
    case 0:
        SetPlayerControlEnabled(0);
        work.motions[0] = 0;
        ++work.frames;
        work.state = 1;
        break;
    case 1:
        work.state = 2;
        ++work.frames;
        break;
    case 2:
        work.sequence = QueueCompiledSequence(KnmnPracticeRootSequence, SHEN_NAME(SEQP), &work);
        ++work.frames;
        work.state = 3;
        break;
    case 3:
        if (!work.active)
            work.state = 4;
        ++work.frames;
        break;
    case 4:
        work.state = 5;
        ++work.frames;
        break;
    case 5:
        if (work.fightSuspended) {
            work.fightSuspended = 0;
            ResumeFight(1);
            mtwkSetRequestedRootMode(scenFindMotionwork(KNMN_RYO), 2);
            check_dead_status(1);
        }
        for (std::size_t index = 0; work.motions[index]; ++index)
            DeleteMotion(static_cast<std::int32_t>(work.motions[index]));
        work.state = 6;
        ++work.frames;
        break;
    default:
        ++work.frames;
        break;
    }
}
void KnmnPracticeRootSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            work.lcdSequence = nullptr;
            KnmnAdvance(sequence, 103);
            break;
        case 103:
            if (!work.lcdSequence)
                work.lcdSequence = KnmnChild(sequence, CompiledLcdCountdownCallback, SHEN_NAME(DLCD), 1);
            KnmnChild(sequence, KnmnStartPracticeSequence, SHEN_NAME(S000), 0);
            KnmnWait(sequence, 8, 104);
            sequence.children[0] = nullptr;
            break;
        case 104: {
            CompiledSequenceWork *child = KnmnChild(sequence, KnmnPracticeDecisionSequence, SHEN_NAME(WDMA), 0);
            const std::int32_t arguments[]{0, 3, -1, 106, 106, 104, 104, 105};
            for (std::size_t index = 0; index < 8; ++index)
                KnmnValue(child, 0x138 + 8 * index, arguments[index]);
            sequence.mode = 9;
            sequence.continueAfterWait = 1;
            sequence.children[0] = nullptr;
            break;
        }
        case 105: {
            QueueFightAdviceControl(12);
            CompiledSequenceWork *child = KnmnChild(sequence, KnmnQteSequence, SHEN_NAME(WQTE), 0);
            KnmnValue(child, 0x138, std::int32_t{106});
            sequence.mode = 9;
            sequence.continueAfterWait = 1;
            sequence.children[0] = nullptr;
            break;
        }
        case 106:
            KnmnAdvance(sequence, 107);
            break;
        case 107:
            if (work.fightSuspended) {
                work.fightSuspended = 0;
                ResumeFight(1);
                mtwkSetRequestedRootMode(scenFindMotionwork(KNMN_RYO), 2);
                check_dead_status(1);
            }
            KnmnAdvance(sequence, 108);
            break;
        case 108:
            KnmnChild(sequence, KnmnFinishPracticeSequence, SHEN_NAME(S100), 0);
            KnmnWait(sequence, 8, 110);
            sequence.children[0] = nullptr;
            break;
        case 110:
            KnmnChild(sequence, KnmnAwardMoveSequence, SHEN_NAME(S200), 0);
            KnmnWait(sequence, 8, 111);
            sequence.children[0] = nullptr;
            break;
        case 111:
            work.active = 0;
            if (work.lcdSequence) {
                KnmnKillSequence(sequence.children[1]);
                work.lcdSequence = nullptr;
                LcdmInit(0);
                CheckHelpIcon(2048, 2048, CheckHelpIconOp::Remove);
                RetireCompiledSequenceLinks(&sequence);
                HLib::DestroyCurrentTask();
            }
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_KEM), nullptr, 0);
            break;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnStartPracticeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnAdvance(sequence, 100);
            break;
        case 100:
            KnmnValue(&work, 0x558, std::uint64_t{0});
            KnmnValue(&work, 0x560, std::uint32_t{0});
            KnmnValue(&work, 0x65c, std::uint8_t{0});
            work.learnedMove = 0;
            KnmnValue(&work, 0x5a8, std::uint64_t{0});
            work.elapsed = 0;
            work.reserved5b4 = 0;
            work.demonstrationCamera = 0;
            work.turning = 0;
            for (std::size_t index = 0; index < 10; ++index) {
                work.firstFailures[index] = 0;
                work.repeatedFailures[index] = 0;
            }
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            KnmnChild(sequence, KnmnInitializeDemonstrationSequence, SHEN_NAME(IWDE), 0);
            KnmnWait(sequence, 8, 105);
            sequence.children[0] = nullptr;
            break;
        case 105:
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnFinishPracticeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        if (!sequence.mode)
            KnmnAdvance(sequence, 100);
        else if (sequence.mode == 100) {
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_KEM), nullptr, 0);
            KnmnComplete(sequence);
            return;
        } else
            UpdateCompiledSequenceWait(&sequence);
    } while (sequence.continueLoop);
}
void KnmnAwardMoveSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnAdvance(sequence, 100);
            break;
        case 100:
            KnmnWait(sequence, 11, 101, 2);
            break;
        case 101:
            if (work.learnedMove == -1)
                KnmnAdvance(sequence, 103);
            else {
                SetMoveProficiency(0, 1, 21, 12);
                SetNewEwzoTypeForMoveLearned(0, 1, 21);
                work.resultFrames = 150;
                KnmnAdvance(sequence, 102);
            }
            break;
        case 102:
            --work.resultFrames;
            if (GetCurrentControlInput(1) == 512 || !work.resultFrames) {
                SetNewEwzoTypeForMoveLearned(1, 0, 0);
                KnmnAdvance(sequence, 103);
            }
            break;
        case 103:
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnInitializeDemonstrationSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    EV_EnableCharacterDisplay(KNMN_RYO);
    EV_EnableCharacterDisplay(KNMN_KEM);
    EV_DisableCharacterDisplay(KNMN_DEKU);
    EV_SetCharacterAlphaEnabled(KNMN_RYO, false);
    EV_SetCharacterAlphaEnabled(KNMN_KEM, false);
    for (shen_name character : {KNMN_RYO, KNMN_KEM, KNMN_DEKU}) {
        EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    }
    for (shen_name character : {KNMN_RYO, KNMN_KEM}) {
        EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
        SetScenarioCharacterDrawControl(character, 1, 1);
    }
    EV_mtwkSetEnabled(scenFindMotionwork(KNMN_RYO), 1);
    EV_mtwkSetEnabled(scenFindMotionwork(KNMN_KEM), 0);
    SetMotionSpeed(scenFindMotionwork(KNMN_RYO), 1, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(KNMN_KEM), 1, MotionRegion::LowerBody);
    for (shen_name character : {KNMN_RYO, KNMN_KEM}) {
        mtwkSetRequestedRootMode(scenFindMotionwork(character), 1);
        SetScenarioReflectorBoardValues(character, &KNMN_PRACTICE_REFLECTOR);
        EV_SetRefBoardParamsFor(character, 1);
    }
    EV_AT_SetPos(KNMN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_PLAYER_PRACTICE_POSITION);
    EV_AT_SetAngY(KNMN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 16384, .z = 0});
    SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(KNMN_KEM, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_TEACHER_PRACTICE_POSITION);
    EV_AT_SetAngY(KNMN_KEM, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 60984, .z = 0});
    SetMotionRegionFlag(scenFindMotionwork(KNMN_KEM), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), 5193, MotionRegion::LowerBody);
    KnmnComplete(sequence);
}
} // namespace shenmue::s2::areas
