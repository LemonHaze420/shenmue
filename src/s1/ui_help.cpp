#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {
void Queue_HP_SetUpHelpIcon();

void HelpNew_DrawFrame(nk_context &context, UILayoutRect rect, std::uint8_t alpha, std::uint32_t colour, float thickness);

void HelpNew_DrawOutlinedText(nk_context &context, const UILayoutRect &rect, nk_flags alignment, const char *text);

void HelpNew_RebuildLayout(nk_context &context, const UILayoutRect &area, float lineHeight);
}

namespace shenmue::s1 {
void VFMA_ICON();

std::uint64_t shenLoadSprite(std::int8_t transition, std::uint8_t fadeTicks, std::uint8_t buttonId);

void LoadRetrySprite(std::uint8_t token);

void UI_HELPSetEmulatorContext(bool enabled);

void UI_HELPClearEmulatorPrompt();

void UI_HELPStartTransitionAnim();
}

namespace shenmue::s1::ui_runtime_detail {
const char *HelpText_Character(const char *text, std::int32_t size, std::int32_t index, std::uint32_t &codepoint, std::int32_t &length);

void AppendHelpInfoBlock(std::int32_t colour, std::int32_t column, std::int32_t line, std::int32_t kind, std::int32_t lines);

void HandleHelpStringInfos(std::uint32_t token);

UIButtonIconWidget HelpNew_InitPlatformButtonIconSpec(std::int32_t id);

void HelpNew_AppendPlatformButtonIconSpec(std::int32_t id);

void ParseHelpTextRuns();

void FormatHelpString(const char *text);

void SelectFont(bool waza);

void SetHelpText(const char *text);

bool HELP_TextExceedsHeapRoom();

void RenderHelpOverlay();
}

namespace shenmue::s1 {
void UI_HELPPreparePage(std::uint8_t token);

void UI_HELPDispatch(std::int32_t mode, std::uint8_t helpIconId, std::uint8_t buttonId);

HLib::Task *UI_HELPGetTask();

void UI_HELPInitCallback(HLib::TaskData *);

void UI_HELPCallback(HLib::TaskData *);
}

namespace shenmue::s1::ui_runtime_detail {




void SetHelpDetailsByIndex(std::int32_t index) {
    g_controllerHelpOverlayState = 1;
    if (static_cast<std::uint32_t>(index) < kHelpStringIds.size() && kHelpStringIds[static_cast<std::size_t>(index)]) {
        g_helpOverlayPageTokenId = static_cast<std::uint8_t>(index);
        g_helpOverlayState = UIHelpWidgetState::PreparePage;
    }
    if (g_helpNewBackground)
        g_helpNewBackground->colour = {20, 20, 20, 240};
}



void Queue_HP_SetUpHelpIcon() {
    HelpPresentationOwner &help = g_helpPresentation;
    help.closing = false;
    g_helpOverlayState = UIHelpWidgetState::Idle;
    for (std::int16_t &surface : g_helpIconSurfaceIds)
        surface = -1;
    help.transitionCommand = 0;
    g_helpOverlayPageTokenId = 0;
    g_defaultHelpTokenId = 0;
    g_currentHelpTokenId = 0;
    g_helpButtonId = 0;
    help.onScreenCount = 0;
    help.restoreFrames = 0;
    help.iconFlags = 0;
    help.animationFrame = 0;
    help.task = HLib_EnqueueTaskWithoutParameter(UI_HELPInitCallback, 15, 0, SHEN_NAME(HELP));
    g_helpIconWidth = 0;
    g_helpIconHeight = 0;
    g_lcdmHelpIconSpritesLoaded = 0;
    help.retryIconFlag = 0x7f;
}

void EnsureHelpTask() {
    HLib::Task *const task = g_helpPresentation.task;
    const bool valid = task && task->taskIndex == 15 && !task->task_data_ptr && task->taskToken == static_cast<std::int32_t>(SHEN_NAME(HELP)) &&
        (task->mainTaskCallbackFnPtr == UI_HELPInitCallback || task->mainTaskCallbackFnPtr == UI_HELPCallback);
    if (!valid) {
        Queue_HP_SetUpHelpIcon();
    }
}



std::int64_t HelpNew_IsSelected() {
    for (std::size_t window = 0; window < g_helpNewWindows.size(); ++window)
        g_helpNewWindows[window] = UI_FindWindowById(kHelpNewWindowIds[window]);
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kUiHelpNew].config.uiHandleIndex);
    if (slot <= 0x4bU) {
        const std::uint32_t item = static_cast<std::uint32_t>(g_uiViewSlots[slot].widgetPoolIndices[0]);
        g_helpNewBackground = item > 0x3ffU ? nullptr : &g_uiStyleItems[item];
        if (g_helpNewBackground)
            g_helpNewBackground->colour = {40, 40, 40, 200};
    }
    g_helpNewTransitionAnimSlot = UIAnimAllocSlot();
    return 1;
}



void HelpNew_Render() {
    UIBaseRuntime &watch = g_uiElements[kUiGameHudWatch];
    watch.isHandledExternally = 1;
    g_helpNewEmulatorContext = 0;
    g_helpNewEmulatorPrompt = 0;
    if (watch.elementDraw)
        watch.elementDraw();
    UI_HELPStartTransitionAnim();
}


std::int64_t HelpNew_IsPressed() {
    if ((g_inputState.slots[0].pressedPriority & 0x0a00U) != 0) {
        g_controllerHelpOverlayState = 0;
        g_inputState.slots[0].pressedPriority = 0;
    }
    return 0;
}


std::int64_t HelpNew_Reset() {
    g_helpNewWindows[0] = nullptr;
    return 0;
}



std::int64_t HelpNew_GetPromptIconSet() {
    static std::array<std::int32_t, 5> prompts{};
    std::int16_t select = -1;
    if (g_helpNewEmulatorContext == 1) {
        if (g_helpNewEmulatorPrompt == 1)
            select = 45;
        g_helpNewPromptIconSet[3] = 5;
    } else {
        g_helpNewPromptIconSet[3] = -1;
    }
    g_helpNewPromptIconSet[0] = select;
    for (std::size_t slot = 0; slot < g_helpNewPromptIconSet.size(); ++slot)
        g_searchPromptIconSlots[slot] = g_helpNewPromptIconSet[slot];
    for (std::size_t slot = 0; slot < prompts.size(); ++slot)
        prompts[slot] = static_cast<std::uint16_t>(g_searchPromptIconSlots[slot]);
    return reinterpret_cast<std::int64_t>(prompts.data());
}



