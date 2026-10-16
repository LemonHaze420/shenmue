#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct SlotRootWork {
    SlotRootWork *self;
    HLib::Task *task;
    HLib::Task *player;
    HLib::Task *machine;
    HLib::Task *drum;
    HLib::Task *cylinder;
    std::int16_t timer;
    std::int8_t state, choice, debug;
    std::byte reserved35[3];
    Vector3 playerPosition;
    Vector3i playerRotation;
    std::int32_t machineIndex, reserved54;
};
struct SlotPlayerWork {
    SlotPlayerWork *self;
    HLib::Task *task;
    HLib::Task *root;
    std::int16_t timer;
    std::int8_t state, phase, authorState;
    std::byte reserved1d[3];
    SlotRootWork *rootWork;
    std::int32_t coins, reserved2c;
};
struct SlotMachineWork {
    SlotMachineWork *self;
    HLib::Task *task;
    HLib::Task *root;
    std::int16_t timer;
    std::int8_t state, phase;
    Vector3 lamps[5];
};
struct SlotReelWork {
    SlotReelWork *self;
    std::byte reserved08[16];
    std::int32_t angle, speed;
    std::int16_t timer, stop;
    std::int8_t state, phase;
    std::byte reserved26[2];
};
struct SlotDrumWork {
    SlotDrumWork *self;
    SlotReelWork *reels[3];
    HLib::Task *task;
    HLib::Task *root;
    std::int16_t timer;
    std::int8_t state, phase;
    std::int32_t repeatOutcome, outcome, payout, credits, wager, reserved48, machineIndex;
};
struct SlotLampWork {
    SlotLampWork *self;
    std::byte reserved08[16];
    std::int32_t angle, speed;
    std::int16_t timer;
    std::int8_t state, phase;
    std::byte reserved24[4];
};
struct SlotCylinderWork {
    SlotCylinderWork *self;
    SlotLampWork *lamps[3];
    HLib::Task *task;
    HLib::Task *root;
    std::int16_t timer;
    std::int8_t state, phase;
    std::byte reserved34[4];
};
static_assert(sizeof(SlotRootWork) == 0x58 && offsetof(SlotRootWork, playerPosition) == 0x38 && offsetof(SlotRootWork, machineIndex) == 0x50);
static_assert(sizeof(SlotPlayerWork) == 0x30 && offsetof(SlotPlayerWork, rootWork) == 0x20 && offsetof(SlotPlayerWork, authorState) == 0x1c);
static_assert(sizeof(SlotMachineWork) == 0x58 && offsetof(SlotMachineWork, lamps) == 0x1c);
static_assert(sizeof(SlotReelWork) == 0x28 && offsetof(SlotReelWork, stop) == 0x22 && offsetof(SlotReelWork, state) == 0x24);
static_assert(sizeof(SlotDrumWork) == 0x50 && offsetof(SlotDrumWork, repeatOutcome) == 0x34 && offsetof(SlotDrumWork, credits) == 0x40);
static_assert(sizeof(SlotLampWork) == 0x28 && offsetof(SlotLampWork, state) == 0x22);
static_assert(sizeof(SlotCylinderWork) == 0x38 && offsetof(SlotCylinderWork, state) == 0x32);
void SlotInitializationNullsub();
void DebugFileLogNullsub(const char *source, int line, const char *format, ...);
void DebugScreenLogNullsub(int x, int y, const char *format, ...);
void QueueSlot(std::uintptr_t argument);
extern const ScriptAreaDefinition slotScriptDefinition;
} // namespace shenmue::s2
