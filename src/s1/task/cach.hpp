#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
struct PackModel;
struct Character;
struct CharacterNode;

struct CachAttachmentRecord {
    PackModel *modelOwner;
    Vector3 position;
    Vector3i rotation;
};

// One examination hand's placement: the point it settles at and the step it moves by.
struct CachHandPlacement {
    Vector3 position;
    Vector3 step;
};




struct CameraControlState {
    Character *chara;
    CharacterNode *savedLink;
    Vector3 savedPosition;
    Vector3i savedRotation;
    Matrix4x4 gripTransform;
    Vector3 anchorPosition;
    Vector3i anchorRotation;
    Vector3 savedEye;
    Vector3 savedTarget;
    Vector3 eye;
    Vector3 boundsCenter;
    Vector3 viewEye;
    Vector3 analogDelta;
    Vector3 handBase;
    std::byte reserved_0d4[0x0c];
    Vector3i referenceRotation;
    Vector3 gripOffset[3];
    Vector3i gripRotation;
    std::int32_t swayPhase[3];
    Vector3 halfExtents;
    Vector3 gripExtents;
    Vector3 handTarget[2];
    Vector3i armRotation[2];
    Vector3i handBaseRotation[2];
    Vector3i armHoldRotation[2];
    Vector3i handHoldRotation[2];
    Vector3 handPoint[2];
    Vector3i armJointRotation[2];
    Vector3 destination;
    Vector3 camera0Position;
    Vector3i armMidRotation[2];
    Vector3i armEndRotation[2];
    float reach;
    std::int32_t yaw;
    float yawSine;
    float yawCosine;
    float savedPlayerOpacity;
    std::int32_t targetFov;
    std::int32_t previousFov;
    float speedCurrent;
    float speedVelocity;
    float speedMinimum;
    float speedMaximum;
    std::int32_t newShenState;
    std::int32_t gameState;
    std::int16_t mode;
    std::int16_t inputMode;
    std::int16_t investigateMode;
    std::uint8_t helpToken;
    std::uint8_t helpButton;
    std::byte reserved_26c[0x54];
};

struct CachContextState {
    Character *chara;
    void *primaryTransform;
    void *secondaryTransform;
    Vector3 previousEye;
    Vector3 previousTarget;
    Vector3 eyePosition;
    Vector3 targetPosition;
    Vector3 cameraAux;
    Vector3 analogDelta;
    Vector3 modelBoundsCenterWorld;
    Vector3 appliedPosition;
    float secondaryTranslationScalar;
    std::byte reserved_07c[4];
    Vector3 target;
    Vector3 eye;
    Vector3 acceleration;
    Vector3 halfExtents;
    Vector3 acceleration3;
    Vector3 smoothedInput;
    std::int32_t partAngleOffset[2];
    Vector3 handTarget[2];
    Vector3i armRotation[2];
    Vector3i handBaseRotation[2];
    CachHandPlacement hand[2];
    Vector3i armJointRotation[2];
    Vector3i handRotation[2];
    Vector3i armEndRotation[2];
    Matrix4x4 primaryAuthoredTransform;
    Matrix4x4 secondaryAuthoredTransform;
    std::int32_t yaw;
    float yawSine;
    float yawCosine;
    float savedPlayerOpacity;
    std::int32_t examineFov;
    std::int32_t savedFov;
    CachAttachmentRecord attachments[2];
    Vector3 attachmentRootPosition;
    Vector3i attachmentRootRotation;
    float attachmentOpacity;
    Vector3 lastCurvePosition;
    Vector3 lastAcceleratedTarget;
    std::int32_t previousAuthorState;
    std::int32_t gameState;
    std::int16_t state;
    std::int16_t interactionType;
    std::int16_t observeMode;
    std::uint8_t previousHelpTokenId;
    std::int8_t previousButtonId;
    std::byte reserved_2ac[0x14];
};




inline constexpr std::int8_t kExamineReachPoseType40[57]{0, 5, -10, 0, 5, -10, 0, 2, -5, 0, 0, -60, 0, 0, -35, 0, 0, -35, 0, 0, 15, 0, 0, -50, 0, 0, -40, 0,
    0, -40, 0, 0, 20, 0, 0, -55, 0, 0, -55, 0, 0, -55, 0, 0, 20, 0, -5, -65, 0, -5, -70, 0, -5, -40, 0, 0, 10};
