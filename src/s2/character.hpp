#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include "s2/hlib.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

struct MapInfoChunk;
struct CharacterControlSlot;
struct ModelComponentStorageBlock;
struct ModelComponentHeader;
struct CharacterControlModel;
struct MotionWork;
struct MotionPose;
struct FacialModelData;
struct MT7Model;
struct MT7Node;

using CharacterUpdateCallback = void (*)(const MT7Model *, const MT7Node *, float);
enum class NpcFillType : std::int32_t { FrontFill = 1, BackFill = 2, Manual = 3 };
inline constexpr shen_name CHARACTER_DRAW_ACTIVATION{SHEN_NAME(CPAM)};
inline constexpr shen_name CHARACTER_ENTITY{SHEN_NAME(CEWP)};
inline constexpr shen_name CHARACTER_TABLE_DEFAULT_NAME{SHEN_NAME(0000)};
inline constexpr shen_name CHARACTER_CLIPPER_COMPONENT{SHEN_NAME(CLIP)};
inline constexpr shen_name CHARACTER_DRAW_TASK{SHEN_NAME(DRWP)}, HUMAN_DRAW_TASK{SHEN_NAME(DRWH)}, OBJECT_DRAW_TASK{SHEN_NAME(DRWO)};
inline constexpr shen_name CHARACTER_ROCK_OBJECT{SHEN_NAME(RCKO)};
inline constexpr shen_name CHARACTER_RYO{SHEN_NAME(RYO_)};




#pragma pack(push, 4)
struct CharacterNode {
    CharacterNode *child;
    Vector3 position;
    std::int32_t rotation[3]; // @todo: Vector3i
                              // @todo: check DC/XB sizes (maybe i32 vs i16?)
};

struct CharacterEntity {
    shen_name name;
    CharacterNode node;
};

struct Character {
    CharacterEntity entity;
    std::int32_t internalState;
    float field28;
    float field2c;
    Vector3 position;
    Vector3 scale;
    void *model;
    void *shadowModel;
    float alpha;
    float drawDistance;   // +0x5c, MTWK view-space magnitude
    Vector3 viewPosition;
    std::uint32_t flags;
    std::int32_t field70;
    std::uint64_t field74;
    float field7c;
    float field80;
    float field84;
    std::int32_t field88;
    std::int32_t type;
    std::int32_t shadowY;
    std::int32_t field94;
    std::uint8_t renderFlags;
    std::uint8_t field99;
    std::int16_t field9a;
    float radius;
    std::uint32_t fieldA0;
    ModelComponentStorageBlock *modelComponentStorage;
    ModelComponentHeader *modelComponents;
    ModelComponentHeader *tokenTree;
    ModelComponentStorageBlock *auxiliaryComponentStorage;
    std::int16_t fieldC4;
    std::int16_t fieldC6;
    void (*core)(Character *);
    void (*collisionCheck)(void *);         // +0xd0, HLib immediate callback
    CharacterUpdateCallback applyScale;     // +0xd8, consumed by charaCore
    CharacterUpdateCallback prepareForDraw; // +0xe0, projected-shadow traversal
    std::uint32_t fieldE8;
    std::int16_t fieldEc[6];
    std::uint32_t fieldF8[6];
    float field110[6];
    FacialModelData *facialModel;
    std::uint32_t useUnknownFlag;
    void *data2;
    std::int32_t field13c;
};
#pragma pack(pop)

