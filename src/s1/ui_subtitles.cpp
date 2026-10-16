#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {
const char *GetSubsPathForLanguage(std::int32_t language);

void BuildSpecialSubtitleIndex();

bool ReadFromCacheSubsData(std::vector<std::byte> &data);

void InsertLocalisedText(void *requestData, std::uint32_t, void *loadedData, std::uint64_t loadedSize, const char *);

void AsyncPrepareFontGlyphs(void *requestData, std::uint32_t, void *loadedData, std::uint64_t loadedSize, const char *);

void LoadFontGlyphsAsync(const char *glyphPath, std::int32_t language);

bool Subtitles_IsIdle();

void Subtitles_FadeOutPanels();

const VoiceIndexRecord *FindVoiceIndexRecordInTable(VoiceIndexRecord *records, std::int32_t count, const char *voiceId, std::uint32_t *aliasState);

const VoiceIndexRecord *FindVoiceIndexRecord(const CVRSEntry &source, const char (&voiceId)[12], std::uint32_t *aliasState);

Character *FindSubtitleCharacter(std::uint32_t id);

void SubtitlePlaybackCallback(CVWKRuntimeSlot *slot);

void SubtitleFadeCallback(CVWKRuntimeSlot *slot);

void FreeSequenceFileHandles(CVWKRuntimeSlot &slot);

void SubtitleAuxLoadCallback(CVWKRuntimeSlot *slot);
}

namespace shenmue::s1 {
void SEQ_FreeFileHandles(CVWKRuntimeSlot &slot);

void CVWK_SetAudioPauseState(bool paused);

void SEQ_ResetSubtitleAudioSlots();

void SEQ_EndActiveScenarioSubtitles();

void DisableSubtitles();

void shenForceClearCurrentSubtitles(CVWKRuntimeSlot *slot);

bool CVWK_IsSlotActive(const CVWKRuntimeSlot *slot);

void ReadSubtitleByID(HLib::Task *scenarioTask, CVWKRuntimeSlot &slot);

void CVWK_StopSlotVoice(CVWKRuntimeSlot &slot);

void ReadCurrentSlotSubtitle(CVWKRuntimeSlot *slot);

void AssignReadSubtitleByID();

static void ApplyVoiceCameraNearClip(const char *voiceId);

const char *ReadSubtitleStringById(const SubtitleStringId &id, bool japaneseAudio);

void UI_PresentSearchFocusedItem(HLib::Task *scenarioTask, const char *stringId, bool visible);

void ProcessSubtitleText(const char *text);

void ApplySubtitlePresentationClass(std::uint32_t presentationClass);

const std::array<std::array<char, 512>, 3> &CurrentSubtitleLines();

void PrepareSubtitleFontAtlas(void *data, std::uint32_t, void *loadedData, std::uint64_t loadedSize, const char *);

void InstallLegacyUIFontRecord(std::uint32_t language, const std::uint32_t *header, const std::byte *record, const char *path, std::uint32_t renderHandle);

void RetireLegacyUIFont(std::int32_t language);
}

