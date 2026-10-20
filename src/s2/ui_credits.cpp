#include "s2/ui_credits.hpp"
#include "s2/ui_internal.hpp"
#include "s2/renderer_scratch.hpp"
#include "d3t/object_store.hpp"

namespace shenmue::s2::ui_detail {
CreditsState creditsState{.textColour = 0xffffffffU};
CreditsLegacyParameters creditsLegacyParameters{};
d3t::UIWindow *creditsScrollSpace{};
UIElementState *creditsElement{};

float LerpEaseInOutQuad(float start, float end, float t) {
    const float twice = t + t;
    const float half = (end - start) * 0.5f;
    if (twice >= 1.0f)
        return start - half * (((twice - 1.0f) - 2.0f) * (twice - 1.0f) - 1.0f);
    return half * twice * twice + start;
}

std::uint8_t CreditsAlphaByte(float scaled) {
    const std::int32_t value = static_cast<std::int32_t>(scaled);
    return static_cast<std::uint8_t>(value < 0 ? 0 : (value < 255 ? value : 255));
}

bool UI_SetImageFromTexture(struct nk_image &image, std::uint32_t texture) {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    const bool described = d3t::GetTexture2DDimensions(texture, &width, &height);
    if (!described) {
        texture = d3t::g_defaultWhiteTexture;
        d3t::GetTexture2DDimensions(texture, &width, &height);
    }
    image.handle.id = static_cast<int>(texture);
    image.w = static_cast<nk_ushort>(width);
    image.h = static_cast<nk_ushort>(height);
    image.region[0] = 0;
    image.region[1] = 0;
    image.region[2] = image.w;
    image.region[3] = image.h;
    return described;
}

bool CreditsResolveImage(CreditsImage &entry) {
    if (entry.texture)
        return true;
    entry.texture = GetShaderDataByIndex(entry.request);
    if (!entry.texture)
        return false;
    entry.image = {};
    UI_SetImageFromTexture(entry.image, entry.texture);
    return true;
}

bool CreditsResolveImages(CreditsImage *images, std::uint32_t count) {
    bool ready = true;
    for (std::uint32_t index = 0; index < count; ++index)
        ready = CreditsResolveImage(images[index]) && ready;
    return ready;
}

bool CreditsTexturesReady() {
    const bool sections = CreditsResolveImages(creditsState.sections.data, creditsState.sections.count);
    bool ready = CreditsResolveImage(creditsState.signature) && sections;
    for (CreditsTable &table : creditsState.tables) {
        const std::uint8_t loaded = std::atomic_ref(table.loaded).load(std::memory_order_acquire);
        ready = ready && loaded;
        if (loaded)
            ready = CreditsResolveImages(table.images.data, table.images.count) && ready;
    }
    return ready;
}

void CreditsLoadImage(CreditsTable &table, CreditsCell &cell, const char *path) {
    if (table.images.count >= table.images.capacity) {
        const std::uint32_t capacity = 2U * table.images.capacity >= 16U ? 2U * table.images.capacity : 16U;
        if (capacity > table.images.capacity) {
            CreditsImage *const grown = static_cast<CreditsImage *>(std::realloc(table.images.data, sizeof(CreditsImage) * capacity));
            if (!grown)
                return;
            table.images.data = grown;
            table.images.capacity = capacity;
        }
    }
    CreditsImage &entry = table.images.data[table.images.count++];
    entry = {};
    std::array<char, 260> localized{};
    if (g_s2JapaneseVersion != 0) {
        std::size_t length = std::strlen(path);
        while (length > 1 && static_cast<std::uint8_t>(path[length - 1] - '.') > 1U)
            --length;
        if (path[length - 1] == '.') {
            std::snprintf(localized.data(), localized.size(), "%.*s_jp.%s?usage=0", static_cast<int>(length - 1), path, path + length);
            if (d3t::AuthoredAssetExists(localized.data()))
                path = localized.data();
        }
    }
    entry.request = SetupUsageForTexture(path, 0);
    cell.kind = CCK_Image;
    cell.value = table.images.count - 1;
}

void CreditsAppendText(CreditsTable &table, CreditsCell &cell, const char *text) {
    cell.kind = CCK_Spacer;
    if (std::strcmp(text, "!!!") == 0)
        return;
    cell.kind = CCK_Text;
    const std::uint32_t size = static_cast<std::uint32_t>(std::strlen(text)) + 1U;
    const std::uint32_t offset = table.text.size;
    const std::uint32_t end = offset + size;
    if (end > table.text.capacity || end < offset) {
        cell.value = 0xffffffffU;
        return;
    }
    table.text.size = end;
    std::memcpy(table.text.data + offset, text, size);
    cell.value = offset;
}

void CreditsParseCell(CreditsTable &table, CreditsCell &cell, const char *start, const char *end) {
    const std::size_t lengthBytes = static_cast<std::size_t>(end - start);
    char *const text = static_cast<char *>(GetRendererScratchBlock(8 * lengthBytes + 8));
    if (!text)
        return;
    std::memcpy(text, start, lengthBytes);
    text[lengthBytes] = '\0';
    for (char *quote = std::strchr(text, '"'); quote; quote = std::strchr(quote, '"')) {
        if (std::strstr(quote, "\"\"\"") == quote)
            ++quote;
        else
            std::memmove(quote, quote + 1, std::strlen(quote));
    }
    std::size_t length = std::strlen(text);
    while (length > 1 && static_cast<std::uint8_t>(text[length - 1] - '.') > 1U)
        --length;
    const char *const extension = length && text[length - 1] == '.' ? text + length : nullptr;
    if (extension && (std::strcmp(extension, "png") == 0 || std::strcmp(extension, "tga") == 0))
        CreditsLoadImage(table, cell, text);
    if (cell.kind == CCK_Empty)
        CreditsAppendText(table, cell, text);
    g_rendererScratch.current = reinterpret_cast<std::byte *>(text);
}

void CreditsParseRow(CreditsTable &table, const char *cursor, const char *end) {
    if (table.rows.count >= table.rows.capacity) {
        const std::uint32_t capacity = 2U * table.rows.capacity >= 16U ? 2U * table.rows.capacity : 16U;
        if (capacity > table.rows.capacity) {
            CreditsRow *const grown = static_cast<CreditsRow *>(std::realloc(table.rows.data, sizeof(CreditsRow) * capacity));
            if (!grown)
                return;
            table.rows.data = grown;
            table.rows.capacity = capacity;
        }
    }
    CreditsRow &row = table.rows.data[table.rows.count++];
    row = {};
    for (std::uint8_t column = 0; cursor < end && column < 4; ++cursor, ++column) {
        bool quoted = false;
        const char *const start = cursor;
        for (; cursor < end; ++cursor) {
            if (*cursor == ',' && !quoted)
                break;
            if (*cursor == '"')
                quoted = !quoted;
        }
        if (cursor - start > 0)
            CreditsParseCell(table, row.cells[column], start, cursor);
    }
}

void AsyncPrepareCreditsCSV(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *) {
    const char *const bytes = static_cast<const char *>(loadedData);
    if (bytes && data) {
        CreditsTable &table = *static_cast<CreditsTable *>(static_cast<UILoadPayload *>(data)->data);
        table.text.data = static_cast<char *>(std::malloc(loadedSize));
        table.text.size = 0;
        table.text.capacity = static_cast<std::uint32_t>(loadedSize);
        const char *line = bytes;
        const char *cursor = bytes;
        if (loadedSize) {
            do {
                std::uint16_t pair;
                std::memcpy(&pair, cursor, sizeof(pair));
                while (pair != 0x0a0d) {
                    if (static_cast<std::uint64_t>(cursor - bytes) >= loadedSize)
                        break;
                    ++cursor;
                    std::memcpy(&pair, cursor, sizeof(pair));
                }
                CreditsParseRow(table, line, cursor);
                cursor += 2;
                line = cursor;
            } while (static_cast<std::uint64_t>(cursor - bytes) < loadedSize);
        }
        std::atomic_ref(table.loaded).store(1, std::memory_order_release);
    }
    d3t::ProcessThreadStates(contentHandle, 0);
}

void StartPrepareCreditsCSV() {
    for (std::size_t index = 0; index < creditsState.tables.size(); ++index) {
        CreditsTable &table = creditsState.tables[index];
        UILoadPayload *const payload = GetUILoadRequest(AllocateUILoadRequest());
        if (!payload)
            continue;
        payload->data = &table;
        char path[272];
        std::snprintf(path, 260, "/ui/Credits/Data/%s.csv", CREDITS_TYPES[index]);
        const d3t::D3TShaderLoadTask task{.path = path,
            .renderHandle = 0,
            .reserved_0c = 0,
            .thread = {.callback = AsyncPrepareCreditsCSV, .data = payload},
            .destination = nullptr,
            .destinationCapacity = 0,
            .option = 0,
            .reserved_31 = {}};
        std::atomic_ref(table.loaded).store(0, std::memory_order_relaxed);
        table.contentHandle = static_cast<std::uint32_t>(d3t::PushShaderLoadToThreadQueue(&task));
    }
}

void ShowCredits() {
    std::array<char, 260> path{};
    std::snprintf(path.data(), path.size(), "/ui/credits/textures/sf_%02u_0.tga?usage=0", creditsState.sections.count + 1U);
    while (d3t::AuthoredAssetExists(path.data())) {
        if (creditsState.sections.count >= creditsState.sections.capacity) {
            const std::uint32_t capacity = 2U * creditsState.sections.capacity >= 16U ? 2U * creditsState.sections.capacity : 16U;
            CreditsImage *const grown = capacity > creditsState.sections.capacity ?
                static_cast<CreditsImage *>(std::realloc(creditsState.sections.data, sizeof(CreditsImage) * capacity)) :
                nullptr;
            if (grown) {
                creditsState.sections.data = grown;
                creditsState.sections.capacity = capacity;
            }
        }
        if (creditsState.sections.count < creditsState.sections.capacity) {
            CreditsImage &entry = creditsState.sections.data[creditsState.sections.count++];
            entry = {};
            std::array<char, 256> request{};
            if (std::strchr(path.data(), '?'))
                std::snprintf(request.data(), request.size(), "%s", path.data());
            else
                std::snprintf(request.data(), request.size(), "%s%s", path.data(), "?usage=0");
            entry.request = SetupUsageForTexture(request.data(), 0);
        }
        std::snprintf(path.data(), path.size(), "/ui/credits/textures/sf_%02u_0.tga?usage=0", creditsState.sections.count + 1U);
    }
    creditsState.signature = {};
    creditsState.signature.request =
        SetupUsageForTexture(g_s2JapaneseVersion ? "/ui/credits/textures/yssignature_jp.tga" : "/ui/credits/textures/yssignature.tga", 0);
}

void ReleaseCreditsData() {
    d3t::InitTexture2DResource(creditsState.signature.texture);
    for (std::uint32_t index = 0; index < creditsState.sections.count; ++index)
        d3t::InitTexture2DResource(creditsState.sections.data[index].texture);
    std::free(creditsState.sections.data);
    creditsState.sections.data = nullptr;
    creditsState.sections.count = 0;
    creditsState.sections.capacity = 0;
    for (CreditsTable &table : creditsState.tables) {
        std::atomic_ref(table.loaded).store(0, std::memory_order_relaxed);
        for (std::uint32_t index = 0; index < table.images.count; ++index)
            d3t::InitTexture2DResource(table.images.data[index].texture);
        std::free(table.images.data);
        table.images.data = nullptr;
        table.images.count = 0;
        table.images.capacity = 0;
        std::free(table.rows.data);
        table.rows.data = nullptr;
        table.rows.count = 0;
        table.rows.capacity = 0;
        std::free(table.text.data);
        table.text.size = 0;
        table.text.capacity = 0;
    }
}

float CreditsRowHeight(const CreditsTable &table, const CreditsRow &row) {
    const float fontHeight = GNuklearWindow.style.font->height;
    float height = fontHeight - 12.0f;
    for (const CreditsCell &cell : row.cells) {
        if (cell.kind == CCK_Image) {
            if (cell.value >= table.images.count || !table.images.data[cell.value].texture)
                continue;
            const float imageHeight = static_cast<float>(table.images.data[cell.value].image.region[3]);
            if (height < imageHeight)
                height = imageHeight;
        } else if (cell.kind == CCK_Text && height < fontHeight) {
            height = fontHeight;
        }
    }
    return height;
}

void CreditsMeasureTables() {
    if (!creditsScrollSpace)
        return;
    UILayoutRect rect{};
    UI_GetWindowRect(creditsScrollSpace, rect);
    for (CreditsTable &table : creditsState.tables) {
        table.height = 0.0f;
        for (std::uint32_t row = 0; row < table.rows.count; ++row)
            table.height = (CreditsRowHeight(table, table.rows.data[row]) + table.height) + 12.0f;
        table.height = rect.height + table.height;
    }
}

void CreditsInitScroll() {
    CVS_CtrlConversTask_Enqueue(true);
    creditsState.voice = shenPlayAudio("stuffroll", {}, 17, false);
    CreditsMeasureTables();
    creditsState.scrollInitialized = 1;
    creditsState.scrollProgress = 0.0f;
    creditsState.scrollSpeed = ((creditsState.tables[0].height + 0.0f) + creditsState.tables[1].height + creditsState.tables[2].height) /
        (CREDITS_DURATION - CREDITS_SIGNATURE_DURATION);
}

void CreditsSkipTable() {
    float progress = creditsState.scrollProgress;
    std::uint32_t table = 0;
    if (progress > creditsState.tables[0].height) {
        float height = creditsState.tables[0].height;
        do {
            ++table;
            progress -= height;
            const std::byte *const bank = reinterpret_cast<const std::byte *>(&creditsState);
            std::memcpy(&height, bank + static_cast<std::uint32_t>(table * sizeof(CreditsTable)) + offsetof(CreditsTable, height), sizeof(height));
        } while (progress > height);
    }
    if (table < creditsState.tables.size()) {
        float start = 0.0f;
        for (std::uint32_t index = 0; index <= table; ++index) {
            start += creditsState.tables[index].height;
            creditsState.scrollProgress = start;
        }
    }
    creditsState.fadeOutRequested = 0;
    if (table == 2)
        creditsState.scrollProgress =
            creditsState.scrollSpeed * 10.0f + ((creditsState.tables[0].height + 0.0f) + creditsState.tables[1].height + creditsState.tables[2].height);
}

void CreditsPlaceItem(float x, float y, float width, float height) {
    nk_context &context = GNuklearWindow;
    if (g_waitingForNextTick == 1 && context.current && context.current->layout)
        context.current->layout->row.item = nk_rect(x, y, width, height);
}

void CreditsDrawSectionImage(const CreditsImage &entry, const UILayoutRect &rect, float alpha) {
    if (!entry.texture)
        return;
    const float width = static_cast<float>(entry.image.region[2]);
    const float height = static_cast<float>(entry.image.region[3]);
    CreditsPlaceItem(rect.x + (rect.width - width) * 0.5f, rect.y + (rect.height - height) * 0.5f, width, height);
    if (g_waitingForNextTick == 1)
        nk_image_color(&GNuklearWindow, entry.image, nk_rgba(255, 255, 255, CreditsAlphaByte(alpha * 255.0f)));
}

void CreditsDrawSection(const UILayoutRect &rect) {
    const float total = (creditsState.tables[0].height + 0.0f) + creditsState.tables[1].height + creditsState.tables[2].height;
    if (!(total > creditsState.scrollProgress))
        return;
    const float count = static_cast<float>(static_cast<std::int32_t>(creditsState.sections.count));
    const std::uint32_t index = static_cast<std::uint32_t>(static_cast<std::int32_t>((creditsState.scrollProgress / total) * count));
    if (index >= creditsState.sections.count)
        return;
    const float share = total / count;
    float t = (creditsState.scrollProgress - static_cast<float>(static_cast<std::int32_t>(index)) * share) / share;
    float fade = 1.0f;
    if (t < 0.25f) {
        fade = LerpEaseInOutQuad(0.0f, 1.0f, t * 4.0f);
    } else if (t > 0.75f) {
        t = t - 0.75f;
        fade = LerpEaseInOutQuad(1.0f, 0.0f, t * 4.0f);
    }
    CreditsDrawSectionImage(creditsState.sections.data[index], rect, creditsState.fadeAlpha * fade);
}

void CreditsDrawSignature(const UILayoutRect &rect) {
    const float total = (creditsState.tables[0].height + 0.0f) + creditsState.tables[1].height + creditsState.tables[2].height;
    if (!(creditsState.scrollProgress > total))
        return;
    float fade = std::fminf(1.0f, (creditsState.scrollProgress - total) / creditsState.scrollSpeed);
    const float fadeOut = (creditsState.scrollSpeed * 10.0f + total) - creditsState.scrollSpeed;
    if (creditsState.scrollProgress > fadeOut)
        fade = fade * (1.0f - (creditsState.scrollProgress - fadeOut) / creditsState.scrollSpeed);
    const float alpha = fade * creditsState.fadeAlpha;
    if (!creditsState.signature.texture)
        return;
    const float width = static_cast<float>(creditsState.signature.image.region[2]);
    const float height = static_cast<float>(creditsState.signature.image.region[3]);
    CreditsPlaceItem(rect.x + (rect.width - width) * 0.5f, rect.y + (rect.height - height) * 0.5f, width, height);
    if (g_waitingForNextTick == 1)
        nk_image_color(&GNuklearWindow, creditsState.signature.image, nk_rgba(255, 255, 255, CreditsAlphaByte(alpha * 255.0f)));
}

void CreditsDrawImage(const CreditsTable &table, const CreditsCell &cell, const UILayoutRect &rect, float alpha) {
    if (cell.value >= table.images.count)
        return;
    const CreditsImage &entry = table.images.data[cell.value];
    if (!entry.texture)
        return;
    const float width = static_cast<float>(entry.image.region[2]);
    CreditsPlaceItem(rect.x + (rect.width - width) * 0.5f, rect.y, width, static_cast<float>(entry.image.region[3]));
    nk_context &context = GNuklearWindow;
    if (context.current && context.current->layout)
        nk_image_color(&context, entry.image, nk_rgba(255, 255, 255, CreditsAlphaByte(alpha * 255.0f)));
}

void CreditsDrawText(
    const CreditsTable &table, const CreditsCell &cell, const UILayoutRect &rect, std::int32_t columns, std::int32_t column, float alpha, float rowHeight) {
    if (cell.value >= table.text.size)
        return;
    const char *const text = table.text.data + cell.value;
    const std::size_t length = std::strlen(text);
    if (!length)
        return;
    nk_flags alignment = NK_TEXT_ALIGN_CENTERED;
    if (columns == 2)
        alignment = column == 0 ? NK_TEXT_ALIGN_RIGHT : NK_TEXT_ALIGN_LEFT;
    nk_context &context = GNuklearWindow;
    const float x = rect.x + 2.0f;
    const float y = (rect.y + (rowHeight - context.style.font->height) * 0.5f) + 2.0f;
    const std::uint32_t red = creditsState.textColour & 0xffU;
    const std::uint32_t green = (creditsState.textColour >> 8U) & 0xffU;
    const std::uint32_t blue = (creditsState.textColour >> 16U) & 0xffU;
    const float colourAlpha = static_cast<float>(creditsState.textColour >> 24U);
    const std::uint8_t shadowAlpha = CreditsAlphaByte(alpha * 200.0f);
    context.style.text.color = nk_rgba(static_cast<int>(static_cast<float>(red) * 0.0f), static_cast<int>(static_cast<float>(green) * 0.0f),
        static_cast<int>(static_cast<float>(blue) * 0.0f), static_cast<int>((static_cast<float>(shadowAlpha) / 255.0f) * colourAlpha));
    if (g_waitingForNextTick == 1) {
        CreditsPlaceItem(x, y, rect.width, rect.height);
        nk_text_colored(&context, text, static_cast<int>(length), alignment, context.style.text.color);
    }
    const std::uint8_t textAlpha = CreditsAlphaByte(alpha * 255.0f);
    context.style.text.color = nk_rgba(
        static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue), static_cast<int>((static_cast<float>(textAlpha) / 255.0f) * colourAlpha));
    if (g_waitingForNextTick == 1) {
        CreditsPlaceItem(x - 2.0f, y - 2.0f, rect.width, rect.height);
        nk_text_colored(&context, text, static_cast<int>(length), alignment, context.style.text.color);
    }
}

