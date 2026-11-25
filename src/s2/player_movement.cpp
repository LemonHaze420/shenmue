#include "shenmue.h"
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
#include "s2/player_model_dc.hpp"
#include "s2/math.hpp"
#include "s2/player_movement_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/player_flags_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/search_state_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/scalar_math_dc.hpp"
#include "common/native_sqrt.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/camera_conversation_dc.hpp"
#include <cmath>
#include <cfloat>
#include <cstring>
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/player_internal.hpp"
#endif

namespace shenmue {

namespace s2 {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
float GetCharacterCollisionRadius(const Character *character);
static std::int32_t plymRequestSpecialModeTarget(shen_name character);
float mtwkGetRegionTimeOverride(const MotionWork *work, std::uint32_t region);

// @verified - d
void GetPlayerTargetWorldPosition(Vector3 *position) {
    PlayerModelState *state = g_playerRuntimePointer->model;
    PlayerMovementState *movement = reinterpret_cast<PlayerMovementState *>(&state->retainedSecondaryController);
    GetCharacterWorldPosition(state->character, position);
    if (!(movement->flags & 8U))
        return;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-movement->targetWorldYaw);
    Matrix4x4::preTranslate(-movement->targetWorldPosition.x, -movement->targetWorldPosition.y, -movement->targetWorldPosition.z);
    Vector3 local;
    Matrix4x4::TransformPoint(*position, local);
    PopCurrentMatrix(1);
    float offset = -local.z;
    offset *= 0.74999994f;
    const float height = (0.0f > offset ? 0.0f : offset) < static_cast<float>(movement->targetHeightScale) * 0.22499999f ?
        (0.0f > offset ? 0.0f : offset) :
        static_cast<float>(movement->targetHeightScale) * 0.22499999f;
    position->y = movement->targetWorldPosition.y + height;
}
#endif

namespace player_detail {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void UpdateTargetWorldMovementGate(PlayerModelState &state) {
    // PC v1.07 plymUpdateMovementContextGate. This gate is produced immediately
    // before PLYM_PrepareMovementContext and the movement-mode/motion callback.
    state.padFlags &= ~0x00400000U;
    const PlayerMovementState *const movement = state.movementState;
    if (!movement || (movement->flags & 8U) == 0)
        return;

    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-movement->targetWorldYaw));
    Matrix4x4::preTranslate(-movement->targetWorldPosition.x, -movement->targetWorldPosition.y, -movement->targetWorldPosition.z);
    Vector3 local = TransformWithActiveMatrix(state.searchLocation);
    Matrix4x4::Pop(1);

    local.z = -local.z;
    const std::uint32_t mode = state.rootYawState;
    const bool movementModeAllowsGate = (((mode - 1U) & 0xfffffffau) != 0U) || mode == 5U;
    std::uint32_t gate = 0x00400000U;
    if ((local.z < 0.0f || (static_cast<float>(movement->targetHeightScale - 1) * 0.30000001f) < local.z) && movementModeAllowsGate) {
        const std::int16_t yawDelta = static_cast<std::int16_t>(state.desiredYaw - movement->targetWorldYaw);
        if (local.y <= static_cast<float>(movement->targetHeightScale) * 0.30000001f * 0.5f) {
            gate = yawDelta < 0x4000 ? 0x00400000U : 0U;
        } else if (yawDelta <= 0x4000) {
            gate = 0U;
        }
    }
    state.padFlags |= gate;
}

std::uint16_t GetCharacterAccumulatedYaw(const Character *character) {
    return static_cast<std::uint16_t>(GetAllAngYForNode(character ? &character->entity.node : nullptr));
}

void ApplyPlayerMotionFlagCommand(PlayerMovementState &movement, const MotnPlayerMotionFlagCommand &command) {
    const std::uint32_t payload = command.flags & 0x9fffffffU;
    if (command.flags & 0x40000000U) {
        movement.playerMotionFlags |= payload;
    }
    if (command.flags & 0x20000000U) {
        movement.playerMotionFlags &= ~payload;
    }
    if (command.flags & 0x08000000U) {
        movement.playerMotionFlags |= 0x08000000U;
    }
    if (command.flags & 0x10000000U) {
        movement.playerMotionFlags |= 0x10000000U;
    }
}

// @verified - p
void ProcessPlayerMotionFlagCursor(PlayerMovementState &movement) {
    movement.playerMotionFlags &= 0xe7ffffffU;
    const std::byte *cursor = reinterpret_cast<const std::byte *>(movement.playerMotionFlagCursor);
    if (!cursor)
        return;
    while (std::to_integer<std::uint8_t>(cursor[2])) {
        std::uint16_t frame;
        std::memcpy(&frame, cursor, sizeof(frame));
        if (static_cast<float>(frame) > movement.playerMotionFrame)
            break;
        const std::uint8_t opcode = std::to_integer<std::uint8_t>(cursor[2]);
        if (opcode == 52U) {
            MotnPlayerMotionFlagCommand command;
            std::memcpy(&command.flags, cursor + 4, sizeof(command.flags));
            ApplyPlayerMotionFlagCommand(movement, command);
        }
        cursor += motnTimedCommandWidths[opcode];
    }
    movement.playerMotionFlagCursor = reinterpret_cast<const MotnPlayerMotionFlagCommand *>(cursor);
}

// @verified - p
void ResetPlayerMotionFlagCursor(PlayerMovementState &movement) {
    movement.playerMotionFlags = 0;
    movement.playerMotionFlagCommands = nullptr;
    movement.playerMotionFlagCursor = nullptr;
    const std::span<const std::byte> command = FindLoadedMotionSecondaryEntry(static_cast<std::uint16_t>(movement.playerMotionId), 8U);
    if (command.empty())
        return;
    movement.playerMotionFlagCommands = reinterpret_cast<const MotnPlayerMotionFlagCommand *>(command.data() + 4);
    movement.playerMotionFlagCursor = movement.playerMotionFlagCommands;
    ProcessPlayerMotionFlagCursor(movement);
}

// @verified - p
float mtwkSampleCurrentRemainingTime(const PlayerMovementState &movement) {
    Vector3 root{};
    SampleNativeMotionRoot(0xf005U, mtwkGetRigBoneId(movement.motionWork), -1.0f, root);
    return (std::fabs(root.z) / static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(0xf005U)) - 1)) * 1.3f;
}

void SynchronizePlayerModelState(Character *character, PlayerModelState &state) {
    // PLYM_SynchronizeRuntimeState publishes motion context before search
    // transforms and the per-frame runtime flag handoff.
    state.characterNode = character;
    MotionWork *const previousMotionWork = state.motionWork;
    state.motionWork = FindMotionWork(character);
    state.embeddedMovementState.character = character;
    state.embeddedMovementState.motionWork = state.motionWork;
    if (!previousMotionWork && state.motionWork) {
        plymInitializeMotionBinding(state.embeddedMovementState);
        state.remainingMotionTime = mtwkSampleCurrentRemainingTime(state.embeddedMovementState);
    }
    if (state.motionWork && mtwkGetAnimationId(state.motionWork, MotionRegion::LowerBody)) {
        state.padFlags |= 2U;
    }
    UpdatePlayerMovementState(*state.movementState, previousMotionWork != state.motionWork);
    state.previousRootYawState = state.rootYawState;
    state.rootYawState = state.embeddedMovementState.rootYawMode;
    const float speedTarget = state.embeddedMovementState.rootMotionStep / 0.041097f;
    state.speed = std::clamp(state.speed + (speedTarget - state.speed) * (0.5f * HLib::GetScaledDeltaTick()), 1.0f, 2.0f);
    state.searchLocation = GetCharacterWorldPosition(character);
    state.currentYaw = static_cast<std::int16_t>(GetCharacterAccumulatedYaw(character));
    SynchronizePlayerSearchTransform(state);

    state.previousPadFlags = state.padFlags;
    state.padFlags &= 0xff7fffffU;
    state.movementQueryState = 0;
    if (g_playerRuntime.flags & 0x01000000U) {
        g_playerRuntime.flags &= ~0x01000000U;
        state.padFlags |= 0x00800000U;
    }
    if (GetS2ControlMode() != 0)
        g_playerRuntime.flags |= 8U;
    else
        g_playerRuntime.flags &= ~8U;
    state.flags = (state.flags & 0x8939bac5U) | (g_playerRuntime.flags & 0x76c6453aU);
    g_playerRuntime.flags &= 0x9effffffU;
    if ((state.flags & 2U) == 0) {
        if (state.motionWork && mtwkSetExtraFlag(state.motionWork, 58U, 1)) {
            const auto current = static_cast<std::int16_t>(state.yaw);
            const auto target = static_cast<std::int16_t>(state.motionWork->runtime.entityRotation.y);
            state.yaw = static_cast<std::uint16_t>(
                static_cast<std::int32_t>(current + static_cast<std::int16_t>(target - current) * (0.050000001f * HLib::GetScaledDeltaTick())));
        } else {
            state.yaw = static_cast<std::uint16_t>(state.currentYaw);
        }
    }

    if ((state.flags & 0x4002U) == 0) {
        state.movementSelectionFlags = 0;
        state.inactiveDirection168 = 0;
        state.inactiveDirection188 = 0;
        state.inactiveDirection182 = 0;
    }
    if ((g_playerRuntime.flags & 0x00040000U) == 0) {
        ++state.specialModeFrame;
    }
    UpdatePlayerPartyCompanions(state);
    plymUpdateLockOnCharacter(state);
    Vector3 intersectionPosition = GetPlayerTargetWorldPosition();
    intersectionPosition.y += 0.5f;
    g_playerRuntime.flags &= ~0x00020000U;
    state.flags &= ~0x00020000U;
    if (plymDoIntersectionTest(intersectionPosition, intersectionPosition.y)) {
        g_playerRuntime.flags |= 0x00020000U;
        state.flags |= 0x00020000U;
    }
    if (g_cameraControl && g_cameraControl->contextMode == 2) {
        if (CharacterControlModel *control = FindCharacterControlModel(character)) {
            control->flags &= ~0x00020000U;
        }
    }
}

void mtwkInitFlags(MotionWork *motionWork, std::uint32_t region) {
    if (!motionWork)
        return;
    if (region) {
        if (region < 5)
            motionWork->runtime.regionFlags[region] |= 0x200U;
        return;
    }
    for (std::uint32_t index = 1; index < 5; ++index)
        motionWork->runtime.regionFlags[index] |= 0x200U;
}

void PreparePlayerMovementContext(PlayerModelState &state) {
    // PLYM_PrepareMovementContext initializes the collision/movement handoff consumed by
    // DoesRyoMovementCalculation later in the same PLYM update.
    state.placementRoot.next = nullptr;
    state.placementScratch.next = nullptr;
    state.placementRoot.flags &= 0x7ff6dff7U;
    state.placementScratch.flags &= 0x7ffff7f7U;
    state.placementScratch.flags |= 0x00000800U;
    state.placementRoot.character = state.characterNode;
    if (state.padFlags & 0x40U) {
        state.padFlags &= ~0x40U;
        state.placementRoot.flags |= 0x200U;
    }
    if ((state.flags & 0x00040000U) == 0) {
        if (state.motionInputState == 10U) {
            plymBuildInteractionPlacementContext(state);
            state.placementRoot.flags |= 0x80000000U;
        }
        state.activePlacementNode = &state.placementRoot;
        state.selectedPlacementNode = &state.placementRoot;
    }
}

// @verified - p

// @verified - p

// @verified - p
bool CanRequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested, float frame, float frameCount) {
    MotionWork *const work = movement.motionWork;
    if ((movement.flags & 4U) || mtwkSetRegionFlag(work, 65U, 1, MotionRegion::LowerBody))
        return true;
    const std::uint16_t current = mtwkGetAnimationId(work, MotionRegion::LowerBody);
    bool tracked = mtwkSetRegionFlag(work, 68U, 1, MotionRegion::LowerBody) != 0;
    if (current == 0xf005U && (requested == 0xf006 || static_cast<std::uint32_t>(requested - 0xf035) <= 8U))
        tracked = false;
    else if (current == 0xfa1aU || current == 0xfa20U)
        return true;
    switch (movement.rootYawMode) {
    case 0:
        return !tracked && requested != 0xf003;
    case 1:
        if (tracked || (requested != 0xf006 && static_cast<std::uint32_t>(requested - 0xf035) > 4U))
            return false;
        return (current == 0xf02cU || current == 0xf02dU || current == 0xf030U || current == 0xf031U) && frame > 3.0f;
    case 2:
    case 6:
        if ((requested >= 0xf008 && requested <= 0xf009) || (requested >= 0xf02a && requested <= 0xf02b))
            return (movement.playerMotionFlags & 3U) != 2U;
        return !tracked && ((movement.playerMotionFlags & 2U) == 0 || (movement.playerMotionFlags & 1U) != 0);
    case 3:
    case 4:
        return false;
    case 5:
        return !tracked && requested != -2 && requested != 0xf003;
    case 9:
        if (tracked)
            return false;
        return !((current == 0xf024U || current == 0xf029U || requested == 0xf003 || frame <= frameCount * 0.5f) &&
            (!(current >= 0xf020U && current <= 0xf023U || current >= 0xf025U && current <= 0xf028U) ||
                (requested != -2 && (requested <= 0xf004 || requested > 0xf006 && static_cast<std::uint32_t>(requested - 0xf035) > 8U)) ||
                ((movement.flags & 8U) != 0 && frame < 10.0f) || frame <= 2.0f));
    case 10:
        return !tracked && mtwkSetRegionFlag(work, 66U, 1, MotionRegion::LowerBody) != 0;
    default:
        return true;
    }
}

std::uint16_t SelectPlayerIdleMotion(PlayerMovementState &movement, bool &mirrored) {
    MotionWork *const work = movement.motionWork;
    const std::uint16_t current = mtwkGetAnimationId(work, MotionRegion::LowerBody);
    std::uint16_t selected = 0xf002U;
    mirrored = false;
    switch (current) {
    case 0xf002U:
        mirrored = mtwkSetRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
        if ((g_playerRuntime.flags & 0x00080000U) == 0 &&
            static_cast<std::int8_t>(movement.field05f) > static_cast<std::int32_t>(IncrementSharedPrngUInt16() % 4U + 2U)) {
            selected = 0xf004U;
        }
        break;
    case 0xf003U:
        selected = 0xf003U;
        break;
    case 0xf004U:
        mirrored = mtwkSetRegionFlag(work, 70U, 1, MotionRegion::LowerBody) == 0;
        break;
    default:
        mirrored = !mtwkIsRightHandAboveShoulder(work);
        break;
    }
    if ((g_playerRuntime.flags & 0x00080000U) == 0)
        ++movement.field05f;
    if (selected == 0xf004U)
        movement.field05f = 0;
    return selected;
}

// @verified - p
void plymAdjustControllerGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *work, void *) {
    PlayerModelState &state = *g_playerRuntime.model;
    if (!(g_playerRuntime.flags & 4U) || !(state.field436 & 1U))
        return;
    if (controller == 6U && !(state.field436 & 2U))
        return;
    if (controller == 5U && !(state.field436 & 4U))
        return;
    const std::uint32_t motion = mtwkGetAnimationId(work, MotionRegion::LowerBody);
    const std::uint32_t range = motion - 0xf008U;
    if (range <= 0x23U && ((0xc00000003ULL >> range) & 1U))
        return;
    if (motion - 0xf12eU <= 1U)
        return;
    const Vector3 root = mtwkGetRootPosition(work);
    const Vector3 probe{position->x, (position->y - 0.117f) + 1.5f, position->z};
    float ground = shenFindIntersectionWithScene(probe, probe.z);
    if (ground == -std::numeric_limits<float>::max())
        ground = root.y;
    else {
        Vector3s normal{};
        transformIntersectionResult(normal);
        const std::int32_t normalX = normal.x < 0 ? -static_cast<std::int32_t>(normal.x) : normal.x;
        if (!(ground <= ((static_cast<float>(normalX) * 0.00012207031f + 1.0f) * 0.44999999f) + root.y))
            ground = root.y;
    }
    const float height = std::fmax(0.0f, ground - root.y);
    float offset = (position->y - 0.117f) - root.y;
    const unsigned slot = controller == 6U ? 0U : 1U;
    PlayerMovementState &movement = state.embeddedMovementState;
    movement.controllerVerticalOffset[slot] = offset;
    if (height > 0.1f)
        offset *= std::fmax(0.0f, (0.5f - height) / 0.40000001f);
    const float blend = (height - movement.controllerGroundBlend[slot]) * 0.75f + movement.controllerGroundBlend[slot];
    movement.controllerGroundBlend[slot] = blend;
    position->y = ((blend + root.y) + 0.117f) + offset;
    const Vector3 location = GetCharacterWorldPosition(movement.character);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-GetAllAngYForNode(&movement.character->entity.node)));
    Matrix4x4::preTranslate(-location.x, -location.y, -location.z);
    const Vector3 local = TransformWithActiveMatrix(*position);
    Matrix4x4::Pop(1);
    const Vector3 previous = movement.controllerPreviousLocal[slot];
    movement.controllerDelta[slot] = {local.x - previous.x, local.y - previous.y, local.z - previous.z};
    movement.controllerPreviousLocal[slot] = local;
}

