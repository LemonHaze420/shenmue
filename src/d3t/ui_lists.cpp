#include "d3t/ui_internal.hpp"

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif



void UI_ResetCollectionPage(std::int32_t collection, std::int32_t pageIndex) {
    UICollectionPage &page = g_uiCollections[static_cast<std::size_t>(collection)].pages[static_cast<std::size_t>(pageIndex)];
    page.inUse = 0;
    page.columns = 0;
    page.rows = 0;
    page.elementWidth = 0.0f;
    page.elementHeight = 0.0f;
    page.field5C = 0;
    page.field60 = {0, 0};
    page.field68 = -1;
    if (static_cast<std::uint32_t>(page.texture) <= 4U)
        UI_ResetTexturePage(page.texture);
    page.texture = -1;
}




void UI_ResetCollection(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiCollections.size()))
        return;
    UICollection &collection = g_uiCollections[static_cast<std::size_t>(index)];
    if (g_uiReadStateInitialized == 1 && collection.inUse == 1) {
        if (collection.dataBinding >= 0 && collection.dataBinding < static_cast<std::int32_t>(g_uiDataBindings.size())) {
            UIDataBinding &binding = g_uiDataBindings[static_cast<std::size_t>(collection.dataBinding)];
            binding.inUse = 0;
            binding.method = 0;
            binding.variable = -1;
        }
        for (const std::int32_t style : {collection.selectedImage, collection.field7C}) {
            if (static_cast<std::uint32_t>(style) < g_uiStyleItems.size())
                g_uiStyleItems[static_cast<std::size_t>(style)] = {.inUse = 0, .isColour = 1, .image = -1, .colour = {}};
        }
    }
    collection.inUse = 0;
    collection.id[0] = 0;
    collection.dataBinding = -1;
    collection.selectedImage = -1;
    collection.field7C = -1;
    collection.field64.fill(0);
    collection.pageCount = 0;
    collection.field94 = 0;
    for (std::int32_t page = 0; page < static_cast<std::int32_t>(collection.pages.size()); ++page)
        UI_ResetCollectionPage(index, page);
    for (UICollectionElement &element : collection.elements)
        element.colour = ConstructColour(255, 255, 255, 255);
    collection.hasBar = 0;
    ResetUIWindow(collection.bar.window);
    collection.bar.horizontal = 0;
    collection.bar.window = -1;
    collection.bar.outlineImage = -1;
    collection.bar.barImage = -1;
    collection.bar.topImage = -1;
    collection.bar.bottomImage = -1;
}




void UI_ResetOptionsList(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiOptionsLists.size()))
        return;
    UIOptionsList &list = g_uiOptionsLists[static_cast<std::size_t>(index)];
    list.inUse = 0;
    list.id[0] = 0;
    ResetUIWindow(list.window);
    list.window = -1;
    list.field48 = {0, 0};
    list.optionCount = 0;
    list.columns = 0;
    list.labelWidth = 0.0f;
    list.boxWidth = 0.0f;
    list.rows = 0;
    list.rowHeight = 0.0f;
    list.scrollDownOnSelection = -1;
    list.scrollUpOnSelection = -1;
    for (UIOptionsListOption &option : list.options) {
        option.inUse = 0;
        option.x = 0.0f;
        option.y = 0.0f;
        option.width = 0.0f;
        option.height = 0.0f;
        option.label.token = "";
        option.label.key[0] = 0;
        option.label.key[1] = 0;
        option.label.key[2] = 0;
        option.label.key[3] = 0;
        option.label.alreadyLocalized = 0;
        option.field40 = "";
        option.type = 0;
        option.value = 0;
        option.field60 = 0;
    }
    list.elementImages.fill(-1);
    list.selectedColour = 0xffffffffU;
    list.unselectedColour = 0xffffffffU;
    list.offsetColour = 0;
    list.font = 4;
}



void UI_ResetScrollListDraw(UIScrollListDraw &draw) {
    draw.image = -1;
    draw.method = 0;
    draw.place08[0] = {0.0f, 0.0f};
    draw.place08[1] = {0.0f, 0.0f};
    for (const std::int32_t style : {draw.selectedStyle, draw.unselectedStyle}) {
        if (static_cast<std::uint32_t>(style) < g_uiStyleItems.size())
            g_uiStyleItems[static_cast<std::size_t>(style)] = {.inUse = 0, .isColour = 1, .image = -1, .colour = {}};
    }
    draw.field108 = 1;
    draw.selectedStyle = -1;
    draw.unselectedStyle = -1;
    draw.fontSelectedColour = 0xffffffffU;
    draw.fontUnselectedColour = 0xffffffffU;
    draw.offsetColour = 0;
    draw.sizes.fill(2);
    draw.field118 = 0.0f;
}




void UI_ResetScrollList(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiScrollLists.size()))
        return;
    UIScrollList &list = g_uiScrollLists[static_cast<std::size_t>(index)];
    list.inUse = 0;
    list.id[0] = 0;
    list.field44 = 0;
    list.window = -1;
    list.rowHeight.scale = 1.0f;
    list.field64 = 0;
    list.columnWidth.fromImage = 0;
    list.columnWidth.value.fraction = 0.0f;
    list.columnWidth.scale = 1.0f;
    list.rowHeight.fromImage = 0;
    list.rowHeight.value.fraction = 0.0f;
    list.rows = 0;
    list.field6C = 0;
    list.drawCount = 0;
    list.field74 = {0, 0};
    list.field7C = 0;
    list.font = 4;
    list.field3A4 = 0;
    list.field3A8 = -1;
    list.field3AC = 0;
    list.selectorWidth = 0.0f;
    list.selectorHeight = 0.0f;
    list.selectorX = 0.0f;
    list.selectorY = 0.0f;
    list.field3CC = 0;
    list.selectorImage = -1;
    list.field3D4 = -1;
    list.texts.fill("");
    list.words.fill(-1);
    for (UIScrollListDraw &draw : list.draws)
        UI_ResetScrollListDraw(draw);
    ResetUIWindow(list.bar.window);
    list.bar.horizontal = 0;
    list.bar.window = -1;
    list.bar.outlineImage = -1;
    list.bar.barImage = -1;
    list.bar.topImage = -1;
    list.bar.bottomImage = -1;
    list.hasBar = 0;
}





void UI_ResetMenuBar(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiMenuBars.size()))
        return;
    UIMenuBar &bar = g_uiMenuBars[static_cast<std::size_t>(index)];
    if (g_uiReadStateInitialized == 1 && bar.inUse == 1) {
        ResetUIWindow(bar.extraWindows[0]);
        ResetUIWindow(bar.extraWindows[1]);
        if (static_cast<std::uint32_t>(bar.style) < g_uiStyleItems.size())
            g_uiStyleItems[static_cast<std::size_t>(bar.style)] = {.inUse = 0, .isColour = 1, .image = -1, .colour = {}};
        for (const std::int32_t window : bar.windows)
            ResetUIWindow(window);
    }
    bar.inUse = 0;
    bar.id[0] = 0;
    bar.optionCount = 0;
    bar.extraWindows = {-1, -1};
    bar.style = -1;
    bar.font = 4;
    bar.field184 = 0;
    bar.separatorImage = -1;
    for (std::size_t option = 0; option < bar.labels.size(); ++option) {
        bar.labels[option].token = "";
        bar.labels[option].key[0] = 0;
        bar.labels[option].key[1] = 0;
        bar.labels[option].key[2] = 0;
        bar.labels[option].key[3] = 0;
        bar.labels[option].alreadyLocalized = 0;
        bar.windows[option] = -1;
    }
    bar.offsetX = 0.0f;
    bar.offsetY = 0.0f;
    bar.image = -1;
    bar.window = -1;
    bar.left = -1;
    bar.right = -1;
    bar.width = 0.0f;
    bar.height = 0.0f;
    bar.optionsWidth = 0.0f;
    bar.optionsHeight = 0.0f;
    bar.selectedImage = -1;
    bar.selectedOverride = 0;
    for (std::size_t option = 0; option < bar.optionPositions.size(); ++option) {
        bar.optionPositions[option] = {0.0f, 0.0f};
        bar.selectedX[option] = 0.0f;
        bar.selectedY[option] = 0.0f;
        bar.selectedWidth[option] = 0.0f;
        bar.selectedHeight[option] = 0.0f;
    }
}




void UI_ReadScrollBar(cJSON *barObject, UIScrollBar &bar) {
    const d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
    cJSON *const panel = cJSON_GetObjectItem(barObject, "Panel");
    bar.window = panel ? UI_ReadWindow(panel) : -1;
    if (cJSON *const outline = cJSON_GetObjectItem(barObject, "Outline")) {
        const cJSON *const image = cJSON_GetObjectItem(outline, "Image");
        bar.outlineImage = d3t::FindViewImage(slot, image ? image->valuestring : "");
    }
    if (cJSON *const barMember = cJSON_GetObjectItem(barObject, "Bar")) {
        const cJSON *const image = cJSON_GetObjectItem(barMember, "Image");
        bar.barImage = d3t::FindViewImage(slot, image ? image->valuestring : "");
        const cJSON *const top = cJSON_GetObjectItem(barMember, "Top");
        bar.topImage = d3t::FindViewImage(slot, top ? top->valuestring : "");
        const cJSON *const bottom = cJSON_GetObjectItem(barMember, "Bottom");
        bar.bottomImage = d3t::FindViewImage(slot, bottom ? bottom->valuestring : "");
    }
    const cJSON *const direction = cJSON_GetObjectItem(barObject, "Direction");
    bar.horizontal = direction && direction->valuestring && !std::strcmp("Horizontal", direction->valuestring) ? 1 : 0;
}



void UI_ReadSnapshotDraw(cJSON *drawObject, UICollectionDraw &draw) {
    if (cJSON *const background = cJSON_GetObjectItem(drawObject, "Background")) {
        const cJSON *const image = cJSON_GetObjectItem(background, "Image");
        draw.backgroundImage = d3t::FindViewImage(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], image ? image->valuestring : "");
        const cJSON *const x = cJSON_GetObjectItem(background, "X");
        draw.backgroundX = static_cast<float>(static_cast<double>(x ? static_cast<float>(x->valuedouble) : 0.0f) / 1920.0);
        const cJSON *const y = cJSON_GetObjectItem(background, "Y");
        draw.backgroundY = static_cast<float>(static_cast<double>(y ? static_cast<float>(y->valuedouble) : 0.0f) / 1080.0);
        const cJSON *const width = cJSON_GetObjectItem(background, "Width");
        draw.backgroundWidth = static_cast<float>(static_cast<double>(width ? static_cast<float>(width->valuedouble) : 0.0f) / 1920.0);
        const cJSON *const height = cJSON_GetObjectItem(background, "Height");
        draw.backgroundHeight = static_cast<float>(static_cast<double>(height ? static_cast<float>(height->valuedouble) : 0.0f) / 1080.0);
    }
    if (cJSON *const main = cJSON_GetObjectItem(drawObject, "Main")) {
        const cJSON *const x = cJSON_GetObjectItem(main, "X");
        draw.mainX = static_cast<float>(static_cast<double>(x ? static_cast<float>(x->valuedouble) : 0.0f) / 1920.0);
        const cJSON *const y = cJSON_GetObjectItem(main, "Y");
        draw.mainY = static_cast<float>(static_cast<double>(y ? static_cast<float>(y->valuedouble) : 0.0f) / 1080.0);
        const cJSON *const width = cJSON_GetObjectItem(main, "Width");
        draw.mainWidth = static_cast<float>(static_cast<double>(width ? static_cast<float>(width->valuedouble) : 0.0f) / 1920.0);
        const cJSON *const height = cJSON_GetObjectItem(main, "Height");
        draw.mainHeight = static_cast<float>(static_cast<double>(height ? static_cast<float>(height->valuedouble) : 0.0f) / 1080.0);
    }
}



void UI_ReadCollectionDraw(std::int32_t collection, std::int32_t pageIndex, cJSON *drawObject) {
    const cJSON *const type = drawObject ? cJSON_GetObjectItem(drawObject, "Type") : nullptr;
    if (!type || !type->valuestring || std::strcmp(type->valuestring, "Snapshot"))
        return;
    UICollectionPage &page = g_uiCollections[static_cast<std::size_t>(collection)].pages[static_cast<std::size_t>(pageIndex)];
    page.drawType = 0;
    UI_ReadSnapshotDraw(drawObject, page.draw);
}




