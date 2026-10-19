#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name UEDE_AREA{SHEN_NAME(UEDE)}, UEDE_TASK{SHEN_NAME(EVNT)}, UEDE_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name UEDE_BASE{SHEN_NAME(BASE)}, UEDE_CAMERA{SHEN_NAME(CAM_)}, UEDE_POT{SHEN_NAME(NABE)}, UEDE_TABLE{SHEN_NAME(TABL)};
inline constexpr shen_name UEDE_MEDAL{SHEN_NAME(MEDL)}, UEDE_REGION{SHEN_NAME(area)}, UEDE_BUTTON{SHEN_NAME(BTBA)}, UEDE_INVALID{0xffffffffU};
inline constexpr shen_name UEDE_DICE[]{SHEN_NAME(DIC1), SHEN_NAME(DIC2)};
struct UedeWork {
    HLib::Task *self;
    EventReadCharactersWork *characters;
    shen_name opponent, previousBase;
    const char *audio;
    std::byte reserved20[8];
    std::int32_t elapsed, reserved2c, wager, minimumBet, maximumBet;
    std::int16_t opponentDice[2], playerDice[2];
    std::uint16_t reserved44, state;
    std::uint32_t reserved48, played, repeat, reserved54;
};
struct UedeVoiceWork {
    HLib::Task *parent;
    std::int32_t delay, index, reserved10, conversationFrames;
    std::uint32_t flags, reserved1c;
};
struct UedeDiceWork {
    std::uint32_t frame;
    std::int32_t trajectory, faces[2];
};
struct UedeVenue {
    shen_name area;
    Vector3 position;
    float angle;
    std::int32_t minimumBet, maximumBet;
};
static_assert(sizeof(UedeWork) == 0x58 && offsetof(UedeWork, opponent) == 0x10 && offsetof(UedeWork, maximumBet) == 0x38);
static_assert(offsetof(UedeWork, state) == 0x46 && offsetof(UedeWork, played) == 0x4c && offsetof(UedeWork, repeat) == 0x50);
static_assert(sizeof(UedeVoiceWork) == 0x20 && sizeof(UedeDiceWork) == 0x10 && sizeof(UedeVenue) == 0x1c);
inline constexpr UedeVenue uedeVenues[]{{.area = UEDE_INVALID, .position = {280.74f, 100.03f, 224.22f}, .angle = 90, .minimumBet = 5, .maximumBet = 20},
    {.area = SHEN_NAME(WN00), .position = {240.6f, 104.5f, 160.7f}, .angle = 180, .minimumBet = 10, .maximumBet = 100},
    {.area = SHEN_NAME(WR00), .position = {178.6f, 101.3f, 149.6f}, .angle = 0, .minimumBet = 10, .maximumBet = 50},
    {.area = SHEN_NAME(WR00), .position = {151, 100, 172.8f}, .angle = 270, .minimumBet = 20, .maximumBet = 100},
    {.area = SHEN_NAME(WS00), .position = {47, 98.2f, 145.8f}, .angle = 180, .minimumBet = 50, .maximumBet = 300},
    {.area = SHEN_NAME(WS00), .position = {52.5f, 98.2f, 140.8f}, .angle = 270, .minimumBet = 100, .maximumBet = 500},
    {.area = SHEN_NAME(Q300), .position = {229.6f, 100, 166.2f}, .angle = 10, .minimumBet = 100, .maximumBet = 500},
    {.area = SHEN_NAME(QR00), .position = {154.6f, 100, 193.8f}, .angle = 0, .minimumBet = 10, .maximumBet = 100},
    {.area = UEDE_AREA, .position = {}, .angle = 0, .minimumBet = 10, .maximumBet = 50}};
inline constexpr shen_name uedeOpponents[]{
    SHEN_NAME(AHE_), SHEN_NAME(SSA_), SHEN_NAME(SNN_), SHEN_NAME(SYY_), SHEN_NAME(EMI_), SHEN_NAME(KCH_), SHEN_NAME(KKK_), SHEN_NAME(DS2_), SHEN_NAME(KYO_)};
inline constexpr std::uint32_t uedeThrowMotions[]{4431, 4418}, uedeSmallMotions[]{4430, 4417}, uedeInfoMotions[]{4432, 4419};
inline constexpr std::uint32_t uedeIdleMotions[]{4433, 4420}, uedeTalkMotions[]{4434, 4429}, uedeMiddleMotions[]{4435, 4421}, uedeBigMotions[]{4436, 4422};
inline constexpr std::int32_t uedePlayedFlags[]{1368, 1373, 1371, 1372, 1369, 1370, 1374, 1375};
inline std::int32_t uedeCupHandPose[19][3]{{0, 364, 56071}, {0, 2730, 910}, {0, 62442, 1818}, {0, 64990, 58801}, {0, 0, 61532}, {0, 0, 58984}, {0, 0, 1273},
    {909, 364, 52249}, {0, 64990, 60258}, {0, 0, 57527}, {0, 0, 1456}, {0, 182, 56798}, {0, 64990, 59529}, {0, 65172, 56798}, {0, 182, 1638}, {0, 64810, 60258},
    {1456, 64628, 58801}, {0, 64628, 56798}, {0, 546, 1638}};
inline std::int32_t uedeClosedHandPose[19][3]{{1999, 65354, 60806}, {1638, 180, 63171}, {546, 64719, 3365}, {0, 64810, 53705}, {0, 64810, 49335}, {0, 0, 60075},
    {0, 0, 64811}, {0, 0, 56800}, {0, 0, 47516}, {0, 65172, 56618}, {0, 0, 65354}, {0, 729, 55706}, {0, 65173, 49338}, {0, 64990, 56256}, {0, 0, 0},
    {0, 0, 56434}, {0, 1639, 49882}, {0, 182, 59349}, {64080, 65354, 64444}};
extern std::int32_t uedeOpponentIndex, uedeOpponentPose, uedeStartingCash, uedeInfoPlayed, uedeWinStreak, uedeDiceSpeaker, uedeVoiceCount;
extern char uedeVoicePrefixes[4][16], uedeVoices[16][12];
extern shen_name uedeCharacterList[4];
void InitializeUede();
void RunUede(std::uintptr_t argument);
extern const ScriptAreaDefinition uedeScriptDefinition;
void UedeInitializeCharacters(void *);
void UedePrepareResources(UedeWork &);
void UedeWaitLoading(void *);
void UedeIntroductionCamera(void *);
void UedeTalk(void *);
void UedeNoMoney(void *);
void UedeSelectInfo(void *);
void UedePlay(void *);
void UedeInfo(void *);
void UedeNoGame(void *);
void UedeBet(void *);
void UedeSleep(void *);
void UedeStop(void *);
void UedeEnd(void *);
bool UedeCheckMoney(UedeWork &);
void UedeOpponentThrow(void *);
void UedePlayerThrow(void *);
void UedeJudge(void *);
void UedeWin(void *);
void UedeDraw(void *);
void UedeLose(void *);
void UedeAwardMedal(void *);
void UedeRollDice(void *);
void UedeQueueDice(std::int16_t first, std::int16_t second);
void UedeDiceNullsub();
void UedeSetMob(void *);
void UedeSayTotal(std::int16_t total);
void UedeAppendVoice(std::int32_t speaker, const char *suffix);
void UedePlayVoice(std::int32_t speaker, const char *suffix, std::int32_t delay);
void UedeQueueVoices(HLib::Task *parent, std::uint32_t flags = 0, std::int32_t delay = 0);
void UedeWaitVoices(void *);
} // namespace shenmue::s2::areas