void HelpNew_DrawFrame(nk_context &context, UILayoutRect rect, std::uint8_t alpha, std::uint32_t colour, float thickness) {
    nk_window *const window = context.current;
    if (window) {
        rect.x += window->bounds.x;
        rect.y += window->bounds.y;
    }
    if (g_waitingForNextTick != 1 || !window)
        return;
    if (window->layout)
        window->layout->row.item = nk_rect(rect.x, rect.y, rect.width, rect.height);
    nk_color outline{};
    std::memcpy(&outline, &colour, sizeof(colour));
    nk_stroke_rect(&window->buffer, nk_rect(rect.x, rect.y, rect.width, rect.height), 0.0f, thickness, outline);
    if (window->layout)
        window->layout->row.item = nk_rect(rect.x, rect.y, rect.width, rect.height);
    nk_fill_rect(&window->buffer, nk_rect(rect.x, rect.y, rect.width, rect.height), 0.0f, nk_rgba(0, 0, 0, alpha));
}



void HelpNew_DrawOutlinedText(nk_context &context, const UILayoutRect &rect, nk_flags alignment, const char *text) {
    const auto placeItem = [&context, &rect] {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(rect.x, rect.y, rect.width, rect.height);
    };
    const std::int32_t length = text ? static_cast<std::int32_t>(std::strlen(text)) : 0;
    placeItem();
    if (g_uiFontFace != 1) {
        g_uiFontFace = 1;
        UI_SelectFont();
    }
    context.style.text.color = nk_rgba(0, 0, 0, 255);
    if (g_waitingForNextTick == 1)
        nk_text_colored(&context, text, length, alignment, context.style.text.color);
    if (g_uiFontFace != 0) {
        g_uiFontFace = 0;
        UI_SelectFont();
    }
    context.style.text.color = nk_rgba(255, 255, 255, 255);
    if (g_waitingForNextTick == 1) {
        placeItem();
        nk_text_colored(&context, text, length, alignment, context.style.text.color);
    }
}






void HelpNew_RebuildLayout(nk_context &context, const UILayoutRect &area, float lineHeight) {
    const nk_user_font *const font = context.style.font;
    const float bottom = area.height + area.y;
    g_helpNewTitleRect = {.x = area.width * 0.725f + area.x, .y = bottom - lineHeight * 0.5f, .width = area.width * 0.25f, .height = lineHeight};
    g_helpNewBodyRect = {.x = area.width * 0.01f + area.x, .y = bottom - lineHeight, .width = area.width * 0.5f, .height = lineHeight};
    SubtitleStringId title{};
    std::memcpy(&title, "UIHELPPAUS", sizeof("UIHELPPAUS")); // @text EN: "Game Paused" | JA: "ポーズ中"
    std::snprintf(g_helpNewTitleText.data(), g_helpNewTitleText.size(), "%s", ReadStringFromStringTableById(title));
    std::snprintf(g_helpNewBodyText.data(), g_helpNewBodyText.size(), "%s", "");
    std::array<float, 48> widths{};
    for (std::int32_t index = 0; index < g_helpNewRunCount; ++index) {
        HelpNewRun &run = g_helpNewRuns[static_cast<std::size_t>(index)];
        const std::int32_t cell = run.line + 16 * run.column;
        if (cell >= 48)
            break;
        const float column = run.column == 1 ? area.width * 0.16f : run.column == 2 ? area.width * 0.51f : 0.0f;
        const float advance = widths[static_cast<std::size_t>(cell)];
        run.rect.height = lineHeight;
        run.rect.x = advance + column;
        run.rect.y = static_cast<float>(run.line) * lineHeight;
        if (run.kind == 1) {
            run.rect.width = font->width(font->userdata, font->height, run.text.data(), run.length);
        } else {
            const UIButtonIconWidget &icon = g_helpNewIcons[static_cast<std::size_t>(run.icon)];
            const d3t::UIViewImage *const image = icon.field88 == 1 ? g_platformButtonIconImages[0] : g_platformButtonIconImages[2];
            const std::int32_t glyph = ResolveButtonIconGlyph(icon.action, icon.controlMethod, icon.controlState, icon.allowDoubleWidth);
            const struct nk_image sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(icon.sheet), glyph, image);
            run.rect.width = 0.0f;
            run.rect.height = 0.0f;
            const float glyphHeight = static_cast<float>(sprite.region[3]);
            const float glyphWidth = static_cast<float>(sprite.region[2]);
            if (lineHeight != 0.0f && glyphWidth != 0.0f && glyphHeight != 0.0f) {
                if (lineHeight / 1000.0f > glyphHeight / glyphWidth) {
                    run.rect.width = 1000.0f;
                    run.rect.height = 1000.0f / glyphWidth * glyphHeight;
                } else if (glyphHeight / glyphWidth > lineHeight / 1000.0f) {
                    run.rect.height = lineHeight;
                    run.rect.width = lineHeight / glyphHeight * glyphWidth;
                } else {
                    run.rect.width = 1000.0f;
                    run.rect.height = lineHeight;
                }
            }
            run.rect.x += 3.0f;
            widths[static_cast<std::size_t>(cell)] = advance + 6.0f;
        }
        widths[static_cast<std::size_t>(cell)] += run.rect.width;
    }
    for (std::int32_t index = 0; index < g_helpNewRunCount; ++index) {
        HelpNewRun &run = g_helpNewRuns[static_cast<std::size_t>(index)];
        if (run.column == 0)
            run.rect.x += (area.width - widths[static_cast<std::size_t>(run.line)]) * 0.5f;
        run.rect.x += area.x;
        run.rect.y += area.y + 80.0f;
    }
    for (std::int32_t index = 0; index < g_helpNewInfoBlockCount; ++index) {
        HelpNewInfoBlock &block = g_helpNewInfoBlocks[static_cast<std::size_t>(index)];
        if (block.kind == 0) {
            block.rect.x = area.width * 0.15f + area.x;
            block.rect.width = area.width * 0.7f;
        } else if (block.kind == 2) {
            block.rect.x = area.width * 0.15f + area.x;
            block.rect.width = area.width * 0.65f;
        } else {
            block.rect.x = area.width * (block.column ? 0.505f : 0.15f) + area.x;
            block.rect.width = area.width * 0.345f;
        }
        block.rect.y = static_cast<float>(block.line) * lineHeight + (area.y + 80.0f);
        block.rect.height = static_cast<float>(block.lines) * lineHeight;
    }
}







