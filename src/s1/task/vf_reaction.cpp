#include "s1/task/scen.hpp"
#include "s1/task/vfma.hpp"
#include "s1/input.hpp"
#include "s1/motion.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_types.hpp"

#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/task/time.hpp"
#include "s1/collision.hpp"
#include "s1/player_control.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/lighting.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/btpk.hpp"
#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/char.hpp"
#include "s1/task/scnt.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/vfcl.hpp"
#include "s1/task/vf_core.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/ewzo.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/tmex.hpp"
#include "s1/subtitles.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <type_traits>

#include "s1/task/vf_internal.hpp"

namespace shenmue::s1 {
using namespace vf_detail;

namespace vf_detail {


void UpdateRepeatedReactionHistory(VFActor &actor) {
    VFMAData *const game = actor.game;
    VFSlotRuntimeState *const slot = actor.slot;
    if (!game || !slot || actor.targetSlotIndex < 0 || static_cast<std::size_t>(actor.targetSlotIndex) >= kBattleSlots) {
        return;
    }
    VFActor *const target = game->actors[static_cast<std::size_t>(actor.targetSlotIndex)];
    if (!target)
        return;

    if ((target->hitFlags & 0x100U) != 0 && (actor.hitStateFlags2 & 0x00024000U) != 0 && actor.currentReactionAnimation != actor.previousAnimationId &&
        (target->hitFlags & 0x2U) == 0 && (actor.hitStateFlags2 & 0x10000U) == 0) {
        slot->reactionAnimationHistory[slot->reactionAnimationHistoryIndex] = target->currentReactionAnimation & 0x0fffU;
        slot->reactionAnimationHistoryIndex = static_cast<std::uint8_t>((slot->reactionAnimationHistoryIndex + 1U) & 3U);
    }

    slot->updateCamera &= ~0x400U;
    for (std::size_t first = 0; first < 3; ++first) {
        const std::uint16_t animation = slot->reactionAnimationHistory[first];
        if (animation == 0)
            continue;
        for (std::size_t second = first + 1; second < 4; ++second) {
            if (slot->reactionAnimationHistory[second] != animation)
                continue;
            slot->updateCamera |= 0x400U;
            slot->repeatedReactionAnimation = animation;
            break;
        }
        if ((slot->updateCamera & 0x400U) != 0)
            break;
    }

    VFActor *const hitSource = actor.hitSourceActor;
    if ((actor.previousHitStateFlags2 & 0x80000U) == 0 && (actor.hitStateFlags2 & 0x80000U) != 0 && hitSource && hitSource->fightStyleIndex == 0 &&
        hitSource->pairContactPriority != 0) {
        slot->rankHistory[slot->rankHistoryIndex] = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(hitSource->fighterRankId)));
        slot->rankHistoryIndex = static_cast<std::uint8_t>((slot->rankHistoryIndex + 1U) % 3U);
    }

    slot->repeatedRankCount = 0;
    if (hitSource && hitSource->fightStyleIndex == 0 && slot->rankHistory[1] != 0) {
        for (const std::uint16_t rank : slot->rankHistory) {
            if (static_cast<std::int32_t>(rank) == static_cast<std::int8_t>(hitSource->fighterRankId))
                ++slot->repeatedRankCount;
        }
    }
}


void vfUpdateMoveVelocityWithKnockback(VFActor &actor, float strength) {
    if (auto *const motion = static_cast<MotionInformationWork *>(actor.motion))
        motion->playbackRate = strength;
    const float ratio = actor.knockbackAcceleration / strength;
    const float squaredRatio = ratio * ratio;
    actor.knockbackStrength = strength;
    actor.animationScale = 1.0f / strength;
    actor.moveVelocity.x *= ratio;
    actor.moveVelocity.y *= squaredRatio;
    actor.moveVelocity.z *= ratio;
    actor.knockbackAccelerationBias *= squaredRatio;
    actor.knockbackSlideX *= ratio;
    actor.knockbackSlideZ *= ratio;
    actor.pendingKnockback.x *= ratio;
    actor.pendingKnockback.y *= squaredRatio;
    actor.pendingKnockback.z *= ratio;
}


void vfRescaleReactionFrameWindows(VFActor &actor) {
    if (actor.animationScale == 1.0f)
        return;
    const auto scaleFrame = [&](auto &frame) {
        const auto value = static_cast<std::uint16_t>(frame);
        if (value != 1)
            frame = static_cast<std::remove_reference_t<decltype(frame)>>(static_cast<std::int32_t>(static_cast<float>(value) * actor.animationScale + 0.5f));
    };
    scaleFrame(actor.attackWindowEndFrame);
    scaleFrame(actor.reactionMidFrame);
    scaleFrame(actor.reactionEndFrame);
    scaleFrame(actor.reactionState02EndFrame);
    scaleFrame(actor.field314);
    scaleFrame(actor.midairRecoveryEndFrame);
    scaleFrame(actor.field330);
    scaleFrame(actor.fallLandingWindowStartFrame);
    scaleFrame(actor.knockbackAnimationStartParameter);
    scaleFrame(actor.knockbackAnimationEndFrame);
    scaleFrame(actor.fallLandingWindowEndFrame);
    scaleFrame(actor.reactionAnimationEndFrame);
    scaleFrame(actor.trackingSlideStartFrame);
    scaleFrame(actor.field324);
    scaleFrame(actor.pairActionTriggerFrame);
    scaleFrame(actor.pairPriorityAnimationFrame);
    if (actor.attackWindowEndFrame >= actor.reactionMidFrame) {
        const std::uint16_t width = static_cast<std::uint16_t>(actor.reactionEndFrame - actor.reactionMidFrame);
        const auto duration = static_cast<std::uint16_t>(actor.reactionAnimationEndFrame);
        actor.reactionMidFrame = std::min(static_cast<std::uint16_t>(actor.attackWindowEndFrame + 1U), duration);
        actor.reactionEndFrame = std::min(static_cast<std::uint16_t>(actor.reactionMidFrame + width), duration);
    }
}

bool vfclInitReactionProgramAndRunHandlers(VFActor &actor, bool mirrorReaction) {
    const auto animationId = static_cast<std::uint16_t>(actor.pendingAnimationId & 0x0fffU);
    const std::byte *const reactionData = MOTN_GetReactionDataForAnimId(animationId);
    if (animationId == 0 || !reactionData)
        return false;
    const std::int32_t frameCount = MOTN_GetFrameCountFromAnimId(animationId);
    if (frameCount <= 0)
        return false;
    const std::uint32_t previousHitState = actor.hitStateFlags2;
    const std::size_t targetIndex = static_cast<std::size_t>(actor.targetSlotIndex);
    VFCoreMotionState motionState{};
    // vfclInitReactionProgramAndRunHandlers resets only its named fields;
    // omitted records retain the actor's existing motion parameters.
    motionState.reactionEventTable = static_cast<const std::byte *>(actor.reactionEventTable);
    motionState.counterReactionAnimationId = actor.counterReactionAnimationId;
    motionState.reactionStateId = actor.reactionStateId;
    motionState.fallLandingWindowStartFrame = actor.fallLandingWindowStartFrame;
    motionState.trackingSlideStartFrame = actor.trackingSlideStartFrame;
    motionState.knockbackTotalFrames = actor.knockbackTotalFrames;
    motionState.activeContactCount = actor.activeContactCount;
    motionState.hitParameter = actor.hitParameter;
    motionState.hitDirectionAngle = actor.hitDirectionAngle;
    motionState.midairRecoveryEndFrame = actor.midairRecoveryEndFrame;
    motionState.field324 = actor.field324;
    motionState.field332 = actor.field332;
    motionState.pairPriorityAnimationFrame = actor.pairPriorityAnimationFrame;
    motionState.pairFollowupActionWord = actor.pairFollowupActionWord;
    motionState.pairActionTriggerFrame = actor.pairActionTriggerFrame;
    motionState.pairContactPriority = actor.pairContactPriority;
    motionState.hitDamageScale = actor.hitDamageScale;
    motionState.hitReactionScaleX = actor.hitReactionScaleX;
    motionState.hitReactionScaleZ = actor.hitReactionScaleZ;
    motionState.knockbackReactionTime = actor.knockbackReactionTime;
    motionState.knockbackAccelerationBias = actor.knockbackAccelerationBias;
    motionState.knockbackAcceleration = actor.knockbackAcceleration;
    motionState.moveId = actor.moveId;
    motionState.guardState = actor.guardState;
    motionState.guardStrength = actor.guardStrength;
    motionState.hitGuardType = actor.hitGuardType;
    motionState.hitStunOverride = actor.hitStunOverride;
    motionState.hitHeightClass = actor.hitHeightClass;
    motionState.hitHeightClass2 = actor.hitHeightClass2;
    motionState.skipAlignment = actor.skipAlignment;
    motionState.field314 = actor.field314;
    motionState.field330 = actor.field330;
    motionState.field322Low = static_cast<std::uint8_t>(actor.field322);
    if (FightPoseWork *const figp = FIGP_GetWork(actor.character))
        figp->active = 0;
    const VFCoreMotionGate motionGate = VFCore_DecodeNativeMotion(
        {
            reactionData,
            animationId,
            static_cast<std::uint16_t>(frameCount),
            static_cast<std::uint16_t>(actor.pendingAnimationId),
            actor.animationScale,
            actor.contactPoints[targetIndex],
            static_cast<std::uint8_t>(actor.guardStat),
            actor.pairedActor ? static_cast<std::int16_t>(actor.pairedActor->fightStyleIndex) : static_cast<std::int16_t>(-1),
            mirrorReaction,
            actor.knockbackStrength,
            actor.game ? actor.game->knockbackGravityScale : 0.0f,
        },
        motionState);
    const bool decoded = motionGate == VFCoreMotionGate::Ready;

    actor.temporaryHitFlags = actor.hitFlags;
    actor.temporaryHitStateFlags = actor.hitStateFlags;
    actor.knockbackAccelerationBias = motionState.knockbackAccelerationBias;
    actor.postReactionAnimationId = motionState.postReactionAnimationId;
    actor.reactionWindowPhase = static_cast<std::int8_t>(motionState.reactionWindowPhase);
    actor.field32c = 0;
    actor.savedReactionStateFlags = previousHitState;
    actor.hitFlags = motionState.hitFlags;
    actor.hitStateFlags = motionState.hitStateFlags;
    actor.hitStateFlags2 = motionState.hitStateFlags2;
    actor.motionFlags = motionState.motionFlags;
    actor.stateFlags2 = motionState.stateFlags2;
    actor.counterReactionAnimationId = motionState.counterReactionAnimationId;
    actor.reactionStateId = motionState.reactionStateId;
    actor.reactionAnimationEndFrame = static_cast<std::int16_t>(motionState.reactionAnimationEndFrame);
    actor.reactionMidFrame = motionState.reactionMidFrame;
    actor.reactionEndFrame = motionState.reactionEndFrame;
    actor.attackWindowEndFrame = motionState.attackWindowEndFrame;
    actor.fallLandingWindowStartFrame = motionState.fallLandingWindowStartFrame;
    actor.fallLandingWindowEndFrame = motionState.fallLandingWindowEndFrame;
    actor.knockbackAnimationStartParameter = motionState.knockbackAnimationStartParameter;
    actor.knockbackAnimationEndFrame = motionState.knockbackAnimationEndFrame;
    actor.trackingSlideStartFrame = motionState.trackingSlideStartFrame;
    actor.knockbackTotalFrames = motionState.knockbackTotalFrames;
    actor.field332 = motionState.field332;
    actor.facingAngleStep = motionState.facingAngleStep;
    actor.previousHitStateFlags = motionState.previousHitStateFlags;
    actor.hitParameter = motionState.hitParameter;
    actor.hitDirectionAngle = motionState.hitDirectionAngle;
    actor.reactionState02EndFrame = motionState.reactionState02EndFrame;
    actor.midairRecoveryEndFrame = motionState.midairRecoveryEndFrame;
    actor.field324 = motionState.field324;
    actor.pairPriorityAnimationFrame = motionState.pairPriorityAnimationFrame;
    actor.knockbackAnimationId = motionState.knockbackAnimationId;
    actor.pairFollowupActionWord = motionState.pairFollowupActionWord;
    actor.pairActionTriggerFrame = motionState.pairActionTriggerFrame;
    actor.pairContactPriority = motionState.pairContactPriority;
    actor.activeContactCount = motionState.activeContactCount;
    actor.hitDamageScale = motionState.hitDamageScale;
    actor.hitReactionScaleX = motionState.hitReactionScaleX;
    actor.hitReactionScaleZ = motionState.hitReactionScaleZ;
    actor.knockbackReactionTime = motionState.knockbackReactionTime;
    actor.knockbackAcceleration = motionState.knockbackAcceleration;
    actor.moveId = motionState.moveId;
    actor.guardState = motionState.guardState;
    actor.guardStrength = motionState.guardStrength;
    actor.hitGuardType = motionState.hitGuardType;
    actor.reactionEventCount = static_cast<std::int8_t>(motionState.reactionEventCount);
    actor.reactionRequestedMoveId = motionState.reactionRequestedMoveId;
    actor.hitAttributeFlags = motionState.hitAttributeFlags;
    actor.hitStunOverride = motionState.hitStunOverride;
    actor.hitHeightClass = motionState.hitHeightClass;
    actor.hitHeightClass2 = motionState.hitHeightClass2;
    actor.reactionMotionRestartMode = motionState.reactionMotionRestartMode;
    actor.field2e1 = motionState.field2e1;
    actor.skipAlignment = motionState.skipAlignment;
    actor.field314 = motionState.field314;
    actor.field330 = motionState.field330;
    actor.field322 = static_cast<std::int16_t>((static_cast<std::uint16_t>(actor.field322) & 0xff00U) | motionState.field322Low);
    if (motionState.frameEventProgram) {
        actor.reactionEventTable = const_cast<std::byte *>(motionState.reactionEventTable);
        actor.reactionProgramStart = const_cast<std::byte *>(motionState.frameEventProgram);
        actor.reactionProgram = actor.reactionProgramStart;
        actor.reactionProgramCounter = actor.reactionProgram;
    }
    if (motionState.mirrorReaction)
        actor.moveFlags |= 0x40U;
    else
        actor.moveFlags &= ~0x40U;
    return decoded;
}


bool ApplyPendingHitReactionMotion(VFActor &actor, bool guardRealignYaw) {
    constexpr std::uint32_t kTransitionFlags = 0x00000020U | 0x00000040U | 0x00200000U;
    constexpr std::uint32_t kMirrorReactionAngle = 0x00000040U;
    constexpr std::uint32_t kSnapToContact = 0x00000020U;
    constexpr std::uint32_t kSnapToContactApplied = 0x00000080U;
    constexpr std::uint32_t kAttackActive = 0x00000100U;
    constexpr std::uint32_t kAlignedToGround = 0x00800000U;
    constexpr std::uint32_t kMotionOverride = 0x00200000U;
    constexpr std::uint32_t kHitReactionLock = 0x00000010U;
    constexpr std::uint32_t kHasHitbox = 0x00000100U;

    actor.moveFlags &= ~0x00010200U;
    actor.knockbackAccelerationBias = 0.0f;
    actor.queuedReactionEvent = 0;
    const std::uint32_t pending = actor.pendingAnimationId;
    if (pending == 0) {
        actor.currentReactionAnimation = 0;
        actor.hitStateFlags2 &= ~(0x00000008U | kSnapToContact | kMirrorReactionAngle | 0x00004000U | 0x00010000U | kAlignedToGround);
        return false;
    }

    const std::uint16_t animationId = static_cast<std::uint16_t>(pending & 0x0fffU);

    const std::uint32_t previousHitState = actor.hitStateFlags2;
    const std::uint16_t previousFacingStep = actor.facingAngleStep;
    const std::uint32_t transitionState = (pending & 0x4000U) != 0 ? previousHitState & ~kTransitionFlags : previousHitState;
    const bool mirrorReaction = ((actor.moveFlags & kMirrorReactionAngle) != 0) ^ ((transitionState & kMotionOverride) != 0) ^ ((pending & 0x8000U) != 0);
    actor.temporaryAnimationId = actor.currentReactionAnimation;
    actor.moveFlags |= 0x00000800U;
    if (animationId == 2429U || animationId == 2430U)
        actor.moveFlags |= 0x20000000U;

    // The outgoing motion owns transition realignment. Decode of the
    // incoming motion must not apply its facing step on entry.
    if ((transitionState & kMirrorReactionAngle) != 0) {
        actor.facingAngle =
            guardRealignYaw && (transitionState & 0x4000U) != 0 ? actor.facingYaw : static_cast<std::uint16_t>(actor.facingAngle + previousFacingStep);
    }
    const bool snapToContact = (transitionState & kSnapToContact) != 0;
    if (snapToContact) {
        actor.position.x = actor.contactCurrentPosition.x;
        actor.position.z = actor.contactCurrentPosition.z;
    }
    if ((transitionState & kAlignedToGround) != 0)
        actor.position.y = actor.groundY;

    actor.currentReactionAnimation = static_cast<std::uint16_t>(pending);
    actor.currentAnimationEndFrame = 1;
    actor.previousAnimationEndFrame = 1;
    if (!vfclInitReactionProgramAndRunHandlers(actor, mirrorReaction))
        return false;
    if (actor.knockbackAcceleration != 0.0f) {
        vfUpdateMoveVelocityWithKnockback(actor, actor.knockbackAcceleration);
        actor.knockbackAcceleration = 0.0f;
    } else if (actor.knockbackStrength != actor.timeScale) {
        vfUpdateMoveVelocityWithKnockback(actor, actor.timeScale);
    }
    vfRescaleReactionFrameWindows(actor);

    std::uint32_t hitState = actor.hitStateFlags2;

    if (static_cast<std::int16_t>(pending) < 0 && (actor.hitFlags & kMotionOverride) != 0) {
        hitState |= kMotionOverride;
    }
    if ((hitState & kAttackActive) != 0)
        ++actor.lastHitMoveId;
    if ((hitState & kAlignedToGround) != 0)
        actor.facingAngle = actor.facingYaw;
    if (snapToContact)
        hitState |= kSnapToContactApplied;
    actor.previousReactionAnimationId = actor.currentReactionAnimation;
    actor.hitStateFlags2 = hitState;

    if ((actor.moveFlags & kHitReactionLock) != 0) {
        actor.rootReactionModeFlags = 0;
    } else if ((hitState & kAttackActive) != 0) {
        const std::int16_t scheduledYaw = actor.scheduledReactionYaw;
        actor.scheduledReactionYaw = 0;
        if (scheduledYaw != 0) {
            actor.rootReactionModeFlags = scheduledYaw;
            actor.alternateRootReactionProgram = actor.pendingAlternateRootReactionProgram;
        } else {
            actor.rootReactionModeFlags = 0;
        }
    }
    actor.copiedGroundDelta = (actor.hitFlags & kHasHitbox) != 0 ? actor.targetActor->headGroundDelta : 0.0f;

    return true;
}

std::uint32_t ReadVFEventU32(const std::byte *data) {
    std::uint32_t value{};
    std::memcpy(&value, data, sizeof(value));
    return value;
}

