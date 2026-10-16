#include "d3t/ui_internal.hpp"

#if SHENMUE_IS_S1
#include "s1/d3t/d3t_texture.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/texture_loader.hpp"
#include "s1/texture_cache.hpp"
#include "s1/shader_initialization.hpp"
#include "s1/d3t/d3t_content.hpp"
#endif

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif

#if !SHENMUE_IS_S1

void TextureLoadReleaseNullsub(void *) {
}

void InitTexture2dLoaded(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *) {
    std::uint32_t *const payload = static_cast<std::uint32_t *>(data);
    payload[0] = d3t::CreateImageTextureFromMemory(loadedData, static_cast<std::size_t>(loadedSize), payload[1] != 0);
    d3t::AssignShaderCallback(payload[0], TextureLoadReleaseNullsub, nullptr);
    d3t::ProcessThreadStates(contentHandle, 0);
}

std::uint32_t InitTexture2d(const char *path, std::uint32_t linearColour, std::uint32_t renderHandle) {
    const std::uint16_t index = g_textureLoadRequestIndex.fetch_add(1, std::memory_order_seq_cst) & 0x1fffU;
    TextureLoadRequest &request = g_textureLoadRequests[index];
    const std::uint16_t generation = ++request.generation;
    if (generation) {
        request.texture = 0;
        request.linearColour = linearColour;
        const d3t::D3TShaderLoadTask task{path, renderHandle, 0, {InitTexture2dLoaded, &request.texture}, nullptr, 0, 0, {}};
        d3t::PushShaderLoadToThreadQueue(&task);
    }
    return (static_cast<std::uint32_t>(generation) << 16U) | index;
}

std::uint32_t GetShaderDataByIndexInternal(std::uint32_t requestHandle) {
    const TextureLoadRequest &request = g_textureLoadRequests[static_cast<std::uint16_t>(requestHandle)];
    return static_cast<std::uint16_t>(requestHandle >> 16U) == request.generation ? request.texture : 0;
}

std::uint32_t GetShaderDataByIndex(std::uint32_t requestHandle) {
    return GetShaderDataByIndexInternal(requestHandle);
}

std::uint32_t SetupUsageForTexture(const char *path, std::uint32_t renderHandle) {
    std::array<char, 256> request{};
#if defined(_WIN32)
    if (std::strchr(path, '?'))
        strcpy_s(request.data(), request.size(), path);
    else
        sprintf_s(request.data(), request.size(), "%s%s", path, "?usage=0");
#else
    if (std::strchr(path, '?'))
        std::snprintf(request.data(), request.size(), "%s", path);
    else
        std::snprintf(request.data(), request.size(), "%s%s", path, "?usage=0");
#endif
    return InitTexture2d(request.data(), 0, renderHandle);
}

void UI_RecordImageRequest(std::int32_t image, std::uint32_t requestHandle) {
    if (g_uiImageLoadDiagnosticIndex > -1) {
        UIViewImageLoadDiagnostic &record = g_uiImageLoadDiagnostics[static_cast<std::size_t>(g_uiImageLoadDiagnosticIndex)];
        record.imageIndices[static_cast<std::size_t>(image)] = image;
        record.requestHandles[static_cast<std::size_t>(image)] = requestHandle;
        ++record.count;
    }
}

void UI_RecordImageFailure(std::int32_t image, std::uint32_t requestHandle) {
    if (g_uiImageLoadDiagnosticIndex > -1) {
        UIViewImageLoadDiagnostic &record = g_uiImageLoadDiagnostics[static_cast<std::size_t>(g_uiImageLoadDiagnosticIndex)];
        record.missing[static_cast<std::size_t>(image)] = 1;
        record.ready[static_cast<std::size_t>(image)] = 0;
        record.failureHandles[static_cast<std::size_t>(image)] = requestHandle;
    }
}

void UI_RecordImageReady(std::int32_t image) {
    if (g_uiImageLoadDiagnosticIndex > -1) {
        UIViewImageLoadDiagnostic &record = g_uiImageLoadDiagnostics[static_cast<std::size_t>(g_uiImageLoadDiagnosticIndex)];
        record.ready[static_cast<std::size_t>(image)] = 1;
        record.missing[static_cast<std::size_t>(image)] = 0;
    }
}

