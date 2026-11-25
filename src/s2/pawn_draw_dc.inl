namespace {
inline HLib::Task *PawnWindowTextTask() {
    return static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD;
}

#define PAWN_CLEAR_PACKET(packet) \
    do { \
        (packet).quad.flags = 0; \
        (packet).colour = 0xffffffffU; \
        (packet).secondaryColour = 0; \
        (packet).uv[0] = (packet).uv[1] = 0.0f; \
        (packet).uv[2] = (packet).uv[3] = 1.0f; \
        (packet).reservedTail = 0; \
    } while (0)
#define PAWN_INIT_PACKET(packet) \
    do { \
        PAWN_CLEAR_PACKET(packet); \
        (packet).quad.flags |= 0x400U; \
    } while (0)
#define PAWN_INIT_COLOUR_PACKET(packet, fill) \
    do { \
        PAWN_CLEAR_PACKET(packet); \
        (packet).colour = (fill); \
        (packet).quad.flags |= 0x400U; \
    } while (0)

std::uint32_t PawnPulseColour(std::uint32_t first, std::uint32_t second, std::int32_t frame) {
    std::uint32_t result;
    if (!shenIsGamePaused()) {
        std::uint16_t weight = static_cast<std::uint16_t>(frame % 32);
        if (weight < 16)
            weight = static_cast<std::uint16_t>(16 - weight);
        else
            weight = static_cast<std::uint16_t>(weight - 16);
        const std::uint16_t complement = static_cast<std::uint16_t>(16 - weight);
        const std::uint8_t *const a = reinterpret_cast<const std::uint8_t *>(&first);
        const std::uint8_t *const b = reinterpret_cast<const std::uint8_t *>(&second);
        std::uint8_t *const out = reinterpret_cast<std::uint8_t *>(&result);
        for (std::int32_t i = 0; i < 4; ++i)
            out[i] = static_cast<std::uint8_t>((a[i] * weight + b[i] * complement) >> 4);
    } else {
        result = first;
    }
    return result;
}
void PawnHighlight(
    std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t border, float depth, std::uint32_t fill, std::uint32_t edge) {
    LegacySpritePacket packet;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    packet.colour = fill;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), x, y, width, height);
    DrawLegacyColourPacket(packet, depth);
    if (border > 0) {
        packet.colour = edge;
        AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), x, y - border, width, border);
        DrawLegacyColourPacket(packet, depth);
        AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), x + width, y - border, border, height + 2 * border);
        DrawLegacyColourPacket(packet, depth);
        AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), x, y + height, width, border);
        DrawLegacyColourPacket(packet, depth);
        AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), x - border, y - border, border, height + 2 * border);
        DrawLegacyColourPacket(packet, depth);
    }
}
void PawnUnderline(
    const PawnLegacyDrawState *draw, std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, float depth, std::uint32_t colour) {
    LegacySpritePacket packet;
    std::int32_t alignment;
    if (!draw || draw->background == -1)
        return;
    if (y < draw->top + 5) {
        alignment = 8;
        y += 4;
    } else
        alignment = 0;
    PAWN_CLEAR_PACKET(packet);
    if (colour != 0xff000000U) {
        packet.secondaryColour = colour & 0xffffffU;
        packet.colour = 0xff808080U;
    }
    packet.quad.flags |= 0x400U;
    if (width > 10) {
        TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(alignment), draw->left + x, draw->top + y, 5, height, 1.0f, 1.0f, 0);
        packet.uv[0] = .5f;
        packet.uv[1] = .25f;
        packet.uv[2] = .5390625f;
        packet.uv[3] = .2890625f;
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(draw->background), depth);
        TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(alignment), draw->left + x + width - 10, draw->top + y, 5, height, 1.0f, 1.0f, 0);
        packet.uv[0] = .9609375f;
        packet.uv[1] = .25f;
        packet.uv[2] = 1.0f;
        packet.uv[3] = .2890625f;
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(draw->background), depth);
        TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(alignment), draw->left + x + 5, draw->top + y, width - 10, height, 1.0f, 1.0f, 0);
        packet.uv[0] = .5390625f;
        packet.uv[1] = .25f;
        packet.uv[2] = .9609375f;
        packet.uv[3] = .2890625f;
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(draw->background), depth);
    } else {
        TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(alignment), draw->left + x, draw->top + y, width, height, 1.0f, 1.0f, 0);
        packet.uv[0] = .5f;
        packet.uv[1] = .25f;
        packet.uv[2] = 1.0f;
        packet.uv[3] = .2890625f;
        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(draw->background), 1.1f);
    }
}
void PawnCheckerBackground(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t scroll, std::int32_t background) {
    LegacySpritePacket packet;
    if (background == -1 || width <= 0 || height <= 0)
        return;
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    std::int32_t offset = 64 - scroll % 64;
    std::int32_t parity = (scroll / 64) & 1;
    std::int32_t curY = y;
    while (curY - offset <= y + height) {
        std::int32_t tileParity = parity;
        for (std::int32_t tile = x; tile <= x + width; tile += 64) {
            if (tileParity) {
                for (std::int32_t quadrant = 0; quadrant < 4; ++quadrant) {
                    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(0), tile + 32 * (quadrant & 1), curY - offset + 32 * (quadrant >> 1), 32,
                        32, 1.0f, 1.0f, 0);
                    packet.uv[0] = .5f;
                    packet.uv[1] = .5f;
                    packet.uv[2] = .75f;
                    packet.uv[3] = .75f;
                    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(background), 0.0f);
                }
            } else {
                TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(0), tile, curY - offset, 64, 64, 1.0f, 1.0f, 0);
                packet.uv[0] = 0.0f;
                packet.uv[1] = .5f;
                packet.uv[2] = .5f;
                packet.uv[3] = 1.0f;
                DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(background), 0.0f);
            }
            tileParity = !tileParity;
        }
        parity = !parity;
        curY += offset;
        offset = 64;
    }
}
void SetPawnEdgeUV(LegacySpritePacket *packet, std::int32_t kind);
inline std::int32_t PawnInset(std::int32_t edge, std::int32_t inset) {
    return edge - inset;
}
void PawnPaneEdges(
    std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t scroll, std::int32_t contentHeight, std::int32_t background) {
    LegacySpritePacket edge, solid;
    std::int32_t top, tile, tileWidth, pattern, offset, boundary, maskHeight;
    if (background == -1 || width <= 0 || height <= 0)
        return;
    PAWN_INIT_PACKET(edge);
    PAWN_CLEAR_PACKET(solid);
    solid.quad.flags |= 0x400U;
    if (scroll < 12) {
        top = scroll ? PawnInset(y, scroll - 4) : y + 4;
        pattern = 0;
        for (tile = x + 4; tile < x + width; tile += 64) {
            tileWidth = tile + 64 > x + width - 4 ? PawnInset(x + width, tile + 4) : 64;
            TransformScreenSpace(edge.quad, static_cast<ScreenAlignment>(0), tile, top, tileWidth, 8, 1, 1, 0);
            SetPawnEdgeUV(&edge, pattern);
            pattern = (pattern + 1) & 1;
            DrawLegacySpritePacket(edge, static_cast<std::uint32_t>(background), 2);
        }
        if (top - 8 > 0) {
            solid.colour = 0xff000000U;
            AdjustScreenAspectRatio(solid.quad, static_cast<ScreenAlignment>(8), x, top - 1, width, top - 8);
            DrawLegacyColourPacket(solid, 2);
        }
    }
    if (scroll > 12) {
        offset = -((scroll - 12) % 64);
        pattern = (scroll - 12) / 64 % 4;
    } else {
        offset = 12 - scroll;
        pattern = 0;
    }
    for (; offset < height; offset += 64) {
        SetPawnEdgeUV(&edge, 3 - pattern);
        TransformScreenSpace(edge.quad, static_cast<ScreenAlignment>(8), x + 12, offset + 64, 64, 8, 1, 1, 49152);
        DrawLegacySpritePacket(edge, static_cast<std::uint32_t>(background), 2);
        SetPawnEdgeUV(&edge, pattern);
        TransformScreenSpace(edge.quad, static_cast<ScreenAlignment>(2), x + width - 4, offset + 64, 64, 8, 1, 1, 16384);
        DrawLegacySpritePacket(edge, static_cast<std::uint32_t>(background), 2);
        pattern = (pattern + 1) % 4;
    }
    solid.colour = 0xff000000U;
    AdjustScreenAspectRatio(solid.quad, static_cast<ScreenAlignment>(0), x, y, 5, height);
    DrawLegacyColourPacket(solid, 2);
    AdjustScreenAspectRatio(solid.quad, static_cast<ScreenAlignment>(0), x + width - 4, y, 5, height);
    DrawLegacyColourPacket(solid, 2);
    if (contentHeight - scroll - height < 12) {
        boundary = y + contentHeight - scroll - 12;
        do {
            pattern = 0;
            for (tile = x + 4; tile < x + width; tile += 64) {
                tileWidth = tile + 64 > x + width - 4 ? PawnInset(x + width, tile + 4) : 64;
                TransformScreenSpace(edge.quad, static_cast<ScreenAlignment>(10), tile, boundary, tileWidth, 8, 1, 1, 32768);
                SetPawnEdgeUV(&edge, pattern);
                pattern = (pattern + 1) & 1;
                DrawLegacySpritePacket(edge, static_cast<std::uint32_t>(background), 2);
            }
        } while (0);
        boundary += 7;
        maskHeight = y + height - boundary + 1;
        if (maskHeight > 0) {
            solid.colour = 0xff000000U;
            AdjustScreenAspectRatio(solid.quad, static_cast<ScreenAlignment>(0), x, boundary, width, maskHeight);
            DrawLegacyColourPacket(solid, 2);
        }
    }
}
void SetPawnEdgeUV(LegacySpritePacket *packet, std::int32_t kind) {
    if (!packet)
        return;
    float v0 = .9375f, u1 = 1, v1 = u1;
    switch (kind) {
    case 1:
        v0 = .875f;
        v1 = .9375f;
        break;
    case 2:
        v0 = .8125f;
        v1 = .875f;
        break;
    case 3:
        v0 = .75f;
        v1 = .8125f;
        break;
    }
    packet->uv[0] = .5f;
    packet->uv[1] = v0;
    packet->uv[2] = u1;
    packet->uv[3] = v1;
}
// @verified - d
void PawnPaneTitle(PawnLegacyDrawState *draw) {
    LegacySpritePacket packet;
    LegacyIconSprite sprite;
    char text[256];
    std::uint32_t item, set;
    std::int32_t left, width, top, scroll, titleOffset;
    const char *title;
    std::int32_t background;
    std::int32_t *modeLabel, *shopLabel;
    const char *mode;
    std::int32_t x, y, remaining, titleWidth, margin, row, i;
    while (draw) {
        item = draw->view & 1;
        set = draw->view & 2;
        left = draw->left;
        width = draw->right - left + 1;
        top = draw->top;
        scroll = draw->scroll;
        titleOffset = draw->titleOffset;
        title = draw->title;
        background = draw->background;
        modeLabel = &draw->modeLabel;
        shopLabel = &draw->shopLabel;
        if (background == -1)
            break;
        if (scroll >= top + titleOffset + 72)
            return;
        INIT_LEGACY_SPRITE_PACKET(packet);
        packet.quad.flags |= 0x400U;
        if (*shopLabel == -1)
            *shopLabel = CreateCenteredWindowTextSurface(
                static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, title, 128, 0xffff, 0xf000, 1);
        if (set)
            mode = item ? "\xa5\xa2\xa5\xa4\xa5\xc6\xa5\xe0\xa1\xa1\xa5\xbb\xa5\xc3\xa5\xc8" :
                          "\xa5\xb3\xa5\xec\xa5\xaf\xa5\xb7\xa5\xe7\xa5\xf3\xa1\xa1\xa5\xbb\xa5\xc3\xa5\xc8";
        else
            mode = item ? "\xc3\xb1\xc9\xca\0" : "\xc3\xb1\xc9\xca";
        for (i = 0; mode[i]; ++i)
            text[i] = mode[i];
        text[i] = 0;
        if (*modeLabel == -1)
            *modeLabel = CreateCenteredWindowTextSurface(
                static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, text, 512, 0xffff, 0xf000, 1);
        titleWidth = 0;
        for (i = 0; title[i]; i += 2)
            titleWidth += 34;
        margin = (width - titleWidth) / 128;
        y = titleOffset + 8 + top - scroll + 8;
        remaining = width / 64;
        for (x = 0; x < width; x += 64) {
            if (remaining <= margin || remaining > width / 64 - margin) {
                TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(8), x, y + 64, 64, 64, 1, 1, 0);
                packet.uv[0] = 0;
                packet.uv[1] = 0;
                packet.uv[2] = .5f;
                packet.uv[3] = .5f;
                DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(background), 1);
            } else
                for (i = 0; i < 4; ++i) {
                    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(8), x + i * 16, y + 8, 16, 8, 1, 1, 0);
                    packet.uv[0] = .75f;
                    packet.uv[1] = .1875f;
                    packet.uv[2] = .875f;
                    packet.uv[3] = .25f;
                    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(background), 1);
                    for (row = 1; row < 7; ++row) {
                        packet.uv[0] = .875f;
                        packet.uv[1] = .1875f;
                        packet.uv[2] = 1;
                        packet.uv[3] = .25f;
                        TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(8), x + i * 16, y + row * 8 + 8, 16, 8, 1, 1, 0);
                        DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(background), 1);
                    }
                    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(8), x + i * 16, y + 64, 16, 8, 1, 1, 0);
                    packet.uv[0] = .875f;
                    packet.uv[1] = .25f;
                    packet.uv[2] = .75f;
                    packet.uv[3] = .1875f;
                    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(background), 1);
                }
            --remaining;
        }
        if (*shopLabel != -1) {
            sprite.x = static_cast<float>(width / 2 + left);
            sprite.y = static_cast<float>(y + 40);
            sprite.scaleX = 1;
            sprite.scaleY = 1;
            sprite.depth = 1.1f;
            sprite.flags = 0;
            sprite.rotation = 0;
            sprite.icon = static_cast<std::uint16_t>(*shopLabel);
            sprite.width = static_cast<std::int16_t>(titleWidth);
            sprite.height = 34;
            sprite.alignment = 9;
            PrepareDrawTextureWithMode(sprite);
        }
        if (*modeLabel != -1) {
            sprite.x = static_cast<float>(width / 2 + left);
            sprite.y = static_cast<float>(y + 56);
            sprite.scaleX = 1;
            sprite.scaleY = 1;
            sprite.depth = 1.1f;
            sprite.flags = 0;
            sprite.rotation = 0;
            sprite.icon = static_cast<std::uint16_t>(*modeLabel);
            sprite.width = 512;
            sprite.height = 16;
            sprite.alignment = 9;
            PrepareDrawTextureWithMode(sprite);
        }
        break;
    }
}
void PawnPaneBackground(PawnLegacyDrawState *draw, std::int32_t maskHeight) {
    LegacySpritePacket packet;
    if (!draw)
        return;
    std::int32_t left = draw->left;
    std::int32_t width = draw->right - left + 1;
    std::int32_t top = draw->top;
    std::int32_t height = draw->bottom - top + 1;
    std::int32_t scroll = draw->scroll;
    std::int32_t contentHeight = draw->contentHeight;
    std::int32_t background = draw->background;
    PawnCheckerBackground(left, top, width, height, scroll, background);
    PawnPaneEdges(left, top, width, height, scroll, contentHeight, background);
    PawnPaneTitle(draw);
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    packet.colour = 0xff000000U;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 0, height - maskHeight + top, width, maskHeight);
    DrawLegacyColourPacket(packet, 2.0f);
}
void PawnArrows(std::int32_t x, std::int32_t y, std::uint32_t mode) {
    LegacyIconSprite sprite;
    const std::uint16_t ends[2] = {1283, 1281};
    const std::uint16_t middles[2] = {1284, 1282};
    if (mode > 1)
        return;
    InitializeLegacyIconSprite(sprite);
    sprite.x = static_cast<float>(x) + (-24.0f);
    sprite.y = static_cast<float>(y) + (-10.0f);
    sprite.scaleX = 1.0f;
    sprite.scaleY = 1.0f;
    sprite.depth = 1.0f;
    sprite.flags = 0;
    sprite.rotation = 16384;
    sprite.icon = ends[mode];
    sprite.width = 16;
    sprite.height = 80;
    sprite.alignment = 8;
    PrepareDrawWithMode(sprite);
    const float baseY = static_cast<float>(y) + 16.0f;
    sprite.y = (baseY + 80.0f) + (-10.0f);
    sprite.rotation = 49152;
    sprite.alignment = 2;
    PrepareDrawWithMode(sprite);
    sprite.y = baseY + (-10.0f);
    sprite.icon = middles[mode];
    sprite.rotation = 16384;
    sprite.width = 80;
    sprite.alignment = 8;
    PrepareDrawWithMode(sprite);
}
void PawnActionButtons(PawnLegacyDrawState *draw, std::uint32_t arrows) {
    std::int32_t ids[4];
    const float ys[4] = {120.0f, 160.0f, 240.0f, 280.0f};
    LegacyIconSprite sprite;
    std::int32_t arrowY[2] = {0, 0};
    arrowY[0] = 110;
    arrowY[1] = 230;
    if (!draw)
        return;
    HLib::Task *const textTask = PawnWindowTextTask();
    if (draw->actionLabels[0] == -1)
        draw->actionLabels[0] = SetupUIElement(textTask, "\xb7\xe8", 256, 0xffff, 0xf000, 1);
    if (draw->actionLabels[1] == -1)
        draw->actionLabels[1] = SetupUIElement(textTask, "\xc4\xea", 256, 0xffff, 0xf000, 1);
    if (draw->actionLabels[2] == -1)
        draw->actionLabels[2] = SetupUIElement(textTask, "\xcc\xe1", 256, 0xffff, 0xf000, 1);
    if (draw->actionLabels[3] == -1)
        draw->actionLabels[3] = SetupUIElement(textTask, "\xa4\xeb", 256, 0xffff, 0xf000, 1);
    ids[0] = draw->actionLabels[0];
    ids[1] = draw->actionLabels[1];
    ids[2] = draw->actionLabels[2];
    ids[3] = draw->actionLabels[3];
    sprite.x = 589.0f;
    sprite.flags = 0;
    sprite.rotation = 0;
    sprite.scaleX = 1.0f;
    sprite.scaleY = 1.0f;
    sprite.width = 256;
    sprite.height = 28;
    sprite.depth = 1.1f;
    sprite.alignment = 0;
    for (std::int32_t i = 0; i < 4; ++i) {
        sprite.y = ys[i];
        if (ids[i] != -1) {
            sprite.icon = static_cast<std::uint16_t>(ids[i]);
            PrepareDrawTextureWithMode(sprite);
        }
    }
    for (std::int32_t i = 0; i < 2; ++i) {
        PawnArrows(589, arrowY[i], arrows & 1);
        arrows >>= 1;
    }
}
void PawnSingleItemDetails(std::int32_t priceCount, const PawnItemPrice *prices, PawnLegacyDrawState *draw, const std::uint16_t *items,
    const std::int16_t *quantities, std::int32_t itemCount, std::int32_t cursor) {
    LegacySpritePacket packet;
    LegacyIconSprite sprite;
    std::int16_t label;
    Vector2 position;
    std::int32_t index;
    if (priceCount < 0 || !prices || !draw || !items || !quantities || itemCount <= 0 || cursor < 0)
        return;
    index = cursor % 10;
    PAWN_INIT_COLOUR_PACKET(packet, 0xa0001860U);
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 110, 404, 376, 40);
    DrawLegacyColourPacket(packet, 2.1f);
    packet.colour = 0xd0001860U;
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 106, 400, 384, 4);
    DrawLegacyColourPacket(packet, 2.2f);
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 106, 404, 4, 40);
    DrawLegacyColourPacket(packet, 2.2f);
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 486, 404, 4, 40);
    DrawLegacyColourPacket(packet, 2.2f);
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 106, 444, 384, 5);
    DrawLegacyColourPacket(packet, 2.2f);
    sprite.flags = 0;
    sprite.rotation = 0;
    sprite.scaleX = 1;
    sprite.scaleY = 1;
    sprite.depth = 2.3f;
    sprite.alignment = 0;
    if (index == 9) {
        if (draw->actionLabels[4] == -1)
            draw->actionLabels[4] = SetupUIElement(
                static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, "\xb9\xe7\xb7\xd7\xa1\xa7", 256, 0xffff, 0xf000, 1);
        sprite.x = 126;
        sprite.y = 408;
        sprite.width = 300;
        sprite.height = 32;
        if (draw->actionLabels[4] != -1) {
            sprite.icon = static_cast<std::uint16_t>(draw->actionLabels[4]);
            PrepareDrawTextureWithMode(sprite);
        }
        std::int32_t total = static_cast<std::int32_t>(CalculatePawnSelectedItemsPrice(priceCount, prices, items, quantities, itemCount));
        if (total < 0)
            total = 0;
        position.x = 254;
        position.y = 424;
        DrawMoneyAmount(&position, 2.3f, total, 2, 0xff656565U, 1.2f);
    } else {
        index += 9 * (cursor / 10);
        const std::uint32_t item = static_cast<std::int32_t>(static_cast<std::int16_t>(items[index]));
        const std::int32_t remaining = static_cast<std::int32_t>(GetItemState(item)) - quantities[index];
        std::uint32_t colour;
        if (quantities[index] != -256) {
            position.x = 126;
            position.y = 424;
            std::int32_t price = 0;
            for (std::int32_t row = 0; row < priceCount; ++row)
                if (static_cast<std::int16_t>(prices[row].item) == static_cast<std::int32_t>(item)) {
                    price = prices[row].price;
                    break;
                }
            DrawMoneyAmount(&position, 2.3f, price, 2, 0xff656565U, 1.3f);
            DrawLegacyItemQuantity(474.48f, 424, remaining, 2.3f);
            colour = 0x656565U;
        } else {
            PAWN_CLEAR_PACKET(packet);
            packet.colour = 0xffffffffU;
            packet.quad.flags |= 0x400U;
            TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(5), 150, 424, 64, 28, 1, 1, 0);
            packet.uv[0] = .5f;
            packet.uv[1] = .3359375f;
            packet.uv[2] = 1;
            packet.uv[3] = .5f;
            DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(draw->background), 2.3f);
            colour = 0x2b1552U;
        }
        PrepareLegacyInventoryItemLabel(&label, item);
        if (label != -1) {
            position.x = 298;
            position.y = 424;
            DrawLegacyItemLabel(&label, &position, 2.3f, colour);
        }
    }
}
void PawnSingleItems(const PawnLegacyDrawState *draw, std::int32_t cursor, const std::int16_t *items, const std::int16_t *quantities, std::int32_t itemCount) {
    LegacyIconSprite sprite;
    LegacySpritePacket packet;
    if (!draw && !items && !quantities && itemCount <= 0)
        return;
    const std::int32_t visibleHeight = draw->bottom - draw->top - 101;
    const std::int32_t offset = draw->scroll - 94;
    const std::int32_t row = offset >= 0 ? offset / 52 : 0;
    std::int32_t y = row * 52 - offset;
    std::int32_t first = row * 9;
    std::int32_t column = 0;
    InitializeLegacyIconSprite(sprite);
    while (y < visibleHeight && column + first < itemCount) {
        const std::int16_t itemId = items[column + first];
        sprite.depth = 1.0f;
        sprite.icon = GetInventoryItemIconById(itemId);
        sprite.x = static_cast<float>(60 * column + 32);
        sprite.width = 40;
        sprite.height = 40;
        if (y < 0) {
            sprite.alignment = 8;
            sprite.y = static_cast<float>(draw->top + 39 + y);
        } else {
            sprite.alignment = 0;
            sprite.y = static_cast<float>(draw->top + y);
        }
        std::uint32_t iconColour = 0x80101020U;
        if (quantities[column + first] != -256)
            iconColour = 0xff000000U;
        PrepareDrawSimple(sprite, iconColour);
        std::uint32_t underline, boxColour;
        if ((first / 9) * 10 + column == cursor) {
            underline = PawnPulseColour(0x3b8c0000U, 0x208c0000U, draw->frame);
            if (quantities[column + first] == -256)
                underline = 0;
            boxColour = underline;
        } else {
            underline = quantities[column + first] == -256 ? 0 : 0xff000000U;
            boxColour = 0x80802010U;
        }
        if (quantities[column + first] > 0) {
            DrawMoneyDigits(static_cast<float>(60 * column + 72), static_cast<float>(draw->top + y + 32), 1.0f, 1.0f, quantities[column + first], 0xff656565U);
            INIT_LEGACY_SPRITE_PACKET(packet);
            packet.colour = boxColour;
            packet.quad.flags |= 0x400U;
            AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 60 * column + 32, draw->top + y, 60, 40);
            DrawLegacyColourPacket(packet, .9f);
        }
        PawnUnderline(draw, 60 * column + 32, draw->top + 28 + y, 56, 4, 1.0f, underline);
        if (++column == 9) {
            y += 52;
            column = 0;
            first += 9;
        }
    }
}
void InitPawnshopUIElement(PawnDataHeader *data, shen_name shopName, PawnLegacyDrawState *draw, const std::uint16_t *items, const std::int16_t *quantities,
    std::int32_t cursor, std::int32_t itemCount, std::int32_t view) {
    LegacyIconSprite sprite;
    const PawnShopRecord *shop;
    while (data && (shop = FindPawnShop(data, shopName)) != 0 && items && quantities) {
        const std::int32_t visibleHeight = draw->bottom - draw->top - 101;
        PawnPaneBackground(draw, 102);
        const std::int32_t column = cursor % 10;
        const std::int32_t rowY = 52 * (cursor / 10);
        std::int32_t scroll = draw->targetScroll;
        scroll -= 94;
        std::int32_t y = rowY - scroll;
        std::uint32_t arrows = 0;
        if (column == 9) {
            if (y < visibleHeight / 2)
                arrows = 1;
            else
                arrows = 2;
        } else {
            const std::uint32_t pulse = PawnPulseColour(0x3b8c0000U, 0x208c0000U, draw->frame);
            const std::int32_t x = 60 * column + 32;
            y += draw->top;
            InitializeLegacyIconSprite(sprite);
            sprite.depth = 1.9f;
            sprite.icon = 0x405;
            sprite.x = static_cast<float>(x);
            sprite.y = static_cast<float>(y);
            sprite.width = 56;
            sprite.height = 38;
            sprite.alignment = 0;
            PrepareDrawSimple(sprite, pulse | 0xff000000U);
        }
        PawnSingleItemDetails(shop->itemCount, reinterpret_cast<const PawnItemPrice *>(static_cast<std::uintptr_t>(shop->itemsOffset)), draw, items,
            quantities, itemCount, cursor);
        PawnSingleItems(draw, cursor, reinterpret_cast<const std::int16_t *>(items), quantities, itemCount);
        PawnActionButtons(draw, arrows);
        break;
    }
}

