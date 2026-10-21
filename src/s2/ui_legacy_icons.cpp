#include "s2/ui.hpp"
#include "s2/easy_option.hpp"
#include "s2/opening_staff_roll.hpp"
#include "s2/magic_room.hpp"
#include "s2/ctrl_speak_icon.hpp"
#include "s2/now_qte.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/battle_qte_ui.hpp"
#include "s2/ui_transition.hpp"
#include "s2/ui_scene.hpp"
#include "s2/ui_grid.hpp"
#include "s2/ui_collection.hpp"
#include "s2/ui_moves.hpp"
#include "s2/ui_loadsave.hpp"
#include "s2/ui_prompt.hpp"
#include "s2/input_bindings.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/mt7.hpp"
#include "s2/world_render.hpp"
#include "s2/areas/area_0001.hpp"
#include "s2/areas/area_armw.hpp"
#include "s2/areas/game_arcade.hpp"
#include "s2/areas/omak_tables.hpp"
#include "s2/omak_collection_ui.hpp"

#include "d3t/content.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/texture.hpp"
#include "d3t/threads.hpp"
#include "d3t/ui_definition.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_view.hpp"
#include "d3t/ui_runtime.hpp"
#include "d3t/dx11_renderer.hpp"
#include "platform/performance_counter.hpp"
#include "s2/input.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/item_wf.hpp"
#include "s2/item_selector.hpp"
#include "s2/main_icon.hpp"
#include "s2/flag.hpp"
#include "s2/item_selector_actions.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/math.hpp"
#include "s2/screen_film.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/save.hpp"
#include "s2/snapshot_viewer.hpp"
#include "s2/snapshot.hpp"
#include "s2/conversation.hpp"
#include "s2/system_modules.hpp"
#include "s2/title.hpp"
#include "s2/scene_data.hpp"
#include "s2/font.hpp"
#include "s2/archive_data.hpp"
#include "s2/pak.hpp"
#include "s2/directory_cache.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/event_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/inventory.hpp"
#include "s2/date_time.hpp"
#include "s2/gauge.hpp"
#include "s2/npc_package.hpp"
#include "s2/sound_program.hpp"
#include "s2/navigation.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/author.hpp"
#include "s2/help.hpp"
#include "s2/pause.hpp"
#include "s2/memo.hpp"
#include "s2/search.hpp"
#include "s2/story_state.hpp"
#include "s2/weather.hpp"
#include "s2/display_configuration.hpp"
#include "s2/audio_data.hpp"
#include "s2/hand.hpp"
#include "s2/full_map_menu.hpp"
#include "s2/legacy_collection.hpp"
#include "s2/pawn.hpp"
#include "s2/game_runtime.hpp"
#include <json/json.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <charconv>
#include <cctype>
#include <cmath>
#include <limits>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <stdexcept>
#include <vector>
#include "s2/area_names.hpp"

#include "s2/ui_internal.hpp"

    namespace shenmue::s2 {
using namespace ui_detail;

namespace ui_detail {

void StartHelpBlinkTransition() {
    if (helpWidgetBlinkTransition == -1)
        helpWidgetBlinkTransition = UIAnimAllocSlot();
    StartUIIntegerTransition(helpWidgetBlinkTransition, 0, 2, 1.0f, 0.0f);
    SetUITransitionLoopMode(helpWidgetBlinkTransition, 1);
}

constexpr std::uint32_t HelpColour(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
    return static_cast<std::uint32_t>(a) << 24U | static_cast<std::uint32_t>(b) << 16U | static_cast<std::uint32_t>(g) << 8U | r;
}

// helpIconCodeToAction.
std::int32_t HelpIconInputType() {
    switch (helpGetCurrentHelp()) {
    case 0x04:
    case 0x1b:
        return 5;
    case 0x12:
    case 0x13:
    case 0x18:
    case 0x33:
    case 0x53:
        return 1;
    case 0x23:
        return 4;
    default:
        return 2;
    }
}

void HelpWidgetMakeButtonIcon(UIButtonIconWidget &icon, std::int32_t code) {
    icon = {};
    icon.inUse = 1;
    icon.action = helpWidgetMode == 1 ? 3 : UI_GetActionForId(code);
    std::sprintf(icon.id.data(), "UIHelp::PlatformButtonIcon_ID:%i_Action:%i", code, icon.action);
    std::sprintf(icon.image.data(), "%s", g_platformButtonIconImages[0]->name);
    icon.field88 = 1;
    icon.iconFontSize = 4;
    icon.allowDoubleWidth = 1;
    icon.controlMethod = -1;
    icon.controlState = HelpIconInputType();
    icon.fontColour = HelpColour(0, 0, 0, 255);
    icon.alignment = 18;
    icon.sheet = g_platformIconsSheet;
    icon.colour = HelpColour(255, 255, 255, 255);
    icon.window = UI_FindViewWindow(g_uiElements[static_cast<std::size_t>(UIElement::GameHelp)].config.uiHandleIndex, "Help_MainPanel");
}

void DrawHelpBox(nk_context &context, const d3t::UIRect &rect, std::uint32_t colour, std::uint8_t alpha) {
    const float x = rect.x + (context.current ? context.current->bounds.x : 0.0f);
    const float y = rect.y + (context.current ? context.current->bounds.y : 0.0f);
    const struct nk_rect bounds = nk_rect(x, y, rect.width, rect.height);
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(&context, bounds);
    nk_color stroke{};
    std::memcpy(&stroke, &colour, sizeof(stroke));
    nk_stroke_rect(&context.current->buffer, bounds, 0.0f, 1.0f, stroke);
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(&context, bounds);
    nk_fill_rect(&context.current->buffer, bounds, 0.0f, nk_rgba(0, 0, 0, alpha));
}

void DrawHelpLabel(nk_context &context, const d3t::UIRect &rect, const char *text, nk_flags alignment) {
    const struct nk_rect bounds = nk_rect(rect.x, rect.y, rect.width, rect.height);
    if (g_waitingForNextTick == 1)
        nk_layout_space_push(&context, bounds);
    if (g_uiFontFace != 1) {
        g_uiFontFace = 1;
        UI_SelectFont();
    }
    context.style.text.color = nk_rgba(0, 0, 0, 255);
    if (g_waitingForNextTick == 1)
        nk_label(&context, text, alignment);
    g_uiFontFace = 0;
    UI_SelectFont();
    context.style.text.color = nk_rgba(255, 255, 255, 255);
    if (g_waitingForNextTick == 1) {
        nk_layout_space_push(&context, bounds);
        nk_label(&context, text, alignment);
    }
}

// helpWidgetResolveLocation: the location label of the current map, SCEN region or author.
void HelpWidgetResolveLocation() {
    const char *label = GetLocationLabelId(g_currentMap.name);
    const char *const regionLabel = GetLocationLabelId(GetScenarioEventObjectMapNames()[0].name);
    const char *const authorLabel = GetLocationLabelId(GetCurrentAuthorId().name);
    std::string_view location = label ? GetLocalizedSceneText(label) : std::string_view{};
    const std::string_view region = regionLabel ? GetLocalizedSceneText(regionLabel) : std::string_view{};
    const std::string_view author = authorLabel ? GetLocalizedSceneText(authorLabel) : std::string_view{};
    if (!author.empty())
        location = author;
    if (!location.empty()) {
        if (!region.empty() && reinterpret_cast<std::uintptr_t>(region.data()) < reinterpret_cast<std::uintptr_t>(location.data()))
            location = region;
    } else {
        location = region;
    }
    std::sprintf(helpWidgetLocationText.data(), "%.*s", static_cast<int>(location.size()), location.data() ? location.data() : "");
}

// helpWidgetLayout.
void HelpWidgetLayout(const d3t::UIRect &area, float lineHeight) {
    helpWidgetPauseRect = {area.width * 0.725f + area.x, area.height + area.y - lineHeight * 0.5f, area.width * 0.25f, lineHeight};
    helpWidgetLocationRect = {area.width * 0.0099999998f + area.x, area.height + area.y - lineHeight, area.width * 0.5f, lineHeight};
    const std::string_view pause = GetLocalizedSceneText("UIHELPPAUS"); // @text EN: "Game Paused" | JA: "ポーズ中"
    std::sprintf(helpWidgetPauseText.data(), "%.*s", static_cast<int>(pause.size()), pause.data());
    HelpWidgetResolveLocation();
    const nk_user_font &font = *GNuklearWindow.style.font;
    std::array<float, 45> cells{};
    for (std::int32_t index = 0; index < helpWidgetRunCount; ++index) {
        HelpWidgetRun &run = helpWidgetRuns[static_cast<std::size_t>(index)];
        const std::int32_t cell = run.line + 15 * run.column;
        if (cell >= 45)
            break;
        const float offset = run.column == 1 ? area.width * 0.16f : run.column == 2 ? area.width * 0.50999999f : 0.0f;
        float &cursor = cells[static_cast<std::size_t>(cell)];
        run.rect.height = lineHeight;
        run.rect.x = cursor + offset;
        run.rect.y = static_cast<float>(run.line) * lineHeight;
        if (run.kind == 1) {
            run.rect.width = font.width(font.userdata, font.height, run.text.data(), run.length);
        } else {
            const UIButtonIconWidget &icon = helpWidgetIcons[static_cast<std::size_t>(run.icon)];
            const d3t::UIViewImage *const image = g_platformButtonIconImages[icon.field88 == 1 ? 0 : 2];
            const std::int32_t glyph = ResolveButtonIconGlyph(icon.action, icon.controlMethod, icon.controlState, icon.allowDoubleWidth);
            const struct nk_image sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(icon.sheet), glyph, image);
            UI_ScaleSize(
                run.rect.width, run.rect.height, 2, 1000.0f, lineHeight, static_cast<float>(sprite.region[2]), static_cast<float>(sprite.region[3]), nullptr);
            run.rect.x += 3.0f;
            cursor += 6.0f;
        }
        cursor += run.rect.width;
    }
    for (std::int32_t index = 0; index < helpWidgetRunCount; ++index) {
        HelpWidgetRun &run = helpWidgetRuns[static_cast<std::size_t>(index)];
        if (run.column == 0 && run.line >= 0 && run.line < 45)
            run.rect.x += (area.width - cells[static_cast<std::size_t>(run.line)]) * 0.5f;
        run.rect.x += area.x;
        run.rect.y += area.y + 80.0f;
    }
    for (std::int32_t index = 0; index < helpWidgetBoxCount && index < static_cast<std::int32_t>(helpWidgetBoxes.size()); ++index) {
        HelpWidgetBox &box = helpWidgetBoxes[static_cast<std::size_t>(index)];
        if (box.widthMode == 0) {
            box.rect.x = area.width * 0.15000001f + area.x;
            box.rect.width = area.width * 0.69999999f;
        } else if (box.widthMode == 2) {
            box.rect.x = area.width * 0.15000001f + area.x;
            box.rect.width = area.width * 0.64999998f;
        } else {
            box.rect.x = (box.column ? 0.505f : 0.15000001f) * area.width + area.x;
            box.rect.width = area.width * 0.345f;
        }
        box.rect.y = static_cast<float>(box.row) * lineHeight + (area.y + 80.0f);
        box.rect.height = static_cast<float>(box.rows) * lineHeight;
    }
}

std::int64_t HelpWidgetRender() {
    if (helpWidgetPeekHidden)
        return 0;
    helpWidgetPanels[0]->rectCached = 0;
    helpWidgetPanels[1]->rectCached = 0;
    nk_context &context = *GetNativeUiContext();
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::GameHelp)];
    if (helpWidgetMode == 0)
        DrawUIEntity(context, element.config.uiHandleIndex, element.index);
    UILayoutRect panel{};
    UI_GetWindowRect(helpWidgetPanels[1], panel);
    if (helpWidgetMode == 1)
        panel.y += panel.height * 0.5f;
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(&context, &helpWidgetPanel, "UIHelpBack", "UIHelpBack", nk_rect(panel.x, panel.y, panel.width, panel.height), NK_WINDOW_NO_SCROLLBAR);
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 2);
    }
    const d3t::UIRect area{50.0f, 20.0f, panel.width - 100.0f, panel.height - 50.0f};
    const float lineHeight = area.height / 15.0f;
    if (g_uiFontFace != 0) {
        g_uiFontFace = 0;
        UI_SelectFont();
    }
    if (g_uiFontSize != 3) {
        g_uiFontSize = 3;
        UI_SelectFont();
    }
    const std::int32_t keyboard = static_cast<std::int32_t>(GetIsKeyboardActive());
    if (helpWidgetRelayout == 1 || helpWidgetKeyboardType != keyboard) {
        helpWidgetRelayout = 0;
        helpWidgetKeyboardType = keyboard;
        HelpWidgetLayout(area, lineHeight);
    }
    if (helpWidgetMode == 0) {
        DrawHelpBox(context, area, HelpColour(255, 255, 255, 255), 216);
        for (std::int32_t index = 0; index < helpWidgetBoxCount; ++index) {
            const HelpWidgetBox &box = helpWidgetBoxes[static_cast<std::size_t>(index)];
            DrawHelpBox(context, box.rect, box.colour, 255);
        }
    }
    for (std::int32_t index = 0; index < helpWidgetRunCount; ++index) {
        const HelpWidgetRun &run = helpWidgetRuns[static_cast<std::size_t>(index)];
        if (run.kind == 1) {
            DrawHelpLabel(context, run.rect, run.text.data(), NK_TEXT_LEFT);
        } else {
            const UILayoutRect rect{.x = run.rect.x, .y = run.rect.y, .width = run.rect.width, .height = run.rect.height};
            if (g_waitingForNextTick == 1)
                nk_layout_space_push(&context, nk_rect(rect.x, rect.y, rect.width, rect.height));
            UI_DrawButtonIcon(context, helpWidgetIcons[static_cast<std::size_t>(run.icon)], helpWidgetMode == 1 ? &rect : nullptr);
        }
    }
    if (helpWidgetMode == 0) {
        DrawHelpBox(context, helpWidgetPauseRect, HelpColour(255, 255, 255, 255), 255);
        if (static_cast<std::uint16_t>(std::bit_cast<std::int32_t>(GetUITransitionValue(helpWidgetBlinkTransition))) == 1)
            DrawHelpLabel(context, helpWidgetPauseRect, helpWidgetPauseText.data(), NK_TEXT_CENTERED);
        DrawHelpLabel(context, helpWidgetLocationRect, helpWidgetLocationText.data(), NK_TEXT_LEFT);
    }
    if (g_waitingForNextTick == 1) {
        nk_layout_space_end(&context);
        nk_end(&context);
    }
    return 0;
}

std::intptr_t DrawHelpFrame() {
    HelpWidgetRender();
    Nuklear_BindRenderTarget();
    NuklearUpdateForAspectRatio(GetAspectRatioModeShouldScale());
    CopyNuklearTexToBackBuffer();
    return 0;
}

void RenderHelpFrame() {
    if (!drawingUI)
        return;
#if defined(_WIN32)
    ExecuteCurrentCommandList();
    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(1);
    const NuklearFrameCallback previous = g_nuklearFrameCallback;
    g_nuklearFrameCallback = DrawHelpFrame;
    InvokeNuklearFrame(d3t::DX11::g_ppD3DDeviceContext);
    FinishNuklearFrame();
    g_nuklearFrameCallback = previous;
    d3t::DX11::g_ppD3DDeviceContext = d3t::DXGetPipelineContext(0);
    d3t::BindUIFrameRenderTarget();
#endif
}

std::uint32_t LoadHelpDefinition() {
    return ReadUIDefinitionFile(UIElement::GameHelp, "/ui/gamehelp/Help_New.ui", 0);
}

std::uint32_t LoadPlatformIcons() {
    return ReadUIDefinitionFile(UIElement::PlatformIcons, "/ui/PlatformIcons.ui", 1);
}

// helpWidgetIsPressed.
std::int64_t HelpWidgetInput() {
    if (g_inputBuffer[0].pressedPriority & 0x200U)
        DisableHelpIcon();
    helpWidgetPeekHidden = false;
    if ((g_inputBuffer[0].held & 4U) && (g_inputBuffer[0].held & 2U))
        helpWidgetPeekHidden = g_inputBuffer[0].connectionState != 0;
    return 0;
}

std::int64_t HelpWidgetBind() {
    for (std::size_t index = 0; index < helpWidgetPanels.size(); ++index)
        helpWidgetPanels[index] = UI_FindWindowById(HELP_PANEL_NAMES[index]);
    helpWidgetBlinkTransition = UIAnimAllocSlot();
    return 1;
}

void HelpWidgetShow() {
    helpWidgetMode = 0;
    helpWidgetArcadeContext = 0;
    auto &watch = g_uiElements[static_cast<std::size_t>(UIElement::WristwatchOverlay)];
    if (!watch.isRenderable) {
        watch.isRenderable = 1;
        if (watch.resetRender)
            watch.resetRender();
    }
    StartHelpBlinkTransition();
    for (const auto transition : nativeHudTransitions)
        if (transition != -1)
            RestartUITransition(transition);
    if (nativeHudBlink != -1)
        RestartUITransition(nativeHudBlink);
    for (auto &hint : nativeHudRequestedHints)
        hint = (hint & 0xffff0000U) | 0xffffU;
    for (auto &hint : nativeHudShownHints)
        hint = (hint & 0xffff0000U) | 0xffffU;
}

// @verified - p: HelpWidgetDestroy.
std::int64_t HelpWidgetDestroy() {
    helpWidgetMode = 0;
    helpWidgetArcadeContext = 0;
    ClearWindowInput();
    ResetInputHandler();
    DisableElement(UIElement::WristwatchOverlay);
    ClearMenuInputHints();
    return 1;
}

// @verified - p
std::int64_t HelpWidgetHints() {
    const std::array<std::int16_t, 4> hints{static_cast<std::int16_t>(helpWidgetMode == 1 && helpWidgetArcadeContext == 1 ? 45 : -1), 44, -1,
        static_cast<std::int16_t>(helpWidgetMode == 1 ? 3845 : -1)};
    return CopyHudHintIds(hints);
}

// @verified - p
std::uint8_t ConfigureHelpDirectionBlink(std::uint32_t visible) {
    std::size_t count = 0, missing = 0;
    for (std::size_t index = 0; index < 4; ++index) {
        if ((visible & (1U << (12U + index))) != 0)
            ++count;
        else
            missing = HELP_DIRECTION_MISSING_ORDER[index];
    }
    if (!count)
        return 0;
    if (count == 3) {
        for (std::size_t index = 0; index < 4; ++index) {
            HelpDirectionBlink &blink = helpDirectionBlinks[HELP_DIRECTION_RING_ORDER[(index + missing) % 4]];
            blink.onFrames = HELP_DIRECTION_ON_FRAMES[8 + index];
            blink.offFrames = blink.onFrames;
            blink.remaining = HELP_DIRECTION_PHASES[8 + index];
            blink.mode = index == 1 ? 2 : (blink.remaining != 0);
        }
        return static_cast<std::uint8_t>(HELP_DIRECTION_PHASES[11] != 0);
    }
    std::size_t selected = 0;
    std::uint8_t result = 0;
    for (std::size_t index = 0; index < 4; ++index) {
        HelpDirectionBlink &blink = helpDirectionBlinks[index];
        blink.mode = 0;
        result = static_cast<std::uint8_t>(1U << (12U + index));
        if ((visible & (1U << (12U + index))) == 0)
            continue;
        const std::size_t offset = (count - 1) * 4 + selected++;
        blink.onFrames = HELP_DIRECTION_ON_FRAMES[offset];
        blink.offFrames = blink.onFrames;
        blink.remaining = HELP_DIRECTION_PHASES[offset];
        result = static_cast<std::uint8_t>(blink.remaining);
        blink.mode = 1;
    }
    return result;
}


std::int32_t NativeHelpHint(std::uint32_t button) {
    switch (GetMediatedButtonOwner(button).name) {
    case SHEN_NAME(BTDR):
    case SHEN_NAME(BTD2):
        return 0;
    case SHEN_NAME(BTFT):
        return 1;
    case SHEN_NAME(BTMT):
        return 2;
    case SHEN_NAME(BTLD):
        return 3;
    case SHEN_NAME(BTEV):
        return 4;
    case SHEN_NAME(BTME):
        return 5;
    case SHEN_NAME(BTIL):
        return IsAnyItemSelectActive() ? 5 : 9;
    case SHEN_NAME(BTCC):
        return 6;
    case SHEN_NAME(BTBT):
        return 7;
    case SHEN_NAME(BTSW):
        return 8;
    case SHEN_NAME(BTSP):
        return 10;
    case SHEN_NAME(BTAD):
        return 11;
    case SHEN_NAME(BTKK):
        return 12;
    case SHEN_NAME(BTTT):
        return 13;
    case SHEN_NAME(BTPP):
        return 14;
    case SHEN_NAME(BTDD):
        return 15;
    case SHEN_NAME(BTAA):
        return 16;
    case SHEN_NAME(BTIF):
        return 17;
    case SHEN_NAME(BTNV):
        return 18;
    case SHEN_NAME(BTMK):
        return button == 0 ? 21 : button == 2 ? 19 : button == 3 ? 20 : 6;
    case SHEN_NAME(BTBB):
    case SHEN_NAME(BTBA):
    case SHEN_NAME(BTBX):
    case SHEN_NAME(BTBY):
        return 22;
    case SHEN_NAME(BTSD):
        return 23;
    case SHEN_NAME(BTMR):
        return 24;
    case SHEN_NAME(BTMP):
        return 25;
    case SHEN_NAME(BTMM):
        return 26;
    case SHEN_NAME(BTBF):
        return button == 0 ? 12 : button == 1 ? 13 : button == 2 ? 14 : 15;
    default:
        return -1;
    }
}

