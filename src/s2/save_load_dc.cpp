#include "s2/save_load_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/backup_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/arcade_rankings_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/story_state.hpp"
#include "s2/navi.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/save.hpp"
#include "s2/startup_assets_dc.hpp"
#include "common/math.hpp"
#include <cstring>

extern "C" std::uint32_t syRtcGetDate(void *date);

namespace shenmue {
namespace s2 {

void RestoreGameSaveSlot(std::uint32_t slot);

void SetUnusedCtrlFlag(std::uint8_t value);

void ActivateSavedAreaJumpLocation();
void ResetAreaJumpWarp();
void ResetNpcScheduleState();

void RestoreMagicRoomModelVariant();

void ResetScenarioGlobals();

void RestoreNpcScheduleSeedPayload(const void *payload);

NativeMoveSkillBank g_moveSkillBank1, g_moveSkillBank2, g_moveSkillBank3;

std::uint16_t g_saveRetainedSceneWords[2];

namespace {

template <typename T> T ReadSaveValue(const std::byte *source) {
    T value;
    std::memcpy(&value, source, sizeof(value));
    return value;
}

} // namespace

namespace {

void RestoreSavedPlayerStats(const std::byte *payload, bool imported) {
    g_nativePlayerStats.savedFlags = ReadSaveValue<std::uint32_t>(payload);
    for (std::int32_t index = 0; index < 4; ++index)
        g_nativePlayerStats.statFlags[index] = ReadSaveValue<std::uint32_t>(payload + 4 + index * 4);
    g_nativePlayerStats.retained = ReadSaveValue<std::uint32_t>(payload + 0x14);
    if (!imported)
        g_nativePlayerStats.cash = ReadSaveValue<std::int32_t>(payload + 0x18);
    g_nativePlayerStats.segaCoins = ReadSaveValue<std::int32_t>(payload + 0x1c);
    g_nativePlayerStats.alternateHealth = ReadSaveValue<std::uint16_t>(payload + 0x20);
    g_nativePlayerStats.defaultHealth = ReadSaveValue<std::uint16_t>(payload + 0x22);
    g_nativePlayerStats.reactionCameraScale = ReadSaveValue<std::uint16_t>(payload + 0x24);
    g_nativePlayerStats.assistScale = ReadSaveValue<std::uint16_t>(payload + 0x26);
    g_nativePlayerStats.gameConfigurationFlags = ReadSaveValue<std::uint32_t>(payload + 0x28);
    g_nativePlayerStats.contextActionMode = ReadSaveValue<std::uint16_t>(payload + 0x2c);
    g_nativePlayerStats.optionA = ReadSaveValue<std::uint8_t>(payload + 0x2e);
    g_nativePlayerStats.optionB = ReadSaveValue<std::uint8_t>(payload + 0x2f);
    g_nativePlayerStats.actionState = ReadSaveValue<std::uint8_t>(payload + 0x30);
}

} // namespace

// @verified - d
void ResetGameForSaveLoad(std::int32_t loading) {
    ResetScenarioPersistentFlags();
    ResetScenarioGlobals();
    InitializePlayerRuntime();
    ResetNewGameConfiguration();
    if (loading) {
        ResetNativePlayerStats();
        InitItems();
    }
    InitCalendar();
    ResetArcadeRankings();
    ResetNpcScheduleState();
    SetDoorQueuedAuthor(shen_name());
    ResetAreaJumpWarp();
    SetScenarioFollowCharacter(shen_name());
    ResetNaviSavedData();
    SetRetainedSaveSelection(-1);
}

// @verified - d
void ImportSelectedTitleSaveData(std::int32_t inherited, const void *bank, std::int32_t slot) {
    if (!inherited)
        InitializeDefaultSaveData();
    if (slot < 0) {
        ResetNativePlayerStats();
        InitItems();
        return;
    }
    const std::byte *const payload = static_cast<const std::byte *>(bank) + 0x2000 + slot * 0x1800;
    RestoreSavedPlayerStats(payload, true);
    g_transferredYen = ReadSaveValue<std::int32_t>(payload + 0x18);
    g_nativePlayerStats.cash = TruncateToInt32(static_cast<float>(g_transferredYen) * 0.033333335f);
    NativeMoveSkillBank *const banks[3] = {&g_moveSkillBank1, &g_moveSkillBank2, &g_moveSkillBank3};
    for (std::int32_t category = 0; category < 3; ++category) {
        NativeMoveSkillBank &moves = *banks[category];
        for (std::int32_t index = 0; index < 40; ++index) {
            moves.proficiency[index] = ReadSaveValue<std::int16_t>(payload + 0x32 + category * 0x52 + index * 2);
            moves.usage[index] = ReadSaveValue<std::int16_t>(payload + 0x5a + category * 0x52 + index * 2);
        }
        moves.count = ReadSaveValue<std::int8_t>(payload + 0x82 + category * 0x52);
        moves.selected = ReadSaveValue<std::uint8_t>(payload + 0x83 + category * 0x52);
    }
    std::memcpy(g_scenarioPersistentFlags.scratch, payload + 0x130, 0x80);
    const std::uint8_t *const inheritedItems = reinterpret_cast<const std::uint8_t *>(payload + 0x250);
    for (std::int32_t index = 0; index < 12; ++index) {
        const std::uint8_t count = inheritedItems[inheritedInventoryMappings[index][0]];
        if (count)
            AddInventoryItem(inheritedInventoryMappings[index][1], count, 0);
    }
    const std::uint8_t *const capsules = reinterpret_cast<const std::uint8_t *>(payload + 0x350);
    std::int32_t destination = 0;
    for (std::int32_t index = 0; index < 256; ++index) {
        if (index != 0x5c && index != 0x5d && index != 0xe2 && index != 0xe3 && index != 0xe4 && index != 0xe5)
            g_capsuleStock[destination++] = capsules[index];
    }
}

namespace {

// @verified - d
void CaptureSaveSystemTime(std::byte *metadata) {
    SaveRtcDate date;
    syRtcGetDate(&date);
    *reinterpret_cast<std::uint16_t *>(metadata) = date.year;
    metadata[2] = static_cast<std::byte>(date.month);
    metadata[3] = static_cast<std::byte>(date.day);
    metadata[4] = static_cast<std::byte>(date.hour);
    metadata[5] = static_cast<std::byte>(date.minute);
    metadata[6] = static_cast<std::byte>(date.second);
    metadata[7] = static_cast<std::byte>(date.dayOfWeek);
}

} // namespace

namespace {

// @verified - d
void CaptureSaveGameTime(std::byte *metadata) {
    *reinterpret_cast<std::int16_t *>(metadata) = g_calendarRecords[0].year;
    metadata[2] = static_cast<std::uint8_t>(g_calendarRecords[0].month);
    metadata[3] = static_cast<std::uint8_t>(g_calendarRecords[0].day);
    metadata[4] = static_cast<std::uint8_t>(g_calendarRecords[0].hour);
    metadata[5] = static_cast<std::uint8_t>(g_calendarRecords[0].minute);
    metadata[6] = static_cast<std::uint8_t>(g_calendarRecords[0].second);
    metadata[7] = g_calendarRecords[0].week;
}

} // namespace

namespace {

struct SavedArcadeRankings {
    ArcadeRankingRecord records[20];
};

struct SavedGamePayload {
    std::byte stats[0x32];
    NativeMoveSkillBank moves[3];
    std::byte retained218[0x2e8];
    std::uint8_t keyItems[0x180];
    std::uint8_t maps[0x80];
    std::uint8_t capsules[0x200];
    std::uint8_t pawns[0x10];
    std::byte retained910[0x30];
    std::uint8_t moveItems[0x1c];
    std::byte retained95c[0x24];
    std::uint8_t resetWords[0x10];
    std::uint32_t minigameScores[0x1a];
    std::byte retained9f8[0x18];
    SavedArcadeRankings outRunRankings, hangOnRankings;
    std::uint16_t sceneWords[2];
    std::byte location[0x30];
    std::int16_t selections[3];
    std::uint16_t selectionCounter;
    std::int32_t transientStates[5];
    std::int16_t memoPages[2];
    std::byte retainedc44[0x1c];
    std::uint8_t photos[0x40];
    std::uint8_t motnNibbles[16];
    std::byte retainedcb0[0x170];
    std::uint16_t arcadeGames[4];
    std::byte retainede28[0x18];
    std::uint32_t arcadeTimes[4];
    std::byte retainede50[0x30];
    std::uint32_t arcadeHighScores[4];
};
typedef char SavedGamePayloadMotnCheck[offsetof(SavedGamePayload, motnNibbles) == 0xca0 ? 1 : -1];
typedef char SavedGamePayloadRankingCheck[offsetof(SavedGamePayload, outRunRankings) == 0xa10 ? 1 : -1];
typedef char SavedGamePayloadArcadeCheck[offsetof(SavedGamePayload, arcadeHighScores) == 0xe80 ? 1 : -1];
typedef char SavedGamePayloadTransientCheck[offsetof(SavedGamePayload, transientStates) == 0xc2c ? 1 : -1];

} // namespace

std::int16_t GetMemoPageStateValue(std::uint8_t side);

std::uint8_t vfMotnNibbleGet(std::uint8_t code);

float GetBattleQteParameter(std::int32_t index);

void CaptureSavedGamePayload(std::byte *payload, std::byte *metadata, std::int32_t initializationMode) {
    std::byte *save = payload;
    std::int32_t i, j;
    reinterpret_cast<NativePlayerStats *>(save)->savedFlags = g_nativePlayerStats.savedFlags;
    for (i = 0; i < 4; ++i)
        reinterpret_cast<NativePlayerStats *>(save)->statFlags[i] = g_nativePlayerStats.statFlags[i];
    reinterpret_cast<NativePlayerStats *>(save)->retained = g_nativePlayerStats.retained;
    reinterpret_cast<NativePlayerStats *>(save)->cash = g_nativePlayerStats.cash;
    reinterpret_cast<NativePlayerStats *>(save)->segaCoins = g_nativePlayerStats.segaCoins;
    reinterpret_cast<NativePlayerStats *>(save)->alternateHealth = g_nativePlayerStats.alternateHealth;
    reinterpret_cast<NativePlayerStats *>(save)->defaultHealth = g_nativePlayerStats.defaultHealth;
    reinterpret_cast<NativePlayerStats *>(save)->reactionCameraScale = g_nativePlayerStats.reactionCameraScale;
    reinterpret_cast<NativePlayerStats *>(save)->assistScale = g_nativePlayerStats.assistScale;
    reinterpret_cast<NativePlayerStats *>(save)->gameConfigurationFlags = g_nativePlayerStats.gameConfigurationFlags;
    reinterpret_cast<NativePlayerStats *>(save)->contextActionMode = g_nativePlayerStats.contextActionMode;
    reinterpret_cast<NativePlayerStats *>(save)->optionA = g_nativePlayerStats.optionA;
    reinterpret_cast<NativePlayerStats *>(save)->optionB = g_nativePlayerStats.optionB;
    reinterpret_cast<NativePlayerStats *>(save)->actionState = g_nativePlayerStats.actionState;
    NativeMoveSkillBank *banks[3] = {&g_moveSkillBank1, &g_moveSkillBank2, &g_moveSkillBank3};
    NativeMoveSkillBank **bank = banks;
    std::int32_t trainedMoves = 0;
    for (i = 0; i < 3; ++i, ++bank) {
        for (j = 0; j < (*bank)->count; ++j) {
            reinterpret_cast<SavedGamePayload *>(save)->moves[i].proficiency[j] = (*bank)->proficiency[j];
            reinterpret_cast<SavedGamePayload *>(save)->moves[i].usage[j] = (*bank)->usage[j];
            if ((*bank)->proficiency[j] > 11)
                ++trainedMoves;
        }
        reinterpret_cast<SavedGamePayload *>(save)->moves[i].count = (*bank)->count;
        reinterpret_cast<SavedGamePayload *>(save)->moves[i].selected = (*bank)->selected;
    }
    CaptureSaveBufferBlocks(save + 0x4c0, save + 0x218, save + 0x2a0);
    for (i = 0; i < 0x180; ++i)
        reinterpret_cast<SavedGamePayload *>(save)->keyItems[i] = g_keyItemStock[i];
    for (i = 0; i < 0x200; ++i)
        reinterpret_cast<SavedGamePayload *>(save)->capsules[i] = g_capsuleStock[i];
    for (i = 0; i < 0x40; ++i)
        reinterpret_cast<SavedGamePayload *>(save)->photos[i] = g_photoStock[i];
    for (i = 0; i < 0x80; ++i)
        reinterpret_cast<SavedGamePayload *>(save)->maps[i] = g_mapStock[i];
    for (i = 0; i < 0x10; ++i)
        reinterpret_cast<SavedGamePayload *>(save)->pawns[i] = g_pawnStock[i];
    for (i = 0; i < 0x1c; ++i)
        reinterpret_cast<SavedGamePayload *>(save)->moveItems[i] = g_moveStock[i];
    for (i = 0; i < 0x10; ++i)
        reinterpret_cast<SavedGamePayload *>(save)->resetWords[i] = g_playerRetainedResetWords[i];
    for (i = 0; i < 0x1a; ++i)
        reinterpret_cast<SavedGamePayload *>(save)->minigameScores[i] = g_minigameScores[i];
    reinterpret_cast<SavedGamePayload *>(save)->outRunRankings = *reinterpret_cast<const SavedArcadeRankings *>(g_outRunRankings);
    reinterpret_cast<SavedGamePayload *>(save)->hangOnRankings = *reinterpret_cast<const SavedArcadeRankings *>(g_hangOnRankings);
    *reinterpret_cast<std::uint16_t *>(save + 0xbf0) = g_saveRetainedSceneWords[0];
    *reinterpret_cast<std::uint16_t *>(save + 0xbf2) = g_saveRetainedSceneWords[1];
    *reinterpret_cast<std::int16_t *>(save + 0xc24) = g_playerSaveSelections[0];
    *reinterpret_cast<std::int16_t *>(save + 0xc26) = g_playerSaveSelections[1];
    *reinterpret_cast<std::int16_t *>(save + 0xc28) = g_playerSaveSelections[2];
    *reinterpret_cast<std::uint16_t *>(save + 0xc2a) = g_playerSelectionCounter;
    for (i = 0; i < 5; ++i)
        reinterpret_cast<SavedGamePayload *>(save)->transientStates[i] = GetScenarioTransientState(i);
    *reinterpret_cast<shen_name *>(save + 0xc5c) = drGetCurrentAuthorId();
    *reinterpret_cast<std::int16_t *>(save + 0xc40) = GetMemoPageStateValue(0);
    *reinterpret_cast<std::int16_t *>(save + 0xc42) = GetMemoPageStateValue(1);
    if (!initializationMode)
        CaptureCurrentPlayerState(reinterpret_cast<SavedAreaJumpLocation *>(save + 0xbf4), save + 0x4a0);
    plymExportPersistentState(reinterpret_cast<PlayerPersistentState *>(save + 0xd88));
    save[0xc54] = static_cast<std::byte>(streetFightProgress);
    *reinterpret_cast<std::int32_t *>(save + 0xc58) = g_transferredYen;
    for (i = 0; i < 16; ++i)
        reinterpret_cast<SavedGamePayload *>(save)->motnNibbles[i] = vfMotnNibbleGet(static_cast<std::uint8_t>(i));
    *reinterpret_cast<float *>(save + 0xcb0) = GetBattleQteParameter(0);
    save[0xcb4] = static_cast<std::byte>(static_cast<std::int32_t>(GetBattleQteParameter(1)));
    save[0xcb5] = static_cast<std::byte>(static_cast<std::int32_t>(GetBattleQteParameter(2)));
    CaptureNpcScheduleSeedPayload(save + 0xcb8);
    if (!initializationMode) {
        ExportPartyCompanionTransition(reinterpret_cast<std::uint8_t *>(save + 0xdc8));
    } else {
        std::uint32_t *transition = reinterpret_cast<std::uint32_t *>(save + 0xdc8);
        transition[0] = 0;
        transition[1] = 0;
        transition[2] = 0;
        transition[3] = 0;
        transition[4] = 0;
        transition[5] = 0;
        transition[6] = 0;
        transition[7] = 0;
    }
    *reinterpret_cast<CombatStatistics *>(payload + 0xe00) = g_combatStatistics;
    payload[0xe02] = static_cast<std::byte>(trainedMoves);
    for (i = 0; i < 4; ++i) {
        reinterpret_cast<SavedGamePayload *>(payload)->arcadeHighScores[i] = g_arcadeStatistics[i].highScore;
        reinterpret_cast<SavedGamePayload *>(payload)->arcadeGames[i] = static_cast<std::uint16_t>(g_arcadeStatistics[i].games);
        reinterpret_cast<SavedGamePayload *>(payload)->arcadeTimes[i] = g_arcadeStatistics[i].time;
    }
    *reinterpret_cast<std::uint32_t *>(payload + 0xe90) = g_minigameScores[2];
    *reinterpret_cast<std::uint32_t *>(payload + 0xe94) = g_minigameScores[8];
    *reinterpret_cast<std::uint32_t *>(payload + 0xe98) = g_minigameScores[10];
    *reinterpret_cast<std::uint32_t *>(payload + 0xe9c) = g_minigameScores[12];
    *reinterpret_cast<std::uint32_t *>(payload + 0xea0) = g_minigameScores[14];
    *reinterpret_cast<std::uint32_t *>(payload + 0xea4) = g_minigameScores[0];
    *reinterpret_cast<std::uint32_t *>(payload + 0xea8) = g_minigameScores[5];
    SaveScenarioGameplayFlags(reinterpret_cast<ScenarioGameplayFlagState *>(payload + 0x1cf4));
    CaptureSaveSystemTime(metadata);
    CaptureSaveGameTime(metadata + 8);
    std::int32_t scene, entry;
    shen_name map;
    GetSceneInfos(&scene, &map, &entry);
    std::uint16_t *saves = reinterpret_cast<std::uint16_t *>(metadata + 0x10);
    const std::int16_t previous = *saves;
    *saves = previous + 1;
    if (*saves == 0)
        *saves = previous;
    *reinterpret_cast<std::int16_t *>(metadata + 0x14) = static_cast<std::int32_t>(GetCurrentStep()) / 100;
    metadata[0x25] = static_cast<std::byte>(static_cast<std::int32_t>(GetCurrentStep()) % 100);
    *reinterpret_cast<std::int16_t *>(metadata + 0x16) = scene;
    *reinterpret_cast<shen_name *>(metadata + 0x18) = map;
    *reinterpret_cast<std::int32_t *>(metadata + 0x1c) = entry;
    *reinterpret_cast<std::uint32_t *>(metadata + 0x20) = CalculateSavePayloadChecksum(payload, 0x1e00);
    metadata[0x24] = static_cast<std::byte>(IsSaveAllowed());
    std::int16_t *version = reinterpret_cast<std::int16_t *>(metadata + 0x12);
    std::int32_t act = static_cast<std::int16_t>(g_loadedActNumber);
    if (static_cast<std::uint8_t>(metadata[0x24]) == 4)
        ++act;
    *version = act;
}

void SetBattleQteParameters(float average, std::uint8_t successes, std::uint8_t failures);

void RestoreSavedGamePayload(const std::byte *metadata, const std::byte *payload) {
    const std::byte *save = payload;
    std::int32_t i, j;
    g_nativePlayerStats.savedFlags = reinterpret_cast<const NativePlayerStats *>(save)->savedFlags;
    for (i = 0; i < 4; ++i)
        g_nativePlayerStats.statFlags[i] = reinterpret_cast<const NativePlayerStats *>(save)->statFlags[i];
    g_nativePlayerStats.retained = reinterpret_cast<const NativePlayerStats *>(save)->retained;
    g_nativePlayerStats.cash = reinterpret_cast<const NativePlayerStats *>(save)->cash;
    g_nativePlayerStats.segaCoins = reinterpret_cast<const NativePlayerStats *>(save)->segaCoins;
    g_nativePlayerStats.alternateHealth = reinterpret_cast<const NativePlayerStats *>(save)->alternateHealth;
    g_nativePlayerStats.defaultHealth = reinterpret_cast<const NativePlayerStats *>(save)->defaultHealth;
    g_nativePlayerStats.reactionCameraScale = reinterpret_cast<const NativePlayerStats *>(save)->reactionCameraScale;
    g_nativePlayerStats.assistScale = reinterpret_cast<const NativePlayerStats *>(save)->assistScale;
    g_nativePlayerStats.gameConfigurationFlags = reinterpret_cast<const NativePlayerStats *>(save)->gameConfigurationFlags;
    g_nativePlayerStats.contextActionMode = reinterpret_cast<const NativePlayerStats *>(save)->contextActionMode;
    g_nativePlayerStats.optionA = reinterpret_cast<const NativePlayerStats *>(save)->optionA;
    g_nativePlayerStats.optionB = reinterpret_cast<const NativePlayerStats *>(save)->optionB;
    g_nativePlayerStats.actionState = reinterpret_cast<const NativePlayerStats *>(save)->actionState;
    NativeMoveSkillBank *moveBanks[3] = {&g_moveSkillBank1, &g_moveSkillBank2, &g_moveSkillBank3};
    NativeMoveSkillBank **bank = moveBanks;
    for (i = 0; i < 3; ++i, ++bank) {
        (*bank)->count = reinterpret_cast<const SavedGamePayload *>(save)->moves[i].count;
        (*bank)->selected = reinterpret_cast<const SavedGamePayload *>(save)->moves[i].selected;
        for (j = 0; j < (*bank)->count; ++j) {
            (*bank)->proficiency[j] = reinterpret_cast<const SavedGamePayload *>(save)->moves[i].proficiency[j];
            (*bank)->usage[j] = reinterpret_cast<const SavedGamePayload *>(save)->moves[i].usage[j];
        }
    }
    RestoreSaveBufferBlocks(save + 0x4c0, save + 0x218, save + 0x2a0);
    j = GetMaxItemsForItemType(0);
    for (i = 0; i < j; ++i)
        g_keyItemStock[i] = reinterpret_cast<const SavedGamePayload *>(save)->keyItems[i];
    j = GetMaxItemsForItemType(1);
    for (i = 0; i < j; ++i)
        g_capsuleStock[i] = reinterpret_cast<const SavedGamePayload *>(save)->capsules[i];
    j = GetMaxItemsForItemType(2);
    for (i = 0; i < j; ++i)
        g_photoStock[i] = reinterpret_cast<const SavedGamePayload *>(save)->photos[i];
    j = GetMaxItemsForItemType(3);
    for (i = 0; i < j; ++i)
        g_mapStock[i] = reinterpret_cast<const SavedGamePayload *>(save)->maps[i];
    j = GetMaxItemsForItemType(5);
    for (i = 0; i < j; ++i)
        g_pawnStock[i] = reinterpret_cast<const SavedGamePayload *>(save)->pawns[i];
    j = GetMaxItemsForItemType(4);
    for (i = 0; i < j; ++i)
        g_moveStock[i] = reinterpret_cast<const SavedGamePayload *>(save)->moveItems[i];
    for (i = 0; i < 16; ++i)
        g_playerRetainedResetWords[i] = reinterpret_cast<const SavedGamePayload *>(save)->resetWords[i];
    for (i = 0; i < 26; ++i)
        g_minigameScores[i] = reinterpret_cast<const SavedGamePayload *>(save)->minigameScores[i];
    *reinterpret_cast<SavedArcadeRankings *>(g_outRunRankings) = reinterpret_cast<const SavedGamePayload *>(save)->outRunRankings;
    *reinterpret_cast<SavedArcadeRankings *>(g_hangOnRankings) = reinterpret_cast<const SavedGamePayload *>(save)->hangOnRankings;
    g_saveRetainedSceneWords[0] = reinterpret_cast<const SavedGamePayload *>(save)->sceneWords[0];
    g_saveRetainedSceneWords[1] = reinterpret_cast<const SavedGamePayload *>(save)->sceneWords[1];
    g_playerSaveSelections[0] = reinterpret_cast<const SavedGamePayload *>(save)->selections[0];
    g_playerSaveSelections[1] = reinterpret_cast<const SavedGamePayload *>(save)->selections[1];
    g_playerSaveSelections[2] = reinterpret_cast<const SavedGamePayload *>(save)->selections[2];
    g_playerSelectionCounter = reinterpret_cast<const SavedGamePayload *>(save)->selectionCounter;
    for (i = 0; i < 5; ++i)
        SetScenarioTransientState(i, reinterpret_cast<const SavedGamePayload *>(save)->transientStates[i]);
    SetDoorQueuedAuthor(shen_name(*reinterpret_cast<const std::uint32_t *>(save + 0xc5c)));
    InitializeMemoPageState(reinterpret_cast<const SavedGamePayload *>(save)->memoPages[0], reinterpret_cast<const SavedGamePayload *>(save)->memoPages[1]);
    plymImportPersistentState(reinterpret_cast<const PlayerPersistentState *>(save + 0xd88));
    RestoreSavedAreaJumpLocation(reinterpret_cast<const SavedAreaJumpLocation *>(save + 0xbf4), save + 0x4a0);
    streetFightProgress = static_cast<std::uint8_t>(save[0xc54]);
    g_transferredYen = *reinterpret_cast<const std::int32_t *>(save + 0xc58);
    for (i = 0; i < 16; ++i)
        vfMotnNibbleSet(i, reinterpret_cast<const SavedGamePayload *>(save)->motnNibbles[i]);
    SetBattleQteParameters(*reinterpret_cast<const float *>(save + 0xcb0), static_cast<std::uint8_t>(save[0xcb4]), static_cast<std::uint8_t>(save[0xcb5]));
    RestoreNpcScheduleSeedPayload(save + 0xcb8);
    ImportPartyCompanionTransition(*reinterpret_cast<const PartyCompanionTransition *>(save + 0xdc8));
    g_combatStatistics = *reinterpret_cast<const CombatStatistics *>(payload + 0xe00);
    for (i = 0; i < 4; ++i) {
        g_arcadeStatistics[i].highScore = reinterpret_cast<const SavedGamePayload *>(payload)->arcadeHighScores[i];
        g_arcadeStatistics[i].games = reinterpret_cast<const SavedGamePayload *>(payload)->arcadeGames[i];
        g_arcadeStatistics[i].time = reinterpret_cast<const SavedGamePayload *>(payload)->arcadeTimes[i];
    }
    RestoreScenarioGameplayFlags(reinterpret_cast<const ScenarioGameplayFlagState *>(payload + 0x1cf4));
    RestoreSaveGameTime(metadata + 8);
    SetSTEP(*reinterpret_cast<const std::int16_t *>(metadata + 0x14) * 100 + *reinterpret_cast<const std::int8_t *>(metadata + 0x25));
}

// @verified - d
std::int32_t NormalizeNewGameSaveResult(std::int32_t result) {
    switch (result) {
    case 0:
        break;
    case -1:
    case -3:
    case -4:
        InitializeDefaultSaveData();
        break;
    default:
        return 0;
    }
    return 1;
}

// @verified - d
std::int32_t InitializeNewGameFromSaveSlot() {
    const std::uint32_t configuration = g_nativePlayerStats.gameConfigurationFlags;
    ResetGameForSaveLoad(0);
    InitializeDefaultSaveData();
    if (NormalizeNewGameSaveResult(ReadSaveDataSection(3))) {
        SetUnusedCtrlFlag(0);
        g_nativePlayerStats.gameConfigurationFlags = configuration;
        return 1;
    }
    g_nativePlayerStats.gameConfigurationFlags = configuration;
    return 0;
}

// @verified - d
std::int32_t LoadGameFromSaveSlot(std::int32_t slot, std::int32_t restorePlacement) {
    const std::uint32_t configuration = g_nativePlayerStats.gameConfigurationFlags;
    const std::uint32_t index = slot;
    if (static_cast<std::int32_t>(index) > 2)
        return 0;
    ResetGameForSaveLoad(1);
    ReadSaveDataSection(3);
    std::int32_t result = WaitForBackupOperationPort0();
    if (result == 0)
        result = ReadSaveDataSection(static_cast<std::int32_t>(index));
    if (result == 0) {
        SetRetainedSaveSelection(slot);
        RestoreGameSaveSlot(index);
        SetScenarioSaveRestoreState(1);
        if (restorePlacement)
            ActivateSavedAreaJumpLocation();
        SetUnusedCtrlFlag(1);
        g_nativePlayerStats.gameConfigurationFlags = configuration;
        RestoreMagicRoomModelVariant();
        return 1;
    }
    g_nativePlayerStats.gameConfigurationFlags = configuration;
    return 0;
}

} // namespace s2
} // namespace shenmue
#endif
