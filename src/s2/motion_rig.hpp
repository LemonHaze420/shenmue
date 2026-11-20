#pragma once

#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

#define SHENMUE_MOTION_RIG_LAYOUT(condition) static_assert(condition)
#define SHENMUE_MOTION_RIG_CONSTANT inline constexpr
#define SHENMUE_MOTION_RIG_INITIALIZER
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
    float inverseDuration;       // +0x20
    std::uint32_t keysRemaining; // +0x24
    std::uint32_t keyCount;      // +0x28, authored tuples (explicit keys plus two)
    std::uint32_t reserved2c;
    const std::uint16_t *frameCursor; // +0x30
    const std::uint16_t *keyBlock;    // +0x38, first tuple, reread when the stream is not reinitialized
    float scale;                      // +0x40
    float bias;                       // +0x44
};

struct MotionRigAxis {
    MotionRigChannel channel;
    float dimensions[2]; // +0x48
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
    MotionRotationBasis basis; // +0x0f0
    MotionCallback callback;   // +0x120
};

// TwoJointFkNode: the override rig's two-channel toe joints.
struct MotionRigTwoJoint {
    MotionRigAxis axes[2];
    MotionRotationBasis basis; // +0x0a0
    MotionCallback callback;   // +0x0d0
};

// TargetPvaOverrideNode and every in-rig target group: three position channels whose
// last tail and the following word hold the solved target, then its override callback
// and the position/velocity/acceleration triplet the next transition blends from.
struct MotionRigTarget {
    MotionRigAxis axes[3]; // axes[2].dimensions are output X/Y
    float outputZ;         // +0x0f0
    float outputW;
    MotionCallback callback; // +0x0f8
    Vector4 pva[3];          // +0x108, x unused, y/z half tangents, w value
};

struct MotionBaseRig {
    Matrix4x4 rootPrepassTransform; // +0x000
    Matrix4x4 pelvisTransform;      // +0x040
    std::uint32_t poseMode;         // +0x080
    std::uint32_t motionFlags;      // +0x084
    MotionRigClock clock;           // +0x088
    float reserveda4;
    MotionRigAxis pelvisPosition[3];       // +0x0a8
    MotionRigAxis pelvisRotation[3];       // +0x198, Z/Y/X
    Matrix4x4 pelvisWorld;                 // +0x288
    MotionRotationBasis pelvisBasis;       // +0x2c8
    MotionCallback pelvisPositionCallback; // +0x2f8
    MotionCallback pelvisRotationCallback; // +0x308
    Vector4 pelvisPva[3];                  // +0x318
    MotionRigRotation hips;                // +0x348; axes[1] tail is the hip height
    MotionRigRotation *overrideHips;       // +0x478
    MotionRigTarget *overridePelvisTarget; // +0x480
    Matrix4x4 hipsWorld;                   // +0x488
    MotionRigRotation *leftHipNode;        // +0x4c8
    Matrix4x4 *leftHipWorld;               // +0x4d0
    MotionRigRotation leftThigh;           // +0x4d8
    MotionRigTarget leftFoot;              // +0x608; axes[0]/axes[1] tails are shin/thigh lengths
    Matrix4x4 leftThighWorld;              // +0x740
    Matrix4x4 leftShinWorld;               // +0x780
    MotionRigAxis leftAnkleRotation[3];    // +0x7c0
    Matrix4x4 leftAnkleWorld;              // +0x8b0
    MotionRotationBasis leftAnkleBasis;    // +0x8f0
    MotionCallback leftAnkleCallback;      // +0x920
    MotionRigAxis leftFootRotation[3];     // +0x930; axes[1]/axes[2] tails are the foot offset
    Matrix4x4 leftFootWorld;               // +0xa20
    std::byte reserveda60[0x40];
    MotionRigRotation *rightHipNode;     // +0xaa0
    Matrix4x4 *rightHipWorld;            // +0xaa8
    MotionRigRotation rightThigh;        // +0xab0
    MotionRigTarget rightFoot;           // +0xbe0
    Matrix4x4 rightThighWorld;           // +0xd18
    Matrix4x4 rightShinWorld;            // +0xd58
    MotionRigAxis rightAnkleRotation[3]; // +0xd98
    Matrix4x4 rightAnkleWorld;           // +0xe88
    MotionRotationBasis rightAnkleBasis; // +0xec8
    MotionCallback rightAnkleCallback;   // +0xef8
    MotionRigAxis rightFootRotation[3];  // +0xf08; axes[1]/axes[2] tails are the foot offset
    Matrix4x4 rightFootWorld;            // +0xff8
    std::byte reserved1038[0x40];
    MotionRigTwoJoint *rightToe1Node; // +0x1078
    Matrix4x4 *rightToe1World;        // +0x1080
    MotionRigTwoJoint *rightToe2Node; // +0x1088
    Matrix4x4 *rightToe2World;        // +0x1090
    MotionRigTwoJoint *rightToe3Node; // +0x1098
    Matrix4x4 *rightToe3World;        // +0x10a0
};

