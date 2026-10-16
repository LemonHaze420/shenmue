#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/event_characters.hpp"
#include "s2/seqp_event.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct Bt40BoutWork {
    CompiledSequenceWork *sequence;
    HLib::Task *task;
    std::int32_t active, phase;
    std::byte reserved18[8];
    std::int32_t motions[16];
    std::int64_t mode;
    EventReadCharactersWork *characters;
    std::byte reserved70[8];
    std::int32_t result, difficulty, rank, retries, lastAdjustment;
    std::byte reserved8c[4];
};
static_assert(sizeof(Bt40BoutWork) == 0x90);
static_assert(offsetof(Bt40BoutWork, mode) == 0x60);
static_assert(offsetof(Bt40BoutWork, characters) == 0x68);
static_assert(offsetof(Bt40BoutWork, result) == 0x78);
void BT40_init_func();
void BT40_event_function(std::uintptr_t parameter);
void Bt40BoutCallback(void *parameter);
extern const ScriptAreaDefinition areaBT40ScriptDefinition;
} // namespace shenmue::s2
