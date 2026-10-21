#pragma once

#include <cstdint>

namespace shenmue::s2 {
struct SDFoliageParameters {
    float bendiness;
    float maxHeight;
    float startHeight;
    float wiggleSpeed;
    float deadzone;
};
static_assert(sizeof(SDFoliageParameters) == 20);
extern SDFoliageParameters *g_sdFoliageParameters;
extern std::uint32_t g_sdFoliageCount, g_sdFoliageCapacity;
extern std::uint32_t g_sdFoliageBuffer;
extern std::uint32_t g_sdLightBuffer;
extern std::uint32_t g_sdShaderFileLock;
SDFoliageParameters *AllocateSDFoliageParameters();
std::uint32_t CreateSDFoliageBuffer();
} // namespace shenmue::s2
