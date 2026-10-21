#include "s1/task/vf_internal.hpp"

namespace shenmue::s1::vf_detail {


// @verified - p
void BuildFightNodeTransform(Character &character, std::uint8_t nodeId, Matrix4x4 *output) {
    MotionManagerWork *const manager = MOTM_GetWork(&character);
    matrix_stack::Matrix4x4_Push();
    MotionNodeRuntime *row = static_cast<MotionNodeRuntime *>(manager->motion.previousNodes);
    while (static_cast<std::int8_t>(row->nodeId) != nodeId && row->nodeId != 0xffU)
        ++row;
    if (row->nodeId == 0xffU)
        row = nullptr;
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::Multiply(row->transform);
    *(output ? output : g_matrixStack.top.stack) = workTransformMatrix;
    Matrix4x4::Pop();
}


// @verified - p
void TransformFightPoint(const Vector3 (&points)[2], Vector3 &first, Vector3 &second) {
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::TransformPoint(points[0], first);
    const Matrix4x4 transform = workTransformMatrix;
    Matrix4x4::Pop();
    matrix_stack::Matrix4x4_Push(&transform);
    Matrix4x4::TransformPoint(points[1], second);
    Matrix4x4::Pop();
}

std::int16_t FightFixedAtan2(float x, float z) {
    return static_cast<std::int16_t>(atan2_fixed16(x, z));
}


// @verified - p
std::int32_t ConvertFightNodeAngle(const Vector3 (&points)[2]) {
    Vector3 transformed[2];
    TransformFightPoint(points, transformed[0], transformed[1]);
    if (static_cast<double>(transformed[0].y) > 0.2)
        return static_cast<std::int32_t>(atan2_fixed16(-transformed[1].x, transformed[1].y)) + 0x8000;
    if (static_cast<double>(transformed[0].y) >= -0.2)
        return static_cast<std::int32_t>(atan2_fixed16(transformed[0].x, transformed[0].z)) + 0x8000;
    return atan2_fixed16(-transformed[1].x, transformed[1].y);
}

// @verified - p
float vfQueryMoveClearance(const VFActor &actor, std::int16_t direction, float maximumDistance) {
    const Vector3 &origin = (actor.moveFlags & 0x10000000U) != 0 ? actor.contactCurrentPosition : actor.contactProbePositions[0];
    return COLS_QueryCombatMoveClearance(origin, direction, maximumDistance, &g_vfAirborneAngle);
}




void vfUpdateHitFacingAndKnockback(VFActor &actor) {
    VFMAData *const game = actor.game;
    if (!game || !actor.character)
        return;

    actor.previousContactPosition = actor.contactCurrentPosition;

    auto readPreviousNodePosition = [](const MotionInformationWork *motion, std::uint8_t nodeId, Vector3 &output) {
        if (!motion || !motion->previousNodes)
            return false;
        const auto *const rows = static_cast<const MotionNodeRuntime *>(motion->previousNodes);
        for (std::size_t index = 0; index < 128; ++index) {
            const MotionNodeRuntime &row = rows[index];
            if (row.nodeId == 0xffU)
                break;
            if (row.nodeId != nodeId || !row.transform)
                continue;
            output = {
                row.transform->rows[3].x,
                row.transform->rows[3].y,
                row.transform->rows[3].z,
            };
            return true;
        }
        return false;
    };

    const auto *const motion = static_cast<const MotionInformationWork *>(actor.motion);
    Vector3 node{};
    if (readPreviousNodePosition(motion, 0, node))
        actor.contactCurrentPosition = node;
    if (readPreviousNodePosition(motion, 2, node))
        actor.lowerSpinePosition = node;
    if (readPreviousNodePosition(motion, 47, node))
        actor.headAttachmentPosition = node;
    if (readPreviousNodePosition(motion, 18, node))
        actor.rightFootEndPosition = node;
    if (readPreviousNodePosition(motion, 12, node))
        actor.leftFootEndPosition = node;
    if (readPreviousNodePosition(motion, 34, node))
        actor.rightWristPosition = node;
    if (readPreviousNodePosition(motion, 27, node))
        actor.leftWristPosition = node;
    actor.headGroundDelta = actor.headAttachmentPosition.y - (actor.groundY + 1.5f);

    constexpr std::uint32_t kFacingStepOnTransition = 0x40U;
    constexpr std::uint32_t kSpecialGuardOrParry = 0x4000U;
    constexpr std::uint32_t kSpecialThrowState = 0x8000U;
    constexpr std::uint32_t kGuardState = 0x10000U;
    constexpr std::uint32_t kAirborneFlag = 0x800U;
    constexpr std::uint32_t kUnknown0x200000 = 0x200000U;
    constexpr std::uint32_t kMirrorReactionAngle = 0x40U;
    constexpr std::uint32_t kUnknown2Flag = 0x2U;
    constexpr std::uint32_t kAttackActive = 0x100U;
    constexpr std::uint32_t kHitTimeout = 0x2U;
    constexpr std::uint32_t kUnknown8 = 0x8U;

    const std::uint32_t initialHitStateFlags2 = actor.hitStateFlags2;
    actor.temporaryFacingAngle =
        static_cast<std::uint16_t>((initialHitStateFlags2 & kFacingStepOnTransition) != 0 ? actor.facingAngle + actor.facingAngleStep : actor.facingAngle);
    if (actor.reactionWindowPhase == 4) {
        actor.reactionWindowPhase = 0;
        if ((initialHitStateFlags2 & kSpecialThrowState) != 0) {
            actor.hitStateFlags2 = initialHitStateFlags2 & ~kSpecialThrowState;
            game->hitStopFrames = static_cast<std::uint8_t>(actor.animationScale * 6.0f);
        }
    }

    std::uint32_t counterReactionFlags = 0;
    actor.field438 = actor.field436;
    const std::uint32_t hitStateFlags2 = actor.hitStateFlags2;
    if ((hitStateFlags2 & (kSpecialGuardOrParry | kGuardState | kFacingStepOnTransition)) == 0) {
        actor.facingYaw = actor.facingAngle;
    } else {
        matrix_stack::Matrix4x4_Push();
        BuildFightNodeTransform(*actor.character, 2, &actor.lowerSpineTransform);
        actor.lowerSpineTransform.rows[3].x = 0.0f;
        actor.lowerSpineTransform.rows[3].y = 0.0f;
        actor.lowerSpineTransform.rows[3].z = 0.0f;
        workTransformMatrix = actor.lowerSpineTransform;
        TransformFightPoint(kVFGuardProbeLocalVectors, actor.guardProbeWorldPosition, actor.guardProbeWorldDirection);
        BuildFightNodeTransform(*actor.character, 27, &actor.leftWristTransform);
        actor.leftWristTransform.rows[3].x = 0.0f;
        actor.leftWristTransform.rows[3].y = 0.0f;
        actor.leftWristTransform.rows[3].z = 0.0f;
        workTransformMatrix = actor.leftWristTransform;
        actor.angles[0] = static_cast<std::int16_t>(ConvertFightNodeAngle(kVFWristAngleLocalVectors));
        BuildFightNodeTransform(*actor.character, 34, &actor.rightWristTransform);
        actor.rightWristTransform.rows[3].x = 0.0f;
        actor.rightWristTransform.rows[3].y = 0.0f;
        actor.rightWristTransform.rows[3].z = 0.0f;
        workTransformMatrix = actor.rightWristTransform;
        actor.angles[1] = static_cast<std::int16_t>(ConvertFightNodeAngle(kVFWristAngleLocalVectors));
        Matrix4x4::Pop();

        if ((actor.hitStateFlags2 & (kSpecialGuardOrParry | kGuardState)) != 0) {
            const float tmpY = (actor.hitFlags & 0x100U) != 0 ? 0.40000001f : -0.40000001f;
            actor.hitStateFlags2 = tmpY < actor.guardProbeWorldPosition.y ? actor.hitStateFlags2 & ~kAirborneFlag : actor.hitStateFlags2 | kAirborneFlag;
        }
    }



    if ((hitStateFlags2 & (kSpecialGuardOrParry | kGuardState | kFacingStepOnTransition)) != 0) {
        const Vector3 node34 = actor.rightWristPosition;
        const Vector3 node27 = actor.leftWristPosition;
        const Vector3 node0 = actor.contactCurrentPosition;

        const float z = node27.z;
        const float x = node27.x;
        const float deltaZ = node34.z;
        const float deltaX = node34.x;
        const float dz = node27.z - node34.z;
        const float dx = node27.x - node34.x;
        const float distance = std::sqrt(dx * dx + dz * dz);
        const float y = node27.y;
        const float verticalDelta = node27.y - node0.y;
        actor.field3ec = distance;
        actor.pendingAnimationId = std::fabs(verticalDelta) <= 0.4f ? 1U : 0U;
        if (std::fabs(node34.y - node0.y) <= 0.4f)
            actor.pendingAnimationId |= 2U;
        const float wristHeightDelta = std::fabs(y - node34.y);
        actor.field3f0 = wristHeightDelta;

        const auto applyYawFromCenter = [&](float centerX, float centerZ) {
            const std::int16_t baseYaw = FightFixedAtan2(node0.x - centerX, node0.z - centerZ);
            actor.field3f8 = 2;
            std::uint16_t yawOffset = actor.guardProbeWorldPosition.y < 0.0f ? 0x8000U : 0U;
            const float facingDot = (centerZ - node0.z) * actor.guardProbeWorldDirection.z + (centerX - node0.x) * actor.guardProbeWorldDirection.x;
            if (facingDot >= 0.0f)
                yawOffset ^= 0x8000U;
            if ((actor.hitStateFlags2 & (kSpecialGuardOrParry | kGuardState)) != 0) {
                actor.hitStateFlags2 = static_cast<std::int16_t>(yawOffset) < 0 ? actor.hitStateFlags2 | kAirborneFlag : actor.hitStateFlags2 & ~kAirborneFlag;
            }
            actor.facingYaw = static_cast<std::uint16_t>(baseYaw + yawOffset);
        };

        if (actor.pendingAnimationId && (actor.pendingAnimationId >= 2U || (actor.hitStateFlags2 & (kSpecialGuardOrParry | kGuardState)) != 0)) {
            if (actor.pendingAnimationId >= 2U) {
                applyYawFromCenter((deltaX + x) * 0.5f, (deltaZ + z) * 0.5f);
            } else {
                applyYawFromCenter(x, z);
            }
        } else if (wristHeightDelta < 0.4f || distance <= 0.4f) {
            applyYawFromCenter((deltaX + x) * 0.5f, (deltaZ + z) * 0.5f);
        } else {
            actor.field3f8 = 0;
            actor.facingYaw = static_cast<std::uint16_t>(FightFixedAtan2(y <= node34.y ? dx : deltaX - x, y <= node34.y ? dz : deltaZ - z));
        }
    }

    std::uint32_t moveFlags = actor.moveFlags;
    if ((actor.hitStateFlags2 & kUnknown0x200000) == 0) {
        moveFlags = (moveFlags & kMirrorReactionAngle) != 0 ? moveFlags | kRunOrDashMode : moveFlags & ~kRunOrDashMode;
    } else {
        moveFlags = (moveFlags & kMirrorReactionAngle) != 0 ? moveFlags & ~kRunOrDashMode : moveFlags | kRunOrDashMode;
    }
    actor.moveFlags = moveFlags;

    float minimumProbeY = 32000.0f;
    for (const Vector3 &probe : actor.contactProbePositions)
        minimumProbeY = std::fmin(probe.y, minimumProbeY);
    actor.field3f4 = minimumProbeY;
    if ((((static_cast<std::int32_t>(game->battleType) - 1) & 0xfb) != 0) || actor.slotIndex == static_cast<std::uint8_t>(game->playerSlot)) {
        const std::int16_t facing = static_cast<std::int16_t>(actor.temporaryFacingAngle);
        actor.rightClearance = vfQueryMoveClearance(actor, static_cast<std::int16_t>(facing + 12288), 5.0f);
        actor.leftClearance = vfQueryMoveClearance(actor, static_cast<std::int16_t>(facing - 12288), 5.0f);
        const float forward = vfQueryMoveClearance(actor, facing, 10.0f);
        actor.airAngle = static_cast<std::int16_t>(g_vfAirborneAngle);
        actor.forwardClearance = forward;
        const float backward = vfQueryMoveClearance(actor, static_cast<std::int16_t>(facing + 32768), 10.0f);
        actor.airTiltAngle = static_cast<std::uint16_t>(g_vfAirborneAngle);
        actor.backwardClearance = backward;
    }

    const auto accumulatedYaw = static_cast<std::uint32_t>(g_fCamAccumYaw);
    if (((static_cast<std::uint32_t>(accumulatedYaw) + static_cast<std::uint32_t>(static_cast<std::int16_t>(actor.temporaryFacingAngle))) & 0x8000U) != 0) {
        actor.moveFlags &= ~kUnknown2Flag;
    } else {
        actor.moveFlags |= kUnknown2Flag;
    }

    const std::uint32_t currentHitStateFlags2 = actor.hitStateFlags2;
    if ((currentHitStateFlags2 & kAttackActive) != 0) {
        switch (actor.guardState) {
        case 0:
            counterReactionFlags = 4U;
            [[fallthrough]];
        case 1:
        case 3:
        case 4:

            counterReactionFlags |= 1U;
            if ((actor.hitParameter & 0x200U) != 0)
                counterReactionFlags &= ~1U;
            if (counterReactionFlags == 0)
                counterReactionFlags = kSpecialThrowState;
            break;
        case 2:
        case 5:
        case 6:
            counterReactionFlags = kUnknown8;
            if ((actor.hitParameter & 0x200U) == 0)
                counterReactionFlags |= kHitTimeout;
            break;
        default:
            counterReactionFlags = kSpecialThrowState;
            break;
        }
    }
    actor.counterReactionFlags = counterReactionFlags;

    if ((game->gameFlags & 0x20U) == 0) {
        if ((currentHitStateFlags2 & 0x20000800U) == 0x20000800U) {
            actor.hitStateHoldTimer = 30;
        } else if (actor.hitStateHoldTimer != 0) {
            --actor.hitStateHoldTimer;
        }
        if ((currentHitStateFlags2 & kUnknown8) != 0) {
            if (actor.reactionMotionRestartMode >= 0 && actor.currentAnimationEndFrame <= static_cast<std::uint16_t>(actor.reactionAnimationEndFrame >> 1)) {
                actor.moveFlags &= ~0x100U;
            } else {
                actor.moveFlags |= 0x100U;
            }
        } else if (actor.reactionMotionRestartMode >= 0 || actor.currentAnimationEndFrame > static_cast<std::uint16_t>(actor.reactionAnimationEndFrame >> 1)) {
            actor.moveFlags &= ~0x100U;
        } else {
            actor.moveFlags |= 0x100U;
        }
        actor.knockbackFramesRemaining =
            static_cast<std::uint16_t>(vfPredictKnockbackFramesToZ(actor, actor.predictedGroundLanding + actor.knockbackReactionTime));
        actor.previousFacingAngle = actor.facingAngle;
    }
}


std::uint32_t vfUpdateContactSlotsAndGroundFlags(VFActor &self) {
    VFMAData &game = *self.game;
    const std::int8_t targetSlot = self.targetSlotIndex;
    const std::uint8_t pairingSlot = self.pairedFighterId;
    if (targetSlot == -1 || pairingSlot == 0xffU)
        return pairingSlot;

    const std::uint8_t team = game.slotTeamId[self.slotIndex];
    bool nearEnemy = game.state != VFS_InBattle;
    bool closeEnemy = nearEnemy;
    VFActor &pair = *game.actors[pairingSlot];
    VFActor &target = *game.actors[static_cast<std::uint8_t>(targetSlot)];
    const std::uint32_t selfHitState2 = self.hitStateFlags2;
    const std::uint32_t targetHitState2 = target.hitStateFlags2;
    const std::uint32_t pairHitState2 = pair.hitStateFlags2;
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        if (!game.characters[index]) {
            self.contactPoints[index] = 0.0f;
            continue;
        }
        const VFActor &actor = *game.actors[index];
        const float dz = self.contactCurrentPosition.z - actor.contactCurrentPosition.z;
        const float dx = self.contactCurrentPosition.x - actor.contactCurrentPosition.x;
        const float distance = std::sqrt(dz * dz + dx * dx);
        self.contactPoints[index] = distance;
        if (game.slotTeamId[index] == team) {
            if (index == 0 && self.fightStyleIndex == 0x1dU && distance > 10.0f) {
                (void)vfmaAssignVoiceReactForFrame(game, 19);
                (void)vfmaAssignVoiceReactForFrame(game, 20);
            }
        } else {
            if (distance < 10.0f)
                nearEnemy = true;
            if (distance < 7.0f)
                closeEnemy = true;
        }
    }
    const std::uint32_t closeFlag = closeEnemy ? 4U : 0U;
    const float pairDistance = self.contactPoints[pairingSlot];
    if (game.reactionTypeIndex == 1 && self.fightStyleIndex == 0 && !nearEnemy) {
        (void)vfmaAssignVoiceReactForFrame(game, 7);
        (void)vfmaAssignVoiceReactForFrame(game, 8);
    }
    for (std::uint8_t index = 0; index < kBattleSlots; ++index) {
        const VFActor *const actor = game.actors[index];
        self.contactAngles[index] = game.characters[index] ? Shenmue_Atan2Fixed(self.contactCurrentPosition.x - actor->contactCurrentPosition.x,
                                                                 self.contactCurrentPosition.z - actor->contactCurrentPosition.z) :
                                                             0;
    }
    const std::int32_t pairContactAngle = static_cast<std::int16_t>(self.contactAngles[pairingSlot]);
    const std::int32_t selfAngle = static_cast<std::int16_t>(self.temporaryFacingAngle) - pairContactAngle;
    std::int32_t pairAngle = static_cast<std::int16_t>(pair.temporaryFacingAngle) - pairContactAngle;
    if ((pair.hitFlags & 0x100000U) != 0 && (pair.motionFlags & 0x100000U) != 0)
        pairAngle ^= 0x8000;