std::int64_t HelpNew_Tick() {
    for (d3t::UIWindow *const window : g_helpNewWindows) {
        if (window)
            window->rectCached = 0;
    }
    nk_context &context = GNuklearWindow;
    const UIBaseRuntime &element = g_uiElements[kUiHelpNew];
    if (!g_helpNewEmulatorContext) {
        const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
        g_currActiveUIIndex = element.index;
        if (slot <= 0x4bU)
            UI_InputHandler(context, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    }
    UILayoutRect panel{};
    UI_GetWindowRect(g_helpNewWindows[1], panel);
    if (g_helpNewEmulatorContext == 1)
        panel.y += panel.height * 0.5f;
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(&context, &g_helpNewPanel, "UIHelpBack", "UIHelpBack", nk_rect(panel.x, panel.y, panel.width, panel.height), NK_WINDOW_NO_SCROLLBAR);
        if (context.current && context.current->layout)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, 2);
    }
    const UILayoutRect area{.x = 50.0f, .y = 20.0f, .width = panel.width - 100.0f, .height = panel.height - 50.0f};
    const float lineHeight = area.height * 0.0625f;
    if (g_uiFontFace != 0) {
        g_uiFontFace = 0;
        UI_SelectFont();
    }
    if (g_uiFontSize != 3) {
        g_uiFontSize = 3;
        UI_SelectFont();
    }
    if (g_helpNewRelayout == 1 || g_keyboardActive != g_helpNewKeyboardActive) {
        g_helpNewRelayout = 0;
        g_helpNewKeyboardActive = g_keyboardActive;
        HelpNew_RebuildLayout(context, area, lineHeight);
    }
    if (!g_helpNewEmulatorContext) {
        HelpNew_DrawFrame(context, area, 0xd8, 0xffffffffU, 1.0f);
        for (std::int32_t index = 0; index < g_helpNewInfoBlockCount; ++index) {
            const HelpNewInfoBlock &block = g_helpNewInfoBlocks[static_cast<std::size_t>(index)];
            HelpNew_DrawFrame(context, block.rect, 0xff, block.colour, 1.0f);
        }
    }
    for (std::int32_t index = 0; index < g_helpNewRunCount; ++index) {
        HelpNewRun &run = g_helpNewRuns[static_cast<std::size_t>(index)];
        if (run.kind == 1) {
            HelpNew_DrawOutlinedText(context, run.rect, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, run.text.data());
            continue;
        }
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(run.rect.x, run.rect.y, run.rect.width, run.rect.height);
        UI_DrawButtonIcon(context, g_helpNewIcons[static_cast<std::size_t>(run.icon)], g_helpNewEmulatorContext == 1 ? &run.rect : nullptr);
    }
    if (!g_helpNewEmulatorContext) {
        HelpNew_DrawFrame(context, g_helpNewTitleRect, 0xff, 0xffffffffU, 1.0f);
        const std::int32_t slot = g_helpNewTransitionAnimSlot;
        if (slot >= 0 && std::bit_cast<std::int32_t>(g_uiAnimSlots[static_cast<std::size_t>(slot)].current[0]) == 1)
            HelpNew_DrawOutlinedText(context, g_helpNewTitleRect, NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, g_helpNewTitleText.data());
        HelpNew_DrawOutlinedText(context, g_helpNewBodyRect, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, g_helpNewBodyText.data());
    }
    if (g_waitingForNextTick == 1 && context.current) {
        if (context.current->layout) {
            nk_row_layout &row = context.current->layout->row;
            row.item_width = 0.0f;
            row.item_height = 0.0f;
            row.item_offset = 0.0f;
            row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    return 0;
}
}

namespace shenmue::s1 {


void VFMA_ICON() {
    if (!g_updateRenderFrameMode)
        return;
    OriginalStrip strip{};
    strip.flags = 0x400;
    strip.corner0 = 0xffffffffU;
    strip.normalizedX3 = 1.0f;
    strip.normalizedY2 = 1.0f;
    if (!g_helpButtonId) {
        if (!g_xinputReserved)
            g_uiElements[kUiHelpNew].isActive = 1;
        g_uiElements[kUi70ManBattle].isActive = 1;
        g_70ManIconTransitionState = 4;
        g_70mbStartPromptIconVisible = (g_helpPresentation.animationFrame & 0x10U) == 0;

        strip.flags = 0;
        strip.corner0 = 0xbf000000U;
        AdjustAspectRatioMatrix(&strip, 5, 320, 240, 640, 480, 3);
        ApplyFadeToPacket(&strip, 7.0f);
        return;
    }
    strip.corner0 = (static_cast<std::uint32_t>(g_helpPresentation.alpha) << 24U) | 0xffffffU;
    if (g_helpButtonId <= 2) {
        g_uiElements[kUiVfGameOver].isActive = 1;
        const std::int32_t size = g_helpButtonId == 1 ? 256 : 128;
        AdjustAspectRatioMatrix(&strip, 5, 240, 240, size, size, 4);
        AdjustAspectRatioMatrix(&strip, 5, 416, 240, 256, 256, 4);
    } else if (g_helpButtonId == 3) {
        if (!g_lcdmHelpIconSpritesLoaded) {
            g_helpIconWidth = 320;
            g_helpIconHeight = 240;
        }
        AdjustAspectRatioMatrix(&strip, 5, g_helpIconWidth, g_helpIconHeight, 256, 256, 4);
    } else if (static_cast<std::uint8_t>(g_helpButtonId - 4) <= 1U) {
        AdjustAspectRatioMatrix(&strip, 5, 288, 240, 64, 64, 4);
        AdjustAspectRatioMatrix(&strip, 5, 360, 240, 128, 128, 4);
    }
}


// @verified - p
std::uint64_t shenLoadSprite(std::int8_t transition, std::uint8_t fadeTicks, std::uint8_t buttonId) {
    HelpPresentationOwner &help = g_helpPresentation;
    const std::uint32_t step = fadeTicks ? 0x100U / fadeTicks : 0x100U;
    help.iconFlags |= 1U;
    g_helpButtonId = buttonId;
    help.alphaStep = static_cast<std::int16_t>(step);
    help.transitionCommand = static_cast<std::uint8_t>(transition);
    help.animationFrame = 0;
    if (static_cast<std::uint8_t>(buttonId - 4) <= 1U)
        return step;
    std::array<std::int32_t, 3> &surfaces = g_seventyManBattlePresentation.iconSurfaces;
    for (std::int32_t &surface : surfaces) {
        if (surface != -1)
            PVR_FreeAllocatedSurfaceByIndex(surface);
        surface = -1;
    }
    const char *filename = buttonId == 1 ? "TNGMOV00.SPR" : buttonId == 2 ? "TNTMOV00.SPR" : buttonId == 3 ? "TNRANKIN.SPR" : "TNPUSH_A.SPR";
    std::array<char, 260> path{};
    std::snprintf(path.data(), path.size(), "sprite/%s", filename);
    const std::int32_t fileSize = static_cast<std::int32_t>(OpenFileInternal(path.data()));
    std::byte *const file =
        static_cast<std::byte *>(HLib_AllocHeapMemBlock(static_cast<std::uint32_t>(fileSize), static_cast<std::int32_t>(kVfPromptSpriteAllocationToken.name)));
    shenLoadFile("sprite", filename, file, fileSize);
    if (file) {
        std::size_t index = 0;
        std::byte *cursor = file;
        while (cursor < file + fileSize) {
            std::uint32_t kind = 0;
            std::memcpy(&kind, cursor, sizeof(kind));
            if (!kind)
                break;
            surfaces[index++] = LoadTEXN(cursor);
            std::uint32_t size = 0;
            std::memcpy(&size, cursor + 4, sizeof(size));
            cursor += size;
        }
    }
    return HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(file), 0);
}