void PrepareNativeHudHelp() {
    const bool persistentOnly = GetShouldRenderIcons(RenderIconSlot::ItemSelect) || IsItemSelectable();
    helpIconDrawVisible = helpIconState.persistentVisible | (persistentOnly ? 0 : helpIconState.frameOverlayVisible);
    helpIconDrawSelected = helpIconState.persistentSelected | (persistentOnly ? 0 : helpIconState.frameOverlaySelected);
    const MapControlData *const map = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const bool author = authGetLoaded() != 0, film = !IsScreenFilmNotShowing(map->screenFilmTask);
    const bool searchable = plymIsSearchable(), parent = GetParentMenu() != 0;
    if (author || film || (!searchable && parent))
        helpIconHudDelay = 5;
    else
        helpIconHudDelay = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(helpIconHudDelay) - 1U);
    helpIconHintAngle += static_cast<std::uint32_t>(TruncateSpriteCoordinate(shenGetScaledDeltaTickTime(1638.0f)));
    for (std::uint32_t &angle : helpIconHintAngles)
        angle = helpIconHintAngle;
    qteForcedHelp = (helpIconDrawVisible & 0x100000U) != 0;
    bool forceAdmission = qteForcedHelp != 0;
    for (std::int16_t &hint : helpIconHintIds)
        hint = -1;
    if ((GetSleepActionIconFlags() & 3U) == 1 || (!IsCtrlSpeakIconInactive() && IsSpeakIconCancellable())) {
        helpIconDrawVisible |= 2U;
        helpIconDrawSelected &= ~2U;
        qteForcedHelp = 1;
        forceAdmission = true;
    }
    std::array<std::int32_t, 4> hints{};
    for (std::size_t index = 0; index < hints.size(); ++index) {
        hints[index] = NativeHelpHint(static_cast<std::uint32_t>(index));
        if (hints[index] >= 0)
            continue;
        if (index == 1)
            hints[index] = 6;
        else if ((index == 0 || index == 2) && (helpIconDrawVisible & HELP_BUTTON_MASKS[index]))
            hints[index] = 27;
        if (iconSelectFlags & HELP_SELECTED_BUTTON_MASKS[index]) {
            if (index != 1)
                hints[index] = 22;
            helpIconDrawVisible |= HELP_BUTTON_MASKS[index];
        }
    }
    if (helpIconHudDelay < 0 && searchable && !IsScenarioBusy() && !(GetSleepActionIconFlags() & 1U) && IsCtrlSpeakIconInactive()) {
        if (!(helpIconDrawVisible & 4U) && !srchCheckFlags(6) && IsWristwatchOpenable()) {
            helpIconDrawVisible |= 4U;
            helpIconDrawSelected &= ~4U;
        }
        if (!(helpIconDrawVisible & 2U) && (srchCheckFlags(3) > 0 || srchCheckFlags(6))) {
            helpIconDrawVisible |= 2U;
            helpIconDrawSelected &= ~2U;
        }
    }
    if (helpIconHudDelay < 0 || forceAdmission || (!author && !film && !searchable && parent)) {
        for (std::size_t index = 0; index < hints.size(); ++index) {
            if (!(helpIconDrawVisible & HELP_BUTTON_MASKS[index])) {
                helpIconHintAngles[index] = 0;
                continue;
            }
            if (hints[index] < 0 || (index == 2 && helpIconHudDelay < 0 && IsItemSelectable()))
                continue;
            helpIconHintIds[index] = HELP_HINT_ICONS[static_cast<std::size_t>(hints[index])];
            if (helpIconHudDelay >= 0)
                forceAdmission = true;
        }
    }
    if (hints[0] == 1 && scenarioTalkPromptResumed)
        helpIconHintAngles[0] = 0;
    if (hints[2] == 5 && GetScenarioFlagNotificationShown() == 0)
        helpIconHintAngles[2] = 0;
    if (hints[3] == 2) {
        if (scenarioFollowPromptShown)
            helpIconHintAngles[3] = 0;
    } else if (hints[3] == 8 && helpIconItemEffectState == 0)
        helpIconHintAngles[3] = 0;
    for (std::size_t index = 0; index < hints.size(); ++index)
        if (helpIconHintIds[index] < 0)
            helpIconDrawVisible &= ~HELP_BUTTON_MASKS[index];
    helpIconHintSelected = {};
    for (std::size_t bit = 0; bit < HELP_HINT_BLINK_SLOTS.size(); ++bit) {
        const std::int32_t slot = HELP_HINT_BLINK_SLOTS[bit];
        if (slot != -1 && ((helpIconDrawVisible & helpIconDrawSelected) & (1U << bit)))
            helpIconHintSelected[static_cast<std::size_t>(slot)] = 1;
    }
    if (forceAdmission)
        helpIconHudDelay = -1;
    helpIconDrawVisible &= ~0x200U;
    helpIconPublishedVisible = helpIconDrawVisible;
    std::array<std::uint32_t, 5> output{};
    for (std::size_t index = 0; index < hints.size(); ++index)
        output[index] = static_cast<std::uint16_t>(helpIconHintIds[index]) | (static_cast<std::uint32_t>(helpIconHintSelected[index]) << 16U);
    if (!g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)].isVisible && !g_uiElements[static_cast<std::size_t>(UIElement::ActionSelector)].isVisible)
        PublishHudHints(output.data(), false);
}

void DoQteDraw(void *) {
    if (nowQteDrawSuppression == 1)
        return;
    helpIconRefreshDelay = (g_loadingFlag & 2U) ? 3 : std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(helpIconRefreshDelay) - 1U);
    const float cardinalX = IsFightHealthUIActive() ? 160.0f : 52.0f;
    const MapControlData *const map = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    std::uint32_t blocked = IsSaveAllowed() != 0;
    if (!IsScreenFilmNotShowing(map->screenFilmTask) || GetIsLoadScreenActive())
        blocked |= 2U;
    if (authGetLoaded() && !qteForcedHelp)
        blocked |= 4U;
    if (IsWazamakiInactive())
        blocked |= 0x10U;
    if (blocked)
        helpIconDrawVisible = 0;
    if (helpIconHudDelay < 0 && helpIconDrawVisible && GetDebugForceUIEnabled(UIElement::GameHud)) {
        ConstructHudCameraState(qteControlMask, nativeHudSpriteWidgets[5]);
        EnableElement(UIElement::GameHud);
    }
    LegacyIconSprite cardinal{.y = 445.0f, .icon = 2048, .width = 38, .height = 38};
    float offset = 0.0f;
    for (std::size_t index = 0; index < 4; ++index) {
        const std::uint32_t bit = 0x10000U << index;
        cardinal.rotation = static_cast<std::uint16_t>(TruncateSpriteCoordinate(static_cast<float>(HELP_CARDINAL_ROTATIONS[index]) * 8192.0f / 45.0f));
        if (!(helpIconDrawVisible & bit) || ((helpIconDrawSelected & bit) && !helpIconBlinkVisible))
            continue;
        cardinal.x = cardinalX + offset;
        PrepareDrawWithMode(cardinal, ScreenMode::AdjustedWithOffset);
        offset += 48.0f;
    }
    if (qteControlState != 1)
        return;
    LegacyIconSprite camera{};
    InitializeLegacyIconSprite(camera);
    camera.depth = 1.1f;
    camera.icon = 2305;
    std::int32_t stripX = 510, stripY = 330;
    if (qteCameraAtTop == 1 || authGetLoaded() == 1) {
        camera.x = 600.0f;
        camera.y = 50.0f;
        stripX = 480;
        stripY = 50;
        ConstructHudCameraState(qteControlMask, hudSnapshotCameraState);
        EnableElement(UIElement::HudSnapshot);
    } else {
        camera.x = 580.0f;
        camera.y = 350.0f;
    }
    PrepareDrawWithMode(camera, ScreenMode::Stretched);
    if (qteControlMask == 1 || qteControlMask == 2) {
        camera.icon = qteControlMask == 1 ? 2306 : 2307;
        PrepareDrawWithMode(camera, ScreenMode::Stretched);
        qteDrawFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(qteDrawFrames) - 1U);
        if (qteDrawFrames <= 0)
            qteControlMask = 0;
    } else if (qteControlMask == 3) {
        LegacySpritePacket packet{.quad = {.flags = 1024}, .colour = 0xffffffffU, .uv = {0.0f, 0.0f, 1.0f, 1.0f}};
        AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, stripX, stripY, 256, 16, ScreenMode::Stretched);
        LegacySpriteNullsub(packet, nowQteTexture, camera.depth);
    }
}

void NQ_CtrlButtonIcon(void *) {
    if (helpIconItemPromptFrames >= 0 && (--helpIconItemPromptFrames < 0 || IsItemSelectable())) {
        helpIconItemPromptFrames = -1;
        CheckHelpIcon(0x200, 0x200, CheckHelpIconOp::Remove);
        SetHudItemPromptIcon(1);
    }
    if (helpIconBlinkFrames > 0) {
        helpIconBlinkVisible = 1;
        if (--helpIconBlinkFrames <= 0)
            helpIconBlinkFrames = -3;
    } else {
        helpIconBlinkVisible = 0;
        helpIconBlinkFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(helpIconBlinkFrames) + 1U);
        if (helpIconBlinkFrames >= 0)
            helpIconBlinkFrames = 5;
    }
    if (!GetParentMenu()) {
        helpIconItemEffectState = 0;
        helpIconItemEffectFrames = -1;
    } else if (helpIconItemEffectState <= 0) {
        helpIconItemEffectFrames = -1;
    } else {
        helpIconItemEffectFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(helpIconItemEffectFrames) - 1U);
        if (helpIconItemEffectFrames < 0) {
            helpIconItemEffectState = -1;
            helpIconItemEffectFrames = -1;
        }
    }
    if (helpIconDeferredRemoval && !helpIconBlinkVisible) {
        helpIconState.persistentVisible &= ~helpIconDeferredRemoval;
        helpIconState.persistentSelected &= ~helpIconDeferredRemoval;
        helpIconDeferredRemoval = 0;
    }
    for (HelpDirectionBlink &blink : helpDirectionBlinks) {
        if (blink.remaining > 0) {
            blink.visible = 1;
            if (--blink.remaining <= 0)
                blink.remaining = std::bit_cast<std::int32_t>(0U - static_cast<std::uint32_t>(blink.offFrames));
        } else {
            blink.visible = 0;
            blink.remaining = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(blink.remaining) + 1U);
            if (blink.remaining >= 0)
                blink.remaining = blink.onFrames;
        }
    }
    if (helpIconRefreshDelay < 0)
        PrepareNativeHudHelp();
    HLib::ExecuteCallbackNow(DoQteDraw, nullptr);
    currentIconDeltaTime += shenGetScaledDeltaTickTime(static_cast<float>(iconDeltaTime));
    if (currentIconDeltaTime <= 0.0f) {
        currentIconDeltaTime = 0.0f;
        iconDeltaTime = 7;
    } else if (currentIconDeltaTime >= 127.0f) {
        currentIconDeltaTime = 127.0f;
        iconDeltaTime = -7;
    }
    helpIconState.frameOverlayVisible = helpIconState.frameOverlaySelected = 0;
    iconSelectFlags = 0;
}

// @verified - p
std::int64_t ProcessSpeakSelection() {
    std::uint8_t input = GetDebugForceUIEnabled(UIElement::ActionSelector) ? static_cast<std::uint8_t>(ConsumeActionSelectorInput()) :
                                                                             DecodeSelectorInput(GetPadControlData(1) & 0xff3fU, 0x55);
    std::int64_t result = input;
    if (input == 0 || input == 2 || input == 4 || input == 6) {
        const std::int8_t slot = ACTION_SELECTOR_DIRECTION_TO_SLOT[input];
        const std::uint16_t choice = static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]);
        if (choice != 0xffffU) {
            ctrlSpeakSelectedChoiceSlot = static_cast<std::uint16_t>(slot);
            ctrlSpeakChoiceDrawSprites[slot].depth = 7.0f;
            ctrlSpeakRetainedMotion = 0.0f;
            ctrlSpeakIconState = 3;
            ctrlSpeakResultChoiceId = choice;
            ctrlSpeakSelectionAnimationFrame = -1;
            return 9 * slot;
        }
        input = 13;
    } else if (input == 9) {
        result = ctrlSpeakMode;
        if (IsSpeakIconCancellable()) {
            ctrlSpeakIconState = 3;
            ctrlSpeakResultChoiceId = 0xffffU;
            return result;
        }
    } else if (input == 11 && ctrlSpeakDedicatedCancelEnabled) {
        ctrlSpeakIconState = 3;
        ctrlSpeakResultChoiceId = 470;
        return 470;
    }
    if (ctrlSpeakIconState == 3)
        return result;
    if (input == 13) {
        ctrlSpeakElapsedFrames = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(ctrlSpeakElapsedFrames) + 1U));
        result = static_cast<std::uint16_t>(ctrlSpeakElapsedFrames);
        if (ctrlSpeakElapsedFrames >= ctrlSpeakTimeoutFrames) {
            ctrlSpeakIconState = 3;
            dialogueSelectionTimedOut = true;
            if (ctrlSpeakDefaultChoiceId == 0xffffU) {
                ctrlSpeakResultChoiceId = 0xffffU;
            } else {
                for (std::int8_t slot = 0; slot < 4; ++slot) {
                    result = slot;
                    if (static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]) == ctrlSpeakDefaultChoiceId) {
                        ctrlSpeakSelectedChoiceSlot = static_cast<std::uint16_t>(slot);
                        ctrlSpeakResultChoiceId = ctrlSpeakDefaultChoiceId;
                        ctrlSpeakChoiceDrawSprites[slot].depth = 7.0f;
                        return result;
                    }
                }
            }
            return result;
        }
    }
    if (ctrlSpeakDedicatedCancelEnabled)
        return RegisterButtonMediation(BUTTON_CTRL_SPEAK_CANCEL, nullptr, (ButtonMediationShowButtonHint | ButtonMediationBindB | 0x1U));
    return result;
}

// @verified - p
void CtrlSpeakIcon_Callback(void *) {
    ctrlSpeakPulseAngle = static_cast<std::uint16_t>(ctrlSpeakPulseAngle + 1638U);
    switch (ctrlSpeakIconState) {
    case 0:
        if (ctrlSpeakOpenRequested) {
            ctrlSpeakIconState = 1;
            PrepareSpeakTextSlots();
            padcRefreshCurrentInputs(true);
            iconToggleStateFor(2, true);
            ctrlSpeakResultChoiceId = 0xffff;
            ctrlSpeakSavedLoadingFlags = g_loadingFlag;
            if ((g_loadingFlag & 0x20U) != 0)
                FinishLoading();
            ctrlSpeakRadialScale = 0.0f;
            ctrlSpeakSelectorOrigin[0] = ctrlSpeakRightOriginX;
            ctrlSpeakRetainedMotion = 10.0f;
            ctrlSpeakSelectionAnimationFrame = -1;
            ShowElementByIndex(UIElement::ActionSelector);
        }
        break;
    case 1:
        ctrlSpeakIconState = 2;
        break;
    case 2:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ProcessSpeakSelection();
        break;
    case 3:
        if (ctrlSpeakSelectedChoiceSlot == 4) {
            ctrlSpeakIconState = 4;
            DestroyElementByIndex(UIElement::ActionSelector);
            DestroyElementByIndex(UIElement::GameMoves);
        }
        break;
    case 4:
        if (!(ActionSelectorPosition() > 0.0f)) {
            ctrlSpeakIconState = 0;
            padcRefreshCurrentInputs(false);
            ToggleRenderIcons(RenderIconSlot::ActionSelector, false);
            iconToggleStateFor(2, false);
            ReleaseSpeakTextSlots();
            ctrlSpeakOpenRequested = false;
            if ((ctrlSpeakSavedLoadingFlags & 0x20U) != 0) {
                g_loadingFlag = ctrlSpeakSavedLoadingFlags;
                ctrlSpeakSavedLoadingFlags = 0;
            }
        }
        break;
    case 5: {
        const float interpolation = 1.0f - ctrlSpeakResizeFrame / 5.0f;
        for (std::size_t slot = 0; slot < 4; ++slot) {
            ctrlSpeakChoiceWidths[slot] = ctrlSpeakChoiceWidthDeltas[slot] ?
                static_cast<std::int16_t>(static_cast<std::int32_t>(
                    static_cast<float>(ctrlSpeakTargetChoiceWidths[slot]) - static_cast<float>(ctrlSpeakChoiceWidthDeltas[slot]) * interpolation)) :
                ctrlSpeakTargetChoiceWidths[slot];
        }
        ctrlSpeakResizeFrame += 1.0f;
        if (ctrlSpeakResizeFrame > 5.0f) {
            ctrlSpeakChoiceWidths = ctrlSpeakTargetChoiceWidths;
            ctrlSpeakIconState = 2;
        }
        if (ctrlSpeakChoiceWidths[1] != ctrlSpeakChoiceWidths[3]) {
            if (ctrlSpeakChoiceWidths[1] <= ctrlSpeakChoiceWidths[3])
                ctrlSpeakChoiceWidths[1] = ctrlSpeakChoiceWidths[3];
            else
                ctrlSpeakChoiceWidths[3] = ctrlSpeakChoiceWidths[1];
        }
        const float expansion = static_cast<float>(ctrlSpeakChoiceWidths[1]) - 128.0f;
        ctrlSpeakHalfWidthExpansion = expansion * 0.5f;
        ctrlSpeakRightOriginX = expansion + 180.0f;
        ctrlSpeakSelectorOrigin[0] = expansion + 180.0f;
        ctrlSpeakLeftOriginX = -176.0f - expansion;
        break;
    }
    default:
        break;
    }
    if (GetItemSelectionMode(2)) {
        if (ctrlSpeakSelectedChoiceSlot != 4) {
            ctrlSpeakSelectionAnimationFrame = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(ctrlSpeakSelectionAnimationFrame) + 1U);
            if (ctrlSpeakSelectionAnimationFrame >= 20) {
                ctrlSpeakSelectionAnimationFrame = 0;
                ctrlSpeakSelectedChoiceSlot = 4;
            }
        }
        if (GetDebugForceUIEnabled(UIElement::ActionSelector)) {
            EnableElement(UIElement::ActionSelector);
            ExecuteElementCallback(UIElement::ActionSelector);
        } else {
            HLib::ExecuteCallbackNow(DrawCtrlSpeakSelector, nullptr);
        }
    }
}

// @verified - p
std::uint32_t ClearCtrlSpeakRasterizerResultNullsub(std::byte *) {
    return 0;
}

std::int32_t AcquireCtrlSpeakTextHandle(const char *) {
    for (std::size_t slot = 0; slot < ctrlSpeakTexturePoolHandles.size(); ++slot) {
        if (ctrlSpeakTexturePoolFlags[slot] & 1U)
            continue;
        GuiPtxHeader *const header = reinterpret_cast<GuiPtxHeader *>(guiPtxBacking);
        header->width = 256;
        header->height = 16;
        const std::uint32_t height = 16, width = 256;
        std::memcpy(guiPtxBacking + 60, &height, sizeof(height));
        std::memcpy(guiPtxBacking + 64, &width, sizeof(width));
        (void)ClearCtrlSpeakRasterizerResultNullsub(guiPtxBacking);
        (void)ReplacePtxTexture(static_cast<std::uint32_t>(ctrlSpeakTexturePoolHandles[slot]), guiPtxBacking);
        ctrlSpeakTexturePoolFlags[slot] |= 1U;
        return ctrlSpeakTexturePoolHandles[slot];
    }
    return -1;
}

void PrepareSpeakTextSlots() {
    for (std::size_t slot = 0; slot < 4; ++slot) {
        const std::uint16_t choice = static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]);
        if (choice == 0xffffU) {
            ctrlSpeakChoiceDrawSprites[slot].icon = 0xffffU;
        } else {
            const std::string_view label = ctrlSpeakMode < 3 ? GetActionSelectorLabel(choice) : GetLocalizedSceneText(CTRL_SPEAK_ALTERNATE_LABELS[choice]);
            if (label.data() && ctrlSpeakTexturePoolHandles[slot] >= 0)
                ctrlSpeakChoiceTextHandles[slot] = AcquireCtrlSpeakTextHandle(label.data());
        }
    }
}

std::int64_t RenderItemSelectorCounterNullsub(std::byte *) {
    return 0;
}

std::uint64_t BeginItemSelector() {
    const std::uint32_t claimed = GetShouldRenderIcons(RenderIconSlot::ItemSelect);
    if (claimed || !itemSelectTask)
        return claimed;
    HLib::ResolveDeferredCallback(itemSelectTask);
    itemSelectPhase = 1;
    RefreshItemSelectorSelection();
    itemSelectorNavigationInput = 0;
    padcRefreshCurrentInputs(true);
    ToggleRenderIcons(RenderIconSlot::ItemSelect, true);
    itemSelectorDelayedResult = 0;
    char text[24];
    const std::int32_t photos = GetSnapshotControlPhotoCount();
    if (photos == -1)
        std::sprintf(text, "???/%3d", 126);
    else
        std::sprintf(text, "%3d/%3d", photos, 126);
    RenderItemSelectorCounterNullsub(scenarioItemPtx);
    d3t::CreateMipmappedScaledTexture(scenarioItemTexture, 0, 1, scenarioItemPtx + guiPtxPrefixSize, 256 * 16 * 4);
    return scenarioItemTexture;
}

std::int64_t RequestItemSelector() {
    if (renderIconFlags || nativeGuiActive != 1)
        return 0;
    mainIconPostFxBank = static_cast<std::int32_t>(GetPostFxBankValue(1));
    SelectPostFxBank(1, 0);
    BeginItemSelector();
    mainIconPhase = 3;
    return 1;
}

// @verified - p
std::int64_t CheckMainIconShortcut() {
    if (mainIconDelay >= 0)
        return 0;
    for (const MainIconShortcut &shortcut : MAIN_ICON_SHORTCUTS) {
        if (!EV_GetShenmueFlag(shortcut.flag))
            continue;
        if (btnCheck(BUTTON_MAIN_ICON_SHORTCUT)) {
            UpdateSelectedItemStateFlag(shortcut.item);
            return 0;
        }
        RegisterButtonMediation(BUTTON_MAIN_ICON_SHORTCUT, nullptr, ButtonMediationBindA);
        return 1;
    }
    return 1;
}

// @verified - p
bool DrawMainIconOverlay(std::uint32_t colour, float depth) {
    LegacyColourQuad packet{.quad = {.flags = 0}, .colour = colour};
    AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 320, 240, 640, 480, ScreenMode::Adjusted);
    packet.quad.flags |= 0x1400U;
    return shenSubmitQuad(packet, depth);
}

void CtrlMainIcon_Callback(void *) {
    mainIconOverlayActive = 0;
    HLib::ExecuteCallbackNow([](void *) { TickScenarioFlagNotification(); }, nullptr);
    if (ctrlGetPadData())
        (void)ctrlGetPadInputBuffer0(ctrlGetPadData());
    if (nativeGuiActive && plymIsSearchable() && !plymIsSearchBlocked() && (g_loadingFlag & 2U) == 0 && renderIconFlags == 0 && !IsScenarioBusyForMainIcon() &&
        g_fightUiActive == 0 && srchCheckFlags(6) == 0 && srchCheckFlags(3) <= 0 && GetCurrentItemSelectState() == 0) {
        mainIconDelay = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(mainIconDelay) - 1U);
    } else if (mainIconDelay < 1) {
        mainIconDelay = 0;
        if ((g_loadingFlag & 2U) == 0)
            mainIconDelay = 5;
    } else {
        mainIconDelay = 5;
    }

    if (mainIconPhase) {
        if (mainIconPhase == 5)
            mainIconPhase = 0;
    } else {
        if (CheckMainIconShortcut()) {
            if (btnCheck(BUTTON_MAIN_ICON_MEMO)) {
                (void)RefreshPadTask(0, 1);
                return;
            }
            RegisterButtonMediation(BUTTON_MAIN_ICON_MEMO, nullptr, ButtonMediationBindA);
        }
        if (btnCheck(BUTTON_MAIN_ICON_ITEM_HELP) && helpGetCurrentIconIdx(1) == 67) {
            UpdateSelectedItemStateFlag(14);
        } else if (mainIconDelay < 0) {
            if (btnCheck(BUTTON_MAIN_ICON_ITEM_SELECT)) {
                if (nativeGuiActive == 1) {
                    mainIconPostFxBank = static_cast<std::int32_t>(GetPostFxBankValue(1));
                    SelectPostFxBank(1, 0);
                    BeginItemSelector();
                    mainIconPhase = 3;
                }
            } else {
                RegisterButtonMediation(BUTTON_MAIN_ICON_ITEM_SELECT, nullptr, ButtonMediationBindB);
            }
        }
    }

    const std::uint32_t itemState = GetCurrentItemSelectState() & 0x23U;
    if ((GetShouldRenderIcons(RenderIconSlot::ItemSelect) || (itemState & 0x21U) != 0) && IsWazamakiInactive() == 0 && (itemState & 2U) == 0) {
        DrawMainIconOverlay(0x76000000U, 0.3f);
        mainIconOverlayActive = 1;
    }
}

// @verified - p
void InitMainIcon_Callback(void *) {
    mainIconDelay = 5;
    iconPadControl = AllocatePadControlEntry();
    HLib::SetCurrentTaskTo(CtrlMainIcon_Callback);
}

