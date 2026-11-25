#include "s2/player_config_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/scene_assets_dc.hpp"
#include "s2/area_jump_data_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include <cstring>
#define INV_NOEXCEPT
#define INV_NULL 0
#define INV_INIT =
#define INV_ZERO
#include "platform/dc/sdk.hpp"
#include "s2/camera_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/game_save_area.hpp"
#include "s2/input_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/texture_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/area_jump_dc.hpp"
#include "s2/battle_qte.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/story_state.hpp"
#include <cstdarg>
#include <cstdio>
#endif
#include "shenmue.h"
#include "s2/party_companion.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/math.hpp"
#include "s2/maze_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "platform/dc/matrix.hpp"
#include <cfloat>
#include <cmath>
#include "s2/conversation_fade_dc.hpp"
#include "s2/scenario_talk_dc.hpp"
#include "s2/scenario_dc.hpp"
#include <cri_adxt.h>
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/conversation_playback_dc.hpp"
#endif
#include "s2/startup_assets_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/sound_table_dc.hpp"
#include <sg_xpt.h>
#include <cri_adxf.h>
#include "s2/scenario_control_dc.hpp"
#include "s2/scenario_actions_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/scenario_scnt.hpp"
#include "s2/model_dc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/ship_spray.hpp"
#include "s2/hiki.hpp"
#include "s2/event_character_state_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/moves_scroll.hpp"
#include "platform/dc/mt7.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/player_control_state_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/date_time.hpp"
#include "s2/author_control_state_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "s2/search_runtime_dc.hpp"
#endif
#include "s2/map_region_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/hand_dc.hpp"
#include "s2/object_xforms.hpp"
#include "s2/sound_program.hpp"
#endif
#include "s2/search_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/map_region_data_dc.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/speak_icon_setup_dc.hpp"
#include "s2/search_commands_dc.hpp"
#include "s2/flag_data.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/flag_context_dc.hpp"
#endif

