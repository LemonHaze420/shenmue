#include "s2/character.hpp"

#include "logging.hpp"

#include "d3t/ui_renderer.hpp"
#include "d3t/object_store.hpp"
#include "s2/lighting.hpp"
#include "s2/author.hpp"
#include "s2/weather.hpp"
#include "s2/archive_data.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/field_collision.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/flag.hpp"
#include "s2/face.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_info.hpp"
#include "s2/pak.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/model_image.hpp"
#include "s2/game_runtime.hpp"
#include "s2/motion.hpp"
#include "s2/motion_pose.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/player.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/world_render.hpp"
#include "s2/handcuffs.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/hand.hpp"
#include "s2/osage.hpp"
#include "s2/pause.hpp"
#if defined(_WIN32)
#include "imgui.h"
#endif

#include <algorithm>
#include <array>
#include <cmath>
#include <bit>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
#if SHENMUE_IS_PC
#include <xmmintrin.h>
#endif

namespace shenmue::s2 {

Matrix4x4 g_characterNodeSavedTransform;
Matrix4x4 g_characterChildRotationTransform;
Matrix4x4 g_characterNodeEulerTransform;
Matrix4x4 g_characterNodePositionTransform;
Matrix4x4 g_characterNodeInverseTransform;
std::int32_t g_characterMirroredTransformEnabled;
namespace {
Matrix4x4 characterMirrorMatrix{};
Matrix4x4 *characterMirroredControllerMatrices{};
Matrix4x4 characterMirrorPlaneTransform{};

// @verified - p
void AllocateCharacterMirrorMatrices(std::int32_t enabled) {
    if (enabled) {
        characterMirroredControllerMatrices = static_cast<Matrix4x4 *>(HLib::AllocateTaskData(0x780, CHARACTER_MIRROR_STORAGE));
    } else if (characterMirroredControllerMatrices) {
        HLib::FreeTaskData(characterMirroredControllerMatrices);
        characterMirroredControllerMatrices = nullptr;
    }
}

// @verified - p
void ApplyCharacterMirrorPlaneTransform(const CharacterMirrorPlane *plane) {
    const Vector3 &p = plane->position;
    const Vector3 &n = plane->normal;
    const float distance = (n.y * p.y + n.x * p.x) + n.z * p.z;
    const float twiceDistance = distance + distance;
    characterMirrorPlaneTransform = {
        {{1.0f - (n.x + n.x) * n.x, (n.x * -2.0f) * n.y, (n.x * -2.0f) * n.z, 0.0f}, {(n.x * -2.0f) * n.y, 1.0f - (n.y + n.y) * n.y, (n.y * -2.0f) * n.z, 0.0f},
            {(n.x * -2.0f) * n.z, (n.y * -2.0f) * n.z, 1.0f - (n.z + n.z) * n.z, 0.0f}, {twiceDistance * n.x, twiceDistance * n.y, twiceDistance * n.z, 1.0f}}};
    Matrix4x4::MultiplyWithPrePassMatrix(&characterMirrorPlaneTransform);
}
}

// @verified - p
const Matrix4x4 *GetCharacterMirrorMatrix() {
    return &characterMirrorMatrix;
}

void ResetCharacterMirrorAdmission() {
    g_characterMirroredTransformEnabled = 0;
}

void RetireCharacterMirrorStorage() {
    AllocateCharacterMirrorMatrices(0);
    g_characterMirroredTransformEnabled = 0;
}

// @verified - p
void SetCharacterMirrorPlane(const CharacterMirrorPlane *plane, const Vector3i *rotation) {
    (void)Matrix4x4_Push(&transformMatrix_prePass);
    if (plane) {
        if (!g_characterMirroredTransformEnabled)
            AllocateCharacterMirrorMatrices(1);
        g_characterMirroredTransformEnabled = 1;
        const Vector3 &p = plane->position;
        if (rotation) {
            Matrix4x4::preTranslate(p.x, p.y, p.z);
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(rotation->z));
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(rotation->y));
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(rotation->x));
            Matrix4x4::preTranslate(-p.x, -p.y, -p.z);
        }
        ApplyCharacterMirrorPlaneTransform(plane);
    } else {
        if (g_characterMirroredTransformEnabled)
            AllocateCharacterMirrorMatrices(0);
        g_characterMirroredTransformEnabled = 0;
    }
    Matrix4x4::GetTransformMatrix(&characterMirrorMatrix);
    Matrix4x4::Pop(1);
}

// @verified - p
void BindMirroredMotionControllerMatricesToModelNodes(MotionWork *work, MT7Model *model) {
    (void)Matrix4x4_Push();
    ClearMT7NodeTransformOverrides(model->nodes.data());
    IKRig &rig = work->rig;
    const Matrix4x4 *const matrices[]{&rig.chest.upperChestWorld, &rig.head.headWorld, &rig.head.neckWorld, &rig.rightArm.shoulderWorld,
        &rig.rightArm.upperArmWorld, &rig.rightArm.forearmWorld, &rig.rightArm.handWorld, &rig.leftArm.shoulderWorld, &rig.leftArm.upperArmWorld,
        &rig.leftArm.forearmWorld, &rig.leftArm.handWorld, &rig.lowerBody.hipsWorld, &rig.lowerBody.leftThighWorld, &rig.lowerBody.leftShinWorld,
        &rig.lowerBody.leftAnkleWorld, &rig.lowerBody.rightThighWorld, &rig.lowerBody.rightShinWorld, &rig.lowerBody.rightAnkleWorld};
    for (std::size_t index = 0; index < motionControllerModelNodeIds.size(); ++index) {
        const std::uint16_t id = index == 1 ? 0xffbdU : index == 2 ? 2U : motionControllerModelNodeIds[index];
        MT7Node *const node = GetChildObjectByIndex(*model, model->nodes.data(), id);
        if (!node)
            continue;
        Matrix4x4::SetActiveTransform(&characterMirrorMatrix);
        Matrix4x4::MultiplyWithPrePassMatrix(matrices[index]);
        Matrix4x4::GetTransformMatrix(characterMirroredControllerMatrices + index);
        SetMT7NodeTransformOverride(*node, characterMirroredControllerMatrices + index);
    }
    const Matrix4x4 *const tail[]{rig.useOverride ? &rig.lowerBody.pelvisWorld : &rig.chest.chestWorld,
        rig.useOverride ? rig.lowerBody.leftHipWorld : &rig.rightArm.wristWorld, rig.useOverride ? rig.lowerBody.rightHipWorld : &rig.leftArm.wristWorld,
        rig.lowerBody.rightToe1World, rig.lowerBody.rightToe2World, rig.lowerBody.rightToe3World, rig.head.aimSourceWorld, rig.head.nodeAWorld,
        rig.head.nodeBWorld, rig.head.nodeCWorld};
    const std::size_t count = rig.useOverride ? motionOverrideModelNodeIds.size() : motionNormalModelNodeIds.size();
    for (std::size_t index = 0; index < count; ++index) {
        const std::uint16_t id = rig.useOverride ? motionOverrideModelNodeIds[index] : motionNormalModelNodeIds[index];
        MT7Node *const node = GetChildObjectByIndex(*model, model->nodes.data(), id);
        if (!node)
            continue;
        Matrix4x4::SetActiveTransform(&characterMirrorMatrix);
        Matrix4x4::MultiplyWithPrePassMatrix(tail[index]);
        Matrix4x4::GetTransformMatrix(characterMirroredControllerMatrices + 18 + index);
        SetMT7NodeTransformOverride(*node, characterMirroredControllerMatrices + 18 + index);
    }
    Matrix4x4::Pop(1);
}

// @verified - p
void SetChildObjectFlagByIndex(Character *character, std::uint8_t mode) {
    if (mode > 4U || !character->model)
        return;
    MT7Model &model = *static_cast<MT7Model *>(character->model);
    MT7Node *const rightElbow = GetChildObjectByIndex(model, model.nodes.data(), 5U);
    if (!rightElbow)
        return;
    MT7Node *const leftElbow = GetChildObjectByIndex(model, model.nodes.data(), 10U);
    if (!leftElbow)
        return;
    const std::uint32_t selector = static_cast<std::uint32_t>(mode) << 16U;
    rightElbow->mdcVertexLinkFlags = (rightElbow->mdcVertexLinkFlags & 0xffffU) | selector;
    leftElbow->mdcVertexLinkFlags = (leftElbow->mdcVertexLinkFlags & 0xffffU) | selector;
}

// @verified - p
bool IsCharacterMirroredForDraw(const Character *character) {
    return charaHasMirroredFlags(character) != 0 && g_characterMirroredTransformEnabled != 0;
}

// @verified - p
float GetCharacterDisplayRadius(const Character *character) {
    if (!character)
        return 0.0f;
    const float radius = character->radius;
    return radius < 5.0f && IsCharacterMirroredForDraw(character) ? 5.0f : radius;
}

// @verified - p
float SetScenarioCharacterRadius(shen_name name, float radius) {
    Character *const character = ctrlGetCharaByName(name);
    const float previous = GetCharacterDisplayRadius(character);
    if (radius >= 0.0f)
        SetCharacterRadius(character, radius);
    return previous;
}

// @verified - p
void ApplyRecursiveCharacterNodeRotations(const CharacterNode *node) {
    if (node->child)
        ApplyRecursiveCharacterNodeRotations(node->child);
    Matrix4x4::SetViewMatrixFromEuler(node->rotation[1], node->rotation[0], node->rotation[2]);
}

// @verified - p
void TransformVectorToCharacterChildSpace(const CharacterNode *node, Vector3 *vector) {
    Matrix4x4::GetTransformMatrix(&g_characterNodeSavedTransform);
    Matrix4x4::SetPrePassToTempTransform();
    if (node->child)
        ApplyRecursiveCharacterNodeRotations(node->child);
    Matrix4x4::SetViewMatrixFromEuler(node->rotation[1], node->rotation[0], node->rotation[2]);
    Matrix4x4::GetTransformMatrix(&g_characterChildRotationTransform);
    const float y = vector->y - g_characterChildRotationTransform.rows[3].y;
    const float x = vector->x - g_characterChildRotationTransform.rows[3].x;
    const float z = vector->z - g_characterChildRotationTransform.rows[3].z;
    const Matrix4x4 &transform = g_characterChildRotationTransform;
    vector->x = (transform.rows[0].y * y + transform.rows[0].x * x) + transform.rows[0].z * z;
    vector->y = (transform.rows[1].y * y + transform.rows[1].x * x) + transform.rows[1].z * z;
    vector->z = (transform.rows[2].y * y + transform.rows[2].x * x) + transform.rows[2].z * z;
    Matrix4x4::SetActiveTransform(&g_characterNodeSavedTransform);
}

// @verified - p
void AssignCharacterChildSpacePosition(Character *character, const Vector3 *position) {
    if (!character)
        return;
    Vector3 local = *position;
    if (character->entity.node.child)
        TransformVectorToCharacterChildSpace(character->entity.node.child, &local);
    character->position = local;
}

// @verified - p
void AddCharacterChildSpacePosition(Character *character, const Vector3 *delta) {
    if (!character)
        return;
    Vector3 local = *delta;
    if (character->entity.node.child)
        TransformVectorToCharacterChildSpace(character->entity.node.child, &local);
    character->position.x = local.x + character->position.x;
    character->position.y = local.y + character->position.y;
    character->position.z = local.z + character->position.z;
}

// @verified - p
void UpdateMotionRootDeltaVector(const MotionWork *work, Vector4 *vector) {
    if (work->runtime.motnFlags & 1U) {
        vector->x = work->runtime.rootPosition.x - work->runtime.previousRootPosition.x;
        vector->y = work->runtime.rootDelta.y;
        vector->z = work->runtime.rootPosition.z - work->runtime.previousRootPosition.z;
    } else {
        vector->x = work->runtime.rootDelta.x;
        vector->y = work->runtime.rootDelta.y;
        vector->z = work->runtime.rootDelta.z;
    }
}

// @verified - p
void SetCharaTargetTransformOffset(shen_name name, const Vector3 *factor, std::int32_t rotation) {
    Character *const character = ctrlGetCharaByName(name);
    if (!character)
        return;
    const MotionWork *const work = FindMotionWork(character);
    if (!work)
        return;
    Vector4 rootDelta;
    UpdateMotionRootDeltaVector(work, &rootDelta);
    Vector3 delta{rootDelta.x, rootDelta.y, rootDelta.z};
    if (rotation) {
        Vector3i angles{};
        GetCharacterLocalRotation(character, &angles);
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int16_t>(0U - static_cast<std::uint32_t>(angles.y)));
        Matrix4x4::TransformPoint(delta, delta);
        delta.x *= factor->x;
        delta.y *= factor->y;
        delta.z *= factor->z;
        Matrix4x4::Pop(1);
        Matrix4x4_Push(&transformMatrix_prePass);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::int16_t>(angles.y));
        Matrix4x4::TransformPoint(delta, delta);
        Matrix4x4::Pop(1);
        AssignCharacterChildSpacePosition(character, &delta);
    } else {
        delta.x *= factor->x;
        delta.y *= factor->y;
        delta.z *= factor->z;
        AddCharacterChildSpacePosition(character, &delta);
    }
}

// @verified - p
bool HasCharacterMotionWork(const Character *character) {
    return FindMotionWork(character) != nullptr;
}

// @verified - p
std::int32_t GetCharacterMotionCompatibilityResult(const Character *character) {
    HasCharacterMotionWork(character);
    return -1;
}

CharacterNode *GetCharacterNode(Character *character) {
    return character ? &character->entity.node : nullptr;
}
std::uint32_t *GetControllerForNode(Character *character) {
    return character ? &character->fieldE8 : nullptr;
}
void GetCharacterLocalPosition(const Character *character, Vector3 *output) {
    *output = character ? character->entity.node.position : Vector3{};
}
void GetCharacterLocalRotation(const Character *character, Vector3i *output) {
    if (character)
        *output = {character->entity.node.rotation[0], character->entity.node.rotation[1], character->entity.node.rotation[2]};
}
std::int32_t shenGetRotYFromNode(const Character *character) {
    return character ? character->entity.node.rotation[1] : 0;
}
float GetCharacterLocalPositionY(const Character *character) {
    return character ? character->entity.node.position.y : 0.0f;
}
void SetCharacterLocalPosition(Character *character, const Vector3 *position) {
    if (character)
        character->entity.node.position = *position;
}
void SetCharacterLocalRotation(Character *character, const Vector3i *rotation) {
    if (!character)
        return;
    character->entity.node.rotation[0] = rotation->x;
    character->entity.node.rotation[1] = rotation->y;
    character->entity.node.rotation[2] = rotation->z;
}
void shenSetRotY(Character *character, std::int32_t yaw) {
    if (character)
        character->entity.node.rotation[1] = yaw;
}
void shenGetNodePosY(const Character *character, Vector3 *output) {
    if (character)
        *output = character->position;
}
void SetCharacterCollisionPosition(Character *character, const Vector3 *position) {
    if (character)
        character->position = *position;
}
void coliPositionAdd(Character *character, const Vector3 *delta) {
    if (!character)
        return;
    character->position.x += delta->x;
    character->position.y = delta->y + character->position.y;
    character->position.z += delta->z;
}
void GetCharacterScale(const Character *character, Vector3 *output) {
    if (character)
        *output = character->scale;
}
void SetCharacterScale(Character *character, const Vector3 *scale) {
    if (!character)
        return;
    std::uint64_t scaleXY{};
    std::memcpy(&scaleXY, scale, sizeof(scaleXY));
    std::memcpy(&character->scale, &scaleXY, sizeof(scaleXY));
    std::uint32_t scaleZ{};
    std::memcpy(&scaleZ, reinterpret_cast<const unsigned char *>(scale) + sizeof(scaleXY), sizeof(scaleZ));
    std::memcpy(reinterpret_cast<unsigned char *>(&character->scale) + sizeof(scaleXY), &scaleZ, sizeof(scaleZ));
}
std::uint8_t GetCharaFlag(const Character *character) {
    return character->field99;
}
void SetCharaFlag(Character *character, std::uint8_t flag) {
    character->field99 = flag;
}
std::int32_t GetCharacterInternalState(const Character *character) {
    return character ? character->internalState : 0;
}
void SetCharacterInternalState(Character *character, std::int32_t state) {
    if (character)
        character->internalState = state;
}
float charaGetAlphaDraw(const Character *character) {
    return character ? character->alpha : 0.0f;
}
float charaGetShadowY(const Character *character) {
    return character ? std::bit_cast<float>(character->shadowY) : 0.0f;
}
std::int32_t GetCharacterShadowYaw(const Character *character) {
    return character ? character->field88 : 0;
}
void GetCharacterShadowScale(const Character *character, Vector3 *output) {
    if (character)
        *output = {character->field7c, character->field80, character->field84};
}
void GetCharacterShadowOffset(const Character *character, Vector3 *output) {
    if (character)
        std::memcpy(output, &character->field70, sizeof(*output));
}
void SetCharacterShadowScale(Character *character, const Vector3 *scale) {
    if (!character)
        return;
    character->field7c = scale->x;
    character->field80 = scale->y;
    character->field84 = scale->z;
}
void SetCharacterShadowOffset(Character *character, const Vector3 *offset) {
    if (character)
        std::memcpy(&character->field70, offset, sizeof(*offset));
}
void SetCharacterShadowYaw(Character *character, std::int32_t yaw) {
    if (character)
        character->field88 = yaw;
}
void SetCharacterShadowY(Character *character, float height) {
    if (character)
        character->shadowY = std::bit_cast<std::int32_t>(height);
}
void SetCharacterStoredShadowYEnabled(Character *character, std::int32_t enabled) {
    if (character)
        character->flags = enabled ? character->flags | CF_UseStoredShadowY : character->flags & ~CF_UseStoredShadowY;
}
std::int32_t GetEntityType(const Character *character) {
    return character ? character->type : -1;
}
bool charaCheckFlags(const Character *character) {
    return character && (character->flags & CF_Unknown40) != 0;
}
bool charaIsDisplayCheckEnabled(const Character *character) {
    return character && (character->flags & CF_DisplayCheckEnabled) != 0;
}
bool charaGetWasVisible(const Character *character) {
    return character && (character->flags & CF_VisibleThisCheck) != 0;
}
bool charaForceVisibleCheck(const Character *character) {
    return character && (character->flags & CF_ForceVisibleCheck) != 0;
}
std::uint32_t charaHasMirroredFlags(const Character *character) {
    return character ? character->flags & CF_DrawMirrored : 0;
}
bool IsCharacterActive(const Character *character) {
    return character && (character->flags & CF_Active) != 0;
}
bool IsCharacterUsingStoredShadowY(const Character *character) {
    return character && (character->flags & CF_UseStoredShadowY) != 0;
}
bool HasCharacterFacialModel(const Character *character) {
    return character && character->facialModel != nullptr;
}
void ClearCharacterFacialModel(Character *character) {
    if (character)
        character->facialModel = nullptr;
}
void charaSetDisplayEnabledFlag(Character *character, std::int32_t enabled) {
    if (character)
        character->flags = enabled ? character->flags | CF_DisplayCheckEnabled : character->flags & ~CF_DisplayCheckEnabled;
}
void charaSetFlags(Character *character, std::int32_t enabled) {
    if (character)
        character->flags = enabled ? character->flags | CF_Active : character->flags & ~CF_Active;
}
void charaToggleFlag(Character *character, std::int32_t enabled) {
    if (character)
        character->flags = enabled ? character->flags | CF_Unknown40 : character->flags & ~CF_Unknown40;
}
void SetMirrored(Character *character, std::int32_t enabled) {
    if (character)
        character->flags = enabled ? character->flags | CF_DrawMirrored : character->flags & ~CF_DrawMirrored;
}
void charaSetFlagFor(Character *character, std::int32_t flag) {
    if (character)
        character->field94 = flag;
}
void SetCharacterRadius(Character *character, float radius) {
    if (character)
        character->radius = radius;
}
shen_name GetCharacterModelName(const Character *character) {
    return character ? shen_name{character->fieldA0} : CHARACTER_INVALID_NAME;
}
void SetCharacterModelName(Character *character, shen_name name) {
    if (character)
        character->fieldA0 = name.name;
}
void SetCharacterUnknownFlag(Character *character, std::uint32_t flag) {
    if (character)
        character->useUnknownFlag = flag;
}
void *GetCharacterAuxiliaryData(const Character *character) {
    return character ? character->data2 : nullptr;
}
void SetCharacterAuxiliaryData(Character *character, void *data) {
    if (character && data)
        character->data2 = data;
}
CharacterCoreCallback charaAssignCoreCallback(Character *character, CharacterCoreCallback callback) {
    if (!character)
        return nullptr;
    const CharacterCoreCallback previous = character->core;
    character->core = callback;
    return previous;
}
CharacterUpdateCallback GetMotionCallback(const Character *character) {
    return character ? character->applyScale : nullptr;
}
CharacterUpdateCallback imgmAssignUpdateCallback(Character *character, CharacterUpdateCallback callback) {
    if (!character)
        return nullptr;
    const CharacterUpdateCallback previous = character->applyScale;
    character->applyScale = callback;
    return previous;
}
CharacterDrawCallback mtwkAssignDrawCallback(Character *character, CharacterDrawCallback callback) {
    if (!character)
        return nullptr;
    const CharacterDrawCallback previous = character->collisionCheck;
    character->collisionCheck = callback;
    return previous;
}
CharacterUpdateCallback charaAssignPrepareForDrawCallback(Character *character, CharacterUpdateCallback callback) {
    if (!character)
        return nullptr;
    const CharacterUpdateCallback previous = character->prepareForDraw;
    character->prepareForDraw = callback;
    return previous;
}

