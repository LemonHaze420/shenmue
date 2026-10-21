#pragma once

#include "s2/motion_work.hpp"
#include "s2/character.hpp"
#include "s2/mt7.hpp"

#include <cstddef>
#include <cstdint>
#include <array>
#include <string>
#include <vector>

namespace shenmue::s2 {

struct Character;
struct PresentationState;
struct HandParams;
struct PakGlobalData;
struct HandPoseBank {
    std::int32_t *offsets;
    std::int16_t first;
    std::int16_t last;
    std::uint32_t reserved0c;
    HandPoseBank *next;
    HandPoseBank *previous;
    PakGlobalData *package;
};
static_assert(sizeof(HandPoseBank) == 0x28);
static_assert(offsetof(HandPoseBank, next) == 0x10);
struct HandCubicKeyframe {
    std::int32_t frame;
    float incoming;
    float outgoing;
    float degrees;
};
static_assert(sizeof(HandCubicKeyframe) == 0x10);
// MDH7/MDHX depth-first record; child count controls traversal independently of render-resource vectors.
struct HandPoseModelJoint {
    std::int32_t childCount;
    std::int32_t id;
    Vector3 position;
    Vector3i rotation;
    Matrix4x4 inverseBind;
};
static_assert(sizeof(HandPoseModelJoint) == 0x60);
static_assert(offsetof(HandPoseModelJoint, rotation) == 0x14);
inline constexpr shen_name HAND_MATRIX_MEMORY{SHEN_NAME(HNDP)};
inline constexpr shen_name HAND_CALCULATION_MEMORY{SHEN_NAME(HNDC)};
inline constexpr shen_name HAND_BANK_MEMORY{SHEN_NAME(HNDM)};
inline constexpr std::int32_t handDefaultJointAngles[19][3] = {
    {0, 727, 63535}, {0, 0, 65354}, {0, 62624, 3821}, {0, 0, 60258}, {0, 0, 61168}, {0, 0, 64080},
    {0, 546, 1820}, {0, 0, 58802}, {0, 0, 54253}, {0, 0, 59894}, {0, 546, 1456}, {0, 0, 56436}, {0, 0, 56072},
    {0, 0, 59167}, {0, 546, 908}, {0, 0, 52979}, {0, 0, 56801}, {0, 0, 59349}, {0, 546, 1091}
};


inline std::int32_t neutralHandPoseCommand[19][3]{};
inline std::int32_t openHandPoseCommand[19][3]{
    {1999, 65354, 62444},
    {1638, 180, 63900},
    {546, 64719, 3366},
    {0, 64809, 60257},
    {0, 64809, 59528},
    {0, 0, 62623},
    {0, 0, 64628},
    {0, 0, 58983},
    {0, 0, 58073},
    {0, 65172, 61897},
    {0, 0, 65354},
    {0, 729, 60074},
    {0, 65173, 57163},
    {0, 65172, 63171},
    {0, 0, 0},
    {0, 547, 56252},
    {0, 1639, 60074},
    {0, 64809, 61715},
    {64080, 182, 65535},
};
inline std::int32_t chestHandPoseCommand[19][3]{
    {0, 0, 46606},
    {0, 2182, 64444},
    {0, 0, 1456},
    {0, 64080, 49154},
    {0, 908, 44239},
    {0, 0, 53340},
    {0, 0, 0},
    {0, 1456, 54796},
    {0, 0, 43692},
    {0, 727, 50974},
    {0, 0, 0},
    {0, 2912, 54797},
    {0, 182, 44421},
    {0, 1456, 50245},
    {0, 0, 0},
    {2912, 910, 51336},
    {726, 726, 47517},
    {0, 2003, 50974},
    {65518, 65427, 0},
};
inline std::int32_t greetingHandPoseCommand[19][3]{
    {1999, 1456, 60440},
    {1638, 65354, 64810},
    {546, 61988, 4093},
    {0, 64811, 60257},
    {0, 64081, 59891},
    {0, 726, 64625},
    {0, 0, 64628},
    {0, 0, 61348},
    {0, 0, 59347},
    {0, 65172, 63717},
    {0, 0, 65354},
    {0, 729, 62439},
    {0, 65355, 59165},
    {0, 64810, 64991},
    {0, 0, 0},
    {0, 909, 58617},
    {0, 1456, 60618},
    {0, 63354, 182},
    {64080, 182, 0},
};
inline constexpr Vector3i motionAuxiliaryHandPoses[9][2][4]{
    {{{0, 0, 0}, {0, 0, 3640}, {0, 910, 0}, {0, 64626, 0}}, {{0, 0, 0}, {0, 0, 3640}, {0, 910, 0}, {0, 64626, 0}}},
    {{{0, 0, 0}, {0, 0, 3185}, {1601, 800, 1601}, {1601, 64736, 1601}}, {{0, 0, 0}, {0, 0, 3185}, {63935, 800, 63935}, {63935, 64736, 63935}}},
    {{{0, 0, 0}, {0, 0, 2730}, {3185, 691, 3185}, {3185, 64845, 3185}}, {{0, 0, 0}, {0, 0, 2730}, {62351, 691, 62351}, {62351, 64845, 62351}}},
    {{{0, 0, 0}, {0, 0, 2275}, {4787, 564, 4787}, {4787, 64972, 4787}}, {{0, 0, 0}, {0, 0, 2275}, {60749, 564, 60749}, {60749, 64972, 60749}}},
    {{{0, 0, 0}, {0, 0, 1820}, {6371, 455, 6371}, {6371, 65081, 6371}}, {{0, 0, 0}, {0, 0, 1820}, {59165, 455, 59165}, {59165, 65081, 59165}}},
    {{{0, 0, 0}, {0, 0, 1365}, {7973, 345, 7973}, {7973, 65191, 7973}}, {{0, 0, 0}, {0, 0, 1365}, {57563, 345, 57563}, {57563, 65191, 57563}}},
    {{{0, 0, 0}, {0, 0, 910}, {9557, 236, 9557}, {9557, 65300, 9557}}, {{0, 0, 0}, {0, 0, 910}, {55979, 236, 55979}, {55979, 65300, 55979}}},
    {{{0, 0, 0}, {0, 0, 455}, {11159, 127, 11159}, {11159, 65409, 11159}}, {{0, 0, 0}, {0, 0, 455}, {54377, 127, 54377}, {54377, 65409, 54377}}},
    {{{0, 0, 0}, {0, 0, 0}, {12743, 0, 12743}, {12743, 0, 12743}}, {{0, 0, 0}, {0, 0, 0}, {52793, 0, 52793}, {52793, 0, 52793}}}
};
inline constexpr Vector3i motionArmHandPoses[12][2][4]{
    {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
    {{{0, 0, 0}, {0, 0, 0}, {1820, 0, 1820}, {3640, 0, 3640}}, {{0, 0, 0}, {0, 0, 0}, {63716, 0, 63716}, {61896, 0, 61896}}},
    {{{0, 0, 0}, {0, 9102, 60075}, {5461, 0, 14563}, {7281, 0, 12743}}, {{0, 0, 0}, {0, 56434, 60075}, {60075, 0, 50973}, {58255, 0, 52793}}},
    {{{0, 0, 0}, {0, 5461, 61896}, {2730, 0, 14563}, {9102, 0, 10922}}, {{0, 0, 0}, {0, 60075, 61896}, {62806, 0, 50973}, {56434, 0, 54614}}},
    {{{0, 0, 0}, {0, 0, 63716}, {1820, 0, 9102}, {7281, 63716, 9102}}, {{0, 0, 0}, {0, 0, 63716}, {63716, 0, 56434}, {58255, 63716, 56434}}},
    {{{0, 0, 0}, {54614, 12743, 52793}, {3640, 0, 12743}, {5461, 0, 9102}},
     {{0, 0, 0}, {10922, 52793, 52793}, {61896, 0, 52793}, {60075, 0, 56434}}},
    {{{0, 0, 0}, {10922, 0, 5461}, {63716, 910, 64626}, {63716, 64626, 64626}}, {{0, 0, 0}, {54614, 0, 5461}, {1820, 910, 910}, {1820, 64626, 910}}},
    {{{0, 0, 0}, {0, 3640, 61896}, {0, 0, 0}, {12743, 0, 16384}}, {{0, 0, 0}, {0, 61896, 61896}, {0, 0, 0}, {52793, 0, 49152}}},
    {{{0, 0, 0}, {0, 0, 63716}, {12743, 0, 14563}, {16384, 0, 16384}}, {{0, 0, 0}, {0, 0, 63716}, {52793, 0, 50973}, {49152, 0, 49152}}},
    {{{0, 0, 0}, {0, 0, 9102}, {16384, 0, 16384}, {16384, 0, 16384}}, {{0, 0, 0}, {0, 0, 9102}, {49152, 0, 49152}, {49152, 0, 49152}}},
    {{{0, 0, 0}, {54614, 12743, 52793}, {1820, 0, 9102}, {1820, 0, 10922}},
     {{0, 0, 0}, {10922, 52793, 52793}, {63716, 0, 56434}, {63716, 0, 54614}}},
    {{{0, 0, 0}, {49152, 3640, 60075}, {3640, 63716, 14563}, {0, 0, 7281}}, {{0, 0, 0}, {16384, 61896, 60075}, {61896, 63716, 50973}, {0, 0, 58255}}}
};

struct HandPoseJoint {
    std::int32_t parent;
    std::int32_t relativeParent;
    std::uint32_t id;
    Vector3 position;
    Vector3i rotation;
    Matrix4x4 inverseBind;
    MT7Node *renderNode{};
    const Matrix4x4 *previousRenderTransform{};
};

struct HandPoseVertex {
    Vector3 position;
    Vector3 normal;
    Vector3 deformedPosition;
    Vector3 deformedNormal;
};

struct HandPoseInfluence {
    std::uint32_t joint;
    std::uint32_t vertex;
    float weight;
};

struct HandModelResource {
    HandParams *hand{};
    HandModelResource *next{};
    MT7Model *reader{};
    LegacyDrawResources meshResources;
    LegacyNodeCommandOverride meshCommand{};
    std::vector<HandPoseJoint> joints;
    std::vector<HandPoseVertex> poseVertices;
    std::vector<HandPoseInfluence> influences;
    std::vector<MT7VertexMapping> mappings;
    std::array<Matrix4x4, 72> jointMatrices;
    std::array<Matrix4x4, 72> skinMatrices;
    std::array<Matrix4x4, 72> relativeMatrices;
    // Legacy transformOverride consumes absolute model-local matrices, not HNDP_1.
    std::array<Matrix4x4, 20> renderMatrices;
    std::size_t poseNode{};
    std::uint32_t normalizedNormalCount{};
    std::uint32_t markedVertexCount{};
    bool poseDirty{};
    PresentationState *state{};
};
#pragma pack(push, 4)
struct HandParams {
    char filename[16];
    MT7Node *primaryNode;
    std::byte *primaryFile;
    MT7Model *primaryReader;
    MT7Node *secondaryNode;
    std::byte *secondaryFile;
    MT7Model *secondaryReader;
    void (*previousCore)(Character *);
    const std::byte *poseData;
    const std::byte *poseModelData;
    std::int32_t poseFrameCount;
    std::uint32_t reserved05c;
    const std::byte *poseFrames;
    std::int16_t poseJointCount;
    std::int16_t meshWordCount;
    std::uint32_t reserved06c;
    const std::uint32_t *meshBackup;
    std::uint32_t *meshCopy;
    std::uint8_t selector;
    std::uint8_t poseMode;
    std::int16_t poseTicks;
    Vector3 jointPositions[72];
    std::int16_t targetJointAngles[72][3];
    std::int16_t currentJointAngles[72][3];
    std::int16_t wristAngles[3];
    std::byte reserved74a[6];
    void *extendedPoseWork;
    void *secondaryPoseWork;
    Character *owner;
    float boundsWork[16];
    float poseFrame;
    float poseStep;
    float modelAlpha;
    std::int8_t regionState;
    std::int8_t previousRegionState;
    std::int16_t currentPoseFrame;
    std::int16_t previousPoseFrame;
    std::int16_t firstPoseFrame;
    std::int16_t lastPoseFrame;
    std::uint8_t poseFlags;
    std::uint8_t secondaryEnabled; // +0x7bf, secondary-model allocation policy
    std::byte reserved7c0[6];
    std::uint8_t immediatePoseResult;  // +0x7c6, low byte of attachment publication result
    std::uint8_t poseRefreshRequested; // +0x7c7, independent deferred refresh request
    std::uint8_t externalDrawControl;
    std::byte reserved7c9[3];
    std::int32_t calculationEnabled;
    float retainedUnitValues[6];
};
#pragma pack(pop)
static_assert(sizeof(HandParams) == 0x7e8);
static_assert(offsetof(HandParams, primaryNode) == 0x10);
static_assert(offsetof(HandParams, primaryFile) == 0x18);
static_assert(offsetof(HandParams, primaryReader) == 0x20);
static_assert(offsetof(HandParams, secondaryNode) == 0x28);
static_assert(offsetof(HandParams, previousCore) == 0x40);
static_assert(offsetof(HandParams, meshWordCount) == 0x6a);
static_assert(offsetof(HandParams, meshBackup) == 0x70);
static_assert(offsetof(HandParams, meshCopy) == 0x78);
static_assert(offsetof(HandParams, selector) == 0x80);
static_assert(offsetof(HandParams, owner) == 0x760);
static_assert(offsetof(HandParams, poseFrame) == 0x7a8);
static_assert(offsetof(HandParams, externalDrawControl) == 0x7c8);
static_assert(offsetof(HandParams, poseMode) == 0x81);
static_assert(offsetof(HandParams, targetJointAngles) == 0x3e4);
static_assert(offsetof(HandParams, poseTicks) == 0x082);
static_assert(offsetof(HandParams, currentJointAngles) == 0x594);
static_assert(offsetof(HandParams, modelAlpha) == 0x7b0);
static_assert(offsetof(HandParams, wristAngles) == 0x744);
static_assert(offsetof(HandParams, regionState) == 0x7b4);
static_assert(offsetof(HandParams, previousRegionState) == 0x7b5);
static_assert(offsetof(HandParams, currentPoseFrame) == 0x7b6);
static_assert(offsetof(HandParams, previousPoseFrame) == 0x7b8);
static_assert(offsetof(HandParams, firstPoseFrame) == 0x7ba);
static_assert(offsetof(HandParams, lastPoseFrame) == 0x7bc);
static_assert(offsetof(HandParams, poseFlags) == 0x7be);
static_assert(offsetof(HandParams, secondaryEnabled) == 0x7bf);
static_assert(offsetof(HandParams, immediatePoseResult) == 0x7c6);
static_assert(offsetof(HandParams, poseRefreshRequested) == 0x7c7);
static_assert(offsetof(HandParams, calculationEnabled) == 0x7cc);
static_assert(offsetof(HandParams, retainedUnitValues) == 0x7d0);

enum HandPoseFlags : std::uint8_t {
    HandPoseLoop = 1,
    HandPoseReverse = 2,
    HandPosePaused = 4,
    HandPoseInactive = 8,
};
inline constexpr std::uint64_t HAND_MISSING_RESULT = 0xffffffffULL;
inline constexpr std::uint64_t HAND_RESET_BOUNDS_RESULT = 0x7f7fffff7f7fffffULL;
inline constexpr std::int16_t handEndBoneIds[20] = {69, 67, 65, 59, 57, 55, 53, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 23, 0};
inline constexpr std::int16_t handGripJointGroups[72]{
    -1, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, 4,
    4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, -1,
    -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1};
struct HandGripProbe {
    Vector3 position;
    float radius;
};
static_assert(sizeof(HandGripProbe) == 0x10);
extern void *handCalculationWork;
extern void *handProbeWork;
extern std::int32_t gripDisabled;
void AdvanceGripAngles(HandParams &hand, std::int32_t collide, std::int32_t flags);
void UpdateHandGripBounds(HandParams &hand);
void StageHandGripProbes(HandParams &hand);
inline constexpr std::uint32_t HAND_POSE_COMMAND_RANGE_MASK = 0xe000U;
inline constexpr std::uint32_t HAND_POSE_COMMAND_SIGN_BIT = 0x8000U;
inline constexpr std::uint32_t HAND_POSE_REPLACEMENT_OWNER_HASH = 0x949db44bU;
inline constexpr std::int32_t handPoseReplacementMatch[3] = {1999, 65354, 62444};
inline std::int32_t handPoseReplacementCommand[19][3] = {
    {0, 0, 46606}, {0, 2182, 64444}, {0, 0, 1456}, {0, 64080, 49154}, {0, 908, 44239}, {0, 0, 53340}, {0, 0, 0},
    {0, 1456, 54796}, {0, 0, 43692}, {0, 727, 50974}, {0, 0, 0}, {0, 2912, 54797}, {0, 182, 44421}, {0, 1456, 50245},
    {0, 0, 0}, {2912, 910, 51336}, {726, 726, 47517}, {0, 2003, 50974}, {65518, 65427, 0}
};

inline std::int32_t commonAuthoredHandPose[60]{
    1999,65354,59167,1638,543,64810,546,63262,4821,0,64810,56252,0,64081,58800,
    0,727,363,0,0,64628,0,0,57709,0,0,57344,0,65172,65354,0,0,65354,0,729,60074,
    0,65355,57526,0,64809,182,0,0,0,0,910,55705,0,1456,59164,0,63354,1092,64080,182,0,0,0,0
};

enum class HandIKMode : std::uint8_t {
    Disabled = 0,
    PathFollow = 1,
    HoldTransform = 2,
    DeltaVector = 3,
    Unblended = 4,
    HandPose = 5,
    Unavailable = 6,
};

enum HandIKFlags : std::uint8_t {
    HandIKEnabled = 1,
    HandIKPathActive = 2,
    HandIKBlendToPath = 4,
    HandIKFastCommandTiming = 8,
    HandIKRotatingTarget = 16,
};

#pragma pack(push, 4)


struct HandIKConfig {
    std::uint8_t flags;
    HandIKMode mode;
    HandIKMode pendingMode;
    std::uint8_t totalTicks;
    std::uint8_t ticksRemaining;
    std::uint8_t blendTotalTicks;
    std::uint8_t blendTicksRemaining;
    std::uint8_t commandDelay;
    std::uint8_t jointTable;
    std::uint8_t rotationAxis;
    std::uint16_t reserved0a;
    std::uint16_t rotationAngle;
    std::uint16_t reserved0e;
    std::byte *beginPoseCommand; // +0x010, borrowed writable command streams
    std::byte *endPoseCommand;
    float pathLength;
    float pathBias;
    float jointAngles[8];      // +0x028, radians
    Vector3 localTargetOffset;
    Vector3 currentTarget;
    Vector3 currentAxisX;
    Vector3 currentAxisZ;
    Vector3 destination;
    Vector3 destinationAxisX;
    Vector3 destinationAxisZ;
    std::uint32_t reserved09c;
    MotionRigAxis path[3];
    Vector3 rotationOrigin;
    Matrix4x4 rotationTransform;
    std::uint32_t reserved1dc;
};
#pragma pack(pop)

static_assert(sizeof(HandIKConfig) == 0x1e0);
static_assert(offsetof(HandIKConfig, beginPoseCommand) == 0x10);
static_assert(offsetof(HandIKConfig, jointAngles) == 0x28);
static_assert(offsetof(HandIKConfig, localTargetOffset) == 0x48);
static_assert(offsetof(HandIKConfig, path) == 0xa0);
static_assert(offsetof(HandIKConfig, rotationTransform) == 0x19c);

float SearchIK_GetMaxBlend();
void setSearchIKFlag(float blend);
HandIKMode getSearchHandConfigModeFor(std::uint8_t side);
void SearchIK_SetTarget(const Matrix4x4 *transform);
bool BeginSearchHand(Character *character, std::int32_t handSelection, std::uint32_t jointTable);
bool SetSearchHandOverrideEnabled(bool enabled);
bool mtwkApplyHand(HandIKMode mode, std::uint8_t side);
bool srchSetTargetsWithParams(std::int32_t side, const Matrix4x4 *transform, const Vector3 *tangentOrigin, const Vector3 *pathOrigin, std::uint8_t ticks);
bool get_x_from_target(Vector3 *targetOffset, std::uint8_t side);
bool srchSetXformFor(const Vector3 *targetOffset, std::uint8_t side);
bool srchSetSearchJointAngles(std::uint8_t side, std::byte *beginCommand, std::byte *endCommand, std::int32_t fastTiming);
bool SetSearchHandRotatingTarget(
    std::uint8_t side, const Matrix4x4 *transform, const Matrix4x4 *rotationTransform, std::uint8_t axis, std::uint16_t angle, std::uint8_t ticks);
std::uint8_t srchSetJointTable(std::uint8_t table, std::uint8_t side);
bool srchSetNewTicksTimeFor(std::uint8_t ticks, std::uint8_t side);
std::int32_t srchGetTicksDifference(std::uint8_t side);
std::int32_t srchGetCurrentTotalTicks(std::uint8_t side);
Matrix4x4 *mtwkGetSearchArmTransform(std::uint8_t side);
void GetSearchHandJointRotation(std::uint8_t side, Vector3i *rotation);
bool SetSearchHandCurrentTransform(const Matrix4x4 *transform, std::uint8_t side);
void SetSearchHandChestTransform(const Vector3 *position, const Vector3i *rotation);
void mtwkResetBlends();
void ResetSearchHandState();



extern MotionOverride g_searchHandMotionOverride;
void MotionCommonCallback(std::int64_t mode, std::int32_t interpolationMode);
void AssignMotionCallbacks(MotionWork *work);
bool EvaluateSearchHandArm(MotionWork &work, std::uint8_t side, std::uint32_t interpolationMode);
std::uint32_t GetCustomMotionOverrideFlags(const MotionWork *work);
bool ClearSearchHandMotionRequests(MotionWork *work);
void GetSearchHandWristAngles(MotionWork &work, std::int16_t (&angles)[4]);
inline std::int16_t g_characterHandWristAngles[4]{};

// HNDL/HNDR model state: zero selects right, every nonzero int32 selects left.
HandParams *GetCharacterHand(Character *character, std::int32_t side);
HandModelResource *GetHandModelResource(const HandParams &hand);
void LoadHandModels();
void InitializeHandWork();
void *LoadHandPoseBank(std::int32_t act, shen_name area, const char *filename);
std::uint64_t ReleaseHandPoseBank(void *data);
std::int16_t ConfigureHandPoseSequence(
    HandParams &hand, std::int16_t pose, std::int16_t first, std::int16_t last, std::int16_t start, std::uint8_t flags, float step, std::int16_t ticks);
bool IsHandPoseInactive(const HandParams &hand);
std::uint8_t SetHandPoseFlags(HandParams &hand, std::int16_t enabled, std::uint8_t flags);
std::uint64_t GetHandPoseAnimationResult(const HandParams &hand);
void SetHandPoseStep(HandParams &hand, float step);
// Borrowed pointer: a change invalidates all sixteen cached bounds, without allocating or freeing work.
void SetHandSecondaryPoseWork(HandParams &hand, void *work);

void GetHandEndpointPosition(const HandParams &hand, std::int32_t endpoint, Vector3 &position);
void GetHandPrimaryNodeRotation(const HandParams &hand, Vector3i &rotation);
MT7Node *SetHandPrimaryNodeRotation(HandParams &hand, const Vector3i &rotation);
std::uint64_t SetLowQualityHands(Character *character);
// FTBL336 always snaps alpha; FTBL807 retains the caller's signed alpha policy.
MT7Node *SetHighModelXYZ(Character *character, std::int8_t state, std::int32_t side, std::int16_t pose);
MT7Node *SetHandRegionXYZWithFlags(Character *character, std::int8_t state, std::int32_t side, std::int16_t pose, std::int8_t alphaPolicy);
std::uint8_t hndlSetFlags(Character *character);
bool SetCharacterHandRegionEnabled(Character *character, std::uint8_t side, bool enabled, std::int8_t alphaPolicy);
// Valid region and nineteen writable int32 triples are required. Commands may be
// sanitized in place; no command/resource ownership is transferred.
std::uint64_t ParseHandPoseCommand(HandParams *hand, std::byte *command, std::int16_t ticks);
std::byte *WriteHandPoseCommand(const HandParams &hand, std::byte *command);
void SolveHandGripPose(HandParams *hand, const Matrix4x4 *base, const MT7Node *geometry, const Matrix4x4 *unused, std::int32_t flags, std::byte *beginCommand,
    std::byte *endCommand);
// FTBL335 returns zero for a missing region, otherwise the parser's raw result.
std::uint64_t ApplyCharacterHandPoseCommand(Character *character, std::int32_t side, std::byte *command, std::int16_t ticks);
bool ApplyScenarioLeftHandPose(Character *character);
std::int8_t AdvanceHandDrawState(HandParams &hand);
void StageHandPose(HandParams &hand, std::int32_t frame = 0);

} // namespace shenmue::s2
