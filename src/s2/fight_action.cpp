#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/fight_stage.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cmath>
#include <cstring>
#else
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
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

CameraMotionMode savedPairCameraMode{};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

void UpdateInstallMotion(VFActor *self);
void UpdateDefaultReaction(VFActor *self);
void UpdateRecovery(VFActor *self);
void UpdateLandingHold(VFActor *self);
void UpdateApproachContact(VFActor *self);
void UpdateLinkedAttackWait(VFActor *self);
void UpdateContactAttackAdmission(VFActor *self);
void UpdatePairedTiming(VFActor *self);
void UpdateReverseRecovery(VFActor *self);
void UpdateTimedGuard(VFActor *self);
void UpdateReactionHold(VFActor *self);
void UpdateAuthoredDefeat(VFActor *self);
void UpdateLocomotion(VFActor *self);
void UpdateTargetLocomotion(VFActor *self);
void UpdateContactFacingMotion(VFActor *self);
void UpdateGuardImpactHold(VFActor *self);
void UpdatePairedMotion(VFActor *self);
void UpdatePairedDamage(VFActor *self);
void UpdateAirborneReaction(VFActor *self);
void UpdateTerrainDrop(VFActor *self);
void UpdateDirectionalMotion(VFActor *self);
void vfActionNullsub(VFActor *);
void vfRemoveQueuedActionAtEnd(VFActor *self);
void VFActor_ClearActionFields(VFActor *self);
void vfQueueActionAnimation(VFActor *self);
void vfCopyActionToQueuedActionAndResetHitPhase(VFActor *self);
void vfQueueStepAnimationIfDue(VFActor *self);
void VF_SetKnockbackAnimationVariablesAndTriggerReaction(VFActor *self);
void vfQueueReactionEventFromEntryTable(VFActor *self);
void vfApplyCameraRelativeDodgeOrStep(VFActor *self);
void VFActor_UpdateQueuedActionIdx(VFActor *self);
void VFM_ResetHitPhase(VFActor *self);

} // namespace

void vfDispatchCurrentActionState(VFActor *self);
void vfUpdateDynamicActionState(VFActor *self);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t Animation(const VFActor *self, std::size_t offset) {
    const auto *data = self->motionParameters[4] + offset;
    return int(static_cast<std::int16_t>(std::uint16_t(std::to_integer<std::uint8_t>(data[0])) << 8)) | std::to_integer<std::uint8_t>(data[1]);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int16_t DecrementHold(VFActor *self) {
    auto timer = static_cast<std::int16_t>(static_cast<std::uint16_t>(self->parameter40c) - 1);
    if (timer < 0)
        timer = 0;
    self->parameter40c = (self->parameter40c & 0xffff0000U) | static_cast<std::uint16_t>(timer);
    return timer;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void Install(VFActor *self, std::int32_t animation) {
    self->pendingAnimation = animation;
    vfApplyHitReactionAndStartMotion(self);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define SHENMUE_ACTION_VALUE(T, actor, offset) (*reinterpret_cast<T *>(reinterpret_cast<std::byte *>(actor) + (offset)))

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define SHENMUE_ACTION_ANIMATION(actor, offset)                                                                                                                \
    (static_cast<std::uint8_t>(SHENMUE_ACTION_VALUE(const std::byte *, actor, 0x38)[(offset) + 1]) |                                                           \
        (static_cast<std::int8_t>(SHENMUE_ACTION_VALUE(const std::byte *, actor, 0x38)[offset]) << 8))

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define SHENMUE_INSTALL_ACTION_ANIMATION(actor, animation) (SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = (animation), vfStartFormationAnimation(actor))

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline std::int16_t DecrementActionHold(FightActorRuntimePrefix *actor) {
    std::int16_t timer = static_cast<std::int16_t>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3a0) - 1U);
    if (timer < 0)
        timer = 0;
    SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x3a0) = timer;
    return timer;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
inline void ClearActionFields(FightActorRuntimePrefix *actor) {
    actor->queuedAction = SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x320);
    SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 0;
    SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3c6) = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ResetActionHitPhase(FightActorRuntimePrefix *actor) {
    SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 0;
    actor->queuedAction = SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x320);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateQueuedActionIndex(FightActorRuntimePrefix *actor) {
    if (SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x323) != SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x32b))
        ClearActionFields(actor);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void QueueActionAnimation(FightActorRuntimePrefix *actor) {
    SHENMUE_INSTALL_ACTION_ANIMATION(actor, SHENMUE_ACTION_ANIMATION(actor, 40));
    actor->queuedAction = SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x320);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void CopyActionAndResetHitPhase(FightActorRuntimePrefix *actor) {
    if (SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x32b) != 6)
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 0;
    actor->queuedAction = SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x320);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void QueueStepAnimationIfDue(FightActorRuntimePrefix *actor) {
    if (actor->animationTime > 9 || SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x323) != SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x32b)) {
        SHENMUE_INSTALL_ACTION_ANIMATION(actor, SHENMUE_ACTION_ANIMATION(actor, 286));
        actor->queuedAction = SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x320);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void QueueKnockbackAnimationIfDue(FightActorRuntimePrefix *actor) {
    if (actor->animationTime > 9 || SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x323) != SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x32b)) {
        SHENMUE_INSTALL_ACTION_ANIMATION(actor, SHENMUE_ACTION_ANIMATION(actor, 290));
        actor->queuedAction = SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x320);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void QueueReactionEvent(FightActorRuntimePrefix *actor) {
    const std::uint16_t action = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x320);
    if (vfFindQueuedReactionEventEntry(actor, static_cast<std::uint8_t>(action)))
        SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x27e) = action;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfStartQueuedReactionAnimation(FightActorRuntimePrefix *actor);

namespace {

void QueueBallisticAction(FightActorRuntimePrefix *actor);

void QueueCameraRelativeDodge(FightActorRuntimePrefix *actor);

void UpdateBallisticAction(FightActorRuntimePrefix *actor);

void UpdateGuardImpactHold(FightActorRuntimePrefix *actor);

void UpdateAirborneReaction(FightActorRuntimePrefix *actor);

void UpdateRecovery(FightActorRuntimePrefix *actor);

void UpdateLandingHold(FightActorRuntimePrefix *actor);

void UpdatePairedMotion(FightActorRuntimePrefix *actor);

void UpdatePairedDamage(FightActorRuntimePrefix *actor);

void UpdateTerrainDrop(FightActorRuntimePrefix *actor);

void UpdateDirectionalMotion(FightActorRuntimePrefix *actor);

void UpdateLocomotion(FightActorRuntimePrefix *actor);

void UpdateTargetLocomotion(FightActorRuntimePrefix *actor);

void UpdateReactionHold(FightActorRuntimePrefix *actor);

void UpdateTimedGuard(FightActorRuntimePrefix *actor);

void UpdateContactFacingMotion(FightActorRuntimePrefix *actor);

void UpdateAuthoredDefeat(FightActorRuntimePrefix *actor);

void UpdateReverseRecovery(FightActorRuntimePrefix *actor);

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool IsLastEnemyForCamera(FightActorRuntimePrefix *actor) {
    const std::int8_t index = SHENMUE_ACTION_VALUE(std::int8_t, actor, 0);
    if (actor->game->slotTeams[index] == 1 && !g_pFightStage->secondaryGroupCounters[1] && !g_pFightStage->readyEnemySlots[1]) {
        if (g_pFightStage->unknownGroupCounts[1] == 1 && !(actor->moveFlags & 0x20U))
            return true;
        if (!g_pFightStage->unknownGroupCounts[1] && (actor->moveFlags & 0x20U))
            return true;
    }
    return false;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateAirborneReaction(FightActorRuntimePrefix *actor) {
    std::uint8_t &phase = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    switch (phase) {
    case 0: {
        actor->moveFlags |= 0x10U;
        const std::uint16_t animation = static_cast<std::uint16_t>(actor->queuedAction);
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = animation;
        SHENMUE_ACTION_VALUE(float, actor, 0x278) = SHENMUE_ACTION_VALUE(float, actor, 0x274);
        if (std::fabs(actor->hp) < 1.1920929e-7f && !EV_camCtrlToggleCombatCamera(1, -1)) {
            const bool admitted = SHENMUE_ACTION_VALUE(std::int8_t, actor, 0) == actor->game->playerSlot ?
                !(g_pFightStage->battleFlags & 0x2000U) :
                IsLastEnemyForCamera(actor) && !(g_pFightStage->battleFlags & 0x4000U);
            if (admitted) {
                SHENMUE_ACTION_VALUE(float, actor, 0x278) = 0.375f;
                SHENMUE_ACTION_VALUE(float, actor, 0x46c) *= 0.375f;
                SHENMUE_ACTION_VALUE(float, actor, 0x470) *= 0.375f;
                SHENMUE_ACTION_VALUE(float, actor, 0x474) *= 0.375f;
                camCtrlSetInterpModeTo(4);
                const std::uint16_t id = animation & 0xfffU;
                vfSetupLinkedFightCamera(actor, id == 458 || id == 456 ? 1 : 2);
            }
        }
        vfStartReactionAnimation(actor);
        mtwkSetExtraFlag(actor->motion, 0, 2);
        if (!MOTN_FindAnimationExtraRecord(animation & 0xfffU, 3)) {
            SHENMUE_ACTION_VALUE(float, actor, 0x454) = SHENMUE_ACTION_VALUE(float, actor, 0x46c) * 1.5f;
            SHENMUE_ACTION_VALUE(float, actor, 0x45c) = SHENMUE_ACTION_VALUE(float, actor, 0x474) * 1.5f;
            SHENMUE_ACTION_VALUE(float, actor, 0x460) = SHENMUE_ACTION_VALUE(float, actor, 0x46c) * 4;
            SHENMUE_ACTION_VALUE(float, actor, 0x468) = SHENMUE_ACTION_VALUE(float, actor, 0x474) * 4;
            SHENMUE_ACTION_VALUE(float, actor, 0x424) += SHENMUE_ACTION_VALUE(float, actor, 0x26c) * 0.4f;
            phase = 4;
        } else {
            SHENMUE_ACTION_VALUE(Vector3, actor, 0x454) = Vector3();
            ++phase;
        }
        return;
    }
    case 1:
        if (static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x296)) > actor->animationTime)
            return;
        SHENMUE_ACTION_VALUE(float, actor, 0x454) += SHENMUE_ACTION_VALUE(float, actor, 0x46c);
        SHENMUE_ACTION_VALUE(float, actor, 0x458) += SHENMUE_ACTION_VALUE(float, actor, 0x470);
        SHENMUE_ACTION_VALUE(float, actor, 0x45c) += SHENMUE_ACTION_VALUE(float, actor, 0x474);
        vfSnapActorToContact(actor);
        SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3be) = static_cast<std::uint16_t>(
            (static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2a0)) - actor->animationTime) * SHENMUE_ACTION_VALUE(float, actor, 0x270));
        vfPredictLandingGround(actor);
        ++phase;
        return;
    case 2:
        if (!(actor->moveFlags & 8U)) {
            const float scale = SHENMUE_ACTION_VALUE(float, actor, 0x26c);
            SHENMUE_ACTION_VALUE(float, actor, 0x458) -= scale * scale * actor->game->gravityScale;
        }
        vfUpdateGroundImpactReaction(actor);
        if (!(actor->moveFlags & 4U)) {
            if (!(actor->activeMotionFlags & 1U))
                actor->animationTime = SHENMUE_ACTION_VALUE(float, actor, 0x450) = static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2a0));
            phase = 4;
        }
        return;
    case 4:
        mtwkSetExtraFlag(actor->motion, 0, 0);
        if (mtwkSetRegionFlag(actor->motion, 65, 1, static_cast<MotionRegion>(0))) {
            if (!(actor->hitStateFlags & 0x8000U) && !(actor->moveFlags & 0x20U))
                SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x318) = 0x6000000U;
            actor->moveFlags &= ~0x10U;
            actor->queuedAction = 0;
        }
        return;
    default:
        return;
    }
}

} // namespace