std::int64_t BeginSelectorMemo() {
    return RefreshPadTask(1, 1);
}
std::int64_t UpdateSelectorMemo() {
    return IsItemSelectable();
}
void CancelSelectorMemo(std::int32_t force) {
    if (force)
        RequestMemoClose();
}
std::int64_t BeginSelectorWatch() {
    return OpenGameWatchMenuItem();
}
std::int64_t UpdateSelectorWatch() {
    return IsWristwatchOpenable();
}
void CancelSelectorFullMap(std::int32_t) {
    CloseFullMapMenu();
}
std::int64_t BeginSelectorItemLook() {
    return OpenItemLook(itemSelectorSelection.selectedItem, 1);
}
std::int64_t UpdateSelectorItemLook() {
    return IsAnyItemSelectActive();
}
// @verified - p
void CancelSelectorItemLook(std::int32_t force) {
    if ((GetCurrentItemSelectState() & 1U) && force)
        CloseItemLook();
}
std::int64_t BeginSelectorSave() {
    return OpenSaveGameMenuItem();
}
std::int64_t UpdateSelectorSave() {
    return IsSaveAllowed();
}
void CompleteSelectorSave() {
    shenSetTimeSimulationTo(true);
    CheckHelpIcon(itemSelectorDelayedResult == 32 ? 0 : 33, 0, CheckHelpIconOp::Set);
}
std::int64_t BeginSelectorEasyOptions() {
    SetNewEasyOptionState(0);
    return 1;
}
std::int64_t UpdateSelectorEasyOptions() {
    return IsEasyOptionActive();
}
void CancelSelectorEasyOptions(std::int32_t force) {
    AbortEasyOptionState(force);
}
std::int64_t UpdateSelectorMoves() {
    return !UpdateWazamakiMenu();
}
std::int64_t BeginSelectorCollection() {
    return OpenLegacyCollectionMenu(0);
}
std::int64_t BeginSelectorPhotos() {
    return OpenLegacyCollectionMenu(2);
}
// @verified - p
std::int64_t UpdateSelectorCollection() {
    return static_cast<std::uint8_t>(UpdateLegacyCollectionMenu()) == 0;
}
// @verified - p
std::int64_t UpdateSelectorPhotos() {
    PublishItemSelectorOwnedMask(itemSelectorOwnedMask.data());
    return static_cast<std::uint8_t>(UpdateLegacyCollectionMenu()) == 0;
}
void CancelSelectorCollection(std::int32_t force) {
    CloseLegacyCollectionMenu(force);
}
std::int64_t UpdateSelectorNavigation() {
    return (GetCurrentItemSelectState() & 8U) != 0;
}
void CancelSelectorNavigation(std::int32_t force) {
    CloseNaviWindow(force);
}
std::int64_t BeginSelectorPawn() {
    return OpenPawnWindow(0.0f);
}
std::int64_t UpdateSelectorPawn() {
    return IsPawnWindowOpen();
}
void CancelSelectorPawn(std::int32_t force) {
    ClosePawnWindow(force);
}
std::int64_t BeginSelectorMoveHandbook() {
    return OpenWfScrollMenu();
}
std::int64_t UpdateSelectorMoveHandbook() {
    return GetwfMainControlState();
}
void CancelSelectorMoveHandbook(std::int32_t force) {
    CloseWfScrollMenu(force);
}

void ResolveUniqueItemAction(std::int32_t mode) {
    UniqueItemActionData *const action = static_cast<UniqueItemActionData *>(HLib::GetTaskParameter(uniqueItemActionTask));
    if (!action || mode >= static_cast<std::int32_t>(ITEM_SELECTOR_CALLBACKS.size()))
        return;
    action->item = mode;
    action->begin = ITEM_SELECTOR_CALLBACKS[mode].begin;
    action->update = ITEM_SELECTOR_CALLBACKS[mode].update;
    action->complete = ITEM_SELECTOR_CALLBACKS[mode].complete;
    action->cancel = ITEM_SELECTOR_CALLBACKS[mode].cancel;
    HLib::ResolveDeferredCallback(uniqueItemActionTask);
    HLib::SetupTimedTask(itemSelectTask, 0);
}

void ResetUniqueItemAction(UniqueItemActionData &action) {
    action.begin = nullptr;
    action.update = nullptr;
    action.complete = nullptr;
    action.cancel = nullptr;
    action.item = -1;
    if (uniqueItemActionTask)
        HLib::SetupTimedTask(uniqueItemActionTask, 0);
}

// @verified - p
void ICIT_UniqAction_Callback(void *) {
    UniqueItemActionData &action = *static_cast<UniqueItemActionData *>(HLib::GetTaskParameter(uniqueItemActionTask));
    if (action.begin) {
        if (static_cast<std::uint32_t>(action.begin()) != 0) {
            (void)ExecuteAicaCommand(0x900abU, 0, 0);
        } else {
            (void)ExecuteAicaCommand(0x800abU, 0, 0);
            action.update = nullptr;
            action.complete = nullptr;
            action.cancel = nullptr;
            action.item = -1;
            RefreshItemSelectorInventory(0);
            HLib::SetupTimedTask(uniqueItemActionTask, 0);
            HLib::ResolveDeferredCallback(itemSelectTask);
            ToggleGameMenu();
        }
        action.begin = nullptr;
        return;
    }
    if (action.update && static_cast<std::uint32_t>(action.update()) != 0)
        return;
    if (action.complete)
        action.complete();
    iconToggleStateFor(1, true);
    RefreshItemSelectorLabel();
    RefreshItemSelectorInventory(0);
    (void)ExecuteAicaCommand(0x900abU, 0, 0);
    ResetUniqueItemAction(action);
    HLib::ResolveDeferredCallback(itemSelectTask);
    ToggleGameMenu();
}

Character *ItemSelectorPlayer() {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return static_cast<Character *>(control->ryo);
}

ItemSelectorContext *CaptureItemSelectorContext() {
    ItemSelectorContext &context = itemSelectorContext;
    context.perspective = camCtrlGetPerspective();
    context.cameraState = camCtrlGetState();
    camCtrlGetNextCamPos(&context.eye);
    camCtrlGetTargetPosition(&context.target);
    context.helpTopic = helpGetCurrentIconIdx(1);
    context.visible = static_cast<std::uint16_t>(GetUiFlag(0) & 0xfdffU);
    context.selected = static_cast<std::uint8_t>(GetUiFlag(1));
    Character *const player = ItemSelectorPlayer();
    if (!player)
        return nullptr;
    context.playerDisplay = IsOfThisVisibilityType(player, 3);
    context.playerVisible = charaGetWasVisible(player);
    context.weather = SetWeatherLightManager(0);
    return &context;
}

void UpdateItemSelectorCameraGoal(ItemSelectorCamera &camera, Character &player) {
    const std::int32_t yaw = -static_cast<std::int16_t>(GetCharacterMotionYaw(&player));
    camera.goalEye = GetCharacterWorldPosition(&player);
    camera.goalEye.y += 1.6f;
    camera.goalTarget = camera.goalEye;
    camera.goalTarget.x += sin_fixed16(yaw) * 0.2f;
    camera.goalTarget.z = cos_fixed16(static_cast<std::uint16_t>(yaw)) * -0.2f + camera.goalTarget.z;
}

void SetItemSelectorCameraSteps(ItemSelectorCamera &camera, float frames) {
    camera.eyeStep = {(camera.goalEye.x - camera.eye.x) / frames, (camera.goalEye.y - camera.eye.y) / frames, (camera.goalEye.z - camera.eye.z) / frames};
    camera.targetStep = {
        (camera.goalTarget.x - camera.target.x) / frames, (camera.goalTarget.y - camera.target.y) / frames, (camera.goalTarget.z - camera.target.z) / frames};
}

void PrepareItemSelectorCamera() {
    using enum CameraState;
    Character *const player = ItemSelectorPlayer();
    if (!player)
        return;
    ItemSelectorCamera &camera = itemSelectorCamera;
    camera.duration = 8;
    camera.frame = 0;
    camera.mode = 2;
    camCtrlGetNextCamPos(&camera.savedEye);
    camCtrlGetTargetPosition(&camera.savedTarget);
    camera.savedAlpha = player->alpha;
    camCtrlSetState(Event);
    camera.eye = camera.savedEye;
    camera.target = camera.savedTarget;
    UpdateItemSelectorCameraGoal(camera, *player);
    SetItemSelectorCameraSteps(camera, static_cast<float>(camera.duration));
    camCtrlSetNextPosition(camera.eye);
    camCtrlSetTargetPosition(camera.target);
    camera.alpha = camera.savedAlpha;
    camera.alphaStep = -camera.savedAlpha / static_cast<float>(camera.duration / 2);
    player->alpha = camera.alpha;
}

bool AdvanceItemSelectorCamera() {
    Character *const player = ItemSelectorPlayer();
    if (!player || srchCheckFlags(3) >= 0)
        return true;
    ItemSelectorCamera &camera = itemSelectorCamera;
    ++camera.frame;
    const bool finished = camera.frame > camera.duration;
    if (!finished) {
        if (camera.mode == 2) {
            UpdateItemSelectorCameraGoal(camera, *player);
            const std::int32_t remaining = camera.duration - camera.frame;
            SetItemSelectorCameraSteps(camera, static_cast<float>(remaining > 0 ? remaining : 1));
        }
        camera.eye.x += camera.eyeStep.x;
        camera.eye.y += camera.eyeStep.y;
        camera.eye.z += camera.eyeStep.z;
        camera.target.x += camera.targetStep.x;
        camera.target.y += camera.targetStep.y;
        camera.target.z += camera.targetStep.z;
        camera.alpha += camera.alphaStep;
        if (!(camera.alpha >= 0.0f))
            camera.alpha = 0.0f;
        else if (camera.alpha > 1.0f)
            camera.alpha = 1.0f;
    } else {
        camera.eye = camera.goalEye;
        camera.target = camera.goalTarget;
        camera.alpha = camera.mode == 0 ? camera.savedAlpha : 0.0f;
    }
    camCtrlSetNextPosition(camera.eye);
    camCtrlSetTargetPosition(camera.target);
    player->alpha = camera.alpha;
    return finished;
}

void CloseItemSelectorCamera() {
    Character *const player = ItemSelectorPlayer();
    if (!player)
        return;
    ItemSelectorCamera &camera = itemSelectorCamera;
    camera.goalEye = camera.savedEye;
    camera.goalTarget = camera.savedTarget;
    camera.duration = 8;
    camera.frame = 0;
    camera.mode = 3;
    SetItemSelectorCameraSteps(camera, 8.0f);
    camCtrlSetNextPosition(camera.eye);
    camCtrlSetTargetPosition(camera.target);
    camera.alphaStep = camera.savedAlpha / static_cast<float>(camera.duration / 2);
    player->alpha = camera.alpha;
}

void RestoreItemSelectorContext() {
    if (!itemSelectorSelection.context)
        return;
    const ItemSelectorContext &context = *itemSelectorSelection.context;
    if (Character *const player = ItemSelectorPlayer()) {
        EV_SetCharDisplay(player, CVM_DisplayEnabled, context.playerDisplay);
        charaSetVisibilityFlagTo(player, context.playerVisible);
    }
    camCtrlSetState(context.cameraState);
    camCtrlSetPerspective(context.perspective);
    camCtrlSetNextPosition(context.eye);
    camCtrlSetTargetPosition(context.target);
    EV_EventSetUpHelpIcon_0(context.helpTopic, 0);
    SetHelpIconMenuFlags(context.visible);
    if (!context.visible)
        ResetHelpIconsToItemPrompt();
    SetWeatherLightManager(context.weather);
    itemSelectorSelection.context = nullptr;
}

void FinishItemSelector() {
    if (!GetShouldRenderIcons(RenderIconSlot::ItemSelect))
        return;
    if (itemSelectTask)
        HLib::SetupTimedTask(itemSelectTask, 0);
    itemSelectPhase = 0;
    padcRefreshCurrentInputs(false);
    ToggleRenderIcons(RenderIconSlot::ItemSelect, false);
    itemSelectorSelection.exitFlags = 0;
    RestoreItemSelectorContext();
    itemSelectorLabelTexture = -1;
    SelectPostFxBank(1, mainIconPostFxBank);
    mainIconPhase = 5;
}

bool ConfirmGameMenuClose() {
    if (AskKeepOrRevertChanges())
        return false;
    if (DestroyElementByIndex(UIElement::GameMenu) != 1)
        return false;
    itemSelectPhase = 4;
    return true;
}
void ResetLegacySelectorSubmenu() {
    if (legacySelectorSubmenuPhase == 1) {


        static constexpr std::array<std::int32_t, 8> elements{44, 45, -1, 46, 47, 30, 48, 57};
        DestroyElementByIndex(static_cast<UIElement>(elements[legacySelectorSubmenuSelection]));
    }
    legacySelectorSubmenuPhase = 0;
}

bool NavigateLegacyItemSelector(std::uint16_t pressed) {
    ItemSelectorSelection &selection = itemSelectorSelection;
    if (selection.activeMenu == 9 && selection.actionResult == 0) {
        if (selection.count < 2)
            pressed &= 0x3fffU;
        if (selection.selectedItem < 0)
            pressed &= 0xfbffU;
        if (pressed & 0x400U) {
            std::int32_t mode = 3;
            for (std::int32_t index = 0; index < 3; ++index)
                if (selection.selectedItem == ITEM_SELECTOR_SPECIAL_ITEMS[index]) {
                    mode = index;
                    break;
                }
            ResolveUniqueItemAction(mode);
            return false;
        }
        if (pressed & 0x4000U) {
            if (--selection.column < 0)
                selection.column = selection.columns - 1;
            ClampItemSelectorSelection();
        } else if (pressed & 0x8000U) {
            ++selection.column;
            if (selection.column + selection.row * selection.columns >= selection.count)
                selection.column = 0;
            if (selection.column >= selection.columns)
                selection.column = 0;
        } else if (pressed & 0x2000U) {
            if (selection.column + (selection.row + 1) * selection.columns >= selection.count || selection.row == selection.visibleRows - 1) {
                selection.lowerRow = 0;
                selection.activeMenu = selection.column < ITEM_SELECTOR_LOWER_COUNTS[0] ? selection.column : ITEM_SELECTOR_LOWER_COUNTS[0] - 1;
            } else
                ++selection.row;
        } else if (pressed & 0x1000U) {
            if (--selection.row < 0) {
                selection.lowerRow = 1;
                selection.activeMenu = selection.column < ITEM_SELECTOR_LOWER_COUNTS[1] ? selection.column : ITEM_SELECTOR_LOWER_COUNTS[1] - 1;
            }
        }
    } else if (pressed & 0x400U) {
        ResolveUniqueItemAction(ITEM_SELECTOR_ACTIONS[ITEM_SELECTOR_MENU_MAP[selection.lowerRow][selection.activeMenu]]);
        return false;
    } else if (pressed & 0x1000U) {
        if (--selection.lowerRow < 0) {
            selection.row = selection.visibleRows - 1;
            selection.column = static_cast<std::int8_t>(selection.activeMenu);
            selection.activeMenu = 9;
            if (selection.column + selection.row * selection.columns >= selection.count && selection.row >= 1)
                --selection.row;
            ClampItemSelectorSelection();
        } else {
            if (selection.activeMenu >= ITEM_SELECTOR_LOWER_COUNTS[selection.lowerRow])
                selection.activeMenu = ITEM_SELECTOR_LOWER_COUNTS[selection.lowerRow] - 1;
            selection.column = static_cast<std::int8_t>(selection.activeMenu);
        }
    } else if (pressed & 0x2000U) {
        if (++selection.lowerRow < 2) {
            selection.column = static_cast<std::int8_t>(selection.activeMenu);
        } else {
            selection.column = static_cast<std::int8_t>(selection.activeMenu);
            selection.row = 0;
            selection.activeMenu = 9;
            ClampItemSelectorSelection();
        }
    } else if (pressed & 0x8000U) {
        selection.activeMenu = (selection.activeMenu + 1) % ITEM_SELECTOR_LOWER_COUNTS[selection.lowerRow];
    } else if (pressed & 0x4000U) {
        selection.activeMenu = selection.activeMenu ? selection.activeMenu - 1 : ITEM_SELECTOR_LOWER_COUNTS[selection.lowerRow] - 1;
    }
    return (pressed & 0xf000U) != 0;
}

void ProcessLegacyItemSelectorInput() {
    ItemSelectorSelection &selection = itemSelectorSelection;
    const std::uint16_t rawInput = GetPadControlData(1);
    std::uint16_t pressed = itemSelectorDelayedResult ? 0 : rawInput & 0xf600U;
    itemSelectorNavigationInput &= 0xffff0000U;
    if (pressed & 0x200U)
        itemSelectPhase = 4;
    else {
        if (selection.activeMenu != 9 || selection.actionResult != 0)
            itemSelectorNavigationInput &= 0xffffU;
        if (pressed && NavigateLegacyItemSelector(pressed)) {
            RefreshItemSelectorSelectedItem();
            RefreshItemSelectorLabel();
        }
    }
    if (itemSelectorDelayedResult == 2) {
        itemSelectPhase = 4;
        selection.actionResult = (static_cast<std::uint32_t>(selection.selectedItem) << 16U) | 2U;
    } else if (itemSelectorDelayedResult == 32) {
        itemSelectPhase = 6;
        iconToggleStateFor(1, false);
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    }
    itemSelectorDelayedResult = 0;
}

void CtrlItemSelectIcon_Callback(void *) {
    if (Character *const player = ItemSelectorPlayer())
        charaSetFlagFor(player, 1);
    if (!(g_loadingFlag & 2U)) {
        switch (itemSelectPhase) {
        case 1:
            itemSelectPhase = 2;
            itemSelectorSelection.exitFlags = 0;
            itemSelectorSelection.context = CaptureItemSelectorContext();
            itemSelectorWindowPosition = {-672.0f, 55.0f};
            itemSelectorCameraActive = true;
            PrepareItemSelectorCamera();
            RefreshItemSelectorSelectedItem();
            RefreshItemSelectorLabel();
            break;
        case 2: {
            const std::uint16_t pendingInput = static_cast<std::uint16_t>(GetPadControlData(1) & 0x202U);
            if (!(pendingInput & 0x200U) && (pendingInput & 2U))
                itemSelectorNavigationInput = (itemSelectorNavigationInput & 0xffff0000U) | pendingInput;
            if (AdvanceItemSelectorCamera()) {
                iconToggleStateFor(1, true);
                itemSelectorWindowPosition.x = -55.0f;
                itemSelectPhase = 3;
                EV_EventSetUpHelpIcon_0(0x11, 0);
                if (itemSelectorSelection.context->helpTopic)
                    SetHelpIconMenuFlags(0x21);
                else
                    AddHelpIconMenuFlags(0x21);
                ShowElementByIndex(UIElement::GameMenu);
            }
            break;
        }
        case 3:
            if (itemSelectorSelection.exitFlags & 1U) {
                iconToggleStateFor(1, false);
                itemSelectPhase = 4;
            } else if (GetDebugForceUIEnabled(UIElement::GameMenu)) {
                if (((itemSelectPhase - 4) & 0xfc) != 0 || itemSelectPhase == 6) {
                    if (ExecuteElementCallback(UIElement::GameMenu) == 0 && g_debugImGuiRenderMode == 0)
                        itemSelectPhase = 4;
                }
            } else
                ProcessLegacyItemSelectorInput();
            set_icon_select_flags_to(0x600U);
            break;
        case 4:
            itemSelectPhase = 5;
            break;
        case 5:
            iconToggleStateFor(1, false);
            ResetLegacySelectorSubmenu();
            [[fallthrough]];
        case 6:
            if (!itemSelectorSelection.context || static_cast<std::int32_t>(itemSelectorSelection.context->cameraState) != 0) {
                CloseItemSelectorCamera();
                itemSelectPhase = 7;
            } else {
                itemSelectorCameraActive = false;
                camCtrlSetFovTransitionTo(false);
                FinishItemSelector();
            }
            break;
        case 7:
            if (itemSelectorCameraActive && AdvanceItemSelectorCamera()) {
                itemSelectorCameraActive = false;
                FinishItemSelector();
            }
            break;
        default:
            break;
        }
    }
    if (GetItemSelectionMode(1))
        HLib::ExecuteCallbackNow(RenderLegacyItemSelector, &itemSelectorSelection);
}

void InitItemSelectIcon_Callback(void *) {
    itemLookTask = HLib::EnqueueTaskWithoutParameter(ICSU_InitItemLook_Callback, HLib::GetCurrentTaskIndex(), 0, static_cast<std::int32_t>(TASK_ICLI.name));
    uniqueItemActionTask = HLib::EnqueueTaskWithParameter(ICIT_UniqAction_Callback, HLib::GetCurrentTaskIndex(), 0, sizeof(UniqueItemActionData), TASK_ICSS);
    if (uniqueItemActionTask) {
        UniqueItemActionData *const action = static_cast<UniqueItemActionData *>(HLib::GetTaskParameter(uniqueItemActionTask));
        if (action) {
            action->begin = nullptr;
            action->update = nullptr;
            action->complete = nullptr;
            action->cancel = nullptr;
            action->item = -1;
        }
        HLib::SetupTimedTask(uniqueItemActionTask, 0);
    }
    InitializeItemSelectorWindow();
    InitializeLegacyCollectionMenu();
    itemSelectPhase = 0;
    iconToggleStateFor(1, false);
    itemSelectorSelection.activeMenu = 9;
    itemSelectorSelection.actionResult = 0;
    itemSelectorSelection.count = 0;
    itemSelectorSelection.row = itemSelectorSelection.column = 0;
    itemSelectorSelection.visibleRows = 1;
    itemSelectorSelection.columns = 9;
    itemSelectorSelection.glowColour = itemSelectorSelection.exitFlags = 0;
    itemSelectorSelection.glowBrightening = 1;
    itemSelectorSelection.context = nullptr;
    itemSelectorSelection.selectedItem = -1;
    helpIconItemEffectState = 0;
    itemSelectorNavigationInput = 0;
    currentItemSelectState = 0;
    itemSelectorCameraActive = false;
    itemSelectTask = HLib::GetCurrentTask();
    Queue_wf_MainControl(7);
    HLib::SetCurrentTaskTo(CtrlItemSelectIcon_Callback);
    HLib::SetupTimedTask(itemSelectTask, 0);
}

std::int32_t MoneyIconStep(std::int32_t delta) {
    if (moneyIconFlags & 0x40) {
        const std::uint32_t raw = static_cast<std::uint32_t>(moneyIconFixedStep);
        const std::uint32_t magnitude = moneyIconFixedStep < 0 ? 0U - raw : raw;
        return std::bit_cast<std::int32_t>((delta < 0 ? 0U - magnitude : magnitude));
    }
    const std::int32_t sign = delta < 0 ? -1 : 1;
    const std::uint32_t raw = static_cast<std::uint32_t>(delta);
    std::int32_t remaining = std::bit_cast<std::int32_t>(delta < 0 ? 0U - raw : raw);
    std::int32_t step = 0;
    for (const std::int32_t increment : moneyIconIncrements) {
        if (remaining < increment)
            continue;
        step += increment;
        remaining %= increment;
    }
    return sign * step;
}

// @verified - p
std::int64_t ReleaseCtrlSpeakTextHandle(std::int32_t handle) {
    for (std::size_t slot = 0; slot < ctrlSpeakTexturePoolHandles.size(); ++slot)
        if (ctrlSpeakTexturePoolHandles[slot] == handle)
            ctrlSpeakTexturePoolFlags[slot] &= ~1U;
    return 0;
}

// @verified - p
void ReleaseMoneyTextSlots() {
    for (LegacyIconSprite &sprite : moneyIconTextSprites) {
        if (sprite.icon != 0xffffU)
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(sprite.icon));
        sprite.icon = 0xffffU;
    }
}

// @verified - p
void ChooseMoneyOptions(std::int32_t flags) {
    for (std::size_t index = 0; index < moneyIconTextSprites.size(); ++index) {
        if (!(flags & MONEY_TEXT_OPTION_FLAGS[index]))
            continue;
        LegacyIconSprite &sprite = moneyIconTextSprites[index];
        if (sprite.icon != 0xffffU)
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(sprite.icon));
        sprite.icon = static_cast<std::uint16_t>(AcquireCtrlSpeakTextHandle(MONEY_TEXT_OPTION_NAMES[index]));
    }
}