// @verified - p
void plymInitializeMotionBinding(PlayerMovementState &movement) {
    MotionWork *const work = movement.motionWork;
    if (!work)
        return;
    movement.field15c = 1.0f;
    movement.field160 = 1.0f;
    movement.field16c = 1.0f;
    mtwkSetExtraFlag(work, 55U, 0);
    mtwkAssignTargetCallback(work, plymAdjustControllerGroundTarget, 25);
    mtwkAssignTargetCallback(work, plymAdjustControllerGroundTarget, 30);
    mtwkSetTimeRelated(work, 1U, 0U, 0xf002U);
    mtwkSetTimeRelated(work, 1U, 1U, 0xf005U);
    mtwkSetTimeRelated(work, 1U, 4U, 0xf006U);
    const Vector3 location = GetCharacterWorldPosition(movement.character);
    Vector3 first{}, second{};
    mtwkGetControllerTargetPosition(work, 6U, first);
    mtwkGetControllerTargetPosition(work, 5U, second);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-GetAllAngYForNode(&movement.character->entity.node)));
    Matrix4x4::preTranslate(-location.x, -location.y, -location.z);
    movement.controllerPreviousLocal[0] = TransformWithActiveMatrix(first);
    movement.controllerPreviousLocal[1] = TransformWithActiveMatrix(second);
    Matrix4x4::Pop(1);
    bool mirrored = false;
    const std::uint16_t motion = SelectPlayerIdleMotion(movement, mirrored);
    mtwkSetRequestedRootMode(work, 1U);
    mtwkRequestMotion(work, motion, MotionRegion::LowerBody);
}

// @verified - p
void ResetPlayerMovementMotion(PlayerMovementState *movement, std::int32_t retainMotion) {
    MotionWork *const work = movement->motionWork;
    if (!work)
        return;
    movement->field160 = 1.0f;
    movement->field15c = 1.0f;
    movement->field16c = 1.0f;
    movement->rootYawOffset = 0;
    movement->motionTransitionParameter = 0;
    movement->motionStartFrame = 0.0f;
    mtwkSetTimeRelated(work, 1, 0, 0xf002);
    mtwkSetTimeRelated(work, 1, 1, 0xf005);
    mtwkSetTimeRelated(work, 1, 4, 0xf006);
    if (retainMotion == 0) {
        bool mirrored = false;
        const std::uint16_t motion = SelectPlayerIdleMotion(*movement, mirrored);
        mtwkSetSpeed(work, 1.0f, MotionRegion::LowerBody);
        mtwkSetRigScale(work, 1.0f);
        if (mtwkGetAnimationId(work, MotionRegion::LowerBody) != motion) {
            mtwkSetRequestedRootMode(work, 1);
            mtwkSetFrameTime(work, 20, MotionRegion::LowerBody);
            mtwkRequestMotion(work, motion, MotionRegion::LowerBody);
            if (mirrored)
                mtwkSetRegionFlag(work, 69, 0, MotionRegion::LowerBody);
        }
    }
}

bool IsPlayerOrdinaryGait(std::uint32_t motion) {
    return motion == 0xf005U || motion == 0xf006U || (motion >= 0xf035U && motion <= 0xf03dU);
}

bool IsPlayerStairGait(std::uint32_t motion) {
    return motion == 0xf008U || motion == 0xf009U || motion == 0xf02aU || motion == 0xf02bU || motion == 0xf12eU || motion == 0xf12fU;
}

// ConfigurePlayerGaitRequest: the phase is in authored frames, never in the temporary
// axis-angle blend clock. Opcode 70 changes the lead-foot half-cycle.
// @verified - p
void ConfigurePlayerGaitRequest(PlayerMotionRequest &request, MotionWork *work) {
    const std::uint16_t previous = request.previousMotion;
    const bool targetStairs = IsPlayerStairGait(request.motion);
    const bool middleHalf = request.previousFraction > 0.25f && request.previousFraction < 0.75f;
    if (previous == 0xf002U || previous == 0xf004U) {
        bool secondHalf = false;
        if (previous == 0xf002U) {
            request.parentSelector = mtwkIsRightHandAboveShoulder(work) ? 1 : -1;
        } else {
            secondHalf = request.previousFrame >= 20.0f;
            if (mtwkSetRegionFlag(work, 70U, 1, MotionRegion::LowerBody))
                secondHalf = !secondHalf;
            request.parentSelector = secondHalf ? -1 : 1;
        }
        request.rootMode = 2;
        if (targetStairs) {
            request.blendFrames = 2;
            request.mirrored = mtwkSetRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
        } else {
            if (previous == 0xf002U)
                secondHalf = mtwkSetRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
            request.blendFrames = 4;
            request.sampleFrame = request.frameCount * (secondHalf ? 0.51351351f : 0.027027028f);
        }
    } else if (IsPlayerOrdinaryGait(previous)) {
        if (targetStairs) {
            request.rootMode = 2;
            request.blendFrames = 4;
            request.mirrored = middleHalf;
        } else {
            request.rootMode = 0;
            request.blendFrames = 3;
            request.sampleFrame = ReachedMotionSpeed(request.motion) + request.previousFraction * request.frameCount;
        }
    } else if (previous == 0xf008U || previous == 0xf009U || previous == 0xf02aU || previous == 0xf02bU) {
        if (targetStairs) {
            request.rootMode = 2;
            request.blendFrames = 4;
            request.mirrored = middleHalf != (mtwkSetRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0);
        } else if (IsPlayerOrdinaryGait(request.motion)) {
            const bool secondHalf = middleHalf != (mtwkSetRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0);
            request.rootMode = 2;
            if (request.motion == 0xf005U) {
                request.sampleFrame = secondHalf ? 21.0f : 3.0f;
            } else if (request.motion == 0xf006U) {
                request.sampleFrame = secondHalf ? 9.0f : 1.0f;
            } else {
                request.sampleFrame = request.frameCount * (secondHalf ? 9.0f : 1.0f) / 18.0f;
            }
        } else {
            request.mirrored = mtwkSetRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
        }
    } else if (previous == 0xf12eU || previous == 0xf12fU) {
        const bool leadFoot = mtwkSetRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
        if (request.motion == 0xf005U || request.motion == 0xf006U) {
            request.rootMode = 2;
            request.sampleFrame = request.motion == 0xf005U ? (leadFoot ? 3.0f : 21.0f) : (leadFoot ? 1.0f : 9.0f);
        } else if (request.motion >= 0xf035U && request.motion <= 0xf03dU) {
            request.rootMode = 2;
            request.sampleFrame = request.frameCount * (leadFoot ? 1.0f : 9.0f) / 18.0f;
        } else {
            if (request.motion == 0xf008U || request.motion == 0xf009U || request.motion == 0xf02aU || request.motion == 0xf02bU) {
                request.rootMode = 2;
                request.blendFrames = 4;
            }
            request.mirrored = !leadFoot;
        }
    } else if ((previous >= 0xf020U && previous <= 0xf023U) || (previous >= 0xf025U && previous <= 0xf028U)) {
        request.rootMode = 2;
        if (targetStairs)
            request.blendFrames = 4;
        else
            request.sampleFrame = request.previousFraction * 4.0f + 4.0f;
    } else if (previous >= 0xf02cU && previous <= 0xf033U) {
        request.rootMode = 2;
        request.blendFrames = 3;
        request.sampleFrame = request.frameCount * (previous <= 0xf02fU ? 0.98648649f : 0.47297296f);
    }
}

// ConfigurePlayerMotionRequest selects the request policy;
// ConfigureOrdinaryGaitRequest owns the collision-bearing ordinary-gait-to-idle branch.
// @verified - p
void ConfigurePlayerMotionRequest(PlayerMotionRequest &request, PlayerMovementState &movement) {
    MotionWork *const work = movement.motionWork;
    request.frameCount = static_cast<float>(GetMotionFrameCount(static_cast<std::uint16_t>(request.motion)));
    if (request.motion == 0xf002U) {
        if (IsPlayerOrdinaryGait(request.previousMotion)) {
            request.blendFrames = 12;
            request.stopTranslation = true;
            ApplyPlayerIdleStop(movement);
        } else if (request.previousMotion >= 0xf020U && request.previousMotion <= 0xf029U) {
            request.blendFrames = 1;
            request.mirrored = request.previousMotion >= 0xf025U;
        } else {
            if (request.previousClass == 10U)
                request.blendFrames = 10;
            if ((movement.playerMotionFlags & 0x10000U) != 0)
                request.blendFrames = 5;
        }
    } else if ((request.motion >= 0xf02cU && request.motion <= 0xf033U) || (request.motion >= 0xf014U && request.motion <= 0xf017U)) {
        request.rootMode = 2;
        request.blendFrames = 3;
    } else if (IsPlayerOrdinaryGait(request.motion) || IsPlayerStairGait(request.motion)) {
        ConfigurePlayerGaitRequest(request, work);
    } else if (request.motion == 0xfa1aU || request.motion == 0xfa20U) {
        if (request.previousMotion == 0xfa1aU || request.previousMotion == 0xfa20U) {
            request.rootMode = 3;
            request.blendFrames = 8;
            request.sampleFrame = request.frameCount * request.previousFraction;
        }
        request.mirrored = mtwkSetRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
    }
    if (mtwkSetRegionFlag(work, 66U, 1, MotionRegion::LowerBody)) {
        if (request.sampleFrame >= request.frameCount) {
            request.sampleFrame -= request.frameCount;
        }
        if (request.sampleFrame < 0.0f)
            request.sampleFrame += request.frameCount;
    } else {
        if (request.sampleFrame >= request.frameCount) {
            request.sampleFrame = request.frameCount - 1.0f;
        }
        if (request.sampleFrame < 0.0f)
            request.sampleFrame = 0.0f;
    }
    mtwkSetRegionEvalTime(work, request.sampleFrame, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(work, request.rootMode);
    mtwkSetFrameTime(work, request.blendFrames, MotionRegion::LowerBody);
}

void ApplyPlayerIdleStop(PlayerMovementState &movement) {
    MotionWork *work = movement.motionWork;
    const std::uint16_t previousMotion = mtwkGetAnimationId(work, MotionRegion::LowerBody);
    Vector3 terminal{};
    SampleNativeMotionRoot(previousMotion, mtwkGetRigBoneId(work), -1.0f, terminal);
    const float distance = std::fabs(terminal.z) / (static_cast<float>(GetMotionFrameCount(previousMotion)) - 1.0f);
    Vector3 origin = GetCharacterWorldPosition(movement.character);
    Vector3 backward{0.0f, 0.0f, -1.0f};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(movement.currentYaw));
    Matrix4x4::Transform(backward, backward);
    Matrix4x4::Pop(1);
    Vector3 target{origin.x + backward.x * (distance * 4.0f), origin.y + backward.y * (distance * 4.0f), origin.z + backward.z * (distance * 4.0f)};
    const float radius = GetCharacterCollisionRadius(movement.character);
    origin.y += 0.40000001f;
    target.y += 0.40000001f;
    Vector3 scratch;
    if (ResolveCharacterCylinderOverlap(scratch, target, radius)) {
        target = {origin.x + (target.x - origin.x) * 0.5f, origin.y + (target.y - origin.y) * 0.5f, origin.z + (target.z - origin.z) * 0.5f};
    }
    flddToggleFlag(8);
    Vector3 hit{};
    if (RaycastForCollision(&hit, &target, &origin)) {
        Vector3 normal{};
        TransformIntersectionNormal(normal);
        target.x = hit.x + normal.x * 0.050000001f;
        target.z = hit.z + normal.z * 0.050000001f;
    }
    if (RaycastInWorldGrid(hit, target, radius)) {
        target.x = hit.x;
        target.z = hit.z;
    }
    target.y -= 0.40000001f;
    shenApplyTranslationToAll(&movement.character->entity.node, target);
    mtwkSetExtraFlag(work, 15U, 0);
    mtwkSetExtraFlag(work, 16U, mtwkIsRightHandBelowShoulder(work) ? 2 : 0);
}

void ApplyPlayerMotionRequest(PlayerMovementState &movement, std::uint32_t motionId, float previousFrame, float previousFrameCount, bool mirrored,
    std::int16_t requestedYaw, std::int16_t authoredYaw) {
    MotionWork *const motionWork = movement.motionWork;
    const std::uint16_t previousMotion = mtwkGetAnimationId(motionWork, MotionRegion::LowerBody);
    const float fraction = std::fabs(previousFrameCount) >= std::numeric_limits<float>::epsilon() ? previousFrame / (previousFrameCount - 1.0f) : 0.0f;
    PlayerMotionRequest request{previousMotion, motionId, movement.rootYawMode, previousFrame, fraction, 0.0f};
    request.mirrored = mirrored;

    movement.playerMotionId = motionId;
    const bool preserveRootYaw =
        mtwkSetRegionFlag(motionWork, 65U, 1, MotionRegion::LowerBody) != 0 || (movement.flags & 0x2000U) != 0 || movement.motionTransitionParameter != 0;
    const std::int32_t correction = preserveRootYaw ? movement.rootYawOffset : static_cast<std::int32_t>(static_cast<float>(movement.rootYawOffset) * fraction);
    movement.pendingYawCorrection = static_cast<std::int16_t>(movement.pendingYawCorrection + correction);
    movement.rootYawOffset = requestedYaw;
    movement.motionEntityYaw = static_cast<std::int16_t>(motionWork->runtime.entityRotation.y);
    movement.motionNodeYaw = GetAllAngYForNode(&movement.character->entity.node);
    movement.motionRootEntityPosition = motionWork->runtime.rootPosition;
    movement.motionCharacterPosition = GetCharacterWorldPosition(movement.character);

    ConfigurePlayerMotionRequest(request, movement);
    const float speed = ReachedMotionSpeed(motionId);
    mtwkRequestMotion(motionWork, motionId, MotionRegion::LowerBody);
    mtwkSetSpeed(motionWork, speed, MotionRegion::LowerBody);
    if (authoredYaw != 0) {
        mtwkAssignRigYawOffset(motionWork, static_cast<std::uint32_t>(static_cast<std::int16_t>(requestedYaw - authoredYaw)));
        movement.motionTransitionParameter = 0;
    } else {
        movement.motionTransitionParameter = requestedYaw;
    }
    movement.motionStartFrame = request.sampleFrame;
    mtwkSetRegionFlag(motionWork, 69U, request.mirrored ? 0 : 2, MotionRegion::LowerBody);
    movement.flags |= 0x0900U;
    movement.field160 = speed;
    movement.playerMotionFrame = request.sampleFrame;
    movement.field15c = 1.0f;
    movement.field16c = 1.0f;
    ResetPlayerMotionFlagCursor(movement);
    switch (motionId) {
    case 0xf020U:
    case 0xf025U:
        mtwkSetLayerEndFrame(motionWork, 19.0f, MotionRegion::LowerBody);
        break;
    case 0xf021U:
    case 0xf026U:
        mtwkSetLayerEndFrame(motionWork, 23.0f, MotionRegion::LowerBody);
        break;
    case 0xf022U:
    case 0xf027U:
        mtwkSetLayerEndFrame(motionWork, 27.0f, MotionRegion::LowerBody);
        break;
    case 0xf023U:
    case 0xf028U:
        mtwkSetLayerEndFrame(motionWork, 29.0f, MotionRegion::LowerBody);
        break;
    case 0xf024U:
    case 0xf029U:
        mtwkSetLayerEndFrame(motionWork, 41.0f, MotionRegion::LowerBody);
        break;
    case 0xfa08U:
        movement.rootYawOffset = static_cast<std::int16_t>(0x8000U);
        break;
    case 0xfa09U:
        movement.rootYawOffset = 8192;
        break;
    case 0xfa0aU:
        movement.rootYawOffset = 16384;
        break;
    case 0xfa0cU:
        movement.rootYawOffset = -8192;
        break;
    case 0xfa0dU:
    case 0xfa22U:
        movement.rootYawOffset = -16384;
        break;
    default:
        break;
    }
    movement.flags &= ~0x2000U;
    if ((motionId >= 0xf020U && motionId <= 0xf023U) || (motionId >= 0xf025U && motionId <= 0xf028U) || motionId == 0xf02cU || motionId == 0xf02dU ||
        motionId == 0xf030U || motionId == 0xf031U)
        movement.flags |= 0x2000U;
    movement.rootYawMode = ReachedMotionRootMode(motionId);
    if (((static_cast<std::uint32_t>(movement.rootYawMode) - 2U) & 0xfffffffbU) != 0U)
        movement.field04c = 0.0f;
    movement.parentBoneSelector = request.parentSelector;
    if (request.stopTranslation)
        movement.flags |= 0x00101000U;
}

// Native stair-gait set: bit test of 0xC00000003 over motion - 0xf008 (f008, f009, f02a, f02b), plus f12e/f12f.
bool IsReachedRootConsumerTranslationMotion(std::uint16_t motionId) {
    const std::uint32_t range = static_cast<std::uint32_t>(motionId) - 0xf008U;
    return (range <= 0x23U && ((std::uint64_t{0xc00000003} >> range) & 1U) != 0) || motionId - 0xf12eU <= 1U;
}

std::uint16_t SelectReachedRootConsumerMotion(const PlayerMovementState &movement, std::int8_t stepDirection) {
    if (stepDirection == 1) {
        if (movement.field04c <= 0.5f)
            return 0xf02bU;
        if (movement.groundedStepIndex == movement.targetHeightScale - 1) {
            return 0xf12fU;
        }
        return 0xf02aU;
    }
    if (stepDirection == -1) {
        if (movement.field04c <= 0.5f)
            return 0xf009U;
        if (movement.groundedStepIndex == 1)
            return 0xf12eU;
        return 0xf008U;
    }
    return 0;
}

