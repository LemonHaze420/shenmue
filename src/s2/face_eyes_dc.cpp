#include "s2/face_controller_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/matrix_stack.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/motion_work_dc.hpp"
#include <cmath>
#include <cstring>

namespace shenmue {
namespace s2 {

void SetCharacterEyelidPitch(Character *character, float pitch);

Vector3 g_facialFocusTarget = {};

std::uint16_t g_facialFocusId = 0;

// @verified - d
std::int32_t GetEyeLookHeadYaw(Character *character) {
    matrix_stack::Matrix4x4_Push();
    matrix_stack::LoadMatrix(mtwkGetBoneTransform(character, 5));
    Vector3 origin, shifted;
    matrix_stack::GetTranslation(&origin);
    Matrix4x4::preTranslate(100.0f, 0.0f, 0.0f);
    matrix_stack::GetTranslation(&shifted);
    PopCurrentMatrix(1);
    const float x = shifted.x - origin.x, z = shifted.z - origin.z;
    if (!(((x) * (x) + (z * z)) < 1.0f))
        return dc::sdk::ArcTan2Fixed(-z, x);
    return mtwkGetEntityYaw(charFindMotionwork(character));
}

// @verified - d
void ResolveFacialEyeTarget(Character *character, FacialEyeControl &eyes) {
    MT7Node *node;
    if (!character || !&eyes || !mtwkGetBoneTransform(character, 5) || !(node = eyes.headNode))
        return;
    if (eyes.rotationFlags & 0x7fU) {
        Vector3 difference, target, origin, shifted;
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(mtwkGetBoneTransform(character, 5));
        matrix_stack::GetTranslation(&origin);
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        matrix_stack::GetTranslation(&shifted);
        PopCurrentMatrix(1);
        origin.y = shifted.y;
        const std::int16_t baseAngle = static_cast<std::int16_t>(-GetEyeLookHeadYaw(character));
        target.x = eyes.target.x;
        target.y = eyes.target.y;
        target.z = eyes.target.z;
        difference.y = origin.y - target.y;
        float x = origin.x - target.x, z = origin.z - target.z, squared = x * x + z * z;
        const float horizontal = std::sqrt(squared);
        difference.z = horizontal;
        eyes.targetHeadRotation.x = static_cast<std::int32_t>(dc::sdk::ArcTan2Fixed(difference.y, difference.z));
        if (eyes.targetHeadRotation.x & 0x8000)
            eyes.targetHeadRotation.x |= static_cast<std::int32_t>(0xffff0000U);
        eyes.targetHeadRotation.x /= 4;
        eyes.targetEyeRotation.x = eyes.targetHeadRotation.x;
        difference.x = target.x - origin.x;
        difference.z = target.z - origin.z;
        std::int16_t yaw = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-difference.z, difference.x));
        yaw = baseAngle + yaw;
        yaw /= 4;
        eyes.targetHeadRotation.y = static_cast<std::uint16_t>(yaw);
        eyes.targetEyeRotation.y = static_cast<std::uint16_t>(yaw);
    }
    std::int32_t duration = static_cast<std::int16_t>(eyes.transitionFrames - eyes.elapsedFrames);
    if (duration < 1)
        duration = 1;
    if (eyes.targetHeadRotation.x & 0x8000) {
        eyes.targetHeadRotation.x |= static_cast<std::int32_t>(0xffff0000U);
        eyes.targetEyeRotation.x |= static_cast<std::int32_t>(0xffff0000U);
    }
    if (eyes.targetHeadRotation.y & 0x8000)
        eyes.targetHeadRotation.y |= static_cast<std::int32_t>(0xffff0000U);
    if (eyes.targetEyeRotation.y & 0x8000)
        eyes.targetEyeRotation.y |= static_cast<std::int32_t>(0xffff0000U);
    if (eyes.targetHeadRotation.x > eyes.pitchUpper) {
        eyes.targetHeadRotation.x = eyes.pitchUpper;
        eyes.targetEyeRotation.x = eyes.pitchUpper;
    } else if (eyes.targetHeadRotation.x < eyes.pitchLower) {
        eyes.targetHeadRotation.x = eyes.pitchLower;
        eyes.targetEyeRotation.x = eyes.pitchLower;
    }
    if (eyes.targetHeadRotation.y > eyes.headYawUpper)
        eyes.targetHeadRotation.y = eyes.headYawUpper;
    else if (eyes.targetHeadRotation.y < eyes.headYawLower)
        eyes.targetHeadRotation.y = eyes.headYawLower;
    if (eyes.targetEyeRotation.y > eyes.eyeYawUpper)
        eyes.targetEyeRotation.y = eyes.eyeYawUpper;
    else if (eyes.targetEyeRotation.y < eyes.eyeYawLower)
        eyes.targetEyeRotation.y = eyes.eyeYawLower;
    const std::int32_t headPitchDifference = static_cast<std::int32_t>(static_cast<std::uint32_t>(eyes.targetHeadRotation.x) - eyes.headRotation.x);
    eyes.headRotationStep.x = headPitchDifference / duration;
    eyes.headRotationStep.y = (static_cast<std::int16_t>(eyes.targetHeadRotation.y) - static_cast<std::int16_t>(eyes.headRotation.y)) / duration;
    const std::int32_t eyePitchDifference = static_cast<std::int32_t>(static_cast<std::uint32_t>(eyes.targetEyeRotation.x) - eyes.eyeRotation.x);
    eyes.eyeRotationStep[0] = eyePitchDifference / duration;
    eyes.eyeRotationStep[1] = (static_cast<std::int16_t>(eyes.targetEyeRotation.y) - static_cast<std::int16_t>(eyes.eyeRotation.y)) / duration;
}

