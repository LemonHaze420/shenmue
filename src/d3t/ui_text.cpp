#include "d3t/ui_internal.hpp"

#include "d3t/ui_runtime.hpp"
#if SHENMUE_IS_S1
#include "s1/d3t/d3t_content.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/d3t/d3t_threads.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/file_loader.hpp"
#include "s1/texture_loader.hpp"
#include "s1/mip_streaming.hpp"
#endif

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif

#if !SHENMUE_IS_S1

const char *GetStringForUILabel(const char *text) {
    if (!text)
        return "";
    SubtitleStringId id{};
    char *const bytes = reinterpret_cast<char *>(&id);
    for (std::size_t index = 0; index < sizeof(id) && text[index]; ++index)
        bytes[index] = text[index];
    return ReadStringFromStringTableById(id);
}

const char *ReadStringFromStringTableById(const SubtitleStringId &id) {
    std::array<char, 13> key{};
    std::memcpy(key.data(), &id, sizeof(id));
    return GetLocalizedSceneText(std::string_view(key.data())).data();
}
#endif

#if !SHENMUE_IS_S1




std::int32_t GetKeyForInputType(std::int32_t action, std::int32_t inputType) {
    const std::int32_t type = inputType == -1 ? GetCurrentUIInputType() : inputType;
    if (type <= 1)
        return static_cast<std::int32_t>(MapInputKeyToCurrentLayout(GetMenuKeyMapping(action)));
    if (type == 4) {
        const bool fightQte = IsGameUiSecondaryFlagSet(2) && GetBattleQteUIMode() != 2;
        if (action <= 3)
            return static_cast<std::int32_t>(MapInputKeyToCurrentLayout(fightQte ? GetFightKeyMapping(action) : GetQteKeyMapping(action)));
        if (fightQte)
            return static_cast<std::int32_t>(MapInputKeyToCurrentLayout(GetFightKeyMapping(action - 18)));
        return static_cast<std::int32_t>(MapInputKeyToCurrentLayout(GetGameKeyMapping(action)));
    }
    if (type == 5) {
        if (static_cast<std::uint32_t>(action - 18) <= 3U)
            return static_cast<std::int32_t>(MapInputKeyToCurrentLayout(GetQteKeyMapping(action - 18)));
        if (action <= 3)
            return static_cast<std::int32_t>(MapInputKeyToCurrentLayout(GetQteKeyMapping(action)));
        return static_cast<std::int32_t>(MapInputKeyToCurrentLayout(GetGameKeyMapping(action)));
    }
    if (type == 3 && action < 22)
        return static_cast<std::int32_t>(MapInputKeyToCurrentLayout(GetFightKeyMapping(action)));
    return static_cast<std::int32_t>(MapInputKeyToCurrentLayout(GetGameKeyMapping(action)));
}



std::int32_t ResolvePressedKey(std::int32_t action, std::int32_t inputType, std::int32_t doubleWidth) {
    std::int32_t key = 0;
    if (action < 50) {
        key = GetKeyForInputType(action, inputType);
    } else {
        switch (action) {
        case 50:
            key = static_cast<std::int32_t>(GetMenuKeyMapping(8));
            break;
        case 51:
            key = static_cast<std::int32_t>(GetMenuKeyMapping(9));
            break;
        case 52:
            key = static_cast<std::int32_t>(GetMenuKeyMapping(4));
            break;
        case 53:
            key = static_cast<std::int32_t>(GetMenuKeyMapping(5));
            break;
        case 54:
            key = static_cast<std::int32_t>(GetFightKeyMapping(16));
            break;
        case 55:
            key = static_cast<std::int32_t>(GetFightKeyMapping(17));
            break;
        case 56:
            key = static_cast<std::int32_t>(GetGameKeyMapping(8));
            break;
        case 57:
        case 67:
            return 30;
        case 58:
            return 31;
        case 59:
            return 32;
        case 60:
        case 63:
            return 33;
        case 61:
            return 34;
        case 62:
            return 35;
        case 64:
            return 55;
        case 65:
        case 66: {
            const std::int32_t slot = action - 65;
            const std::int32_t type = inputType == -1 ? GetCurrentUIInputType() : inputType;
            std::uint32_t mapped;
            if (type <= 1)
                mapped = GetMenuKeyMapping(slot);
            else if (type == 4)
                mapped = IsGameUiSecondaryFlagSet(2) && GetBattleQteUIMode() != 2 ? GetFightKeyMapping(slot) : GetQteKeyMapping(slot);
            else if (type == 5)
                mapped = GetQteKeyMapping(slot);
            else if (type == 3)
                mapped = GetFightKeyMapping(slot);
            else
                mapped = GetGameKeyMapping(slot);
            key = static_cast<std::int32_t>(MapInputKeyToCurrentLayout(mapped));
            break;
        }
        case 68:
            return 37;
        default:
            break;
        }
    }
    if (key > 256)
        return key == 257 ? 43 : key == 258 ? 42 : 26;
    switch (key) {
    case 256:
        return 41;
    case -1:
        return 40;
    case 8:
    case 9:
    case 16:
    case 17:
    case 20:
    case 32:
        return doubleWidth == 1 ? 27 : 26;
    case 13:
        return doubleWidth == 1 ? 29 : 26;
    case 37:
        return 36;
    case 38:
        return 37;
    case 39:
        return 38;
    case 40:
        return 39;
    default:
        return 26;
    }
}