void UI_RecordImageViewSlot(std::int32_t slotIndex) {
    if (g_uiImageLoadDiagnosticIndex > -1)
        g_uiImageLoadDiagnostics[static_cast<std::size_t>(g_uiImageLoadDiagnosticIndex)].viewSlot = slotIndex;
}
#endif



void ResetUIViewSlot(std::int32_t slotIndex) {
    if (slotIndex == -1)
        return;
    d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(slotIndex)];
    if (!g_uiReadStateInitialized)
        slot.keepLoaded = 0;
    if (slot.keepLoaded == 1)
        return;
    slot.imagesLoading = 0;
    slot.widgetCount = 0;
    slot.keepLoaded = 0;
    for (std::int32_t widget = 0; widget < static_cast<std::int32_t>(d3t::kUIViewWidgetCount); ++widget)
        UI_FreeViewWidget(slotIndex, widget);
    for (std::size_t index = 0; index < slot.images.size(); ++index) {
        d3t::UIViewImage &image = slot.images[index];
        image.textureIndex = slotIndex * static_cast<std::int32_t>(kUIViewImageCapacity) + static_cast<std::int32_t>(index);
        image.name = "";
        image.ready = 0;
        image.widthScale = 0.0f;
        image.heightScale = 0.0f;
        if (slot.inUse == 1) {
            std::uint32_t &texture = g_uiViewTextures[static_cast<std::size_t>(image.textureIndex)];
#if SHENMUE_IS_S1
            std::uint32_t width = 0;
            std::uint32_t height = 0;
            if (texture && GetTexture2DDimensions(texture, &width, &height) && width && height)
                InitTexture2DResource(texture);
#else
            const std::uint32_t resolved = GetShaderDataByIndex(texture);
            if (resolved)
                InitTexture2DResource(resolved);
#endif
            texture = 0;
        }
        image.textureHandle = -1;
        image.width = 0;
        image.height = 0;
        image.region = {};
        image.rotate = 0;
        image.angle = 0.0f;
        image.colour = 0xffffffffU;
    }
    slot.inUse = 0;
    for (d3t::UIViewVariable &variable : slot.variables) {
        variable.used = 0;
        variable.name = "";
        variable.type = d3t::UIViewVariableType::Int;
        variable.value.intValue = 0;
    }
}




void ReadImages(d3t::UIViewSlot<kUIViewImageCapacity> &slot) {
    cJSON *const table = cJSON_GetObjectItem(g_parsedUIDocument, "Images");
    if (!table)
        return;
    const int count = cJSON_GetArraySize(table);
    slot.imagesLoading = 1;
    for (int index = 0; index < count; ++index) {
        const cJSON *const member = cJSON_GetArrayItem(table, index);
        d3t::UIViewImage &image = slot.images[static_cast<std::size_t>(index)];
#if SHENMUE_IS_S1
        const std::uint32_t texture = LoadTextureFromFilepath_Wrapper(member->valuestring, g_activeRenderHandle);
#else
        const std::uint32_t texture = SetupUsageForTexture(member->valuestring, 0);
#endif
        image.path = member->valuestring;
        image.ready = 0;
        g_uiViewTextures[static_cast<std::size_t>(image.textureIndex)] = texture;
        image.name = member->string;
        slot.imagesLoading = 0;
#if SHENMUE_IS_S2
        UI_RecordImageRequest(index, texture);
#endif
    }
}





