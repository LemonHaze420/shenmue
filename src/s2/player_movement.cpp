#include "s2/player_internal.hpp"

namespace shenmue::s2::player_detail {

void UpdateTargetWorldMovementGate(PlayerModelState &state) {


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
    SampleNativeMotionRoot(0xf005U, GetMotionRigBoneId(movement.motionWork), -1.0f, root);
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
    if (state.motionWork && GetMotionAnimationId(state.motionWork, MotionRegion::LowerBody)) {
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
std::uint8_t ReachedMotionRootMode(std::uint32_t motionId) {
    if (motionId >= 0xf002U && motionId <= 0xf004U)
        return 0;
    if (motionId == 0xf005U || motionId == 0xf009U || motionId == 0xf02bU || (motionId >= 0xf035U && motionId <= 0xf03dU)) {
        return 2;
    }
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
}

// @verified - p
float ReachedMotionSpeed(std::uint32_t motionId) {
    if (motionId == 0xf005U || (motionId >= 0xf02cU && motionId <= 0xf033U)) {
        return 1.3f;
    }
    if (motionId == 0xf006U || (motionId >= 0xf035U && motionId <= 0xf03dU)) {
        return 1.075f;
    }
    if (motionId == 0xf034U)
        return 1.2f;
    return 1.0f;
}

// @verified - p
bool CanRequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested, float frame, float frameCount) {
    MotionWork *const work = movement.motionWork;
    if ((movement.flags & 4U) || SetMotionRegionFlag(work, 65U, 1, MotionRegion::LowerBody))
        return true;
    const std::uint16_t current = GetMotionAnimationId(work, MotionRegion::LowerBody);
    bool tracked = SetMotionRegionFlag(work, 68U, 1, MotionRegion::LowerBody) != 0;
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
        return !tracked && SetMotionRegionFlag(work, 66U, 1, MotionRegion::LowerBody) != 0;
    default:
        return true;
    }
}

std::uint16_t SelectPlayerIdleMotion(PlayerMovementState &movement, bool &mirrored) {
    MotionWork *const work = movement.motionWork;
    const std::uint16_t current = GetMotionAnimationId(work, MotionRegion::LowerBody);
    std::uint16_t selected = 0xf002U;
    mirrored = false;
    switch (current) {
    case 0xf002U:
        mirrored = SetMotionRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
        if ((g_playerRuntime.flags & 0x00080000U) == 0 &&
            static_cast<std::int8_t>(movement.field05f) > static_cast<std::int32_t>(IncrementSharedPrngUInt16() % 4U + 2U)) {
            selected = 0xf004U;
        }
        break;
    case 0xf003U:
        selected = 0xf003U;
        break;
    case 0xf004U:
        mirrored = SetMotionRegionFlag(work, 70U, 1, MotionRegion::LowerBody) == 0;
        break;
    default:
        mirrored = !GetIsRHandAboveShoulder(work);
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
    const std::uint32_t motion = GetMotionAnimationId(work, MotionRegion::LowerBody);
    const std::uint32_t range = motion - 0xf008U;
    if (range <= 0x23U && ((0xc00000003ULL >> range) & 1U))
        return;
    if (motion - 0xf12eU <= 1U)
        return;
    const Vector3 root = GetMotionRootPosition(work);
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
    AssignMotionTargetCallback(work, plymAdjustControllerGroundTarget, 25);
    AssignMotionTargetCallback(work, plymAdjustControllerGroundTarget, 30);
    SetMotionTimeRelated(work, 1U, 0U, 0xf002U);
    SetMotionTimeRelated(work, 1U, 1U, 0xf005U);
    SetMotionTimeRelated(work, 1U, 4U, 0xf006U);
    const Vector3 location = GetCharacterWorldPosition(movement.character);
    Vector3 first{}, second{};
    GetMotionControllerTargetPosition(work, 6U, first);
    GetMotionControllerTargetPosition(work, 5U, second);
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
    SetMotionTimeRelated(work, 1, 0, 0xf002);
    SetMotionTimeRelated(work, 1, 1, 0xf005);
    SetMotionTimeRelated(work, 1, 4, 0xf006);
    if (retainMotion == 0) {
        bool mirrored = false;
        const std::uint16_t motion = SelectPlayerIdleMotion(*movement, mirrored);
        SetMotionSpeed(work, 1.0f, MotionRegion::LowerBody);
        SetMotionRigScale(work, 1.0f);
        if (GetMotionAnimationId(work, MotionRegion::LowerBody) != motion) {
            mtwkSetRequestedRootMode(work, 1);
            SetMotionFrameTime(work, 20, MotionRegion::LowerBody);
            mtwkRequestMotion(work, motion, MotionRegion::LowerBody);
            if (mirrored)
                SetMotionRegionFlag(work, 69, 0, MotionRegion::LowerBody);
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
            request.parentSelector = GetIsRHandAboveShoulder(work) ? 1 : -1;
        } else {
            secondHalf = request.previousFrame >= 20.0f;
            if (SetMotionRegionFlag(work, 70U, 1, MotionRegion::LowerBody))
                secondHalf = !secondHalf;
            request.parentSelector = secondHalf ? -1 : 1;
        }
        request.rootMode = 2;
        if (targetStairs) {
            request.blendFrames = 2;
            request.mirrored = SetMotionRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
        } else {
            if (previous == 0xf002U)
                secondHalf = SetMotionRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
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
            request.mirrored = middleHalf != (SetMotionRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0);
        } else if (IsPlayerOrdinaryGait(request.motion)) {
            const bool secondHalf = middleHalf != (SetMotionRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0);
            request.rootMode = 2;
            if (request.motion == 0xf005U) {
                request.sampleFrame = secondHalf ? 21.0f : 3.0f;
            } else if (request.motion == 0xf006U) {
                request.sampleFrame = secondHalf ? 9.0f : 1.0f;
            } else {
                request.sampleFrame = request.frameCount * (secondHalf ? 9.0f : 1.0f) / 18.0f;
            }
        } else {
            request.mirrored = SetMotionRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
        }
    } else if (previous == 0xf12eU || previous == 0xf12fU) {
        const bool leadFoot = SetMotionRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
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
        request.mirrored = SetMotionRegionFlag(work, 70U, 1, MotionRegion::LowerBody) != 0;
    }
    if (SetMotionRegionFlag(work, 66U, 1, MotionRegion::LowerBody)) {
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
    SetMotionRegionEvalTime(work, request.sampleFrame, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(work, request.rootMode);
    SetMotionFrameTime(work, request.blendFrames, MotionRegion::LowerBody);
}

void ApplyPlayerIdleStop(PlayerMovementState &movement) {
    MotionWork *work = movement.motionWork;
    const std::uint16_t previousMotion = GetMotionAnimationId(work, MotionRegion::LowerBody);
    Vector3 terminal{};
    SampleNativeMotionRoot(previousMotion, GetMotionRigBoneId(work), -1.0f, terminal);
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
    mtwkSetExtraFlag(work, 16U, GetIsRHandBelowShoulder(work) ? 2 : 0);
}

void ApplyPlayerMotionRequest(PlayerMovementState &movement, std::uint32_t motionId, float previousFrame, float previousFrameCount, bool mirrored,
    std::int16_t requestedYaw, std::int16_t authoredYaw) {
    MotionWork *const motionWork = movement.motionWork;
    const std::uint16_t previousMotion = GetMotionAnimationId(motionWork, MotionRegion::LowerBody);
    const float fraction = std::fabs(previousFrameCount) >= std::numeric_limits<float>::epsilon() ? previousFrame / (previousFrameCount - 1.0f) : 0.0f;
    PlayerMotionRequest request{previousMotion, motionId, movement.rootYawMode, previousFrame, fraction, 0.0f};
    request.mirrored = mirrored;

    movement.playerMotionId = motionId;
    const bool preserveRootYaw =
        SetMotionRegionFlag(motionWork, 65U, 1, MotionRegion::LowerBody) != 0 || (movement.flags & 0x2000U) != 0 || movement.motionTransitionParameter != 0;
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
    SetMotionSpeed(motionWork, speed, MotionRegion::LowerBody);
    if (authoredYaw != 0) {
        AssignMotionRigYawOffset(motionWork, static_cast<std::uint32_t>(static_cast<std::int16_t>(requestedYaw - authoredYaw)));
        movement.motionTransitionParameter = 0;
    } else {
        movement.motionTransitionParameter = requestedYaw;
    }
    movement.motionStartFrame = request.sampleFrame;
    SetMotionRegionFlag(motionWork, 69U, request.mirrored ? 0 : 2, MotionRegion::LowerBody);
    movement.flags |= 0x0900U;
    movement.field160 = speed;
    movement.playerMotionFrame = request.sampleFrame;
    movement.field15c = 1.0f;
    movement.field16c = 1.0f;
    ResetPlayerMotionFlagCursor(movement);
    switch (motionId) {
    case 0xf020U:
    case 0xf025U:
        SetMotionLayerEndFrame(motionWork, 19.0f, MotionRegion::LowerBody);
        break;
    case 0xf021U:
    case 0xf026U:
        SetMotionLayerEndFrame(motionWork, 23.0f, MotionRegion::LowerBody);
        break;
    case 0xf022U:
    case 0xf027U:
        SetMotionLayerEndFrame(motionWork, 27.0f, MotionRegion::LowerBody);
        break;
    case 0xf023U:
    case 0xf028U:
        SetMotionLayerEndFrame(motionWork, 29.0f, MotionRegion::LowerBody);
        break;
    case 0xf024U:
    case 0xf029U:
        SetMotionLayerEndFrame(motionWork, 41.0f, MotionRegion::LowerBody);
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




    shenApplyTranslationToAll(&movement.character->entity.node, world);
    movement.character->position = {};
    movement.previousStepIndex = movement.groundedStepIndex;
    movement.rootConsumerPosition = world;
    return true;
}


void UpdateReachedRootConsumer(PlayerMovementState &movement) {
    MotionWork *const motionWork = movement.motionWork;
    std::int8_t stepDirection = movement.stepDirection;
    const std::uint16_t animationId = GetMotionAnimationId(motionWork, MotionRegion::LowerBody);
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

    const bool publishRoot = (movement.flags & 0x100U) != 0 || SetMotionRegionFlag(motionWork, 65U, 1, MotionRegion::LowerBody) != 0;
    if (!publishRoot)
        return;
    ApplyReachedRootConsumerTranslation(movement, animationId);
}

void FinalizeReachedRootConsumerScale(PlayerMovementState &movement) {
    MotionWork *const motionWork = movement.motionWork;
    if (!motionWork)
        return;

    SetMotionSpeed(motionWork, movement.field160 * movement.field15c, MotionRegion::LowerBody);
    movement.previousField15c = movement.field15c;
    SetMotionRigScale(motionWork, movement.field16c);
    // ApplyPlayerRootMotion retains the previous rig scale in the shared movement owner.
    movement.previousRigScale = movement.field16c;

    if (movement.parentBoneSelector != 0) {
        const bool parentRegionActive = SetMotionRegionFlag(motionWork, 68U, 1, MotionRegion::LowerBody) != 0;
        if (!parentRegionActive && (movement.flags & 0x100U) == 0) {
            movement.parentBoneSelector = 0;
        } else {
            if (movement.parentBoneSelector > 0) {
                GetParentBoneWrapper(motionWork, movement.parentBoneSelector);
            }
            if (movement.parentBoneSelector < 0) {
                GetParentBoneWrapper(motionWork, movement.parentBoneSelector);
            }
            if (parentRegionActive && GetMotionFrame(motionWork, MotionRegion::LowerBody) > 2.0f) {
                movement.parentBoneSelector = 0;
            }
        }
    }

    if ((movement.flags & 0x1000U) != 0 && SetMotionRegionFlag(motionWork, 68U, 1, MotionRegion::LowerBody) == 0 && (movement.flags & 0x100U) == 0) {
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
    const auto motion = GetMotionAnimationId(movement.motionWork, MotionRegion::LowerBody);
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
            CopyMotionTransition(movement.motionWork, &movement.transitionTargetPosition, movement.transitionTargetYaw, 0, 0);
        } else if (movement.flags & 0x4000U) {
            CopyMotionTransition(movement.motionWork, &movement.transitionTargetPosition, movement.transitionTargetYaw, 0, 2);
        } else if (movement.flags & 0x8000U) {
            CopyMotionTransition(movement.motionWork, &movement.transitionTargetPosition, movement.transitionTargetYaw, 0, 1);
        }
        CopyMotionTransition(movement.motionWork, nullptr, 0, 2, 0);
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
        const auto motion = GetMotionAnimationId(state.motionWork, MotionRegion::LowerBody);
        if (motion == 0xfa21U || motion == 0xfa22U) {
            const int frame = std::clamp(static_cast<int>(GetMotionFrame(state.motionWork, MotionRegion::LowerBody) - 170.0f), 0, 30);
            const int offset = (frame << 14) / 30;
            const bool mirrored = SetMotionRegionFlag(state.motionWork, 70U, 1, MotionRegion::LowerBody) != 0;
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

    std::int32_t yawDistance = static_cast<std::int16_t>(state.collisionBoundaryYaw - state.currentYaw);
    yawDistance = std::clamp(std::abs(yawDistance), 0, 0x4000);
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

} // namespace shenmue::s2::player_detail

namespace shenmue::s2 {

std::int32_t RequestPlayerMotion(PlayerMovementState &movement, std::int32_t requested) {
    MotionWork *const work = movement.motionWork;
    if (!work || !movement.character)
        return -1;
    const float frameCount = static_cast<float>(GetMotionFrameCount(GetMotionAnimationId(work, MotionRegion::LowerBody)));
    float frame = GetMotionFrame(work, MotionRegion::LowerBody);
    if (SetMotionRegionFlag(work, 68U, 1, MotionRegion::LowerBody) && work->runtime.currentRootMode != 2U && work->runtime.currentRootMode != 3U) {
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
    if (requested == GetMotionAnimationId(work, MotionRegion::LowerBody) && SetMotionRegionFlag(work, 66U, 1, MotionRegion::LowerBody))
        return -1;
    ApplyPlayerMotionRequest(movement, static_cast<std::uint32_t>(requested), frame, frameCount, mirrored, requestedYaw, authoredYaw);
    return requested;
}

Vector3 GetPlayerTargetWorldPosition() {
    const PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return {};
    const PlayerMovementState &movement = state->embeddedMovementState;
    Vector3 position = GetCharacterWorldPosition(state->characterNode);
    if ((movement.flags & 8U) == 0)
        return position;

    const auto targetYaw = static_cast<std::uint16_t>(movement.targetWorldYaw);
    const float sine = sin_fixed16(movement.targetWorldYaw);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(targetYaw)));
    const float localZ = (position.x - movement.targetWorldPosition.x) * sine + (position.z - movement.targetWorldPosition.z) * cosine;
    float height = std::max(0.0f, -localZ * 0.74999994f);
    height = std::min(height, static_cast<float>(movement.targetHeightScale) * 0.22499999f);
    position.y = movement.targetWorldPosition.y + height;
    return position;
}

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
    movement.rootYawMode = ReachedMotionRootMode(GetMotionAnimationId(work, MotionRegion::LowerBody));
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
    movement.motionAnimationId = GetMotionAnimationId(work, MotionRegion::LowerBody);
    movement.leadFootDirection = 0;
    movement.yawInterpolationRate = 1.0f;
    movement.field15c = 1.0f;
    movement.field16c = 1.0f;
    if (SetMotionRegionFlag(work, 65U, 1, MotionRegion::LowerBody)) {
        movement.flags |= 0x400U;
    }
    movement.playerMotionFrame = GetMotionFrame(work, MotionRegion::LowerBody);
    Vector3 terminalRoot;
    SampleNativeMotionRoot(movement.motionAnimationId, GetMotionRigBoneId(work), -1.0f, terminalRoot);
    movement.rootMotionStep = std::fabs(terminalRoot.z) / static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(movement.motionAnimationId)) - 1);
    movement.movementResult = 0.0f;
    movement.motionSpeed = movement.rootMotionStep * movement.field160;
    ProcessPlayerMotionFlagCursor(movement);
    if (movement.motionTransitionParameter && !SetMotionRegionFlag(work, 68U, 1, MotionRegion::LowerBody)) {
        const std::int16_t rate = static_cast<std::int16_t>(
            static_cast<std::int32_t>(movement.motionTransitionParameter / (GetMotionEndFrame(work, MotionRegion::LowerBody) - movement.motionStartFrame)));
        const std::int32_t correction = static_cast<std::int32_t>(rate * (movement.previousField15c * movement.field160));
        movement.pendingYawCorrection = static_cast<std::int16_t>(movement.pendingYawCorrection + correction);
        std::int16_t remainder = static_cast<std::int16_t>(movement.rootYawOffset - correction);
        if ((movement.rootYawOffset > 0 && remainder < 0) || (movement.rootYawOffset < 0 && remainder > 0))
            remainder = 0;
        movement.rootYawOffset = remainder;
    }
    if (SetMotionRegionFlag(work, 66U, 1, MotionRegion::LowerBody) && (movement.flags & 0x400U)) {
        movement.playerMotionFlagCursor = movement.playerMotionFlagCommands;
        movement.rootYawOffset = 0;
        movement.motionTransitionParameter = 0;
    }
    GetMotionAnimationId(work, MotionRegion::LowerBody);
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

std::int32_t plymGetMovingFrames() {
    return movingFrames;
}

std::int32_t plymGetTurningFrames() {
    return turningFrames;
}

} // namespace shenmue::s2