namespace shenmue::s1::ui_runtime_detail {

const char *GetSubsPathForLanguage(std::int32_t language) {
    switch (language) {
    case 1:
        return "/subs/japanese";
    case 2:
        return "/subs/french";
    case 3:
        return "/subs/german";
    case 4:
        return "/subs/korean";
    case 5:
        return "/subs/chineset";
    case 6:
        return "/subs/chineses";
    default:
        return "/subs/english";
    }
}

void BuildSpecialSubtitleIndex() {
    if (!g_specialSubtitleEntries.empty())
        return;
    if (!g_stringTable || g_stringTable->version != 3 || g_stringTable->count <= 0 || !g_stringTable->records) {
        return;
    }

    const std::byte *record = g_stringTable->records;
    for (std::int32_t index = 0; index < g_stringTable->count; ++index, record += 28) {
        if (record[0] != std::byte{'Z'} || record[1] != std::byte{'Z'}) {
            continue;
        }
        SpecialSubtitleEntry entry{};
        std::memcpy(entry.identifier, record + 2, sizeof(entry.identifier));
        std::uint16_t value = 0;
        std::memcpy(&value, record + 10, sizeof(value));
        entry.value = value;
        g_specialSubtitleEntries.push_back(entry);
    }
}

bool ReadFromCacheSubsData(std::vector<std::byte> &data) {
    if (data.size() < sizeof(SubtitleTableHeader)) {
        return false;
    }

    std::int32_t version = 0;
    std::int32_t count = 0;
    std::memcpy(&version, data.data(), sizeof(version));
    std::memcpy(&count, data.data() + 4, sizeof(count));
    if (count < 0) {
        return false;
    }

    std::size_t recordsOffset = 0;
    std::size_t recordStride = 0;
    switch (version) {
    case 1:
        recordsOffset = 8;
        recordStride = 12;
        break;
    case 2:
        recordsOffset = 16;
        recordStride = 16;
        break;
    case 3:
        recordsOffset = 16;
        recordStride = 28;
        break;
    default:
        return false;
    }

    const std::size_t recordBytes = static_cast<std::size_t>(count) * recordStride;
    if (recordsOffset > data.size() || recordBytes > data.size() - recordsOffset) {
        return false;
    }

    auto *const header = reinterpret_cast<SubtitleTableHeader *>(data.data());
    const std::byte *const records = data.data() + recordsOffset;
    std::memcpy(data.data() + offsetof(SubtitleTableHeader, records), &records, sizeof(records));
    g_stringTable = header;
    g_stringTableData = records + recordBytes;
    g_stringTableEnd = data.data() + data.size();
    BuildSpecialSubtitleIndex();
    return true;
}

void InsertLocalisedText(void *requestData, std::uint32_t, void *loadedData, std::uint64_t loadedSize, const char *) {
    auto *const request = static_cast<SubtitleLoadRequest *>(requestData);
    if (!request || request->language < 0 || request->language >= 7 || !loadedData || loadedSize < sizeof(SubtitleTableHeader)) {
        return;
    }
    auto &data = g_subtitleData[static_cast<std::size_t>(request->language)];
    const auto *const first = static_cast<const std::byte *>(loadedData);
    data.assign(first, first + loadedSize);
    if (ReadFromCacheSubsData(data)) {
        request->state = 1;
    }
}

void AsyncPrepareFontGlyphs(void *requestData, std::uint32_t, void *loadedData, std::uint64_t loadedSize, const char *) {
    auto *const request = static_cast<GlyphLoadRequest *>(requestData);
    if (!request || request->language < 0 || request->language >= 7 || !loadedData || loadedSize == 0) {
        return;
    }
    auto &data = g_subtitleGlyphData[static_cast<std::size_t>(request->language)];
    const auto *const first = static_cast<const std::byte *>(loadedData);
    data.assign(first, first + loadedSize);
    request->glyphData = data.data();
    request->state = 2;
    g_glyphLoadStates[static_cast<std::size_t>(request->language)] = 2;
}

void LoadFontGlyphsAsync(const char *glyphPath, std::int32_t language) {
    const std::uint16_t index = g_glyphLoadRequestIndex.fetch_add(1, std::memory_order_seq_cst) & 0x1fffU;
    GlyphLoadRequest &request = g_glyphLoadRequests[index];
    request = {};
    request.language = language;
    g_glyphLoadStates[static_cast<std::size_t>(language)] = 1;

    D3TShaderLoadTask task{};
    task.path = glyphPath;
    task.renderHandle = g_activeRenderHandle;
    task.thread.callback = AsyncPrepareFontGlyphs;
    task.thread.data = &request;
    PushShaderLoadToThreadQueue(&task);
}

const SubtitleFontGlyph *FindSubtitleGlyph(const SubtitleFontLayer &font, std::uint32_t codepoint) {
    if (font.glyphs.empty())
        return nullptr;
    auto end = font.glyphs.end();
    const SubtitleFontGlyph *fallback = nullptr;
    if ((end - 1)->codepoint == '?') {
        fallback = &*(end - 1);
        --end;
    }
    const auto found =
        std::lower_bound(font.glyphs.begin(), end, codepoint, [](const SubtitleFontGlyph &glyph, std::uint32_t value) { return glyph.codepoint < value; });
    return found != end && found->codepoint == codepoint ? &*found : fallback;
}

std::uint32_t DecodeSubtitleCodepoint(const char *&text) {
    const auto first = static_cast<std::uint8_t>(*text++);
    if (first < 0x80U)
        return first;
    std::uint32_t codepoint = 0;
    std::uint32_t continuationCount = 0;
    if ((first & 0xe0U) == 0xc0U) {
        codepoint = first & 0x1fU;
        continuationCount = 1;
    } else if ((first & 0xf0U) == 0xe0U) {
        codepoint = first & 0x0fU;
        continuationCount = 2;
    } else if ((first & 0xf8U) == 0xf0U) {
        codepoint = first & 0x07U;
        continuationCount = 3;
    } else {
        return '?';
    }
    for (std::uint32_t index = 0; index < continuationCount; ++index) {
        const auto byte = static_cast<std::uint8_t>(*text);
        if ((byte & 0xc0U) != 0x80U)
            return '?';
        ++text;
        codepoint = (codepoint << 6U) | (byte & 0x3fU);
    }
    return codepoint;
}

bool DrawSubtitleLayer(
    const SubtitleFontLayer &font, const char *text, float lineY, float offsetX, float offsetY, std::uint32_t colour, std::uint32_t renderTarget) {
    const std::uint32_t texture = font.texture.load(std::memory_order_acquire);
    if (!texture || !text || !*text)
        return false;

    std::array<std::uint32_t, 512> codepoints{};
    std::size_t codepointCount = 0;
    float lineWidth = 0.0f;
    while (*text && codepointCount < codepoints.size()) {
        const std::uint32_t codepoint = DecodeSubtitleCodepoint(text);
        codepoints[codepointCount++] = codepoint;
        if (const SubtitleFontGlyph *glyph = FindSubtitleGlyph(font, codepoint)) {
            lineWidth += glyph->advance;
        }
    }

    const float scaleX = static_cast<float>(g_uiScreenWidth) / 1920.0f;
    const float scaleY = static_cast<float>(g_uiScreenHeight) / 1080.0f;
    float cursor = (1920.0f - lineWidth) * 0.5f + offsetX;
    const float originY = lineY + (50.0f - font.lineHeight) * 0.5f + offsetY;
    std::array<NuklearQuad, 512> quads{};
    std::size_t quadCount = 0;
    for (std::size_t index = 0; index < codepointCount; ++index) {
        const SubtitleFontGlyph *const glyph = FindSubtitleGlyph(font, codepoints[index]);
        if (!glyph)
            continue;
        if (glyph->width > 0.0f && glyph->height > 0.0f) {
            quads[quadCount++] = {(cursor + glyph->x0) * scaleX, (originY + glyph->y0) * scaleY, glyph->width * scaleX, glyph->height * scaleY, glyph->u0,
                glyph->v0, glyph->u1, glyph->v1, colour};
        }
        cursor += glyph->advance;
    }
    return quadCount != 0 && DrawNuklearQuads(g_RenderDeviceContext, renderTarget, texture, std::span<const NuklearQuad>{quads.data(), quadCount});
}


std::uint32_t Subtitles_LoadUI() {
    UIBaseRuntime &element = g_uiElements[kSubtitleUiStorageIndex];
    if (!element.setup) {
        element.setup = 1;
        element.someFlag3 = 0;
        constexpr char path[] = "/ui/gamesubtitles/GameSubtitles.ui";
        std::memcpy(element.uiConfigFilepath, path, sizeof(path));
        const std::uint32_t handle = OpenUIFile(path, element);
        element.config.screenWidth = static_cast<std::int16_t>(handle);
        element.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        element.config.init = 1;
    }
    return static_cast<std::uint32_t>(element.setup);
}



bool Subtitles_IsIdle() {
    const std::array<d3t::UIImageWidget *, 2> &images = g_subtitleBackgroundImages;
    return (!images[1] || !images[1]->field44) && (!images[0] || !images[0]->field44) && (!g_subtitleWindows[0] || g_subtitleWindows[0]->alpha == 0.0f) &&
        (!g_subtitleWindows[1] || g_subtitleWindows[1]->alpha == 0.0f) && !g_subtitleLines[0][0] && !g_subtitleLines[1][0] && !g_subtitleLines[2][0];
}



void Subtitles_FadeOutPanels() {
    if (g_debugUiDisabled)
        return;
    if (g_subtitleWindows[0] && g_subtitleWindows[0]->alpha == 1.0f && !g_uiAnimSlots[static_cast<std::size_t>(g_subtitleFullPanelBinding)].status) {
        UIWidget_Animate(g_subtitleFullPanelBinding, 0, 0.3f, 0.0f);
        if (g_debugUiDisabled)
            return;
    }
    if (g_subtitleWindows[1] && g_subtitleWindows[1]->alpha == 1.0f && !g_uiAnimSlots[static_cast<std::size_t>(g_subtitleHudPanelBinding)].status)
        UIWidget_Animate(g_subtitleHudPanelBinding, 0, 0.3f, 0.0f);
}






std::int64_t Subtitles_IsSelected() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kSubtitleUiStorageIndex].config.uiHandleIndex);
    if (slot > 0x4bU)
        return 1;
    const d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &view = g_uiViewSlots[slot];
    for (std::size_t index = 0; index < kSubtitleWindowIds.size(); ++index)
        g_subtitleWindows[index] = UI_GetWindow(UI_FindViewWindow(slot, kSubtitleWindowIds[index]));
    const std::int32_t phone = UI_FindViewLabel(slot, "Sub_PhoneNumber");
    g_subtitlePhoneLabel = static_cast<std::uint32_t>(phone) <= 0x1ffU ? &g_uiLabelWidgets[static_cast<std::size_t>(phone)] : nullptr;
    if (g_subtitlePhoneLabel)
        g_subtitlePhoneLabel->text = g_telephoneDisplayedNumber.data();
    for (std::size_t index = 0; index < kSubtitleImageIds.size(); ++index) {
        const std::int32_t image = UI_FindViewImageWidget(slot, kSubtitleImageIds[index]);
        g_subtitleBackgroundImages[index] = static_cast<std::uint32_t>(image) < 0x200U ? &g_uiImageWidgets[static_cast<std::size_t>(image)] : nullptr;
        if (g_subtitleBackgroundImages[index])
            g_subtitleBackgroundImages[index]->field44 = 0;
    }
    g_subtitleTextAnimSlot = UIAnimAllocSlot();
    const std::int32_t full = UI_FindViewWindow(slot, "SubFullPanel");
    d3t::UIWindow *const fullWindow = UI_GetWindow(full);
    g_subtitleFullPanelBinding = UIWidget_Bind(full, fullWindow ? &fullWindow->alpha : nullptr, {0.0f, 0.0f}, {1.0f, 0.0f});
    const std::int32_t hud = UI_FindViewWindow(slot, "SubHudPanel");
    d3t::UIWindow *const hudWindow = UI_GetWindow(hud);
    g_subtitleHudPanelBinding = UIWidget_Bind(hud, hudWindow ? &hudWindow->alpha : nullptr, {0.0f, 0.0f}, {1.0f, 0.0f});
    g_subtitlePresentationColours = kDefaultSubtitleColours;
    std::int32_t labels = 0;
    for (std::int32_t widget = 0; widget < view.widgetCount && labels < 9; ++widget) {
        if (view.widgetKinds[static_cast<std::size_t>(widget)] != 15)
            continue;
        const std::int32_t label = view.widgetPoolIndices[static_cast<std::size_t>(widget)];
        const std::size_t line = static_cast<std::size_t>(labels / 3);
        if (labels % 3 == 0)
            g_subtitleBackgroundLabels[line] = label;
        else if (labels % 3 == 1)
            g_subtitleOutlineLabels[line] = label;
        else
            g_subtitleForegroundLabels[line] = label;
        ++labels;
    }
    std::uint32_t *const classColours = &g_subtitleClassColours[0].foreground;
    for (std::size_t index = 0; index < 20; ++index) {
        const d3t::UIViewVariable &variable = view.variables[index];
        if (variable.type == d3t::UIViewVariableType::Float4) {
            if (index < 12) {
                const std::array<float, 4> &value = variable.value.float4;
                classColours[index] = ConstructColour(static_cast<std::int32_t>(value[0]), static_cast<std::int32_t>(value[1]),
                    static_cast<std::int32_t>(value[2]), static_cast<std::int32_t>(value[3]));
            }
        } else if (variable.type == d3t::UIViewVariableType::Float) {
            g_subtitleVariableScale = variable.value.floatValue / 1080.0f;
        }
    }
    return 1;
}