std::int32_t ResolveDirectionalGlyph(std::int32_t action) {
    switch (action) {
    case 10:
    case 14:
    case 68:
        return 7;
    case 11:
    case 15:
        return 5;
    case 12:
    case 16:
        return 8;
    case 13:
    case 17:
        return 6;
    case 57:
    case 60:
    case 67:
        return 4;
    case 58:
    case 61:
        return 10;
    case 59:
    case 62:
        return 9;
    case 63:
        return 15;
    default:
        return -1;
    }
}


std::int32_t ResolvePlayStationGlyph(std::int32_t action) {
    switch (action) {
    case 0:
        return GetIsFreeInputType(-1) + 22;
    case 1:
        return 23 - GetIsFreeInputType(-1);
    case 2:
    case 20:
    case 24:
        return 24;
    case 3:
    case 21:
    case 25:
        return 25;
    case 4:
    case 52:
        return 46;
    case 5:
    case 53:
        return 47;
    case 8:
    case 50:
    case 56:
        return 44;
    case 9:
    case 51:
        return 45;
    case 18:
        return GetIsFreeInputType(4) + 22;
    case 19:
        return 23 - GetIsFreeInputType(4);
    case 22:
        return GetIsFreeInputType(3) + 22;
    case 23:
        return 23 - GetIsFreeInputType(3);
    case 54:
        return 48;
    case 55:
        return 50;
    case 64:
        return GetIsFreeInputType(0) + 22;
    case 66:
        return 23;
    default:
        return 22;
    }
}


std::int32_t ResolveXboxGlyph(std::int32_t action) {
    switch (action) {
    case 0:
        return GetIsFreeInputType(-1) + 18;
    case 1:
        return 19 - GetIsFreeInputType(-1);
    case 2:
    case 20:
    case 24:
        return 20;
    case 3:
    case 21:
    case 25:
        return 21;
    case 4:
    case 52:
        return 2;
    case 5:
    case 53:
        return 3;
    case 8:
    case 50:
    case 56:
        return 0;
    case 9:
    case 51:
        return 1;
    case 18:
        return GetIsFreeInputType(4) + 18;
    case 19:
        return 19 - GetIsFreeInputType(4);
    case 22:
        return GetIsFreeInputType(3) + 18;
    case 23:
        return 19 - GetIsFreeInputType(3);
    case 54:
        return 11;
    case 55:
        return 13;
    case 64:
        return GetIsFreeInputType(0) + 18;
    case 66:
        return 19;
    default:
        return 18;
    }
}




std::int32_t ResolveButtonIconGlyph(std::int32_t action, std::int32_t controlMethod, std::int32_t inputType, std::int32_t doubleWidth) {
    if (controlMethod == -1)
        controlMethod = static_cast<std::int32_t>(GetIsKeyboardActive());
    if (g_debugUnknown0 == 1 || (static_cast<std::uint32_t>(g_debugUnknown0 - 2) > 1U && controlMethod == 1))
        return ResolvePressedKey(action, inputType, doubleWidth);
    if (const std::int32_t directional = ResolveDirectionalGlyph(action); directional != -1)
        return directional;
    return g_debugUnknown0 == 3 ? ResolvePlayStationGlyph(action) : ResolveXboxGlyph(action);
}
#endif