#else

namespace {

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
        if (mtwkSetRegionFlag(self->motion, 65, 1, MotionRegion::LowerBody)) {
            if (!(self->hitStateFlags & VFHSF_SpecialThrow) && !(self->moveFlags & VFMF_ForceDefeat))
                self->parameter370 = 0x06000000;
            self->moveFlags &= ~VFMF_HitReactionLock;
            self->queuedAction = 0;
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateTerrainDrop(FightActorRuntimePrefix *actor) {
    if (vfIsWithinFightFloor(actor->contactPosition)) {
        actor->position.x = SHENMUE_ACTION_VALUE(float, actor, 0x158);
        actor->position.z = SHENMUE_ACTION_VALUE(float, actor, 0x160);
    }
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    case 0:
        if (!(actor->animationTime < static_cast<float>(actor->motionFrames))) {
            figpAssignModeForCharacter(actor->character, 0);
            actor->hitStateFlags &= ~0x80000U;
            if (!((actor->motionFlags | actor->hitStateFlags) & 0x1c000U))
                SHENMUE_INSTALL_ACTION_ANIMATION(actor, 198);
        }
        if (g_currentMap == shen_name(SHEN_NAME4('Q', 'L', 'B', 'T')) && camCtrlGetInterpMode() != 4) {
            camCtrlSetInterpModeTo(4);
            vfSetupLinkedFightCamera(actor, 3);
        }
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 1;
        if (actor->hitStateFlags & 0x800000U) {
            SHENMUE_ACTION_VALUE(float, actor, 0x458) = 0;
            if (g_currentMap != shen_name(SHEN_NAME4('Q', 'L', 'B', 'T')))
                SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 3;
        }
        return;
    case 1: {
        const float scale = SHENMUE_ACTION_VALUE(float, actor, 0x26c);
        SHENMUE_ACTION_VALUE(float, actor, 0x458) -= scale * scale * actor->game->gravityScale;
        if (static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x296)) > actor->animationTime)
            return;
        SHENMUE_ACTION_VALUE(float, actor, 0x454) = SHENMUE_ACTION_VALUE(float, actor, 0x45c) = 0;
        const std::uint16_t end = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2a0);
        if (!end) {
            SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2a0) = static_cast<std::uint16_t>(actor->animationTime + 1);
            SHENMUE_ACTION_VALUE(float, actor, 0x2a4) = actor->contactPosition.y - actor->position.y;
        } else if (!(static_cast<float>(end) > actor->animationTime))
            actor->animationTime = SHENMUE_ACTION_VALUE(float, actor, 0x450) = static_cast<float>(static_cast<std::int32_t>(end) - 1);
        if (actor->contactPosition.y > SHENMUE_ACTION_VALUE(float, actor, 0x2a4) + actor->game->floorSnapY) {
            vfSnapActorToContact(actor);
            SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3be) = static_cast<std::uint16_t>(
                (static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2a0)) - actor->animationTime) * SHENMUE_ACTION_VALUE(float, actor, 0x270));
            vfPredictLandingGround(actor);
        }
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 2;
        return;
    }
    case 2:
        if (!(actor->moveFlags & 4U)) {
            actor->animationTime = SHENMUE_ACTION_VALUE(float, actor, 0x450) = static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2a0));
            SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 3;
        }
        return;
    case 3:
    default:
        return;
    }
}

} // namespace

#else

namespace {

// Terrain/drop motion uses the native saved contact vectors.
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdatePairedMotion(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *const game = actor->game;
    if (actor->secondaryMotionFlags & 0x400U)
        actor->moveFlags |= 0x10U;
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    case 0: {
        if (!IsAuthFightCamera()) {
            SHENMUE_ACTION_VALUE(float, game, 0x45c) = actor->targetPosition.x;
            SHENMUE_ACTION_VALUE(float, game, 0x460) = actor->targetPosition.y;
            SHENMUE_ACTION_VALUE(float, game, 0x464) = actor->targetPosition.z;
            SHENMUE_ACTION_VALUE(std::uint16_t, game, 0x468) = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x64);
            SHENMUE_ACTION_VALUE(std::uint16_t, game, 0x46a) = static_cast<std::uint16_t>(actor->queuedAction);
        } else {
            actor->targetPosition.x = SHENMUE_ACTION_VALUE(float, game, 0x45c);
            actor->targetPosition.y = SHENMUE_ACTION_VALUE(float, game, 0x460);
            actor->targetPosition.z = SHENMUE_ACTION_VALUE(float, game, 0x464);
            SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x64) = SHENMUE_ACTION_VALUE(std::uint16_t, game, 0x468);
            actor->moveFlags |= 0x10000000U;
        }
        actor->moveFlags &= ~0x10U;
        actor->stateFlags |= 0x200U;
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x328);
        SHENMUE_ACTION_VALUE(float, actor, 0x278) = actor->game->knockbackAmount;
        vfStartFormationAnimation(actor);
        mtwkSetExtraFlag(actor->motion, 1, 0);
        SHENMUE_ACTION_VALUE(float, actor, 0x454) = SHENMUE_ACTION_VALUE(float, actor, 0x458) = SHENMUE_ACTION_VALUE(float, actor, 0x45c) = 0.0f;
        SHENMUE_ACTION_VALUE(float, actor, 0x46c) = SHENMUE_ACTION_VALUE(float, actor, 0x470) = SHENMUE_ACTION_VALUE(float, actor, 0x474) = 0.0f;
        SHENMUE_ACTION_VALUE(float, actor, 0x460) = SHENMUE_ACTION_VALUE(float, actor, 0x464) = SHENMUE_ACTION_VALUE(float, actor, 0x468) = 0.0f;
        actor->position.x = actor->targetPosition.x;
        actor->position.y = actor->targetPosition.y;
        actor->position.z = actor->targetPosition.z;
        actor->contactPosition.x = actor->targetPosition.x;
        actor->contactPosition.y = actor->targetPosition.y;
        actor->contactPosition.z = actor->targetPosition.z;
        actor->facingAngle = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x64);
        const std::uint16_t animation = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x42c) & 0xfffU;
        const shen_name map = g_currentMap;
        if (map == SHEN_NAME(TGLB) && actor->game->spawnPointCount && static_cast<std::uint16_t>(animation - 2071) <= 1)
            ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        else
            SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 2;
        return;
    }
    case 1:
        if (!(static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2f6)) < actor->animationTime))
            return;
        camCtrlSetInterpModeTo(8);
        vfSetupLinkedFightCamera(actor->actor, 2);
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    case 2:
        if (actor->activeMotionFlags & 0xc00U) {
            ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
            return;
        }
        if (actor->animationTime < static_cast<float>(actor->motionFrames))
            return;
        mtwkSetExtraFlag(actor->motion, 1, 2);
        actor->moveFlags &= ~0x200U;
        actor->queuedAction = 0;
        return;
    case 3: {
        if (actor->animationTime < static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28c)))
            return;
        const std::uint16_t requested = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28e);
        if (!requested || !(actor->secondaryMotionFlags & 0xc00U))
            return;
        if (requested == 0xffffU)
            SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x318) = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x42c) + 0x8000000U;
        else
            SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x318) = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28e) + 0x8000000U;
        mtwkSetExtraFlag(actor->motion, 1, 2);
        actor->queuedAction = 0;
        actor->moveFlags &= ~0x10U;
        return;
    }
    default:
        return;
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdatePairedDamage(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const source = actor->secondActor;
    FightRuntimePrefix *const game = actor->game;
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    case 0: {
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x328);
        SHENMUE_ACTION_VALUE(float, actor, 0x278) = actor->game->knockbackAmount;
        actor->stateFlags |= 0x200U;
        vfStartQueuedReactionAnimation(actor);
        SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x946) = source->facingAngleDelta;
        actor->moveFlags |= 0x10U;
        SHENMUE_ACTION_VALUE(float, actor, 0x454) = SHENMUE_ACTION_VALUE(float, actor, 0x458) = SHENMUE_ACTION_VALUE(float, actor, 0x45c) = 0;
        SHENMUE_ACTION_VALUE(float, actor, 0x46c) = SHENMUE_ACTION_VALUE(float, actor, 0x470) = SHENMUE_ACTION_VALUE(float, actor, 0x474) = 0;
        SHENMUE_ACTION_VALUE(float, actor, 0x460) = SHENMUE_ACTION_VALUE(float, actor, 0x464) = SHENMUE_ACTION_VALUE(float, actor, 0x468) = 0;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        if (IsAuthFightCamera()) {
            source->targetPosition.x = SHENMUE_ACTION_VALUE(float, game, 0x45c);
            source->targetPosition.y = SHENMUE_ACTION_VALUE(float, game, 0x460);
            source->targetPosition.z = SHENMUE_ACTION_VALUE(float, game, 0x464);
            SHENMUE_ACTION_VALUE(std::uint16_t, source, 0x64) = SHENMUE_ACTION_VALUE(std::uint16_t, game, 0x468);
            actor->moveFlags |= 0x10000000U;
        }
        actor->position.x = source->targetPosition.x;
        actor->position.y = source->targetPosition.y;
        actor->position.z = source->targetPosition.z;
        actor->facingAngle = SHENMUE_ACTION_VALUE(std::uint16_t, source, 0x64);
        actor->contactPosition.x = source->targetPosition.x;
        actor->contactPosition.y = source->targetPosition.y;
        actor->contactPosition.z = source->targetPosition.z;
        return;
    }
    case 1: {
        if (actor->animationTime < static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2a0)))
            return;
        int damage =
            source->secondaryMotionFlags & 0x400U ? SHENMUE_ACTION_VALUE(std::uint8_t, source, 0x2c1) : SHENMUE_ACTION_VALUE(std::uint8_t, source, 0x2aa);
        damage = static_cast<std::uint16_t>(damage + SHENMUE_ACTION_VALUE(std::int16_t, source, 0x308));
        const std::uint16_t selection = static_cast<std::uint16_t>(actor->game->formations[SHENMUE_ACTION_VALUE(std::int8_t, actor, 0)]->selectFlags);
        if ((((selection & 0x80U) && (source->secondaryMotionFlags & 0x4000008U)) || ((selection & 0x20U) && (source->secondaryMotionFlags & 0x4000000U))) &&
            !(g_pFightStage->battleFlags & 0x20U))
            damage = static_cast<std::uint16_t>(static_cast<std::int32_t>(actor->hp) + 1);
        if (SHENMUE_ACTION_VALUE(std::uint16_t, actor, 2) == 18) {
            const std::byte *slotParameter = static_cast<std::byte *>(actor->slot);
            slotParameter += 0xf8;
            if (static_cast<std::uint32_t>(static_cast<std::int8_t>(*slotParameter)) > 1U)
                damage = static_cast<std::uint16_t>(static_cast<std::int32_t>(actor->hp) + 1);
        }
        const std::byte *profile = static_cast<std::byte *>(actor->slot);
        profile += 0x175;
        switch (static_cast<std::uint8_t>(*profile)) {
        case 0:
            break;
        case 2:
            if ((actor->savedMotionFlags & 0x40000U) && (source->motionFlags & 0x100000U))
                SHENMUE_ACTION_VALUE(float, actor, 0x9dc) = 20;
            break;
        case 3:
            if (actor->secondaryMotionFlags & 0x10U)
                SHENMUE_ACTION_VALUE(float, actor, 0x9dc) = 3;
            break;
        }
        if (actor->savedMotionFlags & 0x80000U)
            SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3a8) += damage;
        else
            SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3a8) = damage;
        vfHandleImpactSfxAndDefeat(actor, static_cast<std::int16_t>(damage), 0);
        if (actor->secondaryMotionFlags & 0xc00U)
            ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        else
            SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 3;
        return;
    }
    case 2: {
        if (actor->animationTime < static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28c)))
            return;
        const std::uint16_t animation = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28e);
        if (animation && animation != 0xffffU && (actor->secondaryMotionFlags & 0xc00U))
            SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x318) = animation + 0x8000000U;
    }
    case 3:
        break;
    default:
        return;
    }
    if (actor->animationTime < static_cast<float>(actor->motionFrames))
        return;
    if (IsAuthFightCamera() && !(actor->moveFlags & 0x20U) && !(actor->stateFlags & 0x80U))
        vfHandleImpactSfxAndDefeat(actor, 10000, 0);
    if ((actor->moveFlags & 0x20U) || !(actor->hp > 0)) {
        actor->hitStateFlags = (actor->hitStateFlags | 0x14000U) & ~0x80000U;
        return;
    }
    if (SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x29c) == 4 || (actor->hitStateFlags & 0x4000U)) {
        const std::uint16_t damage = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3a8);
        std::uint8_t frames = damage > 14 ? static_cast<std::uint8_t>((static_cast<float>(damage) - 14 + 0.5f) * 0.25f) : 1;
        if (frames > 30)
            frames = 30;
        SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3a0) = static_cast<std::uint16_t>(SHENMUE_ACTION_VALUE(float, actor, 0x270) * static_cast<float>(frames));
        SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x318) = 0x6000000U;
    } else {
        actor->queuedAction = 0;
        actor->moveFlags &= ~0x10U;
        actor->stateFlags &= ~0x200U;
    }
}

} // namespace

