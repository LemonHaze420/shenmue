#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/object_xforms.hpp"
#include "s2/event_matrix_dc.hpp"

#include "s2/math.hpp"
#include "s2/motion_work_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/character_placement_dc.hpp"
#else
#include "s2/model_component.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/model_cache.hpp"

#include "s2/archive_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/motion_pose.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_image.hpp"
#include "s2/hand.hpp"
#include "s2/handcuffs.hpp"
#include "s2/face.hpp"
#include "s2/lighting.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/door.hpp"
#include "s2/maze.hpp"
#include "s2/navi.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/string_utility.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "s2/area_names.hpp"
#include "s2/model_component_internal.hpp"
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

using namespace model_component_detail;

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace model_component_detail {

ObjectXFormsParams *FindObjectXForms(Character *character);
void SetupObjectXForms(Character *character, void *payload);

} // namespace model_component_detail

void SetFixoDataForMotionWork(
    Character *target, Character *source, std::int32_t boneIndex, const Vector3 &position, const std::array<std::int32_t, 3> &rotation);
ModelComponentHeader *SetFixoAlphaPropagation(Character *character, std::int32_t enabled);
void CaptureFixoRelativePlacement(Character *character, ObjectXFormsParams *attachment, const Matrix4x4 *source);
std::uint64_t SetFixoLocalRotation(Character *character, const Vector3i *rotation);
std::uint64_t SetFixoLocalPosition(Character *character, const Vector3 *position);
std::int64_t SetFixoInverseChildPlacement(Character *character, std::uint16_t childId);
void AttachFixoPreservingPlacement(Character *target, Character *source, std::int32_t boneIndex);
void SetFixoInversePositionRotation(Character *character, const Vector3 *position, const Vector3i *rotation);
std::int16_t FindFixo(Character *character);
ObjectXFormsParams *ResetCharaFixo(Character *character);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int16_t FindFixo(Character *character);
void CaptureFixoRelativePlacement(Character *character, ObjectXFormsParams *attachment, const Matrix4x4 *source);
void RefreshFixoRelativePlacement(Character *character, const Matrix4x4 *transform);
void AttachFixoPreservingPlacement(Character *target, Character *source, std::int32_t boneIndex);
void SetFixoDataForMotionWork(Character *target, Character *source, std::int32_t boneIndex, const Vector3 &position, const std::int32_t (&rotation)[3]);
ObjectXFormsParams *SetFixoLocalPosition(Character *character, const Vector3 *position);
ObjectXFormsParams *SetFixoLocalRotation(Character *character, const Vector3i *rotation);
ObjectXFormsParams *ResetCharaFixo(Character *character);
void SetFixoInversePositionRotation(Character *character, const Vector3 *position, const Vector3i *rotation);
std::int64_t SetFixoInverseChildPlacement(Character *character, std::uint16_t childId);
ObjectXFormsParams *SetFixoAlphaPropagation(Character *character, std::int32_t enabled);
void SetupObjectXForms(Character *character, void *payload);
void AllocateFIXO(Character *character);
#else
std::int16_t FindFixo(Character *character);
void CaptureFixoRelativePlacement(Character *character, ObjectXFormsParams *attachment, const Matrix4x4 *source);
void AttachFixoPreservingPlacement(Character *target, Character *source, std::int32_t boneIndex);
void SetFixoDataForMotionWork(
    Character *target, Character *source, std::int32_t boneIndex, const Vector3 &position, const std::array<std::int32_t, 3> &rotation);
std::uint64_t SetFixoLocalPosition(Character *character, const Vector3 *position);
std::uint64_t SetFixoLocalRotation(Character *character, const Vector3i *rotation);
ObjectXFormsParams *ResetCharaFixo(Character *character);
void DetachObjectXFormsDependents(Character *source);
void SetFixoInversePositionRotation(Character *character, const Vector3 *position, const Vector3i *rotation);
std::int64_t SetFixoInverseChildPlacement(Character *character, std::uint16_t childId);
ModelComponentHeader *SetFixoAlphaPropagation(Character *character, std::int32_t enabled);
bool AttachObjectXFormsComponent(Character *character);

