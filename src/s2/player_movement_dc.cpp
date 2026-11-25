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
    std::uint32_t animation = mtwkGetAnimationId(work, static_cast<MotionRegion>(0));
    std::uint32_t mirrored = 0;
    if (animation != 0xf003U) {
        if (animation == 0xf002U) {
            mirrored = mtwkSetRegionFlag(work, 70, 1, static_cast<MotionRegion>(0)) ? 1U : 0U;
            if (!(g_playerRuntimePointer->flags & 0x80000U)) {
                const std::int32_t count = state->idleVariationCounter;
                const std::int32_t random = static_cast<std::int32_t>(dc::sdk::RandomInt());
                animation = random % 4 + 2 < count ? 0xf004U : 0xf002U;
            } else {
                animation = 0xf002U;
            }
        } else if (animation == 0xf004U) {
            mirrored = mtwkSetRegionFlag(work, 70, 1, static_cast<MotionRegion>(0)) ? 0U : 1U;
            animation = 0xf002U;
        } else {
            animation = 0xf002U;
            mirrored = mtwkIsRightFootTargetAhead(work) ? 0U : 1U;
        }
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
