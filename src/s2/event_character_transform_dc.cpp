#include "s2/event_character_transform_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_placement_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/object_xforms.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/math.hpp"
#include "platform/dc/matrix.hpp"
#include <cstring>
#include "s2/character_yaw_dc.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "s2/character_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_light_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "platform/dc/sdk.hpp"
namespace shenmue {
namespace dc {
namespace sdk {
void SetMirroredModelDrawMode(std::int32_t enabled);
}
} // namespace dc
} // namespace shenmue
#include "s2/event_character_state_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/event_matrix_dc.hpp"

namespace shenmue {
namespace s2 {

std::int32_t RotateCharacter(Character *character, std::uint32_t flags, const Vector3i *rotation, const std::uint16_t *customMotions);
float AssignCharacterMotionYaw(Character *character, std::int32_t yaw, const std::uint16_t *customMotions, std::uint32_t flags);

void GetCharacterLocalPosition(const Character *character, Vector3 *position);
void SetCharacterLocalPosition(Character *character, const Vector3 *position);
void charaGetMotNodePos(Character *character, std::int32_t bone, Vector3 *position);
void charaGetMotLocalPos(Character *character, std::int32_t bone, Vector3 *position);
bool SetCharacterMotionTarget(Character *character, std::int32_t selector, const Vector3 *position);
bool LatchCharacterMotionTargetInParentSpace(Character *character, std::int32_t selector, const Vector3 *position);
std::int32_t charGetMotionWorkYaw(const Character *character);
std::int32_t GetCharacterMotionWorldYaw(const Character *character);
void mtwkSetEntityRotation(MotionWork *work, const Vector3i *rotation);
std::int32_t GetCharacterControlSlotIndex(const CharacterNode *node);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
Character *ctrlGetCharaByName(shen_name name) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return scenFindChara(control->scenarioTask, name);
}
#endif

// @verified - d
void EV_AT_SetPos(shen_name name, std::uint32_t flags, const Vector3 &position) {
    Vector3 current;
    current.x = current.y = current.z = 0.0f;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (flags & EventTransformFlags::World)
        GetCharacterWorldPosition(character, &current);
    else
        GetCharacterLocalPosition(character, &current);
    if (flags & EventTransformFlags::Relative) {
        if (flags & EventTransformFlags::X)
            current.x += position.x;
        if (flags & EventTransformFlags::Y)
            current.y += position.y;
        if (flags & EventTransformFlags::Z)
            current.z += position.z;
    } else {
        if (flags & EventTransformFlags::X)
            current.x = position.x;
        if (flags & EventTransformFlags::Y)
            current.y = position.y;
        if (flags & EventTransformFlags::Z)
            current.z = position.z;
    }
    if (flags & EventTransformFlags::World)
        SetCharacterWorldPosition(character, &current);
    else
        SetCharacterLocalPosition(character, &current);
    current.x = current.y = current.z = 0.0f;
    SetCharacterPositionDelta(character, &current);
}

// @verified - d
bool SetScenarioCharacterMotionTarget(shen_name name, std::uint32_t flags, std::int32_t selector, const Vector3 *position) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    bool result;
    if (flags & EventTransformFlags::World)
        result = charaLatchBoneTarget(character, selector, position);
    else if (flags & EventTransformFlags::CharacterNodeSpace)
        result = SetCharacterMotionTarget(character, selector, position);
    else
        result = LatchCharacterMotionTargetInParentSpace(character, selector, position);
    return result;
}

// @verified - d
void EV_GetCharaPosition(shen_name name, std::uint32_t bone, Vector3 *position, std::uint32_t flags) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (flags & EventTransformFlags::World) {
        if (bone == 0xffffffffU)
            GetCharacterWorldPosition(character, position);
        else if (flags & 0x00800000U)
            charaGetMotWorldPos(character, static_cast<std::int32_t>(bone), position);
        else
            charaGetMotWorldPos(character, static_cast<std::int32_t>(bone), position);
    } else if (flags & EventTransformFlags::CharacterNodeSpace) {
        if (bone == 0xffffffffU)
            GetCharacterLocalPosition(character, position);
        else if (flags & 0x00800000U)
            charaGetMotNodePos(character, static_cast<std::int32_t>(bone), position);
        else
            charaGetMotNodePos(character, static_cast<std::int32_t>(bone), position);
    } else if (bone == 0xffffffffU) {
        GetCharacterLocalPosition(character, position);
    } else if (flags & 0x00800000U) {
        charaGetMotLocalPos(character, static_cast<std::int32_t>(bone), position);
    } else {
        charaGetMotLocalPos(character, static_cast<std::int32_t>(bone), position);
    }
}

