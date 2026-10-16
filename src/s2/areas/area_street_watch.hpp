#pragma once
#include "s2/event_characters.hpp"
#include "s2/street_fight.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name STREET_WATCH_TASK{SHEN_NAME(EVNT)}, STREET_WATCH_RYO{SHEN_NAME(RYO_)}, STREET_WATCH_ANNOUNCER{SHEN_NAME(CHN_)};
struct StreetWatchWork {
    HLib::Task *task;
    std::byte reserved08[0x30];
    std::int32_t state;
    std::byte reserved3c[0x4c];
    EventReadCharactersWork *characters;
    std::byte reserved90[8];
    HLib::Task *audience;
    std::byte reserveda0[0xe0];
};
static_assert(sizeof(StreetWatchWork) == 0x180);
static_assert(offsetof(StreetWatchWork, state) == 0x38);
static_assert(offsetof(StreetWatchWork, characters) == 0x88);
static_assert(offsetof(StreetWatchWork, audience) == 0x98);
struct StreetWatchState {
    std::int32_t initialized;
    StreetFightStartParameters selection;
    std::int32_t help, speechStarted, battleCount, announcerLoaded, iconPrepared;
    std::int32_t restart, busy;
    std::uint8_t advice, motion;
    const char *package;
    Vector3 centre, approachCentre, rooftopCentre;
    shen_name fighter, partner;
    const shen_name *withAnnouncer;
    const shen_name *withoutAnnouncer;
    std::int32_t fighterStyle, partnerStyle;
    std::int32_t formationAssignments;
    float approachRadiusSquared;
    void (*prepareParentedActors)();
    bool freeAnnouncerPackage, wideResult;
    void (*queueInitialSleep)();
    void (*queueLostSyunen)();
};
HLib::Task *QueueStreetWatch(StreetWatchState &state, HLib::TaskCallback minigame, HLib::TaskCallback street);
void StreetWatchMinigameCallback(void *payload, StreetWatchState &state);
void StreetWatchMainCallback(void *payload, StreetWatchState &state);
void StartStreetWatchFight(StreetWatchState &state);
void PositionStreetWatchAnnouncer();
std::uint8_t GetStreetWatchHelpTopic();
} // namespace shenmue::s2::areas
