#include "s2/advice.hpp"

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

namespace shenmue::s2 {
namespace {

HLib::Task *adviceTask{};
AdviceControlMode adviceMode{};
std::int32_t advicePhase{}, adviceRetireFrames{};
std::uint32_t adviceTargetColour{}, adviceCurrentColour{};
const char *adviceCurrentToken{};
std::array<char, 128> adviceLine{};
char *adviceLineCursor{};
std::int16_t adviceLegacyRow{};
std::uint8_t adviceLineCount{}, adviceTabCount{}, adviceCharacters{}, adviceIconCount{};
std::array<std::array<std::uint8_t, 8>, 10> adviceLegacyIcons{};
static_assert(sizeof(adviceLine) == 0x80);
static_assert(sizeof(adviceLegacyIcons) == 0x50);
static_assert(sizeof(adviceLegacyIcons[0]) == 8);


void AdviceIdleNullsub(void *) {
}

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

void FightAdviceControlTaskCallback(void *) {
    (void)FightAdviceControlCallback();
}

} // namespace

// @verified - p
std::int64_t FightAdviceControlCallback() {
    if ((g_inputBuffer[0].pressedPriority & 0x0200U) == 0)
        return 512;
    DestroyElementByIndex(UIElement::GameHelp);
    AssignAdviceControlMode(AdviceControlMode::None);
    g_loadingFlag = (g_loadingFlag & ~0x0c00U) | 0x0800U;
    return HLib::DestroyCurrentTask();
}

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

HLib::Task *QueueAdviceControl() {
    advicePhase = 0;
    adviceMode = AdviceControlMode::None;
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(AdviceControl_Callback, 15, 0, TASK_ADVICE_CONTROL.name);
    adviceTargetColour = 0;
    adviceCurrentColour = 0;
    adviceTask = task;
    return task;
}

// @verified - p
void AssignAdviceControlMode(AdviceControlMode mode) noexcept {
    const std::int32_t value = static_cast<std::int32_t>(mode);
    adviceMode = value < 21 ? mode : AdviceControlMode::None;
}

AdviceControlMode GetAdviceControlMode() noexcept {
    return adviceMode;
}

// @verified - p
void QueueFightAdviceControl(std::uint8_t mode) {
    (void)HLib::EnqueueTaskWithoutParameter(FightAdviceControlTaskCallback, 15, 0, TASK_FIGHT_ADVICE.name);
    AssignAdviceControlMode(static_cast<AdviceControlMode>(mode));
    g_loadingFlag |= 0x0400U;
}

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

} // namespace shenmue::s2