void ApplyFadeInOut(const CreditsTable &table, const CreditsRow &row, UILayoutRect &rect) {
    const float rowHeight = CreditsRowHeight(table, row);
    std::int32_t columns = row.cells[0].kind != CCK_Empty ? 1 : 0;
    if (row.cells[1].kind)
        columns = 2;
    if (row.cells[2].kind)
        columns = 3;
    if (row.cells[3].kind)
        columns = 4;
    if (columns && !(rect.y + rowHeight < 0.0f)) {
        const float bottom = rect.y + rowHeight;
        float fade = 1.0f;
        if (bottom < 128.0f)
            fade = LerpEaseInOutQuad(0.0f, 1.0f, bottom * 0.0078125f);
        else if (rect.y > rect.height - 128.0f)
            fade = LerpEaseInOutQuad(1.0f, 0.0f, (rect.y - (rect.height - 128.0f)) * 0.0078125f);
        const float alpha = fade * creditsState.fadeAlpha;
        const float columnWidth = rect.width / static_cast<float>(columns);
        float x = rect.x;
        for (std::int32_t column = 0; column < columns; ++column) {
            const UILayoutRect cellRect{.x = x + 16.0f, .y = rect.y, .width = columnWidth - 32.0f, .height = rect.height};
            const CreditsCell &cell = row.cells[static_cast<std::size_t>(column)];
            if (cell.kind == CCK_Image)
                CreditsDrawImage(table, cell, cellRect, alpha);
            else if (cell.kind == CCK_Text)
                CreditsDrawText(table, cell, cellRect, columns, column, alpha, rowHeight);
            x += columnWidth;
        }
    }
    rect.y = (rowHeight + 12.0f) + rect.y;
}

