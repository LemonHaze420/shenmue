#include "s2/ui_internal.hpp"
#include "s2/input.hpp"
#include <cstdio>
#include <cstring>

namespace shenmue::s2 {

struct LegacyPromptState {
    std::int32_t selection;
    std::array<char, 128> question, yes, no;
    std::int32_t timed;
    float seconds, remaining;
    std::array<PromptResponseCallback, 5> callbacks;
};
static_assert(sizeof(LegacyPromptState) == 0x1b8);
static_assert(offsetof(LegacyPromptState, timed) == 0x184);
static_assert(offsetof(LegacyPromptState, callbacks) == 0x190);

LegacyPromptState legacyPrompt{};
std::int32_t legacyPromptInitState{};
std::uint32_t legacyPromptRequest{};
UISelector *legacyPromptSelector{};
UIViewConfiguration legacyPromptConfiguration{-1, -1, -1};

// @verified - p
std::int64_t ui_detail::CloseLegacyPrompt() {
    legacyPrompt = {};
    legacyPrompt.selection = 1;
    if (legacyPromptSelector)
        legacyPromptSelector->field44 = 0;
    return 1;
}

// @verified - p
void ui_detail::OpenLegacyPrompt() {
    if (legacyPromptSelector)
        legacyPromptSelector->field44 = 1;
}

// @verified - p
void RefreshTimedPromptQuestion() {
    if (legacyPrompt.timed != 1)
        return;
    const char *const seconds = GetStringForUILabel("UIGNRLSECS");
    SubtitleStringId id{};
    std::memcpy(&id, legacyPromptSelector->label.key.data(), sizeof(id));
    std::snprintf(legacyPrompt.question.data(), legacyPrompt.question.size(), "%s%i %s", ReadStringFromStringTableById(id),
        static_cast<std::int32_t>(legacyPrompt.remaining), seconds);
    legacyPromptSelector->label.token = legacyPrompt.question.data();
    legacyPromptSelector->label.alreadyLocalized = 1;
}

// @verified - p
void BindLegacyPrompt() {
    legacyPromptSelector = UI_FindViewSelectorPointer(static_cast<std::uint32_t>(legacyPromptConfiguration.uiHandleIndex), "YesNo_Selector");
    if (!legacyPromptSelector)
        return;
    legacyPromptSelector->field44 = 1;
    legacyPromptSelector->label.token = legacyPrompt.question.data();
    legacyPromptSelector->label.alreadyLocalized = 1;
    legacyPromptSelector->options[3].token = legacyPrompt.yes.data();
    legacyPromptSelector->options[3].alreadyLocalized = 1;
    legacyPromptSelector->options[1].token = legacyPrompt.no.data();
    legacyPromptSelector->options[1].alreadyLocalized = 1;
    RefreshTimedPromptQuestion();
}

// @verified - p
std::uint32_t AdvanceLegacyPromptInit() {
    if (legacyPromptInitState == 0) {
        legacyPrompt = {};
        legacyPrompt.selection = 1;
        legacyPromptInitState = LoadUIFile("/ui/YesNoPrompt/YesNoPrompt.ui", static_cast<std::int32_t>(UIElement::YesNoPrompt), &legacyPromptRequest);
    } else if (legacyPromptInitState == 1) {
        legacyPromptInitState = GetNewUIFileInitFor(&legacyPromptRequest, legacyPromptConfiguration);
        if (legacyPromptInitState == 2)
            BindLegacyPrompt();
    } else if (legacyPromptInitState == 2) {
        legacyPromptInitState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(legacyPromptConfiguration.uiHandleIndex));
    }
    return static_cast<std::uint32_t>(legacyPromptInitState);
}

// @verified - p
std::int64_t ui_detail::RenderLegacyPrompt() {
    if (legacyPromptInitState != 3) {
        AdvanceLegacyPromptInit();
        if (static_cast<std::uint32_t>(legacyPromptInitState) - 2U > 1U)
            return 0;
    }
    RefreshTimedPromptQuestion();
    uiUpdateWithColour(*GetNativeUiContext(), static_cast<std::uint32_t>(legacyPromptConfiguration.uiHandleIndex));
    return 0;
}

// @verified - p
std::int64_t FinishLegacyPrompt(PromptResponse response) {
    const PromptResponseCallback callback = legacyPrompt.callbacks[static_cast<std::size_t>(response)];
    if (callback)
        callback();
    g_inputBuffer[0].pressedPriority = 0;
    return static_cast<std::int64_t>(response);
}

// @verified - p
std::int64_t ui_detail::LegacyPromptInput() {
    if (legacyPrompt.timed == 1) {
        if (legacyPrompt.remaining <= 0.0f)
            return FinishLegacyPrompt(PromptResponse::TimedOut);
        legacyPrompt.remaining = legacyPrompt.remaining + -0.033330001f;
    }
    const std::uint16_t input = g_inputBuffer[0].pressedPriority;
    legacyPrompt.selection = (input & 0x4000U) ? 3 : ((input & 0x8000U) ? 1 : legacyPrompt.selection);
    if (legacyPromptSelector)
        legacyPromptSelector->pos = legacyPrompt.selection;
    if (input & 0x400U) {
        if (legacyPrompt.selection == 1)
            return FinishLegacyPrompt(PromptResponse::No);
        if (legacyPrompt.selection == 3)
            return FinishLegacyPrompt(PromptResponse::Yes);
    }
    if (input & 0x200U)
        return FinishLegacyPrompt(PromptResponse::Cancelled);
    return FinishLegacyPrompt(PromptResponse::None);
}

// @verified - p
std::int64_t ui_detail::ReloadLegacyPrompt() {
    ResetUIViewSlot(legacyPromptConfiguration.uiHandleIndex);
    legacyPromptConfiguration.uiHandleIndex = -1;
    legacyPromptInitState = 0;
    legacyPromptRequest = 0;
    LoadOwnedUIFromMemoryIntoViewSlot(GetUIReloadFileMemory(), legacyPromptConfiguration.uiHandleIndex);
    legacyPromptInitState = 2;
    return 2;
}

// @verified - p
std::int64_t ui_detail::GetLegacyPromptInitState() {
    return static_cast<std::uint32_t>(legacyPromptInitState);
}

// @verified - p
void MessageBoxTimed(const char *question, const char *yes, const char *no, std::int32_t timed, float seconds) {
    std::snprintf(legacyPrompt.question.data(), legacyPrompt.question.size(), "%s", question);
    std::snprintf(legacyPrompt.yes.data(), legacyPrompt.yes.size(), "%s", yes);
    std::snprintf(legacyPrompt.no.data(), legacyPrompt.no.size(), "%s", no);
    legacyPrompt.seconds = seconds;
    legacyPrompt.remaining = seconds;
    legacyPrompt.timed = timed;
    if (!legacyPromptSelector)
        return;
    legacyPromptSelector->label.token = legacyPrompt.question.data();
    legacyPromptSelector->label.alreadyLocalized = 1;
    legacyPromptSelector->options[3].token = legacyPrompt.yes.data();
    legacyPromptSelector->options[3].alreadyLocalized = 1;
    legacyPromptSelector->options[1].token = legacyPrompt.no.data();
    legacyPromptSelector->options[1].alreadyLocalized = 1;
    RefreshTimedPromptQuestion();
}

// @verified - p
std::int32_t SetResponseCallbackForWidget(PromptResponse response, PromptResponseCallback callback) {
    legacyPrompt.callbacks[static_cast<std::size_t>(response)] = callback;
    return static_cast<std::int32_t>(response);
}

} // namespace shenmue::s2