#else

namespace {

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
                                                                                  static_cast<std::uint8_t>(source->hitDamage);
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateGuardImpactHold(FightActorRuntimePrefix *actor) {
    SHENMUE_ACTION_VALUE(float, actor, 0x424) += SHENMUE_ACTION_VALUE(float, actor, 0x26c) * 0.8f;
    const std::int16_t timer = DecrementActionHold(actor);
    std::uint8_t &phase = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    FightActorRuntimePrefix *const other = actor->actor;
    switch (phase) {
    default:
    case 0: {
        const std::uint32_t hit = actor->hitStateFlags;
        actor->moveFlags |= 0x10U;
        SHENMUE_ACTION_VALUE(float, actor, 0x454) = SHENMUE_ACTION_VALUE(float, actor, 0x46c) * 1.5f;
        SHENMUE_ACTION_VALUE(float, actor, 0x45c) = SHENMUE_ACTION_VALUE(float, actor, 0x474) * 1.5f;
        SHENMUE_ACTION_VALUE(float, actor, 0x460) = SHENMUE_ACTION_VALUE(float, actor, 0x46c) * 4;
        SHENMUE_ACTION_VALUE(float, actor, 0x468) = SHENMUE_ACTION_VALUE(float, actor, 0x474) * 4;
        std::int32_t animation = static_cast<std::uint16_t>(actor->queuedAction);
        if (!animation)
            animation = SHENMUE_ACTION_ANIMATION(actor, hit & 8U ? 20 : 16);
        SHENMUE_INSTALL_ACTION_ANIMATION(actor, animation);
        actor->hitStateFlags |= 1U;
        if (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x288)) {
            if (actor->motionFrames > SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28c))
                actor->motionFrames = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28c);
            actor->animationTime = SHENMUE_ACTION_VALUE(float, actor, 0x450) = static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x290));
        } else {
            SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x290) = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x292) = actor->motionFrames = 0;
        }
        ++phase;
    }
    case 1:
        if (static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x292)) - SHENMUE_ACTION_VALUE(float, actor, 0x26c) > actor->animationTime)
            return;
        ++phase;
    case 2: {
        const std::int16_t difference = static_cast<std::int16_t>(timer - actor->motionFrames);
        const float middle = static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x292)) - SHENMUE_ACTION_VALUE(float, actor, 0x26c);
        if (static_cast<std::int16_t>(static_cast<float>(difference) + middle) >= 0) {
            actor->animationTime = SHENMUE_ACTION_VALUE(float, actor, 0x450) = middle;
            break;
        }
        ++phase;
    }
    case 3:
        if (timer <= 0) {
            actor->queuedAction = 0;
            actor->hitStateFlags &= ~1U;
            actor->moveFlags &= ~0x10U;
        }
        break;
    }
    if (SHENMUE_ACTION_VALUE(std::int16_t, other, 0x3d4) != SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3da) && (other->groundContactFlags & 3U)) {
        const std::int32_t animation = vfTryGuardReactionSwap(actor, other);
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = animation;
        if (animation) {
            SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x32a) = 1;
            phase = 3;
        }
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void QueueBallisticAction(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const other = actor->actor;
    std::uint32_t action = SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x320);
    std::uint8_t &phase = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    bool selectedFallback = false;
    if (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x323) != SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x32b) || !(actor->hitStateFlags & 0x1000U))
        phase = 0;
    switch (phase) {
    case 0:
        SHENMUE_ACTION_VALUE(float, actor, 0x260) = action & 0x10000U ? 1.0f : 0.0f;
        if (action & 0x20000U)
            SHENMUE_ACTION_VALUE(float, actor, 0x260) = -1;
        break;
    case 1:
        action |= actor->queuedAction & 0xfff8ffffU;
        break;
    case 2: {
        const std::uint16_t animation = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x320);
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = animation;
        if (animation) {
            if (action & 0x80000U) {
                const std::byte *const extra = MOTN_FindAnimationExtraRecord(animation & 0xfffU, 3);
                SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3c2) = static_cast<std::uint16_t>(*reinterpret_cast<const std::int16_t *>(extra + 6) - 1);
            } else
                vfStartFormationAnimation(actor);
        }
        actor->queuedAction = action;
        return;
    }
    default:
        break;
    }
    const std::uint16_t requested = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x320);
    float firstFrame = 0;
    if (requested) {
        const std::byte *const extra = MOTN_FindAnimationExtraRecord(requested & 0xfffU, 3);
        firstFrame = static_cast<float>(*reinterpret_cast<const std::int16_t *>(extra + 2)) - 1;
    }
    if (firstFrame != 0)
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x320);
    else {
        const std::size_t offset = !(action & 0x40000U) ? 60 : (action & 0x20000U ? 64 : 62);
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = SHENMUE_ACTION_ANIMATION(actor, offset);
        selectedFallback = requested != 0;
    }
    if (!phase || SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x42c) != SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434)) {
        firstFrame = actor->animationTime;
        SHENMUE_ACTION_VALUE(float, actor, 0x278) = SHENMUE_ACTION_VALUE(float, actor, 0x274);
        vfStartFormationAnimation(actor);
        const float first = static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x296));
        if ((other->hitStateFlags & 0x102U) == 0x102U && first > static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, other, 0x290)) - other->animationTime)
            actor->animationTime = SHENMUE_ACTION_VALUE(float, actor, 0x450) = firstFrame = first;
        if (!(actor->hitStateFlags & 0x1000U)) {
            actor->animationTime = SHENMUE_ACTION_VALUE(float, actor, 0x450) = 1;
            actor->queuedAction = action & 0xfcfbffffU;
            return;
        }
        if (phase == 1) {
            if (firstFrame > first)
                firstFrame = first;
            actor->animationTime = SHENMUE_ACTION_VALUE(float, actor, 0x450) = firstFrame;
        }
    }
    if (action & 0x100000U)
        SHENMUE_ACTION_VALUE(Vector3, actor, 0x14c) = other->contactPosition;
    if (!selectedFallback && (action & 0x80000U)) {
        const std::byte *const extra = MOTN_FindAnimationExtraRecord(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x320) & 0xfffU, 3);
        SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3c2) = *reinterpret_cast<const std::uint16_t *>(extra + 6);
    }
    actor->queuedAction = action;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline float FightSquareRoot(float value) {
    return __builtin_sqrtf(value);
}

