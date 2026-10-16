#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name GATE_EVENT_TASK{SHEN_NAME(gate)};
inline constexpr shen_name GATE_WHITE_TASK{SHEN_NAME(whit)};
struct GateEventWork {
    HLib::Task *task;
    std::uint32_t state, entry;
    std::byte reserved10[0x10];
    float offsets[6];
    std::byte reserved38[0x48];
    const char *voices[8];
    std::int32_t durations[8], cameras[8];
    std::byte reserved100[8];
};
static_assert(sizeof(GateEventWork) == 0x108);
static_assert(offsetof(GateEventWork, offsets) == 0x20);
static_assert(offsetof(GateEventWork, voices) == 0x80);
static_assert(offsetof(GateEventWork, durations) == 0xc0);
static_assert(offsetof(GateEventWork, cameras) == 0xe0);
void GATE_init_func();
void GATE_main_func(std::uintptr_t entry);
void GateEventCallback(void *);
void GateAuthorWhiteCallback(void *);
void GateAuthorWhiteCompleteCallback(void *);
extern const ScriptAreaDefinition areaGATEScriptDefinition;
} // namespace shenmue::s2
