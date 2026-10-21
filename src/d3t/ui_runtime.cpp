#include "d3t/ui_internal.hpp"

#if SHENMUE_IS_S2
#include "s2/ui_scroll_list.hpp"
#endif

#if SHENMUE_IS_S1
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/d3t/d3t_texture.hpp"
#endif

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif

#if !SHENMUE_IS_S1


std::int32_t g_waitingForNextTick = 1;

std::uint32_t g_uiScreenWidth = 1920;

std::int32_t g_debugUnknown0 = 0;

std::uint8_t g_NuklearInitialized = 0;
std::int32_t g_nuklearVertexBuffer = 0;
std::int32_t g_nuklearIndexBuffer = 0;
std::uint32_t g_Nuklear_num_verts{};
std::uint32_t g_Nuklear_num_indices{};
d3t::D3TDeviceContext *g_nuklearRenderDeviceContext{};
Vector4 g_currentUIViewportRect{};
Vector2 g_currentUIViewportDepth{};
bool g_bOrthographicRender = true;
Matrix4x4 g_notebookPageMatrix{};
std::array<float, 2> g_nuklearArmedImageSize{};
NuklearFrameCallback g_nuklearFrameCallback{};
bool g_nuklearTargetRedirectPending{};
const std::uint32_t *g_nuklearTargetRedirect{};
#endif

#if !SHENMUE_IS_S1
std::array<UIAsyncLoadRequest, 8192> g_uiLoadRequests{};
std::atomic<std::uint16_t> g_uiLoadRequestIndex{};
std::array<UIViewImageLoadDiagnostic, 83> g_uiImageLoadDiagnostics{};
std::int32_t g_uiImageLoadDiagnosticIndex = -1;
#endif


nk_context GNuklearWindow{};


nk_draw_null_texture g_nuklearNullTexture{};
nk_buffer g_nuklearCommands{};
std::array<std::array<UIFontFace, 4>, 7> g_uiFonts{};
#if SHENMUE_IS_S1
std::array<std::vector<std::byte>, 7> g_fontDefinitions;
#else
std::array<std::byte *, 7> g_uiFontDefinitions{};
#endif

nk_draw_null_texture g_uiFontNullTexture{};


std::int32_t g_uiFontSize = -1;
std::int32_t g_uiFontFace = -1;
float g_uiFontScale = 1.0f;
std::array<std::int32_t, 7> g_fontLanguageAvailable{1, 1, 1, 1, 1, 1, 1};
std::int32_t g_forceFontLanguageAvailable = 0;
#if SHENMUE_IS_S2
std::int32_t g_pendingUIFontLanguageValid{};
std::int32_t g_pendingUIFontLanguage{};
#endif

static_assert(offsetof(nk_context, stacks.fonts.head) == 0x2dd0);

