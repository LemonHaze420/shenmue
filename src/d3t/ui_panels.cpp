#include "d3t/ui_internal.hpp"

#include "d3t/ui_definition.hpp"

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif



void UI_ResetTextBlock(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiTextBlocks.size()))
        return;
    UITextBlock &block = g_uiTextBlocks[static_cast<std::size_t>(index)];
    block.inUse = 0;
    block.id[0] = 0;
    for (std::size_t line = 0; line < block.lines.size(); ++line) {
        block.lines[line] = "";
        block.cells[line] = {.field0 = -1, .field4 = -1, .field8 = 0};
    }
    block.field54C = 0;
    block.lineCount = 0;
    block.field248 = 0;
    block.columnCount = 0;
    block.field558 = 0;
    block.size = 2;
    ResetUIWindow(block.window);
    block.window = -1;
}



void UI_ResetGaugeWidget(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_fightGaugeWidgetStates.size()))
        return;
    d3t::UIGaugeState &gauge = g_fightGaugeWidgetStates[static_cast<std::size_t>(index)];
    gauge.allocated = 0;
    ResetUIWindow(gauge.panelIndex);
    gauge.panelIndex = -1;
    gauge.hidden = 0;
    gauge.transparentBackground = 0;
    gauge.backgroundImage = 0;
    gauge.colour = 0xffffffffU;
    gauge.images.fill(-1);
    gauge.count = 0;
    gauge.filledCount = 0;
    gauge.suppressEmptyOrbs = 0;
    for (std::size_t orb = 0; orb < gauge.positions.size(); ++orb) {
        gauge.positions[orb] = {0.0f, 0.0f};
        gauge.orbColours[orb] = orb < 8 ? 2 : orb < 11 ? 3 : 1;
    }
    gauge.fieldE8 = 0;
    gauge.width = 0.0f;
    gauge.height = 0.0f;
}





void UI_ResetButtonWheel(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiButtonWheels.size()))
        return;
    UIButtonWheel &wheel = g_uiButtonWheels[static_cast<std::size_t>(index)];
    if (g_uiReadStateInitialized == 1 && wheel.inUse == 1) {
        wheel.selectedImage = -1;
        wheel.unselectedImage = -1;
        for (const UIButtonWheelItem &item : wheel.items) {
            UI_ReleaseAnimSlot(item.animSlots[0]);
            UI_ReleaseAnimSlot(item.animSlots[1]);
        }
    }
    wheel.originY = 0.0f;
    wheel.originX = 0.0f;
    wheel.inUse = 0;
    wheel.count = 0;
    wheel.width = 0.0f;
    wheel.height = 0.0f;
    wheel.spreadY = 0.0f;
    wheel.spreadX = 0.0f;
    for (std::size_t at = 0; at < wheel.items.size(); ++at) {
        UIButtonWheelItem &item = wheel.items[at];
        item.field50 = "";
        item.field58 = "";
        item.animSlots = {-1, -1};
        wheel.angles[at] = 0.0f;
        item.angle = 0.0f;
        item.placeY = 0.0f;
        item.placeX = 0.0f;
        wheel.scales[at] = {0.0f, 0.0f};
        item.placeScale = 0.0f;
        item.field80 = 0;
        item.index = 0;
        item.field90 = 0;
    }
    wheel.shadowColour = 0xffffffffU;
    wheel.unselectedColour = 0xffffffffU;
    wheel.selectedImage = -1;
    wheel.unselectedImage = -1;
    wheel.selectedOffset = 0.0f;
    wheel.override = 0;
    wheel.dataBinding = -1;
    wheel.selection = 0;
    wheel.hasShadow = 0;
    wheel.shadowImage = -1;
    wheel.shadowMin = 0.0f;
    wheel.shadowMax = 0.0f;
}



void UI_ResetAreaMap(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiAreaMaps.size()))
        return;
    UIAreaMap &map = g_uiAreaMaps[static_cast<std::size_t>(index)];
    const std::int32_t window = map.window;
    map.inUse = 0;
    map.mapCount = 0;
    map.fieldC = 0;
    map.field238 = {0.0f, 0.0f};
    ResetUIWindow(window);
    map.window = -1;
    for (UIAreaMapEntry &entry : map.maps) {
        entry.inUse = 0;
        entry.count = 0;
        entry.sheetName = "";
        entry.sheet = -1;
        for (std::array<float, 2> &pair : entry.data)
            pair = {0.0f, 0.0f};
    }
    map.field1F8.fill(0);
}



void UI_ResetSelector(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiSelectors.size()))
        return;
    UISelector &selector = g_uiSelectors[static_cast<std::size_t>(index)];
    selector.inUse = 0;
    selector.id[0] = 0;
    selector.field44 = 0;
    selector.window = -1;
    selector.optionCount = 0;
    for (std::size_t option = 0; option < selector.options.size(); ++option) {
        selector.hasOption[option] = 0;
        selector.options[option].token = "";
        selector.options[option].key[0] = 0;
        selector.options[option].key[1] = 0;
        selector.options[option].key[2] = 0;
        selector.options[option].key[3] = 0;
        selector.options[option].alreadyLocalized = 0;
    }
    selector.selectedImage = -1;
    selector.unselectedImage = -1;
    selector.hasLabel = 0;
    selector.labelFont = 4;
    selector.label.token = "";
    selector.label.key[0] = 0;
    selector.label.key[1] = 0;
    selector.label.key[2] = 0;
    selector.label.key[3] = 0;
    selector.label.alreadyLocalized = 0;
}


void UI_ResetTexturePage(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiTexturePages.size()))
        return;
    UITexturePage &page = g_uiTexturePages[static_cast<std::size_t>(index)];
    page.inUse = 0;
    page.id[0] = 0;
    page.count = 0;
    page.images.fill(-1);
    page.min.fill(-1);
    page.max.fill(-1);
}



void UI_ResetImageFrame(std::int32_t index) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiImageFrames.size()))
        return;
    UIImageFrame &frame = g_uiImageFrames[static_cast<std::size_t>(index)];
    frame.inUse = 0;
    frame.hidden = 0;
    ResetUIWindow(frame.window);
    frame.window = -1;
    frame.background = -1;
}




void UI_PlaceButtonWheelItem(
    float angle, float spreadY, float spreadX, float originY, float originX, float &placeY, float &placeX, float &scale, std::int32_t override) {
    const float cosine = std::cos(angle);
    placeX = spreadX * cosine + originX;
    const float depth = cosine + 2.5f;
    placeY = originY - std::sin(angle) * (depth * spreadY);
    if (override == 1) {
        float distance = static_cast<float>(std::fabs(static_cast<double>(angle)));
        if (angle > 3.1415949f)
            distance = 6.2831898f - angle;
        scale = 1.0f - distance / 3.1415949f;
    } else {
        scale = (depth * 0.2f + 0.2f) * 1.1111112f;
    }
}








