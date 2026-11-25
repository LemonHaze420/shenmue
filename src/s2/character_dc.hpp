#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/hlib.hpp"

#define SHENMUE_CHARACTER_LAYOUT_NAME_(line) ShenmueCharacterLayoutLayoutCheck##line
#define SHENMUE_CHARACTER_LAYOUT_NAME(line) SHENMUE_CHARACTER_LAYOUT_NAME_(line)
#define SHENMUE_CHARACTER_LAYOUT(condition) typedef char SHENMUE_CHARACTER_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {
using ::shenmue::shen_name;

#define CHARACTER_ENTITY shen_name(SHEN_NAME4('C', 'E', 'W', 'P'))
#define CHARACTER_INVALID_NAME shen_name(SHEN_NAME4('E', 'R', 'R', 'R'))
#define CHARACTER_NAME_RYO shen_name(SHEN_NAME4('R', 'Y', 'O', '_'))
#define CHARACTER_NAME_NULL shen_name(SHEN_NAME4('n', 'u', 'l', 'l'))
#define CHARACTER_NAME_AKIR shen_name(SHEN_NAME4('A', 'K', 'I', 'R'))
#define CHARACTER_NAME_AKID shen_name(SHEN_NAME4('A', 'K', 'I', 'D'))
#define CHARACTER_NAME_JAKR shen_name(SHEN_NAME4('J', 'A', 'K', 'R'))
#define CHARACTER_NAME_AKI shen_name(SHEN_NAME4('A', 'K', 'I', '_'))
#define TASK_CHAR shen_name(SHEN_NAME4('C', 'H', 'A', 'R'))
#define MEMORY_MODL shen_name(SHEN_NAME4('M', 'O', 'D', 'L'))
#define CHARACTER_MAP_EVENT shen_name(SHEN_NAME4('M', 'E', 'V', 'W'))
#define CHARACTER_CLIPPER shen_name(SHEN_NAME4('C', 'L', 'I', 'P'))
#define CHARACTER_IMAGE shen_name(SHEN_NAME4('I', 'M', 'G', 'M'))
#define CHARACTER_SHENHUA shen_name(SHEN_NAME4('S', 'I', 'N', '_'))
#define CHARACTER_MOTION shen_name(SHEN_NAME4('M', 'T', 'W', 'K'))
#define MEMORY_PROP shen_name(SHEN_NAME4('P', 'R', 'O', 'P'))
enum CharacterFlags {
    CF_Valid = 1,
    CF_Unknown02 = 2,
    CF_Active = 4,
    CF_DisplayCheckEnabled = 8,
    CF_VisibleThisCheck = 0x10,
    CF_HandlerDisabled = 0x20,
    CF_Unknown40 = 0x40,
    CF_DrawMirrored = 0x80,
    CF_UseStoredShadowY = 0x100,
    CF_ForceVisibleCheck = 0x200,
    CF_NpcLoggerTarget = 0x400
};
enum CharaVisibilityMode {
    CVM_All = 0,
    CVM_FrustumResult = 1,
    CVM_CameraOcclusionResult = 2,
    CVM_DisplayEnabled = 3,
    CVM_Unknown4 = 4,
    CVM_MotionworkDisplayCheck = 5,
    CVM_Unknown6 = 6
};
static const std::uint8_t characterVisibilityMasks[] = {0x3f, 1, 2, 4, 8, 0x10, 0x20};
static const std::int8_t characterHandlerIndices[] = {9, 10, 11, 11};

