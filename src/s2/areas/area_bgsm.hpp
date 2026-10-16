#pragma once
#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name BGSM_AREA{SHEN_NAME(BGSM)}, BGSM_TASK{SHEN_NAME(EVNT)}, BGSM_MONEY_TASK{SHEN_NAME(mony)};
inline constexpr shen_name BGSM_RYO{SHEN_NAME(RYO_)}, BGSM_BASE{SHEN_NAME(BASE)}, BGSM_HAND{SHEN_NAME(HAND)}, BGSM_POT{SHEN_NAME(TUBO)};
inline constexpr shen_name BGSM_CASH{SHEN_NAME(KANE)}, BGSM_HAND_BASE{SHEN_NAME(HDUM)}, BGSM_CAMERA_BASE{SHEN_NAME(HDMC)};
inline constexpr shen_name BGSM_REGION{SHEN_NAME(area)}, BGSM_INVALID{0xffffffffU}, BGSM_BATTLE_INPUT{SHEN_NAME(BTBA)};
inline constexpr shen_name BGSM_DICE[]{SHEN_NAME(DIC1), SHEN_NAME(DIC2), SHEN_NAME(DIC3)};
inline constexpr shen_name BGSM_SPOTS[]{
    SHEN_NAME(DUM0), SHEN_NAME(DUM1), SHEN_NAME(DUM2), SHEN_NAME(DUM3), SHEN_NAME(DUM4), SHEN_NAME(DUM5), SHEN_NAME(DUM6), SHEN_NAME(DUM7), SHEN_NAME(DUM8)};
inline constexpr shen_name BGSM_OPPONENTS[]{
    SHEN_NAME(MIK_), SHEN_NAME(TLB_), SHEN_NAME(UGN_), SHEN_NAME(DS1_), SHEN_NAME(CAK_), SHEN_NAME(KYO_), SHEN_NAME(DYU_)};
