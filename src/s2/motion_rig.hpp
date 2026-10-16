#pragma once

#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {





struct MotionTimePair {
    float splitTimeFrames;
    float ticksTime;
    float maxTime;
};

// The clock that g_motionRegionClocks publishes for each body region: the current
// time pair, its playback speed and the authored split/end/duration triplet.
struct MotionRigClock {
    MotionTimePair current;
    float playbackSpeed;
    MotionTimePair authored;
};

struct MotionRigKey {
    float timeSeconds;
    float tangentIn;
    float tangentOut;
    float value;
};

struct MotionRigChannel {
    MotionRigKey current;
    MotionRigKey next;
    float inverseDuration;
    std::uint32_t keysRemaining;
    std::uint32_t keyCount;      // +0x28, authored tuples (explicit keys plus two)
    std::uint32_t reserved2c;
    const std::uint16_t *frameCursor;
    const std::uint16_t *keyBlock;    // +0x38, first tuple, reread when the stream is not reinitialized
    float scale;
    float bias;
};

struct MotionRigAxis {
    MotionRigChannel channel;
    float dimensions[2];
};


struct MotionCallback {
    void (*evaluate)(void *parameters);
    void *parameters;
};



struct MotionRotationBasis {
    Vector3 targetAxis;
    Vector3 targetTangent;
    Vector3 sourceAxis;
    Vector3 sourceTangent;
};

// FKChainOverrideNode: three rotation channels, their blend basis and override.
struct MotionRigRotation {
    MotionRigAxis axes[3];
    MotionRotationBasis basis;
    MotionCallback callback;
};

// TwoJointFkNode: the override rig's two-channel toe joints.
struct MotionRigTwoJoint {
    MotionRigAxis axes[2];
    MotionRotationBasis basis;
    MotionCallback callback;
};

// TargetPvaOverrideNode and every in-rig target group: three position channels whose
// last tail and the following word hold the solved target, then its override callback
// and the position/velocity/acceleration triplet the next transition blends from.
struct MotionRigTarget {
    MotionRigAxis axes[3]; // axes[2].dimensions are output X/Y
    float outputZ;
    float outputW;
    MotionCallback callback;
    Vector4 pva[3];          // +0x108, x unused, y/z half tangents, w value
};

struct MotionBaseRig {
    Matrix4x4 rootPrepassTransform;
    Matrix4x4 pelvisTransform;
    std::uint32_t poseMode;
    std::uint32_t motionFlags;
    MotionRigClock clock;
    float reserveda4;
    MotionRigAxis pelvisPosition[3];
    MotionRigAxis pelvisRotation[3];       // +0x198, Z/Y/X
    Matrix4x4 pelvisWorld;
    MotionRotationBasis pelvisBasis;
    MotionCallback pelvisPositionCallback;
    MotionCallback pelvisRotationCallback;
    Vector4 pelvisPva[3];
    MotionRigRotation hips;                // +0x348; axes[1] tail is the hip height
    MotionRigRotation *overrideHips;
    MotionRigTarget *overridePelvisTarget;
    Matrix4x4 hipsWorld;
    MotionRigRotation *leftHipNode;
    Matrix4x4 *leftHipWorld;
    MotionRigRotation leftThigh;
    MotionRigTarget leftFoot;              // +0x608; axes[0]/axes[1] tails are shin/thigh lengths
    Matrix4x4 leftThighWorld;
    Matrix4x4 leftShinWorld;
    MotionRigAxis leftAnkleRotation[3];
    Matrix4x4 leftAnkleWorld;
    MotionRotationBasis leftAnkleBasis;
    MotionCallback leftAnkleCallback;
    MotionRigAxis leftFootRotation[3];     // +0x930; axes[1]/axes[2] tails are the foot offset
    Matrix4x4 leftFootWorld;
    std::byte reserveda60[0x40];
    MotionRigRotation *rightHipNode;
    Matrix4x4 *rightHipWorld;
    MotionRigRotation rightThigh;
    MotionRigTarget rightFoot;
    Matrix4x4 rightThighWorld;
    Matrix4x4 rightShinWorld;
    MotionRigAxis rightAnkleRotation[3];
    Matrix4x4 rightAnkleWorld;
    MotionRotationBasis rightAnkleBasis;
    MotionCallback rightAnkleCallback;
    MotionRigAxis rightFootRotation[3];  // +0xf08; axes[1]/axes[2] tails are the foot offset
    Matrix4x4 rightFootWorld;
    std::byte reserved1038[0x40];
    MotionRigTwoJoint *rightToe1Node;
    Matrix4x4 *rightToe1World;
    MotionRigTwoJoint *rightToe2Node;
    Matrix4x4 *rightToe2World;
    MotionRigTwoJoint *rightToe3Node;
    Matrix4x4 *rightToe3World;
};