void SetupNuklear() {
#if SHENMUE_IS_S1
    if (g_NuklearInitialized)
        return;
#endif
    if (!g_NuklearInitialized) {
        nk_init_default(&GNuklearWindow, nullptr);
        g_nuklearNullTexture.texture.id = static_cast<int>(d3t::g_defaultWhiteTexture);
        nk_buffer_init_default(&g_nuklearCommands);
#if SHENMUE_IS_S2
        CreateDXTextureForNuklearBufferForScreen();
#endif
        GNuklearWindow.style.window.fixed_background = nk_style_item_hide();
        GNuklearWindow.style.button.normal = nk_style_item_hide();
        GNuklearWindow.style.button.hover = nk_style_item_hide();
        GNuklearWindow.style.button.active = nk_style_item_hide();
        GNuklearWindow.style.window.background = nk_rgba(0, 0, 0, 0);
        GNuklearWindow.style.button.border_color = nk_rgba(0, 0, 0, 0);
#if SHENMUE_IS_S1
        g_nuklearVertexShader = d3t_GetLoadedShader(g_nuklearShaderLoad, 0);
        g_nuklearPixelShader = d3t_GetLoadedShader(g_nuklearShaderLoad, 1);
#else
        g_NuklearInitialized = g_nuklearVertexShader != 0 && g_nuklearPixelShader != 0;
#endif
        if (!g_nuklearVertexBuffer) {
            const d3t::BufferDescription description{.flags = 1, .size = 1638400, .count = 0, .flags3 = 2, .buffer = nullptr, .name = "nuklear sVertexBuffer"};
            g_nuklearVertexBuffer = d3t::CreateBuffer(&description);
        }
        if (!g_nuklearIndexBuffer) {
            const d3t::BufferDescription description{.flags = 2, .size = 450560, .count = 0, .flags3 = 2, .buffer = nullptr, .name = "nuklear sIndexBuffer"};
            g_nuklearIndexBuffer = d3t::CreateBuffer(&description);
        }
#if SHENMUE_IS_S1
        g_NuklearInitialized = g_nuklearVertexShader != 0 && g_nuklearPixelShader != 0 && g_nuklearVertexBuffer != 0 && g_nuklearIndexBuffer != 0;
#else
        if (d3t::g_originalScreenWidth > g_uiScreenWidth)
            g_uiScreenWidth = d3t::g_originalScreenWidth;
        InitNuklearScreenTexture();
#endif
    }


    if (g_uiFontSize != 4) {
        g_uiFontSize = 4;
        UI_SelectFont();
    }
    if (g_uiFontFace != 0) {
        g_uiFontFace = 0;
        UI_SelectFont();
    }
    d3t::Texture2DDescription quarter{};
    d3t::GetTexture2DDescription(d3t::g_defaultQuarterTexture, &quarter);
    g_uiDefaultImage.handle.id = static_cast<int>(d3t::g_defaultQuarterTexture);
    g_uiDefaultImage.w = static_cast<unsigned short>(quarter.width);
    g_uiDefaultImage.h = static_cast<unsigned short>(quarter.height);
    g_uiDefaultImage.region[0] = 0;
    g_uiDefaultImage.region[1] = 0;
    g_uiDefaultImage.region[2] = g_uiDefaultImage.w;
    g_uiDefaultImage.region[3] = g_uiDefaultImage.h;
}



std::int32_t g_bIs43AspectRatio = 0;

std::array<d3t::UIViewSlot<kUIViewImageCapacity>, d3t::kUIViewSlotCount> g_uiViewSlots{};
std::array<std::uint32_t, d3t::kUIViewSlotCount * kUIViewImageCapacity> g_uiViewTextures{};
std::int32_t g_currentUIViewSlot = 0;
cJSON *g_parsedUIDocument = nullptr;
std::int32_t g_uiReadStateInitialized = 0;
std::array<d3t::UISpriteSheet, d3t::kUISpriteSheetCount> g_uiSpriteSheets{};
std::array<d3t::UIWindow, d3t::kUIWindowCount> g_uiWindows{};
std::array<d3t::UILayoutItem, d3t::kUILayoutItemCount> g_uiLayoutItems{};
std::array<UIStyleItem, 1024> g_uiStyleItems{};
std::array<UIDataBinding, 50> g_uiDataBindings{};
std::array<UIButtonWidget, 128> g_uiButtonWidgets{};
std::array<UIColourImageWidget, 256> g_uiColourImageWidgets{};
std::array<UIScissorWidget, 10> g_uiScissorWidgets{};
std::array<UIIconLabelWidget, 64> g_uiIconLabelWidgets{};
std::array<UIButtonIconWidget, 256> g_uiButtonIconWidgets{};
std::array<UITextBlock, 3> g_uiTextBlocks{};
std::array<UIButtonPanel, 32> g_uiButtonPanels{};
std::array<d3t::UIGaugeState, 5> g_fightGaugeWidgetStates{};
std::array<UIImageFrame, 3> g_uiImageFrames{};
std::array<UIButtonWheel, 3> g_uiButtonWheels{};
std::array<UIAreaMap, 1> g_uiAreaMaps{};
std::array<UISelector, 10> g_uiSelectors{};
std::array<UICollection, 8> g_uiCollections{};
std::array<UIOptionsList, 6> g_uiOptionsLists{};
std::array<UIStateStyle, 128> g_uiStateStyles{};
std::array<UIScrollList, 8> g_uiScrollLists{};
std::array<UIMenuBar, 10> g_uiMenuBars{};
std::array<UITexturePage, 5> g_uiTexturePages{};
std::array<d3t::UILabelWidget, d3t::kUILabelWidgetCount> g_uiLabelWidgets{};
std::array<d3t::UIImageWidget, d3t::kUIImageWidgetCount> g_uiImageWidgets{};

