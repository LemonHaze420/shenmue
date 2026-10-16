#include "s1/task/esop.hpp"

#include "s1/boot.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/input.hpp"
#include "s1/player_stats.hpp"
#include "s1/save_format.hpp"
#include "s1/texture_cache.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/task/sond.hpp"

#include <cstdint>
#include <array>

namespace shenmue::s1 {

extern std::uint8_t g_disableClockAndActionSelector;
extern std::int32_t g_busy_reset;

EasyOptionState g_easyOptionState{};
float g_runStartSpeed = 3.0f;
HLib::Task *g_easyOptionTask = nullptr;

namespace {

constexpr std::uint8_t kIdle = 0;
constexpr std::uint8_t kInitialize = 1;
constexpr std::uint8_t kMoveToWaitForInput = 2;
constexpr std::uint8_t kWaitForUserInput = 3;
constexpr std::uint8_t kCleanup = 4;

constexpr std::uint16_t kCancelButton = 0x0200;
constexpr std::int32_t kOptionsElement = 17;
constexpr std::int32_t kGameBackgroundElement = 7;
constexpr std::int32_t kYesNoPromptElement = 56;
constexpr std::int32_t kInformationBarElement = 57;
constexpr std::uint16_t kOptionsSicnState = 39;
constexpr std::uint8_t kSettingsHelpToken = 12;
constexpr std::int32_t kBootRomAllocationToken = SHEN_NAME(BROM);

constexpr std::uint8_t kDialogueValueTokens[4]{6, 5, 8, 7};
constexpr std::uint8_t kOptionRowTokens[10]{
    0xff,
    1,
    9,
    10,
    2,
    11,
    12,
    3,
    13,
    14,
};
constexpr std::uint8_t kBinaryValueTokens[2]{14, 13};

constexpr const char *kOptionHelpLines[3][2]{
    {"Toggle the sound setting", "between stereo or mono."},
    {"Enable |Area Jump+ from the Hazuki", "entrance in certain situations."},
    {"Toggle the Run and Zoom", "control buttons."},
};
constexpr const char *kDialogueHelpLines[4][2]{
    {"Play using dialog only", "to aid your progress."},
    {"Play using both dialog and text.", "Text is displayed when skip is used."},
    {"Play using both dialog and text", "to aid your progress."},
    {"Play using text only", "to aid your progress."},
};

bool AicaBootRomUnavailable() {


    void *const allocation = HLib_AllocMemBlock(0x4000, kBootRomAllocationToken);
    if (!allocation)
        return true;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(allocation), 0);
    return false;
}

void FreeLegacyTextSurfaces(EasyOptionState &state) {
    const auto release = [](std::int16_t &surface) {
        if (surface != -1)
            PVR_FreeAllocatedSurfaceByIndex(surface);
        surface = -1;
    };
    release(state.field_028);
    release(state.field_02a);
    for (std::int16_t &surface : state.fields_054)
        release(surface);
}

void RefreshSelectedOptionHelp(EasyOptionState &state) {
    std::int16_t *const surfaces[2]{&state.field_028, &state.field_02a};
    for (std::int16_t *const surface : surfaces) {
        if (*surface != -1)
            PVR_FreeAllocatedSurfaceByIndex(*surface);
        *surface = -1;
    }
    const char *const *const lines = state.selectedOption ? kOptionHelpLines[static_cast<std::int8_t>(state.selectedOption) - 1] :
                                                            kDialogueHelpLines[static_cast<std::int8_t>(state.optionValues[0])];
    state.field_028 = AllocatePVRSurface(lines[0], 512, 0xfddd, 0xf000, 0, true);
    if (lines[1])
        state.field_02a = AllocatePVRSurface(lines[1], 512, 0xfddd, 0xf000, 0, true);
}

void AllocateLegacyTextSurfaces(EasyOptionState &state) {
    FreeLegacyTextSurfaces(state);
    for (std::size_t index = 0; index < kOptionLabels.size(); ++index) {
        const EasyOptionLabelDefinition &label = kOptionLabels[index];
        state.fields_054[index] = AllocatePVRSurface(label.text, 64 << label.widthShift, 0xfddd, 0xf000, 0, true);
    }
    RefreshSelectedOptionHelp(state);
}

void InitializeRemainingOptionValues(EasyOptionState &state, std::uint32_t flags, std::uint8_t dialogueMode) {
    state.optionValues[1] = static_cast<std::uint8_t>(AicaBootRomUnavailable());
    state.optionValues[2] = static_cast<std::uint8_t>((flags & 8U) == 0);
    state.optionValues[3] = static_cast<std::uint8_t>(g_applicationSettings.savedInput.swapTriggers == 0 || g_keyboardActive == 1);

    state.menuKind = 0;
    state.dialogueValueToken = kDialogueValueTokens[dialogueMode];
    for (std::size_t index = 0; index < 10; ++index)
        state.optionRowTokens[index] = kOptionRowTokens[index];
    state.optionCount = 4;
    state.binaryValueTokens[0] = kBinaryValueTokens[0];
    state.binaryValueTokens[1] = kBinaryValueTokens[1];
    state.selectedValue = state.optionValues[static_cast<std::int8_t>(state.selectedOption)];
}

void ApplySoundMode(bool enabled) {
    void *const allocation = HLib_AllocMemBlock(0x4000, kBootRomAllocationToken);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(allocation), 0);
    shenExecuteAICACommand(enabled ? 0x900a0U : 0x800a0U, 0, 0);
    SOND_ExecuteRawAicaCommand(static_cast<std::uint32_t>(((enabled ? 14 : 15) << 16) + 0x1ba0));
}