// The numeric mode borrows the dialogue Action Selector's input/transition owner.
void SetNumericActionSelector(const std::array<std::int32_t, 4> &choices, std::int32_t maximum) {
    NumericActionSelector &selector = numericActionSelector;
    selector.choices = choices;
    selector.mode = 1;
    selector.amount = 0;
    selector.step = choices[1];
    selector.maximum = maximum;
    SetGameMessageContent(1, 0, "0");
    ShowElementByIndex(UIElement::GameMessage);
    for (std::size_t slot = 0; slot < choices.size(); ++slot) {
        std::array<char, 256> &label = selector.labels[slot];
        if (slot == 1 || slot == 2) {
            FormatMoneyAmount(label.data(), choices[slot], true);
        } else {
            const std::string_view text = GetActionSelectorLabel(static_cast<std::uint16_t>(choices[slot]));
            std::memcpy(label.data(), text.data(), text.size());
            char *output = label.data() + text.size();
            if (choices[slot] == 485) {
                *output++ = ' ';
                *output++ = '(';
                FormatMoneyAmount(output, maximum, true);
                output += std::strlen(output);
                *output++ = ')';
            }
            *output = '\0';
        }
    }
}

std::uint32_t GetNumericActionSelectorAmount() {
    return static_cast<std::uint32_t>(numericActionSelector.amount);
}

std::uint32_t ConsumeActionSelectorInput() {
    const std::uint32_t input = actionSelectorInput;
    actionSelectorInput = 13;
    return input;
}

// @verified - p
void UpdateMoneySelection() {
    const std::int32_t previous = moneyIconAmount;
    moneyIconAmount = static_cast<std::int32_t>(GetNumericActionSelectorAmount());
    const std::uint32_t input = ConsumeActionSelectorInput();
    if ((input == 4 || input == 9) && previous == 0) {
        moneyIconResult = -2;
        moneyIconPhase = 4;
    } else if (input == 8) {
        moneyIconPhase = 4;
    }
    if (previous == moneyIconAmount &&
        ((input == 2 && moneyIconAmount <= 0) || (input == 6 && moneyIconAmount >= moneyIconMaximum) || (input == 0 && moneyIconAmount >= moneyIconMaximum)))
        (void)ExecuteAicaCommand(0x800abU, 0, 0);
    set_icon_select_flags_to(input);
}

bool UpdateMoneyCash() {
    moneyIconSettleFrames -= 1.0f;
    if (moneyIconDisplay == moneyIconTarget) {
        if (moneyIconSettleFrames > 0.0f)
            return false;
        moneyIconSettleFrames = 0.0f;
        return true;
    }
    const bool decreasing = moneyIconDisplay > moneyIconTarget;
    const std::uint32_t display = static_cast<std::uint32_t>(moneyIconDisplay);
    const std::uint32_t target = static_cast<std::uint32_t>(moneyIconTarget);
    moneyIconLastIncrement = MoneyIconStep(std::bit_cast<std::int32_t>(decreasing ? display - target : target - display));
    const std::uint32_t step = static_cast<std::uint32_t>(moneyIconLastIncrement);
    moneyIconDisplay = std::bit_cast<std::int32_t>(decreasing ? display - step : display + step);
    const bool complete = decreasing ? moneyIconDisplay <= moneyIconTarget : moneyIconDisplay >= moneyIconTarget;
    if (complete)
        moneyIconDisplay = moneyIconTarget;
    if (!(moneyIconFlags & 0x20)) {
        if (moneyIconDrawMode)
            SetCurrentCash(moneyIconDisplay);
        else
            SetCurrentSegaCoins(moneyIconDisplay);
    }
    SetGameMessageContent(moneyIconDrawMode ? 1 : 2, moneyIconDisplay, "0");
    (void)ExecuteAicaCommand(0xd00abU, 0, 0);
    return complete;
}

// @verified - p
void AnimateMoneyUiElement(void *) {
    if (!(moneyIconFlags & 4))
        return;
    const std::int32_t amount = moneyIconDisplay - ((moneyIconFlags & 8) ? moneyIconAmount : 0);
    std::uint32_t colour = 0xff656565U;
    std::uint8_t cash = 2;
    LegacyIconSprite &icon = moneyIconSprite;
    icon.x = moneyIconPosition.x;
    icon.y = moneyIconPosition.y;
    icon.icon = 1544;
    icon.width = 80;
    icon.height = 40;
    if (!moneyIconDrawMode || moneyIconDrawMode == 3) {
        icon.icon = moneyIconDrawMode ? 515 : 3;
        icon.width = icon.height = 36;
        icon.y += 4.0f;
        if (moneyIconDrawMode)
            colour = 0xe0105010U;
        else
            cash = 0;
    }
    PrepareDrawWithMode(icon, static_cast<ScreenMode>(0));
    const float x = moneyIconPosition.x - 63.0f, y = moneyIconPosition.y - 21.0f;
    DrawLegacyWindowHorizontal(x, y, 6, 1, 0, 0.2f, 1024, static_cast<ScreenMode>(0));
    DrawLegacyWindowHorizontal(x, y + 48.0f, 6, 1, 1, 0.2f, 1024, static_cast<ScreenMode>(0));
    DrawLegacyWindowTiles(x + 35.0f, y + 24.0f, 6, 1, 0.18f, 1024, static_cast<ScreenMode>(0));
    DrawLegacyWindowBackground(x + 15.0f, y + 4.0f, 240.0f, 40.0f, 0.19f, 1024, static_cast<ScreenMode>(0));
    DrawMoneyAmount(moneyIconPosition.x + 148.0f, moneyIconPosition.y + 10.0f, moneyIconPosition.depth, 1.0f, amount, cash, colour, static_cast<ScreenMode>(0));
}

// @verified - p
void SetupMoneyIconForRendering(void *) {
    const std::array<bool, 4> visible{true, true, (moneyIconFlags & 2) != 0, (moneyIconFlags & 1) != 0};
    for (std::size_t index = 0; index < moneyIconDirections.size(); ++index) {
        LegacyIconSprite &sprite = moneyIconDirections[index];
        sprite.x = moneyIconPosition.x + 43.0f + MONEY_DIRECTION_X[index];
        sprite.y = moneyIconSelectorPosition.y - 2.6875f + MONEY_DIRECTION_Y[index];
        if (moneyIconPosition.selectedDirection == index) {
            sprite.scaleX = sprite.scaleY = moneyIconSelectionScale;
            if (moneyIconSelectionScale <= 1.0f)
                moneyIconPosition.selectedDirection = 4;
        }
        if (visible[index])
            PrepareDrawSimple(sprite, 0xb0000000U, static_cast<ScreenMode>(0));
    }
    const std::int32_t digits = GetSelectorNumberDigits(moneyIconStepAmount);
    const float leadingOffset = digits >= 0 ? static_cast<float>(digits) * -16.0f : -16.0f;
    for (std::int32_t index = 1; index >= 0; --index) {
        const LegacyIconSprite &sprite = moneyIconDirections[index];
        DrawMoneyDigits(sprite.x + 16.0f, sprite.y, sprite.scaleX, sprite.depth + 1.0f, moneyIconStepAmount, 0xff656565U, static_cast<ScreenMode>(0));
        LegacyIconSprite &sign = moneyIconSigns[index];
        sign.x = sprite.x + 16.0f + sprite.scaleX * leadingOffset;
        sign.y = sprite.y;
        sign.scaleX = sign.scaleY = sprite.scaleX;
        PrepareDrawSimple(sign, 0xff656565U, static_cast<ScreenMode>(0));
    }
    DrawMoneyAmount(moneyIconPosition.x + 48.0f, 170.0f, 3.0f, 1.0f, moneyIconAmount, moneyIconCurrency, 0, static_cast<ScreenMode>(0));
    for (std::int32_t index = 1; index >= 0; --index) {
        if (!(moneyIconFlags & MONEY_TEXT_RENDER_FLAGS[index]))
            continue;
        const LegacyIconSprite &direction = moneyIconDirections[index + 2];
        LegacyIconSprite &text = moneyIconTextSprites[index];
        text.x = direction.x;
        text.y = direction.y;
        text.scaleX = direction.scaleX;
        text.scaleY = direction.scaleY;
        if (text.icon != 0xffffU)
            PrepareDrawTextureWithMode(text, static_cast<ScreenMode>(0));
    }
}

bool MoneyMessageTransitionBusy() {
    return gameMessageTransition == -1 || IsUITransitionActive(gameMessageTransition);
}

void RestoreMoneyHelpState() {
    if (moneyIconSavedHelpPresent)
        SetHelpIconMenuFlags(moneyIconSavedHelpVisible);
    else
        ResetHelpIconsToItemPrompt();
}

void CtrlMoneyIcon_Callback(void *) {
    switch (moneyIconPhase) {
    case 2:
        moneyIconSprite.icon = moneyIconDrawMode ? 1544 : 3;
        moneyIconSprite.width = moneyIconSprite.height = moneyIconDrawMode ? 40 : 36;
        if (moneyIconDrawMode)
            moneyIconSprite.width = 80;
        if (!(moneyIconFlags & 0x20))
            moneyIconDisplay = moneyIconDrawMode ? GetCurrentCash() : GetCurrentSegaCoins();
        moneyIconPhase = 1;
        iconToggleStateFor(4, true);
        moneyIconPosition.x = -516.0f;
        moneyIconPosition.selectedDirection = 4;
        moneyIconSettleFrames = 4.0f;
        moneyIconAmount = 0;
        moneyIconResult = 0;
        moneyIconSelectionScale = 1.0f;
        ChooseMoneyOptions(moneyIconFlags);
        if (moneyIconFlags & 8) {
            padcRefreshCurrentInputs(true);
            if (GetDebugForceUIEnabled(UIElement::ActionSelector)) {
                ShowElementByIndex(UIElement::ActionSelector);
                SetNumericActionSelector({485, moneyIconStepAmount, -moneyIconStepAmount, 483}, moneyIconMaximum);
            }
        } else if (GetDebugForceUIEnabled(UIElement::GameMessage)) {
            SetGameMessageContent(moneyIconDrawMode ? 1 : 2, moneyIconDisplay, "0");
            ShowElementByIndex(UIElement::GameMessage);
        }
        break;
    case 1:
        if (GetDebugForceUIEnabled(UIElement::GameMessage)) {
            if (MoneyMessageTransitionBusy())
                break;
        } else if (!AdvanceIconPosition(moneyIconPosition.x, moneyIconPosition.velocity, 80.0f)) {
            break;
        }
        if (moneyIconFlags & 8) {
            moneyIconSavedHelpSelected = static_cast<std::uint16_t>(GetUiFlag(1));
            moneyIconSavedHelpVisible = static_cast<std::uint16_t>(GetUiFlag(0));
            moneyIconSavedHelpPresent = static_cast<std::uint16_t>(GetUiFlag(2));
            if (moneyIconSavedHelpPresent)
                SetHelpIconMenuFlags(33);
            else
                AddHelpIconMenuFlags(33);
        }
        moneyIconPhase = 3;
        break;
    case 3:
        if (moneyIconFlags & 8) {
            moneyIconFrames -= 1.0f;
            if (moneyIconFrames > 0.0f)
                UpdateMoneySelection();
            else {
                moneyIconPhase = 4;
                moneyIconAmount = 0;
                moneyIconResult = -1;
                DestroyElementByIndex(UIElement::GameMessage);
                DestroyElementByIndex(UIElement::ActionSelector);
            }
        } else {
            if (moneyIconFrames >= 30.0f)
                moneyIconFrames -= 1.0f;
            if (moneyIconFrames < 30.0f && (moneyIconFlags & 0x10) && UpdateMoneyCash() && (moneyIconFlags & 0x10)) {
                moneyIconFrames -= 1.0f;
                if (moneyIconFrames <= 0.0f) {
                    moneyIconPhase = 4;
                    moneyIconFrames = 0.0f;
                }
            }
        }
        break;
    case 4:
        if (moneyIconPosition.selectedDirection == 4) {
            moneyIconPhase = 5;
            DestroyElementByIndex(UIElement::GameMessage);
            if (moneyIconFlags & 8)
                DestroyElementByIndex(UIElement::ActionSelector);
        }
        break;
    case 5:
        if (GetDebugForceUIEnabled(UIElement::GameMessage)) {
            if (MoneyMessageTransitionBusy())
                break;
        } else if (!RetreatIconPosition(moneyIconPosition.x, moneyIconPosition.velocity, -516.0f)) {
            break;
        }
        moneyIconPhase = 0;
        iconToggleStateFor(4, false);
        ToggleRenderIcons(RenderIconSlot::MoneyMessage, false);
        moneyIconDrawMode = 2;
        if (moneyIconFlags & 8) {
            RestoreMoneyHelpState();
            padcRefreshCurrentInputs(false);
        }
        moneyIconFlags = 0;
        ReleaseMoneyTextSlots();
        break;
    default:
        break;
    }
    if (!GetItemSelectionMode(4))
        return;
    if (GetDebugForceUIEnabled(UIElement::GameMessage))
        EnableElement(UIElement::GameMessage);
    HLib::ExecuteCallbackNow(AnimateMoneyUiElement, nullptr);
    if (moneyIconFlags & 8) {
        if (moneyIconPosition.selectedDirection != 4) {
            moneyIconSelectionScale += 0.25f;
            if (moneyIconSelectionScale >= 2.0f)
                moneyIconSelectionScale = 1.0f;
        }
        HLib::ExecuteCallbackNow(SetupMoneyIconForRendering, nullptr);
        EnableElement(UIElement::ActionSelector);
        ExecuteElementCallback(UIElement::ActionSelector);
    }
}

void PlayQuickIconResult(bool firstTable) {
    if ((quickIconFlags & 1U) == 0 || (quickIconFlags & 0x80U) != 0)
        return;
    const std::size_t variant = (quickIconFlags >> 10U) & 1U;
    (void)ExecuteAicaCommand(firstTable ? QUICK_ICON_FIRST_SOUND[variant] : QUICK_ICON_SECOND_SOUND[variant], 0, 0);
}

void UpdateQuickIconBlink() {
    quickIconPreviousDrawFrames = quickIconDrawFrames;
    if (quickIconPhase != 3)
        return;
    quickIconDrawFrames += shenGetScaledDeltaTickTime(1.0f);
    if (quickIconUnpressed == 1) {
        if (quickIconDrawFrames >= 3.0f) {
            quickIconUnpressed = false;
            quickIconDrawFrames = 0.0f;
        }
        if (quickIconPreviousDrawFrames < 1.0f && quickIconDrawFrames >= 1.0f && (quickIconFlags & 1U) != 0 && (quickIconFlags & 0x400U) == 0) {
            (void)ExecuteAicaCommand(0x700abU, 0, 0);
        }
    } else if (quickIconDrawFrames >= 2.0f) {
        quickIconUnpressed = true;
        quickIconDrawFrames = 0.0f;
        quickIconPreviousDrawFrames = 0.0f;
    }
}

void UpdateQuickIconSplitDirections() {
    const std::uint16_t pressed = GetPadControlData(1) & 0xf6c6U;
    std::uint16_t status{};
    if ((quickIconRequiredStatus & 0x40U) != 0) {
        status = quickIconDirectionalStatuses[pressed >> 12U] & 0xeeU;
        if ((pressed & 0x4000U) != 0)
            status = 64;
        if ((pressed & 0x8000U) != 0)
            status = 4;
    } else {
        status = quickIconDirectionalStatuses[pressed >> 12U] & 0xbbU;
        if ((pressed & 0x1000U) != 0)
            status = 1;
        if ((pressed & 0x2000U) != 0)
            status = 16;
    }
    if (g_forcePlayerGodmode != 0)
        status = quickIconRequiredStatus;
    if ((status & quickIconRequiredStatus) != 0) {
        quickIconCurrentStatus = static_cast<std::int16_t>(status | 0x2000U);
        PlayQuickIconResult((quickIconFlags & 8U) == 0);
        quickIconPhase = 4;
    } else if (quickIconMaximumFrames > 0.0f) {
        quickIconElapsedFrames += shenGetScaledDeltaTickTime(1.0f);
        if (quickIconElapsedFrames > quickIconMaximumFrames) {
            quickIconCurrentStatus = static_cast<std::int16_t>(0xa000U);
            quickIconTerminalStatus = static_cast<std::int16_t>(0xa000U);
            quickIconPhase = 4;
            quickIconMaximumFrames = 0.0f;
            PlayQuickIconResult((quickIconFlags & 8U) != 0);
            quickIconUnpressed = 1;
        }
    }
    UpdateQuickIconBlink();
}

void UpdateQuickIconSingleButton() {
    const std::uint16_t pressed = GetPadControlData(1) & quickIconButtonMask & 0xf6c6U;
    std::uint16_t status = quickIconDirectionalStatuses[pressed >> 12U];
    std::uint8_t faceButtonCount{};
    if ((pressed & 0x0400U) != 0) {
        ++faceButtonCount;
        status |= 0x0100U;
    }
    if ((pressed & 0x0200U) != 0) {
        ++faceButtonCount;
        status |= 0x0200U;
    }
    if ((pressed & 0x0002U) != 0) {
        ++faceButtonCount;
        status |= 0x0800U;
    }
    if ((pressed & 0x0004U) != 0) {
        ++faceButtonCount;
        status |= 0x0400U;
    }
    if ((status & 0x0f00U) == 0) {
        if (quickIconRequiredStatus == 1 && (pressed & 0x1000U) != 0)
            status = 1;
        else if (quickIconRequiredStatus == 16 && (pressed & 0x2000U) != 0)
            status = 16;
        else if (quickIconRequiredStatus == 64 && (pressed & 0x4000U) != 0)
            status = 64;
        else if (quickIconRequiredStatus == 4 && (pressed & 0x8000U) != 0)
            status = 4;
    }
    if (g_forcePlayerGodmode != 0)
        status = quickIconRequiredStatus;
    if (status != 0) {
        quickIconCurrentStatus = static_cast<std::int16_t>(status);
        const bool invalid = (quickIconFlags & 0x40U) != 0 || faceButtonCount >= 2 || ((status & 0x0f00U) != 0 && static_cast<std::uint8_t>(status) != 0);
        if (invalid) {
            PlayQuickIconResult((quickIconFlags & 8U) != 0);
            quickIconCurrentStatus = 0;
        } else if (status == quickIconRequiredStatus) {
            quickIconCurrentStatus = static_cast<std::int16_t>(status | 0x2000U);
            PlayQuickIconResult((quickIconFlags & 8U) == 0);
        } else {
            quickIconCurrentStatus = 0;
            PlayQuickIconResult((quickIconFlags & 8U) != 0);
        }
        quickIconPhase = 4;
        quickIconUnpressed = 1;
    } else if (quickIconMaximumFrames > 0.0f) {
        quickIconElapsedFrames += shenGetScaledDeltaTickTime(1.0f);
        if (quickIconElapsedFrames > quickIconMaximumFrames) {
            quickIconPhase = 4;
            quickIconCurrentStatus = static_cast<std::int16_t>(0xa000U);
            quickIconTerminalStatus = static_cast<std::int16_t>(0xa000U);
            quickIconMaximumFrames = 0.0f;
            const bool strict = (quickIconFlags & 0x40U) != 0;
            PlayQuickIconResult(strict ? (quickIconFlags & 8U) == 0 : (quickIconFlags & 8U) != 0);
            quickIconUnpressed = 1;
        }
    }
    UpdateQuickIconBlink();
}

void RetireQuickIcon(bool resetPosition) {
    quickIconPhase = 0;
    padcRefreshCurrentInputs(false);
    ToggleRenderIcons(RenderIconSlot::Qte, false);
    iconToggleStateFor(3, false);
    if (resetPosition)
        quickIconPosition[0] = -176.0f;
}

void DrawQuickIcon(void *) {
    quickIconSprite.x = quickIconPosition[0];
    if (quickIconFlags & 0x20U) {
        for (std::int8_t bit = 0; bit < 8; ++bit) {
            if (!(quickIconRequiredStatus & (1U << bit)))
                continue;
            quickIconSprite.rotation = static_cast<std::uint32_t>(bit) << 13U;
            quickIconSprite.x = FixedAngleSin(static_cast<std::uint16_t>(quickIconSprite.rotation)) * 55.0f + quickIconPosition[0];
            quickIconSprite.y = quickIconPosition[1] - FixedAngleCos(static_cast<std::uint16_t>(quickIconSprite.rotation)) * 55.0f;
            PrepareDrawWithMode(quickIconSprite, ScreenMode::AdjustedWithOffset);
        }
    } else
        PrepareDrawWithMode(quickIconSprite, ScreenMode::Centered);
}

void CtrlQuickIcon_Callback(void *) {
    switch (quickIconPhase) {
    case 1:
        quickIconPhase = 3;
        break;
    case 2:
        if ((quickIconFlags & 1U) != 0 && (quickIconFlags & 0x400U) == 0)
            (void)ExecuteAicaCommand(0x700abU, 0, 0);
        quickIconPhase = 1;
        if ((quickIconFlags & 0x200U) == 0)
            padcRefreshCurrentInputs(true);
        iconToggleStateFor(3, true);
        quickIconPosition[0] = 320.0f;
        quickIconTerminalStatus = -1;
        quickIconUnpressed = true;
        break;
    case 3:
        if ((quickIconFlags & 0x100U) != 0)
            UpdateQuickIconBlink();
        else if ((quickIconFlags & 0x20U) != 0)
            UpdateQuickIconSplitDirections();
        else
            UpdateQuickIconSingleButton();
        break;
    case 4:
        if ((quickIconFlags & 0x10U) != 0)
            RetireQuickIcon(true);
        else {
            quickIconPhase = 5;
            if ((quickIconFlags & 4U) == 0 || quickIconTerminalStatus == static_cast<std::int16_t>(0xa000U))
                BeginBattleQteDeparture();
        }
        break;
    case 5:
        if (((quickIconFlags & 4U) != 0 && quickIconTerminalStatus != static_cast<std::int16_t>(0xa000U)) || !IsBattleQteTransitionActive())
            RetireQuickIcon(false);
        break;
    default:
        break;
    }
    if (GetDebugForceUIEnabled(UIElement::GameQte)) {
        if ((quickIconFlags & 2U) != 0 && GetItemSelectionMode(3) != 0) {
            SetupBattleQteIcon(quickIconRequiredStatus, static_cast<std::uint8_t>(quickIconFlags));
            if (quickIconUnpressed)
                EnableElement(UIElement::GameQte);
        }
    } else if ((quickIconFlags & 2U) != 0 && GetItemSelectionMode(3) != 0 && quickIconUnpressed) {
        HLib::ExecuteCallbackNow(DrawQuickIcon, nullptr);
    }
}

void InitQuickIcon_Callback(void *) {
    quickIconPositionFlags = 0;
    quickIconSprite.height = 64;
    quickIconLegacyTexture = -1;
    quickIconPosition = QUICK_ICON_POSITION;
    quickIconSprite.flags = 0;
    quickIconSprite.rotation = 0;
    quickIconLegacyReserved = 0;
    quickIconLoadingReleaseFlag = 0;
    quickIconLegacyWidth = 32;
    quickIconSprite.x = -176.0f;
    quickIconSprite.y = 240.0f;
    quickIconPhase = 0;
    quickIconPositionDepth = 3.0f;
    quickIconSprite.icon = 0x700;
    quickIconSprite.width = 64;
    quickIconSprite.scaleX = 1.0f;
    quickIconSprite.scaleY = 1.0f;
    quickIconSprite.depth = 3.0f;
    quickIconSprite.alignment = 5;
    quickIconLegacyMode = 4;
    HLib::SetCurrentTaskTo(CtrlQuickIcon_Callback);
}