namespace model_component_detail {

void DetachObjectXFormsVisitor(Character *character, void *user);
ObjectXFormsParams *FindObjectXForms(Character *character);
void SetupObjectXForms(Character *character, void *payload);

} // namespace model_component_detail
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AllocateFIXO(Character *character) {
    const shen_name token = SHEN_NAME4('F', 'I', 'X', 'O');
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(AllocateModelComponent(character, token, sizeof(ObjectXFormsParams), 0));
    attachment->mode = 0;
    *reinterpret_cast<std::int16_t *>(&attachment->transformFlags) = 0;
    attachment->localPosition.x = attachment->localPosition.y = attachment->localPosition.z = 0.0f;
    attachment->localRotation[0] = attachment->localRotation[1] = attachment->localRotation[2] = 0;
    attachment->multiplySourceAlpha = 0;
    SetModelComponentCore(character, token, SetupObjectXForms);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetupObjectXForms(Character *character, void *payload) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(payload);
    const Matrix4x4 *source;
    Matrix4x4 transform;
    Vector3 position;
    Vector3i rotation;
    switch (attachment->mode) {
    case 0:
        return;
    case 2:
        source = attachment->source.transform;
        break;
    case 1:
        if (HasCharacterMotionWork(attachment->sourceCharacter) &&
            (source = mtwkGetBoneTransform(attachment->sourceCharacter, attachment->source.motion.boneIndex)))
            break;
    default:
        matrix_stack::PushUnitMatrix();
        ApplyRecursiveCharacterNodeTransforms(GetCharacterNode(attachment->sourceCharacter));
        matrix_stack::GetMatrix(&transform);
        PopCurrentMatrix(1);
        source = &transform;
    }
    if (*reinterpret_cast<std::uint16_t *>(&attachment->transformFlags) & 1U) {
        matrix_stack::Matrix4x4_Push(source);
        Matrix4x4::preTranslate(attachment->position.x, attachment->position.y, attachment->position.z);
        Matrix4x4::SetViewMatrixFromEuler(attachment->rotation[0], attachment->rotation[1], attachment->rotation[2]);
        Matrix4x4::preTranslate(attachment->secondaryPosition.x, attachment->secondaryPosition.y, attachment->secondaryPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(attachment->secondaryRotation[0], attachment->secondaryRotation[1], attachment->secondaryRotation[2]);
        matrix_stack::GetMatrix(&transform);
        matrix_stack::GetTranslation(&position);
        PopCurrentMatrix(1);
        ExtractMatrixEulerAnglesFixed(&transform, &rotation);
    } else {
        position = attachment->position;
        rotation.x = attachment->rotation[0];
        rotation.y = attachment->rotation[1];
        rotation.z = attachment->rotation[2];
        TransformCharacterPoseByMatrix(source, &position, &rotation);
    }
    if (attachment->sourceCharacter) {
        if (attachment->multiplySourceAlpha)
            character->alpha = attachment->sourceCharacter->alpha * character->alpha;
        else
            character->alpha = attachment->sourceCharacter->alpha;
        if (!IsOfThisVisibilityType(attachment->sourceCharacter, CVM_All))
            character->alpha = 0.0f;
    }
    SetCharacterWorldPosition(character, &position);
    charaSetWorldRot(character, rotation);
}

#else

namespace model_component_detail {

ObjectXFormsParams *FindObjectXForms(Character *character) {
    ModelComponentHeader *const component = FindHeader(character, kObjectXFormsToken);
    return component ? reinterpret_cast<ObjectXFormsParams *>(Payload(component)) : nullptr;
}

// Native SetupObjectXForms: the character stands on its source, offset by the attachment's position and rotation (and, under flag 1,
// by the inverse placement SetCharaPosRot recorded). The source is a motion bone (mode 1, else the source's node chain), a borrowed
// matrix (mode 2) or the source's node chain (3). The character takes the source's alpha (times its own when +0x60 is set), or none
// while the source is hidden.
// @verified - p
void SetupObjectXForms(Character *character, void *payload) {
    auto *const objectXForms = static_cast<ObjectXFormsParams *>(payload);
    if (objectXForms->mode == 0)
        return;
    const Matrix4x4 *source = nullptr;
    Matrix4x4 nodeTransform;
    if (objectXForms->mode == 2) {
        source = objectXForms->source.transform;
    } else {
        if (objectXForms->mode == 1) {
            if (MotionWork *const motion = FindMotionWork(objectXForms->sourceCharacter))
                source = mtwkGetBoneTransform(motion, objectXForms->source.motion.boneIndex);
        }
        if (!source) {
            Matrix4x4_PushActiveTransform();
            ApplyRecursiveCharacterNodeTransforms(&objectXForms->sourceCharacter->entity.node);
            Matrix4x4::GetTransformMatrix(&nodeTransform);
            Matrix4x4::Pop(1);
            source = &nodeTransform;
        }
    }

    Vector3 position = objectXForms->position;
    Vector3i rotation{objectXForms->rotation[0], objectXForms->rotation[1], objectXForms->rotation[2]};
    Matrix4x4 transform;
    if (objectXForms->transformFlags & 1U) {
        Matrix4x4_Push(source);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
        Matrix4x4::preTranslate(objectXForms->secondaryPosition.x, objectXForms->secondaryPosition.y, objectXForms->secondaryPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(objectXForms->secondaryRotation[1], objectXForms->secondaryRotation[0], objectXForms->secondaryRotation[2]);
    } else {
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(source);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    }
    Matrix4x4::GetTransformMatrix(&transform);
    position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    ExtractMatrixEulerAnglesFixed(&transform, &rotation);
    shenApplyTranslationToAll(&character->entity.node, position);
    SetCharacterNodeWorldRotation(&character->entity.node, rotation);

    if (Character *const sourceCharacter = objectXForms->sourceCharacter) {
        character->alpha = objectXForms->multiplySourceAlpha ? sourceCharacter->alpha * character->alpha : sourceCharacter->alpha;
        if (!IsOfThisVisibilityType(sourceCharacter, 0))
            character->alpha = 0.0f;
    } else {
        character->alpha = 1.0f;
    }
}

} // namespace model_component_detail

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
bool AttachObjectXFormsComponent(Character *character) {
    ObjectXFormsParams *const attachment =
        static_cast<ObjectXFormsParams *>(AllocateModelComponent(character, kObjectXFormsToken, sizeof(ObjectXFormsParams), nullptr));
    if (!attachment)
        return false;
    SetModelComponentCore(character, kObjectXFormsToken, SetupObjectXForms);
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void CaptureFixoRelativePlacement(Character *character, ObjectXFormsParams *attachment, const Matrix4x4 *source) {
    matrix_stack::PushUnitMatrix();
    ApplyRecursiveCharacterNodeTransforms(GetCharacterNode(character));
    if (*reinterpret_cast<const std::int16_t *>(&attachment->transformFlags) & 1) {
        Matrix4x4::RotateAroundX_Wrapper(0U - static_cast<std::uint32_t>(attachment->secondaryRotation[0]));
        Matrix4x4::RotateAroundY_Wrapper(0U - static_cast<std::uint32_t>(attachment->secondaryRotation[1]));
        Matrix4x4::RotateAroundZ_Wrapper(0U - static_cast<std::uint32_t>(attachment->secondaryRotation[2]));
        Matrix4x4::preTranslate(-attachment->secondaryPosition.x, -attachment->secondaryPosition.y, -attachment->secondaryPosition.z);
    }
    matrix_stack::GetTranslation(&attachment->position);
    Matrix4x4 transform;
    matrix_stack::GetMatrix(&transform);
    PopCurrentMatrix(1);
    Vector3i *const rotation = reinterpret_cast<Vector3i *>(attachment->rotation);
    ExtractMatrixEulerAnglesFixed(&transform, rotation);
    InverseTransformCharacterPoseByMatrix(source, &attachment->position, rotation);
}

#else

// @verified - p
void CaptureFixoRelativePlacement(Character *character, ObjectXFormsParams *attachment, const Matrix4x4 *source) {
    Matrix4x4 transform;
    Matrix4x4_PushActiveTransform();
    ApplyCharacterEntityTransform(character);
    if (attachment->transformFlags & 1U) {
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-static_cast<std::int16_t>(attachment->secondaryRotation[0])));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-static_cast<std::int16_t>(attachment->secondaryRotation[1])));
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-static_cast<std::int16_t>(attachment->secondaryRotation[2])));
        Matrix4x4::preTranslate(-attachment->secondaryPosition.x, -attachment->secondaryPosition.y, -attachment->secondaryPosition.z);
    }
    attachment->position = {.x = workTransformMatrix.rows[3].x, .y = workTransformMatrix.rows[3].y, .z = workTransformMatrix.rows[3].z};
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::Pop(1);
    Vector3i rotation;
    ExtractMatrixEulerAnglesFixed(&transform, &rotation);
    for (unsigned index = 0; index < 3; ++index)
        attachment->rotation[index] = rotation[index];
    InverseTransformCharacterPoseByMatrix(source, &attachment->position, &rotation);
    for (unsigned index = 0; index < 3; ++index)
        attachment->rotation[index] = rotation[index];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void RefreshFixoRelativePlacement(Character *character, const Matrix4x4 *transform) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(character, CHARACTER_OBJECT_TRANSFORMS));
    attachment->mode = 2;
    attachment->source.transform = transform;
    attachment->sourceCharacter = 0;
    CaptureFixoRelativePlacement(character, attachment, transform);
}

