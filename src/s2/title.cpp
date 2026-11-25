#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/title_dc.hpp"

#include "platform/dc/sdk.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/system_modules_dc.hpp"
#include <cstring>
#else
#include "s2/title.hpp"

#include "s2/date_time.hpp"
#include "s2/debug_text.hpp"
#include "s2/game_runtime.hpp"
#include "s2/conversation.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/legacy_sprite.hpp"
#include "d3t/ui_renderer.hpp"

#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/sound_program.hpp"
#include "s2/system_modules.hpp"
#include "s2/ui.hpp"
#include "s2/archive_data.hpp"
#include "s2/initialization.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/math.hpp"
#include "s2/ui_loadsave.hpp"
#include "d3t/ui_runtime.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <bit>
#include <limits>
#include "s2/area_names.hpp"
#if defined(_WIN32)
#include "imgui.h"
#endif
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

std::int32_t InitializeTitleDebugGame(UIElement);
void DebugTitleSceneSelector();
void SwitchSystemModuleTaskForFrontend(void *);
void TitleMenuControl(void *data);
void TitleMenuDisplay(void *);
void TitleOptionsControl(void *data);
void TitleOptionsDisplay(void *);
void StartTitleCreditsMovie(void *);
void DrawTitleCreditsFrame(void *);
void StartNextTitleMenuSequence(void *data);
void ShowTitleStorageWarning(void *data);
void ShowTitleOptionsWarning(void *data);
void ShowTitleArcadeWarning(void *data);
void StartTitleHangOnMovie(void *);

} // namespace

std::uint16_t InterpretTitleAnalogInput();
void ReturnToTitleControlTasks();
std::int64_t ExecuteTitleMenuCallback(TitleControlData *control, SystemModule module);
void SetPressStart(void *);
void TitleDisplayNullsub(void *);
const char *GetShenNameString(shen_name name);
void ResetTitleMenuState();
std::int32_t SetTitleSceneInfo(const TitleSceneEntry &scene);
void TitleAudioControl(std::int32_t sceneIndex);
void ApplyTitleMenuOptions();
void TitleControl(void *taskData);
void TitleModule(void *taskData);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void TitleControl(void *taskData);

std::uint16_t g_previousTitleInputUserHandle = 0;

std::int32_t g_titleFlagShouldRun = 0;

std::uint32_t g_titleVersionOverride{};

std::int32_t g_titleMenuSelection = -1;

std::int32_t g_previousTitleMenuSelection = -1;

std::int32_t g_titleMenuUIState{};

std::int32_t g_titleMenuTransitionRequested{};

