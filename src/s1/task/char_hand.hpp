#pragma once
#include "s1/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
inline constexpr shen_name RESOURCE_HNDB = SHEN_NAME(HNDB);
struct Character;
struct HandAngleKey;
struct HandAngleKeyDescriptor;
struct HandBindTransform;
struct HandCommandState;
struct HandDeformVertex;
struct HandFingerCurveSet;
struct HandFingerNode;
struct HandFingerProbe;
struct HandModelWork;
struct HandMotionEntry;
struct HandMotionModelHeader;
struct HandPlaybackState;
struct HandPoseBlendState;
struct HandPoseRecord;
struct HandPoseWork;
struct HandScaleColumn;
struct HandSkinningAttachmentRecord;
struct Mt5RuntimeNode;
struct PackModel;
struct ScenarioContext;

#pragma pack(push, 1)


struct HandPlaybackState {
    std::int8_t status;
    std::uint8_t playbackMode;
    std::int16_t frameIndex;
    std::int16_t previousFrameIndex;
    std::int16_t startFrame;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandModelWork {
    char filename[0x10];
    Mt5RuntimeNode *primaryRoot;
    void *primaryFile;
    PackModel *primaryOwner;
    Mt5RuntimeNode *secondaryRoot;
    void *secondaryFile;
    PackModel *secondaryOwner;
    void *previousFigurePoseCallback;
    void *activeMotionRecord;
    void *handMotionModel;
    std::uint32_t sequenceDuration;
    std::byte reserved05C[4];
    void *animationCurves;
    std::int16_t jointCount;
    std::int16_t vertexCount;
    std::byte reserved06C[4];
    void *sourceVertices;
    void *mutableVertices;
    std::uint8_t side;
    std::uint8_t animationFlags;
    std::int16_t blendFrames;
    std::byte targetAngleDeltas[0x354];
    std::byte angleDeltas[0x354];
    std::byte jointPositions[0x354];
    void *jointMatrices;
    Character *gripTarget;
    std::int8_t fingerBlendLimits[0x10];
    float frame;
    float frameStep;
    union {
        std::int64_t modelSelector;
        HandPlaybackState playback;
    };
    std::int16_t endFrame;
    std::uint8_t control;
    std::uint8_t secondaryModelRequested;
    std::int32_t handMotionTableIndex;
    Vector3 fingerCenter;
    std::uint32_t scaleTableValid;
    float scaleTable[6];
};
#pragma pack(pop)

static_assert(sizeof(HandModelWork) == 0xae0);
static_assert(offsetof(HandModelWork, primaryRoot) == 0x10);
static_assert(offsetof(HandModelWork, secondaryRoot) == 0x28);
static_assert(offsetof(HandModelWork, previousFigurePoseCallback) == 0x40);
static_assert(offsetof(HandModelWork, activeMotionRecord) == 0x48);
static_assert(offsetof(HandModelWork, handMotionModel) == 0x50);
static_assert(offsetof(HandModelWork, sequenceDuration) == 0x58);
static_assert(offsetof(HandModelWork, animationCurves) == 0x60);
static_assert(offsetof(HandModelWork, jointCount) == 0x68);
static_assert(offsetof(HandModelWork, vertexCount) == 0x6a);
static_assert(offsetof(HandModelWork, sourceVertices) == 0x70);
static_assert(offsetof(HandModelWork, mutableVertices) == 0x78);
static_assert(offsetof(HandModelWork, side) == 0x80);
static_assert(offsetof(HandModelWork, animationFlags) == 0x81);
static_assert(offsetof(HandModelWork, blendFrames) == 0x82);
static_assert(offsetof(HandModelWork, targetAngleDeltas) == 0x84);
static_assert(offsetof(HandModelWork, angleDeltas) == 0x3d8);
static_assert(offsetof(HandModelWork, jointPositions) == 0x72c);
static_assert(offsetof(HandModelWork, jointMatrices) == 0xa80);
static_assert(offsetof(HandModelWork, gripTarget) == 0xa88);
static_assert(offsetof(HandModelWork, fingerBlendLimits) == 0xa90);
static_assert(offsetof(HandModelWork, frame) == 0xaa0);
static_assert(offsetof(HandModelWork, frameStep) == 0xaa4);
static_assert(offsetof(HandModelWork, modelSelector) == 0xaa8);
static_assert(offsetof(HandModelWork, endFrame) == 0xab0);
static_assert(offsetof(HandModelWork, control) == 0xab2);
static_assert(offsetof(HandModelWork, secondaryModelRequested) == 0xab3);
static_assert(offsetof(HandModelWork, handMotionTableIndex) == 0xab4);
static_assert(offsetof(HandModelWork, fingerCenter) == 0xab8);
static_assert(offsetof(HandModelWork, scaleTableValid) == 0xac4);
static_assert(offsetof(HandModelWork, scaleTable) == 0xac8);



struct HandSkinningAttachmentRecord {
    std::uint32_t cacheHandle;
    std::uint32_t reserved04;
    Mt5RuntimeNode *node;
    std::uint32_t nodeHash;
    std::uint32_t matrixCount;
    std::int32_t nodeKind;
    std::uint32_t enabled;
    std::uint32_t ready;
    std::uint32_t slot;
    std::uint32_t boneHashes[20];
    std::uint8_t matrixIndices[24];
    Matrix4x4 *matrices;
    std::uint64_t reserved98;
};

static_assert(sizeof(HandSkinningAttachmentRecord) == 0xa0);
static_assert(offsetof(HandSkinningAttachmentRecord, nodeHash) == 0x10);
static_assert(offsetof(HandSkinningAttachmentRecord, matrixCount) == 0x14);
static_assert(offsetof(HandSkinningAttachmentRecord, nodeKind) == 0x18);
static_assert(offsetof(HandSkinningAttachmentRecord, boneHashes) == 0x28);
static_assert(offsetof(HandSkinningAttachmentRecord, matrixIndices) == 0x78);
static_assert(offsetof(HandSkinningAttachmentRecord, matrices) == 0x90);

void HMDL_CreateHandAttachmentRecords(
    void *node, Character *character, HandSkinningAttachmentRecord *records, std::uint32_t &recordType, std::uint32_t &slotIndex, std::uint32_t flags);
HandModelWork *HND_GetWork(Character *character, bool left);
void AllocateAndLoadArms(
    Character *character, const char *leftFilename, const char *rightFilename, std::uint8_t optionFlags, bool calibrateToCharacter = false);
std::int32_t HND_LoadModels(Character *character);
std::int32_t HND_AcquireMotionModel(const char *filename, std::uint8_t motionKind, void **data, std::size_t *size);
std::size_t HND_MotionModelSize(std::int32_t tableIndex);
void HND_ReleaseMotionModel(std::int32_t tableIndex);
Matrix4x4 *HND_FingerMatrixPool();
void HND_SetNewData(HandModelWork &hand, std::int16_t sequenceId, std::int16_t startFrame, std::int16_t endFrame, std::int16_t initialFrame,
    std::uint8_t control, float frameStep, std::int16_t blendFrames);
void HND_SetFloatsFor(HandModelWork &hand, const std::int32_t *angles, std::int16_t blendFrames);
void HND_SetEffectFlags(HandModelWork &hand, const std::int8_t *pose, float blendFrames);
void HND_ResetMotion(HandModelWork &hand, std::int8_t status, std::int16_t sequenceId);
std::uint8_t HND_ReleaseModels(Character *character);
void HND_ResetVfActorHands(Character *character);
void CHAR_SetHandFaceDetails(std::int32_t mode);
void HND_SetControlFlags(HandModelWork &hand, std::uint16_t enabled, std::uint8_t mask);
void HND_SetRootRotation(HandModelWork &hand, std::uint32_t mask, const Vector3i *rotation);
void HND_ApplyRootEuler(HandModelWork &hand, Vector3i value);
Vector3i HND_GetRootEuler(const HandModelWork &hand);
void ApplyHandRootTransform(Character *character, const HandModelWork &hand);
void HND_ResetMotionModels();
void HND_SetFingerMatrixPool(Matrix4x4 *matrices);
void MHND_Initialize(Character *character);
void mhndApplyCharaHandPose(Character *character, std::int32_t hand, std::int32_t poseIndex, std::int32_t frameCount);
void MHND_ApplyCycleUmbrellaPose(Character *character);
void mhndApplyRawHandPose(Character *character, std::int32_t hand, const std::int32_t *pose, std::int32_t frameCount);
void mhndStartIdleHandProgram(Character *character);
void mhndStartAutomaticHandProgram(Character *character);
std::int32_t mhndGetPoseStatus(Character *character, std::int32_t hand);
void Script_SetHandMotionTargetChara(ScenarioContext *context, void *rawParams);
void Script_RotateAllAxisWrapper(ScenarioContext *context, void *rawParams);
void Script_SetHandEulerRotationMasked(ScenarioContext *context, void *rawParams);

Mt5RuntimeNode *shenGetNodeByChildIdx(Mt5RuntimeNode *node, std::int16_t nodeKind);
#pragma pack(push, 1)

struct HandPoseRecord {
    std::int32_t values[10];
};
#pragma pack(pop)

#pragma pack(push, 1)



struct HandPoseBlendState {
    Mt5RuntimeNode *node;
    std::int32_t hand;
    std::int32_t poseIndex;
    std::int32_t framesRemaining;
    HandPoseRecord current;
    HandPoseRecord target;
    std::int32_t step[10];
    float scratch[6];
    std::uint32_t reservedA4;
};
#pragma pack(pop)

#pragma pack(push, 1)




struct HandCommandState {
    using Callback = void (*)(Character *, HandCommandState &, HandPoseBlendState &);

