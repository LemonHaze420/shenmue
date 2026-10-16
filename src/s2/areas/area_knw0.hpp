#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/seqp_event.hpp"
#include "common/math.hpp"
#include <cstring>
namespace shenmue::s2::areas {
inline constexpr shen_name KNW0_AREA{SHEN_NAME(KNW0)}, KNW0_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name KNW0_RYO{SHEN_NAME(RYO_)}, KNW0_KNK{SHEN_NAME(KNK_)}, KNW0_DEKU{SHEN_NAME(DEKU)};
inline constexpr shen_name KNW0_CHARACTERS[]{KNW0_RYO, KNW0_KNK, shen_name{0xffffffffU}};
inline constexpr Vector3 KNW0_INITIAL_PLAYER{.x = 335.100006103515625f, .y = 127, .z = 268.399993896484375f};
inline constexpr Vector3 KNW0_INITIAL_TEACHER{.x = 338.399993896484375f, .y = 127, .z = 268.5f};
inline constexpr Vector3 KNW0_TEACHER_POSITION{.x = 337.399993896484375f, .y = 127, .z = 270.100006103515625f};
inline constexpr Vector3 KNW0_PLAYER_POSITION{.x = 335.20001220703125f, .y = 127, .z = 268.100006103515625f};
struct Knw0MoveResult {
    std::int8_t status, phase, error, steps, force, timing, unused06, flags;
    std::int32_t detail;
};
struct Knw0Work {
    CompiledSequenceWork *sequence;
    HLib::Task *task;
    std::int32_t active, state;
    std::uint32_t frames;
    std::uint16_t camera, reserved1e;
    std::uint32_t motions[16];
    std::int32_t entryPoint;
    std::uint32_t reserved64;
    EventReadCharactersWork *characters;
    std::uint32_t reserved70;
    std::int8_t learnedMove;
    std::uint8_t reserved75;
    std::int16_t resultFrames;
    Knw0MoveResult results[100];
    std::byte reserved528[0x10];
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
    std::uint8_t practiceReady, turning;
    std::byte reserved652[0xa];
    std::uint8_t gesture;
    std::byte reserved65d[0x2b];
};
static_assert(sizeof(Knw0Work) == 0x688 && offsetof(Knw0Work, entryPoint) == 0x60 && offsetof(Knw0Work, results) == 0x78);
static_assert(offsetof(Knw0Work, advicePending) == 0x538 && offsetof(Knw0Work, gesture) == 0x65c);
template <class T> T Knw0Value(const void *data, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<const std::byte *>(data) + offset, sizeof(value));
    return value;
}
template <class T> void Knw0Value(void *data, std::size_t offset, T value) {
    std::memcpy(static_cast<std::byte *>(data) + offset, &value, sizeof(value));
}
inline void Knw0Advance(CompiledSequenceWork &work, std::uint16_t mode) {
    work.previousMode = work.mode;
    work.mode = mode;
    work.continueAfterWait = 1;
    work.continueLoop = 1;
}
inline void Knw0Wait(CompiledSequenceWork &work, std::uint16_t mode, std::uint16_t next) {
    work.mode = mode;
    work.nextMode = next;
    work.continueAfterWait = 1;
}
inline void Knw0Wait(CompiledSequenceWork &work, std::uint16_t mode, std::uint16_t next, std::int32_t delay, std::int32_t immediate = 1) {
    work.mode = mode;
    work.nextMode = next;
    work.delay = delay;
    work.continueAfterWait = immediate;
}
CompiledSequenceWork *Knw0Child(CompiledSequenceWork &, HLib::TaskCallback, shen_name, std::int32_t);
void Knw0Complete(CompiledSequenceWork &, bool result = false);
void Knw0KillSequence(CompiledSequenceWork *);
void Knw0QueueAuthor(shen_name);
void Knw0RestoreActors();
void Knw0ResetActors(std::int32_t);
void Knw0InitializeActors(void *);
void Knw0RootSequence(void *);
void Knw0StartPractice(void *);
void Knw0Introduction(void *);
void Knw0FirstLesson(void *);
void Knw0SecondLesson(void *);
void Knw0FinishPractice(void *);
void Knw0AwardMove(void *);
void Knw0Decision(void *);
void Knw0CapturePractice(void *);
void Knw0Feedback(void *);
void Knw0ExitPractice(void *);
void Knw0Success(void *);
void Knw0ShowLcd(Knw0Work &);
extern const ScriptAreaDefinition knw0ScriptDefinition;
} // namespace shenmue::s2::areas