// @verified - d
void AttachFixoPreservingPlacement(Character *target, Character *source, std::int32_t boneIndex) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(target, CHARACTER_OBJECT_TRANSFORMS));
    const Matrix4x4 *transform;
    if (!HasCharacterMotionWork(source) || !(transform = mtwkGetBoneTransform(source, boneIndex))) {
        attachment->mode = 3;
        attachment->sourceCharacter = source;
        matrix_stack::PushUnitMatrix();
        ApplyRecursiveCharacterNodeTransforms(GetCharacterNode(source));
        Matrix4x4 fallback;
        matrix_stack::GetMatrix(&fallback);
        PopCurrentMatrix(1);
        CaptureFixoRelativePlacement(target, attachment, &fallback);
    } else {
        attachment->mode = 1;
        attachment->source.motion.boneIndex = boneIndex;
        attachment->sourceCharacter = source;
        CaptureFixoRelativePlacement(target, attachment, transform);
    }
}

#else

// @verified - p
void AttachFixoPreservingPlacement(Character *target, Character *source, std::int32_t boneIndex) {
    ObjectXFormsParams *const attachment = FindObjectXForms(target);
    const Matrix4x4 *transform = nullptr;
    if (MotionWork *const motion = FindMotionWork(source))
        transform = mtwkGetBoneTransform(motion, boneIndex);
    Matrix4x4 fallback;
    if (transform) {
        attachment->source.motion.boneIndex = boneIndex;
        attachment->mode = 1;
        attachment->sourceCharacter = source;
    } else {
        attachment->sourceCharacter = source;
        attachment->mode = 3;
        Matrix4x4_PushActiveTransform();
        ApplyCharacterEntityTransform(source);
        Matrix4x4::GetTransformMatrix(&fallback);
        Matrix4x4::Pop(1);
        transform = &fallback;
    }
    CaptureFixoRelativePlacement(target, attachment, transform);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetFixoDataForMotionWork(Character *target, Character *source, std::int32_t boneIndex, const Vector3 &position, const std::int32_t (&rotation)[3]) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(target, CHARACTER_OBJECT_TRANSFORMS));
    if (HasCharacterMotionWork(source) && mtwkGetBoneTransform(source, boneIndex)) {
        attachment->mode = 1;
        attachment->source.motion.boneIndex = boneIndex;
        attachment->sourceCharacter = source;
    } else {
        attachment->mode = 3;
        attachment->sourceCharacter = source;
    }
    attachment->position = position;
    attachment->rotation[0] = rotation[0];
    attachment->rotation[1] = rotation[1];
    attachment->rotation[2] = rotation[2];
    attachment->localPosition = position;
    attachment->localRotation[0] = rotation[0];
    attachment->localRotation[1] = rotation[1];
    attachment->localRotation[2] = rotation[2];
}