    std::uint32_t mode;
    std::int32_t command;
    std::int32_t framesRemaining;
    std::uint32_t reserved0c;
    const std::int32_t *cursor;
    const std::int32_t *loopStart;
    const std::int32_t *loopEnd;
    std::int32_t loopCount;
    std::uint32_t reserved2c;
    Callback callback;
    std::int32_t lastPose;
    std::uint32_t reserved3c;
};
#pragma pack(pop)

#pragma pack(push, 1)


struct HandPoseWork {
    HandPoseBlendState right;
    HandPoseBlendState left;
    HandCommandState rightCommand;
    HandCommandState leftCommand;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct HandMotionModelHeader {
    std::uint32_t hierarchyOffset;
    std::uint32_t bindTransformsOffset;
    std::uint32_t influenceCountsOffset;
    std::uint32_t influenceIndicesOffset;
    std::uint32_t influenceWeightsOffset;
    std::uint32_t originalVerticesOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandFingerNode {
    std::uint8_t jointIndex;
    std::uint8_t childCount;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandBindTransform {
    float unused;
    Vector3 value;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandDeformVertex {
    float y;
    float x;
    float z;
    float height;
    float width;
    float depth;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandAngleKeyDescriptor {
    std::int32_t count;
    std::uint32_t offset;
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandFingerCurveSet {
    HandAngleKeyDescriptor axes[3];
};
#pragma pack(pop)

#pragma pack(push, 1)

struct HandAngleKey {
    std::int32_t frame;
    float inTangent;
    float outTangent;
    float angleDegrees;
};
#pragma pack(pop)

struct HandFingerProbe {
    Vector3 start;
    float startRadius;
    Vector3 end;
    float endRadius;
};


struct HandScaleColumn {
    std::int32_t joint;
    std::int32_t column;
};

struct HandMotionEntry {
    char name[4];
    std::uint32_t motionSize;
    void *motion;
    std::uint16_t retained;
    std::uint16_t references;
    std::uint32_t reserved14;
};
} // namespace shenmue::s1
