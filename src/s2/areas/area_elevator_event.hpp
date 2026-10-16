#pragma once
#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name ELEVATOR_EVENT_TASK = SHEN_NAME(elvM);
struct ElevatorEventDestination {
    std::int16_t floor;
    std::uint16_t flags;
    shen_name area;
    std::int32_t entry;
};
struct ElevatorEventFloorTable {
    shen_name elevator;
    std::array<ElevatorEventDestination, 10> destinations;
};
struct ElevatorEventButton {
    std::uint32_t flags;
    shen_name character;
    std::int32_t floor;
    shen_name area;
    std::int32_t entry;
};
struct ElevatorEventState {
    std::uint32_t flags;
    std::int32_t phase;
    shen_name elevator;
    Vector3 position;
    Vector3i rotation;
    std::int32_t upperFloor;
    std::int32_t lowerFloor;
    std::int32_t sourceFloor;
    std::int32_t direction;
    std::int32_t destination;
    std::int32_t ticks;
    std::int32_t selection;
    std::int32_t timeout;
    std::int32_t reserved44;
    const ElevatorEventFloorTable *floorTable;
    std::array<ElevatorEventButton, 10> buttons;
};
static_assert(sizeof(ElevatorEventDestination) == 12);
static_assert(sizeof(ElevatorEventFloorTable) == 124);
static_assert(sizeof(ElevatorEventButton) == 20);
static_assert(sizeof(ElevatorEventState) == 280);
static_assert(offsetof(ElevatorEventState, position) == 12);
static_assert(offsetof(ElevatorEventState, rotation) == 24);
static_assert(offsetof(ElevatorEventState, ticks) == 0x38);
static_assert(offsetof(ElevatorEventState, selection) == 0x3c);
static_assert(offsetof(ElevatorEventState, timeout) == 64);
static_assert(offsetof(ElevatorEventState, floorTable) == 72);
static_assert(offsetof(ElevatorEventState, buttons) == 80);
void ELV2_init_func();
void ELV2_main_func(std::uintptr_t elevator);
void ELVT_init_func();
void ELVT_main_func(std::uintptr_t elevator);
extern const ScriptAreaDefinition areaELV2ScriptDefinition;
extern const ScriptAreaDefinition areaELVTScriptDefinition;
} // namespace shenmue::s2
