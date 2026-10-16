#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include <cstddef>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
struct WtkqWork {
    HLib::Task *self;
    std::uint16_t phase, step;
    std::uint32_t reserved0c;
    EventReadCharactersWork *characters;
    std::int32_t qteElapsed;
    std::uint32_t reserved1c;
    std::uint16_t qteStatus, reserved22;
    std::int32_t qteSucceeded;
    std::int32_t retried;
    std::uint32_t reserved2c;
};
static_assert(sizeof(WtkqWork) == 0x30);
static_assert(offsetof(WtkqWork, qteElapsed) == 0x18 && offsetof(WtkqWork, retried) == 0x28);
void WtkqInit();
void WtkqMain(std::uintptr_t);
void QueueWtkq(void *);
void WtkqMainCallback(void *);
void WtkqEndCallback(void *);
std::int32_t WtkqQteSuccess();
std::int32_t WtkqQteFailure();
extern const ScriptAreaDefinition wtkqScriptDefinition;
} // namespace shenmue::s2
