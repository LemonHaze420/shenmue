#include "d3t/ui_internal.hpp"
#if SHENMUE_IS_S2
#include "s2/player.hpp"
#include <cstdlib>
#include <new>
#endif

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

char *g_uiReloadFileMemory{};

// @verified - p
char *GetUIReloadFileMemory() {
    return g_uiReloadFileMemory;
}

// @verified - p
std::uint32_t GetDefaultUIFileRequest() {
    return 0;
}

// @verified - p
void ReloadUIFile(void *data, void *, std::uint32_t contentHandle, void *loadedData) {
    UILoadPayload *const payload = static_cast<UILoadPayload *>(data);
    g_uiReloadFileMemory = static_cast<char *>(loadedData);
    payload->file.data->text = static_cast<char *>(loadedData);
    payload->file.data->parsed = 0;
    ReinitializeUIFileElement(payload->file.elementIndex);
    g_uiReloadFileMemory = nullptr;
    d3t::ProcessThreadStates(contentHandle, 1);
}

// @verified - p
const char *GetUIFilePath(const char *path) {
    return path;
}

// @verified - p
void CompleteUIFileLoad(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t, const char *) {
    UILoadPayload *const payload = static_cast<UILoadPayload *>(data);
    UIFileData *const file = new UIFileData;
    file->text = static_cast<char *>(loadedData);
    file->parsed = 0;
    std::atomic_ref<UIFileData *>(payload->file.data).store(file, std::memory_order_release);
    ReturnZero();
    d3t::ProcessThreadStates(contentHandle, 1);
}

// @verified - p
std::uint32_t QueueUIFileLoad(const char *path, std::int32_t elementIndex, std::uint32_t renderHandle) {
    const std::uint32_t handle = AllocateUILoadRequest();
    UILoadPayload *const payload = GetUILoadRequest(handle);
    if (payload) {
        payload->file.renderHandle = renderHandle;
        payload->file.data = nullptr;
        payload->file.elementIndex = elementIndex;
        const d3t::D3TShaderLoadTask task{.path = path,
            .renderHandle = renderHandle,
            .reserved_0c = 0,
            .thread = {.callback = CompleteUIFileLoad, .data = payload},
            .destination = nullptr,
            .destinationCapacity = 0,
            .option = 0,
            .reserved_31 = {}};
        d3t::PushShaderLoadToThreadQueue(&task);
    }
    return handle;
}

// @verified - p
std::int32_t GetNewUIFileInitFor(std::uint32_t *request, UIViewConfiguration &configuration) {
    UILoadPayload *payload = GetUILoadRequest(*request);
    if (!payload)
        return 1;
    UIFileData *file = std::atomic_ref<UIFileData *>(payload->file.data).load(std::memory_order_acquire);
    if (!file || !file->text)
        return 1;
    char *const text = file->text;
    payload = GetUILoadRequest(*request);
    file = payload ? std::atomic_ref<UIFileData *>(payload->file.data).load(std::memory_order_acquire) : nullptr;
    if (!file || !file->parsed) {
        LoadOwnedUIFromMemoryIntoViewSlot(text, configuration.uiHandleIndex);
        payload = GetUILoadRequest(*request);
        if (payload) {
            file = std::atomic_ref<UIFileData *>(payload->file.data).load(std::memory_order_acquire);
            if (file)
                file->parsed = 1;
        }
    }
    return 2;
}

// @verified - p
std::int32_t GetShouldInitUIFileFor(std::uint32_t slotIndex) {
    if (slotIndex >= d3t::kUIViewSlotCount)
        return 2;
    return TryFinalizeUIViewTextures(static_cast<std::int32_t>(slotIndex)) == 1 ? 3 : 2;
}

void TextureLoadReleaseNullsub(void *) {
}

void InitTexture2dLoaded(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *) {
    std::uint32_t *const payload = static_cast<std::uint32_t *>(data);
    payload[0] = d3t::CreateImageTextureFromMemory(loadedData, static_cast<std::size_t>(loadedSize), payload[1] != 0);
    d3t::AssignShaderCallback(payload[0], TextureLoadReleaseNullsub, nullptr);
    d3t::ProcessThreadStates(contentHandle, 0);
}

std::uint32_t AllocateUILoadRequest() {
    const std::uint16_t index = g_uiLoadRequestIndex.fetch_add(1, std::memory_order_seq_cst) & 0x1fffU;
    const std::uint16_t generation = ++g_uiLoadRequests[index].generation;
    return (static_cast<std::uint32_t>(generation) << 16U) | index;
}

// @verified - p
UILoadPayload *GetUILoadRequest(std::uint32_t handle) {
    UIAsyncLoadRequest &request = g_uiLoadRequests[static_cast<std::uint16_t>(handle)];
    return static_cast<std::uint16_t>(handle >> 16U) == request.generation ? &request.payload : nullptr;
}

std::uint32_t InitTexture2d(const char *path, std::uint32_t linearColour, std::uint32_t renderHandle) {
    const std::uint32_t handle = AllocateUILoadRequest();
    UIAsyncLoadRequest &request = g_uiLoadRequests[static_cast<std::uint16_t>(handle)];
    if (static_cast<std::uint16_t>(handle >> 16U)) {
        request.payload.image.texture = 0;
        request.payload.image.linearColour = linearColour;
        const d3t::D3TShaderLoadTask task{path, renderHandle, 0, {InitTexture2dLoaded, &request.payload.image}, nullptr, 0, 0, {}};
        d3t::PushShaderLoadToThreadQueue(&task);
    }
    return handle;
}

std::uint32_t GetShaderDataByIndexInternal(std::uint32_t requestHandle) {
    const UIAsyncLoadRequest &request = g_uiLoadRequests[static_cast<std::uint16_t>(requestHandle)];
    if (static_cast<std::uint16_t>(requestHandle >> 16U) != request.generation)
        return 0;
    std::uint32_t texture;
    std::memcpy(&texture, &request.payload, sizeof(texture));
    return texture;
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
#if SHENMUE_IS_S2
// @verified - p
void LoadOwnedUIFromMemoryIntoViewSlot(char *memory, std::int32_t &slotIndex) {
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
    std::free(memory);
    if (!g_parsedUIDocument)
        FindUIElementByIdx(slotIndex);
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
#endif

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
