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
#include "s2/areas/game/arcade.hpp"
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

std::uint32_t LoadPawnSetFlyer() {
    return ReadUIDefinitionFile(UIElement::PawnshopFlyer2, "/ui/pawnshop/PawnshopFlyer_Sets.ui", 0);
}
std::uint32_t LoadPawnItemFlyer() {
    return ReadUIDefinitionFile(UIElement::PawnshopFlyer1, "/ui/pawnshop/PawnshopFlyer_Single.ui", 0);
}
bool PawnSetIsAvailable(std::int32_t index) {
    const PawnSetRecord *const set = GetPawnSaleSet(index);
    const std::uint16_t *const items = reinterpret_cast<const std::uint16_t *>(set + 1);
    for (std::int32_t item = 0; item < set->itemCount; ++item)
        if (static_cast<std::int16_t>(items[item]) > 0 && GetItemState(items[item]) == 0)
            return false;
    return true;
}
const std::int32_t *PawnSetSelectionData(UIGrid *grid, std::int32_t row) {
    return grid->dictionary ? grid->dictionary + row : nullptr;
}
const std::int32_t *PawnItemSelectionData(UIGrid *grid, std::int32_t row) {
    return grid->dictionary ? reinterpret_cast<const std::int32_t *>(reinterpret_cast<const std::uint16_t *>(grid->dictionary) + row) : nullptr;
}
std::int32_t PawnSetRowSpan(UIGrid *grid, std::int32_t row) {
    return grid->dictionary && row >= 0 ? GetPawnSaleSet(grid->dictionary[row])->itemCount / 16 + 2 : 1;
}
void SelectPawnSet(float *, void *data) {
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    if (!row)
        return;
    for (std::int32_t index = 0; index < GetPawnSaleSetCount(); ++index)
        if (GetPawnSaleSetOrder()[index] == *row) {
            pawnSetFlyer.grid.row = index;
            pawnSetFlyer.grid.column = 0;
            break;
        }
}
void ActivatePawnSet(float *, void *data) {
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    if (!row || pawnSetFlyerInspection)
        return;
    if (PawnSetIsAvailable(*row))
        CompletePawnSaleSelection(static_cast<std::int16_t>(*row));
    else
        AicaExecuteWrapper(8);
}
void PublishPawnItemMessage(const Vector4 *bounds, std::int32_t index) {
    UIMessageBox message = GetCurrentUIMessageBox();
    message.imageIndex = -1;
    if (bounds) {
        message.bounds = *bounds;
        message.hideTarget = 0;
    }
    const std::uint16_t item = GetPawnSaleItems()[index];
    const std::int16_t quantity = GetPawnSaleQuantities()[index];
    message.primaryText = shenGetAmtOf(item);
    message.primaryLocalized = 0;
    message.secondaryText = nullptr;
    message.tertiaryText = nullptr;
    if (quantity != -256) {
        char price[32]{}, total[32]{};
        FormatMoneyAmount(price, GetPawnSaleItemPrice(index), true);
        const std::int32_t amount = static_cast<std::int32_t>(GetPawnSaleSelectedTotal());
        FormatMoneyAmount(total, amount, true);
        if (amount <= 0)
            std::snprintf(pawnItemFlyer.price, sizeof(pawnItemFlyer.price), "%s", price);
        else
            std::snprintf(pawnItemFlyer.price, sizeof(pawnItemFlyer.price), "%s (%s)", price, total);
        std::snprintf(pawnItemFlyer.quantity, sizeof(pawnItemFlyer.quantity), "(%u)", static_cast<unsigned>(GetItemState(item) - quantity));
        message.secondaryText = pawnItemFlyer.price;
        message.secondaryLocalized = 1;
        message.tertiaryText = pawnItemFlyer.quantity;
        message.tertiaryLocalized = 1;
    }
    CopyUIMessageBox(&message);
}
void SelectPawnItem(float *rectangle, void *data) {
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    Vector4 bounds{};
    if (rectangle)
        std::memcpy(&bounds, rectangle, sizeof(bounds));
    if (!row)
        return;
    const std::uint16_t item = *reinterpret_cast<const std::uint16_t *>(row);
    for (std::int32_t index = 0; index < GetPawnSaleItemCount(); ++index)
        if (GetPawnSaleItems()[index] == item) {
            PublishPawnItemMessage(rectangle ? &bounds : nullptr, index);
            return;
        }
}
void ActivatePawnItem(float *rectangle, void *data) {
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    Vector4 bounds{};
    if (rectangle)
        std::memcpy(&bounds, rectangle, sizeof(bounds));
    if (!row)
        return;
    const std::uint16_t item = *reinterpret_cast<const std::uint16_t *>(row);
    for (std::int32_t index = 0; index < GetPawnSaleItemCount(); ++index)
        if (GetPawnSaleItems()[index] == item) {
            AicaExecuteWrapper(IncrementPawnSaleQuantity(index) ? 10 : 8);
            PublishPawnItemMessage(rectangle ? &bounds : nullptr, index);
            return;
        }
}
void DrawPawnFlyerIcon(d3t::UIDefinition &definition, std::uint16_t item, const d3t::UIRect &rect, const d3t::UIRect &clip, float opacity) {
    d3t::UISharedSpriteImage image{};
    if (!ResolveSharedItemImage(GetInventoryItemIconById(item), image))
        return;
    d3t::UITextureObjectDefinition object;
    object.kind = d3t::UITextureObjectKind::Sprite;
    object.useShared = true;
    d3t::DrawUITextureObject(definition, object, {rect, clip}, opacity, &image);
}
void DrawPawnFlyerLabel(std::string_view text, const d3t::UIRect &rect, const d3t::UIRect &clip, std::uint32_t colour, bool right, bool quantity = false) {
    const UIFontSelection font{.language = uiFontLanguage, .family = 2, .image = quantity ? 3U : 1U};
    const float sy = static_cast<float>(d3t::g_originalScreenHeight) / 1080.0f;
    const float sx = static_cast<float>(d3t::g_originalScreenWidth) / 1920.0f;
    const float height = GetUIFontHeight(font);
    const float width = GetUIFontTextWidth(font, height, text);
    const float x = quantity ? rect.x + (rect.width - width) * .5f : right ? rect.x + rect.width - width : rect.x;
    const float y = quantity ? rect.y + (rect.height - height) * .5f : rect.y;
    const std::array<std::int32_t, 4> scissor{static_cast<std::int32_t>(clip.x * sx), static_cast<std::int32_t>(clip.y * sy),
        static_cast<std::int32_t>((clip.x + clip.width) * sx), static_cast<std::int32_t>((clip.y + clip.height) * sy)};
    DrawUIFontText(font, height * sy, text, (x + 2) * sx, (y + 2) * sy, (colour & 0xff000000U) ? 0xc8000000U : 0, scissor);
    DrawUIFontText(font, height * sy, text, x * sx, y * sy, colour, scissor);
}
void DrawPawnFlyerDivider(PawnFlyerRuntime &runtime, d3t::UIDefinition &definition, const d3t::UIRect &tile, const d3t::UIRect &clip, float opacity) {
    if (runtime.divider < 0)
        return;
    const d3t::UITextureObjectDefinition &divider = definition.textureObjects[runtime.divider];
    const float height = runtime.layout.objects[runtime.divider].bounds.height;
    d3t::DrawUITextureObject(definition, divider, {{tile.x, tile.y + tile.height - height, tile.width, height}, clip}, opacity);
}
void DrawPawnSetRow(
    UIGrid *, void *, const Vector4 *outer, const Vector4 *bounds, std::int32_t, std::uint32_t, const std::int32_t *dictionary, std::int32_t ordinal) {
    if (!dictionary || !drawingUI)
        return;
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::PawnshopFlyer2)];
    const float sx = 1920.0f / GetUIViewportWidth();
    const d3t::UIRect tile{(outer->x + bounds->x) * sx, outer->y + bounds->y, bounds->z * sx, bounds->w};
    const d3t::UIRect clip{outer->x * sx, outer->y, outer->z * sx, outer->w};
    const std::int32_t setIndex = dictionary[ordinal];
    const bool available = PawnSetIsAvailable(setIndex);
    const std::uint32_t colour = available ? 0xff9be0dcU : 0xff372d2dU;
    if (ordinal == GetUIGridSelection(pawnSetFlyer.grid))
        d3t::DrawUIRoundedRectangle(tile, 0, 0x7f313273U, clip);
    const std::array<float, 2> padding = GetUILayoutPaddingScale();
    const float gapX = 16 * padding[0], gapY = 16 * padding[1];
    const d3t::UIRect content{tile.x + gapX, tile.y + gapY, tile.width - 2 * gapX, tile.height - 2 * gapY};
    DrawPawnFlyerLabel(GetPawnSaleSetLabel(setIndex), {content.x, content.y, content.width * .7f, 64}, clip, colour, false);
    char price[32]{};
    FormatMoneyAmount(price, GetPawnSaleSetPrice(setIndex), true);
    DrawPawnFlyerLabel(price, {content.x + content.width * .7f, content.y, content.width * .3f, 64}, clip, colour, true);
    const PawnSetRecord *const set = GetPawnSaleSet(setIndex);
    const std::uint16_t *const items = reinterpret_cast<const std::uint16_t *>(set + 1);
    const float width = (content.width - 15 * gapX) / 16;
    const float height = width / sx;
    for (std::int32_t index = 0; index < set->itemCount; ++index) {
        if (items[index] == 0)
            continue;
        const d3t::UIRect icon{content.x + (index % 16) * (width + gapX), content.y + 64 + (index / 16) * (height + gapY), width, height};
        DrawPawnFlyerIcon(definition, items[index], icon, clip, 1);
        if (GetItemState(items[index]) > 0 && pawnSetFlyer.collected[items[index] % 5] >= 0) {
            d3t::UITextureObjectDefinition &badge = definition.textureObjects[pawnSetFlyer.collected[items[index] % 5]];
            pawnSetFlyer.imageControl = 1;
            pawnSetFlyer.imageAngle = items[index] * 0.0099999998f;
            badge.rotation = pawnSetFlyer.imageAngle;
            d3t::DrawUITextureObject(definition, badge, {icon, clip}, 1);
            pawnSetFlyer.imageControl = 0;
            badge.rotation = 0;
        }
    }
    DrawPawnFlyerDivider(pawnSetFlyer, definition, tile, clip, 1);
}
void DrawPawnItemRow(
    UIGrid *, void *, const Vector4 *outer, const Vector4 *bounds, std::int32_t, std::uint32_t, const std::int32_t *dictionary, std::int32_t ordinal) {
    if (!dictionary || !drawingUI)
        return;
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(UIElement::PawnshopFlyer1)];
    const float sx = 1920.0f / GetUIViewportWidth();
    const d3t::UIRect tile{(outer->x + bounds->x) * sx, outer->y + bounds->y, bounds->z * sx, bounds->w};
    const d3t::UIRect clip{outer->x * sx, outer->y, outer->z * sx, outer->w};
    const std::int16_t quantity = GetPawnSaleQuantities()[ordinal];
    if (quantity > 0)
        d3t::DrawUIRoundedRectangle(tile, 0, 0x7f313273U, clip);
    d3t::UIRect icon{tile.x + tile.width * .7f * .25f, tile.y + tile.height * .7f * .25f, tile.width * .7f, tile.height * .7f};
    const std::uint16_t item = reinterpret_cast<const std::uint16_t *>(dictionary)[ordinal];
    d3t::UISharedSpriteImage image{};
    if (ResolveSharedItemImage(GetInventoryItemIconById(item), image) && image.source.height / image.source.width > icon.height / icon.width)
        icon.width *= icon.height / image.source.height;
    DrawPawnFlyerIcon(definition, item, icon, clip, quantity == -256 ? 100.0f / 255.0f : 1);
    if (quantity > 0) {
        char value[32]{};
        std::snprintf(value, sizeof(value), "%u", static_cast<unsigned>(quantity));
        DrawPawnFlyerLabel(value, {icon.x + icon.width - 16, icon.y + icon.height - 24, 16, 16}, clip, 0xff9be0dcU, false, true);
    }
    DrawPawnFlyerDivider(pawnItemFlyer, definition, tile, clip, quantity == -256 ? 100.0f / 255.0f : 1);
}
std::int64_t BindPawnFlyer(PawnFlyerRuntime &runtime, UIElement element, bool sets) {
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(element)];
    BindAuthoredMenuLayout(definition, runtime.layout);
    const std::string_view panel = sets ? "PawnshopSets::Panel_GridHolder" : "PawnshopSingle::Panel_GridHolder";
    const std::string_view label = sets ? "PawnshopSets::Label_PawnshopName" : "PawnshopSingle::Label_PawnshopName";
    const std::int32_t holder = MenuPanelIndex(definition, panel);
    runtime.nameLabel = -1;
    runtime.divider = -1;
    for (std::size_t index = 0; index < definition.labels.size(); ++index)
        if (definition.labels[index].id == label)
            runtime.nameLabel = static_cast<std::int32_t>(index);
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index) {
        const std::string &id = definition.textureObjects[index].id;
        if (id == (sets ? "PawnshopSets::Divider" : "PawnshopSingle::Divider"))
            runtime.divider = static_cast<std::int32_t>(index);
        if (sets)
            for (std::size_t badge = 0; badge < 5; ++badge) {
                char name[32];
                std::snprintf(name, sizeof(name), "PawnshopSets::Collected%u", static_cast<unsigned>(badge));
                if (id == name)
                    runtime.collected[badge] = static_cast<std::int32_t>(index);
            }
    }
    runtime.style = UI_GRID_FRONTEND_STYLE;
    runtime.style.colours[0] = 0x642f2f2fU;
    runtime.style.colours[3] = 0xff3e8986U;
    const UIGrid configuration{.columns = sets ? 1 : 8,
        .visibleRows = sets ? 8 : 4,
        .rows = sets ? 8 : 4,
        .drawEmpty = static_cast<std::uint8_t>(!sets),
        .style = &runtime.style,
        .panel = holder < 0 ? nullptr : &definition.panels[holder],
        .drawRow = sets ? DrawPawnSetRow : DrawPawnItemRow,
        .actions = {sets ? SelectPawnSet : SelectPawnItem, nullptr, nullptr, sets ? ActivatePawnSet : ActivatePawnItem},
        .selectionData = sets ? PawnSetSelectionData : PawnItemSelectionData,
        .rowSpan = sets ? PawnSetRowSpan : nullptr,
        .sound = 3};
    InitializeUIGrid(runtime.grid, configuration);
    return 1;
}
std::int64_t BindPawnSetFlyer() {
    return BindPawnFlyer(pawnSetFlyer, UIElement::PawnshopFlyer2, true);
}
std::int64_t BindPawnItemFlyer() {
    return BindPawnFlyer(pawnItemFlyer, UIElement::PawnshopFlyer1, false);
}
std::int64_t ResetPawnSetFlyer() {
    pawnSetFlyer.grid.panel = nullptr;
    pawnSetFlyer.nameLabel = -1;
    pawnSetFlyer.placeholder = -1;
    return 0;
}
std::int64_t ResetPawnItemFlyer() {
    pawnItemFlyer.grid.panel = nullptr;
    pawnItemFlyer.nameLabel = -1;
    pawnItemFlyer.placeholder = -1;
    return 0;
}
std::int64_t TickPawnSetFlyer() {
    EnableRenderableByIndex(UIElement::GameMenu);
    ShowElementByIndex(UIElement::GameHud);
    ResetUIGrid(pawnSetFlyer.grid);
    SetUIGridTabSelector(nullptr, 1);
    return reinterpret_cast<std::int64_t>(currentUIGrid);
}
std::int64_t TickPawnItemFlyer() {
    ShowElementByIndex(UIElement::GameInfoBar);
    ShowElementByIndex(UIElement::GameHud);
    ResetUIGrid(pawnItemFlyer.grid);
    SetUIGridTabSelector(nullptr, 1);
    return reinterpret_cast<std::int64_t>(currentUIGrid);
}
std::int64_t DestroyPawnSetFlyer() {
    DestroyElementByIndex(UIElement::GameHud);
    g_uiElements[static_cast<std::size_t>(UIElement::GameMenu)].isRenderable = 0;
    AicaExecuteWrapper(2);
    return 1;
}
std::int64_t DestroyPawnItemFlyer() {
    DestroyElementByIndex(UIElement::GameInfoBar);
    DestroyElementByIndex(UIElement::GameHud);
    AicaExecuteWrapper(2);
    return 1;
}
std::int64_t PawnSetFlyerInput() {
    if (g_inputBuffer[0].pressedPriority & 0x200U) {
        if (pawnSetFlyerInspection == 1)
            DestroyElementByIndex(UIElement::PawnshopFlyer2);
        else
            CompletePawnSaleSelection(-1);
        return 0;
    }
    return ProcessUIGridInput(pawnSetFlyer.grid, uiPointer, GetFrameTime());
}
std::int64_t PawnItemFlyerInput() {
    const std::uint32_t pressed = g_inputBuffer[0].pressedPriority;
    if (pressed & 0x200U)
        CompletePawnSaleSelection(-1);
    if (pressed & 4U) {
        if (static_cast<std::int32_t>(GetPawnSaleSelectedTotal()) > 0)
            CompletePawnSaleSelection(-2);
        else
            AicaExecuteWrapper(8);
    }
    if (pressed & 2U) {
        const std::int32_t index = GetUIGridSelection(pawnItemFlyer.grid);
        AicaExecuteWrapper(DecrementPawnSaleQuantity(index) ? 10 : 8);
        PublishPawnItemMessage(nullptr, index);
    }
    return ProcessUIGridInput(pawnItemFlyer.grid, uiPointer, GetFrameTime());
}
std::int64_t RenderPawnFlyer(PawnFlyerRuntime &runtime, UIElement element) {
    d3t::UIDefinition &definition = uiDefinitions[static_cast<std::size_t>(element)];
    const bool sets = element == UIElement::PawnshopFlyer2;
    if (runtime.nameLabel >= 0) {
        definition.labels[runtime.nameLabel].text = sets ? pawnSetFlyerShopLabel : pawnItemFlyerShopLabel;
    }
    d3t::ResolveUIDefinitionLayout(definition, runtime.layout.parents, runtime.layout.panels, runtime.layout.objects, runtime.layout.labels,
        runtime.layout.objectPanels, runtime.layout.labelPanels, {}, GetUILayoutPaddingScale());
    if (drawingUI)
        d3t::DrawUIDefinition(definition, 1);
    RenderUIGrid(runtime.grid, static_cast<std::uint32_t>(element), uiPointer);
    if (drawingUI)
        DrawUIGridScrollbar(runtime.grid, static_cast<std::uint32_t>(element), uiPointer);
    return 0;
}
std::int64_t RenderPawnSetFlyer() {
    return RenderPawnFlyer(pawnSetFlyer, UIElement::PawnshopFlyer2);
}
std::int64_t RenderPawnItemFlyer() {
    return RenderPawnFlyer(pawnItemFlyer, UIElement::PawnshopFlyer1);
}
std::int64_t PawnSetFlyerHints() {
    static std::array<std::uint32_t, 5> hints{};
    hints[0] = (hints[0] & 0xffff0000U) | 0xffffU;
    hints[1] = (hints[1] & 0xffff0000U) | 44U;
    hints[2] = (hints[2] & 0xffff0000U) | 0xffffU;
    hints[3] = (hints[3] & 0xffff0000U) | 0xffffU;
    if (pawnSetFlyerInspection == 0) {
        const bool available = PawnSetIsAvailable(GetUIGridSelection(pawnSetFlyer.grid));
        GetPawnSaleSelectedTotal();
        hints[0] = 56U | (available ? 0 : 0x20000U);
    }
    return reinterpret_cast<std::int64_t>(hints.data());
}
std::int64_t PawnItemFlyerHints() {
    const std::int32_t index = GetUIGridSelection(pawnItemFlyer.grid);
    static std::array<std::uint32_t, 5> hints{};
    hints[0] = (hints[0] & 0xffff0000U) | 3850U;
    hints[1] = (hints[1] & 0xffff0000U) | 44U;
    hints[2] = 0x10000U | (static_cast<std::int32_t>(GetPawnSaleSelectedTotal()) > 0 ? 56U : 0xffffU);
    hints[3] = (hints[3] & 0xffff0000U) | (GetPawnSaleQuantities()[index] > 0 ? 3851U : 0xffffU);
    return reinterpret_cast<std::int64_t>(hints.data());
}
} // namespace ui_detail

