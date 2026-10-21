#include "s1/task/lght.hpp"
#include "s1/d3t/d3t_shader_load.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/lighting.hpp"
#include "s1/modern_lighting.hpp"
#include "s1/model_lighting.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/motion.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/scen.hpp"
#include "s1/ui_runtime.hpp"

#include <bit>
#include <cstdint>
#include <cmath>
#include <cstring>

namespace shenmue::s1 {

HLib::Task *g_lghtTask = nullptr;

std::array<EffectLightRenderSlot, 32> g_effectLightRenderSlots{};
std::uint32_t g_effectEnabledSpotLightMask = 0;
std::uint32_t g_effectEnabledStandardLightMask = 0;
std::uint32_t g_defaultLightMask = 7;
float g_defaultLightRadius = 10.0f;
std::int32_t g_shadowFlag = 1;

std::int32_t g_hasSetupLightFlags = 0;
float g_shadowBlackness = 1.0f;
std::int32_t g_shadowStrength = 0;
Vector4 g_sceneLightingActiveColour{0.3f, 0.3f, 0.3f, 0.3f};
float g_sceneObjectMaxScale = 1.0f;
Vector4 g_sceneLightingBaseColour{0.3f, 0.3f, 0.3f, 0.3f};
std::int32_t g_cachedGlobalLightingAdjustFlag = 0;
std::int32_t g_prevCachedGlobalLightingAdjustFlag = 0;
float g_inverseShadowRange = 10.0f;
std::int32_t g_effectsNeedInitialization = 1;

LightStorage *g_lightDataStatics = nullptr;
LightStorage *g_LightData = nullptr;
ShenmueLightSlot *g_lightRenderSlots = nullptr;

namespace {

std::array<const std::byte *, 24> g_lightTables{};
std::array<std::size_t, 24> g_lightTableSizes{};
Vector4 g_defaultSceneLighting{0.3f, 0.3f, 0.3f, 0.3f};
std::int32_t g_currentLghtMode = 0;
float g_lightSelectionRadius = 60.0f;
std::int32_t g_lightSelectionCapacity = 28;
ShenmueLightSlot g_defaultLightSlot{};
float g_shadowRange = 0.1f;
std::array<std::uint8_t, 32> g_shadowLightSlotByRank{};
bool g_lghtInitialized = false;

Vector3 NormalizeVector(Vector3 value) {
    const float length = std::sqrt(value.x * value.x + value.y * value.y + value.z * value.z);
    if (length > 0.0f) {
        const float inverse = static_cast<float>(1.0 / static_cast<double>(length));
        value.x *= inverse;
        value.y *= inverse;
        value.z *= inverse;
    }
    return value;
}

float PowEighth(float value) {
    value *= value;
    value *= value;
    return value * value;
}

void UpdateEffectLightShaderConstants(EffectLightRenderSlot &effect) {
    const float red = effect.intensity * effect.colourR;
    const float green = effect.intensity * effect.colourG;
    const float blue = effect.intensity * effect.colourB;
    effect.shaderConstants04[4] = red * 0.3f + green * 0.59f + blue * 0.11f;
    effect.shaderConstants04[5] = red;
    effect.shaderConstants04[6] = green;
    effect.shaderConstants04[7] = blue;
}

void UpdateEffectLightMasks(std::uint32_t lightIndex) {
    if (lightIndex >= g_effectLightRenderSlots.size())
        return;
    const std::uint32_t bit = 1U << lightIndex;
    g_effectEnabledSpotLightMask &= ~bit;
    g_effectEnabledStandardLightMask &= ~bit;
    const EffectLightRenderSlot &effect = g_effectLightRenderSlots[lightIndex];
    if (effect.enabled != 1)
        return;
    if ((effect.modeFlags & 8) != 0)
        g_effectEnabledSpotLightMask |= bit;
    else
        g_effectEnabledStandardLightMask |= bit;
}

void SetEffectLightCone(EffectLightRenderSlot &effect, std::int32_t angle) {
    const std::uint16_t raw = static_cast<std::uint16_t>(angle);
    const std::int16_t folded = raw <= 0x8000U ? static_cast<std::int16_t>(raw) : static_cast<std::int16_t>(-static_cast<std::int16_t>(raw));
    effect.coneCos = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
    effect.coneSin = sin_fixed16(static_cast<std::int16_t>(raw));
    effect.inverseOneMinusConeCos = 1.0f / (1.0f - effect.coneCos);
}

void CopySlotToEffect(std::uint32_t lightIndex, const ShenmueLightSlot &slot) {
    if (lightIndex >= g_effectLightRenderSlots.size())
        return;
    EffectLightRenderSlot &effect = g_effectLightRenderSlots[lightIndex];
    effect.enabled = static_cast<std::int16_t>(slot.enabled);
    effect.modeFlags = static_cast<std::int16_t>(slot.modeFlags);
    effect.colourR = slot.colourR;
    effect.colourG = slot.colourG;
    effect.colourB = slot.colourB;
    effect.position = slot.position;
    effect.negativeNormalizedDirection = NormalizeVector({-slot.direction.x, -slot.direction.y, -slot.direction.z});
    effect.range = slot.range;
    effect.rangeSquared = slot.range * slot.range;
    effect.inverseRange = 1.0f / slot.range;
    const std::uint16_t type = static_cast<std::uint16_t>(slot.modeFlags) & 0xfff7U;
    const float scaled = slot.range * slot.falloff;
    effect.attenuationShape = type == 3U ? scaled : PowEighth(scaled);
    SetEffectLightCone(effect, slot.coneAngle);
    effect.intensity = static_cast<double>(std::fabs(slot.intensity)) < 0.0001 ? 0.0f : slot.intensity;
    UpdateEffectLightShaderConstants(effect);
    UpdateEffectLightMasks(lightIndex);
}

float Fixed16ToFloat(std::int16_t value) {
    if (!value)
        return 0.0f;
    std::uint32_t raw = static_cast<std::uint32_t>(value);
    if (value < 0)
        raw = (raw & 0x7fffU) | 0xfffc0000U;
    raw = (raw << 13U) + 0x38000000U;
    return std::bit_cast<float>(raw);
}

template <typename T> T ReadPacked(const std::byte *bytes, std::size_t offset) {
    T value{};
    std::memcpy(&value, bytes + offset, sizeof(value));
    return value;
}

void ApplyPackedLight(std::uint32_t lightIndex, const std::byte *data) {
    if (!g_LightData || !data || lightIndex >= 128)
        return;
    ShenmueLightSlot &slot = g_LightData->slots[lightIndex];


    slot.enabled = ReadPacked<std::int16_t>(data, 2);
    slot.modeFlags = static_cast<std::uint16_t>(ReadPacked<std::int16_t>(data, 4));
    slot.colourScale = 1.0f;
    slot.colourR = Fixed16ToFloat(ReadPacked<std::int16_t>(data, 6));
    slot.colourG = Fixed16ToFloat(ReadPacked<std::int16_t>(data, 8));
    slot.colourB = Fixed16ToFloat(ReadPacked<std::int16_t>(data, 10));
    slot.intensity = Fixed16ToFloat(ReadPacked<std::int16_t>(data, 12));
    if (slot.intensity == 0.0f)
        slot.intensity = 1.0f;
    slot.range = std::max(Fixed16ToFloat(ReadPacked<std::int16_t>(data, 14)), 0.01f);
    slot.falloff = std::max(Fixed16ToFloat(ReadPacked<std::int16_t>(data, 16)), 0.01f);
    slot.position = {
        Fixed16ToFloat(ReadPacked<std::int16_t>(data, 18)),
        Fixed16ToFloat(ReadPacked<std::int16_t>(data, 20)),
        Fixed16ToFloat(ReadPacked<std::int16_t>(data, 22)),
    };
    slot.direction = {
        Fixed16ToFloat(ReadPacked<std::int16_t>(data, 24)),
        Fixed16ToFloat(ReadPacked<std::int16_t>(data, 26)),
        Fixed16ToFloat(ReadPacked<std::int16_t>(data, 28)),
    };
    slot.coneAngle = ReadPacked<std::int16_t>(data, 30);
    if (lightIndex < 32 && g_lightRenderSlots) {
        g_lightRenderSlots[lightIndex] = slot;
        CopySlotToEffect(lightIndex, slot);
    }
}

void ApplyLegacyLight(std::uint32_t lightIndex, const std::byte *data) {
    if (!g_LightData || !data || lightIndex >= 128)
        return;
    ShenmueLightSlot &slot = g_LightData->slots[lightIndex];
    slot.enabled = static_cast<std::int32_t>(ReadPacked<std::uint32_t>(data, 0));
    slot.modeFlags = ReadPacked<std::uint32_t>(data, 4);
    slot.colourScale = ReadPacked<float>(data, 8);
    slot.colourR = ReadPacked<float>(data, 12);
    slot.colourG = ReadPacked<float>(data, 16);
    slot.colourB = ReadPacked<float>(data, 20);
    float intensity = ReadPacked<float>(data, 0x18);
    if (intensity == 0.0f) {
        intensity = 1.0f;
        std::memcpy(const_cast<std::byte *>(data) + 0x18, &intensity, sizeof(intensity));
    }
    slot.intensity = intensity;
    slot.range = std::max(ReadPacked<float>(data, 0x1c), 0.01f);
    slot.falloff = std::max(ReadPacked<float>(data, 0x20), 0.01f);
    slot.position = {
        ReadPacked<float>(data, 0x28),
        ReadPacked<float>(data, 0x2c),
        ReadPacked<float>(data, 0x30),
    };
    slot.direction = {
        ReadPacked<float>(data, 0x34),
        ReadPacked<float>(data, 0x38),
        ReadPacked<float>(data, 0x3c),
    };
    slot.coneAngle = ReadPacked<std::int32_t>(data, 0x40);
    if (lightIndex < 32 && g_lightRenderSlots) {
        g_lightRenderSlots[lightIndex] = slot;
        CopySlotToEffect(lightIndex, slot);
    }
}

void ResetLightSlotToDefaults(std::uint32_t lightIndex) {
    if (lightIndex >= 128)
        return;
    const bool defaultDirectional = lightIndex == 0 && g_cachedGlobalLightingAdjustFlag == 1;
    ShenmueLightSlot &slot = g_LightData->slots[lightIndex];
    slot.enabled = defaultDirectional ? 1 : 0;
    slot.modeFlags = defaultDirectional ? 2U : 0U;
    if (lightIndex < 32) {
        g_lightRenderSlots[lightIndex].enabled = slot.enabled;
        g_lightRenderSlots[lightIndex].modeFlags = slot.modeFlags;
        g_effectLightRenderSlots[lightIndex].enabled = static_cast<std::int16_t>(slot.enabled);
        g_effectLightRenderSlots[lightIndex].modeFlags = static_cast<std::int16_t>(slot.modeFlags);
        UpdateEffectLightMasks(lightIndex);
    }
    const Vector4 colour{1.0f, 1.0f, 1.0f, 1.0f};
    const Vector3 position{0.0f, 2.0f, 0.0f};
    const Vector3 direction{0.0f, -1.0f, 0.0f};
    SetRGBStruct(lightIndex, &colour);
    SetLightIntensity(lightIndex, 1.0f);
    SetLightFalloffRangeByIndex(lightIndex, 2.0f);
    SetLightFalloff(lightIndex, 0.25f);
    SetLightDirectionVectorByIndex(lightIndex, &position);
    SetEffectTransformByIndex(lightIndex, &direction);
    SetLightAngleByIndex(lightIndex, 1820);
}

void shenSetLightingModeInternal(std::int32_t index) {
    g_sceneLightingBaseColour = g_defaultSceneLighting;
    g_sceneLightingActiveColour = g_defaultSceneLighting;
    g_cachedGlobalLightingAdjustFlag = 0;
    for (std::uint32_t lightIndex = 0; lightIndex < 128; ++lightIndex) {
        ResetLightSlotToDefaults(lightIndex);
    }
    g_cachedGlobalLightingAdjustFlag = index;

    if (index == 1) {
        ResetLightSlotToDefaults(0);
        g_effectsNeedInitialization = 0;
    } else {
        g_effectsNeedInitialization = 1;
    }
}

void lightEnableDirLightInternal(std::uint32_t lightIndex, const Vector3 *direction) {
    ResetLightSlotToDefaults(lightIndex);
    SetLightBaseType(lightIndex, 2);
    SetEffectTransformByIndex(lightIndex, direction);
    lightSetSlotEnabled(lightIndex, 1);
}

} // namespace




std::int32_t BuildActiveSpotLightRenderParams(EffectLightRenderSlot *lights) {
    if (g_cachedGlobalLightingAdjustFlag == 1)
        return 0;
    std::int32_t count = 0;
    for (std::uint32_t index = 0; index < 128; ++index) {
        const ShenmueLightSlot &slot = *LGHT_GetDataSlot(index);
        if (slot.enabled != 1 || (slot.modeFlags & 8U) == 0U)
            continue;
        EffectLightRenderSlot &light = lights[count++];
        light.modeFlags = static_cast<std::int16_t>(slot.modeFlags);
        light.colourR = slot.colourR;
        light.colourG = slot.colourG;
        light.colourB = slot.colourB;
        const float red = slot.intensity * slot.colourR;
        const float green = slot.intensity * slot.colourG;
        const float blue = slot.intensity * slot.colourB;
        light.shaderConstants04[5] = red;
        light.shaderConstants04[6] = green;
        light.shaderConstants04[7] = blue;
        light.shaderConstants04[4] =
            static_cast<float>((static_cast<double>(red) * 0.3 + static_cast<double>(green) * 0.59) + static_cast<double>(blue) * 0.11);
        light.position = slot.position;
        light.negativeNormalizedDirection = {-slot.direction.x, -slot.direction.y, -slot.direction.z};
        light.range = slot.range;
        light.rangeSquared = slot.range * slot.range;
        light.inverseRange = 1.0f / slot.range;
        light.attenuationShape = fast_powf_approx(slot.falloff * slot.range, 8.0f);
        SetEffectLightCone(light, slot.coneAngle);
        light.intensity = slot.intensity;
    }
    return count;
}

Vector3 UpdateSunForDateTime(const LightDateTime &dateTime, float tiltAngle, std::int32_t clampAngle) {
    static constexpr std::array<std::int32_t, 12> accumulatedDays{31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    const float timeOfDay = static_cast<float>(dateTime.minutes) / 60.0f + static_cast<float>(dateTime.hour) + static_cast<float>(dateTime.seconds) / 3600.0f;
    const float sunAngle = static_cast<float>((static_cast<double>(timeOfDay) - 12.0) * -15.0 * 3.14 / 180.0);
    const float sunAngleCos = std::sin(1.5707964f - std::fabs(sunAngle));
    const float seasonalDegrees =
        static_cast<float>(202 - accumulatedDays[static_cast<std::size_t>(dateTime.month - 1)] - dateTime.day) / 365.0f * 360.0f / 180.0f;
    const float seasonalAngle = static_cast<float>(static_cast<double>(seasonalDegrees) * 3.14);
    const float seasonalSin = std::sin(1.5707964f - std::fabs(seasonalAngle));
    float elevation =
        static_cast<float>((90.0 - (static_cast<double>(tiltAngle) - static_cast<double>(seasonalSin) * 23.5)) * static_cast<double>(sunAngleCos));
    if (clampAngle == 1)
        elevation = std::fmax(25.0f, std::fabs(elevation));

    Vector3 direction{0.0f, 0.0f, -1.0f};
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY(std::sin(sunAngle), sunAngleCos);
    const float azimuthalAngle = -static_cast<float>(static_cast<double>(elevation) * 3.14 / 180.0);
    Matrix4x4::RotateAroundX(std::sin(azimuthalAngle), std::sin(1.5707964f - std::fabs(azimuthalAngle)));
    Matrix4x4::Transform(direction, direction);
    Matrix4x4::Pop(1);
    return direction;
}
const ShenmueLightSlot *LGHT_GetDataSlot(std::uint32_t lightIndex) {
    return g_LightData && lightIndex < 128 ? &g_LightData->slots[lightIndex] : nullptr;
}

const ShenmueLightSlot *LGHT_GetRenderSlot(std::uint32_t lightIndex) {
    return g_lightRenderSlots && lightIndex < 32 ? &g_lightRenderSlots[lightIndex] : nullptr;
}
std::uint32_t LGHT_GetShadowSourceIndex(std::uint32_t rank) noexcept {
    return rank < g_shadowLightSlotByRank.size() ? g_shadowLightSlotByRank[rank] : 33U;
}

void LGHT_SetRenderIntensity(std::uint32_t lightIndex, float intensity) {
    if (lightIndex >= 32)
        return;
    if (g_lightRenderSlots)
        g_lightRenderSlots[lightIndex].intensity = intensity;
    g_effectLightRenderSlots[lightIndex].intensity = intensity;
}

std::int32_t LGHT_GetMode() {
    return g_cachedGlobalLightingAdjustFlag;
}

void LGHT_SetLightIntensityRadius(float radius) noexcept {
    g_lightSelectionRadius = radius;
}

void LGHT_Parse(std::uint32_t index) {
    constexpr std::uint32_t kLightChunk = static_cast<std::uint32_t>('L') | (static_cast<std::uint32_t>('G') << 8U) | (static_cast<std::uint32_t>('H') << 16U) |
        (static_cast<std::uint32_t>('T') << 24U);


    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (!control || !MAPINFO_FindChunkIn(control->mapInfo, g_mapInfoSize, kLightChunk))
        return;
    if (index >= g_lightTables.size() || !g_lightTables[index])
        return;
    const std::byte *const light = g_lightTables[index];
    const std::size_t tableSize = g_lightTableSizes[index];
    if (tableSize < sizeof(std::uint32_t))
        return;

    const std::uint32_t tag = ReadPacked<std::uint32_t>(light, 0);
    if (tag == kLightChunk) {
        if (tableSize < 0x18)
            return;
        const std::int32_t mode = ReadPacked<std::int16_t>(light, 0x0e);
        shenSetLightingModeInternal(mode);
        g_currentLghtMode = (mode == 3);
        g_sceneLightingBaseColour = {
            Fixed16ToFloat(ReadPacked<std::int16_t>(light, 0x10)),
            Fixed16ToFloat(ReadPacked<std::int16_t>(light, 0x12)),
            Fixed16ToFloat(ReadPacked<std::int16_t>(light, 0x14)),
            Fixed16ToFloat(ReadPacked<std::int16_t>(light, 0x16)),
        };
        g_sceneLightingActiveColour = g_sceneLightingBaseColour;

        const std::size_t availableCount = (tableSize - 0x18) / 0x20;
        const std::size_t declaredCount = static_cast<std::size_t>(std::max<std::int32_t>(ReadPacked<std::int16_t>(light, 0x0c), 0));
        const std::size_t count = std::min(declaredCount, availableCount);
        const std::byte *entry = light + 0x18;
        for (std::size_t parsed = 0; parsed < count; ++parsed, entry += 0x20) {
            const auto lightIndex = static_cast<std::uint16_t>(ReadPacked<std::int16_t>(entry, 0));
            ApplyPackedLight(lightIndex, entry);
        }
        return;
    }

    if (tableSize < 0x58)
        return;
    const std::int32_t mode = static_cast<std::int32_t>(tag);
    shenSetLightingModeInternal(mode);
    g_sceneLightingBaseColour = {
        ReadPacked<float>(light, 4),
        ReadPacked<float>(light, 8),
        ReadPacked<float>(light, 12),
        ReadPacked<float>(light, 16),
    };
    g_sceneLightingActiveColour = g_sceneLightingBaseColour;
    ApplyLegacyLight(mode == 1 ? 0U : 16U, light + 0x14);
    if (mode != 1) {
        for (std::uint32_t lightIndex = 1; lightIndex <= 15; ++lightIndex) {
            const std::size_t offset = 0x58 + static_cast<std::size_t>(lightIndex - 1) * 0x44;
            if (offset + 0x44 > tableSize)
                break;
            ApplyLegacyLight(lightIndex, light + offset);
        }
    }
}

void LGHT_ResetRuntime() {
    g_lghtTask = nullptr;
    g_lghtInitialized = false;
    g_lightTables.fill(nullptr);
    g_lightTableSizes.fill(0);
    g_currentLghtMode = 0;
    g_lightSelectionRadius = 60.0f;
    g_lightSelectionCapacity = 28;
    g_prevCachedGlobalLightingAdjustFlag = g_cachedGlobalLightingAdjustFlag;
    g_cachedGlobalLightingAdjustFlag = 0;
    g_effectsNeedInitialization = 1;
    g_sceneLightingBaseColour = g_defaultSceneLighting;
    g_sceneLightingActiveColour = g_defaultSceneLighting;
    g_shadowLightSlotByRank.fill(0);
    g_effectLightRenderSlots.fill(EffectLightRenderSlot{});
    g_effectEnabledSpotLightMask = 0;
    g_effectEnabledStandardLightMask = 0;
    LightStorage *const storage = g_LightData ? g_LightData : g_lightDataStatics;
    if (storage)
        std::memset(storage, 0, sizeof(*storage));
    g_LightData = g_lightDataStatics;
    g_lightRenderSlots = g_lightDataStatics ? g_lightDataStatics->renderSlots : nullptr;
}


bool GetLightPosition(std::uint32_t lightIndex, Vector3 *output) {
    if (!g_LightData || !output || lightIndex >= 128U)
        return false;
    *output = g_LightData->slots[lightIndex].position;
    return true;
}

void LGHT_SetDateTime(const LightDateTime &dateTime) {
    g_currentDateTime.month = static_cast<std::uint8_t>(dateTime.month);
    g_currentDateTime.day = static_cast<std::uint8_t>(dateTime.day);
    g_currentDateTime.hours = static_cast<std::uint8_t>(dateTime.hour);
    g_currentDateTime.minutes = static_cast<std::uint8_t>(dateTime.minutes);
    g_currentDateTime.seconds = static_cast<std::uint8_t>(dateTime.seconds);
}

void LGHT_Update() {
    if (!g_lghtInitialized || !g_LightData)
        return;
    const std::int32_t cachedMode = g_cachedGlobalLightingAdjustFlag;
    g_prevCachedGlobalLightingAdjustFlag = cachedMode;
    const ShenmueLightSlot &sun = g_LightData->slots[0];
    const std::uint32_t sunMode = sun.modeFlags & ~8U;
    if (sunMode == 0U || sunMode == 2U) {
        const LightDateTime dateTime{static_cast<std::int32_t>(g_currentDateTime.month), static_cast<std::int32_t>(g_currentDateTime.day),
            static_cast<std::int32_t>(g_currentDateTime.hours), static_cast<std::int32_t>(g_currentDateTime.minutes),
            static_cast<std::int32_t>(g_currentDateTime.seconds)};
        (void)UpdateSunForDateTime(dateTime, 22.18f, 0);
    }
    if (g_currentLghtMode == 0 || cachedMode != 3) {
        for (std::uint32_t rank = 0; rank < 32; ++rank)
            g_shadowLightSlotByRank[rank] = static_cast<std::uint8_t>(rank);
        return;
    }

    struct Candidate {
        std::uint32_t index;
        float distanceSquared;
    };
    std::array<Candidate, 128> candidates{};
    std::size_t count = 0;
    const Vector3 eye = g_CameraEyePos;
    const float radiusSquared = g_lightSelectionRadius * g_lightSelectionRadius;
    for (std::uint32_t index = 1; index < 128; ++index) {
        if (index >= 29 && index <= 31)
            continue;
        const ShenmueLightSlot &slot = g_LightData->slots[index];
        if (slot.enabled != 1)
            continue;
        const float dx = eye.x - slot.position.x;
        const float dy = eye.y - slot.position.y;
        const float dz = eye.z - slot.position.z;
        const float distanceSquared = (slot.modeFlags & ~8U) == 2U ? 0.0f : dx * dx + dy * dy + dz * dz;
        if (distanceSquared <= radiusSquared && count < candidates.size()) {
            candidates[count++] = {index, distanceSquared};
        }
    }
    std::stable_sort(
        candidates.begin(), candidates.begin() + count, [](const Candidate &lhs, const Candidate &rhs) { return lhs.distanceSquared < rhs.distanceSquared; });
    const std::size_t selected = std::min<std::size_t>(count, static_cast<std::size_t>(std::max(g_lightSelectionCapacity, 0)));
    for (std::uint32_t rank = 1; rank <= 28; ++rank) {
        if (rank <= selected) {
            const std::uint32_t source = candidates[rank - 1].index;
            g_shadowLightSlotByRank[rank] = static_cast<std::uint8_t>(source);
            if (g_lightRenderSlots) {
                g_lightRenderSlots[rank] = g_LightData->slots[source];
                CopySlotToEffect(rank, g_lightRenderSlots[rank]);
            }
        } else {
            g_shadowLightSlotByRank[rank] = 0;
            if (g_lightRenderSlots)
                g_lightRenderSlots[rank] = g_defaultLightSlot;
            CopySlotToEffect(rank, g_defaultLightSlot);
        }
    }
}

void LGHT_Initialize(const void *mapInfo, std::uint32_t mapInfoSize) {
    constexpr std::uint32_t kLightChunk = static_cast<std::uint32_t>('L') | (static_cast<std::uint32_t>('G') << 8U) | (static_cast<std::uint32_t>('H') << 16U) |
        (static_cast<std::uint32_t>('T') << 24U);
    SetNewSceneDetails(static_cast<std::uint32_t>(g_sceneId), shen_name{g_areaId});
    g_modelLightOverrides.count = 0;
    g_modelLightCount = 2;
    LGHT_ResetRuntime();
    if (!mapInfo || mapInfoSize < 8)
        return;
    const auto *const bytes = static_cast<const std::byte *>(mapInfo);
    auto loadChunk = [&](std::size_t chunkOffset) {
        const std::uint32_t next = ReadPacked<std::uint32_t>(bytes, chunkOffset + 4);
        const std::size_t chunkEnd = next >= 8 && next <= mapInfoSize - chunkOffset ? chunkOffset + next : mapInfoSize;
        std::size_t cursor = chunkOffset + 8;
        for (std::size_t index = 0; index < g_lightTables.size() && cursor + 8 <= chunkEnd; ++index) {
            const std::uint32_t tableTag = ReadPacked<std::uint32_t>(bytes, cursor);
            const std::uint32_t step = tableTag == kLightChunk ? ReadPacked<std::uint32_t>(bytes, cursor + 4) : 0x454U;
            if (!step || step > chunkEnd - cursor)
                break;
            g_lightTables[index] = bytes + cursor;
            g_lightTableSizes[index] = step;
            cursor += step;
        }
        LGHT_Parse(0);
    };

    if (const std::byte *const chunk = MAPINFO_FindChunkIn(mapInfo, mapInfoSize, kLightChunk)) {
        loadChunk(static_cast<std::size_t>(chunk - bytes));
        g_currentLghtMode = 0;
        g_lghtInitialized = true;
        g_prevCachedGlobalLightingAdjustFlag = g_cachedGlobalLightingAdjustFlag;
        (void)LGHT_InitializeTask();
        return;
    }
    if (!g_lightTables[0]) {
        shenSetLightingModeInternal(1);
        ShenmueLightSlot &sun = g_LightData->slots[0];
        sun.enabled = 1;
        sun.modeFlags = 2;
        sun.direction = {0.0f, -0.707107f, -0.707107f};
        if (g_lightRenderSlots) {
            g_lightRenderSlots[0] = sun;
            CopySlotToEffect(0, sun);
        }
    }
    g_lghtInitialized = true;
    g_prevCachedGlobalLightingAdjustFlag = g_cachedGlobalLightingAdjustFlag;
    (void)LGHT_InitializeTask();
}

void shenSetLightingMode(std::int32_t index) {
    shenSetLightingModeInternal(index);
}

void lightResetSlotToDefaults(std::uint32_t lightIndex) {
    ResetLightSlotToDefaults(lightIndex);
}

void lightEnableDefaultSlot(std::uint32_t lightIndex) {
    ResetLightSlotToDefaults(lightIndex);
    SetLightBaseType(lightIndex, 1);
    lightSetSlotEnabled(lightIndex, 1);
}

void lightSetShadowRange(float range) {
    g_inverseShadowRange = 1.0f / range;
    g_shadowRange = range;
}

float lightGetShadowRange() noexcept {
    return g_shadowRange;
}

void lightEnableDirLight(std::uint32_t lightIndex, const Vector3 *direction) {
    lightEnableDirLightInternal(lightIndex, direction);
}
void lightEnablePointLightAt(std::uint32_t lightIndex, const Vector3 *position) {
    ResetLightSlotToDefaults(lightIndex);
    SetLightBaseType(lightIndex, 3);
    SetLightDirectionVectorByIndex(lightIndex, position);
    lightSetSlotEnabled(lightIndex, 1);
}
void lightEnableSpotLight(std::uint32_t lightIndex, const Vector3 *position, const Vector3 *direction) {
    ResetLightSlotToDefaults(lightIndex);
    SetLightBaseType(lightIndex, 4);
    SetLightDirectionVectorByIndex(lightIndex, position);
    SetEffectTransformByIndex(lightIndex, direction);
    lightSetSlotEnabled(lightIndex, 1);
}

void lightSetGlobalSceneLighting(const Vector4 &colour) {
    g_sceneLightingBaseColour = colour;
    g_sceneLightingActiveColour = colour;
}

void SetRGBStruct(std::uint32_t lightIndex, const Vector4 *colour) {
    if (lightIndex >= 128)
        return;
    ShenmueLightSlot &slot = g_LightData->slots[lightIndex];
    slot.colourScale = colour->x;
    slot.colourR = static_cast<double>(colour->y) < 0.00001 ? 0.0f : colour->y;
    slot.colourG = static_cast<double>(colour->z) < 0.00001 ? 0.0f : colour->z;
    slot.colourB = static_cast<double>(colour->w) < 0.00001 ? 0.0f : colour->w;
    if (lightIndex < 32) {
        ShenmueLightSlot &render = g_lightRenderSlots[lightIndex];
        render.colourScale = slot.colourScale;
        render.colourR = slot.colourR;
        render.colourG = slot.colourG;
        render.colourB = slot.colourB;
        EffectLightRenderSlot &effect = g_effectLightRenderSlots[lightIndex];
        effect.colourR = render.colourR;
        effect.colourG = render.colourG;
        effect.colourB = render.colourB;
    }
}

void SetLightIntensity(std::uint32_t lightIndex, float intensity) {
    if (lightIndex >= 128)
        return;
    g_LightData->slots[lightIndex].intensity = intensity;
    if (lightIndex < 32) {
        g_lightRenderSlots[lightIndex].intensity = intensity;
        g_effectLightRenderSlots[lightIndex].intensity = static_cast<double>(std::fabs(intensity)) < 0.0001 ? 0.0f : intensity;
    }
}

void SetLightFalloffRangeByIndex(std::uint32_t lightIndex, float range) {
    if (lightIndex >= 128)
        return;
    const float value = std::max(range, 0.01f);
    ShenmueLightSlot &slot = g_LightData->slots[lightIndex];
    slot.range = value;
    if (lightIndex < 32) {
        g_lightRenderSlots[lightIndex].range = value;
        EffectLightRenderSlot &effect = g_effectLightRenderSlots[lightIndex];
        effect.range = value;
        effect.inverseRange = 1.0f / value;
        effect.rangeSquared = value * value;
    }
    SetLightFalloff(lightIndex, slot.falloff);
}

void SetLightFalloff(std::uint32_t lightIndex, float falloff) {
    const float value = std::max(falloff, 0.01f);
    if (lightIndex >= 128)
        return;
    ShenmueLightSlot &slot = g_LightData->slots[lightIndex];
    slot.falloff = value;
    if (lightIndex < 32) {
        g_lightRenderSlots[lightIndex].falloff = value;
        EffectLightRenderSlot &effect = g_effectLightRenderSlots[lightIndex];
        const float scaled = slot.range * value;
        const std::uint16_t type = static_cast<std::uint16_t>(effect.modeFlags) & 0xfff7U;
        effect.attenuationShape = type == 3U ? scaled : PowEighth(scaled);
    }
}

void SetLightDirectionVectorByIndex(std::uint32_t lightIndex, const Vector3 *position) {
    if (lightIndex >= 128)
        return;
    g_LightData->slots[lightIndex].position = *position;
    if (lightIndex < 32) {
        g_lightRenderSlots[lightIndex].position = *position;
        g_effectLightRenderSlots[lightIndex].position = *position;
    }
}

void SetEffectTransformByIndex(std::uint32_t lightIndex, const Vector3 *direction) {
    if (lightIndex >= 128)
        return;
    g_LightData->slots[lightIndex].direction = *direction;
    if (lightIndex < 32) {
        g_lightRenderSlots[lightIndex].direction = *direction;
        g_effectLightRenderSlots[lightIndex].negativeNormalizedDirection = NormalizeVector({-direction->x, -direction->y, -direction->z});
    }
}

void SetLightAngleByIndex(std::uint32_t lightIndex, std::int32_t angle) {
    if (lightIndex >= 128)
        return;
    g_LightData->slots[lightIndex].coneAngle = angle;
    if (lightIndex < 32) {
        g_lightRenderSlots[lightIndex].coneAngle = angle;
        SetEffectLightCone(g_effectLightRenderSlots[lightIndex], angle);
    }
}

void SetLightTypeOrFlagsByIndex(std::uint32_t lightIndex, std::uint32_t modeFlags) {
    if (!g_LightData || lightIndex >= 128)
        return;
    g_LightData->slots[lightIndex].modeFlags = modeFlags;
    if (lightIndex < 32 && g_lightRenderSlots) {
        g_lightRenderSlots[lightIndex].modeFlags = modeFlags;
        g_effectLightRenderSlots[lightIndex].modeFlags = static_cast<std::int16_t>(modeFlags);
        UpdateEffectLightMasks(lightIndex);
    }
}

void SetLightBaseType(std::uint32_t lightIndex, std::uint32_t type) {
    if (!g_LightData || lightIndex >= 128)
        return;
    const std::uint32_t flags = g_LightData->slots[lightIndex].modeFlags;
    SetLightTypeOrFlagsByIndex(lightIndex, (flags & 8U) ? (type | 8U) : (type & ~8U));
}

void SetLightSpecialFlag(std::uint32_t lightIndex, bool enabled) {
    if (!g_LightData || lightIndex >= 128)
        return;
    std::uint32_t flags = g_LightData->slots[lightIndex].modeFlags;
    flags = enabled ? (flags | 8U) : (flags & ~8U);
    SetLightTypeOrFlagsByIndex(lightIndex, flags);
}

void lightSetGlobalLightAndKey(const float *parameters) {
    if (!parameters || !g_LightData || !g_lightRenderSlots)
        return;
    const float scene = parameters[0];
    g_sceneLightingBaseColour = {scene, scene, scene, scene};
    g_sceneLightingActiveColour = g_sceneLightingBaseColour;
    const Vector3 direction{parameters[1], parameters[2], parameters[3]};
    SetEffectTransformByIndex(0, &direction);
    const Vector4 colour{1.0f, parameters[4], parameters[5], parameters[6]};
    SetRGBStruct(0, &colour);
}

std::int32_t lightSetSlotEnabled(std::uint32_t lightIndex, std::uint32_t enabled) {
    if (!g_LightData || lightIndex >= 128)
        return 3;
    const std::int32_t previous = g_LightData->slots[lightIndex].enabled;
    if (enabled <= 1) {
        g_LightData->slots[lightIndex].enabled = enabled;
        if (lightIndex < 32 && g_lightRenderSlots) {
            g_lightRenderSlots[lightIndex].enabled = enabled;
            g_effectLightRenderSlots[lightIndex].enabled = static_cast<std::int16_t>(enabled);
            UpdateEffectLightMasks(lightIndex);
        }
    }
    return previous;
}



bool lightSetMaskBit(Character *character, std::uint32_t bit, bool enabled) {
    std::uint32_t mask = character ? character->lightMask : 0U;
    bool previous = false;
    if (bit <= 32) {
        const std::uint32_t nativeBit = 1U << (bit & 31U);
        previous = (mask & nativeBit) != 0;
        mask = enabled ? (mask | nativeBit) : (mask & ~nativeBit);
    }
    (character ? character->lightMask : g_defaultLightMask) = mask;
    return previous;
}

void lightSetMask(Character *character, std::uint32_t mask) {
    (character ? character->lightMask : g_defaultLightMask) = mask;
}

void lightSetRadius(Character *character, float radius) {
    (character ? character->lightRadius : g_defaultLightRadius) = radius;
}

void lightSetGlobalShadowEnabled(bool enabled) {
    g_shadowFlag = enabled ? 1 : 0;
}

void lightSetShadowBlackness(float blackness) {
    if (blackness < 0.0f || blackness > 1.0f)
        return;
    g_shadowBlackness = blackness;
    const float strength = static_cast<float>((1.0 - static_cast<double>(blackness)) * 255.0);
    g_shadowStrength = strength >= -2147483648.0f && strength < 2147483648.0f ? static_cast<std::int32_t>(strength) : INT32_MIN;
}



std::array<DirectionalLightShaderParameters, 2> g_directionalLightData{};
std::array<std::array<float, 8>, 32> g_pointLightData{};
std::array<SpotLightShaderParameters, 3> g_spotLightData{};

std::int16_t g_enableOverrideLightParams = 0;

void CopyDirectionalLights(const DirectionalLightShaderParameters *lights, std::int32_t count) {
    for (std::int32_t index = 0; index < count; ++index)
        g_renderParams.directionalLights[index] = lights[index];
}



void CopyPointLights(const std::array<float, 8> *lights, std::int32_t count) {
    for (std::int32_t index = 0; index < count; ++index) {
        std::uint16_t *const packed = reinterpret_cast<std::uint16_t *>(&g_renderParams.pointLights[index]);
        for (std::size_t component = 0; component < lights[index].size(); ++component) {
            const std::uint32_t bits = std::bit_cast<std::uint32_t>(lights[index][component]);
            std::int16_t exponent = static_cast<std::int16_t>(static_cast<std::uint8_t>(bits >> 23U) - 112);
            if (exponent < 0)
                exponent = 0;
            else if (exponent > 31)
                exponent = 31;
            packed[component] = static_cast<std::uint16_t>((exponent << 10) | ((bits >> 16U) & 0x8000U) | ((bits >> 13U) & 0x3ffU));
        }
    }
}

void CopySpotLights(const SpotLightShaderParameters *lights, std::int32_t count) {
    for (std::int32_t index = 0; index < count; ++index)
        g_renderParams.spotLights[index] = lights[index];
}






void PrepareSceneLightingForRendering() {
    const float intensity = g_effectLightRenderSlots[0].intensity < 1.0f ? g_effectLightRenderSlots[0].intensity : 1.0f;
    const float maxScale = g_sceneObjectMaxScale;
    g_modernAmbientColour = {g_sceneLightingActiveColour.y, g_sceneLightingActiveColour.z, g_sceneLightingActiveColour.w};
    g_hasSetupLightFlags = 1;
    const Vector4 ambient{
        g_sceneLightingActiveColour.y * intensity, g_sceneLightingActiveColour.z * intensity, g_sceneLightingActiveColour.w * intensity, 0.0f};

    *g_matrixStack.top.stack = workTransformMatrix;
    ++g_matrixStack.top.info.index;
    if (g_matrixStack.top.info.index < g_matrixStack.top.info.capacity)
        ++g_matrixStack.top.stack;
    Matrix4x4::NormalizeAndSetTransformMatrix();
    Matrix4x4::RotateMatrixCounterClockwise90();
    const Vector4 basisX = workTransformMatrix.rows[0];
    const Vector4 basisY = workTransformMatrix.rows[1];
    const Vector4 basisZ = workTransformMatrix.rows[2];
    Matrix4x4::RotateMatrixCounterClockwise90();
    g_renderParams.constants[14] = basisZ;
    g_renderParams.constants[12] = basisX;
    g_renderParams.constants[13] = basisY;

    std::int32_t directionalCount = 0;
    std::int32_t pointCount = 0;
    std::int32_t spotCount = 0;
    for (std::uint32_t index = 0; index < g_effectLightRenderSlots.size(); ++index) {
        EffectLightRenderSlot &light = g_effectLightRenderSlots[index];
        if (!light.enabled)
            continue;
        switch (light.modeFlags) {
        case 2:
        case 10: {
            if (directionalCount >= 2)
                break;
            Vector3 direction{};
            Matrix4x4::Transform(light.transformedDirection, direction);
            if (maxScale != 1.0f) {
                direction.x /= maxScale;
                direction.y /= maxScale;
                direction.z /= maxScale;
            }
            light.shaderConstants5C[3] = direction.x;
            light.shaderConstants5C[4] = direction.y;
            light.shaderConstants5C[5] = direction.z;
            DirectionalLightShaderParameters &staged = g_directionalLightData[directionalCount++];
            staged.direction.x = direction.x;
            staged.direction.y = direction.y;
            staged.direction.z = direction.z;
            staged.diffuse = {light.shaderConstants04[5], light.shaderConstants04[6], light.shaderConstants04[7], light.shaderConstants04[4]};
            if (index == 0 && staged.diffuse.x > 0.0f && staged.diffuse.y > 0.0f && staged.diffuse.z > 0.0f) {
                staged.diffuse.x = std::pow(staged.diffuse.x, 2.2f);
                staged.diffuse.y = std::pow(staged.diffuse.y, 2.2f);
                staged.diffuse.z = std::pow(staged.diffuse.z, 2.2f);
            }
            break;
        }
        case 3:
        case 11:
            if (pointCount >= 32)
                break;
            g_pointLightData[pointCount++] = {light.transformedPosition.x, light.transformedPosition.y, light.transformedPosition.z, light.attenuationShape,
                light.shaderConstants04[5], light.shaderConstants04[6], light.shaderConstants04[7], light.range};
            break;
        case 4:
        case 12: {
            if (spotCount >= 3)
                break;
            Vector3 direction{};
            Matrix4x4::Transform(light.transformedDirection, direction);
            if (maxScale != 1.0f) {
                direction.x /= maxScale;
                direction.y /= maxScale;
                direction.z /= maxScale;
            }
            light.shaderConstants5C[3] = direction.x;
            light.shaderConstants5C[4] = direction.y;
            light.shaderConstants5C[5] = direction.z;
            Vector3 position{};
            Matrix4x4::TransformPoint(light.transformedPosition, position);
            light.shaderConstants5C[0] = position.x;
            light.shaderConstants5C[1] = position.y;
            light.shaderConstants5C[2] = position.z;
            SpotLightShaderParameters &staged = g_spotLightData[spotCount++];
            staged.direction.x = direction.x;
            staged.direction.y = direction.y;
            staged.direction.z = direction.z;
            staged.position.x = position.x;
            staged.position.y = position.y;
            staged.position.z = position.z;
            staged.diffuse = {light.shaderConstants04[5], light.shaderConstants04[6], light.shaderConstants04[7], light.shaderConstants04[4]};
            staged.attenuation = {light.attenuationShape, light.range, light.reserved50[1], 8.0f};
            staged.cutoff.x = light.coneCos;
            staged.cutoff.y = light.inverseOneMinusConeCos;
            break;
        }
        default:
            break;
        }
    }
    std::int16_t popCount = g_matrixStack.top.info.index < 1 ? g_matrixStack.top.info.index : 1;
    std::int16_t pointerCount = popCount;
    if (g_matrixStack.top.info.index > g_matrixStack.top.info.capacity)
        pointerCount = static_cast<std::int16_t>(popCount + g_matrixStack.top.info.capacity - g_matrixStack.top.info.index);
    g_matrixStack.top.info.index = static_cast<std::int16_t>(g_matrixStack.top.info.index - popCount);
    g_matrixStack.top.stack -= pointerCount;
    workTransformMatrix = *g_matrixStack.top.stack;

    g_renderParams.ambientLight = ambient;
    CopyDirectionalLights(g_directionalLightData.data(), directionalCount);
    CopyPointLights(g_pointLightData.data(), pointCount);
    CopySpotLights(g_spotLightData.data(), spotCount);
    g_currentVertexShaderSet = g_litVertexShaderSet.data();
    g_currentInputLayoutSet = g_litInputLayoutSet.data();
    g_renderParams.lightCount = {static_cast<float>(directionalCount), static_cast<float>(pointCount), static_cast<float>(spotCount), ambient.w};
    if (g_enableOverrideLightParams) {
        const EffectLightRenderSlot &light = g_effectLightRenderSlots[15];
        g_renderParams.constants[8] = {light.shaderConstants5C[3], light.shaderConstants5C[4], light.shaderConstants5C[5], 0.0f};
    }
}

namespace {

#pragma pack(push, 1)
struct PointLightScriptData {
    Vector4 colour;
    float intensity;
    float range;
    float falloff;
    std::uint32_t reserved_1c;
    Vector3 position;
};

struct DirectionalLightScriptData {
    Vector3 colour;
    float intensity;
    Vector3 direction;
};

struct PointLightUtilsScriptData {
    Vector3 colour;
    float intensity;
    Vector3 position;
    float range;
    float falloff;
};

struct SpotLightScriptData {
    Vector3 colour;
    float intensity;
    Vector3 direction;
    Vector3 position;
    std::uint32_t reserved_28;
    float falloff;
    std::int32_t angle;
};
#pragma pack(pop)

static_assert(sizeof(PointLightScriptData) == 0x2c);
static_assert(sizeof(DirectionalLightScriptData) == 0x1c);
static_assert(sizeof(PointLightUtilsScriptData) == 0x24);
static_assert(sizeof(SpotLightScriptData) == 0x34);


std::int16_t g_effectLightSmoothedPitch = 0;
std::int16_t g_effectLightSmoothedYaw = 0;

std::byte *LightTaskPointer(std::int32_t offset) {
    return offset && g_pCurrentTaskData ? g_pCurrentTaskData + offset : nullptr;
}

Character *CurrentRyo() {
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    return control ? static_cast<Character *>(control->ryo) : nullptr;
}

std::int16_t SmoothEffectLightAngle(std::int16_t state, std::int16_t target) {
    const std::int16_t delta = static_cast<std::int16_t>(target - state);
    if (delta < -0x2000)
        state = static_cast<std::int16_t>(state + delta + 0x2000);
    else if (delta > 0x2000)
        state = static_cast<std::int16_t>(state + delta - 0x2000);
    return static_cast<std::int16_t>(state + static_cast<std::int16_t>(target - state) / 4);
}

float Vector3_NormalizeInPlace_ReturnLength(Vector3 &value) {
    const float x = value.x;
    const float y = value.y;
    const float z = value.z;
    const float lengthSquared = x * x + y * y + z * z;
    if (static_cast<double>(lengthSquared) <= 0.000001)
        return 0.0f;
    const float length = std::sqrt(lengthSquared);
    value = {x / length, y / length, z / length};
    return length;
}

void SetScriptLightColour(std::uint32_t lightIndex, const Vector3 &colour) {
    const Vector4 nativeColour{1.0f, colour.x, colour.y, colour.z};
    SetRGBStruct(lightIndex, &nativeColour);
}

} // namespace

HLib::Task *LGHT_InitializeTask() {
    if (g_lghtTask)
        return g_lghtTask;
    g_lghtTask = HLib_EnqueueTaskWithoutParameter(LGHT_TaskCallback, 3, 2, 0x5448474c);
    return g_lghtTask;
}

void LGHT_TaskCallback(HLib::TaskData *) {
    LGHT_Update();
}

void Script_lightCreatePoint(ScenarioContext *, void *arguments) {
    const auto *const params = static_cast<const std::uint32_t *>(arguments);
    std::byte *const bytes = LightTaskPointer(params[1]);

    PointLightScriptData data{};
    std::memcpy(&data, bytes, sizeof(data));
    lightSetGlobalSceneLighting({data.colour.x, data.colour.x, data.colour.x, data.colour.x});
    data.colour.x = 1.0f;
    std::memcpy(bytes, &data.colour.x, sizeof(data.colour.x));
    lightEnablePointLightAt(params[0], &data.position);
    SetRGBStruct(params[0], &data.colour);
    SetLightIntensity(params[0], data.intensity);
    SetLightFalloffRangeByIndex(params[0], data.range);
    SetLightFalloff(params[0], data.falloff);
}

void Script_lightUtils(ScenarioContext *, void *arguments) {
    const auto *const params = static_cast<const std::uint32_t *>(arguments);
    const std::uint32_t lightIndex = params[0];
    const std::uint32_t mode = params[1];
    lightResetSlotToDefaults(lightIndex);
    if (lightIndex < 128U && static_cast<std::int32_t>(mode) < 5) {
        SetLightBaseType(lightIndex, mode);
    }

    const std::byte *const bytes = LightTaskPointer(params[2]);
    if (mode == 2) {
        DirectionalLightScriptData data{};
        std::memcpy(&data, bytes, sizeof(data));
        SetScriptLightColour(lightIndex, data.colour);
        SetLightIntensity(lightIndex, data.intensity);
        SetEffectTransformByIndex(lightIndex, &data.direction);
    } else if (mode == 3) {
        PointLightUtilsScriptData data{};
        std::memcpy(&data, bytes, sizeof(data));
        SetScriptLightColour(lightIndex, data.colour);
        SetLightIntensity(lightIndex, data.intensity);
        SetLightDirectionVectorByIndex(lightIndex, &data.position);
        SetLightFalloffRangeByIndex(lightIndex, data.range);
        SetLightFalloff(lightIndex, data.falloff);
    } else if (mode == 4) {
        SpotLightScriptData data{};
        std::memcpy(&data, bytes, sizeof(data));
        SetScriptLightColour(lightIndex, data.colour);
        SetLightIntensity(lightIndex, data.intensity);
        SetEffectTransformByIndex(lightIndex, &data.direction);
        SetLightDirectionVectorByIndex(lightIndex, &data.position);
        SetLightFalloff(lightIndex, data.falloff);
        SetLightAngleByIndex(lightIndex, data.angle);
    }
    lightSetSlotEnabled(lightIndex, 1);
}

void UpdateEffectLightFromCameraAndBones(std::int32_t lightIndex) {
    Character *const ryo = CurrentRyo();
    if (!ryo || g_playerStats.contextActionMode != 3)
        return;
    float stick = 0.0f;
    if (ShouldRun() && g_pControlPadController->controlMode == 2 && useAnalogInput)
        stick = static_cast<float>(rawAnalogInputValue) / 245.0f;
    const Vector3 eye = g_CameraEyePos;
    const Vector3 target = g_CameraTargetPos;
    Vector3 foot{};
    Vector3 head{};
    mtwkGetCharaBonePosition(ryo, 0x0c, &foot);
    mtwkGetCharaBonePosition(ryo, 5, &head);
    const double poseBlend = 1.0 - static_cast<double>(stick);
    Vector3 position{
        static_cast<float>(static_cast<double>(foot.x) * poseBlend + static_cast<double>(head.x * stick)),
        static_cast<float>(static_cast<double>(foot.y) * poseBlend + static_cast<double>(head.y * stick)),
        static_cast<float>(static_cast<double>(foot.z) * poseBlend + static_cast<double>(head.z * stick)),
    };
    const Vector3 cameraVector{eye.x - target.x, eye.y - target.y, eye.z - target.z};

    std::int16_t pitch = 0;
    std::int16_t yaw = 0;
    const CameraInterpolatedOrbit &lookAround = g_cam_target_orbit_state;
    if (PlayerEffectLightUsesAccumulatedYaw()) {
        yaw = static_cast<std::int16_t>(-static_cast<std::uint16_t>(g_fCamAccumYaw));
        const std::int16_t facing = static_cast<std::int16_t>(ReturnCurrentFixed16Angle());
        const std::int16_t delta = static_cast<std::int16_t>(yaw - facing);
        if (delta > 0x4000)
            yaw = static_cast<std::int16_t>(facing + 0x4000);
        else if (delta < -0x4000)
            yaw = static_cast<std::int16_t>(facing - 0x4000);
        if (g_cameraStateFlags & 8U)
            pitch = static_cast<std::int16_t>(static_cast<std::int32_t>(lookAround.orbit.w));
    } else if (g_cameraStateFlags & 8U) {
        pitch = static_cast<std::int16_t>(static_cast<std::int32_t>(lookAround.orbit.w));
        yaw = static_cast<std::int16_t>(ReturnCurrentFixed16Angle() + static_cast<std::int32_t>(lookAround.orbit.z));
    } else {
        yaw = static_cast<std::int16_t>(ReturnCurrentFixed16Angle());
    }
    g_effectLightSmoothedPitch = SmoothEffectLightAngle(g_effectLightSmoothedPitch, pitch);
    g_effectLightSmoothedYaw = SmoothEffectLightAngle(g_effectLightSmoothedYaw, yaw);

    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(g_effectLightSmoothedYaw));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(g_effectLightSmoothedPitch));
    const Matrix4x4 &m = workTransformMatrix;
    const Vector3 forward{
        m.rows[0].x * 0.0f + m.rows[1].x * 0.0f - m.rows[2].x * 1.0f + m.rows[3].x,
        m.rows[0].y * 0.0f + m.rows[1].y * 0.0f - m.rows[2].y * 1.0f + m.rows[3].y,
        m.rows[0].z * 0.0f + m.rows[1].z * 0.0f - m.rows[2].z * 1.0f + m.rows[3].z,
    };
    Matrix4x4::Pop(1);
    Vector3 direction{
        static_cast<float>(static_cast<double>(-forward.x) * poseBlend + static_cast<double>(cameraVector.x * stick)),
        static_cast<float>(static_cast<double>(-forward.y) * poseBlend + static_cast<double>(cameraVector.y * stick)),
        static_cast<float>(static_cast<double>(-forward.z) * poseBlend + static_cast<double>(cameraVector.z * stick)),
    };
    SetLightAngleByIndex(lightIndex, 0x1300);
    if (Vector3_NormalizeInPlace_ReturnLength(direction) == 0.0f)
        direction = {1.0f, 0.0f, 0.0f};
    if (0.5f > g_ryoRawMovementInputSpeed) {
        const float offset = static_cast<float>(0.5 - static_cast<double>(g_ryoRawMovementInputSpeed));
        position = {position.x + offset * direction.x, position.y + offset * direction.y, position.z + offset * direction.z};
    }
    SetLightDirectionVectorByIndex(lightIndex, &position);
    SetEffectTransformByIndex(lightIndex, &direction);
}

void Script_GetCameraInputDetails(ScenarioContext *, void *arguments) {
    UpdateEffectLightFromCameraAndBones(static_cast<std::int32_t>(*static_cast<const std::uint32_t *>(arguments)));
}

} // namespace shenmue::s1