// @verified - p
void DrawItemEffectWindow(std::int32_t columns, float x, float y, float depth) {
    const ScreenMode mode = static_cast<ScreenMode>(0);
    DrawLegacyWindowHorizontal(x, y, columns, 1, 0, depth, 1024, mode);
    DrawLegacyWindowHorizontal(x, y + 48.0f, columns, 1, 1, depth, 1024, mode);
    DrawLegacyWindowTiles((x + 24.0f) + 11.0f, y + 24.0f, columns, 1, depth - 0.02f, 1024, mode);
    DrawLegacyWindowBackground((x + 11.0f) + 4.0f, y + 4.0f, static_cast<float>(columns) * 40.0f, 40.0f, depth - 0.01f, 1024, mode);
}

void ItemEffectRender_Callback(void *) {
    std::int32_t columns = itemEffectColumns;
    if (itemEffectLabelTexture <= -1)
        return;
    itemEffectSprite.x = itemEffectPosition.x;
    itemEffectSprite.y = itemEffectPosition.y;
    PrepareDrawWithMode(itemEffectSprite, static_cast<ScreenMode>(0));
    itemEffectSprite.x += -33.0f;
    if (itemEffectAmount > 1)
        ++columns;
    if (itemEffectAmount >= 100)
        ++columns;
    DrawItemEffectWindow(columns, itemEffectSprite.x, itemEffectSprite.y - 25.0f, 0.3f);
    if (itemEffectLabelTexture != -1) {
        LegacySpritePacket packet{};
        packet.quad.flags = 1024;
        packet.colour = 0xcdcdcdcdU;
        packet.secondaryColour = 0xff000000U;
        packet.uv = {0.0f, 0.0f, 1.0f, 1.0f};
        const float x = ((static_cast<float>(20 * itemEffectColumns - 20) + itemEffectSprite.x) + 40.0f);
        TransformScreenSpace(packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(x), TruncateSpriteCoordinate(itemEffectSprite.y), 512,
            32, 1.0f, 1.0f, 0, static_cast<ScreenMode>(0));
        LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int32_t>(itemEffectLabelTexture)), 0.3f);
    }
    if (itemEffectAmount > 1)
        DrawLegacyItemQuantity(static_cast<float>(40 * columns) + itemEffectSprite.x, itemEffectSprite.y, itemEffectAmount, 0.4f);
}

void CtrlItemEffectIcon_Callback(void *) {
    switch (itemEffectIconPhase) {
    case 0:
        break;
    case 1:
        if (GetDebugForceUIEnabled(UIElement::GameMessage)) {
            if (!MoneyMessageTransitionBusy())
                itemEffectIconPhase = 3;
        } else if (AdvanceIconPosition(itemEffectPosition.x, itemEffectPosition.velocity, 50.0f))
            itemEffectIconPhase = 3;
        break;
    case 2:
        ShowElementByIndex(UIElement::GameMessage);
        itemEffectIconPhase = 1;
        itemEffectAudio = shenPlayAudioWithFlags("JINGURU01", 2);
        break;
    case 3:
        itemEffectIconFrames = static_cast<std::int8_t>(static_cast<std::uint8_t>(itemEffectIconFrames) + 1U);
        if (itemEffectIconFrames >= 75) {
            itemEffectIconPhase = 4;
            itemEffectIconFrames = 0;
            if (GetDebugForceUIEnabled(UIElement::GameMessage))
                DestroyElementByIndex(UIElement::GameMessage);
        }
        break;
    case 4:
        if (GetDebugForceUIEnabled(UIElement::GameMessage)) {
            if (!MoneyMessageTransitionBusy())
                itemEffectIconPhase = 5;
        } else if (RetreatIconPosition(itemEffectPosition.x, itemEffectPosition.velocity, -672.0f))
            itemEffectIconPhase = 5;
        break;
    case 5:
        itemEffectPosition.x = -672.0f;
        itemEffectPosition.y = 55.0f;
        itemEffectIconPhase = 0;
        itemEffectLabelTexture = -1;
        ToggleRenderIcons(RenderIconSlot::ItemEffectMessage, false);
        iconToggleStateFor(5, false);
        if (itemEffectIconTask)
            HLib::SetupTimedTask(itemEffectIconTask, 0);
        break;
    default:
        itemEffectIconPhase = 5;
        break;
    }
    if (!GetItemSelectionMode(5))
        return;
    if (GetDebugForceUIEnabled(UIElement::GameMessage))
        EnableElement(UIElement::GameMessage);
    else
        HLib::ExecuteCallbackNow(ItemEffectRender_Callback, nullptr);
}

void PrepareSleepActionLabels() {
    dialogueSelectionPrompt.flagIds = {0xffff, 481, 482, 0xffff};
    if (sleepActionFlags & 1)
        dialogueSelectionPrompt.flagIds[0] = 484;
    if (sleepActionFlags & 0x100)
        dialogueSelectionPrompt.flagIds[3] = 483;
    numericActionSelector.mode = 0;
    dialogueSelectionPrompt.textAvailable = false;
    for (std::size_t slot = 0; slot < 4; ++slot) {
        const std::uint16_t label = dialogueSelectionPrompt.flagIds[slot];
        PublishActionSelectorText(slot, label == 0xffff ? std::string_view{} : GetActionSelectorLabel(label));
        dialogueSelectionPrompt.textAvailable |= !dialogueSelectionPrompt.labels[slot].empty();
    }
    dialogueSelectionPrompt.focusedSlot = -1;
    dialogueSelectionPrompt.active = true;
}

// @verified - p
void ChooseSleepActionText(std::int32_t flags) {
    for (std::size_t slot = 0; slot < 4; ++slot) {
        LegacyIconSprite &sprite = sleepActionTextSprites[slot];
        if (sprite.icon != 0xffffU) {
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(sprite.icon));
            sprite.icon = 0xffffU;
        }
        if (!(flags & (1U << SLEEP_TEXT_FLAG_BITS[slot])))
            continue;
        sprite.icon = static_cast<std::uint16_t>(AcquireCtrlSpeakTextHandle(SLEEP_TEXT_NAMES[slot]));
    }
}

// @verified - p
void ReleaseSleepActionLabels() {
    for (LegacyIconSprite &sprite : sleepActionTextSprites) {
        if (sprite.icon != 0xffffU)
            ReleaseCtrlSpeakTextHandle(static_cast<std::int16_t>(sprite.icon));
        sprite.icon = 0xffffU;
    }
}

std::uint8_t GetSleepActionInput(std::uint16_t pressed) {
    if (!(sleepActionFlags & 1))
        pressed &= 0xefffU;
    if (!(sleepActionFlags & 2))
        pressed &= 0xbfffU;
    if (!(sleepActionFlags & 8))
        pressed &= 0x7fffU;
    if (!(sleepActionFlags & 0x100))
        pressed &= 0xddffU;
    return DecodeSelectorInput(pressed, 0x55);
}

void PrepareDrawWithUVOffsets(const LegacyIconSprite &sprite, float u, float v, float width, float height, std::uint32_t colour, ScreenMode mode) {
    const std::array<std::uint8_t, 4> uv = GetCommonIconRect(sprite.icon);
    const CommonIconAtlas *const atlas = GetCommonIconAtlas(sprite.icon);
    if (!atlas->textureHandle || atlas->textureHandle == 0xffffffffU)
        return;
    LegacySpritePacket packet{.quad = {.flags = 0}, .colour = colour | 0x00ffffffU, .secondaryColour = colour & 0x00ffffffU, .reservedTail = 0};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(sprite.alignment), TruncateSpriteCoordinate(sprite.x), TruncateSpriteCoordinate(sprite.y),
        sprite.width, sprite.height, sprite.scaleX, sprite.scaleY, std::bit_cast<std::int32_t>(sprite.rotation), mode);
    packet.uv = {static_cast<float>(static_cast<std::int8_t>(uv[0])) * 0.125f + u, static_cast<float>(static_cast<std::int8_t>(uv[1])) * 0.125f + v,
        static_cast<float>(static_cast<std::int8_t>(uv[2])) * 0.125f + width, static_cast<float>(static_cast<std::int8_t>(uv[3])) * 0.125f + height};
    packet.quad.flags |= sprite.flags | 0x400U;
    LegacySpriteNullsub(packet, static_cast<std::uint16_t>(atlas->textureHandle), sprite.depth);
}

void DrawActionSelectorTextFrame(const Vector2 &position, float depth, std::int32_t width, std::int16_t height, std::uint8_t selected) {
    ctrlSpeakSelectedFrameIds = ACTION_SELECTOR_FRAME_IDS.data() + 2 * (selected & 1);
    const std::int32_t edge = width <= 32 ? width / 2 : 16;
    const std::int32_t centre = width - 2 * edge;
    const float halfCentre = static_cast<float>(centre / 2), halfEdge = static_cast<float>(edge / 2);
    LegacyIconSprite sprite{.x = (position.x - halfCentre) - halfEdge,
        .y = position.y,
        .scaleX = 1.0f,
        .scaleY = 1.0f,
        .depth = depth,
        .flags = 0x400,
        .rotation = 0,
        .icon = ctrlSpeakSelectedFrameIds[1],
        .width = static_cast<std::int16_t>(edge),
        .height = height,
        .alignment = 5};
    PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
    sprite.flags = 0x500;
    sprite.x = (halfCentre + position.x) + halfEdge;
    PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
    sprite.icon = ctrlSpeakSelectedFrameIds[0];
    sprite.depth = depth - 0.05f;
    sprite.x = position.x;
    sprite.width = static_cast<std::int16_t>(centre);
    sprite.flags = 0x400;
    PrepareDrawWithUVOffsets(sprite, 0.0f, 0.0f, -0.005f, 0.0f, 0xff000000U, static_cast<ScreenMode>(0));
}

// @verified - p
void DrawActionSelectorRadial(const Vector2 &position, const std::array<std::int32_t, 4> &handles, float depth, float scale,
    const std::array<std::int16_t, 4> &widths, std::int32_t selected, std::int32_t marker, std::int32_t forceText) {
    ctrlSpeakRadialFrameIds[1] = ACTION_SELECTOR_MARKERS[marker];
    const std::int16_t extent = static_cast<std::int16_t>(static_cast<std::int32_t>((scale + 0.5f) * 64.0f));
    LegacyIconSprite sprite{.x = position.x,
        .y = position.y,
        .scaleX = 1.0f,
        .scaleY = 1.0f,
        .depth = depth,
        .flags = 0,
        .rotation = 0,
        .icon = 1280,
        .width = extent,
        .height = extent,
        .alignment = 5};
    PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
    sprite.width = sprite.height = 40;
    sprite.depth = depth + 3.0f;
    for (std::size_t ring = 0; ring < 3; ++ring) {
        sprite.scaleX = ring == 1 ? cos_fixed16(ctrlSpeakPulseAngle) : 1.0f;
        sprite.icon = ctrlSpeakRadialFrameIds[ring];
        PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
        sprite.depth += -0.5f;
    }
    LegacyIconSprite &textSprite = ctrlSpeakChoiceDrawSprites[0];
    const float retainedScale = textSprite.scaleX;
    sprite.icon = 1285;
    sprite.scaleX = sprite.scaleY = 1.0f;
    sprite.width = sprite.height = 32;
    sprite.depth = depth;
    const std::int32_t inset = static_cast<std::int32_t>(scale * 3.0f);
    for (std::size_t slot = 0; slot < 4; ++slot) {
        textSprite.scaleX = textSprite.scaleY = scale;
        if (handles[slot] != -1) {
            const float x = ACTION_SELECTOR_RADIAL_X[slot], y = ACTION_SELECTOR_RADIAL_Y[slot];
            sprite.flags = 0;
            sprite.x = x * 16.0f + position.x;
            sprite.y = y * 16.0f + position.y;
            sprite.depth = depth + 0.2f;
            const float degrees = static_cast<float>(180 - 90 * static_cast<std::int32_t>(slot));
            sprite.rotation = static_cast<std::uint16_t>(static_cast<std::int32_t>(degrees * 8192.0f / 45.0f));
            PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
            const float halfHeight = (scale * 36.0f) * 0.5f, scaledWidth = static_cast<float>(widths[slot]) * scale;
            const float halfWidth = scaledWidth * 0.5f;
            const float radialY = (halfHeight + 24.0f) - static_cast<float>(inset);
            const float radialX = (halfWidth + 24.0f) - static_cast<float>(inset);
            const float textX = x * radialX + position.x, textY = radialY * y + position.y;
            DrawActionSelectorTextFrame({textX, textY}, depth + 0.1f, static_cast<std::int32_t>(scaledWidth),
                static_cast<std::int16_t>(static_cast<std::int32_t>(scale * 64.0f)), static_cast<std::int32_t>(slot) == selected ? 1 : 0);
            textSprite.x = textX;
            textSprite.y = textY;
            sprite.x = (((halfWidth + radialX) + 8.0f) - static_cast<float>(inset)) * x + position.x;
            sprite.y = (((halfHeight + radialY) + 8.0f) - static_cast<float>(inset)) * y + position.y;
            sprite.depth = depth + 0.2f;
            sprite.flags = 0x200;
            PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
            std::uint16_t glyph = 0;
            if (!forceText) {
                for (const std::array<std::int32_t, 2> &label : CTRL_SPEAK_SPECIAL_LABELS) {
                    if (static_cast<std::uint16_t>(ctrlSpeakChoiceIds[slot]) == label[0]) {
                        glyph = static_cast<std::uint16_t>(label[1]);
                        break;
                    }
                }
                if (glyph)
                    ctrlSpeakHasSpecialLabel = true;
            }
            if (forceText || ctrlSpeakForceTextLabels || !glyph) {
                textSprite.icon = static_cast<std::uint16_t>(handles[slot]);
                PrepareDrawTextureWithMode(textSprite, static_cast<ScreenMode>(0));
            } else {
                LegacyIconSprite special{.x = textSprite.x,
                    .y = textSprite.y,
                    .scaleX = textSprite.scaleX + textSprite.scaleX,
                    .scaleY = textSprite.scaleY * ctrlSpeakSpecialLabelWidthScale,
                    .depth = textSprite.depth + 0.2f,
                    .flags = 0,
                    .rotation = 0,
                    .icon = glyph,
                    .width = 32,
                    .height = 32,
                    .alignment = 5};
                PrepareDrawWithMode(special, static_cast<ScreenMode>(0));
            }
        }
        textSprite.scaleX = textSprite.scaleY = retainedScale;
    }
}

// @verified - p
void DrawCtrlSpeakSelector(void *) {
    const float offset = ctrlSpeakChoiceTextHandles[0] == -1 && ctrlSpeakChoiceTextHandles[2] != -1 ? -18.0f : 0.0f;
    const std::int32_t selected = ctrlSpeakSelectionAnimationFrame % 5 == 0 ? static_cast<std::int16_t>(ctrlSpeakSelectedChoiceSlot) : 4;
    DrawActionSelectorRadial({ctrlSpeakRightOriginX, ctrlSpeakSelectorOrigin[1] + offset}, ctrlSpeakChoiceTextHandles, 1.0f, ctrlSpeakRadialScale,
        ctrlSpeakChoiceWidths, selected, ctrlSpeakMarkerGlyphIndex, 0);
}

void DrawSleepActionSelector(void *) {
    if (GetDebugForceUIEnabled(UIElement::ActionSelector)) {
        EnableElement(UIElement::ActionSelector);
        ExecuteElementCallback(UIElement::ActionSelector);
        return;
    }
    std::array<std::int32_t, 4> handles;
    for (std::size_t slot = 0; slot < 4; ++slot)
        handles[slot] = (sleepActionFlags & (1U << SLEEP_TEXT_FLAG_BITS[slot])) ? static_cast<std::int16_t>(sleepActionTextSprites[slot].icon) : -1;
    const std::int32_t selected = sleepActionRetargetFrames % 5 == 0 ? sleepActionCurrentSelection : sleepActionNoSelection;
    DrawActionSelectorRadial(
        {sleepActionPosition[0], sleepActionPosition[1]}, handles, sleepActionPosition[2], sleepActionScale, SLEEP_LABEL_WIDTHS, selected, 2, 1);
}

void CtrlSleepActionIcon_Callback(void *) {
    if (sleepActionSubmenuState == 1 && GetDebugForceUIEnabled(UIElement::LoadSave) && (g_inputBuffer[0].pressedPriority & 0x200U)) {
        g_inputBuffer[0].pressedPriority = 0;
        DestroyElementByIndex(UIElement::LoadSave);
        DestroyElementByIndex(UIElement::GameBackground);
        DestroyElementByIndex(UIElement::GameInfoBar);
    } else if (sleepActionSubmenuState == 2 && GetDebugForceUIEnabled(UIElement::SubMenuOptions) && (g_inputBuffer[0].pressedPriority & 0x200U) &&
        !AskKeepOrRevertChanges()) {
        sleepActionPhase &= 0xff00U;
        sleepActionStatus = static_cast<std::uint16_t>((sleepActionStatus & 0xff00U) | 1U);
        sleepActionSubmenuState = 0;
        g_inputBuffer[0].pressedPriority = 0;
        DestroyElementByIndex(UIElement::SubMenuOptions);
        DestroyElementByIndex(UIElement::GameBackground);
        DestroyElementByIndex(UIElement::GameInfoBar);
    }
    const std::uint16_t pressed = ctrlGetPadInputBuffer0(sleepActionPadControl)->pressedPriority;
    switch (static_cast<std::int8_t>(sleepActionPhase)) {
    case 0:
        if (!(sleepActionStatus & 0xffU))
            break;
        if (GetDebugForceUIEnabled(UIElement::ActionSelector)) {
            PrepareSleepActionLabels();
            ShowElementByIndex(UIElement::ActionSelector);
        }
        ResetHelpIconsToItemPrompt();
        CheckHelpIcon((static_cast<std::uint32_t>(sleepActionFlags) >> 7U & 2U) | 1U, 0, CheckHelpIconOp::Add);
        sleepActionPreviousPadControl = BindPadControlEntry(sleepActionPadControl);
        sleepActionPhase = static_cast<std::uint16_t>((sleepActionPhase & 0xff00U) | 1U);
        sleepActionTargetSelection = sleepActionNoSelection;
        iconToggleStateFor(6, true);
        ChooseSleepActionText(sleepActionFlags & 0x1b);
        break;
    case 1:
        sleepActionPhase = static_cast<std::uint16_t>((sleepActionPhase & 0xff00U) | 2U);
        break;
    case 2: {
        const std::uint8_t input = GetSleepActionInput(pressed);
        if (input == 0 || input == 2 || input == 6) {
            sleepActionPhase = static_cast<std::uint16_t>((sleepActionPhase & 0xff00U) | 3U);
            if (GetDebugForceUIEnabled(UIElement::ActionSelector))
                DestroyElementByIndex(UIElement::ActionSelector);
            sleepActionRetargetFrames = -1;
            sleepActionCurrentSelection = sleepActionTargetSelection = ACTION_SELECTOR_DIRECTION_TO_SLOT[input];
            sleepActionArrowSprites[sleepActionCurrentSelection].depth = 5.0f;
            sleepActionTextSprites[sleepActionCurrentSelection].depth = 6.0f;
        } else if (input == 4 || input == 9) {
            if (input == 4) {
                sleepActionRetargetFrames = -1;
                sleepActionCurrentSelection = ACTION_SELECTOR_DIRECTION_TO_SLOT[input];
                sleepActionArrowSprites[sleepActionCurrentSelection].depth = 5.0f;
                sleepActionTextSprites[sleepActionCurrentSelection].depth = 6.0f;
            }
            sleepActionPhase = static_cast<std::uint16_t>((sleepActionPhase & 0xff00U) | 3U);
            SelectActionSelectorButton(3);
            if (GetDebugForceUIEnabled(UIElement::ActionSelector))
                DestroyElementByIndex(UIElement::ActionSelector);
        }
        break;
    }
    case 3:
        if (sleepActionCurrentSelection == sleepActionNoSelection && !(ActionSelectorPosition() > 0.0f)) {
            ResetHelpIconsToItemPrompt();
            sleepActionPhase = static_cast<std::uint16_t>((sleepActionPhase & 0xff00U) | (sleepActionTargetSelection == 4 ? 6U : 4U));
        }
        break;
    case 4:
        sleepActionPhase &= 0xff00U;
        ToggleRenderIcons(RenderIconSlot::SleepAction, false);
        ReleaseSleepActionLabels();
        BindPadControlEntry(sleepActionPreviousPadControl);
        iconToggleStateFor(6, false);
        sleepActionPosition[0] = -176.0f;
        sleepActionStatus &= 0xff00U;
        switch (sleepActionTargetSelection) {
        case 0:
            SelectActionSelectorButton(0);
            sleepActionStatus = static_cast<std::uint16_t>((sleepActionStatus & 0x00ffU) | 0x0200U);
            ResetHelpIconsToItemPrompt();
            sleepActionPublishedSelection = 0;
            break;
        case 1:
            SelectActionSelectorButton(1);
            EnableSaveScreenAdmission();
            sleepActionSubmenuState = 1;
            ShowElementByIndex(UIElement::GameBackground);
            SetSaveFrontendMode(1);
            ShowElementByIndex(UIElement::LoadSave);
            ShowElementByIndex(UIElement::GameInfoBar);
            break;
        case 2:
            SelectActionSelectorButton(3);
            break;
        case 3:
            SelectActionSelectorButton(2);
            options.cameraRequested = true;
            ShowElementByIndex(UIElement::GameBackground);
            ShowElementByIndex(UIElement::SubMenuOptions);
            ShowElementByIndex(UIElement::GameInfoBar);
            sleepActionSubmenuState = 2;
            break;
        default:
            break;
        }
        break;
    case 6:
        sleepActionPhase &= 0xff00U;
        sleepActionScale = 0.0f;
        ToggleRenderIcons(RenderIconSlot::SleepAction, false);
        iconToggleStateFor(6, false);
        ReleaseSleepActionLabels();
        BindPadControlEntry(sleepActionPreviousPadControl);
        sleepActionStatus = 0x0100U;
        sleepActionPublishedSelection = sleepActionNoSelection;
        break;
    default:
        break;
    }
    if (GetItemSelectionMode(6)) {
        if (sleepActionCurrentSelection != sleepActionNoSelection) {
            sleepActionRetargetFrames = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(sleepActionRetargetFrames) + 1U);
            if (sleepActionRetargetFrames >= 20) {
                sleepActionCurrentSelection = sleepActionNoSelection;
                sleepActionRetargetFrames = 20;
            }
        }
        HLib::ExecuteCallbackNow(DrawSleepActionSelector, nullptr);
    }
}
} // namespace ui_detail

void DrawMoneyDigits(float x, float y, float scale, float depth, std::int32_t amount, std::uint32_t colour, ScreenMode mode) {
    const float advance = static_cast<float>(static_cast<std::int16_t>(TruncateSpriteCoordinate(scale * 16.0f)));
    float currentX = x - 7.0f * advance;
    bool significant = false;
    for (std::int32_t index = 0; index < 8; ++index) {
        LegacyIconSprite &sprite = selectorNumberSprites[index];
        sprite.x = currentX;
        sprite.y = y;
        const std::int32_t digit = amount / moneyIconIncrements[index];
        amount %= moneyIconIncrements[index];
        sprite.depth = depth;
        sprite.scaleX = sprite.scaleY = scale;
        currentX += advance;
        if (digit != 0)
            significant = true;
        sprite.icon = SELECTOR_DIGIT_ICONS[digit];
        if (significant || index == 7)
            PrepareDrawSimple(sprite, colour, mode);
    }
}
void DrawMoneyAmount(float x, float y, float depth, float scale, std::int32_t amount, std::uint8_t cash, std::uint32_t colour, ScreenMode mode) {
    const float work[4]{x, y, depth, scale};
    DrawSelectorCash(work, amount, cash, colour, 0, mode);
}

