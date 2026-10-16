#pragma once

#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
struct ScenarioContext;

struct PlayerStatShadowRecord {
    std::uint32_t bestValue{};
    std::uint32_t count{};
    std::uint32_t accumulatedValue{};
};
static_assert(sizeof(PlayerStatShadowRecord) == 0x0c);
static_assert(offsetof(PlayerStatShadowRecord, count) == 0x04);
static_assert(offsetof(PlayerStatShadowRecord, accumulatedValue) == 0x08);
extern std::array<PlayerStatShadowRecord, 4> g_playerStatShadowRecords;

#pragma pack(push, 1)
struct PlayerStatSparsePairSlot {
    std::uint8_t key0{};
    std::uint8_t key1{};
    std::uint8_t value{};
};

struct PlayerStatsPages {
    std::uint8_t unknown000[0x2]{};
    std::uint16_t strayCatAffinityPoints{};
    std::uint8_t strayCatInteractionCount{};
    std::uint8_t unknown005[0x2]{};
    std::uint8_t statU8_2{};
    std::uint16_t searchEnterCount{};
    std::uint16_t searchSelectCount{};
    std::uint32_t noInputFrameCount{};
    std::uint32_t ctrlInitializedFrameCount{};
    std::uint32_t nonNeutralNonRunAnimFrameCount{};
    std::uint32_t runAnimFrameCount{};
    std::uint32_t motorcycleRideFrameCountTotal{};
    std::uint32_t motorcycleRaceBestTimeFrames{};
    std::uint32_t statU32_1{};
    std::uint32_t statU32_0{};
    std::uint16_t statU16_0{};
    std::uint16_t statU16_1{};
    std::uint32_t statU32_2{};
    std::uint32_t bestCompositeTime{};
    std::uint16_t dartsModePlayCountSlots[16]{};
    std::uint32_t dartsModeActiveFrameSlots[33]{};
    std::uint32_t slotMachineCashInTotal{};
    std::uint8_t unknown0E0[0x4]{};
    std::uint16_t slotMachinePlayCountBySlot[16]{};
    std::uint32_t slotMachineTotalBetBySlot[16]{};
    std::uint32_t slotMachineTotalWinBySlot[16]{};
    PlayerStatSparsePairSlot sparsePairU8Slots[127]{};
    std::uint8_t sparsePairTerminatorPadding[0x3]{};
    std::uint16_t motorcycleRoadEdgeHitCount{};
    std::uint16_t notebookOpenCount{};
    std::uint8_t helpCounts[0x80]{};
    std::uint32_t playerStatFlags{};
    std::uint8_t indexedU8Table1[32]{};
    std::uint16_t matrix4x8U16[32]{};
    std::uint8_t unknown3EC[0x400]{};
    std::uint8_t charaItemCounts[512]{};
    std::uint8_t spotVisitCounts[128]{};
    std::uint16_t areaIdleFrameCountSlots[128]{};
    std::uint16_t strayCatSceneVisitCountSlots[16]{};
    std::uint16_t indexedU16Table3[8]{};
    std::uint8_t save70MBSamples[0x10]{};
    std::uint32_t bestTimeTicks{};
    std::uint32_t stateTick70MB{};
    std::uint8_t indexedU8Table0[16]{};
    std::uint16_t playerCombatContactCounts[128]{};
    std::uint16_t playerCombatConfirmedHitCounts[128]{};
    std::uint32_t vfBestFightTicks[32]{};
    std::uint8_t vfHpRegenProfiles[32]{};
    std::uint16_t totalDefeatsDeltaAccum{};
    std::uint16_t attemptCount70MB{};
    std::uint8_t timeAccum70MB[0x10]{};
    std::uint8_t timeCap70MB[32]{};
    std::uint8_t counter1_70MB{};
    std::uint8_t counter0_70MB{};
    std::uint8_t statU8_0{};
    std::uint8_t statU8_1{};
    std::uint16_t motorcycleRaceAttemptCount{};
    std::uint16_t totalDefeated70MB{};
    std::uint32_t bestSegmentTime{};
    std::uint16_t indexedU16Table0[6]{};
    std::uint32_t slotMachinePlayFramesBySlot[16]{};
    std::uint32_t shugyoCourseActiveFrameSlots[5]{};
    std::uint16_t shugyoCourseClearCountSlots[5]{};
    std::uint8_t unknownF0E[0x52]{};
};
#pragma pack(pop)