void UI_ReadCollectionPage(cJSON *pageObject, std::int32_t collection, std::int32_t pageIndex) {
    UICollectionPage &page = g_uiCollections[static_cast<std::size_t>(collection)].pages[static_cast<std::size_t>(pageIndex)];
    const cJSON *const columns = cJSON_GetObjectItem(pageObject, "Columns");
    page.columns = columns ? columns->valueint : 0;
    if (const cJSON *const parent = cJSON_GetObjectItem(pageObject, "ParentID"); parent && parent->valuestring && parent->valuestring[0])
        std::snprintf(page.parentId.data(), page.parentId.size(), "%s", parent->valuestring);
    if (const cJSON *const rows = cJSON_GetObjectItem(pageObject, "Rows"))
        page.rows = rows->valueint;
    if (const cJSON *const width = cJSON_GetObjectItem(pageObject, "ElementWidth"))
        page.elementWidth = static_cast<float>(width->valuedouble);
    if (const cJSON *const height = cJSON_GetObjectItem(pageObject, "ElementHeight"))
        page.elementHeight = static_cast<float>(height->valuedouble);
    if (cJSON *const scale = cJSON_GetObjectItem(pageObject, "Scale")) {
        if (const cJSON *const x = cJSON_GetObjectItem(scale, "X"))
            page.scaleX = static_cast<float>(x->valuedouble);
        if (const cJSON *const y = cJSON_GetObjectItem(scale, "Y"))
            page.scaleY = static_cast<float>(y->valuedouble);
    } else {
        page.scaleX = 1.0f;
        page.scaleY = 1.0f;
    }
    page.texture = -1;
    if (cJSON *const texturePage = cJSON_GetObjectItem(pageObject, "TexturePage")) {
        page.texture = UI_ReadTexturePage(texturePage);
    } else if (const cJSON *const sheetName = cJSON_GetObjectItem(pageObject, "SpriteSheet")) {
        for (std::size_t sheet = 0; sheetName->valuestring && sheet < g_uiSpriteSheets.size(); ++sheet) {
            const d3t::UISpriteSheet &candidate = g_uiSpriteSheets[sheet];
            if (candidate.imageName && candidate.used && !std::strcmp(candidate.imageName, sheetName->valuestring)) {
                page.texture = static_cast<std::int32_t>(sheet);
                break;
            }
        }
    }
    if (cJSON_GetObjectItem(pageObject, "Override")) {
        page.override = 1;
        if (cJSON *const draw = cJSON_GetObjectItem(pageObject, "Draw"))
            UI_ReadCollectionDraw(collection, pageIndex, draw);
    }
}