bool ApplyReachedRootConsumerTranslation(PlayerMovementState &movement, std::uint16_t animationId) {
    if (!movement.character ||
        !IsReachedRootConsumerTranslationMotion(animationId) && !IsReachedRootConsumerTranslationMotion(static_cast<std::uint16_t>(movement.playerMotionId))) {
        return false;
    }

    const CharacterControlModel *const control = FindCharacterControlModel(movement.character);
    const float ccmFlag = control && control->owner == movement.character ? control->collisionRadius : 0.0f;

    const Vector3 characterPosition = GetCharacterWorldPosition(movement.character);
    Vector3 local{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-movement.targetWorldYaw));
    Matrix4x4::preTranslate(-movement.targetWorldPosition.x, -movement.targetWorldPosition.y, -movement.targetWorldPosition.z);
    Matrix4x4::TransformPoint(characterPosition, local);
    Matrix4x4::Pop(1);

    const float step = static_cast<float>(movement.groundedStepIndex);
    Vector3 stairLocal{local.x, step * 0.22499999f, local.z};
    if (movement.groundedStepIndex != 0 && movement.groundedStepIndex != movement.targetHeightScale)
        stairLocal.z = -(step * 0.30000001f - 0.15000001f);
    if (local.x > 0.0f)
        stairLocal.x = -ccmFlag;
    if (-movement.targetStairEdgeLength > local.x)
        stairLocal.x = ccmFlag - movement.targetStairEdgeLength;

    Vector3 world{};
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(movement.targetWorldPosition.x, movement.targetWorldPosition.y, movement.targetWorldPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(movement.targetWorldYaw));
    Matrix4x4::TransformPoint(stairLocal, world);
    Matrix4x4::Pop(1);

    // Native ApplyTranslationToAllAndRoot handles both the leaf fast path
    // and the recursive child-node branch before CHAR's child position is
    // reset by SetPosForChildNode.
    shenApplyTranslationToAll(&movement.character->entity.node, world);
    movement.character->position = {};
    movement.previousStepIndex = movement.groundedStepIndex;
    movement.rootConsumerPosition = world;
    return true;
}

// Native gait/stair motion selection and root consumer (called from the PLYM movement update).
void UpdateReachedRootConsumer(PlayerMovementState &movement) {
    MotionWork *const motionWork = movement.motionWork;
    std::int8_t stepDirection = movement.stepDirection;
    const std::uint16_t animationId = mtwkGetAnimationId(motionWork, MotionRegion::LowerBody);
    const std::uint32_t selectedMotion = (movement.flags & 0x100U) != 0 ? movement.playerMotionId : animationId;
    if (selectedMotion - 0xf005U > 1U && selectedMotion - 0xf035U > 8U && ((animationId - 0xf02cU) & 0xfffffffaU) != 0 &&
        !IsReachedRootConsumerTranslationMotion(static_cast<std::uint16_t>(selectedMotion)) && !IsReachedRootConsumerTranslationMotion(animationId))
        return;

    if (movement.stepDirection != 0 && (movement.flags & 8U) != 0 && motionWork) {
        const std::int16_t yawDifference = static_cast<std::int16_t>(static_cast<std::int16_t>(motionWork->runtime.entityRotation.y) - movement.targetWorldYaw);
        const std::int32_t absoluteDifference = yawDifference >= 0 ? yawDifference : -yawDifference;
        stepDirection = absoluteDifference < 12743 ? 1 : 0;
        if (absoluteDifference > 20024)
            stepDirection = -1;
    }

    if ((movement.flags & 8U) != 0 && stepDirection != 0) {
        const std::uint16_t motion = SelectReachedRootConsumerMotion(movement, stepDirection);
        if (motion && selectedMotion != motion)
            RequestPlayerMotion(movement, motion);
    } else if ((movement.flags & 0x100U) == 0) {
        std::int32_t index = static_cast<std::int32_t>(static_cast<double>(movement.field04c) / 0.099999);
        if (index < 0)
            index = 0;
        if (index > 10)
            index = 10;
        RequestPlayerMotion(movement, plymGaitMotions[static_cast<std::size_t>(index)]);
    }

    const bool publishRoot = (movement.flags & 0x100U) != 0 || mtwkSetRegionFlag(motionWork, 65U, 1, MotionRegion::LowerBody) != 0;
    if (!publishRoot)
        return;
    ApplyReachedRootConsumerTranslation(movement, animationId);
}

void FinalizeReachedRootConsumerScale(PlayerMovementState &movement) {
    MotionWork *const motionWork = movement.motionWork;
    if (!motionWork)
        return;

    mtwkSetSpeed(motionWork, movement.field160 * movement.field15c, MotionRegion::LowerBody);
    movement.previousField15c = movement.field15c;
    mtwkSetRigScale(motionWork, movement.field16c);
    // ApplyPlayerRootMotion retains the previous rig scale in the shared movement owner.
    movement.previousRigScale = movement.field16c;

    if (movement.parentBoneSelector != 0) {
        const bool parentRegionActive = mtwkSetRegionFlag(motionWork, 68U, 1, MotionRegion::LowerBody) != 0;
        if (!parentRegionActive && (movement.flags & 0x100U) == 0) {
            movement.parentBoneSelector = 0;
        } else {
            if (movement.parentBoneSelector > 0) {
                mtwkGetParentBoneWrapper(motionWork, movement.parentBoneSelector);
            }
            if (movement.parentBoneSelector < 0) {
                mtwkGetParentBoneWrapper(motionWork, movement.parentBoneSelector);
            }
            if (parentRegionActive && mtwkGetFrame(motionWork, MotionRegion::LowerBody) > 2.0f) {
                movement.parentBoneSelector = 0;
            }
        }
    }

    if ((movement.flags & 0x1000U) != 0 && mtwkSetRegionFlag(motionWork, 68U, 1, MotionRegion::LowerBody) == 0 && (movement.flags & 0x100U) == 0) {
        mtwkSetExtraFlag(motionWork, 15U, 2);
        movement.flags &= ~0x1000U;
    }
}

float ScaledDelta(float value) {
    return value * HLib::GetScaledDeltaTick();
}

float Lerp(float target, float base, float amount) {
    return (target - base) * amount + base;
}

std::int16_t LerpFixedAngle(std::int16_t target, std::int16_t base, float amount) {

    const auto distance = static_cast<std::int16_t>(target - base);
    return static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(distance) * amount + static_cast<float>(base)));
}

// UpdatePlayerCollisionSpeedScale splits collision-imposed speed into playback and rig scale.
void UpdatePlayerCollisionSpeedScale(PlayerMovementState &movement) {
    if (std::fabs(movement.movementResult) < 0.00000011920929f)
        return;
    const auto motion = mtwkGetAnimationId(movement.motionWork, MotionRegion::LowerBody);
    if (!IsPlayerOrdinaryGait(motion))
        return;
    const MotnSequence *sequence = FindLoadedMotionSequence(motion);
    if (!sequence)
        return;
    Vector3 terminal{};
    if (!SampleMotionRootTrack(*sequence, (sequence->frameCount - 1.0f) / 30.0f, static_cast<std::uint8_t>(movement.motionWork->rig.boneId), terminal))
        return;
    float ratio = std::fmax(0.60000002f, movement.movementResult / (std::fabs(terminal.z) / (sequence->frameCount - 1.0f)));
    float factor = 0.96153849f;
    if (ratio >= 1.625f)
        ratio = 1.625f;
    else if (ratio < 1.0f)
        factor = 0.9375f;
    const float speed = std::sqrt(ratio / factor);
    movement.field15c = Lerp(speed / movement.field160, movement.previousField15c, 0.30000001f);
    movement.field16c = Lerp(speed * factor, movement.previousRigScale, 0.30000001f);
}

// UpdatePlayerTargetYaw's signed-angle interpolation includes scaled frame delta.
void UpdatePlayerTargetYaw(PlayerMovementState &movement) {
    const float rate = std::fmin(1.0f, std::fmax(0.050000001f, movement.yawInterpolationRate * 0.050000001f));
    movement.currentYaw = LerpFixedAngle(movement.targetYaw, movement.currentYaw, ScaledDelta(rate));
}

// @verified - p
bool TryRequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested) {
    return RequestPlayerMotion(movement, requested) >= 0;
}

void UpdatePlayerLocomotionRequest(PlayerMovementState &movement) {
    // UpdatePlayerLocomotionRequest preserves copy-request order before ordinary yaw update.
    if (movement.requestedMode == 1U) {
        TryRequestPlayerMotion(movement, -2);
    } else if (movement.requestedMode == 6U) {
        TryRequestPlayerMotion(movement, -3);
        return;
    } else if (movement.requestedMode == 10U) {
        if (movement.flags & 0xc000U) {
            mtwkCopyTransition(movement.motionWork, &movement.transitionTargetPosition, movement.transitionTargetYaw, 0, 0);
        } else if (movement.flags & 0x4000U) {
            mtwkCopyTransition(movement.motionWork, &movement.transitionTargetPosition, movement.transitionTargetYaw, 0, 2);
        } else if (movement.flags & 0x8000U) {
            mtwkCopyTransition(movement.motionWork, &movement.transitionTargetPosition, movement.transitionTargetYaw, 0, 1);
        }
        mtwkCopyTransition(movement.motionWork, nullptr, 0, 2, 0);
    }
    UpdatePlayerTargetYaw(movement);
}

void ExecutePlayerMovement(PlayerMovementState &movement) {
    if (!movement.character || !movement.motionWork)
        return;
    movement.currentYaw = GetAllAngYForNode(&movement.character->entity.node);
    // ResolvePlayerRootYawMode lets script requests and MTWK copy ownership override root class.
    std::uint32_t mode;
    switch (movement.rootYawMode) {
    case 0:
        mode = 0;
        break;
    case 1:
        mode = 1;
        break;
    case 2:
        mode = 2;
        break;
    case 3:
        mode = 3;
        break;
    case 4:
        mode = 6;
        break;
    case 6:
        mode = 4;
        break;
    case 9:
        mode = 5;
        break;
    default:
        mode = 9;
        break;
    }
    if ((movement.flags & 2U) || movement.pendingMotionRequest)
        mode = 7;
    if (mtwkSetExtraFlag(movement.motionWork, 58U, 1) || (movement.flags & 0x20U))
        mode = 8;
    movement.dispatchMode = mode;
    // DispatchPlayerMovementMode covers all native movement cases, not a motion-ID stand-in.
    const std::uint32_t requested = movement.requestedMode;
    switch (mode) {
    case 0:
        TryRequestPlayerMotion(movement, requested == 2U || requested == 6U ? -3 : requested == 3U || requested == 4U ? -1 : requested == 13U ? -4 : -2);
        break;
    case 1:
    case 5:
    case 6:
        TryRequestPlayerMotion(movement, requested == 3U || requested == 4U ? -1 : -2);
        break;
    case 2:
        UpdatePlayerLocomotionRequest(movement);
        break;
    case 3:
        if (requested == 1U)
            TryRequestPlayerMotion(movement, -2);
        else if (requested == 3U || requested == 4U)
            TryRequestPlayerMotion(movement, -1);
        break;
    case 4:
        if (requested == 6U)
            TryRequestPlayerMotion(movement, -3);
        else {
            if (requested == 1U)
                TryRequestPlayerMotion(movement, -2);
            UpdatePlayerTargetYaw(movement);
        }
        break;
    case 7:
        if (static_cast<std::int32_t>(movement.pendingMotionRequest) > 0 &&
            TryRequestPlayerMotion(movement, static_cast<std::int32_t>(movement.pendingMotionRequest))) {
            movement.pendingMotionRequest = 0;
        }
        break;
    case 8:
        break;
    default:
        TryRequestPlayerMotion(movement, -2);
        break;
    }
    if (movement.dispatchMode - 7U > 1U && (movement.flags & 0x100U)) {
        movement.previousRequestedMode = movement.requestedMode;
    }
    movement.currentYaw = static_cast<std::int16_t>(movement.currentYaw + movement.pendingYawCorrection);
    movement.pendingYawCorrection = 0;
    shenDecrementRotYAllNodes(&movement.character->entity.node, movement.currentYaw);
    UpdatePlayerCollisionSpeedScale(movement);
    UpdateReachedFloorStairCollision(movement);
    PrepareReachedStairPhaseCorrection(movement);
    ApplyReachedCollisionRootCorrection(movement);
    UpdateReachedRootConsumer(movement);
    FinalizeReachedRootConsumerScale(movement);
    movement.currentYaw = static_cast<std::int16_t>(movement.currentYaw + movement.pendingYawCorrection);
    shenDecrementRotYAllNodes(&movement.character->entity.node, movement.currentYaw);
}

void UpdateReachedPlayerYawWriteback(PlayerModelState &state) {
    PlayerMovementState &movement = state.embeddedMovementState;
    if (!movement.character)
        return;
    std::int16_t yaw = GetAllAngYForNode(&movement.character->entity.node);
    auto useMotionYaw = [&]() {
        if (!movement.motionWork)
            return false;
        yaw = static_cast<std::int16_t>(movement.motionWork->runtime.entityRotation.y);
        return true;
    };
    switch (movement.rootYawMode) {
    case 0:
    case 10:
        if (state.padFlags & 0x200U)
            yaw = static_cast<std::int16_t>(state.forcedYaw);
        break;
    case 1:
    case 9:
        if (state.padFlags & 0x200U) {
            yaw = static_cast<std::int16_t>(yaw + movement.rootYawOffset);
        } else if (!useMotionYaw()) {
            return;
        }
        break;
    case 2:
    case 6:
        if (movement.rootYawOffset)
            yaw = static_cast<std::int16_t>(yaw + movement.rootYawOffset);
        break;
    case 4:
        if (!useMotionYaw())
            return;
        break;
    default:
        break;
    }
    if (movement.dispatchMode == 8U) {
        if (!movement.motionWork)
            return;
        yaw = LerpFixedAngle(
            static_cast<std::int16_t>(movement.motionWork->runtime.entityRotation.y), static_cast<std::int16_t>(state.yaw), ScaledDelta(0.050000001f));
    }
    if (state.padFlags & 0x80U)
        yaw = state.overrideYaw;
    state.yaw = static_cast<std::uint16_t>(yaw);
    // UpdatePlayerMountYaw rotates mounting/dismounting relative to the frame's node yaw.
    if (state.motionWork) {
        const auto motion = mtwkGetAnimationId(state.motionWork, MotionRegion::LowerBody);
        if (motion == 0xfa21U || motion == 0xfa22U) {
            const int frame = std::clamp(static_cast<int>(mtwkGetFrame(state.motionWork, MotionRegion::LowerBody) - 170.0f), 0, 30);
            const int offset = (frame << 14) / 30;
            const bool mirrored = mtwkSetRegionFlag(state.motionWork, 70U, 1, MotionRegion::LowerBody) != 0;
            state.yaw = static_cast<std::uint16_t>(state.currentYaw + ((motion == 0xfa21U) != mirrored ? offset : -offset));
        }
    }
}

void UpdatePlayerDeltaSpeedRatio(PlayerModelState &state) {
    const std::uint32_t movementMode = state.rootYawState;
    const float motionScale = (movementMode & 0xfffffff6U) != 0 || movementMode == 8U ? state.forwardProbeDistance : state.desiredYawProbeDistance;
    const float lowerScale = (state.speed - 1.0f) * 0.2f + 0.80000001f;
    state.movementSpeedRatio = std::min(1.0f, std::max(0.0f, (motionScale - lowerScale) / (5.0f - lowerScale)));
}

