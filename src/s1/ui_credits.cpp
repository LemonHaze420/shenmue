#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {
float LerpEaseInOutQuad(float start, float end, float t);

std::uint8_t CreditsAlphaByte(float scaled);

bool UI_SetImageFromTexture(struct nk_image &image, std::uint32_t texture);

bool CreditsResolveImage(CreditsImage &entry);

bool CreditsResolveImages(CreditsImage *images, std::uint32_t count);

bool CreditsTexturesReady();

void CreditsLoadImage(CreditsTable &table, CreditsCell &cell, const char *path);

void CreditsAppendText(CreditsTable &table, CreditsCell &cell, const char *text);

void CreditsParseCell(CreditsTable &table, CreditsCell &cell, const char *start, const char *end);

void CreditsParseRow(CreditsTable &table, const char *cursor, const char *end);

void AsyncPrepareCreditsCSV(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *);

void StartPrepareCreditsCSV();

void ShowCredits();

void CreateTextureForCredits();

float CreditsRowHeight(const CreditsTable &table, const CreditsRow &row);

void CreditsMeasureTables();

void CreditsInitScroll();

void CreditsSkipTable();

void CreditsPlaceItem(float x, float y, float width, float height);

void CreditsDrawSectionImage(const CreditsImage &entry, const UILayoutRect &rect, float alpha);

void CreditsDrawSection(const UILayoutRect &rect);

void CreditsDrawSignature(const UILayoutRect &rect);

void CreditsDrawImage(const CreditsTable &table, const CreditsCell &cell, const UILayoutRect &rect, float alpha);

void CreditsDrawText(
    const CreditsTable &table, const CreditsCell &cell, const UILayoutRect &rect, std::int32_t columns, std::int32_t column, float alpha, float rowHeight);

void ApplyFadeInOut(const CreditsTable &table, const CreditsRow &row, UILayoutRect &rect);


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
    const bool described = GetTexture2DDimensions(texture, &width, &height);
    if (!described) {
        texture = g_rendererWhiteTexture;
        GetTexture2DDimensions(texture, &width, &height);
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
    const TextureLoadRequest &request = g_textureLoadRequests[entry.request & 0x1fffU];
    entry.texture = request.generation == static_cast<std::uint16_t>(entry.request >> 16U) ? request.texture : 0U;
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
    const bool sections = CreditsResolveImages(g_creditsSections, g_creditsSectionCount);
    bool ready = CreditsResolveImage(g_creditsSignature) && sections;
    for (CreditsTable &table : g_creditsTables) {
        ready = ready && table.loaded;
        if (table.loaded)
            ready = CreditsResolveImages(table.images, table.imageCount) && ready;
    }
    return ready;
}


void CreditsLoadImage(CreditsTable &table, CreditsCell &cell, const char *path) {
    if (table.imageCount >= table.imageCapacity) {
        const std::uint32_t capacity = 2U * table.imageCapacity >= 16U ? 2U * table.imageCapacity : 16U;
        if (capacity > table.imageCapacity) {
            CreditsImage *const grown = static_cast<CreditsImage *>(std::realloc(table.images, sizeof(CreditsImage) * capacity));
            if (!grown)
                return;
            table.images = grown;
            table.imageCapacity = capacity;
        }
    }
    CreditsImage &entry = table.images[table.imageCount++];
    entry = {};
    std::array<char, 260> localized{};
    if (g_dbg_is_jp_ver != 0) {
        std::size_t length = std::strlen(path);
        while (length > 1 && static_cast<std::uint8_t>(path[length - 1] - '.') > 1U)
            --length;
        if (path[length - 1] == '.') {
            std::snprintf(localized.data(), localized.size(), "%.*s_jp.%s?usage=0", static_cast<int>(length - 1), path, path + length);
            if (d3t::CheckForFile(localized.data()))
                path = localized.data();
        }
    }
    entry.request = LoadTextureFromFilepathWithUsage(path);
    cell.kind = 1;
    cell.value = table.imageCount - 1;
}



