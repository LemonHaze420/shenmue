#include "s2/event_character_api.hpp"
#include "s2/scenario_internal.hpp"

namespace shenmue::s2 {
using namespace event_runtime_detail;

namespace event_runtime_detail {


Vector3i EventCharacterRotation(const Character &character, bool recursive) {
    if (recursive)
        return GetCharacterWorldRotation(&character);

    return {character.entity.node.rotation[0], character.entity.node.rotation[1], character.entity.node.rotation[2]};
}
} // namespace event_runtime_detail


std::uint64_t SetMotionworkFlagForCharaById(shen_name characterId, const Vector3 *scale) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, characterId);
    if (!character)
        return 0;
    if (FindMotionWork(character))
        return 1;

    SetCharacterScale(character, scale);
    std::uint32_t result{};
    std::memcpy(&result, &character->scale.z, sizeof(result));
    return static_cast<std::uint64_t>(result);
}

std::int64_t scenMotionworkUtil(shen_name characterId, std::uint32_t yaw, float angle, std::int32_t flags, const void *motionData) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, characterId);
    return ExecuteMotionworkUtility(character, yaw, angle, flags, motionData);
}

// @verified - p
std::int64_t TurnScenarioCharacterTowardPosition(
    shen_name characterId, const Vector3 *target, const std::uint16_t *customMotions, std::uint32_t *yaw, float *angularStep, std::uint32_t flags) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = GetScenarioCharacterNode(control->scenarioTask, characterId);
    Vector3 position;
    GetCharacterLocalPosition(character, &position);
    const std::uint16_t targetYaw = static_cast<std::uint16_t>(InverseFixedAtan2(target->x - position.x, target->z - position.z));
    *yaw = targetYaw;
    *angularStep = AssignCharacterMotionYaw(character, targetYaw, customMotions, flags);
    return ExecuteMotionworkUtility(character, *yaw, *angularStep, std::bit_cast<std::int32_t>(flags), customMotions);
}

// @verified - p
void EV_GetCharaPosition(shen_name name, std::uint32_t bone, Vector3 *position, std::uint32_t mode) {
    Character *const character = ctrlGetCharaByName(name);
    if (mode & EventTransformFlags::World) {
        if (bone == CHARACTER_TRANSLATION_ROOT)
            *position = GetCharacterWorldPosition(character);
        else
            GetCharacterMotionWorldPosition(character, static_cast<std::int32_t>(bone), position);
    } else if (bone == CHARACTER_TRANSLATION_ROOT) {
        GetCharacterLocalPosition(character, position);
    } else if (mode & EventTransformFlags::CharacterNodeSpace) {
        GetCharacterMotionNodePosition(character, static_cast<std::int32_t>(bone), position);
    } else {
        GetCharacterMotionLocalPosition(character, static_cast<std::int32_t>(bone), position);
    }
}

// @verified - p
void GetScenarioCharacterEulerAngles(shen_name name, Vector3i *rotation, std::uint32_t flags) {
    Character *const character = ctrlGetCharaByName(name);
    if (flags & EventTransformFlags::MotionYaw) {
        std::uint32_t yaw;
        if (flags & EventTransformFlags::World) {
            const MotionWork *const work = FindMotionWork(character);
            yaw =
                work ? static_cast<std::uint32_t>(work->runtime.entityRotation.y) : static_cast<std::uint16_t>(GetAllAngYForNode(GetCharacterNode(character)));
        } else {
            yaw = GetCharacterMotionYaw(character);
        }
        *rotation = {.x = 0, .y = static_cast<std::uint16_t>(yaw), .z = 0};
        return;
    }
    if (flags & EventTransformFlags::World)
        *rotation = GetCharacterWorldRotation(character);
    else
        GetCharacterLocalRotation(character, rotation);
    rotation->x = static_cast<std::uint16_t>(rotation->x);
    rotation->y = static_cast<std::uint16_t>(rotation->y);
    rotation->z = static_cast<std::uint16_t>(rotation->z);
}
// @verified - p
void EV_scenApplyPosOrRotToPartOf(shen_name character, std::uint16_t part, Vector3 *position, Vector3i *rotation, EventCharacterPartTransformMode mode) {
    Character *const found = FindCharacterByScenarioName(character);
    if (!found || !found->model)
        return;

    MT7Model &model = *static_cast<MT7Model *>(found->model);
    MT7Node *const node = GetChildObjectByIndex(model, model.nodes.empty() ? nullptr : &model.nodes[0], part);
    if (!node)
        return;

    if (position) {
        if (mode == EventCharacterPartTransformMode::Relative) {
            node->position.x += position->x;
            node->position.y = position->y + node->position.y;
            node->position.z = position->z + node->position.z;
        } else if (mode == EventCharacterPartTransformMode::Absolute) {
            node->position = *position;
        } else if (mode == EventCharacterPartTransformMode::Get) {
            *position = node->position;
        } else if (mode == EventCharacterPartTransformMode::Set) {
            node->scale = *position;
        }
    }
    if (rotation) {
        if (mode == EventCharacterPartTransformMode::Relative) {
            node->rotation.x = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(node->rotation.x) + static_cast<std::uint32_t>(rotation->x));
            node->rotation.y = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(node->rotation.y) + static_cast<std::uint32_t>(rotation->y));
            node->rotation.z = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(node->rotation.z) + static_cast<std::uint32_t>(rotation->z));
        } else if (mode == EventCharacterPartTransformMode::Absolute) {
            node->rotation = *rotation;
        } else if (mode == EventCharacterPartTransformMode::Get) {
            *rotation = node->rotation;
        }
    }
}
// @verified - p
void ApplyScenarioTranslationAndRotation(const Vector3 *position, const Vector3i *rotation, Vector3 *point) {
    const Vector3 input = *point;
    Matrix4x4_Push(&transformMatrix_prePass);
    if (position)
        Matrix4x4::preTranslate(position->x, position->y, position->z);
    if (rotation->z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int16_t>(rotation->z));
    if (rotation->y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int16_t>(rotation->y));
    if (rotation->x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int16_t>(rotation->x));
    Matrix4x4::TransformPoint(input, *point);
    Matrix4x4::Pop(1);
}

