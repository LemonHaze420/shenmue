#ifndef SHENMUE_S2_SHADOW_CAST_DC_H
#define SHENMUE_S2_SHADOW_CAST_DC_H

/* GameJam builds the character shadow casting code (ComputeShadowLightSource through DrawCharacterShadowSprite) from a
   C source. This is that translation unit's view of the layouts it reads and of the C++ functions and globals it uses;
   character_dc.hpp and model_dc.hpp check the C++ layouts these mirror. */

typedef unsigned int shen_name;
typedef struct Vector3 {
    float x, y, z;
} Vector3;
typedef struct Vector3i {
    long x, y, z;
} Vector3i;
/* the plane a shadow is projected onto: a point on it and its normal, read as six consecutive floats */
typedef struct ShadowPlane {
    Vector3 point, normal;
} ShadowPlane;
typedef struct MT7Node {
    short id, kind;
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    void *mesh;
    struct MT7Node *child, *sibling, *parent;
    const void *transformOverride;
    void *taskData;
} MT7Node;
typedef struct CharacterNode {
    struct CharacterNode *parent;
    Vector3 position;
    Vector3i rotation;
    long layer;
    unsigned long collision;
} CharacterNode;
/* the leading members of Character; this code reads nothing past the shadow model */
typedef struct Character {
    shen_name name;
    CharacterNode node;
    Vector3 positionDelta, scale;
    MT7Node *model, *shadowModel;
} Character;
#define CHARACTER_LIGHT_COUNT 6
typedef struct CharacterLightControl {
    short state;
    signed char mode, fadeLimit;
    short ids[CHARACTER_LIGHT_COUNT];
    float weights[CHARACTER_LIGHT_COUNT], intensities[CHARACTER_LIGHT_COUNT];
} CharacterLightControl;
typedef void (*CharacterUpdateCallback)(const MT7Node *root, float alpha);

typedef char ShadowCastLayout0[sizeof(MT7Node) == 0x40 ? 1 : -1];
typedef char ShadowCastLayout1[sizeof(CharacterNode) == 0x24 ? 1 : -1];
typedef char ShadowCastLayout2[sizeof(Character) == 0x48 ? 1 : -1];
typedef char ShadowCastLayout3[sizeof(CharacterLightControl) == 0x40 ? 1 : -1];

#define CHARACTER_SHADOW_THRESHOLD 0.003000000026077032f

extern long g_shadowVolumesDisabled __asm__("__Q27shenmue2s2.g_shadowVolumesDisabled");
extern long g_shadowRenderingEnabled __asm__("__Q27shenmue2s2.g_shadowRenderingEnabled");
extern float g_shadowDistanceFadeWidth __asm__("__Q27shenmue2s2.g_shadowDistanceFadeWidth");
extern MT7Node *g_shadowSprite __asm__("__Q27shenmue2s2.g_shadowSprite");

unsigned long GetLightRuntimeKind(unsigned long index) __asm__("_GetLightRuntimeKind__Q27shenmue2s2Ul");
void GetLightRuntimeDirection(unsigned long index, Vector3 *direction) __asm__("_GetLightRuntimeDirection__Q27shenmue2s2UlPQ27shenmuet8Vector3T1Zf");
void GetLightRuntimePosition(unsigned long index, Vector3 *position) __asm__("_GetLightRuntimePosition__Q27shenmue2s2UlPQ27shenmuet8Vector3T1Zf");
void PrepareCharacterShadowLighting(void) __asm__("_PrepareCharacterShadowLighting__Q27shenmue2s2v");
void RestoreCharacterShadowLighting(void) __asm__("_RestoreCharacterShadowLighting__Q27shenmue2s2v");
Vector3 *camCtrlGetNextCamPos(Vector3 *position) __asm__("_camCtrlGetNextCamPos__Q27shenmue2s2PQ27shenmuet8Vector3T1Zf");
float CalculatePlanarDistanceBetweenPoints(const Vector3 *first, const Vector3 *second) __asm__(
    "_CalculatePlanarDistanceBetweenPoints__7shenmuePCQ27shenmuet8Vector3T1ZfT1");