void UI_ReadCollection(cJSON *collectionObject) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiCollections.size(); ++record) {
        if (!g_uiCollections[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
    if (index != -1) {
        UICollection &collection = g_uiCollections[static_cast<std::size_t>(index)];
        collection.inUse = 1;
        const cJSON *const type = cJSON_GetObjectItem(collectionObject, "Type");
        if (type && type->valueint) {
            cJSON *const pages = cJSON_GetObjectItem(collectionObject, "Pages");
            collection.pageCount = pages ? cJSON_GetArraySize(pages) : 0;
            for (std::int32_t page = 0; page < collection.pageCount && page < static_cast<std::int32_t>(collection.pages.size()); ++page) {
                collection.pages[static_cast<std::size_t>(page)].inUse = 1;
                UI_ReadCollectionPage(cJSON_GetArrayItem(pages, page), index, page);
            }
        } else {
            UI_ReadCollectionPage(collectionObject, index, 0);
        }
        if (cJSON *const selected = cJSON_GetObjectItem(collectionObject, "Selected")) {
            if (cJSON_GetArraySize(selected) > 1) {
                const cJSON *const image = cJSON_GetObjectItem(selected, "Image");
                collection.selectedImage = d3t::FindViewImage(slot, image ? image->valuestring : "");
                collection.selectedX = 0.0f;
                collection.selectedY = 0.0f;
                collection.selectedWidth = 0.0f;
                collection.selectedHeight = 0.0f;
                const cJSON *const x = cJSON_GetObjectItem(selected, "X");
                const cJSON *const width = cJSON_GetObjectItem(selected, "Width");
                const cJSON *const height = cJSON_GetObjectItem(selected, "Height");
                if (x) {
                    collection.selectedX = static_cast<float>(static_cast<double>(static_cast<float>(x->valuedouble)) / 1920.0);
                    collection.selectedY = static_cast<float>(static_cast<double>(static_cast<float>(x->valuedouble)) / 1080.0);
                }
                if (width)
                    collection.selectedWidth = static_cast<float>(static_cast<double>(static_cast<float>(width->valuedouble)) / 1920.0);
                if (height)
                    collection.selectedHeight = static_cast<float>(static_cast<double>(static_cast<float>(height->valuedouble)) / 1080.0);
            } else {
                collection.selectedImage = d3t::FindViewImage(slot, selected->valuestring);
                if (collection.selectedImage == -1) {
                    const cJSON *const image = cJSON_GetObjectItem(selected, "Image");
                    collection.selectedImage = d3t::FindViewImage(slot, image ? image->valuestring : "");
                }
            }
        }
        if (cJSON *const data = cJSON_GetObjectItem(collectionObject, "Data"))
            collection.dataBinding = UI_ReadDataBinding(data);
        if (cJSON *const bar = cJSON_GetObjectItem(collectionObject, "Bar")) {
            collection.hasBar = 1;
            UI_ReadScrollBar(bar, collection.bar);
        }
        const cJSON *const id = cJSON_GetObjectItem(collectionObject, "Id");
        std::snprintf(collection.id.data(), collection.id.size(), "%s", id && id->valuestring ? id->valuestring : "");
    }
    d3t::AddViewWidget(slot, 25, index);
}










void UI_ReadOptionsList(cJSON *listObject) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiOptionsLists.size(); ++record) {
        if (!g_uiOptionsLists[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    UIOptionsList *const list = index == -1 ? nullptr : &g_uiOptionsLists[static_cast<std::size_t>(index)];
    if (list)
        list->inUse = 1;
    cJSON *const options = cJSON_GetObjectItem(listObject, "Options");
    if (!options)
        return;
    d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
    cJSON *const layout = cJSON_GetObjectItem(listObject, "Layout");
    if (list) {
        const cJSON *const down = cJSON_GetObjectItem(listObject, "ScrollDownOnSelection");
        list->scrollDownOnSelection = down ? down->valueint : -1;
        const cJSON *const up = cJSON_GetObjectItem(listObject, "ScrollUpOnSelection");
        list->scrollUpOnSelection = up ? up->valueint : -1;
        list->optionCount = cJSON_GetArraySize(options);
        for (std::int32_t at = 0; at < list->optionCount && at < static_cast<std::int32_t>(list->options.size()); ++at) {
            cJSON *const source = cJSON_GetArrayItem(options, at);
            UIOptionsListOption &option = list->options[static_cast<std::size_t>(at)];
            option.inUse = 1;
            const cJSON *const type = source ? cJSON_GetObjectItem(source, "Type") : nullptr;
            const char *const typeName = type && type->valuestring ? type->valuestring : "";
            option.type = -1;
            for (std::size_t name = 0; name < kUIOptionTypeNames.size(); ++name) {
                if (!std::strcmp(typeName, kUIOptionTypeNames[name])) {
                    option.type = static_cast<std::int32_t>(name);
                    break;
                }
            }
            const cJSON *const label = source ? cJSON_GetObjectItem(source, "Label") : nullptr;
            option.label.token = label ? label->valuestring : "";
            UI_SetLabelTokenKey(option.label);
            const cJSON *const x = source ? cJSON_GetObjectItem(source, "X") : nullptr;
            option.x = static_cast<float>(static_cast<double>(x ? static_cast<float>(x->valuedouble) : 0.0f) / 1920.0);
            const cJSON *const y = source ? cJSON_GetObjectItem(source, "Y") : nullptr;
            option.y = static_cast<float>(static_cast<double>(y ? static_cast<float>(y->valuedouble) : 0.0f) / 1080.0);
            const cJSON *const width = source ? cJSON_GetObjectItem(source, "Width") : nullptr;
            option.width = static_cast<float>(static_cast<double>(width ? static_cast<float>(width->valuedouble) : 0.0f) / 1920.0);
            const cJSON *const height = source ? cJSON_GetObjectItem(source, "Height") : nullptr;
            option.height = static_cast<float>(static_cast<double>(height ? static_cast<float>(height->valuedouble) : 0.0f) / 1080.0);
            const cJSON *const value = source ? cJSON_GetObjectItem(source, "Value") : nullptr;
            option.value = value ? value->valueint : -1;
            option.field4 = 1.0f;
            const cJSON *const tab = source ? cJSON_GetObjectItem(source, "Tab") : nullptr;
            option.tab = tab ? tab->valueint : 0;
            const cJSON *const action = source ? cJSON_GetObjectItem(source, "Action") : nullptr;
            option.action = 0;
            for (std::size_t name = 0; action && action->valuestring && name < kInputActionNames.size(); ++name) {
                if (!std::strncmp(action->valuestring, std::string_view(kInputActionNames[name]).data(), 64)) {
                    option.action = static_cast<std::int32_t>(name);
                    break;
                }
            }
        }
        if (cJSON *const images = cJSON_GetObjectItem(listObject, "Images")) {
            for (cJSON *element = images->child; element; element = element->next) {
                std::size_t type = 0;
                while (type < kUIElementTypeNames.size() && std::strcmp(element->string ? element->string : "", kUIElementTypeNames[type]))
                    ++type;
                if (type == kUIElementTypeNames.size())
                    continue;
                const cJSON *const image = cJSON_GetObjectItem(element, "Image");
                list->elementImages[type] = d3t::FindViewImage(slot, image ? image->valuestring : "");
                const cJSON *const width = cJSON_GetObjectItem(element, "Width");
                const cJSON *const height = cJSON_GetObjectItem(element, "Height");
                const cJSON *const x = cJSON_GetObjectItem(element, "X");
                const cJSON *const y = cJSON_GetObjectItem(element, "Y");
                list->elementSizes[type] = {static_cast<float>(static_cast<double>(width ? static_cast<float>(width->valuedouble) : 0.0f) / 1920.0),
                    static_cast<float>(static_cast<double>(height ? static_cast<float>(height->valuedouble) : 0.0f) / 1080.0)};
                list->elementPositions[type] = {static_cast<float>(static_cast<double>(x ? static_cast<float>(x->valuedouble) : 0.0f) / 1920.0),
                    static_cast<float>(static_cast<double>(y ? static_cast<float>(y->valuedouble) : 0.0f) / 1080.0)};
            }
        }
        const cJSON *const columns = layout ? cJSON_GetObjectItem(layout, "Columns") : nullptr;
        list->columns = columns ? columns->valueint : -1;
        const cJSON *const rows = layout ? cJSON_GetObjectItem(layout, "Rows") : nullptr;
        list->rows = rows ? rows->valueint : -1;
    }
    cJSON *const panel = cJSON_GetObjectItem(listObject, "Panel");
    const std::int32_t window = panel ? UI_ReadWindow(panel) : -1;
    if (list) {
        list->window = window;
        const cJSON *const height = layout ? cJSON_GetObjectItem(layout, "Height") : nullptr;
        const cJSON *const label = layout ? cJSON_GetObjectItem(layout, "Label") : nullptr;
        const cJSON *const box = layout ? cJSON_GetObjectItem(layout, "Box") : nullptr;
        const float heightValue = height ? static_cast<float>(height->valuedouble) : 0.0f;
        const float labelValue = label ? static_cast<float>(label->valuedouble) : 0.0f;
        const float boxValue = box ? static_cast<float>(box->valuedouble) : 0.0f;
        if (static_cast<std::uint32_t>(window) <= 0x3ffU) {
            const d3t::UIWindow &record = g_uiWindows[static_cast<std::size_t>(window)];
            list->rowHeight = heightValue / static_cast<float>(static_cast<double>(record.layout.height.value.fraction) * 1080.0);
            list->labelWidth = labelValue / static_cast<float>(static_cast<double>(record.layout.width.value.fraction) * 1920.0);
            list->boxWidth = boxValue / static_cast<float>(static_cast<double>(record.layout.width.value.fraction) * 1920.0);
        } else {
            list->rowHeight = static_cast<float>(static_cast<double>(heightValue) / 1080.0);
            list->labelWidth = static_cast<float>(static_cast<double>(labelValue) / 1920.0);
            list->boxWidth = static_cast<float>(static_cast<double>(boxValue) / 1920.0);
        }
        if (const cJSON *const font = cJSON_GetObjectItem(listObject, "Font")) {
            list->font = 4;
            for (std::size_t name = 0; font->valuestring && name < kIconFontSizeNames.size(); ++name) {
                if (kIconFontSizeNames[name] == font->valuestring) {
                    list->font = static_cast<std::int32_t>(name);
                    break;
                }
            }
        }
        list->iconFontSize = 5;
        if (const cJSON *const size = cJSON_GetObjectItem(listObject, "IconFontSize")) {
            list->iconFontSize = 4;
            for (std::size_t name = 0; size->valuestring && name < kIconFontSizeNames.size(); ++name) {
                if (kIconFontSizeNames[name] == size->valuestring) {
                    list->iconFontSize = static_cast<std::int32_t>(name);
                    break;
                }
            }
        }
        list->selectedColour = UI_ReadColour4(cJSON_GetObjectItem(listObject, "Selected"));
        const cJSON *const selectedImage = cJSON_GetObjectItem(listObject, "SelectedImage");
        list->selectedImage = d3t::FindViewImage(slot, selectedImage ? selectedImage->valuestring : "");
        list->fieldEC8 = 0;
        list->unselectedColour = UI_ReadColour4(cJSON_GetObjectItem(listObject, "Unselected"));
        list->offsetColour = UI_ReadColour4(cJSON_GetObjectItem(listObject, "Offset"));
        const cJSON *const id = cJSON_GetObjectItem(listObject, "Id");
        std::snprintf(list->id.data(), list->id.size(), "%s", id && id->valuestring ? id->valuestring : "");
    }
    d3t::AddViewWidget(slot, 26, index);
}



std::int32_t UI_ReadStateStyle(cJSON *styleObject) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiStateStyles.size(); ++record) {
        if (!g_uiStateStyles[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    if (index == -1)
        return -1;
    UIStateStyle &style = g_uiStateStyles[static_cast<std::size_t>(index)];
    style.inUse = 1;
    if (cJSON *const normal = cJSON_GetObjectItem(styleObject, "Normal"))
        style.normal = UI_ParseStyleItem(normal);
    if (cJSON *const hover = cJSON_GetObjectItem(styleObject, "Hover"))
        style.hover = UI_ParseStyleItem(hover);
    if (cJSON *const active = cJSON_GetObjectItem(styleObject, "Active"))
        style.active = UI_ParseStyleItem(active);
    return index;
}


std::int32_t UI_FindSpriteSheet(const char *name) {
    for (std::size_t sheet = 0; name && sheet < g_uiSpriteSheets.size(); ++sheet) {
        const d3t::UISpriteSheet &candidate = g_uiSpriteSheets[sheet];
        if (candidate.imageName && candidate.used && !std::strcmp(candidate.imageName, name))
            return static_cast<std::int32_t>(sheet);
    }
    return -1;
}


float UI_ReadFraction(cJSON *object, const char *name, double reference) {
    const cJSON *const member = object ? cJSON_GetObjectItem(object, name) : nullptr;
    return static_cast<float>(static_cast<double>(member ? static_cast<float>(member->valuedouble) : 0.0f) / reference);
}


std::int32_t UI_ReadViewImage(cJSON *object, const char *name) {
    const cJSON *const member = object ? cJSON_GetObjectItem(object, name) : nullptr;
    return d3t::FindViewImage(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], member ? member->valuestring : "");
}


std::int32_t UI_ReadSizeIndex(const cJSON *size) {
    for (std::size_t name = 0; size && size->valuestring && name < kIconFontSizeNames.size(); ++name) {
        if (kIconFontSizeNames[name] == size->valuestring)
            return static_cast<std::int32_t>(name);
    }
    return 4;
}








void UI_ReadScrollListDraw(cJSON *drawObject, UIScrollListDraw &draw, std::int32_t window) {
    if (!drawObject)
        return;
    const cJSON *const methodMember = cJSON_GetObjectItem(drawObject, "Method");
    const char *const method = methodMember && methodMember->valuestring ? methodMember->valuestring : "";
    if (!std::strcmp(method, "Move")) {
        draw.method = 1;
        cJSON *const move = cJSON_GetObjectItem(drawObject, "Move");
        const cJSON *const sheet = move ? cJSON_GetObjectItem(move, "SpriteSheet") : nullptr;
        draw.image = UI_FindSpriteSheet(sheet && sheet->valuestring ? sheet->valuestring : "");
        float moveWidth = 1920.0f;
        float moveHeight = 1080.0f;
        if (cJSON *const place = move ? cJSON_GetObjectItem(move, "Move") : nullptr) {
            draw.place60[0] = {UI_ReadFraction(place, "X", 1920.0), UI_ReadFraction(place, "Y", 1080.0)};
            const cJSON *const width = cJSON_GetObjectItem(place, "Width");
            const cJSON *const height = cJSON_GetObjectItem(place, "Height");
            moveWidth = width ? static_cast<float>(width->valuedouble) : 0.0f;
            moveHeight = height ? static_cast<float>(height->valuedouble) : 0.0f;
            draw.size20[0] = {static_cast<float>(static_cast<double>(moveWidth) / 1920.0), static_cast<float>(static_cast<double>(moveHeight) / 1080.0)};
        }
        if (cJSON *const input = move ? cJSON_GetObjectItem(move, "Input") : nullptr) {
            draw.place60[1] = {UI_ReadFraction(input, "X", 1920.0), UI_ReadFraction(input, "Y", 1080.0)};
            const cJSON *const width = cJSON_GetObjectItem(input, "Width");
            const cJSON *const height = cJSON_GetObjectItem(input, "Height");
            draw.size20[1] = {
                (width ? static_cast<float>(width->valuedouble) : 0.0f) / moveWidth, (height ? static_cast<float>(height->valuedouble) : 0.0f) / moveHeight};
            const cJSON *const offset = cJSON_GetObjectItem(input, "Offset");
            draw.field118 = offset ? static_cast<float>(offset->valuedouble) : 0.0f;
        }
        if (cJSON *const image = move ? cJSON_GetObjectItem(move, "Image") : nullptr) {
            constexpr std::array<const char *, 3> kParts{"Outline", "Bar", "Selected"};
            for (std::size_t part = 0; part < kParts.size(); ++part) {
                if (cJSON *const member = cJSON_GetObjectItem(image, kParts[part])) {
                    draw.images[part] = UI_ReadViewImage(member, "Image");
                    draw.colours[part] = UI_ReadColour4(cJSON_GetObjectItem(member, "Colour"));
                }
            }
            constexpr std::array<const char *, 4> kCaps{"OnCapLeft", "OnCapRight", "OffCapLeft", "OffCapRight"};
            for (std::size_t cap = 0; cap < kCaps.size(); ++cap)
                draw.images[3 + cap] = UI_ReadViewImage(image, kCaps[cap]);
            draw.size20[2] = {UI_ReadFraction(image, "Width", 1920.0), UI_ReadFraction(image, "Height", 1080.0)};
            if (cJSON *const caps = cJSON_GetObjectItem(image, "Caps")) {
                draw.size20[3] = {UI_ReadFraction(caps, "Width", 1920.0), UI_ReadFraction(caps, "Height", 1080.0)};
                draw.place60[3] = {UI_ReadFraction(caps, "X", 1920.0), UI_ReadFraction(caps, "Y", 1080.0)};
            }
        }
        if (cJSON *const font = move ? cJSON_GetObjectItem(move, "Font") : nullptr) {
            draw.fontSelectedColour = UI_ReadColour4(cJSON_GetObjectItem(font, "Selected"));
            draw.fontUnselectedColour = UI_ReadColour4(cJSON_GetObjectItem(font, "Unselected"));
        }
        draw.selectedStyle = UI_ReadViewImage(move, "Selected");
    } else if (!std::strcmp(method, "Scroll")) {
        draw.method = 2;
        cJSON *const scroll = cJSON_GetObjectItem(drawObject, "Scroll");
        const cJSON *const sheet = scroll ? cJSON_GetObjectItem(scroll, "SpriteSheet") : nullptr;
        draw.image = UI_FindSpriteSheet(sheet ? sheet->valuestring : nullptr);
        cJSON *const pos = scroll ? cJSON_GetObjectItem(scroll, "Pos") : nullptr;
        const cJSON *const posX = pos ? cJSON_GetArrayItem(pos, 0) : nullptr;
        const cJSON *const posY = pos ? cJSON_GetArrayItem(pos, 1) : nullptr;
        draw.place08[0] = {static_cast<float>(static_cast<double>(posX ? static_cast<float>(posX->valuedouble) : 0.0f) / 1920.0),
            static_cast<float>(static_cast<double>(posY ? static_cast<float>(posY->valuedouble) : 0.0f) / 1080.0)};
        cJSON *const icon = scroll ? cJSON_GetObjectItem(scroll, "Icon") : nullptr;
        draw.place08[1] = {UI_ReadFraction(icon, "Width", 1920.0), UI_ReadFraction(icon, "Height", 1080.0)};
        cJSON *const iconPos = icon ? cJSON_GetObjectItem(icon, "Pos") : nullptr;
        const cJSON *const iconX = iconPos ? cJSON_GetArrayItem(iconPos, 0) : nullptr;
        const cJSON *const iconY = iconPos ? cJSON_GetArrayItem(iconPos, 1) : nullptr;
        draw.place08[2] = {static_cast<float>(static_cast<double>(iconX ? static_cast<float>(iconX->valuedouble) : 0.0f) / 1920.0),
            static_cast<float>(static_cast<double>(iconY ? static_cast<float>(iconY->valuedouble) : 0.0f) / 1080.0)};
        if (cJSON *const font = scroll ? cJSON_GetObjectItem(scroll, "Font") : nullptr) {
            draw.fontSelectedColour = UI_ReadColour4(cJSON_GetObjectItem(font, "Selected"));
            draw.fontUnselectedColour = UI_ReadColour4(cJSON_GetObjectItem(font, "Unselected"));
            draw.sizes[0] = UI_ReadSizeIndex(cJSON_GetObjectItem(font, "Size"));
        }
        if (cJSON *const selected = scroll ? cJSON_GetObjectItem(scroll, "Selected") : nullptr) {
            draw.images[0] = UI_ReadViewImage(selected, "Image");
            draw.size20[0] = {UI_ReadFraction(selected, "Width", 1920.0), UI_ReadFraction(selected, "Height", 1080.0)};
        }
    } else if (!std::strcmp(method, "Save")) {
        draw.method = 3;
        cJSON *const save = cJSON_GetObjectItem(drawObject, "Save");
        const cJSON *const image = save ? cJSON_GetObjectItem(save, "Image") : nullptr;
        draw.image = image ? d3t::FindViewImage(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], image->valuestring) : -1;
        const cJSON *const width = save ? cJSON_GetObjectItem(save, "Width") : nullptr;
        draw.field11C = width ? static_cast<float>(width->valuedouble) : 0.0f;
        const d3t::UIWindow *const record = static_cast<std::uint32_t>(window) <= 0x3ffU ? &g_uiWindows[static_cast<std::size_t>(window)] : nullptr;
        cJSON *time = save ? cJSON_GetObjectItem(save, "Time") : nullptr;
        for (std::size_t entry = 0; entry < 5 && time; ++entry, time = time->next) {
            std::array<float, 4> values{};
            for (int at = 0; at < 4; ++at) {
                const cJSON *const value = cJSON_GetArrayItem(time, at);
                values[static_cast<std::size_t>(at)] = value ? static_cast<float>(value->valuedouble) : 0.0f;
            }
            if (record) {
                const float authoredWidth = static_cast<float>(static_cast<double>(record->layout.width.value.fraction) * 1920.0);
                const float authoredHeight = static_cast<float>(static_cast<double>(record->layout.height.value.fraction) * 1080.0);
                draw.size20[entry] = {values[0] / authoredWidth, values[1] / authoredHeight};
                draw.place60[entry] = {values[2] / authoredWidth, values[3] / authoredHeight};
            } else {
                draw.size20[entry] = {static_cast<float>(static_cast<double>(values[0]) / 1920.0), static_cast<float>(static_cast<double>(values[1]) / 1080.0)};
                draw.place60[entry] = {
                    static_cast<float>(static_cast<double>(values[2]) / 1920.0), static_cast<float>(static_cast<double>(values[3]) / 1080.0)};
            }
            if (const cJSON *const size = cJSON_GetObjectItem(time, "Size"))
                draw.sizes[entry] = UI_ReadSizeIndex(size);
        }
        const cJSON *const padding = save ? cJSON_GetObjectItem(save, "Padding") : nullptr;
        const float saveWidth = draw.field11C;
        draw.field118 = (padding ? static_cast<float>(padding->valuedouble) : 0.0f) / saveWidth;
        draw.field11C = static_cast<float>(static_cast<double>(saveWidth) / 1920.0);
        if (cJSON *const selected = save ? cJSON_GetObjectItem(save, "Selected") : nullptr)
            draw.fontSelectedColour = UI_ReadColour4(selected);
        if (cJSON *const unselected = save ? cJSON_GetObjectItem(save, "Unselected") : nullptr)
            draw.fontUnselectedColour = UI_ReadColour4(unselected);
        if (cJSON *const offset = save ? cJSON_GetObjectItem(save, "Offset") : nullptr)
            draw.offsetColour = UI_ReadColour4(offset);
    } else if (!std::strcmp(method, "Map")) {
        draw.method = 4;
        cJSON *const map = cJSON_GetObjectItem(drawObject, "Map");
        draw.place08[0] = {UI_ReadFraction(map, "X", 1920.0), UI_ReadFraction(map, "Y", 1080.0)};
        draw.place08[1] = {UI_ReadFraction(map, "Width", 1920.0), UI_ReadFraction(map, "Height", 1080.0)};
        if (cJSON *const font = map ? cJSON_GetObjectItem(map, "Font") : nullptr) {
            draw.fontSelectedColour = UI_ReadColour4(cJSON_GetObjectItem(font, "Selected"));
            draw.fontUnselectedColour = UI_ReadColour4(cJSON_GetObjectItem(font, "Unselected"));
        }
        draw.selectedStyle = UI_ReadViewImage(map, "Selected");
        if (cJSON *const image = map ? cJSON_GetObjectItem(map, "Image") : nullptr) {
            draw.image = UI_ReadViewImage(image, "Image");
            draw.colours[0] = UI_ReadColour4(cJSON_GetObjectItem(image, "Colour"));
        }
    } else if (!std::strcmp(method, "Pawnshop") || !std::strcmp(method, "Standard")) {
        const bool pawnshop = !std::strcmp(method, "Pawnshop");
        draw.method = pawnshop ? 5 : 6;
        cJSON *const body = cJSON_GetObjectItem(drawObject, method);
        if (pawnshop) {
            if (cJSON *const set = body ? cJSON_GetObjectItem(body, "Set") : nullptr) {
                draw.place60[0] = {UI_ReadFraction(set, "X", 1920.0), UI_ReadFraction(set, "Y", 1080.0)};
                draw.size20[0] = {UI_ReadFraction(set, "Width", 1920.0), UI_ReadFraction(set, "Height", 1080.0)};
            }
            if (cJSON *const cost = body ? cJSON_GetObjectItem(body, "Cost") : nullptr) {
                draw.place60[1] = {UI_ReadFraction(cost, "X", 1920.0), UI_ReadFraction(cost, "Y", 1080.0)};
                draw.size20[1] = {UI_ReadFraction(cost, "Width", 1920.0), UI_ReadFraction(cost, "Height", 1080.0)};
            }
            if (cJSON *const setIcon = body ? cJSON_GetObjectItem(body, "SetIcon") : nullptr) {
                draw.place60[2] = {UI_ReadFraction(setIcon, "X", 1920.0), UI_ReadFraction(setIcon, "Y", 1080.0)};
                draw.size20[2] = {UI_ReadFraction(setIcon, "Width", 1920.0), UI_ReadFraction(setIcon, "Height", 1080.0)};
                draw.place08[2] = {UI_ReadFraction(setIcon, "XOffset", 1920.0), UI_ReadFraction(setIcon, "YOffset", 1080.0)};
            }
        }
        draw.selectedStyle = UI_ReadViewImage(body, "Selected");
        if (cJSON *const image = body ? cJSON_GetObjectItem(body, "Image") : nullptr) {
            draw.image = UI_ReadViewImage(image, "Image");
            draw.colours[0] = UI_ReadColour4(cJSON_GetObjectItem(image, "Colour"));
        }
    }
    if (cJSON *const selected = cJSON_GetObjectItem(drawObject, "Selected"))
        draw.selectedStyle = UI_ReadStateStyle(selected);
    if (cJSON *const unselected = cJSON_GetObjectItem(drawObject, "Unselected"))
        draw.unselectedStyle = UI_ReadStateStyle(unselected);
}







void UI_ReadScrollList(cJSON *listObject) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiScrollLists.size(); ++record) {
        if (!g_uiScrollLists[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    if (index != -1) {
        UIScrollList &list = g_uiScrollLists[static_cast<std::size_t>(index)];
        list.inUse = 1;
        cJSON *const panel = cJSON_GetObjectItem(listObject, "Panel");
        list.window = panel ? UI_ReadWindow(panel) : -1;
        cJSON *const listMember = cJSON_GetObjectItem(listObject, "List");
        const cJSON *const rows = listMember ? cJSON_GetObjectItem(listMember, "Rows") : nullptr;
        list.rows = rows ? rows->valueint : 0;
        list.rowCount = list.rows;
        if (listMember) {
            list.columnWidth = UI_ReadWindowLength(cJSON_GetObjectItem(listMember, "ColumnWidth"), listMember, "WImage", 1920.0f);
            list.rowHeight = UI_ReadWindowLength(cJSON_GetObjectItem(listMember, "RowHeight"), listMember, "HImage", 1080.0f);
        }
        if (const cJSON *const font = cJSON_GetObjectItem(listObject, "Font"))
            list.font = UI_ReadSizeIndex(font);
        const cJSON *const iconFontSize = cJSON_GetObjectItem(listObject, "IconFontSize");
        list.iconFontSize = iconFontSize ? UI_ReadSizeIndex(iconFontSize) : 5;
        list.drawCount = 0;
        cJSON *const draws = cJSON_GetObjectItem(listObject, "Draw");
        const std::int32_t drawTotal = draws ? cJSON_GetArraySize(draws) : 0;
        for (std::int32_t draw = 0; draw < drawTotal && draw < static_cast<std::int32_t>(list.draws.size()); ++draw) {
            UI_ReadScrollListDraw(cJSON_GetArrayItem(draws, draw), list.draws[static_cast<std::size_t>(draw)], list.window);
            ++list.drawCount;
        }
        list.field7C = 0;
        if (cJSON *const selector = cJSON_GetObjectItem(listObject, "Selector")) {
            const cJSON *const width = cJSON_GetObjectItem(selector, "Width");
            const cJSON *const height = cJSON_GetObjectItem(selector, "Height");
            const cJSON *const x = cJSON_GetObjectItem(selector, "X");
            const cJSON *const y = cJSON_GetObjectItem(selector, "Y");
            list.selectorWidth = width ? static_cast<float>(width->valuedouble) : 0.0f;
            list.selectorHeight = height ? static_cast<float>(height->valuedouble) : 0.0f;
            list.selectorX = x ? static_cast<float>(x->valuedouble) : 0.0f;
            list.selectorY = y ? static_cast<float>(y->valuedouble) : 0.0f;
            list.selectorImage = UI_ReadViewImage(selector, "Image");
        } else {
            list.selectorImage = -1;
        }
        if (cJSON *const bar = cJSON_GetObjectItem(listObject, "Bar")) {
            list.hasBar = 1;
            UI_ReadScrollBar(bar, list.bar);
        }
        const cJSON *const id = cJSON_GetObjectItem(listObject, "Id");
        std::snprintf(list.id.data(), list.id.size(), "%s", id && id->valuestring ? id->valuestring : "");
    }
    d3t::AddViewWidget(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], 27, index);
}


std::int32_t UI_TakeWindow() {
    for (std::size_t window = 0; window < g_uiWindows.size(); ++window) {
        if (!g_uiWindows[window].inUse) {
            g_uiWindows[window].inUse = 1;
            return static_cast<std::int32_t>(window);
        }
    }
    return -1;
}







void UI_ReadMenuBarWithoutPanel(cJSON *menuBar, UIMenuBar *bar) {
    if (!bar)
        return;
    bar->inUse = 1;
    cJSON *const options = cJSON_GetObjectItem(menuBar, "Options");
    const cJSON *const count = options ? cJSON_GetObjectItem(options, "Count") : nullptr;
    const std::int32_t optionCount = count ? count->valueint : 0;
    bar->optionCount = optionCount;
    bar->field184 = 0;
    cJSON *const xs = options ? cJSON_GetObjectItem(options, "X") : nullptr;
    cJSON *const ys = options ? cJSON_GetObjectItem(options, "Y") : nullptr;
    cJSON *const xAlign = xs ? cJSON_GetObjectItem(xs, "Align") : nullptr;
    cJSON *const yAlign = ys ? cJSON_GetObjectItem(ys, "Align") : nullptr;
    cJSON *const xOffset = xs ? cJSON_GetObjectItem(xs, "Offset") : nullptr;
    cJSON *const yOffset = ys ? cJSON_GetObjectItem(ys, "Offset") : nullptr;
    cJSON *const labels = options ? cJSON_GetObjectItem(options, "Labels") : nullptr;
    cJSON *const widthMember = options ? cJSON_GetObjectItem(options, "Width") : nullptr;
    cJSON *const heightMember = options ? cJSON_GetObjectItem(options, "Height") : nullptr;
    const d3t::UIWindowLength width = UI_ReadWindowLength(widthMember, options, "WImage", 1920.0f);
    const d3t::UIWindowLength height = UI_ReadWindowLength(heightMember, options, "WImage", 1080.0f);
    for (std::int32_t at = 0; at < optionCount && at < static_cast<std::int32_t>(bar->windows.size()); ++at) {
        const std::int32_t window = UI_TakeWindow();
        bar->windows[static_cast<std::size_t>(at)] = window;
        if (window >= 0) {
            d3t::UIWindow &record = g_uiWindows[static_cast<std::size_t>(window)];
            record.layout.width = width;
            record.layout.height = height;
            record.layout.alignX = d3t::ReadSpriteAlignmentValue(xAlign ? cJSON_GetArrayItem(xAlign, at) : nullptr);
            record.layout.alignY = d3t::ReadSpriteAlignmentValue(yAlign ? cJSON_GetArrayItem(yAlign, at) : nullptr);
            const cJSON *const x = xOffset ? cJSON_GetArrayItem(xOffset, at) : nullptr;
            const cJSON *const y = yOffset ? cJSON_GetArrayItem(yOffset, at) : nullptr;
            const double xValue = x ? x->valuedouble : 0.0;
            const double yValue = y ? y->valuedouble : 0.0;
            record.layout.offsetX = static_cast<float>(static_cast<double>(static_cast<float>(xValue / 1920.0)) + xValue);
            record.layout.offsetY = static_cast<float>(static_cast<double>(static_cast<float>(yValue / 1080.0)) + yValue);
        }
        const cJSON *const label = labels ? cJSON_GetArrayItem(labels, at) : nullptr;
        bar->labels[static_cast<std::size_t>(at)].token = label ? label->valuestring : nullptr;
        UI_SetLabelTokenKey(bar->labels[static_cast<std::size_t>(at)]);
    }
    cJSON *const selected = cJSON_GetObjectItem(menuBar, "Selected");
    const std::int32_t style = selected ? UI_ParseStyleItem(selected) : -1;
    bar->field28 = style;
    const std::int32_t selectedWindow = UI_TakeWindow();
    bar->extraWindows[1] = selectedWindow;
    if (selectedWindow >= 0) {
        d3t::UIWindow &record = g_uiWindows[static_cast<std::size_t>(selectedWindow)];
        if (static_cast<std::uint32_t>(style) < g_uiStyleItems.size() && g_uiStyleItems[static_cast<std::size_t>(style)].isColour) {
            record.layout.width = {.fromImage = 0, .value = {.fraction = widthMember ? static_cast<float>(widthMember->valuedouble) : 0.0f}, .scale = 1.0f};
            record.layout.height = {.fromImage = 0, .value = {.fraction = heightMember ? static_cast<float>(heightMember->valuedouble) : 0.0f}, .scale = 1.0f};
        } else {
            const std::int32_t image = static_cast<std::uint32_t>(style) < g_uiStyleItems.size() ?
                static_cast<std::int32_t>(static_cast<float>(g_uiStyleItems[static_cast<std::size_t>(style)].image)) :
                0;
            record.layout.width = {.fromImage = 1, .value = {.image = image}, .scale = 1.0f};
            record.layout.height = {.fromImage = 1, .value = {.image = image}, .scale = 1.0f};
        }
        cJSON *const selectedX = selected ? cJSON_GetObjectItem(selected, "X") : nullptr;
        cJSON *const selectedY = selected ? cJSON_GetObjectItem(selected, "Y") : nullptr;
        const cJSON *const offsetX = selectedX ? cJSON_GetObjectItem(selectedX, "Offset") : nullptr;
        const cJSON *const offsetY = selectedY ? cJSON_GetObjectItem(selectedY, "Offset") : nullptr;
        const std::int32_t first = selectedWindow - optionCount;
        const float firstOffsetX =
            first >= 0 && first < static_cast<std::int32_t>(g_uiWindows.size()) ? g_uiWindows[static_cast<std::size_t>(first)].layout.offsetX : 0.0f;
        record.layout.offsetX = static_cast<float>(static_cast<double>(offsetX ? static_cast<float>(offsetX->valuedouble) : 0.0f) / 1920.0) + firstOffsetX;
        record.layout.offsetY = static_cast<float>(static_cast<double>(offsetY ? static_cast<float>(offsetY->valuedouble) : 0.0f) / 1080.0) + firstOffsetX;
    }
    cJSON *const styleObject = cJSON_GetObjectItem(menuBar, "Style");
    cJSON *const windowStyle = styleObject ? cJSON_GetObjectItem(styleObject, "Window") : nullptr;
    if (cJSON *const background = windowStyle ? cJSON_GetObjectItem(windowStyle, "FixedBackground") : nullptr)
        bar->style = UI_ParseStyleItem(background);
    bar->extraWindows[0] = UI_ReadWindow(menuBar);
    if (const cJSON *const font = cJSON_GetObjectItem(menuBar, "Font"))
        bar->font = UI_ReadSizeIndex(font);
}








void UI_ReadMenuBar(cJSON *menuBar) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiMenuBars.size(); ++record) {
        if (!g_uiMenuBars[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    UIMenuBar *const bar = index == -1 ? nullptr : &g_uiMenuBars[static_cast<std::size_t>(index)];
    cJSON *const panel = cJSON_GetObjectItem(menuBar, "Panel");
    if (!panel) {
        UI_ReadMenuBarWithoutPanel(menuBar, bar);
        d3t::AddViewWidget(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], 24, index);
        return;
    }
    if (bar) {
        bar->inUse = 1;
        bar->field2D8 = 1;
    }
    const std::int32_t window = UI_ReadWindow(panel);
    if (bar) {
        bar->window = window;
        if (const cJSON *const image = cJSON_GetObjectItem(menuBar, "Image"))
            bar->image = d3t::FindViewImage(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], image->valuestring);
        bar->hasButtonIcon = 0;
        bar->field1A4 = -1;
        bar->left = -1;
        bar->right = -1;
        std::int32_t optionCount = -1;
        if (cJSON *const buttonIcon = cJSON_GetObjectItem(menuBar, "ButtonIcon")) {
            bar->hasButtonIcon = 1;
            const cJSON *const image = cJSON_GetObjectItem(buttonIcon, "Image");
            std::snprintf(bar->buttonIconImage.data(), bar->buttonIconImage.size(), "%s", image && image->valuestring ? image->valuestring : "");
            bar->field1A4 = -1;
            bar->width = UI_ReadFraction(buttonIcon, "Width", 1920.0);
            bar->height = UI_ReadFraction(buttonIcon, "Height", 1080.0);
            for (std::int32_t *const side : {&bar->left, &bar->right}) {
                const cJSON *const action = cJSON_GetObjectItem(buttonIcon, side == &bar->left ? "Left" : "Right");
                *side = 0;
                for (std::size_t name = 0; action && action->valuestring && name < kInputActionNames.size(); ++name) {
                    if (!std::strncmp(action->valuestring, std::string_view(kInputActionNames[name]).data(), 64)) {
                        *side = static_cast<std::int32_t>(name);
                        break;
                    }
                }
            }
            const cJSON *const iconFontSize = cJSON_GetObjectItem(buttonIcon, "IconFontSize");
            bar->iconFontSize = iconFontSize ? UI_ReadSizeIndex(iconFontSize) : 5;
            if (cJSON *const offset = cJSON_GetObjectItem(buttonIcon, "Offset")) {
                bar->offsetX = UI_ReadFraction(offset, "X", 1920.0);
                bar->offsetY = UI_ReadFraction(offset, "Y", 1080.0);
            }
        } else if (cJSON *const images = cJSON_GetObjectItem(menuBar, "Images")) {
            bar->hasButtonIcon = 0;
            bar->left = UI_ReadViewImage(images, "Left");
            bar->right = UI_ReadViewImage(images, "Right");
            bar->width = UI_ReadFraction(images, "Width", 1920.0);
            const cJSON *const iconFontSize = cJSON_GetObjectItem(images, "IconFontSize");
            bar->iconFontSize = iconFontSize ? UI_ReadSizeIndex(iconFontSize) : 5;
            bar->height = UI_ReadFraction(images, "Height", 1080.0);
            if (cJSON *const offset = cJSON_GetObjectItem(images, "Offset")) {
                bar->offsetX = UI_ReadFraction(offset, "X", 1920.0);
                bar->offsetY = UI_ReadFraction(offset, "Y", 1080.0);
            }
        }
        if (cJSON *const options = cJSON_GetObjectItem(menuBar, "Options")) {
            if (const cJSON *const count = cJSON_GetObjectItem(options, "Count"))
                optionCount = count->valueint;
            bar->optionCount = optionCount;
            bar->optionsWidth = UI_ReadFraction(options, "Width", 1920.0);
            bar->optionsHeight = UI_ReadFraction(options, "Height", 1080.0);
            cJSON *const xs = cJSON_GetObjectItem(options, "X");
            cJSON *const ys = cJSON_GetObjectItem(options, "Y");
            cJSON *const labels = cJSON_GetObjectItem(options, "Labels");
            for (std::int32_t at = 0; xs && ys && labels && at < bar->optionCount && at < static_cast<std::int32_t>(bar->labels.size()); ++at) {
                const cJSON *const x = cJSON_GetArrayItem(xs, at);
                const cJSON *const y = cJSON_GetArrayItem(ys, at);
                const float xValue = x ? static_cast<float>(x->valuedouble) : 0.0f;
                const float yValue = y ? static_cast<float>(y->valuedouble) : 0.0f;
                bar->optionPositions[static_cast<std::size_t>(at)] = {
                    static_cast<float>(static_cast<double>(xValue) / 1920.0), static_cast<float>(static_cast<double>(yValue) / 1080.0)};
                const cJSON *const label = cJSON_GetArrayItem(labels, at);
                bar->labels[static_cast<std::size_t>(at)].token = label ? label->valuestring : "";
                UI_SetLabelTokenKey(bar->labels[static_cast<std::size_t>(at)]);
            }
        }
        if (cJSON *const separator = cJSON_GetObjectItem(menuBar, "Separator")) {
            if (const cJSON *const image = cJSON_GetObjectItem(separator, "Image"))
                bar->separatorImage = d3t::FindViewImage(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], image->valuestring);
            if (cJSON *const scale = cJSON_GetObjectItem(separator, "Scale")) {
                if (const cJSON *const x = cJSON_GetObjectItem(scale, "X"))
                    bar->separatorScaleX = static_cast<float>(x->valuedouble);
                if (const cJSON *const y = cJSON_GetObjectItem(scale, "Y"))
                    bar->separatorScaleY = static_cast<float>(y->valuedouble);
            } else {
                bar->separatorScaleX = 1.0f;
                bar->separatorScaleY = 1.0f;
            }
            const cJSON *const alpha = cJSON_GetObjectItem(separator, "Alpha");
            bar->separatorAlpha = alpha ? static_cast<float>(alpha->valuedouble) : 1.0f;
        }
        if (cJSON *const selected = cJSON_GetObjectItem(menuBar, "Selected")) {
            bar->selectedImage = UI_ReadViewImage(selected, "Image");
            bar->selectedColour = UI_ReadColour4(cJSON_GetObjectItem(selected, "Colour"));
            if (cJSON_GetObjectItem(selected, "Override")) {
                bar->selectedOverride = 1;
                cJSON *const xs = cJSON_GetObjectItem(selected, "X");
                cJSON *const ys = cJSON_GetObjectItem(selected, "Y");
                cJSON *const widths = cJSON_GetObjectItem(selected, "Width");
                cJSON *const heights = cJSON_GetObjectItem(selected, "Height");
                for (std::int32_t at = 0; xs && ys && widths && heights && at < bar->optionCount && at < 8; ++at) {
                    const std::size_t slot = static_cast<std::size_t>(at);
                    const cJSON *const x = cJSON_GetArrayItem(xs, at);
                    const cJSON *const y = cJSON_GetArrayItem(ys, at);
                    const cJSON *const width = cJSON_GetArrayItem(widths, at);
                    const cJSON *const height = cJSON_GetArrayItem(heights, at);
                    const float xValue = x ? static_cast<float>(x->valuedouble) : 0.0f;
                    const float yValue = y ? static_cast<float>(y->valuedouble) : 0.0f;
                    const float widthValue = width ? static_cast<float>(width->valuedouble) : 0.0f;
                    const float heightValue = height ? static_cast<float>(height->valuedouble) : 0.0f;
                    bar->selectedX[slot] = static_cast<float>(static_cast<double>(xValue) / 1920.0);
                    bar->selectedY[slot] = static_cast<float>(static_cast<double>(yValue) / 1080.0);
                    bar->selectedWidth[slot] = static_cast<float>(static_cast<double>(widthValue) / 1920.0);
                    bar->selectedHeight[slot] = static_cast<float>(static_cast<double>(heightValue) / 1080.0);
                }
            } else {
                bar->selectedX[0] = UI_ReadFraction(selected, "X", 1920.0);
                bar->selectedY[0] = UI_ReadFraction(selected, "Y", 1080.0);
                bar->selectedWidth[0] = UI_ReadFraction(selected, "Width", 1920.0);
                bar->selectedHeight[0] = UI_ReadFraction(selected, "Height", 1080.0);
            }
        }
        bar->font = UI_ReadSizeIndex(cJSON_GetObjectItem(menuBar, "Size"));
        const cJSON *const id = cJSON_GetObjectItem(menuBar, "Id");
        std::snprintf(bar->id.data(), bar->id.size(), "%s", id && id->valuestring ? id->valuestring : "");
    }
    d3t::AddViewWidget(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], 24, index);
}