// @verified - d
void UpdateBallisticAction(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *const game = actor->game;
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    case 0:
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    case 1: {
        if (actor->animationTime < static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x296)))
            return;
        actor->hitStateFlags &= ~2U;
        SHENMUE_ACTION_VALUE(float, actor, 0x3fc) = actor->targetGroundY;
        std::int16_t frames;
        const std::uint16_t last = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2a0);
        if (last)
            frames = static_cast<std::int16_t>(
                SHENMUE_ACTION_VALUE(float, actor, 0x270) * static_cast<float>(last - SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x296) - 2));
        else
            frames = static_cast<std::int16_t>(SHENMUE_ACTION_VALUE(float, actor, 0x270) * 16.0f);
        FightActorRuntimePrefix *target;
        if (actor->animationTime + static_cast<float>(frames) < static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, target = actor->actor, 0x292)))
            frames = static_cast<std::int16_t>(
                SHENMUE_ACTION_VALUE(float, actor, 0x270) * (static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, target, 0x292)) - actor->animationTime));
        SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x3be) = frames;
        const float height = SHENMUE_ACTION_VALUE(float, actor, 0x2a4) + SHENMUE_ACTION_VALUE(float, actor, 0x3fc) - actor->contactPosition.y;
        float count = static_cast<float>(frames);
        const float speed = actor->game->knockbackAmount;
        float gravity = speed * speed * game->gravityScale - SHENMUE_ACTION_VALUE(float, actor, 0x464);
        if (!(actor->queuedAction & 0x100000U)) {
            const float midpoint = 0.5f * count + 0.5f;
            SHENMUE_ACTION_VALUE(float, actor, 0x458) = height / count + gravity * midpoint;
            const float direction = SHENMUE_ACTION_VALUE(float, actor, 0x260);
            float distance = count * 0.1f;
            float lateral;
            if (direction != 0) {
                float contact, maximum;
                if (direction == 1) {
                    maximum = SHENMUE_ACTION_VALUE(float, actor, 0x3ac);
                    contact = actor->contactDistances[SHENMUE_ACTION_VALUE(std::int8_t, actor, 1)];
                    if (actor->moveFlags & 1U)
                        contact = -contact;
                } else {
                    maximum = SHENMUE_ACTION_VALUE(float, actor, 0x3b0);
                    contact = actor->contactDistances[SHENMUE_ACTION_VALUE(std::int8_t, actor, 1)];
                    if ((actor->moveFlags & 1U) == 0)
                        contact = -contact;
                }
                lateral = maximum * 0.6f;
                if (distance > lateral)
                    distance = lateral;
                if (contact > 0 && distance > contact - 0.9f && contact + 0.9f > distance)
                    distance = contact - 0.9f;
            }
            lateral = (distance / count) * direction;
            if (!(lateral < 0))
                SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x17c) = actor->facingAngle;
            else
                SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x17c) = static_cast<std::int16_t>(actor->facingAngle) ^ 0x8000;
            SHENMUE_ACTION_VALUE(float, actor, 0x454) =
                actor->game->knockbackAmount * (-CalculateFixedSine(static_cast<std::int16_t>(actor->facingAngle)) * lateral);
            SHENMUE_ACTION_VALUE(float, actor, 0x45c) =
                actor->game->knockbackAmount * (-CalculateFixedCosine(static_cast<std::int16_t>(actor->facingAngle)) * lateral);
        } else {
            float delta, x, z, length, reach;
            FightActorRuntimePrefix *other;
            other = actor->actor;
            delta = SHENMUE_ACTION_VALUE(float, other, 0x404) - SHENMUE_ACTION_VALUE(float, actor, 0x404);
            SHENMUE_ACTION_VALUE(float, actor, 0x458) = (delta + (gravity * 0.5f * count) * count) / count;
            if (SHENMUE_ACTION_VALUE(float, actor, 0x458) < 0.07f) {
                SHENMUE_ACTION_VALUE(float, actor, 0x458) = 0.07f;
                count = (FightSquareRoot((gravity + gravity) * -delta + 0.0025f) + 0.05f) / gravity;
                SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3be) = static_cast<std::uint16_t>(count + 0.5f);
            }
            Vector3 impulse, rotated;
            impulse.x = SHENMUE_ACTION_VALUE(float, actor, 0x2b4);
            impulse.z = SHENMUE_ACTION_VALUE(float, actor, 0x2b8);
            vfRotateFightVector(static_cast<std::int16_t>(actor->facingAngle), &impulse, &rotated);
            x = SHENMUE_ACTION_VALUE(float, actor, 0x14c) - rotated.x - actor->contactPosition.x;
            z = SHENMUE_ACTION_VALUE(float, actor, 0x154) + rotated.z - actor->contactPosition.z;
            length = FightSquareRoot(x * x + z * z);
            reach = SHENMUE_ACTION_VALUE(float, actor, 0x41c);
            if (length > reach) {
                reach -= 0.4f;
                const float scale = reach / length;
                x *= scale;
                z *= scale;
            }
            SHENMUE_ACTION_VALUE(float, actor, 0x454) = x / count;
            SHENMUE_ACTION_VALUE(float, actor, 0x45c) = z / count;
            SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x17c) = static_cast<std::uint16_t>(dc::sdk::ArcTan2Fixed(x, z));
        }
        vfSnapActorToContact(actor);
        vfPredictLandingGround(actor);
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        vfPlayFightSfx(35, actor);
        break;
    }
    case 2:
        if ((actor->queuedAction & 0x80000U) && SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3c2) >= SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3be)) {
            SHENMUE_INSTALL_ACTION_ANIMATION(actor, SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x328));
            actor->queuedAction &= ~0x80000U;
        }
        if (!(actor->moveFlags & 4U)) {
            vfPlayFightSfx(40, actor);
            SHENMUE_ACTION_VALUE(float, actor, 0x454) = SHENMUE_ACTION_VALUE(float, actor, 0x458) = SHENMUE_ACTION_VALUE(float, actor, 0x45c) = 0;
            actor->queuedAction = 0;
            if (!(actor->animationTime > static_cast<float>(actor->motionFrames)))
                actor->hitStateFlags |= 1U;
        }
        break;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void QueueCameraRelativeDodge(FightActorRuntimePrefix *actor) {
    std::uint16_t offsets[8] = {30, 32, 236, 238, 12, 14, 236, 238};
    if (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x323) == SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x32b))
        return;
    FightRuntimePrefix *const game = actor->game;
    const std::uint32_t ground = actor->groundContactFlags;
    const std::uint32_t mirrored = actor->moveFlags & 0x400U;
    FightActorRuntimePrefix *const other = actor->actor;
    std::int16_t facing;
    if (actor->groupIndex == game->playerSlot && !(game->controlFlags & 0x400000U))
        facing = SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x9b4);
    else
        facing = SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x9b0);
    std::uint16_t index;
    if (!(ground & 1U))
        index = !(std::fabs(static_cast<float>(static_cast<std::int16_t>(facing - 0x4000))) > 8192) ? !mirrored : mirrored != 0;
    else if (!(std::fabs(static_cast<float>(static_cast<std::int16_t>(facing + 0x4000))) > 8192))
        index = mirrored ? 2 : 3;
    else
        index = mirrored ? 3 : 2;
    if (((other->hitStateFlags & 0x100U) && static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, other, 0x290)) - other->animationTime > 5) ||
        (other->secondaryMotionFlags & 2U))
        index += 4;
    std::uint16_t animation = static_cast<std::uint16_t>(SHENMUE_ACTION_ANIMATION(actor, offsets[index]));
    if (!animation && index > 3)
        animation = static_cast<std::uint16_t>(SHENMUE_ACTION_ANIMATION(actor, offsets[index - 4]));
    SHENMUE_INSTALL_ACTION_ANIMATION(actor, animation);
    actor->queuedAction = SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x320);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateReverseRecovery(FightActorRuntimePrefix *actor) {
    const std::uint8_t phase = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    if (phase == 1) {
        if (!(9 > actor->animationTime))
            return;
        actor->hitStateFlags |= 4U;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    } else if (phase != 2) {
        vfStartFormationAnimation(actor);
        actor->hitStateFlags &= ~0x40000U;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        return;
    }
    actor->animationTime -= SHENMUE_ACTION_VALUE(float, actor, 0x26c) * 2;
    if (!(actor->animationTime > 0))
        actor->animationTime = 0;
    SHENMUE_ACTION_VALUE(float, actor, 0x450) = actor->animationTime;
    if (!(actor->animationTime > 0)) {
        actor->hitStateFlags &= ~1U;
        actor->queuedAction = 0;
        SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28c) = 1;
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateTimedGuard(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const other = actor->actor;
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    case 0: {
        SHENMUE_INSTALL_ACTION_ANIMATION(actor, SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x328));
        const float remaining = static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, other, 0x290)) - other->animationTime;
        const float first = static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x290));
        if (first - SHENMUE_ACTION_VALUE(float, actor, 0x26c) > remaining)
            actor->animationTime = first - remaining;
        SHENMUE_ACTION_VALUE(float, actor, 0x450) = actor->animationTime;
        vfPlayFightSfx(35, actor);
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        break;
    }
    case 1:
        if (actor->animationTime < static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x292)))
            break;
        if ((other->hitStateFlags & 0x104U) != 0x100U)
            ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        else
            actor->animationTime -= SHENMUE_ACTION_VALUE(float, actor, 0x26c);
        break;
    case 2:
        if (!(actor->animationTime < static_cast<float>(actor->motionFrames)))
            actor->queuedAction = 0;
        break;
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateAuthoredDefeat(FightActorRuntimePrefix *actor) {
    std::int32_t animation = 0;
    MotionWork *const motion = actor->motion;
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    case 0:
        actor->moveFlags |= 0x10U;
        if (!(actor->hp > 0))
            SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 2;
        if ((actor->hitStateFlags & 0x814200U) == 0x200U) {
            mtwkSetExtraFlag(motion, 0, 0);
            mtwkRemoveCallback(motion);
            mtwkSetSpeed(motion, 1, static_cast<MotionRegion>(0));
            ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        }
        break;
    case 1:
        switch (actor->game->elapsedTicks % 3U) {
        case 0:
            animation = 395;
            break;
        case 1:
            animation = 397;
            break;
        case 2:
            animation = 399;
            break;
        }
        SHENMUE_INSTALL_ACTION_ANIMATION(actor, animation);
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        break;
    case 2:
        break;
    }
}

} // namespace

#else

namespace {

// @verified - p
void UpdateAuthoredDefeat(VFActor *self) {
    if (!self->hitPhase) {
        self->moveFlags |= VFMF_HitReactionLock;
        if (self->hp <= 0)
            self->hitPhase = 2;
        if ((self->hitStateFlags & (VFHSF_AttackGuardWindow | VFHSF_SpecialGuardOrParry | VFHSF_GuardState | VFHSF_AlignedToGround)) == 0x200) {
            mtwkSetExtraFlag(self->motion, 0, 0);
            mtwkRemoveCallback(self->motion);
            mtwkSetSpeed(self->motion, 1, MotionRegion::LowerBody);
            ++self->hitPhase;
        }
    } else if (self->hitPhase == 1) {
        const int frame = int(self->game->frame) % 3;
        Install(self, frame == 0 ? 395 : (frame == 1 ? 397 : (frame == 2 ? 399 : 0)));
        ++self->hitPhase;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float reactionHoldContactRate = 0.8f;

// @verified - d
void UpdateReactionHold(FightActorRuntimePrefix *actor) {
    SHENMUE_ACTION_VALUE(float, actor, 0x424) += SHENMUE_ACTION_VALUE(float, actor, 0x26c) * reactionHoldContactRate;
    if (--SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x3a0) < 0)
        SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x3a0) = 0;
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    case 0: {
        SHENMUE_ACTION_VALUE(float, actor, 0x454) = 0;
        SHENMUE_ACTION_VALUE(float, actor, 0x45c) = 0;
        SHENMUE_ACTION_VALUE(float, actor, 0x460) = 0;
        SHENMUE_ACTION_VALUE(float, actor, 0x468) = 0;
        const std::uint16_t animation = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x328);
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = animation;
        SHENMUE_ACTION_VALUE(float, actor, 0x278) = SHENMUE_ACTION_VALUE(float, actor, 0x274);
        vfStartFormationAnimation(actor);
        actor->hitStateFlags |= 1U;
        if (!SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x288)) {
            SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x290) = 0;
            SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x292) = static_cast<std::uint16_t>(SHENMUE_ACTION_VALUE(float, actor, 0x43c));
            actor->motionFrames = static_cast<std::uint16_t>(SHENMUE_ACTION_VALUE(float, actor, 0x43c));
        }
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        return;
    }
    case 1:
        if (actor->animationTime < static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x292)))
            return;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    case 2:
        if (actor->stateFlags & 0x800U)
            actor->stateFlags &= ~0x800U;
        else if (actor->animationTime < static_cast<float>(actor->motionFrames))
            return;
    case 3:
        actor->queuedAction = 0;
        actor->hitStateFlags &= ~1U;
        return;
    default:
        return;
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

const std::uint16_t recoveryAnimationOffsets[4] = {70, 74, 72, 76};

