#include "shenmue.h"
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
#include "s2/player_model_dc.hpp"
#include "s2/player_movement_dc.hpp"
#include "s2/math.hpp"
#include "s2/scalar_math_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include <cmath>
#include <cfloat>
#include "common/native_sqrt.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/button_mediation_dc.hpp"
#include "s2/search_state_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/modifier_vertices_dc.hpp"
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/player_internal.hpp"
#endif

namespace shenmue {

namespace s2 {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
std::int16_t CameraLerpAngle(std::int32_t target, std::int32_t base, float amount);
void SetDebugScreenTextPosition(std::int32_t column, std::int32_t line);
std::int32_t GetNpcRouteMotionMode(shen_name name);
shen_name GetCameraSpecialTargetName();
std::int32_t IsNaviActive();

void drBeginEntry(shen_name region);
void OffsetPointAlongYaw(Vector3 *result, const Vector3 *point, float distance, std::int16_t yaw);

std::int16_t CopyHeading(const Vector3 &from, const Vector3 &to);
Vector3 *ccmGetFrameDisplacement(Character *character, Vector3 *displacement);
std::int32_t FindFieldSegmentOffsetInWorld(Vector3 *output, const Vector3 *destination, const Vector3 *origin);
std::int32_t PartyGuideCallbackInitialize();
std::int32_t PartyGuideCallbackPick(const Vector3 *point);
void PartyGuideCallbackFind(const Vector3 *point);
std::int32_t PartyGuideCallbackEndState();
std::int32_t PartyGuideCallbackDirection();
void PartyGuideCallbackRouteHint(std::int32_t routeIndex);
void PartyGuideCallbackSample(Vector3 *point, std::int16_t *yaw);
std::int32_t PartyGuideCallbackAdvance(float distance);
#endif

namespace model_component_detail {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
float Vector3_ComputeSquaredDistance(const Vector3 &left, const Vector3 &right);
#endif

} // namespace model_component_detail

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
 // namespace model_component_detail

// @verified - d
std::int16_t plymGetAuthorPlacementYaw(shen_name name) {
    const PartyCompanion *party = FindPartyCompanionByName(name);
    if (!party)
        return 0;
    if (party->mode == 2)
        return party->routeYaw;
    const PlayerPlacementNode *selected;
    for (selected = &party->placement; selected->next; selected = selected->next)
        if (selected->next->flags & 0x20000U)
            break;
    return CopyHeading(party->position, selected->position);
}
#endif

namespace player_detail {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
bool IsHeldTransitionReady(const PlayerModelState &state);
std::int32_t flddRaycastScene(PlayerModelState &state, const Vector3 &origin, const Vector3 &destination);

std::int32_t plymUpdateRegionInteraction(PlayerModelState &state);
std::int32_t plymUpdateSpecialMode(PlayerModelState &state);
void plymUpdateLockOnMovement(PlayerModelState &state);
void plymUpdateScriptedPlacementTarget(PlayerModelState &state);
void plymUpdateInteractionPlacementTarget(PlayerModelState &state);
void plymProbeMovementCollision(PlayerModelState &state);
void plymProbeDesiredYawCollision(PlayerModelState &state);
void UpdatePlayerDeltaSpeedRatio(PlayerModelState &state);
void plymResolveCollisionTurn(PlayerModelState &state);
std::int32_t plymBuildAuthorPlacementContext(PlayerModelState &state);
void plymUpdateAuthorPlacementTarget(PlayerModelState &state);

// @verified - d
void plymSelectActiveMovementMode(PlayerModelState &state) {
    PlayerMovementState &movement = *reinterpret_cast<PlayerMovementState *>(&state.retainedSecondaryController);
    if (state.padFlags & 0x1000000U) {
        state.activeMovementMode = 2;
        state.motionInputEcho = 0.0f;
        return;
    }
    if (state.flags & 0x80U) {
        state.activeMovementMode = 1;
        state.desiredYaw = state.currentYaw;
        state.motionInputEcho = 0.0f;
        return;
    }
    state.activeMovementMode = 1;
    plymUpdateRegionInteraction(state);
    if (state.padFlags & 0x800U)
        return;
    plymUpdateSpecialMode(state);
    if (state.flags & 0x40000U)
        return;
    if (*reinterpret_cast<const std::int8_t *>(&state.motionInputState)) {
        switch (*reinterpret_cast<const std::int8_t *>(&state.motionInputState)) {
        case 1:
            state.activeMovementMode = 1;
            state.motionInputEcho = 0.0f;
            if (IsHeldTransitionReady(state))
                state.padFlags |= 0x20U;
            break;
        case 3:
            state.activeMovementMode = 3;
            state.motionInputEcho = 0.0f;
            break;
        case 2:
            state.activeMovementMode = state.requestedMovementMode;
            state.motionInputEcho = 0.0f;
            if (state.activeMovementMode == 4)
                state.activeMovementMode = 3;
            break;
        case 10:
            plymUpdateInteractionPlacementTarget(state);
            break;
        case 7:
            plymUpdateScriptedPlacementTarget(state);
            break;
        }
        return;
    }
    if (state.flags & 0x8000000U) {
        plymUpdateLockOnMovement(state);
        return;
    }
    if (!(state.padFlags & 0x10U)) {
        if (state.retainedMovementMode == 6 && movement.previousRequestedMode == 6)
            state.padFlags |= 0x10U;
    } else if (state.currentMovementMode != 6) {
        state.padFlags &= ~0x10U;
    }
    if (!(state.flags & 2U))
        return;
    if (*reinterpret_cast<const std::int8_t *>(&state.turnaroundState)) {
        if (*reinterpret_cast<const std::int8_t *>(&state.turnaroundState) == 1) {
            switch (*reinterpret_cast<const std::int16_t *>(&state.turnaroundPhase)) {
            case 0:
                state.motionInputEcho = 0.0f;
                if (state.interpolationValue > 0.1f)
                    state.activeMovementMode = 3;
                else {
                    state.activeMovementMode = 1;
                    ++state.turnaroundPhase;
                }
                state.desiredYaw = state.currentYaw;
                state.turnaroundFrames = 0;
                return;
            case 1:
                state.motionInputEcho = 0.0f;
                state.activeMovementMode = 6;
                state.desiredYaw = static_cast<std::int16_t>(state.currentYaw + 0x8000);
                if (state.motionState != 4 && state.motionState != 9)
                    return;
                ++state.turnaroundPhase;
            case 2:
                ++state.turnaroundPhase;
            case 3:
                if (!(state.padFlags & 0x10U)) {
                    state.turnaroundState = 0;
                    return;
                }
                if (state.motionState == 4 || state.motionState == 9)
                    return;
                if (state.movementInterpolation == 0.0f) {
                    state.turnaroundState = 0;
                    return;
                }
                if (!(state.padFlags & 0x10U)) {
                    state.turnaroundState = 0;
                    return;
                }
                ++state.turnaroundPhase;
            case 4:
                if (!(state.padFlags & 0x10U))
                    state.turnaroundState = 0;
                state.activeMovementMode = 3;
                state.desiredYaw = state.currentYaw;
                if (++state.turnaroundFrames >= 20)
                    state.turnaroundPhase = 0;
                return;
            default:
                state.turnaroundState = 0;
                return;
            }
        }
        state.turnaroundState = 0;
        state.turnaroundPhase = 0;
    }
    if (state.currentMovementMode != 6)
        state.rotationOnlyWaitFrames = 0;
    switch (state.currentMovementMode) {
    case 6:
        state.activeMovementMode = 0;
        if (state.padFlags & 0x10U)
            return;
        if (!IsHeldTransitionReady(state)) {
            if (state.rotationOnlyWaitFrames++ > 3) {
                state.turnaroundState = 1;
                state.turnaroundPhase = 0;
            }
            return;
        }
        state.activeMovementMode = state.currentMovementMode;
        return;
    case 1:
        state.activeMovementMode = state.currentMovementMode;
        if (state.interpolationValue > 0.1f)
            state.activeMovementMode = 3;
        return;
    case 3:
    case 4: {
        state.activeMovementMode = state.currentMovementMode;
        if (!(state.padFlags & 0x400000U))
            return;
        std::int16_t limit = IsHeldTransitionReady(state) ? 0x1000 : 0x2001;
        --limit;
        std::int32_t offset;
        std::int32_t magnitude;
        std::int32_t clamped;
        if (IsHeldTransitionReady(state)) {
            offset = static_cast<std::int16_t>(state.currentYaw - movement.targetWorldYaw);
            magnitude = offset < 0 ? -offset : offset;
            if (static_cast<std::uint32_t>(magnitude) - 0x2001U > 0x3ffeU) {
                if (!state.yawDelta)
                    return;
                offset = static_cast<std::int16_t>(state.desiredYaw - state.currentYaw);
                clamped = offset < -limit ? -limit : offset;
                offset = static_cast<std::int16_t>(clamped < limit ? clamped : limit);
                clamped = static_cast<std::int16_t>(state.currentYaw + offset);
                offset = static_cast<std::int16_t>(state.desiredYaw - movement.targetWorldYaw);
                magnitude = offset < 0 ? -offset : offset;
                if (static_cast<std::uint32_t>(magnitude) - 0x2001U > 0x3ffeU) {
                    state.desiredYaw = clamped;
                    return;
                }
            } else {
                offset = static_cast<std::int16_t>(state.desiredYaw - movement.targetWorldYaw);
                magnitude = offset < 0 ? -offset : offset;
                if (magnitude < 0x4000)
                    state.desiredYaw = movement.targetWorldYaw;
                else
                    state.desiredYaw = static_cast<std::int16_t>(movement.targetWorldYaw + 0x8000);
            }
            state.activeMovementMode = 2;
            state.padFlags &= 0xf7fffdffU;
            return;
        }
        offset = static_cast<std::int16_t>(state.desiredYaw - movement.targetWorldYaw);
        magnitude = static_cast<std::int16_t>(state.currentYaw - movement.targetWorldYaw);
        if (magnitude < 0)
            magnitude = -magnitude;
        if (magnitude < 0x4000) {
            clamped = offset < -limit ? -limit : offset;
            offset = static_cast<std::int16_t>(clamped < limit ? clamped : limit);
        } else {
            offset = static_cast<std::int16_t>(offset + 0x8000);
            magnitude = offset < -limit ? -limit : offset;
            offset = static_cast<std::int16_t>(magnitude < limit ? magnitude : limit);
            offset = static_cast<std::int16_t>(offset + 0x8000);
        }
        state.desiredYaw = static_cast<std::int16_t>(offset + movement.targetWorldYaw);
        if (state.motionState == 9)
            state.activeMovementMode = 1;
        return;
    }
    case 5:
        state.activeMovementMode = 1;
        return;
    default:
        state.activeMovementMode = state.currentMovementMode;
        return;
    }
}


// @verified - d
void plymResolveMovementMode(PlayerModelState &state) {
    PlayerMovementState &movement = *reinterpret_cast<PlayerMovementState *>(&state.retainedSecondaryController);
    state.previousMovementResult = state.movementResult;
    state.movementResult = 0.0f;
    plymSelectActiveMovementMode(state);
    if (state.committedYaw != state.desiredYaw) {
        plymProbeMovementCollision(state);
        plymProbeDesiredYawCollision(state);
        UpdatePlayerDeltaSpeedRatio(state);
    }
    const std::int16_t yawDelta = static_cast<std::int16_t>(state.desiredYaw - state.currentYaw);
    state.yawDelta = yawDelta;
    plymResolveCollisionTurn(state);
    std::uint32_t mode = state.activeMovementMode;
    switch (mode) {
    case 0:
    case 5:
        break;
    case 1:
        if (state.interpolationValue > 0.1f)
            mode = 3;
        break;
    case 3:
    case 4:
        if (state.movementSpeedRatio < 0.001f) {
            mode = 1;
            if (!movement.dispatchMode) {
                const std::int32_t delta = static_cast<std::int32_t>(state.desiredYaw) - state.currentYaw;
                if (delta >= 0 ? delta > 910 : state.currentYaw - state.desiredYaw > 910)
                    mode = 2;
            }
            if (mode == 1 && state.interpolationValue > 0.1f)
                mode = 3;
        }
    case 2:
    case 6:
    default:
        movement.targetYaw = state.desiredYaw;
        break;
    }
    movement.requestedMode = mode;
    movement.movementResult = state.movementResult;
}


std::int32_t plymUpdateRegionInteraction(PlayerModelState &state) {
    PlayerMovementState *const movement = state.movementState;
    if (!(state.flags & 2U))
        return 0;
    Vector3 ingress, position, buttonPosition;
    switch (static_cast<signed char>(state.regionInteractionState)) {
    case 0:
        if (GetMediatedButtonBindings(SHEN_NAME(BTDR)) && state.searchRegionKey &&
                btnCheck(SHEN_NAME(BTDR)) && ClaimDoor(shen_name(state.searchRegionKey), ctrlGetScenarioCharaName(state.character))) {
            drBeginEntry(shen_name(state.searchRegionKey));
            return 0;
        }
        GetCharacterWorldPosition(state.character, &position);
        OffsetPointAlongYaw(&position, &position, 0.40000001f, state.currentYaw);
        state.searchRegionKey = GetNearbyDoorForNpc(&position);
        if ((state.searchRegionKey >> 8 & 0xFFU) == '5' && (g_currentMap & 0xFFU) == 'Q')
            state.searchRegionKey = 0;
        if (state.searchRegionKey) {
            if (srchCheckFlags(6) && static_cast<std::uint32_t>(srchCheckFlags(6)) != state.searchRegionKey)
                return 0;
            if (srchHasActiveLockonTarget())
                return 0;
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrix(doorFindEntryTransform(shen_name(state.searchRegionKey)));
            Matrix4x4::preTranslate(0.44999999f, 0.89999998f, 0.0f);
            matrix_stack::GetTranslation(&buttonPosition);
            PopCurrentMatrix(1);
            if (drIsDoorButtonAvailable(shen_name(state.searchRegionKey)))
                RegisterButtonMediation(SHEN_NAME(BTDR), &buttonPosition, 0x40005U);
        }
        return 0;
    case 1:
        state.activeMovementMode = 1;
        if (!IsHeldTransitionReady(state))
            return 0;
        ++state.regionInteractionState;
        break;
    case 2:
        GetCharacterWorldPosition(state.character, &position);
        ComputeDoorIngress(state.searchRegionKey, &position, &ingress);
        state.regionInteractionMotionId = GetNpcDoorAnimation(state.searchRegionKey, state.character, &position);
        if ((state.searchRegionKey >> 8 & 0xFFU) == '7')
            mtwkRequestPositionTransition(state.motion, &ingress, g_doorIngress.rotation.y, state.regionInteractionMotionId | 0x1000000U, 0.0f);
        else
            mtwkRequestPositionTransition(state.motion, &ingress, g_doorIngress.rotation.y, state.regionInteractionMotionId | 0x1000000U, 1.0f);
        toggleCCMflagsByAmt(state.character, 0x800U);
        ++state.regionInteractionState;
        break;
    case 3:
        if (!mtwkSetExtraFlag(state.motion, 58, 1)) {
            ++state.regionInteractionState;
            movement->flags |= 2U;
            if ((state.searchRegionKey >> 8 & 0xFFU) == '7')
                BeginDoorAnimation(state.searchRegionKey, ctrlGetScenarioCharaName(state.character), state.regionInteractionMotionId, 0.0f, 1.0f);
            else
                BeginDoorAnimation(state.searchRegionKey, ctrlGetScenarioCharaName(state.character), state.regionInteractionMotionId, 1.0f, 1.0f);
            ccmClearFlags(state.character, 0x80U);
            return 0;
        }
        break;
    case 4:
        if (!mtwkSetRegionFlag(state.motion, 65, 1, static_cast<MotionRegion>(0)))
            return 0;
        ++state.regionInteractionState;
    case 5:
    default:
        toggleCCMflagsByAmt(state.character, 0x80U);
        ccmClearFlags(state.character, 0x800U);
        movement->flags &= ~2U;
        state.regionInteractionState = 0;
        state.padFlags &= ~0x800U;
        break;
    }
    return 0;
}


#define PLACEMENT_MIN(a, b) ((a) < (b) ? (a) : (b))
#define PLACEMENT_MAX(a, b) ((a) > (b) ? (a) : (b))
#define PLACEMENT_ABS(x) ((x) < 0 ? -(x) : (x))

static inline std::int16_t ClampPlacementAngle(int value, int limit) {
    return PLACEMENT_MIN(PLACEMENT_MAX(-limit, value), limit);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Native plymUpdateRegionInteraction. Idle, a door-button press on the held region claims the door and begins its
// entry; otherwise the door 0.4 m ahead becomes the search region and shows the door button at its entry point.
// States 1..5 walk Ryo through a door whose entry the scenario scripts took over.
void plymUpdateRegionInteraction(PlayerModelState &state) {
    PlayerMovementState *const movement = state.movementState;
    if (!(state.flags & 2U))
        return;
    const PlayerModelState *const model = g_playerRuntime.model;
    if (model &&
        (((model->flags & 0x100U) && model->heldStateMode != 0) || (model->padFlags & 0x800U) || doorGetSearchRegion().name != 0 ||
            (g_playerRuntime.flags & 0x01000000U) || (model->padFlags & 0x00800000U) || !(g_playerRuntime.flags & 2U))) {
        return;
    }
    const bool automaticRegion = static_cast<std::uint8_t>(state.searchRegionKey >> 8U) == '7';
    switch (state.regionInteractionState) {
    case 0: {
        if (srchHasActiveLockonTarget() || ptycFindDoorCompanion({}).name != 0)
            return;
        if (state.rootYawState == 9U && std::abs(static_cast<std::int32_t>(movement->rootYawOffset)) > 0x4000)
            return;
        const shen_name held{state.searchRegionKey};
        if (GetMediatedButtonBindings(BUTTON_DOOR) && held.name && btnCheck(BUTTON_DOOR) &&
            ClaimDoor(held, FindShenmueNodeFromScenarioTask(state.characterNode)) && static_cast<std::int32_t>(drBeginEntry(held)) != 0) {
            return;
        }
        const Vector3 ahead = RotateAngleAxis(GetCharacterWorldPosition(state.characterNode), 0.40000001f, state.currentYaw);
        shen_name region = ComputeRegionKeyForPos(&ahead);
        if (static_cast<std::uint8_t>(region.name >> 8U) == '5' && static_cast<std::uint8_t>(g_currentMap.name) == 'Q')
            region = {};
        state.searchRegionKey = region.name;
        if (!state.searchRegionKey)
            return;
        if (static_cast<std::uint32_t>(srchCheckFlags(6)) != 0 && static_cast<std::uint32_t>(srchCheckFlags(6)) != state.searchRegionKey)
            return;
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(doorFindEntryTransform(state.searchRegionKey));
        Matrix4x4::preTranslate(0.44999999f, 0.89999998f, 0.0f);
        const Vector3 buttonPosition{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        if (drIsDoorButtonAvailable(shen_name{state.searchRegionKey}))
            RegisterButtonMediation(BUTTON_DOOR, &buttonPosition, (ButtonMediationShowButtonHint | ButtonMediationBindA | 0x1U));
        return;
    }
    case 1:
        state.activeMovementMode = 1;
        if (state.rootYawState == 0)
            state.regionInteractionState = 2;
        return;
    case 2: {
        const Vector3 position = GetCharacterWorldPosition(state.characterNode);
        Vector3 ingress{};
        ComputeDoorIngress(static_cast<std::int32_t>(state.searchRegionKey), &position, &ingress);
        state.regionInteractionMotionId = GetNpcDoorAnimation(static_cast<std::int32_t>(state.searchRegionKey), state.characterNode, &position);
        mtwkRequestPositionTransition(state.motionWork, &ingress, static_cast<std::uint16_t>(g_doorIngressRotation.y),
            state.regionInteractionMotionId | 0x01000000U, automaticRegion ? 0.0f : 1.0f);
        toggleCCMflagsByAmt(state.characterNode, 0x800U);
        ++state.regionInteractionState;
        return;
    }
    case 3:
        if (mtwkSetExtraFlag(state.motionWork, 0x3aU, 1) != 0)
            return;
        ++state.regionInteractionState;
        movement->flags |= 2U;
        BeginDoorAnimation(static_cast<std::int32_t>(state.searchRegionKey), FindShenmueNodeFromScenarioTask(state.characterNode),
            static_cast<std::int32_t>(state.regionInteractionMotionId), automaticRegion ? 0.0f : 1.0f, 1.0f);
        ccmSetFlagsFor(state.characterNode, 0x80U);
        return;
    case 4:
        if (!mtwkSetRegionFlag(state.motionWork, 65U, 1, MotionRegion::LowerBody))
            return;
        ++state.regionInteractionState;
        break;
    default:
        break;
    }
    toggleCCMflagsByAmt(state.characterNode, 0x80U);
    ccmSetFlagsFor(state.characterNode, 0x800U);
    movement->flags &= ~2U;
    state.padFlags &= ~0x800U;
    state.regionInteractionState = 0;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymUpdatePlacementNode(PlayerPlacementNode *node) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    am2Vector3Subtract(&node->delta, &node->position, &node->previousPosition);
    if (std::fabs(node->delta.x) >= 0.00000011920929f && std::fabs(node->delta.z) >= 0.00000011920929f)
        node->movementBearingYaw = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(node->delta.x, node->delta.z)) - 0x8000;
    else
        node->movementBearingYaw = 0;
    node->previousPosition = node->position;
    Vector3 position;
    GetCharacterWorldPosition(node->character, &position);
    const std::int16_t yaw = static_cast<std::int16_t>(GetCharacterWorldYaw(node->character));
    Vector3 local;
    am2Vector3Subtract(&local, &node->position, &position);
    float distance;
    distance = NativeSqrt(local.x * local.x + local.z * local.z);
    const std::int16_t bearing = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(local.x, local.z)) - 0x8000;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-yaw);
    Matrix4x4::Transform(local, local);
    PopCurrentMatrix(1);
    node->distance = distance;
    if (node->minimumDistance > distance)
        node->minimumDistance = distance;
    std::uint32_t flags = node->flags;
    if (local.z > 0.0f) {
        if (!(flags & 2U))
            flags |= 4U;
        flags |= 2U;
    } else {
        flags &= ~6U;
    }
    const float movement = am2Vector3LengthSquared(&node->delta);
    flags &= ~0x4000U;
    if (movement < 0.0001f) {
        ++node->stationaryFrames;
        node->stationaryFrames = node->stationaryFrames > 4 ? 4 : node->stationaryFrames;
        if (node->stationaryFrames == 4)
            flags |= 0x4000U;
    } else {
        node->stationaryFrames = 0;
    }
    node->flags = flags;
    node->distance = distance;
    node->characterBearingYaw = bearing;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    node->delta = {node->position.x - node->previousPosition.x, node->position.y - node->previousPosition.y, node->position.z - node->previousPosition.z};
    node->movementBearingYaw = std::fabs(node->delta.x) < 0.00000011920929f || std::fabs(node->delta.z) < 0.00000011920929f ?
        0 :
        static_cast<std::int16_t>(FixedAtan2(node->delta.x, node->delta.z) + 0x8000);
    node->previousPosition = node->position;
    const Vector3 position = GetCharacterWorldPosition(node->character);
    const std::int16_t yaw = GetAllAngYForNode(&node->character->entity.node);
    Vector3 delta{node->position.x - position.x, node->position.y - position.y, node->position.z - position.z};
    const float distance = std::sqrt(delta.z * delta.z + delta.x * delta.x);
    const std::int16_t bearing = static_cast<std::int16_t>(FixedAtan2(delta.x, delta.z) + 0x8000);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    delta = TransformWithActiveMatrix(delta);
    Matrix4x4::Pop(1);
    node->distance = distance;
    if (node->minimumDistance > distance)
        node->minimumDistance = distance;
    std::uint32_t flags = node->flags;
    if (delta.z <= 0.0f)
        flags &= 0xfffffff9U;
    else {
        if (!(flags & 2U))
            flags |= 4U;
        flags |= 2U;
    }
    flags &= ~0x4000U;
    if ((node->delta.y * node->delta.y + node->delta.x * node->delta.x) + node->delta.z * node->delta.z >= 0.0001f)
        node->stationaryFrames = 0;
    else {
        node->stationaryFrames = static_cast<std::int16_t>(node->stationaryFrames + 1);
        if (node->stationaryFrames >= 4)
            node->stationaryFrames = 4;
        if (node->stationaryFrames == 4)
            flags |= 0x4000U;
    }
    node->distance = distance;
    node->flags = flags;
    node->characterBearingYaw = bearing;
#endif
}


// @verified - d
PlayerPlacementNode *plymUpdatePlacementChain(PlayerPlacementNode *node) {
    if (!node)

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
        return 0;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        return nullptr;
#endif
    Character *const character = node->character;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    PlayerPlacementNode *last = node;
#endif
    plymUpdatePlacementNode(node);

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    for (node = last->next; node; node = last->next) {
        node->character = character;
        last = node;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    for (PlayerPlacementNode *next = node->next; next; next = next->next) {
        next->character = character;
        node = next;
#endif
        plymUpdatePlacementNode(node);
    }

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    return last;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    return node;
#endif
}


// @verified - d
float plymGetApproachStepDistance(float previous, const Vector3 &delta) {

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    float change = NativeSqrt(delta.x * delta.x + delta.z * delta.z) - previous;
    change = (change < -0.050000001f ? -0.050000001f : change) < 0.050000001f ? (change < -0.050000001f ? -0.050000001f : change) : 0.050000001f;
    previous = InterpolateTo(previous + change, previous, 0.5f);
    return previous < 0.18000001f ? previous : 0.18000001f;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    float change = std::fmax(-0.050000001f, std::sqrt(delta.x * delta.x + delta.z * delta.z) - previous);
    if (change >= 0.050000001f)
        change = 0.050000001f;
    return std::fmin(Lerp(change + previous, previous, shenGetScaledDeltaTickTime(0.5f)), 0.18000001f);
#endif
}


int plymResolvePlacementMovement(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    PlayerPlacementNode *const node = state.selectedPlacementNode;
    float result = 0.0f;
    std::uint32_t mode;
    std::int16_t yaw;
    int code;
    if (node->flags & 0x800U) {
        mode = 3;
        yaw = node->characterBearingYaw;
        result = state.approachStepDistance + state.approachStepDistance;
        code = 99;
    } else {
        if (node->flags & 0x4000U) {
            state.activeMovementMode = 1;
            return -1;
        }
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(-node->targetYaw);
        matrix_stack::TranslateInverseVector(&node->position);
        Vector3 local;
        Matrix4x4::TransformPoint(state.searchLocation, local);
        PopCurrentMatrix(1);
        float x = local.x;
        if (x < 0.0f)
            x = -x;
        float z = local.z;
        if (z < 0.0f)
            z = -z;
        const int facing = static_cast<std::int16_t>(node->characterBearingYaw - state.currentYaw);
        const int target = static_cast<std::int16_t>(node->targetYaw - state.currentYaw);
        SetDebugScreenTextPosition(10, 11);
        float ratio;
        float amount;
        if (node->flags & 2U) {
            if (PLACEMENT_ABS(target) < 0x1555) {
                if (node->distance > 3.0f) {
                    mode = 1;
                    yaw = state.currentYaw;
                    code = 0;
                } else {
                    mode = 3;
                    ratio = x / 3.0f;
                    const float scale = PLACEMENT_MIN(PLACEMENT_MAX(0.0f, ratio), 1.0f);
                    const std::int16_t maximum = static_cast<std::int16_t>(scale * 45.0f * 8192.0f / 45.0f);
                    yaw = static_cast<std::int16_t>(state.currentYaw + ClampPlacementAngle(facing, maximum));
                    ratio = z / 3.0f;
                    amount = PLACEMENT_MIN(PLACEMENT_MAX(0.0f, ratio), 1.0f);
                    result = state.approachStepDistance * (1.0f - amount) * 0.89999998f;
                    code = 1;
                    if (IsHeldTransitionReady(state)) {
                        mode = 1;
                        code = 11;
                    }
                }
            } else {
                mode = 3;
                if (node->distance > 3.0f) {
                    yaw = node->characterBearingYaw;
                    result = 1.0f;
                    code = 2;
                } else {
                    yaw = CameraLerpAngle(node->targetYaw, state.currentYaw, 0.30000001f);
                    ratio = z / 3.0f;
                    amount = PLACEMENT_MIN(PLACEMENT_MAX(0.0f, ratio), 1.0f);
                    result = state.approachStepDistance * (1.0f - amount) * 0.89999998f;
                    code = 3;
                }
            }
        } else if (PLACEMENT_ABS(target) < 0x1555) {
            mode = 3;
            if (node->distance > 3.0f) {
                yaw = node->characterBearingYaw;
                result = 1.0f;
                code = 4;
            } else {
                ratio = x / 3.0f;
                ratio = PLACEMENT_MIN(PLACEMENT_MAX(0.0f, ratio), 1.0f);
                int limit = 0x1555;
                yaw = static_cast<std::int16_t>(state.currentYaw + ClampPlacementAngle(facing, limit));
                yaw = static_cast<std::int16_t>(static_cast<int>(static_cast<float>(static_cast<std::int16_t>(node->targetYaw - yaw)) * (1.0f - ratio)) + yaw);
                if (PLACEMENT_ABS(facing) < 0x2000)
                    result = state.approachStepDistance;
                else {
                    ratio = PLACEMENT_MIN(PLACEMENT_MAX(0.5f, 1.0f - x), 1.0f);
                    result = ratio * state.approachStepDistance;
                }
                ratio = z / 3.0f;
                amount = PLACEMENT_MIN(PLACEMENT_MAX(0.0f, ratio), 1.0f);
                result *= 1.0f + amount;
                code = 5;
            }
        } else {
            mode = 3;
            if (node->distance > 3.0f) {
                yaw = node->characterBearingYaw;
                result = state.approachStepDistance + state.approachStepDistance;
                code = 6;
            } else {
                ratio = x / 3.0f;
                amount = PLACEMENT_MIN(PLACEMENT_MAX(0.0f, ratio * 2.0f), 1.0f);
                yaw = static_cast<std::int16_t>(state.currentYaw + static_cast<std::int16_t>(static_cast<float>(facing) * amount));
                ratio = z / 3.0f;
                amount = PLACEMENT_MIN(PLACEMENT_MAX(0.0f, ratio), 1.0f);
                result = state.approachStepDistance * 0.89999998f;
                code = 7;
                if (IsHeldTransitionReady(state)) {
                    yaw = node->characterBearingYaw;
                    code = 71;
                }
            }
        }
    }
    state.activeMovementMode = mode;
    state.desiredYaw = yaw;
    state.movementResult = result;
    if (state.movementInterpolation > 0.0f && state.movementInterpolation == state.motionInputEcho)
        state.movementResult = 0.0f;
    return code;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    const PlayerPlacementNode &node = *state.selectedPlacementNode;
    float result = 0.0f;
    std::uint32_t mode = 3;
    std::int16_t yaw = node.characterBearingYaw;
    int code = 99;
    if (node.flags & 0x800U)
        result = state.approachStepDistance + state.approachStepDistance;
    else {
        if (node.flags & 0x4000U) {
            state.activeMovementMode = 1;
            return -1;
        }
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-node.targetYaw));
        Matrix4x4::preTranslate(-node.position.x, -node.position.y, -node.position.z);
        const Vector3 local = TransformWithActiveMatrix(state.searchLocation);
        Matrix4x4::Pop(1);
        const float x = std::fabs(local.x), z = std::fabs(local.z);
        const std::int16_t facing = static_cast<std::int16_t>(node.characterBearingYaw - state.currentYaw);
        const std::int16_t target = static_cast<std::int16_t>(node.targetYaw - state.currentYaw);
        SetDebugStringPosition(10, 11);
        const int absoluteTarget = std::abs(static_cast<int>(target));
        if (node.flags & 2U) {
            if (absoluteTarget < 5461) {
                if (node.distance <= 3.0f) {
                    float amount = std::fmax(0.0f, x / 3.0f);
                    if (amount >= 1.0f)
                        amount = 1.0f;
                    const std::int16_t maximum = static_cast<std::int16_t>(static_cast<int>(((amount * 45.0f) * 8192.0f) / 45.0f));
                    std::int16_t turn = facing;
                    if (turn < -maximum)
                        turn = static_cast<std::int16_t>(-maximum);
                    if (turn > maximum)
                        turn = maximum;
                    yaw = static_cast<std::int16_t>(state.currentYaw + turn);
                    amount = std::fmax(0.0f, z / 3.0f);
                    if (amount >= 1.0f)
                        amount = 1.0f;
                    result = ((1.0f - amount) * state.approachStepDistance) * 0.89999998f;
                    code = 1;
                    if (!state.rootYawState) {
                        mode = 1;
                        code = 11;
                    }
                } else {
                    yaw = state.currentYaw;
                    mode = 1;
                    code = 0;
                }
            } else if (node.distance <= 3.0f) {
                yaw = LerpFixedAngle(node.targetYaw, state.currentYaw, shenGetScaledDeltaTickTime(0.30000001f));
                float amount = std::fmax(0.0f, z / 3.0f);
                if (amount >= 1.0f)
                    amount = 1.0f;
                result = ((1.0f - amount) * state.approachStepDistance) * 0.89999998f;
                code = 3;
            } else {
                result = 1.0f;
                code = 2;
            }
        } else if (absoluteTarget >= 5461) {
            if (node.distance <= 3.0f) {
                float amount = std::fmax(0.0f, (x / 3.0f) + (x / 3.0f));
                if (amount >= 1.0f)
                    amount = 1.0f;
                result = state.approachStepDistance * 0.89999998f;
                yaw = static_cast<std::int16_t>(state.currentYaw + static_cast<int>(static_cast<float>(facing) * amount));
                code = 7;
                if (!state.rootYawState) {
                    code = 71;
                    yaw = node.characterBearingYaw;
                }
            } else {
                result = state.approachStepDistance + state.approachStepDistance;
                code = 6;
            }
        } else if (node.distance > 3.0f) {
            result = 1.0f;
            code = 4;
        } else {
            float amount = std::fmax(0.0f, x / 3.0f);
            if (amount >= 1.0f)
                amount = 1.0f;
            int base = facing;
            if (base < -5461)
                base = -5461;
            if (base > 5461)
                base = 5461;
            yaw = static_cast<std::int16_t>(static_cast<int>((1.0 - static_cast<double>(amount)) *
                                                static_cast<double>(static_cast<std::int16_t>(node.targetYaw - (base + state.currentYaw)))) +
                base + state.currentYaw);
            float step = state.approachStepDistance;
            if (std::abs(static_cast<int>(facing)) >= 0x2000) {
                float rate = std::fmax(0.5f, 1.0f - x);
                if (rate >= 1.0f)
                    rate = 1.0f;
                step = rate * step;
            }
            amount = std::fmax(0.0f, z / 3.0f);
            if (amount >= 1.0f)
                amount = 1.0f;
            result = step * (amount + 1.0f);
            code = 5;
        }
    }
    state.activeMovementMode = mode;
    state.desiredYaw = yaw;
    state.movementResult = result;
    if (state.normalizedMotionInput > 0.0f && state.normalizedMotionInput == state.motionInputEcho)
        state.movementResult = 0.0f;
    return code;
#endif
}
#endif

} // namespace player_detail

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
std::int16_t plymGetAuthorPlacementYaw(shen_name name) {
    Character *const character = authFindChara(name);
    const PartyCompanion *const party = character ? FindPartyCompanion(character) : nullptr;
    if (!party)
        return 0;
    if (party->mode == 2)
        return party->routeYaw;
    const PartyPlacementNode *selected = &party->placement;
    for (const PartyPlacementNode *node = party->placement.next; node; node = node->next) {
        if (node->flags & 0x20000U)
            break;
        selected = node;
    }
    return InverseFixedAtan2(selected->position.x - party->position.x, selected->position.z - party->position.z);
}
#endif

