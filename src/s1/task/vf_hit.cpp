#include "s1/task/vf_internal.hpp"

namespace shenmue::s1::vf_detail {

float VFCL_CalculateGuardKnockbackFactor(std::uint8_t fighterId) {
    static constexpr std::array<std::uint8_t, 32> kBank1Groups{{
        0,
        1,
        1,
        1,
        1,
        1,
        1,
        2,
        1,
        1,
        2,
        1,
        1,
        2,
        2,
        2,
        2,
        2,
        2,
        0,
        1,
        1,
        2,
        2,
        0,
        0,
        1,
        2,
        2,
        2,
        1,
        0,
    }};
    static constexpr std::array<std::uint8_t, 32> kBank2Groups{{
        1,
        1,
        2,
        2,
        0,
        0,
        1,
        2,
        2,
        2,
        1,
        0,
        2,
        2,
        2,
        2,
        0,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        32,
        0,
        64,
        128,
    }};
    static constexpr std::array<std::uint8_t, 16> kBank3Groups{};
    static constexpr std::array<std::array<float, 2>, 3> kCurves{{
        {{0.0f, 0.0f}},
        {{80.0f, 30.0f}},
        {{50.0f, 20.0f}},
    }};

    std::uint8_t group = 0;
    if (fighterId < 0x20U) {
        group = kBank1Groups[fighterId];
    } else if (fighterId < 0x40U) {
        group = kBank2Groups[fighterId - 0x20U];
    } else if (fighterId < 0x50U) {
        group = kBank3Groups[fighterId - 0x40U];
    } else {
        return 0.0f;
    }
    if (group >= kCurves.size())
        return 0.0f;

    const float threshold = kCurves[group][0];
    const float lowValue = kCurves[group][1];
    if (threshold == 0.0f)
        return 0.0f;
    const float value = static_cast<float>(VF_GetAnimSetValue(fighterId));
    if (value < threshold)
        return lowValue * value / threshold;
    if (value < 100.0f)
        return ((60.0f - lowValue) / (100.0f - threshold)) * (value - threshold) + lowValue;
    return 60.0f;
}

} // namespace shenmue::s1::vf_detail