void UI_ReadButtonWheel(cJSON *buttonWheel) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiButtonWheels.size(); ++record) {
        if (!g_uiButtonWheels[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    UIButtonWheel *const wheel = index == -1 ? nullptr : &g_uiButtonWheels[static_cast<std::size_t>(index)];
    d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
    if (wheel) {
        wheel->inUse = 1;
        const cJSON *const count = cJSON_GetObjectItem(buttonWheel, "Count");
        wheel->count = count ? count->valueint : 0;
        wheel->selection = 0;
    }
    cJSON *const panel = cJSON_GetObjectItem(buttonWheel, "Panel");
    const std::int32_t window = panel ? UI_ReadWindow(panel) : -1;
    if (wheel) {
        wheel->window = window;
        const cJSON *const width = cJSON_GetObjectItem(buttonWheel, "Width");
        const cJSON *const height = cJSON_GetObjectItem(buttonWheel, "Height");
        wheel->width = static_cast<float>(static_cast<double>(width ? static_cast<float>(width->valuedouble) : 0.0f) / 1920.0);
        wheel->height = static_cast<float>(static_cast<double>(height ? static_cast<float>(height->valuedouble) : 0.0f) / 1080.0);
        if (const cJSON *const overrideMember = cJSON_GetObjectItem(buttonWheel, "Override"))
            wheel->override = overrideMember->valueint;
        cJSON *const scale = cJSON_GetObjectItem(buttonWheel, "Scale");
        cJSON *const scaleX = scale ? cJSON_GetObjectItem(scale, "X") : nullptr;
        cJSON *const scaleY = scale ? cJSON_GetObjectItem(scale, "Y") : nullptr;
        cJSON *const angles = cJSON_GetObjectItem(buttonWheel, "Angles");
        cJSON *const labels = cJSON_GetObjectItem(buttonWheel, "Labels");
        cJSON *const descriptions = cJSON_GetObjectItem(buttonWheel, "Descriptions");
        cJSON *const sizes = cJSON_GetObjectItem(buttonWheel, "Size");
        for (std::int32_t item = 0; item < wheel->count && item < static_cast<std::int32_t>(wheel->items.size()); ++item) {
            const std::size_t at = static_cast<std::size_t>(item);
            const cJSON *const x = scaleX ? cJSON_GetArrayItem(scaleX, item) : nullptr;
            const cJSON *const y = scaleY ? cJSON_GetArrayItem(scaleY, item) : nullptr;
            wheel->scales[at] = {x ? static_cast<float>(x->valuedouble) : 0.0f, y ? static_cast<float>(y->valuedouble) : 0.0f};
            const cJSON *const angle = angles ? cJSON_GetArrayItem(angles, item) : nullptr;
            wheel->angles[at] = static_cast<float>(static_cast<double>(angle ? static_cast<float>(angle->valuedouble) : 0.0f) / 57.29577951308238);
            UIButtonWheelItem &entry = wheel->items[at];
            const cJSON *const label = labels ? cJSON_GetArrayItem(labels, item) : nullptr;
#if SHENMUE_IS_S1
            entry.label.token = label ? label->valuestring : nullptr;
            UI_SetLabelTokenKey(entry.label);
            const cJSON *const description = descriptions ? cJSON_GetArrayItem(descriptions, item) : nullptr;
            entry.description.token = description ? description->valuestring : nullptr;
            UI_SetLabelTokenKey(entry.description);
#else
            const char *const labelText = label ? label->valuestring : nullptr;
            std::memcpy(entry.field48.data(), &labelText, sizeof(labelText));
            if (labelText)
                UI_SetTokenKeyFromString(entry.label, labelText);
            else
                entry.label.key.fill(0);
            const cJSON *const description = descriptions ? cJSON_GetArrayItem(descriptions, item) : nullptr;
            entry.field58 = description ? description->valuestring : nullptr;
            if (entry.field58)
                UI_SetTokenKeyFromString(entry.description, entry.field58);
            else
                entry.description.key.fill(0);
#endif
            UI_PlaceButtonWheelItem(wheel->angles[at], wheel->spreadY, wheel->spreadX, wheel->originY, wheel->originX, entry.placeY, entry.placeX,
                entry.placeScale, wheel->override);
            entry.angle = wheel->angles[at];
            entry.index = item;
            entry.index84 = item;
            const cJSON *const size = sizes ? cJSON_GetArrayItem(sizes, item) : nullptr;
            entry.size = 4;
            for (std::size_t name = 0; size && size->valuestring && name < kIconFontSizeNames.size(); ++name) {
                if (kIconFontSizeNames[name] == size->valuestring) {
                    entry.size = static_cast<std::int32_t>(name);
                    break;
                }
            }
        }
        if (const cJSON *const id = cJSON_GetObjectItem(buttonWheel, "Id"))
            std::snprintf(wheel->id.data(), wheel->id.size(), "%s", id->valuestring ? id->valuestring : "");
    }
    std::int32_t dataBinding = wheel ? wheel->dataBinding : -1;
    if (cJSON *const data = cJSON_GetObjectItem(buttonWheel, "Data"))
        dataBinding = UI_ReadDataBinding(data);
    if (wheel) {
        wheel->dataBinding = dataBinding;
        cJSON *const selected = cJSON_GetObjectItem(buttonWheel, "Selected");
        const cJSON *const selectedImage = selected ? cJSON_GetObjectItem(selected, "Image") : nullptr;
        wheel->selectedImage = d3t::FindViewImage(slot, selectedImage ? selectedImage->valuestring : nullptr);
        if (selected)
            UI_ReadColour(selected, "Colour", wheel->selectedColour);
        const cJSON *const offset = selected ? cJSON_GetObjectItem(selected, "Offset") : nullptr;
        wheel->selectedOffset = static_cast<float>(static_cast<double>(offset ? static_cast<float>(offset->valuedouble) : 0.0f) / 1080.0);
        cJSON *const unselected = cJSON_GetObjectItem(buttonWheel, "Unselected");
        const cJSON *const unselectedImage = unselected ? cJSON_GetObjectItem(unselected, "Image") : nullptr;
        wheel->unselectedImage = d3t::FindViewImage(slot, unselectedImage ? unselectedImage->valuestring : nullptr);
        if (unselected)
            UI_ReadColour(unselected, "Unselected", wheel->unselectedColour);
        if (cJSON *const shadow = cJSON_GetObjectItem(buttonWheel, "Shadow")) {
            wheel->hasShadow = 1;
            const cJSON *const shadowImage = cJSON_GetObjectItem(shadow, "Image");
            wheel->shadowImage = d3t::FindViewImage(slot, shadowImage ? shadowImage->valuestring : nullptr);
            const cJSON *const minimum = cJSON_GetObjectItem(shadow, "Min");
            wheel->shadowMin = minimum ? static_cast<float>(minimum->valuedouble) : 0.0f;
            const cJSON *const maximum = cJSON_GetObjectItem(shadow, "Max");
            wheel->shadowMax = maximum ? static_cast<float>(maximum->valuedouble) : 0.0f;
            UI_ReadColour(shadow, "Colour", wheel->shadowColour);
        }
    }
    d3t::AddViewWidget(slot, 23, index);
}






void UI_ReadAreaMap(cJSON *areaMap) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiAreaMaps.size(); ++record) {
        if (!g_uiAreaMaps[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    UIAreaMap *const map = index == -1 ? nullptr : &g_uiAreaMaps[static_cast<std::size_t>(index)];
    if (map) {
        map->inUse = 1;
        cJSON *const maps = cJSON_GetObjectItem(areaMap, "Map");
        map->mapCount = maps ? cJSON_GetArraySize(maps) : 0;
        map->fieldC = 0;
        map->field10 = 1.0f;
        map->field238 = {0.0f, 0.0f};
        for (std::int32_t at = 0; at < map->mapCount && at < static_cast<std::int32_t>(map->maps.size()); ++at) {
            cJSON *const source = cJSON_GetArrayItem(maps, at);
            UIAreaMapEntry &entry = map->maps[static_cast<std::size_t>(at)];
            entry.inUse = 1;
            const cJSON *const sheetName = source ? cJSON_GetObjectItem(source, "SpriteSheet") : nullptr;
            entry.sheetName = sheetName ? sheetName->valuestring : nullptr;
            entry.sheet = -1;
            for (std::size_t sheet = 0; entry.sheetName && sheet < g_uiSpriteSheets.size(); ++sheet) {
                const d3t::UISpriteSheet &candidate = g_uiSpriteSheets[sheet];
                if (candidate.imageName && candidate.used && !std::strcmp(candidate.imageName, entry.sheetName)) {
                    entry.sheet = static_cast<std::int32_t>(sheet);
                    break;
                }
            }
            const cJSON *const count = source ? cJSON_GetObjectItem(source, "Count") : nullptr;
            entry.count = count ? count->valueint : 0;
            cJSON *const data = source ? cJSON_GetObjectItem(source, "Data") : nullptr;
            for (std::int32_t pair = 0; pair < entry.count && pair < static_cast<std::int32_t>(entry.data.size()); ++pair) {
                cJSON *const values = data ? cJSON_GetArrayItem(data, pair) : nullptr;
                const cJSON *const first = values ? cJSON_GetArrayItem(values, 0) : nullptr;
                const cJSON *const second = values ? cJSON_GetArrayItem(values, 1) : nullptr;
                entry.data[static_cast<std::size_t>(pair)] = {
                    first ? static_cast<float>(first->valuedouble) : 0.0f, second ? static_cast<float>(second->valuedouble) : 0.0f};
            }
            const cJSON *const width = source ? cJSON_GetObjectItem(source, "Width") : nullptr;
            const cJSON *const height = source ? cJSON_GetObjectItem(source, "Height") : nullptr;
            entry.width = static_cast<float>(static_cast<double>(width ? static_cast<float>(width->valuedouble) : 0.0f) / 1920.0);
            entry.height = static_cast<float>(static_cast<double>(height ? static_cast<float>(height->valuedouble) : 0.0f) / 1080.0);
        }
    }
    cJSON *const panel = cJSON_GetObjectItem(areaMap, "Panel");
    const std::int32_t window = panel ? UI_ReadWindow(panel) : -1;
    if (map) {
        map->window = window;
        cJSON *const control = cJSON_GetObjectItem(areaMap, "Control");
        const cJSON *const scale = control ? cJSON_GetObjectItem(control, "Scale") : nullptr;
        map->field240 = 1.0f;
        map->controlScale = scale ? static_cast<float>(scale->valuedouble) : 0.0f;
        cJSON *const minimum = control ? cJSON_GetObjectItem(control, "Min") : nullptr;
        cJSON *const maximum = control ? cJSON_GetObjectItem(control, "Max") : nullptr;
        for (int axis = 0; axis < 2; ++axis) {
            const cJSON *const low = minimum ? cJSON_GetArrayItem(minimum, axis) : nullptr;
            const cJSON *const high = maximum ? cJSON_GetArrayItem(maximum, axis) : nullptr;
            map->controlMin[static_cast<std::size_t>(axis)] = low ? static_cast<float>(low->valuedouble) : 0.0f;
            map->controlMax[static_cast<std::size_t>(axis)] = high ? static_cast<float>(high->valuedouble) : 0.0f;
        }
    }
    d3t::AddViewWidget(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], 30, index);
}







void UI_ReadSelector(cJSON *selectorObject) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiSelectors.size(); ++record) {
        if (!g_uiSelectors[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
    UISelector *const selector = index == -1 ? nullptr : &g_uiSelectors[static_cast<std::size_t>(index)];
    if (selector)
        selector->inUse = 1;
    cJSON *const panel = cJSON_GetObjectItem(selectorObject, "Panel");
    const std::int32_t window = panel ? UI_ReadWindow(panel) : -1;
    if (selector) {
        selector->window = window;
        const cJSON *const image = cJSON_GetObjectItem(selectorObject, "Image");
        selector->image = d3t::FindViewImage(slot, image ? image->valuestring : "");
        selector->colour = UI_ReadColour4(cJSON_GetObjectItem(selectorObject, "Colour"));
        if (cJSON *const label = cJSON_GetObjectItem(selectorObject, "Label")) {
            selector->hasLabel = 1;
            const cJSON *const font = cJSON_GetObjectItem(label, "Font");
            selector->labelFont = 4;
            for (std::size_t name = 0; font && font->valuestring && name < kIconFontSizeNames.size(); ++name) {
                if (kIconFontSizeNames[name] == font->valuestring) {
                    selector->labelFont = static_cast<std::int32_t>(name);
                    break;
                }
            }
            const cJSON *const text = cJSON_GetObjectItem(label, "Label");
            selector->label.token = text ? text->valuestring : nullptr;
            UI_SetLabelTokenKey(selector->label);
            const cJSON *const labelImage = cJSON_GetObjectItem(label, "Image");
            selector->labelImage = d3t::FindViewImage(slot, labelImage ? labelImage->valuestring : "");
            const cJSON *const width = cJSON_GetObjectItem(label, "Width");
            const cJSON *const height = cJSON_GetObjectItem(label, "Height");
            selector->labelWidth = static_cast<float>(static_cast<double>(width ? static_cast<float>(width->valuedouble) : 0.0f) / 1920.0);
            selector->labelHeight = static_cast<float>(static_cast<double>(height ? static_cast<float>(height->valuedouble) : 0.0f) / 1080.0);
        }
        constexpr std::array<const char *, 4> kDirections{"Up", "Right", "Down", "Left"};
        cJSON *const options = cJSON_GetObjectItem(selectorObject, "Options");
        selector->optionCount = options ? cJSON_GetArraySize(options) : 0;
        for (cJSON *option = options ? options->child : nullptr; option; option = option->next) {
            for (std::size_t direction = 0; direction < kDirections.size(); ++direction) {
                if (!std::strcmp(option->string, kDirections[direction])) {
                    selector->hasOption[direction] = 1;
                    selector->options[direction].token = option->valuestring;
                    UI_SetLabelTokenKey(selector->options[direction]);
                    break;
                }
            }
        }
        const cJSON *const pos = cJSON_GetObjectItem(selectorObject, "Pos");
        selector->pos = -1;
        for (std::size_t direction = 0; pos && pos->valuestring && direction < kDirections.size(); ++direction) {
            if (!std::strcmp(pos->valuestring, kDirections[direction])) {
                selector->pos = static_cast<std::int32_t>(direction);
                break;
            }
        }
        if (cJSON *const selected = cJSON_GetObjectItem(selectorObject, "Selected")) {
            const cJSON *const selectedImage = cJSON_GetObjectItem(selected, "Image");
            selector->selectedImage = d3t::FindViewImage(slot, selectedImage ? selectedImage->valuestring : "");
            selector->selectedColour = UI_ReadColour4(cJSON_GetObjectItem(selected, "Colour"));
        }
        if (cJSON *const unselected = cJSON_GetObjectItem(selectorObject, "Unselected")) {
            const cJSON *const unselectedImage = cJSON_GetObjectItem(unselected, "Image");
            selector->unselectedImage = d3t::FindViewImage(slot, unselectedImage ? unselectedImage->valuestring : "");
            selector->unselectedColour = UI_ReadColour4(cJSON_GetObjectItem(unselected, "Colour"));
        }
        const cJSON *const width = cJSON_GetObjectItem(selectorObject, "Width");
        const cJSON *const height = cJSON_GetObjectItem(selectorObject, "Height");
        selector->width = static_cast<float>(static_cast<double>(width ? static_cast<float>(width->valuedouble) : 0.0f) / 1920.0);
        selector->height = static_cast<float>(static_cast<double>(height ? static_cast<float>(height->valuedouble) : 0.0f) / 1080.0);
        if (const cJSON *const id = cJSON_GetObjectItem(selectorObject, "Id"))
            std::snprintf(selector->id.data(), selector->id.size(), "%s", id->valuestring ? id->valuestring : "");
    }
    d3t::AddViewWidget(slot, 31, index);
}





void UI_ReadTextBlock(cJSON *textBlock) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiTextBlocks.size(); ++record) {
        if (!g_uiTextBlocks[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    UITextBlock *const block = index == -1 ? nullptr : &g_uiTextBlocks[static_cast<std::size_t>(index)];
    if (block)
        block->inUse = 1;
    if (const cJSON *const lines = cJSON_GetObjectItem(textBlock, "LineCount"); lines && block)
        block->lineCount = lines->valueint;
    if (const cJSON *const columns = cJSON_GetObjectItem(textBlock, "ColumnCount"); columns && block) {
        block->columnCount = columns->valueint;
        block->field558 = columns->valueint;
    }
    if (const cJSON *const sheetName = cJSON_GetObjectItem(textBlock, "SpriteSheet"); sheetName && block) {
        block->sheet = -1;
        for (std::size_t sheet = 0; sheetName->valuestring && sheet < g_uiSpriteSheets.size(); ++sheet) {
            const d3t::UISpriteSheet &candidate = g_uiSpriteSheets[sheet];
            if (candidate.imageName && candidate.used && !std::strcmp(candidate.imageName, sheetName->valuestring)) {
                block->sheet = static_cast<std::int32_t>(sheet);
                break;
            }
        }
    }
    if (block) {
        block->size = 2;
        if (const cJSON *const size = cJSON_GetObjectItem(textBlock, "Size")) {
            block->size = 4;
            for (std::size_t name = 0; size->valuestring && name < kIconFontSizeNames.size(); ++name) {
                if (kIconFontSizeNames[name] == size->valuestring) {
                    block->size = static_cast<std::int32_t>(name);
                    break;
                }
            }
        }
    }
    if (cJSON *const panel = cJSON_GetObjectItem(textBlock, "Panel")) {
        const std::int32_t window = UI_ReadWindow(panel);
        if (block)
            block->window = window;
    }
    if (const cJSON *const id = cJSON_GetObjectItem(textBlock, "Id"); id && block)
        std::snprintf(block->id.data(), block->id.size(), "%s", id->valuestring ? id->valuestring : "");
    d3t::AddViewWidget(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], 33, index);
}





void UI_ReadButtonPanel(cJSON *buttonPanel) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiButtonPanels.size(); ++record) {
        if (!g_uiButtonPanels[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    if (index != -1) {
        UIButtonPanel &panel = g_uiButtonPanels[static_cast<std::size_t>(index)];
        panel.inUse = 1;
        if (const cJSON *const id = cJSON_GetObjectItem(buttonPanel, "Id"); id && id->valuestring && id->valuestring[0])
            std::snprintf(panel.id.data(), panel.id.size(), "%s", id->valuestring);
        if (const cJSON *const parent = cJSON_GetObjectItem(buttonPanel, "ParentID"); parent && parent->valuestring && parent->valuestring[0])
            std::snprintf(panel.parentId.data(), panel.parentId.size(), "%s", parent->valuestring);
        if (const cJSON *const width = cJSON_GetObjectItem(buttonPanel, "ButtonWidth"))
            panel.buttonWidth = static_cast<float>(width->valuedouble);
        if (const cJSON *const height = cJSON_GetObjectItem(buttonPanel, "ButtonHeight"))
            panel.buttonHeight = static_cast<float>(height->valuedouble);
        for (UIButtonPanelEntry &entry : panel.entries) {
            entry.field0 = -1;
            entry.field4 = -1;
            entry.field8[0] = 0;
        }
        if (const cJSON *const size = cJSON_GetObjectItem(buttonPanel, "FontSize")) {
            panel.fontSize = 4;
            for (std::size_t name = 0; size->valuestring && name < kIconFontSizeNames.size(); ++name) {
                if (kIconFontSizeNames[name] == size->valuestring) {
                    panel.fontSize = static_cast<std::int32_t>(name);
                    break;
                }
            }
        }
        panel.iconFontSize = 5;
        if (const cJSON *const size = cJSON_GetObjectItem(buttonPanel, "IconFontSize")) {
            panel.iconFontSize = 4;
            for (std::size_t name = 0; size->valuestring && name < kIconFontSizeNames.size(); ++name) {
                if (kIconFontSizeNames[name] == size->valuestring) {
                    panel.iconFontSize = static_cast<std::int32_t>(name);
                    break;
                }
            }
        }
    }
    d3t::AddViewWidget(g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)], 36, index);
}







void UI_ReadGaugeWidget(cJSON *gaugeObject) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_fightGaugeWidgetStates.size(); ++record) {
        if (!g_fightGaugeWidgetStates[record].allocated)
            index = static_cast<std::int32_t>(record);
    }
    d3t::UIGaugeState *const gauge = index == -1 ? nullptr : &g_fightGaugeWidgetStates[static_cast<std::size_t>(index)];
    if (gauge)
        gauge->allocated = 1;
    cJSON *const panel = cJSON_GetObjectItem(gaugeObject, "Panel");
    const std::int32_t window = panel ? UI_ReadWindow(panel) : -1;
    d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
    if (gauge) {
        gauge->panelIndex = window;
        const d3t::UIWindow *const record = static_cast<std::uint32_t>(window) <= 0x3ffU ? &g_uiWindows[static_cast<std::size_t>(window)] : nullptr;
        const float authoredWidth = record ? static_cast<float>(static_cast<double>(record->layout.width.value.fraction) * 1920.0) : 0.0f;
        const float authoredHeight = record ? static_cast<float>(static_cast<double>(record->layout.height.value.fraction) * 1080.0) : 0.0f;
        const cJSON *const count = cJSON_GetObjectItem(gaugeObject, "Count");
        gauge->count = count ? count->valueint : 0;
        cJSON *const xs = cJSON_GetObjectItem(gaugeObject, "X");
        cJSON *const ys = cJSON_GetObjectItem(gaugeObject, "Y");
        for (std::int32_t orb = 0; orb < gauge->count && orb < static_cast<std::int32_t>(gauge->positions.size()); ++orb) {
            const cJSON *const x = xs ? cJSON_GetArrayItem(xs, orb) : nullptr;
            const cJSON *const y = ys ? cJSON_GetArrayItem(ys, orb) : nullptr;
            const float xValue = x ? static_cast<float>(x->valuedouble) : 0.0f;
            const float yValue = y ? static_cast<float>(y->valuedouble) : 0.0f;
            std::array<float, 2> &position = gauge->positions[static_cast<std::size_t>(orb)];
            position[0] = record ? xValue / authoredWidth : static_cast<float>(static_cast<double>(xValue) / 1920.0);
            position[1] = record ? yValue / authoredHeight : static_cast<float>(static_cast<double>(yValue) / 1080.0);
        }
        if (cJSON *const images = cJSON_GetObjectItem(gaugeObject, "Image"); images && cJSON_GetArraySize(images) == 9) {
            for (int image = 0; image < 9; ++image) {
                const cJSON *const name = cJSON_GetArrayItem(images, image);
                gauge->images[static_cast<std::size_t>(image)] = d3t::FindViewImage(slot, name ? name->valuestring : "");
            }
        }
        const cJSON *const width = cJSON_GetObjectItem(gaugeObject, "Width");
        const cJSON *const height = cJSON_GetObjectItem(gaugeObject, "Height");
        const float widthValue = width ? static_cast<float>(width->valuedouble) : 0.0f;
        const float heightValue = height ? static_cast<float>(height->valuedouble) : 0.0f;
        gauge->width = record ? widthValue / authoredWidth : static_cast<float>(static_cast<double>(widthValue) / 1920.0);
        gauge->height = record ? heightValue / authoredHeight : static_cast<float>(static_cast<double>(heightValue) / 1080.0);
        gauge->colour = UI_ReadColour4(cJSON_GetObjectItem(gaugeObject, "Colour"));
    }
    d3t::AddViewWidget(slot, 35, index);
}





