#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/player_model_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/object_xforms.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/party_companion.hpp"
#include "s2/player_flags_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/math.hpp"
#include <cmath>
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
#include "s2/player_internal.hpp"
#endif


namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void AllocateFIXO(Character *character);

PartyCompanion *FindPartyCompanionByName(shen_name name);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t plymGetPartyAuthorStatus(shen_name name) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const PartyCompanion *const party = FindPartyCompanionByName(name);
    if (!party)
        return 0;
#else
    const PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(name));
    if (!party) return 0;
#endif
    if (party->mode == 2) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        if (party->placement.flags & 0x20U) {
            if (!party->behaviorState)
                return 2;
            return 1;
#else
        const PlayerPlacementNode *const placement = &party->placement;
        if (!(placement->flags & 0x20U) && !(party->flags & 0x40000U)) return 0;
        return party->behaviorState ? 1 : 2;
#endif
        }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        if (!(party->flags & 0x40000U))
            return 0;
        if (!party->behaviorState)
            return 2;
        return 1;
#else
    return party->behaviorState ? 0 : 1;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (party->behaviorState)
        return 0;
    return 1;
}
#endif
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetPartyCompanionRouteIndex(shen_name name) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t index;
    PartyCompanion *party = FindPartyCompanionByName(name);
    if (party)
        index = party->routeIndex;
    else
        index = 0;
    return index;
#else
    const PartyCompanion *const party = FindPartyCompanion(ctrlGetScenarioChara(name));
    return party ? party->routeIndex : 0;
#endif
}
#endif