struct BgsmWork {
    HLib::Task *self;
    HLib::Task *selection;
    EventReadCharactersWork *characters;
    shen_name opponent;
    std::uint32_t reserved1c;
    std::int32_t elapsed;
    std::uint16_t state, reserved26;
    std::int32_t maximumBet, minimumBet, board, wager;
    std::uint32_t combinations, result;
    std::int16_t choice;
    std::uint16_t table;
    std::uint8_t dice[3];
    std::int8_t chosenFace, pairFace;
    std::byte reserved49[3];
    std::uint32_t reserved4c, standalone;
    char music[20], soundBank[20];
    std::byte reserved7c[4];
    std::uint64_t soundProgram;
};
static_assert(sizeof(BgsmWork) == 0x88 && offsetof(BgsmWork, elapsed) == 0x20 && offsetof(BgsmWork, state) == 0x24);
static_assert(offsetof(BgsmWork, maximumBet) == 0x28 && offsetof(BgsmWork, wager) == 0x34 && offsetof(BgsmWork, dice) == 0x44);
static_assert(offsetof(BgsmWork, music) == 0x54 && offsetof(BgsmWork, soundProgram) == 0x80);
struct BgsmSelectionWork {
    std::uint64_t reserved00;
    std::int32_t requested, board;
    std::int16_t index, state;
    std::uint32_t reserved14;
};
static_assert(sizeof(BgsmSelectionWork) == 0x18 && offsetof(BgsmSelectionWork, index) == 0x10);
struct BgsmVoiceWork {
    HLib::Task *parent;
    std::int32_t delay, index, reserved10, conversationFrames;
    std::uint32_t flags, reserved1c;
};
static_assert(sizeof(BgsmVoiceWork) == 0x20 && offsetof(BgsmVoiceWork, flags) == 0x18);
struct BgsmChoice {
    std::int8_t up, down, left, right, combination, face;
};
static_assert(sizeof(BgsmChoice) == 6);
inline constexpr BgsmChoice BGSM_CHOICES[7][9]{{{2, -1, 1, 3, 2, -1}, {-1, -1, -1, 2, 4, -1}, {-1, 0, 1, 3, 0, -1}, {-1, -1, 2, -1, 3, -1}},
    {{2, -1, 5, 6, 1, 3}, {-1, 4, -1, 2, 4, -1}, {-1, 0, 1, 3, 2, -1}, {-1, 7, 2, -1, 3, -1}, {1, -1, -1, 5, 1, 1}, {1, -1, 4, 0, 1, 2}, {2, -1, 0, 7, 1, 4},
        {3, -1, 6, 8, 1, 5}, {3, -1, 7, -1, 1, 6}},
    {{2, -1, 5, 6, 1, 3}, {-1, 4, -1, 2, 4, -1}, {-1, 0, 1, 3, 0, -1}, {-1, 7, 2, -1, 3, -1}, {1, -1, -1, 5, 1, 1}, {1, -1, 4, 0, 1, 2}, {2, -1, 0, 7, 1, 4},
        {3, -1, 6, 8, 1, 5}, {3, -1, 7, -1, 1, 6}},
    {{2, -1, 5, 6, 0, 3}, {-1, 4, -1, 2, 4, -1}, {-1, 0, 1, 3, 0, -1}, {-1, 7, 2, -1, 3, -1}, {1, -1, -1, 5, 0, 1}, {1, -1, 4, 0, 0, 2}, {2, -1, 0, 7, 0, 4},
        {3, -1, 6, 8, 0, 5}, {3, -1, 7, -1, 0, 6}},
    {{2, -1, 5, 6, 0, 3}, {-1, 4, -1, 2, 4, -1}, {-1, 0, 1, 3, 2, -1}, {-1, 7, 2, -1, 3, -1}, {1, -1, -1, 5, 0, 1}, {1, -1, 4, 0, 0, 2}, {2, -1, 0, 7, 0, 4},
        {3, -1, 6, 8, 0, 5}, {3, -1, 7, -1, 0, 6}},
    {{2, -1, 1, 3, 5, -1}, {-1, -1, -1, 2, 4, -1}, {-1, 0, 1, 3, 2, -1}, {-1, -1, 2, -1, 3, -1}},
    {{2, -1, 1, 3, 5, -1}, {-1, -1, -1, 2, 4, -1}, {-1, 0, 1, 3, 0, -1}, {-1, -1, 2, -1, 3, -1}}};
inline constexpr std::int32_t BGSM_SPOT_COUNTS[]{4, 9, 9, 9, 9, 4, 4};
inline constexpr std::uint32_t BGSM_COMBINATIONS[]{5, 6, 3, 1, 5, 36, 33};
inline constexpr std::int32_t BGSM_PLAYED_FLAGS[]{1365, 1366, 1367, 1379, 1380, 1377, 1378};
inline constexpr std::uint32_t BGSM_IDLE_MOTIONS[]{4495, 4491, 4488}, BGSM_HAPPY_MOTIONS[]{4496, 4492, 4489};
inline constexpr std::uint32_t BGSM_LOSE_MOTIONS[]{4494, 4499, 4498}, BGSM_TALK_MOTIONS[]{4497, 4493, 4490};
inline constexpr const char *BGSM_LABELS[]{"UIBOSWAISI", " ", "UIBOSCHOW0", "UIBOSSMALL", "UIBOSBIG00", "UIBOSZHENZ"}; // @text UIBOSWAISI EN: "Wai Sik" | JA: "刻子" ; UIBOSCHOW0 EN: "Chow" | JA: "順子" ; UIBOSSMALL EN: "Small" | JA: "小" ; UIBOSBIG00 EN: "Big" | JA: "大" ; UIBOSZHENZ EN: "Zhen Zhong" | JA: "真中"
inline constexpr const char *BGSM_NUMBER_LABELS[]{nullptr, "UIBOSNUM01", "UIBOSNUM02", "UIBOSNUM03", "UIBOSNUM04", "UIBOSNUM05", "UIBOSNUM06"}; // @text UIBOSNUM01 EN: "1" | JA: " " ; UIBOSNUM02 EN: "2" | JA: " " ; UIBOSNUM03 EN: "3" | JA: " " ; UIBOSNUM04 EN: "4" | JA: " " ; UIBOSNUM05 EN: "5" | JA: " " ; UIBOSNUM06 EN: "6" | JA: " "
inline constexpr Vector4 BGSM_FOUR_SPOTS[]{{-.03f, .8f, -.25f, 0}, {.29f, .8f, -.08f, 22.5f}, {-.03f, .8f, 0, 0}, {-.35f, .8f, -.08f, 337.5f}};
inline constexpr Vector4 BGSM_NINE_SPOTS[]{{.04f, .8f, -.26f, 0}, {.3f, .8f, .05f, 45}, {-.03f, .8f, .05f, 0}, {-.36f, .8f, .05f, 337.5f},
    {.3f, .8f, -.26f, 45}, {.16f, .8f, -.26f, 22.5f}, {-.11f, .8f, -.26f, 0}, {-.25f, .8f, -.26f, 337.5f}, {-.4f, .8f, -.26f, 315}};
