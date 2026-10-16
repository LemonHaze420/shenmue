#include "s2/player_internal.hpp"

namespace shenmue::s2 {

PlayerRuntimeState g_playerRuntime{};

static std::array<std::uint64_t, 7> playerRetainedResetWords{};

std::uint32_t GetCharacterControlModelFlags(Character *character) {
    const CharacterControlModel *const state = FindCharacterControlModel(character);
    return state ? state->flags : 0;
}

void SetCharacterControlModelFlags(Character *character, std::uint32_t flags) {
    CharacterControlModel *const state = FindCharacterControlModel(character);
    if (!state)
        return;

    // FindCCM_OR_flags selects CTRL's root, not the global PLYM attachment.
    if ((flags & 0x00100000U) != 0) {
        flags = (flags & 0xbfefff20U) | 0x400000dfU;
        const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (control && character == control->ryo)
            flags |= 0x04400200U;
    }
    state->flags = flags;
}

} // namespace shenmue::s2

namespace shenmue::s2::player_detail {

LegacyInputSlot playerInputSnapshot{};

Vector3 publishedPlayerSearchLocation{};

std::int16_t movingFrames{}, stationaryFrames{}, turningFrames{};

void UpdatePlayerDisplayAlpha(PlayerModelState &state) {
    const bool useSavedAlpha = (state.padFlags & 0x8000U) != 0;
    state.padFlags &= ~0x8000U;
    if ((state.flags & 0x04000000U) == 0 || !state.characterNode)
        return;

    float alpha = state.characterNode->alpha;
    const CameraState cameraState = g_cameraControl ? g_cameraControl->state : CameraState::Init;
    if ((cameraState == CameraState::LookAround || cameraState == CameraState::LockedOn || cameraState == CameraState::Init) && useSavedAlpha) {
        alpha = state.field7c4;
    }
    modelSetCharaAlpha(state.characterNode, std::clamp(alpha, 0.0f, 1.0f));
}

void InitializePlayerModelState(PlayerModelState &state) {
    std::memset(static_cast<void *>(&state), 0, sizeof(state));
    state.movementState = &state.embeddedMovementState;
    InitializePlayerMovementState(state.embeddedMovementState);
    state.field37c = 20.0f;
    state.secondaryInteractionDistanceLimit = 20.0f;
    state.specialModeOrbitMinimum = 0.25f;
    state.specialModeOrbitMaximum = 0.75f;
    state.field436 = 63;
    state.placementQueryHandle = -1;
}

std::int32_t largeTurnInputFrames = 0;

} // namespace shenmue::s2::player_detail

namespace shenmue::s2 {

Character *authFindChara(shen_name name) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control ? FindScenarioCharacter(control->scenarioTask, name) : nullptr;
}

} // namespace shenmue::s2

namespace shenmue::s2::player_detail {

void PlayerModelUpdate(Character *character, void *payload) {
    auto &state = *static_cast<PlayerModelState *>(payload);



    if (!(g_playerRuntime.flags & 0x8000U))
        return;
    if (++state.frameCount == 0)
        ++state.frameCount;
    SynchronizePlayerModelState(character, state);
    UpdatePlayerDisplayAlpha(state);
    plymUpdateRyoBag(state);
    plymUpdateHeldState(state);
    if ((g_playerRuntime.flags & 0x4002U) && state.activeFrameCount < 0xffffU) {
        ++state.activeFrameCount;
    }
    if (g_playerRuntime.flags & 0x4002U) {
        plymSampleInput(state);
        plymUpdateCameraInput(state);
        UpdatePlayerYawFromInput(state);
        plymProbeMovementCollision(state);
        plymProbeDesiredYawCollision(state);
        UpdatePlayerDeltaSpeedRatio(state);
        UpdateTargetWorldMovementGate(state);
        PreparePlayerMovementContext(state);
        plymResolveMovementMode(state);
        if (state.useExternalMovementState) {
            ExecutePlayerMovement(*state.movementState);
        } else {
            UpdatePlayerInterpolation(state);
            ExecutePlayerMovement(state.embeddedMovementState);
            plymUpdateLookTarget(state);
        }
        UpdateReachedPlayerYawWriteback(state);
    }
}

} // namespace shenmue::s2::player_detail