struct Character;
struct MT7Model;
struct MT7Node;
struct CharacterNode {
    CharacterNode *parent;
    Vector3 position;
    Vector3i rotation;
    std::int32_t layer;
    std::uint32_t collision;
};
struct ModelComponentStorageBlock {
    ModelComponentStorageBlock *previous;
    std::uint32_t usedBytes;
};
struct NameTreeNode {
    NameTreeNode *higher, *lower;
    shen_name name;
};
struct ModelComponentHeader {
    NameTreeNode tree;
    ModelComponentHeader *next;
    std::uint32_t flags;
    void (*update)(Character *, void *);
    void (*cleanup)(Character *, void *);
    void (*core)(Character *, void *);
};
typedef void (*CharacterUpdateCallback)(const MT7Node *, float);
static const std::int32_t kCharacterLightCount = 6;
struct Character {
    shen_name name;
    CharacterNode node;
    Vector3 positionDelta, scale;
    MT7Node *model, *shadowModel;
    float alpha, drawDistance;
    Vector3 viewPosition;
    std::uint32_t flags;
    Vector3 shadowOffset, shadowScale;
    std::int32_t shadowYaw, type;
    float shadowY;
    std::int32_t positionDeltaApplied;
    std::uint8_t renderFlags, retainedRenderByte;
    std::int16_t componentAllocationBytes;
    float radius;
    shen_name resourceOwner;
    ModelComponentStorageBlock *modelComponentStorage;
    ModelComponentHeader *modelComponents;
    NameTreeNode *tokenTree;
    ModelComponentStorageBlock *auxiliaryComponentStorage;
    std::int16_t retainedComponentState[2];
    void (*core)(Character *);
    void (*collisionCheck)(void *);
    CharacterUpdateCallback applyScale;
    CharacterUpdateCallback prepareForDraw;
    std::uint16_t retainedLightState;
    std::uint8_t retainedLightMode, retainedLightEnabled;
    std::int16_t lightIds[kCharacterLightCount];
    float lightWeights[kCharacterLightCount], lightIntensities[kCharacterLightCount];
    void *facialModel;
};
SHENMUE_CHARACTER_LAYOUT(sizeof(CharacterNode) == 0x24);
SHENMUE_CHARACTER_LAYOUT(offsetof(CharacterNode, position) == 4);
SHENMUE_CHARACTER_LAYOUT(offsetof(CharacterNode, rotation) == 0x10);
SHENMUE_CHARACTER_LAYOUT(offsetof(CharacterNode, layer) == 0x1c);
SHENMUE_CHARACTER_LAYOUT(sizeof(ModelComponentStorageBlock) == 8);
SHENMUE_CHARACTER_LAYOUT(sizeof(NameTreeNode) == 12);
SHENMUE_CHARACTER_LAYOUT(sizeof(ModelComponentHeader) == 0x20);
SHENMUE_CHARACTER_LAYOUT(offsetof(ModelComponentHeader, next) == 0x0c);
SHENMUE_CHARACTER_LAYOUT(offsetof(ModelComponentHeader, cleanup) == 0x18);
SHENMUE_CHARACTER_LAYOUT(sizeof(Character) == 0xfc);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, model) == 0x40);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, shadowModel) == 0x44);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, flags) == 0x5c);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, componentAllocationBytes) == 0x8a);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, resourceOwner) == 0x90);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, modelComponentStorage) == 0x94);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, auxiliaryComponentStorage) == 0xa0);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, core) == 0xa8);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, lightIds) == 0xbc);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, retainedLightState) == 0xb8);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, retainedLightEnabled) == 0xbb);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, lightWeights) == 0xc8);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, lightIntensities) == 0xe0);
SHENMUE_CHARACTER_LAYOUT(offsetof(Character, facialModel) == 0xf8);
struct CharacterPropertySlot {
    std::int16_t lower, higher;
    std::uint32_t key, value;
};
struct CharacterPropertyBlock {
    CharacterPropertyBlock *next;
    CharacterPropertySlot slots[32];
};
SHENMUE_CHARACTER_LAYOUT(sizeof(CharacterPropertySlot) == 12);
SHENMUE_CHARACTER_LAYOUT(sizeof(CharacterPropertyBlock) == 0x184);

struct CharacterLightControl {
    std::int16_t state;
    std::int8_t mode, fadeLimit;
    std::int16_t ids[kCharacterLightCount];
    float weights[kCharacterLightCount], intensities[kCharacterLightCount];
};
SHENMUE_CHARACTER_LAYOUT(sizeof(CharacterLightControl) == 0x40);