namespace player_detail {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
int plymBuildAuthorPlacementContext(PlayerModelState &state) {
    PlayerPlacementNode &root = state.placementRoot;
    root.flags |= 0x80000000U;
    if (!state.specialModeAuthorName.name)
        return -1;
    Character *const author = authFindChara(state.specialModeAuthorName);
    if (!author)
        return -1;
    Vector3 player = GetCharacterWorldPosition(state.characterNode);
    player.y += 0.40000001f;
    Vector3 target = GetCharacterWorldPosition(author);
    root.targetCharacterPosition = target;
    target.y += 0.40000001f;
    const std::int16_t authorYaw = GetAllAngYForNode(&author->entity.node);
    const std::int16_t bearing = plymGetAuthorPlacementYaw(state.specialModeAuthorName);
    root.targetPosition = target;
    const CharacterControlModel *const control = FindCharacterControlModel(author);
    const Vector3 delta = control ? control->frameDisplacement : Vector3{};
    if (plymGetPartyAuthorStatus(state.specialModeAuthorName) > 0)
        root.flags |= 0x80000U;
    root.flags &= 0xfffedfffU;
    if (!((delta.y * delta.y + delta.x * delta.x) + delta.z * delta.z >= 0.000225f)) {
        root.flags |= 0x12000U;
        root.targetYaw = InverseFixedAtan2(target.x - root.position.x, target.z - root.position.z);
        return 0;
    }
    Vector3 local{player.x - target.x, player.y - target.y, player.z - target.z};
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-bearing));
    local = TransformWithActiveMatrix(local);
    Matrix4x4::Pop(1);
    const std::int16_t phase = static_cast<std::int16_t>(state.specialModeOrbitPhase);
    state.specialModeOrbitPhase = (state.specialModeOrbitPhase & ~0xffff) | static_cast<std::uint16_t>(phase + 182);
    const float longitudinal =
        ((FixedAngleSin(static_cast<std::uint16_t>(phase)) + 1.0f) * ((state.specialModeOrbitMaximum - state.specialModeOrbitMinimum) * 0.5f)) +
        state.specialModeOrbitMinimum;
    float lateral = ((FixedAngleCos(static_cast<std::uint16_t>(static_cast<int>(static_cast<float>(phase) * 1.5f))) + 1.0f) * 0.15000001f) + 0.69999999f;
    if (local.x > 0.0f)
        lateral *= -1.0f;
    Vector3 candidate{}, hit{};
    for (int attempt = 0; attempt < 3; ++attempt) {
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::preTranslate(target.x, target.y, target.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(bearing));
        Matrix4x4::preTranslate(attempt == 0 ? -lateral : attempt == 1 ? -(-lateral) : 0.0f, 0.0f, longitudinal);
        candidate = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        if (attempt == 2)
            break;
        flddToggleFlag(8);
        if (!RaycastForCollision(&hit, &candidate, &target))
            break;
    }
    Vector3 previous = root.previousPosition;
    std::int16_t previousYaw = root.targetYaw;
    if (root.flags & 0x200U) {
        previous = target;
        previousYaw = authorYaw;
        root.flags &= ~0x200U;
    }
    root.position.x = Lerp(candidate.x, previous.x, shenGetScaledDeltaTickTime(0.2f));
    root.position.y = Lerp(candidate.y, previous.y, shenGetScaledDeltaTickTime(0.2f));
    root.position.z = Lerp(candidate.z, previous.z, shenGetScaledDeltaTickTime(0.2f));
    root.targetYaw = LerpFixedAngle(bearing, previousYaw, shenGetScaledDeltaTickTime(0.40000001f));
    flddToggleFlag(1064);
    const bool blocked = GlobalRayIntersectionTest(player, root.position, 0.375f);
    const bool guide = InitializePartyGuide() != 0;
    if (guide) {
        SetPartyGuideRouteHint(state.placementQueryHandle);
        state.placementQueryHandle = PickClosestTargetForPlacement(&state.searchLocation);
    }
    if (!blocked || !guide)
        return 0;
    InitializePartyGuide();
    const PartyCompanion *const companion = FindPartyCompanion(authFindChara(state.interactionTarget));
    const int route = companion ? companion->routeIndex : 0;
    SetPartyGuideRouteHint(route);
    PickClosestTargetForPlacement(&target);
    SetPartyGuideRouteHint(state.placementQueryHandle);
    FindClosestTargetForPlacement(&player);
    SetPartyGuideDirection();
    std::int16_t sampledYaw{};
    if (PartyGuideEndState() > 0)
        goto reverseGuide;
    for (;;) {
        SamplePartyGuide(&candidate, &sampledYaw);
        candidate.y += 0.40000001f;
        flddToggleFlag(1064);
        const bool wall = GlobalRayIntersectionTest(player, candidate, 0.375f);
        ccmUpdateEntityFlag(state.characterNode);
        ccmUpdateEntityFlag(author);
        const bool character = CheckPointInCylinder_Wrapper(player, candidate) != nullptr;
        if (!(wall | character))
            goto publish;
        AdvancePartyGuide(4.0f);
        if (PartyGuideEndState() > 0)
            goto reverseGuide;
    }