namespace player_detail {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct RyoBagForm {
    shen_name name;
    MT7Model *model;
};
typedef char Layout_ryo_bag_form[(sizeof(RyoBagForm) == 8) ? 1 : -1];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ctrlAssignCharaHandlerFor(PlayerModelState &state) {
    if (!state.helperCharacter)
        return;
    const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    UnassignScenarioCharacter(control->scenarioTask, state.helperCharacter);
    MarkCharacterForCleanup(state.helperCharacter, 1);
    state.helperCharacter = 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void modelFree(Character *character, void *payload) {
    RyoBagForm *const form = static_cast<RyoBagForm *>(payload);
    character->model = 0;
    SetCharacterAlpha(character, 0.0f);
    if (form->model) {
        ReleaseMT7Model(form->model);
        form->model = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t plymCreateRyoBagCharacter(PlayerModelState &state, std::int32_t mode) {
#else
std::int64_t plymCreateRyoBagCharacter(PlayerModelState &state, std::int32_t mode) {
#endif
    shen_name name;
    const char *filename;
    RyoBagForm *form;
    switch (mode) {
    default:
    case 1:
        name = SHEN_NAME(RCKO);
        filename = "RYBG2H2G.MT7";
        break;
    case 2:
        name = SHEN_NAME(RCKF);
        filename = "RYBG2F2G.MT7";
        break;
    }
    if (state.helperCharacter) {
        form = static_cast<RyoBagForm *>(charFindComp(state.helperCharacter, SHEN_NAME(FORM)));
        if (form && form->name == name)
            return 1;
        ctrlAssignCharaHandlerFor(state);
    }
    Character *const character = AddCharacterHandler(3);
    if (!character)
        return 0;
    form = static_cast<RyoBagForm *>(AllocateModelComponent(character, SHEN_NAME(FORM), sizeof(RyoBagForm), 0));
    if (!form)
        return 0;
    SetModelComponentCleanup(character, SHEN_NAME(FORM), modelFree);
    const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    CreateScenarioCharacterId(control->scenarioTask, name, character);
    AllocateFIXO(character);
    MT7Model *const model = InitializeMT7Model(shenLoadFile("misc", filename, SHEN_NAME(FORM)));
    character->model = model->root;
    SetCharacterAlpha(character, 1.0f);
    EV_SetCharDisplay(character, CVM_DisplayEnabled, 0);
    imgmAssignUpdateCallback(character, charaPrepareNodesForDraw);
    form->name = name;
    form->model = model;
    state.helperCharacter = character;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void plymSelectRyoBagMode(PlayerModelState &state) {
    if (!(state.flags & 0x20U)) {
        if (state.helperFlags & 2U)
            state.pendingHelperMode = 0;
        return;
    }
    if (static_cast<std::int8_t>(state.activeHelperMode) == 0) {
        if (!(state.helperFlags & 2U)) {
            if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 123)) {
                state.pendingHelperMode = 1;
                if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 551))
                    state.pendingHelperMode = 2;
            }
        } else {
            state.pendingHelperMode = 0;
        }
        return;
    }
    switch (static_cast<std::int8_t>(state.activeHelperMode)) {
    case 1:
        if (!(state.helperFlags & 2U)) {
            if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 123)) {
                state.pendingHelperMode = 1;
                if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 551))
                    state.pendingHelperMode = 2;
            }
        } else {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 123))
                state.pendingHelperMode = 0;
            else if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 551))
                state.pendingHelperMode = 2;
        }
        return;
    case 2:
        if (!(state.helperFlags & 2U)) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 123))
                return;
            if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 551)) {
                state.pendingHelperMode = 2;
                return;
            }
        } else {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 123)) {
                state.pendingHelperMode = 0;
                return;
            }
            if (EV_GetPlayerFlag(PlayerFlagReadMode_Internal, 551))
                return;
        }
        state.pendingHelperMode = 1;
        return;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void plymReleaseRyoBag(PlayerModelState &state, MotionWork *motion) {
    ResetCharaFixo(state.helperCharacter);
    mtwkInitFlags(motion, 0);
    ctrlAssignCharaHandlerFor(state);
    SetupHandPoseScript(state.character);
    state.helperFlags &= ~2U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void plymApplyPendingRyoBagMode(PlayerModelState &state) {
    MotionWork *const motion = state.motion;
    if (state.pendingHelperMode == -1 || !motion || !(state.padFlags & 2U))
        return;
    const std::int8_t pending = state.pendingHelperMode;
    state.pendingHelperMode = -1;
    if (static_cast<std::int8_t>(state.activeHelperMode) != 0 && pending != 0 && static_cast<std::int8_t>(state.activeHelperMode) != pending) {
        ResetCharaFixo(state.helperCharacter);
        mtwkInitFlags(motion, 0);
        ctrlAssignCharaHandlerFor(state);
        SetupHandPoseScript(state.character);
        state.helperFlags &= ~2U;
    }
    switch (pending) {
    case 0:
        ResetCharaFixo(state.helperCharacter);
        mtwkInitFlags(motion, 0);
        ctrlAssignCharaHandlerFor(state);
        SetupHandPoseScript(state.character);
        state.helperFlags &= ~2U;
        break;
    case 1: {
        mtwkRequestMotion(motion, 0xf007, static_cast<MotionRegion>(4));
        plymCreateRyoBagCharacter(state, pending);
        Vector3 position;
        position.x = -0.105f;
        position.y = 0.19f;
        position.z = -0.067000002f;
        const std::int32_t rotation[3] = {58892, 44638, 51628};
        SetFixoDataForMotionWork(state.helperCharacter, state.character, 15, position, rotation);
        EV_SetCharDisplay(state.helperCharacter, CVM_DisplayEnabled, 1);
        hmdlSetFlagsFor(state.character, 1, 0, 1);
        state.activeHelperMode = pending;
        state.helperFlags |= 2U;
        break;
    }
    case 2: {
        plymCreateRyoBagCharacter(state, pending);
        EV_SetCharDisplay(state.helperCharacter, CVM_DisplayEnabled, 1);
        Vector3 position;
        position.x = -0.13177f;
        position.y = 0.18569f;
        position.z = -0.074680001f;
        const std::int32_t rotation[3] = {60823, 44637, 51623};
        SetFixoDataForMotionWork(state.helperCharacter, state.character, 15, position, rotation);
        state.activeHelperMode = pending;
        state.helperFlags |= 2U;
        break;
    }
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void plymUpdateRyoBagVisibility(PlayerModelState &state) {
    if (!state.helperCharacter)
        return;
    Vector3 bag;
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    GetCharacterWorldPosition(state.helperCharacter, &bag);
    float distance = CalculateDistanceBetweenPoints(&bag, &camera);
    float threshold = 0.34999999f;
    float alpha;
    if (threshold > distance) {
        alpha = 0.0f;
    } else {
        float range = 0.2f;
        distance -= threshold;
        alpha = distance / range;
        if (0.0f > alpha || 1.0f > alpha) {
            if (0.0f > alpha)
                alpha = 0.0f;
        } else {
            alpha = 1.0f;
        }
    }
    state.helperCharacter->alpha = alpha;
    if (alpha != 1.0f)
        SetFixoAlphaPropagation(state.helperCharacter, 1);
    else
        SetFixoAlphaPropagation(state.helperCharacter, 0);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
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
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
void ReleaseRyoBagModel(RyoBagModel *bag) {
    ReleaseModelFormat7(bag->model);
    HLib::FreeTaskData(bag->source);
    delete bag;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
// Native modelFree: the FORM cleanup drops the bag and shadow readers.
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
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
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
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
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
    EV_SetCharDisplay(character, CVM_DisplayEnabled, 0);
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
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
// Internal flag 123 carries the bag; 551 selects the shouldered RCKF form.
// @verified - d
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
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
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
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
void plymReleaseRyoBag(PlayerModelState &state, MotionWork *motionWork) {
    if (state.helperCharacter)
        ResetCharaFixo(state.helperCharacter);
    mtwkInitFlags(motionWork, 0);
    ctrlAssignCharaHandlerFor(state);
    SetupHandPoseScript(state.characterNode);
    state.helperFlags &= ~2U;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
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
        EV_SetCharDisplay(state.helperCharacter, CVM_DisplayEnabled, 1);
        charaSetDisplayEnabledFlag(state.helperCharacter, 1);
        charaSetVisibilityFlagTo(state.helperCharacter, 1);
        SetCharacterType(state.helperCharacter, CharacterTypeStencilShadow);
        hmdlSetFlagsFor(state.characterNode, 1, 0, 1);
        state.helperFlags |= 2U;
        state.activeHelperMode = 1;
    } else if (pending == 2) {
        plymCreateRyoBagCharacter(state, 2);
        EV_SetCharDisplay(state.helperCharacter, CVM_DisplayEnabled, 1);
        SetFixoDataForMotionWork(state.helperCharacter, state.characterNode, 15, {-0.13177f, 0.18569f, -0.074680001f}, {60823, 44637, 51623});
        state.helperFlags |= 2U;
        state.activeHelperMode = 2;
    }
}
#endif

#if (SHENMUE_IS_DC && SHENMUE_IS_S2) || !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
// @verified - d
void plymUpdateRyoBag(PlayerModelState &state) {
    plymSelectRyoBagMode(state);
    if (state.pendingHelperMode != -1)
        plymApplyPendingRyoBagMode(state);
    plymUpdateRyoBagVisibility(state);
}
#endif

} // namespace player_detail
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t npcGetPosFor(shen_name name, Vector3 *position);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void UpdatePlayerPartyCompanions(PlayerModelState *model) {
    PlayerModelState &state = *model;
    bool following = false;
    PlayerRuntimeState *const runtime = g_playerRuntimePointer;
    std::int32_t requested = (runtime->flags & 0x400002U) == 2U;
    std::int32_t changed = 0;
    shen_name name = SHEN_NAME(REN_);
    if (requested && EV_GetShenmueFlag(400)) {
        if (!FindPTYCForChara(name)) {
            ActivatePartyCompanion(name);
            changed = 1;
            if (FindPTYCForChara(name))
                SetPartyCompanionMode(name, 1);
        }
    } else if (requested && EV_GetShenmueFlag(401)) {
        Vector3 position;
        if (npcGetPosFor(name, &position)) {
            const float distance = CalculateDistanceBetweenPoints(&position, &state.searchLocation);
            if (distance >= 15.0f)
                plymRequestRetryScene(3, SHEN_NAME(QR00), 97);
        }
    } else {
        if (EV_GetShenmueFlag(401) || EV_GetShenmueFlag(400))
            SetPartyNpcRouteReload(name, 1);
        if (FindPTYCForChara(name)) {
            ptycCharaUnlock(name);
        } else if ((state.padFlags & 0x40000U) && (EV_GetShenmueFlag(400) || EV_GetShenmueFlag(401))) {
            changed = -1;
            FreeInactivePartyCompanion(name);
        }
    }
    name = SHEN_NAME(SIN_);
    if (requested && EV_GetShenmueFlag(402)) {
        if (static_cast<std::int8_t>(state.motionInputState) != 10 && static_cast<std::int8_t>(state.motionInputState) != 1) {
            shenToggleAreaFlags(10);
            state.interactionTarget = name;
            state.primaryInteractionDistanceLimit = 20.0f;
            state.padFlags |= 0x80000U;
        }
        if (!FindPTYCForChara(name)) {
            changed = 1;
            ActivatePartyCompanion(name);
        }
        if (FindPTYCForChara(name)) {
            SetPartyCompanionMode(name, 2);
            following = true;
        }
    } else if (requested && EV_GetShenmueFlag(403)) {
        if (!FindPTYCForChara(name)) {
            changed = 1;
            ActivatePartyCompanion(name);
            if (FindPTYCForChara(name))
                SetPartyCompanionMode(name, 1);
        }
        if (FindPTYCForChara(name))
            SetPartyCompanionMode(name, 1);
    } else {
        if (EV_GetShenmueFlag(402) || EV_GetShenmueFlag(403))
            SetPartyNpcRouteReload(name, 1);
        if (FindPTYCForChara(name)) {
            ptycCharaUnlock(name);
        } else if ((state.padFlags & 0x40000U) && (EV_GetShenmueFlag(402) || EV_GetShenmueFlag(403))) {
            changed = -1;
            FreeInactivePartyCompanion(name);
        }
    }
    name = SHEN_NAME(JOP_);
    if (requested && EV_GetShenmueFlag(404)) {
        if (!FindPTYCForChara(name)) {
            changed = 1;
            ActivatePartyCompanion(name);
            if (FindPTYCForChara(name))
                SetPartyCompanionMode(name, 1);
        }
        if (FindPTYCForChara(name))
            SetPartyCompanionMode(name, 1);
    } else if (FindPTYCForChara(name)) {
        ptycCharaUnlock(name);
    } else if ((state.padFlags & 0x40000U) && EV_GetShenmueFlag(404)) {
        changed = -1;
        FreeInactivePartyCompanion(name);
    }
    if (!following && (static_cast<std::int8_t>(state.motionInputState) == 10 || static_cast<std::int8_t>(state.motionInputState) == 7)) {
        shenToggleAreaFlags(0);
        plymAddFlags(0x10000000U);
    }
    if (changed == 1)
        state.padFlags |= 0x40000U;
    if (changed == -1)
        state.padFlags &= ~0x40000U;
}
#endif

namespace player_detail {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
// Native FindShenmueNodeFromScenarioTask: the character's scenario name through CTRL's scenario task.
shen_name FindShenmueNodeFromScenarioTask(Character *character) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return scenGetCharaName(control->scenarioTask, character);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
// Native ptycFindDoorCompanion: the party companion (the named one, or REN_/SIN_/JOP_) in its door state 5 on the
// door PLYM holds as its search region.
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
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
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
        if ((EV_GetShenmueFlag(0x191) || EV_GetShenmueFlag(0x190)) && !RegisterNpcFlags(PLAYER_COMPANION_REN, NRSF_AbstractActive, 2))
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
        if ((EV_GetShenmueFlag(0x192) || EV_GetShenmueFlag(0x193)) && !RegisterNpcFlags(PLAYER_COMPANION_SHENHUA, NRSF_AbstractActive, 2))
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
#endif

} // namespace player_detail
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t plymHasRyoBag(std::int32_t mode) {
#else
bool plymHasRyoBag(std::int32_t mode) {
#endif
    const PlayerModelState *const model = g_playerRuntime.model;
    return model && (model->helperFlags & 2U) && (mode != 1 || model->activeHelperMode == 1);
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2) && !SHENMUE_IS_DC
bool IsPlayerPartyGuideBlocked() {
    const PlayerModelState *state = g_playerRuntime.model;
    return state && (state->padFlags & 0x1000U) == 0 &&
        ((state->flags & 0x40000U) == 0 || (state->motionInputState != 10U && state->placementTargetState != 1U));
}
#endif


} // namespace s2
} // namespace shenmue