static_assert(sizeof(PlayerStatSparsePairSlot) == 0x3);
static_assert(sizeof(PlayerStatsPages) == 0xF60);
static_assert(offsetof(PlayerStatsPages, motorcycleRideFrameCountTotal) == 0x01C);
static_assert(offsetof(PlayerStatsPages, sparsePairU8Slots) == 0x184);
static_assert(offsetof(PlayerStatsPages, playerStatFlags) == 0x388);
static_assert(offsetof(PlayerStatsPages, charaItemCounts) == 0x7EC);
static_assert(offsetof(PlayerStatsPages, spotVisitCounts) == 0x9EC);
static_assert(offsetof(PlayerStatsPages, noInputFrameCount) == 0x0C);
static_assert(offsetof(PlayerStatsPages, runAnimFrameCount) == 0x18);
static_assert(offsetof(PlayerStatsPages, areaIdleFrameCountSlots) == 0xA6C);
static_assert(offsetof(PlayerStatsPages, slotMachinePlayFramesBySlot) == 0xEB0);
static_assert(offsetof(PlayerStatsPages, playerCombatContactCounts) == 0xBC4);
static_assert(offsetof(PlayerStatsPages, playerCombatConfirmedHitCounts) == 0xCC4);
static_assert(offsetof(PlayerStatsPages, vfBestFightTicks) == 0xDC4);
static_assert(offsetof(PlayerStatsPages, vfHpRegenProfiles) == 0xE44);
static_assert(offsetof(PlayerStatsPages, totalDefeatsDeltaAccum) == 0xE64);
static_assert(offsetof(PlayerStatsPages, attemptCount70MB) == 0xE66);
static_assert(offsetof(PlayerStatsPages, timeAccum70MB) == 0xE68);
static_assert(offsetof(PlayerStatsPages, timeCap70MB) == 0xE78);
static_assert(offsetof(PlayerStatsPages, totalDefeated70MB) == 0xE9E);

struct CassetteTapeState {
    std::uint8_t count{};
    std::uint8_t id{0xff};
};

struct MoveSetRuntimeState {
    std::array<std::uint16_t, 20> learnValue{};
    std::array<std::uint16_t, 20> moveValue{};
    std::uint8_t count{};
    std::uint8_t countOrUnknown{};
};

static_assert(sizeof(MoveSetRuntimeState) == 0x52);

struct PlayerStatsRuntimeState {
    std::uint32_t currentSaveProgress{};
    std::array<std::uint8_t, 16> step{};
    std::array<std::uint8_t, 320> statBank4A0{};
    PlayerStatsPages pages{};
    std::array<std::int16_t, 512> inventoryItemIds{};
    std::int16_t inventoryItemCount{};
    std::array<std::uint8_t, 0x100> inventoryItemCounts0To66{};
    std::array<std::uint8_t, 0x100> inventoryItemCounts67To234{};
    std::array<std::uint8_t, 67> ownedItemMenuFlags0To66{};
    std::array<CassetteTapeState, 32> cassetteTapes{};
    std::uint32_t savedFlags{};
    std::uint32_t runtimeFlags{};
    std::int32_t cash{};
    std::int32_t alternateHealth{};
    std::int32_t defaultHealth{};
    std::uint32_t cameraShakeAmount{};
    std::uint32_t fightAssistScale{};
    std::uint32_t contextActionMode{};
    std::uint8_t vfHpRegenerationProfile{};
    std::uint32_t segaCoins{};
    std::uint32_t segaCoinsMaximum{};
    std::uint32_t inventoryControlValue9{};
    std::array<std::uint16_t, 2> batteryLives{{28800, 28800}};
    std::uint8_t batteryDecrementMask{};
    std::int16_t recentItemKeyLatest{-1};
    std::int16_t recentItemKeyPrevious{-1};
    std::int16_t recentItemKeyOldest{-1};
    std::int16_t scenarioStepUniqueItemCount{};
    std::int32_t cassetteUtilityResult{};
    std::uint32_t creditsCompletionGate{};
    std::uint32_t creditsCompletionCount{};
    std::uint32_t arcadeAggregateGate{};
    std::uint32_t arcadeAggregateCount{};
    std::uint32_t harrierCompletionGate{};
    std::uint32_t harrierCompletionCount{};
    std::uint32_t hangOnCompletionGate{};
    std::uint32_t hangOnCompletionCount{};
    std::array<std::int32_t, 4> currentVfSkill{};
    std::uint16_t currentMove{0xff00};
    std::array<MoveSetRuntimeState, 3> moveSets{};
    std::array<std::array<std::uint16_t, 20>, 3> moveUsageCounts{};
    std::uint16_t movesScreenMoveSetAndSelectionState{0xff00};
    std::uint8_t movesScreenSelectedMoveIndex{};
};

