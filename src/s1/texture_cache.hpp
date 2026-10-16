#pragma once

#include <cstdint>

namespace shenmue::s1 {

extern std::int32_t g_texnTextureCacheBank[4];

std::int32_t PVR_FindOrAllocateSurface(std::int32_t textureIdLow, std::int32_t textureIdHigh);
std::int32_t PVR_FindSurface(std::int32_t textureIdLow, std::int32_t textureIdHigh);
std::int64_t PVR_FreeAllocatedSurfaceByIndex(std::int32_t surfaceIndex);

} // namespace shenmue::s1