void CreditsAppendText(CreditsTable &table, CreditsCell &cell, const char *text) {
    cell.kind = 3;
    if (std::strcmp(text, "!!!") == 0)
        return;
    cell.kind = 2;
    const std::uint32_t size = static_cast<std::uint32_t>(std::strlen(text)) + 1U;
    const std::uint32_t offset = table.textSize;
    const std::uint32_t end = offset + size;
    if (end > table.textCapacity || end < offset) {
        cell.value = 0xffffffffU;
        return;
    }
    table.textSize = end;
    std::memcpy(table.text + offset, text, size);
    cell.value = offset;
}



void CreditsParseCell(CreditsTable &table, CreditsCell &cell, const char *start, const char *end) {
    std::vector<char> field(start, end);
    field.push_back('\0');
    char *const text = field.data();
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
    if (cell.kind == 0)
        CreditsAppendText(table, cell, text);
}



void CreditsParseRow(CreditsTable &table, const char *cursor, const char *end) {
    if (table.rowCount >= table.rowCapacity) {
        const std::uint32_t capacity = 2U * table.rowCapacity >= 16U ? 2U * table.rowCapacity : 16U;
        if (capacity > table.rowCapacity) {
            CreditsRow *const grown = static_cast<CreditsRow *>(std::realloc(table.rows, sizeof(CreditsRow) * capacity));
            if (!grown)
                return;
            table.rows = grown;
            table.rowCapacity = capacity;
        }
    }
    CreditsRow &row = table.rows[table.rowCount++];
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
    CreditsTable *const table = static_cast<CreditsTable *>(data);
    const char *const bytes = static_cast<const char *>(loadedData);
    if (bytes && table) {
        table->text = static_cast<char *>(std::malloc(loadedSize));
        table->textSize = 0;
        table->textCapacity = static_cast<std::uint32_t>(loadedSize);
        const char *line = bytes;
        const char *cursor = bytes;
        const char *const end = bytes + loadedSize;
        if (loadedSize) {
            do {
                while (cursor < end && !(cursor[0] == '\r' && cursor + 1 < end && cursor[1] == '\n'))
                    ++cursor;
                CreditsParseRow(*table, line, cursor);
                cursor += 2;
                line = cursor;
            } while (cursor < end);
        }
        table->loaded = 1;
    }
    ProcessThreadStates(contentHandle, 0);
}


void StartPrepareCreditsCSV() {
    for (std::size_t index = 0; index < g_creditsTables.size(); ++index) {
        CreditsTable &table = g_creditsTables[index];
        std::array<char, 260> path{};
        std::snprintf(path.data(), path.size(), "/ui/Credits/Data/%s.csv", kCreditsTypes[index]);
        table.loaded = 0;
        D3TShaderLoadTask task{};
        task.path = path.data();
        task.thread.callback = AsyncPrepareCreditsCSV;
        task.thread.data = &table;
        table.contentHandle = static_cast<std::uint32_t>(PushShaderLoadToThreadQueue(&task));
    }
}



void ShowCredits() {
    std::array<char, 260> path{};
    std::snprintf(path.data(), path.size(), "/ui/credits/textures/sf_%02u_0.tga?usage=0", g_creditsSectionCount + 1U);
    while (d3t::CheckForFile(path.data())) {
        if (g_creditsSectionCount >= g_creditsSectionCapacity) {
            const std::uint32_t capacity = 2U * g_creditsSectionCapacity >= 16U ? 2U * g_creditsSectionCapacity : 16U;
            CreditsImage *const grown =
                capacity > g_creditsSectionCapacity ? static_cast<CreditsImage *>(std::realloc(g_creditsSections, sizeof(CreditsImage) * capacity)) : nullptr;
            if (grown) {
                g_creditsSections = grown;
                g_creditsSectionCapacity = capacity;
            }
        }
        if (g_creditsSectionCount < g_creditsSectionCapacity) {
            CreditsImage &entry = g_creditsSections[g_creditsSectionCount++];
            entry = {};
            std::array<char, 256> request{};
            if (std::strchr(path.data(), '?'))
                std::snprintf(request.data(), request.size(), "%s", path.data());
            else
                std::snprintf(request.data(), request.size(), "%s%s", path.data(), "?usage=0");
            entry.request = LoadTextureFromFilepath(request.data(), 0, 0);
        }
        std::snprintf(path.data(), path.size(), "/ui/credits/textures/sf_%02u_0.tga?usage=0", g_creditsSectionCount + 1U);
    }
    g_creditsSignature = {};
    g_creditsSignature.request =
        LoadTextureFromFilepathWithUsage(g_dbg_is_jp_ver ? "/ui/credits/textures/yssignature_jp.tga" : "/ui/credits/textures/yssignature.tga");
}



