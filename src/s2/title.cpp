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

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <bit>
#include <limits>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
void TitleControl(void *taskData);
std::uint16_t g_previousTitleInputUserHandle = 0;
std::int32_t g_titleFlagShouldRun = 0;
std::uint32_t g_titleVersionOverride{};
std::uint32_t GetTitleVersionOverride() noexcept {
    return g_titleVersionOverride;
}
void SetTitleVersionOverride(std::uint32_t value) noexcept {
    g_titleVersionOverride = value;
}
std::int32_t g_titleMenuSelection = -1;
std::int32_t g_previousTitleMenuSelection = -1;
std::int32_t g_titleMenuUIState{};
std::int64_t (*g_titleMenuTransitionCallback)(){};
std::int32_t g_titleMenuTransitionRequested{};

// @verified - p
void SetTitleMenuUIState(std::int32_t state) noexcept {
    g_titleMenuUIState = state;
}
// @verified - p
void SetTitleMenuTransitionCallback(std::int64_t (*callback)()) noexcept {
    g_titleMenuTransitionCallback = callback;
}
// @verified - p
void RequestTitleMenuTransition() noexcept {
    g_titleMenuTransitionRequested = 1;
}

bool shenIsInIntro() noexcept {
    // shenIsInIntro reads a DWORD at Version+0xdc, not a byte at +0x37.
    return GetS2AudioSettings().dialogueMode != 3 || (g_actNumber == 1 && g_currentMap == AREA_OP02);
}

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
std::int32_t needsTitleSequenceDraw = 0;
std::int32_t setPressStartPending = 0;
std::int32_t pressStartSelected = 0;
float titleSequenceTime = 0.0f;
std::int32_t titleAttractSkipRequested{};
std::int32_t titleWheelTransitionAngle{};
ConversationBuffer *currentTitleLogo = nullptr;
std::int32_t titleMenuDialogueMode = 0;
std::int32_t titleMenuControlMode = 0;
std::int32_t titleMenuTriggerSwap = 0;
std::int32_t titleOptionsSelection = 0;
std::array<std::int32_t, 4> titleOptionItems{};
std::int32_t titleOptionItemCount = 0;
std::int16_t titleSelectedAct = -1;
std::int32_t titleDebugAct = 0;
std::int32_t titleDebugSaveSlot = -1;
std::int8_t titleDebugSaveLoaded{};
std::int32_t titleCurrentAreaIndex = 0;
std::int32_t titleDebugSelection = 0;
std::int32_t titleDebugAreaCharacter = 0;
std::int32_t titleCurrentCoreModule = 0;
std::int32_t titleAnalogInputLatched{};

// @verified - p
std::int32_t FindLastTitleSceneGroup(const TitleSceneGroup *groups) {
    std::int32_t count = 0;
    while (groups[count].scenes != reinterpret_cast<const TitleSceneEntry *>(static_cast<std::uintptr_t>(-1)))
        ++count;
    return count - 1;
}

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