extern PlayerStatsRuntimeState g_playerStats;
void RegisterInventoryItemGroup(std::uint16_t groupId);
bool AddInventoryItemCount(std::uint16_t itemId, std::uint8_t amount);
std::uint8_t RemoveInventoryItemCount(std::uint16_t itemId, std::uint8_t amount);
std::uint8_t GetInventoryItemCount(std::uint16_t itemId);
void SetOwnedItemMenuInteractive(std::uint16_t itemId, bool enabled);
bool AddCassetteTapeCount(std::uint8_t tapeId, std::uint8_t amount);
std::uint8_t RemoveCassetteTapeCount(std::uint8_t tapeId, std::uint8_t amount);
std::uint8_t GetCassetteTapeCount(std::uint8_t tapeId);
std::uint16_t CountOwnedCassetteTapes();
std::uint32_t GetPlayerSimpleStat(std::uint32_t command);
void SetPlayerSimpleStat(std::uint32_t command, std::uint32_t value);
void ResetInventoryControlValue9();
std::uint32_t RecordGameModuleLoadStats(std::uint32_t module);
std::uint32_t AccessBatteryLife(std::uint32_t mode, std::uint8_t index, std::uint32_t value);
std::int32_t AccessScenarioStepUniqueItemCount(std::uint32_t mode);
std::int32_t UpdateAveragedRuntimeStatPageSlot(std::int8_t slotIndex, std::int32_t sampleValue);
std::uint32_t ConsumeCassetteUtilityResult(bool gameStateActive);

extern const std::array<std::array<std::uint8_t, 2>, 4> g_dialogueModeVoiceTextEnable;
void InitialiseShenmueSettings();
void shenInitDefaultSettings();
void shenInitDefaultStats();
void shenInitDefaultCassetteInventory();

std::uint32_t SetPlayerStats(const std::uint32_t *arguments);
void Script_PlayerStatsUtils(ScenarioContext *context, void *rawParams);