void SetCharacterType(Character *character, std::int32_t type) {
    if (character && character->type != CharacterTypePlayer && character->type != CharacterTypePlayerInverted)
        character->type = type;
}

void modelSetCharaAlpha(Character *character, float alpha) {
    if (character)
        character->alpha = alpha;
}

std::uint16_t SetPluralControlFor(Character *character, std::int16_t enabled) {
    if (!character)
        return 1;
    const std::uint16_t previous = static_cast<std::uint16_t>(character->fieldE8 & 1U);

    character->fieldE8 = (character->fieldE8 & 0x00fffffeU) | (enabled ? 0x01000001U : 0x02000000U);
    return previous;
}

bool IsCharacterDrawControlEnabled(const Character *character) {
    return character && (character->fieldE8 & 0x00ff0000U) != 0;
}

void SetCharacterDrawControlEnabled(Character *character, bool enabled) {
    if (!character)
        return;
    if (!enabled)
        character->fieldE8 &= ~0x00ff0000U;
    else if (!(character->fieldE8 & 0x00ff0000U))
        character->fieldE8 |= 0x00010000U;
}

std::int16_t GetAllAngYForNode(const CharacterNode *root) {
    std::uint16_t rotation = 0;
    for (const CharacterNode *node = root; node; node = node->child) {
        rotation = static_cast<std::uint16_t>(rotation + static_cast<std::uint16_t>(node->rotation[1]));
    }
    return static_cast<std::int16_t>(rotation);
}

void shenDecrementRotYAllNodes(CharacterNode *root, std::int16_t rotationY) {
    if (!root)
        return;
    std::uint16_t rootRotation = static_cast<std::uint16_t>(rotationY);
    for (CharacterNode *node = root->child; node; node = node->child) {
        rootRotation = static_cast<std::uint16_t>(rootRotation - static_cast<std::uint16_t>(node->rotation[1]));
    }
    root->rotation[1] = static_cast<std::int16_t>(rootRotation);
}
void ApplyRecursiveCharacterNodeTransforms(const CharacterNode *node) {
    if (!node)
        return;
    if (node->child)
        ApplyRecursiveCharacterNodeTransforms(node->child);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    Matrix4x4::SetViewMatrixFromEuler(node->rotation[1], node->rotation[0], node->rotation[2]);
}

// @verified - p
void TransformPointToCharacterNodeSpace(const CharacterNode *node, Vector3 *point) {
    Matrix4x4::GetTransformMatrix(&g_characterNodeSavedTransform);
    Matrix4x4::SetPrePassToTempTransform();
    ApplyRecursiveCharacterNodeTransforms(node);
    Matrix4x4::GetTransformMatrix(&g_characterNodeInverseTransform);
    const Matrix4x4 &transform = g_characterNodeInverseTransform;
    const float y = point->y - transform.rows[3].y;
    const float x = point->x - transform.rows[3].x;
    const float z = point->z - transform.rows[3].z;
    point->x = (transform.rows[0].y * y + transform.rows[0].x * x) + transform.rows[0].z * z;
    point->y = (transform.rows[1].y * y + transform.rows[1].x * x) + transform.rows[1].z * z;
    point->z = (transform.rows[2].y * y + transform.rows[2].x * x) + transform.rows[2].z * z;
    Matrix4x4::SetActiveTransform(&g_characterNodeSavedTransform);
}
namespace {

Vector3i ExtractCharacterRotation(const Matrix4x4 &transform) {
    constexpr float poleEpsilon = 0.0000076295109f;
    const float sine = -transform.rows[0].z;
    Vector3i rotation{};
    if (sine - 1.0f < -poleEpsilon) {
        if (1.0f - transform.rows[0].z > poleEpsilon) {
            rotation.x = FixedAtan2(transform.rows[1].z, transform.rows[2].z);
            rotation.y = FixedAtan2(sine, std::sqrt(1.0f - sine * sine));
            rotation.z = FixedAtan2(transform.rows[0].y, transform.rows[0].x);
        } else {
            rotation.x = FixedAtan2(-transform.rows[1].x, transform.rows[1].y);
            rotation.y = 49152;
        }
    } else {
        rotation.x = FixedAtan2(transform.rows[1].x, transform.rows[1].y);
        rotation.y = 16384;
    }
    if (rotation.x >= 32768 && rotation.z >= 32768) {
        rotation.x -= 32768;
        rotation.y = 32768 - rotation.y;
        rotation.z -= 32768;
    }
    return rotation;
}

} // namespace
// @verified - p
void TransformCharacterPoseByMatrix(const Matrix4x4 *transform, Vector3 *position, Vector3i *rotation) {
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(transform);
    Matrix4x4::preTranslate(position->x, position->y, position->z);
    Matrix4x4::SetViewMatrixFromEuler(rotation->y, rotation->x, rotation->z);
    Matrix4x4 composed;
    Matrix4x4::GetTransformMatrix(&composed);
    *position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    *rotation = ExtractCharacterRotation(composed);
}

// @verified - p
void InverseTransformCharacterPoseByMatrix(const Matrix4x4 *transform, Vector3 *position, Vector3i *rotation) {
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(transform);
    Matrix4x4::InvertInPlace(nullptr);
    Matrix4x4::preTranslate(position->x, position->y, position->z);
    Matrix4x4::SetViewMatrixFromEuler(rotation->y, rotation->x, rotation->z);
    Matrix4x4 composed;
    Matrix4x4::GetTransformMatrix(&composed);
    *position = {.x = workTransformMatrix.rows[3].x, .y = workTransformMatrix.rows[3].y, .z = workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    ExtractMatrixEulerAnglesFixed(&composed, rotation);
}

void ApplyCharacterEntityTransform(const Character *character) {
    if (character)
        ApplyRecursiveCharacterNodeTransforms(&character->entity.node);
}

// @verified - p
Vector3 GetCharacterWorldPosition(const Character *character) {
    if (!character)
        return {};
    const CharacterNode &node = character->entity.node;
    if (!node.child)
        return node.position;

    Matrix4x4::GetTransformMatrix(&g_characterNodeSavedTransform);
    Matrix4x4::SetPrePassToTempTransform();
    ApplyRecursiveCharacterNodeTransforms(&node);
    const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::SetActiveTransform(&g_characterNodeSavedTransform);
    return position;
}

// @verified - p
Vector3i GetCharacterWorldRotation(const Character *character) {
    if (!character)
        return {};
    const CharacterNode &node = character->entity.node;
    if (!node.child) {
        return {node.rotation[0], node.rotation[1], node.rotation[2]};
    }
    Matrix4x4::GetTransformMatrix(&g_characterNodeSavedTransform);
    Matrix4x4::SetPrePassToTempTransform();
    ApplyRecursiveCharacterNodeTransforms(&node);
    Matrix4x4::GetTransformMatrix(&g_characterNodeEulerTransform);
    const Vector3i rotation = ExtractCharacterRotation(g_characterNodeEulerTransform);
    Matrix4x4::SetActiveTransform(&g_characterNodeSavedTransform);
    return rotation;
}

// @verified - p
void shenApplyTranslationToAll(CharacterNode *root, const Vector3 &translation) {
    if (!root)
        return;
    if (!root->child) {
        root->position = translation;
        return;
    }

    Matrix4x4::GetTransformMatrix(&g_characterNodeSavedTransform);
    Matrix4x4::SetPrePassToTempTransform();
    CharacterNode *const next = root->child;
    if (next->child) {
        ApplyRecursiveCharacterNodeTransforms(next->child);
    }
    Matrix4x4::preTranslate(next->position.x, next->position.y, next->position.z);
    Matrix4x4::SetViewMatrixFromEuler(next->rotation[1], next->rotation[0], next->rotation[2]);
    Matrix4x4::GetTransformMatrix(&g_characterNodePositionTransform);
    const Matrix4x4 &tmpTransform = g_characterNodePositionTransform;
    const float deltaX = translation.x - tmpTransform.rows[3].x;
    const float deltaY = translation.y - tmpTransform.rows[3].y;
    const float deltaZ = translation.z - tmpTransform.rows[3].z;
    root->position = {tmpTransform.rows[0].y * deltaY + tmpTransform.rows[0].x * deltaX + tmpTransform.rows[0].z * deltaZ,
        tmpTransform.rows[1].y * deltaY + tmpTransform.rows[1].x * deltaX + tmpTransform.rows[1].z * deltaZ,
        tmpTransform.rows[2].y * deltaY + tmpTransform.rows[2].x * deltaX + tmpTransform.rows[2].z * deltaZ};
    Matrix4x4::SetActiveTransform(&g_characterNodeSavedTransform);
}

// @verified - p
void SetCharacterNodeWorldRotation(CharacterNode *node, const Vector3i &rotation) {
    Vector3i local;
    if (node->child) {
        Matrix4x4_Push(&transformMatrix_prePass);
        CharacterNode *const child = node->child;
        if (child->child)
            ApplyRecursiveCharacterNodeTransforms(child->child);
        Matrix4x4::preTranslate(child->position.x, child->position.y, child->position.z);
        Matrix4x4::SetViewMatrixFromEuler(child->rotation[1], child->rotation[0], child->rotation[2]);
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::InvertInPlace(nullptr);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
        Matrix4x4 transform;
        Matrix4x4::GetTransformMatrix(&transform);
        Matrix4x4::Pop(1);
        local = ExtractCharacterRotation(transform);
    } else {
        local = rotation;
    }
    node->rotation[0] = local.x;
    node->rotation[1] = local.y;
    node->rotation[2] = local.z;
}

// @verified - p
void SetCharacterWorldRotation(Character *character, const Vector3i &rotation) {
    if (character)
        SetCharacterNodeWorldRotation(&character->entity.node, rotation);
}

// @verified - p
float GetHorizontalDistance(const Vector3 *first, const Vector3 *second) {
    const float x = first->x - second->x, z = first->z - second->z;
    return std::sqrt(x * x + z * z);
}

namespace {

enum class ArgumentType : std::uint8_t {
    Float = 1,
    Name = 2,
    String = 3,
    Records = 4,
};

struct Record;

struct Argument {
    ArgumentType type{};
    std::uint32_t value{};
    const char *string{};
    std::vector<Record> records;
};

struct Record {
    std::string_view token;
    std::vector<Argument> arguments;
};

shen_name currentCharacterTableName{CHARACTER_TABLE_DEFAULT_NAME};

struct CharacterMapEntry {
    CharacterMapEntry *next;
    Character *character;
    HLib::Task *handlerTask;
};

static_assert(sizeof(CharacterMapEntry) == 0x18);
static_assert(offsetof(CharacterMapEntry, character) == 0x08);
static_assert(offsetof(CharacterMapEntry, handlerTask) == 0x10);

constexpr std::size_t scenarioStepModelCount = static_cast<std::size_t>(ScenarioStepModelAsset::HKSMH01G) + 1;

struct ScenarioStepModelResource {
    std::vector<std::byte> bytes;
    MT7Model model;
    bool loaded{};
};

struct ScenarioStepModelSet {
    std::array<ScenarioStepModelResource, scenarioStepModelCount> models;
    std::uint32_t loadedMask{};
};

struct CharacterResource {
    Character *character{};
    HLib::Task *handlerTask{};
    CharacterMapEntry *mapEntry{};
    shen_name scenarioName{};
    std::span<std::byte> modelBytes;
    MT7Model model;
    std::string image;
    std::string leftHand;
    std::string rightHand;
    bool human{};
    std::unique_ptr<ScenarioStepModelSet> scenarioStepModels;
    ScenarioStepHighModelState pendingHighModel{};
    bool hasPendingHighModel{};
    ~CharacterResource() {
        if (!modelBytes.empty())
            HLib::FreeTaskData(modelBytes.data());
    }
};

struct CharacterCoreDrawContext {
    Character *character{};
    const MT7Model *model{};
    const Matrix4x4 *entityTransform{};
    const Matrix4x4 *drawBase{};
    std::uint32_t draws{};
};

CharacterCoreDrawContext *currentCharacterCoreDrawContext{};
std::vector<std::unique_ptr<CharacterResource>> characters;
Matrix4x4 IdentityTransform() {
    Matrix4x4 transform{};
    transform.rows[0].x = 1.0f;
    transform.rows[1].y = 1.0f;
    transform.rows[2].z = 1.0f;
    transform.rows[3].w = 1.0f;
    return transform;
}

Matrix4x4 MultiplyTransforms(const Matrix4x4 &lhs, const Matrix4x4 &rhs) {
    Matrix4x4 output{};
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t column = 0; column < 4; ++column) {
            output.rows[row][column] = lhs.rows[row].x * rhs.rows[0][column] + lhs.rows[row].y * rhs.rows[1][column] + lhs.rows[row].z * rhs.rows[2][column] +
                lhs.rows[row].w * rhs.rows[3][column];
        }
    }
    return output;
}

Matrix4x4 CharacterEntityTransform(const Character &character, bool applyScale = true) {
    const Matrix4x4 saved = workTransformMatrix;
    workTransformMatrix = IdentityTransform();
    ApplyRecursiveCharacterNodeTransforms(&character.entity.node);
    if (applyScale && (character.scale.x != 1.0f || character.scale.y != 1.0f || character.scale.z != 1.0f)) {
        Matrix4x4::Scale(character.scale.x, character.scale.y, character.scale.z);
    }
    const Matrix4x4 transform = workTransformMatrix;
    workTransformMatrix = saved;
    return transform;
}

Matrix4x4 ModelNodeLocalTransform(const MT7Node &node) {
    const Matrix4x4 saved = workTransformMatrix;
    workTransformMatrix = IdentityTransform();
    ApplyMT7NodeTransform(node);
    const Matrix4x4 transform = workTransformMatrix;
    workTransformMatrix = saved;
    return transform;
}

CharacterResource *FindCharacterResource(const Character *character) {
    for (const std::unique_ptr<CharacterResource> &resource : characters) {
        if (resource && resource->character == character) {
            return resource.get();
        }
    }
    return nullptr;
}