void LoadUIFromMemoryIntoViewSlot(const char *memory, std::int32_t &slotIndex) {
    if (!g_uiReadStateInitialized)
        InitializeUIReadState();
    if (!memory)
        return;
    if (slotIndex == -1) {
        std::size_t freeSlot = 0;
        while (freeSlot < g_uiViewSlots.size() && g_uiViewSlots[freeSlot].inUse)
            ++freeSlot;
        if (freeSlot < g_uiViewSlots.size()) {
            g_uiViewSlots[freeSlot].inUse = 1;
            slotIndex = static_cast<std::int32_t>(freeSlot);
        }
    } else {
        g_uiViewSlots[static_cast<std::size_t>(slotIndex)].keepLoaded = 0;
        ResetUIViewSlot(slotIndex);
    }
#if SHENMUE_IS_S2
    UI_RecordImageViewSlot(slotIndex);
#endif
    if (slotIndex <= -1)
        return;
    g_currentUIViewSlot = slotIndex;
    g_parsedUIDocument = cJSON_Parse(memory);
    if (!g_parsedUIDocument)
        return;
    ReadImages(g_uiViewSlots[static_cast<std::size_t>(slotIndex)]);
    d3t::ReadVariables(g_parsedUIDocument, g_uiViewSlots[static_cast<std::size_t>(slotIndex)].variables);
    d3t::ReadSprites(g_parsedUIDocument, g_uiSpriteSheets, g_uiViewSlots[static_cast<std::size_t>(slotIndex)]);
    if (cJSON *const view = cJSON_GetObjectItem(g_parsedUIDocument, "View")) {
        ReadUIView(view);
        g_uiViewSlots[static_cast<std::size_t>(slotIndex)].inUse = 1;
    }
}

// S1's table holds GPU handles; S2 resolves generation-tagged requests. S2 completes only at the zero-request sentinel,
// not when all eighty records are occupied.
std::int32_t TryFinalizeUIViewTextures(std::int32_t slotIndex) {
    d3t::UIViewSlot<kUIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(slotIndex)];
    if (slot.imagesLoading >= 1)
        return slot.imagesLoading;
    std::int32_t ready = 0;
    std::int32_t total = 0;
    for (d3t::UIViewImage &image : slot.images) {
        const std::uint32_t request = g_uiViewTextures[static_cast<std::size_t>(image.textureIndex)];
        if (!request)
            break;
        if (image.ready != 1) {
#if SHENMUE_IS_S1
            const std::uint32_t texture = request;
#else
            std::uint32_t texture = GetShaderDataByIndex(request);
#endif
            std::uint32_t width = 0;
            std::uint32_t height = 0;
#if SHENMUE_IS_S1
            if (!GetTexture2DDimensions(texture, &width, &height) || !width || !height) {
                ++total;
                continue;
            }
#else
            if (!texture) {
                if (++total >= static_cast<std::int32_t>(slot.images.size()))
                    return slot.imagesLoading;
                continue;
            }
            if (GetTexture2DDimensions(texture, &width, &height)) {
                UI_RecordImageReady(total);
            } else {
                texture = d3t::g_defaultWhiteTexture;
                GetTexture2DDimensions(texture, &width, &height);
                UI_RecordImageFailure(total, request);
            }
#endif
            image.widthScale = static_cast<float>(width) / 1920.0f;
            image.heightScale = static_cast<float>(height) / 1080.0f;
            image.textureHandle = static_cast<std::int32_t>(texture);
            image.width = static_cast<std::uint16_t>(width);
            image.height = static_cast<std::uint16_t>(height);
            image.region = {0, 0, static_cast<std::uint16_t>(width), static_cast<std::uint16_t>(height)};
            image.ready = 1;
        }
        ++ready;
        ++total;
#if SHENMUE_IS_S2
        if (total >= static_cast<std::int32_t>(slot.images.size()))
            return slot.imagesLoading;
#endif
    }
    if (ready == total)
        slot.imagesLoading = 1;
    return slot.imagesLoading;
}



std::int32_t UI_FindViewImage(std::uint32_t slot, const char *name) {
    if (slot > 75U)
        return -1;
    const d3t::UIViewSlot<kUIViewImageCapacity> &view = g_uiViewSlots[slot];
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(kUIViewImageCapacity); ++index) {
        const char *entry = view.images[static_cast<std::size_t>(index)].name;
        if (!entry || !name)
            return index;
        const char *request = name;
        for (;;) {
            const char current = static_cast<char>(std::tolower(static_cast<unsigned char>(*entry++)));
            const char wanted = static_cast<char>(std::tolower(static_cast<unsigned char>(*request++)));
            if (current != wanted)
                break;
            if (!current)
                return index;
        }
    }
    return -1;
}

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
