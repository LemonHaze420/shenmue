#include "s2/fight_stage.hpp"
#include "s2/fight.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/map_control.hpp"
#include "s2/input.hpp"
#include "s2/math.hpp"

#include "s2/frame_pacing.hpp"
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include "s2/area_names.hpp"

namespace shenmue::s2 {
namespace {

std::int32_t Animation(const VFActor *self, std::size_t offset) {
    const auto *data = self->motionParameters[4] + offset;
    return int(static_cast<std::int16_t>(std::uint16_t(std::to_integer<std::uint8_t>(data[0])) << 8)) | std::to_integer<std::uint8_t>(data[1]);
}
std::int16_t DecrementHold(VFActor *self) {
    auto timer = static_cast<std::int16_t>(static_cast<std::uint16_t>(self->parameter40c) - 1);
    if (timer < 0)
        timer = 0;
    self->parameter40c = (self->parameter40c & 0xffff0000U) | static_cast<std::uint16_t>(timer);
    return timer;
}
void Install(VFActor *self, std::int32_t animation) {
    self->pendingAnimation = animation;
    vfApplyHitReactionAndStartMotion(self);
}
CameraMotionMode savedPairCameraMode{};


// @verified - p
void UpdateInstallMotion(VFActor *self) {
    if (!self->hitPhase) {
        Install(self, static_cast<std::int16_t>(self->queuedAction));
        ++self->hitPhase;
    }
    if (!GetMotionAnimationId(self->motion, MotionRegion::LowerBody))
        self->queuedAction = 0;
}

// @verified - p
void UpdateDefaultReaction(VFActor *self) {
    if ((self->hitStateFlags & VFHSF_AirborneOrDowned) && self->secondActor && (self->secondActor->moveFlags & VFMF_AutoAdvanceMotion)) {
        vfSelectDefaultMotion(self, 0);
        return;
    }
    if ((self->hitStateFlags & VFHSF_AttackGuardWindow) && (self->currentAnimation & 0xfff) == self->defaultAnimations[0]) {
        vfResetScriptedFormationMotion(self);
        if ((self->savedSecondaryMotionFlags & VFSMF_AlternateFootTarget) && static_cast<std::uint16_t>(self->motionTimer))
            GetParentBoneWrapper(self->motion, ((self->stateFlags & VFSF_UseLeftFootTarget) ? MB_LeftFootTarget : MB_RightFootTarget) | motionTargetIgnoreY);
        return;
    }
    if ((!self->requestedAnimation && (self->moveFlags & VFMF_NeedsAnimationStep)) ||
        ((!(self->secondaryMotionFlags & VFSMF_Unknown8000000) || !(self->motionFlags & VFMOF_Unknown4) ||
             float(self->reactionEndFrame) <= self->animationTime) &&
            ((self->motionFlags & VFMOF_Unknown2000) ||
                (self->secondaryMotionFlags & (VFSMF_AlternateFootTarget | VFSMF_Unknown8000000 | VFSMF_Unknown40000000)) ||
                (self->hitStateFlags & VFHSF_Unknown4)) &&
            !(self->secondaryMotionFlags & VFSMF_Unknown200000))) {
        vfResetScriptedFormationMotion(self);
        if (self->secondaryMotionFlags & VFSMF_AlternateFootTarget) {
            self->stateFlags ^= VFSF_UseLeftFootTarget;
            GetParentBoneWrapper(self->motion, ((self->stateFlags & VFSF_UseLeftFootTarget) ? MB_LeftFootTarget : MB_RightFootTarget) | motionTargetIgnoreY);
        }
        vfSelectDefaultMotion(self, 0);
    }
}

// @verified - p
void UpdateRecovery(VFActor *self) {
    const auto orientation = self->footFacingBytes[1];
    const auto action = self->queuedAction;
    switch (self->hitPhase) {
    case 0:
        self->hitStateFlags |= VFHSF_GuardState;
        self->reactionContinuation = nullptr;
        self->hitPhase = 1;
        [[fallthrough]];
    case 1: {
        if (self->animationTime <= float(self->reactionEndFrame))
            return;
        const auto animation = (self->motionFlags & VFMOF_Unknown4000000) ? std::uint16_t(((self->motionFlags & VFMOF_ContactCategoryB) ? 0 : 1) | 0x1c4) :
                                                                            static_cast<std::uint16_t>(Animation(self, (orientation & 1) ? 66 : 68));
        Install(self, animation);
        if ((self->moveFlags & VFMF_ForceDefeat) || self->hp <= 0) {
            self->hitPhase = 5;
            return;
        }
        self->knockbackAcceleration = 1 - float(std::bit_cast<std::int16_t>(self->hitPhaseWords[0])) * 0.03f;
        if (self->knockbackAcceleration < 0.5f)
            self->knockbackAcceleration = 0.5f;
        ++self->hitPhase;
        self->hitPhaseWaitFrames = std::uint16_t{};
        [[fallthrough]];
    }
    case 2: {
        const auto hold = std::bit_cast<std::int16_t>(self->hitPhaseWords[0]);
        self->hitPhaseWords[0] = static_cast<std::uint16_t>(hold - 1);
        if (hold > int(float(4 * self->hitPhaseWaitFrames) * self->inverseScale))
            return;
        self->moveFlags &= ~VFMF_HitReactionLock;
        self->stateFlags &= ~VFSF_Unknown200;
        ++self->hitPhase;
        self->hitPhaseWords[0] = std::uint16_t{};
        self->tickFrameTime = 0;
        self->parameter450 = 0;
        [[fallthrough]];
    }
    case 3: {
        self->moveFlags &= ~(VFMF_MirrorReactionAngle | VFMF_Unknown20000000);
        auto next = action & 0xfffffffffffffe0fULL;
        std::uint16_t index = 0;
        if (!(orientation & 1)) {
            next |= 0x200;
            index = 1;
        }
        if (!(orientation & 2)) {
            next |= 0x400;
            index += 2;
        }
        constexpr std::uint16_t offsets[4]{70, 74, 72, 76};
        Install(self, Animation(self, offsets[index]));
        ++self->hitPhase;
        self->queuedAction = next;
        [[fallthrough]];
    }
    case 4:
        if (self->hitStateFlags & VFHSF_Unknown4)
            self->queuedAction = 0;
        break;
    default:
        break;
    }
}

// @verified - p
void UpdateLandingHold(VFActor *self) {
    self->contactTimer += self->scale * 0.4f;
    const auto timer = DecrementHold(self);
    const float middle = float(static_cast<std::uint16_t>(self->reactionMiddleFrame)) - self->scale;
    switch (self->hitPhase) {
    case 0: {
        self->moveFlags |= VFMF_HitReactionLock;
        if (self->queuedActionState != 1) {
            self->knockbackVelocity.x = self->pendingKnockback.x * 1.5f;
            self->knockbackVelocity.z = self->pendingKnockback.z * 1.5f;
            self->slideVelocity.x = self->pendingKnockback.x * 4;
            self->slideVelocity.z = self->pendingKnockback.z * 4;
        }
        const auto animation = static_cast<std::int16_t>(self->queuedAction);
        self->pendingAnimation = animation;
        if (animation) {
            self->knockbackAcceleration = self->game->knockbackAmount;
            vfApplyHitReactionAndStartMotion(self);
            mtwkSetExtraFlag(self->motion, 0, 2);
        }
        if (self->motionFlags & VFMOF_Unknown10) {
            vfSnapActorToContact(self);
            self->knockbackVelocity.x += self->pendingKnockback.x;
            self->knockbackVelocity.y += self->pendingKnockback.y;
            self->knockbackVelocity.z += self->pendingKnockback.z;
            vfPredictLandingGround(self);
        }
        ++self->hitPhase;
        [[fallthrough]];
    }
    case 1:
        if (middle > self->animationTime)
            return;
        ++self->hitPhase;
        [[fallthrough]];
    case 2:
        if (!(self->moveFlags & VFMF_Airborne) && timer > self->reactionEndFrame) {
            self->animationTime = self->parameter4c8 = middle;
            return;
        }
        ++self->hitPhase;
        self->moveFlags &= ~VFMF_HitReactionLock;
        [[fallthrough]];
    case 3:
        if (timer <= 0) {
            self->stateFlags &= ~VFSF_Unknown200;
            self->queuedAction = 0;
            if (!(self->hitStateFlags & VFHSF_Unknown1000000))
                self->hitStateFlags &= ~VFHSF_ReactionActive;
            mtwkSetExtraFlag(self->motion, 0, 0);
        }
        break;
    default:
        break;
    }
}


// @verified - p
void UpdateApproachContact(VFActor *self) {
    if (self->hitPhase != 1 && self->hitPhase != 2) {
        Install(self, static_cast<std::int16_t>(self->queuedAction));
        self->contactVectors[1] = self->actor->contactPosition;
        ++self->hitPhase;
    }
    if (self->hitPhase != 2) {
        if (float(static_cast<std::uint16_t>(self->parameter2e2)) > self->animationTime)
            return;
        const auto point = self->contactVectors[1];
        float x = point.x - self->contactPosition.x;
        const float z = point.z - self->contactPosition.z;
        const auto angle = self->contactAngles[self->actionIndex];
        self->moveFlags |= VFMF_Unknown200;
        self->parameter1b6 = static_cast<std::uint16_t>(angle);
        const float distance = std::sqrt(x * x + z * z);
        if (distance > self->impulseMagnitude)
            x *= self->impulseMagnitude / distance;
        const float rotatedX = cos_fixed16(static_cast<std::uint16_t>(angle)) * self->hitVector.x - sin_fixed16(angle) * self->hitVector.y;
        ++self->hitPhase;
        const float step = (x - rotatedX) / (float(static_cast<std::uint16_t>(self->parameter2d0)) - self->animationTime + self->scale);
        self->parameter4fc.x = self->parameter4fc.y = step;
    }
    if (float(static_cast<std::uint16_t>(self->parameter2d0)) < self->animationTime)
        self->queuedAction = 0;
    else {
        self->position.x += self->parameter4fc.x;
        self->position.z += self->parameter4fc.y;
    }
}

// @verified - p
void UpdateLinkedAttackWait(VFActor *self) {
    auto *const linked = self->pairedActor;
    if (self->hitPhase != 1) {
        self->hitReaction = linked->currentAnimation;
        self->hitPhase = 1;
        self->hitStateFlags &= ~VFHSF_ReactionActive;
        self->queuedAction = 0;
    } else if (!(linked->hitStateFlags & 0x100) || !linked->currentAnimation) {
        self->hitStateFlags &= ~VFHSF_ReactionActive;
        self->queuedAction = 0;
    } else {
        self->moveFlags |= VFMF_Unknown1000;
        UpdateDefaultReaction(self);
    }
}

// @verified - p
void UpdateContactAttackAdmission(VFActor *self) {
    self->hitStateFlags &= ~VFHSF_AttackActive;
    if (float(static_cast<std::uint16_t>(self->parameter2d0)) > self->animationTime)
        return;
    if (self->contactDistances[self->actionIndex] <= self->impulseMagnitude) {
        const auto animation = static_cast<std::int16_t>(self->queuedAction);
        self->pendingAnimation = animation;
        self->queuedAction = static_cast<std::uint32_t>(animation + 0x09000000);
        vfApplyHitReactionAndStartMotion(self);
        ++self->hitPhase;
        self->knockbackVelocity = {};
    } else
        self->queuedAction = 0;
}

// @verified - p
void UpdatePairedTiming(VFActor *self) {
    auto *const linked = self->pairedActor;
    if (((linked->queuedAction >> 24) & 0xff) == 9)
        self->animationTime = self->parameter4c8 = float(self->eventFrame318);
    else {
        const float duration = self->pairTimeOffset - self->pairAnimationTime;
        const float ticks = self->tickFrameTime + shenGetScaledDeltaTickTime(1);
        self->animationTime = self->parameter4c8 = ((float(self->eventFrame318) - 1) * ticks) / std::fmax(0, ticks + duration) + 1;
        if (!vfCheckAnimTimeWithin(self, float(self->eventFrame318)))
            return;
    }
    self->hitStateFlags |= VFHSF_MidAirRecovery;
    self->queuedAction = 0x08000000;
    self->knockbackVelocity = {};
    self->hitPhase = 1;
}

// @verified - p
void UpdateReverseRecovery(VFActor *self) {
    if (self->hitPhase == 1) {
        if (self->animationTime >= 9)
            return;
        self->hitStateFlags |= VFHSF_Unknown4;
        self->hitPhase = 2;
    } else if (self->hitPhase != 2) {
        vfApplyHitReactionAndStartMotion(self);
        self->hitStateFlags &= ~VFHSF_MidAirRecovery;
        ++self->hitPhase;
        return;
    }
    self->animationTime -= self->scale + self->scale;
    if (self->animationTime <= 0)
        self->animationTime = 0;
    self->parameter4c8 = self->animationTime;
    if (self->animationTime <= 0) {
        self->hitStateFlags &= ~VFHSF_ReactionActive;
        self->queuedAction = 0;
        self->currentAnimationEndFrame = 1;
    }
}

// @verified - p
void UpdateTimedGuard(VFActor *self) {
    VFActor *const other = self->actor;
    if (!self->hitPhase) {
        Install(self, static_cast<std::int16_t>(self->queuedAction));
        const float remaining = float(static_cast<std::uint16_t>(other->parameter2d0)) - other->animationTime;
        const float first = float(static_cast<std::uint16_t>(self->parameter2d0));
        if (first - self->scale > remaining)
            self->animationTime = first - remaining;
        self->parameter4c8 = self->animationTime;
        vfPlayFightSfx(35, self, 0);
        ++self->hitPhase;
    } else if (self->hitPhase == 1 && self->animationTime >= float(static_cast<std::uint16_t>(self->reactionMiddleFrame))) {
        if ((other->hitStateFlags & VFHSF_Unknown4) || !(other->hitStateFlags & VFHSF_AttackActive))
            ++self->hitPhase;
        else
            self->animationTime -= self->scale;
    } else if (self->hitPhase == 2 && self->animationTime >= float(self->reactionEndFrame))
        self->queuedAction = 0;
}

// @verified - p
void UpdateReactionHold(VFActor *self) {
    self->contactTimer += 0.8f * self->scale;
    DecrementHold(self);
    if (!self->hitPhase) {
        self->pendingAnimation = static_cast<std::uint16_t>(self->queuedAction);
        self->knockbackAcceleration = self->timeScale;
        self->knockbackVelocity.x = self->knockbackVelocity.z = self->slideVelocity.x = self->slideVelocity.z = 0;
        vfApplyHitReactionAndStartMotion(self);
        self->hitStateFlags |= VFHSF_ReactionActive;
        if (!self->actionTriggerPhase) {
            self->parameter2d0 = 0;
            self->reactionMiddleFrame = static_cast<std::int16_t>(int(self->animationTimeParameter));
            self->reactionEndFrame = static_cast<std::uint16_t>(self->reactionMiddleFrame);
        }
        ++self->hitPhase;
    } else if (self->hitPhase == 3) {
        self->queuedAction = 0;
        self->hitStateFlags &= ~VFHSF_ReactionActive;
    } else if (self->hitPhase == 1 || self->hitPhase == 2) {
        if (self->hitPhase == 1) {
            if (float(static_cast<std::uint16_t>(self->reactionMiddleFrame)) > self->animationTime)
                return;
            ++self->hitPhase;
        }
        if (self->stateFlags & VFSF_Unknown800) {
            self->queuedAction = 0;
            self->hitStateFlags &= ~VFHSF_ReactionActive;
            self->stateFlags &= ~VFSF_Unknown800;
        } else if (float(self->reactionEndFrame) <= self->animationTime) {
            self->queuedAction = 0;
            self->hitStateFlags &= ~VFHSF_ReactionActive;
        }
    }
}

// @verified - p
void UpdateAuthoredDefeat(VFActor *self) {
    if (!self->hitPhase) {
        self->moveFlags |= VFMF_HitReactionLock;
        if (self->hp <= 0)
            self->hitPhase = 2;
        if ((self->hitStateFlags & (VFHSF_AttackGuardWindow | VFHSF_SpecialGuardOrParry | VFHSF_GuardState | VFHSF_AlignedToGround)) == 0x200) {
            mtwkSetExtraFlag(self->motion, 0, 0);
            motRemoveCallback(self->motion);
            SetMotionSpeed(self->motion, 1, MotionRegion::LowerBody);
            ++self->hitPhase;
        }
    } else if (self->hitPhase == 1) {
        const int frame = int(self->game->frame) % 3;
        Install(self, frame == 0 ? 395 : (frame == 1 ? 397 : (frame == 2 ? 399 : 0)));
        ++self->hitPhase;
    }
}

// @verified - p
void UpdateLocomotion(VFActor *self) {
    auto previous = self->relativeInputSectors[0];
    auto mode = self->reactionMovement.state.mode;
    bool keepMotion = false, stationary = false;
    if (self->fightStyle == 1) {
        mode = 1;
        self->reactionMovement.state.mode = mode;
    }
    if (mode == 3) {
        if (!(self->cameraControlFlags & 0xf0)) {
            previous = 0;
            self->cameraControlFlags = 32;
        }
        const auto &game = *self->game;
        const auto &input = g_inputBuffer[(game.gameFlags & 0x40000) ? self->groupIndex : 0];
        if (((game.gameFlags & 0x40000) || !game.slotPoolModes[self->groupIndex]) && input.analog0d < 250 && input.analog0c < 250) {
            mode = 1;
            self->reactionMovement.state.mode = mode;
        }
    }
    if (mode == 2)
        self->cameraControlFlags = 0;
    std::int32_t animation;
    if (self->cameraControlFlags & 0xf0) {
        if ((self->activeMotionFlags & VFSMF_Unknown8000000) && (self->reactionEvaluationPhase != 1 || (self->motionFlags & VFMOF_Unknown4)) &&
            (mode != 3 || (self->motionFlags & VFMOF_Unknown20000000))) {
            const auto id = self->currentAnimation & 0xfff;
            if (id != 61 && id != 286) {
                constexpr std::int16_t turns[8]{0, 1024, 2048, 3072, 4096, -3072, -2048, -1024};
                self->facingAngle = static_cast<std::uint16_t>(self->facingAngle + int(float(turns[previous]) * self->scale));
            }
            animation = Animation(self, mode == 1 ? 314 : 316);
            if (SetMotionRegionFlag(self->motion, 65, 1, MotionRegion::LowerBody))
                keepMotion = self->requestedAnimation == -1;
            else if (!(self->motionFlags & VFMOF_Unknown4))
                return;
        } else if (self->fightStyle == 0 && previous == 4)
            animation = mode == 1 ? 61 : 286;
        else
            animation = Animation(self, mode == 1 ? 314 : 316);
    } else {
        stationary = true;
        if ((self->motionFlags & VFMOF_Unknown4) ||
            ((self->motionFlags & VFMOF_Unknown2) && !SetMotionRegionFlag(self->motion, 65, 1, MotionRegion::LowerBody)))
            return;
        if (self->fightStyle == 0 && (float(self->initialHp) * 0.5f > self->hp || self->parameterB24 > 0))
            animation = (self->moveFlags & VFMF_MirroredStance) ? 32826 : 59;
        else
            animation = Animation(self, (self->moveFlags & VFMF_MirroredStance) ? 320 : 318);
        if (self->reactionMovement.state.mode == 1)
            animation = self->defaultAnimations[0];
    }
    self->moveFlags &= ~VFMF_MirrorReactionAngle;
    self->knockbackAcceleration = self->timeScale;
    self->pendingAnimation = animation;
    self->knockbackAcceleration = self->timeScale * self->contactParameter;
    if (!stationary) {
        self->motionFlags &= ~VFMOF_Unknown200000;
        self->hitStateFlags &= ~VFHSF_MirroredReactionAngle;
    }
    if (!keepMotion)
        vfApplyHitReactionAndStartMotion(self);
    self->queuedAction = 0;
}

// @verified - p
void UpdateTargetLocomotion(VFActor *self) {
    VFActor *const target = self->game->actors[self->actionIndex];
    const auto direction =
        static_cast<std::int16_t>(FixedAtan2(self->contactPosition.x - self->parameterA78, self->contactPosition.z - std::bit_cast<float>(self->parameterA80)));
    if (self->targetCache != -1)
        self->reactionMovement.state.phase = std::uint8_t{};
    const auto difference = static_cast<std::int16_t>(direction - self->facingAngle);
    auto turn = self->reactionMovement.state.phase;
    if (turn) {
        if (std::fabs(float(difference)) <= 4096)
            turn = 0;
    } else if (difference > 4096)
        turn = 1;
    else if (difference < -4096)
        turn = 2;
    self->reactionMovement.state.phase = turn;
    if (turn == 1)
        self->facingAngle = static_cast<std::uint16_t>(self->facingAngle + 4096);
    if (turn == 2)
        self->facingAngle = static_cast<std::uint16_t>(self->facingAngle - 4096);
    const bool alternate = self->reactionMovement.state.mode != 1;
    const auto animation = Animation(self, alternate ? 316 : 314);
    if (!(self->activeMotionFlags & VFSMF_Unknown8000000) || (self->motionFlags & VFMOF_Unknown4) ||
        SetMotionRegionFlag(self->motion, 65, 1, MotionRegion::LowerBody) || (!(self->motionFlags & VFMOF_ContactCategoryA) != alternate)) {
        self->moveFlags &= ~VFMF_MirrorReactionAngle;
        self->pendingAnimation = animation;
        if (alternate)
            self->knockbackAcceleration = self->timeScale *
                (((target->hitStateFlags & VFHSF_CameraFocusTarget) && (target->groundContactFlags & 1) && !(self->groundContactFlags & 1)) ? 1.2f :
                                                                                                                                              0.85000002f);
        vfApplyHitReactionAndStartMotion(self);
        self->queuedAction = 0;
    }
}

// @verified - p
void UpdateContactFacingMotion(VFActor *self) {
    if (self->hitPhase) {
        if (self->hitPhase == 1 && float(self->reactionEndFrame) <= self->animationTime)
            self->queuedAction = 0;
        return;
    }
    std::uint16_t slot;
    std::uint8_t direction;
    const bool mirrored = (self->moveFlags & VFMF_MirroredStance) != 0;
    switch (static_cast<std::uint8_t>(self->packedHitImpulse >> 24)) {
    case 0:
    case 1:
    case 7:
        slot = 246;
        direction = 8;
        break;
    case 3:
    case 4:
    case 5:
        slot = 248;
        direction = 16;
        break;
    case 2:
        slot = mirrored ? 250 : 252;
        direction = mirrored ? 32 : 64;
        break;
    case 6:
        slot = mirrored ? 252 : 250;
        direction = mirrored ? 64 : 32;
        break;
    default:
        return;
    }
    if ((self->hitStateFlags & VFHSF_CameraAssistLock) && float(static_cast<std::uint16_t>(self->reactionMiddleFrame)) >= self->animationTime) {
        const auto intersection = direction & self->cameraMotionFlags;
        if (intersection & 8)
            slot = 286;
        else if (intersection & 16)
            slot = 290;
    }
    self->previousCameraMotionFlags = self->cameraMotionFlags;
    self->cameraMotionFlags = direction;
    Install(self, Animation(self, slot));
    ++self->hitPhase;
    self->reactionDepthBits = std::uint32_t{1};
}

// @verified - p
void UpdateGuardImpactHold(VFActor *self) {
    self->contactTimer += self->scale * 0.8f;
    const auto timer = DecrementHold(self);
    VFActor *const other = self->actor;
    switch (self->hitPhase) {
    case 0:
        self->moveFlags |= VFMF_HitReactionLock;
        self->knockbackVelocity.x = self->pendingKnockback.x * 1.5f;
        self->knockbackVelocity.z = self->pendingKnockback.z * 1.5f;
        self->slideVelocity.x = self->pendingKnockback.x * 4;
        self->slideVelocity.z = self->pendingKnockback.z * 4;
        self->pendingAnimation = static_cast<std::uint16_t>(self->queuedAction);
        if (!self->pendingAnimation)
            self->pendingAnimation = Animation(self, (self->hitStateFlags & VFHSF_Unknown8) ? 20 : 16);
        vfApplyHitReactionAndStartMotion(self);
        self->hitStateFlags |= VFHSF_ReactionActive;
        if (self->actionTriggerPhase) {
            if (self->reactionEndFrame > self->currentAnimationEndFrame)
                self->reactionEndFrame = self->currentAnimationEndFrame;
            self->animationTime = self->parameter4c8 = float(static_cast<std::uint16_t>(self->parameter2d0));
        } else {
            self->parameter2d0 = self->reactionMiddleFrame = 0;
            self->reactionEndFrame = 0;
        }
        ++self->hitPhase;
        [[fallthrough]];
    case 1:
        if (float(static_cast<std::uint16_t>(self->reactionMiddleFrame)) - self->scale > self->animationTime)
            return;
        ++self->hitPhase;
        [[fallthrough]];
    case 2: {
        const float middle = float(static_cast<std::uint16_t>(self->reactionMiddleFrame)) - self->scale;
        if (static_cast<std::int16_t>(timer + int(middle) - self->reactionEndFrame) < 0)
            ++self->hitPhase;
        else {
            self->animationTime = self->parameter4c8 = middle;
            break;
        }
        [[fallthrough]];
    }
    case 3:
        if (timer <= 0) {
            self->hitStateFlags &= ~VFHSF_ReactionActive;
            self->moveFlags &= ~VFMF_HitReactionLock;
            self->queuedAction = 0;
        }
        break;
    default:
        break;
    }
    if (other->lastHitMove != self->parameter44e && (other->groundContactFlags & 3)) {
        self->pendingAnimation = vfTryGuardReactionSwap(self, other);
        if (self->pendingAnimation) {
            self->queuedActionState = std::uint8_t{1};
            self->hitPhase = 3;
        }
    }
}

// @verified - p
void UpdatePairedMotion(VFActor *self) {
    VFMA_t *const game = self->game;
    if (self->secondaryMotionFlags & VFSMF_Unknown400)
        self->moveFlags |= VFMF_HitReactionLock;
    if (!self->hitPhase) {
        VFMA_t *const current = vfmaGetTaskParam();
        if (current && (current->mode & 0x80000)) {
            std::memcpy(&self->targetPosition, game->reserved560, sizeof(Vector3));
            std::memcpy(&self->targetFacingAngle, game->reserved560 + 12, sizeof(std::uint16_t));
            self->moveFlags |= VFMF_RefreshContactHistory;
        } else if (((self->queuedAction >> 24) & 0xff) == 8) {
            std::memcpy(game->reserved560, &self->targetPosition, sizeof(Vector3));
            std::memcpy(game->reserved560 + 12, &self->targetFacingAngle, sizeof(std::uint16_t));
            const auto animation = static_cast<std::uint16_t>(self->queuedAction);
            std::memcpy(game->reserved560 + 14, &animation, sizeof(animation));
        }
        self->moveFlags &= ~VFMF_HitReactionLock;
        self->stateFlags |= VFSF_Unknown200;
        self->pendingAnimation = static_cast<std::int16_t>(self->queuedAction);
        self->knockbackAcceleration = game->knockbackAmount;
        vfApplyHitReactionAndStartMotion(self);
        mtwkSetExtraFlag(self->motion, 1, 0);
        self->knockbackVelocity = self->pendingKnockback = self->slideVelocity = {};
        self->facingAngle = static_cast<std::uint16_t>(self->targetFacingAngle);
        self->position = self->contactPosition = self->targetPosition;
        savedPairCameraMode = camCtrlGetInterpMode();
        const auto animation = self->currentAnimation & 0xfff;
        if (g_currentMap == AREA_QFRR && game->spawnPointCount && std::uint16_t(animation - 2071) <= 1)
            ++self->hitPhase;
        else
            self->hitPhase = 2;
        return;
    }
    if (self->hitPhase == 1) {
        if (self->animationTime <= float(self->parameter33c[1]))
            return;
        camCtrlSetInterpModeTo(CameraMotionMode::AutoQuadratic2);
        vfSetupLinkedFightCamera(self->actor, 2);
        ++self->hitPhase;
    }
    if (self->hitPhase == 2) {
        if (self->activeMotionFlags & (VFSMF_Unknown400 | VFSMF_Unknown800))
            ++self->hitPhase;
        else if (self->animationTime >= float(self->reactionEndFrame)) {
            mtwkSetExtraFlag(self->motion, 1, 2);
            self->moveFlags &= ~VFMF_Unknown200;
            self->queuedAction = 0;
            return;
        }
    }
    if ((self->hitPhase == 2 || self->hitPhase == 3) && float(self->reactionEndFrame) <= self->animationTime && self->requestedAnimation &&
        (self->secondaryMotionFlags & (VFSMF_Unknown400 | VFSMF_Unknown800))) {
        const auto animation = self->requestedAnimation == -1 ? self->currentAnimation : static_cast<std::uint16_t>(self->requestedAnimation);
        self->parameter370 = std::uint64_t(animation) + 0x08000000;
        mtwkSetExtraFlag(self->motion, 1, 2);
        self->moveFlags &= ~VFMF_HitReactionLock;
        self->queuedAction = 0;
    }
}

// @verified - p
void UpdatePairedDamage(VFActor *self) {
    VFActor *const source = self->secondActor;
    VFMA_t *const game = self->game;
    if (!self->hitPhase) {
        self->pendingAnimation = static_cast<std::int16_t>(self->queuedAction);
        self->stateFlags |= VFSF_Unknown200;
        self->knockbackAcceleration = game->knockbackAmount;
        vfApplyHitReactionAndStartMotion(self);
        self->moveFlags |= VFMF_HitReactionLock;
        ++self->hitPhase;
        self->reactionFacingAngleDelta = source->facingAngleDelta;
        self->knockbackVelocity = self->pendingKnockback = self->slideVelocity = {};
        VFMA_t *const current = vfmaGetTaskParam();
        if (current && (current->mode & 0x80000)) {
            std::memcpy(&source->targetPosition, game->reserved560, sizeof(Vector3));
            std::memcpy(&source->targetFacingAngle, game->reserved560 + 12, sizeof(std::uint16_t));
            self->moveFlags |= VFMF_RefreshContactHistory;
        }
        self->position = self->contactPosition = source->targetPosition;
        self->facingAngle = static_cast<std::uint16_t>(source->targetFacingAngle);
        if (game->mode & 0x81000)
            self->pairedContactPriority |= 1;
        return;
    }
    if (self->hitPhase == 1) {
        if (self->animationTime < float(self->parameter2e0))
            return;
        if (self->motionFlags & VFMOF_Unknown4000)
            self->hitStateFlags |= VFHSF_SpecialGuardOrParry;
        const auto strength = (source->secondaryMotionFlags & VFSMF_Unknown400) ? static_cast<std::uint8_t>(source->guardHitParameter) :
                                                                                  static_cast<std::uint8_t>(source->guardStrength);
        auto damage = static_cast<std::int16_t>(strength + source->parameter350);
        const auto flags = game->groups[self->groupIndex]->selectFlags;
        if ((((flags & 0x80) && (source->secondaryMotionFlags & (VFSMF_Unknown8 | VFSMF_Unknown4000000))) ||
                ((flags & 0x20) && (source->secondaryMotionFlags & VFSMF_Unknown4000000))) &&
            !(g_pFightStage->battleFlags & 0x20))
            damage = static_cast<std::int16_t>(int(self->hp) + 1);
        if (self->fightStyle == 18 && self->slot->parameter128 > 1)
            damage = static_cast<std::int16_t>(int(self->hp) + 1);
        const auto slot = static_cast<std::uint8_t>(self->slot->parameter1a5);
        if (slot == 2) {
            if ((self->savedMotionFlags & VFMOF_Unknown40000) && (source->motionFlags & VFMOF_ReverseFacingOnHit))
                self->parameterB2c = 20;
        } else if (slot == 3 && ((self->secondaryMotionFlags & VFSMF_Unknown10) || std::uint16_t((self->currentAnimation & 0xfff) - 67) <= 1)) {
            constexpr float multipliers[16]{6, 6.4f, 6.8f, 7.2f, 7.6f, 8, 8.4f, 8.8f, 9.2f, 9.6f, 10, 10.4f, 10.8f, 11.2f, 11.6f, 12};
            self->parameterB2c = multipliers[vfMotnNibbleGet(67)];
        }
        self->pendingGuardDamage =
            (self->savedMotionFlags & VFMOF_Unknown80000) ? static_cast<std::uint16_t>(self->pendingGuardDamage + damage) : static_cast<std::uint16_t>(damage);
        if (self->secondaryMotionFlags & (VFSMF_Unknown400 | VFSMF_Unknown800))
            ++self->hitPhase;
        else
            self->hitPhase = 3;
        vfHandleImpactSfxAndDefeat(self, damage, 0);
        return;
    }
    if (self->hitPhase == 2) {
        if (float(self->currentAnimationEndFrame) > self->animationTime)
            return;
        const auto animation = static_cast<std::uint16_t>(self->requestedAnimation);
        if (std::uint16_t(animation - 1) <= 0xfffd && (self->secondaryMotionFlags & (VFSMF_Unknown400 | VFSMF_Unknown800)))
            self->parameter370 = std::uint64_t(animation) + 0x08000000;
    } else if (self->hitPhase != 3)
        return;
    if (float(self->reactionEndFrame) > self->animationTime)
        return;
    if (camCtrlGetInterpMode() == CameraMotionMode::AutoQuadratic2) {
        vfUnknownCamRelated(game);
        camCtrlSetInterpModeTo(savedPairCameraMode);
    }
    VFMA_t *const current = vfmaGetTaskParam();
    if (current && (current->mode & 0x80000) && !(self->moveFlags & VFMF_ForceDefeat) && !(self->stateFlags & VFSF_Unknown80))
        vfHandleImpactSfxAndDefeat(self, 10000, 0);
    figpAssignModeForCharacter(self->character, 0);
    if ((self->moveFlags & VFMF_ForceDefeat) || self->hp <= 0)
        self->hitStateFlags = (self->hitStateFlags & ~VFHSF_AirborneOrDowned) | 0x14000;
    else if (self->parameter2dc == 4 || (self->hitStateFlags & VFHSF_SpecialGuardOrParry)) {
        const auto amount = self->pendingGuardDamage;
        auto frames = amount <= 14 ? std::uint8_t{1} : static_cast<std::uint8_t>(int((float(amount) - 14 + 0.5f) * 0.25f));
        if (frames > 30)
            frames = 30;
        self->parameter370 = 0x06000000;
        self->hitPhaseWords[0] = static_cast<std::uint16_t>(int(float(frames) * self->inverseScale));
    } else {
        self->stateFlags &= ~VFSF_Unknown200;
        self->queuedAction = 0;
        self->moveFlags &= ~VFMF_HitReactionLock;
    }
}

// Airborne reactions retain the authored takeoff/landing gates.
// @verified - p
void UpdateAirborneReaction(VFActor *self) {
    if (!self->hitPhase) {
        self->moveFlags |= VFMF_HitReactionLock;
        const auto animation = static_cast<std::uint16_t>(self->queuedAction);
        self->pendingAnimation = animation;
        self->knockbackAcceleration = self->timeScale;
        if (std::fabs(self->hp) < 0.00000011920929f && !EV_camCtrlToggleCombatCamera(1, -1)) {
            const bool admitted = self->groupIndex == self->game->playerSlot ? !(g_pFightStage->battleFlags & 0x2000) :
                                                                               vfIsLastEnemyForCamera(self) && !(g_pFightStage->battleFlags & 0x4000);
            if (admitted) {
                self->knockbackAcceleration = 0.375f;
                self->pendingKnockback.x *= 0.375f;
                self->pendingKnockback.y *= 0.375f;
                self->pendingKnockback.z *= 0.375f;
                camCtrlSetInterpModeTo(CameraMotionMode::AutoQuadratic);
                vfSetupLinkedFightCamera(self, (((animation & 0xfff) - 456) & 0xfffd) ? 2 : 1);
            }
        }
        vfApplyHitReactionAndStartMotion(self);
        mtwkSetExtraFlag(self->motion, 0, 2);
        if (camCtrlGetInterpMode() == CameraMotionMode::AutoQuadratic)
            self->hitStateFlags &= ~VFHSF_AttackActive;
        if (!FindLoadedMotionSecondaryEntry(animation & 0xfff, 3).empty()) {
            ++self->hitPhase;
            self->knockbackVelocity = {};
        } else {
            self->knockbackVelocity.x = self->pendingKnockback.x * 1.5f;
            self->knockbackVelocity.z = self->pendingKnockback.z * 1.5f;
            self->slideVelocity.x = self->pendingKnockback.x * 4;
            self->slideVelocity.z = self->pendingKnockback.z * 4;
            self->hitPhase = 4;
            self->contactTimer += 0.4f * self->scale;
        }
    } else if (self->hitPhase == 1) {
        if (self->animationTime < float(self->parameter2d6))
            return;
        self->knockbackVelocity.x += self->pendingKnockback.x;
        self->knockbackVelocity.y += self->pendingKnockback.y;
        self->knockbackVelocity.z += self->pendingKnockback.z;
        vfSnapActorToContact(self);
        self->predictedKnockbackFrames = static_cast<std::uint16_t>(int((float(self->parameter2e0) - self->animationTime) * self->inverseScale));
        vfPredictLandingGround(self);
        ++self->hitPhase;
    } else if (self->hitPhase == 2) {
        if (!(self->moveFlags & VFMF_Ascending))
            self->knockbackVelocity.y -= self->scale * self->scale * self->game->gravityScale;
        vfUpdateGroundImpactReaction(self);
        if (!(self->moveFlags & VFMF_Airborne)) {
            if (!(self->activeMotionFlags & VFSMF_Unknown1))
                self->animationTime = self->parameter4c8 = float(self->parameter2e0);
            self->hitPhase = 4;
        }
    } else if (self->hitPhase == 4) {
        if (self->animationTime >= float(self->reactionEndFrame))
            figpAssignModeForCharacter(self->character, 0);
        mtwkSetExtraFlag(self->motion, 0, 0);
        if (SetMotionRegionFlag(self->motion, 65, 1, MotionRegion::LowerBody)) {
            if (!(self->hitStateFlags & VFHSF_SpecialThrow) && !(self->moveFlags & VFMF_ForceDefeat))
                self->parameter370 = 0x06000000;
            self->moveFlags &= ~VFMF_HitReactionLock;
            self->queuedAction = 0;
        }
    }
}


// @verified - p
void UpdateTerrainDrop(VFActor *self) {
    if (vfIsWithinFightFloor(self->contactPosition)) {
        self->position.x = self->contactPosition.x = self->contactVectors[0].x = self->contactVectors[2].x;
        self->position.z = self->contactPosition.z = self->contactVectors[0].z = self->contactVectors[2].z;
    }
    switch (self->hitPhase) {
    case 0:
        self->moveFlags |= VFMF_HitReactionLock;
        if (self->animationTime >= float(self->reactionEndFrame)) {
            figpAssignModeForCharacter(self->character, 0);
            self->hitStateFlags &= ~VFHSF_AirborneOrDowned;
            if (!((self->hitStateFlags | self->motionFlags) & 0x1c000))
                Install(self, 198);
        }
        if (g_currentMap.name == SHEN_NAME(QLBT) && camCtrlGetInterpMode() != CameraMotionMode::AutoQuadratic) {
            camCtrlSetInterpModeTo(CameraMotionMode::AutoQuadratic);
            vfSetupLinkedFightCamera(self, 3);
        }
        self->hitPhase = 1;
        if (self->hitStateFlags & VFHSF_AlignedToGround) {
            self->knockbackVelocity = self->pendingKnockback = self->slideVelocity = {};
            if (g_currentMap.name != SHEN_NAME(QLBT))
                self->hitPhase = 3;
        }
        break;
    case 1:
        self->knockbackVelocity.y -= self->scale * self->scale * self->game->gravityScale;
        if (self->animationTime < float(self->parameter2d6))
            break;
        if (self->knockbackVelocity.y > self->contactPosition.y - self->contactVectors[0].y)
            self->knockbackVelocity.y = self->contactPosition.y - self->contactVectors[0].y;
        if (!FindLoadedMotionSecondaryEntry(self->currentAnimation & 0xfff, 3).empty()) {
            if (self->animationTime >= float(self->parameter2e0))
                self->animationTime = self->parameter4c8 = float(self->parameter2e0) - 1;
        } else {
            self->parameter2e0 = static_cast<std::uint16_t>(int(self->animationTime) + 1);
            self->groundOffset = self->contactPosition.y - self->position.y;
        }
        if (self->contactPosition.y > self->groundOffset + self->game->floorSnapY) {
            vfSnapActorToContact(self);
            self->knockbackVelocity.x = self->contactPosition.x - self->contactVectors[0].x;
            self->knockbackVelocity.z = self->contactPosition.z - self->contactVectors[0].z;
            self->predictedKnockbackFrames = static_cast<std::uint16_t>(int((float(self->parameter2e0) - self->animationTime) * self->inverseScale));
            vfPredictLandingGround(self);
        }
        self->hitPhase = 2;
        break;
    case 2:
        if (!(self->moveFlags & VFMF_Airborne)) {
            self->animationTime = self->parameter4c8 = float(self->parameter2e0);
            if ((self->motionFlags & (VFMOF_Unknown4000 | VFMOF_Unknown80000)) == 0x84000)
                self->hitStateFlags |= VFHSF_SpecialGuardOrParry;
            self->hitPhase = 3;
        }
        break;
    case 3:
        if (self->animationTime > float(self->reactionEndFrame)) {
            figpAssignModeForCharacter(self->character, 0);
            const auto animation = !(self->hitStateFlags & VFHSF_SpecialGuardOrParry) ? self->defaultAnimations[0] :
                (self->motionFlags & VFMOF_Unknown4000000)                            ? ((self->motionFlags & VFMOF_ContactCategoryB) ? 452 : 453) :
                                                                                        Animation(self, (self->motionFlags & VFMOF_ContactCategoryB) ? 68 : 66);
            Install(self, animation);
            self->hitPhase = 4;
        }
        break;
    default:
        break;
    }
}

// Authored eight-way locomotion and mirrored stance transitions.
// @verified - p
void UpdateDirectionalMotion(VFActor *self) {
    const auto oldFlags = self->cameraMotionFlags;
    const auto sector = self->relativeInputSectors[0];
    const bool changed = sector != self->relativeInputSectors[1];
    const bool mirrored = (self->moveFlags & VFMF_MirroredStance) != 0;
    const bool motionTwo = (self->motionFlags & VFMOF_Unknown2) != 0;
    std::uint16_t slot = 0;
    std::uint8_t direction = 0;
    bool flip = false;
    switch (self->packedHitImpulse >> 24) {
    case 0:
        slot = mirrored ? 306 : 246;
        direction = 8;
        flip = mirrored && changed;
        if (mirrored) {
            self->hitStateFlags &= ~VFHSF_MirroredReactionAngle;
            self->moveFlags &= ~(VFMF_MirrorReactionAngle | VFMF_MirroredStance);
            SetMotionRegionFlag(self->motion, 69, 2, MotionRegion::LowerBody);
        }
        break;
    case 1:
        slot = mirrored || motionTwo ? 258 : 284;
        direction = 72;
        flip = (mirrored || motionTwo) && changed;
        break;
    case 2:
        slot = mirrored ? 250 : 252;
        direction = 64;
        flip = changed;
        break;
    case 3:
        slot = !mirrored || motionTwo ? 256 : 278;
        direction = 80;
        flip = (!mirrored || motionTwo) && changed;
        break;
    case 4:
        slot = mirrored ? 308 : 248;
        direction = 16;
        flip = !mirrored && changed;
        if (mirrored) {
            self->hitStateFlags &= ~VFHSF_MirroredReactionAngle;
            self->moveFlags &= ~(VFMF_MirrorReactionAngle | VFMF_MirroredStance);
            SetMotionRegionFlag(self->motion, 69, 2, MotionRegion::LowerBody);
        }
        break;
    case 5:
        slot = mirrored || motionTwo ? 256 : 278;
        direction = 48;
        flip = !(mirrored || motionTwo) && changed;
        break;
    case 6:
        slot = mirrored ? 252 : 250;
        direction = 32;
        break;
    case 7:
        slot = !mirrored || motionTwo ? 258 : 284;
        direction = 40;
        flip = !(!mirrored || motionTwo) && changed;
        break;
    default:
        break;
    }
    if (changed) {
        if (flip)
            self->stateFlags |= VFSF_UseLeftFootTarget;
        else
            self->stateFlags &= ~VFSF_UseLeftFootTarget;
    }
    if ((self->hitStateFlags & VFHSF_CameraAssistLock) && self->animationTime <= float(self->reactionEndFrame)) {
        if (sector == self->relativeInputSectors[1] || !((self->cameraControlFlags ^ self->previousReactionInputBytes[0]) & 0xf0))
            return;
        self->relativeInputSectors[1] = sector;
        self->previousCameraMotionFlags = self->cameraMotionFlags;
        self->cameraMotionFlags = direction | (oldFlags & 0x87);
        self->queuedAction = 0;
        Install(self, Animation(self, slot));
    } else {
        Install(self, Animation(self, slot));
        self->previousCameraMotionFlags = self->cameraMotionFlags;
        self->relativeInputSectors[1] = self->relativeInputSectors[0];
        self->cameraMotionFlags = direction | (oldFlags & 0x87);
        self->queuedAction = 0;
    }
}


// @verified - p
void vfActionNullsub(VFActor *) {
}

// @verified - p
void vfRemoveQueuedActionAtEnd(VFActor *self) {
    if (self->animationTime >= float(self->currentAnimationEndFrame))
        self->queuedAction = 0;
}

std::uint16_t ReactionAnimation(const VFActor *self, std::size_t offset) {
    const auto *data = self->motionParameters[4] + offset;
    return static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(data[0]) << 8 | std::to_integer<std::uint8_t>(data[1]));
}

// @verified - p
void VFActor_ClearActionFields(VFActor *self) {
    self->queuedAction = self->action;
    self->hitPhase = 0;
    self->tickFrameTime = 0;
}

// @verified - p
void vfQueueActionAnimation(VFActor *self) {
    self->pendingAnimation = static_cast<std::int16_t>(ReactionAnimation(self, 40));
    vfApplyHitReactionAndStartMotion(self);
    self->queuedAction = self->action;
}

// @verified - p
void vfCopyActionToQueuedActionAndResetHitPhase(VFActor *self) {
    const bool reset = static_cast<std::uint8_t>(self->queuedAction >> 24) != 6;
    self->queuedAction = self->action;
    if (reset)
        self->hitPhase = 0;
}

// @verified - p
void vfQueueStepAnimationIfDue(VFActor *self) {
    if ((self->animationTime > 9 || static_cast<std::uint8_t>(self->action >> 24) != static_cast<std::uint8_t>(self->queuedAction >> 24)) &&
        !(self->hitStateFlags & VFHSF_ForcedReaction)) {
        self->pendingAnimation = static_cast<std::int16_t>(ReactionAnimation(self, 286));
        vfApplyHitReactionAndStartMotion(self);
        self->queuedAction = self->action;
    }
}

// @verified - p
void VF_SetKnockbackAnimationVariablesAndTriggerReaction(VFActor *self) {
    if ((self->animationTime > 9 || static_cast<std::uint8_t>(self->action >> 24) != static_cast<std::uint8_t>(self->queuedAction >> 24)) &&
        !(self->hitStateFlags & VFHSF_ForcedReaction)) {
        self->pendingAnimation = static_cast<std::int16_t>(ReactionAnimation(self, 290));
        vfApplyHitReactionAndStartMotion(self);
        self->queuedAction = self->action;
    }
}

// @verified - p
void vfQueueReactionEventFromEntryTable(VFActor *self) {
    const auto event = static_cast<std::uint8_t>(self->action);
    if (vfFindQueuedReactionEventEntry(self, event))
        self->queuedReactionEvent = event;
}

// @verified - p
void vfApplyCameraRelativeDodgeOrStep(VFActor *self) {
    if (static_cast<std::uint8_t>(self->action >> 24) == static_cast<std::uint8_t>(self->queuedAction >> 24) || (self->hitStateFlags & VFHSF_ForcedReaction))
        return;
    constexpr std::uint16_t offsets[8]{30, 32, 236, 238, 12, 14, 236, 238};
    const VFActor *other = self->actor;
    const bool airborne = (self->moveFlags & VFMF_MirroredStance) != 0;
    const bool player = self->groupIndex == static_cast<std::uint8_t>(self->game->playerSlot) && !(self->game->gameFlags & 0x400000);
    const std::int16_t facing = player ? self->alternateContactFacing : self->contactFacing;
    const auto close = [](std::int16_t angle, std::int16_t target) { return std::fabs(float(static_cast<std::int16_t>(angle - target))) <= 8192; };
    std::uint16_t index;
    if (self->groundContactFlags & 1) {
        if (close(facing, -16384))
            index = 3 - airborne;
        else
            index = airborne ? 3 : 2;
    } else if (close(facing, 16384))
        index = !airborne;
    else
        index = airborne;
    if (((other->hitStateFlags & VFHSF_AttackActive) && float(static_cast<std::uint16_t>(other->parameter2d0)) - other->animationTime > 5) ||
        (other->secondaryMotionFlags & VFSMF_Unknown2))
        index += 4;
    std::uint16_t id = ReactionAnimation(self, offsets[index]);
    if (!id && index >= 4)
        id = ReactionAnimation(self, offsets[index - 4]);
    self->pendingAnimation = id;
    vfApplyHitReactionAndStartMotion(self);
    self->queuedAction = self->action;
}

// @verified - p
void VFActor_UpdateQueuedActionIdx(VFActor *self) {
    if (static_cast<std::uint8_t>(self->action >> 24) == static_cast<std::uint8_t>(self->queuedAction >> 24))
        return;
    self->queuedAction = self->action;
    self->hitPhase = 0;
    self->tickFrameTime = 0;
}

// @verified - p
void VFM_ResetHitPhase(VFActor *self) {
    self->queuedAction = self->action;
    self->hitPhase = 0;
}

constexpr VFActionCallbacks g_vfPreUpdateCallbacks[37]{
    {.clear = VFActor_ClearActionFields, .update = UpdateInstallMotion},
    {.clear = VFActor_ClearActionFields, .update = UpdateDefaultReaction},
    {.clear = vfQueueActionAnimation, .update = vfRemoveQueuedActionAtEnd},
    {.clear = vfActionNullsub, .update = vfActionNullsub},
    {.clear = VFActor_ClearActionFields, .update = UpdateGuardImpactHold},
    {.clear = VFActor_ClearActionFields, .update = UpdateAirborneReaction},
    {.clear = vfCopyActionToQueuedActionAndResetHitPhase, .update = UpdateRecovery},
    {.clear = VFActor_ClearActionFields, .update = UpdateLandingHold},
    {.clear = VFActor_ClearActionFields, .update = UpdatePairedMotion},
    {.clear = VFActor_ClearActionFields, .update = UpdatePairedDamage},
    {.clear = vfQueueStepAnimationIfDue, .update = vfRemoveQueuedActionAtEnd},
    {.clear = VF_SetKnockbackAnimationVariablesAndTriggerReaction, .update = vfRemoveQueuedActionAtEnd},
    {.clear = vfQueueReactionEventFromEntryTable, .update = vfActionNullsub},
    {.clear = VFActor_ClearActionFields, .update = UpdateApproachContact},
    {.clear = VFActor_ClearActionFields, .update = UpdateLinkedAttackWait},
    {.clear = VFActor_ClearActionFields, .update = UpdateContactAttackAdmission},
    {.clear = VFActor_ClearActionFields, .update = vfActionNullsub},
    {.clear = VFActor_ClearActionFields, .update = UpdatePairedTiming},
    {.clear = VFActor_UpdateQueuedActionIdx, .update = vfActionNullsub},
    {.clear = VFActor_ClearActionFields, .update = vfActionNullsub},
    {.clear = VFActor_ClearActionFields, .update = vfActionNullsub},
    {.clear = VFActor_UpdateQueuedActionIdx, .update = vfActionNullsub},
    {.clear = VFActor_UpdateQueuedActionIdx, .update = vfActionNullsub},
    {.clear = vfApplyCameraRelativeDodgeOrStep, .update = vfRemoveQueuedActionAtEnd},
    {.clear = VFActor_ClearActionFields, .update = UpdateReverseRecovery},
    {.clear = VFActor_ClearActionFields, .update = UpdateTerrainDrop},
    {.clear = VFActor_ClearActionFields, .update = vfActionNullsub},
    {.clear = VFActor_ClearActionFields, .update = UpdateDirectionalMotion},
    {.clear = VFActor_ClearActionFields, .update = UpdateLocomotion},
    {.clear = VFActor_ClearActionFields, .update = UpdateTargetLocomotion},
    {.clear = VFActor_ClearActionFields, .update = vfActionNullsub},
    {.clear = VFM_ResetHitPhase, .update = UpdateTimedGuard},
    {.clear = VFActor_ClearActionFields, .update = vfActionNullsub},
    {.clear = VFActor_ClearActionFields, .update = UpdateReactionHold},
    {.clear = VFActor_ClearActionFields, .update = UpdatePairedMotion},
    {.clear = VFActor_ClearActionFields, .update = UpdateContactFacingMotion},
    {.clear = VFActor_ClearActionFields, .update = UpdateAuthoredDefeat},
};
} // namespace

// @verified - p
void vfDispatchCurrentActionState(VFActor *self) {
    self->previousAction = self->action;
    if (!self->action)
        return;
    auto state = static_cast<std::uint8_t>(self->action >> 24);
    if (state >= 37)
        state = 1;
    g_vfPreUpdateCallbacks[state].clear(self);
}


// @verified - p
void vfUpdateDynamicActionState(VFActor *self) {
    self->previousQueuedAction = self->queuedAction;
    if (self->queuedAction)
        g_vfPreUpdateCallbacks[static_cast<std::int8_t>(self->queuedAction >> 24)].update(self);
}
} // namespace shenmue::s2
