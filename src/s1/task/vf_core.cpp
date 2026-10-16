#include "s1/task/vf_core.hpp"

#include <algorithm>
#include <array>
#include <cmath>

namespace shenmue::s1 {

namespace {

constexpr std::uint32_t kVFHitTimeout = 0x00000002U;
constexpr std::uint32_t kVFMotionOverride = 0x00200000U;

constexpr std::array<std::uint8_t, 48> kVFMirroredMoveIds{
    0x00, 0x02, 0x01, 0x04, 0x03, 0x06, 0x05, 0x08,
    0x07, 0x0a, 0x09, 0x0c, 0x0b, 0x0d, 0x0f, 0x0e,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x17, 0x16,
    0x19, 0x18, 0x1b, 0x1a, 0x1c, 0x1d, 0x1f, 0x1e,
    0x21, 0x20, 0x22, 0x23, 0x25, 0x24, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x00, 0x02, 0x01, 0x01, 0x01,
};

std::uint16_t VFReadU16(const std::byte *data) {
    return static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(data[0]) | (static_cast<std::uint16_t>(std::to_integer<std::uint8_t>(data[1])) << 8U));
}

std::uint32_t VFReadU32(const std::byte *data) {
    return static_cast<std::uint32_t>(VFReadU16(data)) | (static_cast<std::uint32_t>(VFReadU16(data + 2)) << 16U);
}

float VFReadFixed16(const std::byte *data) {
    const std::uint32_t value = (static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(data[0])) << 24U) |
        (static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(data[1])) << 16U) |
        (static_cast<std::uint32_t>(std::to_integer<std::uint8_t>(data[2])) << 8U) | std::to_integer<std::uint8_t>(data[3]);
    return static_cast<float>(static_cast<std::int32_t>(value)) / 65536.0f;
}

std::uint8_t VFScaledGuard(std::uint8_t guardStat, std::uint8_t strength) {
    const double scale = static_cast<double>(guardStat) * 0.05 + 1.0;
    return static_cast<std::uint8_t>(static_cast<std::int32_t>(scale * static_cast<double>(strength)));
}


bool vfSetReactProgram(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.frameEventProgram = cursor;
    return true;
}


bool VFActor_UpdateReactFrameHeader(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.postReactionAnimationId = VFReadU16(cursor + 2);
    cursor += 4;
    return false;
}


bool VFActor_UpdateReactAnimationFrameRange(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    std::uint16_t attackLimit = state.reactionAnimationEndFrame;
    state.reactionEndFrame = std::min(VFReadU16(cursor + 6), attackLimit);
    state.reactionMidFrame = VFReadU16(cursor + 4);
    if (state.reactionMidFrame > attackLimit) {
        state.reactionMidFrame = attackLimit;
        --attackLimit;
    }
    state.attackWindowEndFrame = std::min(VFReadU16(cursor + 2), attackLimit);
    state.hitStateFlags2 |= kVFHitTimeout;
    state.reactionWindowPhase = 1;
    cursor += 8;
    return false;
}


bool VFActor_InitKnockbackParams(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.skipAlignment = static_cast<std::int16_t>((static_cast<std::uint16_t>(state.skipAlignment) & 0xff00U) | std::to_integer<std::uint8_t>(cursor[1]));
    state.fallLandingWindowStartFrame = VFReadU16(cursor + 2);
    state.knockbackAnimationStartParameter = VFReadU16(cursor + 4);
    state.knockbackAnimationEndFrame = VFReadU16(cursor + 6);
    state.knockbackReactionTime = VFReadFixed16(cursor + 8);
    state.fallLandingWindowEndFrame = VFReadU16(cursor + 12);
    cursor += 16;
    return false;
}


bool vfUpdateFromReactProg(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.moveId = std::to_integer<std::uint8_t>(cursor[1]);
    state.guardState = std::to_integer<std::uint8_t>(cursor[2]);
    std::uint8_t guardStrength = std::to_integer<std::uint8_t>(cursor[3]);
    state.hitDamageScale = VFReadFixed16(cursor + 4);
    state.hitDirectionAngle = VFReadU16(cursor + 8);
    state.hitParameter = VFReadU16(cursor + 10);
    state.hitAttributeFlags = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[12]));
    state.hitGuardType = std::to_integer<std::uint8_t>(cursor[13]);
    state.hitStunOverride = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[14]));
    state.hitHeightClass = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[15]));
    state.hitHeightClass2 = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[16]));
    state.field322Low = 0;
    if (((input.pendingAnimationId - 502U) & 0xfffbU) == 0U) {
        state.field322Low = 1;
        guardStrength = 29;
        state.hitParameter &= ~0x4000U;
    } else if (input.pendingAnimationId == 588U) {
        guardStrength = 29;
        state.hitParameter &= ~0x4000U;
    }
    state.guardStrength = VFScaledGuard(input.guardStat, guardStrength);
    if (state.mirrorReaction && state.moveId < kVFMirroredMoveIds.size()) {
        state.moveId = kVFMirroredMoveIds[state.moveId];
    }
    cursor += 20;
    return false;
}


