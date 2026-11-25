#include "s2/face_controller_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/model_image_component_dc.hpp"

namespace shenmue {
namespace s2 {

const std::int8_t clipMouthExpressions[16] = {0, 3, 2, 5, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// @verified - d
const char *GetCharacterFaceNameNullsub(const Character *) {
    return 0;
}

// @verified - d
std::uint32_t GetCharacterFaceIndexNullsub(const char *) {
    return 0;
}

// @verified - d
void SetCharacterFaceParametersNullsub(const char *, std::uint8_t, std::int16_t, std::int16_t) {
}

// @verified - d
void SetCharacterFaceSecondaryParametersNullsub(const void *, std::uint8_t, std::int16_t, std::uint8_t) {
}

std::uint32_t eyeControlFlags;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void FacialControllerNullsub() {
}

std::int32_t FacialControllerZeroNullsub() {
    return 0;
}
#endif

// @verified - d
MT7Node *FindFacialSubnode(MT7Node *node, std::int16_t id) {
    if (node->id == id)
        return node;
    if (node->child) {
        MT7Node *const found = FindFacialSubnode(node->child, id);
        if (found->id == id)
            return found;
    }
    if (node->sibling) {
        MT7Node *const found = FindFacialSubnode(node->sibling, id);
        if (found->id == id)
            return found;
    }
    return node;
}

// @verified - d
void InitializeFacialEyeControl(FacialEyeControl &eyes, MT7Node *root, const FacialModelData &face) {
    eyes.state[0] = 0;
    eyes.state[1] = 0;
    eyes.state[2] = 0;
    eyes.state[3] = 0;
    eyes.parameters[0] = 0;
    eyes.parameters[1] = 0;
    eyes.parameters[2] = 0;
    eyes.parameters[3] = 0;
    eyes.enabled = 1;
    eyes.rotationFlags = 0;
    eyes.target.x = 0.0f;
    eyes.target.y = 0.0f;
    eyes.target.z = 0.0f;
    eyes.eyeNode = eyes.headNode = 0;
    eyes.headRotation.y = eyes.headRotation.x = 0;
    eyes.eyeRotation.y = eyes.eyeRotation.x = 0;
    eyes.targetHeadRotation.y = eyes.targetHeadRotation.x = 0;
    eyes.targetEyeRotation.y = eyes.targetEyeRotation.x = 0;
    eyes.headRotationStep.y = eyes.headRotationStep.x = 0;
    eyes.eyeRotationStep[1] = eyes.eyeRotationStep[0] = 0;
    eyes.pitchUpper = face.lookLimits[1];
    eyes.pitchLower = static_cast<std::int32_t>(0U - static_cast<std::uint32_t>(face.lookLimits[0]));
    eyes.headYawUpper = face.lookLimits[2];
    eyes.headYawLower = static_cast<std::int32_t>(0U - static_cast<std::uint32_t>(face.lookLimits[3]));
    eyes.eyeYawUpper = face.lookLimits[5];
    eyes.eyeYawLower = static_cast<std::int32_t>(0U - static_cast<std::uint32_t>(face.lookLimits[4]));
    eyes.transitionFrames = 0;
    eyes.elapsedFrames = 0;
    eyes.headNode = FindFacialSubnode(root, 0x51);
    if (eyes.headNode == root) {
        eyes.headNode = FindFacialSubnode(root, 0x4d);
        if (eyes.headNode == root)
            eyes.headNode = 0;
    }
    eyes.eyeNode = FindFacialSubnode(root, 0x52);
    if (eyes.eyeNode == root) {
        eyes.eyeNode = FindFacialSubnode(root, 0x4e);
        if (eyes.eyeNode == root)
            eyes.eyeNode = 0;
    }
}

// @verified - d
void UpdateFacialTask(Character *character, void *payload) {
    FacialTaskData &task = *static_cast<FacialTaskData *>(payload);
    if (task.clip) {
        task.clipFrame = static_cast<std::int32_t>(static_cast<std::uint32_t>(task.clipFrame) + 1U);
        if (task.clipFrame > task.clipDuration) {
            if (task.clip[0] < 0) {
                task.clip = 0;
                task.blinkFrame = 0;
            } else {
                const std::int16_t code = task.clip[0];
                const std::int32_t duration = task.clip[1] / 2;
                task.clipDuration = duration;
                task.clipFrame = 1;
                std::int32_t expression = clipMouthExpressions[code & 15];
                std::int32_t transitionFrames = 2;
                if (task.clipLeadIn) {
                    task.clipLeadIn = 0;
                    task.clipDuration = duration - 2;
                } else if ((code & 0xfff0) && expression != 3 && task.clipDuration > 2) {
                    task.clipDuration = 2;
                    task.clipLeadIn = 1;
                    expression = 6;
                }
                if (transitionFrames < 4)
                    transitionFrames = 4;
                if (!task.clipLeadIn)
                    task.clip += 2;
                SetFacialMouthExpression(character, expression, transitionFrames, 1.0f);
            }
        }
    } else if (task.blinkInterval) {
        task.blinkFrame = static_cast<std::int32_t>(static_cast<std::uint32_t>(task.blinkFrame) + 1U);
        if (task.blinkFrame > task.blinkInterval) {
            StartFacialBlink(character);
            task.blinkFrame = 0;
        }
    }
    UpdateFacialMorphs(character);
    facxApplyEyeControl(character, task);
}

// @verified - d
void AttachFacialController(Character *character) {
    *reinterpret_cast<std::uint8_t *>(&eyeControlFlags) = 0;
    if (!HasCharacterFacialModel(character))
        return;
    FacialTaskData *const task = static_cast<FacialTaskData *>(AllocateModelComponent(character, CHARACTER_FACE, sizeof(FacialTaskData), UpdateFacialTask));
    const MT7Model *const source = GetImageModuleSource(character);
    const FacialModelData *const face = source ? reinterpret_cast<const FacialModelData *>(source->face) : 0;
    if (task && face) {
        task->clip = 0;
        task->clipFrame = 0;
        task->clipDuration = 0;
        task->blinkInterval = 270;
        task->enabled = 1;
        task->blinkFrame = 0;
        task->retainedState = 0;
        task->clipLeadIn = 0;
        task->focusId = facialFocusNone;
        InitializeFacialEyeControl(task->eyes, character->model, *face);
    }
}

// @verified - d
std::uint8_t AssignFacialClip(Character *character, const std::int16_t *clip) {
    std::int32_t assigned = 0;
    if (character) {
        FacialTaskData *const task = static_cast<FacialTaskData *>(charFindComp(character, CHARACTER_FACE));
        if (task && clip[0] >= 0) {
            task->clip = clip;
            task->clipFrame = 1;
            task->clipDuration = 0;
            task->clipLeadIn = 0;
            assigned = 1;
        }
    }
    return assigned;
}

// @verified - d
void SetFacialBlinkDelay(Character *character, std::int32_t delay) {
    if (character) {
        FacialTaskData *const task = static_cast<FacialTaskData *>(charFindComp(character, CHARACTER_FACE));
        if (task)
            task->blinkInterval = delay;
    }
}

// @verified - d
void SetFaceFlagsForChara(Character *character, std::int32_t enabled) {
    if (character) {
        FacialTaskData *const task = static_cast<FacialTaskData *>(charFindComp(character, CHARACTER_FACE));
        if (task)
            task->enabled = enabled;
    }
}

} // namespace s2
} // namespace shenmue

#endif
