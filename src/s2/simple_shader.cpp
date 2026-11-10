#include "s2/simple_shader.hpp"

#include "d3t/buffer.hpp"
#include "d3t/content.hpp"
#include "d3t/ui_runtime.hpp"

#include <cJSON.h>
#include <cstdlib>

namespace shenmue::s2 {
SDFoliageParameters *g_sdFoliageParameters{};
std::uint32_t g_sdFoliageCount{}, g_sdFoliageCapacity{};
std::uint32_t g_sdFoliageBuffer{};
std::uint32_t g_sdLightBuffer{};
std::uint32_t g_sdShaderFileLock{0xffffU};
std::uint32_t g_wetnessMaskTexture{};

namespace {

void FoliageAnimJson_Entry(void *, std::uint32_t handle, void *bytes, std::uint64_t, const char *) {
    g_sdFoliageCount = 0;
    if (cJSON *const root = cJSON_Parse(static_cast<const char *>(bytes))) {
        cJSON *const data = cJSON_GetObjectItem(root, "data");
        const int count = data ? cJSON_GetArraySize(data) : 0;
        for (int index = 0; index < count; ++index) {
            const cJSON *const entry = cJSON_GetArrayItem(data, index);
            SDFoliageParameters *const parameters = AllocateSDFoliageParameters();
            if (!entry || !parameters)
                continue;
            if (const cJSON *const value = cJSON_GetObjectItem(const_cast<cJSON *>(entry), "bendiness"))
                parameters->bendiness = static_cast<float>(value->valuedouble);
            if (const cJSON *const value = cJSON_GetObjectItem(const_cast<cJSON *>(entry), "maxHeight"))
                parameters->maxHeight = static_cast<float>(value->valuedouble);
            if (const cJSON *const value = cJSON_GetObjectItem(const_cast<cJSON *>(entry), "startHeight"))
                parameters->startHeight = static_cast<float>(value->valuedouble);
            if (const cJSON *const value = cJSON_GetObjectItem(const_cast<cJSON *>(entry), "wiggleSpeed"))
                parameters->wiggleSpeed = static_cast<float>(value->valuedouble);
            if (const cJSON *const value = cJSON_GetObjectItem(const_cast<cJSON *>(entry), "deadzone"))
                parameters->deadzone = static_cast<float>(value->valuedouble);
        }
        cJSON_Delete(root);
    }
    d3t::ProcessThreadStates(handle, 0);
}

} // namespace

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

void FoliageAnim(std::uint32_t renderHandle) {
    const d3t::D3TShaderLoadTask request{.path = "/foliageanim.json", .renderHandle = renderHandle, .thread = {FoliageAnimJson_Entry, nullptr}};
    d3t::PushShaderLoadToThreadQueue(&request);
    g_wetnessMaskTexture = InitTexture2d("/fx/wetness_mask01.png?usage=0", 1, renderHandle);
}
} // namespace shenmue::s2
