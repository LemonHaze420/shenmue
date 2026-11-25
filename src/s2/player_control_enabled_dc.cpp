#include "s2/player_model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/motion_work_dc.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
std::uint32_t SetPlayerControlEnabled(std::int32_t mode) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!model)
        return UINT32_MAX;
    std::int32_t previous = static_cast<std::int32_t>(runtime->flags) >> 1;
    previous &= 1;
    if (mode < 0)
        return previous;
    if (mode) {
        if (previous)
            return previous;
    } else if (!previous) {
        return 0;
    }
    if (model->motion) {
        Vector3i rotation;
        GetCharacterLocalRotation(model->character, &rotation);
        rotation.z = 0;
        rotation.x = 0;
        SetCharacterLocalRotation(model->character, &rotation);
        SetCharacterMotionEnabled(model->character, 1);
        if (mtwkSetExtraFlag(model->motion, 59, 1))
            mtwkUpdateWithPosition(model->motion, 0, 0);
    }
    if (mode) {
        runtime->flags = (runtime->flags | 0x8002U) & ~0x4000U;
        model->padFlags &= 0xfbf7ffffU;
        model->pendingMovementMode = 0;
        model->motionInputState = 0;
        ResetPlayerMovementController(model->movementState);
        if (model->flags & 0x10000U)
            SetPlayerGameFlags(13, reinterpret_cast<void *>(1U));
        model->interpolationValue = 0.0f;
    } else {
        runtime->flags &= ~2U;
        model->latchedMovementMode = model->currentMovementMode;
        if (model->currentMovementMode == 3 && model->movementInterpolation > 0.5f)
            model->latchedMovementMode = 4;
        model->latchedMovementMode = model->activeMovementMode;
        model->requestedMovementMode = 1;
        if (model->activeMovementMode == 3)
            model->requestedMovementMode = 3;
        if (model->activeMovementMode == 4)
            model->requestedMovementMode = 4;
        if (model->interpolationValue > 0.5f)
            model->requestedMovementMode = 4;
        UpdatePlayerPartyCompanions(model);
    }
    runtime->flags &= ~0x1000000U;
    model->padFlags &= 0xff7fffffU;
    model->lookAroundState = 0;
    model->lookInputGraceFrames = 0;
    model->turnaroundState = 0;
    model->padFlags &= ~0x100U;
    model->padFlags &= ~0x10000000U;
    model->lookTargetMode = 0;
    model->lookTargetFlags = 0;
    model->automaticLookIdleTimer = 0.0f;
    model->pendingHeldStateMode = 0;
    return previous;
}

} // namespace s2
} // namespace shenmue

#endif
