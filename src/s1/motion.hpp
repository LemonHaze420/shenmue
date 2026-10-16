#pragma once

#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>

namespace shenmue::s1 {

struct MotionInformationWork;
struct Character;
std::uint16_t Shenmue_Atan2Fixed(float first, float direction);
void MOTI_ApplyPositionOffset(MotionInformationWork &motion, const Vector3 &offset);
struct ScenarioContext;
extern std::int32_t g_motnHasYValue;
extern std::int32_t g_motnRowBytesInUse;

#pragma pack(push, 1)
struct MotnAnimationHeader {
    std::uint16_t attribute;
    std::uint16_t sourceRig;
    std::uint16_t block2Offset;
    std::uint16_t block3Offset;
    std::uint16_t block4Offset;
    std::uint16_t block5Offset;
};

struct MotnHandCommandEvent {
    std::uint16_t frame;
    std::uint8_t eventType;
    std::uint8_t sideOrVariant;
    std::uint8_t commandIndex;
    std::uint8_t reserved05;
    std::int16_t frameCount;
};
struct MotnSoundCommandEvent {
    std::uint16_t frame;
    std::uint8_t eventType;
    std::uint8_t soundBank;
    std::uint32_t soundCommand;
};
struct MotnFigpControlEvent {
    std::uint16_t frame;
    std::uint8_t eventType;
    std::uint8_t mode;
    std::uint8_t cacheColumns;
    std::uint8_t cacheRows;
    std::uint8_t reserved06[2];
};

struct MotionSequenceWork {
    std::uint32_t state;
    Vector3 targetPosition;
    std::uint32_t targetYaw;
    std::int32_t animationFlags;
    std::uint32_t directionBit;
    std::uint32_t yawDelta;
    float startFrame;
    float distance;
    float playbackRate;
};
struct MotionPointWork {
    Vector3 targetPosition;
    Vector3 residualPosition;
    std::uint32_t state;
    std::uint32_t animationId;
    float transitionFrames;
    float timeOffset;
    float originalPlaybackRate;
};




struct CharacterMoveRequestWork {
    Vector3 targetPosition;
    Vector3 workPosition;
    std::int32_t state;
    std::uint32_t mode;
    std::int32_t animationId;
    std::uint32_t locomotionPayload[24];
    std::uint32_t directionBit;
    std::int32_t targetYaw;
    std::int32_t yawDelta;
    std::int32_t currentYaw;
    float playbackRate;
    float savedPlaybackRate;
    float distance;
    float startFrame;
};
static_assert(sizeof(CharacterMoveRequestWork) == 0xa4);
static_assert(offsetof(CharacterMoveRequestWork, state) == 0x18);
static_assert(offsetof(CharacterMoveRequestWork, locomotionPayload) == 0x24);
static_assert(offsetof(CharacterMoveRequestWork, directionBit) == 0x84);
static_assert(offsetof(CharacterMoveRequestWork, targetYaw) == 0x88);
static_assert(offsetof(CharacterMoveRequestWork, playbackRate) == 0x94);
static_assert(offsetof(CharacterMoveRequestWork, startFrame) == 0xa0);

struct MotionRigNodeSource {
    std::uint8_t nodeId;
    std::uint8_t parentMode;
    std::uint8_t flipFlags;
    std::uint8_t flags;
    std::uint8_t keyframeSlots;
    std::uint8_t childCount;
    std::uint8_t positionMode;
    std::uint8_t nodeKind;
    Vector3 bindPosition;
    Vector3s bindEuler;
    std::byte reserved1a[6];
    const std::uint8_t *children;
};

struct MotnKeyframeChannel {
    std::uint16_t present;
    std::int16_t keyframeCount;
    std::uint32_t reserved04;
    const void *block3Data;
    const std::uint16_t *sizeData;
    const std::uint8_t *block4Data;
    const std::uint16_t *block5Begin;
    const std::uint16_t *block5Cursor;
    const std::uint16_t *block5End;
};

struct MotnPackedKeyframe {
    const std::uint16_t *data;
    float time;
    std::uint8_t type;
};

struct MotnFCVKey {
    float time;
    std::int16_t leftSlope;
    std::int16_t rightSlope;
    std::int16_t value;
    std::uint16_t reserved[3];
};

struct MotnKeyframeTriplet {
    MotnKeyframeChannel x;
    MotnKeyframeChannel y;
    MotnKeyframeChannel z;
};

struct MotionNodeRuntime {
    std::uint8_t nodeId;
    std::uint8_t templateParentOrMode;
    std::uint8_t flipFlags;
    std::uint8_t flags;
    std::uint8_t childCount;
    std::uint8_t channelSet;
    std::uint8_t nodeKind;
    std::uint8_t reserved07;
    std::uint8_t bindPositionPacked[2];
    Vector3s bindEuler;
    Vector4 effectorPosition;
    MotnKeyframeChannel *translationKeys;
    MotnKeyframeChannel *rotationKeys;
    const Vector3 *bindPosition;
    const Vector3s *bindEulerSource;
    MotionNodeRuntime *children[3];
    MotionNodeRuntime *pairedSourceNode;
    float interpolationAmount;
    float timeScale;
    Matrix4x4 *transform;
};



struct MotionNodeConstraintState {
    std::int32_t nodeId;
    Vector3 correction;
    Vector3 previousEffectorPosition;
    Vector3 persistentOffset;
    float radiusBias;
    std::uint8_t flags;
    std::uint8_t mode;
    std::uint16_t reserved2e;
};

struct MotionConstraintEndpoint {
    std::int32_t sourceId;
    std::int32_t nodeId;
    Vector3 position;
    float radius;
    std::uint16_t flags;
    std::uint16_t reserved1a;
};

struct MotionNodeConstraintWorkspace {
    MotionNodeConstraintState states[2];
    MotionConstraintEndpoint endpoints[2];
};

static_assert(sizeof(MotionNodeConstraintState) == 0x30);
static_assert(sizeof(MotionConstraintEndpoint) == 0x1c);
static_assert(sizeof(MotionNodeConstraintWorkspace) == 0x98);


struct MotionNodeOffsetPair {
    std::uint32_t reserved00;
    std::int32_t nodeId;
    Vector3s offset;
    std::uint8_t flags;
    std::uint8_t reserved0f;
};

static_assert(sizeof(MotionNodeOffsetPair) == 0x10);



struct MotionCubicSequenceScratch {
    Vector3 startTangent[10];
    Vector3 endTangent[10];
    Vector3 startAnchor[5];
    Vector3 endAnchor[5];
};

static_assert(sizeof(MotionCubicSequenceScratch) == 0x168);



struct MotnRawFrameContext {
    std::byte reserved00[8];
    Vector3 rootPosition;
    std::byte reserved14[4];
    const std::int16_t *currentWords[2];
    const std::int16_t *nextWords[2];
    const std::int16_t *previousWords;
    const std::int16_t *currentRecord;
    const std::int16_t *nextRecord;
    const std::int16_t *referenceRecord;
    const std::int16_t *referenceNextRecord;
    float fraction;
    float referenceFraction;
    std::byte reserved68[0xc];
    std::int32_t entityYaw;
    std::byte reserved78[4];
    std::uint8_t secondaryNodeId;
    std::uint8_t visible;
    std::uint16_t flags;
};

static_assert(sizeof(MotnRawFrameContext) == 0x80);
static_assert(offsetof(MotnRawFrameContext, currentWords) == 0x18);
static_assert(offsetof(MotnRawFrameContext, fraction) == 0x60);
static_assert(offsetof(MotnRawFrameContext, entityYaw) == 0x74);
static_assert(offsetof(MotnRawFrameContext, secondaryNodeId) == 0x7c);

// The MOTN clock globals one MOTN_ApplyFKPoseWithRootMotion pass samples against: g_fTimerRef, g_fCurrentFrameAccum,
// currentAnimNumFrames and g_fCurrentResidualTime around the frame being evaluated.
struct MotionSampleClock {
    float frame;
    float reference;
    float start;
    float end;
    float residual;
};




struct MotionRigNode {
    std::uint8_t nodeId;
    Vector3 position;
    std::int16_t euler[3];
    std::uint8_t children[4];
    std::uint8_t childCount;
    std::uint8_t parentMode;
    std::uint8_t flipFlags;
    std::uint8_t flags;
    std::uint8_t keyframeSlots;
    std::uint8_t positionMode;
    std::uint8_t nodeKind;
};

struct MotionRigType {
    const MotionRigNode *nodes;
    std::size_t nodeCount;
    const char *rigName;
    float scale;
};
#pragma pack(pop)

static_assert(sizeof(MotnAnimationHeader) == 0x0c);
static_assert(sizeof(MotionRigNodeSource) == 0x28);
static_assert(offsetof(MotionRigNodeSource, children) == 0x20);
static_assert(sizeof(MotnKeyframeChannel) == 0x38);
static_assert(offsetof(MotnKeyframeChannel, block3Data) == 0x08);
static_assert(sizeof(MotnKeyframeTriplet) == 0xa8);
static_assert(sizeof(MotnPackedKeyframe) == 0x0d);
static_assert(sizeof(MotnFCVKey) == 0x10);
static_assert(sizeof(MotnHandCommandEvent) == 0x08);
static_assert(sizeof(MotnSoundCommandEvent) == 0x08);
static_assert(sizeof(MotnFigpControlEvent) == 0x08);
static_assert(sizeof(MotionNodeRuntime) == 0x70);
static_assert(sizeof(MotionSequenceWork) == 0x2c);
static_assert(offsetof(MotionSequenceWork, targetPosition) == 0x04);
static_assert(offsetof(MotionSequenceWork, playbackRate) == 0x28);
static_assert(offsetof(MotionNodeRuntime, bindEuler) == 0x0a);
static_assert(offsetof(MotionNodeRuntime, translationKeys) == 0x20);
static_assert(offsetof(MotionNodeRuntime, transform) == 0x68);

const MotnAnimationHeader *MOTN_GetDataForAnimId(std::uint16_t animationId);
const std::byte *MOTN_GetReactionDataForAnimId(std::uint16_t animationId);
std::uint32_t MOTN_GetAnimationFlag(std::uint16_t animationId);
float MOTI_GetPlayRateScaledRemain(const MotionInformationWork &motion);
bool MOTI_RefreshInterpolationState(MotionInformationWork &motion);
void MOTI_SetRootTransformBlendBits(MotionInformationWork &motion, std::uint32_t value);
std::int32_t MOTN_GetFrameCountFromAnimId(std::uint16_t animationId);
std::uint16_t MOTN_ExtractEulerAngle(const MotionInformationWork &motion, std::uint8_t nodeId);
const std::uint8_t *shenLookupAnimationIndex(std::uint16_t animationId, std::uint8_t cueType);
const MotnHandCommandEvent *MOTN_FindHandCommandEvent(
    std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive, std::uint32_t occurrenceOneBased);
const MotnSoundCommandEvent *MOTN_FindSoundCommandEvent(std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive);
const MotnFigpControlEvent *MOTN_FindFigpControlEvent(std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive);
bool MOTN_FindFrameMarker(std::uint16_t animationId, float firstFrameInclusive, float lastFrameExclusive, std::uint16_t &frame);
void MOTN_UpdateFrameTimer(MotionInformationWork &motion);
void UNFO_Init(MotionInformationWork &motion);
void MOTI_InitAndAllocPrimaryBuffers(MotionInformationWork &motion);
void MOTI_Cleanup(MotionInformationWork &motion);
void MOTI_Free(MotionInformationWork &motion);
void MOTI_TransferPrimaryRows(MotionInformationWork *source, MotionInformationWork *destination);
void MOTI_BeginSecondaryBlend(Character *character);
void MOTI_SetBaseMotionScale(Character *character, float scale);
void MOTI_SetBlendFrameCount(Character *character, std::uint16_t frames);
std::uint16_t MOTI_GetCurrentAnimationId(Character *character);
void MOTI_SetTrackedNode(Character *character, std::int32_t nodeId);
void MOTI_SetSlowAngleRamp(Character *character, bool enabled);
std::uint32_t MOTI_UpdateLookPointAnimation(Character *character, const Vector3 *target);
std::uint32_t MOTI_UpdateLookPointAnimationFullBody(Character *character, const Vector3 *target);
void MOTI_StopLookPointAnimation(Character *character);
bool MOTI_AccessDetailFlag(Character *character, bool query, std::uint32_t value, std::uint32_t &result);
void MOTM_Cleanup(Character *character, void *payload);
void MOTM_UpdateSequenceNodeConstraints(const void *records, std::uint32_t count, std::int32_t frameIndex);
float ryoPickTurnAnimByYawDelta(Character *character, std::int32_t targetYaw);
std::int32_t FIGP_PlayFightAnim_Wrapper(Character *character, std::uint16_t animationId, std::int16_t nodeId, float startFrame, std::uint32_t flags);
const MotionRigType *MOTN_GetRigType(std::uint16_t rigTypeIndex);
void MOTN_InitRigNodes(MotionInformationWork &motion, MotionNodeRuntime *nodes, std::uint16_t rigTypeIndex, float timeScale, MotionNodeRuntime *pairedNodes);
bool MOTN_LoadAnimation(MotionInformationWork &motion, MotionNodeRuntime *nodes, std::uint16_t animationId, std::uint16_t targetRigIndex,
    MotnKeyframeTriplet *&keyStorage, MotionNodeRuntime *pairedNodes);
float MOTN_CalculateKeyframeForTime(MotnKeyframeChannel &channel, float currentTime, std::int32_t frameCount);
// mainMotionUpdate's evaluation context (g_pMotion, g_bBasePoseOnly, Shenmue::MOTN_n_g_bIsRyo, clipAngle, g_motn_data and
// g_tmp_motn_snd_bank), published by MOTM_Callback and ManageMotionAnimations.
extern MotionInformationWork *g_pMotion;
extern std::uint32_t g_motnBasePoseOnly;
extern std::uint32_t g_motnIsRyo;
extern float g_motnClipAngle;
extern MotionCubicSequenceScratch *g_motnCubicScratch;
extern std::uint8_t g_motnSoundBank;

extern float g_maxHandRotationBlend;
void MOTN_FootPlantingCallback(MotionInformationWork *motion, void *user);
bool MOTN_PlaySecondaryMotion(
    MotionInformationWork &motion, std::uint16_t animationId, std::int16_t nodeId, float frameStart, float frameEnd, std::uint32_t flags);
std::int32_t FIGP_PlayFightAnim_Wrapper(Character *character, std::uint16_t animationId, std::int16_t nodeId, float startFrame, std::uint32_t flags);
std::uint32_t motCalculateAnimationToPlay(Character *character, const Vector3 *target, bool sinusRig);
MotionNodeRuntime *MOTN_FindNodeRow(MotionNodeRuntime *row, std::int32_t nodeId);
Vector3 motGetEntityPos_Or_RotateEntity(Character *character, std::int32_t nodeId);
void MOTN_RequestSecondaryMotionStop(MotionInformationWork &motion);

std::int32_t motnGetRootMotionEndMarker(std::uint16_t animationId);
bool MOTN_ReadRootPositionAtFrame(std::uint16_t animationId, std::uint32_t basePoseTemplateIndex, float frame, Vector3 &outPosition, bool includeY = true);
void MOTN_ReadVfRootPosition(MotionInformationWork &motion, float frame, Vector3 &outPosition);
bool motCalcDeltaXZFromAnimTime(MotionInformationWork &motion, float frame, Vector3 &outPosition);
float MOTN_CalculateRootMotionSpeedZ(std::uint16_t animationId, std::uint16_t basePoseTemplateIndex = 0xffffU, bool *outUsedRootChannel = nullptr);
void motmApplyRootMot(const Character *character, const Vector3 &localPoint, Vector3 &worldPoint);
bool MOTN_GetEvaluatedNodeWorldPosition(const MotionInformationWork &motion, std::uint8_t nodeId, Vector3 &outPosition);
std::int32_t mainMotionUpdate(MotionInformationWork &motion);
void SetupAnimationSequence(Character *character, MotionSequenceWork &sequence);
void freeTalkConv_SetMoveAndFacingConstraint(Character &owner, const Vector3 &target, std::int32_t yaw, std::int32_t animationFlags, float startFrame,
    float playbackRate, float distance, bool forceAnimation);
void ManageMotionAnimations(Character *character);
void charaSubmitMoveRequest(Character *character, const Vector3 *target, std::int32_t targetYaw, std::int32_t animationId,
    const std::uint32_t *locomotionPayload, float playbackRate, std::uint32_t mode);
void charaCancelMoveRequest(Character *character, std::int32_t keepPosition);
bool charaIsActiveMode(Character *character, std::int32_t mode);
bool charaHasBlockingTPMRequest(Character *character);
bool WMPT_Request(Character *character, const Vector3 &target, std::uint32_t animationId, float timeOffset);
bool WMPT_IsActive(Character *character);
void MOTN_ClearSecondaryMotion(MotionInformationWork &motion);
void Script_ReadMOTNPositionAtFrame(ScenarioContext *context, void *rawParams);

namespace motion_rigs {

static constexpr MotionRigNode kMotnAkiraRigNodes[] = {
    {0, {0.000000f, 1.150400f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.251262f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.109731f, 0.000000f, 0.055835f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.049936f, 0.000000f, 0.035837f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16383}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.392729f, 0.000000f, 0.000000f}, {0, 0, 0}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.444608f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {-0.000001f, -0.088240f, -0.115665f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.109732f, 0.000000f, -0.055837f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.049937f, 0.000000f, -0.035835f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16329}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.392729f, 0.000000f, 0.000000f}, {0, 0, -102}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.444624f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, -0.088240f, -0.115665f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.380852f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.000000f, 0.000001f, 0.000001f}, {-32768, -16384, 0}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.209385f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.125631f, 0.000000f, -0.055834f}, {0, 0, 0}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {8, {0.016951f, 0.000000f, -0.168590f}, {0, 0, -16384}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {10, {0.242530f, 0.000000f, 0.000000f}, {0, 0, 0}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {11, {0.250658f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {13, {-0.125632f, 0.000000f, 0.055838f}, {0, 0, 0}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.016945f, 0.000000f, 0.168566f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16451}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.242525f, 0.000000f, 0.000000f}, {0, 0, 133}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.250665f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};
static constexpr MotionRigNode kMotnTogBigRigNodes[] = {
    {0, {0.000000f, 1.319535f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.220002f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.061230f, 0.000000f, 0.083410f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.031230f, 0.000000f, 0.101200f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16431}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.480113f, 0.000000f, 0.000000f}, {0, 0, -91}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.480408f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {-0.000010f, -0.093937f, -0.086896f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.061230f, 0.000000f, -0.083410f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.031230f, 0.000000f, -0.101200f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16431}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.480111f, 0.000000f, 0.000000f}, {0, 0, -95}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.480411f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, -0.094374f, -0.086896f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.682291f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.000000f, 0.000001f, 0.000001f}, {8191, -16384, 24575}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.209385f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.265070f, -0.010000f, -0.116040f}, {0, 0, 0}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {8, {0.062710f, 0.000000f, -0.213960f}, {0, 0, -16384}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16444}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {10, {0.300004f, 0.000000f, 0.000000f}, {0, 0, 119}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {11, {0.310002f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {13, {-0.265070f, -0.010000f, 0.116040f}, {0, 0, 0}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.062710f, 0.000000f, 0.213960f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16444}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.300005f, 0.000000f, 0.000000f}, {0, 0, 119}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.310000f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnChaRigNodes[] = {
    {0, {0.000000f, 1.120971f, 0.000000f}, {0, 0, -1}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.220000f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.078475f, -0.000003f, 0.055850f}, {0, -1, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.048479f, 0.000005f, 0.034145f}, {1, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16329}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.390000f, 0.000000f, 0.000000f}, {0, 0, -102}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.440000f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.078464f, 0.000004f, -0.055822f}, {0, -1, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.048461f, -0.000006f, -0.034183f}, {1, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16329}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.390000f, 0.000000f, 0.000000f}, {0, 0, -102}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.440000f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, -3, 16383}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.380000f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {-0.051221f, -0.050381f, 0.000114f}, {6421, -16377, 26352}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 1, 16384}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.210000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.124784f, -0.000003f, -0.055861f}, {0, 1, 0}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {8, {0.024804f, 0.000015f, -0.184136f}, {0, 0, -16384}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {435, -4, -16384}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {10, {0.250000f, 0.000000f, 0.000000f}, {0, 0, 0}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {11, {0.250000f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {13, {-0.124774f, 0.000004f, 0.055811f}, {0, 1, 0}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.024754f, -0.000015f, 0.184192f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, -0.000001f, 0.000000f}, {-485, -3, -16384}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.250000f, 0.000000f, 0.000000f}, {0, 0, 0}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.250000f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnDebRigNodes[] = {
    {0, {0.000000f, 1.181201f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.251262f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.109731f, 0.000000f, 0.069166f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.049936f, 0.000000f, 0.040723f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16433}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.431540f, 0.000000f, 0.000000f}, {0, 0, -100}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.433194f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {0.000000f, -0.086560f, -0.108250f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.109732f, 0.000000f, -0.069166f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.049937f, 0.000000f, -0.040694f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16434}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.431540f, 0.000000f, 0.000000f}, {0, 0, -100}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.433194f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, -0.086560f, -0.108250f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.396483f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.024000f, 0.000001f, 0.000001f}, {8191, -16384, 24575}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.209385f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.125631f, 0.000000f, -0.065850f}, {0, 0, 0}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {8, {0.009001f, 0.000000f, -0.171870f}, {0, 0, -16384}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {10, {0.260476f, 0.000000f, 0.000000f}, {0, 0, 0}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {11, {0.242725f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {13, {-0.125632f, 0.000000f, 0.065848f}, {0, 0, 0}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.009002f, 0.000000f, 0.171868f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16692}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.260476f, 0.000000f, 0.000000f}, {0, 0, 0}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.242725f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnGakKkoRigNodes[] = {
    {0, {0.000000f, 0.908311f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.176000f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.059945f, 0.000000f, 0.045785f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.035945f, 0.000000f, 0.026215f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.311999f, 0.000000f, 0.000000f}, {0, 0, 0}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.351999f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.059945f, 0.000000f, -0.045786f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.035945f, 0.000000f, -0.026214f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16322}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.311999f, 0.000000f, 0.000000f}, {0, 0, -115}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.351999f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.304000f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {-0.016000f, 0.000000f, 0.000100f}, {16384, -16378, 16383}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16385}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.168000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {13, {-0.094719f, 0.000000f, 0.045787f}, {0, 0, 0}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {14, {0.014719f, 0.000000f, 0.138213f}, {0, 0, -16384}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {15, {0.000001f, 0.000000f, 0.000000f}, {0, 0, -16384}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {16, {0.200000f, 0.000000f, 0.000000f}, {0, 0, 0}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {17, {0.200000f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {7, {-0.094719f, 0.000000f, -0.045784f}, {0, 0, 0}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {8, {0.014718f, 0.000000f, -0.138216f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {10, {0.200000f, 0.000000f, 0.000000f}, {0, 0, 0}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {11, {0.199999f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnGenRigNodes[] = {
    {0, {0.000000f, 1.289173f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.220000f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.066844f, 0.000000f, 0.067460f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.036844f, 0.000000f, 0.061103f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16333}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.447232f, 0.000000f, 0.000000f}, {0, 0, -104}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.456048f, 0.000000f, 0.000000f}, {-4, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {-0.013563f, -0.128601f, -0.103636f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.066844f, 0.000000f, -0.067460f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.036844f, 0.000000f, -0.061100f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16333}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.447232f, 0.000000f, 0.000000f}, {0, 0, -104}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.456048f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.013556f, -0.128637f, -0.103592f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.526336f, 0.000000f, 0.000000f}, {0, 0, 14}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.075672f, 0.007344f, 0.000142f}, {-24569, -16384, -8199}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, -7, 16384}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.210000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {13, {-0.163553f, 0.000225f, 0.101355f}, {0, 0, -14}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {14, {0.052529f, -0.000138f, 0.186214f}, {0, 0, -16369}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16332}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {16, {0.313157f, 0.000000f, 0.000000f}, {0, 0, 0}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {17, {0.295028f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {7, {-0.163553f, 0.000225f, -0.101360f}, {0, 0, -14}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {8, {0.052529f, -0.000138f, -0.186210f}, {0, 0, -16369}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16332}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {10, {0.313157f, 0.000000f, 0.000000f}, {0, 0, 0}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {11, {0.295028f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnOtherHumanStandardRigNodes[] = {
    {0, {0.000000f, 1.180000f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.251262f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.109731f, 0.000000f, 0.055835f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.049936f, 0.000000f, 0.035865f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16501}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.431540f, 0.000000f, 0.000000f}, {0, 0, -100}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.433194f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {0.000000f, -0.100254f, -0.113757f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.109732f, 0.000000f, -0.055837f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.049937f, 0.000000f, -0.035835f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16501}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.431540f, 0.000000f, 0.000000f}, {0, 0, -100}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.433194f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, -0.100254f, -0.113757f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.396483f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.024001f, 0.000001f, 0.000001f}, {8191, -16383, 24575}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.209385f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.125631f, 0.000000f, -0.055834f}, {0, 0, 0}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {8, {0.016944f, 0.000000f, -0.169966f}, {0, 0, -16384}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16438}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {10, {0.260476f, 0.000000f, 0.000000f}, {0, 0, 132}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {11, {0.242725f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {13, {-0.125632f, 0.000000f, 0.055838f}, {0, 0, 0}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.016945f, 0.000000f, 0.170000f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16438}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.260476f, 0.000000f, 0.000000f}, {0, 0, 132}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.242725f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnJijRigNodes[] = {
    {0, {0.000000f, 1.024343f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.219219f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.024990f, 0.000000f, 0.055835f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.000640f, 0.000000f, 0.034160f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16284}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.370072f, 0.000000f, 0.000000f}, {0, 0, -108}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.370157f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {0.000000f, -0.057268f, -0.089339f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.024991f, 0.000000f, -0.055837f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.000641f, 0.000000f, -0.034163f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16256}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.370072f, 0.000000f, 0.000000f}, {0, 0, -108}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.370157f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, -0.057268f, -0.089339f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.418314f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.000001f, 0.000031f, 0.000142f}, {8191, -16383, 24576}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.209385f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {13, {-0.134180f, 0.000000f, 0.055838f}, {0, 0, 0}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {14, {-0.000807f, -0.000138f, 0.149162f}, {0, 0, -16384}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16448}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {16, {0.257917f, 0.000000f, 0.000000f}, {0, 0, 0}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {17, {0.243285f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {7, {-0.134179f, 0.000000f, -0.055834f}, {0, 0, 0}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {8, {-0.000807f, -0.000138f, -0.149166f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16512}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {10, {0.257917f, 0.000000f, 0.000000f}, {0, 0, 132}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {11, {0.243285f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnKodRigNodes[] = {
    {0, {0.000000f, 0.722532f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.161813f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.047655f, 0.000000f, 0.035958f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.032159f, 0.000000f, 0.023079f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16201}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.250439f, 0.000000f, 0.000000f}, {0, 0, -130}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.267257f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {0.000000f, -0.040397f, -0.053021f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.047655f, 0.000000f, -0.035959f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.032160f, 0.000000f, -0.023078f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16201}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.250437f, 0.000000f, 0.000000f}, {0, 0, -129}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.267262f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, -0.040397f, -0.053021f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.245269f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.000000f, 0.000001f, 0.000001f}, {32767, -16384, 0}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.134844f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.080906f, 0.000000f, -0.035957f}, {0, 0, 0}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {8, {0.010916f, 0.000000f, -0.090163f}, {0, 0, -16384}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16375}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {10, {0.162930f, 0.000000f, 0.000000f}, {0, 0, 0}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {11, {0.154684f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {13, {-0.080907f, 0.000000f, 0.035960f}, {0, 0, 0}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.010918f, 0.000000f, 0.090162f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16455}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.162932f, 0.000000f, 0.000000f}, {0, 0, 165}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.154682f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnOtherHumanScript09RigNodes[] = {
    {0, {0.000000f, 1.199471f, 0.000000f}, {0, 0, 0}, {1, 16, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.251262f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 10, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.109731f, 0.000000f, 0.055835f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 10, 4},
    {30, {0.049936f, 0.000000f, 0.035865f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 11, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16506}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 12, 4},
    {32, {0.431540f, 0.000000f, 0.000000f}, {0, 0, -100}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {33, {0.433194f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 14, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 15, 0},
    {22, {-0.109732f, 0.000000f, -0.055837f}, {0, 0, 0}, {11, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.049937f, 0.000000f, -0.035835f}, {0, 0, 16384}, {12, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16602}, {13, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.431540f, 0.000000f, 0.000000f}, {0, 0, -100}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.433194f, 0.000000f, 0.000000f}, {0, 0, 0}, {15, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 0},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {17, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {18, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.396483f, 0.000000f, 0.000000f}, {0, 0, 0}, {19, 23, 29, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.024000f, 0.000001f, 0.000001f}, {8191, -16384, 24575}, {20, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {21, 22, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.209385f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.021256f, -0.000002f, 0.094219f}, {16384, 0, -16384}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.125631f, 0.000000f, -0.055834f}, {0, 0, 0}, {24, 0, 0, 0}, 1, 0, 0, 0, 1, 29, 3},
    {8, {0.016944f, 0.000000f, -0.169966f}, {0, 0, -16384}, {25, 0, 0, 0}, 1, 2, 2, 0, 0, 30, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16375}, {26, 0, 0, 0}, 1, 3, 1, 0, 0, 31, 4},
    {10, {0.260476f, 0.000000f, 0.000000f}, {0, 0, 0}, {27, 0, 0, 0}, 1, 3, 0, 0, 0, 32, 4},
    {11, {0.242725f, 0.000000f, 0.000000f}, {0, 0, 0}, {28, 0, 0, 0}, 1, 4, 0, 0, 0, 33, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 34, 3},
    {13, {-0.125632f, 0.000000f, 0.055838f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.016945f, 0.000000f, 0.170000f}, {0, 0, -16384}, {31, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16375}, {32, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.260476f, 0.000000f, 0.000000f}, {0, 0, 0}, {33, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.242725f, 0.000000f, 0.000000f}, {0, 0, 0}, {34, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnOtherHumanRigNodes[] = {
    {0, {0.000000f, 1.180000f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.251262f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.109731f, 0.000000f, 0.055835f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.049936f, 0.000000f, 0.035865f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16501}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.431540f, 0.000000f, 0.000000f}, {0, 0, -100}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.433194f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {0.000000f, -0.100254f, -0.113757f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.109732f, 0.000000f, -0.055837f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.049937f, 0.000000f, -0.035835f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16501}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.431540f, 0.000000f, 0.000000f}, {0, 0, -100}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.433194f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, -0.100254f, -0.113757f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.396483f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.024001f, 0.000001f, 0.000001f}, {8191, -16383, 24575}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.209385f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.125631f, 0.000000f, -0.055834f}, {0, 0, 0}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {8, {0.016944f, 0.000000f, -0.169966f}, {0, 0, -16384}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16438}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {10, {0.260476f, 0.000000f, 0.000000f}, {0, 0, 132}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {11, {0.242725f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {13, {-0.125632f, 0.000000f, 0.055838f}, {0, 0, 0}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.016945f, 0.000000f, 0.170000f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16438}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.260476f, 0.000000f, 0.000000f}, {0, 0, 132}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.242725f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnOtherHumanScript11RigNodes[] = {
    {0, {0.000000f, 1.145400f, 0.000000f}, {0, 0, 0}, {1, 16, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.251262f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 10, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.109731f, 0.000000f, 0.055835f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 10, 4},
    {30, {0.049936f, 0.000000f, 0.035837f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 11, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16329}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 12, 4},
    {32, {0.392729f, 0.000000f, 0.000000f}, {0, 0, -102}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {33, {0.444608f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 14, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 15, 0},
    {22, {-0.109732f, 0.000000f, -0.055837f}, {0, 0, 0}, {11, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.049937f, 0.000000f, -0.035835f}, {0, 0, 16384}, {12, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16274}, {13, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.392729f, 0.000000f, 0.000000f}, {0, 0, -102}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.444624f, 0.000000f, 0.000000f}, {0, 0, 0}, {15, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 0},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {17, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {18, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.380852f, 0.000000f, 0.000000f}, {0, 0, 0}, {19, 23, 29, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.000000f, 0.000001f, 0.000001f}, {-32768, -16384, 0}, {20, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {21, 22, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.209385f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.125631f, 0.000000f, -0.055834f}, {0, 0, 0}, {24, 0, 0, 0}, 1, 0, 0, 0, 1, 29, 3},
    {8, {0.016951f, 0.000000f, -0.168590f}, {0, 0, -16384}, {25, 0, 0, 0}, 1, 2, 2, 0, 0, 30, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16451}, {26, 0, 0, 0}, 1, 3, 1, 0, 0, 31, 4},
    {10, {0.242530f, 0.000000f, 0.000000f}, {0, 0, 133}, {27, 0, 0, 0}, 1, 3, 0, 0, 0, 32, 4},
    {11, {0.250658f, 0.000000f, 0.000000f}, {0, 0, 0}, {28, 0, 0, 0}, 1, 4, 0, 0, 0, 33, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 34, 3},
    {13, {-0.125632f, 0.000000f, 0.055838f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.016945f, 0.000000f, 0.168566f}, {0, 0, -16384}, {31, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16451}, {32, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.242525f, 0.000000f, 0.000000f}, {0, 0, 133}, {33, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.250665f, 0.000000f, 0.000000f}, {0, 0, 0}, {34, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnSypRigNodes[] = {
    {0, {0.000000f, 1.107851f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.220000f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.070000f, 0.000000f, 0.045000f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.030000f, 0.000000f, 0.040998f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16284}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.390000f, 0.000000f, 0.000000f}, {0, 0, -103}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.440000f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.070000f, 0.000000f, -0.045000f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.030002f, 0.000000f, -0.041001f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16447}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.390001f, 0.000000f, 0.000000f}, {0, 0, 0}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.439999f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.330000f, 0.000000f, 0.000000f}, {0, 0, 14}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {-0.010000f, 0.000000f, 0.000000f}, {32764, -16384, -3}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 14, 16383}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.210000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.092514f, 0.000127f, -0.065000f}, {0, 0, -14}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {8, {0.036455f, -0.000103f, -0.082004f}, {0, 0, -16355}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16479}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {10, {0.248000f, 0.000000f, 0.000000f}, {0, 0, 134}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {11, {0.247003f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {13, {-0.092514f, 0.000127f, 0.065000f}, {0, 0, -14}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.036450f, -0.000103f, 0.082004f}, {0, 0, -16355}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16479}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.245798f, 0.000000f, 0.000000f}, {0, 0, 133}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.249412f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnAkiraVariantRigNodes[] = {
    {0, {0.000000f, 1.150400f, 0.000000f}, {0, 0, 0}, {1, 18, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.251262f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 11, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.109731f, 0.000000f, 0.055835f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 11, 4},
    {30, {0.049936f, 0.000000f, 0.035837f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 12, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16383}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {32, {0.392729f, 0.000000f, 0.000000f}, {0, 0, 0}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 14, 4},
    {33, {0.444608f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 15, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {10, 0, 0, 0}, 1, 1, 0, 0, 0, 16, 0},
    {35, {-0.000001f, -0.088240f, -0.115665f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 17, 4},
    {22, {-0.109732f, 0.000000f, -0.055837f}, {0, 0, 0}, {12, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.049937f, 0.000000f, -0.035835f}, {0, 0, 16384}, {13, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16329}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.392729f, 0.000000f, 0.000000f}, {0, 0, -102}, {15, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.444624f, 0.000000f, 0.000000f}, {0, 0, 0}, {16, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 0},
    {28, {0.000000f, -0.088240f, -0.115665f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {19, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {20, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.380852f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 25, 31, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.000000f, 0.000001f, 0.000001f}, {-32768, -16384, 0}, {22, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {23, 24, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.209385f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.125631f, 0.000000f, -0.055834f}, {0, 0, 0}, {26, 0, 0, 0}, 1, 0, 0, 0, 1, 31, 3},
    {8, {0.016951f, 0.000000f, -0.168590f}, {0, 0, -16384}, {27, 0, 0, 0}, 1, 2, 2, 0, 0, 32, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {28, 0, 0, 0}, 1, 3, 1, 0, 0, 33, 4},
    {10, {0.242530f, 0.000000f, 0.000000f}, {0, 0, 0}, {29, 0, 0, 0}, 1, 3, 0, 0, 0, 34, 4},
    {11, {0.250658f, 0.000000f, 0.000000f}, {0, 0, 0}, {30, 0, 0, 0}, 1, 4, 0, 0, 0, 35, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 36, 3},
    {13, {-0.125632f, 0.000000f, 0.055838f}, {0, 0, 0}, {32, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.016945f, 0.000000f, 0.168566f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16451}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.242525f, 0.000000f, 0.000000f}, {0, 0, 133}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.250665f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnSypScriptRigNodes[] = {
    {0, {0.000000f, 1.155605f, 0.000000f}, {0, 0, 0}, {1, 16, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.220000f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 10, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.070000f, 0.000000f, 0.045000f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 10, 4},
    {30, {0.030000f, 0.000000f, 0.040998f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 11, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16284}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 12, 4},
    {32, {0.390000f, 0.000000f, 0.000000f}, {0, 0, -103}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {33, {0.440000f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 14, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 15, 0},
    {22, {-0.070000f, 0.000000f, -0.045000f}, {0, 0, 0}, {11, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.030002f, 0.000000f, -0.041001f}, {0, 0, 16384}, {12, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16447}, {13, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.390001f, 0.000000f, 0.000000f}, {0, 0, 0}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.439999f, 0.000000f, 0.000000f}, {0, 0, 0}, {15, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 0},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {17, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {18, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.330000f, 0.000000f, 0.000000f}, {0, 0, 14}, {19, 23, 29, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {-0.010000f, 0.000000f, 0.000000f}, {32764, -16384, -3}, {20, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 14, 16383}, {21, 22, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.210000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.092514f, 0.000127f, -0.065000f}, {0, 0, -14}, {24, 0, 0, 0}, 1, 0, 0, 0, 1, 29, 3},
    {8, {0.036455f, -0.000103f, -0.082004f}, {0, 0, -16355}, {25, 0, 0, 0}, 1, 2, 2, 0, 0, 30, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16479}, {26, 0, 0, 0}, 1, 3, 1, 0, 0, 31, 4},
    {10, {0.248000f, 0.000000f, 0.000000f}, {0, 0, 134}, {27, 0, 0, 0}, 1, 3, 0, 0, 0, 32, 4},
    {11, {0.247003f, 0.000000f, 0.000000f}, {0, 0, 0}, {28, 0, 0, 0}, 1, 4, 0, 0, 0, 33, 4},
    {12, {0.000191f, -0.003187f, 0.000000f}, {0, 0, -134}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 34, 3},
    {13, {-0.092514f, 0.000127f, 0.065000f}, {0, 0, -14}, {30, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.036450f, -0.000103f, 0.082004f}, {0, 0, -16355}, {31, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16546}, {32, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.245798f, 0.000000f, 0.000000f}, {0, 0, 132}, {33, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.249412f, 0.000000f, 0.000000f}, {0, 0, 0}, {34, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnSinRigNodes[] = {
    {0, {0.000000f, 1.052724f, 0.000000f}, {0, 0, 0}, {1, 16, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.210507f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 10, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.070000f, 0.000000f, 0.045000f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 10, 4},
    {30, {0.030000f, 0.000000f, 0.036428f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 11, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16328}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 12, 4},
    {32, {0.370992f, 0.000000f, 0.000000f}, {0, 0, -105}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {33, {0.418188f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 14, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 15, 0},
    {22, {-0.070000f, 0.000000f, -0.045000f}, {0, 0, 0}, {11, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.030002f, 0.000000f, -0.036433f}, {0, 0, 16384}, {12, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {13, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.370994f, 0.000000f, 0.000000f}, {0, 0, 0}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.418188f, 0.000000f, 0.000000f}, {0, 0, 0}, {15, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 0},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {17, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {18, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.330000f, 0.000000f, 0.000000f}, {0, 0, 14}, {19, 23, 29, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {-0.018070f, -0.000001f, 0.000000f}, {32764, -16384, -3}, {20, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {21, 22, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.210137f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.014831f, 0.000000f, 0.038460f}, {0, 8, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.092514f, 0.000127f, -0.065000f}, {0, 0, -14}, {24, 0, 0, 0}, 1, 0, 0, 0, 1, 29, 3},
    {8, {0.022570f, -0.000103f, -0.080000f}, {0, 0, -16384}, {25, 0, 0, 0}, 1, 2, 2, 0, 0, 30, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {26, 0, 0, 0}, 1, 3, 1, 0, 0, 31, 4},
    {10, {0.235414f, 0.000000f, 0.000000f}, {0, 0, 0}, {27, 0, 0, 0}, 1, 3, 0, 0, 0, 32, 4},
    {11, {0.234624f, 0.000000f, 0.000000f}, {0, 0, 0}, {28, 0, 0, 0}, 1, 4, 0, 0, 0, 33, 4},
    {12, {0.000089f, -0.000004f, -0.000350f}, {0, 0, 67}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 34, 3},
    {13, {-0.092514f, 0.000127f, 0.065000f}, {0, 0, -14}, {30, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.022574f, -0.000103f, 0.080000f}, {0, 0, -16384}, {31, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {32, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.235407f, 0.000000f, 0.000000f}, {0, 0, 0}, {33, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.234619f, 0.000000f, 0.000000f}, {0, 0, 0}, {34, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000306f, -0.000004f, 0.000350f}, {0, 0, 66}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnCatRigNodes[] = {
    {0, {0.000000f, 0.288388f, 0.000000f}, {0, 0, 0}, {1, 19, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -32655}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.161813f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 10, 16, 0}, 3, 4, 0, 0, 0, 0, 4},
    {22, {-0.006696f, 0.059266f, -0.065253f}, {0, 0, -112}, {5, 0, 0, 0}, 1, 1, 0, 0, 1, 10, 4},
    {23, {0.005260f, 0.075569f, 0.002031f}, {0, 0, 32767}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 11, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -23773}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 12, 4},
    {25, {0.081680f, 0.000000f, 0.000000f}, {0, 0, 14387}, {8, 0, 0, 0}, 1, 3, 0, 0, 0, 13, 4},
    {26, {0.076931f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 14, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 15, 0},
    {29, {-0.006694f, 0.059265f, 0.065249f}, {0, 0, -112}, {11, 0, 0, 0}, 1, 1, 0, 0, 1, 0, 4},
    {30, {0.005259f, 0.075570f, -0.002031f}, {-32767, 32767, 0}, {12, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -23773}, {13, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {32, {0.081680f, 0.000000f, 0.000000f}, {0, 0, 14387}, {14, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {33, {0.076931f, 0.000000f, 0.000000f}, {0, 0, 0}, {15, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 0},
    {40, {0.093352f, -0.006989f, -0.000008f}, {16327, -16383, 16327}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 4},
    {41, {0.000000f, 0.000000f, 0.068277f}, {0, 0, 0}, {18, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 4},
    {43, {0.000001f, 0.000000f, 0.084533f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {20, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 210}, {21, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.164453f, 0.000000f, 0.000000f}, {0, 0, -210}, {22, 29, 35, 0}, 3, 4, 0, 0, 0, 0, 4},
    {36, {0.000124f, -0.009239f, -0.000001f}, {0, 0, 3}, {23, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 4},
    {37, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 5118}, {24, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {38, {0.100537f, 0.000000f, 0.000000f}, {0, 0, 0}, {25, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {4, {-0.015596f, 0.010261f, 0.000001f}, {14962, -16383, 30207}, {26, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 4485, 16384}, {27, 28, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.128778f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.018925f, 0.000001f, 0.003676f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.014285f, -0.032439f, -0.039690f}, {0, 0, 13768}, {30, 0, 0, 0}, 1, 0, 0, 0, 1, 35, 3},
    {8, {-0.032868f, -0.014018f, -0.026031f}, {0, 0, -13768}, {31, 0, 0, 0}, 1, 2, 2, 0, 0, 36, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -21466}, {32, 0, 0, 0}, 1, 3, 1, 0, 0, 37, 4},
    {10, {0.118120f, 0.000000f, 0.000000f}, {0, 0, 10118}, {33, 0, 0, 0}, 1, 3, 0, 0, 0, 38, 4},
    {11, {0.116421f, 0.000000f, 0.000000f}, {0, 0, 0}, {34, 0, 0, 0}, 1, 4, 0, 0, 0, 39, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 40, 3},
    {13, {-0.014286f, -0.032439f, 0.039690f}, {0, 0, 13768}, {36, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {-0.032869f, -0.014020f, 0.026024f}, {0, 0, -13768}, {37, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -21466}, {38, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.118120f, 0.000000f, 0.000000f}, {0, 0, 10118}, {39, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.116421f, 0.000000f, 0.000000f}, {0, 0, 0}, {40, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnYagRigNodes[] = {
    {0, {0.000000f, 0.444074f, 0.000000f}, {0, 0, 0}, {1, 17, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, -0.000008f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 32705}, {3, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {21, {0.186699f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 10, 16, 0}, 3, 4, 0, 0, 0, 0, 4},
    {29, {-0.007013f, 0.062594f, 0.064843f}, {0, 0, 20096}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 10, 4},
    {30, {-0.030542f, -0.042891f, 0.014194f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 11, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -23515}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 12, 4},
    {32, {0.214488f, 0.000000f, 0.000000f}, {0, 0, 6249}, {8, 0, 0, 0}, 1, 3, 0, 0, 0, 13, 4},
    {33, {0.162051f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 14, 4},
    {34, {-0.000001f, 0.000023f, 0.000029f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 15, 0},
    {22, {-0.007013f, 0.062591f, -0.064837f}, {0, 0, 20096}, {11, 0, 0, 0}, 1, 0, 0, 0, 1, 0, 4},
    {23, {-0.030540f, -0.042890f, -0.014195f}, {0, 0, 16384}, {12, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -23515}, {13, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.214487f, 0.000000f, 0.000000f}, {0, 0, 6249}, {14, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {26, {0.162053f, 0.000000f, 0.000000f}, {0, 0, 0}, {15, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {-0.000001f, 0.000023f, 0.000030f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 0},
    {43, {0.132162f, 0.024960f, -0.000005f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, -0.000008f, 0.000000f}, {0, 16384, 0}, {18, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 1161}, {19, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.219885f, 0.000000f, 0.000000f}, {0, 0, 0}, {20, 30, 36, 0}, 3, 4, 0, 0, 0, 0, 4},
    {36, {0.000001f, 0.000004f, 0.000000f}, {0, 0, 1571}, {21, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 4},
    {37, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 3554}, {22, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {38, {0.187014f, 0.000000f, 0.000000f}, {0, 0, 0}, {23, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {4, {0.000034f, 0.000017f, 0.000000f}, {12989, -16383, 28539}, {24, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 5843, 16384}, {25, 26, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.134850f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {-0.000002f, 0.000001f, 0.000002f}, {0, 0, 0}, {27, 28, 29, 0}, 3, 1, 0, 0, 0, 0, 4},
    {44, {-0.025411f, 0.040001f, 0.030897f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 1, 28, 4},
    {45, {-0.025411f, -0.039999f, 0.030897f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 1, 0, 4},
    {46, {0.102355f, 0.000000f, 0.040659f}, {0, -169, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.046690f, -0.086762f, -0.035959f}, {0, 0, 15034}, {31, 0, 0, 0}, 1, 0, 0, 0, 1, 36, 3},
    {8, {-0.055711f, -0.004987f, -0.015162f}, {0, 0, -15403}, {32, 0, 0, 0}, 1, 2, 2, 0, 0, 37, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16455}, {33, 0, 0, 0}, 1, 3, 1, 0, 0, 38, 4},
    {10, {0.172375f, 0.000000f, 0.000000f}, {0, 0, -1366}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 39, 4},
    {11, {0.102750f, 0.000000f, 0.000000f}, {0, 0, 0}, {35, 0, 0, 0}, 1, 4, 0, 0, 0, 40, 4},
    {12, {-0.000004f, -0.000001f, -0.000003f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 41, 3},
    {13, {-0.046691f, -0.086763f, 0.035959f}, {0, 0, 15034}, {37, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {-0.055709f, -0.004986f, 0.015162f}, {0, 0, -15403}, {38, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16453}, {39, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.172373f, 0.000000f, 0.000000f}, {0, 0, -1370}, {40, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {17, {0.102754f, 0.000000f, 0.000000f}, {0, 0, 0}, {41, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {-0.000004f, 0.000000f, -0.000003f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnDogRigNodes[] = {
    {0, {0.000000f, 0.400000f, 0.000000f}, {0, 0, 0}, {1, 19, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -32655}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.161813f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 10, 16, 0}, 3, 4, 0, 0, 0, 0, 4},
    {29, {-0.018098f, 0.024900f, 0.058999f}, {0, 0, 13588}, {5, 0, 0, 0}, 1, 1, 0, 0, 1, 10, 4},
    {30, {0.154580f, -0.000339f, 0.000039f}, {0, 0, 22717}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 11, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -24950}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 12, 4},
    {32, {0.113611f, 0.000000f, 0.000000f}, {0, 0, 7815}, {8, 0, 0, 0}, 1, 3, 0, 0, 0, 13, 4},
    {33, {0.095434f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 14, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 15, 0},
    {22, {-0.018098f, 0.024900f, -0.059001f}, {0, 0, 13588}, {11, 0, 0, 0}, 1, 1, 0, 0, 1, 0, 4},
    {23, {0.154580f, -0.000337f, -0.000036f}, {0, 0, 22717}, {12, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -24950}, {13, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.113611f, 0.000000f, 0.000000f}, {0, 0, 7815}, {14, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {26, {0.095434f, 0.000000f, 0.000000f}, {0, 0, 0}, {15, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 0},
    {40, {0.066502f, -0.017601f, -0.000001f}, {-32768, -32768, 32632}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 4},
    {41, {0.021000f, -0.032000f, 0.000000f}, {0, 0, 0}, {18, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 4},
    {43, {-0.061600f, -0.049000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {20, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -304}, {21, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.164453f, 0.000000f, 0.000000f}, {0, 0, 0}, {22, 31, 37, 0}, 3, 4, 0, 0, 0, 0, 4},
    {36, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 304}, {23, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 4},
    {37, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 8456}, {24, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {38, {0.154789f, 0.000000f, 0.000000f}, {0, 0, 0}, {25, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {4, {0.000000f, 0.000000f, 0.000000f}, {12989, -16383, 28539}, {26, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 4485, 16384}, {27, 28, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.134844f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {29, 30, 0, 0}, 2, 1, 0, 0, 0, 0, 4},
    {45, {0.041429f, -0.026899f, 0.110110f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 1, 30, 4},
    {44, {0.041429f, 0.026901f, 0.110110f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.006927f, -0.054585f, -0.064313f}, {0, 0, 13768}, {32, 0, 0, 0}, 1, 0, 0, 0, 1, 37, 3},
    {8, {0.014427f, -0.002293f, 0.002792f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 38, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -12754}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 39, 4},
    {10, {0.181763f, 0.000000f, 0.000000f}, {0, 0, 1448}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 40, 4},
    {11, {0.139953f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 41, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 42, 3},
    {13, {-0.006928f, -0.054584f, 0.064315f}, {0, 0, 13768}, {38, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {0.014429f, -0.002293f, -0.002794f}, {0, 0, -16384}, {39, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -12754}, {40, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.181762f, 0.000000f, 0.000000f}, {0, 0, 1448}, {41, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.139954f, 0.000000f, 0.000000f}, {0, 0, 0}, {42, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnPnwRigNodes[] = {
    {0, {0.000000f, 1.048932f, 0.000000f}, {0, 0, 0}, {1, 16, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.210507f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 10, 0, 0}, 2, 4, 0, 0, 0, 0, 4},
    {29, {-0.063235f, 0.000000f, 0.045000f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 0, 0, 0, 1, 10, 4},
    {30, {0.036766f, 0.000000f, 0.056428f}, {0, 0, 16384}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 11, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16384}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 12, 4},
    {32, {0.391389f, 0.000000f, 0.000000f}, {0, 0, 0}, {8, 0, 0, 0}, 1, 3, 1, 0, 0, 13, 4},
    {33, {0.380132f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 14, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 15, 0},
    {22, {-0.063235f, 0.000000f, -0.045000f}, {0, 0, 0}, {11, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 4},
    {23, {0.036766f, 0.000000f, -0.056428f}, {0, 0, 16383}, {12, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16331}, {13, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {25, {0.391389f, 0.000000f, 0.000000f}, {0, 0, -106}, {14, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {26, {0.380132f, 0.000000f, 0.000000f}, {0, 0, 0}, {15, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 0},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {17, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {18, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.350495f, 0.000000f, 0.000000f}, {0, 0, 0}, {19, 23, 29, 0}, 3, 4, 0, 0, 0, 0, 4},
    {4, {0.000000f, -0.000001f, 0.000000f}, {-32768, -16383, 0}, {20, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 16384}, {21, 22, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.210085f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {13, {-0.082514f, -0.000001f, 0.065000f}, {0, 0, 0}, {24, 0, 0, 0}, 1, 0, 0, 0, 1, 29, 3},
    {14, {0.022570f, -0.000103f, 0.110000f}, {0, 0, -16384}, {25, 0, 0, 0}, 1, 2, 2, 0, 0, 30, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16443}, {26, 0, 0, 0}, 1, 3, 1, 0, 0, 31, 4},
    {16, {0.274265f, 0.000000f, 0.000000f}, {0, 0, 135}, {27, 0, 0, 0}, 1, 3, 0, 0, 0, 32, 4},
    {17, {0.210981f, 0.000000f, 0.000000f}, {0, 0, 0}, {28, 0, 0, 0}, 1, 4, 0, 0, 0, 33, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 34, 3},
    {7, {-0.082514f, -0.000001f, -0.065000f}, {0, 0, 14}, {30, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {8, {0.022570f, -0.000134f, -0.110000f}, {0, 0, -16384}, {31, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -16325}, {32, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {10, {0.274265f, 0.000000f, 0.000000f}, {0, 0, 136}, {33, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {11, {0.210981f, 0.000000f, 0.000000f}, {0, 0, 0}, {34, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigNode kMotnKittenRigNodes[] = {
    {0, {0.000000f, 0.119710f, 0.000000f}, {0, 0, 0}, {1, 19, 0, 0}, 2, 6, 0, 0, 0, 0, 0},
    {19, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {2, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 1},
    {20, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -32767}, {3, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {21, {0.078888f, 0.000000f, 0.000000f}, {0, 0, 0}, {4, 10, 16, 0}, 3, 4, 0, 0, 0, 0, 4},
    {22, {-0.017100f, 0.018720f, -0.026100f}, {0, 0, 0}, {5, 0, 0, 0}, 1, 1, 0, 0, 1, 10, 4},
    {23, {-0.004759f, 0.039025f, -0.000062f}, {0, 0, -29230}, {6, 0, 0, 0}, 1, 2, 2, 0, 0, 11, 3},
    {24, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -27086}, {7, 0, 0, 0}, 1, 3, 1, 0, 0, 12, 4},
    {25, {0.035788f, 0.000000f, 0.000000f}, {0, 0, 12366}, {8, 0, 0, 0}, 1, 3, 0, 0, 0, 13, 4},
    {26, {0.030062f, 0.000000f, 0.000000f}, {0, 0, 0}, {9, 0, 0, 0}, 1, 4, 0, 1, 0, 14, 4},
    {27, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 15, 0},
    {29, {-0.017100f, 0.018720f, 0.026100f}, {0, 0, 0}, {11, 0, 0, 0}, 1, 1, 0, 0, 1, 0, 4},
    {30, {-0.004759f, 0.039024f, 0.000063f}, {0, 0, -29230}, {12, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {31, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -27086}, {13, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {32, {0.035788f, 0.000000f, 0.000000f}, {0, 0, 12366}, {14, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {33, {0.030062f, 0.000000f, 0.000000f}, {0, 0, 0}, {15, 0, 0, 0}, 1, 4, 0, 1, 0, 0, 4},
    {34, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 0},
    {40, {0.023202f, 0.002661f, -0.000001f}, {16383, -16383, 16384}, {17, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 4},
    {41, {0.000000f, 0.000000f, 0.027311f}, {0, 0, 0}, {18, 0, 0, 0}, 1, 1, 0, 0, 0, 0, 4},
    {43, {0.000000f, 0.000000f, 0.033813f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {1, {0.000000f, 0.000000f, 0.000000f}, {0, 16384, 0}, {20, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 1},
    {2, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {21, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {3, {0.083222f, 0.000000f, 0.000000f}, {0, 0, 0}, {22, 31, 37, 0}, 3, 4, 0, 0, 0, 0, 4},
    {36, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 304}, {23, 0, 0, 0}, 1, 2, 1, 0, 1, 0, 4},
    {37, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 8456}, {24, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {38, {0.018782f, 0.000000f, 0.000000f}, {0, 0, 0}, {25, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {4, {0.000000f, 0.000000f, 0.000000f}, {12989, -16383, 28539}, {26, 0, 0, 0}, 1, 2, 1, 0, 0, 0, 2},
    {5, {0.000000f, 0.000000f, 0.000000f}, {0, 4485, 16384}, {27, 28, 0, 0}, 2, 3, 0, 0, 0, 0, 4},
    {6, {0.052563f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 4, 0, 0, 1, 0, 4},
    {47, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {29, 30, 0, 0}, 2, 1, 0, 0, 0, 0, 4},
    {44, {0.009677f, 0.029700f, 0.035006f}, {20205, -5156, -18262}, {0, 0, 0, 0}, 0, 1, 0, 0, 1, 30, 4},
    {45, {0.009677f, -0.029700f, 0.035006f}, {20205, 5156, -14505}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 4},
    {7, {-0.021943f, -0.013566f, -0.016200f}, {0, 0, 13768}, {32, 0, 0, 0}, 1, 0, 0, 0, 1, 37, 3},
    {8, {-0.022737f, 0.000680f, -0.010858f}, {0, 0, -16384}, {33, 0, 0, 0}, 1, 2, 2, 0, 0, 38, 3},
    {9, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -18102}, {34, 0, 0, 0}, 1, 3, 1, 0, 0, 39, 4},
    {10, {0.038196f, 0.000000f, 0.000000f}, {0, 0, 8964}, {35, 0, 0, 0}, 1, 3, 0, 0, 0, 40, 4},
    {11, {0.051508f, 0.000000f, 0.000000f}, {0, 0, 0}, {36, 0, 0, 0}, 1, 4, 0, 0, 0, 41, 4},
    {12, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 42, 3},
    {13, {-0.021943f, -0.013566f, 0.016200f}, {0, 0, 13768}, {38, 0, 0, 0}, 1, 0, 0, 0, 0, 0, 3},
    {14, {-0.022738f, 0.000679f, 0.010859f}, {0, 0, -16384}, {39, 0, 0, 0}, 1, 2, 2, 0, 0, 0, 3},
    {15, {0.000000f, 0.000000f, 0.000000f}, {0, 0, -18102}, {40, 0, 0, 0}, 1, 3, 1, 0, 0, 0, 4},
    {16, {0.038196f, 0.000000f, 0.000000f}, {0, 0, 8964}, {41, 0, 0, 0}, 1, 3, 0, 0, 0, 0, 4},
    {17, {0.051508f, 0.000000f, 0.000000f}, {0, 0, 0}, {42, 0, 0, 0}, 1, 4, 0, 0, 0, 0, 4},
    {18, {0.000000f, 0.000000f, 0.000000f}, {0, 0, 0}, {0, 0, 0, 0}, 0, 1, 0, 0, 0, 0, 3},
};

static constexpr MotionRigType kMotionRigTypes[] = {
    {kMotnAkiraRigNodes, std::size(kMotnAkiraRigNodes), "Akira", 1.150400f},
    {kMotnTogBigRigNodes, std::size(kMotnTogBigRigNodes), "TogBig", 1.319535f},
    {kMotnChaRigNodes, std::size(kMotnChaRigNodes), "Cha", 1.120971f},
    {kMotnDebRigNodes, std::size(kMotnDebRigNodes), "Deb", 1.181201f},
    {kMotnGakKkoRigNodes, std::size(kMotnGakKkoRigNodes), "GakKko", 0.908311f},
    {kMotnGenRigNodes, std::size(kMotnGenRigNodes), "Gen", 1.289173f},
    {kMotnOtherHumanStandardRigNodes, std::size(kMotnOtherHumanStandardRigNodes), "OtherHumanStandard", 1.180000f},
    {kMotnJijRigNodes, std::size(kMotnJijRigNodes), "Jij", 1.024343f},
    {kMotnKodRigNodes, std::size(kMotnKodRigNodes), "Kod", 0.722532f},
    {kMotnOtherHumanScript09RigNodes, std::size(kMotnOtherHumanScript09RigNodes), "OtherHumanScript09", 1.199471f},
    {kMotnOtherHumanRigNodes, std::size(kMotnOtherHumanRigNodes), "OtherHuman", 1.180000f},
    {kMotnOtherHumanScript11RigNodes, std::size(kMotnOtherHumanScript11RigNodes), "OtherHumanScript11", 1.145400f},
    {kMotnSypRigNodes, std::size(kMotnSypRigNodes), "Syp", 1.107851f},
    {kMotnAkiraVariantRigNodes, std::size(kMotnAkiraVariantRigNodes), "AkiraVariant", 1.150400f},
    {kMotnSypScriptRigNodes, std::size(kMotnSypScriptRigNodes), "SypScript", 1.155605f},
    {kMotnSinRigNodes, std::size(kMotnSinRigNodes), "Sin", 1.052724f},
    {kMotnCatRigNodes, std::size(kMotnCatRigNodes), "Cat", 0.288388f},
    {kMotnYagRigNodes, std::size(kMotnYagRigNodes), "Yag", 0.444074f},
    {kMotnDogRigNodes, std::size(kMotnDogRigNodes), "Dog", 0.400000f},
    {kMotnPnwRigNodes, std::size(kMotnPnwRigNodes), "Pnw", 1.048932f},
    {kMotnKittenRigNodes, std::size(kMotnKittenRigNodes), "Kitten", 0.119710f},
};

} // namespace motion_rigs


inline constexpr std::array<std::uint16_t, 21> kBasePoseAnimationIds{
    594,
    602,
    762,
    595,
    596,
    597,
    600,
    598,
    599,
    600,
    600,
    600,
    601,
    594,
    601,
    656,
    814,
    815,
    832,
    1000,
    1405,
};

inline constexpr std::array<std::uint8_t, 32> kMarkerRecordSizes{
    12,
    4,
    8,
    16,
    20,
    12,
    8,
    4,
    0,
    16,
    8,
    12,
    4,
    4,
    16,
    8,
    2,
    8,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    8,
    8,
    4,
    0,
    0,
    0,
};



inline constexpr std::array<std::uint8_t, 56> kFrameEventRecordSizes{
    0,
    4,
    4,
    16,
    8,
    8,
    8,
    8,
    4,
    4,
    16,
    8,
    8,
    4,
    8,
    4,
    4,
    4,
    8,
    8,
    8,
    4,
    8,
    8,
    8,
    16,
    12,
    28,
    4,
    8,
    8,
    4,
    8,
    8,
    8,
    8,
    8,
    12,
    8,
    12,
    8,
    8,
    4,
    8,
    8,
    4,
    4,
    4,
    4,
    16,
    0,
    0,
    0,
    0,
    0,
    0,
};

} // namespace shenmue::s1
