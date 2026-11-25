#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/lighting.hpp"

#define SHENMUE_DC_LIGHT_ARRAY(type, name, count) type name[count]

namespace shenmue {
namespace s2 {

extern SHENMUE_DC_LIGHT_ARRAY(LightDefinitionEntry, g_lightDefinitions, 24);
extern SHENMUE_DC_LIGHT_ARRAY(LightRuntimeEntry, g_lightRuntimeEntries, 256);
extern SHENMUE_DC_LIGHT_ARRAY(LightRuntimeEntry, g_selectedLights, 16);
extern SHENMUE_DC_LIGHT_ARRAY(Vector4, g_savedLightColours, 256);
extern Vector4 g_globalLighting;
extern float g_weatherLighting, g_weatherSceneLighting;
extern LightRuntimeEntry disabledLight;
extern std::int32_t g_currentActiveLight, g_previousActiveLight, g_lightRankingEnabled, g_rankedLightLimit;
extern float g_lightSelectionRadius;
extern Vector3 g_lightRootPosition, g_lightPositionScratch;
extern SHENMUE_DC_LIGHT_ARRAY(std::int16_t, g_characterLightCandidates, 256);
Vector3 *GetRuntimeLightWorldPosition(const LightRuntimeEntry *light);
void GetLightRuntimePosition(std::uint32_t index, Vector3 *position);
void SelectLightForFrame(std::uint32_t index, LightRuntimeEntry *light);
void InitializeMapLighting();
void PublishGlobalLighting();
std::uint32_t GetGeometryLightColour(Vector4 *colour);
void AllocateGeometryLightPositionScratch();
void RestoreGeometryLightPositionScratch();
extern const Matrix4x4 *g_geometryLightTransform;
std::uint32_t ApplyGeometryLightTree(MT7Node *root, const LightRuntimeEntry *lights, std::int32_t count, const Vector4 *colour);
void SetLightRuntimeType(std::uint32_t index, std::int32_t value);
std::uint32_t GetSelectedLightKind(std::uint32_t index);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static const Vector4 characterShadowBlack = {};
extern std::int32_t g_characterShadowSavedLightMode;
extern Vector4 g_characterShadowSavedAmbient;
std::uint32_t GetLightRuntimeKind(std::uint32_t index);
float GetLightRuntimeRange(std::uint32_t index);
std::int32_t GetLightRuntimeConeAngle(std::uint32_t index);
void GetLightRuntimeDirection(std::uint32_t index, Vector3 *direction);
void GetSelectedLightColour(std::uint32_t index, Vector4 *colour);
void PrepareCharacterShadowLighting();
void RestoreCharacterShadowLighting();
#endif
void GetSelectedLightDirection(std::uint32_t index, Vector3 *direction);
float GetSelectedLightSceneIntensity(std::uint32_t index);
void SetSelectedLightSceneIntensity(std::uint32_t index, float intensity);
void SetSelectedLightEnabled(std::uint32_t index, std::uint32_t enabled);
void EnableSelectedLight(std::uint32_t index);
void DisableSelectedLight(std::uint32_t index);
void SetSelectedLightRange(std::uint32_t index, float range);
void SetSelectedLightFalloff(std::uint32_t index, float falloff);
void SetSelectedLightPosition(std::uint32_t index, const Vector3 &position);
void SetSelectedLightConeAngle(std::uint32_t index, std::int32_t angle);
void ResetSelectedLightEntry(std::uint32_t index);
void InitializeLightRuntimeTables();
#if SHENMUE_TARGET_S2_DC_JP_GAMEJAM || SHENMUE_TARGET_S2_DC_JP_1001
std::int32_t FindUnusedSelectedLight();
#endif

void ResetCharacterGeometryLighting(Character *character);

} // namespace s2
} // namespace shenmue

#undef SHENMUE_DC_LIGHT_ARRAY

#endif