bool ResolveStaticModelNodeTransform(const CharacterResource &resource, std::uint32_t nodeId, Matrix4x4 &transform) {
    if (!resource.character->model)
        return false;
    const MT7Model &model = *static_cast<const MT7Model *>(resource.character->model);
    std::size_t index = 0;
    while (index < model.nodes.size() && (static_cast<std::uint32_t>(model.nodes[index].id) & 0xffffU) != nodeId)
        ++index;
    if (index == model.nodes.size())
        return false;

    std::int32_t nodeIndex = static_cast<std::int32_t>(index);
    transform = IdentityTransform();
    while (nodeIndex >= 0 && static_cast<std::size_t>(nodeIndex) < model.nodes.size()) {
        const MT7Node &current = model.nodes[static_cast<std::size_t>(nodeIndex)];
        transform = MultiplyTransforms(transform, ModelNodeLocalTransform(current));
        nodeIndex = current.parent;
    }
    transform = MultiplyTransforms(transform, CharacterEntityTransform(*resource.character));
    return true;
}

} // namespace

// @verified - p
void charaCore(Character *character) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const shen_name name = scenGetCharaName(control->scenarioTask, character);
    const MT7Model *const model = static_cast<const MT7Model *>(character->model);
    if (!model)
        return;
    SetScriptedModelEntity(character);
    Matrix4x4_Push(nullptr);
    ApplyRecursiveCharacterNodeTransforms(&character->entity.node);
    if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
        Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
    g_scriptedModelScale.x = character->scale.x;
    if (name.name == CHARACTER_ROCK_OBJECT.name) {
        SetLegacyBusyPreparingDraw(true);
        if (character->alpha < 1.0f)
            SetLegacyQueuedDrawDepthMode(0, 1000.0f);
    }
    if (character->useUnknownFlag)
        SetLegacyBusyPreparingDraw(true);
    const CharacterUpdateCallback applyScale = character->applyScale;
    applyScale(model, model->nodes.data(), character->alpha);
    if (IsCharacterMirroredForDraw(character)) {
        Matrix4x4::Pop(1);
        Matrix4x4_Push(nullptr);
        Matrix4x4::MultiplyWithPrePassMatrix(&characterMirrorMatrix);
        ApplyRecursiveCharacterNodeTransforms(&character->entity.node);
        if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
            Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
        g_scriptedModelScale.x = character->scale.x;
        SetLegacyMirroredDrawMode(1);
        applyScale(model, model->nodes.data(), 1.0f);
        SetLegacyMirroredDrawMode(0);
    }
    if (name.name == CHARACTER_ROCK_OBJECT.name) {
        SetLegacyBusyPreparingDraw(false);
        if (character->alpha < 1.0f)
            SetLegacyQueuedDrawDepthMode(1, 0.0f);
    }
    if (character->useUnknownFlag)
        SetLegacyBusyPreparingDraw(false);
    Matrix4x4::Pop(1);
    SetScriptedModelEntity(nullptr);
}

// @verified - p
void DrawDummyCharacterCore(Character *character) {
    const MT7Model *const model = static_cast<const MT7Model *>(character->model);
    if (!model)
        return;
    Matrix4x4_Push(nullptr);
    ApplyRecursiveCharacterNodeTransforms(&character->entity.node);
    if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
        Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
    DrawDummyCharacterModel(*model);
    Matrix4x4::Pop(1);
}

// @verified - p
CharacterCoreCallback charaAssignDummyCore(Character *character) {
    return charaAssignCoreCallback(character, DrawDummyCharacterCore);
}

void mtwkDrawCharacterCore(Character *character) {
    MotionWork *const motionWork = FindMotionWork(character);
    if (!motionWork)
        return;
    g_motionDeformationEnabled = 1;
    g_motionDeformationControllers = {&motionWork->rig.chest.upperChestWorld, &motionWork->rig.rightArm.shoulderWorld, &motionWork->rig.rightArm.upperArmWorld,
        &motionWork->rig.leftArm.shoulderWorld, &motionWork->rig.leftArm.upperArmWorld, &motionWork->rig.rightArm.wristWorld,
        &motionWork->rig.leftArm.wristWorld};
    g_motionDrawOverrideFlags = GetCustomMotionOverrideFlags(motionWork);
    if (!character->model) {
        g_motionDrawOverrideFlags = 0;
        return;
    }
    const MT7Model *const model = static_cast<const MT7Model *>(character->model);
    Matrix4x4 drawBase;
    Matrix4x4 entityTransform;
    CharacterCoreDrawContext standalone{};
    CharacterCoreDrawContext *context = currentCharacterCoreDrawContext;
    if (!context || context->character != character || !context->model) {
        drawBase = workTransformMatrix;
        const Matrix4x4 *const frames = IsCharacterMirroredForDraw(character) ? nullptr : GetMotionDrawOverrideData(motionWork);
        if (frames)
            entityTransform = MultiplyTransforms(CharacterEntityTransform(*character, false), drawBase);
        standalone = {.character = character, .model = model, .entityTransform = frames ? &entityTransform : nullptr, .drawBase = &drawBase, .draws = 0};
        context = &standalone;
    }
    SetScriptedModelEntity(character);
    const bool motionDraw = motionWork != nullptr;
    const bool mirrored = IsCharacterMirroredForDraw(character);
    const Matrix4x4 *const matrixFrames = mirrored ? nullptr : GetMotionDrawOverrideData(motionWork);
    const bool scripted = !motionDraw || FindModelComponent(character, TASK_TMNM) ||
        (character->applyScale && character->applyScale != charaApplyScale && character->applyScale != mtUpdateTransparencyCallback);
    if (motionDraw) {
        UpdateCharacterOsage(const_cast<MT7Node *>(context->model->nodes.data()), character);
    }
    if (motionWork && !matrixFrames && (!mirrored || g_loadingFlag != 0))
        BindMotionControllerMatricesToModelNodes(character, motionWork, const_cast<MT7Model *>(context->model));
    const Matrix4x4 *const originalEntityTransform = context->entityTransform;
    bool fixedQueueDepth = false;
    if (!mirrored && !matrixFrames) {
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        fixedQueueDepth = scenGetCharaName(control->scenarioTask, character).name == CHARACTER_RYO.name && character->alpha < 1.0f;
    }
    if (fixedQueueDepth)
        SetLegacyQueuedDrawDepthMode(0, 1000.0f);
    for (std::uint32_t pass = 0; pass < (mirrored ? 2U : 1U); ++pass) {
        Matrix4x4 mirroredEntity;
        if (pass) {
            (void)Matrix4x4_Push(context->drawBase);
            Matrix4x4::MultiplyWithPrePassMatrix(&characterMirrorMatrix);
            ApplyRecursiveCharacterNodeTransforms(&character->entity.node);
            if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
                Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
            Matrix4x4 local = CharacterEntityTransform(*character);
            Matrix4x4 rootRebase;
            if (motionWork && BuildMotionRootRebaseTransform(motionWork, rootRebase))
                local = MultiplyTransforms(local, rootRebase);
            mirroredEntity = MultiplyTransforms(MultiplyTransforms(local, characterMirrorMatrix), *context->drawBase);
            if (originalEntityTransform)
                context->entityTransform = &mirroredEntity;
            if (motionWork) {
                BindMirroredMotionControllerMatricesToModelNodes(motionWork, const_cast<MT7Model *>(context->model));
                motionWork->runtime.seqFlags &= ~MSF_ModelMatricesBound;
                Matrix4x4::SetActiveTransform(context->drawBase);
            }
            SetLegacyMirroredDrawMode(1);
        }
        const float drawAlpha = pass ? 1.0f : character->alpha;
        if (scripted && !matrixFrames && character->applyScale) {
            Character *const previousEntity = SetScriptedModelEntity(character);
            g_scriptedModelScale.x = character->scale.x;
            character->applyScale(context->model, context->model->nodes.data(), drawAlpha);
            SetScriptedModelEntity(previousEntity);
        } else {
            if (!matrixFrames)
                Matrix4x4::GetTransformMatrix(&g_scriptedIncomingTransform);
            context->draws += DrawMT7Model(*context->model, context->entityTransform, GetLegacyCharacterShaderPath(), drawAlpha, nullptr, matrixFrames,
                motionWork && !matrixFrames ? context->drawBase : nullptr);
        }
        if (!pass && motionWork && !matrixFrames)
            DrawMotionParts(*character, *motionWork, false);
        if (pass) {
            SetLegacyMirroredDrawMode(0);
            Matrix4x4::Pop(1);
            context->entityTransform = originalEntityTransform;
            if (motionWork)
                BindMotionControllerMatricesToModelNodes(character, motionWork, const_cast<MT7Model *>(context->model));
        }
    }
    if (fixedQueueDepth)
        SetLegacyQueuedDrawDepthMode(1, 0.0f);
    SetScriptedModelEntity(nullptr);
    if (motionDraw) {
        g_motionDrawOverrideFlags = 0;
        if (g_motionDrawOverridesEnabled)
            g_motionDeformationEnabled = 1;
    }
}

namespace {

// @verified - p
void MarkImageModelNodes(MT7Node *node) {
    for (; node; node = node->runtimeSibling) {
        node->id = static_cast<std::int32_t>((static_cast<std::uint32_t>(node->id) & 0xffffU) | 0x10000U);
        MarkImageModelNodes(node->runtimeChild);
    }
}

// @verified - p
void DrawImageModelSelection(Character *character, std::uint32_t flags, std::int32_t draw) {
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    if (!model)
        return;
    MT7Node *const root = model->nodes.data();
    root->id = static_cast<std::int32_t>((static_cast<std::uint32_t>(root->id) & 0xffffU) | 0x10000U);
    MarkImageModelNodes(root->runtimeChild);
    MarkImageModelNodes(root->runtimeSibling);
    MT7Node *group = root->runtimeChild;
    for (std::uint32_t index = (flags >> 8U) & 15U; index; --index)
        group = group->runtimeSibling;
    MT7Node *frame = group->runtimeChild;
    for (std::uint32_t index = flags & 0xffU; index; --index)
        frame = frame->runtimeSibling;
    Matrix4x4_Push(nullptr);
    ApplyRecursiveCharacterNodeTransforms(&character->entity.node);
    if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
        Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
    root->id = static_cast<std::int32_t>(static_cast<std::uint32_t>(root->id) & 0xffffU);
    group->id = static_cast<std::int32_t>(static_cast<std::uint32_t>(group->id) & 0xffffU);
    frame->id = static_cast<std::int32_t>(static_cast<std::uint32_t>(frame->id) & 0xffffU);
    const CharacterUpdateCallback applyScale = character->applyScale;
    if (draw) {
        g_scriptedModelScale.x = character->scale.x;
        applyScale(model, root, character->alpha);
        g_scriptedModelScale.x = character->scale.x;
        applyScale(model, group, character->alpha);
        g_scriptedModelScale.x = character->scale.x;
        ConstructMatricesForObjectRecursive(*model, frame);
        if (IsCharacterMirroredForDraw(character)) {
            Matrix4x4::Pop(1);
            Matrix4x4_Push(nullptr);
            Matrix4x4::MultiplyWithPrePassMatrix(&characterMirrorMatrix);
            ApplyRecursiveCharacterNodeTransforms(&character->entity.node);
            if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
                Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
            SetLegacyMirroredDrawMode(1);
            g_scriptedModelScale.x = character->scale.x;
            applyScale(model, root, 1.0f);
            g_scriptedModelScale.x = character->scale.x;
            applyScale(model, group, 1.0f);
            g_scriptedModelScale.x = character->scale.x;
            ConstructMatricesForObjectRecursive(*model, frame);
            SetLegacyMirroredDrawMode(0);
        }
    }
    Matrix4x4::Pop(1);
}

HLib::Task *characterDrawTask{};
std::int32_t npcBatchCountOverride{-1};
NpcFillType npcFillType{};
float characterHumanDrawBudget{10.0f};
std::int32_t npcUpdatedLastTick{};
std::uint32_t characterClipPointCount{};
std::int32_t npcPoolSizingModeOrOverride{-5};
Vector4 characterDrawVector{.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
bool mapAllowsAuthorSpawn{};
std::int32_t generatedEventCharacterCount{-1};
constexpr std::uint32_t scenarioStepModelMask = 0x7f;
constexpr std::array<const char *, scenarioStepModelCount> scenarioStepModelFiles{
    "LMALH00G.MT7",
    "TMEMH00G.MT7",
    "ABR0HMPG.MT7",
    "PSF72AWG.MT7",
    "PSF6200G.MT7",
    "PSF3200G.MT7",
    "HKSMH01G.MT7",
};

constexpr bool IsPlayer(const CharacterResource &resource) {
    return resource.scenarioName.name == SHEN_NAME(RYO_);
}
void PublishCharacterInMapControl(CharacterResource &resource, MapControlData &control, CharacterMapEntry &entry) {
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(resource.character) % control.characterTable.size();
    entry.next = static_cast<CharacterMapEntry *>(control.characterTable[bucket]);
    control.characterTable[bucket] = &entry;
    entry.handlerTask = resource.handlerTask;
    entry.character = resource.character;
    resource.mapEntry = &entry;
}

bool RegisterCharacterInMapControl(CharacterResource &resource) {
    if (!g_mapControlTask)
        return false;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control)
        return false;
    CharacterMapEntry *const entry = static_cast<CharacterMapEntry *>(HLib::AllocateTaskData(sizeof(CharacterMapEntry), TASK_CHAR));
    if (!entry)
        return false;
    PublishCharacterInMapControl(resource, *control, *entry);
    return true;
}

void UnregisterCharacterFromMapControl(CharacterResource &resource) {
    if (!resource.mapEntry || !g_mapControlTask)
        return;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(resource.character) % control->characterTable.size();
    auto *entry = static_cast<CharacterMapEntry *>(control->characterTable[bucket]);
    CharacterMapEntry *previous = nullptr;
    while (entry && entry != resource.mapEntry) {
        previous = entry;
        entry = entry->next;
    }
    if (!entry)
        return;
    if (previous) {
        previous->next = entry->next;
    } else {
        control->characterTable[bucket] = entry->next;
    }
    HLib::FreeTaskData(entry);
    resource.mapEntry = nullptr;
}

// @verified - p
void CharacterHandler_Callback(void *taskData) {
    auto *const character = static_cast<Character *>(taskData);
    if (!character || character->entity.name.name != CHARACTER_ENTITY.name)
        return;
    if ((character->flags & 0x24U) == CF_Active) {
        for (std::int16_t &angle : g_characterHandWristAngles)
            angle = 0;
        UpdateModelComponents(character);
    }
    if (!character->field94 && (character->position.x != 0.0f || character->position.y != 0.0f || character->position.z != 0.0f)) {
        character->entity.node.position.x += character->position.x;
        character->entity.node.position.y += character->position.y;
        character->entity.node.position.z += character->position.z;
    }
    character->position = {};
    character->field94 = 0;
}

Character *EnqueueCharacterHandler(std::uint8_t nextFunctionIndex, HLib::Task *&task) {
    task = HLib::EnqueueTaskWithParameter(CharacterHandler_Callback, nextFunctionIndex, 4, sizeof(Character), TASK_CHAR);
    Character *const character = static_cast<Character *>(HLib::GetTaskParameter(task));
    character->entity.name = CHARACTER_ENTITY;
    character->entity.node.child = nullptr;
    character->entity.node.position = {};
    character->entity.node.rotation[0] = 0;
    character->entity.node.rotation[1] = 0;
    character->entity.node.rotation[2] = 0;
    character->internalState = 0;
    character->field28 = 0.0f;
    character->field2c = 0.0f;
    character->position = {};
    character->core = charaCore;
    character->applyScale = charaApplyScale;
    character->prepareForDraw = charaPrepareNodesForDraw;
    character->collisionCheck = charaCollisionDistanceChecks;
    character->scale = {1.0f, 1.0f, 1.0f};
    character->alpha = 1.0f;
    character->field7c = 1.0f;
    character->field80 = 1.0f;
    character->field84 = 1.0f;
    character->radius = 1.8f;
    character->flags = CF_Valid | CF_Active | CF_Unknown40 | CF_DrawMirrored;
    character->model = nullptr;
    character->shadowModel = nullptr;
    character->drawDistance = 0.0f;
    character->viewPosition = {};
    character->field70 = 0;
    character->field74 = 0;
    character->field88 = 0;
    character->type = 0;
    character->shadowY = 0;
    character->field94 = 0;
    character->field99 = 0;
    character->fieldA0 = 0;
    character->modelComponentStorage = nullptr;
    character->modelComponents = nullptr;
    character->tokenTree = nullptr;
    character->auxiliaryComponentStorage = nullptr;
    character->field9a = 320;
    character->fieldC4 = -1;
    character->fieldC6 = -1;
    character->fieldE8 = 0x01010001U;
    for (unsigned index = 0; index < 6; ++index) {
        character->fieldEc[index] = -1;
        character->fieldF8[index] = 0;
        character->field110[index] = 0.0f;
    }
    character->facialModel = nullptr;
    character->useUnknownFlag = 0;
    character->data2 = nullptr;
    EV_SetCharDisplay(character, CVM_All, 1);
    EV_SetCharDisplay(character, CVM_DisplayEnabled, 0);
    return character;
}
CharacterResource *AddCharacterHandler(std::int32_t type) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    std::unique_ptr<CharacterResource> resource = std::make_unique<CharacterResource>();
    resource->character = EnqueueCharacterHandler(characterHandlerIndices[static_cast<std::size_t>(type)], resource->handlerTask);
    if (!resource->character) {
        if (resource->handlerTask)
            HLib::DestroyTask(resource->handlerTask);
        return nullptr;
    }
    CharacterMapEntry *const entry = static_cast<CharacterMapEntry *>(HLib::AllocateTaskData(sizeof(CharacterMapEntry), TASK_CHAR));
    PublishCharacterInMapControl(*resource, *control, *entry);
    CharacterResource *const admitted = resource.get();
    characters.push_back(std::move(resource));
    return admitted;
}

