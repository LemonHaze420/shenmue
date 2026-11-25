#include "auto_test.hpp"
#include "s2/title_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/sdk.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "s2/title_save_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/sound_dc.hpp"
#include "s2/title_warning.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/legacy_sprite_slot.hpp"
#include "s2/texture_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include <cstring>
#include "s2/area_names.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/startup_assets_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/scene_data.hpp"
#include <cstdio>
#include <cmath>
#include "s2/prop_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/search_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/date_time.hpp"
#include "s2/camera_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/magic_room_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/map_event_task_dc.hpp"
#include "s2/event_character_control_dc.hpp"
#include "s2/map_info.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/math.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/story_state.hpp"
#include "s2/sound_program.hpp"
#endif
#include "s2/flag_startup_dc.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/weather.hpp"
#include "s2/weather_instruction_dc.hpp"
#include "s2/npc_area_route.hpp"
#define SHENMUE_NPC_NULL 0
#include "s2/directory_cache.hpp"
#include <sg_gd.h>
#include <string.h>
#define SHENMUE_LOADER_NULL 0
#define SHENMUE_LOADER_ALIGN __attribute__((aligned(32)))
#define SHENMUE_LOADER_INIT =
#define SHENMUE_LOADER_MEMCPY ::memcpy
#define SHENMUE_LOADER_STRCPY ::strcpy
#define SHENMUE_LOADER_STRCMP ::strcmp
#define SHENMUE_LOADER_STRSTR ::strstr
#define SHENMUE_LOADER_STRLEN ::strlen
#define SHENMUE_LOADER_STRNCPY ::strncpy
#define SHENMUE_LOADER_FILE_COUNT (sizeof(g_files) / sizeof(g_files[0]))
#define SHENMUE_LOADER_CONST_BYTES(value) static_cast<const std::byte *>(static_cast<const void *>(value))
#include <zlib.h>
#include "s2/event_overlay.hpp"