// @verified - d
void SetEyeToChara(Character *character, const Vector3 *target, std::int16_t frames, std::int8_t mode) {
    if (!character)
        return;
    FacialTaskData *const task = static_cast<FacialTaskData *>(charFindComp(character, CHARACTER_FACE));
    if (!task)
        return;
    FacialEyeControl &eyes = task->eyes;
    eyes.rotationFlags = static_cast<std::uint8_t>(mode | 0x80);
    eyes.transitionFrames = frames;
    eyes.elapsedFrames = 0;
    eyes.target.x = 0.0f;
    eyes.target.y = 0.0f;
    eyes.target.z = 0.0f;
    if (mode == 0) {
        eyes.targetHeadRotation.x = 0;
        eyes.targetHeadRotation.y = 0;
        eyes.targetEyeRotation.x = 0;
        eyes.targetEyeRotation.y = 0;
    } else if (target) {
        eyes.target.x = target->x;
        eyes.target.y = target->y;
        eyes.target.z = target->z;
    }
    ResolveFacialEyeTarget(character, eyes);
}

// @verified - d
void AdvanceFacialEyeRotation(FacialEyeControl &eyes) {
    if (++eyes.elapsedFrames <= eyes.transitionFrames) {
        eyes.headRotation.x = static_cast<std::int32_t>(static_cast<std::uint32_t>(eyes.headRotation.x) + eyes.headRotationStep.x);
        eyes.headRotation.y = static_cast<std::int32_t>(static_cast<std::uint32_t>(eyes.headRotation.y) + eyes.headRotationStep.y);
        eyes.eyeRotation.x = static_cast<std::int32_t>(static_cast<std::uint32_t>(eyes.eyeRotation.x) + eyes.eyeRotationStep[0]);
        eyes.eyeRotation.y = static_cast<std::int32_t>(static_cast<std::uint32_t>(eyes.eyeRotation.y) + eyes.eyeRotationStep[1]);
    } else {
        const std::uint8_t mode = eyes.rotationFlags & 0x7fU;
        if (mode != 2)
            eyes.rotationFlags = mode;
        eyes.elapsedFrames = eyes.transitionFrames;
        eyes.headRotation.x = eyes.targetHeadRotation.x;
        eyes.headRotation.y = eyes.targetHeadRotation.y;
        eyes.eyeRotation.x = eyes.targetEyeRotation.x;
        eyes.eyeRotation.y = eyes.targetEyeRotation.y;
    }
    if (*reinterpret_cast<const std::uint8_t *>(&eyeControlFlags) & 1U) {
        if (eyes.headRotation.x > eyes.pitchUpper) {
            eyes.headRotation.x = eyes.pitchUpper;
            eyes.eyeRotation.x = eyes.pitchUpper;
        } else if (eyes.headRotation.x < eyes.pitchLower) {
            eyes.headRotation.x = eyes.pitchLower;
            eyes.eyeRotation.x = eyes.pitchLower;
        }
        if (eyes.headRotation.y > eyes.headYawUpper)
            eyes.headRotation.y = eyes.headYawUpper;
        else if (eyes.headRotation.y < eyes.headYawLower)
            eyes.headRotation.y = eyes.headYawLower;
        if (eyes.eyeRotation.y > eyes.eyeYawUpper)
            eyes.eyeRotation.y = eyes.eyeYawUpper;
        else if (eyes.eyeRotation.y < eyes.eyeYawLower)
            eyes.eyeRotation.y = eyes.eyeYawLower;
    }
}