// @verified - d
void UpdateRecovery(FightActorRuntimePrefix *actor) {
    const std::uint8_t orientation = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x39d);
    std::uint32_t action = actor->queuedAction;
    const std::byte *const animations = SHENMUE_ACTION_VALUE(const std::byte *, actor, 0x38);
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    case 0:
        SHENMUE_ACTION_VALUE(void *, actor, 0x5b0) = 0;
        actor->hitStateFlags |= 0x10000U;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    case 1: {
        if (!(actor->animationTime > static_cast<float>(actor->motionFrames)))
            return;
        std::int32_t animation;
        if (actor->motionFlags & 0x4000000U)
            animation = actor->motionFlags & 0x800U ? 452 : 453;
        else if (orientation & 1U)
            animation = static_cast<std::uint16_t>(static_cast<std::uint8_t>(animations[67]) | (static_cast<std::int8_t>(animations[66]) << 8));
        else
            animation = static_cast<std::uint16_t>(static_cast<std::uint8_t>(animations[69]) | (static_cast<std::int8_t>(animations[68]) << 8));
        SHENMUE_INSTALL_ACTION_ANIMATION(actor, animation);
        if ((actor->moveFlags & 0x20U) || !(actor->hp > 0)) {
            SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 5;
            return;
        }
        SHENMUE_ACTION_VALUE(float, actor, 0x278) = 1 - static_cast<float>(SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x3a0)) * 0.03f;
        if (SHENMUE_ACTION_VALUE(float, actor, 0x278) < 0.5f)
            SHENMUE_ACTION_VALUE(float, actor, 0x278) = 0.5f;
        SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x39e) = 0;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    }
    case 2: {
        if (SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x3a0)-- >
            static_cast<int>(SHENMUE_ACTION_VALUE(float, actor, 0x270) * static_cast<float>(4 * SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x39e))))
            return;
        SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3a0) = 0;
        SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3c6) = 0;
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x3dd) = 0;
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x3dc) = 0;
        actor->moveFlags &= ~0x10U;
        actor->stateFlags &= ~0x200U;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    }
    case 3: {
        actor->moveFlags &= 0xdfffffbfU;
        action &= 0xfffffe0fU;
        std::uint16_t index = 0;
        if ((orientation & 1U) == 0) {
            action |= 0x200U;
            index = 1;
        }
        if (!(orientation & 2U)) {
            action |= 0x400U;
            index += 2;
        }
        index = recoveryAnimationOffsets[index];
        SHENMUE_INSTALL_ACTION_ANIMATION(actor, static_cast<std::uint8_t>((animations + index)[1]) | (static_cast<std::int8_t>(animations[index]) << 8));
        actor->queuedAction = action;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    }
    case 4:
        if (actor->hitStateFlags & 4U)
            actor->queuedAction = 0;
        return;
    case 5:
        return;
    default:
        return;
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateContactFacingMotion(FightActorRuntimePrefix *actor) {
    const std::uint32_t mirrored = actor->moveFlags & 0x400U;
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    case 0: {
        std::uint16_t offset;
        std::uint8_t direction;
        switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x883)) {
        case 0:
        case 1:
        case 7:
            offset = 246;
            direction = 8;
            break;
        case 3:
        case 4:
        case 5:
            offset = 248;
            direction = 16;
            break;
        case 6:
            if (mirrored) {
                offset = 252;
                direction = 64;
            } else {
                offset = 250;
                direction = 32;
            }
            break;
        case 2:
            if (mirrored) {
                offset = 250;
                direction = 32;
            } else {
                offset = 252;
                direction = 64;
            }
            break;
        default:
            return;
        }
        if ((actor->hitStateFlags & 0x400U) && !(actor->animationTime > static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x292)))) {
            if (direction & SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94a) & 8U)
                offset = 286;
            else if (direction & SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94a) & 16U)
                offset = 290;
        }
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94b) = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94a);
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94a) = direction;
        SHENMUE_INSTALL_ACTION_ANIMATION(actor,
            static_cast<std::uint8_t>((SHENMUE_ACTION_VALUE(const std::byte *, actor, 0x38) + offset)[1]) |
                (static_cast<std::int8_t>(SHENMUE_ACTION_VALUE(const std::byte *, actor, 0x38)[offset]) << 8));
        SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x864) = 1;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        break;
    }
    case 1:
        if (!(actor->animationTime < static_cast<float>(actor->motionFrames)))
            actor->queuedAction = 0;
        break;
    }
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateLocomotion(FightActorRuntimePrefix *actor) {
    std::uint8_t previous = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x948);
    bool keepMotion = false, stationary = false;
    std::int8_t &mode = SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x94e);
    if (SHENMUE_ACTION_VALUE(std::uint16_t, actor, 2) == 1)
        mode = 1;
    const std::int8_t index = SHENMUE_ACTION_VALUE(std::int8_t, actor, 0);
    std::uint16_t &input = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x490);
    if (mode == 3) {
        if (!(input & 0xf0U)) {
            input = 32;
            previous = 0;
        }
        const bool perActor = (actor->game->controlFlags & 0x40000U) != 0;
        const LegacyInputSlot &pad = g_inputBuffer[perActor ? index : 0];
        if ((perActor || !actor->game->slotPoolModes[index]) && pad.triggers[1] <= 249 && pad.triggers[0] <= 249)
            mode = 1;
    }
    if (mode == 2)
        input = 0;
    std::int32_t animation;
    if (!(input & 0xf0U)) {
        stationary = true;
        if ((actor->motionFlags & 4U) || ((actor->motionFlags & 2U) && !mtwkSetRegionFlag(actor->motion, 65, 1, static_cast<MotionRegion>(0))))
            return;
        if (!SHENMUE_ACTION_VALUE(std::uint16_t, actor, 2) &&
            (static_cast<float>(actor->initialHp) * 0.5f > actor->hp || SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x9d6)))
            animation = actor->moveFlags & 0x400U ? 32826 : 59;
        else
            animation = SHENMUE_ACTION_ANIMATION(actor, actor->moveFlags & 0x400U ? 320 : 318);
        if (mode == 1)
            animation = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x9b6);
    } else {
        if ((actor->activeMotionFlags & 0x8000000U) && (SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x9bc) != 1 || (actor->motionFlags & 4U)) &&
            (mode != 3 || (actor->motionFlags & 0x20000000U))) {
            const std::uint16_t current = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x42c) & 0xfffU;
            if (current != 61 && current != 286) {
                static const std::int16_t turns[8] = {0, 1024, 2048, 3072, 4096, -3072, -2048, -1024};
                actor->facingAngle = static_cast<std::uint16_t>(
                    actor->facingAngle + static_cast<std::int32_t>(static_cast<float>(turns[previous]) * SHENMUE_ACTION_VALUE(float, actor, 0x26c)));
            }
            animation = SHENMUE_ACTION_ANIMATION(actor, mode == 1 ? 314 : 316);
            if (mtwkSetRegionFlag(actor->motion, 65, 1, static_cast<MotionRegion>(0)))
                keepMotion = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28e) == 0xffffU;
            else if (!(actor->motionFlags & 4U))
                return;
        } else if (!SHENMUE_ACTION_VALUE(std::uint16_t, actor, 2) && previous == 4)
            animation = mode == 1 ? 61 : 286;
        else
            animation = SHENMUE_ACTION_ANIMATION(actor, mode == 1 ? 314 : 316);
    }
    SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = animation;
    SHENMUE_ACTION_VALUE(float, actor, 0x278) = SHENMUE_ACTION_VALUE(float, actor, 0x274);
    SHENMUE_ACTION_VALUE(float, actor, 0x278) *= SHENMUE_ACTION_VALUE(float, actor, 0x428);
    actor->moveFlags &= ~0x40U;
    if (!stationary) {
        actor->motionFlags &= ~0x200000U;
        actor->hitStateFlags &= ~0x200000U;
    }
    if (!keepMotion)
        vfStartFormationAnimation(actor);
    actor->queuedAction = 0;
}

} // namespace

#else

namespace {

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
        if (!(self->inputFlags & 0xf0)) {
            previous = 0;
            self->inputFlags = 32;
        }
        const auto &game = *self->game;
        const auto &input = g_inputBuffer[(game.gameFlags & 0x40000) ? self->groupIndex : 0];
        if (((game.gameFlags & 0x40000) || !game.slotPoolModes[self->groupIndex]) && input.analog0d < 250 && input.analog0c < 250) {
            mode = 1;
            self->reactionMovement.state.mode = mode;
        }
    }
    if (mode == 2)
        self->inputFlags = 0;
    std::int32_t animation;
    if (self->inputFlags & 0xf0) {
        if ((self->activeMotionFlags & VFSMF_Unknown8000000) && (self->reactionEvaluationPhase != 1 || (self->motionFlags & VFMOF_Unknown4)) &&
            (mode != 3 || (self->motionFlags & VFMOF_Unknown20000000))) {
            const auto id = self->currentAnimation & 0xfff;
            if (id != 61 && id != 286) {
                constexpr std::int16_t turns[8]{0, 1024, 2048, 3072, 4096, -3072, -2048, -1024};
                self->facingAngle = static_cast<std::uint16_t>(self->facingAngle + int(float(turns[previous]) * self->scale));
            }
            animation = Animation(self, mode == 1 ? 314 : 316);
            if (mtwkSetRegionFlag(self->motion, 65, 1, MotionRegion::LowerBody))
                keepMotion = self->requestedAnimation == -1;
            else if (!(self->motionFlags & VFMOF_Unknown4))
                return;
        } else if (self->fightStyle == 0 && previous == 4)
            animation = mode == 1 ? 61 : 286;
        else
            animation = Animation(self, mode == 1 ? 314 : 316);
    } else {
        stationary = true;
        if ((self->motionFlags & VFMOF_Unknown4) || ((self->motionFlags & VFMOF_Unknown2) && !mtwkSetRegionFlag(self->motion, 65, 1, MotionRegion::LowerBody)))
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void UpdateTargetLocomotion(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const target = actor->game->actors[SHENMUE_ACTION_VALUE(std::int8_t, actor, 1)];
    const std::int16_t direction = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(
        actor->contactPosition.x - SHENMUE_ACTION_VALUE(float, actor, 0x950), actor->contactPosition.z - SHENMUE_ACTION_VALUE(float, actor, 0x958)));
    if (SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x9a4) != -1)
        SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x94f) = 0;
    const std::int16_t difference = static_cast<std::int16_t>(direction - actor->facingAngle);
    if (SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x94f) == 0) {
        if (difference > 4096)
            SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x94f) = 1;
        else if (difference < -4096)
            SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x94f) = 2;
    } else if (!(std::fabs(static_cast<float>(difference)) > 4096))
        SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x94f) = 0;
    if (SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x94f) == 1)
        actor->facingAngle = static_cast<std::uint16_t>(actor->facingAngle + 4096);
    if (SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x94f) == 2)
        actor->facingAngle = static_cast<std::uint16_t>(actor->facingAngle - 4096);
    std::int32_t animation;
    std::uint8_t alternate;
    const std::byte *const motions = SHENMUE_ACTION_VALUE(const std::byte *, actor, 0x38);
    if (SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x94e) == 1) {
        animation = static_cast<std::uint8_t>(motions[315]) | static_cast<std::int8_t>(motions[314]) << 8;
        alternate = 0;
    } else {
        animation = static_cast<std::uint8_t>(motions[317]) | static_cast<std::int8_t>(motions[316]) << 8;
        alternate = 1;
    }
    if (actor->activeMotionFlags & 0x8000000U) {
        const std::uint32_t flags = actor->motionFlags;
        if ((flags & 4U) == 0) {
            const std::uint8_t unflagged = (flags & 0x400U) == 0;
            if (!mtwkSetRegionFlag(actor->motion, 65, 1, static_cast<MotionRegion>(0)) && unflagged == alternate)
                return;
        }
    }
    SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = animation;
    actor->moveFlags &= ~0x40U;
    if (alternate == 1) {
        if ((target->hitStateFlags & 0x20000000U) && (target->groundContactFlags & 1U) && (actor->groundContactFlags & 1U) == 0)
            SHENMUE_ACTION_VALUE(float, actor, 0x278) = SHENMUE_ACTION_VALUE(float, actor, 0x274) * 1.2f;
        else
            SHENMUE_ACTION_VALUE(float, actor, 0x278) = SHENMUE_ACTION_VALUE(float, actor, 0x274) * 0.85f;
    }
    vfStartFormationAnimation(actor);
    actor->queuedAction = 0;
}

} // namespace

#else