namespace shenmue {
namespace s2 {

void DrawTitleSaveSelection(void *);
void InitializeTitleNewGameSelection(void *);
void InitializeTitleLoadSelection(void *);
std::int32_t InitializeTitleDebugGame();
void ControlTitleDebugSceneSelector();
void ControlTitleDebugTimeAttack();
std::int32_t InitializeTitleDebugModule();
void SelectTitleSceneGroup(const TitleSceneGroup *groups);
void SelectTitleArea(const TitleSceneEntry *scenes);
void SetTitleDebugStartTime(shen_name area);
void ControlTitleNewGameConfirmation(void *);
void ControlTitleMissingContinue(void *data);
void ControlTitleOptions(void *data);
void TitleMenuControl(void *data);
void DrawTitleMenu(void *);
void ApplyTitleMenuOptions(std::int32_t unused);
void DrawScaledTitleSprite(const TitleTextureRectangle *rectangle, const Vector3 &position, std::int32_t texture, float scale);
void DrawTitleInfoBar(std::int32_t line, std::int32_t secondaryLine);
void DrawTitleBackgroundAndButtons(std::uint32_t flags);
void DrawTitleOptions(void *);
void DrawTitleMissingContinue(void *data);
void DrawTitleNewGameConfirmation(void *);
void SetTitleWarningLogo(std::int32_t logo, const void *parameters);

void SetTitleSceneInfo(const TitleSceneEntry &scene);
void SetTitleLogoPresent();
void SetTitleSceneInfo(const TitleSceneEntry &scene);
void FreeTempSprites(std::int32_t index);
std::int32_t AllocateTempSprites(std::int32_t index);
bool BindTempSprite(std::int32_t index, std::int32_t texture, std::int32_t slot, const std::uint16_t *rectangles);
void InitializeTitlePacket(LegacySpritePacket &packet);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t InitializeNewGameFromSaveSlot();
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t LoadGameFromSaveSlot(std::int32_t slot, std::int32_t restorePlacement);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const TitleDebugModule titleDebugModules[9] = {
    {SHEN_FIELD(name) "             ", SHEN_FIELD(module) SystemModule_Game, SHEN_FIELD(control) ControlTitleDebugSceneSelector,
        SHEN_FIELD(initialize) InitializeTitleDebugGame},
    {SHEN_FIELD(name) "  COLOR BAR  ", SHEN_FIELD(module) SystemModule_Test, SHEN_FIELD(control) 0, SHEN_FIELD(initialize) InitializeTitleDebugModule},
    {SHEN_FIELD(name) "   OPTION    ", SHEN_FIELD(module) SystemModule_Option, SHEN_FIELD(control) 0, SHEN_FIELD(initialize) InitializeTitleDebugModule},
    {SHEN_FIELD(name) "   OUTRUN    ", SHEN_FIELD(module) SystemModule_OutRun, SHEN_FIELD(control) 0, SHEN_FIELD(initialize) InitializeTitleDebugModule},
    {SHEN_FIELD(name) "AFTER BURNER ", SHEN_FIELD(module) SystemModule_AfterBurner, SHEN_FIELD(control) 0, SHEN_FIELD(initialize) InitializeTitleDebugModule},
    {SHEN_FIELD(name) "   HANGON    ", SHEN_FIELD(module) SystemModule_HangOn, SHEN_FIELD(control) 0, SHEN_FIELD(initialize) InitializeTitleDebugModule},
    {SHEN_FIELD(name) "  HARRIER    ", SHEN_FIELD(module) SystemModule_SpaceHarrier, SHEN_FIELD(control) 0, SHEN_FIELD(initialize) InitializeTitleDebugModule},
    {SHEN_FIELD(name) " STAFF ROLL  ", SHEN_FIELD(module) SystemModule_Credits, SHEN_FIELD(control) 0, SHEN_FIELD(initialize) InitializeTitleDebugModule},
    {SHEN_FIELD(name) " FREE BATTLE ", SHEN_FIELD(module) SystemModule_Game, SHEN_FIELD(control) ControlTitleDebugTimeAttack,
        SHEN_FIELD(initialize) InitializeTitleDebugGame},
};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const TitleMenuDispatchEntry titleMenuDispatch[6] = {
    {SHEN_FIELD(control) TitleMenuControl, SHEN_FIELD(display) DrawTitleMenu, SHEN_FIELD(uiState) 1, SHEN_FIELD(sound) 0},
    {SHEN_FIELD(control) ControlTitleOptions, SHEN_FIELD(display) DrawTitleOptions, SHEN_FIELD(uiState) 3, SHEN_FIELD(sound) 0},
    {SHEN_FIELD(control) InitializeTitleLoadSelection, SHEN_FIELD(display) DrawTitleSaveSelection, SHEN_FIELD(uiState) 5, SHEN_FIELD(sound) 0},
    {SHEN_FIELD(control) InitializeTitleNewGameSelection, SHEN_FIELD(display) DrawTitleSaveSelection, SHEN_FIELD(uiState) 2, SHEN_FIELD(sound) 0},
    {SHEN_FIELD(control) ControlTitleNewGameConfirmation, SHEN_FIELD(display) DrawTitleNewGameConfirmation, SHEN_FIELD(uiState) - 1, SHEN_FIELD(sound) - 1},
    {SHEN_FIELD(control) ControlTitleMissingContinue, SHEN_FIELD(display) DrawTitleMissingContinue, SHEN_FIELD(uiState) - 1, SHEN_FIELD(sound) - 1},
};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const TitleMenuRequest titleMenuRequests[5] = {{SHEN_FIELD(action) 2, SHEN_FIELD(callback) 3}, {SHEN_FIELD(action) 0, SHEN_FIELD(callback) 2},
    {SHEN_FIELD(action) - 1, SHEN_FIELD(callback) 1}, {SHEN_FIELD(action) - 1, SHEN_FIELD(callback) 4}, {SHEN_FIELD(action) - 1, SHEN_FIELD(callback) 5}};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const TitleMenuRequest *titlePendingMenuRequest = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t titleMenuAction = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t titleOptionsSelection = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t titleTimeAttackAlpha = 0, titleTimeAttackAlphaStep = 8;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int8_t titleDebugSaveLoaded;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
LegacySpritePacket titleSpritePacket;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const TitleOptionChoiceStyle *titleOptionChoiceCursor = 0;
#endif

std::int32_t titleOptionValues[4];

#if SHENMUE_IS_DC && SHENMUE_IS_S2
HLib::Task *titleWarningTask = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int16_t titleWarningLogos[44][2] = {{-1, -1}, {0, 5}, {1, 6}, {1, 7}, {3, 8}, {0, 9}, {0, 9}, {0, 10}, {1, 11}, {3, 12}, {1, 13}, {1, 14}, {1, 14},
    {1, 15}, {3, 16}, {3, -1}, {1, -1}, {0, -1}, {1, -1}, {0, -1}, {1, -1}, {1, -1}, {0, -1}, {3, -1}, {3, -1}, {1, -1}, {3, -1}, {3, -1}, {1, 17}, {1, 18},
    {3, 19}, {0, 20}, {0, 21}, {0, 22}, {0, 23}, {0, 24}, {0, 25}, {0, 26}, {3, 27}, {0, 28}, {0, 3}, {0, 4}, {0, 29}, {0, 30}};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const std::uint16_t titleWarningDecorationRectangles[56] = {0, 0, 32, 32, 32, 0, 32, 32, 64, 0, 32, 32, 96, 0, 32, 32, 0, 32, 32, 32, 32, 32, 32, 32, 64,
    32, 32, 32, 96, 32, 32, 32, 0, 64, 32, 32, 32, 64, 32, 32, 0, 96, 32, 32, 32, 96, 32, 32, 64, 96, 32, 32, 96, 96, 32, 32};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const std::int32_t titleWarningSelections[2][4] = {{36, 0, 35, 1}, {35, 0, 36, 1}};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const std::int16_t titleWarningLogoY[4] = {240, 240, 400, 323};
#endif

void LoadTitleSoundProgram_Wrapper(std::int32_t sceneIndex);
void FreeTempSprites(std::int32_t index);
std::int32_t AllocateTempSprites(std::int32_t index);
bool BindTempSprite(std::int32_t index, std::int32_t texture, std::int32_t slot, const std::uint16_t *rectangles);

// @verified - d
void TitleControlNullsub(void *) {
}

// @verified - d
void TitleControl(void *data) {
#if SHEN_AUTO_TEST_ENABLED
    if (auto_test::TitleLatched())
        return;
#endif
    TitleControlData &control = *static_cast<TitleControlData *>(data);
    TitleDisplayData &display = *static_cast<TitleDisplayData *>(HLib::GetTaskParameter(control.titleSequenceTask));
    if (control.state > 0) {
        HLib::SetCurrentTaskTo(titleSecondaryCallbacks[control.state - 1].control);
        HLib::AssignCallbackForTask(control.titleSequenceTask, titleSecondaryCallbacks[control.state - 1].display);
        control.state = 0;
        return;
    }
    display.frameCount = (display.frameCount + 1U) & 31U;
    if ((logoFlags & 0xc0U) == 0x40U) {
        titleSequenceTime = static_cast<std::int32_t>(static_cast<std::uint32_t>(titleSequenceTime) - 1U);
        if (titleSequenceTime < 0) {
            SetTitleWarningLogo(0, 0);
            const std::uint32_t backup = GetBackupInfo(0) & 0x11U;
            switch (titleLoopSequence) {
            case 9:
                switch (backup) {
                case 0x11:
                    titleLoopSequence = setPressStartPending;
                    break;
                case 1:
                    titleLoopSequence = 12;
                    break;
                default:
                    titleLoopSequence = 10;
                    break;
                }
                break;
            case 10:
                if (backup & 1U)
                    titleLoopSequence = setPressStartPending;
                else
                    titleLoopSequence = 11;
                break;
            case 11:
                ++titleLoopSequence;
                break;
            default:
                ++titleLoopSequence;
                if (titleLoopSequence > needsTitleSequenceDraw)
                    titleLoopSequence = setPressStartPending;
                break;
            }
            if (titleLoopSequence == 2)
                LoadTitleSoundProgram_Wrapper(0);
            else
                LoadTitleSoundProgram_Wrapper(-1);
            titleSequenceTime = titleSequenceDurations[titleLoopSequence];
            dc::sdk::njSetBackColor(0xff000000U, 0xff000000U, 0xff000000U);
            HLib::AssignCallbackForTask(control.titleSequenceTask, titleDisplayCallbacks[titleLoopSequence]);
            display.currentStateFrame = 0.0f;
            display.stateFrame = 0.062745102f;
            logoFlags |= 0x80U;
            return;
        }
    } else {
        display.currentStateFrame += 0.015686275f;
        if (display.currentStateFrame > 1.0f) {
            display.currentStateFrame = 1.0f;
            display.stateFrame = -0.062745102f;
            logoFlags &= ~0x80U;
        }
    }
    titleSequenceControlCallbacks[titleLoopSequence](&control);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleSaveSelection(void *) {
    DrawTitleBackgroundAndButtons(1);
    HLib::SetCurrentTaskTo(DrawTitleFileSelect);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InitializeTitleNewGameSelection(void *) {
    titleSaveBackupListener = HPLG_RegisterCallback(TitleSaveHotplugCallback);
    titleSaveSelection.phase = 0;
    titleActiveSaveSelection->inherited = 0;
    SetTitleWarningLogo(0, 0);
    titleSaveTransitionTimer = 0;
    titleSaveState = 0;
    InitializeTitleFileSelectState();
    HLib::SetCurrentTaskTo(ControlTitleFileSelect);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InitializeTitleLoadSelection(void *) {
    titleSaveBackupListener = HPLG_RegisterCallback(TitleSaveHotplugCallback);
    titleSaveSelection.phase = 0;
    SetTitleWarningLogo(0, 0);
    titleSaveTransitionTimer = 0;
    titleSaveState = 6;
    InitializeTitleFileSelectState();
    HLib::SetCurrentTaskTo(ControlTitleFileSelect);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t InitializeTitleDebugGame() {
    if (titleDebugSaveSlot < 0) {
        if (!InitializeNewGameFromSaveSlot())
            return 0;
        SetTitleDebugStartTime(g_currentMap);
        ApplyTitleMenuOptions(1);
        return 1;
    }
    if (!LoadGameFromSaveSlot(titleDebugSaveSlot, titleDebugSaveLoaded))
        return 0;
    ApplyTitleMenuOptions(0);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ControlTitleDebugSceneSelector() {
    const std::uint16_t input = g_inputBuffer[0].repeatedPresses;
    SelectTitleSceneGroup(TITLE_SCENE_GROUPS);
    SelectTitleArea(TITLE_SCENE_GROUPS[titleDebugAct].scenes);
    if (input & 0x4000U) {
        switch (titleDebugSelection) {
        case 0:
            do {
                titleDebugSaveSlot = titleDebugSaveSlot < 0 ? 3 : titleDebugSaveSlot - 1;
            } while (titleDebugSaveSlot >= 0 && !titleBackupData->saves[titleDebugSaveSlot].saveKind);
            if (titleDebugSaveSlot >= 0) {
                const BackupSlotMetadata &save = titleBackupData->saves[titleDebugSaveSlot];
                g_actNumber = save.sceneNumber;
                g_currentMap = save.mapName;
                g_entryPoint = save.entryPoint;
                titleDebugSaveLoaded = 1;
                break;
            }
            titleDebugSaveLoaded = 0;
            break;
        case 1:
            g_actNumber = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_actNumber) - 1U);
            if (g_actNumber == -1)
                g_actNumber = 99;
            titleDebugSaveLoaded = 0;
            break;
        case 2:
            if (--titleDebugAreaCharacter < -1)
                titleDebugAreaCharacter = 3;
            titleDebugSaveLoaded = 0;
            break;
        case 3:
            g_entryPoint = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_entryPoint) - 1U);
            if (g_entryPoint == -1)
                g_entryPoint = 99;
            titleDebugSaveLoaded = 0;
            break;
        }
    } else if (input & 0x8000U) {
        switch (titleDebugSelection) {
        case 0:
            do {
                titleDebugSaveSlot = titleDebugSaveSlot > 2 ? -1 : titleDebugSaveSlot + 1;
                if (titleDebugSaveSlot < 0)
                    break;
            } while (!titleBackupData->saves[titleDebugSaveSlot].saveKind);
            if (titleDebugSaveSlot >= 0) {
                const BackupSlotMetadata &save = titleBackupData->saves[titleDebugSaveSlot];
                g_actNumber = save.sceneNumber;
                g_currentMap = save.mapName;
                g_entryPoint = save.entryPoint;
                titleDebugSaveLoaded = 1;
                break;
            }
            titleDebugSaveLoaded = 0;
            break;
        case 1:
            g_actNumber = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_actNumber) + 1U);
            if (static_cast<std::uint32_t>(g_actNumber) > 99U)
                g_actNumber = 0;
            titleDebugSaveLoaded = 0;
            break;
        case 2:
            if (++titleDebugAreaCharacter > 3)
                titleDebugAreaCharacter = -1;
            titleDebugSaveLoaded = 0;
            break;
        case 3:
            g_entryPoint = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_entryPoint) + 1U);
            if (static_cast<std::uint32_t>(g_entryPoint) > 99U)
                g_entryPoint = 0;
            titleDebugSaveLoaded = 0;
            break;
        }
    }
    if (!g_currentMap)
        g_currentMap = TITLE_AREA_0000;
    shen_name area = g_currentMap;
    if (input & 0x1000U) {
        if (titleDebugAreaCharacter < 0) {
            if (--titleDebugSelection < 0)
                titleDebugSelection = 3;
        } else if (reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] == '_')
            reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] = '0';
        else if (reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] == '9')
            reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] = 'A';
        else if (reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] == 'Z')
            reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] = '_';
        else
            reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] = reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] + 1;
    } else if (input & 0x2000U) {
        if (titleDebugAreaCharacter < 0) {
            if (++titleDebugSelection > 3)
                titleDebugSelection = 0;
        } else if (reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] == '_')
            reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] = 'Z';
        else if (reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] == '0')
            reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] = '_';
        else if (reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] == 'A')
            reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] = '9';
        else
            reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] = reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] - 1;
    }
    g_currentMap = area;
    if ((g_frameCounter & 15U) > 7U && titleDebugAreaCharacter >= 0)
        reinterpret_cast<char *>(&area)[titleDebugAreaCharacter] = ' ';
    SetDebugScreenTextPosition(14, 21);
    switch (titleDebugSaveSlot) {
    case 0:
    case 1:
    case 2:
        shenmue_s2_event_DebugLogToScreen("CONTINUE - #%d:STEP%2.4f", titleDebugSaveSlot + 1,
            static_cast<double>(static_cast<float>(titleBackupData->saves[titleDebugSaveSlot].stepHundreds) * 0.0001f));
        break;
    case 3:
        shenmue_s2_event_DebugLogToScreen("CONTINUE - RAM");
        break;
    default:
        shenmue_s2_event_DebugLogToScreen("CONTINUE - OFF");
        break;
    }
    shenmue_s2_event_DebugLogToScreen(" %c\n", titleDebugSaveLoaded ? 'R' : ' ');
    shenmue_s2_event_DebugLogToScreen("SCENE    - %02d\n", g_actNumber);
    shenmue_s2_event_DebugLogToScreen("AREA     - %c%c%c%c\n", reinterpret_cast<const char *>(&area)[0], reinterpret_cast<const char *>(&area)[1],
        reinterpret_cast<const char *>(&area)[2], reinterpret_cast<const char *>(&area)[3]);
    shenmue_s2_event_DebugLogToScreen("ENTRY    - %02d\n", g_entryPoint);
    SetDebugScreenTextPosition(12, titleDebugSelection + 21);
    shenmue_s2_event_DebugLogToScreen(">");
    SetDebugScreenTextPosition(1, 25);
    if (titleCurrentAreaIndex >= 0)
        shenmue_s2_event_DebugLogToScreen("(%2d:%2d)", titleDebugAct, titleCurrentAreaIndex);
    else
        shenmue_s2_event_DebugLogToScreen("(%2d:--)", titleDebugAct);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ControlTitleDebugTimeAttack() {
    g_actNumber = 3;
    g_currentMap = shen_name(SHEN_NAME(MFBT));
    g_entryPoint = 99;
    titleTimeAttackAlpha += titleTimeAttackAlphaStep;
    if (titleTimeAttackAlpha > 255) {
        titleTimeAttackAlpha = 255;
        titleTimeAttackAlphaStep = -titleTimeAttackAlphaStep;
    } else if (titleTimeAttackAlpha < 0) {
        titleTimeAttackAlpha = 0;
        titleTimeAttackAlphaStep = -titleTimeAttackAlphaStep;
    }
    SetDebugScreenTextColour((static_cast<std::uint32_t>(titleTimeAttackAlpha) << 24U) | 0x00800000U);
    SetDebugScreenTextPosition(15, 23);
    shenmue_s2_event_DebugLogToScreen("TIME ATTACK");
    SetDebugScreenTextColour(UINT32_MAX);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t InitializeTitleDebugModule() {
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BeginTitleModule(SystemModule module, std::int32_t unpause) {
    if (unpause)
        g_gamePauseState = 0;
    ExecuteAicaCommand(0x1200a0, 5, 0);
    ChangeSystemModule(module, 0);
}
#endif

void LoadTitleSoundProgram_Wrapper(std::int32_t sceneIndex) {
    LoadTitleSoundProgram(sceneIndex);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void RestoreTitleCallbacks(std::int32_t menu) {
    TitleControlData &control = *static_cast<TitleControlData *>(HLib::GetTaskParameter(titleControlTask));
    if (menu) {
        HLib::AssignCallbackForTask(titleControlTask, TitleMenuControl);
        HLib::AssignCallbackForTask(control.titleSequenceTask, DrawTitleMenu);
    } else {
        titleSequenceTime = titleSequenceDurations[titleLoopSequence];
        HLib::AssignCallbackForTask(titleControlTask, TitleControl);
        HLib::AssignCallbackForTask(control.titleSequenceTask, titleDisplayCallbacks[titleLoopSequence]);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t HasTitleContinueSave() {
    if (GetBackupInfo(0) & 0x40U)
        return 1;
    return 0;
}
#endif

// @verified - d
std::int32_t SetTitleMenuUIState(std::int32_t mode) {
    if (mode == 1 && g_titleMenuSelection < 0) {
        g_previousTitleMenuSelection = g_titleMenuSelection = HasTitleContinueSave() != 0;
    }
    const std::int32_t previous = g_titleMenuUIState;
    g_titleMenuUIState = mode;
    return previous;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t FindLastTitleSceneGroup(const TitleSceneGroup *groups) {
    std::int32_t count = 0;
    for (;;) {
        if (groups[count].scenes == reinterpret_cast<const TitleSceneEntry *>(UINTPTR_MAX)) {
            --count;
            break;
        }
        ++count;
    }
    return count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t FindLastTitleArea(const TitleSceneEntry *scenes) {
    std::int32_t count = 0;
    while (scenes[count].act != 255)
        ++count;
    --count;
    return count;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SelectTitleSceneGroup(const TitleSceneGroup *groups) {
    const std::uint16_t input = g_inputBuffer[0].repeatedPresses;
    if (input & 2U) {
        do {
            if (titleDebugAct == 0)
                titleDebugAct = FindLastTitleSceneGroup(groups);
            else
                --titleDebugAct;
        } while (!groups[titleDebugAct].scenes);
    } else if (input & 4U) {
        do {
            ++titleDebugAct;
            if (groups[titleDebugAct].scenes == reinterpret_cast<const TitleSceneEntry *>(UINTPTR_MAX))
                titleDebugAct = 0;
        } while (!groups[titleDebugAct].scenes);
    } else {
        return;
    }
    titleCurrentAreaIndex = groups[titleDebugAct].index;
    SetTitleSceneInfo(groups[titleDebugAct].scenes[titleCurrentAreaIndex]);
    titleDebugSaveLoaded = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SelectTitleArea(const TitleSceneEntry *scenes) {
    const std::uint16_t input = *reinterpret_cast<const volatile std::uint16_t *>(&g_inputBuffer[0].repeatedPresses);
    if (titleCurrentAreaIndex == -2)
        return;
    if (input & 0x200U) {
        if (titleCurrentAreaIndex == -1)
            titleCurrentAreaIndex = FindLastTitleArea(scenes);
        else if (titleCurrentAreaIndex == 0)
            titleCurrentAreaIndex = FindLastTitleArea(scenes);
        else
            --titleCurrentAreaIndex;
    } else if (input & 0x400U) {
        if (titleCurrentAreaIndex == -1) {
            titleCurrentAreaIndex = 0;
        } else {
            ++titleCurrentAreaIndex;
            if (scenes[titleCurrentAreaIndex].act == 255)
                titleCurrentAreaIndex = 0;
        }
    } else {
        return;
    }
    SetTitleSceneInfo(scenes[titleCurrentAreaIndex]);
    titleDebugSaveLoaded = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetTitleDebugStartTime(shen_name area) {
    DateTime time;
    GetCurrentCalendarRecord(time);
    if (area == TITLE_AREA_0200) {
        time.hour = 10;
        time.minute = 0;
        time.second = 0;
        time.flags = 0;
    } else if (reinterpret_cast<const char *>(&area)[0] == 'K') {
        if (area == TITLE_AREA_KRF1) {
            time.hour = 8;
            time.minute = 0;
        } else {
            time.hour = 9;
            time.minute = 30;
        }
        time.second = 0;
        time.flags = 0;
    }
    SetCurrentDateTimeTo(time);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetTitleMenuAction() {
    return titleMenuAction;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t SetTitleMenuAction(std::int32_t action) {
    const std::int32_t previous = titleMenuAction;
    titleMenuAction = action;
    return previous;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ControlTitleNewGameConfirmation(void *) {
    if (IsSaveAllowed() || !InitializeNewGameFromSaveSlot())
        return;
    SetTitleWarningLogo(0, 0);
    g_actNumber = 1;
    g_currentMap = TITLE_AREA_0004;
    g_entryPoint = 0;
    ApplyTitleMenuOptions(1);
    BeginTitleModule(SystemModule_Game, 1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t ExecuteTitleMenuCallback(TitleControlData *control, std::int32_t callback) {
    switch (titleMenuDispatch[callback].sound) {
    case 0:
        ExecuteAicaCommand(0x900ab, 0, 0);
        break;
    case 1:
        ExecuteAicaCommand(0x800ab, 0, 0);
        break;
    }
    if (titleMenuDispatch[callback].uiState >= 0) {
        SetTitleMenuUIState(titleMenuDispatch[callback].uiState);
        ExecuteAicaCommand(0x900ab, 0, 0);
    }
    HLib::SetCurrentTaskTo(titleMenuDispatch[callback].control);
    if (control->titleSequenceTask)
        HLib::AssignCallbackForTask(control->titleSequenceTask, titleMenuDispatch[callback].display);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ControlTitleMissingContinue(void *data) {
    if (g_inputBuffer[0].pressedPriority & kTitleSkipButtons) {
        SetTitleWarningLogo(0, 0);
        ExecuteTitleMenuCallback(static_cast<TitleControlData *>(data), 0);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ControlTitleOptions(void *data) {
    std::uint16_t buttons = g_inputBuffer[0].pressedPriority;
    if (buttons & kTitleSkipButtons) {
        ExecuteTitleMenuCallback(static_cast<TitleControlData *>(data), 0);
        return;
    }
    const std::int32_t previousSelection = titleOptionsSelection;
    if (buttons & 0x2000U) {
        if (++titleOptionsSelection > 3)
            titleOptionsSelection = 0;
        buttons &= 0x3fffU;
    }
    if (buttons & 0x1000U) {
        if (--titleOptionsSelection < 0)
            titleOptionsSelection = 3;
        buttons &= 0x3fffU;
    }
    const std::int32_t previousValue = titleOptionValues[titleOptionsSelection];
    if (titleOptionsSelection != 4) {
        std::int32_t value = previousValue;
        if (buttons & 0x4000U) {
            if (--value < 0)
                value = titleOptionsSelection == 0 ? titleOptionLimits[titleOptionsSelection] - 1 : 0;
        }
        if (buttons & 0x8000U) {
            if (++value >= titleOptionLimits[titleOptionsSelection]) {
                value = 0;
                if (titleOptionsSelection) {
                    const volatile std::int32_t &selection = titleOptionsSelection;
                    const volatile std::int8_t *const limits = titleOptionLimits;
                    value = limits[selection] - 1;
                }
            }
        }
        titleOptionValues[titleOptionsSelection] = value;
    }
    if ((previousSelection ^ titleOptionsSelection) | (titleOptionValues[titleOptionsSelection] ^ previousValue))
        ExecuteAicaCommand(0x500ab, 0, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void TitleMenuControl(void *data) {
    titlePendingMenuRequest = 0;
    std::uint16_t buttons = g_inputBuffer[0].pressedPriority;
    std::uint16_t directions = g_inputBuffer[0].repeatedPresses;
    if (titleWheelRotation) {
        buttons = 0;
        directions = 0;
    }
    g_previousTitleMenuSelection = g_titleMenuSelection;
    if (directions & 0x4000U) {
        if (--g_titleMenuSelection < 0)
            g_titleMenuSelection = 2;
        buttons = 0;
        directions = 0;
    }
    if (directions & 0x8000U) {
        if (++g_titleMenuSelection > 2)
            g_titleMenuSelection = 0;
        buttons = 0;
    }
    if (g_previousTitleMenuSelection != g_titleMenuSelection)
        ExecuteAicaCommand(0x200ab, 0, 0);
    if (buttons & 0x400U) {
        switch (g_titleMenuSelection) {
        case 0:
            if (g_loadedActNumber == 1)
                titlePendingMenuRequest = &titleMenuRequests[0];
            else
                ExecuteAicaCommand(0x800ab, 0, 0);
            break;
        case 1:
            if (HasTitleContinueSave())
                titlePendingMenuRequest = &titleMenuRequests[1];
            else
                titlePendingMenuRequest = &titleMenuRequests[4];
            break;
        case 2:
            titlePendingMenuRequest = &titleMenuRequests[2];
            break;
        default:
            ExecuteAicaCommand(0x800ab, 0, 0);
            break;
        }
        if (titlePendingMenuRequest) {
            if (titlePendingMenuRequest->action >= 0)
                SetTitleMenuAction(titlePendingMenuRequest->action);
            ExecuteTitleMenuCallback(static_cast<TitleControlData *>(data), titlePendingMenuRequest->callback);
        }
    } else if (buttons & 0x200U) {
        ExecuteAicaCommand(0x900ab, 0, 0);
        g_titleMenuUIState = 0;
        g_gamePauseState = 1;
        RestoreTitleCallbacks(0);
        LoadTitleSoundProgram(-1);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleMenuWheel() {
    std::int32_t item, angle;
    const TitleTextureRectangle *orb;
    Vector3 position;
    float scale, orbitY, orbitX;
    if (titleWheelRotation > 0) {
        titleWheelRotation -= 0xaaa;
        if (titleWheelRotation < 255)
            titleWheelRotation = 0;
    } else if (titleWheelRotation < 0) {
        titleWheelRotation += 0xaaa;
        if (titleWheelRotation > -255)
            titleWheelRotation = 0;
    }
    const std::int32_t selectionStep = g_titleMenuSelection - g_previousTitleMenuSelection;
    if (titleWheelRotation == 0) {
        switch (selectionStep) {
        case -1:
        case 2:
            titleWheelRotation = 0x4aab;
            break;
        case -2:
        case 1:
            titleWheelRotation = -0x4aab;
            break;
        }
    }
    for (item = 0; item < 3; ++item) {
        orb = &titleMenuOrbRectangles[1];
        position.z = 1.2f;
        angle = static_cast<std::int32_t>(static_cast<float>((g_titleMenuSelection - item) * 0x5555)) + titleWheelRotation;
        orbitY = dc::sdk::njCos(angle);
        scale = orbitY + 2.5f;
        orbitY = orbitY * 70.0f + 200.0f;
        orbitX = dc::sdk::njSin(angle) * (scale * 70.0f) + 320.0f;
        scale = (scale * 0.2f + 0.2f) * 1.1111111640930176f;
        if (item == g_titleMenuSelection)
            orb = &titleMenuOrbRectangles[0];
        position.x = scale * 10.0f + orbitX;
        position.y = scale * 15.0f + orbitY;
        DrawScaledTitleSprite(orb, position, 7, scale);
        position.z = 1.4000000953674316f;
        DrawScaledTitleSprite(&titleMenuLabelRectangles[item], position, 7, scale);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleMenu(void *) {
    Vector3 position;
    position.x = 160.0f;
    position.y = 59.0f;
    position.z = 1.2f;
    DrawScaledTitleSprite(&titleMenuHeadingRectangles[0], position, 7, 1.0f);
    position.x = 480.0f;
    DrawScaledTitleSprite(&titleMenuHeadingRectangles[1], position, 7, 1.0f);
    DrawTitleMenuWheel();
    if (g_titleMenuSelection == 1 && !HasTitleContinueSave())
        DrawTitleInfoBar(3, -1);
    else
        DrawTitleInfoBar(g_titleMenuSelection, -1);
    DrawTitleBackgroundAndButtons(UINT32_MAX);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ApplyTitleMenuOptions(std::int32_t) {
    SetNormalTriggerMapping(titleOptionValues[TitleOptionTrigger] == 0);
    SetSystemAudioOutputMode(titleOptionValues[TitleOptionAudio]);
    ApplyS2DialogueMode(static_cast<std::uint8_t>(titleOptionValues[TitleOptionDialogue]));
    ApplyS2ControlMode(titleOptionValues[TitleOptionControl]);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void TitleUnusedNullsub() {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawScaledTitleSprite(const TitleTextureRectangle *rectangle, const Vector3 &position, std::int32_t texture, float scale) {
    std::int32_t width = titleTextureDimensions[texture][0];
    std::int32_t height = titleTextureDimensions[texture][1];
    InitializeTitlePacket(titleSpritePacket);
    titleSpritePacket.quad.flags |= 0x400U;
    if (rectangle) {
        titleSpritePacket.uv[0] = static_cast<float>(rectangle->left) * 0.001f;
        titleSpritePacket.uv[1] = static_cast<float>(rectangle->top) * 0.001f;
        titleSpritePacket.uv[2] = static_cast<float>(rectangle->right) * 0.001f;
        titleSpritePacket.uv[3] = static_cast<float>(rectangle->bottom) * 0.001f;
        width = static_cast<std::int32_t>(static_cast<float>(rectangle->right - rectangle->left) * 0.001f * static_cast<float>(width));
        height = static_cast<std::int32_t>(static_cast<float>(rectangle->bottom - rectangle->top) * 0.001f * static_cast<float>(height));
    }
    AdjustScreenAspectRatio(titleSpritePacket.quad, HorizontalStretch, static_cast<std::int32_t>(position.x), static_cast<std::int32_t>(position.y),
        static_cast<std::int32_t>(static_cast<float>(width) * scale), static_cast<std::int32_t>(static_cast<float>(height) * scale));
    DrawLegacySpritePacket(titleSpritePacket, titleSpriteTextures[texture], position.z);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleMenuBackground() {
    InitializeTitlePacket(titleSpritePacket);
    titleSpritePacket.quad.flags |= 0x400U;
    AdjustScreenAspectRatio(titleSpritePacket.quad, HorizontalStretch, 320, 240, 640, 640);
    DrawLegacySpritePacket(titleSpritePacket, titleSpriteTextures[10], 1.1f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleControllerIcons() {
    for (std::int32_t icon = 0; icon < 3; ++icon) {
        const float u = static_cast<float>(icon & 1) * 0.5f;
        const float v = static_cast<float>((icon & 2) >> 1) * 0.5f;
        InitializeTitlePacket(titleSpritePacket);
        titleSpritePacket.quad.flags |= 0x400U;
        AdjustScreenAspectRatio(titleSpritePacket.quad, HorizontalStretch, 485 + icon * 48, 54, 32, 32);
        titleSpritePacket.uv[0] = u;
        titleSpritePacket.uv[1] = v;
        titleSpritePacket.uv[2] = u + 0.5f;
        titleSpritePacket.uv[3] = v + 0.5f;
        DrawLegacySpritePacket(titleSpritePacket, titleSpriteTextures[13], 1.3000000715255737f);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleInfoLine(std::int32_t line, float y) {
    const float bottom = static_cast<float>(17 - line) * 30.0f * 0.001953125f;
    const float top = static_cast<float>(16 - line) * 30.0f * 0.001953125f;
    InitializeTitlePacket(titleSpritePacket);
    titleSpritePacket.quad.flags |= 0x400U;
    AdjustScreenAspectRatio(titleSpritePacket.quad, HorizontalStretch, 320, static_cast<std::int32_t>(y), 512, 30);
    titleSpritePacket.uv[0] = 0.0f;
    titleSpritePacket.uv[1] = top;
    titleSpritePacket.uv[2] = 1.0f;
    titleSpritePacket.uv[3] = bottom;
    DrawLegacySpritePacket(titleSpritePacket, titleSpriteTextures[11], 1.4000000953674316f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleInfoBar(std::int32_t line, std::int32_t secondaryLine) {
    float y = 415.0f;
    if (secondaryLine >= 0) {
        void (*drawLine)(std::int32_t, float) = DrawTitleInfoLine;
        drawLine(secondaryLine, 400.0f);
        y = 430.0f;
    }
    DrawTitleInfoLine(line, y);
    InitializeTitlePacket(titleSpritePacket);
    titleSpritePacket.quad.flags |= 0x400U;
    AdjustScreenAspectRatio(titleSpritePacket.quad, HorizontalStretch, 320, 415, 512, 128);
    DrawLegacySpritePacket(titleSpritePacket, titleSpriteTextures[12], 1.2f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleBackgroundAndButtons(std::uint32_t flags) {
    if (flags & 1U)
        DrawTitleMenuBackground();
    if (flags & 2U)
        DrawTitleControllerIcons();
}
#endif

// @verified - d
void ResetTitleMenuState() {
    titleOptionValues[TitleOptionDialogue] = GetS2DialogueMode();
    titleOptionValues[TitleOptionAudio] = GetSystemAudioOutputMode();
    titleOptionValues[TitleOptionTrigger] = GetNormalTriggerMapping() == 0;
    titleOptionValues[TitleOptionControl] = GetS2ControlMode();
    SetTitleLogoPresent();
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleOptionSprite(std::int32_t texture, std::int32_t rectangle, const Vector3 &position) {
    const std::int32_t table = texture - 8;
    static const TitleTextureRectangle *const optionRectangleSets[2] = {titleOptionPanelRectangles, titleOptionLabelRectangles};
    DrawScaledTitleSprite(&optionRectangleSets[table][rectangle], position, texture, 1.0f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DrawTitleOptionValue(const std::int16_t *labels, std::int32_t value, float y) {
    Vector3 position;
    position.y = y;
    value &= 1;
    titleOptionChoiceCursor = &titleOptionChoiceStyles[value * 2];
    for (std::int32_t choice = 0; choice < 2; ++choice) {
        position.z = 1.2f;
        position.x = static_cast<float>(titleOptionArrowPositions[choice]);
        DrawTitleOptionSprite(8, titleOptionChoiceCursor->arrow, position);
        position.x = static_cast<float>(choice * 210 + 310);
        DrawTitleOptionSprite(8, titleOptionChoiceCursor->panel, position);
        position.z = 1.4000000953674316f;
        const std::int16_t &label = labels[choice];
        DrawTitleOptionSprite(9, label, position);
        ++titleOptionChoiceCursor;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleOptions(void *) {
    Vector3 position;
    position.x = 160.0f;
    position.y = 59.0f;
    position.z = 1.2f;
    DrawTitleOptionSprite(8, 0, position);
    position.x = 480.0f;
    DrawTitleOptionSprite(8, 1, position);
    for (std::int32_t option = 0; option < 4; ++option) {
        position.x = 130.0f;
        position.z = 1.2f;
        std::int32_t sprite = 3;
        if (option == titleOptionsSelection)
            sprite = 2;
        position.y = static_cast<float>(option * 45 + 140);
        DrawTitleOptionSprite(8, sprite, position);
        position.z = 1.4000000953674316f;
        DrawTitleOptionSprite(9, titleOptionLabels[option].rectangle, position);
    }
    position.x = 415.0f;
    position.y = 140.0f;
    position.z = 1.2f;
    DrawTitleOptionSprite(8, 6, position);
    position.z = 1.4000000953674316f;
    DrawTitleOptionSprite(9, titleOptionDialogueLabels[titleOptionValues[TitleOptionDialogue]], position);
    position.z = 1.2f;
    position.x = 300.0f;
    DrawTitleOptionSprite(8, 7, position);
    position.x = 525.0f;
    DrawTitleOptionSprite(8, 8, position);
    for (std::int32_t option = 1; option < 4; ++option)
        DrawTitleOptionValue(titleOptionValueLabels[option - 1], titleOptionValues[option], static_cast<float>(option * 45 + 140));
    const std::int8_t *information;
    std::int32_t index;
    if (!titleOptionsSelection) {
        information = titleOptionDialogueInfo[0];
        index = titleOptionValues[TitleOptionDialogue];
    } else {
        information = titleOptionItemInfo[0];
        index = titleOptionsSelection;
    }
    std::int32_t first = information[index * 2];
    ++information;
    DrawTitleInfoBar(first, information[index * 2]);
    DrawTitleBackgroundAndButtons(UINT32_MAX);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleMissingContinue(void *data) {
    SetTitleWarningLogo(11, 0);
    DrawTitleMenu(data);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawTitleNewGameConfirmation(void *) {
    DrawTitleBackgroundAndButtons(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetTitleWarningSelection(std::int32_t selection) {
    if (selection >= 0) {
        titleWarningUsesLogoParameters = 1;
        titleWarningLogoParameters[1] = static_cast<std::uint8_t>(selection);
    } else {
        titleWarningUsesLogoParameters = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void BindTitleWarningDecorationSprites(std::int32_t texture) {
    BindTempSprite(texture, 1, 33, titleWarningDecorationRectangles);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void LoadTitleWarningLogo(TitleWarningTaskData *data, std::int32_t texture, std::int32_t x, std::int32_t y) {
    if (data->textureIndex != texture) {
        if (data->textureIndex >= 0) {
            FreeTempSprites(data->textureIndex);
            data->textureIndex = -1;
            return;
        }
        if (AllocateTempSprites(texture) != 1)
            return;
        BindTitleWarningDecorationSprites(texture);
    }
    data->textureIndex = texture;
    DrawTempSpriteSlot(static_cast<std::uint32_t>(texture) << 16, (static_cast<std::uint32_t>(x) << 16) | static_cast<std::uint32_t>(y), 13.0f, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void DrawTitleWarningDecorations(std::int32_t logo, const std::uint8_t *parameters) {
    switch (logo) {
    case 28:
        DrawTempSpriteSlot(0x210000U | (parameters[2] + 10U), 0x12e00dcU, 14.0f, 0);
        DrawTempSpriteSlot(0x210000U | (parameters[3] + 1U), 0x1d900dcU, 14.0f, 0);
        break;
    case 36:
        DrawTempSpriteSlot(0x210000U | parameters[5], 0x13100bfU, 14.0f, 0);
        DrawTempSpriteSlot(0x210000U | parameters[6], 0x0ee0110U, 14.0f, 0);
        break;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void DrawTitleWarningSelections(std::int32_t index) {
    static const std::int32_t *warningSelection;
    warningSelection = titleWarningSelections[index];
    DrawTempSpriteSlot((static_cast<std::uint32_t>(warningSelection[0]) << 16) | warningSelection[1], 0x00e600a2U, 13.0f, 16);
    warningSelection += 2;
    DrawTempSpriteSlot((static_cast<std::uint32_t>(warningSelection[0]) << 16) | warningSelection[1], 0x019a00a2U, 13.0f, 16);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void DrawTitleWarningLogo(TitleWarningTaskData *data, std::int32_t logo, const std::uint8_t *parameters) {
    std::int16_t *entry = titleWarningLogos[logo];
    if (entry[0] < 0)
        return;
    LoadTitleWarningLogo(data, entry[1], 320, titleWarningLogoY[entry[0]]);
    if (!parameters)
        return;
    DrawTitleWarningDecorations(logo, parameters);
    if (entry[0] == 3)
        DrawTitleWarningSelections(parameters[1]);
}

void SubmitTitleWarningLogo(void *) {
    TitleWarningTaskData *const data = static_cast<TitleWarningTaskData *>(HLib::GetTaskParameter(titleWarningTask));
    if (titleWarningUsesLogoParameters)
        DrawTitleWarningLogo(data, titleWarningLogo, titleWarningLogoParameters);
    else
        DrawTitleWarningLogo(data, titleWarningLogo, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitTitleWarningTaskData(TitleWarningTaskData *data) {
    data->textureIndex = -1;
}
#endif

// @verified - d
void SetTitleWarningLogo(std::int32_t logo, const void *parameters) {
    if (logo > 43)
        logo = 0;
    titleWarningLogo = logo;
    if (parameters) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        std::int32_t enabled;
        enabled = 1;
        titleWarningUsesLogoParameters = enabled;
#else
        titleWarningUsesLogoParameters = 1;
#endif
        std::memcpy(titleWarningLogoParameters, parameters, sizeof(titleWarningLogoParameters));
    } else
        titleWarningUsesLogoParameters = 0;
}

// @verified - d
void InitializeTitleCommonTempSprites() {
    if (AllocateTempSprites(0) == 1) {
        BindTempSprite(0, 0, 35, titleCommonTempRectangles);
        BindTempSprite(0, 1, 36, titleCommonTempRectangles);
    }
}

// @verified - d
void FreeTitleWarningResources() {
    ReleaseBorrowedLegacySpriteSlot(36);
    ReleaseBorrowedLegacySpriteSlot(35);
    FreeTempSprites(0);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
TitleWarningTaskData *GetTitleWarningTaskData() {
    return static_cast<TitleWarningTaskData *>(HLib::GetTaskParameter(titleWarningTask));
}
#endif

} // namespace s2
} // namespace shenmue

#endif