float VF_GetLocalActorAnimScale(const VFActor &actor);
void vfResetSfxTimers(VFMAData &game);

float ReadVFFrameEventFixed16(const std::byte *data) {
    const std::uint32_t value = (static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(data[0])) << 24U) |
        (static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(data[1])) << 16U) |
        (static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(data[2])) << 8U) | std::to_integer<std::uint8_t>(data[3]);
    return static_cast<float>(static_cast<std::int32_t>(value)) / 65536.0f;
}

std::uint16_t NextVFFrameEventRandomWord() {
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    return static_cast<std::uint16_t>((g_EnvPrngSeed >> 16U) & 0x7fffU);
}

bool vfAssignVoiceFor(VFMAData &game, std::uint8_t voiceCode);
bool vfmaAssignVoiceReactForFrame(VFMAData &game, std::uint8_t voiceCode);

constexpr std::array<std::uint32_t, 6> kVFMoveSfxTypes{0x003376abU, 0x003676abU, 0x003776abU, 0x004276abU, 0x000b09a9U, 0x000809a9U};
constexpr std::array<std::uint32_t, 4> kVFAicaSoundCommands{0x004076abU, 0x004176abU, 0x004376abU, 0x004476abU};

void vfHandleImpactSfxAndDefeat(VFActor &self, std::int16_t damage, std::uint8_t attackerSelectMode) {
    VFMAData *const game = self.game;
    const std::int8_t playerSlot = game->playerSlot;
    if (playerSlot == -1 || self.slotIndex != static_cast<std::uint8_t>(playerSlot)) {
        if ((game->flags & (PlayerAggro | EnemyAggro)) == (PlayerAggro | EnemyAggro))
            return;
    } else if ((game->flags & PlayerAggro) != 0) {
        return;
    }
    if (g_VFForcePlayerGodmode != 0) {
        if (self.slotIndex == static_cast<std::uint8_t>(playerSlot))
            return;
        damage = 10000;
    } else if (damage == 0) {
        return;
    }

    VFActor *attacker = nullptr;
    if (attackerSelectMode == 0)
        attacker = self.hitSourceActor ? self.hitSourceActor : self.pairedActor;
    else if (attackerSelectMode == 1)
        attacker = self.pairedFighter ? self.pairedFighter : self.targetActor;
    if (game->playerCharacter && self.character == game->playerCharacter) {
        damage = static_cast<std::int16_t>(static_cast<std::int32_t>((1.0 - static_cast<double>(game->playerAssistScale)) * damage));
        if (damage < 1)
            damage = 1;
    }
    if (self.fightStyleIndex == 0x1bU && self.slot && self.slot->currentHpRegenerationPhase == 3U && attacker &&
        static_cast<std::uint8_t>(attacker->fighterRankId - 88U) <= 1U) {
        damage = 10000;
    }

    constexpr std::uint32_t kSfxEventPending = 0x00000001U;
    constexpr std::uint32_t kHpTeamBased = 0x00020000U;
    constexpr std::uint32_t kWaveBased = 0x00080000U;
    const std::uint32_t triggerMask = self.triggerMask;
    if ((triggerMask & 0x04010000U) != 0 ||
        ((triggerMask & kWaveBased) != 0 && self.currentHp <= damage &&
            static_cast<std::int16_t>(static_cast<std::int8_t>(self.fighterIdAlt3)) != static_cast<std::int16_t>(self.requiredFighterRank))) {
        return;
    }
    const std::int16_t groupHpFloor = (triggerMask & kHpTeamBased) != 0 ? static_cast<std::int16_t>(self.groupHpFloor) : 0;
    if (self.currentHp > groupHpFloor) {
        self.currentHp = std::max(groupHpFloor, static_cast<std::int16_t>(self.currentHp - damage));
        if (self.currentHp == 0) {
            if (game->soundFlag != 0 && (game->soundSlots[self.slotIndex] & 1) != 0)
                vfResetSfxTimers(*game);
            game->soundSlots[self.slotIndex] = 0;
            if (self.fightStyleIndex != 0 && (self.moveFlags & 0x4000U) == 0) {
                std::uint32_t command = 0;
                switch (self.fightStyleIndex) {
                case 0x05U:
                case 0x2eU:
                    command = kVFMoveSfxTypes[1];
                    break;
                case 0x06U:
                case 0x1bU:
                    command = kVFMoveSfxTypes[5];
                    break;
                case 0x0dU:
                    command = kVFMoveSfxTypes[2];
                    break;
                case 0x11U:
                    command = kVFMoveSfxTypes[3];
                    break;
                case 0x14U:
                case 0x1cU:
                case 0x1dU:
                    command = kVFMoveSfxTypes[4];
                    break;
                case 0x21U:
                    command = kVFMoveSfxTypes[0];
                    break;
                default: {
                    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
                    const float sample = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125);
                    command = kVFAicaSoundCommands[static_cast<std::uint8_t>(static_cast<std::int32_t>(sample * 4.0f))];
                    break;
                }
                }
                if ((self.game->flags & (PlayerAggro | FreeBattleIntroActive)) == 0 && self.character)
                    SNDWrapper(command, 5U, self.character);
                self.moveFlags |= 0x4000U;
            }

            Character *const character = self.character;
            if (character == game->playerCharacter || game->state != VFS_InBattle) {
                if ((self.game->flags & (PlayerAggro | FreeBattleIntroActive)) == 0 && character)
                    SNDWrapper(0x000c77abU, 5U, character);
                if ((self.hitStateFlags2 & 0x0001d010U) == 0 && self.motion) {
                    auto *const selfMotion = static_cast<MotionInformationWork *>(self.motion);
                    selfMotion->motionFlags &= ~8U;
                    selfMotion->mobjCallback = nullptr;
                    VFEnemySlot *const group = game->battleMeta.groups[self.slotIndex];
                    if (group && group->motion) {
                        VFActor *const fighter = group->fighter;
                        auto *const groupMotion = static_cast<MotionInformationWork *>(group->motion);
                        groupMotion->pendingAnimId = 0x04ecU;
                        if (fighter && group->state == SS_Active) {
                            if ((fighter->moveFlags & 0x40U) != 0)
                                groupMotion->motionFlags |= 0x80U;
                            else
                                groupMotion->motionFlags &= ~0x80U;
                        }
                        group->isRunning = static_cast<std::int8_t>(group->isRunning | 1);
                    }
                    if (self.character) {
                        if (FightPoseWork *const figp = FIGP_GetWork(self.character))
                            figp->active = 0;
                    }
                    game->flags |= BattleCamUiLock;
                }
                if (g_areaId != SHEN_NAME(MGCT))
                    g_playerStats.moveUsageCounts = {};
            } else {
                ++game->defeatedEnemies;
                if (!attacker)
                    return;
                if (attacker->slotIndex == static_cast<std::uint8_t>(game->playerSlot)) {
                    ++game->timer2;
                    game->comboTimer = 15;
                } else if (game->slotTeamId[attacker->slotIndex] == game->slotTeamId[static_cast<std::uint8_t>(game->playerSlot)]) {
                    ++game->previousDefeatedEnemies;
                }
            }

            if (attacker) {
                ++attacker->knockoutCount;
                if (game->reactionTypeIndex == 1U) {
                    if (attacker->fightStyleIndex == 0) {
                        if ((game->battleMeta.waves[self.slotIndex]->selectFlags & 0xa0U) == 0) {
                            vfAssignVoiceFor(*game, 11);
                            vfAssignVoiceFor(*game, 12);
                        }
                        if (attacker->knockoutCount % 15U == 0) {
                            vfmaAssignVoiceReactForFrame(*game, 10);
                            vfmaAssignVoiceReactForFrame(*game, 14);
                        }
                        VFActor *strongestEnemy = nullptr;
                        for (std::size_t slot = 0; slot < kBattleSlots; ++slot) {
                            if (game->characters[slot] && game->slotTeamId[slot] == 0 && game->actors[slot]->fightStyleIndex != 0)
                                strongestEnemy = game->actors[slot];
                        }
                        if (game->primaryTargetSlot == 0xffU && attacker->knockoutCount > 25U && strongestEnemy &&
                            attacker->knockoutCount > static_cast<std::uint8_t>(strongestEnemy->knockoutCount + 15U)) {
                            vfAssignVoiceFor(*game, 3);
                        }
                    } else if (attacker->fightStyleIndex == 0x1dU) {
                        if ((game->battleMeta.waves[self.slotIndex]->selectFlags & 0xa0U) == 0) {
                            vfmaAssignVoiceReactForFrame(*game, 1);
                            vfAssignVoiceFor(*game, 2);
                        }
                        if (attacker->knockoutCount % 5U == 0)
                            vfmaAssignVoiceReactForFrame(*game, 0);
                    }
                }
            }
        }
    }
    if (self.currentHp != 0 && (triggerMask & kSfxEventPending) != 0 && self.currentHp <= self.soundHpFloor)
        self.triggerState |= kSfxEventPending;
}

VFStageObject *VFFrameEventStageObject(const VFActor &actor) {
    if (actor.stageHitObjectIndex < 0 || static_cast<std::size_t>(actor.stageHitObjectIndex) >= g_vfStageObjs.size()) {
        return nullptr;
    }
    return g_vfStageObjs[static_cast<std::size_t>(actor.stageHitObjectIndex)];
}

VOBJ *vfAssignStageObjVelocityXZ(std::int32_t index, std::int16_t velocityXZ) {
    VFStageObject &object = *g_vfStageObjs[index];
    ResetStageObjectMotion(object);
    const std::int16_t folded = static_cast<std::uint16_t>(velocityXZ) <= 0x8000U ? velocityXZ : static_cast<std::int16_t>(-velocityXZ);
    const double speed = 0.5 - static_cast<double>(object.object->timeScale) * 0.0166;
    const float z = static_cast<float>(static_cast<double>(-sin_fixed16(static_cast<std::int16_t>(0x4000 - folded))) * speed);
    g_vfStageObjs[index]->localVelocity.x = static_cast<float>(static_cast<double>(-sin_fixed16(velocityXZ)) * speed);
    g_vfStageObjs[index]->localVelocity.y = 0.2f;
    g_vfStageObjs[index]->localVelocity.z = z;
    if ((object.object->entityFlags & 0x380U) != 0)
        object.stateFlags |= 2U;
    return object.object;
}


std::int32_t vfPredictKnockbackFramesToZ(const VFActor &actor, float targetY) {
    if ((actor.moveFlags & 4U) == 0)
        return 0;
    float frames = 0.0f;
    float height = actor.contactCurrentPosition.y;
    float velocity = actor.moveVelocity.y;
    for (;;) {
        velocity -= (actor.knockbackStrength * actor.knockbackStrength * actor.game->knockbackGravityScale) - actor.knockbackAccelerationBias;
        height += velocity;
        if (targetY > height && velocity < 0.0f)
            break;
        frames += 1.0f;
    }
    return static_cast<std::int32_t>(frames);
}

void UpdateAttackWindowForFrame(VFActor &actor) {
    constexpr std::uint32_t kHitTimeout = 0x00000002U;
    constexpr std::uint32_t kAttackActive = 0x00000100U;
    constexpr std::uint32_t kGuardState = 0x00010000U;
    constexpr std::uint32_t kFallLandingRequired = 0x00000010U;
    constexpr std::uint32_t kReactionLight = 0x00001000U;
    constexpr std::uint32_t kReactionActive = 0x00000001U;
    constexpr std::uint32_t kForcedReaction = 0x10000000U;

    std::uint32_t hitState = actor.hitStateFlags2;
    if (actor.reactionWindowPhase == 1) {
        if (actor.currentAnimationEndFrame < actor.attackWindowEndFrame) {
            return;
        }
        if ((actor.hitFlags & 0x8100U) == 0x8100U)
            hitState &= ~kGuardState;
        hitState &= ~kHitTimeout;
        ++actor.reactionWindowPhase;
    }
    if (actor.reactionWindowPhase == 2 && actor.currentAnimationEndFrame >= actor.reactionMidFrame) {
        actor.motionFlags &= ~0x80005200U;
        if ((hitState & kAttackActive) != 0)
            hitState &= ~0x00008000U;
        hitState = (hitState & ~(kAttackActive | kGuardState | kForcedReaction | 0x00000004U)) | 0x00000004U;
        ++actor.reactionWindowPhase;
    }
    if (actor.reactionWindowPhase == 3) {
        if ((hitState & (kReactionLight | kFallLandingRequired)) == (kReactionLight | kFallLandingRequired) && actor.fallLandingWindowEndFrame != 0 &&
            actor.currentAnimationEndFrame >= actor.fallLandingWindowEndFrame) {
            hitState = 0;
        }
        if (actor.currentAnimationEndFrame >= actor.reactionEndFrame) {
            hitState &= ~(kReactionLight | kReactionActive);
            ++actor.reactionWindowPhase;
        }
    }
    actor.hitStateFlags2 = hitState;
}



void vfProcessHitStateAndMovement(VFActor &actor) {
    actor.reactionProgram = actor.reactionProgramCounter;
    std::uint32_t hitState = actor.hitStateFlags2;
    if ((hitState & 0x00100000U) != 0) {
        hitState &= ~0x00100000U;
        actor.facingAngle ^= 0x8000U;
        actor.trackedContactAngle ^= 0x8000U;
    }
    if ((actor.hitFlags & 0x80U) != 0)
        hitState |= 0x20U;
    if (actor.currentAnimationEndFrame == 1U) {
        if (actor.moveDelta.x != 0.0f || actor.moveDelta.z != 0.0f) {
            const auto yaw = static_cast<std::uint16_t>(-actor.facingAngle);
            const float sine = sin_fixed16(yaw);
            const float cosine = sin_fixed16(fixed16_cosine_angle(yaw));
            actor.position.x = actor.contactCurrentPosition.x - (cosine * actor.moveDelta.x - sine * actor.moveDelta.z);
            actor.position.z = actor.contactCurrentPosition.z - (cosine * actor.moveDelta.z + sine * actor.moveDelta.x);
            if ((hitState & 0x80U) == 0) {
                actor.moveDelta.x = 0.0f;
                actor.moveDelta.z = 0.0f;
            }
        }
        hitState &= ~0x80U;
    }
    if ((actor.moveFlags & 0x1000U) != 0) {
        actor.moveFlags &= ~0x1000U;
        hitState |= 1U;
    }
    actor.hitStateFlags2 = hitState;
}

constexpr std::array<std::array<std::uint32_t, 8>, 8> kVFFrameEventSoundCommands{{
    {{0, 0, 0, 0, 0, 0, 0, 0}},
    {{0x000377abU, 0x000009a9U, 0x020009a9U, 0x003476abU, 0x003c76abU, 0, 0, 0}},
    {{0x000477abU, 0x000209a9U, 0x000309a9U, 0x003276abU, 0x003276abU, 0, 0, 0}},
    {{0x000077abU, 0x000709a9U, 0x000409a9U, 0x003976abU, 0x003b76abU, 0, 0, 0}},
    {{0x000177abU, 0x000409a9U, 0, 0x003a76abU, 0x003f76abU, 0, 0, 0}},
    {{0x000577abU, 0x000909a9U, 0x000109a9U, 0, 0x003e76abU, 0, 0, 0}},
    {{0x000277abU, 0x000509a9U, 0x000509a9U, 0, 0, 0, 0, 0}},
    {{0x000877abU, 0x000609a9U, 0, 0, 0, 0, 0, 0}},
}};

std::size_t VFFrameEventSoundStyleGroup(std::uint8_t fightStyleIndex) {
    switch (fightStyleIndex) {
    case 0:
        return 0;
    case 20:
    case 28:
    case 29:
        return 1;
    case 6:
    case 27:
        return 2;
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 14:
    case 15:
    case 30:
    case 31:
    case 34:
    case 35:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 45:
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
        return 3;
    case 5:
    case 13:
    case 17:
    case 18:
    case 19:
    case 33:
    case 46:
        return 4;
    default:
        return 7;
    }
}

std::int32_t vfScaleFrames(float scale, std::uint16_t frames) {
    return scale == 1.0f || frames == 1 ? frames : static_cast<std::int32_t>(static_cast<float>(frames) * scale + 0.5f);
}


bool VFActor_ToggleHitStateFlag2AndAdvanceReactPC(VFActor &actor, const std::byte *&cursor) {
    actor.hitStateFlags2 ^= 0x00200000U;
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_ResetState(VFActor &actor, const std::byte *&cursor) {
    static_cast<MotionInformationWork *>(actor.motion)->motionFlags &= ~8U;
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    actor.hitStunFrames = 0;
    actor.action = {};
    actor.action.state = 13;
    return true;
}


bool VFActor_UpdateKnockbackVelocityAndPosition(VFActor &actor, const std::byte *&cursor) {
    VFActor *const pair = actor.pairedActor;
    MotionInformationWork *const motion = static_cast<MotionInformationWork *>(actor.motion);
    std::int16_t angle =
        static_cast<std::int16_t>(std::to_integer<std::uint8_t>(cursor[4]) | (static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[5])) << 8U));
    if ((actor.moveFlags & 0x40U) != 0)
        angle = static_cast<std::int16_t>(-angle);
    const std::int16_t relative = static_cast<std::int16_t>(angle - actor.facingAngle);
    const float scale = pair->knockbackStrength;
    const float horizontal = ReadVFFrameEventFixed16(cursor + 8) * scale;
    actor.moveVelocity.x = horizontal * sin_fixed16(relative);
    actor.moveVelocity.z = -horizontal * sin_fixed16(fixed16_cosine_angle(static_cast<std::uint16_t>(relative)));
    actor.moveVelocity.y = (scale * scale * actor.game->knockbackGravityScale) * (ReadVFFrameEventFixed16(cursor + 12) * scale);
    if ((actor.moveFlags & 4U) == 0) {
        actor.position.y = actor.contactCurrentPosition.y;
        if ((actor.skipAlignment & 1) == 0) {
            actor.position.x = actor.contactCurrentPosition.x;
            actor.position.z = actor.contactCurrentPosition.z;
        }
    }
    actor.hitStateFlags2 |= 0x10U;
    actor.moveFlags |= 0x0cU;
    motion->motionFlags |= 0x400U;
    cursor += 16;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_IncrementReactProgPc(VFActor &actor, const std::byte *&cursor) {
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_SetReactProgStartFlag(VFActor &actor, const std::byte *&cursor) {
    actor.hitStateFlags2 |= ReadVFEventU32(cursor + 4);
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_AdvanceReactProgCounter(VFActor &actor, const std::byte *&cursor) {
    actor.hitStateFlags2 &= ReadVFEventU32(cursor + 4);
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_ToggleReactProgStartAndIncrementPC(VFActor &actor, const std::byte *&cursor) {
    actor.hitStateFlags2 ^= ReadVFEventU32(cursor + 4);
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_ProcessStepGuardImpact(VFActor &actor, const std::byte *&cursor) {
    if (actor.game->guardEffectTimer != 0) {
        VFActor &source = *actor.hitSourceActor;
        const std::int32_t guardScaled = static_cast<std::int32_t>((static_cast<double>(static_cast<std::uint8_t>(actor.guardStat)) * 0.05 + 1.0) *
            static_cast<double>(static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[3]))));
        const double amount = static_cast<float>(static_cast<std::uint8_t>(guardScaled)) * VF_GetLocalActorAnimScale(source);
        actor.guardImpactAccum = static_cast<std::uint16_t>(actor.guardImpactAccum + static_cast<std::uint8_t>(static_cast<std::int32_t>(amount * 0.5 + 0.5)));
        vfHandleImpactSfxAndDefeat(actor, static_cast<std::uint8_t>(static_cast<std::int32_t>(amount + 0.5)), 0);
    }
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_ProcessSoundCommand(VFActor &actor, const std::byte *&cursor) {
    bool dialogueActive = false;
    if (g_pCVWK) {
        dialogueActive = g_pCVWK->slots[0].callback || g_pCVWK->slots[1].callback;
    }
    VFMAData &game = *actor.game;
    if (!dialogueActive && game.voiceCodeIndex == -1 && (game.flags & PlayerAggro) == 0) {
        const std::size_t table = std::to_integer<std::uint8_t>(cursor[3]);
        const std::size_t styleGroup = VFFrameEventSoundStyleGroup(actor.fightStyleIndex);
        const std::uint32_t command = table < kVFFrameEventSoundCommands.size() ? kVFFrameEventSoundCommands[table][styleGroup] : 0;
        if (command && actor.character && (game.flags & (PlayerAggro | FreeBattleIntroActive)) == 0) {
            SNDWrapper(command, 5U, actor.character);
        }
    }
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_AdvanceReactProgPC(VFActor &actor, const std::byte *&cursor) {
    cursor += 16;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_ProcessReactionStep(VFActor &actor, const std::byte *&cursor) {
    const std::uint16_t threshold =
        static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[4]) | (static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[5])) << 8U));
    if (threshold != 0 && threshold >= NextVFFrameEventRandomWord()) {
        cursor += 8;
        actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
        return true;
    }
    actor.queuedReactionEvent = 31;
    actor.queuedReactionAnimationId =
        static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[6]) | (static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[7])) << 8U));
    return false;
}