// @verified - p
void TransformScenarioDirection(const Vector3 *position, const Vector3i *rotation, Vector3 *direction) {
    const Vector3 input = *direction;
    Matrix4x4_Push(&transformMatrix_prePass);
    if (position)
        Matrix4x4::preTranslate(position->x, position->y, position->z);
    if (rotation->z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::int16_t>(rotation->z));
    if (rotation->y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int16_t>(rotation->y));
    if (rotation->x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::int16_t>(rotation->x));
    Matrix4x4::Transform(input, *direction);
    Matrix4x4::Pop(1);
}

// @verified - p
void GetCharacterHandRotation(Character *character, std::int32_t side, Vector3i *rotation) {
    HandParams *const hand = GetCharacterHand(character, side);
    if (hand)
        GetHandPrimaryNodeRotation(*hand, *rotation);
}

// @verified - p
MT7Node *ApplyCharacterHandRotation(Character *character, std::int32_t side, std::uint8_t flags, const Vector3i *rotation) {
    HandParams *const hand = GetCharacterHand(character, side);
    if (!hand)
        return nullptr;
    Vector3i current;
    GetHandPrimaryNodeRotation(*hand, current);
    if (flags & 1U)
        current.x = rotation->x;
    if (flags & 4U)
        current.y = rotation->y;
    if (flags & 16U)
        current.z = rotation->z;
    if (flags & 2U)
        current.x = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(current.x) + static_cast<std::uint32_t>(rotation->x));
    if (flags & 8U)
        current.y = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(current.y) + static_cast<std::uint32_t>(rotation->y));
    if (flags & 32U)
        current.z = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(current.z) + static_cast<std::uint32_t>(rotation->z));
    return SetHandPrimaryNodeRotation(*hand, current);
}

// @verified - p
void GetCharacterHandEndpoint(Character *character, std::int32_t side, std::int32_t endpoint, Vector3 *position) {
    HandParams *const hand = GetCharacterHand(character, side);
    if (hand)
        GetHandEndpointPosition(*hand, endpoint, *position);
}

// @verified - p
std::int16_t ConfigureCharacterHandPoseSequence(Character *character, std::int32_t side, std::int16_t pose, std::int16_t first, std::int16_t last,
    std::int16_t start, std::uint8_t flags, float step, std::int16_t ticks) {
    HandParams *const hand = GetCharacterHand(character, side);
    return hand ? ConfigureHandPoseSequence(*hand, pose, first, last, start, flags, step, ticks) : 0;
}

// @verified - p
std::uint64_t IsCharacterHandPoseInactive(Character *character, std::int32_t side) {
    HandParams *const hand = GetCharacterHand(character, side);
    return hand ? IsHandPoseInactive(*hand) : HAND_MISSING_RESULT;
}

// @verified - p
std::uint8_t SetCharacterHandPoseFlags(Character *character, std::int32_t side, std::int16_t enabled, std::uint8_t flags) {
    HandParams *const hand = GetCharacterHand(character, side);
    return hand ? SetHandPoseFlags(*hand, enabled, flags) : 0;
}

