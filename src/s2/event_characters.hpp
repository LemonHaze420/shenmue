#pragma once

#include "s2/hlib.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name EVENT_CHARACTER_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name EVENT_CHARACTER_COMPONENT{SHEN_NAME(CEWP)};
inline constexpr shen_name EVENT_CHARACTER_INVALID{0xffffffffU};
inline constexpr std::uint16_t EVENT_CHARACTER_STAND_MOTION = 0xf003;

struct EventReadCharacterRecord {
    std::int32_t state;
    std::int32_t newlyLoaded;
    shen_name character;
    shen_name previousBase;
    std::int32_t restoreGravity;
    std::int32_t restoreHeightGravity;
    std::uint32_t reserved18;
    std::uint32_t motionRootMode;
};

struct EventReadCharactersWork {
    std::array<EventReadCharacterRecord, 30> characters;
    std::byte reserved3c0[0x80];
    HLib::Task *task;
    HLib::Task *parentTask;
    const shen_name *names;
    std::int32_t characterIndex;
    std::int32_t delayFrames;
    std::int32_t maxNpc;
    std::uint32_t reserved464;
    std::uint64_t needsReload;
};

static_assert(sizeof(EventReadCharacterRecord) == 0x20);
static_assert(offsetof(EventReadCharacterRecord, motionRootMode) == 0x1c);
static_assert(sizeof(EventReadCharactersWork) == 0x470);
static_assert(offsetof(EventReadCharactersWork, task) == 0x440);
static_assert(offsetof(EventReadCharactersWork, names) == 0x450);
static_assert(offsetof(EventReadCharactersWork, characterIndex) == 0x458);
static_assert(offsetof(EventReadCharactersWork, needsReload) == 0x468);

void AT_SetLocalBaseChara(shen_name character, shen_name base);
shen_name EV_SCEN_FindChar_Wrapper(shen_name character);
EventReadCharactersWork *Queue_EVT_AT_ReadChr(HLib::Task *parent, const shen_name *names);
void EVT_AT_ReadChr_Callback(void *taskData);
std::int64_t LoadOneNpcAndAdvance(EventReadCharactersWork *work, shen_name character);
std::int64_t RestoreEventCharacter(EventReadCharactersWork *work);
void ET_AT_KillWork(EventReadCharactersWork *work);

struct AuthoredPluralControlEntry {
    std::int32_t frame;
    shen_name character;
    std::int32_t control;
};
struct AuthoredPluralControlSaved {
    shen_name character;
    std::int32_t control;
};
struct AuthoredPluralControlWork {
    HLib::Task *task;
    std::uint8_t state;
    std::byte reserved09[7];
    const AuthoredPluralControlEntry *entries;
    std::uint8_t count;
    std::byte reserved19[3];
    AuthoredPluralControlSaved saved[8];
    std::byte reserved5c[4];
};
static_assert(sizeof(AuthoredPluralControlEntry) == 0xc);
static_assert(sizeof(AuthoredPluralControlSaved) == 8);
static_assert(sizeof(AuthoredPluralControlWork) == 0x60);
static_assert(offsetof(AuthoredPluralControlWork, entries) == 0x10);
static_assert(offsetof(AuthoredPluralControlWork, saved) == 0x1c);
AuthoredPluralControlWork *QueueAuthoredPluralControl(const AuthoredPluralControlEntry *entries);
void AuthoredPluralControlCallback(void *taskData);

} // namespace shenmue::s2