bool VFActor_UpdateHitStateFlagsAndAdvanceReactPC(VFActor &actor, const std::byte *&cursor) {
    if (std::to_integer<std::uint8_t>(cursor[3]) != 0)
        actor.hitStateFlags2 &= ~0x00004000U;
    else
        actor.hitStateFlags2 |= 0x00004000U;
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFReaction_UpdateBattleShake(VFActor &actor, const std::byte *&cursor) {
    const std::int32_t eventFrame = vfScaleFrames(actor.animationScale, ReadPackedU16(reinterpret_cast<const std::uint8_t *>(cursor)));
    if (actor.currentAnimationEndFrame == eventFrame) {
        float shake = ReadVFFrameEventFixed16(cursor + 4);
        VFMAData &game = *actor.game;
        if (actor.slotIndex == static_cast<std::uint8_t>(game.playerSlot)) {
            shake *= game.playerReactionCameraScale + 1.5f;
        }
        if ((game.flags & FreeBattleIntroActive) == 0)
            CAME_SetShake(0.0f, shake, 0.5f);
    }
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_IncrementGuardStatAndAdvanceReactProgPC(VFActor &actor, const std::byte *&cursor) {
    if (static_cast<std::uint8_t>(actor.guardStat) < 8U)
        ++actor.guardStat;
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_AdvanceReactProgPC_0(VFActor &actor, const std::byte *&cursor) {
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_AdvanceReactProgAndSetHitStun(VFActor &actor, const std::byte *&cursor) {
    actor.hitStunFrames =
        static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[4]) | (static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[5])) << 8U));
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_AdvanceReactProgResetState(VFActor &actor, const std::byte *&cursor) {
    const std::int32_t eventFrame = vfScaleFrames(actor.animationScale, ReadPackedU16(reinterpret_cast<const std::uint8_t *>(cursor)));
    if (actor.currentAnimationEndFrame == eventFrame)
        actor.reactionResumeProgramCounter = nullptr;
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_SetReactProgStartState(VFActor &actor, const std::byte *&cursor) {
    actor.hitStateFlags2 |= 0x00000040U;
    actor.facingAngleStep =
        static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[4]) | (static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[5])) << 8U));
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_ProcessReactionState(VFActor &actor, const std::byte *&cursor) {
    const bool parameter = std::to_integer<std::uint8_t>(cursor[3]) != 0;
    const bool grounded = (actor.groundContactFlags & 1) != 0;
    if (grounded == parameter) {
        cursor += 8;
        actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
        return true;
    }
    actor.queuedReactionEvent = 31;
    actor.queuedReactionAnimationId =
        static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[4]) | (static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(cursor[5])) << 8U));
    return false;
}


bool VFActor_UpdateContactReaction(VFActor &actor, const std::byte *&cursor) {
    if ((ReadVFEventU32(cursor + 4) & actor.contactDecisionMask) != 0 &&
        ReadVFFrameEventFixed16(cursor + 12) >= actor.contactPoints[static_cast<std::size_t>(actor.targetSlotIndex)]) {
        actor.nextAction =
            std::bit_cast<VFActionDescriptor>(static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(ReadVFEventU32(cursor + 8)))));
    }
    cursor += 16;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_AdvanceReactProgPc(VFActor &actor, const std::byte *&cursor) {
    cursor += 12;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_AdvanceReactProgPc_0(VFActor &actor, const std::byte *&cursor) {
    cursor += 28;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_StartReactProgram(VFActor &actor, const std::byte *&cursor) {
    actor.motionFlags |= ReadVFEventU32(cursor + 4);
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_AdvanceReactProgPc_1(VFActor &actor, const std::byte *&cursor) {
    actor.motionFlags &= ReadVFEventU32(cursor + 4);
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFActor_UpdateReactProgPC(VFActor &actor, const std::byte *&cursor) {
    const std::uint32_t command = ReadVFEventU32(cursor + 4);
    if (actor.character && (actor.game->flags & (PlayerAggro | FreeBattleIntroActive)) == 0) {
        SNDWrapper(command, 5U, actor.character);
    }
    cursor += 8;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFM_vfUpdateWeatherEffects(VFActor &actor, const std::byte *&cursor) {
    EFFE_SpawnVFFightGroundEffect(std::to_integer<std::uint8_t>(cursor[3]), &actor);
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    return true;
}


bool VFM_vfStageObjectResetAndActivateForChara(VFActor &actor, const std::byte *&cursor) {
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    vfStageObjectResetAndActivateForChara(actor.stageHitObjectIndex, actor.character);
    return true;
}


bool VFM_vfResetStageObjectMotion(VFActor &actor, const std::byte *&cursor) {
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    ResetStageObjectMotion(*VFFrameEventStageObject(actor));
    return true;
}


bool VFM_vfSetFacingAng(VFActor &actor, const std::byte *&cursor) {
    cursor += 4;
    actor.reactionProgramCounter = const_cast<std::byte *>(cursor);
    (void)vfAssignStageObjVelocityXZ(actor.stageHitObjectIndex, static_cast<std::int16_t>(actor.temporaryFacingAngle));
    return true;
}

bool ConsumeVFFrameEvent(VFActor &actor, const std::byte *&cursor) {
    switch (std::to_integer<std::uint8_t>(cursor[2])) {
    case 0:
        vfProcessHitStateAndMovement(actor);
        return false;
    case 1:
        return VFActor_ToggleHitStateFlag2AndAdvanceReactPC(actor, cursor);
    case 2:
        return VFActor_ResetState(actor, cursor);
    case 3:
        return VFActor_UpdateKnockbackVelocityAndPosition(actor, cursor);
    case 4:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 5:
        return VFActor_SetReactProgStartFlag(actor, cursor);
    case 6:
        return VFActor_AdvanceReactProgCounter(actor, cursor);
    case 7:
        return VFActor_ToggleReactProgStartAndIncrementPC(actor, cursor);
    case 8:
        return VFActor_ProcessStepGuardImpact(actor, cursor);
    case 9:
        return VFActor_ProcessSoundCommand(actor, cursor);
    case 10:
        return VFActor_AdvanceReactProgPC(actor, cursor);
    case 11:
        return VFActor_ProcessReactionStep(actor, cursor);
    case 12:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 13:
        return VFActor_UpdateHitStateFlagsAndAdvanceReactPC(actor, cursor);
    case 14:
        return VFReaction_UpdateBattleShake(actor, cursor);
    case 15:
        return VFActor_IncrementGuardStatAndAdvanceReactProgPC(actor, cursor);
    case 16:
        return VFActor_AdvanceReactProgPC_0(actor, cursor);
    case 17:
        return VFActor_AdvanceReactProgPC_0(actor, cursor);
    case 18:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 19:
        return VFActor_AdvanceReactProgAndSetHitStun(actor, cursor);
    case 20:
        return VFActor_AdvanceReactProgResetState(actor, cursor);
    case 21:
        return VFActor_AdvanceReactProgPC_0(actor, cursor);
    case 22:
        return VFActor_SetReactProgStartState(actor, cursor);
    case 23:
        return VFActor_ProcessReactionState(actor, cursor);
    case 24:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 25:
        return VFActor_UpdateContactReaction(actor, cursor);
    case 26:
        return VFActor_AdvanceReactProgPc(actor, cursor);
    case 27:
        return VFActor_AdvanceReactProgPc_0(actor, cursor);
    case 28:
        return VFActor_AdvanceReactProgPC_0(actor, cursor);
    case 29:
        return VFActor_StartReactProgram(actor, cursor);
    case 30:
        return VFActor_AdvanceReactProgPc_1(actor, cursor);
    case 31:
        return VFActor_AdvanceReactProgPC_0(actor, cursor);
    case 32:
        return VFActor_UpdateReactProgPC(actor, cursor);
    case 33:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 34:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 35:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 36:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 37:
        return VFActor_AdvanceReactProgPc(actor, cursor);
    case 38:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 39:
        return VFActor_AdvanceReactProgPc(actor, cursor);
    case 40:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 41:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 42:
        return VFActor_AdvanceReactProgPC_0(actor, cursor);
    case 43:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 44:
        return VFActor_IncrementReactProgPc(actor, cursor);
    case 45:
        return VFM_vfUpdateWeatherEffects(actor, cursor);
    case 46:
        return VFM_vfStageObjectResetAndActivateForChara(actor, cursor);
    case 47:
        return VFM_vfResetStageObjectMotion(actor, cursor);
    case 48:
        return VFM_vfSetFacingAng(actor, cursor);
    default:
        return false;
    }
}


void ConsumeVFFrameEvents(VFActor &actor) {
    if (actor.reactionProgramCounter == nullptr)
        return;
    for (;;) {
        const std::byte *cursor = static_cast<const std::byte *>(actor.reactionProgramCounter);
        const std::int32_t eventFrame = vfScaleFrames(actor.animationScale, ReadPackedU16(reinterpret_cast<const std::uint8_t *>(cursor)));
        if (actor.currentAnimationEndFrame < static_cast<std::uint16_t>(eventFrame)) {
            vfProcessHitStateAndMovement(actor);
            return;
        }
        if (!ConsumeVFFrameEvent(actor, cursor))
            return;
    }
}

static_assert(offsetof(VFActor, hitSourceActor) == 0x40);
static_assert(offsetof(VFActor, reactionScriptBase) == 0x48);
static_assert(offsetof(VFActor, knockbackAcceleration) == 0x2b4);
static_assert(offsetof(VFActor, attackWindowEndFrame) == 0x2d8);
static_assert(offsetof(VFActor, guardStrength) == 0x2f2);
static_assert(offsetof(VFActor, hitParameter) == 0x2f8);
static_assert(offsetof(VFActor, field330) == 0x330);
static_assert(offsetof(VFActor, pairActionTriggerFrame) == 0x342);
static_assert(offsetof(VFActor, action) == 0x370);
static_assert(offsetof(VFActor, deferredPairAction) == 0x380);
static_assert(offsetof(VFActor, hitReactionAnimId) == 0x420);
static_assert(offsetof(VFActor, pendingAnimationId) == 0x484);
static_assert(offsetof(VFActor, pairReferenceFrame) == 0x48c);
static_assert(offsetof(VFActor, pairFrameOffset) == 0x48e);
static_assert(offsetof(VFActor, deferredPairActionTriggerFrame) == 0x490);
static_assert(offsetof(VFActor, pairProcessedMask) == 0x492);
static_assert(offsetof(VFActor, pairedCharacterCache) == 0xae8);


void vfSetPairedFighter(VFMAData &game, std::uint8_t selfId, std::uint8_t pairedFighterId) {
    VFActor &self = *game.actors[selfId];
    Character *const previous = self.pairedCharacterCache;
    self.pairedFighterId = pairedFighterId;
    self.pairedActor = game.actors[pairedFighterId];
    self.pairedCharacterCache = game.characters[pairedFighterId];
    if (previous != self.pairedCharacterCache)
        self.contactDecisionMask = 0;
}



void vfUpdatePairActionState(VFActor &self, const VFActor &other) {
    if (std::bit_cast<std::uint64_t>(self.action) != 0) {
        self.nextAction = self.action;
        self.deferredPairAction = {};
        return;
    }
    if (std::bit_cast<std::uint64_t>(self.deferredPairAction) != 0 &&
        self.currentAnimationEndFrame == static_cast<std::uint16_t>(self.deferredPairActionTriggerFrame)) {
        self.nextAction = self.deferredPairAction;
        self.deferredPairAction = {};
        return;
    }
    const std::uint8_t state = self.nextAction.state;
    const std::uint8_t otherState = other.nextAction.state;
    if (state == 27) {
        self.deferredPairActionTriggerFrame = other.pairActionTriggerFrame;
        self.deferredPairAction = self.nextAction;
        self.nextAction = {};
        return;
    }
    if (state == 28) {
        self.currentAnimationEndFrame = static_cast<std::uint16_t>(self.currentAnimationEndFrame + 3U);
        self.nextAction = {};
        return;
    }
    if ((state == 16 || state == 25) && otherState <= 27 && ((0x08c30000U >> otherState) & 1U) != 0 &&
        ((self.motionFlags & 0x400U) == 0 || (self.hitFlags & 0x100U) != 0 || other.hitSourceActor != &self)) {
        self.nextAction = {};
        return;
    }
    if ((self.moveFlags & 0x10U) != 0 && state != 13 && ((self.hitStateFlags & 0xc00U) == 0 || (self.hitFlags & 0x100U) != 0)) {
        self.nextAction = {};
        return;
    }
    if (std::bit_cast<std::uint64_t>(self.nextAction) != 0)
        self.deferredPairAction = {};
}


void vfUpdatePairedFighterState(VFActor &self, VFActor &pairedFighter) {
    VFMAData &game = *self.game;
    VFActor &caught = *game.actors[pairedFighter.slotIndex];
    caught.pairedFighterId = self.slotIndex;
    Character *const previous = caught.pairedCharacterCache;
    caught.pairedActor = game.actors[self.slotIndex];
    caught.pairedCharacterCache = game.characters[self.slotIndex];
    if (previous != caught.pairedCharacterCache)
        caught.contactDecisionMask = 0;
    pairedFighter.hitSourceActor = &self;
    const std::uint16_t animation = VFCore_ActionAnimation(self.nextAction.opcode, self.nextAction.index) & 0x0fffU;
    std::uint16_t response = 0;
    if (const std::uint8_t *const styleResponse = shenLookupAnimationIndex(animation, 26);
        styleResponse && pairedFighter.fightStyleIndex == static_cast<std::int8_t>(styleResponse[1]))
        std::memcpy(&response, styleResponse + 4, sizeof(response));
    else
        std::memcpy(&response, shenLookupAnimationIndex(animation, 5) + 2, sizeof(response));
    pairedFighter.hitPhase = 0;
    pairedFighter.nextAction =
        std::bit_cast<VFActionDescriptor>(static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int16_t>(response))) | 0x11000000U);
    pairedFighter.knockbackAcceleration = pairedFighter.game->knockbackAmount;
    VFActor &cleared = animation == 1512U ? pairedFighter : self;
    VFActor &source = animation == 1512U ? self : pairedFighter;
    cleared.moveFlags &= ~0x440U;
    cleared.hitStateFlags2 &= ~0x200000U;
    if (cleared.nextAction.resolve == 1) {
        source.moveFlags &= ~0x440U;
        source.hitStateFlags2 &= ~0x200000U;
    } else {
        cleared.moveFlags |= source.moveFlags & 0x440U;
        cleared.hitStateFlags2 |= source.hitStateFlags2 & 0x200000U;
    }
}

std::uint16_t VFAnimationTableValue(const VFActor &actor, std::size_t offset) {
    if (!actor.animationTable)
        return 0;
    return static_cast<std::uint16_t>((static_cast<std::uint16_t>(actor.animationTable[offset]) << 8U) | actor.animationTable[offset + 1U]);
}

bool StartVFQueuedAnimation(VFActor &actor, std::uint16_t animation, bool useKnockbackAcceleration = false) {
    actor.pendingAnimationId = animation;
    if (useKnockbackAcceleration)
        actor.knockbackAcceleration = actor.timeScale;
    return ApplyPendingHitReactionMotion(actor);
}
void vfRestartMotionPreservingProgress(VFActor &actor) {
    const std::uint32_t previousEnd = static_cast<std::uint16_t>(actor.reactionAnimationEndFrame);
    const std::int16_t previousFrame = static_cast<std::int16_t>(actor.currentAnimationEndFrame);
    actor.hitStateFlags2 |= 0x20U;
    (void)ApplyPendingHitReactionMotion(actor);
    const std::uint16_t frame = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(static_cast<std::uint32_t>(static_cast<std::uint16_t>(actor.reactionAnimationEndFrame)) * previousFrame) / previousEnd);
    actor.currentAnimationEndFrame = frame;
    actor.previousAnimationEndFrame = frame;
}

void vfRestartMotionReverseProgress(VFActor &actor) {
    const std::uint32_t previousEnd = static_cast<std::uint16_t>(actor.reactionAnimationEndFrame);
    const std::uint32_t reverseFrame = static_cast<std::uint16_t>(previousEnd - actor.currentAnimationEndFrame + 2U);
    actor.hitStateFlags2 |= 0x20U;
    (void)ApplyPendingHitReactionMotion(actor);
    const std::uint16_t frame = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(static_cast<std::uint32_t>(static_cast<std::uint16_t>(actor.reactionAnimationEndFrame)) * reverseFrame) / previousEnd);
    actor.currentAnimationEndFrame = frame;
    actor.previousAnimationEndFrame = frame;
}

void vfSyncPairedReactionFrameToAttackerWindow(VFActor &actor) {
    VFActor &target = *actor.targetActor;
    if ((target.hitStateFlags2 & 0x100U) == 0)
        return;
    const std::uint16_t reactionEnd = static_cast<std::uint16_t>(actor.reactionAnimationEndFrame);
    const std::int16_t current = static_cast<std::int16_t>(actor.currentAnimationEndFrame);
    const std::int16_t remaining = static_cast<std::int16_t>(target.attackWindowEndFrame - actor.pairReferenceFrame);
    const std::uint16_t frame = remaining > 1 ?
        static_cast<std::uint16_t>(current + static_cast<std::uint8_t>(remaining / static_cast<std::int16_t>(reactionEnd - current))) :
        reactionEnd;
    actor.currentAnimationEndFrame = frame;
    actor.previousAnimationEndFrame = frame;
}

void ClearVFQueuedAction(VFActor &actor) {
    actor.queuedAction = {};
}

bool vfQueueCounterReaction(VFActor &actor, VFActor &other);

void vfclSetupAirborneHit(VFActor &actor);
void vfRecalculateKnockbackFallFrames(VFActor &actor);

void vfclSetupAirborneHit(VFActor &actor) {
    if ((actor.moveFlags & 4U) == 0) {
        actor.position.y = actor.contactCurrentPosition.y;
        if ((actor.skipAlignment & 1) == 0) {
            actor.position.x = actor.contactCurrentPosition.x;
            actor.position.z = actor.contactCurrentPosition.z;
        }
    }
    actor.hitStateFlags2 |= 0x10U;
    actor.moveFlags |= 0x0cU;
    static_cast<MotionInformationWork *>(actor.motion)->motionFlags |= 0x400U;
}


void vfRecalculateKnockbackFallFrames(VFActor &actor) {
    actor.predictedGroundLanding = actor.groundY;
    if (actor.moveVelocity.x == 0.0f && actor.moveVelocity.z == 0.0f)
        return;
    float height = actor.contactCurrentPosition.y;
    if (std::fabs(height) < 0.00000011920929f)
        height = 0.0f;
    const float frames = static_cast<std::int16_t>(actor.knockbackFramesRemaining);
    float x = actor.contactCurrentPosition.x + frames * actor.moveVelocity.x;
    float z = actor.contactCurrentPosition.z + frames * actor.moveVelocity.z;
    Vector3 probe{x, static_cast<float>(static_cast<double>(height) + 1.0), z};
    float ground = mapUpdateIntersectionStateAtLocalPoint(probe);
    if (actor.character) {
        for (const CharacterNode *node = actor.character->entity.node.child; node; node = node->child)
            ground += node->position.y;
    }
    if (ground == actor.groundY)
        return;
    float velocity = actor.moveVelocity.y;
    std::uint8_t remaining;
    if (actor.groundY > ground) {
        remaining = static_cast<std::uint8_t>(actor.knockbackFramesRemaining);
        velocity = -velocity;
    } else {
        remaining = 0;
        x = actor.contactCurrentPosition.x;
        z = actor.contactCurrentPosition.z;
    }
    height = actor.contactCurrentPosition.y;
    for (;;) {
        do {
            x += actor.moveVelocity.x;
            z += actor.moveVelocity.z;
            velocity = (velocity - actor.knockbackStrength * actor.knockbackStrength * actor.game->knockbackGravityScale) + actor.knockbackAccelerationBias;
        } while (velocity >= 0.0f);
        if (std::fabs(height) < 0.00000011920929f)
            height = 0.0f;
        probe = {x, static_cast<float>(static_cast<double>(height) + 1.0), z};
        (void)mapUpdateIntersectionStateAtLocalPoint(probe);
        height += velocity;
        if (static_cast<double>(ground) + 0.02 > static_cast<double>(height - actor.knockbackReactionTime))
            break;
        ++remaining;
    }
    actor.knockbackFramesRemaining = remaining;
    actor.predictedGroundLanding = ground;
}


void UpdateVFStandingHitReaction(VFActor &actor) {
    const std::uint32_t entryFlags = actor.hitStateFlags2;
    const std::int16_t decremented = static_cast<std::int16_t>(actor.hitStunFrames - 1U);
    actor.hitStunFrames = decremented < 0 ? 0 : static_cast<std::uint16_t>(decremented);
    actor.reactionTimer += 1.0f;
    const std::int16_t stun = actor.hitStunFrames;
    if (actor.queuedAction.resolve == 1U) {
        actor.pendingAnimationId = actor.targetActor && vfQueueCounterReaction(actor, *actor.targetActor) ? 1U : 0U;
    }

    if (actor.hitPhase == 0 || actor.hitPhase > 3) {
        actor.moveFlags |= 0x10U;
        actor.moveVelocity.x = actor.pendingKnockback.x;
        actor.moveVelocity.z = actor.pendingKnockback.z;
        actor.knockbackSlideX = actor.pendingKnockback.x * 5.0f;
        actor.knockbackSlideZ = actor.pendingKnockback.z * 5.0f;
        actor.pendingAnimationId = VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index);
        if (actor.pendingAnimationId == 0) {
            actor.pendingAnimationId = VFAnimationTableValue(actor, (entryFlags & 8U) != 0 ? 20U : 16U);
        }
        (void)ApplyPendingHitReactionMotion(actor);
        actor.hitStateFlags2 |= 1U;
        if (actor.reactionWindowPhase != 0) {
            if (actor.reactionEndFrame > static_cast<std::uint16_t>(actor.reactionAnimationEndFrame)) {
                actor.reactionEndFrame = static_cast<std::uint16_t>(actor.reactionAnimationEndFrame);
            }
            actor.currentAnimationEndFrame = actor.attackWindowEndFrame;
            actor.previousAnimationEndFrame = actor.attackWindowEndFrame;
        } else {
            actor.attackWindowEndFrame = 1;
            actor.reactionMidFrame = 1;
            actor.reactionEndFrame = 1;
        }
        ++actor.hitPhase;
        goto CheckMiddle;
    }
    if (actor.hitPhase == 1)
        goto CheckMiddle;
    if (actor.hitPhase == 2)
        goto HoldMiddle;
    goto FinishStun;
CheckMiddle:
    if (actor.currentAnimationEndFrame < static_cast<std::uint16_t>(actor.reactionMidFrame - 1U))
        return;
    ++actor.hitPhase;
HoldMiddle:
    if (static_cast<std::int16_t>(stun + actor.reactionMidFrame - actor.reactionEndFrame) > 0) {
        actor.currentAnimationEndFrame = static_cast<std::uint16_t>(actor.reactionMidFrame - 1U);
        actor.previousAnimationEndFrame = actor.currentAnimationEndFrame;
    } else {
        ++actor.hitPhase;
    FinishStun:
        if (stun <= 0) {
            actor.hitStateFlags2 &= ~1U;
            actor.moveFlags &= ~0x10U;
            ClearVFQueuedAction(actor);
        }
    }
    VFActor &target = *actor.targetActor;
    if (static_cast<std::int16_t>(target.lastHitMoveId) != actor.lastProcessedHitMoveId && (target.counterReactionFlags & 3U) != 0) {
        const bool queued = vfQueueCounterReaction(actor, target);
        actor.pendingAnimationId = queued ? 1U : 0U;
        if (queued) {
            actor.queuedAction.resolve = 1;
            actor.hitPhase = 3;
        }
    }
}