// The grasp for examination types 2, 3, 20 and 21.
inline constexpr std::int8_t kExamineGraspPose[57]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -80, 0, 0, -70, 0, 0, -60, 0, 0, 0, 0, 0, -80, 0, 0, -70, 0, 0, -55, 0, 0,
    0, 0, 0, -80, 0, 0, -50, 0, 0, -45, 0, 0, 0, 0, -10, -60, 0, -15, -45, 0, 0, -5, 0, 0, 0};
// The grasp for examination type 40.
inline constexpr std::int8_t kExamineGraspPoseType40[57]{-15, 0, -20, -15, 0, -20, -7, 0, -10, 0, 0, -60, 0, 0, -45, 0, 0, -45, 0, 0, 20, 0, 0, -50, 0, 0, -50,
    0, 0, -50, 0, 0, 25, 0, 0, -55, 0, 0, -55, 0, 0, -55, 0, 0, 20, 0, -5, -65, 0, -5, -70, 0, -5, -40, 0, 0, 10};
// The reach for examination types 2, 3, 20 and 21.
inline constexpr std::int8_t kExamineReachPose[57]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -20, 0, 0, -20, 0, 0, 0, 0, 0, 0, 0, 0, -20, 0, 0, -20, 0, 0, 0,
    0, 0, 0, 0, 0, -20, 0, 0, -15, 0, 0, 0, 0, -5, -40, 0, -10, -20, 0, -10, -10, 0, 0, 0};
inline constexpr std::int8_t kTargetPoseDefault[57]{0, 0, 0, -10, 30, 0, -10, 20, -5, 0, 0, -5, 0, 0, -10, 0, 0, 10, 0, 0, 10, 0, 0, -5, 0, 0, -10, 0, 0, 10,
    0, 0, 10, 0, 0, -5, 0, 0, -10, 0, 0, 10, 0, 0, 10, 0, 0, -5, 0, 0, -10, 0, 0, 10, 0, 0, 10};
// The reach for examination type 0 and the grasp for types 10 and 11.
inline constexpr std::int8_t kExamineReachPoseType0[57]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -20, 0, 0, -20, 0, 0, 0, 0, 0, 0, 0, 0, -20, 0, 0, -20, 0, 0,
    0, 0, 0, 0, 0, 0, -20, 0, 0, -15, 0, 0, 0, 0, -5, -40, 0, -10, -20, 0, -10, -10, 0, 0, 0};
// The reach for examination type 1.
inline constexpr std::int8_t kExamineReachPoseType1[57]{-10, 0, 0, -10, 0, -10, -20, 20, -20, 0, 0, -60, 0, 0, -60, 0, 0, 10, 0, 0, 0, 0, 0, -60, 0, 0, -60, 0,
    0, 10, 0, 0, 0, 0, 0, -50, 0, 0, -20, 0, 0, 5, 0, 0, 0, 0, 0, -10, 0, 0, -10, 0, 0, -10, 0, 0, 0};
// The grasp for examination type 0.
inline constexpr std::int8_t kExamineGraspPoseType0[57]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -8, 0, 0, -30, 0, 0, -20, 0, 0, 0, 0, 0, 5, 0, 0, -33, 0, 0, -20, 0,
    0, 0, 0, 0, -25, 0, 0, -20, 0, 0, -15, 0, 0, 0, 0, -5, -50, 0, -10, -30, 0, -10, -15, 0, 0, 0};
inline constexpr std::int8_t kTargetPoseMode8[57]{0, 0, -10, 0, -10, 0, 30, -10, 5, 0, 0, -10, 0, 0, -10, 0, 0, 5, 0, 0, 0, 0, 0, -10, 0, 0, -10, 0, 0, 5, 0,
    0, 0, 0, 0, -10, 0, 0, -10, 0, 0, 5, 0, 0, 0, 0, 0, -10, 0, 0, -10, 0, 0, 5, 0, 0, 0};
inline constexpr std::int8_t kTargetGripPoseDefault[57]{0, -15, -20, -10, 10, -10, -10, 0, -5, 0, 0, -55, 0, 5, -55, 5, 5, -40, 0, 0, 15, 0, 0, -55, 0, 0, -55,
    0, 0, -40, 0, 0, 18, 0, 0, -50, 0, 0, -50, 0, 0, -30, 0, 0, 15, 0, 0, -35, 0, -5, -35, -10, 0, -35, 0, 0, 10};