#else

// @verified - p
void SetFixoDataForMotionWork(
    Character *target, Character *source, std::int32_t boneIndex, const Vector3 &position, const std::array<std::int32_t, 3> &rotation) {
    ObjectXFormsParams *const objectXForms = FindObjectXForms(target);
    MotionWork *const motion = FindMotionWork(source);
    std::int16_t mode = 3;
    if (motion && mtwkGetBoneTransform(motion, boneIndex)) {
        objectXForms->source.motion.boneIndex = boneIndex;
        mode = 1;
    }
    objectXForms->sourceCharacter = source;
    objectXForms->mode = mode;
    objectXForms->position = position;
    for (unsigned index = 0; index < 3; ++index)
        objectXForms->rotation[index] = rotation[index];
    objectXForms->localPosition = position;
    for (unsigned index = 0; index < 3; ++index)
        objectXForms->localRotation[index] = rotation[index];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
ObjectXFormsParams *ResetCharaFixo(Character *character) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(character, CHARACTER_OBJECT_TRANSFORMS));
    attachment->mode = 0;
    *reinterpret_cast<std::int16_t *>(&attachment->transformFlags) = 0;
    attachment->localPosition.x = attachment->localPosition.y = attachment->localPosition.z = 0.0f;
    attachment->localRotation[0] = attachment->localRotation[1] = attachment->localRotation[2] = 0;
    return attachment;
}

