#include "s1/modern_material.hpp"

#include "s1/skinning_cache.hpp"
#include "s1/d3t/model.hpp"
#include "d3t/file_state.hpp"

namespace shenmue::s1 {

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