void CreateTextureForCredits() {
    InitTexture2DResource(g_creditsSignature.texture);
    for (std::uint32_t index = 0; index < g_creditsSectionCount; ++index)
        InitTexture2DResource(g_creditsSections[index].texture);
    std::free(g_creditsSections);
    g_creditsSections = nullptr;
    g_creditsSectionCount = 0;
    g_creditsSectionCapacity = 0;
    for (CreditsTable &table : g_creditsTables) {
        table.loaded = 0;
        for (std::uint32_t index = 0; index < table.imageCount; ++index)
            InitTexture2DResource(table.images[index].texture);
        std::free(table.images);
        table.images = nullptr;
        table.imageCount = 0;
        table.imageCapacity = 0;
        std::free(table.rows);
        table.rows = nullptr;
        table.rowCount = 0;
        table.rowCapacity = 0;
        std::free(table.text);
        table.text = nullptr;
        table.textSize = 0;
        table.textCapacity = 0;
    }
}


float CreditsRowHeight(const CreditsTable &table, const CreditsRow &row) {
    const float fontHeight = GNuklearWindow.style.font->height;
    float height = fontHeight - 12.0f;
    for (const CreditsCell &cell : row.cells) {
        if (cell.kind == 1) {
            if (cell.value >= table.imageCount || !table.images[cell.value].texture)
                continue;
            const float imageHeight = static_cast<float>(table.images[cell.value].image.region[3]);
            if (height < imageHeight)
                height = imageHeight;
        } else if (cell.kind == 2 && height < fontHeight) {
            height = fontHeight;
        }
    }
    return height;
}


void CreditsMeasureTables() {
    if (!g_creditsScrollSpace)
        return;
    UILayoutRect rect{};
    UI_GetWindowRect(g_creditsScrollSpace, rect);
    for (CreditsTable &table : g_creditsTables) {
        table.height = 0.0f;
        for (std::uint32_t row = 0; row < table.rowCount; ++row)
            table.height = (CreditsRowHeight(table, table.rows[row]) + table.height) + 12.0f;
        table.height = rect.height + table.height;
    }
}



void CreditsInitScroll() {
    sondLoadSndFromDirIntoBank(3, 2, "rim004.snd");
    shenExecuteAICACommand(0x9a8, 0, 0);
    shenExecuteAICACommand(0x4a0, 2, 98);
    CreditsMeasureTables();
    g_creditsScrollInitialized = 1;
    g_creditsScrollProgress = 0.0f;
    g_creditsScrollSpeed = ((g_creditsTables[0].height + 0.0f) + g_creditsTables[1].height + g_creditsTables[2].height) / 515.0f;
}




void CreditsSkipTable() {
    float progress = g_creditsScrollProgress;
    std::uint32_t table = 0;
    if (progress > g_creditsTables[0].height) {
        float height = g_creditsTables[0].height;
        do {
            ++table;
            progress -= height;
            if (table >= g_creditsTables.size())
                break;
            height = g_creditsTables[table].height;
        } while (progress > height);
    }
    if (table < g_creditsTables.size()) {
        float start = 0.0f;
        for (std::uint32_t index = 0; index <= table; ++index) {
            start += g_creditsTables[index].height;
            g_creditsScrollProgress = start;
        }
    }
    g_creditsFadeOutRequested = 0;
    if (table == 2)
        g_creditsScrollProgress = g_creditsScrollSpeed * 10.0f + ((g_creditsTables[0].height + 0.0f) + g_creditsTables[1].height + g_creditsTables[2].height);
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
    const float total = (g_creditsTables[0].height + 0.0f) + g_creditsTables[1].height + g_creditsTables[2].height;
    if (total <= g_creditsScrollProgress)
        return;
    const float count = static_cast<float>(static_cast<std::int32_t>(g_creditsSectionCount));
    const std::uint32_t index = static_cast<std::uint32_t>(static_cast<std::int32_t>((g_creditsScrollProgress / total) * count));
    if (index >= g_creditsSectionCount)
        return;
    const float share = total / count;
    float t = (g_creditsScrollProgress - static_cast<float>(static_cast<std::int32_t>(index)) * share) / share;
    float fade = 1.0f;
    if (t < 0.25f) {
        fade = LerpEaseInOutQuad(0.0f, 1.0f, t * 4.0f);
    } else if (t > 0.75f) {
        t = t - 0.75f;
        fade = LerpEaseInOutQuad(1.0f, 0.0f, t * 4.0f);
    }
    CreditsDrawSectionImage(g_creditsSections[index], rect, g_creditsFadeAlpha * fade);
}



