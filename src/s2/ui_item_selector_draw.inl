Vector2 itemSelectorWindowPosition{};

std::array<LegacyIconSprite, 8> selectorNumberSprites{};
LegacyIconSprite selectorCurrencySprite{};
LegacyIconSprite selectorDateDigit = SELECTOR_DATE_DIGIT_INITIAL;
LegacyIconSprite selectorDateWeekday = SELECTOR_DATE_WEEKDAY_INITIAL;
LegacyIconSprite itemSelectorInventorySprite{};
LegacyIconSprite itemSelectorHeadingSprite{};
float itemSelectorCounterX{}, itemSelectorCounterY{};

std::int32_t GetSelectorNumberDigits(std::int32_t value) {
    if (value <= 0)
        return -1;
    for (std::int32_t index = 0; index < 8; ++index) {
        if (value >= moneyIconIncrements[index])
            return 8 - index;
    }
    return -1;
}

void DrawSelectorCash(const float *position, std::int32_t value, std::uint8_t currency, std::uint32_t colour, std::uint8_t fullPrefix, ScreenMode mode) {
    if (colour == 0)
        colour = 0xff656565U;
    selectorCurrencySprite.depth = position[2];
    const std::int32_t maximum = currency ? 999999 : 99999999;
    if (value > maximum)
        value = maximum;
    float numberX = position[0];
    if (currency) {
        const std::int32_t digits = GetSelectorNumberDigits(value);
        selectorCurrencySprite.height = static_cast<std::int16_t>(TruncateSpriteCoordinate(position[3] * 16.0f));
        selectorCurrencySprite.width = selectorCurrencySprite.height;
        const float advance = static_cast<float>(selectorCurrencySprite.height);
        const float right = position[0] + advance;
        selectorCurrencySprite.x = right - static_cast<float>(digits > 1 ? digits : 1) * advance;
        numberX = right + advance * 0.5f;
        selectorCurrencySprite.y = position[1];
        for (std::int32_t index = 0; index < 2 * (fullPrefix & 1U) + 1; ++index) {
            selectorCurrencySprite.icon = SELECTOR_CURRENCY_ICONS[index];
            PrepareDrawSimple(selectorCurrencySprite, colour, mode);
            selectorCurrencySprite.x -= advance;
        }
    }
    DrawMoneyDigits(numberX, position[1], position[3], position[2], value, colour, mode);
}

void DrawSelectorDate(const Vector2 &position, float depth, const DateTime &date) {
    const ScreenMode mode = static_cast<ScreenMode>(0);
    selectorDateDigit.depth = selectorDateWeekday.depth = depth;
    selectorDateDigit.y = selectorDateWeekday.y = position.y;
    float x = position.x;
    const std::int32_t month = static_cast<std::int8_t>(date.month);
    const std::int32_t day = static_cast<std::int8_t>(date.day);
    if (month / 10 != 0) {
        selectorDateDigit.x = x;
        selectorDateDigit.icon = SELECTOR_DIGIT_ICONS[month / 10];
        PrepareDrawSimple(selectorDateDigit, 0xff656565U, mode);
    }
    x += selectorDateDigit.width;
    selectorDateDigit.x = x;
    selectorDateDigit.icon = SELECTOR_DIGIT_ICONS[month % 10];
    PrepareDrawSimple(selectorDateDigit, 0xff656565U, mode);
    x += selectorDateDigit.width;
    selectorDateDigit.x = x;
    selectorDateDigit.icon = 1050;
    PrepareDrawSimple(selectorDateDigit, 0xff656565U, mode);
    x += selectorDateDigit.width;
    if (day / 10 != 0) {
        selectorDateDigit.x = x;
        selectorDateDigit.icon = SELECTOR_DIGIT_ICONS[day / 10];
        PrepareDrawSimple(selectorDateDigit, 0xff656565U, mode);
        x += selectorDateDigit.width;
    }
    selectorDateDigit.x = x;
    selectorDateDigit.icon = SELECTOR_DIGIT_ICONS[day % 10];
    PrepareDrawSimple(selectorDateDigit, 0xff656565U, mode);
    selectorDateWeekday.icon = SELECTOR_WEEKDAY_ICONS[static_cast<std::int8_t>(date.week)];
    selectorDateWeekday.x = (x + selectorDateDigit.width) + selectorDateDigit.width * 0.5f;
    PrepareDrawSimple(selectorDateWeekday, 0xff656565U, mode);
}