    const float moveDeltaX = self.moveDelta.x - target.moveDelta.x;
    const float moveDeltaZ = self.moveDelta.z - target.moveDelta.z;
    const std::uint16_t inverseFacing = static_cast<std::uint16_t>(-target.facingAngle);
    const float cosine = cos_fixed16(inverseFacing);
    const float sine = sin_fixed16(static_cast<std::int16_t>(inverseFacing));

    self.relativeMoveDelta.y = 0.0f;
    self.relativeMoveDelta.x = cosine * moveDeltaX - sine * moveDeltaZ;
    self.relativeMoveDelta.z = cosine * moveDeltaZ + sine * moveDeltaX;

    const std::int32_t targetContactAngle = static_cast<std::int16_t>(self.contactAngles[static_cast<std::uint8_t>(targetSlot)]);
    std::uint32_t frontFlags = closeFlag | 1U;
    if (((targetContactAngle - static_cast<std::int16_t>(self.temporaryFacingAngle) + 0x4000) & 0x8000) == 0)
        frontFlags = closeFlag;
    std::uint32_t facingFlags = frontFlags | 2U;
    if (((targetContactAngle - static_cast<std::int16_t>(target.temporaryFacingAngle) + 0x4000) & 0x8000) != 0)
        facingFlags = frontFlags;