void UI_DrawWindowBegin(nk_context &context, std::int32_t index);

// The view image `image` of the drawn slot, or the 2x2 default image while it is not ready (or out of range).
struct nk_image UI_ReadyViewImage(std::int32_t image) {
    if (static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU)
        return g_uiDefaultImage;
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    if (static_cast<std::size_t>(image) >= view.images.size() || view.images[static_cast<std::size_t>(image)].ready != 1)
        return g_uiDefaultImage;
    struct nk_image drawn {};
    std::memcpy(&drawn, &view.images[static_cast<std::size_t>(image)].textureHandle, sizeof(drawn));
    return drawn;
}



void UI_DrawCollectionCount(nk_context &context, const UILayoutRect &cell, std::int32_t count, const UICollectionPage &page) {
    if (count <= 0)
        return;
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        context.current->layout->row.item = nk_rect(cell.x, cell.y, cell.width, cell.height);
    std::array<char, 16> text{};
    std::snprintf(text.data(), text.size(), "%i", count);
    const UILabelFit fit = UI_FitLabelScale(context, text.data(), -1, 36, cell, page.elementWidth, page.elementHeight, 1);
    UI_DrawLabelText(context, text.data(), -1, 36, cell, fit);
}



void UI_DrawCollectionSprite(nk_context &context, const struct nk_image &image, nk_color colour, const UICollectionDraw &draw, const UILayoutRect &cell) {
    if (static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU)
        return;
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    const auto placeItem = [&context](const UILayoutRect &item) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(item.x, item.y, item.width, item.height);
    };
    placeItem({.x = cell.x + 2.0f, .y = cell.y + 2.0f, .width = cell.width - 4.0f, .height = cell.height - 4.0f});
    if (g_waitingForNextTick == 1) {
        const std::int32_t record = draw.field0[1];
        const bool ready = static_cast<std::size_t>(record) < view.images.size() && view.images[static_cast<std::size_t>(record)].ready == 1;
        nk_image_color(&context, ready ? image : g_uiDefaultImage, colour);
        placeItem(cell);
    }
    if (g_waitingForNextTick == 1)
        nk_image_color(&context, UI_ReadyViewImage(draw.backgroundImage), nk_rgba(255, 255, 255, 255));
}