void PreparePawnSetFlyer(std::int32_t index, std::int32_t inspection) {
    if (index >= 12)
        return;
    pawnSetFlyer.grid.rows = 8;
    pawnSetFlyerIndex = index;
    pawnSetFlyerInspection = inspection;
    char token[12];
    std::snprintf(token, sizeof(token), "UIPWNSHP%02u", static_cast<std::uint32_t>(index));
    std::snprintf(pawnSetFlyerShopLabel, sizeof(pawnSetFlyerShopLabel), "%s", GetLocalizedSceneText(token).data());
    SelectPawnFlyerShop(index);
    RefreshPawnSaleSets(GetPawnFlyerShopName(index));
    SetUIGridDictionary(pawnSetFlyer.grid, GetPawnSaleSetCount(), GetPawnSaleSetOrder());
}
void PreparePawnItemFlyer(std::int32_t index) {
    if (index >= 12)
        return;
    pawnItemFlyerIndex = index;
    char token[12];
    std::snprintf(token, sizeof(token), "UIPWNSHP%02u", static_cast<std::uint32_t>(index));
    std::snprintf(pawnItemFlyerShopLabel, sizeof(pawnItemFlyerShopLabel), "%s", GetLocalizedSceneText(token).data());
    SelectPawnFlyerShop(index);
    RefreshPawnSaleItems(GetPawnFlyerShopName(index));
    SetUIGridDictionary(pawnItemFlyer.grid, GetPawnSaleItemCount(), reinterpret_cast<const std::int32_t *>(GetPawnSaleItems()));
}
} // namespace shenmue::s2
