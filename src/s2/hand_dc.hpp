#pragma once

#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/debug_screen_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_rig.hpp"
#endif

#define SHENMUE_HAND_LAYOUT_NAME_(line) SHENMUE_HANDLayoutCheck##line
#define SHENMUE_HAND_LAYOUT_NAME(line) SHENMUE_HAND_LAYOUT_NAME_(line)
#define SHENMUE_HAND_LAYOUT(condition) typedef char SHENMUE_HAND_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#define SHENMUE_HAND_CONSTANT static const
#define SHENMUE_HAND_INITIALIZER =
namespace shenmue {
namespace s2 {

struct MotionWork;
struct MotionOverride {
    std::uint32_t flags;
    void (*evaluate)(std::int32_t side, std::int32_t interpolationMode);
};
SHENMUE_HAND_LAYOUT(sizeof(MotionOverride) == 8);
SHENMUE_HAND_LAYOUT(offsetof(MotionOverride, evaluate) == 4);
extern MotionOverride g_searchHandMotionOverride;
extern Character *g_searchHandCharacter;
extern MotionWork *g_searchHandMotionWork;
extern std::int32_t g_searchHandOverrideEnabled, g_searchHandChestOverrideEnabled;
void ResetSearchHandState();
void mtwkResetBlends();

#define CHARACTER_LEFT_HAND (::shenmue::shen_name(SHEN_NAME4('H', 'N', 'D', 'L')))
#define CHARACTER_RIGHT_HAND (::shenmue::shen_name(SHEN_NAME4('H', 'N', 'D', 'R')))
#define HAND_MATRIX_MEMORY (::shenmue::shen_name(SHEN_NAME4('H', 'N', 'D', 'P')))
#define HAND_CALCULATION_MEMORY (::shenmue::shen_name(SHEN_NAME4('H', 'N', 'D', 'C')))
#define HAND_BANK_MEMORY (::shenmue::shen_name(SHEN_NAME4('H', 'N', 'D', 'M')))
#define HAND_MODEL_MEMORY (::shenmue::shen_name(SHEN_NAME4('H', 'A', 'N', 'D')))
#define HAND_SECONDARY_MODEL_MEMORY (::shenmue::shen_name(SHEN_NAME4('H', 'N', 'D', 'T')))
#define HAND_MESH_MEMORY (::shenmue::shen_name(SHEN_NAME4('H', 'N', 'D', 'V')))
#define HAND_EXTENDED_POSE_MEMORY (::shenmue::shen_name(SHEN_NAME4('H', 'N', 'D', 'X')))
#define HAND_MODEL_CHUNK (::shenmue::shen_name(SHEN_NAME4('M', 'D', 'H', '7')))
#define HAND_MODEL_EXTENSION (::shenmue::shen_name(SHEN_NAME4('C', 'H', 'R', 'M')))
struct HandMeshHeader {
    std::uint32_t flags, retainedState[5];
};
SHENMUE_HAND_LAYOUT(sizeof(HandMeshHeader) == 0x18);
struct HandGripProbeDefinition {
    std::int16_t firstJoint, secondJoint;
    float firstWeight, radius;
};
struct HandGripProbe {
    Vector3 position;
    float radius;
};
SHENMUE_HAND_LAYOUT(sizeof(HandGripProbeDefinition) == 12);
SHENMUE_HAND_LAYOUT(sizeof(HandGripProbe) == 16);
SHENMUE_HAND_CONSTANT HandGripProbeDefinition handGripProbes[10] SHENMUE_HAND_INITIALIZER{{69, 70, 0.29660001397132874f, 0.00570000009611249f},
    {69, 70, 1.0f, 0.010599999688565731f}, {59, 60, 0.25529998540878296f, 0.00930000003427267f}, {59, 60, 1.0f, 0.01140000019222498f},
    {45, 46, 0.2854999899864197f, 0.009100000374019146f}, {45, 46, 1.0f, 0.01209999993443489f}, {37, 38, 0.0f, 0.009700000286102295f},
    {37, 38, 0.800000011920929f, 0.010599999688565731f}, {29, 30, 0.30730000138282776f, 0.00989999994635582f},
    {29, 30, 0.8456000089645386f, 0.010999999940395355f}};
SHENMUE_HAND_CONSTANT std::int16_t handGripJointGroups[72] SHENMUE_HAND_INITIALIZER{-1, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, 4,
    4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1};
struct HandCubicKeyframe {
    std::int32_t frame;
    float incoming, outgoing, degrees;
};
struct HandPoseChannel {
    std::int32_t keyCount;
    const HandCubicKeyframe *keys;
};
struct HandPoseSequenceHeader {
    std::byte retainedHeader[32];
    std::int32_t frameCount;
    std::int16_t jointCount;
    std::uint16_t retainedJointState;
};
struct HandPoseBank {
    const HandPoseSequenceHeader **poses;
    std::int16_t first, last;
    HandPoseBank *next, *previous;
    void *package;
};
struct HandPoseModelJoint {
    std::int32_t childCount, id;
    Vector3 position;
    Vector3i rotation;
    Matrix4x4 inverseBind;
};
struct HandSkinVertex {
    Vector3 position, normal, deformedPosition, deformedNormal;
};
struct HandSkinVertexHeader {
    std::uint32_t retainedState;
    std::int32_t vertexCount;
};
struct HandSkinGroupHeader {
    std::uint32_t retainedState[2];
    std::int32_t groupCount;
};
struct HandSkinInfluenceGroup {
    std::int32_t joint, count;
};
struct HandSkinInfluence {
    std::int32_t vertex;
    float weight;
};
struct HandSkinNormalHeader {
    std::uint32_t retainedState;
    std::int32_t normalCount, markedCount;
};
struct HandSkinRemapHeader {
    std::uint32_t retainedState;
    std::int32_t count, firstVertex;
};
struct HandSkinRemap {
    std::int32_t destinationByteOffset, nextVertex;
};
SHENMUE_HAND_LAYOUT(sizeof(HandSkinVertex) == 0x30);
SHENMUE_HAND_LAYOUT(offsetof(HandSkinVertex, deformedPosition) == 0x18);
SHENMUE_HAND_LAYOUT(offsetof(HandSkinVertex, deformedNormal) == 0x24);
SHENMUE_HAND_LAYOUT(sizeof(HandSkinVertexHeader) == 8);
SHENMUE_HAND_LAYOUT(sizeof(HandSkinGroupHeader) == 12);
SHENMUE_HAND_LAYOUT(sizeof(HandSkinInfluenceGroup) == 8);
SHENMUE_HAND_LAYOUT(sizeof(HandSkinInfluence) == 8);
SHENMUE_HAND_LAYOUT(sizeof(HandSkinNormalHeader) == 12);
SHENMUE_HAND_LAYOUT(sizeof(HandSkinRemapHeader) == 12);
SHENMUE_HAND_LAYOUT(sizeof(HandSkinRemap) == 8);
struct HandPoseState {
    const std::byte *modelData;
    std::int32_t frameCount;
    const HandPoseChannel *channels;
    std::int16_t jointCount, meshWordCount;
    const std::uint32_t *meshBackup;
    std::uint32_t *meshCopy;
    std::int8_t selector;
    std::uint8_t poseMode;
    std::int16_t poseTicks;
    Vector3 jointPositions[72];
    std::int16_t targetJointAngles[72][3], currentJointAngles[72][3], wristAngles[3];
    std::byte retainedWristState[2];
    void *extendedPoseWork;
    Character *secondaryPoseWork;
    float bounds[16];
    float frame, step, alpha;
    std::int8_t region, previousRegion;
    std::int16_t currentFrame, previousFrame, firstFrame, lastFrame;
    std::uint8_t flags, secondaryEnabled;
    std::byte retainedState[8];
};
struct CharacterHandComponent {
    char filename[16];
    MT7Node *primaryNode;
    void *primaryFile;
    MT7Model *primaryModel;
    MT7Node *secondaryNode;
    void *secondaryFile;
    MT7Model *secondaryModel;
    void (*previousCore)(Character *);
    const HandPoseSequenceHeader *sequence;
    HandPoseState pose;
};
SHENMUE_HAND_LAYOUT(sizeof(HandCubicKeyframe) == 16);
SHENMUE_HAND_LAYOUT(sizeof(HandPoseChannel) == 8);
SHENMUE_HAND_LAYOUT(sizeof(HandPoseSequenceHeader) == 0x28);
SHENMUE_HAND_LAYOUT(sizeof(HandPoseBank) == 0x14);
SHENMUE_HAND_LAYOUT(offsetof(HandPoseBank, next) == 8);
SHENMUE_HAND_LAYOUT(sizeof(HandPoseModelJoint) == 0x60);
SHENMUE_HAND_LAYOUT(offsetof(HandPoseModelJoint, rotation) == 0x14);
SHENMUE_HAND_LAYOUT(sizeof(HandPoseState) == 0x74c);
SHENMUE_HAND_LAYOUT(offsetof(HandPoseState, targetJointAngles) == 0x37c);
SHENMUE_HAND_LAYOUT(offsetof(HandPoseState, currentJointAngles) == 0x52c);
SHENMUE_HAND_LAYOUT(sizeof(CharacterHandComponent) == 0x77c);
SHENMUE_HAND_LAYOUT(offsetof(CharacterHandComponent, primaryNode) == 0x10);
SHENMUE_HAND_LAYOUT(offsetof(CharacterHandComponent, pose) == 0x30);
SHENMUE_HAND_LAYOUT(offsetof(CharacterHandComponent, pose) + offsetof(HandPoseState, extendedPoseWork) == 0x714);
SHENMUE_HAND_LAYOUT(offsetof(CharacterHandComponent, pose) + offsetof(HandPoseState, bounds) == 0x71c);
SHENMUE_HAND_LAYOUT(offsetof(CharacterHandComponent, pose) + offsetof(HandPoseState, frame) == 0x75c);
SHENMUE_HAND_LAYOUT(offsetof(CharacterHandComponent, pose) + offsetof(HandPoseState, region) == 0x768);
SHENMUE_HAND_LAYOUT(offsetof(CharacterHandComponent, pose) + offsetof(HandPoseState, flags) == 0x772);
enum HandPoseFlag { HandPoseLoop = 1, HandPoseReverse = 2, HandPosePaused = 4, HandPoseInactive = 8 };
SHENMUE_HAND_CONSTANT std::int32_t handDefaultJointAngles[19][3] SHENMUE_HAND_INITIALIZER{{0, 727, 63534}, {0, 0, 65354}, {0, 62624, 3822}, {0, 0, 60258},
    {0, 0, 61168}, {0, 0, 64080}, {0, 546, 1820}, {0, 0, 58802}, {0, 0, 54252}, {0, 0, 59894}, {0, 546, 1456}, {0, 0, 56436}, {0, 0, 56072}, {0, 0, 59166},
    {0, 546, 909}, {0, 0, 52978}, {0, 0, 56800}, {0, 0, 59348}, {0, 546, 1092}};
extern HandPoseBank handPoseBanks;
extern Matrix4x4 *handPoseMatrices;
extern void *handCalculationWork, *handProbeWork;
extern CharacterHandComponent *g_temporaryHNDL, *g_temporaryHNDR;
extern std::int32_t gripDisabled;
void InitializeHandPoseNullsub();
void InitializeHandWork();
void *LoadHandPoseBank(std::int32_t act, shen_name area, const char *filename);
std::int32_t ReleaseHandPoseBank(void *data);
extern const HandPoseModelJoint *g_handPoseJointCursor;
extern const HandPoseChannel *g_handPoseChannelCursor;
extern std::int16_t (*g_handPoseOutputAngles)[3];
extern std::int32_t g_handPoseEvaluationFrame;
extern std::int32_t g_handPoseMode, g_handPosePositiveXY;
extern Vector3 *g_handPoseJointPositions;
extern const std::int16_t *g_handPoseWristAngles;
extern Matrix4x4 *g_handPoseMatrixOutput;
void BuildHandJointTransform(float positionX, float positionY, float positionZ, std::int32_t x, std::int32_t y, std::int32_t z) asm(
    "_shenmue_dc_hand_joint_transform");
void PublishHandJointMatrix(std::int32_t joint);
void StageHandJoint(std::int32_t publishMatrices);
void BuildHandPoseMatrices(std::int32_t frame, HandPoseState *state, Matrix4x4 *matrices, std::int32_t publishMatrices);
void HMT_HandCalcSkin(HandPoseState *state, Matrix4x4 *matrices);
const HandSkinRemapHeader *NormalizeHandSkinVertices(HandSkinVertex *vertices, const HandSkinNormalHeader *header) asm("_shenmue_dc_hand_skin_normals");
CharacterHandComponent *AllocateHNDL(Character *character);
CharacterHandComponent *AllocateHNDR(Character *character);
void ResetCharacterHandComponent(CharacterHandComponent &hand);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct HandIKConfig {
    std::uint8_t flags, mode, pendingMode, totalTicks, ticksRemaining, blendTotalTicks, blendTicksRemaining, commandDelay;
    std::uint8_t jointTable, rotationAxis;
    std::uint16_t reserved0a, rotationAngle, reserved0e;
    std::byte *beginPoseCommand, *endPoseCommand;
    float pathLength, pathBias;
    float jointAngles[8];
    Vector3 localTargetOffset, currentTarget, currentAxisX, currentAxisZ, destination, destinationAxisX, destinationAxisZ;
    MotionRigAxis path[3];
    Vector3 rotationOrigin;
    Matrix4x4 rotationTransform;
};
SHENMUE_HAND_LAYOUT(sizeof(HandIKConfig) == 0x1a0);
SHENMUE_HAND_LAYOUT(offsetof(HandIKConfig, beginPoseCommand) == 0x10);
SHENMUE_HAND_LAYOUT(offsetof(HandIKConfig, jointAngles) == 0x20);
SHENMUE_HAND_LAYOUT(offsetof(HandIKConfig, localTargetOffset) == 0x40);
SHENMUE_HAND_LAYOUT(offsetof(HandIKConfig, path) == 0x94);
SHENMUE_HAND_LAYOUT(offsetof(HandIKConfig, rotationTransform) == 0x160);
static const float searchHandJointTables[4][2][8] = {{{0, -1.0471975803375244f, -0.6981317400932312f, 0, 1.7453293800354004f, 0, 0, 0},
                                                         {0, -1.0471975803375244f, 0.6981317400932312f, 0, 1.7453293800354004f, 0, 0, 0}},
    {{0, -0.7853981852531433f, -0.3490658700466156f, 0, 1.5707963705062866f, 0.7853981852531433f, 0, 0},
        {0, -0.7853981852531433f, 0.3490658700466156f, 0, 1.5707963705062866f, -0.7853981852531433f, 0, 0}},
    {{0, -0.8726646900177002f, -0.6981317400932312f, 0.7853981852531433f, 1.7453293800354004f, -0.5235987901687622f, 0, 0},
        {0, -0.8726646900177002f, 0.6981317400932312f, -0.7853981852531433f, 1.7453293800354004f, 0.5235987901687622f, 0, 0}},
    {{-0.3490658700466156f, -0.3490658700466156f, -0.6981317400932312f, -1.3962634801864624f, 1.7453293800354004f, 0, 0, 0},
        {0.3490658700466156f, -0.3490658700466156f, 0.6981317400932312f, 1.3962634801864624f, 1.7453293800354004f, 0, 0, 0}}};
static const float searchHandJointLimits[2][8][2] = {
    {{-.8726646900177002f, .8726646900177002f}, {-1.7453293800354004f, 1.7453293800354004f}, {-1.2217305898666382f, .3490658700466156f},
        {-1.5707963705062866f, 1.5707963705062866f}, {0, 2.967059850692749f}, {-2.094395160675049f, 2.094395160675049f},
        {-1.3962634801864624f, 1.3962634801864624f}, {-1.5707963705062866f, 1.5707963705062866f}},
    {{-.8726646900177002f, .8726646900177002f}, {-1.7453293800354004f, 1.7453293800354004f}, {-.3490658700466156f, 1.2217305898666382f},
        {-1.5707963705062866f, 1.5707963705062866f}, {0, 2.967059850692749f}, {-2.094395160675049f, 2.094395160675049f},
        {-1.3962634801864624f, 1.3962634801864624f}, {-1.5707963705062866f, 1.5707963705062866f}}};
extern HandIKConfig g_searchHandConfigurations[2];
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
enum HandIKMode { HandIK_Disabled, HandIK_PathFollow, HandIK_HoldTransform, HandIK_DeltaVector, HandIK_Unblended, HandIK_HandPose, HandIK_Unavailable };
enum HandIKFlags { HandIKEnabled = 1, HandIKPathActive = 2, HandIKBlendToPath = 4, HandIKFastCommandTiming = 8, HandIKRotatingTarget = 16 };
HandIKMode getSearchHandConfigModeFor(std::uint8_t side);
bool BeginSearchHand(Character *character, std::int32_t handSelection, std::uint32_t jointTable);
void SetSearchHandChestTransform(const Vector3 *position, const Vector3i *rotation);
void srchArmEvaluate(MotionArmRig &arm, std::uint32_t interpolationMode, const float *angles);
void MotionCommonCallback(std::int32_t mode, std::int32_t interpolationMode);
bool mtwkApplyHand(std::uint8_t mode, std::uint8_t side);
bool srchSetTargetsWithParams(std::int32_t side, const Matrix4x4 *transform, const Vector3 *tangentOrigin, const Vector3 *pathOrigin, std::uint8_t ticks);
bool srchSetSearchJointAngles(std::uint8_t side, std::byte *beginCommand, std::byte *endCommand, std::int32_t fastTiming);
bool SetSearchHandRotatingTarget(
    std::uint8_t side, const Matrix4x4 *transform, const Matrix4x4 *rotationTransform, std::uint8_t axis, std::int32_t angle, std::uint8_t ticks);
std::uint8_t srchSetJointTable(std::uint8_t table, std::uint8_t side);
bool srchSetXformFor(const Vector3 *targetOffset, std::uint8_t side);
bool get_x_from_target(Vector3 *targetOffset, std::uint8_t side);
bool srchSetNewTicksTimeFor(std::int32_t ticks, std::uint8_t side);
std::int32_t srchGetTicksDifference(std::uint8_t side);
std::int32_t srchGetCurrentTotalTicks(std::uint8_t side);
bool SearchIK_SetTarget(const Matrix4x4 *transform);
bool SetSearchHandCurrentTransform(const Matrix4x4 *transform, std::uint8_t side);
const Matrix4x4 *mtwkGetSearchArmTransform(std::uint8_t side);
void SetSearchHandOverrideEnabled(std::uint8_t enabled);
bool IsSearchHandOverrideEnabled();
extern bool g_searchHandPairedTarget;
extern Matrix4x4 g_searchHandPairTransform;
#endif
void GetSearchHandWristAngles(MotionWork &work, std::int16_t (&angles)[4]);
extern std::uint8_t g_searchHandSolveStatus, g_searchHandPreviousSolveStatus;
void mtwkAssignCallbacks(MotionWork *work);
bool mtwkIsSearchHandActive(const MotionWork *work);
bool mtwkClearSearchHandRequests(MotionWork *work);
std::int32_t mtwkGetCustomOverrideFlags(const MotionWork *work);
CharacterHandComponent *GetCharacterHand(Character *character, std::int32_t side);
void ApplyCharacterHandRotation(Character *character, std::int32_t side, std::uint32_t flags, const Vector3i *rotation);
bool IsHandPoseInactive(const CharacterHandComponent &hand);
void SetHandPoseFlags(CharacterHandComponent &hand, std::int16_t enabled, std::uint16_t flags);
std::int32_t GetHandPoseAnimationResult(const CharacterHandComponent &hand);
void SetHandPoseStep(CharacterHandComponent &hand, float step);
void SetHandSecondaryPoseWork(CharacterHandComponent &hand, Character *work);
void SetHighModelXYZ(Character *character, std::int32_t region, std::int32_t side, std::int32_t pose);
void SetHandRegionXYZWithFlags(Character *character, std::int32_t region, std::int32_t side, std::int32_t pose, std::int8_t alphaPolicy);
std::uint32_t RetireHandModels(Character *character, CharacterHandComponent &hand);
std::uint32_t hndlSetFlags(Character *character);
SHENMUE_HAND_CONSTANT std::int16_t handCommandJointIds[19] SHENMUE_HAND_INITIALIZER{19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
SHENMUE_HAND_CONSTANT std::int16_t handEndJointIds[20] SHENMUE_HAND_INITIALIZER{69, 67, 65, 59, 57, 55, 53, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23, -1};
SHENMUE_HAND_CONSTANT std::uint32_t handPoseCommandRangeMask = 0xe000, handPoseCommandSignBit = 0x8000;
void ParseHandPoseCommand(CharacterHandComponent &hand, std::byte *command, std::int16_t ticks);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void WriteHandPoseCommand(const CharacterHandComponent *hand, std::byte *command);
void SolveHandGripPose(CharacterHandComponent *hand, const Matrix4x4 *base, MT7Node *geometry, const Matrix4x4 *transform, std::int32_t flags,
    std::byte *beginCommand, std::byte *endCommand);
void GetSearchHandJointRotation(std::uint8_t side, Vector3i *rotation);
#endif
void ApplyCharacterHandPoseCommand(Character *character, std::int32_t side, std::byte *command, std::int32_t ticks);
void GetHandEndpointPosition(const CharacterHandComponent &hand, std::int32_t endpoint, Vector3 &position);
void GetHandPrimaryNodeRotation(const CharacterHandComponent &hand, Vector3i &rotation);
#endif
std::int32_t InterpolateHandPoseChannel(std::int32_t count, const HandCubicKeyframe *keys, std::int32_t frame);
void ParseHandPoseJoint();
void ParseHandPoseFrame(std::int32_t frame, HandPoseState *state, Matrix4x4 *matrices);
void ConfigureHandPoseSequence(CharacterHandComponent &hand, std::int16_t pose, std::int32_t first, std::int32_t last, std::int16_t start, std::uint16_t flags,
    float step, std::int16_t ticks);
void SetCharacterHandRegionEnabled(CharacterHandComponent *hand, std::int8_t region, std::int16_t pose, std::int8_t alphaPolicy);
void SetHandPrimaryRotation(CharacterHandComponent *hand, std::int32_t x, std::int32_t y, std::int32_t z);
void mhndSetPrimaryRotation(CharacterHandComponent *hand, std::int32_t x, std::int32_t y, std::int32_t z);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void mhndGetPrimaryRotation(const CharacterHandComponent *hand, Vector3i *rotation);
#endif
MT7Model *QueryHandModelCacheNullsub(const char *filename);
std::uint32_t GetNumHandModelChunks(const HandMeshHeader *mesh);
std::uint32_t AllocateHandModels(Character *character, CharacterHandComponent *hand);
std::uint32_t SetLowQualityHands(Character *character);
std::uint32_t GetLoadedHandModelFlags(Character *character);
std::int32_t IsCharacterHandRegionEnabled(const CharacterHandComponent *hand);
void CleanupHand(Character *character, void *payload);
void CharacterHandDrawClosure(Character *character);
void CharacterHandDirectCore(Character *character);
void UpdateHand(Character *character, void *payload);
void AttachCharacterHandComponents(Character *character, const char *left, const char *right, std::int32_t flags);
std::int32_t GetHandAngleDifference(std::int32_t target, std::int32_t current);
void AdvanceGripAngles(CharacterHandComponent *hand, std::int32_t collide, std::uint32_t flags);
void ComputeHandTransformedProbes(const Vector3 *positions, void *extendedWork);
void StageHandPose(std::int32_t frame, HandPoseState *state, Matrix4x4 *matrices);
void ApplyHandBodyTransform(Character *character, CharacterHandComponent *hand);
void UpdateHandGripBounds(CharacterHandComponent *hand);
void BuildHandGripGeometry(CharacterHandComponent *hand, MT7Node *node);
void UpdateMeshHandGripBounds(CharacterHandComponent *hand);

SHENMUE_HAND_CONSTANT std::int32_t handGripMeshCapacityWords = 0x6000;
extern std::int16_t handGripMeshWords;
void BuildHandGripNodeGeometry(CharacterHandComponent *hand, MT7Node *node);
void TransformHandGripMesh(const HandMeshHeader *mesh, std::uint32_t *destination);
void IntersectHandGripMesh(float *bounds, const HandMeshHeader *mesh, const HandGripProbe *probes);
void IntersectHandGripMeshes(float *bounds);
void UpdateHandGripTriangleBounds(float *bounds, const Vector3 *const *vertices, std::uint32_t winding, const HandGripProbe *probes);

} // namespace s2
} // namespace shenmue
#undef SHENMUE_HAND_LAYOUT_NAME_
#undef SHENMUE_HAND_LAYOUT_NAME
#undef SHENMUE_HAND_LAYOUT
#undef SHENMUE_HAND_CONSTANT
#undef SHENMUE_HAND_INITIALIZER

#endif