static void WriteGuiDds(std::byte *buffer, std::uint32_t width, std::uint32_t height, std::uint32_t pixelBytes, std::uint8_t format) {
    std::array<std::uint32_t, 32> dds{};
    dds[0] = make_shen_name('D', 'D', 'S', ' ');
    dds[1] = 124;
    dds[2] = 0x1007;
    dds[3] = height;
    dds[4] = width;
    dds[27] = 0x1000;
    const std::array<std::uint32_t, 8> *pixelFormat = nullptr;
    switch (format) {
    case 1:
        pixelFormat = &DDS_PIXEL_RGB565;
        dds[2] = 0x100f;
        dds[5] = 2 * width;
        break;
    case 2:
    case 133:
        pixelFormat = &DDS_PIXEL_ARGB4444;
        dds[2] = 0x100f;
        dds[5] = 2 * width;
        break;
    case 128:
        pixelFormat = &DDS_PIXEL_DXT1;
        dds[2] = 0x81007;
        dds[5] = pixelBytes;
        break;
    case 129:
        pixelFormat = &DDS_PIXEL_DXT3;
        dds[2] = 0x81007;
        dds[5] = pixelBytes;
        break;
    case 130:
        pixelFormat = &DDS_PIXEL_DXT5;
        dds[2] = 0x81007;
        dds[5] = pixelBytes;
        break;
    case 131:
    case 137:
        pixelFormat = &DDS_PIXEL_BGRA8;
        dds[2] = 0x100f;
        dds[5] = 4 * width;
        break;
    default:
        break;
    }
    if (pixelFormat)
        std::memcpy(dds.data() + 19, pixelFormat->data(), 32);
    std::memcpy(buffer, dds.data(), sizeof(dds));
}

void ParseGuiPtx(std::byte *buffer, std::uint16_t width, std::uint16_t height, std::uint32_t idLow, std::uint32_t idHigh, std::uint16_t format) {
    const std::uint32_t storageBytes = GetTextureStorageSize(format, width, height);
    const std::uint32_t pixelBytes = (format & 0xff00U) == 0x8000U ? storageBytes - 128U : storageBytes;
    const GuiPtxHeader header{.tag = SHEN_NAME(TEXN),
        .size = storageBytes + 48U,
        .idLow = idLow,
        .idHigh = idHigh,
        .gbixTag = SHEN_NAME(GBIX),
        .gbixSize = 4,
        .pvrTag = SHEN_NAME(PVRT),
        .pvrSize = pixelBytes + 8,
        .format = format,
        .width = width,
        .height = height};
    std::memcpy(buffer, &header, sizeof(header));
    if ((format & 0xff00U) == 0x8000U)
        WriteGuiDds(buffer + sizeof(header), width, height, pixelBytes, static_cast<std::uint8_t>(format));
}

float GetScreenAlignmentOffset() noexcept {
    const float aspect = static_cast<float>(d3t::g_originalScreenWidth) / static_cast<float>(d3t::g_originalScreenHeight);
    return ((1.0f - (1.3333334f / aspect)) * 640.0f) * 0.5f;
}

void InitializeLegacyIconSprite(LegacyIconSprite &sprite) noexcept {
    sprite.x = 0.0f;
    sprite.y = 0.0f;
    sprite.scaleX = 1.0f;
    sprite.scaleY = 1.0f;
    sprite.depth = 1.0f;
    sprite.flags = 0;
    sprite.rotation = 0;
    sprite.icon = 0;
    sprite.width = 32;
    sprite.height = 32;
    sprite.alignment = 5;
}

std::uint8_t DecodeSelectorInput(std::uint16_t pressed, std::uint8_t allowedDirections) noexcept {
    const std::uint8_t direction = selectorDirectionIcons[pressed >> 12U];
    for (std::uint8_t index = 0; index < 12; ++index) {
        if ((allowedDirections & 1U) != 0 && direction == index)
            return direction;
        allowedDirections >>= 1U;
    }
    if ((pressed & 0x0200U) != 0)
        return 9;
    if ((pressed & 0x0400U) != 0)
        return 8;
    if ((pressed & 0x0002U) != 0)
        return 11;
    return (pressed & 0x0004U) != 0 ? 10 : 13;
}

bool AdvanceIconPosition(float &position, float &velocity, float target) noexcept {
    float distance = target - position;
    if (distance < 0.0f)
        distance = -distance;
    distance *= 0.5f;
    velocity = distance <= 64.0f ? distance : velocity + velocity;
    if (velocity >= 1.0f) {
        if (velocity > 64.0f)
            velocity = 64.0f;
    } else
        velocity = 1.0f;
    position += velocity;
    if (position < target)
        return false;
    position = target;
    return true;
}

bool RetreatIconPosition(float &position, float &velocity, float target) noexcept {
    float distance = target - position;
    if (distance < 0.0f)
        distance = -distance;
    distance *= 0.5f;
    velocity = distance <= 64.0f ? distance : velocity + velocity;
    if (velocity >= 1.0f) {
        if (velocity > 64.0f)
            velocity = 64.0f;
    } else
        velocity = 1.0f;
    position -= velocity;
    if (target < position)
        return false;
    position = target;
    return true;
}

std::uint32_t GetTextureStorageSize(std::uint16_t format, std::int32_t width, std::int32_t height) noexcept {
    const std::uint32_t pixels = static_cast<std::uint32_t>(width) * static_cast<std::uint32_t>(height);
    switch (format & 0xff00U) {
    case 0x0100:
    case 0x0900:
        return 2U * pixels;
    case 0x0700:
        return pixels;
    case 0x0500:
        return static_cast<std::uint32_t>(std::bit_cast<std::int32_t>(pixels) / 2);
    case 0x0300:
        if (width != height)
            return 0;
        switch (width) {
        case 64:
            return 0x0c00;
        case 128:
            return 0x1800;
        case 256:
            return 0x4800;
        case 512:
            return 0x10800;
        default:
            return 0;
        }
    case 0x8000:
        break;
    default:
        return 0;
    }
    const std::uint32_t blocksX = static_cast<std::uint32_t>(width / 4 < 1 ? 1 : width / 4);
    const std::uint32_t blocksY = static_cast<std::uint32_t>(height / 4 < 1 ? 1 : height / 4);
    switch (format & 0xffU) {
    case 1:
    case 2:
    case 133:
        return 128U + 2U * pixels;
    case 128:
        return 128U + 8U * blocksX * blocksY;
    case 129:
    case 130:
        return 16U * (blocksX * blocksY + 8U);
    case 137:
        return 128U + 4U * pixels;
    default:
        return 128U;
    }
}

// helpWidgetResetRuns.
void HelpWidgetResetRuns() {
    helpWidgetRunCount = 0;
    helpWidgetLineCount = 0;
    helpWidgetBoxesLocked = false;
    helpWidgetIconRunCount = 0;
    helpWidgetBoxCount = 0;
    helpWidgetIconCount = 0;
}
namespace ui_detail {

void HelpWidgetCommitText(const std::array<char, 256> &pending, std::int32_t length, std::int32_t column) {
    HelpWidgetRun &run = helpWidgetRuns[static_cast<std::size_t>(helpWidgetRunCount++)];
    for (std::size_t index = 0; index < run.text.size(); ++index)
        run.text[index] = index < static_cast<std::size_t>(length) ? pending[index] : 0;
    run.length = length;
    run.column = column;
    run.line = helpWidgetLineCount;
    run.kind = 1;
}
} // namespace ui_detail

// helpWidgetParseLine: splits one help line into text runs at "\t" column tabs and
// button-icon runs at '^'; leading spaces of a run are dropped.
void HelpWidgetParseLine(std::string_view line) {
    const std::size_t terminator = line.find('\0');
    if (terminator != std::string_view::npos)
        line = line.substr(0, terminator);
    std::array<char, 256> pending{};
    std::int32_t pendingLength = 0;
    std::int32_t column = 0;
    std::size_t offset = 0;
    while (offset < line.size()) {
        std::uint32_t codepoint = 0;
        const std::size_t length = DecodeHelpCodepoint(line.substr(offset), codepoint);
        if (!length)
            break;
        if (codepoint == '\\' && offset + 1 < line.size() && line[offset + 1] == 't') {
            if (pendingLength > 0 && helpWidgetRunCount < 64)
                HelpWidgetCommitText(pending, pendingLength, column);
            pendingLength = 0;
            ++column;
            offset += 2;
            continue;
        }
        if (codepoint == '^') {
            if (pendingLength > 0 && helpWidgetRunCount < 64)
                HelpWidgetCommitText(pending, pendingLength, column);
            const std::int32_t icon = helpWidgetIconRunCount++;
            if (icon != -1 && helpWidgetRunCount < 64) {
                HelpWidgetRun &run = helpWidgetRuns[static_cast<std::size_t>(helpWidgetRunCount++)];
                run.column = column;
                run.line = helpWidgetLineCount;
                run.icon = icon;
                run.kind = 2;
            }
            pendingLength = 0;
            ++offset;
            continue;
        }
        if (pendingLength || codepoint != ' ') {
            std::memcpy(pending.data() + pendingLength, line.data() + offset, length);
            pendingLength += static_cast<std::int32_t>(length);
        }
        offset += length;
    }
    if (pendingLength > 0 && helpWidgetRunCount < 64)
        HelpWidgetCommitText(pending, pendingLength, column);
    ++helpWidgetLineCount;
    helpWidgetRelayout = true;
}

// helpWidgetAddButtonIcon.
void HelpWidgetAddButtonIcon(std::int32_t code) {
    HelpWidgetMakeButtonIcon(helpWidgetIcons[static_cast<std::size_t>(helpWidgetIconCount)], code);
    helpWidgetIconActions[static_cast<std::size_t>(helpWidgetIconCount++)] = UI_GetActionForId(code);
}

// FormatColoursForHelpTopic: the coloured boxes behind topic columns, as
// {colour, column, row, rows, width mode}.
void FormatColoursForHelpTopic(std::uint8_t topic) {
    using Box = HelpWidgetBoxDefinition;
    constexpr std::uint32_t red = HelpColour(140, 0, 0, 255);
    constexpr std::uint32_t green = HelpColour(0, 140, 0, 255);
    constexpr std::uint32_t blue = HelpColour(0, 0, 140, 255);
    constexpr std::uint32_t yellow = HelpColour(140, 140, 0, 255);
    auto add = [](std::initializer_list<Box> boxes) {
        for (const Box &box : boxes) {
            if (helpWidgetBoxesLocked)
                return;
            HelpWidgetBox &target = helpWidgetBoxes[static_cast<std::size_t>(helpWidgetBoxCount++)];
            target.colour = box.colour;
            target.column = box.column;
            target.row = box.row;
            target.rows = box.rows;
            target.widthMode = box.widthMode;
        }
    };
    switch (topic) {
    case 0x00:
    case 0x1e:
        add({{red, 0, 3, 3, 1}, {green, 1, 3, 3, 3}, {blue, 0, 7, 4, 1}, {yellow, 1, 7, 4, 3}});
        break;
    case 0x26:
    case 0x2b:
        add({{red, 0, 3, 3, 1}, {green, 1, 3, 3, 3}, {blue, 0, 7, 4, 1}, {yellow, 1, 7, 3, 3}});
        break;
    case 0x01:
        add({{green, 0, 7, 1, 1}, {blue, 1, 7, 2, 3}});
        break;
    case 0x02:
        add({{green, 0, 7, 2, 1}, {blue, 1, 7, 2, 3}});
        break;
    case 0x03:
    case 0x37:
        add({{green, 0, 9, 1, 1}});
        break;
    case 0x04:
    case 0x72:
        add({{green, 1, 6, 1, 0}});
        break;
    case 0x06:
        add({{green, 0, 7, 1, 1}, {blue, 1, 7, 4, 3}});
        break;
    case 0x07:
        add({{red, 0, 8, 1, 1}, {blue, 1, 8, 1, 3}, {green, 1, 10, 1, 3}});
        break;
    case 0x0a:
        add({{green, 1, 5, 3, 0}});
        break;
    case 0x0b:
    case 0x3d:
        add({{red, 0, 7, 1, 1}, {green, 1, 7, 2, 3}});
        break;
    case 0x0c:
        add({{red, 0, 3, 1, 1}, {green, 1, 3, 1, 3}});
        break;
    case 0x0e:
        add({{red, 0, 7, 1, 1}, {green, 1, 7, 4, 3}});
        break;
    case 0x0f:
        add({{blue, 0, 8, 2, 1}});
        break;
    case 0x10:
        add({{green, 1, 6, 2, 0}});
        break;
    case 0x11:
    case 0x4f:
    case 0x57:
        add({{red, 0, 7, 1, 1}, {green, 1, 7, 1, 3}});
        break;
    case 0x12:
    case 0x53:
        add({{blue, 0, 7, 3, 1}, {green, 1, 7, 1, 3}});
        break;
    case 0x13:
        add({{green, 1, 6, 4, 0}});
        break;
    case 0x14:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7a:
    case 0x7b:
    case 0x7c:
        add({{green, 1, 6, 3, 0}});
        break;
    case 0x16:
        add({{red, 1, 8, 3, 0}});
        break;
    case 0x17:
    case 0x35:
        add({{red, 1, 8, 2, 0}});
        break;
    case 0x18:
        add({{yellow, 1, 8, 1, 3}, {green, 0, 8, 1, 1}});
        break;
    case 0x19:
    case 0x4e:
        add({{red, 0, 6, 1, 1}, {green, 1, 6, 2, 3}});
        break;
    case 0x1b:
    case 0x23:
    case 0x42:
    case 0x70:
    case 0x73:
        add({{green, 1, 7, 1, 0}});
        break;
    case 0x1c:
        add({{green, 1, 9, 2, 3}, {red, 0, 9, 2, 1}});
        break;
    case 0x24:
    case 0x32:
        add({{red, 0, 7, 1, 1}});
        break;
    case 0x27:
        add({{green, 0, 8, 1, 1}});
        break;
    case 0x2a:
        add({{green, 1, 5, 2, 0}});
        break;
    case 0x2f:
    case 0x4c:
        add({{green, 1, 7, 4, 0}});
        break;
    case 0x30:
        add({{green, 0, 10, 1, 1}});
        break;
    case 0x31:
    case 0x55:
        add({{red, 0, 8, 1, 1}, {green, 1, 8, 2, 3}});
        break;
    case 0x33:
        add({{red, 0, 5, 2, 1}, {blue, 1, 5, 2, 3}});
        break;
    case 0x34:
        add({{red, 1, 8, 2, 2}});
        break;
    case 0x3e:
        add({{green, 1, 8, 2, 0}});
        break;
    case 0x43:
        add({{red, 0, 6, 2, 1}, {green, 1, 6, 1, 3}});
        break;
    case 0x50:
        add({{red, 0, 7, 2, 1}, {green, 1, 7, 1, 3}});
        break;
    case 0x51:
        add({{red, 1, 8, 1, 0}});
        break;
    case 0x59:
    case 0x5d:
    case 0x5f:
    case 0x61:
    case 0x63:
    case 0x64:
    case 0x6d:
    case 0x7d:
    case 0x7e:
    case 0x83:
    case 0x8b:
    case 0x8c:
    case 0x8d:
    case 0x8e:
    case 0x90:
        add({{green, 1, 7, 3, 0}});
        break;
    case 0x5a:
        add({{green, 1, 8, 1, 0}});
        break;
    case 0x5b:
    case 0x66:
    case 0x67:
    case 0x6a:
    case 0x7f:
    case 0x80:
    case 0x81:
    case 0x84:
    case 0x85:
    case 0x8f:
    case 0x95:
    case 0x96:
    case 0x97:
    case 0x98:
        add({{green, 1, 8, 3, 0}});
        break;
    case 0x86:
        add({{red, 1, 7, 3, 0}});
        break;
    case 0x87:
    case 0x89:
        add({{red, 1, 7, 2, 0}});
        break;
    case 0x88:
        add({{red, 1, 7, 2, 2}});
        break;
    case 0x94:
        add({{green, 1, 9, 2, 0}});
        break;
    case 0x99:
        add({{red, 0, 8, 1, 1}, {green, 1, 8, 1, 3}});
        break;
    case 0x9a:
        add({{red, 1, 6, 2, 0}, {green, 1, 9, 2, 0}});
        break;
    default:
        break;
    }
}

// @verified - p
void SetHelpIconState() noexcept {
    g_helpIconState = 1;
}

std::int32_t IsCameraIconHidden() noexcept {
    return currentCameraIconMode;
}



void SetDebugHideCameraIcon(std::int32_t hidden) noexcept {
    previousCameraIconMode = hidden;
}

void SetupCameraIcon() noexcept {
    std::int32_t resolvedMode = previousCameraIconMode;
    if (requestedCameraIconMode != 2)
        resolvedMode = requestedCameraIconMode;
    previousCameraIconMode = resolvedMode;
    if (resolvedMode == currentCameraIconMode)
        return;

    currentCameraIconMode = resolvedMode;
    for (UIElementState &element : g_uiElements) {
        if ((element.setup == 0 || element.config.init == 3) && element.unknown88) {
            element.unknown88();
        }
    }
}

// @verified - p
std::int32_t GetCurrentUIInputType() noexcept {
    constexpr std::uint32_t kCameraIconRange = 0x03ff0000U;
    constexpr std::uint32_t kAlwaysBlockedWord0 = 0x2c000020U;
    constexpr std::uint32_t kAlwaysBlockedWord1 = 0x30000000U;
    constexpr std::uint32_t kMenuWord0 = 0x0000fe40U;
    constexpr std::uint32_t kMenuWord1 = 0x0003f000U;
    constexpr std::uint32_t kCameraBlockedWord1 = 0x06000000U;
    constexpr std::uint32_t kCameraMenuWord1 = 0x06000080U;

    const std::array<std::uint32_t, 3> &input = g_uiPreviousInputElements;
    if ((input[0] & kAlwaysBlockedWord0) || (input[1] & kAlwaysBlockedWord1) ||
        (IsCameraIconHidden() == 0 && ((input[0] & kCameraIconRange) || (input[1] & kCameraBlockedWord1))))
        return static_cast<std::int32_t>(InputType::Menu);

    if ((input[0] & kMenuWord0) || (input[1] & kMenuWord1) || (IsCameraIconHidden() == 1 && ((input[0] & kCameraIconRange) || (input[1] & kCameraMenuWord1))))
        return static_cast<std::int32_t>(InputType::GameUi);

    constexpr std::uint32_t kHelpOverlay = 1U << 16U;
    if ((input[2] & kHelpOverlay) && IsGameUIActive())
        return static_cast<std::int32_t>(InputType::GameUi);

    constexpr std::uint32_t kQte2 = 1U << 22U;
    if ((input[1] & kQte2) || (g_uiRefreshedElements[1] & kQte2) || GetIsQuickIconShowing())
        return static_cast<std::int32_t>((input[2] & kHelpOverlay) ? InputType::Game : InputType::Qte2);

    constexpr std::uint32_t kQte = 1U << 3U;
    if ((input[2] & kQte) || (g_uiRefreshedElements[2] & kQte) || areas::GetArmwGameplayActive() == 1)
        return static_cast<std::int32_t>(InputType::Qte);

    constexpr std::uint32_t kFightRoot = 1U << 2U;
    if ((input[0] & kFightRoot) && (IsFightGaugeInputActive() || IsFightGaugeOverlayInputActive()))
        return static_cast<std::int32_t>(InputType::Fight);
    if (g_uiElements[static_cast<std::size_t>(UIElement::FightGauge)].isVisible && (IsFightGaugeInputActive() || IsFightGaugeOverlayInputActive()))
        return static_cast<std::int32_t>(InputType::Fight);
    if ((g_uiRefreshedElements[0] & kFightRoot) && (IsFightGaugeInputActive() || IsFightGaugeOverlayInputActive()))
        return static_cast<std::int32_t>(InputType::Fight);
    constexpr std::uint32_t kFightOverlay = 1U << 3U;
    if ((input[1] & kFightOverlay) || IsHudFightSpriteActive() == 1)
        return static_cast<std::int32_t>(InputType::Fight);

    return gameMenuActive != 0 && g_gameArcadeRunning == 0 && !GetIsGameHangOnHandleValid() ? 0 : 2;
}

bool DrawLegacyUISpritePrimitive(const LegacyUISpritePrimitive &primitive) {
    if (!primitive.texture || primitive.width <= 0.0f || primitive.height <= 0.0f) {
        return false;
    }
    const float sx = static_cast<float>(d3t::g_originalScreenWidth) / 640.0f;
    const float sy = static_cast<float>(d3t::g_originalScreenHeight) / 480.0f;
    const float left = (primitive.centerX - primitive.width * 0.5f) * sx;
    const float right = (primitive.centerX + primitive.width * 0.5f) * sx;
    const float top = (primitive.centerY - primitive.height * 0.5f) * sy;
    const float bottom = (primitive.centerY + primitive.height * 0.5f) * sy;
    const std::array<d3t::UITexturedVertex, 4> vertices{{
        {left, top, primitive.u0, primitive.v0, primitive.colour},
        {right, top, primitive.u1, primitive.v0, primitive.colour},
        {right, bottom, primitive.u1, primitive.v1, primitive.colour},
        {left, bottom, primitive.u0, primitive.v1, primitive.colour},
    }};
    constexpr std::array<std::uint16_t, 6> indices{0, 1, 2, 0, 2, 3};
    const std::array<std::int32_t, 4> scissor{
        0, 0, static_cast<std::int32_t>(d3t::g_originalScreenWidth), static_cast<std::int32_t>(d3t::g_originalScreenHeight)};
    return d3t::DrawUITexturedGeometry(primitive.texture, vertices, indices, scissor);
}

bool DrawLegacyUIColourPrimitive(float centerX, float centerY, float width, float height, std::uint32_t colour) {
    return DrawLegacyUISpritePrimitive({d3t::g_defaultWhiteTexture, centerX, centerY, width, height, 0.0f, 0.0f, 1.0f, 1.0f, colour});
}

// @verified - p
std::uint32_t LoadCommonIconSprites(PakGlobalData *package) {


    for (CommonIconAtlas &atlas : commonIconAtlases) {
        if (atlas.textureHandle && atlas.textureHandle != ResolveNamedTextureHandle(atlas.textureIndex))
            d3t::InitTexture2DResource(atlas.textureHandle);
        atlas = {};
    }
    commonIconAtlasCount = 0;
    constexpr shen_name binExtension{make_shen_name('B', 'I', 'N', ' ')};
    std::span<const std::byte> bytes = FileLoaderView(package, "COMICON.SPR", binExtension);
    while (bytes.size() >= 8 && commonIconAtlasCount < commonIconAtlases.size()) {
        std::uint32_t tag{}, size{};
        std::memcpy(&tag, bytes.data(), sizeof(tag));
        std::memcpy(&size, bytes.data() + 4, sizeof(size));
        if (!tag || size < 8 || size > bytes.size())
            break;
        CommonIconAtlas &atlas = commonIconAtlases[commonIconAtlasCount++];
        atlas.textureIndex = static_cast<std::int32_t>(ReadTexnTexture(bytes.data()));
        atlas.textureHandle = ResolveNamedTextureHandle(atlas.textureIndex);
        bytes = bytes.subspan(size);
    }
    return commonIconAtlasCount;
}

// @verified - p
std::int64_t LoadLoadScreenSpriteTexture(PakGlobalData *package) {
    constexpr shen_name binExtension{make_shen_name('B', 'I', 'N', ' ')};
    const std::span<const std::byte> bytes = FileLoaderView(package, "LD_nl.SPR", binExtension);
    if (bytes.empty())
        return 0;
    const std::int64_t result = ReadTexnTexture(bytes.data());
    g_loadScreenSpriteTexture = static_cast<std::int32_t>(result);
    return result;
}

void ReleaseLoadScreenSpriteTexture() {
    SetNamedTextureBank(2);
    if (g_loadScreenSpriteTexture != -1)
        ReleaseNamedTexture(g_loadScreenSpriteTexture);
    g_loadScreenSpriteTexture = -1;
    SetNamedTextureBank(0);
}