namespace shenmue::s1 {

using namespace vf_detail;

bool VFCL_IsIgnoredGuardAnimation(const VFActor &actor) {
    const std::uint16_t animation = static_cast<std::uint16_t>(actor.pendingAnimationId & 0x0fffU);
    switch (animation) {
    case 868:
    case 894:
    case 1142:
    case 1213:
    case 1329:
    case 1431:
    case 2261:
    case 2306:
    case 2442:
    case 2444:
    case 2445:
    case 2447:
    case 2474:
    case 2480:
        return true;
    case 2094:
        return actor.fightStyleIndex == 6U || actor.fightStyleIndex == 27U;
    default:
        return false;
    }
}

float VFCL_CalculateHitImpulse(VFActor &attacker) {
    const std::size_t type = attacker.hitGuardType < kVFKnockbackInfo.size() ? attacker.hitGuardType : 0U;
    const VFKnockbackInfo &info = kVFKnockbackInfo[type];
    attacker.hitPowerScale = info.baseScale;
    const float factor = VFCL_CalculateGuardKnockbackFactor(attacker.fighterRankId);
    const float acceleration = std::sqrt(static_cast<float>(static_cast<double>(factor + static_cast<float>(attacker.guardStrength)) * 0.03333333));
    return static_cast<float>(static_cast<double>(acceleration) * 30.0 * 0.0025) * info.baseScale;
}

void VFCL_AssignKnockbackParameters(
    VFActor &attacker, VFActor &defender, std::uint32_t reaction, std::uint16_t reactionAnimation, float hitImpulse, float impact) {
    if (reaction <= 2U) {
        const bool airborneVariant =
            reaction == 0U || (((attacker.hitStateFlags2 & 0x00400000U) != 0) && (defender.hitStateFlags2 & 0x00000810U) == 0x00000800U);
        const std::uint8_t stun = impact > 14.0f ? static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<double>(impact) - 14.0 + 0.5)) : 1U;
        if (airborneVariant) {
            attacker.hitStunFrames = stun;
            defender.pairedFighter = &attacker;
            vfHandleImpactSfxAndDefeat(defender, static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(impact) + 0.5)), 1);
            attacker.suppressKnockback = 1;
            return;
        }
        defender.field8f4 |= 2U;
        defender.hitStunFrames = stun;

        const std::uint8_t *const metadata = shenLookupAnimationIndex(reactionAnimation & 0x0fffU, 17);
        if (metadata) {
            std::uint16_t angle = 0;
            std::memcpy(&angle, metadata + 2, sizeof(angle));
            const std::uint32_t packedScale = (static_cast<std::uint32_t>(metadata[4]) << 24U) | (static_cast<std::uint32_t>(metadata[5]) << 16U) |
                (static_cast<std::uint32_t>(metadata[6]) << 8U) | static_cast<std::uint32_t>(metadata[7]);
            attacker.knockbackAngle = angle;
            attacker.hitImpulseMagnitude = static_cast<float>(std::bit_cast<std::int32_t>(packedScale)) * 0.000015258789f * hitImpulse;
            return;
        }

        const std::size_t type = attacker.hitGuardType < kVFKnockbackInfo.size() ? attacker.hitGuardType : 0U;
        const VFKnockbackInfo &info = kVFKnockbackInfo[type];
        const bool counter = attacker.hitOutcome == 6;
        const bool falling = (defender.hitStateFlags2 & 0x00000010U) != 0;
        float scale = 0.0f;
        std::uint16_t angle = 0;
        if (falling) {
            scale = counter ? info.counterFallScale : info.fallScale;
            angle = counter ? info.counterFallAngle : info.fallAngle;
        } else {
            scale = counter ? info.counterGroundScale : info.groundScale;
            angle = counter ? info.counterGroundAngle : info.groundAngle;
        }
        attacker.temporaryKnockback = static_cast<float>(static_cast<std::int16_t>(angle));
        attacker.hitPowerScale = scale;
        attacker.hitImpulseMagnitude = hitImpulse * scale;
        const std::uint8_t phase = static_cast<std::uint8_t>(defender.knockbackPhase);
        if (phase > 1U) {
            std::int16_t adjusted = static_cast<std::int16_t>(angle - 1408 * phase + 1408);
            const std::int16_t minimum = static_cast<std::int32_t>(static_cast<std::int16_t>(angle)) > 4096 ? 4096 : 0;
            if (adjusted < minimum)
                adjusted = minimum;
            if (adjusted > 13824)
                adjusted = 13824;
            angle = static_cast<std::uint16_t>(adjusted);
        }
        attacker.knockbackAngle = angle;
        return;
    }
    if (reaction <= 4U) {
        if (reaction == 3U && attacker.game) {
            float height = static_cast<float>(attacker.hitHeightClass);
            if (attacker.hitHeightClass == 6)
                height = static_cast<float>(attacker.hitHeightClass2);
            if (attacker.game->reactionTypeIndex == 0)
                vfSelectHitReactionSequence(defender, 0);
            if (height == 0.0f)
                height = static_cast<float>(static_cast<double>(impact) * 0.8 + 6.0);
            const std::uint8_t stun = static_cast<std::uint8_t>(static_cast<std::int32_t>(height * 0.5f + 0.5f));
            defender.hitStunFrames = stun;
            std::uint32_t hitStopBase = static_cast<std::uint32_t>(stun) + 8U;
            if (attacker.hitOutcome == 37 || attacker.hitOutcome == 6)
                hitStopBase = static_cast<std::uint32_t>(stun) - 2U;
            const float hitStop = static_cast<float>(static_cast<std::int32_t>((hitStopBase >> 1U) + 1U)) * attacker.knockbackStrength;
            attacker.game->hitStopFrames = static_cast<std::uint8_t>(hitStop);
        }
        attacker.hitImpulseMagnitude = hitImpulse;
        attacker.knockbackAngle = 0;
        return;
    }
    attacker.knockbackAngle = 0;
    attacker.hitImpulseMagnitude = 0.0f;
}