// @verified - d
void SetFacialEyelidPitch(FacialModelData &face, float pitch) {
    face.shapeCoefficients[29] = pitch * 0.7f;
    if (face.eyelid0Mode) {
        const float duration = face.eyelidDuration == 0 ? 0.0f : static_cast<float>(face.eyelidDuration);
        if (!(face.flags & 0x600U))
            PrepareOuterEyelids(face, duration, face.eyelid0OuterWeight, face.eyelid1OuterWeight);
    }
}

// @verified - d
void facxApplyEyeControl(Character *character, FacialTaskData &task) {
    FacialEyeControl &eyes = task.eyes;
    switch (eyes.rotationFlags & 0x7fU) {
    case 2:
        ResolveFacialEyeTarget(character, eyes);
        break;
    case 1:
        if (static_cast<std::int16_t>(g_facialFocusId) == facialFocusNone) {
            task.focusId = facialFocusNone;
        } else if (static_cast<std::int16_t>(task.focusId) != static_cast<std::int16_t>(g_facialFocusId)) {
            task.focusId = g_facialFocusId;
            const void *const clip = charFindComp(character, CHARACTER_CLIPPER);
            if (clip) {
                if (static_cast<std::int16_t>(g_facialFocusId) != *reinterpret_cast<const std::int16_t *>(static_cast<const char *>(clip) + 0x0a))
                    SetEyeToChara(character, &g_facialFocusTarget, 8, 1);
            }
        } else {
            ResolveFacialEyeTarget(character, eyes);
        }
        break;
    }
    if (!(eyes.rotationFlags & 0x80U))
        return;
    AdvanceFacialEyeRotation(eyes);
    if (eyes.headRotation.x > 0 && eyes.pitchUpper)
        SetCharacterEyelidPitch(character, static_cast<float>(eyes.headRotation.x) / static_cast<float>(eyes.pitchUpper));
    else
        SetCharacterEyelidPitch(character, 0.0f);
    MT7Node *node = eyes.headNode;
    if (node) {
        node->rotation.x = 0;
        node->rotation.y = eyes.headRotation.x;
        node->rotation.z = eyes.headRotation.y;
    }
    node = eyes.eyeNode;
    if (node) {
        node->rotation.x = 0;
        node->rotation.y = eyes.eyeRotation.x;
        node->rotation.z = eyes.eyeRotation.y;
    }
}

} // namespace s2
} // namespace shenmue

#endif
