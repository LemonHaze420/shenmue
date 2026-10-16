#pragma once
#include "shenmue.h"

#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {
struct MT7Node;
struct MT7Model;
struct Character;
struct MagicRoomEntityLight;
struct DateTime;
inline constexpr shen_name TASK_LGTP{SHEN_NAME(LGTP)};
inline constexpr shen_name CHUNK_LGHT{SHEN_NAME(LGHT)};
inline constexpr shen_name WEATHER_LENS_PACKAGE{SHEN_NAME(LNSW)}, WEATHER_LENS_MODEL{SHEN_NAME(LENS)};
struct WeatherLensMaterialParameter {
    std::uint32_t model, parameter;
};
inline constexpr std::array<WeatherLensMaterialParameter, 11> WEATHER_LENS_MATERIAL_PARAMETERS{{{11, 1157240422}, {0, 1157238784}, {1, 1157238784},
    {2, 1157238784}, {4, 1157235507}, {5, 0x44fa1000}, {6, 1157238784}, {7, 1157238784}, {8, 1157238784}, {9, 1157237146}, {10, 1157237146}}};

struct PackedLightDefinitionHeader {
    shen_name name;
    std::uint32_t byteSize;
    std::uint32_t reserved;
    std::int16_t lightCount;
    std::int16_t mode;
    std::array<std::int16_t, 4> colour;
};
struct PackedLightDefinitionRecord {
    std::int16_t index;
    std::int16_t enabled;
    std::int16_t flags;
    std::array<std::int16_t, 12> parameters;
    std::int16_t coneAngle;
};
static_assert(sizeof(PackedLightDefinitionHeader) == 0x18);
static_assert(offsetof(PackedLightDefinitionHeader, mode) == 0x0e);
static_assert(sizeof(PackedLightDefinitionRecord) == 0x20);
static_assert(offsetof(PackedLightDefinitionRecord, parameters) == 6);

struct LightDefinitionEntry {
    const std::byte *block{};
    std::uint32_t identifier{};
};
#if SHENMUE_IS_PC
static_assert(sizeof(LightDefinitionEntry) == 0x10);
static_assert(offsetof(LightDefinitionEntry, identifier) == 0x08);
#else
static_assert(sizeof(LightDefinitionEntry) == 0x08);
static_assert(offsetof(LightDefinitionEntry, identifier) == 0x04);
#endif
std::int64_t LightTask_Enqueue();
void ImmediateLightWork(void *);
void AssignMagicRoomLighting(const Vector3 &origin, float radius);
void AssignLightParametersForMagicRoomEntity(std::uint32_t index, const MagicRoomEntityLight &light);
void LGHT_Parser_Internal(std::uint64_t identifier);


bool RegisterLightData(const void *block, std::int32_t identifier, std::size_t size);

void UnregisterLightData(std::int32_t identifier);
std::int32_t GetCurrentActiveLight();
void SetCurrentActiveLight(std::int32_t mode);
void DrawPreparedVertexColours(const MT7Model *model, const MT7Node *node, float alpha);
void ResetLightEntry(std::uint32_t slot);
void SetLightKind(std::uint32_t slot, std::int32_t kind);
void SetSelectedLightDirection(std::uint32_t slot, const Vector3 &direction);
void shenSetLightColour(std::uint32_t slot, const Vector4 &colour);
void ReinitializeLights(std::int32_t mode);
void SetLightEntryIndex(std::uint32_t index, std::uint32_t value);
std::uint64_t GetLightEntryIndex(std::uint32_t index);
void SetLightConeAngle(std::uint32_t index, std::int16_t angle);
void SetLightFlagForIndex(std::uint32_t index, std::int32_t value);
std::uint64_t SetLightRootPosition(const Vector3 &position);
void TranslateLightDefinition(std::uint32_t identifier, const Vector3 &translation);
void PrioritizeLight(std::int16_t index);
std::uint64_t SetRankedLightLimit(std::uint32_t count);
void ConfigureDynamicLight(std::uint32_t slot, std::uint8_t kind, const Vector4 &colour, float scale);
void SetDynamicLightPosition(std::uint32_t slot, const Vector3 &position);
void SetDynamicLightEnabled(std::uint32_t slot, bool enabled);
void ApplyWeatherLightState(std::uint32_t index, const Vector4 *colour, const float *sceneIntensity, const Vector3 *sunDirection);
void ApplyWeatherGlobalLighting(float value);
void SetWeatherLighting(float value);
float GetWeatherLighting();
struct WeatherSunState {
    float jitterCountdown{};
    float sunlightIntensity{};
    Vector3 direction{};
    float diskScale{};
    float jitterScale{};
    std::int32_t enabled{};
    std::int32_t explicitDirection{};
    std::int32_t control{};
    float sourceIntensity{};
};
static_assert(sizeof(WeatherSunState) == 0x2c);
static_assert(offsetof(WeatherSunState, direction) == 0x08);
static_assert(offsetof(WeatherSunState, diskScale) == 0x14);
static_assert(offsetof(WeatherSunState, jitterScale) == 0x18);
static_assert(offsetof(WeatherSunState, enabled) == 0x1c);
static_assert(offsetof(WeatherSunState, explicitDirection) == 0x20);
static_assert(offsetof(WeatherSunState, control) == 0x24);
static_assert(offsetof(WeatherSunState, sourceIntensity) == 0x28);
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