void UI_DrawScrollBar(nk_context &context, std::int32_t pageCount, std::int32_t page, const UIScrollBar &bar) {
    d3t::UIWindow *const window = UI_GetWindow(bar.window);
    if (!window || static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU || pageCount <= 0)
        return;
    d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    UILayoutRect rect{};
    UI_GetWindowRect(window, rect);
    if (g_waitingForNextTick == 1) {
        nk_panel *const panel = reinterpret_cast<nk_panel *>(window->panel.data());
        nk_begin_titled(
            &context, panel, "scroll bar innit", "scroll bar innit", nk_rect(rect.x, rect.y, rect.width, rect.height), static_cast<nk_flags>(window->flags));
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 2);
    std::array<std::uint8_t, 4> channels{};
    std::memcpy(channels.data(), &window->colour, sizeof(channels));
    const std::uint32_t windowColour =
        ConstructColour(channels[0], channels[1], channels[2], static_cast<std::int32_t>(static_cast<float>(channels[3]) * window->alpha));
    nk_color tint{};
    std::memcpy(&tint, &windowColour, sizeof(windowColour));
    const auto drawPart = [&context, tint](std::int32_t image, const UILayoutRect &item) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(item.x, item.y, item.width, item.height);
        if (g_waitingForNextTick == 1)
            nk_image_color(&context, UI_ReadyViewImage(image), tint);
    };
    const auto turn = [&view](std::int32_t image, float angle) {
        if (static_cast<std::size_t>(image) >= view.images.size())
            return;
        view.images[static_cast<std::size_t>(image)].rotate = 1;
        view.images[static_cast<std::size_t>(image)].angle = angle;
    };
    const float y = rect.y;
    const float width = rect.width;
    const float height = rect.height;
    drawPart(bar.outlineImage, {.x = 0.0f, .y = 0.0f, .width = width, .height = height});
    if (bar.horizontal == 0) {
        turn(bar.topImage, 1.57079506f);
        drawPart(bar.topImage, {.x = 0.0f, .y = y, .width = width, .height = width});
        const float step = (height - width) * (1.0f / static_cast<float>(pageCount));
        turn(bar.bottomImage, -1.57079506f);
        drawPart(bar.bottomImage, {.x = 0.0f, .y = y + step, .width = width, .height = width});
        drawPart(bar.barImage,
            {.x = 0.0f, .y = static_cast<float>(page) / static_cast<float>(pageCount) * height + width, .width = step - width * 0.9f, .height = width});
    } else {
        turn(bar.topImage, 0.0f);
        const float step = (width - height) * (1.0f / static_cast<float>(pageCount));
        const float start = static_cast<float>(page) / static_cast<float>(pageCount) * width;
        drawPart(bar.topImage, {.x = start, .y = 0.0f, .width = height, .height = height});
        turn(bar.bottomImage, 3.14159012f);
        drawPart(bar.bottomImage, {.x = start + step, .y = 0.0f, .width = height, .height = height});
        turn(bar.barImage, 1.57079506f);
        drawPart(bar.barImage, {.x = start + height, .y = 0.0f, .width = step - height * 0.9f, .height = height});
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout) {
        nk_row_layout &row = context.current->layout->row;
        row.item_width = 0.0f;
        row.item_height = 0.0f;
        row.item_offset = 0.0f;
        std::memset(&row.item, g_waitingForNextTick, sizeof(row.item));
    }
    if (g_waitingForNextTick == 1 && context.current) {
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
}