void UpdatePlayerInterpolation(PlayerModelState &state) {
    PlayerMovementState *const movement = state.movementState;
    if (!movement)
        return;

    float movementBlend = state.normalizedMotionInput;
    if (movementBlend != state.motionInputEcho)
        movementBlend = state.motionInputEcho;
    if ((state.flags & 0x40000000U) == 0 && (state.flags & 0x00020000U) != 0) {
        movementBlend = 0.0f;
    }
    const std::uint32_t movementMode = state.rootYawState;
    if (movementMode == 0)
        movementBlend = 0.0f;

    float motionScale = state.forwardProbeDistance;
    if ((movementMode & 0xfffffff6U) == 0 && movementMode != 8U) {
        motionScale = state.desiredYawProbeDistance;
    }
    if (state.limitingCollisionDistance < 0.80000001f && std::fabs(motionScale - state.limitingCollisionDistance) < 0.75f) {
        motionScale *= 0.5f;
    }

    const std::int32_t yawDifference = static_cast<std::int32_t>(state.collisionBoundaryYaw) - state.currentYaw;
    const std::int32_t yawDistance = std::clamp<std::int32_t>(static_cast<std::int16_t>(yawDifference < 0 ? -yawDifference : yawDifference), 0, 0x4000);
    const float targetScale = motionScale * (sin_fixed16(static_cast<std::int16_t>(yawDistance)) * 0.75f + 1.0f);
    const float upperScale = (state.movementSelectionFlags & 0x2000U) ? 1.0f : 5.0f;

    float normalized = 1.0f;
    if (upperScale > targetScale) {
        const float lower = (state.speed - 1.0f) * 0.2f + 0.80000001f;
        normalized = std::clamp((targetScale - lower) / (5.0f - lower), 0.0f, 1.0f);
    }
    const float bounded = std::min(state.movementBlendLimit, std::max(state.movementSpeedRatio, normalized));
    state.movementBlendOutput = bounded;
    float target = std::min(bounded, movementBlend);
    const float current = state.interpolationValue;
    if (current <= target) {
        if (target > current) {
            const float step = std::min(target - current, 0.40000001f);
            const float rate = ((movementMode - 2U) & 0xfffffffbu) != 0 ? 0.125f : 0.5f;
            target = Lerp(current + step, current, ScaledDelta(rate));
            if (target > 0.99000001f)
                target = 1.0f;
        }
    } else {
        const float rate = (state.flags & 0x00040000U) ? 0.34999999f : 0.2f;
        target = Lerp(target, current, ScaledDelta(rate));
        if (target < 0.0099999998f)
            target = 0.0f;
    }
    if (state.padFlags & 0x800U)
        target = 0.0f;
    if (state.flags & 0x02000000U) {
        state.interpolationValue = 0.0f;
        movement->field04c = 0.0f;
    } else {
        state.interpolationValue = target;
        movement->field04c = target;
    }
}
#endif

} // namespace player_detail

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t RequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested) {
    MotionWork *const work = movement.motionWork;
    if (!work || !movement.character)
        return -1;
    const float frameCount = static_cast<float>(GetMotionFrameCount(mtwkGetAnimationId(work, MotionRegion::LowerBody)));
    float frame = mtwkGetFrame(work, MotionRegion::LowerBody);
    if (mtwkSetRegionFlag(work, 68U, 1, MotionRegion::LowerBody) && work->runtime.currentRootMode != 2U && work->runtime.currentRootMode != 3U) {
        const float current = work->runtime.layerSampleFrame[0];
        if (current >= 0.0f)
            frame = current;
    }
    if (requested == -4)
        requested = 308;
    else if (requested == -3)
        requested = -4;
    else if (requested == -2)
        requested = 0xf003;
    else if (requested == -1)
        requested = -2;
    if (!CanRequestPlayerMotion(movement, requested, frame, frameCount))
        return -1;
    std::int16_t requestedYaw = 0;
    std::int16_t authoredYaw = 0;
    bool mirrored = false;
    const auto delta = static_cast<std::int16_t>(movement.targetYaw - movement.currentYaw);
    if ((movement.rootYawMode == 0U || movement.rootYawMode == 10U) && (requested == -2 || requested == -4) || movement.rootYawMode == 2U && requested == -4) {
        constexpr std::uint16_t motions[] = {0xf002, 0xf005, 0xf005, 0xf02c, 0xf02d, 0xf02e, 0xf02f, 0xf032, 0xf031, 0xf030, 0xf020, 0xf021, 0xf022, 0xf023,
            0xf024, 0xf029, 0xf028, 0xf027, 0xf026, 0xf025, 0xf024, 0xf034};
        constexpr std::uint16_t yaws[] = {
            0, 0, 0, 8192, 16384, 24576, 32768, 40960, 49152, 57344, 4096, 8192, 16384, 24576, 32768, 32768, 40960, 49152, 57344, 61440, 32768, 32768};
        constexpr std::uint8_t walk[] = {2, 3, 4, 5, 6, 7, 8, 9};
        constexpr std::uint8_t start[] = {0, 11, 12, 13, 20, 16, 17, 18};
        const std::int32_t bias = requested == -2 && delta <= 0 ? 4095 : 4096;
        const auto sector = static_cast<std::uint16_t>(delta + bias) >> 13U;
        std::size_t index;
        if (movement.rootYawMode == 2U)
            index = sector == 4U ? 21U : 1U;
        else if (requested == -4 && sector == 0U)
            index = delta >= 0 ? 10U : 19U;
        else
            index = requested == -2 ? walk[sector] : start[sector];
        if (index == 20U)
            index = movement.leadFootDirection < 0 ? 15U : 14U;
        requested = motions[index];
        requestedYaw = delta;
        authoredYaw = static_cast<std::int16_t>(yaws[index]);
    }
    if (requested == 0xf003)
        requested = SelectPlayerIdleMotion(movement, mirrored);
    if (requested == -2)
        requested = 0xf005;
    if (requested < 0)
        return requested;
    if (requested == mtwkGetAnimationId(work, MotionRegion::LowerBody) && mtwkSetRegionFlag(work, 66U, 1, MotionRegion::LowerBody))
        return -1;
    ApplyPlayerMotionRequest(movement, static_cast<std::uint32_t>(requested), frame, frameCount, mirrored, requestedYaw, authoredYaw);
    return requested;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
Vector3 GetPlayerTargetWorldPosition() {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    Vector3 position = {0.0f, 0.0f, 0.0f};
    if (g_playerRuntimePointer->model)
        GetPlayerTargetWorldPosition(&position);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return {};
    const PlayerMovementState &movement = state->embeddedMovementState;
    Vector3 position = GetCharacterWorldPosition(state->characterNode);
    if ((movement.flags & 8U) == 0)
#endif
    return position;

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const auto targetYaw = static_cast<std::uint16_t>(movement.targetWorldYaw);
    const float sine = sin_fixed16(movement.targetWorldYaw);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(targetYaw)));
    const float localZ = (position.x - movement.targetWorldPosition.x) * sine + (position.z - movement.targetWorldPosition.z) * cosine;
    float height = std::max(0.0f, -localZ * 0.74999994f);
    height = std::min(height, static_cast<float>(movement.targetHeightScale) * 0.22499999f);
    position.y = movement.targetWorldPosition.y + height;
    return position;
#endif
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
std::int32_t plymIntersectionTest() {
    Vector3 position;
    GetPlayerTargetWorldPosition(&position);
    position.y += 0.5f;
    return plymDoIntersectionTest(&position);
}


// @verified - d
bool plymCanEvaluateSearchTargets() {
    const PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    const PlayerModelState *const model = runtime->model;
    if (!model)
        return false;
    if (!(runtime->flags & 2U))
        return false;
    if ((model->flags & 0x100U) && model->heldStateMode)
        return false;
    if (model->padFlags & 0x800U)
        return false;
    if (!model->motion)
        return false;
    const std::uint16_t motion = mtwkGetAnimationId(model->motion, static_cast<MotionRegion>(0));
    if (motion == 0xf024U || motion == 0xf029U)
        return false;
    if (runtime->flags & 0x40000U)
        return false;
    return true;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void InitializePlayerMovementState(PlayerMovementState &movement) {
    movement.field15c = 1.0f;
    movement.flags = 0;
    movement.character = nullptr;
    movement.motionWork = nullptr;
    movement.requestedMode = 0;
    movement.currentYaw = 0;
    movement.targetYaw = 0;
    movement.playerMotionId = 0;
    movement.pendingMotionRequest = 0;
    movement.rootYawMode = 0;
    movement.pendingPositionCorrection = 0.0f;
    std::memset(movement.reserved050, 0, sizeof(movement.reserved050));
    movement.field160 = 1.0f;
    movement.yawInterpolationRate = 1.0f;
}

void UpdatePlayerMovementState(PlayerMovementState &movement, bool) {
    MotionWork *const work = movement.motionWork;
    if (!work)
        return;
    if (movement.flags & 0x700U)
        movement.field05e = movement.field05d;
    movement.field05d = movement.rootYawMode;
    movement.rootYawMode = ReachedMotionRootMode(mtwkGetAnimationId(work, MotionRegion::LowerBody));
    const std::uint8_t mode = movement.rootYawMode;
    if (static_cast<std::uint8_t>(mode - 1U) <= 2U || static_cast<std::uint8_t>(mode - 5U) <= 2U) {
        if (movingFrames < 10000)
            ++movingFrames;
        stationaryFrames = 0;
        if (movement.movingFrames < 10000)
            ++movement.movingFrames;
        movement.stationaryFrames = 0;
    } else {
        movingFrames = 0;
        if (stationaryFrames < 10000)
            ++stationaryFrames;
        movement.movingFrames = 0;
        if (movement.stationaryFrames < 10000)
            ++movement.stationaryFrames;
    }
    if (((mode - 4U) & 0xfaU) != 0U || mode == 5U) {
        turningFrames = 0;
        movement.turningFrames = 0;
    } else {
        if (turningFrames < 10000)
            ++turningFrames;
        if (movement.turningFrames < 10000)
            ++movement.turningFrames;
    }
    movement.flags &= 0xffe738fbU;
    movement.pendingYawCorrection = 0;
    movement.motionAnimationId = mtwkGetAnimationId(work, MotionRegion::LowerBody);
    movement.leadFootDirection = 0;
    movement.yawInterpolationRate = 1.0f;
    movement.field15c = 1.0f;
    movement.field16c = 1.0f;
    if (mtwkSetRegionFlag(work, 65U, 1, MotionRegion::LowerBody)) {
        movement.flags |= 0x400U;
    }
    movement.playerMotionFrame = mtwkGetFrame(work, MotionRegion::LowerBody);
    Vector3 terminalRoot;
    SampleNativeMotionRoot(movement.motionAnimationId, mtwkGetRigBoneId(work), -1.0f, terminalRoot);
    movement.rootMotionStep = std::fabs(terminalRoot.z) / static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(movement.motionAnimationId)) - 1);
    movement.movementResult = 0.0f;
    movement.motionSpeed = movement.rootMotionStep * movement.field160;
    ProcessPlayerMotionFlagCursor(movement);
    if (movement.motionTransitionParameter && !mtwkSetRegionFlag(work, 68U, 1, MotionRegion::LowerBody)) {
        const std::int16_t rate = static_cast<std::int16_t>(
            static_cast<std::int32_t>(movement.motionTransitionParameter / (mtwkGetEndFrame(work, MotionRegion::LowerBody) - movement.motionStartFrame)));
        const std::int32_t correction = static_cast<std::int32_t>(rate * (movement.previousField15c * movement.field160));
        movement.pendingYawCorrection = static_cast<std::int16_t>(movement.pendingYawCorrection + correction);
        std::int16_t remainder = static_cast<std::int16_t>(movement.rootYawOffset - correction);
        if ((movement.rootYawOffset > 0 && remainder < 0) || (movement.rootYawOffset < 0 && remainder > 0))
            remainder = 0;
        movement.rootYawOffset = remainder;
    }
    if (mtwkSetRegionFlag(work, 66U, 1, MotionRegion::LowerBody) && (movement.flags & 0x400U)) {
        movement.playerMotionFlagCursor = movement.playerMotionFlagCommands;
        movement.rootYawOffset = 0;
        movement.motionTransitionParameter = 0;
    }
    mtwkGetAnimationId(work, MotionRegion::LowerBody);
}

void plymGetControllerVerticalAxis(float *vertical, float *verticalSecond) {
    *verticalSecond = 0.0f;
    *vertical = 0.0f;
    if (const PlayerModelState *const state = g_playerRuntime.model) {
        *vertical = state->embeddedMovementState.controllerVerticalOffset[0];
        *verticalSecond = state->embeddedMovementState.controllerVerticalOffset[1];
    }
}

void plymGetControllerGroundBlendedXZ(float *x, float *z) {
    *z = 0.0f;
    *x = 0.0f;
    if (const PlayerModelState *const state = g_playerRuntime.model) {
        *x = state->embeddedMovementState.controllerGroundBlend[0];
        *z = state->embeddedMovementState.controllerGroundBlend[1];
    }
}

void plymGetControllerDeltaXZ(Vector3 *first, Vector3 *second) {
    if (const PlayerModelState *const state = g_playerRuntime.model) {
        *first = state->embeddedMovementState.controllerDelta[0];
        *second = state->embeddedMovementState.controllerDelta[1];
    }
}

float plymGetSpeedZ() {
    const PlayerModelState *const state = g_playerRuntime.model;
    if (state && state->rootYawState != 0 && state->rootYawState != 9) {
        return state->speed;
    }
    return 0.0f;
}
#endif

namespace model_component_detail {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
float Vector3_ComputeSquaredDistance(const Vector3 &left, const Vector3 &right);
#endif

} // namespace model_component_detail

namespace player_detail {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
std::uint8_t ReachedMotionRootMode(std::uint32_t motionId);
float ReachedMotionSpeed(std::uint32_t motionId);
std::int16_t movingFrames, stationaryFrames, turningFrames;

// @verified - d
void UpdatePlayerMotionCounters(PlayerMovementState &movement) {
    if (!movement.motion)
        return;
    movement.rootYawMode = ReachedMotionRootMode(mtwkGetAnimationId(movement.motion, static_cast<MotionRegion>(0)));
    switch (movement.rootYawMode) {
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
        if (movingFrames < 10000)
            ++movingFrames;
        stationaryFrames = 0;
        if (movement.movingFrames < 10000)
            ++movement.movingFrames;
        movement.stationaryFrames = 0;
        break;
    default:
        movingFrames = 0;
        if (stationaryFrames < 10000)
            ++stationaryFrames;
        movement.movingFrames = 0;
        if (movement.stationaryFrames < 10000)
            ++movement.stationaryFrames;
        break;
    }
    switch (static_cast<std::int32_t>(movement.rootYawMode)) {
    case 4:
    case 8:
    case 9:
        if (turningFrames < 10000)
            ++turningFrames;
        if (movement.turningFrames < 10000)
            ++movement.turningFrames;
        break;
    default:
        turningFrames = 0;
        movement.turningFrames = 0;
        break;
    }
}


// @verified - d
void ProcessPlayerMotionFlagCursor(PlayerMovementState &movement) {
    const std::byte *cursor = movement.playerMotionFlagCursor;
    if (!cursor)
        return;
    std::uint8_t opcode;
    while ((opcode = static_cast<std::uint8_t>(cursor[2])) != 0 &&
        !(static_cast<float>(static_cast<std::uint8_t>(cursor[0])) + static_cast<float>(static_cast<std::uint8_t>(cursor[1])) * 256.0f >
            movement.playerMotionFrame)) {
        if (opcode == 52) {
            const std::uint32_t flags = *reinterpret_cast<const std::uint32_t *>(cursor + 4);
            const std::uint32_t value = flags & 0x9fffffffU;
            if (flags & 0x40000000U)
                movement.playerMotionFlags |= value;
            if (flags & 0x20000000U)
                movement.playerMotionFlags &= ~value;
        }
        const std::uint8_t *widths = motionFrameCommandWidths;
        cursor += widths[opcode];
    }
    movement.playerMotionFlagCursor = cursor;
}
#endif

} // namespace player_detail

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
 // namespace player_detail

// @verified - d
std::int32_t plymGetMovingFrames() {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    return player_detail::movingFrames;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return movingFrames;
#endif
}


// @verified - d
std::int32_t plymGetTurningFrames() {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    return player_detail::turningFrames;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return turningFrames;
#endif
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
void UpdatePlayerMovementState(PlayerMovementState &movement) {
    MotionWork *const motion = movement.motion;
    if (!motion)
        return;
    player_detail::UpdatePlayerMotionCounters(movement);
    movement.flags &= ~0x700U;
    movement.flags &= ~0x4U;
    movement.flags &= ~0xc000U;
    movement.flags &= ~0x80000U;
    movement.flags &= ~0x100000U;
    movement.pendingYawCorrection = 0;
    movement.motionAnimationId = mtwkGetAnimationId(motion, static_cast<MotionRegion>(0));
    movement.firstScale = 1.0f;
    movement.verticalMotionScale = 1.0f;
    if (mtwkSetRegionFlag(motion, 65, 1, static_cast<MotionRegion>(0)))
        movement.flags |= 0x400U;
    movement.playerMotionFrame = mtwkGetFrame(motion, static_cast<MotionRegion>(0));
    Vector3 root;
    SampleNativeMotionRoot(movement.motionAnimationId, mtwkGetRigBoneId(motion), -1.0f, &root);
    movement.rootMotionStep = (root.z < 0.0f ? -root.z : root.z) / static_cast<float>(GetMotionFrameCount(movement.motionAnimationId) - 1);
    movement.movementResult = 0.0f;
    movement.motionSpeed = movement.rootMotionStep * movement.secondScale;
    player_detail::ProcessPlayerMotionFlagCursor(movement);
    if (movement.motionTransitionParameter && !mtwkSetRegionFlag(motion, 68, 1, static_cast<MotionRegion>(0))) {
        std::int16_t rest = movement.rootYawOffset;
        const float remaining = mtwkGetEndFrame(motion, static_cast<MotionRegion>(0)) - movement.motionStartFrame;
        const std::int16_t rate = static_cast<std::int16_t>(static_cast<std::int32_t>(movement.motionTransitionParameter / remaining));
        const float scaled = rate * (movement.secondScale * movement.previousFirstScale);
        std::int32_t correction;
        movement.pendingYawCorrection += correction = static_cast<std::int32_t>(scaled);
        rest -= correction;
        if (movement.rootYawOffset > 0) {
            if (rest < 0)
                rest = 0;
        } else if (movement.rootYawOffset < 0 && rest > 0)
            rest = 0;
        movement.rootYawOffset = rest;
    }
    if (mtwkSetRegionFlag(motion, 66, 1, static_cast<MotionRegion>(0)) && (movement.flags & 0x400U)) {
        movement.playerMotionFlagCursor = movement.playerMotionFlagCommands;
        if (movement.rootYawOffset)
            movement.rootYawOffset = 0;
        movement.motionTransitionParameter = 0;
    }
    mtwkGetAnimationId(motion, static_cast<MotionRegion>(0));
}


void GetPlayerTargetWorldPosition(Vector3 *position);

std::int16_t CameraLerpAngle(std::int32_t target, std::int32_t base, float amount);
#endif

namespace player_detail {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
std::int16_t camCtrlGetPitchYawWithOffsetsFrom(const PlayerModelState &state);
void plymAdjustControllerGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *work, void *user);

// @verified - d
void plymAdjustControllerGroundTarget(std::uint32_t controller, Vector3 *position, MotionWork *work, void *) {
    PlayerMovementState *const movement = reinterpret_cast<PlayerMovementState *>(&g_playerRuntimePointer->model->retainedSecondaryController);
    PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!(g_playerRuntimePointer->flags & 4U) || (model->retainedPlacementSelector & 1U) == 0)
        return;
    if (controller == 6 && !(model->retainedPlacementSelector & 2U))
        return;
    if (controller == 5 && !(model->retainedPlacementSelector & 4U))
        return;
    const std::uint32_t animation = mtwkGetAnimationId(work, static_cast<MotionRegion>(0));
    if (animation == 0xf02b || animation == 0xf009 || animation == 0xf02a || animation == 0xf008 || animation - 0xf12eU <= 1U)
        return;
    Vector3 probe;
    Vector3 root;
    Vector3 local;
    Vector3 location;
    mtwkGetRootPosition(work, &root);
    probe.x = position->x;
    probe.y = position->y - 0.117f + 1.5f;
    probe.z = position->z;
    float height = FindFieldIntersectionHeight(&probe);
    if (height == -FLT_MAX)
        height = root.y;
    height -= root.y;
    if (0.0f > height)
        height = 0.0f;
    float offset = position->y - 0.117f - root.y;
    if (controller == 6)
        movement->controllerVerticalOffset[0] = offset;
    else
        movement->controllerVerticalOffset[1] = offset;
    if (height > 0.1f)
        offset *= 0.0f > (0.5f - height) / 0.40000001f ? 0.0f : (0.5f - height) / 0.40000001f;
    float blend;
    if (controller == 6)
        blend = movement->controllerGroundBlend[0] = InterpolateTo(height, movement->controllerGroundBlend[0], 0.75f);
    else
        blend = movement->controllerGroundBlend[1] = InterpolateTo(height, movement->controllerGroundBlend[1], 0.75f);
    const float y = blend + root.y + 0.117f + offset;
    position->y = y;
    GetCharacterWorldPosition(movement->character, &location);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-GetCharacterWorldYaw(movement->character));
    matrix_stack::TranslateInverseVector(&location);
    Matrix4x4::TransformPoint(*position, local);
    PopCurrentMatrix(1);
    if (controller == 6) {
        am2Vector3Subtract(&movement->controllerDelta[0], &local, &movement->controllerPreviousLocal[0]);
        movement->controllerPreviousLocal[0] = local;
    } else {
        am2Vector3Subtract(&movement->controllerDelta[1], &local, &movement->controllerPreviousLocal[1]);
        movement->controllerPreviousLocal[1] = local;
    }
}


