#pragma once
#include "s1/task/scen_internal.hpp"

namespace shenmue::s1::scen_detail {

struct TimedHelpDestination {
    std::int16_t helpToken;
    char mapId[4];
    std::uint16_t reserved06;
    std::uint16_t entryId;
};

static_assert(sizeof(TimedHelpDestination) == 10);

struct TimedHelpScheduleEntry {
    const TimedHelpDestination *destination;
    std::uint8_t schedule[8];
    std::uint16_t scheduleWord;
};

inline constexpr TimedHelpDestination kTimedHelpJU00{.helpToken = 0x37, .mapId = {'J', 'U', '0', '0'}, .reserved06 = 0, .entryId = 1};

inline constexpr TimedHelpDestination kTimedHelpJD00{.helpToken = 0x36, .mapId = {'J', 'D', '0', '0'}, .reserved06 = 0, .entryId = 2};

inline constexpr TimedHelpDestination kTimedHelpD000{.helpToken = 0x35, .mapId = {'D', '0', '0', '0'}, .reserved06 = 0, .entryId = 1};

inline constexpr TimedHelpDestination kTimedHelpJHD0{.helpToken = 0x38, .mapId = {'J', 'H', 'D', '0'}, .reserved06 = 0, .entryId = 2};

inline constexpr TimedHelpScheduleEntry kTimedHelpToJU00{.destination = &kTimedHelpJU00, .schedule = {0, 0, 0, 0, 0, 5, 0, 0}, .scheduleWord = 0};

inline constexpr TimedHelpScheduleEntry kTimedHelpToJD00{.destination = &kTimedHelpJD00, .schedule = {0, 0, 0, 0, 0, 10, 0, 0}, .scheduleWord = 0};

inline constexpr TimedHelpScheduleEntry kTimedHelpToD000{.destination = &kTimedHelpD000, .schedule = {0, 0, 0, 0, 0, 15, 0, 0}, .scheduleWord = 0};

inline constexpr TimedHelpScheduleEntry kTimedHelpToJHD0{.destination = &kTimedHelpJHD0, .schedule = {0, 0, 0, 0, 0, 0, 0, 0}, .scheduleWord = 0};


inline constexpr std::array<const TimedHelpScheduleEntry *, 16> kTimedHelpSchedule{
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &kTimedHelpToJU00,
    &kTimedHelpToJHD0,
    nullptr,
    nullptr,
    &kTimedHelpToJU00,
    &kTimedHelpToJD00,
    &kTimedHelpToJHD0,
    &kTimedHelpToD000,
    &kTimedHelpToJU00,
    &kTimedHelpToJD00,
    &kTimedHelpToJHD0,
};

#pragma pack(push, 1)
struct TimedTransitionEntry {
    TimedTransitionState state{};
    std::uint8_t flags{};
    std::uint8_t reserved02[2]{};
    std::int8_t sceneId{};
    std::int8_t entryId{};
    std::uint8_t reserved06[2]{};
    std::uint32_t areaId{};
    std::uint16_t conditionWords[8]{};
    std::int16_t startHHMM{};
    std::int16_t endHHMM{};
    shen_name boundAreaId{};
};
#pragma pack(pop)

static_assert(sizeof(TimedTransitionEntry) == 0x24);

static_assert(offsetof(TimedTransitionEntry, areaId) == 0x08);

static_assert(offsetof(TimedTransitionEntry, conditionWords) == 0x0c);

static_assert(offsetof(TimedTransitionEntry, startHHMM) == 0x1c);

static_assert(offsetof(TimedTransitionEntry, boundAreaId) == 0x20);

inline constexpr std::array<std::uint8_t, 13> kTimedTransitionMonthDays{{31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

} // namespace shenmue::s1::scen_detail