void UpdateVFAlternateHitReaction(VFActor &actor) {
    const std::int16_t decremented = static_cast<std::int16_t>(actor.hitStunFrames - 1U);
    actor.hitStunFrames = decremented < 0 ? 0 : static_cast<std::uint16_t>(decremented);
    actor.reactionTimer += actor.knockbackStrength * 0.5f;
    const std::int16_t stun = actor.hitStunFrames;
    if (actor.hitPhase == 1 && actor.currentAnimationEndFrame >= static_cast<std::uint16_t>(actor.reactionMidFrame - 1U)) {
        actor.hitPhase = 2;
    }
    if (actor.hitPhase == 2) {
        const std::int32_t remaining = static_cast<std::int16_t>(stun + actor.reactionMidFrame - actor.reactionEndFrame);
        if (remaining > 0) {
            const std::uint16_t hold = static_cast<std::uint16_t>(actor.reactionMidFrame - 1U);
            actor.currentAnimationEndFrame = hold;
            actor.previousAnimationEndFrame = hold;
            return;
        }
        actor.moveFlags &= ~0x10U;
        actor.hitPhase = 3;
    }
    if (actor.hitPhase == 3) {
        if (stun <= 0) {
            actor.queuedAction = {};
            if ((actor.hitStateFlags2 & 0x01000000U) == 0)
                actor.hitStateFlags2 &= ~1U;
        }
        return;
    }
    if (actor.hitPhase != 0 && actor.hitPhase <= 3)
        return;

    if (actor.motion) {
        static_cast<MotionInformationWork *>(actor.motion)->motionFlags &= ~8U;
    }
    actor.moveFlags |= 0x10U;
    if (actor.queuedAction.resolve != 1U) {
        actor.moveVelocity.x = actor.pendingKnockback.x;
        actor.moveVelocity.z = actor.pendingKnockback.z;
        actor.knockbackSlideX = actor.pendingKnockback.x * 5.0f;
        actor.knockbackSlideZ = actor.pendingKnockback.z * 5.0f;
    }
    actor.pendingAnimationId =
        static_cast<std::uint32_t>(static_cast<std::int16_t>(VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index)));
    if (actor.pendingAnimationId != 0) {
        actor.knockbackAcceleration = actor.timeScale;
        if ((actor.hitFlags & 0x00100000U) != 0 && (actor.hitStateFlags & 0x00000004U) == 0 &&
            (!actor.pairedFighter || actor.pairedFighter->fighterIdAlt != 44U)) {
            actor.facingAngle ^= 0x8000U;
        }
        (void)ApplyPendingHitReactionMotion(actor, true);
    }
    if ((actor.hitFlags & 0x00000010U) != 0) {
        vfclSetupAirborneHit(actor);
        actor.moveVelocity.x += actor.pendingKnockback.x;
        actor.moveVelocity.y += actor.pendingKnockback.y;
        actor.moveVelocity.z += actor.pendingKnockback.z;
        vfRecalculateKnockbackFallFrames(actor);
    }
    ++actor.hitPhase;
}


void UpdateVFHitReactionPhase(VFActor &actor) {
    static constexpr std::array<std::uint16_t, 4> kReactionOffsets{70U, 74U, 72U, 76U};
    const std::uint8_t selectionFlags = actor.hitReactionAnimSelectFlags;
    std::uint64_t queued = std::bit_cast<std::uint64_t>(actor.queuedAction);
    if (actor.hitPhase == 0) {
        actor.hitStateFlags2 |= 0x10000U;
        actor.reactionResumeProgramCounter = nullptr;
        actor.hitPhase = 1;
    }
    if (actor.hitPhase == 1) {
        if ((actor.moveFlags & 1U) == 0)
            return;
        actor.pendingAnimationId = (actor.hitFlags & 0x04000000U) != 0 ?
            static_cast<std::uint16_t>((~static_cast<std::uint8_t>(actor.hitFlags >> 10U) & 0x02U) | 0x03bcU) :
            VFAnimationTableValue(actor, (selectionFlags & 1U) != 0 ? 66U : 68U);
        (void)ApplyPendingHitReactionMotion(actor);
        if ((actor.moveFlags & 0x20U) != 0 || actor.currentHp <= 0) {
            actor.hitPhase = 5;
            return;
        }
        ++actor.hitPhase;
        actor.hitPhaseWaitFrames = 0;
    }
    if (actor.hitPhase == 2) {
        const std::int16_t prior = actor.hitStunFrames;
        actor.hitStunFrames = static_cast<std::uint16_t>(prior - 1);
        if (prior > static_cast<std::int16_t>(4 * actor.hitPhaseWaitFrames))
            return;
        actor.moveFlags &= ~0x10U;
        ++actor.hitPhase;
        actor.hitStunFrames = 0;
        actor.actionPhaseFrameCounter = 0;
        actor.knockbackUnknown = 0;
        actor.knockbackPhase = 0;
    }
    if (actor.hitPhase == 3) {
        actor.moveFlags &= ~0x20000040U;
        queued &= 0xfffffffffffffe0fULL;
        std::size_t selection = 0;
        if ((selectionFlags & 1U) == 0) {
            queued |= 0x200U;
            ++selection;
        }
        if ((selectionFlags & 2U) == 0) {
            queued |= 0x400U;
            selection += 2;
        }
        actor.pendingAnimationId = VFAnimationTableValue(actor, kReactionOffsets[selection]);
        (void)ApplyPendingHitReactionMotion(actor);
        actor.queuedAction = std::bit_cast<VFActionDescriptor>(queued);
        if (actor.game->reactionTypeIndex == 1U && (actor.game->battleMeta.waves[actor.slotIndex]->selectFlags & 0xa0U) != 0 &&
            g_pCVWK->slots[0].callback == nullptr && g_pCVWK->slots[1].callback == nullptr) {
            static constexpr const char *voices[] = {"BVOICE018", "BVOICE024", "BVOICE025", "BVOICE031"};
            CVWKRuntimeSlot *const subtitle = ProcessSubtitleFile(voices[NextVFFrameEventRandomWord() & 3U], 0, 0, nullptr);
            ProcessAudioData(subtitle, 0);
        }
        ++actor.hitPhase;
    }
    if (actor.hitPhase == 4 && (actor.hitStateFlags2 & 4U) != 0)
        actor.queuedAction = {};
}


void UpdateVFFallbackGuardOrReaction(VFActor &actor) {
    const std::uint32_t state = actor.hitStateFlags2;
    const std::uint16_t fallback = VFAnimationTableValue(actor, 4U);
    if ((state & 8U) != 0) {
        actor.pendingAnimationId = fallback;
        if (actor.reactionMotionRestartMode >= 0)
            vfRestartMotionReverseProgress(actor);
        else
            (void)ApplyPendingHitReactionMotion(actor);
        return;
    }
    if ((state & 0x200U) != 0)
        return;
    if (actor.postReactionAnimationId == 0 && (actor.moveFlags & 1U) != 0) {
        actor.pendingAnimationId = static_cast<std::uint16_t>(actor.defaultReactionAnimationId);
        (void)ApplyPendingHitReactionMotion(actor);
        return;
    }
    const auto &motion = *static_cast<MotionInformationWork *>(actor.motion);
    if (motion.currentAnimId == fallback)
        return;
    if (actor.reactionMotionRestartMode < 0) {
        actor.pendingAnimationId = fallback;
        vfRestartMotionPreservingProgress(actor);
        return;
    }
    if ((actor.hitFlags & 0x2000U) != 0) {
        actor.pendingAnimationId = VFAnimationTableValue(actor, 18U);
        if ((state & 2U) != 0)
            vfRestartMotionReverseProgress(actor);
        else
            (void)ApplyPendingHitReactionMotion(actor);
        return;
    }
    const std::uint32_t hitState = actor.hitStateFlags;
    if ((hitState & 0x100U) != 0 || ((hitState & 0x08000000U) != 0 && (actor.hitFlags & 0x400U) != 0) || ((state & 4U) != 0 && (hitState & 0x08000000U) == 0)) {
        actor.pendingAnimationId = static_cast<std::uint16_t>(actor.defaultReactionAnimationId);
        (void)ApplyPendingHitReactionMotion(actor);
    }
}


void UpdateVFProcessReactionState(VFActor &actor) {
    const std::uint32_t state = actor.hitStateFlags2;
    if ((state & 8U) == 0) {
        actor.pendingAnimationId = VFAnimationTableValue(actor, 10U);
        if (actor.reactionMotionRestartMode < 0)
            vfRestartMotionReverseProgress(actor);
        else
            (void)ApplyPendingHitReactionMotion(actor);
        return;
    }
    if ((state & 0x200U) != 0)
        return;
    if ((actor.moveFlags & 1U) == 0) {
        const std::uint16_t animation = VFAnimationTableValue(actor, 10U);
        const auto &motion = *static_cast<MotionInformationWork *>(actor.motion);
        if (motion.currentAnimId == animation) {
            vfSyncPairedReactionFrameToAttackerWindow(actor);
            return;
        }
        if (actor.reactionMotionRestartMode >= 0) {
            actor.pendingAnimationId = animation;
            vfRestartMotionPreservingProgress(actor);
            vfSyncPairedReactionFrameToAttackerWindow(actor);
            return;
        }
        if ((actor.hitFlags & 0x2000U) != 0) {
            actor.pendingAnimationId = VFAnimationTableValue(actor, 22U);
            if ((state & 2U) != 0)
                vfRestartMotionReverseProgress(actor);
            else
                (void)ApplyPendingHitReactionMotion(actor);
            return;
        }
        if ((state & 4U) != 0)
            return;
    }
    actor.pendingAnimationId = VFAnimationTableValue(actor, 2U);
    (void)ApplyPendingHitReactionMotion(actor);
}

bool vfQueueCounterReaction(VFActor &actor, VFActor &other) {
    const std::uint16_t lastHitMoveId = actor.lastHitMoveId;
    if ((actor.hitStateFlags2 & 0x2000U) != 0 && actor.lastProcessedHitMoveId == lastHitMoveId) {
        std::uint16_t frame = actor.attackWindowEndFrame;
        if (static_cast<std::int16_t>(frame) > 1)
            --frame;
        if (static_cast<std::int16_t>(actor.currentAnimationEndFrame) > static_cast<std::int16_t>(frame)) {
            actor.currentAnimationEndFrame = frame;
            actor.previousAnimationEndFrame = frame;
        }
        return true;
    }
    if ((actor.groundContactFlags & 1) != 0)
        return false;

    const std::uint16_t animation = VFCL_SelectReactionAnimation(other, actor, 0U);
    const std::uint8_t *const header = animation != 0 ? shenLookupAnimationIndex(animation & 0x0fffU, 2) : nullptr;
    if (!header ||
        static_cast<std::int16_t>(ReadPackedU16(header + 2) - 1U) < static_cast<std::int16_t>(other.attackWindowEndFrame - actor.pairReferenceFrame)) {
        return false;
    }
    actor.pendingAnimationId = animation;
    (void)ApplyPendingHitReactionMotion(actor);
    actor.lastProcessedHitMoveId = lastHitMoveId;
    return true;
}


void UpdateVFProcessHitReaction(VFActor &actor) {
    if (actor.targetActor && (actor.targetActor->counterReactionFlags & 1U) != 0) {
        const bool queued = vfQueueCounterReaction(actor, *actor.targetActor);
        actor.hitStateFlags2 &= ~1U;
        actor.pendingAnimationId = queued ? 1U : 0U;
        if (queued)
            return;
    }
    const std::uint32_t moveFlags = actor.moveFlags;
    if ((actor.hitStateFlags2 & 8U) != 0) {
        actor.pendingAnimationId = VFAnimationTableValue(actor, 24U);
        if ((moveFlags & 1U) == 0 && actor.reactionMotionRestartMode >= 0) {
            vfRestartMotionReverseProgress(actor);
        } else {
            (void)ApplyPendingHitReactionMotion(actor);
        }
        return;
    }

    actor.pendingAnimationId = VFAnimationTableValue(actor, 16U);
    if ((actor.hitFlags & 0x2000U) != 0) {
        if (actor.postReactionAnimationId == 0 && (moveFlags & 1U) != 0) {
            (void)ApplyPendingHitReactionMotion(actor);
        }
        return;
    }
    if ((moveFlags & 1U) != 0 || actor.reactionMotionRestartMode >= 0) {
        (void)ApplyPendingHitReactionMotion(actor);
        return;
    }
    actor.pendingAnimationId = VFAnimationTableValue(actor, 24U);
    vfRestartMotionPreservingProgress(actor);
}