void ResetUIWindow(std::int32_t index);
void UI_ResetLabelWidget(std::int32_t index);




void UI_FreeViewWidget(std::int32_t slotIndex, std::int32_t widget) {
    d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(slotIndex)];
    const std::size_t entry = static_cast<std::size_t>(widget);
    const std::int32_t index = slot.widgetPoolIndices[entry];
    const std::size_t record = static_cast<std::size_t>(index);
    switch (slot.widgetKinds[entry]) {
    case 1:
        ResetUIWindow(index);
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        if (static_cast<std::uint32_t>(index) < g_uiStyleItems.size())
            g_uiStyleItems[record] = {.inUse = 0, .isColour = 1, .image = -1, .colour = {}};
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        if (index >= 0) {
            d3t::UILayoutItem &item = g_uiLayoutItems[record];
            item.inUse = 0;
            item.id[0] = 0;
            item.count = 0;
            item.window = 0;
            item.width = 0.0f;
            item.height = 0.0f;
        }
        break;
    case 15:
        UI_ResetLabelWidget(index);
        break;
    case 16:
        g_uiIconLabelWidgets[record].label.inUse = 0;
        g_uiIconLabelWidgets[record].label.id[0] = 0;
        [[fallthrough]];
    case 17:
        g_uiButtonWidgets[record].inUse = 0;
        g_uiButtonWidgets[record].id[0] = 0;
        g_uiButtonWidgets[record].colour = 0xffffffffU;
        break;
    case 18: {
        d3t::UIImageWidget &image = g_uiImageWidgets[record];
        image.inUse = 0;
        ResetUIWindow(image.window);
        image.window = -1;
        image.field44 = 1;
        image.image = -1;
        break;
    }
    case 19: {
        UIColourImageWidget &image = g_uiColourImageWidgets[record];
        image.inUse = 0;
        image.id[0] = 0;
        image.image = -1;
        image.colour = 0xffffffffU;
        image.window = -1;
        break;
    }
    case 22:
        g_uiScissorWidgets[record] = {};
        break;
    case 23:
        UI_ResetButtonWheel(index);
        break;
    case 24:
        UI_ResetMenuBar(index);
        break;
    case 25:
        UI_ResetCollection(index);
        break;
    case 26:
        UI_ResetOptionsList(index);
        break;
    case 27:
        UI_ResetScrollList(index);
        break;
    case 31:
        UI_ResetSelector(index);
        break;
    case 33:
        UI_ResetTextBlock(index);
        break;
    case 34:
        UI_ResetImageFrame(index);
        break;
    case 35:
        UI_ResetGaugeWidget(index);
        break;
    default:
        break;
    }
    slot.widgetKinds[entry] = 0;
    slot.widgetPoolIndices[entry] = -1;
}



std::int32_t UI_ReadWidgetObject(cJSON *objects, std::int32_t &kind, std::int32_t window) {
    for (cJSON *object = objects->child; object; object = object->next) {
        const char *const name = object->string;
        std::int32_t index = -1;
        if (!std::strcmp(name, "Label")) {
            index = UI_ReadLabelObject(object, window);
            kind = 15;
        } else if (!std::strcmp(name, "IconLabel")) {
            index = UI_ReadIconLabelObject(object, window);
            kind = 16;
        } else if (!std::strcmp(name, "Image")) {
            index = UI_LoadImageObject(object, window);
            kind = 18;
        } else if (!std::strcmp(name, "ColourImage")) {
            index = UI_ReadColourImageObject(object, window);
            kind = 19;
        } else if (!std::strcmp(name, "Sprite") || !std::strcmp(name, "ButtonIcon")) {
            index = UI_ReadButtonIconObject(object, window);
            kind = std::strcmp(name, "ButtonIcon") ? 20 : 21;
        } else if (!std::strcmp(name, "Scissor")) {
            index = UI_ReadScissorObject(object);
            kind = 22;
        } else if (!std::strcmp(name, "Button")) {
            index = UI_ReadButtonObject(object, window);
            kind = 17;
        } else {
            continue;
        }
        return index;
    }
    return -1;
}