    const auto within = [](std::int32_t angle) { return static_cast<std::uint16_t>(angle + 0x1ffe) <= 0x3ffcU; };
    const bool contactEnabled = (self.moveFlags & 0x40000000U) != 0;
    const bool fallLanding = (selfHitState2 & 0x10U) != 0;
    std::uint32_t decisionMask = 0;
    if (contactEnabled && !fallLanding && (targetHitState2 & 0xd8110U) == 0x10000U && (target.moveFlags & 0x20U) == 0 && within(selfAngle) &&
        within(pairAngle ^ 0x8000) && pairDistance <= 3.0f && (pair.hitReactionAnimSelectFlags & 1) != 0)
        decisionMask = 0x40U;

    const bool pairGuardAllows = (pairHitState2 & 0x4000U) == 0 || ((pair.motionFlags >> 20U) & 1U) != 0;
    if (contactEnabled && (selfHitState2 & 0xdc100U) == 0 && (self.motionFlags & 0x800000U) == 0 && (pairHitState2 & 0xd8110U) == 0 &&
        (pair.motionFlags & 0x800000U) == 0 && pair.currentHp > 0 && pairGuardAllows && pair.fightStyleIndex != 0x15U &&
        static_cast<std::uint16_t>((pair.currentReactionAnimation & 0x0fffU) - 0x049eU) > 1U) {
        const bool pairFlag100 = (pair.moveFlags & 0x100U) != 0;
        if (within(selfAngle ^ 0x8000) && within(pairAngle ^ 0x8000) && !fallLanding && self.contactRangeSideMin >= pairDistance) {
            decisionMask |= 2U;
        } else if (within(selfAngle)) {
            if (within(pairAngle) && !fallLanding && self.contactRangeSideMin >= pairDistance)
                decisionMask |= pairFlag100 ? 0x10U : 8U;
            else if (within(pairAngle ^ 0x8000) && self.contactRangeFrontMin >= pairDistance)
                decisionMask |= fallLanding ? 0x400000U : pairFlag100 ? 4U : 1U;
            else if (within(pairAngle - 0x4000) && !fallLanding && self.contactRangeSideMax >= pairDistance)
                decisionMask |= pairFlag100 ? 0x20000000U : 0x4000000U;
            else if (within(pairAngle + 0x4000) && !fallLanding && self.contactRangeSideMax >= pairDistance)
                decisionMask |= pairFlag100 ? 0x10000000U : 0x2000000U;
        }
    }

    bool contactAllowed = true;
    if ((target.moveFlags & 0x20000000U) == 0) {
        if ((targetHitState2 & 0x8100U) != 0) {
            contactAllowed = false;
        } else if ((targetHitState2 & 0x10000U) == 0) {
            if ((targetHitState2 & 0x4000U) == 0)
                contactAllowed = false;
            else if ((targetHitState2 & 0x10U) != 0)
                contactAllowed = self.knockbackFramesRemaining == 0;
        }
    }
    const std::uint32_t groundFlags = contactAllowed ? facingFlags | 0x20U : facingFlags;

    std::uint32_t contactActionMask = 0;
    if (contactEnabled && (pairHitState2 & 0x102U) == 0x102U && static_cast<std::uint16_t>(selfAngle + 0x1554) <= 0x2aa8U &&
        static_cast<std::uint16_t>((pairAngle ^ 0x8000) + 0x1554) <= 0x2aa8U) {
        std::uint32_t stateMask = pairDistance > 2.0f ? 0x733f00U : 0x7fff00U;
        if (static_cast<double>(pairDistance) > 1.8)
            stateMask &= 0xffafd0ffU;
        if (static_cast<double>(pairDistance) > 1.6)
            stateMask &= ~0x1000U;
        contactActionMask = target.stateFlags2 & stateMask;
    }
    self.contactActionMask = contactActionMask;

    if ((self.latchedPairContactPriority & 1) == 0) {
        self.latchedPairContactPriority = pair.pairContactPriority;
        if (self.latchedPairContactPriority != 0 && target.currentAnimationEndFrame > pair.pairPriorityAnimationFrame) {
            self.latchedPairContactPriority = 0;
            pair.pairContactPriority = 0;
        }
    } else if (target.currentAnimationEndFrame > pair.pairPriorityAnimationFrame) {
        self.latchedPairContactPriority = 0;
        pair.pairContactPriority = 0;
    }

    const std::uint32_t biasFlags = (target.stateFlags2 & 1U) != 0 ? 0x48U : 0U;
    const std::uint32_t selfRunFlags = (self.moveFlags & kRunOrDashMode) != 0 ? biasFlags ^ 8U : biasFlags;
    const std::uint32_t runFlags = (target.moveFlags & kRunOrDashMode) != 0 ? selfRunFlags ^ 0x40U : selfRunFlags;
    std::uint32_t mirrorDifference = selfHitState2 ^ targetHitState2;
    if ((mirrorDifference & 0x200000U) != 0)
        mirrorDifference = 0x40U;
    const std::uint32_t flags = runFlags | groundFlags;
    const std::uint32_t result = ((target.moveFlags ^ mirrorDifference ^ self.moveFlags) & 0x40U) != 0 ? flags | 0x10U : flags;
    if (self.forwardClearance <= 2.5f)
        decisionMask |= 0x100U;
    if (self.backwardClearance <= 1.5f)
        decisionMask |= 0x200U;
    self.contactDecisionMask = decisionMask;
    self.groundContactFlags = static_cast<std::int32_t>(result);
    return result;
}


void vfUpdateGroundHeightAndEnemyContacts(VFActor &actor) {
    std::int32_t nearbyMask = 0;
    const float groundOffset = vfAccumEnemyGroundOffsetAndMask(actor.position, nearbyMask);
    const std::uint8_t enemyCount = std::min<std::uint8_t>(g_pFightStage->maxEnemies, static_cast<std::uint8_t>(kEnemyCapacity));

    const float target = actor.targetGroundY;
    const float ground = target + groundOffset;
    const bool changedByEnemy = ground != target;
    if (changedByEnemy || actor.groundY > target) {
        actor.groundY = ground;
        if ((actor.moveFlags & 4U) == 0) {
            actor.position.y = ground;
            if (actor.character) {
                shenApplyTranslationToAll(actor.character->entity.node, actor.position);
            }
        }
    }
    if (!changedByEnemy || (actor.moveFlags & 4U) != 0)
        return;

    for (std::uint8_t index = 0; index < enemyCount; ++index) {
        if ((nearbyMask & (1U << index)) == 0)
            continue;
        VFEnemySlot &slot = g_pFightStage->enemySlots[index];
        if ((slot.triggerFlags & 4U) != 0)
            continue;
        slot.triggerFlags |= 4U;
        auto *const motion = static_cast<MotionInformationWork *>(slot.motion);
        if (!motion || (slot.triggerFlags & 8U) != 0 || motion->frameTimer != motion->frameEnd) {
            continue;
        }
        if ((slot.triggerFlags & 2U) == 0) {
            slot.character->entity.node.rotation.y += slot.facingAngle;
            slot.triggerFlags |= 2U;
        }
        const std::int16_t animation = (slot.triggerFlags & 0x10U) != 0 ? 960 :
            (slot.triggerFlags & 0x20U) != 0                            ? 959 :
            (slot.triggerFlags & 1U) != 0                               ? 294 :
                                                                          293;
        vfUpdateDefaultReactionAnim(&slot, static_cast<std::uint16_t>(animation));
        if ((slot.triggerFlags & 0x40U) != 0)
            motion->motionFlags |= 0x80U;
        else
            motion->motionFlags &= ~0x80U;
        }
    }

void vfclResolveContactPoseConstraints(MotionInformationWork *motion, void *user);