// @verified - p
void SetCharacterHandPoseStep(Character *character, std::int32_t side, float step) {
    HandParams *const hand = GetCharacterHand(character, side);
    if (hand)
        SetHandPoseStep(*hand, step);
}

// @verified - p
std::uint64_t GetCharacterHandPoseAnimationResult(Character *character, std::int32_t side) {
    HandParams *const hand = GetCharacterHand(character, side);
    return hand ? GetHandPoseAnimationResult(*hand) : HAND_MISSING_RESULT;
}

// @verified - p
std::uint64_t SetCharacterHandSecondaryPoseWork(Character *character, std::int32_t side, void *work) {
    HandParams *const hand = GetCharacterHand(character, side);
    if (!hand)
        return 0;
    const bool changed = hand->secondaryPoseWork != work;
    SetHandSecondaryPoseWork(*hand, work);
    return changed ? HAND_RESET_BOUNDS_RESULT : reinterpret_cast<std::uintptr_t>(hand);
}

// @verified - p
std::uint64_t SetScenarioCharacterFlag40(shen_name name, std::int32_t enabled) {
    Character *const character = ctrlGetCharaByName(name);
    const std::uint32_t previous = charaCheckFlags(character);
    if (enabled >= 0)
        charaToggleFlag(character, enabled);
    return previous;
}

// @verified - p
void SetScenarioCharacterDrawControl(shen_name name, std::int32_t mode, std::int32_t argument) {
    Character *const character = name.name ? ctrlGetCharaByName(name) : nullptr;
    switch (mode) {
    case 0:
        SetCharacterDrawControlEnabled(character, false);
        break;
    case 1:
        SetCharacterDrawControlEnabled(character, true);
        break;
    case 2:
        SetCharacterDrawControlEnabled(character, argument > 0);
        break;
    default:
        break;
    }
}

// @verified - p
void SetScenarioCharacterType(shen_name name, std::int32_t type) {
    SetCharacterType(ctrlGetCharaByName(name), type);
}

// @verified - p
bool IsScenarioCharacterDisplayEnabled(shen_name name) {
    return IsOfThisVisibilityType(ctrlGetCharaByName(name), 3);
}

// @verified - p
bool DoesScenarioCharacterExist(shen_name name) {
    return ctrlGetCharaByName(name) != nullptr;
}

// @verified - p
void SetScenarioCharacterKillMode(shen_name name, std::uint32_t mode) {
    ctrlSetKillAndIncrementCounter(ctrlGetCharaByName(name), mode);
}

// @verified - p
void SetScenarioCharacterTransparency(shen_name name, float alpha) {
    if (Character *const character = ctrlGetCharaByName(name))
        character->alpha = alpha;
}

// @verified - p
ReflectorBoardParams *SetScenarioReflectorBoardValues(shen_name name, const Vector4 *values) {
    return SetReflectorBoardValues(ctrlGetCharaByName(name), *values);
}

// @verified - p
ObjectXFormsParams *ResetScenarioCharacterAttachment(shen_name name) {
    return ResetCharaFixo(ctrlGetCharaByName(name));
}

// @verified - p
std::int64_t SetScenarioCharacterInverseChildPlacement(shen_name character, std::uint16_t childId) {
    return SetFixoInverseChildPlacement(ctrlGetCharaByName(character), childId);
}

// @verified - p
std::uint64_t AttachScenarioCharacterPreservingPlacement(
    shen_name target, shen_name source, std::int32_t bone, const Vector3 *position, const Vector3i *rotation) {
    Character *const targetCharacter = ctrlGetCharaByName(target);
    Character *const sourceCharacter = ctrlGetCharaByName(source);
    AttachFixoPreservingPlacement(targetCharacter, sourceCharacter, bone);
    SetFixoLocalPosition(targetCharacter, position);
    return SetFixoLocalRotation(targetCharacter, rotation);
}

// @verified - p
void AttachScenarioCharacter(shen_name target, shen_name source, std::int32_t bone, const Vector3 *position, const Vector3i *rotation) {
    Character *const targetCharacter = ctrlGetCharaByName(target);
    Character *const sourceCharacter = ctrlGetCharaByName(source);
    SetFixoDataForMotionWork(targetCharacter, sourceCharacter, bone, *position, {rotation->x, rotation->y, rotation->z});
}

