#include "s2/player_movement_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/motion_ground_dc.hpp"
#include "platform/dc/sdk.hpp"
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <bit>
#endif

namespace shenmue {
namespace s2 {

// @verified - d
void InitializePlayerMovementController(PlayerMovementState *state) {
    state->flags = 0;
    state->character = 0;
    state->motion = 0;
    state->state = 0;
    state->retained24 = 0;
    state->retained26 = 0;
    state->retained48 = 0;
    state->rootYawMode = 0;
    state->retained50 = 0;
    state->firstScale = 1.0f;
    state->secondScale = 1.0f;
    state->retained124 = 0.0f;
    state->motionScale = 1.0f;
    state->retained44 = 0;
}

// @verified - d
std::uint32_t ChooseRestingPlayerMotion(PlayerMovementState *state, PlayerMotionSelection *selection) {
    MotionWork *const work = state->motion;
    const std::int32_t current = mtwkGetAnimationId(work, static_cast<MotionRegion>(0));
    std::int32_t animation;
    std::uint32_t mirrored = 0;
    switch (current) {
    case 0xf002:
        if (mtwkSetRegionFlag(work, 70, 1, static_cast<MotionRegion>(0)))
            mirrored = 1;
        if (g_playerRuntimePointer->flags & 0x80000U) {
            animation = 0xf002;
        } else {
            const std::int32_t count = state->idleVariationCounter;
            const std::int32_t random = static_cast<std::int32_t>(dc::sdk::RandomInt());
            animation = count > random % 4 + 2 ? 0xf004 : 0xf002;
        }
        break;
    case 0xf004:
        if (!mtwkSetRegionFlag(work, 70, 1, static_cast<MotionRegion>(0)))
            mirrored = 1;
        animation = 0xf002;
        break;
    case 0xf003:
        animation = 0xf003;
        break;
    default:
        animation = 0xf002;
        if (!mtwkIsRightFootTargetAhead(work))
            mirrored = 1;
        break;
    }
    if (!(g_playerRuntimePointer->flags & 0x80000U))
        state->idleVariationCounter = static_cast<std::int8_t>(static_cast<std::uint8_t>(static_cast<std::uint8_t>(state->idleVariationCounter) + 1U));
    if (animation == 0xf004U)
        state->idleVariationCounter = 0;
    selection->mirrored = mirrored;
    return animation;
}

// @verified - d
void ResetPlayerMovementController(PlayerMovementState *state) {
    MotionWork *const work = state->motion;
    if (!work)
        return;
    mtwkSetTimeRelated(work, 1, 0, 0xf002U);
    mtwkSetTimeRelated(work, 1, 1, 0xf005U);
    mtwkSetTimeRelated(work, 1, 4, 0xf006U);
    player_detail::PlayerMotionRequest request;
    const std::uint32_t animation = ChooseRestingPlayerMotion(state, reinterpret_cast<PlayerMotionSelection *>(&request));
    mtwkSetRequestedRootMode(work, 1);
    mtwkSetFrameTime(work, 20, static_cast<MotionRegion>(0));
    mtwkSetSpeed(work, 1.0f, static_cast<MotionRegion>(0));
    mtwkRequestMotion(work, animation, static_cast<MotionRegion>(0));
    if (request.mirrored)
        mtwkSetRegionFlag(work, 69, 0, static_cast<MotionRegion>(0));
}

} // namespace s2
} // namespace shenmue

#endif
