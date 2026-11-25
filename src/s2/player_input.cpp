#include "shenmue.h"
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
#include "s2/player_model_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/math.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/camera_sequence_dc.hpp"
#include "s2/camera_conversation_dc.hpp"
#include <cmath>
#include <cstdlib>
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/player_internal.hpp"
#endif

namespace shenmue {

namespace s2 {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
std::int16_t CameraLerpAngle(std::int32_t target, std::int32_t base, float amount);
#endif

namespace player_detail {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
static LegacyInputSlot playerInputSnapshot;

struct PackedStickDirection {
    std::int8_t x, y;
};

static const std::uint32_t directionHoldMasks[4] = {0x1000U, 0x2000U, 0x4000U, 0x8000U};

void plymUpdateDirectionHoldFrames(PlayerModelState &state) {
    const dc::InputSlot *input = state.inputSnapshot;
    const std::uint16_t held = input->held;
    const std::uint16_t pressed = input->pressedPriority;
    for (std::int32_t index = 0; index < 4; ++index) {
        const std::uint32_t mask = directionHoldMasks[index];
        if (pressed & mask)
            state.buttonHoldFrames[index] = 1;
        else if (held & mask)
            ++state.buttonHoldFrames[index];
        else
            state.buttonHoldFrames[index] = 0;
    }
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Native camCtrlGetPitchYawWithOffsetsFrom: the pitch of the player's facing
// over the CCM_ (or authored stair) ground plane.
std::int16_t camCtrlGetPitchYawWithOffsetsFrom(const PlayerModelState &state) {
    Vector3s angles{};
    camCtrlQueryPitchYaw(angles);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(angles.y));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(angles.x));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.currentYaw - angles.y));
    const Vector3 backward{-workTransformMatrix.rows[2].x, -workTransformMatrix.rows[2].y, -workTransformMatrix.rows[2].z};
    Matrix4x4::Pop(1);
    if (std::fabs(backward.x) < 0.00000011920929f && std::fabs(backward.y) < 0.00000011920929f && std::fabs(backward.z) < 0.00000011920929f)
        return angles.x;
    return static_cast<std::int16_t>(FixedAtan2(backward.y, std::sqrt(backward.x * backward.x + backward.z * backward.z)));
}

void SynchronizePlayerSearchTransform(PlayerModelState &state) {
    const std::int16_t pitch = camCtrlGetPitchYawWithOffsetsFrom(state);
    float height = 0.40000001f;
    if (pitch > 910) {
        height -= std::clamp((pitch - 910) / 5801.0f, 0.0f, 1.0f) * 0.2f;
    } else if (pitch < -910) {
        height += std::clamp((-910 - pitch) / 5801.0f, 0.0f, 1.0f) * 0.20000002f;
    }
    state.worldPosition = GetPlayerTargetWorldPosition();
    state.worldPosition.y += height;
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(state.worldPosition.x, state.worldPosition.y, state.worldPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.currentYaw));
    Matrix4x4::GetTransformMatrix(&state.worldTransform);
    Matrix4x4::SetPrePassToTempTransform();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-state.currentYaw));
    Matrix4x4::preTranslate(-state.worldPosition.x, -state.worldPosition.y, -state.worldPosition.z);
    Matrix4x4::GetTransformMatrix(&state.inverseWorldTransform);
    Matrix4x4::Pop(1);
}