// @verified - d
ObjectXFormsParams *AttachScenarioCharacterPreservingPlacement(
    shen_name target, shen_name source, std::int32_t bone, const Vector3 *position, const Vector3i *rotation) {
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const targetCharacter = scenFindChara(control->scenarioTask, target);
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const sourceCharacter = scenFindChara(control->scenarioTask, source);
    AttachFixoPreservingPlacement(targetCharacter, sourceCharacter, bone);
    SetFixoLocalPosition(targetCharacter, position);
    return SetFixoLocalRotation(targetCharacter, rotation);
}

// @verified - d
void AttachScenarioCharacter(shen_name target, shen_name source, std::int32_t bone, const Vector3 *position, const Vector3i *rotation) {
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const targetCharacter = scenFindChara(control->scenarioTask, target);
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const sourceCharacter = scenFindChara(control->scenarioTask, source);
    SetFixoDataForMotionWork(targetCharacter, sourceCharacter, bone, *position, reinterpret_cast<const std::int32_t (&)[3]>(*rotation));
}

// @verified - d
ObjectXFormsParams *ResetScenarioCharacterAttachment(shen_name name) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return ResetCharaFixo(scenFindChara(control->scenarioTask, name));
}

// @verified - d
void SetScenarioCharacterInversePlacement(shen_name name, const Vector3 *position, const Vector3i *rotation) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetFixoInversePositionRotation(scenFindChara(control->scenarioTask, name), position, rotation);
}

// @verified - d
std::int32_t SetScenarioCharacterInverseChildPlacement(shen_name name, std::int16_t childId) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return static_cast<std::int32_t>(SetFixoInverseChildPlacement(scenFindChara(control->scenarioTask, name), static_cast<std::uint16_t>(childId)));
}

// @verified - d
std::int32_t EV_AT_SetAngY(shen_name name, std::uint32_t flags, const Vector3i &rotation, const std::uint16_t *customMotions) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return RotateCharacter(scenFindChara(control->scenarioTask, name), flags, &rotation, customMotions);
}