#else

// @verified - p
ObjectXFormsParams *ResetCharaFixo(Character *character) {
    ObjectXFormsParams *const objectXForms = FindObjectXForms(character);
    objectXForms->localRotation[2] = 0;
    objectXForms->mode = 0;
    objectXForms->transformFlags = 0;
    objectXForms->reserved33 = 0;
    objectXForms->sourceCharacter = nullptr;
    objectXForms->localPosition = {};
    objectXForms->localRotation[0] = 0;
    objectXForms->localRotation[1] = 0;
    character->alpha = 1.0f;
    return objectXForms;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace model_component_detail {

void DetachObjectXFormsVisitor(Character *character, void *user) {
    auto &context = *static_cast<DetachObjectXFormsContext *>(user);
    ObjectXFormsParams *const objectXForms = FindObjectXForms(character);
    if (!objectXForms || objectXForms->sourceCharacter != context.source) {
        return;
    }
    *objectXForms = {};
    modelSetCharaAlpha(character, 1.0f);
}

} // namespace model_component_detail

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Port-only, a documented deviation: native leaves dependents pointing at a freed source, which the port's heap-owned characters
// cannot survive, so a freed source's dependents are cleared.
void DetachObjectXFormsDependents(Character *source) {
    if (!source)
        return;
    DetachObjectXFormsContext context{source};
    ForEachLiveCharacterInMapOrder(DetachObjectXFormsVisitor, &context);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
ObjectXFormsParams *SetFixoLocalPosition(Character *character, const Vector3 *position) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(character, CHARACTER_OBJECT_TRANSFORMS));
    attachment->position.x -= attachment->localPosition.x;
    attachment->position.y -= attachment->localPosition.y;
    attachment->position.z -= attachment->localPosition.z;
    attachment->localPosition.x = position->x;
    attachment->localPosition.y = position->y;
    attachment->localPosition.z = position->z;
    attachment->position.x += attachment->localPosition.x;
    attachment->position.y += attachment->localPosition.y;
    attachment->position.z += attachment->localPosition.z;
    return attachment;
}

#else

// @verified - p
std::uint64_t SetFixoLocalPosition(Character *character, const Vector3 *position) {
    ObjectXFormsParams *const attachment = FindObjectXForms(character);
    const float x = attachment->position.x - attachment->localPosition.x;
    const float y = attachment->position.y - attachment->localPosition.y;
    const float z = attachment->position.z - attachment->localPosition.z;
    attachment->position = {.x = x, .y = y, .z = z};
    attachment->localPosition.x = position->x;
    const float nextX = x + attachment->localPosition.x;
    attachment->localPosition.y = position->y;
    const std::uint32_t result = std::bit_cast<std::uint32_t>(position->z);
    attachment->localPosition.z = std::bit_cast<float>(result);
    attachment->position.x = nextX;
    attachment->position.y = attachment->localPosition.y + y;
    attachment->position.z = attachment->localPosition.z + z;
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
ObjectXFormsParams *SetFixoLocalRotation(Character *character, const Vector3i *rotation) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(character, CHARACTER_OBJECT_TRANSFORMS));
    attachment->rotation[0] -= attachment->localRotation[0];
    attachment->rotation[1] -= attachment->localRotation[1];
    attachment->rotation[2] -= attachment->localRotation[2];
    attachment->localRotation[0] = rotation->x;
    attachment->localRotation[1] = rotation->y;
    attachment->localRotation[2] = rotation->z;
    attachment->rotation[0] += attachment->localRotation[0];
    attachment->rotation[1] += attachment->localRotation[1];
    attachment->rotation[2] += attachment->localRotation[2];
    return attachment;
}