std::uint8_t vfRegisterContactSlot(VFMAData *game, VFEnemySlot *group) {
    Character *const character = group->character;
    std::uint8_t index = 0;
    while (game->characters[index]) {
        if (++index >= kBattleSlots)
            return 99;
    }
    VFFightStage *const stage = g_pFightStage;
    const std::uint8_t team = group->teamId;
    switch (group->state) {
    case SS_Allocated:
    case SS_Inactive:
        if (!vfBindEnemySlotToFormationEntry(group))
            return 99;
        break;
    case SS_Ready:
        if (group->wave && group->wave->contactType != 0)
            return 99;
        --g_pFightStage->readyEnemySlots[team];
        break;
    case SS_Active:
        return 99;
    case SS_Cooldown:
        if (!vfAttachCharaToFormationSlot(game, group))
            return 99;
        --stage->groupCounters[team];
        break;
    default:
        break;
    }
    VFFormationGroup *const wave = group->wave;
    game->characters[index] = character;
    game->battleMeta.waves[index] = wave;
    game->battleMeta.groups[index] = group;
    group->state = SS_Active;
    group->tickFrames = 0;
    wave->waveId = 1;
    g_vfFightRegistry[static_cast<std::uint8_t>(group->registryIndex)].slot = 1;
    VFActor *const self = group->fighter;
    game->actors[index] = self;
    group->battleIndex = static_cast<std::int8_t>(index);
    game->slotRuntime[index] = self->slot;
    if (character)
        character->flags.value |= CharacterFlagActive;
    self->slotIndex = index;
    self->pairedCharacterCache = nullptr;
    self->targetingCharacter = nullptr;
    self->pairedActor = nullptr;
    self->targetActor = nullptr;
    self->pairedFighterId = 0xffU;
    self->targetSlotIndex = -1;
    group->position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    self->contactCurrentPosition.x = group->position.x;
    self->position.x = group->position.x;
    self->contactCurrentPosition.y = group->position.y;
    self->position.y = group->position.y;
    self->contactCurrentPosition.z = group->position.z;
    self->position.z = group->position.z;
    self->fightStyleIndex = wave->fightStyle;
    self->motionFlags = 0;
    self->hitStateFlags2 = 0;
    self->moveFlags = 0;
    self->hitFlags = 0;
    self->hitStateFlags = 0;
    auto *const motion = static_cast<MotionInformationWork *>(group->motion);
    motion->mobjCallback = reinterpret_cast<void *>(vfclResolveContactPoseConstraints);
    motion->mobjCallbackUser = self;
    game->slotTeamId[index] = static_cast<std::uint8_t>(wave->teamId);
    const std::int8_t hpPoolMode = wave->hpPoolMode;
    game->slotHpPoolMode[index] = static_cast<std::uint8_t>(hpPoolMode);
    if (!hpPoolMode)
        game->playerSlot = static_cast<std::int8_t>(index);
    ++stage->activeFightersByTeam[team];
    return index;
}

MotionNodeRuntime *vfFindEffectorRow(void *rows, std::uint8_t nodeId) {
    for (auto *row = static_cast<MotionNodeRuntime *>(rows); row; ++row) {
        if (row->nodeId == nodeId)
            return row;
        if (row->nodeId == 0xffU)
            return nullptr;
    }
    return nullptr;
}

float shenCheckIntersectionWithOffset(Character *character, Vector3 &position) {
    if (std::fabs(position.y) < std::numeric_limits<float>::epsilon())
        position.y = 0.0f;
    const float y = position.y;
    position.y = static_cast<float>(static_cast<double>(y) + 1.0);
    float height = mapUpdateIntersectionStateAtLocalPoint(position);
    position.y = y;
    for (CharacterNode *node = character ? character->entity.node.child : nullptr; node; node = node->child)
        height += node->position.y;
    return height;
}

void vfclResolveContactPoseConstraints(MotionInformationWork *motion, void *user) {
    auto *const other = static_cast<VFActor *>(user);
    if ((other->game->gameFlags & 0x2000000U) != 0)
        return;
    SelectCharacterMapQuery(other->character);
    const VFActor *const target = other->targetActor;
    Vector3 positions[7]{};
    for (std::size_t effector = 0; effector < 7; ++effector) {
        if (const MotionNodeRuntime *const row = vfFindEffectorRow(motion->activeNodes, kVFContactEffectorNodes[effector]))
            positions[effector] = {row->effectorPosition.x, row->effectorPosition.y, row->effectorPosition.z};
    }
    float yOffset = 0.0f;
    float offsets[7]{};
    offsets[0] = vfAccumEnemyGroundOffsetAndMask(positions[0], other->effectorEnemyMasks[0]) + shenCheckIntersectionWithOffset(other->character, positions[0]);
    offsets[1] = vfAccumEnemyGroundOffsetAndMask(positions[1], other->effectorEnemyMasks[1]) + shenCheckIntersectionWithOffset(other->character, positions[1]);
    const bool airborne = (other->moveFlags & kVFAirborne) != 0;
    std::uint8_t groundSide = 0;
    if (!airborne && (other->hitStateFlags2 & 0x4000U) == 0) {
        groundSide = offsets[1] <= offsets[0] ? 1 : 0;
        if ((other->hitFlags & 0x100U) != 0) {
            const std::uint32_t category = kVFMoveCategoryMasks[other->moveId];
            if (!groundSide && (category & 0x1c00U) != 0)
                groundSide = 1;
            else if ((category & 0xe000U) != 0)
                groundSide = 0;
        }
        if (other->groundY > offsets[groundSide])
            yOffset = offsets[groundSide] - other->groundY;
    }
    float attackBlend = 0.0f;
    if ((other->hitFlags & 0x100U) != 0 && !airborne) {
        const float dx = other->contactCurrentPosition.x - target->contactCurrentPosition.x;
        const float dz = other->contactCurrentPosition.z - target->contactCurrentPosition.z;
        const float distance = std::sqrt(dx * dx + dz * dz);
        float amount = 0.0f;
        if (distance <= 1.0f)
            amount = 1.0f;
        else if (distance < 2.0f)
            amount = static_cast<float>(2.0 - static_cast<double>(distance));
        const std::uint16_t frame = other->currentAnimationEndFrame;
        const std::uint16_t attackWindowEnd = other->attackWindowEndFrame;
        if (frame < attackWindowEnd) {
            attackBlend = static_cast<float>(frame) / static_cast<float>(attackWindowEnd) * amount;
        } else {
            float scale = 1.0f;
            const std::uint16_t middle = other->reactionMidFrame;
            if (frame > middle) {
                const float progress = (static_cast<float>(frame) - static_cast<float>(middle)) /
                    (static_cast<float>(static_cast<std::uint16_t>(other->reactionAnimationEndFrame)) - static_cast<float>(middle));
                scale = static_cast<float>(1.0 - static_cast<double>(progress));
            }
            attackBlend = scale * amount;
        }
    }
    bool changed[7]{};
    float limit = 0.0f;
    for (std::size_t effector = 0; effector < 7; ++effector) {
        if (effector > 1)
            offsets[effector] = vfAccumEnemyGroundOffsetAndMask(positions[effector], other->effectorEnemyMasks[effector]) +
                shenCheckIntersectionWithOffset(other->character, positions[effector]);
        const float offset = offsets[effector];
        other->effectorGroundOffsets[effector] = offset;
        float top = kVFContactEffectorHeights[effector] + other->position.y;
        const float floor = offset + kVFContactEffectorHeights[effector];
        if (yOffset != 0.0f) {
            changed[effector] = true;
            top += yOffset;
            positions[effector].y += yOffset;
        }
        if ((other->hitFlags & 0x100U) != 0 && target->fightStyleIndex != 0x15U && (target->hitStateFlags & 0x2000000U) == 0 &&
            (other->moveFlags & kVFAirborne) == 0) {
            const std::uint32_t category = kVFMoveCategoryMasks[other->moveId];
            bool apply = false;
            if (effector == 0) {
                apply = (category & 0x1000U) != 0;
                if (apply)
                    limit = 0.3f;
            } else if (effector == 1) {
                apply = (category & 0x8000U) != 0;
                if (apply)
                    limit = 0.3f;
            } else if (effector == 5) {
                apply = (category & 0x20U) != 0;
                if (apply)
                    limit = 0.2f;
            } else if (effector == 6) {
                apply = (category & 0x100U) != 0;
                if (apply)
                    limit = 0.2f;
            }
            if (apply) {
                const float delta = ((other->copiedGroundDelta + target->groundY) - (yOffset + other->groundY)) * attackBlend;
                const float step = delta > limit ? limit : std::fmax(-limit, delta);
                positions[effector].y += step;
                if (floor >= positions[effector].y)
                    positions[effector].y = floor;
                changed[effector] = true;
            }
        }
        const float sideDelta = static_cast<float>(std::fabs(static_cast<double>(offset - offsets[groundSide])));
        if ((other->moveFlags & kVFAirborne) != 0 || static_cast<double>(sideDelta) <= 0.01 || top <= positions[effector].y || positions[effector].y <= floor) {
            other->effectorHeightBlend[effector] = -10000.0f;
            if (static_cast<double>(sideDelta) > 0.01 && floor > positions[effector].y) {
                changed[effector] = true;
                positions[effector].y = floor;
            }
        } else {
            float blend = other->effectorHeightBlend[effector];
            changed[effector] = true;
            if (blend == -10000.0f) {
                blend = positions[effector].y - floor;
                other->effectorHeightBlend[effector] = blend;
            }
            blend *= 0.5f;
            other->effectorHeightBlend[effector] = blend;
            positions[effector].y = offset + blend;
        }
    }
    for (std::size_t effector = 0; effector < 7; ++effector) {
        if (!changed[effector])
            continue;
        if (MotionNodeRuntime *const row = vfFindEffectorRow(motion->activeNodes, kVFContactEffectorNodes[effector])) {
            row->effectorPosition.x = positions[effector].x;
            row->effectorPosition.z = positions[effector].z;
            row->effectorPosition.y = positions[effector].y;
        }
    }
}