struct MotionChestRig {
    std::uint32_t poseMode;
    MotionRigClock clock;
    MotionRigRotation rotation;
    MotionRigTarget target;
    Matrix4x4 chestWorld;
    MotionRigRotation upperRotation;
    Matrix4x4 upperChestWorld;
};

struct MotionHeadRig {
    std::uint32_t poseMode;
    MotionRigClock clock;
    Vector3 lookTarget;
    std::uint32_t reserved2c;
    MotionRigRotation baseRotation;
    Matrix4x4 neckWorld;
    MotionRigRotation midRotation;
    MotionRigTarget target;
    Matrix4x4 headWorld;
    Vector4 eyeOffset;
    MotionRigRotation *aimSourceNode;
    MotionRigTarget *aimTargetNode;
    Matrix4x4 *aimSourceWorld;
    MotionRigRotation *nodeA;
    Matrix4x4 *nodeAWorld;
    MotionRigRotation *nodeB;
    Matrix4x4 *nodeBWorld;
    MotionRigRotation *nodeC;
    Matrix4x4 *nodeCWorld;
};

// Region 3 drives the arm at IKRig +0x1988 (named rightArm here) and region 4 the arm
// at +0x1ff0 with ArmSideL set.
struct MotionArmRig {
    std::uint32_t poseMode;
    MotionRigClock clock;
    MotionRigRotation shoulder;
    Matrix4x4 shoulderWorld;
    MotionRigRotation elbow;
    MotionRigTarget target;           // +0x2c0; axes[0]/axes[1] tails are forearm/upper-arm lengths
    Matrix4x4 upperArmWorld;
    Matrix4x4 forearmWorld;
    MotionRotationBasis forearmBasis;
    MotionCallback forearmCallback;
    Matrix4x4 wristWorld;
    MotionRigAxis wristRotation[3];   // +0x4f8, forearm twist X, wrist Z, wrist Y
    Matrix4x4 handWorld;
    MotionRotationBasis wristBasis;
    MotionCallback wristCallback;
};

enum MotionRegion : std::uint32_t { LowerBody, Chest, Head, RightArm, LeftArm };

struct IKRig {
    std::uint32_t boneId;
    std::uint32_t useOverride;
    MotionBaseRig lowerBody;
    MotionChestRig chest;
    MotionHeadRig head;
    MotionArmRig rightArm;
    MotionArmRig leftArm;
};



struct MotionStreamContext {
    float version;
    std::uint16_t flags;
    std::uint16_t frameCount;
    struct {
        const void *keyCounts;
        const std::uint16_t *keyData;
    } regions[5];
};


struct MotionOverride {
    std::uint32_t flags;
    std::uint32_t reserved04;
    void (*evaluate)(std::int64_t side, std::int32_t interpolationMode);
};

enum MotionPoseMode : std::uint32_t {
    motionPoseFromChannels = 0,
    motionPoseFromAxisAngle = 1,
};

enum MotionInterpolationMode : std::uint32_t {
    motionInterpolationHermiteAuto = 0,
    motionInterpolationLinear = 1,
    motionInterpolationHermite = 2,
};