void ResetLightRuntimeEntry(std::uint32_t index);
void UpdateGlobalLighting(const Vector4 &colour);
void GetGlobalLighting(Vector4 *colour);
void ExchangeRetainedGlobalLighting(const Vector4 &colour, Vector4 *saved);
void SetLightRuntimeColour(std::uint32_t index, const Vector4 &colour);
void SetLightSceneIntensity(std::uint32_t index, float intensity);
void SetLightRange(std::uint32_t index, float range);
void SetLightFalloff(std::uint32_t index, const float *falloff);
void SetLightRuntimePosition(std::uint32_t index, const Vector3 &position);
std::uint64_t SetLightSunDirection(std::uint32_t index, const Vector3 &direction);
void SetLightGeometryEnabled(std::uint32_t index, std::int32_t enabled);
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



struct LightRuntimeEntry {
    std::array<std::uint8_t, 2> indexes{};
    std::int16_t unknown02{};
    std::array<Vector3, 2> vectors{};
    float value1c{};
    Vector3 vector20{};
    std::array<float, 2> values2c{};
    float value34{};
};

static_assert(sizeof(LightRuntimeEntry) == 0x38);
static_assert(offsetof(LightRuntimeEntry, vectors) == 0x04);
static_assert(offsetof(LightRuntimeEntry, value1c) == 0x1c);
static_assert(offsetof(LightRuntimeEntry, vector20) == 0x20);
static_assert(offsetof(LightRuntimeEntry, values2c) == 0x2c);
static_assert(offsetof(LightRuntimeEntry, unknown02) == 0x02);
static_assert(offsetof(LightRuntimeEntry, value34) == 0x34);

struct LegacyLightDefinitionHeader {
    std::int32_t mode;
    Vector4 colour;
    LightRuntimeEntry primary;
};
static_assert(sizeof(LegacyLightDefinitionHeader) == 0x4c);
static_assert(offsetof(LegacyLightDefinitionHeader, primary) == 0x14);


std::span<const std::int16_t> GetCharacterLightCandidates();
const LightRuntimeEntry *GetLightRuntimeEntry(std::uint32_t index);
void CopyLightRuntimeEntry(std::uint32_t destination, std::uint32_t source);
std::uint64_t GetLightRootPosition(Vector3 *position);
std::int32_t GetPreviousActiveLight();

struct MapLightingPlacement {
    std::uint32_t enabled;
    std::uint32_t mapSlot;
    Vector3 position;
    std::int32_t rotationY;
    std::uint32_t active;
};
static_assert(sizeof(MapLightingPlacement) == 0x1c);
static_assert(offsetof(MapLightingPlacement, mapSlot) == 4);
static_assert(offsetof(MapLightingPlacement, position) == 8);
static_assert(offsetof(MapLightingPlacement, rotationY) == 0x14);
static_assert(offsetof(MapLightingPlacement, active) == 0x18);

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
    std::array<GeometryLightInfluence, 128> influences{};
    std::uint32_t influenceCount{};
    std::uint32_t lightingRequired{};
};

void ResetMapLightingPlacements();
std::uint32_t GetMapLightingPlacementEnabled(std::int32_t index);
std::uint32_t GetMagicRoomFlag(std::int32_t index);
std::uint32_t GetMapLightingPlacementPosition(Vector3 *output, std::int32_t index);
std::uint32_t GetMapLightingPlacementRotation(std::int32_t index);
std::uint32_t GetMapLightingPlacementActive(std::int32_t index);
MapLightingPlacement *GetMapLightingPlacement(std::int32_t index);
void SetMapLightingPlacementEnabled(std::int32_t value, std::int32_t index);
std::uint32_t SetMapLightingPlacementPosition(const Vector3 *position, std::int32_t index);
void SetMapLightingPlacementRotation(std::int32_t value, std::int32_t index);
std::int64_t AttachMapLightingPlacement(std::int32_t mapSlot, std::int32_t placement);
std::int64_t AssignLightForMapLayer(Vector3 *output, std::int32_t mapSlot);
void AssignLightProgram(std::uint32_t mapSlot);
void PushGlobalLightingProgram();
void ApplyCharacterGeometryLighting(Character *character, std::int32_t replaceDrawCallback);
inline constexpr shen_name LIGHT_DEFAULT_DEFINITION{SHEN_NAME(0000)};
void SetupLightConfig();
void SetupModelDataDefaultVS(MT7Node *root);
void SetDefaultVS();

struct TemporaryReflectionLightState {
    std::uint32_t slot{};
    std::int32_t activeLight{};
    bool engaged{};
};

bool BeginTemporaryReflectionLight(std::uint32_t slot, const Vector3 &sourcePosition, const Vector3 &cameraPosition, const Vector4 &colour, std::uint32_t mode,
    std::int16_t yaw, TemporaryReflectionLightState &saved);
void EndTemporaryReflectionLight(TemporaryReflectionLightState &saved);

struct TemporaryAttachedLightState {
    std::uint32_t slot{};
    std::int32_t activeLight{};
    LightRuntimeEntry entry{};
    std::int16_t selectedIndex{};
    bool engaged{};
};

bool BeginTemporaryAttachedLight(std::uint32_t slot, const Vector3 &origin, std::int16_t yaw, const Vector4 &colour, TemporaryAttachedLightState &saved);
void EndTemporaryAttachedLight(TemporaryAttachedLightState &saved);

} // namespace shenmue::s2