// Native trigger byte to 0..255 analog history (scale 1.4088398, bias 90.165749).
std::uint8_t plymTriggerHistoryValue(std::uint8_t trigger) {
    const float value = static_cast<float>(trigger) * 1.4088398f - 90.165749f;
    if (0.0f > value)
        return 0;
    if (value >= 255.0f)
        return 0xff;
    return static_cast<std::uint8_t>(static_cast<std::int32_t>(value));
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
void plymSampleInput(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    state.padFlags &= ~0x1000U;
    state.isLookAround = 0;
    if (IsPadControlEntryBound(GetMapPadControlEntry())) {
        state.padFlags |= 0x1000U;
        state.isLookAround = 0;
    } else if (camCtrlGetState() == 22 && HasScenarioCharacterControl()) {
        state.isLookAround = 1;
    }
    playerInputSnapshot = *ctrlGetPadInputBuffer0(GetMapPadControlEntry());
    dc::InputSlot *input = &playerInputSnapshot;
    state.inputSnapshot = input;

    std::int8_t stickX = static_cast<std::int8_t>(input->leftStickX + 0x80);
    std::int8_t stickY = static_cast<std::int8_t>(input->leftStickY + 0x80);
    state.stickX = stickX;
    state.stickY = stickY;
    const float magnitude = std::sqrt(static_cast<float>(stickX * stickX + stickY * stickY));
    state.stickMagnitude = magnitude;
    if (magnitude > 127.0f)
        state.stickMagnitude = 127.0f;
    if (state.stickMagnitude == 0.0f)
        state.accumulatedMagnitude = 0.0f;
    state.accumulatedMagnitude += state.stickMagnitude;
    if (!state.horizontalHold)
        state.accumulatedStickX = 0;
    if (!state.verticalHold)
        state.accumulatedStickY = 0;
    state.accumulatedStickX += stickX;
    state.accumulatedStickY += stickY;
    state.smoothedStickX = static_cast<std::int8_t>((stickX + state.stickHistoryX + state.stickHistoryX + state.previousStickX) / 4);
    state.smoothedStickY = static_cast<std::int8_t>((stickY + state.stickHistoryY + state.stickHistoryY + state.previousStickY) / 4);
    if ((state.padFlags & 0x1000U) == 0) {
        state.smoothedStickX = 0;
        state.smoothedStickY = 0;
    }
    state.previousPreviousStickX = state.previousStickX;
    state.previousStickX = state.stickHistoryX;
    state.stickHistoryX = stickX;
    state.previousPreviousStickY = state.previousStickY;
    state.previousStickY = state.stickHistoryY;
    state.stickHistoryY = stickY;

    const std::uint16_t previousDirection = state.direction;
    std::uint16_t direction = 0;
    if (stickX <= -64)
        direction = 0x4000U;
    else if (stickX >= 64)
        direction |= 0x8000U;
    if (stickY <= -64)
        direction |= 0x1000U;
    else if (stickY >= 64)
        direction |= 0x2000U;
    state.direction = direction;
    state.risingDirection = direction & ~previousDirection;
    state.fallingDirection = ~direction & previousDirection;
    if ((direction & ~previousDirection) & 0xc000U) {
        state.horizontalHold = 1;
    } else {
        if (state.horizontalHold == 0xffffU)
            --state.horizontalHold;
        if (direction & 0xc000U)
            ++state.horizontalHold;
        else if ((previousDirection & 0xc000U) == 0)
            state.horizontalHold = 0;
    }
    if ((direction & ~previousDirection) & 0x3000U) {
        state.verticalHold = 1;
    } else {
        if (state.verticalHold == 0xffffU)
            --state.verticalHold;
        if (direction & 0x3000U)
            ++state.verticalHold;
        else if ((previousDirection & 0x3000U) == 0)
            state.verticalHold = 0;
    }
    state.inputActive = 0;
    if (stickX || stickY || (state.fallingDirection & 0xf000U))
        state.inputActive = 1;
    if ((state.flags & 8U) == 0)
        state.inputActive = 0;

    // Single back/left/right presses are held back one frame so a diagonal can still form.
    std::uint16_t heldDirection = input->held & 0xf000U;
    switch (state.directionLatchState) {
    case 0:
        if ((input->previousHeld & 0xf000U) == 0 && (heldDirection == 0x4000U || heldDirection == 0x8000U || heldDirection == 0x2000U)) {
            state.directionLatchState = 1;
            state.directionLatchValue = heldDirection;
            heldDirection &= 0x1fffU;
        }
        break;
    case 1:
        if (state.directionLatchValue & 0x4000U) {
            if ((heldDirection & 0x5000U) != 0x5000U && (heldDirection & 0x6000U) != 0x6000U)
                heldDirection = 0x4000U;
        } else if (state.directionLatchValue & 0x8000U) {
            if ((heldDirection & 0x9000U) != 0x9000U && (heldDirection & 0xa000U) != 0xa000U)
                heldDirection = 0x8000U;
        } else if (state.directionLatchValue & 0x2000U) {
            if ((heldDirection & 0x6000U) != 0x6000U && (heldDirection & 0xa000U) != 0xa000U)
                heldDirection = 0x2000U;
        }
        state.directionLatchState = 2;
        state.directionLatchValue = 0;
        break;
    case 2:
        if (heldDirection == 0)
            state.directionLatchState = 0;
        break;
    }
    input->held = (input->held & 0x0fffU) | heldDirection;
    plymUpdateDirectionHoldFrames(state);

    const std::uint16_t held = input->held;
    reinterpret_cast<PackedStickDirection &>(state.packedDirection).x = 0;
    reinterpret_cast<PackedStickDirection &>(state.packedDirection).y = 0;
    if (!state.inputActive) {
        if (held & 0x1000U)
            reinterpret_cast<PackedStickDirection &>(state.packedDirection).y = -0x7f;
        else if (held & 0x2000U)
            reinterpret_cast<PackedStickDirection &>(state.packedDirection).y = 0x7f;
        if (held & 0x4000U)
            reinterpret_cast<PackedStickDirection &>(state.packedDirection).x = -0x7f;
        else if (held & 0x8000U)
            reinterpret_cast<PackedStickDirection &>(state.packedDirection).x = 0x7f;
    } else {
        reinterpret_cast<PackedStickDirection &>(state.packedDirection).x = stickX;
        reinterpret_cast<PackedStickDirection &>(state.packedDirection).y = stickY;
    }

    state.analogHistory2 = state.analogHistory1;
    state.analogHistory1 = state.analogHistory0;
    const std::uint8_t previousLookZoom = state.lookZoomScale;
    if (GetNormalTriggerMapping()) {
        state.analogHistory0 = input->triggers[0];
        state.lookZoomScale = input->triggers[1];
    } else {
        state.analogHistory0 = input->triggers[1];
        state.lookZoomScale = input->triggers[0];
    }
    if (camCtrlIsFlag21Set())
        state.analogHistory0 = 0;
    if (state.padFlags & 0x1000U) {
        if (state.lookZoomScale < previousLookZoom)
            state.lookZoomScale = LerpInteger(state.lookZoomScale, previousLookZoom, 0.4f);
    } else {
        if (state.isLookAround)
            state.lookZoomScale = previousLookZoom;
        if (IsItemSelectable())
            state.lookZoomScale = previousLookZoom;
        if (GetShouldRenderIcons(static_cast<RenderIconSlot>(1)))
            state.lookZoomScale = previousLookZoom;
    }
    state.padFlags &= 0xfdfffff3U;
    if (state.lookZoomScale > 10U)
        state.padFlags |= 4U;
    if (input->held & 0x0200U)
        state.padFlags |= 8U;
    if (input->pressedPriority & 0x0200U)
        state.padFlags |= 0x02000000U;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    using enum CameraState;
    state.padFlags &= ~0x1000U;
    state.isLookAround = 0;
    if ((ctrlGetPadData()->flags & 2U) != 0) {
        state.padFlags |= 0x1000U;
        state.isLookAround = 0;
    } else if (camCtrlGetState() == LookAround && HasScenarioCharacterControl()) {
        state.isLookAround = 1;
    }
    if ((state.padFlags & 0x1000U) != 0 || state.isLookAround != 0) {
        if (++state.lookAroundFrames > 10000)
            state.lookAroundFrames = 10000;
    } else {
        state.lookAroundFrames = 0;
    }
    if ((state.flags & 2U) == 0)
        state.lookAroundFrames = 0;
    playerInputSnapshot = *ctrlGetPadInputBuffer0(ctrlGetPadData());
    state.inputSnapshot = &playerInputSnapshot;

    std::int8_t stickX = static_cast<std::int8_t>(g_inputBuffer[0].analog0e + 0x80U);
    std::int8_t stickY = static_cast<std::int8_t>(g_inputBuffer[0].analog0f + 0x80U);
    if (GetIsKeyboardActive() != 1) {
        if (std::abs(static_cast<std::int32_t>(stickX)) < 40)
            stickX = 0;
        if (std::abs(static_cast<std::int32_t>(stickY)) < 40)
            stickY = 0;
    }
    state.stickX = stickX;
    state.stickY = stickY;
    state.stickMagnitude = std::sqrt(static_cast<float>(stickX * stickX + stickY * stickY));
    if (state.stickMagnitude > 127.0f)
        state.stickMagnitude = 127.0f;
    if (state.stickMagnitude == 0.0f)
        state.accumulatedMagnitude = 0.0f;
    state.accumulatedMagnitude = state.stickMagnitude + state.accumulatedMagnitude;
    if (!state.horizontalHold)
        state.accumulatedStickX = 0;
    if (!state.verticalHold)
        state.accumulatedStickY = 0;
    state.accumulatedStickX = static_cast<std::int16_t>(state.accumulatedStickX + stickX);
    state.accumulatedStickY = static_cast<std::int16_t>(state.accumulatedStickY + stickY);
    state.smoothedStickX = static_cast<std::int8_t>((stickX + state.previousStickX + 2 * state.stickHistoryX) / 4);
    state.smoothedStickY = static_cast<std::int8_t>((stickY + state.previousStickY + 2 * state.stickHistoryY) / 4);
    if ((state.padFlags & 0x1000U) == 0) {
        state.smoothedStickX = 0;
        state.smoothedStickY = 0;
    }
    state.previousPreviousStickX = state.previousStickX;
    state.previousStickX = state.stickHistoryX;
    state.previousPreviousStickY = state.previousStickY;
    state.previousStickY = state.stickHistoryY;
    state.stickHistoryX = stickX;
    state.stickHistoryY = stickY;

    const std::uint16_t previousDirection = state.direction;
    std::uint16_t direction = 0;
    if (stickX <= -64)
        direction = 0x4000U;
    else if (stickX >= 64)
        direction = 0x8000U;
    if (stickY <= -64)
        direction |= 0x1000U;
    else if (stickY >= 64)
        direction |= 0x2000U;
    state.direction = direction;
    state.risingDirection = direction & ~previousDirection;
    state.fallingDirection = previousDirection & ~direction;
    if ((state.risingDirection & 0xc000U) != 0) {
        state.horizontalHold = 1;
    } else {
        if (state.horizontalHold == 0xffffU)
            state.horizontalHold = 0xfffeU;
        if ((direction & 0xc000U) != 0)
            ++state.horizontalHold;
        else if ((previousDirection & 0xc000U) == 0)
            state.horizontalHold = 0;
    }
    if ((state.risingDirection & 0x3000U) != 0) {
        state.verticalHold = 1;
    } else {
        if (state.verticalHold == 0xffffU)
            state.verticalHold = 0xfffeU;
        if ((direction & 0x3000U) != 0)
            ++state.verticalHold;
        else if ((previousDirection & 0x3000U) == 0)
            state.verticalHold = 0;
    }
    state.inputActive = stickX || stickY || (state.fallingDirection & 0xf000U) ? 1U : 0U;
    if ((state.flags & 8U) == 0)
        state.inputActive = 0;

    // Single back/left/right presses are held back one frame so a diagonal can still form.
    std::uint16_t heldDirection = playerInputSnapshot.held & 0xf000U;
    if (state.directionLatchState == 0) {
        if ((playerInputSnapshot.previousHeld & 0xf000U) == 0 && ((heldDirection - 0x2000U) & 0x9fffU) == 0 && heldDirection != 0x6000U) {
            state.directionLatchValue = heldDirection;
            heldDirection &= 0x1fffU;
            state.directionLatchState = 1;
        }
    } else if (state.directionLatchState == 1) {
        const std::uint16_t latched = state.directionLatchValue;
        if ((latched & 0x4000U) != 0) {
            if ((heldDirection & 0x6000U) != 0x6000U && (heldDirection & 0x5000U) != 0x5000U)
                heldDirection = 0x4000U;
        } else if ((latched & 0x8000U) != 0) {
            if ((heldDirection & 0xa000U) != 0xa000U && (heldDirection & 0x9000U) != 0x9000U)
                heldDirection = 0x8000U;
        } else if ((latched & 0x2000U) != 0) {
            if ((heldDirection & 0x6000U) != 0x6000U && (heldDirection & 0xa000U) != 0xa000U)
                heldDirection = 0x2000U;
        }
        state.directionLatchState = 2;
        state.directionLatchValue = 0;
    } else if (state.directionLatchState == 2 && heldDirection == 0) {
        state.directionLatchState = 0;
    }
    playerInputSnapshot.held = static_cast<std::uint16_t>(heldDirection | (playerInputSnapshot.held & 0x0fffU));

    constexpr std::array<std::uint32_t, 4> directionMasks{0x1000U, 0x2000U, 0x4000U, 0x8000U};
    for (std::size_t index = 0; index < directionMasks.size(); ++index) {
        if (playerInputSnapshot.pressedPriority & directionMasks[index])
            state.buttonHoldFrames[index] = 1;
        else if (playerInputSnapshot.held & directionMasks[index])
            ++state.buttonHoldFrames[index];
        else
            state.buttonHoldFrames[index] = 0;
    }

    const std::uint16_t held = playerInputSnapshot.held;
    state.packedDirection = 0;
    if (!IsGameUIActive() && !IsItemSelectable()) {
        if (state.inputActive) {
            state.packedDirection = static_cast<std::uint16_t>(static_cast<std::uint8_t>(stickX) | (static_cast<std::uint8_t>(stickY) << 8U));
        } else {
            if (held & 0x1000U)
                state.packedDirection = 0x8100U;
            else if (held & 0x2000U)
                state.packedDirection = 0x7f00U;
            if (held & 0x4000U)
                state.packedDirection = static_cast<std::uint16_t>((state.packedDirection & 0xff00U) | 0x0081U);
            else if (held & 0x8000U)
                state.packedDirection = static_cast<std::uint16_t>((state.packedDirection & 0xff00U) | 0x007fU);
        }
    }

    const std::uint8_t previousLookZoom = state.lookZoomInput;
    state.analogHistory2 = state.analogHistory1;
    state.analogHistory1 = state.analogHistory0;
    const bool triggersSwapped = GetRuntimeUserConfiguration().swapTriggers != 0;
    state.analogHistory0 = plymTriggerHistoryValue(triggersSwapped ? playerInputSnapshot.analog0d : playerInputSnapshot.analog0c);
    state.lookZoomInput = plymTriggerHistoryValue(triggersSwapped ? playerInputSnapshot.analog0c : playerInputSnapshot.analog0d);
    if ((state.flags & 2U) == 0) {
        state.analogHistory0 = 0;
        state.lookZoomInput = 0;
    }
    if (camCtrlGetContextVal() != 0 && (state.flags & 0x40000000U) == 0)
        state.analogHistory0 = 0;
    if ((state.padFlags & 0x1000U) != 0) {
        if (state.lookZoomInput < previousLookZoom) {
            const float step = shenGetScaledDeltaTickTime(0.40000001f);
            const float base = static_cast<float>(previousLookZoom);
            state.lookZoomInput = static_cast<std::uint8_t>(static_cast<std::int32_t>((static_cast<float>(state.lookZoomInput) - base) * step + base));
        }
    } else {
        if (state.isLookAround != 0)
            state.lookZoomInput = previousLookZoom;
        if (IsItemSelectable())
            state.lookZoomInput = previousLookZoom;
        if (GetShouldRenderIcons(RenderIconSlot::ItemSelect))
            state.lookZoomInput = previousLookZoom;
    }
    state.padFlags &= 0xfdfffff3U;
    if (state.lookZoomInput > 10U)
        state.padFlags |= 4U;
    if (playerInputSnapshot.held & 0x0200U)
        state.padFlags |= 8U;
    if (playerInputSnapshot.pressedPriority & 0x0200U)
        state.padFlags |= 0x02000000U;
#endif
}
#endif

} // namespace player_detail

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
float ctrlSetLookOverlayWeight(float alpha) {
    PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model)
        return reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo->alpha;
    const float previous = model->character->alpha;
    if (alpha < 0.0f)
        model->character->alpha = -alpha;
    else
        model->savedDisplayAlpha = alpha;
    model->padFlags |= 0x8000U;
    return previous;
}


std::int32_t plymIsLookAllowed(std::uint32_t modes) {
    const PlayerModelState *const model = g_playerRuntimePointer->model;
    std::int32_t allowed = 0;
    if (!model)
        return 0;
    const PlayerMovementState *movement = model->movementState;
    if ((modes & 1U) && movement->stationaryFrames > 0)
        allowed = 1;
    return allowed;
}


std::int32_t plymIsSpecialModeCameraRequested() {
    const PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    const PlayerModelState *const model = runtime->model;
    if (!(runtime->flags & 0x40000U))
        return 0;
    if (!model->specialModeTargetId)
        return 0;
    if (!(model->specialModeFlags & 4U))
        return 0;
    if (static_cast<std::int8_t>(model->specialModeAuxState) == 2)
        return 1;
    return 0;
}


// @verified - d
bool plymSampleLookInput(Vector2s &facing, Vector2s &look) {
    const PlayerModelState *const model = g_playerRuntimePointer->model;
    facing.x = 0;
    facing.y = static_cast<std::int16_t>(plymGetAngY());
    look.x = 0;
    look.y = 0;
    if (!model)
        return false;
    if (!model->lookAroundState)
        return false;
    facing.x = model->lookBasePitch;
    facing.y = model->lookBaseYaw;
    look.x = model->lookPitchDelta;
    look.y = model->lookYawDelta;
    return true;
}