namespace shenmue::s2 {

// @verified - p
std::uint64_t ReturnZero() noexcept {
    return 0;
}

// @verified - p
std::int64_t ResetPlayerRetainedState() noexcept {
    for (std::int32_t index = 6; index >= 0; --index)
        playerRetainedResetWords[index] = 0;
    return 0;
}

// @verified - p
std::int64_t InitializePlayerRuntime() {
    g_playerRuntime.flags = 0;
    g_playerRuntime.field08 = 0;
    g_playerRuntime.currentDay = 0;
    g_playerRuntime.model = nullptr;
    ResetPlayerRetainedState();
    ResetPendingPlayerPersistentState();
    ResetPartyCompanionTransition();
    return 0;
}

void plymSetDefaultFlags() {
    g_playerRuntime.flags &= ~0x00204003U;
    g_playerRuntime.model = nullptr;
}

// @verified - p
void plymInitializeRyoBagState(PlayerModelState &state) {
    state.helperFlags = 0;
    state.activeHelperMode = 0;
    state.pendingHelperMode = -1;
    state.helperCharacter = nullptr;
}

// @verified - p
std::int32_t InitializePlayerSceneContexts(PlayerModelState &state) {
    g_playerRuntime.currentDay = g_currentDateTime.day;
    state.heldStateFlags = 0;
    state.heldStateMode = 0;
    state.heldStateStep = 0;
    state.pendingHeldStateMode = 0;
    state.heldStateTimer = 0;
    state.heldStateTriggerMinute = 0;
    state.heldStateCallback = nullptr;
    PlayerModelState &current = *g_playerRuntime.model;
    current.holdWindowStartMinute = 1350;
    current.holdWindowEndMinute = 1410;
    current.sceneActId = static_cast<std::int16_t>(g_actNumber);
    current.sceneAreaId = PLAYER_RETURN_AREA;
    current.sceneEntryId = 25;
    current.scheduledTransitionMinute = 1260;
    current.requestedActId = static_cast<std::int16_t>(g_actNumber);
    current.requestedAreaId = PLAYER_RETURN_AREA;
    current.requestedEntryId = 25;
    state.timeAdvanceTargetHour = 21;
    state.timeAdvanceTargetMinute = 0;
    state.timeAdvanceMinutesPerTick = 3;
    state.reserved3f2[0] = std::byte{};
    state.reserved3f2[15] = std::byte{};
    std::memset(state.reserved3f2 + 22, 0, 8);
    return 1260;
}

bool AttachPlayerModel(Character *character) {
    if (!character || (g_playerRuntime.flags & 1U))
        return false;
    auto *const state = static_cast<PlayerModelState *>(AllocateModelComponent(character, SHEN_NAME(PLYM), sizeof(PlayerModelState), PlayerModelUpdate));
    if (!state)
        return false;
    g_playerRuntime.model = state;
    g_playerRuntime.character = character;
    g_playerRuntime.flags = (g_playerRuntime.flags & 0xffbbffffU) | 0x1482843dU;
    InitializePlayerModelState(*state);
    state->characterNode = character;
    state->embeddedMovementState.character = character;
    state->flags |= 0x120U;
    state->yaw = static_cast<std::uint16_t>(GetAllAngYForNode(&character->entity.node));
    if (g_playerRuntime.flags & 0x10U)
        state->flags |= 0x10U;
    if (auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))) {
        control->ryo = character;
    }
    if (character->type != 1 && character->type != 10)
        character->type = 4;
    plymInitializeRyoBagState(*state);
    InitializePlayerSceneContexts(*state);
    PlayerPersistentState persistent{};
    if (S2Save_ConsumePendingPlayerPersistentState(persistent)) {
        g_playerRuntime.field08 = persistent.runtimeState;
        state->heldStateFlags = persistent.playerStateFlags;
        state->heldStateTriggerMinute = static_cast<std::int16_t>(persistent.callbackValue);
        g_playerRuntime.savedHeldStateFlags = persistent.playerStateFlags;
        g_playerRuntime.scheduledCallbackMinute = static_cast<std::int16_t>(persistent.callbackValue);
    }
    if (g_playerRuntime.field08 & 0x10) {
        if (g_playerRuntime.field08 & 0x20)
            plymScheduleCycleFlagCallback(g_playerRuntime.scheduledCallbackMinute);
        else
            plymScheduleOneShotFlagCallback(g_playerRuntime.scheduledCallbackMinute);
    }
    return true;
}