// @verified - p
void SetTitleDebugStartTime(shen_name area) {
    DateTime time;
    GetCurrentDateTime(time);
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

const std::array<TitleDebugModuleEntry, 6> TITLE_DEBUG_MODULES{{
    {.name = "             ", .module = SystemModule::Game, .control = DebugTitleSceneSelector, .initialize = InitializeTitleDebugGame},
    {.name = " STAFF ROLL  ", .module = SystemModule::Credits, .initialize = GetDebugForceUIEnabled},
    {.name = "   OUTRUN    ", .module = SystemModule::OutRun, .initialize = GetDebugForceUIEnabled},
    {.name = "  HARRIER    ", .module = SystemModule::SpaceHarrier, .initialize = GetDebugForceUIEnabled},
    {.name = "   HANGON    ", .module = SystemModule::HangOn, .initialize = GetDebugForceUIEnabled},
    {.name = "AFTER BURNER ", .module = SystemModule::AfterBurner, .initialize = GetDebugForceUIEnabled},
}};

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
void NativeTitleNoOp(void *) {
}

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

void TitleMenuDisplay(void *) {
    EnableElement(UIElement::MainMenu);
}

const std::array<TitleCallbackPair, 2> TITLE_SECOND_CALLBACKS{{
    {.control = SwitchSystemModuleTaskForFrontend, .display = SetPressStart},
    {.control = TitleMenuControl, .display = TitleMenuDisplay},
}};
void TitleOptionsControl(void *) {
    padcCheckControllerConnection();
    ProcessTitleOptionsInput();
    g_previousTitleInputUserHandle = FrontendControllerUserHandle();
}

void TitleOptionsDisplay(void *) {
    ShowElementByIndex(UIElement::SubMenuOptions);
    EnableElement(UIElement::SubMenuOptions);
}

void StartTitleCreditsMovie(void *) {
    ChangeSystemModule(SystemModule ::Movie, 0);
}

void DrawTitleCreditsFrame(void *) {
    DrawTitleFrame(1);
}

void StartNextTitleMenuSequence(void *data) {
    if ((g_inputBuffer[0].pressedPriority & 0x0e00U) == 0)
        return;
    SetTitleWarningLogo(0, nullptr);
    HLib::SetCurrentTaskTo(TitleMenuControl);
    TitleControlData *const control = static_cast<TitleControlData *>(data);
    if (control->titleSequenceTask)
        HLib::AssignCallbackForTask(control->titleSequenceTask, TitleMenuDisplay);
}

void ShowTitleStorageWarning(void *data) {
    SetTitleWarningLogo(42, static_cast<TitleControlData *>(data)->warningParameters);
    vfSubmitQuad(0x7f000000U, -2.5900004f);
}

void ShowTitleOptionsWarning(void *data) {
    SetTitleWarningLogo(45, static_cast<TitleControlData *>(data)->warningParameters);
    vfSubmitQuad(0x7f000000U, -2.5900004f);
}

void ShowTitleArcadeWarning(void *data) {
    SetTitleWarningLogo(44, static_cast<TitleControlData *>(data)->warningParameters);
    vfSubmitQuad(0x7f000000U, -2.5900004f);
}

void StartTitleHangOnMovie(void *) {
    ChangeSystemModule(SystemModule::Movie, 0);
}

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

bool AssignTitleCallbacks(HLib::TaskCallback controlCallback, HLib::TaskCallback displayCallback) {
    auto *const control = static_cast<TitleControlData *>(HLib::GetTaskParameter(titleControlTask));
    if (!control || !controlCallback || !control->titleSequenceTask || !displayCallback) {
        return false;
    }
    HLib::AssignCallbackForTask(control->titleSequenceTask, displayCallback);
    HLib::AssignCallbackForTask(titleControlTask, controlCallback);
    return true;
}

std::uint32_t TitleAlphaColour(float alpha) {
    const float scaled = alpha * 255.0f;
    const std::int64_t truncated =
        scaled >= -9223372036854775808.0f && scaled < 9223372036854775808.0f ? static_cast<std::int64_t>(scaled) : std::numeric_limits<std::int64_t>::min();
    return (static_cast<std::uint32_t>(truncated) << 24) | 0x00ffffffU;
}

} // namespace

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

// @verified - p
void ReturnToTitleControlTasks() {
    titleAttractSkipRequested = 0;
    ResetSelectedInputDevice();
    g_bPaused = true;
    SetupTitleMenuTask(0);
    TitleAudioControl(-1);
    logoFlags |= kLeaving;
}

// @verified - p
void RequestTitleAttractSkip() {
    titleAttractSkipRequested = 1;
}
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

// @verified - p
void SubmitTitleScreenQuad(std::int32_t texture, float alpha) {
    d3t::SetSceneClearColour(0xffffffffU);
    LegacySpritePacket packet{.quad = {.flags = 0x400}, .colour = TitleAlphaColour(alpha), .uv = {0.0f, 0.0f, 1.0f, 1.0f}};
    const std::array<std::int16_t, 2> &dimensions = TITLE_LOGO_DIMENSIONS[texture - 2];
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 320, 240, dimensions[0], dimensions[1], ScreenMode::Centered);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(titleSpriteTextures[texture]), -2.9f);
}

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