void VFCL_PublishPendingKnockback(const VFActor &attacker, VFActor &defender, std::uint16_t facingAngle) {
    defender.pendingKnockback = {};
    if (attacker.suppressKnockback || !defender.game)
        return;
    const float denominator = static_cast<float>((static_cast<double>(defender.knockbackBaseParameter) - 60.0) * 0.6 + 60.0);
    const float magnitude = std::sqrt((attacker.hitImpulseMagnitude * 5.0f) / denominator);
    const float horizontal = sin_fixed16(fixed16_cosine_angle(attacker.knockbackAngle)) * magnitude;
    const std::uint16_t inverseFacing = static_cast<std::uint16_t>(0U - facingAngle);
    const float scale = defender.game->knockbackAmount;
    defender.pendingKnockback.y = sin_fixed16(attacker.knockbackAngle) * magnitude * scale;
    defender.pendingKnockback.x = sin_fixed16(inverseFacing) * horizontal * scale;
    defender.pendingKnockback.z = -sin_fixed16(fixed16_cosine_angle(inverseFacing)) * horizontal * scale;
}


void vfAssignKnockbackReaction(float impact, float hitImpulse, VFActor &attacker, VFActor &defender) {
    std::uint32_t nextReaction = 3;
    const MotnAnimationHeader *const knockbackAnimation = attacker.knockbackAnimationId != 0 ? MOTN_GetDataForAnimId(attacker.knockbackAnimationId) : nullptr;
    if ((defender.motionFlags & 0x00000800U) != 0)
        nextReaction = 5;
    else if ((defender.hitStateFlags2 & 0x00010000U) != 0 && (defender.moveFlags & 0x20000000U) == 0)
        nextReaction = 0;
    else if (knockbackAnimation && (knockbackAnimation->attribute & 0x4000U) != 0)
        nextReaction = 2;
    else if ((defender.hitStateFlags2 & 0x00004000U) != 0)
        nextReaction = 2U - ((attacker.hitStateFlags2 & 0x00400000U) != 0 ? 1U : 0U);
    else if ((defender.hitStateFlags2 & 0x00001010U) != 0 || (attacker.hitParameter & 0x00004000U) != 0)
        nextReaction = 2;
    else if ((attacker.hitParameter & 0x00000400U) != 0)
        nextReaction = 3;
    else if ((attacker.hitParameter & 0x00000100U) != 0 && (defender.hitStateFlags2 & 0x00000008U) != 0)
        nextReaction = 2;
    else if (attacker.guardState == 1U)
        nextReaction = (defender.hitStateFlags2 & 0x01000000U) != 0 ? 2U : 3U;
    if ((defender.motionFlags & 0x01000000U) != 0)
        nextReaction = 2;
    if (nextReaction == 2U) {
        if ((defender.stateFlags & 2U) != 0 && (defender.moveFlags & 0x20000000U) == 0 && (defender.hitStateFlags2 & 0x0001c000U) == 0 &&
            (attacker.hitParameter & 0x0200U) == 0) {
            nextReaction = 3;
        } else if (attacker.fightStyleIndex == 0x14U || defender.fightStyleIndex == 0x14U) {
            nextReaction = 3;
        }
    }
    if (nextReaction >= 2U) {
        VFMAData &game = *attacker.game;
        std::int16_t lethalDamage = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(impact) + 0.5));
        if (defender.slotIndex == static_cast<std::uint8_t>(game.playerSlot)) {
            lethalDamage = static_cast<std::int16_t>((1.0 - static_cast<double>(game.playerAssistScale)) * lethalDamage + 0.5);
            if (lethalDamage < 1)
                lethalDamage = 1;
        }
        const std::int8_t selectFlags = static_cast<std::int8_t>(game.battleMeta.waves[static_cast<std::int8_t>(defender.slotIndex)]->selectFlags);
        if ((selectFlags < 0 && (attacker.hitStateFlags & 0x04000008U) != 0) || ((selectFlags & 0x20) != 0 && (attacker.hitStateFlags & 0x04000000U) != 0))
            lethalDamage = defender.currentHp;
        if (defender.currentHp <= lethalDamage)
            nextReaction = 4;
    }
    if ((defender.motionFlags & 0x00800000U) != 0) {
        const std::uint8_t guard = attacker.guardState;
        if (nextReaction == 3U && (guard == 0U || guard == 1U || guard == 3U))
            vfStageObjectApplyVelocity(defender);
        else if (nextReaction != 3U || guard == 2U || guard == 5U || guard == 6U)
            ResetStageObjectMotion(*g_vfStageObjs[defender.stageHitObjectIndex]);
    }
    if (nextReaction == 5U) {
        constexpr std::array<std::uint16_t, 3> kAdvancedGuardAnimations{1080U, 1079U, 1078U};
        std::uint8_t guardType = 0xffU;
        if (attacker.guardState == 0U)
            guardType = 0;
        else if (attacker.guardState == 1U || attacker.guardState == 3U)
            guardType = 1;
        else if (attacker.guardState == 2U || attacker.guardState == 5U || attacker.guardState == 6U)
            guardType = 2;
        if (guardType >= kAdvancedGuardAnimations.size())
            return;
        VFCL_SetAction(defender.action, kAdvancedGuardAnimations[guardType], 16U);
        attacker.knockbackAngle = 0;
        attacker.hitImpulseMagnitude = 0.0f;
        vfHandleImpactSfxAndDefeat(defender, static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(impact) + 0.5)), 0);
        return;
    }
    std::uint16_t reactionAnimation = 0;
    if (nextReaction <= 2U) {
        if (nextReaction == 1U)
            defender.knockbackPhase = 1;
        const bool airborneVariant =
            nextReaction == 0U || ((attacker.hitStateFlags2 & 0x00400000U) != 0 && (defender.hitStateFlags2 & 0x00000810U) == 0x00000800U);
        reactionAnimation = VFCL_SelectReactionAnimation(attacker, defender, airborneVariant ? 9U : 7U);
        if (!airborneVariant) {
            vfAssignJumpIfNeeded(attacker, defender);
            reactionAnimation = static_cast<std::uint16_t>(g_vfHitActionContext);
        }
        VFCL_SetAction(defender.action, reactionAnimation, 12U);
        defender.action.resolve = airborneVariant ? 1U : 0U;
    } else {
        std::uint32_t selector = nextReaction == 4U ? 8U : static_cast<std::uint32_t>(attacker.hitOutcome) & 31U;
        if (nextReaction == 3U && static_cast<std::int8_t>(defender.guardLevel - defender.targetActor->guardLevel) > 10)
            selector = 4U;
        reactionAnimation = VFCL_SelectReactionAnimation(attacker, defender, selector);
        VFCL_SetAction(defender.action, reactionAnimation, nextReaction == 4U ? 12U : 15U);
    }
    VFCL_AssignKnockbackParameters(attacker, defender, nextReaction, reactionAnimation, hitImpulse, impact);
    if (nextReaction == 4U && (defender.motionFlags & 0x00000800U) != 0)
        VFCL_SetAction(defender.hitSourceActor->action, 0U, 1U);
}