extern std::int32_t bgsmOpponentIndex, bgsmStartingCash, bgsmOpponentPose, bgsmVoiceCount;
extern char bgsmVoices[16][12], bgsmVoicePrefixes[4][16], bgsmMessage[52];
void AreaBGSMInit();
void AreaBGSMMain(std::uintptr_t argument);
extern const ScriptAreaDefinition bgsmScriptDefinition;
void BgsmBegin(void *);
void BgsmInitialize(void *);
void BgsmWaitLoading(void *);
void BgsmCreateBoard(BgsmWork &);
void BgsmPlaceCharacters(BgsmWork &);
void BgsmEnd(void *);
void BgsmStop(void *);
void BgsmTalk(void *);
bool BgsmCheckMoney(BgsmWork &);
void BgsmNoMoney(void *);
void BgsmSleep(void *);
void BgsmInfoSelect(void *);
void BgsmPlay(void *);
void BgsmNoGame(void *);
void BgsmInfo(void *);
void BgsmBet(void *);
void BgsmSelectSet(void *);
void BgsmSelectBet(void *);
void BgsmFallMoney(void *);
void BgsmSelectMove(void *);
void BgsmShakePot(void *);
void BgsmSayDice(void *);
void BgsmJudgeCombination(void *);
void BgsmSayCombination(void *);
void BgsmJudge(void *);
void BgsmTriple(void *);
void BgsmSpecificTriple(void *);
void BgsmSequence(void *);
void BgsmMiddle(void *);
void BgsmSpecificPair(void *);
void BgsmNormalWin(void *);
void BgsmWin(void *);
void BgsmLose(void *);
void BgsmWaitVoice(void *);
void BgsmQueueVoices(HLib::Task *parent, std::uint32_t flags = 0);
void BgsmAppendVoice(std::int32_t speaker, const char *suffix);
void BgsmPlayVoice(std::int32_t speaker, const char *suffix);
void BgsmCamera(std::int32_t camera, shen_name base = BGSM_BASE);
void BgsmMotion(shen_name character, std::uint32_t motion, std::uint32_t flags = 0x50000000U, float speed = 1);
void BgsmPosition(shen_name character, shen_name base, const Vector3 &position);
void BgsmAngle(shen_name character, shen_name base, float degrees);
void BgsmRawAngle(shen_name character, shen_name base, const Vector3i &rotation);
void BgsmHand(shen_name character, std::int32_t *pose, std::uint32_t frames);
extern std::int32_t bgsmOpenHand[57], bgsmBetHand[57], bgsmReleaseHand[57], bgsmPotHand[57];
} // namespace shenmue::s2::areas