void CreditsDrawSignature(const UILayoutRect &rect) {
    const float total = (g_creditsTables[0].height + 0.0f) + g_creditsTables[1].height + g_creditsTables[2].height;
    if (g_creditsScrollProgress <= total)
        return;
    float fade = std::fminf(1.0f, (g_creditsScrollProgress - total) / g_creditsScrollSpeed);
    const float fadeOut = (g_creditsScrollSpeed * 10.0f + total) - g_creditsScrollSpeed;
    if (g_creditsScrollProgress > fadeOut)
        fade = fade * (1.0f - (g_creditsScrollProgress - fadeOut) / g_creditsScrollSpeed);
    const float alpha = fade * g_creditsFadeAlpha;
    if (!g_creditsSignature.texture)
        return;
    const float width = static_cast<float>(g_creditsSignature.image.region[2]);
    const float height = static_cast<float>(g_creditsSignature.image.region[3]);
    CreditsPlaceItem(rect.x + (rect.width - width) * 0.5f, rect.y + (rect.height - height) * 0.5f, width, height);
    if (g_waitingForNextTick == 1)
        nk_image_color(&GNuklearWindow, g_creditsSignature.image, nk_rgba(255, 255, 255, CreditsAlphaByte(alpha * 255.0f)));
}


void CreditsDrawImage(const CreditsTable &table, const CreditsCell &cell, const UILayoutRect &rect, float alpha) {
    if (cell.value >= table.imageCount)
        return;
    const CreditsImage &entry = table.images[cell.value];
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
    if (cell.value >= table.textSize)
        return;
    const char *const text = table.text + cell.value;
    const std::size_t length = std::strlen(text);
    if (!length)
        return;
    nk_flags alignment = NK_TEXT_ALIGN_CENTERED;
    if (columns == 2)
        alignment = column == 0 ? NK_TEXT_ALIGN_RIGHT : NK_TEXT_ALIGN_LEFT;
    nk_context &context = GNuklearWindow;
    const float x = rect.x + 2.0f;
    const float y = (rect.y + (rowHeight - context.style.font->height) * 0.5f) + 2.0f;
    const std::uint32_t red = kCreditsTextColour & 0xffU;
    const std::uint32_t green = (kCreditsTextColour >> 8U) & 0xffU;
    const std::uint32_t blue = (kCreditsTextColour >> 16U) & 0xffU;
    const float colourAlpha = static_cast<float>(kCreditsTextColour >> 24U);
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
    std::int32_t columns = row.cells[0].kind != 0 ? 1 : 0;
    if (row.cells[1].kind)
        columns = 2;
    if (row.cells[2].kind)
        columns = 3;
    if (row.cells[3].kind)
        columns = 4;
    if (columns && rect.y + rowHeight >= 0.0f) {
        const float bottom = rect.y + rowHeight;
        float fade = 1.0f;
        if (bottom < 128.0f)
            fade = LerpEaseInOutQuad(0.0f, 1.0f, bottom * 0.0078125f);
        else if (rect.y > rect.height - 128.0f)
            fade = LerpEaseInOutQuad(1.0f, 0.0f, (rect.y - (rect.height - 128.0f)) * 0.0078125f);
        const float alpha = fade * g_creditsFadeAlpha;
        const float columnWidth = rect.width / static_cast<float>(columns);
        float x = rect.x;
        for (std::int32_t column = 0; column < columns; ++column) {
            const UILayoutRect cellRect{.x = x + 16.0f, .y = rect.y, .width = columnWidth - 32.0f, .height = rect.height};
            const CreditsCell &cell = row.cells[static_cast<std::size_t>(column)];
            if (cell.kind == 1)
                CreditsDrawImage(table, cell, cellRect, alpha);
            else if (cell.kind == 2)
                CreditsDrawText(table, cell, cellRect, columns, column, alpha, rowHeight);
            x += columnWidth;
        }
    }
    rect.y = (rowHeight + 12.0f) + rect.y;
}