void LoadRetrySprite(std::uint8_t token) {
    HelpPresentationOwner &help = g_helpPresentation;
    if (help.retryIconFlag == token)
        return;
    help.retryIconFlag = 14;
    g_lcdmHelpIconSpritesLoaded = 0;
    const std::int32_t fileSize = static_cast<std::int32_t>(OpenFileInternal("sprite/TNRETRY0.SPR"));
    std::byte *const file =
        static_cast<std::byte *>(HLib_AllocHeapMemBlock(static_cast<std::uint32_t>(fileSize), static_cast<std::int32_t>(kVfPromptSpriteAllocationToken.name)));
    shenLoadFile("sprite", "TNRETRY0.SPR", file, fileSize);
    if (file) {
        std::size_t index = 0;
        std::byte *cursor = file;
        while (cursor < file + fileSize && index < g_helpRetrySurfaces.size()) {
            std::uint32_t kind = 0;
            std::memcpy(&kind, cursor, sizeof(kind));
            if (!kind)
                break;
            g_helpRetrySurfaces[index++] = LoadTEXN(cursor);
            std::uint32_t size = 0;
            std::memcpy(&size, cursor + 4, sizeof(size));
            cursor += size;
        }
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(file), 0);
    g_seventyManBattlePresentation.iconSurfaces = {-1, -1, -1};
}


void UI_HELPSetEmulatorContext(bool enabled) {
    g_helpNewEmulatorContext = enabled ? 1 : 0;
}


void UI_HELPClearEmulatorPrompt() {
    g_helpNewEmulatorPrompt = 0;
}



void UI_HELPStartTransitionAnim() {
    if (g_helpNewTransitionAnimSlot == -1)
        g_helpNewTransitionAnimSlot = UIAnimAllocSlot();
    if (g_helpNewTransitionAnimSlot == -1)
        return;
    UIAnimSlot &slot = g_uiAnimSlots[static_cast<std::size_t>(g_helpNewTransitionAnimSlot)];
    slot.allocated = 1;
    slot.status = 1;
    slot.interpolation = 3;
    slot.start[0] = std::bit_cast<float>(0);
    slot.end[0] = std::bit_cast<float>(2);
    slot.current[0] = slot.start[0];
    slot.delay = 0.0f;
    slot.elapsed = 0.0f;
    slot.duration = 1.0f;
    slot.loopMode = 1;
    slot.reserved40 = -1;
}
}

