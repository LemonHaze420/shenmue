#include "s2/areas/knmn_practice.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
namespace shenmue::s2::areas {
void KnmnFeedbackOpening(CompiledSequenceWork &sequence) {
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    const std::uint8_t gesture = KnmnValue<std::uint8_t>(&work, 0x65c);
    if (gesture <= 2) {
        UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_KEM), nullptr, 0);
        mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), static_cast<std::uint16_t>(5195 + gesture), MotionRegion::LowerBody);
    } else if (gesture == 3 && SetMotionRegionFlag(scenFindMotionwork(KNMN_KEM), 65, 1, MotionRegion::LowerBody)) {
        Vector3 target;
        EV_GetCharaPosition(KNMN_RYO, 5, &target, EventTransformFlags::World);
        UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_KEM), &target, 0);
        mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), 5193, MotionRegion::LowerBody);
    }
    const std::uint8_t next = static_cast<std::uint8_t>(gesture + 1);
    KnmnValue(&work, 0x65c, std::uint8_t(next > 3 ? 0 : next));
}
void KnmnFeedbackClosing() {
    if (SetMotionRegionFlag(scenFindMotionwork(KNMN_KEM), 65, 1, MotionRegion::LowerBody)) {
        Vector3 target;
        EV_GetCharaPosition(KNMN_RYO, 5, &target, EventTransformFlags::World);
        UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_KEM), &target, 0);
        mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), 5193, MotionRegion::LowerBody);
    }
}
namespace {
void KnmnFeedbackLine(void *data, const char *dialogue, bool gesture, std::int32_t initialWait = 0) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            if (gesture)
                KnmnFeedbackOpening(sequence);
            if (initialWait)
                KnmnWait(sequence, 2, 100, initialWait);
            else
                KnmnAdvance(sequence, 100);
            break;
        case 100:
            EV_PlayDialogue(dialogue);
            KnmnWait(sequence, 4, 101, 60);
            break;
        case 101:
            if (gesture)
                KnmnFeedbackClosing();
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace
void KnmnIdleAdviceSequence(void *data) {
    KnmnFeedbackLine(data, "0026B039", false);
}
void KnmnStayAndStrikeSequence(void *data) {
    KnmnFeedbackLine(data, "0026B038", true);
}
void KnmnRelaxShouldersSequence(void *data) {
    KnmnFeedbackLine(data, "0026B056", true);
}
void KnmnUseHandsSequence(void *data) {
    KnmnFeedbackLine(data, "0026B037", true);
}
void KnmnUseLegsSequence(void *data) {
    KnmnFeedbackLine(data, "0026B036", true);
}
void KnmnHandsSlowSequence(void *data) {
    KnmnFeedbackLine(data, "0026B040", true);
}
void KnmnHandsLegsTogetherSequence(void *data) {
    KnmnFeedbackLine(data, "0026B041", true);
}
void KnmnRelaxWithoutGestureSequence(void *data) {
    KnmnFeedbackLine(data, "0026B056", false);
}
void KnmnUnclassifiedFeedbackSequence(void *data) {
    KnmnRelaxWithoutGestureSequence(data);
}
void KnmnQteFailureSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnFeedbackOpening(sequence);
            KnmnWait(sequence, 2, 100, 30);
            break;
        case 100:
            EV_PlayDialogue("0026B056");
            KnmnWait(sequence, 4, 101, 60);
            break;
        case 101:
            KnmnFeedbackClosing();
            knmnQteDialogue = nullptr;
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnSelectFeedback(CompiledSequenceWork &sequence, HLib::TaskCallback *callback) {
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    const KnmnMoveResult &result = work.results[work.resultIndex + 1];
    if (work.result == 20)
        *callback = KnmnIdleAdviceSequence;
    else if (result.steps != 1)
        *callback = KnmnUnclassifiedFeedbackSequence;
    else {
        switch (result.error) {
        case 2:
        case 10:
        case 11:
            *callback = KnmnRelaxShouldersSequence;
            break;
        case 3:
            *callback = KnmnUseHandsSequence;
            break;
        case 4:
            *callback = KnmnUseLegsSequence;
            break;
        case 12:
            *callback = KnmnStayAndStrikeSequence;
            break;
        case 15:
            *callback = (work.results[0].detail & 1) ? KnmnHandsSlowSequence : KnmnHandsLegsTogetherSequence;
            break;
        default:
            *callback = KnmnRelaxWithoutGestureSequence;
            break;
        }
    }
}
void KnmnFeedbackSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnAdvance(sequence, 100);
            break;
        case 100: {
            HLib::TaskCallback callback;
            KnmnSelectFeedback(sequence, &callback);
            KnmnChild(sequence, callback, SHEN_NAME(FS1S), 1);
            sequence.resultSlot = 1;
            KnmnWait(sequence, 8, 101);
            break;
        }
        case 101:
            work.dialogueSequence = nullptr;
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnExitPracticeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    const std::int64_t retry = KnmnValue<std::int64_t>(data, 0x140), finish = KnmnValue<std::int64_t>(data, 0x150);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnAdvance(sequence, 100);
            break;
        case 100:
            mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), 5193, MotionRegion::LowerBody);
            if (work.practiceExpired)
                KnmnAdvance(sequence, 101);
            else if (work.practiceWarning)
                KnmnAdvance(sequence, 105);
            else {
                KnmnValue(data, 0x88, static_cast<std::int32_t>(retry));
                sequence.reserved1a0 = 1;
                KnmnAdvance(sequence, 125);
            }
            break;
        case 101:
            work.conversationActive = 1;
            KnmnWait(sequence, 13, 103, 5);
            sequence.continueAfterWait = 0;
            break;
        case 103:
            KnmnWait(sequence, 13, 104, 5);
            sequence.continueAfterWait = 0;
            break;
        case 104:
            KnmnValue(data, 0x88, static_cast<std::int32_t>(finish));
            sequence.reserved1a0 = 1;
            work.practiceExpired = 0;
            KnmnAdvance(sequence, 125);
            break;
        case 125:
            KnmnComplete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnRepeatFeedbackSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnAdvance(sequence, 100);
            break;
        case 100:
            EV_AT_SetPos(KNMN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_PLAYER_PRACTICE_POSITION);
            EV_AT_SetAngY(KNMN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 16384, .z = 0});
            EV_AT_SetPos(KNMN_KEM, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_TEACHER_PRACTICE_POSITION);
            EV_AT_SetAngY(KNMN_KEM, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 60984, .z = 0});
            EV_DisableCharacterDisplay(KNMN_KEM);
            KnmnAdvance(sequence, 101);
            break;
        case 101:
            figpSetFlagsBlendForBlur(KNMN_RYO, 8, 2);
            figpAssignModeFor(KNMN_RYO, 4);
            SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 71, 0, MotionRegion::LowerBody);
            EV_mtwkSetEnabled(scenFindMotionwork(KNMN_RYO), 1);
            mtwkRequestMotion(scenFindMotionwork(KNMN_RYO), 1, MotionRegion::LowerBody);
            SetMotionLayerFrameRange(scenFindMotionwork(KNMN_RYO), -1, -1, -1, MotionRegion::LowerBody);
            SetMotionSpeed(scenFindMotionwork(KNMN_RYO), 1, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 69, 0, MotionRegion::LowerBody);
            sequence.waitCharacter = KNMN_RYO;
            KnmnWait(sequence, 5, 102);
            break;
        case 102:
            SetMotionSpeed(scenFindMotionwork(KNMN_RYO), 1, MotionRegion::LowerBody);
            figpAssignModeFor(KNMN_RYO, 0);
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