// @verified - p
std::uint64_t SetPlayerControlEnabled(std::int32_t mode) {
    PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return 0xffffffffULL;

    const std::uint32_t wasEnabled = (g_playerRuntime.flags >> 1U) & 1U;
    const std::uint32_t retainedMode = (g_playerRuntime.flags >> 21U) & 1U;
    g_playerRuntime.flags &= ~0x00200000U;
    if (mode < 0 || (mode != 0 && wasEnabled != 0)) {
        return wasEnabled;
    }
    if (mode == 0 && wasEnabled == 0) {
        return 0;
    }

    if (state->motionWork) {
        // GetRotationFromNode/SetRotationForNode copy the node's local Euler
        // triplet; only yaw survives this boundary.
        state->characterNode->entity.node.rotation[0] = 0;
        state->characterNode->entity.node.rotation[2] = 0;
        EV_mtwkSetEnabled(FindMotionWork(state->characterNode), true);
        if (mtwkSetExtraFlag(state->motionWork, 59U, 1)) {
            ResetMotionWorkPositionTransition(state->motionWork);
        }
    }


    if (mode != 0) {
        g_playerRuntime.flags = (g_playerRuntime.flags & 0xffff2ffdU) | 0x00008002U;
        state->padFlags &= 0xfbf7ffffU;
        state->activeFrameCount = 0;
        if (retainedMode == 0) {
            state->padFlags |= 0x00080000U;
            state->motionInputState = 0;
        }
        ResetPlayerMovementMotion(state->movementState, static_cast<std::int32_t>(retainedMode));
        state->interpolationValue = 0.0f;
    } else {
        g_playerRuntime.flags &= ~2U;
        std::uint32_t movementMode = state->movementMode;
        state->latchedMovementMode = movementMode;
        if (movementMode == 3U && state->normalizedMotionInput > 0.5f) {
            movementMode = 4U;
            state->latchedMovementMode = movementMode;
        }
        const std::uint32_t activeMode = state->activeMovementMode;
        std::uint32_t requestedMode = 1;
        if (activeMode == 3U)
            requestedMode = 3U;
        if (activeMode == 4U || state->interpolationValue > 0.5f) {
            requestedMode = 4U;
        }
        state->latchedMovementMode = activeMode;
        state->requestedMovementMode = requestedMode;
        UpdatePlayerPartyCompanions(*state);
    }
    g_playerRuntime.flags &= ~0x01000000U;
    state->padFlags &= 0xef7efe7fU;
    state->lookAroundState = 0;
    state->lookInputGraceFrames = 0;
    state->turnaroundState = 0;
    state->lookTargetMode = 0;
    state->lookTargetFlags = 0;
    state->automaticLookIdleTimer = 0.0f;
    state->pendingHeldStateMode = 0;
    return wasEnabled;
}