inline constexpr std::uint32_t motionInitStream = 0x0001;
inline constexpr std::uint32_t motionInitMirror = 0x0002;
inline constexpr std::uint32_t motionInitLeftArm = 0x0004;
inline constexpr std::uint32_t motionInitChannels = 0x0008;
inline constexpr std::uint32_t motionInitSwapLeadFoot = 0x0010;
inline constexpr std::uint32_t motionInitSplitWindow = 0x0020;
inline constexpr std::uint32_t motionInitCallbacks = 0x0040;
inline constexpr std::uint32_t motionInitTranslation = 0x0080;
inline constexpr std::uint32_t motionInitGraphFrames = 0xff00;
inline constexpr std::uint32_t motionInitBaking = 0x10000;
inline constexpr std::uint16_t motionWideKeyCounts = 0x0100;

static_assert(sizeof(MotionTimePair) == 0x0c);
static_assert(sizeof(MotionRigClock) == 0x1c);
static_assert(sizeof(MotionRigKey) == 0x10);
static_assert(sizeof(MotionRigChannel) == 0x48);
static_assert(offsetof(MotionRigChannel, keyCount) == 0x28);
static_assert(offsetof(MotionRigChannel, frameCursor) == 0x30);
static_assert(offsetof(MotionRigChannel, keyBlock) == 0x38);
static_assert(offsetof(MotionRigChannel, scale) == 0x40);
static_assert(sizeof(MotionRigAxis) == 0x50);
static_assert(sizeof(MotionCallback) == 0x10);
static_assert(sizeof(MotionRotationBasis) == 0x30);
static_assert(sizeof(MotionRigRotation) == 0x130);
static_assert(sizeof(MotionRigTwoJoint) == 0xe0);
static_assert(sizeof(MotionRigTarget) == 0x138);
static_assert(offsetof(MotionRigTarget, outputZ) == 0xf0);
static_assert(offsetof(MotionRigTarget, callback) == 0xf8);
static_assert(offsetof(MotionRigTarget, pva) == 0x108);
static_assert(offsetof(MotionBaseRig, poseMode) == 0x080);
static_assert(offsetof(MotionBaseRig, clock) == 0x088);
static_assert(offsetof(MotionBaseRig, pelvisPosition) == 0x0a8);
static_assert(offsetof(MotionBaseRig, pelvisRotation) == 0x198);
static_assert(offsetof(MotionBaseRig, pelvisWorld) == 0x288);
static_assert(offsetof(MotionBaseRig, pelvisBasis) == 0x2c8);
static_assert(offsetof(MotionBaseRig, pelvisPositionCallback) == 0x2f8);
static_assert(offsetof(MotionBaseRig, pelvisPva) == 0x318);
static_assert(offsetof(MotionBaseRig, hips) == 0x348);
static_assert(offsetof(MotionBaseRig, overrideHips) == 0x478);
static_assert(offsetof(MotionBaseRig, hipsWorld) == 0x488);
static_assert(offsetof(MotionBaseRig, leftHipNode) == 0x4c8);
static_assert(offsetof(MotionBaseRig, leftThigh) == 0x4d8);
static_assert(offsetof(MotionBaseRig, leftFoot) == 0x608);
static_assert(offsetof(MotionBaseRig, leftThighWorld) == 0x740);
static_assert(offsetof(MotionBaseRig, leftAnkleRotation) == 0x7c0);
static_assert(offsetof(MotionBaseRig, leftAnkleWorld) == 0x8b0);
static_assert(offsetof(MotionBaseRig, leftAnkleBasis) == 0x8f0);
static_assert(offsetof(MotionBaseRig, leftAnkleCallback) == 0x920);
static_assert(offsetof(MotionBaseRig, leftFootRotation) == 0x930);
static_assert(offsetof(MotionBaseRig, leftFootWorld) == 0xa20);
static_assert(offsetof(MotionBaseRig, rightHipNode) == 0xaa0);
static_assert(offsetof(MotionBaseRig, rightThigh) == 0xab0);
static_assert(offsetof(MotionBaseRig, rightFoot) == 0xbe0);
static_assert(offsetof(MotionBaseRig, rightThighWorld) == 0xd18);
static_assert(offsetof(MotionBaseRig, rightAnkleRotation) == 0xd98);
static_assert(offsetof(MotionBaseRig, rightAnkleWorld) == 0xe88);
static_assert(offsetof(MotionBaseRig, rightAnkleBasis) == 0xec8);
static_assert(offsetof(MotionBaseRig, rightFootRotation) == 0xf08);
static_assert(offsetof(MotionBaseRig, rightFootWorld) == 0xff8);
static_assert(offsetof(MotionBaseRig, rightToe1Node) == 0x1078);
static_assert(sizeof(MotionBaseRig) == 0x10a8);
static_assert(offsetof(MotionChestRig, rotation) == 0x020);
static_assert(offsetof(MotionChestRig, target) == 0x150);
static_assert(offsetof(MotionChestRig, chestWorld) == 0x288);
static_assert(offsetof(MotionChestRig, upperRotation) == 0x2c8);
static_assert(offsetof(MotionChestRig, upperChestWorld) == 0x3f8);
static_assert(sizeof(MotionChestRig) == 0x438);
static_assert(offsetof(MotionHeadRig, baseRotation) == 0x030);
static_assert(offsetof(MotionHeadRig, neckWorld) == 0x160);
static_assert(offsetof(MotionHeadRig, midRotation) == 0x1a0);
static_assert(offsetof(MotionHeadRig, target) == 0x2d0);
static_assert(offsetof(MotionHeadRig, headWorld) == 0x408);
static_assert(offsetof(MotionHeadRig, eyeOffset) == 0x448);
static_assert(offsetof(MotionHeadRig, aimSourceNode) == 0x458);
static_assert(offsetof(MotionHeadRig, nodeCWorld) == 0x498);
static_assert(sizeof(MotionHeadRig) == 0x4a0);
static_assert(offsetof(MotionArmRig, shoulder) == 0x020);
static_assert(offsetof(MotionArmRig, shoulderWorld) == 0x150);
static_assert(offsetof(MotionArmRig, elbow) == 0x190);
static_assert(offsetof(MotionArmRig, target) == 0x2c0);
static_assert(offsetof(MotionArmRig, upperArmWorld) == 0x3f8);
static_assert(offsetof(MotionArmRig, forearmWorld) == 0x438);
static_assert(offsetof(MotionArmRig, forearmBasis) == 0x478);
static_assert(offsetof(MotionArmRig, wristWorld) == 0x4b8);
static_assert(offsetof(MotionArmRig, wristRotation) == 0x4f8);
static_assert(offsetof(MotionArmRig, handWorld) == 0x5e8);
static_assert(offsetof(MotionArmRig, wristBasis) == 0x628);
static_assert(offsetof(MotionArmRig, wristCallback) == 0x658);
static_assert(sizeof(MotionArmRig) == 0x668);
static_assert(offsetof(IKRig, lowerBody) == 0x0008);
static_assert(offsetof(IKRig, chest) == 0x10b0);
static_assert(offsetof(IKRig, head) == 0x14e8);
static_assert(offsetof(IKRig, rightArm) == 0x1988);
static_assert(offsetof(IKRig, leftArm) == 0x1ff0);
static_assert(sizeof(IKRig) == 0x2658);
static_assert(sizeof(MotionStreamContext) == 0x58);
static_assert(sizeof(MotionOverride) == 0x10);