void DrawItemSelectorInventory(
    std::int32_t columns, std::int32_t maximum, float x, float y, float depth, std::int32_t first, std::uint32_t bank, ScreenMode mode) {
    LegacyIconSprite &sprite = itemSelectorInventorySprite;
    sprite.depth = depth;
    sprite.x = x;
    sprite.y = y;
    const std::int32_t count = static_cast<std::int32_t>(GetMaxItemsForItemType(bank));
    std::int32_t column = 0, drawn = 0;
    for (std::int32_t item = first; item < count; ++item) {
        if (!(itemSelectorOwnedMask[item / 8] & (1U << (item & 7))))
            continue;
        sprite.icon = GetInventoryItemIcon(bank, item);
        PrepareDrawWithMode(sprite, mode);
        sprite.x += 40.0f;
        if (++column >= columns) {
            column = 0;
            sprite.x = x;
            sprite.y += 40.0f;
        }
        if (++drawn >= maximum)
            break;
    }
}

void DrawItemSelectorLowerActions(const Vector2 &position, float depth) {
    LegacyIconSprite sprite = itemSelectorInventorySprite;
    sprite.depth = depth;
    for (std::int32_t row = 0; row < 2; ++row) {
        sprite.x = position.x;
        sprite.y = static_cast<float>(row) * 48.0f + position.y;
        for (std::int32_t index = 0; index < ITEM_SELECTOR_LOWER_COUNTS[row]; ++index) {
            sprite.icon = ITEM_SELECTOR_LOWER_ICONS[row][index];
            PrepareDrawWithMode(sprite, static_cast<ScreenMode>(0));
            sprite.x += 40.0f;
        }
    }
}

void InitializeItemSelectorWindow() {
    itemEffectPosition = ITEM_EFFECT_POSITION_INITIAL;
    itemEffectLabelTexture = -1;
    itemSelectorWindowPosition = {-672.0f, 55.0f};
    itemSelectorLabelTexture = -1;
    InitializeLegacyItemWindow();
    InitializeLegacyIconSprite(itemEffectSprite);
    itemEffectSprite.depth = 0.5f;
    InitializeLegacyIconSprite(itemSelectorInventorySprite);
    itemSelectorInventorySprite.depth = 0.5f;
    InitializeLegacyIconSprite(itemSelectorHeadingSprite);
    itemSelectorHeadingSprite.depth = 0.5f;
    itemSelectorHeadingSprite.width = 80;
    itemSelectorHeadingSprite.height = 40;
    itemSelectorHeadingSprite.icon = 1544;
}