void UI_DrawOptionsList(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) > 5U || static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU)
        return;
    UIOptionsList &list = g_uiOptionsLists[static_cast<std::size_t>(index)];
    d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    d3t::UIWindow *const window = UI_GetWindow(list.window);
    if (list.field48[1] != 0 || !window)
        return;
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    context.style.window.background = nk_rgba(0, 0, 0, 0);
    UILayoutRect rect{};
    UI_GetWindowRect(window, rect);
    const float rowHeight = rect.height / static_cast<float>(list.rows);
    const float fitHeight = window->layout.height.value.fraction * 1080.0f / static_cast<float>(list.rows);
    if (g_uiFontSize != list.font && static_cast<std::uint32_t>(list.font) < 6U) {
        g_uiFontSize = list.font;
        UI_SelectFont();
    }
    std::array<char, 32> name{};
    std::snprintf(name.data(), name.size(), "optionList%d", index);
    if (g_waitingForNextTick == 1) {
        nk_panel *const panel = reinterpret_cast<nk_panel *>(window->panel.data());
        nk_begin_titled(&context, panel, name.data(), name.data(), nk_rect(rect.x, rect.y, rect.width, rect.height), static_cast<nk_flags>(window->flags));
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 3 * list.optionCount);
    std::array<std::uint8_t, 4> tint{};
    {
        std::array<std::uint8_t, 4> channels{};
        std::memcpy(channels.data(), &window->colour, sizeof(channels));
        const std::uint32_t windowColour =
            ConstructColour(channels[0], channels[1], channels[2], static_cast<std::int32_t>(static_cast<float>(channels[3]) * window->alpha));
        std::memcpy(tint.data(), &windowColour, sizeof(tint));
    }
    const nk_color savedColour = context.style.text.color;
    const auto placeItem = [&context](const UILayoutRect &item) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(item.x, item.y, item.width, item.height);
    };
    const auto drawImage = [&context](const struct nk_image &image, nk_color colour) {
        if (g_waitingForNextTick == 1)
            nk_image_color(&context, image, colour);
    };
    const nk_color separatorColour = nk_rgba(255, 255, 255, 50);
    const std::int32_t separatorElement = list.elementImages[4];
    std::int32_t separator = separatorElement;
    UILayoutRect separatorRect{};
    if (separatorElement != -1) {
        separator = separatorElement + (g_uiElements[kUiGameMenu].isVisible != 0 || g_uiElements[kUiGameMenuBackground].isVisible != 0 ? 1 : 0);
        const bool valid = static_cast<std::size_t>(separator) < view.images.size();
        const float imageWidth = valid ? view.images[static_cast<std::size_t>(separator)].widthScale * 1920.0f : 0.0f;
        const float imageHeight = valid ? view.images[static_cast<std::size_t>(separator)].heightScale * 1080.0f : 0.0f;
        if (rect.height != 0.0f && rect.width != 0.0f && imageWidth != 0.0f && imageHeight != 0.0f) {
            if (rect.height / rect.width > imageHeight / imageWidth) {
                separatorRect.height = rect.width / imageWidth * imageHeight;
                separatorRect.width = rect.width;
            } else {
                separatorRect.height = rect.height;
                separatorRect.width = imageHeight / imageWidth != rect.height / rect.width ? rect.height / imageHeight * imageWidth : rect.width;
            }
        }
        separatorRect.x = separatorRect.x + rect.width * 0.5f - separatorRect.width * 0.5f;
        separatorRect.y = separatorRect.y - separatorRect.height * 0.5f;
        placeItem(separatorRect);
        drawImage(UI_ReadyViewImage(separator), separatorColour);
    }
    const float boxWidth = rect.width * list.boxWidth;
    for (std::int32_t row = 0; row < list.rows; ++row) {
        if (separatorElement != -1) {
            separatorRect.y = separatorRect.y + rowHeight;
            placeItem(separatorRect);
            drawImage(UI_ReadyViewImage(separator), separatorColour);
        }
        const std::int32_t optionIndex = list.field54 + row;
        if (optionIndex >= list.optionCount || static_cast<std::size_t>(optionIndex) >= list.options.size())
            break;
        const UIOptionsListOption &option = list.options[static_cast<std::size_t>(optionIndex)];
        const float rowY = static_cast<float>(row) * rowHeight;
        const UILayoutRect labelRect{.x = 0.0f, .y = rowY, .width = rect.width * list.labelWidth, .height = rowHeight};
        placeItem(labelRect);
        std::array<std::uint8_t, 4> colour{};
        if (list.field48[0] + list.field54 == optionIndex) {
            colour = {255, 255, 255, static_cast<std::uint8_t>(static_cast<std::int32_t>(option.field4 * 255.0f))};
            colour[0] = static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<float>(tint[0]) / 255.0f * 255.0f));
        } else {
            std::array<std::uint8_t, 4> unselected{};
            std::memcpy(unselected.data(), &list.unselectedColour, sizeof(unselected));
            std::array<std::uint8_t, 4> offset{};
            std::memcpy(offset.data(), &list.offsetColour, sizeof(offset));
            const std::int32_t distance = std::abs(optionIndex - list.field48[0] - list.field54) - 1;
            const std::int32_t fade = static_cast<std::int32_t>(1.0f / static_cast<float>(list.rows - 1) * static_cast<float>(offset[3]));
            const float alpha = static_cast<float>(static_cast<std::int32_t>(offset[3]) - distance * fade) / 255.0f * 255.0f * option.field4;
            const std::uint32_t built = ConstructColour(unselected[0] - distance * offset[0], unselected[1] - distance * offset[1],
                unselected[2] - distance * offset[2], static_cast<std::int32_t>(alpha));
            std::memcpy(colour.data(), &built, sizeof(colour));
            colour[0] = static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<float>(colour[0]) * (static_cast<float>(tint[0]) / 255.0f)));
        }
        for (std::size_t channel = 1; channel < colour.size(); ++channel) {
            const float scale = static_cast<float>(tint[channel]) / 255.0f;
            colour[channel] = static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<float>(colour[channel]) * scale));
        }
        std::memcpy(&context.style.text.color, colour.data(), sizeof(colour));
        SubtitleStringId id{};
        std::memcpy(&id, option.label.key.data(), sizeof(id));
        const char *const label = ReadStringFromStringTableById(id);
        std::array<char, 256> text{};
        if (option.tab <= 0)
            std::snprintf(text.data(), text.size(), "%s", label ? label : "(null)");
        else
            std::snprintf(text.data(), text.size(), "%*s%s", option.tab, "   ", label ? label : "(null)");
        const UILabelFit labelFit = UI_FitLabelScale(context, text.data(), -1, 17, labelRect, list.labelWidth * 1920.0f, fitHeight, 1);
        UI_DrawLabelText(context, text.data(), -1, 17, labelRect, labelFit);
        const float boxX = rect.width - boxWidth;
        const float boxSpan = rect.width * list.boxWidth;
        const UILayoutRect boxRect{.x = boxX, .y = rowY, .width = boxSpan, .height = rowHeight};
        if (list.field48[0] == optionIndex) {
            std::array<std::uint8_t, 4> selected{};
            std::memcpy(selected.data(), &list.selectedColour, sizeof(selected));
            for (std::size_t channel = 0; channel < selected.size(); ++channel) {
                const float scale = static_cast<float>(tint[channel]) / 255.0f;
                selected[channel] = static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<float>(selected[channel]) * scale));
            }
            std::memcpy(&context.style.text.color, selected.data(), sizeof(selected));
        }
        const nk_color imageColour = nk_rgba(tint[0], tint[1], tint[2], context.style.text.color.a);
        const auto fitGlyph = [&](float cellWidth, float cellHeight) {
            float width = 0.0f;
            float height = 0.0f;
            if (rowHeight != 0.0f && boxSpan != 0.0f && cellWidth != 0.0f && cellHeight != 0.0f) {
                if (rowHeight / boxSpan > cellHeight / cellWidth) {
                    height = boxSpan / cellWidth * cellHeight;
                    width = boxSpan;
                } else {
                    height = rowHeight;
                    width = cellHeight / cellWidth > rowHeight / boxSpan ? rowHeight / cellHeight * cellWidth : boxSpan;
                }
            }
            const float drawWidth = width * 0.8f;
            const float drawHeight = height * 0.8f;
            return UILayoutRect{
                .x = boxX + boxSpan * 0.5f - drawWidth * 0.5f, .y = rowY + rowHeight * 0.5f - drawHeight * 0.5f, .width = drawWidth, .height = drawHeight};
        };
        const auto drawGlyph = [&](std::int32_t glyph, std::int32_t action, std::int32_t controlMethod, std::int32_t controlState) {
            const d3t::UIViewImage *const image = g_platformButtonIconImages[0];
            const struct nk_image sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(g_platformIconsSheet), glyph, image);
            const UILayoutRect glyphRect = fitGlyph(static_cast<float>(sprite.region[2]), static_cast<float>(sprite.region[3]));
            placeItem(glyphRect);
            drawImage(image && image->ready == 1 ? sprite : g_uiDefaultImage, imageColour);
            const UIButtonIconLabel keyLabel{.rect = glyphRect,
                .fontWidth = glyphRect.width,
                .fontHeight = window->layout.height.value.fraction * 1080.0f,
                .action = action,
                .doubleWidth = 1,
                .iconFontSize = list.iconFontSize,
                .colour = 0xffffffffU,
                .alignment = 18,
                .controlMethod = controlMethod,
                .controlState = controlState};
            UI_DrawButtonIconLabel(context, keyLabel);
        };
        switch (option.type) {
        case 0:
            placeItem(boxRect);
            if (option.field40)
                UI_FitAndDrawLabelText(context, option.field40, -1, 18, boxRect, list.boxWidth * 1920.0f, fitHeight, 1);
            break;
        case 1: {
            float trackWidth = 0.0f;
            float trackHeight = 0.0f;
            const float sliderHeight = list.elementSizes[1][1] * 1080.0f;
            const float sliderWidth = list.elementSizes[1][0] * 1920.0f;
            if (rowHeight != 0.0f && boxSpan != 0.0f && sliderWidth != 0.0f && sliderHeight != 0.0f) {
                if (rowHeight / boxSpan > sliderHeight / sliderWidth) {
                    trackHeight = boxSpan / sliderWidth * sliderHeight;
                    trackWidth = boxSpan;
                } else {
                    trackHeight = rowHeight;
                    trackWidth = sliderHeight / sliderWidth > rowHeight / boxSpan ? rowHeight / sliderHeight * sliderWidth : boxSpan;
                }
            }
            const float track = trackWidth * 0.8f;
            UILayoutRect slider{
                .x = boxWidth * 0.5f + boxX - track * 0.5f, .y = rowHeight * 0.5f + rowY - trackHeight * 0.5f, .width = track, .height = trackHeight};
            placeItem(slider);
            drawImage(UI_ReadyViewImage(list.elementImages[1]), imageColour);
            float position = 0.0f;
            std::memcpy(&position, &option.field50[0], sizeof(position));
            slider.width = trackHeight;
            slider.x = slider.x + (track - trackHeight) * position;
            placeItem(slider);
            drawImage(UI_ReadyViewImage(list.elementImages[2]), imageColour);
            break;
        }
        case 3:
            if (option.value < 4) {
                drawGlyph(ResolveButtonIconGlyph(option.value, -1, -1, 1), option.value, -1, -1);
            } else if (static_cast<std::size_t>(option.value + 5) < list.elementImages.size()) {
                const std::array<float, 2> &size = list.elementSizes[static_cast<std::size_t>(option.value + 5)];
                const UILayoutRect imageRect = fitGlyph(size[0] * 1920.0f, size[1] * 1080.0f);
                placeItem(imageRect);
                drawImage(UI_ReadyViewImage(list.elementImages[static_cast<std::size_t>(option.value + 5)]), imageColour);
            }
            break;
        case 4: {
            const std::int32_t action = option.action;
            InputMode type = InputMode::Game;
            if (static_cast<std::uint32_t>(action - 22) <= 8U || static_cast<std::uint32_t>(action - 54) <= 1U)
                type = InputMode::Fight;
            else if (static_cast<std::uint32_t>(action - 18) <= 3U)
                type = InputMode::Qte2;
            drawGlyph(ResolvePressedKey(action, static_cast<std::int32_t>(type), 1), action, 1, static_cast<std::int32_t>(type));
            break;
        }
        default:
            break;
        }
        if (list.selectedImage == -1 || row != list.field48[0] || list.fieldEC8 != 1)
            continue;
        float scale = 1.0f;
        float alpha = 1.0f;
        if (const d3t::UIWindow *const animator = UI_FindWindowById("ListSelectorAnimator")) {
            scale = animator->layout.offsetX;
            alpha = animator->alpha;
        }
        const nk_color cornerColour = nk_rgba(255, 255, 255, static_cast<nk_byte>(static_cast<std::int32_t>(255.0f * alpha)));
        const float spanWidth = boxRect.width * scale;
        const float left = (boxSpan - scale * boxSpan) * 0.5f + boxX;
        const float spanHeight = scale * boxRect.height;
        const float top = boxRect.y + (boxRect.height - spanHeight) * 0.5f;
        d3t::UIViewImage *const corner =
            static_cast<std::size_t>(list.selectedImage) < view.images.size() ? &view.images[static_cast<std::size_t>(list.selectedImage)] : nullptr;
        const float cornerWidth = corner ? static_cast<float>(corner->width) : 0.0f;
        const float cornerHeight = corner ? static_cast<float>(corner->height) : 0.0f;
        const float halfHeight = spanHeight * 0.5f;
        const float halfWidth = spanWidth * 0.5f;
        float pieceWidth = 0.0f;
        float pieceHeight = 0.0f;
        if (halfHeight != 0.0f && halfWidth != 0.0f && cornerWidth != 0.0f && cornerHeight != 0.0f) {
            if (halfHeight / halfWidth > cornerHeight / cornerWidth) {
                pieceHeight = halfWidth / cornerWidth * cornerHeight;
                pieceWidth = halfWidth;
            } else {
                pieceHeight = halfHeight;
                pieceWidth = cornerHeight / cornerWidth <= halfHeight / halfWidth ? halfWidth : halfHeight / cornerHeight * cornerWidth;
            }
        }
        if (corner) {
            corner->rotate = 1;
            corner->angle = 0.0f;
        }
        const float right = spanWidth - pieceWidth + left;
        const float bottom = spanHeight - pieceHeight + top;
        const std::array<std::array<float, 2>, 4> corners{{{left, top}, {right, top}, {right, bottom}, {left, bottom}}};
        for (std::size_t piece = 0; piece < corners.size(); ++piece) {
            if (piece && corner)
                corner->angle = corner->angle + 1.57079506f;
            placeItem({.x = corners[piece][0], .y = corners[piece][1], .width = pieceWidth, .height = pieceHeight});
            drawImage(UI_ReadyViewImage(list.selectedImage), cornerColour);
        }
    }
    if (separatorElement != -1) {
        placeItem({.x = rect.width * 0.5f - boxWidth, .y = rect.height * 0.5f, .width = separatorRect.width, .height = separatorRect.height});
        struct nk_image turned = UI_ReadyViewImage(separator);
        const bool ready = static_cast<std::size_t>(separator) < view.images.size() && view.images[static_cast<std::size_t>(separator)].ready == 1;
        if (ready) {
            turned.rotate = 1;
            turned.angle = turned.angle + 1.57079506f;
        }
        drawImage(turned, separatorColour);
    }
    if (g_waitingForNextTick == 1) {
        if (context.current && context.current->layout) {
            nk_row_layout &layoutRow = context.current->layout->row;
            layoutRow.item_width = 0.0f;
            layoutRow.item_height = 0.0f;
            layoutRow.item_offset = 0.0f;
            layoutRow.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (context.current) {
            if (!(context.current->flags & NK_WINDOW_HIDDEN))
                nk_end(&context);
            context.current = nullptr;
        }
    }
    context.style.text.color = savedColour;
}