std::int64_t Subtitles_Tick() {
    const std::uint32_t slot = static_cast<std::uint32_t>(g_uiElements[kSubtitleUiStorageIndex].config.uiHandleIndex);
    if (slot > 0x4bU)
        return 0;
    const auto labelRecord = [](std::int32_t label) {
        return static_cast<std::uint32_t>(label) <= 0x1ffU ? &g_uiLabelWidgets[static_cast<std::size_t>(label)] : nullptr;
    };
    if (!g_subtitlesShown || Subtitles_IsIdle()) {
        for (std::size_t line = 0; line < 3; ++line) {
            for (const std::int32_t label : {g_subtitleBackgroundLabels[line], g_subtitleOutlineLabels[line], g_subtitleForegroundLabels[line]}) {
                if (d3t::UILabelWidget *const record = label != -1 ? labelRecord(label) : nullptr) {
                    record->text = "";
                    record->token.alreadyLocalized = 1;
                }
            }
        }
    } else {
        UIAnimSlot &timer = g_uiAnimSlots[static_cast<std::size_t>(g_subtitleTextAnimSlot)];
        timer.allocated = 1;
        timer.status = 1;
        timer.interpolation = 3;
        timer.start[0] = std::bit_cast<float>(1);
        timer.current[0] = std::bit_cast<float>(1);
        timer.end[0] = 0.0f;
        timer.delay = 0.3f;
        timer.elapsed = 0.0f;
        timer.duration = 0.1f;
        timer.reserved40 = -1;
        if (!g_uiAnimSlots[static_cast<std::size_t>(g_subtitleFullPanelBinding)].status && g_subtitleWindows[0] && g_subtitleWindows[0]->alpha == 0.0f)
            UIWidget_Animate(g_subtitleFullPanelBinding, 1, 0.1f, 0.0f);
        if (!g_uiAnimSlots[static_cast<std::size_t>(g_subtitleHudPanelBinding)].status && g_subtitleWindows[1] && g_subtitleWindows[1]->alpha == 0.0f)
            UIWidget_Animate(g_subtitleHudPanelBinding, 1, 0.1f, 0.0f);
        const SubtitlePresentationColours colours = g_subtitlePresentationColours;
        for (std::size_t line = 0; line < 3; ++line) {
            const char *const text = g_subtitleLines[line].data();
            if (d3t::UILabelWidget *const background = labelRecord(g_subtitleBackgroundLabels[line])) {
                background->text = text;
                background->colour = colours.background;
                background->size = 0;
                background->fontFamily = 1;
                background->token.alreadyLocalized = 1;
            }
            if (d3t::UILabelWidget *const outline = labelRecord(g_subtitleOutlineLabels[line])) {
                outline->text = text;
                outline->colour = colours.outline;
                outline->size = 0;
                outline->fontFamily = 1;
                outline->token.alreadyLocalized = 1;
            }
            if (d3t::UILabelWidget *const foreground = labelRecord(g_subtitleForegroundLabels[line])) {
                foreground->text = text;
                foreground->colour = colours.foreground;
                foreground->size = 0;
                foreground->fontFamily = 0;
                foreground->token.alreadyLocalized = 1;
            }
        }
    }
    if (!std::bit_cast<std::int32_t>(g_uiAnimSlots[static_cast<std::size_t>(g_subtitleTextAnimSlot)].current[0]))
        Subtitles_FadeOutPanels();
    if (static_cast<std::uint32_t>(g_perspectiveProjection - 1) > 2U && g_subtitleBackgroundImages[1] && g_subtitleBackgroundImages[0]) {
        g_subtitleBackgroundImages[1]->field44 = 0;
        g_subtitleBackgroundImages[0]->field44 = 0;
    }
    UI_InputHandler(GNuklearWindow, static_cast<std::int32_t>(slot), 0, g_uiViewSlots[slot].widgetCount);
    return 0;
}