std::int32_t UI_ReadTexturePage(cJSON *texturePage) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiTexturePages.size(); ++record) {
        if (!g_uiTexturePages[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    if (index == -1)
        return -1;
    UITexturePage &page = g_uiTexturePages[static_cast<std::size_t>(index)];
    page.inUse = 1;
    const cJSON *const count = cJSON_GetObjectItem(texturePage, "Count");
    page.count = count ? count->valueint : 0;
    cJSON *const images = cJSON_GetObjectItem(texturePage, "Images");
    cJSON *const minimums = cJSON_GetObjectItem(texturePage, "Min");
    cJSON *const maximums = cJSON_GetObjectItem(texturePage, "Max");
    const d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
    for (std::int32_t entry = 0; entry < page.count && entry < static_cast<std::int32_t>(page.images.size()); ++entry) {
        const std::size_t at = static_cast<std::size_t>(entry);
        const cJSON *const image = images ? cJSON_GetArrayItem(images, entry) : nullptr;
        page.images[at] = d3t::FindViewImage(slot, image ? image->valuestring : nullptr);
        const cJSON *const minimum = minimums ? cJSON_GetArrayItem(minimums, entry) : nullptr;
        page.min[at] = minimum ? minimum->valueint : 0;
        const cJSON *const maximum = maximums ? cJSON_GetArrayItem(maximums, entry) : nullptr;
        page.max[at] = maximum ? maximum->valueint : 0;
    }
    const cJSON *const id = cJSON_GetObjectItem(texturePage, "Id");
    std::snprintf(page.id.data(), page.id.size(), "%s", id && id->valuestring ? id->valuestring : "");
    return index;
}




void UI_ReadImageFrame(cJSON *imageFrame) {
    std::int32_t index = -1;
    for (std::size_t record = 0; index == -1 && record < g_uiImageFrames.size(); ++record) {
        if (!g_uiImageFrames[record].inUse)
            index = static_cast<std::int32_t>(record);
    }
    UIImageFrame *const frame = index == -1 ? nullptr : &g_uiImageFrames[static_cast<std::size_t>(index)];
    d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(g_currentUIViewSlot)];
    if (frame)
        frame->inUse = 1;
    if (const cJSON *const hidden = cJSON_GetObjectItem(imageFrame, "Hidden"); hidden && frame)
        frame->hidden = hidden->valueint != 0 ? 1 : 0;
    if (const cJSON *const background = cJSON_GetObjectItem(imageFrame, "Background"); background && frame)
        frame->background = d3t::FindViewImage(slot, background->valuestring);
    cJSON *const panel = cJSON_GetObjectItem(imageFrame, "Panel");
    const std::int32_t window = panel ? UI_ReadWindow(panel) : -1;
    if (frame)
        frame->window = window;
    d3t::AddViewWidget(slot, 34, index);
}











void UI_DrawButtonWheel(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) > 2U || static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU)
        return;
    UIButtonWheel &wheel = g_uiButtonWheels[static_cast<std::size_t>(index)];
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    context.style.window.background = nk_rgba(0, 0, 0, 0);
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    d3t::UIWindow *const window = UI_GetWindow(wheel.window);
    std::array<char, 32> name{};
    std::snprintf(name.data(), name.size(), "s%d,bw%d", g_uiDrawViewSlot, index);
    UILayoutRect rect{};
    UI_GetWindowRect(window, rect);
    const float spreadY = rect.width * 0.5f - (rect.width * wheel.width + rect.width * wheel.width);
    const float originY = rect.width * 0.5f + rect.x;
    const float spreadX = rect.height * 0.5f - rect.height * wheel.height * 0.5f;
    const float originX = rect.height * 0.5f + rect.y;
    if (g_waitingForNextTick == 1 && window) {
        nk_panel *const panel = reinterpret_cast<nk_panel *>(window->panel.data());
        nk_begin_titled(
            &context, panel, name.data(), name.data(), nk_rect(0.0f, 0.0f, static_cast<float>(UI_GetAspectWidth()), 1080.0f), NK_WINDOW_NO_SCROLLBAR);
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, wheel.hasShadow ? wheel.count * 2 : wheel.count);
    const std::int32_t count = wheel.count < 8 ? wheel.count : 8;
    std::array<std::int32_t, 8> order{0, 1, 2, 3, 4, 5, 6, 7};
    for (std::int32_t first = 0; first < count; ++first) {
        for (std::int32_t other = first + 1; other < count; ++other) {
            const std::int32_t a = order[static_cast<std::size_t>(first)];
            const std::int32_t b = order[static_cast<std::size_t>(other)];
            if (wheel.items[static_cast<std::size_t>(a)].placeScale > wheel.items[static_cast<std::size_t>(b)].placeScale) {
                order[static_cast<std::size_t>(first)] = b;
                order[static_cast<std::size_t>(other)] = a;
            }
        }
    }
    const nk_color savedColour = context.style.text.color;
    const auto placeItem = [&context](const UILayoutRect &item) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(item.x, item.y, item.width, item.height);
    };
    const auto viewImage = [&view](std::int32_t image) {
        struct nk_image drawn {};
        if (static_cast<std::size_t>(image) < view.images.size())
            std::memcpy(&drawn, &view.images[static_cast<std::size_t>(image)].textureHandle, sizeof(drawn));
        return drawn;
    };
    const auto readyImage = [&view, &viewImage](std::int32_t image) {
        const bool ready = static_cast<std::size_t>(image) < view.images.size() && view.images[static_cast<std::size_t>(image)].ready == 1;
        return ready ? viewImage(image) : g_uiDefaultImage;
    };
    for (std::int32_t position = 0; position < count; ++position) {
        const std::int32_t at = order[static_cast<std::size_t>(position)];
        UIButtonWheelItem &item = wheel.items[static_cast<std::size_t>(at)];
#if SHENMUE_IS_S1
        if (item.field0[1])
            continue;
#endif
        if (at == wheel.selection)
            context.style.text.color = savedColour;
        else
            std::memcpy(&context.style.text.color, &wheel.unselectedColour, sizeof(wheel.unselectedColour));
        if (item.animSlots[0] == -1)
            item.animSlots[0] = UIAnimAllocSlot();
        const float angle =
            static_cast<std::uint32_t>(item.animSlots[0]) < g_uiAnimSlots.size() ? g_uiAnimSlots[static_cast<std::size_t>(item.animSlots[0])].current[0] : 0.0f;
        item.angle = angle;
        UI_PlaceButtonWheelItem(angle, spreadY, spreadX, originY, originX, item.placeY, item.placeX, item.placeScale, wheel.override);
        float scale = item.placeScale;
        if (wheel.override)
            scale = (wheel.scales[0][0] - wheel.scales[1][0]) * scale + wheel.scales[1][0];
        const std::int32_t aspectWidth = UI_GetAspectWidth();
        const float safeHeight = UI_SafeAreaScale() * 1080.0f;
        const float area = safeHeight != 0.0f && static_cast<float>(aspectWidth) - (1080.0f - safeHeight) != 0.0f ? safeHeight : 0.0f;
        const float side = scale * wheel.height * area;
        const float x = item.placeY - side * 0.5f;
        const float y = item.placeX - side * 0.5f;
        const std::int32_t size = wheel.items[static_cast<std::size_t>(position)].size;
        if (g_uiFontSize != size && static_cast<std::uint32_t>(size) < 6U) {
            g_uiFontSize = size;
            UI_SelectFont();
        }
        context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
        if (wheel.hasShadow) {
#if SHENMUE_IS_S1
            const float shadowX = scale * 15.0f / 1080.0f * 1080.0f + x;
            const float shadowY =
                ((wheel.shadowMax - wheel.shadowMin) * (1.0f - scale) + wheel.shadowMin) / 1080.0f * 1080.0f * scale + y + scale * -8.0f / 1080.0f * 1080.0f;
#else
            const float shadowX = x;
            const float shadowY = ((wheel.shadowMax - wheel.shadowMin) * (1.0f - scale) + wheel.shadowMin) / 1080.0f * 1080.0f * scale + y;
#endif
            placeItem({.x = shadowX, .y = shadowY, .width = side, .height = side});
            if (g_waitingForNextTick == 1)
                nk_image_color(&context, readyImage(wheel.shadowImage), nk_rgba(255, 255, 255, 255));
        }
        const UILayoutRect itemRect{.x = x, .y = y, .width = side, .height = side};
        placeItem(itemRect);
        if (g_waitingForNextTick == 1)
            nk_button_image_color(&context, viewImage(wheel.unselectedImage), nk_rgba(255, 255, 255, 255));
        if (at == wheel.selection) {
            const float pulse = static_cast<std::uint32_t>(g_mainMenuPulseSlot) < g_uiAnimSlots.size() ?
                g_uiAnimSlots[static_cast<std::size_t>(g_mainMenuPulseSlot)].current[0] :
                0.0f;
            const nk_color colour = nk_rgba(255, 255, 255, static_cast<nk_byte>(static_cast<std::int32_t>(pulse * 255.0f)));
            placeItem(itemRect);
            if (g_waitingForNextTick == 1)
                nk_button_image_color(&context, viewImage(wheel.selectedImage), colour);
        }
        UI_WindowMouseCallbacks(wheel.mouseCallbacks, g_currActiveUIIndex, itemRect, &item);
        placeItem(itemRect);
        if (g_waitingForNextTick != 1)
            continue;
        nk_image_color(&context, readyImage(item.field48[0]), nk_rgba(255, 255, 255, 255));
        if (context.current && context.current->layout) {
            nk_row_layout &row = context.current->layout->row;
            row.item_width = 0.0f;
            row.item_height = 0.0f;
            row.item_offset = 0.0f;
            row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }
    if (g_waitingForNextTick == 1 && context.current) {
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }

    context.style.text.color = savedColour;
#if SHENMUE_IS_S2
    if (static_cast<std::uint32_t>(wheel.dataBinding) < g_uiDataBindings.size() && static_cast<std::uint32_t>(wheel.selection) < wheel.items.size()) {
        const std::int32_t variable = g_uiDataBindings[static_cast<std::size_t>(wheel.dataBinding)].variable;
        if (static_cast<std::uint32_t>(variable) < view.variables.size())
            g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)].variables[static_cast<std::size_t>(variable)].value.stringValue =
                wheel.items[static_cast<std::size_t>(wheel.selection)].field58;
    }
