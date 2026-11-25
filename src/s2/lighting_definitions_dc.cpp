#include "s2/lighting_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_info.hpp"
#include "s2/motion_node_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

// @verified - d
std::int32_t GetLightEntryIndex(std::uint32_t index) {
    std::int32_t result;
    if (index > 255)
        result = 3;
    else
        result = static_cast<std::int8_t>(g_lightRuntimeEntries[index].indexes[0]);
    return result;
}

// @verified - d
void SetRankedLightLimit(std::uint32_t count) {
    if (count < 13)
        g_rankedLightLimit = count;
}

// @verified - d
void SetLightRootPosition(const Vector3 &position) {
    g_lightRootPosition = position;
}

// @verified - d
void GetLightRuntimePosition(std::uint32_t index, Vector3 *position) {
    if (index < 256) {
        if ((static_cast<std::int8_t>(g_lightRuntimeEntries[index].indexes[1]) & ~8) == 3) {
            position->x = g_lightRuntimeEntries[index].vector20.x + g_lightRootPosition.x;
            position->y = g_lightRuntimeEntries[index].vector20.y + g_lightRootPosition.y;
            position->z = g_lightRuntimeEntries[index].vector20.z + g_lightRootPosition.z;
        } else {
            const LightRuntimeEntry *entries = g_lightRuntimeEntries;
            const LightRuntimeEntry *const light = entries + index;
            *position = light->vector20;
        }
    }
}

// @verified - d
void CopyLightRuntimeEntry(std::uint32_t destination, std::uint32_t source) {
    if (destination < 256 && source < 256) {
        std::uint8_t *entries = reinterpret_cast<std::uint8_t *>(g_lightRuntimeEntries);
        const std::uint32_t to = destination * sizeof(LightRuntimeEntry);
        const std::uint32_t from = source * sizeof(LightRuntimeEntry);
        *reinterpret_cast<LightRuntimeEntry *>(entries + to) = *reinterpret_cast<LightRuntimeEntry *>(entries + from);
    }
}

// @verified - d
void TranslateLightDefinition(std::uint32_t identifier, const Vector3 &translation) {
    if (!FindNativeChunk("LGHT", GetCurrentMapInfo()))
        return;
    std::int32_t slot;
    if (identifier < 24) {
        slot = identifier;
    } else {
        for (slot = 0; slot < 24; ++slot) {
            if (g_lightDefinitions[slot].identifier == identifier)
                break;
        }
    }
    if (slot >= 24 || !g_lightDefinitions[slot].block || *reinterpret_cast<const std::uint32_t *>(g_lightDefinitions[slot].block) != CHUNK_LGHT)
        return;
    for (std::int32_t index = 0; index < 256; ++index) {
        if (GetLightRuntimeKind(index) == 3) {
            Vector3 position;
            GetLightRuntimePosition(index, &position);
            position.x += translation.x;
            position.y += translation.y;
            position.z += translation.z;
            SetLightRuntimePosition(index, position);
        }
    }
}

// @verified - d
void EnableLightEntry(std::uint32_t index) {
    SetLightEntryIndex(index, 1);
}

// @verified - d
void DisableLightEntry(std::uint32_t index) {
    SetLightEntryIndex(index, 0);
}

// @verified - d
void SetLightEntryIndex(std::uint32_t index, std::uint32_t value) {
    if (index < 256 && value < 2)
        g_lightRuntimeEntries[index].indexes[0] = static_cast<std::uint8_t>(value);
}

// @verified - d
void SetLightGeometryEnabled(std::uint32_t index, std::int32_t enabled) {
    if (index < 256) {
        if (enabled)
            g_lightRuntimeEntries[index].indexes[1] |= 8;
        else
            g_lightRuntimeEntries[index].indexes[1] &= 0xf7;
    }
}

std::uint32_t GetLightGeometryEnabled(std::uint32_t index);

// @verified - d
void SetLightFlagForIndex(std::uint32_t index, std::int32_t value) {
    if (index < 256 && static_cast<std::uint32_t>(value) < 5) {
        const std::uint32_t geometry = GetLightGeometryEnabled(index);
        g_lightRuntimeEntries[index].indexes[1] = static_cast<std::uint8_t>(value);
        SetLightGeometryEnabled(index, geometry);
    }
}

// @verified - d
void SetLightRuntimeType(std::uint32_t index, std::int32_t value) {
    if (index < 256)
        g_lightRuntimeEntries[index].indexes[1] = value;
}

// @verified - d
void SetLightRange(std::uint32_t index, float range) {
    if (index < 256) {
        if (range < 0.01f)
            range = 0.01f;
        g_lightRuntimeEntries[index].vectors[1].z = range;
    }
}

// @verified - d
void SetLightRuntimeFalloff(std::uint32_t index, float falloff);