#if SHENMUE_IS_S2
// @verified - p
void UI_SetFontFamily(std::uint32_t family) {
    if (static_cast<std::uint32_t>(g_uiFontFace) != family && family < 4U) {
        g_uiFontFace = static_cast<std::int32_t>(family);
        UI_SelectFont();
    }
}
#endif
void UI_SelectFont() {
#if SHENMUE_IS_S1
    std::int32_t language = g_uiLoadedLanguage;
    if (language == -1) {
        if (g_uiStringTable) {
            g_uiStringTable = nullptr;
            std::free(g_uiStringBuffer);
            g_uiStringBufferSize = 0;
        }
        g_uiLoadedLanguage = 0;
        ReadSubtitleGlyphsAndSubs(0);
        language = g_uiLoadedLanguage;
    }
#else
    const std::int32_t language = GetLoadingScreenTextType(ResetSceneTextLanguage());
#endif
    if (g_uiFontFace > -1 && g_uiFontSize > -1) {
        UIFontFace &face = g_uiFonts[static_cast<std::size_t>(language)][static_cast<std::size_t>(g_uiFontFace)];
        if (nk_font *const font = face.fonts[static_cast<std::size_t>(g_uiFontSize)]) {
            font->handle.height = g_uiFontScale * face.sizes[static_cast<std::size_t>(g_uiFontSize)];
            GNuklearWindow.stacks.fonts.head = 0;
            GNuklearWindow.style.font = &font->handle;
        }
    }
}


void CreateFontTexture(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *) {
    const std::uint32_t texture = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(data));
    d3t::CreateMipmappedScaledTexture(texture, 0, 1, loadedData, static_cast<std::uint32_t>(loadedSize));
    d3t::ProcessThreadStates(contentHandle, 0);
}

