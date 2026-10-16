#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct PakAfsPartition;
inline constexpr shen_name CWON_AREA{SHEN_NAME(CWON)}, CWON_TASK{SHEN_NAME(EVNT)}, CWON_RYO{SHEN_NAME(RYO_)}, CWON_WON{SHEN_NAME(WON_)};
inline constexpr shen_name CWON_CENTER{SHEN_NAME(CENT)}, CWON_DUMMY{SHEN_NAME(DUM0)}, CWON_SECOND_DUMMY{SHEN_NAME(DUM1)}, CWON_INVALID{0xffffffffU};
struct CwonEventWork {
    std::uint8_t failures, routeChoice, routePass, entry, reserved04, audioScene;
    std::uint16_t reserved06, temporarySlot, reserved0a;
    std::int32_t birdTimer;
    std::uint32_t reserved10, reserved14;
    float routePosition, routeSpeed;
    std::int32_t routeTimer;
    shen_name nextArea;
    std::int32_t authorState, routeState, battleResult;
    std::uint32_t motions[4];
    std::int32_t smoke;
    HLib::TaskCallback startCallback;
    HLib::Task *task;
    HLib::Task *authorTask;
    HLib::Task *dummyTask;
    HLib::Task *escapeTask;
    HLib::Task *temporaryTasks[10];
    float followHeight;
    std::uint32_t reservedc4;
};
struct CwonTemporaryWork {
    CwonEventWork *root;
    std::uint32_t slot;
    std::int32_t ticks;
};
struct CwonEscapeWork {
    CwonEventWork *root;
    HLib::Task *task;
    std::int32_t state, ticks;
};
struct CwonAuthorWaitWork {
    std::int32_t frame;
    std::uint32_t reserved04;
    HLib::Task *parent;
    const shen_name *motionCharacters;
    const shen_name *gravityCharacters;
};
static_assert(sizeof(CwonEventWork) == 0xc8 && offsetof(CwonEventWork, entry) == 3 && offsetof(CwonEventWork, authorState) == 0x28);
static_assert(offsetof(CwonEventWork, motions) == 0x34 && offsetof(CwonEventWork, task) == 0x50 && offsetof(CwonEventWork, temporaryTasks) == 0x70 &&
    offsetof(CwonEventWork, followHeight) == 0xc0);
static_assert(sizeof(CwonTemporaryWork) == 0x10 && sizeof(CwonEscapeWork) == 0x18 && sizeof(CwonAuthorWaitWork) == 0x20);
extern PakAfsPartition *cwonPack;
extern HLib::Task *cwonDynamixTask;
extern HLib::Task *cwonNpcLoadTask;
extern std::int32_t cwonWaveDisabled, cwonSpringDisabled, cwonPipeStep, cwonSmoke, cwonPackIndex, cwonPackState;
extern Vector3 cwonSmokePosition, cwonSmokeVolume, cwonSmokeVelocity, cwonSmokeVariance;
extern const shen_name CWON_CHARACTERS[], CWON_ADDITIONAL_CHARACTERS[], CWON_AUTHOR_CHARACTERS[], CWON_AUTHORS[];
void AreaCWONInit();
void AreaCWONMain(std::uintptr_t);
extern const ScriptAreaDefinition areaCWONScriptDefinition;
void CwonMainCallback(void *);
void CwonInitialize(CwonEventWork *, HLib::Task *, std::int32_t);
void CwonInitializePlayer(CwonEventWork *);
void CwonStartPlayerAt(std::int32_t);
void CwonWaveCallback(void *);
void CwonSpringCallback(void *);
void CwonPrepareCharacters(const shen_name *);
std::int32_t CwonPollQte(CwonEventWork *, std::int32_t, HLib::TaskCallback, HLib::TaskCallback);
std::uint32_t CwonQueueTemporary(CwonEventWork *, HLib::TaskCallback, std::int32_t);
std::int64_t CwonDestroyTemporary(CwonEventWork *, std::int32_t);
std::int32_t CwonStartAuthor(std::int32_t, HLib::Task *, std::int32_t, const shen_name *, const shen_name *);
void CwonAuthorWaitCallback(void *);
void CwonPrepareMotionCharacters(const shen_name *);
void CwonRestoreMotionCharacters(const shen_name *);
void CwonQueueEscape(CwonEventWork *, std::int32_t);
void CwonDummyControlCallback(void *);
void CwonEndChaseCallback(void *);
void CwonInitializeBattleCallback(void *);
void CwonBattleAuthorCallback(void *);
void CwonBattleCallback(void *);
void CwonBattleEndCallback(void *);
void CwonBattleLostCallback(void *);
void CwonBattleLostChoiceCallback(void *);
void CwonRestoreBattleProps(CwonEventWork *, std::int32_t);
void CwonRestoreBattlePoses(CwonEventWork *);
std::int32_t CwonUpdateVictoryAuthorHands(CwonEventWork *);
void CwonRootStreetIntroCallback(void *);
void CwonRootMarketIntroCallback(void *);
void CwonRootWarehouseIntroCallback(void *);
} // namespace shenmue::s2