namespace {

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
        mtwkSetRegionFlag(self->motion, 65, 1, MotionRegion::LowerBody) || (!(self->motionFlags & VFMOF_ContactCategoryA) != alternate)) {
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateDirectionalMotion(FightActorRuntimePrefix *actor) {
    const std::uint8_t oldDirection = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94a);
    const std::uint8_t sector = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x948);
    const bool changed = sector != SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x949);
    const bool mirrored = (actor->moveFlags & 0x400U) != 0;
    const bool motionTwo = (actor->motionFlags & 2U) != 0;
    std::uint16_t offset = 0;
    std::uint8_t direction = 0;
    bool flip = false;
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x883)) {
    case 0:
        offset = mirrored ? 306 : 246;
        direction = 8;
        flip = mirrored && changed;
        if (mirrored) {
            actor->moveFlags &= ~0x440U;
            actor->hitStateFlags &= ~0x200000U;
            mtwkSetRegionFlag(actor->motion, 69, 2, static_cast<MotionRegion>(0));
        }
        break;
    case 1:
        offset = mirrored || motionTwo ? 258 : 284;
        direction = 72;
        flip = (mirrored || motionTwo) && changed;
        break;
    case 2:
        offset = mirrored ? 250 : 252;
        direction = 64;
        flip = changed;
        break;
    case 3:
        offset = !mirrored || motionTwo ? 256 : 278;
        direction = 80;
        flip = (!mirrored || motionTwo) && changed;
        break;
    case 4:
        offset = mirrored ? 308 : 248;
        direction = 16;
        flip = !mirrored && changed;
        if (mirrored) {
            actor->moveFlags &= ~0x440U;
            actor->hitStateFlags &= ~0x200000U;
            mtwkSetRegionFlag(actor->motion, 69, 2, static_cast<MotionRegion>(0));
        }
        break;
    case 5:
        offset = mirrored || motionTwo ? 256 : 278;
        direction = 48;
        flip = !(mirrored || motionTwo) && changed;
        break;
    case 6:
        offset = mirrored ? 252 : 250;
        direction = 32;
        break;
    case 7:
        offset = !mirrored || motionTwo ? 258 : 284;
        direction = 40;
        flip = mirrored && !motionTwo && changed;
        break;
    default:
        break;
    }
    if (changed) {
        if (flip)
            actor->stateFlags |= 0x10000U;
        else
            actor->stateFlags &= ~0x10000U;
    }
    if ((actor->hitStateFlags & 0x400U) && !(actor->animationTime > static_cast<float>(actor->motionFrames))) {
        if (sector == SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x949) ||
            (SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x490) & 0xf0U) == (SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x94c) & 0xf0U))
            return;
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = SHENMUE_ACTION_ANIMATION(actor, offset);
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x949) = sector;
        actor->queuedAction = 0;
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94b) = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94a);
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94a) = direction | (oldDirection & 0x87U);
        vfStartFormationAnimation(actor);
    } else {
        SHENMUE_INSTALL_ACTION_ANIMATION(actor, SHENMUE_ACTION_ANIMATION(actor, offset));
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94b) = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94a);
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x94a) = direction | (oldDirection & 0x87U);
        actor->queuedAction = 0;
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x949) = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x948);
    }
}

} // namespace

#else

namespace {

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
            mtwkSetRegionFlag(self->motion, 69, 2, MotionRegion::LowerBody);
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
            mtwkSetRegionFlag(self->motion, 69, 2, MotionRegion::LowerBody);
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
        if (sector == self->relativeInputSectors[1] || !((self->inputFlags ^ self->previousReactionInputBytes[0]) & 0xf0))
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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// The shared native nullsub at the state-3 clear and the unhandled update slots of g_vfPreUpdateCallbacks.
// @verified - p
void vfActionNullsub(VFActor *) {
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void vfRemoveQueuedActionAtEnd(VFActor *self) {
    if (self->animationTime >= float(self->currentAnimationEndFrame))
        self->queuedAction = 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint16_t ReactionAnimation(const VFActor *self, std::size_t offset) {
    const auto *data = self->motionParameters[4] + offset;
    return static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(data[0]) << 8 | std::to_integer<std::uint8_t>(data[1]));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void VFActor_ClearActionFields(VFActor *self) {
    self->queuedAction = self->action;
    self->hitPhase = 0;
    self->tickFrameTime = 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void vfQueueActionAnimation(VFActor *self) {
    self->pendingAnimation = static_cast<std::int16_t>(ReactionAnimation(self, 40));
    vfApplyHitReactionAndStartMotion(self);
    self->queuedAction = self->action;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void vfCopyActionToQueuedActionAndResetHitPhase(VFActor *self) {
    const bool reset = static_cast<std::uint8_t>(self->queuedAction >> 24) != 6;
    self->queuedAction = self->action;
    if (reset)
        self->hitPhase = 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void vfQueueStepAnimationIfDue(VFActor *self) {
    if ((self->animationTime > 9 || static_cast<std::uint8_t>(self->action >> 24) != static_cast<std::uint8_t>(self->queuedAction >> 24)) &&
        !(self->hitStateFlags & VFHSF_ForcedReaction)) {
        self->pendingAnimation = static_cast<std::int16_t>(ReactionAnimation(self, 286));
        vfApplyHitReactionAndStartMotion(self);
        self->queuedAction = self->action;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void VF_SetKnockbackAnimationVariablesAndTriggerReaction(VFActor *self) {
    if ((self->animationTime > 9 || static_cast<std::uint8_t>(self->action >> 24) != static_cast<std::uint8_t>(self->queuedAction >> 24)) &&
        !(self->hitStateFlags & VFHSF_ForcedReaction)) {
        self->pendingAnimation = static_cast<std::int16_t>(ReactionAnimation(self, 290));
        vfApplyHitReactionAndStartMotion(self);
        self->queuedAction = self->action;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void vfQueueReactionEventFromEntryTable(VFActor *self) {
    const auto event = static_cast<std::uint8_t>(self->action);
    if (vfFindQueuedReactionEventEntry(self, event))
        self->queuedReactionEvent = event;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void VFActor_UpdateQueuedActionIdx(VFActor *self) {
    if (static_cast<std::uint8_t>(self->action >> 24) == static_cast<std::uint8_t>(self->queuedAction >> 24))
        return;
    self->queuedAction = self->action;
    self->hitPhase = 0;
    self->tickFrameTime = 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void VFM_ResetHitPhase(VFActor *self) {
    self->queuedAction = self->action;
    self->hitPhase = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

extern const float landingHoldContactScale, landingHoldKnockbackScale, landingHoldSlideScale;

// @verified - d
void UpdateLandingHold(FightActorRuntimePrefix *actor) {
    SHENMUE_ACTION_VALUE(float, actor, 0x424) += SHENMUE_ACTION_VALUE(float, actor, 0x26c) * landingHoldContactScale;
    if (--SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x3a0) < 0)
        SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x3a0) = 0;
    const std::int16_t timer = SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x3a0);
    float middle = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x292);
    middle -= SHENMUE_ACTION_VALUE(float, actor, 0x26c);
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    case 0: {
        actor->moveFlags |= 0x10U;
        if (SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x32a) != 1) {
            SHENMUE_ACTION_VALUE(float, actor, 0x454) = SHENMUE_ACTION_VALUE(float, actor, 0x46c) * landingHoldKnockbackScale;
            SHENMUE_ACTION_VALUE(float, actor, 0x45c) = SHENMUE_ACTION_VALUE(float, actor, 0x474) * landingHoldKnockbackScale;
            SHENMUE_ACTION_VALUE(float, actor, 0x460) = SHENMUE_ACTION_VALUE(float, actor, 0x46c) * landingHoldSlideScale;
            SHENMUE_ACTION_VALUE(float, actor, 0x468) = SHENMUE_ACTION_VALUE(float, actor, 0x474) * landingHoldSlideScale;
        }
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x328);
        if (SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434)) {
            SHENMUE_ACTION_VALUE(float, actor, 0x278) = actor->game->knockbackAmount;
            vfStartQueuedReactionAnimation(actor);
            mtwkSetExtraFlag(actor->motion, 0, 2);
        }
        if (actor->motionFlags & 0x10U) {
            vfSnapActorToContact(actor);
            SHENMUE_ACTION_VALUE(float, actor, 0x454) += SHENMUE_ACTION_VALUE(float, actor, 0x46c);
            SHENMUE_ACTION_VALUE(float, actor, 0x458) += SHENMUE_ACTION_VALUE(float, actor, 0x470);
            SHENMUE_ACTION_VALUE(float, actor, 0x45c) += SHENMUE_ACTION_VALUE(float, actor, 0x474);
            vfPredictLandingGround(actor);
        }
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    }
    case 1:
        if (SHENMUE_ACTION_VALUE(float, actor, 0x438) < middle)
            return;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    case 2:
        if (!(actor->moveFlags & 4U) && timer > static_cast<std::int32_t>(actor->motionFrames)) {
            SHENMUE_ACTION_VALUE(float, actor, 0x438) = middle;
            SHENMUE_ACTION_VALUE(float, actor, 0x450) = middle;
            return;
        }
        actor->moveFlags &= ~0x10U;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    case 3:
        if (timer <= 0) {
            actor->stateFlags &= ~0x200U;
            actor->queuedAction = 0;
            if (!(actor->hitStateFlags & 0x1000000U))
                actor->hitStateFlags &= ~1U;
            mtwkSetExtraFlag(actor->motion, 0, 0);
        }
        return;
    default:
        return;
    }
}

const float landingHoldContactScale = 0.4f;
const float landingHoldKnockbackScale = 1.5f;
const float landingHoldSlideScale = 4.0f;

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateInstallMotion(FightActorRuntimePrefix *actor) {
    MotionWork *const motion = actor->motion;
    if (!SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
        SHENMUE_INSTALL_ACTION_ANIMATION(actor, SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x328));
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    }
    if (!mtwkGetAnimationId(motion, static_cast<MotionRegion>(0)))
        actor->queuedAction = 0;
}

} // namespace

#else

namespace {


// @verified - p
void UpdateInstallMotion(VFActor *self) {
    if (!self->hitPhase) {
        Install(self, static_cast<std::int16_t>(self->queuedAction));
        ++self->hitPhase;
    }
    if (!mtwkGetAnimationId(self->motion, MotionRegion::LowerBody))
        self->queuedAction = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateDefaultReaction(FightActorRuntimePrefix *actor) {
    const std::uint32_t hitState = actor->hitStateFlags;
    const std::uint32_t moveFlags = actor->moveFlags;
    if ((hitState & 0x200U) && (SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x42c) & 0xfffU) == SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x9b6)) {
        vfResetScriptedFormationMotion(actor);
        if ((actor->savedSecondaryMotionFlags & 0x100U) && SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x66)) {
            std::uint32_t bone;
            if (actor->stateFlags & 0x10000U)
                bone = 25;
            else
                bone = 30;
            mtwkGetParentBoneWrapper(actor->motion, bone | 0x40000000U);
        }
    } else {
        if (SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x28e) || (moveFlags & 1U) == 0) {
            if ((actor->secondaryMotionFlags & 0x8000000U) && (actor->motionFlags & 4U) && actor->animationTime < static_cast<float>(actor->motionFrames))
                return;
            if (!(actor->motionFlags & 0x2000U) && !(actor->secondaryMotionFlags & 0x48000100U) && !(hitState & 4U))
                return;
            if (actor->secondaryMotionFlags & 0x200000U)
                return;
        }
        vfResetScriptedFormationMotion(actor);
        if (actor->secondaryMotionFlags & 0x100U) {
            actor->stateFlags ^= 0x10000U;
            std::uint32_t bone;
            if (actor->stateFlags & 0x10000U)
                bone = 25;
            else
                bone = 30;
            mtwkGetParentBoneWrapper(actor->motion, bone | 0x40000000U);
        }
        vfSelectDefaultMotion(actor, 0);
    }
}

} // namespace

#else

