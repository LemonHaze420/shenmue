#include "s2/areas/area_mfw0.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/training_seqp.hpp"
namespace shenmue::s2::areas {
namespace {
void Mfw0NoAttemptFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B023" : selected == 1 ? "0050B024" : "0050B025");
            break;
        }
        case 101: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B026" : selected == 1 ? "0050B027" : "0050B028");
            break;
        }
        case 102:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0SimultaneousFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B093" : selected == 1 ? "0050B094" : "0050B095");
            break;
        }
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0SlowFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B081" : "0050B082");
            break;
        }
        case 101: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B083" : "0050B084");
            break;
        }
        case 102:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0VerySlowFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B085" : "0050B086");
            break;
        }
        case 101: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B084" : "0050B087");
            break;
        }
        case 102:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0BodyFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Dialogue(sequence, "0050B049");
            break;
        case 101:
            Mfw0Dialogue(sequence, "0050B050");
            break;
        case 102:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0StepReminderFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B037" : selected == 1 ? "0050B038" : "0050B039");
            break;
        }
        case 101: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B040" : selected == 1 ? "0050B041" : "0050B042");
            break;
        }
        case 102:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0DoubleStepReminderFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B046" : selected == 1 ? "0050B047" : "0050B048");
            break;
        }
        case 101: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B040" : selected == 1 ? "0050B041" : "0050B042");
            break;
        }
        case 102:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0UnnecessaryFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B090" : selected == 1 ? "0050B091" : "0050B092");
            break;
        }
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0ForgotDoubleFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B088" : "0050B089");
            break;
        }
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0UnnecessaryMovementFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Dialogue(sequence, "0050B018");
            break;
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0HandForceFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Dialogue(sequence, "0050B019");
            break;
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0LegForceFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Dialogue(sequence, "0050B020");
            break;
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0MovesFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Dialogue(sequence, "0050B022");
            break;
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0ForceFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Dialogue(sequence, "0050B021");
            break;
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0FailureFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B060" : "0050B061");
            break;
        }
        case 101: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B062" : "0050B063");
            break;
        }
        case 102:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0StepTwiceFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B054" : selected == 1 ? "0050B055" : "0050B056");
            break;
        }
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0LateRepeatFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B076" : "0050B077");
            break;
        }
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0VeryLateRepeatFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B079" : "0050B080");
            break;
        }
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0EarlyRepeatFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B067" : "0050B068");
            break;
        }
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0VeryEarlyRepeatFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            const float selected = static_cast<float>(GetRandomNumberInRange(2.0f));
            Mfw0Dialogue(sequence, selected == 0 ? "0050B071" : "0050B072");
            break;
        }
        case 101:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0FinishCorrective(CompiledSequenceWork &sequence) {
    if (sequence.parent)
        Mfw0Value(sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
    for (std::int32_t i = 0; i < 10; ++i) {
        if (sequence.children[i]) {
            ClearTrainingSequenceChildren(sequence.children[i]);
            Mfw0KillSequence(sequence.children[i]);
        }
    }
    RetireCompiledSequenceLinks(&sequence);
    HLib::DestroyCurrentTask();
    RetireCompiledSequenceLinks(&sequence);
    HLib::DestroyCurrentTask();
}
void Mfw0CorrectivePose(std::int32_t delta, bool step) {
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(MFW0_TKR, &rotation, EventTransformFlags::MotionYaw);
    SetTrainingCharacterYaw(MFW0_TKR, static_cast<std::uint16_t>(rotation.y + delta));
    mtwkRequestMotion(scenFindMotionwork(MFW0_TKR), 5000, MotionRegion::LowerBody);
    if (step)
        SetMotionLayerFrameRange(scenFindMotionwork(MFW0_TKR), -1, 300, -1, MotionRegion::LowerBody);
    GetScenarioCharacterEulerAngles(MFW0_RYO, &rotation, EventTransformFlags::MotionYaw);
    SetTrainingCharacterYaw(MFW0_RYO, static_cast<std::uint16_t>(rotation.y));
    mtwkRequestMotion(scenFindMotionwork(MFW0_RYO), 0xf003, MotionRegion::LowerBody);
}
void Mfw0StepDemonstrationFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Camera(sequence, 9016);
            Mfw0TurnToTeacher(sequence);
            sequence.waitChildIndex = 1;
            Mfw0Wait(sequence, 10, 100);
            break;
        case 100:
            Mfw0CorrectivePose(-20206, true);
            Mfw0Dialogue(sequence, "0050B034");
            break;
        case 101:
            Mfw0Dialogue(sequence, "0050B035");
            break;
        case 102:
            Mfw0Camera(sequence, 9026);
            Mfw0Dialogue(sequence, "0050A011");
            break;
        case 103:
            Mfw0Camera(sequence, 9036);
            Mfw0Dialogue(sequence, "0050B036");
            break;
        case 104:
            sequence.waitCharacter = MFW0_TKR;
            Mfw0Wait(sequence, 5, 105);
            break;
        case 105:
            work.stepHint = 1;
            Mfw0FinishCorrective(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0DoubleStepDemonstrationFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Camera(sequence, 9015);
            Mfw0TurnToTeacher(sequence);
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0CorrectivePose(-19478, false);
            Mfw0Dialogue(sequence, "0050B043");
            break;
        case 101:
            Mfw0Dialogue(sequence, "0050B044");
            break;
        case 102:
            Mfw0Camera(sequence, 9025);
            Mfw0Dialogue(sequence, "0050A012");
            break;
        case 103:
            Mfw0Camera(sequence, 9035);
            Mfw0Dialogue(sequence, "0050B045");
            break;
        case 104:
            work.doubleStepHint = 1;
            Mfw0FinishCorrective(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0PunchDemonstrationFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Camera(sequence, 9015);
            Mfw0TurnToTeacher(sequence);
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0CorrectivePose(-20753, false);
            Mfw0Dialogue(sequence, "0050B057");
            break;
        case 101:
            Mfw0Dialogue(sequence, "0050B058");
            break;
        case 102:
            Mfw0Camera(sequence, 9025);
            Mfw0Dialogue(sequence, "0050A013");
            break;
        case 103:
            Mfw0Camera(sequence, 9035);
            Mfw0Dialogue(sequence, "0050B059");
            break;
        case 104:
            work.punchHint = 1;
            Mfw0FinishCorrective(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0LateDemonstrationFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Camera(sequence, 9017);
            Mfw0TurnToTeacher(sequence);
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0CorrectivePose(-20753, false);
            Mfw0Dialogue(sequence, "0050B073");
            break;
        case 101:
            Mfw0Dialogue(sequence, "0050B074");
            break;
        case 102:
            Mfw0Camera(sequence, 9027);
            Mfw0Dialogue(sequence, "0050A016");
            break;
        case 103:
            Mfw0Camera(sequence, 9037);
            Mfw0Dialogue(sequence, "0050B075");
            break;
        case 104:
            work.lateHint = 1;
            Mfw0FinishCorrective(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0EarlyDemonstrationFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Camera(sequence, 9015);
            Mfw0TurnToTeacher(sequence);
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0CorrectivePose(-20753, false);
            Mfw0Dialogue(sequence, "0050B064");
            break;
        case 101:
            Mfw0Dialogue(sequence, "0050B065");
            break;
        case 102:
            Mfw0Camera(sequence, 9025);
            Mfw0Dialogue(sequence, "0050A014");
            break;
        case 103:
            Mfw0Camera(sequence, 9035);
            Mfw0Dialogue(sequence, "0050B066");
            break;
        case 104:
            work.earlyHint = 1;
            Mfw0FinishCorrective(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0VeryLateDemonstrationFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Camera(sequence, 9017);
            Mfw0TurnToTeacher(sequence);
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0CorrectivePose(-20753, false);
            Mfw0Dialogue(sequence, "0050B078");
            break;
        case 101:
            Mfw0Dialogue(sequence, "0050B070");
            break;
        case 102:
            Mfw0Camera(sequence, 9027);
            Mfw0Dialogue(sequence, "0050A015");
            break;
        case 103:
            work.veryLateHint = 1;
            Mfw0FinishCorrective(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0VeryEarlyDemonstrationFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Camera(sequence, 9015);
            Mfw0TurnToTeacher(sequence);
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0CorrectivePose(-20753, false);
            Mfw0Dialogue(sequence, "0050B069");
            break;
        case 101:
            Mfw0Dialogue(sequence, "0050B070");
            break;
        case 102:
            Mfw0Camera(sequence, 9025);
            Mfw0Dialogue(sequence, "0050A015");
            break;
        case 103:
            work.veryEarlyHint = 1;
            Mfw0FinishCorrective(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace
Mfw0FeedbackSelection Mfw0SelectStepFeedback(CompiledSequenceWork &sequence) {
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const Mfw0MoveResult &result = work.results[1 + work.resultIndex];
    switch (work.result) {
    case 2:
    case 8:
    case 9:
    case 10:
    case 11:
        return {.callback = Mfw0BodyFeedback, .corrective = 0};
    case 16:
        return {.callback = static_cast<std::uint8_t>(result.timing) >= 6 ? Mfw0VerySlowFeedback : Mfw0SlowFeedback, .corrective = 0};
    case 17:
        return {.callback = Mfw0SimultaneousFeedback, .corrective = 0};
    case 20:
        return {.callback = Mfw0NoAttemptFeedback, .corrective = 0};
    default:
        if (result.steps == 1)
            return {.callback = work.stepHint ? Mfw0StepReminderFeedback : Mfw0StepDemonstrationFeedback, .corrective = !work.stepHint};
        if (result.steps == 2)
            return {.callback = work.doubleStepHint ? Mfw0DoubleStepReminderFeedback : Mfw0DoubleStepDemonstrationFeedback, .corrective = !work.doubleStepHint};
        return {.callback = Mfw0FailureFeedback, .corrective = 0};
    }
}
Mfw0FeedbackSelection Mfw0SelectPunchFeedback(CompiledSequenceWork &sequence) {
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const Mfw0MoveResult &result = work.results[1 + work.resultIndex];
    HLib::TaskCallback callback = Mfw0FailureFeedback;
    switch (work.result) {
    case 16:
        callback = Mfw0SlowFeedback;
        break;
    case 17:
        callback = Mfw0SimultaneousFeedback;
        break;
    case 20:
        callback = Mfw0NoAttemptFeedback;
        break;
    default:
        if (result.steps == 1) {
            switch (result.error) {
            case 1:
                callback = Mfw0UnnecessaryFeedback;
                break;
            case 2:
                callback = Mfw0UnnecessaryMovementFeedback;
                break;
            case 7:
                callback = Mfw0ForgotDoubleFeedback;
                break;
            case 8:
                callback = Mfw0HandForceFeedback;
                break;
            case 9:
                callback = Mfw0LegForceFeedback;
                break;
            case 10:
                callback = Mfw0ForceFeedback;
                break;
            case 11:
                callback = Mfw0MovesFeedback;
                break;
            default:
                break;
            }
        } else if (result.steps == 2) {
            switch (result.error) {
            case 1:
                callback = Mfw0StepTwiceFeedback;
                break;
            case 3:
                return {.callback = work.punchHint ? Mfw0FailureFeedback : Mfw0PunchDemonstrationFeedback, .corrective = !work.punchHint};
            case 7:
                callback = Mfw0ForgotDoubleFeedback;
                break;
            case 9:
                callback = Mfw0LegForceFeedback;
                break;
            case 10:
                callback = Mfw0ForceFeedback;
                break;
            case 11:
                callback = Mfw0MovesFeedback;
                break;
            case 15:
                if (work.results[0].detail & 1) {
                    if (static_cast<std::uint8_t>(work.results[0].timing) >= 4)
                        return {
                            .callback = work.veryLateHint ? Mfw0VeryLateRepeatFeedback : Mfw0VeryLateDemonstrationFeedback, .corrective = !work.veryLateHint};
                    return {.callback = work.lateHint ? Mfw0LateRepeatFeedback : Mfw0LateDemonstrationFeedback, .corrective = !work.lateHint};
                }
                if (static_cast<std::uint8_t>(work.results[0].timing) >= 4)
                    return {
                        .callback = work.veryEarlyHint ? Mfw0VeryEarlyRepeatFeedback : Mfw0VeryEarlyDemonstrationFeedback, .corrective = !work.veryEarlyHint};
                return {.callback = work.earlyHint ? Mfw0EarlyRepeatFeedback : Mfw0EarlyDemonstrationFeedback, .corrective = !work.earlyHint};
            default:
                break;
            }
        }
    }
    return {.callback = callback, .corrective = 0};
}
void Mfw0UpdateRestartMotion(CompiledSequenceWork &sequence, std::int32_t practice) {
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    if (work.results[0].error == 20 && !(work.timedAdvice | work.practiceWarning | work.practiceExpired)) {
        Mfw0FeedbackSelection selection;
        if (!practice)
            selection = Mfw0SelectStepFeedback(sequence);
        else if (practice == 1 || practice == 9)
            selection = Mfw0SelectPunchFeedback(sequence);
        if (!selection.corrective) {
            work.restartMotion = work.advicePending ? 2 : 0;
            return;
        }
    }
    work.restartMotion = work.advicePending ? 2 : 1;
}
void Mfw0StepFeedbackSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Child(sequence, Mfw0SelectStepFeedback(sequence).callback, SHEN_NAME(FS1S), 1);
            sequence.resultSlot = 1;
            Mfw0Wait(sequence, 8, 101);
            break;
        case 101:
            static_cast<Mfw0Work *>(sequence.event)->dialogueSequence = nullptr;
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0PunchFeedbackSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Child(sequence, Mfw0SelectPunchFeedback(sequence).callback, SHEN_NAME(FS2S), 1);
            sequence.resultSlot = 1;
            Mfw0Wait(sequence, 8, 101);
            break;
        case 101:
            static_cast<Mfw0Work *>(sequence.event)->dialogueSequence = nullptr;
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0FinalFeedbackSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const std::int64_t first = Mfw0Value<std::int64_t>(data, 0x138), second = Mfw0Value<std::int64_t>(data, 0x140);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, (work.timedAdvice | work.practiceWarning | work.practiceExpired) ? 101 : 100);
            break;
        case 100:
            Mfw0Child(sequence, Mfw0PunchFeedbackSequence, SHEN_NAME(FS_2), 1);
            sequence.resultSlot = 1;
            Mfw0Wait(sequence, 8, 101);
            break;
        case 101: {
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0LastDemonstrationSequence, SHEN_NAME(COE3), 1);
            Mfw0Value(child, 0x138, static_cast<std::int32_t>(first));
            Mfw0Value(child, 0x140, static_cast<std::int32_t>(second));
            Mfw0Value(child, 0x148, std::int32_t{2});
            sequence.resultSlot = 1;
            Mfw0Wait(sequence, 8, 102);
            break;
        }
        case 102:
            sequence.reserved88 = Mfw0Value<std::uint32_t>(data, 0xbc);
            sequence.reserved1a0 = 1;
            Mfw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0RepeatSuccessSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    const std::int64_t first = Mfw0Value<std::int64_t>(data, 0x148), second = Mfw0Value<std::int64_t>(data, 0x150);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0LastDemonstrationSequence, SHEN_NAME(COE3), 1);
            Mfw0Value(child, 0x138, static_cast<std::int32_t>(first));
            Mfw0Value(child, 0x140, static_cast<std::int32_t>(second));
            Mfw0Value(child, 0x148, std::int32_t{1});
            sequence.resultSlot = 1;
            Mfw0Wait(sequence, 8, 101);
            break;
        }
        case 101:
            sequence.reserved88 = Mfw0Value<std::uint32_t>(data, 0xbc);
            sequence.reserved1a0 = 1;
            Mfw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
