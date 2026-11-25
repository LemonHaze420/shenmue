#include "s2/search_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/am2_vector.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/camera_search_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/search_model_dc.hpp"
#include "s2/search_runtime_dc.hpp"
#include "s2/search_prop_ik_dc.hpp"
#include <cstring>
#include "common/native_sqrt.hpp"
#include "s2/search_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include <cmath>
#include "s2/character_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include <cstdio>
#include "s2/inventory_dc.hpp"
#include "s2/scenario_flags.hpp"
#include <float.h>

namespace shenmue {
namespace s2 {

void srchInitialize(SearchTaskData &search);
SearchTarget *srchReadMapTargets();
void srchReadFromData(SearchTaskData &search);
void ResetSearchCharacterNames();
void UpdatePropSearchVisibility(MT7Node *root, std::uint32_t parentMarker, std::int32_t enabled);
std::uint32_t srchRegisterAndPush(MT7Node *root);
void SearchControlTask_Callback(void *taskData);
void srchUpdate(SearchTaskData &search);
void srchUpdateLockOn(SearchTaskData &search);
void srchComputeLockOn(SearchTaskData &search);
std::int32_t srchRestoreRyoNpcStates(SearchTaskData &search);
void srchIncrementTemporaryModel();
void srchStaticApplySearchPosRot(void *);
void srchLoadSignBoardModelForArea(SearchTaskData &search);
bool srchSetTarget(std::uint32_t index, SearchTarget *targets);
void srchReset();
void UpdateFollowPositions();
float FindBestFollowCandidate();
void DetectTargetFromView();
void SwitchDetectedTarget();
shen_name GetSearchCharacterName();

void srchReadSelectionSounds(const std::byte *configuration);
bool srchGetEulerAngles(const SearchTarget *target, Vector3 *position, std::uint32_t *yaw);
bool srchGetEulerAngles(const SearchTarget *target, Vector3 *position, std::uint32_t *yaw);
const SearchConfiguration *FindSearchConfiguration(const SearchTaskData &search, shen_name name);
Vector3 *g_publishedSearchPosition = 0;
SearchSavedState savedPlayerState;
Vector3 selectedSearchPosition;
shen_name selectedSearchName;
shen_name focusObject;
const SearchConfiguration *activeSearchConfig = 0;
std::uint8_t inputHistory[16], lastInputX = 0, lastInputY = 0;
std::uint16_t lastDirectionInput = 0;
std::uint32_t inputHistoryIndex = 0, searchExitRequested = 0;
MT7Model *g_searchTemporaryModel = 0;
float searchTemporaryAlpha = 0.0f, searchTemporaryAlphaStep = 0.0f;
shen_name g_searchLastSignboard;
shen_name anchorCharacter;
std::int32_t lastSearchCone;
Vector3 currentFollowPosition, focusPosition, followPosition, anchorPosition = {}, playerSearchPosition = {};
Vector3 searchTemporaryPosition;
std::uint32_t searchTemporaryYaw;
shen_name scenarioFollowCharacter, g_searchCharacterFallback;
std::int32_t scenarioFollowGraceFrames = 0;
HLib::Task *searchTask = 0;
SearchTaskData *searchControl = 0;
std::uint32_t GetModelMarker(const MT7Node *node);
void srchApplyObjectTransform(const MT7Node *node, std::uint32_t *yaw);
std::uint32_t GetModelMarker(const MT7Node *node);
MT7Node *FindModelCharacterRecursive(MT7Node *node, shen_name name);
float searchModelRadii[16] = SHENMUE_SEARCH_MODEL_RADII_VALUES;
SearchTarget staticTargets[257];

// @verified - d
void init_search_ctrl() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(SearchControlTask_Callback, 12, 2, sizeof(SearchTaskData), SHEN_NAME4('S', 'R', 'C', 'H'));
    searchControl = static_cast<SearchTaskData *>(HLib::GetTaskParameter(task));
    g_searchTemporaryModel = 0;
    searchTemporaryAlpha = 0.0f;
    searchTemporaryAlphaStep = 0.0f;
    g_searchLastSignboard = 0;
    srchInitialize(*searchControl);
    searchControl->targets[0] = srchReadMapTargets();
    srchResetIK();
    srchReadFromData(*searchControl);
    srchSetupCameraFov();
    ResetSearchCharacterNames();
}

// @verified - d
void srchInitialize(SearchTaskData &search) {
    std::int32_t index;
    search.state = 0;
    search.targetState = 0;
    search.mode = SearchMode::Idle;
    search.currentTarget = 0;
    search.targets[0] = 0;
    search.targets[1] = 0;
    search.targets[3] = 0;
    search.targets[2] = 0;
    search.targets[4] = 0;
    search.contextIndex = 0;
    search.savedState = 0;
    for (index = 0; index < 3; ++index) {
        SearchResourceContext &context = search.contexts[index];
        context.cursor = 0;
        context.remainingTicks = 0.0f;
        context.flags = 0;
        context.resource = 0;
        context.strings = 0;
    }
    search.context = &search.contexts[0];
    g_publishedSearchPosition = 0;
    savedPlayerState.initialized = 1;
    savedPlayerState.playerFlags = 0;
    savedPlayerState.used = 0;
    activeSearchConfig = 0;
    selectedSearchName = 0;
    lastInputX = lastInputY = 0;
    lastDirectionInput = 0;
    for (index = 0; index < 16; ++index)
        inputHistory[index] = 0;
    std::memset(staticTargets, 0, sizeof(staticTargets));
    searchExitRequested = 0;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
SearchTarget *srchReadMapTargets() {
    char directory[64], filename[32];
    return 0;
}
#else
SearchTarget *srchReadMapTargets();
#endif

// @verified - d
bool ReadLOCK(SearchTarget *targets) {
    SearchTaskData *const control = searchControl;
    if (targets && !control->currentTarget && !(control->targetState & 2)) {
        control->targets[4] = control->targets[0];
        control->targets[0] = targets;
        control->targetState |= 2;
        return true;
    }
    return false;
}

// @verified - d
void srchPushPopTarget() {
    SearchTaskData *const control = searchControl;
    if (control->currentTarget)
        return;
    if (control->targetState & 2) {
        control->targets[0] = control->targets[4];
        control->targets[4] = 0;
    }
    control->targetState = static_cast<std::int16_t>(control->targetState) & ~2;
}

// @verified - d
void srchReadFromData(SearchTaskData &search) {
    char directory[64], filename[32];
    std::strcpy(directory, Filepath_Gen(FilepathGenMode::SceneDirectory));
    std::strcpy(filename, "search.bin");
    std::byte *const resource = static_cast<std::byte *>(shenLoadFile(directory, filename, SEARCH_TASK));
    if (!resource)
        return;
    HLib::AssignDynamicAlignmentForTaskData(resource);
    search.context->resource = resource;
    const std::uint32_t offset = reinterpret_cast<const std::uint32_t *>(resource)[2];
    if (offset)
        search.context->strings = resource + offset;
    else
        search.context->strings = 0;
}

// @verified - d
void ResetSearchCharacterNames() {
    g_searchCharacterFallback = focusObject = 0;
}

// @verified - d
bool ReadSRCH(void *resource) {
    SearchTaskData *const control = searchControl;
    if (!resource || control->currentTarget || control->contextIndex >= 2)
        return false;
    control->context = &control->contexts[++control->contextIndex];
    control->context->resource = static_cast<std::byte *>(resource);
    const std::uint32_t offset = reinterpret_cast<const std::uint32_t *>(resource)[2];
    control->context->strings = offset ? static_cast<std::byte *>(resource) + offset : 0;
    if (MT7Node *const root = GetPropRoot()) {
        UpdatePropSearchVisibility(root, 0, 0);
        srchRegisterAndPush(root->child);
    }
    return true;
}

// @verified - d
void SRCHParser(const std::byte *resource) {
    SearchTaskData *const control = searchControl;
    if (control->currentTarget)
        return;
    if (control->contextIndex) {
        for (std::int32_t index = 1; index < 3; ++index) {
            if (control->contexts[index].resource != resource)
                continue;
            for (std::int32_t next = index; next < control->contextIndex; ++next)
                control->contexts[next] = control->contexts[next + 1];
            SearchResourceContext *const last = &control->contexts[control->contextIndex];
            last->resource = 0;
            last->cursor = 0;
            last->strings = 0;
            last->retained = 0;
            last->flags = 0;
            last->remainingTicks = 0.0f;
        }
        control->context = &control->contexts[--control->contextIndex];
    }
    if (MT7Node *const root = GetPropRoot()) {
        UpdatePropSearchVisibility(root, 0, 0);
        srchRegisterAndPush(root->child);
    }
}

// @verified - d
float srchGetDefaultModelRadius(std::uint32_t name) {
    if (static_cast<char>(name) != 'M')
        return 0.0f;
    char digit[2];
    digit[0] = name >> 8;
    digit[1] = 0;
    std::int32_t index = digit[0] - '0';
    if (static_cast<std::uint32_t>(index) > 9U)
        index = digit[0] - '7';
    return index < 16 ? searchModelRadii[index] : 0.0f;
}

// @verified - d
void srchUpdateInputBuff() {
    SearchTaskData *const control = searchControl;
    inputHistory[inputHistoryIndex] = 0;
    if (control->input->leftStickY != 0x80) {
        lastInputX = control->input->leftStickY;
        inputHistory[inputHistoryIndex] = 1;
    }
    if (control->input->leftStickX != 0x80) {
        lastInputY = control->input->leftStickX;
        inputHistory[inputHistoryIndex] |= 2;
    }
    if (control->input->held & 0xf000U) {
        lastDirectionInput = control->input->held;
        inputHistory[inputHistoryIndex] |= 4;
    }
    inputHistoryIndex = (inputHistoryIndex + 1U) & 0xfU;
}

// @verified - d
bool srchSavePlayerState(SearchTaskData &search) {
    SearchSavedState &saved = savedPlayerState;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const player = control->ryo;
    if (player && !search.savedState) {
        const std::uint32_t flags = plymGetCurrentFlags();
        saved.playerFlags = flags;
        plymSetFlags(flags & ~0x20U);
        saved.characterDisplay = IsOfThisVisibilityType(player, CVM_DisplayEnabled);
        EV_SetCharDisplay(player, CVM_DisplayEnabled, 0);
        saved.characterFlags = IsCharacterCollisionCallbackEnabled(player);
        charaSetVisibilityFlagTo(player, 0);
        search.savedState = &saved;
        return true;
    }
    return false;
}

// @verified - d
std::uint32_t srchRegisterStaticObject(MT7Node *node, std::uint32_t marker) {
    SearchTarget *target = staticTargets;
    if (node->kind)
        return 0;
    std::int32_t index = 0;
    do {
        std::uint32_t name = marker;
        if (target->name == name)
            return 0;
        if (static_cast<std::int8_t>(name) == 'M' && static_cast<std::int8_t>(name >> 8) != 'A' && !node->child)
            return 0;
        ++index;
        if (!target->name) {
            Vector3 position;
            std::uint32_t yaw;
            GetPropVector4(&position);
            target->name = name;
            const SearchConfiguration *const configuration = FindSearchConfiguration(*searchControl, marker);
            if (static_cast<std::int8_t>(name) == 'K') {
                yaw = 0;
                matrix_stack::PushUnitMatrix();
                matrix_stack::TranslateVector(&position);
                srchApplyObjectTransform(node, &yaw);
                matrix_stack::GetTranslation(&target->position);
                PopCurrentMatrix(1);
                MT7Node *child = 0;
                name = (name & 0xffffff00U) | 'k';
                if (node->child)
                    child = FindModelCharacterRecursive(node->child, shen_name(name));
                if (!child)
                    target->lockonAngle = 0.600000024f;
                else {
                    const float square = child->position.x * child->position.x + child->position.y * child->position.y + child->position.z * child->position.z;
                    target->lockonAngle = shenmue::NativeSqrt(square);
                }
                target->yaw = static_cast<std::int16_t>(yaw) - 0x8000;
            } else {
                target->position.x = position.x;
                target->position.y = position.y;
                target->position.z = position.z;
                target->lockonAngle = srchGetDefaultModelRadius(target->name);
                target->yaw = static_cast<std::int16_t>(0x8000U);
                if (configuration && configuration->radius > FLT_EPSILON)
                    target->lockonAngle = configuration->radius;
            }
            target->yawLimit = 0x3c71;
            target->flags = 0;
            if (configuration)
                target->yawLimit = configuration->yawLimit;
            return 1;
        }
        if (index >= 256)
            break;
        ++target;
    } while (true);
    return 0;
}

// @verified - d
void UpdatePropSearchVisibility(MT7Node *root, std::uint32_t parentMarker, std::int32_t enabled) {
    if (!root)
        return;
    const std::uint32_t marker = GetModelMarker(root);
    if (static_cast<std::int8_t>(marker) == 'M') {
        if (parentMarker != marker) {
            const SearchConfiguration *const configuration = FindSearchConfiguration(*searchControl, shen_name(marker));
            enabled = 0;
            if (configuration) {
                if (configuration->flags & 4U) {
                    if (EV_GetShenmueFlag(configuration->flags >> 16))
                        enabled = (static_cast<std::int32_t>(configuration->flags) >> 5) & 1;
                    else
                        enabled = ((static_cast<std::int32_t>(configuration->flags) >> 5) ^ 1) & 1;
                } else if (configuration->flags & 8U) {
                    if (GetInternalFlag(configuration->flags >> 16))
                        enabled = (static_cast<std::int32_t>(configuration->flags) >> 5) & 1;
                    else
                        enabled = ((static_cast<std::int32_t>(configuration->flags) >> 5) ^ 1) & 1;
                } else if (configuration->flags & 0x10U) {
                    if (GetItemState(configuration->flags >> 16))
                        enabled = (static_cast<std::int32_t>(configuration->flags) >> 5) & 1;
                    else
                        enabled = ((static_cast<std::int32_t>(configuration->flags) >> 5) ^ 1) & 1;
                }
            }
        }
        root->kind = static_cast<std::int16_t>(enabled);
    }
    if (root->child)
        UpdatePropSearchVisibility(root->child, marker, enabled);
    if (root->sibling)
        UpdatePropSearchVisibility(root->sibling, parentMarker, enabled);
}

// @verified - d
std::uint32_t srchRegisterAndPush(MT7Node *root) {
    std::uint32_t registered = 0;
    if (!root)
        return 0;
    std::memset(staticTargets, 0, sizeof(staticTargets));
    for (MT7Node *node = root; node; node = node->sibling) {
        const std::uint32_t marker = GetModelMarker(node);
        if ((static_cast<std::int8_t>(marker) == 'K' || static_cast<std::int8_t>(marker) == 'M') && srchRegisterStaticObject(node, marker))
            registered = 1;
    }
    srchSetTarget(3, staticTargets);
    return registered;
}

// @verified - d
static std::int32_t srchHasRegisteredTarget() {
    SearchTaskData *const control = searchControl;
    for (std::int32_t index = 0; index < 4; ++index) {
        if (control->targets[index])
            return 1;
    }
    return 0;
}

// @verified - d
void SearchControlTask_Callback(void *taskData) {
    SearchTaskData &search = *static_cast<SearchTaskData *>(taskData);
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control->ryo)
        return;
    search.initialCameraState = camCtrlGetState();
    search.targetFov = plymGetLookZoomScale();
    search.input = ctrlGetPadInputBuffer0(GetMapPadControlEntry());
    const LegacyInputSlot &input = *search.input;
    if ((input.held & 0xf7ffU) || input.triggers[0] || input.triggers[1] || input.leftStickX != 0x80 || input.leftStickY != 0x80)
        search.idleTicks = 0;
    else
        ++search.idleTicks;
    switch (search.mode) {
    case SearchMode::Idle:
        searchExitRequested = 0;
        search.idleTicks = 0;
        if (!IsPlayerControlEnabled())
            search.targetFov = 0;
        srchUpdate(search);
        break;
    case SearchMode::Signboard:
        srchUpdateInputBuff();
        if (!shenCheckCurrentFoVModeIs2()) {
            search.mode = SearchMode::Lockon;
            srchGetEulerAngles(search.currentTarget, &selectedSearchPosition, 0);
            g_publishedSearchPosition = &selectedSearchPosition;
            selectedSearchName = search.currentTarget->name;
            srchSavePlayerState(search);
            activeSearchConfig = FindSearchConfiguration(search, search.currentTarget->name);
            srchReadSelectionSounds(reinterpret_cast<const std::byte *>(activeSearchConfig));
            if (activeSearchConfig && !(search.state & 4))
                search.context->cursor = reinterpret_cast<std::byte *>(const_cast<SearchConfiguration *>(activeSearchConfig)) + sizeof(SearchConfiguration);
            else
                search.context->cursor = 0;
            search.state &= ~8;
            search.state &= ~4;
        }
        break;
    case SearchMode::Lockon:
        srchUpdateLockOn(search);
        break;
    }
    srchIncrementTemporaryModel();
    HLib::ExecuteCallbackNow(srchStaticApplySearchPosRot, 0);
}

// @verified - d
void srchUpdate(SearchTaskData &search) {
    Vector3 next, player, point, chosenPosition, screen, eye;
    std::uint32_t yaw;
    SearchTarget *chosen = 0;
    UpdateFollowPositions();
    float best = FindBestFollowCandidate();
    if (!g_searchCharacterFallback || (g_searchCharacterFallback != focusObject && g_searchCharacterFallback != scenarioFollowCharacter)) {
        if (search.initialCameraState != 22 || (search.input->held & 0x200U) || (search.state & 1U) || search.targetFov < 120 ||
            ctrlGetLookOverlayWeight() > 0.05f)
            return;
        camCtrlGetNextCamPos(&next);
        plymUpdatePlayerSearchLocation(&player);
        for (std::int32_t bank = 0; bank < 4; ++bank) {
            SearchTarget *target = search.targets[bank];
            if (!target)
                continue;
            for (; target->name; ++target) {
                float range = 3.0f;
                const shen_name name = target->name;
                if ((target->flags & 2U) || (static_cast<char>(name) == 'K' && drGetCurrentAuthorId()))
                    continue;
                if (!srchGetEulerAngles(target, &point, &yaw))
                    continue;
                float x = next.x - point.x, y = next.y - point.y, z = next.z - point.z;
                std::int16_t direction = dc::sdk::ArcTan2Fixed(x, z);
                std::int16_t difference;
                if (direction - camCtrlGetNextRotY() < 0)
                    difference = camCtrlGetNextRotY() - direction;
                else
                    difference = direction - camCtrlGetNextRotY();
                if (difference > 5461)
                    continue;
                x = player.x - point.x;
                y = player.y - point.y;
                z = player.z - point.z;
                float distance = x * x + z * z;
                if (static_cast<char>(name) == 'K') {
                    range = std::fabs(y) * 1.5f > range ? std::fabs(y) * 1.5f : range;
                    if (distance > range * range || y > 0.0f)
                        continue;
                } else if (target->flags & 8U) {
                    if (distance > range * range)
                        continue;
                } else if (distance > range * range || y < -3.0f || y > 0.0f)
                    continue;
                if (target->yawLimit) {
                    direction = dc::sdk::ArcTan2Fixed(-x, -z);
                    difference = direction - static_cast<std::int16_t>(yaw);
                    std::int32_t delta = difference;
                    if (delta < 0)
                        delta = -delta;
                    if (delta > target->yawLimit)
                        continue;
                }
                ProjectConversationActorToScreen(point, screen);
                if (screen.z > 0.0f || (screen.x < 0.0f ? -screen.x : screen.x) > 180.0f || (screen.y < 0.0f ? -screen.y : screen.y) > 140.0f)
                    continue;
                distance = screen.x * screen.x + screen.y * screen.y;
                if (best > distance) {
                    best = distance;
                    chosen = target;
                    chosenPosition = point;
                }
            }
        }
        if (chosen) {
            g_searchCharacterFallback = 0;
            search.currentTarget = chosen;
            search.state = 0;
            search.mode = SearchMode::Signboard;
            srchLoadSignBoardModelForArea(search);
            if (srchGetPropIKIndex())
                search.fovStyle = 7;
            else
                search.fovStyle = 1;
            srchSetLookTarget(0);
            srchSetLookBase(&chosenPosition);
            camCtrlGetNextCamPos(&eye);
            srchSetRotY(dc::sdk::ArcTan2Fixed(chosenPosition.x - eye.x, chosenPosition.z - eye.z));
            camCtrlSetLockedOnAngle(chosen->lockonAngle);
            camCtrlSwitchToLockedOn(search.fovStyle);
            if (search.currentTarget->flags & 4U)
                shenApplyLockonFlag(1);
            return;
        }
        if (!g_searchCharacterFallback)
            return;
    }
    SwitchDetectedTarget();
    search.mode = SearchMode::Lockon;
    search.currentTarget = 0;
    search.state = 0;
    search.fovStyle = 1;
}

// @verified - d
void srchUpdateLockOn(SearchTaskData &search) {
    if (g_searchCharacterFallback) {
        srchComputeLockOn(search);
        if (plymIsSearchable() && !plymIsSearchBlocked() && !HasScenarioCharacterControl())
            scenarioFollowCharacter = g_searchCharacterFallback && npcGetCanInteract(g_searchCharacterFallback) ? g_searchCharacterFallback : shen_name();
        scenarioFollowGraceFrames = 90;
        return;
    }
    if (search.idleTicks > 10800U)
        searchExitRequested = 1;
    if (search.state & 2U) {
        search.state &= ~2;
        mtwkResetBlends();
        mtwkSetOverridesEnabled(1);
        srchReset();
        camCtrlSetLookAroundMode();
    } else if (!srchProcessSearchIK(search)) {
        if (((search.input->held & 0x200U) || searchExitRequested) && srchGetPropIKIndex() == 0)
            search.state |= 2U;
        else
            srchComputeLockOn(search);
    }
}

void srchComputeLockOn(SearchTaskData &search) {
    Vector3 locked, look, forward;
    struct {
        Vector3 position;
        std::uint32_t yaw;
    } chosenPose;
    Matrix4x4 view;
    Vector3 eye, axis, relative, offset, point;
    std::uint32_t lockedYaw, targetYaw;
    std::int32_t bank;
    SearchTarget *chosen;
    shen_name previous, detected;
    std::int32_t best, direction, haveInput, playerYaw, angle;
    float x, z, length, range, distance;
    haveInput = 1;
    chosen = 0;
    best = 32767;
    previous = g_searchCharacterFallback;
    if (previous) {
        UpdateFollowPositions();
        locked = currentFollowPosition;
        lockedYaw = 0;
        DetectTargetFromView();
        detected = g_searchCharacterFallback;
        if (!detected)
            goto reset;
    } else {
        UpdateFollowPositions();
        if (!srchGetEulerAngles(search.currentTarget, &locked, &lockedYaw)) {
        reset:
            srchReset();
            camCtrlSetLookAroundMode();
            return;
        }
        currentFollowPosition = locked;
        DetectTargetFromView();
        detected = g_searchCharacterFallback;
    }
    if (detected && (detected == focusObject || detected == scenarioFollowCharacter)) {
        SwitchDetectedTarget();
        return;
    }
    plymUpdatePlayerSearchLocation(&look);
    playerYaw = shenGetRotYFromNode(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
    forward.x = -dc::sdk::SinFixed(playerYaw);
    forward.y = 0.0f;
    forward.z = -dc::sdk::CosFixed(playerYaw);
    z = x = 0.0f;
    if (search.input->pressedPriority & 0xf000U) {
        if (search.input->held & 0x8000U)
            z = 1.0f;
        if (search.input->held & 0x4000U)
            z = -1.0f;
        if (search.input->held & 0x1000U)
            x = 1.0f;
        if (search.input->held & 0x2000U)
            x = -1.0f;
    } else
        haveInput = 0;
    direction = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(x, z));
    if (haveInput) {
        matrix_stack::Matrix4x4_Push(0);
        srchGetLookTarget(&eye);
        axis.x = eye.x - locked.x;
        axis.y = eye.y - locked.y;
        axis.z = eye.z - locked.z;
        length = Vector3::CalcMagnitude(axis);
        axis.x /= length;
        axis.y /= length;
        axis.z /= length;
        reinterpret_cast<Vector3 &>(view.rows[2]) = axis;
        axis.x = axis.z = 0.0f;
        axis.y = 1.0f;
        am2Vector3Cross(&axis, reinterpret_cast<Vector3 *>(&view.rows[2]), &axis);
        length = Vector3::CalcMagnitude(axis);
        axis.x /= length;
        axis.y /= length;
        axis.z /= length;
        reinterpret_cast<Vector3 &>(view.rows[0]) = axis;
        am2Vector3Cross(reinterpret_cast<Vector3 *>(&view.rows[2]), reinterpret_cast<Vector3 *>(&view.rows[0]), reinterpret_cast<Vector3 *>(&view.rows[1]));
        reinterpret_cast<Vector3 &>(view.rows[3]) = eye;
        view.rows[0].w = 0.0f;
        view.rows[1].w = 0.0f;
        view.rows[2].w = 0.0f;
        view.rows[3].w = 1.0f;
        matrix_stack::LoadMatrix(&view);
        matrix_stack::InvertMatrix(0);
        matrix_stack::GetMatrix(&view);
        if (g_searchCharacterFallback && g_searchCharacterFallback != previous) {
            relative.x = currentFollowPosition.x - locked.x;
            relative.y = currentFollowPosition.y - locked.y;
            relative.z = currentFollowPosition.z - locked.z;
            matrix_stack::LoadMatrix(&view);
            Matrix4x4::TransformPoint(currentFollowPosition, relative);
            matrix_stack::UnitCurrentMatrix();
            Matrix4x4::RotateAroundZ_Wrapper(-direction);
            Matrix4x4::TransformPoint(relative, relative);
            angle = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(relative.y, relative.x));
            if (10922.0f > ::fabsf(static_cast<float>(static_cast<std::int16_t>(angle)))) {
                const float planar = relative.x * relative.x + relative.y * relative.y;
                best = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(planar * planar, -relative.z));
            }
        }
        for (bank = 0; bank < 4; ++bank) {
            SearchTarget *target = search.targets[bank];
            if (!target)
                continue;
            for (; target->name; ++target) {
                const shen_name name = target->name;
                if (search.currentTarget && name == search.currentTarget->name)
                    continue;
                if (target->flags & 2U)
                    continue;
                if (static_cast<std::int8_t>(name) == 'K' && drGetCurrentAuthorId())
                    continue;
                if (!srchGetEulerAngles(target, &point, &targetYaw))
                    continue;
                offset.x = point.x - look.x;
                offset.y = point.y - look.y;
                offset.z = point.z - look.z;
                distance = offset.x * offset.x + offset.z * offset.z;
                if (static_cast<std::int8_t>(name) == 'K') {
                    range = 36.0f;
                    range = offset.y * offset.y * 1.5f > range ? offset.y * offset.y * 1.5f : range;
                    if (distance > range)
                        continue;
                    if (0.0f > offset.y)
                        continue;
                } else if (target->flags & 8U) {
                    if (distance > 36.0f)
                        continue;
                } else {
                    if (distance > 36.0f)
                        continue;
                    if (offset.y > 3.0f)
                        continue;
                    if (0.0f > offset.y)
                        continue;
                }
                if (target->yawLimit) {
                    const std::int16_t angle = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(offset.x, offset.z));
                    std::int32_t delta = static_cast<std::int16_t>(angle - static_cast<std::int16_t>(targetYaw));
                    if (delta < 0)
                        delta = -delta;
                    if (delta > target->yawLimit)
                        continue;
                }
                offset.y -= 1.6f;
                if (0.0f > am2Vector3Dot(&offset, &forward))
                    continue;
                offset.x = point.x - locked.x;
                offset.y = point.y - locked.y;
                offset.z = point.z - locked.z;
                distance = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
                if (distance > 9.0f)
                    continue;
                matrix_stack::LoadMatrix(&view);
                Matrix4x4::TransformPoint(point, offset);
                matrix_stack::UnitCurrentMatrix();
                Matrix4x4::RotateAroundZ_Wrapper(-direction);
                Matrix4x4::TransformPoint(offset, offset);
                angle = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(offset.y, offset.x));
                const std::int32_t angular = static_cast<std::int32_t>(::fabsf(static_cast<float>(static_cast<std::int16_t>(angle))));
                if (angular > 12743)
                    continue;
                const float ratio = static_cast<float>(angular) / 12743.0f;
                float planar;
                planar = shenmue::NativeSqrt(offset.x * offset.x + offset.y * offset.y);
                const std::int32_t score = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(planar, -offset.z)) + static_cast<std::int32_t>(ratio * 1820.0f);
                if (score >= best)
                    continue;
                best = score;
                chosen = target;
                chosenPose.position = point;
                chosenPose.yaw = targetYaw;
            }
        }
        PopCurrentMatrix(1);
    }
    if (chosen) {
        g_searchCharacterFallback = shen_name();
        search.currentTarget = chosen;
        search.mode = SearchMode::Signboard;
        srchLoadSignBoardModelForArea(search);
        if (srchGetPropIKIndex())
            search.fovStyle = 7;
        else
            search.fovStyle = 1;
        srchSetLookBase(&chosenPose.position);
        srchSetRotY(static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(chosenPose.position.x - look.x, chosenPose.position.z - look.z)));
        camCtrlSetLockedOnAngle(chosen->lockonAngle);
        camCtrlSwitchToLockedOn(search.fovStyle);
        if (search.currentTarget->flags & 4U)
            shenApplyLockonFlag(1);
    } else if (g_searchCharacterFallback) {
        SwitchDetectedTarget();
        search.mode = SearchMode::Lockon;
        search.currentTarget = 0;
        search.state = 0;
        search.fovStyle = 1;
        selectedSearchName = shen_name();
        search.context->flags = 0;
        search.context->cursor = 0;
        srchRestoreRyoNpcStates(search);
    }
}

