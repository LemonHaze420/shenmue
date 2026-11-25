#include "s2/shadow_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/cold_archive_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/hlib.hpp"
#include "platform/dc/ninja.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/modifier_volume_dc.hpp"
#endif
#include <cstdio>
#include <cstring>
#include "common/native_sqrt.hpp"
#include "s2/prop_dc.hpp"
#include "s2/flag_wind_dc.hpp"
#include "s2/search_dc.hpp"
#include "shenmue.h"
#include "s2/object_draw_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/world_render.hpp"
#include "s2/maze_dc.hpp"
#include "s2/motion_rig.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/math.hpp"
#include "s2/camera_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/shadow_render_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/character_transparency_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "platform/dc/particle_effects.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/event_matrix_dc.hpp"
#endif
#include "s2/character_components_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/effect_fft.hpp"
#include "common/math.hpp"
#include <cmath>
#include "s2/map_info.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "s2/player_model_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/object_light_dc.hpp"
namespace shenmue {
namespace dc {
namespace sdk {
void SetMirroredModelDrawMode(std::int32_t enabled);
}
} // namespace dc
} // namespace shenmue
#include "s2/event_character_state_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/hand_dc.hpp"
#include "s2/pause.hpp"
#endif
#include "s2/pad_control_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/event_character_control_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/object_xforms.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "s2/map_event_task_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "common/matrix_stack.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/character_table_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/directory_cache.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/font.hpp"
#include "s2/screen_film.hpp"
#include "s2/title_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/window.hpp"
#include "s2/area_event.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/game_save_area.hpp"
#include "s2/input_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/ui_state_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/story_state.hpp"
#include <cstdarg>
#endif
#include "s2/scenario_character_query_dc.hpp"
namespace shenmue {
namespace s2 {

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct WaveLighting {
    std::uint32_t flags;
    float cellSize;
    float ambient;
    Vector3 diffuse;
    Vector3 specular;
    float specularExponent;
    float specularIntensity;
};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef char WaveLightingSizeCheck[sizeof(WaveLighting) == 0x2c ? 1 : -1];
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef char WaveLightingSpecularCheck[offsetof(WaveLighting, specular) == 0x18 ? 1 : -1];
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct WaveVertex {
    float height;
    std::uint32_t normal, diffuse, specular, retained10;
};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef char WaveVertexSizeCheck[sizeof(WaveVertex) == 0x14 ? 1 : -1];
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct WaveState {
    WaveLighting lighting;
    float textureU, textureV;
    float scrollU, scrollV;
    std::uint32_t reflectionDiffuse, reflectionSpecular;
    float real[17][17], imaginary[17][17];
    WaveBin bins[64];
    WaveVertex vertices[17][17];
};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef char WaveStateScrollCheck[offsetof(WaveState, scrollU) == 0x34 ? 1 : -1];
typedef char WaveStateReflectionCheck[offsetof(WaveState, reflectionDiffuse) == 0x3c ? 1 : -1];
typedef char WaveStateRealCheck[offsetof(WaveState, real) == 0x44 ? 1 : -1];
typedef char WaveStateImaginaryCheck[offsetof(WaveState, imaginary) == 0x4c8 ? 1 : -1];
typedef char WaveStateBinsCheck[offsetof(WaveState, bins) == 0x94c ? 1 : -1];
typedef char WaveStateVerticesCheck[offsetof(WaveState, vertices) == 0xe4c ? 1 : -1];
typedef char WaveStateSizeCheck[sizeof(WaveState) == 0x24e0 ? 1 : -1];
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct WaveMeshVertex {
    Vector3 position;
    std::uint32_t normal, diffuse, specular;
    float u, v;
};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef char WaveMeshVertexSizeCheck[sizeof(WaveMeshVertex) == 0x20 ? 1 : -1];
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct WaterSurfaceCandidate {
    float distance;
    Vector3 position;
    MT7Node *node;
};
#endif

bool ContainsWaterSurfaceQuad(std::int32_t first, std::int32_t stride);
void SampleWaterSurfaceHeight();

} // namespace

void SetShadowOpacityScale(float opacity);
void SetShadowPointLightMode(std::int32_t mode);
static void InitializeWaveBinPhases(WaveBin *bins);
float EvaluateCurveSegment(const MotionRigKey *key, std::int32_t kind, float time);
float EvaluateCurveSegmentCubic(const MotionRigKey *previous, const MotionRigKey *key, float time);
float EvaluateCurveSlopeSegment(const MotionRigKey *key, std::int32_t kind, float time);
float EvaluateCurveSlopeCubic(const MotionRigKey *previous, const MotionRigKey *key, float time);
void MapModifierVolumeTask_Callback(void *);
MT7Node *GetSHDS();
ShadowShapeNode *ReadShadowShape(ShadowShapeHeader *header);
ShadowShapeNode *LoadShadowShape(const char *name);

void *charFindComp(const Character *character, shen_name token);
float GetCharacterAlpha(const Character *character);
void GetCharacterWorldPosition(const Character *character, Vector3 *position);
void GetCharacterShadowOffset(const Character *character, Vector3 *offset);
void GetCharacterShadowScale(const Character *character, Vector3 *scale);
std::int32_t GetCharacterShadowYaw(const Character *character);
std::int32_t GetCharacterType(const Character *character);
float GetCharacterShadowY(const Character *character);
bool IsCharacterStoredShadowYEnabled(const Character *character);
void GetCharacterShadowOffset(const Character *character, Vector3 *offset);
void GetCharacterShadowScale(const Character *character, Vector3 *scale);
std::int32_t GetCharacterShadowYaw(const Character *character);
std::int32_t GetCharacterType(const Character *character);
float GetCharacterShadowY(const Character *character);
bool IsCharacterStoredShadowYEnabled(const Character *character);
void charaCollisionDistanceChecks(void *data);
void InitializeShadowDrawBuffers(std::byte *memory, const ShadowDrawParameters &parameters);
void InitializeShadowDrawBuffers(std::byte *memory, const ShadowDrawParameters &parameters);
MT7Node *g_sydRoot;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_shadowRenderingEnabled = 1;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float g_shadowDistanceFadeWidth = 1.0f;
#endif

ShadowShapeCacheEntry g_shadowShapeCache[8];
std::int32_t g_shadowShapeCacheCount;
ShadowShapeNode *g_shadowChestShape;
ShadowShapeNode *g_shadowArmShape;
ShadowDrawParameters g_shadowDrawParameters;
std::int32_t g_shadowVertexCapacity = 0;
std::int32_t g_shadowClipCapacity = 0;
std::int32_t g_shadowPolygonCapacity = 0;
ShadowVolumeVertex *g_shadowVertices;
ShadowVolumeVertex *g_shadowExtrudedVertices;
ShadowVolumeVertex *g_shadowNearPlaneVertices;
ShadowVolumeVertex *g_shadowEdgeNearPoints;
ShadowVolumeVertex *g_shadowEdgeFarPoints;
std::uint8_t *g_shadowEdgeFlags;
Vector3 *g_shadowFaceLightVectors;
std::uint32_t g_shadowStripHeader[2];
std::uint32_t g_shadowStripLastHeader[2];

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float g_shadowOpacityScale = 128.0f / 255.0f;
#else
float g_shadowOpacityScale = 0.0f;
#endif

float g_shadowParameter = 0.0f;
std::int32_t g_shadowPointLightMode = 0;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float g_mazeUnitDrawDistance;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_mazeTerrainDrawEnabled;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_mazeAuxiliaryWord = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_mazeRetainedSetting = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_mazeRetainedValue = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float g_waveLightAmbient = 0.0f;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Vector3 g_waveLightDirection = {};
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
WaveState waveState = {};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Vector3 waterQueryPosition;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
WaterSurfaceCandidate waterCandidates[4];
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Vector3 waterSurfacePositions[17][17], waterSurfaceQuery;
float floatingSurfaceOriginX, floatingSurfaceOriginY, floatingSurfaceOriginZ;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t waterSurfaceCell = -1, floatingSurfaceNodeCount = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int16_t floatingSurfacePreviousAngles[30][2];
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void *fixedShadowMeshes[2] = {fixedCharacterShadowMesh0, fixedCharacterShadowMesh1};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Vector3 floatingSurfaceSample0 = {0.0f, 0.0f, 3.8f}, floatingSurfaceSample1 = {1.4f, 0.0f, -3.98f}, floatingSurfaceSample2 = {-1.4f, 0.0f, -3.98f};
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
ShadowShapeNode *g_mapShadowShapes[10];
MapShadowPlacement g_mapShadowPlacements[101];
#else
MapShadowStorage g_mapShadowStorage = {};
#endif
std::int32_t g_shadowVolumesDisabled = 0;
std::int32_t g_shadowStrengthLimit = 999;
ShadowVolume *g_mapShadowVolumes[2] = {};
ShadowModelCacheEntry g_shadowModelCache[10];
std::int32_t g_shadowModelCacheCount = 0;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MT7Node *g_shadowSprite;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetNaviTaskEnabled(std::int32_t enabled);
#endif

// @verified - d
void InitSYD() {
    g_shadowShapeCacheCount = 0;
    g_shadowChestShape = LoadShadowShape("SYD_MUNE");
    g_shadowArmShape = LoadShadowShape("SYD_UDE0");
    std::byte *const memory = static_cast<std::byte *>(HLib::AllocateTaskData(97000, SHADOW_VERTEX_MEMORY));
    g_shadowDrawParameters.flags = 0;
    g_shadowDrawParameters.vertexCapacity = 1000;
    g_shadowDrawParameters.clipCapacity = 1000;
    g_shadowDrawParameters.polygonCapacity = 1000;
    SetShadowOpacityScale(0.5f);
    SetShadowPointLightMode(0);
    InitializeShadowDrawBuffers(memory, g_shadowDrawParameters);
    g_shadowParameter = 0.0f;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeTerrainDrawEnabled() {
    return g_mazeTerrainDrawEnabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeRoadDrawEnabled() {
    return g_mazeRoadDrawEnabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeRoadPropsEnabled() {
    return g_mazeRoadPropsEnabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeUnitDrawEnabled() {
    return g_mazeUnitDrawEnabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeWireframeEnabled() {
    return g_mazeWireframeEnabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeNaviEnabled() {
    return g_mazeRouteDrawEnabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeCullingAngleLimit() {
    return g_mazeCullingSteps << 10;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeModelOverride() {
    return g_mazeModelOverride;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeUnitWallsEnabled() {
    return g_mazeUnitWallsEnabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float GetMazeUnitDrawDistance() {
    return g_mazeUnitDrawDistance;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetMazeUnitDrawDistance(float distance) {
    g_mazeUnitDrawDistance = distance;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeRetainedSetting() {
    return g_mazeRetainedSetting;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeRetainedSetting(std::int32_t value) {
    g_mazeRetainedSetting = value;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeRoadDrawEnabled(std::int32_t enabled) {
    g_mazeRoadDrawEnabled = enabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeCullingSteps(std::int32_t steps) {
    g_mazeCullingSteps = steps;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetMazeTerrainDrawEnabled(std::int32_t enabled) {
    g_mazeTerrainDrawEnabled = enabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeRoadPropsEnabled(std::int32_t enabled) {
    g_mazeRoadPropsEnabled = enabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeUnitDrawEnabled(std::int32_t enabled) {
    g_mazeUnitDrawEnabled = enabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeWireframeEnabled(std::int32_t enabled) {
    g_mazeWireframeEnabled = enabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeUnitWallsEnabled(std::int32_t enabled) {
    g_mazeUnitWallsEnabled = enabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeNaviEnabled(std::int32_t enabled) {
    SetNaviTaskEnabled(enabled);
    g_mazeRouteDrawEnabled = enabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeModelOverride(std::int32_t enabled) {
    g_mazeModelOverride = enabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeAuxiliaryWord(std::int32_t value) {
    g_mazeAuxiliaryWord = value;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeAuxiliaryWord() {
    return g_mazeAuxiliaryWord;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetMazeRetainedValue(std::int32_t value) {
    g_mazeRetainedValue = value;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetMazeRetainedValue() {
    return g_mazeRetainedValue;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void *GetFixedShadowMesh(std::uint32_t index) {
    void *mesh;
    if (index > 1)
        mesh = 0;
    else
        mesh = fixedShadowMeshes[index];
    return mesh;
}
#endif

// @verified - d
void SetShadowOpacityScale(float opacity) {
    if (opacity < 0.0f || opacity > 1.0f)
        return;
    g_shadowOpacityScale = opacity;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float GetShadowOpacityScale() {
    return g_shadowOpacityScale;
}
#endif

// @verified - d
void SetShadowPointLightMode(std::int32_t mode) {
    if (mode < 3)
        g_shadowPointLightMode = mode;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void MVTR_Reader() {
    MapInfoChunk *mapInfo = GetCurrentMapInfo();
    std::int32_t *cursor = reinterpret_cast<std::int32_t *>(FindNativeChunk("MVTR", mapInfo));
    if (cursor)
        kmUseAnotherModifier(1);
    else
        kmUseAnotherModifier(0);
    SetShadowOpacityScale(0.5f);
    SetShadowPointLightMode(0);
    g_shadowParameter = 0.0f;
    std::int32_t shapeCount = 0;
    std::int32_t placementCount = 0;
    cursor = reinterpret_cast<std::int32_t *>(FindNativeChunk("MPMV", mapInfo));
    if (cursor)
        cursor += 2;
    while (cursor) {
        switch (static_cast<std::uint32_t>(*cursor)) {
        case SHEN_NAME4('M', 'V', 'D', 'T'):
            if (shapeCount < 10 && reinterpret_cast<ShadowShapeHeader *>(cursor + 2)->unknown00 == SHEN_NAME4('M', 'V', 'S', 'D'))
                g_mapShadowShapes[shapeCount++] = ReadShadowShape(reinterpret_cast<ShadowShapeHeader *>(cursor + 2));
            cursor = reinterpret_cast<std::int32_t *>(reinterpret_cast<std::byte *>(cursor) + cursor[1]);
            break;
        case SHEN_NAME4('M', 'V', 'T', 'B'):
            cursor += 2;
            while (*cursor >= 0) {
                if (placementCount < 100) {
                    g_mapShadowPlacements[placementCount].shapeIndex = *cursor++;
                    g_mapShadowPlacements[placementCount].position.x = *reinterpret_cast<float *>(cursor++);
                    g_mapShadowPlacements[placementCount].position.y = *reinterpret_cast<float *>(cursor++);
                    g_mapShadowPlacements[placementCount].position.z = *reinterpret_cast<float *>(cursor++);
                    g_mapShadowPlacements[placementCount].rotation.x = *reinterpret_cast<float *>(cursor++);
                    g_mapShadowPlacements[placementCount].rotation.y = *reinterpret_cast<float *>(cursor++);
                    g_mapShadowPlacements[placementCount].rotation.z = *reinterpret_cast<float *>(cursor++);
                    ++placementCount;
                } else
                    cursor += 7;
            }
            ++cursor;
            break;
        case 0:
            cursor = 0;
            break;
        default:
            cursor = 0;
        }
    }
    for (; shapeCount < 10; ++shapeCount)
        g_mapShadowShapes[shapeCount] = 0;
    g_mapShadowPlacements[placementCount].shapeIndex = -1;
    placementCount = 0;
    while (g_mapShadowPlacements[placementCount++].shapeIndex >= 0)
        ;
    HLib::EnqueueTaskWithoutParameter(MapModifierVolumeTask_Callback, 10, 0, MAP_MODIFIER_VOLUME_TASK);
}
#else
void MVTR_Reader() {
    MapInfoChunk *const mapInfo = GetCurrentMapInfo();
    kmUseAnotherModifier(FindNativeChunk("MVTR", mapInfo) ? 1U : 0U);
    SetShadowOpacityScale(0.5f);
    SetShadowPointLightMode(0);
    g_shadowParameter = 0.0f;
    std::int32_t shapeCount = 0;
    std::int32_t placementCount = 0;
    const MapInfoChunk *chunk = FindNativeChunk("MPMV", mapInfo);
    if (chunk)
        ++chunk;
    while (chunk) {
        if (chunk->name == SHEN_NAME(MVTB)) {
            const std::byte *cursor = reinterpret_cast<const std::byte *>(chunk + 1);
            std::int32_t shapeIndex;
            std::memcpy(&shapeIndex, cursor, sizeof(shapeIndex));
            while (shapeIndex >= 0) {
                if (placementCount < 100) {
                    std::memcpy(&g_mapShadowStorage.placements[placementCount], cursor, sizeof(MapShadowPlacement));
                    ++placementCount;
                }
                cursor += sizeof(MapShadowPlacement);
                std::memcpy(&shapeIndex, cursor, sizeof(shapeIndex));
            }
            chunk = reinterpret_cast<const MapInfoChunk *>(cursor + sizeof(shapeIndex));
        } else if (chunk->name == SHEN_NAME(MVDT)) {
            ShadowShapeHeader *const shape = reinterpret_cast<ShadowShapeHeader *>(const_cast<MapInfoChunk *>(chunk + 1));
            if (shapeCount < 10 && shape->unknown00 == SHEN_NAME(MVSD))
                g_mapShadowStorage.shapes[shapeCount++] = ReadShadowShape(shape);
            chunk = reinterpret_cast<const MapInfoChunk *>(reinterpret_cast<const std::byte *>(chunk) + chunk->nextByteOffset);
        } else
            chunk = 0;
    }
    for (; shapeCount < 10; ++shapeCount)
        g_mapShadowStorage.shapes[shapeCount] = 0;
    const std::int32_t terminator = -1;
    std::memcpy(reinterpret_cast<std::byte *>(&g_mapShadowStorage) + offsetof(MapShadowStorage, placements) + placementCount * sizeof(MapShadowPlacement),
        &terminator, sizeof(terminator));
    HLib::EnqueueTaskWithoutParameter(MapModifierVolumeTask_Callback, 10, 0, MAP_MODIFIER_VOLUME_TASK);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyCharacterShadowBoneTransform(MT7Node *model, std::int16_t boneId) {
    MT7Node *const node = FindMT7Node(model, boneId);
    if (node && node->transformOverride)
        matrix_stack::LoadMatrix(node->transformOverride);
    if (boneId == 1)
        Matrix4x4::preTranslate(-0.13f, 0.0f, 0.0f);
    if (boneId == 14)
        Matrix4x4::RotateAroundZ_Wrapper(-DEG_TO_ANG(90.0f));
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawCharacterShadowShapeNodes(MT7Node *model, ShadowShapeNode *shape) {
    ShadowVolume *const volume = reinterpret_cast<ShadowVolume *>(&shape->data[2]);
    Vector3 *vertices = static_cast<Vector3 *>(shape->data[0]);
    Vector3 *transformedVertices = volume->vertices;
    Vector3 *normals = static_cast<Vector3 *>(shape->data[1]);
    ShadowVolumeFace *transformedFaces = volume->faces;
    ApplyCharacterShadowBoneTransform(model, shape->boneId);
    for (std::int32_t index = 0; index < volume->vertexCount; ++index)
        Matrix4x4::TransformPoint(*vertices++, *transformedVertices++);
    for (std::int32_t index = 0; index < volume->faceCount; ++index)
        Matrix4x4::Transform(*normals++, (transformedFaces++)->normal);
    matrix_stack::GetPreviousMatrix(0);
    g_mapShadowVolumes[0] = volume;
    DrawShadowVolumes(g_mapShadowVolumes, &g_shadowDrawParameters);
    if (shape->child)
        DrawCharacterShadowShapeNodes(model, shape->child);
    if (shape->sibling)
        DrawCharacterShadowShapeNodes(model, shape->sibling);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void DrawCharacterShadowShape(MT7Node *model, ShadowShapeNode *shape) {
    matrix_stack::Matrix4x4_Push(0);
    DrawCharacterShadowShapeNodes(model, shape);
    PopCurrentMatrix(1);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawObject_AssignShadow(const Vector3 *position, float groundY, MT7Node *model, std::int32_t, Character *character) {
    if (g_shadowVolumesDisabled || !g_shadowRenderingEnabled || !model || !character)
        return;
    CharacterLightControl *const control = reinterpret_cast<CharacterLightControl *>(GetControllerForNode(character));
    if (!control || !control->mode)
        return;
    std::int32_t lights[kCharacterLightCount];
    float weights[kCharacterLightCount];
    Vector3 direction, source;
    g_shadowDrawParameters.flags &= ~1U;
    float vertical = 0.0f;
    camCtrlGetNextCamPos(&source);
    float measure = CalculatePlanarDistanceBetweenPoints(position, &source);
    float fade = GetShadowRange();
    fade -= measure;
    if (g_shadowDistanceFadeWidth < 0.01f)
        fade = 1.0f;
    else
        fade /= g_shadowDistanceFadeWidth;
    if (fade < 0.0f)
        fade = 0.0f;
    else if (fade > 1.0f)
        fade = 1.0f;
    const float alpha = GetCharacterAlpha(character);
    fade *= g_shadowOpacityScale * alpha;
    if (characterShadowThreshold > fade)
        return;
    measure = 0.0f;
    std::int32_t slot;
    for (slot = 0; slot < kCharacterLightCount && (lights[slot] = control->ids[slot]) >= 0; ++slot) {
        const float intensity = __builtin_sqrtf(control->intensities[slot]);
        weights[slot] = intensity * control->weights[slot];
        if (characterShadowThreshold > weights[slot])
            lights[slot] = -1;
        else if (weights[slot] > measure)
            measure = weights[slot];
    }
    for (; slot < kCharacterLightCount; ++slot)
        lights[slot] = -1;
    PrepareCharacterShadowLighting();
    for (slot = 0; slot < kCharacterLightCount; ++slot) {
        if (weights[slot] < measure - 0.004f) {
            DrawProjectedSharedCharacterShadow(position, lights[slot], groundY, character, fade * weights[slot]);
            lights[slot] = -1;
        }
    }
    RestoreCharacterShadowLighting();
    std::int32_t strength = static_cast<std::int32_t>((1.0f - fade * measure) * 255.0f);
    if (strength < 0)
        strength = 0;
    else if (strength > 254)
        return;
    std::int32_t drawValid;
    for (slot = 0; slot < kCharacterLightCount; ++slot) {
        if (lights[slot] < 0)
            continue;
        switch (GetLightRuntimeKind(lights[slot])) {
        case 2:
            GetLightRuntimeDirection(lights[slot], &direction);
            am2Vector3NormalizeInPlace(&direction);
            vertical = direction.y;
            Matrix4x4::Transform(direction, direction);
            g_shadowDrawParameters.lightDirection.x = -direction.x;
            g_shadowDrawParameters.lightDirection.y = -direction.y;
            g_shadowDrawParameters.lightDirection.z = -direction.z;
            g_shadowDrawParameters.flags &= ~1U;
            drawValid = 1;
            break;
        case 3:
        case 4:
            GetLightRuntimePosition(lights[slot], &source);
            direction.x = source.x - position->x;
            direction.y = source.y - position->y;
            direction.z = source.z - position->z;
            if (direction.y < -0.3f)
                break;
            if (direction.y < 1.0f) {
                source.y = position->y + 1.0f;
                direction.y = 1.0f;
            }
            if (!g_shadowPointLightMode) {
                am2Vector3NormalizeInPlace(&direction);
                vertical = -direction.y;
                Matrix4x4::Transform(direction, direction);
                g_shadowDrawParameters.lightDirection.x = direction.x;
                g_shadowDrawParameters.lightDirection.y = direction.y;
                g_shadowDrawParameters.lightDirection.z = direction.z;
                drawValid = 1;
            } else {
                am2Vector3NormalizeInPlace(&direction);
                vertical = -direction.y;
                Matrix4x4::TransformPoint(source, source);
                g_shadowDrawParameters.lightDirection.x = source.x;
                g_shadowDrawParameters.lightDirection.y = source.y;
                g_shadowDrawParameters.lightDirection.z = source.z;
                g_shadowDrawParameters.flags |= 1U;
                drawValid = 1;
            }
            break;
        default:
            drawValid = 0;
            break;
        }
        if (!drawValid)
            continue;
        if (g_shadowStrengthLimit > strength) {
            g_shadowStrengthLimit = strength;
            g_shadowDrawParameters.shadowStrength = strength;
        } else {
            strength = g_shadowStrengthLimit;
            g_shadowDrawParameters.shadowStrength = strength;
        }
        matrix_stack::Matrix4x4_Push(0);
        if (vertical < 0.0f) {
            float length = shadowVolumeNominalHeight / -vertical;
            if (length > shadowVolumeMaximumLength)
                length = shadowVolumeMaximumLength;
            g_shadowDrawParameters.length = length;
        } else {
            g_shadowDrawParameters.length = shadowVolumeMaximumLength;
            Matrix4x4::preTranslate(0.0f, -0.7f, 0.0f);
        }
        DrawCharacterShadowShape(model, g_shadowChestShape);
        PopCurrentMatrix(1);
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawShenmueEntity(void *parameter) {
    Character *const character = static_cast<Character *>(parameter);
    MT7Node *const model = character->model;
    if (!model)
        return;
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (work->sequenceFlags & MSF_SearchHand)
        return;
    Vector3 position, offset, scale;
    Vector3i rotation;
    if (work->drawOverrideData)
        GetCharacterWorldPosition(character, &position);
    else
        mtwkGetRootPosition(work, &position);
    float ground;
    if (!IsCharacterStoredShadowYEnabled(character)) {
        Vector3 probe;
        probe.x = position.x;
        probe.y = position.y + 0.5f;
        probe.z = position.z;
        ground = FindFieldIntersectionHeight(&probe);
        if (ground < -1000.0f)
            ground = position.y;
        else if (ground > position.y)
            ground = position.y;
    } else {
        ground = GetCharacterShadowY(character);
    }
    do {
        if (work->drawOverrideData) {
            DrawCharacterShadowSprite(&position, ground, character);
            return;
        }
    } while (0);
    void *mesh;
    switch (GetCharacterType(character)) {
    case 2:
        mesh = GetFixedShadowMesh(1);
        GetCharacterShadowOffset(character, &offset);
        GetCharacterShadowScale(character, &scale);
        position.x += offset.x;
        position.y += offset.y;
        position.z += offset.z;
        rotation.z = 0;
        rotation.x = 0;
        rotation.y = shenGetRotYFromNode(character) + GetCharacterShadowYaw(character);
        DrawProjectedFixedShadow(&position, &rotation, &scale, ground, mesh, -1.0f, character);
        break;
    case 9:
        mesh = GetFixedShadowMesh(1);
        GetCharacterShadowOffset(character, &offset);
        GetCharacterShadowScale(character, &scale);
        position.x += offset.x;
        position.y += offset.y;
        position.z += offset.z;
        rotation.z = 0;
        rotation.x = 0;
        rotation.y = shenGetRotYFromNode(character) + GetCharacterShadowYaw(character);
        DrawProjectedFixedShadow(&position, &rotation, &scale, ground, mesh, 1.0f, character);
        break;
    case 3:
        mesh = GetFixedShadowMesh(0);
        GetCharacterShadowOffset(character, &offset);
        GetCharacterShadowScale(character, &scale);
        position.x += offset.x;
        position.y += offset.y;
        position.z += offset.z;
        rotation.z = 0;
        rotation.x = 0;
        rotation.y = shenGetRotYFromNode(character) + GetCharacterShadowYaw(character);
        DrawProjectedFixedShadow(&position, &rotation, &scale, ground, mesh, -1.0f, character);
        break;
    case 1:
        DrawMotionCharacterShadow(&position, ground, model, character->shadowModel, 1.0f, character);
        break;
    case 10:
        DrawMotionCharacterShadow(&position, ground, model, character->shadowModel, -1.0f, character);
        break;
    case 4:
        DrawObject_AssignShadow(&position, ground, model, 0, character);
        break;
    case 5:
        DrawObject_AssignShadow(&position, ground, model, 1, character);
        break;
    case 6:
        DrawMotionCharacterShadow(&position, ground, model, character->shadowModel, 0.5f, character);
        break;
    case 7:
        DrawMotionCharacterShadow(&position, ground, model, model, -1.0f, character);
        break;
    case 8:
        DrawMotionCharacterShadow(&position, ground, model, model, 1.0f, character);
        break;
    case 11:
        DrawMotionCharacterShadow(&position, ground, model, model, -2.0f, character);
        break;
    case 0:
    default:
        DrawMotionCharacterShadow(&position, ground, model, character->shadowModel, -1.0f, character);
        break;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void GenerateFFT(float cellSize) {
    WaveState *state = &waveState;
    dc::sdk::InitializeFftTwiddles();
    state->lighting.flags = 0;
    state->lighting.cellSize = cellSize;
    state->lighting.specularExponent = 16.0f;
    state->lighting.specularIntensity = 1.0f;
    state->textureU = 0.0f;
    state->textureV = 0.0f;
    state->scrollU = 0.0f;
    state->scrollV = 0.0f;
    dc::sdk::SeedRandom(777);
    InitializeWaveBinPhases(state->bins);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static void InitializeWaveBinPhases(WaveBin *bins) {
    for (std::int32_t row = 0; row < 8; ++row) {
        for (std::int32_t column = 0; column < 8; ++column) {
            bins->phaseA = dc::sdk::RandomFloat() * 6.2831854820251465f;
            bins->phaseB = dc::sdk::RandomFloat() * 6.2831854820251465f;
            bins->speedA = 0.0f;
            bins->speedB = 0.0f;
            ++bins;
        }
    }
}
#endif

// @verified - d
float *GenerateDistanceFalloff(float exponent, float *output) {
    float *cursor = output;
    *cursor++ = 0.0f;
    std::int32_t row, column;
    for (row = 0, column = 1; row < 8; ++row) {
        for (; column < 8; ++column) {
            const float distance = ::sqrtf(static_cast<float>(row * row + column * column));
            *cursor++ = 1.0f / fast_powf_approx(distance, exponent);
        }
        column = 0;
    }
    return output;
}

// @verified - d
float *GenerateDistanceKernel(float exponent, float *output) {
    float *cursor = output;
    for (std::int32_t row = 0; row < 8; ++row) {
        for (std::int32_t column = 0; column < 8; ++column) {
            const float distance = ::sqrtf(static_cast<float>(row * row + column * column));
            *cursor++ = fast_powf_approx(distance, exponent);
        }
    }
    return output;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void *FillFftKernel(float scale, void *kernel) {
    float *const values = static_cast<float *>(kernel);
    values[0] = 0.0f;
    float *value = values + 1;
    std::int32_t row, column;
    for (row = 0, column = 1; row < 8; ++row, column = 0) {
        for (; column < 8; ++column, ++value) {
            const float scaled = *value * scale;
            *value = scaled;
            if (scaled < 9.999999747378752e-06f && scaled > -9.999999747378752e-06f)
                *value = 9.999999747378752e-06f;
        }
    }
    return kernel;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FillFftDistance(const void *kernel) {
    const float *distance = static_cast<const float *>(kernel);
    WaveBin *bin = waveState.bins;
    for (std::int32_t row = 0; row < 8; ++row)
        for (std::int32_t column = 0; column < 8; ++column, ++bin)
            bin->amplitude = *distance++;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyFft(const void *first, const void *second) {
    const float *speedA = static_cast<const float *>(first);
    const float *speedB = static_cast<const float *>(second);
    WaveBin *bin = waveState.bins;
    for (std::int32_t row = 0; row < 8; ++row) {
        for (std::int32_t column = 0; column < 8; ++column, ++bin) {
            bin->speedA = *speedA++;
            bin->speedB = *speedB++;
        }
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FinalizeFft(float scrollU, float scrollV) {
    waveState.scrollU = scrollU;
    waveState.scrollV = scrollV;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SetWaveLighting(float ambient, const Vector3 *direction) {
    g_waveLightAmbient = ambient;
    g_waveLightDirection = *direction;
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void FillWaveSpectrum(float *real, float *imaginary, const WaveBin *bins) {
    for (std::int32_t row = 0; row < 8; ++row) {
        for (std::int32_t column = 0; column < 8; ++column) {
            const WaveBin *const bin = &bins[row * 8 + column];
            const std::int32_t firstAngle = static_cast<std::int32_t>((bin->phaseA * 65536.0f) / 6.2831854820251465f);
            Vector2 rotation;
            CalculateFixedSinCos(static_cast<std::uint32_t>(firstAngle), &rotation);
            const float firstReal = rotation.y * bin->amplitude;
            const float firstImaginary = rotation.x * bin->amplitude;
            real[row * 17 + column] = firstReal;
            imaginary[row * 17 + column] = firstImaginary;
            real[(16 - row) * 17 + 16 - column] = firstReal;
            imaginary[(16 - row) * 17 + 16 - column] = -firstImaginary;
            const std::int32_t secondAngle = static_cast<std::int32_t>((bin->phaseB * 65536.0f) / 6.2831854820251465f);
            CalculateFixedSinCos(static_cast<std::uint32_t>(secondAngle), &rotation);
            const float secondReal = rotation.y * bin->amplitude;
            const float secondImaginary = rotation.x * bin->amplitude;
            real[row * 17 + 16 - column] = secondReal;
            imaginary[row * 17 + 16 - column] = secondImaginary;
            real[(16 - row) * 17 + column] = secondReal;
            imaginary[(16 - row) * 17 + column] = -secondImaginary;
        }
    }
    real[0] = imaginary[0] = 0;
    real[16] = imaginary[16] = 0;
    real[272] = imaginary[272] = 0;
    real[288] = imaginary[288] = 0;
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float EvaluateCurve(std::int32_t count, const MotionRigKey *keys, float time) {
    std::int32_t index = 0, kind = 3;
    for (; index < count; ++index, ++keys) {
        if (__builtin_fabsf(keys->timeSeconds - time) < 1.17549435e-38f) {
            kind = 0;
            break;
        }
        if (keys->timeSeconds > time) {
            kind = !index ? 1 : 2;
            break;
        }
    }
    return EvaluateCurveSegment(keys, kind, time);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float EvaluateCurveSegment(const MotionRigKey *key, std::int32_t kind, float time) {
    float value = 0.0f;
    switch (kind) {
    case 0:
        value = key->value;
        break;
    case 1:
        value = key->value;
        break;
    case 2:
        value = EvaluateCurveSegmentCubic(key - 1, key, time);
        break;
    case 3:
        value = key[-1].value;
        break;
    }
    return value;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float EvaluateCurveSegmentCubic(const MotionRigKey *previous, const MotionRigKey *key, float time) {
    const float end = key->timeSeconds, endValue = key->value, endTangent = key->tangentIn;
    const float start = previous->timeSeconds, startValue = previous->value, startTangent = previous->tangentOut;
    time -= start;
    const float inverse = 1.0f / (end - start), inverseSquared = inverse * inverse;
    const float squared = time * time, scaledSquared = squared * inverse;
    const float cubic = (squared * time) * inverseSquared;
    const float twiceCubic = (cubic + cubic) * inverse;
    const float tripleSquared = (squared * 3.0f) * inverseSquared;
    const float startWeight = (twiceCubic - tripleSquared) + 1.0f;
    float value = startValue * startWeight;
    value += endValue * (-twiceCubic + tripleSquared);
    const float incoming = cubic - scaledSquared;
    value += startTangent * ((incoming - scaledSquared) + time);
    value += endTangent * incoming;
    return value;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct MotionRigCurve {
    const std::int32_t *count;
    MotionRigKey *keys;
};

void ScaleCurve(MotionRigCurve *curve, float scale) {
    MotionRigKey *key = curve->keys;
    for (std::int32_t index = 0; index < *curve->count; ++key, ++index) {
        key->tangentIn *= scale;
        key->tangentOut *= scale;
        key->value *= scale;
    }
}

void OffsetCurve(MotionRigCurve *curve, float offset) {
    MotionRigKey *key = curve->keys;
    for (std::int32_t index = 0; index < *curve->count; ++key, ++index)
        key->value += offset;
}

void SwapCurveKeys(MotionRigCurve *first, MotionRigCurve *second) {
    MotionRigKey *a = first->keys, *b = second->keys;
    std::int32_t count;
    if (first->count > second->count)
        count = *first->count;
    else
        count = *second->count;
    for (std::int32_t index = 0; index < count; ++a, ++index, ++b) {
        const MotionRigKey key = *a;
        *a = *b;
        *b = key;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float EvaluateCurveSlope(std::int32_t count, const MotionRigKey *keys, float time) {
    std::int32_t index = 0, kind = 3;
    for (; index < count; ++index, ++keys) {
        if (__builtin_fabsf(keys->timeSeconds - time) < 1.17549435e-38f) {
            kind = 0;
            break;
        }
        if (keys->timeSeconds > time) {
            kind = !index ? 1 : 2;
            break;
        }
    }
    return EvaluateCurveSlopeSegment(keys, kind, time);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float EvaluateCurveSlopeSegment(const MotionRigKey *key, std::int32_t kind, float time) {
    float slope = 0.0f;
    switch (kind) {
    case 0:
        slope = (key->tangentIn + key->tangentOut) * 0.5f;
        break;
    case 1:
        slope = key->tangentOut;
        break;
    case 2:
        slope = EvaluateCurveSlopeCubic(key - 1, key, time);
        break;
    case 3:
        slope = key[-1].tangentIn;
        break;
    }
    return slope;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float EvaluateCurveSlopeCubic(const MotionRigKey *previous, const MotionRigKey *key, float time) {
    const float end = key->timeSeconds, endValue = key->value, endTangent = key->tangentIn;
    const float start = previous->timeSeconds, startValue = previous->value, startTangent = previous->tangentOut;
    time -= start;
    const float inverse = 1.0f / (end - start), inverseSquared = inverse * inverse;
    const float squared = time * time;
    const float three = (squared * 3.0f) * inverseSquared;
    const float six = ((squared * 6.0f) * inverseSquared) * inverse;
    const float linear = (6.0f * time) * inverseSquared;
    const float twice = (time + time) * inverse;
    const float startWeight = six - linear;
    float value = startValue * startWeight;
    value += endValue * (-six + linear);
    const float incoming = three - twice;
    value += startTangent * ((incoming - twice) + 1.0f);
    value += endTangent * incoming;
    return value;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void BuildWaveLightingTable(std::uint32_t flags, float ambient, float, float diffuseR, float diffuseG, float diffuseB, float specularR, float specularG,
    float specularB, float exponent, float intensity) {
    WaveLighting *lighting = &waveState.lighting;
    lighting->flags = flags;
    lighting->ambient = ambient;
    lighting->diffuse.x = diffuseR;
    lighting->diffuse.y = diffuseG;
    lighting->diffuse.z = diffuseB;
    if (flags & 1) {
        lighting->specular.x = specularR;
        lighting->specular.y = specularG;
        lighting->specular.z = specularB;
    }
    lighting->specularExponent = exponent;
    lighting->specularIntensity = intensity;
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateWaveSpectrum(WaveState *state) {
    std::int32_t i, j;
    Vector2 line[16];
    WaveBin *bin = state->bins;
    for (i = 0; i < 8; ++i) {
        for (j = 0; j < 8; ++j, ++bin) {
            bin->phaseA += bin->speedA;
            if (bin->phaseA > 3.1415927410125732f)
                bin->phaseA -= 6.2831854820251465f;
            else if (bin->phaseA < -3.1415927410125732f)
                bin->phaseA += 6.2831854820251465f;
            bin->phaseB += bin->speedB;
            if (bin->phaseB > 3.1415927410125732f)
                bin->phaseB -= 6.2831854820251465f;
            else if (bin->phaseB < -3.1415927410125732f)
                bin->phaseB += 6.2831854820251465f;
        }
    }
    float (*const real)[17] = state->real;
    float (*const imaginary)[17] = state->imaginary;
    FillWaveSpectrum(real[0], imaginary[0], state->bins);
    for (i = 0; i < 16; ++i) {
        for (j = 0; j < 16; ++j) {
            line[j].x = real[i][j];
            line[j].y = imaginary[i][j];
        }
        dc::sdk::FftInverse16(line);
        for (j = 0; j < 16; ++j) {
            real[i][j] = line[j].x;
            imaginary[i][j] = line[j].y;
        }
    }
    for (i = 0; i < 16; ++i) {
        for (j = 0; j < 16; ++j) {
            line[j].x = real[j][i];
            line[j].y = imaginary[j][i];
        }
        dc::sdk::FftInverse16(line);
        for (j = 0; j < 16; ++j) {
            real[j][i] = line[j].x;
            imaginary[j][i] = line[j].y;
        }
    }
    for (i = 0; i < 17; ++i) {
        real[i][16] = real[i][0];
        real[16][i] = real[0][i];
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateWaveNormals(WaveState *state) {
    float (*const real)[17] = state->real;
    WaveVertex(*const vertices)[17] = state->vertices;
    const std::uint32_t flags = state->lighting.flags;
    const float cell = state->lighting.cellSize;
    const float level = state->lighting.ambient;
    const float exponent = state->lighting.specularExponent;
    const float intensity = state->lighting.specularIntensity;
    const Vector3 direction = g_waveLightDirection;
    const float ambient = level * g_waveLightAmbient;
    Vector3 diffuse, specular;
    diffuse.x = state->lighting.diffuse.x;
    diffuse.y = state->lighting.diffuse.y;
    diffuse.z = state->lighting.diffuse.z;
    if (flags & 1) {
        specular.x = state->lighting.specular.x;
        specular.y = state->lighting.specular.y;
        specular.z = state->lighting.specular.z;
    }
    std::int32_t row, column, edge;
    std::uint32_t red, green, blue;
    for (row = 0; row < 16; ++row) {
        for (column = 0; column < 16; ++column) {
            Vector3 origin, right, down, first, second, normal;
            origin.x = static_cast<float>(column) * cell;
            origin.y = real[row][column];
            origin.z = static_cast<float>(row) * cell;
            right.x = origin.x + cell;
            right.y = real[row][column + 1];
            right.z = origin.z;
            down.x = origin.x;
            down.y = real[row + 1][column];
            down.z = origin.z + cell;
            first.x = right.x - origin.x;
            first.y = right.y - origin.y;
            first.z = right.z - origin.z;
            second.x = down.x - origin.x;
            second.y = down.y - origin.y;
            second.z = down.z - origin.z;
            am2Vector3Cross(&second, &first, &normal);
            am2Vector3NormalizeInPlace(&normal);
            float shade = ambient;
            const float dot = am2Vector3Dot(&normal, &direction);
            if (dot > 0)
                shade += dot;
            vertices[row][column].height = origin.y;
            const std::uint32_t normalX = static_cast<std::uint8_t>(static_cast<std::int32_t>(normal.x * 127.0f));
            const std::uint32_t normalY = static_cast<std::uint8_t>(static_cast<std::int32_t>(normal.y * 127.0f));
            const std::uint32_t normalZ = static_cast<std::uint8_t>(static_cast<std::int32_t>(normal.z * 127.0f));
            vertices[row][column].normal = (normalX << 16) | (normalY << 8) | normalZ;
            if (shade > 1)
                shade = 1;
            shade *= 255.0f;
            red = static_cast<std::uint8_t>(static_cast<std::int32_t>(diffuse.x * shade));
            green = static_cast<std::uint8_t>(static_cast<std::int32_t>(diffuse.y * shade));
            blue = static_cast<std::uint8_t>(static_cast<std::int32_t>(diffuse.z * shade));
            vertices[row][column].diffuse = 0xff000000U | (red << 16) | (green << 8) | blue;
            if ((flags & 1) == 1) {
                float highlight = dot > 0 ? fast_powf_approx(dot, exponent) : 0.0f;
                if (highlight > 1)
                    highlight = 1;
                highlight *= 255.0f * intensity;
                red = static_cast<std::uint8_t>(static_cast<std::int32_t>(specular.x * highlight));
                green = static_cast<std::uint8_t>(static_cast<std::int32_t>(specular.y * highlight));
                blue = static_cast<std::uint8_t>(static_cast<std::int32_t>(specular.z * highlight));
                vertices[row][column].specular = 0xff000000U | (red << 16) | (green << 8) | blue;
            } else
                vertices[row][column].specular = 0;
        }
        vertices[row][16].height = vertices[row][0].height;
        vertices[row][16].normal = vertices[row][0].normal;
        vertices[row][16].diffuse = vertices[row][0].diffuse;
        vertices[row][16].specular = vertices[row][0].specular;
    }
    for (edge = 0; edge < 17; ++edge) {
        vertices[16][edge].height = vertices[0][edge].height;
        vertices[16][edge].normal = vertices[0][edge].normal;
        vertices[16][edge].diffuse = vertices[0][edge].diffuse;
        vertices[16][edge].specular = vertices[0][edge].specular;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#define PUBLISH_WAVE_VERTEX(packet, vertex)                                                                                                                    \
    {                                                                                                                                                          \
        output = reinterpret_cast<WaveMeshVertex *>(packet);                                                                                                   \
        words = reinterpret_cast<std::uint32_t *>(packet);                                                                                                     \
        output->position.y = (vertex).height;                                                                                                                  \
        output->normal = (vertex).normal;                                                                                                                      \
        output->diffuse = (vertex).diffuse;                                                                                                                    \
        output->specular = (vertex).specular;                                                                                                                  \
        output->u += u;                                                                                                                                        \
        if (output->u < 0.0f)                                                                                                                                  \
            uClass |= 0x100;                                                                                                                                   \
        else if (output->u < 1.0f)                                                                                                                             \
            uClass |= 0x10;                                                                                                                                    \
        else                                                                                                                                                   \
            uClass |= 1;                                                                                                                                       \
        output->v += v;                                                                                                                                        \
        if (output->v < 0.0f)                                                                                                                                  \
            vClass |= 0x100;                                                                                                                                   \
        else if (output->v < 1.0f)                                                                                                                             \
            vClass |= 0x10;                                                                                                                                    \
        else                                                                                                                                                   \
            vClass |= 1;                                                                                                                                       \
        words[7] |= 1;                                                                                                                                         \
    }

// @verified - d
void PublishWaveMesh(WaveState *state, void *mesh) {
    WaveVertex(*const vertices)[17] = state->vertices;
    float u = state->scrollU, v = state->scrollV;
    std::byte *packet = static_cast<std::byte *>(mesh);
    std::uint32_t *const flags = reinterpret_cast<std::uint32_t *>(packet + 4);
    std::uint32_t uClass, vClass;
    std::int32_t row;
    if (*flags & 0x200)
        u += (*flags & 0x100) ? 1.0f : -1.0f;
    if (*flags & 0x2000)
        v += (*flags & 0x1000) ? 1.0f : -1.0f;
    uClass = vClass = 0;
    *flags &= 0xffff00ffU;
    packet += 0x18;
    reinterpret_cast<std::uint32_t *>(packet)[2] &= 0xfffe7fffU;
    packet += 0x58;
    for (std::int32_t column = 0; column < 17; ++column) {
        WaveMeshVertex *output;
        std::uint32_t *words;
        PUBLISH_WAVE_VERTEX(packet, vertices[7][16 - column]);
        packet += 0x20;
        PUBLISH_WAVE_VERTEX(packet, vertices[8][16 - column]);
        packet += 0x20;
    }
    for (row = 1; row < 7; ++row) {
        WaveMeshVertex *output;
        std::uint32_t *words;
        packet += 8;
        for (std::int32_t column = 0; column < 17; ++column) {
            PUBLISH_WAVE_VERTEX(packet, vertices[row + 8][column]);
            packet += 0x28;
        }
        packet += 8;
        for (std::int32_t column = 0; column < 17; ++column) {
            PUBLISH_WAVE_VERTEX(packet, vertices[7 - row][16 - column]);
            packet += 0x28;
        }
    }
    for (row = 15; row < 17; ++row) {
        WaveMeshVertex *output;
        std::uint32_t *words;
        packet += 8;
        for (std::int32_t column = 0; column < 17; ++column) {
            PUBLISH_WAVE_VERTEX(packet, vertices[row][column]);
            packet += 0x28;
        }
    }
    packet += 8;
    for (std::int32_t column = 0; column < 17; ++column) {
        WaveMeshVertex *output;
        std::uint32_t *words;
        packet += 8;
        PUBLISH_WAVE_VERTEX(packet, vertices[0][column]);
        packet += 0x20;
    }
    if (uClass == 0x100)
        *flags |= 0x300;
    else if (uClass == 1)
        *flags |= 0x200;
    if (vClass == 0x100)
        *flags |= 0x3000;
    else if (vClass == 1)
        *flags |= 0x2000;
}
#undef PUBLISH_WAVE_VERTEX
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyWaveformToMesh(void *mesh) {
    UpdateWaveSpectrum(&waveState);
    UpdateWaveNormals(&waveState);
    PublishWaveMesh(&waveState, mesh);
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyWaveNode(WaveState *state, MT7Node *node) {
    if (!(node->kind & 0x10) && (node->mesh || node->child)) {
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->mesh)
            PublishWaveMesh(state, node->mesh);
        if (node->child)
            ApplyWaveNode(state, node->child);
        PopCurrentMatrix(1);
    }
    if (node->sibling)
        ApplyWaveNode(state, node->sibling);
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyWaveformToNode(MT7Node *node) {
    UpdateWaveSpectrum(&waveState);
    UpdateWaveNormals(&waveState);
    ApplyWaveNode(&waveState, node);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ApplyCurrentWaveformToNode(MT7Node *node) {
    ApplyWaveNode(&waveState, node);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyWaveformToModel(MT7ModelChunk *model) {
    WaveState *state = &waveState;
    MT7ModelChunk *chunk = model;
    if (chunk->name == SHEN_NAME4('M', 'D', 'L', '7') || chunk->name == SHEN_NAME4('M', 'D', 'O', '7')) {
        UpdateWaveSpectrum(state);
        UpdateWaveNormals(state);
        ApplyWaveNode(state, chunk->root);
    }
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void PrepareWaveReflectionColours(WaveState *state) {
    const Vector3 diffuse = state->lighting.diffuse;
    const Vector3 specular = state->lighting.specular;
    const std::uint32_t diffuseRed = static_cast<std::uint8_t>(static_cast<std::int32_t>(diffuse.x * 255.0f));
    const std::uint32_t diffuseGreen = static_cast<std::uint8_t>(static_cast<std::int32_t>(diffuse.y * 255.0f));
    const std::uint32_t diffuseBlue = static_cast<std::uint8_t>(static_cast<std::int32_t>(diffuse.z * 255.0f));
    state->reflectionDiffuse = 0xff000000U | (diffuseRed << 16) | (diffuseGreen << 8) | diffuseBlue;
    const std::uint32_t specularRed = static_cast<std::uint8_t>(static_cast<std::int32_t>((specular.x * 255.0f) * 0.4f));
    const std::uint32_t specularGreen = static_cast<std::uint8_t>(static_cast<std::int32_t>((specular.y * 255.0f) * 0.4f));
    const std::uint32_t specularBlue = static_cast<std::uint8_t>(static_cast<std::int32_t>((specular.z * 255.0f) * 0.4f));
    state->reflectionSpecular = 0xff000000U | (specularRed << 16) | (specularGreen << 8) | specularBlue;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyWaveReflectionMesh(WaveState *state, void *mesh) {
    const std::uint32_t diffuse = state->reflectionDiffuse, specular = state->reflectionSpecular;
    std::uint32_t *packet = static_cast<std::uint32_t *>(mesh) + 6;
    for (;;) {
        std::uint32_t flags = *packet;
        if (!flags)
            break;
        if (flags & 0x80000000U) {
            packet[2] &= 0xfffe7fffU;
            packet += 20;
            flags = *packet++;
        } else {
            ++packet;
        }
        std::uint32_t count = *packet++;
        if (flags & 8U)
            count *= 3;
        while (count-- > 0) {
            if (*packet & 1U) {
                packet[4] = diffuse;
                packet[5] = specular;
                packet += 8;
            } else {
                packet += 2;
            }
        }
    }
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyWaveReflectionToMesh(void *mesh) {
    PrepareWaveReflectionColours(&waveState);
    ApplyWaveReflectionMesh(&waveState, mesh);
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyWaveReflectionTree(WaveState *state, MT7Node *node) {
    if (!(node->kind & 0x10) && (node->mesh || node->child)) {
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->mesh)
            ApplyWaveReflectionMesh(state, node->mesh);
        if (node->child)
            ApplyWaveReflectionTree(state, node->child);
        PopCurrentMatrix(1);
    }
    if (node->sibling)
        ApplyWaveReflectionTree(state, node->sibling);
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ApplyWaveReflectionToNode(MT7Node *node) {
    PrepareWaveReflectionColours(&waveState);
    ApplyWaveReflectionTree(&waveState, node);
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#define READ_WATER_SURFACE_POINT(position, packet)                                                                                                             \
    {                                                                                                                                                          \
        Vector3 *const target = &(position);                                                                                                                   \
        target->x = reinterpret_cast<const Vector3 *>(packet)->x;                                                                                              \
        target->z = reinterpret_cast<const Vector3 *>(packet)->z;                                                                                              \
    }
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ReadWaterSurfaceGrid(void *mesh) {
    std::int32_t row;
    const std::byte *packet = static_cast<std::byte *>(mesh) + 0x70;
    for (std::int32_t column = 0; column < 17; ++column) {
        READ_WATER_SURFACE_POINT(waterSurfacePositions[7][16 - column], packet);
        packet += 0x20;
        READ_WATER_SURFACE_POINT(waterSurfacePositions[8][16 - column], packet);
        packet += 0x20;
    }
    for (row = 1; row < 7; ++row) {
        packet += 8;
        for (std::int32_t column = 0; column < 17; ++column, packet += 0x28)
            READ_WATER_SURFACE_POINT(waterSurfacePositions[row + 8][column], packet);
        packet += 8;
        for (std::int32_t column = 0; column < 17; ++column, packet += 0x28)
            READ_WATER_SURFACE_POINT(waterSurfacePositions[7 - row][16 - column], packet);
    }
    for (row = 15; row < 17; ++row) {
        packet += 8;
        for (std::int32_t column = 0; column < 17; ++column, packet += 0x28)
            READ_WATER_SURFACE_POINT(waterSurfacePositions[row][column], packet);
    }
    packet += 8;
    for (std::int32_t column = 0; column < 17; ++column) {
        packet += 8;
        READ_WATER_SURFACE_POINT(waterSurfacePositions[0][column], packet);
        packet += 0x20;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CollectWaterSurfaceCandidates(MT7Node *node) {
    if (node->mesh || node->child) {
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->mesh) {
            Vector3 local;
            matrix_stack::Matrix4x4_Push(0);
            Matrix4x4::InvertRigidTransform();
            Matrix4x4::TransformPoint(waterQueryPosition, local);
            PopCurrentMatrix(1);
            const float distance = am2Vector3Length(&local);
            std::uint32_t positionX, positionY, positionZ;
            std::memcpy(&positionX, &local.x, sizeof(positionX));
            std::memcpy(&positionY, &local.y, sizeof(positionY));
            std::memcpy(&positionZ, &local.z, sizeof(positionZ));
            for (std::int32_t index = 0; index < 4; ++index) {
                if (distance < waterCandidates[index].distance) {
                    for (std::int32_t tail = 3; index < tail; --tail)
                        waterCandidates[tail] = waterCandidates[tail - 1];
                    waterCandidates[index].distance = distance;
                    WaterSurfaceCandidate *candidate = &waterCandidates[index];
                    std::memcpy(&candidate->position.x, &positionX, sizeof(positionX));
                    std::memcpy(&candidate->position.y, &positionY, sizeof(positionY));
                    std::memcpy(&candidate->position.z, &positionZ, sizeof(positionZ));
                    waterCandidates[index].node = node;
                    break;
                }
            }
        }
        if (node->child)
            CollectWaterSurfaceCandidates(node->child);
        PopCurrentMatrix(1);
    }
    if (node->sibling)
        CollectWaterSurfaceCandidates(node->sibling);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateFloatingSurfaceNode(MT7Node *node) {
    if (node->mesh) {
        const float sine = dc::sdk::SinFixed(node->rotation.y), cosine = dc::sdk::CosFixed(node->rotation.y);
        Vector3 sample0, sample1, sample2;
        sample0.x = cosine * floatingSurfaceSample0.x + sine * floatingSurfaceSample0.z;
        sample0.z = -sine * floatingSurfaceSample0.x + cosine * floatingSurfaceSample0.z;
        sample0.x += node->position.x;
        sample0.z += node->position.z;
        sample0.x -= floatingSurfaceOriginX;
        sample0.z -= floatingSurfaceOriginZ;
        waterSurfaceQuery = sample0;
        waterSurfaceCell = -1;
        SampleWaterSurfaceHeight();
        sample0.y = waterSurfaceQuery.y + floatingSurfaceOriginY;
        sample1.x = cosine * floatingSurfaceSample1.x + sine * floatingSurfaceSample1.z;
        sample1.z = -sine * floatingSurfaceSample1.x + cosine * floatingSurfaceSample1.z;
        sample1.x += node->position.x;
        sample1.z += node->position.z;
        sample1.x -= floatingSurfaceOriginX;
        sample1.z -= floatingSurfaceOriginZ;
        waterSurfaceQuery = sample1;
        waterSurfaceCell = -1;
        SampleWaterSurfaceHeight();
        sample1.y = waterSurfaceQuery.y + floatingSurfaceOriginY;
        sample2.x = cosine * floatingSurfaceSample2.x + sine * floatingSurfaceSample2.z;
        sample2.z = -sine * floatingSurfaceSample2.x + cosine * floatingSurfaceSample2.z;
        sample2.x += node->position.x;
        sample2.z += node->position.z;
        sample2.x -= floatingSurfaceOriginX;
        sample2.z -= floatingSurfaceOriginZ;
        waterSurfaceQuery = sample2;
        waterSurfaceCell = -1;
        SampleWaterSurfaceHeight();
        sample2.y = waterSurfaceQuery.y + floatingSurfaceOriginY;
        if (floatingSurfaceNodeCount < 30) {
            const std::int16_t pitch = static_cast<std::int16_t>(
                dc::sdk::ArcTan2Fixed((sample1.y + sample2.y) * 0.5f - sample0.y, floatingSurfaceSample0.z - floatingSurfaceSample1.z));
            const float rollX = floatingSurfaceSample1.x - floatingSurfaceSample2.x;
            const std::int16_t roll = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(sample1.y - sample2.y, rollX));
            node->rotation.z = (floatingSurfacePreviousAngles[floatingSurfaceNodeCount][0] + roll) / 2;
            node->rotation.x = (floatingSurfacePreviousAngles[floatingSurfaceNodeCount][1] + pitch) / 2;
            node->position.y = (sample0.y + sample1.y + sample2.y) / 3.0f;
            ++floatingSurfaceNodeCount;
        }
    }
    if (node->child)
        UpdateFloatingSurfaceNode(node->child);
    if (node->sibling)
        UpdateFloatingSurfaceNode(node->sibling);
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
float FindWaterSurfaceHeight(Vector3 position, MT7Node *root) {
    std::int32_t i;
    for (i = 0; i < 4; ++i)
        waterCandidates[i].distance = 9999.0f;
    waterQueryPosition = position;
    matrix_stack::PushUnitMatrix();
    CollectWaterSurfaceCandidates(root);
    PopCurrentMatrix(1);
    for (i = 0; i < 4; ++i) {
        ReadWaterSurfaceGrid(waterCandidates[i].node->mesh);
        const WaterSurfaceCandidate *candidate = &waterCandidates[i];
        waterSurfaceQuery = candidate->position;
        waterSurfaceCell = -1;
        if (!ContainsWaterSurfaceQuad(0, 16))
            continue;
        SampleWaterSurfaceHeight();
        MT7Node *const node = waterCandidates[i].node;
        matrix_stack::PushUnitMatrix();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        Vector3 world;
        Matrix4x4::TransformPoint(waterSurfaceQuery, world);
        PopCurrentMatrix(1);
        return world.y;
    }
    return 9999.0f;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateFloatingSurfaceModels(std::int32_t surfaceSlot, const FloatingSurfaceBinding *bindings) {
    MT7Node *const surface = GetMapModelRoot(surfaceSlot);
    ReadWaterSurfaceGrid(surface->child->mesh);
    for (const FloatingSurfaceBinding *binding = bindings; binding->slot >= 0; ++binding) {
        floatingSurfaceOriginX = binding->origin.x;
        floatingSurfaceOriginY = binding->origin.y;
        floatingSurfaceOriginZ = binding->origin.z;
        floatingSurfaceNodeCount = 0;
        UpdateFloatingSurfaceNode(GetMapModelRoot(binding->slot));
    }
}
#endif

// @verified - d
void InitializeDefaultCharacterShadow(Character *character) {
    character->shadowModel = GetSHDS();
    charaSetDisplayEnabledFlag(character, 0);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void DrawMapModifierVolumes(void *) {
    g_shadowStrengthLimit = 255;
    if (g_shadowVolumesDisabled || !g_shadowRenderingEnabled || GetCurrentActiveLight() != 1 || GetSelectedLightKind(0) != 2)
        return;
    std::int32_t placementIndex;
    std::int32_t strength = static_cast<std::int32_t>((1.0f - g_shadowOpacityScale) * 255.0f);
    if (strength < 0)
        strength = 0;
    else if (strength > 254)
        return;
    Vector3 direction, position;
    Matrix4x4 transform;
    for (placementIndex = 0; g_mapShadowPlacements[placementIndex].shapeIndex >= 0; ++placementIndex) {
        matrix_stack::PushUnitMatrix();
        const float positionX = position.x = g_mapShadowPlacements[placementIndex].position.x;
        const float positionY = position.y = g_mapShadowPlacements[placementIndex].position.y;
        const float positionZ = position.z = g_mapShadowPlacements[placementIndex].position.z;
        Matrix4x4::preTranslate(positionX, positionY, positionZ);
        Matrix4x4::SetViewMatrixFromEuler(static_cast<std::int32_t>(g_mapShadowPlacements[placementIndex].rotation.x * 8192.0f / 45.0f) & 0xffff,
            static_cast<std::int32_t>(g_mapShadowPlacements[placementIndex].rotation.y * 8192.0f / 45.0f) & 0xffff,
            static_cast<std::int32_t>(g_mapShadowPlacements[placementIndex].rotation.z * 8192.0f / 45.0f) & 0xffff);
        matrix_stack::GetMatrix(&transform);
        matrix_stack::GetPreviousMatrix(0);
        GetSelectedLightDirection(0, &direction);
        am2Vector3NormalizeInPlace(&direction);
        const float worldY = direction.y;
        Matrix4x4::Transform(direction, direction);
        g_shadowDrawParameters.lightDirection.x = -direction.x;
        g_shadowDrawParameters.lightDirection.y = -direction.y;
        g_shadowDrawParameters.lightDirection.z = -direction.z;
        g_shadowDrawParameters.flags &= ~1U;
        g_shadowDrawParameters.shadowStrength = strength;
        float length;
        if (worldY < 0.0f) {
            length = shadowVolumeNominalHeight / -worldY;
            if (length > shadowVolumeMaximumLength)
                length = shadowVolumeMaximumLength;
        } else
            length = shadowVolumeMaximumLength;
        g_shadowDrawParameters.length = length;
        ShadowShapeNode *const shape = g_mapShadowShapes[g_mapShadowPlacements[placementIndex].shapeIndex];
        ShadowVolume *const volume = reinterpret_cast<ShadowVolume *>(&shape->data[2]);
        Vector3 *vertices = static_cast<Vector3 *>(shape->data[0]);
        Vector3 *transformedVertices = volume->vertices;
        Vector3 *normals = static_cast<Vector3 *>(shape->data[1]);
        ShadowVolumeFace *transformedFaces = volume->faces;
        matrix_stack::LoadMatrix(&transform);
        std::int32_t index;
        for (index = 0; index < volume->vertexCount; ++index)
            Matrix4x4::TransformPoint(*vertices++, *transformedVertices++);
        for (index = 0; index < volume->faceCount; ++index)
            Matrix4x4::Transform(*normals++, (transformedFaces++)->normal);
        matrix_stack::GetPreviousMatrix(0);
        g_mapShadowVolumes[0] = volume;
        DrawShadowVolumes(g_mapShadowVolumes, &g_shadowDrawParameters);
        g_shadowStrengthLimit = strength;
        PopCurrentMatrix(1);
    }
}
#else
// @verified - d
void DrawMapModifierVolumes(void *) {
    g_shadowStrengthLimit = 255;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    if (g_shadowVolumesDisabled || !g_shadowRenderingEnabled || GetCurrentActiveLight() != 1 || GetSelectedLightKind(0) != 2)
#else
    if (g_shadowVolumesDisabled || !g_worldDrawingEnabled || GetCurrentActiveLight() != 1 || GetSelectedLightKind(0) != 2)
#endif
        return;
    std::int32_t strength = static_cast<std::int32_t>((1.0f - g_shadowOpacityScale) * 255.0f);
    if (strength < 0)
        strength = 0;
    else if (strength > 254)
        return;
    const std::byte *placementData = reinterpret_cast<const std::byte *>(&g_mapShadowStorage) + offsetof(MapShadowStorage, placements);
    std::int32_t shapeIndex;
    std::memcpy(&shapeIndex, placementData, sizeof(shapeIndex));
    while (shapeIndex >= 0) {
        const MapShadowPlacement *const placement = reinterpret_cast<const MapShadowPlacement *>(placementData);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::preTranslate(placement->position.x, placement->position.y, placement->position.z);
        const std::uint16_t first = static_cast<std::uint16_t>(static_cast<std::int32_t>((placement->rotation.x * 8192.0f) / 45.0f));
        const std::uint16_t second = static_cast<std::uint16_t>(static_cast<std::int32_t>((placement->rotation.y * 8192.0f) / 45.0f));
        const std::uint16_t third = static_cast<std::uint16_t>(static_cast<std::int32_t>((placement->rotation.z * 8192.0f) / 45.0f));
        Matrix4x4::SetViewMatrixFromEuler(second, first, third);
        Matrix4x4 transform;
        matrix_stack::GetMatrix(&transform);
        matrix_stack::GetPreviousMatrix(0);
        Vector3 direction;
        GetSelectedLightDirection(0, &direction);
        dc::sdk::njUnitVector(&direction);
        const float worldY = direction.y;
        dc::sdk::njCalcVector(0, &direction, &direction);
        g_shadowDrawParameters.lightDirection =
            (__typeof__(g_shadowDrawParameters.lightDirection)){SHEN_FIELD(x) - direction.x, SHEN_FIELD(y) - direction.y, SHEN_FIELD(z) - direction.z};
        g_shadowDrawParameters.flags &= ~1U;
        g_shadowDrawParameters.shadowStrength = strength;
        float length = shadowVolumeMaximumLength;
        if (worldY < 0.0f) {
            length = shadowVolumeNominalHeight / -worldY;
            if (length > shadowVolumeMaximumLength)
                length = shadowVolumeMaximumLength;
        }
        g_shadowDrawParameters.length = length;
        ShadowShapeNode *const shape = g_mapShadowStorage.shapes[shapeIndex];
        ShadowVolume *const volume = reinterpret_cast<ShadowVolume *>(&shape->data[2]);
        Vector3 *const sourceVertices = static_cast<Vector3 *>(shape->data[0]);
        Vector3 *const sourceNormals = static_cast<Vector3 *>(shape->data[1]);
        matrix_stack::LoadMatrix(&transform);
        for (std::int32_t index = 0; index < volume->vertexCount; ++index)
            dc::sdk::njCalcPoint(0, &sourceVertices[index], &volume->vertices[index]);
        for (std::int32_t index = 0; index < volume->faceCount; ++index)
            dc::sdk::njCalcVector(0, &sourceNormals[index], &volume->faces[index].normal);
        matrix_stack::GetPreviousMatrix(0);
        g_mapShadowVolumes[0] = volume;
        DrawShadowVolumes(g_mapShadowVolumes, &g_shadowDrawParameters);
        g_shadowStrengthLimit = strength;
        PopCurrentMatrix(1);
        placementData += sizeof(MapShadowPlacement);
        std::memcpy(&shapeIndex, placementData, sizeof(shapeIndex));
    }
}
#endif

// @verified - d
void MapModifierVolumeTask_Callback(void *) {
    HLib::ExecuteCallbackNow(DrawMapModifierVolumes, 0);
}

// @verified - d
void SetupModelForImageModule(MT7Node *root) {
    if (root->mesh) {
        std::byte *cursor = static_cast<std::byte *>(root->mesh);
        if (*reinterpret_cast<const std::int32_t *>(cursor) != -1) {
            cursor += 0x18;
            do {
                MT7MeshPrimitive *primitive = reinterpret_cast<MT7MeshPrimitive *>(cursor);
                cursor += sizeof(MT7MeshPrimitive);
                const std::uint32_t flags = primitive->flags;
                if ((flags & 0xc0U) != 0xc0U)
                    primitive->flags = (flags & ~0xc0U) | 0x80U;
                cursor += primitive->dataBytes;
            } while (reinterpret_cast<MT7MeshPrimitive *>(cursor)->flags);
        }
    }
    if (root->child)
        SetupModelForImageModule(root->child);
    if (root->sibling)
        SetupModelForImageModule(root->sibling);
}

namespace {

// @verified - d
void RelocateShadowShape(ShadowShapeNode *node, std::uintptr_t base) {
    node->data[0] = reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(node->data[0]) + base);
    node->data[1] = reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(node->data[1]) + base);
    node->data[2] = reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(node->data[2]) + base);
    node->data[3] = reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(node->data[3]) + base);
    node->data[4] = reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(node->data[4]) + base);
    if (node->child) {
        node->child = reinterpret_cast<ShadowShapeNode *>(reinterpret_cast<std::uintptr_t>(node->child) + base);
        RelocateShadowShape(node->child, base);
    }
    if (node->sibling) {
        node->sibling = reinterpret_cast<ShadowShapeNode *>(reinterpret_cast<std::uintptr_t>(node->sibling) + base);
        RelocateShadowShape(node->sibling, base);
    }
}

} // namespace

// @verified - d
MT7Node *FindCachedShadowModel(const char *filename) {
    for (std::int32_t index = 0; index < g_shadowModelCacheCount; ++index) {
        if (!std::strcmp(g_shadowModelCache[index].filename, filename))
            return g_shadowModelCache[index].model;
    }
    return 0;
}

// @verified - d
MT7Node *CreateShadowModel(const char *name) {
    char directory[64];
    char filename[16];
    std::int32_t skip;
    if (name[0] == '@') {
        skip = 1;
        std::sprintf(directory, "scene/%02d/model", static_cast<int>(g_actNumber));
    } else if (name[0] == '$') {
        skip = 1;
        const std::uint32_t map = static_cast<std::uint32_t>(g_currentMap);
        std::sprintf(directory, "scene/%02d/%c%c%c%c", static_cast<int>(g_actNumber), static_cast<signed char>(map), static_cast<signed char>(map >> 8),
            static_cast<signed char>(static_cast<std::int32_t>(map << 8) >> 24), static_cast<signed char>(static_cast<std::int32_t>(map) >> 24));
    } else {
        skip = 0;
        std::sprintf(directory, "/model/chara");
    }
    std::sprintf(filename, "%s.MT7", name + skip);
    MT7Node *model;
    if (GetPakLoadParams()) {
        model = LoadGameModel(pakLoadGetNext(), filename)->root;
    } else {
        MT7Node *cached = 0;
        if (g_shadowModelCacheCount)
            cached = FindCachedShadowModel(filename);
        if (!cached) {
            void *const data = shenLoadFile(directory, filename,
                ('S' << 24) | (static_cast<std::uint8_t>(name[skip + 2]) << 16) | (static_cast<std::uint8_t>(name[skip + 1]) << 8) |
                    static_cast<std::uint8_t>(name[skip]));
            model = ParseMT7ModelRoot(data, 0);
            if (g_shadowModelCacheCount < 10) {
                g_shadowModelCache[g_shadowModelCacheCount].model = model;
                for (std::int32_t index = 0; index < 16; ++index)
                    g_shadowModelCache[g_shadowModelCacheCount].filename[index] = filename[index];
                ++g_shadowModelCacheCount;
            }
        } else
            model = cached;
    }
    return model;
}

// @verified - d
MT7Node *GetSHDS() {
    return g_sydRoot;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void LoadShadowSprite() {
    g_shadowModelCacheCount = 0;
    ShadowModelCacheEntry *entry = g_shadowModelCache;
    for (std::int32_t index = 0; index < 10; ++index, ++entry) {
        entry->model = 0;
        for (std::int32_t character = 0; character < 16; ++character)
            entry->filename[character] = 0;
    }
    void *const data = shenLoadFile("misc", "KAGE200G.MT7", SHADOW_SPRITE_MEMORY);
    g_shadowSprite = data ? ParseMT7ModelRoot(data, 0) : 0;
}
#endif

// @verified - d
void *LoadSYD() {
    void *const source = shenLoadFile("misc", "SYD_S.MT7", SHARED_SHADOW_MEMORY);
    HLib::SetTaskDataMotionFlag(source);
    g_sydRoot = ParseMT7ModelRoot(source, 0);
    return source;
}

// @verified - d
ShadowShapeNode *ReadShadowShape(ShadowShapeHeader *header) {
    if (!header->relocated) {
        const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(header);
        header->root = reinterpret_cast<ShadowShapeNode *>(reinterpret_cast<std::uintptr_t>(header->root) + base);
        RelocateShadowShape(header->root, base);
        header->relocated = 1;
    }
    return header->root;
}

// @verified - d
ShadowShapeNode *LoadShadowShape(const char *name) {
    ShadowShapeHeader *data = 0;
    char directory[16];
    char filename[16];
    std::sprintf(directory, "misc");
    std::sprintf(filename, "%s.MVS", name);
    std::int32_t count = g_shadowShapeCacheCount;
    if (count > 8)
        count = 8;
    std::int32_t index;
    for (index = 0; index < count; ++index) {
        if (!std::strcmp(g_shadowShapeCache[index].name, filename))
            data = static_cast<ShadowShapeHeader *>(g_shadowShapeCache[index].data);
    }
    if (!data) {
        const std::int32_t quotient = g_shadowShapeCacheCount / 10;
        const char tens = static_cast<char>(quotient + '0');
        const char ones = static_cast<char>(g_shadowShapeCacheCount + '0' - quotient * 10);
        data = static_cast<ShadowShapeHeader *>(!std::strcmp(name, "SYD_MUNE") ?
                CopyColdArchiveSection(
                    6, false, static_cast<std::uint32_t>(ones) << 24 | static_cast<std::uint32_t>(static_cast<std::uint8_t>(tens)) << 16 | 'V' << 8 | 'M') :
                shenLoadFile(directory, filename,
                    static_cast<std::uint32_t>(ones) << 24 | static_cast<std::uint32_t>(static_cast<std::uint8_t>(tens)) << 16 | 'V' << 8 | 'M'));
        if (!data)
            return 0;
        HLib::SetTaskDataMotionFlag(data);
        if (index < 8) {
            g_shadowShapeCache[index].data = data;
            std::strcpy(g_shadowShapeCache[index].name, filename);
        }
        ++g_shadowShapeCacheCount;
    }
    return ReadShadowShape(data);
}

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool ContainsWaterSurfaceQuad(std::int32_t first, std::int32_t stride) {
    if (waterSurfaceCell != -1)
        return false;
    Vector3 a = waterSurfacePositions[0][first], b = *(waterSurfacePositions[0] + first + stride), c, d, edge, relative;
    float cross[4];
    am2Vector3Subtract(&edge, &b, &a);
    am2Vector3Subtract(&relative, &waterSurfaceQuery, &a);
    cross[0] = edge.z * relative.x - edge.x * relative.z;
    c = *(waterSurfacePositions[0] + first + 18 * stride);
    am2Vector3Subtract(&edge, &c, &b);
    am2Vector3Subtract(&relative, &waterSurfaceQuery, &b);
    cross[1] = edge.z * relative.x - edge.x * relative.z;
    d = *(waterSurfacePositions[0] + first + 17 * stride);
    am2Vector3Subtract(&edge, &d, &c);
    am2Vector3Subtract(&relative, &waterSurfaceQuery, &c);
    cross[2] = edge.z * relative.x - edge.x * relative.z;
    am2Vector3Subtract(&edge, &a, &d);
    am2Vector3Subtract(&relative, &waterSurfaceQuery, &d);
    cross[3] = edge.z * relative.x - edge.x * relative.z;
    if (cross[0] >= 0.0f && cross[1] >= 0.0f && cross[2] >= 0.0f && cross[3] >= 0.0f)
        return true;
    if (cross[0] <= 0.0f && cross[1] <= 0.0f && cross[2] <= 0.0f && cross[3] <= 0.0f)
        return true;
    return false;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SelectWaterSurfaceCell(std::int32_t first, std::int32_t stride) {
    if (waterSurfaceCell != -1)
        return;
    if (stride == 1) {
        waterSurfaceCell = first;
        return;
    }
    stride >>= 1;
    if (ContainsWaterSurfaceQuad(first, stride))
        SelectWaterSurfaceCell(first, stride);
    if (ContainsWaterSurfaceQuad(first + stride, stride))
        SelectWaterSurfaceCell(first + stride, stride);
    if (ContainsWaterSurfaceQuad(first + 17 * stride + stride, stride))
        SelectWaterSurfaceCell(first + 17 * stride + stride, stride);
    if (ContainsWaterSurfaceQuad(first + 17 * stride, stride))
        SelectWaterSurfaceCell(first + 17 * stride, stride);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void SampleWaterSurfaceHeight() {
    WaveState *state = &waveState;
    waterSurfaceCell = -1;
    SelectWaterSurfaceCell(0, 16);
    if (waterSurfaceCell == -1) {
        for (std::int32_t row = 0; row < 17; ++row)
            for (std::int32_t column = 0; column < 17; ++column)
                if (ContainsWaterSurfaceQuad(column + row * 17, 1) == 1) {
                    waterSurfaceCell = column + row * 17;
                    goto found;
                }
    }
found:
    Vector3 origin = *(waterSurfacePositions[0] + waterSurfaceCell), first = *(waterSurfacePositions[0] + waterSurfaceCell + 1);
    Vector3 far = *(waterSurfacePositions[0] + waterSurfaceCell + 18), second = *(waterSurfacePositions[0] + waterSurfaceCell + 17);
    Vector3 a, b, normal, nearFlat, farFlat;
    origin.y = (state->vertices[0] + waterSurfaceCell)->height;
    first.y = (state->vertices[0] + (waterSurfaceCell + 1))->height;
    far.y = (state->vertices[0] + (waterSurfaceCell + 18))->height;
    second.y = (state->vertices[0] + (waterSurfaceCell + 17))->height;
    nearFlat = origin;
    farFlat = far;
    nearFlat.y = 0.0f;
    farFlat.y = 0.0f;
    const float nearDistance = am2Vector3Distance(&nearFlat, &waterSurfaceQuery);
    if (am2Vector3Distance(&farFlat, &waterSurfaceQuery) < nearDistance)
        origin = far;
    am2Vector3Subtract(&a, &first, &origin);
    am2Vector3Subtract(&b, &second, &origin);
    am2Vector3Cross(&a, &b, &normal);
    waterSurfaceQuery.y = origin.y - ((waterSurfaceQuery.x - origin.x) * normal.x + normal.z * (waterSurfaceQuery.z - origin.z)) / normal.y;
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ProjectFireFftPoint(const Vector3 &point, Vector3 &output);

void UpdateFloatingSurfaceProjection(void *mesh, float refraction) {
    Matrix4x4 projection, nodeMatrix;
    std::uint32_t *packet = static_cast<std::uint32_t *>(mesh);
    const float inverse = 1.0f / refraction;
    matrix_stack::GetMatrix(&nodeMatrix);
    packet += 6;
    matrix_stack::PushUnitMatrix();
    reinterpret_cast<void (*)(float, float)>(PrepareShadowVolumeProjection)(0.0f, 0.0f);
    matrix_stack::GetMatrix(&projection);
    for (;;) {
        std::uint32_t flags = *packet;
        if (!flags)
            break;
        if (flags & 0x80000000U) {
            packet[2] &= 0xfffe7fffU;
            packet += 20;
            flags = *packet++;
        } else {
            ++packet;
        }
        std::uint32_t count = *packet++;
        if (flags & 8U)
            count *= 3;
        while (count-- > 0) {
            std::uint32_t *const vertex = packet;
            float *const uv = reinterpret_cast<float *>(packet);
            if (*packet & 1U) {
                Vector3 position, unused, translation, screen, rotated, normal;
                matrix_stack::LoadMatrix(&nodeMatrix);
                Matrix4x4::TransformPoint(*reinterpret_cast<const Vector3 *>(packet), position);
                const std::int32_t packed = static_cast<std::int32_t>(packet[3]);
                const std::int8_t normalX = static_cast<std::int8_t>(packed >> 16), normalY = static_cast<std::int8_t>(packed >> 8);
                const std::int8_t normalZ = static_cast<std::int8_t>(packed);
                normal.x = normalX / 127.0f;
                normal.y = normalY / 127.0f;
                normal.z = normalZ / 127.0f;
                Matrix4x4::Transform(normal, rotated);
                const float distance = am2Vector3Length(&position);
                const float horizontal = shenmue::NativeSqrt(position.x * position.x + position.z * position.z);
                matrix_stack::PushUnitMatrix();
                matrix_stack::RotateYSinCos(-position.x / horizontal, -position.z / horizontal);
                matrix_stack::RotateXSinCos(position.y / distance, horizontal / distance);
                Matrix4x4::preTranslate(0.0f, 0.0f, -distance);
                const float cosine = am2Vector3Dot(&position, &rotated) / distance;
                const float sine = __builtin_sqrtf(1.0f - cosine * cosine);
                const float scaled = inverse * sine;
                const std::int32_t base = ComputeNormalizedAngleRatio(sine);
                Matrix4x4::RotateAroundX_Wrapper(ComputeNormalizedAngleRatio(scaled) - base);
                Matrix4x4::preTranslate(0.0f, 0.0f, -distance);
                matrix_stack::GetTranslation(&translation);
                float inverseWidth = 1.0f / 640.0f, inverseHeight = 1.0f / 480.0f;
                PopCurrentMatrix(1);
                matrix_stack::LoadMatrix(&projection);
                ProjectFireFftPoint(translation, screen);
                float u = screen.x * inverseWidth, v = screen.y * inverseHeight;
                if (u < 0.0f)
                    u = 0.0f;
                if (u > 1.0f)
                    u = 1.0f;
                if (v < 0.0f)
                    v = 0.0f;
                if (v > 1.0f)
                    v = 1.0f;
                uv[6] = u;
                uv[7] = v;
                vertex[7] |= 1U;
                packet += 8;
            } else {
                packet += 2;
            }
        }
    }
    PopCurrentMatrix(1);
}

void UpdateFloatingSurfaceProjectionTree(MT7Node *node, float refraction) {
    if (node->mesh || node->child) {
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
        if (node->mesh)
            UpdateFloatingSurfaceProjection(node->mesh, refraction);
        if (node->child)
            UpdateFloatingSurfaceProjectionTree(node->child, refraction);
        PopCurrentMatrix(1);
    }
    if (node->sibling)
        UpdateFloatingSurfaceProjectionTree(node->sibling, refraction);
}
#endif

} // namespace s2
} // namespace shenmue
#endif