reverseGuide:
    InitializePartyGuide();
    SetPartyGuideRouteHint(state.placementQueryHandle);
    PickClosestTargetForPlacement(&player);
    SetPartyGuideRouteHint(route);
    FindClosestTargetForPlacement(&target);
    SetPartyGuideDirection();
    AdvancePartyGuide(10.0f);
    SetPartyGuideRouteHint(state.placementQueryHandle);
    FindClosestTargetForPlacement(&player);
    SetPartyGuideDirection();
    SamplePartyGuide(&candidate, &sampledYaw);
    candidate.y += 0.40000001f;
    flddToggleFlag(1064);
    {
        const bool wall = GlobalRayIntersectionTest(player, candidate, 0.375f);
        ccmUpdateEntityFlag(state.characterNode);
        ccmUpdateEntityFlag(author);
        const bool character = CheckPointInCylinder_Wrapper(player, candidate) != nullptr;
        if (!(wall | character))
            goto publish;
    }
    while (PartyGuideEndState() <= 0) {
        AdvancePartyGuide(1.0f);
        SamplePartyGuide(&candidate, &sampledYaw);
        candidate.y += 0.40000001f;
        flddToggleFlag(1064);
        const bool wall = GlobalRayIntersectionTest(player, candidate, 0.375f);
        ccmUpdateEntityFlag(state.characterNode);
        ccmUpdateEntityFlag(author);
        const bool character = CheckPointInCylinder_Wrapper(player, candidate) != nullptr;
        if (!(wall | character))
            goto publish;
    }
    root.flags |= 8U;
    return 0;
publish:
    state.placementScratch.position = candidate;
    root.next = &state.placementScratch;
    return 0;
}

// @verified - p
void plymUpdateAuthorPlacementTarget(PlayerModelState &state) {
    PlayerPlacementNode *const node = plymUpdatePlacementChain(&state.placementRoot);
    state.activePlacementNode = node;
    state.selectedPlacementNode = node;
    Character *const author = !(node->flags & 0x800U) && (node->flags & 0x80000U) && !state.rootYawState ? authFindChara(state.specialModeAuthorName) : nullptr;
    if (!author) {
        if ((state.specialModeFlags & 0x8000U) && (g_playerRuntime.flags & 0x40000U) && g_playerRuntime.model)
            g_playerRuntime.model->specialModeFlags &= ~0x10U;
    } else {
        Vector3 position{};
        charaGetMotWorldPos(author, 6, &position);
        if ((g_playerRuntime.flags & 0x40000U) && g_playerRuntime.model) {
            g_playerRuntime.model->specialModeState = 2;
            g_playerRuntime.model->specialModeTargetPosition = position;
            ++g_playerRuntime.model->specialModeStep;
        }
        state.specialModeFlags |= 0x8000U;
    }
    state.approachStepDistance = plymGetApproachStepDistance(state.approachStepDistance, node->delta);
    plymResolvePlacementMovement(state);
    if (state.movementResult != 0.0f)
        state.movementResult = Lerp(state.movementResult, state.previousMovementResult, shenGetScaledDeltaTickTime(0.5f));
}

// @verified - p
bool plymRecordPlacementHistory(PlayerModelState &state, const Vector3 &position) {
    const float distance = std::sqrt(PlymSquaredDistance(position, state.previousPlacementSample)) + state.placementHistoryDistanceAccumulator;
    state.placementHistoryDistanceAccumulator = distance;
    int previous = static_cast<std::int8_t>(state.placementHistoryWriteIndex) - 1;
    if (previous < 0)
        previous = 31;
    const bool record = !state.placementHistoryCount || (distance > 1.0f && PlymSquaredDistance(state.placementHistory[previous], position) > 0.80000001f);
    if (record) {
        state.placementHistory[static_cast<std::int8_t>(state.placementHistoryWriteIndex++)] = position;
        if (static_cast<std::int8_t>(state.placementHistoryWriteIndex) >= 32)
            state.placementHistoryWriteIndex = 0;
        state.placementHistoryDistanceAccumulator = 0.0f;
        ++state.placementHistoryCount;
        if (static_cast<std::int8_t>(state.placementHistoryCount) > 32)
            state.placementHistoryCount = 32;
    }
    state.previousPlacementSample = position;
    return record;
}