void ReleaseCommonIconSprites() {
    SetNamedTextureBank(2);
    for (CommonIconAtlas &atlas : commonIconAtlases) {
        if (atlas.textureHandle && atlas.textureHandle != ResolveNamedTextureHandle(atlas.textureIndex))
            d3t::InitTexture2DResource(atlas.textureHandle);
        if (atlas.textureIndex != -1)
            ReleaseNamedTexture(atlas.textureIndex);
        atlas = {};
    }
    commonIconAtlasCount = 0;
    SetNamedTextureBank(0);
}

CommonIconAtlas *GetCommonIconAtlas(std::uint16_t icon) {
    const std::size_t index = icon >> 8U;
    return &commonIconAtlases[index < commonIconAtlases.size() ? index : 0];
}

std::array<std::uint8_t, 4> GetCommonIconRect(std::uint16_t icon) {
    const std::uint32_t bank = icon >> 8U;


    const std::size_t offset = bank < commonIconRectangleOffsets.size() ? commonIconRectangleOffsets[bank] + (icon & 0xffU) * 4U : 0x140U;
    return {commonIconRectangles[offset], commonIconRectangles[offset + 1], commonIconRectangles[offset + 2], commonIconRectangles[offset + 3]};
}

bool ReplaceCommonIconAtlasImage(std::uint16_t icon, std::string_view path) {
    CommonIconAtlas &atlas = *GetCommonIconAtlas(icon);
    if (!atlas.textureHandle || path.empty())
        return false;
    const std::string source(path);
    const std::string filename(path.substr(0, path.find('?')));
    if (!ArchiveData_GetFileSize(filename.c_str()))
        return false;
    std::uint32_t replacement = 0;
    d3t::LoadImageTexture(source.c_str(), false, &replacement, 0);
    if (!replacement)
        return false;
    if (atlas.textureHandle != ResolveNamedTextureHandle(atlas.textureIndex))
        d3t::InitTexture2DResource(atlas.textureHandle);
    atlas.textureHandle = replacement;
    return true;
}

bool GetShouldRenderIcons(RenderIconSlot slot) {
    if (slot == RenderIconSlot::Any)
        return renderIconFlags != 0;
    const auto index = static_cast<std::uint32_t>(slot);
    return index < static_cast<std::uint32_t>(RenderIconSlot::Count) && (renderIconFlags & (1U << index)) != 0;
}

bool ToggleRenderIcons(RenderIconSlot slot, bool enabled) {
    const auto index = static_cast<std::uint32_t>(slot);
    if (index >= static_cast<std::uint32_t>(RenderIconSlot::Count))
        return false;
    const bool previous = GetShouldRenderIcons(slot);
    if (enabled)
        renderIconFlags |= 1U << index;
    else
        renderIconFlags &= ~(1U << index);
    renderIconFlags &= ~1U;
    return previous;
}

void SetShowQteFlag(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint16_t buttonFlags) noexcept {
    quickIconFlags = buttonFlags;
    quickIconMaximumFrames = maximumFrames == 0x7fff ? -1.0f : (maximumFrames > 0 ? static_cast<float>(maximumFrames) : 1.0f);
    quickIconElapsedFrames = 0.0f;
    quickIconPhase = 2;
    ToggleRenderIcons(RenderIconSlot::Qte, true);
    quickIconCurrentStatus = -1;
    if ((buttonFlags & 0x20U) == 0) {
        std::uint16_t normalized = 1;
        std::uint8_t bit = 0;
        while (bit < 12 && (requiredStatus & normalized) == 0) {
            normalized = static_cast<std::uint16_t>(normalized << 1U);
            ++bit;
        }
        requiredStatus = normalized;
    }
    quickIconRequiredStatus = requiredStatus;
    quickIconUnpressed = true;
    quickIconDrawFrames = 0.0f;
    quickIconPreviousDrawFrames = 0.0f;
    std::uint8_t bit = 0;
    if (static_cast<std::uint8_t>(requiredStatus)) {
        quickIconSprite.icon = 0x800;
        while (bit < 7 && !(requiredStatus & (1U << bit)))
            ++bit;
        quickIconSprite.rotation = static_cast<std::uint32_t>(bit) << 13U;
        quickIconSprite.scaleY = (bit & 0xfbU) == 0 ? 0.9f : 1.0f;
    } else {
        const std::uint8_t buttons = static_cast<std::uint8_t>(requiredStatus >> 8U);
        while (bit < 4 && !(buttons & (1U << bit)))
            ++bit;
        quickIconSprite.rotation = 0;
        quickIconSprite.icon = QUICK_ICON_FACE_ICONS[bit];
        quickIconSprite.scaleY = 1.0f;
    }
    ResetBattleQteTransition();
}

void SetShowQteFlagForButtonType(std::uint16_t requiredStatus, std::int16_t maximumFrames, std::uint32_t buttonType) noexcept {
    if (buttonType >= QUICK_ICON_BUTTON_TYPES.size())
        buttonType = 0;
    SetShowQteFlag(requiredStatus, maximumFrames, QUICK_ICON_BUTTON_TYPES[buttonType]);
    quickIconButtonMask = 0xffffU;
}

std::int32_t GetCurrentQteStatus() noexcept {
    const std::uint16_t status = static_cast<std::uint16_t>(quickIconCurrentStatus);
    quickIconCurrentStatus = -1;
    return status;
}

bool GetIsQuickIconShowing() noexcept {
    return quickIconPhase != 0;
}

bool ResetQuickIconState(bool enabled) noexcept {
    if (!GetShouldRenderIcons(RenderIconSlot::Qte) || !enabled)
        return false;
    quickIconButtonMask = 0;
    quickIconPhase = 0;
    iconToggleStateFor(3, false);
    quickIconTerminalStatus = -1;
    quickIconPosition[0] = -176.0f;
    return true;
}

void SetQteButtonState(std::uint16_t mask) noexcept {
    quickIconButtonMask = mask;
}

void SetQteMinigameModel(MT7Model *model) noexcept {
    qteMinigame.model = model;
}
void SetQteMinigameTitleButtons(const std::uint32_t *buttons) noexcept {
    qteMinigame.titleButtons = buttons;
}

std::uint8_t GetParentMenu() {

    if ((renderIconFlags & 2U) != 0)
        return 0;
    if ((renderIconFlags & 8U) != 0)
        return 2;
    if ((renderIconFlags & 4U) != 0)
        return 4;
    return (renderIconFlags & 32U) != 0 ? 6 : 7;
}

// @verified - p
std::uint8_t SetPersistentHelpIconState(std::uint32_t visible, std::uint32_t selected, std::int32_t merge) {
    HelpIconState &help = helpIconState;
    if (merge) {
        visible |= help.persistentVisible;
        selected |= help.persistentSelected;
    }
    help.persistentVisible = visible;
    help.persistentSelected = selected & visible;
    help.frameOverlaySelected &= help.frameOverlayVisible;
    return ConfigureHelpDirectionBlink(visible);
}

void CheckHelpIcon(std::uint32_t visible, std::uint32_t selected, CheckHelpIconOp operation) {
    HelpIconState &help = helpIconState;
    switch (operation) {
    case CheckHelpIconOp::Set:
        SetPersistentHelpIconState(visible, selected, 0);
        return;
    case CheckHelpIconOp::Add:
        SetPersistentHelpIconState(visible, selected, 1);
        return;
    case CheckHelpIconOp::Remove: {
        const std::uint32_t blink = selected & visible & help.persistentVisible & help.persistentSelected;
        helpIconDeferredRemoval |= blink & help.persistentSelected;
        help.persistentVisible &= ~(visible & ~blink);
        break;
    }
    case CheckHelpIconOp::FrameOverlay:
        help.frameOverlayVisible |= visible;
        help.frameOverlaySelected |= selected;
        break;
    }
    help.frameOverlaySelected &= help.frameOverlayVisible;
    help.persistentSelected &= help.persistentVisible;
    ConfigureHelpDirectionBlink(help.persistentVisible);
}

const HelpIconState &GetHelpIconState() {
    return helpIconState;
}

std::uint32_t GetPersistentHelpIconFlags(std::uint32_t index) {
    if (index == 0)
        return helpIconState.persistentVisible;
    if (index == 1)
        return helpIconState.persistentSelected;
    return 0;
}

std::int32_t GetHelpIconItemEffectState() {
    return helpIconItemEffectState;
}

void doItemEffectStuff() {
    helpIconItemEffectFrames = 150;
    helpIconItemEffectState = 1;
}

// @verified - p
void RequestHelpIconItemPrompt() {
    helpIconItemPromptFrames = 150;
    CheckHelpIcon(0x200, 0x200, CheckHelpIconOp::Add);
    SetHudItemPromptIcon(1);
}

// @verified - p
void SetHudItemPromptIcon(std::int32_t enabled) {
    nativeHudItemPromptIcon = enabled;
}

std::uint32_t GetUiFlag(std::uint8_t mode) {
    if (!mode)
        return static_cast<std::uint16_t>(helpIconState.persistentVisible);
    return mode == 1 || (mode == 2 && helpIconState.persistentVisible);
}

std::int32_t GetQteControlState() noexcept {
    return qteControlState;
}

void SetQteControlState(std::int32_t state) noexcept {
    if (qteControlState != 1 && state == 1)
        qteControlMask = 0;
    qteControlState = state;
}

// @verified - p
void SetQteCameraAtTop(std::int32_t mode) noexcept {
    qteCameraAtTop = mode;
}

void qteSetupMaskFor90(std::uint32_t mask) noexcept {
    qteControlMask = mask;
    qteDrawFrames = 90;
}

std::uint32_t GetQteMaskFor90() noexcept {
    return qteControlMask;
}

// @verified - p
std::uint32_t GetQuickIconLoadingReleaseState() noexcept {
    return quickIconLoadingReleaseFlag;
}

void SetNowQteDrawEnabled(bool enabled) noexcept {
    nowQteDrawSuppression = enabled;
}

bool IsNowQteDrawEnabled() noexcept {
    return nowQteDrawSuppression;
}

void SetHelpIconMenuFlags(std::uint16_t flags) {
    if (GetQuickIconLoadingReleaseState() == 0 && (g_loadingFlag & 0x20U) != 0)
        flags = static_cast<std::uint16_t>(flags & ~1U);
    helpIconState.frameOverlaySelected &= helpIconState.frameOverlayVisible;
    helpIconState.persistentVisible = flags;
    helpIconState.persistentSelected = 0;
    ConfigureHelpDirectionBlink(flags);
}

void AddHelpIconMenuFlags(std::uint16_t flags) {
    const bool releaseLoading = GetQuickIconLoadingReleaseState() != 0 && !(helpIconPreviousLoadingFlags & 0x20U) && (g_loadingFlag & 0x20U);
    helpIconPreviousLoadingFlags = g_loadingFlag;
    if (releaseLoading)
        FinishLoading();
    if ((flags & 0x400U) != 0)
        ExecuteAicaCommand(0xaabU, 0, 0);
    const std::uint32_t visible = static_cast<std::uint32_t>(flags) | helpIconState.persistentVisible;
    if (helpIconState.persistentVisible)
        helpIconState.persistentSelected &= visible;
    else
        helpIconState.persistentSelected = 0;
    helpIconState.frameOverlaySelected &= helpIconState.frameOverlayVisible;
    helpIconState.persistentVisible = visible;
    ConfigureHelpDirectionBlink(visible);
}

void ResetHelpIconsToItemPrompt() {
    const std::uint32_t visible = helpIconState.persistentVisible & 0x200U;
    CheckHelpIcon(visible, visible, CheckHelpIconOp::Set);
}

// @verified - p
std::uint32_t GetIconSelectFlags() noexcept {
    return iconSelectFlags;
}

void set_icon_select_flags_to(std::uint32_t flags) {
    iconSelectFlags |= flags;
}

void SetGameMessageContent(std::int32_t mode, std::int32_t value, std::string_view text) {
    gameMessageMode = mode;
    gameMessageIcon = mode == 0 ? value : -1;
    if (mode == 1 || mode == 2) {
        FormatGameMessageAmount(value, mode == 1);
    } else {
#ifdef _WIN32
        sprintf_s(gameMessageText.data(), gameMessageText.size(), "%s", text.data());
#else
        const std::size_t length = std::strlen(text.data());
        if (length >= gameMessageText.size())
            std::abort();
        std::memcpy(gameMessageText.data(), text.data(), length + 1);
#endif
    }
    ApplyGameMessageContent();
}

void UpdateGameMessageMoney(std::int32_t value) {
    FormatGameMessageAmount(value, true);
}

bool IsGameMessageVisible() {
    return GameMessagePosition() > 0.0f;
}

void SetGameMessageDelay(float seconds) {
    gameMessageNextDelay = seconds;
}

void ClearGameMessageMode() {
    gameMessageMode = 0;
}

bool InitializeNativeGuiResources() {


    guiPtxBacking = static_cast<std::byte *>(HLib::AllocateTaskData(guiPtxBackingSize, SHEN_NAME(ITPH)));
    if (!guiPtxBacking)
        return false;
    HLib::AssignDynamicAlignmentForTaskData(guiPtxBacking);
    return true;
}
// @verified - p
std::uint32_t RasterizeItemLabelNullsub(std::byte *, std::int32_t, const char *, std::int32_t, std::int32_t) noexcept {
    return 0;
}

std::uint16_t PublishLegacyItemLabel(const char *text, std::uint16_t width) noexcept {
    const std::uint16_t height = 16;
    std::memcpy(guiPtxBacking + 40, &width, sizeof(width));
    std::memcpy(guiPtxBacking + 42, &height, sizeof(height));
    const std::uint32_t wideWidth = width, wideHeight = height;
    std::memcpy(guiPtxBacking + 60, &wideHeight, sizeof(wideHeight));
    std::memcpy(guiPtxBacking + 64, &wideWidth, sizeof(wideWidth));
    (void)RasterizeItemLabelNullsub(guiPtxBacking, width, text, 0, 1);
    (void)ReplacePtxTexture(commonItemTexture, guiPtxBacking);
    return static_cast<std::uint16_t>(commonItemTexture);
}

// @verified - p
std::int64_t PrepareLegacyInventoryItemLabel(std::uint16_t &label, std::uint32_t item) {
    itemSelectorLabel = shenGetAmtOf(item);
    if (!itemSelectorLabel)
        itemSelectorLabel = "";
    const std::int64_t length = static_cast<std::int64_t>(std::strlen(itemSelectorLabel));
    label = 0xffff;
    label = PublishLegacyItemLabel(itemSelectorLabel, 512);
    return length;
}

void SetLegacyItemSelectorExitFlag() noexcept {
    itemSelectorSelection.exitFlags |= 1U;
}

void SetItemSelectorDelayedResult(std::uint16_t result) noexcept {
    itemSelectorDelayedResult = result;
}

bool initSCNT_item_data() {
    commonItemTexture = 0xffffffffU;
    const std::span<const std::byte> common = SetupPtxTexture(512, ITEM_TEXTURE_TYPE.name, ITEM_COMMON_TEXTURE.name);
    if (common.data()) {
        commonItemTexture = static_cast<std::uint32_t>(ReadTexnTexture(common.data()));
        if (static_cast<std::int32_t>(commonItemTexture) < 0)
            return false;
    }
    scenarioItemPtx = static_cast<std::byte *>(HLib::AllocateTaskData(0x8030, ITEM_COUNTER_TEXTURE));
    if (!scenarioItemPtx)
        return true;
    ParseGuiPtx(scenarioItemPtx, 256, 16, ITEM_TEXTURE_TYPE.name, ITEM_COUNTER_TEXTURE.name);
    scenarioItemTexture = UploadTexn({scenarioItemPtx, guiPtxTextSize});
    return scenarioItemTexture != 0;
}

// @verified - p
std::int64_t InitializeCtrlSpeakIconState() {
    ctrlSpeakSpecialLabelWidthScale = 2.0f;
    ctrlSpeakResultChoiceId = ctrlSpeakDefaultChoiceId = 0xffffU;
    ctrlSpeakTimeoutFrames = 10800;
    ctrlSpeakSelectorOrigin = CTRL_SPEAK_ORIGIN_INITIAL;
    ctrlSpeakIconState = 0;
    ctrlSpeakOpenRequested = false;
    ctrlSpeakElapsedFrames = 0;
    ctrlSpeakDedicatedCancelEnabled = 0;
    ctrlSpeakHasSpecialLabel = ctrlSpeakForceTextLabels = false;
    ctrlSpeakSelectedChoiceSlot = 4;
    ctrlSpeakSelectorScale = 1.0f;
    for (std::size_t slot = 0; slot < 4; ++slot) {
        LegacyIconSprite &sprite = ctrlSpeakChoiceDrawSprites[slot];
        sprite.icon = 0xffffU;
        sprite.flags = 0;
        sprite.width = 256;
        sprite.height = 32;
        sprite.scaleX = sprite.scaleY = 1.0f;
        sprite.x = static_cast<float>(SLEEP_ARROW_X[slot]) - 176.0f;
        sprite.y = ACTION_SELECTOR_CHOICE_Y[slot] + 91.0f;
        sprite.rotation = 0;
        sprite.alignment = 5;
        sprite.depth = SLEEP_TEXT_DEPTH[slot] + 1.0f;
    }
    ctrlSpeakTexturePoolFlags.fill(0);
    ctrlSpeakTexturePoolHandles.fill(-1);
    ctrlSpeakChoiceTextHandles.fill(-1);
    std::int64_t result = -1;
    for (std::size_t index = 0; index < ctrlSpeakTexturePoolHandles.size(); ++index) {
        const std::span<const std::byte> texture =
            SetupPtxTexture(256, CTRL_SPEAK_TEXTURE_TYPE.name, make_shen_name('0', '0', '0', static_cast<char>('0' + index)));
        result = ReadTexnTexture(texture.data());
        ctrlSpeakTexturePoolHandles[index] = static_cast<std::int32_t>(result);
    }
    return result;
}

HLib::Task *InitSpeakIcon(HLib::Task *parent, std::uint8_t nextFunctionIndex) {
    InitializeCtrlSpeakIconState();
    speakIconTask = HLib::QueueTaskAfterCurrentTask(parent, CtrlSpeakIcon_Callback, nextFunctionIndex, 0, TASK_CTRL_SPEAK.name);
    return speakIconTask;
}

// @verified - p
HLib::Task *InitNowQte(std::uint8_t nextFunctionIndex) {
    helpIconPreviousLoadingFlags = 0;
    helpIconItemPromptFrames = -1;
    helpIconItemEffectFrames = -1;
    helpIconItemEffectState = 0;
    currentIconDeltaTime = 0.0f;
    iconDeltaTime = 7;
    helpIconBlinkFrames = 5;
    helpIconDeferredRemoval = 0;
    helpIconState.frameOverlayVisible = helpIconState.frameOverlaySelected = 0;
    helpIconState.persistentVisible = helpIconState.persistentSelected = 0;
    iconSelectFlags = 0;
    helpIconPublishedVisible = 0;
    helpIconState.removalBlinkMask = 0;
    helpIconHudDelay = 5;
    helpIconRefreshDelay = -1;
    qteControlState = 0;
    qteCameraAtTop = 0;
    ParseGuiPtx(nowQtePtx.data(), 256, 16, NOW_QTE_TEXTURE_TYPE.name, NOW_QTE_TEXTURE_NAME.name);
    nowQteTexture = static_cast<std::uint32_t>(ReadTexnTexture(nowQtePtx.data()));
    (void)RasterizeItemLabelNullsub(nowQtePtx.data(), 256, "Now processing...", 0, 1);
    (void)ReplacePtxTexture(nowQteTexture, nowQtePtx.data());
    nowQteDrawSuppression = 0;
    nowQteTask = HLib::EnqueueTaskWithoutParameter(NQ_CtrlButtonIcon, nextFunctionIndex, 0, static_cast<std::int32_t>(TASK_NOW_QTE.name));
    return nowQteTask;
}

// @verified - p
std::int32_t SetupControlSpeakAndUI(std::int16_t *choices, std::int32_t timeout, std::int16_t defaultChoice, std::uint16_t mode) {
    return static_cast<std::int32_t>(SetupControlSpeakAndUI_Internal(choices, timeout, defaultChoice, mode, 0));
}

std::int64_t SetupControlSpeakAndUI_Internal(std::int16_t *choices, std::int32_t, std::int16_t defaultChoice, std::uint16_t mode, std::int32_t style) {
    dialogueSelectionOwned = false;
    ctrlSpeakDedicatedCancelEnabled = 0;
    for (std::size_t index = 0; index < 4; ++index)
        if (choices[index] == 470)
            choices[index] = -1;

    dialogueSelectionPrompt = {};
    for (std::size_t index = 0; index < 4; ++index) {
        dialogueSelectionPrompt.flagIds[index] = static_cast<std::uint16_t>(choices[index]);
        PublishActionSelectorText(index, GetActionSelectorLabel(dialogueSelectionPrompt.flagIds[index]));
        dialogueSelectionPrompt.textAvailable |= !dialogueSelectionPrompt.labels[index].empty();
    }
    numericActionSelector.mode = 0;
    dialogueSelectionPrompt.active = true;
    dialogueSelectionPrompt.focusedSlot = FirstDialogueSelectionSlot();
    ctrlSpeakResultChoiceId = 0xffff;
    ctrlSpeakOpenRequested = true;
    ctrlSpeakMarkerGlyphIndex = style >= 5 ? 0 : style;
    ctrlSpeakTimeoutFrames = 10800;
    ctrlSpeakElapsedFrames = 0;
    dialogueSelectionTimedOut = false;
    for (std::size_t index = 0; index < 4; ++index)
        if (choices[index] >= 486)
            choices[index] = -1;
    if (choices[0] == choices[1] && choices[2] == choices[3] && choices[0] == choices[2]) {
        const std::uint64_t zero = 0;
        std::memcpy(choices, &zero, sizeof(zero));
    }
    ctrlSpeakChoiceIds = {choices[0], choices[1], choices[3], choices[2]};
    ctrlSpeakSpecialLabelWidthScale = 2.0f;
    ctrlSpeakHasSpecialLabel = ctrlSpeakForceTextLabels = false;
    for (std::size_t index = 0; index < 4; ++index) {
        const std::string_view label = GetActionSelectorLabel(static_cast<std::uint16_t>(ctrlSpeakChoiceIds[index]));
        ctrlSpeakChoiceWidths[index] = static_cast<std::int16_t>(label.size() > 7 ? 14 * label.size() + 30 : 128);
    }
    if (ctrlSpeakChoiceWidths[1] != ctrlSpeakChoiceWidths[3]) {
        if (ctrlSpeakChoiceWidths[1] <= ctrlSpeakChoiceWidths[3])
            ctrlSpeakChoiceWidths[1] = ctrlSpeakChoiceWidths[3];
        else
            ctrlSpeakChoiceWidths[3] = ctrlSpeakChoiceWidths[1];
    }
    ctrlSpeakMode = mode;
    ctrlSpeakDefaultChoiceId = mode ? static_cast<std::uint16_t>(defaultChoice) : 0xffffU;
    const float expansion = static_cast<float>(ctrlSpeakChoiceWidths[1]) - 128.0f;
    ctrlSpeakHalfWidthExpansion = expansion * 0.5f;
    ctrlSpeakRightOriginX = expansion + 180.0f;
    ctrlSpeakLeftOriginX = -176.0f - expansion;
    ToggleRenderIcons(RenderIconSlot::ActionSelector, true);
    return 1;
}