void VFCL_ApplyImpact(VFActor &attacker, VFActor &defender) {
    enum class HitResolution : std::uint8_t {
        Ignored,
        Normal,
        HeavyGuard,
        LightGuard,
        ForcedReaction,
        SpecialCounter,
    };

    ++attacker.activeContactCount;
    const std::uint16_t activeContactCount = attacker.activeContactCount;
    const std::uint8_t queuedState = attacker.queuedAction.state;
    if (queuedState == 22U)
        return;

    const bool blockedByState = queuedState == 33U || (attacker.guardState == 0U && (defender.motionFlags & 0x02000000U) != 0);
    bool blockedByGuardWindow = false;
    if ((defender.hitStateFlags2 & 0x00018000U) != 0 && (defender.motionFlags & 0x00000002U) == 0) {
        if ((defender.motionFlags & 0x10000000U) != 0)
            blockedByGuardWindow = defender.hitPhase <= 4U;
        else
            blockedByGuardWindow = (defender.hitFlags & 0x00000100U) == 0 || (defender.hitStateFlags2 & 0x00000002U) != 0;
        blockedByGuardWindow = blockedByGuardWindow && attacker.guardState != 4U;
    }
    if (blockedByState || blockedByGuardWindow) {
        attacker.activeContactCount = static_cast<std::uint16_t>(activeContactCount - 1U);
        return;
    }

    const std::uint16_t facingAngle = static_cast<std::uint16_t>(attacker.facingAngle + attacker.hitDirectionAngle);
    float guardStrength = static_cast<float>(attacker.guardStrength);
    float hitImpulse = VFCL_CalculateHitImpulse(attacker);
    if ((attacker.hitStateFlags2 & 0x00001010U) == 0x00001010U && static_cast<std::int16_t>(attacker.moveFacingLimit - facingAngle + 0x4000U) >= 0) {
        attacker.moveVelocity.x *= 0.5f;
        attacker.moveVelocity.z *= 0.5f;
    }

    const std::uint8_t slot = static_cast<std::uint8_t>(attacker.slotIndex) & 7U;
    const std::int16_t contactAngleDifference =
        static_cast<std::int16_t>(std::abs(static_cast<std::int32_t>(static_cast<std::int16_t>(defender.contactAngles[slot] - defender.temporaryFacingAngle))));
    HitResolution resolution = HitResolution::Normal;
    if ((defender.motionFlags & 0x00004000U) != 0 && contactAngleDifference < 0x2000) {
        if ((attacker.hitStateFlags2 & 0x00081010U) == 0 || (attacker.stateFlags2 & 0x00400000U) != 0 || VFCL_IsIgnoredGuardAnimation(attacker)) {
            resolution = HitResolution::Ignored;
        }
    } else if ((defender.hitStateFlags2 & 0x00002000U) != 0 && contactAngleDifference < 0x2000) {
        const bool counterWindow = (defender.hitStateFlags2 & 0x00000008U) != 0;
        const bool evading = attacker.guardState == 2U || attacker.guardState == 5U || attacker.guardState == 6U;
        if (counterWindow && (attacker.stateFlags2 & 0x00000002U) != 0) {
            resolution = HitResolution::SpecialCounter;
        } else if (counterWindow && !evading) {
            if ((attacker.hitParameter & 0x00002000U) != 0)
                resolution = HitResolution::ForcedReaction;
            else if ((attacker.hitParameter & 0x00001000U) != 0)
                resolution = HitResolution::HeavyGuard;
            else
                resolution = attacker.guardState == 1U ? HitResolution::LightGuard : HitResolution::Normal;
        } else if ((attacker.hitParameter & 0x00000200U) == 0 && (counterWindow || (!evading && (attacker.motionFlags & 0x80000000U) == 0))) {
            resolution = HitResolution::Ignored;
        } else {
            resolution = (attacker.hitParameter & 0x00001000U) != 0 ? HitResolution::HeavyGuard : HitResolution::Normal;
        }
    }
    if ((attacker.motionFlags & 0x00020000U) != 0)
        resolution = HitResolution::Normal;

    bool specialReaction = false;
    switch (resolution) {
    case HitResolution::Ignored:
        g_vfHitActionContext = 0;
        ++attacker.ignoredHitCount;
        attacker.activeContactCount = static_cast<std::uint16_t>(activeContactCount - 1U);
        specialReaction = true;
        break;
    case HitResolution::Normal:
        attacker.hitOutcome = 5;
        break;
    case HitResolution::HeavyGuard:
        attacker.hitOutcome = 3;
        guardStrength *= 0.5f;
        hitImpulse *= 0.5f;
        break;
    case HitResolution::LightGuard:
        guardStrength *= 0.75f;
        hitImpulse *= 0.75f;
        attacker.hitOutcome = 5;
        break;
    case HitResolution::ForcedReaction:
        g_vfHitActionContext = 3;
        specialReaction = true;
        break;
    case HitResolution::SpecialCounter:
        (void)VFCL_PublishSpecialCounter(attacker, defender);
        return;
    }

    attacker.hitImpulseMagnitude = hitImpulse;
    attacker.suppressKnockback = 0;
    defender.pairedFighter = &attacker;
    float impact = 0.0f;
    if (specialReaction) {
        const bool armed = (defender.stateFlags & 0x0000000cU) != 0;
        const std::uint32_t selector = armed ? 2U : (resolution == HitResolution::ForcedReaction ? 3U : 0U);
        std::uint16_t reactionAnimation = 0;
        if (armed) {
            attacker.hitOutcome = 3;
            reactionAnimation = VFCL_SelectReactionAnimation(attacker, defender, selector);
            VFCL_SetAction(defender.action, reactionAnimation, 42U);
            attacker.slots.primary = attacker.selectedReactionSlot;
            attacker.slots.secondary = attacker.selectedReactionSlot;
            attacker.reactionResumeProgramCounter = nullptr;
            attacker.delayedReactionTimer = 0;
        } else {
            attacker.hitOutcome = 0;
            VFCL_SetAction(attacker.nextAction, 2U, 20U);
            reactionAnimation = VFCL_SelectReactionAnimation(attacker, defender, selector);
            VFCL_SetAction(defender.action, reactionAnimation, 14U);
        }
        defender.lastProcessedHitMoveId = attacker.lastHitMoveId;
        if ((attacker.rootReactionModeFlags & 2) != 0) {
            attacker.rootReactionVmProgramCounter = attacker.alternateRootReactionProgram;
            attacker.rootReactionDelayFrames = attacker.alternateRootReactionDelayFrames;
        }
        if ((defender.triggerMask & 0x00000020U) != 0)
            defender.triggerState |= 0x00000020U;

        std::uint8_t hitStun = static_cast<std::uint8_t>(attacker.hitStunOverride);
        if (hitStun == 0) {
            hitStun = static_cast<std::uint8_t>(static_cast<float>((static_cast<double>(guardStrength) * 0.6 + 6.0) * 0.5) + 0.5f);
            if (hitStun > 22U)
                hitStun = 22U;
        }
        if (defender.game && defender.game->reactionTypeIndex == 0)
            vfSelectHitReactionSequence(defender, 1);
        if (defender.targetActor) {
            const std::int8_t guardDifference = static_cast<std::int8_t>(defender.guardLevel - defender.targetActor->guardLevel);
            if (guardDifference > 0)
                hitStun = static_cast<std::uint8_t>(static_cast<std::int8_t>(hitStun) <= guardDifference ? 1U : hitStun - guardDifference);
        }
        defender.hitStunFrames = hitStun;
        if (defender.game) {
            defender.game->hitStopFrames =
                static_cast<std::uint8_t>(static_cast<float>((static_cast<std::uint8_t>(hitStun + 8U) >> 1U)) * defender.knockbackStrength);
        }
        attacker.knockbackAngle = 0;
    } else {
        if ((attacker.hitFlags & 0x00080000U) != 0)
            defender.motionFlags |= 0x01000000U;
        VFCL_SetAction(attacker.nextAction, 1U, 20U);
        if ((attacker.rootReactionModeFlags & 1) != 0) {
            attacker.rootReactionVmProgramCounter = attacker.alternateRootReactionProgram;
            attacker.rootReactionDelayFrames = attacker.alternateRootReactionDelayFrames;
        }
        if (attacker.fightStyleIndex == 0U && attacker.game) {
            const std::uint16_t fighterId = attacker.fighterRankId == 87U ? attacker.fighterIdAlt : attacker.fighterRankId;
            vfmaUpdateCollisionCounter(*defender.game, VFCollisionCounterEvent::HitConfirmed, fighterId);
            attacker.fighterRankId = 80U;
        }

        impact = VFCL_ResolveImpact(attacker, defender, guardStrength * VF_GetLocalActorAnimScale(attacker));
        VFCL_PlayImmediateHitCue(impact, attacker);
        vfAssignKnockbackReaction(impact, hitImpulse, attacker, defender);
        const std::uint16_t defenderActionAnimation =
            static_cast<std::uint16_t>(defender.action.opcode | (static_cast<std::uint16_t>(defender.action.index) << 8U));
        if (defender.fightStyleIndex == 0U && (defenderActionAnimation & 0x0fffU) == 0x038aU)
            defender.action = std::bit_cast<VFActionDescriptor>((std::bit_cast<std::uint64_t>(defender.action) & ~0x0fffULL) | 0x04f6ULL);
    }

    VFCL_PublishPendingKnockback(attacker, defender, facingAngle);
    if (attacker.suppressKnockback)
        return;
    std::int32_t damage = static_cast<std::int32_t>(static_cast<double>(impact) + 0.5);
    if (!specialReaction && defender.slotIndex < kBattleSlots) {
        const VFFormationGroup *const wave = defender.game ? defender.game->battleMeta.waves[defender.slotIndex] : nullptr;
        if (wave) {
            const std::uint8_t selectFlags = static_cast<std::uint8_t>(wave->selectFlags);
            if ((static_cast<std::int8_t>(selectFlags) < 0 && (attacker.hitStateFlags & 0x04000008U) != 0) ||
                ((selectFlags & 0x20U) != 0 && (attacker.hitStateFlags & 0x04000000U) != 0)) {
                damage = defender.currentHp;
            }
        }
    }
    vfHandleImpactSfxAndDefeat(defender, static_cast<std::int16_t>(damage), 1);
}

