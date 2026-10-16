#pragma once
#include "s2/areas/area_bkw0.hpp"
#include <cstring>
namespace shenmue::s2::areas {
template <class T> T Bkw0SequenceValue(const CompiledSequenceWork &work, std::size_t offset) {
    T value;
    std::memcpy(&value, reinterpret_cast<const std::byte *>(&work) + offset, sizeof(value));
    return value;
}
template <class T> void Bkw0SequenceValue(CompiledSequenceWork &work, std::size_t offset, T value) {
    std::memcpy(reinterpret_cast<std::byte *>(&work) + offset, &value, sizeof(value));
}
inline void Bkw0Advance(CompiledSequenceWork &work, std::uint16_t mode) {
    work.previousMode = work.mode;
    work.mode = mode;
    work.continueAfterWait = 1;
    work.continueLoop = 1;
}
inline void Bkw0Wait(CompiledSequenceWork &work, std::uint16_t mode, std::uint16_t next) {
    work.mode = mode;
    work.nextMode = next;
    work.continueAfterWait = 1;
}
inline void Bkw0Wait(CompiledSequenceWork &work, std::uint16_t mode, std::uint16_t next, std::int32_t delay, std::int32_t immediate = 1) {
    work.mode = mode;
    work.nextMode = next;
    work.delay = delay;
    work.continueAfterWait = immediate;
}
inline CompiledSequenceWork *Bkw0Child(CompiledSequenceWork &work, HLib::TaskCallback callback, shen_name token, std::int32_t slot) {
    CompiledSequenceWork *child = QueueCompiledSequence(callback, token, work.event);
    if (!work.children[slot])
        work.children[slot] = child;
    child->parent = &work;
    child->childSlot = slot;
    child->reference = work.reference + 1;
    Bkw0SequenceValue(work, 0x19c, slot);
    Bkw0SequenceValue(work, 0x90 + 4 * slot, std::int32_t{0});
    return child;
}
inline void Bkw0Complete(CompiledSequenceWork &work, bool result = false) {
    if (work.parent) {
        Bkw0SequenceValue(*work.parent, 0x90 + 4 * work.childSlot, std::int32_t{1});
        if (result)
            Bkw0SequenceValue(*work.parent, 0xb8 + 4 * work.childSlot, Bkw0SequenceValue<std::int32_t>(work, 0x88));
    }
    RetireCompiledSequenceLinks(&work);
    HLib::DestroyCurrentTask();
}
inline void Bkw0KillSequence(CompiledSequenceWork *work) {
    if (work && work->task) {
        HLib::Task *task = work->task;
        RetireCompiledSequenceLinks(work);
        HLib::DestroyTask(task);
    }
}
inline constexpr shen_name BKW0_SEQUENCE{SHEN_NAME(SEQP)}, BKW0_START{SHEN_NAME(S000)}, BKW0_INTRO{SHEN_NAME(S010)};
inline constexpr shen_name BKW0_FINISH{SHEN_NAME(S100)}, BKW0_AWARD{SHEN_NAME(S200)}, BKW0_PRACTICE{SHEN_NAME(WDMA)};
inline constexpr shen_name BKW0_INITIALIZE{SHEN_NAME(IWDE)}, BKW0_LCD{SHEN_NAME(DLCD)};
inline constexpr shen_name BKW0_LOOK{SHEN_NAME(TRTS)}, BKW0_ATTACK{SHEN_NAME(BATT)}, BKW0_PRACTICE_FIGHT{SHEN_NAME(WDTP)};
struct Bkw0AuthorWork {
    HLib::Task *task;
    std::int32_t reserved08;
    shen_name author;
    std::int16_t phase;
    std::uint16_t reserved12;
    std::int32_t postFxBank;
};
static_assert(sizeof(Bkw0AuthorWork) == 0x18 && offsetof(Bkw0AuthorWork, phase) == 0x10);
inline constexpr shen_name BKW0_AUTHOR_INTRO{SHEN_NAME(AU_0)}, BKW0_AUTHOR_SUCCESS{SHEN_NAME(AU_1)}, BKW0_AUTHOR_FAILURE{SHEN_NAME(AU_2)};
void Bkw0QueueAuthor(shen_name author);
void Bkw0PracticeFeedbackSequence(void *);
void Bkw0PracticeExitSequence(void *);
void Bkw0ThrowOutcomeSequence(void *);
void Bkw0FinalThrowSequence(void *);
void Bkw0PracticeDismissalSequence(void *);
void Bkw0ShowPracticeLcd(Bkw0Work &);
} // namespace shenmue::s2::areas
