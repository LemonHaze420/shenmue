#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_dc.hpp"
#include "s2/face_data.hpp"
#include "s2/model_dc.hpp"

namespace shenmue {
namespace s2 {

#define CHARACTER_FACE SHEN_NAME4('F', 'A', 'C', 'E')
const char *GetCharacterFaceNameNullsub(const Character *character);
std::uint32_t GetCharacterFaceIndexNullsub(const char *name);
void SetCharacterFaceParametersNullsub(const char *name, std::uint8_t mode, std::int16_t first, std::int16_t second);
void SetCharacterFaceSecondaryParametersNullsub(const void *parameters, std::uint8_t mode, std::int16_t first, std::uint8_t second);
std::int32_t GetCharacterFaceCacheIndex(const Character *character);
void ReleaseCharacterFaceCache(const Character *character);
void SetCharacterFaceParameters(const Character *character, std::int32_t mode, std::int32_t first, std::int32_t second);
void SetCharacterFaceSecondaryParameters(const Character *character, std::uint8_t mode, std::int16_t first, std::uint8_t second);
static const std::uint8_t facialModelCacheCount = 8;
extern const char facialModelCacheEmptyName[5];
static const char facialModelCachePersistentName[] = "YKB_F.MT7";
struct FacialModelCacheEntry {
    MT7Node *root;
    MT7Model *model;
    char name[16];
    std::int16_t references;
    std::byte retained1a[2];
};
typedef char Layout_face_controller_dc_0[(sizeof(FacialModelCacheEntry) == 0x1c) ? 1 : -1];
typedef char Layout_face_controller_dc_1[(offsetof(FacialModelCacheEntry, model) == 4) ? 1 : -1];
typedef char Layout_face_controller_dc_2[(offsetof(FacialModelCacheEntry, name) == 8) ? 1 : -1];
typedef char Layout_face_controller_dc_3[(offsetof(FacialModelCacheEntry, references) == 0x18) ? 1 : -1];
extern FacialModelCacheEntry facialModelCache[facialModelCacheCount];
std::uint32_t FindCachedFacialModelIndex(const char *name);
std::int32_t RetainCachedFacialModelNullsub(const MT7Model *model);
void FacialModelCacheReleaseNullsub(std::uint8_t index, std::int32_t released);
void ReleaseCachedFacialModel(const char *name, std::uint8_t index);
void ResetCachedFacialModels();

extern const std::int8_t clipMouthExpressions[16];
static const std::int16_t defaultFacialClip[6] = {-1, -1, 0, 0, 0, 0};

struct FacialMorphVertex {
    Vector3 neutral, start, target;
    float weight0, weight1;
    Vector3 *position;
};
typedef char Layout_face_controller_dc_4[(sizeof(FacialMorphVertex) == 0x30) ? 1 : -1];
typedef char Layout_face_controller_dc_5[(offsetof(FacialMorphVertex, start) == 0x0c) ? 1 : -1];
typedef char Layout_face_controller_dc_6[(offsetof(FacialMorphVertex, target) == 0x18) ? 1 : -1];
typedef char Layout_face_controller_dc_7[(offsetof(FacialMorphVertex, position) == 0x2c) ? 1 : -1];

struct FacialEyeControl {
    std::uint8_t state[4];
    std::int16_t parameters[4];
    std::int16_t enabled;
    std::uint8_t rotationFlags, retainedFlags;
    Vector3 target;
    MT7Node *headNode, *eyeNode;
    std::uint32_t retainedState;
    Vector3i headRotation, eyeRotation, targetHeadRotation, targetEyeRotation, headRotationStep;
    std::int32_t eyeRotationStep[2];
    std::int32_t pitchUpper, pitchLower, headYawUpper, headYawLower, eyeYawUpper, eyeYawLower;
    std::int16_t transitionFrames, elapsedFrames;
};
struct FacialTaskData {
    const std::int16_t *clip;
    std::int32_t clipFrame, clipDuration, blinkInterval, blinkFrame, retainedState, enabled, clipLeadIn;
    std::uint16_t focusId, retainedFocusState;
    FacialEyeControl eyes;
};
typedef char Layout_face_controller_dc_8[(sizeof(FacialEyeControl) == 0x88) ? 1 : -1];
typedef char Layout_face_controller_dc_9[(offsetof(FacialEyeControl, rotationFlags) == 0x0e) ? 1 : -1];
typedef char Layout_face_controller_dc_10[(offsetof(FacialEyeControl, target) == 0x10) ? 1 : -1];
typedef char Layout_face_controller_dc_11[(offsetof(FacialEyeControl, headNode) == 0x1c) ? 1 : -1];
typedef char Layout_face_controller_dc_12[(offsetof(FacialEyeControl, eyeNode) == 0x20) ? 1 : -1];
typedef char Layout_face_controller_dc_13[(offsetof(FacialEyeControl, headRotation) == 0x28) ? 1 : -1];
typedef char Layout_face_controller_dc_14[(offsetof(FacialEyeControl, pitchUpper) == 0x6c) ? 1 : -1];
typedef char Layout_face_controller_dc_15[(offsetof(FacialEyeControl, transitionFrames) == 0x84) ? 1 : -1];
typedef char Layout_face_controller_dc_16[(sizeof(FacialTaskData) == 0xac) ? 1 : -1];
typedef char Layout_face_controller_dc_17[(offsetof(FacialTaskData, clipFrame) == 4) ? 1 : -1];
typedef char Layout_face_controller_dc_18[(offsetof(FacialTaskData, enabled) == 0x18) ? 1 : -1];
typedef char Layout_face_controller_dc_19[(offsetof(FacialTaskData, clipLeadIn) == 0x1c) ? 1 : -1];
typedef char Layout_face_controller_dc_20[(offsetof(FacialTaskData, focusId) == 0x20) ? 1 : -1];
typedef char Layout_face_controller_dc_21[(offsetof(FacialTaskData, eyes) == 0x24) ? 1 : -1];
typedef char Layout_face_controller_dc_22[(offsetof(FacialModelData, upperFaceFrame) == 0x28) ? 1 : -1];
typedef char Layout_face_controller_dc_23[(offsetof(FacialModelData, lowerFaceFrame) == 0x30) ? 1 : -1];
typedef char Layout_face_controller_dc_24[(offsetof(FacialModelData, eyelidFrame) == 0x38) ? 1 : -1];
typedef char Layout_face_controller_dc_25[(offsetof(FacialModelData, mouthFrame) == 0x40) ? 1 : -1];
extern std::uint32_t eyeControlFlags;
extern Vector3 g_facialFocusTarget;
extern std::uint16_t g_facialFocusId;
static const std::uint16_t facialFocusNone = 129;
std::int32_t GetEyeLookHeadYaw(Character *character);
void ResolveFacialEyeTarget(Character *character, FacialEyeControl &eyes);
void AdvanceFacialEyeRotation(FacialEyeControl &eyes);
void SetEyeToChara(Character *character, const Vector3 *target, std::int16_t frames, std::int8_t mode);
void SetFacialEyelidPitch(FacialModelData &face, float pitch);
void SetCharacterEyelidPitch(Character *character, float pitch);
MT7Node *FindFacialSubnode(MT7Node *node, std::int16_t id);
void InitializeFacialEyeControl(FacialEyeControl &eyes, MT7Node *root, const FacialModelData &face);
void UpdateFacialTask(Character *character, void *payload);
void AttachFacialController(Character *character);
std::uint8_t AssignFacialClip(Character *character, const std::int16_t *clip);
void SetFacialBlinkDelay(Character *character, std::int32_t delay);
void SetFaceFlagsForChara(Character *character, std::int32_t enabled);
void InterpolateFacialGroup(FacialMorphVertex *vertices, std::int32_t count, float amount);
void InterpolateFacialGroup(FacialMorphVertex *vertices, std::int32_t count, std::int32_t frame, std::int32_t duration);
void FinishFacialGroup(FacialMorphVertex *vertices, std::int32_t count);
void InterpolateUpperFace(FacialModelData &face);
void FinishUpperFace(FacialModelData &face);
void InterpolateLowerFace(FacialModelData &face);
void FinishLowerFace(FacialModelData &face);
void InterpolateEyelids(FacialModelData &face);
void InterpolateClosingEyelids(FacialModelData &face);
void InterpolateOpeningEyelids(FacialModelData &face);
void FinishEyelids(FacialModelData &face);
void InterpolateMouth(FacialModelData &face);
void FinishMouth(FacialModelData &face);
void SnapshotFacialTarget(FacialMorphVertex &vertex, float duration);
void PrepareOuterEyelids(FacialModelData &face, float duration, float first, float second);
void PrepareInnerEyelids(FacialModelData &face, float duration, float first, float second);
void SetOuterEyelidWeights(FacialModelData &face, float duration, float first, float second);
void SetInnerEyelidWeights(FacialModelData &face, float duration, float first, float second);
void BeginFacialBlink(FacialModelData &face);
void BeginFacialBlinkRecovery(FacialModelData &face);
void AdvanceFacialModel(FacialModelData &face);
void PrepareUpperFace(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale);
void PrepareLowerFace(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale);
void SetUpperFacialExpression(Character *character, std::int32_t expression, std::int32_t transitionFrames, float scale);
void SetLowerFacialExpression(Character *character, std::int32_t expression, std::int32_t transitionFrames, float scale);
void SetHighModelAndFaceFlagsForChara(Character *character, std::int32_t expression, std::int32_t transitionFrames, float scale);
void SetFacialMouthExpression(Character *character, std::int32_t expression, std::int32_t transitionFrames, float scale);
typedef void (*FacialShapeModifier)(const FacialModelData &, FacialMorphVertex &);
extern FacialShapeModifier mouthModifiers[3];
void SelectMouthModifiers(const FacialModelData &face);
typedef std::int32_t FacialMouthExpression;
enum {
    FacialMouthExpression_Closed,
    FacialMouthExpression_A,
    FacialMouthExpression_I,
    FacialMouthExpression_U,
    FacialMouthExpression_E,
    FacialMouthExpression_O,
    FacialMouthExpression_LeadIn
};
static const float facialEyelidWeights[21][4] = {{0, 0, 0, 0}, {0.25f, 0.25f, 0, 0}, {0.3f, 0.3f, 0.3f, 0.3f}, {0.1f, 0.1f, 0.5f, 0.5f},
    {-0.5f, -0.5f, -0.2f, -0.2f}, {0.2f, 0.2f, 0.1f, 0.1f}, {0.8f, 0.25f, 0.6f, 0.2f}, {0.2f, 0.2f, 0.1f, 0.1f}, {1, 1, 0, 0}, {0.5f, 0.5f, 0.2f, 0.2f},
    {0.1f, 0.1f, 0.5f, 0.5f}, {1.1f, 1.1f, 1.1f, 1.1f}, {-0.2f, -0.2f, 0, 0}, {-0.1f, -0.1f, 0.1f, 0.1f}, {0.25f, 0.25f, 0.8f, 0.8f},
    {0.056f, 0.056f, 0.56f, 0.56f}, {-0.2f, -1, 1, 0.2f}, {-1, -0.2f, 0.2f, 1}, {-1, -1, 0.8f, 0.8f}, {0.1f, 0.1f, 0.5f, 0.5f}, {1.1f, 1.1f, 1.1f, 1.1f}};
static const std::uint32_t facialMouthInnerDirty = 0x80U;
std::int32_t BeginNeutralFacialGroup(FacialMorphVertex *vertices, std::int32_t count, float duration);
void PrepareFacialMouth(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale);
void PrepareMouthClosed(FacialModelData &face, std::int32_t duration);
void PrepareMouthA(FacialModelData &face, std::int32_t duration, float scale);
void PrepareMouthI(FacialModelData &face, std::int32_t duration, float scale);
void PrepareMouthU(FacialModelData &face, std::int32_t duration, float scale);
void PrepareMouthE(FacialModelData &face, std::int32_t duration, float scale);
void PrepareMouthO(FacialModelData &face, std::int32_t duration, float scale);
void PrepareMouthLeadIn(FacialModelData &face, std::int32_t duration, float scale);
void StartFacialBlink(Character *character);
void UpdateFacialMorphs(Character *character);
void facxApplyEyeControl(Character *character, FacialTaskData &task);

} // namespace s2
} // namespace shenmue

#endif