void Subtitles_Render() {
    g_subtitlesShown = true;
}


std::int64_t ResetSubtitles() {
    g_subtitleText[0] = '\0';
    g_subtitlesShown = false;
    for (std::array<char, 512> &line : g_subtitleLines)
        line.fill(0);
    return 1;
}


std::int64_t Subtitles_Reset() {
    g_subtitleWindows = {};
    return 0;
}


const VoiceIndexRecord *FindVoiceIndexRecordInTable(VoiceIndexRecord *records, std::int32_t count, const char *voiceId, std::uint32_t *aliasState) {
    if (g_usingJapaneseAudio == 1 && g_stringTable && g_stringTable->version == 3) {
        const std::byte *alias = g_stringTable->records;
        for (std::int32_t index = 0; index < g_stringTable->count; ++index, alias += 28) {
            if (std::memcmp(alias, voiceId, 12) != 0)
                continue;
            std::uint32_t firstWord;
            std::memcpy(&firstWord, alias + 12, sizeof(firstWord));
            if (!firstWord) {
                if (aliasState)
                    *aliasState = 1;
                return nullptr;
            }
            voiceId = reinterpret_cast<const char *>(alias + 12);
            break;
        }
    }
    std::strncpy(records[count].id, voiceId, sizeof(records[count].id));
    while (std::memcmp(records->id, voiceId, sizeof(records->id)) != 0) {
        ++records;
        --count;
    }
    return count ? records : nullptr;
}

const VoiceIndexRecord *FindVoiceIndexRecord(const CVRSEntry &source, const char (&voiceId)[12], std::uint32_t *aliasState) {
    if (!source.fileData)
        return nullptr;
    std::byte *const bytes = reinterpret_cast<std::byte *>(source.fileData);
    std::uint32_t tag{};
    std::uint16_t count{};
    std::memcpy(&tag, bytes, sizeof(tag));
    std::memcpy(&count, bytes + 6, sizeof(count));
    VoiceIndexRecord *const records = reinterpret_cast<VoiceIndexRecord *>(bytes + 0x14);


    if (tag == SHEN_NAME(IDX1))
        return records;

    return FindVoiceIndexRecordInTable(records, count, voiceId, aliasState);
}

Character *FindSubtitleCharacter(std::uint32_t id) {
    if (!id || !g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    const auto *const map = reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr);
    return CHID_Find(map->scenarioTask, id);
}

void SubtitlePlaybackCallback(CVWKRuntimeSlot *slot) {
    if (!slot)
        return;
    if (g_currentSubtitleSlot == slot && g_cvwkReadSubtitleOnPlay) {
        const MapControlData *const map = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        ReadSubtitleByID(map ? map->scenarioTask : nullptr, *slot);
        g_cvwkReadSubtitleOnPlay = 0;
    }
    if (DialogueVoiceIsPlaying(slot->audioContext))
        return;
    if (!Audio_HasMasteringVoice() && faceHasRemainingDuration(FindSubtitleCharacter(slot->participantId)))
        return;
    if (g_currentSubtitleSlot == slot) {
        ProcessSubtitleText(nullptr);
        g_currentSubtitleSlot = nullptr;
    }
    DestroyDialogueVoice(slot->audioContext);
    bool participantStillOwned = false;
    if (g_pCVWK && slot->participantId) {
        for (CVWKRuntimeSlot &candidate : g_pCVWK->slots) {
            if (&candidate != slot && candidate.callback && candidate.participantId == slot->participantId) {
                participantStillOwned = true;
                break;
            }
        }
    }
    if (!participantStillOwned) {
        faceSetGeneratedLipCues(FindSubtitleCharacter(slot->participantId), nullptr, true);
    }
    slot->callback = nullptr;
}

void SubtitleFadeCallback(CVWKRuntimeSlot *slot) {
    if (!slot)
        return;
    const std::int32_t volume = static_cast<std::int32_t>(slot->fadeFrame) - static_cast<std::int32_t>(slot->fadeFrames);
    slot->fadeFrame = static_cast<std::uint32_t>(volume);
    if (volume <= -200) {
        shenForceClearCurrentSubtitles(slot);
        return;
    }
    if ((g_sceneId == 1 && g_areaId == SHEN_NAME(OP02)) || (S1SettingsFlags_Get() & 2U) != 0) {
        ProcessAudioData(slot, volume);
    }
}

void FreeSequenceFileHandles(CVWKRuntimeSlot &slot) {
    DestroyDialogueVoice(slot.audioContext);
    if (slot.fileHandle)
        ReadCachedFile(slot.fileHandle);
    if (slot.participantId) {
        faceSetGeneratedLipCues(FindSubtitleCharacter(slot.participantId), nullptr, true);
    }
    if (g_currentSubtitleSlot == &slot) {
        ProcessSubtitleText(nullptr);
        g_currentSubtitleSlot = nullptr;
    }
    slot.fileHandle = nullptr;
    slot.callback = nullptr;
}