// @verified - p
bool TestCharaPositionWithinRange(shen_name name, const Vector3 *first, const Vector3 *second, shen_name relativeTo) {
    Character *const character = ctrlGetCharaByName(name);
    Vector3 relative{};
    if (relativeTo.name)
        GetCharacterLocalPosition(ctrlGetCharaByName(relativeTo), &relative);
    float minimumX = relative.x + first->x;
    float maximumX = relative.x + second->x;
    float minimumZ = relative.z + first->z;
    float maximumZ = relative.z + second->z;
    if (minimumX > maximumX) {
        minimumX = relative.x + second->x;
        maximumX = relative.x + first->x;
    }
    if (minimumZ > maximumZ) {
        minimumZ = relative.z + second->z;
        maximumZ = relative.z + first->z;
    }
    Vector3 position;
    GetCharacterLocalPosition(character, &position);
    return position.x >= minimumX && maximumX >= position.x && position.z >= minimumZ && maximumZ >= position.z;
}

// @verified - p
void EV_AT_SetPos(shen_name character, std::uint32_t flags, const Vector3 &requested) {
    Character *const found = ctrlGetCharaByName(character);
    if (!found)
        return;
    // EV_AT_SetPosition preserves unselected axes and clears CHAR+0x30.
    const bool recursive = (flags & EventTransformFlags::World) != 0;
    Vector3 position = recursive ? GetCharacterWorldPosition(found) : found->entity.node.position;
    const bool relative = (flags & EventTransformFlags::Relative) != 0;
    if (flags & EventTransformFlags::X) {
        position.x = relative ? position.x + requested.x : requested.x;
    }
    if (flags & EventTransformFlags::Y) {
        position.y = relative ? position.y + requested.y : requested.y;
    }
    if (flags & EventTransformFlags::Z) {
        position.z = relative ? position.z + requested.z : requested.z;
    }
    if (recursive) {
        shenApplyTranslationToAll(&found->entity.node, position);
    } else {
        found->entity.node.position = position;
    }
    found->position = {};
}

std::int64_t EV_AT_SetAngY(shen_name character, std::uint32_t flags, const Vector3i &requested, const std::uint16_t *customMotions) {
    Character *const found = FindCharacterByScenarioName(character);
    if (!found)
        return 0;
    // EV_AT_SetAngY delegates every flag to SetAngY.
    const bool recursive = (flags & EventTransformFlags::World) != 0;
    Vector3i rotation = EventCharacterRotation(*found, recursive);
    const bool relative = (flags & EventTransformFlags::Relative) != 0;
    const auto assign = [relative](std::int32_t previous, std::int32_t value) {
        return relative ? static_cast<std::int32_t>(static_cast<std::uint32_t>(previous) + static_cast<std::uint32_t>(value)) : value;
    };
    if (flags & EventTransformFlags::X)
        rotation.x = assign(rotation.x, requested.x);
    if (flags & EventTransformFlags::Y)
        rotation.y = assign(rotation.y, requested.y);
    if (flags & EventTransformFlags::Z)
        rotation.z = assign(rotation.z, requested.z);
    if (flags & EventTransformFlags::MotionTurnMask) {
        AssignCharacterMotionYaw(found, rotation.y, customMotions, flags);
    } else {
        if (recursive) {
            SetCharacterWorldRotation(found, rotation);
        } else {
            found->entity.node.rotation[0] = rotation.x;
            found->entity.node.rotation[1] = rotation.y;
            found->entity.node.rotation[2] = rotation.z;
        }
        if (MotionWork *const work = FindMotionWork(found)) {
            SetMotionEntityRotation(work, rotation);
        }
    }
    return (flags & EventTransformFlags::MotionTurnMask) ? static_cast<std::uint32_t>(requested.y) : 0;
}

std::int32_t EV_ActivatePartyCompanion(shen_name character) {
    return ActivatePartyCompanion(character);
}

std::int32_t EV_PTYCUtils(shen_name character, std::int32_t command, std::int32_t argument) {
    return PTYCUtils(character, command, argument);
}

void EV_SetGlobalShadowLightingEnabled(std::int32_t enabled) {
    SetGlobalShadowLightingEnabled(enabled);
}

void EV_AUVW_SetLayerState(std::int32_t layer, std::int32_t state) {
    if (layer >= 0 && layer < static_cast<std::int32_t>(eventAuvwRuntime.layerStates.size())) {
        eventAuvwRuntime.layerStates[static_cast<std::size_t>(layer)] = state;
    }
}

void EV_AUVW_Initialize(std::int32_t state) {
    eventAuvwRuntime = {};
    eventAuvwRuntime.initializedState = state;
}

void EV_AUVW_Update() {
    ++eventAuvwRuntime.updateCount;
}