#endif
}










void UI_DrawSelector(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) >= 10U)
        return;
    const UISelector &selector = g_uiSelectors[static_cast<std::size_t>(index)];
    if (!selector.field44)
        return;
    d3t::UIWindow *const window = UI_GetWindow(selector.window);
    if (!window || static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU)
        return;
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    const auto viewImage = [&view](std::int32_t image, bool requireReady) {
        const bool valid = static_cast<std::size_t>(image) < view.images.size();
        if (requireReady && !(valid && view.images[static_cast<std::size_t>(image)].ready == 1))
            return g_uiDefaultImage;
        struct nk_image drawn {};
        if (valid)
            std::memcpy(&drawn, &view.images[static_cast<std::size_t>(image)].textureHandle, sizeof(drawn));
        return drawn;
    };
    const auto placeItem = [&context](const UILayoutRect &item) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(item.x, item.y, item.width, item.height);
    };
    const auto tokenText = [](const d3t::UILabelToken &token) {
        SubtitleStringId id{};
        std::memcpy(&id, token.key.data(), sizeof(id));
        return ReadStringFromStringTableById(id);
    };
    UILayoutRect rect{};
    UI_GetWindowRect(window, rect);
    context.style.window.background = nk_rgba(0, 0, 0, 0);
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    if (selector.image == -1) {
        nk_color colour{};
        std::memcpy(&colour, &selector.colour, sizeof(colour));
        context.style.window.fixed_background = nk_style_item_color(colour);
    } else {
        context.style.window.fixed_background = nk_style_item_image(viewImage(selector.image, false));
    }
    std::memcpy(&context.style.window.background, &selector.colour, sizeof(selector.colour));
    std::array<char, 16> name{};
    std::snprintf(name.data(), name.size(), "selector%d", index);
    if (g_waitingForNextTick == 1) {
        nk_panel *const panel = reinterpret_cast<nk_panel *>(window->panel.data());
        nk_begin_titled(&context, panel, name.data(), name.data(), nk_rect(0.0f, 0.0f, static_cast<float>(UI_GetAspectWidth()), 1080.0f),
            static_cast<nk_flags>(window->flags));
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, selector.optionCount + (selector.hasLabel != 0 ? 1 : 0));
    float labelWidth = 0.0f;
    float labelHeight = 0.0f;
    if (rect.height * 0.55f != 0.0f && rect.width != 0.0f && selector.labelWidth != 0.0f && selector.labelHeight != 0.0f) {
        labelWidth = rect.width;
        labelHeight = rect.height * 0.55f;
    }
    const float labelX = rect.width * 0.5f + rect.x - labelWidth * 0.5f;
    if (selector.hasLabel) {
        if (g_uiFontSize != selector.labelFont && static_cast<std::uint32_t>(selector.labelFont) < 6U) {
            g_uiFontSize = selector.labelFont;
            UI_SelectFont();
        }
        if (selector.labelImage != -1) {
            placeItem({.x = labelX, .y = rect.y, .width = labelWidth, .height = labelHeight});
            if (g_waitingForNextTick == 1)
                nk_image_color(&context, viewImage(selector.labelImage, true), nk_rgba(255, 255, 255, 255));
        }
        const float inset = labelHeight - labelHeight * 0.6f;
        const UILayoutRect textRect{.x = inset * 0.5f + labelX, .y = inset * 0.5f + rect.y, .width = labelWidth - inset, .height = labelHeight - inset};
        placeItem(textRect);
        const float fitWidth = selector.labelWidth * 1920.0f;
        const float fitHeight = selector.labelHeight * 1080.0f * 0.6f;
        if (selector.label.alreadyLocalized) {
            const char *line = selector.label.token;
            const char *const end = line ? line + std::strlen(line) : nullptr;
            while (line && line < end) {
                placeItem(textRect);
                const char *const newline = std::strchr(line, '\n');
                if (!newline) {
                    UI_FitAndDrawLabelText(context, line, static_cast<std::int32_t>(std::strlen(line)), 18, textRect, fitWidth, fitHeight, 1);
                    break;
                }
                const std::int32_t length = static_cast<std::int32_t>(newline - line);
                const UILabelFit fit = UI_FitLabelScale(context, line, length, 18, textRect, fitWidth, fitHeight, 1);
                UI_DrawLabelText(context, line, length, 18, textRect, fit);
                line = newline + 1;
            }
        } else if (const char *const text = tokenText(selector.label)) {
            UI_FitAndDrawLabelText(context, text, -1, 18, textRect, fitWidth, fitHeight, 1);
        }
    }
    const float optionWidth = labelWidth * 0.45f;
    const float optionHeight = rect.height * 0.45f;
    const nk_color savedColour = context.style.text.color;
    std::int32_t lastImage = 0;
    for (std::int32_t option = 0; option < 4; ++option) {
        float x = labelX;
        float y = rect.y;
        float width = 0.0f;
        float height = 0.0f;
        if (optionHeight != 0.0f && optionWidth != 0.0f && selector.width != 0.0f && selector.height != 0.0f) {
            width = optionWidth;
            height = optionHeight;
        }
        UI_AlignInArea(option == 1 ? 8 : option == 3 ? 9 : 1, 7, width, height, x, y, labelWidth, rect.height);
        std::int32_t image = selector.unselectedImage;
        std::uint32_t imageColour = selector.unselectedColour;
        std::uint32_t textColour = selector.unselectedColour;
        if (selector.pos == option) {
            image = selector.selectedImage;
            imageColour = 0xffffffffU;
            textColour = selector.selectedColour;
            x = x - width * 0.025f;
            width = width * 1.05f;
            y = y - height * 0.025f;
            height = height * 1.05f;
        }
        lastImage = image;
        if (selector.hasOption[static_cast<std::size_t>(option)] != 1)
            continue;
        const UILayoutRect optionRect{.x = x, .y = y, .width = width, .height = height};
        placeItem(optionRect);
        nk_color tint{};
        std::memcpy(&tint, &imageColour, sizeof(imageColour));
        if (g_waitingForNextTick == 1)
            nk_image_color(&context, viewImage(image, true), tint);
        const d3t::UILabelToken &token = selector.options[static_cast<std::size_t>(option)];
        const bool localized = token.alreadyLocalized == 1 && token.token;
        const float fitWidth = selector.width * 1920.0f;
        const float fitHeight = selector.height * 1080.0f;
        context.style.text.color = nk_rgba(0, 0, 0, 64);
        const UILayoutRect shadowRect{.x = x, .y = y + 5.0f, .width = width, .height = height};
        placeItem(shadowRect);
        if (localized) {
            UI_FitAndDrawLabelText(context, token.token, -1, 18, shadowRect, fitWidth, fitHeight, 1);
        } else if (const char *const text = tokenText(token)) {
            const UILabelFit fit = UI_FitLabelScale(context, text, -1, 18, shadowRect, fitWidth, fitHeight, 1);
            UI_DrawLabelText(context, text, -1, 18, shadowRect, fit);
        }
        std::memcpy(&context.style.text.color, &textColour, sizeof(textColour));
        placeItem(optionRect);
        if (localized) {
            UI_FitAndDrawLabelText(context, token.token, -1, 18, optionRect, fitWidth, fitHeight, 1);
        } else if (const char *const text = tokenText(token)) {
            const UILabelFit fit = UI_FitLabelScale(context, text, -1, 18, optionRect, fitWidth, fitHeight, 1);
            UI_DrawLabelText(context, text, -1, 18, optionRect, fit);
        }
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout) {
        nk_row_layout &row = context.current->layout->row;
        row.item_width = 0.0f;
        row.item_height = 0.0f;
        row.item_offset = 0.0f;
        std::memset(&row.item, lastImage, sizeof(row.item));
    }
    if (g_waitingForNextTick == 1 && context.current) {
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    context.style.text.color = savedColour;
}