bool VFActor_UpdateHitState(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    std::uint8_t rawStrength = 0;
    if ((state.hitStateFlags & 0x0c00U) == 0) {
        state.hitParameter = VFReadU16(cursor + 2);
        rawStrength = std::to_integer<std::uint8_t>(cursor[1]);
    }
    const std::uint8_t scaled = VFScaledGuard(input.guardStat, rawStrength);
    if ((state.hitStateFlags & 0x0400U) != 0) {
        state.skipAlignment =
            static_cast<std::int16_t>((static_cast<std::uint16_t>(state.skipAlignment) & 0x00ffU) | (static_cast<std::uint16_t>(scaled) << 8U));
    } else {
        state.guardStrength = scaled;
    }
    state.hitDamageScale = VFReadFixed16(cursor + 4);
    state.facingAngleStep = VFReadU16(cursor + 8);
    ++state.activeContactCount;
    cursor += 12;
    return false;
}


bool VFActor_ResetKnockbackAndFacing(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.knockbackAnimationEndFrame = VFReadU16(cursor + 2);
    state.facingAngleStep = VFReadU16(cursor + 4);
    state.field332 = 0;
    cursor += 8;
    return false;
}


bool VF_SetActorFacingAngleStep(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.facingAngleStep = VFReadU16(cursor + 2);
    cursor += 4;
    return false;
}


bool VFActor_InitializeReactProgram(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.frameEventProgram = cursor + 4;
    if (state.mirrorReaction) {
        state.hitReactionScaleX = -state.hitReactionScaleX;
        state.facingAngleStep = static_cast<std::uint16_t>(-static_cast<std::int16_t>(state.facingAngleStep));
        state.hitDirectionAngle = static_cast<std::uint16_t>(-static_cast<std::int16_t>(state.hitDirectionAngle));
    }
    return true;
}


bool VFActor_UpdateKnockbackReactData(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.hitReactionScaleX = VFReadFixed16(cursor + 4);
    state.hitReactionScaleZ = VFReadFixed16(cursor + 8);
    state.knockbackTotalFrames = static_cast<std::uint16_t>(
        static_cast<std::int32_t>(static_cast<float>(static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[1]))) * input.animationScale));
    state.trackingSlideStartFrame = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[2]));
    state.knockbackAccelerationBias = std::fabs(static_cast<float>(
        static_cast<std::int32_t>(VFReadFixed16(cursor + 12) * (input.knockbackStrength * input.knockbackStrength * input.knockbackGravityScale))));
    cursor += 16;
    return false;
}


bool VFActor_IncrementReactEvtCnt2AndSetEntryTblIfZero(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    if (state.reactionEventCount == 0)
        state.reactionEventTable = cursor;
    ++state.reactionEventCount;
    cursor += 8;
    return false;
}


bool vfUpdateStateFlagsFromReactProg(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.field324 = VFReadU16(cursor + 2);
    state.stateFlags2 = VFReadU32(cursor + 4);
    if (state.mirrorReaction)
        state.stateFlags2 ^= 1U;
    state.reactionStateId = static_cast<std::uint16_t>(state.stateFlags2 & 0x7fffU);
    state.counterReactionAnimationId = VFReadU32(cursor + 8);
    cursor += 12;
    return false;
}


bool VFActor_ApplyReactProgMoveId(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.reactionState02EndFrame = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[1]));
    state.field314 = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[2]));
    cursor += 4;
    return false;
}


bool VFActor_UpdateReactProgMoveID(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.midairRecoveryEndFrame = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[1]));
    state.field330 = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[2]));
    cursor += 4;
    return false;
}


