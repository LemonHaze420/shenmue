#include "s2/player_internal.hpp"

namespace shenmue::s2::player_detail {

// The bag helper leaves the scenario table and is killed.
void ctrlAssignCharaHandlerFor(PlayerModelState &state) {
    Character *const helper = state.helperCharacter;
    if (!helper)
        return;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    UnassignScenarioCharacter(control->scenarioTask, helper);
    ctrlSetKillAndIncrementCounter(state.helperCharacter, 1);
    state.helperCharacter = nullptr;
}

void ReleaseRyoBagModel(RyoBagModel *bag) {
    ReleaseModelFormat7(bag->model);
    HLib::FreeTaskData(bag->source);
    delete bag;
}


void modelFree(Character *character, void *payload) {
    RyoBagForm &form = *static_cast<RyoBagForm *>(payload);
    character->model = nullptr;
    modelSetCharaAlpha(character, 0.0f);
    if (form.bag) {
        ReleaseRyoBagModel(form.bag);
        form.bag = nullptr;
    }
    if (form.shadow) {
        ReleaseRyoBagModel(form.shadow);
        form.shadow = nullptr;
    }
}

RyoBagModel *LoadRyoBagModel(const char *filename, shen_name token, std::uint32_t mode) {
    std::uint32_t size{};
    void *const source = LoadLegacySceneFile("misc", filename, token, &size);
    if (!source)
        return nullptr;
    RyoBagModel *const bag = new (std::nothrow) RyoBagModel{};
    if (!bag)
        return nullptr;
    bag->source = source;
    std::array<char, 0x105> path{};
    std::snprintf(path.data(), path.size(), "%s/%s", "misc", filename);
    if (!ReadModelFormat7({static_cast<const std::byte *>(source), size}, path.data(), bag->model, mode)) {
        ReleaseRyoBagModel(bag);
        return nullptr;
    }
    return bag;
}

// Mode 1 is the hand-carried RCKO bag, mode 2 the shouldered RCKF bag; an existing helper of the other kind is released.
std::int64_t plymCreateRyoBagCharacter(PlayerModelState &state, std::int32_t mode) {
    const char *const filename = mode == 2 ? "RYBG2F2G.MT7" : "RYBG2H3G.MT7";
    const shen_name name = mode == 2 ? SHEN_NAME(RCKF) : SHEN_NAME(RCKO);
    if (state.helperCharacter) {
        const RyoBagForm *const form = static_cast<const RyoBagForm *>(FindModelComponent(state.helperCharacter, SHEN_NAME(FORM)));
        if (form && form->name.name == name.name)
            return 1;
        ctrlAssignCharaHandlerFor(state);
    }
    HLib::Task *handlerTask{};
    Character *const character = AddHighModelCharacter(name, handlerTask);
    if (!character)
        return 0;
    RyoBagForm *const form = static_cast<RyoBagForm *>(AllocateModelComponent(character, SHEN_NAME(FORM), sizeof(RyoBagForm), nullptr));
    if (!form)
        return 0;
    (reinterpret_cast<ModelComponentHeader *>(form) - 1)->cleanup = modelFree;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    CreateScenarioCharacterId(control->scenarioTask, name, character);
    AttachObjectXFormsComponent(character);
    RyoBagModel *const bag = LoadRyoBagModel(filename, SHEN_NAME(FORM), 2);
    if (!bag)
        return 0;
    character->model = &bag->model;
    modelSetCharaAlpha(character, 1.0f);
    EV_SetCharDisplay(character, 3, 0);
    imgmAssignUpdateCallback(character, charaPrepareNodesForDraw);
    RyoBagModel *const shadow = LoadRyoBagModel("SYD_KAB.MT7", SHEN_NAME(SVKB), 0);
    if (shadow)
        SetCharacterAuxiliaryData(character, &shadow->model);
    if (!bag->model.nodes.empty())
        InitializeObject(&bag->model.nodes.front());
    form->bag = bag;
    form->name = name;
    form->shadow = shadow;
    state.helperCharacter = character;
    return 1;
}

// Internal flag 123 carries the bag; 551 selects the shouldered RCKF form.
void plymSelectRyoBagMode(PlayerModelState &state) {
    const bool attached = (state.helperFlags & 2U) != 0;
    if (!(state.flags & 0x20U)) {
        if (attached)
            state.pendingHelperMode = 0;
        return;
    }
    const bool carried = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 123) != 0;
    switch (state.activeHelperMode) {
    case 0:
        if (attached) {
            state.pendingHelperMode = 0;
        } else if (carried) {
            state.pendingHelperMode = 1;
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 551))
                state.pendingHelperMode = 2;
        }
        break;
    case 1:
        if (!attached) {
            if (!carried)
                break;
            state.pendingHelperMode = 1;
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 551))
                state.pendingHelperMode = 2;
        } else if (!carried) {
            state.pendingHelperMode = 0;
        } else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 551)) {
            state.pendingHelperMode = 2;
        }
        break;
    case 2:
        if (!attached) {
            if (carried)
                state.pendingHelperMode = static_cast<std::int8_t>((EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 551) != 0) + 1);
        } else if (!carried) {
            state.pendingHelperMode = 0;
        } else if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 551)) {
            state.pendingHelperMode = 1;
        }
        break;
    default:
        break;
    }
}