void vfUpdateContactEffectorHeightConstraints_Callback(MotionInformationWork *motion, void *user) {
    auto *const group = static_cast<VFEnemySlot *>(user);
    Vector3 position{};
    for (std::size_t effector = 0; effector < 7; ++effector) {
        if (const MotionNodeRuntime *const row = vfFindEffectorRow(motion->previousNodes, kVFContactEffectorNodes[effector]))
            position = {row->effectorPosition.x, row->effectorPosition.y, row->effectorPosition.z};
        const float offset = group->effectorGroundOffsets[effector];
        const float top = kVFContactEffectorHeights[effector] + group->position.y;
        const float floor = kVFContactEffectorHeights[effector] + offset;
        bool changed = false;
        if (top <= position.y || position.y <= floor) {
            group->effectorHeightBlend[effector] = -10000.0f;
            if (floor > position.y) {
                changed = true;
                position.y = floor;
            }
        } else {
            float blend = group->effectorHeightBlend[effector];
            changed = true;
            if (blend == -10000.0f) {
                blend = position.y - floor;
                group->effectorHeightBlend[effector] = blend;
            }
            group->effectorHeightBlend[effector] = blend * 0.5f;
            position.y = blend * 0.5f + offset;
        }
        if (!changed)
            continue;
        if (MotionNodeRuntime *const row = vfFindEffectorRow(motion->previousNodes, kVFContactEffectorNodes[effector])) {
            row->effectorPosition.z = position.z;
            row->effectorPosition.y = position.y;
            row->effectorPosition.x = position.x;
        }
    }
}

Vector3 vfComputeNearbyFighterPush(float currentX, float currentZ, float previousX, float previousZ) {
    constexpr float kContactDiameter = 0.80000001f;
    const float currentLengthSquared = currentX * currentX + currentZ * currentZ;
    if (currentLengthSquared > 0.0f) {
        const float penetration = kContactDiameter - std::sqrt(currentLengthSquared);
        const float dot = previousX * currentX + previousZ * currentZ;
        if (dot < 0.0f) {
            const float cross = previousZ * currentX - previousX * currentZ;
            const std::int16_t angle = static_cast<std::int16_t>(Shenmue_Atan2Fixed(cross, dot));
            const std::uint16_t rotation = static_cast<std::uint16_t>(2 * (-0x4000 - static_cast<std::int32_t>(angle)));
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4 ::RotateAroundY_Wrapper(rotation);
            const Vector3 rotated{(workTransformMatrix.rows[0].x * currentX + workTransformMatrix.rows[1].x * 0.0f) + workTransformMatrix.rows[2].x * currentZ,
                (workTransformMatrix.rows[0].y * currentX + workTransformMatrix.rows[1].y * 0.0f) + workTransformMatrix.rows[2].y * currentZ,
                (workTransformMatrix.rows[0].z * currentX + workTransformMatrix.rows[1].z * 0.0f) + workTransformMatrix.rows[2].z * currentZ};
            const float scale = penetration / std::sqrt((rotated.y * rotated.y + rotated.x * rotated.x) + rotated.z * rotated.z);
            return {rotated.x - currentX + scale * rotated.x, 0.0f, rotated.z - currentZ + scale * rotated.z};
        }
        const float scale = penetration / std::sqrt(currentLengthSquared);
        return {scale * currentX, 0.0f, scale * currentZ};
    }

    const float previousLength = std::sqrt(previousX * previousX + previousZ * previousZ);
    return {previousX * (kContactDiameter / previousLength), 0.0f, previousZ * (kContactDiameter / previousLength)};
}


void vfSetNearbyFighterXZ(VFEnemySlot &fighter, float x, float z) {
    if (static_cast<double>(std::fabs(x)) < 0.00001)
        x = 0.0f;
    if (static_cast<double>(std::fabs(z)) < 0.00001)
        z = 0.0f;
    fighter.position.x += x;
    fighter.position.z += z;
    if (auto *const motion = static_cast<MotionInformationWork *>(fighter.motion)) {
        motion->rootWorldPosition.x += x;
        motion->rootWorldPosition.z += z;
    }
    if (fighter.character) {
        shenApplyTranslationToAll(fighter.character->entity.node, fighter.position);
    }
}

bool vfNearbyFighterUsesDirectedContact(const VFEnemySlot &fighter) {
    return fighter.wave && fighter.wave->contactType >= 3U && fighter.wave->contactType <= 5U;
}


void vfclApplyDeltaToContactHistory(VFCLTaskData &context, VFActor &actor, float x, float z) {
    for (Vector3 &probe : actor.contactProbePositions) {
        probe.x += x;
        probe.z += z;
    }
    context.previousContact[actor.slotIndex].x += x;
    context.previousContact[actor.slotIndex].z += z;
}

void vfclMoveEntityWithContactHistory(VFCLTaskData &context, VFActor &actor, float x, float z) {
    constexpr float epsilon = 0.00000011920929f;
    if (std::fabs(x) < epsilon && std::fabs(z) < epsilon)
        return;
    auto *const motion = static_cast<MotionInformationWork *>(actor.motion);
    motion->rootWorldPosition.x += x;
    motion->rootWorldPosition.z += z;
    actor.contactCurrentPosition.x += x;
    actor.contactCurrentPosition.z += z;
    actor.position.x += x;
    actor.position.z += z;
    if (actor.character) {
        Vector3 position = GetPositionForEntity(actor.character->entity.node);
        position.x += x;
        position.z += z;
        shenApplyTranslationToAll(actor.character->entity.node, position);
    }
    Vector2 &displacement = context.contactDisplacement[actor.slotIndex];
    displacement.x += x;
    displacement.y += z;
    vfclApplyDeltaToContactHistory(context, actor, x, z);
}

void vfclAdjustForSlideAndStick(VFCLTaskData &context) {
    VFActor &defender = *context.defender;
    VFActor &attacker = *context.attacker;
    if (context.contactType == 5U || context.contactType == 6U) {
        defender.contactShakeHistory[0] = std::max(defender.contactShakeAverage + context.specialPenetration, defender.groundY + 0.2f);
    } else {
        const Vector3 &defenderPosition = context.previousContact[defender.slotIndex];
        const Vector3 &attackerPosition = context.previousContact[attacker.slotIndex];
        float defenderX = context.separation.x * (-context.slideScale * context.penetration);
        float defenderZ = context.separation.z * (-context.slideScale * context.penetration);
        float attackerX = context.separation.x * (context.stickScale * context.penetration);
        float attackerZ = context.separation.z * (context.stickScale * context.penetration);
        const float dx = (attackerPosition.x + attackerX) - (defenderPosition.x + defenderX);
        const float dz = (attackerPosition.z + attackerZ) - (defenderPosition.z + defenderZ);
        const float distance = std::sqrt(dx * dx + dz * dz);
        if (distance > 0.00000011920929f) {
            const float averagedDistance = (distance + context.separationLength) * 0.5f;
            const float shiftX = averagedDistance * dx / distance - (attackerPosition.x - defenderPosition.x);
            const float shiftZ = averagedDistance * dz / distance - (attackerPosition.z - defenderPosition.z);
            defenderX = shiftX * -context.slideScale;
            defenderZ = shiftZ * -context.slideScale;
            attackerX = shiftX * context.stickScale;
            attackerZ = shiftZ * context.stickScale;
        }
        vfclMoveEntityWithContactHistory(context, defender, defenderX, defenderZ);
        vfclMoveEntityWithContactHistory(context, attacker, attackerX, attackerZ);
    }
    if (context.specialPenetration < 0.05f)
        defender.contactShakeHistory[0] = 0.0f;
}

} // namespace shenmue::s1::vf_detail