// @verified - p
const Vector3 *plymGetPlacementFromHistory(const PlayerModelState &state, int count) {
    const int available = static_cast<std::int8_t>(state.placementHistoryCount);
    if (!available || !count)
        return nullptr;
    const int direction = count < 0 ? -1 : 1;
    if (count < 0)
        count = -count;
    if (count > 32)
        return nullptr;
    int write = static_cast<std::int8_t>(state.placementHistoryWriteIndex);
    if (count > available)
        count = available;
    if (available < 32 && direction < 0)
        write = 0;
    int index = (direction < 0 ? write - 1 : write) - count * direction;
    if (index < 0)
        index += 31;
    if (index > 31)
        index -= 32;
    return &state.placementHistory[index];
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymUpdateScriptedPlacementTarget(PlayerModelState &state) {
    state.motionInputEcho = 0.0f;
    PlayerPlacementNode *const node = plymUpdatePlacementChain(&state.placementRoot);

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    state.selectedPlacementNode = state.activePlacementNode = node;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    state.activePlacementNode = node;
    state.selectedPlacementNode = node;
#endif
    const std::int16_t bearing = node->characterBearingYaw;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    const float &distance = node->distance;
#endif
    state.approachStepDistance = plymGetApproachStepDistance(state.approachStepDistance, node->delta);
    if (node->flags & 0x4000U) {
        state.desiredYaw = bearing;

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
        state.activeMovementMode = 3;
        if (distance < 0.25f)
            state.activeMovementMode = 1;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
        state.activeMovementMode = node->distance < 0.25f ? 1 : 3;
#endif
    } else {
        plymResolvePlacementMovement(state);
        if (state.movementResult != 0.0f)

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
            state.movementResult = InterpolateTo(state.movementResult, state.previousMovementResult, 0.5f);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
            state.movementResult = Lerp(state.movementResult, state.previousMovementResult, shenGetScaledDeltaTickTime(0.5f));
#endif
    }
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
bool plymRecordPlacementHistory(PlayerModelState &state, const Vector3 &position) {
    bool recorded = false;
    bool record = false;
    Vector3 delta;
    am2Vector3Subtract(&delta, &position, &state.previousPlacementSample);
    state.placementHistoryDistanceAccumulator += am2Vector3Length(&delta);
    if (!state.placementHistoryCount)
        record = true;
    else if (state.placementHistoryDistanceAccumulator > 1.0f) {
        std::int32_t previous = state.placementHistoryWriteIndex - 1;
        if (previous < 0)
            previous = 31;
        if (model_component_detail::Vector3_ComputeSquaredDistance(state.placementHistory[previous], position) > 0.80000001f)
            record = true;
    }
    if (record) {
        state.placementHistory[state.placementHistoryWriteIndex] = position;
        if (++state.placementHistoryWriteIndex > 31)
            state.placementHistoryWriteIndex = 0;
        if (++state.placementHistoryCount > 32)
            state.placementHistoryCount = 32;
        state.placementHistoryDistanceAccumulator = 0.0f;
        recorded = true;
    }
    state.previousPlacementSample = position;
    return recorded;
}


// @verified - d
const Vector3 *plymGetPlacementFromHistory(const PlayerModelState &state, std::int32_t count) {
    std::int32_t direction = 1;
    if (!state.placementHistoryCount || !count)
        return 0;
    if (count < 0) {
        direction = -1;
        count = -count;
    }
    if (count > 32)
        return 0;
    if (count > state.placementHistoryCount)
        count = state.placementHistoryCount;
    std::int32_t index = state.placementHistoryWriteIndex;
    if (state.placementHistoryCount < 32 && direction < 0)
        index = 0;
    if (direction < 0)
        --index;
    index -= count * direction;
    if (index < 0)
        index += 31;
    if (index > 31)
        index -= 32;
    return &state.placementHistory[index];
}


static const Vector3 plymRayRightStart = {0.34999999f, 0.0f, 0.0f};
static const Vector3 plymRayLeftStart = {-0.34999999f, 0.0f, 0.0f};
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
void plymUpdateLockOnCharacter(PlayerModelState &state) {
    using enum CameraState;
    const CameraState camera = camCtrlGetState();
    const shen_name previous = state.lockOnCharacterName;
    const std::uint32_t previousFlags = state.flags;
    state.lockOnCharacterName = {};
    const shen_name target{static_cast<std::uint32_t>(srchCheckFlags(6))};
    if (authFindChara(target)) {
        state.lockOnCharacterName = target;
        if (previous.name != target.name) {
            state.placementHistoryDistanceAccumulator = 0.0f;
            state.placementHistoryWriteIndex = 0;
            state.placementHistoryCount = 0;
        }
    }
    if (state.flags & plymControlLockOnMovement) {
        if (camera != LockedOn)
            state.flags &= ~plymControlLockOnMovement;
        if (!target.name || !IsNpcRouteActive(target))
            state.flags &= ~plymControlLockOnMovement;
    } else if (camera == LockedOn && target.name && IsNpcRouteActive(target))
        state.flags |= plymControlLockOnMovement;
    if (state.flags & plymControlLockOnMovement) {
        if (!(GetCharacterControlModelFlags(state.characterNode) & 0x800U))
            toggleCCMflagsByAmt(state.characterNode, 0x800U);
    } else if (!(state.movementState->flags & 8U) && (previousFlags & plymControlLockOnMovement))
        ccmSetFlagsFor(state.characterNode, 0x800U);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymUpdateLockOnMovement(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    Character *character;
    shen_name name;
    PlayerPlacementNode *node;
    std::int32_t visible;
    node = &state.placementRoot;
    visible = 1;
    name = state.lockOnCharacterName;
    character = ctrlGetScenarioChara(name);
    if (!character)
        return;
    Vector3 position;
    GetCharacterWorldPosition(character, &position);
    plymRecordPlacementHistory(state, position);
    Vector3 elevated;
    Vector3 selected = position;
    elevated.x = selected.x;
    elevated.y = selected.y + 0.40000001f;
    elevated.z = selected.z;
    ccmUpdateEntityFlag(character);
    std::int32_t blocked = flddRaycastScene(state, state.worldPosition, elevated);
    if (blocked) {
        std::int32_t found = 0;
        float nearest = FLT_MAX;
        Vector3 nearestPosition;
        const Vector3 *history;
        for (std::int32_t index = 0; index < state.placementHistoryCount && (history = plymGetPlacementFromHistory(state, index + 1)); ++index) {
            const float distance = model_component_detail::Vector3_ComputeSquaredDistance(state.worldPosition, *history);
            if (distance < nearest) {
                nearest = distance;
                nearestPosition = *history;
            }
            elevated.x = history->x;
            elevated.y = history->y + 0.40000001f;
            elevated.z = history->z;
            ccmUpdateEntityFlag(character);
            blocked = flddRaycastScene(state, state.worldPosition, elevated);
            if (!blocked) {
                selected = *history;
                found = 1;
                break;
            }
        }
        if (!found && nearest != FLT_MAX)
            selected = nearestPosition;
    }
    Vector3 left, right;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(position.x, position.y + 0.40000001f, position.z);
    Matrix4x4::RotateAroundY_Wrapper(CopyHeading(state.worldPosition, position));
    Matrix4x4::TransformPoint(plymRayLeftStart, left);
    Matrix4x4::TransformPoint(plymRayRightStart, right);
    PopCurrentMatrix(1);
    flddToggleFlag(0x428);
    blocked = TestFieldWorldSegment(&state.worldPosition, &left) & 1;
    flddToggleFlag(0x428);
    blocked &= TestFieldWorldSegment(&state.worldPosition, &right);
    if (blocked)
        visible = 0;
    node->position = selected;
    node->targetYaw = GetCharacterWorldYaw(character);
    const float distance = CalculateDistanceBetweenPoints(&state.searchLocation, &position);
    node = plymUpdatePlacementChain(&state.placementRoot);
    state.approachStepDistance = plymGetApproachStepDistance(state.approachStepDistance, node->delta);
    if (distance < 2.0f)
        state.movementResult = state.approachStepDistance * 0.94999999f;
    std::int32_t mode = state.currentMovementMode;
    switch (mode) {
    case 3:
    case 4:
        if (!reinterpret_cast<const std::int8_t *>(&state.packedDirection)[0]) {
            if (reinterpret_cast<const std::int8_t *>(&state.packedDirection)[1] > 0)
                mode = 1;
            break;
        }
    default:
        mode = 1;
    }
    if (mode == 1) {
        if (distance > 4.0f)
            mode = 3;
        if (!visible && distance > 2.0f)
            mode = 3;
        if (QueryNpcRouteLiveState(name) == 11 && distance > 1.75f)
            mode = 3;
    }
    if (mode == 3 && (distance > 6.0f || !visible))
        state.motionInputEcho = 1.0f;
    if (distance < 3.0f && !GetNpcRouteMotionMode(name))
        state.motionInputEcho = 0.0f;
    state.activeMovementMode = mode;
    state.desiredYaw = node->characterBearingYaw;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    Character *const target = authFindChara(state.lockOnCharacterName);
    if (!target)
        return;
    const Vector3 position = GetCharacterWorldPosition(target);
    plymRecordPlacementHistory(state, position);
    Vector3 selected = position;
    Vector3 elevated = position;
    elevated.y += 0.40000001f;
    state.collisionFlags |= 4U;
    ccmUpdateEntityFlag(target);
    if (flddRaycastScene(state, state.worldPosition, elevated)) {
        float nearest = std::numeric_limits<float>::max();
        Vector3 nearestPosition{};
        for (int index = 1; index <= static_cast<std::int8_t>(state.placementHistoryCount); ++index) {
            const Vector3 *const history = plymGetPlacementFromHistory(state, index);
            if (!history)
                break;
            const float distance = PlymSquaredDistance(state.worldPosition, *history);
            if (nearest > distance) {
                nearest = distance;
                nearestPosition = *history;
            }
            elevated = *history;
            elevated.y += 0.40000001f;
            state.collisionFlags |= 4U;
            ccmUpdateEntityFlag(target);
            if (!flddRaycastScene(state, state.worldPosition, elevated)) {
                selected = *history;
                goto selectedHistory;
            }
        }
        if (nearest != std::numeric_limits<float>::max())
            selected = nearestPosition;
    }
selectedHistory:
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(position.x, position.y + 0.40000001f, position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(InverseFixedAtan2(position.x - state.worldPosition.x, position.z - state.worldPosition.z)));
    const Vector3 left = TransformWithActiveMatrix(plymRayLeftStart);
    const Vector3 right = TransformWithActiveMatrix(plymRayRightStart);
    Matrix4x4::Pop(1);
    flddToggleFlag(1064);
    const bool leftBlocked = flddFindIntersections(state.worldPosition, left);
    flddToggleFlag(1064);
    const bool rightBlocked = flddFindIntersections(state.worldPosition, right);
    const bool visible = !(leftBlocked & rightBlocked);
    state.placementRoot.position = selected;
    state.placementRoot.targetYaw = GetAllAngYForNode(&target->entity.node);
    const float distance = GetEuclideanDistance(&state.searchLocation, &position);
    const PlayerPlacementNode *const node = plymUpdatePlacementChain(&state.placementRoot);
    state.approachStepDistance = plymGetApproachStepDistance(state.approachStepDistance, node->delta);
    if (distance < 2.0f)
        state.movementResult = state.approachStepDistance * 0.94999999f;
    std::uint32_t mode = state.movementMode;
    const std::int8_t x = static_cast<std::int8_t>(state.packedDirection);
    const std::int8_t y = static_cast<std::int8_t>(state.packedDirection >> 8U);
    if ((mode - 3U) > 1U || x || y > 0) {
        mode = distance > 4.0f ? 3 : 1;
        if (!visible && distance > 2.0f)
            mode = 3;
        if (QueryNpcRouteLiveState(state.lockOnCharacterName) == 11 && distance > 1.75f)
            mode = 3;
    }
    if (mode == 3 && (distance > 6.0f || !visible))
        state.motionInputEcho = 1.0f;
    if (distance < 3.0f)
        state.motionInputEcho = 0.0f;
    if (!(distance >= (state.rootYawState ? 1.25f : 1.75f))) {
        mode = 1;
        state.motionInputEcho = 0.0f;
    }
    if (mode == 1 && QueryNpcRouteLiveState(state.lockOnCharacterName) == 11 &&
        std::abs(static_cast<int>(static_cast<std::int16_t>(node->characterBearingYaw - state.currentYaw))) > 10922)
        mode = 2;
    state.activeMovementMode = mode;
    state.desiredYaw = node->characterBearingYaw;
#endif
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
// @verified - d
std::int32_t plymUpdateSpecialMode(PlayerModelState &state) {
    PlayerMovementState *const movement = state.movementState;
    std::int32_t result = 0;
    const std::uint16_t entryFlags = state.specialModeFlags;
    if ((state.specialModeFlags & 8U) && state.motionState != 9)
        state.specialModeFlags &= ~8U;
    if (!(state.flags & 0x40000U))
        return result;
    if (state.specialModeFlags & 0x1000U) {
        state.specialModeFlags &= ~0x1000U;
        if (state.specialModeTargetCharacter && QueryNpcCameraMotionProfile(state.specialModeTargetCharacter, 4))
            state.specialModeState = 8;
    }
    if ((state.specialModeFlags & 1U) == 0) {
        if (state.specialModeFlags & 0x2000U) {
            state.specialModeFlags &= ~0x2000U;
            if (state.specialModeFlags & 0x800U)
                state.specialModeState = 9;
        }
        if (!(state.specialModeFlags & 0x400U) && !state.specialModeState) {
            if (state.specialModeFlags & 0x40U) {
                state.specialModeFlags &= ~0x40U;
                state.specialModeState = 6;
            }
        }
    }
    if (state.specialModeFlags & 0x100U) {
        if (!(state.specialModeFlags & 0x400U) && !state.specialModeState && !HasScenarioCharacterControl()) {
            SetPlayerSpecialMode(-1);
            return result;
        }
    }
    if (state.interactionTarget == SHEN_NAME4('S', 'I', 'N', '_') && !(state.placementRoot.flags & 0x10000U))
        state.padFlags |= 0x80000U;
    state.activeMovementMode = 1;
    if (state.specialModeFlags & 2U) {
        plymBuildAuthorPlacementContext(state);
        plymUpdateAuthorPlacementTarget(state);
    }
    switch (static_cast<std::int8_t>(state.specialModeState)) {
    case 0:
    default:
        state.specialModeState = 0;
        break;
    case 2: {
        state.specialModeState = 0;
        state.specialModeFlags = (state.specialModeFlags | 0x10U) & ~0x20U;
        state.specialModeFlags &= ~0x80U;
        const std::int16_t yaw = CopyHeading(state.searchLocation, state.specialModeTargetPosition);
        std::int32_t angle = static_cast<std::int16_t>(yaw - state.currentYaw);
        if (state.specialModeFlags & 2U)
            break;
        if (entryFlags & 0x80U) {
            if ((angle < 0 ? -angle : angle) > 0x4000 && IsHeldTransitionReady(state)) {
                state.activeMovementMode = 2;
                state.desiredYaw = yaw;
                Character *const character = ctrlGetScenarioChara(GetCameraSpecialTargetName());
                if (character) {
                    Vector3 position;
                    GetCharacterWorldPosition(character, &position);
                    const std::int16_t targetYaw = CopyHeading(position, state.specialModeTargetPosition);
                    const std::int16_t playerYaw = CopyHeading(state.searchLocation, position);
                    angle = static_cast<std::int16_t>(playerYaw - targetYaw);
                    if (angle < 0)
                        angle = -0x4000;
                    else
                        angle = 0x4000;
                    state.desiredYaw = static_cast<std::int16_t>(targetYaw + angle);
                }
            }
            state.specialModeFlags |= 0x40U;
        } else {
            std::int32_t limit = 0x3555;
            if (!(state.specialModeFlags & 0x800U)) {
                if (static_cast<std::int8_t>(state.retainedHeldFlags) > 1)
                    limit = 0x4e38;
                if ((angle < 0 ? -angle : angle) > limit && IsHeldTransitionReady(state)) {
                    state.activeMovementMode = 2;
                    state.desiredYaw = yaw;
                }
            }
        }
        break;
    }
    case 5:
        state.specialModeState = 0;
        state.placementRoot.flags |= 0x200U;
        break;
    case 3:
        state.specialModeState = 0;
        state.specialModeFlags |= 4U;
        state.specialModeAuxState = 0;
        break;
    case 4:
        state.specialModeState = 0;
        state.specialModeAuxState = 3;
        break;
    case 6:
        state.specialModeState = 0;
        state.specialModeFlags |= 0x400U;
        *reinterpret_cast<std::int8_t *>(&state.specialModeCounter) = 20;
        state.specialModeAuxState = 0;
        break;
    case 7:
        state.specialModeState = 0;
        state.specialModeFlags |= 0x400U;
        *reinterpret_cast<std::int8_t *>(&state.specialModeCounter) = 1;
        state.specialModeAuxState = 0;
        break;
    case 8:
        state.specialModeState = 0;
        state.specialModeFlags |= 0x400U;
        *reinterpret_cast<std::int8_t *>(&state.specialModeCounter) = 10;
        state.specialModeAuxState = 0;
        break;
    case 9:
        state.specialModeState = 0;
        state.specialModeFlags |= 0x400U;
        *reinterpret_cast<std::int8_t *>(&state.specialModeCounter) = 10;
        state.specialModeAuxState = 3;
        break;
    }
    if (state.specialModeFlags & 0x10U) {
        state.padFlags |= 0x100U;
        state.lookAtPosition.x = state.specialModeTargetPosition.x;
        state.lookAtPosition.y = state.specialModeTargetPosition.y;
        state.lookAtPosition.z = state.specialModeTargetPosition.z;
        state.lookTargetFlags |= 4U;
    }
    if ((state.specialModeFlags & 0x20U) && !(state.specialModeFlags & 8U)) {
        if (!(state.specialModeFlags & 0x80U) && IsHeldTransitionReady(state))
            state.specialModeFlags |= 0x80U;
        if (state.specialModeFlags & 0x80U) {
            if (state.motionState != 9) {
                state.activeMovementMode = 2;
                state.desiredYaw = CopyHeading(state.searchLocation, state.specialModeTargetPosition);
                state.movementSelectionFlags |= 0x40000000U;
            } else
                state.specialModeFlags &= ~0xa0U;
        }
    }
    if (state.specialModeFlags & 4U) {
        switch (static_cast<std::int8_t>(state.specialModeAuxState)) {
        case 0: {
            std::uint32_t motion = 0xf019U;
            if (state.specialModeFlags & 0x800U)
                motion = 0xf12aU;
            if (movement->playerMotionId == motion)
                ++state.specialModeAuxState;
            else {
                movement->flags |= 2U;
                movement->pendingMotionRequest = motion;
            }
            break;
        }
        case 1:
            if (movement->flags & 0x400U) {
                std::uint32_t motion = 0xf0f7U;
                if (state.specialModeFlags & 0x800U)
                    motion = 0xf129U;
                movement->pendingMotionRequest = motion;
                ++state.specialModeAuxState;
            }
            break;
        case 3: {
            std::uint32_t motion = 0xf018U;
            if (state.specialModeFlags & 0x800U)
                motion = 0xf128U;
            if (movement->playerMotionId == motion)
                ++state.specialModeAuxState;
            else
                movement->pendingMotionRequest = motion;
            break;
        }
        case 4:
            if (movement->flags & 0x400U) {
                movement->flags &= ~2U;
                state.specialModeFlags &= ~4U;
                if (state.specialModeFlags & 0x800U) {
                    movement->flags |= 2U;
                    state.specialModeFlags |= 0x400U;
                    *reinterpret_cast<std::int8_t *>(&state.specialModeCounter) = 10;
                    state.specialModeAuxState = 1;
                }
            }
            break;
        }
    }
    if (state.specialModeFlags & 0x400U) {
        switch (*reinterpret_cast<std::int8_t *>(&state.specialModeCounter)) {
        case 0:
        default:
            state.specialModeFlags &= ~0x400U;
            break;
        case 1:
            switch (static_cast<std::int8_t>(state.specialModeAuxState)) {
            case 0:
                if (movement->playerMotionId == 0xf11eU)
                    ++state.specialModeAuxState;
                else
                    movement->pendingMotionRequest = 0xf11eU;
                break;
            case 1:
                if (movement->flags & 0x400U) {
                    state.specialModeFlags &= ~0x400U;
                    *reinterpret_cast<std::int8_t *>(&state.specialModeCounter) = 0;
                }
                break;
            }
            break;
        case 10:
            switch (static_cast<std::int8_t>(state.specialModeAuxState)) {
            case 0:
                state.specialModeFlags |= 0x800U;
                if (movement->playerMotionId == 0xf12dU)
                    ++state.specialModeAuxState;
                else {
                    movement->flags |= 2U;
                    movement->pendingMotionRequest = 0xf12dU;
                }
                break;
            case 1:
                if (movement->flags & 0x400U) {
                    movement->pendingMotionRequest = 0xf12cU;
                    ++state.specialModeAuxState;
                }
                break;
            case 2:
                state.specialModeFlags &= ~0x400U;
                break;
            case 3:
                if (movement->playerMotionId == 0xf12bU)
                    ++state.specialModeAuxState;
                else {
                    movement->flags |= 2U;
                    movement->pendingMotionRequest = 0xf12bU;
                }
                break;
            case 4:
                if (movement->flags & 0x400U) {
                    movement->flags &= ~2U;
                    state.flags &= ~0x800U;
                    state.specialModeFlags &= ~0x400U;
                    ++state.specialModeAuxState;
                    *reinterpret_cast<std::int8_t *>(&state.specialModeCounter) = 0;
                }
                break;
            }
            break;
        case 100:
            switch (static_cast<std::int8_t>(state.specialModeAuxState)) {
            case 0:
                if (!IsHeldTransitionReady(state))
                    break;
                ++state.specialModeAuxState;
            case 1:
                if (state.motionState != 9) {
                    state.activeMovementMode = 2;
                    state.desiredYaw = CopyHeading(state.searchLocation, state.specialModeTargetPosition);
                    state.movementSelectionFlags |= 0x40000000U;
                } else {
                    ++state.specialModeAuxState;
                    *reinterpret_cast<std::int8_t *>(&state.specialModeCounter) = 0;
                }
                break;
            }
            break;
        case 20:
            switch (static_cast<std::int8_t>(state.specialModeAuxState)) {
            case 0:
                if (!IsHeldTransitionReady(state))
                    break;
                ++state.specialModeAuxState;
            case 1:
                state.activeMovementMode = 2;
                state.desiredYaw = CopyHeading(state.searchLocation, state.specialModeTargetPosition);
                state.movementSelectionFlags |= 0x40000000U;
                if (state.motionState != 9)
                    break;
                ++state.specialModeAuxState;
            case 2:
                state.specialModeFlags &= ~0x400U;
                *reinterpret_cast<std::int8_t *>(&state.specialModeCounter) = 0;
                break;
            }
            break;
        }
    }
    return result;
}


std::int32_t plymBuildAuthorPlacementContext(PlayerModelState &state) {
    PlayerPlacementNode &root = state.placementRoot;
    Vector3 candidate, target, local, hit, player, delta, previous;
    root.flags |= 0x80000000U;
    if (!state.specialModeTargetCharacter)
        return -1;
    Character *const author = ctrlGetScenarioChara(state.specialModeTargetCharacter);
    if (!author)
        return -1;
    GetCharacterWorldPosition(state.character, &player);
    player.y += 0.40000001f;
    GetCharacterWorldPosition(author, &target);
    root.targetCharacterPosition = target;
    target.y += 0.40000001f;
    const std::int16_t authorYaw = static_cast<std::int16_t>(GetCharacterWorldYaw(author));
    const std::int16_t bearing = plymGetAuthorPlacementYaw(state.specialModeTargetCharacter);
    am2Vector3Subtract(&delta, &root.targetPosition, &target);
    root.targetPosition = target;
    ccmGetFrameDisplacement(author, &delta);
    root.flags &= 0xfffedfffU;
    if (am2Vector3LengthSquared(&delta) < 0.000225f) {
        root.flags |= 0x12000U;
        root.targetYaw = CopyHeading(root.position, target);
        return 0;
    }
    am2Vector3Subtract(&local, &player, &target);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-bearing);
    Matrix4x4::Transform(local, local);
    PopCurrentMatrix(1);
    std::int16_t &orbitPhase = *reinterpret_cast<std::int16_t *>(&state.specialModeOrbitPhase);
    const float phase = orbitPhase;
    orbitPhase += 218;
    const float minimum = state.specialModeOrbitMinimum;
    const float halfRange = (state.specialModeOrbitMaximum - minimum) * 0.5f;
    const float longitudinal = halfRange * (dc::sdk::SinFixed(static_cast<std::int32_t>(phase)) + 1.0f) + minimum;
    const float cosine = dc::sdk::CosFixed(static_cast<std::int32_t>(phase * 1.5f)) + 1.0f;
    float lateral = 0.69999999f;
    lateral += 0.15000001f * cosine;
    if (local.x > 0.0f)
        lateral *= -1.0f;
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&target);
    Matrix4x4::RotateAroundY_Wrapper(bearing);
    Matrix4x4::preTranslate(-lateral, 0.0f, longitudinal);
    matrix_stack::GetTranslation(&candidate);
    PopCurrentMatrix(1);
    flddToggleFlag(8);
    if (FindFieldSegmentOffsetInWorld(&hit, &candidate, &target)) {
        lateral *= -1.0f;
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&target);
        Matrix4x4::RotateAroundY_Wrapper(bearing);
        Matrix4x4::preTranslate(-lateral, 0.0f, longitudinal);
        matrix_stack::GetTranslation(&candidate);
        PopCurrentMatrix(1);
        flddToggleFlag(8);
        if (FindFieldSegmentOffsetInWorld(&hit, &candidate, &target)) {
            matrix_stack::PushUnitMatrix();
            matrix_stack::TranslateVector(&target);
            Matrix4x4::RotateAroundY_Wrapper(bearing);
            Matrix4x4::preTranslate(0.0f, 0.0f, longitudinal);
            matrix_stack::GetTranslation(&candidate);
            PopCurrentMatrix(1);
        }
    }
    previous = root.previousPosition;
    std::int16_t previousYaw = root.targetYaw;
    if (root.flags & 0x200U) {
        root.flags &= ~0x200U;
        previous = target;
        previousYaw = authorYaw;
    }
    root.position.x = InterpolateTo(candidate.x, previous.x, 0.2f);
    root.position.y = InterpolateTo(candidate.y, previous.y, 0.2f);
    root.position.z = InterpolateTo(candidate.z, previous.z, 0.2f);
    root.targetYaw = CameraLerpAngle(bearing, previousYaw, 0.40000001f);
    return 0;
}


// @verified - d
void plymUpdateAuthorPlacementTarget(PlayerModelState &state) {
    PlayerPlacementNode *const node = plymUpdatePlacementChain(&state.placementRoot);
    state.selectedPlacementNode = state.activePlacementNode = node;
    if (!(node->flags & 0x800U) && (node->flags & 0x14000U) && IsHeldTransitionReady(state)) {
        Vector3 position;
        position.x = position.y = position.z = 0.0f;
        Character *const author = ctrlGetScenarioChara(state.specialModeTargetCharacter);
        if (author) {
            charaGetMotWorldPos(author, 6, &position);
            plymSetSpecialModeTargetPosition(&position, 0);
        }
    }
    state.approachStepDistance = plymGetApproachStepDistance(state.approachStepDistance, node->delta);
    plymResolvePlacementMovement(state);
    if (state.movementResult != 0.0f)
        state.movementResult = InterpolateTo(state.movementResult, state.previousMovementResult, 0.5f);
}


// @verified - d
bool plymIsInteractionPlacementBlocked(const PlayerModelState &state) {
    if (state.padFlags & 0x1000U)
        return false;
    if (!(state.flags & 0x40000U))
        return true;
    if (*reinterpret_cast<const std::int8_t *>(&state.motionInputState) != 10)
        return true;
    return false;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// @verified - p
bool plymCanLatchInteractionTarget(shen_name name) {
    const PartyCompanion *const party = FindPartyCompanion(authFindChara(name));
    return party && (party->flags & 2U) && !(party->flags & 0x200U);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) || ((SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)))
// @verified - d
void plymUpdateInteractionPlacementTarget(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    PlayerPlacementNode &root = state.placementRoot;
    float range = 20.0f;
    std::uint32_t latchFlags = 0;
    Character *const character = ctrlGetScenarioChara(state.interactionTarget);
    if (!character) {
        state.activeMovementMode = state.currentMovementMode;
        return;
    }
    Vector3 position;
    GetCharacterWorldPosition(character, &position);
    GetCharacterWorldYaw(character);
    const int yaw = plymGetAuthorPlacementYaw(state.interactionTarget);
    if (plymGetPartyAuthorStatus(state.interactionTarget) > 0) {
        const int frames = ++state.interactionTargetCounter;
        if (frames >= 900) {
            if (frames < 1350)
                range = 10.0f;
            else if (frames < 1800)
                range = 5.0f;
            else if (frames < 2250)
                range = 2.5f;
            else
                range = frames < 2700 ? 1.25f : 0.0099999998f;
        }
    } else
        state.interactionTargetCounter = 0;
    if (plymIsInteractionPlacementBlocked(state)) {
        state.activeMovementMode = 1;
        return;
    }
    PlayerPlacementNode *const node = plymUpdatePlacementChain(&state.placementRoot);
    state.selectedPlacementNode = node;
    const int facing = static_cast<std::int16_t>(node->characterBearingYaw - state.currentYaw);
    state.approachStepDistance = plymGetApproachStepDistance(state.approachStepDistance, root.delta);
    const float distance = GetPlanarDistance(&state.searchLocation, &position);
    Vector3 local;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-yaw);
    matrix_stack::TranslateInverseVector(&position);
    Matrix4x4::TransformPoint(state.searchLocation, local);
    PopCurrentMatrix(1);
    float ratio = 0.0f;
    const float halfRange = range * 0.5f;
    if (distance > halfRange) {
        ratio = (distance - halfRange) / halfRange;
        ratio = (ratio < 0.0f ? 0.0f : ratio) < 1.0f ? (ratio < 0.0f ? 0.0f : ratio) : 1.0f;
    }
    state.interactionRangeRatio = ratio;
    if (root.flags & 8U) {
        state.activeMovementMode = state.currentMovementMode;
        return;
    }
    std::uint32_t mode = state.currentMovementMode;
    const std::int16_t desiredYaw = state.desiredYaw;
    switch (mode) {
    case 3:
    case 4:
        if (state.flags & 0x8000000U) {
            if (static_cast<std::int8_t>(state.packedDirection))
                mode = 1;
            else if (reinterpret_cast<const std::int8_t *>(&state.packedDirection)[1] > 0)
                mode = 1;
        }
        break;
    case 1:
    case 2:
    case 5:
    case 6:
    default:
        if (state.flags & 0x8000000U)
            mode = 1;
        break;
    }
    if (state.currentMovementMode != 1 || (state.padFlags & 8U))
        state.idleNearTargetFrames = 0;
    else {
        const std::int16_t frames = ++state.idleNearTargetFrames;
        state.idleNearTargetFrames = frames > 10000 ? 10000 : frames;
    }
    if (!(state.padFlags & 0x80000U)) {
        bool latch = false;
        if (state.idleNearTargetFrames > 45 && (facing < 0 ? -facing : facing) < 0x4000)
            latch = true;
        if (!(ratio < 0.9f))
            latch = true;
        if (IsNaviActive())
            latch = false;
        if (latch) {
            state.padFlags |= 0x80000U | latchFlags;
            if ((facing < 0 ? -facing : facing) > 0x4000)
                state.padFlags |= 0x100000U;
        }
    } else {
        bool keep = true;
        if (ratio < 0.85f) {
            keep = mode != 6;
            if (mode != 1 && (state.turnInput < 0 ? -state.turnInput : state.turnInput) >= 0x4000)
                keep = false;
        }
        if (IsNaviActive())
            keep = false;
        if ((state.padFlags & 0x2000000U) && ratio < 0.85f)
            keep = false;
        if (!keep) {
            state.padFlags &= ~0x280000U;
            state.padFlags &= ~0x100000U;
        }
        if (local.z < 1.8f && !(state.padFlags & 8U))
            state.padFlags |= 0x4000000U;
    }
    if ((local.x < 0.0f ? -local.x : local.x) < 4.0f) {
        const int turn = static_cast<std::int16_t>(node->targetYaw - state.currentYaw);
        if ((turn < 0 ? -turn : turn) < 0x4000 && local.z < 2.0f)
            state.motionInputEcho = 0.0f;
        if (!(state.padFlags & 0x80000U) && local.z < 3.5f) {
            const float initial = state.approachStepDistance * 0.97500002f;
            const float z = (local.z < 0.0f ? 0.0f : local.z) < 3.5f ? (local.z < 0.0f ? 0.0f : local.z) : 3.5f;
            const float amount = (z - 1.8f) / 1.7f;
            state.movementResult = amount * state.remainingMotionTime + initial * (1.0f - amount);
            state.movementResult = InterpolateTo(state.movementResult, state.previousMovementResult, 0.5f);
        }
    }
    if (!(state.padFlags & 0x80000U)) {
        state.activeMovementMode = mode;
        state.desiredYaw = desiredYaw;
        return;
    }
    if (state.padFlags & 0x100000U) {
        if (!IsHeldTransitionReady(state)) {
            state.desiredYaw = state.currentYaw;
            state.activeMovementMode = 1;
            state.motionInputEcho = 0.0f;
            return;
        }
        state.padFlags &= ~0x100000U;
    }
    if (plymGetPartyAuthorStatus(state.interactionTarget) > 0) {
        if (node->flags & 2U) {
            if (IsHeldTransitionReady(state)) {
                if (node->distance < 0.44999999f && !(node->flags & 0x800U)) {
                    state.desiredYaw = node->targetYaw;
                    state.activeMovementMode = 1;
                } else {
                    state.desiredYaw = node->characterBearingYaw;
                    state.activeMovementMode = 3;
                }
            } else if (node->distance < 0.44999999f && !(node->flags & 0x800U)) {
                state.desiredYaw = node->targetYaw;
                state.activeMovementMode = 1;
            } else {
                state.desiredYaw = node->characterBearingYaw;
                state.activeMovementMode = 3;
                if (range == 0.0099999998f) {
                    state.desiredYaw = node->targetYaw;
                    state.activeMovementMode = 1;
                }
            }
        } else if (node->distance < 0.44999999f && !(node->flags & 0x800U)) {
            state.desiredYaw = node->targetYaw;
            state.activeMovementMode = 1;
        } else {
            state.desiredYaw = node->characterBearingYaw;
            state.activeMovementMode = 3;
        }
    } else
        plymResolvePlacementMovement(state);
    if (state.activeFrameCount < 5)
        state.activeMovementMode = 1;
    if (state.movementResult != 0.0f)
        state.movementResult = InterpolateTo(state.movementResult, state.previousMovementResult, 0.5f);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    Character *const target = authFindChara(state.interactionTarget);
    if (!target) {
        state.activeMovementMode = state.movementMode;
        return;
    }
    Vector3 position = GetCharacterWorldPosition(target);
    GetAllAngYForNode(&target->entity.node);
    const std::int16_t yaw = plymGetAuthorPlacementYaw(state.interactionTarget);
    float range = 20.0f;
    float sideClearance = 10.0f;
    if (plymGetPartyAuthorStatus(state.interactionTarget) <= 0)
        state.interactionTargetCounter = 0;
    else {
        const unsigned frames = ++state.interactionTargetCounter;
        if (frames >= 2700)
            range = 0.0099999998f;
        else if (frames >= 2250)
            range = 1.25f;
        else if (frames >= 1800)
            range = 2.5f;
        else if (frames >= 1350)
            range = 5.0f;
        else if (frames >= 900)
            range = 10.0f;
    }
    if (!(state.padFlags & 0x1000U) && (!(state.flags & 0x40000U) || (state.motionInputState != 10 && state.placementTargetState != 1))) {
        state.activeMovementMode = 1;
        return;
    }
    PlayerPlacementNode *const node = plymUpdatePlacementChain(&state.placementRoot);
    state.selectedPlacementNode = node;
    const std::int16_t facing = static_cast<std::int16_t>(node->characterBearingYaw - state.currentYaw);
    state.approachStepDistance = plymGetApproachStepDistance(state.approachStepDistance, state.placementRoot.delta);
    const float distance = GetPlanarDistance(&state.searchLocation, &position);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-yaw));
    Matrix4x4::preTranslate(-position.x, -position.y, -position.z);
    const Vector3 local = TransformWithActiveMatrix(state.searchLocation);
    Matrix4x4::Pop(1);
    position.y += 0.40000001f;
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw + 0x4000));
    const Vector3 right = TransformWithActiveMatrix(plymRayForward);
    Matrix4x4::RotateAroundY_Wrapper(0x8000);
    const Vector3 left = TransformWithActiveMatrix(plymRayForward);
    Matrix4x4::Pop(1);
    Vector3 hit{};
    if (RaycastForCollision(&hit, &right, &position))
        sideClearance = GetPlanarDistance(&position, &hit);
    if (RaycastForCollision(&hit, &left, &position))
        GetPlanarDistance(&position, &hit);
    float ratio = 0.0f;
    if (distance > range * 0.5f) {
        ratio = std::fmax(0.0f, (distance - range * 0.5f) / (range * 0.5f));
        if (ratio >= 1.0f)
            ratio = 1.0f;
    }
    state.interactionRangeRatio = ratio;
    const std::uint32_t movement = state.movementMode;
    if (state.placementRoot.flags & 8U) {
        state.activeMovementMode = movement;
        return;
    }
    std::uint32_t mode = movement;
    const std::int16_t desiredYaw = state.desiredYaw;
    if ((state.flags & plymControlLockOnMovement) &&
        ((movement - 3U) > 1U || static_cast<std::int8_t>(state.packedDirection) || static_cast<std::int8_t>(state.packedDirection >> 8U) > 0))
        mode = 1;
    if (movement != 1 || (state.padFlags & 8U))
        state.idleNearTargetFrames = 0;
    else {
        ++state.idleNearTargetFrames;
        if (static_cast<std::int16_t>(state.idleNearTargetFrames) >= 10000)
            state.idleNearTargetFrames = 10000;
    }
    if (state.padFlags & 0x80000U) {
        bool keep = true;
        if (ratio < 0.85f) {
            keep = mode != 6;
            if (mode != 1 && std::abs(static_cast<int>(state.turnInput)) >= 16384)
                keep = false;
        }
        if (IsNaviActive())
            keep = false;
        if ((state.padFlags & 0x2000000U) && ratio < 0.85f)
            keep = false;
        if (!plymCanLatchInteractionTarget(state.interactionTarget) || !keep)
            state.padFlags &= ~0x380000U;
        if (local.z < 1.8f && !(state.padFlags & 8U))
            state.padFlags |= 0x4000000U;
    } else {
        bool latch = static_cast<std::int16_t>(state.idleNearTargetFrames) > 45 && std::abs(static_cast<int>(facing)) < 16384;
        if (ratio >= 0.9f)
            latch = true;
        if (!plymCanLatchInteractionTarget(state.interactionTarget))
            latch = false;
        if (!IsNaviActive() && latch) {
            state.padFlags |= 0x80000U;
            if (std::abs(static_cast<int>(facing)) > 16384)
                state.padFlags |= 0x100000U;
        }
        if (local.z < 0.5f && distance < 7.5f)
            mode = 1;
    }
    const float lateralLimit = sideClearance + 2.0f;
    if (local.x > -lateralLimit && lateralLimit > local.x) {
        if (std::abs(static_cast<int>(static_cast<std::int16_t>(node->targetYaw - state.currentYaw))) < 16384 && local.z < 2.0f)
            state.motionInputEcho = 0.0f;
        if (!(state.padFlags & 0x80000U) && local.z < 3.5f) {
            float z = std::fmax(0.0f, local.z);
            if (z >= 3.5f)
                z = 3.5f;
            const float amount = (z - 1.8f) / 1.7f;
            state.movementResult = ((1.0f - amount) * (state.approachStepDistance * 0.97500002f)) + (amount * state.remainingMotionTime);
            state.movementResult = Lerp(state.movementResult, state.previousMovementResult, shenGetScaledDeltaTickTime(0.5f));
        }
    }
    if (!(state.padFlags & 0x80000U)) {
        state.activeMovementMode = mode;
        state.desiredYaw = desiredYaw;
        return;
    }
    if (state.padFlags & 0x100000U) {
        if (state.rootYawState) {
            state.desiredYaw = state.currentYaw;
            state.activeMovementMode = 1;
            state.motionInputEcho = 0.0f;
            return;
        }
        state.padFlags &= ~0x100000U;
    }
    if (plymGetPartyAuthorStatus(state.interactionTarget) <= 0)
        plymResolvePlacementMovement(state);
    else {
        if (node->distance >= 0.44999999f || (node->flags & 0x800U)) {
            state.desiredYaw = node->characterBearingYaw;
            state.activeMovementMode = 3;
            if ((node->flags & 2U) && state.rootYawState && range == 0.01f) {
                state.desiredYaw = node->targetYaw;
                state.activeMovementMode = 1;
            }
        } else {
            state.desiredYaw = node->targetYaw;
            state.activeMovementMode = 1;
        }
    }
    if (state.activeFrameCount < 5)
        state.activeMovementMode = 1;
    if (state.movementResult != 0.0f)
        state.movementResult = Lerp(state.movementResult, state.previousMovementResult, shenGetScaledDeltaTickTime(0.5f));
