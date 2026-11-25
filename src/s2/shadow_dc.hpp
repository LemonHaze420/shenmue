#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "common/types.hpp"
#include "s2/model_dc.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue {
namespace s2 {
struct Character;
struct ShadowVolumeVertex;
struct ShadowShapeNode {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int16_t boneId;
    std::uint16_t retained02;
#else
    std::uint32_t unknown00;
#endif
    ShadowShapeNode *child, *sibling;
    void *data[5];
    std::int16_t vertexCount, edgeCount, faceCount;
};
struct ShadowShapeHeader {
    std::uint32_t unknown00;
    std::uint16_t unknown04, relocated;
    ShadowShapeNode *root;
};
struct ShadowShapeCacheEntry {
    void *data;
    char name[16];
};
struct ShadowModelCacheEntry {
    MT7Node *model;
    char filename[16];
};
struct MapShadowPlacement {
    std::int32_t shapeIndex;
    Vector3 position, rotation;
};
struct MapShadowStorage {
    ShadowShapeNode *shapes[10];
    MapShadowPlacement placements[100];
    std::int32_t terminator;
};
typedef char Layout_shadow_dc_0[(sizeof(MapShadowPlacement) == 0x1c) ? 1 : -1];
typedef char Layout_shadow_dc_1[(offsetof(MapShadowPlacement, position) == 4) ? 1 : -1];
typedef char Layout_shadow_dc_2[(offsetof(MapShadowPlacement, rotation) == 0x10) ? 1 : -1];
typedef char Layout_shadow_dc_3[(offsetof(MapShadowStorage, placements) == 0x28) ? 1 : -1];
typedef char Layout_shadow_dc_4[(offsetof(MapShadowStorage, terminator) == 0xb18) ? 1 : -1];
typedef char Layout_shadow_dc_5[(sizeof(MapShadowStorage) == 0xb1c) ? 1 : -1];
struct ShadowDrawParameters {
    std::uint32_t flags;
    std::int32_t vertexCapacity, clipCapacity, polygonCapacity;
    std::int32_t shadowStrength;
    float length;
    Vector3 lightDirection;
};
typedef char Layout_shadow_dc_6[(sizeof(ShadowShapeNode) == 0x28) ? 1 : -1];
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char Layout_shadow_dc_7[(offsetof(ShadowShapeNode, boneId) == 0) ? 1 : -1];
typedef char Layout_shadow_dc_8[(offsetof(ShadowShapeNode, retained02) == 2) ? 1 : -1];
#endif
typedef char Layout_shadow_dc_9[(offsetof(ShadowShapeNode, data) == 0x0c) ? 1 : -1];
typedef char Layout_shadow_dc_10[(offsetof(ShadowShapeNode, vertexCount) == 0x20) ? 1 : -1];
typedef char Layout_shadow_dc_11[(offsetof(ShadowShapeNode, edgeCount) == 0x22) ? 1 : -1];
typedef char Layout_shadow_dc_12[(offsetof(ShadowShapeNode, faceCount) == 0x24) ? 1 : -1];
typedef char Layout_shadow_dc_13[(sizeof(ShadowShapeHeader) == 0x0c) ? 1 : -1];
typedef char Layout_shadow_dc_14[(offsetof(ShadowShapeHeader, relocated) == 6) ? 1 : -1];
typedef char Layout_shadow_dc_15[(sizeof(ShadowShapeCacheEntry) == 0x14) ? 1 : -1];
typedef char Layout_shadow_dc_16[(sizeof(ShadowModelCacheEntry) == 0x14) ? 1 : -1];
typedef char Layout_shadow_dc_17[(sizeof(ShadowDrawParameters) == 0x24) ? 1 : -1];
typedef char Layout_shadow_dc_18[(offsetof(ShadowDrawParameters, shadowStrength) == 0x10) ? 1 : -1];
typedef char Layout_shadow_dc_19[(offsetof(ShadowDrawParameters, length) == 0x14) ? 1 : -1];
typedef char Layout_shadow_dc_20[(offsetof(ShadowDrawParameters, lightDirection) == 0x18) ? 1 : -1];
#define SHARED_SHADOW_MEMORY SHEN_NAME4('S', 'H', 'D', 'S')
#define SHADOW_VERTEX_MEMORY SHEN_NAME4('M', 'V', 'S', 'W')
static const std::uint32_t shadowStripHeaderInitial[4] = {0x81000000U, 0, 0x81000040U, 0x20000000U};
extern MT7Node *g_sydRoot;
extern ShadowShapeCacheEntry g_shadowShapeCache[8];
extern std::int32_t g_shadowShapeCacheCount;
extern ShadowModelCacheEntry g_shadowModelCache[10];
extern std::int32_t g_shadowModelCacheCount;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#define SHADOW_SPRITE_MEMORY SHEN_NAME4('S', 'H', 'D', 'M')
extern MT7Node *g_shadowSprite;
extern std::int32_t g_shadowRenderingEnabled;
extern float g_shadowDistanceFadeWidth;
void LoadShadowSprite();
void ApplyCharacterShadowBoneTransform(MT7Node *model, std::int16_t boneId);
void DrawCharacterShadowShapeNodes(MT7Node *model, ShadowShapeNode *shape);
void DrawCharacterShadowShape(MT7Node *model, ShadowShapeNode *shape);
#endif
static const char *shadowModelTokens[] = {"Image", "Mode", "Position", "Scale", "Yang", "Height", 0};
extern ShadowShapeNode *g_shadowChestShape, *g_shadowArmShape;
extern ShadowDrawParameters g_shadowDrawParameters;
extern std::int32_t g_shadowVertexCapacity, g_shadowClipCapacity, g_shadowPolygonCapacity;
extern ShadowVolumeVertex *g_shadowVertices, *g_shadowExtrudedVertices, *g_shadowNearPlaneVertices, *g_shadowEdgeNearPoints, *g_shadowEdgeFarPoints;
extern std::uint8_t *g_shadowEdgeFlags;
extern Vector3 *g_shadowFaceLightVectors;
extern std::uint32_t g_shadowStripHeader[2], g_shadowStripLastHeader[2];
extern float g_shadowOpacityScale, g_shadowParameter;
extern float g_shadowRange, g_shadowRangeInverse;
float GetShadowRange();
void EV_SetAlignedShadowRange(float range);
void SHRG_Reader(const void *chunk);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern ShadowShapeNode *g_mapShadowShapes[10];
extern MapShadowPlacement g_mapShadowPlacements[101];
#else
extern MapShadowStorage g_mapShadowStorage;
#endif
#define MAP_MODIFIER_VOLUME_TASK SHEN_NAME4('M', 'P', 'M', 'V')
void MVTR_Reader();
void MapModifierVolumeTask_Callback(void *data);
void DrawMapModifierVolumes(void *data);
extern std::int32_t g_shadowPointLightMode;
void *LoadSYD();
MT7Node *GetSHDS();
void InitializeDefaultCharacterShadow(Character *character);
void InitSYD();
ShadowShapeNode *ReadShadowShape(ShadowShapeHeader *header);
ShadowShapeNode *LoadShadowShape(const char *name);
void SetShadowOpacityScale(float opacity);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
float GetShadowOpacityScale();
std::int32_t GetMazeTerrainDrawEnabled();
std::int32_t GetMazeRoadDrawEnabled();
std::int32_t GetMazeRoadPropsEnabled();
std::int32_t GetMazeUnitDrawEnabled();
std::int32_t GetMazeWireframeEnabled();
std::int32_t GetMazeNaviEnabled();
std::int32_t GetMazeModelOverride();
std::int32_t GetMazeUnitWallsEnabled();
std::int32_t GetMazeRetainedSetting();
void SetMazeRetainedSetting(std::int32_t value);
std::int32_t GetMazeCullingAngleLimit();
float GetMazeUnitDrawDistance();
void SetMazeRoadDrawEnabled(std::int32_t enabled);
void SetMazeCullingSteps(std::int32_t steps);
void SetMazeRoadPropsEnabled(std::int32_t enabled);
void SetMazeUnitDrawEnabled(std::int32_t enabled);
void SetMazeWireframeEnabled(std::int32_t enabled);
void SetMazeUnitWallsEnabled(std::int32_t enabled);
void SetMazeNaviEnabled(std::int32_t enabled);
void SetMazeModelOverride(std::int32_t enabled);
void SetMazeAuxiliaryWord(std::int32_t value);
std::int32_t GetMazeAuxiliaryWord();
void SetMazeRetainedValue(std::int32_t value);
std::int32_t GetMazeRetainedValue();
struct MotionRigKey;
float EvaluateCurve(std::int32_t count, const MotionRigKey *keys, float time);
float EvaluateCurveSegmentCubic(const MotionRigKey *previous, const MotionRigKey *key, float time);
float EvaluateCurveSlope(std::int32_t count, const MotionRigKey *keys, float time);
float EvaluateCurveSlopeCubic(const MotionRigKey *previous, const MotionRigKey *key, float time);
#endif
void SetShadowPointLightMode(std::int32_t mode);
void InitializeShadowDrawBuffers(std::byte *memory, const ShadowDrawParameters &parameters);
MT7Node *FindCachedShadowModel(const char *filename);
MT7Node *CreateShadowModel(const char *name);
} // namespace s2
} // namespace shenmue
#endif