std::uint32_t ReadU32(const std::byte *source) {
    std::uint32_t value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

float FloatFromBits(std::uint32_t bits) {
    float value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

bool IsStringInChunk(const char *string, const std::byte *begin, const std::byte *end) {
    const auto *cursor = reinterpret_cast<const std::byte *>(string);
    if (cursor < begin || cursor >= end)
        return false;
    while (cursor < end) {
        if (*cursor == std::byte{})
            return true;
        ++cursor;
    }
    return false;
}

bool DecodeRecord(
    const std::byte *&cursor, const std::byte *recordEnd, const std::byte *chunkBegin, const std::byte *chunkEnd, Record &output, std::uint32_t depth) {
    if (depth >= 32 || cursor > recordEnd || static_cast<std::size_t>(recordEnd - cursor) < 8) {
        return false;
    }
    const std::byte *const header = cursor;
    const std::uint32_t tokenOffset = ReadU32(cursor);
    std::uint32_t argumentMask = ReadU32(cursor + 4);
    cursor += 8;
    if (tokenOffset > static_cast<std::uint64_t>(chunkEnd - header)) {
        return false;
    }
    const char *const token = reinterpret_cast<const char *>(header + tokenOffset);
    if (!IsStringInChunk(token, chunkBegin, chunkEnd))
        return false;
    output.token = token;

    while (argumentMask) {
        const std::uint32_t typeValue = argumentMask & 7U;
        argumentMask >>= 3U;
        if (typeValue < 1 || typeValue > 4 || cursor > recordEnd || static_cast<std::size_t>(recordEnd - cursor) < 4) {
            return false;
        }
        Argument argument{};
        argument.type = static_cast<ArgumentType>(typeValue);
        argument.value = ReadU32(cursor);
        if (argument.type == ArgumentType::String) {
            if (argument.value > static_cast<std::uint64_t>(chunkEnd - cursor)) {
                return false;
            }
            argument.string = reinterpret_cast<const char *>(cursor + argument.value);
            if (!IsStringInChunk(argument.string, chunkBegin, chunkEnd)) {
                return false;
            }
            cursor += 4;
        } else if (argument.type == ArgumentType::Records) {
            const std::uint64_t byteCount = static_cast<std::uint64_t>(argument.value) * 4U;
            cursor += 4;
            if (byteCount > static_cast<std::uint64_t>(recordEnd - cursor)) {
                return false;
            }
            const std::byte *const nestedEnd = cursor + byteCount;
            while (cursor < nestedEnd) {
                Record nested{};
                if (!DecodeRecord(cursor, nestedEnd, chunkBegin, chunkEnd, nested, depth + 1)) {
                    return false;
                }
                argument.records.push_back(std::move(nested));
            }
        } else {
            cursor += 4;
        }
        output.arguments.push_back(std::move(argument));
    }
    return true;
}

bool DecodeCharacterTable(std::span<const std::byte> table, std::vector<Record> &records) {
    if (table.size() < 8)
        return false;
    const std::uint32_t recordBytes = ReadU32(table.data() + 4);
    if (recordBytes < 8 || recordBytes > table.size() || (recordBytes & 3U)) {
        return false;
    }
    const std::byte *cursor = table.data() + 8;
    const std::byte *const recordEnd = table.data() + recordBytes;
    while (cursor < recordEnd) {
        Record record{};
        if (!DecodeRecord(cursor, recordEnd, table.data(), table.data() + table.size(), record, 0)) {
            return false;
        }
        records.push_back(std::move(record));
    }
    return true;
}

const Argument *FindArgument(const Record &record, ArgumentType type, std::size_t occurrence = 0) {
    for (const Argument &argument : record.arguments) {
        if (argument.type != type)
            continue;
        if (!occurrence--)
            return &argument;
    }
    return nullptr;
}

const std::vector<Record> *FindNestedRecords(const Record &record) {
    const Argument *const argument = FindArgument(record, ArgumentType::Records);
    return argument ? &argument->records : nullptr;
}


bool EqualCharacterToken(std::string_view left, std::string_view right) {
    if (left.size() != right.size())
        return false;
    for (std::size_t index = 0; index < left.size(); ++index) {
        auto fold = [](char value) { return value >= 'a' && value <= 'z' ? static_cast<char>(value - ('a' - 'A')) : value; };
        if (fold(left[index]) != fold(right[index]))
            return false;
    }
    return true;
}

const Record *FindRecord(const std::vector<Record> &records, std::string_view token) {
    const auto found = std::find_if(records.begin(), records.end(), [token](const Record &record) { return EqualCharacterToken(record.token, token); });
    return found == records.end() ? nullptr : &*found;
}
const Record *FindRecord(std::span<const Record *const> records, std::string_view token) {
    for (const Record *record : records) {
        if (record && EqualCharacterToken(record->token, token))
            return record;
    }
    return nullptr;
}

bool RecordContainsString(const Record &record, std::string_view value) {
    for (const Argument &argument : record.arguments) {
        if (argument.type == ArgumentType::String && EqualCharacterToken(argument.string, value))
            return true;
    }
    return false;
}

void ApplyCharacterTableFlags(Character *character, const Record &record) {
    for (const Argument &argument : record.arguments) {
        if (argument.type != ArgumentType::String)
            continue;
        const char *const value = argument.string;
        if (EqualCharacterToken(value, "DISP"))
            EV_SetCharDisplay(character, CVM_DisplayEnabled, 1);
        else if (EqualCharacterToken(value, "KILL"))
            ctrlSetKillAndIncrementCounter(character, 1);
        else if (EqualCharacterToken(value, "MAPEV"))
            ParseMapEv(character, 1);
        else if (EqualCharacterToken(value, "NOMAPEV"))
            SetFlagForToken(character, mapEventComponentToken, 0);
        else if (EqualCharacterToken(value, "NOCLIPPER"))
            SetFlagForToken(character, CHARACTER_CLIPPER_COMPONENT, 0);
        else if (EqualCharacterToken(value, "NOIMGMODULE"))
            SetFlagForToken(character, TASK_IMGM, 0);
        else if (EqualCharacterToken(value, "SLEEP")) {
            charaSetFlags(character, 0);
            HLib::SetupTimedTask(GetCharacterHandlerTask(character), 0);
        } else if (EqualCharacterToken(value, "NOTMIRRORED"))
            SetMirrored(character, 0);
    }
}

void ApplyAuthoredCharacterDisplay(Character &character, bool motionOnly, bool shadowOff, const Record *flags) {
    if (!motionOnly) {
        character.flags |= CF_DisplayCheckEnabled;
        if (shadowOff)
            character.flags &= ~CF_DisplayCheckEnabled;
    }
    if (!flags)
        return;
    for (const Argument &argument : flags->arguments) {
        if (argument.type == ArgumentType::String && EqualCharacterToken(argument.string, "DISP"))
            character.renderFlags |= CRF_DisplayEnabled;
    }
}

bool ReadArchiveFile(const char *path, std::vector<std::byte> &output) {
    const std::uint64_t size = ArchiveData_GetFileSize(path);
    if (!size || size > (std::numeric_limits<std::size_t>::max)())
        return false;
    output.resize(static_cast<std::size_t>(size));
    return ArchiveData_Read(path, 0, output.data(), output.size());
}

bool LoadScenarioStepModel(CharacterResource &resource, std::size_t modelIndex) {
    if (!resource.scenarioStepModels || modelIndex >= resource.scenarioStepModels->models.size()) {
        return false;
    }
    ScenarioStepModelResource &destination = resource.scenarioStepModels->models[modelIndex];
    if (destination.loaded)
        return true;

    std::array<char, 64> archivePath{};
    const int length = std::snprintf(archivePath.data(), archivePath.size(), "/misc/%s", scenarioStepModelFiles[modelIndex]);
    if (length < 0 || static_cast<std::size_t>(length) >= archivePath.size() || !ReadArchiveFile(archivePath.data(), destination.bytes) ||
        !ReadModelFormat7(destination.bytes, archivePath.data() + 1, destination.model)) {
        ReleaseModelFormat7(destination.model);
        destination.bytes.clear();
        return false;
    }

    destination.loaded = true;
    resource.scenarioStepModels->loadedMask |= std::uint32_t{1} << modelIndex;
    return true;
}

// @verified - p
bool ApplyCharacterTableNodeRecord(std::byte *node, const Record &record, std::uint32_t &mask) {
    if (EqualCharacterToken(record.token, "Position")) {
        const Vector3 position{FloatFromBits(record.arguments[0].value), FloatFromBits(record.arguments[1].value), FloatFromBits(record.arguments[2].value)};
        std::memcpy(node + 8, &position, sizeof(position));
        mask |= 1;
    } else if (EqualCharacterToken(record.token, "Angle")) {
        if (record.arguments.size() == 1) {
            const std::int32_t yaw = FloatToShort((FloatFromBits(record.arguments[0].value) * 8192.0f) / 45.0f);
            std::memcpy(node + 24, &yaw, sizeof(yaw));
        } else {
            std::int32_t rotation[3];
            for (unsigned index = 0; index < 3; ++index)
                rotation[index] = FloatToShort((FloatFromBits(record.arguments[index].value) * 8192.0f) / 45.0f);
            std::memcpy(node + 20, rotation, sizeof(rotation));
        }
        mask |= 2;
    } else if (EqualCharacterToken(record.token, "Layer")) {
        const float value = FloatFromBits(record.arguments[0].value);
        const std::int32_t layer = value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : INT32_MIN;
        std::memcpy(node + 32, &layer, sizeof(layer));
        mask |= 4;
    } else if (EqualCharacterToken(record.token, "Local")) {
        const Argument &argument = record.arguments[0];
        void *parent;
        if (argument.type == ArgumentType::Float) {
            const float value = FloatFromBits(argument.value);
            const std::int32_t index = value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : INT32_MIN;
            parent = GetCharacterInMapControlTask(index);
        } else
            parent = GetCharacterNode(FindCharacterByScenarioName(shen_name{argument.value}));
        std::memcpy(node, &parent, sizeof(parent));
        mask |= 8;
    } else if (EqualCharacterToken(record.token, "HasColi")) {
        const std::uint64_t zero = 0;
        std::memcpy(node + 36, &zero, sizeof(zero));
        mask |= 16;
    } else
        return false;
    return true;
}

void ReadCharacterTableComponents(Character &character, const std::vector<Record> &records, std::int32_t entryId, std::array<const Record *, 55> &components) {
    std::uint32_t baseMask = 0, entryMask = 0;
    CharacterControlSlot entryNode{};
    const Record *entryFlags = nullptr;
    const Record *entryGroup = nullptr;
    const Record *entryCommonMotionWork = nullptr;
    std::byte *const node = reinterpret_cast<std::byte *>(&character.entity.node);
    std::byte *const selectedNode = reinterpret_cast<std::byte *>(&entryNode);
    for (const Record &record : records) {
        if (ApplyCharacterTableNodeRecord(node, record, baseMask))
            continue;
        if (EqualCharacterToken(record.token, "Entry")) {
            if (entryId != static_cast<std::int32_t>(FloatFromBits(record.arguments[0].value)))
                continue;
            for (const Record &child : *FindNestedRecords(record)) {
                if (ApplyCharacterTableNodeRecord(selectedNode, child, entryMask))
                    continue;
                if (EqualCharacterToken(child.token, "Flags"))
                    entryFlags = &child;
                else if (EqualCharacterToken(child.token, "Group"))
                    entryGroup = &child;
                else if (EqualCharacterToken(child.token, "CommonMotionWork"))
                    entryCommonMotionWork = &child;
            }
        } else {
            for (unsigned index = 0; index < components.size(); ++index) {
                if (EqualCharacterToken(record.token, characterTableComponentTokens[index])) {
                    components[index] = &record;
                    break;
                }
            }
        }
    }
    if (entryFlags)
        components[3] = entryFlags;
    if (entryGroup)
        components[21] = entryGroup;
    if (entryCommonMotionWork)
        components[31] = entryCommonMotionWork;
    if (entryMask & 1)
        std::memcpy(node + 8, selectedNode + 8, 12);
    if (entryMask & 2)
        std::memcpy(node + 20, selectedNode + 20, 12);
    if (entryMask & 4)
        std::memcpy(node + 32, selectedNode + 32, 4);
    if (entryMask & 8)
        std::memcpy(node, selectedNode, 8);
    if (entryMask & 16)
        std::memcpy(node + 36, selectedNode + 36, 8);
}

MT7Model *FindPreloadedCharacterImageNullsub(const char *) {
    return nullptr;
}

void AssignModelImageDefinition(const Record &record, std::int32_t entryId, HLib::Task *task) {
    const Argument *const alias = FindArgument(record, ArgumentType::String);
    const std::vector<Record> *const records = FindNestedRecords(record);
    if (!alias || !records)
        return;
    ModelImageIndex *const image = AllocateModelImageIndex(task, alias->string);
    if (image->levelCount)
        return;
    std::uint32_t count = 0;
    std::int32_t load = 1, gate = 1;
    bool explicitGate = false;
    for (const Record &child : *records) {
        const bool avail = EqualCharacterToken(child.token, "Avail") || EqualCharacterToken(child.token, "Avail_Ext");
        if (avail || EqualCharacterToken(child.token, "NotAvail")) {
            gate = avail ? 0 : 1;
            if (!avail)
                explicitGate = true;
            for (const Argument &argument : child.arguments) {
                const bool matches = argument.type == ArgumentType::Float ?
                    FloatFromBits(argument.value) == static_cast<float>(entryId) :
                    argument.type == ArgumentType::Name && argument.value == static_cast<std::uint32_t>(entryId);
                if (matches) {
                    gate = avail ? 1 : 0;
                    explicitGate = avail;
                }
            }
        } else if (EqualCharacterToken(child.token, "Dynamic"))
            gate = 2;
        else
            ++count;
    }
    if (!count)
        return;
    if (!explicitGate)
        load = gate;
    image->levelCount = count;
    image->levels = static_cast<ModelImageLevel *>(HLib::AllocateTaskData(count * sizeof(ModelImageLevel), MEMORY_MAIL));
    image->pak = GetPakLoadParams() ? pakLoadGetNext() : nullptr;
    ModelManagerTaskData *const manager = static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(task));
    std::uint32_t assigned = 0;
    for (const Record &child : *records) {
        if (EqualCharacterToken(child.token, "Avail") || EqualCharacterToken(child.token, "NotAvail") || EqualCharacterToken(child.token, "Dynamic") ||
            EqualCharacterToken(child.token, "Avail_Ext"))
            continue;
        const Argument *const distance = FindArgument(child, ArgumentType::Float);
        const Argument *const filename = FindArgument(child, ArgumentType::String);
        const float authoredThreshold = FloatFromBits(distance->value);
#if SHENMUE_IS_PC
        const std::int32_t threshold = _mm_cvttss_si32(_mm_set_ss(authoredThreshold));
#else
        const std::int32_t threshold = authoredThreshold >= -2147483648.0f && authoredThreshold < 2147483648.0f ? static_cast<std::int32_t>(authoredThreshold) :
                                                                                                                  std::numeric_limits<std::int32_t>::min();
#endif
        std::uint32_t index = 0;
        while (index < assigned && std::bit_cast<std::int32_t>(image->levels[index].threshold) < threshold)
            ++index;
        for (std::uint32_t move = assigned; move > index; --move)
            image->levels[move] = image->levels[move - 1];
        ModelImageLevel &level = image->levels[index];
        level.threshold = static_cast<std::uint32_t>(threshold);
        const char *source = filename->string;
        char directory[64];
        if (*source == '$') {
            ++source;
            std::snprintf(directory, sizeof(directory), "scene/%02d/%.4s", g_actNumber, g_currentMap.characters);
        } else {
            if (*source == '@')
                ++source;
            const bool hand = source[3] == '_' && source[4] == 'U' && (source[5] == 'L' || source[5] == 'R') && source[6] == '.';
            std::snprintf(directory, sizeof(directory), hand ? "scene/%02d/model/hand" : "scene/%02d/model/chara", g_actNumber);
        }
        const std::size_t size = std::strlen(source) + 1;
        level.filename = reinterpret_cast<char *>(manager->managerState) + manager->entryCount;
        std::memcpy(level.filename, source, size);
        manager->entryCount += static_cast<std::uint32_t>(size);
        level.model = nullptr;
        if (load == 1) {
            level.model = FindPreloadedCharacterImageNullsub(level.filename);
            if (!level.model) {
                if (image->pak)
                    level.model = LoadGameModel(static_cast<PakGlobalData *>(image->pak), level.filename);
                else
                    QueueModelImageFileRead(&level.model, directory, level.filename);
            }
        }
        ++assigned;
    }
}

void scenCreateModel(Character *character, const Record &shadow) {
    const std::vector<Record> *const records = FindNestedRecords(shadow);
    if (!records)
        return;
    const shen_name name = GetCharacterScenarioName(character);
    const bool hat = name.characters[0] == 'h' && name.characters[1] == 'a' && name.characters[2] == 't';
    for (const Record &record : *records) {
        if (EqualCharacterToken(record.token, "Image")) {
            SetCharacterType(character, CharacterTypePlayerInverted);
            character->shadowModel = CreateMT7(FindArgument(record, ArgumentType::String)->string);
        } else if (EqualCharacterToken(record.token, "Mode")) {
            const float value = FloatFromBits(FindArgument(record, ArgumentType::Float)->value);
            const std::int32_t mode =
                value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : std::numeric_limits<std::int32_t>::min();
            SetCharacterType(character, mode >= 12 ? 0 : mode);
        } else if (EqualCharacterToken(record.token, "Position") || EqualCharacterToken(record.token, "Scale")) {
            Vector3 value{FloatFromBits(FindArgument(record, ArgumentType::Float, 0)->value),
                FloatFromBits(FindArgument(record, ArgumentType::Float, 1)->value), FloatFromBits(FindArgument(record, ArgumentType::Float, 2)->value)};
            const bool scale = EqualCharacterToken(record.token, "Scale");
            if (hat)
                value = scale ? Vector3{.2f, .2f, .5f} : Vector3{0, 1, 0};
            if (scale)
                SetCharacterShadowScale(character, &value);
            else
                SetCharacterShadowOffset(character, &value);
        } else if (EqualCharacterToken(record.token, "Yang")) {
            SetCharacterShadowYaw(character, FloatToShort((FloatFromBits(FindArgument(record, ArgumentType::Float)->value) * 8192.0f) / 45.0f));
        } else if (EqualCharacterToken(record.token, "Height")) {
            SetCharacterStoredShadowYEnabled(character, 1);
            SetCharacterShadowY(character, FloatFromBits(FindArgument(record, ArgumentType::Float)->value));
        }
    }
}

void AssignCharacterTableImage(Character *character, const Record &record, HLib::Task *manager) {
    SetupImageModuleHandlers(character);
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM));
    std::int32_t loop = -1;
    std::uint32_t count = 0;
    float wait = 1;
    std::uint8_t kind = 0;
    if (const std::vector<Record> *const records = FindNestedRecords(record)) {
        for (const Record &child : *records) {
            if (EqualCharacterToken(child.token, "Kind")) {
                const Argument *const value = FindArgument(child, ArgumentType::String);
                kind = EqualCharacterToken(value->string, "NONE") ? 0 : EqualCharacterToken(value->string, "REPEAT") ? 1 : 2;
            } else if (const Argument *const value = FindArgument(child, ArgumentType::Float)) {
                if (EqualCharacterToken(child.token, "Loop"))
                    loop = static_cast<std::int32_t>(FloatFromBits(value->value));
                else if (EqualCharacterToken(child.token, "Wait")) {
                    wait = FloatFromBits(value->value);
                    if (wait < .1f)
                        wait = 1;
                } else
                    count = static_cast<std::uint32_t>(static_cast<std::int32_t>(FloatFromBits(value->value)));
            }
        }
    }
    image->field2c = wait;
    image->forceFirst = kind;
    image->rigType &= 0xffffff00U;
    image->field18 = static_cast<std::uint32_t>(loop);
    image->field24 = 0;
    image->field28 = 0;
    image->field30 = 0;
    image->field20 = count;
    const Argument *const name = FindArgument(record, ArgumentType::String);
    SetupImageModuleVariants(character, manager, name->string, count, 0);
}