#endif
}


void plymBuildInteractionPlacementContext(PlayerModelState &state) {
#if (SHENMUE_IS_DC && SHENMUE_IS_S2) && (defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
    Vector3 offset, hit, player, target, delta, candidate, previous;
    Matrix4x4 transform;
    Vector3 sample;
    std::int16_t sampledYaw;
    Character *character;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = -1.25f;
    PlayerPlacementNode *const tracking = &state.placementRoot;
    PlayerPlacementNode *const root = &state.placementRoot;
    PlayerPlacementNode *const scratch = &state.placementScratch;
    std::int16_t characterYaw;
    std::int32_t found = 0;
    std::int16_t bearing;
    std::int32_t author = 0;
    character = ctrlGetScenarioChara(state.interactionTarget);
    if (!character)
        return;
    GetCharacterWorldPosition(state.character, &player);
    player.y += 0.4f;
    GetCharacterWorldPosition(character, &target);
    tracking->targetCharacterPosition = target;
    target.y += 0.4f;
    characterYaw = GetCharacterWorldYaw(character);
    bearing = plymGetAuthorPlacementYaw(state.interactionTarget);
    plymRecordPlacementHistory(state, target);
    am2Vector3Subtract(&delta, &tracking->targetPosition, &target);
    tracking->flags &= ~0x2000U;
    if (am2Vector3LengthSquared(&delta) < 0.0009f)
        tracking->flags |= 0x2000U;
    root->targetPosition = target;
    root->flags &= 0xfffedfffU;
    ccmGetFrameDisplacement(character, &delta);
    if (am2Vector3LengthSquared(&delta) < 0.0009f)
        root->flags |= 0x12000U;
    if (plymGetPartyAuthorStatus(state.interactionTarget) > 0)
        author = 1;
    am2Vector3Subtract(&delta, &player, &target);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-bearing);
    Matrix4x4::Transform(delta, delta);
    PopCurrentMatrix(1);
    if (tracking->flags & 0x400U) {
        if (delta.x > 0.25f)
            tracking->flags &= ~0x400U;
    } else if (delta.x < -0.25f)
        tracking->flags |= 0x400U;
    matrix_stack::PushUnitMatrix();
    matrix_stack::TranslateVector(&target);
    if (author)
        Matrix4x4::RotateAroundY_Wrapper(bearing);
    else
        Matrix4x4::RotateAroundY_Wrapper(characterYaw);
    matrix_stack::GetMatrix(&transform);
    PopCurrentMatrix(1);
    static const std::int16_t angles[5] = {0x2000, 0x1000, -0x1000, -0x2000, 0};
    candidate = target;
    const std::int16_t *angle = angles;
    std::int32_t side = 1;
    if (!(tracking->flags & 0x400U))
        side = -1;
    std::int16_t blocked;
    for (; *angle; angle++) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::LoadMatrix(&transform);
        Matrix4x4::RotateAroundY_Wrapper(0x8000 - *angle * side);
        Matrix4x4::TransformPoint(offset, candidate);
        PopCurrentMatrix(1);
        flddToggleFlag(8);
        blocked = FindFieldSegmentOffsetInWorld(&hit, &candidate, &target);
        flddToggleFlag(8);
        blocked |= IsWithinRadiusFromVector3PolyIntersectTest(&candidate, 0.3f);
        if (!blocked)
            break;
    }
    if (!*angle) {
        const Vector3 *const history = plymGetPlacementFromHistory(state, 2);
        if (history)
            candidate = *history;
    }
    previous = root->previousPosition;
    std::int16_t previousYaw = root->targetYaw;
    if (root->flags & 0x200U) {
        root->flags &= ~0x200U;
        previous = target;
        previousYaw = characterYaw;
    }
    candidate.x = InterpolateTo(candidate.x, previous.x, 0.2f);
    candidate.y = InterpolateTo(candidate.y, previous.y, 0.2f);
    candidate.z = InterpolateTo(candidate.z, previous.z, 0.2f);
    am2Vector3Subtract(&delta, &candidate, &target);
    if (std::fabs(delta.x) < 0.00000011920929f && std::fabs(delta.z) < 0.00000011920929f)
        am2Vector3Subtract(&delta, &player, &target);
    if (!(std::fabs(delta.x) < 0.00000011920929f && std::fabs(delta.z) < 0.00000011920929f)) {
        dc::sdk::SetVectorLength(&delta, &delta, am2Vector3Length(&offset));
        am2Vector3Add(&candidate, &target, &delta);
    }
    root->position = candidate;
    root->targetYaw = CameraLerpAngle(bearing, previousYaw, 0.4f);
    flddToggleFlag(0x428);
    blocked = GlobalRayIntersectionTest(player, candidate, 0.35f);
    const std::int32_t guide = PartyGuideCallbackInitialize();
    if (guide) {
        PartyGuideCallbackRouteHint(state.placementQueryHandle);
        state.placementQueryHandle = PartyGuideCallbackPick(&state.searchLocation);
    }
    if (!blocked || !guide)
        return;
    PartyGuideCallbackInitialize();
    PartyGuideCallbackRouteHint(GetPartyCompanionRouteIndex(state.interactionTarget));
    PartyGuideCallbackPick(&target);
    PartyGuideCallbackRouteHint(state.placementQueryHandle);
    PartyGuideCallbackFind(&player);
    PartyGuideCallbackDirection();
    while (PartyGuideCallbackEndState() < 1) {
        PartyGuideCallbackSample(&sample, &sampledYaw);
        sample.y += 0.4f;
        flddToggleFlag(0x428);
        blocked = GlobalRayIntersectionTest(player, sample, 0.35f);
        ccmUpdateEntityFlag(state.character);
        ccmUpdateEntityFlag(character);
        blocked |= CheckPointInCylinder(&player, &sample);
        if (!blocked) {
            found = 1;
            scratch->position = sample;
            break;
        }
        PartyGuideCallbackAdvance(4.0f);
    }
    if (!found) {
        PartyGuideCallbackInitialize();
        PartyGuideCallbackRouteHint(state.placementQueryHandle);
        PartyGuideCallbackPick(&player);
        PartyGuideCallbackRouteHint(GetPartyCompanionRouteIndex(state.interactionTarget));
        PartyGuideCallbackFind(&target);
        PartyGuideCallbackDirection();
        PartyGuideCallbackAdvance(10.0f);
        PartyGuideCallbackRouteHint(state.placementQueryHandle);
        PartyGuideCallbackFind(&player);
        PartyGuideCallbackDirection();
        for (;;) {
            PartyGuideCallbackSample(&sample, &sampledYaw);
            sample.y += 0.4f;
            flddToggleFlag(0x428);
            blocked = GlobalRayIntersectionTest(player, sample, 0.35f);
            ccmUpdateEntityFlag(state.character);
            ccmUpdateEntityFlag(character);
            blocked |= CheckPointInCylinder(&player, &sample);
            if (!blocked) {
                found = 1;
                scratch->position = sample;
                break;
            }
            if (PartyGuideCallbackEndState() > 0)
                break;
            PartyGuideCallbackAdvance(1.0f);
        }
    }
    if (!found)
        root->flags |= 8U;
    if (found)
        root->next = scratch;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    PlayerPlacementNode &root = state.placementRoot;
    Character *const targetCharacter = authFindChara(state.interactionTarget);
    if (!targetCharacter)
        return;
    Vector3 player = GetCharacterWorldPosition(state.characterNode);
    player.y += 0.40000001f;
    Vector3 target = GetCharacterWorldPosition(targetCharacter);
    root.targetCharacterPosition = target;
    target.y += 0.40000001f;
    const std::int16_t characterYaw = GetAllAngYForNode(&targetCharacter->entity.node);
    const std::int16_t bearing = plymGetAuthorPlacementYaw(state.interactionTarget);
    plymRecordPlacementHistory(state, target);
    const Vector3 difference{root.targetPosition.x - target.x, root.targetPosition.y - target.y, root.targetPosition.z - target.z};
    root.flags &= ~0x2000U;
    if ((difference.y * difference.y + difference.x * difference.x) + difference.z * difference.z < 0.0009f)
        root.flags |= 0x2000U;
    root.flags &= 0xfffedfffU;
    root.targetPosition = target;
    const CharacterControlModel *const control = FindCharacterControlModel(targetCharacter);
    const Vector3 delta = control ? control->frameDisplacement : Vector3{};
    if ((delta.y * delta.y + delta.x * delta.x) + delta.z * delta.z < 0.0009f)
        root.flags |= 0x12000U;
    const bool author = plymGetPartyAuthorStatus(state.interactionTarget) > 0;
    Vector3 local{player.x - target.x, player.y - target.y, player.z - target.z};
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-bearing));
    local = TransformWithActiveMatrix(local);
    Matrix4x4::Pop(1);
    if (root.flags & 0x400U) {
        if (local.x > 0.25f)
            root.flags &= ~0x400U;
    } else if (local.x < -0.25f)
        root.flags |= 0x400U;
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(target.x, target.y, target.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(author ? bearing : characterYaw));
    const Matrix4x4 transform = workTransformMatrix;
    Matrix4x4::Pop(1);
    Vector3 candidate = target, hit{};
    int angleIndex = 0;
    for (; playerInteractionPlacementAngles[angleIndex]; ++angleIndex) {
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(&transform);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>((root.flags & 0x400U ? -1 : 1) * playerInteractionPlacementAngles[angleIndex] + 0x8000));
        candidate = TransformWithActiveMatrix(playerInteractionPlacementOffset);
        Matrix4x4::Pop(1);
        flddToggleFlag(8);
        const std::int32_t wall = RaycastForCollision(&hit, &candidate, &target);
        flddToggleFlag(8);
        if (!(wall | IsWithinRadiusFromVector3PolyIntersectTest(candidate, 0.30000001f)))
            break;
    }
    if (!playerInteractionPlacementAngles[angleIndex] && state.placementHistoryCount) {
        const int count = static_cast<std::int8_t>(state.placementHistoryCount);
        int index = static_cast<std::int8_t>(state.placementHistoryWriteIndex) - (count < 2 ? count : 2);
        if (index < 0)
            index += 31;
        if (index > 31)
            index -= 32;
        candidate = state.placementHistory[index];
    }
    Vector3 previous = root.previousPosition;
    std::int16_t previousYaw = root.targetYaw;
    if (root.flags & 0x200U) {
        previous = target;
        previousYaw = characterYaw;
        root.flags &= ~0x200U;
    }
    candidate.x = Lerp(candidate.x, previous.x, shenGetScaledDeltaTickTime(0.2f));
    candidate.y = Lerp(candidate.y, previous.y, shenGetScaledDeltaTickTime(0.2f));
    candidate.z = Lerp(candidate.z, previous.z, shenGetScaledDeltaTickTime(0.2f));
    Vector3 radial{candidate.x - target.x, candidate.y - target.y, candidate.z - target.z};
    if (std::fabs(radial.x) < 0.00000011920929f && std::fabs(radial.z) < 0.00000011920929f && std::fabs(radial.z) < 0.00000011920929f)
        radial = {player.x - target.x, player.y - target.y, player.z - target.z};
    if (std::fabs(radial.x) >= 0.00000011920929f || std::fabs(radial.z) >= 0.00000011920929f || std::fabs(radial.z) >= 0.00000011920929f) {
        const float length = std::sqrt((radial.y * radial.y + radial.x * radial.x) + radial.z * radial.z);
        const float scale = 1.25f / length;
        radial = {radial.x * scale, radial.y * scale, radial.z * scale};
        candidate = {target.x + radial.x, target.y + radial.y, target.z + radial.z};
    }
    root.position = candidate;
    root.targetYaw = LerpFixedAngle(bearing, previousYaw, shenGetScaledDeltaTickTime(0.40000001f));
    flddToggleFlag(1064);
    const bool blocked = GlobalRayIntersectionTest(player, candidate, 0.375f);
    const bool guide = InitializePartyGuide() != 0;
    if (guide) {
        SetPartyGuideRouteHint(state.placementQueryHandle);
        state.placementQueryHandle = PickClosestTargetForPlacement(&state.searchLocation);
    }
    if (!blocked || !guide)
        return;
    InitializePartyGuide();
    const PartyCompanion *const companion = FindPartyCompanion(authFindChara(state.interactionTarget));
    const int route = companion ? companion->routeIndex : 0;
    SetPartyGuideRouteHint(route);
    PickClosestTargetForPlacement(&target);
    SetPartyGuideRouteHint(state.placementQueryHandle);
    FindClosestTargetForPlacement(&player);
    SetPartyGuideDirection();
    std::int16_t sampledYaw{};
    if (PartyGuideEndState() > 0)
        goto reverseGuide;
    for (;;) {
        SamplePartyGuide(&candidate, &sampledYaw);
        candidate.y += 0.40000001f;
        flddToggleFlag(1064);
        const bool wall = GlobalRayIntersectionTest(player, candidate, 0.375f);
        ccmUpdateEntityFlag(state.characterNode);
        ccmUpdateEntityFlag(targetCharacter);
        const bool character = CheckPointInCylinder_Wrapper(player, candidate) != nullptr;
        if (!(wall | character))
            goto publish;
        AdvancePartyGuide(4.0f);
        if (PartyGuideEndState() > 0)
            goto reverseGuide;
    }
