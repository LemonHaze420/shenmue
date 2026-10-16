#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>
#include <array>

namespace shenmue::s1 {
struct Character;

#pragma pack(push, 1)
struct ShenmueLightSlot {
    std::int32_t enabled;
    std::uint32_t modeFlags;
    float colourScale;
    float colourR;
    float colourG;
    float colourB;
    float intensity;
    float range;
    float falloff;
    std::uint32_t reserved_24;
    Vector3 position;
    Vector3 direction;
    std::int32_t coneAngle;
};

struct EffectLightRenderSlot {
    std::int16_t enabled;
    std::int16_t modeFlags;
    float shaderConstants04[9];
    float colourR;
    float colourG;
    float colourB;
    Vector3 position;
    Vector3 negativeNormalizedDirection;
    float attenuationShape;
    float reserved50[2];
    float coneCos;
    Vector3 transformedPosition;
    Vector3 transformedDirection;
    float shaderConstants5C[9];
    float inverseRange;
    std::uint32_t packedConstant9C;
    float rangeSquared;
    float inverseOneMinusConeCos;
    float range;
    float coneSin;
    std::uint32_t reservedB0;
    float intensity;
};
#pragma pack(pop)

struct LightStorage {
    ShenmueLightSlot slots[128];
    ShenmueLightSlot renderSlots[32];
    std::byte reserved_2a80[0x800];
};

struct LightDateTime {
    std::int32_t month = 1;
    std::int32_t day = 1;
    std::int32_t hour = 12;
    std::int32_t minutes = 0;
    std::int32_t seconds = 0;
};

Vector3 UpdateSunForDateTime(const LightDateTime &dateTime, float tiltAngle, std::int32_t clampAngle);

static_assert(sizeof(ShenmueLightSlot) == 0x44);
static_assert(offsetof(ShenmueLightSlot, position) == 0x28);
static_assert(offsetof(ShenmueLightSlot, direction) == 0x34);
static_assert(sizeof(EffectLightRenderSlot) == 0xb8);
static_assert(offsetof(EffectLightRenderSlot, colourR) == 0x28);
static_assert(offsetof(EffectLightRenderSlot, negativeNormalizedDirection) == 0x40);
static_assert(offsetof(EffectLightRenderSlot, intensity) == 0xb4);
static_assert(sizeof(LightStorage) == 0x3280);

extern std::array<EffectLightRenderSlot, 32> g_effectLightRenderSlots;
extern std::uint32_t g_effectEnabledSpotLightMask;
extern std::uint32_t g_effectEnabledStandardLightMask;
extern std::uint32_t g_defaultLightMask;
extern float g_defaultLightRadius;
extern std::int32_t g_shadowFlag;
extern std::int32_t g_hasSetupLightFlags;
extern float g_shadowBlackness;
extern std::int32_t g_shadowStrength;

extern float g_inverseShadowRange;

extern std::int32_t g_prevCachedGlobalLightingAdjustFlag;
extern Vector4 g_sceneLightingActiveColour;
extern float g_sceneObjectMaxScale;
extern Vector4 g_sceneLightingBaseColour;
extern std::int32_t g_cachedGlobalLightingAdjustFlag;
extern std::int32_t g_effectsNeedInitialization;
extern LightStorage *g_lightDataStatics;
extern LightStorage *g_LightData;
extern ShenmueLightSlot *g_lightRenderSlots;
const ShenmueLightSlot *LGHT_GetDataSlot(std::uint32_t lightIndex);
std::int32_t BuildActiveSpotLightRenderParams(EffectLightRenderSlot *lights);
const ShenmueLightSlot *LGHT_GetRenderSlot(std::uint32_t lightIndex);
std::uint32_t LGHT_GetShadowSourceIndex(std::uint32_t rank) noexcept;
void LGHT_SetRenderIntensity(std::uint32_t lightIndex, float intensity);

void shenSetLightingMode(std::int32_t index);
void lightEnableDirLight(std::uint32_t lightIndex, const Vector3 *direction);
void lightEnablePointLightAt(std::uint32_t lightIndex, const Vector3 *position);
void lightEnableSpotLight(std::uint32_t lightIndex, const Vector3 *position, const Vector3 *direction);
void lightSetGlobalSceneLighting(const Vector4 &colour);
void SetRGBStruct(std::uint32_t lightIndex, const Vector4 *colour);
void SetLightIntensity(std::uint32_t lightIndex, float intensity);
void SetLightFalloffRangeByIndex(std::uint32_t lightIndex, float range);
void SetLightFalloff(std::uint32_t lightIndex, float falloff);
void SetLightDirectionVectorByIndex(std::uint32_t lightIndex, const Vector3 *position);
void SetEffectTransformByIndex(std::uint32_t lightIndex, const Vector3 *direction);
void SetLightAngleByIndex(std::uint32_t lightIndex, std::int32_t angle);
void SetLightTypeOrFlagsByIndex(std::uint32_t lightIndex, std::uint32_t modeFlags);
void SetLightBaseType(std::uint32_t lightIndex, std::uint32_t type);
void SetLightSpecialFlag(std::uint32_t lightIndex, bool enabled);
void lightSetGlobalLightAndKey(const float *parameters);
std::int32_t lightSetSlotEnabled(std::uint32_t lightIndex, std::uint32_t enabled);
void lightResetSlotToDefaults(std::uint32_t lightIndex);
void lightEnableDefaultSlot(std::uint32_t lightIndex);
void lightSetShadowRange(float range);
float lightGetShadowRange() noexcept;
bool lightSetMaskBit(Character *character, std::uint32_t bit, bool enabled);
void lightSetMask(Character *character, std::uint32_t mask);
void lightSetRadius(Character *character, float radius);
void lightSetGlobalShadowEnabled(bool enabled);
void lightSetShadowBlackness(float blackness);
void PrepareSceneLightingForRendering();
void LGHT_Initialize(const void *mapInfo, std::uint32_t mapInfoSize);
void LGHT_Parse(std::uint32_t index);
void LGHT_Update();
void LGHT_SetLightIntensityRadius(float radius) noexcept;
void LGHT_SetDateTime(const LightDateTime &dateTime);
void LGHT_ResetRuntime();
std::int32_t LGHT_GetMode();

bool GetLightPosition(std::uint32_t lightIndex, Vector3 *output);

} // namespace shenmue::s1