void ApplyOptionValues(const EasyOptionState &state) {
    const std::uint8_t dialogueMode = state.optionValues[0];
    std::uint32_t flags = (S1SettingsFlags_Get() & 0xffffffcfU) | (static_cast<std::uint32_t>(dialogueMode) << 4U);
    if (g_dialogueModeVoiceTextEnable[dialogueMode][0])
        flags |= 2U;
    else
        flags &= 0xfffffffdU;
    if (g_dialogueModeVoiceTextEnable[dialogueMode][1])
        flags |= 1U;
    else
        flags &= 0xfffffffeU;
    S1SettingsFlags_Set(flags);

    ApplySoundMode(state.optionValues[1] == 0);

    flags = S1SettingsFlags_Get();
    if (state.optionValues[2])
        flags &= 0xfffffff7U;
    else
        flags |= 8U;
    if (state.optionValues[3])
        flags |= 0x40U;
    else
        flags &= 0xffffffbfU;
    S1SettingsFlags_Set(flags);
}

bool CloseElement(std::int32_t elementIndex) {
    UIBaseRuntime &element = g_uiElements[static_cast<std::size_t>(elementIndex)];
    if (!element.isVisible)
        return true;
    if (element.destroy && element.destroy() != 1 && g_busy_reset != 1) {
        return false;
    }
    element.isVisible = 0;
    return true;
}

void InitializePresentation(EasyOptionState &state) {
    d3t::ActivateElementByIdx(kOptionsElement);
    d3t::ActivateElementByIdx(kGameBackgroundElement);
    g_disableClockAndActionSelector = 1;

    if (state.padEntry && (state.padEntry->flags & 2U) == 0) {
        state.previousPadEntry = shenUpdateInputsInto(state.padEntry);
    }

    state.previousHelpToken = g_defaultHelpTokenId;
    const std::uint32_t flags = S1SettingsFlags_Get();
    const std::uint8_t dialogueMode = static_cast<std::uint8_t>((flags >> 4U) & 3U);
    state.vector_02c[0] = -640.0f;
    state.optionValues[0] = dialogueMode;
    state.vector_02c[1] = 65.0f;
    g_previousHelpTokenId = g_currentHelpTokenId;
    g_defaultHelpTokenId = kSettingsHelpToken;
    g_currentHelpTokenId = kSettingsHelpToken;
    g_helpButtonId = 0;
    g_currentHelpIconState = 0;
    state.state = kWaitForUserInput;
    state.nextState = kInitialize;
    InitializeRemainingOptionValues(state, flags, dialogueMode);
    AllocateLegacyTextSurfaces(state);
    if (state.embeddedOwner)
        return;

    const UIPresentationSnapshot presentation = UI_GetPresentationSnapshot();
    state.previousSicnPausePolicy = static_cast<std::uint8_t>(presentation.sicnPausePolicy);
    state.previousSicnState = presentation.sicnState;
    state.previousSicnActive = static_cast<std::uint8_t>(UI_SICNIsTaskActive());
    if (state.previousSicnActive) {
        UI_SICNSetState(kOptionsSicnState, UISicnPausePolicy::KeepOnPause);
    } else {
        UI_SICNEnqueue(kOptionsSicnState, UISicnPausePolicy::KeepOnPause);
    }
}

void RestorePresentation(EasyOptionState &state) {
    state.state = kIdle;
    if (state.previousPadEntry && state.padEntry && (state.padEntry->flags & 2U) != 0) {
        shenUpdateInputsInto(state.previousPadEntry);
    }
    state.nextState = kIdle;
    FreeLegacyTextSurfaces(state);

    if (state.embeddedOwner) {
        g_sleepSaveTrainingMenu.requested = 1;
        g_sleepSaveTrainingMenu.scriptResult = 0;
    } else {
        if (state.previousSicnActive) {
            UI_SICNSetState(state.previousSicnState, static_cast<UISicnPausePolicy>(state.previousSicnPausePolicy));
        } else {
            const std::uint16_t current = UI_GetPresentationSnapshot().sicnState;
            const std::uint16_t clearMask = static_cast<std::uint16_t>(((~current) & 0x0200U) | 0x0dffU);
            UI_SICNClearState(clearMask);
        }
        g_disableClockAndActionSelector = 0;
    }

    g_previousHelpTokenId = g_currentHelpTokenId;
    const std::uint8_t helpToken = static_cast<std::int8_t>(state.previousHelpToken) < 68 ? static_cast<std::uint8_t>(state.previousHelpToken) : 0;
    g_defaultHelpTokenId = helpToken;
    g_currentHelpTokenId = helpToken;
    g_helpButtonId = 0;
    g_currentHelpIconState = 0;
}

} // namespace

