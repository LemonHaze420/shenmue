#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include <cstddef>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
struct WrsqWork {
    HLib::Task *self;
    std::uint16_t phase, step;
    std::uint32_t reserved0c;
    EventReadCharactersWork *characters;
    std::uint8_t won;
    std::byte reserved19[7];
};
static_assert(sizeof(WrsqWork) == 0x20);
static_assert(offsetof(WrsqWork, characters) == 0x10 && offsetof(WrsqWork, won) == 0x18);
void WrsqInit();
void WrsqMain(std::uintptr_t);
void QueueWrsq(void *);
void WrsqMainCallback(void *);
void WrsqEndCallback(void *);
void WrsqLoseAuthorCallback(void *);
extern const ScriptAreaDefinition wrsqScriptDefinition;
} // namespace shenmue::s2
