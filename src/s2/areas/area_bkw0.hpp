#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/seqp_event.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name BKW0_AREA{SHEN_NAME(BKW0)}, BKW0_TASK{SHEN_NAME(EVNT)}, BKW0_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name BKW0_RYO{SHEN_NAME(RYO_)}, BKW0_LKN{SHEN_NAME(LKN_)}, BKW0_BKN{SHEN_NAME(BKN_)}, BKW0_MYS{SHEN_NAME(MYS_)};
inline constexpr shen_name BKW0_DEKU{SHEN_NAME(DEKU)}, BKW0_RETURN_AREA{SHEN_NAME(WS00)};
inline constexpr shen_name BKW0_CROWD[]{SHEN_NAME(K04_), SHEN_NAME(K01_), SHEN_NAME(S04_), SHEN_NAME(S01_), SHEN_NAME(S13_), SHEN_NAME(S02_), SHEN_NAME(S14_),
    SHEN_NAME(S03_), SHEN_NAME(S07_), SHEN_NAME(K03_), SHEN_NAME(K02_), SHEN_NAME(S10_), SHEN_NAME(S12_), SHEN_NAME(S09_), SHEN_NAME(S11_), SHEN_NAME(S08_),
    SHEN_NAME(S06_), SHEN_NAME(S05_)};
extern const shen_name bkw0TrainingCharacters[23], bkw0BowCharacters[21];
struct Bkw0Work {
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
    std::byte fightResults[0x4c0];
    std::int32_t advicePending, timedAdvice, practiceWarning, practiceExpired;
    std::uint32_t repetitions, consecutiveRepetitions, previousRepetitions, attemptedMoves;
    std::int32_t firstFailures[10], repeatedFailures[10];
    std::int32_t reserved5a8, conversationActive, changeCamera, reserved5b4, restartMotion;
    std::uint16_t reserved5bc, reserved5be;
    CompiledSequenceWork *cameraSequence, *dialogueSequence, *timerSequence, *adviceSequence, *lcdSequence;
    std::int32_t practice, result, resultIndex, practiceCamera;
    std::uint32_t elapsed, adviceElapsed, attempts;
    std::int32_t repositionFrames[2];
    std::byte reserved60c[0x24];
    std::int32_t lcdFrames;
    std::uint32_t reserved634;
    const void *lcdBitmap;
    std::int32_t fightSuspended;
    shen_name opponent;
    std::int32_t characterIndex, characterLoading;
    std::uint8_t practiceReady, practiceBlocked;
    std::byte reserved652[0xa];
    std::uint8_t adviceFacingCycle;
    std::byte reserved65d[3];
    std::byte reserved660[8];
    std::int32_t differentThrows, repeatedThrows, consecutiveThrows, previousFailure, throwDirection;
    std::uint8_t throwDirections[4], finalPractice;
    std::byte reserved681[7];
};
static_assert(sizeof(Bkw0Work) == 0x688);
static_assert(offsetof(Bkw0Work, task) == 8 && offsetof(Bkw0Work, state) == 0x14 && offsetof(Bkw0Work, selector) == 0x60);
static_assert(offsetof(Bkw0Work, characters) == 0x68 && offsetof(Bkw0Work, fightResults) == 0x78 && offsetof(Bkw0Work, advicePending) == 0x538);
static_assert(offsetof(Bkw0Work, cameraSequence) == 0x5c0 && offsetof(Bkw0Work, lcdFrames) == 0x630 && offsetof(Bkw0Work, fightSuspended) == 0x640);
static_assert(offsetof(Bkw0Work, differentThrows) == 0x668 && offsetof(Bkw0Work, finalPractice) == 0x680);
extern const shen_name *bkw0Characters;
extern std::int32_t bkw0CrowdOffset, bkw0AdviceIssued;
extern std::uint32_t bkw0Effects[16];
void InitializeBkw0Event();
void RunBkw0Event(std::uintptr_t);
extern const ScriptAreaDefinition bkw0ScriptDefinition;
void Bkw0TrainingSequence(void *);
void Bkw0BowSequence(void *);
void Bkw0InitializeTrainingSequence(void *);
void Bkw0PlaceCrowd();
void Bkw0TrainCrowd(std::int32_t includeSisters, std::int32_t fixed);
void Bkw0RandomizeCrowdPractice();
void Bkw0IdleCrowd();
void Bkw0LookSequence(void *);
void Bkw0PracticeSequence(void *);
void Bkw0ResetReflectors();
void Bkw0CreateSmoke();
void Bkw0ResetActors(std::int32_t practice);
void Bkw0ResetPracticeOpponentAngle();
void Bkw0PracticeDecisionSequence(void *);
void Bkw0StartTrainingSequence(void *);
void Bkw0IntroductionSequence(void *);
void Bkw0FinishTrainingSequence(void *);
void Bkw0AwardMoveSequence(void *);
void Bkw0AuthorCallback(void *);
} // namespace shenmue::s2::areas
