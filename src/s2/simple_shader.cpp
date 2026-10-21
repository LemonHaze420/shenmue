#include "s2/simple_shader.hpp"

#include "d3t/buffer.hpp"
#include <cstdlib>

namespace shenmue::s2 {
SDFoliageParameters *g_sdFoliageParameters{};
std::uint32_t g_sdFoliageCount{}, g_sdFoliageCapacity{};
std::uint32_t g_sdFoliageBuffer{};
std::uint32_t g_sdLightBuffer{};
std::uint32_t g_sdShaderFileLock{0xffffU};

// @verified - p
SDFoliageParameters *AllocateSDFoliageParameters() {
    if (g_sdFoliageCount >= g_sdFoliageCapacity) {
        std::uint32_t capacity = 2U * g_sdFoliageCapacity;
        if (capacity < 16U)
            capacity = 16U;
        if (capacity > g_sdFoliageCapacity) {
            SDFoliageParameters *const values = static_cast<SDFoliageParameters *>(std::realloc(g_sdFoliageParameters, 20ULL * capacity));
            if (!values)
                return nullptr;
            g_sdFoliageParameters = values;
            g_sdFoliageCapacity = capacity;
        }
    }
    return &g_sdFoliageParameters[g_sdFoliageCount++];
}

// @verified - p
std::uint32_t CreateSDFoliageBuffer() {
    const d3t::BufferDescription description{
        .flags = 3, .size = static_cast<std::int32_t>(20U * g_sdFoliageCount), .count = 20, .flags3 = 0, .buffer = g_sdFoliageParameters, .name = nullptr};
    g_sdFoliageBuffer = d3t::CreateBuffer(&description);
    return g_sdFoliageBuffer;
}
} // namespace shenmue::s2