static_assert(sizeof(CharacterNode) == 0x20);
static_assert(sizeof(CharacterEntity) == 0x24);
static_assert(offsetof(Character, position) == 0x30);
static_assert(offsetof(Character, model) == 0x48);
static_assert(offsetof(Character, drawDistance) == 0x5c);
static_assert(offsetof(Character, viewPosition) == 0x60);
static_assert(offsetof(Character, flags) == 0x6c);
static_assert(offsetof(Character, type) == 0x8c);
static_assert(offsetof(Character, renderFlags) == 0x98);
static_assert(offsetof(Character, modelComponentStorage) == 0xa4);
static_assert(offsetof(Character, modelComponents) == 0xac);
static_assert(offsetof(Character, tokenTree) == 0xb4);
static_assert(offsetof(Character, auxiliaryComponentStorage) == 0xbc);
static_assert(offsetof(Character, core) == 0xc8);
static_assert(offsetof(Character, collisionCheck) == 0xd0);
static_assert(offsetof(Character, applyScale) == 0xd8);
static_assert(offsetof(Character, prepareForDraw) == 0xe0);
static_assert(offsetof(Character, fieldE8) == 0xe8);
static_assert(offsetof(Character, facialModel) == 0x128);
static_assert(offsetof(Character, data2) == 0x134);
static_assert(sizeof(Character) == 0x140);




struct CharacterDrawTaskParameters {

    std::array<Character *, 51> humans;
    std::array<float, 51> humanSortDistance;
    std::uint32_t humanCount;
    std::array<Character *, 101> objects;
    std::uint32_t objectCount;
    std::uint32_t reserved594;
    Character *player;
    std::array<Character *, 100> allCharacters;
    std::uint32_t allCharacterCount;
    std::uint32_t reserved8c4;
};

static_assert(offsetof(CharacterDrawTaskParameters, humanSortDistance) == 0x198);
static_assert(offsetof(CharacterDrawTaskParameters, humanCount) == 0x264);
static_assert(offsetof(CharacterDrawTaskParameters, objects) == 0x268);
static_assert(offsetof(CharacterDrawTaskParameters, objectCount) == 0x590);
static_assert(offsetof(CharacterDrawTaskParameters, player) == 0x598);
static_assert(offsetof(CharacterDrawTaskParameters, allCharacters) == 0x5a0);
static_assert(offsetof(CharacterDrawTaskParameters, allCharacterCount) == 0x8c0);
static_assert(sizeof(CharacterDrawTaskParameters) == 0x8c8);

enum CharacterFlags : std::uint32_t {
    CharacterValid = 0x01,
    CharacterFlag02 = 0x02,
    CharacterActive = 0x04,
    CharacterDisplayCheckEnabled = 0x08,
    CharacterVisibleThisCheck = 0x10,
    CharacterHandlerDisabled = 0x20,
    CharacterFlag40 = 0x40,
    CharacterDrawMirrored = 0x80,
    CharacterUseStoredShadowY = 0x100,
    CharacterForceVisibleCheck = 0x200,
    CharacterNpcLoggerTarget = 0x400,
};

using CharacterCoreCallback = void (*)(Character *);
inline constexpr shen_name CHARACTER_INVALID_NAME{SHEN_NAME(ERRR)};

extern Matrix4x4 g_characterNodeSavedTransform;
extern Matrix4x4 g_characterChildRotationTransform;
extern Matrix4x4 g_characterNodeEulerTransform;
extern Matrix4x4 g_characterNodePositionTransform;
extern Matrix4x4 g_characterNodeInverseTransform;
void ApplyRecursiveCharacterNodeRotations(const CharacterNode *node);
void TransformVectorToCharacterChildSpace(const CharacterNode *node, Vector3 *vector);
void AssignCharacterChildSpacePosition(Character *character, const Vector3 *position);
void AddCharacterChildSpacePosition(Character *character, const Vector3 *delta);
void UpdateMotionRootDeltaVector(const MotionWork *work, Vector4 *vector);
void SetCharaTargetTransformOffset(shen_name character, const Vector3 *factor, std::int32_t rotation);
bool HasCharacterMotionWork(const Character *character);
std::int32_t GetCharacterMotionCompatibilityResult(const Character *character);
extern std::int32_t g_characterMirroredTransformEnabled;
struct CharacterMirrorPlane {
    Vector3 position;
    Vector3 normal;
};
static_assert(sizeof(CharacterMirrorPlane) == 24);
static_assert(offsetof(CharacterMirrorPlane, normal) == 12);
inline constexpr shen_name CHARACTER_MIRROR_STORAGE = SHEN_NAME(MIRM);
void SetCharacterMirrorPlane(const CharacterMirrorPlane *plane, const Vector3i *rotation);
const Matrix4x4 *GetCharacterMirrorMatrix();
void ResetCharacterMirrorAdmission();
void RetireCharacterMirrorStorage();
void BindMirroredMotionControllerMatricesToModelNodes(MotionWork *work, MT7Model *model);
bool IsCharacterMirroredForDraw(const Character *character);
float GetCharacterDisplayRadius(const Character *character);
float SetScenarioCharacterRadius(shen_name character, float radius);
void SetChildObjectFlagByIndex(Character *character, std::uint8_t mode);