reverseGuide:
    InitializePartyGuide();
    SetPartyGuideRouteHint(state.placementQueryHandle);
    PickClosestTargetForPlacement(&player);
    SetPartyGuideRouteHint(route);
    FindClosestTargetForPlacement(&target);
    SetPartyGuideDirection();
    AdvancePartyGuide(10.0f);
    SetPartyGuideRouteHint(state.placementQueryHandle);
    FindClosestTargetForPlacement(&player);
    SetPartyGuideDirection();
    SamplePartyGuide(&candidate, &sampledYaw);
    candidate.y += 0.40000001f;
    flddToggleFlag(1064);
    {
        const bool wall = GlobalRayIntersectionTest(player, candidate, 0.375f);
        ccmUpdateEntityFlag(state.characterNode);
        ccmUpdateEntityFlag(targetCharacter);
        const bool character = CheckPointInCylinder_Wrapper(player, candidate) != nullptr;
        if (!(wall | character))
            goto publish;
    }
    while (PartyGuideEndState() <= 0) {
        AdvancePartyGuide(1.0f);
        SamplePartyGuide(&candidate, &sampledYaw);
        candidate.y += 0.40000001f;
        flddToggleFlag(1064);
        const bool wall = GlobalRayIntersectionTest(player, candidate, 0.375f);
        ccmUpdateEntityFlag(state.characterNode);
        ccmUpdateEntityFlag(targetCharacter);
        const bool character = CheckPointInCylinder_Wrapper(player, candidate) != nullptr;
        if (!(wall | character))
            goto publish;
    }
    root.flags |= 8U;
    return;