// @verified - d
std::int32_t RotateCharacter(Character *character, std::uint32_t flags, const Vector3i *rotation, const std::uint16_t *customMotions) {
    Vector3i current = {};
    if (flags & EventTransformFlags::World)
        GetCharacterWorldRotation(character, &current);
    else
        GetCharacterLocalRotation(character, &current);
    if (flags & EventTransformFlags::Relative) {
        if (flags & EventTransformFlags::X)
            current.x = static_cast<std::int32_t>(static_cast<std::uint32_t>(current.x) + static_cast<std::uint32_t>(rotation->x));
        if (flags & EventTransformFlags::Y)
            current.y = static_cast<std::int32_t>(static_cast<std::uint32_t>(current.y) + static_cast<std::uint32_t>(rotation->y));
        if (flags & EventTransformFlags::Z)
            current.z = static_cast<std::int32_t>(static_cast<std::uint32_t>(current.z) + static_cast<std::uint32_t>(rotation->z));
    } else {
        if (flags & EventTransformFlags::X)
            current.x = rotation->x;
        if (flags & EventTransformFlags::Y)
            current.y = rotation->y;
        if (flags & EventTransformFlags::Z)
            current.z = rotation->z;
    }
    if (flags & EventTransformFlags::MotionTurnMask) {
        AssignCharacterMotionYaw(character, current.y, customMotions, flags);
        return rotation->y;
    }
    if (flags & EventTransformFlags::World) {
        charaSetWorldRot(character, current);
        if (HasCharacterMotionWork(character))
            mtwkSetEntityRotation(charFindMotionwork(character), &current);
    } else {
        SetCharacterLocalRotation(character, &current);
        if (HasCharacterMotionWork(character))
            mtwkSetEntityRotation(charFindMotionwork(character), &current);
    }
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool charRequestMotion(Character *character, std::int32_t motion, MotionRegion region);
std::uint32_t charSetMotionRegionFlag(Character *character, std::uint32_t flag, std::int32_t operation, MotionRegion region);

std::int32_t ExecuteMotionworkUtility(Character *character, std::uint32_t yaw, float angle, std::uint32_t flags, const void *motionData) {
    const std::uint32_t currentYaw = static_cast<std::uint32_t>(shenGetRotYFromNode(character));
    angle *= 8192.0f;
    angle /= 45.0f;
    const std::uint32_t angleMask = 0xffffU;
    const std::uint32_t delta = static_cast<std::uint32_t>(static_cast<std::int32_t>(angle)) & angleMask;
    shenSetRotY(character, static_cast<std::int32_t>((currentYaw + delta) & angleMask));
    if (!charSetMotionRegionFlag(character, 65, 1, static_cast<MotionRegion>(0)))
        return 0;
    shenSetRotY(character, static_cast<std::int32_t>(yaw));
    if (flags & EventTransformFlags::CustomIdleMotion) {
        const volatile std::uint16_t *motions = static_cast<const std::uint16_t *>(motionData);
        if (!motions)
            motions = scenarioTurnAnimations;
        charRequestMotion(character, motions[7], static_cast<MotionRegion>(0));
    } else {
        charRequestMotion(character, scenarioTurnAnimations[7], static_cast<MotionRegion>(0));
    }
    return 0x10000;
}
#endif

float AssignCharacterMotionYaw(Character *character, std::int32_t yaw, const std::uint16_t *customMotions, std::uint32_t flags) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const std::uint16_t rounded = static_cast<std::uint16_t>(static_cast<std::uint32_t>(yaw) - charGetMotionWorkYaw(character) + 0x1000U);
    const std::uint32_t direction = (static_cast<std::uint32_t>(rounded) >> 13) & 7U;
    std::int32_t difference;
    float duration;
    if (!direction) {
        if (flags & EventTransformFlags::CustomIdleMotion) {
            if (customMotions) {
                duration = static_cast<float>(GetMotionFrameCount(customMotions[7]));
            } else {
                const volatile std::uint16_t *const defaultMotions = scenarioTurnAnimations;
                duration = static_cast<float>(GetMotionFrameCount(defaultMotions[7]));
            }
        } else {
            duration = static_cast<float>(GetMotionFrameCount(scenarioTurnAnimations[7]));
        }
        difference = static_cast<std::int32_t>(static_cast<std::uint32_t>(yaw) - charGetMotionWorkYaw(character));
    } else {
        if (!customMotions) {
            MotionWork *const work = charFindMotionwork(character);
            const std::uint16_t animation = scenarioTurnAnimations[direction - 1];
            mtwkRequestMotion(work, animation, static_cast<MotionRegion>(0));
            duration = static_cast<float>(GetMotionFrameCount(animation));
        } else {
            MotionWork *const work = charFindMotionwork(character);
            const std::uint16_t *const animation = customMotions + direction - 1;
            mtwkRequestMotion(work, *animation, static_cast<MotionRegion>(0));
            duration = static_cast<float>(GetMotionFrameCount(*animation));
        }
        const std::int32_t currentYaw = charGetMotionWorkYaw(character);
        const std::uint16_t base = static_cast<std::uint16_t>(static_cast<std::uint32_t>(scenarioTurnYawOffsets[direction - 1]) + currentYaw);
        yaw = static_cast<std::int32_t>(static_cast<std::uint32_t>(yaw) - base);
        difference = yaw;
    }
    float degrees = static_cast<float>(difference) / 182.04445f;
    if (!(degrees < 180.0f))
        degrees -= 360.0f;
    else if (!(degrees > -180.0f))
        degrees += 360.0f;
    return degrees / duration;
#else
    const std::uint32_t direction = ((static_cast<std::uint32_t>(yaw) - static_cast<std::uint32_t>(charGetMotionWorkYaw(character)) + 0x1000U) & 0xffffU) >> 13;
    std::int32_t difference;
    float duration;
    if (!direction) {
        const std::uint16_t animation = (flags & EventTransformFlags::CustomIdleMotion) && customMotions ? customMotions[7] : scenarioTurnAnimations[7];
        duration = static_cast<float>(GetMotionFrameCount(animation));
        difference = static_cast<std::int32_t>(static_cast<std::uint32_t>(yaw) - static_cast<std::uint32_t>(charGetMotionWorkYaw(character)));
    } else {
        MotionWork *const work = charFindMotionwork(character);
        const std::uint16_t animation = customMotions ? customMotions[direction - 1] : scenarioTurnAnimations[direction - 1];
        mtwkRequestMotion(work, animation, static_cast<MotionRegion>(0));
        duration = static_cast<float>(GetMotionFrameCount(customMotions ? customMotions[direction - 1] : animation));
        const std::uint32_t base = (static_cast<std::uint32_t>(charGetMotionWorkYaw(character)) + scenarioTurnYawOffsets[direction - 1]) & 0xffffU;
        difference = static_cast<std::int32_t>(static_cast<std::uint32_t>(yaw) - base);
    }
    float degrees = static_cast<float>(difference) / 182.04445f;
    if (!(degrees < 180.0f))
        degrees -= 360.0f;
    else if (!(degrees > -180.0f))
        degrees += 360.0f;
    return degrees / duration;
#endif
}