bool VFActor_UpdateGuardStrength(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    const std::uint8_t highDistanceStrength = state.guardStrength;
    const std::uint8_t lowDistanceStrength = std::to_integer<std::uint8_t>(cursor[1]);
    const std::uint8_t middleStrength = std::to_integer<std::uint8_t>(cursor[2]);
    const float highDistance = VFReadFixed16(cursor + 4);
    const float lowDistance = VFReadFixed16(cursor + 8);
    const float middleDistance = VFReadFixed16(cursor + 12);
    std::uint8_t startStrength = lowDistanceStrength;
    std::uint8_t endStrength = highDistanceStrength;
    float segmentStart = lowDistance;
    float segmentEnd = highDistance;
    if (middleStrength != 0 && middleDistance < input.contactDistance) {
        endStrength = middleStrength;
        segmentEnd = middleDistance;
    } else if (middleStrength != 0) {
        startStrength = middleStrength;
        segmentStart = middleDistance;
    }
    const float span = segmentEnd - segmentStart;
    const float clampedOffset = std::min(
        std::max(input.contactDistance - segmentStart,
                 std::min(0.0f, span)),
        std::max(0.0f, span));
    const std::int32_t strength = startStrength +
        static_cast<std::uint8_t>(static_cast<std::int32_t>((static_cast<float>(endStrength) - static_cast<float>(startStrength)) * (clampedOffset / span)));
    state.guardStrength =
        static_cast<std::uint8_t>(static_cast<std::int32_t>(static_cast<double>(strength) * (static_cast<double>(input.guardStat) * 0.05 + 1.0)));
    cursor += 16;
    return false;
}


bool vfApplyNextReactProg(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.pairActionTriggerFrame = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[1]));
    state.pairFollowupActionWord = static_cast<std::int16_t>(VFReadU16(cursor + 2));
    state.pairContactPriority = static_cast<std::int16_t>(VFReadU16(cursor + 4));
    state.pairPriorityAnimationFrame = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[6]));
    cursor += 8;
    return false;
}


bool vfIncrementReactProgPC(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    cursor += 8;
    return false;
}


bool VFActor_ResetReactProgram(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.reactionRequestedMoveId = std::to_integer<std::uint8_t>(cursor[1]);
    cursor += 4;
    return false;
}


bool vfSetMotionOverrideActive(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.mirrorReaction = !state.mirrorReaction;
    cursor += 4;
    return false;
}


bool vfIncrementReactProgPC4(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    cursor += 4;
    return false;
}


bool VFActor_UpdateGuardOnReactProgMatch(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    if (input.pairedFightStyleIndex == static_cast<std::int8_t>(std::to_integer<std::uint8_t>(cursor[1]))) {
        state.hitParameter = VFReadU16(cursor + 4);
        state.guardStrength = VFScaledGuard(input.guardStat, std::to_integer<std::uint8_t>(cursor[2]));
    }
    cursor += 8;
    return false;
}


bool VFActor_SetKnockbackAccelFromProg(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.knockbackAcceleration = VFReadFixed16(cursor + 4);
    cursor += 8;
    return false;
}


bool VFActor_UpdateReactProgAnimationFrames(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state, const std::byte *&cursor) {
    state.knockbackAnimationId = VFReadU16(cursor + 2);
    cursor += 4;
    return false;
}