std::int32_t plymResetLookAngles() {
    PlayerModelState *model = g_playerRuntimePointer->model;
    if (!model)
        return -1;
    model->lookBasePitch = 0;
    model->lookBaseYaw = 0;
    model->lookPitchDelta = 0;
    model->lookYawDelta = 0;
    return 0;
}


// @verified - d
std::int32_t plymGetLookInputState() {
    const PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model)
        return 0;
    if (!model->lookAroundState)
        return (model->padFlags & 0x200U) ? -1 : 0;
    return (model->lookInputFlags & 1U) ? 2 : 1;
}


void ccmGetGroundAngles(Character *character, std::int16_t *angles);
void GetTransformedForwardVector(Vector3 *direction);

// @verified - d
void camCtrlQueryPitchYaw(Vector3s &angles) {
    const PlayerModelState *const model = g_playerRuntimePointer->model;
    if (!model)
        return;
    ccmGetGroundAngles(model->character, &angles.x);
    const PlayerMovementState *const movement = reinterpret_cast<const PlayerMovementState *>(&model->retainedSecondaryController);
    if (movement->flags & 8U) {
        Vector3 local, position;
        GetCharacterWorldPosition(model->character, &position);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(-movement->targetWorldYaw);
        Matrix4x4::preTranslate(-movement->targetWorldPosition.x, -movement->targetWorldPosition.y, -movement->targetWorldPosition.z);
        Matrix4x4::TransformPoint(position, local);
        PopCurrentMatrix(1);
        local.z = -local.z;
        if (!(0.0f > local.z) && !(local.z > static_cast<float>(movement->targetHeightScale - 1) * 0.30000001f)) {
            angles.x = static_cast<std::int16_t>(dc::sdk::ArcTanFixed(0.74999994f));
            angles.y = movement->targetWorldYaw;
        }
    }
    angles.x = static_cast<std::int16_t>(-angles.x);
}


std::int16_t CopyHeading(const Vector3 &from, const Vector3 &to);
#endif

namespace player_detail {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymComputeTurnInput(PlayerModelState &state) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    const std::int8_t *const direction = reinterpret_cast<const std::int8_t *>(&state.packedDirection);
    const float x = static_cast<float>(-direction[0]);
    const float y = static_cast<float>(direction[1]);
    const std::int16_t camera = static_cast<std::int16_t>(camCtrlGetNextRotY());
    std::int16_t turn;
    if (!(std::fabs(y) < 0.001f && std::fabs(x) < 0.001f))
        turn = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(y, x) + 0x4000);
    else
        turn = 0;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const float x = -static_cast<float>(static_cast<std::int8_t>(state.packedDirection & 0xffU));
    const float y = static_cast<float>(static_cast<std::int8_t>(state.packedDirection >> 8U));
    const std::int16_t cameraYaw = static_cast<std::int16_t>(camCtrlGetNextRotY());
    std::int16_t turn = 0;
    if (std::fabs(y) >= 0.001f || std::fabs(x) >= 0.001f)
        turn = static_cast<std::int16_t>(FixedAtan2(y, x) + 0x4000);
#endif
    state.turnInput = turn;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    state.inputTargetYaw = static_cast<std::int16_t>(turn + camera);
    state.inputBaseYaw = camera;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    state.inputTargetYaw = static_cast<std::int16_t>(cameraYaw + turn);
    state.inputBaseYaw = cameraYaw;
#endif
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uint8_t Abs8(std::int8_t value) {
    return static_cast<std::uint8_t>(value < 0 ? -value : value);
}

