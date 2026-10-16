#include "s2/areas/area_mfw0.hpp"
#include "s2/advice.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
void Mfw0Result(CompiledSequenceWork &sequence, std::int64_t result) {
    sequence.reserved88 = static_cast<std::uint32_t>(result);
    sequence.reserved1a0 = 1;
}
void Mfw0AuthorWait(CompiledSequenceWork &sequence, shen_name author, std::uint16_t next, bool reset = true) {
    if (reset) {
        ResetTrainingAuthorCharacter(MFW0_RYO);
        ResetTrainingAuthorCharacter(MFW0_TKR);
    }
    Mfw0QueueAuthor(author);
    Mfw0Wait(sequence, 13, next, 5, 0);
}
void Mfw0Choice(CompiledSequenceWork &sequence, std::int16_t yes, std::int16_t no, std::uint16_t next, bool teacher = true) {
    QueueTrainingCameraLookAt(&sequence, 9070, MFW0_RYO);
    Mfw0ResetPlayer(0);
    if (teacher)
        Mfw0ResetTeacher();
    std::int16_t choices[]{-1, yes, no, -1};
    SetupControlSpeakAndUI(choices, 450, 0, 2);
    Mfw0Wait(sequence, 6, next);
}
void Mfw0TeacherIdle(bool motion = true) {
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(MFW0_TKR, &rotation, EventTransformFlags::MotionYaw);
    SetTrainingCharacterYaw(MFW0_TKR, static_cast<std::uint16_t>(rotation.y + 10923));
    if (motion)
        mtwkRequestMotion(scenFindMotionwork(MFW0_TKR), 5003, MotionRegion::LowerBody);
}
void Mfw0SimpleLookTurnSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const shen_name actor{static_cast<std::uint32_t>(Mfw0Value<std::int64_t>(data, 0x138))};
    const shen_name target{static_cast<std::uint32_t>(Mfw0Value<std::int64_t>(data, 0x140))};
    const std::int64_t delay = Mfw0Value<std::int64_t>(data, 0x148);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            work.turning = 1;
            Vector3 position;
            EV_GetCharaPosition(target, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(actor), &position, 0);
            Mfw0Wait(sequence, 2, 101, static_cast<std::int32_t>(delay));
            break;
        }
        case 101: {
            Vector3 position;
            EV_GetCharaPosition(target, 5, &position, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(actor), &position, 0);
            work.turning = 0;
            Mfw0Complete(sequence);
            return;
        }
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace
void Mfw0PracticeSuccessSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const std::int64_t first = Mfw0Value<std::int64_t>(data, 0x138), second = Mfw0Value<std::int64_t>(data, 0x140);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Child(sequence, Mfw0DemonstrationSequence, SHEN_NAME(REPL), 0);
            sequence.waitChildIndex = 0;
            Mfw0Wait(sequence, 10, 103);
            break;
        case 103:
            Mfw0Child(sequence, Mfw0SlowDemonstrationSequence, SHEN_NAME(REP2), 0);
            sequence.resultSlot = 0;
            Mfw0Wait(sequence, 8, 105);
            break;
        case 104:
            Mfw0Advance(sequence, 105);
            break;
        case 105:
            Mfw0Advance(sequence, work.conversationActive ? 114 : 106);
            break;
        case 106:
            Mfw0AuthorWait(sequence, SHEN_NAME(AU_2), 107, false);
            break;
        case 107:
        case 115:
            Mfw0ResetReflectors();
            Mfw0Wait(sequence, 2, 122, 2);
            break;
        case 114:
            Mfw0AuthorWait(sequence, SHEN_NAME(AU_4), 115);
            break;
        case 122:
            Mfw0Choice(sequence, 37, 34, 123);
            break;
        case 123:
            Mfw0KillSequence(sequence.children[3]);
            Mfw0Advance(sequence, sequence.choice == 37 ? 124 : 128);
            break;
        case 124:
            Mfw0Camera(sequence, 9043);
            Mfw0Advance(sequence, 125);
            Mfw0Result(sequence, first);
            break;
        case 125:
            Mfw0Dialogue(sequence, "0050A022");
            break;
        case 126:
            Mfw0Camera(sequence, 9042);
            Mfw0Dialogue(sequence, "0050B105");
            break;
        case 127:
            Mfw0Advance(sequence, 137);
            break;
        case 128:
            Mfw0Result(sequence, second);
            Mfw0AuthorWait(sequence, SHEN_NAME(AU_7), 132);
            break;
        case 132:
            Mfw0ResetReflectors();
            Mfw0Wait(sequence, 2, 137, 2);
            break;
        case 137:
            Mfw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0RestSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const std::int64_t first = Mfw0Value<std::int64_t>(data, 0x138), second = Mfw0Value<std::int64_t>(data, 0x140);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Camera(sequence, 9060, shen_name{});
            Mfw0TeacherIdle();
            work.results[0].force = 0;
            Mfw0Advance(sequence, 101);
            break;
        case 101:
            Mfw0Dialogue(sequence, "0050B113");
            break;
        case 102:
            Mfw0Advance(sequence, 103);
            break;
        case 103:
            if (work.restCount < 2)
                Mfw0Choice(sequence, 37, 34, 104);
            else {
                sequence.choice = 0;
                Mfw0Advance(sequence, 104);
            }
            break;
        case 104:
            Mfw0KillSequence(sequence.children[3]);
            Mfw0Advance(sequence, sequence.choice == 37 ? 105 : 110);
            break;
        case 105:
            ++work.restCount;
            Mfw0Advance(sequence, 106);
            break;
        case 106:
            Mfw0Camera(sequence, 9043);
            Mfw0Dialogue(sequence, "0050A024");
            break;
        case 107:
            Mfw0Camera(sequence, 9042);
            Mfw0Dialogue(sequence, "0050B114");
            break;
        case 108:
            Mfw0Advance(sequence, 109);
            break;
        case 109:
            Mfw0Result(sequence, first);
            Mfw0Advance(sequence, 131);
            break;
        case 110:
            Mfw0AuthorWait(sequence, SHEN_NAME(AU_5), 115, false);
            break;
        case 115:
            Mfw0ResetReflectors();
            if (work.restCount >= 2) {
                Mfw0Advance(sequence, 116);
                sequence.choice = 0;
            } else
                Mfw0Choice(sequence, 37, 34, 116);
            break;
        case 116:
            Mfw0KillSequence(sequence.children[3]);
            Mfw0Advance(sequence, sequence.choice == 0 || sequence.choice == 34 ? 117 : 125);
            break;
        case 117:
            Mfw0Result(sequence, second);
            Mfw0AuthorWait(sequence, SHEN_NAME(AU_9), 131);
            break;
        case 125:
            Mfw0Advance(sequence, 126);
            break;
        case 126:
            Mfw0Camera(sequence, 9041);
            Mfw0Dialogue(sequence, "0050A029");
            break;
        case 127:
            Mfw0Camera(sequence, 9042);
            Mfw0Dialogue(sequence, "0050B119");
            break;
        case 128:
            Mfw0Dialogue(sequence, "0050B120");
            break;
        case 129:
            Mfw0Advance(sequence, 130);
            break;
        case 130:
            ++work.restCount;
            Mfw0Result(sequence, first);
            Mfw0Advance(sequence, 131);
            break;
        case 131:
            Mfw0ResetReflectors();
            Mfw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0LastDemonstrationSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const std::int64_t first = Mfw0Value<std::int64_t>(data, 0x138), second = Mfw0Value<std::int64_t>(data, 0x140);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100: {
            EV_EnableCharacterDisplay(MFW0_RYO);
            EV_EnableCharacterDisplay(MFW0_TKR);
            EV_AT_SetPos(MFW0_TKR, EventTransformFlags::World | EventTransformFlags::AllAxes, MFW0_TEACHER_POSITION);
            SetTrainingCharacterYaw(MFW0_TKR, 41264);
            Mfw0TeacherIdle(false);
            SetMotionRegionFlag(scenFindMotionwork(MFW0_TKR), 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(MFW0_TKR), 5003, MotionRegion::LowerBody);
            EV_AT_SetPos(MFW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, MFW0_RYO_POSITION);
            SetTrainingCharacterYaw(MFW0_RYO, 8192);
            Vector3i rotation;
            GetScenarioCharacterEulerAngles(MFW0_RYO, &rotation, EventTransformFlags::MotionYaw);
            SetTrainingCharacterYaw(MFW0_RYO, static_cast<std::uint16_t>(rotation.y));
            SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(MFW0_RYO), 0xf003, MotionRegion::LowerBody);
            if (work.restCount < 2)
                Mfw0Advance(sequence, 111);
            else {
                Mfw0Advance(sequence, 101);
                Mfw0Result(sequence, second);
            }
            break;
        }
        case 101:
            QueueTrainingCameraLookAt(&sequence, 9050, MFW0_BOX);
            Mfw0Advance(sequence, 102);
            break;
        case 102:
            Mfw0Dialogue(sequence, "0050B122");
            break;
        case 103:
            Mfw0Dialogue(sequence, "0050B123");
            break;
        case 104:
            Mfw0Dialogue(sequence, "0050B124");
            break;
        case 105:
            Mfw0Advance(sequence, 106);
            break;
        case 106:
            Mfw0Choice(sequence, 162, 60, 107);
            break;
        case 107:
            Mfw0KillSequence(sequence.children[3]);
            Mfw0Result(sequence, second);
            Mfw0Advance(sequence, sequence.choice == 162 ? 108 : 109);
            break;
        case 108:
            Mfw0AuthorWait(sequence, SHEN_NAME(AU_A), 132);
            break;
        case 109:
            Mfw0AuthorWait(sequence, SHEN_NAME(AU_C), 132);
            break;
        case 111:
            QueueTrainingCameraLookAt(&sequence, 9050, MFW0_BOX);
            Mfw0Advance(sequence, 112);
            break;
        case 112:
            Mfw0Dialogue(sequence, "0050B108");
            break;
        case 113:
            Mfw0Dialogue(sequence, "0050B109");
            break;
        case 114:
            Mfw0Advance(sequence, 115);
            break;
        case 115:
            Mfw0Choice(sequence, 37, 34, 116);
            break;
        case 116:
            Mfw0KillSequence(sequence.children[3]);
            Mfw0Result(sequence, sequence.choice == 37 ? first : second);
            Mfw0Advance(sequence, sequence.choice == 37 ? 117 : 122);
            break;
        case 117:
            ++work.restCount;
            Mfw0Advance(sequence, 118);
            break;
        case 118:
            Mfw0Camera(sequence, 9043);
            Mfw0Dialogue(sequence, "0050A024");
            break;
        case 119:
            Mfw0Camera(sequence, 9042);
            Mfw0Dialogue(sequence, "0050B110");
            break;
        case 120:
            Mfw0Advance(sequence, 121);
            break;
        case 121:
            Mfw0Advance(sequence, 132);
            break;
        case 122:
            Mfw0AuthorWait(sequence, SHEN_NAME(AU_8), 132);
            break;
        case 132:
            Mfw0ResetReflectors();
            work.elapsed = 0;
            Mfw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0ExitPracticeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const std::int64_t instruction = Mfw0Value<std::int64_t>(data, 0x138), continuation = Mfw0Value<std::int64_t>(data, 0x140);
    const std::int64_t retry = Mfw0Value<std::int64_t>(data, 0x148), exit = Mfw0Value<std::int64_t>(data, 0x150);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0TeacherIdle();
            if (work.practiceExpired)
                Mfw0Advance(sequence, 101);
            else if (work.practiceWarning)
                Mfw0Advance(sequence, 105);
            else {
                Mfw0Result(sequence, continuation);
                Mfw0Advance(sequence, 125);
            }
            break;
        case 101:
            work.conversationActive = 1;
            Mfw0Advance(sequence, instruction == 1 ? 102 : 103);
            break;
        case 102:
            Mfw0TurnToTeacher(sequence);
            sequence.resultSlot = 1;
            Mfw0Wait(sequence, 8, 103);
            break;
        case 103: {
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0RestSequence, SHEN_NAME(COEX), 2);
            Mfw0Value(child, 0x138, static_cast<std::int32_t>(continuation));
            Mfw0Value(child, 0x140, static_cast<std::int32_t>(exit));
            sequence.resultSlot = 2;
            Mfw0Wait(sequence, 8, 104);
            break;
        }
        case 104:
            Mfw0Result(sequence, Mfw0Value<std::int32_t>(data, 0xc0));
            work.practiceExpired = 0;
            Mfw0Advance(sequence, 125);
            break;
        case 105:
            work.conversationActive = 1;
            Mfw0Advance(sequence, instruction == 1 ? 106 : 107);
            break;
        case 106:
            Mfw0TurnToTeacher(sequence);
            sequence.resultSlot = 1;
            Mfw0Wait(sequence, 8, 107);
            break;
        case 107:
            QueueTrainingCameraLookAt(&sequence, 9050, MFW0_BOX);
            Mfw0Advance(sequence, 108);
            break;
        case 108:
            Mfw0Dialogue(sequence, "0050B096");
            break;
        case 109:
            Mfw0Dialogue(sequence, "0050B097");
            break;
        case 110:
            Mfw0Advance(sequence, 111);
            break;
        case 111:
            Mfw0Choice(sequence, 62, 60, 112);
            break;
        case 112:
            Mfw0KillSequence(sequence.children[3]);
            QueueFightAdviceControl(13);
            if (sequence.choice == 62)
                Mfw0Advance(sequence, 113);
            else
                Mfw0Wait(sequence, 2, 118, 10);
            break;
        case 113:
        case 116:
            Mfw0Advance(sequence, 117);
            break;
        case 117:
            Mfw0Result(sequence, retry);
            Mfw0Advance(sequence, 124);
            break;
        case 118:
            Mfw0Advance(sequence, 119);
            break;
        case 119:
            Mfw0Camera(sequence, 9043);
            Mfw0Dialogue(sequence, "0050A018");
            break;
        case 120:
            Mfw0Camera(sequence, 9042);
            Mfw0Dialogue(sequence, "0050B099");
            break;
        case 121:
            Mfw0Dialogue(sequence, "0050B100");
            break;
        case 122:
            Mfw0Advance(sequence, 123);
            break;
        case 123:
            Mfw0Result(sequence, continuation);
            Mfw0Advance(sequence, 124);
            break;
        case 124:
            work.practiceWarning = 0;
            Mfw0Advance(sequence, 125);
            break;
        case 125:
            Mfw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0StepSuccessSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Camera(sequence, 9050);
            Mfw0TurnToTeacher(sequence);
            sequence.resultSlot = 1;
            Mfw0Wait(sequence, 8, 101);
            break;
        case 101:
            Mfw0Camera(sequence, 9050);
            Mfw0Dialogue(sequence, "0050B051");
            break;
        case 102:
            Mfw0Camera(sequence, 9072);
            Mfw0Dialogue(sequence, "0050B052");
            break;
        case 103:
            Mfw0Advance(sequence, 104);
            break;
        case 104: {
            CompiledSequenceWork *child = Mfw0Child(sequence, Mfw0SimpleLookTurnSequence, SHEN_NAME(YES_), 0);
            Mfw0Value(child, 0x138, MFW0_RYO);
            Mfw0Value(child, 0x140, MFW0_TKR);
            Mfw0Value(child, 0x148, std::int32_t{15});
            sequence.resultSlot = 0;
            Mfw0Wait(sequence, 8, 105);
            break;
        }
        case 105:
            Mfw0Camera(sequence, 9052);
            Mfw0Dialogue(sequence, "0050B053");
            break;
        case 106:
            Mfw0Advance(sequence, 107);
            break;
        case 107:
            Mfw0Advance(sequence, 108);
            break;
        case 108:
            Mfw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void Mfw0PunchSuccessSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Mfw0Work &work = *static_cast<Mfw0Work *>(sequence.event);
    const std::int64_t continuation = Mfw0Value<std::int64_t>(data, 0x148), exit = Mfw0Value<std::int64_t>(data, 0x150);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Mfw0Advance(sequence, 100);
            break;
        case 100:
            Mfw0Advance(sequence, 101);
            break;
        case 101:
            Mfw0Child(sequence, Mfw0DemonstrationSequence, SHEN_NAME(REPL), 0);
            sequence.waitChildIndex = 0;
            Mfw0Wait(sequence, 10, 102);
            break;
        case 102:
            Mfw0Child(sequence, Mfw0SlowDemonstrationSequence, SHEN_NAME(REP2), 0);
            sequence.resultSlot = 0;
            Mfw0Wait(sequence, 8, 103);
            break;
        case 103:
            Mfw0AuthorWait(sequence, work.conversationActive ? SHEN_NAME(AU_4) : SHEN_NAME(AU_3), 104);
            break;
        case 104:
            Mfw0ResetReflectors();
            Mfw0Advance(sequence, 126);
            break;
        case 123:
            Mfw0Advance(sequence, 126);
            break;
        case 126:
            Mfw0Choice(sequence, 37, 34, 127, false);
            break;
        case 127:
            Mfw0KillSequence(sequence.children[3]);
            Mfw0Advance(sequence, sequence.choice == 37 ? 128 : 133);
            break;
        case 128:
            Mfw0Advance(sequence, 129);
            break;
        case 129:
            Mfw0Camera(sequence, 9043);
            Mfw0Dialogue(sequence, "0050A022");
            break;
        case 130:
            Mfw0Camera(sequence, 9042);
            Mfw0Dialogue(sequence, "0050B105");
            break;
        case 131:
            Mfw0Advance(sequence, 132);
            break;
        case 132:
            Mfw0Result(sequence, continuation);
            Mfw0Advance(sequence, 142);
            break;
        case 133:
            Mfw0Result(sequence, exit);
            Mfw0AuthorWait(sequence, SHEN_NAME(AU_7), 134);
            break;
        case 134:
            Mfw0ResetReflectors();
            Mfw0Wait(sequence, 2, 142, 2);
            break;
        case 142:
            Mfw0Complete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