// @verified - p
std::uint64_t SetPlayerGameFlags(PlayerGameFlagMode mode, void *parameter) {
    PlayerRuntimeState &runtime = g_playerRuntime;
    PlayerModelState *const state = runtime.model;
    if (!state)
        return 0;
    std::uint32_t mask = 0;
    switch (mode) {
    case PlayerGameFlagMode::RestoreControl: {
        SetPlayerControlEnabled(1);
        PlayerModelState *const current = g_playerRuntime.model;
        if ((g_playerRuntime.flags & 0x40000U) != 0 && current) {
            g_playerRuntime.flags &= ~0x80000U;
            current->specialModeFlags &= ~0x100U;
            current->padFlags |= 0x200000U;
            current->specialModeFrame = 0;
            current->specialModeState = 0;
            g_playerRuntime.flags &= ~0x40000U;
            current->flags &= ~0x40000U;
            current->specialModeFlags = (current->specialModeFlags & 0xffffdefeU) | 0x2100U;
            if ((current->movementState->flags & 2U) != 0)
                current->movementState->flags &= ~2U;
        }
        break;
    }
    case PlayerGameFlagMode::Bit2:
        mask = 4U;
        break;
    case PlayerGameFlagMode::Bit5:
        mask = 0x20U;
        break;
    case PlayerGameFlagMode::Bit26:
        mask = 0x4000000U;
        break;
    case PlayerGameFlagMode::SearchBlocked:
        mask = 0x1000000U;
        break;
    case PlayerGameFlagMode::Bit29:
        mask = 0x20000000U;
        break;
    case PlayerGameFlagMode::Bit25:
        mask = 0x2000000U;
        break;
    case PlayerGameFlagMode::Bit30:
        mask = 0x40000000U;
        break;
    case PlayerGameFlagMode::Bit22:
        mask = 0x400000U;
        break;
    case PlayerGameFlagMode::SetInteractionDistanceLimit:
        std::memcpy(&state->secondaryInteractionDistanceLimit, parameter, sizeof(float));
        break;
    case PlayerGameFlagMode::GetInteractionDistanceLimit:
        std::memcpy(parameter, &state->secondaryInteractionDistanceLimit, sizeof(float));
        break;
    case PlayerGameFlagMode::Bit12:
        mask = 0x1000U;
        break;
    case PlayerGameFlagMode::DisableAreaControl:
        runtime.flags &= ~0x4000U;
        state->padFlags |= 0x20U;
        state->motionInputState = 0;
        return 0;
    case PlayerGameFlagMode::EnableAreaControl:
        runtime.flags |= 0x4000U;
        state->padFlags &= ~0x20U;
        state->motionInputState = 1;
        state->padFlags |= 0x40U;
        return 0;
    case PlayerGameFlagMode::QueryAreaControl:
        return (runtime.flags & 0x4000U) != 0 ? (state->padFlags >> 5U) & 1U : 0xffffffffULL;
    case PlayerGameFlagMode::SetPlacementTarget: {
        const PlayerPlacementTarget &target = *static_cast<const PlayerPlacementTarget *>(parameter);
        PlayerModelState *const current = g_playerRuntime.model;
        const std::uint16_t yaw = target.yaw;
        if (!current)
            return 0;
        current->placementRoot.position = *target.position;
        current->placementRoot.targetYaw = yaw;
        return 1;
    }
    case PlayerGameFlagMode::RetainMovementMode:
        mask = 0x200000U;
        break;
    default:
        break;
    }
    const std::uintptr_t operation = reinterpret_cast<std::uintptr_t>(parameter);
    if (operation == UINTPTR_MAX)
        return (runtime.flags & mask) != 0;
    if (operation == 1U)
        runtime.flags |= mask;
    else if (operation == 0U)
        runtime.flags &= ~mask;
    return 0;
}

bool SetPlayerRootPosition(const Vector3 &position) {
    Character *const character = g_playerRuntime.character;
    if (!character)
        return false;
    shenApplyTranslationToAll(&character->entity.node, position);
    return true;
}

bool IsPlayerPositionTransitionActive() {
    const PlayerModelState *const state = g_playerRuntime.model;
    return state && state->motionWork && (state->motionWork->runtime.seqFlags & MotionSequencePositionTransition) != 0;
}

std::uint32_t plymGetCurrentFlags() {
    return g_playerRuntime.flags;
}

std::int32_t plymIntersectionTest() {
    const Vector3 position = GetPlayerTargetWorldPosition();
    if (!g_playerRuntime.model || !g_playerRuntime.model->movementState || !g_playerRuntime.model->movementState->character) {
        return 0;
    }
    return plymDoIntersectionTest(position, position.y + 0.5f);
}

