#pragma once

#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct RenderArrayEntry;

struct Character;
struct FaceClipControllerState;
struct FaceLipCue;
struct FaceWork;

void FACE_Init();
std::int16_t Clipper_Alloc(Character *character);
FaceClipControllerState *CLIP_GetWork(Character *character);
FaceWork *FACE_GetWork(Character *character);
bool FACE_IsBoundToCachedModel(const FaceWork &face);
void Clipper_SetGlobalFlag(std::int32_t value);
std::int32_t Clipper_GetGlobalFlag();
void FACE_ConfigureClip(Character *character, std::int8_t mode, bool flag);
const void *FACE_GetChannelVertices(std::uint16_t channelIndex);
FaceWork *AllocateFACE(Character *character, const char *faceName);
std::int32_t FACE_ScriptFaceMotion(Character *character, std::int32_t mode);
void FACE_ScriptReleaseMotion(Character *character);
void npcReleaseFace(Character *character);
void FACE_SetLcgState(Character *character, std::int32_t mode, std::int32_t interval, std::int32_t flags);
void FACE_UpdateExpression(Character *character, std::int32_t eyeMode, std::int32_t eyePattern, std::int32_t timing);
void FACE_SetLookTarget(Character *character, std::int32_t mode, const Vector3 *target, std::int32_t blendFrames);
void FACE_ApplyScriptMorph(Character *character, std::int32_t channel, std::int32_t morph, std::int32_t timing);
void FACE_ResetCache();
std::uint8_t FACE_FindCacheSlotByModelName(const char *filename);
void faceBindNewPhoneme(FaceClipControllerState *clip);
std::int16_t faceAdvanceSequence(FaceClipControllerState *clip);
void faceApplyMorph(void *model, std::int32_t morphIndex, float speed, float weight);
void FACE_UpdateEyeLookAtModelTarget(void *model, std::uint32_t channel, const Vector3 *origin, const Vector3 *target);
void HMDL_PerformBlink(RenderArrayEntry &entry, std::int32_t side);
void HMDL_PerformFacialExpressions(RenderArrayEntry &entry);
void HMDL_InitCEWP(RenderArrayEntry &entry);
bool faceHasRemainingDuration(Character *character);
bool faceSetGeneratedLipCues(Character *character, FaceLipCue *cues, bool enabled);



#pragma pack(push, 1)
struct FaceWork {
    std::byte faceName[0x10];
    std::byte faceTableName[0x10];
    std::uint64_t field20;
    std::uint64_t field28;
    std::uint32_t field30;
    std::int16_t eyePatternMode;
    std::int16_t frameCount;
    std::int16_t eyeMode;
    std::uint16_t field3a;
    std::uint16_t field3c;
    std::uint16_t field3e;
    std::uint64_t field40;
    std::uint32_t key;
    std::byte reserved4c[4];
    std::uint64_t field50;
    std::uint16_t field58;
    std::uint16_t field5a;
    std::uint32_t field5c;
    std::int8_t field60;
    std::byte reserved61[3];
    std::uint64_t field64;
    std::uint32_t field6c;
    std::uint64_t field70;
    std::uint64_t field78;
    std::byte reserved80[4];
    std::uint64_t field84;
    std::byte reserved8c[4];
    std::uint64_t field90;
    std::byte reserved98[4];
    std::uint64_t field9c;
    Vector3 fieldA4;
    std::byte reservedB0[4];
    std::uint64_t fieldB4;
    std::byte reservedBC[4];
    std::uint64_t fieldC0;
    std::uint64_t fieldC8;
    std::uint64_t fieldD0;
    std::uint64_t fieldD8;
    std::int64_t fieldE0;
    std::uint16_t fieldE8;
    std::byte reservedEA[6];
    std::uint64_t fieldF0;
    std::int64_t fieldF8;
    std::uint32_t field100;
    std::byte reserved104[4];
    std::uint32_t field108;
    std::byte reserved10C[4];
    std::uint64_t field110;
    std::uint64_t field118;
    std::uint64_t field120;
    std::uint64_t field128;
};

struct FaceLipCue {
    std::int16_t viseme;
    std::int16_t field02;
    std::int16_t duration;
    std::int16_t field06;
};