void FreeUIFontDefinition(std::int32_t language) {
    if (static_cast<std::uint32_t>(language) >= g_uiFonts.size())
        return;
    for (UIFontFace &face : g_uiFonts[language]) {
        for (std::size_t image = 0; image < face.atlases.size(); ++image) {
            d3t::InitTexture2DResource(face.textures[image]);
            face.textures[image] = 0;
            std::memset(&face.atlases[image], 0, sizeof(face.atlases[image]));
        }
    }
#if SHENMUE_IS_S1
    std::vector<std::byte>{}.swap(g_fontDefinitions[language]);
#else
    std::free(g_uiFontDefinitions[language]);
    g_uiFontDefinitions[language] = nullptr;
#endif
}
#if SHENMUE_IS_S1
void FontLoader() {
    std::int32_t language = g_requestedUiLanguage;
    if (static_cast<std::uint32_t>(language) > 6U) {
        language = GetCurrentLanguageId(static_cast<GameLanguage>(-1));
        g_requestedUiLanguage = language;
    }
#else
void LoadUIFontLanguage(std::int32_t language) {
    if (static_cast<std::uint32_t>(language) > 6U) {
        language = GetLoadingScreenTextType(-1);
        g_s2SelectedSubtitleLanguage = language;
    }
#endif
    if (g_fontLanguageAvailable[static_cast<std::size_t>(language)] != 1 && g_forceFontLanguageAvailable != 1)
        language = 0;
#if SHENMUE_IS_S1
    if (g_uiLoadedLanguage == -1) {
        if (g_uiStringTable) {
            g_uiStringTable = nullptr;
            std::free(g_uiStringBuffer);
            g_uiStringBufferSize = 0;
        }
        ReadSubtitleGlyphsAndSubs(English);
    }
    const std::int32_t previousLanguage = g_uiLoadedLanguage;
#else
    const std::int32_t previousLanguage = ResetSceneTextLanguage();
#endif
    if (language == previousLanguage)
        return;
#if SHENMUE_IS_S1
    if (g_uiStringTable) {
        g_uiStringTable = nullptr;
        std::free(g_uiStringBuffer);
        g_uiStringBufferSize = 0;
    }
    ReadSubtitleGlyphsAndSubs(language);
    RetireLegacyUIFont(previousLanguage);
#else
    LoadLocalizedSceneText(language);
    FreeUIFontDefinition(previousLanguage);
#endif
    std::uint32_t handle = d3t::d3t_AllocateRenderHandle();
#if SHENMUE_IS_S1
    SelectAppropriateFontDefinition(static_cast<GameLanguage>(language), handle);
#else
    SelectAppropriateFontDefinition(language, handle);
#endif
    while (d3t::d3t_IsRenderHandlePending(handle))
        d3t::ProcessRemappings();
#if SHENMUE_IS_S1
    d3t::d3t_ReleaseRenderHandle(handle);
#else
    d3t::d3t_CloseRenderHandle(handle);
    UpdateCollectionMapLanguage(language);
    if (IsHelpOverlayVisible(UIElement::SubMenuOptions)) {
        if (IsHelpOverlayVisible(UIElement::MainMenu))
            RefreshMainMenuTitle();
        RefreshOptionsLanguage();
    }
#endif
}
#if SHENMUE_IS_S2
void FontLoader() {
    if (g_pendingUIFontLanguageValid == 1) {
        g_s2SelectedSubtitleLanguage = g_pendingUIFontLanguage;
        g_pendingUIFontLanguageValid = 0;
    }
    LoadUIFontLanguage(g_s2SelectedSubtitleLanguage);
    UI_BeginFrame();
}
#endif

#if SHENMUE_IS_S1
namespace d3t {
void LoadFont(void *data, std::uint32_t, void *loadedData, std::uint64_t loadedSize, const char *sourcePath) {
#else
void LoadUIFont(void *data, std::uint32_t, void *loadedData, std::uint64_t loadedSize, const char *sourcePath) {
#endif
    const std::uint32_t *const words = static_cast<const std::uint32_t *>(data);
    const std::uint32_t language = words[1];
    const std::byte *const first = static_cast<const std::byte *>(loadedData);
    const std::size_t size = static_cast<std::size_t>(loadedSize);
#if SHENMUE_IS_S1
    g_fontDefinitions[language].assign(first, first + size);
    const std::byte *const base = g_fontDefinitions[language].data();
#else
    g_uiFontDefinitions[language] = static_cast<std::byte *>(loadedData);
    const std::byte *const base = first;
#endif
    std::uint64_t glyphBytes = 0;
    std::memcpy(&glyphBytes, first + 4, sizeof(glyphBytes));
    if (glyphBytes > size - 12)
        return;
    const std::byte *ranges = base + 12;
    std::size_t cursor = 12 + static_cast<std::size_t>(glyphBytes);
    while (cursor + 20 <= size) {
        std::uint32_t header[5]{};
        std::memcpy(header, first + cursor, sizeof(header));
        if (header[0] == kFontDefinitionChunkTag) {
#if SHENMUE_IS_S1
            break;
#else
            std::memcpy(&glyphBytes, first + cursor + 4, sizeof(glyphBytes));
            ranges = base + cursor + 12;
            cursor += 12 + static_cast<std::size_t>(glyphBytes);
            continue;
#endif
        }
        if (header[0] != 0x9abcdef0U)
            return;
        const std::uint32_t font = header[1], image = header[2], glyphCount = header[4];
        const std::size_t recordBytes = static_cast<std::size_t>(glyphCount) * sizeof(struct nk_font_glyph);
        const std::byte *const record = base + cursor;
        const std::byte *const tail = record + kFontRecordHeaderSize + recordBytes;
        std::array<std::uint32_t, 2> textureSize{};
        std::memcpy(textureSize.data(), tail + kFontRecordTextureSizeOffset, sizeof(textureSize));
#if SHENMUE_IS_S1
        const char *const path = sourcePath ? sourcePath : kUIFontDefinitionPaths[language <= 6 ? language : 0];
#else
        const char *const path = sourcePath;
#endif
        std::array<char, 256> texturePath{};
        std::snprintf(texturePath.data(), texturePath.size(), "%s?font=%d&image=%d", path, static_cast<int>(font), static_cast<int>(image));
        const d3t::Texture2DDescription description{.width = textureSize[0],
            .height = textureSize[1],
            .arraySize = 1,
            .mipLevels = 1,
            .format = 30,
            .bindFlags = 0,
            .flags = 8,
            .sampleCount = 1,
            .sampleQuality = 0,
            .reserved_24 = 0,
            .initialData = nullptr};
        const std::uint32_t texture = d3t::CreateTexture2D(&description);
        const d3t::D3TShaderLoadTask textureTask{.path = texturePath.data(),
            .renderHandle = words[0],
            .thread = {CreateFontTexture, reinterpret_cast<void *>(static_cast<std::uintptr_t>(texture))}};
        d3t::PushShaderLoadToThreadQueue(&textureTask);
        UIFontFace &face = g_uiFonts[language][font];
        const nk_handle textureHandle{.id = static_cast<int>(texture)};
        face.fonts[image] = Nuklear_LoadFontRecord(face.atlases[image], face.configs[image], record, ranges, textureHandle, g_uiFontNullTexture);
        face.textures[image] = texture;
        face.configs[image] = *face.atlases[image].config;
        face.sizes[image] = static_cast<float>(header[3]);
        const std::size_t chunkBytes = kFontRecordHeaderSize + recordBytes + kFontRecordTailSize;
#if SHENMUE_IS_S1
        if (recordBytes / sizeof(struct nk_font_glyph) != glyphCount || chunkBytes > size - cursor)
            return;
        InstallLegacyUIFontRecord(language, header, first + cursor, path, words[0]);
#endif
        cursor += chunkBytes;
    }
}
#if SHENMUE_IS_S1
} // namespace d3t
#endif


void UI_SetLabelTokenKey(d3t::UILabelToken &token) {
    token.key.fill(0);
    for (std::size_t index = 0; token.token && index < token.key.size() && token.token[index]; ++index)
        token.key[index] = token.token[index];
}


void UI_SetTokenKeyFromString(d3t::UILabelToken &token, const char *text) {
    std::array<char, 12> key{};
    for (std::size_t index = 0; index < key.size() && text[index]; ++index)
        key[index] = text[index];
    token.key = key;
}




void UI_ResolveLabelText(d3t::UILabelWidget &label) {
    const char *text = "";
    const std::int32_t bindingIndex = label.dataBinding;
    const d3t::UIViewVariable *variable = nullptr;
    if (bindingIndex != -1) {
        const UIDataBinding &binding = g_uiDataBindings[static_cast<std::size_t>(bindingIndex)];
        if (static_cast<std::uint32_t>(g_uiDrawViewSlot) <= 0x4bU && static_cast<std::uint32_t>(binding.variable) <= 0x14U)
            variable = &g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)].variables[static_cast<std::size_t>(binding.variable)];
    }
    if (label.token.alreadyLocalized) {
        if (bindingIndex == -1)
            return;
        label.text = variable ? variable->value.stringValue : text;
        return;
    }
    if (bindingIndex != -1)
        UI_SetTokenKeyFromString(label.token, variable ? variable->value.stringValue : "");
    if (label.token.key[0] || label.token.key[1] || label.token.key[2] || label.token.key[3]) {
        SubtitleStringId id{};
        std::memcpy(&id, label.token.key.data(), sizeof(id));
        label.text = ReadStringFromStringTableById(id);
        return;
    }
    label.text = text;
}