// @verified - p
void DrawTitleDisc(void *data) {
    SubmitTitleScreenQuad(4, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}
// @verified - p
void DrawTitleYsMark(void *data) {
    SubmitTitleScreenQuad(5, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}
// @verified - p
void DrawTitleSegaLogo(void *data) {
    SubmitTitleScreenQuad(3, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}
// @verified - p
void DrawTitleAm2Logo(void *data) {
    SubmitTitleScreenQuad(2, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}
// @verified - p
void DrawTitleAdditionalLogo(void *data) {
    SubmitTitleScreenQuad(6, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}
// @verified - p
void DrawTitleWarning41(void *) {
    SetTitleWarningLogo(41, nullptr);
}
// @verified - p
void DrawTitleWarning40(void *) {
    SetTitleWarningLogo(40, nullptr);
}
// @verified - p
void DrawTitleWarning1(void *) {
    SetTitleWarningLogo(1, nullptr);
}
// @verified - p
void TitleDisplayNullsub(void *) {
}

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

// @verified - p
void SetTitleLogoPresent() {
    logoFlags |= kHasCurrentLogo;
}

// @verified - p
void ResetTitleMenuState() {
    titleMenuDialogueMode = static_cast<std::uint8_t>(GetS2DialogueMode());
    titleMenuTriggerSwap = AreS2TriggersSwapped();
    titleMenuControlMode = GetS2ControlMode();
    titleOptionsSelection = 0;
    SetTitleLogoPresent();
    titleOptionItems = TITLE_OPTION_ITEMS;
    titleOptionItemCount = 4;
}

// @verified - p
void SetTitleMenuSelection(std::int32_t selection) {
    g_titleMenuSelection = selection;
    g_previousTitleMenuSelection = selection;
}

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

// @verified - p
void TitleAudioControl(std::int32_t sceneIndex) {
    LoadSoundProgram("0002S000.sbt", static_cast<std::uint32_t>(sceneIndex >> 31), 0xffffffffU);
    if (currentTitleLogo) {
        scenCvsCtrlFree(currentTitleLogo);
        currentTitleLogo = nullptr;
    }
}

// @verified - p
void ApplyTitleMenuOptions() {
    ApplyS2TriggerSwapMode(titleMenuTriggerSwap);
    ApplyS2DialogueMode(static_cast<std::uint8_t>(GetS2AudioSettings().dialogueMode));
    ApplyS2ControlMode(titleMenuControlMode);
} // namespace

// @verified - p
void CheckTitleDebugInput(void *data) {
    TitleControlData *const control = static_cast<TitleControlData *>(data);
    if (control->display->currentStateFrame >= 1.0f && (g_inputBuffer[0].pressedPriority & 0x0fffU)) {
        ResetTitleMenuState();
        control->state = 1;
    }
}

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

// @verified - p
void LoadTitleIntroArea(void *) {
    g_actNumber = 1;
    g_currentMap = AREA_0001;
    g_entryPoint = 1;
    ExecuteAicaCommand(0x1200a0, 5, 0);
    ChangeSystemModule(SystemModule::Game, 0);
}

// @verified - p
void LoadTitleAttractArea(void *) {
    g_actNumber = 1;
    g_currentMap = AREA_OP02;
    g_entryPoint = 1;
    d3t::SetSceneClearColour(0xff000000U);
    ExecuteAicaCommand(0x1200a0, 5, 0);
    ChangeSystemModule(SystemModule::Game, 0);
}

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

bool SelectTitlePressStart() {
    auto *const control = static_cast<TitleControlData *>(HLib::GetTaskParameter(titleControlTask));
    if (!control) {
        return false;
    }
    pressStartSelected = 1;
    return true;
}

bool BeginTitleOptionsTask() {
    return AssignTitleCallbacks(TitleOptionsControl, TitleOptionsDisplay);
}

bool BeginTitlePlatformTask(void (*callback)(void *)) {
    return AssignTitleCallbacks(callback, NativeTitleNoOp);
}

bool ReturnToTitleMenuTask() {
    if (!AssignTitleCallbacks(TitleMenuControl, TitleMenuDisplay)) {
        return false;
    }
    ReturnToTitleWheel();
    return true;
}

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

void TitleModule(void *taskData) {
    SystemModuleTaskData *const module = static_cast<SystemModuleTaskData *>(taskData);
    DispatchPostFxFilterRender(0);
    ResetLegacySpriteSlotDrawState();
    g_bPaused = true;
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

} // namespace shenmue::s2
