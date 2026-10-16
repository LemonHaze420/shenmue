#pragma once
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name SL_PLAYER{SHEN_NAME(RYO_)}, SL_BAD_END_AUTHOR{SHEN_NAME(SLPJ)}, SL_SNAPSHOT_AUTHOR{SHEN_NAME(SLP2)};
extern shen_name slCurrentAuthor;
struct SlEventData {
    char packageName[32];
    shen_name characters[10];
    HLib::TaskCallback nextCallback;
    std::byte reserved50[0x10];
    std::uint32_t pendingMask;
    std::uint32_t reserved64;
    std::uint32_t authorFlags;
    std::byte reserved6c[0x0c];
    std::int32_t savedFilmBars;
    std::uint32_t reserved7c;
};
static_assert(sizeof(SlEventData) == 0x80);
static_assert(offsetof(SlEventData, characters) == 0x20);
static_assert(offsetof(SlEventData, nextCallback) == 0x48);
static_assert(offsetof(SlEventData, pendingMask) == 0x60);
static_assert(offsetof(SlEventData, authorFlags) == 0x68);
static_assert(offsetof(SlEventData, savedFilmBars) == 0x78);
extern SlEventData slEventData;
void SlPackageWaitCallback(void *);
void SlAuthorWaitCallback(void *);
void ResetSlAuthorCharacter(shen_name character);
} // namespace shenmue::s2