// @verified - d
void plymInitializeMotionBinding(PlayerMovementState &movement) {
    MotionWork *const motion = movement.motion;
    if (!motion)
        return;
    mtwkSetExtraFlag(motion, 55, 0);
    mtwkAssignTargetCallback(motion, plymAdjustControllerGroundTarget, 25);
    mtwkAssignTargetCallback(motion, plymAdjustControllerGroundTarget, 30);
    mtwkSetTimeRelated(motion, 1, 0, 0xf002);
    mtwkSetTimeRelated(motion, 1, 1, 0xf005);
    mtwkSetTimeRelated(motion, 1, 4, 0xf006);
    PlayerMotionRequest request;
    Vector3 first, second, location;
    GetCharacterWorldPosition(movement.character, &location);
    mtwkGetControllerTargetPosition(motion, 6, first);
    mtwkGetControllerTargetPosition(motion, 5, second);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-GetCharacterWorldYaw(movement.character));
    matrix_stack::TranslateInverseVector(&location);
    Matrix4x4::TransformPoint(first, movement.controllerPreviousLocal[0]);
    Matrix4x4::TransformPoint(second, movement.controllerPreviousLocal[1]);
    PopCurrentMatrix(1);
    const std::uint32_t animation = ChooseRestingPlayerMotion(&movement, reinterpret_cast<PlayerMotionSelection *>(&request));
    mtwkSetRequestedRootMode(motion, 1);
    mtwkRequestMotion(motion, animation, static_cast<MotionRegion>(0));
}


// @verified - d
float mtwkSampleCurrentRemainingTime(const PlayerMovementState &movement) {
    Vector3 root;
    SampleNativeMotionRoot(0xf005, mtwkGetRigBoneId(movement.motion), -1.0f, &root);
    const float frames = static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(0xf005)) - 1);
    float distance = (0.0f > root.z ? -root.z : root.z) / frames;
    distance *= ReachedMotionSpeed(0xf005);
    return distance;
}


// @verified - d
void ClearPlayerPlacementHistory(PlayerModelState &state) {
    state.placementHistoryDistanceAccumulator = 0.0f;
    state.placementHistoryWriteIndex = 0;
    state.placementHistoryCount = 0;
}


// @verified - d
void plymUpdateLockOnCharacter(PlayerModelState &state) {
    const std::int32_t camera = camCtrlGetState();
    const std::uint32_t previous = state.lockOnCharacterName;
    state.lockOnCharacterName = 0;
    const std::uint32_t target = srchCheckFlags(6);
    if (ctrlGetScenarioChara(target)) {
        state.lockOnCharacterName = target;
        if (previous != target)
            ClearPlayerPlacementHistory(state);
    }
    if (!(state.flags & 0x8000000U)) {
        if (camera != 24 || !target || !npcGetCanInteract(target))
            return;
        state.flags |= 0x8000000U;
    } else {
        if (camera != 24)
            state.flags &= ~0x8000000U;
        if (target && npcGetCanInteract(target))
            return;
        state.flags &= ~0x8000000U;
    }
}


// @verified - d
void UpdatePlayerIntersectionFlag(PlayerModelState &) {
    Vector3 position;
    GetPlayerTargetWorldPosition(&position);
    position.y += 0.5f;
    plymRemoveFlags(0x20000U);
    if (plymDoIntersectionTest(&position))
        plymAddFlags(0x20000U);
}


static void plymUpdateSinMotionCounter(PlayerModelState &state);

// @verified - d
void SynchronizePlayerModelState(Character *character, PlayerModelState &state) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerMovementState *const embedded = reinterpret_cast<PlayerMovementState *>(&state.retainedSecondaryController);
    MotionWork *const motion = charFindMotionwork(character);
    if (motion && !state.motion) {
        state.motion = motion;
        embedded->motion = motion;
        embedded->character = character;
        plymInitializeMotionBinding(*embedded);
        state.remainingMotionTime = mtwkSampleCurrentRemainingTime(*embedded);
    }
    if (!(state.padFlags & 2U) && motion && mtwkGetAnimationId(motion, static_cast<MotionRegion>(0)))
        state.padFlags |= 2U;
    state.character = character;
    state.motion = motion;
    embedded->character = character;
    embedded->motion = motion;
    UpdatePlayerMovementState(*state.movementState);
    state.previousRootYawState = state.motionState;
    state.motionState = embedded->rootYawMode;
    float speed = InterpolateTo(embedded->rootMotionStep / 0.041097f, state.speedZ, 0.5f);
    speed = (speed < 1.0f ? 1.0f : speed) < 2.0f ? (speed < 1.0f ? 1.0f : speed) : 2.0f;
    state.speedZ = speed;
    GetCharacterWorldPosition(character, &state.searchLocation);
    state.currentYaw = static_cast<std::int16_t>(GetCharacterWorldYaw(character));
    const std::int16_t pitch = camCtrlGetPitchYawWithOffsetsFrom(state);
    float height = 0.40000001f;
    if (pitch > 0) {
        std::int32_t lower = 910, upper = 6711;
        if (pitch > lower) {
            float ratio = static_cast<float>(pitch - lower) / static_cast<float>(upper - lower);
            ratio = (ratio < 0.0f ? 0.0f : ratio) < 1.0f ? (ratio < 0.0f ? 0.0f : ratio) : 1.0f;
            height -= 0.2f * ratio;
        }
    } else {
        std::int32_t lower = 910, upper = 6711;
        if (pitch < -lower) {
            float ratio = static_cast<float>(-pitch - lower) / static_cast<float>(upper - lower);
            ratio = (ratio < 0.0f ? 0.0f : ratio) < 1.0f ? (ratio < 0.0f ? 0.0f : ratio) : 1.0f;
            height += ratio * 0.20000002f;
        }
    }
    GetPlayerTargetWorldPosition(&state.worldPosition);
    state.worldPosition.y += height;
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&state.worldPosition);
    Matrix4x4::RotateAroundY_Wrapper(state.currentYaw);
    matrix_stack::GetMatrix(&state.worldTransform);
    matrix_stack::UnitCurrentMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-state.currentYaw);
    matrix_stack::TranslateInverseVector(&state.worldPosition);
    matrix_stack::GetMatrix(&state.inverseWorldTransform);
    PopCurrentMatrix(1);
    state.movementQueryState = 0;
    state.retained50a = 0;
    state.previousPadFlags = state.padFlags;
    state.padFlags &= 0xff7fffffU;
    if (runtime->flags & 0x1000000U) {
        runtime->flags &= ~0x1000000U;
        state.padFlags |= 0x800000U;
    }
    if (GetS2ControlMode())
        runtime->flags |= 8U;
    else
        runtime->flags &= ~8U;
    std::uint32_t inherited = 0x36c6453aU;
    state.flags &= ~inherited;
    state.flags |= runtime->flags & inherited;
    runtime->flags &= ~0x1000000U;
    runtime->flags &= ~0x20000000U;
    if (!(state.flags & 2U)) {
        if (state.motion && mtwkSetExtraFlag(state.motion, 58, 1)) {
            state.yaw = s2::CameraLerpAngle(mtwkGetEntityYaw(state.motion), state.yaw, 0.050000001f);
        } else {
            state.yaw = state.currentYaw;
        }
    }
    if (!(state.flags & 2U)) {
        if (!(state.flags & 0x4000U)) {
            state.movementSelectionFlags = 0;
            state.inactiveDirection164 = 0;
            state.inactiveDirection184 = 0;
            state.inactiveDirection17e = 0;
        }
    }
    plymUpdateSinMotionCounter(state);
    UpdatePlayerPartyCompanions(&state);
    plymUpdateLockOnCharacter(state);
    UpdatePlayerIntersectionFlag(state);
}


// @verified - d
static void plymUpdateSinMotionCounter(PlayerModelState &state) {
    if (g_playerRuntimePointer->flags & 0x40000U) {
        if (static_cast<std::int8_t>(state.motionInputState) == 10)
            plymRequestSpecialModeTarget(SHEN_NAME(SIN_));
    } else {
        ++state.specialModeFrame;
    }
}


std::int32_t flddRaycastScene(PlayerModelState &state, const Vector3 &origin, const Vector3 &destination);

bool plymIsInteractionPlacementBlocked(const PlayerModelState &state);
#endif

} // namespace player_detail

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
 // namespace player_detail

void PlayerModuleNullsubA(void *, float);
void PlayerModuleNullsubB(void *, std::int16_t);
void PlayerModuleNullsubC(std::uint32_t);

// @verified - d
void plymSetRequestedSceneContext(std::int16_t minute, std::int16_t act, shen_name area, std::int16_t entry) {
    PlayerModelState *model = g_playerRuntimePointer->model;
    model->scheduledTransitionMinute = minute;
    model->requestedActId = act;
    model->requestedAreaId = area;
    model->requestedEntryId = entry;
}


// @verified - d
std::int32_t plymIsAuthorTargetActive() {
    return plymEvaluateAuthorTargetRequest();
}


// @verified - d
std::int32_t plymIsMotionInputState(std::int32_t state) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (model && (runtime->flags & 0x4000U) && static_cast<std::int8_t>(model->motionInputState) == state)
        return 1;
    return 0;
}


// @verified - d
std::int32_t plymExchangeExternalMovementState(std::int32_t value) {
    PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model)
        return -1;
    const std::int8_t previous = static_cast<std::int8_t>(model->useExternalMovementState);
    model->useExternalMovementState = value;
    return previous;
}


// @verified - d
std::int32_t EV_plymSetSceneContext(std::int16_t startMinute, std::int16_t endMinute, std::int16_t act, shen_name area, std::int16_t entry) {
    if (!g_playerRuntimePointer->model)
        return -1;
    plymSetSceneContext(startMinute, endMinute, act, area, entry);
    return 0;
}


// @verified - d
std::int32_t plymSetSpecialModeTargetCharacter(shen_name character) {
    PlayerModelState *model = g_playerRuntimePointer->model;
    model->specialModeTargetCharacter = character;
    return 0;
}


// @verified - d
static std::int32_t plymRequestSpecialModeTarget(shen_name character) {
    PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!(g_playerRuntimePointer->flags & 0x40000U))
        return -1;
    model->specialModeTargetCharacter = character;
    if (!(model->specialModeFlags & 2U))
        model->specialModeState = 5;
    model->specialModeFlags |= 2U;
    return 0;
}


std::int32_t plymCancelSpecialModeTarget() {
    PlayerModelState *model = g_playerRuntimePointer->model;
    std::uint32_t flags = g_playerRuntimePointer->flags;
    if ((flags & 0x40000U) && (model->specialModeFlags & 2U)) {
        model->specialModeFlags &= ~2U;
        return 0;
    }
    return -1;
}


std::int32_t plymIsSpecialModeTargetRequested() {
    const PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    const PlayerModelState *const model = runtime->model;
    if (!(runtime->flags & 0x40000U))
        return 0;
    if (model && (model->specialModeFlags & 2U))
        return 1;
    return 0;
}


// @verified - d
std::int32_t plymEvaluateAuthorTargetRequest() {
    const PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    const PlayerModelState *const model = runtime->model;
    if (!model)
        return 0;
    if (!(runtime->flags & 0x40000U) || !(model->specialModeFlags & 2U))
        return 0;
    if (!(g_playerRuntimePointer->flags & 0x4000U))
        return 0;
    if (!static_cast<std::int8_t>(model->motionInputState))
        return 0;
    if (static_cast<std::int8_t>(model->motionInputState) != 10)
        return 0;
    if (!(model->placementRoot.flags & 0x10000U))
        return 1;
    return 0;
}


// @verified - d
std::int32_t plymIsSpecialModeFlag800() {
    const PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    const PlayerModelState *const model = runtime->model;
    if (!model)
        return 0;
    if (!(runtime->flags & 0x40000U))
        return 0;
    if (!(model->specialModeFlags & 0x800U))
        return 0;
    return 1;
}


// @verified - d
std::int32_t plymIsPlacementBlockedForParty() {
    PlayerModelState *const model = g_playerRuntimePointer->model;
    return model ? player_detail::plymIsInteractionPlacementBlocked(*model) : 0;
}


// @verified - d
static void plymDialogueNullsub(PlayerModelState *) {
}


// @verified - d
static void plymDrawDialoguePlacementDebug(PlayerModelState *state) {
    PlayerPlacementNode *const root = &state->placementRoot;
    PlayerPlacementNode *const scratch = &state->placementScratch;
    do {
        if (static_cast<std::int32_t>(root->flags) >= 0)
            return;
        PlayerModuleNullsubC(0xff0000ffU);
        PlayerModuleNullsubA(&root->position, 0.3f);
        PlayerModuleNullsubC(0xff00ff00U);
        PlayerModuleNullsubB(&root->position, root->targetYaw);
        if (root->next) {
            PlayerModuleNullsubC(0xff00ff00U);
            PlayerModuleNullsubA(&scratch->position, 0.3f);
        }
    } while (0);
}


// @verified - d
void plymSetDialogueMode() {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!model || !model->character || !model->motion || !g_developerDisplayEnabled || static_cast<std::int32_t>(runtime->flags) >= 0)
        return;
    plymDialogueNullsub(model);
    plymDrawDialoguePlacementDebug(model);
    ApplyS2DialogueMode(2);
    if (g_inputBuffer[3].pressedPriority & 4U)
        runtime->field04 ^= 0x40000000;
}
#endif

namespace player_detail {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
std::int32_t UpdatePlayerMountYaw(PlayerModelState &state) {
    std::int16_t yaw = state.yaw;
    MotionWork *const motion = state.motion;
    const float time = mtwkGetFrame(motion, static_cast<MotionRegion>(0));
    std::int32_t frame;
    switch (mtwkGetAnimationId(state.motion, static_cast<MotionRegion>(0))) {
    case 0xfa21:
        frame = static_cast<std::int32_t>(time - 170.0f);
        if (frame < 0)
            frame = 0;
        if (frame > 30)
            frame = 30;
        if (mtwkSetRegionFlag(motion, 70, 1, static_cast<MotionRegion>(0)))
            yaw = static_cast<std::int16_t>(state.currentYaw - frame * 0x4000 / 30);
        else
            yaw = static_cast<std::int16_t>(state.currentYaw + frame * 0x4000 / 30);
        break;
    case 0xfa22:
        frame = static_cast<std::int32_t>(time - 170.0f);
        if (frame < 0)
            frame = 0;
        if (frame > 30)
            frame = 30;
        if (!mtwkSetRegionFlag(motion, 70, 1, static_cast<MotionRegion>(0)))
            yaw = static_cast<std::int16_t>(state.currentYaw - frame * 0x4000 / 30);
        else
            yaw = static_cast<std::int16_t>(state.currentYaw + frame * 0x4000 / 30);
        break;
    }
    state.yaw = yaw;
    return yaw;
}


// @verified - d
void UpdateReachedPlayerYawWriteback(PlayerModelState &state) {
    const PlayerMovementState *const movement = reinterpret_cast<const PlayerMovementState *>(&state.retainedSecondaryController);
    std::int16_t yaw = GetCharacterWorldYaw(movement->character);
    switch (movement->rootYawMode) {
    case 1:
        if (state.padFlags & 0x200U)
            yaw += movement->rootYawOffset;
        else
            yaw = mtwkGetEntityYaw(movement->motion);
        break;
    case 9:
        if (state.padFlags & 0x200U)
            yaw += movement->rootYawOffset;
        else
            yaw = mtwkGetEntityYaw(movement->motion);
        if (state.specialModeFlags & 8U)
            yaw = InverseFixedAtan2(state.specialModeTargetPosition.x - state.searchLocation.x, state.specialModeTargetPosition.z - state.searchLocation.z);
        break;
    case 4:
        yaw = mtwkGetEntityYaw(movement->motion);
        break;
    case 0:
        if (state.padFlags & 0x200U)
            yaw = state.forcedYaw;
        if (movement->dispatchMode == 8)
            yaw = s2::CameraLerpAngle(static_cast<std::int16_t>(mtwkGetEntityYaw(movement->motion)), state.yaw, 0.050000001f);
        break;
    case 2:
    case 6:
        if (movement->rootYawOffset)
            yaw += movement->rootYawOffset;
        break;
    case 10:
        if (movement->dispatchMode == 8)
            yaw = s2::CameraLerpAngle(static_cast<std::int16_t>(mtwkGetEntityYaw(movement->motion)), state.yaw, 0.050000001f);
        break;
    case 11:
    case 12:
        break;
    }
    if (movement->dispatchMode == 8)
        yaw = s2::CameraLerpAngle(static_cast<std::int16_t>(mtwkGetEntityYaw(movement->motion)), state.yaw, 0.050000001f);
    state.yaw = yaw;
    UpdatePlayerMountYaw(state);
}


// @verified - d
float plymProbeSpeedRatio(float distance, float speed) {
    const float lower = (speed - 1.0f) * 0.2f + 0.80000001f;
    float ratio = distance - lower;
    ratio /= 5.0f - lower;
    return (ratio < 0.0f ? 0.0f : ratio) < 1.0f ? (ratio < 0.0f ? 0.0f : ratio) : 1.0f;
}


// @verified - d
void UpdatePlayerDeltaSpeedRatio(PlayerModelState &state) {
    const std::uint32_t mode = static_cast<std::uint32_t>(state.motionState);
    const bool desired = mode < 2 || mode == 9;
    state.movementSpeedRatio = plymProbeSpeedRatio(desired ? state.desiredYawProbeDistance : state.forwardProbeDistance, state.speedZ);
}


// @verified - d
void UpdateTargetWorldMovementGate(PlayerModelState &state) {
    PlayerMovementState *const movement = state.movementState;
    std::uint32_t gate = 0;
    state.padFlags &= ~0x400000U;
    if (!(movement->flags & 8U))
        return;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-movement->targetWorldYaw);
    matrix_stack::TranslateInverseVector(&movement->targetWorldPosition);
    Vector3 local;
    Matrix4x4::TransformPoint(state.searchLocation, local);
    PopCurrentMatrix(1);
    local.z = -local.z;
    if (local.z < 0.0f || static_cast<float>(movement->targetHeightScale - 1) * 0.30000001f < local.z) {
        const std::int16_t delta = static_cast<std::int16_t>(state.desiredYaw - movement->targetWorldYaw);
        if (local.y > static_cast<float>(movement->targetHeightScale) * 0.30000001f * 0.5f) {
            if (delta > 0x4000)
                gate = 0x400000U;
        } else if (delta < 0x4000) {
            gate = 0x400000U;
        }
    } else {
        gate = 0x400000U;
    }
    state.padFlags |= gate;
}