void ApplyRecursiveCharacterNodeTransforms(const CharacterNode *node);
void TransformPointToCharacterNodeSpace(const CharacterNode *node, Vector3 *point);
void ApplyCharacterEntityTransform(const Character *character);
CharacterNode *GetCharacterNode(Character *character);
std::uint32_t *GetControllerForNode(Character *character);
void GetCharacterLocalPosition(const Character *character, Vector3 *output);
void GetCharacterLocalRotation(const Character *character, Vector3i *output);
std::int32_t shenGetRotYFromNode(const Character *character);
float GetCharacterLocalPositionY(const Character *character);
void SetCharacterLocalPosition(Character *character, const Vector3 *position);
void SetCharacterLocalRotation(Character *character, const Vector3i *rotation);
void shenSetRotY(Character *character, std::int32_t yaw);
void shenGetNodePosY(const Character *character, Vector3 *output);
void SetCharacterCollisionPosition(Character *character, const Vector3 *position);
void coliPositionAdd(Character *character, const Vector3 *delta);
void GetCharacterScale(const Character *character, Vector3 *output);
void SetCharacterScale(Character *character, const Vector3 *scale);
std::uint8_t GetCharaFlag(const Character *character);
void SetCharaFlag(Character *character, std::uint8_t flag);
std::int32_t GetCharacterInternalState(const Character *character);
void SetCharacterInternalState(Character *character, std::int32_t state);
float charaGetAlphaDraw(const Character *character);
float charaGetShadowY(const Character *character);
std::int32_t GetCharacterShadowYaw(const Character *character);
void GetCharacterShadowScale(const Character *character, Vector3 *output);
void GetCharacterShadowOffset(const Character *character, Vector3 *output);
void SetCharacterShadowScale(Character *character, const Vector3 *scale);
void SetCharacterShadowOffset(Character *character, const Vector3 *offset);
void SetCharacterShadowYaw(Character *character, std::int32_t yaw);
void SetCharacterShadowY(Character *character, float height);
void SetCharacterStoredShadowYEnabled(Character *character, std::int32_t enabled);
std::int32_t GetEntityType(const Character *character);
bool charaCheckFlags(const Character *character);
bool charaIsDisplayCheckEnabled(const Character *character);
bool charaGetWasVisible(const Character *character);
bool charaForceVisibleCheck(const Character *character);
std::uint32_t charaHasMirroredFlags(const Character *character);
bool IsCharacterActive(const Character *character);
bool IsCharacterUsingStoredShadowY(const Character *character);
bool HasCharacterFacialModel(const Character *character);
void ClearCharacterFacialModel(Character *character);
void charaSetDisplayEnabledFlag(Character *character, std::int32_t enabled);
void charaSetFlags(Character *character, std::int32_t enabled);
void charaToggleFlag(Character *character, std::int32_t enabled);
void SetMirrored(Character *character, std::int32_t enabled);
void charaSetFlagFor(Character *character, std::int32_t flag);
void SetCharacterRadius(Character *character, float radius);
shen_name GetCharacterModelName(const Character *character);
void SetCharacterModelName(Character *character, shen_name name);
void SetCharacterUnknownFlag(Character *character, std::uint32_t flag);
void *GetCharacterAuxiliaryData(const Character *character);
void SetCharacterAuxiliaryData(Character *character, void *data);
CharacterCoreCallback charaAssignCoreCallback(Character *character, CharacterCoreCallback callback);
void mtwkDrawCharacterCore(Character *character);
void charaCore(Character *character);
void DrawDummyCharacterCore(Character *character);
CharacterCoreCallback charaAssignDummyCore(Character *character);
CharacterUpdateCallback GetMotionCallback(const Character *character);
CharacterUpdateCallback imgmAssignUpdateCallback(Character *character, CharacterUpdateCallback callback);
using CharacterDrawCallback = void (*)(void *);
CharacterDrawCallback mtwkAssignDrawCallback(Character *character, CharacterDrawCallback callback);
CharacterUpdateCallback charaAssignPrepareForDrawCallback(Character *character, CharacterUpdateCallback callback);
void DrawLights(Character *character);
void shenDrawCharaLighting(Character *character);
void charaCollisionDistanceChecks(void *character);
void shenExecuteCharaDrawCallbacks(Character *character);
// Bind the real core-draw context without the world visibility admission gate.
std::uint32_t ExecuteCharacterCoreDraw(Character *character);