inline constexpr std::int8_t kTargetGripPoseMode8[57]{0, 0, -25, 0, -5, -15, 30, 15, 0, 0, 0, -60, 0, 5, -50, 0, 0, -40, 0, 0, 0, 0, 0, -60, 0, 0, -50, 0, 0,
    -40, 0, 0, 0, 0, 0, -60, -5, 0, -50, 0, 0, -40, 0, 0, 0, 0, 0, -60, 0, 0, -50, -10, -15, -40, 0, 0, 0};
// The grasp for examination type 1.
inline constexpr std::int8_t kExamineGraspPoseType1[57]{-10, 0, 0, -10, 0, -10, -20, 0, -20, 0, 0, -60, 0, 0, -60, 0, 0, -10, 0, 0, 0, 0, 0, -60, 0, 0, -60, 0,
    0, -10, 0, 0, 0, 0, 0, -50, 0, 0, -20, 0, 0, 0, 0, 0, 0, 0, 0, -10, 0, 0, -10, 0, 0, -10, 0, 0, 0};

#pragma pack(push, 1)
struct SpecialAssetScriptRecord {
    std::int32_t stringIndex;
    Vector3 position;
    Vector3i rotation;
};
#pragma pack(pop)

static_assert(sizeof(CachAttachmentRecord) == 0x20);
static_assert(offsetof(CachAttachmentRecord, position) == 0x08);
static_assert(offsetof(CachAttachmentRecord, rotation) == 0x14);
static_assert(offsetof(CameraControlState, savedLink) == 0x08);
static_assert(offsetof(CameraControlState, savedPosition) == 0x10);
static_assert(offsetof(CameraControlState, savedRotation) == 0x1c);
static_assert(offsetof(CameraControlState, gripTransform) == 0x28);
static_assert(offsetof(CameraControlState, anchorPosition) == 0x68);
static_assert(offsetof(CameraControlState, anchorRotation) == 0x74);
static_assert(offsetof(CameraControlState, savedEye) == 0x80);
static_assert(offsetof(CameraControlState, savedTarget) == 0x8c);
static_assert(offsetof(CameraControlState, eye) == 0x98);
static_assert(offsetof(CameraControlState, boundsCenter) == 0xa4);
static_assert(offsetof(CameraControlState, viewEye) == 0xb0);
static_assert(offsetof(CameraControlState, analogDelta) == 0xbc);
static_assert(offsetof(CameraControlState, handBase) == 0xc8);
static_assert(offsetof(CameraControlState, referenceRotation) == 0xe0);
static_assert(offsetof(CameraControlState, gripOffset) == 0xec);
static_assert(offsetof(CameraControlState, gripRotation) == 0x110);
static_assert(offsetof(CameraControlState, halfExtents) == 0x128);
static_assert(offsetof(CameraControlState, handTarget) == 0x140);
static_assert(offsetof(CameraControlState, armRotation) == 0x158);
static_assert(offsetof(CameraControlState, swayPhase) == 0x11c);
static_assert(offsetof(CameraControlState, gripExtents) == 0x134);
static_assert(offsetof(CameraControlState, handBaseRotation) == 0x170);
static_assert(offsetof(CameraControlState, armHoldRotation) == 0x188);
static_assert(offsetof(CameraControlState, handHoldRotation) == 0x1a0);
static_assert(offsetof(CameraControlState, handPoint) == 0x1b8);
static_assert(offsetof(CameraControlState, armJointRotation) == 0x1d0);
static_assert(offsetof(CameraControlState, destination) == 0x1e8);
static_assert(offsetof(CameraControlState, camera0Position) == 0x1f4);
static_assert(offsetof(CameraControlState, armMidRotation) == 0x200);
static_assert(offsetof(CameraControlState, armEndRotation) == 0x218);
static_assert(offsetof(CameraControlState, reach) == 0x230);
static_assert(offsetof(CameraControlState, yaw) == 0x234);
static_assert(offsetof(CameraControlState, yawSine) == 0x238);
static_assert(offsetof(CameraControlState, targetFov) == 0x244);
static_assert(offsetof(CameraControlState, previousFov) == 0x248);
static_assert(offsetof(CameraControlState, speedCurrent) == 0x24c);
static_assert(offsetof(CameraControlState, newShenState) == 0x25c);
static_assert(offsetof(CameraControlState, gameState) == 0x260);
static_assert(offsetof(CameraControlState, mode) == 0x264);
static_assert(offsetof(CameraControlState, inputMode) == 0x266);
static_assert(offsetof(CameraControlState, investigateMode) == 0x268);
static_assert(offsetof(CameraControlState, helpToken) == 0x26a);
static_assert(offsetof(CameraControlState, helpButton) == 0x26b);
static_assert(sizeof(CameraControlState) == 0x2c0);
static_assert(sizeof(CachHandPlacement) == 0x18);
static_assert(offsetof(CachContextState, partAngleOffset) == 0xc8);
static_assert(offsetof(CachContextState, handTarget) == 0xd0);
static_assert(offsetof(CachContextState, armRotation) == 0xe8);
static_assert(offsetof(CachContextState, handBaseRotation) == 0x100);
static_assert(offsetof(CachContextState, hand) == 0x118);
static_assert(offsetof(CachContextState, armJointRotation) == 0x148);
static_assert(offsetof(CachContextState, handRotation) == 0x160);
static_assert(offsetof(CachContextState, armEndRotation) == 0x178);
static_assert(offsetof(CachContextState, primaryAuthoredTransform) == 0x190);
static_assert(offsetof(CachContextState, secondaryAuthoredTransform) == 0x1d0);
static_assert(offsetof(CachContextState, yaw) == 0x210);
static_assert(offsetof(CachContextState, yawSine) == 0x214);
static_assert(offsetof(CachContextState, yawCosine) == 0x218);
static_assert(offsetof(CachContextState, savedPlayerOpacity) == 0x21c);
static_assert(offsetof(CachContextState, examineFov) == 0x220);
static_assert(offsetof(CachContextState, savedFov) == 0x224);
static_assert(offsetof(CachContextState, attachments) == 0x228);
static_assert(offsetof(CachContextState, attachmentRootPosition) == 0x268);
static_assert(offsetof(CachContextState, attachmentRootRotation) == 0x274);
static_assert(offsetof(CachContextState, attachmentOpacity) == 0x280);
static_assert(offsetof(CachContextState, state) == 0x2a4);
static_assert(offsetof(CachContextState, previousAuthorState) == 0x29c);
static_assert(offsetof(CachContextState, gameState) == 0x2a0);
static_assert(offsetof(CachContextState, observeMode) == 0x2a8);
static_assert(offsetof(CachContextState, interactionType) == 0x2a6);
static_assert(offsetof(CachContextState, analogDelta) == 0x54);
static_assert(sizeof(SpecialAssetScriptRecord) == 0x1c);
static_assert(sizeof(CachContextState) == 0x2c0);


