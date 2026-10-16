#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name SLEEP_EVENT_TASK{SHEN_NAME(inn_)};


struct SleepTriggerBox {
    Vector3 minimum;
    float reserved0c;
    Vector4 reserved10;
    float reserved20;
    Vector3 maximum;
    Vector3 reserved30;
    std::uint32_t state;
};
static_assert(sizeof(SleepTriggerBox) == 0x40);
static_assert(offsetof(SleepTriggerBox, maximum) == 0x24);
static_assert(offsetof(SleepTriggerBox, state) == 0x3c);



struct SleepEventData {
    char package[32];
    shen_name actors[10];
    HLib::TaskCallback continuation;
    HLib::TaskCallback cancelled;
    std::byte reserved58[8];
    std::uint32_t pendingResources;
    std::int32_t subtitleFrames;
    std::uint32_t flags;
    shen_name nextScene;
    std::int32_t icon;
    std::int32_t nextEntry;
    std::int32_t savedFilmBars;
};
static_assert(offsetof(SleepEventData, actors) == 0x20);
static_assert(offsetof(SleepEventData, continuation) == 0x48);
static_assert(offsetof(SleepEventData, pendingResources) == 0x60);
static_assert(offsetof(SleepEventData, flags) == 0x68);
static_assert(offsetof(SleepEventData, savedFilmBars) == 0x78);

extern SleepEventData sleepEventData;
extern shen_name sleepCurrentAuthor;

std::uint32_t shenTrySleepTrigger(SleepTriggerBox *box);
void QueueSleepCallback(HLib::TaskCallback accepted, HLib::TaskCallback cancelled, std::int32_t expectedSchedule);
void QueueSleepPackageActors(const char *package, const shen_name *actors, HLib::TaskCallback continuation);
void FreeSleepPackageActors();
void HideSleepActor(shen_name actor);
void SleepAuthorWaitCallback(void *taskData);
void SleepMemoWaitCallback(void *taskData);
std::int32_t GetBadEndingState();
void SelectBadEndingState(HLib::TaskCallback continuation);
void QueueSleepSceneChange(shen_name scene, std::int32_t entry);

} // namespace shenmue::s2