bool IsOfThisVisibilityType(const Character *character, std::uint32_t mode);


std::uint8_t EV_SetCharDisplay(Character *character, std::int32_t visibilityType, std::int32_t enabled);
void charaSetVisibilityFlagTo(Character *character, std::uint32_t set);
extern std::int32_t g_npcLoggerEnabled;

enum class ScenarioStepModelAsset : std::uint8_t {
    LMALH00G = 0,
    TMEMH00G = 1,
    ABR0HMPG = 2,
    PSF72AWG = 3,
    PSF6200G = 4,
    PSF3200G = 5,
    HKSMH01G = 6,
    Unresolved = 0xff,
};

struct ScenarioStepHighModelState {
    std::uint32_t modelLoadMask{};
    std::uint32_t modelLoadedMask{};
    std::int32_t nativeMode{-1};
    ScenarioStepModelAsset selectedAsset{ScenarioStepModelAsset::Unresolved};
};

enum CharacterCreationType : std::int32_t {
    CharacterCreationTable0 = 0,
    CharacterCreationLowModel = 1,
    CharacterCreationDefault = 2,
    CharacterCreationHighModel = 3,
};

enum CharacterType : std::int32_t {
    CharacterTypeDefault = 0,
    CharacterTypePlayer = 1,
    CharacterTypeStencilShadow = 4,
    CharacterTypeModelShadow = 7,
    CharacterTypePlayerInverted = 10,
};
inline constexpr shen_name TASK_CHAR{SHEN_NAME(CHAR)};
inline constexpr std::array<std::uint8_t, 4> characterHandlerIndices{9, 10, 11, 11};
inline constexpr shen_name CHARACTER_KUN{SHEN_NAME(KUN_)}, CHARACTER_SYE{SHEN_NAME(SYE_)}, CHARACTER_REN{SHEN_NAME(REN_)};
inline constexpr shen_name CHARACTER_SHENHUA{SHEN_NAME(SIN_)}, CHARACTER_WON{SHEN_NAME(WON_)};
inline constexpr shen_name CHARACTER_JOP{SHEN_NAME(JOP_)}, CHARACTER_TGY{SHEN_NAME(TGY_)};
inline constexpr shen_name characterDedicatedControllers[]{
    CHARACTER_KUN, CHARACTER_SYE, CHARACTER_REN, CHARACTER_SHENHUA, CHARACTER_WON, CHARACTER_JOP, CHARACTER_TGY};
inline constexpr const char *characterTableComponentTokens[]{"Player", "Image", "Entry", "Flags", "Coli", "Human", "Object", "Party", "Face", "Hand", "Item",
    "Range", "ColiOff", "LineWalk", "ForkLift", "MotClip", "Shadow", "ShadowOff", "Book", "KeysRing", "Fight", "Group", "MotOnly", "CasetteDeck", "CasetteTape",
    "Handcuffs", "Cloth", "TinyMot", "Tel", "Step", "Walk3D", "CommonMotionWork", "FootMark", "RefBoard", "Osage", "Knife", "TParty", "Trans", "Vehicle",
    "Bone", "DummyChara", "Bicycle", "Patrol", "Vender", "LowQuality", "DesignOK", "Clock", "ProjectionTrans", "TailLamp", "TinyGame", "Calendar",
    "CutMvShadow", "ObjBlur", "ForceTrans", "ColiOn"};