std::int32_t UI_FindLabelSplit(const char *text, std::int32_t length) {
    std::int32_t split = -1;
    std::int32_t nearest = 0x7fffffff;
    for (std::int32_t index = 0; index < length; ++index) {
        if (text[index] != ' ')
            continue;
        const std::int32_t distance = index - length / 2 < 0 ? length / 2 - index : index - length / 2;
        if (distance < nearest) {
            split = index;
            nearest = distance;
        }
    }
    return split;
}





UILabelFit UI_FitLabelScale(nk_context &context, const char *text, std::int32_t length, nk_flags alignment, const UILayoutRect &rect, float width, float height,
    std::int32_t allowSplit) {
    const float savedScale = g_uiFontScale;
    g_uiFontScale = 1.0f;
    UILabelFit fit{.scale = 1.0f, .split = 0};
    UI_SelectFont();
    if (length == -1)
        length = static_cast<std::int32_t>(std::strlen(text));
    fit.scale = 0.33f;
    if (width > rect.width && width != -1.0f)
        fit.scale = rect.width / width;
    if (height <= rect.height || height == -1.0f) {
        fit.scale = 1.0f;
    } else if (rect.height / height > fit.scale) {
        fit.scale = rect.height / height;
    }
    g_uiFontScale = fit.scale;
    UI_SelectFont();
    const nk_user_font *const font = context.style.font;
    const float textWidth = font->width(font->userdata, font->height, text, length);
    const float fontHeight = font->height;
    if (textWidth > rect.width && width != -1.0f)
        fit.scale = (rect.width / textWidth) * fit.scale;
    if (fontHeight > rect.height && height != -1.0f && fit.scale > rect.height / fontHeight)
        fit.scale = fit.scale * (rect.height / fontHeight);
    if (fit.scale < 0.33f)
        fit.scale = 0.33f;
    g_uiFontScale = fit.scale;
    UI_SelectFont();
    if (allowSplit == 1 && fit.scale < 0.5f && !g_uiLabelSplitting) {
        g_uiLabelSplitting = 1;
        const std::int32_t split = UI_FindLabelSplit(text, length);
        if (split == -1) {
            if (g_waitingForNextTick == 1)
                nk_text_colored(&context, text, length, alignment, context.style.text.color);
        } else {
            fit.split = 1;
            const float halfHeight = rect.height * 0.5f;
            const UILayoutRect first{.x = rect.x, .y = rect.y, .width = rect.width, .height = halfHeight};
            if (g_waitingForNextTick == 1 && context.current && context.current->layout)
                context.current->layout->row.item = nk_rect(first.x, first.y, first.width, first.height);
            fit.scale = UI_FitLabelScale(context, text, split, alignment, first, width, height * 0.5f, 1).scale;
            const UILayoutRect second{.x = rect.x, .y = halfHeight + rect.y, .width = rect.width, .height = halfHeight};
            if (g_waitingForNextTick == 1 && context.current && context.current->layout)
                context.current->layout->row.item = nk_rect(second.x, second.y, second.width, second.height);
            const float secondScale = UI_FitLabelScale(context, text + split + 1, length - split - 1, alignment, second, width, height * 0.5f, 1).scale;
            if (fit.scale > secondScale)
                fit.scale = secondScale;
        }
        g_uiLabelSplitting = 0;
    }
    g_uiFontScale = savedScale;
    UI_SelectFont();
    return fit;
}