void RenderLegacyItemSelector(void *parameter) {
    if (g_debugImGuiRenderMode == 0 && GetDebugForceUIEnabled(UIElement::GameMenu)) {
        EnableElement(UIElement::GameMenu);
        return;
    }
    ItemSelectorSelection &selection = *static_cast<ItemSelectorSelection *>(parameter);
    const ScreenMode mode = static_cast<ScreenMode>(0);
    const float leftEdgeX = itemSelectorWindowPosition.x + 64.0f;
    const float x = leftEdgeX + 11.0f;
    const float y = itemSelectorWindowPosition.y - 20.0f;
    const float width = static_cast<float>(40 * selection.columns);
    const float height = static_cast<float>(40 * selection.visibleRows);
    const float gridX = x + 24.0f, gridY = y + 24.0f;
    DrawItemSelectorInventory(selection.columns, 384, gridX, gridY, 0.5f, 0, 0, mode);
    const float lowerY = y + height + 32.0f;
    const bool inventory = selection.activeMenu == 9 && selection.count > 0;
    DrawLegacySelectionGlow(gridX + static_cast<float>(40 * (inventory ? selection.column : selection.activeMenu)),
        inventory ? gridY + static_cast<float>(40 * selection.row) : lowerY + static_cast<float>(48 * selection.lowerRow), 0.8f, selection.glowColour,
        selection.glowBrightening, mode);
    DrawItemSelectorLowerActions({.x = gridX, .y = lowerY}, 0.5f);
    DrawLegacyWindowTiles(gridX, lowerY, selection.columns, 1, ITEM_SELECTOR_TILE_DEPTH, 1024, mode);
    DrawLegacyWindowTiles(gridX, lowerY + 48.0f, selection.columns, 1, ITEM_SELECTOR_TILE_DEPTH, 1024, mode);
    itemSelectorCounterY = y + height + 26.0f;
    itemSelectorCounterX = gridX + 160.0f;
    DrawLegacyWindowBackground(x + 4.0f, y + 4.0f, width, height + 192.0f, 0.3f, 1024, mode);
    DrawLegacyWindowEdges(x, y + 27.0f, selection.visibleRows + 4, selection.columns, 0.5f, 1024, mode);
    DrawLegacyWindowTiles(gridX, gridY, selection.columns, selection.visibleRows, ITEM_SELECTOR_TILE_DEPTH, 1024, mode);
    DrawLegacyWindowHorizontal(leftEdgeX, y, selection.columns, 1, 0, 0.5f, 1024, mode);
    float edgeY = y + (height + 8.0f);
    for (std::int32_t index = 0; index < 4; ++index, edgeY += 48.0f)
        DrawLegacyWindowHorizontal(leftEdgeX, edgeY, selection.columns, 0, 0, 0.5f, 1024, mode);
    DrawLegacyWindowHorizontal(leftEdgeX, edgeY, selection.columns, 1, 1, 0.5f, 1024, mode);
    const float labelY = edgeY - 24.0f;
    float labelX = x + width * 0.5f;
    if (selection.activeMenu == 9 && selection.selectedIndex >= 0) {
        const std::span<const std::byte> stock = S2Save_GetInventoryBank(S2InventoryBank::Type0);
        const std::uint8_t quantity = std::to_integer<std::uint8_t>(stock[selection.selectedIndex]);
        if (quantity > 1) {
            const char *const token = GetShenmueItem(0, selection.selectedIndex);
            labelX -= static_cast<float>(2 * (token ? std::strlen(token) : 8U));
            DrawLegacyItemQuantity(x + width - 4.0f, labelY + 4.0f, quantity, 0.5f);
        }
        if (selection.selectedItem == 21) {
            const std::int32_t coins = GetCurrentSegaCoins();
            const std::int32_t digits = GetSelectorNumberDigits(coins);
            const float coinX = (labelX - 36.0f) + 164.0f;
            labelX -= static_cast<float>((digits > 0 ? digits : 1) * 8);
            DrawMoneyAmount(coinX, labelY + 12.0f, 0.5f, 1.0f, GetCurrentSegaCoins(), 0, 0, mode);
        }
    }
    LegacySpritePacket packet{};
    packet.quad.flags = 1024;
    packet.colour = 0xcdcdcdcdU;
    packet.secondaryColour = 0xff000000U;
    packet.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    if (itemSelectorLabelTexture != -1) {
        TransformScreenSpace(
            packet.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(labelX), TruncateSpriteCoordinate(labelY), 512, 32, 1.0f, 1.0f, 0, mode);
        LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int32_t>(itemSelectorLabelTexture)), 0.5f);
    }
    DrawLegacyWindowTiles(gridX, labelY, selection.columns, 1, ITEM_SELECTOR_TILE_DEPTH, 1024, mode);
    DrawLegacyWindowTiles(gridX, labelY - 48.0f, selection.columns, 1, ITEM_SELECTOR_TILE_DEPTH, 1024, mode);
    LegacyIconSprite &sprite = itemSelectorHeadingSprite;
    sprite.x = static_cast<float>(20 * (selection.columns - 9)) + (x + 50.0f);
    sprite.y = edgeY - 72.0f;
    PrepareDrawWithMode(sprite, mode);
    DateTime date[2];
    CopyCurrentCalendarRecord(std::span<std::byte, CALENDAR_RECORD_SIZE>(reinterpret_cast<std::byte *>(date), CALENDAR_RECORD_SIZE));
    DrawSelectorDate({.x = (sprite.x + 82.0f) + 48.0f, .y = sprite.y + 10.0f}, 0.7f, date[0]);
    const float cashPosition[4]{sprite.x + 82.0f, sprite.y + 10.0f, 0.6f, 1.0f};
    DrawSelectorCash(cashPosition, GetCurrentCash(), 1, 0, 1, mode);
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(0), TruncateSpriteCoordinate(itemSelectorCounterX),
        TruncateSpriteCoordinate(itemSelectorCounterY), 256, 16, 1.0f, 1.0f, 0, mode);
    LegacySpriteNullsub(packet, scenarioItemTexture, 0.7f);
}