namespace shenmue::s1::ui_runtime_detail {

// The index-th character of text's size bytes as Shenmue_FormatHelpString and ParseHelpTextRuns find it, decoding from the start: its
// codepoint, its length (left as it was when the text ends first) and where it starts (null when the text ends before it).
const char *HelpText_Character(const char *text, std::int32_t size, std::int32_t index, std::uint32_t &codepoint, std::int32_t &length) {
    std::int32_t at = 0;
    std::int32_t offset = 0;
    std::int32_t decoded = DecodeUITextCharacter(text, &codepoint, size);
    if (decoded) {
        while (at != index) {
            offset += decoded;
            ++at;
            decoded = DecodeUITextCharacter(text + offset, &codepoint, size - offset);
            if (!decoded)
                return at == index ? text + offset : nullptr;
        }
        length = decoded;
    }
    return at == index ? text + offset : nullptr;
}




void AppendHelpInfoBlock(std::int32_t colour, std::int32_t column, std::int32_t line, std::int32_t kind, std::int32_t lines) {
    if (g_helpFlag || static_cast<std::uint32_t>(g_helpNewInfoBlockCount) >= g_helpNewInfoBlocks.size())
        return;
    HelpNewInfoBlock &block = g_helpNewInfoBlocks[static_cast<std::size_t>(g_helpNewInfoBlockCount++)];
    if (static_cast<std::uint32_t>(colour) < kHelpNewFrameColours.size())
        block.colour = kHelpNewFrameColours[static_cast<std::size_t>(colour)];
    block.column = column != 1 ? 1 : 0;
    block.line = line;
    block.lines = lines + 1;
    block.kind = kind;
}


void HandleHelpStringInfos(std::uint32_t token) {
    if (token >= kHelpNewPageBlocks.size())
        return;
    const HelpNewPageBlocks &page = kHelpNewPageBlocks[token];
    for (std::int32_t block = 0; block < page.count; ++block) {
        const std::array<std::int8_t, 5> &args = page.blocks[static_cast<std::size_t>(block)];
        AppendHelpInfoBlock(args[0], args[1], args[2], args[3], args[4]);
    }
}






UIButtonIconWidget HelpNew_InitPlatformButtonIconSpec(std::int32_t id) {
    const std::int32_t action = g_helpNewEmulatorContext == 1 ? 3 : UI_GetActionForId(id);
    UIButtonIconWidget spec{};
    spec.inUse = 1;
    std::snprintf(spec.id.data(), spec.id.size(), "UIHelp::PlatformButtonIcon_ID:%i_Action:%i", id, action);
    if (g_platformButtonIconImages[0])
        std::snprintf(spec.image.data(), spec.image.size(), "%s", g_platformButtonIconImages[0]->name);
    spec.window = UI_FindViewWindow(static_cast<std::uint32_t>(g_uiElements[kUiHelpNew].config.uiHandleIndex), "Help_MainPanel");
    spec.action = action;
    spec.field88 = 1;
    spec.iconFontSize = 4;
    spec.allowDoubleWidth = 1;
    spec.controlMethod = -1;
    const std::uint8_t page = g_helpOverlayPageTokenId;
    if (static_cast<std::uint32_t>(id - 6) <= 1U && page == 51) {
        spec.controlState = 3;
    } else {
        switch (page) {
        case 4:
        case 27:
            spec.controlState = 5;
            break;
        case 13:
        case 14:
        case 15:
        case 16:
        case 20:
        case 55:
            spec.controlState = 3;
            break;
        case 18:
        case 19:
        case 24:
        case 51:
            spec.controlState = 1;
            break;
        case 21:
        case 35:
            spec.controlState = 4;
            break;
        case 22:
        case 23:
            spec.controlState = 6;
            break;
        default:
            spec.controlState = 2;
            break;
        }
    }
    spec.fontColour = 0xff000000U;
    spec.alignment = 18;
    spec.sheet = static_cast<std::int32_t>(g_platformIconsSheet);
    spec.colour = 0xffffffffU;
    return spec;
}



void HelpNew_AppendPlatformButtonIconSpec(std::int32_t id) {
    if (static_cast<std::uint32_t>(g_helpNewIconCount) >= g_helpNewIcons.size())
        return;
    g_helpNewIcons[static_cast<std::size_t>(g_helpNewIconCount)] = HelpNew_InitPlatformButtonIconSpec(id);
    g_helpNewIconActions[static_cast<std::size_t>(g_helpNewIconCount++)] = UI_GetActionForId(id);
}




void ParseHelpTextRuns() {
    const std::int32_t size = static_cast<std::int32_t>(std::strlen(g_helpFormattedText.data()));
    const std::int32_t count = FillStringData(g_helpFormattedText.data(), size);
    const std::int32_t line = g_helpNewLineCount;
    std::array<char, 256> text{};
    std::int32_t length = 0;
    std::int32_t column = 0;
    std::int32_t characterLength = count;
    const auto flush = [&] {
        if (length <= 0 || g_helpNewRunCount >= 64)
            return;
        HelpNewRun &run = g_helpNewRuns[static_cast<std::size_t>(g_helpNewRunCount++)];
        for (std::size_t at = 0; at < run.text.size(); ++at)
            run.text[at] = static_cast<std::int32_t>(at) < length ? text[at] : '\0';
        run.length = length;
        run.column = column;
        run.line = line;
        run.kind = 1;
    };
    for (std::int32_t index = 0; index < count; ++index) {
        std::uint32_t codepoint = 0;
        const char *const character = HelpText_Character(g_helpFormattedText.data(), size, index, codepoint, characterLength);
        if (codepoint == '\t') {
            flush();
            length = 0;
            ++column;
        } else if (codepoint == '^') {
            flush();
            const std::int32_t icon = g_helpNewNextIcon++;
            if (icon != -1 && g_helpNewRunCount < 64) {
                HelpNewRun &run = g_helpNewRuns[static_cast<std::size_t>(g_helpNewRunCount++)];
                run.column = column;
                run.line = line;
                run.icon = icon;
                run.kind = 2;
            }
            length = 0;
        } else if ((length != 0 || codepoint != ' ') && character && length + characterLength <= static_cast<std::int32_t>(text.size())) {
            std::memcpy(text.data() + length, character, static_cast<std::size_t>(characterLength));
            length += characterLength;
        }
    }
    flush();
    g_helpNewRelayout = 1;
    g_helpNewLineCount = line + 1;
}









void FormatHelpString(const char *text) {
    if (!text)
        return;
    const std::int32_t size = static_cast<std::int32_t>(std::strlen(text));
    const std::int32_t count = FillStringData(text, size);
    HandleHelpStringInfos(g_helpOverlayPageTokenId);
    std::uint32_t codepoint = 0;
    std::int32_t characterLength = 0;
    std::uint32_t skip = g_helpColumnSkip;
    std::uint32_t at = g_helpFormattedLength;
    for (std::int32_t index = 0; index < count; ++index) {
        const char *const character = HelpText_Character(text, size, index, codepoint, characterLength);
        if (codepoint == '%') {
            if (at >= g_helpFormattedText.size())
                return;
            g_helpFormattedText[at] = '\0';
            if (g_helpLegacyLineCount < 32) {
                std::array<char, 128> &copy = g_helpLegacyLines[static_cast<std::size_t>(g_helpLegacyLineCount++)];
                std::snprintf(copy.data(), copy.size(), "%s", g_helpFormattedText.data());
            }
            ParseHelpTextRuns();
            const std::uint32_t counter = g_helpLineCounter + 2;
            g_helpFormattedText[0] = '\0';
            g_helpFormattedLength = 0;
            at = 0;
            g_helpLineCounter = counter;
            if (counter < g_helpLineWidths.size()) {
                g_helpLineWidths[counter] = 0;
                skip = 0;
                g_helpColumnSkip = 0;
                g_helpColumnOffset = 0;
                continue;
            }
        } else if (codepoint == '@') {
            if (!skip) {
                if (at >= g_helpFormattedText.size())
                    return;
                g_helpFormattedText[at++] = '\t';
                g_helpFormattedLength = at;
                continue;
            }
            g_helpColumnSkip = ++skip;
        } else if (codepoint == '^') {
            HelpText_Character(text, size, ++index, codepoint, characterLength);
            const std::int16_t slot = g_helpIconRecordCount;
            if (slot < 10) {
                HelpIconRecord &record = g_helpIconRecords[static_cast<std::size_t>(slot)];
                record.line = static_cast<std::uint8_t>(g_helpLineCounter >> 1);
                record.column = static_cast<std::uint8_t>(g_helpColumnOffset + at);
                const bool trade = (g_keyboardActive == 1 || !g_currentSettings.input.swapTriggers) && g_helpOverlayPageTokenId <= 55 &&
                    ((0x8800000011e000ULL >> g_helpOverlayPageTokenId) & 1U) != 0;
                switch (codepoint) {
                case '2':
                    record.id = 20;
                    break;
                case '4':
                    record.id = 17;
                    break;
                case '6':
                    record.id = 18;
                    break;
                case '8':
                    record.id = 19;
                    break;
                case 'A':
                    record.id = 2;
                    break;
                case 'B':
                    record.id = 3;
                    break;
                case 'C':
                    record.id = 0;
                    break;
                case 'D':
                    record.id = 23;
                    break;
                case 'G':
                    record.id = 16;
                    break;
                case 'H':
                    record.id = 21;
                    break;
                case 'K':
                    record.id = 14;
                    break;
                case 'L':
                    record.id = trade ? 7 : 6;
                    break;
                case 'N':
                    record.id = 1;
                    break;
                case 'O':
                    record.id = 15;
                    break;
                case 'P':
                    record.id = 13;
                    break;
                case 'R':
                    record.id = trade ? 6 : 7;
                    break;
                case 'S':
                    record.id = 8;
                    break;
                case 'T':
                    record.id = 10;
                    break;
                case 'V':
                    record.id = 22;
                    break;
                case 'W':
                    record.id = 7;
                    break;
                case 'X':
                    record.id = 4;
                    break;
                case 'Y':
                    record.id = 5;
                    break;
                case 'Z':
                    record.id = 6;
                    break;
                default:
                    record.id = -1;
                    break;
                }
                if (record.id >= 0 && at + 2 <= g_helpFormattedText.size()) {
                    g_helpFormattedText[at] = ' ';
                    g_helpFormattedText[at + 1] = '^';
                    g_helpFormattedLength = at + 2;
                    HelpNew_AppendPlatformButtonIconSpec(record.id);
                    if (g_helpLegacyIconCount < 12) {
                        const std::size_t copy = static_cast<std::size_t>(g_helpLegacyIconCount++);
                        g_helpLegacyIconIds[copy] = record.id;
                        g_helpLegacyIconLines[copy] = record.line;
                        g_helpLegacyIconColumns[copy] = static_cast<float>(record.column);
                    }
                    at = g_helpFormattedLength;
                    skip = g_helpColumnSkip;
                    g_helpIconRecordCount = static_cast<std::int16_t>(g_helpIconRecordCount + 1);
                }
            }
        } else if (character && at + static_cast<std::uint32_t>(characterLength) <= g_helpFormattedText.size()) {
            std::memcpy(g_helpFormattedText.data() + at, character, static_cast<std::size_t>(characterLength));
            at += static_cast<std::uint32_t>(characterLength);
            g_helpFormattedLength = at;
        }
        if (skip > 1) {
            skip = 1;
            g_helpColumnSkip = 1;
            while (codepoint != '%' && index + 1 < count)
                HelpText_Character(text, size, ++index, codepoint, characterLength);
        }
    }
}




void SelectFont(bool waza) {
    switch (g_currentHelpTokenId) {
    case 10:
    case 13:
    case 31:
    case 32:
    case 33:
    case 34:
    case 51:
    case 60:
        LoadFont(waza ? "WAZA.FON" : "DC_KANJI.FON");
        return;
    case 19:
        break;
    default:
        LoadFont("DC_KANJI.FON");
        break;
    }
    const MapControlData *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    HLib::Task *const task = control && control->windTask ? control->windTask : g_windTask;
    WindTaskData *const wind = task ? reinterpret_cast<WindTaskData *>(HLib_GetTaskDataForTask(task)) : nullptr;
    if (wind)
        wind->tailWord = g_currentHelpTokenId == 19 && waza ? 0U : 1U;
}




void SetHelpText(const char *text) {
    const char *cursor = text + 1;
    std::int32_t pendingSpaces = 0;
    char character = *text;
    if (!character)
        return;
    std::int32_t length = g_helpTextRunLength;
    std::int32_t width = g_helpTextWidth;
    for (;;) {
        if (character == '^') {
            ++cursor;
            pendingSpaces = 2;
            character = ' ';
            continue;
        }
        if (character == '@')
            ++cursor;
        if (character == '%' || character == '@' || length >= 36) {
            if (length != 0) {
                width += kHelpTextRunWidths[static_cast<std::size_t>(length - 1)];
                g_helpTextWidth = width;
            }
            length = 0;
        } else {
            ++length;
        }
        g_helpTextRunLength = length;
        if (pendingSpaces <= 0) {
            character = *cursor++;
            if (!character)
                return;
        } else {
            character = ' ';
            --pendingSpaces;
        }
    }
}




bool HELP_TextExceedsHeapRoom() {
    std::uint64_t largest = 0;
    for (HLib::TaskData *block = g_mallocFirst; block; block = block->next_ptr) {
        if ((reinterpret_cast<std::uintptr_t>(block + 1) & 0x1fU) != 0)
            break;
        if (block->status != HLIB_USED_TOKEN.name || block->taskToken == 0) {
            const std::uint64_t payload = static_cast<std::uint64_t>(block->content_size) - 0x40U;
            if (largest < payload)
                largest = payload;
        }
        if (block->next_ptr == g_mallocLast)
            break;
    }
    const std::uint32_t room = static_cast<std::uint32_t>(largest);
    const std::int32_t needed = static_cast<std::int32_t>(static_cast<std::uint32_t>(g_helpTextWidth) << 5U);
    return room != 0 && room < static_cast<std::uint64_t>(static_cast<std::int64_t>(needed));
}




void RenderHelpOverlay() {
    if (!GetInputStateForHelp() && g_controllerHelpOverlayState != 1)
        return;
    g_uiElements[69].isActive = 1;
    g_uiElements[kUiHelpNew].isActive = 1;
    TryInvokeRegisteredCallback(kUiHelpNew);
}
}