float GetShadowRange(void) __asm__("_GetShadowRange__Q27shenmue2s2v");
float GetCharacterAlpha(const Character *character) __asm__("_GetCharacterAlpha__Q27shenmue2s2PCQ37shenmue2s29Character");
CharacterLightControl *GetControllerForNode(Character *character) __asm__("_GetControllerForNode__Q27shenmue2s2PQ37shenmue2s29Character");
void flddPushIntersectionFlag(unsigned short flags) __asm__("_flddPushIntersectionFlag__Q27shenmue2s2Us");
float FindFieldIntersectionHeight(const Vector3 *position) __asm__("_FindFieldIntersectionHeight__Q27shenmue2s2PCQ27shenmuet8Vector3T1Zf");
void PublishSharedShadowNodeTransforms(MT7Node *model, MT7Node *shadow) __asm__("_PublishSharedShadowNodeTransforms__Q27shenmue2s2PQ37shenmue2s27MT7NodeT1");
void mtUpdateTransparencyCallback(const MT7Node *root, float alpha) __asm__("_mtUpdateTransparencyCallback__Q27shenmue2s2PCQ37shenmue2s27MT7Nodef");
void PopCurrentMatrix(long count) __asm__("_PopCurrentMatrix__Q27shenmue2s2l");
void Matrix4x4_Push(const void *matrix) __asm__("_Matrix4x4_Push__Q27shenmue12matrix_stackPCQ27shenmue9Matrix4x4");
void Matrix4x4_preTranslate(float x, float y, float z) __asm__("_preTranslate__Q27shenmue9Matrix4x4fff");
void Matrix4x4_RotateAroundX(long angle) __asm__("_RotateAroundX_Wrapper__Q27shenmue9Matrix4x4l");
void Matrix4x4_RotateAroundY(long angle) __asm__("_RotateAroundY_Wrapper__Q27shenmue9Matrix4x4l");
void Matrix4x4_RotateAroundZ(long angle) __asm__("_RotateAroundZ_Wrapper__Q27shenmue9Matrix4x4l");
void Matrix4x4_Scale(float x, float y, float z) __asm__("_Scale__Q27shenmue9Matrix4x4fff");
void Matrix4x4_RotateYSinCos(float sine, float cosine) __asm__("_RotateYSinCos__Q27shenmue12matrix_stackff");
void SetModelScale(float scale) __asm__("_SetModelScale__Q37shenmue2dc3sdkf");
void DrawSingleLightMT7Mesh(void *mesh) __asm__("_DrawSingleLightMT7Mesh__Q37shenmue2dc3sdkPv");
void DrawSingleLightMT7MeshAlpha(void *mesh, float alpha) __asm__("_DrawSingleLightMT7MeshAlpha__Q37shenmue2dc3sdkPvf");
float am2Vector3NormalizeInPlace(Vector3 *value);
void am2MultiplyPlaneProjection(const ShadowPlane *plane, const Vector3 *direction);

float ComputeShadowLightSource(const Vector3 *position, Vector3 *source, float strength, unsigned long lightIndex);
void ApplyDirectionAlignedShadowProjection(Vector3 position, Vector3 source, ShadowPlane plane);
float GetCharacterShadowDistanceFade(const Vector3 *position, const Character *character);
void DrawMotionCharacterShadow(const Vector3 *position, float groundY, MT7Node *model, MT7Node *shadow, float strength, Character *character);
void DrawProjectedSharedCharacterShadow(const Vector3 *position, unsigned long lightIndex, float groundY, Character *character, float opacity);
void DrawProjectedCharacterShadow(CharacterUpdateCallback callback, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale, float groundY,
    float strength, const MT7Node *sourceRoot, const MT7Node *shadowRoot, Character *character);
void DrawProjectedFixedShadow(
    const Vector3 *position, const Vector3i *rotation, const Vector3 *scale, float groundY, void *mesh, float strength, Character *character);
void DrawCharacterShadowSprite(const Vector3 *position, float groundY, Character *character);

#endif
