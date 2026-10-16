#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

enum class VFCoreMotionGate : std::uint8_t {
    Ready,
    MissingMotionData,
    UnsupportedMotionProgram,
};

struct VFCoreMotionDecodeInput {
    const std::byte *reactionData = nullptr;
    std::uint16_t animationId = 0;
    std::uint16_t frameCount = 0;
    std::uint16_t pendingAnimationId = 0;
    float animationScale = 1.0f;
    float contactDistance = 0.0f;
    std::uint8_t guardStat = 0;
    std::int16_t pairedFightStyleIndex = -1;
    bool mirrorReaction = false;
    float knockbackStrength = 1.0f;
    float knockbackGravityScale = 0.0f;
};

struct VFCoreMotionState {
    std::uint32_t hitFlags = 0;
    std::uint32_t hitStateFlags = 0;
    std::uint32_t hitStateFlags2 = 0;
    std::uint32_t motionFlags = 0;
    std::uint32_t stateFlags2 = 0;
    std::uint32_t counterReactionAnimationId = 0;
    const std::byte *frameEventProgram = nullptr;
    const std::byte *reactionEventTable = nullptr;
    std::uint16_t reactionAnimationEndFrame = 0;
    std::uint16_t reactionStateId = 0;
    std::uint16_t postReactionAnimationId = 0;
    std::uint16_t reactionMidFrame = 0;
    std::uint16_t reactionEndFrame = 0;
    std::uint16_t attackWindowEndFrame = 1;
    std::uint16_t fallLandingWindowStartFrame = 0;
    std::uint16_t fallLandingWindowEndFrame = 0;
    std::uint16_t knockbackAnimationStartParameter = 0;
    std::uint16_t knockbackAnimationEndFrame = 0;
    std::uint16_t trackingSlideStartFrame = 0;
    std::uint16_t knockbackTotalFrames = 0;
    std::uint16_t facingAngleStep = 0;
    std::uint16_t previousHitStateFlags = 0;
    std::uint16_t hitParameter = 0;
    std::uint16_t hitDirectionAngle = 0;
    std::uint16_t reactionState02EndFrame = 0;
    std::uint16_t midairRecoveryEndFrame = 0;
    std::uint16_t field324 = 0;
    std::uint16_t pairPriorityAnimationFrame = 0;
    std::int16_t field332 = 0;
    std::uint16_t knockbackAnimationId = 0;
    std::int16_t pairFollowupActionWord = 0;
    std::int16_t pairActionTriggerFrame = 0;
    std::int32_t pairContactPriority = 0;
    std::uint16_t activeContactCount = 0;
    float hitDamageScale = 0.0f;
    float hitReactionScaleX = 0.0f;
    float hitReactionScaleZ = 0.0f;
    float knockbackReactionTime = 0.0f;
    float knockbackAcceleration = 0.0f;
    float knockbackAccelerationBias = 0.0f;
    std::uint8_t moveId = 0;
    std::uint8_t guardState = 0;
    std::uint8_t guardStrength = 0;
    std::uint8_t hitGuardType = 0;
    std::uint8_t reactionEventCount = 0;
    std::uint8_t reactionRequestedMoveId = 0;
    std::int8_t hitAttributeFlags = 0;
    std::int8_t hitStunOverride = 0;
    std::int8_t hitHeightClass = 0;
    std::int8_t hitHeightClass2 = 0;
    std::int8_t reactionMotionRestartMode = 0;
    std::int8_t field2e1 = 0;
    std::int16_t skipAlignment = 0;
    std::int16_t field314 = 0;
    std::int16_t field330 = 0;
    std::uint8_t field322Low = 0;
    std::uint8_t reactionWindowPhase = 0;
    bool mirrorReaction = false;
};

VFCoreMotionGate VFCore_DecodeNativeMotion(const VFCoreMotionDecodeInput &input, VFCoreMotionState &state);

std::uint16_t VFCore_ActionAnimation(std::uint8_t low, std::uint8_t high);

} // namespace shenmue::s1
