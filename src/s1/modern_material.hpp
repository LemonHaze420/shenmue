#pragma once

#include <d3t/material.hpp>
#include "s1/d3t/d3t_buffer.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct ModelMaterialDescriptor {
    const char *name;
    const D3TModelMaterial *material;
    const D3TModelMaterialParameter *parameters;
    std::uint32_t *flags;
    std::uint32_t textures[16];
    const char *textureNames[16];
    std::uint32_t renderHandle;
    std::uint32_t reserved;
};
static_assert(sizeof(ModelMaterialDescriptor) == 232);
static_assert(offsetof(ModelMaterialDescriptor, textures) == 32);
static_assert(offsetof(ModelMaterialDescriptor, textureNames) == 96);
static_assert(offsetof(ModelMaterialDescriptor, renderHandle) == 224);
std::int32_t SetupWritingArea(ModelMaterialDescriptor *areas, const ModelMaterialDescriptor &descriptor);
bool BuildModelMaterialDescriptor(std::uint32_t handle, std::uint32_t materialIndex, ModelMaterialDescriptor *areas);
std::uint32_t ResolveModelWritingAreas(std::uint32_t handle, ModelMaterialDescriptor *areas);
void RefreshDirtyRenderLayerMaterialEntities(std::uint32_t renderLayer, D3TDeviceContext *context, std::uint32_t fileLockHandle);

} // namespace shenmue::s1