void ReadUIView(cJSON *view) {
    for (cJSON *member = view->child; member; member = member->next) {
        if (!std::strcmp(member->string, "Panel"))
            UI_ReadPanel(member);
        else if (!std::strcmp(member->string, "ButtonWheel"))
            UI_ReadButtonWheel(member);
        else if (!std::strcmp(member->string, "MenuBar"))
            UI_ReadMenuBar(member);
        else if (!std::strcmp(member->string, "AreaMap"))
            UI_ReadAreaMap(member);
        else if (!std::strcmp(member->string, "Collection"))
            UI_ReadCollection(member);
        else if (!std::strcmp(member->string, "OptionsList"))
            UI_ReadOptionsList(member);
        else if (!std::strcmp(member->string, "ScrollList"))
            UI_ReadScrollList(member);
        else if (!std::strcmp(member->string, "Selector"))
            UI_ReadSelector(member);
        else if (!std::strcmp(member->string, "TexturePage"))
            d3t::AddViewWidget(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], 37, UI_ReadTexturePage(member));
        else if (!std::strcmp(member->string, "ImageFrame"))
            UI_ReadImageFrame(member);
        else if (!std::strcmp(member->string, "TextBlock"))
            UI_ReadTextBlock(member);
        else if (!std::strcmp(member->string, "Gauge"))
            UI_ReadGaugeWidget(member);
        else if (!std::strcmp(member->string, "ButtonPanel"))
            UI_ReadButtonPanel(member);
    }
}



void InitializeUIReadState() {
    for (std::int32_t slot = 0; slot < static_cast<std::int32_t>(d3t::kUIViewSlotCount); ++slot)
        ResetUIViewSlot(slot);
    for (std::int32_t window = 0; window < static_cast<std::int32_t>(d3t::kUIWindowCount); ++window)
        ResetUIWindow(window);
    for (UIStyleItem &style : g_uiStyleItems)
        style = {.inUse = 0, .isColour = 1, .image = -1, .colour = {}};
    for (std::int32_t label = 0; label < static_cast<std::int32_t>(d3t::kUILabelWidgetCount); ++label)
        UI_ResetLabelWidget(label);


    for (UIIconLabelWidget &widget : g_uiIconLabelWidgets) {
        widget.label.inUse = 0;
        widget.label.id[0] = 0;
    }
    for (UIButtonIconWidget &widget : g_uiButtonIconWidgets) {
        widget.inUse = 0;
        widget.id[0] = 0;
        widget.sheet = -1;
        widget.sprite = -1;
        widget.fieldAC = 0;
        widget.colour = 0xffffffffU;
        widget.window = -1;
    }
    for (std::int32_t block = 0; block < static_cast<std::int32_t>(g_uiTextBlocks.size()); ++block)
        UI_ResetTextBlock(block);
    for (std::int32_t wheel = 0; wheel < static_cast<std::int32_t>(g_uiButtonWheels.size()); ++wheel)
        UI_ResetButtonWheel(wheel);
    for (std::int32_t bar = 0; bar < static_cast<std::int32_t>(g_uiMenuBars.size()); ++bar)
        UI_ResetMenuBar(bar);
    for (std::int32_t collection = 0; collection < static_cast<std::int32_t>(g_uiCollections.size()); ++collection)
        UI_ResetCollection(collection);
    for (std::int32_t list = 0; list < static_cast<std::int32_t>(g_uiOptionsLists.size()); ++list)
        UI_ResetOptionsList(list);
    for (std::int32_t list = 0; list < static_cast<std::int32_t>(g_uiScrollLists.size()); ++list)
        UI_ResetScrollList(list);
    UI_ResetAreaMap(0);
    for (std::int32_t page = 0; page < static_cast<std::int32_t>(g_uiTexturePages.size()); ++page)
        UI_ResetTexturePage(page);
    for (std::int32_t frame = 0; frame < static_cast<std::int32_t>(g_uiImageFrames.size()); ++frame)
        UI_ResetImageFrame(frame);
    for (std::int32_t gauge = 0; gauge < static_cast<std::int32_t>(g_fightGaugeWidgetStates.size()); ++gauge)
        UI_ResetGaugeWidget(gauge);
    g_uiScissorWidgets = {};
    for (UIColourImageWidget &widget : g_uiColourImageWidgets) {
        widget.inUse = 0;
        widget.id[0] = 0;
        widget.image = -1;
        widget.colour = 0xffffffffU;
        widget.window = -1;
    }
    for (UIButtonWidget &widget : g_uiButtonWidgets) {
        widget.inUse = 0;
        widget.id[0] = 0;
        widget.colour = 0xffffffffU;
    }
    d3t::ResetLayoutItems(g_uiLayoutItems);
    d3t::ResetSpriteSheets(g_uiSpriteSheets);
    g_uiReadStateInitialized = 1;
}