bool plymSampleLookInput(Vector2s &facing, Vector2s &look) {
    const PlayerModelState *const state = g_playerRuntime.model;
    facing.x = 0;
    facing.y = state ? static_cast<std::int16_t>((g_playerRuntime.flags & 2U) ? state->yaw : static_cast<std::uint16_t>(state->currentYaw)) : 0;
    look = {};
    if (!state || state->lookAroundState == 0)
        return false;
    facing = {static_cast<std::int16_t>(state->lookBasePitch), static_cast<std::int16_t>(state->lookBaseYaw)};
    look = {static_cast<std::int16_t>(state->lookPitchDelta), static_cast<std::int16_t>(state->lookYawDelta)};
    return true;
}

std::int32_t plymGetLookInputState() {
    const PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return 0;
    if (state->lookAroundState != 0) {
        return static_cast<std::int32_t>((state->lookInputFlags & 1U) != 0) + 1;
    }
    return (state->padFlags & 0x200U) != 0 ? -1 : 0;
}

std::int32_t playerGetRotY() {
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return shenGetRotYFromNode(static_cast<const Character *>(control->ryo));
}

Vector3 *plymUpdatePlayerSearchLocation(Vector3 *location) {
    if (location) {
        *location = GetCharacterWorldPosition(g_playerRuntime.character);
        publishedPlayerSearchLocation = *location;
        return nullptr;
    }
    PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return nullptr;
    publishedPlayerSearchLocation = state->searchLocation;
    return &state->searchLocation;
}

const Vector3 &plymGetPublishedPlayerSearchLocation() {
    return publishedPlayerSearchLocation;
}

float plymGetInterpolationValue() {
    const PlayerModelState *const state = g_playerRuntime.model;
    return state ? state->interpolationValue : 0.0f;
}

std::uint32_t plymGetAuxiliaryState() {
    const PlayerModelState *const state = g_playerRuntime.model;
    return state ? state->searchRegionKey : 0;
}

// @verified - p
std::uint64_t plymGetAreaExitState() {
    const PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return 0;
    if (state->heldStateMode == static_cast<std::uint8_t>(HeldStateMode::AdvanceTime))
        return 0xffffffffULL;
    return (state->heldStateFlags >> 8U) & 1U;
}

std::uint16_t plymGetAngY() {
    const PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return 0;
    return (g_playerRuntime.flags & 2U) ? state->yaw : static_cast<std::uint16_t>(state->currentYaw);
}

std::uint8_t plymGetLookZoomScale() {
    const PlayerModelState *const state = g_playerRuntime.model;
    return state ? state->lookZoomInput : 0;
}

std::uint32_t plymConsumeFlags8() {
    const std::uint32_t result = g_playerRuntime.field08 & 8U;
    g_playerRuntime.field08 &= ~8;
    return result;
}

std::int32_t plymGetSpecialFlag() {
    const PlayerModelState *const state = g_playerRuntime.model;
    if (!state || (g_playerRuntime.flags & 0x4000U) == 0)
        return -1;
    return static_cast<std::int32_t>((state->padFlags >> 5U) & 1U);
}

bool plymIsSearchStartRequested() {
    return ((g_playerRuntime.flags >> 1U) & 1U) != 0;
}

bool plymIsSearchable() {
    const PlayerModelState *const state = g_playerRuntime.model;
    return state && (g_playerRuntime.flags & 2U) != 0 && (state->padFlags & 0x800U) == 0;
}

// @verified - p
bool plymIsSearchBlocked() {
    const PlayerModelState *const state = g_playerRuntime.model;
    if (!state)
        return false;
    if ((state->flags & 0x100U) != 0 && state->heldStateMode != 0)
        return true;
    if ((state->padFlags & 0x800U) != 0)
        return true;
    if (IsDoorInteractionActive())
        return true;
    return (g_playerRuntime.flags & 0x01000000U) != 0 || (state->padFlags & 0x800000U) != 0 || (g_playerRuntime.flags & 2U) == 0;
}

