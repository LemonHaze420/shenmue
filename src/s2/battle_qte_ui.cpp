#include "s2/battle_qte_ui.hpp"

#include "s2/input.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/ui.hpp"
#include "s2/ui_transition.hpp"
#include <cstdio>

namespace shenmue::s2 {
namespace {
UIButtonIconWidget *defaultIconSheet{};
UIButtonIconWidget *platformIconSheet{};
UIButtonIconWidget *mouseIconSheet{};
d3t::UIWindow *qteDefaultPanel{};
d3t::UIWindow *qteMousePanel{};
d3t::UIWindow *qtePlatformPanel{};
std::array<float, 3> qteInitialPanelX{};
std::array<d3t::UIWindow *, 8> qteMultiPanels{};
std::array<d3t::UIWindow *, 14> qteCommandPanels{};
std::array<UIButtonIconWidget *, 12> qteCommandObjects{};
std::int32_t qteMode{}, qteTransition{};
std::int16_t qteSingleSelection{};

void SetPanelIcon(std::int32_t index, float scale, float alpha) {
    d3t::UIWindow &panel = *qteCommandPanels[static_cast<std::size_t>(index)];
    panel.layout.width.scale = scale;
    panel.layout.height.scale = scale;
    std::int32_t opacity = TruncateSpriteCoordinate(alpha * 255.0f);
    if (opacity < 0)
        opacity = 0;
    if (opacity > 255)
        opacity = 255;
    const std::int32_t mask = battleQtePanelInputMasks[static_cast<std::size_t>(index)];
    const LegacyInputSlot &input = g_inputBuffer[0];
    const bool pressed = mask != -1 && ((input.held | input.repeatedPresses | input.pressedPriority | input.newPresses) & mask) != 0;
    const std::uint32_t colour = pressed ? 0x00a2e3f7U : 0x00ffffffU;
    qteCommandObjects[static_cast<std::size_t>(index - 2)]->colour = colour | static_cast<std::uint32_t>(opacity) << 24U;
}
}

void SetupBattleQteWidgets() {
    const std::uint32_t view = g_uiElements[static_cast<std::size_t>(UIElement::GameQte)].config.uiHandleIndex;
    defaultIconSheet = UI_FindViewIconWidget(view, "DefaultIconSheet", 20);
    platformIconSheet = UI_FindViewIconWidget(view, "PlatformIconSheet", 21);
    mouseIconSheet = UI_FindViewIconWidget(view, "MouseIconSheet", 20);
    qteDefaultPanel = UI_GetWindow(UI_FindViewWindow(view, "QTE_DefaultPanel"));
    qteInitialPanelX[0] = qteDefaultPanel->layout.offsetX;
    qtePlatformPanel = UI_GetWindow(UI_FindViewWindow(view, "QTE_PlatformPanel"));
    qteInitialPanelX[2] = qtePlatformPanel->layout.offsetX;
    qteMousePanel = UI_GetWindow(UI_FindViewWindow(view, "QTE_MousePanel"));
    qteInitialPanelX[1] = qteMousePanel->layout.offsetX;
    char id[64];
    for (std::size_t i = 0; i < qteMultiPanels.size(); ++i) {
        std::snprintf(id, sizeof(id), "QTE_MultiPanel_%s", battleQteAxes[i]);
        qteMultiPanels[i] = UI_GetWindow(UI_FindViewWindow(view, id));
    }
    for (std::size_t i = 0; i < qteCommandPanels.size(); ++i) {
        if (i < 2)
            std::snprintf(id, sizeof(id), "QTE_ParentCommandPanel%s", battleQteButtons[i]);
        else {
            const char *const prefix = i >= 10 ? "" : i >= 6 ? "Default_" : "Mouse_";
            std::snprintf(id, sizeof(id), "Command_%s%s", prefix, battleQteButtons[i]);
            qteCommandObjects[i - 2] = UI_FindViewIconWidget(view, id, i >= 10 ? 21 : 20);
            if (qteCommandObjects[i - 2])
                qteCommandObjects[i - 2]->colour = 0x66ffffffU;
            std::snprintf(id, sizeof(id), "QTE_CommandPanel_%s%s", prefix, battleQteButtons[i]);
        }
        qteCommandPanels[i] = UI_GetWindow(UI_FindViewWindow(view, id));
    }
    qteTransition = UIAnimAllocSlot();
    qteSingleSelection = -1;
    qteMode = -1;
    for (d3t::UIWindow *panel : qteMultiPanels)
        if (panel)
            panel->alpha = 0.0f;
    for (d3t::UIWindow *panel : qteCommandPanels)
        if (panel)
            panel->alpha = 0.0f;
    qteMode = 0;
}

std::int32_t GetBattleQteUIMode() noexcept {
    return qteMode;
}

// @verified - p
void SetBattleQteUIMode(std::int32_t mode) {
    if (qteMode != mode) {
        qteSingleSelection = -1;
        if (mode == 1 || mode == 2) {
            if (qteDefaultPanel)
                qteDefaultPanel->alpha = 0.0f;
            if (qteMousePanel)
                qteMousePanel->alpha = 0.0f;
            if (qtePlatformPanel)
                qtePlatformPanel->alpha = 0.0f;
        }
        if (mode == 0 || mode == 2)
            for (d3t::UIWindow *panel : qteMultiPanels)
                if (panel)
                    panel->alpha = 0.0f;
        if (mode == 0 || mode == 1) {
            for (d3t::UIWindow *panel : qteCommandPanels)
                if (panel)
                    panel->alpha = 0.0f;
        } else if (mode == 2) {
            for (d3t::UIWindow *panel : qteCommandPanels)
                if (panel)
                    panel->alpha = 1.0f;
        }
    }
    qteMode = mode;
}

// @verified - p
void RefreshBattleQteUIInputDevice() {
    if (qteMode != 2)
        return;
    for (std::size_t i = 2; i < 6; ++i) {
        const bool keyboard = GetIsKeyboardActive() == 1;
        d3t::UIWindow *shown = qteCommandPanels[keyboard ? i : i + 4];
        d3t::UIWindow *hidden = qteCommandPanels[keyboard ? i + 4 : i];
        if (shown)
            shown->alpha = 1.0f;
        if (hidden)
            hidden->alpha = 0.0f;
    }
}

void SetBattleQteDirectionIcon(std::int32_t index, float scale, float alpha) {
    const std::uint32_t group = GetIsKeyboardActive() == 1 ? 2U : 6U;
    const std::uint32_t panel = static_cast<std::uint32_t>(index) + group;
    if (panel - 1U <= 8U)
        SetPanelIcon(static_cast<std::int32_t>(panel), scale, alpha);
}

void SetBattleQteActionIcon(std::int32_t index, float scale, float alpha) {
    if (index > 0 && static_cast<std::uint32_t>(index - 1) <= 3U)
        SetPanelIcon(index + 9, scale, alpha);
}

// @verified - p
void SetupBattleQteIcon(std::int32_t selection, std::uint8_t flags) {
    if (g_uiElements[static_cast<std::size_t>(UIElement::GameQte)].config.init != 3)
        return;
    SetBattleQteUIMode((flags & 0x20U) != 0);
    if (qteMode == 1) {
        for (std::size_t i = 0; i < qteMultiPanels.size(); ++i)
            if (qteMultiPanels[i] && (static_cast<std::uint32_t>(selection) & (1U << i)))
                qteMultiPanels[i]->alpha = 1.0f;
        return;
    }
    qteSingleSelection = static_cast<std::int16_t>(selection);
    const std::uint32_t mask = static_cast<std::uint32_t>(selection);
    if (platformIconSheet) {
        std::int32_t action = -1;
        switch (mask) {
        case 0x100:
            action = 18;
            break;
        case 0x200:
            action = 19;
            break;
        case 0x400:
            action = 20;
            break;
        case 0x800:
            action = 21;
            break;
        default:
            break;
        }
        if (qteDefaultPanel)
            qteDefaultPanel->alpha = 0.0f;
        if (qteMousePanel)
            qteMousePanel->alpha = 0.0f;
        if (qtePlatformPanel)
            qtePlatformPanel->alpha = 0.0f;
        if (action >= 0) {
            if (qtePlatformPanel)
                qtePlatformPanel->alpha = 1.0f;
            platformIconSheet->action = action;
            return;
        }
        if (qteDefaultPanel)
            qteDefaultPanel->alpha = 1.0f;
    }
    if (!defaultIconSheet)
        return;
    for (std::size_t i = 0; i < battleQteSingleMasks.size(); ++i) {
        if (mask != battleQteSingleMasks[i])
            continue;
        if (qteDefaultPanel)
            qteDefaultPanel->alpha = 0.0f;
        if (qteMousePanel)
            qteMousePanel->alpha = 0.0f;
        if (qtePlatformPanel)
            qtePlatformPanel->alpha = 0.0f;
        if (qteDefaultPanel)
            qteDefaultPanel->alpha = 1.0f;
        defaultIconSheet->sprite = battleQteSingleFrames[i];
        return;
    }
}

void BeginBattleQteDeparture() {
    if (!IsUITransitionActive(qteTransition))
        UIAnim_Start(qteTransition, qteInitialPanelX[0], -0.6f, 0.25f, 0.0f);
}

bool IsBattleQteTransitionActive() {
    return IsUITransitionActive(qteTransition);
}

void ResetBattleQteTransition() {
    qteDefaultPanel->layout.offsetX = qteInitialPanelX[0];
    qteMousePanel->layout.offsetX = qteInitialPanelX[1];
    qtePlatformPanel->layout.offsetX = qteInitialPanelX[2];
    RestartUITransition(qteTransition);
}

void RenderBattleQteWidgets() {
    RefreshNotebook();
    SetUIElementIndex(static_cast<std::int32_t>(UIElement::GameQte));
    if (IsUITransitionActive(qteTransition)) {
        qteDefaultPanel->layout.offsetX = GetUITransitionValue(qteTransition);
        qteMousePanel->layout.offsetX = GetUITransitionValue(qteTransition);
        qtePlatformPanel->layout.offsetX = GetUITransitionValue(qteTransition);
    }
    uiUpdateWithColour(*GetNativeUiContext(), g_uiElements[static_cast<std::size_t>(UIElement::GameQte)].config.uiHandleIndex);
}

} // namespace shenmue::s2