bool ESOP_CloseOptionsMenu() {
    if (g_uiElements[kYesNoPromptElement].isVisible || (g_currUIState != -1 && g_currUIState != kOptionsElement))
        return false;
    if (!CloseElement(kOptionsElement))
        return false;
    if (g_currUIState == -1 || g_currUIState == kGameBackgroundElement)
        (void)CloseElement(kGameBackgroundElement);
    (void)CloseElement(kInformationBarElement);
    g_easyOptionState.state = kCleanup;
    ApplyOptionValues(g_easyOptionState);
    shenExecuteAICACommand(0x900abU, 0, 0);
    return true;
}

HLib::Task *ESOP_GetTask() {
    return g_easyOptionTask;
}

HLib::Task *ESOP_Init() {
    g_easyOptionTask = HLib_EnqueueTaskWithoutParameter(SetupESOP, 14, 0, SHEN_NAME(ESOP));
    return g_easyOptionTask;
}

void ESOP_Shutdown() {
    EasyOptionState &state = g_easyOptionState;
    if (state.state != kIdle)
        RestorePresentation(state);
    if (state.padEntry) {
        if ((state.padEntry->flags & 2U) != 0)
            shenUpdateInputsInto(state.previousPadEntry);
        ReleasePADCEntry(state.padEntry);
    }
    state.padEntry = nullptr;
    state.previousPadEntry = nullptr;
    if (g_easyOptionTask)
        HLib_DestroyTask(g_easyOptionTask);
    g_easyOptionTask = nullptr;
    state = {};
}

void SetupESOP(HLib::TaskData *) {
    EasyOptionState &state = g_easyOptionState;
    state.padEntry = AllocateClearedPADCEntry();
    state.field_028 = -1;
    state.field_000 = 2;
    for (std::int16_t &value : state.fields_054)
        value = -1;
    state.field_02a = -1;
    state.field_042 = 4;
    state.field_084 = 32;
    state.selectedOption = 0;
    state.embeddedOwner = 0;
    state.state = kIdle;
    state.nextState = kIdle;
    state.field_022 = 0x200100;
    state.field_00c = 1.0f;
    state.field_010 = 1.0f;
    state.field_01c[0] = 0;
    state.field_01c[1] = 0;
    state.field_014 = 5.0f;
    state.field_018 = 0;
    state.field_026 = 5;
    state.field_0e2 = 0x200020;
    state.field_0cc = 1.0f;
    state.field_0d0 = 1.0f;
    state.field_0e6 = 5;
    state.field_0dc = 0;
    state.field_0d8 = 0;
    state.field_0c4 = nullptr;
    state.field_0e0 = 0;
    state.field_0d4 = 5.0f;
    state.vector_02c[0] = -50.0f;
    state.vector_02c[1] = 100.0f;
    state.vector_02c[2] = 1.0f;
    state.vector_02c[3] = 0.0f;
    state.field_03c = g_runStartSpeed;
    state.field_004 = -50.0f;
    if (CurrentTask) {
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->mainTaskCallbackFnPtr = UP_InitEasyOption_ESOP_Callback;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
    g_easyOptionTask = CurrentTask;
}

void UP_InitEasyOption_ESOP_Callback(HLib::TaskData *) {
    if ((Shenmue_nPauseState & 2) != 0)
        return;

    EasyOptionState &state = g_easyOptionState;
    switch (state.state) {
    case kInitialize:
        InitializePresentation(state);
        break;
    case kMoveToWaitForInput:
        state.state = kWaitForUserInput;
        break;
    case kWaitForUserInput:
        state.selectedValue = state.optionValues[static_cast<std::int8_t>(state.selectedOption)];
        if ((g_inputState.slots[0].pressedPriority & kCancelButton) != 0 && g_optionsPendingAction == -1) {
            g_inputState.slots[0].pressedPriority &= static_cast<std::uint16_t>(~kCancelButton);
            if (!ConfirmChanges())
                ESOP_CloseOptionsMenu();
        }
        break;
    case kCleanup:
        RestorePresentation(state);
        break;
    default:
        break;
    }
}

} // namespace shenmue::s1