// nlmotion process globals.
extern float g_motionCurrentAnimTime;
extern float g_motionMaximumAnimTime;
extern float g_motionPreviousAnimTime;
extern std::uint32_t g_motionInterpolationType;
extern std::uint32_t g_motionInitFlags;
extern std::uint16_t g_motionAnimFlags;
extern const std::uint8_t *g_motionKeyCounts;
extern const std::uint16_t *g_motionWideKeyCounts;
extern const std::uint16_t *g_motionKeyData;
extern std::int8_t g_motionGraphFrames;
extern std::int32_t g_motionUseOverride;
extern std::uint32_t g_motionRigBoneId;
extern std::uint32_t g_motionPoseMode;
extern float g_motionOverrideBlendTime;
extern float g_motionGaitSplitFraction;
extern Matrix4x4 g_motionOverrideRootTransform;
extern Matrix4x4 g_motionOverrideBakeTransform;
extern MotionOverride *g_customMotionOverride;
extern const float (*g_motionRigDimensions)[6];

// Channel streams.
void ReadMotionKeyBlock(MotionRigChannel &channel);
void SkipMotionKeyBlock();
void SetMotionStreamFlags(std::int32_t channels, std::uint32_t initFlags, std::uint16_t animFlags);
float EvaluateMotionRigChannelValue(const MotionRigChannel &channel);
float SampleMotionRigChannelAdvance(MotionRigChannel &channel);
void SampleMotionRigChannelPVA(MotionRigChannel &channel, float *value, float *tangentOut, float *tangentIn);
void UpdatePointersForCurrentFrame(MotionStreamContext *context, const std::uint16_t *header, void **armTrack, void **regionTrack);
void UpdateAnimRate(MotionRigChannel &channel, float scale, float bias);
float mtwkGetScaleFactorFor(std::uint32_t targetRig, std::uint32_t sourceRig, std::uint32_t region);
void SetOverrideBlendTime(float time);
void SetCustomMotionOverride(MotionOverride *override);
void SetGaitSplitFraction(float fraction);

