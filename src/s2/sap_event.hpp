#pragma once
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr shen_name SAP_PLAYER{SHEN_NAME(RYO_)};
struct SapEventData {
    char packageName[9];
    char authorBank[11];
    shen_name characters[11];
    HLib::TaskCallback nextCallback;
    std::byte reserved48[0x10];
    std::uint32_t pendingMask;
    std::uint32_t reserved5c;
    std::uint32_t authorFlags;
    std::byte reserved64[8];
    std::int32_t savedFilmBars;
};
static_assert(sizeof(SapEventData) == 0x70);
static_assert(offsetof(SapEventData, authorBank) == 9);
static_assert(offsetof(SapEventData, characters) == 0x14);
static_assert(offsetof(SapEventData, nextCallback) == 0x40);
static_assert(offsetof(SapEventData, pendingMask) == 0x58);
static_assert(offsetof(SapEventData, authorFlags) == 0x60);
static_assert(offsetof(SapEventData, savedFilmBars) == 0x6c);
extern SapEventData sapEventData;
void InitializeSapAuthorEvent(const char *authorBank, std::uintptr_t unused, const shen_name *characters, HLib::TaskCallback next);
void SapPackageLoadCallback(void *);
void SapPackageWaitCallback(void *);
void SapAuthorWaitCallback(void *);
void ResetSapAuthorCharacter(shen_name character);
} // namespace shenmue::s2