bool plymCanEvaluateSearchTargets() {
    const PlayerModelState *state = g_playerRuntime.model;
    if (!state || (g_playerRuntime.flags & 2U) == 0 || ((state->flags & 0x100U) != 0 && state->heldStateMode != 0) || (state->padFlags & 0x800U) != 0)
        return false;
    if (IsDoorInteractionActive())
        return true;
    if (!state->motionWork)
        return false;
    const std::uint16_t motion = GetMotionAnimationId(state->motionWork, MotionRegion::LowerBody);
    return motion != 0xf024U && motion != 0xf029U && motion != 0xf034U && !GetShouldRenderIcons(RenderIconSlot::ItemEffectMessage) &&
        (g_playerRuntime.flags & 0x40000U) == 0;
}

// @verified - p
bool plymIsInterpolationActive() {
    const PlayerModelState *const state = g_playerRuntime.model;
    return state && state->interpolationValue > 0.5f;
}

std::uint32_t plymGetSpecialMode() {
    return (g_playerRuntime.flags >> 10U) & 1U;
}

std::uint32_t plymSetFlags(std::uint32_t flags) {
    const std::uint32_t previous = g_playerRuntime.flags;
    g_playerRuntime.flags = flags;
    return previous;
}

std::uint32_t plymAddFlags(std::uint32_t flags) {
    const std::uint32_t previous = g_playerRuntime.flags;
    g_playerRuntime.flags |= flags;
    if (g_playerRuntime.model)
        g_playerRuntime.model->flags |= flags;
    return previous;
}

std::uint32_t plymRemoveFlags(std::uint32_t flags) {
    const std::uint32_t previous = g_playerRuntime.flags;
    g_playerRuntime.flags &= ~flags;
    if (g_playerRuntime.model)
        g_playerRuntime.model->flags &= ~flags;
    return previous;
}

std::int32_t plymExportPersistentState(PlayerPersistentState *output) {
    if (!output)
        return -1;
    *output = {};
    const PlayerModelState *const state = g_playerRuntime.model;
    if (state) {
        output->present = 1;
        output->runtimeState = g_playerRuntime.field08;
        output->playerStateFlags = state->heldStateFlags;
        output->callbackValue = static_cast<std::uint16_t>(state->heldStateTriggerMinute);
    }
    return 0;
}

bool plymCheckAuthorTargetRequested() {
    const PlayerModelState *const model = g_playerRuntime.model;
    return model && (g_playerRuntime.flags & 0x40000U) && (model->specialModeFlags & 2U) && model->placementTargetState == 1 &&
        plymGetPartyAuthorStatus(model->specialModeAuthorName) != 2;
}

// @verified - p

std::int64_t plymFinishRegionInteraction() {
    PlayerModelState *const state = g_playerRuntime.model;
    if (!state || !(state->padFlags & 0x800U) || state->regionInteractionState != 4)
        return 0xffffffff;
    state->regionInteractionState = 5;
    return 0;
}

bool ryoNotBusy() {
    if (plymIsSearchBlocked() || static_cast<std::int32_t>(srchCheckFlags(3)) >= 0 || GetParentMenu() == 0 || IsItemSelectable() ||
        !IsCtrlSpeakIconInactive() || IsSaveAllowed() != 0)
        return false;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return IsScreenFilmNotShowing(control->screenFilmTask) && !IsAnyItemSelectActive() && !HasScenarioCharacterControl() && ReturnZero() == 0;
}

// @verified - p
bool shenCheckIsBusy() {
    if (plymIsSearchBlocked() || static_cast<std::int32_t>(srchCheckFlags(3)) >= 0 || static_cast<std::uint32_t>(srchCheckFlags(6)) != 0 ||
        GetParentMenu() == 0 || IsItemSelectable() || !IsCtrlSpeakIconInactive() || IsSaveAllowed() != 0)
        return false;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return IsScreenFilmNotShowing(control->screenFilmTask) && !IsAnyItemSelectActive() && !IsScenarioBusy() && ReturnZero() == 0;
}

// @verified - p
bool shenGetIsPlayerInMenu() {
    if (plymIsSearchBlocked() || GetParentMenu() == 0 || IsItemSelectable() || !IsCtrlSpeakIconInactive() || IsSaveAllowed() != 0)
        return false;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return IsScreenFilmNotShowing(control->screenFilmTask) && !IsAnyItemSelectActive() && !IsScenarioBusy() && ReturnZero() == 0;
}

} // namespace shenmue::s2