publish:
    state.placementScratch.position = candidate;
    root.next = &state.placementScratch;
    return;
#endif
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// Counter branches of native plymUpdateSpecialMode; the high byte selects the authored sequence.
void plymUpdateSpecialMotionSequence(PlayerModelState &state) {
    PlayerMovementState &movement = *state.movementState;
    const std::uint32_t flags = state.specialModeFlags;
    if (!(flags & 0x400U))
        return;
    std::uint8_t *const low = reinterpret_cast<std::uint8_t *>(&state.specialModeCounter);
    std::uint8_t *const high = low + 1;
    switch (*high) {
    case 1:
        switch (*low) {
        case 0:
            if (movement.playerMotionId == 61726U)
                ++*low;
            else
                movement.pendingMotionRequest = 61726U;
            break;
        case 1:
            if (HandParams *const hand = GetCharacterHand(state.characterNode, 1))
                ParseHandPoseCommand(hand, reinterpret_cast<std::byte *>(chestHandPoseCommand), 15);
            if (HandParams *const hand = GetCharacterHand(state.characterNode, 0))
                ParseHandPoseCommand(hand, reinterpret_cast<std::byte *>(playerSpecialRightHandPoseCommand), 15);
            ++*low;
            [[fallthrough]];
        case 2:
            ++*low;
            [[fallthrough]];
        case 3:
            if (movement.flags & 0x400U) {
                state.specialModeFlags &= ~0x400U;
                *high = 0;
            }
            break;
        default:
            break;
        }
        state.specialModeFlags &= ~0x10U;
        if (state.padFlags & 0x100U) {
            state.lookTargetMode = lookTargetSpecialFast;
            state.padFlags &= ~0x100U;
        }
        return;
    case 9: {
        const float margin = GetCharacterCollisionCapsuleMargin(state.characterNode);
        SetCharacterCollisionCapsuleMargin(state.characterNode, margin < 0.55000001f ? std::fmin(0.55000001f, margin + 0.003f) : margin);
        std::uint32_t motion;
        switch (*low) {
        case 0:
            state.specialModeFlags |= 4U;
            motion = state.specialModeFlags & 0x800U ? 61738U : 61465U;
            if (movement.playerMotionId == motion) {
                ++*low;
                return;
            }
            movement.flags |= 2U;
            break;
        case 1:
            if (movement.flags & 0x400U) {
                movement.pendingMotionRequest = state.specialModeFlags & 0x800U ? 61737U : 61687U;
                ++*low;
            }
            return;
        case 2:
            state.specialModeFlags &= ~0x400U;
            return;
        case 3:
            motion = state.specialModeFlags & 0x800U ? 61736U : 61464U;
            if (movement.playerMotionId == motion) {
                ++*low;
                return;
            }
            break;
        case 4:
            if (movement.flags & 0x400U) {
                movement.flags &= ~2U;
                state.specialModeFlags &= 0xfffffbfbU;
                if (state.specialModeFlags & 0x800U) {
                    movement.flags |= 2U;
                    state.specialModeFlags |= 0x400U;
                    state.specialModeCounter = 2561U;
                }
            }
            return;
        default:
            return;
        }
        movement.pendingMotionRequest = motion;
        return;
    }
    case 10:
        switch (*low) {
        case 0:
            state.specialModeFlags = (flags & 0xfffef7ffU) | 0x800U;
            if (movement.playerMotionId != 61741U) {
                movement.flags |= 2U;
                movement.pendingMotionRequest = 61741U;
                return;
            }
            ++*low;
            return;
        case 1:
            if (movement.flags & 0x400U) {
                movement.pendingMotionRequest = 61740U;
                ++*low;
            }
            return;
        case 2:
            state.specialModeFlags = flags & ~0x400U;
            return;
        case 3:
            if (movement.playerMotionId != 61739U) {
                movement.flags |= 2U;
                movement.pendingMotionRequest = 61739U;
                return;
            }
            ++*low;
            return;
        case 4:
            if (!(movement.flags & 0x400U))
                return;
            movement.flags &= ~2U;
            state.flags &= ~0x800U;
            state.specialModeFlags &= ~0x400U;
            ++*low;
            *high = 0;
            return;
        default:
            return;
        }
    case 11:
        if (*low == 0) {
            if (state.rootYawState != 0)
                return;
            ++*low;
        } else if (*low != 1)
            return;
        if (state.rootYawState != 9U) {
            state.activeMovementMode = 2;
            state.movementSelectionFlags |= 0x40000000U;
            state.desiredYaw =
                PlymAngleXZ({state.specialModeTargetPosition.x - state.searchLocation.x, 0.0f, state.specialModeTargetPosition.z - state.searchLocation.z});
        } else {
            ++*low;
            *high = 0;
        }
        return;
    case 20:
        if (*low == 0) {
            state.padFlags |= 0x80U;
            state.overrideYaw =
                PlymAngleXZ({state.specialModeTargetPosition.x - state.searchLocation.x, 0.0f, state.specialModeTargetPosition.z - state.searchLocation.z});
            if (state.rootYawState != 0)
                return;
            ++*low;
        } else if (*low != 1) {
            if (*low == 2) {
                state.specialModeFlags &= ~0x400U;
                *high = 0;
            }
            return;
        }
        state.activeMovementMode = 2;
        state.desiredYaw =
            PlymAngleXZ({state.specialModeTargetPosition.x - state.searchLocation.x, 0.0f, state.specialModeTargetPosition.z - state.searchLocation.z});
        state.overrideYaw = state.desiredYaw;
        state.padFlags |= 0x80U;
        state.movementSelectionFlags |= 0x40000000U;
        if (state.rootYawState == 9U) {
            ++*low;
            state.specialModeFlags |= 8U;
            state.specialModeFlags &= ~0x400U;
            *high = 0;
        }
        return;
    default:
        state.specialModeFlags = flags & ~0x400U;
        return;
    }
}

bool plymIsSpecialAuthorMotion(shen_name name) {
    if (!npcIsNpcTaskActive())
        return false;
    for (const NpcRouteLiveEntry &entry : GetNpcRouteLiveEntries()) {
        if (entry.name.name != name.name)
            continue;
        const std::int32_t rig = entry.node ? GetCharacterBoneId(entry.node) : -1;
        if (entry.routeMotionState != 23 && entry.node && static_cast<std::uint32_t>(rig - 3) <= 2U)
            return (GetNpcAreaSpecialMotionFlags(entry) & 1U) != 0;
        return entry.currentMotionId == 32831U || entry.currentMotionId == 58242U || entry.currentMotionId == 57674U;
    }
    return false;
}

bool plymHasScenarioSelectedLocationRoot() {
    const ScenarioEventRuntime *const runtime = GetScenarioEventRuntime();
    if (!runtime)
        return false;
    const ScenarioFreeTalkRecord *const record = FindMutableScenarioFreeTalkRecord(runtime->activeCharacter);
    return !(record && (record->flags & ScenarioTalkInterrupted)) && ((runtime->flags >> 11U) & 1U);
}