namespace shenmue::s1 {

using namespace vf_detail;

void vfclPrepareContact(VFCLTaskData *context, std::uint8_t aIndex, std::uint8_t bIndex) {
    VFActor &defender = *context->owner->actors[aIndex];
    VFActor &attacker = *context->owner->actors[bIndex];
    context->defender = &defender;
    context->attacker = &attacker;
    vfclFindContactType(context);
    if ((context->flags & 1U) != 0)
        return;
    context->attackerMask =
        (defender.hitStateFlags2 & 0x100U) != 0 && defender.currentAnimationEndFrame < defender.attackWindowEndFrame && defender.moveId != 41U ?
        kVFMoveCategoryMasks[defender.moveId] :
        0U;
    context->defenderMask =
        (attacker.hitStateFlags2 & 0x100U) != 0 && attacker.currentAnimationEndFrame < attacker.attackWindowEndFrame && attacker.moveId != 41U ?
        kVFMoveCategoryMasks[attacker.moveId] :
        0U;
    vfclComputePenetrationAndOverlap(context, &defender, &attacker);
    const float oldestShake = defender.contactShakeHistory[3];
    float shakeSum = 0.0f;
    for (int index = 3; index >= 0; --index) {
        shakeSum += defender.contactShakeHistory[index];
        if (index > 0)
            defender.contactShakeHistory[index] = defender.contactShakeHistory[index - 1];
    }
    defender.contactShakeAverage = shakeSum * 0.25f;
    if (context->penetration != 0.0f || context->specialPenetration != 0.0f || oldestShake != 0.0f)
        vfclAdjustForSlideAndStick(*context);
}

std::int64_t vfResolveAttackOrPush(VFCLParticipantTmp *participants, int self, int other) {
    if (!participants || self < 0 || other < 0 || self >= 16 || other >= 16 || self == other || participants[self].type != VFParticipant::Sub ||
        participants[other].type != VFParticipant::Main) {
        return 0;
    }

    auto *const nearby = static_cast<VFEnemySlot *>(participants[self].fcmg);
    VFActor *const actor = participants[other].actor;
    if (!nearby || !actor || !actor->game || (actor->hitStateFlags2 & 0x000c0000U) != 0) {
        return 0;
    }

    std::size_t contactType = 0;
    if (vfNearbyFighterUsesDirectedContact(*nearby)) {
        contactType = 1;
    } else if ((actor->hitStateFlags2 & 0x00010000U) != 0 || actor->collisionHitFlags != 0 || (actor->motionFlags & 0x00000c00U) != 0) {
        contactType = 2;
    }

    const float currentX = nearby->position.x - actor->contactCurrentPosition.x;
    const float currentZ = nearby->position.z - actor->contactCurrentPosition.z;
    if (currentX * currentX + currentZ * currentZ >= 0.64000005f) {
        return 0;
    }

    const Vector3 push =
        vfComputeNearbyFighterPush(currentX, currentZ, participants[self].x - participants[other].x, participants[self].z - participants[other].z);
    const VFContactConfig &config = kVFContactConfigurations[contactType];
    vfSetNearbyFighterXZ(*nearby, push.x * config.slideScale, push.z * config.slideScale);
    auto *const context = static_cast<VFCLTaskData *>(actor->game->collisionData);
    if (context) {
        vfclMoveEntityWithContactHistory(*context, *actor, -(push.x * config.stickScale), -(push.z * config.stickScale));
    }
    return 1;
}

void vfResolveMutualPush(VFCLParticipantTmp *participants, int entityA, int entityB) {
    if (!participants || entityA < 0 || entityB < 0 || entityA >= 16 || entityB >= 16 || entityA == entityB ||
        participants[entityA].type != VFParticipant::Sub || participants[entityB].type != VFParticipant::Sub) {
        return;
    }

    auto *const first = static_cast<VFEnemySlot *>(participants[entityA].fcmg);
    auto *const second = static_cast<VFEnemySlot *>(participants[entityB].fcmg);
    if (!first || !second)
        return;

    std::size_t contactType = vfNearbyFighterUsesDirectedContact(*first) ? 1U : 0U;
    if (vfNearbyFighterUsesDirectedContact(*second)) {
        if (contactType == 1U)
            return;
        contactType = 2U;
    }

    const float currentX = first->position.x - second->position.x;
    const float currentZ = first->position.z - second->position.z;
    if (currentX * currentX + currentZ * currentZ >= 0.64000005f) {
        return;
    }

    const Vector3 push =
        vfComputeNearbyFighterPush(currentX, currentZ, participants[entityA].x - participants[entityB].x, participants[entityA].z - participants[entityB].z);
    const VFContactConfig &config = kVFContactConfigurations[contactType];
    vfSetNearbyFighterXZ(*first, push.x * config.slideScale, push.z * config.slideScale);
    vfSetNearbyFighterXZ(*second, -(push.x * config.stickScale), -(push.z * config.stickScale));
}

void vfclFindContactType(VFCLTaskData *context) {
    const VFActor &defender = *context->defender;
    const VFActor &attacker = *context->attacker;
    const std::uint32_t defenderFlags = defender.hitStateFlags2;
    const std::uint32_t attackerFlags = attacker.hitStateFlags2;
    const std::uint32_t sharedFlags = defenderFlags | attackerFlags;
    const auto forcesAttackParity = [](std::uint16_t animation) {
        return animation == 483U || animation == 479U || animation == 581U || animation == 16U || animation == 254U;
    };
    std::uint32_t flags = 0;
    if (forcesAttackParity(defender.currentReactionAnimation) || forcesAttackParity(attacker.currentReactionAnimation)) {
        flags = 2U;
    } else if ((sharedFlags & 0xc0000U) == 0xc0000U) {
        flags = 1U;
    } else if (((attackerFlags ^ defenderFlags) & 0x100U) != 0) {
        if (defender.moveId != 41U && attacker.moveId != 41U)
            flags = 2U;
    } else if ((attackerFlags & defenderFlags & 0x100U) != 0) {
        flags = 4U;
    }
    if ((sharedFlags & 0x4000U) != 0)
        flags |= 8U;
    std::uint32_t type = 0;
    if ((flags & 5U) == 0) {
        if ((sharedFlags & 0x10000U) != 0) {
            if ((defenderFlags & 0x10000U) != 0) {
                type = (defenderFlags & 0x8000U) == 0 && (attackerFlags & 0x400000U) != 0 && (attacker.hitFlags & 0x1010U) != 0 ? 5U : 1U;
            } else {
                type = (attackerFlags & 0x8000U) == 0 && (defenderFlags & 0x400000U) != 0 && (defender.hitFlags & 0x1010U) != 0 ? 6U : 2U;
            }
        } else if (defender.terrainCollisionLatched) {
            type = 1U;
        } else if (attacker.terrainCollisionLatched) {
            type = 2U;
        } else if (defender.collisionHitFlags) {
            type = 1U;
        } else if (attacker.collisionHitFlags) {
            type = 2U;
        } else if ((defender.motionFlags & 0xc00U) != 0) {
            type = 1U;
        } else if ((attacker.motionFlags & 0xc00U) != 0) {
            type = 2U;
        } else if ((flags & 2U) != 0) {
            type = (defenderFlags & 0x100U) != 0 ? 3U - ((attacker.motionFlags & 0x204000U) != 0) : 4U;
        }
    }
    const VFContactConfig &config = kVFContactConfigurations[type];
    context->flags = flags;
    context->contactType = type;
    context->maskA = config.maskA;
    context->maskB = config.maskB;
    context->slideScale = config.slideScale;
    context->stickScale = config.stickScale;
    if (defender.currentReactionAnimation == 1007U || defender.currentReactionAnimation == 1500U)
        context->maskA &= 0xffff93ffU;
    if (attacker.currentReactionAnimation == 1007U || attacker.currentReactionAnimation == 1500U)
        context->maskB &= 0xffff93ffU;
}

void vfclComputePenetrationAndOverlap(VFCLTaskData *context, VFActor *defender, VFActor *attacker) {
    const auto *const attackVolumes = reinterpret_cast<const VFContactVolumeRecord *>(attacker->contactVolumeRecords);
    const auto *const defenseVolumes = reinterpret_cast<const VFContactVolumeRecord *>(defender->contactVolumeRecords);
    const bool special = context->contactType == 5U || context->contactType == 6U;
    float penetration = 0.0f;
    if (!special) {
        const Vector3 &currentA = context->currentContact[attacker->slotIndex];
        const Vector3 &previousA = context->previousContact[attacker->slotIndex];
        const Vector3 &currentB = context->currentContact[defender->slotIndex];
        const Vector3 &previousB = context->previousContact[defender->slotIndex];
        const Vector3 previousPositionA{currentA.x, previousA.y, currentA.z};
        const Vector3 previousPositionB{currentB.x, previousB.y, currentB.z};
        const float dx = previousPositionA.x - previousPositionB.x;
        const float dz = previousPositionA.z - previousPositionB.z;
        const float squaredLength = dx * dx + dz * dz;
        context->separation = {};
        context->separationLength = 0.0f;
        if (squaredLength >= 0.00000011920929f) {
            context->separationLength = std::sqrt(squaredLength);
            context->separation.x = dx / context->separationLength;
            context->separation.z = dz / context->separationLength;
        }
    }
    for (std::size_t a = 0; a < 10; ++a) {
        const VFContactVolumeRecord &volumeA = attackVolumes[a];
        const bool suppressPush =
            context->defenderMask != context->maskB && ((volumeA.mask & context->defenderMask) != 0 || (volumeA.mask & context->maskB) == 0);
        for (std::size_t b = 0; b < 10; ++b) {
            const VFContactVolumeRecord &volumeB = defenseVolumes[b];
            Vector3 delta{
                attacker->contactProbePositions[a].x - defender->contactProbePositions[b].x,
                attacker->contactProbePositions[a].y - defender->contactProbePositions[b].y,
                attacker->contactProbePositions[a].z - defender->contactProbePositions[b].z,
            };
            bool swept = false;
            float sweepLengthSquared = 0.0f;
            float sweepTime = 0.0f;
            if (!special) {
                const Vector3 previousDelta{
                    attacker->previousContactProbePositions[a].x - defender->previousContactProbePositions[b].x,
                    attacker->previousContactProbePositions[a].y - defender->previousContactProbePositions[b].y,
                    attacker->previousContactProbePositions[a].z - defender->previousContactProbePositions[b].z,
                };
                const Vector3 motionDelta{
                    delta.x - previousDelta.x,
                    delta.y - previousDelta.y,
                    delta.z - previousDelta.z,
                };
                const float approach = -(motionDelta.x * previousDelta.x + motionDelta.y * previousDelta.y + motionDelta.z * previousDelta.z);
                if (approach > 0.00000011920929f) {
                    sweepLengthSquared = motionDelta.x * motionDelta.x + motionDelta.y * motionDelta.y + motionDelta.z * motionDelta.z;
                    if (sweepLengthSquared > approach) {
                        swept = true;
                        sweepTime = approach / sweepLengthSquared;
                        delta = {
                            sweepTime * motionDelta.x + previousDelta.x,
                            sweepTime * motionDelta.y + previousDelta.y,
                            sweepTime * motionDelta.z + previousDelta.z,
                        };
                    }
                }
            }
            const float radius = volumeA.radius + volumeB.radius;
            const float radiusSquared = radius * radius;
            const float distanceSquared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            if (distanceSquared >= radiusSquared)
                continue;
            defender->contactProfileMasks[b] |= volumeA.mask;
            attacker->contactProfileMasks[a] |= volumeB.mask;
            if (special) {
                penetration = std::max(radius - std::sqrt(distanceSquared), penetration);
                continue;
            }
            if (suppressPush ||
                (context->attackerMask != context->maskA && ((volumeB.mask & context->attackerMask) != 0 || (volumeB.mask & context->maskA) == 0)))
                continue;
            float overlap;
            if (swept) {
                overlap = std::sqrt(radiusSquared - distanceSquared) + std::sqrt(sweepLengthSquared) * (1.0f - sweepTime);
            } else {
                const float along = delta.x * context->separation.x + delta.z * context->separation.z;
                overlap = std::sqrt(radiusSquared - (distanceSquared - along * along)) - std::fabs(along);
            }
            penetration = std::max(overlap, penetration);
        }
    }
    context->penetration = special ? 0.0f : penetration;
    context->specialPenetration = special ? penetration : 0.0f;
}

void vfclResolveHitVsHitOutcome(VFCLTaskData *context, std::uint8_t entityA, std::uint8_t entityB) {
    VFMAData *const game = context->owner;
    if (!game || entityA >= kBattleSlots || entityB >= kBattleSlots || entityA == entityB) {
        return;
    }
    VFActor *const first = game->actors[entityA];
    VFActor *const second = game->actors[entityB];
    if (!first || !second)
        return;
    vfclResolveHitVsHitOutcome(*first, *second);
}


bool vfclCheckEnvironmentObstruction(VFCLTaskData &context, const VFActor &first, const VFActor &second) {
    if (first.slotIndex >= kBattleSlots || second.slotIndex >= kBattleSlots) {
        return false;
    }
    const Vector3 &start = context.previousContact[first.slotIndex];
    const Vector3 &end = context.previousContact[second.slotIndex];
    if (std::fabs(end.x - start.x) < 0.00000011920929f && std::fabs(end.z - start.z) < 0.00000011920929f) {
        return false;
    }
    if (first.character)
        Matrix4x4_TranslateAndRotate(*reinterpret_cast<const CharacterIdTransform *>(&first.character->entity.node));
    CharacterVectorCollisionResult result{};
    COLS_TraceMapRay(start, end, result);
    const std::uint32_t hit = result.flags & 7U;
    return hit == 2U || (hit == 1U && result.fraction >= 0.0f && result.fraction < 1.0f);
}

void vfBuildContactsAndResolvePushOrHit(VFCLTaskData *context, VFCLParticipantTmp *participants, int participantLen, VFCLPhase phase) {
    for (int first = participantLen - 2; first >= 0; --first) {
        for (int second = first + 1; second < participantLen; ++second) {
            const VFCLParticipantTmp &a = participants[first];
            const VFCLParticipantTmp &b = participants[second];
            const bool linked = a.link != 0 || b.link != 0;
            if (linked != (phase == VFCLPhase::BroadPhase))
                continue;
            const Vector3 &positionA =
                a.type == VFParticipant::Main ? context->previousContact[a.actor->slotIndex] : static_cast<const VFEnemySlot *>(a.fcmg)->position;
            const Vector3 &positionB =
                b.type == VFParticipant::Main ? context->previousContact[b.actor->slotIndex] : static_cast<const VFEnemySlot *>(b.fcmg)->position;
            const float dx = positionB.x - positionA.x;
            const float dy = positionB.y - positionA.y;
            const float dz = positionB.z - positionA.z;
            if (dx * dx + dy * dy + dz * dz >= 4.41f)
                continue;
            if (a.type == VFParticipant::Main && b.type == VFParticipant::Main) {
                VFActor &actorA = *a.actor;
                VFActor &actorB = *b.actor;
                vfclPrepareContact(context, actorA.slotIndex, actorB.slotIndex);
                const bool paired = context->owner->slotTeamId[actorA.slotIndex] != context->owner->slotTeamId[actorB.slotIndex] ||
                    ((actorA.motionFlags | actorB.motionFlags) & 0x80000000U) != 0;
                actorA.linkFlags = paired ? 2U : 0U;
                actorB.linkFlags = paired ? 2U : 0U;
                if (actorA.fightStyleIndex == 0)
                    actorB.linkFlags |= 1U;
                if (actorB.fightStyleIndex == 0)
                    actorA.linkFlags |= 1U;
                if (paired && ((actorA.motionFlags | actorB.motionFlags) & 0x800U) == 0 && !vfclCheckEnvironmentObstruction(*context, actorA, actorB))
                    vfclResolveHitVsHitOutcome(context, actorA.slotIndex, actorB.slotIndex);
            } else if (a.type == VFParticipant::Main) {
                (void)vfResolveAttackOrPush(participants, second, first);
            } else if (b.type == VFParticipant::Main) {
                (void)vfResolveAttackOrPush(participants, first, second);
            } else {
                vfResolveMutualPush(participants, first, second);
            }
        }
    }
}

} // namespace shenmue::s1