UILayoutRect UI_DrawLabelText(nk_context &context, const char *text, std::int32_t length, nk_flags alignment, const UILayoutRect &rect, const UILabelFit &fit) {
    const float savedScale = g_uiFontScale;
    UILayoutRect placed = rect;
    if (length == -1)
        length = static_cast<std::int32_t>(std::strlen(text));
    g_uiFontScale = fit.scale;
    UI_SelectFont();
    const nk_user_font *const font = context.style.font;
    const std::int32_t split = fit.split == 1 ? UI_FindLabelSplit(text, length) : -1;
    if (split == -1) {
        if (g_waitingForNextTick == 1)
            nk_text_colored(&context, text, length, alignment, context.style.text.color);
        placed.width = font->width(font->userdata, font->height, text, length);
        placed.height = font->height;
    } else {
        const float halfHeight = rect.height * 0.5f;
        if (g_waitingForNextTick == 1) {
            if (context.current && context.current->layout)
                context.current->layout->row.item = nk_rect(rect.x, rect.y, rect.width, halfHeight);
            nk_text_colored(&context, text, split, alignment, context.style.text.color);
        }
        placed.width = font->width(font->userdata, font->height, text, split);
        placed.height = font->height;
        if (g_waitingForNextTick == 1) {
            if (context.current && context.current->layout)
                context.current->layout->row.item = nk_rect(rect.x, halfHeight + rect.y, rect.width, halfHeight);
            nk_text_colored(&context, text + split + 1, length - split - 1, alignment, context.style.text.color);
        }
        const float secondWidth = font->width(font->userdata, font->height, text + split + 1, length - split - 1);
        if (secondWidth > placed.width)
            placed.width = secondWidth;
        placed.height = placed.height + font->height;
    }
    if (alignment & NK_TEXT_ALIGN_MIDDLE)
        placed.y = (rect.height * 0.5f + rect.y) - placed.height * 0.5f;
    else if (alignment & NK_TEXT_ALIGN_BOTTOM)
        placed.y = (rect.height + rect.y) - placed.height;
    if (alignment & NK_TEXT_ALIGN_CENTERED)
        placed.x = (rect.width * 0.5f + rect.x) - placed.width * 0.5f;
    else if (alignment & NK_TEXT_ALIGN_RIGHT)
        placed.x = (rect.width + rect.x) - placed.width;
    g_uiFontScale = savedScale;
    UI_SelectFont();
    return placed;
}