void UI_DrawButtonPanel(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) > 0x1fU)
        return;
    const std::uint32_t savedSize = static_cast<std::uint32_t>(g_uiFontSize);
    const UIButtonPanel &panel = g_uiButtonPanels[static_cast<std::size_t>(index)];
    if (g_uiFontSize != panel.fontSize && static_cast<std::uint32_t>(panel.fontSize) < 6U) {
        g_uiFontSize = panel.fontSize;
        UI_SelectFont();
    }
    if (d3t::UIWindow *const window = UI_FindWindowById(panel.parentId.data())) {
        UILayoutRect rect{};
        UI_GetWindowRect(window, rect);
        std::int32_t used = 0;
        for (const UIButtonPanelEntry &entry : panel.entries)
            used += entry.field0 != -1 ? 1 : 0;
        const float rows = static_cast<float>(used);
        const float panelHeight = rows * panel.buttonHeight;
        float width = 0.0f;
        float height = 0.0f;
        if (rect.height != 0.0f && rect.width != 0.0f && panel.buttonWidth != 0.0f && panelHeight != 0.0f) {
            if (rect.height / rect.width > panelHeight / panel.buttonWidth) {
                height = rect.width / panel.buttonWidth * panelHeight;
                width = rect.width;
            } else {
                height = rect.height;
                width = panelHeight / panel.buttonWidth > rect.height / rect.width ? rect.height / panelHeight * panel.buttonWidth : rect.width;
            }
        }
        std::array<char, 24> name{};
        std::snprintf(name.data(), name.size(), "buttonPanel%d", index);
        if (g_waitingForNextTick == 1) {
            nk_panel *const nkPanel = reinterpret_cast<nk_panel *>(window->panel.data());
            const struct nk_rect bounds = nk_rect(rect.width * 0.5f + rect.x - width * 0.5f, rect.height * 0.5f + rect.y - height * 0.5f, width, height);
            nk_begin_titled(&context, nkPanel, name.data(), name.data(), bounds, static_cast<nk_flags>(window->flags));
            if (context.current && context.current->layout)
                nk_layout_space_begin(&context, NK_STATIC, 0.0f, 0);
        }
        const auto placeItem = [&context](const UILayoutRect &item) {
            if (g_waitingForNextTick == 1 && context.current && context.current->layout)
                context.current->layout->row.item = nk_rect(item.x, item.y, item.width, item.height);
        };
        const auto glyphImage = [](const UIButtonPanelEntry &entry) {
            const std::int32_t glyph = ResolveButtonIconGlyph(entry.field0, -1, entry.field4, 1);
            return UI_GetSpriteImage(static_cast<std::uint32_t>(g_platformIconsSheet), glyph, g_platformButtonIconImages[0]);
        };
        float scale = 1.0f;
        std::int32_t row = 0;
        for (const UIButtonPanelEntry &entry : panel.entries) {
            if (entry.field0 == -1)
                continue;
            const struct nk_image sprite = glyphImage(entry);
            const float rowHeight = height / rows;
            const float cellHeight = static_cast<float>(sprite.region[3]);
            const float cellWidth = static_cast<float>(sprite.region[2]);
            float iconHeight = 0.0f;
            if (rowHeight != 0.0f && width != 0.0f && cellWidth != 0.0f && cellHeight != 0.0f)
                iconHeight = rowHeight / width > cellHeight / cellWidth ? width / cellWidth * cellHeight : rowHeight;
            const UILayoutRect textRect{.x = rowHeight + rowHeight,
                .y = rowHeight * 0.5f + static_cast<float>(row) * rowHeight - iconHeight * 0.5f,
                .width = width - (rowHeight + rowHeight),
                .height = rowHeight};
            ++row;
            const UILabelFit fit = UI_FitLabelScale(context, entry.field8.data(), -1, 17, textRect, panel.buttonWidth, panel.buttonHeight, 1);
            scale = std::fminf(fit.scale, scale);
        }
        row = 0;
        for (const UIButtonPanelEntry &entry : panel.entries) {
            if (entry.field0 == -1)
                continue;
            const struct nk_image sprite = glyphImage(entry);
            const float rowHeight = height / rows;
            const float cellWidth = static_cast<float>(sprite.region[2]);
            const float cellHeight = static_cast<float>(sprite.region[3]);
            float iconWidth = 0.0f;
            float iconHeight = 0.0f;
            if (rowHeight != 0.0f && width != 0.0f && cellWidth != 0.0f && cellHeight != 0.0f) {
                if (rowHeight / width > cellHeight / cellWidth) {
                    iconHeight = width / cellWidth * cellHeight;
                    iconWidth = width;
                } else {
                    iconHeight = rowHeight;
                    iconWidth = cellHeight / cellWidth > rowHeight / width ? rowHeight / cellHeight * cellWidth : width;
                }
            }
            const float iconY = rowHeight * 0.5f + static_cast<float>(row) * rowHeight - iconHeight * 0.5f;
            const UILayoutRect iconRect{.x = (rowHeight + rowHeight) * 0.5f + 0.0f - iconWidth * 0.5f, .y = iconY, .width = iconWidth, .height = iconHeight};
            placeItem(iconRect);
            const d3t::UIViewImage *const image = g_platformButtonIconImages[0];
            if (g_waitingForNextTick == 1)
                nk_image_color(&context, image && image->ready == 1 ? sprite : g_uiDefaultImage, nk_rgba(255, 255, 255, 255));
            const UIButtonIconLabel keyLabel{.rect = iconRect,
                .fontWidth = iconWidth,
                .fontHeight = panel.buttonHeight,
                .action = entry.field0,
                .doubleWidth = 1,
                .iconFontSize = panel.iconFontSize,
                .colour = 0xffffffffU,
                .alignment = 18,
                .controlMethod = -1,
                .controlState = entry.field4};
            UI_DrawButtonIconLabel(context, keyLabel);
            const UILayoutRect textRect{.x = rowHeight + rowHeight, .y = iconY, .width = width - (rowHeight + rowHeight), .height = rowHeight};
            placeItem(textRect);
            UI_DrawLabelText(context, entry.field8.data(), -1, 17, textRect, UILabelFit{.scale = scale, .split = 0});
            ++row;
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
    }
    if (static_cast<std::uint32_t>(g_uiFontSize) != savedSize && savedSize < 6U) {
        g_uiFontSize = static_cast<std::int32_t>(savedSize);
        UI_SelectFont();
    }
}