void SubtitleAuxLoadCallback(CVWKRuntimeSlot *slot) {
    LargeFileHandle *const handle = slot->fileHandle;
    if (handle) {
        const std::int32_t fileState = CheckFileInfo(handle);
        if (fileState != 0 && static_cast<std::uint32_t>(fileState - 3) > 1U)
            return;
        ReadCachedFile(handle);
    }
    slot->fileHandle = nullptr;
    auto *const base = static_cast<std::byte *>(slot->cvfbBlock);
    std::uint32_t tag{};
    std::memcpy(&tag, base, sizeof(tag));
    if (tag == SHEN_NAME(SEGA)) {
        std::uint32_t entry{};
        std::uint32_t block{};
        std::memcpy(&entry, base + 0x818, sizeof(entry));
        std::memcpy(&block, base + 0x81c, sizeof(block));
        PackAdfsWrapper *const afs = static_cast<PackAdfsWrapper *>(static_cast<CVRSEntry *>(slot->fileOrSequence)->afs);
        if (LargeFileHandle *const reread = afs ? LargeFile_OpenTocEntryHandle(afs->status, static_cast<std::int32_t>(entry)) : nullptr) {
            reread->offsetBytes = static_cast<std::int64_t>(block) << 11;
            (void)GetFileFromCachedBuffer(reread, 1, base, 0);
            slot->fileHandle = reread;
            return;
        }
    } else {
        const std::uint32_t landed = 0;
        std::memcpy(base + 0x820, &landed, sizeof(landed));
    }
    const std::size_t capacity = 0x800;
    const std::size_t firstOffset = std::min<std::size_t>(slot->stateOrOwner & ~3U, capacity);
    auto *const first = reinterpret_cast<std::uint32_t *>(base + firstOffset);
    if (!slot->participantId && firstOffset + 8 <= capacity)
        slot->participantId = first[1];
    const std::size_t firstSize = firstOffset + 4 <= capacity ? static_cast<std::size_t>(first[0] & ~3U) : capacity;
    if (firstSize <= capacity - firstOffset) {
        auto *const second = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(first) + firstSize);
        const std::size_t secondOffset = firstOffset + firstSize;
        if (secondOffset + 4 <= capacity) {
            const std::size_t secondSize = second[0] & ~3U;
            if (secondSize <= capacity - secondOffset) {
                std::byte *const payload = reinterpret_cast<std::byte *>(second + 1);
                if (second[0] > 8 || (second[0] == 8 && payload[0] != std::byte{})) {
                    slot->clip = payload;
                }
                auto *const third = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(second) + secondSize);
                const std::size_t thirdOffset = secondOffset + secondSize;
                if (thirdOffset + 4 <= capacity) {
                    const std::size_t thirdSize = third[0] & ~3U;
                    if (thirdSize <= capacity - thirdOffset) {
                        if (third[0] >= 0x10U) {
                            faceSetGeneratedLipCues(FindSubtitleCharacter(slot->participantId), reinterpret_cast<FaceLipCue *>(third + 1), true);
                        }
                    }
                    if (thirdSize <= capacity - thirdOffset && thirdOffset + thirdSize + 8 <= capacity) {
                        std::uint32_t nestedTag{};
                        std::memcpy(&nestedTag, reinterpret_cast<std::byte *>(third) + thirdSize, sizeof(nestedTag));
                        if (nestedTag == SHEN_NAME(TXTJ)) {
                            slot->japaneseText = reinterpret_cast<std::byte *>(third) + thirdSize + 8;
                        }
                    }
                }
            }
        }
    }
    const MapControlData *const map = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    ReadSubtitleByID(map ? map->scenarioTask : nullptr, *slot);
    slot->callback = SubtitlePlaybackCallback;
}

// CVWK_IsSlotActive: a specific voice slot is active while its callback is set; without one, any CVWK slot counts.
bool CVWK_IsVoiceSlotActive(const CVWKRuntimeSlot *slot) {
    if (slot)
        return slot->callback != nullptr;
    if (!g_pCVWK)
        return false;
    for (const CVWKRuntimeSlot &candidate : g_pCVWK->slots) {
        if (candidate.callback)
            return true;
    }
    return false;
}
}

namespace shenmue::s1 {

void SEQ_FreeFileHandles(CVWKRuntimeSlot &slot) {
    FreeSequenceFileHandles(slot);
}

void CVWK_SetAudioPauseState(bool paused) {
    if (!g_pCVWK)
        return;
    for (CVWKRuntimeSlot &slot : g_pCVWK->slots) {
        if (slot.audioContext.sourceVoice) {
            SetDialogueVoicePaused(slot.audioContext, paused);
        }
    }

    const bool preserveSequences = (g_sceneId == 1 && g_areaId == SHEN_NAME(OP02)) || (S1SettingsFlags_Get() & 2U) != 0;
    if (preserveSequences)
        return;
    if (paused) {
        for (CVWKRuntimeSlot &slot : g_pCVWK->slots) {
            if (slot.callback)
                SEQ_FreeFileHandles(slot);
        }
        return;
    }

    UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiStorageIndex];
    if ((g_currUIState == -1 || g_currUIState == kSubtitleUiState) &&
        (!subtitles.isVisible || !subtitles.destroy || subtitles.destroy() == 1 || g_busy_reset == 1)) {
        subtitles.isVisible = 0;
    }
}

void SEQ_ResetSubtitleAudioSlots() {
    g_currentSubtitleSlot = nullptr;
    ProcessSubtitleText(nullptr);
    if (!g_pCVWK)
        return;
    for (CVWKRuntimeSlot &slot : g_pCVWK->slots) {
        if (!slot.callback)
            continue;
        slot.fadeFrame = 0;
        slot.fadeFrames = 10;
        slot.callback = SubtitleFadeCallback;
    }
}

void SEQ_EndActiveScenarioSubtitles() {
    if (!g_pSCNC || g_pSCNC->header.active != 1)
        return;

    SEQ_ResetSubtitleAudioSlots();
    UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiStorageIndex];
    if ((g_currUIState == -1 || g_currUIState == kSubtitleUiState) &&
        (!subtitles.isVisible || !subtitles.destroy || subtitles.destroy() == 1 || g_busy_reset == 1)) {
        subtitles.isVisible = 0;
    }
}


void DisableSubtitles() {
    UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiStorageIndex];
    if ((g_currUIState == -1 || g_currUIState == kSubtitleUiState) &&
        (!subtitles.isVisible || !subtitles.destroy || subtitles.destroy() == 1 || g_busy_reset == 1))
        subtitles.isVisible = 0;
}



void shenForceClearCurrentSubtitles(CVWKRuntimeSlot *slot) {
    if (!g_pCVWK && !slot)
        return;
    if (slot) {
        if (g_currentSubtitleSlot == slot)
            DisableSubtitles();
        if (slot->callback)
            SEQ_FreeFileHandles(*slot);
        return;
    }
    DisableSubtitles();
    for (CVWKRuntimeSlot &current : g_pCVWK->slots) {
        if (current.callback)
            SEQ_FreeFileHandles(current);
    }
}


bool CVWK_IsSlotActive(const CVWKRuntimeSlot *slot) {
    if (slot)
        return slot->callback != nullptr;
    for (const CVWKRuntimeSlot &current : g_pCVWK->slots) {
        if (current.callback)
            return true;
    }
    return false;
}