void UI_DrawCollection(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) > 7U || static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU)
        return;
    UICollection &collection = g_uiCollections[static_cast<std::size_t>(index)];
    if (collection.field44[0] == 1 || static_cast<std::uint32_t>(collection.field94) >= collection.pages.size())
        return;
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    context.style.window.background = nk_rgba(0, 0, 0, 0);
    const UICollectionPage &page = collection.pages[static_cast<std::size_t>(collection.field94)];
    d3t::UIWindow *const window = UI_FindWindowById(page.parentId.data());
    std::uint32_t windowColour = 0;
    nk_panel panel{};
    nk_flags flags = 0;
    UILayoutRect rect{};
    if (window) {
        std::array<std::uint8_t, 4> channels{};
        std::memcpy(channels.data(), &window->colour, sizeof(channels));
        windowColour = ConstructColour(channels[0], channels[1], channels[2], static_cast<std::int32_t>(static_cast<float>(channels[3]) * window->alpha));
        UI_GetWindowRect(window, rect);
        std::memcpy(&panel, window->panel.data(), sizeof(panel));
        flags = static_cast<nk_flags>(window->flags);
    }
    nk_color tint{};
    std::memcpy(&tint, &windowColour, sizeof(windowColour));
    const auto tinted = [windowColour](std::uint32_t colour) {
        std::array<std::uint8_t, 4> base{};
        std::memcpy(base.data(), &windowColour, sizeof(base));
        std::array<std::uint8_t, 4> channels{};
        std::memcpy(channels.data(), &colour, sizeof(channels));
        for (std::size_t channel = 0; channel < channels.size(); ++channel) {
            const float scale = static_cast<float>(base[channel]) / 255.0f;
            channels[channel] = static_cast<std::uint8_t>(static_cast<std::int32_t>(scale * static_cast<float>(channels[channel])));
        }
        nk_color result{};
        std::memcpy(&result, channels.data(), sizeof(result));
        return result;
    };
    const auto placeItem = [&context](const UILayoutRect &item) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(item.x, item.y, item.width, item.height);
    };
    std::array<char, 32> name{};
    std::snprintf(name.data(), name.size(), "coll%d", index);
    if (g_waitingForNextTick == 1)
        nk_begin_titled(&context, &panel, name.data(), name.data(), nk_rect(rect.x, rect.y, rect.width, rect.height), flags);
    const float cellWidth = rect.width / static_cast<float>(page.columns);
    const float cellHeight = rect.height / static_cast<float>(page.rows);
    const std::int32_t selectedInPage = collection.field64[4] - collection.field64[3] * page.columns * page.rows;
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 4);
    float fitWidth = 0.0f;
    float fitHeight = 0.0f;
    if (cellHeight != 0.0f && cellWidth != 0.0f && page.elementWidth != 0.0f && page.elementHeight != 0.0f) {
        if (cellHeight / cellWidth > page.elementHeight / page.elementWidth) {
            fitHeight = cellWidth / page.elementWidth * page.elementHeight;
            fitWidth = cellWidth;
        } else {
            fitHeight = cellHeight;
            fitWidth = page.elementHeight / page.elementWidth > cellHeight / cellWidth ? cellHeight / page.elementHeight * page.elementWidth : cellWidth;
        }
    }
    const float elementWidth = fitWidth * page.scaleX;
    const float elementHeight = fitHeight * page.scaleY;
    const float offsetX = cellWidth * 0.5f + 0.0f - elementWidth * 0.5f;
    const float offsetY = cellHeight * 0.5f + 0.0f - elementHeight * 0.5f;
    float selectedX = offsetX;
    float selectedY = offsetY;
    std::int32_t item = 0;
    for (std::int32_t row = 0; row < page.rows; ++row) {
        const float y = static_cast<float>(row) * cellHeight + offsetY;
        for (std::int32_t column = 0; column < page.columns; ++column, ++item) {
            const UILayoutRect cell{.x = static_cast<float>(column) * cellWidth + offsetX, .y = y, .width = elementWidth, .height = elementHeight};
            placeItem(cell);
            if (item == collection.field64[2])
                break;
            const std::int32_t slotIndex = column + row * page.columns;
            const bool hasElement = static_cast<std::size_t>(slotIndex) < collection.elements.size();
            const UICollectionElement element = hasElement ? collection.elements[static_cast<std::size_t>(slotIndex)] : UICollectionElement{};
            if (page.field5C == 1 || page.field5C == 2) {
                if (page.field5C == 1) {
                    if (page.override != 0 && page.drawType == 1)
                        UI_DrawCollectionCount(context, cell, element.field0[0], page);
                    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
                    struct nk_image image {};
                    if (static_cast<std::size_t>(element.field0[1]) < view.images.size())
                        std::memcpy(&image, &view.images[static_cast<std::size_t>(element.field0[1])].textureHandle, sizeof(image));
                    std::array<float, 4> uv{};
                    std::memcpy(uv.data(), &element.field0[2], sizeof(uv));
                    const float imageWidth = static_cast<float>(static_cast<std::int32_t>(image.w));
                    const float imageHeight = static_cast<float>(image.h);
                    const float top = imageHeight - uv[3] * imageHeight;
                    image.region[0] = static_cast<unsigned short>(static_cast<std::int32_t>(uv[0] * imageWidth));
                    image.region[1] = static_cast<unsigned short>(static_cast<std::int32_t>(top));
                    image.region[2] = static_cast<unsigned short>(static_cast<std::int32_t>(uv[2] * imageWidth - uv[0] * imageWidth));
                    image.region[3] = static_cast<unsigned short>(static_cast<std::int32_t>(imageHeight - uv[1] * imageHeight - top));
                    const bool ready =
                        static_cast<std::size_t>(element.field0[1]) < view.images.size() && view.images[static_cast<std::size_t>(element.field0[1])].ready == 1;
                    if (g_waitingForNextTick == 1)
                        nk_image_color(&context, ready ? image : g_uiDefaultImage, tinted(element.colour));
                    if (page.override != 0 && page.drawType == 1)
                        UI_DrawCollectionCount(context, cell, element.field0[0], page);
                } else {
                    context.style.button.border_color = nk_rgba(0, 0, 0, 0);
                    const std::byte *images = nullptr;
                    std::memcpy(&images, page.field60.data(), sizeof(images));
                    const std::int32_t record = column + page.columns * (row + collection.field64[3] * page.rows);
                    struct nk_image image {};
                    std::uint32_t imageColour = 0;
                    if (images) {
                        std::memcpy(&image, images + 40 * record, sizeof(image));
                        std::memcpy(&imageColour, images + 40 * record + 32, sizeof(imageColour));
                    }
                    if (page.override == 0) {
                        const std::int32_t readyRecord = page.draw.field0[1];
                        const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
                        const bool ready =
                            static_cast<std::size_t>(readyRecord) < view.images.size() && view.images[static_cast<std::size_t>(readyRecord)].ready == 1;
                        if (g_waitingForNextTick == 1)
                            nk_image_color(&context, ready ? image : g_uiDefaultImage, tinted(imageColour));
                    } else if (page.drawType == 0) {
                        UI_DrawCollectionSprite(context, image, tinted(element.colour), page.draw, cell);
                    }
                }
                if (item == selectedInPage) {
                    if (collection.selectedImage != -1) {
                        selectedX = cell.x;
                        selectedY = y;
                    }
                    continue;
                }
                if (collection.field7C == -1)
                    continue;
                placeItem(cell);
            }
            if (g_waitingForNextTick == 1)
                nk_image_color(&context, UI_ReadyViewImage(collection.selectedImage), tint);
        }
    }
    if (g_waitingForNextTick == 1) {
        if (context.current && context.current->layout) {
            nk_row_layout &layoutRow = context.current->layout->row;
            layoutRow.item_width = 0.0f;
            layoutRow.item_height = 0.0f;
            layoutRow.item_offset = 0.0f;
            layoutRow.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (context.current) {
            if (!(context.current->flags & NK_WINDOW_HIDDEN))
                nk_end(&context);
            context.current = nullptr;
        }
        nk_begin_titled(&context, &g_uiCollectionSelectorPanel, "Coll X Selector", "Coll X Selector", nk_rect(rect.x, rect.y, rect.width, rect.height),
            NK_WINDOW_NO_SCROLLBAR);
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, 1);
    }
    const float grow = elementHeight * 0.100000024f;
    placeItem({.x = selectedX - grow * 0.5f, .y = selectedY - grow * 0.5f, .width = grow + elementWidth, .height = grow + elementHeight});
    if (collection.field64[2] > 0 && g_waitingForNextTick == 1)
        nk_image_color(&context, UI_ReadyViewImage(collection.selectedImage), tint);
    if (g_waitingForNextTick == 1) {
        if (context.current && context.current->layout) {
            nk_row_layout &layoutRow = context.current->layout->row;
            layoutRow.item_width = 0.0f;
            layoutRow.item_height = 0.0f;
            layoutRow.item_offset = 0.0f;
            layoutRow.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (context.current) {
            if (!(context.current->flags & NK_WINDOW_HIDDEN))
                nk_end(&context);
            context.current = nullptr;
        }
    }
    if (collection.hasBar) {
        const std::int32_t perPage = page.rows * page.columns;
        const std::int32_t current = perPage ? collection.field64[4] / perPage : 0;
        const std::int32_t pages = perPage ? collection.field64[0] / perPage + (collection.field64[0] % perPage != 0 ? 1 : 0) : 0;
        d3t::UIWindow *const barWindow = UI_GetWindow(collection.bar.window);
        if (barWindow && window)
            barWindow->alpha = window->alpha;
        UI_DrawScrollBar(context, pages, current, collection.bar);
    }
}