std::int32_t UI_GetActionForId(std::int32_t id) {
    return static_cast<std::uint32_t>(id) < kTextBlockCellActions.size() ? kTextBlockCellActions[static_cast<std::size_t>(id)] : -1;
}












void UI_DrawTextBlock(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) > 2U)
        return;
    const UITextBlock &block = g_uiTextBlocks[static_cast<std::size_t>(index)];
    d3t::UIWindow *const window = UI_GetWindow(block.window);
    if (!window || static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU)
        return;
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    UILayoutRect rect{};
    UI_GetWindowRect(window, rect);
    float columnWidth = rect.width * 0.9f;
    const float rowHeight = rect.height / static_cast<float>(block.lineCount);
    if (block.field558 > 1)
        columnWidth = columnWidth / static_cast<float>(block.field558);
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    if (g_uiFontSize != block.size && static_cast<std::uint32_t>(block.size) < 6U) {
        g_uiFontSize = block.size;
        UI_SelectFont();
    }
    std::array<char, 16> name{};
    std::snprintf(name.data(), name.size(), "textBlock%d", index);
    if (g_waitingForNextTick == 1) {
        nk_panel *const panel = reinterpret_cast<nk_panel *>(window->panel.data());
        nk_begin_titled(&context, panel, name.data(), name.data(), nk_rect(rect.x, rect.y, rect.width, rect.height), static_cast<nk_flags>(window->flags));
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, block.field248 + 10);
    const auto placeItem = [&context](const UILayoutRect &item) {
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(item.x, item.y, item.width, item.height);
    };
    const nk_color savedColour = context.style.text.color;
    bool iconAtStart = false;
    float iconColumn = 10000.0f;
    context.style.text.color = nk_rgba(255, 255, 255, 255);
    std::int32_t cell = 0;
    for (std::int32_t line = 0; line < block.field248 && line < static_cast<std::int32_t>(block.lines.size()); ++line) {
        const char *const text = block.lines[static_cast<std::size_t>(line)];
        const std::int32_t length = text ? static_cast<std::int32_t>(std::strlen(text)) : 0;
        std::array<char, 256> pending{};
        std::int32_t pendingLength = 0;
        std::int32_t written = 0;
        float x = 0.0f;
        float columnX = 0.0f;
        std::int32_t column = 0;
        const float y = static_cast<float>(line) * rowHeight;
        const auto flushText = [&]() {
            const UILayoutRect piece{.x = x + columnX, .y = y, .width = columnWidth, .height = rowHeight};
            placeItem(piece);
            const UILabelFit fit = UI_FitLabelScale(context, pending.data(), -1, 17, piece, columnWidth, rowHeight, 0);
            UI_DrawLabelText(context, pending.data(), -1, 17, piece, fit);
        };
        for (std::int32_t at = 0; at < length; ++at) {
            const char c = text[at];
            if (c == '#') {
                const char code = text[++at];
                if (code == 'G')
                    context.style.text.color = nk_rgba(170, 220, 90, 255);
                else if (code == 'W')
                    context.style.text.color = nk_rgba(255, 255, 255, 255);
                continue;
            }
            char first = 0;
            if (c == '\t' || !std::strncmp(text + at, "\\t", 2)) {
                if (pendingLength > 0) {
                    pending[static_cast<std::size_t>(written)] = 0;
                    if (!std::strncmp(pending.data(), "\\t", 2)) {
                        pending[0] = ' ';
                        pending[1] = ' ';
                    } else if (pending[0] == '\t') {
                        pending[0] = ' ';
                    }
                    flushText();
                }
                const float previous = static_cast<float>(column);
                pendingLength = 0;
                pending[0] = 0;
                written = 0;
                ++column;
                x = 0.0f;
                columnX = previous * columnWidth;
            } else {
                first = pending[0];
            }
            if (c != '^') {
                if ((!iconAtStart || pendingLength > 0 || c != ' ') && written < 255) {
                    ++pendingLength;
                    pending[static_cast<std::size_t>(written++)] = c;
                }
                continue;
            }
            if (pendingLength == 2) {
                if (first == '\\' && pending[1] == 't') {
                    x = 0.0f;
                    pendingLength = 0;
                } else {
                    flushText();
                }
            } else if (pendingLength > 0) {
                flushText();
            }
            const nk_user_font *const font = context.style.font;
            x = x + font->width(font->userdata, font->height, pending.data(), pendingLength);
            if (static_cast<std::uint32_t>(block.sheet) <= 0x37U && static_cast<std::size_t>(cell) < block.cells.size()) {
                const std::int32_t id = block.cells[static_cast<std::size_t>(cell)].field0;
                const std::int32_t action = UI_GetActionForId(id);
                if (action == -1) {
                    placeItem({.x = x + columnX, .y = y, .width = rowHeight + rowHeight, .height = rowHeight});
                    x = x + (rowHeight + rowHeight);
                    const struct nk_image sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(block.sheet), id, nullptr);
                    const std::int32_t image = g_uiSpriteSheets[static_cast<std::size_t>(block.sheet)].image;
                    const bool ready = static_cast<std::size_t>(image) < view.images.size() && view.images[static_cast<std::size_t>(image)].ready == 1;
                    if (g_waitingForNextTick == 1)
                        nk_image_color(&context, ready ? sprite : g_uiDefaultImage, nk_rgba(255, 255, 255, 255));
                } else {
                    const d3t::UIViewImage *const image = g_platformButtonIconImages[0];
                    const std::int32_t glyph = ResolveButtonIconGlyph(action, -1, -1, 1);
                    const struct nk_image sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(g_platformIconsSheet), glyph, image);
                    float iconX = x + columnX;
                    float iconY = y;
                    float iconWidth = 0.0f;
                    float iconHeight = 0.0f;
                    const float cellWidth = static_cast<float>(sprite.region[2]);
                    const float cellHeight = static_cast<float>(sprite.region[3]);
                    if (rowHeight != 0.0f && iconColumn != 0.0f && cellWidth != 0.0f && cellHeight != 0.0f) {
                        if (rowHeight / iconColumn > cellHeight / cellWidth) {
                            iconHeight = iconColumn / cellWidth * cellHeight;
                            iconWidth = iconColumn;
                        } else {
                            iconHeight = rowHeight;
                            iconWidth = cellHeight / cellWidth > rowHeight / iconColumn ? rowHeight / cellHeight * cellWidth : iconColumn;
                        }
                    }
                    if (x == 0.0f) {
                        if (iconWidth > iconColumn + 10.0f || iconColumn == 10000.0f)
                            iconColumn = iconWidth + 10.0f;
                        x = x + iconColumn;
                        iconAtStart = true;
                        iconX = iconX + iconColumn * 0.5f - iconWidth * 0.5f;
                        iconY = iconY + rowHeight * 0.5f - iconHeight * 0.5f;
                    } else {
                        x = x + iconWidth;
                        iconAtStart = false;
                    }
                    const float drawWidth = iconWidth * 0.9f;
                    const float drawHeight = iconHeight * 0.9f;
                    const UILayoutRect iconRect{
                        .x = iconX + (iconWidth - drawWidth) * 0.5f, .y = iconY + (iconHeight - drawHeight) * 0.5f, .width = drawWidth, .height = drawHeight};
                    placeItem(iconRect);
                    if (g_waitingForNextTick == 1)
                        nk_image_color(&context, image && image->ready == 1 ? sprite : g_uiDefaultImage, nk_rgba(255, 255, 255, 255));
                    const UIButtonIconLabel keyLabel{.rect = iconRect,
                        .fontWidth = drawWidth,
                        .fontHeight = window->layout.height.value.fraction * 1080.0f,
                        .action = action,
                        .doubleWidth = 1,
                        .iconFontSize = 5,
                        .colour = 0xffffffffU,
                        .alignment = 18,
                        .controlMethod = -1,
                        .controlState = -1};
                    UI_DrawButtonIconLabel(context, keyLabel);
                }
            }
            ++cell;
            pendingLength = 0;
            written = 0;
            pending[0] = 0;
        }
        pending[static_cast<std::size_t>(written)] = 0;
        flushText();
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout) {
        nk_row_layout &row = context.current->layout->row;
        row.item_width = 0.0f;
        row.item_height = 0.0f;
        row.item_offset = 0.0f;
        row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
    }
    if (g_waitingForNextTick == 1 && context.current) {
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    context.style.text.color = savedColour;
}