// @verified - d
std::int32_t srchRestoreRyoNpcStates(SearchTaskData &search) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const player = control->ryo;
    if (!player)
        return 0;
    SearchSavedState *const saved = search.savedState;
    if (!saved)
        return 0;
    plymSetFlags(saved->playerFlags);
    EV_SetCharDisplay(player, CVM_DisplayEnabled, saved->characterDisplay);
    charaSetVisibilityFlagTo(player, saved->characterFlags);
    search.savedState = 0;
    saved->used = 0;
    return 1;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
SearchTarget *FindSearchTarget(const SearchTaskData &search, shen_name name) {
    SearchTarget *found = 0;
    for (std::int32_t bank = 0; bank < 4; ++bank) {
        SearchTarget *target = search.targets[bank];
        if (!target)
            continue;
        for (; target->name; ++target) {
            if (target->name == name)
                found = target;
        }
    }
    return found;
}
#endif

// @verified - d
bool srchLockTarget(SearchTaskData &search, shen_name name, bool faceTarget) {
    SearchTarget *entry = FindSearchTarget(search, name);
    Vector3 position;
    std::uint32_t yaw;
    if (!entry || (entry->flags & 2U) || !srchGetEulerAngles(entry, &position, &yaw))
        return false;
    search.currentTarget = entry;
    search.mode = SearchMode::Signboard;
    srchLoadSignBoardModelForArea(search);
    if (faceTarget) {
        Vector3 player;
        plymUpdatePlayerSearchLocation(&player);
        yaw = dc::sdk::ArcTan2Fixed(position.x - player.x, position.z - player.z);
    }
    srchSetLookBase(&position);
    srchSetRotY(static_cast<std::int16_t>(yaw));
    camCtrlSetLockedOnAngle(entry->lockonAngle);
    camCtrlSwitchToLockedOn(search.fovStyle);
    return true;
}