// @verified - d
void GetScenarioCharacterEulerAngles(shen_name name, Vector3i *rotation, std::uint32_t flags) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t zero;
    std::int32_t (*readYaw)(const Character *);
    void (*readRotation)(const Character *, Vector3i *);
    if (flags & EventTransformFlags::World) {
        if (flags & EventTransformFlags::MotionYaw) {
            zero = 0;
            readYaw = GetCharacterMotionWorldYaw;
            rotation->x = zero;
            goto readMotionYaw;
        }
        readRotation = GetCharacterWorldRotation;
        goto readEulerAngles;
    }
    if (flags & EventTransformFlags::MotionYaw) {
        zero = 0;
        rotation->x = zero;
        readYaw = charGetMotionWorkYaw;
    readMotionYaw:
        rotation->y = static_cast<std::uint16_t>(readYaw(character));
        rotation->z = zero;
        return;
    }
    readRotation = GetCharacterLocalRotation;
readEulerAngles:
    readRotation(character, rotation);
    rotation->x &= 0xffff;
    rotation->y &= 0xffff;
    rotation->z &= 0xffff;
#else
    if (flags & EventTransformFlags::MotionYaw) {
        rotation->x = 0;
        rotation->y = static_cast<std::uint16_t>(flags & EventTransformFlags::World ? GetCharacterMotionWorldYaw(character) : charGetMotionWorkYaw(character));
        rotation->z = 0;
    } else {
        if (flags & EventTransformFlags::World)
            GetCharacterWorldRotation(character, rotation);
        else
            GetCharacterLocalRotation(character, rotation);
        rotation->x &= 0xffff;
        rotation->y &= 0xffff;
        rotation->z &= 0xffff;
    }
#endif
}

// @verified - d
void AT_SetLocalBaseChara(shen_name name, shen_name base) {
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    CharacterNode *const node = GetCharacterNode(character);
    Vector3 position;
    Vector3i rotation;
    GetCharacterWorldPosition(character, &position);
    GetCharacterWorldRotation(character, &rotation);
    CharacterNode *parent;
    if (base == SCENARIO_CHARACTER_NO_PARENT) {
        parent = 0;
    } else if (base <= 31U) {
        parent = GetCharacterControlSlot(static_cast<std::int32_t>(base));
    } else {
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        parent = GetCharacterNode(scenFindChara(control->scenarioTask, base));
    }
    node->parent = parent;
    SetCharacterWorldPosition(character, &position);
    charaSetWorldRot(character, rotation);
}

// @verified - d
shen_name EV_SCEN_FindChar_Wrapper(shen_name name) {
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *character = scenFindChara(control->scenarioTask, name);
    CharacterNode *const parent = GetCharacterNode(character)->parent;
    if (!parent)
        return SCENARIO_CHARACTER_NO_PARENT;
    const std::int32_t index = GetCharacterControlSlotIndex(parent);
    if (index >= 0)
        return shen_name(static_cast<std::uint32_t>(index));
    character = GetCharacterFromNode(parent);
    if (!character)
        return SCENARIO_CHARACTER_NO_PARENT;
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return scenGetCharaName(control->scenarioTask, character);
}

