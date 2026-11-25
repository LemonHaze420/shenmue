#pragma once

#include "s2/system_modules.hpp"
#if !SHENMUE_S2_GCC295
#include <array>
#include <cstddef>
#include <cstdint>
#endif

namespace shenmue { namespace s2 {

#if SHENMUE_S2_GCC295
namespace EmulatorGame {
typedef std::int32_t Type;
enum Value { OutRun, AfterBurner, HangOn, SpaceHarrier };
}
#else
enum class EmulatorGame : std::int32_t { OutRun, AfterBurner, HangOn, SpaceHarrier };
#endif
#if SHENMUE_S2_GCC295
namespace EmulatorState {
typedef std::int32_t Type;
enum Value {
#else
enum class EmulatorState : std::int32_t {
#endif
    StartOutRun = 0,
    StartAfterBurner = 1,
    StartHangOn = 2,
    StartHarrier = 3,
    SetEntryID = 10,
    GetEntryID = 11,
    GetScoreThresholdReached = 20,
    GetCompletionFlag = 21
};
#if SHENMUE_S2_GCC295
}
#endif

#pragma pack(push, 1)
// The mode and flags are unaligned QWORDs.
struct ArcadeSessionContext {
    std::int32_t result;
    std::int64_t mode;
    std::int32_t field0c;
    std::int32_t audioBootrom;
    std::int32_t entryId;
    std::uint32_t scoreBcd;
#if SHENMUE_S2_GCC295
    std::uint8_t initials[4];
#else
    std::array<std::uint8_t, 4> initials;
#endif
    std::uint64_t flags;
};
// Native save-owned records: the emulator admission packs score into BCD,
// and its result consumer unpacks it back into this 12-byte record.
struct ArcadeRankingRecord {
    std::uint32_t score;
#if SHENMUE_IS_DC
    shen_name initials;
#else
    std::array<std::uint8_t, 4> initials;
#endif
    std::uint32_t extra;
};
struct EmulatorInfoHeader {
    std::int32_t mode;
    std::int32_t audioBootrom;
#if SHENMUE_S2_GCC295
    EmulatorGame::Type game;
#else
    EmulatorGame game;
#endif
    std::int32_t result;
    std::uint32_t reserved10;
    std::int32_t exitDelay;
    std::int32_t entryId;
    std::uint32_t scoreBcd;
#if SHENMUE_S2_GCC295
    std::uint8_t initials[4];
#else
    std::array<std::uint8_t, 4> initials;
#endif
    std::uint64_t flags;
    std::uint32_t reserved2c;
};
#pragma pack(pop)
#if SHENMUE_S2_GCC295
#define SHENMUE_ARCADE_CHECK_NAME_(line) ArcadeLayoutCheck##line
#define SHENMUE_ARCADE_CHECK_NAME(line) SHENMUE_ARCADE_CHECK_NAME_(line)
#define SHENMUE_ARCADE_LAYOUT(condition) typedef char SHENMUE_ARCADE_CHECK_NAME(__LINE__)[(condition) ? 1 : -1]
#else
#define SHENMUE_ARCADE_LAYOUT(condition) static_assert(condition)
#endif
SHENMUE_ARCADE_LAYOUT(sizeof(ArcadeSessionContext) == 0x28);
SHENMUE_ARCADE_LAYOUT(offsetof(ArcadeSessionContext, mode) == 4);
SHENMUE_ARCADE_LAYOUT(offsetof(ArcadeSessionContext, entryId) == 0x14);
SHENMUE_ARCADE_LAYOUT(offsetof(ArcadeSessionContext, flags) == 0x20);
SHENMUE_ARCADE_LAYOUT(sizeof(ArcadeRankingRecord) == 12);
SHENMUE_ARCADE_LAYOUT(offsetof(ArcadeRankingRecord, initials) == 4);
SHENMUE_ARCADE_LAYOUT(offsetof(ArcadeRankingRecord, extra) == 8);
SHENMUE_ARCADE_LAYOUT(sizeof(EmulatorInfoHeader) == 0x30);
SHENMUE_ARCADE_LAYOUT(offsetof(EmulatorInfoHeader, flags) == 0x24);

} } // namespace shenmue::s2

#undef SHENMUE_ARCADE_LAYOUT
#if SHENMUE_S2_GCC295
#undef SHENMUE_ARCADE_CHECK_NAME
#undef SHENMUE_ARCADE_CHECK_NAME_
#endif