namespace {

// @verified - p
void UpdateDefaultReaction(VFActor *self) {
    if ((self->hitStateFlags & VFHSF_AirborneOrDowned) && self->secondActor && (self->secondActor->moveFlags & VFMF_AutoAdvanceMotion)) {
        vfSelectDefaultMotion(self, 0);
        return;
    }
    if ((self->hitStateFlags & VFHSF_AttackGuardWindow) && (self->currentAnimation & 0xfff) == self->defaultAnimations[0]) {
        vfResetScriptedFormationMotion(self);
        if ((self->savedSecondaryMotionFlags & VFSMF_AlternateFootTarget) && static_cast<std::uint16_t>(self->motionTimer))
            mtwkGetParentBoneWrapper(
                self->motion, ((self->stateFlags & VFSF_UseLeftFootTarget) ? MB_LeftFootTarget : MB_RightFootTarget) | motionTargetIgnoreY);
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
            mtwkGetParentBoneWrapper(
                self->motion, ((self->stateFlags & VFSF_UseLeftFootTarget) ? MB_LeftFootTarget : MB_RightFootTarget) | motionTargetIgnoreY);
        }
        vfSelectDefaultMotion(self, 0);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateApproachContact(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const other = actor->actor;
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    default:
        SHENMUE_INSTALL_ACTION_ANIMATION(actor, SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x328));
        SHENMUE_ACTION_VALUE(float, actor, 0x14c) = other->contactPosition.x;
        SHENMUE_ACTION_VALUE(float, actor, 0x150) = other->contactPosition.y;
        SHENMUE_ACTION_VALUE(float, actor, 0x154) = other->contactPosition.z;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    case 1: {
        if (actor->animationTime < static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2a2)))
            return;
        SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x17e) = actor->actorFacingAngles[SHENMUE_ACTION_VALUE(std::int8_t, actor, 1)];
        actor->moveFlags |= 0x200U;
        float x = SHENMUE_ACTION_VALUE(float, actor, 0x14c) - actor->contactPosition.x;
        const float z = SHENMUE_ACTION_VALUE(float, actor, 0x154) - actor->contactPosition.z;
        const float distance = std::sqrt(x * x + z * z);
        if (distance > SHENMUE_ACTION_VALUE(float, actor, 0x2ac))
            x *= SHENMUE_ACTION_VALUE(float, actor, 0x2ac) / distance;
        Vector3 impulse, rotated;
        impulse.x = SHENMUE_ACTION_VALUE(float, actor, 0x2b4);
        impulse.z = SHENMUE_ACTION_VALUE(float, actor, 0x2b8);
        vfRotateFightVector(actor->actorFacingAngles[SHENMUE_ACTION_VALUE(std::int8_t, actor, 1)], &impulse, &rotated);
        x -= rotated.x;
        const float frames =
            static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x290)) - actor->animationTime + SHENMUE_ACTION_VALUE(float, actor, 0x26c);
        SHENMUE_ACTION_VALUE(float, actor, 0x484) = x / frames;
        SHENMUE_ACTION_VALUE(float, actor, 0x488) = SHENMUE_ACTION_VALUE(float, actor, 0x484);
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    }
    case 2:
        if (actor->animationTime <= static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x290))) {
            actor->position.x += SHENMUE_ACTION_VALUE(float, actor, 0x484);
            actor->position.z += SHENMUE_ACTION_VALUE(float, actor, 0x488);
        } else
            actor->queuedAction = 0;
    }
}

} // namespace

#else

namespace {

// Preserve the native equal X/Z step stored at +4fc/+500.
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
        if (distance > self->attackReach)
            x *= self->attackReach / distance;
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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateLinkedAttackWait(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const linked = SHENMUE_ACTION_VALUE(FightActorRuntimePrefix *, actor, 0x9c4);
    if (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) != 1) {
        SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3c8) = SHENMUE_ACTION_VALUE(std::uint16_t, linked, 0x42c);
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 1;
    } else if ((linked->hitStateFlags & 0x100U) && SHENMUE_ACTION_VALUE(std::uint16_t, linked, 0x42c)) {
        actor->moveFlags |= 0x1000U;
        UpdateDefaultReaction(actor);
        return;
    }
    actor->hitStateFlags &= ~1U;
    actor->queuedAction = 0;
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateContactAttackAdmission(FightActorRuntimePrefix *actor) {
    actor->hitStateFlags &= ~0x100U;
    if (actor->animationTime < static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x290)))
        return;
    if (SHENMUE_ACTION_VALUE(float, actor, 0x2ac) >= actor->contactDistances[SHENMUE_ACTION_VALUE(std::int8_t, actor, 1)]) {
        actor->queuedAction = (SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x328)) + 0x9000000U;
        vfStartFormationAnimation(actor);
        SHENMUE_ACTION_VALUE(float, actor, 0x454) = SHENMUE_ACTION_VALUE(float, actor, 0x458) = SHENMUE_ACTION_VALUE(float, actor, 0x45c) = 0;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    } else
        SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x328) = 0;
}

} // namespace

#else