CharacterResource *AllocateCharacterTableResource(std::int32_t characterType, bool named) {
    std::unique_ptr<CharacterResource> storage = std::make_unique<CharacterResource>();
    CharacterResource *const resource = storage.get();
    resource->character = EnqueueCharacterHandler(characterHandlerIndices[static_cast<std::size_t>(characterType)], resource->handlerTask);
    if (!resource->character)
        return nullptr;
    if (!RegisterCharacterInMapControl(*resource)) {
        HLib::DestroyTask(resource->handlerTask);
        return nullptr;
    }
    if (named)
        resource->scenarioName = currentCharacterTableName;
    characters.push_back(std::move(storage));
    if (named) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (!AssignScenarioCharacter(control->scenarioTask, resource->scenarioName, resource->character)) {
            FreeCharacter(resource->character);
            return nullptr;
        }
    }
    return resource;
}

// @verified - p
void ApplyCharacterTableCollisionFlags(Character *character, const Record *record) {
    CharacterControlModel *const control = FindCharacterControlModel(character);
    if (!record)
        return;
    control->flags &= 0xfffffff2U;
    if (record->arguments.size() != 1 || record->arguments.front().type != ArgumentType::Records)
        return;
    for (const Record &child : record->arguments.front().records) {
        if (!EqualCharacterToken(child.token, "Adjust"))
            continue;
        for (const Argument &argument : child.arguments) {
            if (EqualCharacterToken(argument.string, "MAP"))
                control->flags |= CharacterControlEvaluateMotion;
            else if (EqualCharacterToken(argument.string, "CHAR"))
                control->flags |= CharacterControlVoxelSweep;
            else if (EqualCharacterToken(argument.string, "FLOAT"))
                control->flags &= ~CharacterControlApplyGravity;
            else if (EqualCharacterToken(argument.string, "GRAVITY"))
                control->flags |= CharacterControlApplyGravity;
            else if (EqualCharacterToken(argument.string, "NONE")) {
                control->flags &= 0xfffffff2U;
                break;
            }
        }
    }
}

bool ApplyCharacterTableComponents(CharacterResource *resource, const std::vector<Record> &authoredComponents, shen_name owner, std::int32_t entryId) {
    Character &character = *resource->character;
    std::array<const Record *, 55> componentRecords{};
    const std::array<const Record *, 55> *const components = &componentRecords;
    ReadCharacterTableComponents(character, authoredComponents, entryId, componentRecords);
    const bool hasPlayerComponent = FindRecord(*components, "Player") != nullptr;
    const bool hasHumanComponent = FindRecord(*components, "Human") != nullptr;
    const bool motionOnly = FindRecord(*components, "MotOnly") != nullptr;
    const Record *const flags = FindRecord(*components, "Flags");
    const bool hasMapEventComponent = hasPlayerComponent || (flags && RecordContainsString(*flags, "MAPEV"));
    resource->human = hasHumanComponent;
    character.type = 0;

    ApplyAuthoredCharacterDisplay(character, motionOnly, FindRecord(*components, "ShadowOff") != nullptr, nullptr);
    const Record *const image = FindRecord(*components, "Image");
    if (image) {
        if (const Argument *value = FindArgument(*image, ArgumentType::String)) {
            resource->image = value->string;
        }
    }
    if (const Record *hand = FindRecord(*components, "Hand")) {
        if (const Argument *left = FindArgument(*hand, ArgumentType::String, 0)) {
            resource->leftHand = left->string;
        }
        if (const Argument *right = FindArgument(*hand, ArgumentType::String, 1)) {
            resource->rightHand = right->string;
        }
    }

    if (image) {
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        AssignCharacterTableImage(&character, *image, control->modelManagerTask);
        IMGM_AssignMTWK(&character);
        const ImageModuleComponent *const module = static_cast<const ImageModuleComponent *>(FindModelComponent(&character, TASK_IMGM));
        const ModelImageIndex *const source = module->entries[module->selected]->image;
        if (source->levelCount)
            resource->image = source->levels[0].filename;
    }
    character.shadowModel = const_cast<MT7Model *>(GetSHDS());
    charaSetDisplayEnabledFlag(&character, motionOnly ? 0 : 1);
    if (!motionOnly && FindRecord(*components, "ShadowOff"))
        charaSetDisplayEnabledFlag(&character, 0);
    if (!motionOnly)
        if (const Record *const shadow = FindRecord(*components, "Shadow"))
            scenCreateModel(&character, *shadow);
    if (motionOnly)
        AllocateMotionWork(&character);
    if (!motionOnly && character.model) {
        MT7Node *const root = &static_cast<MT7Model *>(character.model)->nodes.front();
        if (FindRecord(*components, "CutMvShadow"))
            InitializeObject(root);
        else if (!hasHumanComponent && !FindRecord(*components, "Hand"))
            SetupModelForImageModule(root);
    }
    if (!motionOnly && hasHumanComponent && hasPlayerComponent && !AttachPlayerModel(&character)) {
        FreeCharacter(&character);
        return false;
    }
    if (!motionOnly && hasHumanComponent) {
        MotionWork *const motionWork = AllocateMotionWork(&character);
        if (!motionWork) {
            FreeCharacter(&character);
            return false;
        }
        if (FindRecord(*components, "Party") && !AttachPartyCompanion(&character)) {
            FreeCharacter(&character);
            return false;
        }
        if (resource->scenarioName.name == CHARACTER_BLUR_RYO.name) {
            SetBlurModel(&character, "RYO_B.MT7", 1.0f);
        } else if (const Record *fight = FindRecord(*components, "Fight")) {
            mtSetupModelFigurePose(&character);
            if (const std::vector<Record> *records = FindNestedRecords(*fight)) {
                for (const Record &blur : *records) {
                    if (!EqualCharacterToken(blur.token, "Blur"))
                        continue;
                    const Argument *const filename = FindArgument(blur, ArgumentType::String);
                    if (!filename)
                        continue;
                    const Argument *const duration = FindArgument(blur, ArgumentType::Float);
                    const float time = duration ? FloatFromBits(duration->value) : 0.0f;
                    mtFindBlurModel(&character, filename->string, duration ? &time : nullptr);
                }
            }
        }
        if (FindRecord(*components, "Handcuffs"))
            Handcuffs_Enqueue();
        if (const Record *step = FindRecord(*components, "Step")) {
            const Argument *const mode = FindArgument(*step, ArgumentType::Float);
            const std::int8_t authoredMode = mode ? static_cast<std::int8_t>(static_cast<std::int32_t>(FloatFromBits(mode->value))) : 0;
            AttachFootstepComponent(&character, FindArgument(*step, ArgumentType::String)->string, authoredMode);
        }
        if (const Record *rig = FindRecord(*components, "Bone"))
            InitRigType(&character, FindArgument(*rig, ArgumentType::String)->string);
        if (!AttachCharacterPresentationComponents(&character, !hasPlayerComponent, FindRecord(*components, "RefBoard") != nullptr)) {
            FreeCharacter(&character);
            return false;
        }
        if (hasPlayerComponent)
            EV_mtwkSetEnabled(motionWork, 1);
        if (hasPlayerComponent || resource->scenarioName.name == CHARACTER_SHENHUA.name)
            SetPluralControlFor(&character, 0);
        if (resource->scenarioName.name == CHARACTER_BLUR_RYO.name)
            character.fieldE8 |= 0x10U;
        for (shen_name name : characterDedicatedControllers) {
            if (resource->scenarioName.name == name.name) {
                character.fieldE8 |= 0x10U;
                break;
            }
        }
        AllocateScenarioFreeTalkCharacter(&character);
        character.flags |= CF_NpcLoggerTarget;
    } else if (!motionOnly && FindRecord(*components, "Object")) {
        if (!AttachObjectXFormsComponent(&character)) {
            FreeCharacter(&character);
            return false;
        }
        if (FindRecord(*components, "Knife"))
            imgmAssignUpdateCallback(&character, charaPrepareNodesForDraw);
        if (FindRecord(*components, "ForceTrans"))
            imgmAssignUpdateCallback(&character, DrawPreparedVertexColours);
        if ((FindRecord(*components, "ProjectionTrans") && !AttachCharacterAlphaComponent(&character)) ||
            (FindRecord(*components, "TailLamp") && !AttachTailLampComponent(&character)) ||
            (FindRecord(*components, "TinyGame") && !AttachGameDisplayComponent(&character)) ||
            (FindRecord(*components, "RefBoard") && !AttachReflectorBoardComponent(&character, 6))) {
            FreeCharacter(&character);
            return false;
        }
        if (const Record *blur = FindRecord(*components, "ObjBlur")) {
            const Argument *const duration = FindArgument(*blur, ArgumentType::Float);
            const Argument *const filename = FindArgument(*blur, ArgumentType::String);
            AttachObjectBlur(&character, static_cast<std::uint32_t>(blur->arguments.size()), duration ? FloatFromBits(duration->value) : 0.0f,
                filename ? filename->string : nullptr);
        }
        AttachFlagWindComponent(&character);
    }
    if (!motionOnly) {
        MT7Model *const source = GetImageModuleSource(&character);
        character.facialModel = source && source->facial ? &source->facial->data : nullptr;
        if (character.facialModel)
            GraftFacialModelNode(*static_cast<MT7Model *>(character.model), *source->facial);
        if (!AttachFacialController(&character)) {
            FreeCharacter(&character);
            return false;
        }



        bool assignCloth = true;
        std::int8_t clothMode = 0;
        if (const Record *cloth = FindRecord(*components, "Cloth"); cloth && !cloth->arguments.empty()) {
            const Argument &argument = cloth->arguments.front();
            if (argument.type == ArgumentType::Float) {
                clothMode = static_cast<std::int8_t>(static_cast<std::uint8_t>(FloatToShort(FloatFromBits(argument.value))));
            } else if (argument.string) {
                const char *text = argument.string;
                assignCloth = !((text[0] == 'O' || text[0] == 'o') && (text[1] == 'N' || text[1] == 'n') && text[2] == '\0');
            }
        }
        if (assignCloth)
            mtAssignClothFlags(&character, clothMode);
        mtAssignOsage(&character);
        if (const Record *clock = FindRecord(*components, "Clock")) {
            const Argument *const argument = clock->arguments.empty() ? nullptr : &clock->arguments.front();
            const std::uint32_t bits = !argument       ? 0 :
                argument->type == ArgumentType::String ? static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(argument->string)) :
                                                         argument->value;
            const float value = FloatFromBits(bits);
            const std::int32_t mode =
                value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : std::numeric_limits<std::int32_t>::min();
            nullsub(&character, mode);
        }
        if (!FindRecord(*components, "ColiOff") && (hasHumanComponent || FindRecord(*components, "ColiOn"))) {
            if (!AttachCharacterControlModel(&character)) {
                FreeCharacter(&character);
                return false;
            }
            ApplyCharacterTableCollisionFlags(&character, FindRecord(*components, "Coli"));
            if (!hasHumanComponent)
                FindCharacterControlModel(&character)->flags |=
                    CharacterControlUpdateHeight | CharacterControlVoxelSweep | CharacterControlUnknown10 | CharacterControlStairMode;
        }
        if (const Record *const hand = FindRecord(*components, "Hand"); hand || (hasHumanComponent && hasPlayerComponent)) {
            const std::uint8_t handFlags =
                hand && hand->arguments.size() == 3 ? static_cast<std::uint8_t>(FloatToShort(FloatFromBits(hand->arguments[2].value))) : 0;
            AttachCharacterHandComponents(&character, resource->image.c_str(), hand ? resource->leftHand.c_str() : "RYO_TL.MT7",
                hand ? resource->rightHand.c_str() : "RYO_TR.MT7", handFlags);
            if (hasHumanComponent && hasPlayerComponent) {
                SetLowQualityHands(&character);
                if (g_actNumber == 4 && (g_currentMap.name & 0xffU) == 'K') {
                    SetCharacterHandRegionEnabled(&character, 1, true, 1);
                    SetCharacterHandRegionEnabled(&character, 0, true, 1);
                }
            }
        }
        if (const Record *range = FindRecord(*components, "Range"))
            SetCharacterRadius(&character, FloatFromBits(range->arguments.front().value));
    }


    if (FindRecord(*components, "TinyMot") && !FindRecord(*components, "MotOnly"))
        AllocateTMNM(&character);
    if (FindRecord(*components, "DummyChara"))
        charaAssignDummyCore(&character);
    if (hasMapEventComponent) {
        if (!AttachMapEventComponent(&character)) {
            FreeCharacter(&character);
            return false;
        }
        ParseMapEv(&character, 1);
    }
    if (flags)
        ApplyCharacterTableFlags(&character, *flags);
    if (const Record *group = FindRecord(*components, "Group")) {
        if (!group->arguments.empty()) {
            const Argument &value = group->arguments.front();
            if (value.type == ArgumentType::Name)
                SetCharacterModelName(&character, shen_name{value.value});
            else if (value.type == ArgumentType::String)
                SetCharacterModelName(&character, shen_name{ReadU32(reinterpret_cast<const std::byte *>(value.string))});
        }
    } else {
        SetCharacterModelName(&character, owner);
    }
    return true;
}

bool CreateCharacter(const Record &record, shen_name owner, std::int32_t entryId) {
    Character *character = nullptr;
    std::int32_t characterType = 2;
    bool named = false;
    for (const Argument &argument : record.arguments) {
        switch (argument.type) {
        case ArgumentType::Float:
            characterType = static_cast<std::int32_t>(FloatFromBits(argument.value));
            break;
        case ArgumentType::Name:
            named = true;
            currentCharacterTableName = shen_name{argument.value};
            break;
        case ArgumentType::String:
            if (EqualCharacterToken(argument.string, "HIGH"))
                characterType = 1;
            else if (EqualCharacterToken(argument.string, "LOW"))
                characterType = 3;
            break;
        case ArgumentType::Records:
            if (named)
                character = FindCharacterByScenarioName(currentCharacterTableName);
            if (!character) {
                CharacterResource *const resource = AllocateCharacterTableResource(characterType, named);
                if (!resource || !ApplyCharacterTableComponents(resource, argument.records, owner, entryId))
                    return false;
                character = resource->character;
            }
            break;
        }
    }
    return character || AllocateCharacterTableResource(characterType, named);
}

} // namespace

// @verified - p
void DrawImageCharacterModel(Character *character, std::int32_t draw) {
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM));
    const float wait = image->field2c;
    float accumulator = image->field30;
    std::uint32_t flags = (image->forceFirst == 2 ? 0x12000U : 0x11000U) | image->field24 | (image->field20 << 8U);
    if (!(g_loadingFlag & 2U) && image->field18) {
        for (accumulator += 1.0f; accumulator >= 1.0f; accumulator -= wait) {
            flags = AdvanceImageModelFrame(static_cast<MT7Model *>(character->model)->nodes.data(), flags, &image->field28);
            if (!(flags & 0x1000ffU) && image->field18)
                --image->field18;
        }
    }
    DrawImageModelSelection(character, flags, draw);
    image->field30 = accumulator;
    image->field24 = flags;
}

// @verified - p
void DrawImageCharacterCore(Character *character) {
    DrawImageCharacterModel(character, 1);
}

bool ResolveCharacterWorldTransform(const Character *character, Matrix4x4 &transform) {
    if (!character)
        return false;
    transform = CharacterEntityTransform(*character);
    return true;
}

