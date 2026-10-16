#pragma once
#include "s2/hlib.hpp"
#include "common/types.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
struct BookScenarioData {
    char package[9];
    char authorBank[11];
    shen_name actors[11];
    HLib::TaskCallback continuation;
    std::byte reserved48[16];
    std::uint32_t pendingResources;
    std::int32_t subtitleFrames;
    std::uint32_t flags;
    std::byte reserved64[8];
    std::int32_t savedFilmBars;
};
static_assert(sizeof(BookScenarioData) == 0x70);
static_assert(offsetof(BookScenarioData, authorBank) == 9 && offsetof(BookScenarioData, actors) == 0x14);
static_assert(offsetof(BookScenarioData, continuation) == 0x40 && offsetof(BookScenarioData, pendingResources) == 0x58);
static_assert(offsetof(BookScenarioData, flags) == 0x60 && offsetof(BookScenarioData, savedFilmBars) == 0x6c);
extern BookScenarioData bookScenarioData;
void QueueBookScenarioActors(const char *authorBank, const shen_name *actors, HLib::TaskCallback continuation);
void HideBookScenarioActor(shen_name actor);
void BookScenarioPackageRetryCallback(void *);
void BookScenarioPackageWaitCallback(void *);
void BookScenarioAuthorWaitCallback(void *);
} // namespace shenmue::s2::areas