namespace shenmue::s1::vf_detail {

const std::uint8_t *StageObjectCollisionMask();

} // namespace shenmue::s1::vf_detail

namespace shenmue::s1 {

using namespace vf_detail;

static_assert(offsetof(VFMAData, battleMeta) == 0xe0);

static_assert(offsetof(VFMAData, ringBoundary) == 0x2e0);

static_assert(offsetof(VFActor, stateFlags) == 0x08);

static_assert(offsetof(VFActor, groundY) == 0x458);

static_assert(offsetof(VFActor, terrainCollisionLatched) == 0x905);

static_assert(offsetof(VFActor, collisionHitFlags) == 0x9a8);

static_assert(offsetof(VFActor, field9ae) == 0x9ae);

static_assert(offsetof(VFEnemySlot, positionX) == 0xdc);

static_assert(offsetof(VFEnemySlot, shouldSetPosition) == 0xe4);


void vfclEnforceRingBoundary(VFCLTaskData *context) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    VFMAData *const game = context->owner;
    const float radius = game->ringBoundary[0];
    if (epsilon > radius)
        return;
    const float centerX = game->ringBoundary[1];
    const float centerZ = game->ringBoundary[2];
    for (std::size_t index = 0; index < kBattleSlots; ++index) {
        if (!game->characters[index])
            continue;
        VFActor &actor = *game->actors[index];
        context->ringContactFlags[index] = 0;
        const Vector3 &contact = context->previousContact[index];
        const float offsetX = contact.x - centerX;
        const float offsetZ = contact.z - centerZ;
        const float distanceSquared = offsetZ * offsetZ + offsetX * offsetX;
        const float inner = radius - kVFContactRadius;
        if (inner * inner > distanceSquared)
            continue;
        context->ringContactFlags[index] = 1;
        const float distance = std::sqrt(distanceSquared);
        const float outer = kVFContactRadius + distance;
        if (!(outer > 0.0f) || epsilon > outer)
            continue;
        const float overshoot = static_cast<float>(static_cast<double>(distance) - (static_cast<double>(radius - kVFContactRadius) - 0.01));
        if (epsilon > overshoot)
            continue;
        const float x = centerX - contact.x;
        const float z = centerZ - contact.z;
        const float scale = overshoot / std::sqrt(x * x + 0.0f + z * z);
        vfclMoveEntityWithContactHistory(*context, actor, scale * x, scale * z);
        actor.stateFlags |= 1U;
    }
}



void vfclAdjustToGroundOrSlope(VFCLTaskData *context) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    VFMAData *const game = context->owner;
    for (std::size_t index = 0; index < kBattleSlots; ++index) {
        if (!game->characters[index])
            continue;
        const Vector3 &position = context->previousContact[index];
        const Vector3 &previous = context->currentContact[index];
        float directionX = position.x - previous.x;
        float directionZ = position.z - previous.z;
        if (std::fabs(directionX) < epsilon && std::fabs(directionZ) < epsilon)
            continue;
        VFActor &actor = *game->actors[index];
        SelectCharacterMapQuery(actor.character);
        const float length = std::sqrt(directionX * directionX + directionZ * directionZ);
        if (length > 0.0f) {
            const float inverse = 1.0f / length;
            directionX *= inverse;
            directionZ *= inverse;
        }
        const float stepX = static_cast<float>(static_cast<double>(directionX) * 0.2);
        const float stepZ = static_cast<float>(static_cast<double>(directionZ) * 0.2);
        Vector3 probe{position.x + stepX * 3.0f, static_cast<float>(static_cast<double>(position.y) + 1.5), position.z + stepZ * 3.0f};
        const float first = shenCheckIntersectionWithOffset(actor.character, probe) - actor.groundY;
        probe = {probe.x + stepX, probe.y + first, probe.z + stepZ};
        const float second = shenCheckIntersectionWithOffset(actor.character, probe) - actor.groundY;
        bool blocked = std::fabs(first - second) > 0.5f;
        if (!blocked) {
            probe = {probe.x + stepX, probe.y + second, probe.z + stepZ};
            const float third = shenCheckIntersectionWithOffset(actor.character, probe) - actor.groundY;
            blocked = std::fabs(second - third) > 0.5f;
        }
        if (!blocked)
            continue;
        vfclMoveEntityWithContactHistory(*context, actor, previous.x - position.x, previous.z - position.z);
        actor.stateFlags |= 1U;
    }
}



