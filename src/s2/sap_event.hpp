#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#define SAP_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define SAP_TASK SHEN_NAME4('s', 'a', 'p', '_')
typedef struct SapEventData {
    char packageName[9], authorBank[11];
    shen_name characters[10];
    HLibTaskCallback nextCallback;
    unsigned int reserved40[2], pendingMask, reserved4c, authorFlags;
    shen_name nextArea;
    int nextEntry, savedFilmBars;
} SapEventData;
extern SapEventData sapEventData;
void InitializeSapAuthorEvent(const char *authorBank, const char *package, const shen_name *characters, HLibTaskCallback next);
void SelectSapAuthor(const shen_name *authors, int count, HLibTaskCallback next, unsigned int flags);
void SapPackageLoadCallback(void *taskData);
void SapPackageWaitCallback(void *taskData);
void SapAuthorWaitCallback(void *taskData);
void SapSceneLoadCallback(void *taskData);
void ResetSapAuthorCharacter(shen_name character);
#else
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

#endif