// @verified - d
const SearchSignModel *SignModelsForCurrentArea() {
    switch (g_currentMap) {
    case SHEN_NAME4('A', 'K', '0', '0'):
        return searchSignModelsAK00;
    case SHEN_NAME4('A', 'R', '0', '2'):
        return searchSignModelsAR02;
    case SHEN_NAME4('A', 'R', '0', '3'):
        return searchSignModelsAR03;
    case SHEN_NAME4('W', 'K', '0', '0'):
        return searchSignModelsWK00;
    case SHEN_NAME4('W', 'N', '0', '0'):
        return searchSignModelsWN00;
    case SHEN_NAME4('W', 'S', '0', '0'):
        return searchSignModelsWS00;
    case SHEN_NAME4('W', 'T', '0', '0'):
        return searchSignModelsWT00;
    case SHEN_NAME4('W', 'R', '0', '0'):
        return searchSignModelsWR00;
    case SHEN_NAME4('W', 'E', '0', '0'):
        return searchSignModelsWE00;
    case SHEN_NAME4('W', 'B', '0', '0'):
        return searchSignModelsWB00;
    }
    return 0;
}

// @verified - d
void shenFreeTempModel() {
    if (g_searchTemporaryModel)
        ReleaseMT7Model(g_searchTemporaryModel);
    g_searchTemporaryModel = 0;
    searchTemporaryAlphaStep = 0.0f;
    g_searchLastSignboard = 0;
}