std::int64_t Credits_InitScrollSpace() {
    const std::int32_t window = UI_FindViewWindow(static_cast<std::uint32_t>(g_uiElements[kUiCredits].config.uiHandleIndex), "Credits::ScrollSpace");
    g_creditsScrollSpace = UI_GetWindow(window);
    return 1;
}


std::int64_t Credits_Reset() {
    g_creditsScrollSpace = nullptr;
    return 0;
}


std::int64_t Credits_IsPressed() {
    const std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
    if (!g_dbg_is_jp_ver && !g_creditsFadeOutRequested && ((pressed & 0x400U) != 0 || (pressed & 0x200U) != 0))
        g_creditsFadeOutRequested = 1;
    return 0;
}



void ReadCreditsCSV() {
    LoadPresentationDefinition(kUiCredits, "/ui/Credits/Credits.ui");
    g_isRenderSuppressed = 1;
    ShowCredits();
    StartPrepareCreditsCSV();
    g_creditsScrollProgress = 0.0f;
    g_creditsFadeAlpha = 1.0f;
    g_creditsFadeOutRequested = 0;
    g_creditsScrollInitialized = 0;
}



std::int64_t Credits_Destroy() {
    g_isRenderSuppressed = 0;
    CreateTextureForCredits();
    return 1;
}






std::int64_t Credits_Tick() {
    b60fps = 1;
    nk_context &context = GNuklearWindow;
    const UIBaseRuntime &element = g_uiElements[kUiCredits];
    const std::uint32_t slot = static_cast<std::uint32_t>(element.config.uiHandleIndex);
    g_currActiveUIIndex = element.index;
    if (slot <= 0x4bU)
        UI_InputHandler(context, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
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
    if (!g_creditsScrollInitialized)
        CreditsInitScroll();
    g_creditsScrollProgress = g_creditsScrollProgress + g_creditsScrollSpeed * 0.016666668f;
    const float alpha = g_creditsFadeAlpha;
    if (g_creditsFadeOutRequested) {
        g_creditsFadeAlpha = g_creditsFadeAlpha + -0.016666668f;
        if (alpha + -0.016666668f <= 0.0f)
            CreditsSkipTable();
    } else {
        g_creditsFadeAlpha = g_creditsFadeAlpha + 0.016666668f;
        if (alpha + 0.016666668f > 1.0f)
            g_creditsFadeAlpha = 1.0f;
    }
    if (!g_creditsScrollSpace)
        return 0;
    UILayoutRect rect{};
    UI_GetWindowRect(g_creditsScrollSpace, rect);
    if (g_waitingForNextTick == 1) {
        nk_begin_titled(&context, &g_creditsPanel, "UICredits", "UICredits", nk_rect(rect.x, rect.y, rect.width, rect.height), NK_WINDOW_NO_SCROLLBAR);
        if (g_waitingForNextTick == 1 && context.current && context.current->layout)
            nk_layout_space_begin(&context, NK_STATIC, 0.0f, 1);
    }
    CreditsDrawSignature(rect);
    CreditsDrawSection(rect);
    UILayoutRect roll{.x = rect.x, .y = (rect.y + rect.height) - g_creditsScrollProgress, .width = rect.width, .height = rect.height};
    for (CreditsTable &table : g_creditsTables) {
        if (table.loaded) {
            for (std::uint32_t row = 0; row < table.rowCount; ++row) {
                ApplyFadeInOut(table, table.rows[row], roll);
                if (roll.y > roll.height)
                    break;
            }
        }
        roll.y = roll.y + roll.height;
    }
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
}