VFCoreMotionGate VFDecodeMotionProgram(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state) {
    const std::byte *cursor = input.reactionData + 12;
    for (;;) {
        bool finished;
        switch (std::to_integer<std::uint8_t>(*cursor)) {
        case 0:
            finished = vfSetReactProgram(input, state, cursor);
            break;
        case 1:
            finished = VFActor_UpdateReactFrameHeader(input, state, cursor);
            break;
        case 2:
            finished = VFActor_UpdateReactAnimationFrameRange(input, state, cursor);
            break;
        case 3:
            finished = VFActor_InitKnockbackParams(input, state, cursor);
            break;
        case 4:
            finished = vfUpdateFromReactProg(input, state, cursor);
            break;
        case 5:
            finished = VFActor_UpdateHitState(input, state, cursor);
            break;
        case 6:
            finished = VFActor_ResetKnockbackAndFacing(input, state, cursor);
            break;
        case 7:
            finished = VF_SetActorFacingAngleStep(input, state, cursor);
            break;
        case 8:
            finished = VFActor_InitializeReactProgram(input, state, cursor);
            break;
        case 9:
            finished = VFActor_UpdateKnockbackReactData(input, state, cursor);
            break;
        case 10:
            finished = VFActor_IncrementReactEvtCnt2AndSetEntryTblIfZero(input, state, cursor);
            break;
        case 11:
            finished = vfUpdateStateFlagsFromReactProg(input, state, cursor);
            break;
        case 12:
            finished = VFActor_ApplyReactProgMoveId(input, state, cursor);
            break;
        case 13:
            finished = VFActor_UpdateReactProgMoveID(input, state, cursor);
            break;
        case 14:
            finished = VFActor_UpdateGuardStrength(input, state, cursor);
            break;
        case 15:
            finished = vfApplyNextReactProg(input, state, cursor);
            break;
        case 17:
            finished = vfIncrementReactProgPC(input, state, cursor);
            break;
        case 18:
            finished = VFActor_ResetReactProgram(input, state, cursor);
            break;
        case 19:
            finished = vfSetMotionOverrideActive(input, state, cursor);
            break;
        case 20:
            finished = vfIncrementReactProgPC4(input, state, cursor);
            break;
        case 21:
            finished = vfIncrementReactProgPC4(input, state, cursor);
            break;
        case 22:
            finished = vfIncrementReactProgPC4(input, state, cursor);
            break;
        case 23:
            finished = vfIncrementReactProgPC4(input, state, cursor);
            break;
        case 24:
            finished = vfIncrementReactProgPC4(input, state, cursor);
            break;
        case 25:
            finished = vfIncrementReactProgPC4(input, state, cursor);
            break;
        case 26:
            finished = VFActor_UpdateGuardOnReactProgMatch(input, state, cursor);
            break;
        case 27:
            finished = VFActor_SetKnockbackAccelFromProg(input, state, cursor);
            break;
        case 28:
            finished = VFActor_UpdateReactProgAnimationFrames(input, state, cursor);
            break;
        default:
            return VFCoreMotionGate::UnsupportedMotionProgram;
        }
        if (finished)
            return VFCoreMotionGate::Ready;
    }
}

} // namespace

VFCoreMotionGate VFCore_DecodeNativeMotion(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state) {
    if (!input.reactionData || input.animationId == 0 || input.frameCount == 0) {
        return VFCoreMotionGate::MissingMotionData;
    }

    const std::byte *const bytes = input.reactionData;
    state.reactionWindowPhase = 0;
    state.postReactionAnimationId = 0;
    state.facingAngleStep = 0;
    state.reactionEventCount = 0;
    state.stateFlags2 = 0;
    state.reactionState02EndFrame = 0;
    state.hitAttributeFlags = 0;
    state.fallLandingWindowEndFrame = 0;
    state.knockbackAnimationStartParameter = 0;
    state.knockbackAnimationEndFrame = 0;
    state.reactionRequestedMoveId = 0;
    state.knockbackAnimationId = 0;
    state.attackWindowEndFrame = 1;
    state.reactionAnimationEndFrame = input.frameCount;
    state.reactionMidFrame = input.frameCount;
    state.reactionEndFrame = input.frameCount;
    state.hitFlags = VFReadU32(bytes);
    state.hitStateFlags2 = state.hitFlags & ~kVFMotionOverride;
    state.hitStateFlags = VFReadU32(bytes + 8);
    state.motionFlags = state.hitStateFlags;
    state.reactionMotionRestartMode = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(bytes[4]));
    state.field2e1 = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(bytes[5]));
    state.previousHitStateFlags = VFReadU16(bytes + 6);
    state.mirrorReaction = input.mirrorReaction;

    constexpr std::uint32_t kFacingStepOnTransition = 0x00000040U;
    constexpr std::uint32_t kAttackActive = 0x00000100U;
    constexpr std::uint32_t kSpecialThrowState = 0x00008000U;
    constexpr std::uint32_t kGuardState = 0x00010000U;
    if ((state.hitStateFlags2 & kFacingStepOnTransition) != 0)
        state.facingAngleStep = 0x8000U;

    // normalisation
    if ((state.hitStateFlags2 & (kAttackActive | kSpecialThrowState)) == (kAttackActive | kSpecialThrowState)) {
        state.hitStateFlags2 = (state.hitStateFlags2 & ~(kSpecialThrowState | kGuardState)) | kGuardState;
    }

    return VFDecodeMotionProgram(input, state);
}

std::uint16_t VFCore_ActionAnimation(std::uint8_t low, std::uint8_t high) {

    return static_cast<std::uint16_t>(low | (static_cast<std::uint16_t>(high) << 8U));
}

} // namespace shenmue::s1
