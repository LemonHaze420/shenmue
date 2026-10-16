#pragma once

#include "s2/street_fight.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
struct EventReadCharactersWork;

struct StreetFightFighterDefinition {
    shen_name character;
    std::uint32_t reserved04;
    const char *blurModel;
    std::uint8_t fightStyle;
    std::uint8_t difficulty;
    std::uint8_t guardMode;
    std::uint8_t reserved13;
    std::uint16_t hitPoints;
    std::uint16_t reserved16;
    float strikeScale;
    float guardScale;
};
struct StreetFightEventWork {
    HLib::Task *task;
    shen_name fighters[11];
    shen_name package;
    std::int32_t state;
    std::int32_t battleStatus;
    std::int32_t battleFrames;
    std::int32_t motions[16];
    std::int32_t previousNpcBatchCount;
    EventReadCharactersWork *characterReadTask;
    std::int32_t frameCounter;
    std::uint32_t reserved94;
    HLib::Task *audienceTask;
    Vector3 playerPosition;
    Vector3 bookiePosition;
    std::int32_t fighterIndex;
    std::uint32_t reservedbc;
    float previousFarClip;
    float previousNearClip;
    std::uint64_t reservedc8;
    const StreetFightFighterDefinition *fighterDefinitions[11];
    StreetFightStartParameters *startParameters;
    std::int32_t rank;
    std::int32_t stage;
    std::uint32_t reserved138[2];
    std::int32_t auxiliaryState;
    std::uint8_t fadeFrames;
    std::uint8_t rankHistory[0x20];
    std::uint8_t attempts;
    std::uint8_t completedOpponents;
    std::uint8_t hudEnabled;
    std::uint8_t audienceResultPublished;
    std::byte reserved169[7];
    std::int8_t roundResults[3];
    std::int8_t round;
    std::int8_t wins;
    std::int8_t losses;
    std::uint16_t reserved176;
    std::int32_t reserved178;
    std::int32_t paidEntry;
};
static_assert(sizeof(StreetFightFighterDefinition) == 0x20);
static_assert(offsetof(StreetFightFighterDefinition, fightStyle) == 0x10);
static_assert(offsetof(StreetFightFighterDefinition, hitPoints) == 0x14);
static_assert(offsetof(StreetFightFighterDefinition, strikeScale) == 0x18);
static_assert(sizeof(StreetFightEventWork) == 0x180);
static_assert(offsetof(StreetFightEventWork, package) == 0x34);
static_assert(offsetof(StreetFightEventWork, state) == 0x38);
static_assert(offsetof(StreetFightEventWork, motions) == 0x44);
static_assert(offsetof(StreetFightEventWork, previousNpcBatchCount) == 0x84);
static_assert(offsetof(StreetFightEventWork, characterReadTask) == 0x88);
static_assert(offsetof(StreetFightEventWork, audienceTask) == 0x98);
static_assert(offsetof(StreetFightEventWork, fighterIndex) == 0xb8);
static_assert(offsetof(StreetFightEventWork, fighterDefinitions) == 0xd0);
static_assert(offsetof(StreetFightEventWork, startParameters) == 0x128);
static_assert(offsetof(StreetFightEventWork, fadeFrames) == 0x144);
static_assert(offsetof(StreetFightEventWork, rankHistory) == 0x145);
static_assert(offsetof(StreetFightEventWork, attempts) == 0x165);
static_assert(offsetof(StreetFightEventWork, completedOpponents) == 0x166);
static_assert(offsetof(StreetFightEventWork, roundResults) == 0x170);
static_assert(offsetof(StreetFightEventWork, wins) == 0x174);

std::int64_t RecordStreetFightRound(StreetFightEventWork *work);
std::uint32_t CommitStreetFightStoryRank(std::int32_t stage);
void ResetStreetFightStoryFlag(std::int32_t stage);
std::int64_t UnparentAndSetExistingPosRot(shen_name character);
using StreetFightDebugFighterCallback = const StreetFightFighterDefinition *(*)();
extern StreetFightDebugFighterCallback streetFightDebugFighterCallback;
extern std::uint32_t streetFightSpotlightPositioned;
const StreetFightFighterDefinition *GetStreetFightDebugFighterDefinition();
inline constexpr shen_name STREET_FIGHT_CHN{SHEN_NAME(CHN_)};
inline constexpr Vector3 STREET_FIGHT_VICTORY_POSITION{269.9100036621094f, 94.80999755859375f, 33.970001220703125f};
void StreetFightReactionNullsub(void *);
void StreetFightRestoreChaseCamera(void *);
void QueueStreetFightPlayerReaction(shen_name fighter, bool playerWon);
std::int64_t PlayStreetFightVictoryAppearance();
struct StreetFightHighModelWork {
    HLib::Task *task;
    shen_name character;
    std::uint32_t selector, state, reserved14;
};
static_assert(sizeof(StreetFightHighModelWork) == 0x18);
static_assert(offsetof(StreetFightHighModelWork, character) == 8);
static_assert(offsetof(StreetFightHighModelWork, state) == 0x10);
std::uint64_t QueueStreetFightMotionSpeed(shen_name character, float speed, std::uint32_t delay);
void StreetFightMotionSpeedCallback(void *taskData);
void StreetFightHighModelCallback(void *taskData);

} // namespace shenmue::s2