void ReadSubtitleByID(HLib::Task *scenarioTask, CVWKRuntimeSlot &slot) {
    SubtitleStringId id{};
    std::memcpy(&id, slot.voiceId, sizeof(id));
    const char *const text = ReadSubtitleStringById(id, g_usingJapaneseAudio == 1);
    if (text)
        SCNT_ShowSubtitleText(scenarioTask, text, scntCheckHasParticipant(slot.participantId));
}


void CVWK_StopSlotVoice(CVWKRuntimeSlot &slot) {
    DestroyDialogueVoice(slot.audioContext);
    if (slot.participantId)
        faceSetGeneratedLipCues(FindSubtitleCharacter(slot.participantId), nullptr, true);
}



void ReadCurrentSlotSubtitle(CVWKRuntimeSlot *slot) {
    if (g_currentSubtitleSlot != slot)
        return;
    HLib::Task *const scenarioTask = reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr)->scenarioTask;
    *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(scenarioTask->task_data_ptr) + kScenarioDialogueFlagsOffset) |= 0x40U;
    ReadSubtitleByID(scenarioTask, *slot);
}



void AssignReadSubtitleByID() {
    DisableSubtitles();
    for (CVWKRuntimeSlot &slot : g_pCVWK->slots) {
        if (!slot.callback)
            continue;
        CVWK_StopSlotVoice(slot);
        slot.callback = ReadCurrentSlotSubtitle;
    }
}



CVWK *CVWK_Initialize() {


    g_pCVWK = static_cast<CVWK *>(HLib_AllocHeapMemBlock(sizeof(CVWK), SHEN_NAME(CVWK)));
    if (!g_pCVWK)
        return nullptr;
    std::memset(g_pCVWK, 0, sizeof(g_pCVWK->slots));
    for (CVWKRuntimeSlot &slot : g_pCVWK->slots) {
        slot.cvfbBlock = HLib_AllocHeapMemBlock(0x840, SHEN_NAME(CVBF));
        if (slot.cvfbBlock) {
            const std::int32_t unused = -1;
            std::memcpy(static_cast<std::byte *>(slot.cvfbBlock) + 0x81c, &unused, sizeof(unused));
        }
    }
    g_CVRS = static_cast<CVRSTable *>(HLib_AllocHeapMemBlock(sizeof(CVRSTable), SHEN_NAME(CVRS)));
    if (g_CVRS)
        std::memset(g_CVRS, 0, sizeof(*g_CVRS));
    SetupVBUF();
    for (PackAdfsWrapper &wrapper : g_adfsWrappers)
        wrapper.status = -1;
    g_currentSubtitleSlot = nullptr;
    if (g_CVRS)
        InitializeSceneDialogueAudio();
    return g_pCVWK;
}


static void ApplyVoiceCameraNearClip(const char *voiceId) {
    if (!std::strcmp(voiceId, "E1049C010"))
        g_cameraNearClip = 0.04f;
    if (!std::strcmp(voiceId, "E1049A008"))
        g_cameraNearClip = 0.03f;
}

CVWKRuntimeSlot *ProcessSubtitleFile(const char *voiceId, std::uint32_t participantId, std::uint32_t flags, std::uint32_t *aliasState) {
    if (!voiceId)
        return nullptr;

    CVWK *const work = g_pCVWK;
    if (!work)
        return nullptr;

    char normalizedId[12]{};
    std::strncpy(normalizedId, voiceId, sizeof(normalizedId));
    for (char &value : normalizedId) {
        if (value >= 'a' && value <= 'z')
            value -= 0x20;
    }

    CVRSEntry *source = nullptr;
    const VoiceIndexRecord *record = nullptr;
    if (g_CVRS) {
        for (CVRSEntry &candidate : g_CVRS->entries) {
            if (!candidate.name[0])
                continue;
            record = FindVoiceIndexRecord(candidate, normalizedId, aliasState);
            if (aliasState && *aliasState == 1)
                return nullptr;
            if (record) {
                source = &candidate;
                break;
            }
        }
    }
    if (!source || !record) {
        return nullptr;
    }

    CVWKRuntimeSlot *slot = nullptr;
    for (CVWKRuntimeSlot &candidate : work->slots) {
        if (!candidate.callback) {
            slot = &candidate;
            break;
        }
    }
    if (g_currentSubtitleSlot && record->subtitleEntry != 0xffffU)
        DisableSubtitles();
    const bool bgm = std::strstr(source->name, "BGM") != nullptr;
    const auto clearSlot = [](CVWKRuntimeSlot &target) {
        void *const cvfbBlock = target.cvfbBlock;
        std::memset(&target, 0, sizeof(target));
        target.cvfbBlock = cvfbBlock;
    };
    if (slot)
        clearSlot(*slot);

    if (!slot || !StartDialogueVoice(*source, record->voiceEntry, slot->audioContext, bgm)) {
        slot = nullptr;
        for (CVWKRuntimeSlot &candidate : work->slots) {
            if (!candidate.field68) {
                slot = &candidate;
                break;
            }
        }
        if (!slot)
            return nullptr;
        SEQ_FreeFileHandles(*slot);
        clearSlot(*slot);
        StartDialogueVoice(*source, record->voiceEntry, slot->audioContext, bgm);
    }
    std::memcpy(slot->voiceId, normalizedId, sizeof(slot->voiceId));
    slot->fileOrSequence = source;
    slot->field68 = flags;
    slot->participantId = participantId == SHEN_NAME(null) ? 0 : participantId;

    if (record->subtitleEntry == 0xffffU) {
        slot->stateOrOwner = -1;
        slot->callback = SubtitlePlaybackCallback;
        if (!bgm)
            g_cvwkReadSubtitleOnPlay = 1;
        slot->fileHandle = nullptr;
        slot->clip = nullptr;
        slot->japaneseText = nullptr;
    } else {


        auto *const afs = static_cast<PackAdfsWrapper *>(source->afs);
        const std::uint32_t block = record->subtitleBytes >> 11U;
        LargeFileHandle *handle = afs ? LargeFile_OpenTocEntryHandle(afs->status, record->subtitleEntry) : nullptr;
        if (handle) {
            handle->offsetBytes = static_cast<std::int64_t>(block) << 11;
            (void)GetFileFromCachedBuffer(handle, 1, slot->cvfbBlock, 0);
        }
        auto *const cvfb = static_cast<std::byte *>(slot->cvfbBlock);
        slot->fileHandle = handle;
        std::strncpy(reinterpret_cast<char *>(cvfb) + 0x800, source->name, 23);
        const std::uint32_t entry = record->subtitleEntry;
        std::memcpy(cvfb + 0x818, &entry, sizeof(entry));
        std::memcpy(cvfb + 0x81c, &block, sizeof(block));
        const std::uint32_t sega = SHEN_NAME(SEGA);
        std::memcpy(cvfb, &sega, sizeof(sega));
        const std::uint32_t pending = 1;
        std::memcpy(cvfb + 0x820, &pending, sizeof(pending));
        slot->stateOrOwner = static_cast<std::int32_t>(record->subtitleBytes & 0x7ffU);
        slot->callback = SubtitleAuxLoadCallback;
    }

    if ((g_sceneId != 1 || g_areaId != SHEN_NAME(OP02)) && (S1SettingsFlags_Get() & 2U) == 0 && flags == 0)
        ProcessAudioData(slot, -999);
    g_currentSubtitleSlot = slot;
    ApplyVoiceCameraNearClip(voiceId);
    return slot;
}