void vfResolveTerrainColi(VFCLTaskData *context) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    constexpr std::uint32_t kTerrainResolvePending = 0x40U;
    constexpr std::uint32_t kPositionAdjustedByEnvironment = 0x1U;
    constexpr std::uint32_t kTerrainHit = 0x1U;
    constexpr std::uint32_t kAirborneOrFalling = 0x8000U;
    constexpr std::uint16_t kDekuStyle = 21;
    VFMAData *const game = context->owner;
    for (std::size_t index = 0; index < kBattleSlots; ++index) {
        if (!game->characters[index])
            continue;
        VFActor *const actor = game->actors[index];
        if (!actor || actor->fightStyleIndex == kDekuStyle || (actor->stateFlags & kTerrainResolvePending) == 0)
            continue;
        actor->stateFlags &= ~kTerrainResolvePending;
        SelectCharacterMapQuery(actor->character);
        const Vector3 &position = context->previousContact[index];
        const Vector3 &previous = context->currentContact[index];
        if (std::fabs(position.x - previous.x) < epsilon && std::fabs(position.z - previous.z) < epsilon)
            continue;
        CharacterVectorCollisionResult result{};
        COLS_TraceMapVector(previous, position, kVFContactRadius, result);
        const std::uint32_t hit = result.flags & 7U;
        float x = 0.0f;
        float z = 0.0f;
        if (hit == 1U) {
            if (!(1.0f > result.fraction) || result.fraction < 0.0f)
                continue;
            actor->collisionHitFlags |= kTerrainHit;
            const float remaining = static_cast<float>(1.0 - static_cast<double>(result.fraction));
            x = (previous.x - position.x) * remaining;
            z = (previous.z - position.z) * remaining;
            actor->field9ae = static_cast<std::uint16_t>(result.angle);
        } else if (hit == 2U) {
            context->terrainOverlapFlags[index] |= kTerrainHit;
            actor->collisionHitFlags |= kTerrainHit;
            x = previous.x - position.x;
            z = previous.z - position.z;
            actor->field9ae = 0;
        } else {
            continue;
        }
        actor->stateFlags |= kPositionAdjustedByEnvironment;
        if (static_cast<double>(std::fabs(x)) < 0.00001)
            x = 0.0f;
        if (static_cast<double>(std::fabs(z)) < 0.00001)
            z = 0.0f;
        if (!(z * z + x * x > 0.0f))
            continue;
        const std::uint16_t angle = actor->field9ae;
        actor->moveFlags |= kAirborneOrFalling;
        const float wallCosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - (angle <= 0x8000U ? angle : 0x10000 - angle)));
        x *= wallCosine;
        z *= wallCosine;
        if (std::fabs(x) < epsilon && std::fabs(z) < epsilon)
            continue;
        const auto negativeAngle = static_cast<std::uint16_t>(-angle);
        const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - (negativeAngle <= 0x8000U ? negativeAngle : 0x10000 - negativeAngle)));
        const float sine = sin_fixed16(static_cast<std::int16_t>(negativeAngle));
        const float rotatedX = cosine * x - z * sine;
        const float rotatedZ = cosine * z + x * sine;
        float directionX = rotatedX;
        float directionZ = rotatedZ;
        const float length = std::sqrt(rotatedX * rotatedX + rotatedZ * rotatedZ);
        if (length > 0.0f) {
            const float inverse = 1.0f / length;
            directionX = inverse * rotatedX;
            directionZ = inverse * rotatedZ;
        }
        float moveX = static_cast<float>(static_cast<double>(directionX) * 0.01 + static_cast<double>(rotatedX));
        float moveZ = static_cast<float>(static_cast<double>(directionZ) * 0.01 + static_cast<double>(rotatedZ));
        QueryShape query{.mode = SimpleRadial_CircleXZ_CalculateDistanceWithinRange, .packed = {{kVFContactRadius, moveX + position.x, moveZ + position.z}}};
        if (ExecuteMathFnWrapper(&query, StageObjectCollisionMask())) {
            const VFEnemySlot &slot = *game->battleMeta.groups[index];
            moveX = slot.shouldSetPosition ? slot.positionX - position.x : 0.0f;
            moveZ = slot.shouldSetPosition ? slot.positionZ - position.z : 0.0f;
            actor->terrainCollisionLatched = 1;
        } else if (context->ringContactFlags[index]) {
            actor->terrainCollisionLatched = 1;
        }
        if (std::fabs(moveX) >= epsilon || std::fabs(moveZ) >= epsilon)
            vfclMoveEntityWithContactHistory(*context, *actor, moveX, moveZ);
    }
}

VFMAData *vfclResolvePairedContactDrift(VFMAData *game) {
    if (!game || !game->collisionData)
        return game;
    auto *const context = static_cast<VFCLTaskData *>(game->collisionData);
    constexpr float epsilon = 0.00000011920929f;
    constexpr std::uint32_t kAirborneOrDowned = 0x00080000U;
    constexpr std::uint32_t kMidairRecoveryActive = 0x00040000U;
    constexpr std::uint32_t kStageObjectContact = 0x20U;
    constexpr std::uint32_t kTerrainResolvePending = 0x40U;
    for (std::size_t index = 0; index < kBattleSlots; ++index) {
        if (!game->characters[index])
            continue;
        VFActor *const actor = game->actors[index];
        if (!actor || (actor->hitStateFlags2 & kAirborneOrDowned) == 0 || (actor->motionFlags & 0x80U) != 0) {
            continue;
        }
        VFActor *const source = actor->hitSourceActor;
        if (!source || (source->hitStateFlags2 & kMidairRecoveryActive) == 0) {
            continue;
        }
        const std::size_t sourceIndex = source->slotIndex;
        if (sourceIndex >= kBattleSlots)
            continue;
        const Vector2 actorDelta = context->contactDisplacement[index];
        const Vector2 sourceDelta = context->contactDisplacement[sourceIndex];
        const bool actorFlag = context->ringContactFlags[index] != 0;
        const bool sourceFlag = context->ringContactFlags[sourceIndex] != 0;
        const bool actorBlocked = actor->collisionHitFlags != 0 || (actor->stateFlags & kStageObjectContact) != 0;
        const bool sourceBlocked = source->collisionHitFlags != 0 || (source->stateFlags & kStageObjectContact) != 0;

        if (actorFlag && sourceFlag && !actorBlocked && !sourceBlocked) {
            const bool hasDelta = std::fabs(actorDelta.x) >= epsilon || std::fabs(actorDelta.y) >= epsilon || std::fabs(sourceDelta.x) >= epsilon ||
                std::fabs(sourceDelta.y) >= epsilon;
            if (!hasDelta)
                continue;
            const float actorMagnitude = actorDelta.x * actorDelta.x + actorDelta.y * actorDelta.y;
            const float sourceMagnitude = sourceDelta.x * sourceDelta.x + sourceDelta.y * sourceDelta.y;
            if (actorMagnitude <= sourceMagnitude) {
                vfclMoveEntityWithContactHistory(*context, *actor, sourceDelta.x - actorDelta.x, sourceDelta.y - actorDelta.y);
                actor->stateFlags |= kTerrainResolvePending;
            } else {
                vfclMoveEntityWithContactHistory(*context, *source, actorDelta.x - sourceDelta.x, actorDelta.y - sourceDelta.y);
                source->stateFlags |= kTerrainResolvePending;
            }
            continue;
        }

        if (!actorBlocked && !actorFlag) {
            vfclMoveEntityWithContactHistory(*context, *actor, sourceDelta.x - actorDelta.x, sourceDelta.y - actorDelta.y);
            actor->stateFlags |= kTerrainResolvePending;
            continue;
        }
        if (!sourceBlocked && !sourceFlag) {
            vfclMoveEntityWithContactHistory(*context, *source, actorDelta.x - sourceDelta.x, actorDelta.y - sourceDelta.y);
            source->stateFlags |= kTerrainResolvePending;
        }
    }
    return game;
}

VFActor *barelyUsed(VFMAData *game) {
    if (!game || !game->collisionData)
        return nullptr;
    VFCLTaskData *const context = static_cast<VFCLTaskData *>(game->collisionData);
    VFActor *result = nullptr;
    for (std::size_t index = 0; index < kBattleSlots; ++index) {
        if (!game->characters[index])
            continue;
        const Vector2 &delta = context->contactDisplacement[index];
        if (std::fabs(delta.x) < 0.00000011920929f && std::fabs(delta.y) < 0.00000011920929f) {
            continue;
        }
        VFActor *const actor = game->actors[index];
        result = actor;
        if (!actor || !actor->motion)
            continue;
        MotionInformationWork *const motion = static_cast<MotionInformationWork *>(actor->motion);
        MotionNodeRuntime *node = static_cast<MotionNodeRuntime *>(motion->previousNodes);
        if (!node)
            return actor;
        for (; node->nodeId != 0xffU; ++node) {
            if (!node->transform)
                continue;
            node->transform->rows[3].x += delta.x;
            node->transform->rows[3].z += delta.y;
        }
    }
    return result;
}

} // namespace shenmue::s1
