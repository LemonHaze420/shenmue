#include "s2/player_model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/player_control_state_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

// @verified - d
bool plymIsSearchable() {
    const PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    const PlayerModelState *const model = runtime->model;
    if (!model)
        return false;
    if (!(runtime->flags & 2U))
        return false;
    if (!(model->padFlags & 0x800U))
        return true;
    return false;
}

// @verified - d
std::uint32_t GetPlayerAreaControlState(std::int32_t) {
    const PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    const PlayerModelState *const model = runtime->model;
    if (!model || !(runtime->flags & 0x4000U))
        return UINT32_MAX;
    if (!(model->padFlags & 0x20U))
        return 0;
    return 1;
}

// @verified - d
std::uint32_t SetPlayerSpecialMode(std::int32_t mode) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!(runtime->flags & 0x40000U) || !model)
        return UINT32_MAX;
    runtime->flags &= ~0x80000U;
    model->specialModeFlags &= ~2U;
    model->specialModeFlags &= ~0x100U;
    model->specialModeFrame = 0;
    model->specialModeState = 0;
    model->padFlags |= 0x200000U;
    switch (mode) {
    default:
        runtime->flags &= ~0x40000U;
        model->flags &= ~0x40000U;

    case 0: {
        model->specialModeFlags |= 0x100U;
        model->specialModeFlags &= ~1U;
        model->specialModeFlags &= ~4U;
        model->specialModeFlags |= 0x2000U;
        std::uint32_t *const movementFlags = reinterpret_cast<std::uint32_t *>(model->movementState);
        if (*movementFlags & 2U)
            *movementFlags &= ~2U;
        break;
    }
    case 1:
        model->specialModeState = 6;
        break;
    case 2:
        model->specialModeState = 6;
        model->specialModeFlags |= 0x20U;
        break;
    case 3:
        runtime->flags |= 0x80000U;
        model->specialModeFlags |= 0x100U;
        break;
    }
    return 0;
}

// @verified - d
std::uint32_t SetPlayerGameFlags(std::uint32_t mode, void *parameter) {
    std::uint32_t result = 0;
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    std::uint32_t mask = 0;
    if (!model)
        return 0;
    switch (mode) {
    case 0:
        SetPlayerControlEnabled(1);
        SetPlayerSpecialMode(-1);
        break;
    case 2:
        mask = 4;
        break;
    case 4:
        mask = 0x20;
        break;
    case 5:
        mask = 0x4000000;
        break;
    case 6:
        mask = 0x1000000;
        break;
    case 7:
        mask = 0x20000000;
        break;
    case 22:
        mask = 0x400000;

    case 23:
        model->secondaryInteractionDistanceLimit = *static_cast<float *>(parameter);
        break;
    case 24:
        *static_cast<float *>(parameter) = model->secondaryInteractionDistanceLimit;
        break;
    case 30:
        shenToggleAreaFlags(0);
        parameter = reinterpret_cast<void *>(2U);
        break;
    case 31:
        shenToggleAreaFlags(1);
        parameter = reinterpret_cast<void *>(2U);
        break;
    case 32:
        result = GetPlayerAreaControlState();
        parameter = reinterpret_cast<void *>(2U);
        break;
    }
    switch (reinterpret_cast<std::intptr_t>(parameter)) {
    case 0:
        runtime->flags &= ~mask;
        break;
    case 1:
        runtime->flags |= mask;
        break;
    case -1:
        result = (runtime->flags & mask) != 0;
        break;
    }
    return result;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t EV_setHoldStatesFlagsEx(std::uint32_t mode) {
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    PlayerModelState *const model = runtime->model;
    if (!model)
        return UINT32_MAX;
    std::uint32_t previous = runtime->flags >> 8;
    previous &= 1U;
    switch (mode) {
    case 1:
    case 3:
        runtime->flags &= ~0x100U;
        break;
    case 0:
    case 2:
        runtime->flags |= 0x100U;
        break;
    case 4:
        runtime->field08 &= ~3;
        model->heldStateFlags &= ~6;
        break;
    case 5:
        runtime->field08 |= 3;
        break;
    case 6:
        model->heldStateFlags &= ~0x1000;
        break;
    case 7:
        model->heldStateFlags |= 0x1000;
        break;
    }
    return previous;
}
#endif

} // namespace s2
} // namespace shenmue

#endif
