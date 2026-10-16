#pragma once

#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name STREET_FIGHT_EVENT_TASK{SHEN_NAME(EVNT)}, STREET_FIGHT_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name STREET_FIGHT_BOOKIE{SHEN_NAME(FUY_)};
inline constexpr shen_name STREET_FIGHT_CNT1{SHEN_NAME(CNT1)}, STREET_FIGHT_CNT2{SHEN_NAME(CNT2)};
inline constexpr shen_name STREET_FIGHT_INVALID{0xffffffffU};
inline constexpr shen_name STREET_FIGHT_AB00{SHEN_NAME(AB00)}, STREET_FIGHT_Q100{SHEN_NAME(Q100)}, STREET_FIGHT_QTB1{SHEN_NAME(QTB1)};
inline constexpr shen_name STREET_FIGHT_QSFA{SHEN_NAME(QSFA)}, STREET_FIGHT_QSFB{SHEN_NAME(QSFB)}, STREET_FIGHT_QSFC{SHEN_NAME(QSFC)};
inline constexpr shen_name STREET_FIGHT_QSFD{SHEN_NAME(QSFD)}, STREET_FIGHT_QGBT{SHEN_NAME(QGBT)}, STREET_FIGHT_QJBT{SHEN_NAME(QJBT)};
inline constexpr shen_name STREET_FIGHT_QKBT{SHEN_NAME(QKBT)}, STREET_FIGHT_QLBT{SHEN_NAME(QLBT)}, STREET_FIGHT_QMBT{SHEN_NAME(QMBT)};
inline constexpr shen_name STREET_FIGHT_SFBR{SHEN_NAME(SFBR)}, STREET_FIGHT_SFKK{SHEN_NAME(SFKK)}, STREET_FIGHT_SFTS{SHEN_NAME(SFTS)};
inline constexpr shen_name STREET_FIGHT_SFM2{SHEN_NAME(SFM2)}, STREET_FIGHT_SFS2{SHEN_NAME(SFS2)}, STREET_FIGHT_SFKM{SHEN_NAME(SFKM)};
inline constexpr shen_name STREET_FIGHT_SFTC{SHEN_NAME(SFTC)}, STREET_FIGHT_SFWF{SHEN_NAME(SFWF)}, STREET_FIGHT_SFJF{SHEN_NAME(SFJF)};
inline constexpr shen_name STREET_FIGHT_SFLU{SHEN_NAME(SFLU)}, STREET_FIGHT_SFSR{SHEN_NAME(SFSR)};
inline constexpr std::uint8_t STREET_FIGHT_HELP_FREE = 0x14, STREET_FIGHT_HELP_IRON_THRUST = 0x59;
inline constexpr std::uint8_t STREET_FIGHT_HELP_ACROBAT = 0x5b, STREET_FIGHT_HELP_IRON_HEAD = 0x5d;
inline constexpr std::uint8_t STREET_FIGHT_HELP_MONGOLIAN = 0x5f, STREET_FIGHT_HELP_PASSIVE_RAID = 0x61;
inline constexpr std::uint8_t STREET_FIGHT_HELP_ROD = 0x63, STREET_FIGHT_HELP_ONE_EYED = 0x64;
inline constexpr std::uint8_t STREET_FIGHT_HELP_GREG = 0x66, STREET_FIGHT_HELP_CHUNYAN = 0x67;
inline constexpr std::uint8_t STREET_FIGHT_HELP_BAIHU = 0x6a, STREET_FIGHT_HELP_BEAST = 0x6d;
inline constexpr std::uint8_t STREET_FIGHT_HELP_ROD_THREE = 0x95, STREET_FIGHT_HELP_GREG_THREE = 0x96;
inline constexpr std::uint8_t STREET_FIGHT_HELP_CHUNYAN_THREE = 0x97, STREET_FIGHT_HELP_BAIHU_THREE = 0x98;

