#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

#include "s1/task/char_hand.hpp"
#include "s1/task/char_cloth.hpp"
#include "s1/task/char_osage.hpp"
#include "s1/task/char_navigation.hpp"
#include "s1/task/char_image.hpp"

namespace shenmue::s1 {

inline constexpr shen_name COMPONENT_CPAM = SHEN_NAME(CPAM);

inline constexpr shen_name IMAGE_YKA_M = SHEN_NAME(YKAM);
inline constexpr shen_name IMAGE_YKB_M = SHEN_NAME(YKBM);
inline constexpr shen_name IMAGE_YKC_M = SHEN_NAME(YKCM);
inline constexpr shen_name IMAGE_YKD_M = SHEN_NAME(YKDM);
inline constexpr shen_name IMAGE_YKE_M = SHEN_NAME(YKEM);
inline constexpr shen_name IMAGE_YKF_M = SHEN_NAME(YKFM);

struct Character;
struct PackModel;
struct MdlmImageSlot;
struct Mt5RuntimeNode;
struct ScenarioContext;
struct MotionInformationWork;
struct HandModelWork;
struct PadCaptureEntry;
struct ClothAuthoredSphere;
struct ClothCollisionSphere;
struct ClothChain;



struct LookPointWork {
    Vector3 target;
    std::int16_t savedCommand;
    std::uint16_t reserved0e;
    std::uint32_t currentCommand;
    std::uint32_t active;
    std::uint32_t moveRequestLook;
};
static_assert(sizeof(LookPointWork) == 0x1c);

void LKPT_Callback(Character *character, void *payload);

#pragma pack(push, 1)

#pragma pack(pop)

#pragma pack(push, 1)



struct FightPoseWork {
    char filename[0x10];
    std::uint8_t active;
    std::uint8_t poseCacheColumns;
    std::uint8_t poseCacheRows;
    std::uint8_t previousActive;
    std::byte reserved014[4];
    PackModel *modelOwner;
    Mt5RuntimeNode *model;
    Matrix4x4 poseCache[45][15];
    void (*originalFigurePoseCallback)(Character *);
};
#pragma pack(pop)

static_assert(sizeof(FightPoseWork) == 0xa8f0);
static_assert(offsetof(FightPoseWork, active) == 0x10);
static_assert(offsetof(FightPoseWork, poseCacheColumns) == 0x11);
static_assert(offsetof(FightPoseWork, poseCacheRows) == 0x12);
static_assert(offsetof(FightPoseWork, previousActive) == 0x13);
static_assert(offsetof(FightPoseWork, modelOwner) == 0x18);
static_assert(offsetof(FightPoseWork, model) == 0x20);
static_assert(offsetof(FightPoseWork, poseCache) == 0x28);
static_assert(offsetof(FightPoseWork, originalFigurePoseCallback) == 0xa8e8);

#pragma pack(push, 1)

#pragma pack(pop)





#pragma pack(push, 1)

#pragma pack(pop)

#pragma pack(push, 1)
struct MotionInformationWork {
    std::uint32_t motionFlags;
    std::uint32_t rootAngleFlags;
    Vector3 *localTargets[10];
    Vector3 *worldTargets[10];
    std::uint8_t blendMode;
    std::uint8_t requestedInterpolationMode;
    std::uint8_t interpolationMode;
    std::uint8_t poseEvaluationStage;
    std::uint8_t reserved0ac;
    std::uint8_t poseEvaluationSucceeded;
    std::uint8_t updateMode;
    std::uint8_t reserved0af;
    std::uint8_t secondaryBlendSeed;
    std::uint8_t secondaryBlendCountdown;
    std::uint8_t secondaryBlendFrames;
    std::uint8_t secondaryBlendFramesPrevious;
    std::uint8_t queuedSecondaryBlendFrames;
    std::uint8_t reserved0b5;
    std::uint16_t pendingAnimId;
    std::uint16_t currentAnimId;
    std::uint16_t requestedAnimId;
    std::uint16_t previousAnimId;
    std::uint16_t lookupAnimId;
    std::uint16_t queuedBlendAnimId;
    std::uint16_t secondaryAnimId;
    std::int16_t secondaryNodeId;
    std::uint16_t secondaryLookupAnimId;
    std::uint16_t blendFramesRemaining;
    std::uint16_t blendFramesTotal;
    std::uint16_t blendFrameCount;
    std::uint16_t walkBlendAnimId;
    std::int16_t headLookTimer;
    std::int16_t cachedClampedLookAngle;
    std::uint32_t headLookAngleCommand;
    std::int16_t headLookAngleBase;
    std::int16_t headLookPreviousTimer;
    std::int32_t lookAtBlendTimer;
    std::int16_t lookAtState;
    std::int16_t lookAtMode;
    std::int16_t lookAtModeBackup;
    std::int32_t lookAtHoldTimer;
    std::uint16_t lookAtFlags;
    std::int16_t currentTurnAngle;
    std::int16_t turnCurveState[4];
    std::int16_t spineTurnCompensationAngle;
    std::int16_t targetTurnAngle;
    std::uint16_t deferredPrimaryAnimId;
    std::uint16_t queuedSecondaryAnimId;
    std::int16_t queuedSecondaryNodeId;
    void *nodeMatrixPool;
    void *previousNodes;
    void *activeNodes;
    void *blendTargetNodes;
    void *transitionNodes;
    void *secondaryNodes;
    void *sourceNodes;
    std::int64_t activeNodesCookie;
    std::int64_t blendTargetNodesCookie;
    std::int64_t transitionNodesCookie;
    std::int64_t secondaryNodesCookie;
    float playbackRate;
    float previousPlaybackRate;
    float smoothedFrameDelta;
    float frameStart;
    float frameEnd;
    float frameTimer;
    float frameTimerRef;
    float residualFrameTime;
    float previousFrameStart;
    float previousFrameEnd;
    float previousFrameTimer;
    float previousFrameTimerRef;
    float previousPlaybackRate2;
    float previousResidualFrameTime;
    float secondaryPlaybackRate;
    float secondaryFrameStart;
    float secondaryFrameEnd;
    float secondaryResidualFrameTime;
    float secondaryFrameTimer;
    float secondaryFrameTimerRef;
    float scriptStartFrame;
    float scriptEndFrame;
    float forcedFrameTime;
    float blendScratch[4];
    float interpolationAmount;
    float handRotationBlend;
    Vector3 rootPreTranslate;
    Vector3 previousRootPreTranslate;
    Vector3 rootWorldPosition;
    float rootBlendScratch[3];
    Vector3 rootMotionDelta;
    Vector3i entityEuler;
    Vector3i previousEntityEuler;
    std::int16_t extractedYaw;
    std::int16_t extractedYawAux;
    Vector3i nextEntityEuler;
    void *mobjCallback;
    void *mobjCallbackUser;
    void *motionCallback;
    void *motionCallbackUser;
    std::int32_t basePoseTemplateIndex;
    std::int32_t previousBasePoseTemplateIndex;
    std::int32_t activeBasePoseAnimIndex;
    std::int32_t nextBasePoseAnimIndex;
    Vector3 basePoseAnklePosition[2];
    Vector3 rootOffsetPosition;
    Vector3 previousRootOffsetPosition;
    std::int32_t soundCommand;
    std::int32_t trackedNodeIndex;
    void *nodeConstraints;
    Matrix4x4 *externalTransform;
    std::int32_t externalTransformAnchorNodeId;
    std::uint32_t reserved2ac;
    void *nodeOffsetPairCursor;
    Vector3i pendingTurnEulerDelta;
    Vector3i activeTurnEulerDelta;
    Vector3i previousTurnEulerDelta;
    Vector3i lastTurnEulerDelta;
    std::int32_t rightHandRotationAngle;
    std::int32_t leftHandRotationAngle;
    float queuedSecondaryStartFrame;
    float queuedSecondaryEndFrame;
    std::uint32_t queuedSecondaryFlags;
    std::uint32_t motionControlLock;
    std::uint64_t reserved300;
    std::uint64_t frameContactOverlay;
    std::int32_t previousFootContactTargetIndex[2];
    float contactTargetRadiusBias;
    float contactIKBlend;
    Vector3 footTargetCurrent[2];
    Vector3 footTargetDamping[2];
    Vector3 cachedAnklePosition[2];
    std::byte reserved368[0x18];
    std::int32_t leftAnkleTargetBlendFrames;
    std::int32_t rightAnkleTargetBlendFrames;
    std::uint32_t contactIKEnabled;
    float secondaryBlendFrameScale;
    float baseMotionScale;
    float motionScaleBlendStart;
    std::int32_t motionScaleFrames;
    float currentMotionScale;
    float targetMotionScale;
    Matrix4x4 rootTransformCache;
    float rootTransformBlend;
    Vector3 appliedPositionOffset;
    std::uint32_t spineYaw;
    Vector3 targetPositions[10];
    std::byte reserved470[8];
};

struct MotionManagerWork {
    std::uint32_t flags;
    std::byte cue20Values[0x10];
    std::uint32_t reserved014;
    MotionInformationWork motion;
    Character *transformSourceCharacter;
    std::uint32_t transformModelEntryCount;
    Vector3 transformTranslationOffset;
    Vector3i transformEulerOffset;
    Matrix4x4 transformMatrixCache;
    std::byte reserved4f4[4];
};
#pragma pack(pop)

static_assert(sizeof(MotionInformationWork) == 0x478);
static_assert(offsetof(MotionInformationWork, pendingAnimId) == 0xb6);
static_assert(offsetof(MotionInformationWork, playbackRate) == 0x158);
static_assert(offsetof(MotionInformationWork, forcedFrameTime) == 0x1b0);
static_assert(offsetof(MotionInformationWork, rootWorldPosition) == 0x1e4);
static_assert(offsetof(MotionInformationWork, entityEuler) == 0x208);
static_assert(offsetof(MotionInformationWork, activeBasePoseAnimIndex) == 0x258);
static_assert(offsetof(MotionInformationWork, basePoseTemplateIndex) == 0x250);
static_assert(offsetof(MotionInformationWork, motionControlLock) == 0x2fc);
static_assert(offsetof(MotionInformationWork, rootOffsetPosition) == 0x278);
static_assert(offsetof(MotionInformationWork, headLookAngleCommand) == 0xd4);
static_assert(offsetof(MotionInformationWork, turnCurveState) == 0xee);
static_assert(offsetof(MotionInformationWork, targetPositions) == 0x3f8);
static_assert(offsetof(MotionInformationWork, previousNodes) == 0x108);
static_assert(offsetof(MotionInformationWork, walkBlendAnimId) == 0xce);
static_assert(offsetof(MotionInformationWork, activeNodes) == 0x110);
static_assert(offsetof(MotionInformationWork, previousEntityEuler) == 0x214);
static_assert(offsetof(MotionInformationWork, extractedYaw) == 0x220);
static_assert(offsetof(MotionInformationWork, trackedNodeIndex) == 0x294);
static_assert(offsetof(MotionInformationWork, lastTurnEulerDelta) == 0x2dc);
static_assert(offsetof(MotionInformationWork, spineYaw) == 0x3f4);
static_assert(sizeof(MotionManagerWork) == 0x4f8);
static_assert(offsetof(MotionManagerWork, motion) == 0x18);
static_assert(offsetof(MotionManagerWork, motion) + offsetof(MotionInformationWork, poseEvaluationStage) == 0xc3);
static_assert(offsetof(MotionManagerWork, transformSourceCharacter) == 0x490);

#pragma pack(push, 4)
struct CharacterNode {
    CharacterNode *child;
    Vector3 position;
    Vector3i rotation;
};

struct CharacterIdTransform {
    CharacterNode node;
    std::uint32_t layer;
    const void *collisionData;
};

static_assert(sizeof(CharacterIdTransform) == 0x2c);
static_assert(offsetof(CharacterIdTransform, layer) == 0x20);
static_assert(offsetof(CharacterIdTransform, collisionData) == 0x24);

struct CharacterEntity {
    std::uint32_t name;
    CharacterNode node;
};

struct CharacterComponentNode {
    CharacterComponentNode *greaterChild;
    CharacterComponentNode *lessChild;
    std::uint32_t componentId;
    std::uint32_t reserved_14;
    CharacterComponentNode *next;
    std::uint32_t flags;
    std::uint32_t reserved_24;
    void (*updateCallback)(Character *, void *);
    void (*destroyCallback)(Character *, void *);
    void (*drawCallback)(Character *, void *);
};


struct CharacterShadowState {
    std::byte reserved_00[8];
    std::int32_t lightIndex[2];
    float fade[2];
    std::uint32_t flag[2];
};

union CharacterFlags {
    std::uint32_t value;
    std::byte bytes[4];
};

struct Character {
    CharacterEntity entity;
    std::uint32_t eventGroup;
    void *collisionQuery;
    Vector3 rootMotionDelta;
    Vector3 scale;
    void *model;
    void *shadowModel;
    float renderOpacity;
    float viewDistance;
    Vector3 viewPosition;
    CharacterFlags flags;
    Vector3 shadowPosition;
    Vector3 shadowScale;
    std::uint32_t shadowYaw;
    std::int32_t shadowMode;
    float shadowHeight;
    std::uint32_t skipRootMotionDelta;
    std::uint8_t motionFlags;
    std::uint8_t reserved_99;
    std::uint16_t allocatedSize;
    float clipAngle;
    std::int32_t groupId;
    void *componentAllocationBlocks;
    CharacterComponentNode *componentList;
    CharacterComponentNode *componentTree;
    void *unknownAllocationBlocks;
    std::int16_t unknown_C4;
    std::int16_t unknown_C6;
    void (*figurePoseCallback)(Character *);
    void (*processShadowCallback)(Character *);
    void (*drawCallback)(void *, float);
    void (*alphaDrawCallback)(void *, float);
    std::uint32_t lightMask;
    float lightRadius;
    CharacterShadowState shadowState;
};

struct CharacterBucketNode {
    CharacterBucketNode *next;
    Character *character;
    HLib::Task *task;
};

struct CharacterIdLookupNode {
    CharacterIdLookupNode *left;
    CharacterIdLookupNode *right;
    std::uint32_t id;
    std::uint32_t reserved_14;
    CharacterIdLookupNode *bucketNext;
    Character *character;
};
#pragma pack(pop)

static_assert(sizeof(CharacterNode) == 0x20);
static_assert(sizeof(CharacterEntity) == 0x24);
static_assert(sizeof(CharacterComponentNode) == 0x40);
static_assert(sizeof(CharacterShadowState) == 0x20);
static_assert(sizeof(Character) == 0x110);
static_assert(offsetof(Character, model) == 0x48);
static_assert(offsetof(Character, flags) == 0x6c);
static_assert(offsetof(Character, componentList) == 0xac);
static_assert(offsetof(Character, componentTree) == 0xb4);
static_assert(sizeof(CharacterBucketNode) == 0x18);
static_assert(sizeof(CharacterIdLookupNode) == 0x28);

#pragma pack(push, 1)

struct ReferenceBoardWork {
    void (*figurePoseCallback)(Character *);
    std::int32_t reserved008;
    Vector3 colour;
    std::uint32_t sector;
    std::int32_t mode;
    std::int32_t type;
    std::int32_t field024;
    std::uint32_t status;
    float radius;
};
#pragma pack(pop)

static_assert(sizeof(ReferenceBoardWork) == 0x30);
static_assert(offsetof(ReferenceBoardWork, colour) == 0xc);
static_assert(offsetof(ReferenceBoardWork, status) == 0x28);

ReferenceBoardWork *FindReferenceBoardWork(Character *character);
void GAME_BindModelOffsets(void *model, Character *character);
void GAME_RestoreModelOffsets(void *model, Character *character);

enum class CharacterHandlerPriority : std::uint32_t {
    PreHigh = 0,
    High = 1,
    Normal = 2,
    Low = 3,
};


constexpr std::array<std::uint16_t, 4> kCharacterHandlerTaskIndices{5, 6, 7, 8};

constexpr std::uint32_t CharacterFlagUnknown = 0x1;
constexpr std::uint32_t CharacterFlagVisible = 0x4;
constexpr std::uint32_t CharacterFlagActive = 0x8;
constexpr std::uint32_t CharacterFlagWithinVisibleDistance = 0x40;
constexpr std::uint32_t CharacterFlagReflectInScriptPlane = 0x80;
constexpr std::uint32_t CharacterFlagSkipShadowGroundCheck = 0x100;
constexpr std::uint32_t CharacterFlagHasCpct = 0x400;

Character *HLTaskFunc_CharacterHandler_Enqueue(std::uint16_t taskIndex, HLib::Task **taskOut);
void HLTaskFunc_CharacterHandler_Callback(HLib::TaskData *taskData);
Character *AddCharacterHandler(CharacterHandlerPriority priority);
void *AllocateHeapMemoryMODL(Character *character, std::uint32_t componentId, std::int32_t payloadSize, void (*updateCallback)(Character *, void *));
CharacterComponentNode *HLib_GetCharacterComponent(Character *character, std::uint32_t componentId);
void SetHeapMemoryFreeCallback(Character *character, std::uint32_t componentId, void (*destroyCallback)(Character *, void *));
void CEWP_ToggleVisibility_Callback(Character *character, std::int32_t enabled);
std::int32_t GetObjectAngle(const CharacterNode *node);
void GetEntityNodePos(Character *character, Vector3 *output);
CharacterNode *shenGetNodeFromChara(Character *character);
void NPCLogger(Character *character, std::int32_t index, std::int32_t enabled);
void RotateAndPostTranslate(Character *character);
void TransformCharacterHierarchyZYX(const CharacterNode *node);
extern Character *g_drawingCharacter;



extern void *g_drawingImageModel;
std::uint32_t charGetLightMask(const Character *character);
float charGetLightRadius(const Character *character);
std::uint32_t charaGetAvoidanceRes(Character *character);
float cewpGetSpeedFor(Character *character);

void CHID_Init(HLib::Task *scenarioTask, std::uint32_t id, Character *character);
Character *CHID_Find(HLib::Task *scenarioTask, std::uint32_t id);
void CHAR_DestroyAndFreeComponents(Character *character);
void SCEN_UnlinkCharacterIndexNode(HLib::Task *scenarioTask, Character *character);
void SCEN_PromoteCharacterIndexNode(HLib::Task *scenarioTask, Character *character);
Vector3 GetPositionForEntity(CharacterNode &node);
Vector3 TransformPointByEntity(CharacterNode &node, const Vector3 &point);
Vector3 WorldToEntityLocalPoint(CharacterNode &node, const Vector3 &point);
void SetupTransformationMatrix(Character *character, std::uint32_t nodeKind, Matrix4x4 *output);
bool CHAR_TransformModelNodePoint(Character *character, std::uint32_t nodeKind, const Vector3 &localPoint, Vector3 *output);
Vector3i CHAR_EulerAnglesFromMatrix(const Matrix4x4 &matrix);
void Matrix4x4_ExtractEulerAngles(Vector3i &angles);
void Shenmue_RotatePointByCurrentTransform(const CharacterNode *node, Vector3 &point);
Vector3i GetEulerAnglesForEntity(CharacterNode &node);
std::int32_t charaGetWorldYaw(Character *character);
std::uint32_t cpctGetAccumYaw(Character *character);
void ApplyEulerAnglesToNode(CharacterNode &node, const Vector3i &eulerAngles);
void shenApplyTranslationToAll(CharacterNode &node, const Vector3 &worldPosition);
void Character_SubmitForDrawing(Character *character);
void CharacterHandler_DrawSubmitAndScale(void *model, float opacity);
void CharacterHandler_AlphaDraw(void *model, float opacity);
void MOTM_DrawPlayerHuman(void *model, float opacity);
void KNIF_FigurePose(Character *character);
void VEND_Initialize(Character *character, std::int32_t variant);
void VEND_ReleaseModels();
void ccowEnableCharaFlags(Character *character, std::uint32_t flags);
void ccowSetGroundDropLimit(Character *character, float value);
bool CCOW_UpdateCandidateObject(Character *character, std::int32_t mode, std::uint32_t objectId);
void ccowDisableCharaFlags(Character *character, std::uint32_t flags);
bool ccowHasAnyCharaFlags(Character *character, std::uint32_t flags);
std::int32_t ccowSetOrGetValue(Character *character, std::int32_t value);
void ccowSetDistanceInteractLimits(Character *character, float distanceLimit, std::int32_t interactionHeightBits, std::int32_t maxAngle);
void CCOW_ApplyAuthoredDefinition(Character *character, std::int8_t level, bool disabled, std::uint32_t stateFlags, std::uint32_t maxAngle, float distanceLimit,
    std::uint32_t interactionHeightBits, const Vector3 &shapeExtents, float radialExtent, std::uint32_t shapeMode, std::uint32_t weightBits);
bool ccowConsumeInteractionFlag(Character *character);
void mapEventSetEnabled(Character *character, std::uint32_t index, bool enabled);
bool mapEventIsEnabled(Character *character, std::uint32_t index);
std::int32_t mapEventCurrentId(Character *character);
void motmEnableFlags(Character *character, std::uint32_t flags);
void motmDisableFlags(Character *character, std::uint32_t flags);
bool motmSetBasePoseMode(Character *character, std::int32_t mode);
MotionManagerWork *MOTM_GetWork(Character *character);
MotionInformationWork *MOTN_getMOTM(Character *character);
const Matrix4x4 *GetTransformForFACE_ForSpecificChara(Character *character, std::int32_t boneId);
void Matrix4x4_TransformMatrixAndCalculateEulerAngles(const Matrix4x4 *transform, Vector3 &position, Vector3i &rotation);
void mtwkGetCharaBonePosition(Character *character, std::int32_t nodeId, Vector3 *position);
float scenGetCharaPlayRateScaledRemain(ScenarioContext *context, shen_name characterId);
void scenCharaSetMotForcedFrame(ScenarioContext *context, shen_name characterId, float frame);
void scenCharaSetMotEndFrame(ScenarioContext *context, shen_name characterId, float frame);
void scenToggleCharaMotEnabled(ScenarioContext *context, shen_name characterId, bool enabled);
void scenSetCharaPos(ScenarioContext *context, shen_name characterId, const Vector3 &position);
void scenGetCharaBonePos(ScenarioContext *context, shen_name characterId, std::uint8_t nodeId, Vector3 &position);
void scenGetCharaPos(ScenarioContext *context, shen_name characterId, Vector3 &position);
void scenSetCharaRot(ScenarioContext *context, shen_name characterId, const Vector3i &rotation);
void scenGetCharaRot(ScenarioContext *context, shen_name characterId, Vector3i &rotation);
void scenApplyCharaRot(ScenarioContext *context, shen_name targetId, shen_name sourceId, const Vector3 &offset, std::int16_t yawOffset);
bool shenPushCharaAwayFromOtherChara(ScenarioContext *context, shen_name targetId, shen_name sourceId);
void SetCharaLookAtPosition(Character *character, std::int32_t angle, const Vector3 *target, std::int32_t mode);
void SetLookPointDetails(Character *character, std::int32_t angle, const Vector3 *target, std::int32_t mode);
void SetupTransformMatrixForMotionTask(Character *target, std::int32_t anchorNodeId, Character *source, std::uint32_t sourceModelNodeKind,
    const Vector3 *translationOffset, const Vector3i *eulerOffset);
#pragma pack(push, 1)
struct FixedObjectWork {
    Vector3 position;
    Vector3i rotation;
    Vector3 attachmentPosition;
    Vector3i attachmentRotation;
    std::uint16_t state;
    std::uint8_t flags;
    std::uint8_t reserved33;
    Vector3 offsetPosition;
    Vector3i offsetRotation;
    std::uint32_t reserved4c;
    std::uint64_t sourceNodeOrMatrix;
    Character *owner;
};
#pragma pack(pop)

static_assert(sizeof(FixedObjectWork) == 0x60);
static_assert(offsetof(FixedObjectWork, state) == 0x30);
static_assert(offsetof(FixedObjectWork, offsetPosition) == 0x34);
static_assert(offsetof(FixedObjectWork, sourceNodeOrMatrix) == 0x50);
static_assert(offsetof(FixedObjectWork, owner) == 0x58);

FixedObjectWork *FindFixedObjectWork(Character *character);
void AllocateFIXO(Character *character);
void CalculateMatrixRotationFromParamsHelper(Character *target, Character *source, std::int32_t nodeId);
void shenChainToTargetEval(Matrix4x4 *output, const Mt5RuntimeNode *root, const Mt5RuntimeNode *stop);
void fixoSetCharaPos(Character *target, Character *owner, std::int32_t nodeId, const Vector3 *position, const Vector3i *rotation);
void fixoClearAttachment(Character *character);
void fixoSetAttachmentOffset(Character *character, const Vector3 *position, const Vector3i *rotation);
bool fixoSetAttachmentOffsetFromNode(Character *character, std::int16_t nodeKind);

void STEP_Allocate(Character *character, std::uint8_t surface, std::uint8_t authoredMode);
void CLOK_Allocate(Character *character, std::int16_t minuteOffset);
void GAME_Allocate(Character *character);
void CALE_Allocate(Character *character);

void GAME_CommitStagedEntries(Character *character);
void GAME_StageNodeData(Character *character, std::uint32_t nodeId, const Vector3 *position, float renderAlpha, const Vector3 *basis);
void GAME_ConfigureNumericDisplay(Character *character, std::uint32_t baseNodeId, const Vector3 &digitStep, std::uint32_t maxValue, std::int32_t decimalShift);
void GAME_StageNumericDisplayValue(Character *character, std::uint32_t value, const Vector3 *origin, float renderAlpha, const Vector3 *basis);
FightPoseWork *FIGP_GetWork(Character *character);
FightPoseWork *FIGP_Allocate(Character *character, const char *blurModelFilename);
void FIGP_LoadBlurModel(Character *character, const char *filename);
void MOTN_Alloc(Character *character, bool shouldInitializeWithoutBuffers);
bool SPSS_Init();
void TMNM_Alloc(Character *character);
void TMNM_SetParameters(
    Character *character, std::uint32_t animationId, float startFrame, float endFrame, float currentFrame, float frameStep, std::int32_t flags);
bool TMNM_HasEnded(Character *character);
void TMNM_Reset(Character *character);
std::int32_t getMOTNType(std::int32_t nodeId);
void AssignMotionWorldTarget(MotionInformationWork &motion, std::size_t slot, const Vector3 &target);
void MOTM_SetTarget(Character *character, std::int32_t nodeId, const Vector3 *target, std::uint32_t flags);
extern std::int32_t g_OSAG_Val_0[6];
extern std::int32_t g_OSAG_Val_1[2];
extern float g_OSAG_Val_2;
extern float g_OSAG_Val;
void AllocREFB(Character *character, std::int32_t sector);
void refbSetColour(Character *character, const Vector3 &colour);
void refbSetSector(Character *character, std::uint32_t sector);
void SetRefBoardValues(Character *character, std::int32_t type, std::int32_t field024);
void refbSetGlobalActive(std::uint32_t value);


extern Vector3 g_refBoardSavedDirection;
extern Vector4 g_refBoardSavedColour;
extern std::int32_t g_bDebugLogVisibility;

extern char g_currentCharacterName[5];
void CHAR_LogNPCVisibility(Character *character, bool visible);
std::int32_t SetCharacterDrawMode(std::uint32_t mode, std::int32_t value);
void SetCharacterRenderOpacity(Character *character, float opacity);
void SetCharacterVisibility(Character *character, bool visible);
void CPAM_Alloc(Character *character);
void CPAM_SetEnabled(Character *character, bool enabled);



void Script_motmGetCharaFlags(ScenarioContext *context, void *rawParams);
std::int64_t ForEachCewpEntityApplyCallback(std::int64_t parameter, bool (*callback)(Character *));
void Script_CewpFind(ScenarioContext *context, void *rawParams);
void Script_SetCCOW_Flags(ScenarioContext *context, void *rawParams);
void Script_ccowUnknown(ScenarioContext *context, void *rawParams);
void Script_motnUtils(ScenarioContext *context, void *rawParams);
std::int64_t CHAR_SCNFApplyFacingTowardPoint(ScenarioContext *context, void *rawParams);
std::int64_t CHAR_SCNFAssignStand(ScenarioContext *context, void *rawParams);
std::int64_t CHAR_SCNFApplyFacing(ScenarioContext *context, void *rawParams);

struct CalendarPhaseMonth {
    std::uint8_t previousMonth;
    std::uint8_t dayAtMonthStart;
    std::uint8_t nextMonth;
    std::uint8_t boundaryDay;
};

inline constexpr std::array<CalendarPhaseMonth, 372> kCalendarPhaseMonths{{
    {11, 21, 12, 10},
    {12, 23, 1, 9},
    {1, 21, 2, 10},
    {2, 23, 3, 9},
    {3, 23, 4, 9},
    {4, 24, 5, 7},
    {5, 25, 6, 7},
    {6, 26, 7, 6},
    {7, 27, 8, 4},
    {8, 28, 9, 4},
    {9, 29, 10, 2},
    {10, 30, 11, 2},
    {12, 2, 1, 29},
    {1, 4, 2, 28},
    {2, 2, 3, 29},
    {3, 4, 4, 28},
    {4, 4, 5, 28},
    {5, 5, 6, 26},
    {6, 6, 18, 26},
    {18, 7, 7, 24},
    {7, 7, 8, 23},
    {8, 9, 9, 23},
    {9, 10, 10, 21},
    {10, 11, 11, 21},
    {11, 12, 12, 19},
    {12, 14, 1, 18},
    {1, 13, 2, 18},
    {2, 15, 3, 16},
    {3, 16, 4, 16},
    {4, 17, 5, 14},
    {5, 18, 6, 14},
    {6, 19, 7, 12},
    {7, 21, 8, 11},
    {8, 21, 9, 11},
    {9, 22, 10, 9},
    {10, 23, 11, 9},
    {11, 24, 12, 8},
    {12, 1, 1, 6},
    {1, 24, 2, 8},
    {2, 25, 3, 6},
    {3, 26, 4, 5},
    {4, 28, 5, 4},
    {5, 28, 6, 3},
    {6, 30, 7, 2},
    {8, 2, 9, 30},
    {9, 2, 10, 30},
    {10, 3, 11, 28},
    {11, 4, 12, 28},
    {12, 5, 1, 27},
    {1, 6, 2, 25},
    {2, 5, 3, 27},
    {3, 6, 4, 25},
    {4, 7, 5, 24},
    {5, 9, 17, 23},
    {17, 9, 6, 22},
    {6, 11, 7, 20},
    {7, 13, 8, 19},
    {8, 13, 9, 19},
    {9, 14, 10, 17},
    {10, 15, 11, 17},
    {11, 16, 12, 16},
    {12, 17, 1, 15},
    {1, 15, 2, 16},
    {2, 17, 3, 15},
    {3, 17, 4, 14},
    {4, 19, 5, 12},
    {5, 20, 6, 12},
    {6, 21, 7, 10},
    {7, 23, 8, 8},
    {8, 24, 9, 8},
    {9, 25, 10, 6},
    {10, 26, 11, 6},
    {11, 27, 12, 5},
    {12, 28, 1, 4},
    {1, 27, 2, 4},
    {2, 29, 3, 3},
    {3, 29, 4, 3},
    {5, 1, 6, 30},
    {6, 2, 7, 30},
    {7, 3, 8, 28},
    {8, 5, 9, 26},
    {9, 6, 10, 26},
    {10, 7, 11, 24},
    {11, 8, 12, 24},
    {12, 9, 1, 23},
    {1, 10, 2, 21},
    {2, 9, 3, 23},
    {3, 10, 15, 22},
    {15, 10, 4, 21},
    {4, 12, 5, 20},
    {5, 12, 6, 19},
    {6, 14, 7, 18},
    {7, 15, 8, 16},
    {8, 16, 9, 15},
    {9, 18, 10, 14},
    {10, 18, 11, 13},
    {11, 20, 12, 12},
    {12, 21, 1, 10},
    {1, 20, 2, 12},
    {2, 21, 3, 11},
    {3, 21, 4, 11},
    {4, 22, 5, 9},
    {5, 23, 6, 9},
    {6, 24, 7, 7},
    {7, 26, 8, 6},
    {8, 26, 9, 5},
    {9, 28, 10, 3},
    {10, 29, 11, 3},
    {12, 1, 1, 31},
    {1, 2, 20, 25},
    {2, 1, 3, 31},
    {3, 2, 4, 30},
    {4, 2, 5, 29},
    {5, 4, 6, 28},
    {6, 4, 7, 28},
    {7, 5, 8, 26},
    {8, 7, 20, 25},
    {20, 7, 9, 24},
    {9, 9, 10, 23},
    {10, 9, 11, 22},
    {11, 11, 12, 20},
    {12, 13, 1, 19},
    {1, 12, 2, 19},
    {2, 14, 3, 18},
    {3, 14, 4, 17},
    {4, 16, 5, 16},
    {5, 16, 6, 16},
    {6, 17, 7, 14},
    {7, 19, 8, 13},
    {8, 19, 9, 12},
    {9, 21, 10, 11},
    {10, 21, 11, 11},
    {11, 22, 12, 9},
    {12, 24, 1, 8},
    {1, 22, 2, 9},
    {2, 24, 3, 7},
    {3, 25, 4, 7},
    {4, 26, 5, 5},
    {5, 27, 6, 5},
    {6, 28, 7, 3},
    {7, 30, 8, 2},
    {9, 0, 10, 31},
    {10, 2, 11, 30},
    {11, 2, 12, 30},
    {12, 3, 1, 28},
    {1, 5, 2, 27},
    {2, 3, 3, 28},
    {3, 5, 4, 26},
    {4, 6, 5, 26},
    {5, 7, 17, 24},
    {17, 8, 6, 23},
    {6, 10, 7, 22},
    {7, 11, 8, 21},
    {8, 11, 9, 20},
    {9, 13, 10, 19},
    {10, 13, 11, 19},
    {11, 14, 12, 18},
    {12, 15, 1, 16},
    {1, 14, 2, 18},
    {2, 15, 3, 16},
    {3, 16, 4, 15},
    {4, 18, 5, 14},
    {5, 18, 6, 13},
    {6, 20, 7, 11},
    {7, 22, 8, 10},
    {8, 22, 9, 9},
    {9, 24, 10, 8},
    {10, 24, 11, 8},
}};



inline constexpr std::int32_t kIdleHandCommands[]{
    11,
    0,
    0,
    0,
    15,
    0,
    6,
    0,
    120,
    0,
    11,
    0,
    3,
    0,
    15,
    0,
    5,
    0,
};

inline constexpr std::array<std::uint8_t, 15> kFightPoseSourceNodeIds{{
    2,
    47,
    15,
    16,
    18,
    9,
    10,
    12,
    20,
    31,
    32,
    34,
    24,
    25,
    27,
}};

inline constexpr std::array<std::int16_t, 15> kFightPoseModelNodeKinds{{
    1,
    3,
    5,
    6,
    8,
    10,
    11,
    13,
    14,
    16,
    17,
    18,
    21,
    22,
    23,
}};

struct MotionToModelBone {
    std::int32_t motionNodeId;
    std::int32_t reserved04;
    std::int32_t modelNodeKind;
    std::int32_t reserved0c;
};


inline constexpr std::array<MotionToModelBone, 27> kMotionToModelBones{{
    {0, 0, 0, 0},
    {2, 0, 1, 0},
    {37, 0, 2, 0},
    {5, 0, -67, 0},
    {13, 0, 4, 0},
    {15, 0, 5, 0},
    {16, 0, 6, 0},
    {18, 0, -65, 0},
    {7, 0, 9, 0},
    {9, 0, 10, 0},
    {10, 0, 11, 0},
    {12, 0, -66, 0},
    {20, 0, 14, 0},
    {29, 0, 15, 0},
    {31, 0, 16, 0},
    {32, 0, 17, 0},
    {34, 0, 18, 0},
    {22, 0, 20, 0},
    {24, 0, 21, 0},
    {25, 0, 22, 0},
    {27, 0, 23, 0},
    {40, 0, 55, 0},
    {41, 0, 56, 0},
    {43, 0, 57, 0},
    {45, 0, 58, 0},
    {44, 0, 59, 0},
    {46, 0, 119, 0},
}};

} // namespace shenmue::s1
