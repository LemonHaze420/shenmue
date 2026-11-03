#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"

typedef struct GetoEventWork {
    shen_name prop, searchTarget;
    int enabled;
    HLibTask *caller;
} GetoEventWork;
void GetoMain(const shen_name *parameters);
void GetoLoop(void *taskData);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/hlib.hpp"

namespace shenmue::s2 {

inline constexpr shen_name GETO_AREA{SHEN_NAME(GETO)};


struct GetoEventWork {
    std::uint64_t prop;
    std::uint64_t searchTarget;
    std::uint64_t enabled;
    HLib::Task *caller;
};
static_assert(sizeof(GetoEventWork) == 0x20);
static_assert(offsetof(GetoEventWork, searchTarget) == 0x08 && offsetof(GetoEventWork, enabled) == 0x10);
static_assert(offsetof(GetoEventWork, caller) == 0x18);

void GetoInit();
void GetoMain(std::uintptr_t argument);
void GetoLoop(void *taskData);
extern const ScriptAreaDefinition getoScriptDefinition;

} // namespace shenmue::s2
#endif