UILayoutRect UI_FitAndDrawLabelText(nk_context &context, const char *text, std::int32_t length, nk_flags alignment, const UILayoutRect &rect, float width,
    float height, std::int32_t allowSplit) {
    const UILabelFit fit = UI_FitLabelScale(context, text, length, alignment, rect, width, height, allowSplit);
    return UI_DrawLabelText(context, text, length, alignment, rect, fit);
}

// A colour's channels scaled by the window colour's (each window channel over 255, the window alpha times its alpha first).
#if SHENMUE_IS_S2
std::uint32_t UI_GetWindowColour(const d3t::UIWindow *window) {
    const std::uint32_t white = ConstructColour(255, 255, 255, 255);
    if (!window)
        return white;
    std::array<std::uint8_t, 4> channels{};
    std::memcpy(channels.data(), &window->colour, sizeof(window->colour));
    return ConstructColour(channels[0], channels[1], channels[2], static_cast<std::int32_t>(static_cast<float>(channels[3]) * window->alpha));
}

std::uint32_t UI_ModulateColour(std::uint32_t colour, std::uint32_t tint) {
    std::array<std::uint8_t, 4> tintChannels{};
    std::array<std::uint8_t, 4> channels{};
    std::memcpy(tintChannels.data(), &tint, sizeof(tint));
    std::memcpy(channels.data(), &colour, sizeof(colour));
    for (std::size_t channel = 0; channel < channels.size(); ++channel) {
        const float scale = static_cast<float>(tintChannels[channel]) / 255.0f;
        channels[channel] = static_cast<std::uint8_t>(static_cast<std::int32_t>(scale * static_cast<float>(channels[channel])));
    }
    std::uint32_t result{};
    std::memcpy(&result, channels.data(), sizeof(result));
    return result;
}
#endif
std::uint32_t UI_TintByWindow(const d3t::UIWindow &window, std::uint32_t colour) {
#if SHENMUE_IS_S2
    return UI_ModulateColour(colour, UI_GetWindowColour(&window));
#else
    std::array<std::uint8_t, 4> windowChannels{};
    std::memcpy(windowChannels.data(), &window.colour, sizeof(windowChannels));
    const std::uint32_t tint = ConstructColour(
        windowChannels[0], windowChannels[1], windowChannels[2], static_cast<std::int32_t>(static_cast<float>(windowChannels[3]) * window.alpha));
    std::array<std::uint8_t, 4> tintChannels{};
    std::memcpy(tintChannels.data(), &tint, sizeof(tintChannels));
    std::array<std::uint8_t, 4> channels{};
    std::memcpy(channels.data(), &colour, sizeof(channels));
    for (std::size_t channel = 0; channel < channels.size(); ++channel) {
        const float scale = static_cast<float>(tintChannels[channel]) / 255.0f;
        channels[channel] = static_cast<std::uint8_t>(static_cast<std::int32_t>(scale * static_cast<float>(channels[channel])));
    }
    std::uint32_t tinted = 0;
    std::memcpy(&tinted, channels.data(), sizeof(tinted));
    return tinted;
#endif
}






