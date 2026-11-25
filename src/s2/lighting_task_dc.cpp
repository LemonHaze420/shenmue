#include "s2/lighting_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/am2_vector.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/camera_dc.hpp"
#include "s2/hlib.hpp"
#include <cmath>

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" void memcpy4(void *destination, const void *source, std::size_t bytes);
#endif

namespace shenmue {
namespace s2 {

std::int32_t g_previousActiveLight, g_lightRankingEnabled = 0, g_rankedLightLimit = 12;
float g_lightSelectionRadius = 150.0f;
Vector3 g_lightRootPosition, g_lightPositionScratch = {};
std::int16_t g_characterLightCandidates[256] = {};

// @verified - d
std::int32_t GetPreviousActiveLight() {
    return g_previousActiveLight;
}

// @verified - d
void GetLightRootPosition(Vector3 *position) {
    *position = g_lightRootPosition;
}

// @verified - d
Vector3 *GetRuntimeLightWorldPosition(const LightRuntimeEntry *light) {
    if ((static_cast<std::int8_t>(light->indexes[1]) & ~8U) == 3) {
        g_lightPositionScratch.x = light->vector20.x + g_lightRootPosition.x;
        g_lightPositionScratch.y = light->vector20.y + g_lightRootPosition.y;
        g_lightPositionScratch.z = light->vector20.z + g_lightRootPosition.z;
    } else {
        g_lightPositionScratch = light->vector20;
    }
    return &g_lightPositionScratch;
}

void SelectLightForFrame(std::uint32_t index, LightRuntimeEntry *light) {
    if (index > 15)
        return;
    void *const selected = &g_selectedLights[index];
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    memcpy4(selected, light, sizeof(LightRuntimeEntry));
#else
    std::memcpy(selected, light, sizeof(LightRuntimeEntry));
#endif
    dc::sdk::SetLightType(index, static_cast<std::int8_t>(light->indexes[1]) & ~8U);
    if (light->vectors[0].y < 0.00001f)
        light->vectors[0].y = 0.0f;
    if (light->vectors[0].z < 0.00001f)
        light->vectors[0].z = 0.0f;
    if (light->vectors[1].x < 0.00001f)
        light->vectors[1].x = 0.0f;
    if ((light->vectors[1].y < 0.0f ? -light->vectors[1].y : light->vectors[1].y) < 0.0001f)
        light->vectors[1].y = 0.0f;
    Vector4 colour;
    colour.y = light->vectors[0].y * light->vectors[1].y;
    colour.z = light->vectors[0].z * light->vectors[1].y;
    colour.w = light->vectors[1].x * light->vectors[1].y;
    dc::sdk::SetLightModelColour(index, &colour);
    dc::sdk::SetLightRange(index, light->vectors[1].z);
    float attenuation = light->vectors[1].z * light->value1c;
    attenuation *= attenuation;
    attenuation *= attenuation;
    attenuation *= attenuation;
    dc::sdk::SetLightAttenuation(index, attenuation, 0.0f, 0.0f);
    if ((static_cast<std::int8_t>(light->indexes[1]) & ~8) == 3) {
        dc::sdk::SetLightPosition(
            index, light->vector20.x + g_lightRootPosition.x, light->vector20.y + g_lightRootPosition.y, light->vector20.z + g_lightRootPosition.z);
    } else {
        dc::sdk::SetLightPosition(index, light->vector20.x, light->vector20.y, light->vector20.z);
    }
    dc::sdk::SetLightDirection(index, -light->values2c[0], -light->values2c[1], -light->value34);
    dc::sdk::SetLightAngle(index, light->unknown02);
    dc::sdk::SetLightEnabled(index, static_cast<std::int8_t>(light->indexes[0]));
}

// @verified - d
void LightTask_Callback(void *) {
    g_previousActiveLight = g_currentActiveLight;
    dc::sdk::SetMT7ReflectedDirection(0);
    if (!g_lightRankingEnabled || GetCurrentActiveLight() != 3) {
        std::int32_t count = 0;
        for (std::int32_t index = 0; index < 16; ++index) {
            SelectLightForFrame(index, &g_lightRuntimeEntries[index]);
            if (g_lightRuntimeEntries[index].indexes[0])
                g_characterLightCandidates[count++] = static_cast<std::int16_t>(index);
        }
        g_characterLightCandidates[count] = -1;
    } else {
        Vector3 transformed;
        float distances[256];
        Vector3 camera;
        camCtrlGetNextCamPos(&camera);
        const float distanceLimit = g_lightSelectionRadius * g_lightSelectionRadius;
        std::int32_t index = 0, slot, count = 0;
        LightRuntimeEntry *light = g_lightRuntimeEntries;
        for (; index < 256; ++index, ++light) {
            if (index == 0 || (index >= 13 && index <= 15))
                continue;
            if (static_cast<std::int8_t>(light->indexes[0]) != 1)
                continue;
            float distance = 0.0f;
            if ((static_cast<std::int8_t>(light->indexes[1]) & ~8) != 2) {
                Matrix4x4::TransformPoint(*GetRuntimeLightWorldPosition(light), transformed);
                if (transformed.z > light->vectors[1].z * 1.5f)
                    distance = 1000000.0f;
                else
                    distance = am2Vector3DistanceSquared(&camera, GetRuntimeLightWorldPosition(light));
            }
            if (distance > distanceLimit)
                continue;
            for (slot = 0; slot < count; ++slot) {
                if (distances[slot] > distance) {
                    for (std::int32_t move = count; move > slot; --move) {
                        g_characterLightCandidates[move] = g_characterLightCandidates[move - 1];
                        distances[move] = distances[move - 1];
                    }
                    break;
                }
            }
            g_characterLightCandidates[slot] = static_cast<std::int16_t>(index);
            distances[slot] = distance;
            ++count;
        }
        index = 0;
        slot = count;
        if (slot > g_rankedLightLimit)
            slot = g_rankedLightLimit;
        SelectLightForFrame(0, &g_lightRuntimeEntries[0]);
        for (; index < slot; ++index)
            SelectLightForFrame(index + 1, &g_lightRuntimeEntries[g_characterLightCandidates[index]]);
        for (; index <= 11; ++index)
            SelectLightForFrame(index + 1, &disabledLight);
        for (; index <= 13; ++index) {
            SelectLightForFrame(index + 1, &g_lightRuntimeEntries[index + 1]);
            if (g_lightRuntimeEntries[index + 1].indexes[0])
                g_characterLightCandidates[count++] = static_cast<std::int16_t>(index + 1);
        }
        SelectLightForFrame(index + 1, &disabledLight);
        g_characterLightCandidates[count] = -1;
    }
    PublishGlobalLighting();
}

// @verified - d
void LightTask_Enqueue() {
    HLib::EnqueueTaskWithoutParameter(LightTask_Callback, 13, 2, CHUNK_LGHT);
    g_previousActiveLight = g_currentActiveLight;
}

} // namespace s2
} // namespace shenmue

#endif