void UpdateVFPairedGuardReaction(VFActor &actor) {
    VFActor &target = *actor.targetActor;
    if ((target.counterReactionFlags & 2U) != 0 && (actor.groundContactFlags & 1) == 0) {
        const std::uint16_t animation = VFCL_SelectReactionAnimation(target, actor, 0U);
        if (animation != 0) {
            if (actor.currentReactionAnimation == animation) {
                std::uint16_t frame = actor.attackWindowEndFrame;
                if (frame != 0)
                    --frame;
                if (frame < actor.currentAnimationEndFrame) {
                    actor.currentAnimationEndFrame = frame;
                }
                return;
            }
            const std::uint8_t *const header = shenLookupAnimationIndex(animation & 0x0fffU, 2);
            if (static_cast<std::int16_t>(ReadPackedU16(header + 2)) > static_cast<std::int16_t>(target.attackWindowEndFrame - actor.pairReferenceFrame)) {
                actor.pendingAnimationId = animation;
                (void)ApplyPendingHitReactionMotion(actor);
                return;
            }
        }
    }
    const std::uint32_t moveFlags = actor.moveFlags;
    if ((actor.hitStateFlags2 & 8U) == 0) {
        actor.pendingAnimationId = VFAnimationTableValue(actor, 26U);
        if ((moveFlags & 1U) != 0) {
            (void)ApplyPendingHitReactionMotion(actor);
            return;
        } else if (actor.reactionMotionRestartMode < 0) {
            vfRestartMotionReverseProgress(actor);
        } else {
            (void)ApplyPendingHitReactionMotion(actor);
        }
        vfSyncPairedReactionFrameToAttackerWindow(actor);
        return;
    }

    actor.pendingAnimationId = 943U;
    if ((actor.hitFlags & 0x2000U) == 0) {
        if ((moveFlags & 1U) == 0 && actor.reactionMotionRestartMode >= 0) {
            vfRestartMotionPreservingProgress(actor);
            vfSyncPairedReactionFrameToAttackerWindow(actor);
        } else {
            (void)ApplyPendingHitReactionMotion(actor);
        }
        return;
    }
    if (actor.postReactionAnimationId == 0) {
        if ((moveFlags & 1U) != 0)
            (void)ApplyPendingHitReactionMotion(actor);
        return;
    }
    if ((moveFlags & 1U) != 0 || actor.reactionMotionRestartMode < 0) {
        return;
    }
    vfSyncPairedReactionFrameToAttackerWindow(actor);
}

void vfSelectFallLandingMotion(VFActor &actor);


void UpdateVFHitReactionFollowThrough(VFActor &actor) {
    switch (actor.hitPhase) {
    case 0: {
        const std::uint16_t animation = VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index);
        actor.moveFlags |= 0x10U;
        actor.pendingAnimationId = animation;
        actor.knockbackAcceleration = actor.timeScale;
        if ((actor.hitFlags & 0x00100000U) != 0 && (actor.hitStateFlags & 4U) == 0 &&
            (actor.pairedFighter == nullptr || actor.pairedFighter->fighterIdAlt != 44U))
            actor.facingAngle ^= 0x8000U;
        (void)ApplyPendingHitReactionMotion(actor, true);
        if (shenLookupAnimationIndex(animation & 0x0fffU, 3)) {
            ++actor.hitPhase;
            actor.moveVelocity = {};
        } else {
            actor.moveVelocity.x = actor.pendingKnockback.x;
            actor.moveVelocity.z = actor.pendingKnockback.z;
            actor.knockbackSlideX = actor.pendingKnockback.x * 5.0f;
            actor.knockbackSlideZ = actor.pendingKnockback.z * 5.0f;
            actor.hitPhase = 4;
        }
        return;
    }
    case 1:
        if (actor.currentAnimationEndFrame >= actor.fallLandingWindowStartFrame) {
            actor.moveVelocity.x += actor.pendingKnockback.x;
            actor.moveVelocity.y += actor.pendingKnockback.y;
            actor.moveVelocity.z += actor.pendingKnockback.z;
            vfclSetupAirborneHit(actor);
            vfRecalculateKnockbackFallFrames(actor);
            ++actor.hitPhase;
        }
        return;
    case 2:
        vfSelectFallLandingMotion(actor);
        if ((actor.moveFlags & 4U) == 0) {
            if ((actor.motionFlags & 1U) == 0) {
                actor.currentAnimationEndFrame = actor.knockbackAnimationEndFrame;
                actor.previousAnimationEndFrame = actor.knockbackAnimationEndFrame;
            }
            actor.hitPhase = 4;
        }
        return;
    case 4:
        if (actor.currentAnimationEndFrame >= static_cast<std::uint16_t>(actor.reactionAnimationEndFrame)) {
            if ((actor.hitStateFlags2 & 0x8000U) == 0) {
                actor.action = {};
                actor.action.state = 13;
            }
            actor.moveFlags &= ~0x10U;
            actor.queuedAction = {};
        }
        return;
    default:
        return;
    }
}


void UpdateVFCameraRelativeDodge(VFActor &actor) {
    if (actor.nextAction.state == actor.queuedAction.state)
        return;
    const VFMAData *const game = actor.game;
    const std::uint16_t camera =
        game && actor.slotIndex == game->playerSlot && (game->gameFlags & 0x00400000U) == 0 ? actor.cameraAngleOffset : actor.cameraAngle;
    const bool mirror = (actor.moveFlags & 0x40U) != 0;
    std::size_t offset = 0;
    if ((actor.groundContactFlags & 1) != 0) {
        if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(camera + 0x4000U))) <= 8192) {
            offset = mirror ? 204U : 206U;
        } else {
            offset = mirror ? 238U : 236U;
        }
    } else if (std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(camera - 0x4000U))) <= 8192) {
        offset = mirror ? 30U : 32U;
    } else {
        offset = mirror ? 32U : 30U;
    }
    (void)StartVFQueuedAnimation(actor, VFAnimationTableValue(actor, offset));
    actor.queuedAction = actor.nextAction;
}

std::uint16_t vfResolveAnimIdFor(const VFActor &actor, std::uint16_t moveId) {
    switch (moveId) {
    case 246U:
        return 0x524U;
    case 248U:
        return 0x523U;
    case 250U:
        return 0x528U;
    case 252U:
        return 0x525U;
    case 254U:
        return 0x529U;
    case 256U:
        return 0x526U;
    case 258U:
        return 0x52aU;
    case 260U:
        return 0x527U;
    default:
        return VFAnimationTableValue(actor, moveId);
    }
}

void UpdateVFDirectionalMoveAnimation(VFActor &actor) {
    const bool running = (actor.moveFlags & 0x400U) != 0;
    const bool guarded = (actor.hitFlags & 2U) != 0;
    std::size_t offset = 246U;
    std::uint8_t flags = 8U;
    switch (actor.moveState) {
    case 0:
        if (running) {
            offset = 306U;
            actor.hitStateFlags2 &= ~0x00200000U;
            actor.moveFlags &= ~0x440U;
            if (auto *const motion = static_cast<MotionInformationWork *>(actor.motion)) {
                motion->motionFlags &= ~0x80U;
            }
        }
        break;
    case 1:
        offset = running || guarded ? 258U : 284U;
        flags = 0x48U;
        break;
    case 2:
        offset = running ? 250U : 252U;
        flags = 0x40U;
        break;
    case 3:
        offset = running && !guarded ? 278U : 256U;
        flags = 0x50U;
        break;
    case 4:
        offset = running ? 308U : 248U;
        flags = 0x10U;
        if (running) {
            actor.hitStateFlags2 &= ~0x00200000U;
            actor.moveFlags &= ~0x440U;
            if (auto *const motion = static_cast<MotionInformationWork *>(actor.motion)) {
                motion->motionFlags &= ~0x80U;
            }
        }
        break;
    case 5:
        offset = running || guarded ? 256U : 278U;
        flags = 0x30U;
        break;
    case 6:
        offset = running ? 250U : 252U;
        flags = 0x20U;
        break;
    case 7:
        offset = running && !guarded ? 284U : 258U;
        flags = 0x28U;
        break;
    }




    if ((actor.hitStateFlags2 & 0x400U) != 0 && actor.currentAnimationEndFrame <= actor.reactionEndFrame &&
        (actor.moveAngleTableIndex == actor.moveAngleTableIndex2 || ((actor.cameraControlFlags ^ actor.previousCameraControlFlags) & 0x00f0U) == 0)) {
        return;
    }

    const std::uint8_t previousFlags = actor.moveAnimationFlags;
    const bool useSparringMove =
        actor.game && actor.game->battleType == 2U && actor.targetActor && actor.contactPoints[static_cast<std::size_t>(actor.targetActor->slotIndex)] < 4.0f;
    const std::uint16_t animation = useSparringMove ? vfResolveAnimIdFor(actor, static_cast<std::uint16_t>(offset)) : VFAnimationTableValue(actor, offset);
    if ((actor.hitStateFlags2 & 0x400U) != 0 && actor.currentAnimationEndFrame <= actor.reactionEndFrame) {
        actor.previousMoveAnimationFlags = static_cast<std::int8_t>(previousFlags);
        actor.moveAngleTableIndex2 = actor.moveAngleTableIndex;
        actor.moveAnimationFlags = static_cast<std::uint8_t>(flags | (previousFlags & 0x87U));
        ClearVFQueuedAction(actor);
        (void)StartVFQueuedAnimation(actor, animation);
        return;
    }

    (void)StartVFQueuedAnimation(actor, animation);
    const std::uint8_t postStartFlags = actor.moveAnimationFlags;
    actor.previousMoveAnimationFlags = static_cast<std::int8_t>(postStartFlags);
    actor.moveAngleTableIndex2 = actor.moveAngleTableIndex;
    actor.moveAnimationFlags = static_cast<std::uint8_t>(flags | (postStartFlags & 0x87U));
    ClearVFQueuedAction(actor);
}


void UpdateVFSelectFollowupAnimation(VFActor &actor) {
    std::uint8_t moveAngleIndex = static_cast<std::uint8_t>(actor.moveAngleTableIndex);
    const std::uint16_t style = actor.fightStyleIndex;
    if (style <= 0x39U && (0x0200000000201000ULL & (1ULL << style)) != 0)
        actor.followupState = 1;

    const std::uint8_t followup = actor.followupState;
    if (followup == 3U && (actor.cameraControlFlags & 0x00f0U) == 0) {
        moveAngleIndex = 0;
        actor.cameraControlFlags = 0x20U;
    }
    if (followup == 2U)
        actor.cameraControlFlags = 0;

    const bool primary = followup == 1U;
    const bool noSector = (actor.cameraControlFlags & 0x00f0U) == 0;
    const auto *const motion = static_cast<const MotionInformationWork *>(actor.motion);
    const bool motionEnded = motion && (motion->motionFlags & 2U) != 0;
    std::uint16_t animation;
    if (noSector) {
        if ((actor.hitFlags & 4U) != 0 || ((actor.hitFlags & 2U) != 0 && !motionEnded))
            return;
        const bool exhausted = style == 0 && (actor.currentHp < actor.maximumHp / 2 || actor.game->runningCooldownFrames != 0);
        const bool mirrored = (actor.moveFlags & 0x400U) != 0;
        animation = exhausted ? static_cast<std::uint16_t>(mirrored ? 0x849eU : 1183U) : VFAnimationTableValue(actor, mirrored ? 320U : 318U);
        if (primary)
            animation = static_cast<std::uint16_t>(actor.defaultReactionAnimationId);
    } else if ((actor.motionFlags & 0x08000000U) == 0 || (actor.reactionQueuePhase == 1U && (actor.hitFlags & 4U) == 0) ||
        (followup == 3U && (actor.hitFlags & 0x20000000U) == 0)) {
        animation =
            style == 0 && moveAngleIndex == 4U ? static_cast<std::uint16_t>(primary ? 935U : 621U) : VFAnimationTableValue(actor, primary ? 314U : 316U);
    } else {
        const std::uint16_t current = actor.currentReactionAnimation & 0x0fffU;
        if (current != 935U && current != 621U) {
            static constexpr std::int16_t kMoveAngles[] = {0, 1024, 2048, 3072, 4096, -3072, -2048, -1024};
            actor.facingAngle = static_cast<std::uint16_t>(actor.facingAngle + kMoveAngles[moveAngleIndex]);
        }
        animation = VFAnimationTableValue(actor, primary ? 314U : 316U);


        if (!motionEnded && (actor.hitFlags & 4U) == 0)
            return;
    }

    actor.moveFlags &= ~0x40U;
    actor.knockbackAcceleration = static_cast<float>(static_cast<double>(actor.timeScale) * (primary ? 1.5 : 0.85));
    actor.knockbackAcceleration *= actor.followupKnockbackAccelerationScale;
    if (!noSector) {
        actor.hitFlags &= ~0x00200000U;
        actor.hitStateFlags2 &= ~0x00200000U;
    }
    (void)StartVFQueuedAnimation(actor, animation);
    ClearVFQueuedAction(actor);
}


void UpdateVFAutoFollowup(VFActor &actor) {
    const std::uint16_t animation = static_cast<std::uint16_t>(actor.defaultReactionAnimationId);
    if ((actor.hitStateFlags2 & 0x200U) != 0 && (actor.currentReactionAnimation & 0x0fffU) == animation) {
        return;
    }
    const bool needsMotion = (actor.postReactionAnimationId == 0 && (actor.moveFlags & 1U) != 0) ||
        (((actor.hitStateFlags & 0x08000000U) == 0 || (actor.hitFlags & 4U) == 0 || actor.currentAnimationEndFrame >= actor.reactionEndFrame) &&
            ((actor.hitFlags & 0x2000U) != 0 || (actor.hitStateFlags & 0x48000100U) != 0 || (actor.hitStateFlags2 & 4U) != 0) &&
            (actor.hitStateFlags & 0x00200000U) == 0);
    if (needsMotion)
        (void)StartVFQueuedAnimation(actor, animation);
}


void UpdateVFCameraAssistStep(VFActor &actor) {
    if ((actor.cameraControlFlags & 0x00c0U) == 0)
        return;
    VFActor &target = *actor.game->actors[static_cast<std::uint8_t>(actor.targetSlotIndex)];
    const bool mirror = (actor.moveFlags & 0x40U) != 0;
    const bool sector2 = (actor.cameraControlFlags & 0x80U) != 0;
    const std::size_t offset = sector2 == mirror ? 250U : 252U;
    const float distance = actor.contactPoints[static_cast<std::uint8_t>(actor.targetSlotIndex)];
    const float scale = 0.005f / distance;
    const float dx = (target.contactCurrentPosition.x - actor.contactCurrentPosition.x) * scale;
    const float dz = (target.contactCurrentPosition.z - actor.contactCurrentPosition.z) * scale;
    if ((actor.cameraControlFlags & 0x20U) != 0) {
        actor.position.x += dx;
        actor.position.z += dz;
    } else if ((actor.cameraControlFlags & 0x10U) != 0) {
        actor.position.x -= dx;
        actor.position.z -= dz;
    }
    const auto &motion = *static_cast<const MotionInformationWork *>(actor.motion);
    if ((actor.hitStateFlags2 & 0x400U) == 0 || (motion.motionFlags & 2U) != 0) {
        (void)StartVFQueuedAnimation(actor, VFAnimationTableValue(actor, offset));
    }
}

void UpdateVFSynchronizedReaction(VFActor &actor) {
    VFActor *const target = actor.targetActor;
    if (!target) {
        ClearVFQueuedAction(actor);
        return;
    }
    if (actor.hitPhase == 0) {
        actor.pendingAnimationId = VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index);
        (void)ApplyPendingHitReactionMotion(actor);
        const auto targetWindowRemaining = static_cast<std::uint16_t>(target->attackWindowEndFrame - target->currentAnimationEndFrame);
        const auto ownWindowRemaining = static_cast<std::uint16_t>(actor.attackWindowEndFrame - 1U);
        if (targetWindowRemaining < ownWindowRemaining) {
            actor.currentAnimationEndFrame =
                static_cast<std::uint16_t>(target->currentAnimationEndFrame + actor.attackWindowEndFrame - target->attackWindowEndFrame);
        }
        actor.previousAnimationEndFrame = actor.currentAnimationEndFrame;
        if (actor.game && (actor.game->flags & 9U) == 0 && actor.character) {
            SOND_EmitVFSynchronizedReactionSound(actor.character);
        }
        ++actor.hitPhase;
        return;
    }
    if (actor.hitPhase == 1 && actor.currentAnimationEndFrame >= actor.reactionMidFrame) {
        if ((target->hitStateFlags2 & 4U) != 0 || (target->hitStateFlags2 & 0x100U) == 0) {
            ++actor.hitPhase;
        } else {
            --actor.currentAnimationEndFrame;
        }
    } else if (actor.hitPhase == 2 && actor.currentAnimationEndFrame >= actor.reactionEndFrame) {
        ClearVFQueuedAction(actor);
    }
}


void UpdateVFSimpleReactionLifecycle(VFActor &actor) {
    auto &motion = *static_cast<MotionInformationWork *>(actor.motion);
    if (actor.hitPhase == 0) {
        actor.pendingAnimationId =
            static_cast<std::uint32_t>(static_cast<std::int16_t>(VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index)));
        (void)ApplyPendingHitReactionMotion(actor);
        ++actor.hitPhase;
    }
    if (motion.currentAnimId == 0)
        actor.queuedAction = {};
}


void UpdateVFKnockdownSequence(VFActor &actor) {
    if (actor.hitPhase != 1 && actor.hitPhase != 2 && actor.hitPhase != 3) {
        actor.pendingAnimationId =
            static_cast<std::uint32_t>(static_cast<std::int16_t>(VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index)));
        (void)ApplyPendingHitReactionMotion(actor);
        ++actor.hitPhase;
        actor.minimumGroundY = actor.groundY;
    }
    if (actor.hitPhase != 2 && actor.hitPhase != 3) {
        actor.position.y = actor.minimumGroundY;
        const std::int16_t current = static_cast<std::uint16_t>(vfScaleFrames(actor.animationScale, actor.currentAnimationEndFrame));
        const std::int16_t threshold = static_cast<std::uint16_t>(vfScaleFrames(actor.animationScale, 14));
        if (current <= threshold)
            return;
        ++actor.hitPhase;
    }
    if (actor.hitPhase == 2) {
        --actor.currentAnimationEndFrame;
        actor.previousAnimationEndFrame = actor.currentAnimationEndFrame;
        actor.position.y = static_cast<float>(static_cast<double>(actor.position.y) - 0.2);
        if (static_cast<double>(actor.position.y) >= static_cast<double>(actor.downedReferenceY) + 0.5)
            return;
        ++actor.hitPhase;
    }
    if (actor.currentAnimationEndFrame >= actor.reactionMidFrame) {
        actor.queuedAction = {};
        actor.queuedAction.state = 13;
    }
}


