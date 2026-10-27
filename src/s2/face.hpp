#pragma once

#include "s2/face_expression.hpp"

#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace shenmue::s2 {

struct Character;
struct MT7Model;
struct MT7Node;
struct LegacyVertexBufferBinding;

inline constexpr std::uint16_t facialFocusNone = 129;
inline constexpr std::size_t characterClipFocusIdOffset = 0x12;
inline Vector3 g_facialFocusTarget{};
inline std::uint16_t g_facialFocusId{};

enum FacialNodeSelection : std::int32_t {
    FacialNodeOriginal = 0,
    FacialNodeReplacement = 1,
};



#pragma pack(push, 4)
struct FacialMorphGroup {
    std::int32_t count;
    std::int32_t offset;
};


struct FacialModelData {
    std::uint32_t signature;
    std::uint32_t size;
    std::uint32_t version;
    std::int32_t modelOffset;
    std::int32_t reserved10;
    std::int32_t reserved14;
    std::uint32_t reserved18;
    std::int32_t upperExpression;
    std::int32_t expression;
    std::int32_t mouthExpression;
    float upperFaceFrame;
    float upperFaceDuration;
    float lowerFaceFrame;
    float lowerFaceDuration;
    float eyelidFrame;
    float eyelidDuration;
    float mouthFrame;
    float mouthDuration;
    std::uint32_t flags;
    float expressionScale;
    float eyelid0OuterWeight;
    float eyelid1OuterWeight;
    float eyelid0InnerWeight;
    float eyelid1InnerWeight;
    std::array<float, 30> shapeCoefficients;
    std::int32_t eyelid0Mode;
    std::int32_t eyelid1Mode;
    std::array<std::int32_t, 6> lookLimits;
    std::array<FacialMorphGroup, 10> groups;
};

struct FacialMorphVertex {
    Vector3 neutral;
    Vector3 start;
    Vector3 target;
    float weight0;
    float weight1;
    std::int32_t positionOffset;
    std::int32_t bufferOffset;
};

struct FacialEyeControl {
    std::array<std::byte, 14> reserved00;
    std::uint8_t rotationFlags;
    std::uint8_t reserved0f;
    Vector3 target;
    std::uint32_t reserved1c;
    MT7Node *headNode;
    MT7Node *eyeNode;
    std::uint32_t reserved30;
    Vector3i headRotation;
    Vector3i eyeRotation;
    Vector3i targetHeadRotation;
    Vector3i targetEyeRotation;
    Vector3i headRotationStep;
    std::array<std::int32_t, 2> eyeRotationStep;
    std::int32_t pitchUpper;
    std::int32_t pitchLower;
    std::int32_t headYawUpper;
    std::int32_t headYawLower;
    std::int32_t eyeYawUpper;
    std::int32_t eyeYawLower;
    std::int16_t transitionFrames;
    std::int16_t elapsedFrames;
    std::int32_t reserved94;
};

struct FacialTaskData {
    const std::int16_t *clip;
    std::int32_t clipFrame;
    std::int32_t clipDuration;
    std::int32_t blinkInterval;
    std::int32_t blinkFrame;
    std::int32_t reserved18;
    std::int32_t enabled;
    std::int32_t clipLeadIn;
    std::uint16_t focusId;
    std::uint16_t reserved26;
    FacialEyeControl eyes;
};
#pragma pack(pop)

static_assert(sizeof(FacialModelData) == 0x148);
static_assert(offsetof(FacialModelData, flags) == 0x48);
static_assert(offsetof(FacialModelData, groups) == 0xf8);
static_assert(sizeof(FacialMorphVertex) == 0x34);
static_assert(sizeof(FacialEyeControl) == 0x98);
static_assert(offsetof(FacialEyeControl, headRotation) == 0x34);
static_assert(offsetof(FacialEyeControl, transitionFrames) == 0x90);
static_assert(sizeof(FacialTaskData) == 0xc0);
static_assert(offsetof(FacialTaskData, clip) == 0x00);
static_assert(offsetof(FacialTaskData, clipFrame) == 0x08);
static_assert(offsetof(FacialTaskData, clipDuration) == 0x0c);
static_assert(offsetof(FacialTaskData, blinkInterval) == 0x10);
static_assert(offsetof(FacialTaskData, blinkFrame) == 0x14);
static_assert(offsetof(FacialTaskData, enabled) == 0x1c);
static_assert(offsetof(FacialTaskData, clipLeadIn) == 0x20);
static_assert(offsetof(FacialTaskData, focusId) == 0x24);
static_assert(offsetof(FacialTaskData, eyes) == 0x28);

struct FacialMorphControl {
    FacialMorphVertex vertex;
    const LegacyVertexBufferBinding *buffer{};
    std::uint32_t byteOffset{};
};

struct FacialModelResource {
    FacialModelData data{};
    // Host pointers for the FACE +0x10/+0x14 signed HLib-relative node slots.
    MT7Node *originalNode{};
    MT7Node *replacementNode{};
    std::array<std::vector<FacialMorphControl>, 10> vertices;
};
static_assert(offsetof(FacialModelResource, data) == 0);

bool ReadFacialModelResource(MT7Model &model, std::span<const std::byte> container);
void GraftFacialModelNode(MT7Model &model, FacialModelResource &face);
bool AttachFacialController(Character *character);
bool AssignFacialClip(Character *character, const std::int16_t *clip);
bool IsFacialClipActive(const Character *character);
void SetFacialBlinkDelay(Character *character, std::int32_t delay);
void SetFaceFlagsForChara(Character *character, std::int32_t enabled);
void SetFaceFlagsForChara_0(Character *character, std::int32_t enabled);
void SetHighModelAndFaceFlagsForChara(Character *character, std::int32_t expression, float transitionFrames, float scale);
void SetUpperFacialExpression(Character *character, std::int32_t expression, float transitionFrames, float scale);
void SetLowerFacialExpression(Character *character, std::int32_t expression, float transitionFrames, float scale);
void SetFacialMouthExpression(Character *character, std::int32_t expression, float transitionFrames, float scale);
void StartFacialBlink(Character *character);
void AT_SetEyeCtrl(Character *character, float transitionFrames, float leftWeight, float rightWeight);
void SetEyeToChara(Character *character, const Vector3 *target, std::int16_t frames, std::uint8_t mode);

} // namespace shenmue::s2
