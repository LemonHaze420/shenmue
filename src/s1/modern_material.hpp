#pragma once

#include <d3t/material.hpp>
#include "s1/d3t/d3t_buffer.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

std::int32_t SetupWritingArea(ModelMaterialDescriptor *areas, const ModelMaterialDescriptor &descriptor);
bool BuildModelMaterialDescriptor(std::uint32_t handle, std::uint32_t materialIndex, ModelMaterialDescriptor *areas);
std::uint32_t ResolveModelWritingAreas(std::uint32_t handle, ModelMaterialDescriptor *areas);
using d3t::SetModelMaterialTexture;

void RefreshDirtyRenderLayerMaterialEntities(std::uint32_t renderLayer, D3TDeviceContext *context, std::uint32_t fileLockHandle);

} // namespace shenmue::s1