shen_name CreateEventCharacter(std::int32_t type, const std::byte *data, shen_name name);

bool CHRS_Reader(const MapInfoChunk *chrd, std::int32_t entryId, shen_name chunkName);


bool CHRS_ReadTable(std::span<const std::byte> table, std::int32_t entryId, shen_name owner);
Character *AddHighModelCharacter(shen_name scenarioName, HLib::Task *&handlerTask);
void FreeCharacter(Character *character);
bool ApplyNpcPackageCharacterDisplay(Character *character, std::span<const std::byte> characterTable);
void FreeCharacters();
bool ConfigureScenarioStepHighModelCharacter(Character *character, const ScenarioStepHighModelState &state);
void SetScenarioStepModel(Character *character, std::int32_t mode);
bool GetScenarioStepHighModelState(const Character *character, ScenarioStepHighModelState &state);
Character *FindCharacterByScenarioName(shen_name name);
MotionWork *scenFindMotionwork(shen_name name);

void EV_SetCharacterScale(shen_name name, const Vector3 &scale);
shen_name GetCharacterScenarioName(const Character *character);
std::int32_t GetCharacterBoneId(const Character *character);
void SetCharacterType(Character *character, std::int32_t type);
std::uint16_t SetPluralControlFor(Character *character, std::int16_t enabled);
bool IsCharacterDrawControlEnabled(const Character *character);
void SetCharacterDrawControlEnabled(Character *character, bool enabled);
void modelSetCharaAlpha(Character *character, float alpha);

bool ResolveCharacterWorldTransform(const Character *character, Matrix4x4 &transform);
bool ResolveCharacterDrawNodeTransform(const Character *character, std::uint32_t nodeId, Matrix4x4 &transform);
bool ResolveCharacterDrawNodeTranslation(const Character *character, std::uint32_t nodeId, Vector3 &translation);
CharacterControlSlot *GetCharacterInMapControlTask(std::int32_t characterId);
HLib::Task *GetCharacterHandlerTask(const Character *character);
using CharacterMapVisitor = void (*)(Character *, void *);
void ForEachLiveCharacterInMapOrder(CharacterMapVisitor visitor, void *context);
void CharDispCheck_Callback(void *taskData);
HLib::Task *CharDraw_Human_Object_Enqueue(std::uint8_t nextFunctionIndex);
std::int32_t GetNpcBatchCountOverride(std::int32_t unused);
void SetNewNpcBatchCountOverride(NpcFillType fillType, std::int32_t count);
void ExchangeCharacterDrawVector(const Vector4 *colour, Vector4 *previous);
Vector3i GetCharacterWorldRotation(const Character *character);
void SetCharacterWorldRotation(Character *character, const Vector3i &rotation);
void SetCharacterNodeWorldRotation(CharacterNode *node, const Vector3i &rotation);
void InverseTransformCharacterPoseByMatrix(const Matrix4x4 *transform, Vector3 *position, Vector3i *rotation);
void TransformCharacterPoseByMatrix(const Matrix4x4 *transform, Vector3 *position, Vector3i *rotation);
Vector3 GetCharacterWorldPosition(const Character *character);
float GetHorizontalDistance(const Vector3 *first, const Vector3 *second);
std::int16_t GetAllAngYForNode(const CharacterNode *root);
void shenDecrementRotYAllNodes(CharacterNode *root, std::int16_t rotationY);
void shenApplyTranslationToAll(CharacterNode *root, const Vector3 &translation);

#if defined(_WIN32)
using CharacterDebugCallback = void (*)();
extern CharacterDebugCallback g_characterDebugCallback;
extern std::int32_t g_characterDebugSnapCamera;
extern float g_characterDebugCameraRotation;
extern shen_name g_characterDebugSelectedName;
void DrawRegisteredCharacterDebug();
CharacterDebugCallback SetupCharacterDebug();
#endif

} // namespace shenmue::s2