namespace {

constexpr std::uint32_t kHasCurrentLogo = 0x4;

constexpr std::uint32_t kFadeInProgress = 0x80;

constexpr std::uint32_t kLeaving = 0x40;

constexpr std::uint32_t kTimerMask = 0x1c0;

constexpr std::uint32_t kTimerPattern = 0x140;

constexpr std::uint32_t kQueueYsMark = 0x200;

constexpr std::uint32_t kQueueAm2Logo = 0x400;

HLib::Task *titleControlTask = nullptr;

TitleSequence titleLoopSequence = TitleSequence::PressStart;

std::uint32_t logoFlags = 0;

std::int32_t needsTitleSequenceDraw;

std::int32_t setPressStartPending;

std::int32_t pressStartSelected = 0;

float titleSequenceTime;

std::int32_t titleAttractSkipRequested{};

std::int32_t titleWheelTransitionAngle{};

ConversationBuffer *currentTitleLogo = nullptr;
std::array<std::int32_t, 3> titleMenuOptions{};
std::int32_t titleOptionsInputLocked{};

std::int32_t titleOptionsRow{};

std::int32_t titleOptionsPage1Row{};

std::array<std::int32_t, 2> titleOptionsPage1Values{};
std::int32_t titleOptionsPage2Mode{};

std::int32_t titleOptionsPage2Row{};

std::array<std::int32_t, 11> titleOptionsPage2Values{};

std::int32_t titleOptionsPage{};

std::array<std::int32_t, 5> titleOptionItems{};

const std::int16_t *titleOptionColumnMarker{};

std::int32_t debugModuleChangeRequested{};

SystemModule debugModuleChange{};

std::int32_t debugSceneAct{};

shen_name debugSceneArea{};

std::int32_t debugSceneEntry{};

std::array<MapEventAreaDefinition, 128> titleDebugSceneAreas{};

std::array<char, 5> shenNameString{};

std::int16_t titleSelectedAct = -1;

std::int32_t titleDebugAct = 0;

std::int32_t titleDebugSaveSlot = -1;

std::int8_t titleDebugSaveLoaded{};

std::int32_t titleCurrentAreaIndex = 0;

std::int32_t titleDebugSelection;

std::int32_t titleDebugAreaCharacter;

std::int32_t titleCurrentCoreModule;

std::int32_t titleAnalogInputLatched{};

const std::array<TitleDebugModuleEntry, 6> TITLE_DEBUG_MODULES{{
    {.name = "             ", .module = SystemModule::Game, .control = DebugTitleSceneSelector, .initialize = InitializeTitleDebugGame},
    {.name = " STAFF ROLL  ", .module = SystemModule::Credits, .initialize = GetDebugForceUIEnabled},
    {.name = "   OUTRUN    ", .module = SystemModule::OutRun, .initialize = GetDebugForceUIEnabled},
    {.name = "  HARRIER    ", .module = SystemModule::SpaceHarrier, .initialize = GetDebugForceUIEnabled},
    {.name = "   HANGON    ", .module = SystemModule::HangOn, .initialize = GetDebugForceUIEnabled},
    {.name = "AFTER BURNER ", .module = SystemModule::AfterBurner, .initialize = GetDebugForceUIEnabled},
}};

const std::array<TitleCallbackPair, 2> TITLE_SECOND_CALLBACKS{{
    {.control = SwitchSystemModuleTaskForFrontend, .display = SetPressStart},
    {.control = TitleMenuControl, .display = TitleMenuDisplay},
}};

const std::array<TitleMenuDispatchEntry, 12> titleMenuDispatch{{
    {.control = TitleMenuControl, .display = TitleMenuDisplay, .initialize = nullptr},
    {.control = TitleOptionsControl, .display = TitleOptionsDisplay, .initialize = nullptr},
    {.control = TitleContinueControl, .display = TFSEL_DispFileSelect, .initialize = InitializeTitleContinueFlags},
    {.control = StartTitleNewGame, .display = TFSEL_DispFileSelect, .initialize = InitializeTitleNewGameFlags},
    {.control = ShowTitleOptionsWarning, .display = TitleMenuDisplay, .initialize = nullptr},
    {.control = StartTitleCollection, .display = TFSEL_DispFileSelect, .initialize = InitializeTitleCollectionFlags},
    {.control = StartTitleCreditsMovie, .display = DrawTitleCreditsFrame, .initialize = nullptr},
    {.control = ShowTitleStorageWarning, .display = TitleMenuDisplay, .initialize = nullptr},
    {.control = ShowTitleArcadeWarning, .display = TitleMenuDisplay, .initialize = nullptr},
    {.control = StartNextTitleMenuSequence, .display = TitleMenuDisplay, .initialize = nullptr},
    {.control = StartTitleHangOnMovie, .display = TitleMenuDisplay, .initialize = nullptr},
    {.control = StartTitleSnapshotViewer, .display = TFSEL_DispFileSelect, .initialize = nullptr},
}};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint32_t GetTitleVersionOverride() noexcept {
    return g_titleVersionOverride;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void SetTitleVersionOverride(std::uint32_t value) noexcept {
    g_titleVersionOverride = value;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int64_t (*g_titleMenuTransitionCallback)(){};

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SetTitleMenuUIState(std::int32_t state) noexcept {
    g_titleMenuUIState = state;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SetTitleMenuTransitionCallback(std::int64_t (*callback)()) noexcept {
    g_titleMenuTransitionCallback = callback;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void RequestTitleMenuTransition() noexcept {
    g_titleMenuTransitionRequested = 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool shenIsInIntro() noexcept {
    // shenIsInIntro reads a DWORD at Version+0xdc, not a byte at +0x37.
    return GetS2AudioSettings().dialogueMode != 3 || (g_actNumber == 1 && g_currentMap == AREA_OP02);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
std::int32_t FindLastTitleSceneGroup(const TitleSceneGroup *groups) {
    std::int32_t count = 0;
    while (groups[count].scenes != reinterpret_cast<const TitleSceneEntry *>(static_cast<std::uintptr_t>(-1)))
        ++count;
    return count - 1;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void SelectTitleSceneGroup(const TitleSceneGroup *groups) {
    const std::uint16_t input = g_inputBuffer[0].repeatedPresses;
    if (input & 2U) {
        do {
            titleDebugAct = titleDebugAct != 0 ? titleDebugAct - 1 : FindLastTitleSceneGroup(groups);
        } while (!groups[titleDebugAct].scenes);
    } else if (input & 4U) {
        do {
            ++titleDebugAct;
            if (groups[titleDebugAct].scenes == reinterpret_cast<const TitleSceneEntry *>(static_cast<std::uintptr_t>(-1)))
                titleDebugAct = 0;
        } while (!groups[titleDebugAct].scenes);
    } else {
        return;
    }
    titleCurrentAreaIndex = groups[titleDebugAct].index;
    SetTitleSceneInfo(groups[titleDebugAct].scenes[titleCurrentAreaIndex]);
    titleDebugSaveLoaded = 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void SelectTitleArea(const TitleSceneEntry *scenes) {
    const std::uint16_t input = g_inputBuffer[0].repeatedPresses;
    if (titleCurrentAreaIndex == -2)
        return;
    if (input & 0x200U) {
        if (titleCurrentAreaIndex == -1 || titleCurrentAreaIndex == 0) {
            std::int32_t count = 0;
            while (scenes[count].act != 255)
                ++count;
            titleCurrentAreaIndex = count - 1;
        } else {
            --titleCurrentAreaIndex;
        }
    } else if (input & PAD_BUTTON_X) {
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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void SetTitleDebugStartTime(shen_name area) {
    DateTime time;
    shenGetCurrentDateTime(time);
    if (area == TITLE_AREA_0200) {
        time.hour = 10;
        time.minute = 0;
        time.second = 0;
    } else if (area.characters[0] == 'K') {
        time.hour = 8;
        time.minute = 0;
        if (area != TITLE_AREA_KRF1) {
            time.hour = 9;
            time.minute = 30;
        }
        time.second = 0;
    }
    SetCurrentDateTimeTo(time);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
std::int32_t InitializeTitleDebugGame(UIElement) {
    if (titleDebugSaveSlot < 0) {
        if (InitializeNewGameFromSaveSlot(-1) == 0)
            return 0;
        SetTitleDebugStartTime(g_currentMap);
    } else if (LoadGameFromSaveSlot(titleDebugSaveSlot, titleDebugSaveLoaded) == 0) {
        return 0;
    }
    ApplyTitleMenuOptions();
    return 1;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void DebugTitleSceneSelector() {
    const std::uint16_t input = g_inputBuffer[0].repeatedPresses;
    SelectTitleSceneGroup(TITLE_SCENE_GROUPS.data());
    SelectTitleArea(TITLE_SCENE_GROUPS[titleDebugAct].scenes);
    const S2SaveDataInfo *const saves = GetBackupSaveDataInfo(0);
    if (input & PAD_LEFT) {
        if (titleDebugSelection == 0) {
            do {
                titleDebugSaveSlot = titleDebugSaveSlot < 0 ? 3 : titleDebugSaveSlot - 1;
            } while (titleDebugSaveSlot >= 0 && saves[titleDebugSaveSlot].saveKind == 0);
            if (titleDebugSaveSlot >= 0) {
                titleDebugSaveLoaded = 1;
                g_actNumber = saves[titleDebugSaveSlot].sceneNumber;
                g_currentMap = saves[titleDebugSaveSlot].mapName;
                g_entryPoint = saves[titleDebugSaveSlot].entryPoint;
            } else {
                titleDebugSaveLoaded = 0;
            }
        } else {
            if (titleDebugSelection == 1) {
                if (g_actNumber-- == 0)
                    g_actNumber = 99;
            } else if (titleDebugSelection == 2) {
                if (--titleDebugAreaCharacter < -1)
                    titleDebugAreaCharacter = 3;
            } else if (titleDebugSelection == 3) {
                if (g_entryPoint-- == 0)
                    g_entryPoint = 99;
            }
            titleDebugSaveLoaded = 0;
        }
    } else if (input & PAD_RIGHT) {
        if (titleDebugSelection == 0) {
            bool found = false;
            while (titleDebugSaveSlot <= 2) {
                ++titleDebugSaveSlot;
                if (titleDebugSaveSlot < 0 || saves[titleDebugSaveSlot].saveKind != 0) {
                    found = true;
                    break;
                }
            }
            if (!found)
                titleDebugSaveSlot = -1;
            if (titleDebugSaveSlot >= 0) {
                titleDebugSaveLoaded = 1;
                g_actNumber = saves[titleDebugSaveSlot].sceneNumber;
                g_currentMap = saves[titleDebugSaveSlot].mapName;
                g_entryPoint = saves[titleDebugSaveSlot].entryPoint;
            } else {
                titleDebugSaveLoaded = 0;
            }
        } else {
            if (titleDebugSelection == 1) {
                if (++g_actNumber >= 100)
                    g_actNumber = 0;
            } else if (titleDebugSelection == 2) {
                if (++titleDebugAreaCharacter >= 4)
                    titleDebugAreaCharacter = -1;
            } else if (titleDebugSelection == 3) {
                if (++g_entryPoint >= 100)
                    g_entryPoint = 0;
            }
            titleDebugSaveLoaded = 0;
        }
    }
    shen_name area = g_currentMap.name != 0 ? g_currentMap : TITLE_AREA_0000;
    if (input & PAD_UP) {
        if (titleDebugAreaCharacter < 0) {
            if (--titleDebugSelection < 0)
                titleDebugSelection = 3;
        } else {
            char &character = area.characters[titleDebugAreaCharacter];
            if (character == '_')
                character = '0';
            else if (character == '9')
                character = 'A';
            else if (character == 'Z')
                character = '_';
            else
                ++character;
        }
    } else if (input & PAD_DOWN) {
        if (titleDebugAreaCharacter < 0) {
            if (++titleDebugSelection >= 4)
                titleDebugSelection = 0;
        } else {
            char &character = area.characters[titleDebugAreaCharacter];
            if (character == '_')
                character = 'Z';
            else if (character == '0')
                character = '_';
            else if (character == 'A')
                character = '9';
            else
                --character;
        }
    }
    g_currentMap = area;
    if ((g_rootTaskIterationCount & 0xfU) >= 8 && titleDebugAreaCharacter >= 0)
        area.characters[titleDebugAreaCharacter] = ' ';
    SetDebugStringPosition(14, 18);
    if (titleDebugSaveSlot >= 0 && titleDebugSaveSlot <= 2) {
        const float step = static_cast<float>(static_cast<std::int16_t>(saves[titleDebugSaveSlot].stepHundreds)) * 0.0001f;
        DebugTextPrintf("CONTINUE - #%d:STEP%2.4f", titleDebugSaveSlot + 1, static_cast<double>(step));
    } else {
        DebugTextPrintf(titleDebugSaveSlot == 3 ? "CONTINUE - RAM" : "CONTINUE - OFF");
    }
    DebugTextPrintf(" %c\n", titleDebugSaveLoaded ? 'R' : ' ');
    DebugTextPrintf("SCENE    - %02d\n", g_actNumber);
    DebugTextPrintf("AREA     - %c%c%c%c\n", area.characters[0], area.characters[1], area.characters[2], area.characters[3]);
    DebugTextPrintf("ENTRY    - %02d\n", g_entryPoint);
    SetDebugStringPosition(12, titleDebugSelection + 18);
    DebugTextPrintf(">");
    SetDebugStringPosition(1, 22);
    if (titleCurrentAreaIndex < 0)
        DebugTextPrintf("(%2d:--)", titleDebugAct);
    else
        DebugTextPrintf("(%2d:%2d)", titleDebugAct, titleCurrentAreaIndex);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void SwitchSystemModuleTaskForFrontend(void *) {
    const std::uint16_t input = g_inputBuffer[0].pressedPriority;
    if (input & 0xc0U) {
        titleCurrentCoreModule += (input & PAD_TRIGGER_LEFT) != 0 ? -1 : 1;
        const std::int32_t last = g_loadedActNumber == 2 ? 3 : 5;
        if (titleCurrentCoreModule < 0)
            titleCurrentCoreModule = last;
        else if (titleCurrentCoreModule > last)
            titleCurrentCoreModule = 0;
    }
    const TitleDebugModuleEntry &selected = TITLE_DEBUG_MODULES[titleCurrentCoreModule];
    if (selected.control)
        selected.control();
    SetDebugStringPosition(14, 21);
    DebugTextPrintf("%s", TITLE_DEBUG_MODULES[titleCurrentCoreModule].name);
    if ((input & 0xc00U) != 0 && TITLE_DEBUG_MODULES[titleCurrentCoreModule].initialize(UIElement{}) != 0) {
        g_bPaused = false;
        const SystemModule module = TITLE_DEBUG_MODULES[titleCurrentCoreModule].module;
        ExecuteAicaCommand(0x1200a0U, 5, 0);
        ChangeSystemModule(module, 0);
        if (g_loadedActNumber != g_actNumber) {
            SetNextSceneNumber(static_cast<std::uint32_t>(g_actNumber));
            g_bPaused = false;
            ChangeSystemModule(SystemModule ::DiskChange, 0);
        }
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
const MapEventAreaDefinition *BuildTitleDebugSceneAreas(std::int32_t scene, std::int32_t filter) {
    const MapEventAreaDefinition *areas = nullptr;
    if (scene != 0) {
        if (static_cast<std::uint32_t>(scene) <= 2)
            areas = GetHongKongMapEventAreas();
        else if (scene == 3)
            areas = GetKowloonMapEventAreas();
        else if (scene == 4)
            areas = GetGuilinMapEventAreas();
    }
    if (!filter)
        return areas;
    std::size_t count = 0;
    for (; areas && areas->areaId.name != 0xffffffffU; ++areas) {
        bool keep = true;
        for (const shen_name excluded : TITLE_DEBUG_EXCLUDED_AREAS[static_cast<std::size_t>(scene - 1)]) {
            if (excluded.name == 0 || !keep)
                break;
            if (excluded.name == areas->areaId.name)
                keep = false;
        }
        if (!keep)
            continue;
        const shen_name area = areas->areaId;
        char directory[64];
        std::snprintf(
            directory, sizeof(directory), "scene/%02d/%c%c%c%c", scene, area.characters[0], area.characters[1], area.characters[2], area.characters[3]);
        char path[260];
        std::snprintf(path, sizeof(path), "%s/%s", directory, "MAPINFO.BIN");
        if (ArchiveData_GetFileSize(path) != 0)
            titleDebugSceneAreas[count++] = *areas;
    }
    titleDebugSceneAreas[count].areaId.name = 0xffffffffU;
    return titleDebugSceneAreas.data();
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

#if defined(_WIN32)
// @verified - p
void TitleDebugMenuHeader() {
    ImGui::SameLine(0.0f, -1.0f);
    ImGui::Text("Disk %u: %s", static_cast<std::uint32_t>(g_actNumber), GetShenNameString(g_currentMap));
}
#endif

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

#if defined(_WIN32)
// @verified - p
void TitleDebugMenuBody() {
    char label[512];
    for (std::uint32_t scene = 1; scene <= 4; ++scene) {
        const MapEventAreaDefinition *area = BuildTitleDebugSceneAreas(static_cast<std::int32_t>(scene), 1);
        if (!area)
            continue;
        std::snprintf(label, sizeof(label), "Disk %u", scene);
        if (!ImGui::BeginMenu(label, true))
            continue;
        for (; area->areaId.name != 0xffffffffU; ++area) {
            const std::int32_t index = ui_detail::SaveMenuAreaIndex(area->areaId);
            const char *const name = index == -1 ? "" : GetStringForUILabel(UI_SAVE_AREA_TOKENS[static_cast<std::size_t>(index)]);
            std::snprintf(label, sizeof(label), "%s - %s", GetShenNameString(area->areaId), name);
            if (!ImGui::BeginMenu(label, true))
                continue;
            for (const MapEventTransition *entry = area->transitions; entry && entry->eventId != -1; ++entry) {
                std::snprintf(label, sizeof(label), "Entry %d: %s", entry->eventId, GetShenNameString(entry->areaId));
                if (ImGui::MenuItem(label, nullptr, false, true)) {
                    SetProjectionOff();
                    debugModuleChangeRequested = 1;
                    debugModuleChange = SystemModule::Game;
                    debugSceneAct = static_cast<std::int32_t>(scene);
                    debugSceneArea = area->areaId;
                    debugSceneEntry = entry->eventId;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Change System Module", true)) {
        for (std::size_t module = 0; module < TITLE_DEBUG_SYSTEM_MODULE_NAMES.size(); ++module) {
            if (ImGui::MenuItem(TITLE_DEBUG_SYSTEM_MODULE_NAMES[module], nullptr, false, true)) {
                SetProjectionOff();
                debugModuleChangeRequested = 1;
                debugModuleChange = static_cast<SystemModule>(module);
                debugSceneAct = 0;
                debugSceneArea = {};
                debugSceneEntry = 0;
            }
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Reset To Title", nullptr, false, true))
        ResetToTitleScreen();
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

OmakeSelection omakeSelection = {};

HLib::Task *titleControlTask = 0;

std::uint32_t logoFlags = 0;

std::int32_t titleLoopSequence, needsTitleSequenceDraw = 0, setPressStartPending = 0, titleSequenceTime = 0;

std::int32_t titleDebugSelection, titleDebugAreaCharacter = 0, titleCurrentCoreModule = 0;

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void NativeTitleNoOp(void *) {
}

#else

namespace {

void NativeTitleNoOp(void *) {
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void TitleMenuControl(void *data) {
    if (titleAttractSkipRequested != 0) {
        ReturnToTitleControlTasks();
        return;
    }
    std::uint16_t directions = g_inputBuffer[0].repeatedPresses;
    std::uint16_t buttons = g_inputBuffer[0].pressedPriority;
    padcCheckControllerConnection();
    if (titleWheelTransitionAngle != 0) {
        buttons = 0;
        directions = 0;
    }
    g_previousTitleMenuSelection = g_titleMenuSelection;
    directions |= InterpretTitleAnalogInput();
    static_cast<void>(directions);
    static_cast<void>(GetDebugForceUIEnabled(UIElement::MainMenu));
    if (ExecuteElementCallback(UIElement::MainMenu) == 0 && (!g_titleMenuTransitionCallback || g_titleMenuTransitionCallback() == 0))
        return;
    if (g_debugImGuiRenderMode != 1) {
        g_previousTitleInputUserHandle = FrontendControllerUserHandle();
        return;
    }
    g_titleMenuSelection = g_titleMenuUIState;
    if ((buttons & 0x0c00U) != 0 || g_titleMenuTransitionRequested != 0) {
        g_titleMenuTransitionRequested = 0;
        const std::uint64_t flags = GetLocalBackupInfo();
        SystemModule module = SystemModule::Option;
        switch (g_titleMenuSelection) {
        case 0:
            module = SystemModule::Hot;
            break;
        case 1:
            module = SystemModule::Logo;
            if ((GetLocalBackupInfo() & 0x40U) == 0)
                return;
            break;
        case 2:
            if ((flags & 0x7fe00U) == 0) {
                vfSubmitQuad(0x7f000000U, -2.5900004f);
                SetTitleWarningLogo(46, nullptr);
                module = SystemModule::AfterBurner;
            }
            break;
        case 3:
            module = SystemModule::Game;
            break;
        case 4:
            module = SystemModule::Cold;
            break;
        case 5:
            module = SystemModule::SpaceHarrier;
            break;
        default:
            return;
        }
        ExecuteTitleMenuCallback(static_cast<TitleControlData *>(data), module);
    } else if ((buttons & 0x0201U) != 0) {
        ReturnToTitleControlTasks();
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void DrawTitleMenuWheel() {
    std::int32_t angle = titleWheelTransitionAngle;
    if (angle > 0) {
        angle -= static_cast<std::int32_t>(shenGetScaledDeltaTickTime(1365.0f));
        titleWheelTransitionAngle = angle;
        if (angle < 255) {
            angle = 0;
            titleWheelTransitionAngle = 0;
        }
    } else if (angle < 0) {
        angle += static_cast<std::int32_t>(shenGetScaledDeltaTickTime(1365.0f));
        titleWheelTransitionAngle = angle;
        if (angle > -255) {
            angle = 0;
            titleWheelTransitionAngle = 0;
        }
    }
    if (angle == 0) {
        switch (g_titleMenuSelection - g_previousTitleMenuSelection) {
        case -1:
        case 5:
            angle = 9557;
            titleWheelTransitionAngle = angle;
            break;
        case 1:
        case -5:
            angle = -9557;
            titleWheelTransitionAngle = angle;
            break;
        default:
            break;
        }
    }
    for (std::int32_t item = 0; item < 6; ++item) {
        const std::int32_t itemAngle = static_cast<std::int32_t>(static_cast<float>(10922 * (g_titleMenuSelection - item))) + angle;
        const float cosine = FixedAngleCos(static_cast<std::uint16_t>(itemAngle));
        const float y = cosine * 70.0f + 200.0f;
        const float sine = FixedAngleSin(static_cast<std::uint16_t>(itemAngle));
        const float depth = cosine + 2.5f;
        const float scale = (depth * 0.2f + 0.2f) * 1.1111112f;
        Vector3 position{.x = scale * 10.0f + (sine * (depth * 70.0f) + 320.0f), .y = scale * 15.0f + y, .z = scale * 0.001f - 2.8000002f};
        DrawTitleSpriteRect(item != g_titleMenuSelection ? TITLE_WHEEL_ITEM_UV.data() : TITLE_WHEEL_SELECTED_UV.data(), position, 7, scale);
        position.z = scale * 0.0001f + position.z;
        DrawTitleSpriteRect(TITLE_WHEEL_LABEL_UV[static_cast<std::size_t>(item)].data(), position, 7, scale);
        angle = titleWheelTransitionAngle;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void TitleMenuDisplay(void *) {
    if (GetDebugForceUIEnabled(UIElement::MainMenu) && g_debugImGuiRenderMode != 1) {
        EnableElement(UIElement::MainMenu);
        return;
    }
    DrawTitleSpriteRect(TITLE_MENU_HEADER_UV[0].data(), {.x = 160.0f, .y = 59.0f, .z = -2.8000002f}, 7, 1.0f);
    DrawTitleSpriteRect(TITLE_MENU_HEADER_UV[1].data(), {.x = 480.0f, .y = 59.0f, .z = -2.8000002f}, 7, 1.0f);
    DrawTitleMenuWheel();
    std::int32_t item = 3, secondary = -1;
    if (g_titleMenuSelection != 1 || (GetLocalBackupInfo() & 0x40U) != 0) {
        switch (g_titleMenuSelection) {
        case 2:
            item = 14;
            secondary = 13;
            break;
        case 3:
            item = 15;
            break;
        case 5:
            item = 16;
            break;
        default:
            item = g_titleMenuSelection == -1 ? g_previousTitleMenuSelection : TITLE_MENU_CAPTIONS[static_cast<std::size_t>(g_titleMenuSelection)];
            break;
        }
    }
    DrawTitleCaption(item, secondary);
    DrawTitleFrame(3);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void TitleOptionsControl(void *data) {
    const std::uint16_t analog = InterpretTitleAnalogInput();
    std::uint16_t buttons = g_inputBuffer[0].pressedPriority | analog;
    padcCheckControllerConnection();
    std::int32_t page = titleOptionsPage;
    if ((buttons & 0x200U) != 0) {
        if (titleOptionsInputLocked == 0) {
            ApplyTitleMenuOptions();
            ExecuteTitleMenuCallback(static_cast<TitleControlData *>(data), SystemModule::Test);
            return;
        }
    } else if (titleOptionsInputLocked == 0) {
        if ((buttons & 0x80U) != 0) {
            page = page + 1 > 2 ? 0 : page + 1;
            buttons &= 0xff3fU;
            titleOptionsPage = page;
        }
        if ((buttons & 0x40U) != 0) {
            if (--page < 0)
                page = 2;
            buttons &= 0xff3fU;
            titleOptionsPage = page;
        }
    }
    if (page == 1) {
        std::int32_t row = titleOptionsPage1Row;
        if ((buttons & 0x2000U) != 0) {
            row = row + 1 > 1 ? 0 : row + 1;
            buttons &= 0x3fffU;
            titleOptionsPage1Row = row;
        }
        if ((buttons & 0x1000U) != 0) {
            if (--row < 0)
                row = 1;
            buttons &= 0x3fffU;
            titleOptionsPage1Row = row;
        }
        std::int32_t value = titleOptionsPage1Values[static_cast<std::size_t>(row)];
        if ((buttons & 0x4000U) != 0 && --value < 0)
            value = 0;
        if ((buttons & 0x8000U) != 0) {
            const std::int32_t count = TITLE_OPTION_PAGE1_COUNTS[static_cast<std::size_t>(row)];
            if (++value >= count)
                value = count - 1;
        }
        titleOptionsPage1Values[static_cast<std::size_t>(row)] = value;
    } else if (page == 0) {
        std::int32_t row = titleOptionsRow;
        if ((buttons & 0x2000U) != 0) {
            row = row + 1 > 2 ? 0 : row + 1;
            buttons &= 0x3fffU;
            titleOptionsRow = row;
        }
        if ((buttons & 0x1000U) != 0) {
            if (--row < 0)
                row = 2;
            buttons &= 0x3fffU;
            titleOptionsRow = row;
        }
        if (row != 3) {
            std::int32_t value = titleMenuOptions[static_cast<std::size_t>(row)];
            if ((buttons & 0x4000U) != 0) {
                if (--value < 0)
                    value = row != 0 ? 0 : TITLE_OPTION_PAGE0_COUNTS[0] - 1;
                g_titleVersionOverride = 1;
            }
            if ((buttons & 0x8000U) != 0) {
                const std::int32_t count = TITLE_OPTION_PAGE0_COUNTS[static_cast<std::size_t>(row)];
                if (++value >= count)
                    value = row != 0 ? count - 1 : 0;
                g_titleVersionOverride = 1;
            }
            titleMenuOptions[static_cast<std::size_t>(row)] = value;
        }
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

const std::uint16_t *TitleOptionSpriteUv(std::int32_t texture, std::int32_t index) {
    const std::size_t entry = static_cast<std::size_t>(index);
    switch (texture) {
    case 8:
        return TITLE_OPTION_FRAME_UV[entry].data();
    case 9:
        return TITLE_OPTION_TEXT_UV[entry].data();
    case 21:
        return TITLE_OPTION_VALUE_UV[entry].data();
    case 22:
        return TITLE_OPTION_LABEL_UV[entry].data();
    case 23:
        return TITLE_OPTION_ICON_UV[entry].data();
    default:
        return nullptr;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void DrawTitleOptionColumns(std::int32_t texture, const std::int16_t *labels, std::int32_t value, float y) {
    Vector3 position{.y = y};
    titleOptionColumnMarker = TITLE_OPTION_COLUMN_MARKERS[static_cast<std::size_t>(value & 1)].data();
    for (std::int32_t column = 0; column < 2; ++column) {
        position.z = -2.8000002f;
        position.x = static_cast<float>(TITLE_OPTION_COLUMN_X[static_cast<std::size_t>(column)]);
        DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[static_cast<std::size_t>(titleOptionColumnMarker[1])].data(), position, 8, 1.0f);
        position.x = static_cast<float>(column) * 210.0f + 310.0f;
        DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[static_cast<std::size_t>(titleOptionColumnMarker[0])].data(), position, 8, 1.0f);
        position.z = -2.6000004f;
        DrawTitleSpriteRect(TitleOptionSpriteUv(texture, labels[column]), position, texture, 1.0f);
        titleOptionColumnMarker += 2;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void DrawTitleOptionStepper(Vector3 position, std::int32_t first, std::int32_t row) {
    const std::size_t index = static_cast<std::size_t>(row);
    DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[6].data(), position, 8, 1.0f);
    position.z = -2.6000004f;
    DrawTitleSpriteRect(TITLE_OPTION_VALUE_UV[static_cast<std::size_t>(first + titleOptionsPage1Values[index])].data(), position, 21, 1.0f);
    position.z = -2.8000002f;
    position.x = 300.0f;
    DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[titleOptionsPage1Values[index] == 0 ? 9 : 7].data(), position, 8, 1.0f);
    position.x = 525.0f;
    DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[titleOptionsPage1Values[index] == TITLE_OPTION_PAGE1_COUNTS[index] - 1 ? 10 : 8].data(), position, 8, 1.0f);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Native page 2 also has a selector-1 branch; its selector is never written and that branch reads the column positions as a UV pointer.
// @verified - p
void TitleOptionsDisplay(void *) {
    if (GetDebugForceUIEnabled(UIElement::Options))
        EnableElement(UIElement::Options);
    Vector3 position{.x = 160.0f, .y = 59.0f, .z = -2.8000002f};
    DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[0].data(), position, 8, 1.0f);
    position.x = 480.0f;
    DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[1].data(), position, 8, 1.0f);
    position = {.x = 20.0f, .y = 110.0f, .z = -2.6000004f};
    DrawTitleSpriteRect(TITLE_OPTION_LABEL_UV[21].data(), position, 22, 1.0f);
    position.x = 620.0f;
    DrawTitleSpriteRect(TITLE_OPTION_LABEL_UV[22].data(), position, 22, 1.0f);
    for (std::int32_t tab = 0; tab < 3; ++tab) {
        position = {.x = static_cast<float>(tab * 200) + 120.0f, .y = 110.0f, .z = -2.8000002f};
        DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[tab == titleOptionsPage ? 2 : 3].data(), position, 8, 1.0f);
        position.z = -2.6000004f;
        DrawTitleSpriteRect(TITLE_OPTION_LABEL_UV[static_cast<std::size_t>(TITLE_OPTION_TAB_LABELS[static_cast<std::size_t>(tab)])].data(), position, 22, 1.0f);
    }
    std::int32_t item = -1, secondary = -1;
    if (titleOptionsPage == 0) {
        for (std::int32_t row = 0; row < 3; ++row) {
            position = {.x = 130.0f, .y = static_cast<float>(row) * 45.0f + 160.0f, .z = -2.8000002f};
            DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[row == titleOptionsRow ? 2 : 3].data(), position, 8, 1.0f);
            position.z = -2.6000004f;
            DrawTitleSpriteRect(
                TITLE_OPTION_TEXT_UV[static_cast<std::size_t>(TITLE_OPTION_PAGE0_LABELS[static_cast<std::size_t>(row)])].data(), position, 9, 1.0f);
        }
        position = {.x = 415.0f, .y = 160.0f, .z = -2.8000002f};
        DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[6].data(), position, 8, 1.0f);
        position.z = -2.6000004f;
        DrawTitleSpriteRect(TITLE_OPTION_TEXT_UV[static_cast<std::size_t>(TITLE_OPTION_DIALOGUE_LABELS[static_cast<std::size_t>(titleMenuOptions[0])])].data(),
            position, 9, 1.0f);
        position.x = 300.0f;
        position.z = -2.8000002f;
        DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[7].data(), position, 8, 1.0f);
        position.x = 525.0f;
        DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[8].data(), position, 8, 1.0f);
        for (std::int32_t row = 1; row <= 2; ++row)
            DrawTitleOptionColumns(9, TITLE_OPTION_PAGE0_VALUE_LABELS[static_cast<std::size_t>(row - 1)].data(),
                titleMenuOptions[static_cast<std::size_t>(row)], static_cast<float>(row) * 45.0f + 160.0f);
        const std::array<std::int8_t, 2> &caption = titleOptionsRow != 0 ? TITLE_OPTION_ROW_CAPTIONS[static_cast<std::size_t>(titleOptionsRow)] :
                                                                           TITLE_OPTION_DIALOGUE_CAPTIONS[static_cast<std::size_t>(titleMenuOptions[0])];
        item = caption[0];
        secondary = caption[1];
    } else if (titleOptionsPage == 1) {
        for (std::int32_t row = 0; row <= 1; ++row) {
            position = {.x = 130.0f, .y = static_cast<float>(row) * 45.0f + 160.0f, .z = -2.8000002f};
            DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[row == titleOptionsPage1Row ? 2 : 3].data(), position, 8, 1.0f);
            position.z = -2.6000004f;
            DrawTitleSpriteRect(
                TITLE_OPTION_VALUE_UV[static_cast<std::size_t>(TITLE_OPTION_PAGE1_LABELS[static_cast<std::size_t>(row)])].data(), position, 21, 1.0f);
        }
        position = {.x = 415.0f, .y = 160.0f, .z = -2.8000002f};
        DrawTitleOptionStepper(position, 7, 0);
        position.y = 205.0f;
        DrawTitleOptionStepper(position, 2, 1);
    } else if (titleOptionsPage == 2) {
        for (std::int32_t row = 0; row < 5; ++row) {
            const std::int32_t option = titleOptionItems[static_cast<std::size_t>(row)];
            if (option == 10)
                continue;
            position = {.x = 130.0f, .y = static_cast<float>(row) * 45.0f + 160.0f, .z = -2.8000002f};
            DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[option == titleOptionsPage2Row ? 2 : 3].data(), position, 8, 1.0f);
            position.z = -2.6000004f;
            DrawTitleSpriteRect(
                TITLE_OPTION_LABEL_UV[static_cast<std::size_t>(TITLE_OPTION_ITEM_LABELS[static_cast<std::size_t>(option)])].data(), position, 22, 1.0f);
        }
        for (std::int32_t row = 0; row < 5; ++row) {
            const std::int32_t option = titleOptionItems[static_cast<std::size_t>(row)];
            if (option == 0) {
                position.x = 415.0f;
                position.z = -2.8000002f;
                position.y = static_cast<float>(row) * 45.0f + 160.0f;
                DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[6].data(), position, 8, 1.0f);
                position.z = -2.6000004f;
                DrawTitleSpriteRect(
                    TITLE_OPTION_LABEL_UV[static_cast<std::size_t>(TITLE_OPTION_PAGE2_MODE_LABELS[static_cast<std::size_t>(titleOptionsPage2Mode)])].data(),
                    position, 22, 1.0f);
                position.z = -2.8000002f;
                position.x = 300.0f;
                DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[7].data(), position, 8, 1.0f);
                position.x = 525.0f;
                DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[8].data(), position, 8, 1.0f);
            }
            if (option == 10) {
                position = {.x = 320.0f, .y = static_cast<float>(row) * 45.0f + 160.0f, .z = -2.8000002f};
                DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[titleOptionsPage2Row == 10 ? 2 : 3].data(), position, 8, 1.0f);
                position.z = -2.6000004f;
                DrawTitleSpriteRect(TITLE_OPTION_LABEL_UV[static_cast<std::size_t>(TITLE_OPTION_ITEM_LABELS[10])].data(), position, 22, 1.0f);
            }
        }
        for (std::int32_t row = 0; row < 5; ++row) {
            const std::int32_t option = titleOptionItems[static_cast<std::size_t>(row)];
            if (option == 0 || option == 10 || titleOptionsPage2Mode == 1)
                continue;
            position.x = 415.0f;
            position.y = static_cast<float>(row) * 45.0f + 160.0f;
            const std::int32_t value = titleOptionsPage2Values[static_cast<std::size_t>(option)];
            position.z = -2.8000002f;
            DrawTitleSpriteRect(TITLE_OPTION_FRAME_UV[titleOptionsPage2Row == option ? 4 : 5].data(), position, 8, 1.0f);
            if (value != -1) {
                position.z = -2.6000004f;
                DrawTitleSpriteRect(TITLE_OPTION_ICON_UV[static_cast<std::size_t>(value)].data(), position, 23, 1.0f);
            }
        }
        if (titleOptionItems[0] != 0) {
            position.x = 520.0f;
            position.y = 160.0f;
            DrawTitleSpriteRect(TITLE_OPTION_LABEL_UV[19].data(), position, 22, 1.0f);
        }
        if (titleOptionItems[4] != 10) {
            position.y = 340.0f;
            position.x = 520.0f;
            DrawTitleSpriteRect(TITLE_OPTION_LABEL_UV[20].data(), position, 22, 1.0f);
        }
    }
    DrawTitleCaption(item, secondary);
    DrawTitleFrame(7);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void StartTitleCreditsMovie(void *) {
    ChangeSystemModule(SystemModule ::Movie, 0);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void DrawTitleCreditsFrame(void *) {
    DrawTitleFrame(1);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void StartNextTitleMenuSequence(void *data) {
    if ((g_inputBuffer[0].pressedPriority & 0x0e00U) == 0)
        return;
    SetTitleWarningLogo(0, nullptr);
    HLib::SetCurrentTaskTo(TitleMenuControl);
    TitleControlData *const control = static_cast<TitleControlData *>(data);
    if (control->titleSequenceTask)
        HLib::AssignCallbackForTask(control->titleSequenceTask, TitleMenuDisplay);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ShowTitleStorageWarning(void *data) {
    SetTitleWarningLogo(42, static_cast<TitleControlData *>(data)->warningParameters);
    vfSubmitQuad(0x7f000000U, -2.5900004f);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ShowTitleOptionsWarning(void *data) {
    SetTitleWarningLogo(45, static_cast<TitleControlData *>(data)->warningParameters);
    vfSubmitQuad(0x7f000000U, -2.5900004f);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ShowTitleArcadeWarning(void *data) {
    SetTitleWarningLogo(44, static_cast<TitleControlData *>(data)->warningParameters);
    vfSubmitQuad(0x7f000000U, -2.5900004f);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void StartTitleHangOnMovie(void *) {
    ChangeSystemModule(SystemModule::Movie, 0);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool AssignTitleCallbacks(HLib::TaskCallback controlCallback, HLib::TaskCallback displayCallback) {
    auto *const control = static_cast<TitleControlData *>(HLib::GetTaskParameter(titleControlTask));
    if (!control || !controlCallback || !control->titleSequenceTask || !displayCallback) {
        return false;
    }
    HLib::AssignCallbackForTask(control->titleSequenceTask, displayCallback);
    HLib::AssignCallbackForTask(titleControlTask, controlCallback);
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint32_t TitleAlphaColour(float alpha) {
    const float scaled = alpha * 255.0f;
    const std::int64_t truncated =
        scaled >= -9223372036854775808.0f && scaled < 9223372036854775808.0f ? static_cast<std::int64_t>(scaled) : std::numeric_limits<std::int64_t>::min();
    return (static_cast<std::uint32_t>(truncated) << 24) | 0x00ffffffU;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
// @verified - p
std::uint16_t InterpretTitleAnalogInput() {
    std::uint16_t direction = 0;
    if (g_inputBuffer[0].analog0e < 64)
        direction = PAD_LEFT;
    else if (g_inputBuffer[0].analog0e > 192)
        direction = PAD_RIGHT;
    else if (g_inputBuffer[0].analog0f < 64)
        direction = PAD_UP;
    else if (g_inputBuffer[0].analog0f > 192)
        direction = PAD_DOWN;
    else {
        titleAnalogInputLatched = 0;
        return 0;
    }
    const std::uint16_t result = titleAnalogInputLatched == 0 ? direction : 0;
    titleAnalogInputLatched = 1;
    return result;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SetupTitleMenuTask(std::int32_t mode) {
    TitleControlData *const control = static_cast<TitleControlData *>(HLib::GetTaskParameter(titleControlTask));
    if (mode != 0) {
        HLib::AssignCallbackForTask(titleControlTask, TitleMenuControl);

        HLib::AssignCallbackForTask(control->titleSequenceTask, TitleMenuDisplay);
    } else {
        const std::size_t sequence = static_cast<std::size_t>(titleLoopSequence);
        control->display->frameCount = 0;
        titleSequenceTime = static_cast<float>(TITLE_SEQUENCE_TABLE[sequence].duration);
        HLib::AssignCallbackForTask(titleControlTask, TitleControl);
        HLib::AssignCallbackForTask(control->titleSequenceTask, TITLE_SEQUENCE_TABLE[sequence].display);
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void ReturnToTitleControlTasks() {
    titleAttractSkipRequested = 0;
    ResetSelectedInputDevice();
    g_bPaused = true;
    SetupTitleMenuTask(0);
    TitleAudioControl(-1);
    logoFlags |= kLeaving;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void RequestTitleAttractSkip() {
    titleAttractSkipRequested = 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int64_t ExecuteTitleMenuCallback(TitleControlData *control, SystemModule module) {
    const TitleMenuDispatchEntry &entry = titleMenuDispatch[static_cast<std::size_t>(module)];
    if (entry.initialize)
        entry.initialize();
    HLib ::SetCurrentTaskTo(entry.control);
    if (module == SystemModule ::Game || module == SystemModule ::HangOn) {
        if (module == SystemModule::HangOn)
            HLib::SetupTaskDelay(10);
        ExecuteAicaCommand(0xaa0U, 5, 30);
    }
    if (control->titleSequenceTask)
        HLib ::AssignCallbackForTask(control->titleSequenceTask, entry.display);
    if (module == SystemModule::Cold)
        ResetTitleMenuState();
    return 1;

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SubmitTitleScreenQuad(std::int32_t texture, float alpha) {
    d3t::SetSceneClearColour(0xffffffffU);
    LegacySpritePacket packet{.quad = {.flags = 0x400}, .colour = TitleAlphaColour(alpha), .uv = {0.0f, 0.0f, 1.0f, 1.0f}};
    const std::array<std::int16_t, 2> &dimensions = TITLE_LOGO_DIMENSIONS[texture - 2];
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 320, 240, dimensions[0], dimensions[1], ScreenMode::Centered);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(titleSpriteTextures[texture]), -2.9f);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SetPressStart(void *) {
    if (!SplashSequenceComplete())
        return;
    if (setPressStartPending != 0) {
        titleLoopSequence = TitleSequence::MoveToAm2Logos5;
        setPressStartPending = 0;
    } else if (GetDebugForceUIEnabled(UIElement::PressStart)) {
        EnableElement(UIElement::PressStart);
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleDisc(void *data) {
    SubmitTitleScreenQuad(4, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleYsMark(void *data) {
    SubmitTitleScreenQuad(5, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleSegaLogo(void *data) {
    SubmitTitleScreenQuad(3, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleAm2Logo(void *data) {
    SubmitTitleScreenQuad(2, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleAdditionalLogo(void *data) {
    SubmitTitleScreenQuad(6, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleWarning41(void *) {
    SetTitleWarningLogo(41, nullptr);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleWarning40(void *) {
    SetTitleWarningLogo(40, nullptr);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleWarning1(void *) {
    SetTitleWarningLogo(1, nullptr);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void TitleDisplayNullsub(void *) {
}

#else

// @verified - p
void TitleDisplayNullsub(void *) {
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void DrawTitleEsrb(void *data) {
    logoFlags &= ~kFadeInProgress;
    const std::uint32_t frames = static_cast<TitleDisplayData *>(data)->frameCount;
    float alpha = 1.0f;
    if (frames < 30)
        alpha = static_cast<float>(std::bit_cast<std::int32_t>(frames)) * 0.033333335f;
    else if (frames > 90) {
        const float remaining = 120.0f - static_cast<float>(std::bit_cast<std::int32_t>(frames));
        alpha = (remaining > 0.0f ? remaining : 0.0f) * 0.033333335f;
    }
    d3t::SetSceneClearColour(0xff000000U);
    LegacySpritePacket packet{.quad = {.flags = 0x400}, .colour = TitleAlphaColour(alpha), .uv = {0.0f, 0.0f, 1.0f, 1.0f}};
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 320, 240, 256, 128, ScreenMode::Centered);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(titleEsrbTexture), -2.9f);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SetTitleLogoPresent() {
    logoFlags |= kHasCurrentLogo;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void HandleSceneTransitionForModule() {
    if (!debugModuleChangeRequested)
        return;
    const SystemModule module = debugModuleChange;
    debugModuleChangeRequested = 0;
    if (module == SystemModule::Game) {
        g_actNumber = debugSceneAct;
        g_currentMap = debugSceneArea;
        g_entryPoint = debugSceneEntry;
        (void)ExecuteAicaCommand(0x1200a0U, 5, 0);
        (void)ChangeSystemModule(SystemModule::Game, 0);
        if (g_loadedActNumber != g_actNumber) {
            SetSceneInfos(g_actNumber, g_currentMap, g_entryPoint);
            (void)S2Save_CheckData();
            SetNextSceneNumber(static_cast<std::uint32_t>(g_actNumber));
            g_bPaused = false;
            (void)ChangeSystemModule(SystemModule::DiskChange, 0);
        }
    } else {
        (void)ExecuteAicaCommand(0x1200a0U, 5, 0);
        (void)ChangeSystemModule(module, 0);
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
const char *GetShenNameString(shen_name name) {
    std::memcpy(shenNameString.data(), &name.name, 4);
    shenNameString[4] = 0;
    return shenNameString.data();
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void ResetTitleMenuState() {
    titleMenuOptions[0] = static_cast<std::uint8_t>(GetS2DialogueMode());
    titleMenuOptions[2] = AreS2TriggersSwapped();
    titleMenuOptions[1] = GetS2ControlMode();
    titleOptionsPage1Values[0] = 0;
    SetTitleLogoPresent();
    for (std::size_t index = 0; index < TITLE_OPTION_ITEMS.size(); ++index)
        titleOptionItems[index] = TITLE_OPTION_ITEMS[index];
    titleOptionItems[4] = 4;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SetTitleMenuSelection(std::int32_t selection) {
    g_titleMenuSelection = selection;
    g_previousTitleMenuSelection = selection;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int32_t SetTitleSceneInfo(const TitleSceneEntry &scene) {
    if (scene.act == 255) {
        g_currentMap = TITLE_AREA_0000;
        g_actNumber = 0;
        g_entryPoint = 0;
        titleCurrentAreaIndex = -2;
        return 0;
    }
    g_actNumber = scene.act;
    std::memcpy(&g_currentMap, &scene.area, sizeof(g_currentMap));
    g_entryPoint = scene.entry;
    return scene.entry;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void TitleAudioControl(std::int32_t sceneIndex) {
    LoadSoundProgram("0002S000.sbt", static_cast<std::uint32_t>(sceneIndex >> 31), 0xffffffffU);
    if (currentTitleLogo) {
        scenCvsCtrlFree(currentTitleLogo);
        currentTitleLogo = nullptr;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void ApplyTitleMenuOptions() {
    ApplyS2TriggerSwapMode(titleMenuOptions[2]);
    ApplyS2DialogueMode(static_cast<std::uint8_t>(GetS2AudioSettings().dialogueMode));
    ApplyS2ControlMode(titleMenuOptions[1]);
} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void CheckTitleDebugInput(void *data) {
    TitleControlData *const control = static_cast<TitleControlData *>(data);
    if (control->display->currentStateFrame >= 1.0f && (g_inputBuffer[0].pressedPriority & 0x0fffU)) {
        ResetTitleMenuState();
        control->state = 1;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void AcceptTitlePressStart(void *data) {
    if (pressStartSelected != 1)
        return;
    pressStartSelected = 0;
    SelectInputDeviceForButtons(0x0c00);
    ResetTitleMenuState();
    static_cast<TitleControlData *>(data)->state = 2;
    g_bPaused = false;
    LoadSoundProgram("0002S000.sbt", 0, 0xffffffffU);
    if (currentTitleLogo) {
        scenCvsCtrlFree(currentTitleLogo);
        currentTitleLogo = nullptr;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void LoadTitleIntroArea(void *) {
    g_actNumber = 1;
    g_currentMap = AREA_0001;
    g_entryPoint = 1;
    ExecuteAicaCommand(0x1200a0, 5, 0);
    ChangeSystemModule(SystemModule::Game, 0);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void LoadTitleAttractArea(void *) {
    g_actNumber = 1;
    g_currentMap = AREA_OP02;
    g_entryPoint = 1;
    d3t::SetSceneClearColour(0xff000000U);
    ExecuteAicaCommand(0x1200a0, 5, 0);
    ChangeSystemModule(SystemModule::Game, 0);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SkipTitleWarning(void *data) {
    if ((g_inputBuffer[0].pressedPriority & 0x0e01U) == 0)
        return;
    TitleControlData *const control = static_cast<TitleControlData *>(data);
    logoFlags &= ~kFadeInProgress;
    titleSequenceTime = 0.0f;
    control->display->currentStateFrame = 1.0f;
    control->display->stateFrame = -0.062745102f;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void MoveToAm2LogoOrYsMark(void *taskData) {
    if (!HasUnlockedRenderHandle())
        return;
    TitleControlData *const control = static_cast<TitleControlData *>(taskData);
    if ((g_inputBuffer[0].pressedPriority & 0x0edeU) || (g_previousTitleInputUserHandle == 0 && g_inputBuffer[0].connectionState != 0)) {
        SetupAllShaders();
        TitleDisplayData *const display = static_cast<TitleDisplayData *>(HLib::GetTaskParameter(control->titleSequenceTask));
        logoFlags |= kFadeInProgress;
        titleLoopSequence = TitleSequence::MoveToAm2Logos5;
        titleSequenceTime = static_cast<float>(TITLE_SEQUENCE_TABLE[5].duration);
        d3t::SetSceneClearColour(0xff000000U);
        display->currentStateFrame = 0.0f;
        display->stateFrame = 0.062745102f;
        if (currentTitleLogo) {
            scenCvsCtrlFree(currentTitleLogo);
            currentTitleLogo = nullptr;
        }
        logoFlags &= ~(kQueueYsMark | kQueueAm2Logo);
        HLib::AssignCallbackForTask(control->titleSequenceTask, TITLE_SEQUENCE_TABLE[static_cast<std::size_t>(titleLoopSequence)].display);
        needsTitleSequenceDraw = 0;
    } else {
        if (logoFlags & kQueueYsMark) {
            currentTitleLogo = shenPlayAudioWithFlags("YSMARK", 17);
            logoFlags &= ~(kQueueYsMark | kQueueAm2Logo);
        }
        if (logoFlags & kQueueAm2Logo) {
            ConversationBuffer *const logo = shenPlayAudioWithFlags("AM2LOGO", 17);
            logoFlags &= ~(kQueueYsMark | kQueueAm2Logo);
            currentTitleLogo = logo;
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void TitleControl(void *taskData) {
    TitleControlData *const control = static_cast<TitleControlData *>(taskData);
    TitleDisplayData *const display = static_cast<TitleDisplayData *>(HLib::GetTaskParameter(control->titleSequenceTask));
    std::size_t sequence = static_cast<std::size_t>(titleLoopSequence);
    if (control->state > 0) {

        HLib::SetCurrentTaskTo(TITLE_SECOND_CALLBACKS[control->state - 1].control);
        HLib::AssignCallbackForTask(control->titleSequenceTask, TITLE_SECOND_CALLBACKS[control->state - 1].display);

        control->state = 0;
        return;
    }

    const std::uint32_t flags = logoFlags;
    ++display->frameCount;
    if ((flags & kTimerMask) == kTimerPattern) {
        if (SplashSequenceComplete())
            titleSequenceTime -= shenGetScaledDeltaTickTime(1.0f);
        if (titleSequenceTime < 0.0f) {
            SetTitleWarningLogo(0, nullptr);
            (void)GetLocalBackupInfo();
            if (titleLoopSequence == TitleSequence::MoveToAm2Logos6) {
                if (currentTitleLogo) {
                    scenCvsCtrlFree(currentTitleLogo);
                    currentTitleLogo = nullptr;
                }
                TitleAudioControl(0);
            } else {
                TitleAudioControl(-1);
                if (titleLoopSequence == TitleSequence::MoveToAm2Logos8)
                    logoFlags |= kQueueYsMark;
                else if (titleLoopSequence == TitleSequence::MoveToAm2Logos3)
                    logoFlags |= kQueueAm2Logo;
                else if (currentTitleLogo) {
                    scenCvsCtrlFree(currentTitleLogo);
                    currentTitleLogo = nullptr;
                }
            }
            titleLoopSequence = static_cast<TitleSequence>(static_cast<std::int32_t>(titleLoopSequence) + 1);
            sequence = static_cast<std::size_t>(titleLoopSequence);
            titleSequenceTime = static_cast<float>(TITLE_SEQUENCE_TABLE[sequence].duration);
            display->frameCount = 0;
            d3t::SetSceneClearColour(0xff000000U);
            HLib::AssignCallbackForTask(control->titleSequenceTask, TITLE_SEQUENCE_TABLE[sequence].display);
            logoFlags |= kFadeInProgress;
            display->currentStateFrame = 0.0f;
            display->stateFrame = 0.062745102f;
            return;
        }
    } else {
        display->currentStateFrame += 0.015686275f;
        if (display->currentStateFrame > 1.0f) {
            display->currentStateFrame = 1.0f;
            logoFlags = flags & ~kFadeInProgress;
            display->stateFrame = -0.062745102f;
        }
    }
    TITLE_SEQUENCE_TABLE[sequence].control(control);
    g_previousTitleInputUserHandle = g_inputBuffer[0].connectionState;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void ClearTitlePressStartSelection() {
    pressStartSelected = 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool SelectTitlePressStart() {
    auto *const control = static_cast<TitleControlData *>(HLib::GetTaskParameter(titleControlTask));
    if (!control) {
        return false;
    }
    pressStartSelected = 1;
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool BeginTitleOptionsTask() {
    return AssignTitleCallbacks(TitleOptionsControl, TitleOptionsDisplay);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool BeginTitlePlatformTask(void (*callback)(void *)) {
    return AssignTitleCallbacks(callback, NativeTitleNoOp);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool ReturnToTitleMenuTask() {
    if (!AssignTitleCallbacks(TitleMenuControl, TitleMenuDisplay)) {
        return false;
    }
    ReturnToTitleWheel();
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool ReturnToTitlePressStartTask() {
    if (!AssignTitleCallbacks(TitleControl, SetPressStart)) {
        return false;
    }
    auto *const control = static_cast<TitleControlData *>(HLib::GetTaskParameter(titleControlTask));
    control->display->frameCount = 0;
    titleSequenceTime = static_cast<float>(TITLE_SEQUENCE_TABLE[static_cast<std::size_t>(titleLoopSequence)].duration);
    logoFlags |= kLeaving;
    DestroyElementByIndex(UIElement::MainMenu);
    DestroyElementByIndex(UIElement::MainMenuWheel);
    ShowElementByIndex(UIElement::PressStart);
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void TitleModule(void *data) {
    SystemModuleTaskData &module = *static_cast<SystemModuleTaskData *>(data);
    bool forceAlpha = false;
    bool resumedIntro = false;
    ResetTempTextureReferences();
    ResetLegacySpriteSlotDrawState();
    g_gamePauseState = 1;
    SetTitleMenuUIState(0);
    InitializeTitleWarnings();
    const shen_name previousMap = g_currentMap;
    g_currentMap = TITLE_AREA;
    ResetMapRenderState();
    g_currentMap = previousMap;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(TitleControl, 8, 0, sizeof(TitleControlData), TITLE_CONTROL_TASK);
    TitleControlData &control = *static_cast<TitleControlData *>(HLib::GetTaskParameter(task));
    std::memset(&control, 0, sizeof(control));
    titleControlTask = task;
    TitleModuleWork &work = *static_cast<TitleModuleWork *>(module.work);
    work.frames = 300;
    work.screenFilmTask = Queue_SCF_Callback_ScreenFilmTaskEntry(15);
    InitCalendar();
    InitShotsImages();
    ResetTitleMenuSelection();
    HPLG_Enqueue();
    HPLG_RegisterCallback(TitleBackupCallback);
    logoFlags |= 0x80U;
    if (g_loadedActNumber == 1) {
        needsTitleSequenceDraw = 8;
        if (module.previousModule == SystemModule_Hot) {
            titleLoopSequence = 1;
        } else if (module.previousModule == SystemModule_Logo) {
            titleLoopSequence = 9;
        } else if (g_entryPoint == 49 && titleLoopSequence == 1) {
            titleLoopSequence = 2;
            SetTitleMenuUIState(1);
            forceAlpha = true;
            ResetTitleMenuState();
            control.state = 2;
            resumedIntro = true;
            logoFlags &= ~0x80U;
        } else if (g_entryPoint == 0 && titleLoopSequence == 1) {
            titleLoopSequence = 3;
        } else if (g_entryPoint == 49 && titleLoopSequence == 6) {
            titleLoopSequence = 1;
        } else if (titleLoopSequence == 6) {
            titleLoopSequence = 7;
        } else {
            titleLoopSequence = 1;
        }
        logoFlags |= 0x40U;
        setPressStartPending = 1;
    } else {
        titleLoopSequence = 2;
        needsTitleSequenceDraw = 2;
        setPressStartPending = 2;
        forceAlpha = true;
        logoFlags &= ~0x40U;
    }
    if (module.previousModule == SystemModule_Hot && (g_inputBuffer[0].held & 0xc0U) == 0xc0U)
        titleLoopSequence = 0;
    titleSequenceTime = titleSequenceDurations[titleLoopSequence];
    Queue_KeyRepeat(0);
    HLib::SetCurrentTaskTo(NativeTitleNoOp);
    task = HLib::EnqueueTaskWithParameter(titleDisplayCallbacks[titleLoopSequence], 9, 0, sizeof(TitleDisplayData), TITLE_DISPLAY_TASK);
    control.titleSequenceTask = task;
    TitleDisplayData &display = *static_cast<TitleDisplayData *>(HLib::GetTaskParameter(task));
    control.display = &display;
    if (titleLoopSequence == 0) {
        display.currentStateFrame = 1.0f;
        display.stateFrame = -0.062745102f;
    } else {
        display.currentStateFrame = 0.0f;
        display.stateFrame = 0.062745102f;
    }
    dc::sdk::njSetBackColor(0xff000000U, 0xff000000U, 0xff000000U);
    InitializeTitleAreaSelection();
    titleDebugSelection = 0;
    titleDebugAreaCharacter = -1;
    titleCurrentCoreModule = 0;
    SM_Init_Enqueue(1);
    CVS_CtrlConversTask_Enqueue(0);
    QueueSoundProgramControl();
    LoadTitleSoundData();
    if (resumedIntro)
        LoadTitleSoundProgram(0);
    else
        LoadTitleSoundProgram(-1);
    if (forceAlpha) {
        display.currentStateFrame = 1.0f;
        display.stateFrame = -0.062745102f;
    }
}

void ChangeTitleModuleTask(HLib::TaskCallback callback) {
    HLib::SetCurrentTaskTo(callback);
}

#else

void TitleModule(void *taskData) {
    SystemModuleTaskData *const module = static_cast<SystemModuleTaskData *>(taskData);
#if defined(_WIN32)
    g_debugMenuHeader = TitleDebugMenuHeader;
    g_debugMenuBody = TitleDebugMenuBody;
#endif
    DispatchPostFxFilterRender(0);
    ResetLegacySpriteSlotDrawState();
    g_bPaused = true;
    InitWarnLogos();
    const shen_name previousMap = g_currentMap;
    g_currentMap = titleAreaName;
    ReloadSpriteResources();
    g_currentMap = previousMap;

    pressStartSelected = 0;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TitleControl, 8, 0, sizeof(TitleControlData), TITLE_CONTROL_TASK);
    TitleControlData *const control = static_cast<TitleControlData *>(HLib::GetTaskParameter(task));
    std::memset(control, 0, sizeof(*control));
    titleControlTask = task;
    TitleModuleWork *const work = static_cast<TitleModuleWork *>(module->work);
    g_previousTitleInputUserHandle = g_inputBuffer[0].connectionState;
    work->frames = 300;
    work->screenFilmTask = Queue_SCF_Callback_ScreenFilmTaskEntry(15);
    control->screenFilmTask = work->screenFilmTask;
    InitCalendar();
    LoadAreaSpriteImages();
    RenewLocalBackupInfo();

    bool resumedIntro = false;
    logoFlags = (logoFlags & 4U) | 0x80U;
    if (module->previousModule == SystemModule::Hot) {
        titleLoopSequence = needsTitleSequenceDraw == 1 ? TitleSequence::MoveToAm2Logos2 : TitleSequence::MoveToAm2Logos5;
    } else if (module->previousModule == SystemModule::Logo) {
        needsTitleSequenceDraw = 1;
        if (!SplashSequenceComplete()) {
            ShowElementByIndex(UIElement::Splash);
        }
        titleLoopSequence = TitleSequence::MoveToAm2Logos6;
        setPressStartPending = 1;
    } else if (module->previousModule == SystemModule::Title) {
        titleLoopSequence = needsTitleSequenceDraw == 1 ? TitleSequence::MoveToAm2Logos6 : TitleSequence::MoveToAm2Logos5;
    } else if (g_entryPoint == 49 && titleLoopSequence == TitleSequence::MoveToAm2Logos5) {
        titleLoopSequence = TitleSequence::MoveToAm2Logos6;
        ResetTitleMenuState();
        resumedIntro = true;
        logoFlags &= ~0xc0U;
        control->state = 2;
        needsTitleSequenceDraw = 0;
        g_bPaused = false;
    } else if (g_entryPoint == 49 && titleLoopSequence == TitleSequence::MoveToAm2Logos7) {
        titleLoopSequence = TitleSequence::MoveToAm2Logos6;
        needsTitleSequenceDraw = 0;
    } else if (g_entryPoint != 49 && g_entryPoint == 0 && titleLoopSequence == TitleSequence::MoveToAm2Logos5) {
        titleLoopSequence = TitleSequence::MoveToAm2Logos7;
    } else if (g_entryPoint != 49 && titleLoopSequence == TitleSequence::MoveToAm2Logos7) {
        titleLoopSequence = needsTitleSequenceDraw != 1 ? TitleSequence::MoveToAm2Logos6 : TitleSequence::MoveToAm2Logos5;
    } else {
        titleLoopSequence = TitleSequence::MoveToAm2Logos5;
    }

    logoFlags |= 0x140U;
    if (g_titleFlagShouldRun != 0) {
        ResetTitleMenuState();
        logoFlags &= ~0x80U;
        control->state = 2;
        g_bPaused = false;
        if (g_titleFlagShouldRun == 1)
            SetTitleMenuSelection(static_cast<std::int32_t>(SystemModule::Hot));
        else if (g_titleFlagShouldRun == 10 || g_titleFlagShouldRun == 11)
            SetTitleMenuSelection(static_cast<std::int32_t>(SystemModule::Logo));
        else if (g_titleFlagShouldRun == 20)
            SetTitleMenuSelection(static_cast<std::int32_t>(SystemModule::Option));
        titleLoopSequence = TitleSequence::MoveToAm2Logos6;
    }
    if (module->previousModule == SystemModule::Hot && (g_inputBuffer[0].held & 0x40U)) {
        if (g_inputBuffer[0].held & 0x80U)
            titleLoopSequence = TitleSequence::PressStart;
    }
    titleSequenceTime = static_cast<float>(TITLE_SEQUENCE_TABLE[static_cast<std::size_t>(titleLoopSequence)].duration);
    Queue_KeyRepeat(0);
    HLib::SetCurrentTaskTo(NativeTitleNoOp);

    HLib::Task *const displayTask = HLib::EnqueueTaskWithParameter(
        TITLE_SEQUENCE_TABLE[static_cast<std::size_t>(titleLoopSequence)].display, 9, 0, sizeof(TitleDisplayData), TITLE_DISPLAY_TASK);
    control->titleSequenceTask = displayTask;
    control->display = static_cast<TitleDisplayData *>(HLib::GetTaskParameter(displayTask));
    control->display->frameCount = 0;
    if (g_titleFlagShouldRun != 0) {
        control->display->currentStateFrame = 1.0f;
    } else if (titleLoopSequence != TitleSequence::PressStart) {
        control->display->currentStateFrame = 0.0f;
        control->display->stateFrame = 0.062745102f;
    } else {
        control->display->currentStateFrame = 1.0f;
        control->display->stateFrame = -0.062745102f;
    }
    d3t::SetSceneClearColour(0xff000000U);
    titleDebugAct = titleSelectedAct == -1 ? g_loadedActNumber : titleSelectedAct;
    titleCurrentAreaIndex = TITLE_SCENE_GROUPS[titleDebugAct].index;
    SetTitleSceneInfo(TITLE_SCENE_GROUPS[titleDebugAct].scenes[titleCurrentAreaIndex]);
    titleDebugSelection = 0;
    titleDebugAreaCharacter = -1;
    titleCurrentCoreModule = 0;
    const bool titleSoundMainScene = resumedIntro || g_titleFlagShouldRun != 0;
    g_titleFlagShouldRun = 0;
    SM_Init_Enqueue(1);
    CVS_CtrlConversTask_Enqueue(true);
    currentTitleLogo = nullptr;
    QueueSoundProgramControl();
    std::uint32_t soundDataSize{};
    if (void *const soundData = LoadLegacySceneFile("MISC", "tit_snd.bin", TITLE_SOUND_DATA_TASK, &soundDataSize)) {
        shenAssignSoundProgramByPtr({static_cast<const std::byte *>(soundData), soundDataSize});
        shenAssignActiveSmp("0002S000.smp");
    }
    LoadSoundProgram("0002S000.sbt", titleSoundMainScene ? 0 : 0xffffffffU, 0xffffffffU);
    if (titleLoopSequence == TitleSequence::MoveToAm2Logos3)
        logoFlags |= kQueueAm2Logo;
    else if (titleLoopSequence == TitleSequence::MoveToAm2Logos8)
        logoFlags |= kQueueYsMark;
    if (resumedIntro) {
        control->display->currentStateFrame = 1.0f;
        control->display->stateFrame = -0.062745102f;
    }
    AssignActiveTCRL(control);
}

#endif

} // namespace s2
} // namespace shenmue