// @verified - d
void SetLightFalloff(std::uint32_t index, const float *falloff) {
    SetLightRuntimeFalloff(index, *falloff);
}

// @verified - d
void SetLightRuntimePosition(std::uint32_t index, const Vector3 &position) {
    if (index < 256) {
        if ((static_cast<std::int8_t>(g_lightRuntimeEntries[index].indexes[1]) & ~8) == 3) {
            g_lightRuntimeEntries[index].vector20.x = position.x - g_lightRootPosition.x;
            g_lightRuntimeEntries[index].vector20.y = position.y - g_lightRootPosition.y;
            g_lightRuntimeEntries[index].vector20.z = position.z - g_lightRootPosition.z;
        } else {
            LightRuntimeEntry *entries = g_lightRuntimeEntries;
            LightRuntimeEntry *const light = entries + index;
            light->vector20 = position;
        }
    }
}

// @verified - d
void SetLightConeAngle(std::uint32_t index, std::int32_t angle) {
    if (index < 256)
        g_lightRuntimeEntries[index].unknown02 = angle;
}

// @verified - d
void ResetLightEntry(std::uint32_t index) {
    LightRuntimeEntry *light = &disabledLight;
    if (index == 0 && g_currentActiveLight == 1) {
        EnableLightEntry(0);
        SetLightRuntimeType(0, 2);
    } else {
        DisableLightEntry(index);
        SetLightRuntimeType(index, 0);
    }
    SetLightRuntimeColour(index, *reinterpret_cast<const Vector4 *>(light->vectors));
    SetLightSceneIntensity(index, 1.0f);
    SetLightRange(index, 2.0f);
    SetLightRuntimeFalloff(index, 0.6f);
    SetLightRuntimePosition(index, *GetRuntimeLightWorldPosition(light));
    SetLightSunDirection(index, *reinterpret_cast<const Vector3 *>(light->values2c));
    SetLightConeAngle(index, 0x071c);
}

// @verified - d
void SetDefaultGlobalLighting() {
    Vector4 colour;
    colour.x = 0.3f;
    colour.y = 0.3f;
    colour.z = 0.3f;
    colour.w = 0.3f;
    UpdateGlobalLighting(colour);
}

// @verified - d
void ReinitializeLights(std::int32_t mode) {
    g_currentActiveLight = 0;
    SetDefaultGlobalLighting();
    for (std::uint32_t index = 0; index < 256; ++index)
        ResetLightEntry(index);
    g_currentActiveLight = mode;
    if (mode == 1) {
        ResetLightEntry(0);
        dc::sdk::SetLightProcessingEnabled(0);
    } else {
        dc::sdk::SetLightProcessingEnabled(1);
    }
    g_lightRootPosition.x = g_lightRootPosition.y = g_lightRootPosition.z = 0.0f;
}

// @verified - d
void InitializeDefaultDirectionalLight(std::uint32_t index, const Vector3 *direction) {
    ResetLightEntry(index);
    SetLightFlagForIndex(index, 2);
    SetLightSunDirection(index, *direction);
    EnableLightEntry(index);
}

// @verified - d
void ApplyDefinitionLight(std::uint32_t index, LightRuntimeEntry *light) {
    SetLightRuntimeType(index, static_cast<std::int8_t>(light->indexes[1]));
    SetLightRuntimeColour(index, *reinterpret_cast<const Vector4 *>(light->vectors));
    if (light->vectors[1].y == 0.0f)
        light->vectors[1].y = 1.0f;
    SetLightSceneIntensity(index, light->vectors[1].y);
    SetLightRange(index, light->vectors[1].z);
    SetLightRuntimeFalloff(index, light->value1c);
    SetLightRuntimePosition(index, *GetRuntimeLightWorldPosition(light));
    SetLightSunDirection(index, *reinterpret_cast<const Vector3 *>(light->values2c));
    SetLightConeAngle(index, light->unknown02);
    SetLightEntryIndex(index, static_cast<std::int8_t>(light->indexes[0]));
}