const char *ReadStringFromStringTableById(const SubtitleStringId &id) {
    if (!g_stringTable || !g_stringTable->records || !g_stringTableData || !g_stringTableEnd || g_stringTable->count <= 0) {
        return "";
    }

    const std::byte *record = g_stringTable->records;
    std::size_t stride = 0;
    std::size_t offsetField = 0;
    switch (g_stringTable->version) {
    case 1:
        stride = 12;
        offsetField = 8;
        break;
    case 2:
        stride = 16;
        offsetField = 12;
        break;
    case 3:
        stride = 28;
        offsetField = 24;
        break;
    default:
        return "";
    }

    for (std::int32_t index = 0; index < g_stringTable->count; ++index, record += stride) {
        std::uint32_t words[3]{};
        std::memcpy(words, record, g_stringTable->version == 1 ? 8U : 12U);
        if (words[0] != id.word0 || words[1] != id.word1 || (g_stringTable->version != 1 && words[2] != id.word2)) {
            continue;
        }
        std::uint32_t offset = 0;
        std::memcpy(&offset, record + offsetField, sizeof(offset));
        if (offset >= static_cast<std::uint64_t>(g_stringTableEnd - g_stringTableData)) {
            return "";
        }
        return reinterpret_cast<const char *>(g_stringTableData + offset);
    }
    return "";
}

const char *ReadSubtitleStringById(const SubtitleStringId &id, bool japaneseAudio) {
    if (!japaneseAudio)
        return ReadStringFromStringTableById(id);

    const bool hasJapaneseAlias = std::any_of(g_specialSubtitleEntries.begin(), g_specialSubtitleEntries.end(),
        [&](const SpecialSubtitleEntry &entry) { return std::memcmp(&entry, &id, sizeof(id)) == 0; });
    if (!hasJapaneseAlias)
        return ReadStringFromStringTableById(id);

    SubtitleStringId alias{};
    alias.word0 = (id.word0 << 16U) | 0x5a5aU;
    alias.word1 = (id.word1 << 16U) | (id.word0 >> 16U);
    alias.word2 = (id.word2 << 16U) | (id.word1 >> 16U);
    return ReadStringFromStringTableById(alias);
}

void UI_PresentSearchFocusedItem(HLib::Task *scenarioTask, const char *stringId, bool visible) {
    UIBaseRuntime &subtitles = g_uiElements[kSubtitleUiStorageIndex];
    if (!visible) {
        if ((g_currUIState == -1 || g_currUIState == kSubtitleUiState) &&
            (!subtitles.isVisible || !subtitles.destroy || subtitles.destroy() == 1 || g_busy_reset == 1)) {
            subtitles.isVisible = 0;
        }
        return;
    }
    if (!stringId)
        return;

    SubtitleStringId id{};
    auto *const idBytes = reinterpret_cast<char *>(&id);
    std::size_t length = 0;
    while (length < sizeof(id) && stringId[length]) {
        idBytes[length] = stringId[length];
        ++length;
    }
    const char *const text = ReadStringFromStringTableById(id);

    if (scenarioTask && scenarioTask->task_data_ptr) {
        auto *const taskData = reinterpret_cast<std::byte *>(scenarioTask->task_data_ptr);
        auto *const flags = reinterpret_cast<std::uint32_t *>(taskData + kScenarioDialogueFlagsOffset);
        *flags = 2;
    }

    if (g_currUIState == -1 || g_currUIState == kSubtitleUiState) {
        if (!subtitles.isVisible) {
            std::int64_t (*callback)() = nullptr;
            if (subtitles.render) {
                subtitles.render();
                callback = subtitles.tick;
            } else if (subtitles.initialize) {
                subtitles.initialize();
                callback = subtitles.postInit;
            }
            if (callback) {
                g_waitingForNextTick = 0;
                callback();
                g_waitingForNextTick = 1;
            }
            subtitles.isVisible = 1;
        }
        subtitles.isHandledExternally = 0;
    }

    ProcessSubtitleText(text);
    g_subtitlePresentationColours = kFocusedItemSubtitleColours;
}

const char *InitAndRetrieveStringFromID(const char *stringId) {
    if (!stringId)
        return "";
    SubtitleStringId id{};
    auto *const idBytes = reinterpret_cast<char *>(&id);
    for (std::size_t length = 0; length < sizeof(id) && stringId[length]; ++length)
        idBytes[length] = stringId[length];
    return ReadStringFromStringTableById(id);
}