// rootYawState 1, 2 and 6 are the moving root classes the look owner defers to.
bool IsLookMovingRootState(std::uint32_t rootYawState) {
    return ((rootYawState - 1U) & 0xfffffffaU) == 0 && rootYawState != 5U;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymUpdateLookAround(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    const LegacyInputSlot *const input = state.inputSnapshot;
    std::int8_t x, y;
    std::int32_t directions;
    std::int16_t pitch, yaw;
    std::int32_t request = 0, movementRequests = 0, publish = 0;
    const std::int32_t camera = camCtrlGetState();
    if (camCtrlGetState() == 24)
        return;
    if (camera != 0 && camera != 22)
        return;
    directions = input->held & 0xf000U;
    x = state.smoothedStickX;
    y = state.smoothedStickY;
    if ((state.padFlags & 0x4000U) && state.motionState != 9)
        state.padFlags &= ~0x4000U;
    if (static_cast<std::int8_t>(state.motionInputState) == 10 && (state.padFlags & 0x80000U))
        directions &= 0xfffU;
    if (state.padFlags & 4U) {
        request = 1;
        state.padFlags |= 0x10000U;
    } else if (state.padFlags & 8U) {
        if ((state.padFlags & 0x10000U) || directions || x || y)
            request = 1;
        if (static_cast<std::int8_t>(state.motionInputState) == 10 && (state.padFlags & 0x80000U))
            request = 0;
    } else if (!(state.flags & 8U)) {
        if (x || y)
            request = 2;
        if (request)
            directions &= 0xfffU;
        if (state.lookAroundState && !(state.padFlags & 0x4000U))
            state.lookBaseYaw = CameraLerpAngle(state.yaw, state.lookBaseYaw, 0.2f);
        if (camCtrlIsLookAroundActive())
            request = 0;
    }
    if (state.flags & 0x80U)
        request = 0;
    if (!request && !(state.padFlags & 8U) &&
        ((*reinterpret_cast<const std::uint32_t *>(&state.packedDirection) & 0xffffU) || static_cast<std::int8_t>(state.analogHistory0) < 0 ||
            (state.analogHistory0 && state.analogHistory1)))
        movementRequests = 1;
    switch (static_cast<std::int8_t>(state.lookAroundState)) {
    case 0:
        if (!request) {
            state.lookPitchDelta = CameraLerpAngle(0, state.lookPitchDelta, 0.08f);
            state.lookYawDelta = CameraLerpAngle(0, state.lookYawDelta, 0.08f);
            if (std::abs(static_cast<std::int32_t>(state.lookPitchDelta)) < 64 && std::abs(static_cast<std::int32_t>(state.lookYawDelta)) < 64) {
                state.lookPitchDelta = 0;
                state.lookYawDelta = 0;
            }
            if (camCtrlGetState() == 22) {
                camCtrlSetState(0);
                camCtrlSetInterpolationMode(7);
                camCtrlSetInterpolationSpeed(0.15f);
            }
            break;
        }
        if (((state.flags & 8U) || request != 2) && state.motionState)
            break;
        state.lookAroundState = 1;
        state.lookBaseYaw = state.yaw;
        state.lookBasePitch = 0;
        {
            std::uint16_t cameraPitch[2];
            camCtrlGetAdjustedPitchOrYaw(0, cameraPitch);
            state.lookPitchDelta = static_cast<std::int16_t>(cameraPitch[0]);
        }
        state.padFlags |= 0x10000U;
    case 1:
        if (camCtrlGetState() != 22) {
            camCtrlSetState(22);
            camCtrlSetInterpolationMode(7);
            camCtrlSetInterpolationSpeed(0.3f);
        }
        state.lookAroundState = 2;
        state.lookInputFlags = 0;
        state.lookInputGraceFrames = 2;
    case 2:
        if (state.isLookAround)
            break;
        publish = 1;
        if (static_cast<std::int8_t>(state.lookInputGraceFrames) > 0)
            --state.lookInputGraceFrames;
        if (directions && request) {
            state.lookInputGraceFrames = 2;
            pitch = state.lookPitchDelta;
            yaw = state.lookYawDelta;
            if (directions & 0x4000U)
                yaw = static_cast<std::int16_t>(yaw + 409);
            if (directions & 0x8000U)
                yaw = static_cast<std::int16_t>(yaw - 409);
            if (directions & 0x1000U)
                pitch = static_cast<std::int16_t>(pitch + 273);
            if (directions & 0x2000U)
                pitch = static_cast<std::int16_t>(pitch - 273);
            state.lookInputFlags = 1;
            if (state.padFlags & 4U)
                state.lookInputFlags = 5;
            pitch = static_cast<std::int16_t>((pitch < -0x2aaa ? -0x2aaa : pitch) < 0x2aaa ? (pitch < -0x2aaa ? -0x2aaa : pitch) : 0x2aaa);
            state.lookPitchDelta = pitch;
            state.lookYawDelta = yaw;
        } else if ((x || y) && request) {
            state.lookInputGraceFrames = 2;
            if (static_cast<std::int8_t>(state.lookInputFlags) == 5) {
                state.lookInputGraceFrames = 0;
            } else {
                pitch = static_cast<std::int16_t>(static_cast<std::int32_t>(-(static_cast<float>(y) * 0.0078125f * 10922.0f)));
                yaw = static_cast<std::int16_t>(static_cast<std::int32_t>(-(static_cast<float>(x) * 0.0078125f * 16384.0f)));
                if (x || y) {
                    state.lookInputFlags = 2;
                    state.lookPitchDelta = CameraLerpAngle(pitch, state.lookPitchDelta, 0.06f);
                    state.lookYawDelta = CameraLerpAngle(yaw, state.lookYawDelta, 0.06f);
                }
            }
        } else if (state.lookInputFlags & 1U) {
            state.lookInputGraceFrames = 0;
        } else if (state.lookInputFlags & 2U) {
            if (request)
                state.lookInputGraceFrames = 2;
            state.lookPitchDelta = CameraLerpAngle(0, state.lookPitchDelta, 0.06f);
            state.lookYawDelta = CameraLerpAngle(0, state.lookYawDelta, 0.06f);
        } else if (request) {
            state.lookInputGraceFrames = 2;
        }
        if ((state.padFlags & 0x4000U) && state.motionState == 9) {
            if (std::abs(static_cast<std::int32_t>(state.lookYawDelta)) > 910 || !movementRequests)
                break;
            ++movementRequests;
            state.forcedYaw = static_cast<std::uint16_t>(state.lookBaseYaw + state.lookYawDelta);
            break;
        }
        if (static_cast<std::int8_t>(state.lookInputGraceFrames) > 0)
            break;
        state.lookAroundState = 3;
    case 3:
        publish = 1;
        state.lookAroundState = 0;
        if ((state.lookInputFlags & 1U) && std::abs(static_cast<std::int32_t>(state.lookYawDelta)) > 546)
            state.lookAroundState = 4;
        if (!state.lookAroundState) {
            if ((state.lookInputFlags & 1U) && movementRequests) {
                ++movementRequests;
                state.forcedYaw = static_cast<std::uint16_t>(state.lookBaseYaw + state.lookYawDelta);
                break;
            }
            if (request)
                state.lookAroundState = 1;
            break;
        }
        state.lookBaseYaw = static_cast<std::int16_t>(state.lookBaseYaw + state.lookYawDelta);
        state.lookYawDelta = 0;
    case 4:
        state.padFlags |= 0x4000U;
        publish = 1;
        if (movementRequests) {
            ++movementRequests;
            state.forcedYaw = static_cast<std::uint16_t>(state.lookBaseYaw + state.lookYawDelta);
            break;
        }
        if (static_cast<std::int8_t>(state.motionInputState) == 10 && (state.padFlags & 0x80000U))
            state.lookAroundState = 0;
        else if (state.motionState != 9) {
            state.padFlags |= 0x1000000U;
            state.forcedYaw = static_cast<std::uint16_t>(state.lookBaseYaw);
            state.movementSelectionFlags |= 0x40000000U;
        } else {
            state.lookAroundState = 5;
        }
        break;
    case 5:
        state.padFlags |= 0x4000U;
        publish = 1;
        if (movementRequests) {
            ++movementRequests;
            state.forcedYaw = static_cast<std::uint16_t>(state.lookBaseYaw + state.lookYawDelta);
            break;
        }
        if (state.motionState != 9)
            state.lookAroundState = 0;
        if (request)
            state.lookAroundState = 1;
        else {
            state.lookPitchDelta = CameraLerpAngle(0, state.lookPitchDelta, 0.08f);
            state.lookYawDelta = CameraLerpAngle(0, state.lookYawDelta, 0.08f);
        }
        break;
    default:
        break;
    }
    if (movementRequests > 1) {
        state.lookAroundState = 0;
        state.padFlags |= 0x200U;
        state.padFlags |= 0x8000000U;
        state.lookPitchDelta = 0;
        state.lookYawDelta = 0;
    }
    if ((state.padFlags & 0x200U) && (state.movementSelectionFlags & 1U) == 0)
        publish = 1;
    if (camCtrlGetState() != 22 && state.lookAroundState)
        state.lookAroundState = 0;
    if (publish) {
        yaw = static_cast<std::int16_t>(state.lookBaseYaw + state.lookYawDelta);
        const std::int16_t lookPitch = static_cast<std::int16_t>(state.lookBasePitch + state.lookPitchDelta);
        if (state.padFlags & 0x200U) {
            yaw = static_cast<std::int16_t>(state.forcedYaw);
            state.lookTargetMode = 10;
            state.lookTargetFlags |= 1U;
        } else if (state.motionState == 9) {
            state.lookTargetMode = 5;
        }
        Vector3 head;
        charaGetMotWorldPos(state.character, 6, &head);
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&head);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        Matrix4x4::RotateAroundX_Wrapper(lookPitch);
        Matrix4x4::preTranslate(0.0f, 0.0f, -100.0f);
        matrix_stack::GetTranslation(&state.lookAtPosition);
        PopCurrentMatrix(1);
        state.padFlags |= 0x100U;
    }
    if (!publish && !(state.padFlags & 0x10000U) && state.motionState == 2 && state.previousRootYawState == 9 && mtwkSetExtraFlag(state.motion, 59, 1)) {
        state.padFlags |= 0x100U;
        state.lookTargetMode = 0xffU;
        state.lookTargetFlags |= 2U;
    }
    if (!state.lookAroundState)
        state.padFlags &= ~0x10000U;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    using enum CameraState;
    const LegacyInputSlot *const input = state.inputSnapshot;
    std::int32_t request = 0;
    std::int32_t lookKind = 0;
    std::int32_t movementRequests = 0;
    bool publish = false;
    const CameraState cameraState = camCtrlGetState();
    if (camCtrlGetState() == LockedOn)
        return;
    const std::uint32_t controlFlags = state.flags;
    if ((controlFlags & 2U) == 0 || (controlFlags & 0x40000U) != 0 || (cameraState != Init && cameraState != LookAround))
        return;
    std::int8_t stickX = state.smoothedStickX;
    auto directions = static_cast<std::uint16_t>(input->held & 0xf000U);
    std::int8_t stickY = state.smoothedStickY;
    if (Abs8(stickX) < 8U)
        stickX = 0;
    if (Abs8(state.smoothedStickY) < 8U)
        stickY = 0;
    if ((state.padFlags & 0x4000U) && state.rootYawState != 9U)
        state.padFlags &= ~0x4000U;
    if (state.motionInputState == 10U && (state.padFlags & 0x80000U))
        directions &= 0xfffU;
    if (state.placementTargetState == 1U && (state.padFlags & 0x80000U))
        directions = 0;
    const std::uint32_t pad = state.padFlags;
    if (pad & 4U) {
        lookKind = 1;
        state.padFlags = pad | 0x10000U;
    } else if (pad & 8U) {
        if (pad & 0x10000U) {
            lookKind = 1;
        } else {
            if (directions || stickX || stickY)
                lookKind = 1;
            if ((controlFlags & 8U) && state.lookAroundFrames < 5)
                lookKind = 0;
        }
    } else if ((controlFlags & 8U) == 0) {
        if (stickX || stickY)
            lookKind = 2;
        if (!lookKind)
            request = directions;
        if ((state.lookAroundState || cameraState == LookAround) && (pad & 0x4000U) == 0)
            state.lookBaseYaw = LerpFixedAngle(static_cast<std::int16_t>(state.yaw), state.lookBaseYaw, ScaledDelta(1.0f));
        directions = static_cast<std::uint16_t>(request);
        if (g_cameraControl->state == Init && g_cameraControl->collisionPhase == 1)
            lookKind = 0;
        request = 0;
    }
    if (state.placementTargetState == 1U && (state.padFlags & 0x80000U)) {
        if (lookKind)
            lookKind = 2;
        if (state.lookAroundState && (state.padFlags & 0x4000U) == 0)
            state.lookBaseYaw = LerpFixedAngle(static_cast<std::int16_t>(state.yaw), state.lookBaseYaw, ScaledDelta(0.2f));
    }
    if ((state.flags & 0x80U) == 0)
        request = lookKind;
    if (!request && (state.padFlags & 8U) == 0) {
        if (state.packedDirection || state.analogHistory0 >= 0x80U || (state.analogHistory0 && state.analogHistory1))
            movementRequests = 1;
    }
    bool digitalLook = false;
    switch (state.lookAroundState) {
    case lookAroundInactive:
        if (!request || (((state.flags & 8U) != 0 || request != 2) && state.rootYawState))
            goto finishRequests;
        state.lookBaseYaw = static_cast<std::int16_t>(state.yaw);
        state.lookBasePitch = 0;
        state.lookAroundState = lookAroundEntering;
        EulerRotation16 cameraRotation;
        camCtrlGetRot(&cameraRotation);
        state.lookPitchDelta = static_cast<std::int16_t>(cameraRotation.pitch - state.lookBasePitch);
        state.padFlags |= 0x10000U;
        state.lookYawDelta = static_cast<std::int16_t>(cameraRotation.yaw - state.lookBaseYaw);
        [[fallthrough]];
    case lookAroundEntering:
        if (camCtrlGetState() != LookAround) {
            camCtrlSetState(LookAround);
            camCtrlResetTransition(CameraInterpolationMode::ToTargetContinuous);
            camCtrlSetInterpEaseFactor(0.30000001f);
        }
        state.lookAroundState = lookAroundActive;
        state.lookInputGraceFrames = 2;
        state.lookInputFlags = 0;
        [[fallthrough]];
    case lookAroundActive:
        if (state.isLookAround)
            goto finishRequests;
        publish = true;
        if (state.lookInputGraceFrames > 0)
            --state.lookInputGraceFrames;
        if (directions && request && !IsLookMovingRootState(state.rootYawState)) {
            std::int16_t pitch = state.lookPitchDelta;
            std::int16_t yaw = state.lookYawDelta;
            state.lookInputGraceFrames = 2;
            if (directions & 0x4000U)
                yaw = static_cast<std::int16_t>(yaw + 409);
            if (directions & 0x8000U)
                yaw = static_cast<std::int16_t>(yaw - 409);
            if (directions & 0x1000U)
                pitch = static_cast<std::int16_t>(pitch + 273);
            if (directions & 0x2000U)
                pitch = static_cast<std::int16_t>(pitch - 273);
            state.lookInputFlags = (state.padFlags & 4U) ? lookInputDigitalDirections | lookInputZoom : lookInputDigitalDirections;
            if (pitch < -8192)
                pitch = -8192;
            else if (pitch >= 0x2000)
                pitch = 0x2000;
            state.lookPitchDelta = pitch;
            state.lookYawDelta = yaw;
        } else if ((stickX || stickY) && request) {
            state.lookInputGraceFrames = 2;
            if (state.lookInputFlags != (lookInputDigitalDirections | lookInputZoom)) {
                state.lookInputFlags = lookInputAnalogStick;
                const float rate = GetIsKeyboardActive() == 1 && UpdateAndGetMouseIsMoving() && g_mouseLookInputActive ? 0.2f : 0.059999999f;
                state.lookPitchDelta = LerpFixedAngle(
                    static_cast<std::int16_t>(static_cast<std::int32_t>(-(stickY * 0.0078125 * 8192.0))), state.lookPitchDelta, ScaledDelta(rate));
                state.lookYawDelta = LerpFixedAngle(
                    static_cast<std::int16_t>(static_cast<std::int32_t>(-(stickX * 0.0078125 * 16384.0))), state.lookYawDelta, ScaledDelta(rate));
            } else if (!IsLookMovingRootState(state.rootYawState)) {
                state.lookInputGraceFrames = 0;
            }
        } else if ((state.lookInputFlags & lookInputDigitalDirections) == 0) {
            if (request)
                state.lookInputGraceFrames = 2;
            if ((state.lookInputFlags & lookInputAnalogStick) || (state.placementTargetState == 1U && (state.padFlags & 0x80000U))) {
                state.lookPitchDelta = LerpFixedAngle(0, state.lookPitchDelta, ScaledDelta(0.059999999f));
                state.lookYawDelta = LerpFixedAngle(0, state.lookYawDelta, ScaledDelta(0.059999999f));
            }
        } else if (((state.rootYawState - 1U) & 0xfffffffaU) == 0) {
            if (state.rootYawState == 5U)
                state.lookInputGraceFrames = 0;
        } else {
            state.lookInputGraceFrames = 0;
        }
        if ((state.padFlags & 0x4000U) && state.rootYawState == 9U) {
            if (Abs16(state.lookYawDelta) > 910U || !movementRequests)
                goto finishRequests;
            ++movementRequests;
            state.forcedYaw = static_cast<std::uint16_t>(state.lookYawDelta + state.lookBaseYaw);
            goto finishRequests;
        }
        if (state.lookInputGraceFrames > 0)
            goto finishRequests;
        state.lookAroundState = lookAroundLeaving;
        [[fallthrough]];
    case lookAroundLeaving:
        publish = true;
        state.lookAroundState = lookAroundInactive;
        digitalLook = (state.lookInputFlags & lookInputDigitalDirections) != 0;
        if (digitalLook && Abs16(state.lookYawDelta) > 546U && !IsLookMovingRootState(state.rootYawState))
            state.lookAroundState = lookAroundCommitYawAndLeave;
        if (!state.lookAroundState) {
            if (!digitalLook || !movementRequests) {
                if (request)
                    state.lookAroundState = lookAroundEntering;
                goto finishRequests;
            }
            goto commitForcedYaw;
        }
        state.lookBaseYaw = static_cast<std::int16_t>(state.lookBaseYaw + state.lookYawDelta);
        state.lookYawDelta = 0;
        [[fallthrough]];
    case lookAroundCommitYawAndLeave:
        state.padFlags |= 0x4000U;
        publish = true;
        if (movementRequests)
            goto commitForcedYaw;
        if (state.placementTargetState == 1U && (state.padFlags & 0x80000U))
            goto clearState;
        if (state.rootYawState == 9U) {
            state.lookAroundState = lookAroundRootYawRecenter;
        } else {
            state.movementSelectionFlags |= 0x40000000U;
            state.padFlags |= plymPadForceMotionState2;
            state.forcedYaw = static_cast<std::uint16_t>(state.lookBaseYaw);
        }
        goto finishState;
    case lookAroundRootYawRecenter:
        publish = true;
        state.padFlags |= 0x4000U;
        if (movementRequests)
            goto commitForcedYaw;
        if (state.rootYawState != 9U)
            state.lookAroundState = lookAroundInactive;
        if (request) {
            state.lookAroundState = lookAroundEntering;
        } else {
            state.lookPitchDelta = LerpFixedAngle(0, state.lookPitchDelta, ScaledDelta(0.079999998f));
            state.lookYawDelta = LerpFixedAngle(0, state.lookYawDelta, ScaledDelta(0.079999998f));
        }
        goto finishState;
    default:
        goto finishRequests;
    }
commitForcedYaw:
    ++movementRequests;
    state.forcedYaw = static_cast<std::uint16_t>(state.lookYawDelta + state.lookBaseYaw);
finishRequests:
    if (movementRequests <= 1)
        goto finishState;
    state.padFlags |= 0x8000200U;
    state.lookPitchDelta = 0;
    state.lookYawDelta = 0;
clearState:
    state.lookAroundState = lookAroundInactive;
finishState:
    if ((state.padFlags & 0x200U) && (state.movementSelectionFlags & 1U) == 0)
        publish = true;
    if (camCtrlGetState() != LookAround && state.lookAroundState)
        state.lookAroundState = lookAroundInactive;
    if (publish) {
        std::int16_t yaw = static_cast<std::int16_t>(state.lookYawDelta + state.lookBaseYaw);
        const auto pitch = static_cast<std::uint16_t>(state.lookPitchDelta + state.lookBasePitch);
        if (state.padFlags & 0x200U) {
            yaw = static_cast<std::int16_t>(state.forcedYaw);
            state.lookTargetFlags |= 1U;
            state.lookTargetMode = lookTargetForcedYaw;
        } else if (state.rootYawState == 9U) {
            state.lookTargetMode = lookTargetSpecialFast;
        }
        Vector3 head{};
        charaGetMotWorldPos(state.characterNode, 6, &head);
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(head.x, head.y, head.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
        Matrix4x4::RotateAroundX_Wrapper(pitch);
        Matrix4x4::preTranslate(0.0f, 0.0f, -100.0f);
        state.lookAtPosition = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        state.padFlags |= 0x100U;
    } else if ((state.padFlags & 0x10000U) == 0 && state.rootYawState == 2U && state.previousRootYawState == 9U && mtwkSetExtraFlag(state.motionWork, 59U, 1)) {
        state.padFlags |= 0x100U;
        state.lookTargetFlags |= 2U;
        state.lookTargetMode = lookTargetCancel;
    }
    if (state.lookAroundState == lookAroundInactive)
        state.padFlags &= ~0x10000U;
#endif
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
std::int32_t rotationOnlyPullThreshold = 120;
std::int32_t largeTurnInputFrames;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymUpdateCameraInput(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    state.retainedMovementMode = state.currentMovementMode;
    state.currentMovementMode = 1;
    state.movementInterpolation = 0.0f;
    state.motionInputEcho = 0.0f;
    state.movementBlendLimit = 1.0f;
    state.activeMovementMode = 0;
    state.padFlags &= ~0x1000000U;
    if (state.padFlags & 0x200U) {
        if (state.padFlags & 0x8000000U) {
            if (static_cast<std::uint32_t>(state.motionState - 1) <= 1U)
                state.padFlags &= ~0x8000000U;
            if (state.retainedMovementMode == 1)
                state.padFlags &= ~0x8000000U;
            if (state.retainedMovementMode - 3U <= 1U && state.movementSpeedRatio < 0.001f)
                state.padFlags &= ~0x8000000U;
        }
        if (!(state.padFlags & 0x8000000U) && state.motionState != 1 && (state.motionState != 2 || !state.movementState->motionTransitionParameter) &&
            state.motionState != 9)
            state.padFlags &= ~0x200U;
    }
    plymComputeTurnInput(state);
    if (camCtrlGetState() == 24 && static_cast<std::int8_t>(state.lookAroundState) == 2) {
        std::uint16_t pitch[2];
        camCtrlGetAdjustedPitchOrYaw(0, pitch);
        state.lookPitchDelta = static_cast<std::int16_t>(pitch[0]);
        state.lookYawDelta = static_cast<std::int16_t>(camCtrlGetNextRotY() - state.yaw);
        state.lookBaseYaw = state.yaw;
        state.lookBasePitch = 0;
    }
    if (camCtrlGetState() == 24 && !(state.flags & 0x8000000U))
        return;
    if (!(state.flags & 0x8000000U))
        plymUpdateLookAround(state);
    if (state.flags & 8U) {
        if (state.lookAroundState && !(state.flags & 0x8000000U))
            return;
    } else if (state.lookAroundState && !(state.lookInputFlags & 2U) && !(state.flags & 0x8000000U))
        return;
    if (((state.padFlags & 4U) && !(state.flags & 0x8000000U)) || (state.padFlags & 8U))
        return;
    if (state.analogHistory0 >= 0x80U)
        state.movementInterpolation = static_cast<float>(state.analogHistory0 - 128) / 127.0f;
    if (static_cast<std::int8_t>(state.inputActive) == 1) {
        float x = static_cast<float>(state.stickX) / 127.0f, y = static_cast<float>(state.stickY) / 127.0f;
        x = (x < -1.0f ? -1.0f : x) < 1.0f ? (x < -1.0f ? -1.0f : x) : 1.0f;
        y = (y < -1.0f ? -1.0f : y) < 1.0f ? (y < -1.0f ? -1.0f : y) : 1.0f;
        float magnitude = std::sqrt(x * x + y * y);
        magnitude = (magnitude < 0.0f ? 0.0f : magnitude) < 1.0f ? (magnitude < 0.0f ? 0.0f : magnitude) : 1.0f;
        if (magnitude > 0.96875f) {
            const float interpolation = 1.0f - (1.0f - magnitude) * 32.0f;
            state.movementInterpolation = interpolation;
        }
        if (state.stickY > 90)
            state.movementInterpolation = 0.0f;
    }
    state.motionInputEcho = state.movementInterpolation;
    if (static_cast<std::int8_t>(state.inputActive) == 1 && (state.turnInput < 0 ? -state.turnInput : state.turnInput) > 0x7800) {
        state.turnInput = 0;
        if (reinterpret_cast<const std::int8_t *>(&state.packedDirection)[1] >= rotationOnlyPullThreshold) {
            if (largeTurnInputFrames > 3)
                state.currentMovementMode = 6;
            else
                ++largeTurnInputFrames;
        } else
            largeTurnInputFrames = 0;
        return;
    }
    largeTurnInputFrames = 0;
    if (reinterpret_cast<const std::int8_t *>(&state.packedDirection)[1] == 127) {
        if (!reinterpret_cast<const std::int8_t *>(&state.packedDirection)[0]) {
            state.turnInput = 0;
            if (state.buttonHoldFrames[1] > 4)
                state.currentMovementMode = 6;
            return;
        }
        state.buttonHoldFrames[1] = 1;
    }
    if (static_cast<std::int8_t>(state.analogHistory0) < 0 || (state.analogHistory0 && state.analogHistory1) ||
        ((*reinterpret_cast<const std::uint32_t *>(&state.packedDirection) & 0xffffU) && (state.turnInput < 0 ? -state.turnInput : state.turnInput) <= 0x7000))
        state.currentMovementMode = 3;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    using enum CameraState;
    state.padFlags &= ~plymPadForceMotionState2;
    state.previousInputMovementMode = state.movementMode;
    state.movementMode = 1;
    state.normalizedMotionInput = 0.0f;
    state.motionInputEcho = 0.0f;
    state.movementBlendLimit = 1.0f;
    state.activeMovementMode = 0;
    if ((state.flags & 2U) == 0 || state.activeFrameCount < 6)
        return;
    if (state.padFlags & plymPadUseForcedYaw) {
        if (state.padFlags & plymPadUnknown08000000) {
            if (state.rootYawState - 1U <= 1U)
                state.padFlags &= ~plymPadUnknown08000000;
            if (state.previousInputMovementMode == 1U)
                state.padFlags &= ~plymPadUnknown08000000;
            if (state.previousInputMovementMode - 3U <= 1U && state.movementSpeedRatio < 0.001f)
                state.padFlags &= ~plymPadUnknown08000000;
        }
        if ((state.padFlags & plymPadUnknown08000000) == 0) {
            const std::uint32_t rootYawState = state.rootYawState;
            if (rootYawState != 1U && (rootYawState != 2U || !state.movementState->motionTransitionParameter) && rootYawState != 9U)
                state.padFlags &= ~plymPadUseForcedYaw;
        }
    }
    plymComputeTurnInput(state);
    const CameraState cameraState = camCtrlGetState();
    if (cameraState == LockedOn && state.lookAroundState == 2U) {
        std::uint16_t adjustedPitch[2];
        camCtrlGetAdjustedPitchOrYaw(nullptr, adjustedPitch);
        const std::int16_t resolvedYaw = static_cast<std::int16_t>(state.yaw);
        state.lookPitchDelta = static_cast<std::int16_t>(adjustedPitch[0]);
        state.lookBaseYaw = resolvedYaw;
        state.lookYawDelta = static_cast<std::int16_t>(camCtrlGetNextRotY() - resolvedYaw);
        state.lookBasePitch = 0;
    }
    if (cameraState == LockedOn && (state.flags & plymControlLockOnMovement) == 0)
        return;
    if ((state.flags & plymControlLockOnMovement) == 0)
        plymUpdateLookAround(state);
    if (state.flags & 8U) {
        if (state.lookAroundState != 0 && (state.flags & plymControlLockOnMovement) == 0)
            return;
    } else if (state.lookAroundState != 0 && (state.lookInputFlags & 2U) == 0 && (state.flags & plymControlLockOnMovement) == 0) {
        return;
    }
    if (((state.padFlags & 4U) != 0 && (state.flags & plymControlLockOnMovement) == 0) || (state.padFlags & 8U) != 0)
        return;
    if (state.analogHistory0 >= 0x80U)
        state.normalizedMotionInput = static_cast<float>(state.analogHistory0 - 128) / 127.0f;
    if (state.inputActive == 1) {
        const float x = std::fmin(1.0f, std::fmax(-1.0f, static_cast<float>(state.stickX) / 127.0f));
        const float y = std::fmin(1.0f, std::fmax(-1.0f, static_cast<float>(state.stickY) / 127.0f));
        float magnitude = std::fmax(0.0f, std::sqrt(y * y + x * x));
        if (magnitude >= 1.0f)
            magnitude = 1.0f;
        if (magnitude > 0.96875f)
            state.normalizedMotionInput = 1.0f - (1.0f - magnitude) * 32.0f;
        if (state.stickY > 90)
            state.normalizedMotionInput = 0.0f;
    }
    state.motionInputEcho = state.normalizedMotionInput;
    const auto packedX = static_cast<std::int8_t>(state.packedDirection & 0xffU);
    const auto packedY = static_cast<std::int8_t>(state.packedDirection >> 8U);
    if (state.inputActive == 1 && std::abs(static_cast<std::int32_t>(state.turnInput)) > 30720) {
        state.turnInput = 0;
        if (packedY < rotationOnlyPullThreshold) {
            largeTurnInputFrames = 0;
            return;
        }
        if (largeTurnInputFrames < 4) {
            ++largeTurnInputFrames;
            return;
        }
        state.movementMode = 6;
        return;
    }
    largeTurnInputFrames = 0;
    if (packedY > 120) {
        if (!packedX) {
            state.turnInput = 0;
            state.movementMode = 6;
            return;
        }
        state.buttonHoldFrames[1] = 1;
    }
    if (state.analogHistory0 >= 0x80U || (state.analogHistory0 && state.analogHistory1) ||
        ((packedX || packedY) && std::abs(static_cast<std::int32_t>(state.turnInput)) <= 28672)) {
        state.movementMode = 3;
    }
#endif
}


// @verified - d
void UpdatePlayerYawFromInput(PlayerModelState &state) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    std::int16_t delta;
#endif
    state.yawDelta = 0;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    if (!(state.flags & 2U))
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if ((state.flags & 2U) == 0)
#endif
        return;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    if (static_cast<std::int8_t>(state.inputActive) == 1) {
        state.desiredYaw =
            static_cast<std::int16_t>(static_cast<std::int32_t>(-static_cast<float>(state.stickX) / 127.0f * 8192.0f + static_cast<float>(state.inputBaseYaw)));
        state.desiredYaw = static_cast<std::int16_t>(
            static_cast<std::int32_t>(-static_cast<float>(state.stickX) / 127.0f * 12743.0f + static_cast<float>(state.inputBaseYaw)));
        if ((state.turnInput < 0 ? -state.turnInput : state.turnInput) < 0x4000)
            delta = static_cast<std::int16_t>(
                static_cast<std::int32_t>(-static_cast<float>(state.stickX) / 127.0f * 12743.0f + static_cast<float>(state.inputBaseYaw)) - state.currentYaw);
        else if ((state.turnInput < 0 ? -state.turnInput : state.turnInput) < 0x6000)
            delta = static_cast<std::int16_t>(state.turnInput / 2);
        else if ((state.turnInput < 0 ? -state.turnInput : state.turnInput) < 0x8000)
            delta = static_cast<std::int16_t>(state.turnInput / 3 * 2);
        else
            delta = state.turnInput;
    } else if ((state.turnInput < 0 ? -state.turnInput : state.turnInput) < 0x6000) {
        delta = static_cast<std::int16_t>(state.turnInput / 2);
    } else if ((state.turnInput < 0 ? -state.turnInput : state.turnInput) < 0x8000) {
        delta = static_cast<std::int16_t>(state.turnInput / 3 * 2);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    std::int16_t desired = 0;
    if (state.inputActive == 1) {
        const std::int16_t turn = state.turnInput;
        desired = static_cast<std::int16_t>(static_cast<double>(state.inputBaseYaw) - static_cast<double>(state.stickX) / 127.0 * 12743.0);
        state.desiredYaw = desired;
        const std::int32_t magnitude = std::abs(static_cast<std::int32_t>(turn));
        if (magnitude >= 0x4000 && magnitude < 0x6000) {
            desired = static_cast<std::int16_t>(state.currentYaw + turn / 2);
        } else if (magnitude >= 0x6000 && magnitude < 0x8000) {
            desired = static_cast<std::int16_t>(state.currentYaw + 2 * (turn / 3));
        } else if (magnitude >= 0x8000) {
            desired = static_cast<std::int16_t>(state.currentYaw + turn);
        }
#endif
    } else {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
        delta = state.turnInput;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        const std::int16_t turn = state.turnInput;
        const std::int32_t magnitude = std::abs(static_cast<std::int32_t>(turn));
        std::int16_t scaled = turn;
        if (magnitude < 0x6000)
            scaled = static_cast<std::int16_t>(turn / 2);
        else if (magnitude < 0x8000) {
            scaled = static_cast<std::int16_t>(2 * (turn / 3));
#endif
    }

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    state.desiredYaw = static_cast<std::int16_t>(state.currentYaw + delta);
    if (state.currentMovementMode == 6)
        state.desiredYaw = static_cast<std::int16_t>(state.currentYaw + 0x8000);
    if (state.padFlags & 0x200U)
        state.desiredYaw = static_cast<std::int16_t>(state.forcedYaw);
    if (state.padFlags & 0x1000000U)
        state.desiredYaw = static_cast<std::int16_t>(state.forcedYaw);
    state.yawDelta = static_cast<std::int16_t>(state.desiredYaw - state.currentYaw);
    state.committedYaw = state.desiredYaw;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        desired = static_cast<std::int16_t>(state.currentYaw + scaled);
    }
    if (state.movementMode == 6U) {
        desired = static_cast<std::int16_t>(state.currentYaw + 0x8000U);
    }
    if (state.padFlags & (0x200U | 0x01000000U)) {
        desired = static_cast<std::int16_t>(state.forcedYaw);
    }
    state.desiredYaw = desired;
    state.committedYaw = desired;
    state.yawDelta = static_cast<std::int16_t>(desired - state.currentYaw);
#endif
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
bool IsHeldTransitionReady(const PlayerModelState &state);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// scenIsRecordRunning: the named scenario record exists and has not finished.
bool scenIsRecordRunning(shen_name name) {
    const ScenarioFreeTalkRecord *const record = FindScenarioFreeTalkRecord(name);
    return record && record->executionState != 3U;
}

Vector3 PlymHeadLookPoint(const PlayerModelState &state, std::uint16_t yaw, std::uint16_t pitch, bool withCharacterYaw) {
    Vector3 head{};
    charaGetMotWorldPos(state.characterNode, 6, &head);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(head.x, head.y, head.z);
    if (withCharacterYaw)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(state.currentYaw));
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    Matrix4x4::RotateAroundX_Wrapper(pitch);
    Matrix4x4::preTranslate(0.0f, 0.0f, -100.0f);
    const Vector3 point{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    return point;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymUpdateAutomaticLook(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    std::int32_t expired = 0;
    Vector3 head, position;
    state.padFlags &= ~0x10000000U;
    bool looking = false;
    if (!(state.flags & 0x800000U))
        return;
    if (state.padFlags & 0x100U)
        state.automaticLookIdleTimer = 0;
    else if ((state.flags & 0x40000U) || plymIsSearchBlocked())
        state.automaticLookIdleTimer = 0;
    else if (!IsHeldTransitionReady(state))
        state.automaticLookIdleTimer = 0;
    else
        ++state.automaticLookIdleTimer;
    if (!state.automaticLookIdleTimer) {
        state.automaticLookFlags = 0;
        state.automaticLookElapsedFrames = 0;
        state.automaticLookDurationFrames = 0;
        state.automaticLookPreviousAuthor = shen_name(0);
        state.automaticLookAuthorChangeFrames = 0;
    }
    if (!state.automaticLookIdleTimer)
        return;
    state.automaticLookElapsedFrames = static_cast<std::int16_t>(state.automaticLookElapsedFrames + 1);
    if (state.automaticLookElapsedFrames > state.automaticLookDurationFrames)
        expired = 1;
    if (state.automaticLookAuthorChangeFrames > 0)
        --state.automaticLookAuthorChangeFrames;
    Character *author = 0;
    bool finishedAuthor = false;
    std::int32_t idleThreshold = 60;
    const shen_name authorName = GetActiveScenarioCharacter();
    if (authorName)
        author = ctrlGetScenarioChara(authorName);
    if (authorName && !scenIsRecordRunning(authorName)) {
        finishedAuthor = true;
        idleThreshold = 1;
    }
    if (author && static_cast<std::int32_t>(state.automaticLookIdleTimer) > idleThreshold) {
        if (expired) {
            if (state.automaticLookFlags & 1U) {
                state.automaticLookFlags &= ~1U;
            } else if (!(state.automaticLookFlags & 2U)) {
                state.automaticLookFlags |= 1U;
                state.automaticLookElapsedFrames = 0;
                state.automaticLookDurationFrames = 300;
                state.automaticLookPreviousAuthor = authorName;
            }
        } else if (state.automaticLookFlags & 1U) {
            if (state.automaticLookPreviousAuthor != authorName) {
                state.automaticLookDurationFrames += 90;
                state.automaticLookDurationFrames -= state.automaticLookAuthorChangeFrames;
                state.automaticLookAuthorChangeFrames = 90;
            }
        } else if (state.automaticLookPreviousAuthor != authorName) {
            state.automaticLookFlags |= 1U;
            state.automaticLookElapsedFrames = 0;
            state.automaticLookDurationFrames = 300;
            state.automaticLookPreviousAuthor = authorName;
        }
    } else if (state.automaticLookFlags & 1U) {
        state.automaticLookFlags &= ~1U;
        state.automaticLookDurationFrames = 60;
    }
    if (state.automaticLookFlags & 1U) {
        charaGetMotWorldPos(author, 6, &state.lookAtPosition);
        looking = true;
        if (finishedAuthor) {
            GetCharacterWorldPosition(author, &position);
            const std::int16_t authorYaw = static_cast<std::int16_t>(GetCharacterWorldYaw(author));
            matrix_stack::Matrix4x4_Push();
            matrix_stack::LoadMatrix(&state.inverseWorldTransform);
            Matrix4x4::TransformPoint(position, position);
            PopCurrentMatrix(1);
            std::int16_t side;
            if ((position.x < 0.0f ? -position.x : position.x) < 0.2f) {
                if (static_cast<std::int16_t>(authorYaw - state.currentYaw) < 0)
                    side = 5461;
                else
                    side = -5461;
            } else if (position.x < 0.0f) {
                side = 5461;
            } else {
                side = -5461;
            }
            const std::int16_t heading = static_cast<std::int16_t>(CopyHeading(head, state.lookAtPosition) - state.currentYaw);
            std::int16_t offset = static_cast<std::int16_t>(heading + side);
            offset = static_cast<std::int16_t>((offset < -5461 ? -5461 : offset) < 5461 ? (offset < -5461 ? -5461 : offset) : 5461);
            charaGetMotWorldPos(state.character, 6, &head);
            matrix_stack::PushUnitMatrix();
            matrix_stack::TranslateVector(&head);
            Matrix4x4::RotateAroundY_Wrapper(state.currentYaw + offset);
            Matrix4x4::preTranslate(0.0f, 0.0f, -100.0f);
            matrix_stack::GetTranslation(&state.lookAtPosition);
            PopCurrentMatrix(1);
        }
    }
    state.automaticLookPreviousAuthor = authorName;
    if (!looking &&
        GetMediatedButtonBindings(shen_name(SHEN_NAME(BTDR))) && state.searchRegionKey && static_cast<std::int32_t>(state.automaticLookIdleTimer) > 60) {
        if (expired) {
            if (!(state.automaticLookFlags & 2U)) {
                state.automaticLookFlags |= 2U;
                state.automaticLookElapsedFrames = 0;
                state.automaticLookDurationFrames = 300;
            } else {
                state.automaticLookFlags &= ~2U;
            }
        }
        if (state.automaticLookFlags & 2U) {
            matrix_stack::Matrix4x4_Push();
            matrix_stack::LoadMatrix(doorFindEntryTransform(shen_name(state.searchRegionKey)));
            Matrix4x4::preTranslate(0.44999999f, 0.89999998f, 0.0f);
            matrix_stack::GetTranslation(&state.lookAtPosition);
            PopCurrentMatrix(1);
            looking = true;
        }
    }
    if (state.automaticLookFlags & 3U)
        state.automaticLookFlags &= ~4U;
    if (!looking && static_cast<std::int32_t>(state.automaticLookIdleTimer) > 90) {
        std::int16_t pitch, yaw;
        looking = true;
        if (expired) {
            pitch = static_cast<std::int16_t>(static_cast<std::int32_t>(dc::sdk::RandomInt()) % 5461 - 3640);
            yaw = static_cast<std::int16_t>(static_cast<std::int32_t>(dc::sdk::RandomInt()) % 10922 - 5461);
            if (yaw < 0)
                yaw = static_cast<std::int16_t>(yaw - 1820);
            else
                yaw = static_cast<std::int16_t>(yaw + 1820);
            if (static_cast<std::int32_t>(dc::sdk::RandomInt() & 3U) > 0) {
                if (state.automaticLookYaw > 0)
                    yaw = static_cast<std::int16_t>(-(yaw < 0 ? -yaw : yaw));
                else
                    yaw = static_cast<std::int16_t>(yaw < 0 ? -yaw : yaw);
            }
            state.automaticLookPitch = pitch;
            state.automaticLookYaw = yaw;
            state.automaticLookElapsedFrames = 0;
            state.automaticLookDurationFrames = 240;
            state.automaticLookFlags |= 4U;
        }
        if (looking) {
            yaw = 0;
            pitch = 0;
            if (state.automaticLookFlags & 4U) {
                const std::int32_t phase = static_cast<std::int32_t>(static_cast<float>(state.automaticLookElapsedFrames % 240) / 240.0f * 65536.0f);
                pitch = static_cast<std::int16_t>(state.automaticLookPitch + static_cast<std::int16_t>(dc::sdk::SinFixed(phase) * 546.0f));
                yaw = static_cast<std::int16_t>(state.automaticLookYaw + static_cast<std::int16_t>(dc::sdk::SinFixed(phase) * 1820.0f));
            }
            charaGetMotWorldPos(state.character, 6, &head);
            matrix_stack::PushUnitMatrix();
            matrix_stack::TranslateVector(&head);
            Matrix4x4::RotateAroundY_Wrapper(state.currentYaw);
            Matrix4x4::RotateAroundY_Wrapper(yaw);
            Matrix4x4::RotateAroundX_Wrapper(pitch);
            Matrix4x4::preTranslate(0.0f, 0.0f, -100.0f);
            matrix_stack::GetTranslation(&state.lookAtPosition);
            PopCurrentMatrix(1);
        }
    }
    if (looking) {
        state.padFlags |= 0x100U;
        state.padFlags |= 0x10000000U;
        state.lookTargetMode = 1;
    }
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    PlayerAutomaticLookState &look = state.automaticLook;
    state.padFlags &= 0xefffffffU;
    std::uint16_t yaw = 0;
    const std::uint32_t controlFlags = state.flags;
    bool looking = false;
    if ((controlFlags & 0x800000U) == 0)
        return;
    if ((state.padFlags & 0x100U) || (controlFlags & 0x40000U) || plymIsSearchBlocked() || state.rootYawState) {
        state.automaticLookIdleTimer = 0.0f;
    } else {
        state.automaticLookIdleTimer = ScaledDelta(1.0f) + state.automaticLookIdleTimer;
    }
    if (state.automaticLookIdleTimer == 0.0f) {
        look.flags = 0;
        look.elapsedFrames = 0.0f;
        look.targetDurationFrames = 0.0f;
        look.previousAuthorName = {};
        look.authorChangeFramesRemaining = 0;
        return;
    }
    const float elapsed = ScaledDelta(1.0f) + look.elapsedFrames;
    const bool expired = !(elapsed < look.targetDurationFrames) && elapsed != look.targetDurationFrames;
    look.elapsedFrames = elapsed;
    if (look.authorChangeFramesRemaining > 0)
        --look.authorChangeFramesRemaining;
    Character *author = nullptr;
    bool finishedAuthor = false;
    std::int32_t idleThreshold = 60;
    const shen_name authorName = GetActiveScenarioCharacter();
    if (authorName.name) {
        author = authFindChara(authorName);
        if (!scenIsRecordRunning(authorName)) {
            finishedAuthor = true;
            idleThreshold = 1;
        }
    }
    const std::uint8_t flags = look.flags;
    auto startAuthorLook = [&]() {
        look.flags = flags | automaticLookAuthorTarget;
        look.elapsedFrames = 0.0f;
        look.targetDurationFrames = 300.0f;
        look.previousAuthorName = authorName;
    };
    if (!authorName.name || !author || state.automaticLookIdleTimer <= static_cast<float>(idleThreshold)) {
        if (flags & automaticLookAuthorTarget) {
            look.targetDurationFrames = 60.0f;
            look.flags = flags & ~automaticLookAuthorTarget;
        }
    } else if (expired) {
        if (flags & automaticLookAuthorTarget)
            look.flags = flags & ~automaticLookAuthorTarget;
        else if ((flags & automaticLookRegionTarget) == 0)
            startAuthorLook();
    } else if (flags & automaticLookAuthorTarget) {
        if (look.previousAuthorName.name != authorName.name) {
            const auto extension = static_cast<std::int16_t>(90 - look.authorChangeFramesRemaining);
            look.authorChangeFramesRemaining = 90;
            look.targetDurationFrames = static_cast<float>(extension) + look.targetDurationFrames;
        }
    } else if (look.previousAuthorName.name != authorName.name) {
        startAuthorLook();
    }
    if (look.flags & automaticLookAuthorTarget) {
        charaGetMotWorldPos(author, 6, &state.lookAtPosition);
        looking = true;
        if (finishedAuthor) {
            const std::int16_t authorYaw = GetAllAngYForNode(&author->entity.node);
            const Vector3 local = TransformPlymPoint(state.inverseWorldTransform, GetCharacterWorldPosition(author));
            std::int16_t side{};
            if (std::fabs(local.x) >= 0.2f)
                side = local.x >= 0.0f ? -5461 : 5461;
            else
                side = static_cast<std::int16_t>(authorYaw - state.currentYaw) < 0 ? 5461 : -5461;
            // The native bearing is taken from a zero-initialised origin, not the head.
            auto offset = static_cast<std::int16_t>(PlymAngleXZ(state.lookAtPosition) - state.currentYaw + side);
            if (offset < -5461)
                offset = -5461;
            else if (offset >= 5461)
                offset = 5461;
            state.lookAtPosition = PlymHeadLookPoint(state, static_cast<std::uint16_t>(offset + state.currentYaw), 0, false);
        }
    }
    look.previousAuthorName = authorName;
    if (!looking && GetMediatedButtonBindings(BUTTON_DOOR) && state.searchRegionKey && state.automaticLookIdleTimer > 60.0f) {
        if (expired) {
            if (look.flags & automaticLookRegionTarget) {
                look.flags &= ~automaticLookRegionTarget;
            } else {
                look.flags |= automaticLookRegionTarget;
                look.elapsedFrames = 0.0f;
                look.targetDurationFrames = 300.0f;
            }
        }
        if (look.flags & automaticLookRegionTarget) {
            Matrix4x4_Push();
            Matrix4x4::SetActiveTransform(doorFindEntryTransform(state.searchRegionKey));
            Matrix4x4::preTranslate(0.44999999f, 0.89999998f, 0.0f);
            state.lookAtPosition = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            Matrix4x4::Pop(1);
            looking = true;
        }
    }
    if (look.flags & (automaticLookAuthorTarget | automaticLookRegionTarget))
        look.flags &= ~automaticLookIdleWander;
    if (!looking) {
        if (state.automaticLookIdleTimer <= 90.0f)
            return;
        if (expired) {
            const auto pitchOffset = static_cast<std::int16_t>(IncrementSharedPrngUInt16() % 5461 - 3640);
            const std::uint16_t random = IncrementSharedPrngUInt16();
            const auto spread = static_cast<std::int16_t>(random % 10922 - 5461);
            auto yawOffset = static_cast<std::int16_t>(spread >= 0 ? spread + 1820 : spread - 1820);
            if ((IncrementSharedPrngUInt16() & 3U) != 0) {
                const auto magnitude = static_cast<std::int16_t>(Abs16(yawOffset));
                yawOffset = look.yawOffset <= 0 ? magnitude : static_cast<std::int16_t>(-magnitude);
            }
            look.flags |= automaticLookIdleWander;
            look.pitchOffset = pitchOffset;
            look.yawOffset = yawOffset;
            look.elapsedFrames = 0.0f;
            look.targetDurationFrames = 240.0f;
        }
        std::uint16_t pitch = 0;
        if (look.flags & automaticLookIdleWander) {
            const auto phase = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int32_t>(look.elapsedFrames) % 240) / 240.0f * 65536.0f);
            const float wave = sin_fixed16(static_cast<std::int16_t>(phase));
            pitch = static_cast<std::uint16_t>(look.pitchOffset - static_cast<std::int32_t>(wave * -546.0f));
            yaw = static_cast<std::uint16_t>(look.yawOffset - static_cast<std::int32_t>(wave * -1820.0f));
        }
        state.lookAtPosition = PlymHeadLookPoint(state, yaw, pitch, true);
    }
    state.padFlags |= 0x10000100U;
    state.lookTargetMode = lookTargetAmbientSlow;
#endif
}


// @verified - d
void plymUpdateLookTarget(PlayerModelState &state) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    if (!(state.flags & 2U))
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    if ((state.flags & 2U) == 0)
#endif
        return;
    plymUpdateAutomaticLook(state);

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    std::int32_t requested = 0;
    if (state.padFlags & 0x100U)
        requested = 1;
    if (static_cast<std::int8_t>(state.lookTargetMode) == -1)
        requested = -requested;
    if (requested == 1) {
        if (static_cast<std::int8_t>(state.lookTargetMode) == 0)
            mtwkUpdateWithPosition(state.motion, 0, 0x204);
        else if (static_cast<std::int8_t>(state.lookTargetMode) == 1)
            mtwkUpdateWithPosition(state.motion, 0, 0x104);
        else if (static_cast<std::int8_t>(state.lookTargetMode) == 5)
            mtwkUpdateWithPosition(state.motion, 0, 0x504);
        else if (static_cast<std::int8_t>(state.lookTargetMode) == 10)
            mtwkUpdateWithPosition(state.motion, 0, 3);
        if (state.lookTargetFlags & 4U)
            mtwkUpdateWithPosition(state.motion, &state.lookAtPosition, 1);
        else
            mtwkUpdateWithPosition(state.motion, &state.lookAtPosition, 0);
        if (static_cast<std::int8_t>(state.lookTargetMode) == 10)
            mtwkSetPelvisYawStep(state.motion, 0x2000);
    } else if (requested == -1) {
        if (mtwkSetExtraFlag(state.motion, 59, 1)) {
            mtwkUpdateWithPosition(state.motion, 0, 3);
            mtwkUpdateWithPosition(state.motion, 0, 0);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const std::uint8_t mode = state.lookTargetMode;
    const std::uint32_t requested = (state.padFlags >> 8U) & 1U;
    const std::int32_t request = mode != lookTargetCancel ? static_cast<std::int32_t>(requested) : -static_cast<std::int32_t>(requested);
    MotionWork *const motionWork = state.motionWork;
    if (request == 1) {
        std::uint16_t operation = 0;
        switch (mode) {
        case 0:
            operation = 516;
            break;
        case lookTargetAmbientSlow:
            operation = 260;
            break;
        case lookTargetSpecialFast:
            operation = 1284;
            break;
        case lookTargetForcedYaw:
            operation = 3;
            break;
        default:
            break;
#endif
        }

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    } else if (mtwkSetExtraFlag(state.motion, 59, 1)) {
        mtwkUpdateWithPosition(state.motion, 0, 0x504);
        mtwkUpdateWithPosition(state.motion, 0, 0);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        if (operation)
            mtwkUpdateWithPosition(motionWork, nullptr, operation);
        mtwkUpdateWithPosition(motionWork, &state.lookAtPosition, (state.lookTargetFlags & 4U) != 0);
        if (state.lookTargetFlags & 8U) {
            Vector3 hip{};
            Vector3 knee{};
            mtwkGetBonePosition(motionWork, MB_Head, &hip);
            mtwkGetBonePosition(motionWork, MB_HeadTarget, &knee);
            const std::int16_t legYaw = PlymAngleXZ({knee.x - hip.x, knee.y - hip.y, knee.z - hip.z});
            const std::int16_t targetYaw = PlymAngleXZ({state.lookAtPosition.x - hip.x, state.lookAtPosition.y - hip.y, state.lookAtPosition.z - hip.z});
            const auto difference = static_cast<std::int16_t>(std::abs(targetYaw - legYaw));
            if (difference < 5461) {
                const std::int32_t angle =
                    difference < 2730 ? 32 : static_cast<std::int16_t>(static_cast<std::int32_t>((difference - 2730) / 2730.0f * 448.0f + 32.0f));
                if (static_cast<float>(angle) >= 0.0f)
                    motionWork->runtime.constraintStepLimit[2] = static_cast<float>(angle) / 10430.378f;
            }
        }
        if (state.lookTargetMode == lookTargetForcedYaw)
            mtwkSetPelvisYawStep(motionWork, 0x2000);
    } else if (request == -1 ? mtwkSetExtraFlag(motionWork, 59U, 1) != 0 : mtwkSetExtraFlag(motionWork, 59U, 1) != 0 && !plymCheckAuthorTargetRequested()) {
        mtwkUpdateWithPosition(motionWork, nullptr, request == -1 ? 3 : 1284);
        mtwkUpdateWithPosition(motionWork, nullptr, 0);
#endif
    }
    state.padFlags &= ~0x100U;
    state.lookTargetMode = 0;
    state.lookTargetFlags = 0;
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
std::int16_t camCtrlGetPitchYawWithOffsetsFrom(const PlayerModelState &state) {
    std::int16_t angles[2];
    Vector3 forward;
    camCtrlQueryPitchYaw(*reinterpret_cast<Vector3s *>(angles));
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(angles[1]);
    Matrix4x4::RotateAroundX_Wrapper(angles[0]);
    Matrix4x4::RotateAroundY_Wrapper(state.currentYaw - angles[1]);
    GetTransformedForwardVector(&forward);
    PopCurrentMatrix(1);
    Vector3_ToFixed16EulerAngles(forward, *reinterpret_cast<Vector3s *>(angles));
    return angles[0];
}
#endif

} // namespace player_detail

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
bool plymPublishSearchHandTarget(PlayerSearchHand hand, const Vector3 *worldTarget) {
    MotionConstraintSlot slot{};
    switch (hand) {
    case PlayerSearchHand::Right:
        slot = MotionConstraintSlot::RightArm;
        break;
    case PlayerSearchHand::Left:
        slot = MotionConstraintSlot::LeftArm;
        break;
    default:
        return false;
    }

    MotionWork *const motionWork = g_playerRuntime.model ? g_playerRuntime.model->motionWork : nullptr;
    if (!worldTarget) {
        mtwkClearConstraintTarget(motionWork, slot);
        return false;
    }
    return mtwkSetConstraintTarget(motionWork, slot, *worldTarget);
}

void plymClearSearchHandTargets() {
    MotionWork *const motionWork = g_playerRuntime.model ? g_playerRuntime.model->motionWork : nullptr;
    mtwkClearConstraintTarget(motionWork, MotionConstraintSlot::RightArm);
    mtwkClearConstraintTarget(motionWork, MotionConstraintSlot::LeftArm);
}

bool plymIsSearchHandTargetSettled(PlayerSearchHand hand) {
    const MotionConstraintSlot slot = hand == PlayerSearchHand::Right ? MotionConstraintSlot::RightArm : MotionConstraintSlot::LeftArm;
    return mtwkIsConstraintTargetSettled(g_playerRuntime.model ? g_playerRuntime.model->motionWork : nullptr, slot);
}

void ctrlSetLookOverlayWeight(float alpha) {
    PlayerModelState *state = g_playerRuntime.model;
    if (!state)
        return;
    if (alpha >= 0.0f) {
        state->padFlags |= 0x8000U;
        state->field7c4 = alpha;
    } else {
        modelSetCharaAlpha(state->characterNode, -alpha);
        state->padFlags |= 0x8000U;
    }
}

float ctrlGetLookOverlayWeight() {
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const auto *const ryo = control ? static_cast<const Character *>(control->ryo) : nullptr;
    return ryo ? ryo->alpha : 0.0f;
}

bool plymIsLookAllowed(std::uint32_t modes) {
    const PlayerModelState *const model = g_playerRuntime.model;
    const PlayerMovementState *const state = model ? model->movementState : nullptr;
    if (!state)
        return false;
    bool allowed = false;
    if ((modes & 1U) != 0 && state->stationaryFrames > 0)
        allowed = state->turningFrames == 0;
    if ((modes & 0x10U) != 0 && (model->padFlags & 0x400000U) != 0)
        allowed = true;
    if ((modes & 0x20U) != 0 && (state->flags & 8U) != 0)
        allowed = true;
    if ((modes & 0x40U) != 0 && (state->flags & 8U) != 0) {
        const CharacterControlModel *const control = FindCharacterControlModel(model->characterNode);
        if (control && (control->flags & 0x800U) != 0)
            allowed = true;
    }
    return allowed;
}

void camCtrlQueryPitchYaw(Vector3s &angles) {
    const PlayerModelState *const model = g_playerRuntime.model;
    if (!model)
        return;
    angles.x = 0;
    angles.y = 0;
    if (const CharacterControlModel *const control = FindCharacterControlModel(model->characterNode)) {
        angles.x = control->groundPitch;
        angles.y = control->groundYaw;
    }
    const PlayerMovementState &movement = model->embeddedMovementState;
    if ((movement.flags & 8U) != 0) {
        const Vector3 position = GetCharacterWorldPosition(model->characterNode);
        Vector3 local{};
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-movement.targetWorldYaw));
        Matrix4x4::preTranslate(-movement.targetWorldPosition.x, -movement.targetWorldPosition.y, -movement.targetWorldPosition.z);
        Matrix4x4::TransformPoint(position, local);
        Matrix4x4::Pop(1);
        const float depth = -local.z;
        if (depth >= 0.0f && static_cast<float>(movement.targetHeightScale - 1) * 0.30000001f >= depth) {
            angles.x = static_cast<std::int16_t>(static_cast<std::int32_t>(std::atan(0.74999994f) * 65536.0f / 6.2831855f + 0.5f));
            angles.y = movement.targetWorldYaw;
        }
    }
    angles.x = static_cast<std::int16_t>(-angles.x);
}
#endif

} // namespace s2

} // namespace shenmue