// @verified - d
void LGHT_Parser_Internal(std::uint32_t identifier) {
    if (!FindNativeChunk("LGHT", GetCurrentMapInfo()))
        return;
    std::int32_t slot;
    std::int32_t index;
    if (identifier < 24) {
        slot = identifier;
    } else {
        for (slot = 0; slot < 24; ++slot) {
            if (g_lightDefinitions[slot].identifier == identifier)
                break;
        }
    }
    if (slot >= 24)
        return;
    const std::byte *const block = g_lightDefinitions[slot].block;
    if (!block)
        return;
    if (*reinterpret_cast<const std::uint32_t *>(block) == CHUNK_LGHT) {
        const PackedLightDefinitionHeader *const header = reinterpret_cast<const PackedLightDefinitionHeader *>(block);
        ReinitializeLights(header->mode);
        if (header->mode == 3)
            g_lightRankingEnabled = 1;
        else
            g_lightRankingEnabled = 0;
        Vector4 colour;
        colour.x = motion_detail::F16ToF32(header->colour[0]);
        colour.y = motion_detail::F16ToF32(header->colour[1]);
        colour.z = motion_detail::F16ToF32(header->colour[2]);
        colour.w = motion_detail::F16ToF32(header->colour[3]);
        UpdateGlobalLighting(colour);
        std::uint32_t lightStorage[sizeof(LightRuntimeEntry) / sizeof(std::uint32_t)];
        const PackedLightDefinitionRecord *record = reinterpret_cast<const PackedLightDefinitionRecord *>(block + sizeof(PackedLightDefinitionHeader));
        for (index = 0; index < header->lightCount; ++index, ++record) {
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->indexes[0] = static_cast<std::uint8_t>(record->enabled);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->indexes[1] = static_cast<std::uint8_t>(record->flags);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->vectors[0].x = 1.0f;
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->vectors[0].y = motion_detail::F16ToF32(record->parameters[0]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->vectors[0].z = motion_detail::F16ToF32(record->parameters[1]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->vectors[1].x = motion_detail::F16ToF32(record->parameters[2]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->vectors[1].y = motion_detail::F16ToF32(record->parameters[3]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->vectors[1].z = motion_detail::F16ToF32(record->parameters[4]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->value1c = motion_detail::F16ToF32(record->parameters[5]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->vector20.x = motion_detail::F16ToF32(record->parameters[6]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->vector20.y = motion_detail::F16ToF32(record->parameters[7]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->vector20.z = motion_detail::F16ToF32(record->parameters[8]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->values2c[0] = motion_detail::F16ToF32(record->parameters[9]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->values2c[1] = motion_detail::F16ToF32(record->parameters[10]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->value34 = motion_detail::F16ToF32(record->parameters[11]);
            reinterpret_cast<LightRuntimeEntry *>(lightStorage)->unknown02 = record->coneAngle;
            ApplyDefinitionLight(record->index, reinterpret_cast<LightRuntimeEntry *>(lightStorage));
        }
    } else {
        const LegacyLightDefinitionHeader *const header = reinterpret_cast<const LegacyLightDefinitionHeader *>(block);
        ReinitializeLights(header->mode);
        UpdateGlobalLighting(header->colour);
        if (header->mode == 1) {
            ApplyDefinitionLight(0, const_cast<LightRuntimeEntry *>(&header->primary));
        } else {
            ApplyDefinitionLight(16, const_cast<LightRuntimeEntry *>(&header->primary));
            for (index = 1; index < 16; ++index)
                ApplyDefinitionLight(index, const_cast<LightRuntimeEntry *>(&header->primary) + index);
        }
    }
}

// @verified - d
void InitializeMapLighting() {
    std::int32_t index;
    for (index = 0; index < 24; ++index) {
        g_lightDefinitions[index].block = 0;
        g_lightDefinitions[index].identifier = UINT32_MAX;
    }
    const MapInfoChunk *const chunk = FindNativeChunk("LGHT", GetCurrentMapInfo());
    if (chunk) {
        const std::byte *const end = reinterpret_cast<const std::byte *>(chunk) + chunk->nextByteOffset;
        const std::byte *cursor = reinterpret_cast<const std::byte *>(chunk + 1);
        index = 0;
        if (cursor < end) {
            do {
                if (!*reinterpret_cast<const std::uint32_t *>(cursor))
                    break;
                if (*reinterpret_cast<const std::uint32_t *>(cursor) == CHUNK_LGHT) {
                    g_lightDefinitions[index].identifier = index;
                    g_lightDefinitions[index].block = cursor;
                } else {
                    g_lightDefinitions[index].identifier = *reinterpret_cast<const std::uint32_t *>(cursor);
                    cursor += 8;
                    g_lightDefinitions[index].block = cursor;
                }
                cursor += reinterpret_cast<const MapInfoChunk *>(cursor)->nextByteOffset;
                if (++index > 23)
                    break;
            } while (cursor < end);
        }
        LGHT_Parser_Internal(0);
    } else {
        ReinitializeLights(1);
        Vector3 direction;
        direction.x = 0.0f;
        direction.y = -0.7071070075035095f;
        direction.z = -0.7071070075035095f;
        InitializeDefaultDirectionalLight(0, &direction);
    }
    g_lightRankingEnabled = 0;
    g_lightSelectionRadius = 150.0f;
    g_rankedLightLimit = 12;
    LightTask_Enqueue();
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_lightPriorityCount;
std::int16_t g_lightPriority[5];

void AddLightPriority(std::int32_t light) {
    if (g_lightPriorityCount <= 4)
        g_lightPriority[g_lightPriorityCount++] = static_cast<std::int16_t>(light);
}

#endif

} // namespace s2
} // namespace shenmue

#endif
