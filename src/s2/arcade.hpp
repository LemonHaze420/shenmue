#pragma once

#include "s2/system_modules.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

enum class EmulatorGame : std::int32_t { OutRun, AfterBurner, HangOn, SpaceHarrier };
enum class EmulatorState : std::int32_t {
    StartOutRun = 0,
    StartAfterBurner = 1,
    StartHangOn = 2,
    StartHarrier = 3,
    SetEntryID = 10,
    GetEntryID = 11,
    GetScoreThresholdReached = 20,
    GetCompletionFlag = 21
};

#pragma pack(push, 1)

struct ArcadeSessionContext {
    std::int32_t result;
    std::int64_t mode;
    std::int32_t field0c;
    std::int32_t audioBootrom;
    std::int32_t entryId;
    std::uint32_t scoreBcd;
    std::array<std::uint8_t, 4> initials;
    std::uint64_t flags;
};


struct ArcadeRankingRecord {
    std::uint32_t score;
    std::array<std::uint8_t, 4> initials;
    std::uint32_t extra;
};
struct EmulatorInfoHeader {
    std::int32_t mode;
    std::int32_t audioBootrom;
    EmulatorGame game;
    std::int32_t result;
    std::uint32_t reserved10;
    std::int32_t exitDelay;
    std::int32_t entryId;
    std::uint32_t scoreBcd;
    std::array<std::uint8_t, 4> initials;
    std::uint64_t flags;
    std::uint32_t reserved2c;
};
#pragma pack(pop)
static_assert(sizeof(ArcadeSessionContext) == 0x28);
static_assert(offsetof(ArcadeSessionContext, mode) == 4);
static_assert(offsetof(ArcadeSessionContext, entryId) == 0x14);
static_assert(offsetof(ArcadeSessionContext, flags) == 0x20);
static_assert(sizeof(ArcadeRankingRecord) == 12);
static_assert(sizeof(EmulatorInfoHeader) == 0x30);
static_assert(offsetof(EmulatorInfoHeader, flags) == 0x24);

} // namespace shenmue::s2
