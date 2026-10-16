#include "s2/areas/area_syw0.hpp"
#include "s2/areas/syw0_tables.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/motion_work.hpp"
#include "s2/training_seqp.hpp"
namespace shenmue::s2::areas {
void Syw0PlaceTeacher(std::int32_t practice) {
    if (practice < 0 || (practice > 1 && practice != 2 && practice != 9))
        return;
    EV_AT_SetPos(
        SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, practice <= 1 ? SYW0_TEACHER_WEIGHT_POSITION : SYW0_TEACHER_FORCE_POSITION);
    const Vector3i rotation{.x = 0, .y = practice <= 1 ? 32768 : 49152, .z = 0};
    EV_AT_SetAngY(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
}
void Syw0BeginTeacherGesture(CompiledSequenceWork &sequence) {
    void *event = sequence.event;
    if (GetMotionAnimationId(scenFindMotionwork(SYW0_SYE), MotionRegion::LowerBody) != 8841 &&
        !SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 65, 1, MotionRegion::LowerBody))
        return;
    const std::uint32_t gesture = Syw0Value<std::uint32_t>(event, 0x65c);
    if (gesture <= 2) {
        Syw0PlaceTeacher(Syw0Value<std::int32_t>(event, 0x5e8));
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_SYE), nullptr, 0);
        mtwkRequestMotion(scenFindMotionwork(SYW0_SYE), static_cast<std::uint16_t>(8838 + gesture), MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 71, 0, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 66, 2, MotionRegion::LowerBody);
    } else if (gesture == 3 && SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 65, 1, MotionRegion::LowerBody)) {
        Syw0PlaceTeacher(Syw0Value<std::int32_t>(event, 0x5e8));
        Vector3 target;
        EV_GetCharaPosition(SYW0_RYO, 5, &target, EventTransformFlags::World);
        UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_SYE), &target, 0);
        mtwkRequestMotion(scenFindMotionwork(SYW0_SYE), 8841, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 71, 0, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 66, 0, MotionRegion::LowerBody);
    }
    Syw0Value(event, 0x65c, gesture + 1 > 2 ? std::uint32_t{0} : gesture + 1);
}
void Syw0EndTeacherGesture() {
    if (!SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 65, 1, MotionRegion::LowerBody))
        return;
    Vector3 target;
    EV_GetCharaPosition(SYW0_RYO, 5, &target, EventTransformFlags::World);
    UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_SYE), &target, 0);
    mtwkRequestMotion(scenFindMotionwork(SYW0_SYE), 8841, MotionRegion::LowerBody);
}
namespace {
void Syw0NoAttemptFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x8000}, 1);
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(2.0f));
            const char *voice = selected == 0 ? "0064B021" : "0064B022";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            const char *voice = selected == 0 ? "0064B028" : selected == 1 ? "0064B029" : "0064B030";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 102, 60);
            break;
        }
        case 102:
            EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x8000}, 0);
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0SimultaneousForceFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            EV_PlayDialogue("0064B091");
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0SlowFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(2.0f));
            const char *voice = selected == 0 ? "0064B079" : "0064B080";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(2.0f));
            const char *voice = selected == 0 ? "0064B081" : "0064B082";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 102, 60);
            break;
        }
        case 102:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0StepBackFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            const char *voice = selected == 0 ? "0064B031" : selected == 1 ? "0064B032" : "0064B033";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0ForgotStepFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(2.0f));
            const char *voice = selected == 0 ? "0064B038" : "0064B039";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0WeightOnlyFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            EV_PlayDialogue("0064B034");
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0FirstStepFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            EV_PlayDialogue("0064B031");
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0UnneededArmFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            const char *voice = selected == 0 ? "0064B025" : selected == 1 ? "0064B031" : "0064B034";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0UnneededLegFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            const char *voice = selected == 0 ? "0064B026" : selected == 1 ? "0064B031" : "0064B034";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0UnneededForceFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            const char *voice = selected == 0 ? "0064B027" : selected == 1 ? "0064B031" : "0064B034";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0UnneededMoveFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
            const char *voice = selected == 0 ? "0064B024" : selected == 1 ? "0064B031" : "0064B034";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0ExtraMotionFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(2.0f));
            const char *voice = selected == 0 ? "0064B068" : "0064B024";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101: {
            EV_PlayDialogue("0064B070");
            Syw0Wait(sequence, 4, 102, 60);
            break;
        }
        case 102:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0ArmConcentrationFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(2.0f));
            const char *voice = selected == 0 ? "0064B040" : "0064B091";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0LegConcentrationFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(2.0f));
            const char *voice = selected == 0 ? "0064B045" : "0064B091";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0ArmForceFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            EV_PlayDialogue("0064B043");
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0ArmForceLateFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            EV_PlayDialogue("0064B044");
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0LegForceFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            EV_PlayDialogue("0064B048");
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0LegForceLateFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            EV_PlayDialogue("0064B049");
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0BothForceFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(2.0f));
            const char *voice = selected == 0 ? "0064B075" : "0064B076";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0BothForceLateFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(2.0f));
            const char *voice = selected == 0 ? "0064B077" : "0064B078";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0NoArmForceFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            const std::int32_t selected = static_cast<std::int32_t>(GetRandomNumberInRange(2.0f));
            const char *voice = selected == 0 ? "0064B071" : "0064B072";
            EV_PlayDialogue(voice);
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0ForceWasteFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            EV_PlayDialogue("0064B027");
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0MotionWasteFeedback(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Advance(sequence, 100);
            break;
        case 100: {
            EV_PlayDialogue("0064B024");
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Syw0DefaultFeedback(void *data) {
    Syw0SimultaneousForceFeedback(data);
}
} // namespace
HLib::TaskCallback Syw0SelectWeightFeedback(CompiledSequenceWork &sequence) {
    void *event = sequence.event;
    const std::int32_t result = Syw0Value<std::int32_t>(event, 0x5ec), index = Syw0Value<std::int32_t>(event, 0x5f0);
    if (result == 16)
        return Syw0SlowFeedback;
    if (result == 17)
        return Syw0SimultaneousForceFeedback;
    if (result == 18)
        return Syw0StepBackFeedback;
    if (result == 20)
        return Syw0NoAttemptFeedback;
    const std::size_t record = 12 * (index + 11);
    if (Syw0Value<std::uint8_t>(event, record + 3) != 1)
        return Syw0DefaultFeedback;
    switch (Syw0Value<std::uint8_t>(event, record + 2)) {
    case 1:
        return Syw0StepBackFeedback;
    case 2:
        return Syw0WeightOnlyFeedback;
    case 7:
        return Syw0ForgotStepFeedback;
    case 8:
        return Syw0UnneededArmFeedback;
    case 9:
        return Syw0UnneededLegFeedback;
    case 10:
        return Syw0UnneededForceFeedback;
    case 11:
        return Syw0UnneededMoveFeedback;
    default:
        return Syw0DefaultFeedback;
    }
}
HLib::TaskCallback Syw0SelectForceFeedback(CompiledSequenceWork &sequence) {
    void *event = sequence.event;
    const std::int32_t result = Syw0Value<std::int32_t>(event, 0x5ec), index = Syw0Value<std::int32_t>(event, 0x5f0);
    if (result == 16)
        return Syw0SlowFeedback;
    if (result == 17)
        return Syw0SimultaneousForceFeedback;
    if (result == 18)
        return Syw0NoArmForceFeedback;
    if (result == 19)
        return Syw0BothForceFeedback;
    if (result == 20)
        return Syw0NoAttemptFeedback;
    const std::size_t record = 12 * (index + 11);
    const std::uint8_t kind = Syw0Value<std::uint8_t>(event, record + 3), reason = Syw0Value<std::uint8_t>(event, record + 2);
    if (kind == 1) {
        if (reason == 1)
            return Syw0StepBackFeedback;
        if (reason == 2 || (reason >= 8 && reason <= 11))
            return Syw0FirstStepFeedback;
        if (reason == 7)
            return Syw0ForgotStepFeedback;
    } else if (kind == 2) {
        switch (reason) {
        case 1:
            return Syw0StepBackFeedback;
        case 2:
            return Syw0WeightOnlyFeedback;
        case 3:
            return Syw0ArmConcentrationFeedback;
        case 4:
            return Syw0LegConcentrationFeedback;
        case 7:
            return Syw0ForgotStepFeedback;
        case 10:
            return Syw0ForceWasteFeedback;
        case 11:
            return Syw0MotionWasteFeedback;
        case 12:
            return Syw0ExtraMotionFeedback;
        case 15: {
            const std::uint8_t force = Syw0Value<std::uint8_t>(event, 0x80), frames = Syw0Value<std::uint8_t>(event, 0x7d);
            if (force == 3)
                return frames < 4 ? Syw0BothForceFeedback : Syw0BothForceLateFeedback;
            if (force & 1)
                return frames < 4 ? Syw0ArmForceFeedback : Syw0ArmForceLateFeedback;
            if (force & 2)
                return frames < 4 ? Syw0LegForceFeedback : Syw0LegForceLateFeedback;
            return Syw0ForgotStepFeedback;
        }
        default:
            break;
        }
    }
    return Syw0DefaultFeedback;
}
} // namespace shenmue::s2::areas
