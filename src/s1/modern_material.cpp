#include "s1/modern_material.hpp"

#include "s1/skinning_cache.hpp"
#include "s1/d3t/model.hpp"
#include "d3t/file_state.hpp"

#include <cstdlib>
#include <cstring>
namespace shenmue::s1 {

// @verified - p
std::int32_t SetupWritingArea(ModelMaterialDescriptor *areas, const ModelMaterialDescriptor &descriptor) {
    const char *const suffix = std::strstr(descriptor.name, "Writing_Area_");
    if (!suffix)
        return 0;
    const std::int32_t index = std::atoi(suffix + 13);
    if (static_cast<std::uint32_t>(index - 1) <= 3U) {
        areas[index - 1] = descriptor;
        return static_cast<std::int32_t>(descriptor.renderHandle);
    }
    return index;
}

// @verified - p
bool BuildModelMaterialDescriptor(std::uint32_t handle, std::uint32_t materialIndex, ModelMaterialDescriptor *areas) {
    D3TModelRecord *const model = d3t::ResolveModel(handle);
    if (!model)
        return false;
    const D3TModelMaterial &material = model->materials[materialIndex];
    D3TModelMaterialRuntime &runtime = model->materialRuntime[materialIndex];
    ModelMaterialDescriptor descriptor{.name = material.nameOffset < model->stringsSize ? model->strings + material.nameOffset : nullptr,
        .material = &material,
        .parameters = model->materialParameters + material.firstParameter,
        .flags = &runtime.flags,
        .textures = {},
        .textureNames = {},
        .renderHandle = handle,
        .reserved = 0};
    for (const char *&name : descriptor.textureNames)
        name = "NO TEXTURE";
    for (std::uint32_t index = 0; index < material.parameterCount; ++index) {
        const D3TModelMaterialParameter &parameter = model->materialParameters[material.firstParameter + index];
        std::uint32_t slot;
        if (!d3t::GetMaterialTextureSlot(parameter.type, slot))
            continue;
        if (parameter.texture) {
            const std::uint32_t offset = model->textures[parameter.texture - 1].pathOffset;
            descriptor.textureNames[slot] = offset < model->stringsSize ? model->strings + offset : nullptr;
        } else
            descriptor.textureNames[slot] = "UNKNOWN TEXTURE";
        descriptor.textures[slot] = runtime.textures[slot];
    }
    SetupWritingArea(areas, descriptor);
    return true;
}

// @verified - p
std::uint32_t ResolveModelWritingAreas(std::uint32_t handle, ModelMaterialDescriptor *areas) {
    const D3TModelRecord *const model = d3t::ResolveModel(handle);
    if (!model)
        return 0;
    std::uint32_t count = 0;
    for (std::uint32_t index = 0; index < model->materialCount; ++index)
        count += BuildModelMaterialDescriptor(handle, index, areas);
    return count;
}
void RefreshDirtyRenderLayerMaterialEntities(std::uint32_t renderLayer, D3TDeviceContext *context, std::uint32_t fileLockHandle) {
    CachedSkinningBufferSlot *const layer = ResolveCachedSkinningBuffer(renderLayer);
    if (!layer)
        return;
    for (std::uint32_t nodeIndex = 0; nodeIndex < layer->entryCount; ++nodeIndex) {
        const D3TModelNode &node = reinterpret_cast<const D3TModelNode *>(layer->entries)[nodeIndex];
        if ((node.flags & 1U) || !node.drawGroup)
            continue;
        D3TModelRecord *const model = d3t::ResolveModel(layer->renderHandles[nodeIndex]);
        if (!model)
            continue;
        for (std::uint32_t materialIndex = 0; materialIndex < model->materialCount; ++materialIndex) {
            D3TModelMaterialRuntime &runtime = model->materialRuntime[materialIndex];
            if (!(runtime.flags & 2U))
                continue;
            const D3TModelMaterial &material = model->materials[materialIndex];
            d3t::LockFileHandle(fileLockHandle);
            d3t::GetOrCreateSceneEntity(material, model->materialParameters + material.firstParameter, runtime, context);
            d3t::UnlockFileHandle(fileLockHandle);
            runtime.flags ^= 2U;
        }
    }
}

} // namespace shenmue::s1