Character *GetFixoSourceCharacter(Character *character) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(character, CHARACTER_OBJECT_TRANSFORMS));
    return attachment->sourceCharacter;
}

#else

// @verified - p
std::uint64_t SetFixoLocalRotation(Character *character, const Vector3i *rotation) {
    ObjectXFormsParams *const attachment = FindObjectXForms(character);
    for (unsigned index = 0; index < 3; ++index)
        attachment->rotation[index] =
            static_cast<std::int32_t>(static_cast<std::uint32_t>(attachment->rotation[index]) - static_cast<std::uint32_t>(attachment->localRotation[index]));
    const std::int32_t x = rotation->x;
    attachment->localRotation[0] = x;
    const std::uint32_t result = static_cast<std::uint32_t>(rotation->y);
    attachment->localRotation[1] = static_cast<std::int32_t>(result);
    const std::int32_t z = rotation->z;
    attachment->rotation[0] = static_cast<std::int32_t>(static_cast<std::uint32_t>(attachment->rotation[0]) + static_cast<std::uint32_t>(x));
    attachment->rotation[1] = static_cast<std::int32_t>(static_cast<std::uint32_t>(attachment->rotation[1]) + result);
    attachment->rotation[2] = static_cast<std::int32_t>(static_cast<std::uint32_t>(attachment->rotation[2]) + static_cast<std::uint32_t>(z));
    attachment->localRotation[2] = z;
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int16_t FindFixo(Character *character) {
    std::int16_t mode = 0;
    const ObjectXFormsParams *const attachment = reinterpret_cast<const ObjectXFormsParams *>(charFindComp(character, CHARACTER_OBJECT_TRANSFORMS));
    if (attachment)
        mode = static_cast<std::uint8_t>(attachment->mode);
    return mode;
}

void GetFixoLocalPosition(Character *character, Vector3 *position) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(character, CHARACTER_OBJECT_TRANSFORMS));
    position->x = attachment->localPosition.x;
    position->y = attachment->localPosition.y;
    position->z = attachment->localPosition.z;
}

void GetFixoLocalRotation(Character *character, Vector3i *rotation) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(character, CHARACTER_OBJECT_TRANSFORMS));
    rotation->x = attachment->localRotation[0];
    rotation->y = attachment->localRotation[1];
    rotation->z = attachment->localRotation[2];
}

#else

// Native FindFixo: the attachment mode, 0 without a FIXO component.
// @verified - p
std::int16_t FindFixo(Character *character) {
    const ObjectXFormsParams *const objectXForms = FindObjectXForms(character);
    return objectXForms ? objectXForms->mode : 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void SetFixoInversePositionRotation(Character *character, const Vector3 *position, const Vector3i *rotation) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(character, CHARACTER_OBJECT_TRANSFORMS));
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundX_Wrapper(0U - static_cast<std::uint32_t>(rotation->x));
    Matrix4x4::RotateAroundY_Wrapper(0U - static_cast<std::uint32_t>(rotation->y));
    Matrix4x4::RotateAroundZ_Wrapper(0U - static_cast<std::uint32_t>(rotation->z));
    Matrix4x4::preTranslate(-position->x, -position->y, -position->z);
    Matrix4x4 transform;
    matrix_stack::GetMatrix(&transform);
    matrix_stack::GetTranslation(&attachment->secondaryPosition);
    PopCurrentMatrix(1);
    ExtractMatrixEulerAnglesFixed(&transform, reinterpret_cast<Vector3i *>(attachment->secondaryRotation));
    *reinterpret_cast<std::int16_t *>(&attachment->transformFlags) |= 1;
}

#else

