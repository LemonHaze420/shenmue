#include "s2/areas/area_knw0.hpp"
#include "s2/advice.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
namespace shenmue::s2::areas {
extern std::int32_t knw0AdviceShown;
namespace {
void Knw0FeedbackOpening(CompiledSequenceWork &sequence) {
    Knw0Work &work = *static_cast<Knw0Work *>(sequence.event);
    switch (work.gesture) {
    case 0:
        UpdateMotionWorkWithPosition(scenFindMotionwork(KNW0_KNK), nullptr, 0);
        mtwkRequestMotion(scenFindMotionwork(KNW0_KNK), 0x1905, MotionRegion::LowerBody);
        break;
    case 1:
        UpdateMotionWorkWithPosition(scenFindMotionwork(KNW0_KNK), nullptr, 0);
        mtwkRequestMotion(scenFindMotionwork(KNW0_KNK), 0x1904, MotionRegion::LowerBody);
        break;
    case 2:
        if (SetMotionRegionFlag(scenFindMotionwork(KNW0_KNK), 65, 1, MotionRegion::LowerBody)) {
            Vector3 target;
            EV_GetCharaPosition(KNW0_RYO, 5, &target, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNW0_KNK), &target, 0);
            mtwkRequestMotion(scenFindMotionwork(KNW0_KNK), 0x1902, MotionRegion::LowerBody);
        }
        break;
    default:
        break;
    }
    ++work.gesture;
    if (work.gesture > 2)
        work.gesture = 0;
}
void Knw0FeedbackClosing() {
    if (SetMotionRegionFlag(scenFindMotionwork(KNW0_KNK), 65, 1, MotionRegion::LowerBody)) {
        Vector3 target;
        EV_GetCharaPosition(KNW0_RYO, 5, &target, EventTransformFlags::World);
        UpdateMotionWorkWithPosition(scenFindMotionwork(KNW0_KNK), &target, 0);
        mtwkRequestMotion(scenFindMotionwork(KNW0_KNK), 0x1902, MotionRegion::LowerBody);
    }
}
void Knw0FeedbackLine(void *data, const char *first, const char *alternate, bool gesture, std::int32_t bound, const char *second = nullptr) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        if (!sequence.mode) {
            if (gesture)
                Knw0FeedbackOpening(sequence);
            Knw0Advance(sequence, 100);
        } else if (sequence.mode == 100) {
            const char *line = first;
            if (alternate && GetRandomNumberInRange(static_cast<float>(bound)) != 0)
                line = alternate;
            EV_PlayDialogue(line);
            Knw0Wait(sequence, 4, 101, 60);
        } else if (sequence.mode == 101 && second) {
            EV_PlayDialogue(second);
            Knw0Wait(sequence, 4, 102, 60);
        } else if (sequence.mode == (second ? 102 : 101)) {
            if (gesture)
                Knw0FeedbackClosing();
            Knw0Complete(sequence);
            return;
        } else
            UpdateCompiledSequenceWait(&sequence);
    } while (sequence.continueLoop);
}
void Knw0NeedlessMotion(void *data) {
    Knw0FeedbackLine(data, "0069B057", "0069B058", true, 2, "0069B059");
}
void Knw0StepForward(void *data) {
    Knw0FeedbackLine(data, "0069B034", "0069B035", true, 2);
}
void Knw0UnnecessaryMove(void *data) {
    Knw0FeedbackLine(data, "0069B023", nullptr, true, 0);
}
void Knw0HandsForce(void *data) {
    Knw0FeedbackLine(data, "0069B020", "0069B040", true, 3);
}
void Knw0LegsForce(void *data) {
    Knw0FeedbackLine(data, "0069B020", "0069B045", true, 3);
}
void Knw0ExcessLegForce(void *data) {
    Knw0FeedbackLine(data, "0069B025", nullptr, true, 0);
}
void Knw0MovingNeedlessly(void *data) {
    Knw0FeedbackLine(data, "0069B027", nullptr, true, 0);
}
void Knw0ExcessForce(void *data) {
    Knw0FeedbackLine(data, "0069B026", nullptr, true, 0);
}
void Knw0CombinedSlow(void *data) {
    Knw0FeedbackLine(data, "0069B055", nullptr, true, 0);
}
void Knw0CombinedVerySlow(void *data) {
    Knw0FeedbackLine(data, "0069B056", nullptr, true, 0);
}
void Knw0HandsSlow(void *data) {
    Knw0FeedbackLine(data, "0069B043", nullptr, true, 0);
}
void Knw0HandsVerySlow(void *data) {
    Knw0FeedbackLine(data, "0069B044", nullptr, true, 0);
}
void Knw0LegsSlow(void *data) {
    Knw0FeedbackLine(data, "0069B048", nullptr, true, 0);
}
void Knw0LegsVerySlow(void *data) {
    Knw0FeedbackLine(data, "0069B049", nullptr, true, 0);
}
void Knw0Unclassified(void *data) {
    Knw0FeedbackLine(data, "0069B028", nullptr, false, 0);
}
void Knw0UnclassifiedThunk(void *data) {
    Knw0Unclassified(data);
}
void Knw0TensePalmsAndLegs(void *data) {
    Knw0FeedbackLine(data, "0069B020", nullptr, true, 0);
}
void Knw0IdleAdvice(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        if (!sequence.mode)
            Knw0Advance(sequence, 100);
        else if (sequence.mode == 100 || sequence.mode == 101) {
            const std::int32_t random = static_cast<std::int32_t>(GetRandomNumberInRange(3));
            const char *line = sequence.mode == 100 ? (random == 0        ? "0069B028" :
                                                              random == 1 ? "0069B029" :
                                                                            "0069B030") :
                                                      (random == 0        ? "0069B031" :
                                                              random == 1 ? "0069B032" :
                                                                            "0069B033");
            EV_PlayDialogue(line);
            Knw0Wait(sequence, 4, static_cast<std::uint16_t>(sequence.mode + 1), 60);
        } else if (sequence.mode == 102) {
            Knw0Complete(sequence);
            return;
        } else
            UpdateCompiledSequenceWait(&sequence);
    } while (sequence.continueLoop);
}
HLib::TaskCallback Knw0SelectFeedback(Knw0Work &work) {
    if (work.result == 16)
        return Knw0CombinedSlow;
    if (work.result == 17)
        return Knw0TensePalmsAndLegs;
    if (work.result == 20)
        return Knw0IdleAdvice;
    const Knw0MoveResult &result = work.results[work.resultIndex + 1];
    if (result.steps != 1)
        return Knw0UnclassifiedThunk;
    switch (result.error) {
    case 1:
        return Knw0NeedlessMotion;
    case 2:
        return Knw0UnnecessaryMove;
    case 3:
        return Knw0HandsForce;
    case 4:
        return Knw0LegsForce;
    case 7:
        return Knw0StepForward;
    case 9:
        return Knw0ExcessLegForce;
    case 10:
        return Knw0ExcessForce;
    case 11:
        return Knw0MovingNeedlessly;
    case 15:
        if (work.results[0].detail == 3)
            return static_cast<std::uint8_t>(work.results[0].timing) < 4 ? Knw0CombinedSlow : Knw0CombinedVerySlow;
        if (work.results[0].detail & 1)
            return static_cast<std::uint8_t>(work.results[0].timing) < 4 ? Knw0HandsSlow : Knw0HandsVerySlow;
        if (work.results[0].detail & 2)
            return static_cast<std::uint8_t>(work.results[0].timing) < 4 ? Knw0LegsSlow : Knw0LegsVerySlow;
        return Knw0StepForward;
    default:
        return Knw0Unclassified;
    }
}
} // namespace
void Knw0Feedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Knw0Work &work = *static_cast<Knw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            if (!knw0AdviceShown && (work.repetitions >= 15 || work.elapsed >= 1800)) {
                knw0AdviceShown = 1;
                QueueFightAdviceControl(16);
            }
            Knw0Advance(sequence, 100);
            break;
        case 100:
            Knw0Child(sequence, Knw0SelectFeedback(work), SHEN_NAME(FS1S), 1);
            sequence.resultSlot = 1;
            Knw0Wait(sequence, 8, 101);
            break;
        case 101:
            work.dialogueSequence = nullptr;
            Knw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