void CreditsDrawNames(UILayoutRect &roll) {
    roll.y = (roll.y + roll.height) - creditsState.scrollProgress;
    for (CreditsTable &table : creditsState.tables) {
        if (table.loaded) {
            for (std::uint32_t row = 0; row < table.rows.count; ++row) {
                ApplyFadeInOut(table, table.rows.data[row], roll);
                if (roll.y > roll.height)
                    break;
            }
        }
        roll.y = roll.y + roll.height;
    }
}

std::int64_t BindEndingCredits() {
    const std::int32_t window = UI_FindViewWindow(static_cast<std::uint32_t>(creditsElement->config.uiHandleIndex), "Credits::ScrollSpace");
    creditsScrollSpace = UI_GetWindow(window);
    return 1;
}

std::int64_t ResetEndingCredits() {
    creditsScrollSpace = nullptr;
    return 0;
}

std::int64_t HandleEndingCreditsInput() {
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    if (!g_s2JapaneseVersion && !creditsState.fadeOutRequested && (pressed & CREDITS_SKIP_INPUT))
        creditsState.fadeOutRequested = 1;
    return 0;
}

std::int64_t InitializeEndingCredits() {
    shenSetForcedFilmbars(1);
    ShowCredits();
    StartPrepareCreditsCSV();
    creditsState.scrollProgress = 0.0f;
    creditsState.fadeAlpha = 1.0f;
    creditsState.fadeOutRequested = 0;
    creditsState.scrollInitialized = 0;
    return 0;
}