namespace shenmue {
namespace s2 {

void InitializeCombatStatistics(CombatStatistics *statistics);
void ResetConfigurationNullsub(std::int32_t);
void SetConfigurationUnknown08(std::uint8_t enabled);
void ApplyS2DialogueMode(std::int32_t mode);
void SetNormalTriggerMapping(std::int32_t enabled);
void ApplyS2TitleLogoFlag(std::int32_t enabled);
void ApplyS2ControlMode(std::int32_t mode);
void ResetPlayerSaveSelections();
void ResetSavedFrameCounter();
void ResetRetainedPlayerFlags();
void SetGameCompletionState(float progress, std::uint32_t flags, std::uint32_t completed);

void ResetRetainedBattleState();
void ResetRetainedBattleState();
void ResetQronSceneChange();
void ResetQronSceneChange();
void InitializeInventoryStock();
std::int32_t SetupControlSpeakAndUI_Internal(std::int16_t *choices, std::int32_t timeout, std::uint16_t defaultChoice, std::uint16_t mode, std::int32_t style);
void vfMotnNibbleSet(std::uint8_t code, std::int32_t value);
NativePlayerStats g_nativePlayerStats = {};
std::int32_t g_transferredYen = 0;
std::uint8_t g_fightActive = 0, g_fightUiActive = 0;
std::uint32_t g_minigameScores[26] = {};
std::uint8_t g_playerRetainedResetWords[16] = {};
std::byte g_playerRetainedResetStorage[1024] = {};
std::uint16_t g_playerResetCounter = 0;
ArcadeStatistics g_arcadeStatistics[4] = {};
std::int16_t g_playerSaveSelections[3] = {};
std::uint16_t g_playerSelectionCounter = 0;
std::uint32_t g_savedFrameCounter[2];
std::uint8_t g_retainedPlayerFlags[2] = {};
float g_gameProgress = 0.0f;
std::uint32_t g_currentGameFlags = 0, g_completedGameCount = 0;
std::uint32_t g_mgOtoshidamaFlag = 0;
CombatStatistics g_combatStatistics = {};
std::uint32_t g_totalFrameCount;
std::int32_t g_authorFrameCount = 0;
std::uint8_t frameCounterEnabled;

std::uint32_t GetS2GameConfigurationFlags() {
    return g_nativePlayerStats.gameConfigurationFlags;
}

void SetS2GameConfigurationFlags(std::uint32_t flags) {
    g_nativePlayerStats.gameConfigurationFlags = flags;
}

// @verified - d
bool vfCheckSkillFlag(shen_name flag) {
    const std::uint8_t group = flag >> 30U;
    const std::uint32_t bits = flag & 0x3fffffffU;
    if (bits & g_nativePlayerStats.statFlags[group])
        return true;
    return false;
}

// @verified - d
void ResetNewGameConfiguration() {
    g_nativePlayerStats.optionA = 0;
    g_nativePlayerStats.optionB = 0;
    ApplyS2DialogueMode(2);
    ResetConfigurationNullsub(0);
    SetConfigurationUnknown08(1);
    SetNormalTriggerMapping(1);
    ApplyS2TitleLogoFlag(0);
    ApplyS2ControlMode(0);
    InitializeMemoPageState(0, 0);
}

void ResetNativePlayerStats() {
    g_nativePlayerStats.alternateHealth = 200;
    g_nativePlayerStats.defaultHealth = 200;
    g_nativePlayerStats.reactionCameraScale = 0;
    g_nativePlayerStats.assistScale = 0;
    g_nativePlayerStats.actionState = 0;
    g_nativePlayerStats.retained = 0;
    g_nativePlayerStats.contextActionMode = 0;
    g_nativePlayerStats.savedFlags = 0;
    g_nativePlayerStats.statFlags[0] = 0;
    g_nativePlayerStats.statFlags[1] = 0x369dff40;
    g_nativePlayerStats.statFlags[2] = 0x7ff;
    g_nativePlayerStats.statFlags[3] = 0;
    g_nativePlayerStats.cash = 596;
    g_nativePlayerStats.segaCoins = 0;
    g_transferredYen = 17880;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    g_fightUiActive = g_fightActive = 0;
#else
    g_fightActive = g_fightUiActive = 0;
#endif
    ResetRetainedBattleState();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::size_t scoresSize = sizeof(g_minigameScores);
    std::memset(g_minigameScores, 0, scoresSize);
    std::size_t wordsSize = sizeof(g_playerRetainedResetWords);
    std::memset(g_playerRetainedResetWords, 0, wordsSize);
#else
    std::memset(g_minigameScores, 0, 0x68);
    std::memset(g_playerRetainedResetWords, 0, 0x10);
#endif
    std::memset(g_playerRetainedResetStorage, 0, 0x400);
    g_playerResetCounter = 0;
    ResetPlayerSaveSelections();
    InitializeEventWindowState();
    ResetMainIconState();
    ResetSleepActionDefaultState();
    g_arcadeStatistics[0].highScore = 0;
    g_arcadeStatistics[1].highScore = 0;
    g_arcadeStatistics[2].highScore = 0;
    g_arcadeStatistics[3].highScore = 0;
    g_arcadeStatistics[0].games = 0;
    g_arcadeStatistics[1].games = 0;
    g_arcadeStatistics[2].games = 0;
    g_arcadeStatistics[3].games = 0;
    g_arcadeStatistics[0].time = 0;
    g_arcadeStatistics[1].time = 0;
    g_arcadeStatistics[2].time = 0;
    g_arcadeStatistics[3].time = 0;
    InitializeCombatStatistics(&g_combatStatistics);
    ResetSavedFrameCounter();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    ResetGlobalStateFlags();
#else
    ResetRetainedPlayerFlags();
#endif
    ResetQronSceneChange();
    for (std::int32_t index = 0; index < 16; ++index)
        vfMotnNibbleSet(static_cast<std::uint8_t>(index), 0);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    SetBattleQteParameters(0.0f, 0, 0);
#else
    SetGameCompletionState(0.0f, 0, 0);
#endif
    g_mgOtoshidamaFlag = 0;
}

// @verified - d
void InitializeCombatStatistics(CombatStatistics *statistics) {
    std::memset(statistics, 0, 0x1000);
    std::memset(&statistics->sentinel, 0xff, sizeof(statistics->sentinel));
}

// @verified - d
void InitItems() {
    std::int32_t index;
    for (index = 0; index < static_cast<std::int32_t>(sizeof(g_keyItemStock)); ++index)
        g_keyItemStock[index] = 0;
    for (index = 0; index < static_cast<std::int32_t>(sizeof(g_capsuleStock)); ++index)
        g_capsuleStock[index] = 0;
    for (index = 0; index < static_cast<std::int32_t>(sizeof(g_inventoryItemStates) / sizeof(g_inventoryItemStates[0])); ++index) {
        g_inventoryItemStates[index].count = 0;
        g_inventoryItemStates[index].item = 255;
    }
    for (index = 0; index < static_cast<std::int32_t>(sizeof(g_photoStock)); ++index)
        g_photoStock[index] = 0;
    for (index = 0; index < static_cast<std::int32_t>(sizeof(g_mapStock)); ++index)
        g_mapStock[index] = 0;
    for (index = 0; index < static_cast<std::int32_t>(sizeof(g_moveStock)); ++index)
        g_moveStock[index] = 0;
    for (index = 0; index < static_cast<std::int32_t>(sizeof(g_pawnStock)); ++index)
        g_pawnStock[index] = 0;
    InitializeInventoryStock();
}

// @verified - d
void SetSubtitleSettingEnabled(std::uint8_t enabled) {
    if (!enabled)
        g_nativePlayerStats.gameConfigurationFlags &= ~GCF_SubtitlesEnabled;
    else
        g_nativePlayerStats.gameConfigurationFlags |= GCF_SubtitlesEnabled;
}

std::int32_t GetSubtitleSettingEnabled() {
    return *reinterpret_cast<const std::uint8_t *>(&g_nativePlayerStats.gameConfigurationFlags) & GCF_SubtitlesEnabled;
}

// @verified - d
void SetVoiceSettingEnabled(std::uint8_t enabled) {
    if (!enabled)
        g_nativePlayerStats.gameConfigurationFlags &= ~GCF_VoiceEnabled;
    else
        g_nativePlayerStats.gameConfigurationFlags |= GCF_VoiceEnabled;
}

std::int32_t GetConversationPlaybackEnabled() {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t enabled = 1;
    enabled &= (g_nativePlayerStats.gameConfigurationFlags >> 1U);
    const std::int32_t act = g_actNumber;
    if (act == 1 && g_currentMap == AREA_OP02)
        enabled = act;
#else
    std::int32_t enabled = (g_nativePlayerStats.gameConfigurationFlags >> 1U) & 1U;
    if (g_actNumber == 1 && g_currentMap == AREA_OP02)
        enabled = 1;
#endif
    return enabled;
}

// @verified - d
void ResetConfigurationNullsub(std::int32_t) {
}

// @verified - d
void SetConfigurationUnknown08(std::uint8_t enabled) {
    if (!enabled)
        g_nativePlayerStats.gameConfigurationFlags &= ~GCF_Unknown08;
    else
        g_nativePlayerStats.gameConfigurationFlags |= GCF_Unknown08;
}

// @verified - d
std::int32_t IsAreaJumpEnabled() {
    return (g_nativePlayerStats.gameConfigurationFlags & 8U) != 0;
}

// @verified - d
void ApplyS2DialogueMode(std::int32_t mode) {
    const std::uint8_t value = static_cast<std::uint8_t>(mode);
    g_nativePlayerStats.gameConfigurationFlags &= ~GCF_DialogueMode;
    g_nativePlayerStats.gameConfigurationFlags |= static_cast<std::uint32_t>(value) << 4;
    SetVoiceSettingEnabled(kDialogueModeSettings[0][value * 2]);
    SetSubtitleSettingEnabled(kDialogueModeSettings[0][value * 2 + 1]);
}

// @verified - d
std::int32_t GetS2DialogueMode() {
    return static_cast<std::int32_t>((g_nativePlayerStats.gameConfigurationFlags & GCF_DialogueMode) >> 4U);
}

// @verified - d
void SetNormalTriggerMapping(std::int32_t enabled) {
    if (!enabled)
        g_nativePlayerStats.gameConfigurationFlags &= ~GCF_NormalTriggerMapping;
    else
        g_nativePlayerStats.gameConfigurationFlags |= GCF_NormalTriggerMapping;
}

// @verified - d
std::int32_t GetNormalTriggerMapping() {
    return static_cast<std::int32_t>((g_nativePlayerStats.gameConfigurationFlags & GCF_NormalTriggerMapping) >> 6U);
}

// @verified - d
void ApplyS2TitleLogoFlag(std::int32_t enabled) {
    if (!enabled)
        g_nativePlayerStats.gameConfigurationFlags &= ~GCF_TitleLogo;
    else
        g_nativePlayerStats.gameConfigurationFlags |= GCF_TitleLogo;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetS2TitleLogoFlag() {
    return static_cast<std::int32_t>((g_nativePlayerStats.gameConfigurationFlags & GCF_TitleLogo) >> 7U);
}
#endif

// @verified - d
std::int32_t GetS2ControlMode() {
    return static_cast<std::int32_t>((g_nativePlayerStats.gameConfigurationFlags & GCF_AlternateControls) >> 8U);
}

// @verified - d
void ApplyS2ControlMode(std::int32_t mode) {
    if (mode == 1)
        g_nativePlayerStats.gameConfigurationFlags |= GCF_AlternateControls;
    else
        g_nativePlayerStats.gameConfigurationFlags &= ~GCF_AlternateControls;
}

void SetMGOTOSIDAMA(std::int32_t bit, std::int32_t enabled) {
    const std::uint32_t shift = static_cast<std::uint32_t>(bit) & 31U;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const std::uint32_t mask = static_cast<std::uint32_t>(bit >= 0) << shift;
#else
    const std::uint32_t mask = bit >= 0 ? 1U << shift : (shift ? 1U >> (32U - shift) : 0U);
#endif
    if (enabled)
        g_mgOtoshidamaFlag |= mask;
    else
        g_mgOtoshidamaFlag &= ~mask;
}

// @verified - d
std::uint32_t GetMGOTOSIDAMA() {
    return g_mgOtoshidamaFlag;
}

// @verified - d
std::int32_t GetPlayerStats(std::int32_t index) {
    switch (index) {
    case 0:
        return static_cast<std::int32_t>(g_nativePlayerStats.savedFlags);
    case 1:
        return static_cast<std::int32_t>(g_nativePlayerStats.retained);
    case 2:
        return g_nativePlayerStats.cash;
    case 3:
        return static_cast<std::int16_t>(g_nativePlayerStats.alternateHealth);
    case 4:
        return static_cast<std::int16_t>(g_nativePlayerStats.defaultHealth);
    case 5:
        return static_cast<std::int16_t>(g_nativePlayerStats.reactionCameraScale);
    case 6:
        return static_cast<std::int16_t>(g_nativePlayerStats.assistScale);
    case 7:
        return static_cast<std::int16_t>(g_nativePlayerStats.contextActionMode);
    case 8:
        return g_nativePlayerStats.segaCoins;
    default:
        return -1;
    }
}

// @verified - d
void SetPlayerStat(std::int32_t index, std::uint32_t value) {
    switch (index) {
    case 0:
        g_nativePlayerStats.savedFlags = value;
        break;
    case 1:
        g_nativePlayerStats.retained = value;
        break;
    case 2:
        g_nativePlayerStats.cash = static_cast<std::int32_t>(value);
        break;
    case 3:
        g_nativePlayerStats.alternateHealth = static_cast<std::uint16_t>(value);
        break;
    case 4:
        g_nativePlayerStats.defaultHealth = static_cast<std::uint16_t>(value);
        break;
    case 5:
        g_nativePlayerStats.reactionCameraScale = static_cast<std::uint16_t>(value);
        break;
    case 6:
        g_nativePlayerStats.assistScale = static_cast<std::uint16_t>(value);
        break;
    case 7:
        g_nativePlayerStats.contextActionMode = static_cast<std::uint16_t>(value);
        break;
    case 8: {
        g_nativePlayerStats.segaCoins = static_cast<std::int32_t>(value);
        std::uint32_t *counters = reinterpret_cast<std::uint32_t *>(g_combatStatistics.retainedPrefix + 0xbc);
        if (value < counters[1])
            value = counters[1];
        counters[1] = value;
        break;
    }
    }
}

// @verified - d
std::uint32_t GetSavedMinigameScore(std::uint32_t index) {
    if (index < (sizeof(g_minigameScores) / sizeof((g_minigameScores)[0])))
        return g_minigameScores[index];
    return 0;
}

// @verified - d
void SetSavedMinigameScore(std::uint32_t index, std::uint32_t score) {
    if (index < (sizeof(g_minigameScores) / sizeof((g_minigameScores)[0])))
        g_minigameScores[index] = score;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void RecordScenarioTalkGroup(std::int32_t index) {
    std::int32_t found = -1;
    for (std::int32_t slot = 0; slot < static_cast<std::int16_t>(g_playerResetCounter); ++slot) {
        if (index == reinterpret_cast<std::int16_t *>(g_playerRetainedResetStorage)[slot]) {
            found = slot;
            break;
        }
    }
    if (found < 0 && static_cast<std::int16_t>(g_playerResetCounter) < 512) {
        const std::int16_t cursor = static_cast<std::int16_t>(g_playerResetCounter);
        ++g_playerResetCounter;
        reinterpret_cast<std::int16_t *>(g_playerRetainedResetStorage)[cursor] = static_cast<std::int16_t>(index);
    }
}
#endif

// @verified - d
void ResetPlayerSaveSelections() {
    g_playerSaveSelections[0] = g_playerSaveSelections[1] = g_playerSaveSelections[2] = -1;
    g_playerSelectionCounter = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t GetPlayerSelectionCounter() {
    return static_cast<std::int16_t>(g_playerSelectionCounter);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void RecordRecentScenarioTalk(std::int32_t index) {
    if (g_playerSaveSelections[0] != index && g_playerSaveSelections[1] != index && g_playerSaveSelections[2] != index &&
        static_cast<std::int16_t>(g_playerSelectionCounter) < 32000) {
        g_playerSaveSelections[0] = g_playerSaveSelections[1];
        g_playerSaveSelections[1] = g_playerSaveSelections[2];
        g_playerSaveSelections[2] = static_cast<std::int16_t>(index);
        ++g_playerSelectionCounter;
    }
}
#endif

// @verified - d
std::int32_t GetShenmue1SaveAreaId(shen_name areaId) {
    const Shenmue1SaveAreaMapping *mapping = shenmue1SaveAreaMappings;
    while (mapping->mapId && mapping->mapId != areaId)
        ++mapping;
    return mapping->saveAreaId;
}

// @verified - d
std::uint8_t *FindStoryFlagTriple(std::uint8_t *table, std::int32_t first, std::int32_t second) {
    std::uint8_t *record;
    std::int32_t count;
retry:
    record = table;
    for (count = 127; count != 0; --count) {
        if (*record == first) {
            ++record;
            if (*record == second)
                return record + 1;
            record += 2;
        } else if (*record == 0xff) {
            *record = first;
            ++record;
            *record = second;
            ++record;
            record[1] = record[2] = 0xff;
            return record;
        } else {
            record += 3;
        }
    }
    table[1] = 0xff;
    table[0] = 0xff;
    goto retry;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
shen_name GetScenarioActorTypeName(std::int32_t isNpc, std::int32_t enabled);

struct StatisticRequestFields {
    std::uint8_t unknown00[3];
    std::uint8_t byte03;
    std::uint8_t unknown04[0x14];
    std::uint32_t value18;
    std::uint32_t value1c;
    std::uint16_t words20[16];
    std::uint32_t values40[33];
    std::uint32_t valueC4;
    std::uint16_t wordsC8[16];
    std::uint32_t valuesE8[16];
    std::uint32_t values128[16];
    std::uint8_t storyFlagTriples[0x200];
    std::uint32_t flags368;
    std::uint8_t bytes36c[32];
    std::uint16_t words38c[68][8];
    std::uint8_t actorTalkCounts[0x200];
    std::uint8_t talkGroupCounts[0x180];
    std::uint16_t wordsB4c[16];
    std::uint16_t wordsB6c[20];
    std::uint8_t bytesB94[0x2c0];
    std::uint8_t bytesE54[32];
    std::uint8_t byteE74;
    std::uint8_t byteE75;
    std::uint8_t unknownE76[2];
    std::uint32_t valuesE78[16];
    std::uint32_t valuesEb8[5];
    std::uint16_t wordsEcc[6];
    std::uint16_t wordEd8;
    std::uint16_t unknownEda;
    std::uint32_t valueEdc;
    std::uint32_t valueEe0;
    std::uint32_t valueEe4;
    std::uint16_t wordsEe8[0x8c];
};
typedef char StatisticRequestFieldsSizeCheck[sizeof(StatisticRequestFields) == sizeof(CombatStatistics) ? 1 : -1];
typedef char StatisticRequestFlagsOffsetCheck[offsetof(StatisticRequestFields, flags368) == 0x368 ? 1 : -1];
typedef char StatisticRequestTalkOffsetCheck[offsetof(StatisticRequestFields, talkGroupCounts) == 0x9cc ? 1 : -1];
typedef char StatisticRequestE78OffsetCheck[offsetof(StatisticRequestFields, valuesE78) == 0xe78 ? 1 : -1];
typedef char StatisticRequestEe8OffsetCheck[offsetof(StatisticRequestFields, wordsEe8) == 0xee8 ? 1 : -1];

#define STATISTIC_LIMIT(value, limit) (static_cast<std::int32_t>(value) > (limit) ? (limit) : static_cast<std::int32_t>(value))

std::uint32_t ProcessStatisticRequest(const std::uint32_t *request) {
    StatisticRequestFields *stats = reinterpret_cast<StatisticRequestFields *>(&g_combatStatistics);
    std::uint8_t *byteValue;
    std::int32_t value;
    std::int32_t index;
    switch (request[0]) {
    case 0:
        byteValue = g_playerRetainedResetWords + request[1];
        value = request[2];
        if (value > 255)
            value = 255;
        return *byteValue = value;
    case 1:
        byteValue = g_playerRetainedResetWords;
        return byteValue[request[1]];
    case 8:
        return stats->value1c = request[1];
    case 9:
        return stats->value1c;
    case 14:
        return stats->byte03 = STATISTIC_LIMIT(request[1], 255);
    case 15:
        return stats->byte03;
    case 16:
        return stats->value18 = request[1];
    case 17:
        return stats->value18;
    case 18:
        return stats->wordEd8 = STATISTIC_LIMIT(request[1], 0xffff);
    case 19:
        return stats->wordEd8;
    case 20:
        return stats->valueEdc = request[1];
    case 21:
        return stats->valueEdc;
    case 22:
        return stats->valueEe0 = request[1];
    case 23:
        return stats->valueEe0;
    case 24:
        return stats->valueEe4 = request[1];
    case 25:
        return stats->valueEe4;
    case 26:
        return stats->wordsEe8[request[1]] = STATISTIC_LIMIT(request[2], 0xffff);
    case 27:
        return stats->wordsEe8[request[1]];
    case 28:
        return stats->valueC4 = request[1];
    case 29:
        return stats->valueC4;
    case 30:
        return stats->wordsC8[request[1]] = STATISTIC_LIMIT(request[2], 0xffff);
    case 31:
        return stats->wordsC8[request[1]];
    case 32:
        return stats->valuesE8[request[1]] = request[2];
    case 33:
        return stats->valuesE8[request[1]];
    case 34:
        return stats->values128[request[1]] = request[2];
    case 35:
        return stats->values128[request[1]];
    case 36:
        return stats->valuesE78[request[1]] = request[2];
    case 37:
        return stats->valuesE78[request[1]];
    case 42:
        switch (request[1]) {
        case 99:
            RecordScenarioTalkGroup(0x2c0);
            RecordScenarioTalkGroup(125);
            RecordScenarioTalkGroup(0xfd);
            break;
        case 100:
            RecordScenarioTalkGroup(0x2bd);
            RecordScenarioTalkGroup(0x2c1);
            RecordScenarioTalkGroup(0x322);
            RecordScenarioTalkGroup(0x323);
            RecordScenarioTalkGroup(118);
            break;
        case 15:
        case 19:
            RecordScenarioTalkGroup(0x2be);
            RecordScenarioTalkGroup(0x2bf);
            RecordScenarioTalkGroup(0x2c0);
            break;
        case 90:
            RecordScenarioTalkGroup(0xcb);
            RecordScenarioTalkGroup(0x11c);
            break;
        case 92:
            RecordScenarioTalkGroup(0xd0);
            RecordScenarioTalkGroup(0xd3);
            break;
        case 79:
            RecordScenarioTalkGroup(0xfb);
            break;
        }
        return stats->talkGroupCounts[request[1]] = STATISTIC_LIMIT(stats->talkGroupCounts[request[1]] + 1, 255);
    case 45:
        return stats->bytesB94[request[1]] = STATISTIC_LIMIT(request[2], 255);
    case 46:
        return stats->bytesB94[request[1]];
    case 47:
        return stats->wordsEcc[request[1]] = STATISTIC_LIMIT(request[2], 0xffff);
    case 48:
        return stats->wordsEcc[request[1]];
    case 49:
        return stats->valuesEb8[request[1]] = request[2];
    case 50:
        return stats->valuesEb8[request[1]];
    case 51:
        return stats->wordsB6c[request[1]] = STATISTIC_LIMIT(request[2], 0xffff);
    case 52:
        return stats->wordsB6c[request[1]];
    case 53:
        return stats->words38c[request[1]][request[2]] = STATISTIC_LIMIT(request[3], 0xffff);
    case 54:
        return stats->words38c[request[1]][request[2]];
    case 55:
        return *FindStoryFlagTriple(stats->storyFlagTriples, request[1], request[2]) = STATISTIC_LIMIT(request[3], 255);
    case 56:
        return *FindStoryFlagTriple(stats->storyFlagTriples, request[1], request[2]);
    case 59:
        return stats->wordsB4c[request[1]] = STATISTIC_LIMIT(request[2], 0xffff);
    case 60:
        return stats->wordsB4c[request[1]];
    case 61:
        return stats->bytes36c[request[1]] = STATISTIC_LIMIT(request[2], 255);
    case 62:
        return stats->bytes36c[request[1]];
    case 63:
        return stats->flags368 |= 1U << request[1];
    case 64:
        return stats->flags368 &= ~(1U << request[1]);
    case 65:
        index = GetScenarioActorTypeName(request[1], 0);
        RecordScenarioTalkGroup(index);
        RecordRecentScenarioTalk(index);
        return stats->actorTalkCounts[index] = STATISTIC_LIMIT(request[2], 255);
    case 66:
        return stats->actorTalkCounts[GetScenarioActorTypeName(request[1], 0)];
    case 67:
        return stats->words20[request[1]] = STATISTIC_LIMIT(request[2], 0xffff);
    case 68:
        return stats->words20[request[1]];
    case 69:
        return stats->values40[request[1]] = request[2];
    case 70:
        return stats->values40[request[1]];
    case 71:
        return stats->bytesE54[request[1]] = STATISTIC_LIMIT(request[2], 255);
    case 72:
        return stats->bytesE54[request[1]];
    case 73:
        return stats->byteE75 = STATISTIC_LIMIT(request[1], 255);
    case 74:
        return stats->byteE75;
    case 75:
        value = request[1];
        return stats->byteE74 = value > 255 ? 255 : value;
    case 76:
        return stats->byteE74;
    case 77:
        return g_completedGameCount;
    }
    return 0;
}

#undef STATISTIC_LIMIT
#endif

// @verified - d
void InitializeFrameVars() {
    frameCounterEnabled = 1;
    g_totalFrameCount = 0;
}

// @verified - d
void ResetSavedFrameCounter() {
    frameCounterEnabled = 0;
    g_totalFrameCount = 0;
}

// @verified - d
void ResetRetainedPlayerFlags() {
    g_retainedPlayerFlags[0] = 0;
    g_retainedPlayerFlags[1] = 0;
}

// @verified - d
void SetGameCompletionState(float progress, std::uint32_t flags, std::uint32_t completed) {
    g_gameProgress = progress;
    g_currentGameFlags = static_cast<std::uint8_t>(flags);
    g_completedGameCount = static_cast<std::uint8_t>(completed);
}

// @verified - d
void AddFrameCount(std::int32_t count) {
    g_totalFrameCount += static_cast<std::uint32_t>(count);
    CombatStatistics *statistics = &g_combatStatistics;
    std::int32_t *const authorFrames = reinterpret_cast<std::int32_t *>(statistics->retainedPrefix + 12);
    if (*authorFrames != -1)
        *authorFrames += count;
}

// @verified - d
void IncrementFrameCount() {
    if (frameCounterEnabled) {
        ++g_totalFrameCount;
        CombatStatistics *statistics = &g_combatStatistics;
        std::int32_t *const authorFrames = reinterpret_cast<std::int32_t *>(statistics->retainedPrefix + 12);
        if (*authorFrames != -1)
            ++*authorFrames;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t g_omakeVideoCount;

void SetOmakeVideoCount(std::uint32_t count) {
    if (count < 256)
        g_omakeVideoCount = count;
}

std::uint32_t GetOmakeVideoCount() {
    return g_omakeVideoCount;
}

#endif

} // namespace s2
} // namespace shenmue

#endif