bool ResolveCharacterDrawNodeTransform(const Character *character, std::uint32_t nodeId, Matrix4x4 &transform) {
    if (!character)
        return false;

    const CharacterCoreDrawContext *const context = currentCharacterCoreDrawContext;
    if (const Matrix4x4 *const frame = GetMotionDrawOverrideData(FindMotionWork(character))) {
        if (!character->model)
            return false;
        const auto &model = *static_cast<const MT7Model *>(character->model);
        const Matrix4x4 *const local = FindMotionDrawOverrideNodeTransform(model, frame, nodeId);
        if (!local)
            return false;
        const Matrix4x4 world =
            context && context->character == character && context->entityTransform ? *context->entityTransform : CharacterEntityTransform(*character, false);
        transform = MultiplyTransforms(*local, world);
        return true;
    }
    if (const MotionWork *const motionWork = FindMotionWork(character)) {
        MotionPose pose{};
        if (EvaluateCurrentMotionPose(motionWork, pose) && nodeId < pose.hasNodeMatrix.size() && pose.hasNodeMatrix[nodeId]) {
            transform = MultiplyTransforms(pose.nodeMatrices[nodeId], CharacterEntityTransform(*character));
            return true;
        }
    }

    const CharacterResource *const resource = FindCharacterResource(character);
    return resource && ResolveStaticModelNodeTransform(*resource, nodeId, transform);
}

bool ResolveCharacterDrawNodeTranslation(const Character *character, std::uint32_t nodeId, Vector3 &translation) {
    Matrix4x4 transform{};
    if (!ResolveCharacterDrawNodeTransform(character, nodeId, transform) && !ResolveCharacterWorldTransform(character, transform)) {
        return false;
    }
    translation = {
        transform.rows[3].x,
        transform.rows[3].y,
        transform.rows[3].z,
    };
    return true;
}
// @verified - p
shen_name CreateEventCharacter(std::int32_t type, const std::byte *data, shen_name name) {
    CharacterResource *const resource = AddCharacterHandler(type);
    if (!resource)
        return {};
    Character *const character = resource->character;
    if (!name.name) {
        do {
            char generatedName[5];
            std::snprintf(generatedName, sizeof(generatedName), " %03d", generatedEventCharacterCount);
            std::memcpy(&name.name, generatedName, sizeof(name.name));
            if (++generatedEventCharacterCount >= 1000)
                generatedEventCharacterCount = 0;
            const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            if (!GetScenarioCharacterNode(control->scenarioTask, name))
                break;
        } while (true);
    }
    resource->scenarioName = name;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    CreateScenarioCharacterId(control->scenarioTask, name, character);
    SetupImageModuleHandlers(character);
    if (data) {
        const MapControlData *const imageControl = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        SetupImageModule(character, imageControl->modelManagerTask, reinterpret_cast<const char *>(data), 0);
        MT7Model *const model = static_cast<MT7Model *>(character->model);
        if (model)
            SetupModelForImageModule(model->nodes.data());
    }
    return name;
}

Character *AddHighModelCharacter(shen_name scenarioName, HLib::Task *&handlerTask) {
    handlerTask = nullptr;
    auto resource = std::make_unique<CharacterResource>();
    resource->character = EnqueueCharacterHandler(characterHandlerIndices[CharacterCreationHighModel], resource->handlerTask);
    if (!resource->character) {
        if (resource->handlerTask)
            HLib::DestroyTask(resource->handlerTask);
        return nullptr;
    }
    if (!RegisterCharacterInMapControl(*resource)) {
        HLib::DestroyTask(resource->handlerTask);
        return nullptr;
    }

    resource->scenarioName = scenarioName;
    Character *const character = resource->character;
    handlerTask = resource->handlerTask;
    characters.push_back(std::move(resource));
    BindNpcRouteCharacter(scenarioName, character);
    return character;
}

bool ApplyNpcPackageCharacterDisplay(Character *character, std::span<const std::byte> characterTable) {
    const CharacterResource *const resource = FindCharacterResource(character);
    if (!resource)
        return false;
    std::vector<Record> records;
    if (!DecodeCharacterTable(characterTable, records))
        return false;
    for (const Record &record : records) {
        if (record.token != "Character")
            continue;
        const Argument *const name = FindArgument(record, ArgumentType::Name);
        if (!name || name->value != resource->scenarioName.name)
            continue;
        const std::vector<Record> *const components = FindNestedRecords(record);
        if (!components)
            return false;
        ApplyAuthoredCharacterDisplay(
            *character, FindRecord(*components, "MotOnly") != nullptr, FindRecord(*components, "ShadowOff") != nullptr, FindRecord(*components, "Flags"));
        return true;
    }
    return false;
}

bool ConfigureScenarioStepHighModelCharacter(Character *character, const ScenarioStepHighModelState &state) {
    if (!character || character->model || (state.modelLoadMask & ~scenarioStepModelMask) != 0 || state.nativeMode < -1 ||
        state.nativeMode >= static_cast<std::int32_t>(scenarioStepModelCount)) {
        return false;
    }

    const bool unresolved = state.selectedAsset == ScenarioStepModelAsset::Unresolved;
    const std::size_t selectedIndex = static_cast<std::size_t>(state.selectedAsset);
    if ((unresolved && state.nativeMode != -1) ||
        (!unresolved &&
            (selectedIndex >= scenarioStepModelCount || state.nativeMode != static_cast<std::int32_t>(selectedIndex) ||
                (state.modelLoadMask & (std::uint32_t{1} << selectedIndex)) == 0))) {
        return false;
    }

    const auto resource = std::find_if(characters.begin(), characters.end(),
        [character](const std::unique_ptr<CharacterResource> &candidate) { return candidate && candidate->character == character; });
    if (resource == characters.end() || (*resource)->hasPendingHighModel) {
        return false;
    }

    if (!AttachReflectorBoardComponent(character, 2))
        return false;

    if (!AttachObjectXFormsComponent(character))
        return false;

    (*resource)->scenarioStepModels = std::make_unique<ScenarioStepModelSet>();
    for (std::size_t modelIndex = 0; modelIndex < scenarioStepModelCount; ++modelIndex) {
        if ((state.modelLoadMask & (std::uint32_t{1} << modelIndex)) != 0) {
            LoadScenarioStepModel(**resource, modelIndex);
        }
    }

    ScenarioStepHighModelState configured = state;
    configured.modelLoadedMask = (*resource)->scenarioStepModels->loadedMask;
    if (!unresolved && (configured.modelLoadedMask & (std::uint32_t{1} << selectedIndex)) != 0) {
        character->model = &(*resource)->scenarioStepModels->models[selectedIndex].model;
        const auto &model = *static_cast<const MT7Model *>(character->model);
        character->facialModel = model.facial ? &model.facial->data : nullptr;
    }
    (*resource)->pendingHighModel = configured;
    (*resource)->hasPendingHighModel = true;

    // REFB and FIXO are attached before the high-model resources are loaded.
    // FIXO remains inactive until the scenario supplies an attachment source.
    character->alpha = 0.0f;
    return true;
}

// SetModePerCurrentStep's model swap: the mode indexes the step-model objects, which stay null when not loaded.
void SetScenarioStepModel(Character *character, std::int32_t mode) {
    for (const std::unique_ptr<CharacterResource> &resource : characters) {
        if (!resource || resource->character != character)
            continue;
        if (!resource->scenarioStepModels || mode < 0 || static_cast<std::size_t>(mode) >= scenarioStepModelCount)
            return;
        const bool loaded = (resource->scenarioStepModels->loadedMask & (std::uint32_t{1} << mode)) != 0;
        MT7Model *const model = loaded ? &resource->scenarioStepModels->models[static_cast<std::size_t>(mode)].model : nullptr;
        character->model = model;
        character->facialModel = model && model->facial ? &model->facial->data : nullptr;
        return;
    }
}

bool GetScenarioStepHighModelState(const Character *character, ScenarioStepHighModelState &state) {
    const auto resource = std::find_if(characters.begin(), characters.end(),
        [character](const std::unique_ptr<CharacterResource> &candidate) { return candidate && candidate->character == character; });
    if (resource == characters.end() || !(*resource)->hasPendingHighModel) {
        return false;
    }
    state = (*resource)->pendingHighModel;
    return true;
}

CharacterControlSlot *GetCharacterInMapControlTask(std::int32_t characterId) {
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control->characterControlTable.data() + characterId;
}

HLib::Task *GetCharacterHandlerTask(const Character *character) {
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control)
        return nullptr;
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % control->characterTable.size();
    for (auto *entry = static_cast<CharacterMapEntry *>(control->characterTable[bucket]); entry; entry = entry->next) {
        if (entry->character == character)
            return entry->handlerTask;
    }
    return nullptr;
}

void ForEachLiveCharacterInMapOrder(CharacterMapVisitor visitor, void *context) {
    if (!visitor || !g_mapControlTask)
        return;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control)
        return;

    for (void *const head : control->characterTable) {
        for (auto *entry = static_cast<CharacterMapEntry *>(head); entry; entry = entry->next) {
            Character *const character = entry->character;
            if (!character || character->entity.name.name != SHEN_NAME(CEWP) || (character->flags & CF_Valid) == 0) {
                continue;
            }
            visitor(character, context);
        }
    }
}

// @verified - p
std::int32_t GetCharacterBoneId(const Character *character) {
    return GetMotionBoneIdForEntityType(GetCharacterImageSourceType(character));
}

// @verified - p
shen_name GetCharacterScenarioName(const Character *character) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control ? scenGetCharaName(control->scenarioTask, const_cast<Character *>(character)) : shen_name{};
}

Character *FindCharacterByScenarioName(shen_name name) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control ? FindScenarioCharacter(control->scenarioTask, name) : nullptr;
}
// @verified - p
MotionWork *scenFindMotionwork(shen_name name) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    return character ? FindMotionWork(character) : nullptr;
}

void EV_SetCharacterScale(shen_name name, const Vector3 &scale) {
    Character *const character = FindCharacterByScenarioName(name);
    if (character && !FindMotionWork(character))
        character->scale = scale;
}



bool CHRS_Reader(const MapInfoChunk *chrd, std::int32_t entryId, shen_name chunkName) {
    if (!chrd || chrd->nextByteOffset < 16)
        return false;
    const std::byte *const payload = reinterpret_cast<const std::byte *>(chrd) + sizeof(MapInfoChunk);
    return CHRS_ReadTable({payload, chrd->nextByteOffset - sizeof(MapInfoChunk)}, entryId, chunkName);
}

bool CHRS_ReadTable(std::span<const std::byte> table, std::int32_t entryId, shen_name owner) {
    if (table.size() < 8 || ReadU32(table.data()) != SHEN_NAME(CHRS)) {
        return false;
    }
    std::vector<Record> records;
    if (!DecodeCharacterTable(table, records))
        return false;

    const bool package = GetPakLoadParams() != 0;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!package) {
        BeginModelImageFileReads();
        for (const Record &record : records)
            if (EqualCharacterToken(record.token, "DefImage"))
                AssignModelImageDefinition(record, entryId, control->modelManagerTask);
        FinishModelImageFileReads();
    }
    for (const Record &record : records) {
        if (EqualCharacterToken(record.token, "DefImage") && package) {
            AssignModelImageDefinition(record, entryId, control->modelManagerTask);
        }
        if (EqualCharacterToken(record.token, "Character")) {
            CreateCharacter(record, owner, entryId);
        }
        if (EqualCharacterToken(record.token, "LocalPos")) {
            const float value = FloatFromBits(record.arguments[0].value);
            const std::int32_t index = value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : INT32_MIN;
            std::byte *const node = reinterpret_cast<std::byte *>(GetCharacterInMapControlTask(index));
            std::uint32_t mask = 0;
            for (const Record &child : *FindNestedRecords(record))
                ApplyCharacterTableNodeRecord(node, child, mask);
        }
    }
    return true;
}

void FreeCharacter(Character *character) {
    const auto resource = std::find_if(characters.begin(), characters.end(),
        [character](const std::unique_ptr<CharacterResource> &candidate) { return candidate && candidate->character == character; });
    if (resource == characters.end())
        return;

    UnbindNpcRouteCharacter(character);
    DetachObjectXFormsDependents(character);
    if (auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))) {
        UnassignScenarioCharacter(control->scenarioTask, character);
        if (control->ryo == character)
            control->ryo = nullptr;
    }
    FreeModelComponents((*resource)->character);
    ReleaseModelFormat7((*resource)->model);
    if ((*resource)->scenarioStepModels) {
        for (ScenarioStepModelResource &model : (*resource)->scenarioStepModels->models) {
            ReleaseModelFormat7(model.model);
        }
    }
    if ((*resource)->handlerTask) {
        HLib::DestroyTask((*resource)->handlerTask);
        (*resource)->handlerTask = nullptr;
    }
    UnregisterCharacterFromMapControl(**resource);
    characters.erase(resource);
}

void FreeCharacters() {
    while (!characters.empty()) {
        FreeCharacter(characters.back()->character);
    }
}

std::int32_t g_npcLoggerEnabled{};

namespace {

constexpr std::array<std::uint8_t, 7> visibilityModes{CRF_All, CRF_Unknown01, CRF_Unknown02, CRF_DisplayEnabled, CRF_Unknown08, CRF_InRange, CRF_Unknown20};
std::FILE *npcLogFile{};


std::int32_t npcLogDisplayChange(Character *character, std::int32_t visible) {
    if (!npcLogFile) {
        const std::time_t now = std::time(nullptr);
        std::array<char, 0x80> filename{};
        std::strftime(filename.data(), filename.size(), "npc_log-%H.%M.%S%p_%F.txt", std::localtime(&now));
        npcLogFile = std::fopen(filename.data(), "wb");
        if (npcLogFile)
            std::fwrite("CharID, SceneNumber, AreaId, EntryPoint\n", 0x28, 1, npcLogFile);
    }
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const shen_name name = scenGetCharaName(control->scenarioTask, character);
    std::array<char, 5> charaId{};
    std::memcpy(charaId.data(), &name.name, 4);
    std::array<char, 5> areaId{};
    std::memcpy(areaId.data(), &g_currentMap.name, 4);
    const std::int32_t result =
        d3t::debug_log("Src\\character.c", 1808, "CW_EnableDisplay %s [%02d:%s:%02d]", charaId.data(), g_actNumber, areaId.data(), g_entryPoint);
    if (!npcLogFile)
        return result;
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    std::array<char, 0x80> gameTime{};
    std::snprintf(gameTime.data(), gameTime.size(), "%02d:%02d:%02d, %02d:%02d:%02d", dateTime.hour, dateTime.minute, dateTime.second, dateTime.day,
        dateTime.month, dateTime.year);
    std::array<char, 0x100> line{};
    std::snprintf(line.data(), line.size(), "%s, %s, %02d, %s, %02d, %s\n", charaId.data(), visible ? "VISIBLE" : "HIDDEN", g_actNumber, areaId.data(),
        g_entryPoint, gameTime.data());
    std::fwrite(line.data(), std::strlen(line.data()), 1, npcLogFile);
    return std::fflush(npcLogFile);
}

} // namespace

std::uint8_t EV_SetCharDisplay(Character *character, CharaVisibilityMode visibilityType, std::int32_t enabled) {
    if (!character || visibilityType >= CVM_Count)
        return 0;
    const std::uint8_t mask = visibilityModes[static_cast<std::size_t>(visibilityType)];
    const std::uint8_t previous = character->renderFlags;
    character->renderFlags = static_cast<std::uint8_t>(enabled != 0 ? previous | mask : previous & ~mask);
    const std::uint8_t result = enabled != 0 ? static_cast<std::uint8_t>(visibilityType) : character->renderFlags;
    if (g_npcLoggerEnabled == 0 || (character->flags & CF_NpcLoggerTarget) == 0)
        return result;
    const std::int32_t visible = (mask & character->renderFlags) != 0;
    if (visible != ((previous & mask) != 0))
        return static_cast<std::uint8_t>(npcLogDisplayChange(character, visible));
    return result;
}

void charaSetVisibilityFlagTo(Character *character, std::uint32_t set) {
    if (!character)
        return;
    if (set != 0)
        character->flags |= CF_VisibleThisCheck;
    else
        character->flags &= ~CF_VisibleThisCheck;
}

bool IsOfThisVisibilityType(const Character *character, std::uint32_t mode) {
    constexpr std::uint8_t masks[]{CRF_All, CRF_Unknown01, CRF_Unknown02, CRF_DisplayEnabled, CRF_Unknown08, CRF_InRange, CRF_Unknown20};
    return character && mode < std::size(masks) && (character->renderFlags & masks[mode]) == masks[mode];
}

namespace {
std::int32_t characterLightMode = 1;
Vector3 characterLightRoot{};
Vector4 characterGlobalLighting{};
Vector4 characterAmbientLighting{};
bool characterLightAuthorOverride{};

// @verified - p
void SetupCharacterLightFrame() {
    characterLightMode = GetCurrentActiveLight();
    GetLightRootPosition(&characterLightRoot);
    GetGlobalLighting(&characterGlobalLighting);
    if (characterDrawVector.y > characterGlobalLighting.y)
        characterGlobalLighting.y = characterDrawVector.y;
    if (characterDrawVector.z > characterGlobalLighting.z)
        characterGlobalLighting.z = characterDrawVector.z;
    if (characterDrawVector.w > characterGlobalLighting.w)
        characterGlobalLighting.w = characterDrawVector.w;
    const float maximum = std::fmax(characterGlobalLighting.w, std::fmax(characterGlobalLighting.z, characterGlobalLighting.y));
    characterAmbientLighting = maximum >= 0.003f ?
        Vector4{maximum, characterGlobalLighting.y / maximum, characterGlobalLighting.z / maximum, characterGlobalLighting.w / maximum} :
        Vector4{0.0f, 1.0f, 1.0f, 1.0f};
}
} // namespace



