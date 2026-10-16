#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct BellEventWork {
    std::uint64_t reserved00;
    HLib::Task *task;
    std::int32_t motion;
    std::uint32_t phase;
    std::uint32_t elapsed;
    std::uint32_t command;
    std::uint32_t renRouteCompletion;
    std::uint8_t busy;
    std::uint8_t escapeTalks;
    std::uint8_t conversations;
    std::uint8_t reserved27;
};
static_assert(sizeof(BellEventWork) == 0x28);
static_assert(offsetof(BellEventWork, task) == 8);
static_assert(offsetof(BellEventWork, command) == 0x1c);
static_assert(offsetof(BellEventWork, busy) == 0x24);
void BELL_init_func();
void BELL_main_func(std::uintptr_t argument);
void BellLoadNpcCallback(void *);
void BellInteractionInitCallback(void *);
void BellInteractionCallback(void *);
void BellRingCallback(void *);
void BellExitCallback(void *);
extern const ScriptAreaDefinition areaBELLScriptDefinition;
} // namespace shenmue::s2