void UpdateVFTrackingSlide(VFActor &actor) {
    VFActor &target = *actor.targetActor;
    if (actor.hitPhase != 1 && actor.hitPhase != 2) {
        actor.pendingAnimationId =
            static_cast<std::uint32_t>(static_cast<std::int16_t>(VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index)));
        (void)ApplyPendingHitReactionMotion(actor);
        actor.trackedHitPosition = target.contactCurrentPosition;
        ++actor.hitPhase;
    }
    if (actor.hitPhase != 2) {
        const std::uint16_t current = actor.currentAnimationEndFrame;
        if (current < actor.trackingSlideStartFrame)
            return;
        const float dz = actor.trackedHitPosition.z - actor.contactCurrentPosition.z;
        float dx = actor.trackedHitPosition.x - actor.contactCurrentPosition.x;
        const std::uint16_t angle = actor.contactAngles[actor.targetSlotIndex];
        actor.moveFlags |= 0x200U;
        actor.trackedContactAngle = angle;
        const float distance = std::sqrt(dz * dz + dx * dx);
        if (distance > actor.hitDamageScale)
            dx *= actor.hitDamageScale / distance;
        const float rootX = sin_fixed16(fixed16_cosine_angle(angle)) * actor.hitReactionScaleX;
        const float rootZ = sin_fixed16(angle) * actor.hitReactionScaleZ;
        ++actor.hitPhase;
        const float step = (dx - (rootX - rootZ)) / static_cast<float>(static_cast<double>(static_cast<float>(actor.attackWindowEndFrame) - current) + 1.0);
        actor.trackingSlideStepX = step;
        actor.trackingSlideStepZ = step;
    }
    if (actor.currentAnimationEndFrame > actor.attackWindowEndFrame) {
        actor.queuedAction = {};
    } else {
        actor.position.x += actor.trackingSlideStepX;
        actor.position.z += actor.trackingSlideStepZ;
    }
}


void UpdateVFSimpleHitReaction(VFActor &actor) {
    const std::int16_t decremented = static_cast<std::int16_t>(actor.hitStunFrames - 1U);
    actor.hitStunFrames = decremented < 0 ? 0U : static_cast<std::uint16_t>(decremented);
    actor.reactionTimer += actor.knockbackStrength;
    if (actor.hitPhase == 0) {
        const std::uint16_t animation = VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index);
        actor.moveFlags |= 0x10U;
        actor.moveVelocity.x = actor.pendingKnockback.x;
        actor.moveVelocity.z = actor.pendingKnockback.z;
        actor.knockbackSlideX = actor.pendingKnockback.x * 5.0f;
        actor.knockbackSlideZ = actor.pendingKnockback.z * 5.0f;
        (void)StartVFQueuedAnimation(actor, animation, true);
        actor.hitStateFlags2 |= 1U;
        if (actor.reactionWindowPhase == 0) {
            actor.reactionEndFrame = static_cast<std::uint16_t>(actor.reactionAnimationEndFrame);
            actor.attackWindowEndFrame = 1;
            actor.reactionMidFrame = 1;
        }
        ++actor.hitPhase;
    } else if (actor.hitPhase == 1 && actor.currentAnimationEndFrame == actor.reactionMidFrame) {
        actor.hitStateFlags2 &= ~1U;
        actor.moveFlags &= ~0x10U;
        ClearVFQueuedAction(actor);
    }
}


void UpdateVFFollowupFacingAndMotion(VFActor &actor) {
    const VFActor *const target = actor.game->actors[actor.targetSlotIndex];
    const float dx = actor.contactCurrentPosition.x - actor.followTargetPosition.x;
    const float dz = actor.contactCurrentPosition.z - actor.followTargetPosition.z;
    const std::int16_t targetYaw = static_cast<std::int16_t>(Shenmue_Atan2Fixed(dx, dz));
    const std::int16_t delta = static_cast<std::int16_t>(targetYaw - actor.facingAngle);
    if (actor.facingQuadrantStep != -1)
        actor.forcedTurnState = 0;
    if (actor.forcedTurnState != 0) {
        if (std::abs(static_cast<std::int32_t>(delta)) <= 4096)
            actor.forcedTurnState = 0;
    } else if (delta > 4096) {
        actor.forcedTurnState = 1;
    } else if (delta < -4096) {
        actor.forcedTurnState = 2;
    }
    if (actor.forcedTurnState == 1)
        actor.facingAngle = static_cast<std::uint16_t>(actor.facingAngle + 4096U);
    else if (actor.forcedTurnState == 2)
        actor.facingAngle = static_cast<std::uint16_t>(actor.facingAngle - 4096U);

    const bool secondary = actor.followupState != 1U;
    const std::size_t animationOffset = secondary ? 316U : 314U;
    const std::int32_t animation =
        static_cast<std::int16_t>((static_cast<std::uint16_t>(actor.animationTable[animationOffset]) << 8U) | actor.animationTable[animationOffset + 1U]);
    const auto *const motion = static_cast<const MotionInformationWork *>(actor.motion);
    const bool hitEvent = (actor.hitFlags & 0x400U) != 0;
    if ((actor.motionFlags & 0x08000000U) == 0 || (actor.hitFlags & 0x4U) != 0 || (motion->motionFlags & 2U) != 0 || (hitEvent != !secondary)) {
        actor.moveFlags &= ~0x40U;
        actor.pendingAnimationId = animation;
        if (secondary) {
            const double multiplier =
                (target->hitStateFlags2 & 0x20000000U) != 0 && (target->groundContactFlags & 1U) != 0 && (actor.groundContactFlags & 1U) == 0 ? 1.2 : 0.85;
            actor.knockbackAcceleration = static_cast<float>(static_cast<double>(actor.timeScale) * multiplier);
        }
        (void)ApplyPendingHitReactionMotion(actor);
        ClearVFQueuedAction(actor);
    }
}

const std::uint8_t *vfFindQueuedReactionEventEntry(const VFActor &actor, std::uint8_t event) {
    const auto *entry = static_cast<const std::uint8_t *>(actor.reactionEventTable);
    for (std::uint8_t remaining = static_cast<std::uint8_t>(actor.reactionEventCount); remaining != 0; --remaining, entry += 8) {
        if (static_cast<std::int8_t>(entry[1]) == event || (event == 4 && entry[1] == 1))
            return entry;
    }
    return nullptr;
}


void vfCopyActionToQueuedActionAndResetHitPhase(VFActor &actor) {
    const bool reset = actor.queuedAction.state != 13;
    actor.queuedAction = actor.nextAction;
    if (reset)
        actor.hitPhase = 0;
}


void VF_SetKnockbackAnimationVariablesAndTriggerReaction(VFActor &actor) {
    if (actor.currentAnimationEndFrame > static_cast<std::uint16_t>(vfScaleFrames(actor.animationScale, 9)) ||
        actor.nextAction.state != actor.queuedAction.state) {
        actor.pendingAnimationId = VFAnimationTableValue(actor, 290);
        (void)ApplyPendingHitReactionMotion(actor);
        actor.queuedAction = actor.nextAction;
    }
}

void vfQueueStepAnimationIfDue(VFActor &actor) {
    if (actor.currentAnimationEndFrame > static_cast<std::uint16_t>(vfScaleFrames(actor.animationScale, 9)) ||
        actor.nextAction.state != actor.queuedAction.state) {
        actor.pendingAnimationId = VFAnimationTableValue(actor, 286);
        (void)ApplyPendingHitReactionMotion(actor);
        actor.queuedAction = actor.nextAction;
    }
}


void vfQueueReactionEventFromEntryTable(VFActor &actor) {
    if (vfFindQueuedReactionEventEntry(actor, actor.nextAction.opcode))
        actor.queuedReactionEvent = VFCore_ActionAnimation(actor.nextAction.opcode, actor.nextAction.index);
}


void VFM_ResetHitPhase(VFActor &actor) {
    actor.queuedAction = actor.nextAction;
    actor.hitPhase = 0;
}


void VFActor_UpdateQueuedActionIdx(VFActor &actor) {
    if (actor.nextAction.state != actor.queuedAction.state) {
        actor.queuedAction = actor.nextAction;
        actor.actionPhaseFrameCounter = 0;
        actor.hitPhase = 0;
    }
}


void VFActor_ClearActionFields(VFActor &actor) {
    actor.queuedAction = actor.nextAction;
    actor.actionPhaseFrameCounter = 0;
    actor.hitPhase = 0;
}


void vfActionNoOp(VFActor &) {
}

void vfPreUpdateAction(VFActor &actor) {
    switch (actor.nextAction.state) {
    case 7:
    case 8:
        (void)StartVFQueuedAnimation(actor, VFAnimationTableValue(actor, actor.nextAction.state == 7 ? 40U : 42U));
        actor.queuedAction = actor.nextAction;
        return;
    case 9:
    case 29:
    case 30:
        vfActionNoOp(actor);
        return;
    case 13:
        vfCopyActionToQueuedActionAndResetHitPhase(actor);
        return;
    case 18:
        vfQueueStepAnimationIfDue(actor);
        return;
    case 19:
        VF_SetKnockbackAnimationVariablesAndTriggerReaction(actor);
        return;
    case 20:
        vfQueueReactionEventFromEntryTable(actor);
        return;
    case 26:
    case 31:
    case 32:
        VFActor_UpdateQueuedActionIdx(actor);
        return;
    case 33:
        UpdateVFCameraRelativeDodge(actor);
        return;
    case 45:
        VFM_ResetHitPhase(actor);
        return;
    default:
        VFActor_ClearActionFields(actor);
        return;
    }
}

void vfDispatchQueuedReactionEvent(VFActor &actor) {
    switch (actor.queuedReactionEvent) {
    case 0:
        return;
    case 31:
        actor.pendingAnimationId = actor.queuedReactionAnimationId;
        (void)ApplyPendingHitReactionMotion(actor);
        return;
    case 30:
        if ((actor.hitStateFlags2 & 0x100U) != 0) {
            actor.pendingAnimationId = actor.queuedKnockbackReactionAnimId;
            actor.knockbackAcceleration = actor.game->knockbackAmount;
            (void)ApplyPendingHitReactionMotion(actor);
            actor.queuedReactionEvent = 0;
            actor.reactionEventCount = 0;
            actor.reactionEventTable = nullptr;
            actor.queuedReactionAnimationId = 0;
        }
        return;
    default: {
        const auto *entry = vfFindQueuedReactionEventEntry(actor, static_cast<std::uint8_t>(actor.queuedReactionEvent));
        if (!entry) {
            actor.queuedReactionEvent = 0;
            return;
        }
        const std::uint16_t frames = ReadPackedU16(entry + 2);
        const std::uint16_t threshold = actor.animationScale == 1.0f ? frames : static_cast<std::uint16_t>(actor.animationScale * frames + 0.5f);
        if (actor.currentAnimationEndFrame >= threshold) {
            actor.pendingAnimationId = ReadPackedU16(entry + 4);
            (void)ApplyPendingHitReactionMotion(actor);
        }
        return;
    }
    }
}

Vector3 vfRotateRootTranslation(const Vector3 &root, std::uint16_t angle) {
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    const float cosine = sin_fixed16(fixed16_cosine_angle(angle));
    return {cosine * root.x - sine * root.z, root.y, cosine * root.z + sine * root.x};
}

bool vfAssignVoiceFor(VFMAData &game, std::uint8_t voiceCode);

void vfSelectHitReactionSequence(VFActor &actor, std::uint8_t index) {
    VFMAData &game = *actor.game;
    if (g_pCVWK->slots[0].callback || g_pCVWK->slots[1].callback)
        return;
    if (game.voiceDelay != 0 || game.frameCount < 150U)
        return;
    std::uint8_t bank = 0;
    if (actor.fightStyleIndex != 12 && actor.fightStyleIndex != 57) {
        if (actor.fightStyleIndex != 0)
            return;
        bank = 1;
    }
    // Reaction voice banks are selected by fighter style.
    static constexpr std::uint8_t commands[10][8] = {
        {3, 8, 11, 12},
        {7, 0, 7, 8, 9, 10, 20, 21},
        {},
        {3, 14, 15, 16},
        {},
        {2, 2, 17},
        {1, 4},
        {2, 13, 19},
        {1, 6},
        {1, 18},
    };
    static constexpr float probabilities[5] = {0.3f, 0.3f, 1.0f, 0.2f, 1.0f};
    const auto &row = commands[index + 5U * bank];
    if (row[0] == 0)
        return;
    if (static_cast<float>(NextVFFrameEventRandomWord()) / 32768.0f > probabilities[index])
        return;
    for (std::uint8_t entry = 1; entry <= row[0]; ++entry) {
        if (vfAssignVoiceFor(game, row[entry]))
            break;
    }
}


void vfReactionApplyRootMotionAndReconcilePair(VFActor &actor) {
    VFActor &pair = *actor.pairedActor;
    auto &motion = *static_cast<MotionInformationWork *>(actor.motion);
    auto &pairedMotion = *static_cast<MotionInformationWork *>(pair.motion);
    const std::uint32_t flags = actor.hitFlags;
    const std::int32_t animation = static_cast<std::int16_t>(VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index));
    if ((flags & 0xc0000U) != 0 && animation != 0 && actor.currentReactionAnimation != animation) {
        Vector3 outgoing{};
        actor.moveFlags &= ~0x10U;
        actor.hitStateFlags2 &= ~0x60U;
        actor.pendingAnimationId = static_cast<std::uint32_t>(animation);
        actor.knockbackAcceleration = actor.timeScale;
        (void)MOTN_ReadVfRootPosition(
            motion, static_cast<float>(static_cast<std::int16_t>((static_cast<double>(actor.currentAnimationEndFrame) - 1.0) * actor.timeScale)), outgoing);
        (void)ApplyPendingHitReactionMotion(actor);
        if ((flags & 0x40000U) != 0 && (actor.hitStateFlags2 & 0x80U) == 0) {
            const Vector3 carry = vfRotateRootTranslation(outgoing, static_cast<std::uint16_t>(-actor.facingAngle));
            actor.separation.x = 0.0f;
            actor.separation.z = 0.0f;
            actor.position.x += carry.x;
            actor.position.z += carry.z;
            actor.contactCurrentPosition.x += carry.x;
            actor.contactCurrentPosition.z += carry.z;
        }
        if ((actor.hitStateFlags & 0x400U) != 0)
            actor.moveFlags |= 0x10U;
        if ((flags & 0x40000U) != 0) {
            actor.hitPhase = 3;
            return;
        }
        actor.hitPhase = 4;
    }
    if (actor.hitPhase == 1) {
        Vector3 ownRoot{};
        Vector3 pairedRoot{};
        (void)MOTN_ReadVfRootPosition(motion, 0.0f, ownRoot);
        (void)MOTN_ReadVfRootPosition(pairedMotion, 0.0f, pairedRoot);
        const auto angle = static_cast<std::uint16_t>(-pair.facingAngle);
        ownRoot = vfRotateRootTranslation(ownRoot, angle);
        pairedRoot = vfRotateRootTranslation(pairedRoot, angle);
        actor.pairAnchor.x = actor.position.x - ownRoot.x;
        actor.pairAnchor.z = actor.position.z - ownRoot.z;
        actor.separation.x = pair.position.x - pairedRoot.x - actor.pairAnchor.x;
        actor.separation.z = pair.position.z - pairedRoot.z - actor.pairAnchor.z;
        actor.hitPhase = 3;
    } else if (actor.hitPhase == 2) {
        actor.hitPhase = 3;
    }
    if (actor.hitPhase == 3) {
        if ((actor.hitStateFlags & 0x400U) != 0)
            actor.moveFlags |= 0x10U;
        if ((pair.hitStateFlags2 & 0x80000U) != 0) {
            const auto difference = static_cast<std::int16_t>(pair.facingAngle - actor.facingAngle);
            if (static_cast<std::uint16_t>(difference + 128) > 256U)
                actor.facingAngle = static_cast<std::uint16_t>(actor.facingAngle + difference / 2);
        }
        if (static_cast<double>(std::fabs(actor.separation.x)) > 0.001) {
            actor.separation.x *= 0.5f;
            actor.position.x += actor.separation.x;
            actor.contactCurrentPosition.x += actor.separation.x;
        }
        if (static_cast<double>(std::fabs(actor.separation.z)) > 0.001) {
            actor.separation.z *= 0.5f;
            actor.position.z += actor.separation.z;
            actor.contactCurrentPosition.z += actor.separation.z;
        }
    }
    if (actor.hitPhase == 3 || actor.hitPhase == 4) {
        if (pair.latchedPairContactPriority == 0) {
            if ((actor.triggerMask & 8U) != 0)
                actor.triggerState |= 8U;
            if ((actor.triggerMask & 2U) != 0 && static_cast<std::uint16_t>(static_cast<std::int8_t>(actor.fighterRankId)) == actor.requiredFighterRank)
                actor.triggerState |= 2U;
            pair.fighterIdAlt3 = actor.fighterRankId;
        }
        if (actor.currentAnimationEndFrame >= actor.reactionEndFrame)
            motion.motionFlags &= ~8U;
        if (actor.currentAnimationEndFrame >= static_cast<std::uint16_t>(actor.reactionAnimationEndFrame)) {
            actor.moveFlags &= ~0x10U;
            if (actor.previousHitStateFlags == 4) {
                actor.hitStunFrames = 2;
                actor.action = {};
                actor.action.state = 13;
            } else {
                const std::uint16_t followup = static_cast<std::uint16_t>(actor.postReactionAnimationId);
                if (followup != 0 && (actor.hitStateFlags & 0xc00U) != 0) {
                    const auto next = followup == 0xffffU ? actor.currentReactionAnimation : followup;
                    actor.action = {};
                    actor.action.opcode = static_cast<std::uint8_t>(next);
                    actor.action.index = static_cast<std::uint8_t>(next >> 8U);
                    actor.action.state = 16;
                }
                actor.queuedAction = {};
            }
        }
        return;
    }
    actor.moveFlags &= ~0x10U;
    actor.pendingAnimationId = static_cast<std::uint32_t>(animation);
    actor.knockbackAcceleration = actor.timeScale;
    (void)ApplyPendingHitReactionMotion(actor);
    if (actor.game->reactionTypeIndex == 0)
        vfSelectHitReactionSequence(actor, (actor.queuedAction.resolve & 1U) != 0 ? 4U : 3U);
    ++actor.hitPhase;
    actor.moveVelocity = {};
}