// @verified - p
void DrawLights(Character *character) {
    if (!character)
        return;
    if (characterLightMode == 1) {
        character->fieldEc[0] = 0;
        character->fieldEc[1] = -1;
        character->fieldF8[0] = std::bit_cast<std::uint32_t>(1.0f);
        character->field110[0] = 1.0f;
        character->fieldE8 &= ~0xff00U;
        return;
    }
    if (static_cast<std::int8_t>(character->fieldE8 >> 8) < 1)
        character->fieldEc[0] = -1;
    character->fieldE8 = (character->fieldE8 & ~0xff00U) | 0x100U;
    Vector3 position;
    GetCharacterMotionWorldPosition(character, 1, &position);
    struct Candidate {
        std::int16_t id;
        float score;
        float attenuation;
    };
    std::array<Candidate, 6> ranked{};
    std::size_t count = 0;
    const auto eighthPower = [](float value) {
        const float square = value * value;
        const float fourth = square * square;
        return fourth * fourth;
    };
    bool authorFallback = characterLightAuthorOverride;
    for (const std::int16_t id : GetCharacterLightCandidates()) {
        if (id < 0)
            break;
        const LightRuntimeEntry *light = GetLightRuntimeEntry(id);
        if (!light)
            continue;
        const std::uint8_t kind = light->indexes[1] & 0xf7U;
        Vector3 displacement{-light->values2c[0], -light->values2c[1], -light->value34};
        float distance = 1.0f;
        if (kind == 3 || kind == 4) {
            Vector3 source = light->vector20;
            if (kind == 3) {
                source.x += characterLightRoot.x;
                source.y += characterLightRoot.y;
                source.z += characterLightRoot.z;
            }
            displacement = {source.x - position.x, source.y - position.y, source.z - position.z};
            distance = std::sqrt(displacement.x * displacement.x + displacement.y * displacement.y + displacement.z * displacement.z);
            if (static_cast<double>(distance) > static_cast<double>(light->vectors[1].z) + 1.4)
                continue;
            distance += 0.05f;
        }
        const float inverseDistance = 1.0f / distance;
        float attenuation = inverseDistance;
        float score = -100000.0f;
        if (kind == 2) {
            score = -1.0f;
        } else if (kind == 3 || kind == 4) {
            attenuation = std::fmin(1.0f, eighthPower(inverseDistance) * eighthPower(light->value1c * light->vectors[1].z));
            if (kind == 4) {
                Vector3 direction{light->values2c[0], light->values2c[1], light->value34};
                Vector3::NormalizeInPlace(direction);
                const float cutoff = cos_fixed16(static_cast<std::uint16_t>(light->unknown02 + 910));
                const float dot = displacement.x * direction.x + displacement.y * direction.y + displacement.z * direction.z;
                attenuation *= std::fmax(0.0f, (dot * inverseDistance - cutoff) / (1.0f - cutoff));
            }
            score = std::fabs((light->vectors[0].z + light->vectors[0].y + light->vectors[1].x) * light->vectors[1].y) * attenuation;
        }
        if (static_cast<double>(attenuation) < 0.001)
            score = -2.0f;
        std::size_t insert = 0;
        while (insert < count &&
            !(static_cast<double>(score) - 0.04 > static_cast<double>(ranked[insert].score) ||
                (static_cast<double>(score) + 0.04 > static_cast<double>(ranked[insert].score) && ranked[insert].id < id)))
            ++insert;
        if (insert < ranked.size()) {
            for (std::size_t slot = std::min(count, ranked.size()-1); slot > insert; --slot)
                ranked[slot] = ranked[slot - 1];
            ranked[insert] = {id, score, attenuation};
            if (static_cast<double>(attenuation) > 0.003)
                authorFallback = false;
        }
        count = std::min(count+1, ranked.size());
    }
    std::int32_t fadeLimit = static_cast<std::int8_t>(character->fieldE8 >> 24);
    if (authorFallback || character->fieldEc[0] == 0)
        fadeLimit = 0;
    for (std::size_t slot = 0; slot < 6 && character->fieldEc[slot] >= 0;) {
        std::size_t rank = 0;
        while (rank < count && ranked[rank].id != character->fieldEc[slot])
            ++rank;
        float weight = std::bit_cast<float>(character->fieldF8[slot]);
        if (character->fieldEc[slot] != 0) {
            if (rank < count)
                character->field110[slot] = ranked[rank].attenuation;
            if (rank >= count || !(static_cast<double>(character->field110[slot]) > 0.0009))
                weight = 0.0f;
            else
                weight = static_cast<std::int32_t>(rank) < fadeLimit ? std::fmin(1.0f, weight + 0.1f) : weight - 0.1f;
        } else {
            weight = authorFallback ? std::fmin(1.0f, weight + 0.1f) : weight - 0.1f;
        }
        character->fieldF8[slot] = std::bit_cast<std::uint32_t>(weight);
        if (weight <= 0.01f) {
            for (std::size_t next = slot; next < 5; ++next) {
                character->fieldEc[next] = character->fieldEc[next + 1];
                character->fieldF8[next] = character->fieldF8[next + 1];
                character->field110[next] = character->field110[next + 1];
            }
            character->fieldEc[5] = -1;
        } else
            ++slot;
    }
    if (authorFallback && character->fieldEc[0] < 0) {
        character->fieldEc[0] = 0;
        character->fieldEc[1] = -1;
        character->fieldF8[0] = std::bit_cast<std::uint32_t>(0.0f);
        character->field110[0] = 1.0f;
    }
    for (std::size_t rank = 0; rank < count; ++rank) {
        for (std::size_t slot = 0; slot < 6; ++slot) {
            if (character->fieldEc[slot] == ranked[rank].id)
                break;
            if (character->fieldEc[slot] < 0) {
                character->fieldEc[slot] = ranked[rank].id;
                character->fieldF8[slot] = std::bit_cast<std::uint32_t>(static_cast<std::int32_t>(slot) < fadeLimit ? 1.0f : 0.0f);
                character->field110[slot] = ranked[rank].attenuation;
                if (slot + 1 < 6)
                    character->fieldEc[slot + 1] = -1;
                break;
            }
        }
    }
}

// @verified - p
void shenDrawCharaLighting(Character *character) {
    if (!GetControllerForNode(character))
        return;
    Vector3 hip;
    GetCharacterMotionWorldPosition(character, 1, &hip);
    std::uint32_t destination = 1;
    if (character->fieldEc[0] == 0) {
        SetCurrentActiveLight(1);
        LightRuntimeEntry light = *GetLightRuntimeEntry(0);
        const float weight = std::bit_cast<float>(character->fieldF8[0]);
        light.vectors[0].y *= weight;
        light.vectors[0].z *= weight;
        light.vectors[1].x *= weight;
        SetLegacyLight(0, light);
        SetLightKind(0, 2);
        SetLegacySelectedLightEnabled(0, true);
        SetSelectedLightIntensity(0, GetWeatherSceneLighting());
        const float ambient = GetWeatherLighting();
        UpdateGlobalLighting({ambient, ambient, ambient, ambient});
    } else if ((character->fieldE8 & 1U) != 0) {
        SetCurrentActiveLight(1);
        ResetLightEntry(0);
        SetLightKind(0, 2);
        SetLegacySelectedLightEnabled(0, true);
        Vector3 direction{};
        Vector4 colour{};
        std::uint32_t count = 0;
        for (std::uint32_t index = 0; index < 6 && character->fieldEc[index] >= 0; ++index) {
            const float weight = character->field110[index] * std::bit_cast<float>(character->fieldF8[index]);
            if (!(static_cast<double>(weight) > 0.003))
                continue;
            const LightRuntimeEntry &light = *GetLightRuntimeEntry(character->fieldEc[index]);
            const std::uint32_t kind = light.indexes[1] & 0xf7U;
            Vector3 incoming{light.values2c[0], light.values2c[1], light.value34};
            if (kind == 3 || kind == 4) {
                Vector3 position = light.vector20;
                if (kind == 3) {
                    position.x += characterLightRoot.x;
                    position.y += characterLightRoot.y;
                    position.z += characterLightRoot.z;
                }
                incoming = {hip.x - position.x, hip.y - position.y, hip.z - position.z};
            }
            Vector3::NormalizeInPlace(incoming);
            ++count;
            direction.x += incoming.x * weight;
            direction.y += incoming.y * weight;
            direction.z += incoming.z * weight;
            const float intensity = weight * light.vectors[1].y;
            colour.y += intensity * light.vectors[0].y;
            colour.z += intensity * light.vectors[0].z;
            colour.w += intensity * light.vectors[1].x;
        }
        float brightness = 0.0f;
        if (count != 0) {
            Vector3::NormalizeInPlace(direction);
            if (colour.y < 0.0f)
                colour.y = 0.0f;
            if (colour.z < 0.0f)
                colour.z = 0.0f;
            if (colour.w < 0.0f)
                colour.w = 0.0f;
            brightness = std::fmax(colour.w, std::fmax(colour.z, colour.y));
        }
        if (count == 0 || static_cast<double>(brightness) < 0.003) {
            brightness = 0.0f;
            colour = {1.0f, 1.0f, 1.0f, 1.0f};
            direction = {0.0f, 1.0f, 0.0f};
        }
        const float ambient = std::fmax(0.33329999f, 1.0f - brightness);
        colour.y += ambient * characterAmbientLighting.y;
        colour.z += ambient * characterAmbientLighting.z;
        colour.w += ambient * characterAmbientLighting.w;
        const float maximum = std::fmax(colour.w, std::fmax(colour.z, colour.y));
        colour.y /= maximum;
        colour.z /= maximum;
        colour.w /= maximum;
        shenSetLightColour(0, colour);
        SetSelectedLightDirection(0, direction);
        SetSelectedLightIntensity(0, maximum * brightness);
        const float ambientIntensity = characterAmbientLighting.x;
        UpdateGlobalLighting({ambientIntensity, ambientIntensity, ambientIntensity, ambientIntensity});
    } else {
        SetCurrentActiveLight(characterLightMode);
        SetLegacySelectedLightEnabled(0, false);
        for (std::uint32_t index = 0; index < 6 && character->fieldEc[index] >= 0; ++index) {
            const float weight = std::bit_cast<float>(character->fieldF8[index]);
            if (!(static_cast<double>(weight) > 0.003))
                continue;
            LightRuntimeEntry light = *GetLightRuntimeEntry(character->fieldEc[index]);
            light.vectors[0].y *= weight;
            light.vectors[0].z *= weight;
            light.vectors[1].x *= weight;
            SetLegacyLight(destination++, light);
        }
        UpdateGlobalLighting(characterGlobalLighting);
    }
    for (; destination < 16; ++destination)
        SetLegacySelectedLightEnabled(destination, false);
    UpdateLegacyLightTransforms();
}



void charaCollisionDistanceChecks(void *parameter) {
    auto *character = static_cast<Character *>(parameter);
    if (!character)
        return;
    SetShadowPositioningMode(true);
    if (GetPreviousActiveLight() == 1)
        SetLegacyColourWriteMask(0);
    if (character->shadowModel || character->type == 7 || character->type == 4) {
        Matrix4x4_Push();
        Vector3 position = GetCharacterWorldPosition(character);
        Vector3i rotation = GetCharacterWorldRotation(character);
        float ground = charaGetShadowY(character);
        if ((character->flags & CF_UseStoredShadowY) == 0) {
            Vector3 probe = position;
            probe.y += 0.5f;
            ground = shenFindIntersectionWithScene(probe, position.z);
            if (ground < -1000.0f || ground > position.y + 0.1f)
                ground = position.y;
            ground += 0.01f;
        }
        const MT7Model *root = static_cast<const MT7Model *>(character->model);
        Vector3 scale = character->scale;
        switch (character->type) {
        case 1:
        case 10:
            root = static_cast<const MT7Model *>(character->shadowModel);
            if (root == GetSHDS())
                root = static_cast<const MT7Model *>(character->model);
            ProjectCharacterShadow(character, position, rotation, scale, ground, root, character->type == 1 ? 1.0f : -1.0f);
            break;
        case 2:
        case 3:
        case 9: {
            Vector3 offset{};
            GetCharacterShadowOffset(character, &offset);
            GetCharacterShadowScale(character, &scale);
            position.x += offset.x;
            position.y += offset.y;
            position.z += offset.z;
            rotation.y += GetCharacterShadowYaw(character);
            if (character->type == 3)
                SetShadowPositioningMode(false);
            ProjectFixedCharacterShadow(character, position, rotation, scale, ground, character->type == 3 ? 0 : 1, character->type == 9 ? 1.0f : -1.0f);
            SetShadowPositioningMode(true);
            break;
        }
        case 4:
            DrawObject_AssignShadow(character, position, ground);
            break;
        case 6:
        case 8:
        case 11:
            ProjectCharacterShadow(character, position, rotation, scale, ground, root, character->type == 6 ? 0.5f : character->type == 8 ? 1.0f : -2.0f);
            break;
        default: {
            Vector3 offset{}, shadowScale{};
            GetCharacterShadowOffset(character, &offset);
            GetCharacterShadowScale(character, &shadowScale);
            position.x += offset.x;
            position.y += offset.y;
            position.z += offset.z;
            scale.x *= shadowScale.x;
            scale.y *= shadowScale.y;
            scale.z *= shadowScale.z;
            ProjectCharacterShadow(character, position, rotation, scale, ground, root, -1.0f);
            break;
        }
        }
        Matrix4x4::Pop(1);
    }
    RestoreEngineFlag(0x43);
}

void shenExecuteCharaDrawCallbacks(Character *character) {
    if (character && (character->flags & CF_VisibleThisCheck) != 0 && character->collisionCheck)
        HLib::ExecuteCallbackNow(character->collisionCheck, character);
}

namespace {

std::uint32_t DrawCharacterResource(Character *character) {
    if (!character)
        return 0;
    if (!character->model) {
        if (character->core)
            character->core(character);
        return 0;
    }
    const MotionWork *const motionWork = FindMotionWork(character);
    const MT7Model &model = *static_cast<const MT7Model *>(character->model);
    const Matrix4x4 saved = workTransformMatrix;
    const Matrix4x4 *const matrixFrames = IsCharacterMirroredForDraw(character) ? nullptr : GetMotionDrawOverrideData(motionWork);
    Matrix4x4 entityTransform;
    if (matrixFrames)
        entityTransform = MultiplyTransforms(CharacterEntityTransform(*character, false), saved);
    CharacterCoreDrawContext context{
        .character = character, .model = &model, .entityTransform = matrixFrames ? &entityTransform : nullptr, .drawBase = &saved, .draws = 0};
    CharacterCoreDrawContext *const previousContext = currentCharacterCoreDrawContext;
    currentCharacterCoreDrawContext = &context;
    Character *const previousEntity = SetScriptedModelEntity(character);
    if (character->core)
        character->core(character);
    SetScriptedModelEntity(previousEntity);
    currentCharacterCoreDrawContext = previousContext;
    return context.draws;
}

// @verified - p
void GatherCharacterDrawLists(Character *character, void *taskData) {
    auto &parameters = *static_cast<CharacterDrawTaskParameters *>(taskData);
    if (character != parameters.player) {
        bool listed = false;
        if (FindMotionWork(character)) {
            if (IsOfThisVisibilityType(character, 0)) {
                const float distance = std::fabs(character->drawDistance);
                std::uint32_t index = 0;
                while (index < parameters.humanCount && !(parameters.humanSortDistance[index] > distance))
                    ++index;
                for (std::uint32_t move = parameters.humanCount; move > index; --move) {
                    parameters.humans[move] = parameters.humans[move - 1];
                    parameters.humanSortDistance[move] = parameters.humanSortDistance[move - 1];
                }
                parameters.humans[index] = character;
                parameters.humanSortDistance[index] = distance;
                parameters.humanCount = std::min(parameters.humanCount + 1U, 50U);
                listed = true;
            }
        } else {
            RunModelComponentCoreCallbacks(character);
            if (IsOfThisVisibilityType(character, 0) && static_cast<double>(character->alpha) > 0.001) {
                parameters.objects[parameters.objectCount] = character;
                parameters.objectCount = std::min(parameters.objectCount + 1U, 100U);
                listed = true;
            }
        }
        if (!listed) {
            character->fieldE8 |= 0xff00U;
            character->fieldEc[0] = -1;
        }
    }
    if (parameters.allCharacterCount < parameters.allCharacters.size()) {
        parameters.allCharacters[parameters.allCharacterCount++] = character;
    }
}

void SetCharacterDrawActivation(Character *character, bool active) {
    struct DrawActivationPrefix {
        std::uint32_t flags;
        std::int32_t phase;
    };
    static_assert(sizeof(DrawActivationPrefix) == 0x08);
    static_assert(offsetof(DrawActivationPrefix, flags) == 0x00);
    static_assert(offsetof(DrawActivationPrefix, phase) == 0x04);
    DrawActivationPrefix *const activation = static_cast<DrawActivationPrefix *>(FindModelComponent(character, CHARACTER_DRAW_ACTIVATION));
    if (!activation)
        return;
    std::int32_t &phase = activation->phase;
    if (active) {
        if (phase > 0)
            phase = phase >= 15 && phase <= 40 ? phase - 40 : 0;
    } else if (phase < 0) {
        phase += 40;
    } else if (phase == 0) {
        phase = 5;
    }
}

std::uint32_t DrawCharacterHumanPhase(Character *character) {
    if (!d3t::BindSceneFrameRenderTarget())
        return 0;
    HandParams *const left = GetCharacterHand(character, 1);
    HandParams *const right = GetCharacterHand(character, 0);
    Matrix4x4_Push(nullptr);
    Matrix4x4::SetPrePassToTempTransform();
    ApplyRecursiveCharacterNodeTransforms(&character->entity.node);
    if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
        Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    bool handled = GetLegacyPerspectiveProjection() == 1;
    MotionWork *const motionWork = FindMotionWork(character);
    if (handled) {
        if (left)
            AdvanceHandDrawState(*left);
        if (right)
            AdvanceHandDrawState(*right);
    }
    if (left && left->regionState)
        d3t::HashModelWithAlpha(GetMT7ReaderData(left->primaryReader), left->modelAlpha, GetLegacyPerspectiveProjection());
    if (right && right->regionState)
        d3t::HashModelWithAlpha(GetMT7ReaderData(right->primaryReader), right->modelAlpha, GetLegacyPerspectiveProjection());
    if (model) {
        PrepareClothForDraw(character);
        RecursivePrepareChildMatrices(model->nodes.data(), model->nodes.data());
        if (!(character->alpha <= 0.0f && !GetCustomMotionOverrideFlags(motionWork))) {
            const d3t::ModelRuntimeState *const state = d3t::GetModelRuntimeState(GetMT7ReaderData(model));
            if (state && state->nodeTableAvailable && state->nodesAssigned)
                handled = d3t::HashModelWithAlpha(GetMT7ReaderData(model), character->alpha, GetLegacyPerspectiveProjection());
            if (handled && motionWork)
                DrawMotionParts(*character, *motionWork, true);
        }
        if (handled)
            UpdateCharacterOsage(model->nodes.data(), character);
        UpdateModelFrame(model, character, 1, 0);
    }
    Matrix4x4::Pop(1);
    std::uint32_t draws = 0;
    if (!handled) {
        SetLegacyBusyPreparingDraw(true);
        shenSetCharaDrawFlags(1);
        shenDrawCharaLighting(character);
        draws = DrawCharacterResource(character);
        SetLegacyBusyPreparingDraw(false);
        shenSetRenderFlags();
    }
    return draws;
}

void charaHandlerDrawHumanInternal(void *parameter) {
    DrawCharacterHumanPhase(static_cast<Character *>(parameter));
}

void charaHandlerDrawObjectInternal(void *parameter) {
    if (!d3t::BindSceneFrameRenderTarget())
        return;
    Character *const character = static_cast<Character *>(parameter);
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM));
    if (image && image->entries) {
        Matrix4x4_Push(nullptr);
        Matrix4x4::SetPrePassToTempTransform();
        ApplyRecursiveCharacterNodeTransforms(&character->entity.node);
        if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
            Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
        if (GetLegacyRenderMask() && character->core == DrawImageCharacterCore)
            DrawImageCharacterModel(character, 0);
        const bool handled = d3t::HashModel(GetMT7ReaderData(static_cast<const MT7Model *>(character->model)), GetLegacyPerspectiveProjection());
        Matrix4x4::Pop(1);
        if (handled)
            return;
    }
    shenSetCharaDrawFlags(1);
    shenDrawCharaLighting(character);
    DrawCharacterResource(character);
    shenSetRenderFlags();
}

