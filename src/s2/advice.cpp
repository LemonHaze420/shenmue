#include "s2/advice.hpp"

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/font.hpp"
#include "s2/input_dc.hpp"
#include "s2/pause.hpp"
#include "s2/window.hpp"
#include "s2/warning.hpp"
#include "s2/legacy_sprite_dc.hpp"
#else
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/pause.hpp"
#include "s2/scene_data.hpp"
#include "s2/ui.hpp"
#include "s2/font.hpp"
#include "s2/save.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/warning.hpp"
#include <bit>
#include <cstring>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#endif

namespace shenmue {
namespace s2 {

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

HLib::Task *adviceTask{};

AdviceControlMode adviceMode;

std::int32_t advicePhase, adviceRetireFrames{};

std::uint32_t adviceTargetColour{}, adviceCurrentColour{};

const char *adviceCurrentToken{};

std::array<char, 128> adviceLine;

char *adviceLineCursor;

std::int16_t adviceLegacyRow{};

std::uint8_t adviceLineCount, adviceTabCount{}, adviceCharacters{}, adviceIconCount{};

std::array<std::array<std::uint8_t, 8>, 10> adviceLegacyIcons{};

static_assert(sizeof(adviceLine) == 0x80);

static_assert(sizeof(adviceLegacyIcons) == 0x50);

static_assert(sizeof(adviceLegacyIcons[0]) == 8);

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
namespace {

void AdviceControl_Callback(void *);
void FightAdviceControlTaskCallback(void *);

} // namespace

std::int64_t FightAdviceControlCallback();
void FormatAdviceHelpText(std::string_view text);
void AssignAdviceControlMode(AdviceControlMode mode) noexcept;
void QueueFightAdviceControl(std::uint8_t mode);
#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// Native idle callback is the explicit PC nullsub boundary.
void AdviceIdleNullsub(void *) {
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void StepAdviceColour() {
    std::uint32_t distance = adviceCurrentColour <= adviceTargetColour ? adviceTargetColour - adviceCurrentColour : adviceCurrentColour - adviceTargetColour;
    if (distance > 0x0e000000U)
        distance = 0x0e000000U;
    if (adviceCurrentColour <= adviceTargetColour)
        adviceCurrentColour += distance;
    else
        adviceCurrentColour -= distance;
    if (adviceCurrentColour)
        vfSubmitQuad(adviceCurrentColour, 6.0f);
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void DrawAdvice(void *);

void AdviceControl_Callback(void *);

void FightAdviceControlTaskCallback(void *);

std::uint32_t adviceCurrentColour, adviceTargetColour = 0;

AdviceControlMode adviceMode;

std::int32_t advicePhase, adviceRetireFrames = 0;

const char *adviceCurrentMessage = 0;

char adviceLine[0x34];

char *adviceLineCursor;

std::int16_t adviceTextureRow;

std::uint32_t adviceColumn, adviceLineCount;

AdviceButton adviceButtons[10];

std::uint32_t adviceIconCount = 0;

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

void DrawAdvice(void *) {
    LegacyIconSprite text, button;
    const std::int16_t *textures;
    std::int32_t index;
    text.flags = 0;
    text.alignment = 4;
    text.depth = 8.0f;
    text.scaleX = 1.0f;
    text.scaleY = 1.0f;
    text.rotation = 0;
    text.y = 80.0f;
    text.height = 32;
    textures = WRN_GetWarningTextures();
    index = 0;
    do {
        text.x = 32.0f;
        text.width = 512;
        if (static_cast<std::int16_t>(text.icon = *textures++) >= 0)
            PrepareDrawTextureWithMode(text);
        text.x += 512.0f;
        text.width = 128;
        if (static_cast<std::int16_t>(text.icon = *textures++) >= 0)
            PrepareDrawTextureWithMode(text);
        text.y = 40.0f + text.y;
        ++index;
    } while (index < 10);
    button.flags = 0;
    button.alignment = 4;
    button.depth = 8.0f;
    button.rotation = 0;
    for (index = 0; index < static_cast<std::int32_t>(adviceIconCount); ++index) {
        button.x = (static_cast<float>(adviceButtons[index].column) * 26.0f + 32.0f) + 5.0f;
        button.y = (static_cast<float>(adviceButtons[index].line) * 40.0f + 80.0f);
        float &scaleX = button.scaleX;
        float &scaleY = button.scaleY;
        scaleX = 1.2f;
        scaleY = scaleX;
        button.width = 32;
        button.height = 32;
        if (adviceButtons[index].icon == 8) {
            button.scaleX = 1.0f;
            button.scaleY = 0.8f;
            button.height = button.width = 64;
            button.y += 5.0f;
        }
        button.icon = adviceButtonIcons[adviceButtons[index].icon];
        PrepareDrawSimpleOpaque(button, adviceButtonColours[adviceButtons[index].icon]);
    }
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void AdviceControl_Callback(void *) {
    switch (advicePhase) {
    case 0:
        if (static_cast<std::int32_t>(adviceMode) > 0) {
            KanjiFontLoad("DC_KANJI.FON");
            wndSetFlag(0, 1);
            adviceCurrentMessage = adviceMessages[static_cast<std::int32_t>(adviceMode) - 1];
            advicePhase = 1;
            adviceTargetColour = 0x76000000U;
        }
        break;
    case 1:
        adviceLineCursor = adviceLine;
        adviceTextureRow = 0;
        adviceColumn = adviceLineCount = 0;
        adviceIconCount = 0;
        FormatAdviceHelpText(adviceCurrentMessage);
        for (; adviceTextureRow <= 19; adviceTextureRow = static_cast<std::int16_t>(adviceTextureRow + 2))
            WRN_UploadWarningLine(adviceTextureRow, "");
        advicePhase = 2;
        break;
    case 2:
        if (static_cast<std::int32_t>(adviceMode) > 0)
            HLib::ExecuteCallbackNow(DrawAdvice, 0);
        else {
            advicePhase = 3;
            adviceRetireFrames = 4;
            adviceTargetColour = 0;
        }
        break;
    case 3:
        adviceRetireFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(adviceRetireFrames) - 1U);
        if (adviceRetireFrames < 0)
            advicePhase = 0;
        break;
    }
    if (adviceCurrentColour > adviceTargetColour) {
        std::uint32_t distance = adviceCurrentColour - adviceTargetColour;
        if (distance > 0x0e000000U)
            distance = 0x0e000000U;
        adviceCurrentColour -= distance;
    } else {
        std::uint32_t distance = adviceTargetColour - adviceCurrentColour;
        if (distance > 0x0e000000U)
            distance = 0x0e000000U;
        adviceCurrentColour += distance;
    }
    if (adviceCurrentColour)
        DrawMainIconOverlay(adviceCurrentColour, 6.0f);
}

} // namespace

#else

namespace {

void AdviceControl_Callback(void *) {
    switch (advicePhase) {
    case 0:
        if (static_cast<std::int32_t>(adviceMode) > 0) {
            KanjiFontLoad("DC_KANJI.FON");
            wndSetFlag(nullptr, 1);
            advicePhase = 1;
            adviceTargetColour = 0x76000000U;
            adviceCurrentToken = ADVICE_TEXT_TOKENS[static_cast<std::int32_t>(adviceMode) - 1];
        }
        break;
    case 1:
        adviceLineCursor = adviceLine.data();
        adviceLegacyRow = 0;
        adviceLineCount = 0;
        adviceTabCount = 0;
        adviceIconCount = 0;
        HelpWidgetResetRuns();
        FormatAdviceHelpText(adviceCurrentToken ? GetLocalizedSceneText(adviceCurrentToken) : std::string_view{});
        ShowElementByIndex(UIElement::GameHelp);
        for (; adviceLegacyRow < 20; adviceLegacyRow = static_cast<std::int16_t>(adviceLegacyRow + 2))
            WRN_UploadWarningLine(adviceLegacyRow);
        advicePhase = 2;
        break;
    case 2:
        if (static_cast<std::int32_t>(adviceMode) <= 0) {
            advicePhase = 3;
            adviceTargetColour = 0;
            adviceRetireFrames = 4;
        } else
            HLib::ExecuteCallbackNow(AdviceIdleNullsub, nullptr);
        break;
    case 3:
        adviceRetireFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(adviceRetireFrames) - 1U);
        if (adviceRetireFrames < 0)
            advicePhase = 0;
        break;
    default:
        break;
    }
    StepAdviceColour();
}

} // namespace

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void FightAdviceControlTaskCallback(void *) {
    if (g_inputBuffer[0].pressedPriority & 0x0a00U) {
        AssignAdviceControlMode(AdviceControlMode_None);
        RequestFightTelopResume();
        HLib::DestroyCurrentTask();
    }
}

} // namespace

#else

namespace {

void FightAdviceControlTaskCallback(void *) {
    (void)FightAdviceControlCallback();
}

} // namespace

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t FightAdviceControlCallback() {
    if ((g_inputBuffer[0].pressedPriority & 0x0200U) == 0)
        return 512;
    DestroyElementByIndex(UIElement::GameHelp);
    AssignAdviceControlMode(AdviceControlMode::None);
    g_loadingFlag = (g_loadingFlag & ~0x0c00U) | 0x0800U;
    return HLib::DestroyCurrentTask();
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void Queue_ADV_CtrlAdvice() {
    advicePhase = 0;
    adviceMode = AdviceControlMode_None;
    HLib::EnqueueTaskWithoutParameter(AdviceControl_Callback, 15, 0, TASK_ADVICE_CONTROL);
    adviceCurrentColour = adviceTargetColour = 0;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void AssignAdviceControlMode(AdviceControlMode mode) {
    if (static_cast<std::int32_t>(mode) > 19)
        mode = AdviceControlMode_None;
    adviceMode = mode;
}

#else

// @verified - p
void AssignAdviceControlMode(AdviceControlMode mode) noexcept {
    const std::int32_t value = static_cast<std::int32_t>(mode);
    adviceMode = value < 21 ? mode : AdviceControlMode::None;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

AdviceControlMode GetAdviceControlMode() noexcept {
    return adviceMode;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void QueueFightAdviceControl(std::uint8_t mode) {
    HLib::EnqueueTaskWithoutParameter(FightAdviceControlTaskCallback, 15, 0, TASK_FIGHT_ADVICE);
    AssignAdviceControlMode(static_cast<AdviceControlMode>(mode));
    RequestFightTelopPause();
}

#else

// @verified - p
void QueueFightAdviceControl(std::uint8_t mode) {
    (void)HLib::EnqueueTaskWithoutParameter(FightAdviceControlTaskCallback, 15, 0, TASK_FIGHT_ADVICE.name);
    AssignAdviceControlMode(static_cast<AdviceControlMode>(mode));
    g_loadingFlag |= 0x0400U;
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void ReleaseAdviceControlOwners() noexcept {
    DestroyElementByIndex(UIElement::GameHelp);
    if (adviceTask)
        HLib::DestroyTaskInternal(adviceTask);
    adviceTask = nullptr;
    adviceMode = AdviceControlMode::None;
    advicePhase = 0;
    adviceRetireFrames = 0;
    adviceTargetColour = 0;
    adviceCurrentColour = 0;
    adviceCurrentToken = nullptr;
}

#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void FormatAdviceHelpText(const char *text) {
    char character;
    while ((character = *text++) != 0) {
        if (character == '0') {
            *adviceLineCursor = 0;
            WRN_UploadWarningLine(adviceTextureRow, adviceLine);
            adviceLineCursor = adviceLine;
            adviceTextureRow = static_cast<std::int16_t>(adviceTextureRow + 2);
            adviceColumn = 0;
            ++adviceLineCount;
        } else if (character == '\t') {
            *adviceLineCursor++ = character;
            adviceColumn = 14;
        } else if (character >= '1' && character <= '9') {
            *adviceLineCursor++ = static_cast<char>(character - '0');
            adviceColumn += character - '0';
        } else if (character == '^') {
            character = 2;
            switch (*text++) {
            case 'C':
                adviceButtons[adviceIconCount].icon = 0;
                break;
            case 'N':
                adviceButtons[adviceIconCount].icon = 1;
                break;
            case 'L':
                adviceButtons[adviceIconCount].icon = 2;
                break;
            case 'R':
                adviceButtons[adviceIconCount].icon = 3;
                break;
            case 'A':
                adviceButtons[adviceIconCount].icon = 4;
                break;
            case 'B':
                adviceButtons[adviceIconCount].icon = 5;
                break;
            case 'X':
                adviceButtons[adviceIconCount].icon = 6;
                break;
            case 'Y':
                adviceButtons[adviceIconCount].icon = 7;
                break;
            case 'V':
                adviceButtons[adviceIconCount].icon = 9;
                break;
            default:
                adviceButtons[adviceIconCount].icon = 8;
                character = 3;
                break;
            }
            adviceButtons[adviceIconCount].column = static_cast<std::int8_t>(adviceColumn);
            adviceButtons[adviceIconCount].line = static_cast<std::int8_t>(adviceLineCount);
            ++adviceIconCount;
            adviceColumn += character;
            *adviceLineCursor++ = character;
        } else {
            *adviceLineCursor++ = character;
            *adviceLineCursor++ = *text++;
            ++adviceColumn;
        }
    }
}

#else

void FormatAdviceHelpText(std::string_view text) {
    while (!text.empty()) {
        std::uint32_t codepoint = 0;
        std::size_t length = DecodeHelpCodepoint(text, codepoint);
        if (!length)
            break;
        if (length != 1 || codepoint >= 0x80) {
            std::memcpy(adviceLineCursor, text.data(), length);
            adviceLineCursor += length;
            ++adviceCharacters;
            text.remove_prefix(length);
            continue;
        }
        text.remove_prefix(length);
        switch (codepoint) {
        case '&':
            *adviceLineCursor = 0;
            WRN_UploadWarningLine(adviceLegacyRow);
            HelpWidgetParseLine(adviceLine.data());
            adviceLegacyRow = static_cast<std::int16_t>(adviceLegacyRow + 2);
            ++adviceLineCount;
            adviceLineCursor = adviceLine.data();
            adviceTabCount = 0;
            adviceCharacters = 0;
            break;
        case '\t':
            *adviceLineCursor++ = '\t';
            ++adviceTabCount;
            if (adviceTabCount == 1)
                adviceCharacters = 0;
            else if (adviceTabCount == 2)
                adviceCharacters = 22;
            break;
        case '*': {
            length = DecodeHelpCodepoint(text, codepoint);
            if (length)
                text.remove_prefix(length);
            const std::int32_t spaces = std::bit_cast<std::int32_t>(codepoint - 48U);
            for (std::int16_t count = 0; count < spaces; ++count) {
                *adviceLineCursor++ = ' ';
                ++adviceCharacters;
            }
            break;
        }
        case '^':
            if (adviceIconCount != 10) {
                length = DecodeHelpCodepoint(text, codepoint);
                if (length)
                    text.remove_prefix(length);
                std::uint8_t icon = 0;
                switch (codepoint) {
                case 'A':
                    icon = 2;
                    break;
                case 'B':
                    icon = 3;
                    break;
                case 'G':
                    icon = 16;
                    break;
                case 'H':
                    icon = 15;
                    break;
                case 'K':
                    icon = 14;
                    break;
                case 'L':
                    icon = 6;
                    break;
                case 'P':
                    icon = 13;
                    break;
                case 'Q':
                    icon = g_s2JapaneseVersion != 0 ? 7 : 6;
                    break;
                case 'R':
                    icon = 7;
                    break;
                case 'S':
                    icon = 8;
                    break;
                case 'W':
                    icon = g_s2JapaneseVersion == 0 ? 7 : 6;
                    break;
                case 'X':
                    icon = 4;
                    break;
                case 'Y':
                    icon = 5;
                    break;
                default:
                    break;
                }
                adviceLegacyIcons[adviceIconCount][0] = adviceLineCount;
                adviceLegacyIcons[adviceIconCount][1] = icon;
                HelpWidgetAddButtonIcon(icon);
                *adviceLineCursor++ = '^';
                ++adviceIconCount;
            }
            break;
        default:
            *adviceLineCursor++ = static_cast<char>(codepoint);
            ++adviceCharacters;
            break;
        }
    }
}

#endif

#if !(defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

HLib::Task *QueueAdviceControl() {
    advicePhase = 0;
    adviceMode = AdviceControlMode::None;
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(AdviceControl_Callback, 15, 0, TASK_ADVICE_CONTROL.name);
    adviceTargetColour = 0;
    adviceCurrentColour = 0;
    adviceTask = task;
    return task;
}

#endif

} // namespace s2
} // namespace shenmue

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" void __attribute__((renesas)) shenmue_s2_event_QueueFightAdviceControl(std::uint8_t mode) {
    shenmue::s2::QueueFightAdviceControl(mode);
}
#else
#endif
