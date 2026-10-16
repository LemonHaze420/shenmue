#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/event_characters.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct AlleyBattleWork {
    HLib::Task *task;
    std::uint64_t field08;
    std::int32_t field10;
    std::byte reserved14[0x24];
    std::uint32_t phase;
    std::int32_t field3c, field40;
    std::byte reserved44[0x3c];
    std::int32_t npcBatchCount;
    std::uint32_t reserved84;
    EventReadCharactersWork *characters;
    std::int32_t fadeOutFrames, fadeInFrames;
    std::byte reserved98[8];
    std::uint8_t reservedA0, delay, entry, reservedA3;
    std::uint32_t elapsed;
    std::int32_t colours[4][6];
    std::int8_t fadeOutSeconds, fadeInSeconds;
    std::byte reserved10a[2];
    std::uint32_t skyColour, cloudColour;
    float colourBlend[16];
    std::uint32_t outroFrames;
};
static_assert(sizeof(AlleyBattleWork) == 0x158);
static_assert(offsetof(AlleyBattleWork, phase) == 0x38);
static_assert(offsetof(AlleyBattleWork, characters) == 0x88);
static_assert(offsetof(AlleyBattleWork, colours) == 0xa8);
static_assert(offsetof(AlleyBattleWork, colourBlend) == 0x114);
static_assert(offsetof(AlleyBattleWork, outroFrames) == 0x154);
void ABLB_init_func();
void ABLB_main_func(std::uintptr_t);
void AlleyBattleCallback(void *);
extern const ScriptAreaDefinition areaABLBScriptDefinition;
} // namespace shenmue::s2