// @verified - p
std::int64_t CloseSpeakIcon(std::int32_t force) {
    if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector) || !force)
        return 0;
    ctrlSpeakSelectorOrigin[0] = -176.0f;
    ctrlSpeakIconState = 0;
    ctrlSpeakOpenRequested = false;
    ctrlSpeakDedicatedCancelEnabled = 0;
    iconToggleStateFor(2, false);
    ReleaseSpeakTextSlots();
    ToggleRenderIcons(RenderIconSlot::ActionSelector, false);
    return 1;
}
std::uint32_t shenGetCtrlSpeakIconFlag() {
    return ctrlSpeakResultChoiceId;
}
void speakSetFlagTo(std::uint16_t timeout) {
    ctrlSpeakTimeoutFrames = std::bit_cast<std::int16_t>(timeout);
}
bool IsCtrlSpeakIconInactive() {
    return ctrlSpeakIconState == 0;
}
bool IsSpeakIconCancellable() {
    return ctrlSpeakMode == 2 || ctrlSpeakMode == 5;
}

void iconToggleStateFor(std::uint8_t slot, bool enabled) {
    if (slot == 0)
        activeIconFlags = enabled ? 0xffffffffU : 0;
    else if (enabled)
        activeIconFlags |= 1U << (slot & 31U);
    else
        activeIconFlags &= ~(1U << (slot & 31U));
}

std::uint32_t GetItemSelectionMode(std::uint8_t slot) {
    return activeIconFlags & (1U << (slot & 31U));
}

std::uint32_t RefreshItemSelectorInventory(std::uint32_t bank) noexcept {
    const std::span<const std::byte> stock = S2Save_GetInventoryBank(static_cast<S2InventoryBank>(bank));
    const std::uint32_t count = GetMaxItemsForItemType(bank);
    const bool filtered = GetInternalFlag(86);
    itemSelectorOwnedMask.fill(0);
    std::uint32_t owned = 0;
    for (std::uint32_t index = 0; index < count; ++index) {
        if (std::to_integer<std::uint8_t>(stock[index]) == 0 || (filtered && !(GetInventoryItemFlags(bank, index) & 2U)))
            continue;
        itemSelectorOwnedMask[index >> 3U] |= static_cast<std::uint8_t>(1U << (index & 7U));
        ++owned;
    }
    PublishItemSelectorOwnedMask(itemSelectorOwnedMask.data());
    return owned;
}

std::span<const std::uint8_t, 64> GetItemSelectorOwnedMask() noexcept {
    return itemSelectorOwnedMask;
}

// @verified - p
void PublishItemSelectorOwnedMask(const std::uint8_t *mask) noexcept {
    itemSelectorPublishedMask = mask;
}

// @verified - p
const std::uint8_t *GetPublishedItemSelectorOwnedMask() noexcept {
    return itemSelectorPublishedMask;
}

std::uint32_t GetCurrentItemSelectState() {
    return currentItemSelectState;
}

bool IsModeBitActive(std::int32_t mode) {
    return (static_cast<std::uint32_t>(mode) & currentItemSelectState) != 0;
}

bool IsAnyItemSelectActive() {
    return (GetCurrentItemSelectState() & 1U) != 0;
}

std::uint32_t icsToggleIconState(std::uint32_t index, std::int32_t enabled) {
    const std::uint32_t previous = currentItemSelectState;
    if (index < 9) {
        if (enabled)
            currentItemSelectState |= 1U << index;
        else
            currentItemSelectState &= ~(1U << index);
    }
    return previous;
}

bool IsItemSelectable() {
    return (GetCurrentItemSelectState() & 0x20U) != 0;
}

std::uint16_t GetPadControlData(std::uint8_t mode) {
    if (!iconPadControl)
        return 0;
    const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(iconPadControl);
    switch (mode) {
    case 0:
        return input->held;
    case 1:
        return input->pressedPriority;
    case 2:
        return input->repeatedPresses;
    default:
        return 0;
    }
}

void padcRefreshCurrentInputs(bool bind) {
    if (!iconPadControl)
        return;
    if (bind) {
        if ((iconPadControl->flags & 2U) == 0)
            previousIconPadControl = BindPadControlEntry(iconPadControl);
    } else if ((iconPadControl->flags & 2U) != 0 && previousIconPadControl) {
        BindPadControlEntry(previousIconPadControl);
        previousIconPadControl = nullptr;
    }
}

static std::int64_t RetireIconInputSlot(std::uint32_t slot, std::int32_t force) {
    using enum CameraState;
    if (slot >= 8)
        return 0;
    if (slot == 0) {
        std::int64_t result = 0;
        for (std::uint32_t index = 1; index < 8; ++index)
            result |= RetireIconInputSlot(index, force);
        return result;
    }
    std::int64_t result = 0;
    switch (slot) {
    case 1:
        if (force && GetShouldRenderIcons(RenderIconSlot::ItemSelect) && !IsSaveAllowed()) {
            if (itemSelectTask)
                HLib::SetupTimedTask(itemSelectTask, 0);
            if (uniqueItemActionTask) {
                UniqueItemActionData *const action = static_cast<UniqueItemActionData *>(HLib::GetTaskParameter(uniqueItemActionTask));
                if (action) {
                    if (action->cancel)
                        action->cancel(1);
                    ResetUniqueItemAction(*action);
                }
            }
            CloseWristwatch(1);
            itemSelectorDelayedResult = 0;
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(Init);
            itemSelectorCameraActive = false;
            SelectPostFxBank(1, mainIconPostFxBank);
            mainIconPhase = 5;
            itemSelectorWindowPosition.x = -672.0f;
            itemSelectPhase = 0;
            iconToggleStateFor(1, false);
            ToggleRenderIcons(RenderIconSlot::ItemSelect, false);
            itemSelectorLabelTexture = -1;
            RestoreItemSelectorContext();
            result = 1;
        }
        break;
    case 2:
        result = CloseSpeakIcon(force);
        break;
    case 3:
        result = ResetQuickIconState(force != 0);
        break;
    case 4:
        result = AbortMoneyIcon(force);
        break;
    case 5:
        result = CloseItemEffect(force);
        break;
    case 7:
        if (force == 1) {
            ReinitTextureData();
            result = 1;
        }
        break;
    default:
        break;
    }
    return result;
}

std::int64_t RefreshPadControlData(std::uint32_t slot, std::int32_t force) {
    const std::int64_t result = RetireIconInputSlot(slot, force);
    if (result && iconPadControl && (iconPadControl->flags & 2U) && previousIconPadControl) {
        BindPadControlEntry(previousIconPadControl);
        previousIconPadControl = nullptr;
    }
    return result;
}
static void SetMoneyAnimationTarget(std::int32_t amount, std::int32_t mode) noexcept {
    const std::int32_t current = mode ? GetCurrentCash() : GetCurrentSegaCoins();
    const std::uint32_t wrapped = static_cast<std::uint32_t>(current) + static_cast<std::uint32_t>(amount);
    const std::int32_t requested = std::bit_cast<std::int32_t>(wrapped);
    const std::int32_t limit = mode ? 999999 : 99999999;
    moneyIconDrawMode = mode ? 2 : 0;
    moneyIconTarget = requested > limit ? limit : requested < 0 ? 0 : requested;
    moneyIconPhase = 2;
}

std::int64_t AddThisToCashOrSegaCoins(std::int32_t amount, ECashOrCoins mode) noexcept {
    SetMoneyAnimationTarget(amount, static_cast<std::int32_t>(mode));
    const std::int64_t result = ToggleRenderIcons(RenderIconSlot::MoneyMessage, true);
    moneyIconFrames = 75.0f;
    moneyIconFlags = 0x14;
    return result;
}

std::int64_t AddCashOrCoinsWithStep(std::int32_t amount, std::int32_t mode, std::int32_t step) noexcept {
    SetMoneyAnimationTarget(amount, mode);
    const std::int64_t result = ToggleRenderIcons(RenderIconSlot::MoneyMessage, true);
    moneyIconFrames = 75.0f;
    moneyIconFlags = 0x54;
    moneyIconFixedStep = step;
    return result;
}

std::int64_t AnimateMoneyAmount(std::int32_t initial, std::int32_t target, std::int32_t modeIndex) noexcept {
    moneyIconFrames = 75.0f;
    moneyIconFlags = 0x34;
    moneyIconPhase = 2;
    moneyIconDrawMode = static_cast<std::uint8_t>(MONEY_ANIMATION_MODES[modeIndex]);
    const std::int32_t limit = moneyIconDrawMode ? 999999 : 99999999;
    moneyIconDisplay = initial >= limit ? limit : initial;
    moneyIconTarget = target >= limit ? limit : target;
    return ToggleRenderIcons(RenderIconSlot::MoneyMessage, true);
}

std::int64_t SetMoneyRelatedUIElement_0(std::int32_t maximum, std::int64_t, std::int32_t step, std::int32_t flags) noexcept {
    const std::int64_t result = GetShouldRenderIcons(RenderIconSlot::Any);
    if (result)
        return result;
    moneyIconFrames = 10800.0f;
    if (maximum <= 0)
        maximum = 1;
    moneyIconFlags = flags;
    moneyIconPhase = 2;
    if (maximum > GetCurrentCash())
        maximum = GetCurrentCash();
    if (step <= 0)
        step = 1;
    if (step > maximum)
        step = maximum;
    moneyIconStepAmount = step;

    moneyIconMaximum = maximum - maximum % step;
    return ToggleRenderIcons(RenderIconSlot::MoneyMessage, true);
}

std::int64_t SetMoneyRelatedUIElement(std::int16_t maximum, std::int64_t unused, std::int16_t step) noexcept {
    unused = (unused & ~std::int64_t{0xffff}) | (unused & 0x7fff);
    return SetMoneyRelatedUIElement_0(maximum & 0x7fff, unused, step & 0x7fff, 15);
}

std::uint64_t GetNumBet() noexcept {
    return static_cast<std::uint32_t>(moneyIconResult ? moneyIconResult : moneyIconAmount);
}

void SetMoneyHudCashAdjustment(std::int32_t amount) noexcept {
    moneyHudCashAdjustment = amount;
}

void KeepMoneyIconOpen() noexcept {
    moneyIconFlags &= ~0x10;
}

std::int64_t AbortMoneyIcon(std::int32_t force) noexcept {
    if (!GetShouldRenderIcons(RenderIconSlot::MoneyMessage) || !force)
        return 0;
    iconToggleStateFor(4, false);
    ReleaseMoneyTextSlots();
    moneyIconPhase = 0;
    if (moneyIconFlags & 8)
        RestoreMoneyHelpState();
    moneyIconDrawMode = 2;
    moneyIconFlags = 0;
    ToggleRenderIcons(RenderIconSlot::MoneyMessage, false);
    moneyIconSelectionScale = 1.0f;
    moneyIconPosition.x = -516.0f;
    moneyIconAmount = 0;
    moneyIconPosition.selectedDirection = 4;
    return 1;
}

void CloseMoneyIcon() noexcept {
    moneyIconPhase = 4;
    moneyIconFrames = 0.0f;
}

bool OpenGameWatchMenuItem() {
    return OpenWristwatch();
}

void CloseGameWatchMenuItem(std::int32_t immediate) {
    CloseWristwatch(immediate);
}
bool OpenSaveGameMenuItem() {
    if (IsSaveAllowed() != 0 || !EV_CanDoAreaJump())
        return false;
    shenSetTimeSimulationTo(false);
    (void)StartShowScreenOfId(10);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    return true;
}

void CloseSaveGameMenuItem() {
    shenSetTimeSimulationTo(true);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
}

bool ToggleItemEffect(std::uint32_t itemId, std::int32_t amount) {
    if (GetShouldRenderIcons(RenderIconSlot::Any) || !itemEffectIconTask)
        return false;
    const char *const name = shenGetAmtOf(itemId);
    itemEffectName = name ? name : ITEM_EFFECT_MISSING_NAME;
    const std::size_t length = std::strlen(itemEffectName);
    itemEffectLabelTexture = -1;
    (void)PublishLegacyItemLabel(itemEffectName, 512);
    itemEffectLabelTexture = static_cast<std::int16_t>(commonItemTexture);
    if (itemEffectLabelTexture < 0)
        return false;
    const std::size_t cells = 14 * length / 40;
    std::uint8_t columns = static_cast<std::uint8_t>(cells + 1);
    if (length >= 12)
        ++columns;
    if (14 * length != 40 * cells)
        ++columns;
    itemEffectColumns = columns < 6 ? 6 : columns;
    itemEffectSprite.y = 55.0f;
    itemEffectPosition.y = 340.0f;
    itemEffectSprite.x = -672.0f;
    itemEffectPosition.x = -672.0f;
    HLib::ResolveDeferredCallback(itemEffectIconTask);
    itemEffectIconPhase = 2;
    iconToggleStateFor(5, true);
    itemEffectSprite.icon = GetInventoryItemIconById(itemId);
    ToggleRenderIcons(RenderIconSlot::ItemEffectMessage, true);
    itemEffectAmount = amount;
    SetGameMessageContent(0, static_cast<std::int16_t>(itemEffectSprite.icon), GetLocalizedSceneText(itemEffectName));
    return true;
}

bool CloseItemEffect(std::int32_t force) {
    if (!GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage) || !force)
        return false;
    itemEffectPosition.x = -672.0f;
    itemEffectPosition.y = 55.0f;
    itemEffectLabelTexture = -1;
    itemEffectIconPhase = 0;
    iconToggleStateFor(5, false);
    ToggleRenderIcons(RenderIconSlot::ItemEffectMessage, false);
    if (itemEffectIconTask)
        HLib::SetupTimedTask(itemEffectIconTask, 0);
    return true;
}

// @verified - p
bool SetSleepActionIcon(std::int16_t flags) {
    if (GetShouldRenderIcons(RenderIconSlot::Any))
        return false;
    const std::int32_t requested = flags & 0x11b;
    if (requested != 0)
        sleepActionFlags = requested;
    if ((sleepActionFlags & 0x100) != 0)
        sleepActionFlags |= 0x10;
    if ((sleepActionFlags & 0x1b) == 0)
        return false;
    ToggleRenderIcons(RenderIconSlot::SleepAction, true);
    sleepActionPosition[0] = 180.0f;
    sleepActionScale = 0.0f;
    sleepActionPublishedSelection = -1;
    sleepActionStatus = 1;
    return true;
}

// @verified - p
void ResetSleepActionIcon() {
    sleepActionPhase &= 0xff00U;
    sleepActionStatus &= 0xff00U;
    sleepActionSubmenuState = 0;
}

// @verified - p
void ResumeSleepActionIconAfterSave() {
    if (sleepActionSubmenuState != 1)
        return;
    sleepActionPhase &= 0xff00U;
    sleepActionStatus = static_cast<std::uint16_t>((sleepActionStatus & 0xff00U) | 1U);
    sleepActionSubmenuState = 0;
}

// @verified - p
std::uint64_t GetSleepActionIconFlags() {
    const std::uint32_t shifted = static_cast<std::uint32_t>(sleepActionFlags) >> 7U;
    const std::uint8_t flags = static_cast<std::uint8_t>(sleepActionStatus | ((sleepActionFlags & 5) ? 4 : 0) | (~shifted & 2U));
    return (shifted & 0xffffff00U) | flags;
}

// @verified - p
std::uint64_t GetSleepActionIconResult() {
    return sleepActionStatus >> 8U;
}

// @verified - p
void ResetSleepActionDefaultState() {
    sleepActionPhase = 0;
    sleepActionTargetSelection = 4;
    sleepActionStatus = 0;
    ToggleRenderIcons(RenderIconSlot::SleepAction, false);
    iconToggleStateFor(6, false);
    sleepActionFlags = 0x10f;
    sleepActionPublishedSelection = -1;
}

HLib::Task *QueueMainIconTask() {

    iconPadControl = nullptr;
    previousIconPadControl = nullptr;
    mainIconTask = HLib::EnqueueTaskWithoutParameter(InitMainIcon_Callback, 14, 0, TASK_MAIN_ICON.name);
    return mainIconTask;
}

void QueueItemSelectTask(HLib::Task *parent) {
    itemSelectTask = HLib::QueueTaskAfterCurrentTask(parent, InitItemSelectIcon_Callback, 0x0f, 0, static_cast<std::int32_t>(TASK_ICIS.name));
}

// @verified - p
void InitializeMoneyIconSprites() {
    moneyIconPosition = MONEY_POSITION_INITIAL;
    moneyIconSprite = {.x = -516.0f, .y = 340.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 2.0f, .icon = 1544, .width = 80, .height = 40, .alignment = 5};
    selectorCurrencySprite.x = -516.0f;
    selectorCurrencySprite.y = 350.0f;
    selectorCurrencySprite.scaleX = selectorCurrencySprite.scaleY = 1.0f;
    selectorCurrencySprite.depth = 3.0f;
    selectorCurrencySprite.flags = 0;
    selectorCurrencySprite.rotation = 0;
    selectorCurrencySprite.width = selectorCurrencySprite.height = 16;
    selectorCurrencySprite.alignment = 5;
    for (LegacyIconSprite &sprite : selectorNumberSprites) {
        sprite.x = -516.0f;
        sprite.y = 350.0f;
        sprite.scaleX = sprite.scaleY = 1.0f;
        sprite.depth = 3.0f;
        sprite.flags = 0;
        sprite.rotation = 0;
        sprite.icon = SELECTOR_DIGIT_ICONS[0];
        sprite.width = sprite.height = 16;
        sprite.alignment = 5;
    }
    InitializeLegacyQuantitySprites();
    for (std::size_t index = 0; index < moneyIconSigns.size(); ++index) {
        LegacyIconSprite &sign = moneyIconSigns[index];
        sign.scaleX = sign.scaleY = 1.0f;
        sign.depth = 3.0f;
        sign.flags = 0;
        sign.rotation = 0;
        sign.icon = static_cast<std::uint16_t>(index ? 1045 : 1051);
        sign.width = sign.height = 16;
        sign.alignment = 5;
    }
    moneyIconSelectionScale = 1.0f;
    moneyIconSelectorPosition = MONEY_SELECTOR_POSITION_INITIAL;
    for (LegacyIconSprite &sprite : moneyIconTextSprites)
        sprite = {.x = -176.0f, .y = 91.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 2.0f, .icon = 0xffffU, .width = 256, .height = 32, .alignment = 5};
    for (LegacyIconSprite &sprite : moneyIconDirections) {
        sprite.x = -176.0f;
        sprite.y = 91.0f;
        sprite.scaleX = sprite.scaleY = 1.0f;
        sprite.depth = 2.0f;
        sprite.rotation = 0;
        sprite.icon = 1544;
        sprite.width = 96;
        sprite.height = 50;
        sprite.alignment = 5;
    }
}

// @verified - p
void InitializeMoneyIconParameters() {
    moneyIconCurrency = 1;
    moneyIconDisplay = moneyIconTarget = moneyIconAmount = 0;
    InitializeMoneyIconSprites();
    moneyIconPhase = 0;
    moneyIconDrawMode = 2;
    moneyIconMaximum = 0;
    moneyIconStepAmount = 5;
}

// @verified - p
void QueueMoneyIconTask(HLib::Task *parent) {
    InitializeMoneyIconParameters();
    moneyIconTask = HLib::QueueTaskAfterCurrentTask(parent, CtrlMoneyIcon_Callback, 8, 0, static_cast<std::int32_t>(TASK_ICMO.name));
}

void QueueQuickIconTask(HLib::Task *parent) {
    (void)HLib::QueueTaskAfterCurrentTask(parent, InitQuickIcon_Callback, 8, 0, static_cast<std::int32_t>(TASK_ICQU.name));
}

HLib::Task *QueueItemEffectIconTask(std::uint8_t queue) {
    itemEffectIconTask = HLib::EnqueueTaskWithoutParameter(CtrlItemEffectIcon_Callback, queue, 0, static_cast<std::int32_t>(TASK_IEET.name));
    if (itemEffectIconTask) {
        HLib::SetupTimedTask(itemEffectIconTask, 0);
        itemEffectIconPhase = 2;
    }
    return itemEffectIconTask;
}

// @verified - p
HLib::Task *CtrlSleepActionIcon(std::uint8_t nextFunctionIndex) {
    sleepActionPadControl = AllocatePadControlEntry();
    sleepActionPhase = 0;
    sleepActionTargetSelection = 4;
    sleepActionStatus = 0;
    ToggleRenderIcons(RenderIconSlot::SleepAction, false);
    iconToggleStateFor(6, false);
    sleepActionPosition = SLEEP_POSITION_INITIAL;
    sleepActionCurrentSelection = 4;
    for (std::size_t slot = 0; slot < 4; ++slot) {
        LegacyIconSprite &arrow = sleepActionArrowSprites[slot];
        LegacyIconSprite &text = sleepActionTextSprites[slot];
        arrow.flags = 0;
        arrow.icon = 0;
        arrow.width = arrow.height = 64;
        arrow.scaleX = arrow.scaleY = 1.2f;
        arrow.alignment = 5;
        text.icon = 0xffffU;
        text.flags = 0;
        text.width = 256;
        text.height = 32;
        text.scaleX = text.scaleY = 1.0f;
        text.rotation = 0;
        text.alignment = 5;
        arrow.x = static_cast<float>(SLEEP_ARROW_X[slot]) - 176.0f;
        arrow.y = static_cast<float>(SLEEP_ARROW_Y[slot]) + 118.0f;
        const float degrees = static_cast<float>(90 * (4 - static_cast<std::int32_t>(slot)));
        arrow.rotation = static_cast<std::uint16_t>(static_cast<std::int32_t>(degrees * 8192.0f / 45.0f));
        arrow.depth = SLEEP_ARROW_DEPTH[slot] + 1.0f;
        text.x = static_cast<float>(SLEEP_TEXT_X[slot]) - 176.0f;
        text.y = static_cast<float>(SLEEP_TEXT_Y[slot]) + 118.0f;
        text.depth = SLEEP_TEXT_DEPTH[slot] + 1.0f;
    }
    sleepActionFlags = 0;
    sleepActionPublishedSelection = -1;
    sleepActionIconTask = HLib::EnqueueTaskWithoutParameter(CtrlSleepActionIcon_Callback, nextFunctionIndex, 0, TASK_SLEEP_ACTION.name);
    return sleepActionIconTask;
}

// @verified - p
std::uint64_t IsMainIconInputReady() {
    return mainIconDelay < 0 ? 1ULL : 0ULL;
}

// @verified - p
std::uint64_t IsMainIconOverlayActive() {
    return mainIconOverlayActive;
}

// @verified - p
void ResetMainIconState() {
    mainIconPhase = 0;
    activeIconFlags = 0;
    iconPadControl = nullptr;
    previousIconPadControl = nullptr;
    renderIconFlags = 0;
    mainIconDelay = 5;
}

HLib::Task *GUI_Tasks_Enqueue() {
    ResetLegacyCollectionSpriteOwners();
    if (!commonIconAtlasCount)
        return nullptr;
    mainIconPhase = 0;
    mainIconDelay = 5;
    itemSelectPhase = 0;
    moneyIconPhase = 0;
    itemEffectIconPhase = 0;
    renderIconFlags = 0;
    activeIconFlags = 0;
    ClearGuiItemActions();

    HLib::Task *const main = QueueMainIconTask();
    nativeGuiActive = 1;
    (void)InitializeNativeGuiResources();
    QueueItemSelectTask(main);
    (void)initSCNT_item_data();
    QueueMoneyIconTask(main);
    (void)InitSpeakIcon(main, 8);
    QueueQuickIconTask(main);
    (void)InitNowQte(8);
    helpIconTask = HP_SetUpHelpIcon();
    tmx_CtrlHandWatch(3);
    MM_MemoCtrl(3);
    (void)NAVI_DummyWait_Enqueue(7);
    (void)QueueItemEffectIconTask(3);
    (void)CtrlSleepActionIcon(8);
    (void)CtrlGauge_Init();
    (void)NVW_CtrlNaviWindow_Enqueue();
    RegisterItems();
    (void)CreatePAWNShops();
    return Queue_pw_CtrlPawnWindow();
}
} // namespace shenmue::s2
