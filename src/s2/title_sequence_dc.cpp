#include "s2/title_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/sdk.hpp"
#include "s2/area_names.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/sound_dc.hpp"
#include "s2/sound_program.hpp"
#include "s2/startup_assets_dc.hpp"
#include "s2/system_modules_dc.hpp"

namespace shenmue {
namespace s2 {

void ResetTitleMenuState();

const TitleSceneEntry TITLE_SCENES_DEBUG[35] = {
    {1, {'T', 'E', 'S', 'T'}, 0},
    {1, {'M', 'A', 'P', '0'}, 0},
    {1, {'V', 'I', 'E', 'W'}, 0},
    {1, {'1', '0', '0', '0'}, 0},
    {2, {'A', 'B', '0', '0'}, 1},
    {3, {'Q', 'A', '0', '0'}, 1},
    {3, {'Q', 'C', '0', '0'}, 1},
    {3, {'Q', 'E', '0', '0'}, 1},
    {3, {'Q', 'F', '0', '0'}, 1},
    {3, {'Q', 'F', '3', '9'}, 1},
    {3, {'Q', '1', '0', '0'}, 1},
    {3, {'Q', 'T', 'B', '1'}, 1},
    {3, {'Q', 'S', 'F', 'A'}, 1},
    {3, {'Q', 'S', 'F', 'B'}, 1},
    {3, {'Q', 'S', 'F', 'C'}, 1},
    {3, {'Q', 'S', 'F', 'D'}, 1},
    {3, {'Q', 'G', 'B', 'T'}, 1},
    {3, {'Q', 'J', 'B', 'T'}, 1},
    {3, {'Q', 'K', 'B', 'T'}, 1},
    {3, {'Q', 'L', 'B', 'T'}, 1},
    {2, {'B', 'K', 'W', '0'}, 0},
    {3, {'G', 'A', 'B', 'T'}, 0},
    {2, {'J', 'I', 'A', 'G'}, 0},
    {2, {'S', 'Y', 'W', '0'}, 0},
    {1, {'W', 'B', '0', '0'}, 1},
    {1, {'W', 'K', '0', '0'}, 1},
    {2, {'A', 'K', '0', '0'}, 1},
    {2, {'W', 'B', '0', '0'}, 1},
    {2, {'W', 'K', '0', '0'}, 1},
    {2, {'W', 'S', '0', '0'}, 1},
    {3, {'Q', 'D', 'T', 'B'}, 0},
#if SHENMUE_IS_DC
    {3, {'S', 'T', 'E', 'P'}, 0},
    {3, {'T', 'G', 'L', 'B'}, 0},
#else
    {3, {'Q', 'F', 'R', 'R'}, 1},
    {3, {'S', 'T', 'E', 'P'}, 0},
#endif
    {4, {'G', 'D', '0', '0'}, 1},
    {255, {0, 0, 0, 0}, 0},
};

const TitleSceneEntry TITLE_SCENES_SCENE1[21] = {
    {1, {'A', 'K', '0', '0'}, 1},
    {1, {'A', 'K', 'A', '3'}, 1},
    {1, {'A', 'K', 'T', '0'}, 1},
    {1, {'A', 'R', '0', '1'}, 0},
    {1, {'A', 'R', '0', '2'}, 25},
    {1, {'A', 'R', '0', '3'}, 28},
    {1, {'W', 'B', '0', '0'}, 1},
    {1, {'W', 'B', '0', '1'}, 99},
    {1, {'W', 'E', '0', '0'}, 1},
    {1, {'W', 'E', 'G', '0'}, 1},
    {1, {'W', 'E', 'M', '1'}, 1},
    {1, {'W', 'K', '0', '0'}, 1},
    {1, {'W', 'K', 'A', '0'}, 1},
    {1, {'W', 'N', '0', '0'}, 1},
    {1, {'W', 'R', '0', '0'}, 1},
    {1, {'W', 'R', 'S', '2'}, 1},
    {1, {'W', 'S', '0', '0'}, 1},
    {1, {'W', 'S', 'Y', '0'}, 1},
    {1, {'W', 'T', '0', '0'}, 1},
    {1, {'W', 'T', 'A', '0'}, 1},
    {255, {0, 0, 0, 0}, 0},
};

const TitleSceneEntry TITLE_SCENES_SCENE2[23] = {
    {2, {'A', 'B', '0', '0'}, 1},
    {2, {'A', 'K', '0', '0'}, 1},
    {2, {'A', 'K', 'T', '0'}, 1},
    {2, {'A', 'K', 'A', '3'}, 1},
    {2, {'A', 'R', '0', '1'}, 0},
    {2, {'A', 'R', '0', '2'}, 25},
    {2, {'A', 'R', '0', '3'}, 28},
    {2, {'A', 'R', 'C', '0'}, 1},
    {2, {'W', 'B', '0', '0'}, 1},
    {2, {'W', 'B', '0', '1'}, 99},
    {2, {'W', 'E', '0', '0'}, 1},
    {2, {'W', 'E', 'G', '0'}, 1},
    {2, {'W', 'E', 'M', '1'}, 1},
    {2, {'W', 'K', '0', '0'}, 1},
    {2, {'W', 'K', 'A', '0'}, 1},
    {2, {'W', 'N', '0', '0'}, 1},
    {2, {'W', 'R', '0', '0'}, 1},
    {2, {'W', 'R', 'S', '2'}, 1},
    {2, {'W', 'S', '0', '0'}, 1},
    {2, {'W', 'S', 'Y', '0'}, 1},
    {2, {'W', 'T', '0', '0'}, 1},
    {2, {'W', 'T', 'A', '0'}, 1},
    {255, {0, 0, 0, 0}, 0},
};

const TitleSceneEntry TITLE_SCENES_SCENE3[33] = {
    {3, {'Q', '1', '0', '0'}, 0},
    {3, {'Q', '2', '0', '0'}, 0},
    {3, {'Q', '3', '0', '0'}, 0},
    {3, {'Q', 'A', '0', '0'}, 0},
    {3, {'Q', 'A', 'W', '1'}, 0},
    {3, {'Q', 'A', 'E', '1'}, 0},
    {3, {'Q', 'A', 'W', '6'}, 0},
    {3, {'Q', 'A', 'E', '6'}, 0},
    {3, {'Q', 'A', '1', '1'}, 0},
    {3, {'Q', 'A', 'X', 'X'}, 1},
    {3, {'Q', 'B', '0', '0'}, 1},
    {3, {'Q', 'C', '0', '0'}, 0},
    {3, {'Q', 'C', '0', '1'}, 0},
    {3, {'Q', 'C', '0', '6'}, 0},
    {3, {'Q', 'D', '0', '0'}, 0},
    {3, {'Q', 'D', '0', '1'}, 0},
    {3, {'Q', 'E', '0', '0'}, 1},
    {3, {'Q', 'E', '0', '1'}, 0},
    {3, {'Q', 'E', '0', '6'}, 0},
    {3, {'Q', 'E', 'E', 'C'}, 1},
    {3, {'Q', 'E', 'H', 'I'}, 1},
    {3, {'Q', 'F', '0', '0'}, 0},
    {3, {'Q', 'F', '0', '1'}, 0},
    {3, {'Q', 'F', '0', '2'}, 0},
    {3, {'Q', 'F', '0', '6'}, 0},
    {3, {'Q', 'F', '3', '9'}, 0},
    {3, {'Q', 'F', '4', '0'}, 1},
    {3, {'Q', 'R', '0', '0'}, 1},
    {3, {'Q', 'R', 'C', '0'}, 1},
    {3, {'Q', 'R', 'R', '0'}, 1},
    {3, {'Q', 'T', 'B', '1'}, 1},
    {3, {'Q', 'U', 'G', '0'}, 1},
    {255, {0, 0, 0, 0}, 0},
};

const TitleSceneEntry TITLE_SCENES_SCENE4[23] = {
    {4, {'K', 'R', 'F', '1'}, 0},
    {4, {'K', 'R', 'H', '1'}, 0},
    {4, {'K', 'W', 'M', '1'}, 0},
    {4, {'K', 'R', 'M', '3'}, 0},
    {4, {'K', 'W', 'W', '1'}, 0},
    {4, {'K', 'W', 'W', 'Q'}, 0},
    {4, {'K', 'W', 'Q', 'A'}, 0},
    {4, {'K', 'W', 'A', '1'}, 0},
    {4, {'K', 'W', 'Q', 'B'}, 0},
    {4, {'K', 'W', 'A', '2'}, 0},
    {4, {'K', 'W', 'Q', 'C'}, 0},
    {4, {'K', 'R', 'C', '1'}, 0},
    {4, {'K', 'M', 'Z', '1'}, 0},
    {4, {'K', 'R', 'K', '2'}, 0},
    {4, {'K', 'M', 'Z', '2'}, 0},
    {4, {'K', 'R', 'K', '4'}, 0},
    {4, {'K', 'M', 'Z', '3'}, 0},
    {4, {'K', 'R', 'K', '6'}, 0},
    {4, {'K', 'M', 'Z', '4'}, 0},
    {4, {'K', 'S', 'H', '1'}, 0},
    {4, {'K', 'W', 'W', '4'}, 0},
    {4, {'K', 'E', 'S', '1'}, 0},
    {255, {0, 0, 0, 0}, 0},
};

const TitleSceneEntry TITLE_SCENES_EVENT1[18] = {
    {1, {'A', 'N', 'A', 'N'}, 0},
    {1, {'A', 'R', 'B', 'T'}, 0},
    {1, {'A', 'R', 'M', 'W'}, 1},
    {1, {'B', 'G', 'S', 'M'}, 0},
    {1, {'B', 'O', 'S', 'Y'}, 0},
    {1, {'C', 'W', 'O', 'N'}, 1},
    {1, {'D', 'A', 'B', 'O'}, 0},
    {1, {'D', 'M', 'A', 'S'}, 4},
    {1, {'D', 'U', 'C', 'K'}, 0},
    {1, {'J', 'I', 'A', 'G'}, 0},
    {1, {'K', 'N', 'M', 'N'}, 99},
    {1, {'S', 'L', 'O', 'T'}, 0},
    {1, {'U', 'E', 'D', 'E'}, 0},
    {1, {'W', 'B', 'S', 'S'}, 0},
    {1, {'W', 'K', 'K', 'T'}, 99},
    {1, {'W', 'S', 'S', 'F'}, 0},
    {1, {'W', 'T', 'K', 'A'}, 0},
    {255, {0, 0, 0, 0}, 0},
};

const TitleSceneEntry TITLE_SCENES_EVENT2[15] = {
    {2, {'A', 'H', 'Q', 'B'}, 0},
    {2, {'A', 'R', 'B', 'T'}, 0},
    {2, {'A', 'R', 'K', 'Q'}, 0},
    {2, {'K', 'E', 'W', '0'}, 0},
    {2, {'K', 'N', 'W', '0'}, 0},
    {2, {'M', 'F', 'W', '0'}, 0},
    {2, {'A', 'B', 'L', 'B'}, 0},
    {2, {'T', 'O', 'F', 'B'}, 0},
    {2, {'U', 'E', 'D', 'E'}, 0},
    {2, {'V', 'S', 'Y', 'B'}, 0},
    {2, {'W', 'R', 'S', 'Q'}, 0},
    {2, {'W', 'T', 'K', 'Q'}, 0},
    {2, {'Y', 'T', 'Q', 'B'}, 0},
    {2, {'Y', 'M', 'Q', 'B'}, 0},
    {255, {0, 0, 0, 0}, 0},
};

const TitleSceneEntry TITLE_SCENES_EVENT3[12] = {
    {3, {'Q', '7', 'Q', 'B'}, 0},
    {3, {'Q', 'E', 'S', '1'}, 0},
    {3, {'Q', 'G', 'B', 'T'}, 0},
    {3, {'Q', 'J', 'B', 'T'}, 0},
    {3, {'Q', 'K', 'B', 'T'}, 0},
    {3, {'Q', 'L', 'B', 'T'}, 0},
    {3, {'Q', 'S', 'F', 'A'}, 0},
    {3, {'Q', 'S', 'F', 'B'}, 0},
    {3, {'Q', 'S', 'F', 'C'}, 0},
    {3, {'Q', 'S', 'F', 'D'}, 0},
    {3, {'S', 'T', 'E', 'P'}, 0},
    {255, {0, 0, 0, 0}, 0},
};

const TitleSceneEntry TITLE_SCENES_MOVIE[9] = {
    {1, {'O', 'P', '0', '2'}, 0},
    {1, {'0', '0', '0', '1'}, 0},
    {1, {'0', '0', '0', '4'}, 0},
    {1, {'0', '0', '0', '5'}, 0},
    {3, {'0', '2', '0', '0'}, 0},
    {3, {'0', '2', '7', '8'}, 0},
    {3, {'0', '5', '0', '0'}, 0},
    {4, {'0', '5', '0', '0'}, 0},
    {255, {0, 0, 0, 0}, 0},
};

const TitleSceneEntry *const TITLE_SCENE_TABLES[9] = {
    TITLE_SCENES_DEBUG,
    TITLE_SCENES_SCENE1,
    TITLE_SCENES_SCENE2,
    TITLE_SCENES_SCENE3,
    TITLE_SCENES_SCENE4,
    TITLE_SCENES_EVENT1,
    TITLE_SCENES_EVENT2,
    TITLE_SCENES_EVENT3,
    TITLE_SCENES_MOVIE,
#if !SHENMUE_IS_DC
    TITLE_SCENES_EXTRAS,
#endif
};

TitleSceneGroup TITLE_SCENE_GROUPS[11] = {
    {0, {0, 0, 0}, TITLE_SCENES_DEBUG},
    {0, {0, 0, 0}, TITLE_SCENES_SCENE1},
    {0, {0, 0, 0}, TITLE_SCENES_SCENE2},
    {0, {0, 0, 0}, TITLE_SCENES_SCENE3},
    {0, {0, 0, 0}, TITLE_SCENES_SCENE4},
    {0, {0, 0, 0}, TITLE_SCENES_EVENT1},
    {0, {0, 0, 0}, TITLE_SCENES_EVENT2},
    {0, {0, 0, 0}, TITLE_SCENES_EVENT3},
    {0, {0, 0, 0}, TITLE_SCENES_MOVIE},
#if SHENMUE_IS_DC
    {0, {0, 0, 0}, reinterpret_cast<const TitleSceneEntry *>(static_cast<std::uintptr_t>(-1))},
    {},
#else
    {0, {0, 0, 0}, TITLE_SCENES_EXTRAS},
    {0, {0, 0, 0}, reinterpret_cast<const TitleSceneEntry *>(static_cast<std::uintptr_t>(-1))},
#endif
};

std::int16_t titleSelectedAct = -1;

std::int32_t titleDebugAct, titleCurrentAreaIndex = 0;

// @verified - d
void SetTitleSceneInfo(const TitleSceneEntry &scene) {
    if (scene.act == 255) {
        g_actNumber = 0;
        g_currentMap = TITLE_AREA_0000;
        g_entryPoint = 0;
        titleCurrentAreaIndex = -2;
    } else {
        g_actNumber = scene.act;
        g_currentMap = (static_cast<std::uint32_t>(static_cast<std::uint8_t>(scene.area[3])) << 24U) |
            (static_cast<std::uint32_t>(static_cast<std::uint8_t>(scene.area[2])) << 16U) |
            (static_cast<std::uint32_t>(static_cast<std::uint8_t>(scene.area[1])) << 8U) | static_cast<std::uint32_t>(static_cast<std::uint8_t>(scene.area[0]));
        g_entryPoint = static_cast<std::uint8_t>(scene.entry);
    }
}

// @verified - d
void InitializeTitleAreaSelection() {
    std::uint16_t act = titleSelectedAct;
    if (act != 0xffffU)
        titleDebugAct = act;
    else
        titleDebugAct = static_cast<std::uint16_t>(g_loadedActNumber);
    titleCurrentAreaIndex = TITLE_SCENE_GROUPS[titleDebugAct].index;
    SetTitleSceneInfo(TITLE_SCENE_GROUPS[titleDebugAct].scenes[titleCurrentAreaIndex]);
}

// @verified - d
void SetTitleLogoPresent() {
    logoFlags |= 4U;
}

// @verified - d
void CheckTitleDebugInput(void *data) {
    TitleControlData &control = *static_cast<TitleControlData *>(data);
    if (!(control.display->currentStateFrame < 1.0f) && (g_inputBuffer[0].pressedPriority & kTitleStartButton)) {
        ResetTitleMenuState();
        control.state = 1;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DebugTitleControl(void *) {
    const std::uint16_t buttons = g_inputBuffer[0].pressedPriority;
    if (buttons & 0xc0U) {
        titleCurrentCoreModule = buttons & 0x40U ? titleCurrentCoreModule - 1 : titleCurrentCoreModule + 1;
        if (g_loadedActNumber == 3) {
            if (titleCurrentCoreModule < 0)
                titleCurrentCoreModule = 8;
            else if (titleCurrentCoreModule > 8)
                titleCurrentCoreModule = 0;
        } else {
            if (titleCurrentCoreModule < 0)
                titleCurrentCoreModule = 7;
            else if (titleCurrentCoreModule > 7)
                titleCurrentCoreModule = 0;
        }
    }
    if (titleDebugModules[titleCurrentCoreModule].control)
        (*titleDebugModules[titleCurrentCoreModule].control)();
    SetDebugScreenTextPosition(14, 21);
    shenmue_s2_event_DebugLogToScreen("%s", titleDebugModules[titleCurrentCoreModule].name);
    if ((buttons & 0x800U) && (*titleDebugModules[titleCurrentCoreModule].initialize)())
        BeginTitleModule(titleDebugModules[titleCurrentCoreModule].module, 1);
}
#endif

// @verified - d
void LoadTitleIntroArea(void *) {
    g_actNumber = 1;
    g_currentMap = AREA_0001;
    g_entryPoint = 1;
    BeginTitleModule(SystemModule_Game, 0);
}

// @verified - d
void LoadTitleAttractArea(void *) {
    g_actNumber = 1;
    g_currentMap = AREA_OP02;
    g_entryPoint = 1;
    dc::sdk::njSetBackColor(0xff000000U, 0xff000000U, 0xff000000U);
    BeginTitleModule(SystemModule_Game, 0);
}

// @verified - d
void SkipTitleWarning(void *data) {
    if (!(g_inputBuffer[0].pressedPriority & kTitleSkipButtons))
        return;
    TitleControlData &control = *static_cast<TitleControlData *>(data);
    TitleDisplayData *display = control.display;
    titleSequenceTime = 0;
    display->currentStateFrame = 1.0f;
    display->stateFrame = -0.062745102f;
    logoFlags &= ~0x80U;
}

// @verified - d
void AcceptTitlePressStart(void *data) {
    if (!(g_inputBuffer[0].pressedPriority & kTitleStartButton))
        return;
    TitleControlData &control = *static_cast<TitleControlData *>(data);
    ResetTitleMenuState();
    ExecuteAicaCommand(0x900ab, 0, 0);
    control.state = 2;
    g_gamePauseState = 0;
    LoadTitleSoundProgram(0);
    SetTitleMenuUIState(1);
}

} // namespace s2
} // namespace shenmue

#endif
