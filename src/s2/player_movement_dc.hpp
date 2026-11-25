#pragma once

#include "s2/player_model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct PlayerMovementState {
    std::uint32_t flags;
    Character *character;
    MotionWork *motion;
    std::uint32_t dispatchMode;
    union {
        std::uint32_t state, requestedMode;
    };
    std::uint32_t previousRequestedMode;
    std::byte retained18[0xc];
    union {
        std::uint16_t retained24;
        std::int16_t currentYaw;
    };
    union {
        std::uint16_t retained26;
        std::int16_t targetYaw;
    };
    std::byte retained28[4];
    std::int16_t rootYawOffset, pendingYawCorrection, motionTransitionParameter;
    std::uint16_t retained32;
    float motionStartFrame;
    std::uint32_t retained38;
    float motionScale;
    union {
        std::uint32_t retained40;
        float inputScale;
    };
    std::uint16_t retained44, retained46;
    union {
        std::uint32_t retained48, playerMotionId;
    };
    std::uint32_t rootYawMode;
    union {
        std::uint32_t retained50, pendingMotionRequest;
    };
    std::int8_t idleVariationCounter;
    std::byte retained55[3];
    std::int32_t parentBoneSelector;
    std::byte retained5c[8];
    std::uint32_t playerMotionFlags;
    std::int16_t motionEntityYaw, motionNodeYaw;
    std::uint32_t retained6c;
    Vector3 motionRootEntityPosition, motionCharacterPosition;
    std::byte retained88[0xc];
    std::int8_t groundedStepIndex, upperBodyStepIndex, previousStepIndex, stepDirection;
    std::int8_t targetHeightScale;
    std::byte retained99[3];
    Vector3 targetWorldPosition;
    std::int16_t targetWorldYaw;
    std::byte retainedaa[2];
    Vector3 rootConsumerPosition, stairBoundaryAnchor;
    std::int16_t stairBoundaryYaw;
    std::uint16_t retainedc6;
    std::uint32_t retainedc8;
    std::int8_t sceneGroundIndex;
    std::byte retainedcd[3];
    std::uint32_t cachedCharacterControlFlags;
    float controllerGroundBlend[2], controllerVerticalOffset[2];
    Vector3 controllerPreviousLocal[2], controllerDelta[2];
    std::byte retained114[8];
    float obstacleDistance;
    std::uint32_t retained120;
    union {
        float retained124, pendingPositionCorrection;
    };
    float correctionCycleDistance;
    std::byte retained12c[0xc];
    std::int16_t correctionCycleCount;
    std::uint16_t retained13a;
    const std::byte *playerMotionFlagCommands, *playerMotionFlagCursor;
    float playerMotionFrame;
    float firstScale, secondScale;
    float rootMotionStep, previousFirstScale, verticalMotionScale, previousRigScale;
    std::uint32_t motionAnimationId;
    float motionSpeed, movementResult;
    Vector3 transitionTargetPosition;
    std::uint16_t transitionTargetYaw;
    std::byte retained17a[6];
    std::int16_t movingFrames, stationaryFrames, turningFrames;
    std::uint16_t retained186;
};
typedef char Layout_player_movement_dc_0[(sizeof(PlayerMovementState) == 0x188) ? 1 : -1];
typedef char Layout_player_movement_dc_1[(offsetof(PlayerMovementState, motion) == 8) ? 1 : -1];
typedef char Layout_player_movement_dc_2[(offsetof(PlayerMovementState, motionScale) == 0x3c) ? 1 : -1];
typedef char Layout_player_movement_dc_3[(offsetof(PlayerMovementState, idleVariationCounter) == 0x54) ? 1 : -1];
typedef char Layout_player_movement_dc_4[(offsetof(PlayerMovementState, retained124) == 0x124) ? 1 : -1];
typedef char Layout_player_movement_dc_5[(offsetof(PlayerMovementState, firstScale) == 0x148) ? 1 : -1];
typedef char Layout_player_movement_dc_7[(offsetof(PlayerMovementState, rootConsumerPosition) == 0xac) ? 1 : -1];
typedef char Layout_player_movement_dc_8[(offsetof(PlayerMovementState, stairBoundaryAnchor) == 0xb8) ? 1 : -1];
typedef char Layout_player_movement_dc_9[(offsetof(PlayerMovementState, stairBoundaryYaw) == 0xc4) ? 1 : -1];
typedef char Layout_player_movement_dc_10[(offsetof(PlayerMovementState, obstacleDistance) == 0x11c) ? 1 : -1];

struct PlayerMotionSelection {
    std::byte retained[0x20];
    std::uint32_t mirrored;
};
typedef char Layout_player_movement_dc_6[(offsetof(PlayerMotionSelection, mirrored) == 0x20) ? 1 : -1];

namespace player_detail {
struct PlayerMotionRequest {
    std::uint32_t previousMotion, previousClass;
    float previousFrameCount, previousFrame;
    std::int32_t motion;
    float frameCount, sampleFrame;
    std::uint16_t rootMode, blendFrames;
    std::uint32_t mirrored, retained24;
    float playbackSpeed;
    std::uint32_t blocked;
    std::int16_t requestedYaw, currentYaw, targetYaw;
    std::uint16_t retained36;
    MotionWork *motionWork;
    Character *character;
    std::uint32_t turnMotion;
    std::int16_t authoredYaw;
    std::uint16_t retained46;
    float previousFraction;
    std::int8_t parentSelector;
    std::uint8_t stopTranslation;
    std::uint16_t retained4e;
    std::uint32_t playerMotionFlags, retained54, flags;
};
typedef char Layout_player_motion_request_0[sizeof(PlayerMotionRequest) == 0x5c ? 1 : -1];
typedef char Layout_player_motion_request_1[offsetof(PlayerMotionRequest, motionWork) == 0x38 ? 1 : -1];
} // namespace player_detail

void InitializePlayerMovementController(PlayerMovementState *state);
std::int32_t plymGetMovingFrames();
std::int32_t plymGetTurningFrames();
std::uint32_t ChooseRestingPlayerMotion(PlayerMovementState *state, PlayerMotionSelection *selection);
void UpdatePlayerMovementState(PlayerMovementState &state);
std::int32_t RequestPlayerMotion(PlayerMovementState &state, std::int32_t requested);
namespace player_detail {
std::int32_t plymRaycastFromModel(PlayerModelState &state, const Vector3 &destination);
}
void plymSetRequestedSceneContext(std::int16_t minute, std::int16_t act, shen_name area, std::int16_t entry);
std::int16_t plymGetAngY();
std::int32_t plymIsAuthorTargetActive();
std::int32_t EV_plymSetSceneContext(std::int16_t startMinute, std::int16_t endMinute, std::int16_t act, shen_name area, std::int16_t entry);
std::int32_t plymSetSpecialModeTargetCharacter(shen_name character);
std::int32_t plymEvaluateAuthorTargetRequest();
std::int32_t plymIsSpecialModeFlag800();
std::int32_t plymIsPlacementBlockedForParty();
void plymSetDialogueMode();

} // namespace s2
} // namespace shenmue

#endif