inline constexpr std::array<std::uint32_t, 301> kValidCharaNames{
    {SHEN_NAME(AKIR), SHEN_NAME(SORY), SHEN_NAME(IWAO), SHEN_NAME(JAKR), SHEN_NAME(FUKU), SHEN_NAME(INE_), SHEN_NAME(KISY), SHEN_NAME(TAIJ), SHEN_NAME(CHAI),
        SHEN_NAME(KURA), SHEN_NAME(KURB), SHEN_NAME(KAME), SHEN_NAME(JIMY), SHEN_NAME(TONY), SHEN_NAME(SMTH), SHEN_NAME(AKMI), SHEN_NAME(YOKO), SHEN_NAME(YUKK),
        SHEN_NAME(SYZU), SHEN_NAME(JUKY), SHEN_NAME(OISI), SHEN_NAME(NAMS), SHEN_NAME(SETA), SHEN_NAME(MTUK), SHEN_NAME(HORS), SHEN_NAME(SNKC), SHEN_NAME(YOSE),
        SHEN_NAME(MRIG), SHEN_NAME(NITO), SHEN_NAME(TSUC), SHEN_NAME(HATO), SHEN_NAME(ITOI), SHEN_NAME(TURU), SHEN_NAME(KURI), SHEN_NAME(AOKI), SHEN_NAME(MADA),
        SHEN_NAME(ETKO), SHEN_NAME(BUSS), SHEN_NAME(TOM_), SHEN_NAME(HIRA), SHEN_NAME(HISA), SHEN_NAME(YURI), SHEN_NAME(NANS), SHEN_NAME(HRKO), SHEN_NAME(AKSK),
        SHEN_NAME(MRUA), SHEN_NAME(HOND), SHEN_NAME(ONRR), SHEN_NAME(TATM), SHEN_NAME(KYUR), SHEN_NAME(UNO_), SHEN_NAME(KYOH), SHEN_NAME(ASDA), SHEN_NAME(YAMA),
        SHEN_NAME(MISM), SHEN_NAME(KOND), SHEN_NAME(RNKT), SHEN_NAME(NORK), 0x00000000u, SHEN_NAME(YNHS), SHEN_NAME(KKBN), SHEN_NAME(YOPA), SHEN_NAME(CATM),
        SHEN_NAME(KJIY), 0x00000000u, SHEN_NAME(EIIU), SHEN_NAME(PEDR), SHEN_NAME(PAUR), SHEN_NAME(TOM1), SHEN_NAME(KEBA), SHEN_NAME(KEBB), SHEN_NAME(KEBC),
        SHEN_NAME(KEBD), SHEN_NAME(STP1), SHEN_NAME(SAGA), SHEN_NAME(MSYU), SHEN_NAME(RYBI), SHEN_NAME(KYAS), SHEN_NAME(SIIN), SHEN_NAME(SKGK), 0x00000000u,
        SHEN_NAME(KIM_), 0x00000000u, SHEN_NAME(MIKM), SHEN_NAME(NSMR), SHEN_NAME(TKYM), SHEN_NAME(MTUR), SHEN_NAME(EDST), SHEN_NAME(MYKN), SHEN_NAME(TMNY),
        SHEN_NAME(MYTY), SHEN_NAME(HROT), SHEN_NAME(KYKW), SHEN_NAME(MWAK), SHEN_NAME(MKII), SHEN_NAME(HREO), SHEN_NAME(SCKO), SHEN_NAME(EIKK), SHEN_NAME(KUKT),
        SHEN_NAME(JOHN), 0x00000000u, SHEN_NAME(CMAL), SHEN_NAME(SATM), SHEN_NAME(BIKA), SHEN_NAME(BIKB), SHEN_NAME(TMMR), SHEN_NAME(MEYS), SHEN_NAME(TYHG),
        SHEN_NAME(KJHS), SHEN_NAME(TYMK), 0x00000000u, 0x00000000u, SHEN_NAME(SIND), SHEN_NAME(MASR), SHEN_NAME(ONO_), SHEN_NAME(MIKI), SHEN_NAME(HATR),
        0x00000000u, SHEN_NAME(YAMO), SHEN_NAME(NOMR), SHEN_NAME(MEGM), SHEN_NAME(MAYM), SHEN_NAME(YKDM), SHEN_NAME(YOS1), SHEN_NAME(YOS2), SHEN_NAME(YOB1),
        SHEN_NAME(YOB2), SHEN_NAME(YJJI), SHEN_NAME(YBBA), SHEN_NAME(SUMI), SHEN_NAME(KOMN), SHEN_NAME(SAJO), 0x00000000u, SHEN_NAME(SKMT), SHEN_NAME(KOTA),
        SHEN_NAME(YASU), SHEN_NAME(KAYO), 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, SHEN_NAME(TTYA), SHEN_NAME(SOBA), SHEN_NAME(ASOU),
        SHEN_NAME(HINA), SHEN_NAME(YSHY), SHEN_NAME(MJNT), 0x00000000u, SHEN_NAME(KKEN), SHEN_NAME(TKNB), SHEN_NAME(TOSK), SHEN_NAME(SERA), SHEN_NAME(HMRO),
        SHEN_NAME(BSJM), SHEN_NAME(BOB_), SHEN_NAME(FKSM), SHEN_NAME(KJMA), SHEN_NAME(NRSK), SHEN_NAME(ASNO), SHEN_NAME(NAKA), SHEN_NAME(NMKI), SHEN_NAME(YMGC),
        SHEN_NAME(KOGA), SHEN_NAME(TMRA), SHEN_NAME(MITA), SHEN_NAME(YAYI), SHEN_NAME(HTSI), SHEN_NAME(JOE_), SHEN_NAME(SAKI), SHEN_NAME(MIHO), SHEN_NAME(ENDO),
        SHEN_NAME(YOSI), SHEN_NAME(TKSA), SHEN_NAME(TAGW), SHEN_NAME(RYKO), SHEN_NAME(IRIE), SHEN_NAME(KURT), SHEN_NAME(HANA), SHEN_NAME(SIMZ), 0x00000000u,
        SHEN_NAME(TTAY), SHEN_NAME(DICK), SHEN_NAME(MARK), SHEN_NAME(RBRT), SHEN_NAME(HARY), SHEN_NAME(JONZ), SHEN_NAME(TAKI), SHEN_NAME(HIRI), SHEN_NAME(TAKE),
        SHEN_NAME(AKTG), SHEN_NAME(TOKI), SHEN_NAME(KNJI), SHEN_NAME(RIKA), SHEN_NAME(STSI), SHEN_NAME(GORO), SHEN_NAME(MKMR), SHEN_NAME(TDSI), SHEN_NAME(HRNO),
        0x00000000u, SHEN_NAME(KUDO), SHEN_NAME(MURI), SHEN_NAME(FLD1), SHEN_NAME(FLD2), SHEN_NAME(FLD3), SHEN_NAME(FLD4), SHEN_NAME(FLD5), SHEN_NAME(FLD6),
        SHEN_NAME(FLD7), SHEN_NAME(FLD8), SHEN_NAME(FLD9), SHEN_NAME(FLDA), SHEN_NAME(FLDB), SHEN_NAME(FLDC), SHEN_NAME(FLDD), SHEN_NAME(KEBE), SHEN_NAME(KEBF),
        SHEN_NAME(KEBG), SHEN_NAME(KEBH), SHEN_NAME(KEBI), SHEN_NAME(KEBJ), SHEN_NAME(KEBK), SHEN_NAME(KEBL), SHEN_NAME(YOHI), SHEN_NAME(NGAI), SHEN_NAME(ITOH),
        SHEN_NAME(SKRD), SHEN_NAME(YJIH), SHEN_NAME(TJMA), SHEN_NAME(JONO), SHEN_NAME(UDGW), SHEN_NAME(OKYS), SHEN_NAME(ISDA), SHEN_NAME(MURA), SHEN_NAME(SNGA),
        SHEN_NAME(MITI), SHEN_NAME(MSTA), SHEN_NAME(MRSK), SHEN_NAME(MNWA), SHEN_NAME(HTNK), SHEN_NAME(HRSK), SHEN_NAME(NMNO), SHEN_NAME(ISYM), SHEN_NAME(KYMA),
        SHEN_NAME(ENKI), SHEN_NAME(NGSM), SHEN_NAME(TKHS), SHEN_NAME(HIDE), SHEN_NAME(KTRO), SHEN_NAME(KENI), SHEN_NAME(KYHN), SHEN_NAME(YMST), SHEN_NAME(SNDO),
        SHEN_NAME(NMTO), SHEN_NAME(KWMT), SHEN_NAME(TAEN), SHEN_NAME(SZEN), SHEN_NAME(KYSN), SHEN_NAME(SATO), SHEN_NAME(YSKT), SHEN_NAME(STAK), SHEN_NAME(IZWA),
        0x00000000u, SHEN_NAME(SGRH), SHEN_NAME(MTRI), SHEN_NAME(TERY), SHEN_NAME(YKHI), SHEN_NAME(SBNK), SHEN_NAME(DTRY), 0x00000000u, 0x00000000u,
        SHEN_NAME(RINS), SHEN_NAME(ECHO), 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u, SHEN_NAME(GRKN), SHEN_NAME(MAGO), SHEN_NAME(CHRL), 0x00000000u,
        SHEN_NAME(MORN), SHEN_NAME(HDEI), SHEN_NAME(TEYI), SHEN_NAME(TOMH), SHEN_NAME(FLDE), SHEN_NAME(FLDF), SHEN_NAME(FLDG), SHEN_NAME(TMHN),

        SHEN_NAME(DJUN), 0x00000000u, SHEN_NAME(HSOA), SHEN_NAME(GJHM), SHEN_NAME(GJBM), SHEN_NAME(GJHF), SHEN_NAME(GJBF), SHEN_NAME(YUMM), SHEN_NAME(SYKU),
        SHEN_NAME(HTSK), SHEN_NAME(SMIK), SHEN_NAME(SNJY), SHEN_NAME(SINF)}};