void UI_DrawAreaMap(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU || index != 0)
        return;
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    const UIAreaMap &map = g_uiAreaMaps[0];
    d3t::UIWindow *const window = UI_GetWindow(map.window);
    if (!window)
        return;
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    context.style.window.background = nk_rgba(0, 0, 0, 0);
    UILayoutRect rect{};
    UI_GetWindowRect(window, rect);
    if (g_waitingForNextTick == 1) {
        nk_panel *const panel = reinterpret_cast<nk_panel *>(window->panel.data());
        nk_begin_titled(&context, panel, "areamap0", "areamap0", nk_rect(rect.x, rect.y, rect.width, rect.height), static_cast<nk_flags>(window->flags));
    }
    if (context.current && context.current->layout)
        context.current->layout->row.height = 0.0f;
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, map.mapCount);
    const nk_color savedColour = context.style.text.color;
    std::array<std::uint8_t, 4> channels{};
    std::memcpy(channels.data(), &window->colour, sizeof(channels));
    const std::uint32_t windowColour =
        ConstructColour(channels[0], channels[1], channels[2], static_cast<std::int32_t>(static_cast<float>(channels[3]) * window->alpha));
    std::memcpy(&context.style.text.color, &windowColour, sizeof(windowColour));
    const float zoom = map.field10;
    const UIAreaMapEntry &entry = map.maps[static_cast<std::uint32_t>(map.fieldC) < map.maps.size() ? static_cast<std::size_t>(map.fieldC) : 0U];
    const std::int32_t count = static_cast<std::uint32_t>(map.fieldC) < map.maps.size() ? entry.count : 0;
    for (std::int32_t at = 0; at < count && at < static_cast<std::int32_t>(entry.data.size()); ++at) {
        const std::array<float, 2> &position = entry.data[static_cast<std::size_t>(at)];
        const float x = 272.0f - (zoom - 1.0f) * 64.0f + map.field238[0] + zoom * position[0];
        const float y = 128.0f - (zoom - 1.0f) * 128.0f + map.field238[1] + zoom * position[1];
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(x, y, entry.width * 1920.0f * zoom, entry.height * 1080.0f * zoom);
        if (map.field1F8[static_cast<std::size_t>(at)] != 1 || static_cast<std::uint32_t>(entry.sheet) > 0x37U)
            continue;
        const struct nk_image sprite = UI_GetSpriteImage(static_cast<std::uint32_t>(entry.sheet), at, nullptr);
        const std::int32_t image = g_uiSpriteSheets[static_cast<std::size_t>(entry.sheet)].image;
        const bool ready = static_cast<std::size_t>(image) < view.images.size() && view.images[static_cast<std::size_t>(image)].ready == 1;
        nk_color tint{};
        std::memcpy(&tint, &windowColour, sizeof(windowColour));
        if (g_waitingForNextTick == 1)
            nk_image_color(&context, ready ? sprite : g_uiDefaultImage, tint);
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout) {
        nk_row_layout &row = context.current->layout->row;
        row.item_offset = 0.0f;
        row.item_width = 0.0f;
        row.item_height = 0.0f;
        row.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
    }
    if (g_waitingForNextTick == 1 && context.current) {
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    context.style.text.color = savedColour;
}