// @verified - d
void UpdatePlayerInterpolation(PlayerModelState &state) {
    PlayerMovementState *const movement = state.movementState;
    float limit = 5.0f;
    float target = state.movementInterpolation;
    if (target != state.motionInputEcho)
        target = state.motionInputEcho;
    if (state.flags & 0x20000U)
        target = 0.0f;
    if (!state.motionState)
        target = 0.0f;
    float distance = state.forwardProbeDistance;
    if (static_cast<std::uint32_t>(state.motionState) < 2U || state.motionState == 9)
        distance = state.desiredYawProbeDistance;
    std::int16_t angle =
        state.collisionBoundaryYaw - state.currentYaw < 0 ? state.currentYaw - state.collisionBoundaryYaw : state.collisionBoundaryYaw - state.currentYaw;
    angle = (angle < 0 ? 0 : angle) < 0x4000 ? (angle < 0 ? 0 : angle) : 0x4000;
    distance *= dc::sdk::SinFixed(angle) * 0.75f + 1.0f;
    if (state.movementSelectionFlags & 0x2000U)
        limit = 1.0f;
    float output = 1.0f;
    if (limit > distance)
        output = plymProbeSpeedRatio(distance, state.speedZ);
    if (state.movementSpeedRatio > output)
        output = state.movementSpeedRatio;
    if (output > state.movementBlendLimit)
        output = state.movementBlendLimit;
    state.movementBlendOutput = output;
    if (target > output)
        target = output;
    if (state.interpolationValue > target) {
        if (state.flags & 0x40000U)
            target = InterpolateTo(target, state.interpolationValue, 0.34999999f);
        else
            target = InterpolateTo(target, state.interpolationValue, 0.2f);
        if (target < 0.0099999998f)
            target = 0.0f;
    } else if (target > state.interpolationValue) {
        float step;
        float rate = 0.5f;
        step = target - state.interpolationValue;
        step = step < 0.40000001f ? step : 0.40000001f;
        if (state.motionState != 2 && state.motionState != 6)
            rate = 0.125f;
        target = InterpolateTo(step + state.interpolationValue, state.interpolationValue, rate);
        if (target > 0.99000001f)
            target = 1.0f;
    }
    if (state.padFlags & 0x800U)
        target = 0.0f;
    if (state.flags & 0x2000000U)
        target = 0.0f;
    state.interpolationValue = target;
    movement->inputScale = target;
}


void plymUpdateCameraInput(PlayerModelState &state);
void UpdatePlayerYawFromInput(PlayerModelState &state);
void plymProbeMovementCollision(PlayerModelState &state);
void plymProbeDesiredYawCollision(PlayerModelState &state);
void UpdatePlayerDeltaSpeedRatio(PlayerModelState &state);
void UpdateTargetWorldMovementGate(PlayerModelState &state);
void plymBuildInteractionPlacementContext(PlayerModelState &state);

// @verified - d
void PreparePlayerMovementContext(PlayerModelState &state) {
    PlayerPlacementNode *const root = &state.placementRoot;
    PlayerPlacementNode *const scratch = &state.placementScratch;
    root->next = 0;
    scratch->next = 0;
    root->flags &= 0x7ffffff7U;
    scratch->flags = (scratch->flags & 0x7ffff7f7U) | 0x800U;
    root->character = state.character;
    if (state.padFlags & 0x40U) {
        state.padFlags &= ~0x40U;
        root->flags |= 0x200U;
    }
    if (!(state.flags & 0x40000U)) {
        const std::int8_t input = static_cast<std::int8_t>(state.motionInputState);
        if (input != 0 && input != 7) {
            if (input == 10) {
                plymBuildInteractionPlacementContext(state);
                root->flags |= 0x80000000U;
            }
        }
        state.activePlacementNode = root;
        state.selectedPlacementNode = root;
    }
}

void plymResolveMovementMode(PlayerModelState &state);
void UpdatePlayerInterpolation(PlayerModelState &state);
void ExecutePlayerMovement(PlayerMovementState &movement);
void plymUpdateLookTarget(PlayerModelState &state);
void UpdateReachedPlayerYawWriteback(PlayerModelState &state);

// @verified - d
void UpdatePlayerMovementCollision(PlayerModelState &state) {
    plymProbeMovementCollision(state);
    plymProbeDesiredYawCollision(state);
    UpdatePlayerDeltaSpeedRatio(state);
    UpdateTargetWorldMovementGate(state);
}


// @verified - d
void UpdatePlayerMovement(PlayerModelState &state) {
    plymUpdateCameraInput(state);
    UpdatePlayerYawFromInput(state);
    UpdatePlayerMovementCollision(state);
    PreparePlayerMovementContext(state);
    plymResolveMovementMode(state);
    UpdatePlayerInterpolation(state);
    ExecutePlayerMovement(*reinterpret_cast<PlayerMovementState *>(&state.retainedSecondaryController));
    plymUpdateLookTarget(state);
    UpdateReachedPlayerYawWriteback(state);
}


// @verified - d
void UpdateExternalPlayerMovement(PlayerModelState &state) {
    plymUpdateCameraInput(state);
    UpdatePlayerYawFromInput(state);
    UpdatePlayerMovementCollision(state);
    PreparePlayerMovementContext(state);
    plymResolveMovementMode(state);
    ExecutePlayerMovement(*state.movementState);
    UpdateReachedPlayerYawWriteback(state);
}


void UpdateReachedRootConsumerAndScale(PlayerMovementState &movement);

// @verified - d
std::int32_t TryRequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested) {
    return RequestPlayerMotion(movement, requested) >= 0;
}


std::int16_t UpdatePlayerTargetYaw(PlayerMovementState &movement) {
    const float rate = (movement.motionScale * 0.050000001f < 0.050000001f ? 0.050000001f : movement.motionScale * 0.050000001f) < 1.0f ?
        (movement.motionScale * 0.050000001f < 0.050000001f ? 0.050000001f : movement.motionScale * 0.050000001f) :
        1.0f;
    s2::CameraLerpAngle(static_cast<std::int16_t>(movement.targetYaw - movement.currentYaw), 0, rate);
    return movement.currentYaw = s2::CameraLerpAngle(movement.targetYaw, movement.currentYaw, rate);
}


// @verified - d
void ResolvePlayerRootYawMode(PlayerMovementState &movement) {
    std::uint32_t mode;
    switch (movement.rootYawMode) {
    case 0:
        mode = 0;
        break;
    case 1:
        mode = 1;
        break;
    case 2:
        mode = 2;
        break;
    case 3:
        mode = 3;
        break;
    case 6:
        mode = 4;
        break;
    case 9:
        mode = 5;
        break;
    case 4:
        mode = 6;
        break;
    default:
        mode = 9;
        break;
    }
    if (movement.flags & 2U)
        mode = 7;
    if (movement.pendingMotionRequest)
        mode = 7;
    if (mtwkSetExtraFlag(movement.motion, 58, 1))
        mode = 8;
    if (movement.flags & 0x20U)
        mode = 8;
    movement.dispatchMode = mode;
}


// @verified - d
void UpdatePlayerIdleRequest(PlayerMovementState &movement) {
    switch (movement.requestedMode) {
    case 3:
    case 4:
        TryRequestPlayerMotion(movement, -1);
        break;
    case 2:
    case 6:
        TryRequestPlayerMotion(movement, -3);
        break;
    case 1:
    default:
        TryRequestPlayerMotion(movement, -2);
        break;
    }
}


// @verified - d
void UpdatePlayerStartRequest(PlayerMovementState &movement) {
    switch (static_cast<std::int32_t>(movement.requestedMode)) {
    case 3:
    case 4:
        TryRequestPlayerMotion(movement, -1);
        break;
    case 1:
    default:
        TryRequestPlayerMotion(movement, -2);
        break;
    }
}


// @verified - d
void UpdatePlayerLocomotionRequest(PlayerMovementState &movement) {
    switch (static_cast<std::int32_t>(movement.requestedMode)) {
    case 1:
        TryRequestPlayerMotion(movement, -2);
        break;
    case 6:
        TryRequestPlayerMotion(movement, -3);
        return;
    case 10:
        if (movement.flags & 0xc000U)
            mtwkCopyTransition(movement.motion, &movement.transitionTargetPosition, static_cast<std::int16_t>(movement.transitionTargetYaw), 0, 0);
        else if (movement.flags & 0x4000U)
            mtwkCopyTransition(movement.motion, &movement.transitionTargetPosition, static_cast<std::int16_t>(movement.transitionTargetYaw), 0, 2);
        else if (movement.flags & 0x8000U)
            mtwkCopyTransition(movement.motion, &movement.transitionTargetPosition, static_cast<std::int16_t>(movement.transitionTargetYaw), 0, 1);
        mtwkCopyTransition(movement.motion, 0, 0, 2, 0);
        break;
    }
    UpdatePlayerTargetYaw(movement);
}


// @verified - d
void UpdatePlayerStopRequest(PlayerMovementState &movement) {
    switch (static_cast<std::int32_t>(movement.requestedMode)) {
    case 1:
        TryRequestPlayerMotion(movement, -2);
        break;
    case 3:
    case 4:
        TryRequestPlayerMotion(movement, -1);
        break;
    }
}


void UpdatePlayerReverseRequest(PlayerMovementState &movement) {
    switch (movement.requestedMode) {
    case 1:
        TryRequestPlayerMotion(movement, -2);
        break;
    case 6:
        TryRequestPlayerMotion(movement, -3);
        return;
    }
    UpdatePlayerTargetYaw(movement);
}


void UpdatePlayerTurnRequest(PlayerMovementState &movement) {
    switch (static_cast<std::int32_t>(movement.requestedMode)) {
    case 3:
    case 4:
        TryRequestPlayerMotion(movement, -1);
        break;
    case 1:
    default:
        TryRequestPlayerMotion(movement, -2);
        break;
    }
}


// @verified - d
void UpdatePlayerDefaultRequest(PlayerMovementState &movement) {
    if (movement.requestedMode == 1)
        TryRequestPlayerMotion(movement, -2);
    else
        TryRequestPlayerMotion(movement, -2);
}


// @verified - d
void UpdatePlayerScriptedMotionRequest(PlayerMovementState &movement) {
    if (static_cast<std::int32_t>(movement.pendingMotionRequest) > 0 && TryRequestPlayerMotion(movement, movement.pendingMotionRequest))
        movement.pendingMotionRequest = 0;
}


// @verified - d
void UpdatePlayerCopyMovementRequest(PlayerMovementState &) {
}


// @verified - d
void ExecutePlayerMovement(PlayerMovementState &movement) {
    Character *const character = movement.character;
    movement.currentYaw = static_cast<std::int16_t>(GetCharacterWorldYaw(character));
    ResolvePlayerRootYawMode(movement);
    switch (movement.dispatchMode) {
    case 0:
        UpdatePlayerIdleRequest(movement);
        break;
    case 1:
        UpdatePlayerStartRequest(movement);
        break;
    case 2:
        UpdatePlayerLocomotionRequest(movement);
        break;
    case 3:
        UpdatePlayerStopRequest(movement);
        break;
    case 4:
        UpdatePlayerReverseRequest(movement);
        break;
    case 5:
    case 6:
        UpdatePlayerTurnRequest(movement);
        break;
    case 7:
        UpdatePlayerScriptedMotionRequest(movement);
        break;
    case 8:
        UpdatePlayerCopyMovementRequest(movement);
        break;
    default:
        UpdatePlayerDefaultRequest(movement);
        break;
    }
    if (movement.dispatchMode - 7U > 1U && (movement.flags & 0x100U))
        movement.previousRequestedMode = movement.requestedMode;
    movement.currentYaw = static_cast<std::int16_t>(movement.currentYaw + movement.pendingYawCorrection);
    movement.pendingYawCorrection = 0;
    SetCharacterWorldYaw(character, movement.currentYaw);
    UpdateReachedRootConsumerAndScale(movement);
    movement.currentYaw = static_cast<std::int16_t>(movement.currentYaw + movement.pendingYawCorrection);
    SetCharacterWorldYaw(character, movement.currentYaw);
}


std::uint32_t RequestPlayerRestingMotion(PlayerMovementState &movement) {
    if (!movement.motion)
        return 0;
    PlayerMotionRequest request;
    const std::uint32_t motion = ChooseRestingPlayerMotion(&movement, reinterpret_cast<PlayerMotionSelection *>(&request));
    mtwkSetRequestedRootMode(movement.motion, 1);
    mtwkRequestMotion(movement.motion, motion, static_cast<MotionRegion>(0));
    return motion;
}

void ConfigurePlayerIdleRequest(PlayerMotionRequest &request);

static std::int32_t GetPlayerGaitRank(std::uint32_t motion);

std::uint32_t CheckPlayerMotionRequest(PlayerMotionRequest &request) {
    std::int32_t current = request.previousMotion;
    std::uint32_t previousClass = request.previousClass;
    std::int32_t requested = request.motion;
    MotionWork *motionWork = request.motionWork;
    float frame = request.previousFrame;
    std::uint32_t flags;
    std::uint32_t tracked = mtwkSetRegionFlag(motionWork, 68, 1, static_cast<MotionRegion>(0));
    if (mtwkSetRegionFlag(motionWork, 65, 1, static_cast<MotionRegion>(0)))
        return 0;
    switch (current) {
    case 0xf005:
        switch (requested) {
        case 0xf006:
        case 0xf035 ... 0xf03d:
            tracked = 0;
        }
        break;
    case 0xfa1a:
    case 0xfa20:
        return 0;
    }
    switch (previousClass) {
    case 2:
    case 6:
        switch (requested) {
        case 0xf008:
        case 0xf009:
        case 0xf02a:
        case 0xf02b:
            return 0;
        }
        if (tracked)
            return 1;
        flags = request.playerMotionFlags;
        if (!(flags & 2U))
            return 0;
        if (flags & 1U)
            return 0;
        return 1;
    case 3:
        if (tracked)
            return 1;
        return 1;
    case 1:
        if (tracked)
            return 1;
        switch (requested) {
        case 0xf006:
        case 0xf035 ... 0xf039:
            break;
        default:
            return 1;
        }
        switch (current) {
        case 0xf02c:
        case 0xf02d:
        case 0xf030:
        case 0xf031:
            if (request.previousFrame > 3.0f)
                return 0;
        }
        return 1;
    case 5:
        if (tracked)
            return 1;
        if (requested == -2)
            return 1;
        if (requested == 0xf003)
            return 1;
        return 0;
    case 0:
        if (tracked)
            return 1;
        if (requested == 0xf003)
            return 1;
        return 0;
    case 4:
        if (tracked)
            return 1;
        if (current == 0xf034)
            return 1;
        return 1;
    case 9:
        if (tracked)
            return 1;
        if (current != 0xf024 && requested != 0xf003 && frame > request.previousFrameCount * 0.5f)
            return 0;
        switch (current) {
        case 0xf020 ... 0xf023:
        case 0xf025 ... 0xf028:
            switch (requested) {
            case -2:
            case 0xf005:
            case 0xf006:
            case 0xf035 ... 0xf03d:
                if (frame > 2.0f)
                    return 0;
            }
        }
        return 1;
    case 10:
        if (tracked)
            return 1;
        if (!mtwkSetRegionFlag(motionWork, 66, 1, static_cast<MotionRegion>(0)))
            return 1;
        return 0;
    }
    return 0;
}


// @verified - d
std::int32_t SelectPlayerWalkRequestIndex(std::int16_t delta) {
    static const std::int32_t walkRequestIndices[8] = {2, 3, 4, 5, 6, 7, 8, 9};
    std::uint16_t sector;
    if (delta > 0)
        sector = static_cast<std::uint16_t>(delta + 4096);
    else
        sector = static_cast<std::uint16_t>(delta + 4095);
    return walkRequestIndices[sector / 4096 / 2];
}