void PawnCollectedIcon(const PawnLegacyDrawState *draw, std::int32_t x, std::int32_t y) {
    LegacySpritePacket packet;
    std::int32_t alignment, drawY;
    if (!draw || draw->background == -1)
        return;
    if (y < 32) {
        drawY = y + 31;
        alignment = 8;
    } else {
        drawY = y;
        alignment = 0;
    }
    INIT_LEGACY_SPRITE_PACKET(packet);
    packet.quad.flags |= 0x400U;
    TransformScreenSpace(packet.quad, static_cast<ScreenAlignment>(alignment), x, drawY, 32, 32, 1.0f, 1.0f, 0);
    packet.uv[0] = .5f;
    packet.uv[1] = 0.0f;
    packet.uv[2] = .75f;
    packet.uv[3] = .25f;
    DrawLegacySpritePacket(packet, static_cast<std::uint32_t>(draw->background), 1.8f);
}
void PawnSets(const PawnShopRecord *shop, PawnLegacyDrawState *draw, const std::int32_t *setOrder, PawnStockCount *scratch, std::int32_t cursor,
    std::int32_t view);
void DrawPawnSetPanes(PawnDataHeader *data, shen_name shopName, PawnLegacyDrawState *draw, const std::int32_t *setOrder, PawnStockCount *scratch,
    std::int32_t cursor, std::int32_t view) {
    const PawnShopRecord *shop;
    while (data && (shop = FindPawnShop(data, shopName)) != 0 && draw && draw->background != -1 && draw->scroll >= 0 && setOrder && cursor >= 0) {
        PawnPaneBackground(draw, 32);
        PawnSets(shop, draw, setOrder, scratch, cursor, view);
        break;
    }
}
void PawnSets(const PawnShopRecord *shop, PawnLegacyDrawState *draw, const std::int32_t *setOrder, PawnStockCount *scratch, std::int32_t cursor,
    std::int32_t view) {
    LegacyIconSprite sprite, itemSprite;
    Vector2 position;
    if (!shop || !draw || !setOrder || draw->scroll < 0)
        return;
    const std::uint32_t fill = PawnPulseColour(0x3b8c0000U, 0x208c0000U, draw->frame);
    const std::uint32_t border = PawnPulseColour(0xffc04040U, 0x80c04040U, draw->frame);
    std::uint32_t labelColour = PawnPulseColour(0xffffffffU, 0x00ffff20U, draw->frame);
    const PawnSetRecord *set;
    std::int32_t ordinal = 0, y = 110;
    while (setOrder[ordinal] >= 0) {
        set = GetPawnShopSet(shop, setOrder[ordinal]);
        if (set) {
            const std::int32_t height = 47 * ((set->itemCount + 9) / 10) + 16;
            if (y + height > draw->scroll) {
                y -= draw->scroll;
                break;
            }
            y += height;
        }
        ++ordinal;
    }
    if (setOrder[ordinal] < 0 || ordinal >= shop->setCount)
        return;
    InitializeLegacyIconSprite(itemSprite);
    while (y < draw->bottom && ordinal < shop->setCount) {
        const PawnSetReference *const references = reinterpret_cast<const PawnSetReference *>(static_cast<std::uintptr_t>(shop->setsOffset));
        std::int32_t price;
        set = 0;
        if (references) {
            set = reinterpret_cast<const PawnSetRecord *>(static_cast<std::uintptr_t>(references[setOrder[ordinal]].offset));
            price = static_cast<std::int16_t>(references[setOrder[ordinal]].price);
        }
        ++ordinal;
        if (!set)
            continue;
        const std::int16_t *const items = reinterpret_cast<const std::int16_t *>(set + 1);
        std::int32_t unique = 0, item, index;
        for (item = 0; item < set->itemCount; ++item) {
            for (index = 0; unique > index; ++index)
                if (scratch[index].item == items[item]) {
                    ++scratch[index].remaining;
                    break;
                }
            if (unique <= index) {
                scratch[unique].item = items[item];
                scratch[unique].remaining = 1;
                ++unique;
            }
        }
        std::int32_t available = 1;
        for (index = 0; index < unique; ++index) {
            const std::int32_t count = static_cast<std::int32_t>(GetItemState(scratch[index].item));
            if (scratch[index].remaining > count)
                available = 0;
            scratch[index].remaining = count;
        }
        const char *const label = set->label;
        std::uint32_t priceColour, underlineColour;
        if (available) {
            priceColour = 0xff656565U;
            underlineColour = 0xff000000U;
        } else {
            priceColour = 0xff2b1552U;
            underlineColour = 0;
        }
        std::int32_t slot;
        for (slot = 0; slot < 20; ++slot)
            if (draw->primaryLabels[slot] != -1 && (draw->labelOwners[slot] & 0x7fffffffU) == (reinterpret_cast<std::uintptr_t>(label) & 0x7fffffffU))
                break;
        if (slot == 20) {
            for (slot = 0; slot < 20; ++slot)
                if (draw->primaryLabels[slot] == -1)
                    break;
            if (slot == 20)
                return;
            if (available) {
                draw->primaryLabels[slot] = SetupUIElement(PawnWindowTextTask(), label, 256, 0xffff, 0xf000, 1);
                draw->secondaryLabels[slot] = SetupUIElement(PawnWindowTextTask(), label, 256, 0xfff2, 0xf000, 1);
            } else {
                draw->primaryLabels[slot] = SetupUIElement(PawnWindowTextTask(), label, 256, 0xf315, 0xf000, 1);
                draw->secondaryLabels[slot] = -1;
            }
            draw->labelOwners[slot] = reinterpret_cast<std::uintptr_t>(label) & 0x7fffffffU;
            if (draw->primaryLabels[slot] < 0) {
                draw->primaryLabels[slot] = -1;
                continue;
            }
        }
        if (draw->primaryLabels[slot] != -1) {
            draw->labelOwners[slot] |= 0x80000000U;
            sprite.x = static_cast<float>(draw->left + 20);
            sprite.scaleX = sprite.scaleY = 1;
            sprite.depth = 1.1f;
            sprite.flags = 0;
            sprite.rotation = 0;
            sprite.icon = static_cast<std::uint16_t>(draw->primaryLabels[slot]);
            sprite.width = 256;
            sprite.height = 32;
            if (y < 32) {
                sprite.y = static_cast<float>(draw->top + y + 31);
                sprite.alignment = 8;
            } else {
                sprite.y = static_cast<float>(draw->top + y);
                sprite.alignment = 0;
            }
            PrepareDrawTextureWithMode(sprite);
        }
        if (draw->secondaryLabels[slot] != -1) {
            draw->labelOwners[slot] |= 0x80000000U;
            sprite.x = static_cast<float>(draw->left + 20);
            sprite.scaleX = sprite.scaleY = 1;
            sprite.depth = 1.11f;
            sprite.flags = 0;
            sprite.rotation = 0;
            sprite.icon = static_cast<std::uint16_t>(draw->secondaryLabels[slot]);
            sprite.width = 256;
            sprite.height = 32;
            if (y < 32) {
                sprite.y = static_cast<float>(draw->top + y + 31);
                sprite.alignment = 8;
            } else {
                sprite.y = static_cast<float>(draw->top + y);
                sprite.alignment = 0;
            }
            if (available) {
                labelColour = (labelColour & 0xff000000U) | 0x00ffffffU;
                PrepareDrawTextureWithColour(sprite, labelColour);
            }
        }
        position.x = static_cast<float>(draw->left + 204);
        position.y = static_cast<float>(draw->top + 38 + y);
        if (set->itemCount > 10) {
            position.y += 32.0f;
            if (__builtin_fabs(position.y) < 1.1920929e-7f)
                position.y = 0;
        }
        DrawMoneyAmount(&position, 1.1f, price, 2, priceColour, 1);
        if (!view && ordinal - 1 == cursor)
            PawnHighlight(draw->left + 12, y + 12, 240, 50, 3, 1.9f, fill, border);
        for (item = 0; item < set->itemCount; ++item) {
            for (index = 0; index < unique; ++index)
                if (items[item] == scratch[index].item) {
                    if (--scratch[index].remaining >= 0)
                        PawnCollectedIcon(draw, draw->left + 250 + 36 * (item % 10) + 4, y + 20);
                    break;
                }
            itemSprite.depth = 1.1f;
            itemSprite.icon = GetInventoryItemIconById(items[item]);
            itemSprite.x = static_cast<float>(draw->left + 250 + 36 * (item % 10));
            itemSprite.width = 40;
            itemSprite.height = 40;
            if (y < 40) {
                itemSprite.alignment = 8;
                itemSprite.y = static_cast<float>(y + 55);
            } else {
                itemSprite.alignment = 0;
                itemSprite.y = static_cast<float>(y + 16);
            }
            PrepareDrawSimple(itemSprite, 0xff000000U);
            if (item != 0 && (item + 1) % 10 == 0)
                y += 47;
        }
        if (item != 0 && item % 10 != 0)
            y += 47;
        PawnUnderline(draw, 16, y, draw->right - draw->left - 23, 5, 1.1f, underlineColour);
        y += 16;
    }
}



}
// @verified - d
void PawnShopDraw(void *) {
    PawnShopRuntime *runtime = &pawnShop;
    PawnLegacyDrawState *draw;
    std::int32_t index;
    if (!runtime || !(runtime->selection.flags & 0x80000000U))
        return;
    draw = &runtime->draw;
    if (draw->background == -1)
        return;
    for (index = 0; index < 20; ++index)
        draw->labelOwners[index] &= 0x7fffffffU;
    draw->frame = static_cast<std::int32_t>(static_cast<std::uint32_t>(draw->frame) + 1);
    if (draw->targetScroll != draw->previousTarget) {
        draw->previousTarget = draw->targetScroll;
        draw->scrollStart = draw->scroll;
        if (draw->interpolationFrames < 5)
            draw->interpolationFrames = 10;
    }
    if (draw->scroll != draw->targetScroll) {
        if (draw->interpolationFrames > 0) {
            std::int32_t delta = draw->targetScroll;
            if (draw->interpolationFrames == 1) {
                delta -= draw->scroll;
                if (static_cast<std::uint32_t>(delta + 32) > 64U) {
                    const std::int32_t step = delta > 0 ? 32 : -32;
                    draw->scroll += step;
                } else {
                    --draw->interpolationFrames;
                    draw->scroll = draw->targetScroll;
                }
            } else {
                delta -= draw->scrollStart;
                std::int32_t weight = draw->interpolationFrames;
                if (weight > 5)
                    weight = 11 - weight;
                draw->scroll += weight * delta / 30;
                --draw->interpolationFrames;
            }
        } else
            draw->interpolationFrames = 0;
    }
    if (runtime->selection.flags & 2)
        DrawPawnSetPanes(runtime->data, runtime->shopName, &runtime->draw, runtime->setOrder, runtime->stockScratch, runtime->setCursor,
            runtime->selection.flags & 1);
    else
        InitPawnshopUIElement(runtime->data, runtime->shopName, &runtime->draw, runtime->items, runtime->quantities, runtime->itemCursor, runtime->itemCount,
            runtime->selection.flags & 1);
    for (index = 0; index < 20; ++index)
        if (!(runtime->draw.labelOwners[index] & 0x80000000U)) {
            if (runtime->draw.primaryLabels[index] != -1) {
                ReleaseNamedTexture_Wrapper(runtime->draw.primaryLabels[index]);
                runtime->draw.primaryLabels[index] = -1;
            }
            if (runtime->draw.secondaryLabels[index] != -1) {
                ReleaseNamedTexture_Wrapper(runtime->draw.secondaryLabels[index]);
                runtime->draw.secondaryLabels[index] = -1;
            }
            runtime->draw.labelOwners[index] = 0;
        }
}