void vfSelectFallLandingMotion(VFActor &actor) {
    if ((actor.moveFlags & 0x8000U) == 0 || (actor.motionFlags & 1U) != 0)
        return;
    if ((actor.hitStateFlags2 & 0x80000U) != 0 && actor.game->guardEffectTimer != 0) {
        const std::uint16_t strength = actor.guardStrength;
        actor.hitStunFrames = static_cast<std::uint16_t>(actor.guardImpactAccum + (strength >> 1U));
        actor.guardImpactAccum = 0;
        if (strength != 0)
            vfHandleImpactSfxAndDefeat(actor, static_cast<std::int16_t>(strength), 0);
    }
    const auto direction = Shenmue_Atan2Fixed(actor.moveVelocity.x, actor.moveVelocity.z);
    const auto difference = static_cast<std::int16_t>(direction - ((actor.hitStateFlags2 & 0x80000U) != 0 ? actor.temporaryFacingAngle : actor.facingAngle));
    std::uint16_t animation;
    if (static_cast<std::uint16_t>(std::abs(static_cast<int>(difference)) + 0x4000) > 0x8000U) {
        if (static_cast<std::uint16_t>(actor.airAngle + 8192) > 16384U || (actor.hitStateFlags2 & 0x800U) == 0 ||
            actor.contactCurrentPosition.y < actor.groundY + 0.8f)
            return;
        animation = 629;
    } else {
        if (static_cast<std::uint16_t>(actor.airTiltAngle + 8192) > 16384U || (actor.hitStateFlags2 & 0x800U) != 0 ||
            actor.contactCurrentPosition.y < actor.groundY + 0.8f)
            return;
        if (actor.guardProbeWorldPosition.y >= (actor.fightStyleIndex == 6 ? 0.8f : 0.6f))
            animation = actor.contactCurrentPosition.y <= actor.groundY + 1.7f ? 630 : 644;
        else
            animation = 643;
    }
    actor.queuedReactionAnimationId = animation;
    actor.contactCurrentPosition.x = actor.position.x;
    actor.contactCurrentPosition.z = actor.position.z;
    if (actor.moveVelocity.y > 0.0f)
        actor.moveVelocity.y *= 0.4f;
    actor.temporaryFacingAngle = static_cast<std::uint16_t>(actor.temporaryFacingAngle + actor.facingAngleStep);
    actor.queuedReactionEvent = 31;
    actor.knockbackAcceleration = actor.timeScale;
}


void vfUpdatePairedHitReactionState(VFActor &actor) {
    VFActor &source = *actor.hitSourceActor;
    auto &motion = *static_cast<MotionInformationWork *>(actor.motion);
    motion.motionFlags &= ~8U;
    if (actor.hitPhase == 1) {
        if (actor.currentAnimationEndFrame < actor.knockbackAnimationEndFrame) {
            if ((actor.hitStateFlags2 & 0x10U) != 0)
                vfSelectFallLandingMotion(actor);
            if ((source.hitStateFlags2 & 0x40000U) == 0 && (actor.hitStateFlags2 & 0x80000U) != 0 &&
                static_cast<std::uint16_t>(actor.field332) > actor.currentAnimationEndFrame) {
                actor.moveFlags &= ~0x10U;
                actor.hitStateFlags2 &= ~0x40U;
                actor.action = {};
                actor.action.state = 1;
                if ((actor.hitFlags & 0x40U) != 0) {
                    actor.hitStateFlags2 |= 0x40U;
                    actor.facingAngleStep = 0x8000U;
                }
            }
            return;
        }
        actor.moveFlags &= ~8U;
        actor.hitPhase = 2;
    }
    if (actor.hitPhase == 2) {
        if ((actor.moveFlags & 4U) != 0) {
            actor.currentAnimationEndFrame = actor.knockbackAnimationEndFrame;
            actor.previousAnimationEndFrame = actor.knockbackAnimationEndFrame;
            return;
        }
        actor.hitStateFlags2 &= ~0x10U;
        if ((actor.motionFlags & 1U) == 0) {
            const bool special = (actor.hitStateFlags2 & 0x4000U) != 0;
            if (special) {
                actor.hitStunFrames = static_cast<std::uint8_t>(static_cast<std::uint8_t>(actor.guardImpactAccum) +
                    (static_cast<std::uint8_t>(static_cast<int>(static_cast<std::uint8_t>(actor.guardStrength) * actor.animationScale)) >> 1U));
                actor.guardImpactAccum = 0;
            }
            if ((actor.hitStateFlags & 0xc00U) != 0) {
                actor.hitPhase = 4;
            } else {
                std::int16_t damage = (source.hitStateFlags & 0x400U) != 0 ? static_cast<std::uint8_t>(static_cast<std::uint16_t>(source.skipAlignment) >> 8U) :
                                                                             static_cast<std::uint8_t>(source.guardStrength);
                const auto selection = static_cast<std::uint8_t>(actor.game->battleMeta.waves[actor.slotIndex]->selectFlags);
                if (((selection & 0x80U) != 0 && (source.hitStateFlags & 0x4000008U) != 0) ||
                    ((selection & 0x20U) != 0 && (source.hitStateFlags & 0x4000000U) != 0))
                    damage = actor.currentHp;
                if (damage != 0) {
                    vfHandleImpactSfxAndDefeat(
                        actor, static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(VF_GetLocalActorAnimScale(source) * damage) + 0.5)), 0);
                } else if (!special) {
                    actor.moveFlags &= ~0x10U;
                    actor.queuedAction = {};
                    motion.motionFlags &= ~8U;
                    return;
                }
                if ((actor.hitStateFlags2 & 0x4000U) == 0) {
                    if (actor.currentHp != 0) {
                        actor.moveFlags &= ~0x10U;
                        actor.queuedAction = {};
                        return;
                    }
                    if (vfFindQueuedReactionEventEntry(actor, 3)) {
                        actor.queuedReactionEvent = 3;
                        ++actor.hitPhase;
                    } else {
                        actor.action = {};
                        actor.action.opcode = 7;
                        actor.action.index = 2;
                        actor.action.state = 12;
                    }
                    return;
                }
            }
        }
        if (actor.hitPhase == 2)
            ++actor.hitPhase;
    }
    if (actor.hitPhase == 4) {
        if (actor.currentAnimationEndFrame >= static_cast<std::uint16_t>(actor.reactionAnimationEndFrame)) {
            const auto followup = static_cast<std::uint16_t>(actor.postReactionAnimationId);
            if (static_cast<std::uint16_t>(followup - 1U) <= 65533U && (actor.hitStateFlags & 0xc00U) != 0) {
                actor.action = {};
                actor.action.opcode = static_cast<std::uint8_t>(followup);
                actor.action.index = static_cast<std::uint8_t>(followup >> 8U);
                actor.action.state = 16;
            }
            actor.moveFlags &= ~0x10U;
            actor.queuedAction = {};
        }
        return;
    }
    if (actor.hitPhase == 3) {
        if (actor.currentAnimationEndFrame >= static_cast<std::uint16_t>(actor.reactionAnimationEndFrame)) {
            if ((actor.moveFlags & 0x20U) != 0 || actor.currentHp <= 0) {
                actor.hitStateFlags2 = (actor.hitStateFlags2 & ~0x94000U) | 0x14000U;
            } else {
                actor.action = {};
                actor.action.state = 13;
            }
        }
        return;
    }
    const bool align = (actor.hitFlags & 0x80000U) != 0;
    Vector3 outgoing{};
    actor.pendingAnimationId =
        static_cast<std::uint32_t>(static_cast<std::int16_t>(VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index)));
    actor.knockbackAcceleration = actor.timeScale;
    if (align) {
        (void)MOTN_ReadVfRootPosition(
            motion, static_cast<float>(static_cast<std::int16_t>((static_cast<double>(actor.currentAnimationEndFrame) - 1.0) * actor.timeScale)), outgoing);
        actor.hitStateFlags2 &= ~0x40U;
        if ((actor.hitFlags & 0x100000U) != 0)
            actor.facingAngle ^= 0x8000U;
    }
    (void)ApplyPendingHitReactionMotion(actor, true);
    if (align) {
        Vector3 sourceRoot{};
        (void)MOTN_ReadVfRootPosition(*static_cast<MotionInformationWork *>(source.motion), 0.0f, sourceRoot);
        const Vector3 aligned =
            vfRotateRootTranslation({outgoing.x - sourceRoot.x, 0.0f, outgoing.z - sourceRoot.z}, static_cast<std::uint16_t>(-source.facingAngle));
        actor.position.x = source.position.x + aligned.x;
        actor.position.z = source.position.z + aligned.z;
    }
    actor.moveFlags |= 0x10U;
    actor.moveFacingAngle = source.facingAngleStep;
    actor.moveVelocity = {};
    ++actor.hitPhase;
}


void vfRemoveQueuedActionAtEnd(VFActor &actor) {
    if (actor.currentAnimationEndFrame >= static_cast<std::uint16_t>(actor.reactionAnimationEndFrame))
        actor.queuedAction = {};
}


void vfUpdatePairedReactionSupervisor(VFActor &actor) {

    const VFActor &pair = *actor.pairedActor;
    if (actor.hitPhase != 1) {
        actor.hitReactionAnimId = static_cast<std::int16_t>(pair.currentReactionAnimation);
        actor.hitPhase = 1;
    } else if ((pair.hitStateFlags2 & 0x100U) != 0 && pair.currentReactionAnimation != 0) {
        actor.moveFlags |= 0x1000U;
        UpdateVFFallbackGuardOrReaction(actor);
        return;
    }
    actor.hitStateFlags2 &= ~1U;
    ClearVFQueuedAction(actor);
    return;
}


void VF_SetHitReactionAndClearAction(VFActor &actor) {

    actor.hitStateFlags2 &= ~0x100U;
    if (actor.currentAnimationEndFrame < actor.attackWindowEndFrame)
        return;
    if (actor.contactPoints[actor.targetSlotIndex] > actor.hitDamageScale) {
        ClearVFQueuedAction(actor);
        return;
    }
    const std::int32_t animation = static_cast<std::int16_t>(VFCore_ActionAnimation(actor.queuedAction.opcode, actor.queuedAction.index));
    actor.pendingAnimationId = static_cast<std::uint32_t>(animation);
    actor.queuedAction = std::bit_cast<VFActionDescriptor>(static_cast<std::uint64_t>(static_cast<std::uint32_t>(animation + 0x11000000)));
    (void)ApplyPendingHitReactionMotion(actor);
    ++actor.hitPhase;
    actor.moveVelocity = {};
    return;
}


void VFActor_SetupMidAirRecoveryAnimation(VFActor &actor) {
    const auto end = static_cast<std::int16_t>(actor.midairRecoveryEndFrame);
    std::uint16_t frame = static_cast<std::uint16_t>(end);
    const bool pairedRecovery = actor.pairedActor->queuedAction.state == 17;
    if (!pairedRecovery) {
        const auto elapsed = static_cast<std::int16_t>(actor.actionPhaseFrameCounter + 1);
        const auto duration = std::max<std::int16_t>(1, static_cast<std::int16_t>(elapsed + actor.pairFrameOffset - actor.pairReferenceFrame));
        frame = static_cast<std::uint16_t>(elapsed * (end - 1) / duration + 1);
    }
    actor.currentAnimationEndFrame = frame;
    actor.previousAnimationEndFrame = frame;
    if (pairedRecovery || frame == end) {
        actor.hitStateFlags2 |= 0x40000U;
        actor.queuedAction = {};
        actor.queuedAction.state = 16;
        actor.moveVelocity = {};
        actor.hitPhase = 1;
    }
    return;
}


void VF_UpdateHitReactionState(VFActor &actor) {
    if (actor.hitPhase == 1) {
        const std::uint16_t threshold = actor.animationScale == 1.0f ? 10U : static_cast<std::uint16_t>(actor.animationScale * 10.0f + 0.5f);
        if (actor.currentAnimationEndFrame >= threshold)
            return;
        actor.hitStateFlags2 |= 4U;
        actor.hitPhase = 2;
    }
    if (actor.hitPhase == 2) {
        actor.currentAnimationEndFrame = static_cast<std::uint16_t>(actor.currentAnimationEndFrame - 2U);
        actor.previousAnimationEndFrame = actor.currentAnimationEndFrame;
        if (actor.currentAnimationEndFrame <= 1U) {
            actor.hitStateFlags2 &= ~1U;
            ClearVFQueuedAction(actor);
            actor.reactionAnimationEndFrame = 1;
        }
        return;
    }
    (void)ApplyPendingHitReactionMotion(actor);
    actor.hitStateFlags2 &= ~0x00040000U;
    ++actor.hitPhase;
    if ((actor.hitStateFlags2 & 0x00100000U) != 0) {
        actor.hitStateFlags2 |= 0x40U;
        actor.facingAngleStep = 0x8000U;
    } else {
        actor.facingAngleStep = 0;
    }
    return;
}


void vfTransitionToDefaultGuardAnim(VFActor &actor) {
    if ((actor.targetActor->counterReactionFlags & 1U) != 0)
        actor.hitStateFlags2 &= ~1U;
    const std::uint16_t animation = static_cast<std::uint16_t>(actor.defaultGuardAnimationId);
    const std::uint16_t current = actor.currentReactionAnimation;
    actor.pendingAnimationId = animation;
    if (current != animation) {
        if ((actor.moveFlags & 1U) != 0)
            (void)ApplyPendingHitReactionMotion(actor);
        else
            vfRestartMotionPreservingProgress(actor);
    }
    return;
}


void vfTransitionToDefaultStanceAnim(VFActor &actor) {
    if ((actor.targetActor->counterReactionFlags & 2U) != 0)
        actor.hitStateFlags2 &= ~1U;
    const std::uint16_t animation = static_cast<std::uint16_t>(actor.defaultStanceAnimationId);
    const std::uint16_t current = actor.currentReactionAnimation;
    actor.pendingAnimationId = animation;
    if (current != animation) {
        if ((actor.moveFlags & 1U) != 0)
            (void)ApplyPendingHitReactionMotion(actor);
        else
            vfRestartMotionPreservingProgress(actor);
    }
    return;
}

void UpdateVFQueuedActionLifecycle(VFActor &actor) {
    switch (actor.queuedAction.state) {
    case 0:
        UpdateVFSimpleReactionLifecycle(actor);
        return;
    case 1:
        UpdateVFFallbackGuardOrReaction(actor);
        return;
    case 2:
        UpdateVFProcessReactionState(actor);
        return;
    case 10:
        UpdateVFProcessHitReaction(actor);
        return;
    case 11:
        UpdateVFPairedGuardReaction(actor);
        return;
    case 12:
        UpdateVFHitReactionFollowThrough(actor);
        return;
    case 13:
        UpdateVFHitReactionPhase(actor);
        return;
    case 14:
        UpdateVFStandingHitReaction(actor);
        return;
    case 15:
        UpdateVFAlternateHitReaction(actor);
        return;
    case 16:
        vfReactionApplyRootMotionAndReconcilePair(actor);
        return;
    case 17:
        vfUpdatePairedHitReactionState(actor);
        return;
    case 7:
    case 8:
    case 18:
    case 19:
    case 29:
    case 30:
    case 33:
        vfRemoveQueuedActionAtEnd(actor);
        return;
    case 21:
        UpdateVFTrackingSlide(actor);
        return;
    case 22:
        vfUpdatePairedReactionSupervisor(actor);
        return;
    case 23:
        VF_SetHitReactionAndClearAction(actor);
        return;
    case 25:
        VFActor_SetupMidAirRecoveryAnimation(actor);
        return;
    case 34:
        VF_UpdateHitReactionState(actor);
        return;
    case 35:
        UpdateVFKnockdownSequence(actor);
        return;
    case 37:
        UpdateVFDirectionalMoveAnimation(actor);
        return;
    case 38:
        UpdateVFSelectFollowupAnimation(actor);
        return;
    case 39:
        UpdateVFAutoFollowup(actor);
        return;
    case 40:
        vfTransitionToDefaultGuardAnim(actor);
        return;
    case 41:
        vfTransitionToDefaultStanceAnim(actor);
        return;
    case 42:
        UpdateVFSimpleHitReaction(actor);
        return;
    case 43:
        UpdateVFFollowupFacingAndMotion(actor);
        return;
    case 44:
        UpdateVFCameraAssistStep(actor);
        return;
    case 45:
        UpdateVFSynchronizedReaction(actor);
        return;
    default:
        vfActionNoOp(actor);
        return;
    }
}


void vfclUpdatePairingFacing(VFActor &actor) {
    const VFActor *const target = actor.targetActor;
    if (!target || actor.targetSlotIndex < 0 || static_cast<std::size_t>(actor.targetSlotIndex) >= kBattleSlots)
        return;
    const std::uint32_t move = actor.moveFlags;
    const std::uint32_t state = actor.hitStateFlags2;
    const std::uint32_t motion = actor.motionFlags;
    const std::uint32_t targetState = target->hitStateFlags2;
    if ((move & 0x00400020U) != 0 || (state & 0x08404000U) != 0 || (state & 0x1010U) == 0x1010U ||
        ((state & 0x10100U) == 0x10000U && (targetState & 0x110U) != 0) || (motion & 0x00200c04U) != 0)
        return;

    const std::size_t targetIndex = static_cast<std::size_t>(actor.targetSlotIndex);
    const std::uint16_t animation = actor.currentReactionAnimation & 0x0fffU;
    if ((motion & 0x08000000U) != 0 && (state & 0x100U) == 0 &&
        !(actor.fightStyleIndex == 0 && (actor.hitFlags & 4U) != 0 && actor.contactPoints[targetIndex] < 3.0f && (actor.groundContactFlags & 1U) == 0 &&
            animation != 1182U && animation != 1183U))
        return;

    std::uint8_t mode;
    if ((motion & 0x1000U) != 0) {
        mode = 5;
    } else if ((state & 0x10000000U) != 0) {
        mode = (state & 0x100U) != 0 ? 0 : static_cast<double>(actor.contactPoints[targetIndex]) >= 0.4 ? 3 : 2;
    } else if ((motion & 0x02000000U) != 0) {
        mode = 2;
    } else if ((state & 0x2000U) != 0) {
        mode = 0;
    } else if ((state & 0x400U) != 0) {
        mode = 2;
    } else {
        if ((target->hitFlags & 0x10000000U) != 0 && ((state & 0x10000100U) != 0 || target->hitTimeout != 0))
            return;
        if ((state & 0x000c0000U) != 0) {
            if ((state & 2U) == 0)
                return;
            mode = 4;
        } else if (((motion & 0x800U) != 0 && (state & 0x100U) != 0) || ((motion & 0x400U) != 0 && (targetState & 0x100U) != 0) ||
            (state & 0x8100U) == 0x8100U) {
            mode = 2;
        } else if ((state & 4U) != 0) {
            if ((state & 0x14010U) != 0 || (actor.hitFlags & 0x100U) != 0)
                return;
            mode = 2;
        } else if ((state & 0x104U) == 0x100U || (state & 0x1102U) == 0x1002U) {
            mode = 2;
        } else if ((state & 0x20004U) == 0x20000U) {
            mode = 0;
        } else if ((targetState & 0x4010U) == 0x4010U && (state & 0x200U) == 0) {
            mode = 2;
        } else {
            if ((state & 2U) != 0)
                return;
            if ((state & 0x8000U) != 0)
                mode = 0;
            else if ((state & 0x40000000U) != 0)
                mode = 1;
            else
                return;
        }
    }

    std::int16_t delta = static_cast<std::int16_t>(actor.contactAngles[targetIndex] - actor.temporaryFacingAngle);
    std::int16_t stepLimit = 1820;
    std::int16_t angleLimit = 24570;
    bool rejectReverse = false;
    switch (mode) {
    case 0:
        if ((targetState & 0x8000U) != 0) {
            stepLimit = 182;
            angleLimit = 10920;
            rejectReverse = true;
        }
        break;
    case 1:
        stepLimit = 728;
        angleLimit = 10920;
        rejectReverse = true;
        break;
    case 3:
        stepLimit = angleLimit = 16380;
        break;
    case 4:
        delta = static_cast<std::int16_t>(target->facingAngle - actor.facingAngle);
        break;
    case 5: {
        delta = static_cast<std::int16_t>(actor.contactAngles[targetIndex] - actor.facingAngle);
        const std::int16_t frames = std::max<std::int16_t>(1, static_cast<std::int16_t>(actor.attackWindowEndFrame - actor.currentAnimationEndFrame + 1));
        stepLimit = static_cast<std::int16_t>(delta / frames);
        angleLimit = 32760;
        break;
    }
    default:
        break;
    }
    if (delta < -angleLimit || delta > angleLimit) {
        if (rejectReverse)
            return;
        delta = static_cast<std::int16_t>(delta + 0x8000);
    }
    const std::int16_t bounded = delta < -stepLimit ? static_cast<std::int16_t>(-stepLimit) : delta > stepLimit ? stepLimit : delta;
    const std::int32_t step = static_cast<std::int32_t>(static_cast<float>(bounded) * actor.knockbackStrength);
    if ((move & 4U) == 0 || (actor.skipAlignment & 1) == 0) {
        const float x = actor.position.x - actor.contactCurrentPosition.x;
        const float z = actor.position.z - actor.contactCurrentPosition.z;
        const auto yaw = static_cast<std::uint16_t>(-step);
        const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
        const float cosine = sin_fixed16(fixed16_cosine_angle(yaw));
        actor.position.x = actor.contactCurrentPosition.x + cosine * x - sine * z;
        actor.position.z = actor.contactCurrentPosition.z + cosine * z + sine * x;
    }
    actor.facingAngle = static_cast<std::uint16_t>(actor.facingAngle + step);
}