inline constexpr std::int16_t kInventoryGroupTable[] = {
    753, 120, 62, -1, 757, 134, -1, 775, 134, 135, 136, -1,
    746, 11, -1, 910, 134, 135, 136, -1, 852, 243, 244, -1,
    876, 291, 292, 187, -1, 920, 13, 14, -1, 100, 30, 151,
    184, 185, -1, 801, 239, 148, -1, 140, 24, 278, -1, 851,
    90, 89, -1, 160, 60, 196, 230, 236, 238, -1, 170, 265,
    -1, 928, 181, -1, 220, 194, 244, 200, -1, 192, 170, 172,
    -1, 242, 172, 287, -1, 841, 18, 69, 70, -1, 799, 40,
    69, 71, 72, -1, 838, 18, -1, 913, 69, -1, 250, 69,
    70, 71, 72, 214, -1, 260, 6, 7, -1, 249, 144, 240,
    241, -1, 310, 8, -1, 956, 12, 177, 142, 114, 46, 119,
    26, 32, 44, 57, 33, 129, 161, -1, 857, 183, 154, 67,
    -1, 858, 18, 104, 103, -1, 450, 181, -1, 460, 603, 611,
    618, -1, 470, 648, 650, 600, 153, -1, 500, 109, 160, 162,
    164, 166, 207, 285, 286, 66, -1, 909, 264, 193, 152, 602,
    605, 607, 608, 610, 613, 614, -1, 970, 38, -1, 972, 620,
    622, 635, 666, 683, 632, 626, 630, 627, 631, 629, 639, 633,
    624, 628, 637, 625, 640, 634, 677, 665, 682, 656, 657, 659,
    681, 674, 678, 675, 679, 676, 680, 684, 664, 654, 669, 660,
    652, 670, 662, 653, 655, 661, 667, 663, 658, 668, 699, 636,
    638, 687, 688, 686, 690, 689, 697, 698, 695, 692, 693, -1,
    573, 187, -1, 745, 15, -1, 597, 131, -1, 670, 263, 15,
    16, 131, -1, 520, 114, -1, 733, 114, -1, 961, 0, 5,
    4, 2, 1, 9, 10, 250, -1, 904, 2, 3, -1, 902,
    2, 3, -1, 906, 2, 3, -1, 110, 13, 14, 151, 184,
    185, -1, 815, 265, -1, 816, 265, -1, 766, 265, -1, 180,
    52, -1, 210, 37, -1, 678, 224, 225, -1, 600, 40, 144,
    240, 241, -1, 248, 40, 144, 240, 241, -1, 280, 20, -1,
    45, 300, -1, 46, 300, -1, 47, 300, -1, 845, 112, -1,
    862, 112, -1, 360, 36, -1, 861, 36, -1, 370, 12, 108,
    237, -1, 425, 163, 182, 164, 171, -1, 955, 239, 227, -1,
    475, 194, 13, -1, 527, 238, 196, -1, 833, 239, 264, 66,
    193, 236, -1, 545, 6, 264, 600, 602, 603, 605, 607, 608,
    -1, 979, 300, -1, 181, 300, -1, 818, 300, -1, -1, 0
};

} // namespace shenmue::s1