// Native SetCharaPosRot: the inverse of a placement, recorded as the attachment's secondary offset (flag 1).
// @verified - p
void SetFixoInversePositionRotation(Character *character, const Vector3 *position, const Vector3i *rotation) {
    ObjectXFormsParams *const objectXForms = FindObjectXForms(character);
    Matrix4x4 transform;
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-static_cast<std::int16_t>(rotation->x)));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-static_cast<std::int16_t>(rotation->y)));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-static_cast<std::int16_t>(rotation->z)));
    Matrix4x4::preTranslate(-position->x, -position->y, -position->z);
    Matrix4x4::GetTransformMatrix(&transform);
    objectXForms->secondaryPosition = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    Vector3i secondaryRotation;
    ExtractMatrixEulerAnglesFixed(&transform, &secondaryRotation);
    objectXForms->secondaryRotation[0] = secondaryRotation.x;
    objectXForms->secondaryRotation[1] = secondaryRotation.y;
    objectXForms->secondaryRotation[2] = secondaryRotation.z;
    objectXForms->transformFlags |= 1U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int64_t SetFixoInverseChildPlacement(Character *character, std::uint16_t childId) {
    MT7Node *const root = character->model;
    MT7Node *const child = FindMT7Node(root, static_cast<std::int16_t>(childId));
    if (!child)
        return 0;
    Vector3 position;
    Vector3i rotation;
    Matrix4x4 transform;
    matrix_stack::PushUnitMatrix();
    GetMT7NodeTransformMatrix(&transform, root, child);
    matrix_stack::LoadMatrix(&transform);
    matrix_stack::GetTranslation(&position);
    PopCurrentMatrix(1);
    ExtractMatrixEulerAnglesFixed(&transform, &rotation);
    SetFixoInversePositionRotation(character, &position, &rotation);
    return 1;
}

#else

// @verified - p
std::int64_t SetFixoInverseChildPlacement(Character *character, std::uint16_t childId) {
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    MT7Node *const root = &model->nodes.front();
    MT7Node *const child = GetChildObjectByIndex(*model, root, childId);
    if (!child)
        return 0;
    Matrix4x4 transform;
    Vector3 position;
    Vector3i rotation;
    Matrix4x4_PushActiveTransform();
    GetMT7NodeTransformMatrix(&transform, root, child);
    Matrix4x4::SetActiveTransform(&transform);
    position = {.x = workTransformMatrix.rows[3].x, .y = workTransformMatrix.rows[3].y, .z = workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    ExtractMatrixEulerAnglesFixed(&transform, &rotation);
    ObjectXFormsParams *const attachment = FindObjectXForms(character);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-static_cast<std::int16_t>(rotation.x)));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-static_cast<std::int16_t>(rotation.y)));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-static_cast<std::int16_t>(rotation.z)));
    Matrix4x4::preTranslate(-position.x, -position.y, -position.z);
    Matrix4x4 inverse;
    Matrix4x4::GetTransformMatrix(&inverse);
    attachment->secondaryPosition = {.x = workTransformMatrix.rows[3].x, .y = workTransformMatrix.rows[3].y, .z = workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    Vector3i inverseRotation;
    ExtractMatrixEulerAnglesFixed(&inverse, &inverseRotation);
    for (unsigned index = 0; index < 3; ++index)
        attachment->secondaryRotation[index] = inverseRotation[index];
    attachment->transformFlags |= 1U;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
ObjectXFormsParams *SetFixoAlphaPropagation(Character *character, std::int32_t enabled) {
    ObjectXFormsParams *const attachment = static_cast<ObjectXFormsParams *>(charFindComp(character, CHARACTER_OBJECT_TRANSFORMS));
    attachment->multiplySourceAlpha = static_cast<std::uint32_t>(enabled);
    return attachment;
}

#else

// @verified - p
ModelComponentHeader *SetFixoAlphaPropagation(Character *character, std::int32_t enabled) {
    ModelComponentHeader *const component = FindHeader(character, kObjectXFormsToken);
    reinterpret_cast<ObjectXFormsParams *>(Payload(component))->multiplySourceAlpha = static_cast<std::uint32_t>(enabled);
    return component;
}

#endif

} // namespace s2
} // namespace shenmue