void plymUpdateRyoBagVisibility(PlayerModelState &state) {
    if (!state.helperCharacter)
        return;
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    const Vector3 bag = GetCharacterWorldPosition(state.helperCharacter);
    const float dx = bag.x - camera.x;
    const float dy = bag.y - camera.y;
    const float dz = bag.z - camera.z;
    const float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    const float alpha = distance >= 0.34999999f ? std::clamp((distance - 0.34999999f) / 0.2f, 0.0f, 1.0f) : 0.0f;
    state.helperCharacter->alpha = alpha;
    SetFixoAlphaPropagation(state.helperCharacter, (state.flags & 2U) && alpha != 1.0f ? 1 : 0);
}

void plymReleaseRyoBag(PlayerModelState &state, MotionWork *motionWork) {
    if (state.helperCharacter)
        ResetCharaFixo(state.helperCharacter);
    mtwkInitFlags(motionWork, 0);
    ctrlAssignCharaHandlerFor(state);
    SetupHandPoseScript(state.characterNode);
    state.helperFlags &= ~2U;
}

void plymApplyPendingRyoBagMode(PlayerModelState &state) {
    const std::int8_t pending = state.pendingHelperMode;
    MotionWork *const motionWork = state.motionWork;
    if (pending == -1 || !motionWork || !(state.padFlags & 2U))
        return;
    state.pendingHelperMode = -1;
    if (pending != 0 && state.activeHelperMode != 0 && state.activeHelperMode != pending)
        plymReleaseRyoBag(state, motionWork);
    if (pending == 0) {
        plymReleaseRyoBag(state, motionWork);
        state.activeHelperMode = 0;
    } else if (pending == 1) {
        mtwkRequestMotion(motionWork, 0xf007, MotionRegion::LeftArm);
        plymCreateRyoBagCharacter(state, 1);
        SetFixoDataForMotionWork(state.helperCharacter, state.characterNode, 15, {-0.092499994f, 0.19f, -0.067000002f}, {58892, 44638, 51628});
        EV_SetCharDisplay(state.helperCharacter, 3, 1);
        charaSetDisplayEnabledFlag(state.helperCharacter, 1);
        charaSetVisibilityFlagTo(state.helperCharacter, 1);
        SetCharacterType(state.helperCharacter, CharacterTypeStencilShadow);
        hmdlSetFlagsFor(state.characterNode, 1, 0, 1);
        state.helperFlags |= 2U;
        state.activeHelperMode = 1;
    } else if (pending == 2) {
        plymCreateRyoBagCharacter(state, 2);
        EV_SetCharDisplay(state.helperCharacter, 3, 1);
        SetFixoDataForMotionWork(state.helperCharacter, state.characterNode, 15, {-0.13177f, 0.18569f, -0.074680001f}, {60823, 44637, 51623});
        state.helperFlags |= 2U;
        state.activeHelperMode = 2;
    }
}

