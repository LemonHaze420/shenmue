#include "s1/ui_internal.hpp"

namespace shenmue::s1::ui_runtime_detail {


UIObjectDefinition *FindUIObject(const char *id) {
    for (UIDefinition &definition : g_parsedUIDefinitions) {
        for (UIObjectDefinition &object : definition.objects) {
            if (object.id == id)
                return &object;
        }
    }
    return nullptr;
}



void UI_BindViewImages(UIDefinition &definition) {
    if (definition.viewSlot != -1) {
        const d3t::UIViewSlot<d3t::kS1UIViewImageCapacity> &slot = g_uiViewSlots[static_cast<std::size_t>(definition.viewSlot)];
        for (UIImageDefinition &image : definition.images) {
            for (const d3t::UIViewImage &viewImage : slot.images) {
                if (viewImage.name && image.name == viewImage.name) {
                    image.textureHandle = g_uiViewTextures[static_cast<std::size_t>(viewImage.textureIndex)];
                    break;
                }
            }
        }
    }
    definition.requestTextures(g_activeRenderHandle);
}



void UI_ResolveLabelScaleVars(UIDefinition &definition) {
    for (UIObjectDefinition &object : definition.objects) {
        object.scaleVar = -1;
        if (object.kind != UIObjectKind::Label || object.scaleVarName.empty() || definition.viewSlot == -1)
            continue;
        const std::array<d3t::UIViewVariable, d3t::kUIViewVariableCount> &variables = g_uiViewSlots[static_cast<std::size_t>(definition.viewSlot)].variables;
        for (std::size_t index = 0; index < variables.size() && variables[index].used; ++index) {
            if (object.scaleVarName == variables[index].name) {
                object.scaleVar = static_cast<std::int32_t>(index);
                break;
            }
        }
    }
}

void UIFileLoaded(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *) {
    auto *const element = static_cast<UIBaseRuntime *>(data);
    if (element && element->index >= 0 && element->index < 80 && loadedData && loadedSize != 0) {
        const auto *const first = static_cast<const std::byte *>(loadedData);
        g_uiDefinitions[static_cast<std::size_t>(element->index)].assign(first, first + loadedSize);
        UIDefinition &definition = g_parsedUIDefinitions[static_cast<std::size_t>(element->index)];
        definition.parse(first, static_cast<std::size_t>(loadedSize));
        g_uiDefinitionReady[static_cast<std::size_t>(element->index)].store(true, std::memory_order_release);
    }
    ProcessThreadStates(contentHandle, 0);
}

std::uint32_t OpenUIFile(const char *path, UIBaseRuntime &element) {
    const D3TShaderLoadTask task{
        path,
        g_activeRenderHandle,
        0,
        {UIFileLoaded, &element},
        nullptr,
        0,
        0,
        {},
    };
    return static_cast<std::uint32_t>(PushShaderLoadToThreadQueue(&task));
}

void UILegacyFileLoaded(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *) {
    UILegacyFile *const file = static_cast<UILegacyFile *>(data);
    if (file && loadedData && loadedSize != 0) {
        const char *const first = static_cast<const char *>(loadedData);
        file->data.assign(first, first + loadedSize);
        file->data.push_back('\0');
        file->loaded.store(true, std::memory_order_release);
    }
    ProcessThreadStates(contentHandle, 0);
}


std::uint32_t OpenLegacyUIFile(const char *path, std::uint32_t index) {
    UILegacyFile &file = g_uiLegacyFiles[index];
    file.loaded.store(false, std::memory_order_release);
    file.consumed = false;
    file.data.clear();
    const D3TShaderLoadTask task{
        path,
        g_activeRenderHandle,
        0,
        {UILegacyFileLoaded, &file},
        nullptr,
        0,
        0,
        {},
    };
    return static_cast<std::uint32_t>(PushShaderLoadToThreadQueue(&task));
}



std::int32_t TryLoadUIFromFileHandle(std::uint32_t index, std::int32_t &slot) {
    UILegacyFile &file = g_uiLegacyFiles[index];
    if (!file.loaded.load(std::memory_order_acquire) || file.data.empty())
        return 1;
    if (!file.consumed) {
        LoadUIFromMemoryIntoViewSlot(file.data.data(), slot);
        file.consumed = true;
    }
    return 2;
}

std::uint32_t LoadPresentationDefinition(std::int32_t index, const char *path) {
    if (index < 0 || index >= static_cast<std::int32_t>(g_uiElements.size()) || !path) {
        return 0;
    }
    UIBaseRuntime &element = g_uiElements[static_cast<std::size_t>(index)];
    element.index = index;
    if (!element.setup) {
        element.setup = 1;
        element.someFlag3 = 1;
        std::snprintf(element.uiConfigFilepath, sizeof(element.uiConfigFilepath), "%s", path);
        const std::uint32_t handle = OpenUIFile(path, element);
        element.config.screenWidth = static_cast<std::int16_t>(handle);
        element.config.screenHeight = static_cast<std::int16_t>(handle >> 16U);
        element.config.init = 1;
    }
    return static_cast<std::uint32_t>(element.setup);
}

} // namespace shenmue::s1::ui_runtime_detail

namespace shenmue::s1 {

std::int16_t AllocatePVRSurface(const char *, std::int32_t width, std::uint16_t foreground, std::uint16_t background, std::int32_t, bool centered) {
    if (width < 12)
        return -1;
    ShenmueSurfaceBuffer *const surface = SurfaceAllocation(static_cast<std::uint32_t>(width), 16);
    if (!surface)
        return -1;
    surface->formatEncoding += 0x10000U;
    std::uint8_t *const pixels = static_cast<std::uint8_t *>(surface->ddsBuffer) + 0x80;
    const std::int32_t glyphOffset = centered ? 4 * ((width - 12) / 2) : 0;
    RasterizeFontGlyph(g_pFont, 0x012e, pixels + glyphOffset, 4 * width, foreground, background);
    CreatePVRTextureBufferForSurface(surface->primarySurface, static_cast<std::uint32_t>(width), 16, 0x8089, pixels);
    const std::int16_t primary = surface->primarySurface;
    if (surface->secondarySurface != -1)
        PVR_FreeAllocatedSurfaceByIndex(surface->secondarySurface);
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(surface->ddsBuffer), 0);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(surface), 0);
    return primary;
}

} // namespace shenmue::s1