void vfclResolveHitVsHitOutcome(VFActor &actorA, VFActor &actorB) {
    constexpr std::uint32_t kContactCategoryB = 0x00000800U;
    constexpr std::uint32_t kMidAirRecoveryActive = 0x00040000U;
    constexpr std::uint32_t kAirborneOrDowned = 0x00080000U;
    constexpr std::uint32_t kHitPriorityMedium = 0x00008000U;
    constexpr std::uint32_t kRelativeAlignOnGuard = 0x00080000U;

    const std::uint32_t actorAHitState = actorA.hitStateFlags2;
    const std::uint32_t actorBHitState = actorB.hitStateFlags2;
    if ((((actorAHitState | actorBHitState) & kMidAirRecoveryActive) != 0 && ((actorA.motionFlags | actorB.motionFlags) & kContactCategoryB) == 0) ||
        ((actorAHitState & kAirborneOrDowned) != 0 && actorA.hitSourceActor && (actorA.hitSourceActor->hitStateFlags2 & kMidAirRecoveryActive) != 0) ||
        ((actorBHitState & kAirborneOrDowned) != 0 && actorB.hitSourceActor && (actorB.hitSourceActor->hitStateFlags2 & kMidAirRecoveryActive) != 0)) {
        return;
    }

    const bool actorAConnected = VFCL_CheckConnectionGraph(actorA, actorB);
    const bool actorBConnected = VFCL_CheckConnectionGraph(actorB, actorA);

    std::uint8_t outcome = 0;
    if (actorAConnected) {
        if (!actorBConnected) {
            outcome = 2;
        } else if ((actorA.hitFlags & kRelativeAlignOnGuard) != 0) {
            outcome = 2;
        } else if ((actorB.hitFlags & kRelativeAlignOnGuard) != 0) {
            outcome = 1;
        } else if ((actorA.hitFlags & kHitPriorityMedium) != 0) {
            outcome = 2;
        } else if ((actorB.hitFlags & kHitPriorityMedium) != 0) {
            outcome = 1;
        } else if (actorA.guardStrength > actorB.guardStrength) {
            outcome = 2;
        } else if (actorA.guardStrength < actorB.guardStrength) {
            outcome = 1;
        } else if (actorA.attackWindowEndFrame < actorB.attackWindowEndFrame) {
            outcome = 2;
        } else if (actorA.attackWindowEndFrame > actorB.attackWindowEndFrame) {
            outcome = 1;
        } else {
            outcome = 3;
        }
    } else {
        outcome = actorBConnected ? 1 : 0;
    }

    switch (outcome) {
    case 1:
        VFCL_ApplyImpact(actorB, actorA);
        break;
    case 2:
        VFCL_ApplyImpact(actorA, actorB);
        break;
    case 3:
        VFCL_ApplyImpact(actorA, actorB);
        VFCL_ApplyImpact(actorB, actorA);
        break;
    default:
        break;
    }
}

} // namespace shenmue::s1