std::int32_t g_mainMenuPulseSlot = -1;


std::array<std::int32_t, kInventoryIconSheetCount> g_inventoryIconSheetImageIndices{};

std::array<UIAnimSlot, 256> g_uiAnimSlots{};


std::int32_t g_uiNavigateSoundFrame = 0;

std::array<UIWidgetBinding, 256> g_uiWidgetBindings{};

#if !SHENMUE_IS_S2

std::array<float *, 256> g_uiWidgetBindingProperties{};
#endif


std::int32_t g_uiDrawViewSlot = -1;


std::int32_t g_currActiveUIIndex = -1;

std::uint64_t g_uiLegacyDrawFrame = 0;


std::int32_t g_uiLabelSplitting = 0;

std::array<d3t::UIViewImage *, 3> g_platformButtonIconImages{};

#if SHENMUE_IS_S1
std::int64_t g_platformIconsSheet = 0;
#else
std::int32_t g_platformIconsSheet = 0;
#endif

struct nk_image g_uiDefaultImage {};


std::array<UIMouseCallbackQueue, 4> g_uiMouseCallbackQueues{};


std::int32_t g_uiDebugDrawHitAreas = 0;


nk_panel g_uiCollectionSelectorPanel{};








void UI_InputHandler(nk_context &context, std::int32_t slot, std::int32_t first, std::int32_t end) {
    UI_ResetNuklearStyle(context);
    if (!g_NuklearInitialized || static_cast<std::uint32_t>(slot) > 0x4bU)
        return;
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(slot)];
    g_uiDrawViewSlot = slot;
    for (std::int32_t widget = first; widget < end; ++widget) {
        const std::int32_t index = view.widgetPoolIndices[static_cast<std::size_t>(widget)];
        switch (view.widgetKinds[static_cast<std::size_t>(widget)]) {
        case 1:
            UI_DrawWindowBegin(context, index);
            break;
        case 2:
            if (context.current && g_waitingForNextTick == 1) {
                if (!(context.current->flags & NK_WINDOW_HIDDEN))
                    nk_end(&context);
                context.current = nullptr;
            }
            break;
        case 5:
            if (static_cast<std::uint32_t>(index) <= 0x3ffU) {
                const UIStyleItem &record = g_uiStyleItems[static_cast<std::size_t>(index)];
                const std::uint32_t colour = ConstructColour(static_cast<std::int32_t>(record.colour[0]), static_cast<std::int32_t>(record.colour[1]),
                    static_cast<std::int32_t>(record.colour[2]), static_cast<std::int32_t>(record.colour[3]));
                std::memcpy(&context.style.window.background, &colour, sizeof(colour));
            } else {
                context.style.window.background = nk_rgba(255, 255, 255, 255);
            }
            break;
        case 6:
            context.style.window.fixed_background = UI_StyleItem(index);
            break;
        case 7:
            context.style.button.normal = UI_StyleItem(index);
            break;
        case 8:
            context.style.button.hover = UI_StyleItem(index);
            break;
        case 9:
            context.style.button.active = UI_StyleItem(index);
            break;
        case 10:
            if (static_cast<std::uint32_t>(index) <= 0x3ffU) {
                const d3t::UILayoutItem &item = g_uiLayoutItems[static_cast<std::size_t>(index)];
                d3t::UIWindow *const window = UI_GetWindow(item.window);
                if (window) {
                    UILayoutRect rect{};
                    UI_GetWindowRect(window, rect);
                    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
                        nk_layout_row_dynamic(&context, item.height * 1080.0f * rect.height, item.count);
                }
            }
            break;
        case 12:
            if (static_cast<std::uint32_t>(index) <= 0x3ffU && context.current && g_waitingForNextTick == 1 && context.current->layout) {
                const d3t::UILayoutItem &item = g_uiLayoutItems[static_cast<std::size_t>(index)];
                nk_layout_space_begin(&context, NK_STATIC, item.height * 1080.0f, item.count);
            }
            break;
        case 13:
            if (static_cast<std::uint32_t>(index) <= 0x3ffU) {
                const d3t::UILayoutItem &item = g_uiLayoutItems[static_cast<std::size_t>(index)];
                d3t::UIWindow *const window = UI_GetWindow(item.window);
                if (window && context.current) {
                    const UILayoutRect rect = UI_SpacePushRect(item, window);
                    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
                        context.current->layout->row.item = nk_rect(rect.x, rect.y, rect.width, rect.height);
                }
            }
            break;
        case 15:
            UI_DrawLabel(context, index, widget);
            break;
        case 16:
            UI_DrawIconLabel(context, index, widget);
            break;
        case 17:
            UI_DrawButton(context, index);
            break;
        case 18:
            UI_DrawImage(context, index);
            break;
        case 19:
            UI_DrawColourImage(context, index);
            break;
        case 20:
            UI_DrawSprite(context, index);
            break;
        case 21:
            if (static_cast<std::uint32_t>(index) <= 0xffU)
                UI_DrawButtonIcon(context, g_uiButtonIconWidgets[static_cast<std::size_t>(index)], nullptr);
            break;
        case 22:
            UI_DrawScissor(context, index);
            break;
        case 23:
            UI_DrawButtonWheel(context, index);
            break;
        case 24:
            UI_DrawMenuBar(context, index);
            break;
        case 25:
            UI_DrawCollection(context, index);
            break;
        case 26:
            UI_DrawOptionsList(context, index);
            break;
#if SHENMUE_IS_S2
        case 27:
            UI_DrawScrollList(context, static_cast<std::uint32_t>(index));
            break;
#endif
        case 30:
            UI_DrawAreaMap(context, index);
            break;
        case 31:
            UI_DrawSelector(context, index);
            break;
        case 33:
            UI_DrawTextBlock(context, index);
            break;
        case 34:
            UI_DrawImageFrame(context, index);
            break;
        case 35:
            UI_DrawGauge(context, index);
            break;
        case 36:
            UI_DrawButtonPanel(context, index);
            break;
        case 14:
            if (context.current && g_waitingForNextTick == 1 && context.current->layout) {
                nk_row_layout &row = context.current->layout->row;
                row.item_width = 0.0f;
                row.item_height = 0.0f;
                row.item_offset = 0.0f;
                row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
            }
            break;
        default:
            break;
        }
    }
    g_currActiveUIIndex = -1;
}

#if SHENMUE_IS_S2
nk_context *GetNativeUiContext() {
    return &GNuklearWindow;
}

d3t::UIViewSlot<kUIViewImageCapacity> *GetNuklearElementByIdx(std::uint32_t slot) {
    return slot < d3t::kUIViewSlotCount ? &g_uiViewSlots[slot] : nullptr;
}

std::uint8_t GetIsNuklearActive() {
    return g_NuklearInitialized;
}

void DrawUIEntity(nk_context &context, std::uint32_t slot, std::int32_t element) {
    g_currActiveUIIndex = element;
    const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
    if (view)
        UI_InputHandler(context, static_cast<std::int32_t>(slot), 0, view->widgetCount);
}

void uiUpdateWithColour(nk_context &context, std::uint32_t slot) {
    const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(slot);
    if (view)
        UI_InputHandler(context, static_cast<std::int32_t>(slot), 0, view->widgetCount);
}
#endif

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
