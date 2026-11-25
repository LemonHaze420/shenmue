#include "s2/lighting_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/mt7.hpp"
#include "s2/hlib.hpp"
#include <cstring>
#include <cmath>

namespace shenmue {
namespace dc {
namespace sdk {
void TransformLightRecords();
}
} // namespace dc
namespace s2 {
namespace motion_detail {
std::int16_t F32ToF16(float value);
}

void EnableLightEntry(std::uint32_t index);
std::int32_t GetSelectedLightEnabled(std::uint32_t index);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_characterShadowSavedLightMode;
Vector4 g_characterShadowSavedAmbient;
float g_lightChunkVersion = 1.1f;

// Vector3 interpolator; task data is 44 bytes.
void ZeroWordRange(std::uint32_t *word, std::uint32_t *last);

struct LightInterpolator {
    Vector3 current;
    Vector3 target;
    Vector3 step;
    std::int32_t frame;
    std::int32_t frameCount;
};
typedef char sizeofLightInterpolatorCheck[sizeof(LightInterpolator) == 0x2c ? 1 : -1];
#endif

// @verified - d
std::uint32_t GetGeometryLightColour(Vector4 *colour) {
    GetGlobalLighting(colour);
    UpdateGlobalLighting(*colour);
    colour->y *= 1.4f;
    colour->z *= 1.4f;
    colour->w *= 1.4f;
    return 0;
}

#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM || SHENMUE_TARGET_S2_DC_JP_1001
// @verified - d
std::int32_t FindUnusedSelectedLight() {
    switch (static_cast<std::uint32_t>(GetCurrentActiveLight())) {
    case 1:
        if (!GetSelectedLightEnabled(0))
            return 0;
        break;
    case 2:
    case 3:
        for (std::int32_t index = 0; index < 16; ++index) {
            if (!GetSelectedLightEnabled(index))
                return index;
        }
        break;
    }
    return 17;
}
#endif

// @verified - d
void SetSelectedLightEnabled(std::uint32_t index, std::uint32_t enabled) {
    if (index < 16 && enabled < 2) {
        g_selectedLights[index].indexes[0] = static_cast<std::uint8_t>(enabled);
        dc::sdk::SetLightEnabled(index, static_cast<std::int8_t>(enabled));
    }
}

std::int32_t GetSelectedLightEnabled(std::uint32_t index) {
    if (index < 16)
        return static_cast<std::int8_t>(g_selectedLights[index].indexes[0]);
    else
        return 3;
}

std::uint32_t GetLightGeometryEnabled(std::uint32_t index) {
    if (index < 256)
        return g_lightRuntimeEntries[index].indexes[1] & 8U;
    else
        return 0;
}

// @verified - d
void EnableSelectedLight(std::uint32_t index) {
    SetSelectedLightEnabled(index, 1);
}

// @verified - d
void DisableSelectedLight(std::uint32_t index) {
    SetSelectedLightEnabled(index, 0);
}

// @verified - d
void SetLightKind(std::uint32_t index, std::int32_t kind) {
    if (index < 16 && static_cast<std::uint32_t>(kind) < 5) {
        g_selectedLights[index].indexes[1] = static_cast<std::uint8_t>(kind);
        dc::sdk::SetLightType(index, static_cast<std::uint32_t>(kind));
    }
}

// @verified - d
std::uint32_t GetSelectedLightKind(std::uint32_t index) {
    if (index < 16)
        return static_cast<std::int8_t>(g_selectedLights[index].indexes[1]) & ~8;
    else
        return 6U;
}

std::int32_t GetLightRuntimeType(std::uint32_t index) {
    if (index < 256)
        return static_cast<std::int8_t>(g_lightRuntimeEntries[index].indexes[1]);
    else
        return 6;
}

// @verified - d
void shenSetLightColour(std::uint32_t index, const Vector4 &colour) {
    if (index < 16) {
        LightRuntimeEntry *lights = g_selectedLights;
        LightRuntimeEntry *const light = &lights[index];
        *reinterpret_cast<Vector4 *>(&light->vectors[0]) = colour;
    }
}

void GetLightRuntimeColour(std::uint32_t index, Vector4 *colour) {
    if (index < 256) {
        const LightRuntimeEntry *lights = g_lightRuntimeEntries;
        const LightRuntimeEntry *const light = &lights[index];
        *colour = *reinterpret_cast<const Vector4 *>(&light->vectors[0]);
    }
}

// @verified - d
void SetSelectedLightRange(std::uint32_t index, float range) {
    if (index < 16) {
        if (range < 0.01f)
            range = 0.01f;
        g_selectedLights[index].vectors[1].z = range;
        dc::sdk::SetLightRange(index, range);
        float attenuation = range * g_selectedLights[index].value1c;
        attenuation *= attenuation;
        attenuation *= attenuation;
        attenuation *= attenuation;
        dc::sdk::SetLightAttenuation(index, attenuation, 0.0f, 0.0f);
    }
}

float GetSelectedLightRange(std::uint32_t index) {
    if (index < 16)
        return g_selectedLights[index].vectors[1].z;
    else
        return 0.0f;
}

void SetLightRuntimeFalloff(std::uint32_t index, float falloff) {
    if (falloff < 0.01f)
        falloff = 0.01f;
    if (index < 256)
        g_lightRuntimeEntries[index].value1c = falloff;
}

// @verified - d
void SetSelectedLightFalloff(std::uint32_t index, float falloff) {
    if (index < 16) {
        if (falloff < 0.01f)
            falloff = 0.01f;
        g_selectedLights[index].value1c = falloff;
        float attenuation = falloff * g_selectedLights[index].vectors[1].z;
        attenuation *= attenuation;
        attenuation *= attenuation;
        attenuation *= attenuation;
        dc::sdk::SetLightAttenuation(index, attenuation, 0.0f, 0.0f);
    }
}

void SetSelectedLightFalloffFromPointer(std::uint32_t index, const float *falloff) {
    SetSelectedLightFalloff(index, *falloff);
}

float GetLightRuntimeFalloff(std::uint32_t index) {
    if (index < 256)
        return g_lightRuntimeEntries[index].value1c;
    else
        return 0.0f;
}

float GetSelectedLightFalloff(std::uint32_t index) {
    if (index < 16)
        return g_selectedLights[index].value1c;
    else
        return 0.0f;
}

void GetLightRuntimeFalloffPair(std::uint32_t index, float *falloff) {
    if (index < 256) {
        falloff[0] = g_lightRuntimeEntries[index].value1c;
        falloff[1] = 0.0f;
    }
}

void GetSelectedLightFalloffPair(std::uint32_t index, float *falloff) {
    if (index < 16) {
        falloff[0] = g_selectedLights[index].value1c;
        falloff[1] = 0.0f;
    }
}

// @verified - d
void SetSelectedLightPosition(std::uint32_t index, const Vector3 &position) {
    if (index < 16) {
        if ((static_cast<std::int8_t>(g_selectedLights[index].indexes[1]) & ~8) == 3) {
            g_selectedLights[index].vector20.x = position.x - g_lightRootPosition.x;
            g_selectedLights[index].vector20.y = position.y - g_lightRootPosition.y;
            g_selectedLights[index].vector20.z = position.z - g_lightRootPosition.z;
        } else {
            LightRuntimeEntry *lights = g_selectedLights;
            LightRuntimeEntry *const light = &lights[index];
            light->vector20 = position;
        }
        dc::sdk::SetLightPosition(index, position.x, position.y, position.z);
    }
}

void GetSelectedLightPosition(std::uint32_t index, Vector3 *position) {
    if (index < 16) {
        if ((static_cast<std::int8_t>(g_selectedLights[index].indexes[1]) & ~8) == 3) {
            position->x = g_selectedLights[index].vector20.x + g_lightRootPosition.x;
            position->y = g_selectedLights[index].vector20.y + g_lightRootPosition.y;
            position->z = g_selectedLights[index].vector20.z + g_lightRootPosition.z;
        } else {
            const LightRuntimeEntry *lights = g_selectedLights;
            const LightRuntimeEntry *const light = &lights[index];
            *position = light->vector20;
        }
    }
}

// @verified - d
void SetSelectedLightDirection(std::uint32_t index, const Vector3 &direction) {
    if (index < 16) {
        LightRuntimeEntry *lights = g_selectedLights;
        LightRuntimeEntry *const light = &lights[index];
        *reinterpret_cast<Vector3 *>(&light->values2c[0]) = direction;
        dc::sdk::SetLightDirection(index, -direction.x, -direction.y, -direction.z);
    }
}

// @verified - d
void GetSelectedLightDirection(std::uint32_t index, Vector3 *direction) {
    if (index < 16) {
        const LightRuntimeEntry *lights = g_selectedLights;
        const LightRuntimeEntry *const light = &lights[index];
        *direction = *reinterpret_cast<const Vector3 *>(&light->values2c[0]);
    }
}

// @verified - d
void SetSelectedLightConeAngle(std::uint32_t index, std::int32_t angle) {
    if (index < 16) {
        g_selectedLights[index].unknown02 = static_cast<std::int16_t>(angle);
        dc::sdk::SetLightAngle(index, angle);
    }
}

std::int32_t GetSelectedLightConeAngle(std::uint32_t index) {
    if (index < 16)
        return g_selectedLights[index].unknown02;
    else
        return 0;
}

// @verified - d
void ResetSelectedLightEntry(std::uint32_t index) {
    const LightRuntimeEntry &defaults = disabledLight;
    if (index == 0 && g_currentActiveLight == 1) {
        EnableSelectedLight(0);
        SetLightKind(0, 2);
    } else {
        DisableSelectedLight(index);
        SetLightKind(index, 0);
    }
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    shenSetLightColour(index, *reinterpret_cast<const Vector4 *>(&defaults.vectors[0]));
#else
    shenSetLightColour(index, (Vector4){disabledLight.vectors[0].x, disabledLight.vectors[0].y, disabledLight.vectors[0].z, disabledLight.vectors[1].x});
#endif
    SetSelectedLightSceneIntensity(index, 1.0f);
    SetSelectedLightRange(index, 2.0f);
    SetSelectedLightFalloff(index, 0.6f);
    SetSelectedLightPosition(index, *GetRuntimeLightWorldPosition(&defaults));
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    SetSelectedLightDirection(index, *reinterpret_cast<const Vector3 *>(&defaults.values2c[0]));
#else
    SetSelectedLightDirection(index, (Vector3){disabledLight.values2c[0], disabledLight.values2c[1], disabledLight.value34});
#endif
    SetSelectedLightConeAngle(index, 0x071c);
}

float GetLightRuntimeSceneIntensity(std::uint32_t index) {
    if (index < 256)
        return g_lightRuntimeEntries[index].vectors[1].y;
    else
        return 0.0f;
}

// @verified - d
float GetSelectedLightSceneIntensity(std::uint32_t index) {
    if (index < 16)
        return g_selectedLights[index].vectors[1].y;
    else
        return 0.0f;
}

// @verified - d
void SetSelectedLightSceneIntensity(std::uint32_t index, float intensity) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if ((intensity < 0.0f ? -intensity : intensity) < 0.0001f)
        intensity = 0.0f;
    if (index < 16)
        g_selectedLights[index].vectors[1].y = intensity;
#else
    if (std::fabs(intensity) < 0.0001f)
        intensity = 0.0f;
    if (index < 16)
        g_selectedLights[index].vectors[1].y = intensity;
#endif
}

// @verified - d
void GetSelectedLightColour(std::uint32_t index, Vector4 *colour) {
    if (index < 16) {
        const LightRuntimeEntry *lights = g_selectedLights;
        const LightRuntimeEntry *const light = &lights[index];
        *colour = *reinterpret_cast<const Vector4 *>(&light->vectors[0]);
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void SetPublishedDiffuseChannels(float red, float green, float blue) {
    dc::sdk::g_mt7DiffuseChannels.x = red;
    dc::sdk::g_mt7DiffuseChannels.y = green;
    dc::sdk::g_mt7DiffuseChannels.z = blue;
}

static void SetPublishedSpecularChannels(float red, float green, float blue) {
    dc::sdk::g_mt7SpecularChannels.x = red;
    dc::sdk::g_mt7SpecularChannels.y = green;
    dc::sdk::g_mt7SpecularChannels.z = blue;
}

struct SdkLightRecord {
    std::int16_t enabled;
    std::byte retainedBeforeDiffuse[22];
    Vector3 diffuse;
    std::byte retainedBeforeDirection[68];
    Vector3 direction;
    std::byte retainedAfterDirection[60];
};
typedef char SdkLightRecordSizeCheck[(sizeof(SdkLightRecord) == dc::sdk::kLightRecordWords * sizeof(std::uint32_t)) ? 1 : -1];

void PublishGlobalLighting() {
    Vector4 colour;
    float intensity;
    std::int32_t index = 1;
    dc::sdk::TransformLightState();
    SetPublishedSpecularChannels(0.0f, 0.0f, 0.0f);
    SdkLightRecord *const records = reinterpret_cast<SdkLightRecord *>(dc::sdk::g_lightRecords);
    if (records[0].enabled == 1) {
        dc::sdk::SetLightModelColour(0, reinterpret_cast<const Vector4 *>(&g_selectedLights[0].vectors[0]));
        const Vector3 *const diffuse = &records[0].diffuse;
        SetPublishedDiffuseChannels(diffuse->x, diffuse->y, diffuse->z);
        intensity = g_selectedLights[0].vectors[1].y;
        Vector3 *const direction = &records[0].direction;
        direction->x *= intensity;
        direction->y *= intensity;
        direction->z *= intensity;
        dc::sdk::SetMT7SingleLightDirection(direction);
        colour.y = colour.z = colour.w = 0.0f;
        do {
            dc::sdk::SetLightModelColour(index, &colour);
        } while (++index <= 15);
        if (records[15].enabled == 1) {
            intensity = g_selectedLights[15].vectors[1].y;
            GetSelectedLightColour(15, &colour);
            colour.y *= intensity;
            colour.z *= intensity;
            colour.w *= intensity;
            dc::sdk::SetLightModelColour(15, &colour);
        }
    } else {
        colour.x = colour.y = colour.z = colour.w = 1.0f;
        dc::sdk::SetLightModelColour(0, &colour);
        const Vector3 *const diffuse = &records[0].diffuse;
        SetPublishedDiffuseChannels(diffuse->x, diffuse->y, diffuse->z);
        Vector3 *const direction = &records[0].direction;
        direction->x = direction->y = direction->z = 0.0f;
        dc::sdk::SetMT7SingleLightDirection(direction);
        for (; index <= 15; ++index) {
            if (records[index].enabled == 1) {
                intensity = g_selectedLights[index].vectors[1].y;
                GetSelectedLightColour(index, &colour);
                colour.y *= intensity;
                colour.z *= intensity;
                colour.w *= intensity;
            } else {
                colour.y = colour.z = colour.w = 0.0f;
            }
            dc::sdk::SetLightModelColour(index, &colour);
        }
    }
}
#else
// @verified - d
void PublishGlobalLighting() {
    dc::sdk::TransformLightState();
    dc::sdk::g_mt7SpecularChannels.x = 0.0f;
    dc::sdk::g_mt7SpecularChannels.y = 0.0f;
    dc::sdk::g_mt7SpecularChannels.z = 0.0f;
    Vector4 colour;
    if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[0][0]) == 1) {
        const LightRuntimeEntry &primary = g_selectedLights[0];
        const Vector4 primaryColour = {primary.vectors[0].x, primary.vectors[0].y, primary.vectors[0].z, primary.vectors[1].x};
        std::memcpy(&dc::sdk::g_lightRecords[0][0x14 / 4], &primaryColour, sizeof(primaryColour));
        dc::sdk::g_mt7DiffuseChannels.x = u32AsFloat(dc::sdk::g_lightRecords[0][0x18 / 4]);
        dc::sdk::g_mt7DiffuseChannels.y = u32AsFloat(dc::sdk::g_lightRecords[0][0x1c / 4]);
        dc::sdk::g_mt7DiffuseChannels.z = u32AsFloat(dc::sdk::g_lightRecords[0][0x20 / 4]);
        const float intensity = primary.vectors[1].y;
        for (std::uint32_t axis = 0; axis < 3; ++axis) {
            const std::uint32_t offset = 0x68 / 4 + axis;
            dc::sdk::g_lightRecords[0][offset] = floatAsU32(u32AsFloat(dc::sdk::g_lightRecords[0][offset]) * intensity);
        }
        const Vector3 direction = {u32AsFloat(dc::sdk::g_lightRecords[0][0x68 / 4]), u32AsFloat(dc::sdk::g_lightRecords[0][0x6c / 4]),
            u32AsFloat(dc::sdk::g_lightRecords[0][0x70 / 4])};
        dc::sdk::SetMT7SingleLightDirection(&direction);
        colour.y = 0.0f;
        colour.z = 0.0f;
        colour.w = 0.0f;
        for (std::uint32_t index = 1; index < 16; ++index)
            std::memcpy(&dc::sdk::g_lightRecords[index][0x14 / 4], &colour, sizeof(colour));
        if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[15][0]) == 1) {
            const float lastIntensity = g_selectedLights[15].vectors[1].y;
            GetSelectedLightColour(15, &colour);
            colour.y *= lastIntensity;
            colour.z *= lastIntensity;
            colour.w *= lastIntensity;
            std::memcpy(&dc::sdk::g_lightRecords[15][0x14 / 4], &colour, sizeof(colour));
        }
    } else {
        colour.x = 1.0f;
        colour.y = 1.0f;
        colour.z = 1.0f;
        colour.w = 1.0f;
        std::memcpy(&dc::sdk::g_lightRecords[0][0x14 / 4], &colour, sizeof(colour));
        dc::sdk::g_mt7DiffuseChannels.x = u32AsFloat(dc::sdk::g_lightRecords[0][0x18 / 4]);
        dc::sdk::g_mt7DiffuseChannels.y = u32AsFloat(dc::sdk::g_lightRecords[0][0x1c / 4]);
        dc::sdk::g_mt7DiffuseChannels.z = u32AsFloat(dc::sdk::g_lightRecords[0][0x20 / 4]);
        dc::sdk::g_lightRecords[0][0x68 / 4] = 0;
        dc::sdk::g_lightRecords[0][0x6c / 4] = 0;
        dc::sdk::g_lightRecords[0][0x70 / 4] = 0;
        const Vector3 direction = {};
        dc::sdk::SetMT7SingleLightDirection(&direction);
        for (std::uint32_t index = 1; index < 16; ++index) {
            if (static_cast<std::int16_t>(dc::sdk::g_lightRecords[index][0]) == 1) {
                const float intensity = g_selectedLights[index].vectors[1].y;
                GetSelectedLightColour(index, &colour);
                colour.y *= intensity;
                colour.z *= intensity;
                colour.w *= intensity;
            } else {
                colour.y = 0.0f;
                colour.z = 0.0f;
                colour.w = 0.0f;
            }
            std::memcpy(&dc::sdk::g_lightRecords[index][0x14 / 4], &colour, sizeof(colour));
        }
    }
}
#endif