void UpdateVFKnockbackPhysics(VFActor &actor) {
    VFMAData *const game = actor.game;
    if (!game)
        return;
    const float strength = actor.knockbackStrength * actor.knockbackStrength;
    const float drag = static_cast<float>(
        static_cast<float>(
            static_cast<double>(strength * game->knockbackGravityScale * game->knockbackDragScale * actor.knockbackBaseParameter) * (1.0 / 60.0)) *
        (static_cast<double>(actor.reactionTimer) + 1.0));
    const float slideDrag = drag * game->knockbackSlideDragScale;

    bool moving = false;
    if ((actor.hitStateFlags2 & 0x00000010U) == 0) {
        const float speed = std::sqrt(actor.moveVelocity.x * actor.moveVelocity.x + actor.moveVelocity.z * actor.moveVelocity.z);
        if (speed <= drag) {
            actor.moveVelocity.x = 0.0f;
            actor.moveVelocity.z = 0.0f;
        } else {
            const float factor = 1.0f - drag / speed;
            actor.moveVelocity.x *= factor;
            actor.moveVelocity.z *= factor;
            moving = true;
        }
    }
    const float slideSpeed = std::sqrt(actor.knockbackSlideX * actor.knockbackSlideX + actor.knockbackSlideZ * actor.knockbackSlideZ);
    if (slideSpeed <= slideDrag) {
        actor.knockbackSlideX = 0.0f;
        actor.knockbackSlideZ = 0.0f;
        if (!moving)
            actor.reactionTimer = 0.0f;
    } else {
        const float factor = 1.0f - slideDrag / slideSpeed;
        actor.knockbackSlideX *= factor;
        actor.knockbackSlideZ *= factor;
    }
    if ((actor.moveFlags & 4U) != 0) {
        actor.moveVelocity.y -= strength * game->knockbackGravityScale;
        actor.moveVelocity.y += actor.knockbackAccelerationBias;
    }

    const Vector3 previous = actor.position;
    actor.position.x = (actor.moveVelocity.x + actor.position.x) + actor.knockbackSlideX;
    actor.position.y += actor.moveVelocity.y;
    actor.position.z = (actor.moveVelocity.z + actor.position.z) + actor.knockbackSlideZ;
    actor.knockbackFrameDelta = {actor.position.x - previous.x, actor.position.y - previous.y, actor.position.z - previous.z};
}

void vfclResolveContactPoseConstraints(MotionInformationWork *motion, void *user);


void UpdateVFAutoAdvanceLanding(VFActor &actor) {
    VFMAData *const game = actor.game;
    if (!game || (game->gameFlags & 0x20U) != 0 || (actor.moveFlags & 0x80U) == 0) {
        return;
    }
    actor.moveFlags &= ~0x2000U;
    if ((actor.moveFlags & 4U) != 0) {
        if ((actor.moveFlags & 8U) == 0 &&
            actor.groundY + actor.knockbackReactionTime >
                (actor.moveVelocity.y - actor.knockbackStrength * actor.knockbackStrength * game->knockbackGravityScale) + actor.position.y) {
            actor.moveFlags |= 0x2000U;
        }
        return;
    }
    if (actor.groundY != actor.position.y)
        actor.position.y = actor.groundY;
    MotionInformationWork &motion = *static_cast<MotionInformationWork *>(actor.motion);
    motion.mobjCallbackUser = &actor;
    motion.mobjCallback = reinterpret_cast<void *>(vfclResolveContactPoseConstraints);
    if ((actor.hitFlags & 0x10U) != 0) {
        if (actor.currentAnimationEndFrame < actor.fallLandingWindowStartFrame || actor.currentAnimationEndFrame >= actor.knockbackAnimationEndFrame) {
            actor.hitStateFlags2 &= ~0x10U;
        } else {
            actor.hitStateFlags2 |= 0x10U;
        }
    }
}



void vfUpdateMotionTargetPointAndParams(VFActor &actor) {
    auto *const motion = static_cast<MotionInformationWork *>(actor.motion);
    const VFActor *const target = actor.targetActor;
    if (!motion || !target || !actor.game || actor.targetSlotIndex < 0 || static_cast<std::size_t>(actor.targetSlotIndex) >= kBattleSlots)
        return;
    const std::uint32_t state = actor.hitStateFlags2;
    const std::uint32_t flags = actor.motionFlags;
    if ((ctrlGetNeedsCameraDamping() && actor.slotIndex == static_cast<std::uint8_t>(actor.game->playerSlot)) || target->fightStyleIndex == 0x15U ||
        (actor.moveFlags & 0x800000U) != 0 || actor.contactPoints[static_cast<std::size_t>(actor.targetSlotIndex)] > 4.0f || (flags & 0xc00U) != 0 ||
        (state & 0x000fc010U) != 0) {
        motion->headLookAngleCommand = 2048;
        return;
    }
    if ((state & 0x101U) != 0 && (flags & 0x210000U) == 0) {
        motion->headLookAngleCommand = 16384;
        return;
    }
    motion->headLookAngleCommand = 2048;
    if ((actor.fightStyleIndex == 6U || actor.fightStyleIndex == 0x1bU) && (flags & 0x08000000U) != 0)
        return;
    Vector3 position{};
    if (Character *const character = actor.targetingCharacter) {
        position = GetPositionForEntity(character->entity.node);
        const MotionManagerWork *const manager = MOTM_GetWork(character);
        const auto *node = manager ? static_cast<const MotionNodeRuntime *>(manager->motion.previousNodes) : nullptr;
        for (; node && node->nodeId != 0xffU; ++node) {
            if (node->nodeId == 47U && node->transform) {
                position = {node->transform->rows[3].x, node->transform->rows[3].y, node->transform->rows[3].z};
                break;
            }
        }
    }
    MOTM_SetTarget(actor.character, 6, &position, 0x40000000U);
}

void vfUpdateMotionStateCategory(VFActor &actor) {
    auto *const motion = static_cast<MotionInformationWork *>(actor.motion);
    if (!motion)
        return;
    std::uint16_t blendFrames = 8;
    if ((actor.hitStateFlags2 & 0x00010000U) != 0) {
        blendFrames = 5;
    } else if ((actor.hitStateFlags2 & 0x00024000U) != 0 || (actor.motionFlags & 0x00200000U) != 0) {
        blendFrames = 1;
    } else if ((actor.hitStateFlags2 & 0x10000000U) != 0 || (actor.motionFlags & 0x02000000U) != 0) {
        blendFrames = 2;
    } else if ((actor.hitStateFlags2 & 0x00000100U) != 0) {
        blendFrames = static_cast<std::uint16_t>(actor.attackWindowEndFrame - actor.currentAnimationEndFrame);
    }
    if (actor.animationScale != 1.0f && blendFrames != 1) {
        blendFrames = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(blendFrames) * actor.animationScale + 0.5f));
    }
    blendFrames = std::clamp<std::uint16_t>(blendFrames, 1, 8);
    if ((actor.game && (actor.game->gameFlags & 0x01000000U) != 0) || (actor.hitStateFlags & 0x00000004U) != 0) {
        blendFrames = 1;
    }
    motion->blendFrameCount = blendFrames;
}

void vfUpdateReactAnimationFrames(VFActor &actor) {
    if (!actor.game || ((actor.hitFlags & 0x00040100U) == 0 && (actor.hitStateFlags & 0x20000000U) == 0)) {
        return;
    }
    if ((((actor.hitStateFlags2 ^ actor.previousHitStateFlags2) & 4U) == 0) && (((actor.motionFlags ^ actor.previousMotionFlags) & 0x20000000U) == 0)) {
        return;
    }

    VFMAData &game = *actor.game;
    std::size_t actorCount = 0;
    for (Character *const character : game.characters) {
        if (character)
            ++actorCount;
    }
    const std::uint16_t animationId = actor.currentReactionAnimation & 0x0fffU;
    if (actorCount == 2 && actor.targetActor && (((actor.targetActor->hitStateFlags2 & 0x0009c000U) != 0) || (actor.hitStateFlags2 & 0x00040000U) != 0)) {
        const std::uint8_t *const cue = shenLookupAnimationIndex(animationId, 2);
        if (cue) {
            std::uint16_t cueEnd = 0;
            std::memcpy(&cueEnd, cue + 6, sizeof(cueEnd));
            actor.reactionEndFrame = static_cast<std::uint16_t>(static_cast<float>(cueEnd) * actor.animationScale + 0.5f);
        }
        const std::int32_t frameCount = MOTN_GetFrameCountFromAnimId(animationId);
        actor.reactionAnimationEndFrame = static_cast<std::int16_t>(static_cast<float>(std::max(frameCount, 0)) * actor.animationScale + 0.5f);
        return;
    }

    const std::uint16_t stateEnd = actor.reactionState02EndFrame;
    const std::uint16_t reactionMid = actor.reactionMidFrame;
    const std::uint8_t *const cue = shenLookupAnimationIndex(animationId, 2);
    std::uint16_t reactionEnd = 0;
    if (cue)
        std::memcpy(&reactionEnd, cue + 6, sizeof(reactionEnd));
    if (stateEnd == 0 || stateEnd < reactionMid || stateEnd >= reactionEnd) {
        return;
    }

    float nearest = 100.0f;
    for (std::size_t index = 0; index < kBattleSlots; ++index) {
        VFActor *const other = game.actors[index];
        if (!game.characters[index] || !other || index == static_cast<std::uint8_t>(game.playerSlot) || other->currentHp <= 0 ||
            (other->hitFlags & 0x00080000U) != 0) {
            continue;
        }
        nearest = std::min(nearest, actor.contactPoints[index]);
    }
    if (nearest <= 3.0f) {
        reactionEnd = stateEnd;
    } else if (nearest < 8.0f) {
        reactionEnd = static_cast<std::uint16_t>(static_cast<float>(stateEnd) + (static_cast<float>(reactionEnd - stateEnd) / 5.0f) * (nearest - 3.0f) + 0.5f);
    }
    reactionEnd = std::max<std::uint16_t>(reactionEnd, static_cast<std::uint16_t>(reactionMid + 3U));
    if (actor.currentAnimationEndFrame <= actor.reactionEndFrame && actor.currentAnimationEndFrame > reactionEnd) {
        reactionEnd = actor.currentAnimationEndFrame;
    }
    actor.reactionEndFrame = reactionEnd;
    actor.reactionAnimationEndFrame = static_cast<std::int16_t>(reactionEnd);

    if (actor.currentAnimationEndFrame >= reactionEnd) {
        std::uint16_t markerFrame = 0;
        if (MOTN_FindFrameMarker(animationId, 1.0f, static_cast<float>(reactionEnd), markerFrame) &&
            static_cast<std::uint16_t>(static_cast<float>(markerFrame) * actor.animationScale + 0.5f) > reactionEnd) {
            actor.hitStateFlags2 ^= 0x00200000U;
        }
        if (FightPoseWork *const figp = FIGP_GetWork(actor.character)) {
            figp->active = 0;
        }
    }
    if (actor.currentAnimationEndFrame > static_cast<std::uint16_t>(actor.reactionAnimationEndFrame)) {
        actor.pendingAnimationId = static_cast<std::uint16_t>(actor.defaultReactionAnimationId);
        (void)ApplyPendingHitReactionMotion(actor);
    }
}

void vfUpdateAnimFlagsAndEvents(VFActor &actor) {
    auto *const motion = static_cast<MotionInformationWork *>(actor.motion);
    if (!motion)
        return;

    // VF owns the reaction frame counter. MOTI's frameTimer is driven from
    // it below; feeding frameTimer back here applies playback scaling twice.
    if (actor.currentReactionAnimation == 0 || actor.currentAnimationEndFrame > static_cast<std::uint16_t>(actor.reactionAnimationEndFrame)) {
        const std::uint16_t next = static_cast<std::uint16_t>(actor.postReactionAnimationId);
        if (actor.currentReactionAnimation != 0 && next == 0xffffU) {
            actor.reactionProgram = actor.reactionProgramStart;
            actor.reactionProgramCounter = actor.reactionProgram;
            actor.savedReactionStateFlags = actor.hitStateFlags2;
            actor.previousAnimationEndFrame = 1;
            actor.currentAnimationEndFrame = 1;
        } else {
            actor.pendingAnimationId = next != 0 ? next : static_cast<std::uint16_t>(actor.defaultReactionAnimationId);
            (void)ApplyPendingHitReactionMotion(actor);
        }
    }
    if (actor.currentAnimationEndFrame == 0)
        actor.currentAnimationEndFrame = 1;
    if (actor.previousReactionAnimationId != 0xffffU || actor.previousAnimationEndFrame != 0xffffU) {
        if (actor.previousReactionAnimationId == 0xffffU)
            actor.previousReactionAnimationId = actor.currentReactionAnimation;
        if (actor.previousAnimationEndFrame == 0xffffU)
            actor.previousAnimationEndFrame = 1;
        vfUpdateMotionStateCategory(actor);
        motion->blendMode = 1;
        if ((actor.moveFlags & 0x20000U) != 0) {
            actor.moveFlags &= ~0x20000U;
            actor.pendingAnimationId = motion->currentAnimId;
            (void)vfclInitReactionProgramAndRunHandlers(actor, (actor.moveFlags & 0x40U) != 0);
        } else {
            motion->pendingAnimId = actor.previousReactionAnimationId & 0x0fffU;
        }
    }
    if ((actor.moveFlags & 0x40U) != 0)
        motion->motionFlags |= 0x80U;
    else
        motion->motionFlags &= ~0x80U;
    (void)motCalcDeltaXZFromAnimTime(*motion, (static_cast<float>(actor.currentAnimationEndFrame) - 1.0f) * actor.knockbackStrength, actor.moveDelta);
    UpdateAttackWindowForFrame(actor);
    std::uint8_t animationFlags = 0;
    if ((actor.hitFlags & 0x20000400U) == 0x400U) {
        animationFlags = 2;
        if ((actor.hitFlags & 2U) != 0 || ((actor.savedReactionStateFlags & 0x400U) == 0 && (actor.hitFlags & 0x400U) != 0))
            animationFlags = 3;
        else if ((actor.previousMoveAnimationFlags & 0x78U) != (actor.moveAnimationFlags & 0x78U))
            animationFlags = 0x82;
    } else if ((actor.temporaryHitFlags & 0x400U) != 0) {
        animationFlags = 4;
    }
    actor.moveAnimationFlags = animationFlags | (animationFlags != 0 ? actor.moveAnimationFlags & 0x78U : 0);
    if ((actor.hitStateFlags2 & 0x80100U) == 0x80100U)
        actor.motionFlags |= 0x80000000U;
    if (((actor.priorityFlags2 & 0x30fU) != 0 || (actor.cameraControlFlags & 0x4000U) != 0) && (actor.motionFlags & 0x10000U) != 0) {
        actor.motionFlags &= ~0x10000U;
        actor.hitStateFlags2 &= ~1U;
    }
    vfUpdateReactAnimationFrames(actor);
    actor.reactionProgramCounter = actor.reactionProgram;
    ConsumeVFFrameEvents(actor);
}


void vfResolveKnockbackLanding(VFActor &actor) {
    if ((actor.moveFlags & 4U) == 0)
        return;
    if ((actor.skipAlignment & 1) == 0) {
        const auto yaw = static_cast<std::uint16_t>(-actor.facingAngle);
        const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
        const float cosine = sin_fixed16(fixed16_cosine_angle(yaw));
        actor.position.x -= cosine * actor.moveDelta.x - sine * actor.moveDelta.z;
        actor.position.z -= cosine * actor.moveDelta.z + sine * actor.moveDelta.x;
    }
    actor.moveDelta.y = actor.contactCurrentPosition.y - actor.position.y;
    actor.hitStateFlags2 &= ~0x10U;
    actor.moveFlags &= ~4U;
    if (auto *const motion = static_cast<MotionInformationWork *>(actor.motion))
        motion->motionFlags &= ~0x400U;
}


void vfUpdateAirborneAnimationTiming(VFActor &actor) {
    if ((actor.moveFlags & 0x2000U) != 0) {
        if ((actor.moveFlags & 4U) != 0) {
            actor.moveVelocity.y = 0.0f;
            actor.position.y = actor.groundY;
            if (actor.groundY > actor.minimumGroundY) {
                actor.moveVelocity.x = 0.0f;
                actor.moveVelocity.z = 0.0f;
            }
            vfResolveKnockbackLanding(actor);
        }
        if (actor.knockbackAnimationEndFrame != 0 && actor.knockbackAnimationEndFrame > actor.currentAnimationEndFrame && (actor.motionFlags & 1U) == 0) {
            actor.previousAnimationEndFrame = actor.knockbackAnimationEndFrame;
            actor.currentAnimationEndFrame = actor.knockbackAnimationEndFrame;
        }
    } else if ((actor.moveFlags & 4U) != 0) {
        if (actor.moveVelocity.y < 0.0f)
            actor.moveFlags &= ~8U;
        if (actor.currentAnimationEndFrame > actor.knockbackAnimationStartParameter) {
            const auto remaining = static_cast<std::uint16_t>(actor.knockbackAnimationEndFrame - actor.knockbackFramesRemaining + 1U);
            if (remaining == 0 || actor.currentAnimationEndFrame > remaining) {
                --actor.currentAnimationEndFrame;
                actor.previousAnimationEndFrame = actor.currentAnimationEndFrame;
            }
        }
    }
}
} // namespace vf_detail

} // namespace shenmue::s1