void plymUpdateSpecialMode(PlayerModelState &state) {
    using enum CameraState;
    const std::uint32_t entryFlags = state.specialModeFlags;
    PlayerMovementState &movement = *state.movementState;
    if (state.rootYawState != 9U) {
        state.specialModeFlags &= ~8U;
        state.padFlags &= ~0x80U;
    }
    if (!(state.flags & 0x40000U))
        return;
    if (state.specialModeFlags & 0x1000U) {
        state.specialModeFlags &= ~0x1000U;
        if (state.specialModeAuthorName.name && plymIsSpecialAuthorMotion(state.specialModeAuthorName))
            state.specialModeState = 8;
        if (camCtrlGetState() == LockedOn)
            state.lookAroundState = 0;
        state.padFlags &= ~0x01000000U;
    }
    if (!(state.specialModeFlags & 1U)) {
        if (state.specialModeFlags & 0x2000U) {
            state.specialModeFlags &= ~0x2000U;
            if (state.specialModeFlags & 0x800U)
                state.specialModeState = 9;
        }
        if (!state.specialModeState && !(state.specialModeFlags & 0x400U) && (state.specialModeFlags & 0x40U)) {
            state.specialModeFlags &= ~0x40U;
            const std::int16_t yaw =
                InverseFixedAtan2(state.specialModeTargetPosition.x - state.searchLocation.x, state.specialModeTargetPosition.z - state.searchLocation.z);
            if (std::abs(static_cast<int>(static_cast<std::int16_t>(yaw - state.currentYaw))) <= 910)
                state.padFlags &= ~0x80U;
            else
                state.specialModeState = 6;
        }
    }
    if (state.specialModeFlags & 0x40U) {
        const std::int16_t yaw =
            InverseFixedAtan2(state.specialModeTargetPosition.x - state.searchLocation.x, state.specialModeTargetPosition.z - state.searchLocation.z);
        if (std::abs(static_cast<int>(static_cast<std::int16_t>(yaw - state.currentYaw))) > 910) {
            state.padFlags |= 0x80U;
            state.overrideYaw = yaw;
        }
    }
    if ((state.specialModeFlags & 0x100U) && !state.specialModeState && !(state.specialModeFlags & 0x400U) && !plymHasScenarioSelectedLocationRoot()) {
        PlayerModelState *const model = g_playerRuntime.model;
        if ((g_playerRuntime.flags & 0x40000U) && model) {
            g_playerRuntime.flags &= ~0x80000U;
            model->specialModeFlags &= ~0x100U;
            model->padFlags |= 0x200000U;
            model->specialModeFrame = 0;
            model->specialModeState = 0;
            g_playerRuntime.flags &= ~0x40000U;
            model->flags &= ~0x40000U;
            model->specialModeFlags = (model->specialModeFlags & 0xffffdefeU) | 0x2100U;
            model->movementState->flags &= ~2U;
        }
        return;
    }
    if (state.interactionTarget.name == PLAYER_COMPANION_SHENHUA.name && !(state.placementRoot.flags & 0x10000U))
        state.padFlags |= 0x80000U;
    state.activeMovementMode = 1;
    if (state.specialModeFlags & 2U) {
        plymBuildAuthorPlacementContext(state);
        plymUpdateAuthorPlacementTarget(state);
    }
    switch (state.specialModeState) {
    case 1:
        state.specialModeFlags |= 0x10U;
        break;
    case 2: {
        state.specialModeState = 0;
        state.specialModeFlags = (state.specialModeFlags & 0xfffdbf7fU) | 0x10U;
        const std::int16_t yaw =
            InverseFixedAtan2(state.specialModeTargetPosition.x - state.searchLocation.x, state.specialModeTargetPosition.z - state.searchLocation.z);
        const std::int16_t delta = static_cast<std::int16_t>(yaw - state.currentYaw);
        if (state.specialModeFlags & 2U)
            break;
        if (entryFlags & 0x4000U) {
            state.specialModeCounter = 2816;
            state.specialModeFlags |= 0x400U;
            break;
        }
        if (entryFlags & 0x20000U) {
            const int absolute = std::abs(static_cast<int>(delta));
            if (absolute > 0x4000 && !state.rootYawState) {
                state.activeMovementMode = 2;
                state.desiredYaw = yaw;
                Character *const character = authFindChara(GetCameraSpecialTargetName());
                if (character) {
                    const Vector3 position = GetCharacterWorldPosition(character);
                    const std::int16_t targetYaw =
                        InverseFixedAtan2(state.specialModeTargetPosition.x - position.x, state.specialModeTargetPosition.z - position.z);
                    const std::int16_t playerYaw = InverseFixedAtan2(position.x - state.searchLocation.x, position.z - state.searchLocation.z);
                    const std::int16_t side = static_cast<std::int16_t>(playerYaw - targetYaw) >= 0 ? 0x4000 : -0x4000;
                    state.specialModeFlags |= 0x40U;
                    state.desiredYaw = static_cast<std::int16_t>(targetYaw + side);
                    break;
                }
            } else if (absolute > 0x4000) {
                state.specialModeTurnSide = 2;
                state.specialModeFlags |= 0x20000U;
            }
            state.specialModeFlags |= 0x40U;
            break;
        }
        if (!(state.specialModeFlags & 0x800U)) {
            const int limit = static_cast<std::int8_t>(state.specialModeStep) <= 1 ? 13653 : 20024;
            if (std::abs(static_cast<int>(delta)) > limit && !state.rootYawState) {
                state.activeMovementMode = 2;
                state.desiredYaw = yaw;
            }
        }
        break;
    }
    case 3:
        state.specialModeFlags |= 0x404U;
        state.specialModeCounter = 2304;
        break;
    case 4:
        state.specialModeFlags |= 0x400U;
        state.specialModeCounter = 2307;
        break;
    case 5:
        state.placementRoot.flags |= 0x200U;
        break;
    case 6:
        state.specialModeFlags |= 0x400U;
        state.specialModeCounter = 5120;
        break;
    case 7:
        state.specialModeFlags |= 0x400U;
        state.specialModeCounter = 256;
        break;
    case 8:
        state.specialModeFlags |= 0x400U;
        state.specialModeCounter = 2560;
        break;
    case 9:
        state.specialModeFlags |= 0x400U;
        state.specialModeCounter = 2563;
        break;
    default:
        break;
    }
    state.specialModeState = 0;
    if (state.specialModeFlags & 0x10U) {
        state.lookTargetFlags |= 4U;
        state.padFlags |= 0x100U;
        state.lookAtPosition = state.specialModeTargetPosition;
        if (static_cast<std::int8_t>(state.specialModeStep) > 1 && state.rootYawState != 9U)
            state.lookTargetFlags |= 8U;
    }
    plymUpdateSpecialMotionSequence(state);
}


void plymSelectActiveMovementMode(PlayerModelState &state) {
    if (state.padFlags & plymPadForceMotionState2) {
        state.activeMovementMode = 2;
        state.motionInputEcho = 0.0f;
        return;
    }
    state.activeMovementMode = 1;
    if (state.flags & 0x80U) {
        state.desiredYaw = state.currentYaw;
        state.motionInputEcho = 0.0f;
        return;
    }
    plymUpdateRegionInteraction(state);
    if (state.padFlags & 0x800U)
        return;
    plymUpdateSpecialMode(state);
    if (state.flags & 0x40000U)
        return;
    if (state.motionInputState) {
        plymUpdateMotionInputState(state);
        return;
    }
    if (state.placementTargetState) {
        plymBuildInteractionPlacementContext(state);
        plymUpdateInteractionPlacementTarget(state);
        return;
    }
    if (state.flags & plymControlLockOnMovement) {
        plymUpdateLockOnMovement(state);
        return;
    }
    if (state.padFlags & 0x10U) {
        if (state.movementMode != 6U)
            state.padFlags &= ~0x10U;
    } else if (state.previousInputMovementMode == 6U && state.embeddedMovementState.previousRequestedMode == 6U) {
        state.padFlags |= 0x10U;
    }
    if ((state.flags & 2U) == 0)
        return;
    if (state.turnaroundState == 1U) {
        switch (state.turnaroundPhase) {
        case 0:
            state.motionInputEcho = 0.0f;
            state.desiredYaw = state.currentYaw;
            if (static_cast<double>(state.interpolationValue) > 0.1) {
                state.activeMovementMode = 3;
            } else {
                ++state.turnaroundPhase;
                state.activeMovementMode = 1;
            }
            state.turnaroundFrames = 0;
            return;
        case 1:
            state.motionInputEcho = 0.0f;
            state.desiredYaw = static_cast<std::int16_t>(state.currentYaw + 0x8000);
            state.activeMovementMode = 6;
            if (state.rootYawState != 4U && state.rootYawState != 9U)
                return;
            state.turnaroundPhase = 3;
            break;
        case 2:
            state.turnaroundPhase = 3;
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            state.turnaroundState = 0;
            return;
        }
        if (state.turnaroundPhase == 3) {
            if ((state.padFlags & 0x10U) == 0) {
                state.turnaroundState = 0;
                return;
            }
            if (state.rootYawState == 4U || state.rootYawState == 9U)
                return;
            if (state.normalizedMotionInput == 0.0f) {
                state.turnaroundState = 0;
                return;
            }
            state.turnaroundPhase = 4;
        }
        if ((state.padFlags & 0x10U) == 0)
            state.turnaroundState = 0;
        ++state.turnaroundFrames;
        state.activeMovementMode = 3;
        state.desiredYaw = state.currentYaw;
        if (state.turnaroundFrames >= 20)
            state.turnaroundPhase = 0;
        return;
    }
    if (state.turnaroundState != 0U) {
        state.turnaroundState = 0;
        state.turnaroundPhase = 0;
    }
    const std::uint32_t movementMode = state.movementMode;
    if (movementMode != 6U)
        state.rotationOnlyWaitFrames = 0;
    switch (movementMode) {
    case 1:
        state.activeMovementMode = static_cast<double>(state.interpolationValue) > 0.1 ? 3U : 1U;
        break;
    case 3:
    case 4:
        state.activeMovementMode = movementMode;
        plymConstrainMovementContextYaw(state);
        break;
    case 5:
        state.activeMovementMode = 1;
        break;
    case 6:
        state.activeMovementMode = 0;
        if (state.padFlags & 0x10U)
            break;
        if (state.rootYawState == 0U) {
            state.activeMovementMode = movementMode;
            break;
        }
        if (state.rotationOnlyWaitFrames++ >= 4) {
            state.turnaroundState = 1;
            state.turnaroundPhase = 0;
        }
        break;
    default:
        state.activeMovementMode = movementMode;
        break;
    }
}

// Native plymResolveMovementMode: publishes the movement request consumed by
// DoesRyoMovementCalculation.
void plymResolveMovementMode(PlayerModelState &state) {
    state.previousMovementResult = state.movementResult;
    state.movementResult = 0.0f;
    plymSelectActiveMovementMode(state);
    if (state.committedYaw != state.desiredYaw) {
        plymProbeMovementCollision(state);
        plymProbeDesiredYawCollision(state);
        UpdatePlayerDeltaSpeedRatio(state);
    }
    state.yawDelta = static_cast<std::int16_t>(state.desiredYaw - state.currentYaw);
    PlayerMovementState &movement = state.embeddedMovementState;
    if (movement.flags & 8U) {
        if (state.nearestCollisionCharacter) {
            const float ratio = plymProbeSpeedRatio(state.nearestCharaProbeDistance, state.speed);
            if (state.movementSpeedRatio > ratio)
                state.movementSpeedRatio = ratio;
            if (state.nearestCharaProbeDistance < 1.25f)
                state.movementBlendLimit = 0.0f;
        }
        if (state.nearestCharaProbeDistance < 1.25f)
            state.movementSelectionFlags |= selectionImmediateCharacterCollision;
    }
    plymResolveCollisionTurn(state);
    std::uint32_t mode = state.activeMovementMode;
    state.movementSelectionFlags &= ~selectionImmediateCharacterCollision;
    switch (mode) {
    case 0:
    case 5:
        break;
    case 1:
        if (static_cast<double>(state.interpolationValue) > 0.1)
            mode = 0;
        break;
    case 3:
    case 4:
        movement.targetYaw = state.desiredYaw;
        if ((state.flags & plymControlLockOnMovement) == 0 && state.movementSpeedRatio < 0.01f) {
            mode = 1;
            const std::int32_t delta = std::abs(static_cast<std::int32_t>(state.desiredYaw) - static_cast<std::int32_t>(state.currentYaw));
            if (movement.dispatchMode || delta <= 910) {
                if (static_cast<double>(state.interpolationValue) > 0.1)
                    mode = 3;
            } else {
                mode = 2;
            }
        }
        break;
    case 6:
        if (state.movementSubtype)
            movement.leadFootDirection = state.movementSubtype;
        movement.targetYaw = state.desiredYaw;
        break;
    case 13:
        break;
    default:
        movement.targetYaw = state.desiredYaw;
        break;
    }
    movement.requestedMode = mode;
    movement.movementResult = state.movementResult;
}
#endif

} // namespace player_detail

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t plymSetSceneContext(
    std::int16_t holdWindowStartMinute, std::int16_t holdWindowEndMinute, std::int16_t sceneActId, shen_name sceneAreaId, std::int16_t sceneEntryId) {
    PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return -1;
    state->sceneEntryId = sceneEntryId;
    state->holdWindowStartMinute = holdWindowStartMinute;
    state->holdWindowEndMinute = holdWindowEndMinute;
    state->sceneActId = sceneActId;
    state->sceneAreaId = sceneAreaId;
    return 0;
}

std::int32_t plymRequestSpecialAuthorTarget(shen_name name) {
    if ((g_playerRuntime.flags & 0x40000U) == 0)
        return -1;
    PlayerModelState &state = *g_playerRuntime.model;
    state.specialModeAuthorName = name;
    if ((state.specialModeFlags & 2U) == 0)
        state.specialModeState = 5;
    state.specialModeFlags |= 2U;
    return 0;
}

std::int32_t plymSetSpecialModeTargetPosition(const Vector3 *position, std::int32_t mode) {
    PlayerModelState *const state = g_playerRuntime.model;
    if ((g_playerRuntime.flags & 0x40000U) == 0 || !state)
        return -1;
    state->specialModeState = 2;
    state->specialModeTargetPosition = *position;
    if (mode == 1)
        state->specialModeFlags |= 0x20080U;
    else if (mode == 2)
        state->specialModeFlags |= 0x4000U;
    else if (mode == 3)
        state->specialModeState = (state->specialModeFlags & 0x10U) == 0;
    ++state->specialModeStep;
    return 0;
}

std::int32_t plymBeginSpecialTargetMotionSequence(shen_name name) {
    if ((g_playerRuntime.flags & 0x40000U) == 0 || !name.name)
        return -1;
    g_playerRuntime.model->specialModeState = 3;
    g_playerRuntime.model->specialModeTargetId = name.name;
    return 0;
}

std::int32_t plymContinueSpecialTargetMotionSequence() {
    if ((g_playerRuntime.flags & 0x40000U) == 0 || !g_playerRuntime.model->specialModeTargetId)
        return -1;
    g_playerRuntime.model->specialModeState = 4;
    return 0;
}

std::int32_t plymRequestSpecialPoseSequence() {
    if ((g_playerRuntime.flags & 0x40000U) == 0)
        return -1;
    g_playerRuntime.model->specialModeState = 7;
    g_playerRuntime.model->specialModeCounter = (g_playerRuntime.model->specialModeCounter & 0xffU) | 0x100U;
    return 0;
}

std::int32_t plymBeginSpecialMode() {
    PlayerModelState *state = g_playerRuntime.model;
    if (!state || ((g_playerRuntime.flags & 0x40000U) != 0 && (state->specialModeFlags & 1U) != 0))
        return -1;
    g_playerRuntime.flags |= 0xc0000U;
    state->padFlags &= ~0x80U;
    state->specialModeFlags = 0x1001U;
    state->specialModeState = 0;
    state->specialModeTurnSide = 0;
    state->specialModeFrame = 0;
    state->specialModeCounter &= 0xff00U;
    state->specialModeStep = 0;
    if (mtwkSetExtraFlag(state->motionWork, 59U, 1) != 0)
        mtwkResetPositionTransition(state->motionWork);
    state->specialModeAuthorName = GetActiveScenarioCharacter();
    if ((state->motionInputState == 10U || state->placementTargetState == 1U) && state->specialModeAuthorName.name == SHEN_NAME(SIN_))
        plymRequestSpecialAuthorTarget(state->specialModeAuthorName);
    return 0;
}

std::int32_t plymEndSpecialMode(std::int32_t mode) {
    PlayerModelState *state = g_playerRuntime.model;
    if ((g_playerRuntime.flags & 0x40000U) == 0 || !state)
        return -1;
    g_playerRuntime.flags &= ~0x80000U;
    state->specialModeFlags &= ~0x100U;
    state->padFlags |= 0x200000U;
    state->specialModeFrame = 0;
    state->specialModeState = 0;
    if (mode == 1 || mode == 2) {
        state->specialModeState = 6;
        return 0;
    }
    if (mode == 3) {
        g_playerRuntime.flags |= 0x80000U;
        state->specialModeFlags |= 0x100U;
        return 0;
    }
    if (mode != 0) {
        g_playerRuntime.flags &= ~0x40000U;
        state->flags &= ~0x40000U;
    }
    state->specialModeFlags = (state->specialModeFlags & 0xffffdefeU) | 0x2100U;
    state->movementState->flags &= ~2U;
    return 0;
}

std::uint32_t plymSetSpecialMode(std::int32_t enabled) {
    const std::uint32_t previous = (g_playerRuntime.flags >> 10U) & 1U;
    if (enabled != 0) {
        g_playerRuntime.flags |= 0x400U;
    } else {
        g_playerRuntime.flags &= ~0x400U;
    }
    return previous;
}
#endif

} // namespace s2

} // namespace shenmue
