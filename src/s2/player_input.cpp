#include "s2/player_internal.hpp"

namespace shenmue::s2::player_detail {



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


std::uint8_t plymTriggerHistoryValue(std::uint8_t trigger) {
    const float value = static_cast<float>(trigger) * 1.4088398f - 90.165749f;
    if (0.0f > value)
        return 0;
    if (value >= 255.0f)
        return 0xff;
    return static_cast<std::uint8_t>(static_cast<std::int32_t>(value));
}


// @verified - p
void plymSampleInput(PlayerModelState &state) {
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
}

void plymComputeTurnInput(PlayerModelState &state) {
    const float x = -static_cast<float>(static_cast<std::int8_t>(state.packedDirection & 0xffU));
    const float y = static_cast<float>(static_cast<std::int8_t>(state.packedDirection >> 8U));
    const std::int16_t cameraYaw = static_cast<std::int16_t>(camCtrlGetNextRotY());
    std::int16_t turn = 0;
    if (std::fabs(y) >= 0.001f || std::fabs(x) >= 0.001f)
        turn = static_cast<std::int16_t>(FixedAtan2(y, x) + 0x4000);
    state.turnInput = turn;
    state.inputTargetYaw = static_cast<std::int16_t>(cameraYaw + turn);
    state.inputBaseYaw = cameraYaw;
}

std::uint8_t Abs8(std::int8_t value) {
    return static_cast<std::uint8_t>(value < 0 ? -value : value);
}

// rootYawState 1, 2 and 6 are the moving root classes the look owner defers to.
bool IsLookMovingRootState(std::uint32_t rootYawState) {
    return ((rootYawState - 1U) & 0xfffffffaU) == 0 && rootYawState != 5U;
}





void plymUpdateLookAround(PlayerModelState &state) {
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
        GetCharacterMotionWorldPosition(state.characterNode, 6, &head);
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
}

void plymUpdateCameraInput(PlayerModelState &state) {
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
}

void UpdatePlayerYawFromInput(PlayerModelState &state) {
    state.yawDelta = 0;
    if ((state.flags & 2U) == 0)
        return;

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
    } else {
        const std::int16_t turn = state.turnInput;
        const std::int32_t magnitude = std::abs(static_cast<std::int32_t>(turn));
        std::int16_t scaled = turn;
        if (magnitude < 0x6000)
            scaled = static_cast<std::int16_t>(turn / 2);
        else if (magnitude < 0x8000) {
            scaled = static_cast<std::int16_t>(2 * (turn / 3));
        }
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
}

// scenIsRecordRunning: the named scenario record exists and has not finished.
bool scenIsRecordRunning(shen_name name) {
    const ScenarioFreeTalkRecord *const record = FindScenarioFreeTalkRecord(name);
    return record && record->executionState != 3U;
}

Vector3 PlymHeadLookPoint(const PlayerModelState &state, std::uint16_t yaw, std::uint16_t pitch, bool withCharacterYaw) {
    Vector3 head{};
    GetCharacterMotionWorldPosition(state.characterNode, 6, &head);
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



void plymUpdateAutomaticLook(PlayerModelState &state) {
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
        GetCharacterMotionWorldPosition(author, 6, &state.lookAtPosition);
        looking = true;
        if (finishedAuthor) {
            const std::int16_t authorYaw = GetAllAngYForNode(&author->entity.node);
            const Vector3 local = TransformPlymPoint(state.inverseWorldTransform, GetCharacterWorldPosition(author));
            std::int16_t side{};
            if (std::fabs(local.x) >= 0.2f)
                side = local.x >= 0.0f ? -5461 : 5461;
            else
                side = static_cast<std::int16_t>(authorYaw - state.currentYaw) < 0 ? 5461 : -5461;

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
}



void plymUpdateLookTarget(PlayerModelState &state) {
    if ((state.flags & 2U) == 0)
        return;
    plymUpdateAutomaticLook(state);
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
        }
        if (operation)
            UpdateMotionWorkWithPosition(motionWork, nullptr, operation);
        UpdateMotionWorkWithPosition(motionWork, &state.lookAtPosition, (state.lookTargetFlags & 4U) != 0);
        if (state.lookTargetFlags & 8U) {
            Vector3 hip{};
            Vector3 knee{};
            GetMotionBonePosition(motionWork, MB_Head, &hip);
            GetMotionBonePosition(motionWork, MB_HeadTarget, &knee);
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
            SetMotionPelvisYawStep(motionWork, 0x2000);
    } else if (request == -1 ? mtwkSetExtraFlag(motionWork, 59U, 1) != 0 : mtwkSetExtraFlag(motionWork, 59U, 1) != 0 && !plymCheckAuthorTargetRequested()) {
        UpdateMotionWorkWithPosition(motionWork, nullptr, request == -1 ? 3 : 1284);
        UpdateMotionWorkWithPosition(motionWork, nullptr, 0);
    }
    state.padFlags &= ~0x100U;
    state.lookTargetMode = 0;
    state.lookTargetFlags = 0;
}

} // namespace shenmue::s2::player_detail

namespace shenmue::s2 {

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
        ClearMotionConstraintTarget(motionWork, slot);
        return false;
    }
    return SetMotionConstraintTarget(motionWork, slot, *worldTarget);
}

void plymClearSearchHandTargets() {
    MotionWork *const motionWork = g_playerRuntime.model ? g_playerRuntime.model->motionWork : nullptr;
    ClearMotionConstraintTarget(motionWork, MotionConstraintSlot::RightArm);
    ClearMotionConstraintTarget(motionWork, MotionConstraintSlot::LeftArm);
}

bool plymIsSearchHandTargetSettled(PlayerSearchHand hand) {
    const MotionConstraintSlot slot = hand == PlayerSearchHand::Right ? MotionConstraintSlot::RightArm : MotionConstraintSlot::LeftArm;
    return IsMotionConstraintTargetSettled(g_playerRuntime.model ? g_playerRuntime.model->motionWork : nullptr, slot);
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

} // namespace shenmue::s2