// Basis and target helpers.
float AimBasisAtTarget(const Vector3 &target);
float AlignFrameToTarget_Vector(const Vector3 &target);
void AxisAngleToBasisSlerp(MotionRotationBasis &basis);
void SwizzledBasis3x4IntoActiveFrom(const Vector4 (&rows)[3]);
void ChannelsToPoseColumns(MotionRigAxis *axes, MotionRotationBasis &basis, MotionCallback &callback, std::int32_t joint);
void CaptureChannelsToSourcePose(MotionRigAxis *axes, MotionRotationBasis &basis, MotionCallback &callback, std::int32_t joint);
void ChannelsToWorld_PVA(MotionRigAxis *axes, const Vector4 (&pva)[3], MotionCallback &callback);
void EvaluateObjectMotionAndTransform(MotionRigAxis *axes, Vector4 (&pva)[3], MotionCallback &callback);

// Region readers, blend sources and solvers. The Evaluate* blends rescale a region's channels toward the
// rest pose for an override blend and return 0 when that blend is already applied.
std::int32_t EvaluateBase(MotionBaseRig &base, std::uint8_t sourceRig, float scale, float blend);
std::int32_t EvaluateChestBlend(MotionChestRig &chest, std::uint8_t sourceRig, float scale, float blend);
std::int32_t EvaluateHead(MotionHeadRig &head, std::uint8_t sourceRig, float scale, float blend);
std::int32_t EvaluateArm(MotionArmRig &arm, std::uint8_t sourceRig, float scale, float blend);
void ReadMotionBase(IKRig &rig, MotionBaseRig &base, const MotionStreamContext *context, float sourceTime, float time, std::uint32_t flags);
void ReadMotionChest(IKRig &rig, MotionChestRig &chest, const MotionStreamContext *context, float time, std::uint32_t flags);
void ReadMotionHead(IKRig &rig, MotionHeadRig &head, const MotionStreamContext *context, float time, std::uint32_t flags);
void ReadMotionArm(IKRig &rig, MotionArmRig &arm, const MotionStreamContext *context, float time, std::uint32_t flags);
void TranslatePelvisFromChannels(MotionBaseRig &base, std::uint32_t interpolationMode);
void BuildChannelTransformAtTime(MotionBaseRig &base, Matrix4x4 *output, float time, std::uint32_t interpolationMode);
void SolveBase(MotionBaseRig &base, std::uint32_t interpolationMode);
void SolveChestLookAt(MotionChestRig &chest, std::uint32_t interpolationMode);
void SolveHeadLookAt(MotionHeadRig &head, std::uint32_t interpolationMode);
void SolveArmIK2Bone_FKBlend(MotionArmRig &arm, std::uint32_t interpolationMode);
void IK_BuildAndSolve(IKRig &rig, std::uint32_t interpolationMode);
void Matrix4x4_PushMatrixAndTranslate(const MotionHeadRig &head, Matrix4x4 *output);

} // namespace shenmue::s2