struct StreetFightAreaSelection {
    shen_name area;
    std::int32_t scene;
    std::int32_t reserved;
};
struct StreetFightStartParameters {
    shen_name area;
    std::int32_t scene;
    std::uint32_t reserved08;
    std::uint32_t reserved0c;
    std::int32_t *signal;
    std::uint32_t reserved18;
    std::uint32_t flags;
};
struct StreetFightAudienceWork {
    std::uint32_t mode;
    std::uint32_t reserved04;
    HLib::Task *task;
    shen_name fighters[4];
    std::uint8_t fighterCount;
    std::uint8_t state;
    std::uint8_t reserved22;
    std::uint8_t entityCount;
    Vector3 stagePosition;
    std::byte reserved30[0x12];
    std::uint16_t actionFrames;
    std::int16_t soundFrequency;
    std::int16_t soundVolume;
    float radius;
    float radiusRange;
    Vector3 corridorStart;
    Vector3 corridorEnd;
    float corridorLength;
    Vector3 soundCentre;
    Vector3 soundPositions[30];
    std::uint8_t materialBank;
    std::uint8_t reserved1e1;
    std::int16_t loopDelay;
    std::int16_t reserved1e4;
    std::int16_t reserved1e6;
    const shen_name *characterNames;
    std::uint32_t lastSound;
    std::uint32_t reserved1f4;
};
static_assert(sizeof(StreetFightAreaSelection) == 0x0c);
static_assert(sizeof(StreetFightStartParameters) == 0x20);
static_assert(offsetof(StreetFightStartParameters, signal) == 0x10);
static_assert(offsetof(StreetFightStartParameters, flags) == 0x1c);
static_assert(sizeof(StreetFightAudienceWork) == 0x1f8);
static_assert(offsetof(StreetFightAudienceWork, fighters) == 0x10);
static_assert(offsetof(StreetFightAudienceWork, fighterCount) == 0x20);
static_assert(offsetof(StreetFightAudienceWork, stagePosition) == 0x24);
static_assert(offsetof(StreetFightAudienceWork, actionFrames) == 0x42);
static_assert(offsetof(StreetFightAudienceWork, radius) == 0x48);
static_assert(offsetof(StreetFightAudienceWork, corridorStart) == 0x50);
static_assert(offsetof(StreetFightAudienceWork, corridorLength) == 0x68);
static_assert(offsetof(StreetFightAudienceWork, soundCentre) == 0x6c);
static_assert(offsetof(StreetFightAudienceWork, soundPositions) == 0x78);
static_assert(offsetof(StreetFightAudienceWork, characterNames) == 0x1e8);
static_assert(offsetof(StreetFightAudienceWork, lastSound) == 0x1f0);

extern std::int32_t g_streetFightBusy;
extern std::int32_t streetFightMaximumOpponents;
extern std::int32_t g_streetFightCompletionSignal;
void SetSFCharaForCurrentArea(void *unused, StreetFightAreaSelection *selection);
std::int64_t CanPerformForCurrentAreaFlags();
std::uint8_t GetHelpIconForArea();
std::int32_t GetStreetFightStageFlag(std::int32_t stage);
std::int32_t GetStreetFightStageForCurrentArea();
std::int64_t RecordStreetFightVictory(std::int32_t stage);
std::int64_t RecordStreetFightDefeat(std::int32_t stage);
void StartStreetFightForArea(StreetFightStartParameters *parameters);
void PrepareStreetFightPlayerAngle();
std::int64_t RestoreStreetFightPlayer(void *parameters);
HLib::Task *QueueStreetFightAudience();
HLib::Task *QueueStreetFightAudienceCount(std::uint8_t count);
void StreetFightAudienceCallback(void *taskData);
void ControlStreetFightAudienceVisibility(StreetFightAudienceWork *work);
void ControlStreetFightAudiencePositions(StreetFightAudienceWork *work);
std::int64_t FindStreetFightAudienceSpace(StreetFightAudienceWork *work, std::int32_t entity);
std::uint32_t IsStreetFightAudienceCorridorBlocked(StreetFightAudienceWork *work, const Vector3 *point);
void PlayStreetFightAudienceArrivalSounds(StreetFightAudienceWork *work);
void PlayStreetFightAudienceOpeningSounds(StreetFightAudienceWork *work);
void PlayStreetFightAudienceBattleSounds(StreetFightAudienceWork *work);
void PlayStreetFightAudienceDefeatSounds(StreetFightAudienceWork *work);
void PlayStreetFightAudienceVictorySounds(StreetFightAudienceWork *work);
struct StreetFightNpcWaitWork {
    HLib::Task *parent;
    HLib::Task *self;
    std::uint32_t frames;
    std::uint32_t reserved14;
};
static_assert(sizeof(StreetFightNpcWaitWork) == 0x18);
static_assert(offsetof(StreetFightNpcWaitWork, frames) == 0x10);
std::int64_t QueueStreetFightNpcWait(std::uint8_t flags);
void StreetFightNpcWaitBeginCallback(void *);
void StreetFightNpcWaitEndCallback(void *);
struct StreetFightLookWork {
    shen_name character;
    shen_name target;
    std::uint32_t reserved08;
    std::uint32_t frames;
    std::uint16_t flags;
    std::uint16_t reserved12;
};
static_assert(sizeof(StreetFightLookWork) == 0x14);
static_assert(offsetof(StreetFightLookWork, flags) == 0x10);
StreetFightLookWork *QueueStreetFightLook(shen_name character, shen_name target);
void StreetFightLookCallback(void *);
void ReadStreetFightBookiePosition();
std::uint32_t PollStreetFightActionChoice();
std::uint32_t ControlStreetFightActionChoice(std::int32_t enabled);
std::int64_t ReleaseStreetFightCharacter(shen_name character);
struct StreetFightCharacterLoadWork {
    HLib::Task *task;
    HLib::Task *parent;
    const shen_name *characters;
    std::int32_t index;
    std::int32_t state;
};
static_assert(sizeof(StreetFightCharacterLoadWork) == 0x20);
static_assert(offsetof(StreetFightCharacterLoadWork, index) == 0x18);
extern shen_name streetFightLoadCharacters[2];
std::int64_t QueueStreetFightCharacterLoad(HLib::Task *parent);
void StreetFightCharacterLoadCallback(void *);

} // namespace shenmue::s2