void UI_DrawLabel(nk_context &context, std::int32_t labelIndex, std::int32_t widget) {
    if (static_cast<std::uint32_t>(g_uiDrawViewSlot) > 0x4bU || static_cast<std::uint32_t>(labelIndex) > 0x1ffU)
        return;
    d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[static_cast<std::size_t>(g_uiDrawViewSlot)];
    d3t::UILabelWidget &label = g_uiLabelWidgets[static_cast<std::size_t>(labelIndex)];
    d3t::UIWindow *const window = UI_GetWindow(label.window);
    if (!window || !label.field44)
        return;
    if (g_uiFontFace != label.fontFamily && static_cast<std::uint32_t>(label.fontFamily) < 4U) {
        g_uiFontFace = label.fontFamily;
        UI_SelectFont();
    }
    if (g_uiFontSize != label.size && static_cast<std::uint32_t>(label.size) < 6U) {
        g_uiFontSize = label.size;
        UI_SelectFont();
    }
    UI_ResolveLabelText(label);
    if (!label.text)
        return;

    const std::int32_t itemIndex = widget > 0 ? view.widgetPoolIndices[static_cast<std::size_t>(widget - 1)] : view.widgetKinds.back();
    if (static_cast<std::uint32_t>(itemIndex) > 0x3ffU)
        return;
    const d3t::UILayoutItem &item = g_uiLayoutItems[static_cast<std::size_t>(itemIndex)];
    d3t::UIWindow *const itemWindow = UI_GetWindow(item.window);
    if (!itemWindow)
        return;
    UILayoutRect windowRect{};
    UI_GetWindowRect(itemWindow, windowRect);
    float width = 0.0f;
    float height = 0.0f;
    UI_ScaleSize(width, height, item.scaleStyle, windowRect.width, windowRect.height, item.width * 1920.0f, item.height * 1080.0f, itemWindow);
    float x = 0.0f;
    float y = 0.0f;
    UI_AlignInArea(item.alignX, item.alignY, width, height, x, y, windowRect.width, windowRect.height);
    const float scaledWidth = width * item.scaleX;
    const float scaledHeight = height * item.scaleY;
    const float authoredWidth = item.width * itemWindow->layout.width.value.fraction * 1920.0f;
    const float authoredHeight = item.height * itemWindow->layout.height.value.fraction * 1080.0f;
    x = (item.offsetX + item.posX) * windowRect.width + x;
    y = (item.offsetY + item.posY) * windowRect.height + y;
    const std::int32_t safeHeight = static_cast<std::int32_t>(UI_SafeAreaScale() * 1080.0f);
    x = x + (width - scaledWidth) * 0.5f;
    y = y + (height - scaledHeight) * 0.5f;
    const std::int32_t safeWidth = static_cast<std::int32_t>(static_cast<float>(UI_GetAspectWidth()) - (1080.0f - static_cast<float>(safeHeight)));
    d3t::UIViewVariable *const scaleVariable = label.scaleVar != -1 ? &view.variables[static_cast<std::size_t>(label.scaleVar)] : nullptr;
    if (safeWidth != label.scaleScreenWidth || safeHeight != label.scaleScreenHeight) {

        std::byte *const variable = reinterpret_cast<std::byte *>(view.variables.data()) +
            static_cast<std::ptrdiff_t>(label.scaleVar) * static_cast<std::ptrdiff_t>(sizeof(d3t::UIViewVariable));
        std::memmove(variable + offsetof(d3t::UIViewVariable, value), variable + offsetof(d3t::UIViewVariable, initialValue), sizeof(d3t::UIViewVariableValue));
    }
    const UILayoutRect rect{.x = x, .y = y, .width = scaledWidth, .height = scaledHeight};
    UILabelFit fit = UI_FitLabelScale(context, label.text, -1, label.alignment, rect, authoredWidth, authoredHeight, 1);
    label.scaleScreenWidth = safeWidth;
    label.scaleScreenHeight = safeHeight;
    if (scaleVariable) {
        if (scaleVariable->value.floatValue > fit.scale && label.scaleVarAssign == 1)
            scaleVariable->value.floatValue = fit.scale;
        else
            fit.scale = scaleVariable->value.floatValue;
    }
    if (label.hasStroke == 1) {
        if (g_uiFontFace != 1) {
            g_uiFontFace = 1;
            UI_SelectFont();
        }
        const std::uint32_t stroke = UI_TintByWindow(*window, label.strokeColour);
        std::memcpy(&context.style.text.color, &stroke, sizeof(stroke));
        UI_DrawLabelText(context, label.text, -1, label.alignment, rect, fit);
        if (g_uiFontFace != 0) {
            g_uiFontFace = 0;
            UI_SelectFont();
        }
    }
    const nk_color savedColour = context.style.text.color;
    const std::uint32_t colour = UI_TintByWindow(*window, label.colour);
    std::memcpy(&context.style.text.color, &colour, sizeof(colour));
    UI_DrawLabelText(context, label.text, -1, label.alignment, rect, fit);
    context.style.text.color = savedColour;
}

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
