#pragma once
#include "s2/area_event.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/seqp_event.hpp"
#include <cstring>
namespace shenmue::s2::areas {
inline constexpr shen_name SYW0_AREA{SHEN_NAME(SYW0)}, SYW0_TASK{SHEN_NAME(EVNT)}, SYW0_SEQUENCE{SHEN_NAME(SEQP)};
inline constexpr shen_name SYW0_RYO{SHEN_NAME(RYO_)}, SYW0_SYE{SHEN_NAME(SYE_)}, SYW0_KNK{SHEN_NAME(KNK_)};
inline constexpr shen_name SYW0_KSY{SHEN_NAME(KSY_)}, SYW0_KSM{SHEN_NAME(KSM_)}, SYW0_DEKU{SHEN_NAME(DEKU)};
inline constexpr shen_name SYW0_BASE{SHEN_NAME(BASE)}, SYW0_INVALID{0xffffffffU}, SYW0_GRID{SHEN_NAME(0000)};
inline constexpr shen_name SYW0_NIGHT{SHEN_NAME(YORU)}, SYW0_NIGHT_AUTHOR{SHEN_NAME(YR_0)}, SYW0_LIGHT{SHEN_NAME(L_YR)};
inline constexpr shen_name SYW0_RETURN{SHEN_NAME(0142)}, SYW0_NEXT_SCENE{SHEN_NAME(0200)};
inline constexpr shen_name SYW0_CHARACTERS[]{SYW0_RYO, SYW0_SYE, SYW0_KNK, SYW0_KSY, SYW0_KSM, SYW0_INVALID};
inline constexpr shen_name SYW0_AUTHOR_CHARACTERS[]{SYW0_SYE, SYW0_KNK, SYW0_KSM, SYW0_KSY, SYW0_INVALID};
inline constexpr shen_name SYW0_AUTHORS[]{
    SHEN_NAME(AU_0), SHEN_NAME(AU_1), SHEN_NAME(AU_2), SHEN_NAME(AU_3), SHEN_NAME(AU_4), SHEN_NAME(AU_5), SHEN_NAME(AU_6)};


struct Syw0Work {
    CompiledSequenceWork *sequence;
    HLib::Task *task;
    std::int32_t active, state, frames;
    std::uint16_t camera, reserved1e;
    std::uint32_t motions[16];
    std::int32_t selector;
    std::uint32_t reserved64;
    EventReadCharactersWork *characters;
    std::byte storage70[0x618];
};
static_assert(sizeof(Syw0Work) == 0x688 && offsetof(Syw0Work, state) == 0x14 && offsetof(Syw0Work, selector) == 0x60);
static_assert(offsetof(Syw0Work, characters) == 0x68 && offsetof(Syw0Work, storage70) == 0x70);
struct Syw0AuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name author;
    std::uint16_t phase, reserved12;
    std::int32_t postFxBank;
};
struct Syw0AuthorLoopWork {
    Syw0AuthorWork *parent;
    std::uint16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(Syw0AuthorWork) == 0x18 && offsetof(Syw0AuthorWork, phase) == 0x10);
static_assert(sizeof(Syw0AuthorLoopWork) == 0x10 && offsetof(Syw0AuthorLoopWork, frame) == 8);
template <class T> T Syw0Value(const void *data, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<const std::byte *>(data) + offset, sizeof(value));
    return value;
}
template <class T> void Syw0Value(void *data, std::size_t offset, T value) {
    std::memcpy(static_cast<std::byte *>(data) + offset, &value, sizeof(value));
}
inline void Syw0Advance(CompiledSequenceWork &work, std::uint16_t mode) {
    work.previousMode = work.mode;
    work.mode = mode;
    work.continueAfterWait = 1;
    work.continueLoop = 1;
}
inline void Syw0Wait(CompiledSequenceWork &work, std::uint16_t mode, std::uint16_t next, std::int32_t delay = 0, std::int32_t immediate = 1) {
    work.mode = mode;
    work.nextMode = next;
    work.delay = delay;
    work.continueAfterWait = immediate;
}
CompiledSequenceWork *Syw0Child(CompiledSequenceWork &, HLib::TaskCallback, shen_name, std::int32_t);
void Syw0Complete(CompiledSequenceWork &, bool result = false);
void Syw0KillSequence(CompiledSequenceWork *);
void Syw0TrainingSequence(void *);
void Syw0SparringSequence(void *);
void Syw0FirstDemonstrationSequence(void *);
void Syw0SecondDemonstrationSequence(void *);
void Syw0PracticeSequence(void *);
void Syw0PracticeDecisionSequence(void *);
void Syw0GateSequence(void *);
void Syw0AwardSequence(void *);
void Syw0NightSequence(void *);
void Syw0QueueAuthor(shen_name);
void Syw0PrepareAuthorCharacters();
void Syw0RestoreTrainingCharacters();
void Syw0CreateFire();
void Syw0CreateSmoke();
void Syw0PlaceTeacher(std::int32_t);
void Syw0ResetPracticeActors(std::int32_t);
void Syw0BeginTeacherGesture(CompiledSequenceWork &);
void Syw0EndTeacherGesture();
void Syw0ExitPracticeSequence(void *);
void Syw0WeightFeedbackSequence(void *);
void Syw0ForceFeedbackSequence(void *);
void Syw0GateFeedbackSequence(void *);
void Syw0WeightSuccessSequence(void *);
void Syw0ForceSuccessSequence(void *);
void Syw0GateSuccessSequence(void *);
HLib::TaskCallback Syw0SelectWeightFeedback(CompiledSequenceWork &);
HLib::TaskCallback Syw0SelectForceFeedback(CompiledSequenceWork &);
extern std::uint64_t syw0SmokeEffects[8];
extern std::uint32_t syw0FireEffect;
extern DateTime syw0SavedDateTime;
extern const ScriptAreaDefinition syw0ScriptDefinition;
} // namespace shenmue::s2::areas