extern std::int16_t g_characterHandWristAngles[4];
std::uint32_t *GetControllerForNode(Character *character);
float GetCharacterDisplayRadius(const Character *character);
std::int32_t GetCharacterLayer(const Character *character);
void SetCharacterType(Character *character, std::int32_t type);
void SetCharacterUnknownFlag(Character *character, std::uint32_t enabled);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void GetCharacterShadowOffset(const Character *character, Vector3 *offset);
void GetFacialOsageDisplacement(void *face, Vector3 *output);
void FacialDisplacement(Character *character, Vector3 *displacement);
void GetCharacterShadowScale(const Character *character, Vector3 *scale);
std::int32_t GetCharacterShadowYaw(const Character *character);
std::int32_t GetCharacterType(const Character *character);
float GetCharacterShadowY(const Character *character);
bool IsCharacterStoredShadowYEnabled(const Character *character);
#endif
void SetCharacterShadowOffset(Character *character, const Vector3 *offset);
void SetCharacterShadowScale(Character *character, const Vector3 *scale);
void SetCharacterShadowYaw(Character *character, std::int32_t yaw);
void SetCharacterStoredShadowYEnabled(Character *character, std::int32_t enabled);
void SetCharacterShadowY(Character *character, float y);
extern Matrix4x4 g_characterNodeSavedTransform;
extern Matrix4x4 characterMirrorMatrix;
extern std::int32_t g_characterMirroredTransformEnabled;
CharacterUpdateCallback GetMotionCallback(const Character *character);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef void (*CharacterDrawCallback)(void *);
CharacterDrawCallback mtwkAssignDrawCallback(Character *character, CharacterDrawCallback callback);
#endif
void SetModelComponentCleanup(Character *character, shen_name token, void (*cleanup)(Character *, void *));
void SetModelComponentCore(Character *character, shen_name token, void (*core)(Character *, void *));
void (*charaAssignCoreCallback(Character *character, void (*callback)(Character *)))(Character *);
CharacterUpdateCallback imgmAssignUpdateCallback(Character *character, CharacterUpdateCallback callback);
CharacterUpdateCallback charaAssignPrepareForDrawCallback(Character *character, CharacterUpdateCallback callback);
void charaSetDisplayEnabledFlag(Character *character, std::int32_t enabled);
void SetCharacterModelName(Character *character, shen_name name);
void SetCharacterRadius(Character *character, float radius);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern float g_motionVertexLinkDistance;
float mtwkSetVertexLinkDistance(float distance);
float mtwkGetVertexLinkDistance();
void mtwkDrawCharacterCore(Character *character);
void DrawShenmueEntity(void *parameter);
void PrepareClothForDraw(Character *character);
std::int32_t UpdateClothNodeTree(MT7Node *model);
void UpdateCharacterOsage(MT7Node *model, Character *character);
void DrawCharacterMatrixFrames(MT7Node *model, float alpha, const Matrix4x4 *frames);
#endif
float GetCharacterAlpha(const Character *character);
void SetCharacterAlpha(Character *character, float alpha);
std::int32_t SetPluralControlFor(Character *character, std::int16_t state);
const Matrix4x4 *GetCharacterMirrorMatrix();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct CharacterMirrorPlane {
    Vector3 position, normal;
};
SHENMUE_CHARACTER_LAYOUT(sizeof(CharacterMirrorPlane) == 24);
extern const shen_name CHARACTER_MIRROR_STORAGE;
static const std::int16_t motionMirrorModelNodeIds[28] = {
    1, -67, 2, 4, 5, 6, -65, 9, 10, 11, -66, 14, 16, 17, 18, 21, 22, 23, 0, 15, 20, 55, 56, 57, -79, 58, 59, 119};
void SetCharacterMirrorPlane(const CharacterMirrorPlane *plane, const Vector3i *rotation);
#endif

