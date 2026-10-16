#include "s1/task/vf_internal.hpp"
#include "s1/task/vf_reaction_tables.hpp"

namespace shenmue::s1 {
using namespace vf_detail;

bool VFCL_AttackActive(const VFActor &actor) {
    constexpr std::uint16_t kGroundedContactAnimationA = 343U;
    constexpr std::uint16_t kGroundedContactAnimationB = 500U;
    if ((actor.hitStateFlags2 & 0x00000100U) == 0 || actor.currentAnimationEndFrame < actor.attackWindowEndFrame) {
        return false;
    }
    const std::uint16_t animation = actor.currentReactionAnimation;
    return (actor.groundContactFlags & 1) == 0 || (animation != kGroundedContactAnimationA && animation != kGroundedContactAnimationB);
}


bool VFCL_CheckConnectionGraph(VFActor &attacker, VFActor &defender) {
    const std::uint8_t defenderBit = static_cast<std::uint8_t>(1U << defender.slotIndex);
    constexpr std::uint32_t kAirborneOrDowned = 0x00080000U;
    constexpr std::uint32_t kPairedContactCategory = 0x00000800U;
    if ((defender.moveFlags & 0x00040000U) != 0 || (attacker.hitStateFlags2 & 0x00000100U) == 0 ||
        (((defender.hitStateFlags2 & kAirborneOrDowned) != 0) && (defender.motionFlags & kPairedContactCategory) == 0) ||
        attacker.moveId >= kVFMoveCategoryMasks.size()) {
        attacker.connectionMask = static_cast<std::int8_t>(static_cast<std::uint8_t>(attacker.connectionMask) & ~defenderBit);
        return false;
    }
    if (attacker.currentReactionAnimation != attacker.previousAnimationId) {
        attacker.connectionMask = static_cast<std::int8_t>(static_cast<std::uint8_t>(attacker.connectionMask) & ~defenderBit);
    }
    if (!VFCL_AttackActive(attacker))
        return false;
    if ((static_cast<std::uint8_t>(attacker.connectionMask) & defenderBit) != 0) {
        return false;
    }

    const auto *const attackerVolumes = reinterpret_cast<const VFContactVolumeRecord *>(attacker.contactVolumeRecords);
    const auto *const defenderVolumes = reinterpret_cast<const VFContactVolumeRecord *>(defender.contactVolumeRecords);
    const std::uint32_t attackMask = kVFMoveCategoryMasks[attacker.moveId];
    std::uint32_t resultMask = 0;
    for (std::size_t attackVolume = 0; attackVolume < kVFContactVolumes.size(); ++attackVolume) {
        const std::uint16_t category = attackerVolumes[attackVolume].mask;
        if ((category & attackMask) == 0)
            continue;
        for (std::size_t defenseVolume = 0; defenseVolume < kVFContactVolumes.size(); ++defenseVolume) {
            if ((category & defender.contactProfileMasks[defenseVolume]) != 0)
                resultMask |= defenderVolumes[defenseVolume].mask;
        }
    }
    defender.connectionResultMetadata = static_cast<std::int32_t>(resultMask & ~defender.contactMetadataFilter);
    if (defender.connectionResultMetadata == 0)
        return false;
    attacker.connectionMask = static_cast<std::int8_t>(static_cast<std::uint8_t>(attacker.connectionMask) | defenderBit);
    constexpr std::uint32_t kConnectionPairMatch = 0x2U;
    constexpr std::uint32_t kConnectionAnyMatch = 0x8U;
    if ((attacker.triggerMask & kConnectionAnyMatch) != 0)
        attacker.triggerState |= kConnectionAnyMatch;
    if ((attacker.triggerMask & kConnectionPairMatch) != 0 &&
        static_cast<std::int32_t>(static_cast<std::int8_t>(attacker.fighterRankId)) == attacker.requiredFighterRank) {
        attacker.triggerState |= kConnectionPairMatch;
    }
    defender.fighterIdAlt3 = attacker.fighterRankId;
    return true;
}


float VFCL_ResolveImpact(VFActor &attacker, VFActor &defender, float baseImpact) {
    if ((defender.moveFlags & 0x20000000U) != 0) {
        if ((defender.hitStateFlags2 & 0x00014000U) == 0)
            defender.moveFlags &= ~0x20000000U;
        return static_cast<float>(static_cast<double>(baseImpact) * 1.2);
    }
    if ((defender.hitStateFlags2 & 0x00000100U) != 0) {
        attacker.hitOutcome = 6;
        attacker.nextAction = {};
        attacker.nextAction.opcode = 4;
        attacker.nextAction.state = 20;
        return static_cast<float>(static_cast<double>(baseImpact) * 1.05);
    }
    if ((defender.hitStateFlags2 & 0x00004010U) != 0x00000010U) {
        if ((defender.hitFlags & 0x00000100U) != 0 && (defender.hitStateFlags2 & 0x00000001U) != 0) {
            attacker.hitOutcome = 37;
            return static_cast<float>(static_cast<double>(baseImpact) * 1.03 + 2.0);
        }
        if ((defender.motionFlags & 0x00000004U) != 0)
            return static_cast<float>(static_cast<double>(baseImpact) * 1.3);
        if ((defender.hitStateFlags2 & 0x00080000U) != 0)
            return static_cast<float>(static_cast<double>(baseImpact) * 1.25);
        const std::int16_t facingDelta = static_cast<std::int16_t>(attacker.facingAngle + attacker.hitDirectionAngle - defender.temporaryFacingAngle + 0x4000U);
        if ((defender.hitStateFlags2 & 0x00014000U) != 0 || facingDelta < 0) {
            return baseImpact;
        }
        attacker.hitOutcome = 37;
    }
    return static_cast<float>(static_cast<double>(baseImpact) * 1.1);
}


void VFCL_PlayImmediateHitCue(float impact, const VFActor &attacker) {
    constexpr std::array<std::uint32_t, 2> kHitSoundCommands{0x00000U, 0x176abU};
    if (impact == 0.0f || (attacker.hitStateFlags2 & 0x00040100U) == 0x00040000U || !attacker.game || !attacker.character ||
        (attacker.game->flags & (PlayerAggro | FreeBattleIntroActive)) != 0) {
        return;
    }

    std::uint32_t command = 0;
    const std::uint8_t hitAttribute = static_cast<std::uint8_t>(attacker.hitAttributeFlags);
    if (hitAttribute == 1U) {

        command = kHitSoundCommands[hitAttribute];
    } else if (attacker.moveId == 5U || attacker.moveId == 6U) {
        command = impact < 20.0f ? 0xc76abU : 0xd76abU;
    } else {
        command = 0x276abU;
    }
    if (command == 0)
        return;

    SNDWrapper(command, 5U, attacker.character);
}
std::uint32_t g_vfHitActionContext = 0;


void vfAssignJumpIfNeeded(const VFActor &attacker, const VFActor &defender) {
    if (g_vfHitActionContext == 519U && static_cast<double>(attacker.groundY) + 0.9 >= defender.contactCurrentPosition.y)
        g_vfHitActionContext = static_cast<double>(attacker.groundY) + 0.6 >= defender.contactCurrentPosition.y ? 521U : 270U;
}


std::uint16_t VFCL_SelectReactionAnimation(VFActor &attacker, const VFActor &defender, std::uint32_t selector) {
    VFMAData &game = *attacker.game;
    g_vfHitActionContext = selector;
    if (game.battleType == 2U && selector == 0U) {
        const std::uint8_t guard = attacker.guardState;
        g_vfHitActionContext = guard == 1U ? 1324U : (guard == 2U || guard == 5U || guard == 6U ? 1326U : 1325U);
        return static_cast<std::uint16_t>(g_vfHitActionContext);
    }
    const std::uint8_t table = static_cast<std::uint8_t>(attacker.hitParameter);
    std::uint32_t direction = (defender.hitStateFlags2 & 0x800U) != 0 ? 2U : 0U;
    if (selector != 9U) {
        if ((defender.hitStateFlags2 & 0x1000U) != 0 && game.reactionTypeIndex == 1U && defender.fightStyleIndex == 0U) {
            vfmaAssignVoiceReactForFrame(game, 13U);
            vfmaAssignVoiceReactForFrame(game, 9U);
        }
        std::uint32_t side = attacker.hitParameter >> 15U;
        if ((attacker.moveFlags & 0x40U) != 0)
            side ^= 1U;
        if ((defender.moveFlags & 0x400U) != 0)
            side ^= 1U;
        std::int16_t angle = static_cast<std::int16_t>(attacker.facingAngle + attacker.hitDirectionAngle - defender.temporaryFacingAngle + 0x4000U);
        if ((defender.motionFlags & 0x01000000U) != 0)
            angle = static_cast<std::int16_t>(angle + attacker.moveFacingAngle);
        if ((defender.hitStateFlags2 & 0x4800U) == 0x4800U)
            angle = static_cast<std::int16_t>(static_cast<std::uint16_t>(angle) ^ 0x8000U);
        direction = side;
        if (angle >= 0) {
            direction += 2U;
            if (game.reactionTypeIndex == 1U && defender.fightStyleIndex == 0U)
                vfmaAssignVoiceReactForFrame(game, 21U);
        }
        if (static_cast<std::uint16_t>(angle + 0x1fffU) <= 0x3ffeU)
            direction = side + (((defender.moveFlags >> 5U) & 2U) | 4U);
        else if (angle > 0x6000)
            direction = side + ((defender.moveFlags & 0x40U) != 0 ? 4U : 6U);
    }
    std::uint16_t animation = 0;
    if (attacker.knockbackAnimationId != 0 && direction < 2U) {
        animation = attacker.knockbackAnimationId;
    } else if (selector == 2U) {
        const std::uint32_t pending = attacker.pendingAnimationId;
        bool useGroundVariant = false;
        bool invertGroundVariant = false;
        switch (pending) {
        case 868:
        case 1142:
        case 1329:
        case 2261:
        case 2306:
        case 2480:
            useGroundVariant = true;
            break;
        case 894:
        case 1213:
        case 1431:
        case 2442:
        case 2444:
        case 2445:
        case 2447:
        case 2474:
            useGroundVariant = true;
            invertGroundVariant = true;
            break;
        case 2094:
            useGroundVariant = attacker.fightStyleIndex == 6U || attacker.fightStyleIndex == 27U;
            invertGroundVariant = true;
            break;
        default:
            break;
        }
        if (useGroundVariant) {
            std::uint8_t ground = static_cast<std::uint8_t>(attacker.groundContactFlags);
            if (invertGroundVariant)
                ground = static_cast<std::uint8_t>(~ground);
            animation = static_cast<std::uint16_t>((ground & 0x10U ? 1U : 0U) + 738U);
        } else if ((defender.hitStateFlags2 & 0x1000U) != 0) {
            animation = 1336U;
        } else if (table < kVFReactionAnimations.size()) {
            animation = kVFReactionAnimations[table][selector][direction];
        }
    } else if (selector != 9U && (defender.hitStateFlags2 & 0x1000U) != 0) {
        animation = selector < 7U ? 1336U : (selector < 9U ? kVFActiveLethalReactionAnimations[direction] : 0U);
    } else if (table < kVFReactionAnimations.size()) {
        if (selector == 5U || selector == 6U)
            animation = kVFStrongHitReactionAnimations[kVFStrongHitReactionVariantIndices[table]][direction];
        else if (selector == 7U)
            animation = kVFDownedReactionAnimations[kVFDownedReactionVariantIndices[table]][direction];
        else if (selector < kVFReactionAnimations[table].size())
            animation = kVFReactionAnimations[table][selector][direction];
    }
    g_vfHitActionContext = animation;
    if (animation != 0 && (attacker.stateFlags & 0x0cU) != 0) {
        const std::array<std::uint16_t, 32> &pairs = (attacker.stateFlags & 8U) != 0 ? kVFActiveReactionPairs : kVFArmedReactionPairs;
        for (std::size_t pair = 0; pair < pairs.size(); pair += 2U) {
            if (animation != kVFArmedReactionPairs[pair])
                continue;
            g_vfHitActionContext = pairs[pair];
            attacker.queuedReactionEvent = 30U;
            attacker.queuedKnockbackReactionAnimId = pairs[pair + 1U];
            attacker.moveFlags = (attacker.moveFlags & ~0x440U) | (defender.moveFlags & 0x440U);
            attacker.hitStateFlags2 = (attacker.hitStateFlags2 & ~0x200000U) | (defender.hitStateFlags2 & 0x200000U);
            break;
        }
    }
    vfAssignJumpIfNeeded(attacker, defender);
    return static_cast<std::uint16_t>(g_vfHitActionContext);
}

void VFCL_SetAction(VFActionDescriptor &action, std::uint16_t animation, std::uint8_t state) {
    action = {};
    action.opcode = static_cast<std::uint8_t>(animation);
    action.index = static_cast<std::uint8_t>(animation >> 8U);
    action.state = state;
}

bool VFCL_PublishSpecialCounter(VFActor &attacker, VFActor &defender) {
    defender.action = {};
    std::memcpy(&defender.action, &attacker.counterReactionAnimationId, sizeof(attacker.counterReactionAnimationId));
    const std::uint16_t counterAnimation = static_cast<std::uint16_t>(attacker.counterReactionAnimationId & 0x0fffU);
    const std::uint8_t *const counter = shenLookupAnimationIndex(counterAnimation, 5);
    if (counter) {
        std::int16_t responseAnimation = 0;
        std::memcpy(&responseAnimation, counter + 2, sizeof(responseAnimation));
        attacker.action = std::bit_cast<VFActionDescriptor>(static_cast<std::uint64_t>(static_cast<std::int64_t>(responseAnimation) + 0x11000000));
        attacker.guardStrength = counter[1];
    }
    return true;
}
} // namespace shenmue::s1