// @verified - p
std::uint32_t charaHandlerDrawHuman(Character *character) {
    if (!character)
        return 0;
    const std::uint32_t mask = character->renderFlags & CRF_All;
    if (mask != CRF_All)
        return mask;
    return static_cast<std::uint32_t>(HLib::ExecuteCallbackNow(charaHandlerDrawHumanInternal, character));
}

// @verified - p
std::uint32_t charaHandlerDrawObject(Character *character) {
    if (!character)
        return 0;
    const std::uint32_t mask = character->renderFlags & CRF_All;
    if (mask != CRF_All)
        return mask;
    return static_cast<std::uint32_t>(HLib::ExecuteCallbackNow(charaHandlerDrawObjectInternal, character));
}

// @verified - p
std::uint32_t DrawHumanList(CharacterDrawTaskParameters &parameters, std::int32_t requestedCount) {
    const std::int32_t count = static_cast<std::int32_t>(parameters.humanCount);
    if (npcFillType == NpcFillType::BackFill) {
        std::int32_t firstActive = std::bit_cast<std::int32_t>(parameters.humanCount - static_cast<std::uint32_t>(requestedCount));
        if (firstActive < 0)
            firstActive = 0;
        std::int32_t index = count - 1;
        for (; index >= firstActive; --index)
            SetCharacterDrawActivation(parameters.humans[index], true);
        for (; index >= 0; --index)
            SetCharacterDrawActivation(parameters.humans[index], false);
    } else if (npcFillType != NpcFillType::Manual) {
        if (requestedCount > count)
            requestedCount = count;
        std::int32_t index = 0;
        for (; index < requestedCount; ++index)
            SetCharacterDrawActivation(parameters.humans[index], true);
        for (; index < count; ++index)
            SetCharacterDrawActivation(parameters.humans[index], false);
    }
    std::uint32_t draws = 0;
    npcUpdatedLastTick = 0;
    for (std::uint32_t index = 0; index < parameters.humanCount; ++index) {
        Character *const character = parameters.humans[index];
        DrawLights(character);
        if (IsCharacterMirroredForDraw(character) || static_cast<double>(character->alpha) > 0.001) {
            ++npcUpdatedLastTick;
            if (ReturnZero() == 0)
                draws += charaHandlerDrawHuman(character);
        }
    }
    return draws;
}

// @verified - p
std::uint32_t DrawObjectList(CharacterDrawTaskParameters &parameters) {
    std::uint32_t draws = 0;
    const std::uint32_t count = std::min(parameters.objectCount, 99U);
    for (std::uint32_t index = 0; index < count; ++index) {
        Character *const character = parameters.objects[index];
        SetCharacterDrawActivation(character, true);
        DrawLights(character);
        if (IsCharacterMirroredForDraw(character) || static_cast<double>(character->alpha) > 0.001)
            draws += charaHandlerDrawObject(character);
    }
    return draws;
}

// @verified - p
void TestCharacterVisibility(Character *character, void *) {
    const std::uint32_t pointCount = characterClipPointCount;
    const std::uint8_t visibilityOverride = GetCharaFlag(character);
    SetCharaFlag(character, 0);
    bool shouldBeVisible = false;
    if ((character->renderFlags & CRF_DisplayEnabled) != 0) {
        bool cameraVisible = true;
        if (const MotionWork *const work = FindMotionWork(character)) {
            if ((character->renderFlags & CRF_InRange) != 0) {
                if (pointCount != 0 && (character->flags & CF_Unknown40) != 0) {
                    const Vector4 &root = work->rig.lowerBody.pelvisWorld.rows[3];
                    cameraVisible = visibilityOverride != 0 ? (visibilityOverride & 1U) != 0 : camCtrlCheckPosition({root.x, root.y, root.z}, 0.4f);
                }
                shouldBeVisible = cameraVisible;
            }
        } else {
            shouldBeVisible = true;
            const Vector3 root = GetCharacterWorldPosition(character);
            const bool fixed = FindFixo(character) != 0;
            bool frustumVisible = true;
            if (!fixed) {
                const float radius = GetCharacterDisplayRadius(character);
                frustumVisible = camCtrlIsSphereVisible(root, radius);
                if (frustumVisible && pointCount != 0 && charaCheckFlags(character))
                    cameraVisible = camCtrlCheckPosition(root, radius);
            }
            EV_SetCharDisplay(character, CVM_FrustumResult, frustumVisible);
        }
        EV_SetCharDisplay(character, CVM_CameraOcclusionResult, cameraVisible);
    }
    const bool visible = charaIsDisplayCheckEnabled(character) && (shouldBeVisible || charaForceVisibleCheck(character));
    charaSetVisibilityFlagTo(character, visible);
}

// @verified - p
void CharDraw_Callback(void *taskData) {
    if (ReturnZero() != 0)
        return;
    auto *const parameters = static_cast<CharacterDrawTaskParameters *>(taskData);
    parameters->player = static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
    parameters->humanCount = 0;
    parameters->objectCount = 0;
    parameters->allCharacterCount = 0;
    characterLightAuthorOverride = false;
    ResetCharacterShadowStencilReference();
    if (!IsWorldDrawingEnabled())
        return;
    SetupCharacterLightFrame();
    if (!parameters->player)
        return;
    DrawLights(parameters->player);
    charaHandlerDrawHuman(parameters->player);
}

// @verified - p
void CharDrawHuman_Callback(void *) {
    if (ReturnZero() != 0)
        return;
    if (!authGetLoaded() && (GetCurrentWeatherInstruction() & 3U) == 3U) {
        characterLightAuthorOverride = mapAllowsAuthorSpawn || g_currentMap.name == SHEN_NAME(ARK0) || GetCurrentAuthorId().name != 0;
    }
    if (!IsWorldDrawingEnabled())
        return;
    auto *const parameters = static_cast<CharacterDrawTaskParameters *>(HLib::GetTaskParameter(characterDrawTask));
    if (!parameters)
        return;
    const std::int32_t frameRemainder = TruncateToInt32(g_accumulatedFrameTimeDelta - characterHumanDrawBudget);
    const std::uint32_t product = (32000U - static_cast<std::uint32_t>(frameRemainder)) * static_cast<std::uint32_t>(npcUpdatedLastTick);
    std::int32_t batchCount = std::bit_cast<std::int32_t>(product) / TruncateToInt32(characterHumanDrawBudget) - 1;
    const std::uint32_t startTime = GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    if (batchCount < 6)
        batchCount = 6;
    ForEachLiveCharacterInMapOrder(GatherCharacterDrawLists, parameters);
    if (npcBatchCountOverride >= 0) {
        batchCount = npcBatchCountOverride;
    } else if (npcPoolSizingModeOrOverride >= 0) {
        npcPoolTarget = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(batchCount) * 2U + 2U);
        if (g_currentMap.name == SHEN_NAME(WSKB)) {
            npcPoolTarget = npcPoolSizingModeOrOverride;
            batchCount = 49;
        }
        if (g_currentMap.name == SHEN_NAME(AR01))
            npcPoolTarget = npcPoolSizingModeOrOverride;
    }
    DrawHumanList(*parameters, batchCount);
    const std::uint32_t elapsed = startTime - GetAccumulatedFrameTimes(FrameTimerSlot::MainLoop, FrameTimeOp::Delta);
    characterHumanDrawBudget = static_cast<float>(elapsed) * 0.079999998f;
    if (characterHumanDrawBudget < 10.0f)
        characterHumanDrawBudget = 10.0f;
}

// @verified - p
void DrawCharacterShadowList(CharacterDrawTaskParameters &parameters) {
    const std::int32_t previousActiveLight = GetPreviousActiveLight();
    if (static_cast<std::int16_t>(camCtrlGetFov()) / 2 < camCtrlGetRot(nullptr)->pitch)
        return;
    SetProjectedShadowLightDirection(GetTexUVW(1));
    SetLegacyDisableReflections(true);
    BeginLegacyCharacterShadowFrame();
    for (std::uint32_t index = 0; index < parameters.humanCount; ++index) {
        Character *const character = parameters.humans[index];
        DrawLights(character);
        if (IsCharacterMirroredForDraw(character) || static_cast<double>(character->alpha) > 0.001)
            shenExecuteCharaDrawCallbacks(character);
    }
    const std::uint32_t objectCount = std::min(parameters.objectCount, 99U);
    for (std::uint32_t index = 0; index < objectCount; ++index) {
        Character *const character = parameters.objects[index];
        SetCharacterDrawActivation(character, true);
        DrawLights(character);
        if (IsCharacterMirroredForDraw(character) || static_cast<double>(character->alpha) > 0.001)
            shenExecuteCharaDrawCallbacks(character);
    }
    shenExecuteCharaDrawCallbacks(parameters.player);
    EndLegacyCharacterShadowFrame();
    if (previousActiveLight != 1)
        PerformCommonShaderPass(0);
    SetLegacyDisableReflections(false);
}

// @verified - p
void CharDrawObject_Callback(void *) {
    if (ReturnZero() != 0 || !IsWorldDrawingEnabled())
        return;
    auto *const parameters = static_cast<CharacterDrawTaskParameters *>(HLib::GetTaskParameter(characterDrawTask));
    DrawObjectList(*parameters);
    if (GetLegacyPerspectiveProjection() == 0)
        DrawCharacterShadowList(*parameters);
    HLib::ExecuteCallbackNow(ImmediateLightWork, nullptr);
}

} // namespace

std::uint32_t ExecuteCharacterCoreDraw(Character *character) {
    return DrawCharacterResource(character);
}

// @verified - p
void CharDispCheck_Callback(void *) {
    characterClipPointCount = GetFieldDepthPointCount();
    ForEachLiveCharacterInMapOrder(TestCharacterVisibility, nullptr);
}

// @verified - p
std::int32_t GetNpcBatchCountOverride(std::int32_t) {
    return npcBatchCountOverride;
}

// @verified - p
void SetNewNpcBatchCountOverride(NpcFillType fillType, std::int32_t count) {
    npcFillType = fillType;
    npcBatchCountOverride = static_cast<std::int32_t>(fillType) == 0 ? 49 : count >= 50 ? 49 : count;
}

// @verified - p
void ExchangeCharacterDrawVector(const Vector4 *colour, Vector4 *previous) {
    if (previous)
        *previous = characterDrawVector;
    characterDrawVector = *colour;
}

// @verified - p
HLib::Task *CharDraw_Human_Object_Enqueue(std::uint8_t nextFunctionIndex) {
    characterDrawTask = HLib::EnqueueTaskWithParameter(CharDraw_Callback, nextFunctionIndex, 2, sizeof(CharacterDrawTaskParameters), CHARACTER_DRAW_TASK);
    HLib::EnqueueTaskWithoutParameter(CharDrawHuman_Callback, nextFunctionIndex, 2, static_cast<std::int32_t>(HUMAN_DRAW_TASK.name));
    HLib::EnqueueTaskWithoutParameter(CharDrawObject_Callback, nextFunctionIndex, 2, static_cast<std::int32_t>(OBJECT_DRAW_TASK.name));
    npcBatchCountOverride = 49;
    npcFillType = static_cast<NpcFillType>(0);
    characterHumanDrawBudget = 10.0f;
    npcUpdatedLastTick = 0;
    npcPoolSizingModeOrOverride = -5;
    if (auto *const parameters = static_cast<CharacterDrawTaskParameters *>(HLib::GetTaskParameter(characterDrawTask))) {
        std::memset(parameters, 0, sizeof(*parameters));
    }


    constexpr std::array<std::uint32_t, 25> authorSpawnMaps{SHEN_NAME(WB01), SHEN_NAME(QRR0), SHEN_NAME(ARC0), SHEN_NAME(WKA0), SHEN_NAME(ARA0),
        SHEN_NAME(WTA0), SHEN_NAME(WEG0), SHEN_NAME(ARM0), SHEN_NAME(AKS0), SHEN_NAME(WET0), SHEN_NAME(AKT0), SHEN_NAME(AKY0), SHEN_NAME(WSY0), SHEN_NAME(AKT2),
        SHEN_NAME(AKS1), SHEN_NAME(WSG1), SHEN_NAME(WEM1), SHEN_NAME(WES1), SHEN_NAME(AKT1), SHEN_NAME(WRS2), SHEN_NAME(AKA3), SHEN_NAME(AKT3), SHEN_NAME(AKA4),
        SHEN_NAME(QCAE), SHEN_NAME(WESM)};
    mapAllowsAuthorSpawn = std::find(authorSpawnMaps.begin(), authorSpawnMaps.end(), g_currentMap.name) != authorSpawnMaps.end();
    return characterDrawTask;
}

#if defined(_WIN32)
CharacterDebugCallback g_characterDebugCallback{};
std::int32_t g_characterDebugSnapCamera{};
float g_characterDebugCameraRotation{};
shen_name g_characterDebugSelectedName{};

void DrawRegisteredCharacterDebug() {
    using enum CameraState;
    if (!g_mapControlTask || !HLib::GetTaskParameter(g_mapControlTask) || !ImGui::CollapsingHeader("Registered Characters") || !ImGui::TreeNode("Displayed"))
        return;
    bool logging = (g_npcLoggerEnabled & 0xff) != 0;
    if (ImGui::Checkbox("Log to output", &logging))
        g_npcLoggerEnabled = (g_npcLoggerEnabled & ~0xff) | static_cast<std::int32_t>(logging);
    if (g_mapControlTask && HLib::GetTaskParameter(g_mapControlTask)) {
        bool snapCamera = (g_characterDebugSnapCamera & 0xff) != 0;
        if (ImGui::Checkbox("Snap Camera", &snapCamera))
            g_characterDebugSnapCamera = (g_characterDebugSnapCamera & ~0xff) | static_cast<std::int32_t>(snapCamera);
        ImGui::SliderFloat("Camera Rotation", &g_characterDebugCameraRotation, 0.0f, 360.0f, nullptr);
        if (g_characterDebugSnapCamera) {
            if (camCtrlGetState() != Snap)
                camCtrlSetState(Snap);
        } else {
            if (camCtrlGetState() == Snap)
                camCtrlSetState(Init);
            g_characterDebugSelectedName = {};
        }
        Character *selected = nullptr;
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        for (void *const head : control->characterTable) {
            for (CharacterMapEntry *entry = static_cast<CharacterMapEntry *>(head); entry; entry = entry->next) {
                Character *const character = entry->character;
                const shen_name name = scenGetCharaName(control->scenarioTask, character);
                if ((character->flags & CF_NpcLoggerTarget) && IsOfThisVisibilityType(character, 0)) {
                    char label[5]{};
                    std::memcpy(label, &name.name, 4);
                    if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Leaf)) {
                        if (ImGui::IsItemClicked())
                            g_characterDebugSelectedName = name;
                        ImGui::TreePop();
                    }
                }
                if (g_characterDebugSelectedName.name == name.name)
                    selected = character;
            }
        }
        if (g_characterDebugSnapCamera) {
            if (selected) {
                const bool small = GetCharacterImageSourceType(selected) == 16;
                const float angle = g_characterDebugCameraRotation / 57.295776f;
                Vector3 position = selected->entity.node.position;
                position.x += std::cos(angle) * 1.25f;
                position.z += std::sin(angle) * 1.25f;
                position.y += small ? 1.0f : 2.0f;
                camCtrlSetNextPosition(position);
                Vector3 target = selected->entity.node.position;
                target.y += small ? 0.60000002f : 1.2f;
                camCtrlSetTargetPosition(target);
            } else
                g_characterDebugSelectedName = {};
        }
    }
    ImGui::TreePop();
}

CharacterDebugCallback SetupCharacterDebug() {
    g_characterDebugCallback = DrawRegisteredCharacterDebug;
    return DrawRegisteredCharacterDebug;
}
#endif

} // namespace shenmue::s2