void UI_DrawImageFrame(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) > 2U)
        return;
    const UIImageFrame &frame = g_uiImageFrames[static_cast<std::size_t>(index)];
    if (frame.hidden == 1)
        return;
    d3t::UIWindow *const window = UI_GetWindow(frame.window);
    if (!window)
        return;
    const float aspect = static_cast<float>(UI_GetAspectWidth());
    d3t::UIWindow *const parent = UI_FindWindowById(window->layout.parentId.data());
    struct nk_image image {};
    std::memcpy(&image, frame.image.data(), sizeof(image));
    const float imageHeight = static_cast<float>(image.h);
    const float imageWidth = static_cast<float>(image.w);
    float width = 0.0f;
    float height = 0.0f;
    if (aspect != 0.0f && imageWidth != 0.0f && imageHeight != 0.0f) {
        if (1080.0f / aspect > imageHeight / imageWidth) {
            height = aspect / imageWidth * imageHeight;
            width = aspect;
        } else {
            height = 1080.0f;
            width = imageHeight / imageWidth > 1080.0f / aspect ? 1080.0f / imageHeight * imageWidth : aspect;
        }
    }
    float x = 0.0f;
    float y = 0.0f;
    UI_WindowPosition(*window, width, height, x, y, parent);
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 255));
    std::array<char, 16> name{};
    std::snprintf(name.data(), name.size(), "imgFrame%d", index);
    if (g_waitingForNextTick == 1) {
        nk_panel *const panel = reinterpret_cast<nk_panel *>(window->panel.data());
        nk_begin_titled(&context, panel, name.data(), name.data(), nk_rect(0.0f, 0.0f, aspect, 1080.0f), static_cast<nk_flags>(window->flags));
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, 1);
    if (g_waitingForNextTick == 1) {
        if (context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(x, y, width, height);
        nk_color colour{};
        std::memcpy(&colour, &window->colour, sizeof(colour));
        nk_image_color(&context, image, colour);
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








void UI_DrawGauge(nk_context &context, std::int32_t index) {
    if (static_cast<std::uint32_t>(index) >= 5U)
        return;
    const d3t::UIGaugeState &gauge = g_fightGaugeWidgetStates[static_cast<std::size_t>(index)];
    if (gauge.hidden == 1)
        return;
    d3t::UIWindow *const window = UI_GetWindow(gauge.panelIndex);
    if (static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU || !window)
        return;
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    UILayoutRect rect{};
    UI_GetWindowRect(window, rect);
    if (gauge.transparentBackground) {
        context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    } else {
        std::memcpy(&context.style.window.background, &gauge.colour, sizeof(gauge.colour));
        struct nk_image background {};
        if (static_cast<std::size_t>(gauge.backgroundImage) < view.images.size())
            std::memcpy(&background, &view.images[static_cast<std::size_t>(gauge.backgroundImage)].textureHandle, sizeof(background));
        context.style.window.fixed_background = nk_style_item_image(background);
    }
    std::array<char, 16> name{};
    std::snprintf(name.data(), name.size(), "gauge%d", index);
    const float width = rect.width;
    const float height = rect.height;
    if (g_waitingForNextTick == 1) {
        nk_panel *const panel = reinterpret_cast<nk_panel *>(window->panel.data());
        nk_begin_titled(&context, panel, name.data(), name.data(), nk_rect(rect.x, rect.y, width, height), static_cast<nk_flags>(window->flags));
    }
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        nk_layout_space_begin(&context, NK_STATIC, 0.0f, gauge.count);
    const auto placeOrb = [&context, &gauge, width, height](std::int32_t orb) {
        const std::array<float, 2> &position = gauge.positions[static_cast<std::size_t>(orb)];
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            context.current->layout->row.item = nk_rect(width * position[0], height * position[1], width * gauge.width, height * gauge.height);
    };
    const auto drawImage = [&context, &view](std::int32_t image, nk_color colour) {
        struct nk_image drawn = g_uiDefaultImage;
        if (static_cast<std::size_t>(image) < view.images.size() && view.images[static_cast<std::size_t>(image)].ready == 1)
            std::memcpy(&drawn, &view.images[static_cast<std::size_t>(image)].textureHandle, sizeof(drawn));
        if (g_waitingForNextTick == 1)
            nk_image_color(&context, drawn, colour);
    };
    nk_color colour = nk_rgba(255, 255, 255, 255);
    for (std::int32_t orb = 0; orb < gauge.filledCount; ++orb) {
        placeOrb(orb);
        if (gauge.translucentLastOrb == 1 && orb == gauge.filledCount - 1) {
            drawImage(gauge.images[8], nk_rgba(255, 255, 255, 255));
            placeOrb(orb);
            colour.a = static_cast<nk_byte>(static_cast<std::int32_t>(gauge.lastOrbAlpha));
        }
        drawImage(gauge.images[static_cast<std::size_t>(gauge.orbColours[static_cast<std::size_t>(orb)])], colour);
    }
    if (!gauge.suppressEmptyOrbs) {
        for (std::int32_t orb = gauge.filledCount; orb < gauge.count; ++orb) {
            placeOrb(orb);
            drawImage(gauge.images[8], nk_rgba(255, 255, 255, 255));
        }
    }
    if (g_waitingForNextTick != 1)
        return;
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