void UpdateSdkLightTransforms() {
    dc::sdk::TransformLightRecords();
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ImmediateLightWork(void *) {
    PublishGlobalLighting();
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
float GetLightRuntimeRange(std::uint32_t index) {
    if (index < 256)
        return g_lightRuntimeEntries[index].vectors[1].z;
    else
        return 0.0f;
}

// @verified - d
std::int32_t GetLightRuntimeConeAngle(std::uint32_t index) {
    if (index < 256)
        return g_lightRuntimeEntries[index].unknown02;
    else
        return 0;
}

// @verified - d
std::uint32_t GetLightRuntimeKind(std::uint32_t index) {
    std::uint32_t kind;
    if (index > 255)
        kind = 6U;
    else {
        kind = static_cast<std::uint32_t>(static_cast<std::int8_t>(g_lightRuntimeEntries[index].indexes[1]) & ~8);
    }
    return kind;
}

// @verified - d
void GetLightRuntimeDirection(std::uint32_t index, Vector3 *direction) {
    if (index < 256) {
        const LightRuntimeEntry *lights = g_lightRuntimeEntries;
        const LightRuntimeEntry *const light = &lights[index];
        *direction = *reinterpret_cast<const Vector3 *>(&light->values2c[0]);
    }
}

// @verified - d
void PrepareCharacterShadowLighting() {
    g_characterShadowSavedLightMode = g_currentActiveLight;
    SetCurrentActiveLight(1);
    GetGlobalLighting(&g_characterShadowSavedAmbient);
    UpdateGlobalLighting(characterShadowBlack);
    for (std::int32_t index = 0; index < 16; ++index) {
        GetSelectedLightColour(index, &g_savedLightColours[index]);
        shenSetLightColour(index, characterShadowBlack);
    }
    PublishGlobalLighting();
}

// @verified - d
void RestoreCharacterShadowLighting() {
    SetCurrentActiveLight(g_characterShadowSavedLightMode);
    UpdateGlobalLighting(g_characterShadowSavedAmbient);
    for (std::int32_t index = 0; index < 16; ++index)
        shenSetLightColour(index, g_savedLightColours[index]);
    PublishGlobalLighting();
}

float GetLightSelectionRadius() {
    return g_lightSelectionRadius;
}

void SetLightSelectionRadius(float radius) {
    g_lightSelectionRadius = radius;
}

std::int32_t GetRankedLightLimit() {
    return g_rankedLightLimit;
}

static float ZeroNearZeroLightValue(float value) {
    float result = value;
    if ((value < 0.0f ? -value : value) < 0.0001f)
        result = 0.0f;
    return result;
}

void BuildLightingChunk(PackedLightDefinitionHeader *header) {
    header->name = CHUNK_LGHT;
    *reinterpret_cast<float *>(&header->reserved) = g_lightChunkVersion;
    header->mode = static_cast<std::int16_t>(GetCurrentActiveLight());
    header->colour[0] = motion_detail::F32ToF16(ZeroNearZeroLightValue(g_globalLighting.x));
    header->colour[1] = motion_detail::F32ToF16(ZeroNearZeroLightValue(g_globalLighting.y));
    header->colour[2] = motion_detail::F32ToF16(ZeroNearZeroLightValue(g_globalLighting.z));
    header->colour[3] = motion_detail::F32ToF16(ZeroNearZeroLightValue(g_globalLighting.w));
    std::int32_t written;
    std::int32_t count;
    const std::int32_t mode = GetCurrentActiveLight();
    if (mode == 1)
        count = 1;
    else if (mode == 2)
        count = 16;
    else
        count = 256;
    PackedLightDefinitionRecord *record = reinterpret_cast<PackedLightDefinitionRecord *>(header + 1);
    std::int32_t index;
    for (written = index = 0; index < count; ++index) {
        if (!GetLightRuntimeKind(index))
            continue;
        record->index = static_cast<std::int16_t>(index);
        record->enabled = static_cast<std::int16_t>(GetLightEntryIndex(index));
        record->flags = static_cast<std::int16_t>(GetLightRuntimeType(index));
        Vector4 colour;
        GetLightRuntimeColour(index, &colour);
        record->parameters[0] = motion_detail::F32ToF16(ZeroNearZeroLightValue(colour.y));
        record->parameters[1] = motion_detail::F32ToF16(ZeroNearZeroLightValue(colour.z));
        record->parameters[2] = motion_detail::F32ToF16(ZeroNearZeroLightValue(colour.w));
        record->parameters[3] = motion_detail::F32ToF16(ZeroNearZeroLightValue(GetLightRuntimeSceneIntensity(index)));
        record->parameters[4] = motion_detail::F32ToF16(ZeroNearZeroLightValue(GetLightRuntimeRange(index)));
        record->parameters[5] = motion_detail::F32ToF16(ZeroNearZeroLightValue(GetLightRuntimeFalloff(index)));
        Vector3 position;
        GetLightRuntimePosition(index, &position);
        record->parameters[6] = motion_detail::F32ToF16(ZeroNearZeroLightValue(position.x));
        record->parameters[7] = motion_detail::F32ToF16(ZeroNearZeroLightValue(position.y));
        record->parameters[8] = motion_detail::F32ToF16(ZeroNearZeroLightValue(position.z));
        Vector3 direction;
        GetLightRuntimeDirection(index, &direction);
        record->coneAngle = static_cast<std::int16_t>(GetLightRuntimeConeAngle(index));
        record->parameters[9] = motion_detail::F32ToF16(ZeroNearZeroLightValue(direction.x));
        record->parameters[10] = motion_detail::F32ToF16(ZeroNearZeroLightValue(direction.y));
        record->parameters[11] = motion_detail::F32ToF16(ZeroNearZeroLightValue(direction.z));
        ++written;
        ++record;
    }
    header->lightCount = static_cast<std::int16_t>(written);
    header->byteSize = 0x1000;
}

void InitializeDefaultPointLight(std::uint32_t index, const Vector3 &position) {
    ResetLightEntry(index);
    SetLightFlagForIndex(index, 3);
    SetLightRuntimePosition(index, position);
    EnableLightEntry(index);
}

void InitializeDefaultSunLight(std::uint32_t index, const Vector3 &position, const Vector3 &direction) {
    ResetLightEntry(index);
    SetLightFlagForIndex(index, 4);
    SetLightRuntimePosition(index, position);
    SetLightSunDirection(index, direction);
    EnableLightEntry(index);
}

LightInterpolator *AllocateLINV() {
    LightInterpolator *const value = static_cast<LightInterpolator *>(HLib::AllocateTaskData(sizeof(LightInterpolator), SHEN_NAME(LINV)));
    ZeroWordRange(reinterpret_cast<std::uint32_t *>(value), reinterpret_cast<std::uint32_t *>(&value->frameCount));
    return value;
}

void FreeLINV(LightInterpolator *value) {
    HLib::FreeTaskData(value);
}

std::int32_t AdvanceLINV(LightInterpolator *value) {
    if (value->frame == value->frameCount)
        return 0;
    ++value->frame;
    Vector3 *const current = &value->current;
    if (value->frame == value->frameCount) {
        current->x = value->target.x;
        current->y = value->target.y;
        current->z = value->target.z;
        return 2;
    }
    const Vector3 *const step = &value->step;
    current->x += step->x;
    if (std::fabs(current->x) < 1.1920928955078125e-07f)
        current->x = 0.0f;
    current->y += step->y;
    if (std::fabs(current->y) < 1.1920928955078125e-07f)
        current->y = 0.0f;
    current->z += step->z;
    if (std::fabs(current->z) < 1.1920928955078125e-07f)
        current->z = 0.0f;
    return 1;
}

void linvSetImmediate(LightInterpolator *value, const Vector3 *target) {
    Vector3 *const end = &value->target;
    Vector3 *const step = &value->step;
    value->current.x = end->x = target->x;
    value->current.y = end->y = target->y;
    value->current.z = end->z = target->z;
    step->x = value->step.y = value->step.z = 0.0f;
    value->frameCount = 0;
    value->frame = 0;
}

void BeginLINVTransition(LightInterpolator *value, const Vector3 *target, std::int32_t frames) {
    if (frames < 1) {
        linvSetImmediate(value, target);
        return;
    }
    Vector3 *const current = &value->current;
    Vector3 *const end = &value->target;
    Vector3 *const step = &value->step;
    float component = target->x;
    if (std::fabs(component) < 1.1920928955078125e-07f)
        component = 0.0f;
    end->x = component;
    component = target->y;
    if (std::fabs(component) < 1.1920928955078125e-07f)
        component = 0.0f;
    end->y = component;
    component = target->z;
    if (std::fabs(component) < 1.1920928955078125e-07f)
        component = 0.0f;
    end->z = component;
    const float duration = static_cast<float>(frames);
    float difference = end->x - current->x;
    if (std::fabs(difference) < 1.1920928955078125e-07f)
        step->x = 0.0f;
    else if (std::fabs(difference /= duration) < 1.1920928955078125e-07f)
        step->x = 0.0f;
    else
        step->x = difference;
    difference = end->y - current->y;
    if (std::fabs(difference) < 1.1920928955078125e-07f)
        step->y = 0.0f;
    else if (std::fabs(difference /= duration) < 1.1920928955078125e-07f)
        step->y = 0.0f;
    else
        step->y = difference;
    difference = end->z - current->z;
    if (std::fabs(difference) < 1.1920928955078125e-07f)
        step->z = 0.0f;
    else if (std::fabs(difference /= duration) < 1.1920928955078125e-07f)
        step->z = 0.0f;
    else
        step->z = difference;
    value->frame = 0;
    value->frameCount = frames;
}

Vector3 *GetLINVCurrent(LightInterpolator *value, Vector3 *output) {
    if (std::fabs(value->current.x) < 1.1920928955078125e-07f)
        value->current.x = 0.0f;
    output->x = value->current.x;
    if (std::fabs(value->current.y) < 1.1920928955078125e-07f)
        value->current.y = 0.0f;
    output->y = value->current.y;
    if (std::fabs(value->current.z) < 1.1920928955078125e-07f)
        value->current.z = 0.0f;
    output->z = value->current.z;
    return output;
}

Vector3 *GetLINVTarget(LightInterpolator *value, Vector3 *output) {
    Vector3 *const target = &value->target;
    if (std::fabs(target->x) < 1.1920928955078125e-07f)
        target->x = 0.0f;
    output->x = target->x;
    if (std::fabs(target->y) < 1.1920928955078125e-07f)
        target->y = 0.0f;
    output->y = target->y;
    if (std::fabs(target->z) < 1.1920928955078125e-07f)
        target->z = 0.0f;
    output->z = target->z;
    return output;
}

std::int32_t linvGetFramesRemaining(const LightInterpolator *value) {
    return value->frameCount - value->frame;
}

std::int32_t IsLINVTransitionActive(const LightInterpolator *value) {
    if (value->frame == value->frameCount)
        return 0;
    return 1;
}
#endif

} // namespace s2
} // namespace shenmue

#endif