// @verified - d
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void EV_scenApplyPosOrRotToPartOf(shen_name name, std::uint32_t part, Vector3 *position, Vector3i *rotation, EventCharacterPartTransformMode mode) {
#else
void EV_scenApplyPosOrRotToPartOf(shen_name name, std::uint16_t part, Vector3 *position, Vector3i *rotation, EventCharacterPartTransformMode mode) {
#endif
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (!character)
        return;
    MT7Node *const node = FindMT7Node(character->model, static_cast<std::int16_t>(part));
    if (!node)
        return;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (position) {
        switch (mode) {
        case EventCharacterPartTransformMode_Absolute:
            node->position.x = position->x;
            node->position.y = position->y;
            node->position.z = position->z;
            break;
        case EventCharacterPartTransformMode_Relative:
            node->position.x += position->x;
            node->position.y += position->y;
            node->position.z += position->z;
            break;
        case EventCharacterPartTransformMode_Get:
            position->x = node->position.x;
            position->y = node->position.y;
            position->z = node->position.z;
            break;
        case EventCharacterPartTransformMode_Set:
            node->scale.x = position->x;
            node->scale.y = position->y;
            node->scale.z = position->z;
            break;
        }
    }
    if (rotation) {
        switch (static_cast<std::uint32_t>(mode)) {
        case EventCharacterPartTransformMode_Absolute:
            node->rotation.x = rotation->x;
            node->rotation.y = rotation->y;
            node->rotation.z = rotation->z;
            break;
        case EventCharacterPartTransformMode_Relative:
            node->rotation.x = static_cast<std::int32_t>(static_cast<std::uint32_t>(node->rotation.x) + static_cast<std::uint32_t>(rotation->x));
            node->rotation.y = static_cast<std::int32_t>(static_cast<std::uint32_t>(node->rotation.y) + static_cast<std::uint32_t>(rotation->y));
            node->rotation.z = static_cast<std::int32_t>(static_cast<std::uint32_t>(node->rotation.z) + static_cast<std::uint32_t>(rotation->z));
            break;
        case EventCharacterPartTransformMode_Get:
            rotation->x = node->rotation.x;
            rotation->y = node->rotation.y;
            rotation->z = node->rotation.z;
            break;
        }
    }
#else
    if (position) {
        if (mode == EventCharacterPartTransformMode_Absolute)
            node->position = *position;
        else if (mode == EventCharacterPartTransformMode_Relative) {
            node->position.x += position->x;
            node->position.y += position->y;
            node->position.z += position->z;
        } else if (mode == EventCharacterPartTransformMode_Get)
            *position = node->position;
        else if (mode == EventCharacterPartTransformMode_Set)
            node->scale = *position;
    }
    if (rotation) {
        if (mode == EventCharacterPartTransformMode_Absolute)
            node->rotation = *rotation;
        else if (mode == EventCharacterPartTransformMode_Relative) {
            node->rotation.x = static_cast<std::int32_t>(static_cast<std::uint32_t>(node->rotation.x) + static_cast<std::uint32_t>(rotation->x));
            node->rotation.y = static_cast<std::int32_t>(static_cast<std::uint32_t>(node->rotation.y) + static_cast<std::uint32_t>(rotation->y));
            node->rotation.z = static_cast<std::int32_t>(static_cast<std::uint32_t>(node->rotation.z) + static_cast<std::uint32_t>(rotation->z));
        } else if (mode == EventCharacterPartTransformMode_Get)
            *rotation = node->rotation;
    }
#endif
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t TurnScenarioCharacterTowardPosition(
    shen_name name, const Vector3 *target, const std::uint16_t *customMotions, std::uint32_t *yaw, float *angularStep, std::uint32_t flags) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    Vector3 position;
    GetCharacterLocalPosition(character, &position);
    position.x = target->x - position.x;
    position.z = target->z - position.z;
    *yaw = static_cast<std::uint16_t>(InverseFixedAtan2(position.x, position.z));
    *angularStep = AssignCharacterMotionYaw(character, *yaw, customMotions, flags);
    return ExecuteMotionworkUtility(character, *yaw, *angularStep, flags, customMotions);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t scenMotionworkUtil(shen_name name, std::uint32_t yaw, float angle, std::uint32_t flags, const void *motionData) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return ExecuteMotionworkUtility(scenFindChara(control->scenarioTask, name), yaw, angle, flags, motionData);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawPIN(shen_name name, std::int32_t, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale) {
    const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const pin = scenFindChara(control->scenarioTask, SHEN_NAME(PIN_));
    matrix_stack::Matrix4x4_Push(0);
    ApplyRecursiveCharacterNodeTransforms(GetCharacterNode(pin));
    if (position)
        matrix_stack::TranslateVector(position);
    if (rotation) {
        if (rotation->z)
            Matrix4x4::RotateAroundZ_Wrapper(rotation->z);
        if (rotation->y)
            Matrix4x4::RotateAroundY_Wrapper(rotation->y);
        if (rotation->x)
            Matrix4x4::RotateAroundX_Wrapper(rotation->x);
    }
    if (scale)
        Matrix4x4::Scale(scale->x, scale->y, scale->z);
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    charaApplyScale(scenFindChara(control->scenarioTask, name)->model, 0.0f);
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ApplyTranslationRotationScaleXYZToCharaByID(shen_name name, std::int32_t, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale) {
    const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const dummy = scenFindChara(control->scenarioTask, SHEN_NAME(dumy));
    matrix_stack::Matrix4x4_Push(0);
    ApplyRecursiveCharacterNodeTransforms(GetCharacterNode(dummy));
    if (position)
        matrix_stack::TranslateVector(position);
    if (rotation) {
        if (rotation->z)
            Matrix4x4::RotateAroundZ_Wrapper(rotation->z);
        if (rotation->y)
            Matrix4x4::RotateAroundY_Wrapper(rotation->y);
        if (rotation->x)
            Matrix4x4::RotateAroundX_Wrapper(rotation->x);
    }
    if (scale)
        Matrix4x4::Scale(scale->x, scale->y, scale->z);
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    charaApplyScale(scenFindChara(control->scenarioTask, name)->model, 0.0f);
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawScenarioModelChild(shen_name name, std::int32_t ordinal, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale) {
    const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const dummy = scenFindChara(control->scenarioTask, SHEN_NAME(dumy));
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    MT7Node *node = scenFindChara(control->scenarioTask, name)->model->child->child;
    for (std::int32_t index = 0; index < ordinal; ++index)
        node = node->sibling;
    matrix_stack::Matrix4x4_Push(0);
    ApplyRecursiveCharacterNodeTransforms(GetCharacterNode(dummy));
    if (position)
        matrix_stack::TranslateVector(position);
    if (rotation) {
        if (rotation->z)
            Matrix4x4::RotateAroundZ_Wrapper(rotation->z);
        if (rotation->y)
            Matrix4x4::RotateAroundY_Wrapper(rotation->y);
        if (rotation->x)
            Matrix4x4::RotateAroundX_Wrapper(rotation->x);
    }
    if (scale)
        Matrix4x4::Scale(scale->x, scale->y, scale->z);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    DrawCharacterModelBranch(node);
#else
    DrawCharacterModelBranch(node, 0.0f);
#endif
    PopCurrentMatrix(1);
}
#endif

// @verified - d
void TransformScenarioMotionBonePose(shen_name character, std::int32_t bone, Vector3 *position, Vector3i *rotation) {
    Matrix4x4 transform;
    mtwkCopyBoneTransform(scenFindMotionwork(character), bone, &transform);
    TransformCharacterPoseByMatrix(&transform, position, rotation);
}

} // namespace s2
} // namespace shenmue

extern "C" {

void __attribute__((renesas)) shenmue_s2_event_EV_AT_SetPos(std::uint32_t name, std::uint32_t flags, const shenmue::Vector3 *position) {
    shenmue::s2::EV_AT_SetPos(shenmue::shen_name(name), flags, *position);
}

bool __attribute__((renesas)) shenmue_s2_event_GetMTWK_NodeParent_0(
    std::uint32_t name, std::uint32_t flags, std::int32_t selector, const shenmue::Vector3 *position) {
    return shenmue::s2::SetScenarioCharacterMotionTarget(shenmue::shen_name(name), flags, selector, position);
}

void __attribute__((renesas)) shenmue_s2_event_GetCharaPosition(std::uint32_t name, std::uint32_t bone, shenmue::Vector3 *position, std::uint32_t flags) {
    shenmue::s2::EV_GetCharaPosition(shenmue::shen_name(name), bone, position, flags);
}

shenmue::s2::ObjectXFormsParams *__attribute__((renesas)) shenmue_s2_event_AttachScenarioCharacterWithTransform(
    std::uint32_t target, std::uint32_t source, std::int32_t bone, const shenmue::Vector3 *position, const shenmue::Vector3i *rotation) {
    return shenmue::s2::AttachScenarioCharacterPreservingPlacement(shenmue::shen_name(target), shenmue::shen_name(source), bone, position, rotation);
}

void __attribute__((renesas)) shenmue_s2_event_AttachScenarioCharacter(
    std::uint32_t target, std::uint32_t source, std::int32_t bone, const shenmue::Vector3 *position, const shenmue::Vector3i *rotation) {
    shenmue::s2::AttachScenarioCharacter(shenmue::shen_name(target), shenmue::shen_name(source), bone, position, rotation);
}

shenmue::s2::ObjectXFormsParams *__attribute__((renesas)) shenmue_s2_event_ResetScenarioCharacterAttachment(std::uint32_t name) {
    return shenmue::s2::ResetScenarioCharacterAttachment(shenmue::shen_name(name));
}

void __attribute__((renesas)) shenmue_s2_event_scenSetCharaPosRotById(std::uint32_t name, const shenmue::Vector3 *position, const shenmue::Vector3i *rotation) {
    shenmue::s2::SetScenarioCharacterInversePlacement(shenmue::shen_name(name), position, rotation);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_SetScenarioCharacterPositionAndYaw(std::uint32_t name, std::int16_t childId) {
    return shenmue::s2::SetScenarioCharacterInverseChildPlacement(shenmue::shen_name(name), childId);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_EV_AT_SetAngY_0(
    std::uint32_t name, std::uint32_t flags, const shenmue::Vector3i *rotation, const std::uint16_t *customMotions) {
    return shenmue::s2::EV_AT_SetAngY(shenmue::shen_name(name), flags, *rotation, customMotions);
}

void __attribute__((renesas)) shenmue_s2_event_GetScenarioCharacterEulerAngles(std::uint32_t name, shenmue::Vector3i *rotation, std::uint32_t flags) {
    shenmue::s2::GetScenarioCharacterEulerAngles(shenmue::shen_name(name), rotation, flags);
}

void __attribute__((renesas)) shenmue_s2_event_AT_SetLocalBaseChara(std::uint32_t name, std::uint32_t base) {
    shenmue::s2::AT_SetLocalBaseChara(shenmue::shen_name(name), shenmue::shen_name(base));
}

std::uint32_t __attribute__((renesas)) shenmue_s2_event_SCEN_FindChar_Wrapper(std::uint32_t name) {
    return shenmue::s2::EV_SCEN_FindChar_Wrapper(shenmue::shen_name(name));
}

void __attribute__((renesas)) shenmue_s2_event_scenApplyPosOrRotToChildId(
    std::uint32_t name, std::uint16_t part, shenmue::Vector3 *position, shenmue::Vector3i *rotation, std::int32_t mode) {
    shenmue::s2::EV_scenApplyPosOrRotToPartOf(
        shenmue::shen_name(name), part, position, rotation, static_cast<shenmue::s2::EventCharacterPartTransformMode>(mode));
}
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" {
__attribute__((renesas)) std::int32_t shenmue_s2_event_scenMotionworkUtil(
    std::uint32_t name, std::uint32_t yaw, float angle, std::uint32_t flags, const void *motionData) {
    return shenmue::s2::scenMotionworkUtil(shenmue::shen_name(name), yaw, angle, flags, motionData);
}
__attribute__((renesas)) std::int32_t shenmue_s2_event_TurnScenarioCharacterTowardPosition(
    std::uint32_t name, const shenmue::Vector3 *target, const std::uint16_t *customMotions, std::uint32_t *yaw, float *angularStep, std::uint32_t flags) {
    return shenmue::s2::TurnScenarioCharacterTowardPosition(shenmue::shen_name(name), target, customMotions, yaw, angularStep, flags);
}
}
#endif

#endif