namespace shenmue::s1 {




void UI_HELPPreparePage(std::uint8_t token) {
    g_currentHelpIconState = 0xffffU;
    g_helpLineWidths[0] = 0;
    g_helpFormattedLength = 0;
    g_helpColumnSkip = 0;
    g_helpLineCounter = 0;
    g_helpIconRecordCount = 0;
    g_helpColumnOffset = 0;
    g_helpNewRunCount = 0;
    g_helpNewLineCount = 0;
    g_helpFlag = 0;
    g_helpNewNextIcon = 0;
    g_helpNewInfoBlockCount = 0;
    g_helpNewIconCount = 0;
    g_helpOverlayPageTokenId = token;
    if (token < kHelpStringIds.size())
        FormatHelpString(InitAndRetrieveStringFromID(kHelpStringIds[token]));
    LoadPresentationDefinition(kUiHelpNew, "/ui/gamehelp/Help_New.ui");
    LoadPresentationDefinition(kUiPlatformIcons, "/ui/PlatformIcons.ui");
}

void UI_HELPDispatch(std::int32_t mode, std::uint8_t helpIconId, std::uint8_t buttonId) {
    EnsureHelpTask();
    HelpPresentationOwner &help = g_helpPresentation;
    if (mode == 0) {
        g_previousHelpTokenId = g_currentHelpTokenId;
        g_helpButtonId = buttonId;
        g_defaultHelpTokenId = static_cast<std::int8_t>(helpIconId) < 68 ? helpIconId : 0;
        g_currentHelpTokenId = g_defaultHelpTokenId;
        return;
    }

    if (mode == 1)
        LoadRetrySprite(helpIconId);
    else
        shenLoadSprite(static_cast<std::int8_t>(mode), helpIconId, buttonId);
}

HLib::Task *UI_HELPGetTask() {
    HLib::Task *const task = g_helpPresentation.task;
    return task && task->taskIndex == 15 && !task->task_data_ptr && task->taskToken == static_cast<std::int32_t>(SHEN_NAME(HELP)) &&
            (task->mainTaskCallbackFnPtr == UI_HELPInitCallback || task->mainTaskCallbackFnPtr == UI_HELPCallback) ?
        task :
        nullptr;
}

void UI_HELPClear() {
    HelpPresentationOwner &help = g_helpPresentation;
    help.onScreenCount = 0;
    help.transitionCommand = 0;
    help.iconFlags = 0;
    help.alpha = 0;
    help.closing = false;
    g_helpOverlayState = UIHelpWidgetState::Idle;
    help.restoreFrames = 0;
    ResetOwnedElement(kUiHelpNew);
    g_helpNewRunCount = 0;
    g_helpNewEmulatorContext = 0;
    g_helpNewEmulatorPrompt = 0;
}


// @verified - p
void UI_HELPInitCallback(HLib::TaskData *) {
    HelpPresentationOwner &help = g_helpPresentation;
    help.task = CurrentTask;
    help.iconPacket.depth = 15.0f;
    help.pagePacket.depth = 16.0f;
    help.pagePacket.extent = 32;
    help.iconPacket.zeroWord = 0;
    help.iconPacket.packedFlags = 0x200400;
    help.iconPacket.scaleX = 1.0f;
    help.iconPacket.scaleY = 1.0f;
    help.iconPacket.zeroDword = 0;
    help.iconPacket.mode = 4;
    help.pagePacket.packedFlags = 0x20012a;
    help.pagePacket.zeroWord = 0;
    help.pagePacket.scaleX = 1.2f;
    help.pagePacket.scaleY = 1.2f;
    help.pagePacket.zeroDword = 0;
    help.pagePacket.mode = 4;
    InstallOwnedTaskMain(UI_HELPCallback);
}






















void UI_HELPCallback(HLib::TaskData *) {
    HelpPresentationOwner &help = g_helpPresentation;
    switch (g_helpOverlayState) {
    case UIHelpWidgetState::Idle: {
        if (static_cast<std::uint8_t>(g_moveListScreenState - 3) <= 4U || g_moveListScreenState == 9 ||
            ((help.iconFlags & 1U) != 0 && static_cast<std::uint8_t>(g_helpButtonId - 4) > 1U) ||
            (((Shenmue_nPauseState & PAUSE_STATE_PAUSED) == 0 || g_NumFramesProcessed) && g_controllerHelpOverlayState != 1))
            break;
        SelectFont(false);
        std::uint8_t token = g_currentHelpTokenId;
        g_helpOverlayState = UIHelpWidgetState::PreparePage;
        g_defaultHelpTokenId = g_currentHelpTokenId;
        const std::uint8_t conversationPage = ((S1SettingsFlags_Get() >> 4U) & 3U) == 0 ? 43 : 38;
        if (token == 21) {
            token = 35;
        } else if (token == 0 || (((token - 37) & 0xf6) == 0 && token != 38)) {
            const ScenRuntimeObject *const object = g_scenarioFunctionWork ? g_scenRuntimeState.activeObject : nullptr;
            const std::uint32_t name = object && object->runtimeState != 3 ? object->name.name : 0U;
            if (name && name != g_cpctLastOperand && name != g_cpctPreviousOperand && name != g_scenarioStepLut.pendingCharacterName &&
                (!g_scenHelpPromptOverride || g_searchTemporaryFocusedCharacter || static_cast<std::int32_t>(g_scenRuntimeState.focusBestAngle) <= 10922))
                token = conversationPage;
        }
        if (g_currentHelpTokenId != 36 && (SCEN_IsInteractionModalActive() || g_searchTemporaryFocusedCharacter))
            token = conversationPage;
        else if (token > 67)
            token = 21;
        g_currentHelpTokenId = token;
        std::uint8_t &displayCount = g_playerStats.pages.helpCounts[token];
        if (++displayCount == 0)
            displayCount = 0xff;
        g_helpOverlayPageTokenId = token;
        break;
    }
    case UIHelpWidgetState::PreparePage: {
        const std::int16_t firstSurface = static_cast<std::int16_t>(g_currentHelpIconState);
        for (std::int32_t index = firstSurface; index < 20; ++index) {
            const std::int16_t widthHigh = static_cast<std::int16_t>(static_cast<std::uint32_t>(g_helpTextWidth) >> 16U);
            const std::int16_t surface = index < 0 ? widthHigh : g_helpIconSurfaceIds[static_cast<std::size_t>(index)];
            if (surface == -1)
                continue;
            PVR_FreeAllocatedSurfaceByIndex(surface);
            if (index >= 0)
                g_helpIconSurfaceIds[static_cast<std::size_t>(index)] = -1;
        }
        std::uint8_t page = g_helpOverlayPageTokenId;
        g_helpTextRunLength = 0;
        g_helpTextWidth = 0;
        SetHelpText(InitAndRetrieveStringFromID(page < kHelpStringIds.size() ? kHelpStringIds[page] : nullptr));
        if (g_helpTextMaxWidth < g_helpTextWidth)
            g_helpTextMaxWidth = g_helpTextWidth;
        const bool tooWide = HELP_TextExceedsHeapRoom();
        g_currentHelpIconState = 0xffffU;
        g_helpLineWidths[0] = 0;
        g_helpFormattedLength = 0;
        if (tooWide)
            page = 21;
        g_helpColumnSkip = 0;
        g_helpOverlayPageTokenId = page;
        g_helpLineCounter = 0;
        g_helpIconRecordCount = 0;
        g_helpColumnOffset = 0;
        if (GetInputStateForHelp() || g_controllerHelpOverlayState == 1 || g_pauseInputTransitionPending == 1) {
            d3t::ActivateElementByIdx(kUiHelpNew);
            g_pauseInputTransitionPending = 0;
        } else if ((g_inputContextFlags & (1ULL << 44U)) == 0) {
            g_helpOverlayState = UIHelpWidgetState::RestorePrevious;
            ModifyTasksForPauseScreen();
            return;
        }
        g_helpLegacyLines[0][0] = '\0';
        g_helpLegacyLineCount = 0;
        g_helpNewRunCount = 0;
        g_helpNewLineCount = 0;
        g_helpFlag = 0;
        g_helpNewNextIcon = 0;
        g_helpNewInfoBlockCount = 0;
        g_helpNewIconCount = 0;
        g_helpLegacyIconCount = 0;
        const char *const stringId = page < kHelpStringIds.size() ? kHelpStringIds[page] : nullptr;
        std::snprintf(g_currentHelpText.data(), g_currentHelpText.size(), "%s", InitAndRetrieveStringFromID(stringId));
        FormatHelpString(InitAndRetrieveStringFromID(stringId));

        LoadPresentationDefinition(kUiHelpNew, "/ui/gamehelp/Help_New.ui");
        LoadPresentationDefinition(kUiPlatformIcons, "/ui/PlatformIcons.ui");
        help.iconFlags |= 0x80U;
        g_helpOverlayState = UIHelpWidgetState::Active;
        ++help.onScreenCount;
        g_currentHelpIconState = 0;
        break;
    }
    case UIHelpWidgetState::Active: {
        SEQ_Cleanup();
        if (g_debugHelpPaging) {
            const std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
            const std::int8_t page = static_cast<std::int8_t>(g_helpOverlayPageTokenId);
            if ((pressed & 0x80U) != 0 && page < 64) {
                ++g_helpOverlayPageTokenId;
                g_helpOverlayState = UIHelpWidgetState::PreparePage;
            } else if ((pressed & 0x40U) != 0 && page > 0) {
                --g_helpOverlayPageTokenId;
                g_helpOverlayState = UIHelpWidgetState::PreparePage;
            }
        }
        if ((Shenmue_nPauseState & PAUSE_STATE_PAUSED) != 0 || g_controllerHelpOverlayState != 0)
            break;
        SelectFont(true);
        g_helpOverlayState = UIHelpWidgetState::RestorePrevious;
        d3t::ResetElementByIdx(kUiHelpNew);
        if (g_currentHelpTokenId == 66) {
            g_defaultHelpTokenId = g_previousHelpTokenId;
            g_currentHelpTokenId = g_previousHelpTokenId;
        }
        help.restoreFrames = 4;
        if (g_currentHelpTokenId == 15)
            g_prevHelpIconFlag = 1;
        break;
    }
    case UIHelpWidgetState::RestorePrevious:
        if (--help.restoreFrames >= 0)
            break;
        g_helpOverlayState = UIHelpWidgetState::Idle;
        help.onScreenCount = help.onScreenCount > 0 ? help.onScreenCount - 1 : 0;
        help.iconFlags &= 0x7fU;
        g_currentHelpTokenId = g_defaultHelpTokenId;
        break;
    case UIHelpWidgetState::ForceClose:
        help.onScreenCount = 0;
        g_helpOverlayState = UIHelpWidgetState::Idle;
        break;
    case UIHelpWidgetState::ForceOpen:
        ++help.onScreenCount;
        g_helpOverlayState = UIHelpWidgetState::Idle;
        break;
    case UIHelpWidgetState::ResetToIdle:
        g_helpOverlayState = UIHelpWidgetState::Idle;
        break;
    default:
        break;
    }

    std::int16_t alpha = help.alpha;
    if (help.transitionCommand == 5) {
        ++help.onScreenCount;
        alpha = 0;
        help.closing = false;
        help.alpha = 0;
        help.transitionCommand = 0;
    } else if (help.transitionCommand == 7) {
        help.closing = true;
        alpha = 255;
        help.alpha = 255;
        help.transitionCommand = 0;
    }
    const std::int16_t iconState = static_cast<std::int16_t>(g_currentHelpIconState);
    if ((Shenmue_nPauseState & (PAUSE_STATE_PAUSED | PAUSE_STATE_FREE)) != PAUSE_STATE_PAUSED && iconState >= 0 &&
        g_helpOverlayState == UIHelpWidgetState::RestorePrevious && iconState < 20) {
        for (std::int32_t index = iconState; index < iconState + 5 && index < 20; ++index) {
            std::int16_t &surface = g_helpIconSurfaceIds[static_cast<std::size_t>(index)];
            if (surface != -1) {
                PVR_FreeAllocatedSurfaceByIndex(surface);
                surface = -1;
            }
        }
        g_currentHelpIconState = static_cast<std::uint16_t>(g_currentHelpIconState + 5U);
    }
    if ((help.iconFlags & 0x81U) == 1U) {
        if (!help.closing) {
            alpha = static_cast<std::int16_t>(alpha + help.alphaStep);
            help.alpha = alpha > 255 ? 255 : alpha;
        } else {
            alpha = static_cast<std::int16_t>(alpha - help.alphaStep);
            help.alpha = alpha;
            if (alpha < 0) {
                help.alpha = 0;
                help.onScreenCount = help.onScreenCount > 0 ? help.onScreenCount - 1 : 0;
                g_lcdmHelpIconSpritesLoaded = 0;
                help.iconFlags &= 0xfeU;
            }
        }
        ++help.animationFrame;
    }
    if (!help.onScreenCount)
        return;
    if ((help.iconFlags & 1U) != 0) {
        if (static_cast<std::uint8_t>(g_helpButtonId - 4) > 1U || g_helpOverlayState != UIHelpWidgetState::Active) {
            VFMA_ICON();
            return;
        }
    } else if (g_helpOverlayState != UIHelpWidgetState::Active) {
        return;
    }
    if ((Shenmue_nPauseState & PAUSE_STATE_HIDE_HELP) == 0)
        RenderHelpOverlay();
}
}