struct FaceClipControllerState {
    FaceLipCue *generatedLipCues;
    std::uint8_t enabled;
    std::byte reserved09[7];
    void (*characterFaceHandler)(Character *);
    std::uint16_t field18;
    std::uint16_t field1a;
    std::int32_t field1c;
    void *clip;
    std::uint8_t startState;
    std::uint8_t field29;
    std::uint8_t alternatingShape;
    std::uint8_t activeSequenceState;
    std::int16_t shapeDuration;
    std::int16_t blendDuration;
    std::uint16_t halfDuration;
    std::int16_t field32;
    std::int16_t visemeIndex;
    std::uint16_t channelTag;
    FaceLipCue *activeLipCues;
    std::uint16_t cueIndex;
    std::uint16_t field42;
    std::int32_t field44;
    std::uint32_t rotationY;
    std::int64_t field4c;
    std::int8_t field54;
    std::uint8_t reserved55;
    std::int8_t morphComplete;
    std::byte reserved57[3];
    std::uint16_t channelIndex;
    std::uint32_t reserved5c;
    void *channelVertices;
};

struct FtblPrologue {
    std::uint32_t fileKindOrVariant;
    std::uint32_t channelBlockOffset;
    std::uint32_t blendHeaderOffset;
    std::int32_t tuning[6];
};

struct FtblBlendHeader {
    std::int32_t countsOffset;
    std::int32_t indicesOffset;
    std::int32_t weightsOffset;
};

struct FtblChannelRigRecord {
    Vector3 restPosition;
    std::uint32_t solverKind;
    Vector3 targetPosition;
    Vector3 euler;
    float firstSegmentLength;
    float secondSegmentLength;
};

struct FmotProgramHeader {
    std::int32_t eyeChannelTableOffset;
    std::int32_t mouthChannelTableOffset;
    std::uint32_t keyRecordCount;
    std::int32_t nextProgramOffset;
};

struct FmotChannelDescriptor {
    std::int16_t keyCountX;
    std::int16_t keyIndexX;
    std::int16_t keyCountY;
    std::int16_t keyIndexY;
    std::int16_t keyCountZ;
    std::int16_t keyIndexZ;
};

struct FmotChannelKey {
    std::int16_t frame;
    std::uint16_t incomingTangent;
    std::uint16_t outgoingTangent;
    std::uint16_t value;
};
#pragma pack(pop)



struct FaceMotionProgram {
    std::byte *program;
    std::uint32_t keyRecordCount;
    const FmotChannelDescriptor *eyeChannels;
    const FmotChannelDescriptor *mouthChannels;
};



struct FaceCalcContext {
    const FtblChannelRigRecord *channelRigs;
    std::uint32_t blendFrameCount;
    const void *variantA;
    const FmotChannelDescriptor *eyeChannels;
    const FmotChannelDescriptor *mouthChannels;
    std::int32_t channelCount;
    std::int32_t b;
    const std::byte *program;
};



struct FaceTableBinding {
    const FtblChannelRigRecord *channelRigs;
    FtblBlendHeader *blendHeader;
    const std::int32_t *influenceCounts;
    const std::int32_t *influenceIndices;
    const float *influenceWeights;
};

static_assert(sizeof(FaceWork) == 0x130);
static_assert(offsetof(FaceWork, eyePatternMode) == 0x34);
static_assert(offsetof(FaceWork, field40) == 0x40);
static_assert(offsetof(FaceWork, fieldA4) == 0xa4);
static_assert(offsetof(FaceWork, field118) == 0x118);
static_assert(offsetof(FaceWork, field110) == 0x110);
static_assert(offsetof(FaceWork, field120) == 0x120);
static_assert(offsetof(FaceWork, field128) == 0x128);
static_assert(sizeof(FaceLipCue) == 8);
static_assert(sizeof(FaceClipControllerState) == 0x68);
static_assert(offsetof(FaceClipControllerState, clip) == 0x20);
static_assert(offsetof(FaceClipControllerState, activeLipCues) == 0x38);
static_assert(offsetof(FaceClipControllerState, channelIndex) == 0x5a);
static_assert(offsetof(FaceClipControllerState, channelVertices) == 0x60);
static_assert(sizeof(FaceCalcContext) == 0x38);
static_assert(offsetof(FaceCalcContext, channelCount) == 0x28);
static_assert(offsetof(FaceCalcContext, program) == 0x30);
static_assert(sizeof(FaceTableBinding) == 0x28);
static_assert(sizeof(FtblPrologue) == 0x24);
static_assert(sizeof(FtblBlendHeader) == 0x0c);
static_assert(sizeof(FtblChannelRigRecord) == 0x30);
static_assert(sizeof(FmotProgramHeader) == 0x10);
static_assert(sizeof(FmotChannelDescriptor) == 0x0c);
static_assert(sizeof(FmotChannelKey) == 8);
static_assert(sizeof(FaceMotionProgram) == 0x20);

} // namespace shenmue::s1
