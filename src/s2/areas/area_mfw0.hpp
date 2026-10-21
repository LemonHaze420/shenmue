#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/seqp_event.hpp"
#include "common/math.hpp"
#include <cstring>
namespace shenmue::s2::areas {
inline constexpr shen_name MFW0_AREA{SHEN_NAME(MFW0)}, MFW0_TASK{SHEN_NAME(EVNT)}, MFW0_SEQUENCE{SHEN_NAME(SEQP)};
inline constexpr shen_name MFW0_RYO{SHEN_NAME(RYO_)}, MFW0_TKR{SHEN_NAME(TKR_)}, MFW0_DEKU{SHEN_NAME(DEKU)};
inline constexpr shen_name MFW0_BASE{SHEN_NAME(BASE)}, MFW0_BOX{SHEN_NAME(BOX_)}, MFW0_AB10{SHEN_NAME(ab10)}, MFW0_AB11{SHEN_NAME(ab11)};
inline constexpr shen_name MFW0_INVALID{0xffffffffU};
inline constexpr shen_name MFW0_CHARACTERS[]{MFW0_RYO, MFW0_TKR, MFW0_INVALID};
inline constexpr Vector3 MFW0_RYO_POSITION{.x = 162.5f, .y = 100, .z = 236.3000030517578f};
inline constexpr Vector3 MFW0_TEACHER_POSITION{.x = 160.89999389648438f, .y = 100, .z = 233.89999389648438f};
inline constexpr Vector3 MFW0_INITIAL_RYO_POSITION{.x = 165.3000030517578f, .y = 100, .z = 237.3000030517578f};
inline constexpr Vector3 MFW0_INITIAL_TEACHER_POSITION{.x = 163.3000030517578f, .y = 100, .z = 235.6999969482422f};
inline constexpr Vector4 MFW0_REFLECTOR{.x = 1, .y = .5f, .z = .5f, .w = .5f};
inline constexpr Vector4 MFW0_PRACTICE_REFLECTOR{.x = 1, .y = .2f, .z = .2f, .w = .33000001311302185f};
inline std::int32_t MFW0_HAND_POSE[19][3]{{0, 0, 46606}, {0, 2182, 64444}, {0, 0, 1456}, {0, 64080, 49154}, {0, 908, 44239}, {0, 0, 53340}, {0, 0, 0},
    {0, 1456, 54796}, {0, 0, 43692}, {0, 727, 50974}, {0, 0, 0}, {0, 2912, 54797}, {0, 182, 44421}, {0, 1456, 50245}, {0, 0, 0}, {2912, 910, 51336},
    {726, 726, 47517}, {0, 2003, 50974}, {65518, 65427, 0}};
struct Mfw0MoveResult {
    std::int8_t status, phase, error, steps, force, timing, unused06, flags;
    std::int32_t detail;
};
static_assert(sizeof(Mfw0MoveResult) == 12);
struct Mfw0Work {
    CompiledSequenceWork *sequence;
    HLib::Task *task;
    std::int32_t active, state;
    std::uint32_t frames;
    std::uint16_t camera, reserved1e;
    std::uint32_t motions[16];
    std::int32_t selector;
    std::uint32_t reserved64;
    EventReadCharactersWork *characters;
    std::uint32_t reserved70;
    std::int8_t learnedMove;
    std::uint8_t reserved75;
    std::int16_t resultFrames;
    Mfw0MoveResult results[100];
    std::uint32_t reserved528[4];
    std::int32_t advicePending, timedAdvice, practiceWarning, practiceExpired;
    std::uint32_t repetitions, consecutiveRepetitions, previousRepetitions, attemptedMoves;
    std::int32_t firstFailures[10], repeatedFailures[10];
    std::uint32_t restCount, conversationActive;
    std::int32_t changeCamera, combinedPracticeEntries, restartMotion;
    std::uint16_t demonstrationCamera, reserved5be;
    CompiledSequenceWork *cameraSequence, *dialogueSequence, *timerSequence, *adviceSequence, *lcdSequence;
    std::int32_t practice, result, resultIndex, practiceCamera;
    std::uint32_t elapsed, adviceElapsed, attempts;
    std::int32_t repositionFrames[2];
    std::byte reserved60c[0x24];
    std::int32_t lcdFrames;
    std::uint32_t reserved634;
    const void *lcdBitmap;
    std::int32_t fightSuspended;
    std::byte reserved644[0xc];
    std::uint8_t practiceReady, turning, stepHint, earlyHint, veryEarlyHint, doubleStepHint, punchHint, lateHint, veryLateHint;
    std::byte reserved659[0x2f];
};
static_assert(sizeof(Mfw0Work) == 0x688 && offsetof(Mfw0Work, selector) == 0x60 && offsetof(Mfw0Work, results) == 0x78);
static_assert(offsetof(Mfw0Work, advicePending) == 0x538 && offsetof(Mfw0Work, cameraSequence) == 0x5c0);
static_assert(offsetof(Mfw0Work, lcdFrames) == 0x630 && offsetof(Mfw0Work, fightSuspended) == 0x640 && offsetof(Mfw0Work, turning) == 0x651);
struct Mfw0AuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name author;
    std::uint16_t phase, reserved12;
    std::int32_t postFxBank;
};
struct Mfw0AuthorLoopWork {
    Mfw0AuthorWork *parent;
};
static_assert(sizeof(Mfw0AuthorWork) == 0x18 && sizeof(Mfw0AuthorLoopWork) == 8);
struct Mfw0FeedbackSelection {
    HLib::TaskCallback callback;
    std::int32_t corrective;
};
inline void Mfw0Advance(CompiledSequenceWork &work, std::uint16_t mode) {
    work.previousMode = work.mode;
    work.mode = mode;
    work.continueAfterWait = 1;
    work.continueLoop = 1;
}
inline void Mfw0Wait(CompiledSequenceWork &work, std::uint16_t mode, std::uint16_t next, std::int32_t delay, std::int32_t immediate = 1) {
    work.mode = mode;
    work.nextMode = next;
    work.delay = delay;
    work.continueAfterWait = immediate;
}
inline void Mfw0Wait(CompiledSequenceWork &work, std::uint16_t mode, std::uint16_t next) {
    work.mode = mode;
    work.nextMode = next;
    work.continueAfterWait = 1;
}
template <class T> T Mfw0Value(const void *data, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<const std::byte *>(data) + offset, sizeof(value));
    return value;
}
template <class T> void Mfw0Value(void *data, std::size_t offset, T value) {
    std::memcpy(static_cast<std::byte *>(data) + offset, &value, sizeof(value));
}
CompiledSequenceWork *Mfw0Child(CompiledSequenceWork &, HLib::TaskCallback, shen_name, std::int32_t);
void Mfw0Complete(CompiledSequenceWork &, bool result = false);
void Mfw0KillSequence(CompiledSequenceWork *);
void Mfw0TrainingSequence(void *);
void Mfw0StartTrainingSequence(void *);
void Mfw0InitializeActorsSequence(void *);
void Mfw0IntroductionSequence(void *);
void Mfw0FirstLessonSequence(void *);
void Mfw0SecondLessonSequence(void *);
void Mfw0FinalLessonSequence(void *);
void Mfw0FinishTrainingSequence(void *);
void Mfw0AwardMoveSequence(void *);
void Mfw0DemonstrationSequence(void *);
void Mfw0SlowDemonstrationSequence(void *);
void Mfw0RepeatedDemonstrationSequence(void *);
void Mfw0PracticeSequence(void *);
void Mfw0PracticeDecisionSequence(void *);
void Mfw0QueueAuthor(shen_name);
void Mfw0ResetReflectors();
void Mfw0ResetActors(std::int32_t);
void Mfw0ResetPlayer(std::int32_t);
void Mfw0ResetTeacher();
bool Mfw0NeedsFacingReset(std::int32_t);
void Mfw0PracticeSuccessSequence(void *);
void Mfw0RepeatSuccessSequence(void *);
void Mfw0ExitPracticeSequence(void *);
void Mfw0RestSequence(void *);
void Mfw0LastDemonstrationSequence(void *);
void Mfw0StepSuccessSequence(void *);
void Mfw0PunchSuccessSequence(void *);
void Mfw0StepFeedbackSequence(void *);
void Mfw0PunchFeedbackSequence(void *);
void Mfw0FinalFeedbackSequence(void *);
Mfw0FeedbackSelection Mfw0SelectStepFeedback(CompiledSequenceWork &);
Mfw0FeedbackSelection Mfw0SelectPunchFeedback(CompiledSequenceWork &);
void Mfw0UpdateRestartMotion(CompiledSequenceWork &, std::int32_t);
void Mfw0Dialogue(CompiledSequenceWork &, const char *);
void Mfw0Camera(CompiledSequenceWork &, std::uint16_t, shen_name target = MFW0_BOX);
void Mfw0TurnToTeacher(CompiledSequenceWork &);
void Mfw0Nullsub();
extern const ScriptAreaDefinition mfw0ScriptDefinition;
} // namespace shenmue::s2::areas