std::int32_t EV_cuffsAttach(shen_name firstCharacter, std::int32_t firstHand, shen_name secondCharacter, std::int32_t secondHand, float maximumDistance) {
    return Handcuffs_Attach(firstCharacter, firstHand, secondCharacter, secondHand, maximumDistance);
}

void EV_cuffsSetFlags() {
    cuffsSetFlags();
}

void EV_MAZ_fishCtrl() {
    MAZ_fishCtrl();
}

// @verified - p
Character *ctrlGetCharaByName(shen_name character) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return FindScenarioCharacter(control->scenarioTask, character);
}

// @verified - p
bool CheckIsArgEquZero(std::int32_t value) {
    return value == 0;
}

void EV_SetFish(const Vector3 *minimum, const Vector3 *maximum, std::int32_t count, std::int32_t seed, Character *character) {
    SetFish(minimum, maximum, count, seed, character);
}
std::uint32_t EV_GetCurrentRefCount() {
    return mazeGetCurrentRefCount();
}
void EV_SetCurrentRefCount(std::uint32_t count) {
    mazeSetCurrentRefCount(count);
}
namespace event_runtime_detail {


constexpr std::array<shen_name, 9> openingTemporaryCharacters{SHEN_NAME(FT1_), SHEN_NAME(FT2_), SHEN_NAME(FT3_), SHEN_NAME(FT4_), SHEN_NAME(FT5_),
    SHEN_NAME(FT6_), SHEN_NAME(FT7_), SHEN_NAME(FT8_), shen_name{0xffffffffU}};
} // namespace event_runtime_detail

void EV_InitializeOpeningTemporaryCharacters(HLib::Task *startupTask) {
    AssignAreaNpcLoadContext(openingTemporaryCharacters.data(), startupTask);
    SetAreaNpcLowQualityHands(false);
    g_usingCustomAuthorCharacterList = 0;
}

void EV_CleanupOpeningTemporaryCharacters() {
    for (const shen_name name : openingTemporaryCharacters) {
        if (name.name == 0xffffffffU)
            break;
        if (FindCharacterByScenarioName(name)) {
            EV_npcLockUnlock(name);
            EV_npcFree(name);
        }
    }
}

void EV_SetModelDataDefaultVertexShader() {
    SetMapModelDataDefaultVertexShader();
}
void EV_SetEntityVertexShaderType(std::int32_t modelSlot, std::int32_t value) {
    SetMapModelVertexShaderType(modelSlot, value);
}

std::int32_t EV_SetObjectFlagByIndex(std::int32_t index, std::int32_t value) {
    return SetMapModelSlotEnabled(index, value);
}

void EV_AssignLightParams(std::int32_t value, std::int32_t modelSlot) {
    SetMapModelWeatherLightingOverride(modelSlot, value);
}

void EV_lightLoad(shen_name light) {
    LGHT_Parser_Internal(light.name);
}

void EV_ReinitLights(std::int32_t mode) {
    ReinitializeLights(mode);
}
void EV_SetLightFlagForIndex(std::int32_t index, std::int32_t value) {
    if (index >= 0)
        SetLightFlagForIndex(static_cast<std::uint32_t>(index), value);
}
void EV_SetIndexForLightEntry(std::int32_t index, std::int32_t value) {
    if (index >= 0 && value >= 0) {
        SetLightEntryIndex(static_cast<std::uint32_t>(index), static_cast<std::uint32_t>(value));
    }
}
// @verified - p
void EV_EnsureCharacterDraw(shen_name name) {
    if (Character *const character = ctrlGetCharaByName(name)) {
        HLib::Task *const task = GetCharacterHandlerTask(character);
        charaSetFlags(character, 1);
        HLib::ResolveDeferredCallback(task);
    }
}

// @verified - p
void EV_EnableCharacterDisplay(shen_name name) {
    EV_SetCharDisplay(ctrlGetCharaByName(name), CVM_DisplayEnabled, 1);
}

// @verified - p
void EV_ToggleCharacterFlag(shen_name name) {
    if (Character *const character = ctrlGetCharaByName(name)) {
        HLib::Task *const task = GetCharacterHandlerTask(character);
        charaSetFlags(character, 0);
        HLib::SetupTimedTask(task, 0);
    }
}

// @verified - p
void EV_DisableCharacterDisplay(shen_name name) {
    EV_SetCharDisplay(ctrlGetCharaByName(name), CVM_DisplayEnabled, 0);
}

// @verified - p
void EV_SetCharacterAlphaEnabled(shen_name name, bool enabled) {
    if (Character *const character = ctrlGetCharaByName(name)) {
        cpamSetFlag(character, enabled ? 1 : 0);
    }
}

} // namespace shenmue::s2