extern void *g_camera;
extern std::int32_t g_cameraGlobalState;

extern void *g_cameraControl;

extern void *g_cameraTargetEntity;
extern void *g_shenmueCamera0;

void CACH_Callback(HLib::TaskData *taskData);
void CACH_Related(CachContextState *cameraContext);
void CACH_Enqueue();
void CACH_Shutdown();
HLib::Task *CACH_GetTask();
std::int32_t camCtrlGetState();
std::int32_t shenResetCamInteractStates(Character *entity, Character *cameraEntity);
std::int32_t shenApplyTransformsForInteractStates();
std::int32_t CACH_BeginTargetMode(Character *target, std::uint16_t requestedMode);
std::int32_t CACH_UpdateTargetModeStateMachine();
std::int32_t CACH_UpdateInterpolatedTargetSpeed();
std::int32_t CACH_BeginInteractionMode(void *targetOrPosition, std::int32_t type);
std::int32_t CACH_PollInteractionMode();
std::int32_t CACH_LoadSpecialAssets(const SpecialAssetScriptRecord records[2]);
std::int32_t CACH_UpdateSpecialAssetInput();
std::int32_t CACH_RequestTargetTransition(std::int16_t mode);
std::int32_t CACH_RequestInteractionExit();
std::int32_t CACH_SetSpecialAssetList(void *list);
void CACH_QueueSoundCommands(std::uint32_t globalCommand, std::uint32_t soundCommand);
void AssignHNDP();
void CACH_Init();

} // namespace shenmue::s1