// @verified - d
std::int32_t SelectPlayerTurnRequestIndex(std::int16_t delta) {
    static const std::int32_t turnRequestIndices[8] = {0, 11, 12, 13, 14, 15, 16, 17};
    std::int32_t sector = static_cast<std::uint16_t>(delta) + 0x1000;
    sector = (sector >> 12) / 2 & 7;
    if (!sector) {
        if (delta < 0)
            return 18;
        return 10;
    }
    return turnRequestIndices[sector];
}


// @verified - d
std::int32_t SelectPlayerReverseRequestIndex(std::int16_t delta) {
    const std::int32_t sector = ((static_cast<std::uint16_t>(delta) + 0x1000) >> 12) / 2 & 7;
    if (sector == 4)
        return 19;
    return 1;
}


// @verified - d
std::int32_t SelectPlayerDirectionalMotion(PlayerMotionRequest &request, std::int32_t mode) {
    static const std::uint32_t directionalMotions[20] = {0xf002, 0xf005, 0xf005, 0xf02c, 0xf02d, 0xf02e, 0xf02f, 0xf032, 0xf031, 0xf030, 0xf020, 0xf021, 0xf022,
        0xf023, 0xf024, 0xf028, 0xf027, 0xf026, 0xf025, 0xf034};
    static const std::uint16_t directionalMotionYaws[20] = {
        0, 0, 0, 8192, 16384, 24576, 32768, 40960, 49152, 57344, 4096, 8192, 16384, 24576, 32768, 40960, 49152, 57344, 61440, 32768};
    std::int32_t index = 0;
    std::int16_t delta = request.targetYaw - request.currentYaw;
    switch (mode) {
    case 0:
        index = SelectPlayerWalkRequestIndex(delta);
        break;
    case 1:
        index = SelectPlayerTurnRequestIndex(delta);
        break;
    case 2:
        index = SelectPlayerReverseRequestIndex(delta);
        break;
    }
    request.turnMotion = directionalMotions[index];
    request.authoredYaw = static_cast<std::int16_t>(directionalMotionYaws[index]);
    request.requestedYaw = static_cast<std::int16_t>(delta - request.authoredYaw);
    return request.turnMotion;
}


// @verified - d
std::int32_t SelectPlayerRestingRequest(PlayerMovementState &movement, PlayerMotionRequest &request) {
    PlayerMotionSelection selection;
    const std::uint32_t motion = ChooseRestingPlayerMotion(&movement, &selection);
    request.mirrored = selection.mirrored;
    return motion;
}


// @verified - d
std::int32_t ResolvePlayerRequestedMotion(PlayerMovementState &movement, PlayerMotionRequest &request) {
    const std::int32_t previousClass = static_cast<std::int32_t>(request.previousClass);
    std::int32_t result = request.motion;
    switch (previousClass) {
    case 0:
    case 10:
        if (result == -2)
            result = SelectPlayerDirectionalMotion(request, 0);
        else if (result == -4)
            result = SelectPlayerDirectionalMotion(request, 1);
        break;
    case 2:
        if (result == -4)
            result = SelectPlayerDirectionalMotion(request, 2);
        break;
    }
    if (result == 0xf003)
        result = static_cast<std::int32_t>(ChooseRestingPlayerMotion(&movement, reinterpret_cast<PlayerMotionSelection *>(&request)));
    if (result == -2)
        result = 0xf005;
    return result;
}