namespace {

// @verified - p
void UpdateContactAttackAdmission(VFActor *self) {
    self->hitStateFlags &= ~VFHSF_AttackActive;
    if (float(static_cast<std::uint16_t>(self->parameter2d0)) > self->animationTime)
        return;
    if (self->contactDistances[self->actionIndex] <= self->attackReach) {
        const auto animation = static_cast<std::int16_t>(self->queuedAction);
        self->pendingAnimation = animation;
        self->queuedAction = static_cast<std::uint32_t>(animation + 0x09000000);
        vfApplyHitReactionAndStartMotion(self);
        ++self->hitPhase;
        self->knockbackVelocity = {};
    } else
        self->queuedAction = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void UpdatePairedTiming(FightActorRuntimePrefix *actor) {
    if (SHENMUE_ACTION_VALUE(std::int8_t, SHENMUE_ACTION_VALUE(FightActorRuntimePrefix *, actor, 0x9c4), 0x32b) != 9) {
        float denominator = SHENMUE_ACTION_VALUE(float, actor, 0x440);
        denominator = SHENMUE_ACTION_VALUE(float, actor, 0x444) - denominator;
        float ticks = static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3c6));
        ticks += 1;
        denominator += ticks;
        if (denominator < 0)
            denominator = 0;
        const float frame = static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2d0));
        actor->animationTime = ((frame - 1) * ticks) / denominator + 1;
        SHENMUE_ACTION_VALUE(float, actor, 0x450) = actor->animationTime;
        if (!vfDidCrossFightFrame(actor, frame))
            return;
    } else {
        actor->animationTime = static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2d0));
        SHENMUE_ACTION_VALUE(float, actor, 0x450) = actor->animationTime;
    }
    actor->hitStateFlags |= 0x40000U;
    actor->queuedAction = 0x8000000U;
    SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c) = 0;
    SHENMUE_ACTION_VALUE(float, actor, 0x454) = SHENMUE_ACTION_VALUE(float, actor, 0x458) = SHENMUE_ACTION_VALUE(float, actor, 0x45c) = 0;
    ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
}

} // namespace

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateLateralContactMotion(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const other = actor->actor;
    const std::uint16_t directions = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x490);
    const std::uint32_t moveFlags = actor->moveFlags;
    if (!(directions & 0xc0U))
        return;
    int offset;
    if (directions & 0x80U)
        offset = moveFlags & 0x40U ? 250 : 252;
    else
        offset = moveFlags & 0x40U ? 252 : 250;
    const int scaleOffset = SHENMUE_ACTION_VALUE(std::int8_t, other, 0) * 4;
    const std::byte *scales = &SHENMUE_ACTION_VALUE(std::byte, actor, 0x370);
    scales += scaleOffset;
    const float multiplier = 0.005f / *reinterpret_cast<const float *>(scales);
    float x = other->contactPosition.x - actor->contactPosition.x;
    x *= multiplier;
    float z = other->contactPosition.z - actor->contactPosition.z;
    z *= multiplier;
    if (directions & 0x20U) {
        actor->position.x += x;
        actor->position.z += z;
    } else if (directions & 0x10U) {
        actor->position.x -= x;
        actor->position.z -= z;
    }
    if (!(actor->hitStateFlags & 0x400U) || mtwkSetRegionFlag(actor->motion, 65, 1, static_cast<MotionRegion>(0))) {
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = static_cast<std::uint8_t>((SHENMUE_ACTION_VALUE(const std::byte *, actor, 0x38) + offset)[1]) |
            (static_cast<std::int8_t>(SHENMUE_ACTION_VALUE(const std::byte *, actor, 0x38)[offset]) << 8);
        vfStartFormationAnimation(actor);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateRecoveryDelay(FightActorRuntimePrefix *actor) {
    switch (SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c)) {
    default:
        SHENMUE_ACTION_VALUE(std::int32_t, actor, 0x434) = SHENMUE_ACTION_VALUE(std::int16_t, actor, 0x328);
        vfStartQueuedReactionAnimation(actor);
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
        return;
    case 1:
        if (SHENMUE_ACTION_VALUE(float, actor, 0x438) < static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28c)))
            return;
        ++SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x27c);
    case 2:
        break;
    }
    SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x3a0) = static_cast<std::uint16_t>(10 * SHENMUE_ACTION_VALUE(float, actor, 0x270));
    SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x318) = 0x6000000U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void RemoveQueuedActionAtEnd(FightActorRuntimePrefix *actor) {
    if (actor->animationTime >= static_cast<float>(SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x28c)))
        actor->queuedAction = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateReactionEventIdle(FightActorRuntimePrefix *) {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateContactAdmissionIdle(FightActorRuntimePrefix *) {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateActionEighteenIdle(FightActorRuntimePrefix *) {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateActionNineteenIdle(FightActorRuntimePrefix *) {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateActionTwentyIdle(FightActorRuntimePrefix *) {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateActionTwentyOneIdle(FightActorRuntimePrefix *) {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateActionTwentyTwoIdle(FightActorRuntimePrefix *) {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateActionTwentySixIdle(FightActorRuntimePrefix *) {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

struct ActionCallbacks {
    void (*clear)(FightActorRuntimePrefix *);
    void (*update)(FightActorRuntimePrefix *);
};

const ActionCallbacks actionCallbacks[37] = {{ClearActionFields, UpdateInstallMotion}, {ClearActionFields, UpdateDefaultReaction},
    {QueueActionAnimation, RemoveQueuedActionAtEnd}, {QueueBallisticAction, UpdateBallisticAction}, {ClearActionFields, UpdateGuardImpactHold},
    {ClearActionFields, UpdateAirborneReaction}, {CopyActionAndResetHitPhase, UpdateRecovery}, {ClearActionFields, UpdateLandingHold},
    {ClearActionFields, UpdatePairedMotion}, {ClearActionFields, UpdatePairedDamage}, {QueueStepAnimationIfDue, RemoveQueuedActionAtEnd},
    {QueueKnockbackAnimationIfDue, RemoveQueuedActionAtEnd}, {QueueReactionEvent, UpdateReactionEventIdle}, {ClearActionFields, UpdateApproachContact},
    {ClearActionFields, UpdateLinkedAttackWait}, {ClearActionFields, UpdateContactAttackAdmission}, {ClearActionFields, UpdateContactAdmissionIdle},
    {ClearActionFields, UpdatePairedTiming}, {UpdateQueuedActionIndex, UpdateActionEighteenIdle}, {ClearActionFields, UpdateActionNineteenIdle},
    {ClearActionFields, UpdateActionTwentyIdle}, {UpdateQueuedActionIndex, UpdateActionTwentyOneIdle}, {UpdateQueuedActionIndex, UpdateActionTwentyTwoIdle},
    {QueueCameraRelativeDodge, RemoveQueuedActionAtEnd}, {ClearActionFields, UpdateReverseRecovery}, {ClearActionFields, UpdateTerrainDrop},
    {ClearActionFields, UpdateActionTwentySixIdle}, {ClearActionFields, UpdateDirectionalMotion}, {ClearActionFields, UpdateLocomotion},
    {ClearActionFields, UpdateTargetLocomotion}, {ClearActionFields, UpdateLateralContactMotion}, {ResetActionHitPhase, UpdateTimedGuard},
    {ClearActionFields, UpdateRecoveryDelay}, {ClearActionFields, UpdateReactionHold}, {ClearActionFields, UpdatePairedMotion},
    {ClearActionFields, UpdateContactFacingMotion}, {ClearActionFields, UpdateAuthoredDefeat}};

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfDispatchCurrentActionState(FightActorRuntimePrefix *actor) {
    if ((SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x280) = SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x320))) {
        std::uint8_t state = SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x323);
        if (state > 36)
            state = 1;
        (*actionCallbacks[state].clear)(actor);
    }
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfUpdateDynamicActionState(FightActorRuntimePrefix *actor) {
    SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x284) = actor->queuedAction;
    if (SHENMUE_ACTION_VALUE(std::uint32_t, actor, 0x284))
        (*actionCallbacks[SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x32b)].update)(actor);
}

#else

// Native vfExecuteDynamicActionFuncs: the native action producer guarantees an in-table state.
// @verified - p
void vfUpdateDynamicActionState(VFActor *self) {
    self->previousQueuedAction = self->queuedAction;
    if (self->queuedAction)
        g_vfPreUpdateCallbacks[static_cast<std::int8_t>(self->queuedAction >> 24)].update(self);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline const std::uint8_t *ExtendedReactionPc(FightActorRuntimePrefix *actor) {
    return SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#define SHENMUE_REACTION_DWORD(pc)                                                                                                                             \
    (static_cast<std::uint16_t>((pc)[7] | (pc)[6] << 8) | static_cast<std::uint32_t>(static_cast<std::uint16_t>((pc)[5] | (pc)[4] << 8)) << 16)

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactSelectStageObject(FightActorRuntimePrefix *actor) {
    std::uint8_t facing = ExtendedReactionPc(actor)[3];
    VOBJ_t *entry;
    for (entry = g_fightStageObjectRoot; entry; entry = entry->next)
        if ((entry->object.reactionFlags & 1U) && entry->object.reactingCharacter == actor->character)
            return 3;
    for (entry = g_fightStageObjectRoot; entry; entry = entry->next) {
        if (entry->object.state || (entry->object.reactionFlags & 1U))
            continue;
        if (!(entry->object.motionFlags & 0x200000U))
            continue;
        if (!(entry->object.motionFlags & 0x800000U))
            continue;
        if (!(entry->object.motionFlags & 0x100000U))
            continue;
        if (!(entry->object.contactSlotMask & (1 << actor->groupIndex)))
            continue;
        if (entry->object.definition->index == 2) {
            Vector3 position;
            GetCharacterWorldPosition(entry->object.model, &position);
            const std::int32_t yaw = GetCharacterWorldYaw(entry->object.model);
            const std::int32_t angle =
                static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(actor->contactPosition.x - position.x, actor->contactPosition.z - position.z));
            const std::int16_t difference = static_cast<std::int16_t>(__builtin_fabsf(static_cast<float>(yaw - angle)));
            if (static_cast<std::uint16_t>(difference - 10241) <= 0x2ffeU) {
                if (SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x9a7) == static_cast<std::uint8_t>(entry->object.index))
                    SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x9a7) = -1;
                continue;
            }
        }
        const std::int32_t angle = static_cast<std::int32_t>(
            dc::sdk::ArcTan2Fixed(actor->contactPosition.x - entry->object.contactPosition.x, actor->contactPosition.z - entry->object.contactPosition.z));
        if (static_cast<std::int16_t>(angle - actor->temporaryFacingAngle + 0x4000) & 0x8000)
            facing ^= 1U;
        if (facing)
            continue;
        SHENMUE_ACTION_VALUE(std::int8_t, actor, 0x9a7) = entry->object.index;
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactHasActiveMotionFlags(FightActorRuntimePrefix *actor) {
    if (SHENMUE_REACTION_DWORD(ExtendedReactionPc(actor)) & actor->activeMotionFlags) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 8;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactLacksActiveMotionFlags(FightActorRuntimePrefix *actor) {
    if (!(SHENMUE_REACTION_DWORD(ExtendedReactionPc(actor)) & actor->activeMotionFlags)) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 8;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactHasMotionFlags(FightActorRuntimePrefix *actor) {
    if (SHENMUE_REACTION_DWORD(ExtendedReactionPc(actor)) & actor->motionFlags) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 8;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactLacksMotionFlags(FightActorRuntimePrefix *actor) {
    if (!(SHENMUE_REACTION_DWORD(ExtendedReactionPc(actor)) & actor->motionFlags)) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 8;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactTargetHealthAtMost(FightActorRuntimePrefix *actor) {
    if (!(actor->actor->hp > static_cast<float>(ExtendedReactionPc(actor)[3] | static_cast<std::int8_t>(ExtendedReactionPc(actor)[2]) << 8))) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactTargetStyleEquals(FightActorRuntimePrefix *actor) {
    if (SHENMUE_ACTION_VALUE(std::uint16_t, actor->actor, 2) == static_cast<std::int8_t>(ExtendedReactionPc(actor)[1])) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int8_t VmMovePhase(std::uint8_t move);

std::uint8_t ReactMoveProficiencyAtLeast(FightActorRuntimePrefix *actor) {
    const std::uint8_t *const program = ExtendedReactionPc(actor);
    const std::uint8_t move = program[1];
    const std::int8_t required = static_cast<std::int8_t>(program[2]);
    std::int32_t phase = VmMovePhase(move);
    if (phase == -1)
        return 3;
    if (!phase && actor->game->type == 4)
        phase = 1;
    if (phase >= required) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactSlotModeEquals(FightActorRuntimePrefix *actor) {
    if (*reinterpret_cast<const std::int8_t *>(static_cast<std::byte *>(actor->slot) + 0x74) == static_cast<std::int8_t>(ExtendedReactionPc(actor)[1])) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactTargetGuardStateEquals(FightActorRuntimePrefix *actor) {
    if (static_cast<std::uint8_t>(actor->actor->retained2a9[0]) == static_cast<std::int8_t>(ExtendedReactionPc(actor)[1])) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactBranchOnCondition(FightActorRuntimePrefix *actor) {
    const std::uint16_t condition = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x770);
    const std::uint8_t *const program = ExtendedReactionPc(actor);
    if ((program[3] | static_cast<std::int8_t>(program[2]) << 8) == (condition & 0xff00U) && (condition & 0x1fU))
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) =
            SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x28) + (program[5] | static_cast<std::int8_t>(program[4]) << 8);
    else
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 8;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::uint8_t ReactTargetHasEntryFlag(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const target = actor->actor;
    const std::int32_t bit = SHENMUE_REACTION_DWORD(ExtendedReactionPc(actor));
    if (SHENMUE_ACTION_VALUE(std::uint32_t, target, 0x2c8) & (1 << bit)) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 8;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactRememberTargetCharacter(FightActorRuntimePrefix *actor) {
    SHENMUE_ACTION_VALUE(Character *, actor, 0x9cc) = actor->actor->character;
    SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactMatchesRememberedTarget(FightActorRuntimePrefix *actor) {
    if (SHENMUE_ACTION_VALUE(Character *, actor, 0x9cc) == actor->actor->character) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactTargetGuardDifference(FightActorRuntimePrefix *actor) {
    const std::int8_t threshold = static_cast<std::int8_t>(ExtendedReactionPc(actor)[1]);
    if (static_cast<std::int8_t>(actor->actor->retainedMode - actor->retainedMode) > threshold) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactSetRank(FightActorRuntimePrefix *actor) {
    const std::uint8_t rank = ExtendedReactionPc(actor)[1];
    if (rank) {
        SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x780) = rank;
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactAliveTeamCountAtLeast(FightActorRuntimePrefix *actor) {
    const FightRuntimePrefix *const game = actor->game;
    std::uint8_t count = 0;
    const std::uint8_t ownSlot = static_cast<std::uint8_t>(actor->groupIndex);
    const std::uint8_t team = game->slotTeams[ownSlot];
    for (std::uint8_t slot = 0; slot < 6; ++slot)
        if (game->characters[slot] && game->slotTeams[slot] == team && slot != ownSlot && !(__builtin_fabsf(game->actors[slot]->hp) < 0.00000011920929f))
            ++count;
    if (count >= static_cast<std::int8_t>(ExtendedReactionPc(actor)[1])) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactTargetSurvivesDamage(FightActorRuntimePrefix *actor) {
    if (actor->actor->hp > static_cast<float>(SHENMUE_ACTION_VALUE(std::uint8_t, actor, 0x2aa))) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactTargetStyleDiffers(FightActorRuntimePrefix *actor) {
    if (SHENMUE_ACTION_VALUE(std::uint16_t, actor->actor, 2) != static_cast<std::int8_t>(ExtendedReactionPc(actor)[1])) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactTargetHasActiveMotionFlags(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const target = actor->actor;
    if (SHENMUE_REACTION_DWORD(ExtendedReactionPc(actor)) & target->activeMotionFlags) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 8;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactTargetHasActiveMotionFlagsAgain(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *const target = actor->actor;
    if (SHENMUE_REACTION_DWORD(ExtendedReactionPc(actor)) & target->activeMotionFlags) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 8;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactTargetHasPreviousMotionFlags(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *target = actor->actor;
    if (SHENMUE_REACTION_DWORD(ExtendedReactionPc(actor)) & target->retained350) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 8;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactTargetHasPreviousMotionFlagsAgain(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *target = actor->actor;
    if (SHENMUE_REACTION_DWORD(ExtendedReactionPc(actor)) & target->retained350) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 8;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint16_t vfGetReactionFrame(FightActorRuntimePrefix *actor, std::uint8_t selector);

// @verified - d
std::uint8_t ReactTargetBeforeAnimationFrame(FightActorRuntimePrefix *actor) {
    FightActorRuntimePrefix *target;
    std::int32_t frame;
    target = actor->actor;
    frame = vfGetReactionFrame(target, ExtendedReactionPc(actor)[3]);
    const std::int8_t offset = static_cast<std::int8_t>(ExtendedReactionPc(actor)[1]);
    if (!(target->animationTime > static_cast<float>(frame + offset) - SHENMUE_ACTION_VALUE(float, target, 0x26c))) {
        SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
        return 0;
    }
    return 3;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint8_t ReactSelectNearbyTarget(FightActorRuntimePrefix *actor) {
    FightRuntimePrefix *const game = actor->game;
    if (ExtendedReactionPc(actor)[1] == 1) {
        float distance = 2.5f;
        std::int32_t selected = -1;
        for (std::int32_t slot = 0; slot < 6; ++slot)
            if (game->characters[slot] && game->actors[slot] != actor &&
                (static_cast<std::int16_t>(actor->actorFacingAngles[slot] - actor->temporaryFacingAngle + 0x4000) & 0x8000) &&
                actor->contactDistances[slot] < distance) {
                distance = actor->contactDistances[slot];
                selected = slot;
            }
        if (selected > 0) {
            vfSetTargetActor(game, static_cast<std::uint8_t>(actor->groupIndex), static_cast<std::uint8_t>(selected));
            SHENMUE_ACTION_VALUE(const std::uint8_t *, actor, 0x5b4) += 4;
            return 0;
        }
    }
    return 3;
}

std::uint16_t vfGetReactionFrame(FightActorRuntimePrefix *actor, std::uint8_t selector) {
    std::uint16_t frame;
    switch (selector) {
    case 3:
        frame = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x290);
        break;
    case 0:
    default:
        frame = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x292);
        break;
    case 1:
        frame = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x294);
        break;
    case 2:
        frame = SHENMUE_ACTION_VALUE(std::uint16_t, actor, 0x2c2);
        break;
    }
    return frame;
}

#endif

} // namespace s2
} // namespace shenmue
