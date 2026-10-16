#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/area_loop.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name TOMA_AREA{SHEN_NAME(TOMA)}, TOMA_PLAYER{SHEN_NAME(RYO_)}, TOMA_RACE_AREA{SHEN_NAME(WT00)};
inline constexpr shen_name TOMA_ARZ0{SHEN_NAME(ARZ0)}, TOMA_WET0{SHEN_NAME(WET0)}, TOMA_LYH{SHEN_NAME(LYH_)}, TOMA_XGH{SHEN_NAME(XGH_)};
inline constexpr shen_name TOMA_PROP{SHEN_NAME(10mt)}, TOMA_DEFAULT_LIGHT{SHEN_NAME(0000)};
inline constexpr shen_name TOMA_INIT_AUTHOR{SHEN_NAME(INIT)}, TOMA_HAVE_INIT_AUTHOR{SHEN_NAME(init)}, TOMA_DECLINE_AUTHOR{SHEN_NAME(ASNT)};
inline constexpr shen_name TOMA_GO_AUTHOR{SHEN_NAME(ASGO)}, TOMA_HAVE_GO_AUTHOR{SHEN_NAME(ASgo)}, TOMA_BYE_AUTHOR{SHEN_NAME(BYE_)};
inline constexpr shen_name TOMA_M401{SHEN_NAME(M401)}, TOMA_M402{SHEN_NAME(M402)}, TOMA_ME03{SHEN_NAME(ME03)};
inline constexpr shen_name TOMA_ME04{SHEN_NAME(ME04)}, TOMA_ME05{SHEN_NAME(ME05)}, TOMA_ME06{SHEN_NAME(ME06)};
struct TomaRequest {
    shen_name map;
    std::uint32_t argument;
    std::int32_t selector;
};
struct TomaWork {
    HLib::Task *parent;
    HLib::Task *task;
    std::uint32_t state;
    shen_name map;
    std::uint32_t argument, reserved1c;
};
struct TomaOffer {
    shen_name motion;
    std::uint32_t reserved;
    std::uint16_t metadata, price;
};
struct TomaMapEntry {
    shen_name map, character;
    TomaOffer offers[6];
};
static_assert(sizeof(TomaRequest) == 0x0c && offsetof(TomaRequest, selector) == 8);
static_assert(sizeof(TomaWork) == 0x20 && offsetof(TomaWork, state) == 0x10 && offsetof(TomaWork, map) == 0x14 && offsetof(TomaWork, argument) == 0x18);
static_assert(sizeof(TomaOffer) == 0x0c && offsetof(TomaOffer, metadata) == 8 && offsetof(TomaOffer, price) == 10);
static_assert(sizeof(TomaMapEntry) == 0x50 && offsetof(TomaMapEntry, offers) == 8);
inline constexpr TomaMapEntry TOMA_MAPS[]{
    {.map = TOMA_ARZ0,
        .character = TOMA_LYH,
        .offers = {{.motion = TOMA_M401, .reserved = 0, .metadata = 65, .price = 8}, {.motion = TOMA_M402, .reserved = 0, .metadata = 65, .price = 8},
            {.motion = TOMA_ME03, .reserved = 0, .metadata = 64, .price = 8}, {.motion = TOMA_ME04, .reserved = 0, .metadata = 64, .price = 8},
            {.motion = TOMA_ME05, .reserved = 0, .metadata = 63, .price = 8}, {.motion = TOMA_ME06, .reserved = 0, .metadata = 63, .price = 8}}},
    {.map = TOMA_WET0,
        .character = TOMA_XGH,
        .offers = {{.motion = TOMA_M401, .reserved = 0, .metadata = 65, .price = 8}, {.motion = TOMA_M402, .reserved = 0, .metadata = 65, .price = 8},
            {.motion = TOMA_ME03, .reserved = 0, .metadata = 64, .price = 8}, {.motion = TOMA_ME04, .reserved = 0, .metadata = 64, .price = 8},
            {.motion = TOMA_ME05, .reserved = 0, .metadata = 63, .price = 8}, {.motion = TOMA_ME06, .reserved = 0, .metadata = 63, .price = 8}}},
    {}};
inline constexpr std::uint32_t TOMA_REWARDS[]{
    4075, 4076, 4074, 4075, 4076, 4083, 4084, 4085, 4086, 4050, 4058, 4105, 4019, 4122, 4068, 4069, 4070, 4162, 4163, 4164, 4165, 4166, 4167, 8192};
inline std::array<std::int32_t, 57> TOMA_HAND_POSE{0, 0, 46606, 0, 2182, 64444, 0, 0, 1456, 0, 64080, 49154, 0, 908, 44239, 0, 0, 53340, 0, 0, 0, 0, 1456,
    54796, 0, 0, 43692, 0, 727, 50974, 0, 0, 0, 0, 2912, 54797, 0, 182, 44421, 0, 1456, 50245, 0, 0, 0, 2912, 910, 51336, 726, 726, 47517, 0, 2003, 50974,
    65518, 65427, 0};
extern std::int32_t tomaRaceEntry;
const TomaMapEntry *FindTomaMap(shen_name map);
void InitToma();
void TomaMain(std::uintptr_t argument);
void TomaSearchNullsub();
void QueueTomaWaitPackage(shen_name map, std::uint32_t argument);
void TomaWaitPackage(void *);
void TomaStart(void *);
void TomaDone(void *);
std::int64_t TomaChargeCompletion(std::int32_t phase, std::int64_t, HLib::Task *parent);
std::int64_t TomaRewardCompletion(std::int32_t phase, std::int64_t, HLib::Task *parent);
void QueueTomaFirstTalk();
void QueueTomaTalk();
void TomaWaitPackageFirstTalk(void *);
void TomaWaitNpcsFirstTalk(void *);
void TomaInitializeTalkAuthor(void *);
void TomaStartFirstTalkChoices(void *);
void TomaCheckFirstTalkChoices(void *);
void TomaGoRaceFirstTalk(void *);
void TomaGoRace(void *);
void TomaExitPlus(void *);
void TomaWaitPackageTalk(void *);
void TomaWaitNpcsTalk(void *);
void TomaWaitPackageBye(void *);
void TomaWaitNpcsBye(void *);
void TomaBye(void *);
void TomaWaitPackageHaveFirstTalk(void *);
void TomaWaitNpcsHaveFirstTalk(void *);
void TomaInitializeHaveTalkAuthor(void *);
void TomaStartHaveFirstTalkChoices(void *);
void TomaCheckHaveFirstTalkChoices(void *);
void TomaGoRaceHaveFirstTalk(void *);
void TomaWaitPackageHaveTalk(void *);
void TomaWaitNpcsHaveTalk(void *);
void QueueTomaVoice(const char *voice, std::int32_t delay, AreaLoopCompletion completion);
void QueueTomaCamera(std::int32_t camera, std::int32_t delay);
bool QueueTomaNpcs(const shen_name *characters);
bool QueueTomaAuthor(shen_name author);
bool QueueTomaPlusPackage(const char *folder);
void QueueTomaMotion(shen_name character, std::uint32_t motion, AreaLoopCompletion completion, std::int32_t delay);
extern const ScriptAreaDefinition tomaScriptDefinition;
} // namespace shenmue::s2::areas
