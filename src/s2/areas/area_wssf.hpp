#pragma once
#include "s2/area_event.hpp"
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
struct WssfMainWork {
    std::int8_t phase, step, result, reserved03;
    std::int8_t tier, attempts;
    std::int16_t timer, difficulty, latchedPower, power, reserved0e;
    float scale, endFrame;
    HLib::Task *task;
    HLib::Task *player;
    HLib::Task *partner;
    HLib::Task *stone;
    HLib::Task *spectators;
    std::uint64_t reserved40;
    std::uint32_t reserved48, reserved4c;
    WssfMainWork *self;
};
struct WssfChildWork {
    std::int8_t phase, step;
    std::int16_t timer;
    std::int8_t counter;
    std::byte reserved05[3];
    HLib::Task *task;
    HLib::Task *parent;
    float frame;
    std::int32_t variant;
    std::uint32_t reserved20, reserved24;
    WssfChildWork *self;
};
static_assert(sizeof(WssfMainWork) == 0x58);
static_assert(offsetof(WssfMainWork, difficulty) == 8);
static_assert(offsetof(WssfMainWork, power) == 0xc);
static_assert(offsetof(WssfMainWork, task) == 0x18);
static_assert(offsetof(WssfMainWork, stone) == 0x30);
static_assert(offsetof(WssfMainWork, self) == 0x50);
static_assert(sizeof(WssfChildWork) == 0x30);
static_assert(offsetof(WssfChildWork, parent) == 0x10);
static_assert(offsetof(WssfChildWork, frame) == 0x18);
static_assert(offsetof(WssfChildWork, self) == 0x28);
void WssfInit();
void WssfMain(std::uintptr_t argument);
void WssfMainCallback(void *data);
void WssfPlayerCallback(void *data);
void WssfPartnerCallback(void *data);
void WssfStoneCallback(void *data);
void WssfSpectatorCallback(void *data);
extern const ScriptAreaDefinition wssfScriptDefinition;
} // namespace shenmue::s2