std::int32_t IsCharacterMirroredForDraw(const Character *character);
std::int32_t IsMapModelMirrored(std::int32_t index);
void ApplyRecursiveCharacterNodeTransforms(const CharacterNode *node);
void ApplyRecursiveCharacterNodeRotations(const CharacterNode *node);
void GetNodeWorldPosition(const CharacterNode *node, Vector3 *position);
void GetNodeWorldRotation(const CharacterNode *node, Vector3i *rotation);
void GetCharacterWorldRotation(const Character *character, Vector3i *rotation);
void GetCharacterLocalRotation(const Character *character, Vector3i *rotation);
void SetCharacterLocalRotation(Character *character, const Vector3i *rotation);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void shenSetRotY(Character *character, std::int32_t yaw);
#endif
void GetCharacterScale(const Character *character, Vector3 *scale);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetCharacterScale(Character *character, const Vector3 *scale);
#endif
void shenApplyTranslationToAll(CharacterNode *node, const Vector3 &position);
void SetCharacterNodeWorldRotation(CharacterNode *node, const Vector3i &rotation);
void charaSetWorldRot(Character *character, const Vector3i &rotation);
void TransformCharacterPoseByMatrix(const Matrix4x4 *transform, Vector3 *position, Vector3i *rotation);
bool InsertNameTree(NameTreeNode **root, NameTreeNode *node);
bool RemoveNameTree(NameTreeNode **root, NameTreeNode *node);
NameTreeNode *FindNameTree(NameTreeNode *root, shen_name name);
std::int32_t IsCharacterValid(const Character *character);
bool HasCharacterMotionWork(const Character *character);
bool charaGetWasVisible(const Character *character);
bool charaCheckFlags(const Character *character);
bool charaIsDisplayCheckEnabled(const Character *character);
bool charaForceVisibleCheck(const Character *character);
void charaSetVisibilityFlagTo(Character *character, std::uint32_t set);
std::uint32_t GetCharacterMotionCompatibilityResult(const Character *character);
void SetCharacterMotionClipMode(Character *character, std::int32_t mode);
bool IsOfThisVisibilityType(const Character *character, CharaVisibilityMode mode);
void GetCharacterWorldPosition(const Character *character, Vector3 *position);
void GetCharacterPositionDelta(const Character *character, Vector3 *position);
void SetCharacterPositionDelta(Character *character, const Vector3 *position);
void coliPositionAdd(Character *character, const Vector3 *delta);
shen_name GetCharacterResourceOwner(const Character *character);
CharacterNode *GetCharacterNode(Character *character);
void FreeModelComponents(Character *character);
void *AllocateModelComponent(Character *character, shen_name token, std::uint32_t payloadSize, void (*update)(Character *, void *));
void *charFindComp(const Character *character, shen_name token);
bool IsModelComponentEnabled(const Character *character, shen_name token);
std::int32_t SetFlagForToken(Character *character, shen_name token, std::int32_t enabled);
void SetCharacterActive(Character *character, std::int32_t enabled);
void SetMirrored(Character *character, std::int32_t enabled);
void MarkCharacterForCleanup(Character *character, std::int32_t mode);
void EV_SetCharDisplay(Character *character, CharaVisibilityMode mode, std::int32_t enabled);
void UpdateCharacterPositionDelta(Character *character);
void CharacterHandler_Callback(void *data);
Character *EnqueueCharacterHandler(std::int8_t index, HLib::Task **task);
Character *AddCharacterHandler(std::int32_t type);
float GetCharacterLocalPositionY(const Character *character);
void SetCharacterLocalPositionY(Character *character, float y);
float GetCharacterDrawDistance(const Character *character);
void SetCharacterDrawDistance(Character *character, float distance);
void GetCharacterViewPosition(const Character *character, Vector3 *position);
void SetCharacterViewPosition(Character *character, const Vector3 *position);
void SetCharacterHandlerDisabled(Character *character, std::int32_t disabled);
void SetCharacterPositionDeltaApplied(Character *character, std::int32_t applied);
void SetCharacterForceVisibleCheck(Character *character, std::int32_t enabled);
CharacterUpdateCallback GetCharacterPrepareForDrawCallback(const Character *character);
void DrawDummyCharacterCore(Character *character);
void (*charaAssignDummyCore(Character *character))(Character *);
void ApplyCharacterNodeTransformAndScale(Character *character);
CharacterPropertySlot *FindCharacterPropertySlot(Character *character, std::int32_t index);
void AllocateCharacterPropertyBlock(Character *character);
CharacterPropertySlot *FindOrCreateCharacterProperty(Character *character, std::uint32_t key);
void SetCharacterProperty(Character *character, std::uint32_t key, std::uint32_t value);
std::uint32_t GetCharacterProperty(Character *character, std::uint32_t key);
void SetCharacterFlag80000000(Character *character, std::int32_t enabled);
bool IsCharacterFlag80000000Set(const Character *character);
std::uint16_t GetCharacterComponentAllocationBytes(const Character *character);
std::uint32_t charaHasMirroredFlags(const Character *character);
std::uint32_t GetCharacterFlags(const Character *character);
std::int32_t HasCharacterFacialModel(const Character *character);
void CharacterWorldToParentSpace(Character *character, const Vector3 *position, Vector3 *output);
void CharacterParentToWorldSpace(Character *character, const Vector3 *position, Vector3 *output);
std::int32_t GetCharacterDrawControlEnabled(Character *character);
float GetShadowRangeInverse();
void charaCore(Character *character);
void charaCollisionDistanceChecks(void *character);
void charaApplyScale(const MT7Node *node, float alpha);
void charaPrepareNodesForDraw(const MT7Node *node, float alpha);

} // namespace s2
} // namespace shenmue
#undef SHENMUE_CHARACTER_LAYOUT_NAME
#undef SHENMUE_CHARACTER_LAYOUT_NAME_
#undef SHENMUE_CHARACTER_LAYOUT

#endif