struct MotionChestRig {
    std::uint32_t poseMode;          // +0x000
    MotionRigClock clock;            // +0x004
    MotionRigRotation rotation;      // +0x020
    MotionRigTarget target;          // +0x150
    Matrix4x4 chestWorld;            // +0x288
    MotionRigRotation upperRotation; // +0x2c8
    Matrix4x4 upperChestWorld;       // +0x3f8
};

struct MotionHeadRig {
    std::uint32_t poseMode; // +0x000
    MotionRigClock clock;   // +0x004
    Vector3 lookTarget;     // +0x020
    std::uint32_t reserved2c;
    MotionRigRotation baseRotation;   // +0x030
    Matrix4x4 neckWorld;              // +0x160
    MotionRigRotation midRotation;    // +0x1a0
    MotionRigTarget target;           // +0x2d0
    Matrix4x4 headWorld;              // +0x408
    Vector4 eyeOffset;                // +0x448
    MotionRigRotation *aimSourceNode; // +0x458
    MotionRigTarget *aimTargetNode;   // +0x460
    Matrix4x4 *aimSourceWorld;        // +0x468
    MotionRigRotation *nodeA;         // +0x470
    Matrix4x4 *nodeAWorld;            // +0x478
    MotionRigRotation *nodeB;         // +0x480
    Matrix4x4 *nodeBWorld;            // +0x488
    MotionRigRotation *nodeC;         // +0x490
    Matrix4x4 *nodeCWorld;            // +0x498
};

// Region 3 drives the arm at IKRig +0x1988 (named rightArm here) and region 4 the arm
// at +0x1ff0 with ArmSideL set.
struct MotionArmRig {
    std::uint32_t poseMode;           // +0x000
    MotionRigClock clock;             // +0x004
    MotionRigRotation shoulder;       // +0x020
    Matrix4x4 shoulderWorld;          // +0x150
    MotionRigRotation elbow;          // +0x190
    MotionRigTarget target;           // +0x2c0; axes[0]/axes[1] tails are forearm/upper-arm lengths
    Matrix4x4 upperArmWorld;          // +0x3f8
    Matrix4x4 forearmWorld;           // +0x438
    MotionRotationBasis forearmBasis; // +0x478
    MotionCallback forearmCallback;   // +0x4a8
    Matrix4x4 wristWorld;             // +0x4b8
    MotionRigAxis wristRotation[3];   // +0x4f8, forearm twist X, wrist Z, wrist Y
    Matrix4x4 handWorld;              // +0x5e8
    MotionRotationBasis wristBasis;   // +0x628
    MotionCallback wristCallback;     // +0x658
};


enum MotionRegion : std::uint32_t {
LowerBody, Chest, Head, RightArm, LeftArm };

struct IKRig {
    std::uint32_t boneId;
    std::uint32_t useOverride;
    MotionBaseRig lowerBody; // +0x0008
    MotionChestRig chest;    // +0x10b0
    MotionHeadRig head;      // +0x14e8
    MotionArmRig rightArm;   // +0x1988
    MotionArmRig leftArm;    // +0x1ff0
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
    MP_FromChannels = 0,
    MP_FromAxisAngle = 1,
};

enum MotionInterpolationMode : std::uint32_t {
    MI_HermiteAuto = 0,
    MI_Linear = 1,
    MI_Hermite = 2,
};

SHENMUE_MOTION_RIG_CONSTANT std::uint32_t motionInitStream = 0x0001;
SHENMUE_MOTION_RIG_CONSTANT std::uint32_t motionInitMirror = 0x0002;
SHENMUE_MOTION_RIG_CONSTANT std::uint32_t motionInitLeftArm = 0x0004;
SHENMUE_MOTION_RIG_CONSTANT std::uint32_t motionInitChannels = 0x0008;
SHENMUE_MOTION_RIG_CONSTANT std::uint32_t motionInitSwapLeadFoot = 0x0010;
SHENMUE_MOTION_RIG_CONSTANT std::uint32_t motionInitSplitWindow = 0x0020;
SHENMUE_MOTION_RIG_CONSTANT std::uint32_t motionInitCallbacks = 0x0040;
SHENMUE_MOTION_RIG_CONSTANT std::uint32_t motionInitTranslation = 0x0080;
SHENMUE_MOTION_RIG_CONSTANT std::uint32_t motionInitGraphFrames = 0xff00;
SHENMUE_MOTION_RIG_CONSTANT std::uint32_t motionInitBaking = 0x10000;
SHENMUE_MOTION_RIG_CONSTANT std::uint16_t motionWideKeyCounts = 0x0100;

SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionTimePair) == 0x0c);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionRigClock) == 0x1c);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionRigKey) == 0x10);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionRigChannel) == 0x48);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionRigChannel, keyCount) == 0x28);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionRigChannel, frameCursor) == 0x30);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionRigChannel, keyBlock) == 0x38);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionRigChannel, scale) == 0x40);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionRigAxis) == 0x50);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionCallback) == 0x10);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionRotationBasis) == 0x30);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionRigRotation) == 0x130);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionRigTwoJoint) == 0xe0);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionRigTarget) == 0x138);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionRigTarget, outputZ) == 0xf0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionRigTarget, callback) == 0xf8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionRigTarget, pva) == 0x108);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, poseMode) == 0x080);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, clock) == 0x088);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, pelvisPosition) == 0x0a8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, pelvisRotation) == 0x198);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, pelvisWorld) == 0x288);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, pelvisBasis) == 0x2c8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, pelvisPositionCallback) == 0x2f8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, pelvisPva) == 0x318);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, hips) == 0x348);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, overrideHips) == 0x478);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, hipsWorld) == 0x488);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, leftHipNode) == 0x4c8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, leftThigh) == 0x4d8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, leftFoot) == 0x608);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, leftThighWorld) == 0x740);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, leftAnkleRotation) == 0x7c0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, leftAnkleWorld) == 0x8b0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, leftAnkleBasis) == 0x8f0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, leftAnkleCallback) == 0x920);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, leftFootRotation) == 0x930);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, leftFootWorld) == 0xa20);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, rightHipNode) == 0xaa0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, rightThigh) == 0xab0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, rightFoot) == 0xbe0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, rightThighWorld) == 0xd18);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, rightAnkleRotation) == 0xd98);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, rightAnkleWorld) == 0xe88);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, rightAnkleBasis) == 0xec8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, rightFootRotation) == 0xf08);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, rightFootWorld) == 0xff8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionBaseRig, rightToe1Node) == 0x1078);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionBaseRig) == 0x10a8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionChestRig, rotation) == 0x020);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionChestRig, target) == 0x150);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionChestRig, chestWorld) == 0x288);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionChestRig, upperRotation) == 0x2c8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionChestRig, upperChestWorld) == 0x3f8);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionChestRig) == 0x438);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionHeadRig, baseRotation) == 0x030);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionHeadRig, neckWorld) == 0x160);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionHeadRig, midRotation) == 0x1a0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionHeadRig, target) == 0x2d0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionHeadRig, headWorld) == 0x408);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionHeadRig, eyeOffset) == 0x448);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionHeadRig, aimSourceNode) == 0x458);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionHeadRig, nodeCWorld) == 0x498);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionHeadRig) == 0x4a0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, shoulder) == 0x020);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, shoulderWorld) == 0x150);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, elbow) == 0x190);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, target) == 0x2c0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, upperArmWorld) == 0x3f8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, forearmWorld) == 0x438);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, forearmBasis) == 0x478);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, wristWorld) == 0x4b8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, wristRotation) == 0x4f8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, handWorld) == 0x5e8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, wristBasis) == 0x628);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(MotionArmRig, wristCallback) == 0x658);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionArmRig) == 0x668);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(IKRig, lowerBody) == 0x0008);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(IKRig, chest) == 0x10b0);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(IKRig, head) == 0x14e8);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(IKRig, rightArm) == 0x1988);
SHENMUE_MOTION_RIG_LAYOUT(offsetof(IKRig, leftArm) == 0x1ff0);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(IKRig) == 0x2658);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionStreamContext) == 0x58);
SHENMUE_MOTION_RIG_LAYOUT(sizeof(MotionOverride) == 0x10);


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

#undef SHENMUE_MOTION_RIG_CONSTANT
#undef SHENMUE_MOTION_RIG_INITIALIZER
#undef SHENMUE_MOTION_RIG_LAYOUT