void UI_DrawMenuBar(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) > 9U || static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU)
        return;
    UIMenuBar &bar = g_uiMenuBars[static_cast<std::size_t>(index)];
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    if (bar.window == -1) {
        context.style.window.fixed_background = UI_StyleItem(bar.style);
        UI_DrawWindowBegin(context, bar.window);
        if (context.current && g_waitingForNextTick == 1) {
            if (!(context.current->flags & NK_WINDOW_HIDDEN))
                nk_end(&context);
            context.current = nullptr;
        }
        context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
        context.style.window.background = nk_rgba(0, 0, 0, 0);
        context.style.button.normal = nk_style_item_color(nk_rgba(0, 0, 0, 0));
        context.style.button.border_color = nk_rgba(0, 0, 0, 0);
        context.style.button.hover = nk_style_item_color(nk_rgba(0, 0, 0, 0));
        context.style.button.active = nk_style_item_color(nk_rgba(0, 0, 0, 0));
        return;
    }
    d3t::UIWindow *const window = UI_GetWindow(bar.window);
    if (!window)
        return;
    std::array<std::uint8_t, 4> channels{};
    std::memcpy(channels.data(), &window->colour, sizeof(channels));
    const std::uint32_t windowColour =
        ConstructColour(channels[0], channels[1], channels[2], static_cast<std::int32_t>(static_cast<float>(channels[3]) * window->alpha));
    nk_color tint{};
    std::memcpy(&tint, &windowColour, sizeof(windowColour));
    const auto viewImage = [&view](std::int32_t image) {
        const bool ready = static_cast<std::size_t>(image) < view.images.size() && view.images[static_cast<std::size_t>(image)].ready == 1;
        struct nk_image drawn = g_uiDefaultImage;
        if (ready)
            std::memcpy(&drawn, &view.images[static_cast<std::size_t>(image)].textureHandle, sizeof(drawn));
        return drawn;
    };
    const auto placeItem = [&context](const UILayoutRect &item) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(item.x, item.y, item.width, item.height);
    };
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    context.style.window.background = nk_rgba(0, 0, 0, 0);
    if (bar.image != -1) {
        context.style.window.background = tint;
        struct nk_image background {};
        if (static_cast<std::size_t>(bar.image) < view.images.size())
            std::memcpy(&background, &view.images[static_cast<std::size_t>(bar.image)].textureHandle, sizeof(background));
        context.style.window.fixed_background = nk_style_item_image(background);
    }
    d3t::UIWindow *const parent = UI_FindWindowById(window->layout.parentId.data());
    UILayoutRect parentRect{};
    UI_GetWindowRect(parent, parentRect);
    UILayoutRect rect{};
    UI_GetWindowRect(window, rect);
    const float top = rect.y - parentRect.y;
    const float left = rect.x - parentRect.x;
    std::array<char, 16> name{};
    std::snprintf(name.data(), name.size(), "mb%d", index);
    if (g_waitingForNextTick == 1) {
        nk_panel *const panel = reinterpret_cast<nk_panel *>(window->panel.data());
        nk_begin_titled(
            &context, panel, name.data(), name.data(), nk_rect(parentRect.x, parentRect.y, parentRect.width, parentRect.height), NK_WINDOW_NO_SCROLLBAR);
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, bar.optionCount + 3);
    const float height = rect.height;
    float iconWidth = 0.0f;
    if (bar.hasButtonIcon == 1) {
        if (bar.field1A4 == -1)
            bar.field1A4 = static_cast<std::int32_t>(g_platformIconsSheet);
        const auto drawGlyph = [&](std::int32_t action, bool atRight) {
            const std::int32_t glyph = ResolveButtonIconGlyph(action, -1, -1, 0);
            const std::uint32_t slot = static_cast<std::uint32_t>(g_uiDrawViewSlot);
            const std::int32_t found = UI_FindViewImage(slot, bar.buttonIconImage.data());
            const d3t::UIViewImage *image = found == -1 ? nullptr : &view.images[static_cast<std::size_t>(found)];
            if (!image)
                image = bar.field2D8 == 1 ? g_platformButtonIconImages[0] : g_platformButtonIconImages[2];
            const struct nk_image sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(bar.field1A4), glyph, image);
            if (sprite.handle.id == 0)
                return;
            const float cellHeight = static_cast<float>(sprite.region[3]);
            const float cellWidth = static_cast<float>(sprite.region[2]);
            iconWidth = height / cellHeight * cellWidth;
            const float half = height * 0.5f;
            UILayoutRect place{.x = atRight ? rect.width - iconWidth + left : left, .y = half - half + top, .width = 0.0f, .height = 0.0f};
            float fitHeight = 0.0f;
            float fitWidth = 0.0f;
            if (height != 0.0f && iconWidth != 0.0f && cellWidth != 0.0f && cellHeight != 0.0f) {
                if (height / iconWidth > cellHeight / cellWidth) {
                    fitHeight = iconWidth / cellWidth * cellHeight;
                    fitWidth = height / cellHeight * cellWidth;
                } else {
                    fitHeight = height;
                    fitWidth = height / cellHeight * cellWidth;
                }
                place.width = fitWidth;
                place.height = fitHeight;
            }
            place.y = place.y + half - fitHeight * 0.5f;
            place.x = place.x + iconWidth * 0.5f - fitWidth * 0.5f;
            placeItem(place);
            if (g_waitingForNextTick == 1)
                nk_image_color(&context, image && image->ready == 1 ? sprite : g_uiDefaultImage, tint);
            const UIButtonIconLabel keyLabel{.rect = place,
                .fontWidth = fitWidth,
                .fontHeight = window->layout.height.value.fraction * 1080.0f,
                .action = action,
                .doubleWidth = 0,
                .iconFontSize = bar.iconFontSize,
                .colour = 0xffffffffU,
                .alignment = 18,
                .controlMethod = -1,
                .controlState = -1};
            UI_DrawButtonIconLabel(context, keyLabel);
        };
        drawGlyph(bar.left, false);
        drawGlyph(bar.right, true);
    } else {
        if (static_cast<std::size_t>(bar.left) < view.images.size()) {
            const d3t::UIViewImage &leftImage = view.images[static_cast<std::size_t>(bar.left)];
            iconWidth = height / static_cast<float>(leftImage.height) * static_cast<float>(leftImage.width);
        }
        if (bar.left != -1) {
            placeItem({.x = left, .y = height * 0.5f - height * 0.5f, .width = iconWidth, .height = height});
            if (g_waitingForNextTick == 1)
                nk_image_color(&context, viewImage(bar.left), tint);
        }
        if (bar.right != -1) {
            placeItem({.x = rect.width - iconWidth + left, .y = height * 0.5f - height * 0.5f, .width = iconWidth, .height = height});
            if (g_waitingForNextTick == 1)
                nk_image_color(&context, viewImage(bar.right), tint);
        }
    }
    if (g_uiFontSize != bar.font && static_cast<std::uint32_t>(bar.font) < 6U) {
        g_uiFontSize = bar.font;
        UI_SelectFont();
    }
    const nk_color savedColour = context.style.text.color;
    const float textAlpha = bar.field2D8 ? window->alpha : 0.1f;
    const nk_byte textAlphaByte = static_cast<nk_byte>(static_cast<std::int32_t>(static_cast<float>(savedColour.a) * textAlpha));
    context.style.text.color = nk_rgba(savedColour.r, savedColour.g, savedColour.g, textAlphaByte);
    const float fontHeight = window->layout.height.value.fraction * 1080.0f;
    const float optionsX = iconWidth * 1.5f + left;
    const float optionWidth = (rect.width - iconWidth * 3.0f) / static_cast<float>(bar.optionCount);
    float scale = 1.0f;
    const auto labelText = [&bar](std::int32_t option) {
        SubtitleStringId id{};
        std::memcpy(&id, bar.labels[static_cast<std::size_t>(option)].key.data(), sizeof(id));
        return ReadStringFromStringTableById(id);
    };
    for (std::int32_t option = 0; option < bar.optionCount && option < 8; ++option) {
        const char *const text = labelText(option);
        if (!text)
            continue;
        const UILayoutRect item{.x = static_cast<float>(option) * optionWidth + optionsX, .y = top, .width = optionWidth * 0.9f, .height = height};
        scale = std::fminf(UI_FitLabelScale(context, text, -1, 18, item, optionWidth, fontHeight, 1).scale, scale);
    }
    const nk_color separatorColour = nk_rgba(255, 255, 255, static_cast<nk_byte>(static_cast<std::int32_t>(bar.separatorAlpha * 255.0f)));
    UILayoutRect separator{.x = optionsX, .y = 0.0f, .width = 0.0f, .height = 0.0f};
    if (bar.separatorImage != -1) {
        const float scaledHeight = parentRect.height * bar.separatorScaleY;
        const float scaledWidth = bar.separatorScaleX * rect.width;
        const bool valid = static_cast<std::size_t>(bar.separatorImage) < view.images.size();
        const float imageWidth = valid ? view.images[static_cast<std::size_t>(bar.separatorImage)].widthScale * 1920.0f : 0.0f;
        const float imageHeight = valid ? view.images[static_cast<std::size_t>(bar.separatorImage)].heightScale * 1080.0f : 0.0f;
        float fitWidth = 0.0f;
        float fitHeight = 0.0f;
        if (scaledHeight != 0.0f && scaledWidth != 0.0f && imageWidth != 0.0f && imageHeight != 0.0f) {
            if (scaledHeight / scaledWidth > imageHeight / imageWidth) {
                fitHeight = scaledWidth / imageWidth * imageHeight;
                fitWidth = scaledWidth;
            } else {
                fitHeight = scaledHeight;
                fitWidth = imageHeight / imageWidth != scaledHeight / scaledWidth ? scaledHeight / imageHeight * imageWidth : scaledWidth;
            }
        }
        separator.width = fitWidth;
        separator.height = fitHeight;
        separator.x = (parentRect.width - bar.separatorScaleX * parentRect.width) * 0.5f + (separator.x - fitWidth * 0.5f);
        separator.y = separator.y + scaledHeight * 0.5f - fitHeight * 0.5f + (parentRect.height - scaledHeight) * 0.5f;
        placeItem(separator);
        if (g_waitingForNextTick == 1)
            nk_image_color(&context, viewImage(bar.separatorImage), separatorColour);
    }
    for (std::int32_t option = 0; option < bar.optionCount && option < 8; ++option) {
        if (bar.separatorImage != -1) {
            separator.x = separator.x + optionWidth;
            placeItem(separator);
            if (g_waitingForNextTick == 1)
                nk_image_color(&context, viewImage(bar.separatorImage), separatorColour);
        }
        const bool selected = option == bar.field184;
        const float optionScale = selected ? scale : scale * 0.75f;
        const float raise = selected ? -(height * 0.08f) : height * 0.08f;
        const UILayoutRect item{.x = static_cast<float>(option) * optionWidth + optionsX + (optionWidth - optionWidth * 0.9f) * 0.5f,
            .y = raise + top,
            .width = optionWidth * 0.9f,
            .height = height};
        placeItem(item);
        if (const char *const text = labelText(option))
            UI_DrawLabelText(context, text, -1, 18, item, UILabelFit{.scale = optionScale, .split = 0});
    }
    context.style.text.color = savedColour;
    placeItem({.x = static_cast<float>(bar.field184) * optionWidth + optionsX, .y = top, .width = optionWidth, .height = height});
    const std::uint32_t selectedColour = UI_TintByWindow(*window, bar.selectedColour);
    nk_color selectedTint{};
    std::memcpy(&selectedTint, &selectedColour, sizeof(selectedColour));
    if (g_waitingForNextTick != 1)
        return;
    nk_image_color(&context, viewImage(bar.selectedImage), selectedTint);
    if (context.current && context.current->layout) {
        nk_row_layout &row = context.current->layout->row;
        row.item_width = 0.0f;
        row.item_height = 0.0f;
        row.item_offset = 0.0f;
        row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
    }
    if (context.current) {
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
}

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
