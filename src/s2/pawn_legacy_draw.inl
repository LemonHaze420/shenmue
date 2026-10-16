PawnLegacyDrawState pawnDraw{};
std::array<PawnFlyerSetRow, 12> pawnFlyerRows{};
std::int32_t pawnSetCursor{}, pawnItemCursor{};

HLib::Task *PawnWindowTextTask() {
    return static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->windowTask;
}
void ReleasePawnNamedTexture(std::int32_t &texture) {
    if (texture != -1) {
        ReleaseNamedTexture(texture);
        texture = -1;
    }
}
void InitializePawnDrawState() {
    pawnDraw = {};
    pawnDraw.background = -1;
    pawnDraw.primaryLabels.fill(-1);
    pawnDraw.secondaryLabels.fill(-1);
    pawnDraw.actionLabels.fill(-1);
    pawnSetCursor = 0;
    pawnItemCursor = 0;
}
void ReleasePawnDrawState() {
    ReleasePawnNamedTexture(pawnDraw.background);
    ReleasePawnNamedTexture(pawnDraw.modeLabel);
    ReleasePawnNamedTexture(pawnDraw.shopLabel);
    for (std::int32_t &texture : pawnDraw.actionLabels)
        ReleasePawnNamedTexture(texture);
    for (std::size_t index = 0; index < 20; ++index) {
        ReleasePawnNamedTexture(pawnDraw.primaryLabels[index]);
        ReleasePawnNamedTexture(pawnDraw.secondaryLabels[index]);
        pawnDraw.labelOwners[index] = 0;
    }
    ReleaseLegacyCollectionSprites(TASK_PAWN);
}
std::uint32_t PawnPulseColour(std::uint32_t first, std::uint32_t second, std::int32_t frame) {
    if (shenIsGamePaused())
        return first;
    const std::uint16_t phase = static_cast<std::uint16_t>(frame % 32);
    const std::uint16_t weight = phase >= 16 ? phase - 16 : 16 - phase;
    const std::uint16_t complement = static_cast<std::uint16_t>(16U - weight);
    std::uint32_t result = 0;
    for (std::uint32_t shift = 0; shift < 32; shift += 8)
        result |= ((((weight * ((first >> shift) & 255) + complement * ((second >> shift) & 255)) >> 4) & 255) << shift);
    return result;
}
void PawnNullSprite(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::array<float, 4> uv, std::int32_t alignment = 0,
    std::int32_t rotation = 0, float depth = 1, std::uint32_t colour = 0xffffffffU, std::uint32_t secondary = 0, ScreenMode mode = ScreenMode::Centered,
    float scaleX = 1) {
    LegacySpritePacket packet{.quad = {.flags = 1024}, .colour = colour, .secondaryColour = secondary, .uv = uv};
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(alignment), x, y, width, height, scaleX, 1, rotation, mode);
    LegacySpriteNullsub(packet, static_cast<std::uint32_t>(pawnDraw.background), depth);
}
void PawnSolid(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, float depth, std::uint32_t colour, std::int32_t alignment = 0) {
    LegacyColourQuad packet{.quad = {.flags = 1024}, .colour = colour};
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(alignment), x, y, width, height, ScreenMode::Centered);
    shenSubmitQuad(packet, depth);
}
void PawnHighlight(
    std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t border, float depth, std::uint32_t fill, std::uint32_t edge) {
    PawnSolid(x, y, width, height, depth, fill);
    if (border <= 0)
        return;
    PawnSolid(x, y - border, width, border, depth, edge);
    PawnSolid(x + width, y - border, border, height + 2 * border, depth, edge);
    PawnSolid(x, y + height, width, border, depth, edge);
    PawnSolid(x - border, y - border, border, height + 2 * border, depth, edge);
}
void PawnUnderline(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, float depth, std::uint32_t colour) {
    if (pawnDraw.background == -1)
        return;
    const std::int32_t alignment = y >= pawnDraw.top + 5 ? 0 : 8;
    if (alignment)
        y += 4;
    const std::uint32_t primary = colour == 0xff000000U ? 0xffffffffU : 0xff808080U;
    const std::uint32_t secondary = colour == 0xff000000U ? 0 : colour & 0xffffffU;
    x += pawnDraw.left;
    y += pawnDraw.top;
    if (width <= 10)
        PawnNullSprite(x, y, width, height, {.5f, .25f, 1, .2890625f}, alignment, 0, depth, primary, secondary);
    else {
        PawnNullSprite(x, y, 5, height, {.5f, .25f, .5390625f, .2890625f}, alignment, 0, depth, primary, secondary);
        PawnNullSprite(x + width - 10, y, 5, height, {.9609375f, .25f, 1, .2890625f}, alignment, 0, depth, primary, secondary);
        PawnNullSprite(x + 5, y, width - 10, height, {.5390625f, .25f, .9609375f, .2890625f}, alignment, 0, depth, primary, secondary);
    }
}
void PawnCheckerBackground(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t scroll) {
    if (pawnDraw.background == -1 || width <= 0 || height <= 0)
        return;
    const float aspect = GetS2AspectRatioAdjustment();
    const std::int32_t right = x + static_cast<std::int32_t>(((1.0f - (aspect - .05f)) + 1.0f) * width);
    const std::int32_t bottom = y + height;
    std::int32_t offset = 64 - scroll % 64, parity = (scroll / 64) & 1;
    while (y - offset <= bottom) {
        std::int32_t tileParity = parity;
        for (std::int32_t tile = x; tile <= right; tile += 64) {
            if (tileParity)
                for (std::int32_t quadrant = 0; quadrant < 4; ++quadrant)
                    PawnNullSprite(tile + 32 * (quadrant & 1), y + 32 * (quadrant >> 1) - offset, 32, 32, {.5f, .5f, .75f, .75f}, 0, 0, 0, 0xffffffffU, 0,
                        static_cast<ScreenMode>(0));
            else
                PawnNullSprite(tile, y - offset, 64, 64, {0, .5f, .5f, 1}, 0, 0, 0, 0xffffffffU, 0, static_cast<ScreenMode>(0));
            tileParity ^= 1;
        }
        y += offset;
        offset = 64;
        parity ^= 1;
    }
}
void PawnPaneEdges(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t scroll, std::int32_t contentHeight) {
    if (pawnDraw.background == -1 || width <= 0 || height <= 0)
        return;
    if (scroll < 12) {
        const std::int32_t top = y - scroll + 4;
        std::int32_t pattern = 0;
        for (std::int32_t tile = x + 4; tile < x + width; tile += 64) {
            const std::int32_t tileWidth = tile + 64 > x + width - 4 ? x + width - 4 - tile : 64;
            const float v0 = .9375f - .0625f * pattern;
            PawnNullSprite(tile, top, tileWidth, 8, {.5f, v0, 1, v0 + .0625f}, 0, 0, 2);
            pattern = (pattern - 1) & 1;
        }
        if (top - 8 > 0)
            PawnSolid(x, top - 1, width, top - 8, 2, 0xff000000U, 8);
    }
    std::int32_t offset = scroll <= 12 ? 12 - scroll : -((scroll - 12) % 64);
    std::int32_t pattern = scroll <= 12 ? 0 : (scroll - 12) / 64 % 4;
    for (std::int32_t tile = offset + 64; tile - 64 < height; tile += 64) {
        constexpr std::array<float, 4> leftV{.75f, .8125f, .875f, .9375f};
        const float rightV = .9375f - .0625f * pattern;
        PawnNullSprite(x + 12, tile, 64, 8, {.5f, leftV[pattern], 1, leftV[pattern] + .0625f}, 8, 49152, 2);
        PawnNullSprite(x + width - 4, tile, 64, 8, {.5f, rightV, 1, rightV + .0625f}, 2, 16384, 2);
        pattern = (pattern + 1) % 4;
    }
    PawnSolid(x, y, 5, height, 2, 0xff000000U);
    PawnSolid(x + width - 4, y, 5, height, 2, 0xff000000U);
    if (contentHeight - height - scroll < 12) {
        const std::int32_t boundary = contentHeight + y - scroll;
        pattern = 0;
        for (std::int32_t tile = x + 4; tile < x + width; tile += 64) {
            const std::int32_t tileWidth = tile + 64 > x + width - 4 ? x + width - 4 - tile : 64;
            const float v0 = .9375f - .0625f * pattern;
            PawnNullSprite(tile, boundary - 12, tileWidth, 8, {.5f, v0, 1, v0 + .0625f}, 10, 32768, 2);
            pattern = (pattern - 1) & 1;
        }
        const std::int32_t mask = boundary - 5, maskHeight = y + 1 + height - mask;
        if (maskHeight > 0)
            PawnSolid(x, mask, width, maskHeight, 2, 0xff000000U);
    }
}
void PawnTextSprite(std::int32_t texture, float x, float y, std::int16_t width, std::int16_t height, float depth, std::uint8_t alignment = 0, float scaleY = 1,
    std::uint32_t rotation = 0) {
    LegacyIconSprite sprite{.x = x,
        .y = y,
        .scaleX = 1,
        .scaleY = scaleY,
        .depth = depth,
        .rotation = rotation,
        .icon = static_cast<std::uint16_t>(texture),
        .width = width,
        .height = height,
        .alignment = alignment};
    PrepareDrawTextureWithMode(sprite, ScreenMode::Centered);
}
void PawnPaneTitle() {
    const std::int32_t width = pawnDraw.right - pawnDraw.left + 1;
    if (pawnDraw.background == -1 || pawnDraw.scroll >= pawnDraw.titleOffset + pawnDraw.top + 72)
        return;
    if (pawnDraw.shopLabel == -1)
        pawnDraw.shopLabel = static_cast<std::int32_t>(SetupUIElement(PawnWindowTextTask(), pawnDraw.title, 256, -1, static_cast<std::int16_t>(0xf000), 1));
    const char *const mode = (pawnDraw.view & 2) ? ((pawnDraw.view & 1) ? "ITEM SET" : "COLLECTION SET") : "SINGLE";
    if (pawnDraw.modeLabel == -1)
        pawnDraw.modeLabel = static_cast<std::int32_t>(SetupUIElement(PawnWindowTextTask(), mode, 512, -1, static_cast<std::int16_t>(0xf000), 1));
    const std::int32_t margin = (width - 17 * static_cast<std::int32_t>(std::strlen(pawnDraw.title))) / 128;
    const std::int32_t top = pawnDraw.titleOffset - pawnDraw.scroll + pawnDraw.top + 16;
    std::int32_t remaining = width / 64;
    for (std::int32_t x = 0; x < width; x += 64, --remaining) {
        if (remaining <= margin || remaining > width / 64 - margin)
            PawnNullSprite(x, top + 64, 64, 64, {0, 0, .5f, .5f}, 8);
        else
            for (std::int32_t column = 0; column < 4; ++column) {
                PawnNullSprite(x + 16 * column, top + 8, 16, 8, {.75f, .1875f, .875f, .25f}, 8);
                for (std::int32_t row = 0; row < 6; ++row)
                    PawnNullSprite(x + 16 * column, top + 16 + 8 * row, 16, 8, {.875f, .1875f, 1, .25f}, 8);
                PawnNullSprite(x + 16 * column, top + 64, 16, 8, {.875f, .25f, .75f, .1875f}, 8);
            }
    }
    if (pawnDraw.shopLabel != -1)
        PawnTextSprite(pawnDraw.shopLabel, static_cast<float>(pawnDraw.left + width / 2), static_cast<float>(top + 40), 256, 34, 1.1f, 9);
    if (pawnDraw.modeLabel != -1)
        PawnTextSprite(pawnDraw.modeLabel, static_cast<float>(pawnDraw.left + width / 2), static_cast<float>(top + 56), 512, 16, 1.1f, 9);
}
void PawnPaneBackground(std::int32_t maskHeight) {
    const std::int32_t width = pawnDraw.right - pawnDraw.left + 1, height = pawnDraw.bottom - pawnDraw.top + 1;
    PawnCheckerBackground(pawnDraw.left, pawnDraw.top, width, height, pawnDraw.scroll);
    PawnPaneEdges(pawnDraw.left, pawnDraw.top, width, height, pawnDraw.scroll, pawnDraw.contentHeight);
    PawnPaneTitle();
    PawnSolid(0, pawnDraw.top + height - maskHeight, width, maskHeight, 2, 0xff000000U);
}
void PawnArrows(std::int32_t x, std::int32_t y, std::uint32_t mode) {
    if (mode > 1)
        return;
    constexpr std::array<std::uint16_t, 2> ends{1283, 1281}, middles{1284, 1282};
    LegacyIconSprite sprite{};
    InitializeLegacyIconSprite(sprite);
    sprite.x = static_cast<float>(x) - 24;
    sprite.y = static_cast<float>(y) - 10;
    sprite.scaleX = 1;
    sprite.scaleY = 1;
    sprite.depth = 1;
    sprite.rotation = 16384;
    sprite.icon = ends[mode];
    sprite.width = 16;
    sprite.height = 80;
    sprite.alignment = 8;
    PrepareDrawWithMode(sprite, ScreenMode::Centered);
    const float middle = static_cast<float>(y) + 16.0f;
    sprite.rotation = 49152;
    sprite.alignment = 2;
    sprite.y = (middle + 80.0f) - 10.0f;
    PrepareDrawWithMode(sprite, ScreenMode::Centered);
    sprite.icon = middles[mode];
    sprite.rotation = 16384;
    sprite.width = 80;
    sprite.alignment = 8;
    sprite.y = middle - 10.0f;
    PrepareDrawWithMode(sprite, ScreenMode::Centered);
}
void PawnActionButtons(std::uint32_t arrows) {
    if (pawnDraw.actionLabels[0] == -1)
        pawnDraw.actionLabels[0] =
            static_cast<std::int32_t>(CreateLeftAlignedWindowTextSurface(PawnWindowTextTask(), "Sell", 256, -1, static_cast<std::int16_t>(0xf000), 0));
    if (pawnDraw.actionLabels[2] == -1)
        pawnDraw.actionLabels[2] =
            static_cast<std::int32_t>(CreateLeftAlignedWindowTextSurface(PawnWindowTextTask(), "Cancel", 256, -1, static_cast<std::int16_t>(0xf000), 0));
    constexpr std::array<float, 4> positions{129, 160, 236, 280};
    for (std::size_t index = 0; index < 4; ++index)
        if (pawnDraw.actionLabels[index] != -1)
            PawnTextSprite(pawnDraw.actionLabels[index], 617, positions[index], 256, 32, 1.1f, 0, .9f, 16384);
    PawnArrows(589, 110, arrows & 1);
    PawnArrows(589, 230, (arrows >> 1) & 1);
}
void PawnSingleItemDetails(const PawnShopRecord *shop, std::int32_t cursor) {
    const PawnItemPrice *const prices = ResolveShopItems(shop);
    if (shop->itemCount < 0 || !prices || !pawnSaleItems || !pawnSaleQuantities || pawnSaleItemCount <= 0 || cursor < 0)
        return;
    PawnSolid(142, 404, 376, 40, 5.1f, 0xff000f3cU);
    PawnSolid(138, 400, 384, 4, 5.2f, 0xff00134eU);
    PawnSolid(138, 404, 4, 40, 5.2f, 0xff00134eU);
    PawnSolid(518, 404, 4, 40, 5.2f, 0xff00134eU);
    PawnSolid(138, 444, 384, 5, 5.2f, 0xff00134eU);
    if (cursor % 10 == 9) {
        if (pawnDraw.actionLabels[4] == -1)
            pawnDraw.actionLabels[4] =
                static_cast<std::int32_t>(CreateLeftAlignedWindowTextSurface(PawnWindowTextTask(), "Total :", 256, -1, static_cast<std::int16_t>(0xf000), 0));
        if (pawnDraw.actionLabels[4] != -1)
            PawnTextSprite(pawnDraw.actionLabels[4], 156, 400, 300, 32, 5.3f);
        const std::int32_t selectedTotal =
            static_cast<std::int32_t>(CalculatePawnSelectedItemsPrice(shop->itemCount, prices, pawnSaleItems, pawnSaleQuantities, pawnSaleItemCount));
        const std::int32_t total = selectedTotal < 0 ? 0 : selectedTotal;
        std::int32_t digits = 1;
        for (std::int32_t divisor = 10; divisor <= 1000000 && total / divisor != 0; divisor *= 10)
            ++digits;
        DrawMoneyAmount(static_cast<float>(332 + 12 * digits), 424, 5.3f, 1.2f, total, 2, 0xff656565U, ScreenMode::Centered);
    } else {
        const std::int32_t index = 9 * (cursor / 10) + cursor % 10;
        const std::uint16_t item = pawnSaleItems[index];
        const std::int16_t quantity = pawnSaleQuantities[index];
        const std::uint32_t signedItem = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(item)));
        const std::int32_t remaining = static_cast<std::int32_t>(GetItemState(signedItem)) - quantity;
        std::uint32_t colour;
        if (quantity == -256) {
            PawnNullSprite(169, 424, 64, 28, {.5f, .3359375f, 1, .5f}, 5, 0, 5.3f, 0xffffffffU, 0, ScreenMode::Centered, .8f);
            colour = 0x2b1552U;
        } else {
            std::int32_t price = 0;
            for (std::int32_t row = 0; row < shop->itemCount; ++row)
                if (prices[row].item == item) {
                    price = prices[row].price;
                    break;
                }
            DrawMoneyAmount(180, 424, 5.3f, 1, price, 2, 0xff656565U, ScreenMode::Centered);
            DrawLegacyItemQuantity(506.48001f, 424, remaining, 5.3f);
            colour = 0x656565U;
        }
        std::uint16_t label;
        (void)PrepareLegacyInventoryItemLabel(label, signedItem);
        if (label != 0xffff) {
            LegacySpritePacket packet{.quad = {.flags = 1024}, .colour = 0xcdcdcdcdU, .secondaryColour = colour, .uv = {0, 0, 1, 1}};
            AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, 330, 424, 512, 32, ScreenMode::Centered);
            LegacySpriteNullsub(packet, static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(label))), 5.3f);
        }
    }
}
void PawnSingleItems(std::int32_t cursor) {
    const std::int32_t visibleHeight = pawnDraw.bottom - pawnDraw.top - 101, offset = pawnDraw.scroll - 94;
    std::int32_t first = 9 * (offset >= 0 ? offset / 52 : 0), y = 52 * (first / 9) - offset;
    for (std::int32_t column = 0; y < visibleHeight;) {
        const std::int32_t index = first + column;
        if (index >= pawnSaleItemCount)
            break;
        const std::int16_t quantity = pawnSaleQuantities[index];
        LegacyIconSprite sprite{};
        InitializeLegacyIconSprite(sprite);
        sprite.x = static_cast<float>(60 * column + 32);
        sprite.y = static_cast<float>(pawnDraw.top + y + (y < 0 ? 39 : 0));
        sprite.depth = 1;
        sprite.icon = GetInventoryItemIconById(pawnSaleItems[index]);
        sprite.width = 40;
        sprite.height = 40;
        sprite.alignment = y < 0 ? 8 : 0;
        PrepareDrawSimple(sprite, quantity == -256 ? 0x80101020U : 0xff000000U, ScreenMode::Centered);
        const bool selected = column + 10 * (first / 9) == cursor;
        const std::uint32_t pulse = selected ? PawnPulseColour(0x3b8c0000U, 0x208c0000U, pawnDraw.frame) : 0;
        const std::uint32_t underline = quantity == -256 ? 0 : selected ? pulse : 0xff000000U;
        if (quantity > 0) {
            DrawMoneyDigits(static_cast<float>(60 * column + 72), static_cast<float>(pawnDraw.top + y + 32), 1, 1, quantity, 0xff656565U, ScreenMode::Centered);
            PawnSolid(60 * column + 32, pawnDraw.top + y, 60, 40, .9f, selected ? underline : 0x80802010U);
        }
        PawnUnderline(60 * column + 32, pawnDraw.top + y + 28, 56, 4, 1, underline);
        if (++column == 9) {
            column = 0;
            first += 9;
            y += 52;
        }
    }
}
void InitPawnshopUIElement(PawnDataHeader *data, shen_name shopName, std::int32_t cursor) {
    if (!data)
        return;
    const PawnShopRecord *const shop = FindPawnShop(data, shopName);
    if (!shop || !pawnSaleItems || !pawnSaleQuantities)
        return;
    PawnPaneBackground(102);
    const std::int32_t y = 52 * (cursor / 10) - pawnDraw.targetScroll + 94;
    std::uint32_t arrows = 0;
    if (cursor % 10 == 9)
        arrows = (y >= (pawnDraw.bottom - pawnDraw.top - 101) / 2) + 1;
    else {
        LegacyIconSprite sprite{};
        InitializeLegacyIconSprite(sprite);
        sprite.x = static_cast<float>(60 * (cursor % 10) + 32);
        sprite.y = static_cast<float>(pawnDraw.top + y);
        sprite.depth = 1.9f;
        sprite.icon = PAWN_CURSOR_ICON;
        sprite.width = 56;
        sprite.height = 38;
        sprite.alignment = 0;
        PrepareDrawSimple(sprite, PawnPulseColour(0x3b8c0000U, 0x208c0000U, pawnDraw.frame) | 0xff000000U, ScreenMode::Centered);
    }
    PawnSingleItemDetails(shop, cursor);
    PawnSingleItems(cursor);
    PawnActionButtons(arrows);
}
std::int32_t *PawnSetCollectedFlags(const char *label) {
    for (PawnFlyerSetRow &row : pawnFlyerRows)
        if (std::strcmp(row.label, label) == 0)
            return row.collected.data();
    return nullptr;
}
void PawnCollectedIcon(std::int32_t x, std::int32_t y) {
    if (GetDebugForceUIEnabled(UIElement::PawnshopFlyer) || pawnDraw.background == -1)
        return;
    const std::int32_t alignment = y >= 32 ? 0 : 8;
    PawnNullSprite(x, y + (alignment ? 31 : 0), 32, 32, {.5f, 0, .75f, .25f}, alignment, 0, 1.8f);
}
void PawnSets(const PawnShopRecord *shop, std::int32_t cursor, std::int32_t view) {
    if (!shop || !pawnSetOrder || pawnDraw.scroll < 0)
        return;
    const std::uint32_t fill = PawnPulseColour(0x3b8c0000U, 0x208c0000U, pawnDraw.frame);
    const std::uint32_t border = PawnPulseColour(0xffc04040U, 0x80c04040U, pawnDraw.frame);
    const std::uint32_t labelPulse = PawnPulseColour(0xffffffffU, 0x00ffff20U, pawnDraw.frame) | 0xffffffU;
    std::int32_t ordinal = 0, y = 110;
    while (ordinal < shop->setCount && pawnSetOrder[ordinal] >= 0) {
        const PawnSetRecord *const set = GetPawnShopSet(shop, pawnSetOrder[ordinal]);
        if (set) {
            const std::int32_t next = y + 47 * ((set->itemCount + 8) / 9) + 16;
            if (next > pawnDraw.scroll) {
                y -= pawnDraw.scroll;
                break;
            }
            y = next;
        }
        ++ordinal;
    }
    for (; ordinal < shop->setCount && pawnSetOrder[ordinal] >= 0 && y < pawnDraw.bottom; ++ordinal) {
        const std::int32_t setIndex = pawnSetOrder[ordinal];
        const PawnSetRecord *const set = GetPawnShopSet(shop, setIndex);
        if (!set)
            continue;
        const std::int16_t *const items = reinterpret_cast<const std::int16_t *>(set + 1);
        std::int32_t unique = 0;
        for (std::int32_t item = 0; item < set->itemCount; ++item) {
            std::int32_t index = 0;
            for (; index < unique; ++index)
                if (pawnStockScratch[index].item == items[item])
                    break;
            if (index == unique)
                pawnStockScratch[unique++] = {.item = items[item], .remaining = 1};
            else
                ++pawnStockScratch[index].remaining;
        }
        bool available = true;
        for (std::int32_t index = 0; index < unique; ++index) {
            const std::int32_t count = static_cast<std::int32_t>(GetItemState(pawnStockScratch[index].item));
            if (pawnStockScratch[index].remaining > count)
                available = false;
            pawnStockScratch[index].remaining = count;
        }
        const std::uint32_t identity = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(set)) & 0x7fffffffU;
        std::size_t slot = 0;
        for (; slot < 20; ++slot)
            if (pawnDraw.primaryLabels[slot] != -1 && (pawnDraw.labelOwners[slot] & 0x7fffffffU) == identity)
                break;
        if (slot == 20) {
            for (slot = 0; slot < 20; ++slot)
                if (pawnDraw.primaryLabels[slot] == -1)
                    break;
            if (slot == 20)
                return;
            pawnDraw.primaryLabels[slot] = static_cast<std::int32_t>(CreateLeftAlignedWindowTextSurface(
                PawnWindowTextTask(), set->label, 512, static_cast<std::int16_t>(available ? 0xffff : 0xf315), static_cast<std::int16_t>(0xf000), 0));
            pawnDraw.secondaryLabels[slot] = available ? static_cast<std::int32_t>(CreateLeftAlignedWindowTextSurface(PawnWindowTextTask(), set->label, 512,
                                                             static_cast<std::int16_t>(0xfff2), static_cast<std::int16_t>(0xf000), 0)) :
                                                         -1;
            pawnDraw.labelOwners[slot] = identity;
            if (pawnDraw.primaryLabels[slot] < 0) {
                pawnDraw.primaryLabels[slot] = -1;
                continue;
            }
        }
        const bool legacy = GetDebugForceUIEnabled(UIElement::PawnshopFlyer) == 0;
        if (pawnDraw.primaryLabels[slot] != -1) {
            pawnDraw.labelOwners[slot] |= 0x80000000U;
            if (legacy)
                PawnTextSprite(pawnDraw.primaryLabels[slot], static_cast<float>(pawnDraw.left + 35), static_cast<float>(pawnDraw.top + y + (y < 32 ? 31 : 0)),
                    512, 32, 1.1f, y < 32 ? 8 : 0);
        }
        if (pawnDraw.secondaryLabels[slot] != -1) {
            pawnDraw.labelOwners[slot] |= 0x80000000U;
            if (available && legacy) {
                LegacyIconSprite sprite{.x = static_cast<float>(pawnDraw.left + 35),
                    .y = static_cast<float>(pawnDraw.top + y + (y < 32 ? 31 : 0)),
                    .scaleX = 1,
                    .scaleY = 1,
                    .depth = 1.11f,
                    .icon = static_cast<std::uint16_t>(pawnDraw.secondaryLabels[slot]),
                    .width = 512,
                    .height = 32,
                    .alignment = static_cast<std::uint8_t>(y < 32 ? 8 : 0)};
                shenTransformScreenCoors(sprite, labelPulse, ScreenMode::Centered);
            }
        }
        if (legacy) {
            DrawMoneyAmount(static_cast<float>(pawnDraw.left + 252), static_cast<float>(pawnDraw.top + y + 32 + (set->itemCount > 9 ? 32 : 0)), 1.1f, 1,
                GetPawnSaleSetPrice(setIndex), 2, available ? 0xff656565U : 0xff2b1352U, ScreenMode::Centered);
            if (view == 0 && ordinal == cursor)
                PawnHighlight(pawnDraw.left + 33, y + 9, 255, 50, 3, 1.9f, fill, border);
        }
        std::int32_t *const collected = PawnSetCollectedFlags(set->label);
        for (std::int32_t item = 0; item < set->itemCount; ++item) {
            if (collected)
                collected[item] = 0;
            for (std::int32_t index = 0; index < unique; ++index)
                if (pawnStockScratch[index].item == items[item]) {
                    if (--pawnStockScratch[index].remaining >= 0) {
                        if (legacy)
                            PawnCollectedIcon(pawnDraw.left + 4 * (9 * (item % 9) + 73), y + 20);
                        if (collected)
                            collected[item] = 1;
                    }
                    break;
                }
            LegacyIconSprite sprite{};
            InitializeLegacyIconSprite(sprite);
            sprite.depth = 1.1f;
            sprite.icon = GetInventoryItemIconById(items[item]);
            sprite.width = 40;
            sprite.height = 40;
            sprite.x = static_cast<float>(pawnDraw.left + 36 * (item % 9 + 8));
            sprite.y = static_cast<float>(y + 55 - (y >= 40 ? 39 : 0));
            sprite.alignment = y >= 40 ? 0 : 8;
            if (legacy)
                PrepareDrawSimple(sprite, 0xff000000U, ScreenMode::Centered);
            if (item != 0 && (item + 1) % 9 == 0)
                y += 47;
        }
        if (set->itemCount != 0 && set->itemCount % 9 != 0)
            y += 47;
        if (legacy)
            PawnUnderline(16, y, pawnDraw.right - pawnDraw.left - 23, 5, 1.1f, available ? 0xff000000U : 0);
        y += 16;
    }
}
void OpenPawnshopUIElement(PawnDataHeader *data, shen_name shopName, std::int32_t cursor, std::int32_t view) {
    if (!data)
        return;
    const PawnShopRecord *const shop = FindPawnShop(data, shopName);
    if (!shop || pawnDraw.background == -1 || pawnDraw.scroll < 0 || !pawnSetOrder || cursor < 0)
        return;
    if (GetDebugForceUIEnabled(UIElement::PawnshopFlyer) == 0)
        PawnPaneBackground(32);
    PawnSets(shop, cursor, view);
}
void DrawPawnShopLegacy() {
    if (GetDebugForceUIEnabled(UIElement::PawnshopFlyer) || static_cast<std::int32_t>(pawnSelection.flags) >= 0 || pawnDraw.background == -1)
        return;
    for (std::uint32_t &owner : pawnDraw.labelOwners)
        owner &= 0x7fffffffU;
    pawnDraw.frame = static_cast<std::int32_t>(static_cast<std::uint32_t>(pawnDraw.frame) + 1U);
    if (!shenIsGamePaused()) {
        if (pawnDraw.targetScroll != pawnDraw.previousTarget) {
            pawnDraw.previousTarget = pawnDraw.targetScroll;
            pawnDraw.scrollStart = pawnDraw.scroll;
            if (pawnDraw.interpolationFrames < 5)
                pawnDraw.interpolationFrames = 10;
        }
        if (pawnDraw.scroll != pawnDraw.targetScroll) {
            if (pawnDraw.interpolationFrames > 0) {
                if (pawnDraw.interpolationFrames != 1) {
                    const std::int32_t weight = pawnDraw.interpolationFrames > 5 ? 11 - pawnDraw.interpolationFrames : pawnDraw.interpolationFrames;
                    const std::uint32_t delta = (static_cast<std::uint32_t>(pawnDraw.targetScroll) - static_cast<std::uint32_t>(pawnDraw.scrollStart)) *
                        static_cast<std::uint32_t>(weight);
                    pawnDraw.scroll = static_cast<std::int32_t>(
                        static_cast<std::uint32_t>(pawnDraw.scroll) + static_cast<std::uint32_t>(static_cast<std::int32_t>(delta) / 30));
                    --pawnDraw.interpolationFrames;
                } else if (static_cast<std::uint32_t>(pawnDraw.targetScroll) - static_cast<std::uint32_t>(pawnDraw.scroll) + 32U > 64U) {
                    const std::int32_t delta =
                        static_cast<std::int32_t>(static_cast<std::uint32_t>(pawnDraw.targetScroll) - static_cast<std::uint32_t>(pawnDraw.scroll));
                    pawnDraw.scroll = static_cast<std::int32_t>(static_cast<std::uint32_t>(pawnDraw.scroll) + (delta > 0 ? 32U : 0xffffffe0U));
                } else {
                    pawnDraw.scroll = pawnDraw.targetScroll;
                    pawnDraw.interpolationFrames = 0;
                }
            } else
                pawnDraw.interpolationFrames = 0;
        }
    }
    if (pawnSelection.flags & 2U)
        OpenPawnshopUIElement(pawnShopData, pawnShopName, pawnSetCursor, pawnSelection.flags & 1U);
    else
        InitPawnshopUIElement(pawnShopData, pawnShopName, pawnItemCursor);
    for (std::size_t index = 0; index < 20; ++index)
        if (static_cast<std::int32_t>(pawnDraw.labelOwners[index]) >= 0) {
            ReleasePawnNamedTexture(pawnDraw.primaryLabels[index]);
            ReleasePawnNamedTexture(pawnDraw.secondaryLabels[index]);
            pawnDraw.labelOwners[index] = 0;
        }
}
void OpenPawnDrawState(const PawnShopRecord *shop, std::uint32_t flags) {
    if (flags & 2U) {
        pawnDraw.view = static_cast<std::int32_t>(((flags & 4U) | 8U) >> 2);
        pawnDraw.contentHeight = 186;
        for (std::int32_t index = 0; index < shop->setCount; ++index) {
            const PawnSetRecord *const set = GetPawnShopSet(shop, index);
            if (set)
                pawnDraw.contentHeight += 47 * ((set->itemCount + 8) / 9) + 16;
        }
        if (pawnDraw.contentHeight < pawnDraw.bottom - pawnDraw.top - 31)
            pawnDraw.contentHeight = pawnDraw.bottom - pawnDraw.top - 31;
    } else {
        pawnDraw.view = (flags & 4U) != 0;
        pawnDraw.contentHeight = 52 * ((pawnSaleItemCount + 8) / 9) + 122;
        if (pawnDraw.contentHeight < pawnDraw.bottom - pawnDraw.top - 101)
            pawnDraw.contentHeight = pawnDraw.bottom - pawnDraw.top - 101;
    }
    std::memset(pawnDraw.title, 0, sizeof(pawnDraw.title));
    std::strcpy(pawnDraw.title, shop->label);
    if (pawnDraw.background == -1) {
        void *allocation{};
        std::size_t size{};
        if (LoadLegacySprite(Filepath_Gen(FilepathGenMode::SpriteDirectory), "PAWNBG03.SPR", TASK_PAWN, allocation, size))
            pawnDraw.background = static_cast<std::int32_t>(ReadTexnTexture(allocation));
        if (allocation)
            HLib::FreeTaskData(allocation);
    }
}
