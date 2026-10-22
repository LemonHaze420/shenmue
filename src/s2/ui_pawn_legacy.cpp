#include "s2/ui_pawn_legacy.hpp"
#include "s2/ui_internal.hpp"
#include "s2/ui_scroll_list.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/pawn.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/story_state.hpp"
#include <bit>
#include <cstdio>
#include <cstring>

namespace shenmue::s2 {
std::int32_t pawnLegacyInitState{}, pawnLegacyMode{}, pawnLegacyShop{}, pawnLegacyResult{}, pawnLegacyItemCount{};
std::uint32_t pawnLegacyRequest{};
UIViewConfiguration pawnLegacyConfiguration{-1, -1, -1};
std::array<std::int32_t, 12> pawnLegacySingleWidgets{};
std::array<std::int32_t, 8> pawnLegacySetWidgets{};
d3t::UIViewSlot<kUIViewImageCapacity> *pawnLegacyView{};
d3t::UIWindow *pawnLegacyMainPanel{}, *pawnLegacyHeaderPanel{}, *pawnLegacyScrollPanel{}, *pawnLegacyMonoPanel{};
UICollection *pawnLegacyCollection{};
d3t::UILabelWidget *pawnLegacyTitle{}, *pawnLegacyHeader{}, *pawnLegacyPrize{}, *pawnLegacyName{}, *pawnLegacyCount{};
std::int32_t pawnLegacyCollectionIndex{};
std::array<std::uint16_t, 256> pawnLegacyItems{};
std::array<std::int16_t, 256> pawnLegacyQuantities{};
std::array<char, 12> pawnLegacyPrizeText{}, pawnLegacyCountText{};
std::array<char, 512> pawnLegacyMoneyText{}, pawnLegacyPriceText{};

void BindLegacyPawnFlyer() {
    const std::uint32_t viewIndex = static_cast<std::uint32_t>(pawnLegacyConfiguration.uiHandleIndex);
    for (std::int32_t index = 0; index < 12; ++index)
        pawnLegacySingleWidgets[index] = UI_FindViewWindow(viewIndex, PAWN_LEGACY_SINGLE_WIDGETS[index].data());
    for (std::int32_t index = 0; index < 8; ++index) {
        pawnLegacySetWidgets[index] = UI_FindViewWindow(viewIndex, PAWN_LEGACY_SET_WIDGETS[index].data());
        d3t::UIWindow *const window = UI_GetWindow(pawnLegacySetWidgets[index]);
        if (window)
            window->alpha = 0.0f;
    }
    pawnLegacyView = GetNuklearElementByIdx(viewIndex);
    pawnLegacyMainPanel = UI_FindViewWindowPointer(viewIndex, "Pawnshop_Main_Panel");
    pawnLegacyHeaderPanel = UI_FindViewWindowPointer(viewIndex, "Pawnshop_Header_Panel");
    pawnLegacyConfiguration.enabled = UI_FindViewScrollList(viewIndex, "Pawnshop_Scroll_List");
    g_pawnFlyerScrollList = UI_GetScrollList(static_cast<std::uint32_t>(pawnLegacyConfiguration.enabled));
    if (g_pawnFlyerScrollList)
        pawnLegacyScrollPanel = UI_GetWindow(g_pawnFlyerScrollList->window);
    pawnLegacyConfiguration.status = UI_FindViewTexturePage(viewIndex, "Pawnshop_Texture_Page");
    g_pawnFlyerTexturePage = UI_GetTexturePage(static_cast<std::uint32_t>(pawnLegacyConfiguration.status));
    pawnLegacyTitle = UI_GetLabel(UI_FindViewLabel(viewIndex, "Pawnshop_Title_Label"));
    pawnLegacyHeader = UI_GetLabel(UI_FindViewLabel(viewIndex, "Pawnshop_Header_Label"));
    pawnLegacyCollectionIndex = UI_FindViewCollection(viewIndex, "Pawnshop_Collection");
    pawnLegacyCollection = UI_GetCollection(static_cast<std::uint32_t>(pawnLegacyCollectionIndex));
    if (pawnLegacyCollection) {
        pawnLegacyCollection->pages[0].field5C = 1;
        pawnLegacyCollection->pages[0].override = 1;
        pawnLegacyCollection->pages[0].drawType = 1;
    }
    pawnLegacyMonoPanel = UI_FindViewWindowPointer(viewIndex, "Pawnshop_Mono_Panel");
    pawnLegacyPrize = UI_GetLabel(UI_FindViewLabel(viewIndex, "Pawnshop_Mono_Prize_Label"));
    pawnLegacyName = UI_GetLabel(UI_FindViewLabel(viewIndex, "Pawnshop_Mono_Name_Label"));
    pawnLegacyCount = UI_GetLabel(UI_FindViewLabel(viewIndex, "Pawnshop_Mono_Count_Label"));
    d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(viewIndex);
    for (std::int32_t index = 0; index < view->widgetCount; ++index) {
        if (view->widgetKinds[index] != 15)
            continue;
        d3t::UILabelWidget *const label = UI_GetLabel(view->widgetPoolIndices[index]);
        if (!std::strncmp(label->text, "MONEY", 6)) {
            label->text = pawnLegacyMoneyText.data();
            pawnLegacyMoneyText[0] = 0;
        }
        if (!std::strncmp(label->text, "PRICE", 6)) {
            label->text = pawnLegacyPriceText.data();
            pawnLegacyPriceText[0] = 0;
        }
    }
}

std::uint32_t AdvanceLegacyPawnFlyerInit() {
    if (pawnLegacyInitState == 0)
        pawnLegacyInitState = LoadUIFile("/ui/pawnshop/PawnshopFlyer_Legacy.ui", 40, &pawnLegacyRequest);
    else if (pawnLegacyInitState == 1) {
        pawnLegacyInitState = GetNewUIFileInitFor(&pawnLegacyRequest, pawnLegacyConfiguration);
        if (pawnLegacyInitState == 2)
            BindLegacyPawnFlyer();
    } else if (pawnLegacyInitState == 2)
        pawnLegacyInitState = GetShouldInitUIFileFor(static_cast<std::uint32_t>(pawnLegacyConfiguration.uiHandleIndex));
    return static_cast<std::uint32_t>(pawnLegacyInitState);
}

std::int64_t RenderLegacyPawnFlyer() {
    if (pawnLegacyInitState != 3) {
        AdvanceLegacyPawnFlyerInit();
        if (static_cast<std::uint32_t>(pawnLegacyInitState) - 2U > 1U)
            return 0;
    }
    RefreshNotebook();
    uiUpdateWithColour(*GetNativeUiContext(), static_cast<std::uint32_t>(pawnLegacyConfiguration.uiHandleIndex));
    RefreshLegacyPawnValues();
    return 0;
}

std::int64_t LegacyPawnFlyerInput() {
    if (!g_pawnFlyerScrollList || !pawnLegacyCollection)
        return 0;
    if (pawnLegacyMode == PawnFlyerSets) {
        UI_UpdateScrollListSelection(static_cast<std::uint32_t>(pawnLegacyConfiguration.enabled));
        SetPawnSetCursor(g_pawnFlyerScrollList->field74[0] + g_pawnFlyerScrollList->field74[1]);
        RefreshLegacyPawnSet(g_pawnFlyerScrollList->field74[0] + g_pawnFlyerScrollList->field74[1]);
    } else if (pawnLegacyMode == PawnFlyerItems) {
        UI_UpdateCollectionInput(static_cast<std::uint32_t>(pawnLegacyCollectionIndex));
        RefreshLegacyPawnItems();
    } else if (pawnLegacyMode == PawnFlyerInspect) {
        const std::int32_t total = g_pawnFlyerScrollList->field6C;
        g_pawnFlyerScrollList->field6C -= g_pawnFlyerScrollList->rows;
        UI_UpdateScrollListPage(static_cast<std::uint32_t>(pawnLegacyConfiguration.enabled));
        g_pawnFlyerScrollList->field6C = total;
        if (g_inputBuffer[0].pressedPriority & 0x0200U) {
            SetUIRenderSuppression(0);
            pawnLegacyResult = 0;
            g_inputBuffer[0].pressedPriority &= ~0x0200U;
        }
    }
    return 0;
}

void ConfigureLegacyPawnFlyer(std::int32_t shop, std::int32_t mode) {
    pawnLegacyShop = shop;
    pawnLegacyMode = mode;
    if (g_pawnFlyerScrollList) {
        if (mode == PawnFlyerItems) {
            pawnLegacyCollection->field44[0] = 0;
            g_pawnFlyerScrollList->field44 = 1;
            for (std::int32_t index : pawnLegacySingleWidgets) {
                d3t::UIWindow *const window = UI_GetWindow(index);
                if (window)
                    window->alpha = 1.0f;
            }
            for (std::int32_t index : pawnLegacySetWidgets) {
                d3t::UIWindow *const window = UI_GetWindow(index);
                if (window)
                    window->alpha = 0.0f;
            }
        } else if (mode == PawnFlyerSets || mode == PawnFlyerInspect) {
            g_pawnFlyerScrollList->draws[0].field108 = mode == PawnFlyerSets ? 1 : 0;
            pawnLegacyCollection->field44[0] = 1;
            g_pawnFlyerScrollList->field44 = 0;
            if (mode == PawnFlyerInspect) {
                pawnLegacyShop = shop;
                SelectPawnFlyerShop(shop);
            }
            for (std::int32_t index : pawnLegacySingleWidgets) {
                d3t::UIWindow *const window = UI_GetWindow(index);
                if (window)
                    window->alpha = 0.0f;
            }
            for (std::int32_t index : pawnLegacySetWidgets) {
                d3t::UIWindow *const window = UI_GetWindow(index);
                if (window)
                    window->alpha = 1.0f;
            }
        }
        g_pawnFlyerScrollList->field74 = {0, 0};
    }
    if (pawnLegacyTitle)
        pawnLegacyTitle->text = GetPawnShopLabel(GetPawnFlyerShopName(pawnLegacyShop));
    if (pawnLegacyHeader)
        pawnLegacyHeader->text = GetStringForUILabel(PAWN_LEGACY_HEADERS[pawnLegacyMode]);
    RefreshLegacyPawnValues();
}

void SetLegacyPawnFlyerResult(std::int32_t result) {
    pawnLegacyResult = result;
}
std::int64_t GetLegacyPawnFlyerResult() {
    return static_cast<std::uint32_t>(pawnLegacyResult);
}

void PublishLegacyPawnCollection() {
    if (!pawnLegacyCollection)
        return;
    const std::int32_t columns = pawnLegacyCollection->pages[0].columns, rows = pawnLegacyCollection->pages[0].rows;
    const std::int32_t perPage = rows * columns;
    std::int32_t slot = 0;
    for (std::int32_t column = 0; column < columns; ++column) {
        for (std::int32_t row = 0; row < rows; ++row, ++slot) {
            const std::int32_t index = row + perPage * pawnLegacyCollection->field64[3] + column * rows;
            if (index >= pawnLegacyItemCount)
                return;
            UICollectionElement &element = pawnLegacyCollection->elements.data()[slot];
            element.count = pawnLegacyQuantities[index];
            element.colour = ConstructColour(255, 255, 255, pawnLegacyQuantities[index] == -256 ? 64 : 255);
            const std::int32_t icon = static_cast<std::int16_t>(GetInventoryItemIconById(pawnLegacyItems[index]));
            const std::array<std::uint8_t, 4> rect = GetCommonIconRect(static_cast<std::uint16_t>(icon));
            element.u0 = static_cast<float>(static_cast<std::int8_t>(rect[0])) * 0.125f;
            element.u1 = static_cast<float>(static_cast<std::int8_t>(rect[2])) * 0.125f;
            element.v0 = static_cast<float>(static_cast<std::int8_t>(rect[1])) * 0.125f;
            element.v1 = static_cast<float>(static_cast<std::int8_t>(rect[3])) * 0.125f;
            if (g_pawnFlyerTexturePage) {
                for (std::int32_t page = 0; page < g_pawnFlyerTexturePage->count; ++page) {
                    if (icon >= g_pawnFlyerTexturePage->min[page] && icon < g_pawnFlyerTexturePage->max[page]) {
                        element.image = g_pawnFlyerTexturePage->images[page];
                        break;
                    }
                }
            }
        }
    }
}

void RefreshLegacyPawnItems() {
    if (!pawnLegacyCollection)
        return;
    pawnLegacyItemCount = static_cast<std::int32_t>(CopyPawnSaleItemsAndQuantities(pawnLegacyItems.data(), pawnLegacyQuantities.data()));
    const std::int32_t selected = pawnLegacyCollection->field64[4];
    if (pawnLegacyPrize) {
        std::snprintf(pawnLegacyPrizeText.data(), pawnLegacyPrizeText.size(), "$%d", 0);
        pawnLegacyPrize->text = pawnLegacyPrizeText.data();
    }
    if (pawnLegacyName)
        pawnLegacyName->text = GetStringForUILabel(shenGetAmtOf(pawnLegacyItems[selected]));
    if (pawnLegacyCount) {
        const std::int32_t count =
            pawnLegacyQuantities[selected] == -256 ? 0 : static_cast<std::int32_t>(GetItemState(pawnLegacyItems[selected])) - pawnLegacyQuantities[selected];
        std::snprintf(pawnLegacyCountText.data(), pawnLegacyCountText.size(), "(%d)", count);
        pawnLegacyCount->text = pawnLegacyCountText.data();
    }
}

void RefreshLegacyPawnSet(std::int32_t index) {
    if (!pawnLegacyCollection)
        return;
    std::int32_t count = 0, price = 0;
    const PawnSetRecord *const set = GetPawnOrderedSetDetails(index, &count, &price);
    if (pawnLegacyName)
        pawnLegacyName->text = set->label;
    std::snprintf(pawnLegacyPrizeText.data(), pawnLegacyPrizeText.size(), "$%d", price);
    pawnLegacyPrize->text = pawnLegacyPrizeText.data();
    std::snprintf(pawnLegacyCountText.data(), pawnLegacyCountText.size(), "(%d)", count);
    pawnLegacyCount->text = pawnLegacyCountText.data();
}

void RefreshLegacyPawnValues() {
    std::snprintf(pawnLegacyMoneyText.data(), pawnLegacyMoneyText.size(), "%i", GetCurrentCash());
    if (pawnLegacyMode >= PawnFlyerInspect && pawnLegacyMode <= PawnFlyerSets) {
        pawnLegacyPriceText[0] = 0;
        RebuildPawnFlyerScrollRows(pawnLegacyMode == PawnFlyerInspect);
    } else if (pawnLegacyMode == PawnFlyerItems) {
        if (GetPawnSaleRunningPrice() <= 0)
            pawnLegacyPriceText[0] = 0;
        else
            std::snprintf(pawnLegacyPriceText.data(), pawnLegacyPriceText.size(), "+%i", GetPawnSaleRunningPrice());
        PublishLegacyPawnCollection();
    }
}
void InitializeLegacyPawnFlyerNullsub() {
}
std::int64_t LegacyPawnFlyerReady() {
    return 1;
}
} // namespace shenmue::s2