void plymUpdateRyoBag(PlayerModelState &state) {
    plymSelectRyoBagMode(state);
    if (state.pendingHelperMode != -1)
        plymApplyPendingRyoBagMode(state);
    plymUpdateRyoBagVisibility(state);
}


shen_name FindShenmueNodeFromScenarioTask(Character *character) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return scenGetCharaName(control->scenarioTask, character);
}



shen_name ptycFindDoorCompanion(shen_name name) {
    const std::uint32_t region = plymGetAuxiliaryState();
    if (region == 0)
        return {};
    const shen_name *const candidates = name.name ? &name : plymDoorCompanionNames.data();
    const std::size_t count = name.name ? 1U : plymDoorCompanionNames.size();
    for (std::size_t index = 0; index < count; ++index) {
        Character *const character = authFindChara(candidates[index]);
        const PartyCompanion *const party = character ? FindPartyCompanion(character) : nullptr;
        if (party && (party->flags & 2U) && party->motionState == 5 && party->doorId == region)
            return party->name;
    }
    return {};
}

// @verified - p
void UpdatePlayerPartyCompanions(PlayerModelState &state) {
    bool requested = (g_playerRuntime.flags & 0x400000U) == 0 && (g_playerRuntime.flags & 2U) != 0;
    if (IsDoorInteractionActive() && (g_playerRuntime.flags & 0x400000U) == 0)
        requested = true;
    if (GetPartyCompanionTransitionPending() && (g_playerRuntime.flags & 0x400000U) == 0)
        requested = true;
    if ((g_playerRuntime.flags & 0x401000U) == 0x1000U)
        requested = true;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        requested = false;
    std::int32_t changed = 0;
    if (requested && EV_GetShenmueFlag(0x190)) {
        if (!FindPTYCForChara(PLAYER_COMPANION_REN)) {
            ActivatePartyCompanion(PLAYER_COMPANION_REN);
            changed = 1;
            if (FindPTYCForChara(PLAYER_COMPANION_REN))
                PTYCUtils(PLAYER_COMPANION_REN, 0, 1);
        }
    } else if (requested && EV_GetShenmueFlag(0x191)) {
        Vector3 position;
        if (npcGetPosFor(PLAYER_COMPANION_REN, &position)) {
            const float x = position.x - state.searchLocation.x;
            const float y = position.y - state.searchLocation.y;
            const float z = position.z - state.searchLocation.z;
            if (std::sqrt(x * x + y * y + z * z) >= 15.0f && g_playerRuntime.model && !g_playerRuntime.model->heldStateMode) {
                PlayerModelState &player = *g_playerRuntime.model;
                player.heldStateMode = static_cast<std::uint8_t>(HeldStateMode::RetrySceneChange);
                player.heldStateStep = 0;
                player.retryActId = 3;
                player.retryEntryId = 97;
                player.retryAreaId = PLAYER_COMPANION_RETRY_AREA;
            }
        }
    } else {
        if ((EV_GetShenmueFlag(0x191) || EV_GetShenmueFlag(0x190)) && !RegisterNpcFlags(PLAYER_COMPANION_REN, NpcRouteAbstractActive, 2))
            EV_CheckWarmStartNpcRoute(PLAYER_COMPANION_REN);
        if (FindPTYCForChara(PLAYER_COMPANION_REN))
            ptycCharaUnlock(PLAYER_COMPANION_REN);
        else if ((state.padFlags & 0x40000U) && (EV_GetShenmueFlag(0x190) || EV_GetShenmueFlag(0x191))) {
            changed = -1;
            if (!authFindChara(PLAYER_COMPANION_REN))
                EV_npcFree(PLAYER_COMPANION_REN);
        }
    }
    if (requested && EV_GetShenmueFlag(0x194)) {
        if (!FindPTYCForChara(PLAYER_COMPANION_JOY)) {
            changed = 1;
            ActivatePartyCompanion(PLAYER_COMPANION_JOY);
            if (FindPTYCForChara(PLAYER_COMPANION_JOY))
                PTYCUtils(PLAYER_COMPANION_JOY, 0, 1);
        }
        if (FindPTYCForChara(PLAYER_COMPANION_JOY))
            PTYCUtils(PLAYER_COMPANION_JOY, 0, 1);
    } else if (FindPTYCForChara(PLAYER_COMPANION_JOY))
        ptycCharaUnlock(PLAYER_COMPANION_JOY);
    else if ((state.padFlags & 0x40000U) && EV_GetShenmueFlag(0x194)) {
        changed = -1;
        if (!authFindChara(PLAYER_COMPANION_JOY))
            EV_npcFree(PLAYER_COMPANION_JOY);
    }
    bool targetPlaced = false;
    if (requested && EV_GetShenmueFlag(0x192)) {
        if (!FindPTYCForChara(PLAYER_COMPANION_SHENHUA)) {
            changed = 1;
            ActivatePartyCompanion(PLAYER_COMPANION_SHENHUA);
        }
        if (FindPTYCForChara(PLAYER_COMPANION_SHENHUA)) {
            PTYCUtils(PLAYER_COMPANION_SHENHUA, 0, 2);
            if (state.placementTargetState != 1) {
                state.placementTargetState = 1;
                state.idleNearTargetFrames = 30;
                state.interactionTarget = PLAYER_COMPANION_SHENHUA;
                state.field37c = 20.0f;
            }
            targetPlaced = true;
        }
    } else if (requested && EV_GetShenmueFlag(0x193)) {
        if (!FindPTYCForChara(PLAYER_COMPANION_SHENHUA)) {
            changed = 1;
            ActivatePartyCompanion(PLAYER_COMPANION_SHENHUA);
            if (FindPTYCForChara(PLAYER_COMPANION_SHENHUA))
                PTYCUtils(PLAYER_COMPANION_SHENHUA, 0, 1);
        }
        if (FindPTYCForChara(PLAYER_COMPANION_SHENHUA))
            PTYCUtils(PLAYER_COMPANION_SHENHUA, 0, 1);
    } else {
        if ((EV_GetShenmueFlag(0x192) || EV_GetShenmueFlag(0x193)) && !RegisterNpcFlags(PLAYER_COMPANION_SHENHUA, NpcRouteAbstractActive, 2))
            EV_CheckWarmStartNpcRoute(PLAYER_COMPANION_SHENHUA);
        if (FindPTYCForChara(PLAYER_COMPANION_SHENHUA))
            ptycCharaUnlock(PLAYER_COMPANION_SHENHUA);
        else if ((state.padFlags & 0x40000U) && (EV_GetShenmueFlag(0x192) || EV_GetShenmueFlag(0x193))) {
            changed = -1;
            if (!authFindChara(PLAYER_COMPANION_SHENHUA))
                EV_npcFree(PLAYER_COMPANION_SHENHUA);
        }
    }
    if (!targetPlaced) {
        if (state.motionInputState == 10 || state.motionInputState == 7) {
            if (g_playerRuntime.model) {
                g_playerRuntime.flags &= ~0x4000U;
                g_playerRuntime.model->padFlags |= 0x20U;
                g_playerRuntime.model->motionInputState = 0;
            }
            g_playerRuntime.flags |= 0x10000000U;
            if (g_playerRuntime.model)
                g_playerRuntime.model->flags |= 0x10000000U;
        }
        if (state.placementTargetState)
            state.placementTargetState = 0;
    }
    if (changed == 1)
        state.padFlags |= 0x40000U;
    else if (changed == -1)
        state.padFlags &= ~0x40000U;
}

} // namespace shenmue::s2::player_detail

namespace shenmue::s2 {

bool plymHasRyoBag(std::int32_t mode) {
    const PlayerModelState *const model = g_playerRuntime.model;
    return model && (model->helperFlags & 2U) && (mode != 1 || model->activeHelperMode == 1);
}

bool IsPlayerPartyGuideBlocked() {
    const PlayerModelState *state = g_playerRuntime.model;
    return state && (state->padFlags & 0x1000U) == 0 &&
        ((state->flags & 0x40000U) == 0 || (state->motionInputState != 10U && state->placementTargetState != 1U));
}

} // namespace shenmue::s2