// @verified - d
void srchIncrementTemporaryModel() {
    if (!g_searchTemporaryModel)
        return;
    searchTemporaryAlpha += searchTemporaryAlphaStep;
    if (searchTemporaryAlphaStep > 0.0f && searchTemporaryAlpha > 1.0f)
        searchTemporaryAlpha = 1.0f;
    if (searchTemporaryAlphaStep < 0.0f && searchTemporaryAlpha < 0.0f) {
        searchTemporaryAlpha = 0.0f;
        shenFreeTempModel();
    }
}

// @verified - d
void srchStaticApplySearchPosRot(void *) {
    if (!g_searchTemporaryModel)
        return;
    matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::preTranslate(searchTemporaryPosition.x, searchTemporaryPosition.y, searchTemporaryPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(searchTemporaryYaw);
    Matrix4x4::RotateAroundY_Wrapper(0x8000);
    Matrix4x4::preTranslate(0.0f, 0.0f, 0.005f);
    charaPrepareNodesForDraw(g_searchTemporaryModel->root, searchTemporaryAlpha);
    PopCurrentMatrix(1);
}

// @verified - d
void srchLoadSignBoardModelForArea(SearchTaskData &search) {
    char directory[256], filename[16];
    if (g_searchLastSignboard == search.currentTarget->name) {
        if (g_searchTemporaryModel)
            searchTemporaryAlphaStep = 0.1f;
        return;
    }
    const std::uint32_t name = search.currentTarget->name;
    if (static_cast<std::int8_t>(name) != 'K')
        return;
    const SearchSignModel *const signs = SignModelsForCurrentArea();
    if (!signs)
        return;
    std::int32_t index = 0;
    for (; name != signs[index].name; ++index)
        if (signs[index].name == SHEN_NAME4('_', '_', '_', '_'))
            return;
    std::sprintf(directory, "SCENE/%02d/MODEL/SIGNBD", g_actNumber);
    std::strcpy(filename, signs[index].file);
    shenFreeTempModel();
    EV_ResetFreeTalkFlags();
    g_searchLastSignboard = name;
    srchGetEulerAngles(search.currentTarget, &searchTemporaryPosition, &searchTemporaryYaw);
    g_searchTemporaryModel = InitializeMT7Model(shenLoadFile(directory, filename, shen_name(SHEN_NAME4('S', 'G', 'B', 'D'))));
    if (!g_searchTemporaryModel)
        return;
    searchTemporaryAlpha = 0.0f;
    searchTemporaryAlphaStep = 0.1f;
}

// @verified - d
bool RetrieveSearchFlagFromPtr(Vector3 &position) {
    if (g_publishedSearchPosition) {
        position = *g_publishedSearchPosition;
        return true;
    }
    return false;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t srchSetTargetEnabled(shen_name name, std::int32_t enabled) {
    std::uint32_t previous = UINT32_MAX;
    SearchTarget *const target = FindSearchTarget(*searchControl, name);
    if (target) {
        if (target->flags & 2)
            previous = 0;
        else
            previous = 1;
        if (enabled)
            target->flags &= ~2U;
        else
            target->flags |= 2U;
    }
    return previous;
}
#endif

// @verified - d
bool srchSetTarget(std::uint32_t index, SearchTarget *targets) {
    SearchTaskData *const control = searchControl;
    if (index < 4) {
        control->targets[index] = targets;
        return true;
    }
    return false;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool srchClearTarget(std::uint32_t index) {
    SearchTaskData *const control = searchControl;
    if (index < 4) {
        control->targets[index] = 0;
        return true;
    }
    return false;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t srchGetActiveTargetPosition(Vector3 *position) {
    if (searchControl->mode == SearchMode::Idle)
        return 0;
    if (g_searchCharacterFallback || srchHasRegisteredTarget() != 1)
        return 0;
    const SearchTarget *const target = searchControl->currentTarget;
    if (!target)
        return 0;
    *position = target->position;
    return 1;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t srchGetActiveTargetYaw(std::int16_t *yaw) {
    if (searchControl->mode == SearchMode::Idle)
        return 0;
    if (g_searchCharacterFallback || srchHasRegisteredTarget() != 1)
        return 0;
    const SearchTarget *const target = searchControl->currentTarget;
    if (!target)
        return 0;
    *yaw = target->yaw;
    return 1;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t srchGetActiveTargetLockonAngle(float *angle) {
    if (searchControl->mode == SearchMode::Idle)
        return 0;
    if (g_searchCharacterFallback || srchHasRegisteredTarget() != 1)
        return 0;
    const SearchTarget *const target = searchControl->currentTarget;
    if (!target)
        return 0;
    *angle = target->lockonAngle;
    return 1;
}
#endif

// @verified - d
void *LoadPackageSearchData(void *data) {
    ReadSRCH(data);
    return data;
}

// @verified - d
void ReleasePackageSearchData(void *data) {
    SRCHParser(reinterpret_cast<const std::byte *>(data));
}

// @verified - d
void *LoadPackageLockData(void *data) {
    ReadLOCK(static_cast<SearchTarget *>(data));
    return data;
}

// @verified - d
void ReleasePackageLockData() {
    srchPushPopTarget();
}

// @verified - d
bool srchHasActiveLockonTarget() {
    if (searchControl->mode != SearchMode::Idle) {
        if (g_searchCharacterFallback)
            return false;
        if (!searchControl->currentTarget)
            return false;
        return true;
    }
    return false;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void srchRequestInhibit() {
    searchControl->state |= 1;
}
#endif

// @verified - d
void srchRequestRelease() {
    searchControl->state |= 2U;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void srchRequestLockonExit() {
    searchExitRequested = 1;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
void srchReset() {
    searchControl->mode = SearchMode::Idle;
    searchControl->state = 0;
    searchControl->currentTarget = 0;
    searchControl->context->flags = 0;
    searchControl->context->cursor = 0;
    selectedSearchName = 0;
    activeSearchConfig = 0;
    srchRestoreRyoNpcStates(*searchControl);
    searchTemporaryAlphaStep = -0.1f;
    g_searchCharacterFallback = 0;
    SetupLightConfig();
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
std::int32_t srchSetLookMode() {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    EV_SetCharDisplay(control->ryo, CVM_DisplayEnabled, 1);
    shenFreeTempModel();
    MT7Node *const root = GetPropRoot();
    UpdatePropSearchVisibility(root->child, 0, 0);
    srchRegisterAndPush(root->child);
    mtwkResetBlends();
    srchReset();
    camCtrlSetLookAroundMode();
    srchResetIK();
    return mtwkSetOverridesEnabled(1);
}
#endif

// @verified - d
std::int32_t srchCheckFlags(std::int32_t mode) {
    if (mode == 2)
        return 0;
    if (mode == 3)
        return srchHasActiveLockonTarget() ? 1 : -1;
    if (mode == 6)
        return static_cast<std::int32_t>(GetSearchCharacterName());
    return 0;
}

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
// @verified - d
shen_name srchGetTargetName(std::uint32_t index) {
    static const shen_name *const srchTargetNames[3] = {&selectedSearchName, &g_searchCharacterFallback, &focusObject};
    if (index < 3)
        return *srchTargetNames[index];
    return shen_name();
}
#endif

// @verified - d
bool ApplyQMarkerTranslation(MT7Node *node, std::uint32_t marker, Vector3 *position) {
    bool found = false;
    if (!node)
        return false;
    if (node->sibling)
        matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
    if (node->rotation.y)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.y);
    if (node->rotation.x)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.x);
    if (marker == GetModelMarker(node)) {
        found = true;
        Matrix4x4::preTranslate(0.12f, -0.1f, 0.0f);
        matrix_stack::GetTranslation(position);
    }
    if (node->child && !found)
        found = ApplyQMarkerTranslation(node->child, marker, position);
    if (node->sibling) {
        PopCurrentMatrix(1);
        if (!found)
            found = ApplyQMarkerTranslation(node->sibling, marker, position);
    }
    return found;
}

// @verified - d
void UpdateFollowPositions() {
    const shen_name *names;
    const Vector3 *positions;
    plymUpdatePlayerSearchLocation(&playerSearchPosition);
    currentFollowPosition = focusPosition = followPosition = anchorPosition = playerSearchPosition;
    if (scenarioFollowCharacter) {
        names = GetScenarioCharacterAdmissionNames();
        positions = srchGetScenarioAdmissionPositions();
        for (; *names; ++names, ++positions) {
            if (*names == scenarioFollowCharacter) {
                followPosition = *positions;
                break;
            }
        }
        if (!*names) {
            Character *const character =
                scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, scenarioFollowCharacter);
            if (character)
                charaGetMotWorldPos(character, 5, &followPosition);
        }
    }
    if (focusObject) {
        names = GetScenarioCharacterAdmissionNames();
        positions = srchGetScenarioAdmissionPositions();
        for (; *names; ++names, ++positions) {
            if (*names == focusObject) {
                focusPosition = *positions;
                break;
            }
        }
        if (!*names) {
            Character *const character = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, focusObject);
            if (character)
                charaGetMotWorldPos(character, 5, &focusPosition);
            else
                focusObject = shen_name();
        }
    }
    anchorCharacter = shen_name(plymGetAuxiliaryState());
    if (anchorCharacter) {
        const Matrix4x4 *const transform = doorFindEntryTransform(anchorCharacter);
        if (transform) {
            const std::int32_t index = reinterpret_cast<const std::int8_t *>(&anchorCharacter)[1] - '0';
            MT7Node *const node = FindModelCharacterRecursive(GetPropRoot(), anchorCharacter);
            anchorPosition.x = searchAnchorOffsets[index].x;
            anchorPosition.y = searchAnchorOffsets[index].y;
            anchorPosition.z = 0.0f;
            matrix_stack::Matrix4x4_Push(0);
            matrix_stack::LoadMatrix(transform);
            Matrix4x4::preTranslate(anchorPosition.x, anchorPosition.y, anchorPosition.z);
            matrix_stack::GetTranslation(&anchorPosition);
            if (node) {
                matrix_stack::LoadMatrix(transform);
                ApplyQMarkerTranslation(node->child, (anchorCharacter & 0xffffff00U) | 'Q', &anchorPosition);
            }
            PopCurrentMatrix(1);
        } else
            anchorCharacter = shen_name();
    }
    if (g_searchCharacterFallback) {
        if (g_searchCharacterFallback == anchorCharacter)
            currentFollowPosition = anchorPosition;
        else if (g_searchCharacterFallback == focusObject)
            currentFollowPosition = focusPosition;
        else {
            names = GetScenarioCharacterAdmissionNames();
            positions = srchGetScenarioAdmissionPositions();
            for (; *names; ++names, ++positions) {
                if (*names == g_searchCharacterFallback) {
                    currentFollowPosition = *positions;
                    break;
                }
            }
            if (!*names)
                g_searchCharacterFallback = shen_name();
        }
    }
}

// @verified - d
float FindBestFollowCandidate() {
    float best = 1000000.0f;
    const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(GetMapPadControlEntry());
    Vector3 cameraPoint;
    Vector2 screen;
    g_searchCharacterFallback = shen_name();
    lastSearchCone = -1;
    if (scenarioFollowCharacter && scenarioFollowGraceFrames > 0 && plymIsSearchable() && !plymIsSearchBlocked() && !HasScenarioCharacterControl()) {
        if (scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, scenarioFollowCharacter)) {
            g_searchCharacterFallback = scenarioFollowCharacter;
            currentFollowPosition = followPosition;
            return 0.0f;
        }
        --scenarioFollowGraceFrames;
        if (input->pressedPriority & 0x200U)
            scenarioFollowGraceFrames = 0;
        if (scenarioFollowGraceFrames <= 0)
            scenarioFollowCharacter = shen_name();
        if (camCtrlGetState() == 7)
            camCtrlSetState(0);
    }
    if (focusObject && plymIsSearchable()) {
        g_searchCharacterFallback = focusObject;
        currentFollowPosition = focusPosition;
        return 0.0f;
    }
    focusObject = shen_name();
    if (!plymIsSearchable() || (input->held & 0x200U) || !(0.05f > ctrlGetLookOverlayWeight()) || camCtrlGetState() != 22)
        return best;
    const std::int32_t zoomScale = plymGetLookZoomScale();
    if (zoomScale <= 120)
        return best;
    const shen_name *names = GetScenarioCharacterAdmissionNames();
    const Vector3 *positions = srchGetScenarioAdmissionPositions();
    reinterpret_cast<void (*)(float, float)>(matrix_stack::UpdateWorldToScreenMatrix)(640.0f, 480.0f);
    if (anchorCharacter) {
        const float x = anchorPosition.x - playerSearchPosition.x, z = anchorPosition.z - playerSearchPosition.z;
        Matrix4x4::TransformPoint(anchorPosition, cameraPoint);
        if (9.0f > x * x + z * z && -0.1f > cameraPoint.z) {
            matrix_stack::ProjectWorldPointToScreen(&anchorPosition, &screen);
            screen.x -= 320.0f;
            screen.y -= 240.0f;
            if (screen.x > -180.0f && 180.0f > screen.x && screen.y > -140.0f && 140.0f > screen.y) {
                best = screen.x * screen.x + screen.y * screen.y;
                g_searchCharacterFallback = anchorCharacter;
                currentFollowPosition = anchorPosition;
            }
        }
    }
    for (; (*names); ++names, ++positions) {
        Matrix4x4::TransformPoint(*positions, cameraPoint);
        if (!(-0.1f > cameraPoint.z))
            continue;
        matrix_stack::ProjectWorldPointToScreen(positions, &screen);
        screen.x -= 320.0f;
        screen.y -= 240.0f;
        if (screen.x > -180.0f && 180.0f > screen.x && screen.y > -140.0f && 140.0f > screen.y) {
            const float distance = screen.x * screen.x + screen.y * screen.y;
            if (best > distance) {
                best = distance;
                g_searchCharacterFallback = *names;
                currentFollowPosition = *positions;
            }
        }
    }
    return best;
}

// @verified - d
void DetectTargetFromView() {
    shen_name name = g_searchCharacterFallback;
    std::int32_t cone = -1;
    const LegacyInputSlot *const input = ctrlGetPadInputBuffer0(GetMapPadControlEntry());
    Vector3 point = currentFollowPosition;
    if (!g_searchCharacterFallback && scenarioFollowCharacter && !(input->pressedPriority & 0x200U) && plymIsSearchable() && !plymIsSearchBlocked() &&
        !HasScenarioCharacterControl() &&
        scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, scenarioFollowCharacter)) {
        g_searchCharacterFallback = scenarioFollowCharacter;
        currentFollowPosition = followPosition;
        return;
    }
    if (focusObject && camCtrlGetState() == 24 && plymIsSearchable()) {
        g_searchCharacterFallback = focusObject;
        currentFollowPosition = focusPosition;
        return;
    }
    focusObject = shen_name();
    if ((input->pressedPriority & 0x200U) || camCtrlGetState() != 24 || !plymIsSearchable()) {
        g_searchCharacterFallback = shen_name();
        lastSearchCone = -1;
        return;
    }
    if (input->pressedPriority & 0x1000U) {
        if (input->held & 0x4000U)
            cone = 1;
        else if (input->held & 0x8000U)
            cone = 7;
        else
            cone = 0;
    } else if (input->pressedPriority & 0x2000U) {
        if (input->held & 0x4000U)
            cone = 3;
        else if (input->held & 0x8000U)
            cone = 5;
        else
            cone = 4;
    } else if (input->pressedPriority & 0x4000U) {
        if (input->held & 0x1000U)
            cone = 1;
        else if (input->held & 0x2000U)
            cone = 3;
        else
            cone = 2;
    } else if (input->pressedPriority & 0x8000U) {
        if (input->held & 0x1000U)
            cone = 7;
        else if (input->held & 0x2000U)
            cone = 5;
        else
            cone = 6;
    }
    if ((!name || name == anchorCharacter || !npcGetCanInteract(name)) && lastSearchCone != cone && cone >= 0) {
        const shen_name *names = GetScenarioCharacterAdmissionNames();
        const Vector3 *positions = srchGetScenarioAdmissionPositions();
        const float *config = &searchCones[cone].x;
        Vector3 camera;
        camCtrlGetNextCamPos(&camera);
        float dx = camera.x, dz = camera.z;
        camCtrlGetTargetPosition(&camera);
        dx -= camera.x;
        dz -= camera.z;
        float x = dz * config[0] + dx * config[1], z = dz * config[1] - dx * config[0];
        float length = __builtin_sqrtf(x * x + z * z);
        x /= length;
        z /= length;
        config += 2;
        float bestDot = -1.0f, dot;
        std::int32_t bestBucket = 999, bucket;
        if (anchorCharacter && anchorCharacter != g_searchCharacterFallback) {
            dx = anchorPosition.x - currentFollowPosition.x;
            dz = anchorPosition.z - currentFollowPosition.z;
            length = __builtin_sqrtf(dx * dx + dz * dz);
            dot = (x * dx + z * dz) / length;
            if (3.0f > length && dot > *config) {
                bucket = static_cast<std::int32_t>(length * 2.5f - 0.9f);
                if (bucket < 0)
                    bucket = 0;
                if (bucket < bestBucket - 1 || (bucket <= bestBucket && dot > bestDot)) {
                    name = anchorCharacter;
                    bestDot = dot;
                    bestBucket = bucket;
                    point = anchorPosition;
                }
            }
        }
        for (; (*names); ++names, ++positions) {
            if ((*names) == g_searchCharacterFallback)
                continue;
            dx = positions->x - currentFollowPosition.x;
            dz = positions->z - currentFollowPosition.z;
            length = __builtin_sqrtf(dx * dx + dz * dz);
            dot = (x * dx + z * dz) / length;
            if (!(3.0f > length) || !(dot > *config))
                continue;
            bucket = static_cast<std::int32_t>(length * 2.5f - 0.9f);
            if (bucket < 0)
                bucket = 0;
            if (bucket < bestBucket - 1 || (bucket == bestBucket - 1 && dot > bestDot - 0.02f) || (bucket == bestBucket && dot > bestDot) ||
                (bucket == bestBucket + 1 && dot > bestDot + 0.02f)) {
                name = *names;
                bestDot = dot;
                bestBucket = bucket;
                point = *positions;
            }
        }
    }
    g_searchCharacterFallback = name;
    currentFollowPosition = point;
    lastSearchCone = cone;
}

// @verified - d
void SwitchDetectedTarget() {
    std::int32_t style;
    if (g_searchCharacterFallback == anchorCharacter)
        style = 3;
    else if (npcGetCanInteract(g_searchCharacterFallback))
        style = 5;
    else
        style = GetScenarioFollowEligibility(g_searchCharacterFallback) ? 3 : 4;
    const std::uint16_t yaw =
        static_cast<std::uint16_t>(dc::sdk::ArcTan2Fixed(currentFollowPosition.x - playerSearchPosition.x, currentFollowPosition.z - playerSearchPosition.z));
    srchSetLookTarget(0);
    srchSetLookBase(&currentFollowPosition);
    srchSetRotY(yaw);
    camCtrlSetLockedOnAngle(0.0f);
    camCtrlSwitchToLockedOn(style);
}

// @verified - d
shen_name GetSearchCharacterName() {
    shen_name name = scenarioFollowCharacter;
    if (!name) {
        name = focusObject;
        if (!name)
            name = g_searchCharacterFallback;
    }
    return name;
}

// @verified - d
void SetSearchCharacterOverride(shen_name character) {
    focusObject = character;
}

// @verified - d
shen_name GetSearchCharacterOverride() {
    return focusObject;
}

// @verified - d
void SetScenarioFollowCharacter(shen_name character) {
    scenarioFollowCharacter = character;
    scenarioFollowGraceFrames = 90;
}

// @verified - d
shen_name GetScenarioFollowCharacter() {
    return scenarioFollowCharacter;
}

} // namespace s2
} // namespace shenmue

#endif
