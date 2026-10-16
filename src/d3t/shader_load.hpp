#pragma once

#include "d3t/shader.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

// The async load records (0x2000 x 0x50) that shader and model loads share: allocate the next record of the ring with its generation
// bumped, and look a record's 0x40-byte payload up by handle. The payload is null when the generation wrapped to zero or moved on.
std::uint32_t d3t_AllocateAsyncLoadRecord(void **data);
void *d3t_GetAsyncLoadRecord(std::uint32_t handle);
std::uint32_t LoadShaderHLSL(const char *path, std::uint32_t renderHandle, std::uint32_t shader, ShaderType type);
std::uint32_t LoadPrecompiledShader(const char *shaderPath, std::uint32_t renderHandle);
std::uint32_t GetLoadedShader(std::uint32_t loadHandle, std::size_t shaderIndex = 0);

} // namespace shenmue::d3t