void ProcessSubtitleText(const char *text) {
    g_telephoneDisplayedNumber[0] = '\0';
    if (text) {
        const std::size_t length = std::strlen(text);
        if (length > 6 && text[0] == '[' && text[1] == '[' && text[2] == '[' && text[length - 1] == ']' && text[length - 2] == ']' && text[length - 3] == ']') {
            std::array<char, 64> stringId{};
            std::memcpy(stringId.data(), text + 3, std::min<std::size_t>(length - 6, stringId.size() - 1));
            text = InitAndRetrieveStringFromID(stringId.data());
        }
    }
    std::array<char, 512> &normalized = g_subtitleText;
    std::size_t output = 0;
    const auto append = [&normalized, &output](const char *from, std::size_t count) {
        const std::size_t copied = std::min(count, normalized.size() - 1 - output);
        std::memcpy(normalized.data() + output, from, copied);
        output += copied;
    };
    constexpr char kNewLineBytes[] = "\xa1\xf5";
    const char *cursor = text;
    while (cursor) {
        const char *marker = std::strstr(cursor, "=@");
        const char *const newline = std::strstr(cursor, kNewLineBytes);
        if (newline && (newline < marker || !marker))
            marker = newline;
        if (!marker) {
            append(cursor, std::strlen(cursor));
            break;
        }
        append(cursor, static_cast<std::size_t>(marker - cursor));
        if (static_cast<unsigned char>(*marker) == 0xa1)
            append("\n", 1);
        else if (*marker == '=')
            append("...", 3);
        cursor = marker + 2;
    }
    normalized[output] = '\0';
    for (std::array<char, 512> &line : g_subtitleLines)
        line.fill(0);
    const auto copyLine = [](std::size_t line, const char *from, std::size_t count) {
        std::memcpy(g_subtitleLines[line].data(), from, std::min(count, g_subtitleLines[line].size() - 1));
    };
    float scale = 0.33f;
    if (!std::strchr(normalized.data(), '\n')) {
        copyLine(1, normalized.data(), std::strlen(normalized.data()));
    } else {
        std::size_t line = 0;
        const char *from = normalized.data();
        while (from && line != 3) {
            const char *const next = std::strchr(from, '\n');
            copyLine(line++, from, next ? static_cast<std::size_t>(next - from) : std::strlen(from));
            from = next ? next + 1 : nullptr;
        }
        if (line == 2) {
            g_subtitleLines[2] = g_subtitleLines[1];
            g_subtitleLines[1].fill(0);
            scale = 0.48f;
        }
    }
    if (g_subtitleWindows[2])
        g_subtitleWindows[2]->layout.height.scale = scale;
    if (g_subtitleWindows[4])
        g_subtitleWindows[4]->layout.height.scale = scale;
}



void ApplySubtitlePresentationClass(std::uint32_t presentationClass) {
    g_subtitlePresentationColours = g_subtitleClassColours[std::min<std::size_t>(presentationClass, g_subtitleClassColours.size() - 1U)];
}

const std::array<std::array<char, 512>, 3> &CurrentSubtitleLines() {
    return g_subtitleLines;
}

void PrepareSubtitleFontAtlas(void *data, std::uint32_t, void *loadedData, std::uint64_t loadedSize, const char *) {
    auto *const request = static_cast<SubtitleFontAtlasRequest *>(data);
    if (!request || !request->layer || !loadedData || request->width == 0 || request->height == 0 ||
        loadedSize < static_cast<std::uint64_t>(request->width) * request->height) {
        return;
    }
    const Texture2DDescription description{request->width, request->height, 1, 1, 30, 0, 0, 1, 0, 0, loadedData};
    request->layer->texture.store(CreateTexture2D(&description), std::memory_order_release);
}

void InstallLegacyUIFontRecord(std::uint32_t language, const std::uint32_t *header, const std::byte *record, const char *path, std::uint32_t renderHandle) {
    const std::uint32_t font = header[1], image = header[2], glyphCount = header[4];
    if (language < g_uiFontPixelSizes.size() && font < 4 && image < 6)
        g_uiFontPixelSizes[language][font][image] = static_cast<float>(header[3]);
    const bool memoFont = font == 3 && image == 2;
    if ((font < 2 && image == 0) || memoFont) {
        SubtitleFontLayer &layer = memoFont ? g_memoFonts[language] : g_subtitleFonts[language][font];
        const std::size_t recordBytes = static_cast<std::size_t>(glyphCount) * sizeof(SubtitleFontGlyph);
        layer.glyphs.resize(glyphCount);
        std::memcpy(layer.glyphs.data(), record + 20, recordBytes);
        layer.lineHeight = static_cast<float>(header[3]);
        const std::byte *const metadata = record + 20 + recordBytes;
        SubtitleFontAtlasRequest &request = memoFont ? g_memoFontRequests[language] : g_subtitleFontRequests[language][font];
        request.layer = &layer;
        std::memcpy(&request.width, metadata + 460, sizeof(request.width));
        std::memcpy(&request.height, metadata + 464, sizeof(request.height));
        std::snprintf(request.path.data(), request.path.size(), "%s?font=%u&image=%u", path, font, image);
        const D3TShaderLoadTask task{.path = request.path.data(), .renderHandle = renderHandle, .thread = {PrepareSubtitleFontAtlas, &request}};
        PushShaderLoadToThreadQueue(&task);
    }
}

void ReadSubtitleGlyphsAndSubs(std::int32_t language) {
    if (language < 0 || language >= static_cast<std::int32_t>(g_subtitleData.size())) {
        language = 0;
    }

    if (!g_subtitleData[static_cast<std::size_t>(language)].empty()) {
        ReadFromCacheSubsData(g_subtitleData[static_cast<std::size_t>(language)]);
    } else {
        std::array<char, 64> subtitlePath{};
        std::snprintf(subtitlePath.data(), subtitlePath.size(), "%s.sub", GetSubsPathForLanguage(language));

        const std::uint16_t index = g_subtitleLoadRequestIndex.fetch_add(1, std::memory_order_seq_cst) & 0x1fffU;
        SubtitleLoadRequest &request = g_subtitleLoadRequests[index];
        request = {};
        request.language = language;

        D3TShaderLoadTask task{};
        task.path = subtitlePath.data();
        task.renderHandle = g_activeRenderHandle;
        task.thread.callback = InsertLocalisedText;
        task.thread.data = &request;
        PushShaderLoadToThreadQueue(&task);
    }

    for (std::int32_t glyphLanguage = 0; glyphLanguage < 7; ++glyphLanguage) {
        if (g_glyphLoadStates[static_cast<std::size_t>(glyphLanguage)] >= 1) {
            continue;
        }
        std::array<char, 64> glyphPath{};
        std::snprintf(glyphPath.data(), glyphPath.size(), "%s.glyphs", GetSubsPathForLanguage(glyphLanguage));
        LoadFontGlyphsAsync(glyphPath.data(), glyphLanguage);
    }
    g_uiLoadedLanguage = language;
}

void RetireLegacyUIFont(std::int32_t language) {
    if (static_cast<std::uint32_t>(language) >= g_subtitleFonts.size())
        return;
    const std::size_t previous = static_cast<std::size_t>(language);
    for (SubtitleFontLayer &layer : g_subtitleFonts[previous]) {
        InitTexture2DResource(layer.texture.exchange(0, std::memory_order_acq_rel));
        std::vector<SubtitleFontGlyph>{}.swap(layer.glyphs);
        layer.lineHeight = 0.0f;
    }
    SubtitleFontLayer &memo = g_memoFonts[previous];
    InitTexture2DResource(memo.texture.exchange(0, std::memory_order_acq_rel));
    std::vector<SubtitleFontGlyph>{}.swap(memo.glyphs);
    memo.lineHeight = 0.0f;
    FreeUIFontDefinition(language);
    g_uiFontPixelSizes[previous] = {};
}
}
