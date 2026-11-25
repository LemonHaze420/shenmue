#pragma once
#include "shenmue.h"

#include "common/math.hpp"
#include "common/types.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SHENMUE_LIGHT_ARRAY(type, name, count) type name[count]
#define SHENMUE_LIGHT_DEFAULT
#else
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#define SHENMUE_LIGHT_ARRAY(type, name, count) std::array<type, count> name
#define SHENMUE_LIGHT_DEFAULT                                                                                                                                  \
    {                                                                                                                                                          \
    }
#endif

namespace shenmue {
namespace s2 {
struct MT7Node;
struct MT7Model;
struct Character;
struct MagicRoomEntityLight;
struct DateTime;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
using ::shenmue::shen_name;
#define TASK_LGTP shen_name(SHEN_NAME4('L', 'G', 'T', 'P'))
#define CHUNK_LGHT shen_name(SHEN_NAME4('L', 'G', 'H', 'T'))
#define WEATHER_LENS_PACKAGE shen_name(SHEN_NAME4('L', 'N', 'S', 'W'))
#define WEATHER_LENS_MODEL shen_name(SHEN_NAME4('L', 'E', 'N', 'S'))
#else
inline constexpr shen_name TASK_LGTP{SHEN_NAME(LGTP)};
inline constexpr shen_name CHUNK_LGHT{SHEN_NAME(LGHT)};
inline constexpr shen_name WEATHER_LENS_PACKAGE{SHEN_NAME(LNSW)}, WEATHER_LENS_MODEL{SHEN_NAME(LENS)};
#endif
struct WeatherLensMaterialParameter {
    std::uint32_t model, parameter;
};
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SHENMUE_LENS_PARAMETERS static const WeatherLensMaterialParameter WEATHER_LENS_MATERIAL_PARAMETERS[11] = {
#define SHENMUE_LENS_PARAMETERS_END }
#else
#define SHENMUE_LENS_PARAMETERS                                                                                                                                \
    inline constexpr std::array<WeatherLensMaterialParameter, 11> WEATHER_LENS_MATERIAL_PARAMETERS {                                                           \
        {
#define SHENMUE_LENS_PARAMETERS_END                                                                                                                            \
    }                                                                                                                                                          \
    }
#endif
SHENMUE_LENS_PARAMETERS{11, 1157240422}, {0, 1157238784}, {1, 1157238784}, {2, 1157238784}, {4, 1157235507}, {5, 0x44fa1000}, {6, 1157238784}, {7, 1157238784},
    {8, 1157238784}, {9, 1157237146}, {10, 1157237146} SHENMUE_LENS_PARAMETERS_END;
#undef SHENMUE_LENS_PARAMETERS
#undef SHENMUE_LENS_PARAMETERS_END

struct PackedLightDefinitionHeader {
    shen_name name;
    std::uint32_t byteSize;
    std::uint32_t reserved;
    std::int16_t lightCount;
    std::int16_t mode;
    SHENMUE_LIGHT_ARRAY(std::int16_t, colour, 4);
};
struct PackedLightDefinitionRecord {
    std::int16_t index;
    std::int16_t enabled;
    std::int16_t flags;
    SHENMUE_LIGHT_ARRAY(std::int16_t, parameters, 12);
    std::int16_t coneAngle;
};
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char sizeofPackedLightDefinitionHeaderCheck[sizeof(PackedLightDefinitionHeader) == 0x18 ? 1 : -1];
typedef char offsetofPackedLightDefinitionHeadermodeCheck[offsetof(PackedLightDefinitionHeader, mode) == 0x0e ? 1 : -1];
typedef char sizeofPackedLightDefinitionRecordCheck[sizeof(PackedLightDefinitionRecord) == 0x20 ? 1 : -1];
typedef char offsetofPackedLightDefinitionRecordparametersCheck[offsetof(PackedLightDefinitionRecord, parameters) == 6 ? 1 : -1];
#else
static_assert(sizeof(PackedLightDefinitionHeader) == 0x18);
static_assert(offsetof(PackedLightDefinitionHeader, mode) == 0x0e);
static_assert(sizeof(PackedLightDefinitionRecord) == 0x20);
static_assert(offsetof(PackedLightDefinitionRecord, parameters) == 6);
#endif

struct LightDefinitionEntry {
    const std::byte *block SHENMUE_LIGHT_DEFAULT;
    std::uint32_t identifier SHENMUE_LIGHT_DEFAULT;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    LightDefinitionEntry() : block(0), identifier(0) {
    }
#endif
};
#if SHENMUE_IS_PC
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char sizeofLightDefinitionEntryCheck[sizeof(LightDefinitionEntry) == 0x10 ? 1 : -1];
typedef char offsetofLightDefinitionEntryidentifierCheck[offsetof(LightDefinitionEntry, identifier) == 0x08 ? 1 : -1];
#else
static_assert(sizeof(LightDefinitionEntry) == 0x10);
static_assert(offsetof(LightDefinitionEntry, identifier) == 0x08);
#endif
#else
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char sizeofLightDefinitionEntryCheck[sizeof(LightDefinitionEntry) == 0x08 ? 1 : -1];
typedef char offsetofLightDefinitionEntryidentifierCheck[offsetof(LightDefinitionEntry, identifier) == 0x04 ? 1 : -1];
#else
static_assert(sizeof(LightDefinitionEntry) == 0x08);
static_assert(offsetof(LightDefinitionEntry, identifier) == 0x04);
#endif
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LightTask_Enqueue();
#else
std::int64_t LightTask_Enqueue();
#endif
void ImmediateLightWork(void *);
void AssignMagicRoomLighting(const Vector3 &origin, float radius);
void AssignLightParametersForMagicRoomEntity(std::uint32_t index, const MagicRoomEntityLight &light);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LGHT_Parser_Internal(std::uint32_t identifier);
#else
void LGHT_Parser_Internal(std::uint64_t identifier);
#endif
// Native RegisterLightData: a package LGHT record takes the first free registry entry (of 24). The size bounds the
// port's reads of the block.
bool RegisterLightData(const void *block, std::int32_t identifier, std::size_t size);
// Native UnregisterLightData (PACK retirement): the first entry with the identifier is emptied.
void UnregisterLightData(std::int32_t identifier);
std::int32_t GetCurrentActiveLight();
void SetCurrentActiveLight(std::int32_t mode);
void SetupEffectTransforms();
void DrawPreparedVertexColours(const MT7Model *model, const MT7Node *node, float alpha);
void ResetLightEntry(std::uint32_t slot);
void SetLightKind(std::uint32_t slot, std::int32_t kind);
void SetSelectedLightDirection(std::uint32_t slot, const Vector3 &direction);
void shenSetLightColour(std::uint32_t slot, const Vector4 &colour);
void ReinitializeLights(std::int32_t mode);
void SetLightEntryIndex(std::uint32_t index, std::uint32_t value);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetLightEntryIndex(std::uint32_t index);
void SetLightConeAngle(std::uint32_t index, std::int32_t angle);
#else
std::uint64_t GetLightEntryIndex(std::uint32_t index);
void SetLightConeAngle(std::uint32_t index, std::int16_t angle);
#endif
void SetLightFlagForIndex(std::uint32_t index, std::int32_t value);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetLightRootPosition(const Vector3 &position);
#else
std::uint64_t SetLightRootPosition(const Vector3 &position);
#endif
void TranslateLightDefinition(std::uint32_t identifier, const Vector3 &translation);
void PrioritizeLight(std::int16_t index);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetRankedLightLimit(std::uint32_t count);
#else
std::uint64_t SetRankedLightLimit(std::uint32_t count);
#endif
void ConfigureDynamicLight(std::uint32_t slot, std::uint8_t kind, const Vector4 &colour, float scale);
void SetDynamicLightPosition(std::uint32_t slot, const Vector3 &position);
void SetDynamicLightEnabled(std::uint32_t slot, bool enabled);
void ApplyWeatherLightState(std::uint32_t index, const Vector4 *colour, const float *sceneIntensity, const Vector3 *sunDirection);
void ApplyWeatherGlobalLighting(float value);
void SetWeatherLighting(float value);
float GetWeatherLighting();
struct WeatherSunState {
    float jitterCountdown SHENMUE_LIGHT_DEFAULT;
    float sunlightIntensity SHENMUE_LIGHT_DEFAULT;
    Vector3 direction SHENMUE_LIGHT_DEFAULT;
    float diskScale SHENMUE_LIGHT_DEFAULT;
    float jitterScale SHENMUE_LIGHT_DEFAULT;
    std::int32_t enabled SHENMUE_LIGHT_DEFAULT;
    std::int32_t explicitDirection SHENMUE_LIGHT_DEFAULT;
    std::int32_t control SHENMUE_LIGHT_DEFAULT;
    float sourceIntensity SHENMUE_LIGHT_DEFAULT;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    WeatherSunState()
        : jitterCountdown(0.0f), sunlightIntensity(0.0f), diskScale(0.0f), jitterScale(0.0f), enabled(0), explicitDirection(0), control(0),
          sourceIntensity(0.0f) {
        direction.x = direction.y = direction.z = 0.0f;
    }
#endif
};
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char sizeofWeatherSunStateCheck[sizeof(WeatherSunState) == 0x2c ? 1 : -1];
typedef char offsetofWeatherSunStatedirectionCheck[offsetof(WeatherSunState, direction) == 0x08 ? 1 : -1];
typedef char offsetofWeatherSunStatediskScaleCheck[offsetof(WeatherSunState, diskScale) == 0x14 ? 1 : -1];
typedef char offsetofWeatherSunStatejitterScaleCheck[offsetof(WeatherSunState, jitterScale) == 0x18 ? 1 : -1];
typedef char offsetofWeatherSunStateenabledCheck[offsetof(WeatherSunState, enabled) == 0x1c ? 1 : -1];
typedef char offsetofWeatherSunStateexplicitDirectionCheck[offsetof(WeatherSunState, explicitDirection) == 0x20 ? 1 : -1];
typedef char offsetofWeatherSunStatecontrolCheck[offsetof(WeatherSunState, control) == 0x24 ? 1 : -1];
typedef char offsetofWeatherSunStatesourceIntensityCheck[offsetof(WeatherSunState, sourceIntensity) == 0x28 ? 1 : -1];
#else
static_assert(sizeof(WeatherSunState) == 0x2c);
static_assert(offsetof(WeatherSunState, direction) == 0x08);
static_assert(offsetof(WeatherSunState, diskScale) == 0x14);
static_assert(offsetof(WeatherSunState, jitterScale) == 0x18);
static_assert(offsetof(WeatherSunState, enabled) == 0x1c);
static_assert(offsetof(WeatherSunState, explicitDirection) == 0x20);
static_assert(offsetof(WeatherSunState, control) == 0x24);
static_assert(offsetof(WeatherSunState, sourceIntensity) == 0x28);
#endif
extern WeatherSunState g_weatherSunState;
extern Vector3 &g_sunDirectionVector;
extern float &g_flareIntensity;
extern std::int32_t &g_hasComputedSun;
void UpdateSunForDateTime(const DateTime &date, float latitudeTilt, Vector3 &direction, float intensityScale);
std::int32_t SetWeatherSunControl(std::int32_t control);
void LoadLensflare();
void shenSetLensflare();
void shenLoadSetLensflare();
void ReleaseLensflareHostResources();
void PerformWeatherRenderPass();
void SetSunDirectionVector(const Vector3 *direction);
void SetWeatherSceneLighting(float value);
float GetWeatherSceneLighting();
// Runtime-table setters are independent of the sixteen renderer-selected lights.
// Incidental register/pointer returns of the native void setters are not API results.
void ResetLightRuntimeEntry(std::uint32_t index);
void UpdateGlobalLighting(const Vector4 &colour);
void GetGlobalLighting(Vector4 *colour);
void ExchangeRetainedGlobalLighting(const Vector4 &colour, Vector4 *saved);
void SetLightRuntimeColour(std::uint32_t index, const Vector4 &colour);
void SetLightSceneIntensity(std::uint32_t index, float intensity);
void SetLightRange(std::uint32_t index, float range);
void SetLightFalloff(std::uint32_t index, const float *falloff);
void SetLightRuntimePosition(std::uint32_t index, const Vector3 &position);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetLightSunDirection(std::uint32_t index, const Vector3 &direction);
#else
std::uint64_t SetLightSunDirection(std::uint32_t index, const Vector3 &direction);
#endif
void SetLightGeometryEnabled(std::uint32_t index, std::int32_t enabled);
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
struct TemporaryGlobalLightingState {
    Vector4 colour{};
    bool engaged{};
};

bool BeginTemporaryGlobalLighting(const Vector4 &colour, TemporaryGlobalLightingState &saved);
void EndTemporaryGlobalLighting(TemporaryGlobalLightingState &saved);

struct TemporaryMapWeatherLightingState {
    std::int32_t activeLight{};
    Vector4 globalLighting{};
    float sceneIntensity{};
    bool engaged{};
};

bool BeginTemporaryMapWeatherLighting(TemporaryMapWeatherLightingState &saved);
void EndTemporaryMapWeatherLighting(TemporaryMapWeatherLightingState &saved);

#endif

// Colour A/R/G/B occupies +4..+0x13; intensity/range/falloff occupies
// +0x14..+0x1f, across the native vector aliases below.
struct LightRuntimeEntry {
    SHENMUE_LIGHT_ARRAY(std::uint8_t, indexes, 2) SHENMUE_LIGHT_DEFAULT;
    std::int16_t unknown02 SHENMUE_LIGHT_DEFAULT;
    SHENMUE_LIGHT_ARRAY(Vector3, vectors, 2) SHENMUE_LIGHT_DEFAULT;
    float value1c SHENMUE_LIGHT_DEFAULT;
    Vector3 vector20 SHENMUE_LIGHT_DEFAULT;
    SHENMUE_LIGHT_ARRAY(float, values2c, 2) SHENMUE_LIGHT_DEFAULT;
    float value34 SHENMUE_LIGHT_DEFAULT;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    LightRuntimeEntry() : unknown02(0), value1c(0.0f), value34(0.0f) {
        indexes[0] = indexes[1] = 0;
        for (unsigned index = 0; index < 2; ++index)
            vectors[index].x = vectors[index].y = vectors[index].z = 0.0f;
        vector20.x = vector20.y = vector20.z = 0.0f;
        values2c[0] = values2c[1] = 0.0f;
    }
    LightRuntimeEntry(std::uint8_t index0, std::uint8_t index1, std::int16_t field02, float vector0x, float vector0y, float vector0z, float vector1x,
        float vector1y, float vector1z, float field1c, float vector20x, float vector20y, float vector20z, float value2c, float value30, float field34)
        : unknown02(field02), value1c(field1c), value34(field34) {
        indexes[0] = index0;
        indexes[1] = index1;
        vectors[0].x = vector0x;
        vectors[0].y = vector0y;
        vectors[0].z = vector0z;
        vectors[1].x = vector1x;
        vectors[1].y = vector1y;
        vectors[1].z = vector1z;
        vector20.x = vector20x;
        vector20.y = vector20y;
        vector20.z = vector20z;
        values2c[0] = value2c;
        values2c[1] = value30;
    }
#endif
};

#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char sizeofLightRuntimeEntryCheck[sizeof(LightRuntimeEntry) == 0x38 ? 1 : -1];
typedef char offsetofLightRuntimeEntryvectorsCheck[offsetof(LightRuntimeEntry, vectors) == 0x04 ? 1 : -1];
typedef char offsetofLightRuntimeEntryvalue1cCheck[offsetof(LightRuntimeEntry, value1c) == 0x1c ? 1 : -1];
typedef char offsetofLightRuntimeEntryvector20Check[offsetof(LightRuntimeEntry, vector20) == 0x20 ? 1 : -1];
typedef char offsetofLightRuntimeEntryvalues2cCheck[offsetof(LightRuntimeEntry, values2c) == 0x2c ? 1 : -1];
typedef char offsetofLightRuntimeEntryunknown02Check[offsetof(LightRuntimeEntry, unknown02) == 0x02 ? 1 : -1];
typedef char offsetofLightRuntimeEntryvalue34Check[offsetof(LightRuntimeEntry, value34) == 0x34 ? 1 : -1];
#else
static_assert(sizeof(LightRuntimeEntry) == 0x38);
static_assert(offsetof(LightRuntimeEntry, vectors) == 0x04);
static_assert(offsetof(LightRuntimeEntry, value1c) == 0x1c);
static_assert(offsetof(LightRuntimeEntry, vector20) == 0x20);
static_assert(offsetof(LightRuntimeEntry, values2c) == 0x2c);
static_assert(offsetof(LightRuntimeEntry, unknown02) == 0x02);
static_assert(offsetof(LightRuntimeEntry, value34) == 0x34);
#endif

struct LegacyLightDefinitionHeader {
    std::int32_t mode;
    Vector4 colour;
    LightRuntimeEntry primary;
};
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char sizeofLegacyLightDefinitionHeaderCheck[sizeof(LegacyLightDefinitionHeader) == 0x4c ? 1 : -1];
typedef char offsetofLegacyLightDefinitionHeaderprimaryCheck[offsetof(LegacyLightDefinitionHeader, primary) == 0x14 ? 1 : -1];
#else
static_assert(sizeof(LegacyLightDefinitionHeader) == 0x4c);
static_assert(offsetof(LegacyLightDefinitionHeader, primary) == 0x14);
#endif
// Native character DrawLights consumes the full CPU candidate list, which is
// deliberately independent of the 16 shader-selected indices.
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::span<const std::int16_t> GetCharacterLightCandidates();
#endif
const LightRuntimeEntry *GetLightRuntimeEntry(std::uint32_t index);
void CopyLightRuntimeEntry(std::uint32_t destination, std::uint32_t source);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void GetLightRootPosition(Vector3 *position);
#else
std::uint64_t GetLightRootPosition(Vector3 *position);
#endif
std::int32_t GetPreviousActiveLight();

struct MapLightingPlacement {
    std::uint32_t enabled;
    std::uint32_t mapSlot;
    Vector3 position;
    std::int32_t rotationY;
    std::uint32_t active;
};
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char sizeofMapLightingPlacementCheck[sizeof(MapLightingPlacement) == 0x1c ? 1 : -1];
typedef char offsetofMapLightingPlacementmapSlotCheck[offsetof(MapLightingPlacement, mapSlot) == 4 ? 1 : -1];
typedef char offsetofMapLightingPlacementpositionCheck[offsetof(MapLightingPlacement, position) == 8 ? 1 : -1];
typedef char offsetofMapLightingPlacementrotationYCheck[offsetof(MapLightingPlacement, rotationY) == 0x14 ? 1 : -1];
typedef char offsetofMapLightingPlacementactiveCheck[offsetof(MapLightingPlacement, active) == 0x18 ? 1 : -1];
#else
static_assert(sizeof(MapLightingPlacement) == 0x1c);
static_assert(offsetof(MapLightingPlacement, mapSlot) == 4);
static_assert(offsetof(MapLightingPlacement, position) == 8);
static_assert(offsetof(MapLightingPlacement, rotationY) == 0x14);
static_assert(offsetof(MapLightingPlacement, active) == 0x18);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
struct GeometryLightInfluence {
    std::uint32_t kind{};
    Vector4 colour{};
    Vector3 position{};
    Vector3 direction{};
    float rangeSquared{};
    float falloffPower{};
    float coneCosine{};
};
static_assert(sizeof(GeometryLightInfluence) == 0x38);

struct GeometryLightTreeWork {
    Matrix4x4 initialTransform{};
    std::span<const LightRuntimeEntry> lights;
    Vector4 baseColour{};
    SHENMUE_LIGHT_ARRAY(GeometryLightInfluence, influences, 128) {};
    std::uint32_t influenceCount{};
    std::uint32_t lightingRequired{};
};

#endif

void ResetMapLightingPlacements();
std::uint32_t GetMapLightingPlacementEnabled(std::int32_t index);
std::uint32_t GetMagicRoomFlag(std::int32_t index);
std::uint32_t GetMapLightingPlacementPosition(Vector3 *output, std::int32_t index);
std::uint32_t GetMapLightingPlacementRotation(std::int32_t index);
std::uint32_t GetMapLightingPlacementActive(std::int32_t index);
MapLightingPlacement *GetMapLightingPlacement(std::int32_t index);
void SetMapLightingPlacementEnabled(std::int32_t value, std::int32_t index);
std::uint32_t EV_SetMapLightingPlacementEnabled(std::int32_t index, std::int32_t enabled);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMapLightingPlacementPosition(const Vector3 *position, std::int32_t index);
#else
std::uint32_t SetMapLightingPlacementPosition(const Vector3 *position, std::int32_t index);
#endif
void SetMapLightingPlacementRotation(std::int32_t value, std::int32_t index);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void SetPropSlotTransform(std::int32_t index, const Vector3 *position, std::int32_t rotationY);
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void AttachMapLightingPlacement(std::int32_t mapSlot, std::int32_t placement);
#else
std::int64_t AttachMapLightingPlacement(std::int32_t mapSlot, std::int32_t placement);
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t AssignLightForMapLayer(Vector3 *output, std::int32_t mapSlot);
#else
std::int64_t AssignLightForMapLayer(Vector3 *output, std::int32_t mapSlot);
#endif
void AssignLightProgram(std::uint32_t mapSlot);
void PushGlobalLightingProgram();
void ApplyCharacterGeometryLighting(Character *character, std::int32_t replaceDrawCallback);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
using ::shenmue::shen_name;
#define LIGHT_DEFAULT_DEFINITION shen_name(SHEN_NAME4('0', '0', '0', '0'))
#else
inline constexpr shen_name LIGHT_DEFAULT_DEFINITION{SHEN_NAME(0000)};
#endif
void SetupLightConfig();
void SetupModelDataDefaultVS(MT7Node *root);
void SetDefaultVS();

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
struct TemporaryAttachedLightState {
    std::uint32_t slot{};
    std::int32_t activeLight{};
    LightRuntimeEntry entry{};
    std::int16_t selectedIndex{};
    bool engaged{};
};

bool BeginTemporaryAttachedLight(std::uint32_t slot, const Vector3 &origin, std::int16_t yaw, const Vector4 &colour, TemporaryAttachedLightState &saved);
void EndTemporaryAttachedLight(TemporaryAttachedLightState &saved);

#endif

} // namespace s2
} // namespace shenmue

#undef SHENMUE_LIGHT_ARRAY
#undef SHENMUE_LIGHT_DEFAULT
