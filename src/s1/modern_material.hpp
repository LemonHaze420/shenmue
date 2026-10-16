#pragma once

#include <d3t/material.hpp>
#include "s1/d3t/d3t_buffer.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

void RefreshDirtyRenderLayerMaterialEntities(std::uint32_t renderLayer, D3TDeviceContext *context, std::uint32_t fileLockHandle);

} // namespace shenmue::s1