// @verified - d
void ConfigurePlayerGaitRequest(PlayerMotionRequest &request) {
    const std::int32_t previous = static_cast<std::int32_t>(request.previousMotion);
    const std::int32_t motion = request.motion;
    float sampleFrame = 0.0f;
    std::int32_t flag = 0;
    switch (previous) {
    case 0xf02c:
    case 0xf02d:
    case 0xf02e:
    case 0xf02f:
        request.blendFrames = 3;
        request.rootMode = 2;
        sampleFrame = request.frameCount * 0.98648649f;
        break;
    case 0xf030:
    case 0xf031:
    case 0xf032:
    case 0xf033:
        request.blendFrames = 3;
        request.rootMode = 2;
        sampleFrame = request.frameCount * 0.47297296f;
        break;
    case 0xf002:
        if (mtwkIsRightFootTargetAhead(request.motionWork))
            request.parentSelector = 1;
        else
            request.parentSelector = -1;
        switch (motion) {
        case 0xf008:
        case 0xf009:
        case 0xf02a:
        case 0xf02b:
        case 0xf12e:
        case 0xf12f:
            sampleFrame = 0.0f;
            request.blendFrames = 2;
            request.rootMode = 2;
            if (mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
                request.mirrored = 1;
            break;
        default:
            if (!mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
                sampleFrame = request.frameCount * 0.027027028f;
            else
                sampleFrame = request.frameCount * 0.51351351f;
            request.blendFrames = 4;
            request.rootMode = 2;
            break;
        }
        break;
    case 0xf004:
        if (request.previousFrame >= 20.0f)
            flag = 1;
        if (mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
            flag ^= 1;
        if (!flag)
            request.parentSelector = 1;
        else
            request.parentSelector = -1;
        switch (motion) {
        case 0xf008:
        case 0xf009:
        case 0xf02a:
        case 0xf02b:
        case 0xf12e:
        case 0xf12f:
            sampleFrame = 0.0f;
            request.blendFrames = 2;
            request.rootMode = 2;
            if (mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
                request.mirrored = 1;
            break;
        default:
            if (!flag)
                sampleFrame = request.frameCount * 0.027027028f;
            else
                sampleFrame = request.frameCount * 0.51351351f;
            request.blendFrames = 4;
            request.rootMode = 2;
            break;
        }
        break;
    case 0xf020:
    case 0xf021:
    case 0xf022:
    case 0xf023:
    case 0xf025:
    case 0xf026:
    case 0xf027:
    case 0xf028:
        sampleFrame = 4.0f;
        sampleFrame += request.previousFraction * sampleFrame;
        request.rootMode = 2;
        break;
    case 0xf005:
    case 0xf006:
    case 0xf035:
    case 0xf036:
    case 0xf037:
    case 0xf038:
    case 0xf039:
    case 0xf03a:
    case 0xf03b:
    case 0xf03c:
    case 0xf03d:
        switch (motion) {
        case 0xf008:
        case 0xf009:
        case 0xf02a:
        case 0xf02b:
        case 0xf12e:
        case 0xf12f:
            sampleFrame = 0.0f;
            request.blendFrames = 4;
            request.rootMode = 2;
            if (request.previousFraction > 0.25f && request.previousFraction < 0.75f)
                request.mirrored = 1;
            break;
        default:
            request.rootMode = 2;
            const std::int32_t previousRank = GetPlayerGaitRank(previous);
            if (GetPlayerGaitRank(motion) > previousRank) {
                request.blendFrames = 3;
                sampleFrame = request.frameCount * request.previousFraction;
                sampleFrame += ReachedMotionSpeed(motion);
            } else {
                request.blendFrames = 3;
                sampleFrame = request.frameCount * request.previousFraction;
                sampleFrame += ReachedMotionSpeed(motion);
            }
            request.rootMode = 0;
            break;
        }
        break;
    case 0xf008:
    case 0xf009:
    case 0xf02a:
    case 0xf02b:
        switch (motion) {
        case 0xf005:
            request.rootMode = 2;
            if (request.previousFraction > 0.25f && request.previousFraction < 0.75f)
                flag = 1;
            if (mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
                flag ^= 1;
            sampleFrame = !flag ? 3.0f : 21.0f;
            break;
        case 0xf006:
            request.rootMode = 2;
            if (request.previousFraction > 0.25f && request.previousFraction < 0.75f)
                flag = 1;
            if (mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
                flag ^= 1;
            sampleFrame = !flag ? 1.0f : 9.0f;
            break;
        case 0xf035:
        case 0xf036:
        case 0xf037:
        case 0xf038:
        case 0xf039:
        case 0xf03a:
        case 0xf03b:
        case 0xf03c:
        case 0xf03d:
            request.rootMode = 2;
            if (request.previousFraction > 0.25f && request.previousFraction < 0.75f)
                flag = 1;
            if (mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
                flag ^= 1;
            if (!flag)
                sampleFrame = request.frameCount / 18.0f;
            else
                sampleFrame = request.frameCount * 9.0f / 18.0f;
            break;
        case 0xf008:
        case 0xf009:
        case 0xf02a:
        case 0xf02b:
        case 0xf12e:
        case 0xf12f:
            request.blendFrames = 4;
            request.rootMode = 2;
            if (request.previousFraction > 0.25f && request.previousFraction < 0.75f)
                flag = 1;
            if (mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
                flag ^= 1;
            request.mirrored = flag;
            break;
        default:
            if (mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
                request.mirrored = 1;
            sampleFrame = 0.0f;
            break;
        }
        break;
    case 0xf12e:
    case 0xf12f:
        if (mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
            flag = 1;
        switch (motion) {
        case 0xf005:
            request.rootMode = 2;
            sampleFrame = flag ? 3.0f : 21.0f;
            break;
        case 0xf006:
            request.rootMode = 2;
            sampleFrame = flag ? 1.0f : 9.0f;
            break;
        case 0xf035:
        case 0xf036:
        case 0xf037:
        case 0xf038:
        case 0xf039:
        case 0xf03a:
        case 0xf03b:
        case 0xf03c:
        case 0xf03d:
            request.rootMode = 2;
            if (flag)
                sampleFrame = request.frameCount / 18.0f;
            else
                sampleFrame = request.frameCount * 9.0f / 18.0f;
            break;
        case 0xf008:
        case 0xf009:
        case 0xf02a:
        case 0xf02b:
            request.blendFrames = 4;
            request.rootMode = 2;
            request.mirrored = flag ^ 1;
            break;
        default:
            request.mirrored = flag ^ 1;
            sampleFrame = 0.0f;
            break;
        }
        break;
    case 0xffff:
        break;
    }
    request.sampleFrame = sampleFrame;
}


// @verified - d
void ConfigurePlayerTurnRequest(PlayerMotionRequest &request) {
    request.rootMode = 2;
    request.blendFrames = 3;
}


// @verified - d
void ConfigurePlayerCycleRequest(PlayerMotionRequest &request) {
    if (request.previousMotion == 0xfa1a || request.previousMotion == 0xfa20) {
        request.blendFrames = 8;
        request.rootMode = 3;
        request.sampleFrame = request.frameCount * request.previousFraction;
    }
    if (mtwkSetRegionFlag(request.motionWork, 70, 1, static_cast<MotionRegion>(0)))
        request.mirrored = 1;
}


// @verified - d
void ConfigurePlayerMotionRequest(PlayerMotionRequest &request) {
    MotionWork *work = request.motionWork;
    std::int32_t motion = request.motion;
    request.rootMode = 1;
    request.blendFrames = 5;
    request.frameCount = static_cast<float>(GetMotionFrameCount(static_cast<std::uint16_t>(motion)));
    switch (motion) {
    case 0xf005:
    case 0xf006:
    case 0xf008:
    case 0xf009:
    case 0xf02a:
    case 0xf02b:
    case 0xf035 ... 0xf03d:
    case 0xf12e:
    case 0xf12f:
        ConfigurePlayerGaitRequest(request);
        break;
    case 0xf014 ... 0xf017:
    case 0xf02c ... 0xf033:
        ConfigurePlayerTurnRequest(request);
        break;
    case 0xf002:
        ConfigurePlayerIdleRequest(request);
        break;
    case 0xfa1a:
    case 0xfa20:
        ConfigurePlayerCycleRequest(request);
        break;
    }
    if (mtwkSetRegionFlag(work, 66, 1, static_cast<MotionRegion>(0))) {
        if (!(request.sampleFrame < request.frameCount))
            request.sampleFrame -= request.frameCount;
        if (request.sampleFrame < 0.0f)
            request.sampleFrame += request.frameCount;
    } else {
        if (!(request.sampleFrame < request.frameCount))
            request.sampleFrame = request.frameCount - 1.0f;
        if (request.sampleFrame < 0.0f)
            request.sampleFrame = 0.0f;
    }
    mtwkSetRegionEvalTime(work, request.sampleFrame, static_cast<MotionRegion>(0));
    mtwkSetRequestedRootMode(work, request.rootMode);
    mtwkSetFrameTime(work, request.blendFrames, static_cast<MotionRegion>(0));
}


// @verified - d
void ResetPlayerMotionFlagCursor(PlayerMovementState &movement) {
    movement.playerMotionFlags = 0;
    movement.playerMotionFlagCommands = 0;
    movement.playerMotionFlagCursor = 0;
    const std::byte *record = MOTN_GetMarkerData(static_cast<std::uint16_t>(movement.playerMotionId));
    if (!record)
        return;
    record += 12;
    while (static_cast<std::uint8_t>(*record) != 8) {
        const std::uint8_t command = static_cast<std::uint8_t>(*record);
        if (!command)
            return;
        record += motionCommandWidths[command];
    }
    record += 4;
    movement.playerMotionFlagCommands = record;
    movement.playerMotionFlagCursor = record;
    ProcessPlayerMotionFlagCursor(movement);
}


// @verified - d
void ApplyPlayerMotionRequestAngles(PlayerMovementState &movement) {
    switch (static_cast<std::int32_t>(movement.playerMotionId)) {
    case 0xfa22:
        movement.rootYawOffset = -16384;
        break;
    case 0xfa0c:
        movement.rootYawOffset = -8192;
        break;
    case 0xfa0d:
        movement.rootYawOffset = -16384;
        break;
    case 0xfa21:
        break;
    case 0xfa09:
        movement.rootYawOffset = 8192;
        break;
    case 0xfa0a:
        movement.rootYawOffset = 16384;
        break;
    case 0xfa08:
        movement.rootYawOffset = static_cast<std::int16_t>(0x8000);
        break;
    case 0xf020:
        mtwkSetLayerEndFrame(movement.motion, 19.0f, static_cast<MotionRegion>(0));
        break;
    case 0xf021:
        mtwkSetLayerEndFrame(movement.motion, 23.0f, static_cast<MotionRegion>(0));
        break;
    case 0xf022:
        mtwkSetLayerEndFrame(movement.motion, 27.0f, static_cast<MotionRegion>(0));
        break;
    case 0xf023:
        mtwkSetLayerEndFrame(movement.motion, 29.0f, static_cast<MotionRegion>(0));
        break;
    case 0xf024:
        mtwkSetLayerEndFrame(movement.motion, 41.0f, static_cast<MotionRegion>(0));
        break;
    case 0xf025:
        mtwkSetLayerEndFrame(movement.motion, 19.0f, static_cast<MotionRegion>(0));
        break;
    case 0xf026:
        mtwkSetLayerEndFrame(movement.motion, 23.0f, static_cast<MotionRegion>(0));
        break;
    case 0xf027:
        mtwkSetLayerEndFrame(movement.motion, 27.0f, static_cast<MotionRegion>(0));
        break;
    case 0xf028:
        mtwkSetLayerEndFrame(movement.motion, 29.0f, static_cast<MotionRegion>(0));
        break;
    case 0xf029:
        mtwkSetLayerEndFrame(movement.motion, 41.0f, static_cast<MotionRegion>(0));
        break;
    }
    movement.flags &= ~0x2000U;
    switch (movement.playerMotionId) {
    case 0xf020:
    case 0xf021:
    case 0xf022:
    case 0xf023:
    case 0xf025:
    case 0xf026:
    case 0xf027:
    case 0xf028:
    case 0xf02c:
    case 0xf02d:
    case 0xf030:
    case 0xf031:
        movement.flags |= 0x2000U;
    }
}
#endif

} // namespace player_detail

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
 // namespace player_detail

// @verified - d
std::int32_t RequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested) {
    using namespace player_detail;
    MotionWork *const work = movement.motion;
    PlayerMotionRequest storage;
    PlayerMotionRequest &request = storage;
    request.previousMotion = mtwkGetAnimationId(work, static_cast<MotionRegion>(0));
    request.previousClass = movement.rootYawMode;
    request.previousFrameCount = static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(static_cast<std::uint16_t>(request.previousMotion))));
    request.previousFrame = mtwkGetFrame(work, static_cast<MotionRegion>(0));
    if (mtwkSetRegionFlag(work, 68, 1, static_cast<MotionRegion>(0))) {
        float frame = request.previousFrame;
        switch (mtwkGetTrackedRootMode(work)) {
        case 2:
        case 3:
            break;
        default:
            frame = mtwkGetRegionTimeOverride(work, 0);
            break;
        }
        if (!(frame < 0.0f))
            request.previousFrame = frame;
    }
    if (std::fabs(request.previousFrameCount) < 0.00000011920929f)
        request.previousFraction = 0.0f;
    else
        request.previousFraction = request.previousFrame / (request.previousFrameCount - 1.0f);
    request.mirrored = 0;
    request.requestedYaw = 0;
    request.blocked = 0;
    request.parentSelector = 0;
    request.stopTranslation = 0;
    request.playbackSpeed = 1.0f;
    request.targetYaw = movement.targetYaw;
    request.currentYaw = movement.currentYaw;
    request.sampleFrame = 0.0f;
    request.blendFrames = 0;
    request.motionWork = work;
    request.character = movement.character;
    request.turnMotion = 0;
    request.authoredYaw = 0;
    request.playerMotionFlags = movement.playerMotionFlags;
    request.flags = 0;
    switch (requested) {
    case -1:
        request.motion = -2;
        break;
    case -2:
        request.motion = 0xf003;
        break;
    case -3:
        request.motion = -4;
        break;
    default:
        request.motion = requested;
        break;
    }
    if (!(movement.flags & 4U))
        request.blocked = CheckPlayerMotionRequest(request);
    if (request.blocked)
        return -1;
    request.motion = ResolvePlayerRequestedMotion(movement, request);
    if (request.motion < 0)
        return request.motion;
    if (request.motion == static_cast<std::int32_t>(request.previousMotion) && mtwkSetRegionFlag(work, 66, 1, static_cast<MotionRegion>(0)))
        return -1;
    movement.playerMotionId = request.motion;
    if (mtwkSetRegionFlag(work, 65, 1, static_cast<MotionRegion>(0)))
        movement.pendingYawCorrection = static_cast<std::int16_t>(movement.pendingYawCorrection + movement.rootYawOffset);
    else if (movement.flags & 0x2000U)
        movement.pendingYawCorrection = static_cast<std::int16_t>(movement.pendingYawCorrection + movement.rootYawOffset);
    else if (movement.motionTransitionParameter)
        movement.pendingYawCorrection = static_cast<std::int16_t>(movement.pendingYawCorrection + movement.rootYawOffset);
    else
        movement.pendingYawCorrection =
            static_cast<std::int16_t>(static_cast<std::int32_t>(request.previousFraction * movement.rootYawOffset) + movement.pendingYawCorrection);
    movement.rootYawOffset = static_cast<std::int16_t>(request.authoredYaw + request.requestedYaw);
    movement.motionEntityYaw = static_cast<std::int16_t>(mtwkGetEntityYaw(work));
    movement.motionNodeYaw = static_cast<std::int16_t>(GetCharacterWorldYaw(movement.character));
    mtwkGetRootPosition(work, &movement.motionRootEntityPosition);
    GetCharacterWorldPosition(movement.character, &movement.motionCharacterPosition);
    ConfigurePlayerMotionRequest(request);
    request.playbackSpeed = ReachedMotionSpeed(request.motion);
    mtwkRequestMotion(work, request.motion, static_cast<MotionRegion>(0));
    mtwkSetSpeed(work, request.playbackSpeed, static_cast<MotionRegion>(0));
    if (request.authoredYaw) {
        mtwkAssignRigYawOffset(work, request.requestedYaw);
        movement.motionTransitionParameter = 0;
        movement.motionStartFrame = request.sampleFrame;
    } else {
        movement.motionTransitionParameter = request.requestedYaw;
        movement.motionStartFrame = request.sampleFrame;
    }
    if (request.mirrored)
        mtwkSetRegionFlag(work, 69, 0, static_cast<MotionRegion>(0));
    else
        mtwkSetRegionFlag(work, 69, 2, static_cast<MotionRegion>(0));
    movement.flags |= 0x900U;
    movement.firstScale = 1.0f;
    movement.secondScale = request.playbackSpeed;
    movement.secondScale = ReachedMotionSpeed(request.motion);
    movement.playerMotionFrame = request.sampleFrame;
    movement.verticalMotionScale = 1.0f;
    ResetPlayerMotionFlagCursor(movement);
    ApplyPlayerMotionRequestAngles(movement);
    const std::uint32_t rootMode = ReachedMotionRootMode(request.motion);
    if (rootMode != 2 && rootMode != 6)
        movement.inputScale = 0.0f;
    movement.rootYawMode = rootMode;
    movement.parentBoneSelector = request.parentSelector;
    if (request.stopTranslation)
        movement.flags |= 0x1000U;
    if (request.flags & 0x100000U)
        movement.flags |= 0x100000U;
    return request.motion;
}
#endif

namespace player_detail {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
void ConfigurePlayerIdleRequest(PlayerMotionRequest &request) {
    switch (request.previousMotion) {
    case 0xf005:
    case 0xf006:
    case 0xf035:
    case 0xf036:
    case 0xf037:
    case 0xf038:
    case 0xf039:
    case 0xf03a:
    case 0xf03b:
    case 0xf03c:
    case 0xf03d: {
        Vector3 position, target, hit, direction, offset;
        do {
            request.rootMode = 1;
            request.sampleFrame = 0.0f;
            request.blendFrames = 12;
        } while (0);
        SampleNativeMotionRoot(static_cast<std::uint16_t>(request.previousMotion), mtwkGetRigBoneId(request.motionWork), -1.0f, &position);
        const float distance = (position.z < 0.0f ? -position.z : position.z) / request.previousFrameCount;
        GetCharacterWorldPosition(request.character, &position);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(request.currentYaw);
        GetTransformedForwardVector(&direction);
        PopCurrentMatrix(1);
        am2Vector3Scale(&offset, &direction, distance * 4.0f);
        am2Vector3Add(&target, &position, &offset);
        const float radius = GetCharacterCollisionRadius(request.character);
        target.y += 0.5f;
        position.y += 0.5f;
        if (FindNearestRayIntersection(&hit, &target, &position)) {
            am2Vector3Scale(&offset, &direction, radius);
            am2Vector3Subtract(&target, &hit, &offset);
            if (model_component_detail::Vector3_ComputeSquaredDistance(hit, position) > 0.16000001f) {
                target.x = position.x;
                target.y = position.y;
                target.z = position.z;
            }
        } else if (RaycastInWorldGrid(&hit, &target, radius)) {
            target.x = hit.x;
            target.z = hit.z;
        }
        target.y -= 0.5f;
        position.y -= 0.5f;
        SetCharacterWorldPosition(request.character, &target);
        mtwkSetExtraFlag(request.motionWork, 15, 0);
        request.stopTranslation = 1;
        if (mtwkIsRightFootLower(request.motionWork))
            mtwkSetExtraFlag(request.motionWork, 16, 2);
        else
            mtwkSetExtraFlag(request.motionWork, 16, 0);
        request.flags |= 0x100000U;
        break;
    }
    case 0xf020:
    case 0xf021:
    case 0xf022:
    case 0xf023:
    case 0xf024:
        request.blendFrames = 1;
        request.mirrored = 0;
        break;
    case 0xf025:
    case 0xf026:
    case 0xf027:
    case 0xf028:
    case 0xf029:
        request.blendFrames = 1;
        request.mirrored = 1;
        break;
    default:
        if (request.previousClass == 10)
            request.blendFrames = 10;
        break;
    }
}


void UpdateReachedFloorStairCollision(PlayerMovementState &movement);
void PrepareReachedStairPhaseCorrection(PlayerMovementState &movement);
void ApplyReachedCollisionRootCorrection(PlayerMovementState &movement);
void UpdateReachedRootConsumer(PlayerMovementState &movement);

static const std::uint32_t kPlayerGaitMotions[11] = {0xf005U, 0xf03dU, 0xf03cU, 0xf03bU, 0xf03aU, 0xf039U, 0xf038U, 0xf037U, 0xf036U, 0xf035U, 0xf006U};

// @verified - d
static std::int32_t GetPlayerGaitRank(std::uint32_t motion) {
    for (std::int32_t index = 0; index < 11; ++index) {
        if (kPlayerGaitMotions[index] == motion)
            return index;
    }
    return 0;
}


// @verified - d
void UpdatePlayerCollisionSpeedScale(PlayerMovementState &movement) {
    MotionWork *work = movement.motion;
    if (std::fabs(movement.movementResult) < 0.00000011920929f)
        return;
    const std::int32_t id = mtwkGetAnimationId(work, static_cast<MotionRegion>(0));
    if (!(id == 0xf005 || id == 0xf006 || id == 0xf035 || id == 0xf036 || id == 0xf037 || id == 0xf038 || id == 0xf039 || id == 0xf03a || id == 0xf03b ||
            id == 0xf03c || id == 0xf03d))
        return;
    const std::uint16_t motion = id;
    Vector3 terminal;
    SampleNativeMotionRoot(motion, mtwkGetRigBoneId(work), -1.0f, &terminal);
    float step = (terminal.z < 0.0f ? -terminal.z : terminal.z) / static_cast<float>(GetMotionFrameCount(motion) - 1);
    float ratio = movement.movementResult / step;
    ratio = (ratio < 0.42500001f ? 0.42500001f : ratio) < 1.625f ? (ratio < 0.42500001f ? 0.42500001f : ratio) : 1.625f;
    float factor;
    if (!(ratio < 1.0f))
        factor = 0.96153849f;
    else
        factor = 0.58823526f;
    float speed;
    speed = NativeSqrt(ratio / factor);
    float rig = speed * factor;
    speed = InterpolateTo(speed / movement.secondScale, movement.previousFirstScale, 0.30000001f);
    rig = InterpolateTo(rig, movement.previousRigScale, 0.30000001f);
    movement.firstScale = speed;
    movement.verticalMotionScale = rig;
}


// @verified - d
void UpdateReachedRootConsumerAndScale(PlayerMovementState &movement) {
    MotionWork *const work = movement.motion;
    UpdatePlayerCollisionSpeedScale(movement);
    UpdateReachedFloorStairCollision(movement);
    PrepareReachedStairPhaseCorrection(movement);
    ApplyReachedCollisionRootCorrection(movement);
    UpdateReachedRootConsumer(movement);
    mtwkSetSpeed(work, movement.secondScale * movement.firstScale, static_cast<MotionRegion>(0));
    movement.previousFirstScale = movement.firstScale;
    mtwkSetRigScale(work, movement.verticalMotionScale);
    movement.previousRigScale = movement.verticalMotionScale;
    if (movement.parentBoneSelector) {
        if (mtwkSetRegionFlag(work, 68, 1, static_cast<MotionRegion>(0)) || (movement.flags & 0x100U)) {
            if (movement.parentBoneSelector > 0)
                mtwkGetParentBoneWrapper(work, 0x6000001e);
            if (movement.parentBoneSelector < 0)
                mtwkGetParentBoneWrapper(work, 0x60000019);
            if (mtwkSetRegionFlag(work, 68, 1, static_cast<MotionRegion>(0)) && mtwkGetFrame(work, static_cast<MotionRegion>(0)) > 2.0f)
                movement.parentBoneSelector = 0;
        } else {
            movement.parentBoneSelector = 0;
        }
    }
    if ((movement.flags & 0x1000U) && !mtwkSetRegionFlag(work, 68, 1, static_cast<MotionRegion>(0)) && !(movement.flags & 0x100U)) {
        mtwkSetExtraFlag(work, 15, 2);
        movement.flags &= ~0x1000U;
    }
}


// @verified - d
void UpdateReachedRootConsumer(PlayerMovementState &movement) {
    std::uint32_t animation = mtwkGetAnimationId(movement.motion, static_cast<MotionRegion>(0));
    std::uint32_t selected = animation;
    if (movement.flags & 0x100U)
        selected = movement.playerMotionId;
    if (!(selected == 0xf005U || selected == 0xf006U || selected == 0xf035U || selected == 0xf036U || selected == 0xf037U || selected == 0xf038U ||
            selected == 0xf039U || selected == 0xf03aU || selected == 0xf03bU || selected == 0xf03cU || selected == 0xf03dU || animation == 0xf02cU ||
            animation == 0xf030U || animation == 0xf02dU || animation == 0xf031U || selected == 0xf02bU || selected == 0xf009U || selected == 0xf12fU ||
            selected == 0xf12eU || selected == 0xf02aU || selected == 0xf008U || animation == 0xf02bU || animation == 0xf009U || animation == 0xf12fU ||
            animation == 0xf12eU || animation == 0xf02aU || animation == 0xf008U))
        return;
    if ((movement.flags & 8U) && movement.stepDirection) {
        std::uint32_t motion;
        if (movement.stepDirection == 1) {
            if (movement.inputScale > 0.5f) {
                if (movement.groundedStepIndex == movement.targetHeightScale - 1) {
                    motion = 0xf12fU;
                    if (selected != motion)
                        TryRequestPlayerMotion(movement, motion);
                } else {
                    motion = 0xf02aU;
                    if (selected != motion)
                        TryRequestPlayerMotion(movement, motion);
                }
            } else {
                motion = 0xf02bU;
                if (selected != motion)
                    TryRequestPlayerMotion(movement, motion);
            }
        } else if (movement.stepDirection == -1) {
            if (movement.inputScale > 0.5f) {
                if (movement.groundedStepIndex == 1) {
                    motion = 0xf12eU;
                    if (selected != motion)
                        TryRequestPlayerMotion(movement, motion);
                } else {
                    motion = 0xf008U;
                    if (selected != motion)
                        TryRequestPlayerMotion(movement, motion);
                }
            } else {
                motion = 0xf009U;
                if (selected != motion)
                    TryRequestPlayerMotion(movement, motion);
            }
        }
    } else if (!(movement.flags & 0x100U)) {
        std::int32_t index = static_cast<std::int32_t>(movement.inputScale / 0.099999003f);
        if (index < 0)
            index = 0;
        if (index > 10)
            index = 10;
        TryRequestPlayerMotion(movement, kPlayerGaitMotions[index]);
    }
    if ((movement.flags & 0x100U) || mtwkSetRegionFlag(movement.motion, 65, 1, static_cast<MotionRegion>(0))) {
        std::int32_t stair = 0;
        if (animation == 0xf02bU || animation == 0xf009U || animation == 0xf12fU || animation == 0xf12eU || animation == 0xf02aU || animation == 0xf008U)
            stair = 1;
        else {
            animation = movement.playerMotionId;
            if (animation == 0xf02bU || animation == 0xf009U || animation == 0xf12fU || animation == 0xf12eU || animation == 0xf02aU || animation == 0xf008U)
                stair = 1;
        }
        if (stair == 1) {
            Vector3 position, local, target;
            GetCharacterWorldPosition(movement.character, &position);
            matrix_stack::PushUnitMatrix();
            Matrix4x4::RotateAroundY_Wrapper(-movement.targetWorldYaw);
            Matrix4x4::preTranslate(-movement.targetWorldPosition.x, -movement.targetWorldPosition.y, -movement.targetWorldPosition.z);
            Matrix4x4::TransformPoint(position, local);
            PopCurrentMatrix(1);
            if (!movement.groundedStepIndex || movement.groundedStepIndex == movement.targetHeightScale) {
                target.x = local.x;
                target.y = static_cast<float>(movement.groundedStepIndex) * 0.22499999f;
                target.z = local.z;
            } else {
                target.x = local.x;
                target.y = static_cast<float>(movement.groundedStepIndex) * 0.22499999f;
                target.z = -(static_cast<float>(movement.groundedStepIndex) * 0.30000001f - 0.15000001f);
            }
            matrix_stack::PushUnitMatrix();
            Matrix4x4::preTranslate(movement.targetWorldPosition.x, movement.targetWorldPosition.y, movement.targetWorldPosition.z);
            Matrix4x4::RotateAroundY_Wrapper(movement.targetWorldYaw);
            Matrix4x4::TransformPoint(target, local);
            PopCurrentMatrix(1);
            SetCharacterWorldPosition(movement.character, &local);
            movement.previousStepIndex = movement.groundedStepIndex;
            movement.rootConsumerPosition = local;
        }
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
std::uint8_t ReachedMotionRootMode(std::uint32_t motionId) {

#if ((SHENMUE_IS_DC && SHENMUE_IS_S2) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) && SHENMUE_S2_GCC295)
    std::uint8_t mode = 10;
    switch (static_cast<std::int32_t>(motionId)) {
    case 0xf002 ... 0xf004:
        mode = 0;
        break;
    case 0xf005:
    case 0xf009:
    case 0xf02b:
    case 0xf035 ... 0xf03d:
        mode = 2;
        break;
    case 0xf006:
    case 0xf008:
    case 0xf02a:
    case 0xf12e:
    case 0xf12f:
        mode = 6;
        break;
    case 0xf020 ... 0xf029:
        mode = 9;
        break;
    case 0xf02c ... 0xf033:
        mode = 1;
        break;
    case 0xf034:
        mode = 4;
        break;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || (((SHENMUE_IS_DC && SHENMUE_IS_S2) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) && !SHENMUE_S2_GCC295))
    if (motionId >= 0xf002U && motionId <= 0xf004U)
        return 0;
    if (motionId == 0xf005U || motionId == 0xf009U || motionId == 0xf02bU || (motionId >= 0xf035U && motionId <= 0xf03dU)) {
        return 2;
#endif
    }

#if ((SHENMUE_IS_DC && SHENMUE_IS_S2) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) && SHENMUE_S2_GCC295)
    return mode;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || (((SHENMUE_IS_DC && SHENMUE_IS_S2) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) && !SHENMUE_S2_GCC295))
    if (motionId == 0xf006U || motionId == 0xf008U || motionId == 0xf02aU || motionId == 0xf12eU || motionId == 0xf12fU) {
        return 6;
    }
    if (motionId >= 0xf020U && motionId <= 0xf029U)
        return 9;
    if (motionId >= 0xf02cU && motionId <= 0xf033U)
        return 1;
    if (motionId == 0xf034U)
        return 4;
    return 10;
#endif
}


float ReachedMotionSpeed(std::uint32_t motionId) {

#if ((SHENMUE_IS_DC && SHENMUE_IS_S2) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) && SHENMUE_S2_GCC295)
    float speed = 1.0f;
    switch (motionId) {
    case 0xf005:
    case 0xf02c ... 0xf033:
        speed = 1.3f;
        break;
    case 0xf006:
    case 0xf035 ... 0xf03d:
        speed = 1.075f;
        break;
    case 0xf034:
        speed = 1.2f;
        break;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || (((SHENMUE_IS_DC && SHENMUE_IS_S2) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) && !SHENMUE_S2_GCC295))
    if (motionId == 0xf005U || (motionId >= 0xf02cU && motionId <= 0xf033U)) {
        return 1.3f;
#endif
    }

#if ((SHENMUE_IS_DC && SHENMUE_IS_S2) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) && SHENMUE_S2_GCC295)
    return speed;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || (((SHENMUE_IS_DC && SHENMUE_IS_S2) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM) && !SHENMUE_S2_GCC295))
    if (motionId == 0xf006U || (motionId >= 0xf035U && motionId <= 0xf03dU)) {
        return 1.075f;
    }
    if (motionId == 0xf034U)
        return 1.2f;
    return 1.0f;
#endif
}
#endif

} // namespace player_detail

} // namespace s2

} // namespace shenmue