std::int64_t ReleaseEndingCredits() {
    shenSetForcedFilmbars(0);
    ReleaseCreditsData();
    if (creditsState.voice)
        hsFadeVoice(creditsState.voice, 60);
    return 1;
}

std::int64_t UpdateEndingCredits() {
    Set60FPSForCredits();
    nk_context &context = GNuklearWindow;
    const UIElementState &element = *creditsElement;
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    DrawUIEntity(context, slot, element.index);
    if (!CreditsTexturesReady())
        return 0;
    if (g_uiFontFace != 0) {
        g_uiFontFace = 0;
        UI_SelectFont();
    }
    if (g_uiFontSize != 4) {
        g_uiFontSize = 4;
        UI_SelectFont();
    }
    if (!creditsState.scrollInitialized)
        CreditsInitScroll();
    creditsState.scrollProgress = creditsState.scrollProgress + creditsState.scrollSpeed * 0.016666668f;
    const float alpha = creditsState.fadeAlpha;
    if (creditsState.fadeOutRequested) {
        creditsState.fadeAlpha = creditsState.fadeAlpha + -0.016666668f;
        if (alpha + -0.016666668f <= 0.0f)
            CreditsSkipTable();
    } else {
        creditsState.fadeAlpha = creditsState.fadeAlpha + 0.016666668f;
        if (alpha + 0.016666668f > 1.0f)
            creditsState.fadeAlpha = 1.0f;
    }
    if (!creditsScrollSpace)
        return 0;
    UILayoutRect rect{};
    UI_GetWindowRect(creditsScrollSpace, rect);
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(&context, &creditsState.panel, "UICredits", "UICredits", nk_rect(rect.x, rect.y, rect.width, rect.height), NK_WINDOW_NO_SCROLLBAR);
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, 1);
    }
    CreditsDrawSignature(rect);
    CreditsDrawSection(rect);
    UILayoutRect roll = rect;
    CreditsDrawNames(roll);
    if (g_waitingForNextTick == 1 && context.current) {
        if (context.current->layout) {
            nk_row_layout &layout = context.current->layout->row;
            layout.item_width = 0.0f;
            layout.item_height = 0.0f;
            layout.item_offset = 0.0f;
            layout.item = nk_rect(0.0f, 0.0f, 0.0f, 0.0f);
        }
        if (!(context.current->flags & NK_WINDOW_HIDDEN))
            nk_end(&context);
        context.current = nullptr;
    }
    return 0;
}

void InitializeEndingCreditsElement(UIElementState &element) {
    creditsElement = &element;
    element.isSelected = BindEndingCredits;
    element.parseDefinitionFromFile = LoadEndingCredits;
    element.reset = ResetEndingCredits;
    element.render = UpdateEndingCredits;
    element.tick = InitializeEndingCredits;
    element.destroy = ReleaseEndingCredits;
    element.isPressed = HandleEndingCreditsInput;
    element.name = "Credits";
    element.fieldAc = 1;
}
} // namespace shenmue::s2::ui_detail
